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
//! @attention メッシュの上下左右を決める点をL字型に4点配置し，Lの底辺に3点を配置する．
//! 
//! 出力されるメッシュの座標のうち，対応する点が見つからないものには (-1,-1) を出力する．
//! 
//! @param[in]     chart  … 入力画像（メッシュが映っている画像）
//! @param[in,out] grid   … メッシュの各店の座標を入力とし，画像中の座標を格納がされる
//! @param[in]     leftX  … メッシュの上下左右を決めるための左側の点のメッシュ上での列方向の位置（０から数えた時の位置）
//! @param[in]     leftY  … メッシュの上下左右を決めるための左側の点のメッシュ上での行方向の位置（０から数えた時の位置）
//! @param[in]     rightX … メッシュの上下左右を決めるための右側の点のメッシュ上での列方向の位置（０から数えた時の位置）
//! @param[in]     rightY … メッシュの上下左右を決めるための右側の点のメッシュ上での行方向の位置（０から数えた時の位置）
//! 
//! @return メッシュの抽出に成功したかどうか
//! 
template < class T, class Allocator >
bool extract_mesh( const array2< T, Allocator > &chart, matrix< vector2< double > > &grid,
					typename array2< T, Allocator >::difference_type leftX, typename array2< T, Allocator >::difference_type leftY,
					typename array2< T, Allocator >::difference_type rightX, typename array2< T, Allocator >::difference_type rightY )
{
	typedef array2< T, Allocator >::size_type size_type;
	typedef array2< T, Allocator >::difference_type difference_type;
	typedef vector2< double > vector_type;
	array2< size_type > binary;


	// ２値画像に変換
	convert( chart, binary );

	size_type i, j;

	size_type labelnum = labeling4( binary, binary );
	//std::cout << "LabelNum: " << labelnum << std::endl;

	array< bool > mask( labelnum + 1 );
	array< difference_type > count( labelnum + 1 );
	array< difference_type > round( labelnum + 1 );
	array< vector_type > pos( labelnum + 1 );

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
			unsigned short label = binary( i, j );
			if( label != 0 )
			{
				double x = i - pos[ label ].x;
				double y = j - pos[ label ].y;
				const double pai = 3.1415926535897932384626433832795;
				if( x * x + y * y <= count[ label ] / pai * 2.25 )
				{
					round[ binary( i, j ) ]++;
				}
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
		if( count[ i ] < minimum_count )
		{
			count[ i ] = 0;
			mask[ i ] = true;
		}
		else
		{
			const double pai = 3.1415926535897932384626433832795;
			double e = round[ i ] / static_cast< double >( count[ i ] );
			if( e < 0.95 )
			{
				count[ i ] = 0;
				mask[ i ] = true;
			}
			//std::cout << "ラベル: " << i << ", 円形度: " << e << std::endl;
		}
	}

	// 領域の面積の１番大きいものを選ぶ
	difference_type index1 = -1;
	difference_type index2 = -1;
	difference_type index3 = -1;
	difference_type index4 = -1;
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
	// 4番目に大きな領域を見つける
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( i != index1 && i != index2 && i != index3 && count[ i ] > max )
		{
			max = count[ i ];
			index4 = i;
		}
	}

	vector_type p1, p2, p3;
	vector_type p1_ = grid( leftX, leftY );
	vector_type p2_ = grid( leftX, rightY );
	vector_type p3_ = grid( rightX, rightY );
	// 面積の大きい領域で，第4の点を決定する
	{
		difference_type i1 = index1;
		difference_type i2 = index2;
		difference_type i3 = index3;
		difference_type i4 = index4;
		vector_type pp1 = pos[ index1 ];
		vector_type pp2 = pos[ index2 ];
		vector_type pp3 = pos[ index3 ];
		vector_type pp4 = pos[ index4 ];
		double len12 = ( pp1 - pp2 ).length( );
		double len13 = ( pp1 - pp3 ).length( );
		double len14 = ( pp1 - pp4 ).length( );
		double len23 = ( pp2 - pp3 ).length( );
		double len24 = ( pp2 - pp4 ).length( );
		double len34 = ( pp3 - pp4 ).length( );

		// 三角形を構成する３点を選ぶ
		double l123 = len12 + len23 + len13;
		double l124 = len12 + len24 + len14;
		double l134 = len13 + len34 + len14;
		double l234 = len23 + len34 + len24;

		if( l123 >= l124 && l123 >= l134 && l123 >= l234 )
		{
			index1 = i1;
			index2 = i2;
			index3 = i3;
			index4 = i4;
		}
		else if( l124 >= l123 && l124 >= l134 && l124 >= l234 )
		{
			index1 = i1;
			index2 = i2;
			index3 = i4;
			index4 = i3;
		}
		else if( l134 >= l123 && l134 >= l124 && l134 >= l234 )
		{
			index1 = i1;
			index2 = i4;
			index3 = i3;
			index4 = i2;
		}
		else
		{
			index1 = i4;
			index2 = i2;
			index3 = i3;
			index4 = i1;
		}
	}

	// 面積の大きい領域で，Lの左上の点を決定する
	{
		difference_type i1 = index1;
		difference_type i2 = index2;
		difference_type i3 = index3;
		difference_type i4 = index4;
		vector_type pp1 = pos[ index1 ];
		vector_type pp2 = pos[ index2 ];
		vector_type pp3 = pos[ index3 ];
		vector_type pp4 = pos[ index4 ];

		// 第4の点から最も遠い点を選び，それの番号を１にする
		double len14 = ( pp1 - pp4 ).length( );
		double len24 = ( pp2 - pp4 ).length( );
		double len34 = ( pp3 - pp4 ).length( );

		if( len14 >= len24 && len14 >= len34 )
		{
			index1 = i1;
			index2 = i2;
			index3 = i3;
		}
		else if( len24 >= len14 && len24 >= len34 )
		{
			index1 = i2;
			index2 = i1;
			index3 = i3;
		}
		else
		{
			index1 = i3;
			index2 = i1;
			index3 = i2;
		}
	}

	{
		// ベクトルの外積が紙面の手前を向くように割り当てる
		vector_type p12 = pos[ index1 ] - pos[ index2 ];
		vector_type p32 = pos[ index3 ] - pos[ index2 ];
		vector3< double > v12( p12.x, p12.y, 0 );
		vector3< double > v32( p32.x, p32.y, 0 );
		//　vector3< double > v = v32.outer( v12 );
		if( v32.outer( v12 ).z >= 0 )
		{
			difference_type tmp = index2;
			index2 = index3;
			index3 = tmp;
		}

		p1 = pos[ index1 ];
		p2 = pos[ index2 ];
		p3 = pos[ index3 ];
	}


	// 求まった大きな３つの円の配置を元にグリッドの座標を更新する
	difference_type r, c, rows = grid.rows( ), cols = grid.cols( );
	{
		double scaleX = ( p3  - p2 ).length( ) / static_cast< double >( rightX - leftX );
		double scaleY = ( p1  - p2 ).length( ) / static_cast< double >( leftY - rightY );

		if( scaleX < 0 )
		{
			scaleX = -scaleX;
		}
		if( scaleY < 0 )
		{
			scaleY = -scaleY;
		}

		vector_type dX = ( p3  - p2 ).unit( ) * scaleX;
		vector_type dY = ( p1  - p2 ).unit( ) * scaleY;

		for( r = 0 ; r < rows ; r++ )
		{
			for( c = 0 ; c < cols ; c++ )
			{
				vector_type &p = grid( r, c );
				p = p2 + dX * ( c - leftX ) + dY * ( rightY - r );
			}
		}
	}


	// 変換後のグリッド位置が撮影された絵の円内に入っていて，かつ基準ラインから前後に2ライン以内のものを優先的に割り当てる
	matrix< int > found( rows, cols );
	matrix< double > flength( rows, cols );
	for( r = rightY ; r >= rightY - 1 ; r-- )
	{
		for( c = leftX ; c <= rightX ; c++ )
		{
			if( found( r, c ) == 1 )
			{
				continue;
			}

			vector_type &p = grid( r, c );
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
				// 対応点が見つからなかったので，見つからなかったマークを入れる
				p.x = -1;
				p.y = -1;
			}
			else
			{
				// 対応点が見つかったので，以降の探索から除外する
				vector_type diff = pos[ index ] - p;
				p = pos[ index ];
				found( r, c ) = 1;
				mask[ index ] = true;

				if( r == rightY )
				{
					for( difference_type rr = 0 ; rr < rows ; rr++ )
					{
						if( found( rr, c ) == 0 )
						{
							grid( rr, c ) += diff;
						}
					}
				}
			}
		}
	}

	for( r = rightY ; r >= leftY ; r-- )
	{
		for( c = leftX ; c <= leftX + 1 ; c++ )
		{
			if( found( r, c ) == 1 )
			{
				continue;
			}

			vector_type &p = grid( r, c );
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
				// 対応点が見つからなかったので，見つからなかったマークを入れる
				p.x = -1;
				p.y = -1;
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

	// 再度，グリッドのアップデートを行う
	for( r = rightY ; r >= leftY ; r-- )
	{
		for( c = leftX ; c <= rightX ; c++ )
		{
			if( found( r, c ) == 1 )
			{
				continue;
			}

			vector_type &p = grid( r, c );
			grid( r, c ) = grid( r + 1, c ) + grid( r, c - 1 ) - grid( r + 1, c - 1 );

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
				// 対応点が見つからなかったので，見つからなかったマークを入れる
				p.x = -1;
				p.y = -1;
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
					// 対応点が見つからなかったので，見つからなかったマークを入れる
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
