#ifndef __INCLUDE_MIST_OPERATOR_ARRAY1__
#define __INCLUDE_MIST_OPERATOR_ARRAY1__


template < class T, class Allocator >
inline const array1< T, Allocator >& operator +=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// ‘«‚µZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't add array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator -=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// ˆø‚«Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't subtract array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator *=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// Š|‚¯Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't multiply array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator /=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// Š„‚èZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't divide array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator +=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator -=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator *=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array1< T, Allocator >& operator /=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#ifndef _CHECK_ARRAY_OPERATION_
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
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	array1< T, Allocator > o( a.size1( ), a.reso1( ) );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -a[i];
	return( o );
}

// ‘«‚µZ
template < class T, class Allocator >
inline array1< T, Allocator > operator +( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) += a2 );
}


// ˆø‚«Z
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) -= a2 );
}


// Š|‚¯Z
template < class T, class Allocator >
inline array1< T, Allocator > operator *( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) *= a2 );
}

// Š„‚èZ
template < class T, class Allocator >
inline array1< T, Allocator > operator /( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) /= a2 );
}

// ’è”‚Æ‚Ì‘«‚µZ
template < class T, class Allocator >
inline array1< T, Allocator > operator +( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) += val );
}

template < class T, class Allocator >
inline array1< T, Allocator > operator +( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &a )
{
	return( array1< T, Allocator >( a ) += val );
}



// ’è”‚Æ‚Ìˆø‚«
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) -= val );
}

template < class T, class Allocator >
inline array1< T, Allocator > operator -( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &a )
{
	return( array1< T, Allocator >( a ) -= val );
}


// ’è”‚Æ‚ÌŠ|‚¯Z
template < class T, class Allocator >
inline array1< T, Allocator > operator *( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) *= val );
}

template < class T, class Allocator >
inline array1< T, Allocator > operator *( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &a )
{
	return( array1< T, Allocator >( a ) *= val );
}


// ’è”‚Æ‚ÌŠ„‚è
template < class T, class Allocator >
inline array1< T, Allocator > operator /( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) /= val );
}


#endif // __INCLUDE_MIST_OPERATOR_ARRAY1__