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

/// @file mist/spline.h
//!
//! @brief 3次スプライン曲線を描画するためのライブラリ
//!

#ifndef __INCLUDE_SPLINE__
#define __INCLUDE_SPLINE__

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

namespace __spline_utility__
{
	template < class T >
	struct arithmetic_operation
	{
		// 一般のデータ型用の点演算
		static const T add( const T &v1, const typename type_trait< T >::value_type &v2 ){ return( v1 + v2 ); }

		static const T sub( const T &v1, const typename type_trait< T >::value_type &v2 ){ return( v1 - v2 ); }

		static const T mul( const T &v1, const typename type_trait< T >::value_type &v2 ){ return( v1 * v2 ); }

		static const T div( const T &v1, const typename type_trait< T >::value_type &v2 ){ return( v1 / v2 ); }
	};

	template < class T >
	struct arithmetic_operation< vector3< T > >
	{
		// vector3 用の点演算
		static const vector3< T > add( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ) ); }

		static const vector3< T > add( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x + val, v1.y + val, v1.z + val ) ); }

		static const vector3< T > sub( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ) ); }

		static const vector3< T > sub( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x - val, v1.y - val, v1.z - val ) ); }

		static const vector3< T > mul( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ) ); }

		static const vector3< T > mul( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x * val, v1.y * val, v1.z * val ) ); }

		static const vector3< T > div( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x / v2.x, v1.y / v2.y, v1.z / v2.z ) ); }

		static const vector3< T > div( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x / val, v1.y / val, v1.z / val ) ); }
	};

	template < class T >
	struct arithmetic_operation< vector2< T > >
	{
		// vector2 用の点演算
		static const vector2< T > add( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x + v2.x, v1.y + v2.y ) ); }

		static const vector2< T > add( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x + val, v1.y + val ) ); }

		static const vector2< T > sub( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x - v2.x, v1.y - v2.y ) ); }

		static const vector2< T > sub( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x - val, v1.y - val ) ); }

		static const vector2< T > mul( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x * v2.x, v1.y * v2.y ) ); }

		static const vector2< T > mul( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x * val, v1.y * val ) ); }

		static const vector2< T > div( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x / v2.x, v1.y / v2.y ) ); }

		static const vector2< T > div( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x / val, v1.y / val ) ); }
	};

	template < class T >
	struct arithmetic_operation< array< T > >
	{
		// array 用の点演算
		static const array< T > add( const array< T > &v1, const array< T > &v2 )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] + v2[ i ]; 
			} 
			return( ret ); 
		}

		static const array< T > add( const array< T > &v1, const typename array< T >::value_type &val )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] + val; 
			} 
			return( ret ); 
		}

		static const array< T > sub( const array< T > &v1, const array< T > &v2 )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] - v2[ i ]; 
			} 
			return( ret ); 
		}

		static const array< T > sub( const array< T > &v1, const typename array< T >::value_type &val )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] - val; 
			} 
			return( ret ); 
		}

		static const array< T > mul( const array< T > &v1, const array< T > &v2 )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] * v2[ i ]; 
			} 
			return( ret ); 
		}

		static const array< T > mul( const array< T > &v1, const typename array< T >::value_type &val )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] * val; 
			} 
			return( ret ); 
		}

		static const array< T > div( const array< T > &v1, const array< T > &v2 )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] / v2[ i ]; 
			} 
			return( ret ); 
		}

		static const array< T > div( const array< T > &v1, const typename array< T >::value_type &val )
		{ 
			array< T > ret( v1.size( ) ); 
			for( size_t i = 0 ; i < ret.size( ) ; i ++ )
			{ 
				ret[ i ] = v1[ i ] / val; 
			} 
			return( ret ); 
		}
	};


	// 一般のデータ型用の点演算
	template < class T1, class T2 >
	inline const T1 add( const T1 &v1, const T2 &v2 ){ return( arithmetic_operation< T1 >::add( v1, v2 ) ); }

	template < class T1, class T2 >
	inline const T1 sub( const T1 &v1, const T2 &v2 ){ return( arithmetic_operation< T1 >::sub( v1, v2 ) ); }

	template < class T1, class T2 >
	inline const T1 div( const T1 &v1, const T2 &v2 ){ return( arithmetic_operation< T1 >::div( v1, v2 ) ); }

	template < class T1, class T2 >
	inline const T1 mul( const T1 &v1, const T2 &v2 ){ return( arithmetic_operation< T1 >::mul( v1, v2 ) ); }
}


//! @addtogroup free_form_group 自由曲線・曲面
//!  @{



//! @addtogroup spline_group 3次スプライン曲線
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/spline.h>
//! @endcode
//!
//!  @{



/// @brief 3次スプライン曲線を扱うためのクラス
//!
//! 3次スプライン曲線を描画可能．可変個数の制御点に対応し，開曲線と閉曲線の3次スプライン曲線を描画
//!
//! @attention 入力されるデータ型が，四則演算（要素同士の点演算，単一のスカラー値を全てに代入する操作）をサポートする必要がある．
//! @attention ただし，vector2，vector3 は正しく動作するようになっている．
//! @attention array 等のMISTコンテナを利用する際には，オペレータを有効にする必要がある．
//! 
//! @param T  … 各制御点・補間点を表すデータ構造を指定（double や vector3< double > など）
//!
//! @code 3次スプライン曲線の作成例
//! // 3次で変数の型が double の3次スプラインを作成する
//! mist::spline< double > b;
//! 
//! // 制御点を追加
//! b.push_back( 2.0 );
//! b.push_back( 3.0 );
//! ...
//! 
//! // 曲線の種類を以下の方法のいずれかを用いて設定
//! // ・閉曲線の設定
//! b.mode( mist::spline< double >::CLOSED );
//! // ・最初と最後の制御点を通る場合の設定
//! b.mode( mist::spline< double >::OPEN );
//! 
//! // スプラインを描画する前に必ず呼び出す
//! b.construct_spline( );
//! 
//! // 補間点を計算（区間は０～１）
//! double p1 = b( 0.0 );
//! double p2 = b( 0.4 );
//! double p3 = b( 0.7 );
//! ...
//! 
//! @endcode
//!
template < class T, class Allocator = std::allocator< T > >
class spline : public std::vector< T, Allocator >
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

	typedef std::vector< value_type > point_list;

public:
	/// @brief 3次スプラインを描画する際のパラメータ
	enum SplineMode
	{
		CLOSED,				///< @brief 閉曲線の設定
		OPEN,				///< @brief 開曲線の設定
	};

protected:
	SplineMode mode_;			///< @brief スプライン曲線の設定
	point_list p1_;				///< @brief 各制御点上での微係数

	/// @brief 閉曲線の各制御点上での微係数を計算する
	void closed_spline( )
	{
		using namespace __spline_utility__;

		base p = *this;

		p.push_back( p[ 0 ] );

		difference_type n, num = p.size( );

		value_type *a = new value_type[ num ];
		value_type *b = new value_type[ num ];
		value_type *c = new value_type[ num ];

		// まず，単位要素を作成する
		value_type _0 = mul( value_type( base::operator[]( 0 ) ), 0 );
		value_type _1 = add( _0, 1 );
		value_type _2 = add( _0, 2 );
		value_type _4 = add( _0, 4 );

		p1_.clear( );

		// 開始点と終点について
		a[ 0 ]   = _1;
		b[ 0 ]   = _4;
		c[ 0 ]   = _1;
		p1_.push_back( mul( sub( p[ 1 ], p[ num - 2 ] ), 3 ) );

		// 開始点と終点以外の制御点に関して初期化
		for( n = 1 ; n < num - 1 ; n++ )
		{
			a[ n ]   = _1;
			b[ n ]   = _4;
			c[ n ]   = _1;
			p1_.push_back( mul( sub( p[ n + 1 ], p[ n - 1 ] ), 3 ) );
		}

		a[ num - 1 ]   = _1;
		b[ num - 1 ]   = _4;
		c[ num - 1 ]   = _1;
		p1_.push_back( p1_[ 0 ] );

		// 一次微係数の計算
		for( n = 1; n < num - 1; n++ )		// 開始点と終点以外に関して
		{
			a[ n ]   = div( a[ n ], b[ n - 1 ] );
			b[ n ]   = sub( b[ n ], mul( a[ n ], c[ n - 1 ] ) );
			p1_[ n ] = sub( p1_[ n ],  mul( a[ n ],  p1_[ n - 1 ] ) );
			a[ n ]   = mul( mul( a[ n - 1 ],  a[ n ] ), -1 );
		}

		a[ num - 2 ] = mul( div( c[ num - 2 ], add( a[ num - 2 ], b[ num - 2 ] ) ), -1 );
		b[ num - 2 ] = mul( mul( a[ num - 2 ], div( p1_[ num - 2 ], c[ num - 2 ] ) ), -1 );

		value_type tmp( a[ num - 3 ] );
		a[ num - 3 ] = mul( div( mul( add( a[ num - 3 ], c[ num - 3 ] ), a[ num - 2 ] ), b[ num - 3 ] ), -1 );
		b[ num - 3 ] = div( sub( p1_[ num - 3 ], mul( add( tmp, c[ num - 2 ] ), b[ num - 2 ] ) ), b[ num - 3 ] );

		for( n = num - 4 ; n >= 0 ; n-- )
		{
			value_type tmp( a[ n ] );
			a[ n ] = div( mul( add( mul( c[ n ], a[ n + 1 ] ), mul( a[ n ], a[ num - 2 ] ) ), -1 ), b[ n ] );
			b[ n ] = div( sub( sub( p1_[ n ], mul( c[ n ], b[ n + 1 ] ) ), mul( tmp, b[ num - 2 ] ) ), b[ n ] );
		}

		p1_[ 0 ] = div( b[ 0 ], add( a[ 0 ], 1 ) );

		for( n = 1 ; n < num - 1 ; n++ )
		{
			p1_[ n ] = add( mul( a[ n ], p1_[ 0 ] ), b[ n ] );
		}

		p1_.pop_back( );

		delete [] a;
		delete [] b;
		delete [] c;
	}

	/// @brief 開曲線の各制御点上での1次微係数を計算する
	void open_spline( )
	{
		using namespace __spline_utility__;

		const base &p = *this;

		difference_type n, num = p.size( );

		value_type *a = new value_type[ num ];
		value_type *b = new value_type[ num ];
		value_type *c = new value_type[ num ];

		p1_.clear( );

		// まず，単位要素を作成する
		value_type _0 = mul( value_type( base::operator[]( 0 ) ), 0 );
		value_type _1 = add( _0, 1 );
		value_type _2 = add( _0, 2 );
		value_type _4 = add( _0, 4 );

		// 開始点と終点について
		a[ 0 ]   = _0;
		b[ 0 ]   = _2;
		c[ 0 ]   = _1;
		p1_.push_back( mul( sub( p[ 1 ], p[ 0 ] ), 3 ) );

		// 開始点と終点以外の制御点に関して初期化
		for( n = 1 ; n < num - 1 ; n++ )
		{
			a[ n ]   = _1;
			b[ n ]   = _4;
			c[ n ]   = _1;
			p1_.push_back( mul( sub( p[ n + 1 ], p[ n - 1 ] ), 3 ) );
		}

		a[ num - 1 ]   = _1;
		b[ num - 1 ]   = _2;
		c[ num - 1 ]   = _0;
		p1_.push_back( mul( sub( p[ num-1 ], p[ num-2 ] ), 3 ) );

		// 一次微係数の計算
		for( n = 1; n < num; n++ )
		{
			a[ n ]   = div( a[ n ], b[ n - 1 ] );
			b[ n ]   = sub( b[ n ], mul( a[ n ], c[ n - 1 ] ) );
			p1_[ n ] = sub( p1_[ n ],  mul( a[ n ],  p1_[ n - 1 ] ) );
		}

		p1_[ num-1 ] = div( p1_[ num-1 ], b[ num-1 ] );

		for( n = num-2; n >= 0; n-- )
		{
			p1_[ n ] = div( sub( p1_[n], mul( c[n], p1_[ n+1 ] ) ), b[n] );
		}

		delete [] a;
		delete [] b;
		delete [] c;
	}

public:
	/// @brief スプラインを描画する前に必ず呼び出す
	void construct_spline( )
	{
		switch( mode_ )
		{
		case CLOSED:
            closed_spline( );
			break;

		case OPEN:
		default:
			open_spline( );
			break;
		}
	}

	/// @brief 指定された位置（区間０～１）に対応する，3次スプライン補間結果を返す
	//!
	//! @param[in] t  … 全体の曲線を区間０～１とした時に，補間される点の位置
	//!
	//! @return 指定された点の位置に対応する3次スプライン曲線の補間結果
	//!
	value_type operator( )( double t )
	{
		using namespace __spline_utility__;

		// 曲線を構築するのに必要な点数が存在しない場合
		if( base::size( ) < 3 || p1_.size( ) < base::size( ) )
		{
			return( base::empty( ) ? value_type( 0 ) : base::at( 0 ) );
		}

		if( t < 0.0 )
		{
			t = 0.0;
		}
		else if( t > 1.0 )
		{
			t = 1.0;
		}

		size_type num;

		switch( mode_ )
		{
		case CLOSED:
			num = p1_.size( );
			break;

		case OPEN:
		default:
			num = p1_.size( ) - 1;
			break;
		}

		size_type n = static_cast< size_type >( t * static_cast< double >( num ) );
		double step = 1.0 / static_cast< double >( num );

		value_type a0, a1, a2, a3;
		const base &p = *this;

		switch( mode_ )
		{
		case CLOSED:
			if( n >= num - 1 )
			{
				a3 = add( mul( sub( p[ num - 1 ], p[ 0 ] ), 2 ), add( p1_[ num - 1 ], p1_[ 0 ] ) );
				a2 = sub( add( mul( sub( p[ num - 1 ], p[ 0 ] ), -3 ), mul( p1_[ num - 1 ], -2 ) ), p1_[ 0 ] );
				a1 = p1_[ num - 1 ];
				a0 = p[ num - 1 ];
				if( n == num )
				{
					n--;
				}
			}
			else
			{
				a3 = add( mul( sub( p[ n ], p[ n + 1 ] ), 2 ), add( p1_[ n ], p1_[ n + 1 ] ) );
				a2 = sub( add( mul( sub( p[ n ], p[ n + 1 ] ), -3 ), mul( p1_[ n ], -2 ) ), p1_[ n + 1 ] );
				a1 = p1_[ n ];
				a0 = p[ n ];
			}
			break;

		case OPEN:
		default:
			if( n == num )
			{
				return( p[ n ] );
			}
			else
			{
				a3 = add( mul( sub( p[ n ], p[ n + 1 ] ), 2 ), add( p1_[ n ], p1_[ n + 1 ] ) );
				a2 = sub( add( mul( sub( p[ n ], p[ n + 1 ] ), -3 ), mul( p1_[ n ], -2 ) ), p1_[ n + 1 ] );
				a1 = p1_[ n ];
				a0 = p[ n ];
			}
			break;
		}


		double s1 = t / step - static_cast< double >( n );
		double s2 = s1 * s1;
		double s3 = s2 * s1;

		return( add( add( add( mul( a3, s3 ), mul( a2, s2 ) ), mul( a1, s1 ) ), a0 ) );
	}


	/// @brief スプライン曲線の種類を取得
	SplineMode mode( ) const { return( mode_ ); }

	/// @brief スプライン曲線の種類を設定
	SplineMode mode( SplineMode m ){ return( mode_ = m ); }


	/// @brief 代入演算子
	const spline &operator =( const spline &b )
	{
		if( this != &b )
		{
			base::operator =( b );
			p1_   = b.p1_;
			mode_ = b.mode_;
		}
		return( *this );
	}

	/// @brief コピーコンストラクタ
	spline( const spline &b ) : base( b ), mode_( b.mode_ ), p1_( b.p1_ ){ }

	/// @brief デフォルトのコンストラクタ
	//!
	//! デフォルト値を「開曲線」に設定する
	//!
	spline( ) : mode_( OPEN ){ }
};

/// @}
//  3次スプライングループの終わり


/// @}
//  自由曲線・曲面グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_SPLINE__
