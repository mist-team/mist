/// @file mist/config/iterator.h
//!
//! @brief MISTコンテナを操作するためのイテレータ
//!

#ifndef __INCLUDE_MIST_ITERATOR_H__
#define __INCLUDE_MIST_ITERATOR_H__


#include <iterator>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN



/// @brief mistコンテナで利用する1次元操作用ランダムアクセスイテレータ
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T         … 引数の説明
//! @param Distance  … 引数の説明
//! @param Pointer   … 引数の説明
//! @param Reference … 引数の説明
//! 
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;					///< @brief 内部データ型．bool と同じ
	typedef Pointer pointer;				///< @brief データ型のポインター型．data の場合，data * となる
	typedef Reference reference;			///< @brief データ型の参照．data の場合，data & となる
	typedef size_t size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef Distance difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef Reference const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる

private:
	pointer data_;					///< @brief 変数の説明を書く
	size_type diff_pointer_;		///< @brief 変数の説明を書く

public:
	/// @brief コピー演算子
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] p    … 引数の説明
	//! @param[in] diff … 引数の説明
	//! 
	mist_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }

	/// @brief コピー演算子
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	mist_iterator1( const mist_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }


	/// @brief コピー演算子
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator1& operator =( const mist_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}


	/// @brief 要素のアクセス
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	reference operator *(){ return( *data_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const_reference operator *() const { return( *data_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_reference operator []( difference_type dist ) const { return( data_[ dist * diff_pointer_ ] ); }


	/// @brief 前置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	mist_iterator1& operator ++( )
	{
		*this += 1;
		return( *this );
	}

	/// @brief 後置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator1 operator ++( int )
	{
		mist_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}

	/// @brief 前置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	mist_iterator1& operator --( )
	{
		*this -= 1;
		return( *this );
	}

	/// @brief 後置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator1 operator --( int )
	{
		mist_iterator1 old_val( *this );
		*this -= 1;
		return( old_val );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator1& operator +=( difference_type dist )
	{
		data_ += dist * diff_pointer_;
		return( *this );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator1& operator -=( difference_type dist )
	{
		data_ -= dist * diff_pointer_;
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const difference_type operator -( const mist_iterator1 &ite ) const
	{
		return( ( data_ - ite.data_ ) / diff_pointer_ );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator ==( const mist_iterator1 &ite ) const { return( data_ == ite.data_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator !=( const mist_iterator1 &ite ) const { return( !( *this == ite )  ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] ite  … 引数の説明
//! @param[in] dist … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] dist … 引数の説明
//! @param[in] ite  … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] ite  … 引数の説明
//! @param[in] dist … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}



/// @brief mistコンテナで利用する1次元操作用ランダムアクセスイテレータ
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T         … 引数の説明
//! @param Distance  … 引数の説明
//! @param Pointer   … 引数の説明
//! @param Reference … 引数の説明
//! 
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;					///< @brief 内部データ型．bool と同じ
	typedef Pointer pointer;				///< @brief データ型のポインター型．data の場合，data * となる
	typedef Reference reference;			///< @brief データ型の参照．data の場合，data & となる
	typedef size_t size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef Distance difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef Reference const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる

private:
	pointer data_;				///< @brief 変数の説明を書く
	difference_type index_;		///< @brief 変数の説明を書く
	difference_type width_;		///< @brief 変数の説明を書く
	difference_type step_;		///< @brief 変数の説明を書く

public:
	/// @brief 関数の説明を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] p     … 引数の説明
	//! @param[in] index … 引数の説明
	//! @param[in] width … 引数の説明
	//! @param[in] step  … 引数の説明
	//! 
	mist_iterator2( pointer p = NULL, difference_type index = 0, difference_type width = 1, difference_type step = 1 )
												: data_( p ), index_( index ), width_( width ), step_( step )
	{
	}

	/// @brief 関数の説明を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	mist_iterator2( const mist_iterator2 &ite ) : data_( ite.data_ ), index_( ite.index_ ), width_( ite.width_ ), step_( ite.step_ )
	{
	}


	/// @brief コピー演算子
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		if( &ite != this )
		{
			data_  = ite.data_;
			index_ = ite.index_;
			width_ = ite.width_;
			step_  = ite.step_;
		}
		return( *this );
	}


	/// @brief 要素のアクセス
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	reference operator *()
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const_reference operator *() const
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	reference operator []( difference_type dist ){ return( *( *this += dist ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_reference operator []( difference_type dist ) const { return( *( *this += dist ) ); }


	/// @brief 前置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	mist_iterator2& operator ++( )
	{
		*this += 1;
		return( *this );
	}

	/// @brief 後置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator2 operator ++( int )
	{
		mist_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}

	/// @brief 前置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	mist_iterator2& operator --( )
	{
		*this -= 1;
		return( *this );
	}

	/// @brief 後置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator2 operator --( int )
	{
		mist_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator2& operator +=( difference_type dist )
	{
		index_ += dist;
		return( *this );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_iterator2& operator -=( difference_type dist )
	{
		index_ -= dist;
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const difference_type operator -( const mist_iterator2 &ite ) const
	{
		return( index_ - ite.index_ );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator ==( const mist_iterator2 &ite ) const { return( *this - ite == 0 ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator !=( const mist_iterator2 &ite ) const { return( !( *this == ite ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator < ( const mist_iterator2 &ite ) const { return( *this - ite < 0 ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator <=( const mist_iterator2 &ite ) const { return( !( *this > ite ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator > ( const mist_iterator2 &ite ) const { return( ite < *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator >=( const mist_iterator2 &ite ) const { return( !( *this < ite ) ); }
};



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] ite  … 引数の説明
//! @param[in] dist … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] dist … 引数の説明
//! @param[in] ite  … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] ite  … 引数の説明
//! @param[in] dist … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] _Ite … 引数の説明
//! 
template< class _Ite >
class mist_reverse_iterator :
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type
	>
#else
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type,
		typename _Ite::pointer,
		typename _Ite::reference
	>
#endif
{
public:
 	typedef typename _Ite::size_type size_type;					///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
 	typedef typename _Ite::difference_type difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename _Ite::value_type value_type;				///< @brief ///< @brief 内部データ型．bool と同じ
	typedef typename _Ite::pointer pointer;						///< @brief データ型のポインター型．data の場合，data * となる
	typedef typename _Ite::reference reference;					///< @brief データ型の参照．data の場合，data & となる
	typedef typename _Ite::const_reference const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる

protected:
	_Ite current_iterator_;		///< @brief 変数の説明を書く

public:
	/// @brief コンストラクタ
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	mist_reverse_iterator( ){ }

	/// @brief コンストラクタ
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	mist_reverse_iterator( const _Ite &ite ) : current_iterator_( ite ){ }

	/// @brief コンストラクタ
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//! 
	mist_reverse_iterator( const mist_reverse_iterator &ite ) : current_iterator_( ite.current_iterator_ ){ }


	/// @brief コピー演算子
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @return 戻り値の説明
	//! 
	const mist_reverse_iterator& operator =( const _Ite &ite )
	{
		current_iterator_ = ite;
		return( *this );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @return 戻り値の説明
	//! 
	const mist_reverse_iterator& operator =( const mist_reverse_iterator &ite )
	{
		current_iterator_ = ite.current_iterator_;
		return( *this );
	}


	/// @brief 要素のアクセス
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	reference operator *()
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//!
	//! @return 戻り値の説明
	//! 
	const_reference operator *() const
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//!
	//! @return 戻り値の説明
	//! 
	reference operator []( difference_type dist ){ return( *( *this + dist ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//!
	//! @return 戻り値の説明
	//! 
	const_reference operator []( difference_type dist ) const { return( *( *this + dist ) ); }


	/// @brief 前置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	mist_reverse_iterator& operator ++( )
	{
		--current_iterator_;
		return( *this );
	}

	/// @brief 後置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_reverse_iterator operator ++( int )
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_--;
		return( old_val );
	}

	/// @brief 前置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	mist_reverse_iterator& operator --( )
	{
		++current_iterator_;
		return( *this );
	}

	/// @brief 後置型
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const mist_reverse_iterator operator --( int )
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_++;
		return( old_val );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//!
	//! @return 戻り値の説明
	//! 
	const mist_reverse_iterator& operator +=( difference_type dist )
	{
		current_iterator_ -= dist;
		return( *this );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] dist … 引数の説明
	//!
	//! @return 戻り値の説明
	//! 
	const mist_reverse_iterator& operator -=( difference_type dist )
	{
		current_iterator_ += dist;
		return( *this );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @return 戻り値の説明
	//! 
	const difference_type operator -( const mist_reverse_iterator &ite ) const
	{
		return( ite.current_iterator_ - current_iterator_ );
	}



	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator ==( const mist_reverse_iterator &ite ) const { return( current_iterator_ == ite.current_iterator_ ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator !=( const mist_reverse_iterator &ite ) const { return( !( *this == ite ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator < ( const mist_reverse_iterator &ite ) const { return( ite.current_iterator_ < current_iterator_  ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator <=( const mist_reverse_iterator &ite ) const { return( !( *this > ite ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator > ( const mist_reverse_iterator &ite ) const { return( ite < *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] ite … 引数の説明
	//!
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator >=( const mist_reverse_iterator &ite ) const { return( !( *this < ite ) ); }
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] ite1 … 引数の説明
//! @param[in] ite2 … 引数の説明
//!
//! @return 戻り値の説明
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
{
	return( mist_reverse_iterator< _Ite >( ite1 ) += ite2 );
}

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] ite  … 引数の説明
//! @param[in] dist … 引数の説明
//!
//! @return 戻り値の説明
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] dist … 引数の説明
//! @param[in] ite  … 引数の説明
//!
//! @return 戻り値の説明
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( typename _Ite::difference_type dist, const mist_reverse_iterator< _Ite > &ite )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}


//template< class _Ite >
//inline const typename mist_reverse_iterator< _Ite >::difference_type operator -( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
//{
//	return( ite1 - ite2 );
//}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] ite  … 引数の説明
//! @param[in] dist … 引数の説明
//!
//! @return 戻り値の説明
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator -( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) -= dist );
}

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_ITERATOR_H__
