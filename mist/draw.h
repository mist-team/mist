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

/// @file mist/draw.h
//!
//! @brief ２次元画像をOpenGLを用いて描画するライブラリ
//!
#ifndef __INCLUDE_MIST_DRAW__
#define __INCLUDE_MIST_DRAW__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_BITMAP_H__
#include "bitmap.h"
#endif


#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>


// mist名前空間の始まり
_MIST_BEGIN


namespace pixel_data
{
	// 浮動小数点の場合
	template < bool b >
	struct pixel
	{
		static float get_value( double v ){ return( static_cast< float >( v ) ); }
	};

	template < >
	struct pixel< false >
	{
		static float get_value( double v ){ return( static_cast< float >( v ) / 255.0f ); }
	};

	template < class T > struct gl_type_trait { _MIST_CONST( GLenum, gl_type, 0 ); };
	template <>          struct gl_type_trait< unsigned char > { _MIST_CONST( GLenum, gl_type, GL_UNSIGNED_BYTE ); };
	template <>          struct gl_type_trait< unsigned short >{ _MIST_CONST( GLenum, gl_type, GL_UNSIGNED_SHORT ); };
	template <>          struct gl_type_trait< unsigned int >  { _MIST_CONST( GLenum, gl_type, GL_UNSIGNED_INT ); };
	template <>          struct gl_type_trait< unsigned long > { _MIST_CONST( GLenum, gl_type, GL_UNSIGNED_INT  ); };
	template <>          struct gl_type_trait< signed char >   { _MIST_CONST( GLenum, gl_type, GL_BYTE ); };
	template <>          struct gl_type_trait< signed short >  { _MIST_CONST( GLenum, gl_type, GL_SHORT ); };
	template <>          struct gl_type_trait< signed int >    { _MIST_CONST( GLenum, gl_type, GL_INT ); };
	template <>          struct gl_type_trait< signed long >   { _MIST_CONST( GLenum, gl_type, GL_INT  ); };
	template <>          struct gl_type_trait< bool >          { _MIST_CONST( GLenum, gl_type, GL_UNSIGNED_BYTE ); };
	template <>          struct gl_type_trait< char >          { _MIST_CONST( GLenum, gl_type, GL_BYTE ); };
	template <>          struct gl_type_trait< float >         { _MIST_CONST( GLenum, gl_type, GL_FLOAT ); };
	template <>          struct gl_type_trait< double >        { _MIST_CONST( GLenum, gl_type, GL_DOUBLE  ); };
//	template <>          struct gl_type_trait< long double >   { _MIST_CONST( GLenum, gl_type, true  ); };

	template < bool color >
	struct __glTexImage2D__
	{
		template < class Array >
		static void glTexImage2D( const Array &image )
		{
			::glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA,
				static_cast< GLsizei >( image.width( ) ),
				static_cast< GLsizei >( image.height( ) ),
				0,
				GL_LUMINANCE,
				gl_type_trait< typename Array::value_type >::gl_type,
				static_cast< const GLvoid* >( &( image[0] ) )
				);
		}
	};

	template < >
	struct __glTexImage2D__< true >
	{
		template < class T, class Allocator >
		static void glTexImage2D( const array2< rgb< T >, Allocator > &image )
		{
			::glTexImage2D(
							GL_TEXTURE_2D, 0, GL_RGBA,
							static_cast< GLsizei >( image.width( ) ),
							static_cast< GLsizei >( image.height( ) ),
							0,
							GL_RGB,
							gl_type_trait< T >::gl_type,
							static_cast< const GLvoid* >( &( image[0] ) )
						  );
		}

		template < size_t BITS, class Allocator >
		static void glTexImage2D( const bitmap< BITS, Allocator > &image )
		{
			::glTexImage2D(
							GL_TEXTURE_2D, 0, GL_RGBA,
							static_cast< GLsizei >( image.width( ) ),
							static_cast< GLsizei >( image.height( ) ),
							0,
							GL_BGR_EXT,
							gl_type_trait< unsigned char >::gl_type,
							static_cast< const GLvoid* >( &( image[0] ) )
						  );
		}

		template < class T, class Allocator >
		static void glTexImage2D( const array2< rgba< T >, Allocator > &image )
		{
			::glTexImage2D(
							GL_TEXTURE_2D, 0, GL_RGBA,
							static_cast< GLsizei >( image.width( ) ),
							static_cast< GLsizei >( image.height( ) ),
							0,
							GL_RGBA,
							gl_type_trait< T >::gl_type,
							static_cast< const GLvoid* >( &( image[0] ) )
						  );
		}
	};

	template < bool color >
	struct __glDrawPixels__
	{
		template < class Array >
		static void glDrawPixels( const Array &image, typename Array::size_type w, typename Array::size_type h )
		{
			::glDrawPixels(
					static_cast< GLsizei >( w ),
					static_cast< GLsizei >( h ),
					GL_LUMINANCE,
					gl_type_trait< typename Array::value_type >::gl_type,
					static_cast< const GLvoid* >( &( image[0] ) )
				);
		}
	};

	template < >
	struct __glDrawPixels__< true >
	{
		template < class T, class Allocator >
		static void glDrawPixels( const array2< rgb< T >, Allocator > &image, typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h )
		{
			::glDrawPixels(
					static_cast< GLsizei >( w ),
					static_cast< GLsizei >( h ),
					GL_RGB,
					gl_type_trait< T >::gl_type,
					static_cast< const GLvoid* >( &( image[0] ) )
				);
		}

		template < size_t BITS, class Allocator >
		static void glDrawPixels( const bitmap< BITS, Allocator > &image, typename bitmap< BITS, Allocator >::size_type w, typename bitmap< BITS, Allocator >::size_type h )
		{
			::glDrawPixels(
					static_cast< GLsizei >( w ),
					static_cast< GLsizei >( h ),
					GL_BGR_EXT,
					gl_type_trait< unsigned char >::gl_type,
					static_cast< const GLvoid* >( &( image[0] ) )
				);
		}

		template < class T, class Allocator >
		static void glDrawPixels( const array2< rgba< T >, Allocator > &image, typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h )
		{
			::glDrawPixels(
					static_cast< GLsizei >( w ),
					static_cast< GLsizei >( h ),
					GL_RGBA,
					gl_type_trait< T >::gl_type,
					static_cast< const GLvoid* >( &( image[0] ) )
				);
		}
	};

	template< class Array >
	void glTexImage2D( const Array &image )
	{
		__glTexImage2D__< is_color< typename Array::value_type >::value >::glTexImage2D( image );
	}

	template< class Array >
	void glDrawPixels( const Array &image, typename Array::size_type w, typename Array::size_type h )
	{
		__glDrawPixels__< is_color< typename Array::value_type >::value >::glDrawPixels( image, w, h );
	}


	inline size_t floor_square_index( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( i );
			}
			_2 *= 2;
		}

		return( 0 );
	}

	inline size_t floor_square( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( _2 );
			}
			_2 *= 2;
		}

		return( 0 );
	}

	// 入力された v が2の指数乗であるかどうかをチェックする
	inline bool is_floor_square( size_t v )
	{
		return( ( v & ( v - 1 ) ) == 0 );
	}
}


//! @addtogroup image_draw_group 画像描画
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/draw.h>
//! @endcode
//!
//!  @{


/// @brief 2次元画像をOpenGLを用いてウィンドウに描画する
//! 
//! OpenGLのテクスチャとして，2次元画像をウィンドウに描画する
//! 描画される絵は，ウィンドウの縦と横の短いほうの辺に画像端が張り付くように描画される
//!
//! @attention 入力画像の一辺は，2の指数乗になっている必要がある
//! 
//! @param[in] image         … 入力画像
//! @param[in] image_width   … 入力画像内で，画面に描画する幅
//! @param[in] image_height  … 入力画像内で，画面に描画する高さ
//! @param[in] window_width  … 出力ウィンドウの幅
//! @param[in] window_height … 出力ウィンドウの高さ
//! @param[in] zoom          … 拡大率（0より大きい値を指定．デフォルトは1.0）
//! @param[in] xpos          … 画像をX軸方向にどれだけずらして描画するか（デフォルトは0）
//! @param[in] ypos          … 画像をY軸方向にどれだけずらして描画するか（デフォルトは0）
//! @param[in] interpolate   … 画素の間を補間し，スクリーン上で滑らかにする（デフォルトはtrue）
//!
//! @retval true  … 描画に成功
//! @retval false … 不適切な入力画像の場合
//! 
template< class Array >
bool draw_buffer( const Array &image,
					typename Array::size_type image_width, typename Array::size_type image_height,
					typename Array::size_type window_width, typename Array::size_type window_height,
					double zoom = 1.0, double xpos = 0.0, double ypos = 0.0, bool interpolate = true )
{
	// 背景の初期化を行う
	typedef typename Array::value_type value_type;
	typedef pixel_data::pixel< is_float< value_type >::value > pixel;

	if( image_width > image.width( ) || image_height > image.height( ) ) return( false );
	if( image.width( ) != image.height( ) ) return( false );

	typedef typename Array::size_type size_type;

	// テクスチャのサイズが2の指数上になっているかどうかをチェック
	if( !pixel_data::is_floor_square( image.width( ) ) ) return( false );


	if( image.empty( ) ) return( false );

	double ttt = static_cast< double >( image.width( ) );
	int interpolate_ = interpolate ? GL_LINEAR: GL_NEAREST;

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	GLboolean isLighting = glIsEnabled( GL_LIGHTING );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );

	glViewport( 0, 0, static_cast< GLint >( window_width ), static_cast< GLint >( window_height ) );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glPixelStorei( GL_UNPACK_ROW_LENGTH, static_cast< GLint >( image.width( ) ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolate_ );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolate_ );
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	pixel_data::glTexImage2D( image );

	glEnable( GL_TEXTURE_2D );

	double v0[2], v1[2], v2[2], v3[2], t0[2], t1[2], t2[2], t3[2];
	double imgW = static_cast< double >( image_width );
	double imgH = static_cast< double >( image_height );
	double winW = static_cast< double >( window_width );
	double winH = static_cast< double >( window_height );

	t0[0] = 0.0f;
	t0[1] = imgH / ttt;
	t1[0] = imgW / ttt;
	t1[1] = imgH / ttt;
	t2[0] = imgW / ttt;
	t2[1] = 0.0;
	t3[0] = 0.0;
	t3[1] = 0.0;

	v0[0] = ( -1.0 - xpos / imgW ) * zoom;
	v0[1] = ( -1.0 + ypos / imgH ) * zoom;
	v1[0] = (  1.0 - xpos / imgW ) * zoom;
	v1[1] = ( -1.0 + ypos / imgH ) * zoom;
	v2[0] = (  1.0 - xpos / imgW ) * zoom;
	v2[1] = (  1.0 + ypos / imgH ) * zoom;
	v3[0] = ( -1.0 - xpos / imgW ) * zoom;
	v3[1] = (  1.0 + ypos / imgH ) * zoom;

	double win_aspect = winH / winW;
	double aspect = imgH * image.reso2( ) / image.reso1( ) / imgW;
	if( win_aspect < aspect )
	{
		v0[0] *= win_aspect / aspect;
		v1[0] *= win_aspect / aspect;
		v2[0] *= win_aspect / aspect;
		v3[0] *= win_aspect / aspect;
		//fprintf(stdout, "draw1 %f %f (%f,%f) (%f,%f)\r", win_aspect, double(ia->h)*aspect/double(ia->w), v0[0], v0[1], v1[0], v1[1]);
	}
	else
	{
		v0[1] /= win_aspect / aspect;
		v1[1] /= win_aspect / aspect;
		v2[1] /= win_aspect / aspect;
		v3[1] /= win_aspect / aspect;
		//fprintf(stdout, "draw2 %f %f (%f,%f) (%f,%f)\r", win_aspect, double(ia->h)*aspect/double(ia->w), v0[0], v0[1], v1[0], v1[1]);
	}

	glBegin( GL_QUADS );
		glTexCoord2d( t0[0], t0[1] );
		glVertex2d  ( v0[0], v0[1] );
		glTexCoord2d( t1[0], t1[1] );
		glVertex2d  ( v1[0], v1[1] );
		glTexCoord2d( t2[0], t2[1] );
		glVertex2d  ( v2[0], v2[1] );
		glTexCoord2d( t3[0], t3[1] );
		glVertex2d  ( v3[0], v3[1] );
	glEnd( );

	glDisable( GL_TEXTURE_2D );

	if( isLighting == GL_TRUE ) glEnable( GL_LIGHTING );

	return( true );
}


/// @brief 2次元画像をOpenGLを用いてウィンドウに描画する
//! 
//! OpenGLのテクスチャとして，2次元画像をウィンドウに描画する
//! 描画される絵は，ウィンドウの縦と横の短いほうの辺に画像端が張り付くように描画される
//!
//! @attention 入力画像の一辺は，2の指数乗でなくても正しく描画される
//! 
//! @param[in] image         … 入力画像
//! @param[in] window_width  … 出力ウィンドウの幅
//! @param[in] window_height … 出力ウィンドウの高さ
//! @param[in] zoom          … 拡大率（0より大きい値を指定．デフォルトは1.0）
//! @param[in] xpos          … 画像をX軸方向にどれだけずらして描画するか（デフォルトは0）
//! @param[in] ypos          … 画像をY軸方向にどれだけずらして描画するか（デフォルトは0）
//! @param[in] back_r        … 塗りつぶす背景色の赤成分（デフォルトは0）
//! @param[in] back_g        … 塗りつぶす背景色の緑成分（デフォルトは0）
//! @param[in] back_b        … 塗りつぶす背景色の青成分（デフォルトは0）
//! @param[in] interpolate   … 画素の間を補間し，スクリーン上で滑らかにする（デフォルトはtrue）
//! @param[in] blend         … すでに描画されている物とブレンドするかどうか（デフォルトはfalse）
//!
//! @retval true  … 描画に成功
//! @retval false … 不適切な入力画像の場合
//! 
//! @code 使用例
//! #include <mist/draw.h>
//! 
//! mist::array2< mist::rgb< unsigned char > > image; ←画像読み込みルーチンなどで，画像を読み込んでおく．
//! 
//! mist::draw_image( image,         // 入力画像
//!                   window_width,  // 出力するウィンドウの幅
//!                   window_height, // 出力するウィンドウの高さ
//!                   zoom,          // ズームの割合（デフォルトは1倍）
//!                   xpos,          // 中心からX軸方向にずらす割合（デフォルトは0）
//!                   ypos,          // 中心からY軸方向にずらす割合（デフォルトは0）
//!                   back_r,        // 塗りつぶす背景色の赤成分（デフォルトは0）
//!                   back_g,        // 塗りつぶす背景色の緑成分（デフォルトは0）
//!                   back_b,        // 塗りつぶす背景色の青成分（デフォルトは0）
//!                   interpolate,   // 画素の間を補間し，スクリーン上で滑らかにする（デフォルトはtrue）
//!                   blend          // すでに描画されている物とブレンドするかどうか（デフォルトはfalse）
//!                 );
//! @endcode
//!
template< class Array >
bool draw_image( const Array &image, typename Array::size_type window_width, typename Array::size_type window_height,
					double zoom = 1.0, double xpos = 0.0, double ypos = 0.0, double back_r = 0.0, double back_g = 0.0, double back_b = 0.0, bool interpolate = true, bool blend = false )
{
	typedef typename Array::size_type size_type;
	typedef typename Array::value_type value_type;
	typedef pixel_data::pixel< is_float< value_type >::value > pixel;
	size_type size = image.width( ) > image.height( ) ? image.width( ) : image.height( ); 
	size_type ttt = pixel_data::floor_square( size );

	if( !blend )
	{
		float r = pixel::get_value( back_r );
		float g = pixel::get_value( back_g );
		float b = pixel::get_value( back_b );

		glClearColor( r, g, b, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

	if( ttt == image.width( ) && ttt == image.height( ) )
	{
		return( draw_buffer( image, ttt, ttt, window_width, window_height, zoom, xpos, ypos, interpolate ) );
	}
	else
	{
		static Array img;

		if( ttt > img.width( ) )
		{
			img.resize( ttt, ttt );
		}
		img.reso1( image.reso1( ) );
		img.reso2( image.reso2( ) );

		size_type i, j;
		for( j = 0 ; j < image.height( ) ; j++ )
		{
			for( i = 0 ; i < image.width( ) ; i++ )
			{
				img( i, j ) = image( i, j );
			}
		}
		
		return( draw_buffer( img, image.width( ), image.height( ), window_width, window_height, zoom, xpos, ypos, interpolate ) );
	}
}


/// @brief 2次元画像をOpenGLを用いてウィンドウに描画する
//! 
//! OpenGLの glDrawPixels 関数を用いて，ウィンドウ上に画像を描画する
//! 描画される絵は，ウィンドウの１画素と画像の１画素が対応するように描画される
//!
//! @attention 入力画像の一辺は，2の指数乗でなくても正しく描画される
//! 
//! @param[in] image         … 入力画像
//! @param[in] window_width  … 出力ウィンドウの幅
//! @param[in] window_height … 出力ウィンドウの高さ
//! @param[in] xpos          … 画像をX軸方向に何画素ずらして描画するか（デフォルトは0）
//! @param[in] ypos          … 画像をY軸方向に何画素ずらして描画するか（デフォルトは0）
//! @param[in] blend         … すでに描画されている物とブレンドするかどうか（デフォルトはfalse）
//!
//! @retval true  … 描画に成功
//! @retval false … 不適切な入力画像の場合
//! 
//! @code 使用例
//! #include <mist/draw.h>
//! 
//! mist::array2< mist::rgb< unsigned char > > image; ←画像読み込みルーチンなどで，画像を読み込んでおく．
//! 
//! mist::draw_image( image,         // 入力画像
//!                   window_width,  // 出力するウィンドウの幅
//!                   window_height, // 出力するウィンドウの高さ
//!                   zoom,          // ズームの割合（デフォルトは1倍）
//!                   xpos,          // 中心からX軸方向にずらす割合（デフォルトは0）
//!                   ypos,          // 中心からY軸方向にずらす割合（デフォルトは0）
//!                   blend          // すでに描画されている物とブレンドするかどうか（デフォルトはfalse）
//!                 );
//! @endcode
//!
template< class Array >
bool draw_pixels( const Array &image, typename Array::size_type window_width, typename Array::size_type window_height,
									typename Array::difference_type xpos = 0, typename Array::difference_type ypos = 0, bool blend = false )
{
	typedef typename Array::size_type size_type;
	typedef typename Array::value_type value_type;
	typedef pixel_data::pixel< is_float< value_type >::value > pixel;

	if( image.empty( ) ) return( false );

	if( !blend )
	{
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

	GLsizei w = static_cast< GLsizei >( window_width / 2 );
	GLsizei h = static_cast< GLsizei >( window_height / 2 );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( -w, w, -h, h );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// 左上を原点とする画像座標系を設定
	gluLookAt( w, h, -1, w, h, 0, 0, -1, 0 );

	GLboolean isLighting = glIsEnabled( GL_LIGHTING );
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	size_type ww = image.width( );
	size_type hh = image.height( );

	glPixelStorei( GL_UNPACK_ROW_LENGTH, static_cast< GLint >( image.width( ) ) );
	if( xpos < 0 )
	{
		glPixelStorei( GL_UNPACK_SKIP_PIXELS, static_cast< GLint >( -xpos ) );
		ww = ww + xpos;
		xpos = 0;
	}
	if( ypos < 0 )
	{
		glPixelStorei( GL_UNPACK_SKIP_ROWS, static_cast< GLint >( -ypos ) );
		hh = hh + ypos;
		ypos = 0;
	}

	glRasterPos2i( static_cast< GLsizei >( xpos ), static_cast< GLsizei >( ypos ) );
	glPixelZoom( 1.0f, -1.0f );
	pixel_data::glDrawPixels( image, ww, hh );

	if( isLighting == GL_TRUE ) glEnable( GL_LIGHTING );

	return( true );
}


/// @brief ウィンドウ座標系の点を画像座標系の点に変換する
//! 
//! ウィンドウ上でクリックされた点等を，画像座標系での点に変換する
//! 
//! @param[in] pt     … ウィンドウ座標系における点
//! @param[in] imgX   … 描画に用いている画像の幅
//! @param[in] imgY   … 描画に用いている画像の高さ
//! @param[in] aspect … アスペクト（Y軸方向の画素サイズ / X軸方向の画素サイズ）
//! @param[in] winW   … 出力ウィンドウの幅
//! @param[in] winH   … 出力ウィンドウの高さ
//! @param[in] zoom   … 拡大率
//! @param[in] posx   … 画像をX軸方向にどれだけずらして描画するか
//! @param[in] posy   … 画像をY軸方向にどれだけずらして描画するか
//!
//! @return 画像座標系における点の位置
//! 
inline vector2< double > screen2point( const vector2< double > &pt, double imgX, double imgY, double aspect,
												double winW, double winH, double zoom, double posx, double posy )
{
	if( imgX == 0.0 || imgY == 0.0 || winW == 0.0 || winH == 0.0 )
	{
		return( vector2< double >( 0.0, 0.0 ) );
	}

	double xx = ( pt.x - winW / 2.0 ) / winW * 2.0;
	double yy = ( winH / 2.0 - pt.y ) / winH * 2.0;
	double win_aspect = winH / winW;
	aspect = imgY * aspect / imgX;
	if( win_aspect < aspect )
	{
		xx = xx / win_aspect * aspect / zoom + posx / imgX;
		yy = -( yy / zoom - posy / imgY );
	}
	else
	{
		xx = xx / zoom + posx / imgX;
		yy = -( yy * win_aspect / aspect / zoom - posy / imgY );
	}

	return( vector2< double >( ( xx + 1.0 ) * imgX * 0.5, ( yy + 1.0 ) * imgY * 0.5 ) );
}

/// @brief 画像座標系の点をウィンドウ座標系の点に変換する
//! 
//! 画像上の特定の点をウィンドウ上に反映させたい時などに使用する
//! 
//! @param[in] pt     … 画像座標系における点
//! @param[in] imgX   … 描画に用いている画像の幅
//! @param[in] imgY   … 描画に用いている画像の高さ
//! @param[in] aspect … アスペクト（Y軸方向の画素サイズ / X軸方向の画素サイズ）
//! @param[in] winW   … 出力ウィンドウの幅
//! @param[in] winH   … 出力ウィンドウの高さ
//! @param[in] zoom   … 拡大率
//! @param[in] posx   … 画像をX軸方向にどれだけずらして描画するか
//! @param[in] posy   … 画像をY軸方向にどれだけずらして描画するか
//!
//! @return ウィンドウ座標系における点の位置
//! 
inline vector2< double > point2screen( const vector2< double > &pt, double imgX, double imgY, double aspect,
												double winW, double winH, double zoom, double posx, double posy )
{
	if( imgX == 0.0 || imgY == 0.0 || winW == 0.0 || winH == 0.0 )
	{
		return( vector2< double >( 0.0, 0.0 ) );
	}

	double xx = ( pt.x - imgX / 2.0 ) / imgX * 2.0;
	double yy = ( imgY / 2.0 - pt.y ) / imgY * 2.0;
	double win_aspect = winH / winW;
	aspect = imgY * aspect / imgX;
	if( win_aspect < aspect )
	{
		xx = ( xx - posx / imgX ) * zoom * win_aspect / aspect;
		yy = ( yy + posy / imgY ) * zoom;
	}
	else
	{
		xx = ( xx - posx / imgX ) * zoom;
		yy = ( yy + posy / imgY ) * zoom / win_aspect * aspect;
	}
	return( vector2< double >( ( xx + 1.0 ) * winW * 0.5, ( 1.0 - yy ) * winH * 0.5 ) );
}


/// @}
//  画像描画グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DRAW__
