#ifndef __INCLUDE_MIST_OPERATOR_ARRAY3__
#define __INCLUDE_MIST_OPERATOR_ARRAY3__


template < class T, class Allocator >
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
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
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
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
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �|���Z�ł��܂����O
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
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// ����Z�ł��܂����O
		::std::cout << "can't divide array3s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator -=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator *=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}

template < class T, class Allocator >
inline const array3< T, Allocator >& operator /=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
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
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	array3< T, Allocator > o( a.size1( ), a.size2( ), a.size3( ), a.reso1( ), a.reso2( ), a.reso3( ) );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -a[i];
	return( o );
}


// �����Z
template < class T, class Allocator >
inline array3< T, Allocator > operator +( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) += a2 );
}


// �����Z
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) -= a2 );
}


// �|���Z
template < class T, class Allocator >
inline array3< T, Allocator > operator *( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) *= a2 );
}

// ����Z
template < class T, class Allocator >
inline array3< T, Allocator > operator /( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) /= a2 );
}

// �萔�Ƃ̑����Z
template < class T, class Allocator >
inline array3< T, Allocator > operator +( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) += val );
}

template < class T, class Allocator >
inline array3< T, Allocator > operator +( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) += val );
}



// �萔�Ƃ̈���
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) -= val );
}

template < class T, class Allocator >
inline array3< T, Allocator > operator -( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) -= val );
}


// �萔�Ƃ̊|���Z
template < class T, class Allocator >
inline array3< T, Allocator > operator *( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) *= val );
}

template < class T, class Allocator >
inline array3< T, Allocator > operator *( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) *= val );
}


// �萔�Ƃ̊���
template < class T, class Allocator >
inline array3< T, Allocator > operator /( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) /= val );
}



#endif // __INCLUDE_MIST_OPERATOR_ARRAY3__
