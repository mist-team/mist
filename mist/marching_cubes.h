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

/// @file mist/marching_cubes.h
//!
//! @brief Marching Cubes のためのライブラリ
//!

#ifndef __INCLUDE_MARCHING_CUBES_H__
#define __INCLUDE_MARCHING_CUBES_H__


#include <iostream>
#include <cmath>
#include <vector>

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_FACET__
#include "facet.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_INTERVAL_TREE_H__
#include "interval_tree.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN

namespace __mc__
{
	/// @brief cubeの頂点ノードを扱うためのクラス（ marching_cubes クラスの内部でのみ利用 ）
	//!
	//! @param V … 頂点の値の型
	//! @param P … 頂点の座標の型
	//! @param N … 頂点の法線ベクトルの型
	//!
	template< class V, class P >
	struct node
	{
		typedef V	value_type;
		typedef P	point_type;

		value_type	v;
		point_type	p;
		point_type	n;

		node( ) { }
		node( const value_type &vv, const point_type &pp, const point_type &nn ) : v( vv ), p( pp ), n( nn ) { }
	};

	/// @brief 等値面生成結果から3角形パッチの数を計算
	//!
	//! @param[in]  sv   … サイズ集合（等値面生成結果の1つ）
	//! @return				3角形パッチ数
	//!
	template< typename T >
	inline size_t number_of_triangles( const std::vector< T > &sv )
	{
		size_t ret = 0;
		for( size_t i = 0 ; i < sv.size( ) ; i ++ )
		{
			ret += ( sv[ i ] - 2 ); 
		}
		return ret;
	}
}


#define __MCFUNC__( name )								\
	static size_t name( const node_type nda[ 8 ],		\
						vector_type pv[ 12 ],			\
						vector_type nv[ 12 ],			\
						size_type   sv[  4 ],			\
						float_type th,					\
						const vector_type &o,			\
						const vector_type &s )			\

#define __MCFUNC_DEF__( name )											\
	template< typename V, typename P >									\
	size_t marching_cubes< V, P >::name( const node_type nda[ 8 ],		\
										 vector_type pv[ 12 ],			\
										 vector_type nv[ 12 ],			\
										 size_type   sv[  4 ],			\
										 float_type th,					\
										 const vector_type &o,			\
										 const vector_type &s )			\

#define __MCFUNC0__( name )												\
	__MCFUNC_DEF__( name )												\
	{																	\
		return( 0 );													\
	}																	\

#define __MCFUNC3__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6 )						\
	__MCFUNC_DEF__( name )																					\
	{																										\
		interpolation_about_##AXIS1( nda[ NDA1 ], nda[ NDA2 ], pv[ 0 ], nv[ 0 ], th, o, s );				\
		interpolation_about_##AXIS2( nda[ NDA3 ], nda[ NDA4 ], pv[ 1 ], nv[ 1 ], th, o, s );				\
		interpolation_about_##AXIS3( nda[ NDA5 ], nda[ NDA6 ], pv[ 2 ], nv[ 2 ], th, o, s );				\
		sv[ 0 ] = 3;																						\
		return( 1 );																						\
	}																										\

#define __MCFUNC4__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8 )		\
	__MCFUNC_DEF__( name )																					\
	{																										\
		interpolation_about_##AXIS1( nda[ NDA1 ], nda[ NDA2 ], pv[ 0 ], nv[ 0 ], th, o, s );				\
		interpolation_about_##AXIS2( nda[ NDA3 ], nda[ NDA4 ], pv[ 1 ], nv[ 1 ], th, o, s );				\
		interpolation_about_##AXIS3( nda[ NDA5 ], nda[ NDA6 ], pv[ 2 ], nv[ 2 ], th, o, s );				\
		interpolation_about_##AXIS4( nda[ NDA7 ], nda[ NDA8 ], pv[ 3 ], nv[ 3 ], th, o, s );				\
		sv[ 0 ] = 4;																						\
		return( 1 );																						\
	}																										\

#define __MCFUNC5__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10 )		\
	__MCFUNC_DEF__( name )																										\
	{																															\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );									\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );									\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );									\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );									\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );									\
		sv[ 0 ] = 5;																											\
		return( 1 );																											\
	}																															\

#define __MCFUNC6_1__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12 )	\
	__MCFUNC_DEF__( name )																															\
	{																																				\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );														\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );														\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );														\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );														\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );														\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );														\
		sv[ 0 ] = 3;																																\
		sv[ 1 ] = 3;																																\
		return( 2 );																																\
	}																																				\

#define __MCFUNC6_2__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12 )	\
	__MCFUNC_DEF__( name )																															\
	{																																				\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );														\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );														\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );														\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );														\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );														\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );														\
		sv[ 0 ] = 6;																																\
		return( 1 );																																\
	}																																				\

#define __MCFUNC7_1__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12, AXIS7, NDA13, NDA14 )	\
	__MCFUNC_DEF__( name )																																				\
	{																																									\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );																			\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );																			\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );																			\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );																			\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );																			\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );																			\
		interpolation_about_##AXIS7( nda[ NDA13 ], nda[ NDA14 ], pv[ 6 ], nv[ 6 ], th, o, s );																			\
		sv[ 0 ] = 4;																																					\
		sv[ 1 ] = 3;																																					\
		return( 2 );																																					\
	}																																									\

#define __MCFUNC7_2__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12, AXIS7, NDA13, NDA14 )	\
	__MCFUNC_DEF__( name )																																				\
	{																																									\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );																			\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );																			\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );																			\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );																			\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );																			\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );																			\
		interpolation_about_##AXIS7( nda[ NDA13 ], nda[ NDA14 ], pv[ 6 ], nv[ 6 ], th, o, s );																			\
		sv[ 0 ] = 7;																																					\
		return( 1 );																																					\
	}																																									\

#define __MCFUNC8_1__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12, AXIS7, NDA13, NDA14, AXIS8, NDA15, NDA16 )	\
	__MCFUNC_DEF__( name )																																				\
	{																																									\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );																			\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );																			\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );																			\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );																			\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );																			\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );																			\
		interpolation_about_##AXIS7( nda[ NDA13 ], nda[ NDA14 ], pv[ 6 ], nv[ 6 ], th, o, s );																			\
		interpolation_about_##AXIS8( nda[ NDA15 ], nda[ NDA16 ], pv[ 7 ], nv[ 7 ], th, o, s );																			\
		sv[ 0 ] = 5;																																					\
		sv[ 1 ] = 3;																																					\
		return( 2 );																																					\
	}																																									\

#define __MCFUNC8_2__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12, AXIS7, NDA13, NDA14, AXIS8, NDA15, NDA16 )	\
	__MCFUNC_DEF__( name )																																				\
	{																																									\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );																			\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );																			\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );																			\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );																			\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );																			\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );																			\
		interpolation_about_##AXIS7( nda[ NDA13 ], nda[ NDA14 ], pv[ 6 ], nv[ 6 ], th, o, s );																			\
		interpolation_about_##AXIS8( nda[ NDA15 ], nda[ NDA16 ], pv[ 7 ], nv[ 7 ], th, o, s );																			\
		sv[ 0 ] = 4;																																					\
		sv[ 1 ] = 4;																																					\
		return( 2 );																																					\
	}																																									\

#define __MCFUNC9_1__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12, AXIS7, NDA13, NDA14, AXIS8, NDA15, NDA16, AXIS9, NDA17, NDA18 )	\
	__MCFUNC_DEF__( name )																																				\
	{																																									\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );																			\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );																			\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );																			\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );																			\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );																			\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );																			\
		interpolation_about_##AXIS7( nda[ NDA13 ], nda[ NDA14 ], pv[ 6 ], nv[ 6 ], th, o, s );																			\
		interpolation_about_##AXIS8( nda[ NDA15 ], nda[ NDA16 ], pv[ 7 ], nv[ 7 ], th, o, s );																			\
		interpolation_about_##AXIS9( nda[ NDA17 ], nda[ NDA18 ], pv[ 8 ], nv[ 8 ], th, o, s );																			\
		sv[ 0 ] = 3;																																					\
		sv[ 1 ] = 3;																																					\
		sv[ 2 ] = 3;																																					\
		return( 3 );																																					\
	}																																									\

#define __MCFUNC9_2__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12, AXIS7, NDA13, NDA14, AXIS8, NDA15, NDA16, AXIS9, NDA17, NDA18 )	\
	__MCFUNC_DEF__( name )																																				\
	{																																									\
		interpolation_about_##AXIS1( nda[ NDA1  ], nda[ NDA2  ], pv[ 0 ], nv[ 0 ], th, o, s );																			\
		interpolation_about_##AXIS2( nda[ NDA3  ], nda[ NDA4  ], pv[ 1 ], nv[ 1 ], th, o, s );																			\
		interpolation_about_##AXIS3( nda[ NDA5  ], nda[ NDA6  ], pv[ 2 ], nv[ 2 ], th, o, s );																			\
		interpolation_about_##AXIS4( nda[ NDA7  ], nda[ NDA8  ], pv[ 3 ], nv[ 3 ], th, o, s );																			\
		interpolation_about_##AXIS5( nda[ NDA9  ], nda[ NDA10 ], pv[ 4 ], nv[ 4 ], th, o, s );																			\
		interpolation_about_##AXIS6( nda[ NDA11 ], nda[ NDA12 ], pv[ 5 ], nv[ 5 ], th, o, s );																			\
		interpolation_about_##AXIS7( nda[ NDA13 ], nda[ NDA14 ], pv[ 6 ], nv[ 6 ], th, o, s );																			\
		interpolation_about_##AXIS8( nda[ NDA15 ], nda[ NDA16 ], pv[ 7 ], nv[ 7 ], th, o, s );																			\
		interpolation_about_##AXIS9( nda[ NDA17 ], nda[ NDA18 ], pv[ 8 ], nv[ 8 ], th, o, s );																			\
		sv[ 0 ] = 3;																																					\
		sv[ 1 ] = 6;																																					\
		return( 2 );																																					\
	}																																									\

#define __MCFUNC12__( name, AXIS1, NDA1, NDA2, AXIS2, NDA3, NDA4, AXIS3, NDA5, NDA6, AXIS4, NDA7, NDA8, AXIS5, NDA9, NDA10, AXIS6, NDA11, NDA12, AXIS7, NDA13, NDA14, AXIS8, NDA15, NDA16, AXIS9, NDA17, NDA18, AXIS10, NDA19, NDA20, AXIS11, NDA21, NDA22, AXIS12, NDA23, NDA24 )	\
	__MCFUNC_DEF__( name )																																				\
	{																																									\
		interpolation_about_##AXIS1 ( nda[ NDA1  ], nda[ NDA2  ], pv[  0 ], nv[  0 ], th, o, s );																		\
		interpolation_about_##AXIS2 ( nda[ NDA3  ], nda[ NDA4  ], pv[  1 ], nv[  1 ], th, o, s );																		\
		interpolation_about_##AXIS3 ( nda[ NDA5  ], nda[ NDA6  ], pv[  2 ], nv[  2 ], th, o, s );																		\
		interpolation_about_##AXIS4 ( nda[ NDA7  ], nda[ NDA8  ], pv[  3 ], nv[  3 ], th, o, s );																		\
		interpolation_about_##AXIS5 ( nda[ NDA9  ], nda[ NDA10 ], pv[  4 ], nv[  4 ], th, o, s );																		\
		interpolation_about_##AXIS6 ( nda[ NDA11 ], nda[ NDA12 ], pv[  5 ], nv[  5 ], th, o, s );																		\
		interpolation_about_##AXIS7 ( nda[ NDA13 ], nda[ NDA14 ], pv[  6 ], nv[  6 ], th, o, s );																		\
		interpolation_about_##AXIS8 ( nda[ NDA15 ], nda[ NDA16 ], pv[  7 ], nv[  7 ], th, o, s );																		\
		interpolation_about_##AXIS9 ( nda[ NDA17 ], nda[ NDA18 ], pv[  8 ], nv[  8 ], th, o, s );																		\
		interpolation_about_##AXIS10( nda[ NDA19 ], nda[ NDA20 ], pv[  9 ], nv[  9 ], th, o, s );																		\
		interpolation_about_##AXIS11( nda[ NDA21 ], nda[ NDA22 ], pv[ 10 ], nv[ 10 ], th, o, s );																		\
		interpolation_about_##AXIS12( nda[ NDA23 ], nda[ NDA24 ], pv[ 11 ], nv[ 11 ], th, o, s );																		\
		sv[ 0 ] = 3;																																					\
		sv[ 1 ] = 3;																																					\
		sv[ 2 ] = 3;																																					\
		sv[ 3 ] = 3;																																					\
		return( 4 );																																					\
	}																																									\




//! @addtogroup marching_cubes_group Marching Cubes
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/marching_cubes.h>
//! @endcode
//!
//!  @{

/// @brief ボリュームデータから等値面を生成するためのクラス
//!
//! 区間の集合から2分木を構築し、通常O(n)かかる探索時間をO(log_2(n))に削減する．
//! 線分や面分の重なり判定などに有効．
//!
//! @attention mist::arra3< 算術型 > 型を入力とする．
//! @attention ポリゴンの各頂点座標（ mist::vector3< 算術型 >型 ），各頂点の法線ベクトル（ mist::vector3< 算術型 > 型 ），各ポリゴンの頂点数（ size_t 型 ）の3つのstd::vectorを等値面生成結果の出力とする．
//! @attention ポリゴンの各頂点座標と各頂点の法線ベクトルは OpenGL の頂点配列として利用可能（ GL_POLYGON や GL_TRIANGLE_FAN 等に使える）．
//! 
//! @param ARRAY3 … ボリュームデータの型
//! @param P      … 等値面ポリゴンの頂点座標の要素の型
//!
//! @code 使用例
//! // ボリュームデータの用意
//! mist::array3< int > va;
//! /* 
//!	 * ボリュームデータの作成処理
//!  */
//! 
//! // 等値面生成結果を格納するためのオブジェクトの準備
//! std::vector< mist::vector3< double > >	pv;		// double 型の要素を持つ頂点座標の集合
//! std::vector< mist::vector3< double > >	nv;		// double 型の要素を持つ法線ベクトルの集合
//! std::vector< size_t >					sv;		// 各ポリゴンのサイズ（頂点数）の集合
//!
//! // ボリュームデータの要素が int 型で，
//! // 得られる等値面ポリゴンの頂点座標の要素と法線ベクトルが double 型の
//! // marching_cubes オブジェクトを作成
//! mist::marching_cubes< mist::array3< int >, double >	mcs;
//!
//! // パラメータ設定（ 頂点座標のオフセット値，拡大率，等値面生成のための閾値 ）
//! mcs.offset( 0.0, 0.0, 0.0 );
//! mcs.scale( 1.0, 1.0, 1.0 );
//! mcs.threshold( 0.0 );
//! 
//! // 上記のパラメータで va から等値面ポリゴンを生成し，結果を pv, nv, sv に格納
//! mcs.isosurfacing( va, pv, nv, sv );
//! @endcode
//!
template< typename ARRAY3, typename P = double >
class marching_cubes
{
public:
	typedef ARRAY3				image_type;						///< @brief ボリュームデータの要素の型
	typedef typename image_type::value_type value_type;			///< @brief ボリュームデータの要素の型
	typedef vector3< short >	ivector_type;					///< @brief cube の頂点座標の型
	typedef vector3< P >		vector_type;					///< @brief ポリゴンの頂点座標の型
	typedef typename vector_type::value_type float_type;		///< @brief ポリゴンの頂点の法汚染ベクトルの型
	typedef size_t				size_type;						///< @brief 各ポリゴンのサイズ（頂点数）の型
	typedef ptrdiff_t			difference_type;				///< @brief 各ポリゴンのサイズ（頂点数）の型

	typedef std::vector< vector_type > vector_list_type;		///< @brief 3次元ベクトルリストを扱う型
	typedef __mc__::node< value_type, ivector_type > node_type;	///< @brief cube の頂点情報を保持する型
	typedef size_t ( * func_type )( const node_type *, vector_type *, vector_type *,
							size_type *, float_type, const vector_type &, const vector_type & );	///< @brief cube 単位での等値面生成関数の型
	
	typedef tagged_section< value_type, size_type >	section_type;	///< @brief Interval-tree 用いる区間の型
	typedef typename section_type::min_max_type		min_max_type;	///< @brief 区間の最大値，最小値の型
	typedef typename section_type::tag_type			tag_type;		///< @brief 区間のタグ情報の型

private:
	func_type									fa_[ 256 ];
	vector_type									__p__[ 12 ];
	vector_type									__n__[ 12 ];
	size_type									__s__[ 4 ];
	bool										is_preprocessed_;
	size_type									pda_[ 8 ];
	array3< ivector_type >						pa_;
	array3< ivector_type >						na_;
	interval_tree< section_type, float_type >	it_;
	std::vector< tag_type >						active_cube_tags_;

	float_type		th_;
	vector_type		o_;
	vector_type		s_;

public:	// コンストラクタとプロパティ関連
	/// @brief コンストラクタ
	marching_cubes( ) : is_preprocessed_( false ), th_( 0 ), o_( 0, 0, 0 ), s_( 1, 1, 1 )
	{
		init_function_array( ); 
	}

	/// @brief デストラクタ
	~marching_cubes( )
	{
	}

	/// @brief 前処理されたデータの有無を返す
	//!
	//! @return    … 前処理が終了していれば真を返す
	//!
	bool is_preprocessed( ) const { return is_preprocessed_; }

	/// @brief しきい値パラメータを設定
	//!
	//! @param[in]  th   … 閾値
	//!
	void threshold( float_type th ) { th_ = th; }

	/// @brief しきい値パラメータを取得設定
	//!
	//! @return しきい値を取得する
	//!
	float_type threshold( ) const { return( th_ ); }
	
	/// @brief オフセットパラメータを設定
	//!
	//! @param[in]  ox   … x 方向オフセット
	//! @param[in]  oy   … y 方向オフセット
	//! @param[in]  oz   … z 方向オフセット
	//!
	void offset( float_type ox, float_type oy, float_type oz ) { o_.x = ox; o_.y = oy; o_.z = oz; }

	/// @brief オフセットパラメータを取得設定
	//!
	//! @return オフセット値を取得する
	//!
	vector_type offset( ) const { return( o_ ); }

	/// @brief スケールパラメータを設定
	//!
	//! @param[in]  sx   … x 方向スケール
	//! @param[in]  sy   … y 方向スケール
	//! @param[in]  sz   … z 方向スケール
	//!
	void scale( float_type sx, float_type sy, float_type sz ) { s_.x = sx; s_.y = sy; s_.z = sz; }

	/// @brief スケールパラメータを取得設定
	//!
	//! @return スケール値を取得する
	//!
	vector_type scale( ) const { return( s_ ); }


public:	// 操作関数

	/// @brief 等値面生成（前処理の有無で等値面生成処理を分ける）
	//!
	//! @param[in]  va   … ボリュームデータ
	//! @param[out] pv   … 等値面ポリゴンの頂点座標の集合
	//! @param[out] nv   … 等値面ポリゴンの頂点の法線ベクトルの集合
	//! @param[out] sv   … 等値面ポリゴンのサイズの集合
	//!
	void isosurfacing( const image_type &va, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv )
	{
		if( is_preprocessed_ )
		{
			isosurfacing_with_preprocess( va, pv, nv, sv );
		}
		else
		{
			isosurfacing_without_preprocess( va, pv, nv, sv );
		}
	}

	/// @brief 等値面生成（前処理の有無で等値面生成処理を分ける）
	//!
	//! @param[in]  va     … ボリュームデータ
	//! @param[out] facets … 等値面ポリゴンの集合
	//!
	template < class T >
	void isosurfacing( const image_type &va, facet_list< T > &facets )
	{
		if( is_preprocessed_ )
		{
			isosurfacing_with_preprocess( va, facets );
		}
		else
		{
			isosurfacing_without_preprocess( va, facets );
		}
	}

	/// @brief 等値面生成（前処理有り）
	//!
	//! @param[in]  va   … ボリュームデータ
	//! @param[out] pv   … 等値面ポリゴンの頂点座標の集合
	//! @param[out] nv   … 等値面ポリゴンの頂点の法線ベクトルの集合
	//! @param[out] sv   … 等値面ポリゴンのサイズの集合
	//!
	void isosurfacing_with_preprocess( const image_type &va, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv )
	{
		construct_pointer_difference_array( va );
		pv.resize( 0 );
		nv.resize( 0 );
		sv.resize( 0 );
		it_.find( th_, active_cube_tags_ );
		std::sort( active_cube_tags_.begin( ), active_cube_tags_.end( ) );

		node_type nda[ 8 ];
		const size_type begin = 0;
		const size_type end = active_cube_tags_.size( );
		for( size_type i = begin ; i < end ; i ++ )
		{
			size_type ptn = construct_cube( va, active_cube_tags_[ i ], nda );
			if( 0 < ptn )
			{
				isosurfacing_in_cube( nda, pv, nv, sv, ptn );
			}
		}
	}

	/// @brief 等値面生成（前処理無し）
	//!
	//! @param[in]  va   … ボリュームデータ
	//! @param[out] pv   … 等値面ポリゴンの頂点座標の集合
	//! @param[out] nv   … 等値面ポリゴンの頂点の法線ベクトルの集合
	//! @param[out] sv   … 等値面ポリゴンのサイズの集合
	//!
	void isosurfacing_without_preprocess( const image_type &va, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv )
	{
		construct_pointer_difference_array( va );
		pv.resize( 0 );
		nv.resize( 0 );
		sv.resize( 0 );

		node_type nda[ 8 ];
		for( size_t k = 0 ; k < va.depth( ) - 1 ; k ++ )
		{
			for( size_t j = 0 ; j < va.height( ) - 1 ; j ++ )
			{
				for( size_t i = 0 ; i < va.width( ) - 1 ; i ++ )
				{
					size_type ptn = construct_cube_without_preprocessing( va, i, j, k, nda );
					if( 0 < ptn )
					{
						isosurfacing_in_cube( nda, pv, nv, sv, ptn );
					}
				}
			}
		}
	}


	/// @brief 等値面生成（前処理有り）
	//!
	//! @param[in]  va     … ボリュームデータ
	//! @param[out] facets … 等値面ポリゴンの集合
	//!
	template < class T >
	void isosurfacing_with_preprocess( const image_type &va, facet_list< T > &facets )
	{
		facets.resize( 0 );
		construct_pointer_difference_array( va );

		it_.find( th_, active_cube_tags_ );
		std::sort( active_cube_tags_.begin( ), active_cube_tags_.end( ) );

		node_type nda[ 8 ];
		const size_type begin = 0;
		const size_type end = active_cube_tags_.size( );
		for( size_type i = begin ; i < end ; i ++ )
		{
			size_type ptn = construct_cube( va, active_cube_tags_[ i ], nda );
			if( 0 < ptn )
			{
				isosurfacing_in_cube( nda, facets, ptn );
			}
		}
	}

	/// @brief 等値面生成（前処理無し）
	//!
	//! @param[in]  va     … ボリュームデータ
	//! @param[out] facets … 等値面ポリゴンの集合
	//!
	template < class T >
	void isosurfacing_without_preprocess( const image_type &va, facet_list< T > &facets )
	{
		facets.resize( 0 );
		construct_pointer_difference_array( va );

		node_type nda[ 8 ];

		for( size_type k = 0 ; k < va.depth( ) - 1 ; k++ )
		{
			for( size_type j = 0 ; j < va.height( ) - 1 ; j ++ )
			{
				for( size_type i = 0 ; i < va.width( ) - 1 ; i ++ )
				{
					size_type ptn = construct_cube_without_preprocessing( va, i, j, k, nda );
					if( 0 < ptn )
					{
						isosurfacing_in_cube( nda, facets, ptn );
					}
				}
			}
		}
	}

	/// @brief 前処理（Interval-tree の構築や各格子点の法線ベクトルの計算など）
	//!
	//! @param[in]  va   … 等値面生成処理の対象としているボリュームデータ
	//!
	bool preprocess( const image_type &va )
	{
		if( is_preprocessed_ )
		{
			de_preprocess( );
		}

		construct_pointer_difference_array( va );
		is_preprocessed_ = construct_point_array( va ) && construct_normal_array( va ) && construct_interval_tree( va );

		if( !is_preprocessed_ )
		{
			std::cout << "can't construct preprocessing data for lack of memory, use non-preprocessing version of marching cubes." << std::endl;
			de_preprocess( );
		}

		return( is_preprocessed_ );
	}

	/// @brief 前処理で構築されたデータを破棄
	void de_preprocess( )
	{
		destruct_point_array( );
		destruct_normal_array( );
		destruct_interval_tree( );
		is_preprocessed_ = false;
	}


private:
	size_type construct_cube( const image_type &va, const size_t i, node_type nda[ 8 ] ) const
	{
		nda[ 0 ].v = va[ i + pda_[ 0 ] ];
		nda[ 1 ].v = va[ i + pda_[ 1 ] ];
		nda[ 2 ].v = va[ i + pda_[ 2 ] ];
		nda[ 3 ].v = va[ i + pda_[ 3 ] ];
		nda[ 4 ].v = va[ i + pda_[ 4 ] ];
		nda[ 5 ].v = va[ i + pda_[ 5 ] ];
		nda[ 6 ].v = va[ i + pda_[ 6 ] ];
		nda[ 7 ].v = va[ i + pda_[ 7 ] ];

		size_type ptn = pattern( nda );

		if( 0 < ptn && ptn < 255 )
		{
			nda[ 0 ].p = pa_[ i + pda_[ 0 ] ];
			nda[ 1 ].p = pa_[ i + pda_[ 1 ] ];
			nda[ 2 ].p = pa_[ i + pda_[ 2 ] ];
			nda[ 3 ].p = pa_[ i + pda_[ 3 ] ];
			nda[ 4 ].p = pa_[ i + pda_[ 4 ] ];
			nda[ 5 ].p = pa_[ i + pda_[ 5 ] ];
			nda[ 6 ].p = pa_[ i + pda_[ 6 ] ];
			nda[ 7 ].p = pa_[ i + pda_[ 7 ] ];

			nda[ 0 ].n = na_[ i + pda_[ 0 ] ];
			nda[ 1 ].n = na_[ i + pda_[ 1 ] ];
			nda[ 2 ].n = na_[ i + pda_[ 2 ] ];
			nda[ 3 ].n = na_[ i + pda_[ 3 ] ];
			nda[ 4 ].n = na_[ i + pda_[ 4 ] ];
			nda[ 5 ].n = na_[ i + pda_[ 5 ] ];
			nda[ 6 ].n = na_[ i + pda_[ 6 ] ];
			nda[ 7 ].n = na_[ i + pda_[ 7 ] ];

			return( ptn );
		}
		else
		{
			return( 0 );
		}
	}

	size_type construct_cube_without_preprocessing( const image_type &va, const size_t i, const size_t j, const size_t k, node_type nda[ 8 ] ) const
	{
		typedef typename image_type::const_pointer const_pointer;
		const_pointer p0 = &va( i, j, k );
		const_pointer p1 = p0 + pda_[ 1 ];
		const_pointer p2 = p0 + pda_[ 2 ];
		const_pointer p3 = p0 + pda_[ 3 ];
		const_pointer p4 = p0 + pda_[ 4 ];
		const_pointer p5 = p0 + pda_[ 5 ];
		const_pointer p6 = p0 + pda_[ 6 ];
		const_pointer p7 = p0 + pda_[ 7 ];

		nda[ 0 ].v = p0[ 0 ];
		nda[ 1 ].v = p1[ 0 ];
		nda[ 2 ].v = p2[ 0 ];
		nda[ 3 ].v = p3[ 0 ];
		nda[ 4 ].v = p4[ 0 ];
		nda[ 5 ].v = p5[ 0 ];
		nda[ 6 ].v = p6[ 0 ];
		nda[ 7 ].v = p7[ 0 ];

		size_type ptn = pattern( nda );

		if( 0 < ptn && ptn < 255 )
		{
			nda[ 0 ].p = _point( i,     j,     k     );
			nda[ 1 ].p = _point( i + 1, j,     k     );
			nda[ 2 ].p = _point( i,     j + 1, k     );
			nda[ 3 ].p = _point( i + 1, j + 1, k     );
			nda[ 4 ].p = _point( i,     j,     k + 1 );
			nda[ 5 ].p = _point( i + 1, j,     k + 1 );
			nda[ 6 ].p = _point( i,     j + 1, k + 1 );
			nda[ 7 ].p = _point( i + 1, j + 1, k + 1 );

			nda[ 0 ].n = __normal( va, i,     j,     k     );
			nda[ 1 ].n = __normal( va, i + 1, j,     k     );
			nda[ 2 ].n = __normal( va, i,     j + 1, k     );
			nda[ 3 ].n = __normal( va, i + 1, j + 1, k     );
			nda[ 4 ].n = __normal( va, i,     j,     k + 1 );
			nda[ 5 ].n = __normal( va, i + 1, j,     k + 1 );
			nda[ 6 ].n = __normal( va, i,     j + 1, k + 1 );
			nda[ 7 ].n = __normal( va, i + 1, j + 1, k + 1 );

			return( ptn );
		}
		else
		{
			return( 0 );
		}
	}

	void isosurfacing_in_cube( const node_type nda[ 8 ], vector_list_type &pv, vector_list_type &nv,std::vector< size_type > &sv, size_type ptn )
	{
		size_type num = fa_[ ptn ]( nda, __p__, __n__, __s__, th_, o_, s_ );

		vector_type *PP = __p__;
		vector_type *NN = __n__;
		for( size_type i = 0 ; i < num ; i++ )
		{
			size_type s = __s__[ i ];
			for( size_type j = 0 ; j < s ; j++ )
			{
				pv.push_back( *PP++ );
				nv.push_back( *NN++ );
			}
			sv.push_back( s );
		}
	}

	template < class T >
	void add_facet( facet_list< T > &facets, const vector_type &p1, const vector_type &p2, const vector_type &p3, const vector_type &n1, const vector_type &n2, const vector_type &n3 )
	{
		vector_type N = n1 + n2 + n3;

		if( N.length( ) > 0.0 )
		{
			N = N.unit( );
			// 点の並びが反時計回りになるようにする．
			if( N.inner( ( p2 - p1 ).outer( p3 - p1 ) ) < 0 )
			{
				facets.push_back( typename facet_list< T >::facet_type( N, p1, p3, p2 ) );
			}
			else
			{
				facets.push_back( typename facet_list< T >::facet_type( N, p1, p2, p3 ) );
			}
		}
		else
		{
			facets.push_back( typename facet_list< T >::facet_type( p1, p2, p3 ) );
		}
	}

	template < class T >
	void isosurfacing_in_cube( const node_type nda[ 8 ], facet_list< T > &facets, size_type ptn )
	{
		size_type num = fa_[ ptn ]( nda, __p__, __n__, __s__, th_, o_, s_ );

		vector_type *P = __p__;
		vector_type *N = __n__;
		for( size_type i = 0 ; i < num ; i++ )
		{
			switch( __s__[ i ] )
			{
			case 3:
				add_facet( facets, P[ 0 ], P[ 1 ], P[ 2 ], N[ 0 ], N[ 1 ], N[ 2 ] );
				break;

			case 4:
				add_facet( facets, P[ 0 ], P[ 1 ], P[ 2 ], N[ 0 ], N[ 1 ], N[ 2 ] );
				add_facet( facets, P[ 0 ], P[ 2 ], P[ 3 ], N[ 0 ], N[ 2 ], N[ 3 ] );
				break;

			case 5:
				add_facet( facets, P[ 0 ], P[ 1 ], P[ 2 ], N[ 0 ], N[ 1 ], N[ 2 ] );
				add_facet( facets, P[ 0 ], P[ 2 ], P[ 3 ], N[ 0 ], N[ 2 ], N[ 3 ] );
				add_facet( facets, P[ 0 ], P[ 3 ], P[ 4 ], N[ 0 ], N[ 3 ], N[ 4 ] );
				break;

			case 6:
				add_facet( facets, P[ 0 ], P[ 1 ], P[ 2 ], N[ 0 ], N[ 1 ], N[ 2 ] );
				add_facet( facets, P[ 0 ], P[ 2 ], P[ 3 ], N[ 0 ], N[ 2 ], N[ 3 ] );
				add_facet( facets, P[ 0 ], P[ 3 ], P[ 4 ], N[ 0 ], N[ 3 ], N[ 4 ] );
				add_facet( facets, P[ 0 ], P[ 4 ], P[ 5 ], N[ 0 ], N[ 4 ], N[ 5 ] );
				break;

			case 7:
				add_facet( facets, P[ 0 ], P[ 1 ], P[ 2 ], N[ 0 ], N[ 1 ], N[ 2 ] );
				add_facet( facets, P[ 0 ], P[ 2 ], P[ 3 ], N[ 0 ], N[ 2 ], N[ 3 ] );
				add_facet( facets, P[ 0 ], P[ 3 ], P[ 4 ], N[ 0 ], N[ 3 ], N[ 4 ] );
				add_facet( facets, P[ 0 ], P[ 4 ], P[ 5 ], N[ 0 ], N[ 4 ], N[ 5 ] );
				add_facet( facets, P[ 0 ], P[ 5 ], P[ 6 ], N[ 0 ], N[ 5 ], N[ 6 ] );
				break;

			default:
				break;
			}

			P += __s__[ i ];
			N += __s__[ i ];
		}
	}

	ivector_type _point( const size_type i, const size_type j, const size_type k ) const
	{
		return( ivector_type( static_cast< typename ivector_type::value_type >( i ), static_cast< typename ivector_type::value_type >( j ), static_cast< typename ivector_type::value_type >( k ) ) );
	}

	ivector_type _normal( const image_type &va, const size_type i, const size_type j, const size_type k ) const
	{
		float_type v1 = static_cast< float_type >( _value( va, i - 1, j, k ) - _value( va, i + 1, j, k ) );
		float_type v2 = static_cast< float_type >( _value( va, i, j - 1, k ) - _value( va, i, j + 1, k ) );
		float_type v3 = static_cast< float_type >( _value( va, i, j, k - 1 ) - _value( va, i, j, k + 1 ) );
		vector_type n = vector_type( v1, v2, v3 ).unit( );
		return ivector_type( static_cast< typename ivector_type::value_type >( n.x * 32767 ), static_cast< typename ivector_type::value_type >( n.y * 32767 ), static_cast< typename ivector_type::value_type >( n.z * 32767 ) );
	}

	vector_type __normal( const image_type &va, const size_type i, const size_type j, const size_type k ) const
	{
		float_type v1 = static_cast< float_type >( _value( va, i - 1, j, k ) - _value( va, i + 1, j, k ) );
		float_type v2 = static_cast< float_type >( _value( va, i, j - 1, k ) - _value( va, i, j + 1, k ) );
		float_type v3 = static_cast< float_type >( _value( va, i, j, k - 1 ) - _value( va, i, j, k + 1 ) );
		return( vector_type( v1, v2, v3 ) );
	}

	value_type _value( const image_type &va, const size_t i , const size_t j, const size_t k ) const
	{
		return ( i < va.width( ) && j < va.height( ) && k < va.depth( ) ) ? va( i, j, k ) : value_type( );
	}

	void construct_pointer_difference_array( const image_type &va )
	{
		size_type w = va.width( );
		size_type h = va.height( );

		pda_[ 0 ] = 0;
		pda_[ 1 ] = 1;
		pda_[ 2 ] = w;
		pda_[ 3 ] = w + 1;
		pda_[ 4 ] = w * h;
		pda_[ 5 ] = w * h + 1;
		pda_[ 6 ] = w * h + w;
		pda_[ 7 ] = w * h + w + 1;
	}

	bool construct_point_array( const image_type &va )
	{
		bool ret = pa_.resize( va.width( ), va.height( ), va.depth( ) );

		for( size_t k = 0 ; k < pa_.depth( ) ; k ++ )
		{
			for( size_t j = 0 ; j < pa_.height( ) ; j ++ )
			{
				for( size_t i = 0 ; i < pa_.width( ) ; i ++ )
				{
					pa_( i, j, k ) = _point( i, j, k );
				}
			}
		}

		return( ret );
	}

	bool construct_normal_array( const image_type &va )
	{
		bool ret = na_.resize( va.width( ), va.height( ), va.depth( ) );

		for( size_t k = 0 ; k < na_.depth( ) ; k ++ )
		{
			for( size_t j = 0 ; j < na_.height( ) ; j ++ )
			{
				for( size_t i = 0 ; i < na_.width( ) ; i ++ )
				{
					na_( i, j, k ) = _normal( va, i, j, k );
				}
			}
		}

		return( ret );
	}

	bool construct_interval_tree( const image_type &va )
	{
		std::vector< section_type >	secs;
		min_max_type				minimum, maximum;
		tag_type					tag;
		for( size_t k = 0 ; k < va.depth( ) - 1 ; k ++ )
		{
			for( size_t j = 0 ; j < va.height( ) - 1 ; j ++ )
			{
				for( size_t i = 0 ; i < va.width( ) - 1 ; i ++ )
				{
					tag = i + va.width( ) * j + va.width( ) * va.height( ) * k;
					get_section( va, tag, minimum, maximum );
					if( minimum != maximum )
					{
						secs.push_back( section_type( minimum, maximum, tag ) );
					}
				}
			}
		}
		active_cube_tags_.clear( );
		active_cube_tags_.reserve( secs.size( ) );
		return ( it_.construct( secs ) && active_cube_tags_.capacity( ) >= secs.size( ) );
	}

	void destruct_point_array( )
	{
		pa_.clear( );
	}

	void destruct_normal_array( )
	{
		na_.clear( );
	}

	void destruct_interval_tree( )
	{
		it_.destruct( );
		active_cube_tags_.clear( );
	}

	void get_section( const image_type &va, const tag_type &tag, min_max_type &minimum, min_max_type &maximum ) const
	{
		minimum = va[ tag + pda_[ 0 ] ];
		maximum = va[ tag + pda_[ 0 ] ];
		for( size_t i = 1 ; i < 8 ; i ++ )
		{
			if( va[ tag + pda_[ i ] ] < minimum )
			{
				minimum = va[ tag + pda_[ i ] ];
			}
			else if( va[ tag + pda_[ i ] ] > maximum )
			{
				maximum = va[ tag + pda_[ i ] ];
			}
		}
	}

	size_type pattern( const node_type nda[ 8 ] ) const
	{
		size_type v = 0;
		v += nda[ 0 ].v >= th_ ? 1 : 0;
		v += nda[ 1 ].v >= th_ ? 2 : 0;
		v += nda[ 2 ].v >= th_ ? 4 : 0;
		v += nda[ 3 ].v >= th_ ? 8 : 0;
		v += nda[ 4 ].v >= th_ ? 16 : 0;
		v += nda[ 5 ].v >= th_ ? 32 : 0;
		v += nda[ 6 ].v >= th_ ? 64 : 0;
		v += nda[ 7 ].v >= th_ ? 128 : 0;
		return ( v );
	}

	static void interpolation_about_x( const node_type &nd0, const node_type &nd1, vector_type &p, vector_type &n, float_type th, const vector_type &o, const vector_type &s )
	{
		float_type tp = static_cast< float_type >( ( nd1.v - th ) / ( static_cast< double >( nd1.v ) - nd0.v ) );
		p.x = nd0.p.x * tp + nd1.p.x * ( 1 - tp );
		p.y = nd0.p.y;
		p.z = nd0.p.z;

		float_type tn = static_cast< float_type >( ( static_cast< float_type >( nd1.p.x ) - p.x ) / ( nd1.p.x - nd0.p.x ) );
		n = vector_type( nd0.n.x * tn + nd1.n.x * ( 1 - tn ), nd0.n.y * tn + nd1.n.y * ( 1 - tn ), nd0.n.z * tn + nd1.n.z * ( 1 - tn ) ).unit( );
		p.x = ( p.x + o.x ) * s.x;
		p.y = ( p.y + o.y ) * s.y;
		p.z = ( p.z + o.z ) * s.z;
	}

	static void interpolation_about_y( const node_type &nd0, const node_type &nd1, vector_type &p, vector_type &n, float_type th, const vector_type &o, const vector_type &s )
	{
		float_type tp = static_cast< float_type >( ( nd1.v - th ) / ( static_cast< float_type >( nd1.v ) - nd0.v ) );
		p.x = nd0.p.x;
		p.y = nd0.p.y * tp + nd1.p.y * ( 1 - tp );
		p.z = nd0.p.z;

		float_type tn = static_cast< float_type >( ( static_cast< float_type >( nd1.p.y ) - p.y ) / ( nd1.p.y - nd0.p.y ) );
		n = vector_type( nd0.n.x * tn + nd1.n.x * ( 1 - tn ), nd0.n.y * tn + nd1.n.y * ( 1 - tn ), nd0.n.z * tn + nd1.n.z * ( 1 - tn ) ).unit( );
		p.x = ( p.x + o.x ) * s.x;
		p.y = ( p.y + o.y ) * s.y;
		p.z = ( p.z + o.z ) * s.z;
	}

	static void interpolation_about_z( const node_type &nd0, const node_type &nd1, vector_type &p, vector_type &n, float_type th, const vector_type &o, const vector_type &s )
	{
		float_type tp = static_cast< float_type >( ( nd1.v - th ) / ( static_cast< float_type >( nd1.v ) - nd0.v ) );
		p.x = nd0.p.x;
		p.y = nd0.p.y;
		p.z = nd0.p.z * tp + nd1.p.z * ( 1 - tp );

		float_type tn = static_cast< float_type >( ( static_cast< float_type >( nd1.p.z ) - p.z ) / ( nd1.p.z - nd0.p.z ) );
		n = vector_type( nd0.n.x * tn + nd1.n.x * ( 1 - tn ), nd0.n.y * tn + nd1.n.y * ( 1 - tn ), nd0.n.z * tn + nd1.n.z * ( 1 - tn ) ).unit( );
		p.x = ( p.x + o.x ) * s.x;
		p.y = ( p.y + o.y ) * s.y;
		p.z = ( p.z + o.z ) * s.z;
	}

	void init_function_array( )
	{
		fa_[   0 ] = f000; fa_[   1 ] = f001; fa_[   2 ] = f002; fa_[   3 ] = f003; fa_[   4 ] = f004; fa_[   5 ] = f005; fa_[   6 ] = f006; fa_[   7 ] = f007; 
		fa_[   8 ] = f008; fa_[   9 ] = f009; fa_[  10 ] = f010; fa_[  11 ] = f011; fa_[  12 ] = f012; fa_[  13 ] = f013; fa_[  14 ] = f014; fa_[  15 ] = f015; 
		fa_[  16 ] = f016; fa_[  17 ] = f017; fa_[  18 ] = f018; fa_[  19 ] = f019; fa_[  20 ] = f020; fa_[  21 ] = f021; fa_[  22 ] = f022; fa_[  23 ] = f023; 
		fa_[  24 ] = f024; fa_[  25 ] = f025; fa_[  26 ] = f026; fa_[  27 ] = f027; fa_[  28 ] = f028; fa_[  29 ] = f029; fa_[  30 ] = f030; fa_[  31 ] = f031; 
		fa_[  32 ] = f032; fa_[  33 ] = f033; fa_[  34 ] = f034; fa_[  35 ] = f035; fa_[  36 ] = f036; fa_[  37 ] = f037; fa_[  38 ] = f038; fa_[  39 ] = f039; 
		fa_[  40 ] = f040; fa_[  41 ] = f041; fa_[  42 ] = f042; fa_[  43 ] = f043; fa_[  44 ] = f044; fa_[  45 ] = f045; fa_[  46 ] = f046; fa_[  47 ] = f047; 
		fa_[  48 ] = f048; fa_[  49 ] = f049; fa_[  50 ] = f050; fa_[  51 ] = f051; fa_[  52 ] = f052; fa_[  53 ] = f053; fa_[  54 ] = f054; fa_[  55 ] = f055; 
		fa_[  56 ] = f056; fa_[  57 ] = f057; fa_[  58 ] = f058; fa_[  59 ] = f059; fa_[  60 ] = f060; fa_[  61 ] = f061; fa_[  62 ] = f062; fa_[  63 ] = f063; 
		fa_[  64 ] = f064; fa_[  65 ] = f065; fa_[  66 ] = f066; fa_[  67 ] = f067; fa_[  68 ] = f068; fa_[  69 ] = f069; fa_[  70 ] = f070; fa_[  71 ] = f071; 
		fa_[  72 ] = f072; fa_[  73 ] = f073; fa_[  74 ] = f074; fa_[  75 ] = f075; fa_[  76 ] = f076; fa_[  77 ] = f077; fa_[  78 ] = f078; fa_[  79 ] = f079; 
		fa_[  80 ] = f080; fa_[  81 ] = f081; fa_[  82 ] = f082; fa_[  83 ] = f083; fa_[  84 ] = f084; fa_[  85 ] = f085; fa_[  86 ] = f086; fa_[  87 ] = f087; 
		fa_[  88 ] = f088; fa_[  89 ] = f089; fa_[  90 ] = f090; fa_[  91 ] = f091; fa_[  92 ] = f092; fa_[  93 ] = f093; fa_[  94 ] = f094; fa_[  95 ] = f095; 
		fa_[  96 ] = f096; fa_[  97 ] = f097; fa_[  98 ] = f098; fa_[  99 ] = f099; fa_[ 100 ] = f100; fa_[ 101 ] = f101; fa_[ 102 ] = f102; fa_[ 103 ] = f103; 
		fa_[ 104 ] = f104; fa_[ 105 ] = f105; fa_[ 106 ] = f106; fa_[ 107 ] = f107; fa_[ 108 ] = f108; fa_[ 109 ] = f109; fa_[ 110 ] = f110; fa_[ 111 ] = f111; 
		fa_[ 112 ] = f112; fa_[ 113 ] = f113; fa_[ 114 ] = f114; fa_[ 115 ] = f115; fa_[ 116 ] = f116; fa_[ 117 ] = f117; fa_[ 118 ] = f118; fa_[ 119 ] = f119; 
		fa_[ 120 ] = f120; fa_[ 121 ] = f121; fa_[ 122 ] = f122; fa_[ 123 ] = f123; fa_[ 124 ] = f124; fa_[ 125 ] = f125; fa_[ 126 ] = f126; fa_[ 127 ] = f127; 
		fa_[ 128 ] = f128; fa_[ 129 ] = f129; fa_[ 130 ] = f130; fa_[ 131 ] = f131; fa_[ 132 ] = f132; fa_[ 133 ] = f133; fa_[ 134 ] = f134; fa_[ 135 ] = f135; 
		fa_[ 136 ] = f136; fa_[ 137 ] = f137; fa_[ 138 ] = f138; fa_[ 139 ] = f139; fa_[ 140 ] = f140; fa_[ 141 ] = f141; fa_[ 142 ] = f142; fa_[ 143 ] = f143; 
		fa_[ 144 ] = f144; fa_[ 145 ] = f145; fa_[ 146 ] = f146; fa_[ 147 ] = f147; fa_[ 148 ] = f148; fa_[ 149 ] = f149; fa_[ 150 ] = f150; fa_[ 151 ] = f151; 
		fa_[ 152 ] = f152; fa_[ 153 ] = f153; fa_[ 154 ] = f154; fa_[ 155 ] = f155; fa_[ 156 ] = f156; fa_[ 157 ] = f157; fa_[ 158 ] = f158; fa_[ 159 ] = f159; 
		fa_[ 160 ] = f160; fa_[ 161 ] = f161; fa_[ 162 ] = f162; fa_[ 163 ] = f163; fa_[ 164 ] = f164; fa_[ 165 ] = f165; fa_[ 166 ] = f166; fa_[ 167 ] = f167; 
		fa_[ 168 ] = f168; fa_[ 169 ] = f169; fa_[ 170 ] = f170; fa_[ 171 ] = f171; fa_[ 172 ] = f172; fa_[ 173 ] = f173; fa_[ 174 ] = f174; fa_[ 175 ] = f175; 
		fa_[ 176 ] = f176; fa_[ 177 ] = f177; fa_[ 178 ] = f178; fa_[ 179 ] = f179; fa_[ 180 ] = f180; fa_[ 181 ] = f181; fa_[ 182 ] = f182; fa_[ 183 ] = f183; 
		fa_[ 184 ] = f184; fa_[ 185 ] = f185; fa_[ 186 ] = f186; fa_[ 187 ] = f187; fa_[ 188 ] = f188; fa_[ 189 ] = f189; fa_[ 190 ] = f190; fa_[ 191 ] = f191; 
		fa_[ 192 ] = f192; fa_[ 193 ] = f193; fa_[ 194 ] = f194; fa_[ 195 ] = f195; fa_[ 196 ] = f196; fa_[ 197 ] = f197; fa_[ 198 ] = f198; fa_[ 199 ] = f199; 
		fa_[ 200 ] = f200; fa_[ 201 ] = f201; fa_[ 202 ] = f202; fa_[ 203 ] = f203; fa_[ 204 ] = f204; fa_[ 205 ] = f205; fa_[ 206 ] = f206; fa_[ 207 ] = f207; 
		fa_[ 208 ] = f208; fa_[ 209 ] = f209; fa_[ 210 ] = f210; fa_[ 211 ] = f211; fa_[ 212 ] = f212; fa_[ 213 ] = f213; fa_[ 214 ] = f214; fa_[ 215 ] = f215; 
		fa_[ 216 ] = f216; fa_[ 217 ] = f217; fa_[ 218 ] = f218; fa_[ 219 ] = f219; fa_[ 220 ] = f220; fa_[ 221 ] = f221; fa_[ 222 ] = f222; fa_[ 223 ] = f223; 
		fa_[ 224 ] = f224; fa_[ 225 ] = f225; fa_[ 226 ] = f226; fa_[ 227 ] = f227; fa_[ 228 ] = f228; fa_[ 229 ] = f229; fa_[ 230 ] = f230; fa_[ 231 ] = f231; 
		fa_[ 232 ] = f232; fa_[ 233 ] = f233; fa_[ 234 ] = f234; fa_[ 235 ] = f235; fa_[ 236 ] = f236; fa_[ 237 ] = f237; fa_[ 238 ] = f238; fa_[ 239 ] = f239; 
		fa_[ 240 ] = f240; fa_[ 241 ] = f241; fa_[ 242 ] = f242; fa_[ 243 ] = f243; fa_[ 244 ] = f244; fa_[ 245 ] = f245; fa_[ 246 ] = f246; fa_[ 247 ] = f247; 
		fa_[ 248 ] = f248; fa_[ 249 ] = f249; fa_[ 250 ] = f250; fa_[ 251 ] = f251; fa_[ 252 ] = f252; fa_[ 253 ] = f253; fa_[ 254 ] = f254; fa_[ 255 ] = f255; 
	}

	__MCFUNC__( f000 ); __MCFUNC__( f001 ); __MCFUNC__( f002 ); __MCFUNC__( f003 ); __MCFUNC__( f004 ); __MCFUNC__( f005 ); __MCFUNC__( f006 ); __MCFUNC__( f007 ); __MCFUNC__( f008 ); __MCFUNC__( f009 );
	__MCFUNC__( f010 ); __MCFUNC__( f011 ); __MCFUNC__( f012 ); __MCFUNC__( f013 ); __MCFUNC__( f014 ); __MCFUNC__( f015 ); __MCFUNC__( f016 ); __MCFUNC__( f017 ); __MCFUNC__( f018 ); __MCFUNC__( f019 );
	__MCFUNC__( f020 ); __MCFUNC__( f021 ); __MCFUNC__( f022 ); __MCFUNC__( f023 ); __MCFUNC__( f024 ); __MCFUNC__( f025 ); __MCFUNC__( f026 ); __MCFUNC__( f027 ); __MCFUNC__( f028 ); __MCFUNC__( f029 );
	__MCFUNC__( f030 ); __MCFUNC__( f031 ); __MCFUNC__( f032 ); __MCFUNC__( f033 ); __MCFUNC__( f034 ); __MCFUNC__( f035 ); __MCFUNC__( f036 ); __MCFUNC__( f037 ); __MCFUNC__( f038 ); __MCFUNC__( f039 );
	__MCFUNC__( f040 ); __MCFUNC__( f041 ); __MCFUNC__( f042 ); __MCFUNC__( f043 ); __MCFUNC__( f044 ); __MCFUNC__( f045 ); __MCFUNC__( f046 ); __MCFUNC__( f047 ); __MCFUNC__( f048 ); __MCFUNC__( f049 );
	__MCFUNC__( f050 ); __MCFUNC__( f051 ); __MCFUNC__( f052 ); __MCFUNC__( f053 ); __MCFUNC__( f054 ); __MCFUNC__( f055 ); __MCFUNC__( f056 ); __MCFUNC__( f057 ); __MCFUNC__( f058 ); __MCFUNC__( f059 );
	__MCFUNC__( f060 ); __MCFUNC__( f061 ); __MCFUNC__( f062 ); __MCFUNC__( f063 ); __MCFUNC__( f064 ); __MCFUNC__( f065 ); __MCFUNC__( f066 ); __MCFUNC__( f067 ); __MCFUNC__( f068 ); __MCFUNC__( f069 );
	__MCFUNC__( f070 ); __MCFUNC__( f071 ); __MCFUNC__( f072 ); __MCFUNC__( f073 ); __MCFUNC__( f074 ); __MCFUNC__( f075 ); __MCFUNC__( f076 ); __MCFUNC__( f077 ); __MCFUNC__( f078 ); __MCFUNC__( f079 );
	__MCFUNC__( f080 ); __MCFUNC__( f081 ); __MCFUNC__( f082 ); __MCFUNC__( f083 ); __MCFUNC__( f084 ); __MCFUNC__( f085 ); __MCFUNC__( f086 ); __MCFUNC__( f087 ); __MCFUNC__( f088 ); __MCFUNC__( f089 );
	__MCFUNC__( f090 ); __MCFUNC__( f091 ); __MCFUNC__( f092 ); __MCFUNC__( f093 ); __MCFUNC__( f094 ); __MCFUNC__( f095 ); __MCFUNC__( f096 ); __MCFUNC__( f097 ); __MCFUNC__( f098 ); __MCFUNC__( f099 );

	__MCFUNC__( f100 ); __MCFUNC__( f101 ); __MCFUNC__( f102 ); __MCFUNC__( f103 ); __MCFUNC__( f104 ); __MCFUNC__( f105 ); __MCFUNC__( f106 ); __MCFUNC__( f107 ); __MCFUNC__( f108 ); __MCFUNC__( f109 );
	__MCFUNC__( f110 ); __MCFUNC__( f111 ); __MCFUNC__( f112 ); __MCFUNC__( f113 ); __MCFUNC__( f114 ); __MCFUNC__( f115 ); __MCFUNC__( f116 ); __MCFUNC__( f117 ); __MCFUNC__( f118 ); __MCFUNC__( f119 );
	__MCFUNC__( f120 ); __MCFUNC__( f121 ); __MCFUNC__( f122 ); __MCFUNC__( f123 ); __MCFUNC__( f124 ); __MCFUNC__( f125 ); __MCFUNC__( f126 ); __MCFUNC__( f127 ); __MCFUNC__( f128 ); __MCFUNC__( f129 );
	__MCFUNC__( f130 ); __MCFUNC__( f131 ); __MCFUNC__( f132 ); __MCFUNC__( f133 ); __MCFUNC__( f134 ); __MCFUNC__( f135 ); __MCFUNC__( f136 ); __MCFUNC__( f137 ); __MCFUNC__( f138 ); __MCFUNC__( f139 );
	__MCFUNC__( f140 ); __MCFUNC__( f141 ); __MCFUNC__( f142 ); __MCFUNC__( f143 ); __MCFUNC__( f144 ); __MCFUNC__( f145 ); __MCFUNC__( f146 ); __MCFUNC__( f147 ); __MCFUNC__( f148 ); __MCFUNC__( f149 );
	__MCFUNC__( f150 ); __MCFUNC__( f151 ); __MCFUNC__( f152 ); __MCFUNC__( f153 ); __MCFUNC__( f154 ); __MCFUNC__( f155 ); __MCFUNC__( f156 ); __MCFUNC__( f157 ); __MCFUNC__( f158 ); __MCFUNC__( f159 );
	__MCFUNC__( f160 ); __MCFUNC__( f161 ); __MCFUNC__( f162 ); __MCFUNC__( f163 ); __MCFUNC__( f164 ); __MCFUNC__( f165 ); __MCFUNC__( f166 ); __MCFUNC__( f167 ); __MCFUNC__( f168 ); __MCFUNC__( f169 );
	__MCFUNC__( f170 ); __MCFUNC__( f171 ); __MCFUNC__( f172 ); __MCFUNC__( f173 ); __MCFUNC__( f174 ); __MCFUNC__( f175 ); __MCFUNC__( f176 ); __MCFUNC__( f177 ); __MCFUNC__( f178 ); __MCFUNC__( f179 );
	__MCFUNC__( f180 ); __MCFUNC__( f181 ); __MCFUNC__( f182 ); __MCFUNC__( f183 ); __MCFUNC__( f184 ); __MCFUNC__( f185 ); __MCFUNC__( f186 ); __MCFUNC__( f187 ); __MCFUNC__( f188 ); __MCFUNC__( f189 );
	__MCFUNC__( f190 ); __MCFUNC__( f191 ); __MCFUNC__( f192 ); __MCFUNC__( f193 ); __MCFUNC__( f194 ); __MCFUNC__( f195 ); __MCFUNC__( f196 ); __MCFUNC__( f197 ); __MCFUNC__( f198 ); __MCFUNC__( f199 );

	__MCFUNC__( f200 ); __MCFUNC__( f201 ); __MCFUNC__( f202 ); __MCFUNC__( f203 ); __MCFUNC__( f204 ); __MCFUNC__( f205 ); __MCFUNC__( f206 ); __MCFUNC__( f207 ); __MCFUNC__( f208 ); __MCFUNC__( f209 );
	__MCFUNC__( f210 ); __MCFUNC__( f211 ); __MCFUNC__( f212 ); __MCFUNC__( f213 ); __MCFUNC__( f214 ); __MCFUNC__( f215 ); __MCFUNC__( f216 ); __MCFUNC__( f217 ); __MCFUNC__( f218 ); __MCFUNC__( f219 );
	__MCFUNC__( f220 ); __MCFUNC__( f221 ); __MCFUNC__( f222 ); __MCFUNC__( f223 ); __MCFUNC__( f224 ); __MCFUNC__( f225 ); __MCFUNC__( f226 ); __MCFUNC__( f227 ); __MCFUNC__( f228 ); __MCFUNC__( f229 );
	__MCFUNC__( f230 ); __MCFUNC__( f231 ); __MCFUNC__( f232 ); __MCFUNC__( f233 ); __MCFUNC__( f234 ); __MCFUNC__( f235 ); __MCFUNC__( f236 ); __MCFUNC__( f237 ); __MCFUNC__( f238 ); __MCFUNC__( f239 );
	__MCFUNC__( f240 ); __MCFUNC__( f241 ); __MCFUNC__( f242 ); __MCFUNC__( f243 ); __MCFUNC__( f244 ); __MCFUNC__( f245 ); __MCFUNC__( f246 ); __MCFUNC__( f247 ); __MCFUNC__( f248 ); __MCFUNC__( f249 );
	__MCFUNC__( f250 ); __MCFUNC__( f251 ); __MCFUNC__( f252 ); __MCFUNC__( f253 ); __MCFUNC__( f254 ); __MCFUNC__( f255 );
};

// 0, 0, 0, 0, 0, 0, 0, 0
__MCFUNC0__( f000 );
__MCFUNC0__( f255 );

// 1, 0, 0, 0, 0, 0, 0, 0
__MCFUNC3__( f001, x, 0, 1, y, 0, 2, z, 0, 4 );
__MCFUNC3__( f254, x, 0, 1, z, 0, 4, y, 0, 2 );
__MCFUNC3__( f004, y, 2, 0, x, 2, 3, z, 2, 6 );
__MCFUNC3__( f251, y, 2, 0, z, 2, 6, x, 2, 3 );
__MCFUNC3__( f008, x, 3, 2, y, 3, 1, z, 3, 7 );
__MCFUNC3__( f247, x, 3, 2, z, 3, 7, y, 3, 1 );
__MCFUNC3__( f002, y, 1, 3, x, 1, 0, z, 1, 5 );
__MCFUNC3__( f253, y, 1, 3, z, 1, 5, x, 1, 0 );
__MCFUNC3__( f016, z, 4, 0, y, 4, 6, x, 4, 5 );
__MCFUNC3__( f239, z, 4, 0, x, 4, 5, y, 4, 6 );
__MCFUNC3__( f064, y, 6, 4, z, 6, 2, x, 6, 7 );
__MCFUNC3__( f191, y, 6, 4, x, 6, 7, z, 6, 2 );
__MCFUNC3__( f032, x, 5, 4, y, 5, 7, z, 5, 1 );
__MCFUNC3__( f223, x, 5, 4, z, 5, 1, y, 5, 7 );
__MCFUNC3__( f128, y, 7, 5, x, 7, 6, z, 7, 3 );
__MCFUNC3__( f127, y, 7, 5, z, 7, 3, x, 7, 6 );


// 1, 1, 0, 0, 0, 0, 0, 0
__MCFUNC4__( f003, z, 0, 4, z, 1, 5, y, 1, 3, y, 0, 2 )
__MCFUNC4__( f252, z, 0, 4, y, 0, 2, y, 1, 3, z, 1, 5 )
__MCFUNC4__( f005, z, 2, 6, z, 0, 4, x, 0, 1, x, 2, 3 )
__MCFUNC4__( f250, z, 2, 6, x, 2, 3, x, 0, 1, z, 0, 4 )
__MCFUNC4__( f012, z, 3, 7, z, 2, 6, y, 2, 0, y, 3, 1 )
__MCFUNC4__( f243, z, 3, 7, y, 3, 1, y, 2, 0, z, 2, 6 )
__MCFUNC4__( f010, z, 1, 5, z, 3, 7, x, 3, 2, x, 1, 0 )
__MCFUNC4__( f245, z, 1, 5, x, 1, 0, x, 3, 2, z, 3, 7 )
__MCFUNC4__( f017, x, 4, 5, x, 0, 1, y, 0, 2, y, 4, 6 )
__MCFUNC4__( f238, x, 4, 5, y, 4, 6, y, 0, 2, x, 0, 1 )
__MCFUNC4__( f080, x, 6, 7, x, 4, 5, z, 4, 0, z, 6, 2 )
__MCFUNC4__( f175, x, 6, 7, z, 6, 2, z, 4, 0, x, 4, 5 )
__MCFUNC4__( f068, x, 2, 3, x, 6, 7, y, 6, 4, y, 2, 0 )
__MCFUNC4__( f187, x, 2, 3, y, 2, 0, y, 6, 4, x, 6, 7 )
__MCFUNC4__( f048, z, 5, 1, z, 4, 0, y, 4, 6, y, 5, 7 )
__MCFUNC4__( f207, z, 5, 1, y, 5, 7, y, 4, 6, z, 4, 0 )
__MCFUNC4__( f160, z, 7, 3, z, 5, 1, x, 5, 4, x, 7, 6 )
__MCFUNC4__( f095, z, 7, 3, x, 7, 6, x, 5, 4, z, 5, 1 )
__MCFUNC4__( f192, z, 6, 2, z, 7, 3, y, 7, 5, y, 6, 4 )
__MCFUNC4__( f063, z, 6, 2, y, 6, 4, y, 7, 5, z, 7, 3 )
__MCFUNC4__( f034, x, 1, 0, x, 5, 4, y, 5, 7, y, 1, 3 )
__MCFUNC4__( f221, x, 1, 0, y, 1, 3, y, 5, 7, x, 5, 4 )
__MCFUNC4__( f136, x, 7, 6, x, 3, 2, y, 3, 1, y, 7, 5 )
__MCFUNC4__( f119, x, 7, 6, y, 7, 5, y, 3, 1, x, 3, 2 )


// 1, 0, 0, 1, 0, 0, 0, 0
__MCFUNC6_1__( f009, x, 0, 1, y, 0, 2, z, 0, 4, y, 3, 1, z, 3, 7, x, 3, 2 )
__MCFUNC6_1__( f006, y, 2, 0, x, 2, 3, z, 2, 6, x, 1, 0, z, 1, 5, y, 1, 3 )
__MCFUNC6_1__( f020, z, 4, 0, y, 4, 6, x, 4, 5, y, 2, 0, x, 2, 3, z, 2, 6 )
__MCFUNC6_1__( f065, y, 6, 4, z, 6, 2, x, 6, 7, z, 0, 4, x, 0, 1, y, 0, 2 )
__MCFUNC6_1__( f096, x, 5, 4, y, 5, 7, z, 5, 1, y, 6, 4, z, 6, 2, x, 6, 7 )
__MCFUNC6_1__( f144, y, 7, 5, x, 7, 6, z, 7, 3, x, 4, 5, z, 4, 0, y, 4, 6 )
__MCFUNC6_1__( f130, z, 1, 5, y, 1, 3, x, 1, 0, y, 7, 5, x, 7, 6, z, 7, 3 )
__MCFUNC6_1__( f040, y, 3, 1, z, 3, 7, x, 3, 2, z, 5, 1, x, 5, 4, y, 5, 7 )
__MCFUNC6_1__( f072, z, 3, 7, x, 3, 2, y, 3, 1, x, 6, 7, y, 6, 4, z, 6, 2 )
__MCFUNC6_1__( f132, x, 2, 3, z, 2, 6, y, 2, 0, z, 7, 3, y, 7, 5, x, 7, 6 )
__MCFUNC6_1__( f033, z, 0, 4, x, 0, 1, y, 0, 2, x, 5, 4, y, 5, 7, z, 5, 1 )
__MCFUNC6_1__( f018, x, 1, 0, z, 1, 5, y, 1, 3, z, 4, 0, y, 4, 6, x, 4, 5 )
__MCFUNC6_2__( f246, z, 0, 4, y, 0, 2, x, 3, 2, z, 3, 7, y, 3, 1, x, 0, 1 )
__MCFUNC6_2__( f249, z, 2, 6, x, 2, 3, y, 1, 3, z, 1, 5, x, 1, 0, y, 2, 0 )
__MCFUNC6_2__( f235, x, 4, 5, y, 4, 6, z, 2, 6, x, 2, 3, y, 2, 0, z, 4, 0 )
__MCFUNC6_2__( f190, x, 6, 7, z, 6, 2, y, 0, 2, x, 0, 1, z, 0, 4, y, 6, 4 )
__MCFUNC6_2__( f159, z, 5, 1, y, 5, 7, x, 6, 7, z, 6, 2, y, 6, 4, x, 5, 4 )
__MCFUNC6_2__( f111, z, 7, 3, x, 7, 6, y, 4, 6, z, 4, 0, x, 4, 5, y, 7, 5 )
__MCFUNC6_2__( f125, x, 1, 0, y, 1, 3, z, 7, 3, x, 7, 6, y, 7, 5, z, 1, 5 )
__MCFUNC6_2__( f215, x, 3, 2, z, 3, 7, y, 5, 7, x, 5, 4, z, 5, 1, y, 3, 1 )
__MCFUNC6_2__( f183, y, 3, 1, x, 3, 2, z, 6, 2, y, 6, 4, x, 6, 7, z, 3, 7 )
__MCFUNC6_2__( f123, y, 2, 0, z, 2, 6, x, 7, 6, y, 7, 5, z, 7, 3, x, 2, 3 )
__MCFUNC6_2__( f222, y, 0, 2, x, 0, 1, z, 5, 1, y, 5, 7, x, 5, 4, z, 0, 4 )
__MCFUNC6_2__( f237, y, 1, 3, z, 1, 5, x, 4, 5, y, 4, 6, z, 4, 0, x, 1, 0 )


// 1, 0, 0, 0, 0, 0, 0, 1
__MCFUNC6_1__( f129, x, 0, 1, y, 0, 2, z, 0, 4, z, 7, 3, y, 7, 5, x, 7, 6 )
__MCFUNC6_1__( f126, x, 0, 1, z, 0, 4, y, 0, 2, z, 7, 3, x, 7, 6, y, 7, 5 )
__MCFUNC6_1__( f036, y, 2, 0, x, 2, 3, z, 2, 6, z, 5, 1, x, 5, 4, y, 5, 7 )
__MCFUNC6_1__( f219, y, 2, 0, z, 2, 6, x, 2, 3, z, 5, 1, y, 5, 7, x, 5, 4 )
__MCFUNC6_1__( f024, x, 3, 2, y, 3, 1, z, 3, 7, z, 4, 0, y, 4, 6, x, 4, 5 )
__MCFUNC6_1__( f231, x, 3, 2, z, 3, 7, y, 3, 1, z, 4, 0, x, 4, 5, y, 4, 6 )
__MCFUNC6_1__( f066, y, 1, 3, x, 1, 0, z, 1, 5, z, 6, 2, x, 6, 7, y, 6, 4 )
__MCFUNC6_1__( f189, y, 1, 3, z, 1, 5, x, 1, 0, z, 6, 2, y, 6, 4, x, 6, 7 )


// 1, 1, 1, 0, 0, 0, 0, 0

__MCFUNC5__( f007, z, 0, 4, z, 1, 5, y, 1, 3, x, 2, 3, z, 2, 6 )
__MCFUNC5__( f248, z, 0, 4, z, 2, 6, x, 2, 3, y, 1, 3, z, 1, 5 )
__MCFUNC5__( f013, z, 2, 6, z, 0, 4, x, 0, 1, y, 3, 1, z, 3, 7 )
__MCFUNC5__( f242, z, 2, 6, z, 3, 7, y, 3, 1, x, 0, 1, z, 0, 4 )
__MCFUNC5__( f014, z, 3, 7, z, 2, 6, y, 2, 0, x, 1, 0, z, 1, 5 )
__MCFUNC5__( f241, z, 3, 7, z, 1, 5, x, 1, 0, y, 2, 0, z, 2, 6 )
__MCFUNC5__( f011, z, 1, 5, z, 3, 7, x, 3, 2, y, 0, 2, z, 0, 4 )
__MCFUNC5__( f244, z, 1, 5, z, 0, 4, y, 0, 2, x, 3, 2, z, 3, 7 )
__MCFUNC5__( f081, x, 4, 5, x, 0, 1, y, 0, 2, z, 6, 2, x, 6, 7 )
__MCFUNC5__( f174, x, 4, 5, x, 6, 7, z, 6, 2, y, 0, 2, x, 0, 1 )
__MCFUNC5__( f084, x, 6, 7, x, 4, 5, z, 4, 0, y, 2, 0, x, 2, 3 )
__MCFUNC5__( f171, x, 6, 7, x, 2, 3, y, 2, 0, z, 4, 0, x, 4, 5 )
__MCFUNC5__( f069, x, 2, 3, x, 6, 7, y, 6, 4, z, 0, 4, x, 0, 1 )
__MCFUNC5__( f186, x, 2, 3, x, 0, 1, z, 0, 4, y, 6, 4, x, 6, 7 )
__MCFUNC5__( f021, x, 0, 1, x, 2, 3, z, 2, 6, y, 4, 6, x, 4, 5 )
__MCFUNC5__( f234, x, 0, 1, x, 4, 5, y, 4, 6, z, 2, 6, x, 2, 3 )
__MCFUNC5__( f176, z, 5, 1, z, 4, 0, y, 4, 6, x, 7, 6, z, 7, 3 )
__MCFUNC5__( f079, z, 5, 1, z, 7, 3, x, 7, 6, y, 4, 6, z, 4, 0 )
__MCFUNC5__( f224, z, 7, 3, z, 5, 1, x, 5, 4, y, 6, 4, z, 6, 2 )
__MCFUNC5__( f031, z, 7, 3, z, 6, 2, y, 6, 4, x, 5, 4, z, 5, 1 )
__MCFUNC5__( f208, z, 6, 2, z, 7, 3, y, 7, 5, x, 4, 5, z, 4, 0 )
__MCFUNC5__( f047, z, 6, 2, z, 4, 0, x, 4, 5, y, 7, 5, z, 7, 3 )
__MCFUNC5__( f112, z, 4, 0, z, 6, 2, x, 6, 7, y, 5, 7, z, 5, 1 )
__MCFUNC5__( f143, z, 4, 0, z, 5, 1, y, 5, 7, x, 6, 7, z, 6, 2 )
__MCFUNC5__( f042, x, 1, 0, x, 5, 4, y, 5, 7, z, 3, 7, x, 3, 2 )
__MCFUNC5__( f213, x, 1, 0, x, 3, 2, z, 3, 7, y, 5, 7, x, 5, 4 )
__MCFUNC5__( f138, x, 3, 2, x, 1, 0, z, 1, 5, y, 7, 5, x, 7, 6 )
__MCFUNC5__( f117, x, 3, 2, x, 7, 6, y, 7, 5, z, 1, 5, x, 1, 0 )
__MCFUNC5__( f168, x, 7, 6, x, 3, 2, y, 3, 1, z, 5, 1, x, 5, 4 )
__MCFUNC5__( f087, x, 7, 6, x, 5, 4, z, 5, 1, y, 3, 1, x, 3, 2 )
__MCFUNC5__( f162, x, 5, 4, x, 7, 6, z, 7, 3, y, 1, 3, x, 1, 0 )
__MCFUNC5__( f093, x, 5, 4, x, 1, 0, y, 1, 3, z, 7, 3, x, 7, 6 )
__MCFUNC5__( f140, y, 3, 1, y, 7, 5, x, 7, 6, z, 2, 6, y, 2, 0 )
__MCFUNC5__( f115, y, 3, 1, y, 2, 0, z, 2, 6, x, 7, 6, y, 7, 5 )
__MCFUNC5__( f076, y, 2, 0, y, 3, 1, z, 3, 7, x, 6, 7, y, 6, 4 )
__MCFUNC5__( f179, y, 2, 0, y, 6, 4, x, 6, 7, z, 3, 7, y, 3, 1 )
__MCFUNC5__( f196, y, 6, 4, y, 2, 0, x, 2, 3, z, 7, 3, y, 7, 5 )
__MCFUNC5__( f059, y, 6, 4, y, 7, 5, z, 7, 3, x, 2, 3, y, 2, 0 )
__MCFUNC5__( f200, y, 7, 5, y, 6, 4, z, 6, 2, x, 3, 2, y, 3, 1 )
__MCFUNC5__( f055, y, 7, 5, y, 3, 1, x, 3, 2, z, 6, 2, y, 6, 4 )
__MCFUNC5__( f019, y, 0, 2, y, 4, 6, x, 4, 5, z, 1, 5, y, 1, 3 )
__MCFUNC5__( f236, y, 0, 2, y, 1, 3, z, 1, 5, x, 4, 5, y, 4, 6 )
__MCFUNC5__( f035, y, 1, 3, y, 0, 2, z, 0, 4, x, 5, 4, y, 5, 7 )
__MCFUNC5__( f220, y, 1, 3, y, 5, 7, x, 5, 4, z, 0, 4, y, 0, 2 )
__MCFUNC5__( f050, y, 5, 7, y, 1, 3, x, 1, 0, z, 4, 0, y, 4, 6 )
__MCFUNC5__( f205, y, 5, 7, y, 4, 6, z, 4, 0, x, 1, 0, y, 1, 3 )
__MCFUNC5__( f049, y, 4, 6, y, 5, 7, z, 5, 1, x, 0, 1, y, 0, 2 )
__MCFUNC5__( f206, y, 4, 6, y, 0, 2, x, 0, 1, z, 5, 1, y, 5, 7 )


// 1, 1, 0, 0, 0, 0, 1, 0
__MCFUNC7_1__( f067, z, 0, 4, z, 1, 5, y, 1, 3, y, 0, 2, z, 6, 2, x, 6, 7, y, 6, 4 )
__MCFUNC7_1__( f133, z, 2, 6, z, 0, 4, x, 0, 1, x, 2, 3, z, 7, 3, y, 7, 5, x, 7, 6 )
__MCFUNC7_1__( f044, z, 3, 7, z, 2, 6, y, 2, 0, y, 3, 1, z, 5, 1, x, 5, 4, y, 5, 7 )
__MCFUNC7_1__( f026, z, 1, 5, z, 3, 7, x, 3, 2, x, 1, 0, z, 4, 0, y, 4, 6, x, 4, 5 )
__MCFUNC7_1__( f145, x, 4, 5, x, 0, 1, y, 0, 2, y, 4, 6, x, 7, 6, z, 7, 3, y, 7, 5 )
__MCFUNC7_1__( f088, x, 6, 7, x, 4, 5, z, 4, 0, z, 6, 2, x, 3, 2, y, 3, 1, z, 3, 7 )
__MCFUNC7_1__( f070, x, 2, 3, x, 6, 7, y, 6, 4, y, 2, 0, x, 1, 0, z, 1, 5, y, 1, 3 )
__MCFUNC7_1__( f037, x, 0, 1, x, 2, 3, z, 2, 6, z, 0, 4, x, 5, 4, y, 5, 7, z, 5, 1 )
__MCFUNC7_1__( f056, z, 5, 1, z, 4, 0, y, 4, 6, y, 5, 7, z, 3, 7, x, 3, 2, y, 3, 1 )
__MCFUNC7_1__( f164, z, 7, 3, z, 5, 1, x, 5, 4, x, 7, 6, z, 2, 6, y, 2, 0, x, 2, 3 )
__MCFUNC7_1__( f193, z, 6, 2, z, 7, 3, y, 7, 5, y, 6, 4, z, 0, 4, x, 0, 1, y, 0, 2 )
__MCFUNC7_1__( f082, z, 4, 0, z, 6, 2, x, 6, 7, x, 4, 5, z, 1, 5, y, 1, 3, x, 1, 0 )
__MCFUNC7_1__( f038, x, 1, 0, x, 5, 4, y, 5, 7, y, 1, 3, x, 2, 3, z, 2, 6, y, 2, 0 )
__MCFUNC7_1__( f074, x, 3, 2, x, 1, 0, z, 1, 5, z, 3, 7, x, 6, 7, y, 6, 4, z, 6, 2 )
__MCFUNC7_1__( f152, x, 7, 6, x, 3, 2, y, 3, 1, y, 7, 5, x, 4, 5, z, 4, 0, y, 4, 6 )
__MCFUNC7_1__( f161, x, 5, 4, x, 7, 6, z, 7, 3, z, 5, 1, x, 0, 1, y, 0, 2, z, 0, 4 )
__MCFUNC7_1__( f137, y, 3, 1, y, 7, 5, x, 7, 6, x, 3, 2, y, 0, 2, z, 0, 4, x, 0, 1 )
__MCFUNC7_1__( f028, y, 2, 0, y, 3, 1, z, 3, 7, z, 2, 6, y, 4, 6, x, 4, 5, z, 4, 0 )
__MCFUNC7_1__( f100, y, 6, 4, y, 2, 0, x, 2, 3, x, 6, 7, y, 5, 7, z, 5, 1, x, 5, 4 )
__MCFUNC7_1__( f194, y, 7, 5, y, 6, 4, z, 6, 2, z, 7, 3, y, 1, 3, x, 1, 0, z, 1, 5 )
__MCFUNC7_1__( f025, y, 0, 2, y, 4, 6, x, 4, 5, x, 0, 1, y, 3, 1, z, 3, 7, x, 3, 2 )
__MCFUNC7_1__( f131, y, 1, 3, y, 0, 2, z, 0, 4, z, 1, 5, y, 7, 5, x, 7, 6, z, 7, 3 )
__MCFUNC7_1__( f098, y, 5, 7, y, 1, 3, x, 1, 0, x, 5, 4, y, 6, 4, z, 6, 2, x, 6, 7 )
__MCFUNC7_1__( f052, y, 4, 6, y, 5, 7, z, 5, 1, z, 4, 0, y, 2, 0, x, 2, 3, z, 2, 6 )
__MCFUNC7_2__( f188, x, 6, 7, x, 6, 7, y, 0, 2, y, 1, 3, z, 1, 5, z, 0, 4, y, 6, 4 )
__MCFUNC7_2__( f122, y, 7, 5, y, 7, 5, x, 2, 3, x, 0, 1, z, 0, 4, z, 2, 6, x, 7, 6 )
__MCFUNC7_2__( f211, x, 5, 4, x, 5, 4, y, 3, 1, y, 2, 0, z, 2, 6, z, 3, 7, y, 5, 7 )
__MCFUNC7_2__( f229, y, 4, 6, y, 4, 6, x, 1, 0, x, 3, 2, z, 3, 7, z, 1, 5, x, 4, 5 )
__MCFUNC7_2__( f110, z, 7, 3, z, 7, 3, y, 4, 6, y, 0, 2, x, 0, 1, x, 4, 5, y, 7, 5 )
__MCFUNC7_2__( f167, y, 3, 1, y, 3, 1, z, 6, 2, z, 4, 0, x, 4, 5, x, 6, 7, z, 3, 7 )
__MCFUNC7_2__( f185, z, 1, 5, z, 1, 5, y, 2, 0, y, 6, 4, x, 6, 7, x, 2, 3, y, 1, 3 )
__MCFUNC7_2__( f218, y, 5, 7, y, 5, 7, z, 0, 4, z, 2, 6, x, 2, 3, x, 0, 1, z, 5, 1 )
__MCFUNC7_2__( f199, x, 3, 2, x, 3, 2, y, 5, 7, y, 4, 6, z, 4, 0, z, 5, 1, y, 3, 1 )
__MCFUNC7_2__( f091, y, 2, 0, y, 2, 0, x, 7, 6, x, 5, 4, z, 5, 1, z, 7, 3, x, 2, 3 )
__MCFUNC7_2__( f062, x, 0, 1, x, 0, 1, y, 6, 4, y, 7, 5, z, 7, 3, z, 6, 2, y, 0, 2 )
__MCFUNC7_2__( f173, y, 1, 3, y, 1, 3, x, 4, 5, x, 6, 7, z, 6, 2, z, 4, 0, x, 1, 0 )
__MCFUNC7_2__( f217, z, 2, 6, z, 2, 6, y, 1, 3, y, 5, 7, x, 5, 4, x, 1, 0, y, 2, 0 )
__MCFUNC7_2__( f181, y, 6, 4, y, 6, 4, z, 3, 7, z, 1, 5, x, 1, 0, x, 3, 2, z, 6, 2 )
__MCFUNC7_2__( f103, z, 4, 0, z, 4, 0, y, 7, 5, y, 3, 1, x, 3, 2, x, 7, 6, y, 4, 6 )
__MCFUNC7_2__( f094, y, 0, 2, y, 0, 2, z, 5, 1, z, 7, 3, x, 7, 6, x, 5, 4, z, 0, 4 )
__MCFUNC7_2__( f118, z, 0, 4, z, 0, 4, x, 3, 2, x, 7, 6, y, 7, 5, y, 3, 1, x, 0, 1 )
__MCFUNC7_2__( f227, x, 4, 5, x, 4, 5, z, 2, 6, z, 3, 7, y, 3, 1, y, 2, 0, z, 4, 0 )
__MCFUNC7_2__( f155, z, 5, 1, z, 5, 1, x, 6, 7, x, 2, 3, y, 2, 0, y, 6, 4, x, 5, 4 )
__MCFUNC7_2__( f061, x, 1, 0, x, 1, 0, z, 7, 3, z, 6, 2, y, 6, 4, y, 7, 5, z, 1, 5 )
__MCFUNC7_2__( f230, z, 3, 7, z, 3, 7, x, 0, 1, x, 4, 5, y, 4, 6, y, 0, 2, x, 3, 2 )
__MCFUNC7_2__( f124, x, 7, 6, x, 7, 6, z, 1, 5, z, 0, 4, y, 0, 2, y, 1, 3, z, 7, 3 )
__MCFUNC7_2__( f157, z, 6, 2, z, 6, 2, x, 5, 4, x, 1, 0, y, 1, 3, y, 5, 7, x, 6, 7 )
__MCFUNC7_2__( f203, x, 2, 3, x, 2, 3, z, 4, 0, z, 5, 1, y, 5, 7, y, 4, 6, z, 2, 6 )


// 1, 0, 0, 1, 0, 1, 0, 0
__MCFUNC9_1__( f041, x, 0, 1, y, 0, 2, z, 0, 4, y, 3, 1, z, 3, 7, x, 3, 2, z, 5, 1, x, 5, 4, y, 5, 7 )
__MCFUNC9_1__( f022, y, 2, 0, x, 2, 3, z, 2, 6, x, 1, 0, z, 1, 5, y, 1, 3, z, 4, 0, y, 4, 6, x, 4, 5 )
__MCFUNC9_1__( f073, x, 3, 2, y, 3, 1, z, 3, 7, y, 0, 2, z, 0, 4, x, 0, 1, z, 6, 2, x, 6, 7, y, 6, 4 )
__MCFUNC9_1__( f134, y, 1, 3, x, 1, 0, z, 1, 5, x, 2, 3, z, 2, 6, y, 2, 0, z, 7, 3, y, 7, 5, x, 7, 6 )
__MCFUNC9_1__( f097, y, 6, 4, z, 6, 2, x, 6, 7, z, 0, 4, x, 0, 1, y, 0, 2, x, 5, 4, y, 5, 7, z, 5, 1 )
__MCFUNC9_1__( f148, z, 2, 6, y, 2, 0, x, 2, 3, y, 4, 6, x, 4, 5, z, 4, 0, x, 7, 6, z, 7, 3, y, 7, 5 )
__MCFUNC9_1__( f146, y, 7, 5, x, 7, 6, z, 7, 3, x, 4, 5, z, 4, 0, y, 4, 6, z, 1, 5, y, 1, 3, x, 1, 0 )
__MCFUNC9_1__( f104, x, 6, 7, y, 6, 4, z, 6, 2, y, 5, 7, z, 5, 1, x, 5, 4, z, 3, 7, x, 3, 2, y, 3, 1 )
__MCFUNC9_2__( f214, x, 1, 0, z, 1, 5, y, 1, 3, z, 0, 4, y, 2, 0, x, 3, 2, z, 7, 3, y, 5, 7, x, 4, 5 )
__MCFUNC9_2__( f233, y, 0, 2, z, 0, 4, x, 0, 1, z, 2, 6, x, 3, 2, y, 1, 3, z, 5, 1, x, 4, 5, y, 6, 4 )
__MCFUNC9_2__( f182, x, 2, 3, z, 2, 6, y, 2, 0, z, 3, 7, y, 1, 3, x, 0, 1, z, 4, 0, y, 6, 4, x, 7, 6 )
__MCFUNC9_2__( f121, y, 3, 1, z, 3, 7, x, 3, 2, z, 1, 5, x, 0, 1, y, 2, 0, z, 6, 2, x, 7, 6, y, 5, 7 )
__MCFUNC9_2__( f158, y, 4, 6, x, 4, 5, z, 4, 0, x, 6, 7, z, 2, 6, y, 0, 2, x, 1, 0, z, 5, 1, y, 7, 5 )
__MCFUNC9_2__( f107, z, 6, 2, x, 6, 7, y, 6, 4, x, 2, 3, y, 0, 2, z, 4, 0, x, 5, 4, y, 7, 5, z, 3, 7 )
__MCFUNC9_2__( f109, y, 5, 7, z, 5, 1, x, 5, 4, z, 7, 3, x, 6, 7, y, 4, 6, z, 0, 4, x, 1, 0, y, 3, 1 )
__MCFUNC9_2__( f151, x, 7, 6, z, 7, 3, y, 7, 5, z, 6, 2, y, 4, 6, x, 5, 4, z, 1, 5, y, 3, 1, x, 2, 3 )


// 1, 1, 1, 1, 0, 0, 0, 0
__MCFUNC4__( f015, z, 0, 4, z, 1, 5, z, 3, 7, z, 2, 6 )
__MCFUNC4__( f085, x, 4, 5, x, 0, 1, x, 2, 3, x, 6, 7 )
__MCFUNC4__( f240, z, 5, 1, z, 4, 0, z, 6, 2, z, 7, 3 )
__MCFUNC4__( f170, x, 1, 0, x, 5, 4, x, 7, 6, x, 3, 2 )
__MCFUNC4__( f204, y, 3, 1, y, 7, 5, y, 6, 4, y, 2, 0 )
__MCFUNC4__( f051, y, 0, 2, y, 4, 6, y, 5, 7, y, 1, 3 )


// 1, 1, 1, 0, 1, 0, 0, 0
__MCFUNC6_2__( f023, z, 1, 5, y, 3, 1, x, 2, 3, z, 6, 2, y, 4, 6, x, 5, 4 )
__MCFUNC6_2__( f077, z, 0, 4, x, 1, 0, y, 3, 1, z, 7, 3, x, 6, 7, y, 4, 6 )
__MCFUNC6_2__( f142, z, 2, 6, y, 0, 2, x, 1, 0, z, 5, 1, y, 7, 5, x, 6, 7 )
__MCFUNC6_2__( f043, z, 3, 7, x, 2, 3, y, 0, 2, z, 4, 0, x, 5, 4, y, 7, 5 )
__MCFUNC6_2__( f113, x, 0, 1, y, 2, 0, z, 6, 2, x, 7, 6, y, 5, 7, z, 1, 5 )
__MCFUNC6_2__( f212, x, 4, 5, z, 0, 4, y, 2, 0, x, 3, 2, z, 7, 3, y, 5, 7 )
__MCFUNC6_2__( f178, z, 4, 0, y, 6, 4, x, 7, 6, z, 3, 7, y, 1, 3, x, 0, 1 )
__MCFUNC6_2__( f232, z, 5, 1, x, 4, 5, y, 6, 4, z, 2, 6, x, 3, 2, y, 1, 3 )


// 1, 1, 1, 0, 0, 1, 0, 0
__MCFUNC6_2__( f039, y, 1, 3, x, 2, 3, z, 2, 6, z, 0, 4, x, 5, 4, y, 5, 7 )
__MCFUNC6_2__( f029, x, 0, 1, y, 3, 1, z, 3, 7, z, 2, 6, y, 4, 6, x, 4, 5 )
__MCFUNC6_2__( f078, y, 2, 0, x, 1, 0, z, 1, 5, z, 3, 7, x, 6, 7, y, 6, 4 )
__MCFUNC6_2__( f139, x, 3, 2, y, 0, 2, z, 0, 4, z, 1, 5, y, 7, 5, x, 7, 6 )
__MCFUNC6_2__( f083, y, 0, 2, z, 6, 2, x, 6, 7, x, 4, 5, z, 1, 5, y, 1, 3 )
__MCFUNC6_2__( f116, z, 4, 0, y, 2, 0, x, 2, 3, x, 6, 7, y, 5, 7, z, 5, 1 )
__MCFUNC6_2__( f197, y, 6, 4, z, 0, 4, x, 0, 1, x, 2, 3, z, 7, 3, y, 7, 5 )
__MCFUNC6_2__( f177, y, 4, 6, x, 7, 6, z, 7, 3, z, 5, 1, x, 0, 1, y, 0, 2 )
__MCFUNC6_2__( f226, x, 5, 4, y, 6, 4, z, 6, 2, z, 7, 3, y, 1, 3, x, 1, 0 )
__MCFUNC6_2__( f216, y, 7, 5, x, 4, 5, z, 4, 0, z, 6, 2, x, 3, 2, y, 3, 1 )
__MCFUNC6_2__( f058, y, 5, 7, z, 3, 7, x, 3, 2, x, 1, 0, z, 4, 0, y, 4, 6 )
__MCFUNC6_2__( f172, y, 3, 1, z, 5, 1, x, 5, 4, x, 7, 6, z, 2, 6, y, 2, 0 )


// 1, 1, 1, 0, 0, 0, 1, 0
__MCFUNC6_2__( f071, x, 2, 3, x, 6, 7, y, 6, 4, z, 0, 4, z, 1, 5, y, 1, 3 )
__MCFUNC6_2__( f141, y, 3, 1, y, 7, 5, x, 7, 6, z, 2, 6, z, 0, 4, x, 0, 1 )
__MCFUNC6_2__( f046, x, 1, 0, x, 5, 4, y, 5, 7, z, 3, 7, z, 2, 6, y, 2, 0 )
__MCFUNC6_2__( f027, y, 0, 2, y, 4, 6, x, 4, 5, z, 1, 5, z, 3, 7, x, 3, 2 )
__MCFUNC6_2__( f209, z, 6, 2, z, 7, 3, y, 7, 5, x, 4, 5, x, 0, 1, y, 0, 2 )
__MCFUNC6_2__( f092, y, 2, 0, y, 3, 1, z, 3, 7, x, 6, 7, x, 4, 5, z, 4, 0 )
__MCFUNC6_2__( f053, y, 4, 6, y, 5, 7, z, 5, 1, x, 0, 1, x, 2, 3, z, 2, 6 )
__MCFUNC6_2__( f184, x, 7, 6, x, 3, 2, y, 3, 1, z, 5, 1, z, 4, 0, y, 4, 6 )
__MCFUNC6_2__( f228, y, 6, 4, y, 2, 0, x, 2, 3, z, 7, 3, z, 5, 1, x, 5, 4 )
__MCFUNC6_2__( f114, y, 5, 7, y, 1, 3, x, 1, 0, z, 4, 0, z, 6, 2, x, 6, 7 )
__MCFUNC6_2__( f202, y, 7, 5, y, 6, 4, z, 6, 2, x, 3, 2, x, 1, 0, z, 1, 5 )
__MCFUNC6_2__( f163, y, 1, 3, y, 0, 2, z, 0, 4, x, 5, 4, x, 7, 6, z, 7, 3 )


// 1, 1, 1, 0, 0, 0, 0, 1
__MCFUNC8_1__( f135, z, 0, 4, z, 1, 5, y, 1, 3, x, 2, 3, z, 2, 6, z, 7, 3, y, 7, 5, x, 7, 6 )
__MCFUNC8_1__( f045, z, 2, 6, z, 0, 4, x, 0, 1, y, 3, 1, z, 3, 7, z, 5, 1, x, 5, 4, y, 5, 7 )
__MCFUNC8_1__( f030, z, 3, 7, z, 2, 6, y, 2, 0, x, 1, 0, z, 1, 5, z, 4, 0, y, 4, 6, x, 4, 5 )
__MCFUNC8_1__( f075, z, 1, 5, z, 3, 7, x, 3, 2, y, 0, 2, z, 0, 4, z, 6, 2, x, 6, 7, y, 6, 4 )
__MCFUNC8_1__( f089, x, 4, 5, x, 0, 1, y, 0, 2, z, 6, 2, x, 6, 7, x, 3, 2, y, 3, 1, z, 3, 7 )
__MCFUNC8_1__( f086, x, 6, 7, x, 4, 5, z, 4, 0, y, 2, 0, x, 2, 3, x, 1, 0, z, 1, 5, y, 1, 3 )
__MCFUNC8_1__( f101, x, 2, 3, x, 6, 7, y, 6, 4, z, 0, 4, x, 0, 1, x, 5, 4, y, 5, 7, z, 5, 1 )
__MCFUNC8_1__( f149, x, 0, 1, x, 2, 3, z, 2, 6, y, 4, 6, x, 4, 5, x, 7, 6, z, 7, 3, y, 7, 5 )
__MCFUNC8_1__( f180, z, 5, 1, z, 4, 0, y, 4, 6, x, 7, 6, z, 7, 3, z, 2, 6, y, 2, 0, x, 2, 3 )
__MCFUNC8_1__( f225, z, 7, 3, z, 5, 1, x, 5, 4, y, 6, 4, z, 6, 2, z, 0, 4, x, 0, 1, y, 0, 2 )
__MCFUNC8_1__( f210, z, 6, 2, z, 7, 3, y, 7, 5, x, 4, 5, z, 4, 0, z, 1, 5, y, 1, 3, x, 1, 0 )
__MCFUNC8_1__( f120, z, 4, 0, z, 6, 2, x, 6, 7, y, 5, 7, z, 5, 1, z, 3, 7, x, 3, 2, y, 3, 1 )
__MCFUNC8_1__( f106, x, 1, 0, x, 5, 4, y, 5, 7, z, 3, 7, x, 3, 2, x, 6, 7, y, 6, 4, z, 6, 2 )
__MCFUNC8_1__( f154, x, 3, 2, x, 1, 0, z, 1, 5, y, 7, 5, x, 7, 6, x, 4, 5, z, 4, 0, y, 4, 6 )
__MCFUNC8_1__( f169, x, 7, 6, x, 3, 2, y, 3, 1, z, 5, 1, x, 5, 4, x, 0, 1, y, 0, 2, z, 0, 4 )
__MCFUNC8_1__( f166, x, 5, 4, x, 7, 6, z, 7, 3, y, 1, 3, x, 1, 0, x, 2, 3, z, 2, 6, y, 2, 0 )
__MCFUNC8_1__( f156, y, 3, 1, y, 7, 5, x, 7, 6, z, 2, 6, y, 2, 0, y, 4, 6, x, 4, 5, z, 4, 0 )
__MCFUNC8_1__( f108, y, 2, 0, y, 3, 1, z, 3, 7, x, 6, 7, y, 6, 4, y, 5, 7, z, 5, 1, x, 5, 4 )
__MCFUNC8_1__( f198, y, 6, 4, y, 2, 0, x, 2, 3, z, 7, 3, y, 7, 5, y, 1, 3, x, 1, 0, z, 1, 5 )
__MCFUNC8_1__( f201, y, 7, 5, y, 6, 4, z, 6, 2, x, 3, 2, y, 3, 1, y, 0, 2, z, 0, 4, x, 0, 1 )
__MCFUNC8_1__( f147, y, 0, 2, y, 4, 6, x, 4, 5, z, 1, 5, y, 1, 3, y, 7, 5, x, 7, 6, z, 7, 3 )
__MCFUNC8_1__( f099, y, 1, 3, y, 0, 2, z, 0, 4, x, 5, 4, y, 5, 7, y, 6, 4, z, 6, 2, x, 6, 7 )
__MCFUNC8_1__( f054, y, 5, 7, y, 1, 3, x, 1, 0, z, 4, 0, y, 4, 6, y, 2, 0, x, 2, 3, z, 2, 6 )
__MCFUNC8_1__( f057, y, 4, 6, y, 5, 7, z, 5, 1, x, 0, 1, y, 0, 2, y, 3, 1, z, 3, 7, x, 3, 2 )


// 1, 1, 0, 0, 0, 0, 1, 1
__MCFUNC8_2__( f195, z, 0, 4, z, 1, 5, y, 1, 3, y, 0, 2, z, 6, 2, z, 7, 3, y, 7, 5, y, 6, 4 )
__MCFUNC8_2__( f165, z, 2, 6, z, 0, 4, x, 0, 1, x, 2, 3, z, 7, 3, z, 5, 1, x, 5, 4, x, 7, 6 )
__MCFUNC8_2__( f060, z, 3, 7, z, 2, 6, y, 2, 0, y, 3, 1, z, 5, 1, z, 4, 0, y, 4, 6, y, 5, 7 )
__MCFUNC8_2__( f090, z, 1, 5, z, 3, 7, x, 3, 2, x, 1, 0, z, 4, 0, z, 6, 2, x, 6, 7, x, 4, 5 )
__MCFUNC8_2__( f153, x, 4, 5, x, 0, 1, y, 0, 2, y, 4, 6, x, 7, 6, x, 3, 2, y, 3, 1, y, 7, 5 )
__MCFUNC8_2__( f102, x, 2, 3, x, 6, 7, y, 6, 4, y, 2, 0, x, 1, 0, x, 5, 4, y, 5, 7, y, 1, 3 )


// 1, 0, 0, 1, 0, 1, 1, 0
__MCFUNC12__( f105, x, 0, 1, y, 0, 2, z, 0, 4, y, 3, 1, z, 3, 7, x, 3, 2, z, 5, 1, x, 5, 4, y, 5, 7, z, 6, 2, x, 6, 7, y, 6, 4 )
__MCFUNC12__( f150, y, 2, 0, x, 2, 3, z, 2, 6, x, 1, 0, z, 1, 5, y, 1, 3, z, 4, 0, y, 4, 6, x, 4, 5, z, 7, 3, y, 7, 5, x, 7, 6 )


/// @}
//  Marching Cubesグループの終わり

// mist名前空間の終わり
_MIST_END

#endif // #ifndef __INCLUDE_MARCHING_CUBES_H__
