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

#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>

// mist名前空間の始まり
_MIST_BEGIN


#define __SHOW_FACET_DEBUG_INFORMATION__	0

//! @addtogroup facet_group ポリゴンを扱うクラス
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/facet.h>
//! @endcode
//!
//!  @{

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

	/// @brief 指定した値を用いて初期化する（点の並び順は反時計回りを仮定）
	facet( const vector_type &P1, const vector_type &P2, const vector_type &P3 ) : normal( ( ( P2 - P1 ).outer( P3 - P1 ) ).unit( ) ), p1( P1 ), p2( P2 ), p3( P3 ){ }

	/// @brief 他のポリゴンオブジェクトを用いて初期化する
	template < class TT >
	facet( const facet< TT > &f ) : normal( f.normal ), p1( f.p1 ), p2( f.p2 ), p3( f.p3 ){ }


	/// @brief 他のポリゴンオブジェクトを代入する
	template < class TT >
	const facet &operator =( const facet< TT > &f )
	{
		if( &f != this )
		{
			normal = f.normal;
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
			name = f.name;
		}
		return ( *this );
	}
};


/// @brief 3角形パッチの集合から頂点集合と三角形パッチを構成するインデックス集合に変換する
//!
//! 共通して使用する頂点を全てまとめた頂点集合を作成する．
//!
//! @param[in]  facets   … 3角形パッチの集合
//! @param[in]  vertices … 頂点集合
//! @param[in]  faces    … 三角形パッチを構成するインデックス集合
//! @param[in]  eps      … 同一頂点と判定される頂点の距離
//!
//! @return 頂点集合とインデックス集合の作成に成功したかどうか
//!
template < class T, class T1, class T2 >
inline bool convert_to_vertex_face_list( const facet_list< T > &facets, std::vector< vector3< T1 > > &vertices, std::vector< vector3< T2 > > &faces, const double eps = 1.0e-6 )
{
	if( facets.empty( ) )
	{
		return( false );
	}

	typedef typename facet_list< T >::size_type size_type;
	typedef typename facet_list< T >::difference_type difference_type;
	typedef vector3< T1 > vector_type;
	typedef vector3< T2 > ivector_type;

	typedef std::pair< size_type, vector_type > value_type;
	typedef std::multimap< difference_type, value_type > map_type;
	typedef std::pair< difference_type, value_type > key_type;
	map_type table;

	faces.clear( );
	faces.reserve( facets.size( ) );

	double SCALE = 0.1 / eps;

	// データをテーブルに登録する
	for( size_type i = 0 ; i < facets.size( ) ; i++ )
	{
		const typename facet_list< T >::facet_type &f = facets[ i ];
		size_type indx[ 3 ] = { 0, 0, 0 };
		vector_type v[ 3 ] = { f.p1, f.p2, f.p3 };

		for( size_type j = 0 ; j < 3 ; j++ )
		{
			const vector_type &vec = v[ j ];
			difference_type k1 = static_cast< difference_type > ( vec.x * SCALE );
			difference_type k2 = static_cast< difference_type > ( vec.y * SCALE );
			difference_type k3 = static_cast< difference_type > ( vec.z * SCALE );
			difference_type key = k1 ^ k2 ^ k3;

			typename map_type::iterator ite = table.find( key );
			if( ite == table.end( ) )
			{
				indx[ j ] = table.size( );
				table.insert( key_type( key, value_type( indx[ j ], vec ) ) );
			}
			else
			{
				typename map_type::iterator upper = table.upper_bound( key );
				for( ; ite != upper ; ++ite )
				{
					if( ( ite->second.second - vec ).length( ) < eps )
					{
						break;
					}
				}

				if( ite == upper )
				{
					indx[ j ] = table.size( );
					table.insert( key_type( key, value_type( indx[ j ], vec ) ) );
				}
				else
				{
					indx[ j ] = ite->second.first;
				}
			}
		}

		// 頂点を反時計回りに並べる
		if( f.normal.inner( ( v[ 1 ] - v[ 0 ] ).outer( v[ 2 ] - v[ 0 ] ) ) < 0 )
		{
			size_type tmp = indx[ 1 ];
			indx[ 1 ] = indx[ 2 ];
			indx[ 2 ] = tmp;
		}

		if( indx[ 0 ] != indx[ 1 ] && indx[ 1 ] != indx[ 2 ] && indx[ 2 ] != indx[ 0 ] )
		{
			faces.push_back( ivector_type( indx[ 0 ], indx[ 1 ], indx[ 2 ] ) );
		}
	}

	vertices.clear( );
	vertices.resize( table.size( ) );
	for( typename map_type::iterator ite = table.begin( ) ; ite != table.end( ) ; ++ite )
	{
		vertices[ ite->second.first ] = ite->second.second;
	}


	return( true );
}


namespace __mc__
{
	struct __face__
	{
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;

		difference_type eid1;
		difference_type eid2;
		difference_type eid3;
		bool            flag;

		__face__( ) : eid1( 0 ), eid2( 0 ), eid3( 0 ), flag( true ){ }
	};

	template < class VERTEX >
	struct __edge__
	{
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;
		typedef VERTEX    vector_type;

		difference_type v1;
		difference_type v2;
		difference_type fid1;
		difference_type fid2;
		vector_type     v;
		double          err;

		__edge__( ) : fid1( 0 ), fid2( 0 ){ }
		__edge__( difference_type vv1, difference_type vv2 ) : v1( vv1 ), v2( vv2 ), fid1( 0 ), fid2( 0 ), err( 0.0 ) { }
		__edge__( difference_type vv1, difference_type vv2, difference_type id1 ) : v1( vv1 ), v2( vv2 ), fid1( id1 ), fid2( 0 ), err( 0.0 ) { }

		bool operator <( const __edge__ &v ) const
		{
			return( err < v.err );
		}
	};

	template < class EDGE >
	struct __edge_comp__
	{
		typedef EDGE edge_type;
		typedef typename edge_type::difference_type difference_type;

		const std::vector< edge_type > &edges;

		__edge_comp__( const std::vector< edge_type > &edge_list ) : edges( edge_list )
		{
		}

		bool operator ()( difference_type v1, difference_type v2 ) const
		{
			return( edges[ v1 ].err < edges[ v2 ].err );
		}
	};

	template< class T1, class T2 >
	inline T1 create_key( const T1 v1, const T1 v2, const T2 stride )
	{
		if( v1 < v2 )
		{
			return( v1 * stride + v2 );
		}
		else
		{
			return( v2 * stride + v1 );
		}
	}


	template< class T >
	inline T find_edge( const T V1, const T V2, const T V3, const T v1, const T v2 )
	{
		if( V1 == v1 )
		{
			if( V2 == v2 )
			{
				return( 0 );
			}
			else if( V3 == v2 )
			{
				return( 2 );
			}
		}
		else if( V2 == v1 )
		{
			if( V3 == v2 )
			{
				return( 1 );
			}
			else if( V1 == v2 )
			{
				return( 0 );
			}
		}
		else if( V3 == v1 )
		{
			if( V1 == v2 )
			{
				return( 2 );
			}
			else if( V2 == v2 )
			{
				return( 1 );
			}
		}

		return( -1 );
	}

	template < class T1, class T2, class T3 >
	inline bool convert_to_vertex_face_list( const std::vector< vector3< T1 > > &vertices, const std::vector< vector3< T2 > > &faces, std::vector< __face__ > &face_lists, std::vector< __edge__< T3 > > &edge_lists )
	{
		if( vertices.empty( ) || faces.empty( ) )
		{
			return( false );
		}

		typedef typename vector3< T1 >::size_type size_type;
		typedef typename vector3< T1 >::difference_type difference_type;
		typedef vector3< T1 >           vector_type;
		typedef vector3< T2 >           ivector_type;
		typedef __face__                face_type;
		typedef __edge__< T3 >          edge_type;

		typedef std::map< difference_type, difference_type > map_type;
		typedef typename map_type::iterator iterator;
		map_type table;

		edge_lists.clear( );
		face_lists.clear( );
		face_lists.reserve( faces.size( ) );

		// ダミーを1つだけ挿入する
		face_lists.push_back( face_type( ) );
		edge_lists.push_back( edge_type( ) );

		// 辺をテーブルに登録する
		for( size_type i = 1 ; i <= faces.size( ) ; i++ )
		{
			const ivector_type &f = faces[ i - 1 ];

			difference_type key12 = create_key( f.x, f.y, vertices.size( ) );
			difference_type key23 = create_key( f.y, f.z, vertices.size( ) );
			difference_type key31 = create_key( f.z, f.x, vertices.size( ) );

			// 辺1-2を調べる
			iterator ite = table.find( key12 );
			if( ite == table.end( ) )
			{
				// 新規に辺と面の対応情報を追加する
				table[ key12 ] = edge_lists.size( );

				// エッジを登録する
				if( f.x < f.y )
				{
					edge_lists.push_back( edge_type( f.x, f.y, i ) );
				}
				else if( f.y < f.x )
				{
					edge_lists.push_back( edge_type( f.y, f.x, i ) );
				}
				else
				{
					std::cerr << "Incorrect edge is found." << std::endl;
				}
			}
			else if( edge_lists[ ite->second ].fid2 == 0 )
			{
				edge_lists[ ite->second ].fid2 = i;
			}
			else
			{
				std::cerr << "Edge may be shared among more than three faces." << std::endl;
			}

			// 辺2-3を調べる
			ite = table.find( key23 );
			if( ite == table.end( ) )
			{
				// 新規に辺情報を追加する
				table[ key23 ] = edge_lists.size( );

				// エッジを登録する
				if( f.y < f.z )
				{
					edge_lists.push_back( edge_type( f.y, f.z, i ) );
				}
				else if( f.z < f.y )
				{
					edge_lists.push_back( edge_type( f.z, f.y, i ) );
				}
				else
				{
					std::cerr << "Incorrect edge is found." << std::endl;
				}
			}
			else if( edge_lists[ ite->second ].fid2 == 0 )
			{
				edge_lists[ ite->second ].fid2 = i;
			}
			else
			{
				std::cerr << "Edge may be shared among more than three faces." << std::endl;
			}

			// 辺2-3を調べる
			ite = table.find( key31 );
			if( ite == table.end( ) )
			{
				// 新規に辺情報を追加する
				table[ key31 ] = edge_lists.size( );

				// エッジを登録する
				if( f.z < f.x )
				{
					edge_lists.push_back( edge_type( f.z, f.x, i ) );
				}
				else if( f.x < f.z )
				{
					edge_lists.push_back( edge_type( f.x, f.z, i ) );
				}
				else
				{
					std::cerr << "Incorrect edge is found." << std::endl;
				}
			}
			else if( edge_lists[ ite->second ].fid2 == 0 )
			{
				edge_lists[ ite->second ].fid2 = i;
			}
			else
			{
				std::cerr << "Edge may be shared among more than three faces." << std::endl;
			}
		}

		// 面と辺を関連付ける
		for( size_type i = 1 ; i <= faces.size( ) ; i++ )
		{
			const ivector_type &F = faces[ i - 1 ];
			face_type f;

			difference_type key12 = create_key( F.x, F.y, vertices.size( ) );
			difference_type key23 = create_key( F.y, F.z, vertices.size( ) );
			difference_type key31 = create_key( F.z, F.x, vertices.size( ) );

			// 辺1-2を調べる
			iterator ite = table.find( key12 );
			if( ite == table.end( ) )
			{
				std::cerr << "Can't find edge 1 from edge table." << std::endl;
			}
			else
			{
				edge_type &e = edge_lists[ ite->second ];
				f.eid1 = F.x == e.v1 ? ite->second : -ite->second;
			}

			// 辺2-3を調べる
			ite = table.find( key23 );
			if( ite == table.end( ) )
			{
				std::cerr << "Can't find edge 2 from edge table." << std::endl;
			}
			else
			{
				edge_type &e = edge_lists[ ite->second ];
				f.eid2 = F.y == e.v1 ? ite->second : -ite->second;
			}

			// 辺2-3を調べる
			ite = table.find( key31 );
			if( ite == table.end( ) )
			{
				std::cerr << "Can't find edge 3 from edge table." << std::endl;
			}
			else
			{
				edge_type &e = edge_lists[ ite->second ];
				f.eid3 = F.z == e.v1 ? ite->second : -ite->second;
			}

			face_lists.push_back( f );
		}

		return( true );
	}

	template < class T >
	inline T ABS( T val )
	{
		return( val < 0 ? -val : val );
	}

	template < class T >
	void contract_edges( std::vector< __edge__< T > > &edges, std::vector< __face__ > &faces, __face__::difference_type eid0, __face__::difference_type eid1, __face__::difference_type EID, __face__::difference_type FID1, __face__::difference_type FID2 )
	{
		typedef __edge__< T > edge_type;
		typedef __face__ face_type;
		typedef __face__::difference_type difference_type;

		difference_type fid2 = 0;
		const edge_type &E  = edges[ EID ];
		edge_type &e0 = edges[ eid0 ];
		edge_type &e1 = edges[ eid1 ];

		if( e0.fid1 == FID1 )
		{
			if( e1.fid1 == FID2 )
			{
				fid2 = e1.fid2;
				e0.fid1 = e1.fid2;
			}
			else if( e1.fid2 == FID2 )
			{
				fid2 = e1.fid1;
				e0.fid1 = e1.fid1;
			}
			else
			{
				std::cerr << "Face-edge correspondence is incorrect!!" << std::endl;
			}

		}
		else if( e0.fid2 == FID1 )
		{
			if( e1.fid1 == FID2 )
			{
				fid2 = e1.fid2;
				e0.fid2 = e1.fid2;
			}
			else if( e1.fid2 == FID2 )
			{
				fid2 = e1.fid1;
				e0.fid2 = e1.fid1;
			}
			else
			{
				std::cerr << "Face-edge correspondence is incorrect!!" << std::endl;
			}
		}
		else
		{
			std::cerr << "Unknown error occured!!" << std::endl;
		}

		if( fid2 != 0 )
		{
			face_type &f2 = faces[ fid2 ];

			if( ABS( f2.eid1 ) == eid1 )
			{
				if( e0.v1 == e1.v1 || e0.v2 == e1.v2 )
				{
					f2.eid1 = f2.eid1 < 0 ? -eid0 : eid0;
				}
				else if( e0.v1 == e1.v2 || e0.v2 == e1.v1 )
				{
					f2.eid1 = f2.eid1 < 0 ? eid0 : -eid0;
				}
				else
				{
					std::cerr << "Vertex-edge map is incorrect!! " << "(" << e1.v1 << ", " << e1.v2 << ") <-> (" << E.v1 << ", " << E.v2 << ")" << std::endl;
				}
			}
			else if( ABS( f2.eid2 ) == eid1 )
			{
				if( e0.v1 == e1.v1 || e0.v2 == e1.v2 )
				{
					f2.eid2 = f2.eid2 < 0 ? -eid0 : eid0;
				}
				else if( e0.v1 == e1.v2 || e0.v2 == e1.v1 )
				{
					f2.eid2 = f2.eid2 < 0 ? eid0 : -eid0;
				}
				else
				{
					std::cerr << "Vertex-edge map is incorrect!! " << "(" << e1.v1 << ", " << e1.v2 << ") <-> (" << E.v1 << ", " << E.v2 << ")" << std::endl;
				}
			}
			else if( ABS( f2.eid3 ) == eid1 )
			{
				if( e0.v1 == e1.v1 || e0.v2 == e1.v2 )
				{
					f2.eid3 = f2.eid3 < 0 ? -eid0 : eid0;
				}
				else if( e0.v1 == e1.v2 || e0.v2 == e1.v1 )
				{
					f2.eid3 = f2.eid3 < 0 ? eid0 : -eid0;
				}
				else
				{
					std::cerr << "Vertex-edge map is incorrect!! " << "(" << e1.v1 << ", " << e1.v2 << ") <-> (" << E.v1 << ", " << E.v2 << ")" << std::endl;
				}
			}
			else
			{
				std::cerr << "Face-edge correspondence is incorrect!!" << std::endl;
			}
		}
	}

	template < class T, class Allocator >
	inline bool inv3x3( matrix< T, Allocator > &a, const double eps = 1.0e-6 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;

		double a11 = a( 0, 0 ), a12 = a( 0, 1 ), a13 = a( 0, 2 );
		double a21 = a( 1, 0 ), a22 = a( 1, 1 ), a23 = a( 1, 2 );
		double a31 = a( 2, 0 ), a32 = a( 2, 1 ), a33 = a( 2, 2 );

		// 共通で利用する係数の計算
		double _22x33_23x32_ = a22 * a33 - a23 * a32;
		double _21x32_22x31_ = a21 * a32 - a22 * a31;
		double _23x31_21x33_ = a23 * a31 - a21 * a33;

		// 行列式を計算
		double detA = a11 * _22x33_23x32_ + a13 * _21x32_22x31_ + a12 * _23x31_21x33_;

		// 逆行列が存在する場合のも逆行列を計算する
		if( std::abs( detA ) > eps )
		{
			// 各要素の値を計算
			double A11 = _22x33_23x32_;
			double A12 = a13 * a32 - a12 * a33;
			double A13 = a12 * a23 - a13 * a22;
			double A21 = _23x31_21x33_;
			double A22 = a11 * a33 - a13 * a31;
			double A23 = a13 * a21 - a11 * a23;
			double A31 = _21x32_22x31_;
			double A32 = a12 * a31 - a11 * a32;
			double A33 = a11 * a22 - a12 * a21;

			double _1_detA = 1.0 / detA;
			a( 0, 0 ) = static_cast< value_type >( A11 * _1_detA );
			a( 0, 1 ) = static_cast< value_type >( A12 * _1_detA );
			a( 0, 2 ) = static_cast< value_type >( A13 * _1_detA );
			a( 1, 0 ) = static_cast< value_type >( A21 * _1_detA );
			a( 1, 1 ) = static_cast< value_type >( A22 * _1_detA );
			a( 1, 2 ) = static_cast< value_type >( A23 * _1_detA );
			a( 2, 0 ) = static_cast< value_type >( A31 * _1_detA );
			a( 2, 1 ) = static_cast< value_type >( A32 * _1_detA );
			a( 2, 2 ) = static_cast< value_type >( A33 * _1_detA );

			return( true );
		}
		else
		{
			return( false );
		}
	}

	template < class Matrix, class Vector >
	inline double compute_vertex_error( const Matrix &Q, const Vector &v )
	{
		double e0 = Q( 0, 0 ) * v.x + Q( 0, 1 ) * v.y + Q( 0, 2 ) * v.z + Q( 0, 3 );
		double e1 = Q( 1, 0 ) * v.x + Q( 1, 1 ) * v.y + Q( 1, 2 ) * v.z + Q( 1, 3 );
		double e2 = Q( 2, 0 ) * v.x + Q( 2, 1 ) * v.y + Q( 2, 2 ) * v.z + Q( 2, 3 );
		double e3 = Q( 3, 0 ) * v.x + Q( 3, 1 ) * v.y + Q( 3, 2 ) * v.z + Q( 3, 3 );
		return( e0 * v.x + e1 * v.y + e2 * v.z + e3 );
	}

	template < class T1, class T2, class Matrix >
	inline void update_edge( const std::vector< vector3< T1 > > &vertices, const std::vector< Matrix > &Q, __edge__< T2 > &edge, bool use_optimal_vertex_placement )
	{
		typedef Matrix matrix_type;
		typedef vector3< T1 > vector_type;

		// 各頂点の誤差評価行列を求める
		matrix_type QQ = Q[ edge.v1 ] + Q[ edge.v2 ];
		matrix_type QQQ = matrix_type::_33( QQ( 0, 0 ), QQ( 0, 1 ), QQ( 0, 2 ),
											QQ( 1, 0 ), QQ( 1, 1 ), QQ( 1, 2 ),
											QQ( 2, 0 ), QQ( 2, 1 ), QQ( 2, 2 ) );


		if( use_optimal_vertex_placement && inv3x3( QQQ ) )
		{
			matrix_type V = QQQ * matrix_type::_31( QQ( 0, 3 ), QQ( 1, 3 ), QQ( 2, 3 ) );
			edge.v   = vector_type( -V[ 0 ], -V[ 1 ], -V[ 2 ] );
			edge.err = compute_vertex_error( QQ, edge.v );
		}
		else
		{
			vector_type vs = vertices[ edge.v1 ];
			vector_type ve = vertices[ edge.v2 ];
			edge.err = 1.0e300;

			for( double s = 0.0 ; s <= 1.0 ; s += 0.0625 )
			{
				vector_type v = vs + ( ve - vs ) * s;
				double err = compute_vertex_error( QQ, v );

				if( err < edge.err )
				{
					edge.v   = v;
					edge.err = err;
				}
			}
		}
	}

	template < class T1, class T2 >
	inline double compute_penalty( const std::vector< vector3< T1 > > &vertices, const std::vector< __edge__< T2 > > &edges, const __face__ &f, typename __edge__< T2 >::difference_type vid, const typename __edge__< T2 >::vector_type &v )
	{
		typedef __face__ face_type;
		typedef __edge__< T2 > edge_type;
		typedef typename edge_type::vector_type vector_type;
		typedef typename edge_type::difference_type difference_type;

		const vector_type &v1 = v;
		vector_type v2, v3;

		const edge_type &e1 = edges[ f.eid1 < 0 ? -f.eid1 : f.eid1 ];
		const edge_type &e2 = edges[ f.eid2 < 0 ? -f.eid2 : f.eid2 ];
		const edge_type &e3 = edges[ f.eid3 < 0 ? -f.eid3 : f.eid3 ];
		difference_type vid1 = f.eid1 < 0 ? e1.v2 : e1.v1;
		difference_type vid2 = f.eid2 < 0 ? e2.v2 : e2.v1;
		difference_type vid3 = f.eid3 < 0 ? e3.v2 : e3.v1;

		if( vid1 == vid )
		{
			v2 = vertices[ vid2 ];
			v3 = vertices[ vid3 ];
		}
		else if( vid2 == vid )
		{
			v2 = vertices[ vid1 ];
			v3 = vertices[ vid3 ];
		}
		else if( vid3 == vid )
		{
			v2 = vertices[ vid1 ];
			v3 = vertices[ vid2 ];
		}
		else
		{
			std::cout << "Can't find vertices from the specified face." << std::endl;
		}

		const double coeff = 6.9282032302755091741097853660235;
		double w  = 0.5 * ( ( v2 - v1 ).outer( v3 - v1 ) ).length( );
		double l1 = ( v2 - v1 ).inner( v2 - v1 );
		double l2 = ( v3 - v2 ).inner( v3 - v2 );
		double l3 = ( v1 - v3 ).inner( v1 - v3 );

		return( coeff * w / ( l1 + l2 + l3 ) );
	}

	template < class T1, class T2 >
	inline bool is_collapsed_after_contraction( const std::vector< vector3< T1 > > &vertices, const std::vector< __edge__< T2 > > &edges, const __face__ &f, typename __edge__< T2 >::difference_type vid, const typename __edge__< T2 >::vector_type &v )
	{
		typedef __face__ face_type;
		typedef __edge__< T2 > edge_type;
		typedef typename edge_type::vector_type vector_type;
		typedef typename edge_type::difference_type difference_type;

		const vector_type &v0 = vertices[ vid ];
		const vector_type &v1 = v;
		vector_type v2, v3;

		const edge_type &e1 = edges[ f.eid1 < 0 ? -f.eid1 : f.eid1 ];
		const edge_type &e2 = edges[ f.eid2 < 0 ? -f.eid2 : f.eid2 ];
		const edge_type &e3 = edges[ f.eid3 < 0 ? -f.eid3 : f.eid3 ];
		difference_type vid1 = f.eid1 < 0 ? e1.v2 : e1.v1;
		difference_type vid2 = f.eid2 < 0 ? e2.v2 : e2.v1;
		difference_type vid3 = f.eid3 < 0 ? e3.v2 : e3.v1;

		if( vid1 == vid )
		{
			v2 = vertices[ vid2 ];
			v3 = vertices[ vid3 ];
		}
		else if( vid2 == vid )
		{
			v2 = vertices[ vid1 ];
			v3 = vertices[ vid3 ];
		}
		else if( vid3 == vid )
		{
			v2 = vertices[ vid1 ];
			v3 = vertices[ vid2 ];
		}
		else
		{
			std::cout << "Can't find vertices from the specified face." << std::endl;
		}

		vector_type n1 = ( v2 - v0 ).outer( v3 - v0 );
		vector_type n2 = ( v2 - v1 ).outer( v3 - v1 );

		return( n1.inner( n2 ) < 0.0 );
	}

	template < class MULTIMAP, class T1, class T2 >
	inline void apply_penalties( const MULTIMAP &vertex_edge_map, const std::vector< vector3< T1 > > &vertices, const std::vector< __face__ > &faces, std::vector< __edge__< T2 > > &edges, typename __edge__< T2 >::difference_type EID, double threshold_for_triangle_compactness )
	{
		typedef MULTIMAP vertex_edge_map_type;
		typedef typename vertex_edge_map_type::const_iterator const_iterator;

		typedef __face__ face_type;
		typedef __edge__< T2 > edge_type;
		typedef typename edge_type::vector_type vector_type;
		typedef typename edge_type::difference_type difference_type;

		edge_type &edge = edges[ EID ];

		double penalty = 1.0e100;

		std::set< difference_type > face_map1;
		std::set< difference_type > face_map2;

		{
			const_iterator ite = vertex_edge_map.find( edge.v1 );
			if( ite != vertex_edge_map.end( ) )
			{
				const_iterator upper = vertex_edge_map.upper_bound( edge.v1 );
				for( ; ite != upper ; ++ite )
				{
					face_map1.insert( edges[ ite->second ].fid1 );
					face_map1.insert( edges[ ite->second ].fid2 );
				}
			}

			for( typename std::set< difference_type >::iterator ite = face_map1.begin( ) ; ite != face_map1.end( ) ; ++ite )
			{
				if( *ite != 0 )
				{
					double ppp = compute_penalty( vertices, edges, faces[ *ite ], edge.v1, edge.v );
					if( ppp < penalty )
					{
						penalty = ppp;
					}
				}
			}
		}

		{
			std::set< difference_type > face_map;
			const_iterator ite = vertex_edge_map.find( edge.v2 );
			if( ite != vertex_edge_map.end( ) )
			{
				const_iterator upper = vertex_edge_map.upper_bound( edge.v2 );
				for( ; ite != upper ; ++ite )
				{
					face_map2.insert( edges[ ite->second ].fid1 );
					face_map2.insert( edges[ ite->second ].fid2 );
				}
			}

			for( typename std::set< difference_type >::iterator ite = face_map2.begin( ) ; ite != face_map2.end( ) ; ++ite )
			{
				if( *ite != 0 )
				{
					double ppp = compute_penalty( vertices, edges, faces[ *ite ], edge.v2, edge.v );
					if( ppp < penalty )
					{
						penalty = ppp;
					}
				}
			}
		}

		if( penalty < threshold_for_triangle_compactness )
		{
			edge.err += 1.0 - penalty;
		}


		// 法線が入れ替わるかどうかをチェックする
		for( typename std::set< difference_type >::iterator ite = face_map1.begin( ) ; ite != face_map1.end( ) ; ++ite )
		{
			if( *ite != 0 && face_map2.find( *ite ) == face_map2.end( ) )
			{
				if( is_collapsed_after_contraction( vertices, edges, faces[ *ite ], edge.v1, edge.v ) )
				{
					edge.err += 1.0;
				}
			}
		}

		for( typename std::set< difference_type >::iterator ite = face_map2.begin( ) ; ite != face_map2.end( ) ; ++ite )
		{
			if( *ite != 0 && face_map1.find( *ite ) == face_map1.end( ) )
			{
				if( is_collapsed_after_contraction( vertices, edges, faces[ *ite ], edge.v2, edge.v ) )
				{
					edge.err += 1.0;
				}
			}
		}
	}

	template < class T >
	inline bool is_sharp_edge( const __edge__< T > &edge )
	{
		return( edge.fid1 * edge.fid2 == 0 );
	}

	template < class MULTIMAP, class T >
	inline size_t number_of_sharp_edges( const MULTIMAP &vertex_edge_map, const std::vector< __edge__< T > > &edges, typename MULTIMAP::difference_type vid )
	{
		typedef MULTIMAP vertex_edge_map_type;
		typedef typename vertex_edge_map_type::const_iterator const_iterator;
		typedef typename vertex_edge_map_type::difference_type difference_type;

		difference_type count = 0;

		const_iterator ite = vertex_edge_map.find( vid );
		if( ite != vertex_edge_map.end( ) )
		{
			const_iterator upper = vertex_edge_map.upper_bound( vid );
			for( ; ite != upper ; ++ite )
			{
				if( is_sharp_edge( edges[ ite->second ] ) )
				{
					count++;
				}
			}
		}

		return( count );
	}

	template < class T >
	inline void compute_edge_connections( const std::vector< __edge__< T > > &edges, __face__::difference_type EID, __face__::difference_type eid,
											__face__::difference_type vs, __face__::difference_type vt, __face__::difference_type &vv,
											__face__::difference_type &eid1, __face__::difference_type &eid2 )
	{
		typedef __edge__< T > edge_type;

		if( eid != EID )
		{
			const edge_type &e = edges[ eid ];
			if( e.v1 == vs && eid1 == 0 )
			{
				// vs へ接続する辺
				eid1 = eid;
				vv = e.v2;
			}
			else if( e.v2 == vs && eid1 == 0 )
			{
				// vs へ接続する辺
				eid1 = eid;
				vv = e.v1;
			}
			else if( e.v1 == vt )
			{
				// vt へ接続する辺
				eid2 = eid;
				vv = e.v2;
			}
			else if( e.v2 == vt )
			{
				// vt へ接続する辺
				eid2 = eid;
				vv = e.v1;
			}
			else
			{
				std::cerr << "Incorrect edge pairs." << std::endl;
			}
		}
	}

	template < class T >
	inline bool compute_edge_connections( const std::vector< __face__ > &faces, const std::vector< __edge__< T > > &edges, __face__::difference_type EID,
											__face__::difference_type vs, __face__::difference_type vt, __face__::difference_type &vl, __face__::difference_type &vr, __face__::difference_type eid[ 4 ] )
	{
		typedef __edge__< T >             edge_type;
		typedef __face__                  face_type;
		typedef __face__::difference_type difference_type;

		const edge_type &EDGE = edges[ EID ];

		if( is_sharp_edge( EDGE ) )
		{
			// 縁に接している辺は削除対象外
			return( false );
		}

		const face_type &f1 = faces[ EDGE.fid1 ];
		const face_type &f2 = faces[ EDGE.fid2 ];

		eid[ 0 ] = eid[ 1 ] = eid[ 2 ] = eid[ 3 ] = 0;

		const edge_type &e11 = edges[ f1.eid1 < 0 ? -f1.eid1 : f1.eid1 ];
		const edge_type &e12 = edges[ f1.eid2 < 0 ? -f1.eid2 : f1.eid2 ];
		const edge_type &e13 = edges[ f1.eid3 < 0 ? -f1.eid3 : f1.eid3 ];
		const edge_type &e21 = edges[ f2.eid1 < 0 ? -f2.eid1 : f2.eid1 ];
		const edge_type &e22 = edges[ f2.eid2 < 0 ? -f2.eid2 : f2.eid2 ];
		const edge_type &e23 = edges[ f2.eid3 < 0 ? -f2.eid3 : f2.eid3 ];

		// 面1をチェック
		compute_edge_connections( edges, EID, f1.eid1 < 0 ? -f1.eid1 : f1.eid1, vs, vt, vl, eid[ 0 ], eid[ 1 ] );
		compute_edge_connections( edges, EID, f1.eid2 < 0 ? -f1.eid2 : f1.eid2, vs, vt, vl, eid[ 0 ], eid[ 1 ] );
		compute_edge_connections( edges, EID, f1.eid3 < 0 ? -f1.eid3 : f1.eid3, vs, vt, vl, eid[ 0 ], eid[ 1 ] );

		// 面2をチェック
		compute_edge_connections( edges, EID, f2.eid1 < 0 ? -f2.eid1 : f2.eid1, vs, vt, vr, eid[ 3 ], eid[ 2 ] );
		compute_edge_connections( edges, EID, f2.eid2 < 0 ? -f2.eid2 : f2.eid2, vs, vt, vr, eid[ 3 ], eid[ 2 ] );
		compute_edge_connections( edges, EID, f2.eid3 < 0 ? -f2.eid3 : f2.eid3, vs, vt, vr, eid[ 3 ], eid[ 2 ] );

		return( eid[ 0 ] * eid[ 1 ] * eid[ 2 ] * eid[ 3 ] != 0 );
	}

	template < class MULTIMAP >
	inline void remove_edge_from_map( MULTIMAP &vertex_edge_map, typename MULTIMAP::key_type key, typename MULTIMAP::mapped_type val )
	{
		typename MULTIMAP::iterator ite = vertex_edge_map.find( key );
		if( ite != vertex_edge_map.end( ) )
		{
			typename MULTIMAP::iterator upper = vertex_edge_map.upper_bound( key );
			for( ; ite != upper ; )
			{
				if( ite->second == val )
				{
					// 削除する
					ite = vertex_edge_map.erase( ite );
				}
				else
				{
					++ite;
				}
			}
		}
	}

	template < class SET >
	inline void remove_edge_from_set( SET &edge_set, typename SET::value_type EID )
	{
		typename SET::iterator ite = edge_set.find( EID );
		if( ite != edge_set.end( ) )
		{
			edge_set.erase( ite );
		}
	}

	template < class MULTIMAP, class T >
	inline bool check_topology_change( const MULTIMAP &vertex_edge_map, const std::vector< __face__ > &faces, const std::vector< __edge__< T > > &edges, __face__::difference_type EID )
	{
		typedef MULTIMAP vertex_edge_map_type;
		typedef typename vertex_edge_map_type::const_iterator const_iterator;

		typedef __edge__< T >             edge_type;
		typedef __face__                  face_type;
		typedef __face__::difference_type difference_type;

		const edge_type &EDGE = edges[ EID ];

		if( is_sharp_edge( EDGE ) )
		{
			// 縁に接している辺は削除対象外
			return( true );
		}

		// 処理対象の辺の登録と，テーブル内からの削除等を行う
		difference_type vs = EDGE.v1;
		difference_type vt = EDGE.v2;
		difference_type vl, vr;
		difference_type eid[ 4 ];
		if( !__mc__::compute_edge_connections( faces, edges, EID, vs, vt, vl, vr, eid ) )
		{
			return( true );
		}

		if( vl == vr )
		{
			return( true );
		}

		// 指定した辺を削除した場合にトポロジーが変化するかどうかをチェックする
		if( is_sharp_edge( edges[ eid[ 0 ] ] ) && is_sharp_edge( edges[ eid[ 1 ] ] ) )
		{
			return( true );
		}
		else if( is_sharp_edge( edges[ eid[ 3 ] ] ) && is_sharp_edge( edges[ eid[ 2 ] ] ) )
		{
			return( true );
		}

		difference_type nvs = number_of_sharp_edges( vertex_edge_map, edges, vs );
		difference_type nvt = number_of_sharp_edges( vertex_edge_map, edges, vt );

		if( nvs >= 1 && nvt >= 1 )
		{
			return( true );
		}

		std::set< difference_type > vertex_list;

		{
			const_iterator ite = vertex_edge_map.find( EDGE.v1 );
			if( ite != vertex_edge_map.end( ) )
			{
				const_iterator upper = vertex_edge_map.upper_bound( EDGE.v1 );
				for( ; ite != upper ; ++ite )
				{
					if( ite->second != EID )
					{
						const edge_type &e = edges[ ite->second ];
						if( e.fid1 != EDGE.fid1 && e.fid2 != EDGE.fid2 && e.fid1 != EDGE.fid2 && e.fid2 != EDGE.fid1 )
						{
							if( e.v1 != EDGE.v1 )
							{
								vertex_list.insert( e.v1 );
							}
							else
							{
								vertex_list.insert( e.v2 );
							}
						}
					}
				}
			}
		}

		{
			const_iterator ite = vertex_edge_map.find( EDGE.v2 );
			if( ite != vertex_edge_map.end( ) )
			{
				const_iterator upper = vertex_edge_map.upper_bound( EDGE.v2 );
				for( ; ite != upper ; ++ite )
				{
					if( ite->second != EID )
					{
						const edge_type &e = edges[ ite->second ];
						if( e.fid1 != EDGE.fid1 && e.fid2 != EDGE.fid2 && e.fid1 != EDGE.fid2 && e.fid2 != EDGE.fid1 )
						{
							if( e.v1 != EDGE.v2 )
							{
								if( vertex_list.find( e.v1 ) != vertex_list.end( ) )
								{
									// 面の存在しないループを発見
									return( true );
								}
							}
							else
							{
								if( vertex_list.find( e.v2 ) != vertex_list.end( ) )
								{
									// 面の存在しないループを発見
									return( true );
								}
							}
						}
					}
				}
			}
		}

		return( false );
	}
}

/// @brief 3角形パッチの集合から頂点集合と三角形パッチを構成するインデックス集合に変換する
//!
//! 共通して使用する頂点を全てまとめた頂点集合を作成する．
//!
//! - 参考文献
//!   - Michael Garland and Paul S. Heckbert, "Surface Simplification Using Quadric Error Metrics," Proceedings of SIGGRAPH 97, pp.209-216, 1997
//!   - Michael Garland and Paul S. Heckbert, "Simplifying Surfaces with Color and Texture using Quadric Error Metrics," Proceedings of IEEE Visualization conference , pp.263-269, 1998
//!
//! @param[in]  facets                             … 3角形パッチの集合
//! @param[in]  number_of_facets                   … 削減後の3角形パッチ数
//! @param[in]  use_optimal_vertex_placement       … 辺削除後に最適な位置に頂点を移動させるかどうか（有効にすると場合によってはきれいなメッシュが得られない場合あり）
//! @param[in]  threshold_for_triangle_compactness … 辺削除後に発生する尖った三角形を抑制する（0～1の間を設定）
//! @param[in]  eps                                … 同一頂点と判定される頂点の距離
//!
//! @return 3角形パッチす数の削減に成功したかどうか
//!
template < class T >
inline bool surface_simplification( facet_list< T > &facets, size_t number_of_facets, bool use_optimal_vertex_placement = true, double threshold_for_triangle_compactness = 0.0, const double eps = 1.0e-3 )
{
	typedef typename facet_list< T >::facet_type   facet_type;
	typedef typename facet_type::size_type         size_type;
	typedef typename facet_type::difference_type   difference_type;
	typedef typename facet_type::vector_type       vector_type;
	typedef vector3< difference_type >             ivector_type;
	typedef __mc__::__edge__< vector_type >        edge_type;
	typedef __mc__::__face__                       face_type;
	typedef matrix< double >                       matrix_type;
	typedef std::vector< edge_type >               edge_list_type;


	std::vector< vector_type >  vertices;
	std::vector< ivector_type > face_ids;
	std::vector< face_type >    faces;
	edge_list_type              edges;

	// 頂点と面のリストに変換する
	if( !convert_to_vertex_face_list( facets, vertices, face_ids, eps ) )
	{
		return( false );
	}

	// 面情報にエッジ情報を付与する
	if( !convert_to_vertex_face_list( vertices, face_ids, faces, edges ) )
	{
		return( false );
	}
	else
	{
		face_ids.clear( );
	}

	// Contraction Error を計算するための行列集合を設定する
	std::vector< matrix_type >  Q( vertices.size( ) );
	for( size_type i = 0 ; i < Q.size( ) ; i++ )
	{
		Q[ i ].resize( 4, 4 );
	}

	// 接続する頂点の候補集合を求める
	for( size_type i = 1 ; i < faces.size( ) ; i++ )
	{
		const face_type   &f  = faces[ i ];
		const difference_type v1 = f.eid1 < 0 ? edges[ -f.eid1 ].v2 : edges[ f.eid1 ].v1;
		const difference_type v2 = f.eid2 < 0 ? edges[ -f.eid2 ].v2 : edges[ f.eid2 ].v1;
		const difference_type v3 = f.eid3 < 0 ? edges[ -f.eid3 ].v2 : edges[ f.eid3 ].v1;
		const vector_type  &p1 = vertices[ v1 ];
		const vector_type  &p2 = vertices[ v2 ];
		const vector_type  &p3 = vertices[ v3 ];
		vector_type n = ( ( p2 - p1 ).outer( p3 - p1 ) ).unit( );

		double a = n.x;
		double b = n.y;
		double c = n.z;

		vector_type p0  = ( p1 + p2 + p3 ) / 3.0;
		vector_type d12 = ( p2 - p1 ).unit( );
		vector_type d23 = ( p3 - p2 ).unit( );
		vector_type d31 = ( p1 - p3 ).unit( );
		vector_type p12 = p1 + d12 * ( p0 - p1 ).inner( d12 );
		vector_type p23 = p2 + d23 * ( p0 - p2 ).inner( d23 );
		vector_type p31 = p3 + d31 * ( p0 - p3 ).inner( d31 );

		{
			double area1 = ( ( p0 - p1 ).outer( p12 - p1 ) ).length( ) * 0.5;
			double area2 = ( ( p0 - p1 ).outer( p31 - p1 ) ).length( ) * 0.5;
			double d = -( a * p1.x + b * p1.y + c * p1.z );
			Q[ v1 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d ) * ( area1 + area2 );
		}

		{
			double area1 = ( ( p0 - p2 ).outer( p12 - p1 ) ).length( ) * 0.5;
			double area2 = ( ( p0 - p2 ).outer( p23 - p1 ) ).length( ) * 0.5;
			double d = -( a * p2.x + b * p2.y + c * p2.z );
			Q[ v2 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d ) * ( area1 + area2 );
		}

		{
			double area1 = ( ( p0 - p3 ).outer( p23 - p1 ) ).length( ) * 0.5;
			double area2 = ( ( p0 - p3 ).outer( p31 - p1 ) ).length( ) * 0.5;
			double d = -( a * p3.x + b * p3.y + c * p3.z );
			Q[ v3 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d ) * ( area1 + area2 );
		}

	}

	// 頂点とエッジのテーブルを作成する
	typedef __mc__::__edge_comp__< edge_type > edge_compare_type;
	typedef std::set< difference_type, edge_compare_type > edge_map_type;
	typedef std::multimap< size_type, difference_type > vertex_edge_map_type;
	typedef std::pair< size_type, difference_type > vertex_edge_map_pair_type;

	vertex_edge_map_type vertex_edge_map;
	edge_compare_type ecomp( edges );
	edge_map_type edge_map( ecomp );
	for( size_type i = 1 ; i < edges.size( ) ; i++ )
	{
		edge_type &e = edges[ i ];

		// 削除対象にできるかどうかを判定
		if( !__mc__::is_sharp_edge( edges[ i ] ) )
		{
			// 各頂点の誤差評価を行う
			__mc__::update_edge( vertices, Q, edges[ i ], use_optimal_vertex_placement );
			edge_map.insert( i );
		}

		vertex_edge_map.insert( vertex_edge_map_pair_type( e.v1, i ) );
		vertex_edge_map.insert( vertex_edge_map_pair_type( e.v2, i ) );
	}

	size_t num_facets = faces.size( ) - 1;

	// 頂点をコスト順に削減する
	for( ; num_facets - 2 >= number_of_facets && !edge_map.empty( ) ; num_facets -= 2 )
	{
		typename edge_map_type::iterator mite = edge_map.end( );

		for( typename edge_map_type::iterator ite = edge_map.begin( ) ; ite != edge_map.end( ) ; )
		{
			const edge_type &e = edges[ *ite ];

			if( __mc__::is_sharp_edge( e ) )
			{
				ite = edge_map.erase( ite );
			}
			else if( !__mc__::check_topology_change( vertex_edge_map, faces, edges, *ite ) )
			{
				// 削除可能な辺を発見
				mite = ite;
				break;
			}
			else
			{
				++ite;
			}
		}

		if( mite == edge_map.end( ) )
		{
			// 削除できる辺が見つからなかったので終了する
			break;
		}

		difference_type EID = *mite < 0 ? -( *mite ) : *mite;
		edge_type &EDGE = edges[ EID ];

#if defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ >= 1
		std::cout << "Contraction [" << num_facets << "] : #" << EID << " <" << EDGE.v2 << " -> " << EDGE.v1 << ">" << std::endl;
#endif

		// 辺を削除する
		edge_map.erase( mite );

		// 処理対象の辺の登録と，テーブル内からの削除等を行う
		difference_type vs = EDGE.v1;
		difference_type vt = EDGE.v2;
		difference_type vl, vr;
		difference_type eid[ 4 ];
		if( !__mc__::compute_edge_connections( faces, edges, EID, vs, vt, vl, vr, eid ) )
		{
			std::cerr << "Unknown error occured!!" << std::endl;
			break;
		}

		// 面を削除する
		face_type &f1 = faces[ EDGE.fid1 ];
		face_type &f2 = faces[ EDGE.fid2 ];
		f1.flag = false;
		f2.flag = false;

		// 頂点を移動させる
		vertices[ EDGE.v1 ] = EDGE.v;
		vertices[ EDGE.v2 ] = EDGE.v;

		// 辺の付け替えを行う
		__mc__::contract_edges( edges, faces, eid[ 0 ], eid[ 1 ], EID, EDGE.fid1, EDGE.fid1 );
		__mc__::contract_edges( edges, faces, eid[ 3 ], eid[ 2 ], EID, EDGE.fid2, EDGE.fid2 );

		// 誤差計算行列を更新する
		matrix_type QQQ = Q[ EDGE.v1 ] + Q[ EDGE.v2 ];
		Q[ EDGE.v1 ] = QQQ;
		Q[ EDGE.v2 ] = QQQ;

		std::set< difference_type > emap;
		{
			typename vertex_edge_map_type::iterator ite = vertex_edge_map.find( vs );
			if( ite != vertex_edge_map.end( ) )
			{
				typename vertex_edge_map_type::iterator upper = vertex_edge_map.upper_bound( vs );
				for( ; ite != upper ; )
				{
					if( ite->second == EID )
					{
						// 削除する
						ite = vertex_edge_map.erase( ite );
					}
					else
					{
						emap.insert( ite->second );
						++ite;
					}
				}
			}
		}
		{
			typename vertex_edge_map_type::iterator ite = vertex_edge_map.find( vt );
			if( ite != vertex_edge_map.end( ) )
			{
				std::vector< difference_type > combine_edge;
				typename vertex_edge_map_type::iterator upper = vertex_edge_map.upper_bound( vt );
				for( ; ite != upper ; )
				{
					if( ite->second == EID )
					{
						// 削除する
						ite = vertex_edge_map.erase( ite );
					}
					else
					{
						if( ite->second == eid[ 1 ] || ite->second == eid[ 2 ] )
						{
						}
						else
						{
							emap.insert( ite->second );
							combine_edge.push_back( ite->second );

							if( edges[ ite->second ].v1 == EDGE.v2 )
							{
								edges[ ite->second ].v1 = EDGE.v1;
							}
							else if( edges[ ite->second ].v2 == EDGE.v2 )
							{
								edges[ ite->second ].v2 = EDGE.v1;
							}
							else
							{
								std::cerr << "Incorrect edge is found!!" << std::endl;
							}
						}

						ite = vertex_edge_map.erase( ite );
					}
				}

				// 不要な辺をすべて削除する
				__mc__::remove_edge_from_map( vertex_edge_map, vl, eid[ 1 ] );
				__mc__::remove_edge_from_map( vertex_edge_map, vr, eid[ 2 ] );

				// 処理対象の枝からも除外する
				__mc__::remove_edge_from_set( edge_map, eid[ 1 ] );
				__mc__::remove_edge_from_set( edge_map, eid[ 2 ] );

				// 変更された辺を再登録する
				for( size_type ii = 0 ; ii < combine_edge.size( ) ; ii++ )
				{
					vertex_edge_map.insert( vertex_edge_map_pair_type( vs, combine_edge[ ii ] ) );
				}
			}
		}

		// 各頂点を共有するエッジを更新する
		// ただし，統合して領域の端に接した場合は以降の対象から除く
		for( typename std::set< difference_type >::iterator ite = emap.begin( ) ; ite != emap.end( ) ; ++ite )
		{
			__mc__::remove_edge_from_set( edge_map, *ite );

			// 縁に接しているかどうかを判定する
			if( !__mc__::is_sharp_edge( edges[ *ite ] ) )
			{
				// 各辺の評価値を更新する
				__mc__::update_edge( vertices, Q, edges[ *ite ], use_optimal_vertex_placement );
				__mc__::apply_penalties( vertex_edge_map, vertices, faces, edges, *ite, threshold_for_triangle_compactness );
				edge_map.insert( *ite );
			}
		}

#if defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ >= 2
		for( vertex_edge_map_type::iterator ite = vertex_edge_map.begin( ) ; ite != vertex_edge_map.end( ) ; ++ite )
		{
			edge_type &e = edges[ ite->second ];
			if( e.fid1 > 0 && !faces[ e.fid1 ].flag )
			{
				std::cerr << "Edge " << ite->second << " connects to disappeared face." << std::endl;
			}
			if( e.fid2 > 0 && !faces[ e.fid2 ].flag )
			{
				std::cerr << "Edge " << ite->second << " connects to disappeared face." << std::endl;
			}

			difference_type vs = e.v1;
			difference_type vt = e.v2;
			difference_type vl, vr;
			difference_type eid[ 4 ];
			if( !__mc__::compute_edge_connections( faces, edges, ite->second, vs, vt, vl, vr, eid ) )
			{
				std::cerr << "Edge " << ite->second << " is invalid." << std::endl;
			}

			if( vl == vr )
			{
				std::cerr << "Edge " << ite->second << " share duplicated faces." << std::endl;
			}
		}

		// 面の接続関係が正しく保たれているかどうかをチェックする
		for( size_type i = 1 ; i < faces.size( ) ; i++ )
		{
			const face_type &f = faces[ i ];
			if( f.flag )
			{
				const difference_type eid1 = f.eid1 < 0 ? -f.eid1 : f.eid1;
				const difference_type eid2 = f.eid2 < 0 ? -f.eid2 : f.eid2;
				const difference_type eid3 = f.eid3 < 0 ? -f.eid3 : f.eid3;

				if( eid1 != 0 && edges[ eid1 ].fid1 != i && edges[ eid1 ].fid2 != i )
				{
					std::cerr << "Face-edge relationships is broken." << std::endl;
				}
				if( eid2 != 0 && edges[ eid2 ].fid1 != i && edges[ eid2 ].fid2 != i )
				{
					std::cerr << "Face-edge relationships is broken." << std::endl;
				}
				if( eid3 != 0 && edges[ eid3 ].fid1 != i && edges[ eid3 ].fid2 != i )
				{
					std::cerr << "Face-edge relationships is broken." << std::endl;
				}

				if( eid1 * eid2 * eid3 != 0 )
				{
					const edge_type &e1 = edges[ eid1 ];
					const edge_type &e2 = edges[ eid2 ];
					const edge_type &e3 = edges[ eid3 ];
					difference_type v11 = f.eid1 < 0 ? e1.v2 : e1.v1;
					difference_type v12 = f.eid1 < 0 ? e1.v1 : e1.v2;
					difference_type v21 = f.eid2 < 0 ? e2.v2 : e2.v1;
					difference_type v22 = f.eid2 < 0 ? e2.v1 : e2.v2;
					difference_type v31 = f.eid3 < 0 ? e3.v2 : e3.v1;
					difference_type v32 = f.eid3 < 0 ? e3.v1 : e3.v2;

					if( v12 == v21 && v22 == v31 && v32 == v11 )
					{
					}
					else
					{
						std::cerr << "Face-edge connection is broken." << std::endl;
						std::cout << "(" << v11 << ", " << v12 << ") -> (" << v21 << ", " << v22 << ") -> (" << v31 << ", " << v32 << ")" << std::endl;
					}
				}
			}
		}
#endif

#if defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ == 3
		for( size_type i = 1 ; i < faces.size( ) ; i++ )
		{
			const face_type &f = faces[ i ];
			if( f.flag )
			{
				std::cout << i << ": ";
				if( f.eid1 < 0 )
				{
					std::cout << edges[ -f.eid1 ].v2 << ", " << edges[ -f.eid1 ].v1 << "[" << -f.eid1 << "] -> ";
				}
				else
				{
					std::cout << edges[ f.eid1 ].v1 << ", " << edges[ f.eid1 ].v2 << "[" << f.eid1 << "] -> ";
				}

				if( f.eid2 < 0 )
				{
					std::cout << edges[ -f.eid2 ].v2 << ", " << edges[ -f.eid2 ].v1 << "[" << -f.eid2 << "] -> ";
				}
				else
				{
					std::cout << edges[ f.eid2 ].v1 << ", " << edges[ f.eid2 ].v2 << "[" << f.eid2 << "] -> ";
				}

				if( f.eid3 < 0 )
				{
					std::cout << edges[ -f.eid3 ].v2 << ", " << edges[ -f.eid3 ].v1 << "[" << -f.eid3 << "]" << std::endl;
				}
				else
				{
					std::cout << edges[ f.eid3 ].v1 << ", " << edges[ f.eid3 ].v2  << "[" << f.eid3 << "]" << std::endl;
				}
			}
		}
		std::cout << std::endl;
#elif defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ >= 4
		for( size_type i = 1 ; i < faces.size( ) ; i++ )
		{
			const face_type &f = faces[ i ];
			if( f.flag )
			{
				printf( "%2d: ", i );

				difference_type eid1 = f.eid1 < 0 ? -f.eid1 : f.eid1;
				difference_type eid2 = f.eid2 < 0 ? -f.eid2 : f.eid2;
				difference_type eid3 = f.eid3 < 0 ? -f.eid3 : f.eid3;
				edge_type &e1 = edges[ eid1 ];
				edge_type &e2 = edges[ eid2 ];
				edge_type &e3 = edges[ eid3 ];

				if( f.eid1 < 0 )
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e1.v2, e1.v1, eid1, e1.fid1, e1.fid2 );
				}
				else
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e1.v1, e1.v2, eid1, e1.fid1, e1.fid2 );
				}

				if( f.eid2 < 0 )
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e2.v2, e2.v1, eid2, e2.fid1, e2.fid2 );
				}
				else
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e2.v1, e2.v2, eid2, e2.fid1, e2.fid2 );
				}

				if( f.eid3 < 0 )
				{
					printf( "%2d, %2d [%2d] (%2d, %2d)\n", e3.v2, e3.v1, eid3, e3.fid1, e3.fid2 );
				}
				else
				{
					printf( "%2d, %2d [%2d] (%2d, %2d)\n", e3.v1, e3.v2, eid3, e3.fid1, e3.fid2 );
				}
			}
		}
		std::cout << std::endl;
#endif
	}

	facets.clear( );

	for( size_type i = 1 ; i < faces.size( ) ; i++ )
	{
		const face_type &f = faces[ i ];
		if( f.flag )
		{
			const difference_type v1 = f.eid1 < 0 ? edges[ -f.eid1 ].v2 : edges[ f.eid1 ].v1;
			const difference_type v2 = f.eid2 < 0 ? edges[ -f.eid2 ].v2 : edges[ f.eid2 ].v1;
			const difference_type v3 = f.eid3 < 0 ? edges[ -f.eid3 ].v2 : edges[ f.eid3 ].v1;
			facets.push_back( facet_type( vertices[ v1 ], vertices[ v2 ], vertices[ v3 ] ) );
		}
	}

	return( true );
}

/// @brief 3角形パッチの集合から最大の連結領域のみを取り出し，他を全て削除する
//!
//! @param[in]  facets                             … 3角形パッチの集合
//! @param[in]  eps                                … 同一頂点と判定される頂点の距離
//!
//! @return 最大領域の抽出に成功したかどうか
//!
template < class T >
inline bool maximum_connected_region( facet_list< T > &facets, const double eps = 1.0e-3 )
{
	typedef typename facet_list< T >::facet_type   facet_type;
	typedef typename facet_type::size_type         size_type;
	typedef typename facet_type::difference_type   difference_type;
	typedef typename facet_type::vector_type       vector_type;
	typedef vector3< difference_type >             ivector_type;
	typedef __mc__::__edge__< vector_type >        edge_type;
	typedef __mc__::__face__                       face_type;
	typedef matrix< double >                       matrix_type;
	typedef std::vector< edge_type >               edge_list_type;


	std::vector< vector_type >  vertices;
	std::vector< ivector_type > faces;

	// 頂点と面のリストに変換する
	if( !convert_to_vertex_face_list( facets, vertices, faces, eps ) )
	{
		return( false );
	}


	// 頂点と面のテーブルを作成する
	typedef std::multimap< size_type, difference_type >                    vertex_face_map_type;
	typedef typename std::multimap< size_type, difference_type >::iterator iterator;
	typedef std::pair< size_type, difference_type >                        vertex_face_map_pair_type;

	typedef std::multimap< size_type, size_type > group_map_type;
	typedef std::pair< size_type, size_type > group_map_type_pair_type;

	vertex_face_map_type vertex_face_map;
	group_map_type gmap;
	std::vector< size_type > groups( faces.size( ) );
	for( size_type i = 0 ; i < faces.size( ) ; i++ )
	{
		ivector_type &f = faces[ i ];

		vertex_face_map.insert( vertex_face_map_pair_type( f.x, i ) );
		vertex_face_map.insert( vertex_face_map_pair_type( f.y, i ) );
		vertex_face_map.insert( vertex_face_map_pair_type( f.z, i ) );

		groups[ i ] = i;
		gmap.insert( group_map_type_pair_type( i, i ) );
	}

	// 各面をグルーピングしていく
	for( size_type i = 0 ; i < faces.size( ) ; i++ )
	{
		ivector_type &f = faces[ i ];
		size_type &g1 = groups[ i ];

		{
			iterator ite = vertex_face_map.find( f.x );
			if( ite != vertex_face_map.end( ) )
			{
				iterator upper = vertex_face_map.upper_bound( f.x );
				for( ; ite != upper ; ++ite )
				{
					if( ite->second != i )
					{
						size_type &g2 = groups[ ite->second ];

						if( g1 < g2 )
						{
							typename group_map_type::iterator lower = gmap.find( g2 );
							typename group_map_type::iterator upper = gmap.upper_bound( g2 );

							std::vector< size_type > gtmp;
							for( typename group_map_type::iterator gite = lower ; gite != upper ; ++gite )
							{
								gtmp.push_back( gite->second );
							}

							gmap.erase( lower, upper );

							for( size_type i = 0 ; i < gtmp.size( ) ; i++ )
							{
								groups[ gtmp[ i ] ] = g1;
								gmap.insert( group_map_type_pair_type( g1, gtmp[ i ] ) );
							}
						}
						else if( g1 > g2 )
						{
							typename group_map_type::iterator lower = gmap.find( g1 );
							typename group_map_type::iterator upper = gmap.upper_bound( g1 );

							std::vector< size_type > gtmp;
							for( typename group_map_type::iterator gite = lower ; gite != upper ; ++gite )
							{
								gtmp.push_back( gite->second );
							}

							gmap.erase( lower, upper );

							for( size_type i = 0 ; i < gtmp.size( ) ; i++ )
							{
								groups[ gtmp[ i ] ] = g2;
								gmap.insert( group_map_type_pair_type( g2, gtmp[ i ] ) );
							}
						}
					}
				}
			}
		}

		{
			iterator ite = vertex_face_map.find( f.y );
			if( ite != vertex_face_map.end( ) )
			{
				iterator upper = vertex_face_map.upper_bound( f.y );
				for( ; ite != upper ; ++ite )
				{
					if( ite->second != i )
					{
						size_type &g2 = groups[ ite->second ];

						if( g1 < g2 )
						{
							typename group_map_type::iterator lower = gmap.find( g2 );
							typename group_map_type::iterator upper = gmap.upper_bound( g2 );

							std::vector< size_type > gtmp;
							for( typename group_map_type::iterator gite = lower ; gite != upper ; ++gite )
							{
								gtmp.push_back( gite->second );
							}

							gmap.erase( lower, upper );

							for( size_type i = 0 ; i < gtmp.size( ) ; i++ )
							{
								groups[ gtmp[ i ] ] = g1;
								gmap.insert( group_map_type_pair_type( g1, gtmp[ i ] ) );
							}
						}
						else if( g1 > g2 )
						{
							typename group_map_type::iterator lower = gmap.find( g1 );
							typename group_map_type::iterator upper = gmap.upper_bound( g1 );

							std::vector< size_type > gtmp;
							for( typename group_map_type::iterator gite = lower ; gite != upper ; ++gite )
							{
								gtmp.push_back( gite->second );
							}

							gmap.erase( lower, upper );

							for( size_type i = 0 ; i < gtmp.size( ) ; i++ )
							{
								groups[ gtmp[ i ] ] = g2;
								gmap.insert( group_map_type_pair_type( g2, gtmp[ i ] ) );
							}
						}
					}
				}
			}
		}


		{
			iterator ite = vertex_face_map.find( f.z );
			if( ite != vertex_face_map.end( ) )
			{
				iterator upper = vertex_face_map.upper_bound( f.z );
				for( ; ite != upper ; ++ite )
				{
					if( ite->second != i )
					{
						size_type &g2 = groups[ ite->second ];

						if( g1 < g2 )
						{
							typename group_map_type::iterator lower = gmap.find( g2 );
							typename group_map_type::iterator upper = gmap.upper_bound( g2 );

							std::vector< size_type > gtmp;
							for( typename group_map_type::iterator gite = lower ; gite != upper ; ++gite )
							{
								gtmp.push_back( gite->second );
							}

							gmap.erase( lower, upper );

							for( size_type i = 0 ; i < gtmp.size( ) ; i++ )
							{
								groups[ gtmp[ i ] ] = g1;
								gmap.insert( group_map_type_pair_type( g1, gtmp[ i ] ) );
							}
						}
						else if( g1 > g2 )
						{
							typename group_map_type::iterator lower = gmap.find( g1 );
							typename group_map_type::iterator upper = gmap.upper_bound( g1 );

							std::vector< size_type > gtmp;
							for( typename group_map_type::iterator gite = lower ; gite != upper ; ++gite )
							{
								gtmp.push_back( gite->second );
							}

							gmap.erase( lower, upper );

							for( size_type i = 0 ; i < gtmp.size( ) ; i++ )
							{
								groups[ gtmp[ i ] ] = g2;
								gmap.insert( group_map_type_pair_type( g2, gtmp[ i ] ) );
							}
						}
					}
				}
			}
		}
	}

	size_type mgroup = 0;
	size_type mcount = 0;
	for( typename group_map_type::iterator ite = gmap.begin( ) ; ite != gmap.end( ) ; )
	{
		size_type count = gmap.count( ite->first );
		if( count > mcount )
		{
			mcount = count;
			mgroup = ite->first;
		}

		ite = gmap.upper_bound( ite->first );
		if( ite == gmap.end( ) )
		{
			break;
		}
		else
		{
			++ite;
		}
	}

	facets.clear( );

	typename group_map_type::iterator lower = gmap.find( mgroup );
	if( lower != gmap.end( ) )
	{
		typename group_map_type::iterator upper = gmap.upper_bound( mgroup );
		for( ; lower != upper ; ++lower )
		{
			ivector_type &f = faces[ lower->second ];
			facets.push_back( facet_type( vertices[ f.x ], vertices[ f.y ], vertices[ f.z ] ) );
		}
	}

	return( true );
}

/// @}
//  Marching Cubesグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FACET__
