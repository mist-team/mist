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
class binary
{
public:
	typedef size_t size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef bool& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const bool& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef bool value_type;				///< @brief 内部データ型．bool と同じ
	typedef bool* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef const bool* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	bool value_;		///< @brief 変数の説明を書く

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	binary( ) : value_( false ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	binary( const value_type &b ) : value_( b ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	binary( const binary &b ) : value_( b.value_ ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator  =( const binary &b ){ value_ = b.value_;  return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator  =( const value_type &b ){ value_ = b;   return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator +=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator -=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator *=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator /=( const binary &b ){ value_ = value_ ==  b.value_; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator %=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator |=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator &=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator ^=( const binary &b ){ value_ = value_ !=  b.value_; return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator +=( const value_type &b ){ return( operator +=( binary( b ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator -=( const value_type &b ){ return( operator -=( binary( b ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator *=( const value_type &b ){ return( operator *=( binary( b ) ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const binary &operator /=( const value_type &b ){ return( operator /=( binary( b ) ) ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false  … 引数の説明
	//! 
	bool operator ==( const binary &b ) const { return( value_ == b.value_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false  … 引数の説明
	//! 
	bool operator !=( const binary &b ) const { return( value_ != b.value_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false  … 引数の説明
	//! 
	bool operator < ( const binary &b ) const { return( value_ <  b.value_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false  … 引数の説明
	//! 
	bool operator <=( const binary &b ) const { return( value_ <= b.value_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false  … 引数の説明
	//! 
	bool operator > ( const binary &b ) const { return( value_ >  b.value_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false  … 引数の説明
	//! 
	bool operator >=( const binary &b ) const { return( value_ >= b.value_ ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	value_type get_value( ) const { return( value_ ); }

	// boolへの自動キャスト演算子（危険のため一時停止）
	//operator bool( ) const { return( value_ ); }
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator +( const binary &b1, const binary &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator -( const binary &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator *( const binary &b1, const binary &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator /( const binary &b1, const binary &b2 ){ return( binary( b1 ) /= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator %( const binary &b1, const binary &b2 ){ return( binary( b1 ) %= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator |( const binary &b1, const binary &b2 ){ return( binary( b1 ) |= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator &( const binary &b1, const binary &b2 ){ return( binary( b1 ) &= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator ^( const binary &b1, const binary &b2 ){ return( binary( b1 ) ^= b2 ); }


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator *( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator *( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) *= b1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator /( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) /= b2 ); }


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator +( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator +( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) += b1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator -( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] b1 … 引数の説明
//! @param[in] b2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline const binary operator -( const binary::value_type &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in,out] out … 引数の説明
//! @param[in]     v   … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline std::ostream &operator <<( std::ostream &out, const binary &v )
{
	out << v.get_value( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_BINARY_H__
