/****************************************************************************************************************************
**       MIST ( Media Integration Standard Toolkit )
**
**        We defined following template classes.
**
**          array  : template class of one dimensionl array with STL support.
**          array1 : template class of one dimensionl array containing resolution.
**          array2 : template class of two dimensionl array containing pixel resolution.
**          array3 : template class of three dimensionl array containing voxel resolution.
**          matrix : template class of matrix with its operations, and impremented using expression template technique.
**          vector : template class of vector with its operations, and impremented using expression template technique.
**
**
**         We developed these programs since 2003/09/05.
**
**             $LastChangedDate::                      $
**             $LastChangedRevision$
**             $LastChangedBy$
**             $HeadURL$
**
**
**              Copyright MIST Project Team.
**                    All Rights Reserved.
**
****************************************************************************************************************************/

// 
// Copyright (c) 2003-2008, MIST Project, Intelligent Media Integration COE, Nagoya University
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


/// @file mist/mist.h
//!
//! @brief MISTの基本となる音声・画像を扱うコンテナ
//!
//! MISTで提供する全てのアルゴリズム等で利用するコンテナ（STL風）の集まりです．
//! 基本的には，STL（Standard Template Library）で提供されているコンテナとほぼ互換性を持っています．
//! 特徴としては，コンテナ内のデータはメモリ空間上で連続となるように実装されています．
//! そして，STLのアルゴリズム等で用いるイテレータの内，ランダムアクセスイテレータを実装してあります．
//! イテレータの詳細に関しては，STLの解説本に譲ります．
//! また，MISTのコンテナでは組み込み型とユーザー定義型で，利用するメモリ操作を区別しています．
//! そうすることで，コンテナの代入等で若干高速な実行が可能となっています．
//!
//! MISTプロジェクトで提供するコンテナは，以下の合計6つのコンテナで構成されています．
//!
//! - 基底1次元配列（STL風の1次元配列用コンテナ）
//! - 解像度付1次元配列（各要素の解像度のデータを保持，例えば1mmなど）
//! - 解像度付2次元配列
//! - 解像度付3次元配列
//! - 行列
//! - ベクトル
//!

#ifndef __INCLUDE_MIST_H__
#define __INCLUDE_MIST_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif


#ifndef __INCLUDE_MIST_ALLOC_H__
#include "config/mist_alloc.h"
#endif

#ifndef __INCLUDE_MIST_ITERATOR_H__
#include "config/iterator.h"
#endif



// mist名前空間の始まり
_MIST_BEGIN



/// @brief 要素数が可変の1次元配列
//! 
//! 全てのMISTコンテナの基本となるクラス
//! （@ref mist/mist.h をインクルードする）
//! 
//! @param T         … MISTのコンテナ内に格納するデータ型
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < class T, class Allocator = std::allocator< T > >
class array
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief データ型の変換を行う
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array< TT, AAllocator > other;
	};


protected:
	mist_allocator< T, Allocator > allocator_;		///< @brief 各コンテナで利用するアロケータオブジェクト

private:
	size_type size_;								///< @brief コンテナに格納されている要素数

protected:
	T* data_;	///< @brief コンテナで扱うデータの先頭を表すポインタ

public:
	/// @brief コンテナ内の要素数を変更する
	//! 
	//! コンテナのサイズを num 個に変更する．
	//! 元のコンテナサイズと異なる場合は，自動的にサイズを調整する．
	//! 要素数が変更された場合のみ全ての要素の値はデフォルトコンストラクタで初期化される．
	//! また，組み込み型（int や double など）の場合は，全ての要素を0で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! @param[in] num  … リサイズ後のコンテナ内の要素数
	//! @param[in] dmy1 … ダミー変数（使用しない）
	//! @param[in] dmy2 … ダミー変数（使用しない）
	//! 
	//! @retval true  … 正常にリサイズが終了
	//! @retval false … リサイズ後のメモリを確保できなかった場合
	//! 
	bool resize( size_type num, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		if( size_ < num )
		{
			allocator_.deallocate_objects( data_, size_ );
			data_ = allocator_.allocate_objects( num );
			size_ = data_ == NULL ? 0 : num;
		}
		else if( size_ > num )
		{
			data_ = allocator_.trim_objects( data_, size_, num );
			size_ = data_ == NULL ? 0 : num;
		}

		return( data_ != NULL );
	}

	/// @brief コンテナ内の要素をトリミングする
	//!
	//! メモリの確保に失敗したら，空のコンテナとなる
	//!
	//! @param[in] index … トリミングの開始位置（ゼロから始まるインデックス）
	//! @param[in] num   … トリミング後の要素数（-1の場合は，最後までをコピーする）
	//! 
	//! @retval true  … トリミングに成功した場合（元とサイズが変わらない場合も含む）
	//! @retval false … 不正なトリミングを行おうとした場合
	//! 
	bool trim( size_type index, difference_type num = -1 )
	{
		difference_type num_ = size( );
		if( num_ <= static_cast< difference_type >( index ) || num_ < static_cast< difference_type >( index + num ) )
		{
			return( false );
		}
		else if( num_ == num )
		{
			return( true );
		}

		if( num < 0 )
		{
			num = size( ) - index;
		}

		if( is_memory_shared( ) )
		{
			// 外部メモリを利用している場合
			array o( *this );

			if( resize( num ) )
			{
				for( difference_type i = 0 ; i < num ; i++ )
				{
					operator []( i ) = o( i + index );
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			array o( num );
			for( difference_type i = 0 ; i < num ; i++ )
			{
				o[ i ] = operator []( i + index );
			}

			swap( o );
		}

		return( true );
	}


	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! @attention どちらかのコンテナが外部メモリを利用している場合は，スワップは必ず失敗する
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	//! @retval true  … データのスワップに成功
	//! @retval false … データのスワップに失敗
	//! 
	bool swap( array &a )
	{
		if( is_memory_shared( ) || a.is_memory_shared( ) )
		{
			return( false );
		}
		else
		{
			size_type _dmy_size = size_;
			size_ = a.size_;
			a.size_ = _dmy_size;

			value_type *dmy_data_ = data_;
			data_ = a.data_;
			a.data_ = dmy_data_;

			return( true );
		}
	}


	/// @brief コンテナの要素を空にする
	//! 
	//! コンテナに格納されているデータを全て削除し，コンテナを空（要素数0）にする
	//! 
	void clear( )
	{
		allocator_.deallocate_objects( data_, size_ );
		size_ = 0;
		data_ = NULL;
	}


	/// @brief コンテナ内のデータ要素をデフォルト値で初期化する
	//! 
	//! デフォルトコンストラクタの値で初期化する．
	//! 組み込み型（int や double など）の場合は，全ての要素を0で初期化する．
	//! 
	void fill( )
	{
		allocator_.fill_objects( data_, size_ );
	}


	/// @brief コンテナ内のデータ要素を指定された値で初期化する
	//! 
	//! 全ての要素を値 val で初期化する．
	//! 
	//! @param[in] val … 要素を初期化する値
	//! 
	void fill( const value_type &val )
	{
		allocator_.fill_objects( data_, size_, val );
	}


	/// @brief コンテナが空かどうかを判定
	//! 
	//! @retval true  … コンテナの要素数が空（0）の場合
	//! @retval false … コンテナの要素数が空（0）でない場合
	//! 
	bool empty( ) const { return( size_ == 0 ); }


	size_type size( ) const { return( size_ ); }	///< @brief コンテナに格納されているデータ数を返す
	size_type size1( ) const { return( size_ ); }	///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type size2( ) const { return( 1 ); }		///< @brief Y軸方向のコンテナに格納されているデータ数を返す（常に1を返す）
	size_type size3( ) const { return( 1 ); }		///< @brief Z軸方向のコンテナに格納されているデータ数を返す（常に1を返す）
	size_type width( ) const { return( size_ ); }	///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type height( ) const { return( 1 ); }		///< @brief Y軸方向のコンテナに格納されているデータ数を返す（常に1を返す）
	size_type depth( ) const { return( 1 ); }		///< @brief Z軸方向のコンテナに格納されているデータ数を返す（常に1を返す）

	double reso1( double r1 ){ return( 1.0 ); }		///< @brief X軸方向の解像度を返す
	double reso1( ) const { return( 1.0 ); }		///< @brief X軸方向の解像度を設定する
	double reso2( double r2 ){ return( 1.0 ); }		///< @brief Y軸方向の解像度を返す
	double reso2( ) const { return( 1.0 ); }		///< @brief Y軸方向の解像度を設定する
	double reso3( double r3 ){ return( 1.0 ); }		///< @brief Z軸方向の解像度を返す
	double reso3( ) const { return( 1.0 ); }		///< @brief Z軸方向の解像度を設定する）


	/// @brief コンテナ内の要素が占めるデータ量をバイト単位で返す
	size_type byte( ) const { return( size_ * sizeof( value_type ) ); }

	/// @brief 外部で割り当てられたメモリ領域を使用しているかどうか
	bool is_memory_shared( ) const { return( allocator_.is_memory_shared( ) ); }


	/// @brief コンテナの先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code 順方向ランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.begin( );
	//! for( ; ite1 != a.end( ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator begin( ){ return( iterator( paccess( 0 ), 1 ) ); }

	/// @brief コンテナの先頭を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator begin( ) const { return( const_iterator( paccess( 0 ), 1 ) ); }


	/// @brief コンテナの末尾を指すランダムアクセスイテレータを返す
	iterator end( ){ return( iterator( paccess( size( ) ), 1 ) ); }

	/// @brief コンテナの末尾を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator end( ) const { return( const_iterator( paccess( size( ) ), 1 ) ); }



	/// @brief コンテナの末尾を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code 逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.rbegin( );
	//! for( ; ite2 != a.rend( ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }

	/// @brief コンテナの末尾を指すコンスト型のコンテナを操作する逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }


	/// @brief コンテナの先頭を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }

	/// @brief コンテナの先頭を指すコンスト型のコンテナを操作する逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }


	/// @brief 使用しているアロケータが確保可能なメモリの最大値を返す
	size_type max_size( ) const { return( allocator_.max_size( ) ); }



private: // サポートしないＳＴＬの関数（実装・使用しちゃだめ）
	iterator erase( iterator i );
	iterator erase( iterator s, iterator e );
	iterator insert( iterator i, const value_type &val );
	void insert( iterator i, size_type num, const value_type &val );

public:
	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! コピー元とコピー先でデータ型（array< data > の data）が異なる場合の代入を行う．
	//! メモリの確保に失敗したら，空のコンテナとなる
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class TT, class AAlocator >
	const array& operator =( const array< TT, AAlocator >  &o )
	{
		if( size_ > o.size( ) )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size( ) );
			size_ = data_ == NULL ? 0 : o.size( );
		}
		else if( size_ < o.size( ) )
		{
			allocator_.deallocate_objects( data_, size_ );
			data_ = allocator_.allocate_objects( o.size( ) );
			size_ = data_ == NULL ? 0 : o.size( );
		}

		if( data_ != NULL )
		{
			for( size_type i = 0 ; i < size_ ; i++ )
			{
				data_[ i ] = static_cast< value_type >( o[ i ] );
			}
		}

		return( *this );
	}

	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! メモリの確保に失敗したら，空のコンテナとなる
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	const array& operator =( const array  &o )
	{
		if( this == &o ) return( *this );

		// まず，アロケータをコピーする
		allocator_ = o.allocator_;

		if( size_ > o.size_ )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size_ );
			size_ = data_ == NULL ? 0 : o.size( );
		}
		else if( size_ < o.size_ )
		{
			allocator_.deallocate_objects( data_, size_ );
			data_ = allocator_.allocate_objects( o.size( ) );
			size_ = data_ == NULL ? 0 : o.size( );
		}

		if( data_ != NULL )
		{
			allocator_.copy_objects( o.data_, size_, data_ );
		}

		return( *this );
	}

// コンテナ内の要素へのアクセス演算子
protected:
	/// @brief index で示される位置の要素のポインタを返す
	//! 
	//! @param[in] index  … コンテナ内の要素位置
	//! 
	//! @return 指定された要素を示すポインタ
	//! 
	pointer paccess( size_type index )
	{
		return( data_ + index );
	}

	/// @brief index で示される位置の要素の const ポインタを返す
	//! 
	//! @param[in] index  … コンテナ内の要素位置
	//! 
	//! @return 指定された要素を示す const ポインタ
	//! 
	const_pointer paccess( size_type index ) const
	{
		return( data_ + index );
	}

public:
	/// @brief index で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	//! 
	/// @return 指定された要素を示す参照
	//!
	reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	//! 
	/// @return 指定された要素を示す const 参照
	//!
	const_reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	//! 
	/// @return 指定された要素を示す参照
	//!
	reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	/// @param[in] dmy1  … 使用しない
	/// @param[in] dmy2  … 使用しない
	//! 
	/// @return 指定された要素を示す const 参照
	//!
	const_reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す参照
	//!
	reference operator []( size_type index )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す const 参照
	//!
	const_reference operator []( size_type index ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}

public:
	/// @brief ディフォルトコンストラクタ．要素数 0 のコンテナを作成する
	array( ) : allocator_( ), size_( 0 ), data_( NULL ){}

	/// @brief アロケータ a のコピーを利用する
	explicit array( const Allocator &a ) : allocator_( a ), size_( 0 ), data_( NULL ){}


	/// @brief 要素数 num 個のコンテナを作成し，デフォルト値で要素を初期化する
	explicit array( size_type num ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief 要素数 num 個のコンテナを作成し，アロケータ a のコピーを利用する
	array( size_type num, const Allocator &a ) : allocator_( a ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief 要素数 num 個のコンテナを作成し，値 val で初期化する
	array( size_type num, const value_type &val ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief 要素数 num 個のコンテナを作成し，値 val で初期化し，アロケータ a のコピーを利用する
	array( size_type num, const value_type &val, const Allocator &a ) : allocator_( a ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief イテレータ s と e の範囲の値を用いて，配列を初期化する
	array( const_iterator s, const_iterator e ) : allocator_( ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief イテレータ s と e の範囲の値を用いて，配列を初期化し，アロケータ a のコピーを利用する
	array( const_iterator s, const_iterator e, const Allocator &a ) : allocator_( a ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief ptr が指すメモリ領域に，要素数 num 個のコンテナを作成する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array( size_type num, pointer ptr, size_type mem_available ) : allocator_( ptr, mem_available ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief ptr が指すメモリ領域に，要素数 num 個のコンテナを作成し値 val で初期化する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array( size_type num, const value_type &val, pointer ptr, size_type mem_available ) : allocator_( ptr, mem_available ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief 他の array 配列で要素の型が異なるものから同じ要素数の配列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	array( const array< TT, AAlocator > &o ) : allocator_( ), size_( o.size( ) ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL )
		{
			size_ = 0;
		}
		else
		{
			for( size_type i = 0 ; i < size_ ; i++ ) data_[i] = static_cast< value_type >( o[i] );
		}
	}

	/// @brief 他の array 配列で同じ要素型のものを用いて初期化する
	array( const array< T, Allocator > &o ) : allocator_( o.allocator_ ), size_( o.size_ ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL )
		{
			size_ = 0;
		}
		else
		{
			allocator_.copy_objects( o.data_, size_, data_ );
		}
	}

	/// @brief コンテナが利用しているリソースを全て開放する
	~array( )
	{
		clear( );
	}
};



/// @brief 要素数が可変の解像度付1次元配列
//! 
//! 主に音声を扱うための基本クラス
//! （@ref mist/mist.h をインクルードする）
//! 
//! @param T         … MISTのコンテナ内に格納するデータ型
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < class T, class Allocator = std::allocator< T > >
class array1 : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief データ型の変換を行う
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array1< TT, AAllocator > other;
	};

private:
	typedef array< T, Allocator > base;		///< @brief 基底クラスの別名

protected:
	double reso1_;							///< @brief コンテナ内の要素の解像度（ミリや周波数など）

public:
	double reso1( double r1 ){ return( reso1_ = r1 ); }								//< @brief X軸方向の解像度を r1 に設定し，設定後の値を返す
	double reso1( ) const { return( reso1_ ); }										//< @brief X軸方向の解像度を返す
	void reso( double r1, double dmy1 = 1.0, double dmy2 = 1.0 ){ reso1_ = r1; }	//< @brief X軸方向の解像度を r1 に設定し，設定後の値を返す


/************************************************************************************************************
**
**      X方向に対する順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief X軸方向の先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code 順方向ランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.x_begin( );
	//! for( ; ite1 != a.x_end( ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator x_begin( ){ return( base::begin( ) ); }

	/// @brief X軸方向の先頭を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator x_begin( ) const { return( base::begin( ) ); }


	/// @brief X軸方向の末尾を指すランダムアクセスイテレータを返す
	iterator x_end( ){ return( base::end( ) ); }

	/// @brief X軸方向の末尾を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator x_end( ) const { return( base::end( ) ); }



	/// @brief X軸方向の末尾を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code 逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.x_rbegin( );
	//! for( ; ite2 != a.x_rend( ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator x_rbegin( ){ return( base::rbegin( ) ); }

	/// @brief X軸方向の末尾を指すコンスト型のコンテナを操作する逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator x_rbegin( ) const { return( base::rbegin( ) ); }


	/// @brief X軸方向の先頭を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator x_rend( ){ return( base::rend( ) ); }

	/// @brief X軸方向の先頭を指すコンスト型のコンテナを操作する逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator x_rend( ) const { return( base::rend( )  ); }


public: // 配列に対する操作

	/// @brief コンテナ内の要素をトリミングする
	//! 
	//! @param[in] index … トリミングの開始位置（ゼロから始まるインデックス）
	//! @param[in] num   … トリミング後の要素数（-1の場合は，最後までをコピーする）
	//! 
	//! @retval true  … トリミングに成功した場合（元とサイズが変わらない場合も含む）
	//! @retval false … 不正なトリミングを行おうとした場合
	//! 
	bool trim( size_type index, difference_type num = -1 )
	{
		double r = reso1( );
		if( base::trim( index, num ) )
		{
			reso( r );
			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! @attention どちらかのコンテナが外部メモリを利用している場合は，スワップは必ず失敗する
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	//! @retval true  … データのスワップに成功
	//! @retval false … データのスワップに失敗
	//! 
	bool swap( array1 &a )
	{
		if( base::swap( a ) )
		{
			double dmy_reso1_ = reso1_;
			reso1_ = a.reso1_;
			a.reso1_ = dmy_reso1_;
			return( true );
		}
		else
		{
			return( false );
		}
	}

public:
	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! コピー元とコピー先でデータ型（array< data > の data）が異なる場合の代入を行う．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class TT, class AAlocator >
	const array1& operator =( const array1< TT, AAlocator > &o )
	{
		base::operator =( o );
		reso1_ = o.reso1( );

		return( *this );
	}


	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	const array1& operator =( const array1 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		reso1_ = o.reso1_;

		return( *this );
	}

public:
	/// @brief ディフォルトコンストラクタ．要素数 0，解像度 1.0 のコンテナを作成する
	array1( ) : base( ), reso1_( 1.0 ) {}

	/// @brief アロケータ a のコピーを利用する
	explicit array1( const Allocator &a ) : base( a ), reso1_( 1.0 ) {}

	/// @brief 要素数 num 個のコンテナを作成し，デフォルト値で要素を初期化する
	explicit array1( size_type num ) : base( num ), reso1_( 1.0 ) {}


	/// @brief 要素数 num 個のコンテナを作成し，解像度を r1 に設定する
	array1( size_type num, double r1 ) : base( num ), reso1_( r1 ) {}

	/// @brief 要素数 num 個のコンテナを作成し，使用するアロケータを a に設定する
	array1( size_type num, const Allocator &a ) : base( num, a ), reso1_( 1.0 ) {}

	/// @brief 要素数 num 個のコンテナを作成し，解像度を r1，アロケータを a に設定する
	array1( size_type num, double r1, const Allocator &a ) : base( num, a ), reso1_( r1 ) {}


	/// @brief 要素数 num 個のコンテナを作成し，解像度を r1 に設定し，全要素を val で初期化する
	array1( size_type num, double r1, const value_type &val ) : base( num, val ), reso1_( r1 ) {}

	/// @brief 要素数 num 個のコンテナを作成し，解像度を r1 に設定し，全要素を val で初期化する
	array1( size_type num, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( 1.0 ) {}

	/// @brief 要素数 num 個のコンテナを作成し，解像度を r1，アロケータを a  に設定し，全要素を val で初期化する
	array1( size_type num, double r1, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( r1 ) {}


	/// @brief ptr が指すメモリ領域に，要素数 num 個のコンテナを作成する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array1( size_type num, pointer ptr, size_type mem_available ) : base( num, ptr, mem_available ), reso1_( 1.0 ) {}

	/// @brief ptr が指すメモリ領域に，要素数 num 個のコンテナを作成し，解像度を r1 に設定する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array1( size_type num, double r1, pointer ptr, size_type mem_available ) : base( num, ptr, mem_available ), reso1_( r1 ) {}

	/// @brief ptr が指すメモリ領域に，要素数 num 個のコンテナを作成し，解像度を r1，値 val で初期化する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array1( size_type num, double r1, const value_type &val, pointer ptr, size_type mem_available ) : base( num, val, ptr, mem_available ), reso1_( r1 ) {}

	/// @brief 他の array1 配列で要素の型が異なるものから同じ要素数の配列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	explicit array1( const array1< TT, AAlocator > &o ) : base( o ), reso1_( o.reso1( ) ) {}

	/// @brief 他の array1 配列で同じ要素型のものを用いて初期化する
	array1( const array1< T, Allocator > &o ) : base( o ), reso1_( o.reso1_ ) {}
};



/// @brief 要素数が可変の解像度付き2次元配列
//! 
//! 2次元画像を扱うための基本クラス
//! （@ref mist/mist.h をインクルードする）
//! 
//! @param T         … MISTのコンテナ内に格納するデータ型
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < class T, class Allocator = std::allocator< T > >
class array2 : public array1< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief データ型の変換を行う
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array2< TT, AAllocator > other;
	};


private:
	typedef array1< T, Allocator > base;	///< @brief 基底クラス
	size_type size2_;						///< @brief Y軸方向の要素数
	size_type size1_;						///< @brief X軸方向の要素数

protected:
	double reso2_;							///< @brief X軸方向の解像度

public:
	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 要素数を num1 × num2 個に変更し，要素数が変更された場合のみ全ての要素を値デフォルト値で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! @param[in] num1 … リサイズ後のX軸方向の要素数
	//! @param[in] num2 … リサイズ後のY軸方向の要素数
	//! @param[in] dmy1 … ダミー変数（使用しない）
	//! 
	//! @retval true  … 正常にリサイズが終了
	//! @retval false … リサイズ後のメモリを確保できなかった場合
	//! 
	bool resize( size_type num1, size_type num2, size_type dmy1 = 0 )
	{
		if( base::resize( num1 * num2 ) )
		{
			size1_ = num1;
			size2_ = num2;
			return( true );
		}
		else
		{
			size1_ = size2_ = 0;
			return( false );
		}
	}

	/// @brief コンテナ内の要素をトリミングする
	//! 
	//! @param[in] x … トリミングのX軸方向の開始位置（ゼロから始まるインデックス）
	//! @param[in] y … トリミングのY軸方向の開始位置（ゼロから始まるインデックス）
	//! @param[in] w … トリミング後のX軸方向の要素数（-1の場合は，最後までをコピーする）
	//! @param[in] h … トリミング後のY軸方向の要素数（-1の場合は，最後までをコピーする）
	//! 
	//! @retval true  … トリミングに成功した場合（元とサイズが変わらない場合も含む）
	//! @retval false … 不正なトリミングを行おうとした場合
	//! 
	bool trim( size_type x, size_type y, difference_type w = -1, difference_type h = -1 )
	{
		difference_type w_ = width( );
		difference_type h_ = height( );
		if( w_ <= static_cast< difference_type >( x ) || w_ < static_cast< difference_type >( x + w ) )
		{
			return( false );
		}
		else if( h_ <= static_cast< difference_type >( y ) || h_ < static_cast< difference_type >( y + h ) )
		{
			return( false );
		}
		else if( w_ == w && h_ == h )
		{
			return( true );
		}

		if( w < 0 )
		{
			w = w_ - x;
		}
		if( h < 0 )
		{
			h = h_ - y;
		}

		if( base::is_memory_shared( ) )
		{
			// 外部メモリを利用している場合
			array2 o( *this );
			if( resize( w, h ) )
			{
				const_pointer pi = o.paccess( x, y );
				pointer       po = paccess( 0, 0 );
				for( difference_type j = 0 ; j < h ; j++ )
				{
					po = allocator_.copy_objects( pi, w, po );
					pi += o.width( );
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			array2 o( w, h, base::reso1( ), reso2( ) );

			const_pointer pi = paccess( x, y );
			pointer       po = o.paccess( 0, 0 );
			for( difference_type j = 0 ; j < h ; j++ )
			{
				po = allocator_.copy_objects( pi, w, po );
				pi += this->width( );
			}

			swap( o );
		}

		return( true );
	}


	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! @attention どちらかのコンテナが外部メモリを利用している場合は，スワップは必ず失敗する
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	//! @retval true  … データのスワップに成功
	//! @retval false … データのスワップに失敗
	//! 
	bool swap( array2 &a )
	{
		if( base::swap( a ) )
		{
			double dmy_reso2_ = reso2_;
			reso2_ = a.reso2_;
			a.reso2_ = dmy_reso2_;

			size_type _dmy_size1 = size1_;
			size_type _dmy_size2 = size2_;
			size1_ = a.size1_;
			size2_ = a.size2_;
			a.size1_ = _dmy_size1;
			a.size2_ = _dmy_size2;
			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief コンテナの要素を空にする
	//! 
	//! コンテナに格納されているデータを全て削除し，コンテナを空（要素数0）にする
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}


	size_type size1( )  const { return( size1_ ); }			///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type size2( )  const { return( size2_ ); }			///< @brief Y軸方向のコンテナに格納されているデータ数を返す
	size_type width( )  const { return( size1_ ); }			///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type height( ) const { return( size2_ ); }			///< @brief Y軸方向のコンテナに格納されているデータ数を返す

	double reso2( double r2 ){ return( reso2_ = r2 ); }		///< @brief Y軸方向の解像度を r2 に設定し，設定後の値を返す
	double reso2( ) const { return( reso2_ ); }				///< @brief Y軸方向の解像度を返す

	/// @brief X軸とY軸方向の解像度を1度に設定する
	//!
	//! @param[in] r1  … 新しいX軸方向の解像度
	//! @param[in] r2  … 新しいY軸方向の解像度
	//! @param[in] dmy … 利用しない
	//!
	void reso( double r1, double r2, double dmy = 1.0 ){ base::reso1_ = r1; reso2_ = r2; }



/************************************************************************************************************
**
**      X軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief X軸方向を固定した時の，Y軸方向の先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code X軸方向を固定し，Y軸方向の先頭を指すランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.x_begin( 1 );
	//! for( ; ite1 != a.x_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0 ), width( ) ) ); }

	/// @brief X軸方向を固定した時の，Y軸方向の先頭を指すコンスト型のランダムアクセスイテレータを返す
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0 ), width( ) ) ); }

	/// @brief X軸方向を固定した時の，Y軸方向の末尾を指すランダムアクセスイテレータを返す
	iterator x_end( size_type i ){ return( iterator( paccess( i, height( ) ), width( ) ) ); }

	/// @brief X軸方向を固定した時の，Y軸方向の末尾を指すコンスト型のランダムアクセスイテレータを返す
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, height( ) ), width( ) ) ); }


	/// @brief X軸方向を固定した時の，Y軸方向の末尾を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code X軸方向を固定した時の，Y軸方向の逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.x_rbegin( 2 );
	//! for( ; ite2 != a.x_rend( 2 ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }

	/// @brief X軸方向を固定した時の，Y軸方向の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }


	/// @brief X軸方向を固定した時の，Y軸方向の末尾を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }

	/// @brief X軸方向を固定した時の，Y軸方向の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }

/************************************************************************************************************
**
**      Y軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief Y軸方向を固定した時の，X軸方向の先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code Y軸方向を固定した時の，X軸方向の先頭を指すランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_begin( 1 );
	//! for( ; ite1 != a.y_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j ), 1 ) ); }

	/// @brief Y軸方向を固定した時の，X軸方向の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j ), 1 ) ); }

	/// @brief Y軸方向を固定した時の，X軸方向の末尾を指すランダムアクセスイテレータを返す
	iterator y_end( size_type j ){ return( iterator( paccess( width( ), j ), 1 ) ); }

	/// @brief Y軸方向を固定した時の，X軸方向の末尾を指すコンスト型のランダムアクセスイテレータを返す
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( width( ), j ), 1 ) ); }


	/// @brief Y軸方向を固定した時の，X軸方向の先頭を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code Y軸方向を固定した時の，X軸方向の先頭を指す逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_rbegin( 1 );
	//! for( ; ite1 != a.y_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }

	/// @brief Y軸方向を固定した時の，X軸方向の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief Y軸方向を固定した時の，X軸方向の末尾を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }

	/// @brief Y軸方向を固定した時の，X軸方向の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


public:
	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! コピー元とコピー先でデータ型（array< data > の data）が異なる場合の代入を行う．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class TT, class AAlocator >
	const array2& operator =( const array2< TT, AAlocator > &o )
	{
		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
		}

		reso2_ = o.reso2( );

		return( *this );
	}


	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	const array2& operator =( const array2 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
		}

		reso2_ = o.reso2( );

		return( *this );
	}

// 要素へのアクセス
protected:
	/// @brief index で示される位置の要素のポインタを返す
	//! 
	//! @param[in] i … コンテナ内のX軸方向の位置
	//! @param[in] j … コンテナ内のY軸方向の位置
	//! 
	//! @return 指定された要素を示すポインタ
	//! 
	pointer paccess( size_type i, size_type j )
	{
		return( base::data_ + i + j * size1_ );
	}

	/// @brief index で示される位置の要素の const ポインタを返す
	//! 
	//! @param[in] i … コンテナ内のX軸方向の位置
	//! @param[in] j … コンテナ内のY軸方向の位置
	//! 
	//! @return 指定された要素を示すポインタ
	//! 
	const_pointer paccess( size_type i, size_type j ) const
	{
		return( base::data_ + i + j * size1_ );
	}

public:
	/// @brief ( i, j ) で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i   … コンテナ内のX軸方向の位置
	/// @param[in] j   … コンテナ内のY軸方向の位置
	/// @param[in] dmy … 使用しない
	//!
	/// @return 指定された要素を示す参照
	//!
	reference at( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief ( i, j ) で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i   … コンテナ内のX軸方向の位置
	/// @param[in] j   … コンテナ内のY軸方向の位置
	/// @param[in] dmy … 使用しない
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference at( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief ( i, j ) で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i   … コンテナ内のX軸方向の位置
	/// @param[in] j   … コンテナ内のY軸方向の位置
	/// @param[in] dmy … 使用しない
	//!
	/// @return 指定された要素を示す参照
	//!
	reference operator ()( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief ( i, j ) で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i   … コンテナ内のX軸方向の位置
	/// @param[in] j   … コンテナ内のY軸方向の位置
	/// @param[in] dmy … 使用しない
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference operator ()( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


public:
	/// @brief ディフォルトコンストラクタ．要素数 0，解像度 1.0 × 1.0 のコンテナを作成する
	array2( ) : base( ), size2_( 0 ), size1_( 0 ), reso2_( 1.0 ) {}

	/// @brief アロケータ a のコピーを利用する
	explicit array2( const Allocator &a ) : base( a ), size2_( 0 ), size1_( 0 ), reso2_( 1.0 ) {}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，デフォルト値で要素を初期化する
	array2( size_type num1, size_type num2 ) : base( num1 * num2 ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief 要素数 num1 × num2 個のコンテナを作成し，解像度を r1 × r2 に設定する
	array2( size_type num1, size_type num2, double r1, double r2 ) : base( num1 * num2, r1 ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，使用するアロケータを a に設定する
	array2( size_type num1, size_type num2, const Allocator &a ) : base( num1 * num2, a ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，解像度を r1 × r2 に設定し，使用するアロケータを a に設定する
	array2( size_type num1, size_type num2, double r1, double r2, const Allocator &a ) : base( num1 * num2, r1, a ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief 要素数 num1 × num2 個のコンテナを作成し，解像度を 1.0 × 1.0 に設定し，全要素を val で初期化する
	array2( size_type num1, size_type num2, const value_type &val ) : base( num1 * num2, val ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，解像度を r1 × r2 に設定し，全要素を val で初期化する
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val ) : base( num1 * num2, r1, val ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，解像度を r1 × r2 に設定し，使用するアロケータを a に設定する
	array2( size_type num1, size_type num2, const value_type &val, const Allocator &a ) : base( num1 * num2, val, a ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，解像度を r1 × r2，使用するアロケータを a に設定し，全要素を val で初期化する
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val, const Allocator &a ) : base( num1 * num2, r1, val, a ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 個のコンテナを作成する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array2( size_type num1, size_type num2, pointer ptr, size_type mem_available ) : base( num1 * num2, ptr, mem_available ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 個のコンテナを作成し，解像度を r1 × r2 に設定する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array2( size_type num1, size_type num2, double r1, double r2, pointer ptr, size_type mem_available ) : base( num1 * num2, r1, ptr, mem_available ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 個のコンテナを作成し，解像度を r1 × r2，値 val で初期化する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val, pointer ptr, size_type mem_available ) : base( num1 * num2, r1, val, ptr, mem_available ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}



	/// @brief 他の array2 配列で要素の型が異なるものから同じ要素数の配列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	array2( const array2< TT, AAlocator > &o ) : base( o ), size2_( o.size2( ) ), size1_( o.size1( ) ), reso2_( o.reso2( ) )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief 他の array2 配列で同じ要素型のものを用いて初期化する
	array2( const array2< T, Allocator > &o ) : base( o ), size2_( o.size2_ ), size1_( o.size1_ ), reso2_( o.reso2_ )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}
};





/// @brief 要素数が可変の解像度付き3次元配列
//! 
//! 3次元画像を扱うための基本クラス
//! （@ref mist/mist.h をインクルードする）
//! 
//! @param T         … MISTのコンテナ内に格納するデータ型
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < class T, class Allocator = std::allocator< T > >
class array3 : public array2< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator2< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator2< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief データ型の変換を行う
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array3< TT, AAllocator > other;
	};


private:
	typedef array2< T, Allocator > base;	///< @brief 基底クラス
	size_type size3_;						///< @brief Z軸方向の要素数
	size_type size2_;						///< @brief Y軸方向の要素数
	size_type size1_;						///< @brief X軸方向の要素数

protected:
	double reso3_;							///< @brief Z軸方向の解像度

public:
	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 要素数を num1 × num2 × num3 個に変更し，要素数が変更された場合のみ全ての要素をデフォルト値で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! 
	//! @param[in] num1 … リサイズ後のX軸方向の要素数
	//! @param[in] num2 … リサイズ後のY軸方向の要素数
	//! @param[in] num3 … リサイズ後のZ軸方向の要素数
	//! 
	//! @retval true  … 正常にリサイズが終了
	//! @retval false … リサイズ後のメモリを確保できなかった場合
	//! 
	bool resize( size_type num1, size_type num2, size_type num3 )
	{
		if( base::resize( num1 * num2, num3 ) )
		{
			size1_ = num1;
			size2_ = num2;
			size3_ = num3;
			return( true );
		}
		else
		{
			size1_ = size2_ = size3_ = 0;
			return( false );
		}
	}

	/// @brief コンテナ内の要素をトリミングする
	//! 
	//! @param[in] x … トリミングのX軸方向の開始位置（ゼロから始まるインデックス）
	//! @param[in] y … トリミングのY軸方向の開始位置（ゼロから始まるインデックス）
	//! @param[in] z … トリミングのZ軸方向の開始位置（ゼロから始まるインデックス）
	//! @param[in] w … トリミング後のX軸方向の要素数（-1の場合は，最後までをコピーする）
	//! @param[in] h … トリミング後のY軸方向の要素数（-1の場合は，最後までをコピーする）
	//! @param[in] d … トリミング後のZ軸方向の要素数（-1の場合は，最後までをコピーする）
	//! 
	//! @retval true  … トリミングに成功した場合（元とサイズが変わらない場合も含む）
	//! @retval false … 不正なトリミングを行おうとした場合
	//! 
	bool trim( size_type x, size_type y, size_type z, difference_type w = -1, difference_type h = -1, difference_type d = -1 )
	{
		difference_type w_ = width( );
		difference_type h_ = height( );
		difference_type d_ = depth( );
		if( w_ <= static_cast< difference_type >( x ) || w_ < static_cast< difference_type >( x + w ) )
		{
			return( false );
		}
		else if( h_ <= static_cast< difference_type >( y ) || h_ < static_cast< difference_type >( y + h ) )
		{
			return( false );
		}
		else if( d_ <= static_cast< difference_type >( z ) || d_ < static_cast< difference_type >( z + d ) )
		{
			return( false );
		}
		else if( w_ == w && h_ == h && d_ == d )
		{
			return( true );
		}

		if( w < 0 )
		{
			w = w_ - x;
		}
		if( h < 0 )
		{
			h = h_ - y;
		}
		if( d < 0 )
		{
			d = d_ - z;
		}

		if( base::is_memory_shared( ) )
		{
			// 外部メモリを利用している場合
			array3 o( *this );
			if( resize( w, h, d ) )
			{
				const_pointer pi = o.paccess( x, y, z );
				pointer       po = paccess( 0, 0, 0 );
				size_type     s1 = o.paccess( x, y + 1, z ) - pi;
				size_type     s2 = o.paccess( x, y, z + 1 ) - o.paccess( x, y + h, z );

				for( difference_type k = 0 ; k < d ; k++ )
				{
					for( difference_type j = 0 ; j < h ; j++ )
					{
						po = allocator_.copy_objects( pi, w, po );
						pi += s1;
					}

					pi += s2;
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			array3 o( w, h, d, base::reso1( ), base::reso2( ), reso3( ) );

			const_pointer pi = paccess( x, y, z );
			pointer       po = o.paccess( 0, 0, 0 );
			size_type     s1 = paccess( x, y + 1, z ) - pi;
			size_type     s2 = paccess( x, y, z + 1 ) - paccess( x, y + h, z );

			for( difference_type k = 0 ; k < d ; k++ )
			{
				for( difference_type j = 0 ; j < h ; j++ )
				{
					po = allocator_.copy_objects( pi, w, po );
					pi += s1;
				}

				pi += s2;
			}

			swap( o );
		}

		return( true );
	}


	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! @attention どちらかのコンテナが外部メモリを利用している場合は，スワップは必ず失敗する
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	//! @retval true  … データのスワップに成功
	//! @retval false … データのスワップに失敗
	//! 
	bool swap( array3 &a )
	{
		if( base::swap( a ) )
		{
			double dmy_reso3_ = reso3_;
			reso3_ = a.reso3_;
			a.reso3_ = dmy_reso3_;

			size_type _dmy_size1 = size1_;
			size_type _dmy_size2 = size2_;
			size_type _dmy_size3 = size3_;
			size1_ = a.size1_;
			size2_ = a.size2_;
			size3_ = a.size3_;
			a.size1_ = _dmy_size1;
			a.size2_ = _dmy_size2;
			a.size3_ = _dmy_size3;

			return( true );
		}
		else
		{
			return( true );
		}
	}


	/// @brief コンテナの要素を空にする
	//! 
	//! コンテナに格納されているデータを全て削除し，コンテナを空（要素数0）にする
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}


	size_type size1( ) const { return( size1_ ); }			///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type size2( ) const { return( size2_ ); }			///< @brief Y軸方向のコンテナに格納されているデータ数を返す
	size_type size3( ) const { return( size3_ ); }			///< @brief Z軸方向のコンテナに格納されているデータ数を返す
	size_type width( ) const { return( size1_ ); }			///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type height( ) const { return( size2_ ); }			///< @brief Y軸方向のコンテナに格納されているデータ数を返す
	size_type depth( ) const { return( size3_ ); }			///< @brief Z軸方向のコンテナに格納されているデータ数を返す

	double reso3( double r3 ){ return( reso3_ = r3 ); }		///< @brief Z軸方向の解像度を r3 に設定し，設定後の値を返す
	double reso3( ) const { return( reso3_ ); }				///< @brief Z軸方向の解像度を返す

	/// @brief X軸とY軸とZ軸方向の解像度を1度に設定する
	//!
	//! @param[in] r1 … 新しいX軸方向の解像度
	//! @param[in] r2 … 新しいY軸方向の解像度
	//! @param[in] r3 … 新しいY軸方向の解像度
	//!
	void reso( double r1, double r2, double r3 ){ base::reso1_ = r1; base::reso2_ = r2; reso3_ = r3; }



	/// @brief コンテナの先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code 順方向ランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.begin( );
	//! for( ; ite1 != a.end( ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator begin( ){ return( iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }

	/// @brief コンテナの先頭を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator begin( ) const { return( const_iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }

	/// @brief コンテナの末尾を指すランダムアクセスイテレータを返す
	iterator end( ){ return( iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }

	/// @brief コンテナの末尾を指すコンスト型のコンテナを操作するランダムアクセスイテレータを返す
	const_iterator end( ) const { return( const_iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }


	/// @brief コンテナの末尾を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code 逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.rbegin( );
	//! for( ; ite2 != a.rend( ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }

	/// @brief コンテナの末尾を指すコンスト型のコンテナを操作する逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }

	/// @brief コンテナの先頭を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }

	/// @brief 使用しているアロケータが確保可能なメモリの最大値を返す
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }

/************************************************************************************************************
**
**      X軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code X軸方向を固定し，YZ平面(Sagittal)の先頭を指すランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.x_begin( 1 );
	//! for( ; ite1 != a.x_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }

	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の先頭を指すコンスト型のランダムアクセスイテレータを返す
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }

	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の末尾を指すランダムアクセスイテレータを返す
	iterator x_end( size_type i ){ return( iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }

	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の末尾を指すコンスト型のランダムアクセスイテレータを返す
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }



	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の末尾を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code X軸方向を固定した時の，YZ平面(Sagittal)の逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.x_rbegin( 2 );
	//! for( ; ite2 != a.x_rend( 2 ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }

	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の末尾を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }

	/// @brief X軸方向を固定した時の，YZ平面(Sagittal)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }


/************************************************************************************************************
**
**      Y軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code Y軸方向を固定した時の，XZ平面の先頭を指すランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_begin( 1 );
	//! for( ; ite1 != a.y_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }

	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }

	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の末尾を指すランダムアクセスイテレータを返す
	iterator y_end( size_type j ){ return( iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }

	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の末尾を指すコンスト型のランダムアクセスイテレータを返す
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }


	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の先頭を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code Y軸方向を固定した時の，XZ平面(Coronal)の先頭を指す逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_rbegin( 1 );
	//! for( ; ite1 != a.y_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }

	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の末尾を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }

	/// @brief Y軸方向を固定した時の，XZ平面(Coronal)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


/************************************************************************************************************
**
**      Z軸を固定した場合の順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief Z軸方向を固定した時の，XY平面(Axial)の先頭を指すランダムアクセスイテレータを返す
	//!
	//! @code Z軸方向を固定した時の，XY平面の先頭を指すランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.z_begin( 1 );
	//! for( ; ite1 != a.z_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator z_begin( size_type k ){ return( iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }

	/// @brief Z軸方向を固定した時の，XY平面(Axial)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_iterator z_begin( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }

	/// @brief Z軸方向を固定した時の，XY平面(Axial)の末尾を指すランダムアクセスイテレータを返す
	iterator z_end( size_type k ){ return( iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }

	/// @brief Z軸方向を固定した時の，XY平面(Axial)の末尾を指すコンスト型のランダムアクセスイテレータを返す
	const_iterator z_end( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }


	/// @brief Z軸方向を固定した時の，XY平面(Axial)の先頭を指す逆方向ランダムアクセスイテレータを返す
	//!
	//! @code Z軸方向を固定した時の，XY平面(Axial)の先頭を指す逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.z_rbegin( 1 );
	//! for( ; ite1 != a.z_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator z_rbegin( size_type k ){ return( reverse_iterator( z_end( k ) ) ); }

	/// @brief Z軸方向を固定した時の，XY平面(Axial)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator z_rbegin( size_type k ) const { return( const_reverse_iterator( z_end( k )) ); }

	/// @brief Z軸方向を固定した時の，XY平面(Axial)の末尾を指す逆方向ランダムアクセスイテレータを返す
	reverse_iterator z_rend( size_type k ){ return( reverse_iterator( z_begin( k ) ) ); }

	/// @brief Z軸方向を固定した時の，XY平面(Axial)の末尾を指すコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator z_rend( size_type k ) const { return( const_reverse_iterator( z_begin( k ) ) ); }


public:
	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! コピー元とコピー先でデータ型（array3< data > の data）が異なる場合の代入を行う．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class TT, class AAlocator >
	const array3& operator =( const array3< TT, AAlocator > &o )
	{
		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = size3_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
			size3_ = o.size3( );
		}

		reso3_ = o.reso3( );

		return( *this );
	}


	/// @brief 要素の型が異なるコンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	const array3& operator =( const array3 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = size3_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
			size3_ = o.size3( );
		}

		reso3_ = o.reso3( );

		return( *this );
	}


// 要素へのアクセス
protected:
	/// @brief index で示される位置の要素のポインタを返す
	//! 
	//! @param[in] i … コンテナ内のX軸方向の位置
	//! @param[in] j … コンテナ内のY軸方向の位置
	//! @param[in] k … コンテナ内のZ軸方向の位置
	//! 
	//! @return 指定された要素を示すポインタ
	//! 
	pointer paccess( size_type i, size_type j, size_type k )
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}

	/// @brief index で示される位置の要素の const ポインタを返す
	//! 
	//! @param[in] i … コンテナ内のX軸方向の位置
	//! @param[in] j … コンテナ内のY軸方向の位置
	//! @param[in] k … コンテナ内のZ軸方向の位置
	//! 
	//! @return 指定された要素を示すポインタ
	//! 
	const_pointer paccess( size_type i, size_type j, size_type k ) const
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}

public:
	/// @brief ( i, j, k ) で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j, k ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//! @param[in] k … コンテナ内のZ軸方向の位置
	//!
	/// @return 指定された要素を示す参照
	//!
	reference at( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief ( i, j, k ) で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j, k ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//! @param[in] k … コンテナ内のZ軸方向の位置
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference at( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief ( i, j, k ) で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j, k ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//! @param[in] k … コンテナ内のZ軸方向の位置
	//!
	/// @return 指定された要素を示す参照
	//!
	reference operator ()( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief ( i, j, k ) で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された ( i, j, k ) が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//! @param[in] k … コンテナ内のZ軸方向の位置
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference operator ()( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


public:
	/// @brief ディフォルトコンストラクタ．要素数 0，解像度 1.0 × 1.0 × 1.0 のコンテナを作成する
	array3( ) : base( ), size3_( 0 ), size2_( 0 ), size1_( 0 ), reso3_( 1.0 ) {}

	/// @brief アロケータ a のコピーを利用する
	explicit array3( const Allocator &a ) : base( a ), size3_( 0 ), size2_( 0 ), size1_( 0 ), reso3_( 1.0 ) {}

	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，デフォルト値で要素を初期化する
	array3( size_type num1, size_type num2, size_type num3 ) : base( num1 * num2, num3 ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を r1 × r2 × r3 に設定する
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3 ) : base( num1 * num2, num3, r1, r2 ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，使用するアロケータを a に設定する
	array3( size_type num1, size_type num2, size_type num3, const Allocator &a ) : base( num1 * num2, num3, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を r1 × r2 × r3 に設定し，使用するアロケータを a に設定する
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const Allocator &a ) : base( num1 * num2, num3, r1, r2, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}


	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を 1.0 × 1.0 × 1.0 に設定し，全要素を val で初期化する
	array3( size_type num1, size_type num2, size_type num3, const value_type &val ) : base( num1 * num2, num3, val ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を r1 × r2 × r3 に設定し，全要素を val で初期化する
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val ) : base( num1 * num2, num3, r1, r2, val ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を r1 × r2 × r3 に設定し，使用するアロケータを a に設定する
	array3( size_type num1, size_type num2, size_type num3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, val, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief 要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を r1 × r2 × r3，使用するアロケータを a に設定し，全要素を val で初期化する
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, r1, r2, val, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}



	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 × num3 個のコンテナを作成する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array3( size_type num1, size_type num2, size_type num3, pointer ptr, size_type mem_available ) : base( num1 * num2, num3, ptr, mem_available ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を r1 × r2 × r3 に設定する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, pointer ptr, size_type mem_available ) : base( num1 * num2, num3, r1, r2, ptr, mem_available ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 × num3 個のコンテナを作成し，解像度を r1 × r2 × r3，値 val で初期化する（ptr が指す先の利用可能なメモリ量は mem_available ）
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val, pointer ptr, size_type mem_available ) : base( num1 * num2, num3, r1, r2, val, ptr, mem_available ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}



	/// @brief 他の array3 配列で要素の型が異なるものから同じ要素数の配列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	array3( const array3< TT, AAlocator > &o ) : base( o ), size3_( o.size3( ) ), size2_( o.size2( ) ), size1_( o.size1( ) ), reso3_( o.reso3( ) )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief 他の array3 配列で同じ要素型のものを用いて初期化する
	array3( const array3< T, Allocator > &o ) : base( o ), size3_( o.size3_ ), size2_( o.size2_ ), size1_( o.size1_ ), reso3_( o.reso3_ )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}
};




/// @brief 画像のふちにマージンを持った配列
//! 
//! 1・2・3次元画像にマージンを持たせるための基本クラス
//! （@ref mist/mist.h をインクルードする）
//! 
//! @param Array … 1・2・3次元画像クラスを指定する
//! 
template < class Array >
class marray : public Array
{
public:
	typedef typename Array::allocator_type allocator_type;		///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Array::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Array::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Array::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Array::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Array::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Array::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Array::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	typedef typename Array::iterator iterator;								///< @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename Array::const_iterator const_iterator;					///< @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename Array::reverse_iterator reverse_iterator;				///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename Array::const_reverse_iterator const_reverse_iterator;	///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版


protected:
	typedef Array base;						///< @brief 基底クラス
	size_type margin1_;						///< @brief X軸方向のマージン
	size_type margin2_;						///< @brief Y軸方向のマージン
	size_type margin3_;						///< @brief Z軸方向のマージン

public:
	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 要素数を num1 個に変更し，全ての要素をデフォルト値で初期化する．
	//! 
	//! @param[in] num1 … リサイズ後の全要素数
	//! 
	//! @retval true  … 正常にリサイズが終了
	//! @retval false … リサイズ後のメモリを確保できなかった場合
	//! 
	bool resize( size_type num1 )
	{
		return( base::resize( num1 + margin1_ * 2 ) );
	}


	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 要素数を num1 × num2 個に変更し，要素数が変更された場合のみ全ての要素をデフォルト値で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! 
	//! @param[in] num1 … リサイズ後のX軸方向の要素数
	//! @param[in] num2 … リサイズ後のY軸方向の要素数
	//! 
	//! @retval true  … 正常にリサイズが終了
	//! @retval false … リサイズ後のメモリを確保できなかった場合
	//! 
	bool resize( size_type num1, size_type num2 )
	{
		return( base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2 ) );
	}


	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 要素数を num1 × num2 × num3 個に変更し，要素数が変更された場合のみ全ての要素をデフォルト値で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! 
	//! @param[in] num1 … リサイズ後のX軸方向の要素数
	//! @param[in] num2 … リサイズ後のY軸方向の要素数
	//! @param[in] num3 … リサイズ後のZ軸方向の要素数
	//! 
	//! @retval true  … 正常にリサイズが終了
	//! @retval false … リサイズ後のメモリを確保できなかった場合
	//! 
	bool resize( size_type num1, size_type num2, size_type num3 )
	{
		return( base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, num3 + margin3_ * 2 ) );
	}



	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! @attention どちらかのコンテナが外部メモリを利用している場合は，スワップは必ず失敗する
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	//! @retval true  … データのスワップに成功
	//! @retval false … データのスワップに失敗
	//! 
	bool swap( marray &a )
	{
		if( base::swap( a ) )
		{
			size_type tmp = margin1_;
			margin1_ = a.margin1_;
			a.margin1_ = tmp;

			tmp = margin2_;
			margin2_ = a.margin2_;
			a.margin2_ = tmp;

			tmp = margin3_;
			margin3_ = a.margin3_;
			a.margin3_ = tmp;

			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief コンテナの要素を空にする
	//! 
	//! コンテナに格納されているデータを全て削除し，コンテナを空（要素数0）にする
	//! 
	void clear( )
	{
		base::clear( );
		margin1_ = margin2_ = margin3_ = 0;
	}


	/// @brief コンテナ内のマージン部分のデータ要素を指定された値で初期化する
	//! 
	//! マージン部分の要素を値 val で初期化する．
	//! 
	//! @param[in] val … 要素を初期化する値
	//! 
	void fill_margin( const value_type &val = value_type( ) )
	{
		if( !base::empty( ) )
		{
			base &o = *this;

			size_type i, j, k;
			for( k = 0 ; k < margin3( ) ; k++ )
			{
				for( j = 0 ; j < o.size2( ) ; j++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
			for( k = o.size3( ) - margin3( ) ; k < o.size3( ) ; k++ )
			{
				for( j = 0 ; j < o.size2( ) ; j++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}

			for( j = 0 ; j < margin2( ) ; j++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
			for( j = o.size2( ) - margin2( ) ; j < o.size2( ) ; j++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}

			for( i = 0 ; i < margin1( ) ; i++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( j = 0 ; j < o.size2( ) ; j++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
			for( i = o.size1( ) - margin1( ) ; i < o.size1( ) ; i++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( j = 0 ; j < o.size2( ) ; j++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
		}
	}


	size_type size1( ) const { return( base::empty( ) ? 0 : base::size1( ) - 2 * margin1_ ); }	///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type size2( ) const { return( base::empty( ) ? 0 : base::size2( ) - 2 * margin2_ ); }	///< @brief Y軸方向のコンテナに格納されているデータ数を返す
	size_type size3( ) const { return( base::empty( ) ? 0 : base::size3( ) - 2 * margin3_ ); }	///< @brief Z軸方向のコンテナに格納されているデータ数を返す

	size_type width( ) const { return( size1( ) ); }		///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type height( ) const { return( size2( ) ); }		///< @brief Y軸方向のコンテナに格納されているデータ数を返す
	size_type depth( ) const { return( size3( ) ); }		///< @brief Z軸方向のコンテナに格納されているデータ数を返す

	size_type margin1( ) const { return( margin1_ ); }		///< @brief X軸方向のマージン幅を返す
	size_type margin2( ) const { return( margin2_ ); }		///< @brief Y軸方向のマージン幅を返す
	size_type margin3( ) const { return( margin3_ ); }		///< @brief Z軸方向のマージン幅を返す

private:
	/// @brief array コンテナからデータをコピーする
	//! 
	//! @attention マージン部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& copy( const array< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array1 コンテナからデータをコピーする
	//! 
	//! @attention マージン部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& copy( const array1< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array2 コンテナからデータをコピーする
	//! 
	//! @attention マージン部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& copy( const array2< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		for( size_type j = 0 ; j < h ; j++ )
		{
			for( size_type i = 0 ; i < w ; i++ )
			{
				( *this )( i, j ) = static_cast< value_type >( o( i, j ) );
			}
		}
		return( *this );
	}


	/// @brief array3 コンテナからデータをコピーする
	//! 
	//! @attention マージン部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& copy( const array3< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		size_type d = depth( )  < o.depth( )  ? depth( )  : o.depth( );
		for( size_type k = 0 ; k < d ; k++ )
		{
			for( size_type j = 0 ; j < h ; j++ )
			{
				for( size_type i = 0 ; i < w ; i++ )
				{
					( *this )( i, j, k ) = static_cast< value_type >( o( i, j, k ) );
				}
			}
		}
		return( *this );
	}

public:
	/// @brief 同じ型の marray コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の marray コンテナ
	//! 
	//! @return 自分自身
	//! 
	const marray& operator =( const marray &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		margin1_ = o.margin1_;
		margin2_ = o.margin2_;
		margin3_ = o.margin3_;

		return( *this );
	}


	/// @brief 要素の型が異なる array コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array< T, Allocator > &o )
	{
		if( base::resize( o.size( ) + margin1_ * 2 ) )
		{
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief 要素の型が異なる array1 コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array1 コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array1< T, Allocator > &o )
	{
		if( base::resize( o.size( ) + margin1_ * 2 ) )
		{
			reso1( o.reso1( ) );
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief 要素の型が異なる array2 コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array2 コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array2< T, Allocator > &o )
	{
		if( base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2 ) )
		{
			reso1( o.reso1( ) );
			reso2( o.reso2( ) );
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief 要素の型が異なる array3 コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array3 コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array3< T, Allocator > &o )
	{
		if( base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2, o.size3( ) + margin3_ * 2 ) )
		{
			reso1( o.reso1( ) );
			reso2( o.reso2( ) );
			reso3( o.reso3( ) );
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}

// 要素へのアクセス
public:
	/// @brief index で示される位置の要素の参照を返す
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す参照
	//!
	reference at( difference_type index )
	{
		return( base::at( index + margin1_ ) );
	}

	/// @brief ( i, j ) で示される位置の要素の参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す参照
	//!
	reference at( difference_type i, difference_type j )
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) で示される位置の要素の参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	/// @param[in] k … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す参照
	//!
	reference at( difference_type i, difference_type j, difference_type k )
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief index で示される位置の要素の const 参照を返す
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す const 参照
	//!
	const_reference at( difference_type index ) const
	{
		return( base::at( index + margin1_ ) );
	}

	/// @brief ( i, j ) で示される位置の要素の const 参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference at( difference_type i, difference_type j ) const
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) で示される位置の要素の const 参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	/// @param[in] k … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference at( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す参照
	//!
	reference operator []( difference_type index )
	{
		return( base::operator []( index ) );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す参照
	//!
	reference operator ()( difference_type index )
	{
		return( base::operator ()( index + margin1_ ) );
	}

	/// @brief ( i, j ) で示される位置の要素の参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す参照
	//!
	reference operator ()( difference_type i, difference_type j )
	{
		return( base::operator ()( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) で示される位置の要素の参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	/// @param[in] k … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す参照
	//!
	reference operator ()( difference_type i, difference_type j, difference_type k )
	{
		return( base::operator ()( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す参照
	//!
	const_reference operator []( difference_type index ) const
	{
		return( base::operator []( index ) );
	}

	/// @brief index で示される位置の要素の const 参照を返す
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す const 参照
	//!
	const_reference operator ()( difference_type index ) const
	{
		return( base::operator ()( index + margin1_ ) );
	}

	/// @brief ( i, j ) で示される位置の要素の const 参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference operator ()( difference_type i, difference_type j ) const
	{
		return( base::operator ()( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) で示される位置の要素の const 参照を返す
	//!
	/// @param[in] i … コンテナ内のX軸方向の位置
	/// @param[in] j … コンテナ内のY軸方向の位置
	/// @param[in] k … コンテナ内のY軸方向の位置
	//!
	/// @return 指定された要素を示す const 参照
	//!
	const_reference operator ()( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::operator ()( i + margin1_, j + margin2_, k + margin3_ ) );
	}


public:
	/// @brief ディフォルトコンストラクタ．要素数 0，マージン 0 のコンテナを作成する
	marray( ) : base( ), margin1_( 0 ), margin2_( 0 ), margin3_( 0 ) {}

	/// @brief マージン margin のコンテナを作成する
	marray( size_type margin ) : base( ), margin1_( margin ), margin2_( margin ), margin3_( margin ) {}

	/// @brief コピーコンストラクタ
	marray( const marray &o ) : base( o ), margin1_( o.margin1( ) ), margin2_( o.margin2( ) ), margin3_( o.margin3( ) ) {}



	/// @brief マージン margin のコンテナを作成する
	marray( size_type w, size_type margin ) : base( w + margin * 2 ), margin1_( margin ), margin2_( 0 ), margin3_( 0 ) {}

	/// @brief マージン margin のコンテナを作成する
	marray( size_type w, size_type h, size_type margin ) : base( w + margin * 2, h + margin * 2 ), margin1_( margin ), margin2_( margin ), margin3_( 0 ) {}

	/// @brief マージン margin のコンテナを作成する
	marray( size_type w, size_type h, size_type d, size_type margin ) : base( w + margin * 2, h + margin * 2, d + margin * 2 ), margin1_( margin ), margin2_( margin ), margin3_( margin ) {}




	/// @brief array 配列 o の配列の大きさと，X軸方向のマージン margin1 を用いて初期化し，全要素を val で初期化する
	template < class T, class Allocator >
	marray( const array< T, Allocator > &o, size_type margin1, const value_type &val = value_type( 0 ) )
		: base( o.size( ) + margin1 * 2 ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array1 配列 o の配列の大きさと，X軸方向のマージン margin1 を用いて初期化し，全要素を val で初期化する
	template < class T, class Allocator >
	marray( const array1< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size( ) + margin1 * 2, o.reso1( ) ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array2 配列 o の配列の大きさと，X軸方向のマージン margin1，Y軸方向のマージン margin2 を用いて初期化し，全要素を val で初期化する
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, size_type margin2, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief array2 配列 o の配列の大きさと，X軸方向のマージン margin1，Y軸方向のマージン margin2 を用いて初期化し，全要素を val で初期化する
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, size_type margin2, size_type margin3, const value_type &val )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array3 配列 o の配列の大きさと，X軸方向のマージン margin1，Y軸方向のマージン margin2，Z軸方向のマージン margin3 を用いて初期化し，全要素を val で初期化する
	//! 
	template < class T, class Allocator >
	marray( const array3< T, Allocator > &o, size_type margin1, size_type margin2, size_type margin3, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.size3( ) + margin3 * 2, o.reso1( ), o.reso2( ), o.reso3( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( margin3 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array2 配列 o の配列の大きさと，X・Y軸方向のマージン margin1 を用いて初期化し，全要素を val で初期化する
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin1 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin1 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief array3 配列 o の配列の大きさと，X・Y・Z軸方向のマージン margin1 を用いて初期化し，全要素を val で初期化する
	template < class T, class Allocator >
	marray( const array3< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin1 * 2, o.size3( ) + margin1 * 2, o.reso1( ), o.reso2( ), o.reso3( ) ), margin1_( margin1 ), margin2_( margin1 ), margin3_( margin1 )
	{
		fill_margin( val );
		copy( o );
	}
};





/// @brief １辺が2の指数乗のバッファを持った画像を作成する
//! 
//! 1・2・3次元画像の１辺が2の指数乗にするための基本クラス
//! 
//! @param Array … 1・2・3次元画像クラスを指定する
//! 
template < class Array >
class buffered_array : public Array
{
public:
	typedef typename Array::allocator_type allocator_type;		///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Array::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Array::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Array::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Array::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Array::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Array::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Array::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	typedef typename Array::iterator iterator;								///< @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename Array::const_iterator const_iterator;					///< @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename Array::reverse_iterator reverse_iterator;				///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename Array::const_reverse_iterator const_reverse_iterator;	///< @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版

protected:
	typedef Array base;						///< @brief 基底クラス
	size_type size1_;						///< @brief X軸方向の要素数
	size_type size2_;						///< @brief Y軸方向の要素数
	size_type size3_;						///< @brief Z軸方向の要素数

	/// @brief 指定された数値以上の正整数で最も最小の2の指数乗を計算し，その指数部を返す
	//! 
	//! @param[in] v … 任意の非負の整数
	//!
	/// @return 入力された正整数を超える最も最小の2の指数乗の指数部
	//! 
	inline size_t floor_square_index( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( i );
			}
			_2 *= 2;
		}

		return( 0 );
	}

	/// @brief 指定された数値以上の正整数で最も最小の2の指数乗を計算する
	//! 
	//! @param[in] v … 任意の非負の整数
	//!
	/// @return 入力された正整数を超える最も最小の2の指数乗を表す整数
	//! 
	inline size_t floor_square( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( _2 );
			}
			_2 *= 2;
		}

		return( 0 );
	}

public:
	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 各軸の要素数が指定された整数以上で最小の2の指数乗に変更し，要素数が変更された場合のみ全ての要素をデフォルト値で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! 
	//! @param[in] num1 … リサイズ後のX軸方向の要素数
	//! 
	bool resize( size_type num1 )
	{
		if( base::resize( floor_square( num1 ) ) )
		{
			size1_ = num1;
			return( true );
		}
		else
		{
			size1_ = 0;
			return( false );
		}
	}


	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 各軸の要素数が指定された整数以上で最小の2の指数乗に変更し，要素数が変更された場合のみ全ての要素をデフォルト値で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! 
	//! @param[in] num1 … リサイズ後のX軸方向の要素数
	//! @param[in] num2 … リサイズ後のY軸方向の要素数
	//! 
	bool resize( size_type num1, size_type num2 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size1_ = num1;
		size2_ = num2;
		s1 = s1 > s2 ? s1 : s2;
		if( !base::resize( s1, s1 ) )
		{
			size1_ = size2_ = 0;
			return( false );
		}

		return( true );
	}


	/// @brief コンテナ内の要素数を変更する
	//! 
	//! 各軸の要素数が指定された整数以上で最小の2の指数乗に変更し，要素数が変更された場合のみ全ての要素をデフォルト値で初期化する．
	//! 
	//! @attention 必ず要素を初期化するためには，fill 関数を利用してください．
	//! 
	//! 
	//! @param[in] num1 … リサイズ後のX軸方向の要素数
	//! @param[in] num2 … リサイズ後のY軸方向の要素数
	//! @param[in] num3 … リサイズ後のZ軸方向の要素数
	//! 
	bool resize( size_type num1, size_type num2, size_type num3 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size_type s3 = floor_square( num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
		s1 = s1 > s2 ? s1 : s2;
		s1 = s1 > s3 ? s1 : s3;

		if( !base::resize( s1, s1, s1 ) )
		{
			size1_ = size2_ = size3_ = 0;
			return( false );
		}

		return( true );
	}


	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! @attention どちらかのコンテナが外部メモリを利用している場合は，スワップは必ず失敗する
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	//! @retval true  … データのスワップに成功
	//! @retval false … データのスワップに失敗
	//! 
	bool swap( buffered_array &a )
	{
		if( base::swap( a ) )
		{
			size_type tmp = size1_;
			size1_ = a.size1_;
			a.size1_ = tmp;

			tmp = size2_;
			size2_ = a.size2_;
			a.size2_ = tmp;

			tmp = size3_;
			size3_ = a.size3_;
			a.size3_ = tmp;

			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief コンテナの要素を空にする
	//! 
	//! コンテナに格納されているデータを全て削除し，コンテナを空（要素数0）にする
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}

	size_type size1( ) const { return( size1_ > 0 ? size1_ : base::size1( ) ); }	///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type size2( ) const { return( size2_ > 0 ? size2_ : base::size2( ) ); }	///< @brief Y軸方向のコンテナに格納されているデータ数を返す
	size_type size3( ) const { return( size3_ > 0 ? size3_ : base::size3( ) ); }	///< @brief Z軸方向のコンテナに格納されているデータ数を返す
	size_type width( ) const { return( size1( ) ); }								///< @brief X軸方向のコンテナに格納されているデータ数を返す
	size_type height( ) const { return( size2( ) ); }								///< @brief Y軸方向のコンテナに格納されているデータ数を返す
	size_type depth( ) const { return( size3( ) ); }								///< @brief Z軸方向のコンテナに格納されているデータ数を返す


private:
	/// @brief array コンテナからデータをコピーする
	//! 
	//! @attention 余分な画像部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array1 コンテナからデータをコピーする
	//! 
	//! @attention 余分な画像部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array1< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array2 コンテナからデータをコピーする
	//! 
	//! @attention 余分な画像部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array2< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		for( size_type j = 0 ; j < h ; j++ )
		{
			for( size_type i = 0 ; i < w ; i++ )
			{
				( *this )( i, j ) = static_cast< value_type >( o( i, j ) );
			}
		}
		return( *this );
	}


	/// @brief array3 コンテナからデータをコピーする
	//! 
	//! @attention 余分な画像部分には変更は加えられない
	//!
	//! @param[in] o  … コピー元のコンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array3< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		size_type d = depth( )  < o.depth( )  ? depth( )  : o.depth( );
		for( size_type k = 0 ; k < d ; k++ )
		{
			for( size_type j = 0 ; j < h ; j++ )
			{
				for( size_type i = 0 ; i < w ; i++ )
				{
					( *this )( i, j, k ) = static_cast< value_type >( o( i, j, k ) );
				}
			}
		}
		return( *this );
	}


public:
	/// @brief 同じ型の buffered_array コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の buffered_array コンテナ
	//! 
	//! @return 自分自身
	//! 
	const buffered_array& operator =( const buffered_array &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = size3_ = 0;
		}
		else
		{
			size1_ = o.size1_;
			size2_ = o.size2_;
			size3_ = o.size3_;
		}

		return( *this );
	}


	/// @brief 要素の型が異なる array コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array< T, Allocator > &o )
	{
		resize( o.size( ) );
		return( copy( o ) );
	}


	/// @brief 要素の型が異なる array1 コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array1 コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array1< T, Allocator > &o )
	{
		resize( o.size( ) );
		reso1( o.reso1( ) );
		return( copy( o ) );
	}


	/// @brief 要素の型が異なる array2 コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array2 コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array2< T, Allocator > &o )
	{
		resize( o.size1( ), o.size2( ) );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		return( copy( o ) );
	}


	/// @brief 要素の型が異なる array3 コンテナを代入する
	//! 
	//! コピー元であるコンテナ o と全く同じコンテナを作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の array3 コンテナ
	//! 
	//! @return 自分自身
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array3< T, Allocator > &o )
	{
		resize( o.size1( ), o.size2( ), o.size3( ) );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		reso3( o.reso3( ) );
		return( copy( o ) );
	}

public:
	/// @brief ディフォルトコンストラクタ．要素数 0 のコンテナを作成する
	buffered_array( ) : base( ), size1_( 0 ), size2_( 0 ), size3_( 0 ) {}

	/// @briefコピーコンストラクタ
	buffered_array( const buffered_array &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) ) {}

	/// @brief array 配列 o を用いて初期化し，一辺の長さを2の指数乗にする
	template < class T, class Allocator >
	buffered_array( const array< T, Allocator > &o )
		: base( floor_square( o.size( ) ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	/// @brief array1 配列 o を用いて初期化し，一辺の長さを2の指数乗にする
	template < class T, class Allocator >
	buffered_array( const array1< T, Allocator > &o )
		: base( floor_square( o.size( ) ), o.reso1( ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	/// @brief array2 配列 o を用いて初期化し，一辺の長さを2の指数乗にする
	template < class T, class Allocator >
	buffered_array( const array2< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), o.reso1( ), o.reso2( ) ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( 0 )
	{
		copy( o );
	}

	/// @brief array3 配列 o を用いて初期化し，一辺の長さを2の指数乗にする
	template < class T, class Allocator >
	buffered_array( const array3< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), floor_square( o.size3( ) ), o.reso1( ), o.reso2( ), o.reso3( ) ),
			size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) )
	{
		copy( o );
	}
};


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     a   … array 配列
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 2, 3, 4
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array< T, Allocator > &a )
{
	typename array< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[ i ];
		if( i != a.size1( ) - 1 ) out << ", ";
	}

	return( out );
}


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     a   … array1 配列
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 2, 3, 4
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array1< T, Allocator > &a )
{
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[ i ];
		if( i != a.size1( ) - 1 ) out << ", ";
	}

	return( out );
}


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     a   … array2 配列
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array2< T, Allocator > &a )
{
	typename array2< T, Allocator >::size_type i, j;
	for( j = 0 ; j < a.size2( ) ; j++ )
	{
		if( j != 0 )
		{
			out << std::endl;
		}
		for( i = 0 ; i < a.size1( ) ; i++ )
		{
			out << a( i, j );
			if( i != a.size1( ) - 1 ) out << ", ";
		}
	}

	return( out );
}


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     a   … array3 配列
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! ----- separator -----
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array3< T, Allocator > &a )
{
	typename array3< T, Allocator >::size_type i, j, k;
	for( k = 0 ; k < a.size3( ) ; k++ )
	{
		for( j = 0 ; j < a.size2( ) ; j++ )
		{
			for( i = 0 ; i < a.size1( ) ; i++ )
			{
				out << a( i, j, k );
				if( i != a.size1( ) - 1 ) out << ", ";
			}
			out << std::endl;
		}
		if( k != a.size3( ) - 1 )
		{
			out << "----- separator -----";
		}
		out << std::endl;
	}

	return( out );
}



// 各コンテナに対する画像間演算

#if defined(_ARRAY_BIND_OPERATION_SUPPORT_) && _ARRAY_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array.h"
#endif

#if defined(_ARRAY1_BIND_OPERATION_SUPPORT_) && _ARRAY1_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array1.h"
#endif

#if defined(_ARRAY2_BIND_OPERATION_SUPPORT_) && _ARRAY2_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array2.h"
#endif

#if defined(_ARRAY3_BIND_OPERATION_SUPPORT_) && _ARRAY3_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array3.h"
#endif


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_H__
