#ifndef __INCLUDE_MIST_DRAW__
#define __INCLUDE_MIST_DRAW__


#include "mist.h"

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#ifdef WIN32
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
//	template <>          struct gl_type_trait< unsigned long > { _MIST_CONST( GLenum, gl_type, true  ); };
	template <>          struct gl_type_trait< signed char >   { _MIST_CONST( GLenum, gl_type, GL_BYTE ); };
	template <>          struct gl_type_trait< signed short >  { _MIST_CONST( GLenum, gl_type, GL_SHORT ); };
	template <>          struct gl_type_trait< signed int >    { _MIST_CONST( GLenum, gl_type, GL_INT ); };
//	template <>          struct gl_type_trait< signed long >   { _MIST_CONST( GLenum, gl_type, true  ); };
	template <>          struct gl_type_trait< bool >          { _MIST_CONST( GLenum, gl_type, GL_UNSIGNED_BYTE ); };
	template <>          struct gl_type_trait< char >          { _MIST_CONST( GLenum, gl_type, GL_BYTE ); };
	template <>          struct gl_type_trait< float >         { _MIST_CONST( GLenum, gl_type, GL_FLOAT ); };
//	template <>          struct gl_type_trait< double >        { _MIST_CONST( GLenum, gl_type, true  ); };
//	template <>          struct gl_type_trait< long double >   { _MIST_CONST( GLenum, gl_type, true  ); };

	template < bool color >
	struct __glTexImage2D__
	{
		template < class T, class Allocator >
		static void glTexImage2D( const array2< T, Allocator > &image )
		{
			::glTexImage2D(
				GL_TEXTURE_2D, 0, 3,
				static_cast< GLsizei >( image.width( ) ),
				static_cast< GLsizei >( image.height( ) ),
				0,
				GL_LUMINANCE,
				gl_type_trait< T >::gl_type,
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
							GL_TEXTURE_2D, 0, 3,
							static_cast< GLsizei >( image.width( ) ),
							static_cast< GLsizei >( image.height( ) ),
							0,
							GL_RGB,
							gl_type_trait< T >::gl_type,
							static_cast< const GLvoid* >( &( image[0] ) )
						  );
		}
	};

	template< class T, class Allocator >
	void glTexImage2D( const array2< T, Allocator > &image )
	{
		__glTexImage2D__< is_color< T >::value >::glTexImage2D( image );
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

template< class T, class Allocator >
bool draw_buffer( const array2< T, Allocator > &image,
					typename array2< T, Allocator >::size_type image_width, typename array2< T, Allocator >::size_type image_height,
					typename array2< T, Allocator >::size_type window_width, typename array2< T, Allocator >::size_type window_height,
					double zoom = 1.0, double xpos = 0.0, double ypos = 0.0, double back_r = 0.0, double back_g = 0.0, double back_b = 0.0, bool interpolate = true )
{
	// 背景の初期化を行う
	typedef typename array2< T, Allocator >::value_type value_type;
	typedef pixel_data::pixel< is_float< value_type >::value > pixel;
	float r = pixel::get_value( back_r );
	float g = pixel::get_value( back_g );
	float b = pixel::get_value( back_b );

	glClearColor( r, g, b, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	if( image_width > image.width( ) || image_height > image.height( ) ) return( false );
	if( image.width( ) != image.height( ) ) return( false );

	typedef typename array2< T, Allocator >::size_type size_type;

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
	glViewport( 0, 0, static_cast< GLint >( window_width ), static_cast< GLint >( window_height ) );

	glPixelStorei( GL_UNPACK_ROW_LENGTH, static_cast< GLint >( image.width( ) ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolate_ );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolate_ );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

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


template< class T, class Allocator >
bool draw_image( const array2< T, Allocator > &image,
					typename array2< T, Allocator >::size_type window_width, typename array2< T, Allocator >::size_type window_height,
					double zoom = 1.0, double xpos = 0.0, double ypos = 0.0, double back_r = 0.0, double back_g = 0.0, double back_b = 0.0, bool interpolate = true )
{
	typedef typename array2< T, Allocator >::size_type size_type;
	size_type size = image.width( ) > image.height( ) ? image.width( ) : image.height( ); 
	size_type ttt = pixel_data::floor_square( size );

	if( ttt == image.width( ) && ttt == image.height( ) )
	{
		return( draw_buffer( image, ttt, ttt, window_width, window_height, zoom, xpos, ypos, back_r, back_g, back_b, interpolate ) );
	}
	else
	{
		static array2< T, Allocator > img;

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
		
		return( draw_buffer( img, image.width( ), image.height( ), window_width, window_height, zoom, xpos, ypos, back_r, back_g, back_b, interpolate ) );
	}
}


// ウィンドウ上の点から画像上の点の座標を計算する
// aspectは sizeY / sizeX であり、各ピクセルのアスペクトである
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

// 画像上の点からウィンドウ上の点の座標を計算する
// aspectは sizeX / sizeY であり、各ピクセルのアスペクトである
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

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DRAW__
