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

/// @file mist/integral_image.h
//!
//! @brief Integral Image のためのライブラリ
//!

#ifndef __INCLUDE_INTEGRAL_IMAGE_H__
#define __INCLUDE_INTEGRAL_IMAGE_H__

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN

// 入力のデータ型から画素値の総和のデータ型を決定するためのクラス
namespace __integral_image__
{
	template< class T, bool Is_decimal, bool Is_color >	struct integral_type					{ typedef typename T::template rebind< double >::other		type; };
	template< class T >									struct integral_type< T, false, true >	{ typedef typename T::template rebind< ptrdiff_t >::other	type; };
	template< class T >									struct integral_type< T, true,  false >	{ typedef double			type; };
	template< class T >									struct integral_type< T, false, false >	{ typedef ptrdiff_t			type; };
}

/// @brief 1次元の積分画像を保持し，任意の矩形領域の画素値の総和を高速に計算するためのクラス
//!
//! 画素数nに対してO(n)の前計算を行うことにより，任意の矩形領域の画素値の総和をO(1)で算出する
//! 同一の画像配列中の複数の異なる矩形領域に対して、総和を計算する必要がある場合などに有効
//!
//! mist::array, mist::array1, mist::array2, mist::array3に対応．
//! @attention 入力のデータ型は、算術型，mist::rgb< 算術型 >のみ対応．
//! @attention 得られる画素値の総和のデータ型は，入力のデータ型に依存して変化．
//! @attention 入力型：符号無し整数 → 画素値の総和型：size_t
//! @attention 入力型：符号付き整数 → 画素値の総和型：int
//! @attention 入力型：浮動小数点   → 画素値の総和型：double
//! @attention mist::rgb< 入力型 >  → mist::rgb< 画素値の総和型 >
//!
//! 特殊化のためのクラスであり外部からは利用しない
//!
template< typename Array >
class integral_image
{
};


//! @addtogroup integral_image_group Integral Image
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/integral_image.h>
//! @endcode
//!
//!  @{



/// @brief 1次元の積分画像を保持し，任意の矩形領域の画素値の総和を高速に計算するためのクラス
//!
//! 画素数nに対してO(n)の前計算を行うことにより，任意の矩形領域の画素値の総和をO(1)で算出する
//! 同一の画像配列中の複数の異なる矩形領域に対して、総和を計算する必要がある場合などに有効
//!
//! mist::array, mist::array1, mist::array2, mist::array3に対応．
//! @attention 入力のデータ型は、算術型，mist::rgb< 算術型 >のみ対応．
//! @attention 得られる画素値の総和のデータ型は，入力のデータ型に依存して変化．
//! @attention 入力型：符号無し整数 → 画素値の総和型：size_t
//! @attention 入力型：符号付き整数 → 画素値の総和型：int
//! @attention 入力型：浮動小数点   → 画素値の総和型：double
//! @attention mist::rgb< 入力型 >  → mist::rgb< 画素値の総和型 >
//! 
//! @param T         … 入力のデータ型（ double や mist::rgb< unsigned char > など ）
//! @param Allocator … MISTコンテナで使用するアロケータ
//!
//! @code 使用例
//! // 画像配列（1-3次元）の用意
//! mist::array2< unsigned char > img;
//! mist::read_bmp( img, "img.bmp" );
//! 
//! // Integral Image の構築：O(n) （テンプレートの引数として入力配列の型を指定）
//! mist::integral_image< mist::array2< unsigned char > > i_img( img );
//!
//! // 始点( 100, 200 ), サイズ( 50, 60 )の領域の画素値の総和を出力：O(1)
//! std::cout << i_img( 100, 200, 50, 60 ) << std::endl;
//!
//! // 始点( 200, 100 ), サイズ( 60, 50 )の領域の画素値の総和を出力：O(1)
//! std::cout << i_img( 200, 100, 60, 50 ) << std::endl;
//! @endcode
//!
template< typename T, typename Allocator >
class integral_image< array< T, Allocator > >
{
public:
	typedef typename __integral_image__::integral_type< T, is_float< T >::value, is_color< T >::value >::type value_type;
	typedef array< value_type > integral_image_type;
	typedef array< T, Allocator > image_type;
	typedef typename image_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename image_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	integral_image_type integral_;

public:

	/// @brief 任意の矩形領域内の画素の総和を返す：O(1)
	//! 
	//! @param[in] begin … 始点
	//! @param[in] size  … サイズ
	//!
	//! @return 画素値の総和
	//!
	value_type operator ( )( const size_type begin, const size_type size ) const
	{
		return( integral_( begin + size ) - integral_( begin ) );
	}

	/// @brief 任意の位置の積分値を返す
	//! 
	//! @param[in] i … i方向の始点
	//!
	//! @return 画素値の総和
	//!
	value_type operator ( )( const size_type i ) const
	{
		return( integral_( i + 1 ) );
	}

	/// @brief 全ての画素値の総和を返す：O(1)
	//! 
	//! @return 画素値の総和
	//!
	value_type overall( ) const 
	{
		return( integral_[ integral_.size( ) - 1 ] );
	}

	/// @brief 画像配列のサイズを返す
	//! 
	//! @return サイズ
	//!
	size_type size( ) const
	{
		return( integral_.size( ) - 1 );
	}

	/// @brief Integral Image の構築：O(n)
	//! 
	//! @param[in] in … 画像配列
	//!
	void construct_integral_array( const image_type &in )
	{
		integral_.resize( in.size( ) + 1 );	
		for( size_type i = 1 ; i < integral_.width( ) ; i ++ )
		{
			integral_[ i ] = in[ i - 1 ] + integral_[ i - 1 ];
		}
	}

	/// @brief コンストラクタ
	//! 
	//! @param[in] in … 画像配列
	//!
	integral_image( const image_type &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief デフォルトコンストラクタ
	//!
	integral_image( ) : integral_( )
	{
	}
};


/// @brief 2次元の積分画像を保持し，任意の矩形領域の画素値の総和を高速に計算するためのクラス
//!
//! 画素数nに対してO(n)の前計算を行うことにより，任意の矩形領域の画素値の総和をO(1)で算出する
//! 同一の画像配列中の複数の異なる矩形領域に対して、総和を計算する必要がある場合などに有効
//!
//! mist::array, mist::array1, mist::array2, mist::array3に対応．
//! @attention 入力のデータ型は、算術型，mist::rgb< 算術型 >のみ対応．
//! @attention 得られる画素値の総和のデータ型は，入力のデータ型に依存して変化．
//! @attention 入力型：符号無し整数 → 画素値の総和型：size_t
//! @attention 入力型：符号付き整数 → 画素値の総和型：int
//! @attention 入力型：浮動小数点   → 画素値の総和型：double
//! @attention mist::rgb< 入力型 >  → mist::rgb< 画素値の総和型 >
//! 
//! @param T         … 入力のデータ型（ double や mist::rgb< unsigned char > など ）
//! @param Allocator … MISTコンテナで使用するアロケータ
//!
//! @code 使用例
//! // 画像配列（1-3次元）の用意
//! mist::array2< unsigned char > img;
//! mist::read_bmp( img, "img.bmp" );
//! 
//! // Integral Image の構築：O(n) （テンプレートの引数として入力配列の型を指定）
//! mist::integral_image< mist::array2< unsigned char > > i_img( img );
//!
//! // 始点( 100, 200 ), サイズ( 50, 60 )の領域の画素値の総和を出力：O(1)
//! std::cout << i_img( 100, 200, 50, 60 ) << std::endl;
//!
//! // 始点( 200, 100 ), サイズ( 60, 50 )の領域の画素値の総和を出力：O(1)
//! std::cout << i_img( 200, 100, 60, 50 ) << std::endl;
//! @endcode
//!
template< typename T, typename Allocator >
class integral_image< array2< T, Allocator > >
{
public:
	typedef typename __integral_image__::integral_type< T, is_float< T >::value, is_color< T >::value >::type value_type;
	typedef array2< value_type > integral_image_type;
	typedef array2< T, Allocator > image_type;
	typedef typename image_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename image_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	integral_image_type integral_;

public:
	/// @brief 任意の矩形領域内の画素の総和を返す：O(1)
	//! 
	//! @param[in] begin_i … i方向の始点
	//! @param[in] begin_j … j方向の始点
	//! @param[in] width   … i方向のサイズ
	//! @param[in] height  … i方向のサイズ
	//!
	//! @return 画素値の総和
	//!
	value_type operator ( )( const size_type begin_i, const size_type begin_j, const size_type width, const size_type height ) const
	{
		return( integral_( begin_i + width, begin_j + height ) + integral_( begin_i, begin_j ) - integral_( begin_i + width, begin_j ) - integral_( begin_i, begin_j + height ) );
	}

	/// @brief 任意の位置の積分値を返す
	//! 
	//! @param[in] i … i方向の始点
	//! @param[in] j … j方向の始点
	//!
	//! @return 画素値の総和
	//!
	value_type operator ( )( const size_type i, const size_type j ) const
	{
		return( integral_( i + 1, j + 1 ) );
	}

	/// @brief 全ての画素値の総和を返す：O(1)
	//! 
	//! @return 画素値の総和
	//!
	value_type overall( ) const 
	{
		return( integral_[ integral_.size( ) - 1 ] );
	}

	/// @brief 画像配列のi方向のサイズを返す
	//! 
	//! @return i方向のサイズ
	//!
	size_type size1( ) const
	{
		return( integral_.width( ) - 1 );
	}

	/// @brief 画像配列のj方向のサイズを返す
	//! 
	//! @return j方向のサイズ
	//!
	size_type size2( ) const
	{
		return( integral_.height( ) - 1 );
	}

	/// @brief 画像配列のi方向のサイズを返す
	//! 
	//! @return i方向のサイズ
	//!
	size_type width( ) const
	{
		return( size1( ) );
	}

	/// @brief 画像配列のj方向のサイズを返す
	//! 
	//! @return j方向のサイズ
	//!
	size_type height( ) const
	{
		return( size2( ) );
	}

	/// @brief 画像配列のサイズを返す
	//! 
	//! @return サイズ
	//!
	size_type size( ) const
	{
		return( width( ) * height( ) );
	}

	/// @brief Integral Image の構築：O(n)
	//! 
	//! @param[in] in … 画像配列
	//!
	void construct_integral_array( const image_type &in )
	{ 
		integral_.resize( in.width( ) + 1, in.height( ) + 1 );	

#if 0
		int iw       = static_cast< int >( in.width( ) );
		int ih       = static_cast< int >( in.height( ) );
		size_type ow = integral_.width( );

		#pragma omp parallel for firstprivate( iw, ih ) schedule( guided )
		for( int j = 0 ; j < ih ; j++ )
		{
			typename image_type::const_pointer    ip = &in( 0, j );
			typename integral_image_type::pointer op = &integral_( 1, j + 1 );

			op[ 0 ] = ip[ 0 ];
			for( int i = 1 ; i < iw ; i++ )
			{
				op[ i ] = op[ i - 1 ] + ip[ i ];
			}
		}

		#pragma omp parallel for firstprivate( iw, ih, ow ) schedule( guided )
		for( int i = 0 ; i < iw ; i++ )
		{
			typename integral_image_type::pointer    op = &integral_( i + 1, 1 );
			typename integral_image_type::value_type ov = *op;

			op += ow;

			for( int j = 1 ; j < ih ; j++ )
			{
				ov  += *op;
				*op  = ov;
				op  += ow;
			}
		}
#else
		typename image_type::const_pointer    ip = &in[ 0 ];
		typename integral_image_type::pointer op = &integral_( 1, 1 );

		op[ 0 ] = ip[ 0 ];
		for( size_type i = 1 ; i < in.width( ) ; i++ )
		{
			op[ i ] = op[ i - 1 ] + ip[ i ];
		}

		typename integral_image_type::pointer oop = op;
		ip += in.width( );
		op += integral_.width( );

		for( size_type j = 1 ; j < in.height( ) ; j++ )
		{
			value_type tmp = ip[ 0 ];
			op[ 0 ] = oop[ 0 ] + tmp;

			for( size_type i = 1 ; i < in.width( ) ; i++ )
			{
				tmp += ip[ i ];
				op[ i ] = oop[ i ] + tmp;
			}

			oop = op;
			ip += in.width( );
			op += integral_.width( );
		}
#endif
	}

	/// @brief コンストラクタ
	//! 
	//! @param[in] in … 画像配列
	//!
	integral_image( const image_type &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief デフォルトコンストラクタ
	//!
	integral_image( ) : integral_( )
	{
	}
};


/// @brief 3次元の積分画像を保持し，任意の矩形領域の画素値の総和を高速に計算するためのクラス
//!
//! 画素数nに対してO(n)の前計算を行うことにより，任意の矩形領域の画素値の総和をO(1)で算出する
//! 同一の画像配列中の複数の異なる矩形領域に対して、総和を計算する必要がある場合などに有効
//!
//! mist::array, mist::array1, mist::array2, mist::array3に対応．
//! @attention 入力のデータ型は、算術型，mist::rgb< 算術型 >のみ対応．
//! @attention 得られる画素値の総和のデータ型は，入力のデータ型に依存して変化．
//! @attention 入力型：符号無し整数 → 画素値の総和型：size_t
//! @attention 入力型：符号付き整数 → 画素値の総和型：int
//! @attention 入力型：浮動小数点   → 画素値の総和型：double
//! @attention mist::rgb< 入力型 >  → mist::rgb< 画素値の総和型 >
//! 
//! @param T         … 入力のデータ型（ double や mist::rgb< unsigned char > など ）
//! @param Allocator … MISTコンテナで使用するアロケータ
//!
//! @code 使用例
//! // 画像配列（1-3次元）の用意
//! mist::array2< unsigned char > img;
//! mist::read_bmp( img, "img.bmp" );
//! 
//! // Integral Image の構築：O(n) （テンプレートの引数として入力配列の型を指定）
//! mist::integral_image< mist::array2< unsigned char > > i_img( img );
//!
//! // 始点( 100, 200 ), サイズ( 50, 60 )の領域の画素値の総和を出力：O(1)
//! std::cout << i_img( 100, 200, 50, 60 ) << std::endl;
//!
//! // 始点( 200, 100 ), サイズ( 60, 50 )の領域の画素値の総和を出力：O(1)
//! std::cout << i_img( 200, 100, 60, 50 ) << std::endl;
//! @endcode
//!
template< typename T, typename Allocator >
class integral_image< array3< T, Allocator > >
{
public:
	typedef typename __integral_image__::integral_type< T, is_float< T >::value, is_color< T >::value >::type value_type;
	typedef array3< value_type > integral_image_type;
	typedef array3< T, Allocator > image_type;
	typedef typename image_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename image_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	integral_image_type integral_;

public:
	/// @brief 任意の矩形領域内の画素の総和を返す：O(1)
	//! 
	//! @param[in] begin_i … i方向の始点
	//! @param[in] begin_j … j方向の始点
	//! @param[in] begin_k … k方向の始点
	//! @param[in] width   … i方向のサイズ
	//! @param[in] height  … j方向のサイズ
	//! @param[in] depth   … k方向のサイズ
	//!
	//! @return 画素値の総和
	//!
	value_type operator ( )( const size_type begin_i, const size_type begin_j, const size_type begin_k, const size_type width, const size_type height, const size_type depth ) const
	{
		return( integral_( begin_i + width, begin_j + height, begin_k + depth ) + integral_( begin_i, begin_j, begin_k + depth ) + integral_( begin_i, begin_j + height, begin_k ) + integral_( begin_i + width, begin_j, begin_k ) - integral_( begin_i, begin_j + height, begin_k + depth ) - integral_( begin_i + width, begin_j, begin_k + depth ) - integral_( begin_i + width, begin_j + height, begin_k ) - integral_( begin_i, begin_j, begin_k ) );
	}

	/// @brief 任意の位置の積分値を返す
	//! 
	//! @param[in] i … i方向の始点
	//! @param[in] j … j方向の始点
	//! @param[in] k … k方向の始点
	//!
	//! @return 画素値の総和
	//!
	value_type operator ( )( const size_type i, const size_type j, const size_type k ) const
	{
		return( integral_( i + 1, j + 1, k + 1 ) );
	}

	/// @brief 全ての画素値の総和を返す：O(1)
	//! 
	//! @return 画素値の総和
	//!
	value_type overall( ) const 
	{
		return( integral_( integral_.width( ) - 1, integral_.height( ) - 1, integral_.depth( ) - 1 ) );
	}

	/// @brief 画像配列のi方向のサイズを返す
	//! 
	//! @return i方向のサイズ
	//!
	size_type size1( ) const
	{
		return( integral_.width( ) - 1 );
	}

	/// @brief 画像配列のj方向のサイズを返す
	//! 
	//! @return j方向のサイズ
	//!
	size_type size2( ) const
	{
		return( integral_.height( ) - 1 );
	}

	/// @brief 画像配列のk方向のサイズを返す
	//! 
	//! @return k方向のサイズ
	//!
	size_type size3( ) const
	{
		return( integral_.depth( ) - 1 );
	}

	/// @brief 画像配列のi方向のサイズを返す
	//! 
	//! @return i方向のサイズ
	//!
	size_type width( ) const
	{
		return( size1( ) );
	}

	/// @brief 画像配列のj方向のサイズを返す
	//! 
	//! @return j方向のサイズ
	//!
	size_type height( ) const
	{
		return( size2( ) );
	}

	/// @brief 画像配列のk方向のサイズを返す
	//! 
	//! @return k方向のサイズ
	//!
	size_type depth( ) const
	{
		return( size3( ) );
	}

	/// @brief 画像配列のサイズを返す
	//! 
	//! @return サイズ
	//!
	size_type size( ) const
	{
		return( width( ) * height( ) * depth( ) );
	}

	/// @brief Integral Image の構築：O(n)
	//! 
	//! @param[in] in … 画像配列
	//!
	void construct_integral_array( const image_type &in )
	{
		integral_.resize( in.width( ) + 1, in.height( ) + 1, in.depth( ) + 1 );	
		for( size_type k = 1 ; k < integral_.depth( ) ; k ++  )
		{
			for( size_type j = 1 ; j < integral_.height( ) ; j ++ )
			{
				for( size_type i = 1 ; i < integral_.width( ) ; i ++ )
				{
					integral_( i, j, k ) = in( i - 1, j - 1, k - 1 ) + integral_( i - 1, j, k ) + integral_( i, j - 1, k ) + integral_( i, j, k - 1 ) + integral_( i - 1, j - 1, k - 1 ) - integral_( i - 1, j - 1, k ) - integral_( i - 1, j, k - 1 ) - integral_( i, j - 1, k - 1 );
				}
			}
		}
	}

	/// @brief コンストラクタ
	//! 
	//! @param[in] in … 画像配列
	//!
	integral_image( const image_type &in )
	{
		construct_integral_array( in );
	}

	/// @brief デフォルトコンストラクタ
	//!
	integral_image( ) : integral_( )
	{
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
inline std::ostream &operator <<( std::ostream &out, const integral_image< array< T, Allocator > > &a )
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
inline std::ostream &operator <<( std::ostream &out, const integral_image< array1< T, Allocator > > &a )
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
inline std::ostream &operator <<( std::ostream &out, const integral_image< array2< T, Allocator > > &a )
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
inline std::ostream &operator <<( std::ostream &out, const integral_image< array3< T, Allocator > > &a )
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


/// @}
//  Integral Imageグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_INTEGRAL_IMAGE_H__
