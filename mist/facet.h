// 
// Copyright (c) 2003-2008, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/config/binary.h
//!
//! @brief バイナリ演算を行うためのクラス
//!

#ifndef __INCLUDE_MIST_FACET__
#define __INCLUDE_MIST_FACET__



#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <vector>

// mist名前空間の始まり
_MIST_BEGIN


/// @brief ポリゴン（三角形）を扱うクラス
//! 
//! @param T … 内部で用いるデータ型
//! 
template < class T >
class facet
{
public:
	typedef T value_type;										///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef size_t size_type;									///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;							///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef vector3< T > vector_type;							///< @brief 頂点などを表す3次元ベクトル型
	typedef typename float_type< T >::value_type float_type;	///< @brief 長さなどを計算するときに用いる浮動小数点型

public:
	vector_type normal;	///< @brief 法線
	vector_type p1;		///< @brief 座標1
	vector_type p2;		///< @brief 座標2
	vector_type p3;		///< @brief 座標3

	/// @brief デフォルトコンストラクタ（全要素をデフォルト値で初期化する）
	facet( ) : normal( 0, 0, 1 ){ }

	/// @brief 指定した値を用いて初期化する
	facet( const vector_type &N, const vector_type &P1, const vector_type &P2, const vector_type &P3 ) : normal( N ), p1( P1 ), p2( P2 ), p3( P3 ){ }

	/// @brief 他のポリゴンオブジェクトを用いて初期化する
	template < class TT >
	facet( const facet< TT > &f ) : normal( f.normal ), p1( f.p1 ), p2( f.p2 ), p3( f.p3 ){ }


	/// @brief 他のポリゴンオブジェクトを代入する
	template < class TT >
	const facet &operator =( const facet< TT > &f )
	{
		if( &f != this )
		{
			normal = q.normal;
			p1     = f.p1;
			p2     = f.p2;
			p3     = f.p3;
		}
		return ( *this );
	}
};


/// @brief 指定されたストリームに，ポリゴンオブジェクトを整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     f   … ポリゴンオブジェクト
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2, 3, 4 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const facet< T > &f )
{
	out << "(" << f.normal << ") - ";
	out << "(" << f.p1 << "), ";
	out << "(" << f.p2 << "), ";
	out << "(" << f.p3 << ")";
	return( out );
}


/// @brief ポリゴン（三角形）の集合を扱うクラス
//! 
//! @param T … 内部で用いるデータ型
//! 
template < class T >
class facet_list : public std::vector< facet< T > >
{
private:
	typedef std::vector< facet< T > > base;

public:
	typedef facet< T > facet_type;								///< @brief ポリゴンタイプ
	typedef size_t size_type;									///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;							///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename facet_type::vector_type vector_type;		///< @brief 頂点などを表す3次元ベクトル型
	typedef typename float_type< T >::value_type float_type;	///< @brief 長さなどを計算するときに用いる浮動小数点型

public:
	std::string name;	///< @brief 法線

	/// @brief デフォルトコンストラクタ（全要素をデフォルト値で初期化する）
	facet_list( ){ }

	/// @brief 指定した名前でポリゴン集合を初期化する
	facet_list( const std::string &name_ ) : name( name_ ){ }

	/// @brief 他のポリゴンオブジェクトを用いて初期化する
	template < class TT >
	facet_list( const facet_list< TT > &f ) : base( f ), name( f.name ){ }


	/// @brief 他のポリゴンオブジェクトを代入する
	template < class TT >
	const facet_list &operator =( const facet_list< TT > &f )
	{
		if( &f != this )
		{
			base::operator =( f );
			name = q.name;
		}
		return ( *this );
	}
};

template < class T, class T1, class T2 >
inline bool convert_to_index_list( const facet_list< T > &facets, std::vector< vector3< T1 > > &vertices, std::vector< vector3< T2 > > &indices )
{
	if( facets.empty( ) )
	{
		return( false );
	}

	typedef typename facet_list< T >::size_type size_type;
	typedef vector3< T1 > vector_type;
	typedef vector3< T2 > ivector_type;

	vertices.clear( );
	indices.clear( );
	vertices.reserve( facets.size( ) * 3 );
	indices.reserve( facets.size( ) );

	for( size_type i = 0 ; i < facets.size( ) ; i++ )
	{
		const typename facet_list< T >::facet_type &f = facets[ i ];

		size_type indx[ 3 ] = { 0, 0, 0 };
		vector_type v[ 3 ] = { f.p1, f.p2, f.p3 };

		for( size_type j = 0 ; j < 3 ; j++ )
		{
			if( vertices.empty( ) )
			{
				indx[ j ] = vertices.size( );
				vertices.push_back( v[ j ] );
			}
			else
			{
				size_type k = 0;
				for( ; k < vertices.size( ) ; k++ )
				{
					if( ( vertices[ k ] - v[ j ] ).length( ) < 1.0e-12 )
					{
						break;
					}
				}

				if( k < vertices.size( ) )
				{
					// 登録済みの頂点を発見
					indx[ j ] = k;
				}
				else
				{
					// 新規に頂点を登録する
					indx[ j ] = vertices.size( );
					vertices.push_back( v[ j ] );
				}
			}
		}

		indices.push_back( ivector_type( indx[ 0 ], indx[ 1 ], indx[ 2 ] ) );
	}

	return( true );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FACET__
