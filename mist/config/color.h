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

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型


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
	template < class TT >
	const rgb &operator +=( const TT &pix ){ r = static_cast< value_type >( r + pix ); g = static_cast< value_type >( g + pix ); b = static_cast< value_type >( b + pix ); return( *this ); }

	/// @brief RGB成分から pix 値を引く
	template < class TT >
	const rgb &operator -=( const TT &pix ){ r = static_cast< value_type >( r - pix ); g = static_cast< value_type >( g - pix ); b = static_cast< value_type >( b - pix ); return( *this ); }

	/// @brief RGB成分に pix 値を掛ける
	template < class TT >
	const rgb &operator *=( const TT &pix ){ r = static_cast< value_type >( r * pix ); g = static_cast< value_type >( g * pix ); b = static_cast< value_type >( b * pix ); return( *this ); }

	/// @brief RGB成分を pix 値で割る
	template < class TT >
	const rgb &operator /=( const TT &pix ){ r = static_cast< value_type >( r / pix ); g = static_cast< value_type >( g / pix ); b = static_cast< value_type >( b / pix ); return( *this ); }


	/// @brief 2つのカラー画素が等しい（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b
	//! \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が等しい場合
	//! @retval false … 2つのカラー画素が異なる場合
	//! 
	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && b == c.b ); }

	/// @brief 2つのカラー画素が等しくない（全要素が同じ値を持つ）かどうかを判定する
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b }
	//! \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … 2つのカラー画素が異なる場合
	//! @retval false … 2つのカラー画素が等しい場合
	//! 
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }

	/// @brief 2つのカラー画素の < を判定する
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b }
	//! \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <  c2 の場合
	//! @retval false … c1 >= c2 の場合
	//! 
	bool operator < ( const rgb &c ) const { return( !( *this >= c ) ); }

	/// @brief 2つのカラー画素の <= を判定する
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b
	//! \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 <= c2 の場合
	//! @retval false … c1 >  c2 の場合
	//! 
	bool operator <=( const rgb &c ) const { return( c >= *this ); }

	/// @brief 2つのカラー画素の > を判定する
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b }
	//! \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >  c2 の場合
	//! @retval false … c1 <= c2 の場合
	//! 
	bool operator > ( const rgb &c ) const { return( c < *this ); }

	/// @brief 2つのカラー画素の >= を判定する
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b
	//! \f]
	//! 
	//! @param[in] c … 右辺値
	//! 
	//! @retval true  … c1 >= c2 の場合
	//! @retval false … c1 <  c2 の場合
	//! 
	bool operator >=( const rgb &c ) const { return( r >= c.r && g >= c.g && b >= c.b ); }


	/// @brief NTSC系加重平均法により，グレースケールへ変換する
	value_type get_value( ) const
	{
		return( static_cast< value_type >( r * 0.298912 + g * 0.586610 + b * 0.114478 ) );
	}

	// カラーからグレースケールへの自動キャスト演算子（危険のため一時停止）
	//operator value_type( ) const { return( get_value( ) ); }
};


DEFINE_PROMOTE_BIND_OPERATOR1( rgb, + )			///< @brief カラー画素の和
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, + )			///< @brief カラー画素と定数の和
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, + )			///< @brief 定数とカラー画素の和

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, - )			///< @brief カラー画素の差
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, - )			///< @brief カラー画素と定数の差
DEFINE_PROMOTE_BIND_OPERATOR4( rgb, - )			///< @brief 定数とカラー画素の差

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, * )			///< @brief カラー画素の積
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, * )			///< @brief カラー画素と定数の積
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, * )			///< @brief 定数とカラー画素の積

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, / )			///< @brief カラー画素の割り算
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, / )			///< @brief カラー画素を定数で割る

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, % )			///< @brief カラー画素の剰余

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, | )			///< @brief カラー画素の | 演算

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, & )			///< @brief カラー画素の & 演算

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, ^ )			///< @brief カラー画素の ^ 演算


DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, == )	///< @brief 比較演算子 ==
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, != )	///< @brief 比較演算子 !=
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, <  )	///< @brief 比較演算子 <
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, <= )	///< @brief 比較演算子 <=
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, >  )	///< @brief 比較演算子 >
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, >= )	///< @brief 比較演算子 >=



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


/// @brief 入力された型がカラー画像かどうか調べる
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
