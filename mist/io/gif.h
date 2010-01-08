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

/// @file mist/io/gif.h
//!
//! @brief GIF画像を読み込むためのライブラリ
//!
//! 本ライブラリは，GIFのデコードのみをサポートする．
//! GIFのデコード部分で，LZWのデコードを行う decode_LZW に関しては，libtiff で提供されている gif2tiff.c 内のコードを利用している．
//! その部分に関するコピーライトは以下のとおりである．
//! 
//! libtiff のコピーライト
//!
//! Copyright (c) 1988-1997 Sam Leffler
//! Copyright (c) 1991-1997 Silicon Graphics, Inc.
//! 
//! Permission to use, copy, modify, distribute, and sell this software and 
//! its documentation for any purpose is hereby granted without fee, provided
//! that (i) the above copyright notices and this permission notice appear in
//! all copies of the software and related documentation, and (ii) the names of
//! Sam Leffler and Silicon Graphics may not be used in any advertising or
//! publicity relating to the software without the specific, prior written
//! permission of Sam Leffler and Silicon Graphics.
//! 
//! THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
//! EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
//! WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
//! 
//! IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
//! ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
//! OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
//! WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
//! LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
//! OF THIS SOFTWARE.
//!

#ifndef __INCLUDE_MIST_GIF__
#define __INCLUDE_MIST_GIF__


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
#include <stack>


// mist名前空間の始まり
_MIST_BEGIN


namespace __gif_controller__
{
	// 構造体内のアライメントを1バイトに設定し，パディングを禁止する
#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( push, gif_align, 1 )
#endif
		struct _gif_header_
		{
			enum{ bytes = 13 };

			unsigned char signature[ 3 ];
			unsigned char version[ 3 ];

			unsigned short	image_width;
			unsigned short	image_height;
			unsigned char	image_flags;
			unsigned char	background_color_index;
			unsigned char	pixel_aspect_ratio;

		} _MIST_PACKED;

		struct _image_descriptor_
		{
			enum{ bytes = 10 };

			unsigned char	code;
			unsigned short	left;
			unsigned short	top;
			unsigned short	image_width;
			unsigned short	image_height;
			unsigned char	image_flags;

		} _MIST_PACKED;

		struct _graphic_control_extension_
		{
			enum{ bytes = 7 };

			unsigned char	code;
			unsigned char	label;
			unsigned char	block_size;
			unsigned char	flags;
			unsigned short	delay_time;
			unsigned char	transparent_color_index;

		} _MIST_PACKED;

		struct _comment_extension_
		{
			enum{ bytes = 2 };

			unsigned char	code;
			unsigned char	label;

		} _MIST_PACKED;

		struct _plain_text_extension_
		{
			enum{ bytes = 15 };

			unsigned char	code;
			unsigned char	label;
			unsigned char	block_size;
			unsigned short	left;
			unsigned short	top;
			unsigned short	grid_width;
			unsigned short	grid_height;
			unsigned char	cell_width;
			unsigned char	cell_height;
			unsigned char	text_foreground_color_index;
			unsigned char	text_background_color_index;

		} _MIST_PACKED;

		struct _application_extension_
		{
			enum{ bytes = 14 };

			unsigned char	code;
			unsigned char	label;
			unsigned char	block_size;
			unsigned char	application_identifier[ 8 ];
			unsigned char	authentication_code[ 3 ];

		} _MIST_PACKED;



#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( pop, gif_align )
#endif
	// 構造体内のアライメントを1バイトに設定し，パディングを禁止する ～ ここまで ～

	template < class T, class Allocator >
	struct gif_controller
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		typedef typename array2< T, Allocator >::difference_type difference_type;
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;


		static difference_type decode_LZW( const unsigned char *buff, unsigned char * &out, size_type num_bytes )
		{
			difference_type initial_code_size = buff[ 0 ];
			difference_type clear_code = 1 << initial_code_size;
			difference_type end_code = clear_code + 1;

			difference_type code_size = initial_code_size + 1;
			difference_type code_mask = ( 1 << code_size ) - 1;
			difference_type bits = 0, data = 0, available = 0;

			if( code_size < 3 || 12 < code_size )
			{
				// 不適切なLZWの圧縮データである
				return( -1 );
			}

			out = new unsigned char[ num_bytes ];
			memset( out, 0, sizeof( unsigned char ) * num_bytes );

			static difference_type prefix[ 4096 ];
			static difference_type suffix[ 4096 ];
			std::stack< difference_type > stack;

			const unsigned char *pointer = buff + 1;

			for( difference_type i = 0 ; i < clear_code ; i++ )
			{
				prefix[ i ] = 0;
				suffix[ i ] = static_cast< unsigned char >( i );
			}
			
			difference_type count = 0, code = 0, old_code = -1, first_character = 0;
			while( pointer < buff + num_bytes )
			{
				difference_type num = pointer[ 0 ];
				pointer++;

				for( difference_type i = 0 ; i < num ; i++ )
				{
					difference_type ch = pointer[ i ];
					data += ch << bits;
					bits += 8;

					while( bits >= code_size )
					{
						code = data & code_mask;
						data >>= code_size;
						bits -= code_size;

						if( code == end_code )
						{
							// 終了コードが見つかったので，デコード処理を終了する
							return( pointer - buff + i );
						}

						if( code == clear_code )
						{
							// クリアコードが見つかったので，辞書を初期化する
							code_size = initial_code_size + 1;
							code_mask = ( 1 << code_size ) - 1;
							available = clear_code + 2;
							old_code = -1;
						}
						else if ( old_code == -1 )
						{
							out[ count++ ] = static_cast< unsigned char >( suffix[ code ] );
							first_character = old_code = code;
						}
						else if( code > available )
						{
							delete [] out;
							out = NULL;
							return( -1 );
						}
						else
						{
							difference_type incode = code;
							if( code == available )
							{
								stack.push( first_character );
								code = old_code;
							}
							while( code > clear_code )
							{
								stack.push( suffix[ code ] );
								code = prefix[ code ];
							}

							stack.push( first_character = suffix[ code ] );
							prefix[ available ] = old_code;
							suffix[ available ] = first_character;
							available++;

							if( ( ( available & code_mask ) == 0) && ( available < 4096 ) )
							{
								code_size++;
								code_mask += available;
							}

							old_code = incode;
							while( !stack.empty( ) )
							{
								out[ count++ ] = static_cast< unsigned char >( stack.top( ) );
								stack.pop( );
							}
						}
					}
				}

				pointer += num;
			}

			return( -1 );
		}


		static bool convert_from_gif_data( unsigned char *gif, size_type num_bytes, array2< T, Allocator > &image )
		{
			static difference_type _2[ ] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };

			// GIF用のヘッダの位置を指定する
			_gif_header_	*pheader		= reinterpret_cast < _gif_header_ * >( gif );
			_gif_header_	&header			= *pheader;

			if( !( header.signature[ 0 ] == 'G' && header.signature[ 1 ] == 'I' && header.signature[ 2 ] == 'F' ) )
			{
				// GIFファイルではない
				return( false );
			}

			bool            global_color_table_flag		= ( header.image_flags & 0x80 ) != 0;
			//difference_type color_resolution			= ( header.image_flags & 0x70 ) >> 4;
			//bool            sort_flag					= ( header.image_flags & 0x08 ) != 0;
			difference_type size_of_global_color_table	= ( header.image_flags & 0x07 );

			if( size_of_global_color_table < 0 || size_of_global_color_table > 11 )
			{
				// GIFファイルではない
				return( false );
			}

			difference_type global_color_table_bytes	= 3 * _2[ size_of_global_color_table + 1 ];

			difference_type width  = header.image_width;
			difference_type height = header.image_height;

			unsigned char *global_color_map_data	= gif + _gif_header_::bytes;
			unsigned char *pointer					= global_color_map_data + ( global_color_table_flag ? global_color_table_bytes : 0 );

			bool ret = true;

			image.resize( width, height );

			bool decode_finished = false;
			while( pointer < gif + num_bytes && !decode_finished )
			{
				switch( pointer[ 0 ] )
				{
				case 0x2c:
					{
						// 画像を記述している部分を発見
						_image_descriptor_	*pimage_header	= reinterpret_cast < _image_descriptor_ * >( pointer );
						_image_descriptor_	&image_header	= *pimage_header;

						bool            local_color_table_flag		= ( image_header.image_flags & 0x80 ) != 0;
						bool            interlace_flag				= ( image_header.image_flags & 0x40 ) != 0;
						//bool            sort_flag					= ( image_header.image_flags & 0x20 ) != 0;
						//difference_type reserved					= ( image_header.image_flags & 0x18 ) >> 3;
						difference_type size_of_local_color_table	= ( image_header.image_flags & 0x07 );
						difference_type local_color_table_bytes		= 3 * _2[ size_of_local_color_table + 1 ];

						difference_type x = image_header.left;
						difference_type y = image_header.top;
						difference_type w = image_header.image_width;
						difference_type h = image_header.image_height;

						if( x + w > width || y + h > height )
						{
							// 画像サイズとあっていない！！
							std::cerr << "Error!!" << std::endl;
							return( false );
						}

						unsigned char *local_color_map_data	= pointer + _image_descriptor_::bytes;
						unsigned char *image_data			= local_color_map_data + ( local_color_table_flag ? local_color_table_bytes : 0 );

						unsigned char *color_map = local_color_table_flag ? local_color_map_data : global_color_map_data;

						unsigned char *buff = NULL;
						difference_type num_bytes = decode_LZW( image_data, buff, w * h );
						unsigned char *p = buff;

						if( num_bytes > 0 )
						{
							if( interlace_flag )
							{
								// 画像に対して4パスの処理がいるらしい
								static difference_type interlace_offset[] = { 0, 4, 2, 1 };
								static difference_type interlace_jump[] = { 8, 8, 4, 2 };
								for( difference_type k = 0 ; k < 4 ; k++ )
								{
									for( difference_type j = y + interlace_offset[ k ] ; j < y + h ; j += interlace_jump[ k ] )
									{
										for( difference_type i = x ; i < x + w ; i++ )
										{
											difference_type index = *p++;
											image( i, j ) = pixel_converter::convert_to( color_map[ index * 3 + 0 ], color_map[ index * 3 + 1 ], color_map[ index * 3 + 2 ] );
										}
									}
								}
							}
							else
							{
								for( difference_type j = y ; j < y + h ; j++ )
								{
									for( difference_type i = x ; i < x + w ; i++ )
									{
										difference_type index = *p++;
										image( i, j ) = pixel_converter::convert_to( color_map[ index * 3 + 0 ], color_map[ index * 3 + 1 ], color_map[ index * 3 + 2 ] );
									}
								}
							}
						}
						else
						{
							std::cerr << "LZW Decode failure." << std::endl;
						}

						delete [] buff;

						pointer += num_bytes;

						if( x == 0 && y == 0 && w == width && h == height )
						{
							// 一番最初のフレームが見つかったので探索を終了する
							decode_finished = true;
							break;
						}
					}
					break;

				case 0x21:
					// エクステンションのコードを識別して，スキップ処理を行う
					switch( pointer[ 1 ] )
					{
					case 0xf9:
						pointer += _graphic_control_extension_::bytes;
						break;

					case 0xfe:
						pointer += _comment_extension_::bytes;
						break;

					case 0x01:
						pointer += _plain_text_extension_::bytes;
						break;

					case 0xff:
						pointer += _application_extension_::bytes;
						break;

					default:
						// 認識不能なエクステンションタグ
						pointer += 2;
						break;
					}

					// ターミネーションブロックの次までスキップする
					while( *pointer != 0x00 )
					{
						pointer++;
					}
					pointer++;
					break;


				default:
					// Data Sub-blocks
					pointer += pointer[ 0 ] + 1;
					break;
				}
			}

			return( ret );
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

			bool ret = convert_from_gif_data( buff, filesize, image );
			delete [] buff;
			return( ret );
		}
	};
}


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_gif_group GIF 画像入出力
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/gif.h>
//! @endcode
//!
//!  @{



/// @brief GIF画像をMISTコンテナに読み込む
//! 
//! LZW圧縮のかかったGIF等のさまざまなフォーマットを読み込むことが可能．
//! また，出力するMISTコンテナがカラー画像かグレースケール画像かによって，ピクセルデータを適切に変換する
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_gif( array2< T, Allocator > &image, const std::string &filename )
{
	return( __gif_controller__::gif_controller< T, Allocator >::read( image, filename ) );
}


/// @brief GIF画像をMISTコンテナに読み込む
//! 
//! LZW圧縮のかかったGIF等のさまざまなフォーマットを読み込むことが可能．
//! また，出力するMISTコンテナがカラー画像かグレースケール画像かによって，ピクセルデータを適切に変換する
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_gif( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_gif( image, wstr2str( filename ) ) );
}


/// @}
//  GIF 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_GIF__
