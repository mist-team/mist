/// @file mist/spline.h
//!
//! @brief 3次スプライン曲線を描画するためのライブラリ
//!

#ifndef __INCLUDE_SPLINE__
#define __INCLUDE_SPLINE__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <vector>


// mist名前空間の始まり
_MIST_BEGIN


//! @defgroup spline_group 3次スプライン曲線
//!  @{


// 一般のデータ型用の点演算
template < class T >
inline const T add( const T &v1, const T &v2 ){ return( v1 + v2 ); }

template < class T >
inline const T sub( const T &v1, const T &v2 ){ return( v1 - v2 ); }

template < class T >
inline const T mul( const T &v1, const T &v2 ){ return( v1 * v2 ); }

template < class T >
inline const T div( const T &v1, const T &v2 ){ return( v1 / v2 ); }



// vector3 用の点演算
template < class T >
inline const vector3< T > add( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ) ); }

template < class T >
inline const vector3< T > add( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x + val, v1.y + val, v1.z + val ) ); }

template < class T >
inline const vector3< T > add( const typename vector3< T >::value_type &val, const vector3< T > &v1 ){ return( vector3< T >( val + v1.x, val + v1.y, val + v1.z ) ); }


template < class T >
inline const vector3< T > sub( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ) ); }

template < class T >
inline const vector3< T > sub( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x - val, v1.y - val, v1.z - val ) ); }

template < class T >
inline const vector3< T > sub( const typename vector3< T >::value_type &val, const vector3< T > &v1 ){ return( vector3< T >( val - v1.x, val - v1.y, val - v1.z ) ); }


template < class T >
inline const vector3< T > mul( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ) ); }

template < class T >
inline const vector3< T > mul( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x * val, v1.y * val, v1.z * val ) ); }

template < class T >
inline const vector3< T > mul( const typename vector3< T >::value_type &val, const vector3< T > &v1 ){ return( vector3< T >( val * v1.x, val * v1.y, val * v1.z ) ); }


template < class T >
inline const vector3< T > div( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x / v2.x, v1.y / v2.y, v1.z / v2.z ) ); }

template < class T >
inline const vector3< T > div( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x / val, v1.y / val, v1.z / val ) ); }


// vector2 用の点演算
template < class T >
inline const vector2< T > add( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x + v2.x, v1.y + v2.y ) ); }

template < class T >
inline const vector2< T > add( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x + val, v1.y + val ) ); }

template < class T >
inline const vector2< T > add( const typename vector2< T >::value_type &val, const vector2< T > &v1 ){ return( vector2< T >( val + v1.x, val + v1.y ) ); }


template < class T >
inline const vector2< T > sub( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x - v2.x, v1.y - v2.y ) ); }

template < class T >
inline const vector2< T > sub( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x - val, v1.y - val ) ); }

template < class T >
inline const vector2< T > sub( const typename vector2< T >::value_type &val, const vector2< T > &v1 ){ return( vector2< T >( val - v1.x, val - v1.y ) ); }


template < class T >
inline const vector2< T > mul( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x * v2.x, v1.y * v2.y ) ); }

template < class T >
inline const vector2< T > mul( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x * val, v1.y * val ) ); }

template < class T >
inline const vector2< T > mul( const typename vector2< T >::value_type &val, const vector2< T > &v1 ){ return( vector2< T >( val * v1.x, val * v1.y ) ); }


template < class T >
inline const vector2< T > div( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x / v2.x, v1.y / v2.y ) ); }

template < class T >
inline const vector2< T > div( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x / val, v1.y / val ) ); }




/// @brief 3次スプライン曲線を扱うためのクラス
//!
//! K次の3次スプライン曲線を描画可能．可変個数の制御点に対応し，任意のノットベクトルに対応する3次スプライン曲線を描画
//! 
//! @param T  … 各制御点・補間点を表すデータ構造を指定（double や vector3< double > など）
//! @param K  … 3次スプライン曲線の次数
//!
//! @code 3次スプライン曲線の作成例
//! // 3次で変数の型が double の3次スプラインを作成する
//! mist::spline< double, 3 > b;
//! 
//! // 制御点を追加
//! b.push_back( 2.0 );
//! b.push_back( 3.0 );
//! ...
//! 
//! // ノットベクトルを以下の方法のいずれかを用いて設定
//! // ・閉曲線の設定
//! b.knot( mist::spline< double >::ROUND );
//! // ・最初と最後の制御点を通る場合の設定
//! b.knot( mist::spline< double >::THROUGH );
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

	typedef std::vector< double > point_list;

public:
	/// @brief ノットベクトルを手動で設定しない場合のデフォルトのモード
	enum SplineMode
	{
		ROUND,					///< @brief 閉曲線の設定
		THROUGH,				///< @brief 最初と最後の制御点を通る場合の設定
	};

protected:
	SplineMode mode_;			///< @brief デフォルトのノットベクトル設定
	point_list p1_;				///< @brief デフォルトのノットベクトル設定

	/// @brief 閉曲線の各制御点上での1次美係数を計算する
	void closed_spline( )
	{
		base p = *this;

		p.push_back( p[ 0 ] );

		difference_type n, num = p.size( );

		value_type *a = new value_type[ num ];
		value_type *b = new value_type[ num ];
		value_type *c = new value_type[ num ];

		p1_.reserve( num );

		// 開始点と終点について
		a[ 0 ]   = value_type( 1 );
		b[ 0 ]   = value_type( 4 );
		c[ 0 ]   = value_type( 1 );
		p1_[ 0 ] = mul( sub( p[ 1 ], p[ num - 2 ] ), 3 );

		a[ num - 1 ]   = value_type( 1 );
		b[ num - 1 ]   = value_type( 4 );
		c[ num - 1 ]   = value_type( 1 );
		p1_[ num - 1 ] = p1_[ 0 ];

		// 開始点と終点以外の制御点に関して初期化
		for( n = 1 ; n < num - 1 ; n++ )
		{
			a[ n ]   = value_type( 1 );
			b[ n ]   = value_type( 4 );
			c[ n ]   = value_type( 1 );
			p1_[ n ] = mul( sub( p[ n + 1 ], p[ n - 1 ] ), 3 );
		}

		// 一次微係数の計算

		p_type tmp;

		for( n = 1; n < num - 1; n++ )		// 開始点と終点以外に関して
		{
			a[ n ]   = div( a[ n ], b[ n - 1 ] );
			b[ n ]   = sub( b[ n ], mul( a[ n ], c[ n - 1 ] ) );
			p1_[ n ] = sub( p1_[ n ],  mul( a[ n ],  p1[ n - 1 ] ) );
			a[ n ]   = mul( -1, mul( a[ n - 1 ],  a[ n ] ) );
		}

		a[ num - 2 ] = mul( -1, div( c[ num - 2 ], add( a[ num - 2 ], b[ num - 2 ] ) ) );
		b[ num - 2 ] = mul( -1, mul( a[ num - 2 ], div( p1[ num - 2 ], c[ num - 2 ] ) ) );

		value_type tmp( a[ num - 3 ] );
		a[ num - 3 ] = mul( -1, div( mul( add( a[ num - 3 ], c[ num - 3 ] ), a[ num - 2 ] ), b[ num - 3 ] ) );
		b[ num - 3 ] = div( sub( p1[ num - 3 ], mul( add( tmp, c[ num - 2 ] ), b[ num - 2 ] ), b[ num - 3 ] ) );

		for( n = num - 4 ; n >= 0 ; n-- )
		{
			value_type tmp( a[ n ] );
			a[ n ] = div( mul( -1, add( mul( c[ n ], a[ n + 1 ] ), mul( a[ n ], a[ num - 2 ] ) ) ), b[ n ] );
			b[ n ] = div( sub( sub( p1[ n ], mul( c[ n ], b[ n + 1 ] ) ), mul( tmp, b[ num - 2 ] ) ), b[ n ] );
		}

		p1_[ 0 ] = div( b[ 0 ], add( a[ 0 ], 1 ) );

		for( n = 1 ; n < num - 1 ; n++ )
		{
			p1_[ n ] = add( mul( a[ n ], p1[ 0 ] ), b[ n ] );
		}
		p1_[ num - 1 ] = p1[ 0 ];

		delete [] a;
		delete [] b;
		delete [] c;
	}

public:
	/// @brief スプラインを描画する前に必ず呼び出す
	void construct_spline( )
	{
		closed_spline( );
	}

	/// @brief 指定された位置（区間０～１）に対応する，3次スプライン補間結果を返す
	//!
	//! @param[in] t  … 全体の曲線を区間０～１とした時に，補間される点の位置
	//!
	//! @return 指定された点の位置に対応する3次スプライン曲線の補間結果
	//!
	value_type operator( )( double t )
	{
		if( t < 0.0 )
		{
			t = 0.0;
		}
		else if( t > 1.0 )
		{
			t = 1.0;
		}

		size_type n = static_cast< size_type >( t * static_cast< double >( p1_.size( ) ) );
		double step = 1.0 / static_cast< double >( p1_.size( ) );

		if( t == 1.0 )
		{
			n--;
		}

		const base &p = *this;

		value_type a3 = add( mul( sub( p[ n ], p[ n + 1 ] ), 2 ), add( p1[ n ], p1[ n + 1 ] ) );
		value_type a2 = sub( add( mul( sub( p[ n ], p[ n + 1 ] ), -3 ), mul( p1_[ n ], -2 ) ), p1_[ n + 1 ] );
		value_type a1 = p1_[ n ];
		value_type a0 = p[ n ];

		double s1 = t / step - static_cast< double >( n );
		double s2 = s * s;
		double s3 = s2 * s;

		return( add( add( add( mul( a3, s3 ), mul( a2, s2 ) ), mul( a1, s1 ) ), a0 ) );
	}


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
	spline( const spline &b ) : base( b ), mode_( b.mode_ ){ }

	/// @brief デフォルトのコンストラクタ
	//!
	//! デフォルト値を「最初と最後を通る曲線」に設定する
	//!
	spline( ) : mode_( THROUGH ){ }
};

/// @}
//  3次スプライングループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_SPLINE__
