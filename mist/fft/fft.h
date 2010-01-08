// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/// @file mist/fft/fft.h
//!
//! @brief �����t�[���G�ϊ��̃A���S���Y��
//!
//! ������w�� Takuya OOURA �����J�����ꂽ�CFFT�̃p�b�P�[�W�𗘗p���Ă��܂��D
//! 
//! http://momonga.t.u-tokyo.ac.jp/~ooura/fft.html
//!

#ifndef __INCLUDE_FFT_H__
#define __INCLUDE_FFT_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#ifndef __INCLUDE_FFT_UTIL_H__
#include "fft_util.h"
#endif


#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
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

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup fourier_group �t�[���G�ϊ�
//!  @{

//! @addtogroup fft_group �����t�[���G�ϊ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/fft/fft.h>
//! @endcode
//!
//!  @{



template < class T1, class T2, class Allocator1, class Allocator2 >
bool _fft( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	// 1���������t�[���G�ϊ�
	typedef typename Allocator1::size_type size_type;
	size_type i;
	__fft_util__::FFT_MEMORY1 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.size( ) * 2,
									static_cast< size_t >( std::sqrt( static_cast< double >( in.size( ) ) ) + 3 ),
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



template < class T1, class T2, class Allocator1, class Allocator2 >
bool _ifft( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out )
{
	if( !__fft_util__::size_check( ( unsigned int ) in.size( ) ) )
	{
		return( false );
	}

	// 1���������t�[���G�ϊ�
	typedef typename Allocator1::size_type size_type;
	size_type i;
	__fft_util__::FFT_MEMORY1 mem;

	if( !__fft_util__::allocate_memory( mem,
									in.size( ) * 2,
									static_cast< size_t >( std::sqrt( static_cast< double >( in.size( ) ) ) + 3 ),
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

	double __value__ = 1.0 / in.size( );
	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = __fft_util__::convert_complex< T2 >::convert_from( data[ 2 * i ] * __value__, data[ 2 * i + 1 ] * __value__ );
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}


/// @brief 1���������t�[���G�ϊ�
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention ���͉摜�̈�ӂ�2�̎w����̕K�v������
//! @attention �o�͂������݂̂̔z��̏ꍇ�́C�p���[��Ԃ�
//! 
//! @param[in]  in  �c ���͉摜
//! @param[out] out �c �o�͉摜
//! 
//! @return �ϊ��ɐ����������ǂ���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	return _fft( in, out );
}



/// @brief 1���������t�t�[���G�ϊ�
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention ���͉摜�̈�ӂ�2�̎w����̕K�v������
//! 
//! @param[in]  in  �c ���͉摜
//! @param[out] out �c �o�͉摜
//! 
//! @return �ϊ��ɐ����������ǂ���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool ifft( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out )
{
	return _ifft( in, out );
}



/// @brief 2���������t�[���G�ϊ�
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention ���͉摜�̈�ӂ�2�̎w����̕K�v������
//! @attention �o�͂������݂̂̉摜�z��̏ꍇ�́C�p���[��Ԃ�
//! 
//! @param[in]  in  �c ���͉摜
//! @param[out] out �c �o�͉摜
//! 
//! @return �ϊ��ɐ����������ǂ���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
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
									static_cast< size_t >( std::sqrt( static_cast< double >( size ) ) + 3 ),
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



/// @brief 2���������t�t�[���G�ϊ�
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention ���͉摜�̈�ӂ�2�̎w����̕K�v������
//! 
//! @param[in]  in  �c ���͉摜
//! @param[out] out �c �o�͉摜
//! 
//! @return �ϊ��ɐ����������ǂ���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool ifft( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
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
									static_cast< size_t >( std::sqrt( static_cast< double >( size ) ) + 3 ),
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

	double __value__ = 1.0 / in.size( );
	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			out( i, j ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ 2 * j ] * __value__, data[ i ][ 2 * j + 1 ] * __value__ );
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}



/// @brief 3���������t�[���G�ϊ�
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention ���͉摜�̈�ӂ�2�̎w����̕K�v������
//! @attention �o�͂������݂̂̉摜�z��̏ꍇ�́C�p���[��Ԃ�
//! 
//! @param[in]  in  �c ���͉摜
//! @param[out] out �c �o�͉摜
//! 
//! @return �ϊ��ɐ����������ǂ���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool fft( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
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
									static_cast< size_t >( std::sqrt( static_cast< double >( size ) ) + 3 ),
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

	ooura_fft::cdft3d( static_cast< int >( in.width( ) ), static_cast< int >( in.height( ) ), static_cast< int >( in.depth( ) * 2 ), -1, data, t, ip, w );

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




/// @brief 3���������t�t�[���G�ϊ�
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g�ł����������삷��
//! @attention ���͉摜�̈�ӂ�2�̎w����̕K�v������
//! 
//! @param[in]  in  �c ���͉摜
//! @param[out] out �c �o�͉摜
//! 
//! @return �ϊ��ɐ����������ǂ���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool ifft( const array3< T1, Allocator1 > &in, array3< T2 , Allocator2 > &out )
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
									static_cast< size_t >( std::sqrt( static_cast< double >( size ) ) + 3 ),
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

	ooura_fft::cdft3d( static_cast< int >( in.width( ) ), static_cast< int >( in.height( ) ), static_cast< int >( in.depth( ) * 2 ), 1, data, t, ip, w );

	out.resize( in.width( ), in.height( ), in.depth( ) );

	double __value__ = 1.0 / in.size( );
	for( i = 0 ; i < out.width( ) ; i++ )
	{
		for( j = 0 ; j < out.height( ) ; j++ )
		{
			for( k = 0 ; k < out.depth( ) ; k++ )
			{
				out( i, j, k ) = __fft_util__::convert_complex< T2 >::convert_from( data[ i ][ j ][ 2 * k ] * __value__, data[ i ][ j ][ 2 * k + 1 ] * __value__ );
			}
		}
	}

	__fft_util__::deallocate_memory( mem );

	return( true );
}


/// @}
//  FFT �O���[�v�̏I���

/// @}
//  Fourier �O���[�v�̏I���


_MIST_END

#endif

