/// @file mist/drawing.h
//!
//! @brief 画像に対して，直線や円などを描画するライブラリ
//!
#ifndef __INCLUDE_MIST_DRAWING__
#define __INCLUDE_MIST_DRAWING__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#include <cmath>


// mist名前空間の始まり
_MIST_BEGIN




//! @addtogroup image_drawing_group 直線や円の描画
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/drawing.h>
//! @endcode
//!
//!  @{



/// @brief 2次元画像に直線を描画する
//! 
//! 手法について何か書く
//! 
//! @param[in,out] image … 入出力画像
//! @param[in]     x1    … 直線の端点1のX座標
//! @param[in]     y1    … 直線の端点1のY座標
//! @param[in]     x2    … 直線の端点2のX座標
//! @param[in]     y2    … 直線の端点2のY座標
//! @param[in]     color … 直線の色もしくは値
//!
template < class T, class Allocator >
void draw_line( array2< T, Allocator > &image,
			   typename array2< T, Allocator >::difference_type x1,
			   typename array2< T, Allocator >::difference_type y1,
			   typename array2< T, Allocator >::difference_type x2,
			   typename array2< T, Allocator >::difference_type y2,
			   typename array2< T, Allocator >::value_type color = typename array2< T, Allocator >::value_type( ) )
{
	typedef typename array2< T, Allocator >::difference_type difference_type;
	difference_type dx, dy;
	difference_type sx, sy;
	difference_type x, y, e, i;
	difference_type w = static_cast< difference_type >( image.width( ) );
	difference_type h = static_cast< difference_type >( image.height( ) );

	dx = std::abs( static_cast< int >( x2 - x1 ) );
	dy = std::abs( static_cast< int >( y2 - y1 ) );
	sx = ( x2 - x1 >=0 ) ? 1 : -1;
	sy = ( y2 - y1 >=0 ) ? 1 : -1;
	x = x1;
	y = y1;
	if( dx >= dy )
	{
		e = -dx;
		for( i = 0 ; i <= dx ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h )
			{
				image( x, y ) = color;
			}
			x += sx;
			e += 2 * dy;

			if( e >= 0 )
			{
				y += sy;
				e -= 2 * dx;
			}
		}
	}
	else
	{
		e = -dy;
		for( i = 0 ; i <= dy ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h )
			{
				image( x, y ) = color;
			}
			y += sy;
			e += 2 * dx;
			if( e >= 0 )
			{
				x += sx;
				e -= 2 * dy;
			}
		}
	}
}



/// @brief 3次元画像に直線を描画する
//! 
//! 手法について何か書く
//! 
//! @param[in,out] image … 入出力画像
//! @param[in]     x1    … 直線の端点1のX座標
//! @param[in]     y1    … 直線の端点1のY座標
//! @param[in]     z1    … 直線の端点1のZ座標
//! @param[in]     x2    … 直線の端点2のX座標
//! @param[in]     y2    … 直線の端点2のY座標
//! @param[in]     z2    … 直線の端点2のZ座標
//! @param[in]     color … 直線の色もしくは値
//!
template < class T, class Allocator >
void draw_line( array3< T, Allocator > &image,
			   typename array3< T, Allocator >::difference_type x1,
			   typename array3< T, Allocator >::difference_type y1,
			   typename array3< T, Allocator >::difference_type z1,
			   typename array3< T, Allocator >::difference_type x2,
			   typename array3< T, Allocator >::difference_type y2,
			   typename array3< T, Allocator >::difference_type z2,
			   typename array3< T, Allocator >::value_type value = typename array3< T, Allocator >::value_type( ) )
{
	typedef typename array3< T, Allocator >::difference_type difference_type;
	difference_type dx, dy, dz;
	difference_type sx, sy, sz;
	difference_type x, y, z, e1, e2, i;
	difference_type w = static_cast< difference_type >( image.width( ) );
	difference_type h = static_cast< difference_type >( image.height( ) );
	difference_type d = static_cast< difference_type >( image.depth( ) );

	dx = std::abs( static_cast< int >( x2 - x1 ) );
	dy = std::abs( static_cast< int >( y2 - y1 ) );
	dz = std::abs( static_cast< int >( z2 - z1 ) );
	sx = ( x2 - x1 >=0 ) ? 1 : -1;
	sy = ( y2 - y1 >=0 ) ? 1 : -1;
	sz = ( z2 - z1 >=0 ) ? 1 : -1;
	x = x1;
	y = y1;
	z = z1;
	if( dx >= dy && dx >= dz )
	{
		e1 = e2 = -dx;
		for( i = 0 ; i <= dx ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h && z >= 0 && z < d )
			{
				image( x, y, z ) = value;
			}
			x += sx;
			e1 += 2 * dy;
			e2 += 2 * dz;

			if( e1 >= 0 )
			{
				y += sy;
				e1 -= 2 * dx;
			}

			if( e2 >= 0 )
			{
				z += sz;
				e2 -= 2 * dx;
			}
		}
	}
	else if( dy >= dx && dy >= dz )
	{
		e1 = e2 = -dy;
		for( i = 0 ; i <= dy ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h && z >= 0 && z < d )
			{
				image( x, y, z ) = value;
			}
			y += sy;
			e1 += 2 * dx;
			e2 += 2 * dz;

			if( e1 >= 0 )
			{
				x += sx;
				e1 -= 2 * dy;
			}

			if( e2 >= 0 )
			{
				z += sz;
				e2 -= 2 * dy;
			}
		}
	}
	else
	{
		e1 = e2 = -dz;
		for( i = 0 ; i <= dz ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h && z >= 0 && z < d )
			{
				image( x, y, z ) = value;
			}
			z += sz;
			e1 += 2 * dx;
			e2 += 2 * dy;

			if( e1 >= 0 )
			{
				x += sx;
				e1 -= 2 * dz;
			}

			if( e2 >= 0 )
			{
				y += sy;
				e2 -= 2 * dz;
			}
		}
	}
}



/// @}
//  直線や円の描画グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DRAWING__
