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

/// @file mist/config/type_trait.h
//!
//! @brief 型の情報を取得・判定するためのライブラリ
//!

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#define __INCLUDE_MIST_TYPE_TRAIT_H__

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


// 型属性を調べる


/// @brief char 判定
//! 
//! Tが１バイトの文字列型であれば真に評価する
//! 
//! @param T  … 調査する型
//! 
template< class T > struct is_char        { _MIST_CONST( bool, value, false ); };
template<> struct is_char< unsigned char >{ _MIST_CONST( bool, value, true  ); };
template<> struct is_char< signed char >  { _MIST_CONST( bool, value, true  ); };
template<> struct is_char< char >         { _MIST_CONST( bool, value, true  ); };


/// @brief 浮動小数点 判定
//! 
//! T が float か double 型であれば真に評価する
//! 
//! @param T  … 調査する型
//! 
template< class T > struct is_float       { _MIST_CONST( bool, value, false ); };
template<> struct is_float< float >       { _MIST_CONST( bool, value, true  ); };
template<> struct is_float< double >      { _MIST_CONST( bool, value, true  ); };
template<> struct is_float< long double > { _MIST_CONST( bool, value, true  ); };



/// @brief integer 判定
//! 
//! T が整数型であれば真に評価する。汎整数型がこれにあたる
//! 
//! @param T  … 調査する型
//! 
template< class T > struct is_integer                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_integer< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< char >          { _MIST_CONST( bool, value, true  ); };

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
template<>          struct is_integer< size_t >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< ptrdiff_t >     { _MIST_CONST( bool, value, true  ); };
#endif

/// @brief 符号付き型かどうか判定
//! 
//! T が符号付き型であれば真に評価する。
//! 
//! @param T  … 調査する型
//! 
template< class T > struct is_signed                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_signed< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_signed< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_signed< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_signed< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_signed< char >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_signed< float >         { _MIST_CONST( bool, value, true  ); };
template<>          struct is_signed< double >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_signed< long double >   { _MIST_CONST( bool, value, true  ); };

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
template<>          struct is_signed< ptrdiff_t >     { _MIST_CONST( bool, value, true  ); };
#endif



/// @brief arithmetic 判定
//! 
//! T が算術型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
//! 
//! @param T  … 調査する型
//! 
template< class T > struct is_arithmetic                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_arithmetic< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< char >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< float >         { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< double >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< long double >   { _MIST_CONST( bool, value, true  ); };

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
template<>          struct is_arithmetic< size_t >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< ptrdiff_t >     { _MIST_CONST( bool, value, true  ); };
#endif


/// @brief 組み込み型の判定
//! 
//! T が組み込み型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
//! 
//! @param T  … 調査する型
//! 
template< class T > struct is_builtin                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_builtin< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< char >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< float >         { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< double >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< long double >   { _MIST_CONST( bool, value, true  ); };

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
template<>          struct is_builtin< size_t >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_builtin< ptrdiff_t >     { _MIST_CONST( bool, value, true  ); };
#endif


/// @brief 対応する float 型を返す
//! 
//! T が float の場合は float を返し，それ以外は double （もしくは long double）とする
//! 
//! @param T  … 調査する型
//! 
template< class T > struct float_type       { typedef double value_type; };
template<> struct float_type< float >       { typedef float value_type; };
template<> struct float_type< long double > { typedef long double value_type; };




/// @brief 型の昇格型を返す
//! 
//! T1 と T2 の算術型で演算を行う際に，型の昇格を行った結果の型を返す．
//! たとえば，double と int の演算では，内部計算を double で行うような際に用いる．
//! 
//! @param T1  … 調査する型1
//! @param T2  … 調査する型2
//! 
template< class T1, class T2 > struct promote_trait { typedef T1 value_type; };

#define PROMOTE_TRAIT( T1, T2, T3 )	\
		template< > struct promote_trait< T1, T2 > { typedef T3 value_type; };	\
		template< > struct promote_trait< T2, T1 > { typedef T3 value_type; };

PROMOTE_TRAIT( unsigned  char, unsigned short, unsigned short )
PROMOTE_TRAIT( unsigned  char, unsigned   int, unsigned   int )
PROMOTE_TRAIT( unsigned  char, unsigned  long, unsigned  long )
PROMOTE_TRAIT( unsigned  char,   signed  char,   signed short )
PROMOTE_TRAIT( unsigned  char,   signed short,   signed short )
PROMOTE_TRAIT( unsigned  char,   signed   int,   signed   int )
PROMOTE_TRAIT( unsigned  char,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned  char,           bool, unsigned  char )
PROMOTE_TRAIT( unsigned  char,           char,   signed  char )
PROMOTE_TRAIT( unsigned  char,          float,          float )
PROMOTE_TRAIT( unsigned  char,         double,         double )
PROMOTE_TRAIT( unsigned  char,    long double,    long double )
PROMOTE_TRAIT(   signed  char, unsigned short,   signed   int )
PROMOTE_TRAIT(   signed  char, unsigned   int,   signed  long )
PROMOTE_TRAIT(   signed  char, unsigned  long,   signed  long )
PROMOTE_TRAIT(   signed  char,   signed short,   signed short )
PROMOTE_TRAIT(   signed  char,   signed   int,   signed   int )
PROMOTE_TRAIT(   signed  char,   signed  long,   signed  long )
PROMOTE_TRAIT(   signed  char,           bool,   signed  char )
PROMOTE_TRAIT(   signed  char,           char,   signed  char )
PROMOTE_TRAIT(   signed  char,          float,          float )
PROMOTE_TRAIT(   signed  char,         double,         double )
PROMOTE_TRAIT(   signed  char,    long double,    long double )
PROMOTE_TRAIT(           char, unsigned short,   signed   int )
PROMOTE_TRAIT(           char, unsigned   int,   signed  long )
PROMOTE_TRAIT(           char, unsigned  long,   signed  long )
PROMOTE_TRAIT(           char,   signed short,   signed short )
PROMOTE_TRAIT(           char,   signed   int,   signed   int )
PROMOTE_TRAIT(           char,   signed  long,   signed  long )
PROMOTE_TRAIT(           char,           bool,           char )
PROMOTE_TRAIT(           char,          float,          float )
PROMOTE_TRAIT(           char,         double,         double )
PROMOTE_TRAIT(           char,    long double,    long double )
PROMOTE_TRAIT( unsigned short, unsigned   int, unsigned   int )
PROMOTE_TRAIT( unsigned short, unsigned  long, unsigned  long )
PROMOTE_TRAIT( unsigned short,   signed short,   signed   int )
PROMOTE_TRAIT( unsigned short,   signed   int,   signed   int )
PROMOTE_TRAIT( unsigned short,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned short,           bool, unsigned short )
PROMOTE_TRAIT( unsigned short,          float,          float )
PROMOTE_TRAIT( unsigned short,         double,         double )
PROMOTE_TRAIT( unsigned short,    long double,    long double )
PROMOTE_TRAIT(   signed short, unsigned   int,   signed   int )
PROMOTE_TRAIT(   signed short, unsigned  long,   signed  long )
PROMOTE_TRAIT(   signed short,   signed   int,   signed   int )
PROMOTE_TRAIT(   signed short,   signed  long,   signed  long )
PROMOTE_TRAIT(   signed short,           bool,   signed short )
PROMOTE_TRAIT(   signed short,          float,          float )
PROMOTE_TRAIT(   signed short,         double,         double )
PROMOTE_TRAIT(   signed short,    long double,    long double )
PROMOTE_TRAIT( unsigned   int, unsigned  long, unsigned  long )
PROMOTE_TRAIT( unsigned   int,   signed   int,   signed  long )
PROMOTE_TRAIT( unsigned   int,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned   int,           bool, unsigned   int )
PROMOTE_TRAIT( unsigned   int,          float,         double )
PROMOTE_TRAIT( unsigned   int,         double,         double )
PROMOTE_TRAIT( unsigned   int,    long double,    long double )
PROMOTE_TRAIT(   signed   int, unsigned  long,   signed  long )
PROMOTE_TRAIT(   signed   int,   signed  long,   signed  long )
PROMOTE_TRAIT(   signed   int,           bool,   signed   int )
PROMOTE_TRAIT(   signed   int,          float,         double )
PROMOTE_TRAIT(   signed   int,         double,         double )
PROMOTE_TRAIT(   signed   int,    long double,    long double )
PROMOTE_TRAIT( unsigned  long,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned  long,           bool, unsigned  long )
PROMOTE_TRAIT( unsigned  long,          float,    long double )
PROMOTE_TRAIT( unsigned  long,         double,         double )
PROMOTE_TRAIT( unsigned  long,    long double,    long double )
PROMOTE_TRAIT(   signed  long,           bool,   signed   int )
PROMOTE_TRAIT(   signed  long,          float,         double )
PROMOTE_TRAIT(   signed  long,         double,         double )
PROMOTE_TRAIT(   signed  long,    long double,    long double )
PROMOTE_TRAIT(           bool,          float,          float )
PROMOTE_TRAIT(           bool,         double,         double )
PROMOTE_TRAIT(           bool,    long double,    long double )
PROMOTE_TRAIT(          float,         double,         double )
PROMOTE_TRAIT(          float,    long double,    long double )
PROMOTE_TRAIT(         double,    long double,    long double )

#undef PROMOTE_TRAIT

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7

	// MSVC6 では，演算子に対応する関数の決定が適切に行われないので，デフォルトで 'double' との演算として定義する．

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! オブジェクト '演算子' オブジェクト を実現する際に利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR1( TYPE, OPERATOR )											\
		template < class T1, class T2 >																\
		inline TYPE< promote_trait< T1, T2 >::value_type >											\
					operator OPERATOR( const TYPE< T1 > &t1, const TYPE< T2 > &t2 )					\
		{																							\
			return( TYPE< promote_trait< T1, T2 >::value_type >( t1 ) OPERATOR ## = t2 );			\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! オブジェクト '演算子' 定数 を実現する際に利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR2( TYPE, OPERATOR )												\
		template < class T >																			\
		inline TYPE< promote_trait< T, double >::value_type >											\
					operator OPERATOR( const TYPE< T > &t1, const double &t2 )							\
		{																								\
			return( TYPE< promote_trait< T, double >::value_type >( t1 ) OPERATOR ## = t2 );			\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! 定数 '演算子' オブジェクト を実現する際に利用する．主に，引き算以外で利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR3( TYPE, OPERATOR )												\
		template < class T >																			\
		inline TYPE< promote_trait< T, double >::value_type >											\
					operator OPERATOR( const double &t1, const TYPE< T > &t2 )							\
		{																								\
			return( TYPE< promote_trait< T, double >::value_type >( t2 ) OPERATOR ## = t1 );			\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! 定数 '－' オブジェクト を実現する際に利用する．ただし，1引数のコンストラクタが利用可能でなくてはならない．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR4( TYPE, OPERATOR )												\
		template < class T >																			\
		inline TYPE< promote_trait< T, double >::value_type >											\
					operator OPERATOR( const double &t1, const TYPE< T > &t2 )							\
		{																								\
			return( TYPE< promote_trait< T, double >::value_type >( t1 ) OPERATOR ## = t2 );			\
	}

#else

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! オブジェクト '演算子' オブジェクト を実現する際に利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR1( TYPE, OPERATOR )											\
		template < class T1, class T2 >																\
		inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
					operator OPERATOR( const TYPE< T1 > &t1, const TYPE< T2 > &t2 )					\
		{																							\
			return( TYPE< typename promote_trait< T1, T2 >::value_type >( t1 ) OPERATOR ## = t2 );	\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! オブジェクト '演算子' 定数 を実現する際に利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR2( TYPE, OPERATOR )											\
		template < class T1, class T2 >																\
		inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
					operator OPERATOR( const TYPE< T1 > &t1, const T2 &t2 )							\
		{																							\
			return( TYPE< typename promote_trait< T1, T2 >::value_type >( t1 ) OPERATOR ## = t2 );	\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! 定数 '演算子' オブジェクト を実現する際に利用する．主に，引き算以外で利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR3( TYPE, OPERATOR )											\
		template < class T1, class T2 >																\
		inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
					operator OPERATOR( const T1 &t1, const TYPE< T2 > &t2 )							\
		{																							\
			return( TYPE< typename promote_trait< T1, T2 >::value_type >( t2 ) OPERATOR ## = t1 );	\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! 定数 '－' オブジェクト を実現する際に利用する．ただし，1引数のコンストラクタが利用可能でなくてはならない．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR4( TYPE, OPERATOR )											\
		template < class T1, class T2 >																\
		inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
					operator OPERATOR( const T1 &t1, const TYPE< T2 > &t2 )							\
		{																							\
			return( TYPE< typename promote_trait< T1, T2 >::value_type >( t1 ) OPERATOR ## = t2 );	\
	}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! オブジェクト '演算子' オブジェクト を実現する際に利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR1_( TYPE, OPERATOR )												\
		template < class T1, class T2, size_t NDIM >														\
		inline TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >									\
					operator OPERATOR( const TYPE< T1, NDIM > &t1, const TYPE< T2, NDIM > &t2 )				\
		{																									\
			return( TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >( t1 ) OPERATOR ## = t2 );	\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! オブジェクト '演算子' 定数 を実現する際に利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR2_( TYPE, OPERATOR )												\
		template < class T1, class T2, size_t NDIM >														\
		inline TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >									\
					operator OPERATOR( const TYPE< T1, NDIM > &t1, const T2 &t2 )							\
		{																									\
			return( TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >( t1 ) OPERATOR ## = t2 );	\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! 定数 '演算子' オブジェクト を実現する際に利用する．主に，引き算以外で利用する．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR3_( TYPE, OPERATOR )												\
		template < class T1, class T2, size_t NDIM >														\
		inline TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >									\
					operator OPERATOR( const T1 &t1, const TYPE< T2, NDIM > &t2 )							\
		{																									\
			return( TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >( t2 ) OPERATOR ## = t1 );	\
		}

	/// @brief 型の昇格を用いた四則演算子を定義する際に利用する
	//!
	//! 定数 '－' オブジェクト を実現する際に利用する．ただし，1引数のコンストラクタが利用可能でなくてはならない．
	//!
	#define DEFINE_PROMOTE_BIND_OPERATOR4_( TYPE, OPERATOR )												\
		template < class T1, class T2, size_t NDIM >														\
		inline TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >									\
					operator OPERATOR( const T1 &t1, const TYPE< T2, NDIM > &t2 )							\
		{																									\
			return( TYPE< typename promote_trait< T1, T2 >::value_type, NDIM >( t1 ) OPERATOR ## = t2 );	\
	}

#endif



/// @brief データ型の確定を避けるために利用する
//! 
//! @param T  … 取得したい型
//! 
template< class T >
struct type_trait{ typedef T value_type; };


// 値の四捨五入をするクラス
template < bool b >
struct __half_adjust__
{
	static double convert( const double value )
	{
		return( value + 0.5 );
	}
};


// 値の四捨五入をするクラス
template < >
struct __half_adjust__< false >
{
	static double convert( const double value )
	{
		return( value );
	}
};


/// @brief 値の四捨五入をする
//! 
//! 整数型の場合のみ四捨五入を実行する
//! 
template < class T >
struct half_adjust
{
	typedef T value_type;

	static value_type convert( const double value )
	{
		return( static_cast< value_type >( __half_adjust__< is_integer< value_type >::value >::convert( value ) ) );
	}
};


/// @brief 型のAND演算を行う
//! 
//! 全てが真の場合のみ真となる
//! 
//! @param b1 … パラメータ1
//! @param b2 … パラメータ2
//! @param b3 … パラメータ3
//! @param b4 … パラメータ4
//! @param b5 … パラメータ5
//! @param b6 … パラメータ6
//! @param b7 … パラメータ7
//! 
template < bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true, bool b6 = true, bool b7 = true >
struct type_and
{
    _MIST_CONST( bool, value, false );
};

template <>
struct type_and< true, true, true, true, true, true, true >
{
    _MIST_CONST( bool, value, true  );
};



/// @brief 型のOR演算を行う型
//! 
//! どれかひとつでも真の場合に真となる
//! 
//! @param b1 … パラメータ1
//! @param b2 … パラメータ2
//! @param b3 … パラメータ3
//! @param b4 … パラメータ4
//! @param b5 … パラメータ5
//! @param b6 … パラメータ6
//! @param b7 … パラメータ7
//! 
template < bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false, bool b6 = false, bool b7 = false >
struct type_or
{
    _MIST_CONST( bool, value, true );
};

template <>
struct type_or< false, false, false, false, false, false, false >
{
    _MIST_CONST( bool, value, false  );
};




/// @brief 型のNOT演算を行う型
//! 
//! 真の場合は偽に，偽の場合は真になる
//! 
//! @param b1 … パラメータ
//! 
template < bool b1 >
struct type_not
{
    _MIST_CONST( bool, value, true  );
};

template <>
struct type_not< true >
{
    _MIST_CONST( bool, value, false );
};




/// @brief 型のEQUAL演算を行う型
//! 
//! 指定された2つの型が等しい場合のみ真となる
//! 
//! @param b1 … パラメータ1
//! @param b2 … パラメータ2
//! 
template < bool b1, bool b2 >
struct type_equal
{
    _MIST_CONST( bool, value, false );
};

template <>
struct type_equal< true, true >
{
    _MIST_CONST( bool, value, true  );
};

template <>
struct type_equal< false, false >
{
    _MIST_CONST( bool, value, true  );
};




// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TYPE_TRAIT_H__
