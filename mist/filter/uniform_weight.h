// 
// Copyright (c) 2003-2006, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/filter/uniform_weight.h
//!
//! @brief 一様重み平滑化フィルタのためのライブラリ
//!



#ifndef __INCLUDE_FILTER_UNIFORM_WEIGHT_FILTER_H__
#define __INCLUDE_FILTER_UNIFORM_WEIGHT_FILTER_H__



#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <algorithm>
#include <cmath>
#include <mist/config/color.h>



// mist名前空間の始まり
_MIST_BEGIN



namespace __uniform_weight__
{

	inline void uniform_weight_kernel( mist::array< double > &uni_w, const size_t size )
	{
		uni_w.resize( size );
		uni_w.fill( 1.0 / uni_w.size( ) );
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
		static void filtering( const mist::array< In_type > &in, mist::array< Out_type > &out, const size_t begin, const size_t end, const mist::array< double > &uni_w, const mist::array< int > &p_diff )
		{
			Calc_type val;
			for( size_t i = begin ; i < end ; i ++ )
			{
				val = Calc_type( );
				int di = static_cast< int >( i );
				for( size_t m = 0 ; m < uni_w.size( ) ; m ++ )
				{
					val += in[ static_cast< size_t >( di + p_diff[ m ] ) ] * uni_w[ m ];
				}
				out[ i ] = static_cast< Out_type >( val );
			}
		}
	};	
}



//! @addtogroup uniform_weight_group 一様重み平滑化フィルタ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/uniform_weight.h>
//! @endcode
//!
//! 配列の端（カーネルがはみ出すところ）の要素の計算値は保障しません．
//! 使用可能な要素型は，算術型か mist::rgb< 算術型 > に限ります．
//! 内部の計算は，出力配列の要素型が算術型の場合は double 型，
//! mist::rgb< 算術型 > の場合は mist::rgb< double > で行います．
//! 
//!  @{


/// @brief 一様重み平滑化( array, array1, array2, array3 )
//! 
//! サイズを指定可能な一様重み平滑化
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in] size  … フィルタの一辺のサイズ（デフォルト 3）
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
						   const mist::array< In_value, In_alloc > &in, 
						   mist::array< Out_value, Out_alloc > &out,
						   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	
	array< double > uni_w;
	__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );
	
	out.resize( in.size( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, out, begin, end, uni_w, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
						   const mist::array1< In_value, In_alloc > &in, 
						   mist::array1< Out_value, Out_alloc > &out,
						   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	
	out.reso1( in.reso1( ) );
	
	array< double > uni_w;
	__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );

	out.resize( in.width( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, out, begin, end, uni_w, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
						   const mist::array2< In_value, In_alloc > &in, 
						   mist::array2< Out_value, Out_alloc > &out,
						   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;

	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	
	array< double > uni_w;
	mist::__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );

	mist::array< Out_value > work( in.size( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, work, begin, end, uni_w, p_diff );
	
	out.resize( in.width( ) , in.height( ) );
	begin = ( uni_w.size( ) / 2 ) * ( in.width( ) + 1 );
	end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, static_cast< int >( in.width( ) ) );	
	__uniform_weight__::filter_type< calc_type >::filtering( work, out, begin, end, uni_w, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
						   const mist::array3< In_value, In_alloc > &in, 
						   mist::array3< Out_value, Out_alloc > &out,
						   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	
	array< double > uni_w;
	__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );

	mist::array< Out_value > work1( in.size( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, work1, begin, end, uni_w, p_diff );
	
	mist::array< Out_value > work2( in.size( ) );
	begin = ( uni_w.size( ) / 2 ) * ( in.width( ) + 1 );
	end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, static_cast< int >( in.width( ) ) );	
	__uniform_weight__::filter_type< calc_type >::filtering( work1, work2, begin, end, uni_w, p_diff );
	work1.clear( );

	out.resize( in.width( ), in.height( ), in.depth( ) );
	begin = ( uni_w.size( ) / 2 ) * ( in.height( ) * in.width( ) + in.width( ) + 1 );
	end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, static_cast< int >( in.width( ) * in.height( ) ) );	
	__uniform_weight__::filter_type< calc_type >::filtering( work2, out, begin, end, uni_w, p_diff );
}



/// @}
//   一様重み平滑化グループの終わり



// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_FILTER_UNIFORM_WEIGHT_H__
