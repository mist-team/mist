#ifndef __INCLUDE_LU_DECOMPOSITION__
#define __INCLUDE_LU_DECOMPOSITION__


// 行列クラスがインクルードされていない場合はインポートする．
#ifndef __INCLUDE_MIST_MATRIX__
#include "../matrix.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN

template < class T1, class T2, class Allocator >
void lu_decomposition( matrix< T1, Allocator > &a, matrix< T2, Allocator > &pivot )
{
	if( a.rows( ) != a.cols( ) ) return;

	typedef typename matrix< T1, Allocator >::size_type size_type;
	typedef typename matrix< T1, Allocator >::value_type value_type;
	typedef typename matrix< T2, Allocator >::value_type pivot_value;

	size_type i, imax, j, k, n = a.rows( );
	value_type big, dum, sum, temp;

	matrix< T1, Allocator > v( n, 1 );
	pivot.resize( n );
	
	// i行j列とあらわす
	for( i = 0 ; i < n ; i++ )
	{
		big = 0;
		for( j = 0 ; j < n ; j++ )
		{
			temp = std::abs( a( i, j ) );
			big = temp < big ? big : temp;
		}
		if( big == 0 ) // Singular matrix in routine ludcmp
		v[i] = 1 / big;
	}
	for( j = 0 ; j < n ; j++ )
	{
		for( i = 0 ; i < j ; i++ )
		{
			sum = a( i, j );
			for( k = 0 ; k < i ; k++ )
			{
				sum -= a( i, k ) * a( k, j );
			}
			a( i, j ) = sum;
		}

		big = 0;
		for( i = j ; i < n ; i++ )
		{
			sum = a( i, j );
			for( k = 0 ; k < j ; k++ )
			{
				sum -= a(i, k) * a(k, j);
			}
			a( i, j ) = sum;
			dum = v[i] * std::abs( sum );
			if( dum >= big )
			{
				big = dum;
				imax = i;
			}
		}
		if( j != imax )
		{
			for( k = 0 ; k < n ; k++ )
			{
				dum = a( imax, k );
				a( imax, k ) = a( j, k );
				a( j, k ) = dum;
			}
			v[imax] = v[j];
		}

		pivot[j] = static_cast< pivot_value >( imax );

		if( a( j, j ) == 0 )
		{
			a( j, j ) = 1.0e-20;
		}
		if( j != n - 1 )
		{
			dum = 1 / a( j, j );
			for( i = j + 1 ; i < n ; i++ )
			{
				a( i, j ) *= dum;
			}
		}
	}
}


template < class T1, class T2, class Allocator >
void lu_decomposition1( matrix< T1, Allocator > &a, matrix< T2, Allocator > &pivot )
{
	if( a.rows( ) != a.cols( ) ) return;

	typedef typename matrix< T1, Allocator >::size_type size_type;
	typedef typename matrix< T1, Allocator >::value_type value_type;
	typedef typename matrix< T2, Allocator >::value_type pivot_value;
	size_type i, j, k, n = a.rows( );
	size_type p;
	value_type tmp, r;
	pivot.resize( n );

	for( k = 0 ; k < n - 1 ; k++ )
	{
		p = k;
		for( i = k + 1 ; i < n ; i++ )
		{
			p = std::abs( a( p, k ) ) < std::abs( a( i, k ) ) ? i : p;
		}

		if( std::abs( a( p, k ) ) < 1.0e-20 )
		{
			// 入力行列 a は非正則
			return;
		}

		pivot[k] = static_cast< pivot_value >( p );

		if( p != k )
		{
			for( j = k ; j < n ; j++ )
			{
				tmp = a( k, j );
				a( k, j ) = a( p, j );
				a( p, j ) = tmp;
			}
		}

		for( i = k + 1 ; i < n ; i++ )
		{
			r = a( i, k ) / a( k, k );
			a( i, k ) = r;
			for( j = k + 1 ; j < n ; j++ )
			{
				a( i, j ) -= r * a( k, j );
			}
		}
	}
}

template < class T1, class T2, class Allocator >
void lu_backwardsub( const matrix< T1, Allocator > &a, matrix< T1, Allocator > &b, matrix< T2, Allocator > &pivot )
{
	if(a.rows() != a.cols()) return;
	if(a.rows() != b.rows()) return;

	typedef typename matrix< T1, Allocator >::size_type size_type;
	typedef typename matrix< T1, Allocator >::value_type value_type;
	typedef typename matrix< T2, Allocator >::value_type pivot_value;

	size_type i, ii = 0, ip, j, n = a.rows();
	value_type sum;
	
	for( i = 1 ; i <= n ; i++ )
	{
		ip = static_cast< size_type >( pivot[ i - 1 ] );
		sum = b[ ip ];
		b[ ip ] = b[ i - 1 ];
		if( ii > 0 )
		{
			for( j = ii ; j <= i - 1 ; j++ )
			{
				sum -= a( i - 1, j - 1 ) * b[ j - 1 ];
			}
		}
		else if( sum > 0 )
		{
			ii = i;
		}
		b[ i - 1 ] = sum;
	}
	for( i = n ; i > 0 ; i-- )
	{
		sum = b[ i - 1 ];
        for( j = i + 1 ; j <= n ; j++ )
		{
			sum -= a( i - 1 , j - 1 ) * b[ j - 1 ];
		}
		b[ i - 1 ] = sum / a( i - 1, i - 1 );
	}
}


template < class T1, class T2, class Allocator >
void lu_backwardsub1( const matrix< T1, Allocator > &a, matrix< T1, Allocator > &b, const matrix< T2, Allocator > &pivot )
{
	if(a.rows() != a.cols()) return;
	if(a.rows() != b.rows()) return;

	typedef typename matrix< T1, Allocator >::size_type size_type;
	typedef typename matrix< T1, Allocator >::value_type value_type;
	typedef typename matrix< T2, Allocator >::value_type pivot_value;

	size_type i, j, k, n = a.rows();
	value_type tmp;
	value_type sum;

	for( k = 0 ; k < n - 1 ; k++ )
	{
		tmp = b[ k ];
		b[ k ] = b[ pivot[ k ] ];
		b[ pivot[ k ] ] = tmp;

		for( i = k + 1 ; i < n ; i++ )
		{
			b[ i ] -= a( i, k ) * b[ k ];
		}
	}

	for( k = n ; k > 0 ; k-- )
	{
		i = k - 1;
		tmp = 0;
		for( j = i + 1 ; j < n; j++ )
		{
			tmp += a( i, j ) * b[ j ];
		}
		b[ i ] = ( b[ i ] - tmp ) / a( i, i );
	}
}


template < class T, class Allocator >
void inverse( matrix< T, Allocator > &a )
{
	if( a.rows( ) != a.cols( ) )
	{
		return;
	}

	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef typename matrix< T, Allocator >::value_type value_type;

	size_type i, j, n = a.rows();

	matrix< T, Allocator > lu( a );
	matrix< T, Allocator > ret( n, n );
	matrix< T, Allocator > col( n, 1 );
	matrix< T, Allocator > pivot;

	lu_decomposition( lu, pivot );

	for( j = 0 ; j < n ; j++ )
	{
		col.clean( );
		col[ j ] = 1;
		lu_backwardsub( lu, col, pivot );
		for( i = 0 ; i < n ; i++ )
		{
			a( i, j ) = col[ i ];
		}
	}
}


template < class T, class Allocator >
void inverse1( matrix< T, Allocator > &a )
{
	if( a.rows( ) != a.cols( ) )
	{
		return;
	}

	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef typename matrix< T, Allocator >::value_type value_type;

	size_type i, j, n = a.rows();

	matrix< T, Allocator > lu( a );
	matrix< T, Allocator > ret( n, n );
	matrix< T, Allocator > col( n, 1 );
	matrix< T, Allocator > pivot;

	lu_decomposition1( lu, pivot );

	for( j = 0 ; j < n ; j++ )
	{
		col.clean( );
		col[ j ] = 1;
		lu_backwardsub1( lu, col, pivot );
		for( i = 0 ; i < n ; i++ )
		{
			a( i, j ) = col[ i ];
		}
	}
}

// mist名前空間の終わり
_MIST_END


#endif