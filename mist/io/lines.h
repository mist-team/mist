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

/// @file mist/io/lines.h
//!
//! @brief ファイルから改行で区切られた文字列のリストを読み込むためのライブラリ
//!
#ifndef __INCLUDE_MIST_LINES__
#define __INCLUDE_MIST_LINES__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#include <iostream>
#include <string>
#include <vector>

#include <zlib.h>


// mist名前空間の始まり
_MIST_BEGIN


namespace __lines_controller__
{
	struct lines_controller
	{
		static const unsigned char *get_line( const unsigned char *s, const unsigned char *e, std::string &line, bool &__has_eol__ )
		{
			__has_eol__ = false;
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						__has_eol__ = true;
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
					__has_eol__ = true;
					s = s + 1;
					break;
				}

				line += *s;
				s++;
			}

			return( s > e ? e : s );
		}

		template < template < typename T, typename A > class Array, class Allocator >
		static bool read( Array< std::string, Allocator > &lines, const std::string &filename )
		{
			gzFile fp;
			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				return( false );
			}

			std::string line = "";
			size_t numBytes = 4096;
			unsigned char *buff = new unsigned char[ numBytes ];

			while( gzeof( fp ) == 0 )
			{
				ptrdiff_t read_size = gzread( fp, ( void * )buff, static_cast< unsigned int >( sizeof( unsigned char ) * numBytes ) );

				const unsigned char *sp = buff;
				const unsigned char *ep = sp + read_size;

				while( sp < ep )
				{
					bool __has_eol__ = false;
					sp = get_line( sp, ep, line, __has_eol__ );

					if( !line.empty( ) && __has_eol__ )
					{
						lines.push_back( line );
						line.resize( 0 );
					}
				}
			}

			if( !line.empty( ) )
			{
				lines.push_back( line );
			}

			gzclose( fp );

			delete [] buff;

			return( true );
		}
	};
}


//! @addtogroup lines_data_group 改行区切りのデータの入出力
//!
//! 改行区切りのファイルを読み込んで，MISTコンテナもしくはSTLのコンテナへ格納する．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/lines.h>
//! @endcode
//!
//!
//!  @{


/// @brief 改行区切りのファイルをSTLコンテナに読み込む
//! 
//! データの区切りとして，コンマもしくは半角空白をデフォルトで識別するようになっている．
//! データの区切りを変更する場合は，separator 引数を変更する．
//! 
//! @param[out] lines     … 改行区切りのファイルを読み込む先のSTLコンテナ
//! @param[in]  filename  … 入力ファイル名
//!
//! @retval true  … データの読み込みに成功
//! @retval false … データの読み込みに失敗
//! 
template < template < typename T, typename A > class Array, class Allocator >
bool read_lines( Array< std::string, Allocator > &lines, const std::string &filename )
{
	// データをクリアする
	lines.clear( );
	return( __lines_controller__::lines_controller::read( lines, filename ) );
}


/// @brief 改行区切りのファイルをSTLコンテナに読み込む
//! 
//! データの区切りとして，コンマもしくは半角空白をデフォルトで識別するようになっている．
//! データの区切りを変更する場合は，separator 引数を変更する．
//! 
//! @param[out] lines     … 改行区切りのファイルを読み込む先のSTLコンテナ
//! @param[in]  filename  … 入力ファイル名
//!
//! @retval true  … データの読み込みに成功
//! @retval false … データの読み込みに失敗
//! 
template < template < typename T, typename A > class Array, class Allocator >
bool read_lines( Array< std::string, Allocator > &lines, const std::wstring &filename )
{
	return( read_lines( lines, wstr2str( filename ) ) );
}

/// @}
//  改行区切りのデータの入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_LINES__
