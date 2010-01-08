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

/// @file mist/config/color.h
//!
//! @brief カラーデータを扱うためのクラス群
//!

#ifndef __INCLUDE_MIST_COLOR_H__
#define __INCLUDE_MIST_COLOR_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "type_trait.h"
#endif

#include <iostream>
#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN

namespace __color_utility__
{
	template < bool >
	struct __normalized_color__
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &R, T &G, T &B )
		{
			T sum = r + g + b;

			sum = sum > 0 ? sum : 1;

			R = r / sum;
			G = g / sum;
			B = b / sum;
		}
	};

	template < >
	struct __normalized_color__< false >
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &R, T &G, T &B )
		{
			double sum = static_cast< double >( r ) + static_cast< double >( g ) + static_cast< double >( b );

			sum = sum > 0.0 ? sum : 1.0;

			R = static_cast< T >( r * 255.0 / sum + 0.5 );
			G = static_cast< T >( g * 255.0 / sum + 0.5 );
			B = static_cast< T >( b * 255.0 / sum + 0.5 );
		}
	};

	template < class T > inline T maximum( const T &v1, const T &v2 )
	{
		return( v1 > v2 ? v1 : v2 );
	}

	template < class T > inline T minimum( const T &v1, const T &v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}
}

// MISTで利用する基底のデータ型
template < class T > struct bgr;


//! @addtogroup color_group MIST全般で利用可能な色を表現するクラス
//!
//! @code 次のヘッダを参照
//! #include <mist/config/color.h>
//! @endcode
//!
//! @{


/// @brief カラー画像用の画素
//! 
//! @code カラー画像の作成例
//! mist::array2< mist::rgb< unsigned char > > image;
//! @endcode
//! 
//! @param T … 各色成分のデータ型
//! 
template< class T >
struct rgb
{
public:
	typedef size_t size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T& reference;					///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる
	typedef T value_type;					///< @brief 内部データ型．T と同じ
	typedef T* pointer;						///< @brief データ型のポインター型．data の場合，data * となる
	typedef const T* const_pointer;			///< @brief データ型の const ポインター型．data の場合，const data * となる

	/// @brief データ型の変換を行う
	template < class TT > 
	struct rebind
	{
		typedef rgb< TT > other;
	};

public:
	value_type r;		///< @brief 赤色成分
	value_type g;		///< @brief 緑色成分
	value_type b;		///< @brief 青色成分

	/// @brief デフォルトコンストラクタ（全ての要素を0で初期化する）
	rgb( ) : r( 0 ), g( 0 ), b( 0 ){ }

	/// @brief 全ての成分を pix で初期化する
	explicit rgb( const value_type &pix ) : r( pix ), g( pix ), b( pix ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	rgb( const rgb< TT > &c ) : r( static_cast< value_type >( c.r ) ), g( static_cast< value_type >( c.g ) ), b( static_cast< value_type >( c.b ) ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	rgb( const bgr< TT > &c ) : r( static_cast< value_type >( c.r ) ), g( static_cast< value_type >( c.g ) ), b( static_cast< value_type >( c.b ) ){ }

	/// @brief 他のカラー画素を用いて初期化する
	rgb( const rgb< T > &c ) : r( c.r ), g( c.g ), b( c.b ){ }

	/// @brief 赤 rr，緑 gg，青 bb を用いて初期化する
	rgb( const value_type &rr, const value_type &gg, const value_type &bb ) : r( rr ), g( gg ), b( bb ){ }


	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const rgb &operator =( const rgb< TT > &c )
	{
		r = static_cast< value_type >( c.r );
		g = static_cast< value_type >( c.g );
		b = static_cast< value_type >( c.b );
		return( *this );
	}

	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const rgb &operator =( const bgr< TT > &c )
	{
		r = static_cast< value_type >( c.r );
		g = static_cast< value_type >( c.g );
		b = static_cast< value_type >( c.b );
		return( *this );
	}

	/// @brief 他のカラー画素を代入する
	const rgb &operator =( const rgb< T > &c )
	{
		if( &c != this )
		{
			r = c.r;
			g = c.g;
			b = c.b;
		}
		return( *this );
	}

	/// @brief 全ての要素に pix を代入する
	const rgb &operator =( const value_type &pix )
	{
		r = pix;
		g = pix;
		b = pix;
		return( *this );
	}


	/// @brief 全要素の符号反転
	const rgb  operator -( ) const { return( rgb( -r, -g, -b ) ); }

	/// @brief RGB成分の和
	template < class TT >
	const rgb &operator +=( const rgb< TT > &c ){ r = static_cast< value_type >( r + c.r ); g = static_cast< value_type >( g + c.g ); b = static_cast< value_type >( b + c.b ); return( *this ); }

	/// @brief RGB成分の差
	template < class TT >
	const rgb &operator -=( const rgb< TT > &c ){ r = static_cast< value_type >( r - c.r ); g = static_cast< value_type >( g - c.g ); b = static_cast< value_type >( b - c.b ); return( *this ); }

	/// @brief RGB成分の積
	template < class TT >
	const rgb &operator *=( const rgb< TT > &c ){ r = static_cast< value_type >( r * c.r ); g = static_cast< value_type >( g * c.g ); b = static_cast< value_type >( b * c.b ); return( *this ); }

	/// @brief RGB成分の割り算
	template < class TT >
	const rgb &operator /=( const rgb< TT > &c ){ r = static_cast< value_type >( r / c.r ); g = static_cast< value_type >( g / c.g ); b = static_cast< value_type >( b / c.b ); return( *this ); }

	/// @brief RGB成分の剰余
	const rgb &operator %=( const rgb &c ){ r %= c.r; g %= c.g; b %= c.b; return( *this ); }

	/// @brief RGB成分の | 演算
	const rgb &operator |=( const rgb &c ){ r |= c.r; g |= c.g; b |= c.b; return( *this ); }

	/// @brief RGB成分の & 演算
	const rgb &operator &=( const rgb &c ){ r &= c.r; g &= c.g; b &= c.b; return( *this ); }

	/// @brief RGB成分の ^ 演算
	const rgb &operator ^=( const rgb &c ){ r ^= c.r; g ^= c.g; b ^= c.b; return( *this ); }


	/// @brief RGB成分に pix 値を足す
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator +=( const double &pix )
#else
	template < class TT >
	const rgb &operator +=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r + pix );
		g = static_cast< value_type >( g + pix );
		b = static_cast< value_type >( b + pix );
		return( *this );
	}

	/// @brief RGB成分から pix 値を引く
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator -=( const double &pix )
#else
	template < class TT >
	const rgb &operator -=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r - pix );
		g = static_cast< value_type >( g - pix );
		b = static_cast< value_type >( b - pix );
		return( *this );
	}

	/// @brief RGB成分に pix 値を掛ける
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator *=( const double &pix )
#else
	template < class TT >
	const rgb &operator *=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r * pix );
		g = static_cast< value_type >( g * pix );
		b = static_cast< value_type >( b * pix );
		return( *this );
	}

	/// @brief RGB成分を pix 値で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator /=( const double &pix )
#else
	template < class TT >
	const rgb &operator /=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r / pix );
		g = static_cast< value_type >( g / pix );
		b = static_cast< value_type >( b / pix );
		return( *this );
	}


	/// @brief 2つのカラー画素が等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が等しい場合
	//! @retval false … 2つのカラー画素が異なる場合
	//! 
	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && b == c.b ); }

	/// @brief 2つのカラー画素が等しくない（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が異なる場合
	//! @retval false … 2つのカラー画素が等しい場合
	//! 
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }

	/// @brief 2つのカラー画素の < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <  c2 の場合
	//! @retval false … c1 >= c2 の場合
	//! 
	bool operator < ( const rgb &c ) const
	{
		if( r == c.r )
		{
			if( g == c.g )
			{
				return( b < c.b );
			}
			else
			{
				return( g < c.g );
			}
		}
		else
		{
			return( r < c.r );
		}
	}

	/// @brief 2つのカラー画素の <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <= c2 の場合
	//! @retval false … c1 >  c2 の場合
	//! 
	bool operator <=( const rgb &c ) const { return( c >= *this ); }

	/// @brief 2つのカラー画素の > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >  c2 の場合
	//! @retval false … c1 <= c2 の場合
	//! 
	bool operator > ( const rgb &c ) const { return( c < *this ); }

	/// @brief 2つのカラー画素の >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >= c2 の場合
	//! @retval false … c1 <  c2 の場合
	//! 
	bool operator >=( const rgb &c ) const { return( !( *this < c ) ); }


	/// @brief NTSC系加重平均法により，グレースケールへ変換する
	value_type get_value( ) const
	{
		return( half_adjust< value_type >::convert( r * 0.298912 + g * 0.586610 + b * 0.114478 ) );
	}

	// カラーからグレースケールへの自動キャスト演算子（危険のため一時停止）
	//operator value_type( ) const { return( get_value( ) ); }
};



/// @brief カラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, + )

/// @brief カラー画素と定数の和
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, + )

/// @brief 定数とカラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, + )

/// @brief カラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, - )

/// @brief カラー画素と定数の差
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, - )

/// @brief 定数とカラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR4( rgb, - )

/// @brief カラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, * )

/// @brief カラー画素と定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, * )

/// @brief 定数とカラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, * )

/// @brief カラー画素の割り算
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, / )

/// @brief カラー画素を定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, / )

/// @brief カラー画素の剰余
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, % )


/// @brief カラー画素の | 演算
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, | )

/// @brief カラー画素の & 演算
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, & )

/// @brief カラー画素の ^ 演算
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, ^ )




/// @brief BGRの順でデータが並ぶカラー画像用の画素
//! 
//! 主にWindowsのビットマップで利用されている
//! 
//! @code カラー画像の作成例
//! mist::array2< mist::bgr< unsigned char > > image;
//! @endcode
//! 
//! @param T … 各色成分のデータ型
//! 
template< class T >
struct bgr
{
public:
	typedef size_t size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T& reference;					///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる
	typedef T value_type;					///< @brief 内部データ型．T と同じ
	typedef T* pointer;						///< @brief データ型のポインター型．data の場合，data * となる
	typedef const T* const_pointer;			///< @brief データ型の const ポインター型．data の場合，const data * となる

	/// @brief データ型の変換を行う
	template < class TT > 
	struct rebind
	{
		typedef bgr< TT > other;
	};

public:
	value_type b;		///< @brief 青色成分
	value_type g;		///< @brief 緑色成分
	value_type r;		///< @brief 赤色成分

	/// @brief デフォルトコンストラクタ（全ての要素を0で初期化する）
	bgr( ) : b( 0 ), g( 0 ), r( 0 ){ }

	/// @brief 全ての成分を pix で初期化する
	explicit bgr( const value_type &pix ) : b( pix ), g( pix ), r( pix ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	bgr( const bgr< TT > &c ) : b( static_cast< value_type >( c.b ) ), g( static_cast< value_type >( c.g ) ), r( static_cast< value_type >( c.r ) ){ }

	/// @brief 他のカラー画素を用いて初期化する
	bgr( const bgr< T > &c ) : b( c.b ), g( c.g ), r( c.r ){ }

	/// @brief 赤 rr，緑 gg，青 bb を用いて初期化する
	bgr( const value_type &rr, const value_type &gg, const value_type &bb ) : b( bb ), g( gg ), r( rr ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	bgr( const rgb< TT > &c ) : b( static_cast< value_type >( c.b ) ), g( static_cast< value_type >( c.g ) ), r( static_cast< value_type >( c.r ) ){ }


	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const bgr &operator =( const bgr< TT > &c )
	{
		b = static_cast< value_type >( c.b );
		g = static_cast< value_type >( c.g );
		r = static_cast< value_type >( c.r );
		return( *this );
	}

	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const bgr &operator =( const rgb< TT > &c )
	{
		b = static_cast< value_type >( c.b );
		g = static_cast< value_type >( c.g );
		r = static_cast< value_type >( c.r );
		return( *this );
	}

	/// @brief 他のカラー画素を代入する
	const bgr &operator =( const bgr< T > &c )
	{
		if( &c != this )
		{
			b = c.b;
			g = c.g;
			r = c.r;
		}
		return( *this );
	}

	/// @brief 全ての要素に pix を代入する
	const bgr &operator =( const value_type &pix )
	{
		b = pix;
		g = pix;
		r = pix;
		return( *this );
	}


	/// @brief 全要素の符号反転
	const bgr  operator -( ) const { return( bgr( -r, -g, -b ) ); }

	/// @brief RGB成分の和
	template < class TT >
	const bgr &operator +=( const bgr< TT > &c ){ b = static_cast< value_type >( b + c.b ); g = static_cast< value_type >( g + c.g ); r = static_cast< value_type >( r + c.r ); return( *this ); }

	/// @brief RGB成分の差
	template < class TT >
	const bgr &operator -=( const bgr< TT > &c ){ b = static_cast< value_type >( b - c.b ); g = static_cast< value_type >( g - c.g ); r = static_cast< value_type >( r - c.r ); return( *this ); }

	/// @brief RGB成分の積
	template < class TT >
	const bgr &operator *=( const bgr< TT > &c ){ b = static_cast< value_type >( b * c.b ); g = static_cast< value_type >( g * c.g ); r = static_cast< value_type >( r * c.r ); return( *this ); }

	/// @brief RGB成分の割り算
	template < class TT >
	const bgr &operator /=( const bgr< TT > &c ){ b = static_cast< value_type >( b / c.b ); g = static_cast< value_type >( g / c.g ); r = static_cast< value_type >( r / c.r ); return( *this ); }

	/// @brief RGB成分の剰余
	const bgr &operator %=( const bgr &c ){ b %= c.b; g %= c.g; r %= c.r; return( *this ); }

	/// @brief RGB成分の | 演算
	const bgr &operator |=( const bgr &c ){ b |= c.b; g |= c.g; r |= c.r; return( *this ); }

	/// @brief RGB成分の & 演算
	const bgr &operator &=( const bgr &c ){ b &= c.b; g &= c.g; r &= c.r; return( *this ); }

	/// @brief RGB成分の ^ 演算
	const bgr &operator ^=( const bgr &c ){ b ^= c.b; g ^= c.g; r ^= c.r; return( *this ); }


	/// @brief RGB成分に pix 値を足す
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator +=( const double &pix )
#else
	template < class TT >
	const bgr &operator +=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b + pix );
		g = static_cast< value_type >( g + pix );
		r = static_cast< value_type >( r + pix );
		return( *this );
	}

	/// @brief RGB成分から pix 値を引く
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator -=( const double &pix )
#else
	template < class TT >
	const bgr &operator -=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b - pix );
		g = static_cast< value_type >( g - pix );
		r = static_cast< value_type >( r - pix );
		return( *this );
	}

	/// @brief RGB成分に pix 値を掛ける
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator *=( const double &pix )
#else
	template < class TT >
	const bgr &operator *=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b * pix );
		g = static_cast< value_type >( g * pix );
		r = static_cast< value_type >( r * pix );
		return( *this );
	}

	/// @brief RGB成分を pix 値で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator /=( const double &pix )
#else
	template < class TT >
	const bgr &operator /=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b / pix );
		g = static_cast< value_type >( g / pix );
		r = static_cast< value_type >( r / pix );
		return( *this );
	}


	/// @brief 2つのカラー画素が等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が等しい場合
	//! @retval false … 2つのカラー画素が異なる場合
	//! 
	bool operator ==( const bgr &c ) const { return( b == c.b && g == c.g && r == c.r ); }

	/// @brief 2つのカラー画素が等しくない（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が異なる場合
	//! @retval false … 2つのカラー画素が等しい場合
	//! 
	bool operator !=( const bgr &c ) const { return( !( *this == c ) ); }

	/// @brief 2つのカラー画素の < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <  c2 の場合
	//! @retval false … c1 >= c2 の場合
	//! 
	bool operator < ( const bgr &c ) const
	{
		if( r == c.r )
		{
			if( g == c.g )
			{
				return( b < c.b );
			}
			else
			{
				return( g < c.g );
			}
		}
		else
		{
			return( r < c.r );
		}
	}

	/// @brief 2つのカラー画素の <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <= c2 の場合
	//! @retval false … c1 >  c2 の場合
	//! 
	bool operator <=( const bgr &c ) const { return( c >= *this ); }

	/// @brief 2つのカラー画素の > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >  c2 の場合
	//! @retval false … c1 <= c2 の場合
	//! 
	bool operator > ( const bgr &c ) const { return( c < *this ); }

	/// @brief 2つのカラー画素の >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >= c2 の場合
	//! @retval false … c1 <  c2 の場合
	//! 
	bool operator >=( const bgr &c ) const { return( !( *this < c ) ); }


	/// @brief NTSC系加重平均法により，グレースケールへ変換する
	value_type get_value( ) const
	{
		return( static_cast< value_type >( b * 0.114478 + g * 0.586610 + r * 0.298912 ) );
	}

	// カラーからグレースケールへの自動キャスト演算子（危険のため一時停止）
	//operator value_type( ) const { return( get_value( ) ); }

};

/// @brief カラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, + )

/// @brief カラー画素と定数の和
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, + )

/// @brief 定数とカラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR3( bgr, + )

/// @brief カラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, - )

/// @brief カラー画素と定数の差
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, - )

/// @brief 定数とカラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR4( bgr, - )

/// @brief カラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, * )

/// @brief カラー画素と定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, * )

/// @brief 定数とカラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR3( bgr, * )

/// @brief カラー画素の割り算
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, / )

/// @brief カラー画素を定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, / )

/// @brief カラー画素の剰余
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, % )


/// @brief カラー画素の | 演算
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, | )

/// @brief カラー画素の & 演算
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, & )

/// @brief カラー画素の ^ 演算
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, ^ )




/// @brief アルファチャンネル付きカラー画像用の画素
//! 
//! @code アルファチャンネル付きカラー画像の作成例
//! mist::array2< mist::rgba< unsigned char > > image;
//! @endcode
//! 
//! @param T … 各色成分のデータ型
//! 
template< class T >
struct rgba : public rgb< T >
{
protected:
	typedef rgb< T > base;

public:
	typedef typename base::size_type		size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type	difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename base::reference		reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef typename base::const_reference	const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる
	typedef typename base::value_type		value_type;				///< @brief 内部データ型．T と同じ
	typedef typename base::pointer			pointer;				///< @brief データ型のポインター型．data の場合，data * となる
	typedef typename base::const_pointer	const_pointer;			///< @brief データ型の const ポインター型．data の場合，const data * となる

	/// @brief データ型の変換を行う
	template < class TT > 
	struct rebind
	{
		typedef rgba< TT > other;
	};

public:
	value_type a;		///< @brief アルファ成分

	/// @brief デフォルトコンストラクタ（全ての要素を0で初期化する）
	rgba( ) : base( ), a( 255 ){ }

	/// @brief 全ての成分を pix で初期化する
	explicit rgba( const value_type &pix ) : base( pix ), a( 255 ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	rgba( const rgba< TT > &c ) : base( c ), a( static_cast< value_type >( c.a ) ){ }

	/// @brief 他のカラー画素を用いて初期化する
	rgba( const rgba< T > &c ) : base( c ), a( c.a ){ }


	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	rgba( const rgb< TT > &c ) : base( c ), a( 255 ){ }

	/// @brief 他のカラー画素を用いて初期化する
	rgba( const rgb< T > &c ) : base( c ), a( 255 ){ }


	/// @brief 赤 rr，緑 gg，青 bb を用いて初期化する
	rgba( const value_type &rr, const value_type &gg, const value_type &bb, const value_type &aa = 255 ) : base( rr, gg, bb ), a( aa ){ }


	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const rgba &operator =( const rgba< TT > &c )
	{
		base::operator =( c );
		a = static_cast< value_type >( c.a );
		return( *this );
	}

	/// @brief 他のカラー画素を代入する
	const rgba &operator =( const rgba< T > &c )
	{
		if( &c != this )
		{
			base::operator =( c );
			a = c.a;
		}
		return( *this );
	}

	/// @brief 全ての要素に pix を代入する
	const rgba &operator =( const value_type &pix )
	{
		base::operator =( pix );
		return( *this );
	}


	/// @brief 全要素の符号反転
	const rgba  operator -( ) const { return( rgba( -base::r, -base::g, -base::b, a ) ); }

	/// @brief RGB成分の和
	template < class TT >
	const rgba &operator +=( const rgba< TT > &c ){ base::operator +=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の差
	template < class TT >
	const rgba &operator -=( const rgba< TT > &c ){ base::operator -=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の積
	template < class TT >
	const rgba &operator *=( const rgba< TT > &c ){ base::operator *=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の割り算
	template < class TT >
	const rgba &operator /=( const rgba< TT > &c ){ base::operator /=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の剰余
	const rgba &operator %=( const rgba &c ){ base::operator %=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の | 演算
	const rgba &operator |=( const rgba &c ){ base::operator |=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の & 演算
	const rgba &operator &=( const rgba &c ){ base::operator &=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の ^ 演算
	const rgba &operator ^=( const rgba &c ){ base::operator ^=( ( const base &)c ); return( *this ); }


	/// @brief RGB成分に pix 値を足す
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator +=( const double &pix )
#else
	template < class TT >
	const rgba &operator +=( const TT &pix )
#endif
	{
		base::operator +=( pix );
		return( *this );
	}

	/// @brief RGB成分から pix 値を引く
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator -=( const double &pix )
#else
	template < class TT >
	const rgba &operator -=( const TT &pix )
#endif
	{
		base::operator -=( pix );
		return( *this );
	}

	/// @brief RGB成分に pix 値を掛ける
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator *=( const double &pix )
#else
	template < class TT >
	const rgba &operator *=( const TT &pix )
#endif
	{
		base::operator *=( pix );
		return( *this );
	}

	/// @brief RGB成分を pix 値で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator /=( const double &pix )
#else
	template < class TT >
	const rgba &operator /=( const TT &pix )
#endif
	{
		base::operator /=( pix );
		return( *this );
	}


	/// @brief 2つのカラー画素が等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が等しい場合
	//! @retval false … 2つのカラー画素が異なる場合
	//! 
	bool operator ==( const rgba &c ) const { return( base::operator ==( c ) ); }

	/// @brief 2つのカラー画素が等しくない（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が異なる場合
	//! @retval false … 2つのカラー画素が等しい場合
	//! 
	bool operator !=( const rgba &c ) const { return( !( *this == c ) ); }

	/// @brief 2つのカラー画素の < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <  c2 の場合
	//! @retval false … c1 >= c2 の場合
	//! 
	bool operator < ( const rgba &c ) const
	{
		if( a == c.a )
		{
			return( base::operator <( c ) );
		}
		else
		{
			return( a < c.a );
		}
	}

	/// @brief 2つのカラー画素の <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <= c2 の場合
	//! @retval false … c1 >  c2 の場合
	//! 
	bool operator <=( const rgba &c ) const { return( c >= *this ); }

	/// @brief 2つのカラー画素の > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >  c2 の場合
	//! @retval false … c1 <= c2 の場合
	//! 
	bool operator > ( const rgba &c ) const { return( c < *this ); }

	/// @brief 2つのカラー画素の >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >= c2 の場合
	//! @retval false … c1 <  c2 の場合
	//! 
	bool operator >=( const rgba &c ) const { return( !( *this < c ) ); }

};

/// @brief カラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, + )

/// @brief カラー画素と定数の和
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, + )

/// @brief 定数とカラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR3( rgba, + )

/// @brief カラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, - )

/// @brief カラー画素と定数の差
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, - )

/// @brief 定数とカラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR4( rgba, - )

/// @brief カラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, * )

/// @brief カラー画素と定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, * )

/// @brief 定数とカラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR3( rgba, * )

/// @brief カラー画素の割り算
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, / )

/// @brief カラー画素を定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, / )

/// @brief カラー画素の剰余
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, % )


/// @brief カラー画素の | 演算
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, | )

/// @brief カラー画素の & 演算
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, & )

/// @brief カラー画素の ^ 演算
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, ^ )









/// @brief BGRAの順でデータが並ぶカラー画像用の画素
//! 
//! 主にWindowsのビットマップで利用されている
//! 
//! @code カラー画像の作成例
//! mist::array2< mist::bgra< unsigned char > > image;
//! @endcode
//! 
//! @param T … 各色成分のデータ型
//! 
template< class T >
struct bgra : public bgr< T >
{
protected:
	typedef bgr< T > base;

public:
	typedef typename base::size_type		size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type	difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename base::reference		reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef typename base::const_reference	const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる
	typedef typename base::value_type		value_type;				///< @brief 内部データ型．T と同じ
	typedef typename base::pointer			pointer;				///< @brief データ型のポインター型．data の場合，data * となる
	typedef typename base::const_pointer	const_pointer;			///< @brief データ型の const ポインター型．data の場合，const data * となる

	/// @brief データ型の変換を行う
	template < class TT > 
	struct rebind
	{
		typedef bgra< TT > other;
	};

public:
	value_type a;		///< @brief アルファ成分

	/// @brief デフォルトコンストラクタ（全ての要素を0で初期化する）
	bgra( ) : base( ), a( 255 ){ }

	/// @brief 全ての成分を pix で初期化する
	explicit bgra( const value_type &pix ) : base( pix ), a( 255 ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	bgra( const bgra< TT > &c ) : base( c ), a( static_cast< value_type >( c.a ) ){ }

	/// @brief 他のカラー画素を用いて初期化する
	bgra( const bgra< T > &c ) : base( c ), a( c.a ){ }


	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	bgra( const rgb< TT > &c ) : base( c ), a( 255 ){ }

	/// @brief 他のカラー画素を用いて初期化する
	bgra( const rgb< T > &c ) : base( c ), a( 255 ){ }


	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	bgra( const bgr< TT > &c ) : base( c ), a( 255 ){ }

	/// @brief 他のカラー画素を用いて初期化する
	bgra( const bgr< T > &c ) : base( c ), a( 255 ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	bgra( const rgba< TT > &c ) : base( c ), a( c.a ){ }

	/// @brief 赤 rr，緑 gg，青 bb を用いて初期化する
	bgra( const value_type &rr, const value_type &gg, const value_type &bb, const value_type &aa = 255 ) : base( rr, gg, bb ), a( aa ){ }


	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const bgra &operator =( const bgra< TT > &c )
	{
		base::operator =( c );
		a = static_cast< value_type >( c.a );
		return( *this );
	}

	/// @brief 他のカラー画素を代入する
	const bgra &operator =( const bgra< T > &c )
	{
		if( &c != this )
		{
			base::operator =( c );
			a = c.a;
		}
		return( *this );
	}

	/// @brief 全ての要素に pix を代入する
	const bgra &operator =( const value_type &pix )
	{
		base::operator =( pix );
		return( *this );
	}


	/// @brief 全要素の符号反転
	const bgra  operator -( ) const { return( bgra( -base::r, -base::g, -base::b, a ) ); }

	/// @brief RGB成分の和
	template < class TT >
	const bgra &operator +=( const bgra< TT > &c ){ base::operator +=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の差
	template < class TT >
	const bgra &operator -=( const bgra< TT > &c ){ base::operator -=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の積
	template < class TT >
	const bgra &operator *=( const bgra< TT > &c ){ base::operator *=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の割り算
	template < class TT >
	const bgra &operator /=( const bgra< TT > &c ){ base::operator /=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の剰余
	const bgra &operator %=( const bgra &c ){ base::operator %=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の | 演算
	const bgra &operator |=( const bgra &c ){ base::operator |=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の & 演算
	const bgra &operator &=( const bgra &c ){ base::operator &=( ( const base &)c ); return( *this ); }

	/// @brief RGB成分の ^ 演算
	const bgra &operator ^=( const bgra &c ){ base::operator ^=( ( const base &)c ); return( *this ); }


	/// @brief RGB成分に pix 値を足す
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator +=( const double &pix )
#else
	template < class TT >
	const bgra &operator +=( const TT &pix )
#endif
	{
		base::operator +=( pix );
		return( *this );
	}

	/// @brief RGB成分から pix 値を引く
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator -=( const double &pix )
#else
	template < class TT >
	const bgra &operator -=( const TT &pix )
#endif
	{
		base::operator -=( pix );
		return( *this );
	}

	/// @brief RGB成分に pix 値を掛ける
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator *=( const double &pix )
#else
	template < class TT >
	const bgra &operator *=( const TT &pix )
#endif
	{
		base::operator *=( pix );
		return( *this );
	}

	/// @brief RGB成分を pix 値で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator /=( const double &pix )
#else
	template < class TT >
	const bgra &operator /=( const TT &pix )
#endif
	{
		base::operator /=( pix );
		return( *this );
	}


	/// @brief 2つのカラー画素が等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が等しい場合
	//! @retval false … 2つのカラー画素が異なる場合
	//! 
	bool operator ==( const bgra &c ) const { return( base::operator ==( c ) ); }

	/// @brief 2つのカラー画素が等しくない（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が異なる場合
	//! @retval false … 2つのカラー画素が等しい場合
	//! 
	bool operator !=( const bgra &c ) const { return( !( *this == c ) ); }

	/// @brief 2つのカラー画素の < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <  c2 の場合
	//! @retval false … c1 >= c2 の場合
	//! 
	bool operator < ( const bgra &c ) const
	{
		if( a == c.a )
		{
			return( base::operator <( c ) );
		}
		else
		{
			return( a < c.a );
		}
	}


	/// @brief 2つのカラー画素の <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <= c2 の場合
	//! @retval false … c1 >  c2 の場合
	//! 
	bool operator <=( const bgra &c ) const { return( c >= *this ); }

	/// @brief 2つのカラー画素の > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >  c2 の場合
	//! @retval false … c1 <= c2 の場合
	//! 
	bool operator > ( const bgra &c ) const { return( c < *this ); }

	/// @brief 2つのカラー画素の >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >= c2 の場合
	//! @retval false … c1 <  c2 の場合
	//! 
	bool operator >=( const bgra &c ) const { return( !( *this < c ) ); }

};

/// @brief カラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, + )

/// @brief カラー画素と定数の和
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, + )

/// @brief 定数とカラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR3( bgra, + )

/// @brief カラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, - )

/// @brief カラー画素と定数の差
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, - )

/// @brief 定数とカラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR4( bgra, - )

/// @brief カラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, * )

/// @brief カラー画素と定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, * )

/// @brief 定数とカラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR3( bgra, * )

/// @brief カラー画素の割り算
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, / )

/// @brief カラー画素を定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, / )

/// @brief カラー画素の剰余
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, % )


/// @brief カラー画素の | 演算
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, | )

/// @brief カラー画素の & 演算
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, & )

/// @brief カラー画素の ^ 演算
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, ^ )


/// @brief カラー画像用の画素（6次元）
//! 
//! @code カラー画像の作成例
//! mist::array2< mist::RGB< unsigned char > > image;
//! @endcode
//! 
//! @param T … 各色成分のデータ型
//! 
template< class T >
struct nRGB : public rgb< T >
{
private:
	typedef rgb< T > base;																			///< @brief 基底クラス
	typedef __color_utility__::__normalized_color__< is_float< T >::value >	__color_normalizer__;	///< @brief 色の正規化を行うクラス

public:
	typedef typename base::size_type		size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type	difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T& reference;											///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;								///< @brief データ型の const 参照．data の場合，const data & となる
	typedef T value_type;											///< @brief 内部データ型．T と同じ
	typedef T* pointer;												///< @brief データ型のポインター型．data の場合，data * となる
	typedef const T* const_pointer;									///< @brief データ型の const ポインター型．data の場合，const data * となる

	/// @brief データ型の変換を行う
	template < class TT > 
	struct rebind
	{
		typedef nRGB< TT > other;
	};

public:
	value_type R;		///< @brief 赤色成分（正規化）
	value_type G;		///< @brief 緑色成分（正規化）
	value_type B;		///< @brief 青色成分（正規化）

	/// @brief デフォルトコンストラクタ（全ての要素を0で初期化する）
	nRGB( ) : base( ), R( 0 ), G( 0 ), B( 0 ){ }

	/// @brief 全ての成分を pix で初期化する
	explicit nRGB( const value_type &pix ) : base( pix )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	nRGB( const nRGB< TT > &c ) : base( c ), R( static_cast< T >( c.R ) ), G( static_cast< T >( c.G ) ), B( static_cast< T >( c.B ) ){ }

	/// @brief 他のカラー画素を用いて初期化する
	nRGB( const nRGB< T > &c ) : base( c ), R( c.R ), G( c.G ), B( c.B ){ }

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	nRGB( const rgb< TT > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief 他のカラー画素を用いて初期化する
	nRGB( const rgb< T > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	nRGB( const bgr< TT > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief 他のカラー画素を用いて初期化する
	nRGB( const bgr< T > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}


	/// @brief 赤 rr，緑 gg，青 bb を用いて初期化する
	nRGB( const value_type &rr, const value_type &gg, const value_type &bb ) : base( rr, gg, bb )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief 赤 rr，緑 gg，青 bb を用いて初期化する
	nRGB( const value_type &rr, const value_type &gg, const value_type &bb, const value_type &RR, const value_type &GG, const value_type &BB ) : base( rr, gg, bb ), R( RR ), G( GG ), B( BB ){ }


	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const nRGB &operator =( const nRGB< TT > &c )
	{
		base::operator =( c );
		R = static_cast< value_type >( c.R );
		G = static_cast< value_type >( c.G );
		B = static_cast< value_type >( c.B );
		return( *this );
	}

	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const nRGB &operator =( const rgb< TT > &c )
	{
		base::operator =( c );
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
		return( *this );
	}

	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const nRGB &operator =( const bgr< TT > &c )
	{
		base::operator =( c );
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
		return( *this );
	}

	/// @brief 他のカラー画素を代入する
	const nRGB &operator =( const nRGB< T > &c )
	{
		if( &c != this )
		{
			base::operator =( c );
			R = c.R;
			G = c.G;
			B = c.B;
		}
		return( *this );
	}

	/// @brief 全ての要素に pix を代入する
	const nRGB &operator =( const value_type &pix )
	{
		base::operator =( pix );
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
		return( *this );
	}


	/// @brief 全要素の符号反転
	const nRGB  operator -( ) const { return( nRGB( -base::r, -base::g, -base::b, -R, -G, -B ) ); }

	/// @brief nRGB成分の和
	template < class TT >
	const nRGB &operator +=( const nRGB< TT > &c )
	{
		base::operator +=( ( const base &)c );
		R = static_cast< value_type >( R + c.R );
		G = static_cast< value_type >( G + c.G );
		B = static_cast< value_type >( B + c.B );
		return( *this );
	}

	/// @brief nRGB成分の差
	template < class TT >
	const nRGB &operator -=( const nRGB< TT > &c )
	{
		base::operator -=( ( const base &)c );
		R = static_cast< value_type >( R - c.R );
		G = static_cast< value_type >( G - c.G );
		B = static_cast< value_type >( B - c.B );
		return( *this );
	}

	/// @brief nRGB成分の積
	template < class TT >
	const nRGB &operator *=( const nRGB< TT > &c )
	{
		base::operator *=( ( const base &)c );
		R = static_cast< value_type >( R * c.R );
		G = static_cast< value_type >( G * c.G );
		B = static_cast< value_type >( B * c.B );
		return( *this );
	}

	/// @brief nRGB成分の割り算
	template < class TT >
	const nRGB &operator /=( const nRGB< TT > &c )
	{
		base::operator /=( ( const base &)c );
		R = static_cast< value_type >( R / c.R );
		G = static_cast< value_type >( G / c.G );
		B = static_cast< value_type >( B / c.B );
		return( *this );
	}

	/// @brief nRGB成分の剰余
	const nRGB &operator %=( const nRGB &c )
	{
		base::operator %=( ( const base &)c );
		R %= c.R;
		G %= c.G;
		B %= c.B;
		return( *this );
	}

	/// @brief nRGB成分の | 演算
	const nRGB &operator |=( const nRGB &c )
	{
		base::operator |=( ( const base &)c );
		R |= c.R;
		G |= c.G;
		B |= c.B;
		return( *this );
	}

	/// @brief nRGB成分の & 演算
	const nRGB &operator &=( const nRGB &c )
	{
		base::operator &=( ( const base &)c );
		R &= c.R;
		G &= c.G;
		B &= c.B;
		return( *this );
	}

	/// @brief nRGB成分の ^ 演算
	const nRGB &operator ^=( const nRGB &c )
	{
		base::operator ^=( ( const base &)c );
		R ^= c.R;
		G ^= c.G;
		B ^= c.B;
		return( *this );
	}


	/// @brief nRGB成分に pix 値を足す
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator +=( const double &pix )
#else
	template < class TT >
	const nRGB &operator +=( const TT &pix )
#endif
	{
		base::operator +=( pix );
		R = static_cast< value_type >( R + pix );
		G = static_cast< value_type >( G + pix );
		B = static_cast< value_type >( B + pix );
		return( *this );
	}

	/// @brief nRGB成分から pix 値を引く
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator -=( const double &pix )
#else
	template < class TT >
	const nRGB &operator -=( const TT &pix )
#endif
	{
		base::operator -=( pix );
		R = static_cast< value_type >( R - pix );
		G = static_cast< value_type >( G - pix );
		B = static_cast< value_type >( B - pix );
		return( *this );
	}

	/// @brief nRGB成分に pix 値を掛ける
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator *=( const double &pix )
#else
	template < class TT >
	const nRGB &operator *=( const TT &pix )
#endif
	{
		base::operator *=( pix );
		R = static_cast< value_type >( R * pix );
		G = static_cast< value_type >( G * pix );
		B = static_cast< value_type >( B * pix );
		return( *this );
	}

	/// @brief nRGB成分を pix 値で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator /=( const double &pix )
#else
	template < class TT >
	const nRGB &operator /=( const TT &pix )
#endif
	{
		base::operator /=( pix );
		R = static_cast< value_type >( R / pix );
		G = static_cast< value_type >( G / pix );
		B = static_cast< value_type >( B / pix );
		return( *this );
	}
};



/// @brief カラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, + )

/// @brief カラー画素と定数の和
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, + )

/// @brief 定数とカラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR3( nRGB, + )

/// @brief カラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, - )

/// @brief カラー画素と定数の差
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, - )

/// @brief 定数とカラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR4( nRGB, - )

/// @brief カラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, * )

/// @brief カラー画素と定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, * )

/// @brief 定数とカラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR3( nRGB, * )

/// @brief カラー画素の割り算
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, / )

/// @brief カラー画素を定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, / )

/// @brief カラー画素の剰余
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, % )


/// @brief カラー画素の | 演算
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, | )

/// @brief カラー画素の & 演算
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, & )

/// @brief カラー画素の ^ 演算
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, ^ )



/// @brief カラー画像用の画素
//! 
//! @code カラー画像の作成例
//! mist::array2< mist::color< unsigned char, 9 > > image;
//! @endcode
//! 
//! @param T … 各色成分のデータ型
//! 
template< class T, size_t NDIM = 9 >
struct color
{
public:
	typedef size_t size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T& reference;					///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる
	typedef T value_type;					///< @brief 内部データ型．T と同じ
	typedef T* pointer;						///< @brief データ型のポインター型．data の場合，data * となる
	typedef const T* const_pointer;			///< @brief データ型の const ポインター型．data の場合，const data * となる

	/// @brief データ型の変換を行う
	template < class TT > 
	struct rebind
	{
		typedef color< TT, NDIM > other;
	};

private:
	value_type c_[ NDIM ];		///< @brief 色成分

public:

	/// @brief デフォルトコンストラクタ（全ての要素を0で初期化する）
	color( )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = 0;
		}
	}

	/// @brief 全ての成分を pix で初期化する
	explicit color( const value_type &pix )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = pix;
		}
	}

	/// @brief 異なる型のカラー画素を用いて初期化する
	template < class TT >
	color( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c[ i ] );
		}
	}

	/// @brief 他のカラー画素を用いて初期化する
	color( const color< T, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = c[ i ];
		}
	}


	/// @brief 異なる型の他のカラー画素を代入する
	template < class TT >
	const color &operator =( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c[ i ] );
		}
		return( *this );
	}

	/// @brief 他のカラー画素を代入する
	const color &operator =( const color< T, NDIM > &c )
	{
		if( &c != this )
		{
			for( size_type i = 0 ; i < NDIM ; i++ )
			{
				c_[ i ] = c[ i ];
			}
		}
		return( *this );
	}

	/// @brief 全ての要素に pix を代入する
	const color &operator =( const value_type &pix )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = pix;
		}
		return( *this );
	}


	/// @brief 全要素の符号反転
	const color  operator -( ) const
	{
		color c;
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c[ i ] = -c_[ i ];
		}
		return( c );
	}

	/// @brief RGB成分の和
	template < class TT >
	const color &operator +=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] + c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB成分の差
	template < class TT >
	const color &operator -=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] - c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB成分の積
	template < class TT >
	const color &operator *=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] * c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB成分の割り算
	template < class TT >
	const color &operator /=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] / c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB成分の剰余
	const color &operator %=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] %= c[ i ];
		}
		return( *this );
	}

	/// @brief RGB成分の | 演算
	const color &operator |=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] |= c[ i ];
		}
		return( *this );
	}

	/// @brief RGB成分の & 演算
	const color &operator &=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] &= c[ i ];
		}
		return( *this );
	}

	/// @brief RGB成分の ^ 演算
	const color &operator ^=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] ^= c[ i ];
		}
		return( *this );
	}


	/// @brief RGB成分に pix 値を足す
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator +=( const double &pix )
#else
	template < class TT >
	const color &operator +=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] + pix );
		}
		return( *this );
	}

	/// @brief RGB成分から pix 値を引く
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator -=( const double &pix )
#else
	template < class TT >
	const color &operator -=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] - pix );
		}
		return( *this );
	}

	/// @brief RGB成分に pix 値を掛ける
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator *=( const double &pix )
#else
	template < class TT >
	const color &operator *=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] * pix );
		}
		return( *this );
	}

	/// @brief RGB成分を pix 値で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator /=( const double &pix )
#else
	template < class TT >
	const color &operator /=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] / pix );
		}
		return( *this );
	}


	/// @brief 2つのカラー画素が等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が等しい場合
	//! @retval false … 2つのカラー画素が異なる場合
	//! 
	bool operator ==( const color &c ) const
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			if( c_[ i ] != c[ i ] )
			{
				return( false );
			}
		}

		return( true );
	}

	/// @brief 2つのカラー画素が等しくない（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が異なる場合
	//! @retval false … 2つのカラー画素が等しい場合
	//! 
	bool operator !=( const color &c ) const { return( !( *this == c ) ); }

	/// @brief 2つのカラー画素の < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <  c2 の場合
	//! @retval false … c1 >= c2 の場合
	//! 
	bool operator < ( const color &c ) const
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			if( c_[ i ] != c[ i ] )
			{
				return( c_[ i ] < c[ i ] );
			}
		}

		return( false );
	}

	/// @brief 2つのカラー画素の <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <= c2 の場合
	//! @retval false … c1 >  c2 の場合
	//! 
	bool operator <=( const color &c ) const { return( c >= *this ); }

	/// @brief 2つのカラー画素の > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >  c2 の場合
	//! @retval false … c1 <= c2 の場合
	//! 
	bool operator > ( const color &c ) const { return( c < *this ); }

	/// @brief 2つのカラー画素の >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >= c2 の場合
	//! @retval false … c1 <  c2 の場合
	//! 
	bool operator >=( const color &c ) const { return( !( *this < c ) ); }

	value_type & operator []( size_type index ){ return( c_[ index ] ); }

	const value_type & operator []( size_type index ) const { return( c_[ index ] ); }

	/// @brief NTSC系加重平均法により，グレースケールへ変換する
	value_type get_value( ) const
	{
		double sum = 0.0;
		for( size_type i = 0 ; i < NDIM - 1 ; i++ )
		{
			sum += c_[ i ];
		}
		return( half_adjust< value_type >::convert( sum ) );
	}

	// カラーからグレースケールへの自動キャスト演算子（危険のため一時停止）
	//operator value_type( ) const { return( get_value( ) ); }
};



/// @brief カラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR1_( color, + )

/// @brief カラー画素と定数の和
DEFINE_PROMOTE_BIND_OPERATOR2_( color, + )

/// @brief 定数とカラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR3_( color, + )

/// @brief カラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR1_( color, - )

/// @brief カラー画素と定数の差
DEFINE_PROMOTE_BIND_OPERATOR2_( color, - )

/// @brief 定数とカラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR4_( color, - )

/// @brief カラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR1_( color, * )

/// @brief カラー画素と定数の積
DEFINE_PROMOTE_BIND_OPERATOR2_( color, * )

/// @brief 定数とカラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR3_( color, * )

/// @brief カラー画素の割り算
DEFINE_PROMOTE_BIND_OPERATOR1_( color, / )

/// @brief カラー画素を定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2_( color, / )

/// @brief カラー画素の剰余
DEFINE_PROMOTE_BIND_OPERATOR1_( color, % )


/// @brief カラー画素の | 演算
DEFINE_PROMOTE_BIND_OPERATOR1_( color, | )

/// @brief カラー画素の & 演算
DEFINE_PROMOTE_BIND_OPERATOR1_( color, & )

/// @brief カラー画素の ^ 演算
DEFINE_PROMOTE_BIND_OPERATOR1_( color, ^ )




/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     c   … カラー画素
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2, 3 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const rgb< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << " )";
	return( out );
}


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     c   … カラー画素
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2, 3 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const rgba< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << ", ";
	out << c.a << " )";
	return( out );
}


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     c   … カラー画素
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2, 3, 1, 2, 3 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const nRGB< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << ", ";
	out << c.R << ", ";
	out << c.G << ", ";
	out << c.B << " )";
	return( out );
}


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     c   … カラー画素
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2, 3, 1, 2, 3 )
//! @endcode
//! 
template < class T, size_t NDIM > inline std::ostream &operator <<( std::ostream &out, const color< T, NDIM > &c )
{
	out << "( ";
	for( size_t i = 0 ; i < NDIM - 1 ; i++ )
	{
		out << c[ i ] << ", ";
	}
	out << c[ NDIM - 1 ] << " )";
	return( out );
}



/// @brief RGB色空間をHSV色空間に変換する
//! 
//! @attention 入力と出力の値の範囲に注意！！
//! 
//! @param[in]  r … RGB色空間のR(赤)成分(0～255)
//! @param[in]  g … RGB色空間のG(緑)成分(0～255)
//! @param[in]  b … RGB色空間のB(青)成分(0～255)
//! @param[out] h … HSV色空間のH(色相)成分(0～360)
//! @param[out] s … HSV色空間のS(彩度)成分(0～１)
//! @param[out] v … HSV色空間のV(明度)成分(0～１)
//! 
inline void rgb2hsv( double r, double g, double b, double &h, double &s, double &v )
{
	double max = __color_utility__::maximum( r, __color_utility__::maximum( g, b ) );
	double min = __color_utility__::minimum( r, __color_utility__::minimum( g, b ) );

	double d = max - min;
	v = max / 255.0;

	if( d != 0.0 )
	{
		s = d / max;
	}
	else
	{
		s = 0.0;
	}

	if( s == 0.0 )
	{
		h = 0.0;
	}
	else
	{
		double rt = max - r * 60.0 / d;
		double gt = max - g * 60.0 / d;
		double bt = max - b * 60.0 / d;

		if( r == max )
		{
			h = bt - gt;
		}
		else if( g == max )
		{
			h = 120 + rt - bt;
		}
		else
		{
			h = 240 + gt - rt;
		}

		if( h < 0.0 )
		{
			h += 360.0;
		}
	}
}


/// @brief HSV色空間をRGB色空間に変換する
//! 
//! @attention 入力と出力の値の範囲に注意！！
//! 
//! @param[in]  h … HSV色空間のH(色相)成分(0～360)
//! @param[in]  s … HSV色空間のS(彩度)成分(0～１)
//! @param[in]  v … HSV色空間のV(明度)成分(0～１)
//! @param[out] r … RGB色空間のR(赤)成分(0～255)
//! @param[out] g … RGB色空間のG(緑)成分(0～255)
//! @param[out] b … RGB色空間のB(青)成分(0～255)
//! 
inline void hsv2rgb( double h, double s, double v, double &r, double &g, double &b )
{
	if( s == 0.0 )
	{
		r = g = b = v * 255.0;
	}
	else
	{
		int ht = static_cast< int >( h * 6.0 );
		int d  = ht % 360;

		ht /= 360;

		double t1 = v * ( 1.0 - s );
		double t2 = v * ( 1.0 - s * d / 360.0 );
		double t3 = v * ( 1.0 - s * ( 360.0 - d ) / 360.0 );

		switch( ht )
		{
		case 0:
			r = v;
			g = t3;
			b = t1;
			break;

		case 1:
			r = t2;
			g = v;
			b = t1;
			break;

		case 2:
			r = t1;
			g = v;
			b = t3;
			break;

		case 3:
			r = t1;
			g = t2;
			b = v;
			break;

		case 4:
			r = t3;
			g = t1;
			b = v;
			break;

		default:
			r = v;
			g = t1;
			b = t2;
			break;
		}

		r *= 255.0;
		g *= 255.0;
		b *= 255.0;
	}
}




/// @brief RGB色空間をXYZ色空間に変換する
//! 
//! @attention 入力と出力の値の範囲に注意！！
//! 
//! @param[in]  r … RGB色空間のR(赤)成分(0～255)
//! @param[in]  g … RGB色空間のG(緑)成分(0～255)
//! @param[in]  b … RGB色空間のB(青)成分(0～255)
//! @param[out] x … XYZ色空間のX成分(0～１)
//! @param[out] y … XYZ色空間のY成分(0～１)
//! @param[out] z … XYZ色空間のZ成分(0～１)
//! 
inline void rgb2xyz( double r, double g, double b, double &x, double &y, double &z )
{
	x = ( 0.412453 * r + 0.357580 * g + 0.180423 * b ) / 255.0;
	y = ( 0.212671 * r + 0.715160 * g + 0.072169 * b ) / 255.0;
	z = ( 0.019334 * r + 0.119193 * g + 0.950227 * b ) / 255.0;
}


/// @brief XYZ色空間をRGB色空間に変換する
//! 
//! @attention 入力と出力の値の範囲に注意！！
//! 
//! @param[in]  x … XYZ色空間のX成分(0～１)
//! @param[in]  y … XYZ色空間のY成分(0～１)
//! @param[in]  z … XYZ色空間のZ成分(0～１)
//! @param[out] r … RGB色空間のR(赤)成分(0～255)
//! @param[out] g … RGB色空間のG(緑)成分(0～255)
//! @param[out] b … RGB色空間のB(青)成分(0～255)
//! 
inline void xyz2rgb( double x, double y, double z, double &r, double &g, double &b )
{
	r = (  3.240479 * x - 1.537150 * y - 0.498535 * z ) * 255.0;
	g = ( -0.969256 * x + 1.875991 * y + 0.041556 * z ) * 255.0;
	b = (  0.055648 * x - 0.204043 * y + 1.057311 * z ) * 255.0;
}


/// @brief RGB色空間をL*a*b*色空間に変換する
//! 
//! @attention 入力と出力の値の範囲に注意！！
//! 
//! @param[in]  r … RGB色空間のR(赤)成分(0～255)
//! @param[in]  g … RGB色空間のG(緑)成分(0～255)
//! @param[in]  b … RGB色空間のB(青)成分(0～255)
//! @param[out] l_ … L*a*b*色空間のL(明度指数)成分(0～100)
//! @param[out] a_ … L*a*b*色空間のa(知覚色度)成分(-134～220)
//! @param[out] b_ … L*a*b*色空間のb(知覚色度)成分(-140～122)
//! 
inline void rgb2lab( double r, double g, double b, double &l_, double &a_, double &b_ )
{
	double x, y, z;

	// XYZ表色系からRGB表色系へ変換
	rgb2xyz( r, g, b, x, y, z );

	static const double Xr = 0.9504;
	static const double Yr = 1.0;
	static const double Zr = 1.0889;

	x /= Xr;
	y /= Yr;
	z /= Zr;

	static const double th = 216.0 / 24389.0;
	static const double _1_3 = 1.0 / 3.0;
	static const double A = 29.0 * 29.0 / ( 3.0 * 6.0 * 6.0 );
	static const double B = 4.0 / 29.0;
	x = x > th ? std::pow( x, _1_3 ) : A * x + B;
	y = y > th ? std::pow( y, _1_3 ) : A * y + B;
	z = z > th ? std::pow( z, _1_3 ) : A * z + B;

	l_ = 116.0 * y - 16.0;
	a_ = 500.0 * ( x - y );
	b_ = 200.0 * ( y - z );
}

/// @brief L*a*b*色空間をRGB色空間に変換する
//! 
//! @attention 入力と出力の値の範囲に注意！！
//! 
//! @param[in]  l_ … L*a*b*色空間のL(明度指数)成分(0～100)
//! @param[in]  a_ … L*a*b*色空間のa(知覚色度)成分(-134～220)
//! @param[in]  b_ … L*a*b*色空間のb(知覚色度)成分(-140～122)
//! @param[out] r … RGB色空間のR(赤)成分(0～255)
//! @param[out] g … RGB色空間のG(緑)成分(0～255)
//! @param[out] b … RGB色空間のB(青)成分(0～255)
//! 
inline void lab2rgb( double l_, double a_, double b_, double &r, double &g, double &b )
{
	double fy = ( l_ + 16.0 ) / 116.0;
	double fx = fy + a_ / 500.0 ;
	double fz = fy - b_ / 200.0;

	static const double Xr = 0.9504;
	static const double Yr = 1.0;
	static const double Zr = 1.0889;

	static const double delta = 6.0 / 29.0;
	static const double delta2 = 3.0 * delta * delta;
	double y = fy > delta ? Yr * fy * fy * fy : ( fy - 16.0 / 116.0 ) * delta2 * Yr;
	double x = fx > delta ? Xr * fx * fx * fx : ( fx - 16.0 / 116.0 ) * delta2 * Xr;
	double z = fz > delta ? Zr * fz * fz * fz : ( fz - 16.0 / 116.0 ) * delta2 * Zr;

	// XYZ表色系からRGB表色系へ変換
	xyz2rgb( x, y, z, r, g, b );
}

/// @brief 入力された型がカラー画像かどうか調べる
template < class T >
struct is_color
{
	_MIST_CONST( bool, value, false );
};


/// @}
//  MIST全般で利用できるMIST全般で利用可能な色を表現するクラス


// 画素の変換をサポートするためのコンバータ
template < class T >
struct _pixel_converter_
{
	typedef T value_type;
	typedef rgba< T > color_type;
	enum{ color_num = 1 };

	static value_type convert_to( value_type r, value_type g, value_type b, value_type /* a */ = 255 )
	{
		return( color_type( r, g, b ).get_value( ) );
	}

	static color_type convert_from( const value_type &pixel )
	{
		return( color_type( pixel ) );
	}
};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_COLOR( type ) \
		template < >\
		struct is_color< rgb< type > >\
		{\
			enum{ value = true };\
		};\
		template < >\
		struct is_color< bgr< type > >\
		{\
			enum{ value = true };\
		};\
		template < >\
		struct is_color< rgba< type > >\
		{\
			enum{ value = true };\
		};\
		template < >\
		struct is_color< bgra< type > >\
		{\
			enum{ value = true };\
		};\


	#define __PIXEL_CONVERTER__( type ) \
		template < >\
		struct _pixel_converter_< rgb< type > >\
		{\
			typedef type value_type;\
			typedef rgba< type > color_type;\
			enum{ color_num = 3 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\
		template < >\
		struct _pixel_converter_< bgr< type > >\
		{\
			typedef type value_type;\
			typedef rgba< type > color_type;\
			enum{ color_num = 3 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\
		template < >\
		struct _pixel_converter_< rgba< type > >\
		{\
			typedef type value_type;\
			typedef rgba< type > color_type;\
			enum{ color_num = 4 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b, a ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\
		template < >\
		struct _pixel_converter_< bgra< type > >\
		{\
			typedef type value_type;\
			typedef bgra< type > color_type;\
			enum{ color_num = 4 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b, a ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\

		// 各型に対する特殊化
		IS_COLOR(unsigned char)
		IS_COLOR(unsigned short)
		IS_COLOR(unsigned int)
		IS_COLOR(unsigned long)
		IS_COLOR(signed char)
		IS_COLOR(signed short)
		IS_COLOR(signed int)
		IS_COLOR(signed long)
		IS_COLOR(bool)
		IS_COLOR(char)
		IS_COLOR(float)
		IS_COLOR(double)
		IS_COLOR(long double)
		__PIXEL_CONVERTER__(unsigned char)
		__PIXEL_CONVERTER__(unsigned short)
		__PIXEL_CONVERTER__(unsigned int)
		__PIXEL_CONVERTER__(unsigned long)
		__PIXEL_CONVERTER__(signed char)
		__PIXEL_CONVERTER__(signed short)
		__PIXEL_CONVERTER__(signed int)
		__PIXEL_CONVERTER__(signed long)
		__PIXEL_CONVERTER__(bool)
		__PIXEL_CONVERTER__(char)
		__PIXEL_CONVERTER__(float)
		__PIXEL_CONVERTER__(double)
		__PIXEL_CONVERTER__(long double)

		#undef IS_COLOR
		#undef __PIXEL_CONVERTER__

#else

	template < class T >
	struct is_color< rgb< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< bgr< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< rgba< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< bgra< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< nRGB< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T, size_t NDIM >
	struct is_color< color< T, NDIM > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct _pixel_converter_< rgb< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const rgb< T > &pixel )
		{
			return( pixel );
		}
	};

	template < class T >
	struct _pixel_converter_< bgr< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const bgr< T > &pixel )
		{
			return( color_type( pixel.r, pixel.g, pixel.b ) );
		}
	};

	template < class T >
	struct _pixel_converter_< rgba< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 4 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const rgba< T > &pixel )
		{
			return( pixel );
		}
	};

	template < class T >
	struct _pixel_converter_< bgra< T > >
	{
		typedef T value_type;
		typedef bgra< T > color_type;
		enum{ color_num = 4 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const bgra< T > &pixel )
		{
			return( color_type( pixel.r, pixel.g, pixel.b, pixel.a ) );
		}
	};

	template < class T >
	struct _pixel_converter_< nRGB< T > >
	{
		typedef T value_type;
		typedef nRGB< T > color_type;
		enum{ color_num = 6 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type /* a */ = 255 )
		{
			return( color_type( r, g, b ) );
		}

		static color_type convert_from( const nRGB< T > &pixel )
		{
			return( color_type( pixel.r, pixel.g, pixel.b ) );
		}
	};

	template < class T, size_t NDIM >
	struct _pixel_converter_< color< T, NDIM > >
	{
		typedef T value_type;
		typedef color< T, NDIM > color_type;
		enum{ color_num = NDIM };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( ( r + g + b ) / 3 ) );
		}

		static color_type convert_from( const color< T, NDIM > &pixel )
		{
			if( NDIM < 3 )
			{
				return( color_type( pixel[ 0 ] ) );
			}
			else
			{
				return( color_type( pixel[ 0 ], pixel[ 1 ], pixel[ 2 ] ) );
			}
		}
	};

#endif


#define __DEFINE_COLOR_TYPE_TRAIT__( function, type ) \
	template<> struct function<  rgb< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function<  bgr< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function< rgba< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function< bgra< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function<  nRGB< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template< size_t NDIM > struct function<  color< type, NDIM > >{ _MIST_CONST( bool, value, true  ); }; \

// type_trait 内の機能を拡張する
/// @brief char 判定
//! 
//! Tが１バイトの文字列型であれば真に評価する
//! 
//! @param T  … 調査する型
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_char, unsigned char )
__DEFINE_COLOR_TYPE_TRAIT__( is_char, signed char )
__DEFINE_COLOR_TYPE_TRAIT__( is_char, char )


/// @brief 浮動小数点 判定
//! 
//! T が float か double 型であれば真に評価する
//! 
//! @param T  … 調査する型
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_float, float )
__DEFINE_COLOR_TYPE_TRAIT__( is_float, double )
__DEFINE_COLOR_TYPE_TRAIT__( is_float, long double )



/// @brief integer 判定
//! 
//! T が整数型であれば真に評価する。汎整数型がこれにあたる
//! 
//! @param T  … 調査する型
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned char )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned short )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned int )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned long )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed char )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed short )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed int )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed long )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, bool )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, char )

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, size_t )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, ptrdiff_t )
#endif



/// @brief arithmetic 判定
//! 
//! T が算術型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
//! 
//! @param T  … 調査する型
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned char )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned short )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned int )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned long )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed char )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed short )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed int )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed long )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, bool )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, char )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, float )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, double )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, long double )

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, size_t )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, ptrdiff_t )
#endif


/// @brief 対応する float 型を返す
//! 
//! T が float の場合は float を返し，それ以外は double （もしくは long double）とする
//! 
//! @param T  … 調査する型
//! 
template< class T > struct float_type<  rgb< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };
template< class T > struct float_type<  bgr< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };
template< class T > struct float_type< rgba< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };
template< class T > struct float_type< bgra< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };


#undef __DEFINE_COLOR_TYPE_TRAIT__

/// @brief 定義済み色
template< typename T = rgb< unsigned char > >
struct colors
{
	typedef T color_type;

	static color_type aliceblue( )				{ return color_type( 0xf0, 0xf8, 0xff ); }
	static color_type antiquewhite( )			{ return color_type( 0xfa, 0xeb, 0xd7 ); }
	static color_type aqua( )					{ return color_type( 0x00, 0xff, 0xff ); }
	static color_type aquamarine( )				{ return color_type( 0x7f, 0xff, 0xd4 ); }
	static color_type azure( )					{ return color_type( 0xf0, 0xff, 0xff ); }
	static color_type beige( )					{ return color_type( 0xf5, 0xf5, 0xdc ); }
	static color_type bisque( )					{ return color_type( 0xff, 0xe4, 0xc4 ); }
	static color_type black( )					{ return color_type( 0x00, 0x00, 0x00 ); }
	static color_type blanchedalmond( )			{ return color_type( 0xff, 0xeb, 0xcd ); }
	static color_type blue( )					{ return color_type( 0x00, 0x00, 0xff ); }
	static color_type blueviolet( )				{ return color_type( 0x8a, 0x2b, 0xe2 ); }
	static color_type brown( )					{ return color_type( 0xa5, 0x2a, 0x2a ); }
	static color_type burlywood( )				{ return color_type( 0xde, 0xb8, 0x87 ); }
	static color_type cadetblue( )				{ return color_type( 0x5f, 0x9e, 0xa0 ); }
	static color_type chartreuse( )				{ return color_type( 0x7f, 0xff, 0x00 ); }
	static color_type chocolate( )				{ return color_type( 0xd2, 0x69, 0x1e ); }
	static color_type coral( )					{ return color_type( 0xff, 0x7f, 0x50 ); }
	static color_type cornflowerblue( )			{ return color_type( 0x64, 0x95, 0xed ); }
	static color_type cornsilk( )				{ return color_type( 0xff, 0xf8, 0xdc ); }
	static color_type crimson( )				{ return color_type( 0xdc, 0x14, 0x3c ); }
	static color_type cyan( )					{ return color_type( 0x00, 0xff, 0xff ); }
	static color_type darkblue( )				{ return color_type( 0x00, 0x00, 0x8b ); }
	static color_type darkcyan( )				{ return color_type( 0x00, 0x8b, 0x8b ); }
	static color_type darkgoldenrod( )			{ return color_type( 0xb8, 0x86, 0x0b ); }
	static color_type darkgray( )				{ return color_type( 0xa9, 0xa9, 0xa9 ); }
	static color_type darkgreen( )				{ return color_type( 0x00, 0x64, 0x00 ); }
	static color_type darkkhaki( )				{ return color_type( 0xbd, 0xb7, 0x6b ); }
	static color_type darkmagenta( )			{ return color_type( 0x8b, 0x00, 0x8b ); }
	static color_type darkolivegreen( )			{ return color_type( 0x55, 0x6b, 0x2f ); }
	static color_type darkorange( )				{ return color_type( 0xff, 0x8c, 0x00 ); }
	static color_type darkorchid( )				{ return color_type( 0x99, 0x32, 0xcc ); }
	static color_type darkred( )				{ return color_type( 0x8b, 0x00, 0x00 ); }
	static color_type darksalmon( )				{ return color_type( 0xe9, 0x96, 0x7a ); }
	static color_type darkseagreen( )			{ return color_type( 0x8f, 0xbc, 0x8f ); }
	static color_type darkslateblue( )			{ return color_type( 0x48, 0x3d, 0x8b ); }
	static color_type darkslategray( )			{ return color_type( 0x2f, 0x4f, 0x4f ); }
	static color_type darkturquoise( )			{ return color_type( 0x00, 0xce, 0xd1 ); }
	static color_type darkviolet( )				{ return color_type( 0x94, 0x00, 0xd3 ); }
	static color_type deeppink( )				{ return color_type( 0xff, 0x14, 0x93 ); }
	static color_type deepskyblue( )			{ return color_type( 0x00, 0xbf, 0xff ); }
	static color_type dimgray( )				{ return color_type( 0x69, 0x69, 0x69 ); }
	static color_type dodgerblue( )				{ return color_type( 0x1e, 0x90, 0xff ); }
	static color_type firebrick( )				{ return color_type( 0xb2, 0x22, 0x22 ); }
	static color_type floralwhite( )			{ return color_type( 0xff, 0xfa, 0xf0 ); }
	static color_type forestgreen( )			{ return color_type( 0x22, 0x8b, 0x22 ); }
	static color_type fuchsia( )				{ return color_type( 0xff, 0x00, 0xff ); }
	static color_type gainsboro( )				{ return color_type( 0xdc, 0xdc, 0xdc ); }
	static color_type ghostwhite( )				{ return color_type( 0xf8, 0xf8, 0xff ); }
	static color_type gold( )					{ return color_type( 0xff, 0xd7, 0x00 ); }
	static color_type goldenrod( )				{ return color_type( 0xda, 0xa5, 0x20 ); }
	static color_type gray( )					{ return color_type( 0x80, 0x80, 0x80 ); }
	static color_type green( )					{ return color_type( 0x00, 0xff, 0x00 ); }
	static color_type greenyellow( )			{ return color_type( 0xad, 0xff, 0x2f ); }
	static color_type honeydew( )				{ return color_type( 0xf0, 0xff, 0xf0 ); }
	static color_type hotpink( )				{ return color_type( 0xff, 0x69, 0xb4 ); }
	static color_type indianred( )				{ return color_type( 0xcd, 0x5c, 0x5c ); }
	static color_type indigo( )					{ return color_type( 0x4b, 0x00, 0x82 ); }
	static color_type ivory( )					{ return color_type( 0xff, 0xff, 0xf0 ); }
	static color_type khaki( )					{ return color_type( 0xf0, 0xe6, 0x8c ); }
	static color_type lavender( )				{ return color_type( 0xe6, 0xe6, 0xfa ); }
	static color_type lavenderblush( )			{ return color_type( 0xff, 0xf0, 0xf5 ); }
	static color_type lawngreen( )				{ return color_type( 0x7c, 0xfc, 0x00 ); }
	static color_type lemonchiffon( )			{ return color_type( 0xff, 0xfa, 0xcd ); }
	static color_type lightblue( )				{ return color_type( 0xad, 0xd8, 0xe6 ); }
	static color_type lightcoral( )				{ return color_type( 0xf0, 0x80, 0x80 ); }
	static color_type lightcyan( )				{ return color_type( 0xe0, 0xff, 0xff ); }
	static color_type lightgoldenrodyellow( )	{ return color_type( 0xfa, 0xfa, 0xd2 ); }
	static color_type lightgray( )				{ return color_type( 0xd3, 0xd3, 0xd3 ); }
	static color_type lightgreen( )				{ return color_type( 0x90, 0xee, 0x90 ); }
	static color_type lightpink( )				{ return color_type( 0xff, 0xb6, 0xc1 ); }
	static color_type lightsalmon( )			{ return color_type( 0xff, 0xa0, 0x7a ); }
	static color_type lightseagreen( )			{ return color_type( 0x20, 0xb2, 0xaa ); }
	static color_type lightskyblue( )			{ return color_type( 0x87, 0xce, 0xfa ); }
	static color_type lightslategray( )			{ return color_type( 0x77, 0x88, 0x99 ); }
	static color_type lightsteelblue( )			{ return color_type( 0xb0, 0xc4, 0xde ); }
	static color_type lightyellow( )			{ return color_type( 0xff, 0xff, 0xe0 ); }
	static color_type lime( )					{ return color_type( 0x00, 0xff, 0x00 ); }
	static color_type limegreen( )				{ return color_type( 0x32, 0xcd, 0x32 ); }
	static color_type linen( )					{ return color_type( 0xfa, 0xf0, 0xe6 ); }
	static color_type magenta( )				{ return color_type( 0xff, 0x00, 0xff ); }
	static color_type maroon( )					{ return color_type( 0x80, 0x00, 0x00 ); }
	static color_type mediumaquamarine( )		{ return color_type( 0x66, 0xcd, 0xaa ); }
	static color_type mediumblue( )				{ return color_type( 0x00, 0x00, 0xcd ); }
	static color_type mediumorchid( )			{ return color_type( 0xba, 0x55, 0xd3 ); }
	static color_type mediumpurple( )			{ return color_type( 0x93, 0x70, 0xdb ); }
	static color_type mediumseagreen( )			{ return color_type( 0x3c, 0xb3, 0x71 ); }
	static color_type mediumslateblue( )		{ return color_type( 0x7b, 0x68, 0xee ); }
	static color_type mediumspringgreen( )		{ return color_type( 0x00, 0xfa, 0x9a ); }
	static color_type mediumturquoise( )		{ return color_type( 0x48, 0xd1, 0xcc ); }
	static color_type mediumvioletred( )		{ return color_type( 0xc7, 0x15, 0x85 ); }
	static color_type midnightblue( )			{ return color_type( 0x19, 0x19, 0x70 ); }
	static color_type mintcream( )				{ return color_type( 0xf5, 0xff, 0xfa ); }
	static color_type mistyrose( )				{ return color_type( 0xff, 0xe4, 0xe1 ); }
	static color_type moccasin( )				{ return color_type( 0xff, 0xe4, 0xb5 ); }
	static color_type navajowhite( )			{ return color_type( 0xff, 0xde, 0xad ); }
	static color_type navy( )					{ return color_type( 0x00, 0x00, 0x80 ); }
	static color_type oldlace( )				{ return color_type( 0xfd, 0xf5, 0xe6 ); }
	static color_type olive( )					{ return color_type( 0x80, 0x80, 0x00 ); }
	static color_type olivedrab( )				{ return color_type( 0x6b, 0x8e, 0x23 ); }
	static color_type orange( )					{ return color_type( 0xff, 0xa5, 0x00 ); }
	static color_type orangered( )				{ return color_type( 0xff, 0x45, 0x00 ); }
	static color_type orchid( )					{ return color_type( 0xda, 0x70, 0xd6 ); }
	static color_type palegoldenrod( )			{ return color_type( 0xee, 0xe8, 0xaa ); }
	static color_type palegreen( )				{ return color_type( 0x98, 0xfb, 0x98 ); }
	static color_type paleturquoise( )			{ return color_type( 0xaf, 0xee, 0xee ); }
	static color_type palevioletred( )			{ return color_type( 0xdb, 0x70, 0x93 ); }
	static color_type papayawhip( )				{ return color_type( 0xff, 0xef, 0xd5 ); }
	static color_type peachpuff( )				{ return color_type( 0xff, 0xda, 0xb9 ); }
	static color_type peru( )					{ return color_type( 0xcd, 0x85, 0x3f ); }
	static color_type pink( )					{ return color_type( 0xff, 0xc0, 0xcb ); }
	static color_type plum( )					{ return color_type( 0xdd, 0xa0, 0xdd ); }
	static color_type powderblue( )				{ return color_type( 0xb0, 0xe0, 0xe6 ); }
	static color_type purple( )					{ return color_type( 0x80, 0x00, 0x80 ); }
	static color_type red( )					{ return color_type( 0xff, 0x00, 0x00 ); }
	static color_type rosybrown( )				{ return color_type( 0xbc, 0x8f, 0x8f ); }
	static color_type royalblue( )				{ return color_type( 0x41, 0x69, 0xe1 ); }
	static color_type saddlebrown( )			{ return color_type( 0x8b, 0x45, 0x13 ); }
	static color_type salmon( )					{ return color_type( 0xfa, 0x80, 0x72 ); }
	static color_type sandybrown( )				{ return color_type( 0xf4, 0xa4, 0x60 ); }
	static color_type seagreen( )				{ return color_type( 0x2e, 0x8b, 0x57 ); }
	static color_type seashell( )				{ return color_type( 0xff, 0xf5, 0xee ); }
	static color_type sienna( )					{ return color_type( 0xa0, 0x52, 0x2d ); }
	static color_type silver( )					{ return color_type( 0xc0, 0xc0, 0xc0 ); }
	static color_type skyblue( )				{ return color_type( 0x87, 0xce, 0xeb ); }
	static color_type slateblue( )				{ return color_type( 0x6a, 0x5a, 0xcd ); }
	static color_type slategray( )				{ return color_type( 0x70, 0x80, 0x90 ); }
	static color_type snow( )					{ return color_type( 0xff, 0xfa, 0xfa ); }
	static color_type springgreen( )			{ return color_type( 0x00, 0xff, 0x7f ); }
	static color_type steelblue( )				{ return color_type( 0x46, 0x82, 0xb4 ); }
	static color_type tan( )					{ return color_type( 0xd2, 0xb4, 0x8c ); }
	static color_type teal( )					{ return color_type( 0x00, 0x80, 0x80 ); }
	static color_type thistle( )				{ return color_type( 0xd8, 0xbf, 0xd8 ); }
	static color_type tomato( )					{ return color_type( 0xff, 0x63, 0x47 ); }
	static color_type transparent( )			{ return color_type( 0x00, 0x00, 0xe0 ); }
	static color_type turquoise( )				{ return color_type( 0x40, 0xe0, 0xd0 ); }
	static color_type violet( )					{ return color_type( 0xee, 0x82, 0xee ); }
	static color_type wheat( )					{ return color_type( 0xf5, 0xde, 0xb3 ); }
	static color_type white( )					{ return color_type( 0xff, 0xff, 0xff ); }
	static color_type whitesmoke( )				{ return color_type( 0xf5, 0xf5, 0xf5 ); }
	static color_type yellow( )					{ return color_type( 0xff, 0xff, 0x00 ); }
	static color_type yellowgreen( )			{ return color_type( 0x9a, 0xcd, 0x32 ); }
};


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_COLOR_H__
