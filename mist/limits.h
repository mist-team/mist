/// @file mist/limits.h
//!
//! @brief 各型に対する情報を取得するためのライブラリ
//!
#ifndef __INCLUDE_MIST_LIMITS__
#define __INCLUDE_MIST_LIMITS__

#include <limits.h>
#include <float.h>


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN


// numeric_limitsの特殊化用マクロ
#define _DEFINE_TYPE_LIMITS( _type_, _integer_, _signed_, _min_, _max_, _zero_ ) \
	template < >\
	struct type_limits< _type_ >\
	{\
		typedef _type_ value_type;\
		\
		enum{ is_integer = _integer_ };\
		enum{ is_signed  = _signed_ };\
		\
		static value_type minimum( ) { return(  _min_ ); }\
		static value_type maximum( ) { return(  _max_ ); }\
		static value_type zero( ){ return( _zero_ ); }\
	};\


//! @addtogroup limits_group 型に関する情報の取得
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/limits.h>
//! @endcode
//!
//! @{


/// @brief 入力された型の情報を取得する
//! 
//! 入力された型の以下の情報を調べる
//! - 整数かどうか
//! - 符号付かどうか
//! - 最小値
//! - 最大値
//! - ゼロ
//! 
//! @param T  … 情報を調べる型
//! 
//! @code 使用例
//! std::cout << type_limits< int >::is_integer << std::endl;	// 整数なら1を出力する
//! std::cout << type_limits< int >::is_signed  << std::endl;	// 符号付なら1を出力する
//! std::cout << type_limits< int >::minimum( ) << std::endl;	// 最小値を出力する
//! std::cout << type_limits< int >::maximum( ) << std::endl;	// 最大値を出力する
//! std::cout << type_limits< int >::zero( )    << std::endl;	// ゼロを出力する
//! @endcode
//! 
template < class T >
struct type_limits
{
	typedef T value_type;	///< @brief 情報を調べる対象となる型

	/// @brief 指定された型が整数かどうか
	//!
	//! 整数の場合は true になる
	//!
	_MIST_CONST( bool, is_integer, false );

	/// @brief 指定された型が符号付かどうか
	//!
	//! 符号付の場合は true になる
	//!
	_MIST_CONST( bool, is_signed, false );


	/// @brief 型が表現できる最も最小の値を返す
	static value_type minimum( ) { return( 0 ); }

	/// @brief 型が表現できる最も最大の値を返す
	static value_type maximum( ) { return( 1 ); }

	/// @brief 型が表現できるゼロに対応する値を返す
	static value_type zero( ){ return( 0 ); }
};



/// @}
//  型に関する情報の取得グループの終わり


_DEFINE_TYPE_LIMITS(          char,  true,  true,  CHAR_MIN,  CHAR_MAX, 0x00 )
_DEFINE_TYPE_LIMITS( signed   char,  true,  true, SCHAR_MIN, SCHAR_MAX, 0x00 )
_DEFINE_TYPE_LIMITS( unsigned char,  true, false,      0x00, UCHAR_MAX, 0x00 )

_DEFINE_TYPE_LIMITS( signed   short, true,  true,  SHRT_MIN,  SHRT_MAX, 0x0000 )
_DEFINE_TYPE_LIMITS( unsigned short, true, false,    0x0000, USHRT_MAX, 0x0000 )

_DEFINE_TYPE_LIMITS( signed   int,   true,  true,   INT_MIN,   INT_MAX, 0 )
_DEFINE_TYPE_LIMITS( unsigned int,   true, false,         0,  UINT_MAX, 0 )

_DEFINE_TYPE_LIMITS( signed   long,  true,  true,  LONG_MIN,  LONG_MAX, 0 )
_DEFINE_TYPE_LIMITS( unsigned long,  true, false,         0, ULONG_MAX, 0 )

_DEFINE_TYPE_LIMITS(       float,   false,  true,   FLT_MIN,   FLT_MAX, 0.0f )
_DEFINE_TYPE_LIMITS(      double,   false,  true,   DBL_MIN,   DBL_MAX, 0 )
_DEFINE_TYPE_LIMITS( long double,   false,  true,  LDBL_MIN,  LDBL_MAX, 0 )


#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define _DEFINE_COLOR_TYPE_LIMITS( type ) \
	template < >\
	struct type_limits< rgb< type > >\
	{\
		typedef rgb< type > value_type;\
		typedef rgb< type >::value_type rgb_value_type;\
		\
		enum{ is_integer = type_limits< rgb_value_type >::is_integer };\
		enum{ is_signed  = type_limits< rgb_value_type >::is_signed };\
		\
		static value_type minimum( ) { return( rgb< type >( type_limits< rgb_value_type >::minimum( ) ) ); }\
		static value_type maximum( ) { return( rgb< type >( type_limits< rgb_value_type >::maximum( ) ) ); }\
		static value_type zero( ) { return( rgb< type >( type_limits< rgb_value_type >::zero( ) ) ); }\
	};\
	template < >\
	struct type_limits< rgba< type > >\
	{\
		typedef rgba< type > value_type;\
		typedef rgba< type >::value_type rgb_value_type;\
		\
		enum{ is_integer = type_limits< rgb_value_type >::is_integer };\
		enum{ is_signed  = type_limits< rgb_value_type >::is_signed };\
		\
		static value_type minimum( ) { return( rgba< type >( type_limits< rgb_value_type >::minimum( ) ) ); }\
		static value_type maximum( ) { return( rgba< type >( type_limits< rgb_value_type >::maximum( ) ) ); }\
		static value_type zero( ) { return( rgba< type >( type_limits< rgb_value_type >::zero( ) ) ); }\
	};\

	// 各型に対する特殊化
	_DEFINE_COLOR_TYPE_LIMITS(unsigned char)
	_DEFINE_COLOR_TYPE_LIMITS(unsigned short)
	_DEFINE_COLOR_TYPE_LIMITS(unsigned int)
	_DEFINE_COLOR_TYPE_LIMITS(unsigned long)
	_DEFINE_COLOR_TYPE_LIMITS(signed char)
	_DEFINE_COLOR_TYPE_LIMITS(signed short)
	_DEFINE_COLOR_TYPE_LIMITS(signed int)
	_DEFINE_COLOR_TYPE_LIMITS(signed long)
	_DEFINE_COLOR_TYPE_LIMITS(bool)
	_DEFINE_COLOR_TYPE_LIMITS(char)
	_DEFINE_COLOR_TYPE_LIMITS(float)
	_DEFINE_COLOR_TYPE_LIMITS(double)
	_DEFINE_COLOR_TYPE_LIMITS(long double)

	#undef _DEFINE_COLOR_TYPE_LIMITS

#else

	template < class T >
	struct type_limits< rgb< T > >
	{
		typedef rgb< T > value_type;
		typedef typename rgb< T >::value_type rgb_value_type;

		enum{ is_integer = type_limits< rgb_value_type >::is_integer };
		enum{ is_signed  = type_limits< rgb_value_type >::is_signed };

		static value_type minimum( ) { return( rgb< T >( type_limits< rgb_value_type >::minimum( ) ) ); }
		static value_type maximum( ) { return( rgb< T >( type_limits< rgb_value_type >::maximum( ) ) ); }
		static value_type zero( ) { return( rgb< T >( type_limits< rgb_value_type >::zero( ) ) ); }
	};

	template < class T >
	struct type_limits< rgba< T > >
	{
		typedef rgba< T > value_type;
		typedef typename rgba< T >::value_type rgb_value_type;

		enum{ is_integer = type_limits< rgb_value_type >::is_integer };
		enum{ is_signed  = type_limits< rgb_value_type >::is_signed };

		static value_type minimum( ) { return( rgba< T >( type_limits< rgb_value_type >::minimum( ) ) ); }
		static value_type maximum( ) { return( rgba< T >( type_limits< rgb_value_type >::maximum( ) ) ); }
		static value_type zero( ) { return( rgba< T >( type_limits< rgb_value_type >::zero( ) ) ); }
	};

#endif


#undef _DEFINE_TYPE_LIMITS


namespace __limits_min_max__
{
	template < class T >
	inline const T limits_min_max__( const T &v, const T &min, const T &max )
	{
		return( v < min ? min : ( v > max ? max : v ) );
	};

	template < bool _ISCOLOR_ >
	struct limits_min_max
	{
		template < class T >
		static const T limits( const T &v, const T &min, const T &max )
		{
			return( limits_min_max__( v, min, max ) );
		}
	};

	template < >
	struct limits_min_max< true >
	{
		template < class T >
		static const rgb< T > limits( const rgb< T > &v, const rgb< T > &min, const rgb< T > &max )
		{
			return( rgb< T >( limits_min_max__( v.r, min.r, max.r ), limits_min_max__( v.g, min.g, max.g ), limits_min_max__( v.b, min.b, max.b ) ) );
		}

		template < class T >
		static const rgba< T > limits( const rgba< T > &v, const rgba< T > &min, const rgba< T > &max )
		{
			return( rgba< T >( limits_min_max__( v.r, min.r, max.r ), limits_min_max__( v.g, min.g, max.g ), limits_min_max__( v.b, min.b, max.b ), limits_min_max__( v.a, min.a, max.a ) ) );
		}
	};
}


namespace __limits_0_255__
{
	template < bool _ISCHAR_ >
	struct limits_0_255__
	{
		template < class T >
		static const T limits( const T &v )
		{
			return( v );
		}
	};

	template < >
	struct limits_0_255__< false >
	{
		template < class T >
		static const T limits( const T &v )
		{
			return( v < 0 ? 0 : ( v > 255 ? 255 : v ) );
		}
	};

	template < bool _ISCOLOR_ >
	struct limits_0_255
	{
		template < class T >
		static const T limits( const T &v )
		{
			return( limits_0_255__< is_char< T >::value >::limits( v ) );
		}
	};

	template < >
	struct limits_0_255< true >
	{
		template < class T >
		static const rgb< T > limits( const rgb< T > &v )
		{
			typedef limits_0_255__< is_char< T >::value > limits_0_255_;
			return( rgb< T >( limits_0_255_::limits( v.r ), limits_0_255_::limits( v.g ), limits_0_255_::limits( v.b ) ) );
		}

		template < class T >
		static const rgba< T > limits( const rgba< T > &v )
		{
			typedef limits_0_255__< is_char< T >::value > limits_0_255_;
			return( rgba< T >( limits_0_255_::limits( v.r ), limits_0_255_::limits( v.g ), limits_0_255_::limits( v.b ), limits_0_255_::limits( v.a ) ) );
		}
	};
}



//! @addtogroup limits_group 型に関する情報の取得
//!
//! @{


/// @brief 入力されたデータを min から max の範囲に変換する
//! 
//! min 未満の値は全て min になり，max 以上の値は全て max とする
//! 
//! @param[in] v   … 変換を行うデータ
//! @param[in] min … データの下限
//! @param[in] max … データの上限
//! 
//! @return 変換後のデータ
//! 
template < class T >
inline const T limits_min_max( const T &v, const T &min, const T &max )
{
	return( __limits_min_max__::limits_min_max< is_color< T >::value >::limits( v ) );
}



/// @brief 入力されたデータを 0 から 255 の範囲に変換する
//! 
//! 0未満の値は全て0になり，255以上の値は全て255とする
//! 
//! @param[in] v … 変換を行うデータ
//! 
//! @return 変換後のデータ
//! 
template < class T >
inline const T limits_0_255( const T &v )
{
	return( __limits_0_255__::limits_0_255< is_color< T >::value >::limits( v ) );
}


/// @}
//  型に関する情報の取得グループの終わり


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_LIMITS__
