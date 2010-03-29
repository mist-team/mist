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

// 以下のコードは libpng に付属のサンプルコード，および KATO 氏が http://www5.cds.ne.jp/~kato/png/ にて
// 掲載しているソースを参考に作成し，MISTで利用できるように独自の拡張を施したものである．


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

			fp = fopen( filename.c_str( ), "rb" );	// 読み込むPNG画像ファイルを開く
			if( !fp ) return( false );

			// PNGの画像を読み込む際に，必要となる各種構造体を初期化する
			png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
			info_ptr = png_create_info_struct( png_ptr );	// png_infop 構造体を初期化する
			png_init_io( png_ptr, fp );						// png_structp にファイルポインタを設定する
			png_read_info( png_ptr, info_ptr );				// PNGファイルのヘッダを読み込む
			png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);	// IHDRチャンク情報を取得する

			if( bit_depth < 8 )
			{
				png_set_packing( png_ptr );
			}

			// カラーパレットをRGBに変換する
			if( color_type == PNG_COLOR_TYPE_PALETTE )
			{
				png_set_palette_to_rgb( png_ptr );
			}

			// 8ビット未満のグレースケールを8ビットに変換する
			if( color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8 )
			{
				png_set_expand_gray_1_2_4_to_8( png_ptr );
			}

			// アルファチャンネルを有効化する
			if( png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS ) )
			{
				png_set_tRNS_to_alpha( png_ptr );
			}

			png_read_update_info( png_ptr, info_ptr );

			// IHDRチャンク情報を再度取得する
			png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

			image.resize( width, height );

			bool ret = true;

			if( interlace_type == 0 )
			{
				// PNGの画像を操作するための一時配列を用意する
				png_bytep png_buff = ( png_bytep )malloc( png_get_rowbytes( png_ptr, info_ptr ) );

				for( size_type j = 0 ; j < ( size_type )height ; j++ )
				{
					// 画像データを読み込む
					png_read_rows( png_ptr, &png_buff, NULL, 1 );

					// 出力先のポインタ
					typename array2< T, Allocator >::pointer op = &image( 0, j );

					// PNG に格納されている画像の形式に応じてデータを読み込む
					switch( color_type )
					{
					case PNG_COLOR_TYPE_GRAY:
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							op[ i ] = pixel_converter::convert_to( png_buff[ i ], png_buff[ i ], png_buff[ i ] );
						}
						break;

					case PNG_COLOR_TYPE_RGB:
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							op[ i ] = pixel_converter::convert_to( png_buff[ i * 3 + 0 ], png_buff[ i * 3 + 1 ], png_buff[ i * 3 + 2 ] );
						}
						break;

					case PNG_COLOR_TYPE_RGBA:
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							op[ i ] = pixel_converter::convert_to( png_buff[ i * 4 + 0 ], png_buff[ i * 4 + 1 ], png_buff[ i * 4 + 2 ], png_buff[ i * 4 + 3 ] );
						}
						break;

					case PNG_COLOR_TYPE_GA:
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							op[ i ] = pixel_converter::convert_to( png_buff[ i * 2 ], png_buff[ i * 2 ], png_buff[ i * 2 ], png_buff[ i * 2 + 1 ] );
						}
						break;

					case PNG_COLOR_TYPE_PALETTE:
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							png_color &p = info_ptr->palette[ png_buff[ i ] ];
							op[ i ] = pixel_converter::convert_to( p.red, p.green, p.blue );
						}
						break;

					default:
						ret = false;
						break;
					}
				}

				// 一時画像用に確保したメモリを開放する
				free( png_buff );
			}
			else
			{
				// PNGの画像を操作するための一時配列を用意する
				png_bytepp png_buff;
				png_buff = ( png_bytepp )malloc( height * sizeof( png_bytep ) );
				for( size_type i = 0 ; i < ( size_type )height ; i++ )
				{
					png_uint_32 nbytes = png_get_rowbytes( png_ptr, info_ptr );
					png_buff[ i ] = ( png_bytep )malloc( nbytes );
				}

				// 画像データを読み込む
				png_read_image( png_ptr, png_buff );

				// PNG に格納されている画像の形式に応じてデータを読み込む
				switch( color_type )
				{
				case PNG_COLOR_TYPE_GRAY:
					for( size_type j = 0 ; j < ( size_type )height ; j++ )
					{
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							image( i, j ) = pixel_converter::convert_to( png_buff[ j ][ i ], png_buff[ j ][ i ], png_buff[ j ][ i ] );
						}
					}
					break;

				case PNG_COLOR_TYPE_RGB:
					for( size_type j = 0 ; j < ( size_type )height ; j++ )
					{
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							image( i, j ) = pixel_converter::convert_to( png_buff[ j ][ i * 3 + 0 ], png_buff[ j ][ i * 3 + 1 ], png_buff[ j ][ i * 3 + 2 ] );
						}
					}
					break;

				case PNG_COLOR_TYPE_RGBA:
					for( size_type j = 0 ; j < ( size_type )height ; j++ )
					{
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							image( i, j ) = pixel_converter::convert_to( png_buff[ j ][ i * 4 + 0 ], png_buff[ j ][ i * 4 + 1 ], png_buff[ j ][ i * 4 + 2 ], png_buff[ j ][ i * 4 + 3 ] );
						}
					}
					break;

				case PNG_COLOR_TYPE_GA:
					for( size_type j = 0 ; j < ( size_type )height ; j++ )
					{
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							image( i, j ) = pixel_converter::convert_to( png_buff[ j ][ i * 2 ], png_buff[ j ][ i * 2 ], png_buff[ j ][ i * 2 ], png_buff[ j ][ i * 2 + 1 ] );
						}
					}
					break;

				case PNG_COLOR_TYPE_PALETTE:
					for( size_type j = 0 ; j < ( size_type )height ; j++ )
					{
						for( size_type i = 0 ; i < ( size_type )width ; i++ )
						{
							png_color &p = info_ptr->palette[ png_buff[ j ][ i ] ];
							image( i, j ) = pixel_converter::convert_to( p.red, p.green, p.blue );
						}
					}
					break;

				default:
					ret = false;
					break;
				}

				// 一時画像用に確保したメモリを開放する
				for( size_type i = 0 ; i < ( size_type )height ; i++ )
				{
					free( png_buff[ i ] );
				}
				free( png_buff );
			}

			// PNG の操作用に使用した構造体のメモリを解放する
			png_destroy_read_struct( &png_ptr, &info_ptr, NULL );

			// PNG のファイルを閉じる
			fclose( fp );

			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, int compression_level__ )
		{
			int compression_level = compression_level__ > 9 ? 9 : compression_level__;

			if( image.width( ) == 0 )
			{
				std::cerr << "Image width is zero!" << std::endl;
				return( false );
			}
			else if( image.height( ) == 0 )
			{
				std::cerr << "Image height is zero!" << std::endl;
				return( false );
			}
			
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
				time_t		gmt;
				png_time	mod_time;
				png_text	text_ptr[5];

				time( &gmt );
				png_convert_from_time_t( &mod_time, gmt );
				png_set_tIME( png_ptr, info_ptr, &mod_time );

				static char text[][64] = {
					"Title",
					"MIST PNG Library",
					"Author",
					"",
					"Description",
					"Created by using MIST library",
					"Creation Time",
					"Software",
					"MIST PNG converter",
				};

				text_ptr[ 0 ].key  = text[ 0 ];
				text_ptr[ 0 ].text = text[ 1 ];
				text_ptr[ 0 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 1 ].key  = text[ 2 ];
				text_ptr[ 1 ].text = text[ 3 ];
				text_ptr[ 1 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 2 ].key  = text[ 4 ];
				text_ptr[ 2 ].text = text[ 5 ];
				text_ptr[ 2 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 3 ].key  = text[ 6 ];
				text_ptr[ 3 ].text = png_convert_to_rfc1123( png_ptr, &mod_time );
				text_ptr[ 3 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 4 ].key  = text[ 7 ];
				text_ptr[ 4 ].text = text[ 8 ];
				text_ptr[ 4 ].compression = PNG_TEXT_COMPRESSION_NONE;
				png_set_text( png_ptr, info_ptr, text_ptr, 5 );
			}

			png_write_info( png_ptr, info_ptr );

			if( pixel_converter::color_num == 4 )
			{
				png_bytep png_buff = new png_byte[ width * 4 ];
				for( size_type j = 0 ; j < height ; j++ )
				{
					typename array2< T, Allocator >::const_pointer ip = &image( 0, j );
					for( size_type i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( ip[ i ] ) );
						png_buff[ i * 4 + 0 ] = static_cast< png_byte >( c.r );
						png_buff[ i * 4 + 1 ] = static_cast< png_byte >( c.g );
						png_buff[ i * 4 + 2 ] = static_cast< png_byte >( c.b );
						png_buff[ i * 4 + 3 ] = static_cast< png_byte >( c.a );
					}

					png_write_rows( png_ptr, &png_buff, 1 );
				}

				delete [] png_buff;
			}
			else
			{
				png_bytep png_buff = new png_byte[ width * 4 ];
				for( size_type j = 0 ; j < height ; j++ )
				{
					typename array2< T, Allocator >::const_pointer ip = &image( 0, j );
					for( size_type i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( ip[ i ] ) );
						png_buff[ i * 3 + 0 ] = static_cast< png_byte >( c.r );
						png_buff[ i * 3 + 1 ] = static_cast< png_byte >( c.g );
						png_buff[ i * 3 + 2 ] = static_cast< png_byte >( c.b );
					}

					png_write_rows( png_ptr, &png_buff, 1 );
				}

				delete [] png_buff;
			}

			png_write_end( png_ptr, info_ptr );
			png_destroy_write_struct( &png_ptr, &info_ptr );
			fclose( fp );

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
bool read_png( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_png( image, wstr2str( filename ) ) );
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
bool write_png( const array2< T, Allocator > &image, const std::string &filename, int compression_level = 6 )
{
	return( __png_controller__::png_controller< T, Allocator >::write( image, filename, compression_level ) );
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
bool write_png( const array2< T, Allocator > &image, const std::wstring &filename, int compression_level = 6 )
{
	return( write_png( image, wstr2str( filename ), compression_level ) );
}


/// @}
//  PNG 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_PNG__
