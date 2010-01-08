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

/// @file mist/config/integer.h
//!
//! @brief 多倍長整数を扱うためのクラス
//!
#ifndef __INCLUDE_MIST_INTEGER_H__
#define __INCLUDE_MIST_INTEGER_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <string>
#include <cstring>

#include <stdlib.h>

// mist名前空間の始まり
_MIST_BEGIN

// 以下のコードは Tomy 氏が http://www5.airnet.ne.jp/tomy/cpro/csource.htm にて
// 掲載しているソースを参考にして作成し，演算ミス等のバグを修正し，独自の拡張を施したものである．
// 基底は256であり，無駄なビットが発生しないようにしてある
// integer< 10 > が表現できる数は，-256^10 以上 256^10 未満の整数となる
// つまり，-1208925819614629174706176 <= x <= 1208925819614629174706176
// 10進の桁数で約24桁程度表現可能となる


template < unsigned int __256_N__ >
class integer
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef unsigned char value_type;

private:
	_MIST_CONST( difference_type, BASE, 256 );
	_MIST_CONST( size_type, DATA_NUM, __256_N__ );

protected:
	size_t		length_;
	bool		sign_;
	value_type	data_[ DATA_NUM ];

public:
	integer( ) : length_( 0 ), sign_( true )
	{
		memset( data_, 0, sizeof( value_type ) * DATA_NUM );
	}

	integer( difference_type v ) : length_( 0 ), sign_( true )
	{
		memset( data_, 0, sizeof( value_type ) * DATA_NUM );
		if( v != 0 )
		{
			if( v < 0 )
			{
				sign_ = false;
				v = -v;
			}
			length_ = 0;
			do
			{
				data_[ length_++ ] = static_cast< value_type >( v % BASE );
				v /= BASE;
			} while( v > 0 && length_ < DATA_NUM );
		}
	}

	integer( const integer &v ) : length_( v.length_ ), sign_( v.sign_ )
	{
		memcpy( data_, v.data_, sizeof( value_type ) * DATA_NUM );
	}

	integer( const ::std::string &str ) : length_( 0 ), sign_( true )
	{
		operator =( read( str ) );
	}

	const integer &operator =( const integer &a )
	{
		if( &a != this )
		{
			sign_ = a.sign_;
			length_ = a.length_;
			memcpy( data_, a.data_, sizeof( value_type ) * DATA_NUM );
		}

		return ( *this );
	}

	const integer  operator -( ) const
	{
		integer x( *this );
		x.sign_ = !x.sign_;
		return( x );
	}

	const integer &operator +=( const integer &a )
	{
		if( length_ == 0 )
		{
			return ( operator =( a ) );
		}
		else if( a.length_ == 0 )
		{
			return ( *this );
		}
		else
		{
			if( sign_ == a.sign_ )
			{
				aadd( *this, a );
			}
			else
			{
				sign_ = asub( *this, a ) ? sign_ : !sign_;
			}
			return( *this );
		}
	}

	const integer &operator -=( const integer &a )
	{
		if( length_ == 0 )
		{
			operator =( a );
			sign_ = !a.sign_;
			return( *this );
		}
		else if( a.length_ == 0 )
		{
			return ( *this );
		}
		else
		{
			if( sign_ == a.sign_ )
			{
				sign_ = asub( *this, a ) ? sign_ : !sign_;
			}
			else
			{
				aadd( *this, a );
			}
			return( *this );
		}
	}

	const integer &operator *=( const integer &a )
	{
		if( length_ * a.length_ == 0 )
		{
			return( operator =( integer( 0 ) ) );
		}
		else if( length_ == 1 )
		{
			difference_type t = sign_ ? data_[ 0 ] : -data_[ 0 ];
			return( operator =( integer( a ) *= t ) );
		}
		else if( a.length_ == 1 )
		{
			difference_type t = a.sign_ ? a.data_[ 0 ] : -a.data_[ 0 ];
			return( operator *=( t ) );
		}

		if( length_ + a.length_ > DATA_NUM )
		{
			// オーバーフロー
			std::cerr << "overflow!!" << std::endl;
			return( *this );
		}

		if( a.length_ > length_ )
		{
			return( operator =( integer( a ) *= *this ) );
		}

		integer x( 0 );
		x.sign_ = sign_ == a.sign_;

		size_type i;
		for( size_type j = 0 ; j < a.length_ ; j++ )
		{
			difference_type u = a.data_[ j ];
			if( u != 0 )
			{
				difference_type t = 0;
				size_type imax = length_ + j - 1;
				for( i = j ; i <= imax ; i++ )
				{
					t += ( x.data_[ i ] + u * data_[ i - j ] );
					x.data_[ i ] = static_cast< value_type >( t % BASE );
					t /= BASE;
				}
				t += x.data_[ i ];
				while( t >= BASE )
				{
					x.data_[ i++ ] = static_cast< value_type >( t - BASE );
					t = x.data_[ i ] + 1;
				}
				x.data_[ i ] = static_cast< value_type >( t );
			}
		}
		x.length_ = length_ + a.length_;
		while( x.data_[ x.length_ - 1 ] == 0 )
		{
			x.length_--;
		}
		return( operator =( x ) );
	}

	const integer &operator *=( difference_type n )
	{
		if( length_ == 0 || n == 0 )
		{
			return( operator =( integer( 0 ) ) );
		}
		//		else if( ::std::abs( static_cast< int >( n ) ) >= BASE )
		else if( n >= BASE || -n >= BASE )
		{
			return( operator *=( integer( n ) ) );
		}

		if( n < 0 )
		{
			sign_ = !sign_;
			n = - n;
		}

		difference_type t = 0;
		for( size_type i = 0 ; i < length_ ; i++ )
		{
			t += data_[ i ] * n;
			data_[ i ] = static_cast< value_type >( t % BASE );
			t /= BASE;
		}

		while( t != 0 )
		{
			length_++;
			if( length_ > DATA_NUM )
			{
				// オーバーフロー
				std::cerr << "overflow!!" << std::endl;
				length_--;
				return( *this );
			}
			data_[ length_ - 1 ] = static_cast< value_type >( t % BASE );
			t /= BASE;
		}
		return( *this );
	}


	const integer &operator /=( const integer &a )
	{
		if( length_ == 0 )
		{
			return( operator =( integer( 0 ) ) );
		}
		else if( a.length_ == 0 )
		{
			// ゼロ除算
			std::cerr << "zero division!!" << std::endl;
			return( *this );
		}
		if( a.length_ == 1 )
		{
			value_type t = a.sign_ ? a.data_[ 0 ] : -a.data_[ 0 ];
			return( operator /=( t ) );
		}

		int c = acmp( *this, a );
		if( c < 0 )
		{
			// 割る数のほうが割られる数より大きいため0にする
			return( operator =( integer( 0 ) ) );
		}
		else if( c == 0 )
		{
			// 割る数と割られる数が等しいので1とし，符号のみ設定する
			return( operator =( integer( sign_ == a.sign_ ? 1 : -1 ) ) );
		}

		if( length_ == DATA_NUM )
		{
			// 割られる数が大きすぎるため除算できない
			std::cerr << "too large to divide!!" << std::endl;
			return( *this );
		}

		integer q, w;
		integer aa( *this ), bb( a );
		difference_type d = BASE / ( a.data_[ a.length_ - 1 ] + 1 ), x;
		q.sign_ = sign_ == a.sign_;

		if( d != 1 )
		{
			aa *= d;
			bb *= d;
		}
		q.length_ = aa.length_ - bb.length_ + 1;
		if( ( x = aa.data_[ aa.length_ - 1 ] / bb.data_[ bb.length_ - 1 ] ) == 0 )
		{
			q.length_--;
			x = ( aa.data_[ aa.length_ - 1 ] * BASE + aa.data_[ aa.length_ - 2 ] ) / bb.data_[ bb.length_ - 1 ];
		}
		aa.sign_ = bb.sign_;
		w.length_ = 0;

		difference_type ql = q.length_;
		for( ; ; )
		{
			for( ; ; )
			{
				w = bb * x;
				shift_up( w, ql - 1 );
				if( acmp( w, aa ) <= 0 )
				{
					break;
				}
				x--;
			}
			q.data_[ ql-- - 1 ] = static_cast< value_type >( x );
			aa -= w;
			if( aa.length_ == 0 || ql == 0 )
			{
				break;
			}
			x = ( aa.data_[ aa.length_ - 1 ] * BASE + aa.data_[ aa.length_ - 2 ] ) / bb.data_[ bb.length_ - 1 ];
		}
		if( q.data_[ q.length_ - 1 ] == 0 )
		{
			q.length_--;
		}
		return( operator =( q ) );
	}

	const integer &operator /=( difference_type n )
	{
		if( length_ == 0 )
		{
			return( *this );
		}
		else if( n == 0 )
		{
			// ゼロ除算
			std::cerr << "zero division!!" << std::endl;
			return( *this );
		}
		//		else if( ::std::abs( static_cast< int >( n ) ) >= BASE )
		else if( n >= BASE || -n >= BASE )
		{
			return( operator /=( integer( n ) ) );
		}

		if( n < 0 )
		{
			sign_ = !sign_;
			n = - n;
		}

		difference_type t = 0;
		for( difference_type i = length_ - 1 ; i >= 0 ; i-- )
		{
			t = t * BASE + data_[ i ];
			data_[ i ] = static_cast< value_type >( t / n );
			t %= n;
		}
		if( data_[ length_ - 1 ] == 0 )
		{
			length_--;
		}
		return( *this );
	}

	const integer &operator %=( const integer &a )
	{
		return( operator =( *this - ( *this / a ) * a ) );
	}

	const integer &operator %=( difference_type n )
	{
		return( operator =( *this - ( *this / n ) * n ) );
	}


	const integer &operator &=( const integer &a )
	{
		for( size_type i = 0 ; i < DATA_NUM ; i++ )
		{
			data_[ i ] &= a.data_[ i ];
		}
		length_ = DATA_NUM;
		for( difference_type j = DATA_NUM - 1 ; j >= 0 ; j-- )
		{
			if( data_[ length_ - 1 ] != 0 )
			{
				break;
			}
			length_--;
		}
		return( *this );
	}

	const integer &operator |=( const integer &a )
	{
		for( size_type i = 0 ; i < DATA_NUM ; i++ )
		{
			data_[ i ] |= a.data_[ i ];
		}
		length_ = length_ > a.length_ ? length_ : a.length_;
		return( *this );
	}

	const integer &operator ^=( const integer &a )
	{
		for( size_type i = 0 ; i < DATA_NUM ; i++ )
		{
			data_[ i ] ^= a.data_[ i ];
		}
		length_ = DATA_NUM;
		for( difference_type j = DATA_NUM - 1 ; j >= 0 ; j-- )
		{
			if( data_[ length_ - 1 ] != 0 )
			{
				break;
			}
			length_--;
		}
		return( *this );
	}

	integer &operator ++( )				// 前置型
	{
		*this += 1;
		return( *this );
	}

	const integer operator ++( int )	// 後置型
	{
		integer old_val( *this );
		*this += 1;
		return( old_val );
	}

	integer &operator --( )				// 前置型
	{
		*this -= 1;
		return( *this );
	}

	const integer operator --( int )	// 後置型
	{
		integer old_val( *this );
		*this -= 1;
		return( old_val );
	}


	bool operator ==( const integer &a ) const { return( sign_ == a.sign_ && acmp( *this, a ) == 0 ); }
	bool operator !=( const integer &a ) const { return( !( *this == a ) ); }
	bool operator < ( const integer &a ) const { return( !( *this >= a ) ); }
	bool operator <=( const integer &a ) const { return( a >= *this ); }
	bool operator > ( const integer &a ) const { return( a < *this ); }
	bool operator >=( const integer &a ) const
	{
		if( sign_ && !a.sign_ )
		{
			return( true );
		}
		else if( !sign_ && a.sign_ )
		{
			return( false );
		}
		else
		{
			bool cmp = acmp( *this, a ) >= 0;
			return( sign_ ? cmp : !cmp );
		}
	}

	const ::std::string to_string( ) const
	{
		if( length_ == 0 )
		{
			return( "0" );
		}
		else if( length_ == 1 )
		{
			char tmp[ 20 ];
			sprintf( tmp, "%d", data_[ 0 ] );
			return( ( sign_ ? "" : "-" ) + ::std::string( tmp ) );
		}
		else
		{
			::std::string s = "";
			integer dmy( *this );
			while( dmy.length_ != 0 )
			{
				difference_type t = 0;
				for( difference_type i = dmy.length_ - 1 ; i >= 0 ; i-- )
				{
					t = t * BASE + dmy.data_[ i ];
					dmy.data_[ i ] = static_cast< value_type >( t / 10 );
					t %= 10;
				}
				if( dmy.data_[ dmy.length_ - 1 ] == 0 )
				{
					dmy.length_--;
				}
				s = static_cast< char >( '0' + t ) + s;
			}
			return( ( sign_ ? "" : "-" ) + s );
		}
	}

	int to_int( ) const
	{
		if( length_ == 0 )
		{
			return( 0 );
		}
		else if( length_ > 4 )
		{
			// オーバーフロー
			std::cerr << "overflow!!" << std::endl;
			return( 0 );
		}
		else
		{
			return( ( sign_ ? 1 : -1 ) * ( data_[ 0 ] + BASE * ( data_[ 1 ] + BASE * ( data_[ 2 ] + BASE * data_[ 3 ] ) ) ) );
		}
	}

	int to_uint( ) const
	{
		if( length_ == 0 )
		{
			return( 0 );
		}
		else if( length_ > 4 )
		{
			// オーバーフロー
			std::cerr << "overflow!!" << std::endl;
			return( 0 );
		}
		else
		{
			return( data_[ 0 ] + BASE * ( data_[ 1 ] + BASE * ( data_[ 2 ] + BASE * data_[ 3 ] ) ) );
		}
	}

protected:
	static int acmp( const integer &a, const integer &b )
	{
		if( a.length_ > b.length_ )
		{
			return ( 1 );
		}
		else if( b.length_ > a.length_ )
		{
			return ( -1 );
		}
		else
		{
			for( difference_type i = a.length_ - 1 ; i >= 0 ; i-- )
			{
				if( a.data_[ i ] < b.data_[ i ] )
				{
					return( -1 );
				}
				else if( a.data_[ i ] > b.data_[ i ] )
				{
					return( 1 );
				}
			}
			return( 0 );
		}
	}

	static void aadd( integer &a0, const integer &b0 )
	{
		const integer *pa = a0.length_ >= b0.length_ ? &a0 : &b0;
		const integer *pb = a0.length_ >= b0.length_ ? &b0 : &a0;

		const integer &a = *pa;
		const integer &b = *pb;

		size_type i;
		difference_type t = 0;

		for( i = 0 ; i < b.length_ ; i++ )
		{
			t += a.data_[ i ] + b.data_[ i ];

			if( t < BASE )
			{
				a0.data_[ i ] = static_cast< value_type >( t );
				t = 0;
			}
			else
			{
				a0.data_[ i ] = static_cast< value_type >( t - BASE );
				t = 1;
			}
		}

		while( i < a.length_ && t != 0 )
		{
			t += a.data_[ i ];

			if( t < BASE )
			{
				a0.data_[ i ] = static_cast< value_type >( t );
				t = 0;
			}
			else
			{
				a0.data_[ i ] = static_cast< value_type >( t - BASE );
				t = 1;
			}

			i++;
		}

		for( ; i < a.length_ ; i++ )
		{
			a0.data_[ i ] = a.data_[ i ];
		}

		if( t == 0 )
		{
			a0.length_ = a.length_;
		}
		else
		{
			if( a.length_ <= DATA_NUM )
			{
				a0.length_ = a.length_ + 1;
				a0.data_[ i ] = 1;
			}
			else
			{
				std::cerr << "overflow!!" << std::endl;
			}
		}
	}

	static bool asub( integer &a0, const integer &b0 )
	{
		int c = acmp( a0, b0 );
		if( c == 0 )
		{
			a0 = integer( );
			return( true );
		}

		const integer *pa = c >= 0 ? &a0 : &b0;
		const integer *pb = c >= 0 ? &b0 : &a0;

		const integer &a = *pa;
		const integer &b = *pb;

		size_type i;
		difference_type t = 0;

		for( i = 0 ; i < b.length_ ; i++ )
		{
			t = a.data_[ i ] - b.data_[ i ] - t;

			if( t >= 0 )
			{
				a0.data_[ i ] = static_cast< value_type >( t );
				t = 0;
			}
			else
			{
				a0.data_[ i ] = static_cast< value_type >( t + BASE );
				t = 1;
			}
		}

		while( i < a.length_ && t != 0 )
		{
			t = a.data_[ i ] - t;

			if( t >= 0 )
			{
				a0.data_[ i ] = static_cast< value_type >( t );
				t = 0;
			}
			else
			{
				a0.data_[ i ] = static_cast< value_type >( t + BASE );
				t = 1;
			}

			i++;
		}

		for( ; i < a.length_ ; i++ )
		{
			a0.data_[ i ] = a.data_[ i ];
		}

		a0.length_ = a.length_;

		while( a0.data_[ a0.length_ - 1 ] == 0 )
		{
			a0.length_--;
		}

		return( c >= 0 );
	}

	static void shift_up( integer &a, difference_type n )
	{
		if( a.length_ == 0 || n == 0 )
		{
		}
		else if( n < 0 )
		{
			shift_down( a, -n );
		}
		else
		{
			difference_type i;
			for( i = a.length_ - 1 ; i >= 0 ; i-- )
			{
				a.data_[ i + n ] = a.data_[ i ];
			}
			for( i = n - 1 ; i >= 0 ; i-- )
			{
				a.data_[ i ] = 0;
			}
			a.length_ += n;
		}
	}

	static void shift_down( integer &a, difference_type n )
	{
		difference_type len = a.length_;
		if( a.length_ == 0 || n == 0 )
		{
		}
		else if( n < 0 )
		{
			shift_up( a, -n );
		}
		else if( len <= n )
		{
			a = integer( 0 );
		}
		else
		{
			difference_type i;
			for( i = 0 ; i < len - n ; i++ )
			{
				a.data_[ i ] = a.data_[ i + n ];
			}
			for( ; i < len ; i++ )
			{
				a.data_[ i ] = 0;
			}
			a.length_ -= n;
		}
	}

	static integer read( const ::std::string &str )
	{
		if( str.size( ) == 0 )
		{
			return( integer( 0 ) );
		}
		else
		{
			size_type i, k = 0;
			bool sign = true;
			if( str[ 0 ] == '-' )
			{
				sign = false;
				k++;
			}
			else if( str[ 0 ] == '+' )
			{
				k++;
			}

			if( str.size( ) - k < 10 )
			{
				return( integer( ::atoi( str.c_str( ) ) ) );
			}

			for( i = k ; i < str.size( ) ; i++ )
			{
				// 符号を除いた部分をコピーする．
				if( '0' <= str[ i ] && str[ i ] <= '9' )
				{
				}
				else
				{
					break;
				}
			}

			::std::string s = str.substr( k, i - k + 1 );

			integer x( 0 );
			integer base( 100000000 ), b( 1 );
			while( !s.empty( ) )
			{
				difference_type len = s.size( );
				if( len > 8 )
				{
					x += integer( ::atoi( ( s.substr( len - 8 ) ).c_str( ) ) ) * b;
					b *= base;
					s = s.substr( 0, len - 8 );
				}
				else
				{
					x += integer( ::atoi( s.c_str( ) ) ) * b;
					s = "";
				}
			}
			x.sign_ = sign;
			return( x );
		}
	}
};


template < unsigned int __256_N__ > inline const integer< __256_N__ > operator +( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) += v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator -( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) -= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator *( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) *= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator /( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) /= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator %( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) %= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator |( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) |= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator &( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) &= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator ^( const integer< __256_N__ > &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) ^= v2 ); }

template < unsigned int __256_N__ > inline const integer< __256_N__ > operator *( const integer< __256_N__ > &v1, const typename integer< __256_N__ >::difference_type &v2 ){ return( integer< __256_N__ >( v1 ) *= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator *( const typename integer< __256_N__ >::difference_type &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v2 ) *= v1 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator /( const integer< __256_N__ > &v1, const typename integer< __256_N__ >::difference_type &v2 ){ return( integer< __256_N__ >( v1 ) /= v2 ); }

template < unsigned int __256_N__ > inline const integer< __256_N__ > operator +( const integer< __256_N__ > &v1, const typename integer< __256_N__ >::difference_type &v2 ){ return( integer< __256_N__ >( v1 ) += v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator +( const typename integer< __256_N__ >::difference_type &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v2 ) += v1 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator -( const integer< __256_N__ > &v1, const typename integer< __256_N__ >::difference_type &v2 ){ return( integer< __256_N__ >( v1 ) -= v2 ); }
template < unsigned int __256_N__ > inline const integer< __256_N__ > operator -( const typename integer< __256_N__ >::difference_type &v1, const integer< __256_N__ > &v2 ){ return( integer< __256_N__ >( v1 ) -= v2 ); }

template < unsigned int __256_N__ > inline const integer< __256_N__ > operator %( const integer< __256_N__ > &v1, const typename integer< __256_N__ >::difference_type &v2 ){ return( integer< __256_N__ >( v1 ) %= v2 ); }


template < unsigned int __256_N__ >
inline std::ostream &operator <<( std::ostream &out, const integer< __256_N__ > &a )
{
	out << a.to_string( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_INTEGER_H__

