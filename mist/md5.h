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

/// @file mist/md5.h
//!
//! @brief 任意のバイト列のMD5を計算する
//!
//! - 参考文献
//!   - MD2, RFC1319, http://www.ietf.org/rfc/rfc1319.txt
//!   - MD4, RFC1320, http://www.ietf.org/rfc/rfc1320.txt
//!   - MD5, RFC1321, http://www.ietf.org/rfc/rfc1321.txt
//!
#ifndef __INCLUDE_MD5__
#define __INCLUDE_MD5__

#include <cstring>

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "config/endian.h"
#endif

#ifndef __INCLUDE_HASH_ALGORITHM__
#include "hash_algorithm.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


//! @addtogroup hash_group ハッシュ関数
//!  @{


/// @brief MD2 を生成するクラス
class md2 : public hash_algorithm
{
private:
	typedef hash_algorithm  base;

public:
	typedef base::size_type			size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef base::difference_type	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef base::uint8				uint8;				///< @brief 符号なし8ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned char 型と同じ．
	typedef base::uint32			uint32;				///< @brief 符号なし32ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned int 型と同じ．
	typedef base::uint64			uint64;				///< @brief 符号なし64ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned long long int 型と同じ．


public:
	/// @brief bytes[ 0 ] から bytes[ length - 1 ] の length バイトの MD2 を計算する．
	virtual void compute_hash( const void *bytes, uint64 length )
	{
		size_type len = static_cast< size_type >( length );
		length *= 8;

		const uint8 *data = reinterpret_cast< const uint8 * >( bytes );
		size_type R = len % 16;
		size_type i, j, N16 = len / 16;
		uint8 pad = static_cast< uint8 >( 16 - R );
		uint8 P[ 32 ], *C = P + 16;
		uint8 S[ 256 ] = {   41,  46,  67, 201, 162, 216, 124,   1,  61,  54,  84, 161, 236, 240,   6,
							 19,  98, 167,   5, 243, 192, 199, 115, 140, 152, 147,  43, 217, 188,  76,
							130, 202,  30, 155,  87,  60, 253, 212, 224,  22, 103,  66, 111,  24, 138,
							 23, 229,  18, 190,  78, 196, 214, 218, 158, 222,  73, 160, 251, 245, 142,
							187,  47, 238, 122, 169, 104, 121, 145,  21, 178,   7,  63, 148, 194,  16,
							137,  11,  34,  95,  33, 128, 127,  93, 154,  90, 144,  50,  39,  53,  62,
							204, 231, 191, 247, 151,   3, 255,  25,  48, 179,  72, 165, 181, 209, 215,
							 94, 146,  42, 172,  86, 170, 198,  79, 184,  56, 210, 150, 164, 125, 182,
							118, 252, 107, 226, 156, 116,   4, 241,  69, 157, 112,  89, 100, 113, 135,
							 32, 134,  91, 207, 101, 230,  45, 168,   2,  27,  96,  37, 173, 174, 176,
							185, 246,  28,  70,  97, 105,  52,  64, 126,  15,  85,  71, 163,  35, 221,
							 81, 175,  58, 195,  92, 249, 206, 186, 197, 234,  38,  44,  83,  13, 110,
							133,  40, 132,   9, 211, 223, 205, 244,  65, 129,  77,  82, 106, 220,  55,
							200, 108, 193, 171, 250,  36, 225, 123,   8,  12, 189, 177,  74, 120, 136,
							149, 139, 227,  99, 232, 109, 233, 203, 213, 254,  59,   0,  29,  57, 242,
							239, 183,  14, 102,  88, 208, 228, 166, 119, 114, 248, 235, 117,  75,  10,
							 49,  68,  80, 180, 143, 237,  31,  26, 219, 153, 141,  51, 159,  17, 131,  20
						};

		// 末尾のパディング用データを作成する
		memset( P, 0, sizeof( uint8 ) * 32 );
		for( i = 0 ; i < R ; i++ )
		{
			P[ i ] = data[ len - R + i ];
		}
		for( ; i < 16 ; i++ )
		{
			P[ i ] = pad;
		}

		uint8 L = 0;


		for( i = 0 ; i < N16 ; i++ )
		{
			for( j = 0 ; j < 16 ; j++ )
			{
				uint8 c = data[ i * 16 + j ];
				// RFCのアルゴリズムが間違っている模様
				// 付属のサンプルコードのほうに合わせた
				L = C[ j ] ^= S[ c ^ L ];
			}
		}
		for( j = 0 ; j < 16 ; j++ )
		{
			uint8 c = P[ j ];
			// RFCのアルゴリズムが間違っている模様
			// 付属のサンプルコードのほうに合わせた
			L = C[ j ] ^= S[ c ^ L ];
		}


		uint8 X[ 48 ];
		memset( X, 0, sizeof( uint8 ) * 48 );

		for( i = 0 ; i < N16 ; i++ )
		{
			for( j = 0 ; j < 16 ; j++ )
			{
				X[ 16 + j ] = data[ i * 16 + j ];
				X[ 32 + j ] = X[ 16 + j ] ^ X[ j ];
			}

			size_type t = 0;

 			for( j = 0 ; j < 18 ; j++ )
			{
				for( size_type k = 0 ; k < 48 ; k++ )
				{
					t = X[ k ] ^= S[ t ];
				}

				t = ( t + j ) & 0xff;
			}
		}

		for( i = 0 ; i < 2 ; i++ )
		{
			for( j = 0 ; j < 16 ; j++ )
			{
				X[ 16 + j ] = P[ i * 16 + j ];
				X[ 32 + j ] = X[ 16 + j ] ^ X[ j ];
			}

			size_type t = 0;

 			for( j = 0 ; j < 18 ; j++ )
			{
				for( size_type k = 0 ; k < 48 ; k++ )
				{
					t = X[ k ] ^= S[ t ];
				}

				t = ( t + j ) & 0xff;
			}
		}

		memcpy( digest, X, sizeof( uint8 ) * 16 );
	}


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const{ return( "MD2" ); }


	/// @brief 空文字のダイジェスト文字列で初期化する
	md2( ) : base( "8350e5a3e24c153df2275c9f80692773" ){ }

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	md2( const std::string &str ) : base( 16 ) { base::compute_hash( str ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	md2( const void *data, uint64 len ) : base( 16 ){ compute_hash( data, len ); }
};



/// @brief MD4 を生成するクラス
class md4 : public hash_algorithm
{
private:
	typedef hash_algorithm  base;

public:
	typedef base::size_type			size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef base::difference_type	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef base::uint8				uint8;				///< @brief 符号なし8ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned char 型と同じ．
	typedef base::uint32			uint32;				///< @brief 符号なし32ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned int 型と同じ．
	typedef base::uint64			uint64;				///< @brief 符号なし64ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned long long int 型と同じ．


protected:
	uint32 R( uint32 a, uint32 s ){ return( ( a << s ) | ( a >> ( 32 - s ) ) ); }

	void FF( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 xk, uint32 s )
	{
		a = R( a + ( ( b & c ) | ( ~b & d ) ) + xk, s );
	}

	void GG( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 xk, uint32 s )
	{
		a = R( a + ( ( b & c ) | ( b & d ) | ( d & c ) ) + xk + 0x5a827999, s );
	}

	void HH( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 xk, uint32 s )
	{
		a = R( a + ( b ^ c ^ d ) + xk + 0x6ed9eba1, s );
	}

	void ToCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< uint32 >( x[ i ] ), true ).get_value( );
		}
	}

	void FromCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< uint32 >( x[ i ] ), true ).get_value( );
		}
	}


	void Round( uint32 &a, uint32 &b, uint32 &c, uint32 &d, uint32 x[ 16 ] )
	{
		uint32 A = a;
		uint32 B = b;
		uint32 C = c;
		uint32 D = d;

		// ワードブロックごとの処理を行う
		FF( A, B, C, D, x[  0 ], 3 ); FF( D, A, B, C, x[  1 ], 7 ); FF( C, D, A, B, x[  2 ], 11 ); FF( B, C, D, A, x[  3 ], 19 );
		FF( A, B, C, D, x[  4 ], 3 ); FF( D, A, B, C, x[  5 ], 7 ); FF( C, D, A, B, x[  6 ], 11 ); FF( B, C, D, A, x[  7 ], 19 );
		FF( A, B, C, D, x[  8 ], 3 ); FF( D, A, B, C, x[  9 ], 7 ); FF( C, D, A, B, x[ 10 ], 11 ); FF( B, C, D, A, x[ 11 ], 19 );
		FF( A, B, C, D, x[ 12 ], 3 ); FF( D, A, B, C, x[ 13 ], 7 ); FF( C, D, A, B, x[ 14 ], 11 ); FF( B, C, D, A, x[ 15 ], 19 );

		GG( A, B, C, D, x[  0 ], 3 ); GG( D, A, B, C, x[  4 ], 5 ); GG( C, D, A, B, x[  8 ],  9 ); GG( B, C, D, A, x[ 12 ], 13 );
		GG( A, B, C, D, x[  1 ], 3 ); GG( D, A, B, C, x[  5 ], 5 ); GG( C, D, A, B, x[  9 ],  9 ); GG( B, C, D, A, x[ 13 ], 13 );
		GG( A, B, C, D, x[  2 ], 3 ); GG( D, A, B, C, x[  6 ], 5 ); GG( C, D, A, B, x[ 10 ],  9 ); GG( B, C, D, A, x[ 14 ], 13 );
		GG( A, B, C, D, x[  3 ], 3 ); GG( D, A, B, C, x[  7 ], 5 ); GG( C, D, A, B, x[ 11 ],  9 ); GG( B, C, D, A, x[ 15 ], 13 );

		HH( A, B, C, D, x[  0 ], 3 ); HH( D, A, B, C, x[  8 ], 9 ); HH( C, D, A, B, x[  4 ], 11 ); HH( B, C, D, A, x[ 12 ], 15 );
		HH( A, B, C, D, x[  2 ], 3 ); HH( D, A, B, C, x[ 10 ], 9 ); HH( C, D, A, B, x[  6 ], 11 ); HH( B, C, D, A, x[ 14 ], 15 );
		HH( A, B, C, D, x[  1 ], 3 ); HH( D, A, B, C, x[  9 ], 9 ); HH( C, D, A, B, x[  5 ], 11 ); HH( B, C, D, A, x[ 13 ], 15 );
		HH( A, B, C, D, x[  3 ], 3 ); HH( D, A, B, C, x[ 11 ], 9 ); HH( C, D, A, B, x[  7 ], 11 ); HH( B, C, D, A, x[ 15 ], 15 );

		a += A;
		b += B;
		c += C;
		d += D;
	}

public:
	/// @brief bytes[ 0 ] から bytes[ length - 1 ] の length バイトの MD4 を計算する．
	virtual void compute_hash( const void *bytes, uint64 length )
	{
		size_type len = static_cast< size_type >( length );
		length *= 8;

		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		uint32 &A = *reinterpret_cast< uint32 * >( digest );
		uint32 &B = *reinterpret_cast< uint32 * >( digest + 4 );
		uint32 &C = *reinterpret_cast< uint32 * >( digest + 8 );
		uint32 &D = *reinterpret_cast< uint32 * >( digest + 12 );

		// ダイジェストバイト列の初期化
		A = 0x67452301;
		B = 0xefcdab89;
		C = 0x98badcfe;
		D = 0x10325476;

		size_type i;
		uint32 x[ 16 ];
		uint8 *xx = reinterpret_cast< uint8 * >( x );
		const uint8 *data = reinterpret_cast< const uint8 * >( bytes );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * 64 );
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, x );
		}

		size_type rest = len - i;

		// 最後にバイト長を足す分が存在しなければ，64バイトに拡張して処理する
		if( rest >= 64 - 8 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			// メッセージ処理を行う
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( uint8 ) * 64 );
			x[ 14 ] = static_cast< uint32 >( length );
			x[ 15 ] = static_cast< uint32 >( length >> 32 );

			// メッセージ処理を行う
			Round( A, B, C, D, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = static_cast< uint32 >( length );
			x[ 15 ] = static_cast< uint32 >( length >> 32 );

			// メッセージ処理を行う
			Round( A, B, C, D, x );
		}

		FromCurrentEndian( reinterpret_cast< uint32 * >( digest ), 4 );
	}


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const{ return( "MD4" ); }


	/// @brief 空文字のダイジェスト文字列で初期化する
	md4( ) : base( "31d6cfe0d16ae931b73c59d7e0c089c0" ){ }

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	md4( const std::string &str ) : base( 16 ) { base::compute_hash( str ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	md4( const void *data, uint64 len ) : base( 16 ){ compute_hash( data, len ); }

};





/// @brief MD5 を生成するクラス
class md5 : public hash_algorithm
{
private:
	typedef hash_algorithm  base;

public:
	typedef base::size_type			size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef base::difference_type	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef base::uint8				uint8;				///< @brief 符号なし8ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned char 型と同じ．
	typedef base::uint32			uint32;				///< @brief 符号なし32ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned int 型と同じ．
	typedef base::uint64			uint64;				///< @brief 符号なし64ビット整数を表す型．内部のハッシュ関数値を計算するのに利用．内部的には unsigned long long int 型と同じ．


protected:
	uint32 R( uint32 a, uint32 s ){ return( ( a << s ) | ( a >> ( 32 - s ) ) ); }

	void FF( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 xk, uint32 s, uint32 ti )
	{
		a = b + R( a + ( ( b & c ) | ( ~b & d ) ) + xk + ti, s );
	}

	void GG( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 xk, uint32 s, uint32 ti )
	{
		a = b + R( a + ( ( b & d ) | ( ~d & c ) ) + xk + ti, s );
	}

	void HH( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 xk, uint32 s, uint32 ti )
	{
		a = b + R( a + ( b ^ c ^ d ) + xk + ti, s );
	}

	void II( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 xk, uint32 s, uint32 ti )
	{
		a = b + R( a + ( c ^ ( b | ~d ) ) + xk + ti, s );
	}

	void ToCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< uint32 >( x[ i ] ), true ).get_value( );
		}
	}

	void FromCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< uint32 >( x[ i ] ), true ).get_value( );
		}
	}


	void Round( uint32 &a, uint32 &b, uint32 &c, uint32 &d, uint32 x[ 16 ] )
	{
		uint32 A = a;
		uint32 B = b;
		uint32 C = c;
		uint32 D = d;

		// ワードブロックごとの処理を行う
		FF( A, B, C, D, x[  0 ],  7, 0xd76aa478 );
		FF( D, A, B, C, x[  1 ], 12, 0xe8c7b756 );
		FF( C, D, A, B, x[  2 ], 17, 0x242070db );
		FF( B, C, D, A, x[  3 ], 22, 0xc1bdceee );
		FF( A, B, C, D, x[  4 ],  7, 0xf57c0faf );
		FF( D, A, B, C, x[  5 ], 12, 0x4787c62a );
		FF( C, D, A, B, x[  6 ], 17, 0xa8304613 );
		FF( B, C, D, A, x[  7 ], 22, 0xfd469501 );
		FF( A, B, C, D, x[  8 ],  7, 0x698098d8 );
		FF( D, A, B, C, x[  9 ], 12, 0x8b44f7af );
		FF( C, D, A, B, x[ 10 ], 17, 0xffff5bb1 );
		FF( B, C, D, A, x[ 11 ], 22, 0x895cd7be );
		FF( A, B, C, D, x[ 12 ],  7, 0x6b901122 );
		FF( D, A, B, C, x[ 13 ], 12, 0xfd987193 );
		FF( C, D, A, B, x[ 14 ], 17, 0xa679438e );
		FF( B, C, D, A, x[ 15 ], 22, 0x49b40821 );

		GG( A, B, C, D, x[  1 ],  5, 0xf61e2562 );
		GG( D, A, B, C, x[  6 ],  9, 0xc040b340 );
		GG( C, D, A, B, x[ 11 ], 14, 0x265e5a51 );
		GG( B, C, D, A, x[  0 ], 20, 0xe9b6c7aa );
		GG( A, B, C, D, x[  5 ],  5, 0xd62f105d );
		GG( D, A, B, C, x[ 10 ],  9, 0x02441453 );
		GG( C, D, A, B, x[ 15 ], 14, 0xd8a1e681 );
		GG( B, C, D, A, x[  4 ], 20, 0xe7d3fbc8 );
		GG( A, B, C, D, x[  9 ],  5, 0x21e1cde6 );
		GG( D, A, B, C, x[ 14 ],  9, 0xc33707d6 );
		GG( C, D, A, B, x[  3 ], 14, 0xf4d50d87 );
		GG( B, C, D, A, x[  8 ], 20, 0x455a14ed );
		GG( A, B, C, D, x[ 13 ],  5, 0xa9e3e905 );
		GG( D, A, B, C, x[  2 ],  9, 0xfcefa3f8 );
		GG( C, D, A, B, x[  7 ], 14, 0x676f02d9 );
		GG( B, C, D, A, x[ 12 ], 20, 0x8d2a4c8a );

		HH( A, B, C, D, x[  5 ],  4, 0xfffa3942 );
		HH( D, A, B, C, x[  8 ], 11, 0x8771f681 );
		HH( C, D, A, B, x[ 11 ], 16, 0x6d9d6122 );
		HH( B, C, D, A, x[ 14 ], 23, 0xfde5380c );
		HH( A, B, C, D, x[  1 ],  4, 0xa4beea44 );
		HH( D, A, B, C, x[  4 ], 11, 0x4bdecfa9 );
		HH( C, D, A, B, x[  7 ], 16, 0xf6bb4b60 );
		HH( B, C, D, A, x[ 10 ], 23, 0xbebfbc70 );
		HH( A, B, C, D, x[ 13 ],  4, 0x289b7ec6 );
		HH( D, A, B, C, x[  0 ], 11, 0xeaa127fa );
		HH( C, D, A, B, x[  3 ], 16, 0xd4ef3085 );
		HH( B, C, D, A, x[  6 ], 23, 0x04881d05 );
		HH( A, B, C, D, x[  9 ],  4, 0xd9d4d039 );
		HH( D, A, B, C, x[ 12 ], 11, 0xe6db99e5 );
		HH( C, D, A, B, x[ 15 ], 16, 0x1fa27cf8 );
		HH( B, C, D, A, x[  2 ], 23, 0xc4ac5665 );

		II( A, B, C, D, x[  0 ],  6, 0xf4292244 );
		II( D, A, B, C, x[  7 ], 10, 0x432aff97 );
		II( C, D, A, B, x[ 14 ], 15, 0xab9423a7 );
		II( B, C, D, A, x[  5 ], 21, 0xfc93a039 );
		II( A, B, C, D, x[ 12 ],  6, 0x655b59c3 );
		II( D, A, B, C, x[  3 ], 10, 0x8f0ccc92 );
		II( C, D, A, B, x[ 10 ], 15, 0xffeff47d );
		II( B, C, D, A, x[  1 ], 21, 0x85845dd1 );
		II( A, B, C, D, x[  8 ],  6, 0x6fa87e4f );
		II( D, A, B, C, x[ 15 ], 10, 0xfe2ce6e0 );
		II( C, D, A, B, x[  6 ], 15, 0xa3014314 );
		II( B, C, D, A, x[ 13 ], 21, 0x4e0811a1 );
		II( A, B, C, D, x[  4 ],  6, 0xf7537e82 );
		II( D, A, B, C, x[ 11 ], 10, 0xbd3af235 );
		II( C, D, A, B, x[  2 ], 15, 0x2ad7d2bb );
		II( B, C, D, A, x[  9 ], 21, 0xeb86d391 );

		a += A;
		b += B;
		c += C;
		d += D;
	}

public:
	/// @brief bytes[ 0 ] から bytes[ length - 1 ] の length バイトの MD5 を計算する．
	virtual void compute_hash( const void *bytes, uint64 length )
	{
		size_type len = static_cast< size_type >( length );
		length *= 8;

		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		uint32 &A = *reinterpret_cast< uint32 * >( digest );
		uint32 &B = *reinterpret_cast< uint32 * >( digest + 4 );
		uint32 &C = *reinterpret_cast< uint32 * >( digest + 8 );
		uint32 &D = *reinterpret_cast< uint32 * >( digest + 12 );

		// ダイジェストバイト列の初期化
		A = 0x67452301;
		B = 0xefcdab89;
		C = 0x98badcfe;
		D = 0x10325476;

		size_type i;
		uint32 x[ 16 ];
		uint8 *xx = reinterpret_cast< uint8 * >( x );
		const uint8 *data = reinterpret_cast< const uint8 * >( bytes );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * 64 );
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, x );
		}

		size_type rest = len - i;

		// 最後にバイト長を足す分が存在しなければ，64バイトに拡張して処理する
		if( rest >= 64 - 8 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			// メッセージ処理を行う
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( uint8 ) * 64 );
			x[ 14 ] = static_cast< uint32 >( length );
			x[ 15 ] = static_cast< uint32 >( length >> 32 );

			// メッセージ処理を行う
			Round( A, B, C, D, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = static_cast< uint32 >( length );
			x[ 15 ] = static_cast< uint32 >( length >> 32 );

			// メッセージ処理を行う
			Round( A, B, C, D, x );
		}

		FromCurrentEndian( reinterpret_cast< uint32 * >( digest ), 4 );
	}


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const{ return( "MD5" ); }


	/// @brief 空文字のダイジェスト文字列で初期化する
	md5( ) : base( "d41d8cd98f00b204e9800998ecf8427e" ){ }

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	md5( const std::string &str ) : base( 16 ) { base::compute_hash( str ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	md5( const void *data, uint64 len ) : base( 16 ){ compute_hash( data, len ); }

};


/// @}
//  ハッシュ関数グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MD5__
