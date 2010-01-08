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

/// @file mist/config/stereo.h
//!
//! @brief ステレオ音声を扱うためのクラス
//!

#ifndef __INCLUDE_MIST_STEREO__
#define __INCLUDE_MIST_STEREO__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "type_trait.h"
#endif

#include <iostream>

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型


/// @brief ステレオ音声用のデータ型
//! 
//! @code ステレオ音声の作成例
//! mist::array< mist::stereo< double > > sound;
//! @endcode
//! 
//! @param T … 各音声成分のデータ型
//! 
template< class T >
struct stereo
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
	value_type l;		///< @brief Leftチャンネルく
	value_type r;		///< @brief Rightチャンネル

	/// @brief デフォルトコンストラクタ（全ての要素を0で初期化する）
	stereo( ) : l( 0 ), r( 0 ){ }

	/// @brief 全ての成分を mono で初期化する
	stereo( const value_type &mono ) : l( mono ), r( mono ){ }

	/// @brief 左 left，右 right を用いて初期化する
	stereo( const value_type &left, const value_type &right ) : l( left ), r( right ){ }

	/// @brief 異なる型のステレオ音声データを用いて初期化する
	template < class TT >
	stereo( const stereo< TT > &s ) : l( static_cast< value_type >( s.l ) ), r( static_cast< value_type >( s.r ) ){ }

	/// @brief 他のステレオ音声データを用いて初期化する
	stereo( const stereo< T > &s ) : l( s.l ), r( s.r ){ }



	/// @brief 他の音声成分を代入する
	template < class TT >
	const stereo &operator =( const stereo< TT > &s )
	{
		l = static_cast< value_type >( s.l );
		r = static_cast< value_type >( s.r );
		return( *this );
	}

	/// @brief 他の音声成分を代入する
	const stereo &operator =( const stereo< T > &s )
	{
		if( &s != this )
		{
			l = s.l;
			r = s.r;
		}
		return( *this );
	}


	/// @brief 全ての要素に mono を代入する
	const stereo &operator =( const value_type &mono )
	{
		l = mono;
		r = mono;
		return( *this );
	}


	/// @brief 各音声成分の和
	template < class TT >
	const stereo &operator +=( const stereo< TT > &s ){ l = static_cast< value_type >( l + s.l ); r = static_cast< value_type >( r + s.r ); return( *this ); }

	/// @brief 各音声成分の差
	template < class TT >
	const stereo &operator -=( const stereo< TT > &s ){ l = static_cast< value_type >( l - s.l ); r = static_cast< value_type >( r - s.r ); return( *this ); }

	/// @brief 各音声成分の積
	template < class TT >
	const stereo &operator *=( const stereo< TT > &s ){ l = static_cast< value_type >( l * s.l ); r = static_cast< value_type >( r * s.r ); return( *this ); }

	/// @brief 各音声成分の割り算
	template < class TT >
	const stereo &operator /=( const stereo< TT > &s ){ l = static_cast< value_type >( l / s.l ); r = static_cast< value_type >( r / s.r ); return( *this ); }

	/// @brief 各音声成分の剰余
	const stereo &operator %=( const stereo &s ){ l %= s.l; r %= s.r; return( *this ); }

	/// @brief 各音声成分の | 演算
	const stereo &operator |=( const stereo &s ){ l |= s.l; r |= s.r; return( *this ); }

	/// @brief 各音声成分の & 演算
	const stereo &operator &=( const stereo &s ){ l &= s.l; r &= s.r; return( *this ); }

	/// @brief 各音声成分の & 演算
	const stereo &operator ^=( const stereo &s ){ l ^= s.l; r ^= s.r; return( *this ); }



	/// @brief 各音声成分に mono 値を足す
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator +=( const double &mono )
#else
	template < class TT >
	const stereo &operator +=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l + mono );
		r = static_cast< value_type >( r + mono );
		return( *this );
	}

	/// @brief 各音声成分から mono 値を引く
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator -=( const double &mono )
#else
	template < class TT >
	const stereo &operator -=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l - mono );
		r = static_cast< value_type >( r - mono );
		return( *this );
	}

	/// @brief 各音声成分に mono 値を掛ける
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator *=( const double &mono )
#else
	template < class TT >
	const stereo &operator *=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l * mono );
		r = static_cast< value_type >( r * mono );
		return( *this );
	}

	/// @brief 各音声成分を mono 値で割る
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator /=( const double &mono )
#else
	template < class TT >
	const stereo &operator /=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l / mono );
		r = static_cast< value_type >( r / mono );
		return( *this );
	}


	/// @brief 2つの音声成分が等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_l = q_l \; \wedge \; p_r = q_r \f]
	//! 
	//! @param[in] s … 右辺値
	//! 
	//! @retval true  … 2つの音声成分が等しい場合
	//! @retval false … 2つの音声成分が異なる場合
	//! 
	bool operator ==( const stereo &s ) const { return( l == s.l && r == s.r ); }

	/// @brief 2つの音声成分が等しくない（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_l = q_l \; \wedge \; p_r = q_r } \f]
	//! 
	//! @param[in] s … 右辺値
	//! 
	//! @retval true  … 2つの音声成分が異なる場合
	//! @retval false … 2つの音声成分が等しい場合
	//! 
	bool operator !=( const stereo &s ) const { return( !( *this == s ) ); }

	/// @brief 2つの音声成分の < を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_l \ge q_l \; \wedge \; p_r \ge q_r } \f]
	//! 
	//! @param[in] s … 右辺値
	//! 
	//! @retval true  … s1 <  s2 の場合
	//! @retval false … s1 >= s2 の場合
	//! 
	bool operator < ( const stereo &s ) const
	{
		if( l == s.l )
		{
			return( r < s.r );
		}
		else
		{
			return( l < s.l );
		}
	}

	/// @brief 2つの音声成分の <= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_l \le q_l \; \wedge \; p_r \le q_r \f]
	//! 
	//! @param[in] s … 右辺値
	//! 
	//! @retval true  … s1 <= s2 の場合
	//! @retval false … s1 >  s2 の場合
	//! 
	bool operator <=( const stereo &s ) const { return( s >= *this ); }

	/// @brief 2つの音声成分の > を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_l \le q_l \; \wedge \; p_r \le q_r } \f]
	//! 
	//! @param[in] s … 右辺値
	//! 
	//! @retval true  … s1 >  s2 の場合
	//! @retval false … s1 <= s2 の場合
	//! 
	bool operator > ( const stereo &s ) const { return( s < *this ); }

	/// @brief 2つの音声成分の >= を判定する
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_l \ge q_l \; \wedge \; p_r \ge q_r \f]
	//! 
	//! @param[in] s … 右辺値
	//! 
	//! @retval true  … s1 >= s2 の場合
	//! @retval false … s1 <  s2 の場合
	//! 
	bool operator >=( const stereo &s ) const { return( !( *this < s ) ); }


	/// @brief ステレオ音声からモノラル音声に変換する
	value_type get_value( ) const
	{
		return( static_cast< value_type >( ( l + r ) / 2.0 ) );
	}

	// ステレオからモノラルへの自動キャスト演算子（危険のため一時停止）
	//operator value_type( ) const { return( get_value( ) ); }

};

/// @brief ステレオ音声データの和
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, + )

/// @brief ステレオ音声データと定数の和
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, + )

/// @brief 定数とステレオ音声データの和
DEFINE_PROMOTE_BIND_OPERATOR3( stereo, + )

/// @brief ステレオ音声データの差
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, - )

/// @brief ステレオ音声データと定数の差
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, - )

/// @brief 定数とステレオ音声データの差
DEFINE_PROMOTE_BIND_OPERATOR4( stereo, - )

/// @brief ステレオ音声データの積
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, * )

/// @brief ステレオ音声データと定数の積
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, * )

/// @brief 定数とステレオ音声データの積
DEFINE_PROMOTE_BIND_OPERATOR3( stereo, * )

/// @brief ステレオ音声データの割り算
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, / )

/// @brief ステレオ音声データを定数で割る
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, / )

/// @brief ステレオ音声データの剰余
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, % )

/// @brief ステレオ音声データの | 演算
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, | )

/// @brief ステレオ音声データの & 演算
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, & )

/// @brief ステレオ音声データの ^ 演算
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, ^ )



/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     s   … ステレオ音声
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! ( 1, 2 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const stereo< T > &s )
{
	out << "( ";
	out << s.l << ", ";
	out << s.r << " )";
	return( out );
}


/// @brief 入力された型が音声データかどうか調べる
template < class T >
struct is_stereo
{
	_MIST_CONST( bool, value, false );
};


// 画素の変換をサポートするためのコンバータ
template < class T >
struct _stereo_converter_
{
	typedef T value_type;
	typedef stereo< T > stereo_type;
	enum{ channel_num = 1 };

	static value_type convert_to( value_type l, value_type r )
	{
		return( stereo_type( l, r ).get_value( ) );
	}

	static stereo_type convert_from( const value_type &mono )
	{
		return( stereo_type( mono, mono ) );
	}
};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_STEREO( type ) \
		template < >\
		struct is_stereo< stereo< type > >\
		{\
			enum{ value = true };\
		};\


	#define __STEREO_CONVERTER__( type ) \
		template < >\
		struct _stereo_converter_< stereo< type > >\
		{\
			typedef type value_type;\
			typedef stereo< type > stereo_type;\
			enum{ channel_num = 2 };\
			\
			static stereo_type convert_to( value_type l, value_type r )\
			{\
				return( stereo_type( l, r ) );\
			}\
			\
			static stereo_type convert_from( const stereo_type &sound )\
			{\
				return( sound );\
			}\
		};\

		// 各型に対する特殊化
		IS_STEREO(unsigned char)
		IS_STEREO(unsigned short)
		IS_STEREO(unsigned int)
		IS_STEREO(unsigned long)
		IS_STEREO(signed char)
		IS_STEREO(signed short)
		IS_STEREO(signed int)
		IS_STEREO(signed long)
		IS_STEREO(bool)
		IS_STEREO(char)
		IS_STEREO(float)
		IS_STEREO(double)
		IS_STEREO(long double)
		__STEREO_CONVERTER__(unsigned char)
		__STEREO_CONVERTER__(unsigned short)
		__STEREO_CONVERTER__(unsigned int)
		__STEREO_CONVERTER__(unsigned long)
		__STEREO_CONVERTER__(signed char)
		__STEREO_CONVERTER__(signed short)
		__STEREO_CONVERTER__(signed int)
		__STEREO_CONVERTER__(signed long)
		__STEREO_CONVERTER__(bool)
		__STEREO_CONVERTER__(char)
		__STEREO_CONVERTER__(float)
		__STEREO_CONVERTER__(double)
		__STEREO_CONVERTER__(long double)

		#undef IS_STEREO
		#undef __STEREO_CONVERTER__

#else

	template < class T >
	struct is_stereo< stereo< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct _stereo_converter_< stereo< T > >
	{
		typedef T value_type;
		typedef stereo< T > stereo_type;
		enum{ channel_num = 2 };

		static stereo_type convert_to( value_type l, value_type r )
		{
			return( stereo_type( l, r ) );
		}

		static stereo_type convert_from( const stereo_type &sound )
		{
			return( sound );
		}
	};

#endif

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_STEREO__
