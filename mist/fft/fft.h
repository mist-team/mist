/// @file mist/fft/fft.h
//!
//! @brief 高速フーリエ変換のアルゴリズム
//!

#ifndef __INCLUDE_FFT_H__
#define __INCLUDE_FFT_H__


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


//! @addtogroup fourier_group フーリエ変換
//!  @{

//! @addtogroup fft_group 高速フーリエ変換
//!  @{



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	// 1次元高速フーリエ変換
	typedef typename Allocator1::size_type size_type;
	size_type i;
	__fft_util__::FFT_MEMORY1 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.size( ) * 2,
									static_cast< size_t >( sqrt( static_cast< double >( in.size( ) ) ) + 3 ),
									in.size( ) / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double *data	= mem.data;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in[ i ] ) );
		data[ i * 2 ]     = c.real( );
		data[ i * 2 + 1 ] = c.imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( static_cast< int >( in.size( ) * 2 ), -1, data, ip, w );

	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = __fft_util__::convert_complex< T2 >::convert_from( data[ 2 * i ], data[ 2 * i + 1 ] );
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}



/// @brief 1次元逆フーリエ変換
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft_inverse( array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	// 1次元高速フーリエ変換
	typedef typename Allocator1::size_type size_type;
	size_type i;
	__fft_util__::FFT_MEMORY1 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.size( ) * 2,
									static_cast< size_t >( sqrt( static_cast< double >( in.size( ) ) ) + 3 ),
									in.size( ) / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double *data	= mem.data;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in[ i ] ) );
		data[ i * 2 ]     = c.real( );
		data[ i * 2 + 1 ] = c.imag( );
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft( static_cast< int >(in.size( ) * 2), 1, data, ip, w );

	out.resize( in.size( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = __fft_util__::convert_complex< T2 >::convert_from( data[ 2 * i ], data[ 2 * i + 1 ] ) / in.size( );
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}



/// @brief 2次元高速フーリエ変換
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
//! @return 戻り値の説明
//! 

template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft( array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) || !__fft_util__::size_check( ( unsigned int ) in.height( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY2 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ) * 2,
									8 * in.width( ) * FFT2D_MAX_THREADS,
									static_cast< size_t >( sqrt( static_cast< double >( size ) ) + 3 ),
									size / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double **data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j ) ) );
			data[ i ][ 2 * j ]     = c.real( );
			data[ i ][ 2 * j + 1 ] = c.imag( );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft2d( static_cast< int >( in.width( ) ), static_cast< int >( in.height( ) * 2 ), -1, data, t, ip, w );


	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ 2 * j ], data[ i ][ 2 * j + 1 ] );
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}




/// @brief 2次元逆フーリエ変換
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft_inverse( array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) || !__fft_util__::size_check( ( unsigned int ) in.height( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY2 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ) * 2,
									8 * in.width( ) * FFT2D_MAX_THREADS,
									static_cast< size_t >( sqrt( static_cast< double >( size ) ) + 3 ),
									size / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double **data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j ) ) );
			data[ i ][ 2 * j ]     = c.real( );
			data[ i ][ 2 * j + 1 ] = c.imag( );
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft2d( static_cast< int >(in.width( )), static_cast< int >(in.height( ) * 2), 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ 2 * j ], data[ i ][ 2 * j + 1 ] ) / in.size( );
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}



/// @brief 3次元高速フーリエ変換
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft( array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.height( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.depth( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, k, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY3 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ),
									in.depth( ) * 2,
									8 * size * FFT3D_MAX_THREADS,
									static_cast< size_t >( sqrt( static_cast< double >( size ) ) + 3 ),
									size / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double ***data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j, k ) ) );
				data[ i ][ j ][ 2 * k ]     = c.real( );
				data[ i ][ j ][ 2 * k + 1 ] = c.imag( );
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
				out( i, j, k ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ][ 2 * k ], data[ i ][ j ][ 2 * k + 1 ] );
			}
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}




/// @brief 3次元高速フーリエ逆変換
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft_inverse( array3< T1, Allocator1 > &in, array3< T2 , Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.width( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.height( ) ) ||
		!__fft_util__::size_check( ( unsigned int ) in.depth( ) ) )
	{
		return( false );
	}

	typedef typename Allocator1::size_type size_type;
	size_type i, j, k, size = in.width( ) > in.height( ) ? in.width( ) : in.height( );
	__fft_util__::FFT_MEMORY3 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.width( ),
									in.height( ),
									in.depth( ) * 2,
									8 * size * FFT3D_MAX_THREADS,
									static_cast< size_t >( sqrt( static_cast< double >( size ) ) + 3 ),
									size / 2 ) )
	{
		__fft_util__::deallocate_memory( mem );
		return( false );
	}

	double ***data	= mem.data;
	double *t		= mem.t;
	double *w		= mem.w;
	int    *ip		= mem.ip;

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( k = 0 ; k < in.depth( ) ; k++ )
			{
				std::complex< double > c( __fft_util__::convert_complex< T1 >::convert_to( in( i, j, k ) ) );
				data[ i ][ j ][ 2 * k ]     = c.real( );
				data[ i ][ j ][ 2 * k + 1 ] = c.imag( );
			}
		}
	}

	ip[ 0 ] = 0;

	ooura_fft::cdft3d( static_cast< int >(in.width( )), static_cast< int >( in.height( ) ), static_cast< int >( in.depth( ) * 2 ), 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ][ 2 * k ], data[ i ][ j ][ 2 * k + 1 ] ) / ( in.size( ) );
			}
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}


/// @}
//  FFT グループの終わり

/// @}
//  Fourier グループの終わり


_MIST_END

#endif

