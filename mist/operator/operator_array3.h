/// @file mist/operator/operator_array3.h
//!
//! @brief 3次元画像用のオペレータ定義
//!
#ifndef __INCLUDE_MIST_OPERATOR_ARRAY3__
#define __INCLUDE_MIST_OPERATOR_ARRAY3__



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
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 足し算できません例外
		::std::cout << "can't add array3s." << ::std::endl;
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
inline const array3< T, Allocator >& operator -=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 引き算できません例外
		::std::cout << "can't subtract array3s." << ::std::endl;
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
inline const array3< T, Allocator >& operator *=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 掛け算できません例外
		::std::cout << "can't multiply array3s." << ::std::endl;
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
inline const array3< T, Allocator >& operator /=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 割り算できません例外
		::std::cout << "can't divide array3s." << ::std::endl;
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
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
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
inline const array3< T, Allocator >& operator -=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
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
inline const array3< T, Allocator >& operator *=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
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
inline const array3< T, Allocator >& operator /=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
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
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	array3< T, Allocator > o( a.size1( ), a.size2( ), a.size3( ), a.reso1( ), a.reso2( ), a.reso3( ) );
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
inline array3< T, Allocator > operator +( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) += a2 );
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
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) -= a2 );
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
inline array3< T, Allocator > operator *( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) *= a2 );
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
inline array3< T, Allocator > operator /( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) /= a2 );
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
inline array3< T, Allocator > operator +( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) += val );
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
inline array3< T, Allocator > operator +( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) += val );
}




/// @brief 定数との引き算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] a   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) -= val );
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
inline array3< T, Allocator > operator -( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) -= val );
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
inline array3< T, Allocator > operator *( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) *= val );
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
inline array3< T, Allocator > operator *( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) *= val );
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
inline array3< T, Allocator > operator /( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) /= val );
}



#endif // __INCLUDE_MIST_OPERATOR_ARRAY3__
