// 
// Copyright (c) 2003-2007, MIST Project, Intelligent Media Integration COE, Nagoya University
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

#include <mist/mist.h>


// mist名前空間の始まり
_MIST_BEGIN

// 入力のデータ型から画素値の総和のデータ型を決定するためのクラス
namespace __integral_image__
{
	template< typename Type >	struct is_signed					{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_signed< unsigned char >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< unsigned short >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< unsigned int >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< unsigned long >	{ _MIST_CONST( bool, value, false ); };

	template< typename Type >	struct is_decimal			{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_decimal< float >	{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_decimal< double >	{ _MIST_CONST( bool, value, true  ); };


	template< typename Type >	struct is_signed< mist::rgb< Type > >			{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_signed< mist::rgb< unsigned char > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< mist::rgb< unsigned short > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< mist::rgb< unsigned int > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< mist::rgb< unsigned long > >	{ _MIST_CONST( bool, value, false ); };

	template< typename Type >	struct is_decimal< mist::rgb< Type > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_decimal< mist::rgb< float > >	{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_decimal< mist::rgb< double > >{ _MIST_CONST( bool, value, true  ); };


	template< bool Is_signed, bool Is_decimal, bool Is_color >	struct integral_type						{ typedef mist::rgb< double >	type; };
	template< >													struct integral_type< true,  false, true >	{ typedef mist::rgb< int >		type; };
	template< >													struct integral_type< false, false, true >	{ typedef mist::rgb< size_t >	type; };
	template< >													struct integral_type< true,  true,  false >	{ typedef double				type; };
	template< >													struct integral_type< true,  false, false >	{ typedef int					type; };
	template< >													struct integral_type< false, false, false >	{ typedef size_t				type; };
}

// 部分特殊化しない場合（不使用）
template< typename Value_type >
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



/// @brief 任意の矩形領域の画素値の総和を高速に計算するためのクラス
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
//! @param Value_type  … 入力のデータ型（ double や mist::rgb< unsigned char > など ）
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
template< typename Value_type >
class integral_image< mist::array< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array< integral_type > integral_;

	integral_type _value( const size_t i ) const
	{
		return integral_[ i ];
	}

public:

	/// @brief 任意の矩形領域内の画素の総和を返す：O(1)
	//! 
	//! @param[in] begin … 始点
	//! @param[in] size  … サイズ
	//!
	//! @return 画素値の総和
	//!
	integral_type operator ( )( const size_t begin, const size_t size ) const
	{
		return _value( begin + size ) - _value( begin );
	}

	/// @brief 全ての画素値の総和を返す：O(1)
	//! 
	//! @return 画素値の総和
	//!
	integral_type overall( ) const 
	{
		return integral_[ integral_.size( ) - 1 ];
	}

	/// @brief 画像配列のサイズを返す
	//! 
	//! @return サイズ
	//!
	size_t size( ) const
	{
		return integral_.size( ) - 1;;
	}

	/// @brief Integral Image の構築：O(n)
	//! 
	//! @param[in] in … 画像配列
	//!
	void construct_integral_array( const mist::array< Value_type > &in )
	{
		integral_.resize( in.size( ) + 1 );	
		for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
		{
			integral_[ i ] = in[ i - 1 ] + integral_[ i - 1 ];
		}
	}

	/// @brief コンストラクタ
	//! 
	//! @param[in] in … 画像配列
	//!
	integral_image( const mist::array< Value_type > &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief デフォルトコンストラクタ
	//!
	
	integral_image( ) : integral_( mist::array< integral_type >( ) )
	{
	}
};


template< typename Value_type >
class integral_image< mist::array1< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array1< integral_type > integral_;

	integral_type _value( const size_t i ) const
	{
		return integral_[ i ];
	}

public:

	/// @brief 任意の矩形領域内の画素の総和を返す：O(1)
	//! 
	//! @param[in] begin … 始点
	//! @param[in] size  … サイズ
	//!
	//! @return 画素値の総和
	//!
	integral_type operator ( )( const size_t begin, const size_t size ) const
	{
		return _value( begin + size ) - _value( begin );
	}

	/// @brief 全ての画素値の総和を返す：O(1)
	//! 
	//! @return 画素値の総和
	//!
	integral_type overall( ) const 
	{
		return integral_[ integral_.size( ) - 1 ];
	}

	/// @brief 画像配列のサイズを返す
	//! 
	//! @return サイズ
	//!
	size_t size( ) const
	{
		return integral_.size( ) - 1;;
	}

	/// @brief Integral Image の構築：O(n)
	//! 
	//! @param[in] in … 画像配列
	//!
	void construct_integral_array( const mist::array1< Value_type > &in )
	{ 
		integral_.resize( in.size( ) + 1 );	
		for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
		{
			integral_[ i ] = in[ i - 1 ] + integral_[ i - 1 ];
		}
	}

	/// @brief コンストラクタ
	//! 
	//! @param[in] in … 画像配列
	//!
	integral_image( const mist::array1< Value_type > &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief デフォルトコンストラクタ
	//!
	integral_image( ) : integral_( mist::array1< integral_type >( ) )
	{
	}
};


template< typename Value_type >
class integral_image< mist::array2< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array2< integral_type > integral_;

	integral_type _value( const size_t i, const size_t j ) const
	{
		return integral_( i, j );
	}

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
	integral_type operator ( )( const size_t begin_i, const size_t begin_j, const size_t width, const size_t height ) const
	{
		return( _value( begin_i + width, begin_j + height ) + _value( begin_i, begin_j ) - _value( begin_i + width, begin_j ) - _value( begin_i, begin_j + height ) );
	}

	/// @brief 全ての画素値の総和を返す：O(1)
	//! 
	//! @return 画素値の総和
	//!
	integral_type overall( ) const 
	{
		return integral_( integral_.width( ) - 1, integral_.height( ) - 1 );
	}

	/// @brief 画像配列のi方向のサイズを返す
	//! 
	//! @return i方向のサイズ
	//!
	size_t width( ) const
	{
		return integral_.width( ) - 1;
	}

	/// @brief 画像配列のj方向のサイズを返す
	//! 
	//! @return j方向のサイズ
	//!
	size_t height( ) const
	{
		return integral_.height( ) - 1;
	}

	/// @brief 画像配列のサイズを返す
	//! 
	//! @return サイズ
	//!
	size_t size( ) const
	{
		return width( ) * height( );
	}

	/// @brief Integral Image の構築：O(n)
	//! 
	//! @param[in] in … 画像配列
	//!
	void construct_integral_array( const mist::array2< Value_type > &in )
	{ 
		integral_.resize( in.width( ) + 1, in.height( ) + 1 );	
		for( size_t j = 1 ; j < integral_.height( ) ; j ++ )
		{
			for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
			{
				integral_( i, j ) = in( i - 1, j - 1 ) + integral_( i, j - 1 ) + integral_( i - 1, j ) - integral_( i - 1, j - 1 );
			}
		}
	}

	/// @brief コンストラクタ
	//! 
	//! @param[in] in … 画像配列
	//!
	integral_image( const mist::array2< Value_type > &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief デフォルトコンストラクタ
	//!
	integral_image( ) : integral_( mist::array2< integral_type >( ) )
	{
	}
};


template< typename Value_type >
class integral_image< mist::array3< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array3< integral_type > integral_;

	integral_type _value( const size_t i, const size_t j, const size_t k ) const
	{
		return integral_( i, j, k );
	}

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
	integral_type operator ( )( const size_t begin_i, const size_t begin_j, const size_t begin_k, const size_t width, const size_t height, const size_t depth ) const
	{
		return( _value( begin_i + width, begin_j + height, begin_k + depth ) + _value( begin_i, begin_j, begin_k + depth ) + _value( begin_i, begin_j + height, begin_k ) + _value( begin_i + width, begin_j, begin_k ) - _value( begin_i, begin_j + height, begin_k + depth ) - _value( begin_i + width, begin_j, begin_k + depth ) - _value( begin_i + width, begin_j + height, begin_k ) - _value( begin_i, begin_j, begin_k ) );
	}

	/// @brief 全ての画素値の総和を返す：O(1)
	//! 
	//! @return 画素値の総和
	//!
	integral_type overall( ) const 
	{
		return integral_( integral_.width( ) - 1, integral_.height( ) - 1, integral_.depth( ) - 1 );
	}

	/// @brief 画像配列のi方向のサイズを返す
	//! 
	//! @return i方向のサイズ
	//!
	size_t width( ) const
	{
		return integral_.width( ) - 1;
	}

	/// @brief 画像配列のj方向のサイズを返す
	//! 
	//! @return j方向のサイズ
	//!
	size_t height( ) const
	{
		return integral_.height( ) - 1;
	}

	/// @brief 画像配列のk方向のサイズを返す
	//! 
	//! @return k方向のサイズ
	//!
	size_t depth( ) const
	{
		return integral_.depth( ) - 1;
	}

	/// @brief 画像配列のサイズを返す
	//! 
	//! @return サイズ
	//!
	size_t size( ) const
	{
		return width( ) * height( ) * depth( );
	}

	/// @brief Integral Image の構築：O(n)
	//! 
	//! @param[in] in … 画像配列
	//!
	void construct_integral_array( const mist::array3< Value_type > &in )
	{
		integral_.resize( in.width( ) + 1, in.height( ) + 1, in.depth( ) + 1 );	
		for( size_t k = 1 ; k < integral_.depth( ) ; k ++  )
		{
			for( size_t j = 1 ; j < integral_.height( ) ; j ++ )
			{
				for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
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
	integral_image( const mist::array3< Value_type > &in )
	{
		construct_integral_array( in );
	}

	/// @brief デフォルトコンストラクタ
	//!
	integral_image( ) : integral_( mist::array3< integral_type >( ) )
	{
	}
};


/// @}
//  Integral Imageグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_INTEGRAL_IMAGE_H__
