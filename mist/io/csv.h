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

/// @file mist/io/csv.h
//!
//! @brief CSVファイルを読み込むためのライブラリ
//!
#ifndef __INCLUDE_MIST_CSV__
#define __INCLUDE_MIST_CSV__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "../matrix.h"
#endif


#include <iostream>
#include <string>
#include <vector>

#if defined( _COMPRESSED_CSV_SUPPORT_ ) && _COMPRESSED_CSV_SUPPORT_ != 0
	#include <zlib.h>
#endif


// mist名前空間の始まり
_MIST_BEGIN


namespace __csv_controller__
{
	template < class T >
	struct csv_data_converter
	{
		typedef T value_type;

		static value_type convert_to( const unsigned char *s, const unsigned char *e )
		{
			return( atoi( std::string( s, e ).c_str( ) ) );
		}

		static std::string convert_from( const value_type &val )
		{
			static char buff[ 256 ];
			sprintf( buff, "%d", val );
			return( buff );
		}
	};

	template < >
	struct csv_data_converter< float >
	{
		typedef float value_type;

		static value_type convert_to( const unsigned char *s, const unsigned char *e )
		{
			return( ( float )atof( std::string( s, e ).c_str( ) ) );
		}

		static std::string convert_from( const value_type &val )
		{
			static char buff[ 256 ];
			sprintf( buff, "%f", val );
			return( buff );
		}
	};

	template < >
	struct csv_data_converter< double >
	{
		typedef double value_type;

		static value_type convert_to( const unsigned char *s, const unsigned char *e )
		{
			return( atof( std::string( s, e ).c_str( ) ) );
		}

		static std::string convert_from( const value_type &val )
		{
			static char buff[ 256 ];
			sprintf( buff, "%f", val );
			return( buff );
		}
	};

	template < >
	struct csv_data_converter< std::string >
	{
		typedef std::string value_type;

		static value_type convert_to( const unsigned char *s, const unsigned char *e )
		{
			return( std::string( s, e ).c_str( ) );
		}

		static std::string convert_from( const value_type &val )
		{
			return( val );
		}
	};

	template < class Array >
	struct csv_controller
	{
		typedef typename Array::value_type			element_type;
		typedef typename element_type::value_type	value_type;
		typedef typename Array::size_type			size_type;
		typedef csv_data_converter< value_type >	converter;

		static const unsigned char *get_line( const unsigned char *s, const unsigned char *e, bool &is_empty_line )
		{
			const unsigned char *sp = s;
			const unsigned char *ep = s;
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

				ep++;
				s++;
			}

			// 空行はスキップする
			if( s < e && sp == ep )
			{
				return( get_line( s, e, is_empty_line ) );
			}
			else
			{
				is_empty_line = sp == ep;
				return( s > e ? e : s );
			}
		}

		static const unsigned char *get_value( const unsigned char *s, const unsigned char *e, value_type &val, const std::string &separator )
		{
			// 先頭の空白（改行やタブを含む）を飛ばす
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
				}
				else if( s[ 0 ] == '\n' )
				{
					s++;
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

			const unsigned char *sp = s;
			const unsigned char *ep = sp;

			// 次にコンマが来る前まで進める
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
				else
				{
					bool isFound = false;
					for( size_type i = 0 ; i < separator.size( ) ; i++ )
					{
						if( s[ 0 ] == separator[ i ] )
						{
							isFound = true;
							break;
						}
					}

					if( isFound )
					{
						s++;
						break;
					}
				}

				ep++;
				s++;
			}

			if( sp < ep )
			{
				val = converter::convert_to( sp, ep );
			}

			return( s > e ? e : s );
		}

		static bool convert_from_csv_data( Array &csv, const unsigned char *buff, size_type len, const std::string &separator )
		{
			const unsigned char *p = buff;
			const unsigned char *e = buff + len;

			while( p < e )
			{
				bool is_empty_line = false;
				const unsigned char *np = get_line( p, e, is_empty_line );

				if( is_empty_line )
				{
					p = np;
					continue;
				}

				element_type element;
				while( p < np )
				{
					value_type val;
					p = get_value( p, np, val, separator );
					element.push_back( val );
				}

				p = np;

				if( csv.size( ) == 0 )
				{
					csv.push_back( element );
				}
				else if( csv[ csv.size( ) - 1 ].size( ) == element.size( ) )
				{
					csv.push_back( element );
				}
				else
				{
					return( false );
				}
			}

			return( true );
		}

		static bool read( Array &csv, const std::string &filename, const std::string &separator )
		{
#if defined( _COMPRESSED_CSV_SUPPORT_ ) && _COMPRESSED_CSV_SUPPORT_ != 0
			gzFile fp;
			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				return( false );
			}
#else
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				return( false );
			}
#endif

			size_type numBytes = 4096;
			unsigned char *buff = new unsigned char[ numBytes ];
			unsigned char *sp = buff;
			ptrdiff_t read_size = 0;

			bool ret = true;
#if defined( _COMPRESSED_CSV_SUPPORT_ ) && _COMPRESSED_CSV_SUPPORT_ != 0
			while( gzeof( fp ) == 0 )
#else
			while( feof( fp ) == 0 )
#endif
			{
				ptrdiff_t restBytes = ( buff + numBytes ) - sp;
#if defined( _COMPRESSED_CSV_SUPPORT_ ) && _COMPRESSED_CSV_SUPPORT_ != 0
				read_size = gzread( fp, ( void * )sp, static_cast< unsigned int >( sizeof( unsigned char ) * restBytes ) );
#else
				read_size = fread( ( void * )sp, sizeof( unsigned char ), static_cast< unsigned int >( restBytes ), fp );
#endif

				unsigned char *eep = sp + read_size;
				if( eep < buff + numBytes )
				{
					ret = convert_from_csv_data( csv, buff, sp + read_size - buff, separator );
					break;
				}

				unsigned char *e = sp;
				unsigned char *ep = eep + 1;
				for( ; e < eep ; e++ )
				{
					if( e[ 0 ] == '\r' )
					{
						if( e < e && e[ 1 ] == '\n' )
						{
							e += 2;
							ep = e;
						}
						else
						{
							e++;
							ep = e;
						}
					}
					else if( e[ 0 ] == '\n' )
					{
						e++;
						ep = e;
					}
					else
					{
						e++;
					}
				}

				if( ep > eep )
				{
					// 一行分のデータを読み込めなかったのでテンポラリ領域を拡張する
					unsigned char *tmp = new unsigned char[ numBytes * 2 ];
					memcpy( tmp, buff, sizeof( unsigned char ) * numBytes );
					delete [] buff;
					buff = tmp;
					sp = buff + numBytes;
					numBytes *= 2;
				}
				else
				{
					if( !convert_from_csv_data( csv, buff, ep - buff, separator ) )
					{
						ret = false;
						break;
					}

					unsigned char *s1 = buff;
					unsigned char *s2 = ep;
					while( s2 < eep )
					{
						*s1++ = *s2++;
					}

					sp = buff + ( eep - ep );
				}
			}

#if defined( _COMPRESSED_CSV_SUPPORT_ ) && _COMPRESSED_CSV_SUPPORT_ != 0
			gzclose( fp );
#else
			fclose( fp );
#endif

			delete [] buff;
			return( ret );
		}
	};
}


//! @addtogroup csv_data_group CSVデータ入出力
//!
//! CSV形式のファイルを読み込んで，MISTコンテナもしくはSTLのコンテナへ格納する．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/csv.h>
//! @endcode
//!
//!
//!  @{


/// @brief CSV形式のファイルをSTLコンテナに読み込む
//! 
//! データの区切りとして，コンマもしくは半角空白をデフォルトで識別するようになっている．
//! データの区切りを変更する場合は，separator 引数を変更する．
//! 
//! @param[out] csv       … CSV形式のファイルを読み込む先のSTLコンテナ
//! @param[in]  filename  … 入力ファイル名
//! @param[in]  separator … データの区切り記号
//!
//! @retval true  … CSV形式データの読み込みに成功
//! @retval false … CSV形式データの読み込みに失敗
//! 
template < class Array >
bool read_csv( Array &csv, const std::string &filename, const std::string &separator =", " )
{
	// データをクリアする
	csv.clear( );
	return( __csv_controller__::csv_controller< Array >::read( csv, filename, separator ) );
}


/// @brief CSV形式のファイルをSTLコンテナに読み込む
//! 
//! データの区切りとして，コンマもしくは半角空白をデフォルトで識別するようになっている．
//! データの区切りを変更する場合は，separator 引数を変更する．
//! 
//! @param[out] csv       … CSV形式のファイルを読み込む先のSTLコンテナ
//! @param[in]  filename  … 入力ファイル名
//! @param[in]  separator … データの区切り記号
//!
//! @retval true  … CSV形式データの読み込みに成功
//! @retval false … CSV形式データの読み込みに失敗
//! 
template < class Array >
bool read_csv( Array &csv, const std::wstring &filename, const std::wstring &separator =", " )
{
	return( read_csv( csv, wstr2str( filename  ), separator ) );
}


/// @brief CSV形式のファイルをMISTコンテナ（mist::matrix）に読み込む
//! 
//! データの区切りとして，コンマもしくは半角空白をデフォルトで識別するようになっている．
//! データの区切りを変更する場合は，separator 引数を変更する．
//! 
//! @param[out] csv       … CSV形式のファイルを読み込む先の行列
//! @param[in]  filename  … 入力ファイル名
//! @param[in]  separator … データの区切り記号
//!
//! @retval true  … CSV形式データの読み込みに成功
//! @retval false … CSV形式データの読み込みに失敗
//! 
template < class T, class Allocator >
bool read_csv( matrix< T, Allocator > &csv, const std::string &filename, const std::string &separator =", " )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef std::vector< std::vector< T > > csv_data_type;

	csv_data_type csv_data;
	if( read_csv( csv_data, filename, separator ) && csv_data.size( ) > 0 && csv_data[ 0 ].size( ) > 0 )
	{
		csv.resize( csv_data.size( ), csv_data[ 0 ].size( ) );

		for( size_type r = 0 ; r < csv_data.size( ) ; r++ )
		{
			typename csv_data_type::value_type &data = csv_data[ r ];

			for( size_type c = 0 ; c < data.size( ) ; c++ )
			{
				csv( r, c ) = data[ c ];
			}
		}

		return( true );
	}

	return( false );
}

/// @brief CSV形式のファイルをMISTコンテナ（mist::matrix）に読み込む
//! 
//! データの区切りとして，コンマもしくは半角空白をデフォルトで識別するようになっている．
//! データの区切りを変更する場合は，separator 引数を変更する．
//! 
//! @param[out] csv       … CSV形式のファイルを読み込む先の行列
//! @param[in]  filename  … 入力ファイル名
//! @param[in]  separator … データの区切り記号
//!
//! @retval true  … CSV形式データの読み込みに成功
//! @retval false … CSV形式データの読み込みに失敗
//! 
template < class T, class Allocator >
bool read_csv( matrix< T, Allocator > &csv, const std::wstring &filename, const std::string &separator =", " )
{
	return( read_csv( csv, wstr2str( filename ), separator ) );
}

/// @}
//  CSVデータ入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_CSV__
