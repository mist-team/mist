#ifndef __INCLUDE_FFT_H__
#define __INCLUDE_FFT_H__


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

//1次元高速フーリエ変換

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array1< T1, Allocator1 > &in, array1< std::complex< T2 >, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) * 2 );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >( in.size( ) ) ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) / 2 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i * 2 ] = ( double ) in[ i ];
		data[ i * 2 + 1 ] = 0.0 ;
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( in.size( ) * 2, -1, data, ip, w );


	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = std::complex< T2 >( ( T2 ) data[ 2 * i ], ( T2 ) data[ 2 * i + 1 ] );
	}

	free( w );
	free( ip );
	free( data );
}

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

#define FFT1( type1, type2 ) \
	template < >\
	void fft( array1< std::complex< type1 > > &in, array1< std::complex< type2 > > &out )\
	{\
		int i;\
		double *data;\
		double *w;\
		int *ip;\
		if( !size_check( (unsigned int) in.size() ) )\
		{\
			return;\
		}\
		data = ( double *)malloc( sizeof( double ) * in.size() * 2 );\
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );\
		w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );\
		for( i = 0 ; i < in.size() ; i++ )\
		{\
			data[i*2] = ( double )in[i].real();\
			data[i*2+1] = ( double )in[i].imag();\
		}\
		ip[0] = 0;\
		ooura_fft::cdft( in.size()*2, -1, data, ip, w );\
		out.resize( in.size() );\
		for( i = 0 ; i < out.size() ; i++ )\
		{\
			out[i] = std::complex< type2 >( (type2)data[2*i], (type2)data[2*i+1] );\
		}\
		free( w );\
		free( ip );\
		free( data );\
	};\

	FFT1(	float,	float	)
FFT1( float, double	)
FFT1( float, int	)
FFT1( float, short	)
FFT1( float, char	)
FFT1( float, unsigned int )
FFT1( float, unsigned short )
FFT1( float, unsigned char )

FFT1( double, float	)
FFT1( double, double	)
FFT1( double, int	)
FFT1( double, short	)
FFT1( double, char	)
FFT1( double, unsigned int )
FFT1( double, unsigned short )
FFT1( double, unsigned char )

FFT1( int, float	)
FFT1( int, double	)
FFT1( int, int	)
FFT1( int, short	)
FFT1( int, char	)
FFT1( int, unsigned int )
FFT1( int, unsigned short )
FFT1( int, unsigned char )

FFT1( short, float	)
FFT1( short, double	)
FFT1( short, int	)
FFT1( short, short	)
FFT1( short, char	)
FFT1( short, unsigned int )
FFT1( short, unsigned short )
FFT1( short, unsigned char )

FFT1( char, float	)
FFT1( char, double	)
FFT1( char, int	)
FFT1( char, short	)
FFT1( char, char	)
FFT1( char, unsigned int )
FFT1( char, unsigned short )
FFT1( char, unsigned char )

FFT1( unsigned int, float	)
FFT1( unsigned int, double	)
FFT1( unsigned int, int	)
FFT1( unsigned int, short	)
FFT1( unsigned int, char	)
FFT1( unsigned int, unsigned int )
FFT1( unsigned int, unsigned short )
FFT1( unsigned int, unsigned char )

FFT1( unsigned short, float	)
FFT1( unsigned short, double	)
FFT1( unsigned short, int	)
FFT1( unsigned short, short	)
FFT1( unsigned short, char	)
FFT1( unsigned short, unsigned int )
FFT1( unsigned short, unsigned short )
FFT1( unsigned short, unsigned char )

FFT1( unsigned char, unsigned char	)
FFT1( unsigned char, double	)
FFT1( unsigned char, int	)
FFT1( unsigned char, short	)
FFT1( unsigned char, char	)
FFT1( unsigned char, unsigned int )
FFT1( unsigned char, unsigned short )
FFT1( unsigned char, unsigned char )

#undef FFT1

#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array1< std::complex< T1 >, Allocator1 > &in, array1< std::complex< T2 >, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}


	data = ( double * ) malloc( sizeof( double ) * in.size( ) * 2 );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.size( ) ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) / 2 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i * 2 ] = ( double ) in[ i ].real( );
		data[ i * 2 + 1 ] = ( double ) in[ i ].imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( in.size( ) * 2, -1, data, ip, w );


	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = std::complex< T2 >( ( T2 ) data[ 2 * i ], ( T2 ) data[ 2 * i + 1 ] );
	}

	free( w );
	free( ip );
	free( data );
}

#endif

//1次元逆フーリエ変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array1< std::complex< T1 >, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) * 2 );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.size( ) ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) / 2 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i * 2 ] = ( double ) in[ i ].real( );
		data[ i * 2 + 1 ] = ( double ) in[ i ].imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( in.size( ) * 2, 1, data, ip, w );

	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = ( T2 ) ( data[ 2 * i ] / in.size( ) );
	}

	free( w );
	free( ip );
	free( data );
}

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

#define INVERSE_FFT1( type1, type2 ) \
	template < >\
	void inverse_fft( array1< std::complex< type1 > > &in, array1< std::complex< type2 > > &out )\
	{\
		int i;\
		double *data;\
		double *w;\
		int *ip;\
		if( !size_check( (unsigned int) in.size() ) )\
		{\
			return;\
		}\
		data = ( double *)malloc( sizeof( double ) * in.size() * 2 );\
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.size() ) + 3 ) );\
		w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );\
		for( i = 0 ; i < in.size() ; i++ )\
		{\
			data[i*2] = ( double )in[i].real();\
			data[i*2+1] = ( double )in[i].imag();\
		}\
		ip[0] = 0;\
		ooura_fft::cdft( in.size()*2, 1, data, ip, w );\
		out.resize( in.size() );\
		for( i = 0 ; i < out.size() ; i++ )\
		{\
			out[i] = std::complex< type2 >( (type2)(data[2*i]/in.size() ), (type2)(data[2*i+1]/in.size()) );\
		}\
		free( w );\
		free( ip );\
		free( data );\
	}\

	INVERSE_FFT1(	float,	float	)
INVERSE_FFT1( float, double	)
INVERSE_FFT1( float, int	)
INVERSE_FFT1( float, short	)
INVERSE_FFT1( float, char	)
INVERSE_FFT1( float, unsigned int )
INVERSE_FFT1( float, unsigned short )
INVERSE_FFT1( float, unsigned char )

INVERSE_FFT1( double, float	)
INVERSE_FFT1( double, double	)
INVERSE_FFT1( double, int	)
INVERSE_FFT1( double, short	)
INVERSE_FFT1( double, char	)
INVERSE_FFT1( double, unsigned int )
INVERSE_FFT1( double, unsigned short )
INVERSE_FFT1( double, unsigned char )

INVERSE_FFT1( int, float	)
INVERSE_FFT1( int, double	)
INVERSE_FFT1( int, int	)
INVERSE_FFT1( int, short	)
INVERSE_FFT1( int, char	)
INVERSE_FFT1( int, unsigned int )
INVERSE_FFT1( int, unsigned short )
INVERSE_FFT1( int, unsigned char )

INVERSE_FFT1( short, float	)
INVERSE_FFT1( short, double	)
INVERSE_FFT1( short, int	)
INVERSE_FFT1( short, short	)
INVERSE_FFT1( short, char	)
INVERSE_FFT1( short, unsigned int )
INVERSE_FFT1( short, unsigned short )
INVERSE_FFT1( short, unsigned char )

INVERSE_FFT1( char, float	)
INVERSE_FFT1( char, double	)
INVERSE_FFT1( char, int	)
INVERSE_FFT1( char, short	)
INVERSE_FFT1( char, char	)
INVERSE_FFT1( char, unsigned int )
INVERSE_FFT1( char, unsigned short )
INVERSE_FFT1( char, unsigned char )

INVERSE_FFT1( unsigned int, float	)
INVERSE_FFT1( unsigned int, double	)
INVERSE_FFT1( unsigned int, int	)
INVERSE_FFT1( unsigned int, short	)
INVERSE_FFT1( unsigned int, char	)
INVERSE_FFT1( unsigned int, unsigned int )
INVERSE_FFT1( unsigned int, unsigned short )
INVERSE_FFT1( unsigned int, unsigned char )

INVERSE_FFT1( unsigned short, float	)
INVERSE_FFT1( unsigned short, double	)
INVERSE_FFT1( unsigned short, int	)
INVERSE_FFT1( unsigned short, short	)
INVERSE_FFT1( unsigned short, char	)
INVERSE_FFT1( unsigned short, unsigned int )
INVERSE_FFT1( unsigned short, unsigned short )
INVERSE_FFT1( unsigned short, unsigned char )

INVERSE_FFT1( unsigned char, unsigned char	)
INVERSE_FFT1( unsigned char, double	)
INVERSE_FFT1( unsigned char, int	)
INVERSE_FFT1( unsigned char, short	)
INVERSE_FFT1( unsigned char, char	)
INVERSE_FFT1( unsigned char, unsigned int )
INVERSE_FFT1( unsigned char, unsigned short )
INVERSE_FFT1( unsigned char, unsigned char )

#undef INVERSE_FFT1
#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array1< std::complex< T1 >, Allocator1 > &in, array1< std::complex< T2 >, Allocator2 > &out )
{
	int i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) * 2 );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.size( ) ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) / 2 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i * 2 ] = ( double ) in[ i ].real( );
		data[ i * 2 + 1 ] = ( double ) in[ i ].imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( in.size( ) * 2, 1, data, ip, w );

	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = std::complex< T2 >( ( T2 ) ( data[ 2 * i ] / in.size( ) ), ( T2 ) ( data[ 2 * i + 1 ] / in.size( ) ) );
	}

	free( w );
	free( ip );
	free( data );
}

#endif


//2次元高速フーリエ変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array2< T1, Allocator1 > &in, array2< std::complex< T2 >, Allocator2 > &out )
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
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ 2 * j ] = ( double ) in( i, j );
			data[ i ][ 2 * j + 1 ] = 0.0;
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft2d( in.width( ), in.height( ) * 2, -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = std::complex< T2 >( ( T2 ) data[ i ][ 2 * j ], ( T2 ) data[ i ][ 2 * j + 1 ] );

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


#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

#define FFT2( type1, type2 ) \
	template < >\
	void fft( array2< std::complex< type1 > > &in, array2< std::complex< type2 > > &out )\
	{\
		int i,j;\
		double **data;\
		double *t;\
		double *w;\
		int *ip;\
		if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )\
		{\
			return;\
		}\
		data = ( double **)malloc( sizeof( double *) * in.width() );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );\
		}\
		t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );\
		if( in.width() > in.height() )\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );\
		}\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			for( j = 0 ; j< in.height() ; j++ )\
			{\
				data[i][2*j] = ( double )in( i, j ).real();\
				data[i][2*j+1] = ( double )in( i, j ).imag();\
			}\
		}\
		ip[0] = 0;\
		ooura_fft::cdft2d( in.width(), in.height()*2, -1, data, t, ip, w );\
		out.resize( in.width(), in.height() );\
		for( i = 0 ; i < out.width() ; i++ )\
		{\
			for( j = 0 ; j < out.height() ; j++ )\
			{\
				out( i, j ) = std::complex< type2 >( (type2)data[i][2*j], (type2)data[i][2*j+1] );\
			}\
		}\
		free( t );\
		free( w );\
		free( ip );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			 free( data[i] );\
		}\
		free( data );\
	}\


	FFT2(	float,	float	)
FFT2( float, double	)
FFT2( float, int	)
FFT2( float, short	)
FFT2( float, char	)
FFT2( float, unsigned int )
FFT2( float, unsigned short )
FFT2( float, unsigned char )

FFT2( double, float	)
FFT2( double, double	)
FFT2( double, int	)
FFT2( double, short	)
FFT2( double, char	)
FFT2( double, unsigned int )
FFT2( double, unsigned short )
FFT2( double, unsigned char )

FFT2( int, float	)
FFT2( int, double	)
FFT2( int, int	)
FFT2( int, short	)
FFT2( int, char	)
FFT2( int, unsigned int )
FFT2( int, unsigned short )
FFT2( int, unsigned char )

FFT2( short, float	)
FFT2( short, double	)
FFT2( short, int	)
FFT2( short, short	)
FFT2( short, char	)
FFT2( short, unsigned int )
FFT2( short, unsigned short )
FFT2( short, unsigned char )

FFT2( char, float	)
FFT2( char, double	)
FFT2( char, int	)
FFT2( char, short	)
FFT2( char, char	)
FFT2( char, unsigned int )
FFT2( char, unsigned short )
FFT2( char, unsigned char )

FFT2( unsigned int, float	)
FFT2( unsigned int, double	)
FFT2( unsigned int, int	)
FFT2( unsigned int, short	)
FFT2( unsigned int, char	)
FFT2( unsigned int, unsigned int )
FFT2( unsigned int, unsigned short )
FFT2( unsigned int, unsigned char )

FFT2( unsigned short, float	)
FFT2( unsigned short, double	)
FFT2( unsigned short, int	)
FFT2( unsigned short, short	)
FFT2( unsigned short, char	)
FFT2( unsigned short, unsigned int )
FFT2( unsigned short, unsigned short )
FFT2( unsigned short, unsigned char )

FFT2( unsigned char, unsigned char	)
FFT2( unsigned char, double	)
FFT2( unsigned char, int	)
FFT2( unsigned char, short	)
FFT2( unsigned char, char	)
FFT2( unsigned char, unsigned int )
FFT2( unsigned char, unsigned short )
FFT2( unsigned char, unsigned char )

#undef FFT2
#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array2< std::complex< T1 >, Allocator1 > &in, array2< std::complex< T2 >, Allocator2 > &out )
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
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ 2 * j ] = ( double ) in( i, j ).real( );
			data[ i ][ 2 * j + 1 ] = ( double ) in( i, j ).imag( );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft2d( in.width( ), in.height( ) * 2, -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = std::complex< T2 >( ( T2 ) data[ i ][ 2 * j ], ( T2 ) data[ i ][ 2 * j + 1 ] );

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

#endif

//2次元逆フーリエ変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array2< std::complex< T1 >, Allocator1 > &in, array2< T2, Allocator2 > &out )
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
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ 2 * j ] = ( double ) in( i, j ).real( );
			data[ i ][ 2 * j + 1 ] = ( double ) in( i, j ).imag( );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft2d( in.width( ), in.height( ) * 2, 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = ( T2 ) ( data[ i ][ 2 * j ] / in.size( ) );

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


#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

#define INVERSE_FFT2( type1, type2 ) \
	void inverse_fft( array2< std::complex< type1 > > &in, array2< std::complex< type2 > > &out )\
	{\
		int i,j;\
		double **data;\
		double *t;\
		double *w;\
		int *ip;\
		if( !size_check( (unsigned int) in.width() ) || !size_check( (unsigned int) in.height() ) )\
		{\
			return;\
		}\
		data = ( double **)malloc( sizeof( double *) * in.width() );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			data[i] = ( double * )malloc( sizeof( double ) * in.height() * 2 );\
		}\
		t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT2D_MAX_THREADS );\
		if( in.width() > in.height() )\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );\
		}\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			for( j = 0 ; j< in.height() ; j++ )\
			{\
				data[i][2*j] = ( double )in( i, j ).real();\
				data[i][2*j+1] = ( double )in( i, j ).imag();\
			}\
		}\
		ip[0] = 0;\
		ooura_fft::cdft2d( in.width(), in.height()*2, 1, data, t, ip, w );\
		out.resize( in.width(), in.height() );\
		for( i = 0 ; i < out.width() ; i++ )\
		{\
			for( j = 0 ; j < out.height() ; j++ )\
			{\
				out( i, j ) =  std::complex< type2> ( (type2)( data[i][2*j] / in.size() ), (type2)( data[i][2*j+1] / in.size()  ));\
			}\
		}\
		free( t );\
		free( w );\
		free( ip );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			 free( data[i] );\
		}\
		free( data );\
	}\

	INVERSE_FFT2(	float,	float	)
INVERSE_FFT2( float, double	)
INVERSE_FFT2( float, int	)
INVERSE_FFT2( float, short	)
INVERSE_FFT2( float, char	)
INVERSE_FFT2( float, unsigned int )
INVERSE_FFT2( float, unsigned short )
INVERSE_FFT2( float, unsigned char )

INVERSE_FFT2( double, float	)
INVERSE_FFT2( double, double	)
INVERSE_FFT2( double, int	)
INVERSE_FFT2( double, short	)
INVERSE_FFT2( double, char	)
INVERSE_FFT2( double, unsigned int )
INVERSE_FFT2( double, unsigned short )
INVERSE_FFT2( double, unsigned char )

INVERSE_FFT2( int, float	)
INVERSE_FFT2( int, double	)
INVERSE_FFT2( int, int	)
INVERSE_FFT2( int, short	)
INVERSE_FFT2( int, char	)
INVERSE_FFT2( int, unsigned int )
INVERSE_FFT2( int, unsigned short )
INVERSE_FFT2( int, unsigned char )

INVERSE_FFT2( short, float	)
INVERSE_FFT2( short, double	)
INVERSE_FFT2( short, int	)
INVERSE_FFT2( short, short	)
INVERSE_FFT2( short, char	)
INVERSE_FFT2( short, unsigned int )
INVERSE_FFT2( short, unsigned short )
INVERSE_FFT2( short, unsigned char )

INVERSE_FFT2( char, float	)
INVERSE_FFT2( char, double	)
INVERSE_FFT2( char, int	)
INVERSE_FFT2( char, short	)
INVERSE_FFT2( char, char	)
INVERSE_FFT2( char, unsigned int )
INVERSE_FFT2( char, unsigned short )
INVERSE_FFT2( char, unsigned char )

INVERSE_FFT2( unsigned int, float	)
INVERSE_FFT2( unsigned int, double	)
INVERSE_FFT2( unsigned int, int	)
INVERSE_FFT2( unsigned int, short	)
INVERSE_FFT2( unsigned int, char	)
INVERSE_FFT2( unsigned int, unsigned int )
INVERSE_FFT2( unsigned int, unsigned short )
INVERSE_FFT2( unsigned int, unsigned char )

INVERSE_FFT2( unsigned short, float	)
INVERSE_FFT2( unsigned short, double	)
INVERSE_FFT2( unsigned short, int	)
INVERSE_FFT2( unsigned short, short	)
INVERSE_FFT2( unsigned short, char	)
INVERSE_FFT2( unsigned short, unsigned int )
INVERSE_FFT2( unsigned short, unsigned short )
INVERSE_FFT2( unsigned short, unsigned char )

INVERSE_FFT2( unsigned char, unsigned char	)
INVERSE_FFT2( unsigned char, double	)
INVERSE_FFT2( unsigned char, int	)
INVERSE_FFT2( unsigned char, short	)
INVERSE_FFT2( unsigned char, char	)
INVERSE_FFT2( unsigned char, unsigned int )
INVERSE_FFT2( unsigned char, unsigned short )
INVERSE_FFT2( unsigned char, unsigned char )

#undef INVERSE_FFT2

#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array2< std::complex< T1 >, Allocator1 > &in, array2< std::complex< T2 >, Allocator2 > &out )
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
		data[ i ] = ( double * ) malloc( sizeof( double ) * in.height( ) * 2 );
	}

	t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT2D_MAX_THREADS );

	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			data[ i ][ 2 * j ] = ( double ) in( i, j ).real( );
			data[ i ][ 2 * j + 1 ] = ( double ) in( i, j ).imag( );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft2d( in.width( ), in.height( ) * 2, 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = std::complex< T2 >( ( T2 ) ( data[ i ][ 2 * j ] / in.size( ) ), ( T2 ) ( data[ i ][ 2 * j + 1 ] / in.size( ) ) );
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

#endif


//3次元高速フーリエ変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array3< T1, Allocator1 > &in, array3< std::complex< T2 >, Allocator2 > &out )
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
			data[ i ][ j ] = ( double * ) malloc( sizeof( double ) * in.depth( ) * 2 );
		}
	}

	if( in.width( ) > in.height( ) )
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT3D_MAX_THREADS );
	}
	else
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.height( ) * FFT3D_MAX_THREADS );
	}


	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				data[ i ][ j ][ 2 * k ] = ( double ) in( i, j, k );
				data[ i ][ j ][ 2 * k + 1 ] = 0.0;
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft3d( in.width( ), in.height( ), in.depth( ) * 2, -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = std::complex< T2 >( ( T2 ) data[ i ][ j ][ 2 * k ], ( T2 ) data[ i ][ j ][ 2 * k + 1 ] );
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

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

#define FFT3( type1, type2 ) \
	template <>\
	void fft( array3< std::complex< type1 > > &in, array3< std::complex< type2 > > &out )\
	{\
		int i,j,k;\
		double ***data;\
		double *t;\
		double *w;\
		int *ip;\
		if( !size_check( (unsigned int) in.width() ) ||\
			!size_check( (unsigned int) in.height() ) ||\
			!size_check( (unsigned int) in.depth() ) )\
		{\
			return;\
		}\
		data = ( double ***)malloc( sizeof( double **) * in.width() );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			data[i] = ( double ** )malloc( sizeof( double *) * in.height() );\
			for( j = 0 ; j < in.height() ; j++ )\
			{\
				data[i][j] = ( double * )malloc( sizeof( double ) * in.depth() *2 );\
			}\
		}\
		if( in.width() > in.height() )\
		{\
			t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT3D_MAX_THREADS );\
		}\
		else\
		{\
			t = ( double * ) malloc( sizeof( double ) * 8 *in.height() * FFT3D_MAX_THREADS );\
		}\
		if( in.width() > in.height() )\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );\
		}\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			for( j = 0 ; j< in.height() ; j++ )\
			{\
				for( k = 0 ; k < in.depth() ; k++ )\
				{\
					data[i][j][2*k] = ( double )in( i, j, k ).real();\
					data[i][j][2*k+1] = ( double )in( i, j, k ).imag();\
				}\
			}\
		}\
		ip[0] = 0;\
		ooura_fft::cdft3d( in.width(), in.height(), in.depth()*2,  -1, data, t, ip, w );\
		out.resize( in.width(), in.height(), in.depth() );\
		for( i = 0 ; i < out.width() ; i++ )\
		{\
			for( j = 0 ; j < out.height() ; j++ )\
			{\
				for( k = 0 ; k < out.depth() ; k++ )\
				{\
					out( i, j, k ) = std::complex< type2 >( (type2)data[i][j][2*k], (type2)data[i][j][2*k+1] );\
				}\
			}\
		}\
		free( t );\
		free( w );\
		free( ip );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			for( j = 0 ; j < in.height() ; j++ )\
			{\
				free( data[i][j] );\
			}\
			 free( data[i] );\
		}\
		free( data );\
	}\

	FFT3(	float,	float	)
FFT3( float, double	)
FFT3( float, int	)
FFT3( float, short	)
FFT3( float, char	)
FFT3( float, unsigned int )
FFT3( float, unsigned short )
FFT3( float, unsigned char )

FFT3( double, float	)
FFT3( double, double	)
FFT3( double, int	)
FFT3( double, short	)
FFT3( double, char	)
FFT3( double, unsigned int )
FFT3( double, unsigned short )
FFT3( double, unsigned char )

FFT3( int, float	)
FFT3( int, double	)
FFT3( int, int	)
FFT3( int, short	)
FFT3( int, char	)
FFT3( int, unsigned int )
FFT3( int, unsigned short )
FFT3( int, unsigned char )

FFT3( short, float	)
FFT3( short, double	)
FFT3( short, int	)
FFT3( short, short	)
FFT3( short, char	)
FFT3( short, unsigned int )
FFT3( short, unsigned short )
FFT3( short, unsigned char )

FFT3( char, float	)
FFT3( char, double	)
FFT3( char, int	)
FFT3( char, short	)
FFT3( char, char	)
FFT3( char, unsigned int )
FFT3( char, unsigned short )
FFT3( char, unsigned char )

FFT3( unsigned int, float	)
FFT3( unsigned int, double	)
FFT3( unsigned int, int	)
FFT3( unsigned int, short	)
FFT3( unsigned int, char	)
FFT3( unsigned int, unsigned int )
FFT3( unsigned int, unsigned short )
FFT3( unsigned int, unsigned char )

FFT3( unsigned short, float	)
FFT3( unsigned short, double	)
FFT3( unsigned short, int	)
FFT3( unsigned short, short	)
FFT3( unsigned short, char	)
FFT3( unsigned short, unsigned int )
FFT3( unsigned short, unsigned short )
FFT3( unsigned short, unsigned char )

FFT3( unsigned char, unsigned char	)
FFT3( unsigned char, double	)
FFT3( unsigned char, int	)
FFT3( unsigned char, short	)
FFT3( unsigned char, char	)
FFT3( unsigned char, unsigned int )
FFT3( unsigned char, unsigned short )
FFT3( unsigned char, unsigned char )

#undef FFT3

#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft( array3< std::complex< T1 >, Allocator1 > &in, array3< std::complex< T2 >, Allocator2 > &out )
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
			data[ i ][ j ] = ( double * ) malloc( sizeof( double ) * in.depth( ) * 2 );
		}
	}

	if( in.width( ) > in.height( ) )
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT3D_MAX_THREADS );
	}
	else
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.height( ) * FFT3D_MAX_THREADS );
	}


	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				data[ i ][ j ][ 2 * k ] = ( double ) in( i, j, k ).real( );
				data[ i ][ j ][ 2 * k + 1 ] = ( double ) in( i, j, k ).imag( );
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft3d( in.width( ), in.height( ), in.depth( ) * 2, -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = std::complex< T2 >( ( T2 ) data[ i ][ j ][ 2 * k ], ( T2 ) data[ i ][ j ][ 2 * k + 1 ] );
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

#endif

//3次元高速フーリエ逆変換
template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array3< std::complex< T1 >, Allocator1 > &in, array3< T2 , Allocator2 > &out )
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
			data[ i ][ j ] = ( double * ) malloc( sizeof( double ) * in.depth( ) * 2 );
		}
	}

	if( in.width( ) > in.height( ) )
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT3D_MAX_THREADS );
	}
	else
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.height( ) * FFT3D_MAX_THREADS );
	}


	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				data[ i ][ j ][ 2 * k ] = ( double ) in( i, j, k ).real( );
				data[ i ][ j ][ 2 * k + 1 ] = ( double ) in( i, j, k ).imag( );
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft3d( in.width( ), in.height( ), in.depth( ) * 2, 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = ( T2 ) ( data[ i ][ j ][ 2 * k ] / ( in.size( ) ) );
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

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

#define INVERSE_FFT3( type1, type2 ) \
	template <>\
	void inverse_fft( array3< std::complex< type1 > > &in, array3< std::complex< type2 > > &out )\
	{\
		int i,j,k;\
		double ***data;\
		double *t;\
		double *w;\
		int *ip;\
		if( !size_check( (unsigned int) in.width() ) ||\
			!size_check( (unsigned int) in.height() ) ||\
			!size_check( (unsigned int) in.depth() ) )\
		{\
			return;\
		}\
		data = ( double ***)malloc( sizeof( double **) * in.width() );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			data[i] = ( double ** )malloc( sizeof( double *) * in.height() );\
			for( j = 0 ; j < in.height() ; j++ )\
			{\
				data[i][j] = ( double * )malloc( sizeof( double ) * in.depth() *2 );\
			}\
		}\
		if( in.width() > in.height() )\
		{\
			t = ( double * ) malloc( sizeof( double ) * 8 *in.width() * FFT3D_MAX_THREADS );\
		}\
		else\
		{\
			t = ( double * ) malloc( sizeof( double ) * 8 *in.height() * FFT3D_MAX_THREADS );\
		}\
		if( in.width() > in.height() )\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.width() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( in.height() ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.height() / 2 ) );\
		}\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			for( j = 0 ; j< in.height() ; j++ )\
			{\
				for( k = 0 ; k < in.depth() ; k++ )\
				{\
					data[i][j][2*k] = ( double )in( i, j, k ).real();\
					data[i][j][2*k+1] = ( double )in( i, j, k ).imag();\
				}\
			}\
		}\
		ip[0] = 0;\
		ooura_fft::cdft3d( in.width(), in.height(), in.depth()*2,  1, data, t, ip, w );\
		out.resize( in.width(), in.height(), in.depth() );\
		for( i = 0 ; i < out.width() ; i++ )\
		{\
			for( j = 0 ; j < out.height() ; j++ )\
			{\
				for( k = 0 ; k < out.depth() ; k++ )\
				{\
					out( i, j, k ) = std::complex< type2 >( (type2)(data[i][j][2*k]/in.size() ), (type2)(data[i][j][2*k+1]/in.size() ) );\
				}\
			}\
		}\
		free( t );\
		free( w );\
		free( ip );\
		for( i = 0 ; i < in.width() ; i++ )\
		{\
			for( j = 0 ; j < in.height() ; j++ )\
			{\
				free( data[i][j] );\
			}\
			 free( data[i] );\
		}\
		free( data );\
	}\

	INVERSE_FFT3(	float,	float	)
INVERSE_FFT3( float, double	)
INVERSE_FFT3( float, int	)
INVERSE_FFT3( float, short	)
INVERSE_FFT3( float, char	)
INVERSE_FFT3( float, unsigned int )
INVERSE_FFT3( float, unsigned short )
INVERSE_FFT3( float, unsigned char )

INVERSE_FFT3( double, float	)
INVERSE_FFT3( double, double	)
INVERSE_FFT3( double, int	)
INVERSE_FFT3( double, short	)
INVERSE_FFT3( double, char	)
INVERSE_FFT3( double, unsigned int )
INVERSE_FFT3( double, unsigned short )
INVERSE_FFT3( double, unsigned char )

INVERSE_FFT3( int, float	)
INVERSE_FFT3( int, double	)
INVERSE_FFT3( int, int	)
INVERSE_FFT3( int, short	)
INVERSE_FFT3( int, char	)
INVERSE_FFT3( int, unsigned int )
INVERSE_FFT3( int, unsigned short )
INVERSE_FFT3( int, unsigned char )

INVERSE_FFT3( short, float	)
INVERSE_FFT3( short, double	)
INVERSE_FFT3( short, int	)
INVERSE_FFT3( short, short	)
INVERSE_FFT3( short, char	)
INVERSE_FFT3( short, unsigned int )
INVERSE_FFT3( short, unsigned short )
INVERSE_FFT3( short, unsigned char )

INVERSE_FFT3( char, float	)
INVERSE_FFT3( char, double	)
INVERSE_FFT3( char, int	)
INVERSE_FFT3( char, short	)
INVERSE_FFT3( char, char	)
INVERSE_FFT3( char, unsigned int )
INVERSE_FFT3( char, unsigned short )
INVERSE_FFT3( char, unsigned char )

INVERSE_FFT3( unsigned int, float	)
INVERSE_FFT3( unsigned int, double	)
INVERSE_FFT3( unsigned int, int	)
INVERSE_FFT3( unsigned int, short	)
INVERSE_FFT3( unsigned int, char	)
INVERSE_FFT3( unsigned int, unsigned int )
INVERSE_FFT3( unsigned int, unsigned short )
INVERSE_FFT3( unsigned int, unsigned char )

INVERSE_FFT3( unsigned short, float	)
INVERSE_FFT3( unsigned short, double	)
INVERSE_FFT3( unsigned short, int	)
INVERSE_FFT3( unsigned short, short	)
INVERSE_FFT3( unsigned short, char	)
INVERSE_FFT3( unsigned short, unsigned int )
INVERSE_FFT3( unsigned short, unsigned short )
INVERSE_FFT3( unsigned short, unsigned char )

INVERSE_FFT3( unsigned char, unsigned char	)
INVERSE_FFT3( unsigned char, double	)
INVERSE_FFT3( unsigned char, int	)
INVERSE_FFT3( unsigned char, short	)
INVERSE_FFT3( unsigned char, char	)
INVERSE_FFT3( unsigned char, unsigned int )
INVERSE_FFT3( unsigned char, unsigned short )
INVERSE_FFT3( unsigned char, unsigned char )

#undef INVERSE_FFT3

#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void inverse_fft( array3< std::complex< T1 >, Allocator1 > &in, array3< std::complex< T2 >, Allocator2 > &out )
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
			data[ i ][ j ] = ( double * ) malloc( sizeof( double ) * in.depth( ) * 2 );
		}
	}

	if( in.width( ) > in.height( ) )
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.width( ) * FFT3D_MAX_THREADS );
	}
	else
	{
		t = ( double * ) malloc( sizeof( double ) * 8 * in.height( ) * FFT3D_MAX_THREADS );
	}


	if( in.width( ) > in.height( ) )
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.width( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( in.height( ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.height( ) / 2 ) );

	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				data[ i ][ j ][ 2 * k ] = ( double ) in( i, j, k ).real( );
				data[ i ][ j ][ 2 * k + 1 ] = ( double ) in( i, j, k ).imag( );
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft3d( in.width( ), in.height( ), in.depth( ) * 2, 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = std::complex< T2 >( ( T2 ) ( data[ i ][ j ][ 2 * k ] / in.size( ) ), ( T2 ) ( data[ i ][ j ][ 2 * k + 1 ] / in.size( ) ) );
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

#endif

_MIST_END

#endif

