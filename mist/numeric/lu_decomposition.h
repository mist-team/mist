#ifndef __INCLUDE_LU_DECOMPOSITION__
#define __INCLUDE_LU_DECOMPOSITION__


// 行列クラスがインクルードされていない場合はインポートする．
#ifndef __INCLUDE_MIST_MATRIX__
#include "../matrix.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN

template < class T1, class T2, class Allocator >
void lu_decomposition( matrix< T1, Allocator > &m, matrix< T2, Allocator > &p )
{
	if( m.rows( ) != m.cols( ) ) return;

	int i, imax, j, k, n = a.rows( );
	double big, dum, sum, temp;
	matrix< T, Allocator > v( n, 1 );
	p.resize( n, 1 );
	
	d = 1;
	// i行j列とあらわす
	for( i = 0 ; i < n ; i++ )
	{
		big = 0.0;
		for( j = 0 ; j < n ; j++ )
		{
			if( ( temp = std::abs( m( i, j ) ) ) > big )
			{
				big = temp;
			}
		}
		if( big == 0.0 ) // Singular matrix in routine ludcmp
		v[i] = 1.0 / big;
	}
	for( j = 0 ; j < n ; j++ )
	{
		for( i = 0 ; i < j ; i++ )
		{
			sum = m( i, j );
			for( k = 0 ; k < i ; k++ )
			{
				sum -= m( i, k ) * m( k, j );
			}
			m( i, j ) = sum;
		}

		big = 0.0;
		for( i = j ; i < n ; i++ )
		{
			sum = m( i, j );
			for( k = 0 ; k < j ; k++ )
			{
				sum -= m(i, k) * m(k, j);
			}
			m( i, j ) = sum;
			if( ( dum = v[i] * std::abs( sum ) ) >= big )
			{
				big = dum;
				imax = i;
			}
		}
		if( j != imax )
		{
			for( k = 0 ; k < n ; k++ )
			{
				dum = m( imax, k );
				m( imax, k ) = m( j, k );
				m( j, k ) = dum;
			}
			v[imax] = v[j];
		}
		p[j] = imax;
		if( m( j, j ) == 0.0 )
		{
			m( j, j ) = 1.0e-20;
		}
		if( j != n - 1 )
		{
			dum = 1.0 / m( j, j );
			for( i = j + 1 ; i < n ; i++ )
			{
				m( i, j ) *= dum;
			}
		}
	}
}


template < class T1, class T2, class Allocator >
void lu_decomposition1( matrix< T1, Allocator > &m, matrix< T2, Allocator > &pivot )
{
	if( m.rows( ) != m.cols( ) ) return;

	typedef typename matrix< T1, Allocator >::size_type size_type;
	typedef typename matrix< T1, Allocator >::value_type value_type;
	size_type i, j, k, n = m.rows( );
	size_type p;
	value_type tmp, r;
	pivot.resize( n );

	for( k = 0 ; k < n - 1 ; k++ )
	{
		p = k;
		for( i = k + 1 ; k < n ; i++ )
		{
			p = std::abs( m( p, k ) ) < std::abs( m( i, k ) ) ? i : p;
		}

		if( std::abs( m( p, k ) ) < 1.0e-20 )
		{
			// 入力行列 m は非正則
			return;
		}

		pivot[k] = p;

		if( p != k )
		{
			for( j = k ; j < n ; j++ )
			{
				tmp = m( k, j );
				m( k, j ) = m( p, j );
				m( p, j ) = tmp;
			}
		}

		for( i = k + 1 ; i < n ; i++ )
		{
			r = m( i, k ) / m( k, k );
			m( i, k ) = r;
			for( j = k + 1 ; j < n ; j++ )
			{
				m( i, j ) = m( i, j ) - r * m( k, j );
			}
		}
	}
}

// mist名前空間の終わり
_MIST_END


#endif