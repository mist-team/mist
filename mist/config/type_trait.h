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


//
//
///// @brief 型の昇格型を返す
////! 
////! T が算術型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
////! 
////! @param T  … 調査する型
////! 
//template< class T > struct promote_trait                  { _MIST_CONST( bool, value, false ); };
//template<>          struct is_arithmetic< unsigned char > { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< unsigned short >{ _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< unsigned int >  { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< unsigned long > { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed char >   { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed short >  { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed int >    { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed long >   { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< bool >          { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< char >          { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< float >         { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< double >        { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< long double >   { _MIST_CONST( bool, value, true  ); };



/// @brief データ型の確定を避けるために利用する
//! 
//! @param T  … 取得したい型
//! 
template< class T >
struct type_trait{ typedef T value_type; };



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
