#ifndef __INCLUDE_MIST_OPERATOR_ARRAY2__
#define __INCLUDE_MIST_OPERATOR_ARRAY2__

//#define _CHECK_ARRAY2_OPERATION_

template < class T, class Allocator >
inline const array2< T, Allocator >& operator +=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't add array2s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array2< T, Allocator >& operator -=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't subtract array2s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array2< T, Allocator >& operator *=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// �|���Z�ł��܂����O
		::std::cout << "can't multiply array2s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array2< T, Allocator >& operator /=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size( ) != a2.size( ) )
	{
		// ����Z�ł��܂����O
		::std::cout << "can't divide array2s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array2< T, Allocator >& operator +=( array2< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}

template < class T, class Allocator >
inline const array2< T, Allocator >& operator -=( array2< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array2< T, Allocator >& operator *=( array2< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array2< T, Allocator >& operator /=( array2< T, Allocator > &a1, typename type_trait< T >::type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY2_OPERATION_
	if( val == 0 )
	{
		// �[�����Z����
		::std::cout << "zero division occured." << ::std::endl;
		return;
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= val;
	return( a1 );
}


// �������]
template < class T, class Allocator >
inline array2< T, Allocator > operator -( const array2< T, Allocator > &a )
{
	typedef typename array2< T, Allocator >::size_type size_type;
	array2< T, Allocator > o( a );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -o[i];
	return( o );
}

// �����Z
template < class T, class Allocator >
inline array2< T, Allocator > operator +( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't add arrays." << ::std::endl;
		return( a1 );
	}
#endif
	array2< T, Allocator > a = a1;
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += a2[i];
	return( a );
}


// �����Z
template < class T, class Allocator >
inline array2< T, Allocator > operator -( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't subtract arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array2< T, Allocator > a = a1;
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] -= a2[i];
	return( a );
}


// �|���Z
template < class T, class Allocator >
inline array2< T, Allocator > operator *( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) )
	{
		// �|���Z�ł��܂����O
		::std::cout << "can't multiply arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array2< T, Allocator > a( a1 );
	typename array2< T, Allocator >::size_type i;

	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= a2[i];

	return( a );
}

// ����Z
template < class T, class Allocator >
inline array2< T, Allocator > operator /( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
#ifdef _CHECK_ARRAY2_OPERATION_
	if( a1.size1( ) != a2.size1( ) || a1.size2( ) != a2.size2( ) )
	{
		// ����Z�ł��܂����O
		::std::cout << "can't divide arrays." << ::std::endl;
		return( a1 );
	}
#endif

	array2< T, Allocator > a( a1 );
	typename array2< T, Allocator >::size_type i;

	for( i = 0 ; i < a.size( ) ; i++ ) a[i] /= a2[i];

	return( a );
}

// �萔�Ƃ̑����Z
template < class T, class Allocator >
inline array2< T, Allocator > operator +( const array2< T, Allocator > &m, typename array2< T, Allocator >::value_type val )
{
	array2< T, Allocator > a( m );
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += val;

	return( a );
}

template < class T, class Allocator >
inline array2< T, Allocator > operator +( typename array2< T, Allocator >::value_type val, const array2< T, Allocator > &m )
{
	array2< T, Allocator > a( m );
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] += val;

	return( a );
}



// �萔�Ƃ̈���
template < class T, class Allocator >
inline array2< T, Allocator > operator -( const array2< T, Allocator > &m, typename array2< T, Allocator >::value_type val )
{
	array2< T, Allocator > a( m );
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] -= val;

	return( a );
}

template < class T, class Allocator >
inline array2< T, Allocator > operator -( typename array2< T, Allocator >::value_type val, const array2< T, Allocator > &m )
{
	array2< T, Allocator > a( m.size1( ), m.size2( ), m.reso1( ), m.reso2( ) );
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] = val - a[i];

	return( a );
}


// �萔�Ƃ̊|���Z
template < class T, class Allocator >
inline array2< T, Allocator > operator *( const array2< T, Allocator > &m, typename array2< T, Allocator >::value_type val )
{
	array2< T, Allocator > a( m );
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= val;

	return( a );
}

template < class T, class Allocator >
inline array2< T, Allocator > operator *( typename array2< T, Allocator >::value_type val, const array2< T, Allocator > &m )
{
	array2< T, Allocator > a( m );
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] *= val;

	return( a );
}


// �萔�Ƃ̊���
template < class T, class Allocator >
inline array2< T, Allocator > operator /( const array2< T, Allocator > &m, typename array2< T, Allocator >::value_type val )
{
#ifdef _CHECK_ARRAY2_OPERATION_
	if( val == 0 )
	{
		// �[�����Z����
		::std::cout << "zero division occured." << ::std::endl;
		return( a1 );
	}
#endif

	array2< T, Allocator > a( m );
	typename array2< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ ) a[i] /= val;

	return( a );
}



#endif // __INCLUDE_MIST_OPERATOR_ARRAY2__