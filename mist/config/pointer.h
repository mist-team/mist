/// @file mist/config/pointer.h
//!
//! @brief メモリの自動開放などを行うためのクラス
//!

#ifndef __INCLUDE_MIST_POINTER_H__
#define __INCLUDE_MIST_POINTER_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型


/// @brief オブジェクトのスコープが切れると自動的にメモリ開放を行うポインタ
//! 
//! @attention 配列に対しては利用できない
//! 
template < class T >
class scoped_ptr
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T value_type;				///< @brief 内部データ型．T と同じ
	typedef T* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef T& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef const T* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	pointer ptr_;		///< @brief 管理するポインタ

public:
	/// @brief 成分を b で初期化する
	scoped_ptr( pointer p ) : ptr_( p ){ }

	~scoped_ptr( ){ delete ptr_; }


public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }


private:
	scoped_ptr( const scoped_ptr &p );
	const scoped_ptr &operator  =( const scoped_ptr &p );
};

/// @brief オブジェクトのスコープが切れると配列用に確保したメモリの自動開放を行うポインタ
//! 
//! @attention 配列のポインタに対してしか利用できない
//! 
template < class T >
class scoped_array
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T value_type;				///< @brief 内部データ型．T と同じ
	typedef T* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef T& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef const T* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	pointer ptr_;		///< @brief 管理するポインタ

public:
	/// @brief 成分を b で初期化する
	scoped_array( pointer p ) : ptr_( p ){ }

	~scoped_array( ){ delete [] ptr_; }


public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }

	reference operator []( difference_type index ){ return( ptr_[ index ] ); }
	const_reference operator []( difference_type index ) const { return( ptr_[ index ] ); }

private:
	scoped_array( const scoped_array &p );
	const scoped_array &operator  =( const scoped_array &p );
};



///// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
////! 
////! @param[in,out] out … 入力と出力を行うストリーム
////! @param[in]     b   … バイナリ画素
////! 
////! @return 入力されたストリーム
////! 
////! @code 出力例
////! 1
////! @endcode
////! 
//inline std::ostream &operator <<( std::ostream &out, const binary &b )
//{
//	out << b.get_value( );
//	return( out );
//}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_POINTER_H__
