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

		difference_type v1;
		difference_type v2;
		difference_type v3;
		difference_type fid1;
		difference_type fid2;
		difference_type fid3;
		bool            flag;

		__face__( ) : v1( -1 ), v2( -1 ), v3( -1 ), fid1( -1 ), fid2( -1 ), fid3( -1 ), flag( true ){ }
		__face__( difference_type vv1, difference_type vv2, difference_type vv3 ) : v1( vv1 ), v2( vv2 ), v3( vv3 ), fid1( -1 ), fid2( -1 ), fid3( -1 ), flag( true ) { }
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

		__edge__( ) : fid1( -1 ), fid2( -1 ), err( 0.0 ){ }
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
	inline T find_edge( const __face__ &f, const T v1, const T v2 )
	{
		if( f.v1 == v1 )
		{
			if( f.v2 == v2 )
			{
				return( 0 );
			}
			else if( f.v3 == v2 )
			{
				return( 2 );
			}
		}
		else if( f.v2 == v1 )
		{
			if( f.v3 == v2 )
			{
				return( 1 );
			}
			else if( f.v1 == v2 )
			{
				return( 0 );
			}
		}
		else if( f.v3 == v1 )
		{
			if( f.v1 == v2 )
			{
				return( 2 );
			}
			else if( f.v2 == v2 )
			{
				return( 1 );
			}
		}

		return( -1 );
	}

	template < class T1, class T2, class T3 >
	inline bool convert_to_vertex_face_list( const std::vector< vector3< T1 > > &vertices, const std::vector< vector3< T2 > > &faces, std::vector< __face__ > &face_lists, std::deque< __edge__< T3 > > &edge_lists )
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
		typedef std::deque< edge_type > edge_list_type;

		typedef std::map< difference_type, size_type > map_type;
		typedef map_type::iterator iterator;
		map_type table;

		face_lists.clear( );
		face_lists.reserve( faces.size( ) );

		// データをテーブルに登録する
		for( size_type i = 0 ; i < faces.size( ) ; i++ )
		{
			const ivector_type &f = faces[ i ];
			face_lists.push_back( face_type( f.x, f.y, f.z ) );

			difference_type key12 = create_key( f.x, f.y, vertices.size( ) );
			difference_type key23 = create_key( f.y, f.z, vertices.size( ) );
			difference_type key31 = create_key( f.z, f.x, vertices.size( ) );

			// 辺1-2を調べる
			iterator ite = table.find( key12 );
			if( ite == table.end( ) )
			{
				// 新規に辺情報を追加する
				table[ key12 ] = i;

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
			else
			{
				face_type &f1 = face_lists[ ite->second ];
				face_type &f2 = face_lists[ i ];

				f2.fid1 = ite->second;
				switch( find_edge( f1, f2.v1, f2.v2 ) )
				{
				case 0:	// Line 1-2
					f1.fid1 = i;
					break;

				case 1:	// Line 2-3
					f1.fid2 = i;
					break;

				case 2:	// Line 3-1
					f1.fid3 = i;
					break;

				default:
					// エラー?
					std::cerr << "Cannot find edges!!" << std::endl;
					break;
				}
			}

			// 辺2-3を調べる
			ite = table.find( key23 );
			if( ite == table.end( ) )
			{
				// 新規に辺情報を追加する
				table[ key23 ] = i;

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
			else
			{
				face_type &f1 = face_lists[ ite->second ];
				face_type &f2 = face_lists[ i ];

				f2.fid2 = ite->second;
				switch( find_edge( f1, f2.v2, f2.v3 ) )
				{
				case 0:	// Line 1-2
					f1.fid1 = i;
					break;

				case 1:	// Line 2-3
					f1.fid2 = i;
					break;

				case 2:	// Line 3-1
					f1.fid3 = i;
					break;

				default:
					// エラー?
					std::cerr << "Cannot find edges!!" << std::endl;
					break;
				}
			}

			// 辺2-3を調べる
			ite = table.find( key31 );
			if( ite == table.end( ) )
			{
				// 新規に辺情報を追加する
				table[ key31 ] = i;

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
			else
			{
				face_type &f1 = face_lists[ ite->second ];
				face_type &f2 = face_lists[ i ];

				f2.fid3 = ite->second;
				switch( find_edge( f1, f2.v3, f2.v1 ) )
				{
				case 0:	// Line 1-2
					f1.fid1 = i;
					break;

				case 1:	// Line 2-3
					f1.fid2 = i;
					break;

				case 2:	// Line 3-1
					f1.fid3 = i;
					break;

				default:
					// エラー?
					std::cerr << "Cannot find edges!!" << std::endl;
					break;
				}
			}
		}

		for( typename edge_list_type::iterator ite = edge_lists.begin( ) ; ite != edge_lists.end( ) ; ++ite )
		{
			const face_type &f1 = face_lists[ ite->fid1 ];
			switch( find_edge( f1, ite->v1, ite->v2 ) )
			{
			case 0:	// Line 1-2
				ite->fid2 = f1.fid1;
				break;

			case 1:	// Line 2-3
				ite->fid2 = f1.fid2;
				break;

			case 2:	// Line 3-1
				ite->fid2 = f1.fid3;
				break;

			default:
				// エラー?
				std::cerr << "Cannot find edges!!" << std::endl;
				break;
			}
		}

		return( true );
	}

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
	typedef std::deque< edge_type >                edge_list_type;


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
		// 使用しないデータをクリアする
		face_ids.clear( );
	}

	// Contraction Error を計算するための行列集合を設定する
	std::vector< matrix_type >  Q( vertices.size( ) );
	for( size_type i = 0 ; i < Q.size( ) ; i++ )
	{
		Q[ i ].resize( 4, 4 );
	}

	// 接続する頂点の候補集合を求める
	for( size_type i = 0 ; i < faces.size( ) ; i++ )
	{
		const face_type   &f  = faces[ i ];
		const vector_type &p1 = vertices[ f.v1 ];
		const vector_type &p2 = vertices[ f.v2 ];
		const vector_type &p3 = vertices[ f.v3 ];
		const vector_type &n  = facets[ i ].normal;

		double a = n.x;
		double b = n.y;
		double c = n.z;

		{
			double d = -( a * p1.x + b * p1.y + c * p1.z );
			Q[ f.v1 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										   a * b, b * b, b * c, b * d,
										   a * c, b * c, c * c, c * d,
										   a * d, b * d, c * d, d * d );
		}

		{
			double d = -( a * p2.x + b * p2.y + c * p2.z );
			Q[ f.v2 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										   a * b, b * b, b * c, b * d,
										   a * c, b * c, c * c, c * d,
										   a * d, b * d, c * d, d * d );
		}

		{
			double d = -( a * p3.x + b * p3.y + c * p3.z );
			Q[ f.v3 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										   a * b, b * b, b * c, b * d,
										   a * c, b * c, c * c, c * d,
										   a * d, b * d, c * d, d * d );
		}
	}

	// 辺を共有していないものは削除する
	for( typename edge_list_type::iterator ite = edges.begin( ) ; ite != edges.end( ) ; )
	{
		if( ite->fid2 < 0 )
		{
			ite = edges.erase( ite );
		}
		else
		{
			++ite;
		}
	}

	// 各頂点の誤差評価を行う
	for( typename edge_list_type::iterator ite = edges.begin( ) ; ite != edges.end( ) ; ++ite )
	{
		__mc__::update_edge( vertices, Q, *ite );
	}

	// 頂点とエッジのテーブルを作成する
	typedef std::map< difference_type, edge_list_type::iterator > edge_pointer_map_type;
	typedef std::multimap< size_type, edge_list_type::iterator > vertex_edge_map_type;
	typedef std::pair< size_type, edge_list_type::iterator > vertex_edge_map_pair_type;
	vertex_edge_map_type vertex_edge_emap;
	edge_pointer_map_type edge_pointer_map;
	for( edge_list_type::iterator ite = edges.begin( ) ; ite != edges.end( ) ; ++ite )
	{
		edge_pointer_map[ __mc__::create_key( ite->v1, ite->v2, vertices.size( ) ) ] = ite;
		vertex_edge_emap.insert( vertex_edge_map_pair_type( ite->v1, ite ) );
		vertex_edge_emap.insert( vertex_edge_map_pair_type( ite->v2, ite ) );
	}


	// 頂点をコスト順に削減する
	for( size_type loop = 0 ; loop < number_of_iterations && !edges.empty( ) ; loop++ )
	{
		typename edge_pointer_map_type::iterator mite, iite = edge_pointer_map.begin( );
		double minV = 1.0e100;
		for( ; iite != edge_pointer_map.end( ) ; ++iite )
		{
			if( minV > ( iite->second )->err )
			{
				mite = iite;
				minV = ( iite->second )->err;
			}
		}

		edge_type EDGE = *( mite->second );
		edge_pointer_map.erase( mite );

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

		// 編集対象の辺のキーを列挙する
		edge_type nedges[ 2 ];
		difference_type ekey[ 5 ];
		difference_type vids[ 4 ] = { EDGE.v1, EDGE.v2, -1, -1 };
		edge_list_type::iterator vites[ 5 ];
		ekey[ 0 ] = __mc__::create_key( EDGE.v1, EDGE.v2, vertices.size( ) );
		switch( find_edge( f1, EDGE.v1, EDGE.v2 ) )
		{
		case 0:	// Line 1-2
			ekey[ 1 ] = __mc__::create_key( f1.v1, f1.v3, vertices.size( ) );
			ekey[ 2 ] = __mc__::create_key( f1.v2, f1.v3, vertices.size( ) );
			vids[ 2 ] = f1.v3;
			nedges[ 0 ] = edge_type( EDGE.v1, f1.v3 );
			nedges[ 0 ].fid1 = f1.fid2;
			nedges[ 0 ].fid2 = f1.fid3;
			break;

		case 1:	// Line 2-3
			ekey[ 1 ] = __mc__::create_key( f1.v1, f1.v2, vertices.size( ) );
			ekey[ 2 ] = __mc__::create_key( f1.v1, f1.v3, vertices.size( ) );
			vids[ 2 ] = f1.v1;
			nedges[ 0 ] = edge_type( EDGE.v1, f1.v1 );
			nedges[ 0 ].fid1 = f1.fid1;
			nedges[ 0 ].fid2 = f1.fid3;
			break;

		case 2:	// Line 3-1
			ekey[ 1 ] = __mc__::create_key( f1.v1, f1.v2, vertices.size( ) );
			ekey[ 2 ] = __mc__::create_key( f1.v2, f1.v3, vertices.size( ) );
			vids[ 2 ] = f1.v2;
			nedges[ 0 ] = edge_type( EDGE.v1, f1.v2 );
			nedges[ 0 ].fid1 = f1.fid1;
			nedges[ 0 ].fid2 = f1.fid2;
			break;

		default:
			// エラー?
			std::cerr << "Cannot find edges!!" << std::endl;
			break;
		}

		switch( find_edge( f2, EDGE.v1, EDGE.v2 ) )
		{
		case 0:	// Line 1-2
			ekey[ 1 ] = __mc__::create_key( f2.v1, f2.v3, vertices.size( ) );
			ekey[ 2 ] = __mc__::create_key( f2.v2, f2.v3, vertices.size( ) );
			vids[ 3 ] = f2.v3;
			nedges[ 1 ] = edge_type( EDGE.v1, f2.v3 );
			nedges[ 1 ].fid1 = f2.fid2;
			nedges[ 1 ].fid2 = f2.fid3;
			break;

		case 1:	// Line 2-3
			ekey[ 1 ] = __mc__::create_key( f2.v1, f2.v2, vertices.size( ) );
			ekey[ 2 ] = __mc__::create_key( f2.v1, f2.v3, vertices.size( ) );
			vids[ 3 ] = f2.v1;
			nedges[ 1 ] = edge_type( EDGE.v1, f2.v1 );
			nedges[ 1 ].fid1 = f2.fid1;
			nedges[ 1 ].fid2 = f2.fid3;
			break;

		case 2:	// Line 3-1
			ekey[ 1 ] = __mc__::create_key( f2.v1, f2.v2, vertices.size( ) );
			ekey[ 2 ] = __mc__::create_key( f2.v2, f2.v3, vertices.size( ) );
			vids[ 3 ] = f2.v2;
			nedges[ 1 ] = edge_type( EDGE.v1, f2.v2 );
			nedges[ 1 ].fid1 = f2.fid1;
			nedges[ 1 ].fid2 = f2.fid2;
			break;

		default:
			// エラー?
			std::cerr << "Cannot find edges!!" << std::endl;
			break;
		}

		// 各頂点を共有するエッジを更新する
		for( size_type iii = 0 ; iii < 4 ; iii++ )
		{
			difference_type vid = vids[ iii ];
			vertex_edge_map_type::iterator ite = vertex_edge_emap.find( vid );
			if( ite != vertex_edge_emap.end( ) )
			{
				vertex_edge_map_type::iterator upper = vertex_edge_emap.upper_bound( vid );
				for( ; ite != upper ; )
				{
					difference_type key = __mc__::create_key( ite->second->v1, ite->second->v2, vertices.size( ) );

					size_type indx = 0;
					for( ; indx < 5 ; indx++ )
					{
						if( key == ekey[ indx ] )
						{
							// 削除対象の辺
							vites[ indx ] = ite->second;
							ite = vertex_edge_emap.erase( ite );

							typename edge_pointer_map_type::iterator epite = edge_pointer_map.find( key );
							if( epite != edge_pointer_map.end( ) )
							{
								edge_pointer_map.erase( epite );
							}

							break;
						}
					}

					if( indx >= 5 )
					{
						if( iii < 2 )
						{
							// 更新対象の辺
							__mc__::update_edge( vertices, Q, *( ite->second ) );
						}
						++ite;
					}
				}
			}
		}

		{
			// EDGE.v2 の頂点を EDGE.v1 へ移す
			vertex_edge_map_type::iterator ite = vertex_edge_emap.find( EDGE.v2 );
			if( ite != vertex_edge_emap.end( ) )
			{
				vertex_edge_map_type::iterator upper = vertex_edge_emap.upper_bound( EDGE.v2 );
				for( ; ite != upper ; )
				{
					edge_type &edge = *( ite->second );
					if( edge.fid1 >= 0 )
					{
						face_type &f = faces[ edge.fid1 ];
						if( f.v1 == EDGE.v2 )
						{
							f.v1 = EDGE.v1;
						}
						else if( f.v2 == EDGE.v2 )
						{
							f.v2 = EDGE.v1;
						}
						else if( f.v3 == EDGE.v2 )
						{
							f.v3 = EDGE.v1;
						}
					}

					if( edge.fid2 >= 0 )
					{
						face_type &f = faces[ edge.fid2 ];
						if( f.v1 == EDGE.v2 )
						{
							f.v1 = EDGE.v1;
						}
						else if( f.v2 == EDGE.v2 )
						{
							f.v2 = EDGE.v1;
						}
						else if( f.v3 == EDGE.v2 )
						{
							f.v3 = EDGE.v1;
						}
					}

					vertex_edge_emap.insert( vertex_edge_map_pair_type( EDGE.v1, ite->second ) );
					ite = vertex_edge_emap.erase( ite );
				}
			}
		}

		// 新しい辺を追加する
		for( size_type i = 0 ; i < 2 ; i++ )
		{
			edge_type &edge  = *vites[ i ];
			edge_type &nedge = nedges[ i ];

			if( nedge.fid1 < 0 || nedge.fid2 < 0 )
			{
				continue;
			}

			edge = nedge;
			__mc__::update_edge( vertices, Q, edge );

			edge_pointer_map[ __mc__::create_key( edge.v1, edge.v2, vertices.size( ) ) ] = vites[ i ];
			vertex_edge_emap.insert( vertex_edge_map_pair_type( edge.v1, vites[ i ] ) );
			vertex_edge_emap.insert( vertex_edge_map_pair_type( edge.v2, vites[ i ] ) );

			// 面の接続関係を調整する
			face_type &ff1 = faces[ edge.fid1 ];
			face_type &ff2 = faces[ edge.fid2 ];

			switch( find_edge( ff1, edge.v1, edge.v2 ) )
			{
			case 0:	// Line 1-2
				ff1.fid1 = edge.fid2;
				break;

			case 1:	// Line 2-3
				ff1.fid2 = edge.fid2;
				break;

			case 2:	// Line 3-1
				ff1.fid3 = edge.fid2;
				break;

			default:
				// エラー?
				std::cerr << "Cannot find edges!!" << std::endl;
				break;
			}

			switch( find_edge( ff2, edge.v1, edge.v2 ) )
			{
			case 0:	// Line 1-2
				ff2.fid1 = edge.fid2;
				break;

			case 1:	// Line 2-3
				ff2.fid2 = edge.fid2;
				break;

			case 2:	// Line 3-1
				ff2.fid3 = edge.fid2;
				break;

			default:
				// エラー?
				std::cerr << "Cannot find edges!!" << std::endl;
				break;
			}
		}
	}

	facets.clear( );

	for( size_type i = 0 ; i < faces.size( ) ; i++ )
	{
		const face_type &f = faces[ i ];
		if( f.flag )
		{
			facets.push_back( facet_type( vertices[ f.v1 ], vertices[ f.v2 ], vertices[ f.v3 ] ) );
		}
	}

	return( true );
}

/// @}
//  Marching Cubesグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FACET__
