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

/// @file mist/utility/balloon.h
//!
//! @brief キャリブレーションチャートから点を切り出す
//!
#ifndef __INCLUDE_MIST_MESH__
#define __INCLUDE_MIST_MESH__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "../vector.h"
#endif


#include <vector>
#include <cmath>


// mist名前空間の始まり
_MIST_BEGIN

template < class T, class VTX >
inline void register_vertex( std::vector< vector3< T > > &list, std::vector< vector3< VTX > > &vlist, const vector3< T > &a, const vector3< T > &b, const vector3< T > &c )
{
	typedef vector3< T > vector_type;
	typedef vector3< VTX > vertex_index;
	typedef typename std::vector< vector_type >::size_type size_type;

	const double eps = 1.0e-12;
	typename vertex_index::value_type index[ 3 ] = { 0, 0, 0 };
	vector_type vertex[ 3 ] = { a, b, c };

	for( size_type j = 0 ; j < 3 ; j++ )
	{
		const vector_type &v = vertex[ j ];
		size_type i = 0;
		for( ; i < list.size( ) ; i++ )
		{
			const vector_type &p = list[ i ];
			if( ( p - v ).length( ) < eps )
			{
				index[ j ] = i;
				break;
			}
		}

		if( i == list.size( ) )
		{
			index[ j ] = i;
			list.push_back( v );
		}
	}

	vlist.push_back( vertex_index( index[ 0 ], index[ 1 ], index[ 2 ] ) );
}


/* 三角形を４等分する */
template < class T, class VTX >
inline void divide_triangle( std::vector< vector3< T > > &list, std::vector< vector3< VTX > > &vlist, const vector3< T > v1, const vector3< T > v2, const vector3< T > v3, size_t __division_num__ )
{
	typedef vector3< T > vector_type;

	vector_type v12 = ( v1 + v2 ).unit( );
	vector_type v23 = ( v2 + v3 ).unit( );
	vector_type v31 = ( v3 + v1 ).unit( );

	if( __division_num__ == 1 )
	{
		register_vertex( list, vlist,  v1, v12, v31 );
		register_vertex( list, vlist, v12,  v2, v23 );
		register_vertex( list, vlist, v31, v23,  v3 );
		register_vertex( list, vlist, v31, v12, v23 );
	}
	else
	{
		divide_triangle( list, vlist,  v1, v12, v31, __division_num__ - 1 );
		divide_triangle( list, vlist, v12,  v2, v23, __division_num__ - 1 );
		divide_triangle( list, vlist, v31, v23,  v3, __division_num__ - 1 );
		divide_triangle( list, vlist, v31, v12, v23, __division_num__ - 1 );
	}
}

//! @addtogroup balloon_group 球面上の方向集合を取得
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/utility/balloon.h>
//! @endcode
//!
//!  @{

/// @brief 正20面体を分割して，球面上の方向集合を取得する
//! 
//! @param[in]     number_of_division … 正20面体に何回再分割を適用するか
//! 
//! @return 球面上の方向集合
//! 
inline std::vector< vector3< double > > balloon( size_t number_of_division )
{
	typedef vector3< double > vector_type;
	typedef vector3< size_t > vertex_index;
	typedef std::vector< vector_type > position_list;
	typedef std::vector< vertex_index > vertex_list;
	typedef position_list::size_type size_type;

	const double BX = 0.525731112119133606;
	const double BZ = 0.850650808352039932;

	vector_type vtx20[  12  ] = {
		vector_type( -BX,  0, BZ ), vector_type(  BX,  0,  BZ ), vector_type( -BX,   0, -BZ ), vector_type(  BX,   0, -BZ ),
		vector_type(   0, BZ, BX ), vector_type(   0, BZ, -BX ), vector_type(   0, -BZ,  BX ), vector_type(   0, -BZ, -BX ),
		vector_type(  BZ, BX,  0 ), vector_type( -BZ, BX,   0 ), vector_type(  BZ, -BX,   0 ), vector_type( -BZ, -BX,   0 )
	};

	vertex_index ivtx20[  20  ] = {
		vertex_index( 0,  1,  4 ), vertex_index( 0,  4, 9 ), vertex_index( 9, 4,  5 ), vertex_index(  4, 8, 5 ), vertex_index( 4,  1, 8 ),
		vertex_index( 8,  1, 10 ), vertex_index( 8, 10, 3 ), vertex_index( 5, 8,  3 ), vertex_index(  5, 3, 2 ), vertex_index( 2,  3, 7 ),
		vertex_index( 7,  3, 10 ), vertex_index( 7, 10, 6 ), vertex_index( 7, 6, 11 ), vertex_index( 11, 6, 0 ), vertex_index( 0,  6, 1 ),
		vertex_index( 6, 10,  1 ), vertex_index( 9, 11, 0 ), vertex_index( 9, 2, 11 ), vertex_index(  9, 5, 2 ), vertex_index( 7, 11, 2 )
	};

	position_list plist;
	vertex_list vlist, vtmp;

	for( size_type i = 0; i < 20; i++ )
	{
		register_vertex( plist, vlist, vtx20[ ivtx20[  i  ].x ], vtx20[ ivtx20[  i  ].y ], vtx20[ ivtx20[  i  ].z ] );
	}

	if( number_of_division > 0 )
	{
		vertex_list vtmp;

		for( size_type i = 0 ; i < vlist.size( ) ; i++ )
		{
			divide_triangle( plist, vtmp, plist[  vlist[  i  ].x  ], plist[  vlist[  i  ].y  ], plist[  vlist[  i  ].z  ], number_of_division );
		}
	}

	return( plist );
}


/// @brief 正20面体を分割して，球面上の方向集合を取得する
//! 
//! @param[in]     number_of_division … 正20面体に何回再分割を適用するか
//! 
//! @return 球面上の方向集合
//! 
inline std::vector< vector3< double > > half_balloon( size_t number_of_division )
{
	typedef vector3< double > vector_type;
	typedef std::vector< vector_type > position_list;
	typedef position_list::size_type size_type;

	position_list plist = balloon( number_of_division ), qlist;
	const double eps = 1.0e-12;

	qlist.reserve( plist.size( ) / 2 + 1 );

	for( size_type i = 0 ; i < plist.size( ) ; i++ )
	{
		const vector_type &p = plist[ i ];

		if( p.z > 0 )
		{
			qlist.push_back( p );
		}
		else if( p.z > -eps )
		{
			if( p.y > 0 )
			{
				qlist.push_back( p );
			}
			else if( p.y > -eps && p.x > 0 )
			{
				qlist.push_back( p );
			}
		}
	}

	return( qlist );
}


/// @}
//  メッシュ抽出グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
