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
//! 各データ型とバイト列を同時に利用することが可能です
//! 
//! @param T … 使用するデータ型
//! 
template < class T >
union byte_array
{
public:
	typedef T      value_type;		///< @brief データ型タイプ
	typedef size_t size_type;		///< @brief size_tに同じ

private:
	value_type value;								///< @brief データ型タイプが表す値
	unsigned char byte[ sizeof( value_type ) ];		///< @brief 値をバイト表現に直したもの

public:
	/// @brief デフォルトコンストラクタ
	byte_array( ) : value( 0 ){ }


	/// @brief 値 v を用いて初期化する
	byte_array( const value_type v ) : value( v ){ }


	/// @brief 他の byte_array を用いて初期化する
	byte_array( const byte_array &v ) : value( v.value ){ }


	/// @brief バイト列を用いて初期化を行う
	//!
	//! @attention 入力されるバイト列の要素数は，sizeof( value_type ) 以上でなくてはならない
	//!
	byte_array( const unsigned char *b )
	{
		for( size_type i = 0 ; i < sizeof( value_type ) ; i++ )
		{
			byte[ i ] = b[ i ];
		}
	}

	/// @brief index で指定されて位置のバイトを取得する
	unsigned char &operator[]( size_type index ){ return( byte[ index ] ); }

	/// @brief index で指定されて位置の const バイトを取得する
	const unsigned char &operator[]( size_type index ) const { return( byte[ index ] ); }

	/// @brief 値を取得する
	const value_type get_value( ) const { return( value ); }

	/// @brief 値を設定する
	value_type set_value( const value_type &v ) { return( value = v ); }
};


/// @brief 現在のマシンのエンディアンがリトルエンディアンかどうかを調べる
//! 
//! @retval true  … リトルエンディアン
//! @retval false … ビッグエンディアン
//! 
inline bool _is_little_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 1 );
}


/// @brief 現在のマシンのエンディアンがビッグエンディアンかどうかを調べる
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @retval true  … ビッグエンディアン
//! @retval false … リトルエンディアン
//! 
inline bool _is_big_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 0 );
}


/// @brief byte_array内のバイトスワップをする
//! 
//! @param[in,out] bytes … バイトスワップされるデータ
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
//! - リトルエンディアンの計算機で，ビッグエンディアンのデータを入力し，from_little_endian = false とすると，変換後のデータはリトルエンディアンとなる
//! - ビッグエンディアンの計算機で，リトルエンディアンのデータを入力し，from_little_endian = true  とすると，変換後のデータはビッグエンディアンとなる
//! - それ以外の場合は何も変換されない
//! 
//! @param[in] bytes              … 変換されるデータ
//! @param[in] from_little_endian … 変換前のエンディアンタイプ
//! 
//! @return 変換後のデータ
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
//! - リトルエンディアンの計算機で，to_little_endian = false とすると，変換後のデータはビッグエンディアンとなる
//! - ビッグエンディアンの計算機で，to_little_endian = true  とすると，変換後のデータはリトルエンディアンとなる
//! - それ以外の場合は，それぞれの計算機のエンディアンのままで何も変換されない
//! 
//! @param[in] bytes            … 変換されるデータ
//! @param[in] to_little_endian … 変換後のエンディアンタイプ
//! 
//! @return 変換後のデータ
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
