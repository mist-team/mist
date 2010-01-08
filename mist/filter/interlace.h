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

/// @file mist/filter/interlace.h
//!
//! @brief インターレスの解除を行うためのライブラリ
//!

#ifndef __INCLUDE_MIST_INTERLACE__
#define __INCLUDE_MIST_INTERLACE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif



// mist名前空間の始まり
_MIST_BEGIN


namespace __interlace_controller__
{
	template < bool b >
	struct interlace_controller
	{
		template < class T, class Allocator >
		static bool interlace_odd_row( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;

			// 奇数ラインを補間するため，先頭行を次の行で補完する
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				out( i, 0 ) = in( i, 1 );
			}
			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 2 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = static_cast< value_type >( ( in( i, j - 1 ) + in( i, j + 1 ) ) / 2.0 );
				}
			}
			// 最終行が奇数のため，1行前で補完する
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_odd_col( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.width( ) < 2 )
			{
				// 2列以上ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;

			// 奇数ラインを補間するため，先頭列を次の列で補完する
			for( i = 0 ; i < in.height( ) ; i++ )
			{
				out( 0, i ) = in( 1, i );
			}
			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 0 ; j < in.height( ) ; j++ )
			{
				for( i = 2 ; i < in.width( ) - 1 ; i += 2 )
				{
					out( i, j ) = static_cast< value_type >( ( in( i - 1, j ) + in( i + 1, j ) ) / 2.0 );
				}

				// 最終列が奇数のため，1列前で補完する
				if( i == in.width( ) - 1 )
				{
					out( i, j ) = in( i - 1, j );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even_row( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;

			// 各偶数行を前後の奇数行で線形に補完してうめる
			for( j = 1 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = static_cast< value_type >( ( in( i, j - 1 ) + in( i, j + 1 ) ) / 2.0 );
				}
			}
			// 最終行が偶数のため，1行前で補完する
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even_col( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.width( ) < 2 )
			{
				// 2列以上ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;

			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 0 ; j < in.height( ) ; j++ )
			{
				for( i = 1 ; i < in.width( ) - 1 ; i += 2 )
				{
					out( i, j ) = static_cast< value_type >( ( in( i - 1, j ) + in( i + 1, j ) ) / 2.0 );
				}

				// 最終列が偶数のため，1列前で補完する
				if( i == in.width( ) - 1 )
				{
					out( i, j ) = in( i - 1, j );
				}
			}
			return( true );
		}
	};

	template < >
	struct interlace_controller< true >
	{
		template < class T, class Allocator >
		static bool interlace_odd_row( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上無い場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			// 奇数ラインを補間するため，先頭行を次の行で補完する
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				out( i, 0 ) = in( i, 1 );
			}
			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 2 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					r = ( in( i, j - 1 ).r + in( i, j + 1 ).r ) / 2.0;
					g = ( in( i, j - 1 ).g + in( i, j + 1 ).g ) / 2.0;
					b = ( in( i, j - 1 ).b + in( i, j + 1 ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}
			}
			// 最終行が奇数のため，1行前で補完する
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_odd_col( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.width( ) < 2 )
			{
				// 2列以上ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			// 奇数ラインを補間するため，先頭列を次の列で補完する
			for( i = 0 ; i < in.height( ) ; i++ )
			{
				out( 0, i ) = in( 1, i );
			}
			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 0 ; j < in.height( ) ; j++ )
			{
				for( i = 2 ; i < in.width( ) - 1 ; i += 2 )
				{
					r = ( in( i - 1, j ).r + in( i + 1, j ).r ) / 2.0;
					g = ( in( i - 1, j ).g + in( i + 1, j ).g ) / 2.0;
					b = ( in( i - 1, j ).b + in( i + 1, j ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}

				// 最終列が奇数のため，1列前で補完する
				if( i == in.width( ) - 1 )
				{
					out( i, j ) = in( i - 1, j );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even_row( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上無い場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			for( j = 1 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					r = ( in( i, j - 1 ).r + in( i, j + 1 ).r ) / 2.0;
					g = ( in( i, j - 1 ).g + in( i, j + 1 ).g ) / 2.0;
					b = ( in( i, j - 1 ).b + in( i, j + 1 ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}
			}
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even_col( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.width( ) < 2 )
			{
				// 2列以上ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 0 ; j < in.height( ) ; j++ )
			{
				for( i = 1 ; i < in.width( ) - 1 ; i += 2 )
				{
					r = ( in( i - 1, j ).r + in( i + 1, j ).r ) / 2.0;
					g = ( in( i - 1, j ).g + in( i + 1, j ).g ) / 2.0;
					b = ( in( i - 1, j ).b + in( i + 1, j ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}

				// 最終列が偶数のため，1列前で補完する
				if( i == in.width( ) - 1 )
				{
					out( i, j ) = in( i - 1, j );
				}
			}
			return( true );
		}
	};
}


//! @addtogroup interlace_group インターレス除去
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/interlace.h>
//! @endcode
//!
//!  @{


/// @brief 2次元画像のインターレス除去を行う
//! 
//! 偶数ライン・奇数ラインのどちらかを補間する
//! @attention インターレス除去を行うためには，画像が2行以上でなくてはならない
//! 
//! @param[in]  in               … 入力画像
//! @param[out] out              … 出力画像
//! @param[in]  is_odd_line      … 奇数ラインを偶数ラインで補間する場合は true，偶数ラインを奇数ラインで補間する場合は false を指定する
//! @param[in]  is_row_interlace … 行方向を補間する場合は true，列方向を補間する場合は false を指定する
//! 
//! @return インターレス除去がうまくいったかどうか
//! 
template < class T, class Allocator >
bool interlace( const array2< T, Allocator > &in, array2< T, Allocator > &out, bool is_odd_line = false, bool is_row_interlace = true )
{
	if( is_odd_line )
	{
		if( is_row_interlace )
		{
			return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_odd_row( in, out ) );
		}
		else
		{
			return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_odd_col( in, out ) );
		}
	}
	else
	{
		if( is_row_interlace )
		{
			return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_even_row( in, out ) );
		}
		else
		{
			return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_even_col( in, out ) );
		}
	}
}


/// @}
//  インターレス除去グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_INTERLACE__
