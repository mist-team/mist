/// @file mist/io/jpeg.h
//!
//! @brief JPEG画像を読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_JPEG__
#define __INCLUDE_MIST_JPEG__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#include <iostream>
#include <string>


#ifdef WIN32

	#define XMD_H
	#define HAVE_INT32			// JPEG用INT32型を持っている宣言
	#define HAVE_BOOLEAN		// JPEG用boolean型を持っている宣言

#endif

extern "C"
{
#include <jpeglib.h>
}


// mist名前空間の始まり
_MIST_BEGIN


namespace __jpeg_controller__
{
	template < class T, class Allocator >
	struct jpeg_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
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
			jpeg_stdio_src( &dinfo, fin );

			int n = jpeg_read_header( &dinfo, true );
			if( n < 1 )
			{
				jpeg_destroy_decompress( &dinfo );
				return(false);
			}

			jpeg_start_decompress( &dinfo );
			scanlen = dinfo.output_width * dinfo.output_components;

			image.resize( dinfo.output_width, dinfo.output_height );

			JSAMPLE *buffer = new JSAMPLE[ scanlen ];
			for( j = 0 ; j < dinfo.output_height ; j++ )
			{
				bitmap[0] = &buffer[0];
				if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
				for( i = 0 ; i < dinfo.output_width ; i++ )
				{
					switch( dinfo.output_components )
					{
					case 1:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i ], buffer[ i ], buffer[ i ] );
						break;

					case 3:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i * 3 + 0 ], buffer[ i * 3 + 1 ], buffer[ i * 3 + 2 ] );
						break;
						
					default:
						break;
					}
				}
			}

			jpeg_finish_decompress( &dinfo );

			fclose( fin );
			delete [] buffer;

			jpeg_destroy_decompress( &dinfo );

			return( true );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, int quality )
		{
			FILE *fout;						// 読み書き用ファイルポインター
			fout = fopen( filename.c_str( ), "wb" );
			if( fout == NULL ) return( false );

			JDIMENSION i, j;
			JDIMENSION w = static_cast< JDIMENSION >( image.width( ) ), h = static_cast< JDIMENSION >( image.height( ) );
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
					color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
					*p++ = static_cast< JSAMPLE >( c.r );
					*p++ = static_cast< JSAMPLE >( c.g );
					*p++ = static_cast< JSAMPLE >( c.b );
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


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_jpeg_group JPEG 画像入出力
//!  @{


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[out] image    … 引数の説明
//! @param[in]  filename … 引数の説明
//!
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template < class T, class Allocator >
bool read_jpeg( array2< T, Allocator > &image, const std::string &filename )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::read( image, filename ) );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] image    … 引数の説明
//! @param[in] filename … 引数の説明
//! @param[in] quality  … 引数の説明
//!
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template < class T, class Allocator >
bool write_jpeg( const array2< T, Allocator > &image, const std::string &filename, int quality = 100 )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::write( image, filename, quality ) );
}


/// @}
//  JPEG 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_JPEG__
