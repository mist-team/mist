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

#ifndef __INCLUDE_MIST_EDGE__
#define __INCLUDE_MIST_EDGE__


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
#include <cmath>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @addtogroup edge_group �G�b�W���o�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/edge.h>
//! @endcode
//!
//!  @{


/// @brief Canny�̃G�b�W���o�t�B���^
//! 
//! @attention ���͂Əo�͂́C�����I�u�W�F�N�g�ł����������삷��
//! @attention OpenMP���L���ȏꍇ�͎����I�ɕ���v�Z�����s�����
//! 
//! @param[in]  in            �c ���͉摜
//! @param[out] out           �c �o�͉摜
//! @param[in]  lower         �c �q�X�e���V�X�������l�����̉���
//! @param[in]  upper         �c �q�X�e���V�X�������l�����̏��
//! @param[in]  useL2gradient �c L2�̃O���f�B�G���g���v�Z���邩�ǂ����i�f�t�H���g��false�j
//! @param[in]  fgval         �c �G�b�W��f�i�O�i�j�ɑ������l�i�f�t�H���g��255�j
//! @param[in]  bgval         �c �w�i��f�ɑ������l�i�f�t�H���g��0�j
//! @param[in]  thread_num    �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline bool canny( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, double lower, double upper, bool useL2gradient = false, const typename array2< T2, Allocator2 >::value_type &fgval = typename array2< T2, Allocator2 >::value_type( 255 ), const typename array2< T2, Allocator2 >::value_type &bgval = typename array2< T2, Allocator2 >::value_type( 0 ), typename array2< T2, Allocator2 >::size_type thread_num = 0 )
{
	typedef typename array2< T1, Allocator1 >::size_type       size_type;
	typedef typename array2< T1, Allocator1 >::difference_type difference_type;
	if( in.empty( ) )
	{
		return( false );
	}

	// �l������ւ���Ă���ꍇ�ւ̑Ώ�
	if( lower > upper )
	{
		double ttt = lower;
		lower = upper;
		upper = ttt;
	}

	typedef array2< double > image_type;
	typedef array2< unsigned char > mask_type;

	image_type tmp, k1( 3, 3 ), k2( 3, 3 ), gx, gy;

	convert( in, tmp );

	k1( 0, 0 ) = -1; k1( 1, 0 ) =  0; k1( 2, 0 ) =  1;
	k1( 0, 1 ) = -2; k1( 1, 1 ) =  0; k1( 2, 1 ) =  2;
	k1( 0, 2 ) = -1; k1( 1, 2 ) =  0; k1( 2, 2 ) =  1;

	k2( 0, 0 ) = -1; k2( 1, 0 ) = -2; k2( 2, 0 ) = -1;
	k2( 0, 1 ) =  0; k2( 1, 1 ) =  0; k2( 2, 1 ) =  0;
	k2( 0, 2 ) = +1; k2( 1, 2 ) = +2; k2( 2, 2 ) = +1;

	linear_filter( tmp, gx, k1, thread_num );
	linear_filter( tmp, gy, k2, thread_num );

	if( useL2gradient )
	{
#ifdef _OPENMP
		#pragma omp parallel for
#endif
		for( int i = 0 ; i < static_cast< int >( tmp.size( ) ) ; i++ )
		{
			tmp[ i ] = std::sqrt( gx[ i ] * gx[ i ] + gy[ i ] * gy[ i ] );
		}
	}
	else
	{
#ifdef _OPENMP
		#pragma omp parallel for
#endif
		for( int i = 0 ; i < static_cast< int >( tmp.size( ) ) ; i++ )
		{
			tmp[ i ] = std::abs( gx[ i ] ) + std::abs( gy[ i ] );
		}
	}

	mask_type mask( in.width( ), in.height( ) );

#ifdef _OPENMP
	#pragma omp parallel for schedule( guided )
#endif
	for( int j = 1 ; j < static_cast< int >( tmp.height( ) - 1 ) ; j++ )
	{
		for( size_type i = 1 ; i < tmp.width( ) - 1 ; i++ )
		{
			double dx = gx( i, j );
			double dy = gy( i, j );
			double sign = dx * dy < 0 ? -1 : +1;

			dx = std::abs( dx );
			dy = std::abs( dy );

			double val = tmp( i, j );

			if( val > lower )
			{
				// dy / dx < tan( 22.5 / 180.0 / 3.1415926535897932384626433832795 )
				if( dy < dx * 0.4142135623730950488016887242097 )
				{
					// 0�x
					if( val > tmp( i - 1, j ) && val >= tmp( i + 1, j ) )
					{
						if( val > upper )
						{
							mask( i, j ) = 2;
						}
						else
						{
							mask( i, j ) = 1;
						}
					}
				}
				// dy / dx < tan( ( 22.5 + 45.0 ) / 180.0 / 3.1415926535897932384626433832795 )
				else if( dy > dx * 2.4142135623730950488016887242097 )
				{
					// 90�x
					if( val > tmp( i, j - 1 ) && val >= tmp( i, j + 1 ) )
					{
						if( val > upper )
						{
							mask( i, j ) = 2;
						}
						else
						{
							mask( i, j ) = 1;
						}
					}
				}
				else
				{
					if( sign < 0 )
					{
						// 45�x
						if( val > tmp( i - 1, j + 1 ) && val > tmp( i + 1, j - 1 ) )
						{
							if( val > upper )
							{
								mask( i, j ) = 2;
							}
							else
							{
								mask( i, j ) = 1;
							}
						}
					}
					else
					{
						// 135�x
						if( val > tmp( i - 1, j - 1 ) && val > tmp( i + 1, j + 1 ) )
						{
							if( val > upper )
							{
								mask( i, j ) = 2;
							}
							else
							{
								mask( i, j ) = 1;
							}
						}
					}
				}
			}
		}
	}

	typedef vector2< size_type > vector_type;
	typedef std::list< vector_type > point_list_type;
	point_list_type point_list;

	for( size_type j = 1 ; j < tmp.height( ) - 1 ; j++ )
	{
		for( size_type i = 1 ; i < tmp.width( ) - 1 ; i++ )
		{
			if( mask( i, j ) == 1 )
			{
				if( mask( i - 1, j - 1 ) == 2 || mask( i, j - 1 ) == 2 || mask( i + 1, j - 1 ) == 2 || 
					mask( i - 1, j     ) == 2                          || mask( i + 1, j     ) == 2 || 
					mask( i - 1, j + 1 ) == 2 || mask( i, j + 1 ) == 2 || mask( i + 1, j + 1 ) == 2 )
				{
					mask( i, j ) = 2;
					point_list.push_back( vector_type( i, j ) );
				}
			}
		}
	}

	while( !point_list.empty( ) )
	{
		vector_type v = point_list.front( );
		point_list.pop_front( );
		size_type i = v.x;
		size_type j = v.y;

		if( mask( i - 1, j - 1 ) == 1 )
		{
			mask( i - 1, j - 1 ) = 2;
			point_list.push_back( vector_type( i - 1, j - 1 ) );
		}
		if( mask( i, j - 1 ) == 1 )
		{
			mask( i, j - 1 ) = 2;
			point_list.push_back( vector_type( i, j - 1 ) );
		}
		if( mask( i + 1, j - 1 ) == 1 )
		{
			mask( i + 1, j - 1 ) = 2;
			point_list.push_back( vector_type( i + 1, j - 1 ) );
		}
		if( mask( i - 1, j ) == 1 )
		{
			mask( i - 1, j ) = 2;
			point_list.push_back( vector_type( i - 1, j ) );
		}
		if( mask( i + 1, j ) == 1 )
		{
			mask( i + 1, j ) = 2;
			point_list.push_back( vector_type( i + 1, j ) );
		}
		if( mask( i - 1, j + 1 ) == 1 )
		{
			mask( i - 1, j + 1 ) = 2;
			point_list.push_back( vector_type( i - 1, j + 1 ) );
		}
		if( mask( i, j + 1 ) == 1 )
		{
			mask( i, j + 1 ) = 2;
			point_list.push_back( vector_type( i, j + 1 ) );
		}
		if( mask( i + 1, j + 1 ) == 1 )
		{
			mask( i + 1, j + 1 ) = 2;
			point_list.push_back( vector_type( i + 1, j + 1 ) );
		}
	}

	out.resize( in.width( ), in.height( ) );
	out.reso( in.reso1( ), in.reso2( ) );

#ifdef _OPENMP
	#pragma omp parallel for
#endif
	for( int i = 0 ; i < static_cast< int >( out.size( ) ) ; i++ )
	{
		out[ i ] = mask[ i ] == 2 ? fgval : bgval;
	}

	return( true );
}



/// @}
//  �G�b�W���o�t�B���^�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_EDGE__
