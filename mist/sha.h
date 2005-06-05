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

protected:
	unsigned int R( unsigned int a, unsigned int s ){ return( ( a << s ) | ( a >> ( 32 - s ) ) ); }

	unsigned int left_most( unsigned int x )
	{
		return( ( x << 24 ) | ( ( x & 0x0000ff00 ) << 8 ) | ( ( x & 0x00ff0000 ) >> 8 ) | ( x  >> 24 ) );
	}

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

	/// @brief ダイジェストバイト列の長さ
	size_type size( ) const { return( 20 ); }

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
		D[ 0 ] = 0xd98c1dd4;
		D[ 1 ] = 0x04b2008f;
		D[ 2 ] = 0x980980e9;
		D[ 3 ] = 0x7e42f8ec;
		FromCurrentEndian( D, 20 );
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


/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … weak_ptr 配列
//! 
//! @return 入力されたストリーム
//! 
inline std::ostream &operator <<( std::ostream &out, const sha1 &m )
{
	out << m.to_string( );
	return( out );
}


/// @}
//  ハッシュ関数グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_SHA__
