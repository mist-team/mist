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


/// @brief バイナリ演算を行うためのもの
//! 
//! @code バイナリ画像の作成例
//! mist::array2< mist::binary > image;
//! @endcode
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
	bool value_;		///< @brief バイナリの基本成分

public:
	/// @brief デフォルトコンストラクタ（要素を false で初期化する）
	binary( ) : value_( false ){ }

	/// @brief 成分を b で初期化する
	binary( const value_type &b ) : value_( b ){ }

	/// @brief 他のバイナリ画素を用いて初期化する
	binary( const binary &b ) : value_( b.value_ ){ }


	/// @brief 他のバイナリ画素を代入する
	const binary &operator  =( const binary &b ){ value_ = b.value_; return( *this ); }

	/// @brief 要素に b を代入する
	const binary &operator  =( const value_type &b ){ value_ = b; return( *this ); }


	/// @brief 論理和
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> true
	//! - true  + false -> true
	//! - false + true  -> true
	//! - false + false -> false
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator +=( const binary &b ){ value_ = value_ || b.value_; return( *this ); }

	/// @brief 論理差
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> false
	//! - true  + false -> true
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator -=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief 論理積
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> true
	//! - true  + false -> false
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator *=( const binary &b ){ value_ = value_ && b.value_; return( *this ); }

	/// @brief 排他論理差
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> true
	//! - true  + false -> false
	//! - false + true  -> false
	//! - false + false -> true
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator /=( const binary &b ){ value_ = value_ == b.value_; return( *this ); }

	/// @brief 論理差
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> false
	//! - true  + false -> true
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator %=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief 論理和
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> true
	//! - true  + false -> true
	//! - false + true  -> true
	//! - false + false -> false
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator |=( const binary &b ){ value_ = value_ || b.value_; return( *this ); }

	/// @brief 論理積
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> true
	//! - true  + false -> false
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator &=( const binary &b ){ value_ = value_ && b.value_; return( *this ); }

	/// @brief 排他論理和
	//! 
	//! 以下の演算を行う
	//! - true  + true  -> false
	//! - true  + false -> true
	//! - false + true  -> true
	//! - false + false -> false
	//! 
	//! @param[in] b … 右辺値
	//! 
	//! @return 演算結果
	//! 
	const binary &operator ^=( const binary &b ){ value_ = value_ != b.value_; return( *this ); }


	/// @brief 論理和
	const binary &operator +=( const value_type &b ){ return( operator +=( binary( b ) ) ); }

	/// @brief 論理差
	const binary &operator -=( const value_type &b ){ return( operator -=( binary( b ) ) ); }

	/// @brief 論理積
	const binary &operator *=( const value_type &b ){ return( operator *=( binary( b ) ) ); }

	/// @brief 排他論理差
	const binary &operator /=( const value_type &b ){ return( operator /=( binary( b ) ) ); }


	/// @brief 入力された2つのオブジェクトが等しいかどうかを判定する
	bool operator ==( const binary &b ) const { return( value_ == b.value_ ); }

	/// @brief 入力された2つのオブジェクトが等しくないかどうかを判定する
	bool operator !=( const binary &b ) const { return( value_ != b.value_ ); }

	/// @brief 入力された2つのオブジェクトの < 関係を判定する
	bool operator < ( const binary &b ) const { return( value_ <  b.value_ ); }

	/// @brief 入力された2つのオブジェクトの <= 関係を判定する
	bool operator <=( const binary &b ) const { return( value_ <= b.value_ ); }

	/// @brief 入力された2つのオブジェクトの > 関係を判定する
	bool operator > ( const binary &b ) const { return( value_ >  b.value_ ); }

	/// @brief 入力された2つのオブジェクトの >= 関係を判定する
	bool operator >=( const binary &b ) const { return( value_ >= b.value_ ); }


	/// @brief 内部データを取得する
	value_type get_value( ) const { return( value_ ); }

	// boolへの自動キャスト演算子（危険のため一時停止）
	//operator bool( ) const { return( value_ ); }
};


/// @brief 論理演算
inline const binary operator +( const binary &b1, const binary &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief 論理演算
inline const binary operator -( const binary &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief 論理演算
inline const binary operator *( const binary &b1, const binary &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief 論理演算
inline const binary operator /( const binary &b1, const binary &b2 ){ return( binary( b1 ) /= b2 ); }

/// @brief 論理演算
inline const binary operator %( const binary &b1, const binary &b2 ){ return( binary( b1 ) %= b2 ); }

/// @brief 論理演算
inline const binary operator |( const binary &b1, const binary &b2 ){ return( binary( b1 ) |= b2 ); }

/// @brief 論理演算
inline const binary operator &( const binary &b1, const binary &b2 ){ return( binary( b1 ) &= b2 ); }

/// @brief 論理演算
inline const binary operator ^( const binary &b1, const binary &b2 ){ return( binary( b1 ) ^= b2 ); }


/// @brief 論理演算
inline const binary operator *( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief 論理演算
inline const binary operator *( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) *= b1 ); }

/// @brief 論理演算
inline const binary operator /( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) /= b2 ); }


/// @brief 論理演算
inline const binary operator +( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief 論理演算
inline const binary operator +( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) += b1 ); }

/// @brief 論理演算
inline const binary operator -( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief 論理演算
inline const binary operator -( const binary::value_type &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }



/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     b   … バイナリ画素
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1
//! @endcode
//! 
inline std::ostream &operator <<( std::ostream &out, const binary &b )
{
	out << b.get_value( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_BINARY_H__
