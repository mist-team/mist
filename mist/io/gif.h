/// @file mist/io/gif.h
//!
//! @brief GIF画像を読み書きするためのライブラリ
//!
//! 本ライブラリは，http://sourceforge.net/projects/libungif/ で開発が行われている giflib もしくは libungif を利用している
//! エンコードとデコードは本ライブラリのAPIを用いて実装されているため，別途ライブラリを用意する必要がある
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


// mist名前空間の始まり
_MIST_BEGIN


namespace __gif_controller__
{
	extern "C"{
		#include <gif_lib.h>
	}

	template < class T, class Allocator >
	struct gif_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			size_type i, j;

			// GIFファイルを読み込んで，ヘッダ情報を取得する
			GifFileType *GifFile = DGifOpenFileName( filename.c_str( ) );

			if( GifFile == NULL )
			{
				return( false );
			}

			size_type width  = static_cast< size_type >( GifFile->SWidth );
			size_type height = static_cast< size_type >( GifFile->SHeight );
			GifRowType *image_buffer = new GifRowType[ height ];

			// 1行あたりのバイト数
			size_type size = width * sizeof( GifPixelType );

			image_buffer[ 0 ] = new GifPixelType[ size ];

			// 背景色を設定する
			for( i = 0 ; i < width ; i++ )
			{
				image_buffer[ 0 ][ i ] = GifFile->SBackGroundColor;
			}

			for( i = 1 ; i < height ; i++ )
			{
				image_buffer[ i ] = new GifPixelType[ size ];
				memcpy( image_buffer[ i ], image_buffer[ 0 ], size );
			}

			// GIFファイルの内容を読み込む
			GifRecordType record_type;
			do
			{
				if( DGifGetRecordType( GifFile, &record_type ) == GIF_ERROR )
				{
					// エラーが発生！！
					// 何らかのエラー処理をする
					std::cout << "Error!!" << std::endl;
				}

				switch( record_type )
				{
				case IMAGE_DESC_RECORD_TYPE:
					{
						if( DGifGetImageDesc( GifFile ) == GIF_ERROR )
						{
							// エラーが発生！！
							// 何らかのエラー処理をする
							std::cout << "Error!!" << std::endl;
						}

						size_type x = GifFile->Image.Left;
						size_type y = GifFile->Image.Top;
						size_type w = GifFile->Image.Width;
						size_type h = GifFile->Image.Height;

						if( x + w > width || y + h > height )
						{
							// Image is not confined to screen dimension, aborted
							// エラーが発生！！
							std::cout << "Error!!" << std::endl;
						}

						if( GifFile->Image.Interlace )
						{
							// 画像に対して4パスの処理がいるらしい
							static size_type InterlacedOffset[] = { 0, 4, 2, 1 };	// The way Interlaced image should.
							static size_type InterlacedJumps[] = { 8, 8, 4, 2 };    // be read - offsets and jumps...
							for( i = 0 ; i < 4 ; i++ )
							{
								for( j = y + InterlacedOffset[ i ] ; j < y + h ; j += InterlacedJumps[ i ] )
								{
									if( DGifGetLine( GifFile, &image_buffer[ j ][ x ], static_cast< int >( w ) ) == GIF_ERROR )
									{
										// エラーが発生！！
										// 何らかのエラー処理をする
										std::cout << "Error!!" << std::endl;
									}
								}
							}
						}
						else
						{
							for( i = 0 ; i < h ; i++ )
							{
								if( DGifGetLine( GifFile, &image_buffer[ y++ ][ x ], static_cast< int >( w ) ) == GIF_ERROR )
								{
									// エラーが発生！！
									// 何らかのエラー処理をする
									std::cout << "Error!!" << std::endl;
								}
							}
						}

						if( x == 0 && y == 0 && w == width && h == height )
						{
							// 一番最初のフレームが見つかったので探索を終了する
							record_type = TERMINATE_RECORD_TYPE;
						}
					}
					break;

				case EXTENSION_RECORD_TYPE:
					{
						// 全ての拡張ブロックをスキップする
						GifByteType *extention;
						int ext_code;
						if( DGifGetExtension( GifFile, &ext_code, &extention ) == GIF_ERROR )
						{
							// エラーが発生！！
							// 何らかのエラー処理をする
							std::cout << "Error!!" << std::endl;
						}

						while( extention != NULL )
						{
							if( DGifGetExtensionNext( GifFile, &extention ) == GIF_ERROR )
							{
								// エラーが発生！！
								// 何らかのエラー処理をする
								std::cout << "Error!!" << std::endl;
							}
						}
					}
					break;

				case TERMINATE_RECORD_TYPE:
					break;

				default:
		 		    // 不明なレコードタイプ
					break;
				}
			} while( record_type != TERMINATE_RECORD_TYPE );

			ColorMapObject *color_map = GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap;

			size_type color_map_size = static_cast< size_type >( color_map->ColorCount );

			image.resize( width, height );

			for( j = 0 ; j < height ; j++ )
			{
				for( i = 0 ; i < width ; i++ )
				{
					GifColorType *element = color_map->Colors + image_buffer[ j ][ i ];
					image( i, j ) = pixel_converter::convert_to( element->Red, element->Green, element->Blue );
				}
			}

			if( DGifCloseFile( GifFile ) == GIF_ERROR )
			{
				// エラーが発生！！
				// 何らかのエラー処理をする
				std::cout << "Error!!" << std::endl;
			}

			for( i = 0 ; i < height ; i++ )
			{
				delete [] image_buffer[ i ];
			}
			delete [] image_buffer;

			return( true );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			int color_map_size = 256;
			ColorMapObject *color_map = MakeMapObject( color_map_size, NULL );

			if( color_map == NULL )
			{
				// エラーが発生！！
				// 何らかのエラー処理をする
				std::cout << "Error!!" << std::endl;
			}

			size_type width  = image.width( );
			size_type height = image.height( );

			GifByteType *buffer = new GifByteType[ width * height ];
			GifByteType *red_buffer   = new GifByteType[ width * height ];
			GifByteType *green_buffer = new GifByteType[ width * height ];
			GifByteType *blue_buffer  = new GifByteType[ width * height ];

			size_type i;
			for( i = 0 ; i < width * height ; i++ )
			{
				color_type c = limits_0_255( pixel_converter::convert_from( image[ i ] ) );
				red_buffer[ i ]   = c.r;
				green_buffer[ i ] = c.g;
				blue_buffer[ i ]  = c.b;
			}

			if( QuantizeBuffer( static_cast< unsigned int >( width ), static_cast< unsigned int >( height ), &color_map_size, red_buffer, green_buffer, blue_buffer, buffer, color_map->Colors ) == GIF_ERROR )
			{
				// エラーが発生！！
				// 何らかのエラー処理をする
				std::cout << "Error!!" << std::endl;
			}

			GifFileType *GifFile = EGifOpenFileName( filename.c_str( ), false );
			if( GifFile == NULL )
			{
				// エラーが発生！！
				// 何らかのエラー処理をする
				std::cout << "Error!!" << std::endl;
			}

			EGifSetGifVersion( "89a" );

			if( EGifPutScreenDesc( GifFile, static_cast< int >( width ), static_cast< int >( height ), 8, 0, color_map ) == GIF_ERROR ||
					EGifPutImageDesc( GifFile, 0, 0, static_cast< int >( width ), static_cast< int >( height ), false, NULL ) == GIF_ERROR )
			{
				// エラーが発生！！
				// 何らかのエラー処理をする
				std::cout << "Error!!" << std::endl;
			}

			GifByteType *pointer = buffer;
			for( i = 0 ; i < height ; i++ )
			{
				if( EGifPutLine( GifFile, pointer, static_cast< int >( width ) ) == GIF_ERROR )
				{
					// エラーが発生！！
					// 何らかのエラー処理をする
					std::cout << "Error!!" << std::endl;
				}

				pointer += width;
			}

			if( EGifCloseFile( GifFile ) == GIF_ERROR )
			{
				// エラーが発生！！
				// 何らかのエラー処理をする
			}

			FreeMapObject( color_map );

			delete [] buffer;
			delete [] red_buffer;
			delete [] green_buffer;
			delete [] blue_buffer;

			return( true );
		}
	};
}


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_gif_group GIF 画像入出力
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


/// @brief MISTコンテナの画像をGIF形式でファイルに出力する
//! 
//! @attention LZW圧縮のかかったGIF画像を出力する場合は，giflib ライブラリ側でLZW圧縮が有効になっている必要がある
//! 
//! @param[in] image    … 出力画像を保持するMISTコンテナ
//! @param[in] filename … 出力ファイル名
//! @param[in] use_lzw_compression … LZW圧縮されたGIF画像を出力するかどうか
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//!
template < class T, class Allocator >
bool write_gif( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression = _LZW_COMPRESSION_SUPPORT_ )
{
	return( __gif_controller__::gif_controller< T, Allocator >::write( image, filename, use_lzw_compression ) );
}


/// @}
//  GIF 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_GIF__
