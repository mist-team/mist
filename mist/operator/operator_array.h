/// @file mist/operator/operator_array.h
//!
//! @brief 1次元配列用のオペレータ定義
//!
#ifndef __INCLUDE_MIST_OPERATOR_ARRAY__
#define __INCLUDE_MIST_OPERATOR_ARRAY__


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 足し算できません例外
		::std::cout << "can't add arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 引き算できません例外
		::std::cout << "can't subtract arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 掛け算できません例外
		::std::cout << "can't multiply arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 割り算できません例外
		::std::cout << "can't divide arrays." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1  … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1  … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1  … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1  … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( val == 0 )
	{
		// ゼロ除算発生
		::std::cout << "zero division occured." << ::std::endl;
		return;
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= val;
	return( a1 );
}



/// @brief 符号反転
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	array< T, Allocator > o( a.size( ) );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -a[i];
	return( o );
}




/// @brief 足し算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator +( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) += a2 );
}




/// @brief 引き算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) -= a2 );
}




/// @brief 掛け算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator *( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) *= a2 );
}



/// @brief 割り算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a1 … 引数の説明
//! @param[in] a2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator /( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) /= a2 );
}



/// @brief 定数との足し算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator +( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) += val );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] val … 引数の説明
//! @param[in] a   … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator +( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) += val );
}




/// @brief 定数との引き
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) -= val );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] val … 引数の説明
//! @param[in] a   … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator -( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array2< T, Allocator >( a.size( ), val ) -= a );
}




/// @brief 定数との掛け算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator *( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) *= val );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] val … 引数の説明
//! @param[in] a   … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator *( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) *= val );
}



/// @brief 定数との割り算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator /( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) /= val );
}

#endif // __INCLUDE_MIST_OPERATOR_ARRAY__
