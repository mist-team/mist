// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
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

/// @file mist/filter/mode.h
//!
//! @brief 各次元の画像に対応し，任意の構造要素に対応した２値画像に対する最頻値フィルタ（多数決フィルタ）を行うためのライブラリ
//!
//! @attention 構造要素の形は凸形状でなくてはならない
//!

#ifndef __INCLUDE_MIST_MODE__
#define __INCLUDE_MIST_MODE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#include <vector>
#include <cmath>


// mist名前空間の始まり
_MIST_BEGIN



//! @addtogroup mode_group 最頻値フィルタ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/mode.h>
//! @endcode
//!
//!  @{


/// @brief 最頻値フィルタに利用する構造要素関連
//! 
//! 最頻値フィルタに利用する構造要素を作成するための関数・構造要素を定義している
//! 
namespace mode_filter
{
	/// @brief 構造要素の各要素の座標を表す構造体
	struct point
	{
		typedef ptrdiff_t difference_type;	///< @brief 符号付整数型
		difference_type x;					///< @brief X軸方向の座標値
		difference_type y;					///< @brief Y軸方向の座標値
		difference_type z;					///< @brief Z軸方向の座標値

		/// @brief コンストラクタ
		//! 
		//! @param[in] xx … X座標値
		//! @param[in] yy … Y座標値
		//! @param[in] zz … Z座標値
		//! 
		point( difference_type xx, difference_type yy, difference_type zz ) : x( xx ), y( yy ), z( zz )
		{
		}
	};


	/// @brief 最頻値フィルタに利用する構造要素
	//! 
	//! 本構造体を，最頻値フィルタ関数に渡す．
	//! 本データ構造によって，任意の凸形状の構造要素の最頻値フィルタが実行可能．
	//! 
	struct mode_structure
	{
		typedef std::vector< point > list_type;		///< @brief 構造要素内の座標値を格納するリスト型
		list_type object;							///< @brief 構造要素内の全要素の座標値を格納するリスト
		list_type update_in;						///< @brief 構造要素をX軸方向に1画素移動した時に，新規に構造要素に含まれる要素の座標値を格納するリスト
		list_type update_out;						///< @brief 構造要素をX軸方向に1画素移動した時に，新規に構造要素に含まれる要素の座標値を格納するリスト
		size_t margin_x;							///< @brief X軸方向で構造要素の中心から端までの距離で大きい側の距離
		size_t margin_y;							///< @brief Y軸方向で構造要素の中心から端までの距離で大きい側の距離
		size_t margin_z;							///< @brief Z軸方向で構造要素の中心から端までの距離で大きい側の距離
	};


	/// @brief 最頻値フィルタに用いる円構造要素
	//!
	//! resoX，resoY の値を変えることで，楕円等を表現可能です
	//!
	//! @param[in] radius … 半径（単位は画素）
	//! @param[in] resoX  … X軸方向の解像度
	//! @param[in] resoY  … Y軸方向の解像度
	//!
	//! @return 円構造要素
	//!
	inline mode_structure circle( double radius, double resoX, double resoY )
	{
		typedef array2< bool >::size_type size_type;
		typedef array2< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double xx, yy, rr = radius * radius;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type x, y;

		size_type ox = rx + 1;
		size_type oy = ry + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;

		mode_structure s;
		array2< bool > m( w, h );

		// 円の構造要素を作成する
		for( y = -ry ; y <= ry ; y++ )
		{
			yy = y * y * ay * ay;
			for( x = -rx ; x <= rx ; x++ )
			{
				xx = x * x * ax * ax;
				if( xx + yy <= rr )
				{
					m( x + ox, y + oy ) = true;
				}
			}
		}

		// 円の構造要素の各点を設定する
		for( y = -ry ; y <= ry ; y++ )
		{
			for( x = -rx ; x <= rx ; x++ )
			{
				if( m( x + ox, y + oy ) )
				{
					s.object.push_back( point( x, y, 0 ) );
					if( !m( x + ox + 1, y + oy ) )
					{
						s.update_in.push_back( point( x, y, 0 ) );
					}
					if( !m( x + ox - 1, y + oy ) )
					{
						s.update_out.push_back( point( x - 1, y, 0 ) );
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = 0;

		return( s );
	}



	/// @brief 最頻値フィルタに用いる球構造要素
	//!
	//! resoX，resoY，resoZ の値を変えることで，楕円体等を表現可能です
	//!
	//! @param[in] radius … 半径（単位は画素）
	//! @param[in] resoX  … X軸方向の解像度
	//! @param[in] resoY  … Y軸方向の解像度
	//! @param[in] resoZ  … Z軸方向の解像度
	//!
	//! @return 球構造要素
	//!
	inline mode_structure sphere( double radius, double resoX, double resoY, double resoZ )
	{
		typedef array3< bool >::size_type size_type;
		typedef array3< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;
		max_reso = max_reso > resoZ ? max_reso : resoZ;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double az = resoZ / max_reso;
		double xx, yy, zz, rr = radius * radius;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type rz = static_cast< size_type >( ceil( radius / az ) );
		difference_type x, y, z;

		size_type ox = rx + 1;
		size_type oy = ry + 1;
		size_type oz = rz + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;
		size_type d = 2 * oz + 1;

		mode_structure s;
		array3< bool > m( w, h, d );

		// 球の構造要素を作成する
		for( z = -rz ; z <= rz ; z++ )
		{
			zz = z * z * az * az;
			for( y = -ry ; y <= ry ; y++ )
			{
				yy = y * y * ay * ay;
				for( x = -rx ; x <= rx ; x++ )
				{
					xx = x * x * ax * ax;
					if( xx + yy + zz <= rr )
					{
						m( x + ox, y + oy, z + oz ) = true;
					}
				}
			}
		}

		// 球の構造要素の各点を設定する
		for( z = -rz ; z <= rz ; z++ )
		{
			for( y = -ry ; y <= ry ; y++ )
			{
				for( x = -rx ; x <= rx ; x++ )
				{
					if( m( x + ox, y + oy, z + oz ) )
					{
						s.object.push_back( point( x, y, z ) );
						if( !m( x + ox + 1, y + oy, z + oz ) )
						{
							s.update_in.push_back( point( x, y, z ) );
						}
						if( !m( x + ox - 1, y + oy, z + oz ) )
						{
							s.update_out.push_back( point( x - 1, y, z ) );
						}
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = rz;

		return( s );
	}



	/// @brief 最頻値フィルタに用いる正方形構造要素
	//!
	//! resoX，resoY，resoZ の値を変えることで，長方形等を表現可能です
	//!
	//! @param[in] radius … 正方形の一辺の長さの半分（単位は画素）
	//! @param[in] resoX  … X軸方向の解像度
	//! @param[in] resoY  … Y軸方向の解像度
	//!
	//! @return 正方形構造要素
	//!
	inline mode_structure square( double radius, double resoX, double resoY )
	{
		using namespace std;
		typedef array2< bool >::size_type size_type;
		typedef array2< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double xx, yy;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type x, y;

		size_type ox = rx + 1;
		size_type oy = ry + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;

		mode_structure s;
		array2< bool > m( w, h );

		// 正方形の構造要素を作成する
		for( y = -ry ; y <= ry ; y++ )
		{
			yy = y * ay;
			for( x = -rx ; x <= rx ; x++ )
			{
				xx = x * ax;
				if( std::abs( xx ) <= radius && std::abs( yy ) <= radius )
				{
					m( x + ox, y + oy ) = true;
				}
			}
		}

		// 正方形の構造要素の各点を設定する
		for( y = -ry ; y <= ry ; y++ )
		{
			for( x = -rx ; x <= rx ; x++ )
			{
				if( m( x + ox, y + oy ) )
				{
					s.object.push_back( point( x, y, 0 ) );
					if( !m( x + ox + 1, y + oy ) )
					{
						s.update_in.push_back( point( x, y, 0 ) );
					}
					if( !m( x + ox - 1, y + oy ) )
					{
						s.update_out.push_back( point( x - 1, y, 0 ) );
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = 0;

		return( s );
	}


	/// @brief 最頻値フィルタに用いる立方体構造要素
	//!
	//! resoX，resoY，resoZ の値を変えることで，直方体等を表現可能です
	//!
	//! @param[in] radius … 直方体の一辺の長さの半分（単位は画素）
	//! @param[in] resoX  … X軸方向の解像度
	//! @param[in] resoY  … Y軸方向の解像度
	//! @param[in] resoZ  … Z軸方向の解像度
	//!
	//! @return 立方体構造要素
	//!
	inline mode_structure cube( double radius, double resoX, double resoY, double resoZ )
	{
		using namespace std;
		typedef array3< bool >::size_type size_type;
		typedef array3< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;
		max_reso = max_reso > resoZ ? max_reso : resoZ;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double az = resoZ / max_reso;
		double xx, yy, zz;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type rz = static_cast< size_type >( ceil( radius / az ) );
		difference_type x, y, z;

		size_type ox = rx + 1;
		size_type oy = ry + 1;
		size_type oz = rz + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;
		size_type d = 2 * oz + 1;

		mode_structure s;
		array3< bool > m( w, h, d );

		// 立方体の構造要素を作成する
		for( z = -rz ; z <= rz ; z++ )
		{
			zz = z * az;
			for( y = -ry ; y <= ry ; y++ )
			{
				yy = y * ay;
				for( x = -rx ; x <= rx ; x++ )
				{
					xx = x * ax;
					if( std::abs( xx ) <= radius && std::abs( yy ) <= radius && std::abs( zz ) <= radius )
					{
						m( x + ox, y + oy, z + oz ) = true;
					}
				}
			}
		}

		// 立方体の構造要素の各点を設定する
		for( z = -rz ; z <= rz ; z++ )
		{
			for( y = -ry ; y <= ry ; y++ )
			{
				for( x = -rx ; x <= rx ; x++ )
				{
					if( m( x + ox, y + oy, z + oz ) )
					{
						s.object.push_back( point( x, y, z ) );
						if( !m( x + ox + 1, y + oy, z + oz ) )
						{
							s.update_in.push_back( point( x, y, z ) );
						}
						if( !m( x + ox - 1, y + oy, z + oz ) )
						{
							s.update_out.push_back( point( x - 1, y, z ) );
						}
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = rz;

		return( s );
	}



	/// @brief 任意の画像から，最頻値フィルタ用の構造要素を作成する
	//!
	//! @attention resoX，resoY，resoZ の値は無視される
	//!
	//! @param[in] in … 構造要素の画像
	//! @param[in] cx … 構造要素の中心を表すX座標値
	//! @param[in] cy … 構造要素の中心を表すY座標値
	//! @param[in] cz … 構造要素の中心を表すZ座標値
	//!
	//! @return 最頻値フィルタ用の構造要素
	//!
	template < class Array >
	inline mode_structure create_mode_structure( const Array &in, typename Array::size_type cx, typename Array::size_type cy = 0, typename Array::size_type cz = 0 )
	{
		using namespace std;
		typedef typename Array::size_type size_type;
		typedef typename Array::difference_type difference_type;
		difference_type x, y, z;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		mode_structure s;
		marray< Array > m( in, 1 );

		// 構造要素の形を作成する
		for( z = 0 ; z < d ; z++ )
		{
			for( y = 0 ; y < h ; y++ )
			{
				for( x = 0 ; x < w ; x++ )
				{
					m( x, y, z ) = in( x, y, z ) == 0 ? false : true;
				}
			}
		}

		// 構造要素の各点を設定する
		for( z = 0 ; z < d ; z++ )
		{
			for( y = 0 ; y < h ; y++ )
			{
				size_t life = 0;
				for( x = 0 ; x < w ; x++ )
				{
					if( m( x, y, z ) )
					{
						s.object.push_back( point( x - cx, y - cy, z - cz ) );
						if( !m( x + 1, y, z ) )
						{
							s.update_in.push_back( point( x - cx, y - cy, z - cz ) );
						}
						if( !m( x - 1, y, z ) )
						{
							s.update_out.push_back( point( x - cx - 1, y - cy, z - cz ) );
						}
					}
				}
			}
		}

		s.margin_x = cx > w - cx - 1 ? cx : w - cx;
		s.margin_y = cy > h - cy - 1 ? cy : h - cy;
		s.margin_z = cz > d - cz - 1 ? cz : d - cz;

		return( s );
	}


	/// @brief 各画像において，構造要素の中心と各要素間のポインタの差を計算し，そのリストを作成する
	//!
	//! @attention MISTの内部関数として利用
	//!
	//! @param[in] in   … 入力画像
	//! @param[in] list … 構造要素の座標リスト
	//!
	//! @return 構造要素の中心と各要素間のポインタの差のリスト
	//!
	template < class Array >
	inline std::vector< ptrdiff_t > create_pointer_diff_list( const Array &in, const std::vector< point > &list )
	{
		typedef typename Array::size_type     size_type;
		typedef typename Array::const_pointer const_pointer;
		size_type cx = in.width( ) / 2;
		size_type cy = in.height( ) / 2;
		size_type cz = in.depth( ) / 2;
		const_pointer p = &( in( cx, cy, cz ) );

		std::vector< ptrdiff_t > out;

		for( size_type i = 0 ; i < list.size( ) ; i++ )
		{
			const point &pt = list[ i ];
			const_pointer pp = &in( cx + pt.x, cy + pt.y, cz + pt.z );
			out.push_back( pp - p );
		}

		return( out );
	}
}


/// @}
//  最頻値フィルタグループの終わり


namespace __mode__
{
	template < class Array1, class Array2, class Functor >
	void mode( const Array1 &in, Array2 &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f )
	{
		typedef typename Array1::size_type       size_type;
		typedef typename Array1::value_type      value_type;
		typedef typename Array1::const_pointer   const_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type      out_value_type;
		typedef typename Array2::value_type      out_value_type;
		typedef std::vector< ptrdiff_t >		 list_type;
		typedef list_type::const_iterator        const_iterator;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		const bool bprogress1 = thread_idy == 0 && d == 1;
		const bool bprogress2 = thread_idz == 0 && d > 1;

		for( size_type k = thread_idz ; k < d ; k += thread_numz )
		{
			for( size_type j = thread_idy ; j < h ; j += thread_numy )
			{
				const_pointer p = &in( 0, j, k );
				difference_type count[ 2 ] = { 0, 0 };
				for( const_iterator ite = object.begin( ) ; ite != object.end( ) ; ++ite )
				{
					count[ p[ *ite ] > 0 ]++;
				}

				out( 0, j, k ) = static_cast< out_value_type >( count[ 0 ] > count[ 1 ] ? 0 : 1 );

				for( size_type i = 1 ; i < w ; i++ )
				{
					p = &in( i, j, k );

					// 構造要素から抜けた分だけ減算する
					for( const_iterator ite = update_out.begin( ) ; ite != update_out.end( ) ; ++ite )
					{
						count[ p[ *ite ] > 0 ]--;
					}

					// 構造要素に新しく入った分を加える
					for( const_iterator ite = update_in.begin( ) ; ite != update_in.end( ) ; ++ite )
					{
						count[ p[ *ite ] > 0 ]++;
					}

					out( i, j, k ) = static_cast< out_value_type >( count[ 0 ] > count[ 1 ] ? 0 : 1 );
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 );
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 );
			}
		}
	}
}




// 最頻値フィルタのスレッド実装
namespace __mode_controller__
{
	// 各次元をラッピングするための関数
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array< T1, Allocator1 > > &in, array< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array1< T1, Allocator1 > > &in, array1< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array1< T1, Allocator1 >::size_type thread_id, typename array1< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array2< T1, Allocator1 > > &in, array2< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, thread_id, thread_num, 0, 1, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array3< T1, Allocator1 > > &in, array3< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, 0, 1, thread_id, thread_num, f );
	}


	template < class T1, class T2, class Functor >
	class mode_thread : public mist::thread< mode_thread< T1, T2, Functor > >
	{
	public:
		typedef mist::thread< mode_thread< T1, T2, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;
		typedef typename T1::difference_type difference_type;
		typedef std::vector< difference_type >  list_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const T1 *in_;
		T2 *out_;
		list_type *object_;
		list_type *update_in_;
		list_type *update_out_;

		Functor f_;

	public:
		void setup_parameters( const T1 &in, T2 &out, list_type &object, list_type &update_in, list_type &update_out, size_type thread_id, size_type thread_num, Functor f )
		{
			in_  = &in;
			out_ = &out;
			object_ = &object;
			update_in_ = &update_in;
			update_out_ = &update_out;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
			f_ = f;
		}

		const mode_thread& operator =( const mode_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				out_ = p.out_;
				object_ = p.object_;
				update_in_ = p.update_in_;
				update_out_ = p.update_out_;
				f_ = p.f_;
			}
			return( *this );
		}

		mode_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), object_( NULL ), update_in_( NULL ),  update_out_( NULL )
		{
		}
		mode_thread( const mode_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
												in_( p.in_ ), out_( p.out_ ), object_( p.object_ ), update_in_( p.update_in_ ), update_out_( p.update_out_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			mode( *in_, *out_, *object_, *update_in_, *update_out_, thread_id_, thread_num_, f_ );
			return( true );
		}
	};
}


//! @addtogroup mode_group 最頻値フィルタ
//!  @{


/// @brief 任意の構造要素に対応した最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] s          … 最頻値フィルタに用いる構造要素
//! @param[in] f          … 進行状況を返すコールバック関数
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator, class Functor >
bool mode( array2< T, Allocator > &in, const mode_filter::mode_structure &s, Functor f, typename array2< T, Allocator >::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename array2< T, Allocator >::value_type value_type;
	typedef typename array2< T, Allocator >::size_type  size_type;
	typedef typename array2< T, Allocator >::difference_type  difference_type;
	typedef __mode_controller__::mode_thread< marray< array2< T, Allocator > >, array2< T, Allocator >, Functor > mode_thread;
	typedef std::vector< difference_type >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	marray< array2< T, Allocator > > out( in, s.margin_x, s.margin_y, 0 );

	list_type object     = mode_filter::create_pointer_diff_list( out, s.object );
	list_type update_in  = mode_filter::create_pointer_diff_list( out, s.update_in );
	list_type update_out = mode_filter::create_pointer_diff_list( out, s.update_out );

	mode_thread *thread = new mode_thread[ thread_num ];

	for( size_type i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update_in, update_out, i, thread_num, f );
	}

	f( 0.0 );

	// スレッドを実行して，終了まで待機する
	do_threads_( thread, thread_num );

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief 任意の構造要素に対応した最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] s          … 最頻値フィルタに用いる構造要素
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator >
bool mode( array2< T, Allocator > &in, const mode_filter::mode_structure &s, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, s, __mist_dmy_callback__( ), thread_num ) );
}



/// @brief 円を構造要素とする最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] radius     … 円の半径（単位は画素）
//! @param[in] f          … 進行状況を返すコールバック関数
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator, class Functor >
inline bool mode( array2< T, Allocator > &in, double radius, Functor f, typename array2< T, Allocator >::size_type thread_num )
{
	return( mode( in, mode_filter::circle( radius, in.reso1( ), in.reso2( ) ), f, thread_num ) );
}


/// @brief 円を構造要素とする最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] radius     … 円の半径（単位は画素）
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator >
inline bool mode( array2< T, Allocator > &in, double radius, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, mode_filter::circle( radius, in.reso1( ), in.reso2( ) ), __mist_dmy_callback__( ), thread_num ) );
}







/// @brief 任意の構造要素に対応した最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] s          … 最頻値フィルタに用いる構造要素
//! @param[in] f          … 進行状況を返すコールバック関数
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator, class Functor >
bool mode( array3< T, Allocator > &in, const mode_filter::mode_structure &s, Functor f, typename array3< T, Allocator >::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename array3< T, Allocator >::value_type       value_type;
	typedef typename array3< T, Allocator >::size_type        size_type;
	typedef typename array3< T, Allocator >::difference_type  difference_type;
	typedef __mode_controller__::mode_thread< marray< array3< T, Allocator > >, array3< T, Allocator >, Functor > mode_thread;
	typedef std::vector< difference_type >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	marray< array3< T, Allocator > > out( in, s.margin_x, s.margin_y, s.margin_z, 0 );

	list_type object     = mode_filter::create_pointer_diff_list( out, s.object );
	list_type update_in  = mode_filter::create_pointer_diff_list( out, s.update_in );
	list_type update_out = mode_filter::create_pointer_diff_list( out, s.update_out );

	mode_thread *thread = new mode_thread[ thread_num ];

	for( size_type i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update_in, update_out, i, thread_num, f );
	}

	f( 0.0 );

	// スレッドを実行して，終了まで待機する
	do_threads_( thread, thread_num );

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief 任意の構造要素に対応した最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] s          … 最頻値フィルタに用いる構造要素
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator >
bool mode( array3< T, Allocator > &in, const mode_filter::mode_structure &s, typename array3< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, s, __mist_dmy_callback__( ), thread_num ) );
}




/// @brief 球を構造要素とする最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] radius     … 球の半径（単位は画素）
//! @param[in] f          … 進行状況を返すコールバック関数
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator, class Functor >
inline bool mode( array3< T, Allocator > &in, double radius, Functor f, typename array3< T, Allocator >::size_type thread_num )
{
	return( mode( in, mode_filter::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), f, thread_num ) );
}




/// @brief 球を構造要素とする最頻値フィルタ
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトを使用する
//! @attention CPU数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in     … 入出力画像
//! @param[in] radius     … 球の半径（単位は画素）
//! @param[in] thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力画像が空の場合
//! 
template < class T, class Allocator >
inline bool mode( array3< T, Allocator > &in, double radius, typename array3< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, mode_filter::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), __mist_dmy_callback__( ), thread_num ) );
}



/// @}
//  最頻値フィルタグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MODE__
