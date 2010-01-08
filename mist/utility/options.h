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

#ifndef __INCLUDE_MIST_OPTIONS__
#define __INCLUDE_MIST_OPTIONS__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>


// mist名前空間の始まり
_MIST_BEGIN

/// @brief コマンドライン引数を解析するクラス
//! 
class options : public std::vector< std::string >
{
public:
	typedef std::vector< std::string > base;	///< @brief 基底クラス

private:
	/// @brief オプションを記憶するクラス
	struct arg
	{
		std::string name;		///< @brief オプション名
		std::string comment;	///< @brief オプションの説明
		std::string value;		///< @brief オプションの値
		bool has_value;			///< @brief 値と共に指定されるオプションかどうかを示すフラグ
		bool found;				///< @brief オプションに指定されたかどうか

		/// @brief デフォルトコンストラクタ
		arg( const std::string &aname = "", const std::string &text = "", const std::string &val = "", bool hasValue = false )
			: name( aname ), comment( text ), value( val ), has_value( hasValue ), found( false )
		{
		}

		/// @brief コピーコンストラクタ
		arg( const arg &p ) : name( p.name ), comment( p.comment ), value( p.value ), has_value( p.has_value ), found( p.found )
		{
		}
	};

	std::string                  header_text;		///< @brief ヘルプの先頭に表示される文字列
	std::string                  footer_text;		///< @brief ヘルプの末尾に表示される文字列
	std::string                  program_name;		///< @brief プログラム名を保持する文字列
	std::vector< std::string >   option_list;		///< @brief 指定される可能性のあるオプションのリスト
	std::map< std::string, arg > args;				///< @brief 指定される可能性のあるオプションのリスト

public:
	/// @brief デフォルトコンストラクタ
	options( const std::string header = "", const std::string footer = "" ) : header_text( header ), footer_text( footer )
	{
	}

	/// @brief コピーコンストラクタ
	options( const options &o ) : base( o ), program_name( o.program_name ), args( o.args )
	{
	}

protected:
	/// @brief オプションに指定されている値を取得する
	bool __isset__( const std::string &name, std::string &val ) const
	{
		std::map< std::string, arg >::const_iterator ite = args.find( name );
		if( ite != args.end( ) )
		{
			const arg &a = ite->second;
			val = a.value;
			return( true );
		}
		else
		{
			return( false );
		}
	}

public:
	/// @brief 値無しのオプションを追加する
	void add( const std::string &name, const std::string &comment )
	{
		option_list.push_back( name );
		args[ name ] = arg( name, comment, "", false );
	}

	/// @brief 値を含むオプションを追加する（デフォルト値を指定するとオプションが指定されない場合も値を取得可能）
	void add( const std::string &name, const std::string &comment, const std::string &default_value )
	{
		option_list.push_back( name );
		args[ name ] = arg( name, comment, default_value, true );
	}

	/// @brief 値を含むオプションを追加する（デフォルト値を指定するとオプションが指定されない場合も値を取得可能）
	void add( const std::string &name, const std::string &comment, int default_value )
	{
		char buff[ 50 ];
		sprintf( buff, "%d", default_value );
		option_list.push_back( name );
		args[ name ] = arg( name, comment, buff, true );
	}

	/// @brief 値を含むオプションを追加する（デフォルト値を指定するとオプションが指定されない場合も値を取得可能）
	void add( const std::string &name, const std::string &comment, double default_value )
	{
		char buff[ 50 ];
		sprintf( buff, "%lf", default_value );
		option_list.push_back( name );
		args[ name ] = arg( name, comment, buff, true );
	}

	/// @brief オプションがコマンドライン引数無いに指定されたかどうかを調べる
	bool isset( const std::string &name ) const
	{
		std::map< std::string, arg >::const_iterator ite = args.find( name );
		if( ite != args.end( ) )
		{
			const arg &a = ite->second;
			return( a.found );
		}
		else
		{
			return( false );
		}
	}

	/// @brief オプションに指定された文字列を取得する
	const std::string get_string( const std::string &name ) const
	{
		std::string val;
		if( __isset__( name, val ) )
		{
			return( val );
		}
		else
		{
			return( "" );
		}
	}

	/// @brief オプションに指定された値を取得する
	int get_int( const std::string &name ) const
	{
		std::string val;
		if( __isset__( name, val ) )
		{
			return( atoi( val.c_str( ) ) );
		}
		else
		{
			return( 0 );
		}
	}

	/// @brief オプションに指定された値を取得する
	double get_double( const std::string &name ) const
	{
		std::string val;
		if( __isset__( name, val ) )
		{
			return( atof( val.c_str( ) ) );
		}
		else
		{
			return( 0.0 );
		}
	}

public:
	/// @brief オプションの一覧を標準出力に表示する
	void show_help( ) const
	{
		std::stringstream sout;

		// ヘッダ部分を出力する
		sout << header_text << std::endl;

		size_t max_len = 0;
		for( size_t i = 0 ; i < option_list.size( ) ; i++ )
		{
			if( max_len < option_list[ i ].size( ) )
			{
				max_len = option_list[ i ].size( );
			}
		}

		for( size_t i = 0 ; i < option_list.size( ) ; i++ )
		{
			std::map< std::string, arg >::const_iterator ite = args.find( option_list[ i ] );
			if( ite != args.end( ) )
			{
				const arg &a = ite->second;

				sout << "-";
				sout << a.name;

				for( size_t i = a.name.size( ) ; i < max_len + 3 ; i++ )
				{
					sout << ' ';
				}

				sout << a.comment;

				if( a.has_value )
				{
					sout << "[" << a.value << "]";
				}

				if( a.found )
				{
					sout << "*";
				}
				sout << std::endl;
			}
			else
			{
				std::cerr << "Failed to parse options." << std::endl;
			}
		}

		// フッタ部分を出力する
		sout << footer_text << std::endl;

		puts( sout.str( ).c_str( ) );
		//std::cout << sout.str( ) << std::endl;
	}

	/// @brief プログラム引数を解析して，オプションとの対応関係を求める
	bool parse_args( int argc, char *argv[] )
	{
		program_name = argv[ 0 ];

		bool ret = true;

		for( int i = 1 ; i < argc ; i++ )
		{
			std::string option = argv[ i ];

			if( option[ 0 ] != '-' )
			{
				base::push_back( option );
			}
			else
			{
				option = option.substr( 1 );

				std::map< std::string, arg >::iterator ite = args.find( option );
				if( ite != args.end( ) )
				{
					arg &a = ite->second;
					a.found = true;

					if( a.has_value )
					{
						if( i + 1 < argc && argv[ i + 1 ][ 0 ] != '-' )
						{
							a.value = argv[ i + 1 ];
							i++;
						}
						else if( i + 1 < argc && strlen( argv[ i + 1 ] ) > 1 && argv[ i + 1 ][ 0 ] == '-' && '0' <= argv[ i + 1 ][ 1 ] && argv[ i + 1 ][ 1 ] <= '9' )
						{
							a.value = argv[ i + 1 ];
							i++;
						}
						else
						{
							std::cerr << "Option \"" << option << "\" requires values." << std::endl;
							ret = false;
						}
					}
				}
				else
				{
					std::cerr << "\"" << option << "\" is an unknown option." << std::endl;
					ret = false;
				}
			}
		}

		if( !ret )
		{
			std::cerr << std::endl;
		}

		return( ret );
	}
};

// mist名前空間の終わり
_MIST_END


#endif	// __INCLUDE_MIST_OPTIONS__
