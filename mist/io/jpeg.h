#ifndef __INCLUDE_MIST_JPEG__
#define __INCLUDE_MIST_JPEG__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <string>

#ifdef WIN32

	#include <windows.h>
	#define XMD_H
	#define HAVE_INT32			// JPEG用INT32型を持っている宣言
	#define HAVE_BOOLEAN		// JPEG用boolean型を持っている宣言

#endif

extern "C"
{
//#include <jinclude.h>
#include <jpeglib.h>
#include <jerror.h>
}

#ifdef WIN32
	#ifdef _DEBUG
		#pragma comment(lib, "libjpegd.lib")
	#else
		#pragma comment(lib, "libjpeg.lib")
	#endif
#endif


// mist名前空間の始まり
_MIST_BEGIN


namespace __jpeg_controller__
{
	template < class T, class Allocator >
	struct jpeg_controller
	{
		static bool read_jpeg( array2< T, Allocator > &image, const std::string &filename )
		{
			array2< rgb< T >, Allocator > img;
			bool ret = jpeg_controller< rgb< T >, Allocator >::read_jpeg( img, filename );
			if( !ret ) return( false );

			array2< T, Allocator >::size_type i;
			image.resize( img.width( ), img.height( ) );
			for( i = 0 ; i < img.size( ) ; i++ )
			{
				image[i] = img[i].get_value( );
			}

			return( true );
		}

		static bool write_jpeg( array2< T, Allocator > &image, const std::string &filename, int quality = 100 )
		{
			array2< rgb< T >, Allocator > img( image.width( ), image.height( ) );
			array2< T, Allocator >::size_type i;
			for( i = 0 ; i < img.size( ) ; i++ )
			{
				img[i].r = img[i].get_value( );
				img[i].g = img[i].get_value( );
				img[i].b = img[i].get_value( );
			}

			return( jpeg_controller< rgb< T >, Allocator >::write_jpeg( img, filename, quality ) );
		}
	};

	template < class T, class Allocator >
	struct jpeg_controller< rgb< T >, Allocator >
	{
		static bool read_jpeg( array2< rgb< T >, Allocator > &image, const std::string &filename )
		{
			FILE *fin;				// 読み書き用ファイルポインター
			fin = fopen( filename.c_str( ), "rb" );
			if( fin == NULL ) return( false );

			JDIMENSION i, j;
			JSAMPROW bitmap[1];				// ビットマップデータ配列へのポインター
			jpeg_decompress_struct dinfo;	// JPEG解凍情報構造体
			jpeg_error_mgr jerr;			// JPEGエラー処理用構造体
			int scanlen;					// ビットマップ１行のバイト数

			dinfo.err = jpeg_std_error( &jerr );
			jpeg_create_decompress( &dinfo );

			//jpeg_stdio_src(&dinfo, fin, 0);
			jpeg_stdio_src(&dinfo, fin);

			int n = jpeg_read_header( &dinfo, true );
			if (n < 1)
			{
				jpeg_destroy_decompress(&dinfo);
				return(false);
			}

			jpeg_start_decompress(&dinfo);
			scanlen = dinfo.output_width * dinfo.output_components;

			image.resize( dinfo.output_width, dinfo.output_height );

			JSAMPLE *buffer = new JSAMPLE[ scanlen ];
			for( j = 0 ; j < dinfo.output_height ; j++ )
			{
				bitmap[0] = &buffer[0];
				if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
				for( i = 0 ; i < dinfo.output_width ; i++ )
				{
					image( i, j ).r = static_cast< T >( buffer[ i * 3 ] );
					image( i, j ).g = static_cast< T >( buffer[ i * 3 + 1 ] );
					image( i, j ).b = static_cast< T >( buffer[ i * 3 + 2 ] );
				}
			}

			jpeg_finish_decompress( &dinfo );

			fclose( fin );
			delete [] buffer;

			jpeg_destroy_decompress( &dinfo );

			return( true );
		}

		static bool write_jpeg( array2< rgb< T >, Allocator > &image, const std::string &filename, int quality = 100 )
		{
			FILE *fout;						// 読み書き用ファイルポインター
			fout = fopen( filename.c_str( ), "wb" );
			if( fout == NULL ) return( false );

			JDIMENSION i, j, w = image.width( ), h = image.height( );
			JSAMPROW bitmap[1];				/* pointer to JSAMPLE row[s] */
			jpeg_compress_struct cinfo;		// JPEG解凍情報構造体
			jpeg_error_mgr jerr;			// JPEGエラー処理用構造体
			//int linelen;					// ビットマップ１行の正味バイト数
			int scanlen = w * 3;			// ビットマップ１行のバイト数

			cinfo.err = jpeg_std_error( &jerr );
			jpeg_create_compress( &cinfo );
			//jpeg_stdio_dest(&cinfo, fout, 0);
			jpeg_stdio_dest( &cinfo, fout );
			cinfo.image_width = w;
			cinfo.image_height = h;
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;
			jpeg_set_defaults( &cinfo );
			jpeg_set_quality( &cinfo, quality, true );
			jpeg_start_compress( &cinfo, true );

			JSAMPLE *buffer = new JSAMPLE[ w * h * 3 ];
			JSAMPLE *p = buffer;

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					*p++ = static_cast< JSAMPLE >( image( i, j ).r );
					*p++ = static_cast< JSAMPLE >( image( i, j ).g );
					*p++ = static_cast< JSAMPLE >( image( i, j ).b );
				}
			}

			while( cinfo.next_scanline < cinfo.image_height )
			{
				bitmap[0] = &buffer[ cinfo.next_scanline * scanlen ];
				jpeg_write_scanlines( &cinfo, bitmap, 1 );
			}

			jpeg_finish_compress( &cinfo );

			fclose( fout );
			delete [] buffer;

			jpeg_destroy_compress( &cinfo );

			return( true );
		}
	};
}


template < class T, class Allocator >
bool read_jpeg( array2< T, Allocator > &image, const std::string &filename )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::read_jpeg( image, filename ) );
}

template < class T, class Allocator >
bool write_jpeg( array2< T, Allocator > &image, const std::string &filename, int quality = 100 )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::write_jpeg( image, filename, quality ) );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_JPEG__
