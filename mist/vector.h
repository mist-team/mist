#ifndef __INCLUDE_MIST_VECTOR__
#define __INCLUDE_MIST_VECTOR__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN

template < class T >
class vector3
{
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	value_type x;
	value_type y;
	value_type z;

	vector3( ) : x( 0 ), y( 0 ), z( 0 ){ }
	vector3( const value_type &xx, const value_type &yy, const value_type &zz ) : x( xx ), y( yy ), z( zz ){ }

	template < class TT >
	vector3( const vector3< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }

	vector3( const vector3< T > &v ) : x( v.x ), y( v.y ), z( v.z ){ }


	// オペレータのオーバーライド
	const vector3 &operator =( const vector3 &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		return ( *this );
	}

	vector3 operator -( ) const { return ( vector3( -x, -y, -z ) ); }

	const vector3 &operator +=( const vector3 &v )
	{
		x = x + v.x;
		y = y + v.y;
		z = z + v.z;
		return( *this );
	}

	const vector3 &operator -=( const vector3 &v )
	{
		x = x - v.x;
		y = y - v.y;
		z = z - v.z;
		return( *this );
	}

	const vector3 &operator *=( const value_type &a )
	{
		x *= a;
		y *= a;
		z *= a;
		return( *this );
	}

	const vector3 &operator /=( const value_type &a )
	{
		x /= a;
		y /= a;
		z /= a;
		return( *this );
	}


	// 内積
	value_type operator ^( const vector3 &v ) const { return ( inner( v ) ); }


	// 比較関数
	bool operator ==( const vector3 &v ) const { return( x == v.x && y == v.y && z == v.z ); }
	bool operator !=( const vector3 &v ) const { return( !( *this == v ) ); }
	bool operator < ( const vector3 &v ) const { return( !( *this >= v ) ); }
	bool operator <=( const vector3 &v ) const { return( v >= *this ); }
	bool operator > ( const vector3 &v ) const { return( v < *this ); }
	bool operator >=( const vector3 &v ) const { return( x >= v.x && y >= v.y && z >= v.z ); }

	vector3 unit( ) const
	{
		value_type length_ = length( );
		return vector3( x / length_, y / length_, z / length_ );
	}

	value_type inner( const vector3 &v ) const { return( x * v.x + y * v.y + z * v.z ); }
	vector3 outer( const vector3 &v ) const { return( vector3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ) ); }

	value_type length( ) const { return ( value_type( sqrt( (double)( x * x + y * y + z * z ) ) ) ); }

	// ベクトルの回転
	vector3 rotate( const vector3 &a, double theta ) const
	{
		theta *= 3.1415926535897932384626433832795 / 180.0;
		double cs = std::cos( theta ), sn = std::sin( theta );
		value_type xx = static_cast< value_type >( ( v.x * v.x * ( 1.0 - cs ) + cs ) * x + ( v.x * v.y * ( 1.0 - cs ) - v.z * sn ) * y + ( v.x * v.z * ( 1.0 - cs ) + v.y * sn ) * z );
		value_type yy = static_cast< value_type >( ( v.x * v.y * ( 1.0 - cs ) + v.z * sn ) * x + ( v.y * v.y * ( 1.0 - cs ) + cs ) * y + ( v.y * v.z * ( 1.0 - cs ) - v.x * sn ) * z );
		value_type zz = static_cast< value_type >( ( v.x * v.z * ( 1.0 - cs ) - v.y * sn ) * x + ( v.y * v.z * ( 1.0 - cs ) + v.x * sn ) * y + ( v.z * v.z * ( 1.0 - cs ) + cs ) * z );
		return ( vector3( xx, yy, zz ) );
	}
};

template < class T > inline const vector3< T > operator +( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1 ) += v2 ); }
template < class T > inline const vector3< T > operator -( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1 ) -= v2 ); }
template < class T > inline const vector3< T > operator /( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1 ) /= v2 ); }
template < class T > inline const vector3< T > operator *( const vector3< T > &v1, const vector3< T > &v2 ){ return( v1.outer( v2 ) ); }
template < class T > inline const typename vector3< T >::value_type operator ^( const vector3< T > &v1, const vector3< T > &v2 ){ return( v1.inner( v2 ) ); }

template < class T > inline const vector3< T > operator *( const vector3< T > &v1, const typename vector3< T >::value_type &v2 ){ return( vector3< T >( v1 ) *= v2 ); }
template < class T > inline const vector3< T > operator *( const typename vector3< T >::value_type &v1, const vector3< T > &v2 ){ return( vector3< T >( v2 ) *= v1 ); }
template < class T > inline const vector3< T > operator /( const vector3< T > &v1, const typename vector3< T >::value_type &v2 ){ return( vector3< T >( v1 ) /= v2 ); }

template < class T > inline std::ostream &operator <<( std::ostream &out, const vector3< T > &v )
{
	out << "( ";
	out << v.x << ", ";
	out << v.y << ", ";
	out << v.z << " )";
	return( out );
}



template < class T >
class vector2
{
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	value_type x;
	value_type y;

	vector2( ) : x( 0 ), y( 0 ){ }
	vector2( const value_type &xx, const value_type &yy ) : x( xx ), y( yy ){ }

	template < class TT >
	vector2( const vector2< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ){ }

	vector2( const vector2< T > &v ) : x( v.x ), y( v.y ){ }


	// オペレータのオーバーライド
	const vector2 &operator =( const vector2 &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
		}
		return ( *this );
	}

	vector2 operator -( ) const { return ( vector2( -x, -y ) ); }

	const vector2 &operator +=( const vector2 &v )
	{
		x = x + v.x;
		y = y + v.y;
		return( *this );
	}

	const vector2 &operator -=( const vector2 &v )
	{
		x = x - v.x;
		y = y - v.y;
		return( *this );
	}

	const vector2 &operator *=( const value_type &a )
	{
		x *= a;
		y *= a;
		return( *this );
	}

	const vector2 &operator /=( const value_type &a )
	{
		x /= a;
		y /= a;
		return( *this );
	}


	// 内積
	value_type operator ^( const vector2 &v ) const { return ( inner( v ) ); }


	// 比較関数
	bool operator ==( const vector2 &v ) const { return( x == v.x && y == v.y ); }
	bool operator !=( const vector2 &v ) const { return( !( *this == v ) ); }
	bool operator < ( const vector2 &v ) const { return( !( *this >= v ) ); }
	bool operator <=( const vector2 &v ) const { return( v >= *this ); }
	bool operator > ( const vector2 &v ) const { return( v < *this ); }
	bool operator >=( const vector2 &v ) const { return( x >= v.x && y >= v.y ); }

	vector2 unit( ) const
	{
		value_type length_ = length( );
		return vector2( x / length_, y / length_ );
	}

	value_type inner( const vector2 &v ) const { return( x * v.x + y * v.y ); }
	value_type outer( const vector2 &v ) const { return( x * v.y - y * v.x ); }

	value_type length( ) const { return ( value_type( sqrt( (double)( x * x + y * y ) ) ) ); }

	//// ベクトルの回転
	//vector2 rotate( const vector2 &a, double theta ) const
	//{
	//	theta *= 3.1415926535897932384626433832795 / 180.0;
	//	double cs = std::cos( theta ), sn = std::sin( theta );
	//	value_type xx = static_cast< value_type >( ( v.x * v.x * ( 1.0 - cs ) + cs ) * x + ( v.x * v.y * ( 1.0 - cs ) - v.z * sn ) * y + ( v.x * v.z * ( 1.0 - cs ) + v.y * sn ) * z );
	//	value_type yy = static_cast< value_type >( ( v.x * v.y * ( 1.0 - cs ) + v.z * sn ) * x + ( v.y * v.y * ( 1.0 - cs ) + cs ) * y + ( v.y * v.z * ( 1.0 - cs ) - v.x * sn ) * z );
	//	value_type zz = static_cast< value_type >( ( v.x * v.z * ( 1.0 - cs ) - v.y * sn ) * x + ( v.y * v.z * ( 1.0 - cs ) + v.x * sn ) * y + ( v.z * v.z * ( 1.0 - cs ) + cs ) * z );
	//	return ( vector2( xx, yy, zz ) );
	//}
};

template < class T > inline const vector2< T > operator +( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1 ) += v2 ); }
template < class T > inline const vector2< T > operator -( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1 ) -= v2 ); }
template < class T > inline const vector2< T > operator /( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1 ) /= v2 ); }
template < class T > inline const typename vector2< T >::value_type operator *( const vector2< T > &v1, const vector2< T > &v2 ){ return( v1.outer( v2 ) ); }
template < class T > inline const typename vector2< T >::value_type operator ^( const vector2< T > &v1, const vector2< T > &v2 ){ return( v1.inner( v2 ) ); }

template < class T > inline const vector2< T > operator *( const vector2< T > &v1, const typename vector2< T >::value_type &v2 ){ return( vector2< T >( v1 ) *= v2 ); }
template < class T > inline const vector2< T > operator *( const typename vector2< T >::value_type &v1, const vector2< T > &v2 ){ return( vector2< T >( v2 ) *= v1 ); }
template < class T > inline const vector2< T > operator /( const vector2< T > &v1, const typename vector2< T >::value_type &v2 ){ return( vector2< T >( v1 ) /= v2 ); }

template < class T > inline std::ostream &operator <<( std::ostream &out, const vector2< T > &v )
{
	out << "( ";
	out << v.x << ", ";
	out << v.y << ", ";
	out << v.z << " )";
	return( out );
}

// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_VECTOR__
