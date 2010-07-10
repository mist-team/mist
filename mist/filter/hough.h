// 
// Copyright (c) 2003-2010, Shuichirou Kitou, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/// @file mist/hough.h
//!
//! @brief Hough変換を行うライブラリ
//!

#ifndef __INCLUDE_MIST_HOUGH__
#define __INCLUDE_MIST_HOUGH__

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#include <complex>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN

namespace __hough_detail__
{
	typedef std::multimap< size_t, std::complex< int >, std::greater< size_t > > hough_counter;

	// 三角関数計算の高速化のため、あらかじめテーブルを作成する
	class trigonometric_table
	{
	public:
		typedef size_t size_type;

	private:
		size_type size_;
		array< double > cos_table_;
		array< double > sin_table_;

	public:
		double sin( size_type angle ) const { return( sin_table_[ angle ] ); }
		double cos( size_type angle ) const { return( cos_table_[ angle ] ); }
		size_type size( )             const { return( size_ ); }

	public:
		trigonometric_table( double rho_resolution, double theta_resolution )
			: size_( static_cast< size_type >( std::fabs( 3.1415926535897932384626433832795 / theta_resolution ) + 0.5 ) ), cos_table_( size_ ), sin_table_( size_ )
		{
			const double rho_inverse = 1.0 / rho_resolution;
			double angle = 0.0;

			for( size_type i = 0 ; i < size_ ; angle += theta_resolution, i++ )
			{
				sin_table_[ i ] = std::sin( angle ) * rho_inverse;
				cos_table_[ i ] = std::cos( angle ) * rho_inverse;
			}
		}

		~trigonometric_table( )
		{
		}
	};

	class accumulator
	{
	public:
		typedef array2< size_t >           data_type;
		typedef data_type::size_type       size_type;
		typedef data_type::difference_type difference_type;

	private:
		data_type data_;

	public:
		accumulator( size_type rho_size, size_type angle_size ) : data_( angle_size + 2, rho_size + 2 )  // 外周1ずつ広い平面を用意する。isPeakCell()で注目Pixelの上下左右をアクセスするため、1Pixelずつ大きくしておく
		{
		}

		~accumulator( )
		{
		}

		void count_up( difference_type rho, size_type angle )
		{
			this->operator ()( static_cast< difference_type >( rho + ( get_rho_size( ) - 1 ) / 2 ), angle ) ++;
		}

		void convert_to_counter( hough_counter &c, size_type threshold ) const
		{
			int rho_size = static_cast< int >( get_rho_size( ) );
			size_type angle_size = get_angle_size( );

#ifdef _OPENMP
			#pragma omp parallel for schedule( guided )
#endif
			for( int rho = 0 ; rho < rho_size ; ++rho )
			{
				for( size_type angle = 0 ; angle < angle_size ; ++angle )
				{
					size_type count = at( rho, angle );

					if( ( count > threshold ) && is_peak_cell( rho, angle ) )
					{
#ifdef _OPENMP
						#pragma omp critical
#endif
						{
							c.insert( std::make_pair( count, std::complex< int >( rho - ( rho_size - 1 ) / 2, angle ) ) );
						}
					}
				}
			}
		}

	private:
		size_type & operator ()( size_type rho, size_type angle )
		{
			return( data_( angle + 1, rho + 1 ) );
		}

		size_type operator ()( size_type rho, size_type angle ) const
		{
			return( data_( angle + 1, rho + 1 ) );
		}

		size_type & at( size_type rho, size_type angle )
		{
			return( this->operator ()( rho, angle ) );
		}

		size_type at( size_type rho, size_type angle ) const
		{
			return( this->operator ()( rho, angle ) );
		}

		size_type get_rho_size( ) const
		{
			return( data_.size2( ) - 2 );
		}

		size_type get_angle_size( ) const
		{
			return( data_.size1( ) - 2 );
		}

		bool is_peak_cell( size_type rho, size_type angle ) const
		{
			size_type level = at( rho, angle );

			return( level >  at( rho - 1, angle ) &&
					level >= at( rho + 1, angle ) &&
					level >  at( rho, angle - 1 ) &&
					level >= at( rho, angle + 1 ) );
		}
	};

	template < class T, class Allocator, class FUNCTOR >
	accumulator hough_transform( const array2< T, Allocator >& input, double rho_resolution, const trigonometric_table & table, FUNCTOR f )
	{
		typedef typename array2< T, Allocator >::size_type       size_type;
		typedef typename array2< T, Allocator >::difference_type difference_type;
		typedef typename array2< T, Allocator >::value_type      value_type;

		const size_type angle_size = table.size( );
		const size_type rho_size   = static_cast< size_type >( ( ( input.width( ) + input.height( ) ) * 2 + 1 ) / rho_resolution );

		accumulator accumulator( rho_size, angle_size );

		int height = static_cast< int >( input.height( ) );

#ifdef _OPENMP
		#pragma omp parallel for schedule( guided )
#endif
		for( int j = 0 ; j < height ; j++ )
		{
			for( size_type i = 0 ; i < input.width( ) ; i++ )
			{
				if( f( input( i, j ) ) )
				{
#ifdef _OPENMP
					#pragma omp critical
#endif
					for( size_type angle = 0 ; angle < angle_size ; ++angle )
					{
						difference_type rho = static_cast< difference_type >( i * table.cos( angle ) + j * table.sin( angle ) + 0.5 );
						accumulator.count_up( rho, angle );
					}
				}
			}
		}

		return( accumulator );
	}

	template < template < typename, typename > class LINES, class TT, class AAllocator >
	void hough_inverse( const hough_counter & counter, LINES< TT, AAllocator > &lines, double rho_resolution, double theta_resolution, size_t max_lines )
	{
		typedef typename LINES< TT, AAllocator >::value_type value_type;

		lines.clear( );

		for( hough_counter::const_iterator ite = counter.begin( ) ; ite != counter.end( ) && lines.size( ) < max_lines ; ++ite )
		{
			double rho   = ite->second.real( ) * rho_resolution;
			double theta = ite->second.imag( ) * theta_resolution;
			lines.push_back( value_type( rho, theta ) );
		}
	}

	class foreground_evaluator
	{
	public:
		template < class T >
		bool operator ()( const T &val ) const
		{
			return( val != T( ) );
		}
	};

	class background_evaluator
	{
	public:
		template < class T >
		bool operator ()( const T &val ) const
		{
			return( val == T( ) );
		}
	};
} // namespace __hough_detail__


//! @addtogroup hough_group ハフ変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/hough.h>
//! @endcode
//!
//!  @{


/// @brief 直線に対するHough変換
namespace line
{
	/// @brief Hough 変換（直線）
	//!
	//! @param[in]  input            … 入力画像
	//! @param[out] lines            … 求まった直線群のリスト（直線は極座標で表現される）.
	//! @param[in]  max_lines        … 抽出する直線の最大本数.
	//! @param[in]  rho_resolution   … ρ分解能 [pixel]
	//! @param[in]  theta_resolution … θ分解能 [radian]
	//! @param[in]  threshold        … 線を抽出する際のしきい値（対応する投票数がしきい値より大きい場合のみ線として抽出する）.
	//! @param[in]  value_functor    … 入力画像中から直線を表す画素かどうかを判定するファンクタ.
	//! 
	//! @retval true  … Hough変換により直線が求まった場合
	//! @retval false … 直線が1本も求まらなかった場合
	//!
	template < class T, class Allocator, template < typename, typename > class LINES, class TT, class AAllocator, class FUNCTOR >
	bool hough_transform( const array2< T, Allocator >& input, LINES< TT, AAllocator > &lines, std::size_t max_lines, double rho_resolution, double theta_resolution, size_t threshold, FUNCTOR value_functor )
	{
		//三角関数テーブルを作成
		__hough_detail__::trigonometric_table table = __hough_detail__::trigonometric_table( rho_resolution, theta_resolution );

		// Hough変換...ρ-θ平面をつくる
		__hough_detail__::accumulator accumulator = __hough_detail__::hough_transform( input, rho_resolution, table, value_functor );

		// ρ-θ平面でCountの多いものから順に取り出す
		__hough_detail__::hough_counter counter;
		accumulator.convert_to_counter( counter, threshold );

		// 逆Hough変換
		__hough_detail__::hough_inverse( counter, lines, rho_resolution, theta_resolution, max_lines );

		return( !lines.empty( ) );
	}

	/// @brief Hough 変換（直線）
	//!
	//! @param[in]  input            … 入力画像
	//! @param[out] lines            … 求まった直線群のリスト（直線は極座標で表現される）.
	//! @param[in]  max_lines        … 抽出する直線の最大本数.
	//! @param[in]  rho_resolution   … ρ分解能 [pixel]（デフォルト値 1.0）
	//! @param[in]  theta_resolution … θ分解能 [radian]（デフォルト値 π÷360）
	//! @param[in]  threshold        … 線を抽出する際のしきい値（デフォルト値 100）.
	//! 
	//! @retval true  … Hough変換により直線が求まった場合
	//! @retval false … 直線が1本も求まらなかった場合
	//!
	template < class T, class Allocator, template < typename, typename > class LINES, class TT, class AAllocator >
	bool hough_transform( const array2< T, Allocator >& input, LINES< TT, AAllocator > &lines, std::size_t max_lines, double rho_resolution = 1.0, double theta_resolution = 3.1415926535897932384626433832795 / 360.0, size_t threshold = 100 )
	{
		return( hough_transform( input, lines, max_lines, rho_resolution, theta_resolution, threshold, __hough_detail__::foreground_evaluator( ) ) );
	}
}

/// @}
//  ハフ変換グループの終わり

// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_HOUGH__
