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
inline bool convert_to_vertex_face_list( const facet_list< T > &facets, std::vector< vector3< T1 > > &vertices, std::vector< vector3< T2 > > &faces, const double eps = 1.0e-12 )
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

	// データをテーブルに登録する
	for( size_type i = 0 ; i < facets.size( ) ; i++ )
	{
		const typename facet_list< T >::facet_type &f = facets[ i ];
		size_type indx[ 3 ] = { 0, 0, 0 };
		vector_type v[ 3 ] = { f.p1, f.p2, f.p3 };

		for( size_type j = 0 ; j < 3 ; j++ )
		{
			const vector_type &vec = v[ j ];
			difference_type k1 = static_cast< difference_type > ( vec.x * 100000.0 );
			difference_type k2 = static_cast< difference_type > ( vec.y * 100000.0 );
			difference_type k3 = static_cast< difference_type > ( vec.z * 100000.0 );
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

		faces.push_back( ivector_type( indx[ 0 ], indx[ 1 ], indx[ 2 ] ) );
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

		__edge__( ) : fid1( 0 ), fid2( 0 ), err( 0.0 ){ }
		__edge__( difference_type vv1, difference_type vv2 ) : v1( vv1 ), v2( vv2 ), fid1( -1 ), fid2( -1 ), err( 0.0 ) { }
		__edge__( difference_type vv1, difference_type vv2, difference_type id1 ) : v1( vv1 ), v2( vv2 ), fid1( id1 ), fid2( -1 ), err( 0.0 ) { }

		bool operator <( const __edge__ &v ) const
		{
			return( err < v.err );
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
		typedef map_type::iterator iterator;
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
				else
				{
					edge_lists.push_back( edge_type( f.y, f.x, i ) );
				}
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
				else
				{
					edge_lists.push_back( edge_type( f.z, f.y, i ) );
				}
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
				else
				{
					edge_lists.push_back( edge_type( f.x, f.z, i ) );
				}
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

				if( e.fid1 == 0 )
				{
					e.fid1 = i;
				}
				else
				{
					e.fid2 = i;
				}
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

				if( e.fid1 == 0 )
				{
					e.fid1 = i;
				}
				else
				{
					e.fid2 = i;
				}
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

				if( e.fid1 == 0 )
				{
					e.fid1 = i;
				}
				else
				{
					e.fid2 = i;
				}
			}

			face_lists.push_back( f );
		}

		return( true );
	}

	//template < class T >
	//bool is_same_edge_manner( const __edge__< T > &E, const __edge__< T > &e, const __edge__< T > &EDGE )
	//{
	//	//const edge_type &E = edges[ EID1 ];
	//	if( E.v1 == EDGE.v1 )
	//	{
	//		if( E.v2 == e.v1 )
	//		{
	//			f.eid1 = -EID1;
	//		}
	//		else
	//		{
	//			f.eid1 = EID1;
	//		}
	//	}
	//	else if( E.v2 == EDGE.v2 )
	//	{
	//		if( E.v1 == e.v1 )
	//		{
	//			f.eid1 = EID1;
	//		}
	//		else
	//		{
	//			f.eid1 = -EID1;
	//		}
	//	}
	//	else
	//	{
	//		std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
	//	}
	//}

	template < class T1, class T2, class Matrix >
	inline void update_edge( const std::vector< vector3< T1 > > &vertices, const std::vector< Matrix > &Q, __edge__< T2 > &edge )
	{
		typedef Matrix matrix_type;
		typedef vector3< T1 > vector_type;

		// 各頂点の誤差評価行列を求める
		matrix_type QQ = Q[ edge.v1 ] + Q[ edge.v2 ];

		vector_type v1 = vertices[ edge.v1 ];
		vector_type v2 = vertices[ edge.v2 ];
		vector_type v3 = ( v1 + v2 ) * 0.5;
		matrix_type V1 = matrix_type::_41( v1.x, v1.y, v1.z, 1.0 );
		matrix_type V2 = matrix_type::_41( v2.x, v2.y, v2.z, 1.0 );
		matrix_type V3 = matrix_type::_41( v3.x, v3.y, v3.z, 1.0 );

		double err1 = ( V1.t( ) * QQ * V1 )[ 0 ];
		double err2 = ( V2.t( ) * QQ * V2 )[ 0 ];
		double err3 = ( V3.t( ) * QQ * V3 )[ 0 ];

		if( err1 < err2 && err1 < err3 )
		{
			edge.v   = v1;
			edge.err = err1;
		}
		else if( err2 < err1 && err2 < err3 )
		{
			edge.v   = v2;
			edge.err = err2;
		}
		else
		{
			edge.v   = v3;
			edge.err = err3;
		}
	}
}

/// @brief 3角形パッチの集合から頂点集合と三角形パッチを構成するインデックス集合に変換する
//!
//! 共通して使用する頂点を全てまとめた頂点集合を作成する．
//!
//! @param[in]  facets   … 3角形パッチの集合
//! @param[in]  vertices … 頂点集合
//! @param[in]  indices  … 三角形パッチを構成するインデックス集合
//! @param[in]  eps      … 同一頂点と判定される頂点の距離
//!
//! @return 頂点集合とインデックス集合の作成に成功したかどうか
//!
template < class T >
inline bool surface_simplification( facet_list< T > &facets, const double threshold, size_t number_of_iterations )
{
	typedef typename facet_list< T >::facet_type   facet_type;
	typedef typename facet_type::size_type         size_type;
	typedef typename facet_type::difference_type   difference_type;
	typedef typename facet_type::vector_type       vector_type;
	typedef typename vector3< difference_type >    ivector_type;
	typedef __mc__::__edge__< vector_type >        edge_type;
	typedef __mc__::__face__                       face_type;
	typedef matrix< double >                       matrix_type;
	typedef std::vector< edge_type >               edge_list_type;


	std::vector< vector_type >  vertices;
	std::vector< ivector_type > face_ids;
	std::vector< face_type >    faces;
	edge_list_type              edges;

	// 頂点と面のリストに変換する
	if( !convert_to_vertex_face_list( facets, vertices, face_ids ) )
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
		const vector_type  &n  = facets[ i - 1 ].normal;

		double a = n.x;
		double b = n.y;
		double c = n.z;

		{
			double d = -( a * p1.x + b * p1.y + c * p1.z );
			Q[ v1 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d );
		}

		{
			double d = -( a * p2.x + b * p2.y + c * p2.z );
			Q[ v2 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d );
		}

		{
			double d = -( a * p3.x + b * p3.y + c * p3.z );
			Q[ v3 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d );
		}
	}

	// 頂点とエッジのテーブルを作成する
	typedef std::map< difference_type, difference_type > edge_map_type;
	typedef std::multimap< size_type, difference_type > vertex_edge_map_type;
	typedef std::pair< size_type, difference_type > vertex_edge_map_pair_type;

	vertex_edge_map_type vertex_edge_map;
	edge_map_type        edge_map;
	for( size_type i = 1 ; i < edges.size( ) ; i++ )
	{
		const edge_type &e = edges[ i ];
		difference_type key = __mc__::create_key( e.v1, e.v2, vertices.size( ) );

		edge_map[ key ] = i;
		vertex_edge_map.insert( vertex_edge_map_pair_type( e.v1, i ) );
		vertex_edge_map.insert( vertex_edge_map_pair_type( e.v2, i ) );
	}

	// 辺を共有していないものは削除する
	for( typename edge_map_type::iterator ite = edge_map.begin( ) ; ite != edge_map.end( ) ; )
	{
		if( edges[ ite->second ].fid2 == 0 )
		{
			ite = edge_map.erase( ite );
		}
		else
		{
			++ite;
		}
	}

	// 各頂点の誤差評価を行う
	for( typename edge_map_type::iterator ite = edge_map.begin( ) ; ite != edge_map.end( ) ; ++ite )
	{
		__mc__::update_edge( vertices, Q, edges[ ite->second ] );
	}

	// 頂点をコスト順に削減する
	for( size_type loop = 0 ; loop < number_of_iterations && !edge_map.empty( ) ; loop++ )
	{
		typename edge_map_type::iterator mite = edge_map.end( );
		double minV = 1.0e100;
		for( typename edge_map_type::iterator ite = edge_map.begin( ) ; ite != edge_map.end( ) ; ++ite )
		{
			const edge_type &e = edges[ ite->second ];

			if( minV > e.err )
			{
				mite = ite;
				minV = e.err;
			}
		}

		difference_type eid = mite->second;
		difference_type EID = eid < 0 ? -eid : eid;
		edge_type &EDGE = edges[ EID ];

		// 辺を削除する
		edge_map.erase( mite );

		// 面を削除する
		face_type &f1 = faces[ EDGE.fid1 ];
		face_type &f2 = faces[ EDGE.fid2 ];
		f1.flag = false;
		f2.flag = false;

		// 頂点を移動させる
		vertices[ EDGE.v1 ] = EDGE.v;
		vertices[ EDGE.v2 ] = EDGE.v;

		// 誤差計算行列を更新する
		matrix_type QQQ = Q[ EDGE.v1 ] + Q[ EDGE.v2 ];
		Q[ EDGE.v1 ] = QQQ;
		Q[ EDGE.v2 ] = QQQ;

		// 処理対象の辺の登録と，テーブル内からの削除等を行う
		std::set< difference_type > emap;
		difference_type *pFID1 = NULL, *pFID2 = NULL;
		difference_type EID1 = 0, EID2 = 0;
		{
			vertex_edge_map_type::iterator ite = vertex_edge_map.find( EDGE.v1 );
			if( ite != vertex_edge_map.end( ) )
			{
				vertex_edge_map_type::iterator upper = vertex_edge_map.upper_bound( EDGE.v1 );
				for( ; ite != upper ; )
				{
					if( ite->second == EID )
					{
						// 削除する
						ite = vertex_edge_map.erase( ite );
					}
					else
					{
						edge_type &e = edges[ ite->second ];
						if( e.fid1 == EDGE.fid1 )
						{
							pFID1 = &( e.fid1 );
							EID1  = ite->second;
						}
						else if( e.fid2 == EDGE.fid1 )
						{
							pFID1 = &( e.fid2 );
							EID1  = ite->second;
						}
						else if( e.fid1 == EDGE.fid2 )
						{
							pFID2 = &( e.fid1 );
							EID2  = ite->second;
						}
						else if( e.fid2 == EDGE.fid2 )
						{
							pFID2 = &( e.fid2 );
							EID2  = ite->second;
						}

						emap.insert( ite->second );
						++ite;
					}
				}
			}
		}
		{
			vertex_edge_map_type::iterator ite = vertex_edge_map.find( EDGE.v2 );
			if( ite != vertex_edge_map.end( ) )
			{
				std::vector< difference_type > combine_edge, remove_edge;
				vertex_edge_map_type::iterator upper = vertex_edge_map.upper_bound( EDGE.v2 );
				for( ; ite != upper ; )
				{
					if( ite->second == EID )
					{
						// 削除する
						ite = vertex_edge_map.erase( ite );
					}
					else
					{
						const edge_type &e = edges[ ite->second ];
						if( e.fid1 == EDGE.fid1 )
						{
							face_type &f = faces[ e.fid2 ];
							if( f.eid1 == ite->second || f.eid1 == -ite->second )
							{
								const edge_type &E = edges[ EID1 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid1 = f.eid1 < 0 ? -EID1 : EID1;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid1 = f.eid1 < 0 ? EID1 : -EID1;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid2 == ite->second || f.eid2 == -ite->second )
							{
								const edge_type &E = edges[ EID1 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid2 = f.eid2 < 0 ? -EID1 : EID1;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid2 = f.eid2 < 0 ? EID1 : -EID1;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid3 == ite->second || f.eid3 == -ite->second )
							{
								const edge_type &E = edges[ EID1 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid3 = f.eid3 < 0 ? -EID1 : EID1;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid3 = f.eid3 < 0 ? EID1 : -EID1;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else
							{
								std::cerr << "Incorrect edge pair is found!!" << std::endl;
							}

							*( pFID1 ) = e.fid2;
							remove_edge.push_back( ite->second );
						}
						else if( e.fid2 == EDGE.fid1 )
						{
							face_type &f = faces[ e.fid1 ];
							if( f.eid1 == ite->second || f.eid1 == -ite->second )
							{
								const edge_type &E = edges[ EID1 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid1 = f.eid1 < 0 ? -EID1 : EID1;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid1 = f.eid1 < 0 ? EID1 : -EID1;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid2 == ite->second || f.eid2 == -ite->second )
							{
								const edge_type &E = edges[ EID1 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid2 = f.eid2 < 0 ? -EID1 : EID1;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid2 = f.eid2 < 0 ? EID1 : -EID1;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid3 == ite->second || f.eid3 == -ite->second )
							{
								const edge_type &E = edges[ EID1 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid3 = f.eid3 < 0 ? -EID1 : EID1;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid3 = f.eid3 < 0 ? EID1 : -EID1;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else
							{
								std::cerr << "Incorrect edge pair is found!!" << std::endl;
							}

							*( pFID1 ) = e.fid1;
							remove_edge.push_back( ite->second );
						}
						else if( e.fid1 == EDGE.fid2 )
						{
							face_type &f = faces[ e.fid2 ];
							if( f.eid1 == ite->second || f.eid1 == -ite->second )
							{
								const edge_type &E = edges[ EID2 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid1 = f.eid1 < 0 ? -EID2 : EID2;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid1 = f.eid1 < 0 ? EID2 : -EID2;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid2 == ite->second || f.eid2 == -ite->second )
							{
								const edge_type &E = edges[ EID2 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid2 = f.eid2 < 0 ? -EID2 : EID2;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid2 = f.eid2 < 0 ? EID2 : -EID2;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid3 == ite->second || f.eid3 == -ite->second )
							{
								const edge_type &E = edges[ EID2 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid3 = f.eid3 < 0 ? -EID2 : EID2;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid3 = f.eid3 < 0 ? EID2 : -EID2;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else
							{
								std::cerr << "Incorrect edge pair is found!!" << std::endl;
							}

							*( pFID2 ) = e.fid2;
							remove_edge.push_back( ite->second );
						}
						else if( e.fid2 == EDGE.fid2 )
						{
							face_type &f = faces[ e.fid1 ];
							if( f.eid1 == ite->second || f.eid1 == -ite->second )
							{
								const edge_type &E = edges[ EID2 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid1 = f.eid1 < 0 ? -EID2 : EID2;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid1 = f.eid1 < 0 ? EID2 : -EID2;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid2 == ite->second || f.eid2 == -ite->second )
							{
								const edge_type &E = edges[ EID2 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid2 = f.eid2 < 0 ? -EID2 : EID2;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid2 = f.eid2 < 0 ? EID2 : -EID2;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else if( f.eid3 == ite->second || f.eid3 == -ite->second )
							{
								const edge_type &E = edges[ EID2 ];
								if( E.v1 == e.v1 || E.v2 == e.v2 )
								{
									f.eid3 = f.eid3 < 0 ? -EID2 : EID2;
								}
								else if( E.v1 == e.v2 || E.v2 == e.v1 )
								{
									f.eid3 = f.eid3 < 0 ? EID2 : -EID2;
								}
								else
								{
									std::cerr << "Incorrect vertex-edge map is created!!" << std::endl;
								}
							}
							else
							{
								std::cerr << "Incorrect edge pair is found!!" << std::endl;
							}

							*( pFID2 ) = e.fid1;
							remove_edge.push_back( ite->second );
						}
						else
						{
							emap.insert( ite->second );
							combine_edge.push_back( ite->second );
						}

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

						ite = vertex_edge_map.erase( ite );
					}
				}

				// 不要な辺をすべて削除する
				for( size_type ii = 0 ; ii < remove_edge.size( ) ; ii++ )
				{
					const edge_type &e = edges[ remove_edge[ ii ] ];
					difference_type key = e.v1 == EDGE.v1 ? e.v2 : e.v1;
					vertex_edge_map_type::iterator iite = vertex_edge_map.find( key );
					vertex_edge_map_type::iterator upper = vertex_edge_map.upper_bound( key );
					for( ; iite != upper ; ++iite )
					{
						if( iite->second == remove_edge[ ii ] )
						{
							vertex_edge_map.erase( iite );
							break;
						}
					}
				}

				// 変更された辺を再登録する
				for( size_type ii = 0 ; ii < combine_edge.size( ) ; ii++ )
				{
					vertex_edge_map.insert( vertex_edge_map_pair_type( EDGE.v1, combine_edge[ ii ] ) );
				}
			}
		}

		// 各頂点を共有するエッジを更新する
		for( std::set< difference_type >::iterator ite = emap.begin( ) ; ite != emap.end( ) ; ++ite )
		{
			__mc__::update_edge( vertices, Q, edges[ *ite ] );
		}

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

			std::cout << i << ": ";
			if( f.eid1 < 0 )
			{
				std::cout << edges[ -f.eid1 ].v2 << ", " << edges[ -f.eid1 ].v1 << " -> ";
			}
			else
			{
				std::cout << edges[ f.eid1 ].v1 << ", " << edges[ f.eid1 ].v2 << " -> ";
			}

			if( f.eid2 < 0 )
			{
				std::cout << edges[ -f.eid2 ].v2 << ", " << edges[ -f.eid2 ].v1 << " -> ";
			}
			else
			{
				std::cout << edges[ f.eid2 ].v1 << ", " << edges[ f.eid2 ].v2 << " -> ";
			}

			if( f.eid3 < 0 )
			{
				std::cout << edges[ -f.eid3 ].v2 << ", " << edges[ -f.eid3 ].v1 << std::endl;
			}
			else
			{
				std::cout << edges[ f.eid3 ].v1 << ", " << edges[ f.eid3 ].v2 << std::endl;
			}
		}
	}

	return( true );
}

/// @}
//  Marching Cubesグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FACET__
