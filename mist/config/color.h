#ifndef __INCLUDE_MIST_COLOR_H__
#define __INCLUDE_MIST_COLOR_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型

// boolean演算を行うためのもの
class boolean
{
public:
	typedef bool& reference;
	typedef const bool& const_reference;
	typedef bool value_type;
	typedef bool* pointer;
	typedef const bool* const_pointer;

private:
	bool value_;

public:
	boolean( ) : value_( false ){ }
	boolean( const boolean &b ) : value_( b.value_ ){ }
	boolean( const value_type &b ) : value_( b ){ }

	const boolean &operator  =( const boolean &b ){ value_ = b.value_;  return( *this ); }
	const boolean &operator  =( const value_type &b ){ value_ = b;   return( *this ); }

	const boolean &operator +=( const boolean &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const boolean &operator -=( const boolean &b ){ value_ = value_ && !b.value_; return( *this ); }
	const boolean &operator *=( const boolean &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const boolean &operator /=( const boolean &b ){ value_ = value_ ==  b.value_; return( *this ); }
	const boolean &operator %=( const boolean &b ){ value_ = value_ && !b.value_; return( *this ); }
	const boolean &operator |=( const boolean &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const boolean &operator &=( const boolean &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const boolean &operator ^=( const boolean &b ){ value_ = value_ !=  b.value_; return( *this ); }

	bool operator ==( const boolean &b ) const { return( value_ == b.value_ ); }
	bool operator !=( const boolean &b ) const { return( value_ != b.value_ ); }
	bool operator < ( const boolean &b ) const { return( value_ <  b.value_ ); }
	bool operator <=( const boolean &b ) const { return( value_ <= b.value_ ); }
	bool operator > ( const boolean &b ) const { return( value_ >  b.value_ ); }
	bool operator >=( const boolean &b ) const { return( value_ >= b.value_ ); }

	value_type get_value( ) const { return( value_ ); }

	operator bool( ) const { return( value_ ); }
};

inline const boolean operator +( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) += b2 ); }
inline const boolean operator -( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) -= b2 ); }
inline const boolean operator *( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) *= b2 ); }
inline const boolean operator /( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) /= b2 ); }
inline const boolean operator %( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) %= b2 ); }
inline const boolean operator |( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) |= b2 ); }
inline const boolean operator &( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) &= b2 ); }
inline const boolean operator ^( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) ^= b2 ); }

inline std::ostream &operator <<( std::ostream &out, const boolean &v )
{
	out << v.get_value( );
	return( out );
}


// カラー画像用
template< class T >
struct rgb
{
public:
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;

public:
	value_type r;
	value_type g;
	value_type b;

	rgb( ) : r( 0 ), g( 0 ), b( 0 ){ }
	rgb( const value_type &rr, const value_type &gg, const value_type &bb ) : r( rr ), g( gg ), b( bb ){ }

	const rgb &operator +=( const rgb &c ){ r += c.r; g += c.g; b += c.b; return( *this ); }
	const rgb &operator -=( const rgb &c ){ r -= c.r; g -= c.g; b -= c.b; return( *this ); }
	const rgb &operator *=( const rgb &c ){ r *= c.r; g *= c.g; b *= c.b; return( *this ); }
	const rgb &operator /=( const rgb &c ){ r /= c.r; g /= c.g; b /= c.b; return( *this ); }
	const rgb &operator %=( const rgb &c ){ r %= c.r; g %= c.g; b %= c.b; return( *this ); }
	const rgb &operator |=( const rgb &c ){ r |= c.r; g |= c.g; b |= c.b; return( *this ); }
	const rgb &operator &=( const rgb &c ){ r &= c.r; g &= c.g; b &= c.b; return( *this ); }
	const rgb &operator ^=( const rgb &c ){ r ^= c.r; g ^= c.g; b ^= c.b; return( *this ); }

	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && c == c.b ); }
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }
	bool operator < ( const rgb &c ) const { return( r <  c.r || ( r == c.r && g <  c.g ) || ( r == c.r && g == c.g  &&c <  c.b ) ); }
	bool operator <=( const rgb &c ) const { return( !( c < *this ) ); }
	bool operator > ( const rgb &c ) const { return( c < *this ); }
	bool operator >=( const rgb &c ) const { return( !( *this < c ) ); }

	// NTSC系加重平均法により，グレースケールへ変換する
	value_type get_value( ) const
	{
		return( static_cast< value_type >( r * 0.298912 + g * 0.586610 + b * 0.114478 ) );
	}

	operator value_type( ) const { return( get_value( ) ); }
};

template < class T > inline const rgb< T > operator +( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) += c2 ); }
template < class T > inline const rgb< T > operator -( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) -= c2 ); }
template < class T > inline const rgb< T > operator *( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) *= c2 ); }
template < class T > inline const rgb< T > operator /( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) /= c2 ); }
template < class T > inline const rgb< T > operator %( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) %= c2 ); }
template < class T > inline const rgb< T > operator |( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) |= c2 ); }
template < class T > inline const rgb< T > operator &( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) &= c2 ); }
template < class T > inline const rgb< T > operator ^( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) ^= c2 ); }

template < class T > inline std::ostream &operator <<( std::ostream &out, const rgb< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << " )";
	return( out );
}

template < class T >
struct is_color
{
	_MIST_CONST( bool, value, false );
};

template < class T >
struct is_color< rgb< T > >
{
	_MIST_CONST( bool, value, true );
};


// 画素の変換をサポートするためのコンバータ
template < class T >
struct _pixel_converter_
{
	typedef T value_type;
	typedef rgb< T > color_type;
	enum{ color_num = 1 };

	static value_type convert_to_pixel( value_type r, value_type g, value_type b )
	{
		return( color_type( r, g, b ).get_value( ) );
	}

	static color_type convert_from_pixel( const value_type &pixel )
	{
		return( color_type( pixel, pixel, pixel ) );
	}
};

template < class T >
struct _pixel_converter_< rgb< T > >
{
	typedef T value_type;
	typedef rgb< T > color_type;
	enum{ color_num = 3 };

	static color_type convert_to_pixel( value_type r, value_type g, value_type b )
	{
		return( color_type( r, g, b ) );
	}

	static color_type convert_from_pixel( const value_type &pixel )
	{
		return( color_type( pixel, pixel, pixel ) );
	}
};


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_COLOR_H__
