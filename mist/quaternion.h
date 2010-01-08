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

/// @file mist/quaternion.h
//!
//! @brief クォータニオン（四元数）を扱うためのライブラリ
//!
#ifndef __INCLUDE_MIST_QUATERNION__
#define __INCLUDE_MIST_QUATERNION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN


/// @brief クォータニオン（四元数）を扱うクラス
//! 
//! @param T … 内部で用いるデータ型
//! 
template < class T >
class quaternion
{
public:
	typedef T value_type;										///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef size_t size_type;									///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;							///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename float_type< T >::value_type float_type;	///< @brief 長さなどを計算するときに用いる浮動小数点型

public:
	value_type w;		///< @brief 実数成分
	value_type x;		///< @brief 虚数成分1
	value_type y;		///< @brief 虚数成分2
	value_type z;		///< @brief 虚数成分3

	/// @brief デフォルトコンストラクタ（全要素を0で初期化する）
	quaternion( ) : w( 0 ), x( 0 ), y( 0 ), z( 0 ){ }

	/// @brief ww，xx，yy，zz の値を用いて初期化する
	quaternion( const value_type &ww, const value_type &xx, const value_type &yy, const value_type &zz ) : w( ww ), x( xx ), y( yy ), z( zz ){ }

	/// @brief ww，xx，yy，zz の値を用いて初期化する
	explicit quaternion( const value_type &ww ) : w( ww ), x( 0 ), y( 0 ), z( 0 ){ }

	/// @brief 他のクォータニオンオブジェクト（データ型が異なる）を用いて初期化する
	template < class TT >
	quaternion( const quaternion< TT > &q ) : w( static_cast< value_type >( q.w ) ), x( static_cast< value_type >( q.x ) ), y( static_cast< value_type >( q.y ) ), z( static_cast< value_type >( q.z ) ){ }

	/// @brief 他のクォータニオンオブジェクト（データ型が同じ）を用いて初期化する
	quaternion( const quaternion< T > &q ) : w( q.w ), x( q.x ), y( q.y ), z( q.z ){ }


	/// @brief 実数成分 ww と虚数成分のベクトル v を用いて初期化する
	template < class TT >
	quaternion( value_type ww, const vector3< TT > &v ) : w( ww ), x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }


	/// @brief クォータニオンを用いた任意軸周りの回転
	//! 
	//! @attention 右手系の場合は右ねじ回転，左手系の場合は左ねじ回転となるので注意！！
	//! @attention 回転角度の単位は度を用いる（ラジアンではないので注意！！）
	//! 
	//! @param[in] axis  … 回転軸
	//! @param[in] theta … 回転角度
	//! 
	template < class TT >
	quaternion( const vector3< TT > &axis, value_type theta )
	{
		double t = theta * 3.1415926535897932384626433832795 / 180.0 / 2.0;
		double c = std::cos( t );
		double s = std::sin( t );
		w = static_cast< value_type >( c );
		x = static_cast< value_type >( s * axis.x );
		y = static_cast< value_type >( s * axis.y );
		z = static_cast< value_type >( s * axis.z );
	}


	/// @brief カメラの視線方向と上向き方向を用いて，カメラの姿勢を表すクォータニオンを計算する
	//! 
	//! @param[in] dir … 回転前のベクトル
	//! @param[in] up  … 回転後のベクトル
	//! 
	template < class TT >
	quaternion( vector3< TT > dir, vector3< TT > up )
	{
		// ワールド座標の単位ベクトル
		vector3< TT > e2( 0, 1, 0 );
		vector3< TT > e3( 0, 0, 1 );

		// 単位ベクトルにする
		dir = dir.unit( );
		up  = up.unit( );

		// 視線方向を合わせるクォータニオンを作成
		quaternion q1 = quaternion::rotate( e3, dir );

		// Y軸を回転させる
		e2 = q1.rotate( e2 );

		// 上向き方向を合わせるクォータニオンを作成
		quaternion q2 = quaternion::rotate( e2, up, dir );

		// 回転を合成する
		operator =( q2 * q1 );
	}


	/// @brief 他のクォータニオンオブジェクトを代入する
	template < class TT >
	const quaternion &operator =( const quaternion< TT > &q )
	{
		w = static_cast< value_type >( q.w );
		x = static_cast< value_type >( q.x );
		y = static_cast< value_type >( q.y );
		z = static_cast< value_type >( q.z );
		return ( *this );
	}

	/// @brief 他のクォータニオンオブジェクトを代入する
	const quaternion &operator =( const quaternion< T > &q )
	{
		if( &q != this )
		{
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
		}
		return ( *this );
	}

	/// @brief 符号反転したクォータニオンを返す
	quaternion operator -( ) const { return ( quaternion( -w, -x, -y, -z ) ); }

	/// @brief クォータニオンの足し算
	//! 
	//! \f[ \mbox{\boldmath p} + \mbox{\boldmath q} = \left( p_w + q_w \;,\; p_x + q_x \;,\; p_y + q_y \;,\; p_z + q_z \right)^T \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @return 足し算結果
	//! 
	template< class TT >
	const quaternion &operator +=( const quaternion< TT > &q )
	{
		w = static_cast< value_type >( w + q.w );
		x = static_cast< value_type >( x + q.x );
		y = static_cast< value_type >( y + q.y );
		z = static_cast< value_type >( z + q.z );
		return( *this );
	}

	/// @brief クォータニオンへの実数成分の足し算
	//! 
	//! \f[ \mbox{\boldmath p} + a = \left( p_w + a \;,\; p_x \;,\; p_y \;,\; p_z \right)^T \f]
	//! 
	//! @param[in] a … 実数成分
	//! 
	//! @return 足し算結果
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator +=( const double &a )
#else
	template < class TT >
	const quaternion &operator +=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w + a );
		return( *this );
	}

	/// @brief クォータニオンの引き算
	//! 
	//! \f[ \mbox{\boldmath p} - \mbox{\boldmath q} = \left( p_w - q_w \;,\; p_x - q_x \;,\; p_y - q_y \;,\; p_z - q_z \right)^T \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @return 引き算結果
	//! 
	template< class TT >
	const quaternion &operator -=( const quaternion< TT > &q )
	{
		w = static_cast< value_type >( w - q.w );
		x = static_cast< value_type >( x - q.x );
		y = static_cast< value_type >( y - q.y );
		z = static_cast< value_type >( z - q.z );
		return( *this );
	}

	/// @brief クォータニオンへの実数成分の引き算
	//! 
	//! \f[ \mbox{\boldmath p} - a = \left( p_w - a \;,\; p_x \;,\; p_y \;,\; p_z \right)^T \f]
	//! 
	//! @param[in] a … 実数成分
	//! 
	//! @return 引き算結果
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator -=( const double &a )
#else
	template < class TT >
	const quaternion &operator -=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w - a );
		return( *this );
	}

	/** @brief クォータニオンの掛け算
	 *  
	 *  \f[
	 *      \mbox{\boldmath p} \times \mbox{\boldmath q} =
	 *          \left(
	 *             p_w \times q_w - p_x \times q_x - p_y \times q_y - p_z \times q_z \;,\;
	 *             p_w \times q_x + p_x \times q_w + p_y \times q_z - p_z \times q_y \;,\;
	 *             p_w \times q_y + p_y \times q_w + p_z \times q_x - p_x \times q_z \;,\;
	 *             p_w \times q_z + p_z \times q_w + p_x \times q_y - p_y \times q_x
	 *          \right)^T
	 *  \f]
	 *  
	 *  @param[in] q … 右辺値
	 *  
	 *  @return 掛け算結果
	 */
	template < class TT >
	const quaternion &operator *=( const quaternion< TT > &q )
	{
		value_type ww = static_cast< value_type >( w * q.w - x * q.x - y * q.y - z * q.z );
		value_type xx = static_cast< value_type >( w * q.x + x * q.w + y * q.z - z * q.y );
		value_type yy = static_cast< value_type >( w * q.y + y * q.w + z * q.x - x * q.z );
		value_type zz = static_cast< value_type >( w * q.z + z * q.w + x * q.y - y * q.x );
		w = ww;
		x = xx;
		y = yy;
		z = zz;
		return( *this );
	}

	/// @brief クォータニオンへの実数成分の掛け算
	//! 
	//! \f[ \mbox{\boldmath p} \times a = \left( p_w \times a \;,\; p_x \times a \;,\; p_y \times a \;,\; p_z \times a \right)^T \f]
	//! 
	//! @param[in] a … 実数成分
	//! 
	//! @return 掛け算結果
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator *=( const double &a )
#else
	template < class TT >
	const quaternion &operator *=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w * a );
		x = static_cast< value_type >( x * a );
		y = static_cast< value_type >( y * a );
		z = static_cast< value_type >( z * a );
		return( *this );
	}

	/// @brief クォータニオンの割り算
	//! 
	//! \f[ \frac{ \mbox{\boldmath p} }{ \mbox{\boldmath q} } = \mbox{\boldmath p} \times \mbox{\boldmath q}^{-1} \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @return 掛け算結果
	//! 
	template < class TT >
	const quaternion &operator /=( const quaternion< TT > &q )
	{
		return( this->operator *=( q.inv( ) ) );
	}

	/// @brief クォータニオンを実数成分で割る
	//! 
	//! \f[ \mbox{\boldmath p} \div a = \left( p_w \div a \;,\; p_x \div a \;,\; p_y \div a \;,\; p_z \div a \right)^T \f]
	//! 
	//! @param[in] a … 実数成分
	//! 
	//! @return 掛け算結果
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator /=( const double &a )
#else
	template < class TT >
	const quaternion &operator /=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w / a );
		x = static_cast< value_type >( x / a );
		y = static_cast< value_type >( y / a );
		z = static_cast< value_type >( z / a );
		return( *this );
	}


	/// @brief 2つのクォータニオンが同一かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} == \mbox{\boldmath q} \rightarrow p_w == q_w \; \wedge \; p_x == q_x \; \wedge \; p_y == q_y \; \wedge \; p_z == q_z \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @retval true  … 全ての要素が等しい場合
	//! @retval false … どれか1つでも等しくない場合
	//! 
	bool operator ==( const quaternion &q ) const { return( w == q.w && x == q.x && y == q.y && z == q.z ); }

	/// @brief 2つのクォータニオンが等しくないどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_w = q_w \; \wedge \; p_x = q_x \; \wedge \; p_y = q_y \; \wedge \; p_z = q_z } \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @retval true  … どれか1つでも等しくない場合
	//! @retval false … 全ての要素が等しい場合
	//! 
	bool operator !=( const quaternion &q ) const { return( !( *this == q ) ); }

	/// @brief 2つのベクトルの < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} < \mbox{\boldmath q} \rightarrow \overline{ p_w \ge q_w \; \wedge \; p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z } \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @retval true  … p <  q の場合
	//! @retval false … p >= q の場合
	//! 
	bool operator < ( const quaternion &q ) const
	{
		if( w == q.w )
		{
			if( x == q.x )
			{
				if( y == q.y )
				{
					return( z < q.z );
				}
				else
				{
					return( y < q.y );
				}
			}
			else
			{
				return( x < q.x );
			}
		}
		else
		{
			return( w < q.w );
		}
	}

	/// @brief 2つのベクトルの <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_w \le q_w \; \wedge \; p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @retval true  … p <= q の場合
	//! @retval false … p >  q の場合
	//! 
	bool operator <=( const quaternion &q ) const { return( q >= *this ); }

	/// @brief 2つのベクトルの > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} > \mbox{\boldmath q} \rightarrow \overline{ p_w \le q_w \; \wedge \; p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z } \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @retval true  … p >  q の場合
	//! @retval false … p <= q の場合
	//! 
	bool operator > ( const quaternion &q ) const { return( q < *this ); }

	/// @brief 2つのベクトルの >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_w \ge q_w \; \wedge \; p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z \f]
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! @retval true  … p >= q の場合
	//! @retval false … p <  q の場合
	//! 
	bool operator >=( const quaternion &q ) const { return( !( *this < q ) ); }


public:	// その他の関数

	/// @brief 共役クォータニオン
	//! 
	//! \f[ \overline{ \mbox{\boldmath p} } = \left( p_w \;,\; -p_x \;,\; -p_y \;,\; -p_z \right)^T \f]
	//! 
	const quaternion conjugate( ) const 
	{
		return( quaternion( w, -x, -y, -z ) );
	}

	/// @brief 逆クォータニオン
	//! 
	//! \f[ \mbox{\boldmath p}^{-1} = \frac{ \overline{ \mbox{\boldmath p} } }{ \left\| \mbox{\boldmath p} \right\|^2 } \f]
	//! 
	const quaternion inv( ) const
	{
		float_type length_ = length( );
		return( conjugate( ) / ( length_ * length_ ) );
	}

	/// @brief 単位クォータニオン
	//! 
	//! \f[ \frac{ \mbox{\boldmath p} }{ \left\| \mbox{\boldmath p} \right\|^2 } \f]
	//! 
	const quaternion unit( ) const
	{
		float_type length_ = length( );
		if( length_ > 0 )
		{
			return( quaternion( static_cast< value_type >( w / length_ ), static_cast< value_type >( x / length_ ), static_cast< value_type >( y / length_ ), static_cast< value_type >( z / length_ ) ) );
		}
		else
		{
			return( *this );
		}
	}

	/// @brief クォータニオンの内積
	//! 
	//! @param[in] q … 右辺値
	//! 
	//! \f[ p_w \times q_w + p_x \times q_x + p_y \times q_y + p_z \times q_z \f]
	//! 
	template < class TT >
	value_type inner( const quaternion< TT > &q ) const
	{
		return( w * q.w + x * q.x + y * q.y + z * q.z );
	}

	/// @brief クォータニオンのノルム
	//! 
	//! \f[ \left\| \mbox{\boldmath p} \right\| = \sqrt{ p_w^2 + p_x^2 + p_y^2 + p_z^2 } \f]
	//! 
	float_type length( ) const { return ( static_cast< float_type >( std::sqrt( static_cast< double >( w * w + x * x + y * y + z * z ) ) ) ); }


	/// @brief クォータニオンを用いたベクトルの回転
	//! 
	//! @param[in] v … 回転されるベクトル
	//! 
	//! @return 回転後のベクトル
	//! 
	template < class TT >
	const vector3< TT > rotate( const vector3< TT > &v ) const
	{
		quaternion q = ( *this ) * quaternion( 0, static_cast< value_type >( v.x ), static_cast< value_type >( v.y ), static_cast< value_type >( v.z ) ) * inv( );
		return( vector3< TT >( static_cast< TT >( q.x ), static_cast< TT >( q.y ), static_cast< TT >( q.z ) ) );
	}



	/// @brief ベクトル1からベクトル2への回転を表すクォータニオンを作成する
	//! 
	//! @param[in] v1 … 回転前のベクトル
	//! @param[in] v2 … 回転後のベクトル
	//! 
	//! @return 回転を表すクォータニオン
	//! 
	template < class TT >
	static quaternion rotate( vector3< TT > v1, vector3< TT > v2 )
	{
		// 単位ベクトルにする
		v1 = v1.unit( );
		v2 = v2.unit( );

		// 回転角度を計算する
		double dot = v1.inner( v2 );
		if( dot < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		double c = std::sqrt( ( dot + 1.0 ) * 0.5 );

		if( std::abs( c - 1.0 ) < 1.0e-6 || c > 1.0 )
		{
			return( quaternion( 1, 0, 0, 0 ) );
		}
		else if( std::abs( c + 1.0 ) < 1.0e-6 || c < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		return( quaternion( value_type( c ), std::sqrt( 1.0 - c * c ) * v1.outer( v2 ).unit( ) ) );
	}

	/// @brief 指定した回転軸を用いてベクトル1からベクトル2への回転を表すクォータニオンを作成する
	//! 
	//! @param[in] v1   … 回転前のベクトル
	//! @param[in] v2   … 回転後のベクトル
	//! @param[in] axis … 回転軸ベクトル
	//! 
	//! @return 回転を表すクォータニオン
	//! 
	template < class TT >
	static quaternion rotate( vector3< TT > v1, vector3< TT > v2, const vector3< TT > &axis )
	{
		// 単位ベクトルにする
		v1 = v1.unit( );
		v2 = v2.unit( );

		// 回転角度を計算する
		double dot = v1.inner( v2 );
		if( dot < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		double c = std::sqrt( ( dot + 1.0 ) * 0.5 );

		if( std::abs( c - 1.0 ) < 1.0e-6 || c > 1.0 )
		{
			return( quaternion( 1, 0, 0, 0 ) );
		}
		else if( std::abs( c + 1.0 ) < 1.0e-6 || c < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		double s = std::sqrt( 1.0 - c * c );

		if( axis.inner( v1.outer( v2 ) ) < 0.0 )
		{
			s = -s;
		}

		return( quaternion( value_type( c ), s * axis ) );
	}
};

// 型の昇格を行う演算の定義

/// @brief クォータニオンの和
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, + )

/// @brief クォータニオンと定数の和
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, + )

/// @brief 定数とクォータニオンの和
DEFINE_PROMOTE_BIND_OPERATOR3( quaternion, + )

/// @brief クォータニオンの差
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, - )

/// @brief クォータニオンと定数の差
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, - )

/// @brief 定数とクォータニオンの差
DEFINE_PROMOTE_BIND_OPERATOR4( quaternion, - )

/// @brief クォータニオンの積
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, * )

/// @brief クォータニオンと定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, * )

/// @brief 定数とクォータニオンの積
DEFINE_PROMOTE_BIND_OPERATOR3( quaternion, * )

/// @brief クォータニオンの割り算
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, / )

/// @brief クォータニオンを定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, / )






/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     q   … クォータニオン
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2, 3, 4 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const quaternion< T > &q )
{
	out << "( ";
	out << q.w << ", ";
	out << q.x << ", ";
	out << q.y << ", ";
	out << q.z << " )";
	return( out );
}



/// @brief 球面線形補間を行う
//! 
//! @param[in] q1 … 補間もとのクォータニオン1
//! @param[in] q2 … 補間もとのクォータニオン2
//! @param[in] t  … [0,1]の間の数値で，補間点
//! 
//! @return 球面線形補間されたクォータニオン
//! 
template < class T1, class T2 >
const quaternion< double > interpolate( const quaternion< T1 > &q1, const quaternion< T2 > &q2, double t )
{
	typedef quaternion< double > quaternion_type;

	quaternion_type Q1( q1.unit( ) );
	quaternion_type Q2( q2.unit( ) );

	double dot = Q1.inner( Q2 );

	if( std::abs( dot ) < 1.0e-6 )
	{
		return( Q1 );
	}
	else if( dot < 0.0 )
	{
		double theta = std::acos( dot );

		// 球面線形補間を行う
		return( quaternion_type( Q1 * std::sin( theta * ( 1.0 - t ) ) - Q2 * std::sin( theta * t ) ).unit( ) );
	}
	else
	{
		double theta = std::acos( dot );

		// 球面線形補間を行う
		return( quaternion_type( Q1 * std::sin( theta * ( 1.0 - t ) ) + Q2 * std::sin( theta * t ) ).unit( ) );
	}
}


// クォータニオンから行列へ変換する
//template < class T >
//matrix< T > rotate_matrix( const quaternion< T > &q )
//{
//	matrix< T > mat(  4, 4  );
//	mat( 0, 0 ) = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
//	mat( 0, 1 ) = 2 * ( q.x * q.y - q.w * q.z );
//	mat( 0, 2 ) = 2 * ( q.x * q.z + q.w * q.y );
//	mat( 0, 3 ) = 0;
//	mat( 1, 0 ) = 2 * ( q.x * q.y + q.w * q.z );
//	mat( 1, 1 ) = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
//	mat( 1, 2 ) = 2 * ( q.y * q.z - q.w * q.x );
//	mat( 1, 3 ) = 0;
//	mat( 2, 0 ) = 2 * ( q.x * q.z - q.w * q.y );
//	mat( 2, 1 ) = 2 * ( q.y * q.z + q.w * q.x );
//	mat( 2, 2 ) = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
//	mat( 2, 3 ) = 0;
//	mat( 3, 0 ) = 0;
//	mat( 3, 1 ) = 0;
//	mat( 3, 2 ) = 0;
//	mat( 3, 3 ) = 1;
//	return( mat );
//}

// 
// 
//
//


/// @brief 仮想トラックボールの実装(左手座標系)
//! 
//! @note 以下のソースコードを参考にした
//! 
//! Trackball code:
//! 
//! Implementation of a virtual trackball.
//! Implemented by Gavin Bell, lots of ideas from Thant Tessman and
//!   the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
//! 
//! Vector manip code:
//! 
//! Original code from:
//! David M. Ciemiewicz, Mark Grossman, Henry Moreton, and Paul Haeberli
//! 
//! Much mucking with by:
//! Gavin Bell
//! 
//! @param[in] p1             … 回転前の点
//! @param[in] p2             … 回転後の点
//! @param[in] axisX          … トラックボールのX軸
//! @param[in] axisY          … トラックボールのY軸
//! @param[in] axisZ          … トラックボールのZ軸
//! @param[in] trackball_size … トラックボールの半径（デフォルトは0.8）
//! 
//! @return 回転を表すクォータニオン
//! 
template < class T >
const quaternion< T > track_ball( const vector2< T > &p1, const vector2< T > &p2, const vector3< T > &axisX, const vector3< T > axisY, const vector3< T > axisZ, const typename vector3< T >::value_type &trackball_size )
{
	typedef typename quaternion< T >::value_type value_type;

	if( p1 == p2 )
	{
		return( quaternion< T >( 1, 0, 0, 0 ) );
	}

	vector3< T > sp1( p1.x, p1.y, 0 ), sp2( p2.x, p2.y, 0 );
	value_type l, _2 = std::sqrt( value_type( 2.0 ) );

	// 点1の座標を仮想トラックボール上に投影
	l = p1.length( );
	if( l < trackball_size / _2 )
	{
		sp1.z = - std::sqrt( trackball_size * trackball_size - l * l );
	}
	else
	{
		sp1.z = - trackball_size * trackball_size / 2.0 / l;
	}

	// 点2の座標を仮想トラックボール上に投影
	l = p2.length( );
	if( l < trackball_size / _2 )
	{
		sp2.z = - std::sqrt( trackball_size * trackball_size - l * l );
	}
	else
	{
		sp2.z = - trackball_size * trackball_size / 2.0 / l;
	}

	//	sp1 = sp1.unit();
	//	sp2 = sp2.unit();

	// 右手系と左手系でここの外積の向きを反転させる
	//	Vector3<double> axis = (sp2 * sp1).unit();
	vector3< T > axis = ( sp1 * sp2 ).unit( );
	axis = ( axis.x * axisX + axis.y * axisY + axis.z * axisZ ).unit( );

	l = ( sp2 - sp1 ).length( ) / ( 2 * trackball_size );
	//	l = (l < -1.0)? -1.0: l;
	l = l > 1 ? 1: l;

	double phi = std::asin( l );
	//	fprintf(stdout, "axis(%.1f, %.1f, %.1f)   theta = %.1f\n", axis.x, axis.y, axis.z, phi * 180 / PAI);
	//	printf("%.1f\n", phi * 180 / PAI);
	return( quaternion< T >( std::cos( phi ), std::sin( phi ) * axis ) );
}

/// @brief 仮想トラックボールの実装(左手座標系)
//! 
//! @note 以下のソースコードを参考にした
//! 
//! Trackball code:
//! 
//! Implementation of a virtual trackball.
//! Implemented by Gavin Bell, lots of ideas from Thant Tessman and
//!   the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
//! 
//! Vector manip code:
//! 
//! Original code from:
//! David M. Ciemiewicz, Mark Grossman, Henry Moreton, and Paul Haeberli
//! 
//! Much mucking with by:
//! Gavin Bell
//! 
//! @param[in] p1             … 回転前の点
//! @param[in] p2             … 回転後の点
//! @param[in] axisX          … トラックボールのX軸
//! @param[in] axisY          … トラックボールのY軸
//! @param[in] axisZ          … トラックボールのZ軸
//! 
//! @return 回転を表すクォータニオン
//! 
template < class T >
inline const quaternion< T > track_ball( const vector2< T > &p1, const vector2< T > &p2, const vector3< T > &axisX, const vector3< T > axisY, const vector3< T > axisZ )
{
	return( track_ball( p1, p2, axisX, axisY, axisZ, 0.8 ) );
}


/// @brief 仮想トラックボールの実装(左手座標系)
//! 
//! トラックボールを用いて，任意ベクトルの回転を行う
//! 
//! @param[in] x1             … 回転前のX座標
//! @param[in] y1             … 回転前のY座標
//! @param[in] x2             … 回転後のX座標
//! @param[in] y2             … 回転後のY座標
//! @param[in] axisX          … トラックボールのX軸
//! @param[in] axisY          … トラックボールのY軸
//! @param[in] axisZ          … トラックボールのZ軸
//! @param[in] trackball_size … トラックボールの半径（デフォルトは0.8）
//! 
//! @return 回転を表すクォータニオン
//! 
template < class T >
const quaternion< T > track_ball( const typename vector3< T >::value_type &x1, const typename vector3< T >::value_type &y1, const typename vector3< T >::value_type &x2, const typename vector3< T >::value_type &y2,
												const vector3< T > &axisX, const vector3< T > &axisY, const vector3< T > &axisZ, const typename vector3< T >::value_type &trackball_size )
{
	return( track_ball( vector2< T >( x1, y1 ), vector2< T >( x2, y2 ), axisX, axisY, axisZ, trackball_size ) );
}


/// @brief 仮想トラックボールの実装(左手座標系)
//! 
//! トラックボールを用いて，任意ベクトルの回転を行う
//! 
//! @param[in] x1             … 回転前のX座標
//! @param[in] y1             … 回転前のY座標
//! @param[in] x2             … 回転後のX座標
//! @param[in] y2             … 回転後のY座標
//! @param[in] axisX          … トラックボールのX軸
//! @param[in] axisY          … トラックボールのY軸
//! @param[in] axisZ          … トラックボールのZ軸
//! 
//! @return 回転を表すクォータニオン
//! 
template < class T >
const quaternion< T > track_ball( const typename vector3< T >::value_type &x1, const typename vector3< T >::value_type &y1, const typename vector3< T >::value_type &x2,
											const typename vector3< T >::value_type &y2, const vector3< T > &axisX, const vector3< T > &axisY, const vector3< T > &axisZ )
{
	return( track_ball( vector2< T >( x1, y1 ), vector2< T >( x2, y2 ), axisX, axisY, axisZ, 0.8 ) );
}


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_QUATERNION__
