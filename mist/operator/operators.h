/// @file mist/operator/operators.h
//!
//! @brief 演算子の実装を補助するためのライブラリ
//!
#ifndef __INCLUDE_MIST_OPERATORS__
#define __INCLUDE_MIST_OPERATORS__

#include "../config/mist_conf.h"


// mist名前空間の始まり
_MIST_BEGIN

//! @addtogroup operator_group 演算子の実装補助
//!  @{



/// @brief 足し算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_add
{
	friend T operator +( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj += obj2;
		return( obj );
	}
};


/// @brief 引き算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_sub
{
	friend T operator -( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj -= obj2;
		return( obj );
	}
};



/// @brief 掛け算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_mul
{
	friend T operator *( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj *= obj2;
		return( obj );
	}
};



/// @brief 割り算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_div
{
	friend T operator /( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj /= obj2;
		return( obj );
	}
};



/// @brief 等しくない
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_ne
{
	friend bool operator !=( const T &obj1, const T &obj2 )
	{
		return( !( obj1 == obj2 ) );
	}
};



/// @brief より大きい
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_gt
{
	friend bool operator >( const T &obj1, const T &obj2 )
	{
		return( obj2 < obj1 );
	}
};



/// @brief 以下
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_le
{
	friend bool operator <=( const T &obj1, const T &obj2 )
	{
		return( !( obj2 < obj1 ) );
	}
};



/// @brief 以上
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_ge
{
	friend bool operator >=( const T &obj1, const T &obj2 )
	{
		return( !( obj1 < obj2 ) );
	}
};



/// @brief %演算子
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_modulo
{
	friend T operator %( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj %= obj2;
		return( obj );
	}
};



/// @brief |演算子
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_bitor
{
	friend T operator |( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj |= obj2;
		return( obj );
	}
};



/// @brief &演算子
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_bitand
{
	friend T operator &( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj &= obj2;
		return( obj );
	}
};



/// @brief ^演算子
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operator_exclusiveor
{
	friend T operator ^( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj ^= obj2;
		return( obj );
	}
};



/// @brief 四則演算オペレータを実装する
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_arithmetic_operators : public
	implement_operator_add< T >,
	implement_operator_sub< T >,
	implement_operator_mul< T >,
	implement_operator_div< T >
{
};



/// @brief 比較オペレータを実装する
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_comparison_operators : public
	implement_operator_ne < T >,
	implement_operator_gt < T >,
	implement_operator_le < T >,
	implement_operator_ge < T >
{
};




/// @brief ビット演算オペレータを実装する
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_bit_operators : public
	implement_operator_bitor< T >,
	implement_operator_bitand< T >,
	implement_operator_exclusiveor< T >
{
};



/// @brief 全てのオペレータを実装する
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct implement_operators : public
	implement_arithmetic_operators< T >,
	implement_comparison_operators< T >,
	implement_operator_modulo< T >,
	implement_bit_operators< T >
{
};


/// @}
//  演算子の実装補助グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_OPERATORS__
