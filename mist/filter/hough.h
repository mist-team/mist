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
//! @brief Hough�ϊ����s�����C�u����
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

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

namespace __hough_detail__
{
	typedef std::multimap< size_t, std::complex< int >, std::greater< size_t > > hough_counter;

	// �O�p�֐��v�Z�̍������̂��߁A���炩���߃e�[�u�����쐬����
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
		accumulator( size_type rho_size, size_type angle_size ) : data_( angle_size + 2, rho_size + 2 )  // �O��1���L�����ʂ�p�ӂ���BisPeakCell()�Œ���Pixel�̏㉺���E���A�N�Z�X���邽�߁A1Pixel���傫�����Ă���
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


//! @addtogroup hough_group �n�t�ϊ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/hough.h>
//! @endcode
//!
//!  @{


/// @brief �����ɑ΂���Hough�ϊ�
namespace line
{
	/// @brief Hough �ϊ��i�����j
	//!
	//! @param[in]  input            �c ���͉摜
	//! @param[out] lines            �c ���܂��������Q�̃��X�g�i�����͋ɍ��W�ŕ\�������j.
	//! @param[in]  max_lines        �c ���o���钼���̍ő�{��.
	//! @param[in]  rho_resolution   �c �ϕ���\ [pixel]
	//! @param[in]  theta_resolution �c �ƕ���\ [radian]
	//! @param[in]  threshold        �c ���𒊏o����ۂ̂������l�i�Ή����铊�[�����������l���傫���ꍇ�̂ݐ��Ƃ��Ē��o����j.
	//! @param[in]  value_functor    �c ���͉摜�����璼����\����f���ǂ����𔻒肷��t�@���N�^.
	//! 
	//! @retval true  �c Hough�ϊ��ɂ�蒼�������܂����ꍇ
	//! @retval false �c ������1�{�����܂�Ȃ������ꍇ
	//!
	template < class T, class Allocator, template < typename, typename > class LINES, class TT, class AAllocator, class FUNCTOR >
	bool hough_transform( const array2< T, Allocator >& input, LINES< TT, AAllocator > &lines, std::size_t max_lines, double rho_resolution, double theta_resolution, size_t threshold, FUNCTOR value_functor )
	{
		//�O�p�֐��e�[�u�����쐬
		__hough_detail__::trigonometric_table table = __hough_detail__::trigonometric_table( rho_resolution, theta_resolution );

		// Hough�ϊ�...��-�ƕ��ʂ�����
		__hough_detail__::accumulator accumulator = __hough_detail__::hough_transform( input, rho_resolution, table, value_functor );

		// ��-�ƕ��ʂ�Count�̑������̂��珇�Ɏ��o��
		__hough_detail__::hough_counter counter;
		accumulator.convert_to_counter( counter, threshold );

		// �tHough�ϊ�
		__hough_detail__::hough_inverse( counter, lines, rho_resolution, theta_resolution, max_lines );

		return( !lines.empty( ) );
	}

	/// @brief Hough �ϊ��i�����j
	//!
	//! @param[in]  input            �c ���͉摜
	//! @param[out] lines            �c ���܂��������Q�̃��X�g�i�����͋ɍ��W�ŕ\�������j.
	//! @param[in]  max_lines        �c ���o���钼���̍ő�{��.
	//! @param[in]  rho_resolution   �c �ϕ���\ [pixel]�i�f�t�H���g�l 1.0�j
	//! @param[in]  theta_resolution �c �ƕ���\ [radian]�i�f�t�H���g�l �΁�360�j
	//! @param[in]  threshold        �c ���𒊏o����ۂ̂������l�i�f�t�H���g�l 100�j.
	//! 
	//! @retval true  �c Hough�ϊ��ɂ�蒼�������܂����ꍇ
	//! @retval false �c ������1�{�����܂�Ȃ������ꍇ
	//!
	template < class T, class Allocator, template < typename, typename > class LINES, class TT, class AAllocator >
	bool hough_transform( const array2< T, Allocator >& input, LINES< TT, AAllocator > &lines, std::size_t max_lines, double rho_resolution = 1.0, double theta_resolution = 3.1415926535897932384626433832795 / 360.0, size_t threshold = 100 )
	{
		return( hough_transform( input, lines, max_lines, rho_resolution, theta_resolution, threshold, __hough_detail__::foreground_evaluator( ) ) );
	}
}

/// @}
//  �n�t�ϊ��O���[�v�̏I���

// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_HOUGH__
