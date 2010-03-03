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

/// @file mist/drawing.h
//!
//! @brief 画像に対して，直線や円，テキストなどを描画するライブラリ
//!
#ifndef __INCLUDE_MIST_DRAWING__
#define __INCLUDE_MIST_DRAWING__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_BITMAP_H__
#include "bitmap.h"
#endif

#include <cmath>

#ifdef MIST_USE_DRAW_TEXT
#include <ft2build.h>
#include FT_FREETYPE_H
#endif


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
	sx = ( x2 - x1 >= 0 ) ? 1 : -1;
	sy = ( y2 - y1 >= 0 ) ? 1 : -1;
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



/// @brief 2次元ビットマップ画像に直線を描画する
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
template < size_t BITS >
void draw_line( bitmap< BITS > &image,
			   typename bitmap< BITS >::difference_type x1,
			   typename bitmap< BITS >::difference_type y1,
			   typename bitmap< BITS >::difference_type x2,
			   typename bitmap< BITS >::difference_type y2,
			   typename bitmap< BITS >::value_type color = typename bitmap< BITS >::value_type( ) )
{
	typedef typename bitmap< BITS >::difference_type difference_type;
	difference_type dx, dy;
	difference_type sx, sy;
	difference_type x, y, e, i;
	difference_type w = static_cast< difference_type >( image.width( ) );
	difference_type h = static_cast< difference_type >( image.height( ) );

	dx = std::abs( static_cast< int >( x2 - x1 ) );
	dy = std::abs( static_cast< int >( y2 - y1 ) );
	sx = ( x2 - x1 >= 0 ) ? 1 : -1;
	sy = ( y2 - y1 >= 0 ) ? 1 : -1;
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
			   typename array3< T, Allocator >::value_type color = typename array3< T, Allocator >::value_type( ) )
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
	sx = ( x2 - x1 >= 0 ) ? 1 : -1;
	sy = ( y2 - y1 >= 0 ) ? 1 : -1;
	sz = ( z2 - z1 >= 0 ) ? 1 : -1;
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
				image( x, y, z ) = color;
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
				image( x, y, z ) = color;
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
				image( x, y, z ) = color;
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



/// @brief 3次元画像に直線を描画する
//! 
//! 手法について何か書く
//! 
//! @param[in,out] image  … 入出力画像
//! @param[in]     x      … 描画する点のX座標
//! @param[in]     y      … 描画する点のY座標
//! @param[in]     radius … 描画する点の半径
//! @param[in]     value  … 描画する色もしくは値
//!
template < class T, class Allocator >
void draw_point( array2< T, Allocator > &image,
				 typename array2< T, Allocator >::difference_type x,
				 typename array2< T, Allocator >::difference_type y,
				 typename array2< T, Allocator >::difference_type radius,
				 typename array2< T, Allocator >::value_type value = typename array3< T, Allocator >::value_type( ) )
{
	typedef typename array2< T, Allocator >::difference_type difference_type;

	difference_type is = x - radius;
	difference_type ie = x + radius;
	difference_type js = y - radius;
	difference_type je = y + radius;

	difference_type w = static_cast< difference_type >( image.width( ) );
	difference_type h = static_cast< difference_type >( image.height( ) );

	if( is < 0 )
	{
		is = 0;
	}
	if( is >= w )
	{
		is = w;
		ie = w - 1;
	}
	if( ie < 0 )
	{
		is = 0;
		ie = -1;
	}
	if( ie >= w )
	{
		ie = w - 1;
	}

	if( js < 0 )
	{
		js = 0;
	}
	if( js >= h )
	{
		js = h;
		je = h - 1;
	}
	if( je < 0 )
	{
		js = 0;
		je = -1;
	}
	if( je >= h )
	{
		je = h - 1;
	}

	for( difference_type j = js ; j <= je ; j++ )
	{
		for( difference_type i = is ; i <= ie ; i++ )
		{
			image( i, j ) = value;
		}
	}
}


/// @brief 3次元画像に直線を描画する
//! 
//! 手法について何か書く
//! 
//! @param[in,out] image  … 入出力画像
//! @param[in]     x      … 描画する点のX座標
//! @param[in]     y      … 描画する点のY座標
//! @param[in]     z      … 描画する点のZ座標
//! @param[in]     radius … 描画する点の半径
//! @param[in]     value  … 描画する色もしくは値
//!
template < class T, class Allocator >
void draw_point( array3< T, Allocator > &image,
				 typename array3< T, Allocator >::difference_type x,
				 typename array3< T, Allocator >::difference_type y,
				 typename array3< T, Allocator >::difference_type z,
				 typename array3< T, Allocator >::difference_type radius,
				 typename array3< T, Allocator >::value_type value = typename array3< T, Allocator >::value_type( ) )
{
	typedef typename array3< T, Allocator >::difference_type difference_type;

	difference_type is = x - radius;
	difference_type ie = x + radius;
	difference_type js = y - radius;
	difference_type je = y + radius;
	difference_type ks = z - radius;
	difference_type ke = z + radius;

	difference_type w = static_cast< difference_type >( image.width( ) );
	difference_type h = static_cast< difference_type >( image.height( ) );
	difference_type d = static_cast< difference_type >( image.depth( ) );

	if( is < 0 )
	{
		is = 0;
	}
	if( is >= w )
	{
		is = w;
		ie = w - 1;
	}
	if( ie < 0 )
	{
		is = 0;
		ie = -1;
	}
	if( ie >= w )
	{
		ie = w - 1;
	}

	if( js < 0 )
	{
		js = 0;
	}
	if( js >= h )
	{
		js = h;
		je = h - 1;
	}
	if( je < 0 )
	{
		js = 0;
		je = -1;
	}
	if( je >= h )
	{
		je = h - 1;
	}

	if( ks < 0 )
	{
		ks = 0;
	}
	if( ks >= d )
	{
		ks = d;
		ke = d - 1;
	}
	if( ke < 0 )
	{
		ks = 0;
		ke = -1;
	}
	if( ke >= d )
	{
		ke = d - 1;
	}

	for( difference_type k = ks ; k <= ke ; k++ )
	{
		for( difference_type j = js ; j <= je ; j++ )
		{
			for( difference_type i = is ; i <= ie ; i++ )
			{
				image( i, j, k ) = value;
			}
		}
	}
}

/// @brief 2次元が像に範囲チェックを行い，値を代入
//! @param[out] image 出力画像
//! @param[in]  x     画素のX座標
//! @param[in]  y     画素のY座標
//! @param[in]  value  … 描画する色もしくは値
template< typename T, typename Allocator >
void set_pixel( array2< T, Allocator > &image,
			   typename array2< T, Allocator >::difference_type x,
			   typename array2< T, Allocator >::difference_type y,
			   const typename array2< T, Allocator >::value_type &value )
{
	typedef typename array2< T, Allocator >::difference_type difference_type;

	// 範囲チェック
	if( x < 0 || y < 0 || x >= static_cast< difference_type >( image.width( ) ) || y >= static_cast< difference_type >( image.height( ) ) )
	{
		return;
	}

	image( x, y ) = value;
}

/// @brief 円を描く(Michenerの手法)
//! @param[out] image 出力画像
//! @param[in]  cx     円中心のX座標
//! @param[in]  cy     円中心のY座標
//! @param[in]  r      円の半径
//! @param[in]  value  … 描画する色もしくは値
template< typename T, typename Allocator >
void draw_circle( array2< T, Allocator > &image, 
				 typename array2< T, Allocator >::difference_type cx, 
				 typename array2< T, Allocator >::difference_type cy, 
				 typename array2< T, Allocator >::difference_type r, 
				 const typename array2< T, Allocator >::value_type &value )
{
	typename array2< T, Allocator >::difference_type d = 3 - 2 * r;
	typename array2< T, Allocator >::difference_type dx;
	typename array2< T, Allocator >::difference_type dy = r;

	// 開始点
	set_pixel( image, cx, cy + r, value );
	set_pixel( image, cx, cy - r, value );
	set_pixel( image, cx + r, cy, value );
	set_pixel( image, cx - r, cy, value );

	for( dx = 0 ; dx <= dy ; ++dx )
	{
		if( d < 0 )
		{
			d += 6 + 4 * dx;	  
		}
		else
		{
			d += 10 + 4 * dx - 4 * dy--;
		}

		set_pixel( image, cx + dy, cy + dx, value );
		set_pixel( image, cx + dx, cy + dy, value );
		set_pixel( image, cx - dx, cy + dy, value );
		set_pixel( image, cx - dy, cy + dx, value );
		set_pixel( image, cx - dy, cy - dx, value );
		set_pixel( image, cx - dx, cy - dy, value );
		set_pixel( image, cx + dx, cy - dy, value );
		set_pixel( image, cx + dy, cy - dx, value );
	}
}

/// @brief 長方形を描く
//! @param[out] image 出力画像
//! @param[in]  x0     左上のX座標
//! @param[in]  y0     左上のY座標
//! @param[in]  x1     右下のX座標
//! @param[in]  y1     右下のY座標
//! @param[in]  value  … 描画する色もしくは値
template< typename T, typename Allocator >
void draw_rect( array2< T, Allocator > &image, 
			   typename array2< T, Allocator >::difference_type x0, 
			   typename array2< T, Allocator >::difference_type y0, 
			   typename array2< T, Allocator >::difference_type x1, 
			   typename array2< T, Allocator >::difference_type y1, 		
			   const typename array2< T, Allocator >::value_type &value )
{
	draw_line( image, x0, y0, x1, y0, value );
	draw_line( image, x0, y0, x0, y1, value );
	draw_line( image, x1, y0, x1, y1, value );
	draw_line( image, x0, y1, x1, y1, value );
}

/// @brief 長方形で塗りつぶす
//! @param[out] image 出力画像
//! @param[in]  x0     左上のX座標
//! @param[in]  y0     左上のY座標
//! @param[in]  x1     右下のX座標
//! @param[in]  y1     右下のY座標
//! @param[in]  value  … 描画する色もしくは値
template< typename T, typename Allocator >
void fill_rect( array2< T, Allocator > &image, 
			   typename array2< T, Allocator >::difference_type x0, 
			   typename array2< T, Allocator >::difference_type y0, 
			   typename array2< T, Allocator >::difference_type x1, 
			   typename array2< T, Allocator >::difference_type y1, 		
			   const typename array2< T, Allocator >::value_type &value )
{
	typedef typename array2< T, Allocator >::difference_type difference_type;

	x0 = ( 0 < x0 ) ? x0 : 0;
	y0 = ( 0 < y0 ) ? y0 : 0;
	x1 = ( x1 < static_cast< difference_type >( image.width() - 1 ) ) ? x1 : static_cast< difference_type >( image.width() - 1 );
	y1 = ( y1 < static_cast< difference_type >( image.height() - 1 ) ) ? y1 : static_cast< difference_type >( image.height() - 1 );

	for( difference_type y = y0 ; y <= y1 ; ++y )
	{
		for( difference_type x = x0 ; x <= x1 ; ++x )
		{
			image( x, y ) = value;
		}
	}
}

/// @brief 十字点を描画
//! @param[out] image 出力画像
//! @param[in]  cx     中心のX座標
//! @param[in]  cy     中心のY座標
//! @param[in]  size     十字点の大きさ
//! @param[in]  value  … 描画する色もしくは値
template< typename T, typename Allocator >
void draw_crosspoint( array2< T, Allocator > &image, 
					 typename array2< T, Allocator >::difference_type cx, 
					 typename array2< T, Allocator >::difference_type cy, 
					 typename array2< T, Allocator >::difference_type size, 
					 const typename array2< T, Allocator >::value_type &value )
{
	draw_line( image, cx - size / 2, cy, cx + size / 2, cy, value );
	draw_line( image, cx, cy - size / 2, cx, cy + size / 2, value );
}



#ifdef MIST_USE_DRAW_TEXT


/// @brief Draw a UTF-32 code text
//! 
//! FreeType2 is required.
//! Define `MIST_USE_DRAW_TEXT' to use this function.
//! TureType fonts (.ttf) are recommended to be used.
//! 
//! @param[in,out] img is a image
//! @param[in]     filename is a font file name
//! @param[in]	   px is a X coordinate where the baseline starts
//! @param[in]     py is a Y coordinate where the baseline starts
//! @param[in]     size is a font size in pixels
//! @param[in]     color is a font color
//! @param[in]     str is an array of UTF-32 values
//! @param[in]     len is a length of str
//!
//! @return        result
//!
template < typename T, typename Allocator >
inline bool draw_text( array2< T, Allocator > &img, 
					   char *filename, 
					   int px, 
					   int py, 
					   int size, 
					   typename array2< T, Allocator >::value_type color, 
					   unsigned long *str, 
					   int len )
{
	
	FT_Library library = NULL;
	FT_Face    face    = NULL;
	FT_Error   error;	

	try
	{
		error = FT_Init_FreeType( &library );
		if ( error )
		{
			library = NULL;
			throw error;
		}

		error = FT_New_Face( library, filename, 0, &face );
		if ( error )
		{
			face = NULL;
			throw error;
		}

		error = FT_Set_Pixel_Sizes( face, 0, size );
		if ( error )
		{
			throw error;
		}

		int pen_x = px;
		int pen_y = py;
		for ( int i = 0; i < len; i++)
		{
			FT_UInt glyph_index = FT_Get_Char_Index( face, (FT_ULong)str[i] );
			
			error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
			if ( error )
			{
				throw error;
			}

			error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
			if ( error )
			{
				throw error;
			}

			for (int x = 0; x < face->glyph->bitmap.width; x++)
			{
				for (int y = 0; y < face->glyph->bitmap.rows; y++)
				{
					int xx = x + pen_x + face->glyph->bitmap_left;
					int yy = y + pen_y - face->glyph->bitmap_top;
					if ( ( 0 <= xx ) && ( xx < (int)img.width() ) && ( 0 <= yy ) && ( yy < (int)img.height() ) )
					{
						// alpha blending
						T img_value = img(xx, yy);
						double str_value = static_cast< double > ( face->glyph->bitmap.buffer[ x + y * face->glyph->bitmap.width] ) / 255.0;
						img(xx, yy) = static_cast< typename array2<T>::value_type > ( str_value * color + (1.0 - str_value) * img_value );
					}
				}
			}
			pen_x += face->glyph->advance.x >> 6;
			
		}
		
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		
	}
	catch ( ... )
	{
		if (face != NULL)
		{
			FT_Done_Face(face);
		}
		if (library != NULL)
		{
			FT_Done_FreeType(library);
		}
		return false;
	}

	return true;

}



/// @brief Draw a ASCII code text
//!  
//! FreeType2 is required.
//! Define `MIST_USE_DRAW_TEXT' to use this function.
//! TureType fonts (.ttf) are recommended to be used.
//! 
//! @param[in,out] img is a image
//! @param[in]     filename is a font file name
//! @param[in]	   px is a X coordinate where the baseline starts
//! @param[in]     py is a Y coordinate where the baseline starts
//! @param[in]     size is a font size in pixels
//! @param[in]     color is a font color
//! @param[in]     str is an array of ASCII codes, required to finish by 0x00
//!
//! @return        result
//!
template < typename T, typename Allocator >
inline bool draw_text( array2< T, Allocator > &img,
					   char *filename,
					   int px,
					   int py,
					   int size,
					   typename array2< T, Allocator >::value_type color,
					   char *str )
{
	int len = strlen(str);
	
	unsigned long *str_ulong = new unsigned long[len];

	for (int i = 0; i < len; i++)
	{
		str_ulong[i] = (FT_ULong)str[i];
	}

	bool result = draw_text( img, filename, px, py, size, color, str_ulong, len );

	delete []str_ulong;

	return result;

}


#endif // MIST_USE_DRAW_TEXT


/// @}
//  直線や円の描画グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DRAWING__
