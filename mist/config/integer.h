#ifndef __INCLUDE_MIST_INTEGER_H__
#define __INCLUDE_MIST_INTEGER_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <string>


// mist名前空間の始まり
_MIST_BEGIN


namespace __integer_util__
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
}


template < unsigned int MAX_KETA, class T = int, unsigned int BASE = 10000 >
struct integer
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T value_type;

private:
	_MIST_CONST( unsigned int, DATA_NUM, MAX_KETA / __integer_util__::log10< BASE >::value + 1 );

protected:
	size_t		length_;
	bool		sign_;
	value_type	data_[ DATA_NUM ];

public:
	integer( ) : length_( 0 ), sign_( true )
	{
		memset( data_, 0, sizeof( value_type ) * DATA_NUM );
	}

	integer( value_type v ) : length_( 0 ), sign_( true )
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
				data_[ length_++ ] = v % BASE;
				v /= BASE;
			} while( v > 0 );
		}
	}

	integer( const integer &v ) : length_( v.length_ ), sign_( v.sign_ )
	{
		memcpy( data_, v.data_, sizeof( value_type ) * DATA_NUM );
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
			integer x;
			if( sign_ == a.sign_ )
			{
				if( acmp( *this, a ) >= 0 )
				{
					x = aadd( *this, a );
				}
				else
				{
					x = aadd( a, *this );
				}

				x.sign_ = sign_;
			}
			else
			{
				if( acmp( *this, a ) >= 0 )
				{
					x = asub( *this, a );
					x.sign_ = sign_;
				}
				else
				{
					x = asub( a, *this );
					x.sign_ = !sign_;
				}
			}
			return ( operator =( x ) );
		}
	}

	const integer &operator -=( const integer &a )
	{
		return( operator +=( -a ) );
	}

	const integer &operator *=( const integer &a )
	{
		if( length_ == 0 )
		{
			return( *this );
		}
		else if( a.length_ == 0 )
		{
			return( operator =( integer( 0 ) ) );
		}
		if( length_ == 1 )
		{
			value_type t = sign_ ? data_[ 0 ] : -data_[ 0 ];
			return( operator =( integer( a ) *= t ) );
		}
		else if( a.length_ == 1 )
		{
			value_type t = a.sign_ ? a.data_[ 0 ] : -a.data_[ 0 ];
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

		for( size_type j = 0 ; j < a.length_ ; j++ )
		{
			value_type u = a.data_[ j ];
			if( u != 0 )
			{
				value_type t = 0;
				size_type imax = length_ + j - 1;
				for( size_type i = j ; i < imax ; i++ )
				{
					t += ( x.data_[ i ] + u * data_[ j ] );
					x.data_[ i ] = t % BASE;
					t /= BASE;
				}
				t += x.data_[ i ];
				while( t >= BASE )
				{
					x.data_[ i++ ] = t - BASE;
					t = x.data_[ i ] + 1;
				}
				x.data_[ i ] = t;
			}
		}
		if( x.data_[ length_ + a.length_ - 2 ] == 0 )
		{
			x.length_ = length_ + a.length_ - 2;
		}
		else
		{
			x.length_ = length_ + a.length_;
		}
		return( operator =( x ) );
	}

	const integer &operator *=( value_type n )
	{
		if( length_ == 0 || n == 0 )
		{
			return( operator =( integer( 0 ) ) );
		}
		else if( ::std::abs( n ) >= BASE )
		{
			return( operator *=( integer( n ) ) );
		}

		if( n < 0 )
		{
			sign_ = !sign_;
			n = - n;
		}

		value_type t = 0;
		for( size_type i = 0 ; i < length_ ; i++ )
		{
			t += data_[ i ] * n;
			data_[ i ] = t % BASE;
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
			data_[ length_ - 1 ] = t % BASE;
			t /= BASE;
		}
		return( *this );
	}

	bool operator ==( const integer &a ) const { return( sign_ == a.sign_ && acmp( *this, a ) == 0 ); }
	bool operator !=( const integer &a ) const { return( !( *this == c ) ); }
	bool operator < ( const integer &a ) const { return( !( *this >= c ) ); }
	bool operator <=( const integer &a ) const { return( c >= *this ); }
	bool operator > ( const integer &a ) const { return( c < *this ); }
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
			return( acmp( *this, a ) >= 0 );
		}
	}

	const ::std::string to_string( ) const
	{
		if( length_ == 0 )
		{
			return( "0" );
		}

		::std::string s = sign_ ? "" : "-";
		char tmp[ 20 ];
		static char format[ 20 ];
		sprintf( tmp, "%d", data_[ length_ - 1 ] % BASE );
		s += tmp;

		sprintf( format, "%%0%dd", __integer_util__::log10< BASE >::value );

		for( difference_type i = length_ - 2 ; i >= 0 ; i-- )
		{
			sprintf( tmp, format, data_[ i ] );
			s += tmp;
		}
		return( s );
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
				if( a.data_[ i ] != b.data_[ i ] )
				{
					break;
				}
			}

			if( i < 0 )
			{
				return 0;
			}
			else
			{
				return ( a.data_[ i ] > b.data_[ i ] ? 1 : -1 );
			}
		}
	}

	static const integer aadd( const integer &a, const integer &b )
	{
		if( acmp( a, b ) < 0 )
		{
			return( aadd( b, a ) );
		}

		size_type i;
		integer x( 0 );

		value_type t = 0;

		for( i = 0 ; i < b.length_ ; i++ )
		{
			t += a.data_[ i ] + b.data_[ i ];

			if( t < BASE )
			{
				x.data_[ i ] = t;
				t = 0;
			}
			else
			{
				x.data_[ i ] = t - BASE;
				t = 1;
			}
		}

		while( ( i < a.length_ ) && ( t != 0 ) )
		{
			t += a.data_[ i ];

			if( t < BASE )
			{
				x.data_[ i ] = t;
				t = 0;
			}
			else
			{
				x.data_[ i ] = t - BASE;
				t = 1;
			}

			i++;
		}

		for( ; i < a.length_ ; i++ )
		{
			x.data_[ i ] = a.data_[ i ];
		}

		if( t == 0 )
		{
			x.length_ = a.length_;
		}
		else
		{
			if( a.length_ <= DATA_NUM )
			{
				x.length_ = a.length_ + 1;
				x.data_[ i ] = 1;
			}
			else
			{
				std::cerr << "overflow!!" << std::endl;
			}
		}
		return( x );
	}

	static const integer asub( const integer &a, const integer &b )
	{
		int c = acmp( a, b );

		if( c < 0 )
		{
			return( asub( b, a ) );
		}
		else if( c == 0 )
		{
			return( integer( 0 ) );
		}

		size_type i;
		integer x( 0 );

		value_type t = 0;

		for( i = 0 ; i < b.length_ ; i++ )
		{
			t = a.data_[ i ] - b.data_[ i ] - t;

			if( t >= 0 )
			{
				x.data_[ i ] = t;
				t = 0;
			}
			else
			{
				x.data_[ i ] = t + BASE;
				t = 1;
			}
		}

		while( ( i < a.length_ ) && ( t != 0 ) )
		{
			t = a.data_[ i ] - t;

			if( t >= 0 )
			{
				x.data_[ i ] = t;
				t = 0;
			}
			else
			{
				x.data_[ i ] = t + BASE;
				t = 1;
			}

			i++;
		}

		for( ; i < a.length_ ; i++ )
		{
			x.data_[ i ] = a.data_[ i ];
		}

		x.length_ = a.length_;

		while( x.data_[ x.length_ - 1 ] == 0 )
		{
			x.length_--;
		}

		return( x );
	}
};

template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator +( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) += v2 ); }
template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator -( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) -= v2 ); }
template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator *( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) *= v2 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator /( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) /= v2 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator %( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) %= v2 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator |( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) |= v2 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator &( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) &= v2 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator ^( const integer< MAX_KETA, T, BASE > &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) ^= v2 ); }
//
template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator *( const integer< MAX_KETA, T, BASE > &v1, const typename integer< MAX_KETA, T, BASE >::value_type &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) *= v2 ); }
template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator *( const typename integer< MAX_KETA, T, BASE >::value_type &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v2 ) *= v1 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator /( const integer< MAX_KETA, T, BASE > &v1, const typename integer< MAX_KETA, T, BASE >::value_type &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) /= v2 ); }
//
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator +( const integer< MAX_KETA, T, BASE > &v1, const typename integer< MAX_KETA, T, BASE >::value_type &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) += v2 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator +( const typename integer< MAX_KETA, T, BASE >::value_type &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v2 ) += v1 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator -( const integer< MAX_KETA, T, BASE > &v1, const typename integer< MAX_KETA, T, BASE >::value_type &v2 ){ return( integer< MAX_KETA, T, BASE >( v1 ) -= v2 ); }
//template < unsigned int MAX_KETA, class T, unsigned int BASE > inline const integer< MAX_KETA, T, BASE > operator -( const typename integer< MAX_KETA, T, BASE >::value_type &v1, const integer< MAX_KETA, T, BASE > &v2 ){ return( integer< MAX_KETA, T, BASE >( v2 ) -= v1 ); }


template < unsigned int MAX_KETA, class T, unsigned int BASE >
inline std::ostream &operator <<( std::ostream &out, const integer< MAX_KETA, T, BASE > &a )
{
	out << a.to_string( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_INTEGER_H__
