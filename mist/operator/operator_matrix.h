#ifndef __INCLUDE_MIST_OPERATOR_MATRIX__
#define __INCLUDE_MIST_OPERATOR_MATRIX__


//#define _CHECK_MATRIX_OPERATION_

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator +=( matrix< T, Allocator > &m1, const matrix< T, Allocator >  &m2 )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( m1.size( ) != m2.size( ) )
	{
		// ë´ÇµéZÇ≈Ç´Ç‹ÇπÇÒó·äO
		::std::cout << "can't add matrixs." << ::std::endl;
		return( m1 );
	}
#endif
	for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] += static_cast< T >( m2[i] );
	return( m1 );
}

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator -=( matrix< T, Allocator > &m1, const matrix< T, Allocator >  &m2 )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( m1.size( ) != m2.size( ) )
	{
		// à¯Ç´éZÇ≈Ç´Ç‹ÇπÇÒó·äO
		::std::cout << "can't subtract matrixs." << ::std::endl;
		return( m1 );
	}
#endif
	for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] -= static_cast< T >( m2[i] );
	return( m1 );
}

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator *=( matrix< T, Allocator > &m1, const matrix< T, Allocator >  &m2 )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
#ifdef _CHECK_MATRIX_OPERATION_
	if( m1.cols( ) != m2.rows( ) )
	{
		// ä|ÇØéZÇ≈Ç´Ç‹ÇπÇÒó·äO
		::std::cout << "can't multiply matrices." << ::std::endl;
		return( m1 );
	}
#endif

	matrix< T, Allocator > mat( m1.rows( ), m2.cols( ) );
	size_type r, c, t;

	for( r = 0 ; r < mat.rows( ) ; r++ )
	{
		for( c = 0 ; c < mat.cols( ) ; c++ )
		{
			for( t = 0 ; t < m1.cols( ) ; t++ )
			{
				mat( r, c ) += m1( r, t ) * static_cast< T >( m2( t, c ) );
			}
		}
	}

	m1.swap( mat );

	return( m1 );
}

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator /=( matrix< T, Allocator > &m1, const matrix< T, Allocator >  &m2 )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( m1.size( ) != m2.size( ) )
	{
		// äÑÇËéZÇ≈Ç´Ç‹ÇπÇÒó·äO
		::std::cout << "can't divide matrixs." << ::std::endl;
		return( m1 );
	}
#endif
	for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] /= static_cast< T >( m2[i] );
	return( m1 );
}

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator +=( matrix< T, Allocator > &m1, typename type_trait< T >::type val )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] += val;
	return( m1 );
}

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator -=( matrix< T, Allocator > &m1, typename type_trait< T >::type val )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] -= val;
	return( m1 );
}

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator *=( matrix< T, Allocator > &m1, typename type_trait< T >::type val )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] *= val;
	return( m1 );
}

template < class T, class Allocator >
inline const matrix< T, Allocator >& operator /=( matrix< T, Allocator > &m1, typename type_trait< T >::type val )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
#ifdef _CHECK_ARRAY_OPERATION_
	if( val == 0 )
	{
		// É[ÉçèúéZî≠ê∂
		::std::cout << "zero division occured." << ::std::endl;
		return;
	}
#endif
	for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] /= val;
	return( m1 );
}


// ïÑçÜîΩì]
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	matrix< T, Allocator > o( m );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -o[i];
	return( o );
}

// ë´ÇµéZ
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
#ifdef _CHECK_MATRIX_OPERATION_
	if( m1.rows( ) != m2.rows( ) || m1.cols( ) != m2.cols( ) )
	{
		// ë´ÇµéZÇ≈Ç´Ç‹ÇπÇÒó·äO
		::std::cout << "can't add matrices." << ::std::endl;
		return( m1 );
	}
#endif
	matrix< T, Allocator > mat = m1;
	typename matrix< T, Allocator >::size_type i;
	for( i = 0 ; i < mat.size( ) ; i++ ) mat[i] += m2[i];
	return( mat );
}


// à¯Ç´éZ
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
#ifdef _CHECK_MATRIX_OPERATION_
	if( m1.rows( ) != m2.rows( ) || m1.cols( ) != m2.cols( ) )
	{
		// à¯Ç´éZÇ≈Ç´Ç‹ÇπÇÒó·äO
		::std::cout << "can't subtract matrices." << ::std::endl;
		return( m1 );
	}
#endif

	matrix< T, Allocator > mat = m1;
	typename matrix< T, Allocator >::size_type i;
	for( i = 0 ; i < mat.size( ) ; i++ ) mat[i] -= m2[i];
	return( mat );
}


// ä|ÇØéZ
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
#ifdef _CHECK_MATRIX_OPERATION_
	if( m1.cols( ) != m2.rows( ) )
	{
		// ä|ÇØéZÇ≈Ç´Ç‹ÇπÇÒó·äO
		::std::cout << "can't multiply matrices." << ::std::endl;
		return( m1 );
	}
#endif

	matrix< T, Allocator > mat( m1.rows( ), m2.cols( ) );
	typename matrix< T, Allocator >::size_type r, c, t;

	for( r = 0 ; r < mat.rows( ) ; r++ )
	{
		for( c = 0 ; c < mat.cols( ) ; c++ )
		{
			for( t = 0 ; t < m1.cols( ) ; t++ )
			{
				mat( r, c ) += m1( r, t ) * m2( t, c );
			}
		}
	}

	return( mat );
}


// íËêîÇ∆ÇÃë´ÇµéZ
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m, typename matrix< T, Allocator >::value_type val )
{
	matrix< T, Allocator > mat( m );
	typename matrix< T, Allocator >::size_type i, size = mat.rows( ) < mat.cols( ) ? mat.rows( ) : mat.cols( );

	for( i = 0 ; i < size ; i++ ) mat( i, i ) += val;

	return( mat );
}

template < class T, class Allocator >
inline matrix< T, Allocator > operator +( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	matrix< T, Allocator > mat( m );
	typename matrix< T, Allocator >::size_type i, size = mat.rows( ) < mat.cols( ) ? mat.rows( ) : mat.cols( );

	for( i = 0 ; i < size ; i++ ) mat( i, i ) += val;

	return( mat );
}



// íËêîÇ∆ÇÃà¯Ç´
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m, typename matrix< T, Allocator >::value_type val )
{
	matrix< T, Allocator > mat( m );
	typename matrix< T, Allocator >::size_type i, size = mat.rows( ) < mat.cols( ) ? mat.rows( ) : mat.cols( );

	for( i = 0 ; i < size ; i++ ) mat( i, i ) -= val;

	return( mat );
}

template < class T, class Allocator >
inline matrix< T, Allocator > operator -( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	matrix< T, Allocator > mat( m.rows( ), m.cols( ) );
	typename matrix< T, Allocator >::size_type i, size = mat.rows( ) < mat.cols( ) ? mat.rows( ) : mat.cols( );

	for( i = 0 ; i < size ; i++ ) mat( i, i ) -= val;

	return( mat );
}


// íËêîÇ∆ÇÃä|ÇØéZ
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( const matrix< T, Allocator > &m, typename matrix< T, Allocator >::value_type val )
{
	matrix< T, Allocator > mat( m );
	typename matrix< T, Allocator >::size_type i;

	for( i = 0 ; i < mat.size( ) ; i++ ) mat[i] *= val;

	return( mat );
}

template < class T, class Allocator >
inline matrix< T, Allocator > operator *( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	matrix< T, Allocator > mat( m );
	typename matrix< T, Allocator >::size_type i;

	for( i = 0 ; i < mat.size( ) ; i++ ) mat[i] *= val;

	return( mat );
}


// íËêîÇ∆ÇÃäÑÇË
template < class T, class Allocator >
inline matrix< T, Allocator > operator /( const matrix< T, Allocator > &m, typename matrix< T, Allocator >::value_type val )
{
#ifdef _CHECK_MATRIX_OPERATION_
	if( val == 0 )
	{
		// É[ÉçèúéZî≠ê∂
		::std::cout << "zero division occured." << ::std::endl;
		return( m1 );
	}
#endif

	matrix< T, Allocator > mat( m );
	typename matrix< T, Allocator >::size_type i, size = mat.rows( ) < mat.cols( ) ? mat.rows( ) : mat.cols( );

	for( i = 0 ; i < mat.size( ) ; i++ ) mat[i] /= val;

	return( mat );
}


#endif // __INCLUDE_MIST_OPERATOR_MATRIX__