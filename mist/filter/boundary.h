/// @file mist/filter/labeling.h
//!
//! @brief 各次元の画像に対応した，ラベリングアルゴリズム
//! 
//! @section 参考文献
//! -# 鳥脇純一郎, "3次元ディジタル画像処理," 昭晃堂, 2002
//!

#ifndef __INCLUDE_MIST_BOUNDARY__
#define __INCLUDE_MIST_BOUNDARY__

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


namespace __boundary_controller__
{
	template < int nc >
	struct neighbors
	{
		template < class Array >
		static inline typename Array::size_type neighbor( Array &in, const typename Array::size_type i[ 4 ], const typename Array::size_type j[ 4 ], const typename Array::size_type k[ 4 ] )
		{
			return( 0 );
		}
	};

	template < >
	struct neighbors< 26 >
	{
		template < class Array >
		static inline typename Array::size_type neighbor( Array &in, const typename Array::size_type i[ 4 ], const typename Array::size_type j[ 4 ], const typename Array::size_type k[ 4 ] )
		{
			typedef typename Array::size_type size_type;

			size_type count;

			count  = in( i[ 0 ], j[ 0 ], k[ 0 ] );	// -1, -1, -1
			count *= in( i[ 1 ], j[ 0 ], k[ 0 ] );	//  0, -1, -1
			count *= in( i[ 2 ], j[ 0 ], k[ 0 ] );	// +1, -1, -1
			count *= in( i[ 0 ], j[ 1 ], k[ 0 ] );	// -1,  0, -1
			count *= in( i[ 1 ], j[ 1 ], k[ 0 ] );	//  0,  0, -1
			count *= in( i[ 2 ], j[ 1 ], k[ 0 ] );	// +1,  0, -1
			count *= in( i[ 0 ], j[ 2 ], k[ 0 ] );	// -1, +1, -1
			count *= in( i[ 1 ], j[ 2 ], k[ 0 ] );	//  0, +1, -1
			count *= in( i[ 2 ], j[ 2 ], k[ 0 ] );	// +1, +1, -1

			count *= in( i[ 0 ], j[ 0 ], k[ 1 ] );	// -1, -1,  0
			count *= in( i[ 1 ], j[ 0 ], k[ 1 ] );	//  0, -1,  0
			count *= in( i[ 2 ], j[ 0 ], k[ 1 ] );	// +1, -1,  0
			count *= in( i[ 0 ], j[ 1 ], k[ 1 ] );	// -1,  0,  0
//			count *= in( i[ 1 ], j[ 1 ], k[ 1 ] );	//  0,  0,  0
			count *= in( i[ 2 ], j[ 1 ], k[ 1 ] );	// +1,  0,  0
			count *= in( i[ 0 ], j[ 2 ], k[ 1 ] );	// -1, +1,  0
			count *= in( i[ 1 ], j[ 2 ], k[ 1 ] );	//  0, +1,  0
			count *= in( i[ 2 ], j[ 2 ], k[ 1 ] );	// +1, +1,  0

			count *= in( i[ 0 ], j[ 0 ], k[ 2 ] );	// -1, -1, +1
			count *= in( i[ 1 ], j[ 0 ], k[ 2 ] );	//  0, -1, +1
			count *= in( i[ 2 ], j[ 0 ], k[ 2 ] );	// +1, -1, +1
			count *= in( i[ 0 ], j[ 1 ], k[ 2 ] );	// -1,  0, +1
			count *= in( i[ 1 ], j[ 1 ], k[ 2 ] );	//  0,  0, +1
			count *= in( i[ 2 ], j[ 1 ], k[ 2 ] );	// +1,  0, +1
			count *= in( i[ 0 ], j[ 2 ], k[ 2 ] );	// -1, +1, +1
			count *= in( i[ 1 ], j[ 2 ], k[ 2 ] );	//  0, +1, +1
			count *= in( i[ 2 ], j[ 2 ], k[ 2 ] );	// +1, +1, +1

			count *= i[ 3 ] * j[ 3 ] * k[ 3 ];

			return( count );
		}
	};

	template < >
	struct neighbors< 18 >
	{
		template < class Array >
		static inline typename Array::size_type neighbor( Array &in, const typename Array::size_type i[ 4 ], const typename Array::size_type j[ 4 ], const typename Array::size_type k[ 4 ] )
		{
			typedef typename Array::size_type size_type;

			size_type count;

			count  = in( i[ 1 ], j[ 0 ], k[ 0 ] );	//  0, -1, -1
			count *= in( i[ 0 ], j[ 1 ], k[ 0 ] );	// -1,  0, -1
			count *= in( i[ 1 ], j[ 1 ], k[ 0 ] );	//  0,  0, -1
			count *= in( i[ 2 ], j[ 1 ], k[ 0 ] );	// +1,  0, -1
			count *= in( i[ 1 ], j[ 2 ], k[ 0 ] );	//  0, +1, -1

			count *= in( i[ 0 ], j[ 0 ], k[ 1 ] );	// -1, -1,  0
			count *= in( i[ 1 ], j[ 0 ], k[ 1 ] );	//  0, -1,  0
			count *= in( i[ 2 ], j[ 0 ], k[ 1 ] );	// +1, -1,  0
			count *= in( i[ 0 ], j[ 1 ], k[ 1 ] );	// -1,  0,  0
//			count *= in( i[ 1 ], j[ 1 ], k[ 1 ] );	//  0,  0,  0
			count *= in( i[ 2 ], j[ 1 ], k[ 1 ] );	// +1,  0,  0
			count *= in( i[ 0 ], j[ 2 ], k[ 1 ] );	// -1, +1,  0
			count *= in( i[ 1 ], j[ 2 ], k[ 1 ] );	//  0, +1,  0
			count *= in( i[ 2 ], j[ 2 ], k[ 1 ] );	// +1, +1,  0

			count *= in( i[ 1 ], j[ 0 ], k[ 2 ] );	//  0, -1, +1
			count *= in( i[ 0 ], j[ 1 ], k[ 2 ] );	// -1,  0, +1
			count *= in( i[ 1 ], j[ 1 ], k[ 2 ] );	//  0,  0, +1
			count *= in( i[ 2 ], j[ 1 ], k[ 2 ] );	// +1,  0, +1
			count *= in( i[ 1 ], j[ 2 ], k[ 2 ] );	//  0, +1, +1

			count *= i[ 3 ] * j[ 3 ] * k[ 3 ];

			return( count );
		}
	};

	template < >
	struct neighbors< 6 >
	{
		template < class Array >
		static inline typename Array::size_type neighbor( Array &in, const typename Array::size_type i[ 4 ], const typename Array::size_type j[ 4 ], const typename Array::size_type k[ 4 ] )
		{
			typedef typename Array::size_type size_type;

			size_type count;

			count  = in( i[ 1 ], j[ 1 ], k[ 0 ] );	//  0,  0, -1
			count *= in( i[ 1 ], j[ 0 ], k[ 1 ] );	//  0, -1,  0
			count *= in( i[ 0 ], j[ 1 ], k[ 1 ] );	// -1,  0,  0
//			count *= in( i[ 1 ], j[ 1 ], k[ 1 ] );	//  0,  0,  0
			count *= in( i[ 2 ], j[ 1 ], k[ 1 ] );	// +1,  0,  0
			count *= in( i[ 1 ], j[ 2 ], k[ 1 ] );	//  0, +1,  0
			count *= in( i[ 1 ], j[ 1 ], k[ 2 ] );	//  0,  0, +1

			count *= i[ 3 ] * j[ 3 ] * k[ 3 ];

			return( count );
		}
	};

	template < >
	struct neighbors< 8 >
	{
		template < class Array >
		static inline typename Array::size_type neighbor( Array &in, const typename Array::size_type i[ 4 ], const typename Array::size_type j[ 4 ], const typename Array::size_type k[ 4 ] )
		{
			typedef typename Array::size_type size_type;

			size_type count;

			count  = in( i[ 0 ], j[ 0 ], k[ 1 ] );	// -1, -1,  0
			count *= in( i[ 1 ], j[ 0 ], k[ 1 ] );	//  0, -1,  0
			count *= in( i[ 2 ], j[ 0 ], k[ 1 ] );	// +1, -1,  0
			count *= in( i[ 0 ], j[ 1 ], k[ 1 ] );	// -1,  0,  0
//			count *= in( i[ 1 ], j[ 1 ], k[ 1 ] );	//  0,  0,  0
			count *= in( i[ 2 ], j[ 1 ], k[ 1 ] );	// +1,  0,  0
			count *= in( i[ 0 ], j[ 2 ], k[ 1 ] );	// -1, +1,  0
			count *= in( i[ 1 ], j[ 2 ], k[ 1 ] );	//  0, +1,  0
			count *= in( i[ 2 ], j[ 2 ], k[ 1 ] );	// +1, +1,  0

			count *= i[ 3 ] * j[ 3 ];

			return( count );
		}
	};

	template < >
	struct neighbors< 4 >
	{
		template < class Array >
		static inline typename Array::size_type neighbor( Array &in, const typename Array::size_type i[ 4 ], const typename Array::size_type j[ 4 ], const typename Array::size_type k[ 4 ] )
		{
			typedef typename Array::size_type size_type;

			size_type count;

			count  = in( i[ 1 ], j[ 0 ], k[ 1 ] );	//  0, -1,  0
			count *= in( i[ 0 ], j[ 1 ], k[ 1 ] );	// -1,  0,  0
//			count *= in( i[ 1 ], j[ 1 ], k[ 1 ] );	//  0,  0,  0
			count *= in( i[ 2 ], j[ 1 ], k[ 1 ] );	// +1,  0,  0
			count *= in( i[ 1 ], j[ 2 ], k[ 1 ] );	//  0, +1,  0

			count *= i[ 3 ] * j[ 3 ];

			return( count );
		}
	};


	template < class Array, class neighbor, class Functor >
	typename Array::size_type boundary( Array &in, typename Array::value_type value, const neighbor dmy, Functor f )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;

		size_type i, j, k;
		size_type ii[ 4 ], jj[ 4 ], kk[ 4 ];

		const size_type width = in.width( );
		const size_type height = in.height( );
		const size_type depth = in.depth( );

		const bool bprogress1 = depth == 1;
		const bool bprogress2 = depth >  1;

		f( 0.0 );

		size_type count = 0;

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			in[ i ] = in[ i ] == 0 ? 0 : 1;
		}

		for( k = 0 ; k < depth ; k++ )
		{
			kk[ 0 ] = k == 0 ? k : k - 1;
			kk[ 1 ] = k;
			kk[ 2 ] = k == depth - 1 ? k : k + 1;
			kk[ 3 ] = 1 < k && k < depth - 1 ? 1 : 0;

			for( j = 0 ; j < height ; j++ )
			{
				jj[ 0 ] = j == 0 ? j : j - 1;
				jj[ 1 ] = j;
				jj[ 2 ] = j == height - 1 ? j : j + 1;
				jj[ 3 ] = 1 < j && j < height - 1 ? 1 : 0;

				for( i = 0 ; i < width ; i++ )
				{
					ii[ 0 ] = i == 0 ? i : i - 1;
					ii[ 1 ] = i;
					ii[ 2 ] = i == width - 1 ? i : i + 1;
					ii[ 3 ] = 1 < i && i < width - 1 ? 1 : 0;

					if( in( i, j, k ) != 0 )
					{
						if( neighbor::neighbor( in, ii, jj, kk ) == 0 )
						{
							in( i, j, k ) = 2;
							count++;
						}
					}
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( height ) * 100.0 );
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( depth ) * 100.0 );
			}
		}

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			in[ i ] = in[ i ] == 2 ? value : 0;
		}

		f( 100.1 );

		return( count );
	}
}





//! @addtogroup boundary_group 境界画素抽出
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/boundary.h>
//! @endcode
//!
//!  @{


/// @brief 背景と4近傍で接する境界画素を抽出
//! 
//! 背景と4近傍で接する境界を抽出する
//! 抽出された境界は，8連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! @param[in]  f     … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator, class Functor >
typename array2< T, Allocator >::size_type boundary4( array2< T, Allocator > &in, typename array2< T, Allocator >::value_type value, Functor f )
{
	return( __boundary_controller__::boundary( in, value, __boundary_controller__::neighbors< 4 >( ), f ) );
}


/// @brief 背景と4近傍で接する境界画素を抽出
//! 
//! 背景と4近傍で接する境界を抽出する
//! 抽出された境界は，8連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator >
inline typename array2< T, Allocator >::size_type boundary4( array2< T, Allocator > &in, typename array2< T, Allocator >::value_type value )
{
	return( boundary4( in, value, __mist_dmy_callback__( ) ) );
}



/// @brief 背景と8近傍で接する境界画素を抽出
//! 
//! 背景と8近傍で接する境界を抽出する
//! 抽出された境界は，4連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! @param[in]  f     … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator, class Functor >
typename array2< T, Allocator >::size_type boundary8( array2< T, Allocator > &in, typename array2< T, Allocator >::value_type value, Functor f )
{
	return( __boundary_controller__::boundary( in, value, __boundary_controller__::neighbors< 8 >( ), f ) );
}


/// @brief 背景と8近傍で接する境界画素を抽出
//! 
//! 背景と8近傍で接する境界を抽出する
//! 抽出された境界は，4連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator >
inline typename array2< T, Allocator >::size_type boundary8( array2< T, Allocator > &in, typename array2< T, Allocator >::value_type value )
{
	return( boundary8( in, value, __mist_dmy_callback__( ) ) );
}


/// @brief 背景と6近傍で接する境界画素を抽出
//! 
//! 背景と6近傍で接する境界を抽出する
//! 抽出された境界は，26連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! @param[in]  f     … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator, class Functor >
typename array3< T, Allocator >::size_type boundary6( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type value, Functor f )
{
	return( __boundary_controller__::boundary( in, value, __boundary_controller__::neighbors< 6 >( ), f ) );
}


/// @brief 背景と6近傍で接する境界画素を抽出
//! 
//! 背景と6近傍で接する境界を抽出する
//! 抽出された境界は，26連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator >
inline typename array3< T, Allocator >::size_type boundary6( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type value )
{
	return( boundary6( in, value, __mist_dmy_callback__( ) ) );
}


/// @brief 背景と18近傍で接する境界画素を抽出
//! 
//! 背景と18近傍で接する境界を抽出する
//! 抽出された境界は，6'連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! @param[in]  f     … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator, class Functor >
typename array3< T, Allocator >::size_type boundary18( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type value, Functor f )
{
	return( __boundary_controller__::boundary( in, value, __boundary_controller__::neighbors< 18 >( ), f ) );
}


/// @brief 背景と18近傍で接する境界画素を抽出
//! 
//! 背景と18近傍で接する境界を抽出する
//! 抽出された境界は，6'連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator >
inline typename array3< T, Allocator >::size_type boundary18( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type value )
{
	return( boundary18( in, value, __mist_dmy_callback__( ) ) );
}


/// @brief 背景と26近傍で接する境界画素を抽出
//! 
//! 背景と26近傍で接する境界を抽出する
//! 抽出された境界は，6連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! @param[in]  f     … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator, class Functor >
typename array3< T, Allocator >::size_type boundary26( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type value, Functor f )
{
	return( __boundary_controller__::boundary( in, value, __boundary_controller__::neighbors< 26 >( ), f ) );
}


/// @brief 背景と18近傍で接する境界画素を抽出
//! 
//! 背景と26近傍で接する境界を抽出する
//! 抽出された境界は，6連結となる
//! 
//! @param[in]  in    … 入力画像
//! @param[in]  value … 境界画素に代入する値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T, class Allocator >
inline typename array3< T, Allocator >::size_type boundary26( array3< T, Allocator > &in, typename array3< T, Allocator >::value_type value )
{
	return( boundary26( in, value, __mist_dmy_callback__( ) ) );
}



/// @}
//  境界画素抽出グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_BOUNDARY__
