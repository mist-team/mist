#ifndef __INCLUDE_DCT_H__
#define __INCLUDE_DCT_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_FFT_UTIL_H__
#include "fft_util.h"
#endif


#ifdef WIN32
#define	USE_CDFT_WINTHREADS
#define USE_FFT2D_WINTHREADS
#define USE_FFT3D_WINTHREADS
#else
#define	USE_CDFT_PTHREADS
#define USE_FFT2D_PTHREADS
#define USE_FFT3D_PTHREADS
#endif

#include "fftsg.h"
#include "fftsg2d.h"
#include "fftsg3d.h"

// mist名前空間の始まり
_MIST_BEGIN



// 1次元高速コサイン変換

template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct( array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	typedef typename Allocator2::value_type value_type;
	size_type i;
	double *data;
	double *w;
	int *ip;

	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.size( ) / 2 )) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) * 5 / 4 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in[ i ] ) );
		data[ i ] = c.real( );
	}

	ip[ 0 ] = 0;

	ooura_fft::ddct( static_cast<int>( in.size() ), -1, data, ip, w );


	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = __fft_util__::convert_complex< T2 >::convert_from( data[ i ], 0.0 );
	}

	free( w );
	free( ip );
	free( data );

	return( true );
}

//1次元高速コサイン逆変換
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct_inverse( array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	typedef typename Allocator2::value_type value_type;
	size_type i;
	double *data;
	double *w;
	int *ip;

	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) (  sqrt( static_cast< double >(in.size( ) / 2 )) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) * 5 / 4 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in[ i ] ) );
		data[ i ] = c.real( );
	}

	ip[ 0 ] = 0;
	data[ 0 ] *= 0.5;

	ooura_fft::ddct(  static_cast<int>( in.size() ), 1, data, ip, w );


	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = __fft_util__::convert_complex< T2 >::convert_from( data[ i ] * 2.0 / out.size( ), 0.0 );
	}

	free( w );
	free( ip );
	free( data );

	return( true );
}

//2次元高速コサイン変換
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct( array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	typedef typename Allocator2::value_type value_type;
	size_type i,j;
	double **data;
	double *t;
	double *w;
	int *ip;

	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) || !__fft_util__::size_check( ( unsigned int ) in.height( ) ) )
	{
		return( false );
	}

	data = ( double ** ) malloc( sizeof( double * ) * in.width( ) );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) );
	}

	t = ( double * ) malloc( sizeof( double ) * 4 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt(static_cast< double >(in.width( ) / 2 ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) / 2 ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j ) ) );
			data[ i ][ j ] = c.real( );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::ddct2d(  static_cast<int>( in.width( ) ), static_cast<int>( in.height( ) ), -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ], 0.0 );
		}
	}

	free( t );
	free( w );
	free( ip );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		free( data[ i ] );
	}

	free( data );

	return( true );
}


// 2次元コサイン逆変換
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct_inverse( array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	typedef typename Allocator2::value_type value_type;
	size_type i,j;
	double **data;
	double *t;
	double *w;
	int *ip;

	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) || !__fft_util__::size_check( ( unsigned int ) in.height( ) ) )
	{
		return( false );
	}

	data = ( double ** ) malloc( sizeof( double * ) * in.width( ) );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) );
	}

	t = ( double * ) malloc( sizeof( double ) * 4 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt(static_cast< double >(in.width( ) / 2 )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) / 2 )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j ) ) );
			data[ i ][ j ] = c.real( );
		}
	}

	ip[ 0 ] = 0;


	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ][ 0 ] *= 0.5;
	}

	for( j = 0 ; j < in.height( ) ; j++ )
	{
		data[ 0 ][ j ] *= 0.5;
	}


	ooura_fft::ddct2d( static_cast< int >( in.width( ) ), static_cast< int >( in.height( ) ), 1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ] * 4.0 / in.size( ), 0.0 );
		}
	}

	free( t );
	free( w );
	free( ip );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		free( data[ i ] );
	}

	free( data );

	return( true );
}


// 3次元高速コサイン変換
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct( array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	typedef typename Allocator2::value_type value_type;
	size_type i,j,k;
	double ***data;
	double *t;
	double *w;
	int *ip;

	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.height( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.depth( ) ) )
	{
		return( false );
	}

	data = ( double *** ) malloc( sizeof( double ** ) * in.width( ) );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ] = ( double ** ) malloc( sizeof( double * ) * in.height( ) );

		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ j ] = ( double * ) malloc( sizeof( double ) * in.depth( ) );
		}
	}

	if( in.width( ) > in.height( ) )
	{
		t = ( double * ) malloc( sizeof( double ) * 4 * in.width( ) * FFT3D_MAX_THREADS );
	}
	else
	{
		t = ( double * ) malloc( sizeof( double ) * 4 * in.height( ) * FFT3D_MAX_THREADS );
	}


	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( )/ 2 )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) / 2 )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j, k ) ) );
				data[ i ][ j ][ k ] = c.real( );
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::ddct3d( static_cast<int>( in.width( ) ), static_cast<int>( in.height( ) ), static_cast<int>( in.depth( ) ), -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ][ k ], 0.0 );
			}
		}
	}

	free( t );
	free( w );
	free( ip );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			free( data[ i ][ j ] );
		}

		free( data[ i ] );
	}

	free( data );

	return( true );
}


//3次元高速コサイン逆変換
template < class T1, class T2, class Allocator1, class Allocator2 >
bool dct_inverse( array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	typedef typename Allocator2::value_type value_type;
	size_type i,j,k;
	double ***data;
	double *t;
	double *w;
	int *ip;

	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.height( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.depth( ) ) )
	{
		return( false );
	}

	data = ( double *** ) malloc( sizeof( double ** ) * in.width( ) );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ] = ( double ** ) malloc( sizeof( double * ) * in.height( ) );

		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ j ] = ( double * ) malloc( sizeof( double ) * in.depth( ) );
		}
	}

	if( in.width( ) > in.height( ) )
	{
		t = ( double * ) malloc( sizeof( double ) * 4 * in.width( ) * FFT3D_MAX_THREADS );
	}
	else
	{
		t = ( double * ) malloc( sizeof( double ) * 4 * in.height( ) * FFT3D_MAX_THREADS );
	}


	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) / 2 )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) / 2 )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j, k ) ) );
				data[ i ][ j ][ k ] = c.real( );
			}
		}
	}


	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ j ][ 0 ] *= 0.5;
		}

		for( k = 0 ; k < in.depth( ) ; k++ )
		{
			data[ i ][ 0 ][ k ] *= 0.5;
		}
	}

	for( j = 0 ; j < in.height( ) ; j++ )
	{
		for( k = 0 ; k < in.depth( ) ; k++ )
		{
			data[ 0 ][ j ][ k ] *= 0.5;
		}
	}



	ip[ 0 ] = 0;

	ooura_fft::ddct3d( static_cast<int>( in.width( ) ), static_cast<int>( in.height( ) ), static_cast<int>( in.depth( ) ), 1, data, t, ip, w );


	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ][ k ] * 8.0 / in.size( ), 0.0 );
			}
		}
	}

	free( t );
	free( w );
	free( ip );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			free( data[ i ][ j ] );
		}

		free( data[ i ] );
	}

	free( data );

	return( true );
}

_MIST_END

#endif

