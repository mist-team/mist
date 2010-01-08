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

/// @file mist/hash_algorithm.h
//!
//! @brief 任意のバイト列のハッシュ関数値を計算する
//!
//! - 参考文献
//!   - MD2, RFC1319, http://www.ietf.org/rfc/rfc1319.txt
//!   - MD4, RFC1320, http://www.ietf.org/rfc/rfc1320.txt
//!   - MD5, RFC1321, http://www.ietf.org/rfc/rfc1321.txt
//!   - SHA-1, RFC3174, http://www.ietf.org/rfc/rfc3174.txt
//!   - SHA-256, 384, 512, "Descriptions of SHA-256, SHA-384, and SHA-512." http://csrc.nist.gov/cryptval/shs/sha256-384-512.pdf 
//!
#ifndef __INCLUDE_HASH_ALGORITHM__
#define __INCLUDE_HASH_ALGORITHM__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <string>


// mist名前空間の始まり
_MIST_BEGIN


//! @defgroup hash_group ハッシュ関数
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/hash_algorithm.h>
//! @endcode
//!
//!  @{


/// @brief ハッシュ関数を計算する基底クラス
//!
//! MD2，MD4，MD5，SHA1，SHA-256，SHA-384，SHA-512 を計算するための基底クラス
//!
//! @attention SHA-384，SHA-512 を計算する際に 64 ビット整数を利用するため，コンパイラが 64 ビットをサポートする必要あり．
//!
//! @code ハッシュ関数の計算例
//! // ハッシュ関数を SHA1 で初期化する
//! mist::hash_algorithm &h = mist::sha1( "a" );
//! 
//! // ハッシュ関数値のダイジェスト文字列を出力する
//! std::cout << h << std::endl;
//! 
//! // 別の文字列で再計算する
//! h.compute_hash( "あいうえお" );
//! 
//! // ハッシュ関数値のダイジェスト文字列を出力する
//! std::cout << h << std::endl;
//! 
//! @endcode
//!
class hash_algorithm
{
public:
	typedef size_t					size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t				difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef unsigned char			uint8;				///< @brief 符号なし8ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned char 型と同じ．
	typedef unsigned int			uint32;				///< @brief 符号なし32ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned int 型と同じ．
	typedef unsigned long long int	uint64;				///< @brief 符号なし64ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned long long int 型と同じ．

protected:
	size_type nbytes;
	unsigned char *digest;

public:
	/// @brief data[ 0 ] から data[ len - 1 ] の len バイトのハッシュ関数値を計算する．
	virtual void compute_hash( const void *bytes, uint64 len ) = 0;

	/// @brief 文字列 str のハッシュ関数値を計算する．
	void compute_hash( const std::string &str ){ compute_hash( str.c_str( ), str.length( ) ); }


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const = 0;

	/// @brief ダイジェストバイト列の長さ
	size_type size( ) const { return( nbytes ); }

	/// @brief ダイジェスト文字列の各バイトを取得する
	unsigned char operator []( size_type index ) const { return( digest[ index ] ); }


	/// @brief ダイジェスト文字列を返す
	std::string to_string( ) const
	{
		static char x16[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		std::string str;

		for( size_type i = 0 ; i < size( ) ; i++ )
		{
			str += x16[ ( digest[ i ] >> 4 ) & 0x0f ];
			str += x16[ digest[ i ] & 0x0f ];
		}

		return( str );
	}

protected:
	// ハッシュ関数のコピーは禁止！！
	hash_algorithm &operator =( const hash_algorithm &h );
	hash_algorithm( const hash_algorithm &h );

protected:
	/// @brief 指定されたダイジェストのバイト数で初期化する（派生クラスでのみ利用可能）
	hash_algorithm( size_type num_digest_bytes ) : nbytes( num_digest_bytes ), digest( new unsigned char[ nbytes ] ){ }

	/// @brief 指定されたダイジェスト文字列で初期化する（派生クラスでのみ利用可能）
	hash_algorithm( const std::string &str ) : nbytes( str.length( ) / 2 ), digest( new unsigned char[ nbytes ] )
	{
		static uint8 x16[ 256 ] = {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		for( size_type i = 0, indx = 0 ; i < size( ) ; i++, indx += 2 )
		{
			size_type s1 = static_cast< uint8 >( str[ indx ] );
			size_type s2 = static_cast< uint8 >( str[ indx + 1 ] );
			digest[ i ] = ( ( x16[ s1 ] << 4 ) & 0xf0 ) | ( x16[ s2 ] & 0x0f );
		}
	}

	/// @brief ダイジェストに利用したデータを削除する
	virtual ~hash_algorithm( )
	{
		delete [] digest;
	}


public:
	/// @brief 2つのハッシュ関数値が同一かどうかを判定する
	bool operator ==( const hash_algorithm &h ) const
	{
		if( size( ) != h.size( ) )
		{
			return( false );
		}
		else
		{
			for( size_type i = 0 ; i < size( ) ; i++ )
			{
				if( digest[ i ] != h[ i ] )
				{
					return( false );
				}
			}
			return( true );
		}
	}

	/// @brief 2つのダイジェスト文字列が同一かどうかを判定する
	bool operator ==( const std::string &str ) const
	{
		return( str == to_string( ) );
	}
};


/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     h   … ハッシュ関数オブジェクト
//! 
//! @return 入力されたストリーム
//! 
inline std::ostream &operator <<( std::ostream &out, const hash_algorithm &h )
{
	out << h.to_string( );
	return( out );
}


/// @}
//  ハッシュ関数グループの終わり


// mist名前空間の終わり
_MIST_END


#ifndef __INCLUDE_MD5__
#include "md5.h"
#endif

#ifndef __INCLUDE_SHA__
#include "sha.h"
#endif


#endif // __INCLUDE_HASH_ALGORITHM__
