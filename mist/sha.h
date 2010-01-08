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

/// @file mist/sha.h
//!
//! @brief 任意のバイト列のMD5を計算する
//!
//! - 参考文献
//!   - SHA-1, RFC3174, http://www.ietf.org/rfc/rfc3174.txt
//!   - SHA-256, 384, 512, "Descriptions of SHA-256, SHA-384, and SHA-512." http://csrc.nist.gov/cryptval/shs/sha256-384-512.pdf 
//!
#ifndef __INCLUDE_SHA__
#define __INCLUDE_SHA__

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


/// @brief SHA-1 を生成するクラス
class sha1 : public hash_algorithm
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

	void ToCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< uint32 >( x[ i ] ), false ).get_value( );
		}
	}

	void FromCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< uint32 >( x[ i ] ), false ).get_value( );
		}
	}

	void Round( uint32 &a, uint32 &b, uint32 &c, uint32 &d, uint32 &e, uint32 x[ 16 ] )
	{
		size_type i;
		uint32 W[ 80 ];

		memcpy( W, x, sizeof( uint32 ) * 16 );

		for( i = 16 ; i < 80 ; i++ )
		{
			W[ i ] = R( W[ i - 3 ] ^ W[ i - 8 ] ^ W[ i - 14 ] ^ W[ i- 16 ], 1 );
		}

		uint32 A = a;
		uint32 B = b;
		uint32 C = c;
		uint32 D = d;
		uint32 E = e;

		// ワードブロックごとの処理を行う
		for( i = 0 ; i < 20 ; i++ )
		{
			uint32 temp = R( A, 5 ) + ( ( B & C ) | ( ~B & D ) ) + E + W[ i ] + 0x5a827999;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 40 ; i++ )
		{
			uint32 temp = R( A, 5 ) + ( B ^ C ^ D ) + E + W[ i ] + 0x6ed9eba1;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 60 ; i++ )
		{
			uint32 temp = R( A, 5 ) + ( ( B & C ) | ( B & D ) | ( C & D ) ) + E + W[ i ] + 0x8f1bbcdc;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 80 ; i++ )
		{
			uint32 temp = R( A, 5 ) + ( B ^ C ^ D ) + E + W[ i ] + 0xca62c1d6;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		a += A;
		b += B;
		c += C;
		d += D;
		e += E;
	}

public:
	/// @brief bytes[ 0 ] から bytes[ length - 1 ] の length バイトの SHA1 を計算する．
	virtual void compute_hash( const void *bytes, uint64 length )
	{
		size_type len = static_cast< size_type >( length );
		length *= 8;

		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		uint32 &A = *reinterpret_cast< uint32 * >( digest );
		uint32 &B = *reinterpret_cast< uint32 * >( digest + 4 );
		uint32 &C = *reinterpret_cast< uint32 * >( digest + 8 );
		uint32 &D = *reinterpret_cast< uint32 * >( digest + 12 );
		uint32 &E = *reinterpret_cast< uint32 * >( digest + 16 );

		// ダイジェストバイト列の初期化
		const uint32 H0 = 0x67452301;
		const uint32 H1 = 0xefcdab89;
		const uint32 H2 = 0x98badcfe;
		const uint32 H3 = 0x10325476;
		const uint32 H4 = 0xc3d2e1f0;

		A = H0;
		B = H1;
		C = H2;
		D = H3;
		E = H4;

		size_type i;
		uint32 x[ 16 ];
		uint8 *xx = reinterpret_cast< uint8 * >( x );
		const uint8 *data = reinterpret_cast< const uint8 * >( bytes );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * 64 );
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, E, x );
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
			Round( A, B, C, D, E, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( uint8 ) * 64 );
			x[ 14 ] = static_cast< uint32 >( length >> 32 );
			x[ 15 ] = static_cast< uint32 >( length );

			// メッセージ処理を行う
			Round( A, B, C, D, E, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = static_cast< uint32 >( length >> 32 );
			x[ 15 ] = static_cast< uint32 >( length );

			// メッセージ処理を行う
			Round( A, B, C, D, E, x );
		}

		FromCurrentEndian( reinterpret_cast< uint32 * >( digest ), 5 );
	}


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const{ return( "SHA1" ); }


	/// @brief 空文字のダイジェスト文字列で初期化する
	sha1( ) : base( "da39a3ee5e6b4b0d3255bfef95601890afd80709" ){ }

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	sha1( const std::string &str ) : base( 20 ) { base::compute_hash( str ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	sha1( const void *data, uint64 len ) : base( 20 ){ compute_hash( data, len ); }

};



/// @brief SHA-256 を生成するクラス
class sha256 : public hash_algorithm
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
	uint32 S( uint32 a, uint32 s ){ return( ( a >> s ) | ( a << ( 32 - s ) ) ); }
	uint32 R( uint32 a, uint32 s ){ return( a >> s ); }
	uint32 Ch( uint32 x, uint32 y, uint32 z ){ return( ( x & y ) ^ ( ~x & z ) ); }
	uint32 Maj( uint32 x, uint32 y, uint32 z ){ return( ( x & y ) ^ ( x & z ) ^ ( y & z ) ); }
	uint32 S0( uint32 x ){ return( S( x,  2 ) ^ S( x, 13 ) ^ S( x, 22 ) ); }
	uint32 S1( uint32 x ){ return( S( x,  6 ) ^ S( x, 11 ) ^ S( x, 25 ) ); }
	uint32 s0( uint32 x ){ return( S( x,  7 ) ^ S( x, 18 ) ^ R( x,  3 ) ); }
	uint32 s1( uint32 x ){ return( S( x, 17 ) ^ S( x, 19 ) ^ R( x, 10 ) ); }

	void ToCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< uint32 >( x[ i ] ), false ).get_value( );
		}
	}

	void FromCurrentEndian( uint32 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< uint32 >( x[ i ] ), false ).get_value( );
		}
	}

	void Round( uint32 *H, uint32 x[ 16 ] )
	{
		size_type i;
		uint32 W[ 64 ];

		memcpy( W, x, sizeof( uint32 ) * 16 );

		for( i = 16 ; i < 64 ; i++ )
		{
			W[ i ] = s1( W[ i - 2 ] ) + W[ i - 7 ] + s0( W[ i - 15 ] ) + W[ i- 16 ];
		}

		uint32 a = H[ 0 ];
		uint32 b = H[ 1 ];
		uint32 c = H[ 2 ];
		uint32 d = H[ 3 ];
		uint32 e = H[ 4 ];
		uint32 f = H[ 5 ];
		uint32 g = H[ 6 ];
		uint32 h = H[ 7 ];

		static uint32 K[] =
			{
				0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
				0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
				0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
				0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
				0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
				0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
				0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
				0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
			};

		// ワードブロックごとの処理を行う
		for( i = 0 ; i < 64 ; i++ )
		{
			uint32 T1 = h + S1( e ) + Ch( e, f, g ) + K[ i ] + W[ i ];
			uint32 T2 = S0( a ) + Maj( a, b, c );
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		H[ 0 ] += a;
		H[ 1 ] += b;
		H[ 2 ] += c;
		H[ 3 ] += d;
		H[ 4 ] += e;
		H[ 5 ] += f;
		H[ 6 ] += g;
		H[ 7 ] += h;
	}

public:
	/// @brief bytes[ 0 ] から bytes[ length - 1 ] の length バイトの SHA-256 を計算する．
	virtual void compute_hash( const void *bytes, uint64 length )
	{
		size_type len = static_cast< size_type >( length );
		length *= 8;

		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		uint32 *H = reinterpret_cast< uint32 * >( digest );

		// ダイジェストバイト列の初期化
		H[ 0 ] = 0x6a09e667;
		H[ 1 ] = 0xbb67ae85;
		H[ 2 ] = 0x3c6ef372;
		H[ 3 ] = 0xa54ff53a;
		H[ 4 ] = 0x510e527f;
		H[ 5 ] = 0x9b05688c;
		H[ 6 ] = 0x1f83d9ab;
		H[ 7 ] = 0x5be0cd19;

		size_type i;
		uint32 x[ 16 ];
		uint8 *xx = reinterpret_cast< uint8 * >( x );
		const uint8 *data = reinterpret_cast< const uint8 * >( bytes );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * 64 );
			ToCurrentEndian( x, 16 );
			Round( H, x );
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
			Round( H, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( uint8 ) * 64 );
			x[ 14 ] = static_cast< uint32 >( length >> 32 );
			x[ 15 ] = static_cast< uint32 >( length );

			// メッセージ処理を行う
			Round( H, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = static_cast< uint32 >( length >> 32 );
			x[ 15 ] = static_cast< uint32 >( length );

			// メッセージ処理を行う
			Round( H, x );
		}

		FromCurrentEndian( reinterpret_cast< uint32 * >( digest ), 8 );
	}


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const{ return( "SHA-256" ); }


	/// @brief 空文字のダイジェスト文字列で初期化する
	sha256( ) : base( "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" ){ }

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	sha256( const std::string &str ) : base( 32 ) { base::compute_hash( str ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	sha256( const void *data, uint64 len ) : base( 32 ){ compute_hash( data, len ); }

};



/// @brief SHA-384 を生成するクラス
class sha384 : public hash_algorithm
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
	uint64 S( const uint64 &a, uint32 s ){ return( ( a >> s ) | ( a << ( 64 - s ) ) ); }
	uint64 R( const uint64 &a, uint32 s ){ return( a >> s ); }
	uint64 Ch( const uint64 &x, const uint64 &y, const uint64 &z ){ return( ( x & y ) ^ ( ~x & z ) ); }
	uint64 Maj( const uint64 &x, const uint64 &y, const uint64 &z ){ return( ( x & y ) ^ ( x & z ) ^ ( y & z ) ); }
	uint64 S0( const uint64 &x ){ return( S( x, 28 ) ^ S( x, 34 ) ^ S( x, 39 ) ); }
	uint64 S1( const uint64 &x ){ return( S( x, 14 ) ^ S( x, 18 ) ^ S( x, 41 ) ); }
	uint64 s0( const uint64 &x ){ return( S( x,  1 ) ^ S( x,  8 ) ^ R( x,  7 ) ); }
	uint64 s1( const uint64 &x ){ return( S( x, 19 ) ^ S( x, 61 ) ^ R( x,  6 ) ); }

	void ToCurrentEndian( uint64 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< uint64 >( x[ i ] ), false ).get_value( );
		}
	}

	void FromCurrentEndian( uint64 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< uint64 >( x[ i ] ), false ).get_value( );
		}
	}

	void Round( uint64 *H, uint64 x[ 16 ] )
	{
		size_type i;
		uint64 W[ 80 ];

		memcpy( W, x, sizeof( uint64 ) * 16 );

		for( i = 16 ; i < 80 ; i++ )
		{
			W[ i ] = s1( W[ i - 2 ] ) + W[ i - 7 ] + s0( W[ i - 15 ] ) + W[ i- 16 ];
		}

		uint64 a = H[ 0 ];
		uint64 b = H[ 1 ];
		uint64 c = H[ 2 ];
		uint64 d = H[ 3 ];
		uint64 e = H[ 4 ];
		uint64 f = H[ 5 ];
		uint64 g = H[ 6 ];
		uint64 h = H[ 7 ];


		static uint64 K[] = {	 4794697086780616226ULL,  8158064640168781261ULL, 13096744586834688815ULL, 16840607885511220156ULL,
								 4131703408338449720ULL,  6480981068601479193ULL, 10538285296894168987ULL, 12329834152419229976ULL,
								15566598209576043074ULL,  1334009975649890238ULL,  2608012711638119052ULL,  6128411473006802146ULL,
								 8268148722764581231ULL,  9286055187155687089ULL, 11230858885718282805ULL, 13951009754708518548ULL,
								16472876342353939154ULL, 17275323862435702243ULL,  1135362057144423861ULL,  2597628984639134821ULL,
								 3308224258029322869ULL,  5365058923640841347ULL,  6679025012923562964ULL,  8573033837759648693ULL,
								10970295158949994411ULL, 12119686244451234320ULL, 12683024718118986047ULL, 13788192230050041572ULL,
								14330467153632333762ULL, 15395433587784984357ULL,   489312712824947311ULL,  1452737877330783856ULL,
								 2861767655752347644ULL,  3322285676063803686ULL,  5560940570517711597ULL,  5996557281743188959ULL,
								 7280758554555802590ULL,  8532644243296465576ULL,  9350256976987008742ULL, 10552545826968843579ULL,
								11727347734174303076ULL, 12113106623233404929ULL, 14000437183269869457ULL, 14369950271660146224ULL,
								15101387698204529176ULL, 15463397548674623760ULL, 17586052441742319658ULL,  1182934255886127544ULL,
								 1847814050463011016ULL,  2177327727835720531ULL,  2830643537854262169ULL,  3796741975233480872ULL,
								 4115178125766777443ULL,  5681478168544905931ULL,  6601373596472566643ULL,  7507060721942968483ULL,
								 8399075790359081724ULL,  8693463985226723168ULL,  9568029438360202098ULL, 10144078919501101548ULL,
								10430055236837252648ULL, 11840083180663258601ULL, 13761210420658862357ULL, 14299343276471374635ULL,
								14566680578165727644ULL, 15097957966210449927ULL, 16922976911328602910ULL, 17689382322260857208ULL,
								  500013540394364858ULL,   748580250866718886ULL,  1242879168328830382ULL,  1977374033974150939ULL,
								 2944078676154940804ULL,  3659926193048069267ULL,  4368137639120453308ULL,  4836135668995329356ULL,
								 5532061633213252278ULL,  6448918945643986474ULL,  6902733635092675308ULL,  7801388544844847127ULL };


		// ワードブロックごとの処理を行う
		for( i = 0 ; i < 80 ; i++ )
		{
			uint64 T1 = h + S1( e ) + Ch( e, f, g ) + K[ i ] + W[ i ];
			uint64 T2 = S0( a ) + Maj( a, b, c );
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		H[ 0 ] += a;
		H[ 1 ] += b;
		H[ 2 ] += c;
		H[ 3 ] += d;
		H[ 4 ] += e;
		H[ 5 ] += f;
		H[ 6 ] += g;
		H[ 7 ] += h;
	}

public:
	/// @brief bytes[ 0 ] から bytes[ length - 1 ] の length バイトの SHA-384 を計算する．
	virtual void compute_hash( const void *bytes, uint64 length )
	{
		size_type len = static_cast< size_type >( length );
		length *= 8;

		// 出力用のダイジェストバイト列
		uint64 H[ 8 ];

		// ダイジェストバイト列の初期化
		H[ 0 ] = 14680500436340154072ULL;
		H[ 1 ] =  7105036623409894663ULL;
		H[ 2 ] = 10473403895298186519ULL;
		H[ 3 ] =  1526699215303891257ULL;
		H[ 4 ] =  7436329637833083697ULL;
		H[ 5 ] = 10282925794625328401ULL;
		H[ 6 ] = 15784041429090275239ULL;
		H[ 7 ] =  5167115440072839076ULL;

		size_type i;
		uint64 x[ 16 ];
		uint8 *xx = reinterpret_cast< uint8 * >( x );
		const uint8 *data = reinterpret_cast< const uint8 * >( bytes );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 128 < len ; i += 128 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * 128 );
			ToCurrentEndian( x, 16 );
			Round( H, x );
		}

		size_type rest = len - i;

		// 最後にバイト長を足す分が存在しなければ，64バイトに拡張して処理する
		if( rest >= 128 - 16 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 128 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			// メッセージ処理を行う
			ToCurrentEndian( x, 16 );
			Round( H, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( uint8 ) * 128 );
			x[ 14 ] = 0;
			x[ 15 ] = length;

			// メッセージ処理を行う
			Round( H, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 128 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = 0;
			x[ 15 ] = length;

			// メッセージ処理を行う
			Round( H, x );
		}

		FromCurrentEndian( H, 8 );

		// 文字列を左に切り詰める
		memcpy( digest, H, size( ) );
	}


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const{ return( "SHA-384" ); }


	/// @brief 空文字のダイジェスト文字列で初期化する
	sha384( ) : base( "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" ){ }

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	sha384( const std::string &str ) : base( 48 ) { base::compute_hash( str ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	sha384( const void *data, uint64 len ) : base( 48 ){ compute_hash( data, len ); }

};




/// @brief SHA-512 を生成するクラス
class sha512 : public hash_algorithm
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
	uint64 S( const uint64 &a, uint32 s ){ return( ( a >> s ) | ( a << ( 64 - s ) ) ); }
	uint64 R( const uint64 &a, uint32 s ){ return( a >> s ); }
	uint64 Ch( const uint64 &x, const uint64 &y, const uint64 &z ){ return( ( x & y ) ^ ( ~x & z ) ); }
	uint64 Maj( const uint64 &x, const uint64 &y, const uint64 &z ){ return( ( x & y ) ^ ( x & z ) ^ ( y & z ) ); }
	uint64 S0( const uint64 &x ){ return( S( x, 28 ) ^ S( x, 34 ) ^ S( x, 39 ) ); }
	uint64 S1( const uint64 &x ){ return( S( x, 14 ) ^ S( x, 18 ) ^ S( x, 41 ) ); }
	uint64 s0( const uint64 &x ){ return( S( x,  1 ) ^ S( x,  8 ) ^ R( x,  7 ) ); }
	uint64 s1( const uint64 &x ){ return( S( x, 19 ) ^ S( x, 61 ) ^ R( x,  6 ) ); }

	void ToCurrentEndian( uint64 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< uint64 >( x[ i ] ), false ).get_value( );
		}
	}

	void FromCurrentEndian( uint64 *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< uint64 >( x[ i ] ), false ).get_value( );
		}
	}

	void Round( uint64 *H, uint64 x[ 16 ] )
	{
		size_type i;
		uint64 W[ 80 ];

		memcpy( W, x, sizeof( uint64 ) * 16 );

		for( i = 16 ; i < 80 ; i++ )
		{
			W[ i ] = s1( W[ i - 2 ] ) + W[ i - 7 ] + s0( W[ i - 15 ] ) + W[ i- 16 ];
		}

		uint64 a = H[ 0 ];
		uint64 b = H[ 1 ];
		uint64 c = H[ 2 ];
		uint64 d = H[ 3 ];
		uint64 e = H[ 4 ];
		uint64 f = H[ 5 ];
		uint64 g = H[ 6 ];
		uint64 h = H[ 7 ];


		static uint64 K[] = {	 4794697086780616226ULL,  8158064640168781261ULL, 13096744586834688815ULL, 16840607885511220156ULL,
								 4131703408338449720ULL,  6480981068601479193ULL, 10538285296894168987ULL, 12329834152419229976ULL,
								15566598209576043074ULL,  1334009975649890238ULL,  2608012711638119052ULL,  6128411473006802146ULL,
								 8268148722764581231ULL,  9286055187155687089ULL, 11230858885718282805ULL, 13951009754708518548ULL,
								16472876342353939154ULL, 17275323862435702243ULL,  1135362057144423861ULL,  2597628984639134821ULL,
								 3308224258029322869ULL,  5365058923640841347ULL,  6679025012923562964ULL,  8573033837759648693ULL,
								10970295158949994411ULL, 12119686244451234320ULL, 12683024718118986047ULL, 13788192230050041572ULL,
								14330467153632333762ULL, 15395433587784984357ULL,   489312712824947311ULL,  1452737877330783856ULL,
								 2861767655752347644ULL,  3322285676063803686ULL,  5560940570517711597ULL,  5996557281743188959ULL,
								 7280758554555802590ULL,  8532644243296465576ULL,  9350256976987008742ULL, 10552545826968843579ULL,
								11727347734174303076ULL, 12113106623233404929ULL, 14000437183269869457ULL, 14369950271660146224ULL,
								15101387698204529176ULL, 15463397548674623760ULL, 17586052441742319658ULL,  1182934255886127544ULL,
								 1847814050463011016ULL,  2177327727835720531ULL,  2830643537854262169ULL,  3796741975233480872ULL,
								 4115178125766777443ULL,  5681478168544905931ULL,  6601373596472566643ULL,  7507060721942968483ULL,
								 8399075790359081724ULL,  8693463985226723168ULL,  9568029438360202098ULL, 10144078919501101548ULL,
								10430055236837252648ULL, 11840083180663258601ULL, 13761210420658862357ULL, 14299343276471374635ULL,
								14566680578165727644ULL, 15097957966210449927ULL, 16922976911328602910ULL, 17689382322260857208ULL,
								  500013540394364858ULL,   748580250866718886ULL,  1242879168328830382ULL,  1977374033974150939ULL,
								 2944078676154940804ULL,  3659926193048069267ULL,  4368137639120453308ULL,  4836135668995329356ULL,
								 5532061633213252278ULL,  6448918945643986474ULL,  6902733635092675308ULL,  7801388544844847127ULL };


		// ワードブロックごとの処理を行う
		for( i = 0 ; i < 80 ; i++ )
		{
			uint64 T1 = h + S1( e ) + Ch( e, f, g ) + K[ i ] + W[ i ];
			uint64 T2 = S0( a ) + Maj( a, b, c );
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		H[ 0 ] += a;
		H[ 1 ] += b;
		H[ 2 ] += c;
		H[ 3 ] += d;
		H[ 4 ] += e;
		H[ 5 ] += f;
		H[ 6 ] += g;
		H[ 7 ] += h;
	}

public:
	/// @brief bytes[ 0 ] から bytes[ length - 1 ] の length バイトの SHA-512 を計算する．
	virtual void compute_hash( const void *bytes, uint64 length )
	{
		size_type len = static_cast< size_type >( length );
		length *= 8;

		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		uint64 *H = reinterpret_cast< uint64 * >( digest );

		// ダイジェストバイト列の初期化
		H[ 0 ] =  7640891576956012808ULL;
		H[ 1 ] = 13503953896175478587ULL;
		H[ 2 ] =  4354685564936845355ULL;
		H[ 3 ] = 11912009170470909681ULL;
		H[ 4 ] =  5840696475078001361ULL;
		H[ 5 ] = 11170449401992604703ULL;
		H[ 6 ] =  2270897969802886507ULL;
		H[ 7 ] =  6620516959819538809ULL;

		size_type i;
		uint64 x[ 16 ];
		uint8 *xx = reinterpret_cast< uint8 * >( x );
		const uint8 *data = reinterpret_cast< const uint8 * >( bytes );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 128 < len ; i += 128 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * 128 );
			ToCurrentEndian( x, 16 );
			Round( H, x );
		}

		size_type rest = len - i;

		// 最後にバイト長を足す分が存在しなければ，64バイトに拡張して処理する
		if( rest >= 128 - 16 )
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 128 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			// メッセージ処理を行う
			ToCurrentEndian( x, 16 );
			Round( H, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( uint8 ) * 128 );
			x[ 14 ] = 0;
			x[ 15 ] = length;

			// メッセージ処理を行う
			Round( H, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( uint8 ) * rest );
			memset( xx + rest, 0, sizeof( uint8 ) * ( 128 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = 0;
			x[ 15 ] = length;

			// メッセージ処理を行う
			Round( H, x );
		}

		FromCurrentEndian( reinterpret_cast< uint64 * >( digest ), 8 );
	}


	/// @brief ハッシュ関数名を返す
	virtual const std::string name( ) const{ return( "SHA-512" ); }


	/// @brief 空文字のダイジェスト文字列で初期化する
	sha512( ) : base( "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" ){ }

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	sha512( const std::string &str ) : base( 64 ) { base::compute_hash( str ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	sha512( const void *data, uint64 len ) : base( 64 ){ compute_hash( data, len ); }

};



/// @}
//  ハッシュ関数グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_SHA__
