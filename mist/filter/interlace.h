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
		static bool interlace_odd( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行異常ない場合はインターレス除去はできない
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
		static bool interlace_even( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行異常ない場合はインターレス除去はできない
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
	};

	template < >
	struct interlace_controller< true >
	{
		template < class T, class Allocator >
		static bool interlace_odd( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行異常ない場合はインターレス除去はできない
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
		static bool interlace_even( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行異常ない場合はインターレス除去はできない
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
	};
}


//! @addtogroup interlace_group インターレス除去
//!  @{


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
bool interlace( const array2< T, Allocator > &in, array2< T, Allocator > &out, bool is_odd_line = false )
{
	if( is_odd_line )
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_odd( in, out ) );
	}
	else
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_even( in, out ) );
	}
}



/// @}
//  インターレス除去グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_INTERLACE__
