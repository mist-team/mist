/// @file mist/operator/operator_array2.h
//!
//! @brief 2次元画像用のオペレータ定義
//!
#ifndef __INCLUDE_MIST_OPERATOR_ARRAY2__
#define __INCLUDE_MIST_OPERATOR_ARRAY2__



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
inline const array2< T, Allocator >& operator +=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY2_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 足し算できません例外
		::std::cout << "can't add array2s." << ::std::endl;
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
inline const array2< T, Allocator >& operator -=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY2_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 引き算できません例外
		::std::cout << "can't subtract array2s." << ::std::endl;
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
inline const array2< T, Allocator >& operator *=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY2_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 掛け算できません例外
		::std::cout << "can't multiply array2s." << ::std::endl;
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
inline const array2< T, Allocator >& operator /=( array2< T, Allocator > &a1, const array2< T, Allocator >  &a2 )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY2_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// 割り算できません例外
		::std::cout << "can't divide array2s." << ::std::endl;
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
inline const array2< T, Allocator >& operator +=( array2< T, Allocator > &a1, typename array2< T, Allocator >::value_type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
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
inline const array2< T, Allocator >& operator -=( array2< T, Allocator > &a1, typename array2< T, Allocator >::value_type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
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
inline const array2< T, Allocator >& operator *=( array2< T, Allocator > &a1, typename array2< T, Allocator >::value_type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
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
inline const array2< T, Allocator >& operator /=( array2< T, Allocator > &a1, typename array2< T, Allocator >::value_type val )
{
	typedef typename array2< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY2_OPERATION_ != 0
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
inline array2< T, Allocator > operator -( const array2< T, Allocator > &a )
{
	typedef typename array2< T, Allocator >::size_type size_type;
	array2< T, Allocator > o( a.size1( ), a.size2( ), a.reso1( ), a.reso2( ) );
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
inline array2< T, Allocator > operator +( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
	return( array2< T, Allocator >( a1 ) += a2 );
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
inline array2< T, Allocator > operator -( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
	return( array2< T, Allocator >( a1 ) -= a2 );
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
inline array2< T, Allocator > operator *( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
	return( array2< T, Allocator >( a1 ) *= a2 );
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
inline array2< T, Allocator > operator /( const array2< T, Allocator > &a1, const array2< T, Allocator > &a2 )
{
	return( array2< T, Allocator >( a1 ) /= a2 );
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
inline array2< T, Allocator > operator +( const array2< T, Allocator > &a, typename array2< T, Allocator >::value_type val )
{
	return( array2< T, Allocator >( a ) += val );
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
inline array2< T, Allocator > operator +( typename array2< T, Allocator >::value_type val, const array2< T, Allocator > &a )
{
	return( array2< T, Allocator >( a ) += val );
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
inline array2< T, Allocator > operator -( const array2< T, Allocator > &a, typename array2< T, Allocator >::value_type val )
{
	return( array2< T, Allocator >( a ) -= val );
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
inline array2< T, Allocator > operator -( typename array2< T, Allocator >::value_type val, const array2< T, Allocator > &a )
{
	return( array2< T, Allocator >( a.size1( ), a.size2( ), val ) -= a );
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
inline array2< T, Allocator > operator *( const array2< T, Allocator > &a, typename array2< T, Allocator >::value_type val )
{
	return( array2< T, Allocator >( a ) *= val );
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
inline array2< T, Allocator > operator *( typename array2< T, Allocator >::value_type val, const array2< T, Allocator > &a )
{
	return( array2< T, Allocator >( a ) *= val );
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
inline array2< T, Allocator > operator /( const array2< T, Allocator > &a, typename array2< T, Allocator >::value_type val )
{
	return( array2< T, Allocator >( a ) /= val );
}


#endif // __INCLUDE_MIST_OPERATOR_ARRAY2__
