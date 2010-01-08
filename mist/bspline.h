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

/// @file mist/bspline.h
//!
//! @brief Bスプライン曲線を描画するためのライブラリ
//!

#ifndef __INCLUDE_BSPLINE__
#define __INCLUDE_BSPLINE__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <vector>


// mist名前空間の始まり
_MIST_BEGIN


/// @brief Bスプラインの基底関数を計算する関数を実装した構造体
//!
//! K次のBスプライン基底関数の計算を行う
//!
template < int K >
struct bspline_base
{
	/// @brief K次のBスプライン基底関数の計算を行う
	//! 
	//! @param[in] knot … Bスプラインで利用するノットベクトル
	//! @param[in] i    … Bスプラインの制御点番号
	//! @param[in] t    … Bスプラインの区間位置
	//! 
	//! @return 指定されたノットベクトル，制御点番号，区間位置に基づく基底関数の値
	//! 
	static double Base( const std::vector< double > &knot, std::vector< double >::size_type i, double t )
	{
		double B = 0.0;

		if( knot[ i + K - 1 ] - knot[ i ] != 0 )
		{
			B = ( t - knot[ i ] ) * bspline_base< K - 1 >::Base( knot, i, t ) / ( knot[ i + K - 1 ] - knot[ i ] );
		}

		if( knot[ i + K ] - knot[ i + 1 ] != 0 )
		{
			B += ( knot[ i + K ] - t ) * bspline_base< K - 1 >::Base( knot, i + 1, t ) / ( knot[ i + K ] - knot[ i + 1 ] );
		}
		return( B );
	}
};


/// @brief Bスプラインの基底関数を計算する関数を実装した構造体の特殊化
//!
//! 1次のBスプライン基底関数の計算を行うための特殊化
//!
template < >
struct bspline_base< 1 >
{
	/// @brief K次のBスプライン基底関数の計算を行う
	//! 
	//! @param[in] knot … Bスプラインで利用するノットベクトル
	//! @param[in] i    … Bスプラインの制御点番号
	//! @param[in] t    … Bスプラインの区間位置
	//! 
	//! @return 指定されたノットベクトル，制御点番号，区間位置に基づく基底関数の値
	//! 
	static double Base( const std::vector< double > &knot, std::vector< double >::size_type i, double t )
	{
		return( ( knot[ i ] <= t && t < knot[ i + 1 ] ) ? 1.0 : 0.0 );
	}
};


//! @addtogroup free_form_group 自由曲線・曲面
//!  @{



//! @addtogroup bspline_group Bスプライン曲線
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/bspline.h>
//! @endcode
//!
//!  @{


/// @brief Bスプライン曲線を扱うためのクラス
//!
//! K次のBスプライン曲線を描画可能．可変個数の制御点に対応し，任意のノットベクトルに対応するBスプライン曲線を描画
//!
//! @attention 入力されるデータ型が，加減算と単一のスカラー値を全てに代入する操作と浮動小数との掛け算をサポートする必要がある．
//! @attention ただし，vector2，vector3 は正しく動作するようになっている．
//! @attention array 等のMISTコンテナを利用する際には，オペレータを有効にする必要がある．
//! 
//! @param T  … 各制御点・補間点を表すデータ構造を指定（double や vector3< double > など）
//! @param K  … Bスプライン曲線の次数
//!
//! @code Bスプライン曲線の作成例
//! // 3次で変数の型が double のBスプラインを作成する
//! mist::bspline< double, 3 > b;
//! 
//! // 制御点を追加
//! b.push_back( 2.0 );
//! b.push_back( 3.0 );
//! ...
//! 
//! // ノットベクトルを以下の方法のいずれかを用いて設定
//! // ・閉曲線の設定
//! b.knot( mist::bspline< double >::ROUND );
//! // ・最初と最後の制御点を通る場合の設定
//! b.knot( mist::bspline< double >::THROUGH );
//! // ・任意のノットベクトルを設定
//! b.knot( STLのベクトルタイプのノットのリスト );
//! 
//! // 補間点を計算（区間は０～１）
//! double p1 = b( 0.0 );
//! double p2 = b( 0.4 );
//! double p3 = b( 0.7 );
//! ...
//! 
//! @endcode
//!
template < class T, int K, class Allocator = std::allocator< T > >
class bspline : public std::vector< T, Allocator >
{
private:
	typedef std::vector< T, Allocator > base;
	typedef typename base::allocator_type allocator_type;		///< @brief STLコンテナが利用するアロケータ型
	typedef typename base::reference reference;					///< @brief STLのコンテナ内に格納するデータ型の参照
	typedef typename base::const_reference const_reference;		///< @brief STLのコンテナ内に格納するデータ型の const 参照
	typedef typename base::value_type value_type;				///< @brief STLのコンテナ内に格納するデータ型
	typedef typename base::size_type size_type;					///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename base::pointer pointer;						///< @brief STLのコンテナ内に格納するデータ型のポインター型
	typedef typename base::const_pointer const_pointer;			///< @brief STLのコンテナ内に格納するデータ型の const ポインター型

	typedef std::vector< double > knot_list;

public:
	/// @brief ノットベクトルを手動で設定しない場合のデフォルトのモード
	enum BSplineMode
	{
		ROUND,					///< @brief 閉曲線の設定
		THROUGH,				///< @brief 最初と最後の制御点を通る場合の設定
	};

protected:
	knot_list knot_;			///< @brief ノットベクトル
	BSplineMode mode_;			///< @brief デフォルトのノットベクトル設定


public:
	/// @brief 指定された位置（区間０～１）に対応する，Bスプライン補間結果を返す
	//!
	//! @attention 次数＋１の制御点数が入力されている必要あり．足りない場合は，デフォルトのノットベクトルで最初期化される．
	//!
	//! @param[in] t  … 全体の曲線を区間０～１とした時に，補間される点の位置
	//!
	//! @return 指定された点の位置に対応するBスプライン曲線の補間結果
	//!
	value_type operator( )( double t )
	{
		size_type n = base::size( ) - 1;	// n + 1 は制御点の数
		size_type m = n + K; 				// m + 1 はノットベクトルの数
		if( knot_.size( ) < m + 1 )
		{
			// 不適切なノットベクトルが設定されています
			knot( mode_ );
		}
		else if( base::empty( ) )
		{
			return( value_type( 0 ) );
		}

		t *= static_cast< double >( m - 2 * K + 2 );

		// まず，ゼロ要素を作成する
		value_type p = value_type( base::operator[]( 0 ) ) * 0;
		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			double B = bspline_base< K >::Base( knot_, i, t );
			p += B * base::operator[]( i );
		}

		return( p );
	}

	/// @brief 任意のノットベクトルを設定する
	//!
	//! @attention 次数 K で制御点数 N とした時に，ノットベクトルの数は N + K + 1 必要である
	//! 
	//! @param[in] kknot  … ノットベクトル
	//!
	void knot( const knot_list &kknot )
	{
		knot_ = kknot;
	}

	/// @brief デフォルトのモードでノットベクトルを設定する
	//!
	//! 本クラス内で定義されている列挙型 BSplineMode のうちの一つを利用する
	//! 
	//! @param[in] mode  … 曲線のタイプ
	//!
	void knot( BSplineMode mode )
	{
		size_type n = base::size( ) - 1;	// n + 1 は制御点の数
		size_type m = n + K; 				// m + 1 はノットベクトルの数
		size_type i;

		knot_list kknot( m + 1 );
		switch( mode )
		{
		case ROUND:
			// リングの場合
			for( i = 0 ; i <= m ; i++ )
			{
				kknot[ i ] = static_cast< double >( i - K + 1 );
			}
			break;

		case THROUGH:
			// 最初と最後を通る曲線の場合
			for( i = 0 ; i < K ; i++ )
			{
				kknot[ i ] = 0.0;
			}
			for( i = K ; i < m - K + 1 ; i++ )
			{
				kknot[ i ] = static_cast< double >( i - K + 1 );
			}
			for( i = m - K + 1 ; i <= m ; i++ )
			{
				kknot[ i ] = static_cast< double >( 2 + m - 2 * K );
			}
			break;
		}

		knot( kknot );
	}

	/// @brief 代入演算子
	const bspline &operator =( const bspline &b )
	{
		if( this != &b )
		{
			base::operator =( b );
			knot_ = b.knot_;
			mode_ = b.mode_;
		}
		return( *this );
	}

	/// @brief コピーコンストラクタ
	bspline( const bspline &b ) : base( b ), knot_( b.knot_ ), mode_( b.mode_ )
	{
	}

	/// @brief デフォルトのコンストラクタ
	//!
	//! ノットベクトルのデフォルト値を「最初と最後を通る曲線」に設定する
	//!
	bspline( ) : mode_( THROUGH )
	{
	}
};

/// @}
//  Bスプライングループの終わり


/// @}
//  自由曲線・曲面グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_BSPLINE__
