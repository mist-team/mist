/// @file mist/config/endian.h
//!
//! @brief 計算機のエンディアンを取得し，自動的に変換するライブラリ
//!

#ifndef __INCLUDE_MIST_ENDIAN__
#define __INCLUDE_MIST_ENDIAN__

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN



/// @brief バイト配列と値のペアを表現するクラス
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T … 引数の説明
//! 
template < class T >
union byte_array
{
public:
	typedef T      value_type;		///< @brief 説明を書く
	typedef size_t size_type;		///< @brief 説明を書く

private:
	value_type value;								///< @brief 説明を書く
	unsigned char byte[ sizeof( value_type ) ];		///< @brief 説明を書く

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	byte_array( ) : value( 0 ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] v … 引数の説明
	//! 
	byte_array( const value_type v ) : value( v ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] v … 引数の説明
	//! 
	byte_array( const byte_array &v ) : value( v.value ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] b … 引数の説明
	//! 
	byte_array( const unsigned char *b )
	{
		for( size_type i = 0 ; i < sizeof( value_type ) ; i++ )
		{
			byte[ i ] = b[ i ];
		}
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] index … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	unsigned char &operator[]( size_type index ){ return( byte[ index ] ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] index … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const unsigned char &operator[]( size_type index ) const { return( byte[ index ] ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	const value_type get_value( ) const { return( value ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] v … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	value_type set_value( const value_type &v ) { return( value = v ); }
};


/// @brief 現在のマシンのエンディアンがリトルエンディアンかどうかを調べる
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
inline bool _is_little_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 1 );
}


/// @brief 現在のマシンのエンディアンがビッグエンディアンかどうかを調べる
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
inline bool _is_big_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 0 );
}


/// @brief byte_array内のバイトスワップをする
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in,out] bytes … 引数の説明
//! 
template < class T >
inline void swap_bytes( byte_array< T > &bytes )
{
	typedef typename byte_array< T >::size_type size_type;
	byte_array< T > tmp( bytes );
	for( size_type i = 0 ; i < sizeof( T ) ; ++i )
	{
		bytes[ i ] = tmp[ sizeof( T ) - i - 1 ];
	}
}


/// @brief byte_array内のデータを現在の計算機のエンディアンに合わせる
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] bytes              … 引数の説明
//! @param[in] from_little_endian … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T >
inline byte_array< T > to_current_endian( const byte_array< T > &bytes, bool from_little_endian )
{
	static bool current_endian = _is_little_endian_( );
	if( current_endian != from_little_endian )
	{
		byte_array< T > tmp( bytes );
		swap_bytes( tmp );
		return( tmp );
	}
	else
	{
		return( bytes );
	}
}


/// @brief byte_array内のデータを現在の計算機のエンディアンから目的のエンディアンに合わせる
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] bytes              … 引数の説明
//! @param[in] from_little_endian … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T >
inline byte_array< T > from_current_endian( const byte_array< T > &bytes, bool to_little_endian )
{
	static bool current_endian = _is_little_endian_( );
	if( current_endian != to_little_endian )
	{
		byte_array< T > tmp( bytes );
		swap_bytes( tmp );
		return( tmp );
	}
	else
	{
		return( bytes );
	}
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_ENDIAN__
