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
	vector3 rotate( const vector3 &v, double theta ) const
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



#if defined( _MIST_VECTOR_SUPPORT_ ) && _MIST_VECTOR_SUPPORT_ != 0


// 行列
template < class T, class Allocator = ::std::allocator< T > >
class vector : public matrix< T, Allocator >
{
private:
	typedef matrix< T, Allocator > base;

public:
	typedef typename base::allocator_type allocator_type;
	typedef typename base::reference reference;
	typedef typename base::const_reference const_reference;
	typedef typename base::value_type value_type;
	typedef typename base::size_type size_type;
	typedef typename base::difference_type difference_type;
	typedef typename base::pointer pointer;
	typedef typename base::const_pointer const_pointer;

	typedef typename base::iterator iterator;
	typedef typename base::const_iterator const_iterator;
	typedef typename base::reverse_iterator reverse_iterator;
	typedef typename base::const_reverse_iterator const_reverse_iterator;


/************************************************************************************************************
**
**      行列に対する演算子
**        += 行列
**        += 定数
**
**        -= 行列
**        -= 定数
**
**        *= 行列
**        *= 定数
**
**        /= 定数
**
************************************************************************************************************/
	//template < class TT, class AAlocator >
	//const vector& operator +=( const vector< TT, AAlocator > &v2 )
	//{
	//	base::operator +=( v2 );
	//	return( v1 );
	//}

	//template < class TT, class AAlocator >
	//const vector& operator -=( const vector< TT, AAlocator > &v2 )
	//{
	//	base::operator -=( v2 );
	//	return( v1 );
	//}

	template < class TT, class AAlocator >
	const vector& operator *=( const vector< TT, AAlocator > &v2 )
	{
		vector &v1 = *this;
		typedef typename vector< T, Allocator >::size_type size_type;
#ifdef _CHECK_MATRIX_OPERATION_
		if( base::size( ) != v2.size( ) )
		{
			// 外積の計算ができません例外
			::std::cout << "can't calculate outer product of two vectors." << ::std::endl;
			return( v1 );
		}
#endif

		vector v( v1.size( ) );

		for( size_type i = 0 ; i < ret.dim-2 ; ++i )
		{
			v[ i ] = v1[ i + 1 ] * v2[ i + 2 ] - v1[ i + 2 ] * v2[ i + 1 ];
		}

		v[ i ] = v1[ i + 1 ] * v2[ 0 ] - v1[ 0 ] * v2[ i + 1 ];
		v[ i + 1 ] = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];

		v1.swap( v );

		return( *this );
	}

	//const vector& operator *=( typename type_trait< T >::value_type val )
	//{
	//	base::operator *=( val );
	//	return( *this );
	//}

	//const vector& operator /=( typename type_trait< T >::value_type val )
	//{
	//	base::operator /=( val );
	//	return( *this );
	//}


public:
	// 構築
	vector( ) : base( ), size1_( 0 ), size2_( 0 ) {}
	explicit vector( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ) {}

	vector( size_type num ) : base( num, 1 ) {}
	vector( size_type num, const Allocator &a ) : base( num, 1, a ) {}

	vector( size_type num, const T &val ) : base( num, 1, val ) {}
	vector( size_type num, const T &val, const Allocator &a ) : base( num, 1, val, a ) {}

	template < class TT, class AAlocator >
	vector( const vector< TT, AAlocator > &o ) : base( o ){ }

	vector( const vector< T, Allocator > &o ) : base( o ){ }

#if _USE_EXPRESSION_TEMPLATE_ != 0
	template < class Expression >
	vector( const matrix_expression< Expression > &expression ) : base( expression.size( ), 1 )
	{
		vector &v = *this;
		for( size_type indx = 0 ; indx < v.size( ) ; indx++ )
		{
			v[ indx ] = expression[ indx ];
		}
	}
#endif
};



/************************************************************************************************************
**
** 標準出力への行列の出力
**
************************************************************************************************************/
template < class T, class Allocator >
inline ::std::ostream &operator <<( ::std::ostream &out, const vector< T, Allocator > &m )
{
	typename vector< T, Allocator >::size_type indx;
	for( indx = 0 ; indx < v.size( ) ; indx++ )
	{
		out << m[ indx ];
		if( indx != m.size( ) - 1 ) out << ", ";
	}
	out << ::std::endl;

	return( out );
}


//#if _USE_EXPRESSION_TEMPLATE_ != 0
//
//template < class T >
//inline ::std::ostream &operator <<( ::std::ostream &out,  const matrix_expression< T > &m )
//{
//	typedef typename matrix_expression< T >::allocator_type	allocator_type;
//	typedef typename matrix_expression< T >::value_type		value_type;
//	return( operator <<( out, vector< value_type, allocator_type >( m ) ) );
//}
//
//#endif

#endif // _MIST_VECTOR_SUPPORT_



// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_VECTOR__
