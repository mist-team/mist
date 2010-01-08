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

/// @file mist/io/raw.h
//!
//! @brief バイナリ画像を読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_RAW__
#define __INCLUDE_MIST_RAW__


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

#include <iostream>
#include <string>
#include <zlib.h>



// mist名前空間の始まり
_MIST_BEGIN


namespace __raw_controller__
{
	struct resize_image
	{
		// ファイルから読み出されたデータ量が，指定されたものよりも少ない場合
		template < class T, class Allocator >
		static bool resize( array1< T, Allocator > &image, typename array1< T, Allocator >::size_type num_elements )
		{
			typedef typename array1< T, Allocator >::size_type size_type;

			if( num_elements == 0 )
			{
				return( false );
			}
			else if( image.size( ) == num_elements )
			{
				return( true );
			}
			else
			{
				array1< T, Allocator > tmp( image );
				image.resize( num_elements );
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[ i ] = tmp[ i ];
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool resize( array2< T, Allocator > &image, typename array2< T, Allocator >::size_type num_elements )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			size_type num_lines = num_elements / image.width( );
			if( num_lines == 0 )
			{
				return( false );
			}
			else if( image.height( ) == num_lines )
			{
				return( true );
			}
			else
			{
				array2< T, Allocator > tmp( image );
				image.resize( image.width( ), num_lines );
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[i] = tmp[i];
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool resize( array3< T, Allocator > &image, typename array3< T, Allocator >::size_type num_elements )
		{
			typedef typename array3< T, Allocator >::size_type size_type;

			size_type num_slices = num_elements / ( image.width( ) * image.height( ) );
			if( num_slices == 0 )
			{
				return( false );
			}
			else if( image.depth( ) == num_slices )
			{
				return( true );
			}
			else
			{
				array3< T, Allocator > tmp( image );
				image.resize( image.width( ), image.height( ), num_slices );
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[ i ] = tmp[ i ];
				}
			}
			return( true );
		}
	};

	struct raw_controller
	{
		template < class Array, class Functor, class ValueType >
		static bool read( Array &image,
							const std::string &filename,
							typename Array::size_type w,
							typename Array::size_type h,
							typename Array::size_type d,
							typename Array::value_type offset,
							bool from_little_endian,
							typename Array::size_type skip_num_bytes,
							Functor f,
							ValueType /* v */
						)
		{
			typedef typename Array::value_type value_type;
			typedef typename Array::size_type size_type;

			gzFile fp;

			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				std::cerr << "Error occured while opening RAW file format in [" << filename << "]" << std::endl;
				return( false );
			}

			image.resize( w, h, d );

			size_type byte = sizeof( ValueType );
			unsigned char tmparray[ sizeof( ValueType ) * 4096 ];
			byte_array< ValueType > data;

			f( 0.0 );

			size_type i = 0;
			unsigned int pre_progress = 0;
			while( !gzeof( fp ) )
			{
				size_type num = gzread( fp, ( void * )tmparray, sizeof( ValueType ) * 4096 );

				if( num <= skip_num_bytes )
				{
					skip_num_bytes -= num;
					continue;
				}

				size_type j = skip_num_bytes;
				skip_num_bytes = 0;

				for( ; i < image.size( ) && j < num ; j += byte )
				{
					for( size_type l = 0 ; l < byte ; l++ )
					{
						data[ l ] = tmparray[ j + l ];
					}
					image[ i++ ] = static_cast< value_type >( to_current_endian( data, from_little_endian ).get_value( ) ) + offset;
				}

				if( i >= image.size( ) )
				{
					break;
				}

				// 進行状況を0～100％で表示する
				// コールバック関数の戻り値が false になった場合は処理を中断し，制御を返す
				unsigned int progress = static_cast< unsigned int >( static_cast< double >( i + 1 ) / static_cast< double >( image.size( ) ) * 100.0 );
				if( progress != pre_progress )
				{
					pre_progress = progress;
					if( !f( progress ) )
					{
						image.clear( );
						gzclose( fp );
						return( false );
					}
				}
			}

			gzclose( fp );

			// ファイルから読み出されたデータ量が，指定されたものよりも少ない場合
			if( !resize_image::resize( image, i ) )
			{
				// 1枚もデータが読み込まれなかった場合
				f( 100.1 );
				return( false );
			}
			else
			{
				f( 100.1 );
				return( true );
			}
		}


		template < class Array, class Functor, class ValueType >
		static bool write( const Array &image, const std::string &filename, typename Array::value_type offset, bool to_little_endian, Functor f, ValueType v )
		{
			typedef typename Array::value_type value_type;
			typedef typename Array::size_type size_type;

			FILE *fp;

			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "Error occured while opening RAW file format in [" << filename << "]" << std::endl;
				return( false );
			}

			size_type w = image.width( );
			size_type h = image.height( );
			size_type d = image.depth( );
			unsigned char tmparray[ sizeof( ValueType ) * 4096 ];
			byte_array< ValueType > data;

			f( 0.0 );

			size_type i = 0, n, l, pre_progress = 0;
			while( i < image.size( ) )
			{
				for( n = 0 ; i < image.size( ) && n < sizeof( ValueType ) * 4096 ; i++, n += sizeof( ValueType ) )
				{
					data.set_value( static_cast< ValueType >( image[ i ] + offset ) );
					data = from_current_endian( data, to_little_endian );
					for( l = 0 ; l < sizeof( ValueType ) ; l++ )
					{
						tmparray[ n + l ] = data[ l ];
					}
				}
				fwrite( tmparray, 1, n, fp );

				// 進行状況を0～100％で表示する
				// コールバック関数の戻り値が false になった場合は処理を中断し，制御を返す
				size_type progress = static_cast< size_type >( static_cast< double >( i ) / static_cast< double >( image.size( ) ) * 100.0 );
				if( progress != pre_progress )
				{
					pre_progress = progress;
					if( !f( static_cast< double >( progress ) ) )
					{
						fclose( fp );
						return( false );
					}
				}
			}

			fclose( fp );

			f( 100.1 );

			return( true );
		}

		template < class Array, class Functor, class ValueType >
		static bool write_gz( const Array &image, const std::string &filename, typename Array::value_type offset, bool to_little_endian, Functor f, ValueType v )
		{
			typedef typename Array::value_type value_type;
			typedef typename Array::size_type size_type;

			gzFile fp;

			if( ( fp = gzopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "Error occured while opening RAW + GZ file format in [" << filename << "]" << std::endl;
				return( false );
			}

			unsigned char tmparray[ sizeof( ValueType ) * 4096 ];
			byte_array< ValueType > data;

			f( 0.0 );

			size_type i = 0, n, l, pre_progress = 0;
			while( i < image.size( ) )
			{
				for( n = 0 ; i < image.size( ) && n < sizeof( ValueType ) * 4096 ; i++, n += sizeof( ValueType ) )
				{
					data.set_value( static_cast< ValueType >( image[ i ] + offset ) );
					data = from_current_endian( data, to_little_endian );
					for( l = 0 ; l < sizeof( ValueType ) ; l++ )
					{
						tmparray[ n + l ] = data[ l ];
					}
				}
				gzwrite( fp, tmparray, static_cast< unsigned int >( n ) );

				// 進行状況を0～100％で表示する
				// コールバック関数の戻り値が false になった場合は処理を中断し，制御を返す
				size_type progress = static_cast< size_type >( static_cast< double >( i ) / static_cast< double >( image.size( ) ) * 100.0 );
				if( progress != pre_progress )
				{
					pre_progress = progress;
					if( !f( static_cast< double >( progress ) ) )
					{
						gzclose( fp );
						return( false );
					}
				}
			}

			gzclose( fp );

			f( 100.1 );

			return( true );
		}
	};
}


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_raw_group RAW 画像入出力
//!
//! RAWファイル（GZIP圧縮に対応）の読み込みと書き込みサポートをMISTに追加するものです．
//! 別途 http://www.zlib.org が提供するフリーのZLIBライブラリが必要です．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/raw.h>
//! @endcode
//!
//!  @{




/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像の幅
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  __dmy__            … 読み込もうとしているデータの型をあらわすダミーオブジェクト（short型のオブジェクトなど）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::size_type w,
							typename array< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	return( __raw_controller__::raw_controller::read( image, filename, w, 1, 1, offset, from_little_endian, 0, callback, __dmy__ ) );
}

/// @brief MISTコンテナ内の画像を 無圧縮RAW 画像として出力する
//! 
//! @attention データを変換して保存する際に，値のオーバーフローなどは無視するので注意が必要
//! 
//! @param[in] image            … 画像を読み込む先のMISTコンテナ
//! @param[in] filename         … 入力ファイル名
//! @param[in] offset           … 画像から引き算するオフセット値
//! @param[in] to_little_endian … 出力画像のデータ形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  __dmy__         … 出力するデータの型をあらわすダミーオブジェクト（short型のオブジェクトなど）
//! @param[in] callback         … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool write_raw( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, ValueType __dmy__, Functor callback )
{
	return( __raw_controller__::raw_controller::write( image, filename, offset, to_little_endian, callback, __dmy__ ) );
}

/// @brief MISTコンテナ内の画像を GZ圧縮RAW 画像として出力する
//! 
//! @attention データを変換して保存する際に，値のオーバーフローなどは無視するので注意が必要
//! 
//! @param[in] image            … 画像を読み込む先のMISTコンテナ
//! @param[in] filename         … 入力ファイル名
//! @param[in] offset           … 画像から引き算するオフセット値
//! @param[in] to_little_endian … 出力画像のデータ形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  __dmy__         … 出力するデータの型をあらわすダミーオブジェクト（short型のオブジェクトなど）
//! @param[in] callback         … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool write_raw_gz( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, ValueType __dmy__, Functor callback )
{
	return( __raw_controller__::raw_controller::write_gz( image, filename, offset, to_little_endian, callback, __dmy__ ) );
}



/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像の幅
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::size_type w,
							typename array< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, offset, from_little_endian, v, callback ) );
}



/// @brief MISTコンテナ内の画像を 無圧縮RAW 画像として出力する
//! 
//! @param[in] image            … 画像を読み込む先のMISTコンテナ
//! @param[in] filename         … 入力ファイル名
//! @param[in] offset           … 画像から引き算するオフセット値
//! @param[in] to_little_endian … 出力画像のデータ形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in] callback         … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class Functor >
bool write_raw( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	typename array< T, Allocator >::value_type v( 0 );
	return( write_raw( image, filename, offset, to_little_endian, v, callback ) );
}


/// @brief MISTコンテナ内の画像を GZ圧縮RAW 画像として出力する
//! 
//! @param[in] image            … 画像を読み込む先のMISTコンテナ
//! @param[in] filename         … 入力ファイル名
//! @param[in] offset           … 画像から引き算するオフセット値
//! @param[in] to_little_endian … 出力画像のデータ形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in] callback         … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class Functor >
bool write_raw_gz( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	typename array< T, Allocator >::value_type v( 0 );
	return( write_raw_gz( image, filename, offset, to_little_endian, v, callback ) );
}


/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像の幅
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_raw( array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::size_type w, typename array< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}


/// @brief MISTコンテナ内の画像を 無圧縮RAW 画像として出力する
//! 
//! @param[in] image            … 画像を読み込む先のMISTコンテナ
//! @param[in] filename         … 入力ファイル名
//! @param[in] offset           … 画像から引き算するオフセット値
//! @param[in] to_little_endian … 出力画像のデータ形式（リトルエンディアン … true，ビッグエンディアン … false）
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool write_raw( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}


/// @brief MISTコンテナ内の画像を GZ圧縮RAW 画像として出力する
//! 
//! @param[in] image            … 画像を読み込む先のMISTコンテナ
//! @param[in] filename         … 入力ファイル名
//! @param[in] offset           … 画像から引き算するオフセット値
//! @param[in] to_little_endian … 出力画像のデータ形式（リトルエンディアン … true，ビッグエンディアン … false）
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool write_raw_gz( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset = array< T, Allocator >::value_type( 0 ), bool to_little_endian = false )
{
	return( write_raw_gz( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}






/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像の幅
//! @param[in]  x                  … 入力画像の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  __dmy__            … 読み込もうとしているデータの型をあらわすダミーオブジェクト（short型のオブジェクトなど）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x, typename array1< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	image.reso1( x );
	return( __raw_controller__::raw_controller::read( image, filename, w, 1, 1, offset, from_little_endian, 0, callback, __dmy__ ) );
}


/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像の幅
//! @param[in]  x                  … 入力画像の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x, typename array1< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array1< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, x, offset, from_little_endian, v, callback ) );
}


/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像の幅
//! @param[in]  x                  … 入力画像の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x = 1.0, typename array1< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, x, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}




/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像のX軸方向のサイズ
//! @param[in]  h                  … 入力画像のY軸方向のサイズ
//! @param[in]  x                  … 入力画像のX軸方向の画素サイズ
//! @param[in]  y                  … 入力画像のY軸方向の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  __dmy__            … 読み込もうとしているデータの型をあらわすダミーオブジェクト（short型のオブジェクトなど）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x, double y, typename array2< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	image.reso1( x );
	image.reso2( y );
	return( __raw_controller__::raw_controller::read( image, filename, w, h, 1, offset, from_little_endian, 0, callback, __dmy__ ) );
}


/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像のX軸方向のサイズ
//! @param[in]  h                  … 入力画像のY軸方向のサイズ
//! @param[in]  x                  … 入力画像のX軸方向の画素サイズ
//! @param[in]  y                  … 入力画像のY軸方向の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x, double y, typename array2< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array2< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, h, x, y, offset, from_little_endian, 0, v, callback ) );
}



/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像のX軸方向のサイズ
//! @param[in]  h                  … 入力画像のY軸方向のサイズ
//! @param[in]  x                  … 入力画像のX軸方向の画素サイズ
//! @param[in]  y                  … 入力画像のY軸方向の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x = 1.0, double y = 1.0, typename array2< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, h, x, y, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}




/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像のX軸方向のサイズ
//! @param[in]  h                  … 入力画像のY軸方向のサイズ
//! @param[in]  d                  … 入力画像のZ軸方向のサイズ
//! @param[in]  x                  … 入力画像のX軸方向の画素サイズ
//! @param[in]  y                  … 入力画像のY軸方向の画素サイズ
//! @param[in]  z                  … 入力画像のZ軸方向の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  __dmy__            … 読み込もうとしているデータの型をあらわすダミーオブジェクト（short型のオブジェクトなど）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x, double y, double z, typename array3< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	image.reso1( x );
	image.reso2( y );
	image.reso3( z );
	return( __raw_controller__::raw_controller::read( image, filename, w, h, d, offset, from_little_endian, 0, callback, __dmy__ ) );
}




/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像のX軸方向のサイズ
//! @param[in]  h                  … 入力画像のY軸方向のサイズ
//! @param[in]  d                  … 入力画像のZ軸方向のサイズ
//! @param[in]  x                  … 入力画像のX軸方向の画素サイズ
//! @param[in]  y                  … 入力画像のY軸方向の画素サイズ
//! @param[in]  z                  … 入力画像のZ軸方向の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//! @param[in]  callback           … 進行状況を通知するコールバック関数
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x, double y, double z, typename array3< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array3< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, h, d, x, y, z, offset, from_little_endian, v, callback ) );
}



/// @brief 無圧縮RAW，GZ圧縮RAW 画像をMISTコンテナに読み込む
//! 
//! @param[out] image              … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename           … 入力ファイル名
//! @param[in]  w                  … 入力画像のX軸方向のサイズ
//! @param[in]  h                  … 入力画像のY軸方向のサイズ
//! @param[in]  d                  … 入力画像のZ軸方向のサイズ
//! @param[in]  x                  … 入力画像のX軸方向の画素サイズ
//! @param[in]  y                  … 入力画像のY軸方向の画素サイズ
//! @param[in]  z                  … 入力画像のZ軸方向の画素サイズ
//! @param[in]  offset             … 画像に足しこむオフセット値
//! @param[in]  from_little_endian … 入力画像が記録されている形式（リトルエンディアン … true，ビッグエンディアン … false）
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x = 1.0, double y = 1.0, double z = 1.0, typename array3< T, Allocator >::value_type offset = typename array3< T, Allocator >::value_type( 0 ), bool from_little_endian = false )
{
	return( read_raw( image, filename, w, h, d, x, y, z, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}




/// @}
//  RAW画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_RAW__
