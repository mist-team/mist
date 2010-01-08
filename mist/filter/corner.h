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

/// @file mist/filter/edge.h
//!
//! @brief 2�����̉摜�ɂ���G�b�W��f�����o���郉�C�u����
//!

#ifndef __INCLUDE_MIST_CORNER__
#define __INCLUDE_MIST_CORNER__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "../vector.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "../vector.h"
#endif

#ifndef __INCLUDE_CONVERTER__
#include "../converter.h"
#endif

#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#include "linear.h"
#endif

#include <vector>
#include <algorithm>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __corner_utility__
{
	struct corner_position
	{
		int    x;
		int    y;
		double key;

		/// @brief �f�t�H���g�̃R���X�g���N�^
		corner_position( ){ }

		/// @brief �w�肵���L�[�ƒl�ŏ���������
		corner_position( int xx, int yy, double k ) : x( xx ), y( yy ), key( k ){ }

		/// @brief ���̓����ʂƒl�̑召���r����
		bool operator <( const corner_position &f ) const
		{
			return( key < f.key );
		}

		/// @brief �����Ńf�[�^����ׂ�
		static bool greater( const corner_position &f1, const corner_position &f2 )
		{
			return( f2 < f1 );
		}
	};
}


//! @addtogroup corner_group �R�[�i�[���o�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/corner.h>
//! @endcode
//!
//!  @{


/// @brief Harris�̃R�[�i�[���o�t�B���^
//! 
//! @attention OpenMP���L���ȏꍇ�͎����I�ɕ���v�Z�����s�����
//! 
//! @param[in]  in           �c ���͉摜
//! @param[out] out          �c �o�͉摜
//! @param[in]  max_corners  �c ���o����R�[�i�[�̐�
//! @param[in]  min_distance �c �R�[�i�[�Ԃ̋����̍ŏ��l
//! @param[in]  kappa        �c �R�[�i�[�����o����ۂ̏����Ɏg�p����W��0.04�`0.15�i�f�t�H���g��0.04�j
//! @param[in]  window_size  �c �R�[�i�[���o���s���s������߂�ۂ̑��T�C�Y�i�f�t�H���g��3�j
//! @param[in]  fgval        �c �G�b�W��f�i�O�i�j�ɑ������l�i�f�t�H���g��255�j
//! @param[in]  bgval        �c �w�i��f�ɑ������l�i�f�t�H���g��0�j
//! 
//! @return ���o�����R�[�i�[��
//! 
template < class T, class Allocator, template < typename, typename > class LIST, class TT, class AAllocator >
typename array2< T, Allocator >::difference_type harris( const array2< T, Allocator > &in, LIST< vector2< TT >, AAllocator >&out, typename array2< T, Allocator >::size_type max_corners,
														 double min_distance, double kappa = 0.04, typename array2< T, Allocator >::size_type window_size = 3,
														 const typename array2< T, Allocator >::value_type &fgval = typename array2< T, Allocator >::value_type( 255 ),
														 const typename array2< T, Allocator >::value_type &bgval = typename array2< T, Allocator >::value_type( 0 ) )
{
	typedef typename array2< T, Allocator >::size_type       size_type;
	typedef typename array2< T, Allocator >::difference_type difference_type;

	if( in.empty( ) )
	{
		return( -1 );
	}

	typedef array2< double > image_type;
	typedef vector2< TT > ovector_type;
	typedef rgb< double > vector_type;
	typedef array2< vector_type > vector_image_type;
	typedef array2< unsigned char > mask_type;

	image_type tmp, k1( 3, 3 ), k2( 3, 3 ), gx, gy;

	convert( in, tmp );

	k1( 0, 0 ) = -1; k1( 1, 0 ) =  0; k1( 2, 0 ) =  1;
	k1( 0, 1 ) = -2; k1( 1, 1 ) =  0; k1( 2, 1 ) =  2;
	k1( 0, 2 ) = -1; k1( 1, 2 ) =  0; k1( 2, 2 ) =  1;

	k2( 0, 0 ) = -1; k2( 1, 0 ) = -2; k2( 2, 0 ) = -1;
	k2( 0, 1 ) =  0; k2( 1, 1 ) =  0; k2( 2, 1 ) =  0;
	k2( 0, 2 ) = +1; k2( 1, 2 ) = +2; k2( 2, 2 ) = +1;

	linear_filter( tmp, gx, k1 );
	linear_filter( tmp, gy, k2 );

	vector_image_type work( in.width( ), in.height( ) ), wwork;

#ifdef _OPENMP
	#pragma omp parallel for schedule( guided )
#endif
	for( int j = 0 ; j < static_cast< int >( tmp.height( ) ) ; j++ )
	{
		for( size_type i = 0 ; i < tmp.width( ) ; i++ )
		{
			double dx = gx( i, j );
			double dy = gy( i, j );

			work( i, j ) = vector_type( dx * dx, dy * dy, dx * dy );
		}
	}

#if 0
	window_size /= 2;
	gaussian_filter( work, wwork, window_size * 0.5 );
#else
	average_filter( work, wwork, window_size, window_size );
#endif

	mask_type mask( in.width( ), in.height( ) );

	typedef __corner_utility__::corner_position point_type;
	typedef std::vector< point_type > point_list_type;
	point_list_type point_list, out_list;

	for( size_type j = 1 ; j < wwork.height( ) - 1 ; j++ )
	{
		for( size_type i = 1 ; i < wwork.width( ) - 1 ; i++ )
		{
			const vector_type &v = wwork( i, j );
			double Mc = v.r * v.g - v.b * v.b - kappa * ( v.r + v.g ) * ( v.r + v.g );

			if( Mc > 0 )
			{
				point_list.push_back( point_type( i, j, Mc ) );
			}
		}
	}

	std::sort( point_list.begin( ), point_list.end( ), __corner_utility__::corner_position::greater );

	min_distance = min_distance * min_distance;
	for( size_type i = 0 ; i < point_list.size( ) && out_list.size( ) < max_corners ; i++ )
	{
		const point_type &v = point_list[ i ];

		bool flag = true;
		for( size_type l = 0 ; l < out_list.size( ) ; l++ )
		{
			const point_type &u = out_list[ l ];
			if( ( v.x - u.x ) * ( v.x - u.x ) + ( v.y - u.y ) * ( v.y - u.y ) < min_distance )
			{
				flag = false;
				break;
			}
		}

		if( flag )
		{
			out_list.push_back( v );
		}
	}

	out.clear( );

	for( size_type i = 0 ; i < out_list.size( ) ; i++ )
	{
		out.push_back( ovector_type( out_list[ i ].x, out_list[ i ].y ) );
	}

	return( out.size( ) );
}



/// @}
//  �R�[�i�[���o�t�B���^�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_CORNER__
