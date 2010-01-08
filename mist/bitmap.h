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

/// @file mist/bitmap.h
//!
//! @brief Windows全般で利用されるビットマップ画像を扱うコンテナ
//!

#ifndef __INCLUDE_BITMAP_H__
#define __INCLUDE_BITMAP_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif



// mist名前空間の始まり
_MIST_BEGIN

template < size_t BITS >
struct bitmap_type
{
	typedef bgr< unsigned char > value_type;
};

template < >
struct bitmap_type< 8 >
{
	typedef unsigned char value_type;
};


/// @brief 要素数が可変のビットマップ画像を扱うクラス
//! 
//! 2次元画像を扱うための基本クラス
//! （@ref mist/bitmap.h をインクルードする）
//! 
//! @attention MISTで提供するほとんどのアルゴリズムに対して本コンテナを直接利用することはできません．
//! @attention MISTのアルゴリズムを利用する場合は， @ref mist::convert のコンバート関数を利用して，array2 型に変換してから行ってください．
//! 
//! @param BITS      … ビットマップで利用する1画素あたりのビット数（24ビットと32ビットのみをサポート）
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < size_t BITS, class Allocator = std::allocator< unsigned char > >
class bitmap : public array< unsigned char, Allocator >
{
public:
	typedef typename bitmap_type< BITS >::value_type value_type;	///< @brief 内部データ型
	typedef value_type* pointer;									///< @brief データ型のポインター型．data の場合，data * となる
	typedef value_type& reference;									///< @brief データ型の参照．data の場合，data & となる
	typedef const value_type& const_reference;						///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef const value_type* const_pointer;						///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ


protected:
	typedef array< unsigned char, Allocator > base;		///< @brief 基底クラス
	size_type size1_;									///< @brief X軸方向の要素数
	size_type size2_;									///< @brief Y軸方向の要素数
	size_type nbytes_;									///< @brief １行あたりのバイト数

	/// @brief １画素を表現するのに用いるバイト数
	static size_type pixel_bytes( )
	{
		return( BITS / 8 );
	}

	/// @brief １行あたりに使用するバイト数
	static size_type num_bytes( size_type width )
	{
		width = width * pixel_bytes( );
		if( width % 4 > 0 )
		{
			width += 4 - ( width % 4 );
		}
		return( width );
	}

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
		nbytes_ = num_bytes( num1 );
		if( base::resize( nbytes_ * num2 ) )
		{
			size1_ = num1;
			size2_ = num2;
			return( true );
		}
		else
		{
			size1_ = size2_ = nbytes_ = 0;
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
			bitmap o( *this );
			if( resize( w, h ) )
			{
				for( difference_type j = 0 ; j < h ; j++ )
				{
					for( difference_type i = 0 ; i < w ; i++ )
					{
						operator ()( i, j ) = o( i + x, j + y );
					}
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			bitmap o( w, h );
			for( difference_type j = 0 ; j < h ; j++ )
			{
				for( difference_type i = 0 ; i < w ; i++ )
				{
					o( i, j ) = operator ()( i + x, j + y );
				}
			}

			swap( o );
		}

		return( true );
	}


	/// @brief コンテナ内の全ての内容を入れ替える．
	//! 
	//! 入れ替え元のコンテナ a の中身と全て入れ替える
	//! 
	//! @param[in] a  … 内容を入れ替える対象
	//! 
	//! @retval true  … データのスワップに成功
	//! @retval false … データのスワップに失敗
	//! 
	bool swap( bitmap &a )
	{
		if( base::swap( a ) )
		{
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

	size_type size( ) const { return( size1_ * size2_ ); }			///< @brief コンテナに格納されているデータ数を返す

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
	template < size_t BBITS, class AAlocator >
	const bitmap& operator =( const bitmap< BBITS, AAlocator > &o )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2_ ; j++ )
			{
				for( size_type i = 0 ; i < size1_ ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}

		return( *this );
	}


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
	const bitmap& operator =( const array2< TT, AAlocator > &o )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2_ ; j++ )
			{
				for( size_type i = 0 ; i < size1_ ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}

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
	const bitmap& operator =( const bitmap &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;

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
		return( reinterpret_cast< pointer >( base::data_ + i * pixel_bytes( ) + j * nbytes_ ) );
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
		return( reinterpret_cast< const_pointer >( base::data_ + i * pixel_bytes( ) + j * nbytes_ ) );
	}

public:
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
		_CHECK_ACCESS_VIOLATION1U_( index )
		size_type j = index / size1_;
		size_type i = index - j * size1_;
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


	/// @brief index で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された index が有効な範囲内にあるかをチェックする
	//!
	/// @param[in] index … コンテナ内の要素位置
	//! 
	/// @return 指定された要素を示す参照
	//!
	reference operator []( size_type index ) const
	{
		_CHECK_ACCESS_VIOLATION1U_( index )
		size_type j = index / size1_;
		size_type i = index - j * size1_;
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
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
	reference at( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
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
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
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
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
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
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


public:
	/// @brief ディフォルトコンストラクタ．要素数 0，解像度 1.0 × 1.0 のコンテナを作成する
	bitmap( ) : base( ), size1_( 0 ), size2_( 0 ), nbytes_( 0 ) {}

	/// @brief アロケータ a のコピーを利用する
	explicit bitmap( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ), nbytes_( 0 ) {}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，デフォルト値で要素を初期化する
	bitmap( size_type num1, size_type num2 ) : base( num_bytes( num1 ) * num2 ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，使用するアロケータを a に設定する
	bitmap( size_type num1, size_type num2, const Allocator &a ) : base( num_bytes( num1 ) * num2, a ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}


	/// @brief 要素数 num1 × num2 個のコンテナを作成し，全要素を val で初期化する
	bitmap( size_type num1, size_type num2, const value_type &val ) : base( num_bytes( num1 ) * num2, val ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}

	/// @brief 要素数 num1 × num2 個のコンテナを作成し，全要素を val で初期化し，使用するアロケータを a に設定する
	bitmap( size_type num1, size_type num2, const value_type &val, const Allocator &a ) : base( num_bytes( num1 ) * num2, val, a ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}


	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 個のコンテナを作成する（ptr が指す先の利用可能なメモリ量は mem_available ）
	bitmap( size_type num1, size_type num2, void *ptr, size_type mem_available ) : base( num_bytes( num1 ) * num2, reinterpret_cast< unsigned char * >( ptr ), mem_available ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}

	/// @brief ptr が指すメモリ領域に，要素数 num1 × num2 個のコンテナを作成し，全要素を val で初期化する（ptr が指す先の利用可能なメモリ量は mem_available ）
	bitmap( size_type num1, size_type num2, const value_type &val, void *ptr, size_type mem_available ) : base( num_bytes( num1 ) * num2, reinterpret_cast< unsigned char * >( ptr ), mem_available ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) )
		{
			size1_ = size2_ = nbytes_ = 0;
		}
		else
		{
			for( size_type j = 0 ; j < size2_ ; j++ )
			{
				for( size_type i = 0 ; i < size1_ ; i++ )
				{
					operator ()( i, j ) = val;
				}
			}
		}
	}


	/// @brief 他の bitmap 配列で要素の型が異なるものから同じ要素数の配列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < size_t BBITS, class AAlocator >
	bitmap( const bitmap< BBITS, AAlocator > &o ) : base( ), size1_( 0 ), size2_( 0 ), nbytes_( 0 )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2( ) ; j++ )
			{
				for( size_type i = 0 ; i < size1( ) ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}
	}


	/// @brief 他の array2 配列で要素の型が異なるものから同じ要素数の配列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	bitmap( const array2< TT, AAlocator > &o ) : base( ), size1_( 0 ), size2_( 0 ), nbytes_( 0 )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2( ) ; j++ )
			{
				for( size_type i = 0 ; i < size1( ) ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}
	}

	/// @brief 他の bitmap 配列で同じ要素型のものを用いて初期化する
	bitmap( const bitmap< BITS, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ), nbytes_( o.nbytes_ ) {}
};



/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     a   … bitmap 配列
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < size_t BITS, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const bitmap< BITS, Allocator > &a )
{
	typename bitmap< BITS, Allocator >::size_type i, j;
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



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_H__
