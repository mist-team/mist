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
#include <mist/mist.h>

#include <mist/interval_tree.h>

// mist名前空間の始まり
_MIST_BEGIN

//! @addtogroup marching_cubes_group Marching Cubes
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/marching_cubes.h>
//! @endcode
//!
//!  @{

namespace mc
{
	/// @brief x, y, z 値を持つポリゴンの頂点や法線を表すための構造体
	//!
	//! @param T … ベクトルの要素の型
	//!
	template< typename T >
	class point
	{
	public:
		typedef T value_type;	///< @brief 各 x, y, z 値の型

		/// @brief x の値を返す
		//!
		//! @return x の値
		//!
		const value_type &x( ) const { return x_; }
		
		/// @brief y の値を返す
		//!
		//! @return y の値
		//!
		const value_type &y( ) const { return y_; }
		
		/// @brief z の値を返す
		//!
		//! @return z の値
		//!
		const value_type &z( ) const { return z_; }
		
		/// @brief x の値を返す（入出力可能）
		//!
		//! @return x の値
		//!
		value_type &x( ) { return x_; }
		
		/// @brief y の値を返す（入出力可能）
		//!
		//! @return y の値
		//!
		value_type &y( ) { return y_; }
		
		/// @brief z の値を返す（入出力可能）
		//!
		//! @return z の値
		//!
		value_type &z( ) { return z_; }

		/// @brief デフォルトコンストラクタ
		point( ) : x_( value_type( ) ), y_( value_type( ) ), z_( value_type( ) ) { }
		
		/// @brief コンストラクタ
		//!
		//! @param[in] x … x の値
		//! @param[in] y … y の値
		//! @param[in] z … z の値
		//!
		point( const value_type &x, const value_type &y, const value_type &z ) : x_( x ), y_( y ), z_( z ) { }
		
	protected:
		value_type x_;
		value_type y_; 
		value_type z_;
	};

	/// @brief 出力演算子
	//!
	//! @param[in]  in   … 入力
	//! @param[out] out  … 出力ストリーム
	//! @return 出力ストリーム
	//!
	template< typename T >
	inline std::ostream &operator <<( std::ostream &out, const point< T > &in )
	{
		return out << in.x( ) << ", " << in.y( ) << ", " << in.z( );
	}

	/// @brief 法線ベクトルの単位ベクトル化
	//!
	//! @param[in]  in   … 単位ベクトル化前のベクトル
	//! @param[out] out  … 単位ベクトル化後のベクトル
	//!
	template< typename I, typename O >
	inline void normalize( const point< I > &in, point< O > &out )
	{
		const double nrm = std::sqrt( static_cast< double >( in.x( ) ) * in.x( ) + static_cast< double >( in.y( ) ) * in.y( ) + static_cast< double >( in.z( ) ) * in.z( ) );
		if( nrm != 0.0 )
		{
			out.x( ) = static_cast< O >( in.x( ) / nrm );
			out.y( ) = static_cast< O >( in.y( ) / nrm );
			out.z( ) = static_cast< O >( in.z( ) / nrm );
		}
		else
		{
			out.x( ) = O( );
			out.y( ) = O( );
			out.z( ) = O( );
		}
	}

	/// @brief cubeの頂点ノードを扱うためのクラス（ marching_cubes クラスの内部でのみ利用 ）
	//!
	//! @param V … 頂点の値の型
	//! @param P … 頂点の座標の型
	//! @param N … 頂点の法線ベクトルの型
	//!
	template< typename V, typename P, typename N >
	class node
	{
	public:
		typedef V	value_type;
		typedef P	point_type;
		typedef N	normal_type;

		const value_type						&v( )  const { return v_; }
		const point_type						&p( )  const { return p_; }
		const normal_type						&n( )  const { return n_; }
		const typename point_type::value_type	&px( ) const { return p_.x( ); }
		const typename point_type::value_type	&py( ) const { return p_.y( ); }
		const typename point_type::value_type	&pz( ) const { return p_.z( ); }
		const typename normal_type::value_type	&nx( ) const { return n_.x( ); }
		const typename normal_type::value_type	&ny( ) const { return n_.y( ); }
		const typename normal_type::value_type	&nz( ) const { return n_.z( ); }

		value_type							&v( )  { return v_; }
		point_type							&p( )  { return p_; }
		normal_type							&n( )  { return n_; }
		typename point_type::value_type		&px( ) { return p_.x( ); }
		typename point_type::value_type		&py( ) { return p_.y( ); }
		typename point_type::value_type		&pz( ) { return p_.z( ); }
		typename normal_type::value_type	&nx( ) { return n_.x( ); }
		typename normal_type::value_type	&ny( ) { return n_.y( ); }
		typename normal_type::value_type	&nz( ) { return n_.z( ); }

		node( ) : v_( value_type( ) ), p_( point_type( ) ), n_( normal_type( ) ) { }
		node( const value_type &v, const point_type &p, const normal_type &n ) : v_( v ), p_( p ), n_( n ) { }

	private:
		value_type	v_;
		point_type	p_;
		normal_type	n_;
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


/// @brief ボリュームデータから等値面を生成するためのクラス
//!
//! 区間の集合から2分木を構築し、通常O(n)かかる探索時間をO(log_2(n))に削減する．
//! 線分や面分の重なり判定などに有効．
//!
//! @attention mist::arra3< 算術型 > 型を入力とする．
//! @attention ポリゴンの各頂点座標（ mist::mc::point< 算術型 >型 ），各頂点の法線ベクトル（ mist::mc::point< 算術型 > 型 ），各ポリゴンの頂点数（ size_t 型 ）の3つのstd::vectorを等値面生成結果の出力とする．
//! @attention ポリゴンの各頂点座標と各頂点の法線ベクトルは OpenGL の頂点配列として利用可能（ GL_POLYGON や GL_TRIANGLE_FAN 等に使える）．
//! 
//! @param V  … ボリュームデータの要素の型
//! @param P  … 等値面ポリゴンの頂点座標の要素の型
//! @param N  … 等値面ポリゴンの法線ベクトルの要素の型
//!
//! @code 使用例
//! // ボリュームデータの用意
//! mist::array3< int > va;
//! /* 
//!	 * ボリュームデータの作成処理
//!  */
//! 
//! // 等値面生成結果を格納するためのオブジェクトの準備
//! std::vector< mist::mc::point< double > >	pv;		// double 型の要素を持つ頂点座標の集合
//! std::vector< mist::mc::normal< double > >	nv;		// double 型の要素を持つ法線ベクトルの集合
//! std::vector< size_t >						sv;		// 各ポリゴンのサイズ（頂点数）の集合
//!
//! // ボリュームデータの要素が int 型で，
//! // 得られる等値面ポリゴンの頂点座標の要素と法線ベクトルが double 型の
//! // marching_cubes オブジェクトを作成
//! mist::marching_cubes< int, double, double >	mcs;
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
template< typename V, typename P = double, typename N = double >
class marching_cubes
{
public:
	typedef V							in_value_type;		///< @brief ボリュームデータの要素の型
	typedef typename mc::point< short >	in_point_type;		///< @brief cube の頂点座標の型
	typedef typename mc::point< short >	in_normal_type;		///< @brief cube の頂点の法線ベクトルの型
	typedef typename mc::point< P >		out_point_type;		///< @brief ポリゴンの頂点座標の型
	typedef typename mc::point< N >		out_normal_type;	///< @brief ポリゴンの頂点の法汚染ベクトルの型
	typedef size_t						out_size_type;		///< @brief 各ポリゴンのサイズ（頂点数）の型

	typedef typename mc::node< in_value_type, in_point_type, in_normal_type >	node_type;	///< @brief cube の頂点情報を保持する型
	
	typedef double							threshold_type;		///< @brief 閾値パラメータの型
	typedef typename mc::point< double >	offset_type;		///< @brief オフセットパラメータの型
	typedef typename mc::point< double >	scale_type;			///< @brief スケールパラメータの型
	typedef void ( *						func_type )( const array< node_type > &, std::vector< out_point_type > &, std::vector< out_normal_type > &, std::vector< out_size_type > & );	///< @brief cube 単位での等値面生成関数の型
	
	typedef tagged_section< in_value_type, size_t >		section_type;	///< @brief Interval-tree 用いる区間の型
	typedef typename section_type::min_max_type			min_max_type;	///< @brief 区間の最大値，最小値の型
	typedef typename section_type::tag_type				tag_type;		///< @brief 区間のタグ情報の型


	/// @brief 等値面生成（前処理の有無で等値面生成処理を分ける）
	//!
	//! @param[in]  va   … ボリュームデータ
	//! @param[out] pv   … 等値面ポリゴンの頂点座標の集合
	//! @param[out] nv   … 等値面ポリゴンの頂点の法線ベクトルの集合
	//! @param[out] sv   … 等値面ポリゴンのサイズの集合
	//!
	void isosurfacing( const array3< in_value_type > &va, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
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
	void isosurfacing_with_preprocess( const array3< in_value_type > &va, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
	{
		pv.resize( 0 );
		nv.resize( 0 );
		sv.resize( 0 );
		it_.find( th_, active_cube_tags_ );
		std::sort( active_cube_tags_.begin( ), active_cube_tags_.end( ) );
		array< node_type > nda( 8 );
		const size_t begin = 0;
		const size_t end = active_cube_tags_.size( );
		for( size_t i = begin ; i < end ; i ++ )
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
	void isosurfacing_without_preprocess( const array3< in_value_type > &va, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
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
	bool preprocess( const array3< in_value_type > &va )
	{
		if( is_preprocessed_ )
		{
			de_preprocess( );
		}
		is_preprocessed_ = ( construct_pointer_difference_array( va.width( ), va.height( ) ) && construct_point_array( va ) && construct_normal_array( va ) && construct_interval_tree( va ) );
		if( !is_preprocessed_ )
		{
			std::cout << "can't construct preprocessing data for lack of memory, use non-preprocessing version of marvching cubes." << std::endl;
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
	void threshold( const threshold_type &th ) { th_ = th; }
	
	/// @brief オフセットパラメータを設定
	//!
	//! @param[in]  ox   … x 方向オフセット
	//! @param[in]  oy   … y 方向オフセット
	//! @param[in]  oz   … z 方向オフセット
	//!
	void offset( const offset_type::value_type &ox, const offset_type::value_type &oy, const offset_type::value_type &oz ) { o_.x( ) = ox; o_.y( ) = oy; o_.z( ) = oz; }
	
	/// @brief スケールパラメータを設定
	//!
	//! @param[in]  sx   … x 方向スケール
	//! @param[in]  sy   … y 方向スケール
	//! @param[in]  sz   … z 方向スケール
	//!
	void scale(  const scale_type::value_type  &sx, const scale_type::value_type  &sy, const scale_type::value_type  &sz ) { s_.x( ) = sx; s_.y( ) = sy; s_.z( ) = sz; }

	/// @brief 設定されている閾値パラメータを取得
	//!
	//! @return    … 閾値
	//!
	const threshold_type			&th( ) const { return th_; }
	
	/// @brief 設定されている x 方向オフセットパラメータを取得
	//!
	//! @return    … x 方向オフセット
	//!
	const offset_type::value_type	&ox( ) const { return o_.x( ); }
	
	/// @brief 設定されている y 方向オフセットパラメータを取得
	//!
	//! @return    … y 方向オフセット
	//!
	const offset_type::value_type	&oy( ) const { return o_.y( ); }
	
	/// @brief 設定されている z 方向オフセットパラメータを取得
	//!
	//! @return    … z 方向オフセット
	//!
	const offset_type::value_type	&oz( ) const { return o_.z( ); }
	
	/// @brief 設定されている x 方向スケールパラメータを取得
	//!
	//! @return    … x 方向スケール
	//!
	const scale_type::value_type	&sx( ) const { return s_.x( ); }
	
	/// @brief 設定されている y 方向スケールパラメータを取得
	//!
	//! @return    … y 方向スケール
	//!
	const scale_type::value_type	&sy( ) const { return s_.y( ); }
	
	/// @brief 設定されている z 方向スケールパラメータを取得
	//!
	//! @return    … z 方向スケール
	//!
	const scale_type::value_type	&sz( ) const { return s_.z( ); }

	/// @brief コンストラクタ
	marching_cubes( ) : is_preprocessed_( false )
	{
		init_function_array( ); 
	}

	/// @brief デストラクタ
	~marching_cubes( )
	{
	}

private:
	func_type				fa_[ 256 ];

	bool											is_preprocessed_;
	array< size_t >							pda_;
	array3< in_point_type >					pa_;
	array3< in_normal_type >					na_;
	interval_tree< section_type, threshold_type >	it_;
	std::vector< tag_type >							active_cube_tags_;

	static threshold_type	th_;
	static offset_type		o_;
	static scale_type		s_;


	void construct_cube( const array3< in_value_type > &va, const size_t i, array< node_type > &nda ) const
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

	void construct_cube_without_preprocessing( const array3< in_value_type > &va, const size_t i, const size_t j, const size_t k, array< node_type > &nda ) const
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

	void isosurfacing_in_cube( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv,std::vector< out_size_type > &sv )
	{
		fa_[ pattern( nda ) ]( nda, pv, nv, sv );
	}

	in_point_type _point( const size_t i, const size_t j, const size_t k ) const
	{
		return in_point_type( static_cast< typename in_point_type::value_type >( i ), static_cast< typename in_point_type::value_type >( j ), static_cast< typename in_point_type::value_type >( k ) );
	}

	in_normal_type _normal( const array3< in_value_type > &va, const size_t i, const size_t j, const size_t k ) const
	{
		mc::point< double > n;
		mc::normalize< double, double >( mc::point< double >( static_cast< double >( _value( va, i - 1, j, k ) ) - _value( va, i + 1, j , k ), static_cast< double >( _value( va, i, j - 1, k ) ) - _value( va, i, j + 1, k ), static_cast< double >( _value( va, i, j, k - 1 ) ) - _value( va, i, j, k + 1 ) ), n );
		return in_normal_type( static_cast< typename in_normal_type::value_type >( n.x( ) * 32767 ), static_cast< typename in_normal_type::value_type >( n.y( ) * 32767 ), static_cast< typename in_normal_type::value_type >( n.z( ) * 32767 ) );
	}

	in_value_type _value( const array3< in_value_type > &va, const size_t i , const size_t j, const size_t k ) const
	{
		return ( i < va.width( ) && j < va.height( ) && k < va.depth( ) ) ? va( i, j, k ) : in_value_type( );
	}

	bool construct_pointer_difference_array( const size_t w, const size_t h )
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

	bool construct_point_array( const array3< in_value_type > &va )
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

	bool construct_normal_array( const array3< in_value_type > &va )
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

	bool construct_interval_tree( const array3< in_value_type > &va )
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

	void get_section( const array3< in_value_type > &va, const tag_type &tag, min_max_type &min, min_max_type &max ) const
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

	static void interpolation_about_x( const node_type &nd0, const node_type &nd1, out_point_type &p, out_normal_type &n )
	{
		const double tp = ( nd1.v( ) - th_ ) / ( static_cast< double >( nd1.v( ) ) - nd0.v( ) ); 
		p.x( ) = static_cast< typename out_point_type::value_type >( nd0.px( ) * tp + nd1.px( ) * ( 1.0 - tp ) );
		p.y( ) = static_cast< typename out_point_type::value_type >( nd0.py( ) );
		p.z( ) = static_cast< typename out_point_type::value_type >( nd0.pz( ) );
		const double tn = ( static_cast< double >( nd1.px( ) ) - p.x( ) ) / ( nd1.px( ) - nd0.px( ) );
		mc::normalize< double, typename out_normal_type::value_type >( mc::point< double >( nd0.nx( ) * tn + nd1.nx( ) * ( 1.0 - tn ), nd0.ny( ) * tn + nd1.ny( ) * ( 1.0 - tn ), nd0.nz( ) * tn + nd1.nz( ) * ( 1.0 - tn ) ), n );
		p.x( ) = static_cast< typename out_point_type::value_type >( ( p.x( ) + o_.x( ) ) * s_.x( ) );
		p.y( ) = static_cast< typename out_point_type::value_type >( ( p.y( ) + o_.y( ) ) * s_.y( ) );
		p.z( ) = static_cast< typename out_point_type::value_type >( ( p.z( ) + o_.z( ) ) * s_.z( ) );
	}

	static void interpolation_about_y( const node_type &nd0, const node_type &nd1, out_point_type &p, out_normal_type &n )
	{
		const double tp = ( nd1.v( ) - th_ ) / ( static_cast< double >( nd1.v( ) ) - nd0.v( ) ); 
		p.x( ) = static_cast< typename out_point_type::value_type >( nd0.px( ) );
		p.y( ) = static_cast< typename out_point_type::value_type >( nd0.py( ) * tp + nd1.py( ) * ( 1.0 - tp ) );
		p.z( ) = static_cast< typename out_point_type::value_type >( nd0.pz( ) );
		const double tn = ( static_cast< double >( nd1.py( ) ) - p.y( ) ) / ( nd1.py( ) - nd0.py( ) );
		mc::normalize< double, typename out_normal_type::value_type >( mc::point< double >( nd0.nx( ) * tn + nd1.nx( ) * ( 1.0 - tn ), nd0.ny( ) * tn + nd1.ny( ) * ( 1.0 - tn ), nd0.nz( ) * tn + nd1.nz( ) * ( 1.0 - tn ) ), n );
		p.x( ) = static_cast< typename out_point_type::value_type >( ( p.x( ) + o_.x( ) ) * s_.x( ) );
		p.y( ) = static_cast< typename out_point_type::value_type >( ( p.y( ) + o_.y( ) ) * s_.y( ) );
		p.z( ) = static_cast< typename out_point_type::value_type >( ( p.z( ) + o_.z( ) ) * s_.z( ) );
	}

	static void interpolation_about_z( const node_type &nd0, const node_type &nd1, out_point_type &p, out_normal_type &n )
	{
		const double tp = ( nd1.v( ) - th_ ) / ( static_cast< double >( nd1.v( ) ) - nd0.v( ) ); 
		p.x( ) = static_cast< typename out_point_type::value_type >( nd0.px( ) );
		p.y( ) = static_cast< typename out_point_type::value_type >( nd0.py( ) );
		p.z( ) = static_cast< typename out_point_type::value_type >( nd0.pz( ) * tp + nd1.pz( ) * ( 1.0 - tp ) );
		const double tn = ( static_cast< double >( nd1.pz( ) ) - p.z( ) ) / ( nd1.pz( ) - nd0.pz( ) );
		mc::normalize< double, typename out_normal_type::value_type >( mc::point< double >( nd0.nx( ) * tn + nd1.nx( ) * ( 1.0 - tn ), nd0.ny( ) * tn + nd1.ny( ) * ( 1.0 - tn ), nd0.nz( ) * tn + nd1.nz( ) * ( 1.0 - tn ) ), n );
		p.x( ) = static_cast< typename out_point_type::value_type >( ( p.x( ) + o_.x( ) ) * s_.x( ) );
		p.y( ) = static_cast< typename out_point_type::value_type >( ( p.y( ) + o_.y( ) ) * s_.y( ) );
		p.z( ) = static_cast< typename out_point_type::value_type >( ( p.z( ) + o_.z( ) ) * s_.z( ) );
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

	static void f000( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f001( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f002( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f003( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f004( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f005( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f006( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f007( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f008( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f009( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f010( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f011( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f012( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f013( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f014( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f015( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f016( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f017( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f018( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f019( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f020( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f021( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f022( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f023( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f024( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f025( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f026( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f027( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f028( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f029( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f030( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f031( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f032( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f033( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f034( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f035( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f036( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f037( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f038( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f039( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f040( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f041( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f042( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f043( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f044( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f045( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f046( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f047( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f048( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f049( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f050( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f051( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f052( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f053( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f054( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f055( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f056( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f057( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f058( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f059( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f060( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f061( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f062( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f063( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f064( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f065( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f066( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f067( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f068( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f069( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f070( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f071( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f072( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f073( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f074( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f075( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f076( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f077( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f078( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f079( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f080( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f081( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f082( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f083( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f084( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f085( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f086( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f087( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f088( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f089( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f090( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f091( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f092( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f093( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f094( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f095( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f096( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f097( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f098( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f099( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f100( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f101( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f102( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f103( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f104( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f105( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f106( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f107( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f108( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f109( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f110( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f111( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f112( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f113( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f114( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f115( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f116( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f117( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f118( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f119( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f120( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f121( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f122( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f123( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f124( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f125( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f126( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f127( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f128( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f129( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f130( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f131( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f132( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f133( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f134( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f135( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f136( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f137( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f138( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f139( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f140( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f141( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f142( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f143( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f144( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f145( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f146( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f147( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f148( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f149( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f150( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f151( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f152( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f153( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f154( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f155( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f156( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f157( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f158( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f159( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f160( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f161( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f162( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f163( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f164( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f165( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f166( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f167( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f168( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f169( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f170( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f171( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f172( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f173( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f174( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f175( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f176( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f177( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f178( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f179( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f180( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f181( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f182( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f183( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f184( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f185( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f186( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f187( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f188( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f189( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f190( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f191( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f192( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f193( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f194( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f195( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f196( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f197( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f198( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f199( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f200( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f201( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f202( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f203( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f204( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f205( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f206( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f207( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f208( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f209( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f210( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f211( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f212( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f213( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f214( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f215( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f216( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f217( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f218( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f219( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f220( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f221( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f222( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f223( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f224( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f225( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f226( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f227( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f228( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f229( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f230( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f231( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f232( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f233( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f234( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f235( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f236( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f237( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f238( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f239( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f240( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f241( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f242( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f243( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f244( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f245( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f246( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f247( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f248( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f249( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f250( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f251( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f252( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f253( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f254( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
	static void f255( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv );
};

template< typename V, typename P, typename N > double				marching_cubes< V, P, N >::th_ = 0.0;
template< typename V, typename P, typename N > mc::point< double >	marching_cubes< V, P, N >::o_ = mc::point< double >( 0.0, 0.0, 0.0 );
template< typename V, typename P, typename N > mc::point< double >	marching_cubes< V, P, N >::s_ = mc::point< double >( 1.0, 1.0, 1.0 );

// 0, 0, 0, 0, 0, 0, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f000( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f255( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
}

// 1, 0, 0, 0, 0, 0, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f001( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f254( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f004( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f251( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f008( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f247( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f002( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f253( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f016( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f239( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f064( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f191( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f032( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f223( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f128( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f127( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

// 1, 1, 0, 0, 0, 0, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f003( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f252( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f005( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f250( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f012( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f243( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f010( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f245( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f017( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f238( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f080( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f175( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f068( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f187( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f048( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f207( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f160( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f095( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f192( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f063( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f034( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f221( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f136( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f119( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

// 1, 0, 0, 1, 0, 0, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f009( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f246( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f006( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f249( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f020( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f235( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f065( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f190( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f096( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f159( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f144( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f111( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f130( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f125( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f040( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f215( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f072( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f183( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f132( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f123( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f033( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f222( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f018( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f237( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 0, 0, 0, 0, 0, 0, 1
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f129( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f126( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f036( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f219( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f024( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f231( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f066( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f189( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

// 1, 1, 1, 0, 0, 0, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f007( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f248( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f013( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f242( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f014( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f241( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f011( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f244( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f081( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f174( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f084( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f171( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f069( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f186( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f021( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f234( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f176( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f079( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f224( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f031( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f208( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f047( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f112( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f143( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f042( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f213( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f138( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f117( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f168( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f087( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f162( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f093( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f140( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f115( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f076( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f179( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f196( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f059( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f200( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f055( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f019( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f236( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f035( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f220( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f050( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f205( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f049( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f206( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
}

// 1, 1, 0, 0, 0, 0, 1, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f067( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f188( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f133( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f122( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f044( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f211( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f026( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f229( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f145( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f110( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f088( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f167( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f070( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f185( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f037( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f218( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f056( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f199( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f164( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f091( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f193( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f062( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f082( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f173( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f038( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f217( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f074( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f181( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f152( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f103( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f161( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f094( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f137( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f118( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f028( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f227( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f100( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f155( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f194( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f061( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f025( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f230( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f131( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f124( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f098( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f157( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f052( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f203( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 7 );
}

// 1, 0, 0, 1, 0, 1, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f041( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f214( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f022( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f233( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f073( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f182( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f134( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f121( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f097( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f158( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f148( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f107( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f146( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f109( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f104( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f151( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 1, 0, 0, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f015( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f085( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f240( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f170( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f204( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f051( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

// 1, 1, 1, 0, 1, 0, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f023( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f077( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f142( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f043( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f113( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f212( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f178( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f232( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 0, 0, 1, 0, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f039( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f029( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f078( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f139( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f083( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f116( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f197( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f177( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f226( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f216( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f058( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f172( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 0, 0, 0, 1, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f071( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f141( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f046( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f027( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f209( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f092( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f053( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f184( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f228( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f114( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f202( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f163( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 6 );
}

// 1, 1, 1, 0, 0, 0, 0, 1
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f135( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f045( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f030( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f075( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f089( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f086( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f101( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f149( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f180( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f225( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f210( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f120( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f106( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f154( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f169( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f166( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f156( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f108( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f198( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f201( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f147( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f099( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f054( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f057( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 5 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

// 1, 1, 0, 0, 0, 0, 1, 1
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f195( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f165( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f060( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f090( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f153( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f102( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 4 );
}

// 1, 0, 0, 1, 0, 1, 1, 0
template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f105( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_x( nda[ 0 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 0 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 0 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_y( nda[ 3 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 3 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 3 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 5 ], nda[ 1 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 5 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 5 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 6 ], nda[ 2 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 6 ], nda[ 7 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 6 ], nda[ 4 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

template< typename V, typename P, typename N >
void marching_cubes< V, P, N >::f150( const array< node_type > &nda, std::vector< out_point_type > &pv, std::vector< out_normal_type > &nv, std::vector< out_size_type > &sv )
{
	out_point_type	p;
	out_normal_type	n;
	interpolation_about_y( nda[ 2 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 2 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 2 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_x( nda[ 1 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_z( nda[ 1 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 1 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 4 ], nda[ 0 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 4 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 4 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
	interpolation_about_z( nda[ 7 ], nda[ 3 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_y( nda[ 7 ], nda[ 5 ], p, n ); pv.push_back( p ); nv.push_back( n );
	interpolation_about_x( nda[ 7 ], nda[ 6 ], p, n ); pv.push_back( p ); nv.push_back( n );
	sv.push_back( 3 );
}

/// @}
//  Marching Cubesグループの終わり

// mist名前空間の終わり
_MIST_END

#endif // #ifndef __INCLUDE_MARCHING_CUBES_H__
