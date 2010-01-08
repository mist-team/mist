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

/// @file mist/config/mist_alloc.h
//!
//! @brief MISTコンテナで利用するアロケータおよび，メモリ操作ルーチン
//!

#ifndef __INCLUDE_MIST_ALLOC_H__
#define __INCLUDE_MIST_ALLOC_H__

#include <cstring>
#include <memory>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "type_trait.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN



/// @brief ユーザー定義型用のメモリ操作関数群
template < bool b >
struct mist_memory_operator
{
	/// @brief num 個のオブジェクトを確保し，デフォルトの値で初期化する
	//! 
	//! 組み込み型のデータの場合は 0 で初期化する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] num       … 確保するオブジェクト数
	//! 
	//! @return 確保したメモリ領域の先頭アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		typename Allocator::value_type obj;
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.construct( &( ptr[i] ), obj );
		return( ptr );
	}


	/// @brief num 個のオブジェクトを確保し，引数の値で初期化する
	//! 
	//! 組み込み型のデータの場合は値を代入して初期化する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] num       … 確保するオブジェクト数
	//! @param[in] obj       … コピーコンストラクタに渡す初期値
	//! 
	//! @return 確保したメモリ領域の先頭アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.construct( &( ptr[i] ), obj );
		return( ptr );
	}


	/// @brief ポインタ s から e の範囲のデータで初期化する
	//! 
	//! 組み込み型のデータの場合は値を代入して初期化する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] s         … 開始ポインタ
	//! @param[in] e         … 終了ポインタ
	//! 
	//! @return 確保したメモリ領域の先頭アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( e - s, 0 );
		for( typename Allocator::pointer p = ptr ; s != e ; p++, s++ ) allocator.construct( p, *s );
		return( ptr );
	}


	/// @brief ptr から num 個のオブジェクトを開放しデストラクタを呼び出す
	//! 
	//! 組み込み型の場合はデストラクタは呼び出さない
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … 開放するメモリ領域の先頭アドレス
	//! @param[in] num       … 開放するオブジェクト数
	//! 
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Deallocator" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.destroy( &( ptr[i] ) );
		allocator.deallocate( ptr, num );
	}


	/// @brief ポインタ s から e までの間のデータを x にコピーする
	//! 
	//! 組み込み型の場合は memcpy を利用する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] s         … コピー元の先頭アドレス
	//! @param[in] e         … コピー元の末尾アドレス
	//! @param[in] x         … コピー先の先頭アドレス
	//! 
	//! @return コピー先の末尾アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator & /* allocator */, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Copy Function" << ::std::endl;
#endif
		while( s != e )
		{
			*x = *s;
			++x;
			++s;
		}
		return( x );
	}


	/// @brief ポインタ ptr から num 個のデータを to にコピーする
	//! 
	//! 組み込み型の場合は memcpy を利用する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … コピー元の先頭アドレス
	//! @param[in] num       … コピーするオブジェクト数
	//! @param[in] to        … コピー先の先頭アドレス
	//! 
	//! @return コピー先の末尾アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator & /* allocator */, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Copy Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) to[i] = ptr[i];
		return( to + num );
	}


	/// @brief ポインタ ptr から num 個のデータにobjを代入する
	//! 
	//! char 型の場合は memset を利用する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … 代入する先頭アドレス
	//! @param[in] num       … 代入するオブジェクト数
	//! @param[in] obj       … 代入するオブジェクト
	//! 
	template < class Allocator >
	static void fill_objects1( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Clean Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}


	/// @brief ポインタ ptr から num 個のデータに初期値を代入する
	//! 
	//! 組み込み型の場合は memset を利用する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … 代入する先頭アドレス
	//! @param[in] num       … 代入するオブジェクト数
	//! 
	template < class Allocator >
	static void fill_objects2( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Clean Function" << ::std::endl;
#endif
		typename Allocator::value_type obj;
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}
};




/// @brief 組み込み型用のメモリ操作関数群
template <>
struct mist_memory_operator< true >
{
	/// @brief num 個のオブジェクトを確保し，デフォルトの値で初期化する
	//! 
	//! 組み込み型のデータの場合は 0 で初期化する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] num       … 確保するオブジェクト数
	//! 
	//! @return 確保したメモリ領域の先頭アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memset Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		::memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}


	/// @brief num 個のオブジェクトを確保し，引数の値で初期化する
	//! 
	//! 組み込み型のデータの場合は値を代入して初期化する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] num       … 確保するオブジェクト数
	//! @param[in] obj       … コピーコンストラクタに渡す初期値
	//! 
	//! @return 確保したメモリ領域の先頭アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Specialized Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
		return( ptr );
	}


	/// @brief ポインタ s から e の範囲のデータで初期化する
	//! 
	//! 組み込み型のデータの場合は値を代入して初期化する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] s         … 開始ポインタ
	//! @param[in] e         … 終了ポインタ
	//! 
	//! @return 確保したメモリ領域の先頭アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( e - s, 0 );
		memcpy( ptr, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}


	/// @brief ptr から num 個のオブジェクトを開放しデストラクタを呼び出す
	//! 
	//! 組み込み型の場合はデストラクタは呼び出さない
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … 開放するメモリ領域の先頭アドレス
	//! @param[in] num       … 開放するオブジェクト数
	//! 
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Specialized Deallocator" << ::std::endl;
#endif
		allocator.deallocate( ptr, num );
	}


	/// @brief ポインタ s から e までの間のデータを x にコピーする
	//! 
	//! memcpy を利用してバイト単位でデータをコピーする
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] s         … コピー元の先頭アドレス
	//! @param[in] e         … コピー元の末尾アドレス
	//! @param[in] x         … コピー先の先頭アドレス
	//! 
	//! @return コピー先の末尾アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator & /* allocator */, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( x, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( x + ( e - s ) );
	}


	/// @brief ポインタ ptr から num 個のデータを to にコピーする
	//! 
	//! memcpy を利用してバイト単位でデータをコピーする
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … コピー元の先頭アドレス
	//! @param[in] num       … コピーするオブジェクト数
	//! @param[in] to        … コピー先の先頭アドレス
	//! 
	//! @return コピー先の末尾アドレス
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator & /* allocator */, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( to, ptr, num * sizeof( typename Allocator::value_type ) );
		return( to + num );
	}


	/// @brief ポインタ ptr から num 個のデータにobjを代入する
	//! 
	//! char 型の場合は memset を利用する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … 代入する先頭アドレス
	//! @param[in] num       … 代入するオブジェクト数
	//! @param[in] obj       … 代入するオブジェクト
	//! 
	template < class Allocator >
	static void fill_objects1( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, obj, num * sizeof( typename Allocator::value_type ) );
	}


	/// @brief ポインタ ptr から num 個のデータに初期値を代入する
	//! 
	//! memset を利用して，全要素を0に初期化する
	//! 
	//! @param[in] allocator … 使用するアロケータ
	//! @param[in] ptr       … 代入する先頭アドレス
	//! @param[in] num       … 代入するオブジェクト数
	//! 
	template < class Allocator >
	static void fill_objects2( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
	}
};



/// @brief MIST内部でSTLのアロケータを効果的に利用するための仕組み
//! 
//! データの型によって，メモリ操作を区別し高速化を図っている
//! 
//! @param T         … データ型
//! @param Allocator … アロケータタイプ
//! 
template < class T, class Allocator >
class mist_allocator : public Allocator
{
public:
	typedef Allocator base;											///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

protected:
	pointer   shared_pointer;		// 利用可能なメモリへのポインタ（外部とメモリを共有する場合のみ利用） 
	size_type shared_memory;		// 利用可能なメモリ量（外部とメモリを共有する場合のみ利用） 


public:
	/// @brief num 個のオブジェクトのメモリを確保し初期化を行う（組み込み型かどうかで内部の実装を分岐）
	pointer allocate_objects( size_type num )
	{
		if( num == 0 || num > max_size( ) )
		{
			return( NULL );
		}
		else if( !is_memory_shared( ) )
		{
			// 自動でメモリを割り当てる場合
			return( mist_memory_operator< is_builtin< T >::value >::allocate_objects1( *this, num ) );
		}
		else
		{
			// 外部割り当てのメモリを利用する場合
			return( shared_pointer );
		}
	}

	/// @brief num 個のオブジェクトのメモリを確保し obj を用いて初期化を行う（組み込み型かどうかで内部の実装を分岐）
	pointer allocate_objects( size_type num, const_reference obj )
	{
		if( num == 0 || num > max_size( ) )
		{
			return( NULL );
		}
		else if( !is_memory_shared( ) )
		{
			// 自動でメモリを割り当てる場合
			return( mist_memory_operator< is_builtin< T >::value >::allocate_objects2( *this, num, obj ) );
		}
		else
		{
			// 外部割り当てのメモリを利用する場合
			mist_memory_operator< type_and< is_char< T >::value, is_builtin< T >::value >::value >::fill_objects1( *this, shared_pointer, num, obj );
			return( shared_pointer );
		}
	}

	/// @brief num 個のオブジェクトを確保し，ポインタで囲まれた間のデータで初期化する(組み込み型のデータの場合は memcpy を利用する)
	pointer allocate_objects( const_pointer s, const_pointer e )
	{
		if( s >= e || e - s > max_size( ) )
		{
			return( NULL );
		}
		else if( !is_memory_shared( ) )
		{
			// 自動でメモリを割り当てる場合
			return( mist_memory_operator< is_builtin< T >::value >::allocate_objects3( *this, s, e ) );
		}
		else
		{
			// 外部割り当てのメモリを利用する場合
			mist_memory_operator< type_and< is_char< T >::value, is_builtin< T >::value >::value >::copy_objects2( *this, s, e - s, shared_pointer );
			return( shared_pointer );
		}
	}

	/// @brief num 個のオブジェクトのメモリを開放し，デストラクタを呼び出す（組み込み型の場合はデストラクタは呼ばない）
	void deallocate_objects( pointer ptr, size_type num )
	{
		if( !is_memory_shared( ) )
		{
			// 自動でメモリを割り当てる場合
			if( num <= 0 ) return;
			mist_memory_operator< is_builtin< T >::value >::deallocate_objects( *this, ptr, num );
		}
	}

	/// @brief ポインタ s から e までの間のデータを x にコピーする
	pointer copy_objects( const_pointer s, const_pointer e, pointer x ) const
	{
		if( s >= e ) return( x );
		return( mist_memory_operator< is_builtin< T >::value >::copy_objects1( *this, s, e, x ) );
	}

	/// @brief ポインタ ptr から num 個のデータを to にコピーする
	pointer copy_objects( const_pointer ptr, size_type num, pointer to ) const
	{
		if( num <= 0 ) return( to );
		return( mist_memory_operator< is_builtin< T >::value >::copy_objects2( *this, ptr, num, to ) );
	}

	/// @brief ポインタ ptr から num 個のデータにobjを代入する
	void fill_objects( pointer ptr, size_type num, const_reference obj ) const
	{
		if( num <= 0 ) return;
		mist_memory_operator< type_and< is_char< T >::value, is_builtin< T >::value >::value >::fill_objects1( *this, ptr, num, obj );
	}

	/// @brief ポインタ ptr から num 個のデータに初期値を代入する
	void fill_objects( pointer ptr, size_type num ) const
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_builtin< T >::value >::fill_objects2( *this, ptr, num );
	}

	/// @brief ptr から始まり num 個存在するオブジェクトを dest_num 個まで縮め，全ての要素をデフォルトの値で初期化する
	//! 
	//! @attention MSVCの場合は，メモリ領域のトリム操作が正しく動作しないため，メモリの再確保を行う
	//! 
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num )
	{
		if( num < dest_num ) return( ptr );			// 開放しようとしている配列の要素数は，トリム後の配列サイズよりサイズよりも小さい例外
		//if( num < 0 ) return( NULL );				// トリム先の配列サイズが0より小さい例外
		if( num == dest_num ) return( ptr );		// トリムによる変更の必要なし
		if( num == 0 ) return( NULL );

		if( !is_memory_shared( ) )
		{
			// 自動でメモリを割り当てる場合
#if _MIST_ALLOCATOR_MEMORY_TRIM_ != 0
			deallocate_objects( ptr + dest_num, num - dest_num );
			fill_objects( ptr, dest_num );
#else
			deallocate_objects( ptr, num );
			ptr = allocate_objects( dest_num );
#endif
		}
		return( dest_num == 0 ? NULL : ptr );
	}

	/// @brief ptr から始まり num 個存在するオブジェクトを dest_num 個まで縮め，全ての要素を値 obj で初期化する
	//! 
	//! @attention MSVCの場合は，メモリ領域のトリム操作が正しく動作しないため，メモリの再確保を行う
	//! 
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num, const_reference obj )
	{
		if( num < dest_num ) return( ptr );			// 開放しようとしている配列の要素数は，トリム後の配列サイズよりサイズよりも小さい例外
		if( num < 0 ) return( NULL );				// トリム先の配列サイズが0より小さい例外
		if( num == dest_num ) return( ptr );		// トリムによる変更の必要なし
		if( num == 0 ) return( NULL );

		if( !is_memory_shared( ) )
		{
			// 自動でメモリを割り当てる場合
#if _MIST_ALLOCATOR_MEMORY_TRIM_ != 0
			deallocate_objects( ptr + dest_num, num - dest_num );
			fill_objects( ptr, dest_num, obj );
#else
			deallocate_objects( ptr, num );
			ptr = allocate_objects( dest_num, obj );
#endif
		}
		return( dest_num == 0 ? NULL : ptr );
	}

	/// @brief 使用しているアロケータが確保可能なメモリの最大値を返す
	size_type max_size( ) const
	{
		return( shared_memory == 0 ? base::max_size( ) : shared_memory );
	}

	/// @brief 外部で割り当てられたメモリ領域を使用しているかどうか
	bool is_memory_shared( ) const
	{
		return( shared_memory != 0 );
	}

	/// @brief 他のアロケータを代入する
	const Allocator &operator=( const Allocator &alloc )
	{
		if( &alloc != this )
		{
			// 外部メモリを利用しているかどうかは受け継がない
			base::operator=( alloc );
		}
		return( *this );
	}

	/// @brief 他のアロケータを代入する
	const mist_allocator &operator=( const mist_allocator &alloc )
	{
		if( &alloc != this )
		{
			// 外部メモリを利用しているかどうかは受け継がない
			base::operator=( alloc );
		}
		return( *this );
	}


	/// @brief デフォルトコンストラクタ
	mist_allocator( ) : base( ), shared_pointer( NULL ), shared_memory( 0 ){}


	/// @brief デフォルトコンストラクタ
	//! 
	//! @attention 外部で割り当てたメモリ領域を利用する場合
	//! 
	/// @param[in] ptr       … 外部メモリの先頭のポインタ
	/// @param[in] mem_shared … 外部メモリに割り当てたメモリ量
	//! 
	mist_allocator( pointer ptr, size_type mem_shared ) : base( ),  shared_pointer( mem_shared == 0 ? NULL : ptr ), shared_memory( ptr == NULL ? 0 : mem_shared ){}


	/// @brief 他のアロケータを用いて初期化する
	mist_allocator( const Allocator &alloc ) : base( alloc ), shared_pointer( NULL ), shared_memory( 0 ){}


	/// @brief 他のアロケータを用いて初期化する
	mist_allocator( const mist_allocator &alloc ) : base( alloc ), shared_pointer( NULL ), shared_memory( 0 ){}
};



// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_ALLOC_H__
