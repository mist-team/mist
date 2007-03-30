// 
// Copyright (c) 2003-2007, MIST Project, Intelligent Media Integration COE, Nagoya University
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


namespace __color_utility__
{
	template < class T > inline T maximum( const T &v1, const T &v2 )
	{
		return( v1 > v2 ? v1 : v2 );
	}

	template < class T > inline T minimum( const T &v1, const T &v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}
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

	static const double coef = 16.0 / 116.0;
	x = x > 0.008856 ? std::pow( x, 0.33333333333333333333333333333333333 ) : 7.787 * x + coef;
	y = y > 0.008856 ? std::pow( y, 0.33333333333333333333333333333333333 ) : 7.787 * y + coef;
	z = z > 0.008856 ? std::pow( z, 0.33333333333333333333333333333333333 ) : 7.787 * z + coef;

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
	static const double delta3 = 3.0 * delta * delta * delta;
	double y = fy > delta ? Yr * fy * fy * fy : ( fy - 16.0 / 116.0 ) * delta3 * Yr;
	double x = fx > delta ? Yr * fx * fx * fx : ( fx - 16.0 / 116.0 ) * delta3 * Xr;
	double z = fz > delta ? Yr * fz * fz * fz : ( fz - 16.0 / 116.0 ) * delta3 * Zr;

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

	static value_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
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
	struct _pixel_converter_< rgb< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const color_type &pixel )
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

		static color_type convert_from( const color_type &pixel )
		{
			return( pixel );
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

		static color_type convert_from( const color_type &pixel )
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

		static color_type convert_from( const color_type &pixel )
		{
			return( pixel );
		}
	};

#endif


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_COLOR_H__
