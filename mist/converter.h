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

/// @file mist/converter.h
//!
//! @brief 各画像の要素間を正しく変換するためのライブラリ
//!
#ifndef __INCLUDE_CONVERTER__
#define __INCLUDE_CONVERTER__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_BITMAP_H__
#include "bitmap.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


namespace __converter__
{
	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array< T1, Allocator1 > &in, mist::array< T2, Allocator2 > &out )
	{
		out.resize( in.size( ) );
	}

	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array1< T1, Allocator1 > &in, mist::array1< T2, Allocator2 > &out )
	{
		out.resize( in.size( ) );
		out.reso1( in.reso1( ) );
	}

	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array2< T1, Allocator1 > &in, mist::array2< T2, Allocator2 > &out )
	{
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
	}

	template < size_t T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::bitmap< T1, Allocator1 > &in, mist::array2< T2, Allocator2 > &out )
	{
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( 1.0 );
		out.reso2( 1.0 );
	}

	template < class T1, size_t T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array2< T1, Allocator1 > &in, mist::bitmap< T2, Allocator2 > &out )
	{
		out.resize( in.size1( ), in.size2( ) );
	}

	template < size_t T1, size_t T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::bitmap< T1, Allocator1 > &in, mist::bitmap< T2, Allocator2 > &out )
	{
		out.resize( in.size1( ), in.size2( ) );
	}

	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array3< T1, Allocator1 > &in, mist::array3< T2, Allocator2 > &out )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
	}

	template < bool b1, bool b2 >
	struct _converter_
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = static_cast< T2 >( in );
		}
	};

	template < >
	struct _converter_< true, false >
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = static_cast< T2 >( in.get_value( ) );
		}
	};

	template < >
	struct _converter_< false, true >
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = in;
		}
	};

	template < >
	struct _converter_< true, true >
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = in;
		}
	};

	template < class T1, class T2 >
	inline void converter( const T1 &in, T2 &out )
	{
		_converter_< is_color< T1 >::value, is_color< T2 >::value >::convert( in, out );
	}
}


//! @addtogroup image_converter_group 画像変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/converter.h>
//! @endcode
//!
//!  @{

/// @brief あるMISTコンテナから他のコンテナへの変換
//! 
//! コンテナ内の各要素を適切な変換操作を行って，他のコンテナへ変換する
//! @note カラー画像からグレースケール画像へも適切に変換する
//! 
//! @param[in] in  … 入力画像
//! @param[in] out … 出力画像
//! 
template < class Array1, class Array2 >
inline void convert( const Array1 &in, Array2 &out )
{
	typedef typename Array1::size_type  size_type;
	typedef typename Array1::value_type value_type;
	typedef typename Array2::value_type out_value_type;
	typedef unsigned short hist_value;

	__converter__::resize( in, out );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		__converter__::converter( in[ i ], out[ i ] );
	}
}

/// @}
//  画像変換グループの終わり


// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_CONVERTER__
