#ifndef __INCLUDE_FFT_H__
#define __INCLUDE_FFT_H__

#include <mist.h>
#include <math.h>
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

//データサイズの2のべき乗判定ルーチン
bool size_check( unsigned int t )
{
	int bit;

	if( t & 0x00000001 )
	{
		return false;
	}

	for( bit = 0; 0 != t; t = ( unsigned int )(t >> 1) )
	{
		if ( t & 0x00000001 )
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


//1次元高速フーリエ変換
void fft( array1< std::complex< double > > &in, array1< std::complex< double > > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.size() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double *)malloc( sizeof( double ) * in.size() * 2 );
	ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );
	w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );

	for( i = 0 ; i < in.size() ; i++ )
	{
		data[i*2] = in[i].real();
		data[i*2+1] = in[i].imag();
	}

	ip[0] = 0;

	cdft( in.size()*2, -1, data, ip, w );


	out.resize( in.size() );

	for( i = 0 ; i < out.size() ; i++ )
	{
		out[i] = std::complex< double >( data[2*i], data[2*i+1] );
	}

	delete w;
	delete ip;
	delete data;
}


void fft( array1< std::complex< float > > &in, array1< std::complex< float > > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.size() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double *)malloc( sizeof( double ) * in.size() * 2 );
	ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );
	w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );

	for( i = 0 ; i < in.size() ; i++ )
	{
		data[i*2] = ( double )in[i].real();
		data[i*2+1] = ( double )in[i].imag();
	}

	ip[0] = 0;

	cdft( in.size()*2, -1, data, ip, w );


	out.resize( in.size() );

	for( i = 0 ; i < out.size() ; i++ )
	{
		out[i] = std::complex< float >( (float)data[2*i], (float)data[2*i+1] );
	}

	delete w;
	delete ip;
	delete data;
}

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array1< T1, Allocator1 > &in, array1< std::complex< T2 >, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.size() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double *)malloc( sizeof( double ) * in.size() * 2 );
	ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );
	w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );

	for( i = 0 ; i < in.size() ; i++ )
	{
		data[i*2] = ( double )in[i];
	}

	ip[0] = 0;

	cdft( in.size()*2, -1, data, ip, w );


	out.resize( in.size() );

	for( i = 0 ; i < out.size() ; i++ )
	{
		out[i] = std::complex< T2 >( (T2)data[2*i], (T2)data[2*i+1] );
	}

	delete w;
	delete ip;
	delete data;
}

void inverse_fft( array1< std::complex< double > > &in, array1< std::complex< double > > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.size() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double *)malloc( sizeof( double ) * in.size() * 2 );
	ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );
	w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );

	for( i = 0 ; i < in.size() ; i++ )
	{
		data[i*2] = in[i].real();
		data[i*2+1] = in[i].imag();
	}

	ip[0] = 0;

	cdft( in.size()*2, 1, data, ip, w );

	for( i = 0 ; i < in.size() *2 ; i++ )
	{
		data[i] *= 1.0/ in.size();
	}


	out.resize( in.size() );

	for( i = 0 ; i < out.size() ; i++ )
	{
		out[i] = std::complex< double >( data[2*i], data[2*i+1] );
	}

	delete w;
	delete ip;
	delete data;
}


void inverse_fft( array1< std::complex< float > > &in, array1< std::complex< float > > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.size() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double *)malloc( sizeof( double ) * in.size() * 2 );
	ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );
	w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );

	for( i = 0 ; i < in.size() ; i++ )
	{
		data[i*2] = ( double )in[i].real();
		data[i*2+1] = ( double )in[i].imag();
	}

	ip[0] = 0;

	cdft( in.size()*2, 1, data, ip, w );

	for( i = 0 ; i < in.size() *2 ; i++ )
	{
		data[i] *= 1.0/ in.size();
	}


	out.resize( in.size() );

	for( i = 0 ; i < out.size() ; i++ )
	{
		out[i] = std::complex< float >( (float)data[2*i], (float)data[2*i+1] );
	}

	delete w;
	delete ip;
	delete data;
}

template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array1< std::complex< T1 >, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.size() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double *)malloc( sizeof( double ) * in.size() * 2 );
	ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );
	w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );

	for( i = 0 ; i < in.size() ; i++ )
	{
		data[i*2] = ( double )in[i].real();
		data[i*2+1] = ( double )in[i].imag();
	}

	ip[0] = 0;

	cdft( in.size()*2, 1, data, ip, w );

	for( i = 0 ; i < in.size() *2 ; i++ )
	{
		data[i] *= 1.0/ in.size();
	}


	out.resize( in.size() );

	for( i = 0 ; i < out.size() ; i++ )
	{
		out[i] = (T2)data[2*i];
	}

	delete w;
	delete ip;
	delete data;
}

//2次元高速フーリエ変換

void fft( array2< std::complex< double > > &in, array2< std::complex< double > > &out )
{
	int i,j;
	double **data;
	double *t;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double **)malloc( sizeof( double *) * in.width() );

	for( i = 0 ; i < in.width() ; i++ )
	{
		data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );

	if( in.width() > in.height() )
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );
	}
	else
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );

	}

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j< in.height() ; j++ )
		{
			data[i][2*j] = in( i, j ).real();
			data[i][2*j+1] = in( i, j ).imag();
		}
	}

	ip[0] = 0;

	cdft2d( in.width(), in.height()*2, -1, data, t, ip, w );


	out.resize( in.width(), in.height() );

	for( i = 0 ; i < out.width() ; i++ )
	{
		for( j = 0 ; j < out.height() ; j++ )
		{
			out( i, j ) = std::complex< double >( data[i][2*j], data[i][2*j+1] );

		}
	}

	delete t;
	delete w;
	delete ip;

	for( i = 0 ; i < in.width() ; i++ )
	{
		 delete data[i];
	}
	delete data;
}

void fft( array2< std::complex< float > > &in, array2< std::complex< float > > &out )
{
	int i,j;
	double **data;
	double *t;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double **)malloc( sizeof( double *) * in.width() );

	for( i = 0 ; i < in.width() ; i++ )
	{
		data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );

	if( in.width() > in.height() )
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );
	}
	else
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );

	}

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j< in.height() ; j++ )
		{
			data[i][2*j] = ( double )in( i, j ).real();
			data[i][2*j+1] = ( double )in( i, j ).imag();
		}
	}

	ip[0] = 0;

	cdft2d( in.width(), in.height()*2, -1, data, t, ip, w );


	out.resize( in.width(), in.height() );

	for( i = 0 ; i < out.width() ; i++ )
	{
		for( j = 0 ; j < out.height() ; j++ )
		{
			out( i, j ) = std::complex< float >( (float)data[i][2*j], (float)data[i][2*j+1] );

		}
	}

	delete t;
	delete w;
	delete ip;

	for( i = 0 ; i < in.width() ; i++ )
	{
		 delete data[i];
	}
	delete data;
}

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array2< T1, Allocator1 > &in, array2< std::complex< T2 >, Allocator2 > &out )
{
	int i,j;
	double **data;
	double *t;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double **)malloc( sizeof( double *) * in.width() );

	for( i = 0 ; i < in.width() ; i++ )
	{
		data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );

	if( in.width() > in.height() )
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );
	}
	else
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );

	}

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j< in.height() ; j++ )
		{
			data[i][2*j] = ( double )in( i, j );
			data[i][2*j+1] = 0.0;
		}
	}

	ip[0] = 0;

	cdft2d( in.width(), in.height()*2, -1, data, t, ip, w );


	out.resize( in.width(), in.height() );

	for( i = 0 ; i < out.width() ; i++ )
	{
		for( j = 0 ; j < out.height() ; j++ )
		{
			out( i, j ) = std::complex< T2 >( (T2)data[i][2*j], (T2)data[i][2*j+1] );

		}
	}

	delete t;
	delete w;
	delete ip;

	for( i = 0 ; i < in.width() ; i++ )
	{
		 delete data[i];
	}
	delete data;
}


void inverse_fft( array2< std::complex< double > > &in, array2< std::complex< double > > &out )
{
	int i,j;
	double **data;
	double *t;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double **)malloc( sizeof( double *) * in.width() );

	for( i = 0 ; i < in.width() ; i++ )
	{
		data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );

	if( in.width() > in.height() )
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );
	}
	else
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );

	}

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j< in.height() ; j++ )
		{
			data[i][2*j] = in( i, j ).real();
			data[i][2*j+1] = in( i, j ).imag();
		}
	}

	ip[0] = 0;

	cdft2d( in.width(), in.height()*2, 1, data, t, ip, w );

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j < in.height()*2 ; j++ )
		{
			data[i][j] *= 1.0 / ( in.width() * in.height() );
		}
	}

	out.resize( in.width(), in.height() );

	for( i = 0 ; i < out.width() ; i++ )
	{
		for( j = 0 ; j < out.height() ; j++ )
		{
			out( i, j ) = std::complex< double >( data[i][2*j], data[i][2*j+1] );

		}
	}

	delete t;
	delete w;
	delete ip;

	for( i = 0 ; i < in.width() ; i++ )
	{
		 delete data[i];
	}
	delete data;
}


void inverse_fft( array2< std::complex< float > > &in, array2< std::complex< float > > &out )
{
	int i,j;
	double **data;
	double *t;
	double *w;
	int *ip;

#ifdef _DEBUG

	if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double **)malloc( sizeof( double *) * in.width() );

	for( i = 0 ; i < in.width() ; i++ )
	{
		data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );

	if( in.width() > in.height() )
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );
	}
	else
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );

	}

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j< in.height() ; j++ )
		{
			data[i][2*j] = ( double )in( i, j ).real();
			data[i][2*j+1] = ( double )in( i, j ).imag();
		}
	}

	ip[0] = 0;

	cdft2d( in.width(), in.height()*2, 1, data, t, ip, w );

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j < in.height()*2 ; j++ )
		{
			data[i][j] *= 1.0 / ( in.width() * in.height() );
		}
	}

	out.resize( in.width(), in.height() );

	for( i = 0 ; i < out.width() ; i++ )
	{
		for( j = 0 ; j < out.height() ; j++ )
		{
			out( i, j ) = std::complex< float >( ( float )data[i][2*j], ( float )data[i][2*j+1] );

		}
	}

	delete t;
	delete w;
	delete ip;

	for( i = 0 ; i < in.width() ; i++ )
	{
		 delete data[i];
	}
	delete data;
}


template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array2< std::complex< T1 >, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	int i,j;
	double **data;
	double *t;
	double *w;
	int *ip;
	
#ifdef _DEBUG

	if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )
	{
		printf(" data size is not 2^n\n" );
		return;
	}

#endif

	data = ( double **)malloc( sizeof( double *) * in.width() );

	for( i = 0 ; i < in.width() ; i++ )
	{
		data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );

	if( in.width() > in.height() )
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );
	}
	else
	{
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );
		w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );

	}

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j< in.height() ; j++ )
		{
			data[i][2*j] = ( double )in( i, j ).real();
			data[i][2*j+1] = ( double )in( i, j ).imag();
		}
	}

	ip[0] = 0;

	cdft2d( in.width(), in.height()*2, 1, data, t, ip, w );

	for( i = 0 ; i < in.width() ; i++ )
	{
		for( j = 0 ; j < in.height()*2 ; j++ )
		{
			data[i][j] *= 1.0 / ( in.width() * in.height() );
		}
	}

	out.resize( in.width(), in.height() );

	for( i = 0 ; i < out.width() ; i++ )
	{
		for( j = 0 ; j < out.height() ; j++ )
		{
			out( i, j ) = (T2)data[i][2*j];

		}
	}

	delete t;
	delete w;
	delete ip;

	for( i = 0 ; i < in.width() ; i++ )
	{
		 delete data[i];
	}
	delete data;
}

_MIST_END

#endif