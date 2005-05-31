/// @file mist/io/png.h
//!
//! @brief PNG画像を読み書きするためのライブラリ
//!
//! 本ライブラリは，http://www.libpng.org/pub/png/ , http://www.libpng.org/ で開発が行われている libpng を利用している
//! エンコードとデコードは本ライブラリのAPIを用いて実装されているため，別途ライブラリを用意する必要がある
//!
#ifndef __INCLUDE_MIST_PNG__
#define __INCLUDE_MIST_PNG__


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


#include <string>

#include <png.h>


// mist名前空間の始まり
_MIST_BEGIN


namespace __png_controller__
{
	template < class T, class Allocator >
	struct png_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			FILE			*fp;
			png_structp		png_ptr;
			png_infop		info_ptr;
			unsigned long	width, height;
			int				bit_depth, color_type, interlace_type;
			size_type		i, j;

			fp = fopen( filename.c_str( ), "rb" );                           // まずファイルを開きます
			if( !fp ) return( false );

			// png_ptr構造体を確保・初期化します
			png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
			info_ptr = png_create_info_struct( png_ptr );             // info_ptr構造体を確保・初期化します
			png_init_io( png_ptr, fp );                               // libpngにfpを知らせます
			png_read_info( png_ptr, info_ptr );                       // PNGファイルのヘッダを読み込みます
			png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);         // IHDRチャンク情報を取得します

			png_bytepp png_buff;
			png_buff = ( png_bytepp )malloc( height * sizeof( png_bytep ) ); // 以下３行は２次元配列を確保します
			for( i = 0 ; i < (size_type)height ; i++ ) png_buff[i] = ( png_bytep )malloc( png_get_rowbytes( png_ptr, info_ptr ) );

			image.resize( width, height );

			png_read_image( png_ptr, png_buff );	// 画像データを読み込みます

			bool ret = true;

			switch( color_type )
			{
			case PNG_COLOR_TYPE_GRAY:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][i], png_buff[j][i], png_buff[j][i] );
					}
				}
				break;

			case PNG_COLOR_TYPE_RGB:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][ i * 3 + 0 ], png_buff[j][ i * 3 + 1 ], png_buff[j][ i * 3 + 2 ] );
					}
				}
				break;

			case PNG_COLOR_TYPE_RGBA:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][ i * 4 + 0 ], png_buff[j][ i * 4 + 1 ], png_buff[j][ i * 4 + 2 ], png_buff[j][ i * 4 + 3 ] );
					}
				}
				break;

			case PNG_COLOR_TYPE_GA:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][ i * 2 ], png_buff[j][ i * 2 ], png_buff[j][ i * 2 ], png_buff[j][ i * 2 + 1 ] );
					}
				}
				break;

			case PNG_COLOR_TYPE_PALETTE:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						png_color &p = info_ptr->palette[ png_buff[j][ i ] ];
						image( i, j ) = pixel_converter::convert_to( p.red, p.green, p.blue );
					}
				}
				break;

			default:
				ret = false;
				break;
			}

			for( i = 0 ; i < (size_type)height ; i++ ) free( png_buff[i] );            // 以下２行は２次元配列を解放します
			free( png_buff );

			// ２つの構造体のメモリを解放します
			png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
			fclose( fp );

			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, int compression_level )
		{
			FILE *fp;
			png_structp	png_ptr;
			png_infop	info_ptr;
			size_type	width  = image.width( );
			size_type	height = image.height( );

			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL ) return(false);

			png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
			if( png_ptr == NULL )
			{
				fclose( fp );
				return( false );
			}

			info_ptr = png_create_info_struct( png_ptr );
			if( info_ptr == NULL )
			{
				png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
				fclose( fp );
				return( false );
			}
			if( setjmp( png_ptr->jmpbuf ) )
			{
				png_destroy_write_struct( &png_ptr, &info_ptr );
				fclose( fp );
				return( false );
			}

			if( compression_level > 9 ) compression_level = 9;

			png_init_io( png_ptr, fp );
			//png_set_write_status_fn(png_ptr, write_row_callback);
			png_set_filter( png_ptr, 0, PNG_ALL_FILTERS );
			if( compression_level < 0 )
			{
				png_set_compression_level( png_ptr, Z_BEST_COMPRESSION );
			}
			else
			{
				png_set_compression_level( png_ptr, compression_level );
			}

			int png_color_type = pixel_converter::color_num == 4 ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB;
			png_set_IHDR( png_ptr, info_ptr, static_cast< png_uint_32 >( width ), static_cast< png_uint_32 >( height ), 8, png_color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
			png_set_gAMA( png_ptr, info_ptr, 1.0 );

			{
				time_t		gmt;		// G.M.T.
				png_time	mod_time;
				png_text	text_ptr[5];

				time( &gmt );
				png_convert_from_time_t( &mod_time, gmt );
				png_set_tIME( png_ptr, info_ptr, &mod_time );

				text_ptr[ 0 ].key = "Title";
				text_ptr[ 0 ].text = "slipapng library";
				text_ptr[ 0 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 1 ].key = "Author";
				text_ptr[ 1 ].text = "mist project team";
				text_ptr[ 1 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 2 ].key = "Description";
				text_ptr[ 2 ].text = "created by mist project team";
				text_ptr[ 2 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 3 ].key = "Creation Time";
				text_ptr[ 3 ].text = png_convert_to_rfc1123( png_ptr, &mod_time );
				text_ptr[ 3 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 4 ].key = "Software";
				text_ptr[ 4 ].text = "mist png converter";
				text_ptr[ 4 ].compression = PNG_TEXT_COMPRESSION_NONE;
				png_set_text( png_ptr, info_ptr, text_ptr, 5 );
			}

			png_bytepp png_buff;
			png_buff = new png_byte*[height];

			size_type i, j;
			if( pixel_converter::color_num == 4 )
			{
				for( j = 0 ; j < height ; j++ )
				{
					png_buff[ j ] = new png_byte[width * 4];
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						png_buff[ j ][ i * 4 + 0 ] = static_cast< unsigned char >( c.r );
						png_buff[ j ][ i * 4 + 1 ] = static_cast< unsigned char >( c.g );
						png_buff[ j ][ i * 4 + 2 ] = static_cast< unsigned char >( c.b );
						png_buff[ j ][ i * 4 + 3 ] = static_cast< unsigned char >( c.a );
					}
				}
			}
			else
			{
				for( j = 0 ; j < height ; j++ )
				{
					png_buff[j] = new png_byte[width * 3];
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						png_buff[ j ][ i * 3 + 0 ] = static_cast< unsigned char >( c.r );
						png_buff[ j ][ i * 3 + 1 ] = static_cast< unsigned char >( c.g );
						png_buff[ j ][ i * 3 + 2 ] = static_cast< unsigned char >( c.b );
					}
				}
			}

			png_write_info( png_ptr, info_ptr );
			png_write_image( png_ptr, png_buff );
			png_write_end( png_ptr, info_ptr );
			png_destroy_write_struct( &png_ptr, &info_ptr );
			fclose( fp );

			for( j = 0 ; j < height ; j++ ) delete [] png_buff[j];
			delete [] png_buff;

			return( true );
		}
	};
}


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_png_group PNG 画像入出力
//!
//! PNGファイルを読み込んで，MISTコンテナへ格納する．
//! また，MISTコンテナからPNGファイルへの書き出しをサポート．
//! 別途 http://libpng.org が提供するフリーのPNGライブラリが必要です．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/png.h>
//! @endcode
//!
//!  @{


/// @brief PNG画像をMISTコンテナに読み込む
//! 
//! RGBやRGBAやカラーテーブルを使用した全てのPNG画像を読み込むことが可能です
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_png( array2< T, Allocator > &image, const std::string &filename )
{
	return( __png_controller__::png_controller< T, Allocator >::read( image, filename ) );
}


/// @brief MISTコンテナの画像をPNG形式でファイルに出力する
//! 
//! @attention 圧縮レベルは0～9まであり，引数に負の値を指定すると，適切な圧縮レベルが自動的に選択される（デフォルト）
//! 
//! @param[in] image    … 出力画像を保持するMISTコンテナ
//! @param[in] filename … 出力ファイル名
//! @param[in] compression_level … PNG画像を出力する際の圧縮レベル
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_png( const array2< T, Allocator > &image, const std::string &filename, int compression_level = -1 )
{
	return( __png_controller__::png_controller< T, Allocator >::write( image, filename, compression_level ) );
}


/// @}
//  PNG 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_PNG__
