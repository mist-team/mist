#ifndef __INCLUDE_MIST_BINARY_H__
#define __INCLUDE_MIST_BINARY_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型

// binary演算を行うためのもの
class binary
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
	binary( ) : value_( false ){ }
	binary( const value_type &b ) : value_( b ){ }

	const binary &operator  =( const binary &b ){ value_ = b.value_;  return( *this ); }
	const binary &operator  =( const value_type &b ){ value_ = b;   return( *this ); }

	const binary &operator +=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const binary &operator -=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }
	const binary &operator *=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const binary &operator /=( const binary &b ){ value_ = value_ ==  b.value_; return( *this ); }
	const binary &operator %=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }
	const binary &operator |=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const binary &operator &=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const binary &operator ^=( const binary &b ){ value_ = value_ !=  b.value_; return( *this ); }

	bool operator ==( const binary &b ) const { return( value_ == b.value_ ); }
	bool operator !=( const binary &b ) const { return( value_ != b.value_ ); }
	bool operator < ( const binary &b ) const { return( value_ <  b.value_ ); }
	bool operator <=( const binary &b ) const { return( value_ <= b.value_ ); }
	bool operator > ( const binary &b ) const { return( value_ >  b.value_ ); }
	bool operator >=( const binary &b ) const { return( value_ >= b.value_ ); }

	value_type get_value( ) const { return( value_ ); }

	// boolへの自動キャスト演算子（危険のため一時停止）
	operator bool( ) const { return( value_ ); }
};

inline const binary operator +( const binary &b1, const binary &b2 ){ return( binary( b1 ) += b2 ); }
inline const binary operator -( const binary &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }
inline const binary operator *( const binary &b1, const binary &b2 ){ return( binary( b1 ) *= b2 ); }
inline const binary operator /( const binary &b1, const binary &b2 ){ return( binary( b1 ) /= b2 ); }
inline const binary operator %( const binary &b1, const binary &b2 ){ return( binary( b1 ) %= b2 ); }
inline const binary operator |( const binary &b1, const binary &b2 ){ return( binary( b1 ) |= b2 ); }
inline const binary operator &( const binary &b1, const binary &b2 ){ return( binary( b1 ) &= b2 ); }
inline const binary operator ^( const binary &b1, const binary &b2 ){ return( binary( b1 ) ^= b2 ); }

inline std::ostream &operator <<( std::ostream &out, const binary &v )
{
	out << v.get_value( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_BINARY_H__
