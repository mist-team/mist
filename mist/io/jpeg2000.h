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

/// @file mist/io/jpeg.h
//!
//! @brief JPEG2000画像を読み書きするためのライブラリ
//!
//! 本ライブラリは，http://www.openjpeg.org/ で開発が行われている OpenJPEG ライブラリを利用している
//! エンコードとデコードは本ライブラリのAPIを用いて実装されているため，別途ライブラリを用意する必要がある
//!
#ifndef __INCLUDE_MIST_JPEG2000__
#define __INCLUDE_MIST_JPEG2000__


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

extern "C"
{
#ifndef OPJ_STATIC
	#define OPJ_STATIC
#endif

#include <openjpeg.h>
}


// mist名前空間の始まり
_MIST_BEGIN


namespace __jpeg2000_controller__
{
	template < bool b >
	struct _create_opj_image_
	{
		// グレースケールの画像を作成する
		template < class T, class Allocator >
		static opj_image_t *create( const array2< T, Allocator > &image, const opj_cparameters_t &param )
		{
			opj_image_cmptparm_t cmptparm[ 1 ];

			memset( cmptparm, 0, 1 * sizeof( opj_image_cmptparm_t ) );

			// 各色プレーンの設定を行う
			for( size_t i = 0 ; i < 1 ; i++ )
			{
				cmptparm[ i ].prec = 8;
				cmptparm[ i ].bpp  = 8;
				cmptparm[ i ].sgnd = 0;
				cmptparm[ i ].dx   = param.subsampling_dx;
				cmptparm[ i ].dy   = param.subsampling_dy;
				cmptparm[ i ].w    = static_cast< int >( image.width( ) );
				cmptparm[ i ].h    = static_cast< int >( image.height( ) );
			}

			// カラー画像として作成する
			opj_image_t *img = opj_image_create( 1, cmptparm, CLRSPC_GRAY );

			// 画像のオフセットとレンダリンググリッドを設定する
			img->x0 = param.image_offset_x0;
			img->y0 = param.image_offset_y0;
			img->x1 = img->x0 + ( static_cast< int >( image.width( ) )  - 1 ) * param.subsampling_dx + 1;
			img->y1 = img->y0 + ( static_cast< int >( image.height( ) ) - 1 ) * param.subsampling_dy + 1;

			// 画像データをコピーする
			for( size_t i = 0 ; i < image.size( ) ; i++ )
			{
				img->comps[ 0 ].data[ i ] = static_cast< int >( image[ i ] );
			}

			return( img );
		}
	};

	template < >
	struct _create_opj_image_< true >
	{
		// カラー画像を作成する
		template < class T, class Allocator >
		static opj_image_t *create( const array2< T, Allocator > &image, const opj_cparameters_t &param )
		{
			opj_image_cmptparm_t cmptparm[ 3 ];

			memset( cmptparm, 0, 3 * sizeof( opj_image_cmptparm_t ) );

			// 各色プレーンの設定を行う
			for( size_t i = 0 ; i < 3 ; i++ )
			{
				cmptparm[ i ].prec = 8;
				cmptparm[ i ].bpp  = 8;
				cmptparm[ i ].sgnd = 0;
				cmptparm[ i ].dx   = param.subsampling_dx;
				cmptparm[ i ].dy   = param.subsampling_dy;
				cmptparm[ i ].w    = static_cast< int >( image.width( ) );
				cmptparm[ i ].h    = static_cast< int >( image.height( ) );
			}

			// カラー画像として作成する
			opj_image_t *img = opj_image_create( 3, cmptparm, CLRSPC_SRGB );

			// 画像のオフセットとレンダリンググリッドを設定する
			img->x0 = param.image_offset_x0;
			img->y0 = param.image_offset_y0;
			img->x1 = img->x0 + ( static_cast< int >( image.width( ) )  - 1 ) * param.subsampling_dx + 1;
			img->y1 = img->y0 + ( static_cast< int >( image.height( ) ) - 1 ) * param.subsampling_dy + 1;

			// 画像データをコピーする
			for( size_t i = 0 ; i < image.size( ) ; i++ )
			{
				img->comps[ 0 ].data[ i ] = static_cast< int >( image[ i ].r );
				img->comps[ 1 ].data[ i ] = static_cast< int >( image[ i ].g );
				img->comps[ 2 ].data[ i ] = static_cast< int >( image[ i ].b );
			}

			return( img );
		}
	};

	inline std::string to_lower_case( const std::string &str )
	{
		std::string s = "";
		for( std::string::size_type i = 0 ; i < str.size( ) ; i++ )
		{
			s += static_cast< char >( tolower( str[ i ] ) );
		}
		return( s );
	}

	inline std::string get_ext( const std::string &str )
	{
		std::string::size_type index = str.rfind( '.' );
		if( index == str.npos )
		{
			return( "" );
		}
		return( str.substr( index ) );
	}

	template < class T, class Allocator >
	struct jpeg2000_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			typedef typename array2< T, Allocator >::size_type       size_type;
			typedef typename array2< T, Allocator >::difference_type difference_type;

			// ファイルからデータをすべて読み込む
			FILE *fp = fopen( filename.c_str( ), "rb" );
			if( fp == NULL )
			{
				std::cerr << "Can't open file " << filename << std::endl;
				return( false );
			}

			// ファイルサイズを取得
			fseek( fp, 0, SEEK_END );
			size_type filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			// データを少しずつ読み込む
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

			// ファイルを閉じる
			fclose( fp );

			// デコーダを決定する
			std::string ext = to_lower_case( get_ext( filename ) );
			OPJ_CODEC_FORMAT codec;
			if( ext == ".jp2" )
			{
				codec = CODEC_JP2;
			}
			else if( ext == ".j2k" || ext == ".j2c" )
			{
				codec = CODEC_J2K;
			}
			else if( ext == ".jpt" )
			{
				codec = CODEC_JPT;
			}

			// OpenJPEGライブラリの圧縮パラメータ
			opj_dparameters_t param;

			// デコードパラメータをデフォルトに設定する
			opj_set_default_decoder_parameters( &param );

			// デコーダエンジンを初期化する
			//opj_dinfo_t *dinfo = opj_create_decompress( CODEC_UNKNOWN );
			opj_dinfo_t *dinfo = opj_create_decompress( codec );

			// エラーハンドリングの関数を設定する
			opj_event_mgr_t event_mgr;
			memset( &event_mgr, 0, sizeof( opj_event_mgr_t ) );
			//event_mgr.error_handler   = error;
			//event_mgr.warning_handler = warning;
			//event_mgr.info_handler    = info;
			opj_set_event_mgr( (opj_common_ptr)dinfo, &event_mgr, stderr );

			// デコーダにパラメータを設定する
			opj_setup_decoder( dinfo, &param );

			// デコード用のメモリストリームを準備する
			opj_cio_t *cio = opj_cio_open( ( opj_common_ptr )dinfo, buff, static_cast< int >( filesize ) );

			// 画像をデコードする
			opj_image_t *img = opj_decode( dinfo, cio );			
			if( img == NULL )
			{
				std::cerr << "Failed to read JPEG2000 image from " << filename << std::endl;

				opj_destroy_decompress( dinfo );
				opj_cio_close( cio );

				delete [] buff;

				return( false );
			}
			else
			{
				delete [] buff;
			}

			if( img->numcomps != 1 && img->numcomps != 3 )
			{
				// おかしな画像が読み込まれた？
				opj_destroy_decompress( dinfo );
				opj_cio_close( cio );

				std::cerr << "Invalid image is loaded." << std::endl;

				return( false );
			}

			// OpenJPEG ライブラリの画像形式からMISTの形式に変換する
			image.resize( img->comps[ 0 ].w, img->comps[ 0 ].h );

			if( img->numcomps == 1 )
			{
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					int pix = img->comps[ 0 ].data[ i ];
					image[ i ] = pixel_converter::convert_to( pix, pix, pix );
				}
			}
			else // img->numcomps == 3
			{
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[ i ] = pixel_converter::convert_to( img->comps[ 0 ].data[ i ], img->comps[ 1 ].data[ i ], img->comps[ 2 ].data[ i ] );
				}
			}


			// OpenJPEG ライブラリで使用したリソース等を開放する
			opj_cio_close( cio );
			opj_destroy_decompress( dinfo );
			opj_image_destroy( img );

			return( true );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, double compression_level )
		{
			typedef typename array2< T, Allocator >::size_type       size_type;
			typedef typename array2< T, Allocator >::difference_type difference_type;

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

			// OpenJPEGライブラリの圧縮パラメータ
			opj_cparameters_t param;

			// エンコードパラメータをデフォルトに設定する
			opj_set_default_encoder_parameters( &param );

			// 圧縮率を設定する
			// 0はLossless圧縮
			param.tcp_rates[ 0 ] = static_cast< float >( compression_level );
			param.tcp_numlayers  = 1;
			param.cp_disto_alloc = 1;

			// コメントを埋め込む
			static char comment[] = "Created by MIST with OpenJPEG Library";
			param.cp_comment = comment;

			// OpenJPEGライブラリ内部で使用する画像形式に変換する
			opj_image_t *img = _create_opj_image_< is_color< T >::value >::create( image, param );

			// 色のコンポーネントの数を調べる
			param.tcp_mct = img->numcomps == 3 ? 1 : 0;

			// エラーハンドリングの関数を設定する
			opj_event_mgr_t event_mgr;
			memset( &event_mgr, 0, sizeof( opj_event_mgr_t ) );
			//event_mgr.error_handler   = error;
			//event_mgr.warning_handler = warning;
			//event_mgr.info_handler    = info;

			// JPEG2000の圧縮エンジンを初期化する
			opj_cinfo_t * cinfo = opj_create_compress( CODEC_JP2 );

			// エラー処理用の関数を設定する
			opj_set_event_mgr( ( opj_common_ptr )cinfo, &event_mgr, stderr );

			// JPEG2000 の圧縮エンジンにパラメータと画像を設定する
			opj_setup_encoder( cinfo, &param, img );

			// 画像をメモリ上に展開するためのオブジェクトを初期化する
			opj_cio_t *cio = opj_cio_open( ( opj_common_ptr )cinfo, NULL, 0 );

			// JPEG2000で画像を圧縮する
			if( !opj_encode( cinfo, cio, img, NULL ) )
			{
				opj_cio_close( cio );
				opj_destroy_compress( cinfo );
				opj_image_destroy( img );

				// 圧縮に失敗
				return( false );
			}

			// 圧縮後のデータ長を取得する
			difference_type codestream_length = cio_tell( cio );

			// 画像出力用のファイルポインタを作成する
			FILE *fp = fopen( filename.c_str( ), "wb" );

			if( fp == NULL )
			{
				// 画像を作成することができなかった・・・
				opj_cio_close( cio );
				opj_destroy_compress( cinfo );
				opj_image_destroy( img );
				std::cerr << "Failed to write image to " << filename << std::endl;
				return( false );
			}

			// 画像データをファイルに少しずつ書き出す
			unsigned char *pointer = cio->buffer;
			while( codestream_length > 0 )
			{
				size_type write_size = fwrite( pointer, sizeof( unsigned char ), codestream_length > 1024 ? 1024 : codestream_length, fp );
				pointer           += write_size;
				codestream_length -= write_size;
			}

			// ファイルを閉じる
			fclose( fp );

			// OpenJPEG ライブラリで使用したリソース等を開放する
			opj_cio_close( cio );
			opj_destroy_compress( cinfo );
			opj_image_destroy( img );

			return( true );
		}
	};
}


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_jpeg2000_group JPEG2000 画像入出力
//!
//! JPEG2000ファイルを読み込んで，MISTコンテナへ格納する．
//! また，MISTコンテナからJPEG2000ファイルへの書き出しをサポート．
//! 別途 OpenJPEG (http://www.openjpeg.org/) が提供するフリーのライブラリが必要です．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/jpeg2000.h>
//! @endcode
//!
//!  @{


/// @brief JPEG2000画像をMISTコンテナに読み込む
//! 
//! 全てのJPEG2000画像形式を読み込むことが可能です
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_jpeg2000( array2< T, Allocator > &image, const std::string &filename )
{
	return( __jpeg2000_controller__::jpeg2000_controller< T, Allocator >::read( image, filename ) );
}


/// @brief JPEG2000画像をMISTコンテナに読み込む
//! 
//! 全てのJPEG2000画像形式を読み込むことが可能です
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_jpeg2000( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_jpeg2000( image, wstr2str( filename ) ) );
}


/// @brief MISTコンテナの画像をJPEG2000形式でファイルに出力する
//! 
//! @attention JPEG2000の圧縮率は数値が大きいほど圧縮率が高くなる．0を指定するとLossless圧縮を行う．（デフォルト値は0）
//! 
//! @param[in] image             … 出力画像を保持するMISTコンテナ
//! @param[in] filename          … 出力ファイル名
//! @param[in] compression_level … 画像の圧縮率（例えば2を指定すると画像サイズを2分の1にする．また，0を指定するとLossless圧縮を行う）
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_jpeg2000( const array2< T, Allocator > &image, const std::string &filename, double compression_level = 0 )
{
	return( __jpeg2000_controller__::jpeg2000_controller< T, Allocator >::write( image, filename, compression_level ) );
}


/// @brief MISTコンテナの画像をJPEG2000形式でファイルに出力する
//! 
//! @attention JPEG2000の圧縮率は数値が大きいほど圧縮率が高くなる．0を指定するとLossless圧縮を行う．（デフォルト値は0）
//! 
//! @param[in] image             … 出力画像を保持するMISTコンテナ
//! @param[in] filename          … 出力ファイル名
//! @param[in] compression_level … 画像の圧縮率（例えば2を指定すると画像サイズを2分の1にする．また，0を指定するとLossless圧縮を行う）
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_jpeg2000( const array2< T, Allocator > &image, const std::wstring &filename, double compression_level = 0 )
{
	return( write_jpeg2000( image, wstr2str( filename ), compression_level ) );
}


/// @}
//  JPEG2000 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_JPEG2000__
