/// @file mist/config/color.h
//!
//! @brief カラーデータを扱うためのクラス群
//!

#ifndef __INCLUDE_MIST_COLOR_H__
#define __INCLUDE_MIST_COLOR_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型


/// @brief カラー画像用
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T … 引数の説明
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
	value_type r;		///< @brief 変数の説明を書く
	value_type g;		///< @brief 変数の説明を書く
	value_type b;		///< @brief 変数の説明を書く

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	rgb( ) : r( 0 ), g( 0 ), b( 0 ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pix … 引数の説明
	//! 
	explicit rgb( const value_type &pix ) : r( pix ), g( pix ), b( pix ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	rgb( const rgb &c ) : r( c.r ), g( c.g ), b( c.b ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] rr … 引数の説明
	//! @param[in] gg … 引数の説明
	//! @param[in] bb … 引数の説明
	//! 
	rgb( const value_type &rr, const value_type &gg, const value_type &bb ) : r( rr ), g( gg ), b( bb ){ }



	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator  =( const rgb &c )
	{
		if( &c != this )
		{
			r = c.r;
			g = c.g;
			b = c.b;
		}
		return( *this );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pix … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator  =( const value_type &pix )
	{
		r = pix;
		g = pix;
		b = pix;
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb  operator -( ) const { return( rgb( -r, -g, -b ) ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator +=( const rgb &c ){ r += c.r; g += c.g; b += c.b; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator -=( const rgb &c ){ r -= c.r; g -= c.g; b -= c.b; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator *=( const rgb &c ){ r *= c.r; g *= c.g; b *= c.b; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator /=( const rgb &c ){ r /= c.r; g /= c.g; b /= c.b; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator %=( const rgb &c ){ r %= c.r; g %= c.g; b %= c.b; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator |=( const rgb &c ){ r |= c.r; g |= c.g; b |= c.b; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator &=( const rgb &c ){ r &= c.r; g &= c.g; b &= c.b; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator ^=( const rgb &c ){ r ^= c.r; g ^= c.g; b ^= c.b; return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pix … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator +=( const value_type &pix ){ r += pix; g += pix; b += pix; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pix … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator -=( const value_type &pix ){ r -= pix; g -= pix; b -= pix; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pix … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator *=( const value_type &pix ){ r *= pix; g *= pix; b *= pix; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pix … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const rgb &operator /=( const value_type &pix ){ r /= pix; g /= pix; b /= pix; return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && b == c.b ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator < ( const rgb &c ) const { return( !( *this >= c ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator <=( const rgb &c ) const { return( c >= *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator > ( const rgb &c ) const { return( c < *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] c … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator >=( const rgb &c ) const { return( r >= c.r && g >= c.g && b >= c.b ); }


	/// @brief NTSC系加重平均法により，グレースケールへ変換する
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	value_type get_value( ) const
	{
		return( static_cast< value_type >( r * 0.298912 + g * 0.586610 + b * 0.114478 ) );
	}

	// カラーからグレースケールへの自動キャスト演算子（危険のため一時停止）
	//operator value_type( ) const { return( get_value( ) ); }
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator +( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) += c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator -( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) -= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator *( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) *= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator /( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) /= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator %( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) %= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator |( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) |= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator &( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) &= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator ^( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) ^= c2 ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator *( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) *= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator *( const typename rgb< T >::value_type &c1, const rgb< T > &c2 ){ return( rgb< T >( c2 ) *= c1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator /( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) /= c2 ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator +( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) += c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator +( const typename rgb< T >::value_type &c1, const rgb< T > &c2 ){ return( rgb< T >( c2 ) += c1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator -( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) -= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const rgb< T > operator -( const typename rgb< T >::value_type &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) -= c2 ); }


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const rgb< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << " )";
	return( out );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T >
struct is_color
{
	_MIST_CONST( bool, value, false );
};


// 画素の変換をサポートするためのコンバータ
template < class T >
struct _pixel_converter_
{
	typedef T value_type;
	typedef rgb< T > color_type;
	enum{ color_num = 1 };

	static value_type convert_to( value_type r, value_type g, value_type b )
	{
		return( color_type( r, g, b ).get_value( ) );
	}

	static color_type convert_from( const value_type &pixel )
	{
		return( color_type( pixel, pixel, pixel ) );
	}
};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_COLOR( type ) \
		template < >\
		struct is_color< rgb< type > >\
		{\
			enum{ value = true };\
		};\


	#define __PIXEL_CONVERTER__( type ) \
		template < >\
		struct _pixel_converter_< rgb< type > >\
		{\
			typedef type value_type;\
			typedef rgb< type > color_type;\
			enum{ color_num = 3 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b )\
			{\
				return( color_type( r, g, b ) );\
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
	struct _pixel_converter_< rgb< T > >
	{
		typedef T value_type;
		typedef rgb< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b )
		{
			return( color_type( r, g, b ) );
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
