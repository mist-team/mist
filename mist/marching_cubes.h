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
	class node
	{
	public:
		typedef V	value_type;
		typedef P	point_type;

		const value_type						&v( )  const { return v_; }
		const point_type						&p( )  const { return p_; }
		const point_type						&n( )  const { return n_; }
		const typename point_type::value_type	&px( ) const { return p_.x; }
		const typename point_type::value_type	&py( ) const { return p_.y; }
		const typename point_type::value_type	&pz( ) const { return p_.z; }
		const typename point_type::value_type	&nx( ) const { return n_.x; }
		const typename point_type::value_type	&ny( ) const { return n_.y; }
		const typename point_type::value_type	&nz( ) const { return n_.z; }

		value_type							&v( )  { return v_; }
		point_type							&p( )  { return p_; }
		point_type							&n( )  { return n_; }
		typename point_type::value_type		&px( ) { return p_.x; }
		typename point_type::value_type		&py( ) { return p_.y; }
		typename point_type::value_type		&pz( ) { return p_.z; }
		typename point_type::value_type	&nx( ) { return n_.x; }
		typename point_type::value_type	&ny( ) { return n_.y; }
		typename point_type::value_type	&nz( ) { return n_.z; }

		node( ) : v_( ), p_( ), n_( ) { }
		node( const value_type &v, const point_type &p, const point_type &n ) : v_( v ), p_( p ), n_( n ) { }

	private:
		value_type	v_;
		point_type	p_;
		point_type	n_;
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

	typedef std::vector< vector_type > vector_list_type;		///< @brief 3次元ベクトルリストを扱う型
	typedef __mc__::node< value_type, ivector_type > node_type;	///< @brief cube の頂点情報を保持する型
	typedef void ( * func_type )( const array< node_type > &, vector_list_type &, vector_list_type &,
							std::vector< size_type > &, float_type, const vector_type &, const vector_type & );	///< @brief cube 単位での等値面生成関数の型
	
	typedef tagged_section< value_type, size_type >	section_type;	///< @brief Interval-tree 用いる区間の型
	typedef typename section_type::min_max_type		min_max_type;	///< @brief 区間の最大値，最小値の型
	typedef typename section_type::tag_type			tag_type;		///< @brief 区間のタグ情報の型


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

	/// @brief 等値面生成（前処理有り）
	//!
	//! @param[in]  va   … ボリュームデータ
	//! @param[out] pv   … 等値面ポリゴンの頂点座標の集合
	//! @param[out] nv   … 等値面ポリゴンの頂点の法線ベクトルの集合
	//! @param[out] sv   … 等値面ポリゴンのサイズの集合
	//!
	void isosurfacing_with_preprocess( const image_type &va, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv )
	{
		pv.resize( 0 );
		nv.resize( 0 );
		sv.resize( 0 );
		it_.find( th_, active_cube_tags_ );
		std::sort( active_cube_tags_.begin( ), active_cube_tags_.end( ) );

		array< node_type > nda( 8 );
		const size_type begin = 0;
		const size_type end = active_cube_tags_.size( );
		for( size_type i = begin ; i < end ; i ++ )
		{
			construct_cube( va, active_cube_tags_[ i ], nda );
			isosurfacing_in_cube( nda, pv, nv, sv );
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
		pv.resize( 0 );
		nv.resize( 0 );
		sv.resize( 0 );
		array< node_type > nda( 8 );
		for( size_t k = 0 ; k < va.depth( ) - 1 ; k ++ )
		{
			for( size_t j = 0 ; j < va.height( ) - 1 ; j ++ )
			{
				for( size_t i = 0 ; i < va.width( ) - 1 ; i ++ )
				{
					construct_cube_without_preprocessing( va, i, j, k, nda );
					isosurfacing_in_cube( nda, pv, nv, sv );
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
		is_preprocessed_ = ( construct_pointer_difference_array( va.width( ), va.height( ) ) && construct_point_array( va ) && construct_normal_array( va ) && construct_interval_tree( va ) );
		if( !is_preprocessed_ )
		{
			std::cout << "can't construct preprocessing data for lack of memory, use non-preprocessing version of marching cubes." << std::endl;
			de_preprocess( );
		}
		return is_preprocessed_;
	}

	/// @brief 前処理で構築されたデータを破棄
	void de_preprocess( )
	{
		destruct_pointer_difference_array( );
		destruct_point_array( );
		destruct_normal_array( );
		destruct_interval_tree( );
		is_preprocessed_ = false;
	}

	/// @brief 前処理されたデータの有無を返す
	//!
	//! @return    … 前処理が終了していれば真を返す
	//!
	const bool &is_preprocessed( ) const { return is_preprocessed_; }

	/// @brief 閾値パラメータを設定
	//!
	//! @param[in]  th   … 閾値
	//!
	void threshold( float_type th ) { th_ = th; }
	
	/// @brief オフセットパラメータを設定
	//!
	//! @param[in]  ox   … x 方向オフセット
	//! @param[in]  oy   … y 方向オフセット
	//! @param[in]  oz   … z 方向オフセット
	//!
	void offset( float_type ox, float_type oy, float_type oz ) { o_.x = ox; o_.y = oy; o_.z = oz; }
	
	/// @brief スケールパラメータを設定
	//!
	//! @param[in]  sx   … x 方向スケール
	//! @param[in]  sy   … y 方向スケール
	//! @param[in]  sz   … z 方向スケール
	//!
	void scale( float_type sx, float_type sy, float_type sz ) { s_.x = sx; s_.y = sy; s_.z = sz; }

	/// @brief 設定されている閾値パラメータを取得
	//!
	//! @return    … 閾値
	//!
	const float_type	&th( ) const { return th_; }
	
	/// @brief 設定されている x 方向オフセットパラメータを取得
	//!
	//! @return    … x 方向オフセット
	//!
	const float_type	&ox( ) const { return o_.x( ); }
	
	/// @brief 設定されている y 方向オフセットパラメータを取得
	//!
	//! @return    … y 方向オフセット
	//!
	const float_type	&oy( ) const { return o_.y( ); }
	
	/// @brief 設定されている z 方向オフセットパラメータを取得
	//!
	//! @return    … z 方向オフセット
	//!
	const float_type	&oz( ) const { return o_.z( ); }
	
	/// @brief 設定されている x 方向スケールパラメータを取得
	//!
	//! @return    … x 方向スケール
	//!
	const float_type	&sx( ) const { return s_.x( ); }
	
	/// @brief 設定されている y 方向スケールパラメータを取得
	//!
	//! @return    … y 方向スケール
	//!
	const float_type	&sy( ) const { return s_.y( ); }
	
	/// @brief 設定されている z 方向スケールパラメータを取得
	//!
	//! @return    … z 方向スケール
	//!
	const float_type	&sz( ) const { return s_.z( ); }

	/// @brief コンストラクタ
	marching_cubes( ) : is_preprocessed_( false ), th_( 0 ), o_( 0, 0, 0 ), s_( 1, 1, 1 )
	{
		init_function_array( ); 
	}

	/// @brief デストラクタ
	~marching_cubes( )
	{
	}

private:
	func_type									fa_[ 256 ];
	bool										is_preprocessed_;
	array< size_t >								pda_;
	array3< ivector_type >						pa_;
	array3< ivector_type >						na_;
	interval_tree< section_type, float_type >	it_;
	std::vector< tag_type >						active_cube_tags_;

	float_type		th_;
	vector_type		o_;
	vector_type		s_;


	void construct_cube( const image_type &va, const size_t i, array< node_type > &nda ) const
	{
		nda[ 0 ].v( ) = va[ i + pda_[ 0 ] ]; nda[ 0 ].p( ) = pa_[ i + pda_[ 0 ] ]; nda[ 0 ].n( ) = na_[ i + pda_[ 0 ] ];
		nda[ 1 ].v( ) = va[ i + pda_[ 1 ] ]; nda[ 1 ].p( ) = pa_[ i + pda_[ 1 ] ]; nda[ 1 ].n( ) = na_[ i + pda_[ 1 ] ];
		nda[ 2 ].v( ) = va[ i + pda_[ 2 ] ]; nda[ 2 ].p( ) = pa_[ i + pda_[ 2 ] ]; nda[ 2 ].n( ) = na_[ i + pda_[ 2 ] ];
		nda[ 3 ].v( ) = va[ i + pda_[ 3 ] ]; nda[ 3 ].p( ) = pa_[ i + pda_[ 3 ] ]; nda[ 3 ].n( ) = na_[ i + pda_[ 3 ] ];
		nda[ 4 ].v( ) = va[ i + pda_[ 4 ] ]; nda[ 4 ].p( ) = pa_[ i + pda_[ 4 ] ]; nda[ 4 ].n( ) = na_[ i + pda_[ 4 ] ];
		nda[ 5 ].v( ) = va[ i + pda_[ 5 ] ]; nda[ 5 ].p( ) = pa_[ i + pda_[ 5 ] ]; nda[ 5 ].n( ) = na_[ i + pda_[ 5 ] ];
		nda[ 6 ].v( ) = va[ i + pda_[ 6 ] ]; nda[ 6 ].p( ) = pa_[ i + pda_[ 6 ] ]; nda[ 6 ].n( ) = na_[ i + pda_[ 6 ] ];
		nda[ 7 ].v( ) = va[ i + pda_[ 7 ] ]; nda[ 7 ].p( ) = pa_[ i + pda_[ 7 ] ]; nda[ 7 ].n( ) = na_[ i + pda_[ 7 ] ];
	}

	void construct_cube_without_preprocessing( const image_type &va, const size_t i, const size_t j, const size_t k, array< node_type > &nda ) const
	{
		nda[ 0 ].v( ) = va( i,     j,     k     ); nda[ 0 ].p( ) =  _point( i,     j,     k     ); nda[ 0 ].n( ) = _normal( va, i,     j,     k     );
		nda[ 1 ].v( ) = va( i + 1, j,     k     ); nda[ 1 ].p( ) =  _point( i + 1, j,     k     ); nda[ 1 ].n( ) = _normal( va, i + 1, j,     k     );
		nda[ 2 ].v( ) = va( i,     j + 1, k     ); nda[ 2 ].p( ) =  _point( i,     j + 1, k     ); nda[ 2 ].n( ) = _normal( va, i,     j + 1, k     );
		nda[ 3 ].v( ) = va( i + 1, j + 1, k     ); nda[ 3 ].p( ) =  _point( i + 1, j + 1, k     ); nda[ 3 ].n( ) = _normal( va, i + 1, j + 1, k     );
		nda[ 4 ].v( ) = va( i,     j,     k + 1 ); nda[ 4 ].p( ) =  _point( i,     j,     k + 1 ); nda[ 4 ].n( ) = _normal( va, i,     j,     k + 1 );
		nda[ 5 ].v( ) = va( i + 1, j,     k + 1 ); nda[ 5 ].p( ) =  _point( i + 1, j,     k + 1 ); nda[ 5 ].n( ) = _normal( va, i + 1, j,     k + 1 );
		nda[ 6 ].v( ) = va( i,     j + 1, k + 1 ); nda[ 6 ].p( ) =  _point( i,     j + 1, k + 1 ); nda[ 6 ].n( ) = _normal( va, i,     j + 1, k + 1 );
		nda[ 7 ].v( ) = va( i + 1, j + 1, k + 1 ); nda[ 7 ].p( ) =  _point( i + 1, j + 1, k + 1 ); nda[ 7 ].n( ) = _normal( va, i + 1, j + 1, k + 1 );
	}

	void isosurfacing_in_cube( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv,std::vector< size_type > &sv )
	{
		fa_[ pattern( nda ) ]( nda, pv, nv, sv, th_, o_, s_ );
	}

	ivector_type _point( const size_type i, const size_type j, const size_type k ) const
	{
		return( ivector_type( static_cast< typename ivector_type::value_type >( i ), static_cast< typename ivector_type::value_type >( j ), static_cast< typename ivector_type::value_type >( k ) ) );
	}

	ivector_type _normal( const image_type &va, const size_type i, const size_type j, const size_type k ) const
	{
		double v1 = _value( va, i - 1, j, k ) - _value( va, i + 1, j, k );
		double v2 = _value( va, i, j - 1, k ) - _value( va, i, j + 1, k );
		double v3 = _value( va, i, j, k - 1 ) - _value( va, i, j, k + 1 );
		vector_type n = vector_type( v1, v2, v3 ).unit( );
		return ivector_type( static_cast< typename ivector_type::value_type >( n.x * 32767 ), static_cast< typename ivector_type::value_type >( n.y * 32767 ), static_cast< typename ivector_type::value_type >( n.z * 32767 ) );
	}

	value_type _value( const image_type &va, const size_t i , const size_t j, const size_t k ) const
	{
		return ( i < va.width( ) && j < va.height( ) && k < va.depth( ) ) ? va( i, j, k ) : value_type( );
	}

	bool construct_pointer_difference_array( const size_type w, const size_type h )
	{
		bool ret = pda_.resize( 8 );
		if( ret )
		{
			pda_[ 0 ] = 0;
			pda_[ 1 ] = 1;
			pda_[ 2 ] = w;
			pda_[ 3 ] = w + 1;
			pda_[ 4 ] = w * h;
			pda_[ 5 ] = w * h + 1;
			pda_[ 6 ] = w * h + w;
			pda_[ 7 ] = w * h + w + 1;
		}
		return ret;
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
		return ret;
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
		return ret;
	}

	bool construct_interval_tree( const image_type &va )
	{
		std::vector< section_type >	secs;
		min_max_type				min, max;
		tag_type					tag;
		for( size_t k = 0 ; k < va.depth( ) - 1 ; k ++ )
		{
			for( size_t j = 0 ; j < va.height( ) - 1 ; j ++ )
			{
				for( size_t i = 0 ; i < va.width( ) - 1 ; i ++ )
				{
					tag = i + va.width( ) * j + va.width( ) * va.height( ) * k;
					get_section( va, tag, min, max );
					if( min != max )
					{
						secs.push_back( section_type( min, max, tag ) );
					}
				}
			}
		}
		active_cube_tags_.clear( );
		active_cube_tags_.reserve( secs.size( ) );
		return ( it_.construct( secs ) && active_cube_tags_.capacity( ) >= secs.size( ) );
	}

	void destruct_pointer_difference_array( )
	{
		pda_.clear( );
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

	void get_section( const image_type &va, const tag_type &tag, min_max_type &min, min_max_type &max ) const
	{
		min = va[ tag + pda_[ 0 ] ];
		max = va[ tag + pda_[ 0 ] ];
		for( size_t i = 1 ; i < 8 ; i ++ )
		{
			if( va[ tag + pda_[ i ] ] < min )
			{
				min = va[ tag + pda_[ i ] ];
			}
			if( va[ tag + pda_[ i ] ] > max )
			{
				max = va[ tag + pda_[ i ] ];
			}
		}
	}

	size_t pattern( const array< node_type > &nda ) const
	{
		return ( ( nda[ 0 ].v( ) >= th_ ) ? 1 : 0 ) + ( ( nda[ 1 ].v( ) >= th_ ) ? 2 : 0 ) + ( ( nda[ 2 ].v( ) >= th_ ) ? 4 : 0 ) + ( ( nda[ 3 ].v( ) >= th_ ) ? 8 : 0 ) + ( ( nda[ 4 ].v( ) >= th_ ) ? 16 : 0 ) + ( ( nda[ 5 ].v( ) >= th_ ) ? 32 : 0 ) + ( ( nda[ 6 ].v( ) >= th_ ) ? 64 : 0 ) + ( ( nda[ 7 ].v( ) >= th_ ) ? 128 : 0 );
	}

	static void interpolation_about_x( const node_type &nd0, const node_type &nd1, vector_type &p, vector_type &n, float_type th, const vector_type &o, const vector_type &s )
	{
		const double tp = ( nd1.v( ) - th ) / ( static_cast< double >( nd1.v( ) ) - nd0.v( ) ); 
		p.x = static_cast< typename vector_type::value_type >( nd0.px( ) * tp + nd1.px( ) * ( 1.0 - tp ) );
		p.y = static_cast< typename vector_type::value_type >( nd0.py( ) );
		p.z = static_cast< typename vector_type::value_type >( nd0.pz( ) );

		const double tn = ( static_cast< double >( nd1.px( ) ) - p.x ) / ( nd1.px( ) - nd0.px( ) );
		n = vector_type( nd0.nx( ) * tn + nd1.nx( ) * ( 1.0 - tn ), nd0.ny( ) * tn + nd1.ny( ) * ( 1.0 - tn ), nd0.nz( ) * tn + nd1.nz( ) * ( 1.0 - tn ) ).unit( );
		p.x = static_cast< typename vector_type::value_type >( ( p.x + o.x ) * s.x );
		p.y = static_cast< typename vector_type::value_type >( ( p.y + o.y ) * s.y );
		p.z = static_cast< typename vector_type::value_type >( ( p.z + o.z ) * s.z );
	}

	static void interpolation_about_y( const node_type &nd0, const node_type &nd1, vector_type &p, vector_type &n, float_type th, const vector_type &o, const vector_type &s )
	{
		const double tp = ( nd1.v( ) - th ) / ( static_cast< double >( nd1.v( ) ) - nd0.v( ) ); 
		p.x = static_cast< typename vector_type::value_type >( nd0.px( ) );
		p.y = static_cast< typename vector_type::value_type >( nd0.py( ) * tp + nd1.py( ) * ( 1.0 - tp ) );
		p.z = static_cast< typename vector_type::value_type >( nd0.pz( ) );
		const double tn = ( static_cast< double >( nd1.py( ) ) - p.y ) / ( nd1.py( ) - nd0.py( ) );
		n = vector_type( nd0.nx( ) * tn + nd1.nx( ) * ( 1.0 - tn ), nd0.ny( ) * tn + nd1.ny( ) * ( 1.0 - tn ), nd0.nz( ) * tn + nd1.nz( ) * ( 1.0 - tn ) ).unit( );
		p.x = static_cast< typename vector_type::value_type >( ( p.x + o.x ) * s.x );
		p.y = static_cast< typename vector_type::value_type >( ( p.y + o.y ) * s.y );
		p.z = static_cast< typename vector_type::value_type >( ( p.z + o.z ) * s.z );
	}

	static void interpolation_about_z( const node_type &nd0, const node_type &nd1, vector_type &p, vector_type &n, float_type th, const vector_type &o, const vector_type &s )
	{
		const double tp = ( nd1.v( ) - th ) / ( static_cast< double >( nd1.v( ) ) - nd0.v( ) ); 
		p.x = static_cast< typename vector_type::value_type >( nd0.px( ) );
		p.y = static_cast< typename vector_type::value_type >( nd0.py( ) );
		p.z = static_cast< typename vector_type::value_type >( nd0.pz( ) * tp + nd1.pz( ) * ( 1.0 - tp ) );
		const double tn = ( static_cast< double >( nd1.pz( ) ) - p.z ) / ( nd1.pz( ) - nd0.pz( ) );
		n = vector_type( nd0.nx( ) * tn + nd1.nx( ) * ( 1.0 - tn ), nd0.ny( ) * tn + nd1.ny( ) * ( 1.0 - tn ), nd0.nz( ) * tn + nd1.nz( ) * ( 1.0 - tn ) ).unit( );
		p.x = static_cast< typename vector_type::value_type >( ( p.x + o.x ) * s.x );
		p.y = static_cast< typename vector_type::value_type >( ( p.y + o.y ) * s.y );
		p.z = static_cast< typename vector_type::value_type >( ( p.z + o.z ) * s.z );
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

	static void f000( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f001( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f002( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f003( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f004( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f005( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f006( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f007( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f008( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f009( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f010( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f011( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f012( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f013( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f014( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f015( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f016( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f017( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f018( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f019( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f020( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f021( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f022( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f023( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f024( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f025( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f026( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f027( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f028( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f029( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f030( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f031( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f032( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f033( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f034( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f035( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f036( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f037( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f038( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f039( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f040( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f041( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f042( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f043( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f044( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f045( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f046( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f047( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f048( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f049( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f050( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f051( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f052( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f053( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f054( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f055( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f056( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f057( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f058( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f059( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f060( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f061( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f062( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f063( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f064( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f065( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f066( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f067( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f068( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f069( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f070( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f071( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f072( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f073( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f074( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f075( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f076( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f077( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f078( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f079( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f080( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f081( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f082( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f083( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f084( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f085( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f086( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f087( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f088( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f089( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f090( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f091( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f092( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f093( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f094( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f095( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f096( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f097( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f098( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f099( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f100( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f101( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f102( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f103( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f104( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f105( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f106( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f107( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f108( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f109( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f110( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f111( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f112( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f113( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f114( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f115( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f116( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f117( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f118( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f119( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f120( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f121( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f122( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f123( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f124( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f125( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f126( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f127( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f128( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f129( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f130( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f131( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f132( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f133( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f134( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f135( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f136( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f137( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f138( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f139( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f140( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f141( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f142( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f143( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f144( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f145( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f146( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f147( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f148( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f149( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f150( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f151( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f152( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f153( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f154( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f155( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f156( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f157( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f158( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f159( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f160( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f161( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f162( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f163( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f164( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f165( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f166( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f167( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f168( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f169( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f170( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f171( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f172( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f173( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f174( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f175( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f176( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f177( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f178( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f179( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f180( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f181( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f182( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f183( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f184( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f185( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f186( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f187( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f188( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f189( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f190( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f191( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f192( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f193( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f194( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f195( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f196( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f197( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f198( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f199( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f200( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f201( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f202( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f203( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f204( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f205( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f206( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f207( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f208( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f209( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f210( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f211( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f212( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f213( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f214( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f215( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f216( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f217( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f218( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f219( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f220( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f221( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f222( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f223( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f224( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f225( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f226( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f227( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f228( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f229( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f230( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f231( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f232( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f233( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f234( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f235( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f236( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f237( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f238( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f239( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f240( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f241( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f242( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f243( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f244( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f245( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f246( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f247( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f248( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f249( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f250( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f251( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f252( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f253( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f254( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
	static void f255( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s );
};

// 0, 0, 0, 0, 0, 0, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f000( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
}

template< typename V, typename P >
void marching_cubes< V, P >::f255( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
}

// 1, 0, 0, 0, 0, 0, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f001( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f254( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f004( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f251( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f008( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f247( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f002( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f253( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f016( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f239( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f064( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f191( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f032( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f223( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f128( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f127( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

// 1, 1, 0, 0, 0, 0, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f003( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f252( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f005( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f250( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f012( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f243( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f010( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f245( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f017( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f238( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f080( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f175( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f068( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f187( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f048( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f207( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f160( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f095( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f192( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f063( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f034( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f221( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f136( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f119( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

// 1, 0, 0, 1, 0, 0, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f009( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f246( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f006( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f249( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f020( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f235( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f065( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f190( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f096( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f159( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f144( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f111( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f130( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f125( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f040( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f215( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f072( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f183( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f132( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f123( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f033( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f222( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f018( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f237( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 0, 0, 0, 0, 0, 0, 1
template< typename V, typename P >
void marching_cubes< V, P >::f129( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f126( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f036( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f219( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f024( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f231( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f066( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f189( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

// 1, 1, 1, 0, 0, 0, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f007( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f248( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f013( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f242( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f014( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f241( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f011( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f244( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f081( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f174( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f084( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f171( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f069( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f186( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f021( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f234( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f176( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f079( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f224( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f031( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f208( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f047( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f112( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f143( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f042( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f213( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f138( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f117( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f168( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f087( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f162( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f093( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f140( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f115( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f076( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f179( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f196( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f059( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f200( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f055( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f019( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f236( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f035( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f220( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f050( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f205( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f049( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f206( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

// 1, 1, 0, 0, 0, 0, 1, 0
template< typename V, typename P >
void marching_cubes< V, P >::f067( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f188( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f133( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f122( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f044( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f211( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f026( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f229( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f145( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f110( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f088( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f167( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f070( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f185( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f037( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f218( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f056( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f199( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f164( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f091( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f193( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f062( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f082( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f173( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f038( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f217( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f074( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f181( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f152( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f103( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f161( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f094( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f137( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f118( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f028( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f227( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f100( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f155( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f194( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f061( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f025( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f230( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f131( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f124( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f098( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f157( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f052( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f203( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

// 1, 0, 0, 1, 0, 1, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f041( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f214( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f022( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f233( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f073( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f182( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f134( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f121( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f097( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f158( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f148( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f107( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f146( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f109( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f104( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f151( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 1, 0, 0, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f015( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f085( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f240( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f170( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f204( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f051( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

// 1, 1, 1, 0, 1, 0, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f023( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f077( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f142( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f043( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f113( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f212( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f178( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f232( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 0, 0, 1, 0, 0
template< typename V, typename P >
void marching_cubes< V, P >::f039( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f029( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f078( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f139( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f083( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f116( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f197( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f177( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f226( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f216( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f058( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f172( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 0, 0, 0, 1, 0
template< typename V, typename P >
void marching_cubes< V, P >::f071( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f141( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f046( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f027( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f209( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f092( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f053( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f184( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f228( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f114( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f202( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f163( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 0, 0, 0, 0, 1
template< typename V, typename P >
void marching_cubes< V, P >::f135( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f045( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f030( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f075( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f089( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f086( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f101( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f149( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f180( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f225( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f210( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f120( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f106( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f154( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f169( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f166( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f156( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f108( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f198( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f201( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f147( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f099( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f054( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f057( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

// 1, 1, 0, 0, 0, 0, 1, 1
template< typename V, typename P >
void marching_cubes< V, P >::f195( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f165( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f060( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f090( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f153( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f102( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

// 1, 0, 0, 1, 0, 1, 1, 0
template< typename V, typename P >
void marching_cubes< V, P >::f105( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P >
void marching_cubes< V, P >::f150( const array< node_type > &nda, vector_list_type &pv, vector_list_type &nv, std::vector< size_type > &sv, float_type th, const vector_type &o, const vector_type &s )
{
	vector_type	p;
	vector_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n, th, o, s ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

/// @}
//  Marching Cubesグループの終わり

// mist名前空間の終わり
_MIST_END

#endif // #ifndef __INCLUDE_MARCHING_CUBES_H__
