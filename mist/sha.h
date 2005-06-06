/// @file mist/md5.h
//!
//! @brief 任意のバイト列のMD5を計算する
//!
//! - 参考文献
//!   - RFC3174, http://www.ietf.org/rfc/rfc3174.txt
//!
#ifndef __INCLUDE_SHA__
#define __INCLUDE_SHA__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "config/endian.h"
#endif

#include <string>


// mist名前空間の始まり
_MIST_BEGIN


namespace __integer64__
{
	class uint64
	{
	public:
		typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	private:
		unsigned int value[ 2 ];

	public:
		explicit uint64( unsigned int high = 0, unsigned int low = 0 )
		{
			value[ 0 ] = low;
			value[ 1 ] = high;
		}

		const uint64 &operator =( unsigned int a )
		{
			value[ 1 ] = 0;
			value[ 0 ] = a;

			return( *this );
		}

		const uint64 &operator +=( const uint64 &a )
		{
			if( value[ 0 ] < 0xffffffff - a.value[ 0 ] )
			{
				value[ 0 ] += a.value[ 0 ];
				value[ 1 ] += a.value[ 1 ];
			}
			else
			{
				unsigned int v1 = ( value[ 0 ] & 0x0000ffff ) + ( a.value[ 0 ] & 0x0000ffff );
				unsigned int v2 = ( ( value[ 0 ] & 0xffff0000 ) >> 16 ) + ( ( a.value[ 0 ] & 0xffff0000 ) >> 16 ) + ( v1 >> 16 );

				value[ 0 ] = ( ( v2 << 16 ) & 0xffff0000 ) + ( v1 & 0x0000ffff );
				value[ 1 ] += a.value[ 1 ] + ( ( v2 & 0xffff0000 ) >> 16 );
			}
			return( *this );
		}

		const uint64 &operator <<=( size_type a )
		{
			value[ 1 ] = ( value[ 1 ] << a ) | ( value[ 0 ] >> ( 32 - a ) );
			value[ 0 ] = value[ 0 ] << a;

			return( *this );
		}

		const uint64 &operator >>=( size_type a )
		{
			value[ 0 ] = ( value[ 0 ] >> a ) | ( value[ 1 ] << ( 32 - a ) );
			value[ 1 ] = value[ 1 ] >> a;

			return( *this );
		}

		const uint64 &operator |=( const uint64 &a )
		{
			value[ 0 ] |= a.value[ 0 ];
			value[ 1 ] |= a.value[ 1 ];
			return( *this );
		}

		const uint64 &operator &=( const uint64 &a )
		{
			value[ 0 ] &= a.value[ 0 ];
			value[ 1 ] &= a.value[ 1 ];
			return( *this );
		}

		const uint64 &operator ^=( const uint64 &a )
		{
			value[ 0 ] ^= a.value[ 0 ];
			value[ 1 ] ^= a.value[ 1 ];
			return( *this );
		}

		uint64 operator ~( ) const
		{
			uint64 v;
			v.value[ 0 ] = ~value[ 0 ];
			v.value[ 1 ] = ~value[ 1 ];
			return( v );
		}


		/// @brief ダイジェスト文字列を返す
		uint64 to_current_endian( bool from_little_endian ) const
		{
			uint64 v;
			v.value[ 0 ] = mist::to_current_endian( byte_array< unsigned int >( value[ 1 ] ), from_little_endian ).get_value( );
			v.value[ 1 ] = mist::to_current_endian( byte_array< unsigned int >( value[ 0 ] ), from_little_endian ).get_value( );
			return( v );
		}

		/// @brief ダイジェスト文字列を返す
		uint64 from_current_endian( bool to_little_endian ) const
		{
			uint64 v;
			v.value[ 0 ] = mist::from_current_endian( byte_array< unsigned int >( value[ 1 ] ), to_little_endian ).get_value( );
			v.value[ 1 ] = mist::from_current_endian( byte_array< unsigned int >( value[ 0 ] ), to_little_endian ).get_value( );
			return( v );
		}

		/// @brief ダイジェスト文字列を返す
		std::string to_string( ) const
		{
			static char x16[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
			std::string str;

			const unsigned char *v = reinterpret_cast< const unsigned char * >( value );
			for( int i = 7 ; i >= 0 ; i-- )
			{
				str += x16[ ( v[ i ] >> 4 ) & 0x0f ];
				str += x16[ v[ i ] & 0x0f ];
			}

			return( str );
		}
	};

	inline const uint64 operator +( const uint64 &v1, const uint64 &v2 ){ return( uint64( v1 ) += v2 ); }
	inline const uint64 operator |( const uint64 &v1, const uint64 &v2 ){ return( uint64( v1 ) |= v2 ); }
	inline const uint64 operator &( const uint64 &v1, const uint64 &v2 ){ return( uint64( v1 ) &= v2 ); }
	inline const uint64 operator ^( const uint64 &v1, const uint64 &v2 ){ return( uint64( v1 ) ^= v2 ); }

	inline const uint64 operator <<( const uint64 &v1, const uint64::size_type a ){ return( uint64( v1 ) <<= a ); }
	inline const uint64 operator >>( const uint64 &v1, const uint64::size_type a ){ return( uint64( v1 ) >>= a ); }
}


//! @defgroup hash_group ハッシュ関数
//!  @{


/// @brief SHA-1 を生成するクラス
class sha1
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	unsigned char digest[ 20 ];

public:
	/// @brief ダイジェストバイト列の長さ
	size_type size( ) const { return( 20 ); }

protected:
	unsigned int R( unsigned int a, unsigned int s ){ return( ( a << s ) | ( a >> ( 32 - s ) ) ); }

	void ToCurrentEndian( unsigned int *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< unsigned int >( x[ i ] ), false ).get_value( );
		}
	}

	void FromCurrentEndian( unsigned int *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< unsigned int >( x[ i ] ), false ).get_value( );
		}
	}

	void Round( unsigned int &a, unsigned int &b, unsigned int &c, unsigned int &d, unsigned int &e, unsigned int x[ 16 ] )
	{
		size_type i;
		unsigned int W[ 80 ];

		memcpy( W, x, sizeof( unsigned int ) * 16 );

		for( i = 16 ; i < 80 ; i++ )
		{
			W[ i ] = R( W[ i - 3 ] ^ W[ i - 8 ] ^ W[ i - 14 ] ^ W[ i- 16 ], 1 );
		}

		unsigned int A = a;
		unsigned int B = b;
		unsigned int C = c;
		unsigned int D = d;
		unsigned int E = e;

		// ワードブロックごとの処理を行う
		for( i = 0 ; i < 20 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( ( B & C ) | ( ~B & D ) ) + E + W[ i ] + 0x5a827999;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 40 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( B ^ C ^ D ) + E + W[ i ] + 0x6ed9eba1;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 60 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( ( B & C ) | ( B & D ) | ( C & D ) ) + E + W[ i ] + 0x8f1bbcdc;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 80 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( B ^ C ^ D ) + E + W[ i ] + 0xca62c1d6;
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
	/// @brief data[ 0 ] から data[ len - 1 ] の len バイトの MD5 を計算する．
	void generate( const void *data_, size_type len )
	{
		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		unsigned int &A = *reinterpret_cast< unsigned int * >( digest );
		unsigned int &B = *reinterpret_cast< unsigned int * >( digest + 4 );
		unsigned int &C = *reinterpret_cast< unsigned int * >( digest + 8 );
		unsigned int &D = *reinterpret_cast< unsigned int * >( digest + 12 );
		unsigned int &E = *reinterpret_cast< unsigned int * >( digest + 16 );

		// ダイジェストバイト列の初期化
		const unsigned int H0 = 0x67452301;
		const unsigned int H1 = 0xefcdab89;
		const unsigned int H2 = 0x98badcfe;
		const unsigned int H3 = 0x10325476;
		const unsigned int H4 = 0xc3d2e1f0;

		A = H0;
		B = H1;
		C = H2;
		D = H3;
		E = H4;

		size_type i;
		unsigned int x[ 16 ];
		unsigned char *xx = reinterpret_cast< unsigned char * >( x );
		const unsigned char *data = reinterpret_cast< const unsigned char * >( data_ );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * 64 );
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, E, x );
		}

		size_type rest = len - i;

		// 最後にバイト長を足す分が存在しなければ，64バイトに拡張して処理する
		if( rest >= 64 - 8 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			// メッセージ処理を行う
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, E, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( unsigned char ) * 64 );
			x[ 14 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );

			// メッセージ処理を行う
			Round( A, B, C, D, E, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );

			// メッセージ処理を行う
			Round( A, B, C, D, E, x );
		}

		FromCurrentEndian( reinterpret_cast< unsigned int * >( digest ), 5 );
	}

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

	/// @brief 空文字のダイジェスト文字列で初期化する
	sha1( )
	{
		unsigned int *D = reinterpret_cast< unsigned int * >( digest );
		D[ 0 ] = 0xda39a3ee;
		D[ 1 ] = 0x5e6b4b0d;
		D[ 2 ] = 0x3255bfef;
		D[ 3 ] = 0x95601890;
		D[ 4 ] = 0xafd80709;
		FromCurrentEndian( D, 5 );
	}

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	sha1( const std::string &str ){ generate( str.c_str( ), str.length( ) ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	sha1( const void *data, size_type len ){ generate( data, len ); }


public:
	/// @brief 2つのMD5が同一かどうかを判定する
	bool operator ==( const sha1 &m ) const
	{
		for( size_type i = 0 ; i < size( ) ; i++ )
		{
			if( digest[ i ] != m[ i ] )
			{
				return( false );
			}
		}
		return( true );
	}

	/// @brief 2つのダイジェスト文字列が同一かどうかを判定する
	bool operator ==( const std::string &str ) const
	{
		return( str == to_string( ) );
	}
};



/// @brief SHA-256 を生成するクラス
class sha256
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	unsigned char digest[ 32 ];

public:
	/// @brief ダイジェストバイト列の長さ
	size_type size( ) const { return( 32 ); }

protected:
	unsigned int S( unsigned int a, unsigned int s ){ return( ( a >> s ) | ( a << ( 32 - s ) ) ); }
	unsigned int R( unsigned int a, unsigned int s ){ return( a >> s ); }
	unsigned int Ch( unsigned int x, unsigned int y, unsigned int z ){ return( ( x & y ) ^ ( ~x & z ) ); }
	unsigned int Maj( unsigned int x, unsigned int y, unsigned int z ){ return( ( x & y ) ^ ( x & z ) ^ ( y & z ) ); }
	unsigned int S0( unsigned int x ){ return( S( x,  2 ) ^ S( x, 13 ) ^ S( x, 22 ) ); }
	unsigned int S1( unsigned int x ){ return( S( x,  6 ) ^ S( x, 11 ) ^ S( x, 25 ) ); }
	unsigned int s0( unsigned int x ){ return( S( x,  7 ) ^ S( x, 18 ) ^ R( x,  3 ) ); }
	unsigned int s1( unsigned int x ){ return( S( x, 17 ) ^ S( x, 19 ) ^ R( x, 10 ) ); }

	void ToCurrentEndian( unsigned int *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< unsigned int >( x[ i ] ), false ).get_value( );
		}
	}

	void FromCurrentEndian( unsigned int *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< unsigned int >( x[ i ] ), false ).get_value( );
		}
	}

	void Round( unsigned int *H, unsigned int x[ 16 ] )
	{
		size_type i;
		unsigned int W[ 64 ];

		memcpy( W, x, sizeof( unsigned int ) * 16 );

		for( i = 16 ; i < 64 ; i++ )
		{
			W[ i ] = s1( W[ i - 2 ] ) + W[ i - 7 ] + s0( W[ i - 15 ] ) + W[ i- 16 ];
		}

		unsigned int a = H[ 0 ];
		unsigned int b = H[ 1 ];
		unsigned int c = H[ 2 ];
		unsigned int d = H[ 3 ];
		unsigned int e = H[ 4 ];
		unsigned int f = H[ 5 ];
		unsigned int g = H[ 6 ];
		unsigned int h = H[ 7 ];

		static unsigned int K[] =
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
			unsigned int T1 = h + S1( e ) + Ch( e, f, g ) + K[ i ] + W[ i ];
			unsigned int T2 = S0( a ) + Maj( a, b, c );
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
	/// @brief data[ 0 ] から data[ len - 1 ] の len バイトの MD5 を計算する．
	void generate( const void *data_, size_type len )
	{
		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		unsigned int *H = reinterpret_cast< unsigned int * >( digest );

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
		unsigned int x[ 16 ];
		unsigned char *xx = reinterpret_cast< unsigned char * >( x );
		const unsigned char *data = reinterpret_cast< const unsigned char * >( data_ );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * 64 );
			ToCurrentEndian( x, 16 );
			Round( H, x );
		}

		size_type rest = len - i;

		// 最後にバイト長を足す分が存在しなければ，64バイトに拡張して処理する
		if( rest >= 64 - 8 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			// メッセージ処理を行う
			ToCurrentEndian( x, 16 );
			Round( H, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( unsigned char ) * 64 );
			x[ 14 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );

			// メッセージ処理を行う
			Round( H, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 64 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );

			// メッセージ処理を行う
			Round( H, x );
		}

		FromCurrentEndian( reinterpret_cast< unsigned int * >( digest ), 8 );
	}


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

	/// @brief 空文字のダイジェスト文字列で初期化する
	sha256( )
	{
		unsigned int *H = reinterpret_cast< unsigned int * >( digest );
		H[ 0 ] = 0xe3b0c442;
		H[ 1 ] = 0x98fc1c14;
		H[ 2 ] = 0x9afbf4c8;
		H[ 3 ] = 0x996fb924;
		H[ 4 ] = 0x27ae41e4;
		H[ 5 ] = 0x649b934c;
		H[ 6 ] = 0xa495991b;
		H[ 7 ] = 0x7852b855;
		FromCurrentEndian( H, 8 );
	}

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	sha256( const std::string &str ){ generate( str.c_str( ), str.length( ) ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	sha256( const void *data, size_type len ){ generate( data, len ); }


public:
	/// @brief 2つのMD5が同一かどうかを判定する
	bool operator ==( const sha1 &m ) const
	{
		for( size_type i = 0 ; i < size( ) ; i++ )
		{
			if( digest[ i ] != m[ i ] )
			{
				return( false );
			}
		}
		return( true );
	}

	/// @brief 2つのダイジェスト文字列が同一かどうかを判定する
	bool operator ==( const std::string &str ) const
	{
		return( str == to_string( ) );
	}
};



/// @brief SHA-384 を生成するクラス
class sha512
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ > 0
	typedef unsigned __int64 uint64;
#else
	typedef u_int64_t uint64;
#endif

private:
	unsigned char digest[ 64 ];

public:
	/// @brief ダイジェストバイト列の長さ
	size_type size( ) const { return( 64 ); }

protected:
	uint64 S( const uint64 &a, unsigned int s ){ return( ( a >> s ) | ( a << ( 64 - s ) ) ); }
	uint64 R( const uint64 &a, unsigned int s ){ return( a >> s ); }
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

		static uint64 K[] = {	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
								0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
								0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
								0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
								0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
								0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
								0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
								0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
								0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
								0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
								0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
								0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
								0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
								0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
								0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
								0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
								0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
								0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
								0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
								0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817 };

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
	/// @brief data[ 0 ] から data[ len - 1 ] の len バイトの MD5 を計算する．
	void generate( const void *data_, size_type len )
	{
		// 出力用のダイジェストバイト列を 32 ビット単位で処理できるようにする
		uint64 *H = reinterpret_cast< uint64 * >( digest );

		// ダイジェストバイト列の初期化
		H[ 0 ] = 0x6a09e667f3bcc908;
		H[ 1 ] = 0xbb67ae8584caa73b;
		H[ 2 ] = 0x3c6ef372fe94f82b;
		H[ 3 ] = 0xa54ff53a5f1d36f1;
		H[ 4 ] = 0x510e527fade682d1;
		H[ 5 ] = 0x9b05688c2b3e6c1f;
		H[ 6 ] = 0x1f83d9abfb41bd6b;
		H[ 7 ] = 0x5be0cd19137e2179;

		size_type i;
		uint64 x[ 16 ];
		unsigned char *xx = reinterpret_cast< unsigned char * >( x );
		const unsigned char *data = reinterpret_cast< const unsigned char * >( data_ );

		// 入力データに対してメッセージ処理を行う
		for( i = 0 ; i + 128 < len ; i += 128 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * 128 );
			ToCurrentEndian( x, 16 );
			Round( H, x );
		}

		size_type rest = len - i;

		// 最後にバイト長を足す分が存在しなければ，64バイトに拡張して処理する
		if( rest >= 128 - 16 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 128 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			// メッセージ処理を行う
			ToCurrentEndian( x, 16 );
			Round( H, x );

			// バイト長の分の処理を行う
			memset( xx, 0, sizeof( unsigned char ) * 128 );
			x[ 14 ] = uint64( static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 ) );
			x[ 15 ] = uint64( static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 ) );

			// メッセージ処理を行う
			Round( H, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 128 - rest ) );
			// 先頭のビットを 1 にする
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// バイト長の分の値を付加する
			x[ 14 ] = uint64( static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 ) );
			x[ 15 ] = uint64( static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 ) );

			// メッセージ処理を行う
			Round( H, x );
		}

		FromCurrentEndian( reinterpret_cast< uint64 * >( digest ), 8 );
	}


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

	/// @brief 空文字のダイジェスト文字列で初期化する
	sha512( )
	{
		uint64 *H = reinterpret_cast< uint64 * >( digest );
		H[ 0 ] = 0xcf83e1357eefb8bd;
		H[ 0 ] = 0xf1542850d66d8007;
		H[ 0 ] = 0xd620e4050b5715dc;
		H[ 0 ] = 0x83f4a921d36ce9ce;
		H[ 0 ] = 0x47d0d13c5d85f2b0;
		H[ 0 ] = 0xff8318d2877eec2f;
		H[ 0 ] = 0x63b931bd47417a81;
		H[ 0 ] = 0xa538327af927da3e;
		FromCurrentEndian( H, 8 );
	}

	/// @brief 指定された文字列のダイジェスト文字列で初期化する
	sha512( const std::string &str ){ generate( str.c_str( ), str.length( ) ); }

	/// @brief 指定されたバイト列のダイジェスト文字列で初期化する
	sha512( const void *data, size_type len ){ generate( data, len ); }


public:
	/// @brief 2つのMD5が同一かどうかを判定する
	bool operator ==( const sha1 &m ) const
	{
		for( size_type i = 0 ; i < size( ) ; i++ )
		{
			if( digest[ i ] != m[ i ] )
			{
				return( false );
			}
		}
		return( true );
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
//! @param[in]     p   … sha1 オブジェクト
//! 
//! @return 入力されたストリーム
//! 
inline std::ostream &operator <<( std::ostream &out, const sha1 &m )
{
	out << m.to_string( );
	return( out );
}

/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … sha256 オブジェクト
//! 
//! @return 入力されたストリーム
//! 
inline std::ostream &operator <<( std::ostream &out, const sha256 &m )
{
	out << m.to_string( );
	return( out );
}

/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … sha384 オブジェクト
//! 
//! @return 入力されたストリーム
//! 
//inline std::ostream &operator <<( std::ostream &out, const sha384 &m )
//{
//	out << m.to_string( );
//	return( out );
//}

/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … sha512 オブジェクト
//! 
//! @return 入力されたストリーム
//! 
inline std::ostream &operator <<( std::ostream &out, const sha512 &m )
{
	out << m.to_string( );
	return( out );
}


/// @}
//  ハッシュ関数グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_SHA__
