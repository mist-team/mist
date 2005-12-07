// 
// Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/filter/gaussian.h
//!
//! @brief �K�E�V�A���t�B���^�̂��߂̃��C�u����
//!



#ifndef __INCLUDE_FILTER_GUASSIAN_FILTER_H__
#define __INCLUDE_FILTER_GUASSIAN_FILTER_H__



#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <algorithm>
#include <cmath>
#include <mist/config/color.h>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __gaussian__
{
	inline void normalize( mist::array< double > &ary )
	{
		double norm = 0.0;
		for( size_t i = 0 ; i < ary.size( ) ; i ++ )
		{
			norm += ary[ i ];
		}
		for( size_t i = 0 ; i < ary.size( ) ; i ++ )
		{
			ary[ i ] /= norm;
		}
	}


	inline void gaussian_kernel( mist::array< double > &gauss, const double radius, double sigma )
	{
		if( sigma == 0.0 )
		{
			sigma = radius / 2.0;
		}
		int r = static_cast< int >( radius );
		r = ( radius - static_cast< double >( r ) ) > 0.0 ? r + 1 : r;
		r = ( r == 0 ) ? 1 : r;
		gauss.resize( static_cast< size_t >( r * 2 - 1 ) );
		const int o = static_cast< int >( gauss.size( ) ) / 2;
		for( size_t i = 0 ; i < gauss.size( ) ; i ++ )
		{
			const int di = static_cast< int >( i );
			gauss[ i ] = std::exp( -( ( di - o ) * ( di - o ) ) / ( 2.0 * sigma * sigma ) );
		}
		normalize( gauss );
	}


	inline void pointer_differences( mist::array< int > &p_diff, const int inc )
	{
		const int o = static_cast< int >( p_diff.size( ) ) / 2;
		for( size_t i = 0 ; i < p_diff.size( ) ; i ++ )
		{
			const int di = static_cast< int >( i );
			p_diff[ i ] = ( di - o ) * inc;
		}
	}


	template< bool Is_color >
	struct calc_type
	{
		typedef double type;
	};


	template< >
	struct calc_type< true >
	{
		typedef mist::rgb< double > type;
	};



	template< typename Calc_type >
	struct filter_type
	{
		template< typename In_type, typename Out_type >
		static void filtering( const mist::array< In_type > &in, mist::array< Out_type > &out, const size_t begin, const size_t end, const mist::array< double > &gauss, const mist::array< int > &p_diff )
		{
			Calc_type val;
			for( size_t i = begin ; i < end ; i ++ )
			{
				val = Calc_type( );
				int di = static_cast< int >( i );
				for( size_t m = 0 ; m < gauss.size( ) ; m ++ )
				{
					val += in[ static_cast< size_t >( di + p_diff[ m ] ) ] * gauss[ m ];
				}
				out[ i ] = static_cast< Out_type >( val );
			}
		}
	};	
}



//! @addtogroup gaussian_group gaussian�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/gaussian.h>
//! @endcode
//!
//! �K�E�V�A���̕ϐ��̓Ɨ����𗘗p���č����ȃt�B���^�������s���܂��D
//!
//! �z��̒[�i�J�[�l�����͂ݏo���Ƃ���j�̗v�f�̌v�Z�l�͕ۏႵ�܂���D
//! �g�p�\�ȗv�f�^�́C�Z�p�^�� mist::rgb< �Z�p�^ > �Ɍ���܂��D
//! �����̌v�Z�́C�o�͔z��̗v�f�^���Z�p�^�̏ꍇ�� double �^�C
//! mist::rgb< �Z�p�^ > �̏ꍇ�� mist::rgb< double > �ōs���܂��D
//! 
//!  @{


/// @brief �K�E�V�A��( array, array1, array2, array3 )
//! 
//! �͈͂Ƌ��x���w��\�ȃK�E�V�A��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  radius  �c �K�E�V�A���̔��a�i�f�t�H���g 2.0�j
//! @param[out] sigma  �c �K�E�V�A���̋��x�i���a����v�Z����邽�ߕ��ʂ͗^���Ȃ��Ă��ǂ��F�f�t�H���g 0.0�j
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void gaussian( 
					 const mist::array< In_value, In_alloc > &in, 
					 mist::array< Out_value, Out_alloc > &out,
					 const double radius = 2.0,
					 const double sigma = 0.0 )
{
	typedef typename __gaussian__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	
	array< double > gauss;
	__gaussian__::gaussian_kernel( gauss, radius, sigma );
	
	mist::array< int > p_diff( gauss.size( ) );
	
	out.resize( in.size( ) );
	size_t begin = gauss.size( ) / 2;
	size_t end = in.size( ) - begin;
	__gaussian__::pointer_differences( p_diff, 1 );
	__gaussian__::filter_type< calc_type >::filtering( in, out, begin, end, gauss, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void gaussian( 
					 const mist::array1< In_value, In_alloc > &in, 
					 mist::array1< Out_value, Out_alloc > &out,
					 const double radius = 2.0,
					 const double sigma = 0.0 )
{
	typedef typename __gaussian__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	
	out.reso1( in.reso1( ) );
	
	array< double > gauss;
	__gaussian__::gaussian_kernel( gauss, radius, sigma );
	
	mist::array< int > p_diff( gauss.size( ) );

	out.resize( in.width( ) );
	size_t begin = gauss.size( ) / 2;
	size_t end = in.size( ) - begin;
	__gaussian__::pointer_differences( p_diff, 1 );
	__gaussian__::filter_type< calc_type >::filtering( in, out, begin, end, gauss, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void gaussian( 
					 const mist::array2< In_value, In_alloc > &in, 
					 mist::array2< Out_value, Out_alloc > &out,
					 const double radius = 2.0,
					 const double sigma = 0.0 )
{
	typedef typename __gaussian__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;

	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	
	array< double > gauss;
	mist::__gaussian__::gaussian_kernel( gauss, radius, sigma );
	
	mist::array< int > p_diff( gauss.size( ) );

	mist::array< Out_value > work( in.size( ) );
	size_t begin = gauss.size( ) / 2;
	size_t end = in.size( ) - begin;
	__gaussian__::pointer_differences( p_diff, 1 );
	__gaussian__::filter_type< calc_type >::filtering( in, work, begin, end, gauss, p_diff );
	
	out.resize( in.width( ) , in.height( ) );
	begin = ( gauss.size( ) / 2 ) * ( in.width( ) + 1 );
	end = in.size( ) - begin;
	__gaussian__::pointer_differences( p_diff, static_cast< int >( in.width( ) ) );	
	__gaussian__::filter_type< calc_type >::filtering( work, out, begin, end, gauss, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void gaussian( 
					 const mist::array3< In_value, In_alloc > &in, 
					 mist::array3< Out_value, Out_alloc > &out,
					 const double radius = 2.0,
					 const double sigma = 0.0 )
{
	typedef typename __gaussian__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	
	array< double > gauss;
	__gaussian__::gaussian_kernel( gauss, radius, sigma );
	
	mist::array< int > p_diff( gauss.size( ) );

	mist::array< Out_value > work1( in.size( ) );
	size_t begin = gauss.size( ) / 2;
	size_t end = in.size( ) - begin;
	__gaussian__::pointer_differences( p_diff, 1 );
	__gaussian__::filter_type< calc_type >::filtering( in, work1, begin, end, gauss, p_diff );
	
	mist::array< Out_value > work2( in.size( ) );
	begin = ( gauss.size( ) / 2 ) * ( in.width( ) + 1 );
	end = in.size( ) - begin;
	__gaussian__::pointer_differences( p_diff, static_cast< int >( in.width( ) ) );	
	__gaussian__::filter_type< calc_type >::filtering( work1, work2, begin, end, gauss, p_diff );
	work1.clear( );

	out.resize( in.width( ), in.height( ), in.depth( ) );
	begin = ( gauss.size( ) / 2 ) * ( in.height( ) * in.width( ) + in.width( ) + 1 );
	end = in.size( ) - begin;
	__gaussian__::pointer_differences( p_diff, static_cast< int >( in.width( ) * in.height( ) ) );	
	__gaussian__::filter_type< calc_type >::filtering( work2, out, begin, end, gauss, p_diff );
}



/// @}
//  ���`�O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_FILTER_GAUSSIAN_H__
