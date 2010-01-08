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

/// @file mist/config/decimal.h
//!
//! @brief 多倍長不動少数を扱うためのクラス
//!
#ifndef __INCLUDE_MIST_DECIMAL_H__
#define __INCLUDE_MIST_DECIMAL_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdlib.h>


// mist名前空間の始まり
_MIST_BEGIN

// 以下のコードは Tomy 氏が http://www5.airnet.ne.jp/tomy/cpro/csource.htm にて
// 掲載しているソースを参考に作成し，演算ミス等のバグを修正し，独自の拡張を施したものである．


// 表現可能なデータの有効桁数は，MAX_KETA × log10（32768) となる
// 有効桁数を増やすほどメモリを多く使用するので注意が必要！！



namespace __decimal_util__
{
	template < int N >
	struct log10
	{
		_MIST_CONST( int, value, log10< N / 10 >::value + 1 );
	};

	template < >
	struct log10< 1 >
	{
		_MIST_CONST( int, value, 0 );
	};

	template < >
	struct log10< 0 >
	{
		_MIST_CONST( int, value, 0 );
	};

	template < int N >
	struct pow2
	{
		_MIST_CONST( int, value, 2 * pow2< N - 1 >::value );
	};

	template < >
	struct pow2< 0 >
	{
		_MIST_CONST( int, value, 1 );
	};
}


template < unsigned int MAX_KETA >
class decimal
{
public:
	typedef unsigned int size_type;
	typedef int difference_type;
	typedef unsigned short value_type;

private:
	_MIST_CONST( long, RADIXBITS,	15 );
	_MIST_CONST( long, RADIX,		__decimal_util__::pow2< RADIXBITS >::value );
	_MIST_CONST( long, RADIX1,		RADIX - 1 );
	_MIST_CONST( long, RADIX_2,		RADIX / 2 );
	_MIST_CONST( long, MAXEXP,		__decimal_util__::pow2< RADIXBITS - 1 >::value - 1 );
	_MIST_CONST( long, MINEXP,		- __decimal_util__::pow2< RADIXBITS - 1 >::value );
	//_MIST_CONST( long, MAXEXP,		16383 );
	//_MIST_CONST( long, MINEXP,		-16384 );

	_MIST_CONST( long, NMPA,	MAX_KETA );
	_MIST_CONST( long, NMPA1,	NMPA + 1 );
	_MIST_CONST( long, NMPA2,	NMPA1 * 2 );
	_MIST_CONST( long, MMPA,	NMPA1 * RADIXBITS * 3 / 40 + 1 );

protected:
	bool				sign_;
	difference_type		exp_;
	bool				zero_;
	value_type			data_[ NMPA1 ];

public:
	decimal( ) : sign_( true ), exp_( 0 ), zero_( true )
	{
		memset( data_, 0, sizeof( value_type ) * NMPA1 );
	}

#if 1
	decimal( double a ) : sign_( true ), exp_( 0 ), zero_( true )
	{
		memset( data_, 0, sizeof( value_type ) * NMPA1 );

		if( a != 0.0 )
		{
			zero_ = false;
			if( a < 0.0 )
			{
				sign_ = false;
				a = - a;
			}
			while( a >= static_cast< double >( RADIX ) )
			{
				exp_++;
				a /= static_cast< double >( RADIX );
			}
			while( a < 1.0 )
			{
				exp_--;
				a *= static_cast< double >( RADIX );
			}

			difference_type i = 0;
			do
			{
				data_[ i ] = static_cast< unsigned int >( a );
				a -= data_[ i ];
				a *= RADIX;
			} while( a != 0.0 && ++i <= NMPA );
		}
	}
#else
	decimal( const double &v, difference_type keta = 16 ) : sign_( true ), exp_( 0 ), zero_( true )
	{
		memset( data_, 0, sizeof( value_type ) * NMPA1 );

		if( v != 0.0 )
		{
			double a = v;
			zero_ = false;

			if( a < 0.0 )
			{
				sign_ = false;
				a = - a;
			}

			decimal b( static_cast< difference_type >( a ) );
			a -= static_cast< difference_type >( a );
			decimal x( 1 );

			for( difference_type i = 0 ; i < keta && a != 0.0 ; i++ )
			{
				a *= 10;
				b *= 10;
				b += decimal( static_cast< difference_type >( a ) );
				a -= static_cast< difference_type >( a );
				x *= 10;
			}

			b /= x;
			b.sign_ = sign_;

			operator =( b );
		}
	}
#endif

	decimal( const difference_type &m ) : sign_( true ), exp_( 0 ), zero_( true )
	{
		memset( data_, 0, sizeof( value_type ) * NMPA1 );

		if( m != 0 )
		{
			difference_type n = m;
			zero_ = false;
			if( n < 0 )
			{
				sign_ = false;
				n = - n;
			}
			value_type *p, *q, w;
			p = q = data_;
			while( n != 0 )
			{
				*p++ = static_cast< value_type >( n % RADIX );
				n /= RADIX;
				exp_++;
			}
			exp_--;
			p--;
			while( q < p )
			{
				w = *p;
				*p-- = *q;
				*q++ = w;
			}
		}
	}

	decimal( const decimal &a ) : sign_( a.sign_ ), exp_( a.exp_ ), zero_( a.zero_ )
	{
		memcpy( data_, a.data_, sizeof( value_type ) * NMPA1 );
	}

	decimal( const ::std::string &str ) : sign_( true ), exp_( 0 ), zero_( true )
	{
		operator =( read( str ) );
	}

	const decimal &operator =( const decimal &a )
	{
		if( &a != this )
		{
			sign_ = a.sign_;
			exp_  = a.exp_;
			zero_ = a.zero_;
			memcpy( data_, a.data_, sizeof( value_type ) * NMPA1 );
		}

		return ( *this );
	}

	const decimal  operator -( ) const
	{
		decimal x( *this );
		x.sign_ = !x.sign_;
		return( x );
	}

	const decimal &operator +=( const decimal &a )
	{
		if( zero_ )
		{
			return( operator =( a ) );
		}
		else if( a.zero_ )
		{
			return( *this );
		}
		else if( sign_ == a.sign_ )
		{
			aadd( *this, a );
			return( *this );
		}
		
		int cmp = acmp( *this, a );
		if( cmp == 0 )
		{
			return( operator =( decimal( ) ) );
		}
		else if( cmp > 0 )
		{
			asub( *this, a );
			return( *this );
		}
		decimal tmp( *this );
		*this = a;
		asub( *this, tmp );
		return( *this );
	}

	const decimal &operator -=( const decimal &a )
	{
		return( operator +=( -a ) );
	}

	const decimal &operator *=( const decimal &a )
	{
		if( zero_ )
		{
			return( *this );
		}
		else if( a.zero_ )
		{
			return( operator =( decimal( ) ) );
		}

		difference_type i, j, u, r, xp;
		value_type x[ NMPA2 ];
		memset( x, 0, sizeof( value_type ) * NMPA2 );

		for( i = NMPA, xp = NMPA2 - 1 ; i >= 0 ; i--, xp-- )
		{
			if( a.data_[ i ] )
			{
				u = 0;
				for( j = NMPA, r = xp ; j >= 0; j--, r-- )
				{
					u += static_cast< size_type >( data_[ j ] ) * static_cast< size_type >( a.data_[ i ] ) + x[ r ];
					x[ r ] = static_cast< value_type >( u ) & RADIX1;
					u >>= RADIXBITS;
				}
				while( u )
				{
					u += x[ r ];
					x[ r-- ] = static_cast< value_type >( u ) & RADIX1;
					u >>= RADIXBITS;
				}
			}
		}
		sign_ = sign_ == a.sign_;
		exp_ += a.exp_ + 1;
		for( i = 0, xp = 0 ; i < NMPA2 ; i++, xp++ )
		{
			if( x[ xp ] )
			{
				break;
			}
			exp_--;
		}
		if( exp_ > MAXEXP )
		{
			::std::cerr << "Error : Overflow!!" << ::std::endl;
			return( operator =( maximum( ) ) );
		}
		if( exp_ < MINEXP )
		{
			::std::cerr << "Error : Underflow!!" << ::std::endl;
			return( operator =( decimal( ) ) );
		}
		for( j = 0 ; ( j <= NMPA ) && ( i < NMPA2 ) ; i++, j++, xp++ )
		{
            data_[ j ] = x[ xp ];
		}
		if( j <= NMPA )
		{
			for( ; j <= NMPA ; j++ )
			{
				data_[ j ] = 0;
			}
		}
		else if( x[ xp ] >= RADIX_2 )
		{
			for( i = NMPA ; ++( data_[ i ] ) & RADIX ; i-- )
			{
				data_[ i ] &= RADIX1;
			}
		}
		return( *this );
	}

	const decimal &operator *=( difference_type x )
	{
		if( zero_ )
		{
			return( *this );
		}
		else if( x == 0 )
		{
			return( operator =( decimal( ) ) );
		}
		else if( x < 0 )
		{
			sign_ = !sign_;
			x = - x;
		}
		difference_type xl = static_cast< difference_type >( x );
		difference_type t = 0, i;
		for( i = NMPA ; i >= 0 ; i-- )
		{
			t += static_cast< difference_type >( data_[ i ] * xl );
			data_[ i ] = static_cast< value_type >( t ) & RADIX1;
			t >>= RADIXBITS;
		}
		while( t )
		{
			if( exp_ == MAXEXP )
			{
				::std::cerr << "Error : Overflow!!" << ::std::endl;
				return( operator =( maximum( ) ) );
			}
			for( i = NMPA ; i > 0 ; i-- )
			{
				data_[ i ] = data_[ i - 1 ];
			}
			data_[ i ] = static_cast< value_type >( t ) & RADIX1;
			t >>= RADIXBITS;
			exp_++;
		}
		return( *this );
	}


	const decimal &operator /=( decimal b )
	{
		if( zero_ )
		{
			return( *this );
		}
		else if( b.zero_ )
		{
			// ゼロ除算
			std::cerr << "zero_ division!!" << std::endl;
			return( *this );
		}

		decimal a = *this;
		decimal c, t;
		difference_type a0, b0, bcom, cmp, d, i;
		value_type *p;

		c.zero_ = false;
		c.sign_ = a.sign_ == b.sign_;
		a.sign_ = b.sign_ = true;
		if( ( b.data_[ 0 ] << 1 ) & RADIX )
		{
			bcom = 1;
		}
		else
		{
			bcom = RADIX / ( b.data_[ 0 ] + 1 );
			b *= bcom;
		}
		c.exp_ = a.exp_ - b.exp_;
		a.exp_ = b.exp_ = 0;
		if( acmp( a, b ) < 0 )
		{
			c.exp_--;
			a.exp_++;
		}
		i = 0;
		p = c.data_;
		b0 = b.data_[ 0 ] + 1;
		while( true )
		{
			a0 = a.data_[ 0 ];
			if( a0 > b0 )
			{
				d = a0 / b0;
			}
			else
			{
				a0 = a0 * RADIX + a.data_[ 1 ];
				d = a0 / b0;
				if( d < 1 )
				{
					d = 1;
				}
			}
			while( true )
			{
				t = a - b * d;
				cmp = acmp( t, b );
				if( cmp < 0 )
				{
					break;
				}
				d++;
				if( cmp == 0 )
				{
					break;
				}
			}
			if( i > NMPA )
			{
				if( d >= RADIX_2 )
				{
                    for( i = NMPA, p = c.data_ + i; ++*p & RADIX; i-- )
					{
                        *p-- &= RADIX1;
					}
				}
				break;
			}
			*p++ = static_cast< value_type >( d );
			i++;
			if( cmp == 0 )
			{
				for( ; i <= NMPA ; i++ )
				{
					*p++ = 0;
				}
				break;
			}
			a = t;
			a.exp_++;
			if( acmp( a, b ) < 0 )
			{
				*p++ = 0;
				i++;
				if( i > NMPA )
				{
					break;
				}
				a.exp_++;
			}
		}
		if( bcom != 1 )
		{
			c *= bcom;
		}
		return( operator =( c ) );
	}

	const decimal &operator /=( difference_type x )
	{
		if( x == 0 )
		{
			// ゼロ除算
			std::cerr << "zero division!!" << std::endl;
			return( operator =( maximum( ) ) );
		}
		else if( zero_ )
		{
			return( *this );
		}

		difference_type i, u, n;

		if( x < 0 )
		{
			sign_ = !sign_;
			x = - x;
		}
		if( data_[ 0 ] < static_cast< value_type >( x ) )
		{
			u = data_[ 0 ];
			n = 1;
			exp_--;
		}
		else
		{
			u = 0;
			n = 0;
		}
		for( i = n ; i <= NMPA ; i++ )
		{
			u = ( u << RADIXBITS ) + data_[ i ];
			data_[ i - n ] = static_cast< value_type >( u / x );
			u %= x;
		}
		if( n )
		{
			data_[ i - n ] = static_cast< value_type >( ( u << RADIXBITS ) / x );
			u %= x;
		}
		if( 2 * u >= x )
		{
			for( i = NMPA ; ++( data_[ i ] ) & RADIX ; i-- )
			{
				data_[ i ] &= RADIX1;
			}
		}
		return( *this );
	}

	decimal &operator ++( ) // 前置型
	{
		*this += 1;
		return( *this );
	}

	const decimal operator ++( int ) // 後置型
	{
		decimal old_val( *this );
		*this += 1;
		return( old_val );
	}

	decimal &operator --( ) // 前置型
	{
		*this -= 1;
		return( *this );
	}

	const decimal operator --( int ) // 後置型
	{
		decimal old_val( *this );
		*this -= 1;
		return( old_val );
	}

	bool operator ==( const decimal &a ) const { return( cmp( *this, a ) == 0 ); }
	bool operator !=( const decimal &a ) const { return( !( *this == a ) ); }
	bool operator < ( const decimal &a ) const { return( !( *this >= a ) ); }
	bool operator <=( const decimal &a ) const { return( a >= *this ); }
	bool operator > ( const decimal &a ) const { return( a < *this ); }
	bool operator >=( const decimal &a ) const { return( cmp( *this, a ) >= 0 ); }

	const ::std::string to_string( ) const
	{
		difference_type nmpa = NMPA;
		difference_type mmpa = MMPA - 1;

		if( mmpa < static_cast< difference_type >( std::abs( static_cast< double >( exp_ ) ) ) )
		{
			decimal tmp( *this );
			tmp.exp_ = 0;
			char str[ 10 ];
			sprintf( str, "E%+d", exp_ );
			return( tmp.to_string( ) + str );
		}
		else if( *this == 10000 )
		{
			return( "+10000" );
		}
		else if( *this == -10000 )
		{
			return( "-10000" );
		}

		if( zero_ )
		{
			return( "0" );
		}

		unsigned int c[ MMPA ], t[ NMPA1 ], w;
		difference_type i, cp;

		for( i = 0 ; i <= mmpa ; i++ )
		{
			c[ i ] = 0;
		}

		::std::string s = "";

		if( sign_ )
		{
			s = "+";
		}
		else
		{
			s = "-";
		}

		decimal a( *this );

		cp = 0;
		difference_type n = a.exp_, u, m = 0, mp = -1;
		if( n >= 0 )
		{
			for( i = 0 ; i <= n ; i++ )
			{
				t[ i ] = data_[ i ];
			}
			while( true )
			{
				u = 0;
				for( i = 0 ; i <= n ; i++ )
				{
					u = ( u << RADIXBITS ) + t[ i ];
					t[ i ] = static_cast< value_type >( u / 10000 );
					u %= 10000;
				}
				c[ cp++ ] = static_cast< value_type >( u );
				m++;
				mp++;
				bool zflag = true;
				for( i = 0 ; i <= n ; i++ )
				{
					if( t[ i ] )
					{
						zflag = false;
						break;
					}
				}
				if( zflag )
				{
					break;
				}
			}
			for( i = 0 ; i < m - i - 1 ; i++ )
			{
				w = c[ i ];
				c[ i ] = c[ m - i - 1 ];
				c[ m - i - 1 ] = w;
			}
			for( i = 0 ; i <= nmpa - ( n + 1 ) ; i++ )
			{
				a.data_[ i ] = a.data_[ i + n + 1 ];
			}
			for( ; i <= nmpa ; i++)
			{
				a.data_[ i ] = 0;
			}
			a.exp_ = -1;
		}
		else if( n < -1 )
		{
			while( true )
			{
				a *= 10000;
				if( a.exp_ < 0 )
				{
					mp--;
				}
				else
				{
					c[ cp++ ] = a.data_[ 0 ];
					m++;
					break;
				}
			}
			for( i = 0 ; i < nmpa ; i++ )
			{
				a.data_[ i ] = a.data_[ i + 1 ];
			}
			a.data_[ i ] = 0;
			a.exp_ = -1;
		}

		a *= 10000;
		m++;

		if( a.exp_ == 0 )
		{
			c[ cp++ ] = a.data_[ 0 ];
			a.data_[ 0 ] = 0;
		}
		else
		{
			c[ cp++ ] = 0;
		}

		while( m <= mmpa )
		{
			if( a.is_zero( ) )
			{
				a = decimal( );
				break;
			}
			a *= 10000;
			c[ cp++ ] = a.data_[ 0 ];
			m++;
			a.data_[ 0 ] = 0;
		}

		if( c[ mmpa ] < 5000 )
		{
			c[ mmpa ] = 0;
		}
		else
		{
			c[ mmpa ] = 0;
			for( m = mmpa - 1 ; m >= 0 ; m-- )
			{
				c[ m ]++;
				if( c[ m ] != 10000 )
				{
					break;
				}
				c[ m ] = 0;
			}
		}

		for( i = mmpa ; i >= 0 ; i-- )
		{
			if( c[ i ] )
			{
				break;
			}
			mmpa--;
		}

		if( mmpa < 0 )
		{
			return( "0" );
		}

		if( mp < 0 )
		{
			s += "0.";
			while( ++mp < 0 )
			{
				s += "0000";
			}
			mp = -1;
		}

		cp = 0;

		char str[ 10 ];
		if( mp < 0 )
		{
			sprintf( str, "%04u", c[ cp++ ] );
			s += str;
		}
		else
		{
			sprintf( str, "%u", c[ cp++ ] );
			s += str;
			if( !mp )
			{
				s += ".";
			}
		}

		for( m = 1 ; m <= mmpa ; m++ )
		{
			sprintf( str, "%04u", c[ cp++ ] );
			s += str;
			if( m == mp )
			{
				s += ".";
			}
		}

		if( m < mp )
		{
			do
			{
				s += "0000";
			} while( m++ < mp );
		}

		return( s );
	}

	int to_int( ) const
	{
		if( zero_ || exp_ < 0 )
		{
			return( 0 );
		}
		else if( exp_ > NMPA )
		{
			::std::cerr << "Error : Overflow!!" << ::std::endl;
			return( sign_ ? type_limits< int >::maximum( ) : type_limits< int >::minimum( ) );
		}
		else
		{
			size_type i, index;
			for( index = 0 ; index <= NMPA ; index++ )
			{
				if( data_[ index ] )
				{
					break;
				}
			}
			if( exp_ >= 3 || ( exp_ == 2 && data_[ 0 ] > 2 ) )
			{
				::std::cerr << "Error : Overflow!!" << ::std::endl;
				return( sign_ ? type_limits< int >::maximum( ) : type_limits< int >::minimum( ) );
			}
			else
			{
				int d = 0;
				for( i = index ; i <= index + exp_ && i <= NMPA ; i++ )
				{
                    d = d * RADIX + data_[ i ];
				}
				return( sign_ ? d : -d );
			}
		}
	}

	double to_double( ) const
	{
		if( zero_ )
		{
			return( 0.0 );
		}
		if( exp_ > 68 )
		{
			::std::cerr << "Error : Overflow!!" << ::std::endl;
			return( type_limits< double >::maximum( ) );
		}
		if( exp_ < -68 )
		{
			::std::cerr << "Error : Underflow!!" << ::std::endl;
			return( type_limits< double >::minimum( ) );
		}

		decimal a( *this );
		size_type i;
		while( a.data_[ 0 ] == 0 )
		{
			for( i = 0 ; i < NMPA ; i++ )
			{
				a.data_[ i ] = a.data_[ i + 1 ];
			}
			a.data_[ i ];
			a.exp_--;
		}

		double d = static_cast< double >( a.data_[ 0 ] );
		for( i = 1; i < 7; i++ )
		{
			d = d * static_cast< double >( RADIX ) + static_cast< double >( a.data_[ i ] );
			a.exp_--;
		}
		if( a.data_[ i ] >= RADIX_2 )
		{
			d += 1.0;
		}
		while( a.exp_ > 0 )
		{
			d *= static_cast< double >( RADIX );
			a.exp_--;
		}
		while( a.exp_ < 0 )
		{
			d /= static_cast< double >( RADIX );
			a.exp_++;
		}
		return( a.sign_ ? d : -d );
	}

	bool is_zero( ) const
	{
		if( zero_ )
		{
			return( true );
		}
		for( difference_type i = 0 ; i <= NMPA ; i++ )
		{
			if( data_[ i ] )
			{
				return( false );
			}
		}
		return( true );
	}

	bool is_plus( ) const
	{
		return( sign_ );
	}


public:	// 定数
	static decimal maximum( )
	{
		static decimal max;
		if( max.is_zero( ) )
		{
			max.sign_ = true;
			max.exp_  = MAXEXP;
			for( difference_type i = 0 ; i <= NMPA1 ; i++ )
			{
				max.data_[ i ] = RADIX1;
			}
		}
		return( max );
	}

	static decimal zero( )
	{
		return decimal( );
	}

	static decimal pai( )
	{
		decimal a0( 1 );
		decimal b0( decimal::sqrt( decimal( 2 ) ) / 2 );
		decimal t0( "0.25" );
		decimal a1, b1, t1;

		difference_type _2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
		for( int n = 0 ; n < 10 ; n++ )
		{
			a1 = ( a0 + b0 ) / 2;
			b1 = decimal::sqrt( a0 * b0 );
			t1 = t0 - _2[ n ] * ( a0 - a1 ) * ( a0 - a1 );

			a0 = a1;
			b0 = b1;
			t0 = t1;
		}

		return( ( a0 + b0 ) * ( a0 + b0 ) / ( t0 * 4 ) );
	}

	static decimal sqrt( const decimal &a )
	{
		if( a.is_zero( ) )
		{
			return mist::decimal< MAX_KETA >( );
		}
		else if( !a.is_plus( ) )
		{
			::std::cerr << "Error : Illegal parameter!!" << ::std::endl;
			return( a );
		}

		mist::decimal< MAX_KETA > s( 1 );
		mist::decimal< MAX_KETA > b = a;
		while( s < b )
		{
			s *= 2;
			b /= 2;
		}

		do
		{
			b = s;
			s = ( ( a / s ) + s ) / 2;
		} while( s < b );
		return( b );
	}

protected:
	static int cmp( const decimal &a, const decimal &b )
	{
		if( a.zero_ )
		{
			if( b.zero_ )
			{
				return( 0 );
			}
			return( b.sign_ ? -1 : 1 );
		}
		if( b.zero_ )
		{
			return( a.sign_ ? 1 : -1 );
		}
		if( a.sign_ == b.sign_ )
		{
			int mca = acmp( a, b );
			return( a.sign_ ? mca : -mca );
		}
		return( a.sign_ ? 1 : -1 );
	}

	static int acmp( const decimal &a, const decimal &b )
	{
		if( a.zero_ )
		{
			return( b.zero_ ? 0 : -1 );
		}
		else if( b.zero_ )
		{
			return( 1 );
		}

		difference_type ia, ib;
		difference_type aexp = a.exp_;
		difference_type bexp = b.exp_;

		for( ia = 0 ; ia <= NMPA && aexp != MINEXP ; ia++ )
		{
			if( a.data_[ ia ] != 0 )
			{
				break;
			}
			aexp--;
		}
		for( ib = 0 ; ib <= NMPA && bexp != MINEXP ; ib++ )
		{
			if( b.data_[ ib ] != 0 )
			{
				break;
			}
			bexp--;
		}

		if( aexp != bexp )
		{
			return( aexp > bexp ? 1: -1 );
		}
		for( difference_type i = ia ; i <= NMPA ; i++ )
		{
			if( a.data_[ i ] != b.data_[ i ] )
			{
				return( a.data_[ i ] > b.data_[ i ] ? 1 : -1 );
			}
		}
		return( 0 );
	}

	static difference_type aprs( const decimal &a, decimal &b )
	{
		if( acmp( a, b ) < 0 )
		{
			::std::cerr << "Error : |a| < |b| " << ::std::endl;
			return( -999 );
		}
		difference_type n = a.exp_ - b.exp_;
		if( n == 0 )
		{
			return( -1 );
		}
		else if( n > NMPA )
		{
			b = decimal( );
			return( 0 );
		}
		difference_type  r = b.data_[ NMPA - n + 1 ], i;
		b.exp_ = a.exp_;
		for( i = NMPA ; i >= n ; i-- )
		{
			b.data_[ i ] = b.data_[ i - n ];
		}
		for( ; i >= 0 ; i-- )
		{
			b.data_[ i ] = 0;
		}
		return( r );
	}

	static void aadd( decimal &a, decimal b )
	{
		if( b.zero_ )
		{
			return;
		}

		bool sign = a.sign_;
		if( a.zero_ )
		{
			a = b;
			a.sign_ = sign;
			return;
		}

		decimal t;

		int cmp = acmp( a, b );
		if( cmp < 0 )
		{
			t = a;
			a = b;
			b = t;
			a.sign_ = sign;
		}
		difference_type i = aprs( a, b );
		if( i == -999 )
		{
			a = decimal( );
			return;
		}
		size_type u = 0;
		if( i >= RADIX_2 )
		{
			u = 1;
		}
		for( i = NMPA ; i >= 0 ; i-- )
		{
			u += ( a.data_[ i ] + b.data_[ i ] );
			a.data_[ i ] = static_cast< value_type >( u & RADIX1 );
			u >>= RADIXBITS;
		}
		if( u == 0 )
		{
			a.sign_ = sign;
			return;
		}
		if( a.exp_ == MAXEXP )
		{
			::std::cerr << "Error : Overflow!!" << ::std::endl;
			a = maximum( );
			return;
		}
		difference_type k = a.data_[ NMPA ];
		for( i = NMPA ; i > 0 ; i-- )
		{
			a.data_[ i ] = a.data_[ i - 1 ];
		}
		a.data_[ i ] = static_cast< value_type >( u );
		a.exp_++;
		if( k >= RADIX_2 )
		{
			for( i = NMPA ; ++( a.data_[ i ] ) & RADIX ; i-- )
			{
				a.data_[ i ] &= RADIX1;
			}
		}
		a.sign_ = sign;
	}

	static void asub( decimal &a, decimal b )
	{
		difference_type i = aprs( a, b );
		if( i == -999 )
		{
			return;
		}

		bool sign = a.sign_;
		difference_type u = 0, n, m;

		if( i >= RADIX_2 )
		{
			u = 1;
		}
		for( i = NMPA ; i >= 0 ; i-- )
		{
			u = a.data_[ i ] - b.data_[ i ] - u;
			a.data_[ i ] = static_cast< value_type >( u & RADIX1 );
			u = ( u >> RADIXBITS ) & 1;
		}
		for( n = 0 ; n <= NMPA ; n++ )
		{
			if( a.data_[ n ] )
			{
				break;
			}
		}
		if( n == 0 )
		{
			a.sign_ = sign;
			return;
		}
		if( a.exp_ > MAXEXP - n )
		{
			m = MAXEXP - a.exp_;
		}
		else
		{
			m = n;
		}
		for( i = 0 ; i <= NMPA - m; i++ )
		{
			a.data_[ i ] = a.data_[ n + i ];
		}
		for( i = 1 ; i <= m ; i++ )
		{
			a.data_[ NMPA - m + i ] = 0;
		}
		a.exp_ -= m;
		a.sign_ = sign;
	}

	static decimal read( const ::std::string &str )
	{
		::std::string::const_iterator p = str.begin( );
		bool sign = true;

		if( *p == '-' )
		{
			sign = 0;
			p++;
		}
		else if( *p == '+' )
		{
			p++;
		}

		decimal a, t;
		int c, exp = 0;

		t.zero_ = false;

		int pflag = 0, zflag = 0;
		for( ; p != str.end( ) ; p++ )
		{
			if( *p == '.' )
			{
				if( pflag )
				{
					::std::cerr << "Error : Illegal parameter!!" << ::std::endl;
					return decimal( );
				}
				pflag = true;
			}
			else if( *p == 'E' || *p == 'e' )
			{
				p++;
//				a *= atoi( ::std::string( p, str.end( ) ).c_str( ) );
				exp -= atoi( ::std::string( p, str.end( ) ).c_str( ) );
				break;
			}
			else if( *p == '+' || *p == '-' )
			{
				exp -= atoi( ::std::string( p, str.end( ) ).c_str( ) );
				break;
			}
			else if( *p != ' ' && ( *p < '0' || *p > '9' ) )
			{
				::std::cerr << "Error : Illegal parameter!!" << ::std::endl;
				return decimal( );
			}
			else
			{
				c = *p - '0';
				if( c != 0 || pflag != 0 )
				{
					zflag = -1;
				}
				else
				{
					if( zflag == 0 )
					{
						zflag = 1;
					}
					else if( zflag == 1 )
					{
						::std::cerr << "Error : Illegal parameter!!" << ::std::endl;
						return decimal( );
					}
				}

				a *= 10;

				if( c )
				{
					t.data_[ 0 ] = c;
					a += t;
				}
				if( pflag == 1 )
				{
					exp++;
				}
			}
		}
		if( exp > MAXEXP )
		{
			::std::cerr << "Error : Overflow!!" << ::std::endl;
			return( maximum( ) );
		}
		if( exp < MINEXP )
		{
			::std::cerr << "Error : Underflow!!" << ::std::endl;
			return decimal( );
		}
		a.sign_ = sign;
		if( exp > 0 )
		{
			while( --exp >= 0 )
			{
				a /= 10;
			}
		}
		else
		{
			while( ++exp <= 0 )
			{
				a *= 10;
			}
		}
		if( a.is_zero( ) )
		{
			return decimal( );
		}
		else
		{
			return( a );
		}

	}
};

template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator +( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) += v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator -( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) -= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator *( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) *= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator /( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) /= v2 ); }

template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator *( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) *= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator *( const typename decimal< MAX_KETA >::difference_type &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v2 ) *= v1 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator /( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) /= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator /( const typename decimal< MAX_KETA >::difference_type &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) /= v2 ); }

template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator +( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) += v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator +( const typename decimal< MAX_KETA >::difference_type &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v2 ) += v1 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator -( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) -= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator -( const typename decimal< MAX_KETA >::difference_type &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) -= v2 ); }


template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator *( const decimal< MAX_KETA > &v1, const double &v2 ){ return( decimal< MAX_KETA >( v2 ) *= v1 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator *( const double &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) *= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator /( const decimal< MAX_KETA > &v1, const double &v2 ){ return( decimal< MAX_KETA >( v1 ) /= decimal< MAX_KETA >( v2 ) ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator /( const double &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) /= v2 ); }

template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator +( const decimal< MAX_KETA > &v1, const double &v2 ){ return( decimal< MAX_KETA >( v2 ) += v1 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator +( const double &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) += v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator -( const decimal< MAX_KETA > &v1, const double &v2 ){ return( decimal< MAX_KETA >( v1 ) -= decimal< MAX_KETA >( v2 ) ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator -( const double &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) -= v2 ); }


template < unsigned int MAX_KETA >
inline std::ostream &operator <<( std::ostream &out, const decimal< MAX_KETA > &a )
{
	out << a.to_string( );
	return( out );
}


// mist名前空間の終わり
_MIST_END

namespace std
{
	template < unsigned int MAX_KETA >
	inline mist::decimal< MAX_KETA > sqrt( const mist::decimal< MAX_KETA > &a )
	{
		return( mist::decimal< MAX_KETA >::sqrt( a ) );
	}
}



#endif // __INCLUDE_MIST_DECIMAL_H__
