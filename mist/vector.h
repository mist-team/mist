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

/// @file mist/vector.h
//!
//! @brief 各次元に対応したベクトル演算を行うためのライブラリ
//!
#ifndef __INCLUDE_MIST_VECTOR__
#define __INCLUDE_MIST_VECTOR__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN


//! @addtogroup vector_group ベクトルライブラリ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/vector.h>
//! @endcode
//!
//!  @{



/// @brief 3次元ベクトルを扱うクラス
//! 
//! ベクトルの内積・外積等を簡便に扱うためのクラス
//! 
//! @param T … ベクトル内に各座標を表すデータ型
//! 
template < class T >
class vector3
{
public:
	typedef T value_type;										///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef size_t size_type;									///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;							///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename float_type< T >::value_type float_type;	///< @brief 長さなどを計算するときに用いる浮動小数点型

public:
	value_type x;		///< @brief X座標値
	value_type y;		///< @brief Y座標値
	value_type z;		///< @brief Z座標値

	/// @brief デフォルトコンストラクタ．( 0, 0, 0 ) に初期化する
	vector3( ) : x( 0 ), y( 0 ), z( 0 ){ }

	/// @brief ( xx, yy, zz ) のベクトルを作成する
	vector3( const value_type &xx, const value_type &yy, const value_type &zz ) : x( xx ), y( yy ), z( zz ){ }

	/// @brief ( vv, vv, vv ) のベクトルを作成する
	explicit vector3( const value_type &vv ) : x( vv ), y( vv ), z( vv ){ }


	/// @brief 他の3次元ベクトルで要素の型が異なるものから同じ要素を持つベクトルを作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT >
	vector3( const vector3< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }


	/// @brief 他の3次元ベクトルで同じ要素型のものを用いて初期化する
	vector3( const vector3< T > &v ) : x( v.x ), y( v.y ), z( v.z ){ }


	/// @brief 他データ型の3次元ベクトルを代入する
	//! 
	//! コピー元であるベクトル v と全く同じベクトルを作成する．
	//! 
	//! @param[in] v … コピー元のベクトル
	//! 
	//! @return 自分自身
	//! 
	template < class TT >
	const vector3 &operator =( const vector3< TT > &v )
	{
		x = static_cast< value_type >( v.x );
		y = static_cast< value_type >( v.y );
		z = static_cast< value_type >( v.z );
		return ( *this );
	}

	/// @brief 他の3次元ベクトルを代入する
	//! 
	//! コピー元であるベクトル v と全く同じベクトルを作成する．
	//! 
	//! @param[in] v … コピー元のベクトル
	//! 
	//! @return 自分自身
	//! 
	const vector3 &operator =( const vector3< T > &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		return ( *this );
	}


	/// @brief 符号反転したベクトルを返す
	vector3 operator -( ) const { return ( vector3( -x, -y, -z ) ); }


	/// @brief ベクトル和
	template < class TT >
	vector3 &operator +=( const vector3< TT > &v )
	{
		x = static_cast< value_type >( x + v.x );
		y = static_cast< value_type >( y + v.y );
		z = static_cast< value_type >( z + v.z );
		return( *this );
	}

	/// @brief ベクトル差
	template < class TT >
	vector3 &operator -=( const vector3< TT > &v )
	{
		x = static_cast< value_type >( x - v.x );
		y = static_cast< value_type >( y - v.y );
		z = static_cast< value_type >( z - v.z );
		return( *this );
	}

	/// @brief ベクトルの外積
	template < class TT >
	vector3 &operator *=( const vector3< TT > &v )
	{
		value_type xx = static_cast< value_type >( y * v.z - z * v.y );
		value_type yy = static_cast< value_type >( z * v.x - x * v.z );
		value_type zz = static_cast< value_type >( x * v.y - y * v.x );
		x = xx;
		y = yy;
		z = zz;
		return( *this );
	}

	/// @brief ベクトルの定数倍
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector3 &operator *=( const double &a )
#else
	template < class TT >
	vector3 &operator *=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x * a );
		y = static_cast< value_type >( y * a );
		z = static_cast< value_type >( z * a );
		return( *this );
	}


	/// @brief ベクトルを定数で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector3 &operator /=( const double &a )
#else
	template < class TT >
	vector3 &operator /=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x / a );
		y = static_cast< value_type >( y / a );
		z = static_cast< value_type >( z / a );
		return( *this );
	}


	/// @brief 2つのベクトルが等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_x = q_x \; \wedge \; p_y = q_y \; \wedge \; p_z = q_z \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … 2つのベクトルが等しい場合
	//! @retval false … 2つのベクトルが異なる場合
	//! 
	bool operator ==( const vector3 &v ) const { return( x == v.x && y == v.y && z == v.z ); }

	/// @brief 2つのベクトルが等しくない（どれか１つでも異なる要素を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_x = q_x \; \wedge \; p_y = q_y \; \wedge \; p_z = q_z} \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … 2つのベクトルが異なる場合
	//! @retval false … 2つのベクトルが等しい場合
	//! 
	bool operator !=( const vector3 &v ) const { return( !( *this == v ) ); }

	/// @brief 2つのベクトルの < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z } \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 <  v2 の場合
	//! @retval false … v1 >= v2 の場合
	//! 
	bool operator < ( const vector3 &v ) const
	{
		if( x == v.x )
		{
			if( y == v.y )
			{
				return( z < v.z );
			}
			else
			{
				return( y < v.y );
			}
		}
		else
		{
			return( x < v.x );
		}
	}

	/// @brief 2つのベクトルの <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 <= v2 の場合
	//! @retval false … v1 >  v2 の場合
	//! 
	bool operator <=( const vector3 &v ) const { return( v >= *this ); }

	/// @brief 2つのベクトルの > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z } \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 >  v2 の場合
	//! @retval false … v1 <= v2 の場合
	//! 
	bool operator > ( const vector3 &v ) const { return( v < *this ); }

	/// @brief 2つのベクトルの >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 >= v2 の場合
	//! @retval false … v1 <  v2 の場合
	//! 
	bool operator >=( const vector3 &v ) const { return( !( *this < v ) ); }


	/// @brief 単位ベクトルを計算する
	//! 
	//! \f[ \frac{\mbox{\boldmath v}}{\left\|{\mbox{\boldmath v}}\right\|} \f]
	//! 
	vector3 unit( ) const
	{
		float_type length_ = length( );
		if( length_ > 0 )
		{
			return( vector3( static_cast< value_type >( x / length_ ), static_cast< value_type >( y / length_ ), static_cast< value_type >( z / length_ ) ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief ベクトルの内積を計算する
	//! 
	//! \f[ \mbox{\boldmath p} \cdot \mbox{\boldmath q} = p_x \times q_x + p_y \times q_y + p_z \times q_z \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	template < class TT >
	typename promote_trait< T, TT >::value_type inner( const vector3< TT > &v ) const
	{
		return( static_cast< typename promote_trait< T, TT >::value_type >( x * v.x + y * v.y + z * v.z ) );
	}


	/// @brief ベクトルの外積を計算する
	//! 
	//! \f[ \mbox{\boldmath p} \times \mbox{\boldmath q} = \left( p_y \times q_z - p_z \times q_y \;,\; p_z \times q_x - p_x \times q_z \;,\; p_x \times q_y - p_y \times q_x \right)^T \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	template < class TT >
	vector3< typename promote_trait< value_type, TT >::value_type > outer( const vector3< TT > &v ) const
	{
		return( vector3< typename promote_trait< value_type, TT >::value_type >( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ) );
	}


	/// @brief ベクトルの大きさを計算する
	//! 
	//! \f[ \left\|\mbox{\boldmath v}\right\| = \sqrt{v_x^2 + v_y^2 + v_z^2} \f]
	//! 
	//! @return ベクトルの大きさ
	//! 
	float_type length( ) const { return ( static_cast< float_type >( std::sqrt( static_cast< double >( x * x + y * y + z * z ) ) ) ); }

	/// @brief 任意軸周りのベクトルの回転
	//! 
	//! @param[in] v     … 回転軸
	//! @param[in] theta … 右ねじ方向？で測った回転角度
	//! 
	//! @return 回転後のベクトル
	//! 
	vector3 rotate( const vector3 &v, double theta ) const
	{
		theta *= 3.1415926535897932384626433832795 / 180.0;
		double cs = std::cos( theta ), sn = std::sin( theta );
		value_type xx = static_cast< value_type >( ( v.x * v.x * ( 1.0 - cs ) + cs ) * x + ( v.x * v.y * ( 1.0 - cs ) - v.z * sn ) * y + ( v.x * v.z * ( 1.0 - cs ) + v.y * sn ) * z );
		value_type yy = static_cast< value_type >( ( v.x * v.y * ( 1.0 - cs ) + v.z * sn ) * x + ( v.y * v.y * ( 1.0 - cs ) + cs ) * y + ( v.y * v.z * ( 1.0 - cs ) - v.x * sn ) * z );
		value_type zz = static_cast< value_type >( ( v.x * v.z * ( 1.0 - cs ) - v.y * sn ) * x + ( v.y * v.z * ( 1.0 - cs ) + v.x * sn ) * y + ( v.z * v.z * ( 1.0 - cs ) + cs ) * z );
		return ( vector3( xx, yy, zz ) );
	}
};


/// @brief ベクトルの内積
template < class T1, class T2 >
inline typename promote_trait< T1, T2 >::value_type operator ^( const vector3< T1 > &v1, const vector3< T2 > &v2 )
{
	typedef typename promote_trait< T1, T2 >::value_type value_type;
	return( vector3< value_type >( v1 ).inner( v2 ) );
}

// 型の昇格を行う演算の定義

/// @brief ベクトルの和
DEFINE_PROMOTE_BIND_OPERATOR1( vector3, + )

/// @brief ベクトルの差
DEFINE_PROMOTE_BIND_OPERATOR1( vector3, - )

/// @brief ベクトルの外積
DEFINE_PROMOTE_BIND_OPERATOR1( vector3, * )

/// @brief ベクトルと定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( vector3, * )

/// @brief 定数とベクトルの積
DEFINE_PROMOTE_BIND_OPERATOR3( vector3, * )

/// @brief ベクトルを定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( vector3, / )



/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     v   … 3次元ベクトル
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2, 3 )
//! @endcode
//!
template < class T > inline std::ostream &operator <<( std::ostream &out, const vector3< T > &v )
{
	out << v.x << ", ";
	out << v.y << ", ";
	out << v.z;
	return( out );
}

/// @brief Method for reading the vector comma separated from an istream
//! 
//! @param[in,out] in … input stream
//! @param[out]    v  … 3-dimensional vector
//! 
//! @return the input stream
//! 
//! @code Marco Feuerstein
//! ( 1, 2, 3 )
//! @endcode
//!
template < class T > inline std::istream &operator >>( std::istream &in, vector3< T > &v )
{
	std::string comma;
	in >> v.x >> comma;
	in >> v.y >> comma;
	in >> v.z;
	return( in );
}



/// @brief 2次元ベクトルを扱うクラス
//! 
//! ベクトルの内積・外積等を簡便に扱うためのクラス
//! 
//! @param T … ベクトル内に各座標を表すデータ型
//! 
template < class T >
class vector2
{
public:
	typedef T value_type;										///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef size_t size_type;									///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;							///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename float_type< T >::value_type float_type;	///< @brief 長さなどを計算するときに用いる浮動小数点型

public:
	value_type x;		///< @brief X軸座標値
	value_type y;		///< @brief Y軸座標値

	/// @brief デフォルトコンストラクタ．( 0, 0 ) に初期化する
	vector2( ) : x( 0 ), y( 0 ){ }

	/// @brief ( xx, yy ) のベクトルを作成する
	vector2( const value_type &xx, const value_type &yy ) : x( xx ), y( yy ){ }

	/// @brief ( vv, vv ) のベクトルを作成する
	vector2( const value_type &vv ) : x( vv ), y( vv ){ }


	/// @brief 他の2次元ベクトルで要素の型が異なるものから同じ要素を持つベクトルを作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT >
	vector2( const vector2< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ){ }


	/// @brief 他の2次元ベクトルで同じ要素型のものを用いて初期化する
	vector2( const vector2< T > &v ) : x( v.x ), y( v.y ){ }


	/// @brief 他のデータ型の2次元ベクトルを代入する
	//! 
	//! コピー元であるベクトル v と全く同じベクトルを作成する．
	//! 
	//! @param[in] v … コピー元のベクトル
	//! 
	//! @return 自分自身
	//! 
	template < class TT >
	const vector2 &operator =( const vector2< TT > &v )
	{
		x = static_cast< value_type >( v.x );
		y = static_cast< value_type >( v.y );
		return ( *this );
	}

	/// @brief 他の2次元ベクトルを代入する
	//! 
	//! コピー元であるベクトル v と全く同じベクトルを作成する．
	//! 
	//! @param[in] v … コピー元のベクトル
	//! 
	//! @return 自分自身
	//! 
	const vector2 &operator =( const vector2< T > &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
		}
		return ( *this );
	}


	/// @brief 符号反転したベクトルを返す
	vector2 operator -( ) const { return ( vector2( -x, -y ) ); }


	/// @brief ベクトル和
	template < class TT >
	vector2 &operator +=( const vector2< TT > &v ){ x = static_cast< value_type >( x + v.x ); y = static_cast< value_type >( y + v.y ); return( *this ); }

	/// @brief ベクトル差
	template < class TT >
	vector2 &operator -=( const vector2< TT > &v ){ x = static_cast< value_type >( x - v.x ); y = static_cast< value_type >( y - v.y ); return( *this ); }


	/// @brief ベクトルの定数倍
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector2 &operator *=( const double &a )
#else
	template < class TT >
	vector2 &operator *=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x * a );
		y = static_cast< value_type >( y * a );
		return( *this );
	}


	/// @brief ベクトルを定数で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector2 &operator /=( const double &a )
#else
	template < class TT >
	vector2 &operator /=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x / a );
		y = static_cast< value_type >( y / a );
		return( *this );
	}


	/// @brief 2つのベクトルが等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_x = q_x \; \wedge \; p_y = q_y \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … 2つのベクトルが等しい場合
	//! @retval false … 2つのベクトルが異なる場合
	//! 
	bool operator ==( const vector2 &v ) const { return( x == v.x && y == v.y ); }

	/// @brief 2つのベクトルが等しくない（どれか１つでも異なる要素を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_x = q_x \; \wedge \; p_y = q_y } \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … 2つのベクトルが異なる場合
	//! @retval false … 2つのベクトルが等しい場合
	//! 
	bool operator !=( const vector2 &v ) const { return( !( *this == v ) ); }

	/// @brief 2つのベクトルの < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_x \ge q_x \; \wedge \; p_y \ge q_y } \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 <  v2 の場合
	//! @retval false … v1 >= v2 の場合
	//! 
	bool operator < ( const vector2 &v ) const
	{
		if( x == v.x )
		{
			return( y < v.y );
		}
		else
		{
			return( x < v.x );
		}
	}

	/// @brief 2つのベクトルの <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_x \le q_x \; \wedge \; p_y \le q_y \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 <= v2 の場合
	//! @retval false … v1 >  v2 の場合
	//! 
	bool operator <=( const vector2 &v ) const { return( v >= *this ); }

	/// @brief 2つのベクトルの > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_x \le q_x \; \wedge \; p_y \le q_y } \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 >  v2 の場合
	//! @retval false … v1 <= v2 の場合
	//! 
	bool operator > ( const vector2 &v ) const { return( v < *this ); }

	/// @brief 2つのベクトルの >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_x \ge q_x \; \wedge \; p_y \ge q_y \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @retval true  … v1 >= v2 の場合
	//! @retval false … v1 <  v2 の場合
	//! 
	bool operator >=( const vector2 &v ) const { return( !( *this < v ) ); }


	/// @brief 単位ベクトルを計算する
	//! 
	//! \f[ \frac{\mbox{\boldmath v}}{\left\|\mbox{\boldmath v}\right\|} \f]
	//! 
	vector2 unit( ) const
	{
		float_type length_ = length( );
		if( length_ > 0 )
		{
			return( vector2( static_cast< value_type >( x / length_ ), static_cast< value_type >( y / length_ ) ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief ベクトルの内積を計算する
	//! 
	//! \f[ \mbox{\boldmath p} \cdot \mbox{\boldmath q} = p_x \times q_x + p_y \times q_y \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	template < class TT >
	typename promote_trait< T, TT >::value_type inner( const vector2< TT > &v ) const { return( static_cast< typename promote_trait< T, TT >::value_type >( x * v.x + y * v.y ) ); }


	/// @brief ベクトルの外積を計算する
	//! 
	//! \f[ \mbox{\boldmath p} \times \mbox{\boldmath q} = p_x \times q_y - p_y \times q_x \f]
	//! 
	//! @param[in] v … 右辺値
	//! 
	template < class TT >
	typename promote_trait< T, TT >::value_type outer( const vector2< TT > &v ) const { return( static_cast< typename promote_trait< T, TT >::value_type >( x * v.y - y * v.x ) ); }


	/// @brief ベクトルの大きさを計算する
	//! 
	//! \f[ \left\|\mbox{\boldmath v}\right\| = \sqrt{v_x^2+v_y^2} \f]
	//! 
	//! @return ベクトルの大きさ
	//! 
	float_type length( ) const { return ( static_cast< float_type >( std::sqrt( static_cast< double >( x * x + y * y ) ) ) ); }

	//// ベクトルの回転
	//vector2 rotate( const vector2 &a, double theta ) const
	//{
	//	theta *= 3.1415926535897932384626433832795 / 180.0;
	//	double cs = std::cos( theta ), sn = std::sin( theta );
	//	value_type xx = static_cast< value_type >( ( v.x * v.x * ( 1.0 - cs ) + cs ) * x + ( v.x * v.y * ( 1.0 - cs ) - v.z * sn ) * y + ( v.x * v.z * ( 1.0 - cs ) + v.y * sn ) * z );
	//	value_type yy = static_cast< value_type >( ( v.x * v.y * ( 1.0 - cs ) + v.z * sn ) * x + ( v.y * v.y * ( 1.0 - cs ) + cs ) * y + ( v.y * v.z * ( 1.0 - cs ) - v.x * sn ) * z );
	//	value_type zz = static_cast< value_type >( ( v.x * v.z * ( 1.0 - cs ) - v.y * sn ) * x + ( v.y * v.z * ( 1.0 - cs ) + v.x * sn ) * y + ( v.z * v.z * ( 1.0 - cs ) + cs ) * z );
	//	return ( vector2( xx, yy, zz ) );
	//}

};


/// @brief ベクトルの内積
template < class T1, class T2 >
inline typename promote_trait< T1, T2 >::value_type operator ^( const vector2< T1 > &v1, const vector2< T2 > &v2 )
{
	typedef typename promote_trait< T1, T2 >::value_type value_type;
	return( vector2< value_type >( v1 ).inner( v2 ) );
}

/// @brief ベクトルの外積
template < class T1, class T2 >
inline typename promote_trait< T1, T2 >::value_type operator *( const vector2< T1 > &v1, const vector2< T2 > &v2 )
{
	typedef typename promote_trait< T1, T2 >::value_type value_type;
	return( vector2< value_type >( v1 ).outer( v2 ) );
}


// 型の昇格を行う演算の定義
/// @brief ベクトルの和
DEFINE_PROMOTE_BIND_OPERATOR1( vector2, + )

/// @brief ベクトルの差
DEFINE_PROMOTE_BIND_OPERATOR1( vector2, - )

/// @brief ベクトルと定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( vector2, * )

/// @brief 定数とベクトルの積
DEFINE_PROMOTE_BIND_OPERATOR3( vector2, * )

/// @brief ベクトルを定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( vector2, / )


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     v   … 2次元ベクトル
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2 )
//! @endcode
//!
template < class T > inline std::ostream &operator <<( std::ostream &out, const vector2< T > &v )
{
	out << v.x << ", ";
	out << v.y;
	return( out );
}

/// @brief Method for reading the vector comma separated from an istream
//! 
//! @param[in,out] in … input stream
//! @param[out]    v  … 2-dimensional vector
//! 
//! @return the input stream
//! 
//! @code Marco Feuerstein
//! ( 1, 2 )
//! @endcode
//!
template < class T > inline std::istream &operator >>( std::istream &in, vector2< T > &v )
{
	std::string comma;
	in >> v.x >> comma;
	in >> v.y;
	return( in );
}



#if defined( _MIST_VECTOR_SUPPORT_ ) && _MIST_VECTOR_SUPPORT_ != 0


/// @brief N次元ベクトルを扱うクラス
//! 
//! - _USE_EXPRESSION_TEMPLATE_  マクロが1の場合は，Expression Template を利用して，行列・ベクトル演算を高速化する
//! - _CHECK_MATRIX_OPERATION_   マクロが1の場合は，行列・ベクトル演算の左辺と右辺でベクトルのサイズが適切であるかをチェックする
//! - _CHECK_ACCESS_VIOLATION_   マクロが1の場合は，ベクトルの要素へアクセスする際に，範囲外かどうかを判定する
//! 
//! @param T         … 行列内に格納するデータ型
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < class T, class Allocator = ::std::allocator< T > >
class vector : public matrix< T, Allocator >
{
private:
	typedef matrix< T, Allocator > base;		///< @brief 基底クラスを現す行列クラス

public:
	typedef typename base::allocator_type allocator_type;		///< @brief MISTコンテナが利用するアロケータ型
	typedef typename base::reference reference;					///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename base::const_reference const_reference;		///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename base::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename base::size_type size_type;					///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename base::pointer pointer;						///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename base::const_pointer const_pointer;			///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	typedef typename base::iterator iterator;								///< @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename base::const_iterator const_iterator;					///< @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版

	/// @brief データ型の変換を行う
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef vector< TT, AAllocator > other;
	};


	//template < class TT, class AAlocator >
	//const vector& operator +=( const vector< TT, AAlocator > &v2 )
	//{
	//	base::operator +=( v2 );
	//	return( v1 );
	//}

	//template < class TT, class AAlocator >
	//const vector& operator -=( const vector< TT, AAlocator > &v2 )
	//{
	//	base::operator -=( v2 );
	//	return( v1 );
	//}


	/// @brief ベクトルの外積
	//! 
	//! @attention 外積を計算する2つのベクトルの次元が等しくなくてはならない
	//! 
	//! @param[in] v2 … 右辺値
	//! 
	//! @return 自分自身
	//! 
	template < class TT, class AAlocator >
	const vector& operator *=( const vector< TT, AAlocator > &v2 )
	{
#if defined( _CHECK_MATRIX_OPERATION_ ) && _CHECK_MATRIX_OPERATION_ != 0
		if( base::size( ) != v2.size( ) || base::size( ) < 3 )
		{
			// 外積の計算ができません例外
			::std::cerr << "can't calculate outer product of two vectors." << ::std::endl;
			return( *this );
		}
#endif

		typedef typename vector< T, Allocator >::size_type size_type;
		vector &v1 = *this;
		vector v( v1.size( ) );

		size_type i;
		for( i = 0 ; i < v.size( ) - 2 ; i++ )
		{
			v[ i ] = v1[ i + 1 ] * v2[ i + 2 ] - v1[ i + 2 ] * v2[ i + 1 ];
		}

		v[ i ] = v1[ i + 1 ] * v2[ 0 ] - v1[ 0 ] * v2[ i + 1 ];
		v[ i + 1 ] = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];

		v1.swap( v );

		return( *this );
	}


	/// @brief ベクトルの内積
	//! 
	//! @attention 内積を計算する2つのベクトルの次元が等しくなくてはならない
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @return 内積結果のスカラー値
	//! 
	value_type operator ^( const vector &v ) const { return( inner( v ) ); }


	/// @brief ベクトルの内積
	//! 
	//! @attention 内積を計算する2つのベクトルの次元が等しくなくてはならない
	//! 
	//! @param[in] v2 … 右辺値
	//! 
	//! @return 内積結果のスカラー値
	//! 
	value_type inner( const vector &v2 ) const
	{
#if defined( _CHECK_MATRIX_OPERATION_ ) && _CHECK_MATRIX_OPERATION_ != 0
		if( base::size( ) != v2.size( ) )
		{
			// 内積の計算ができません例外
			::std::cerr << "can't calculate inner product of two vectors." << ::std::endl;
			return( value_type( 0 ) );
		}
#endif

		typedef typename vector< T, Allocator >::size_type size_type;
		const vector &v1 = *this;
		value_type v = value_type( 0 );
		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			v += v1[ i ] * v2[ i ];
		}

		return( v );
	}


	/// @brief ベクトルの外積
	//! 
	//! @attention 外積を計算する2つのベクトルの次元が等しくなくてはならない
	//! 
	//! @param[in] v … 右辺値
	//! 
	//! @return 自分自身
	//! 
	vector outer( const vector &v ) const
	{
		return( vector( *this ) *= v );
	}


public:
	/// @brief ディフォルトコンストラクタ．0 次元のベクトルを作成する
	vector( ) : base( ) {}

	/// @brief アロケータ a のコピーを利用し，0 次元のベクトルを作成する
	explicit vector( const Allocator &a ) : base( a ) {}


	/// @brief num 次元のベクトルを作成し，デフォルト値で要素を初期化する
	vector( size_type num ) : base( num, 1 ) {}

	/// @brief num 次元で，アロケータ a のコピーを利用したベクトルを作成し，デフォルト値で要素を初期化する
	vector( size_type num, const Allocator &a ) : base( num, 1, a ) {}


	/// @brief num 次元のベクトルを作成し，全要素を値 val で要素を初期化する
	vector( size_type num, const T &val ) : base( num, 1, val ) {}

	/// @brief num 次元で，アロケータ a のコピーを利用したベクトルを作成し，全要素を値 val で要素を初期化する
	vector( size_type num, const T &val, const Allocator &a ) : base( num, 1, val, a ) {}


	/// @brief ptr が指すメモリ領域に，num 次元のベクトルを作成を作成する（ptr が指す先の利用可能なメモリ量は mem_available ）
	vector( size_type num, pointer ptr, size_type mem_available ) : base( num, 1, ptr, mem_available ) {}

	/// @brief ptr が指すメモリ領域に，num 次元のベクトルを作成を作成し，全要素を値 val で要素を初期化する（ptr が指す先の利用可能なメモリ量は mem_available ）
	vector( size_type num, const T &val, pointer ptr, size_type mem_available ) : base( num, 1, val, ptr, mem_available ) {}


	/// @brief 他のベクトルで要素の型が異なるものから同じサイズのベクトルを作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	vector( const vector< TT, AAlocator > &o ) : base( o ){ }


	/// @brief 他のベクトルで同じ要素型のものを用いて初期化する
	vector( const vector< T, Allocator > &o ) : base( o ){ }

#if _USE_EXPRESSION_TEMPLATE_ != 0
	/// @brief 複数の行列・ベクトル演算の結果を用いてベクトルを初期化する
	template < class Expression >
	vector( const matrix_expression< Expression > &expression ) : base( expression.size( ), 1 )
	{
		vector &v = *this;
		for( size_type indx = 0 ; indx < v.size( ) ; indx++ )
		{
			v[ indx ] = expression[ indx ];
		}
	}
#endif
};



/// @brief 指定されたストリームに，行列内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     v   … 出力する行列
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 2, 3, 4
//! @endcode
//! 
template < class T, class Allocator >
inline ::std::ostream &operator <<( ::std::ostream &out, const vector< T, Allocator > &v )
{
	typename vector< T, Allocator >::size_type indx;
	for( indx = 0 ; indx < v.size( ) ; indx++ )
	{
		out << v[ indx ];
		if( indx != v.size( ) - 1 ) out << ", ";
	}

	return( out );
}

#if 0
#else

/// @brief ベクトルの外積を計算する
//! 
//! @attention 外積を計算する2つのベクトルの次元が等しくなくてはならない
//! 
//! @param[in] v1 … 左辺値ベクトル
//! @param[in] v2 … 右辺値ベクトル
//! 
//! @return 外積結果
//! 
template < class T, class Allocator >
inline vector< T, Allocator > operator *( const vector< T, Allocator > &v1, const vector< T, Allocator > &v2 )
{
	return( vector< T, Allocator >( v1 ) *= v2 );
}

#endif



#endif // _MIST_VECTOR_SUPPORT_

/// @}
//  ベクトルライブラリグループの終わり


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_VECTOR__
