#ifndef __INCLUDE_MIST_DECIMAL_H__
#define __INCLUDE_MIST_DECIMAL_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <string>


// mist名前空間の始まり
_MIST_BEGIN

// 以下のコードは Tomy 氏が http://www5.airnet.ne.jp/tomy/cpro/csource.htm にて
// 掲載しているソースを基に作成し，演算ミス等のバグを修正し，独自の拡張を施したものである．
// 基底は256であり，無駄なビットが発生しないようにしてある


namespace __decimal_util__
{
	template < unsigned int N >
	struct log10
	{
		_MIST_CONST( unsigned int, value, log10< N / 10 >::value + 1 );
	};

	template < >
	struct log10< 1 >
	{
		_MIST_CONST( unsigned int, value, 0 );
	};

	template < >
	struct log10< 0 >
	{
		_MIST_CONST( unsigned int, value, 0 );
	};

	template < unsigned int N >
	struct pow2
	{
		_MIST_CONST( unsigned int, value, 2 * pow2< N - 1 >::value );
	};

	template < >
	struct pow2< 0 >
	{
		_MIST_CONST( unsigned int, value, 1 );
	};
}


template < unsigned int MAX_KETA >
class decimal
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef unsigned int value_type;

private:
	_MIST_CONST( long, RADIXBITS,	15 );
	_MIST_CONST( long, RADIX,		__decimal_util__::pow2< RADIXBITS >::value );
	_MIST_CONST( long, RADIX1,		RADIX - 1 );
	_MIST_CONST( long, RADIX_2,		RADIX / 2 );
	_MIST_CONST( long, RADIX2,		RADIX * RADIX );
	_MIST_CONST( long, MAXEXP,		16383 );
	_MIST_CONST( long, MINEXP,		-16384 );
	_MIST_CONST( long, MAX_K,		180 );			// < (sqrt(4 * RADIX + 1) - 1) * 0.5 */
	_MIST_CONST( long, MAX_LONG,	0x7FFFFFFFL );
	_MIST_CONST( long, MIN_LONG,	0x80000000L );

	_MIST_CONST( long, NMPA,	MAX_KETA );
	_MIST_CONST( long, NMPA1,	NMPA + 1 );
	_MIST_CONST( long, NMPA2,	NMPA1 * 2 );
	_MIST_CONST( long, MMPA,	NMPA1 * __decimal_util__::log10< RADIX >::value / 4 );

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

			size_type i = 0;
			do
			{
				data_[ i ] = static_cast< unsigned int >( a );
				a -= data_[ i ];
				a *= RADIX;
			} while( a != 0.0 && ++i <= NMPA );
			for( i++ ; i <= NMPA ; i++ )
			{
				data_[ i - 1 ] = 0;
			}
		}
	}

	decimal( difference_type n ) : sign_( true ), exp_( 0 ), zero_( true )
	{
		memset( data_, 0, sizeof( value_type ) * NMPA1 );

		if( n != 0 )
		{
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

	const decimal &operator *=( decimal a )
	{
		if( zero_ )
		{
			return( *this );
		}
		else if( a.zero_ )
		{
			return( operator =( decimal( ) ) );
		}

		difference_type i, j, u;
		value_type x[ NMPA2 ];
		value_type *p, *q, *r, *xp;
		memset( x, 0, sizeof( value_type ) * NMPA2 );

		for( i = NMPA, xp = x + NMPA2 - 1, p = a.data_ + i ; i >= 0 ; i--, xp--, p-- )
		{
			if( *p )
			{
				u = 0;
				for( j = NMPA, q = data_ + j, r = xp ; j >= 0; j-- )
				{
					u += static_cast< size_type >( *q-- ) * static_cast< size_type >( *p ) + *r;
					*r-- = static_cast< value_type >( u ) & RADIX1;
					u >>= RADIXBITS;
				}
				while( u )
				{
					u += *r;
					*r-- = static_cast< value_type >( u ) & RADIX1;
					u >>= RADIXBITS;
				}
			}
		}
		sign_ = sign_ == a.sign_;
		exp_ += ++a.exp_;
		for( i = 0, xp = x ; i < NMPA2 ; i++, xp++ )
		{
			if( *xp )
			{
				break;
			}
			exp_--;
		}
		if( exp_ > MAXEXP )
		{
			::std::cout << "Error : Overflow!!" << ::std::endl;
			//*a = _MMAX;
			return( operator =( decimal ( ) ) );
		}
		if( exp_ < MINEXP )
		{
			::std::cout << "Error : Underflow!!" << ::std::endl;
			return( operator =( decimal( ) ) );
		}
		for( j = 0, p = data_ ; ( j <= NMPA ) && ( i < NMPA2 ) ; i++, j++ )
		{
            *p++ = *xp++;
		}
		if( j <= NMPA )
		{
			for( ; j <= NMPA ; j++ )
			{
				*p++ = 0;
			}
		}
		else if( *xp >= RADIX_2 )
		{
			for( i = NMPA, p = data_ + i ; ++*p & RADIX ; i-- )
			{
				*p-- &= RADIX1;
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
				::std::cout << "Error : Overflow!!" << ::std::endl;
				//*a = _MMAX;
				return( operator =( decimal( ) ) );
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
		if( b.zero_ )
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
			//*a = _MMAX;
			return( operator =( decimal( ) ) );
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

	const decimal &operator %=( const decimal &a )
	{
		return( operator =( *this - ( *this / a ) ) );
	}

	const decimal &operator %=( difference_type n )
	{
		return( operator =( *this - ( *this / n ) ) );
	}


	const decimal &operator &=( const decimal &a )
	{
		return( *this );
	}

	const decimal &operator |=( const decimal &a )
	{
		return( *this );
	}

	const decimal &operator ^=( const decimal &a )
	{
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


	bool operator ==( const decimal &a ) const { return( sign_ == a.sign_ && acmp( *this, a ) == 0 ); }
	bool operator !=( const decimal &a ) const { return( !( *this == a ) ); }
	bool operator < ( const decimal &a ) const { return( !( *this >= a ) ); }
	bool operator <=( const decimal &a ) const { return( a >= *this ); }
	bool operator > ( const decimal &a ) const { return( a < *this ); }
	bool operator >=( const decimal &a ) const
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
			return( acmp( *this, a ) >= 0 );
		}
	}

	const ::std::string to_string( ) const
	{
		difference_type nmpa = NMPA;
		difference_type mmpa = MMPA - 1;

		//if( mmpa < ::std::abs( exp_ ) )
		//{
		//	//m_printe(s, a, _short);
		//	return( "0" );
		//}

		if( zero_ )
		{
			return( "0" );
		}

		unsigned int c[ MMPA ], t[ NMPA1 ];
		unsigned int *cp, *p, *q, w;
		difference_type i;

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

		cp = c;
		difference_type n = a.exp_, u, m = 0, mp = -1;
		if( n >= 0 )
		{
			for( i = 0, p = a.data_, q = t; i <= n; i++ )
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
				*cp++ = static_cast< value_type >( u );
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
			for( p = c, q = c + m - 1 ; p < q ; p++, q-- )
			{
				w = *p;
				*p = *q;
				*q = w;
			}
			for( i = n + 1, p = a.data_, q = a.data_ + i ; i <= nmpa ; i++ )
			{
				*p++ = *q++;
			}
			for( i = 0 ; i <= n ; i++)
			{
				*p++ = 0;
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
					*cp++ = a.data_[ 0 ];
					m++;
					break;
				}
			}
			for( i = 1, p = a.data_, q = p + 1 ; i <= nmpa ; i++ )
			{
				*p++ = *q++;
			}
			*p = 0;
			a.exp_ = -1;
		}
		a *= 10000;
		m++;
		if( a.exp_ == 0 )
		{
			*cp++ = a.data_[ 0 ];
			a.data_[ 0 ] = 0;
		}
		else
		{
			*cp++ = 0;
		}
		while( m <= mmpa )
		{
			if( a.is_zero( ) )
			{
				a = decimal( );
				break;
			}
			a *= 10000;
			*cp++ = a.data_[ 0 ];
			m++;
			a.data_[ 0 ] = 0;
		}
		if( *( c + mmpa ) < 5000 )
		{
			*( c + mmpa ) = 0;
		}
		else
		{
			*( c + mmpa ) = 0;
			for( m = mmpa - 1, cp = c + m ; m >= 0 ; m-- )
			{
				(*cp)++;
				if( *cp != 10000 )
				{
					break;
				}
				*cp-- = 0;
			}
		}
		for( cp = c + mmpa, p = c ; cp >= p ; )
		{
			if( *cp-- )
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
		cp = c;

		char str[ 20 ];
		if( mp < 0 )
		{
			sprintf( str, "%04u", *cp++ );
			s += str;
		}
		else
		{
			sprintf( str, "%u", *cp++ );
			s += str;
			if( !mp )
			{
				s += ".";
			}
		}

		for( m = 1 ; m <= mmpa ; m++ )
		{
			sprintf( str, "%04u", *cp++ );
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

	//int to_int32( ) const
	//{
	//	if( length_ == 0 )
	//	{
	//		return( 0 );
	//	}
	//	else if( length_ > 4 )
	//	{
	//		// オーバーフロー
	//		std::cerr << "overflow!!" << std::endl;
	//		return( 0 );
	//	}
	//	else
	//	{
	//		return( ( sign_ ? 1 : -1 ) * ( data_[ 0 ] + BASE * ( data_[ 1 ] + BASE * ( data_[ 2 ] + BASE * data_[ 3 ] ) ) ) );
	//	}
	//}

	//int to_uint32( ) const
	//{
	//	if( length_ == 0 )
	//	{
	//		return( 0 );
	//	}
	//	else if( length_ > 4 )
	//	{
	//		// オーバーフロー
	//		std::cerr << "overflow!!" << std::endl;
	//		return( 0 );
	//	}
	//	else
	//	{
	//		return( data_[ 0 ] + BASE * ( data_[ 1 ] + BASE * ( data_[ 2 ] + BASE * data_[ 3 ] ) ) );
	//	}
	//}

	bool is_zero( ) const
	{
		if( zero_ )
		{
			return( true );
		}
		for( size_type i = 0 ; i <= NMPA ; i++ )
		{
			if( data_[ i ] )
			{
				return( false );
			}
		}
		return( true );
	}

protected:
	static int acmp( decimal a, decimal b )
	{
		if( a.zero_ )
		{
			return( b.zero_ ? 0 : -1 );
		}
		else if( b.zero_ )
		{
			return( 1 );
		}

		size_type i;

		while( a.data_[ 0 ] == 0 && a.exp_ != MINEXP )
		{
			for( i = 0 ; i < NMPA ; i++ )
			{
				a.data_[ i ] = a.data_[ i + 1 ];
			}
			a.data_[ i ] = 0;
			a.exp_--;
		}
		while( b.data_[ 0 ] == 0 && b.exp_ != MINEXP )
		{
			for( i = 0 ; i < NMPA ; i++ )
			{
				b.data_[ i ] = b.data_[ i + 1 ];
			}
			b.data_[ i ] = 0;
			b.exp_--;
		}
		if( a.exp_ != b.exp_ )
		{
			return( a.exp_ > b.exp_ ? 1: -1 );
		}
		for( i = 0 ; i <= NMPA ; i++ )
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
			b.data_[ i ] = b.data_[ NMPA + i - n ];
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
			::std::cout << "Error : Overflow!!" << ::std::endl;
			a = decimal( );
			//*a = _MMAX;
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
			for( i = NMPA ; ++( a.data_[ NMPA + i ] ) & RADIX ; i-- )
			{
				a.data_[ i ] &= RADIX1;
			}
		}
		a.sign_ = sign;
	}

	static void asub( decimal &a, decimal b )
	{
		bool sign = a.sign_;
		difference_type i = aprs( a, b );
		if( i == -999 )
		{
			return;
		}

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
		for( i = 0 ; i < m ; i++ )
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
					::std::cout << "Error : Illegal parameter!!" << ::std::endl;
					return decimal( );
				}
				pflag = true;
			}
			else if( *p == 'E' || *p == 'e' )
			{
				p++;
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
				::std::cout << "Error : Illegal parameter!!" << ::std::endl;
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
						::std::cout << "Error : Illegal parameter!!" << ::std::endl;
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
			::std::cout << "Error : Overflow!!" << ::std::endl;
			return decimal( );
//			return _MMAX;
		}
		if( exp < MINEXP )
		{
			::std::cout << "Error : Underflow!!" << ::std::endl;
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
//template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator %( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) %= v2 ); }
//template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator |( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) |= v2 ); }
//template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator &( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) &= v2 ); }
//template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator ^( const decimal< MAX_KETA > &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) ^= v2 ); }
//
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator *( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) *= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator *( const typename decimal< MAX_KETA >::difference_type &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v2 ) *= v1 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator /( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) /= v2 ); }
//
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator +( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) += v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator +( const typename decimal< MAX_KETA >::difference_type &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v2 ) += v1 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator -( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) -= v2 ); }
template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator -( const typename decimal< MAX_KETA >::difference_type &v1, const decimal< MAX_KETA > &v2 ){ return( decimal< MAX_KETA >( v1 ) -= v2 ); }
//
//template < unsigned int MAX_KETA > inline const decimal< MAX_KETA > operator %( const decimal< MAX_KETA > &v1, const typename decimal< MAX_KETA >::difference_type &v2 ){ return( decimal< MAX_KETA >( v1 ) %= v2 ); }


template < unsigned int MAX_KETA >
inline std::ostream &operator <<( std::ostream &out, const decimal< MAX_KETA > &a )
{
	out << a.to_string( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DECIMAL_H__
