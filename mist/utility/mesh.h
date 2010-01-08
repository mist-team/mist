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

/// @file mist/utility/mesh.h
//!
//! @brief キャリブレーションチャートから点を切り出す
//!
#ifndef __INCLUDE_MIST_MESH__
#define __INCLUDE_MIST_MESH__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "../matrix.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "../vector.h"
#endif

#ifndef __INCLUDE_CONVERTER__
#include "../converter.h"
#endif

#ifndef __INCLUDE_MIST_LABELING__
#include "../filter/labeling.h"
#endif

#ifndef __INCLUDE_MIST_DRAWING__
#include "../drawing.h"
#endif


#include <deque>
#include <set>
#include <algorithm>

// mist名前空間の始まり
_MIST_BEGIN


//! @addtogroup mesh_group メッシュ抽出
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/utility/mesh.h>
//! @endcode
//!
//!  @{

namespace __mesh_utility__
{
	struct __mesh_information__
	{
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef vector2< double > vector_type;

		difference_type label;
		difference_type count;
		double          round;
		vector_type     pos;
		bool            enabled;
		difference_type mark;
		difference_type row;
		difference_type col;
		double          length;

		__mesh_information__( ) : label( -1 ), count( 0 ), round( 0.0 ), pos( 0.0, 0.0 ), enabled( true ), mark( -1 ), row( -1 ), col( -1 ), length( 1.0e10 )
		{
		}

		// 周囲長の大きさで並べ替える
		bool operator <( const __mesh_information__ &m ) const
		{
			return( this->round > m.round );
		}

		static bool sort_by_round( const __mesh_information__ &m1, const __mesh_information__ &m2 )
		{
			return( m1.round > m2.round );
		}

		static bool sort_by_length( const __mesh_information__ &m1, const __mesh_information__ &m2 )
		{
			return( m1.length < m2.length );
		}
	};

	inline double __minimum__( double v1, double v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}

	inline double __compute_border_distance__( const vector2< double > &pos, double image_width, double image_height )
	{
		double xmin = __minimum__( std::abs( pos.x ), std::abs( image_width - pos.x ) );
		double ymin = __minimum__( std::abs( pos.y ), std::abs( image_height - pos.y ) );
		return( __minimum__( xmin, ymin ) );
	}
}

/// @brief 円を格子状に並べた画像から，各点を抽出しメッシュ上の点と対応付ける
//! 
//! @attention 入力画像はあらかじめ，しきい値処理を行っておく必要がある．
//! 
//! グリッドの点は左上を原点とし，メッシュの基準ラインを定める点列を配置する．
//! 点列のサンプルは，mist/utility/mesh.ai を参考に作成してください．
//! 
//! 出力されるメッシュの座標のうち，対応する点が見つからないものには (-1,-1) を出力する．
//! 
//! @param[in]     chart … 入力画像（メッシュが映っている画像）
//! @param[in,out] grid  … 撮影した画像に含まれるメッシュの点数で初期化された配列（画像に移っていない点も含めた総数）を入力とし，画像中の座標が結果として出力される
//! @param[in]     row   … メッシュの基準ラインを定める点列の中心を表す行方向の位置（０から数えた時の位置）
//! @param[in]     col   … メッシュの基準ラインを定める点列の中心を表す列方向の位置（０から数えた時の位置）
//! @param[in]     threshold_for_circular_ratio … メッシュの候補を絞り込むための円形度のしきい値（これより小さい値のものは除外）
//! 
//! @return メッシュの抽出に成功したかどうか
//! 
template < class T, class Allocator >
bool extract_mesh( array2< T, Allocator > &chart, matrix< vector2< double > > &grid, typename array2< T, Allocator >::difference_type row, typename array2< T, Allocator >::difference_type col, double threshold_for_circular_ratio = 0.4 )
{
	typedef __mesh_utility__::__mesh_information__ __mesh_information__;
	typedef array2< T, Allocator >::size_type size_type;
	typedef array2< T, Allocator >::difference_type difference_type;
	typedef vector2< double > vector_type;
	typedef std::deque< __mesh_information__ > mesh_list_type;
	typedef typename mesh_list_type::iterator mesh_iterator;
	typedef typename mesh_list_type::reverse_iterator mesh_reverse_iterator;
	array2< size_type > binary;


	// ２値画像に変換
	convert( chart, binary );

	size_type i, j;
	difference_type r, c;
	difference_type rows = grid.rows( );
	difference_type cols = grid.cols( );

	size_type labelnum = labeling4( binary, binary );
	//std::cout << "LabelNum: " << labelnum << std::endl;

	mesh_list_type mesh_list;
	array< __mesh_information__ > meshes( labelnum + 1 );

	for( i = 0 ; i <= labelnum ; i++ )
	{
		meshes[ i ].label = i;
	}

	// 全てのグリッドを初期化する
	for( size_type i = 0 ; i < grid.size( ) ; i++ )
	{
		grid[ i ].x = -1;
		grid[ i ].y = -1;
	}

	// 背景は無効にする
	meshes[ 0 ].enabled = false;

	// 各ラベルの画素数，重心位置，周囲長を求める
	const double _2 = std::sqrt( 2.0 );
	for( j = 0 ; j < chart.height( ) - 1 ; j++ )
	{
		for( i = 0 ; i < chart.width( ) - 1 ; i++ )
		{
			__mesh_information__ &mesh = meshes[ binary( i, j ) ];
			 mesh.count++;
			 mesh.pos.x += i;
			 mesh.pos.y += j;

			// 画像の周囲長を計算する
			__mesh_information__ &m0 = mesh;
			__mesh_information__ &m1 = meshes[ binary( i + 1, j     ) ];
			__mesh_information__ &m2 = meshes[ binary( i    , j + 1 ) ];
			__mesh_information__ &m3 = meshes[ binary( i + 1, j + 1 ) ];
			size_type l0 = m0.label;
			size_type l1 = m1.label;
			size_type l2 = m2.label;
			size_type l3 = m3.label;

			if( l0 > 0 )
			{
				size_type num_eq = 0;
				if( l1 == l0 ) num_eq++;
				if( l2 == l0 ) num_eq++;
				if( l3 == l0 ) num_eq++;

				if( num_eq == 2 )
				{
					m0.round += _2;
				}
				else if( num_eq == 1 && l0 != l3 )
				{
					m0.round += 1.0;
				}
			}
			else if( l1 > 0 )
			{
				if( l1 == l3 )
				{
					if( l1 == l2 )
					{
						m1.round += _2;
					}
					else
					{
						m1.round += 1.0;
					}
				}
			}
			else if( l2 > 0 )
			{
				if( l2 == l3 )
				{
					m2.round += 1.0;
				}
			}
		}
	}


	// 各領域の重心位置を計算する
	for( i = 1 ; i <= labelnum ; i++ )
	{
		__mesh_information__ &mesh = meshes[ i ];
		mesh.pos.x /= mesh.count;
		mesh.pos.y /= mesh.count;
	}

	// 画面の境界に接している領域を取り除く
	for( i = 0 ; i < chart.width( ) ; i++ )
	{
		meshes[ binary( i, 0 ) ].enabled = false;
		meshes[ binary( i, chart.height( ) - 1 ) ].enabled = false;
	}

	// 画面の境界に接している領域を取り除く
	for( j = 0 ; j < chart.height( ) ; j++ )
	{
		meshes[ binary( 0, j ) ].enabled = false;
		meshes[ binary( chart.width( ) - 1, j ) ].enabled = false;
	}

	// 円形度を評価して，不要な領域を除去する
	difference_type minimum_count = 15; // この個数に満たない領域は削除する
	for( i = 1 ; i <= labelnum ; i++ )
	{
		__mesh_information__ &mesh = meshes[ i ];
		if( mesh.count < minimum_count )
		{
			mesh.enabled = false;
		}
		else if( mesh.enabled )
		{
			const double pai = 3.1415926535897932384626433832795;
			const double S = mesh.count;
			const double L = mesh.round;
			double e = 4.0 * pai * S / ( L * L );
			if( e < threshold_for_circular_ratio )
			{
				mesh.enabled = false;
			}
			else
			{
				mesh_list.push_back( mesh );
			}
			//std::cout << "ラベル: " << i << ", 円形度: " << e << std::endl;
		}
	}

	if( mesh_list.size( ) < 10 )
	{
		return( false );
	}

	// 周囲長の大きさを基準として並べ替える
	std::sort( mesh_list.begin( ), mesh_list.end( ), __mesh_information__::sort_by_round );

	// 周囲長の大き差を基準として，基準点列を抽出する
	{
		mesh_iterator ite = mesh_list.begin( );
		difference_type base_mesh_num = 1;
		double oround = ite->round;
		double maxround = ite->round;
	
		ite->mark = 1;
		++ite;

		for( ; ite != mesh_list.end( ) ; ++ite )
		{
			double round = ite->round;

			//std::cout << "周囲長: " << round << ", 比: " << round / oround << std::endl;

			// 一つ前の周囲長の90％以上で，最大の円の周囲長の80%以上の長さがある場合，基準点としてマークする
			if( round > oround * 0.9 && round > maxround * 0.5 )
			{
				ite->mark = 1;
				base_mesh_num++;
				oround = round;
			}
			else
			{
				break;
			}
		}
		for( ; ite != mesh_list.end( ) ; ++ite )
		{
			ite->mark = 100;
		}

		/// 基準となる中心点を見つける
		// 基準点の数が少ない場合には終了
		if( base_mesh_num < 2 )
		{
			for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
			{
				meshes[ ite->label ].mark = ite->mark;
			}

			for( i = 0 ; i < chart.size( ) ; i++ )
			{
				if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
				{
					chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
				}
				else
				{
					chart[ i ] = 255;
				}
			}

			return( false );
		}

		// まず，大きい基準点の座標を用いて基準点を結ぶ直線を求める
		double x1 = mesh_list[ 0 ].pos.x;
		double y1 = mesh_list[ 0 ].pos.y;
		double x2 = mesh_list[ 1 ].pos.x;
		double y2 = mesh_list[ 1 ].pos.y;
		double A = y2 - y1;
		double B = x1 - x2;
		double C = x2 * y1 - x1 * y2;

		mesh_iterator site = mesh_list.begin( );
		mesh_iterator eite = site + base_mesh_num;

		// 判定ミスの基準点を排除する
		{
			double __threshold__ = std::sqrt( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) ) * 0.2;
			double D = 1.0 / std::sqrt( A * A + B * B );
			for( ite = site ; ite != eite ; ++ite )
			{
				if( std::abs( A * ite->pos.x + B * ite->pos.y + C ) * D > __threshold__ )
				{
					ite->mark = 100;
					base_mesh_num--;
				}
			}
		}

		// 基準点を順番に並べ替える評価値を計算する
		for( ite = site ; ite != eite ; ++ite )
		{
			if( ite->mark == 1 )
			{
				ite->length = - B * ite->pos.x + A * ite->pos.y;
			}
			else
			{
				ite->length = 1.0e10;
			}
		}

		// 基準線に沿って並び替える
		std::sort( site, eite, __mesh_information__::sort_by_length );
		site = mesh_list.begin( );
		eite = site + base_mesh_num;

		// 大きさが変化する位置を再度検出する
		{
			// まず，もっとも周囲長の長いものを見つける
			mesh_iterator mite = mesh_list.begin( );
			mesh_iterator oite = mesh_list.begin( );
			mesh_iterator cite = oite;
			for( ; cite != eite ; ++cite )
			{
				if( mite->round < cite->round )
				{
					mite = cite;
				}
			}

			for( cite = mite, oite = mite ; cite != eite ; ++cite )
			{
				double r1 = oite->round;
				double r2 = cite->round;

				// 一つ前の周囲長の80％以上であれば，基準点としてマークする
				//std::cout << "周囲長: " << r1 << ", 比: " << r2 / r1 << std::endl;
				if( ( r1 > r2 && r2 < r1 * 0.8 ) || ( r1 < r2 && r1 < r2 * 0.8 ) )
				{
					break;
				}

				oite = cite;
			}

			for( ; cite != eite ; ++cite )
			{
				cite->mark = 100;
				cite->length = 1.0e10;
			}

			mesh_reverse_iterator rmite( mite + 1 );
			mesh_reverse_iterator reite = mesh_list.rend( );
			mesh_reverse_iterator rcite( rmite );
			for( mesh_reverse_iterator roite = rmite ; rcite != reite ; ++rcite )
			{
				double r1 = roite->round;
				double r2 = rcite->round;

				// 一つ前の周囲長の80％以上であれば，基準点としてマークする
				//std::cout << "周囲長: " << r1 << ", 比: " << r2 / r1 << std::endl;
				if( ( r1 > r2 && r2 < r1 * 0.8 ) || ( r1 < r2 && r1 < r2 * 0.8 ) )
				{
					break;
				}

				roite = rcite;
			}

			for( ; rcite != reite ; ++rcite )
			{
				rcite->mark = 100;
				rcite->length = 1.0e10;
			}

			for( cite = site, base_mesh_num = 0 ; cite != eite ; ++cite )
			{
				if( cite->mark == 1 )
				{
					base_mesh_num++;
				}
			}

			// 基準線に沿って並び替える
			std::sort( site, eite, __mesh_information__::sort_by_length );
			site = mesh_list.begin( );
			eite = site + base_mesh_num;

			// 基準点が抽出されすぎたときは終了する
			if( col + 1 < base_mesh_num )
			{
				for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
				{
					meshes[ ite->label ].mark = ite->mark;
				}

				for( i = 0 ; i < chart.size( ) ; i++ )
				{
					if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
					{
						chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
					}
					else
					{
						chart[ i ] = 255;
					}
				}

				return( false );
			}
		}

		// 中心をあらわす基準点を決定する
		// 基準ラインの左右の点のどちらかを基準にする
		{
			mesh_iterator ite1 = mesh_list.begin( );
			mesh_iterator ite2 = ite1 + base_mesh_num - 1;
			double lth = 2.0 * ( ite1->pos - ite2->pos ).length( ) / static_cast< double >( base_mesh_num - 1 );

			double ratio1 = 1.0e10;
			double ratio2 = 1.0e10;

			double len0 = __mesh_utility__::__compute_border_distance__( mesh_list[ 0 ].pos, chart.width( ), chart.height( ) );
			double len1 = __mesh_utility__::__compute_border_distance__( mesh_list[ base_mesh_num - 1 ].pos, chart.width( ), chart.height( ) );

			// 端から一定距離以上離れている場合に判定を行う
			if( len0 > lth )
			{
				vector_type p1  = ite1->pos;
				vector_type p2  = ( ite1 + 1 )->pos;
				double length   = ( p1 - p2 ).length( );
				vector_type dir = ( p1 - p2 ).unit( );
				vector_type p   = p1 + dir * length * 0.8;

				double min = 1.0e10;
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min && cite != ite1 && l < length )
					{
						min = l;
						ratio1 = cite->round / ite1->round;
					}
				}
			}

			// 端から一定距離以上離れている場合に判定を行う
			if( len1 > lth )
			{
				vector_type p1  = ite2->pos;
				vector_type p2  = ( ite2 - 1 )->pos;
				double length   = ( p1 - p2 ).length( );
				vector_type dir = ( p1 - p2 ).unit( );
				vector_type p   = p1 + dir * length * 0.8;

				double min = 1.0e10;
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min && cite != ite2 && l < length )
					{
						min = l;
						ratio2 = cite->round / ite2->round;
					}
				}
			}

			if( ratio1 > ratio2 )
			{
				// 並び順を逆転させる
				std::reverse( mesh_list.begin( ), mesh_list.begin( ) + base_mesh_num );
				site = mesh_list.begin( );
				eite = site + base_mesh_num;
			}
			else if( ratio1 == ratio2 && len0 < len1 )
			{
				// 並び順を逆転させる
				std::reverse( mesh_list.begin( ), mesh_list.begin( ) + base_mesh_num );
				site = mesh_list.begin( );
				eite = site + base_mesh_num;
			}
		}

		// 順番に値を設定する
		difference_type mark = 1;
		difference_type column = col;
		for( ite = site ; ite != eite ; ++ite )
		{
			ite->mark = mark++;
			ite->row = row;
			ite->col = column--;
			//std::cout << "( " << ite->row << ", " << ite->col << " )" << std::endl;
			grid( ite->row, ite->col ) = ite->pos;
		}
		for( ; ite != mesh_list.end( ) ; ++ite )
		{
			ite->mark = 100;
		}

		// 出力データを整形する
		for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
		{
			meshes[ ite->label ].mark = ite->mark;
		}

		// 基準点の数が少ない場合には終了
		if( base_mesh_num < 3 )
		{
			for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
			{
				meshes[ ite->label ].mark = ite->mark;
			}

			for( i = 0 ; i < chart.size( ) ; i++ )
			{
				if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
				{
					chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
				}
				else
				{
					chart[ i ] = 255;
				}
			}

			return( false );
		}


		// 求まった大きな３つの円の配置を元にグリッドの座標を更新する
		// その際，大きいものから３点のみを最初に確定させる
		__mesh_information__ m1 = mesh_list[ 0 ];
		__mesh_information__ m2 = mesh_list[ 2 ];
		site = mesh_list.begin( );
		eite = site + 3;

		vector_type p1 = m1.pos;
		vector_type p2 = m2.pos;
		vector_type dir = ( p1 - p2 ).unit( );
		vector_type norm( -dir.y, dir.x );
		double length = ( p1 - p2 ).length( ) / static_cast< double >( 2 );

		// 不用なデータを削除する
		mesh_list.erase( site, eite );

		// 基準ラインで確定しているものの近傍のみを確定させる
		difference_type rindex[] = { -1, 1 };
		for( r = 0 ; r < 2 ; r++ )
		{
			for( c = m1.col ; c >= m2.col ; c-- )
			{
				vector_type &op = grid( m1.row + rindex[ r ], c );
				vector_type p = grid( m1.row, c ) + rindex[ r ] * norm * length * 0.5;

				double min = 1.0e10;
				mesh_iterator cur = mesh_list.begin( );
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min )
					{
						min = l;
						cur = cite;
					}
				}

				// 一定距離以上はなれた探索点は無視する
				if( cur != mesh_list.end( ) && min < length && min < cur->length )
				{
					// 以前に割り当てられていた場合は切り替える
					if( cur->row >= 0 || cur->col >= 0 )
					{
						grid( cur->row, cur->col ).x = -1;
						grid( cur->row, cur->col ).y = -1;
					}

					cur->row = r;
					cur->col = c;
					cur->length = min;

					// 対応点が見つかったので，以降の探索から除外候補に追加する
					op = cur->pos;
					cur->mark = -2;
				}
			}
		}

		for( mesh_iterator ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; )
		{
			if( ite->mark == -2 )
			{
				ite = mesh_list.erase( ite );
			}
			else
			{
				++ite;
			}
		}
	}

	{
		for( i = 0 ; i < chart.size( ) ; i++ )
		{
			if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
			{
				chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
			}
			else
			{
				chart[ i ] = 255;
			}
		}
	}


	// 反復的に対応点を見つける
	while( true )
	{
		difference_type ncount = 0;
		for( r = 0 ; r < rows && ncount < rows * cols ; r++ )
		{
			for( c = 0 ; c < cols && ncount < rows * cols ; c++ )
			{
				vector_type p = grid( r, c );

				if( p.x != -1 || p.y != -1 )
				{
					ncount++;
					continue;
				}

				double search_length = 0.0;

				// 近傍の状態を使って，グリッド上の点が存在すると思われる位置を予測する
				if( 0 < c && c < cols - 1 && grid( r, c - 1 ).x >= 0 && grid( r, c + 1 ).x >= 0 )
				{
					p = ( grid( r, c - 1 ) + grid( r, c + 1 ) ) / 2.0;
					search_length = ( grid( r, c - 1 ) - grid( r, c + 1 ) ).length( ) / 2.0;
				}
				else if( 0 < r && r < rows - 1 && grid( r - 1, c ).x >= 0 && grid( r + 1, c ).x >= 0 )
				{
					p = ( grid( r - 1, c ) + grid( r + 1, c ) ) / 2.0;
					search_length = ( grid( r - 1, c ) - grid( r + 1, c ) ).length( ) / 2.0;
				}
				else if( c < cols - 3 && grid( r, c + 1 ).x >= 0 && grid( r, c + 2 ).x >= 0 && grid( r, c + 3 ).x >= 0 )
				{
					double l1 = ( grid( r, c + 2 ) - grid( r, c + 3 ) ).length( );
					double l2 = ( grid( r, c + 1 ) - grid( r, c + 2 ) ).length( );
					p = grid( r, c + 1 ) + ( grid( r, c + 1 ) - grid( r, c + 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( c > 3 && grid( r, c - 1 ).x >= 0 && grid( r, c - 2 ).x >= 0 && grid( r, c - 3 ).x >= 0 )
				{
					double l1 = ( grid( r, c - 2 ) - grid( r, c - 3 ) ).length( );
					double l2 = ( grid( r, c - 1 ) - grid( r, c - 2 ) ).length( );
					p = grid( r, c - 1 ) + ( grid( r, c - 1 ) - grid( r, c - 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r < rows - 3 && grid( r + 1, c ).x >= 0 && grid( r + 2, c ).x >= 0 && grid( r + 3, c ).x >= 0 )
				{
					double l1 = ( grid( r + 2, c ) - grid( r + 3, c ) ).length( );
					double l2 = ( grid( r + 1, c ) - grid( r + 2, c ) ).length( );
					p = grid( r + 1, c ) + ( grid( r + 1, c ) - grid( r + 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r > 3 && grid( r - 1, c ).x >= 0 && grid( r - 2, c ).x >= 0 && grid( r - 3, c ).x >= 0 )
				{
					double l1 = ( grid( r - 2, c ) - grid( r - 3, c ) ).length( );
					double l2 = ( grid( r - 1, c ) - grid( r - 2, c ) ).length( );
					p = grid( r - 1, c ) + ( grid( r - 1, c ) - grid( r - 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( c > 1 && r < rows - 1 && grid( r + 1, c ).x >= 0 && grid( r, c - 1 ).x >= 0 && grid( r + 1, c - 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r + 1, c - 1 );
					vector_type &p1 = grid( r + 1, c );
					vector_type &p2 = grid( r, c - 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else if( c > 1 && r > 1 && grid( r - 1, c ).x >= 0 && grid( r, c - 1 ).x >= 0 && grid( r - 1, c - 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r - 1, c - 1 );
					vector_type &p1 = grid( r - 1, c );
					vector_type &p2 = grid( r, c - 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else if( c < cols - 1 && r > 1 && grid( r, c + 1 ).x >= 0 && grid( r - 1, c ).x >= 0 && grid( r - 1, c + 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r - 1, c + 1 );
					vector_type &p1 = grid( r - 1, c );
					vector_type &p2 = grid( r, c + 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else if( c < cols - 1 && r < rows - 1 && grid( r, c + 1 ).x >= 0 && grid( r + 1, c ).x >= 0 && grid( r + 1, c + 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r + 1, c + 1 );
					vector_type &p1 = grid( r + 1, c );
					vector_type &p2 = grid( r, c + 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else
				{
					// 近傍の点が見つからなかった
					ncount++;
					continue;
				}

				// 予測された点を用いて最寄の点を探す
				double min = 1.0e10;
				mesh_iterator cur = mesh_list.begin( );
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min )
					{
						min = l;
						cur = cite;
					}
				}

				if( cur != mesh_list.end( ) && min < search_length && min < cur->length )
				{
					// 以前に割り当てられていた場合は切り替える
					if( cur->row >= 0 || cur->col >= 0 )
					{
						grid( cur->row, cur->col ).x = -1;
						grid( cur->row, cur->col ).y = -1;
					}

					cur->row = r;
					cur->col = c;
					cur->length = min;

					// 対応点が見つかったので，以降の探索から除外候補に追加する
					grid( r, c ) = cur->pos;
					cur->mark = -2;
				}
				else
				{
					// 対応点が見つからなかったので，見つからなかったマークを入れる
					grid( r, c ).x = -2;
					grid( r, c ).y = -2;
				}
			}
		}

		for( mesh_iterator ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; )
		{
			if( ite->mark == -2 )
			{
				ite = mesh_list.erase( ite );
			}
			else
			{
				++ite;
			}
		}

		if( ncount == rows * cols )
		{
			// 更新すべき点が見つからなかったので終了する
			break;
		}
	}

	// 孤立点を削除する
	for( r = 0 ; r < rows ; r++ )
	{
		for( c = 0 ; c < cols ; c++ )
		{
			if( grid( r, c ).x < 0 )
			{
				continue;
			}

			bool b1 = r > 0    ? grid( r - 1, c ).x >= 0 : false;
			bool b2 = r < rows ? grid( r + 1, c ).x >= 0 : false;
			bool b3 = c > 0    ? grid( r, c - 1 ).x >= 0 : false;
			bool b4 = c < cols ? grid( r, c + 1 ).x >= 0 : false;

			size_t count = 0;
			if( r > 0    && grid( r - 1, c ).x >= 0 )
			{
				count++;
			}
			if( r < rows && grid( r + 1, c ).x >= 0 )
			{
				count++;
			}
			if( c > 0    && grid( r, c - 1 ).x >= 0 )
			{
				count++;
			}
			if( c < cols && grid( r, c + 1 ).x >= 0 )
			{
				count++;
			}

			if( count <= 1 )
			{
				grid( r, c ).x = -2;
				grid( r, c ).y = -2;
			}
		}
	}

	return( true );
}


/// @}
//  メッシュ抽出グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
