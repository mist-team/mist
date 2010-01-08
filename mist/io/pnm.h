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

/// @file mist/io/pnm.h
//!
//! @brief PBM，PGM，PPM，PNM画像を読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_PNM__
#define __INCLUDE_MIST_PNM__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif


#include <iostream>
#include <string>
#include <vector>


// mist名前空間の始まり
_MIST_BEGIN


namespace __pnm_controller__
{
	enum PNM_TYPE
	{
		UNKNOWN = 0,
		P1,
		P2,
		P3,
		P4,
		P5,
		P6
	};

	template < bool b >
	struct _gray_converter_
	{
		template < class T >
		static T get_value( const T &v ){ return( v ); }
	};

	template < >
	struct _gray_converter_< true >
	{
		template < class T >
		static typename T::value_type get_value( const T &v ){ return( v.get_value( ) ); }
	};

	template < class T, class Allocator >
	struct pnm_controller
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef _pixel_converter_< T > pixel_converter;
		typedef _gray_converter_< is_color< T >::value > gray_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename pixel_converter::value_type value_type;

		static PNM_TYPE pnm_format( const std::string &str )
		{
			if( str == "P1" )
			{
				return( P1 );
			}
			else if( str == "P2" )
			{
				return( P2 );
			}
			else if( str == "P3" )
			{
				return( P3 );
			}
			else if( str == "P4" )
			{
				return( P4 );
			}
			else if( str == "P5" )
			{
				return( P5 );
			}
			else if( str == "P6" )
			{
				return( P6 );
			}
			else
			{
				return( UNKNOWN );
			}
		}

		static const unsigned char *get_line( const unsigned char *s, const unsigned char *e, std::string &line )
		{
			line = "";
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s++;
					}
					break;
				}
				else if( s[ 0 ] == '\n' )
				{
					s = s + 1;
					break;
				}
				line += *s;
				s++;
			}
			return( s > e ? e : s );
		}

		static const unsigned char *get_value( const unsigned char *s, const unsigned char *e, std::string &line, bool &flag )
		{
			line = "";
			// 先頭の空白（改行やタブを含む）を飛ばす
			while( s < e )
			{
				if( flag && s[ 0 ] == '#' )
				{
					// コメント行なのですっ飛ばす
					while( s < e )
					{
						if( s[ 0 ] == '\r' )
						{
							if( s + 1 != e && s[ 1 ] == '\n' )
							{
								s = s + 2;
							}
							else
							{
								s++;
							}
							break;
						}
						else if( s[ 0 ] == '\n' )
						{
							s++;
							break;
						}
						s++;
					}
				}
				else if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s = s + 1;
					}
					flag = true;
				}
				else if( s[ 0 ] == '\n' )
				{
					s++;
					flag = true;
				}
				else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
				{
					s++;
				}
				else
				{
					break;
				}
			}
			// 次に空白が来る前まで進める
			flag = false;
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s++;
					}
					flag = true;
					break;
				}
				else if( s[ 0 ] == '\n' )
				{
					s++;
					flag = true;
					break;
				}
				else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
				{
					s++;
					break;
				}
				line += *s;
				s++;
			}
			return( s > e ? e : s );
		}

		static size_type split_string( const std::string &line, const char ch, std::vector< std::string > &elements )
		{
			std::string str = "";
			size_type i = 0;
			elements.clear( );
			while( i < line.size( ) )
			{
				for( ; i < line.size( ) && line[ i ] == ch ; i++ ){}

				str = "";
				for( ; i < line.size( ) && line[ i ] != ch ; i++ )
				{
					str += line[ i ];
				}

				if( str != "" )
				{
					elements.push_back( str );
				}
			}
			return( elements.size( ) );
		}

		static bool is_number( const std::string &line )
		{
			for( size_t i = 0 ; i < line.size( ) ; i++ )
			{
				if( line[ i ] < '0' || '9' < line[ i ] )
				{
					return( false );
				}
			}
			return( true );
		}

		static bool convert_from_pnm_data( array2< T, Allocator > &image, const unsigned char *buff, size_type len, typename array2< T, Allocator >::size_type level )
		{
			// PNM形式のヘッダ部分
			const unsigned char *p = buff;
			const unsigned char *e = buff + len;
			std::string line = "";
			std::vector< std::string > elements;

			bool flag = true;
			p = get_value( p, e, line, flag );
			PNM_TYPE pnm_type = pnm_format( line );
			if( pnm_type == UNKNOWN )
			{
				// 未サポートのビットマップ
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			// 画像サイズを取得する
			size_type w = 0, h = 0, gray_level = level;
			p = get_value( p, e, line, flag );
			if( is_number( line ) )
			{
				w = atoi( line.c_str( ) );
			}
			else
			{
				// 未サポートのビットマップ
				std::cerr << "Image size is unknown!" << std::endl;
			}

			p = get_value( p, e, line, flag );
			if( is_number( line ) )
			{
				h = atoi( line.c_str( ) );
			}
			else
			{
				// 未サポートのビットマップ
				std::cerr << "Image size is unknown!" << std::endl;
			}

			image.resize( w, h );

			// 濃淡のレベル数を取得する
			switch( pnm_type )
			{
			case P2:
			case P3:
			case P5:
			case P6:
				p = get_value( p, e, line, flag );
				if( !is_number( line ) )
				{
					// 未サポートのビットマップ
					std::cerr << "Image size is unknown!" << std::endl;
					return( false );
				}
				gray_level = atoi( line.c_str( ) );
				break;

			default:
				break;
			}

			// 画素データを取得する
			size_type i = 0;
			double scale = static_cast< double >( level ) / static_cast< double >( gray_level );
			switch( pnm_type )
			{
			case P1:
			case P2:
				while( i < image.size( ) && p < e )
				{
					p = get_line( p, e, line );
					split_string( line, ' ', elements );
					for( size_type j = 0 ; i < image.size( ) && j < elements.size( ) ; j++ )
					{
						image[ i++ ] = static_cast< value_type >( atoi( elements[ j ].c_str( ) ) * scale );
					}
				}
				break;

			case P3:
				while( i < image.size( ) && p < e )
				{
					p = get_line( p, e, line );
					split_string( line, ' ', elements );
					for( size_type j = 0 ; i < image.size( ) && j < elements.size( ) ; j += 3 )
					{
						value_type r = static_cast< value_type >( atoi( elements[ j + 0 ].c_str( ) ) * scale );
						value_type g = static_cast< value_type >( atoi( elements[ j + 1 ].c_str( ) ) * scale );
						value_type b = static_cast< value_type >( atoi( elements[ j + 2 ].c_str( ) ) * scale );
						image[ i++ ] = pixel_converter::convert_to( r, g, b );
					}
				}
				break;

			case P4:
			case P5:
				while( i < image.size( ) && p < e )
				{
					image[ i++ ] = static_cast< value_type >( *p++ * scale );
				}
				break;

			case P6:
				while( i < image.size( ) && p < e )
				{
					value_type r = static_cast< value_type >( p[ 0 ] * scale );
					value_type g = static_cast< value_type >( p[ 1 ] * scale );
					value_type b = static_cast< value_type >( p[ 2 ] * scale );
					image[ i++ ] = pixel_converter::convert_to( r, g, b );
					p += 3;
				}
				break;

			default:
				return( false );
			}

			return( p == e );
		}

		static bool read( array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type level )
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

			bool ret = convert_from_pnm_data( image, buff, filesize, level );
			delete [] buff;
			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, PNM_TYPE pnm_type, typename array2< T, Allocator >::size_type level )
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
			else if( pnm_type == UNKNOWN )
			{
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				return( false );
			}

			// ヘッダ情報を書き込む
			fprintf( fp, "P%1d\n", pnm_type );
			fprintf( fp, "# Created by MIST\n" );
			fprintf( fp, "%d %d\n", static_cast< int >( image.width( ) ), static_cast< int >( image.height( ) ) );

			size_type i, j;

			switch( pnm_type )
			{
			case P2:
			case P3:
			case P5:
			case P6:
				{
					typename array2< T, Allocator >::value_type max = image[ 0 ];
					for( i = 1 ; i < image.size( ) ; i++ )
					{
						max = max > image[ i ] ? max : image[ i ];
					}
					int max_level = static_cast< int >( gray_converter::get_value( max ) );
					max_level = max_level > static_cast< int >( level ) ? max_level : static_cast< int >( level );
					fprintf( fp, "%d\n", max_level );
				}
				break;

			default:
				break;
			}


			// 画素データを書き出し
			switch( pnm_type )
			{
			case P1:
			case P2:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						fprintf( fp, "%d ", static_cast< int >( gray_converter::get_value( image( i, j ) ) ) );
					}
					fputc( '\n', fp );
				}
				break;

			case P3:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						color_type c = pixel_converter::convert_from( image( i, j ) );
						fprintf( fp, "%d ", static_cast< int >( c.r ) );
						fprintf( fp, "%d ", static_cast< int >( c.g ) );
						fprintf( fp, "%d ", static_cast< int >( c.b ) );
					}
					fputc( '\n', fp );
				}
				break;

			case P4:
			case P5:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						fprintf( fp, "%c", static_cast< unsigned char >( gray_converter::get_value( image( i, j ) ) ) );
					}
				}
				break;

			case P6:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						color_type c = pixel_converter::convert_from( image( i, j ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.r ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.g ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.b ) );
					}
				}
				break;

			default:
				fclose( fp );
				return( false );
			}

			fclose( fp );

			return( true );
		}
	};
}


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_pnm_group PBM，PGM，PPM，PNM 画像入出力
//!
//! PNM（PBM，PGM，PPM）形式のファイルを読み込んで，MISTコンテナへ格納する．
//! また，MISTコンテナからPNMファイルへの書き出しをサポート．
//! P1，P2，P3，P4，P5，P6形式のPNMに対応．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/pnm.h>
//! @endcode
//!
//!
//!  @{


/// @brief PNM画像をMISTコンテナに読み込む
//! 
//! P1～P6までの全てのPNM画像を読み込むことが可能です
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//! @param[in]  level    … 読み込んだPNMファイルをMISTコンテナに格納する際の量子化レベル
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_pnm( array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type level = 255 )
{
	return( __pnm_controller__::pnm_controller< T, Allocator >::read( image, filename, level ) );
}


/// @brief PNM画像をMISTコンテナに読み込む
//! 
//! P1～P6までの全てのPNM画像を読み込むことが可能です
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//! @param[in]  level    … 読み込んだPNMファイルをMISTコンテナに格納する際の量子化レベル
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_pnm( array2< T, Allocator > &image, const std::wstring &filename, typename array2< T, Allocator >::size_type level = 255 )
{
	return( read_pnm( image, wstr2str( filename ), level ) );
}


/// @brief MISTコンテナの画像をPNM形式でファイルに出力する
//! 
//! @attention 出力する画像形式によって，必要なマジックナンバーを選択する必要あり
//! @attention デフォルトのマジックナンバーはP6のカラー画像で，量子化レベルは256階調
//! 
//! - PNM画像のマジックナンバー
//!   -# bitmap (ASCII) 2値 
//!   -# graymap (ASCII) グレースケール 
//!   -# pixmap (ASCII) カラー 
//!   -# bitmap (Binary) 2値 
//!   -# graymap (Binary) グレースケール 
//!   -# pixmap (Binary) カラー 
//!
//! @param[in] image    … 出力画像を保持するMISTコンテナ
//! @param[in] filename … 出力ファイル名
//! @param[in] pnm_type … PNM画像のマジックナンバーのうちの1つ
//! @param[in] level    … 出力するPNMファイルの量子化ビット数
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_pnm( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type pnm_type = 6, typename array2< T, Allocator >::size_type level = 255 )
{
	if( pnm_type > 6 ) 
	{
		std::cerr << "This format is not supported currently!" << std::endl;
		return( false );
	}
	return( __pnm_controller__::pnm_controller< T, Allocator >::write( image, filename, static_cast< __pnm_controller__::PNM_TYPE >( pnm_type ), level ) );
}


/// @brief MISTコンテナの画像をPNM形式でファイルに出力する
//! 
//! @attention 出力する画像形式によって，必要なマジックナンバーを選択する必要あり
//! @attention デフォルトのマジックナンバーはP6のカラー画像で，量子化レベルは256階調
//! 
//! - PNM画像のマジックナンバー
//!   -# bitmap (ASCII) 2値 
//!   -# graymap (ASCII) グレースケール 
//!   -# pixmap (ASCII) カラー 
//!   -# bitmap (Binary) 2値 
//!   -# graymap (Binary) グレースケール 
//!   -# pixmap (Binary) カラー 
//!
//! @param[in] image    … 出力画像を保持するMISTコンテナ
//! @param[in] filename … 出力ファイル名
//! @param[in] pnm_type … PNM画像のマジックナンバーのうちの1つ
//! @param[in] level    … 出力するPNMファイルの量子化ビット数
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_pnm( const array2< T, Allocator > &image, const std::wstring &filename, typename array2< T, Allocator >::size_type pnm_type = 6, typename array2< T, Allocator >::size_type level = 255 )
{
	return( write_pnm( image, wstr2str( filename ), static_cast< __pnm_controller__::PNM_TYPE >( pnm_type ), level ) );
}


/// @}
//  PNM画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_PNM__
