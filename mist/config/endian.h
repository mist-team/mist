// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

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


//! @addtogroup endian_group エンディアンに関する情報の取得及び変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/config/endian.h>
//! @endcode
//!
//! @{




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

	/// @brief 内部で使用するバイト数を返す
	size_type length( ) const { return( sizeof( value_type ) ); }

	/// @brief index で指定されて位置のバイトを取得する
	unsigned char &operator[]( size_type index ){ return( byte[ index ] ); }

	/// @brief index で指定されて位置の const バイトを取得する
	const unsigned char &operator[]( size_type index ) const { return( byte[ index ] ); }

	/// @brief 値を取得する
	const value_type get_value( ) const { return( value ); }

	/// @brief 値を設定する
	value_type set_value( const value_type &v ) { return( value = v ); }

	/// @brief バイト列を取得する
	const unsigned char * get_bytes( ) const { return( byte ); }
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



/// @}
//  エンディアンに関する情報の取得及び変換グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_ENDIAN__
