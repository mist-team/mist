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

// mist名前空間の始まり
_MIST_BEGIN


//! @addtogroup mesh_group メッシュ抽出
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/utility/mesh.h>
//! @endcode
//!
//!  @{


/// @brief 円を格子状に並べた画像から，各店を抽出しメッシュ上の点と対応付ける
//! 
//! @attention 入力画像はあらかじめ，しきい値処理を行っておく必要がある．
//! @attention グリッドの点は左上を原点とする．
//! @attention メッシュの上下左右を決める点は３点入力し，左側に大きな円を２個近づけて配置する．
//! 
//! 出力されるメッシュの座標のうち，対応する点が見つからないものには (-1,-1) を出力する．
//! 
//! @param[in]     chart           … 入力画像（メッシュが映っている画像）
//! @param[in,out] grid            … メッシュの各店の座標を入力とし，画像中の座標を格納がされる
//! @param[in]     left_circularX  … メッシュの上下左右を決めるための左側の点のメッシュ上での列方向の位置（０から数えた時の位置）
//! @param[in]     right_circularX … メッシュの上下左右を決めるための右側の点のメッシュ上での列方向の位置（０から数えた時の位置）
//! @param[in]     circularY       … メッシュの上下左右を決めるための２つの点のメッシュ上での行方向の位置（０から数えた時の位置）
//! 
//! @return メッシュの抽出に成功したかどうか
//! 
template < class T, class Allocator >
bool extract_mesh( const array2< T, Allocator > &chart, matrix< vector2< double > > &grid, size_t left_circularX, size_t right_circularX, size_t circularY )
{
	typedef array2< T, Allocator >::size_type size_type;
	typedef array2< T, Allocator >::difference_type difference_type;
	typedef vector2< double > vector_type;
	array2< unsigned short > binary;


	// ２値画像に変換
	convert( chart, binary );

	size_type i, j;

	size_type labelnum = labeling4( binary, binary );
	//std::cout << "LabelNum: " << labelnum << std::endl;

	array< bool > mask( labelnum + 1 );
	difference_type *count = new difference_type[ labelnum + 1 ];
	difference_type *round = new difference_type[ labelnum + 1 ];
	vector_type *pos = new vector2< double >[ labelnum + 1 ];

	for( i = 0 ; i <= labelnum ; i++ )
	{
		count[ i ] = 0;
		round[ i ] = 0;
		pos[ i ].x = 0;
		pos[ i ].y = 0;
	}

	// 各ラベルの画素数と重心位置を求める
	for( j = 0 ; j < chart.height( ) ; j++ )
	{
		for( i = 0 ; i < chart.width( ) ; i++ )
		{
			size_type label = binary( i, j );
			if( label > 0 )
			{
				count[ label ]++;
				pos[ label ].x += i;
				pos[ label ].y += j;
			}
		}
	}

	// 各領域の重心位置を計算する
	for( i = 1 ; i <= labelnum ; i++ )
	{
		double num = count[ i ];
		pos[ i ].x /= num;
		pos[ i ].y /= num;
	}

	// 円形でない領域を取り除くために，各領域の境界画素の数をカウントする
	for( j = 1 ; j < chart.height( ) - 1 ; j++ )
	{
		for( i = 1 ; i < chart.width( ) - 1 ; i++ )
		{
			if( binary( i, j ) != 0 && ( binary( i - 1, j ) == 0 || binary( i + 1, j ) == 0 || binary( i, j - 1 ) == 0 || binary( i, j + 1 ) == 0 ) )
			{
				round[ binary( i, j ) ]++;
			}
		}
	}

	// 画面の境界に接している領域を取り除く
	for( i = 0 ; i < chart.width( ) ; i++ )
	{
		if( binary( i, 0 ) != 0 )
		{
			count[ binary( i, 0 ) ] = 0;
		}
		if( binary( i, chart.height( ) - 1 ) != 0 )
		{
			count[ binary( i, chart.height( ) - 1 ) ] = 0;
		}
	}

	// 画面の境界に接している領域を取り除く
	for( j = 0 ; j < chart.height( ) ; j++ )
	{
		if( binary( 0, j ) != 0 )
		{
			count[ binary( 0, j ) ] = 0;
		}
		if( binary( chart.width( ) - 1, j ) != 0 )
		{
			count[ binary( chart.width( ) - 1, j ) ] = 0;
		}
	}

	// 円形度を評価して，不要な領域を除去する
	difference_type minimum_count = 15; // この個数に満たない領域は削除する
	for( i = 1 ; i <= labelnum ; i++ )
	{
		const double pai = 3.1415926535897932384626433832795;
		double e = 4.0 * pai * count[ i ] / static_cast< double >( round[ i ] * round[ i ] );
		if( count[ i ] < minimum_count || e < 0.95 )
		{
			count[ i ] = 0;
			mask[ i ] = true;
		}
	}

	delete [] round;

	// 不要な領域を画像から取り除く
	//for( j = 0 ; j < chart.height( ) - 0 ; j++ )
	//{
	//	for( i = 0 ; i < chart.width( ) - 0 ; i++ )
	//	{
	//		if( count[ binary( i, j ) ] == 0 )
	//		{
	//			chart( i, j ) = 255;
	//		}
	//	}
	//}

	// 領域の面積の１番大きいものを選ぶ
	difference_type index1 = 0;
	difference_type index2 = 0;
	difference_type index3 = 0;
	difference_type max = 0;
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( count[ i ] > max )
		{
			max = count[ i ];
			index1 = i;
		}
	}
	// 2番目に大きな領域を見つける
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( i != index1 && count[ i ] > max )
		{
			max = count[ i ];
			index2 = i;
		}
	}
	// 3番目に大きな領域を見つける
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( i != index1 && i != index2 && count[ i ] > max )
		{
			max = count[ i ];
			index3 = i;
		}
	}

	difference_type lindex, rindex;
	vector_type p1, p2;
	vector_type p1_ = grid( circularY, left_circularX );
	vector_type p2_ = grid( circularY, right_circularX );
	// 面積の大きい領域で，隣り合っているほうを１，そうでないほうを2とラベルつける
	{
		vector_type pp1 = pos[ index1 ];
		vector_type pp2 = pos[ index2 ];
		vector_type pp3 = pos[ index3 ];
		double len12 = ( pp1 - pp2 ).length( );
		double len13 = ( pp1 - pp3 ).length( );
		double len23 = ( pp2 - pp3 ).length( );

		if( len12 < len13 && len12 < len23 )
		{
			p2 = pp3;
			rindex = index3;
			if( len13 < len23 )
			{
				p1 = pp2;
				lindex = index2;
			}
			else
			{
				p1 = pp1;
				lindex = index1;
			}
		}
		else if( len13 < len12 && len13 < len23 )
		{
			p2 = pp2;
			rindex = index2;
			if( len12 < len23 )
			{
				p1 = pp3;
				lindex = index3;
			}
			else
			{
				p1 = pp1;
				lindex = index1;
			}
		}
		else
		{
			p2 = pp1;
			rindex = index1;
			if( len12 < len13 )
			{
				p1 = pp3;
				lindex = index3;
			}
			else
			{
				p1 = pp2;
				lindex = index2;
			}
		}
	}


	// 求まった大きな２つの円の配置を元にグリッドの座標を更新する
	{
		double len1 = ( p2  - p1  ).length( );
		double len2 = ( p2_ - p1_ ).length( );
		double scale = len1 / len2;
		vector_type dp = p1 - p1_;
		vector_type d1 = ( p2  - p1  ).unit( );
		vector_type d2 = ( p2_ - p1_ ).unit( );
		double c = d1 ^ d2;
		double s = std::sqrt( 1.0 - c * c );
		if( ( d2.x * c - d1.x ) * d2.y < 0 || ( d1.y - d2.y * c ) * d2.x < 0 )
		{
			s = -s;
		}

		for( i = 0 ; i < grid.size( ) ; i++ )
		{
			vector_type v = grid[ i ] - p1_;
			grid[ i ].x = ( v.x * c - v.y * s ) * scale + p1.x;
			grid[ i ].y = ( v.x * s + v.y * c ) * scale + p1.y;
		}
	}

	// 変換後のグリッド位置が撮影された絵の円内に入っていて，かつ基準ラインから前後に2ライン以内のものを優先的に割り当てる
	difference_type r, c, rows = grid.rows( ), cols = grid.cols( );
	difference_type rrr[ 3 ] = { 0, 1, -1 };
	matrix< int > found( rows, cols );
	matrix< double > flength( rows, cols );
	for( r = 0 ; r <= 2 ; r++ )
	{
		for( c = left_circularX ; c <= static_cast< difference_type >( right_circularX ) ; c++ )
		{
			vector_type &p = grid( circularY - rrr[ r ], c );
			size_type x = static_cast< size_type >( p.x );
			size_type y = static_cast< size_type >( p.y );

			difference_type index = -1;
			double min = 1.0e10;
			for( i = 1 ; i <= labelnum ; i++ )
			{
				if( !mask[ i ] )
				{
					double l = ( pos[ i ] - p ).length( );
					if( l < min )
					{
						min = l;
						index = i;
					}
				}
			}

			if( index < 0 )
			{
				// 対応店が見つからなかったので，見つからなかったマークを入れる
				p.x = -1;
				p.y = -1;
			}
			else
			{
				// 対応点が見つかったので，以降の探索から除外する
				p = pos[ index ];
				found( circularY - rrr[ r ], c ) = 1;
				mask[ index ] = true;
			}
		}
	}

	while( true )
	{
		difference_type ncount = 0;
		for( r = 0 ; r < rows ; r++ )
		{
			for( c = 0 ; c < cols ; c++ )
			{
				if( found( r, c ) >= 1 )
				{
					ncount++;
					continue;
				}

				// 近傍の状態を使って，グリッド上の点が存在すると思われる位置を予測する
				if( 0 < c && c < cols - 1 && found( r, c - 1 ) == 1 && found( r, c + 1 ) == 1 )
				{
					grid( r, c ) = ( grid( r, c - 1 ) + grid( r, c + 1 ) ) / 2.0;
					found( r, c ) = -1;
					flength( r, c ) = ( grid( r, c - 1 ) - grid( r, c + 1 ) ).length( ) / 2.0;
				}
				else if( 0 < r && r < rows - 1 && found( r - 1, c ) == 1 && found( r + 1, c ) == 1 )
				{
					grid( r, c ) = ( grid( r - 1, c ) + grid( r + 1, c ) ) / 2.0;
					found( r, c ) = -1;
					flength( r, c ) = ( grid( r - 1, c ) - grid( r + 1, c ) ).length( ) / 2.0;
				}
				else if( c < cols - 3 && found( r, c + 1 ) == 1 && found( r, c + 2 ) == 1 && found( r, c + 3 ) == 1 )
				{
					double l1 = ( grid( r, c + 2 ) - grid( r, c + 3 ) ).length( );
					double l2 = ( grid( r, c + 1 ) - grid( r, c + 2 ) ).length( );
					grid( r, c ) = grid( r, c + 1 ) + ( grid( r, c + 1 ) - grid( r, c + 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( c > 3 && found( r, c - 1 ) == 1 && found( r, c - 2 ) == 1 && found( r, c - 3 ) == 1 )
				{
					double l1 = ( grid( r, c - 2 ) - grid( r, c - 3 ) ).length( );
					double l2 = ( grid( r, c - 1 ) - grid( r, c - 2 ) ).length( );
					grid( r, c ) = grid( r, c - 1 ) + ( grid( r, c - 1 ) - grid( r, c - 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r < rows - 3 && found( r + 1, c ) == 1 && found( r + 2, c ) == 1 && found( r + 3, c ) == 1 )
				{
					double l1 = ( grid( r + 2, c ) - grid( r + 3, c ) ).length( );
					double l2 = ( grid( r + 1, c ) - grid( r + 2, c ) ).length( );
					grid( r, c ) = grid( r + 1, c ) + ( grid( r + 1, c ) - grid( r + 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r > 3 && found( r - 1, c ) == 1 && found( r - 2, c ) == 1 && found( r - 3, c ) == 1 )
				{
					double l1 = ( grid( r - 2, c ) - grid( r - 3, c ) ).length( );
					double l2 = ( grid( r - 1, c ) - grid( r - 2, c ) ).length( );
					grid( r, c ) = grid( r - 1, c ) + ( grid( r - 1, c ) - grid( r - 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else
				{
					// 近傍の点が見つからなかった
					ncount++;
				}
			}
		}

		if( ncount == rows * cols )
		{
			// 更新すべき点が見つからなかったので終了する
			break;
		}

		// 予測された位置を基にして，最寄の点を求める
		for( r = 0 ; r < rows ; r++ )
		{
			for( c = 0 ; c < cols ; c++ )
			{
				if( found( r, c ) != -1 )
				{
					continue;
				}

				vector_type &p = grid( r, c );
				size_type x = static_cast< size_type >( p.x );
				size_type y = static_cast< size_type >( p.y );
				double length_threshold = flength( r, c );

				difference_type index = -1;
				double min = 1.0e10;
				for( i = 1 ; i <= labelnum ; i++ )
				{
					if( !mask[ i ] )
					{
						double l = ( pos[ i ] - p ).length( );
						if( l < min )
						{
							min = l;
							index = i;
						}
					}
				}

				if( index < 0 || min > length_threshold )
				{
					// 対応店が見つからなかったので，見つからなかったマークを入れる
					p.x = -1;
					p.y = -1;
					found( r, c ) = 2;
				}
				else
				{
					// 対応点が見つかったので，以降の探索から除外する
					p = pos[ index ];
					found( r, c ) = 1;
					mask[ index ] = true;
				}
			}
		}
	}


	delete [] count;
	delete [] pos;

	{
		difference_type width = chart.width( );
		difference_type height = chart.height( );
		for( size_type r = 0 ; r < grid.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < grid.cols( ) ; c++ )
			{
				difference_type x = static_cast< difference_type >( grid( r, c ).x );
				difference_type y = static_cast< difference_type >( grid( r, c ).y );
				if( found( r, c ) != 1 || x < 0 || y < 0 )
				{
					// 点の見つからなかったものの座標にマークをつける
					grid( r, c ).x = -1;
					grid( r, c ).y = -1;
					continue;
				}
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
