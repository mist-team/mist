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
	typedef typename Allocator1::size_type size_type;
	size_type i;
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

	ooura_fft::cdft( static_cast< int >(in.size( ) * 2), -1, data, ip, w );


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
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.size()) ) + 3 ) );\
		w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );\
		for( i = 0 ; i < in.size() ; i++ )\
		{\
			data[i*2] = ( double )in[i].real();\
			data[i*2+1] = ( double )in[i].imag();\
		}\
		ip[0] = 0;\
		ooura_fft::cdft( static_cast< int >(in.size()*2), -1, data, ip, w );\
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
	typedef typename Allocator1::size_type size_type;
	size_type i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}


	data = ( double * ) malloc( sizeof( double ) * in.size( ) * 2 );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.size( )) ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) / 2 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i * 2 ] = ( double ) in[ i ].real( );
		data[ i * 2 + 1 ] = ( double ) in[ i ].imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( static_cast< int >(in.size( ) * 2), -1, data, ip, w );


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
void fft_inverse( array1< std::complex< T1 >, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	size_type i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) * 2 );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.size( )) ) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) / 2 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i * 2 ] = ( double ) in[ i ].real( );
		data[ i * 2 + 1 ] = ( double ) in[ i ].imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( static_cast< int >(in.size( ) * 2), 1, data, ip, w );

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

#define fft_inverse1( type1, type2 ) \
	template < >\
	void fft_inverse( array1< std::complex< type1 > > &in, array1< std::complex< type2 > > &out )\
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
		ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.size() )) + 3 ) );\
		w = ( double * )malloc(sizeof( double ) * (int)( in.size() / 2 ) );\
		for( i = 0 ; i < in.size() ; i++ )\
		{\
			data[i*2] = ( double )in[i].real();\
			data[i*2+1] = ( double )in[i].imag();\
		}\
		ip[0] = 0;\
		ooura_fft::cdft( static_cast< int >(in.size() * 2), 1, data, ip, w );\
		out.resize( in.size() );\
		for( i = 0 ; i < out.size() ; i++ )\
		{\
			out[i] = std::complex< type2 >( (type2)(data[2*i]/in.size() ), (type2)(data[2*i+1]/in.size()) );\
		}\
		free( w );\
		free( ip );\
		free( data );\
	}\

	fft_inverse1(	float,	float	)
fft_inverse1( float, double	)
fft_inverse1( float, int	)
fft_inverse1( float, short	)
fft_inverse1( float, char	)
fft_inverse1( float, unsigned int )
fft_inverse1( float, unsigned short )
fft_inverse1( float, unsigned char )

fft_inverse1( double, float	)
fft_inverse1( double, double	)
fft_inverse1( double, int	)
fft_inverse1( double, short	)
fft_inverse1( double, char	)
fft_inverse1( double, unsigned int )
fft_inverse1( double, unsigned short )
fft_inverse1( double, unsigned char )

fft_inverse1( int, float	)
fft_inverse1( int, double	)
fft_inverse1( int, int	)
fft_inverse1( int, short	)
fft_inverse1( int, char	)
fft_inverse1( int, unsigned int )
fft_inverse1( int, unsigned short )
fft_inverse1( int, unsigned char )

fft_inverse1( short, float	)
fft_inverse1( short, double	)
fft_inverse1( short, int	)
fft_inverse1( short, short	)
fft_inverse1( short, char	)
fft_inverse1( short, unsigned int )
fft_inverse1( short, unsigned short )
fft_inverse1( short, unsigned char )

fft_inverse1( char, float	)
fft_inverse1( char, double	)
fft_inverse1( char, int	)
fft_inverse1( char, short	)
fft_inverse1( char, char	)
fft_inverse1( char, unsigned int )
fft_inverse1( char, unsigned short )
fft_inverse1( char, unsigned char )

fft_inverse1( unsigned int, float	)
fft_inverse1( unsigned int, double	)
fft_inverse1( unsigned int, int	)
fft_inverse1( unsigned int, short	)
fft_inverse1( unsigned int, char	)
fft_inverse1( unsigned int, unsigned int )
fft_inverse1( unsigned int, unsigned short )
fft_inverse1( unsigned int, unsigned char )

fft_inverse1( unsigned short, float	)
fft_inverse1( unsigned short, double	)
fft_inverse1( unsigned short, int	)
fft_inverse1( unsigned short, short	)
fft_inverse1( unsigned short, char	)
fft_inverse1( unsigned short, unsigned int )
fft_inverse1( unsigned short, unsigned short )
fft_inverse1( unsigned short, unsigned char )

fft_inverse1( unsigned char, unsigned char	)
fft_inverse1( unsigned char, double	)
fft_inverse1( unsigned char, int	)
fft_inverse1( unsigned char, short	)
fft_inverse1( unsigned char, char	)
fft_inverse1( unsigned char, unsigned int )
fft_inverse1( unsigned char, unsigned short )
fft_inverse1( unsigned char, unsigned char )

#undef fft_inverse1
#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft_inverse( array1< std::complex< T1 >, Allocator1 > &in, array1< std::complex< T2 >, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	size_type i;
	double *data;
	double *w;
	int *ip;

	if( !size_check( ( unsigned int ) in.size( ) ) )
	{
		return ;
	}

	data = ( double * ) malloc( sizeof( double ) * in.size( ) * 2 );
	ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.size( ) )) + 3 ) );
	w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.size( ) / 2 ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		data[ i * 2 ] = ( double ) in[ i ].real( );
		data[ i * 2 + 1 ] = ( double ) in[ i ].imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( static_cast< int >(in.size( ) * 2), 1, data, ip, w );

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
	typedef typename Allocator1::size_type size_type;
	size_type i,j;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) )) + 3 ) );
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

	ooura_fft::cdft2d( static_cast< int >(in.width( )), static_cast< int >(in.height( ) * 2), -1, data, t, ip, w );


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
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.width() )) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.height() )) + 3 ) );\
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
		ooura_fft::cdft2d( static_cast< int >(in.width()), static_cast< int >(in.height()*2), -1, data, t, ip, w );\
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
	typedef typename Allocator1::size_type size_type;
	size_type i,j;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) )) + 3 ) );
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

	ooura_fft::cdft2d( static_cast< int >(in.width( )), static_cast< int >(in.height( ) * 2), -1, data, t, ip, w );


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
void fft_inverse( array2< std::complex< T1 >, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	size_type i,j;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) ) ) + 3 ) );
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

	ooura_fft::cdft2d( static_cast< int >(in.width( )), static_cast< int >(in.height( ) * 2), 1, data, t, ip, w );

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

#define fft_inverse2( type1, type2 ) \
	void fft_inverse( array2< std::complex< type1 > > &in, array2< std::complex< type2 > > &out )\
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
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.width() ) ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.height() ) ) + 3 ) );\
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
		ooura_fft::cdft2d( static_cast< int >(in.width()), static_cast< int >(in.height()*2), 1, data, t, ip, w );\
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

	fft_inverse2(	float,	float	)
fft_inverse2( float, double	)
fft_inverse2( float, int	)
fft_inverse2( float, short	)
fft_inverse2( float, char	)
fft_inverse2( float, unsigned int )
fft_inverse2( float, unsigned short )
fft_inverse2( float, unsigned char )

fft_inverse2( double, float	)
fft_inverse2( double, double	)
fft_inverse2( double, int	)
fft_inverse2( double, short	)
fft_inverse2( double, char	)
fft_inverse2( double, unsigned int )
fft_inverse2( double, unsigned short )
fft_inverse2( double, unsigned char )

fft_inverse2( int, float	)
fft_inverse2( int, double	)
fft_inverse2( int, int	)
fft_inverse2( int, short	)
fft_inverse2( int, char	)
fft_inverse2( int, unsigned int )
fft_inverse2( int, unsigned short )
fft_inverse2( int, unsigned char )

fft_inverse2( short, float	)
fft_inverse2( short, double	)
fft_inverse2( short, int	)
fft_inverse2( short, short	)
fft_inverse2( short, char	)
fft_inverse2( short, unsigned int )
fft_inverse2( short, unsigned short )
fft_inverse2( short, unsigned char )

fft_inverse2( char, float	)
fft_inverse2( char, double	)
fft_inverse2( char, int	)
fft_inverse2( char, short	)
fft_inverse2( char, char	)
fft_inverse2( char, unsigned int )
fft_inverse2( char, unsigned short )
fft_inverse2( char, unsigned char )

fft_inverse2( unsigned int, float	)
fft_inverse2( unsigned int, double	)
fft_inverse2( unsigned int, int	)
fft_inverse2( unsigned int, short	)
fft_inverse2( unsigned int, char	)
fft_inverse2( unsigned int, unsigned int )
fft_inverse2( unsigned int, unsigned short )
fft_inverse2( unsigned int, unsigned char )

fft_inverse2( unsigned short, float	)
fft_inverse2( unsigned short, double	)
fft_inverse2( unsigned short, int	)
fft_inverse2( unsigned short, short	)
fft_inverse2( unsigned short, char	)
fft_inverse2( unsigned short, unsigned int )
fft_inverse2( unsigned short, unsigned short )
fft_inverse2( unsigned short, unsigned char )

fft_inverse2( unsigned char, unsigned char	)
fft_inverse2( unsigned char, double	)
fft_inverse2( unsigned char, int	)
fft_inverse2( unsigned char, short	)
fft_inverse2( unsigned char, char	)
fft_inverse2( unsigned char, unsigned int )
fft_inverse2( unsigned char, unsigned short )
fft_inverse2( unsigned char, unsigned char )

#undef fft_inverse2

#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft_inverse( array2< std::complex< T1 >, Allocator1 > &in, array2< std::complex< T2 >, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	size_type i,j;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) )) + 3 ) );
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

	ooura_fft::cdft2d( static_cast< int >(in.width( )), static_cast< int >(in.height( ) * 2), 1, data, t, ip, w );

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
	typedef typename Allocator1::size_type size_type;
	size_type i,j,k;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) )) + 3 ) );
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

	ooura_fft::cdft3d( static_cast< int >(in.width( )), static_cast< int >(in.height( )), static_cast< int >(in.depth( ) * 2), -1, data, t, ip, w );


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
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.width()) ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.height()) ) + 3 ) );\
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
		ooura_fft::cdft3d( static_cast< int >(in.width()), static_cast< int >(in.height()), static_cast< int >(in.depth()*2),  -1, data, t, ip, w );\
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
	typedef typename Allocator1::size_type size_type;
	size_type i,j,k;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) ) ) + 3 ) );
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

	ooura_fft::cdft3d( static_cast< int >(in.width( )), static_cast< int >(in.height( )), static_cast< int >(in.depth( )) * 2, -1, data, t, ip, w );


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
void fft_inverse( array3< std::complex< T1 >, Allocator1 > &in, array3< T2 , Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	size_type i,j,k;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.width( ) ) ) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( in.width( ) / 2 ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( sqrt( static_cast< double >(in.height( ) )) + 3 ) );
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

	ooura_fft::cdft3d( static_cast< int >(in.width( )), static_cast< int >(in.height( )), static_cast< int >(in.depth( ) * 2), 1, data, t, ip, w );

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

#define fft_inverse3( type1, type2 ) \
	template <>\
	void fft_inverse( array3< std::complex< type1 > > &in, array3< std::complex< type2 > > &out )\
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
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.width()) ) + 3 ) );\
			w = ( double * )malloc( sizeof( double ) * (int)( in.width() / 2 ) );\
		}\
		else\
		{\
			ip = ( int * )malloc( sizeof( int ) * (int)( sqrt( static_cast< double >(in.height()) ) + 3 ) );\
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
		ooura_fft::cdft3d( static_cast< int >(in.width()), static_cast< int >(in.height()), static_cast< int >(in.depth()*2),  1, data, t, ip, w );\
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

	fft_inverse3(	float,	float	)
fft_inverse3( float, double	)
fft_inverse3( float, int	)
fft_inverse3( float, short	)
fft_inverse3( float, char	)
fft_inverse3( float, unsigned int )
fft_inverse3( float, unsigned short )
fft_inverse3( float, unsigned char )

fft_inverse3( double, float	)
fft_inverse3( double, double	)
fft_inverse3( double, int	)
fft_inverse3( double, short	)
fft_inverse3( double, char	)
fft_inverse3( double, unsigned int )
fft_inverse3( double, unsigned short )
fft_inverse3( double, unsigned char )

fft_inverse3( int, float	)
fft_inverse3( int, double	)
fft_inverse3( int, int	)
fft_inverse3( int, short	)
fft_inverse3( int, char	)
fft_inverse3( int, unsigned int )
fft_inverse3( int, unsigned short )
fft_inverse3( int, unsigned char )

fft_inverse3( short, float	)
fft_inverse3( short, double	)
fft_inverse3( short, int	)
fft_inverse3( short, short	)
fft_inverse3( short, char	)
fft_inverse3( short, unsigned int )
fft_inverse3( short, unsigned short )
fft_inverse3( short, unsigned char )

fft_inverse3( char, float	)
fft_inverse3( char, double	)
fft_inverse3( char, int	)
fft_inverse3( char, short	)
fft_inverse3( char, char	)
fft_inverse3( char, unsigned int )
fft_inverse3( char, unsigned short )
fft_inverse3( char, unsigned char )

fft_inverse3( unsigned int, float	)
fft_inverse3( unsigned int, double	)
fft_inverse3( unsigned int, int	)
fft_inverse3( unsigned int, short	)
fft_inverse3( unsigned int, char	)
fft_inverse3( unsigned int, unsigned int )
fft_inverse3( unsigned int, unsigned short )
fft_inverse3( unsigned int, unsigned char )

fft_inverse3( unsigned short, float	)
fft_inverse3( unsigned short, double	)
fft_inverse3( unsigned short, int	)
fft_inverse3( unsigned short, short	)
fft_inverse3( unsigned short, char	)
fft_inverse3( unsigned short, unsigned int )
fft_inverse3( unsigned short, unsigned short )
fft_inverse3( unsigned short, unsigned char )

fft_inverse3( unsigned char, unsigned char	)
fft_inverse3( unsigned char, double	)
fft_inverse3( unsigned char, int	)
fft_inverse3( unsigned char, short	)
fft_inverse3( unsigned char, char	)
fft_inverse3( unsigned char, unsigned int )
fft_inverse3( unsigned char, unsigned short )
fft_inverse3( unsigned char, unsigned char )

#undef fft_inverse3

#else

template < class T1, class T2, class Allocator1, class Allocator2 >
void fft_inverse( array3< std::complex< T1 >, Allocator1 > &in, array3< std::complex< T2 >, Allocator2 > &out )
{
	typedef typename Allocator1::size_type size_type;
	size_type i,j,k;
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
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( (int)sqrt( static_cast< double >(in.width( ) )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( (int)( in.width( ) / 2 ) ) );
	}
	else
	{
		ip = ( int * ) malloc( sizeof( int ) * ( int ) ( (int)sqrt( static_cast< double >(in.height( ) )) + 3 ) );
		w = ( double * ) malloc( sizeof( double ) * ( int ) ( (int)( in.height( ) / 2 ) ) );

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

	ooura_fft::cdft3d( static_cast< int >(in.width( )), static_cast< int >(in.height( )), static_cast< int >(in.depth( ) * 2), 1, data, t, ip, w );

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

