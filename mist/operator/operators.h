#ifndef __INCLUDE_MIST_OPERATORS__
#define __INCLUDE_MIST_OPERATORS__

#include "../config/mist_conf.h"


// mist名前空間の始まり
_MIST_BEGIN


// 足し算
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

// 足し算
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


// 掛け算
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

// 割り算
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

// 等しくない
template < class T >
struct implement_operator_ne
{
	friend bool operator !=( const T &obj1, const T &obj2 )
	{
		return( !( obj1 == obj2 ) );
	}
};


// より大きい
template < class T >
struct implement_operator_gt
{
	friend bool operator >( const T &obj1, const T &obj2 )
	{
		return( obj2 < obj1 );
	}
};


// 以下
template < class T >
struct implement_operator_le
{
	friend bool operator <=( const T &obj1, const T &obj2 )
	{
		return( !( obj2 < obj1 ) );
	}
};

// 以下
template < class T >
struct implement_operator_ge
{
	friend bool operator >=( const T &obj1, const T &obj2 )
	{
		return( !( obj1 < obj2 ) );
	}
};


// %演算子
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

// |演算子
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

// &演算子
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

// ^演算子
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


// 四則演算オペレータを実装する
template < class T >
struct implement_arithmetic_operators : public
	implement_operator_add< T >,
	implement_operator_sub< T >,
	implement_operator_mul< T >,
	implement_operator_div< T >
{
};


// 比較オペレータを実装する
template < class T >
struct implement_comparison_operators : public
	implement_operator_ne < T >,
	implement_operator_gt < T >,
	implement_operator_le < T >,
	implement_operator_ge < T >
{
};



// ビット演算オペレータを実装する
template < class T >
struct implement_bit_operators : public
	implement_operator_bitor< T >,
	implement_operator_bitand< T >,
	implement_operator_exclusiveor< T >
{
};


// 全てのオペレータを実装する
template < class T >
struct implement_operators : public
	implement_arithmetic_operators< T >,
	implement_comparison_operators< T >,
	implement_operator_modulo< T >,
	implement_bit_operators< T >
{
};


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_OPERATORS__
