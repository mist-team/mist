#ifndef __INCLUDE_DST_H__
#define __INCLUDE_DST_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <cmath>
#include <complex>


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


#ifndef _SIZE_CHECK_
#define _SIZE_CHECK_

//データサイズの2のべき乗判定ルーチン
bool size_check( unsigned int t )
{
	int bit;

	if( t & 0x00000001 )
	{
		return false;
	}

	for( bit = 0 ; 0 != t ; t = ( unsigned int ) ( t >> 1 ) )
	{
		if( t & 0x00000001 )
		{
			bit++;
		}
	}

	if( bit == 1 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

#endif

//1次元高速サイン変換

template < class T1, class T2, class Allocator1, class Allocator2 >
void dst( array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.size( ) / 2 ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) * 5 / 4 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i ] = ( double ) in[ i ];
	}

	ip[ 0 ] = 0;

	ooura_fft::ddst( in.size( ), -1, data, ip, w );


	out.resize( in.size( ) );

	out[ 0 ] = data[ in.size( ) - 1 ];

	for( i = 1 ; i < out.size( ) ; i++ )
	{
		out[ i ] = ( T2 ) data[ i ];
	}

	free( w );
	free( ip );
	free( data );
}

//1次元高速サイン逆変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_dst( array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.size( ) / 2 ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) * 5 / 4 ) );

	data[ 0 ] = in[ in.size( ) - 1 ];

	for( i = 1 ; i < in.size( ) ; i++ )
	{
		data[ i ] = ( double ) in[ i ];
	}

	ip[ 0 ] = 0;
	data[ 0 ] *= 0.5;

	ooura_fft::ddst( in.size( ), 1, data, ip, w );


	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = ( T2 ) ( data[ i ] * 2.0 / out.size( ) );
	}

	free( w );
	free( ip );
	free( data );
}

//2次元高速サイン変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void dst( array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	int i, j;
	double **data;
	double *t;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.width( ) ) || !size_check( ( unsigned int ) in.height( ) ) )
	{
		return ;
	}

	data = ( double ** ) malloc( sizeof( double * ) * in.width( ) );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) );
	}

	t = ( double * ) malloc( sizeof( double ) * 4 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ j ] = ( double ) in( i, j );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::ddst2d( in.width( ), in.height( ), -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = ( T2 ) data[ i % in.width( ) ][ j % in.height( ) ];

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
}


//2次元サイン逆変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_dst( array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	int i, j;
	double **data;
	double *t;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.width( ) ) || !size_check( ( unsigned int ) in.height( ) ) )
	{
		return ;
	}

	data = ( double ** ) malloc( sizeof( double * ) * in.width( ) );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) );
	}

	t = ( double * ) malloc( sizeof( double ) * 4 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i % in.width( ) ][ j % in.height( ) ] = ( double ) in( i, j );
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

	ooura_fft::ddst2d( in.width( ), in.height( ), 1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = ( T2 ) ( data[ i ][ j ] * 4.0 / in.size( ) );

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
}


//3次元高速コサイン変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void dst( array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
{
	int i, j, k;
	double ***data;
	double *t;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.width( ) ) ||
			!size_check( ( unsigned int ) in.height( ) ) ||
			!size_check( ( unsigned int ) in.depth( ) ) )
	{
		return ;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				data[ i ][ j ][ k ] = ( double ) in( i, j, k );
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::ddst3d( in.width( ), in.height( ), in.depth( ), -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = ( T2 ) data[ i % in.width( ) ][ j % in.height( ) ][ k % in.depth( ) ];
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
}


//3次元高速サイン逆変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_dst( array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
{
	int i, j, k;
	double ***data;
	double *t;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.width( ) ) ||
			!size_check( ( unsigned int ) in.height( ) ) ||
			!size_check( ( unsigned int ) in.depth( ) ) )
	{
		return ;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) * 3 / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) / 2 ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) * 3 / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				data[ i % in.width( ) ][ j % in.height( ) ][ k % in.depth( ) ] = ( double ) in( i, j, k );
			}
		}
	}

	ip[ 0 ] = 0;


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


	ooura_fft::ddst3d( in.width( ), in.height( ), in.depth( ), 1, data, t, ip, w );


	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = ( T2 ) ( data[ i ][ j ][ k ] * 8.0 / in.size( ) );
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
}

_MIST_END

#endif

