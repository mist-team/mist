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

/// @file mist/io/tga.h
//!
//! @brief ビットマップ画像を読み書きするためのライブラリ
//!

#ifndef __INCLUDE_MIST_TGA__
#define __INCLUDE_MIST_TGA__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "../config/endian.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#include <iostream>
#include <string>

#include <deque>
#include <map>
#include <algorithm>


// mist名前空間の始まり
_MIST_BEGIN


namespace __tga_controller__
{
	// 構造体内のアライメントを1バイトに設定し，パディングを禁止する
#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( push, tga_align, 1 )
#endif
		struct _tga_header_
		{
			enum{ bytes = 18 };

			// ID Length Field 1 ( 1 byte )
			unsigned char	id_length;

			// Color Map Type Field 2 ( 1 byte )
			unsigned char	color_map_type;

			// Image Type Field 3 ( 1 byte )
			unsigned char	image_type;

			// Color Map Specification Field 4 ( 5 bytes )
			unsigned short	first_entry_index;
			unsigned short	color_map_length;
			unsigned char	color_map_entry_size;

			// Image Specification Field 4 ( 10 bytes )
			unsigned short	x_origin;
			unsigned short	y_origin;
			unsigned short	image_width;
			unsigned short	image_height;
			unsigned char	pixel_depth;
			unsigned char	image_descriptor;

		} _MIST_PACKED;

#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( pop, tga_align )
#endif
	// 構造体内のアライメントを1バイトに設定し，パディングを禁止する ～ ここまで ～


	template < class T, class Allocator >
	struct tga_controller
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		typedef typename array2< T, Allocator >::difference_type difference_type;
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool is_supported( size_type tga_bits )
		{
			bool ret = false;
			switch( tga_bits )
			{
			case 16:
			case 24:
			case 32:
				ret = true;
				break;

			default:
				break;
			}
			return( ret );
		}

		static bool decode_RLE( const unsigned char *pixel, unsigned char * &buff, size_type snum_bytes, size_type num_bytes, size_type pixel_bytes )
		{
			buff = new unsigned char[ num_bytes ];
			memset( buff, 0, sizeof( unsigned char ) * num_bytes );

			size_type i = 0, j = 0;
			for( ; i < snum_bytes && j < num_bytes ; )
			{
				unsigned char byte = pixel[ i++ ];
				if( ( byte & 0x80 ) != 0 )
				{
					// 上位ビットが１のとき反復。次に続くデータバイト（ピクセルのバイト数を単位として）を繰り返す。
					// 繰り返す回数は、byte & 0x7F + 1
					// 例： 83 0A -> 0A 0A 0A 0A

					size_type num = ( byte & 0x7f ) + 1;

					// 範囲外アクセスのチェック
					if( i + pixel_bytes > snum_bytes || j + num * pixel_bytes > num_bytes )
					{
						break;
					}

					for( size_type l = 0 ; l < num ; l++ )
					{
						for( size_type m = 0 ; m < pixel_bytes ; m++ )
						{
							buff[ j + l * pixel_bytes + m ] = pixel[ i + m ];
						}
					}

					i += pixel_bytes;
					j += num * pixel_bytes;
				}
				else
				{
					// 上位ビットが0のとき、リテラルグループ(Literal group)が続く。制御バイトの後ろ
					// byte+1 個のデータ（ピクセルのバイト数を単位として）をコピーする。
					// 例： 04 0A 0B 0C 0D 0E ->  0A 0B 0C 0D 0E
					size_type num = byte + 1;

					// 範囲外アクセスのチェック
					if( i + num * pixel_bytes > snum_bytes || j + num * pixel_bytes > num_bytes )
					{
						break;
					}

					memcpy( buff + j, pixel + i, sizeof( unsigned char ) * num * pixel_bytes );

					i += num * pixel_bytes;
					j += num * pixel_bytes;
				}
			}

			if( j != num_bytes )
			{
				// RLEのデコードに失敗
				delete [] buff;
				buff = NULL;
				return( false );
			}

			return( true );
		}

		inline static bool is_equal( const unsigned char *pix1, const unsigned char *pix2, size_type pixel_bytes )
		{
			for( size_type i = 0 ; i < pixel_bytes ; i++ )
			{
				if( pix1[ i ] != pix2[ i ] )
				{
					return( false );
				}
			}
			return( true );
		}

		inline static size_type count_run_length( const unsigned char *pixel, size_type num_bytes, size_type pixel_bytes )
		{
			if( num_bytes < 2 )
			{
				return( 0 );
			}

			size_type i, l;
			for( i = 0 ; i < num_bytes ; i++ )
			{
				for( l = 0 ; l < pixel_bytes ; l++ )
				{
					if( pixel[ l ] != pixel[ i * pixel_bytes + l ] )
					{
						break;
					}
				}

				if( l != pixel_bytes )
				{
					return( i );
				}
			}
			return( i );
		}

		static size_type encode_RLE( unsigned char *pixel, size_type num_bytes, size_type pixel_bytes )
		{
			unsigned char *buff = new unsigned char[ num_bytes * 2 ];
			unsigned char temp[ 20 ];

			// ランレングスのリストを作成する
			size_type num = num_bytes / pixel_bytes, count = 0, k;
			unsigned char *pbuff = buff;
			const unsigned char *pix = pixel;
			const unsigned char *epix = pixel + num_bytes;
			for( size_type i = 0 ; i < num ; i++ )
			{
				size_type len = count_run_length( pix, epix - pix, pixel_bytes );

				if( len > 2 )
				{
					size_type nrun = len / 128;
					size_type rest = len % 128;

					temp[ 0 ] = 0xff;

					for( k = 0 ; k < pixel_bytes ; k++ )
					{
						temp[ k + 1 ] = pix[ k ];
					}

					for( k = 0 ; k < nrun ; k++ )
					{
						for( size_type l = 0 ; l <= pixel_bytes ; l++ )
						{
							pbuff[ l ] = temp[ l ];
						}
						pbuff += pixel_bytes + 1;
					}

					pix += 128 * nrun * pixel_bytes;

					if( rest > 2 )
					{
						temp[ 0 ] = 0x80 + static_cast< unsigned char >( rest - 1 );
						for( size_type l = 0 ; l <= pixel_bytes ; l++ )
						{
							pbuff[ l ] = temp[ l ];
						}
						pbuff += pixel_bytes + 1;
						pix   += pixel_bytes * rest;

						count = 0;
					}
					else if( rest > 0 )
					{
						pbuff[ 0 ] = static_cast< unsigned char >( rest - 1 );

						for( k = 0 ; k < pixel_bytes * rest ; k++ )
						{
							pbuff[ k + 1 ] = pix[ k ];
						}
						pbuff += pixel_bytes * rest + 1;
						pix   += pixel_bytes * rest;

						count = rest;
					}
				}
				else if( len > 0 )
				{
					if( count > 0 )
					{
						size_type total = count + len;
						size_type nnum, rest;

						if( total > 128 )
						{
							rest  = total % 128;
							nnum  = total - rest;
						}
						else
						{
							nnum = total;
							rest = 0;
						}

						pbuff[ 0 ] = static_cast< unsigned char >( nnum - 1 );

						for( k = 0 ; k < pixel_bytes * nnum ; k++ )
						{
							pbuff[ k + 1 ] = pix[ k ];
						}
						pbuff += pixel_bytes * nnum + 1;
						pix   += pixel_bytes * nnum;

						if( rest > 0 )
						{
							pbuff[ 0 ] = static_cast< unsigned char >( rest - 1 );

							for( k = 0 ; k < pixel_bytes * rest ; k++ )
							{
								pbuff[ k + 1 ] = pix[ k ];
							}
							pbuff += pixel_bytes * rest + 1;
							pix   += pixel_bytes * rest;
						}

						count = rest;
					}
					else
					{
						pbuff[ 0 ] = static_cast< unsigned char >( len - 1 );

						for( k = 0 ; k < pixel_bytes * len ; k++ )
						{
							pbuff[ k + 1 ] = pix[ k ];
						}
						pbuff += pixel_bytes * len + 1;
						pix   += pixel_bytes * len;

						count = len;
					}
				}
			}

			if( num_bytes <= static_cast< size_type >( pbuff - buff ) )
			{
				// RLEのエンコードに失敗
				delete [] buff;
				return( num_bytes );
			}
			else
			{
				num_bytes = pbuff - buff;
				memcpy( pixel, buff, num_bytes );
				delete [] buff;
				return( num_bytes );
			}
		}


		static bool convert_from_tga_data( unsigned char *tga, size_type num_bytes, array2< T, Allocator > &image )
		{
			// TGA用のヘッダの位置を指定する
			_tga_header_	*pheader		= reinterpret_cast < _tga_header_ * >( tga );
			_tga_header_	&header			= *pheader;

			difference_type width  = header.image_width;
			difference_type height = header.image_height;
			difference_type pixel_bytes = header.pixel_depth / 8;

			unsigned char	*color_map_data	= tga + _tga_header_::bytes;
			unsigned char	*image_data		= color_map_data + ( header.color_map_type == 0 ? 0 : header.color_map_length * header.color_map_entry_size / 8 );

//			unsigned char	*pfooter		= image_data + width * height * pixel_bytes;

			bool is_RLE		= ( header.image_type & 0x08 ) != 0;
			bool is_Huffman	= ( header.image_type & 0x20 ) != 0;
			bool from_top	= ( header.image_descriptor & 0x20 ) != 0;
			bool from_left	= ( header.image_descriptor & 0x10 ) == 0;

			if( is_Huffman || header.image_type > 11 )
			{
				// 未サポート
				return( false );
			}

			unsigned char *buff = NULL, *pixels = NULL;

			if( is_RLE )
			{
				if( !decode_RLE( image_data, buff, tga + num_bytes - image_data, width * height * pixel_bytes, pixel_bytes ) )
				{
					return( false );
				}
				pixels = buff + ( from_top ? 0 : width * ( height - 1 ) * pixel_bytes );
			}
			else
			{
				pixels = image_data + ( from_top ? 0 : width * ( height - 1 ) * pixel_bytes );
			}

			difference_type line_skip  = from_top  ? width * pixel_bytes : - width * pixel_bytes;
			difference_type pixel_skip = from_left ? pixel_bytes : - pixel_bytes;

			image.resize( width, height );

			bool ret = true;

			difference_type i, j, k;
			switch( header.image_type & 0x07 )
			{
			case 1:	// Color-mapped Image
				for( j = 0 ; j < height ; j++ )
				{
					unsigned char *pixel = pixels + line_skip * j + ( from_left ? 0 : pixel_bytes ) * ( width - 1 );

					for( i = 0 ; i < width ; i++ )
					{
						difference_type tmp = 0;
						unsigned char *pix = pixel + pixel_skip * i;

						for( k = pixel_bytes - 1 ; k >= 0 ; k-- )
						{
							tmp = ( tmp << 8 ) + pix[ k ];
						}

						difference_type index = tmp - header.first_entry_index;

						switch( header.color_map_entry_size )
						{
						case 16:
							{
								unsigned short v = reinterpret_cast< unsigned short * >( color_map_data + index * 2 )[ 0 ];
								unsigned char a = ( v & 0x8000 ) >> 15;
								unsigned char r = ( ( v & 0x7c00 ) >> 10 ) * 8;
								unsigned char g = ( ( v & 0x03e0 ) >> 5 ) * 8;
								unsigned char b = ( v & 0x001f ) * 8;
								image( i, j ) = pixel_converter::convert_to( r, g, b, static_cast< unsigned char >( a * 255 ) );
							}
							break;

						case 24:
							{
								unsigned char r = color_map_data[ index * 3 + 2 ];
								unsigned char g = color_map_data[ index * 3 + 1 ];
								unsigned char b = color_map_data[ index * 3 + 0 ];
								image( i, j ) = pixel_converter::convert_to( r, g, b );
							}
							break;

						case 32:
							{
								unsigned char a = color_map_data[ index * 4 + 3 ];
								unsigned char r = color_map_data[ index * 4 + 2 ];
								unsigned char g = color_map_data[ index * 4 + 1 ];
								unsigned char b = color_map_data[ index * 4 + 0 ];
								image( i, j ) = pixel_converter::convert_to( r, g, b, a );
							}
							break;

						default:
							break;
						}
					}
				}
				break;

			case 2:	// True-color Image
				for( j = 0 ; j < height ; j++ )
				{
					unsigned char *pixel = pixels + line_skip * j + ( from_left ? 0 : pixel_bytes ) * ( width - 1 );

					switch( header.pixel_depth )
					{
					case 16:
						for( i = 0 ; i < width ; i++ )
						{
							unsigned short pix = reinterpret_cast< unsigned short * >( pixel + pixel_skip * i )[ 0 ];
							unsigned char a = ( pix & 0x8000 ) >> 15;
							unsigned char r = ( ( pix & 0x7c00 ) >> 10 ) * 8;
							unsigned char g = ( ( pix & 0x03e0 ) >> 5 ) * 8;
							unsigned char b = ( pix & 0x001f ) * 8;
							image( i, j ) = pixel_converter::convert_to( r, g, b, static_cast< unsigned char >( a * 255 ) );
						}
						break;

					case 24:
						for( i = 0 ; i < width ; i++ )
						{
							unsigned char *pix = pixel + pixel_skip * i;
							image( i, j ) = pixel_converter::convert_to( pix[ 2 ], pix[ 1 ], pix[ 0 ] );
						}
						break;

					case 32:
						for( i = 0 ; i < width ; i++ )
						{
							unsigned char *pix = pixel + pixel_skip * i;
							image( i, j ) = pixel_converter::convert_to( pix[ 2 ], pix[ 1 ], pix[ 0 ], pix[ 3 ] );
						}
						break;

					default:
						break;
					}
				}
				break;

			case 3:	// Black and White Image
				if( header.pixel_depth != 8 )
				{
					ret = false;
				}
				else
				{
					for( j = 0 ; j < height ; j++ )
					{
						unsigned char *pixel = pixels + line_skip * j + ( from_left ? 0 : pixel_bytes ) * ( width - 1 );

						for( i = 0 ; i < width ; i++ )
						{
							unsigned char *pix = pixel + pixel_skip * i;
							image( i, j ) = pixel_converter::convert_to( pix[ 0 ], pix[ 0 ], pix[ 0 ] );
						}
					}
				}
				break;
			}

			delete [] buff;

			return( ret );
		}

		static difference_type convert_to_tga_data( const array2< T, Allocator > &image, unsigned char * &tga, size_type tga_bits, bool is_encode_RLE, bool from_top, bool from_left )
		{
			if( image.empty( ) )
			{
				return( -1 );
			}

			difference_type pixel_bytes = tga_bits / 8;
			difference_type width  = image.width( );
			difference_type height = image.height( );

			tga = new unsigned char[ _tga_header_::bytes + width * height * pixel_bytes ];

			// TGA用のヘッダの位置を指定する
			_tga_header_	*pheader		= reinterpret_cast < _tga_header_ * >( tga );
			_tga_header_	&header			= *pheader;

			// ID Length Field 1 ( 1 byte )
			header.id_length = 0;

			// Color Map Type Field 2 ( 1 byte )
			header.color_map_type = 0;

			// Image Type Field 3 ( 1 byte )
			header.image_type = 2 + ( is_encode_RLE ? 8 : 0 );

			// Color Map Specification Field 4 ( 5 bytes )
			header.first_entry_index = 0;
			header.color_map_length = 0;
			header.color_map_entry_size = 0;

			// Image Specification Field 4 ( 10 bytes )
			header.x_origin = 0;
			header.y_origin = 0;
			header.image_width = static_cast< unsigned short >( width );
			header.image_height = static_cast< unsigned short >( height );
			header.pixel_depth = static_cast< unsigned char >( tga_bits );
			header.image_descriptor = ( is_encode_RLE ? 0x08 : 0 ) | ( from_top ? 0x20 : 0 ) | ( from_left ? 0 : 0x10 );

			unsigned char *image_data		= tga + _tga_header_::bytes;

			unsigned char *pixels = image_data + ( from_top ? 0 : width * ( height - 1 ) * pixel_bytes );

			difference_type line_skip  = from_top  ? width * pixel_bytes : - width * pixel_bytes;
			difference_type pixel_skip = from_left ? pixel_bytes : - pixel_bytes;

			difference_type i, j;
			for( j = 0 ; j < height ; j++ )
			{
				unsigned char *pixel = pixels + line_skip * j + ( from_left ? 0 : pixel_bytes ) * ( width - 1 );

				switch( header.pixel_depth )
				{
				case 16:
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						unsigned short &pix = reinterpret_cast< unsigned short * >( pixel + pixel_skip * i )[ 0 ];
						unsigned short a = ( ( c.a == 0 ? 0 : 1 ) << 15 ) & 0x8000;
						unsigned short r = ( static_cast< unsigned char >( c.r / 8 ) << 10 ) & 0x7c00;
						unsigned short g = (  static_cast< unsigned char >( c.g / 8 ) << 5 ) & 0x03e0;
						unsigned short b =  static_cast< unsigned char >( c.b / 8 ) & 0x001f;
						pix = a | r | g | b;
					}
					break;

				case 24:
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						unsigned char *pix = pixel + pixel_skip * i;
						pix[ 0 ] = static_cast< unsigned char >( c.b );
						pix[ 1 ] = static_cast< unsigned char >( c.g );
						pix[ 2 ] = static_cast< unsigned char >( c.r );
					}
					break;

				case 32:
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						unsigned char *pix = pixel + pixel_skip * i;
						pix[ 0 ] = static_cast< unsigned char >( c.b );
						pix[ 1 ] = static_cast< unsigned char >( c.g );
						pix[ 2 ] = static_cast< unsigned char >( c.r );
						pix[ 3 ] = static_cast< unsigned char >( c.a );
					}
					break;

				default:
					break;
				}
			}

			if( is_encode_RLE )
			{
				// RLEエンコードを行う
				// 失敗したら，無圧縮に変更する
				difference_type nbytes = encode_RLE( image_data, width * height * pixel_bytes, pixel_bytes );

				if( nbytes == width * height * pixel_bytes )
				{
					header.image_type = 2;
					header.image_descriptor = ( from_top ? 0x20 : 0 ) | ( from_left ? 0 : 0x10 );
				}

				return( _tga_header_::bytes + nbytes );
			}
			else
			{
				return( _tga_header_::bytes + width * height * pixel_bytes );
			}
		}

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			size_type filesize;
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );

			// ファイルサイズを取得
			fseek( fp, 0, SEEK_END );
			filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			unsigned char *buff = new unsigned char[ filesize + 1 ];
			unsigned char *pointer = buff;
			size_type read_size = 0;
			while( feof( fp ) == 0 )
			{
				read_size = fread( pointer, sizeof( unsigned char ), 1024, fp );
				if( read_size < 1024 )
				{
					break;
				}
				pointer += read_size;
			}
			fclose( fp );

			bool ret = convert_from_tga_data( buff, filesize, image );
			delete [] buff;
			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, size_type tga_bits, bool is_encode_RLE )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

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
			else if( !is_supported( tga_bits ) )
			{
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			unsigned char *buff = NULL;
			difference_type size = convert_to_tga_data( image, buff, tga_bits, is_encode_RLE, true, true );

			if( size <= 0 )
			{
				delete [] buff;
				return( false );
			}

			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				delete [] buff;
				return( false );
			}


			// ファイルへ書き出し
			unsigned char *pointer = buff;
			size_type write_size = 0, writed_size = 0;
			while( size > 0 )
			{
				write_size = size < 1024 ? size : 1024;

				writed_size = fwrite( pointer, sizeof( unsigned char ), write_size, fp );
				pointer += writed_size;
				size -= writed_size;
				if( write_size != writed_size )
				{
					fclose( fp );
					delete [] buff;
					return( false );
				}
			}
			fclose( fp );

			delete [] buff;
			return( true );
		}
	};
}

//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_tga_group TGA 画像入出力
//!
//! TGAファイルを読み込んで，MISTコンテナへ格納する．
//! また，MISTコンテナからTGAファイルへの書き出しをサポート．
//! 16，24，32ビットのビットマップに対応．
//! RLE圧縮されたTGAファイルの入出力をサポート．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/tga.h>
//! @endcode
//!
//!  @{


/// @brief TGA画像をMISTコンテナに読み込む
//! 
//! @attention RLE圧縮のかかったTGAもサポート（ハフマン符号化を用いたものに対しては未対応）
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_tga( array2< T, Allocator > &image, const std::string &filename )
{
	return( __tga_controller__::tga_controller< T, Allocator >::read( image, filename ) );
}


/// @brief TGA画像をMISTコンテナに読み込む
//! 
//! @attention RLE圧縮のかかったTGAもサポート（ハフマン符号化を用いたものに対しては未対応）
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_tga( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_tga( image, wstr2str( filename ) ) );
}


/// @brief MISTコンテナの画像をTGA形式でファイルに出力する
//! 
//! @param[in] image         … 出力画像を保持するMISTコンテナ
//! @param[in] filename      … 出力ファイル名
//! @param[in] tga_bits      … 以下のビットマップのビット数のいずれか
//! @param[in] is_encode_RLE … RLE圧縮をかけるかどうか（デフォルトはTrue）
//!
//! -# 16ビット … 各色 5 ビットのビットマップ
//! -# 24ビット … フルカラービットマップ
//! -# 32ビット … フルカラービットマップ（アルファチャンネル付）
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_tga( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type tga_bits = 32, bool is_encode_RLE = true )
{
	return( __tga_controller__::tga_controller< T, Allocator >::write( image, filename, tga_bits, is_encode_RLE ) );
}


/// @brief MISTコンテナの画像をTGA形式でファイルに出力する
//! 
//! @param[in] image         … 出力画像を保持するMISTコンテナ
//! @param[in] filename      … 出力ファイル名
//! @param[in] tga_bits      … 以下のビットマップのビット数のいずれか
//! @param[in] is_encode_RLE … RLE圧縮をかけるかどうか（デフォルトはTrue）
//!
//! -# 16ビット … 各色 5 ビットのビットマップ
//! -# 24ビット … フルカラービットマップ
//! -# 32ビット … フルカラービットマップ（アルファチャンネル付）
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_tga( const array2< T, Allocator > &image, const std::wstring &filename, typename array2< T, Allocator >::size_type tga_bits = 32, bool is_encode_RLE = true )
{
	return( write_tga( image, wstr2str( filename ), tga_bits, is_encode_RLE ) );
}

/// @}
//  TGA 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TGA__
