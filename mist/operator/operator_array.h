#ifndef __INCLUDE_MIST_OPERATOR_ARRAY__
#define __INCLUDE_MIST_OPERATOR_ARRAY__

//#define _CHECK_ARRAY_OPERATION_


template < class T, class Allocator >
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// ‘«‚µZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't add arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// ˆø‚«Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't subtract arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// Š|‚¯Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't multiply arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// Š„‚èZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't divide arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( val == 0 )
	{
		// ƒ[ƒœZ”­¶
		::std::cout << "zero division occured." << ::std::endl;
		return;
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= val;
	return( a1 );
}


// •„†”½“]
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	array< T, Allocator > o( a.size( ) );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -a[i];
	return( o );
}


// ‘«‚µZ
template < class T, class Allocator >
inline array< T, Allocator > operator +( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) += a2 );
}


// ˆø‚«Z
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) -= a2 );
}


// Š|‚¯Z
template < class T, class Allocator >
inline array< T, Allocator > operator *( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) *= a2 );
}

// Š„‚èZ
template < class T, class Allocator >
inline array< T, Allocator > operator /( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) /= a2 );
}

// ’è”‚Æ‚Ì‘«‚µZ
template < class T, class Allocator >
inline array< T, Allocator > operator +( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) += val );
}

template < class T, class Allocator >
inline array< T, Allocator > operator +( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) += val );
}



// ’è”‚Æ‚Ìˆø‚«
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) -= val );
}

template < class T, class Allocator >
inline array< T, Allocator > operator -( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) -= val );
}


// ’è”‚Æ‚ÌŠ|‚¯Z
template < class T, class Allocator >
inline array< T, Allocator > operator *( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) *= val );
}

template < class T, class Allocator >
inline array< T, Allocator > operator *( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) *= val );
}


// ’è”‚Æ‚ÌŠ„‚è
template < class T, class Allocator >
inline array< T, Allocator > operator /( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) /= val );
}

#endif // __INCLUDE_MIST_OPERATOR_ARRAY__