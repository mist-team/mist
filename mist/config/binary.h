/// @file mist/config/binary.h
//!
//! @brief バイナリ演算を行うためのクラス
//!

#ifndef __INCLUDE_MIST_BINARY_H__
#define __INCLUDE_MIST_BINARY_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型


/// @brief binary演算を行うためのもの
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
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
	binary( const binary &b ) : value_( b.value_ ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const binary &operator  =( const binary &b ){ value_ = b.value_;  return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const binary &operator  =( const value_type &b ){ value_ = b;   return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	const binary &operator +=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const binary &operator -=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }
	const binary &operator *=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const binary &operator /=( const binary &b ){ value_ = value_ ==  b.value_; return( *this ); }
	const binary &operator %=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }
	const binary &operator |=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const binary &operator &=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const binary &operator ^=( const binary &b ){ value_ = value_ !=  b.value_; return( *this ); }

	const binary &operator +=( const value_type &b ){ return( operator +=( binary( b ) ) ); }
	const binary &operator -=( const value_type &b ){ return( operator -=( binary( b ) ) ); }
	const binary &operator *=( const value_type &b ){ return( operator *=( binary( b ) ) ); }
	const binary &operator /=( const value_type &b ){ return( operator /=( binary( b ) ) ); }

	bool operator ==( const binary &b ) const { return( value_ == b.value_ ); }
	bool operator !=( const binary &b ) const { return( value_ != b.value_ ); }
	bool operator < ( const binary &b ) const { return( value_ <  b.value_ ); }
	bool operator <=( const binary &b ) const { return( value_ <= b.value_ ); }
	bool operator > ( const binary &b ) const { return( value_ >  b.value_ ); }
	bool operator >=( const binary &b ) const { return( value_ >= b.value_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! @return        … 戻り値の説明
	//! 
	value_type get_value( ) const { return( value_ ); }

	// boolへの自動キャスト演算子（危険のため一時停止）
	//operator bool( ) const { return( value_ ); }
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
inline const binary operator +( const binary &b1, const binary &b2 ){ return( binary( b1 ) += b2 ); }
inline const binary operator -( const binary &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }
inline const binary operator *( const binary &b1, const binary &b2 ){ return( binary( b1 ) *= b2 ); }
inline const binary operator /( const binary &b1, const binary &b2 ){ return( binary( b1 ) /= b2 ); }
inline const binary operator %( const binary &b1, const binary &b2 ){ return( binary( b1 ) %= b2 ); }
inline const binary operator |( const binary &b1, const binary &b2 ){ return( binary( b1 ) |= b2 ); }
inline const binary operator &( const binary &b1, const binary &b2 ){ return( binary( b1 ) &= b2 ); }
inline const binary operator ^( const binary &b1, const binary &b2 ){ return( binary( b1 ) ^= b2 ); }

inline const binary operator *( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) *= b2 ); }
inline const binary operator *( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) *= b1 ); }
inline const binary operator /( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) /= b2 ); }

inline const binary operator +( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) += b2 ); }
inline const binary operator +( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) += b1 ); }
inline const binary operator -( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) -= b2 ); }
inline const binary operator -( const binary::value_type &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
inline std::ostream &operator <<( std::ostream &out, const binary &v )
{
	out << v.get_value( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_BINARY_H__
