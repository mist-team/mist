#ifndef __INCLUDE_MIST_OPERATOR_ARRAY3__
#define __INCLUDE_MIST_OPERATOR_ARRAY3__

//#define _CHECK_ARRAY3_OPERATION_

template < class T, class Allocator >
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// ‘«‚µZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't add array3s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator -=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// ˆø‚«Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't subtract array3s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator *=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// Š|‚¯Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't multiply array3s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator /=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// Š„‚èZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't divide array3s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator -=( array3< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator *=( array3< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator /=( array3< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY3_OPERATION_
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
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	array3< T, Allocator > o( a );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -o[i];
	return( o );
}


// ‘«‚µZ
template < class T, class Allocator >
inline array3< T, Allocator > operator +( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) || a1.size3( ) != a2.size3( ) )
	{
		// ‘«‚µZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't add arrays." << ::std::endl;
		return( a1 );
	}
#endif
	array3< T, Allocator > a = a1;
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += a2[i];
	return( a );
}


// ˆø‚«Z
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) || a1.size3( ) != a2.size3( ) )
	{
		// ˆø‚«Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't subtract arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array3< T, Allocator > a = a1;
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] -= a2[i];
	return( a );
}


// Š|‚¯Z
template < class T, class Allocator >
inline array3< T, Allocator > operator *( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) || a1.size3( ) != a2.size3( ) )
	{
		// Š|‚¯Z‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't multiply arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array3< T, Allocator > a( a1 );
	typename array3< T, Allocator >::size_type i;

	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= a2[i];

	return( a );
}

// Š„‚èZ
template < class T, class Allocator >
inline array3< T, Allocator > operator /( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY3_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) || a1.size3( ) != a2.size3( ) )
	{
		// Š„‚èZ‚Å‚«‚Ü‚¹‚ñ—áŠO
		::std::cout << "can't divide arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array3< T, Allocator > a( a1 );
	typename array3< T, Allocator >::size_type i;

	for( i = 0 ; i < a.size( ) ; i++ ) a[i] /= a2[i];

	return( a );
}

// ’è”‚Æ‚Ì‘«‚µZ
template < class T, class Allocator >
inline array3< T, Allocator > operator +( const array3< T, Allocator > &m, typename array3< T, Allocator >::value_type val )
{
	array3< T, Allocator > a( m );
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += val;

	return( a );
}

template < class T, class Allocator >
inline array3< T, Allocator > operator +( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &m )
{
	array3< T, Allocator > a( m );
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += val;

	return( a );
}



// ’è”‚Æ‚Ìˆø‚«Z
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &m, typename array3< T, Allocator >::value_type val )
{
	array3< T, Allocator > a( m );
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] -= val;

	return( a );
}

template < class T, class Allocator >
inline array3< T, Allocator > operator -( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &m )
{
	array3< T, Allocator > a( m.size1( ), m.size2( ), m.size3( ), m.reso1( ), m.reso2( ), m.reso3( ) );
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] = val - a[i];

	return( a );
}


// ’è”‚Æ‚ÌŠ|‚¯Z
template < class T, class Allocator >
inline array3< T, Allocator > operator *( const array3< T, Allocator > &m, typename array3< T, Allocator >::value_type val )
{
	array3< T, Allocator > a( m );
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= val;

	return( a );
}

template < class T, class Allocator >
inline array3< T, Allocator > operator *( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &m )
{
	array3< T, Allocator > a( m );
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= val;

	return( a );
}


// ’è”‚Æ‚ÌŠ„‚è
template < class T, class Allocator >
inline array3< T, Allocator > operator /( const array3< T, Allocator > &m, typename array3< T, Allocator >::value_type val )
{
#ifdef _CHECK_ARRAY3_OPERATION_
	if( val == 0 )
	{
		// ƒ[ƒœZ”­¶
		::std::cout << "zero division occured." << ::std::endl;
		return( a1 );
	}
#endif

	array3< T, Allocator > a( m );
	typename array3< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] /= val;

	return( a );
}



#endif // __INCLUDE_MIST_OPERATOR_ARRAY3__