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

/// @file mist/filter/decomposition.h
//!
//! @brief ユークリッド距離に基づく図形分割
//!
#ifndef __INCLUDE_MIST_FIGURE_DECOMPOSITION__
#define __INCLUDE_MIST_FIGURE_DECOMPOSITION__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#include "distance.h"
#endif

#ifndef __INCLUDE_MIST_LABELING__
#include "labeling.h"
#endif


#include <deque>
#include <vector>
#include <set>
#include <algorithm>


// mist名前空間の始まり
_MIST_BEGIN


// 図形分割用のツール
namespace __figure_dedomposition__
{
	struct Position
	{
		typedef ptrdiff_t difference_type;
		difference_type	RADIUS;
		double			radius;
		difference_type	index;

		Position( difference_type r = 0, double R = 0, difference_type indx = 0 ) : RADIUS( r ), radius( R - std::sqrt( static_cast< double >( r ) ) ), index( indx )
		{
		}

		bool operator <( const Position &p ) const
		{
			return( RADIUS > p.RADIUS );
		}
	};

	template < class T, class Allocator >
	inline typename array2< T, Allocator >::size_type do_labeling( array2< T, Allocator > &in )
	{
		return( labeling8( in, in ) );
	}

	template < class T, class Allocator >
	inline typename array3< T, Allocator >::size_type do_labeling( array3< T, Allocator > &in )
	{
		return( labeling26( in, in ) );
	}

	// 画像の端に画素を持っているかどうかを調べる
	template < class T, class Allocator >
	bool has_voxel_at_side( const array2< T, Allocator > &in )
	{
		typedef typename array2< T, Allocator >::size_type size_type;

		for( size_type i = 0 ; i < in.width( ) ; i++ )
		{
			if( in( i, 0 ) != 0 || in( i, in.height( ) - 1 ) != 0 )
			{
				return( true );
			}
		}

		for( size_type j = 0 ; j < in.height( ) ; j++ )
		{
			if( in( 0, j ) != 0 || in( in.width( ) - 1, j ) != 0 )
			{
				return( true );
			}
		}

		return( false );
	}

	// 画像の端に画素を持っているかどうかを調べる
	template < class T, class Allocator >
	bool has_voxel_at_side( const array3< T, Allocator > &in )
	{
		typedef typename array2< T, Allocator >::size_type size_type;

		if( in.depth( ) == 1 )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				if( in( i, 0, 0 ) != 0 || in( i, in.height( ) - 1, 0 ) != 0 )
				{
					return( true );
				}
			}

			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				if( in( 0, j, 0 ) != 0 || in( in.width( ) - 1, j, 0 ) != 0 )
				{
					return( true );
				}
			}
		}
		else
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( in( i, j, 0 ) != 0 || in( i, j, in.depth( ) - 1 ) != 0 )
					{
						return( true );
					}
				}
			}

			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( in( i, 0, k ) != 0 || in( i, in.height( ) - 1, k ) != 0 )
					{
						return( true );
					}
				}
			}

			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type j = 0 ; j < in.height( ) ; j++ )
				{
					if( in( 0, j, k ) != 0 || in( in.width( ) - 1, j, k ) != 0 )
					{
						return( true );
					}
				}
			}
		}

		return( false );
	}

	struct __mist_dmy_fd_callback__
	{
		/// @brief 図形分割の進行状況を返す
		//!
		//! @param[in] loop      … 現在の繰り返し回数
		//! @param[in] label_num … 現在の最大ラベル番号
		//! @param[in] radius    … 現在処理中の半径
		//! @param[in] in        … 入力画像
		//! @param[in] out       … 出力ラベル画像
		//!
		template < class Array1, class Array2 >
		void operator()( size_t loop, size_t label_num, double radius, const Array1 &in, const Array2 &out ) const
		{
		}
	};
}


//! @addtogroup decomposition_group 図形分割
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/decomposition.h>
//! @endcode
//!
//!  @{




/// @brief ユークリッド距離に基づく図形分割
//!
//! 図形をくびれ部分で分割する
//!
//! @param[in] in           … 入力データ
//! @param[in] out          … 出力画像
//! @param[in] max_distance … 最大距離値以上で発生するくびれは分割しないようにする
//! @param[in] f            … 進行状況を与えるコールバック
//!
//! @return 分割された領域の数
//!
template < class Array1, class Array2, class Functor >
typename Array1::size_type figure_decomposition( const Array1 &in, Array2 &out, double max_distance, Functor f )
{
	if( in.empty( ) )
	{
		return( 0 );
	}

	typedef typename Array1::size_type								size_type;
	typedef typename Array1::difference_type						difference_type;
	typedef typename Array1::const_pointer							const_pointer;
	typedef typename Array2::value_type								value_type;
	typedef typename Array2::template rebind< size_type >::other	distance_type;
	typedef typename Array2::template rebind< double >::other		mask_type;

	distance_type dist;
	dist.resize( in.width( ), in.height( ), in.depth( ) );
	dist.reso1( 1 );
	dist.reso2( 1 );
	dist.reso3( 1 );

	// 距離変換用の画像を作成
	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		dist[ i ] = in[ i ] != 0 ? 1 : 0;
	}

	if( __figure_dedomposition__::has_voxel_at_side( in ) )
	{
		// 画像の端に１画素がある場合
		marray< distance_type > dist_tmp( dist, 1 );
		distance_type &dist_tmp_map = dist_tmp;

		// 画像の外側には０画素が存在するものとして距離変換を行う
		calvin::distance_transform( dist_tmp_map, dist_tmp_map );

		for( size_type k = 0 ; k < dist.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < dist.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < dist.width( ) ; i++ )
				{
					dist( i, j, k ) = dist_tmp( i, j, k );
				}
			}
		}
	}
	else
	{
		calvin::distance_transform( dist, dist );
	}


	typedef __figure_dedomposition__::Position position_type;

	// 画像中に含まれる距離値のリストを作成する
	std::map< size_type, size_type > distance_list;
	for( size_type l = 0 ; l < dist.size( ) ; l++ )
	{
		distance_list[ dist[ l ] ]++;
	}

	size_type label_count = 0;		// 現在のラベル数
	size_type loop_count = 0;		// 現在のラベル数
	size_type current_label = 0;	// 現在処理中のラベル
	size_type label_max = type_limits< value_type >::maximum( );	// ラベルの最大値
	position_type pt;

	mask_type mask( in );
	mask.fill( );
	out.resize( in.width( ), in.height( ), in.depth( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	out.fill( );

	typename std::map< size_type, size_type >::reverse_iterator dite = distance_list.rbegin( );

	// 最大距離値より大きい部分は処理対象から省く
	if( max_distance > 0 )
	{
		double md = max_distance * max_distance;
		double Md = ( max_distance + 1 ) * ( max_distance + 1 );
		for( size_type l = 0 ; l < dist.size( ) ; l++ )
		{
			if( dist[ l ] >= md )
			{
				out[ l ] = 1;
			}
		}
		for( ; dite != distance_list.rend( ) ; ++dite )
		{
			if( dite->first < Md )
			{
				break;
			}
		}

		label_count = __figure_dedomposition__::do_labeling( out );
	}

	f( 0, label_count, std::sqrt( static_cast< double >( dite->first ) ), in, out );

	// 距離値の大きいものから順番にラベルを復元していく
	for( ; dite != distance_list.rend( ) ; ++dite )
	{
		size_type rr = dite->first;
		difference_type count = dite->second;

		if( rr == 0 )
		{
			// ０の画素はアクセスしない
			break;
		}

		double r = std::sqrt( static_cast< double >( rr ) );

		std::vector< difference_type > list;
		list.reserve( count );
		for( size_type i = 0 ; i < dist.size( ) ; i++ )
		{
			// 現段階で最大の距離値を持つ画素を探す
			if( dist[ i ] == rr && r >= mask[ i ] )
			//if( dist[ i ] == rr && r >= static_cast< difference_type >( mask[ i ] + 1 ) )
			{
				list.push_back( i );
			}
		}


		difference_type rx = static_cast< difference_type >( std::ceil( r ) );
		difference_type ry = in.height( ) < 2 ? 0 : rx;
		difference_type rz = in.depth( ) < 2 ? 0 : rx;
		size_type       RR = ( rx + 1 ) * ( rx + 1 );

		if( rr > 1 )
		{
			std::vector< position_type > sphere;
			sphere.reserve( ( 2 * rx + 1 ) * ( 2 * ry + 1 ) * ( 2 * rz + 1 ) );
			{
				difference_type cx = out.width( ) / 2;
				difference_type cy = out.height( ) / 2;
				difference_type cz = out.depth( ) / 2;
				const_pointer cp = &out( cx, cy, cz );
				for( difference_type k = -rz ; k <= rz ; k++ )
				{
					size_type kk = k * k;

					for( difference_type j = -ry ; j <= ry ; j++ )
					{
						size_type jj = j * j;

						for( difference_type i = -rx ; i <= rx ; i++ )
						{
							size_type ii = i * i;

							size_type rrr = ii + jj + kk;
							if( rrr < rr )
							{
								// インデックスとして，球の中心からの差を代入する
								sphere.push_back( position_type( rrr, r, &out( cx + i, cy + j, cz + k ) - cp ) );
							}
						}
					}
				}
			}

			difference_type csize = list.size( );
			while( csize > 0 )
			{
				difference_type cindex = 0;
				for( ; cindex < csize ; cindex++ )
				{
					if( out[ list[ cindex ] ] != 0 )
					{
						break;
					}
				}

				if( cindex == csize )
				{
					cindex = 0;
				}

				difference_type index = list[ cindex ];
				list[ cindex ] = list[ csize - 1 ];
				csize--;

				if( out[ index ] == 0 )
				{
					// 他の領域から塗られていないので，新しいラベルとする
					label_count++;
					if( label_count > label_max )
					{
						// 最大のラベル数を超えた場合には，最大ラベルとする
						label_count = label_max + 1; 
					}
					current_label = static_cast< value_type >( label_count );
				}
				else
				{
					// 既にラベルが割り当てられているので，そのラベルで塗りつぶす
					current_label = out[ index ];
				}

				for( size_type i = 0 ; i < sphere.size( ) ; i++ )
				{
					const position_type &pt = sphere[ i ];
					const difference_type indx = index + pt.index;
					if( mask[ indx ] < pt.radius )
					{
						if( dist[ indx ] + pt.RADIUS < RR )
						{
							mask[ indx ] = pt.radius;
							out[ indx ] = static_cast< value_type >( current_label );
						}
					}
				}
			}
		}
		else
		{
			// 距離値１の画素だけ特殊な処理を行う
			difference_type csize = list.size( );
			while( csize > 0 )
			{
				current_label = 0;

				difference_type cindex = 0;
				for( ; cindex < csize ; cindex++ )
				{
					difference_type indx = list[ cindex ];
					difference_type z = indx / ( out.width( ) * out.height( ) );
					difference_type y = ( indx - z * out.width( ) * out.height( ) ) / out.width( );
					difference_type x = indx - ( y + z * out.height( ) ) * out.width( );

					// ８・２６近傍から既に塗られているものを探してくる
					double min = 1.0e10;
					for( difference_type k = -rz ; k <= rz ; k++ )
					{
						size_type pz = k + z;
						if( pz < 0 || pz >= in.depth( ) ) continue;
						double kk = static_cast< double >( k * k );

						for( difference_type j = -ry ; j <= ry ; j++ )
						{
							size_type py = j + y;
							if( py < 0 || py >= in.height( ) ) continue;
							double jj = static_cast< double >( j * j );

							for( difference_type i = -rx ; i <= rx ; i++ )
							{
								size_type px = i + x;
								if( px < 0 || px >= in.width( ) ) continue;
								double rR = static_cast< double >( i * i + jj + kk );

								size_type cl = static_cast< size_type >( out( px, py, pz ) );
								if( cl != 0 && min > rR )
								{
									current_label = cl;
									min = rR;
								}
							}
						}
					}
					if( current_label != 0 )
					{
						break;
					}
				}

				if( cindex == csize )
				{
					cindex = 0;
				}

				difference_type index = list[ cindex ];
				list[ cindex ] = list[ csize - 1 ];
				csize--;

				if( current_label == 0 )
				{
					// 他の領域から塗られていないので，新しいラベルとする
					label_count++;
					if( label_count > label_max )
					{
						// 最大のラベル数を超えた場合には，最大ラベルとする
						label_count = label_max + 1; 
					}
					out[ index ] = static_cast< value_type >( label_count );
				}
				else
				{
					// 既にラベルが割り当てられているので，そのラベルで塗りつぶす
					out[ index ] = static_cast< value_type >( current_label );
				}
			}
		}

		f( ++loop_count, label_count, r, in, out );
	}

	// MAXラベル数を超えたものを除去
	for( size_type j = 0 ; j < out.size( ) ; j++ )
	{
		out[ j ] = static_cast< size_type >( out[ j ] ) > label_max ? 0 : out[ j ];
	}

	return( label_count );
}

/// @brief ユークリッド距離に基づく図形分割
//!
//! 図形をくびれ部分で分割する
//!
//! @param[in] in  … 入力データ
//! @param[in] out … 出力画像
//! @param[in] max_distance … 最大距離値以上で発生するくびれは分割しないようにする
//!
//! @return 分割された領域の数
//!
template < class Array1, class Array2 >
typename Array1::size_type figure_decomposition( const Array1 &in, Array2 &out, double max_distance = -1 )
{
	return( figure_decomposition( in, out, max_distance, __figure_dedomposition__::__mist_dmy_fd_callback__( ) ) );
}


/// @}
//  図形分割グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FIGURE_DECOMPOSITION__
