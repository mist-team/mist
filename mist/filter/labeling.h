// 
// Copyright (c) 2003-2009, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/filter/labeling.h
//!
//! @brief 各次元の画像に対応した，ラベリングアルゴリズム
//! 
//! - 参考文献
//!   - 米倉達広, 横井茂樹, 鳥脇純一郎, 福村晃夫, "三次元ディジタル空間における図形の連結性とオイラー数," 電子通信学会論文誌D, J65-D, No.1, pp.80-87, 1982
//!   - 鳥脇純一郎, "3次元ディジタル画像処理," 昭晃堂, 2002
//!

#ifndef __INCLUDE_MIST_LABELING__
#define __INCLUDE_MIST_LABELING__

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#include <vector>



// mist名前空間の始まり
_MIST_BEGIN


namespace __labeling_controller__
{
	template < class T > struct default_label_num2
	{
		_MIST_CONST( size_t, value, 2560 );
	};
	template < > struct default_label_num2< char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num2< signed char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num2< unsigned char >
	{
		_MIST_CONST( size_t, value, 255 );
	};

	template < class T > struct default_label_num3
	{
		_MIST_CONST( size_t, value, 10000 );
	};
	template < > struct default_label_num3< char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num3< signed char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num3< unsigned char >
	{
		_MIST_CONST( size_t, value, 255 );
	};


	template < int nc >
	struct neighbors
	{
		_MIST_CONST( size_t, array_num, 13 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[  0 ] =                           k > 0 ? T[ static_cast< size_type >( in( i    , j    , k - 1 ) ) ] : 0;
			L[  1 ] =              j     > 0          ? T[ static_cast< size_type >( in( i    , j - 1, k     ) ) ] : 0;
			L[  2 ] = i     > 0                       ? T[ static_cast< size_type >( in( i - 1, j    , k     ) ) ] : 0;
			L[  3 ] =              j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i    , j - 1, k - 1 ) ) ] : 0;
			L[  4 ] = i     > 0 &&              k > 0 ? T[ static_cast< size_type >( in( i - 1, j    , k - 1 ) ) ] : 0;
			L[  5 ] = i + 1 < w &&              k > 0 ? T[ static_cast< size_type >( in( i + 1, j    , k - 1 ) ) ] : 0;
			L[  6 ] =              j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i    , j + 1, k - 1 ) ) ] : 0;
			L[  7 ] = i     > 0 && j     > 0          ? T[ static_cast< size_type >( in( i - 1, j - 1, k     ) ) ] : 0;
			L[  8 ] = i + 1 < w && j     > 0          ? T[ static_cast< size_type >( in( i + 1, j - 1, k     ) ) ] : 0;
			L[  9 ] = i     > 0 && j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i - 1, j - 1, k - 1 ) ) ] : 0;
			L[ 10 ] = i + 1 < w && j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i + 1, j - 1, k - 1 ) ) ] : 0;
			L[ 11 ] = i     > 0 && j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i - 1, j + 1, k - 1 ) ) ] : 0;
			L[ 12 ] = i + 1 < w && j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i + 1, j + 1, k - 1 ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 18 >
	{
		_MIST_CONST( size_t, array_num, 9 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[  0 ] =                           k > 0 ? T[ static_cast< size_type >( in( i    , j    , k - 1 ) ) ] : 0;
			L[  1 ] =              j     > 0          ? T[ static_cast< size_type >( in( i    , j - 1, k     ) ) ] : 0;
			L[  2 ] = i     > 0                       ? T[ static_cast< size_type >( in( i - 1, j    , k     ) ) ] : 0;
			L[  3 ] =              j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i    , j - 1, k - 1 ) ) ] : 0;
			L[  4 ] = i     > 0 &&              k > 0 ? T[ static_cast< size_type >( in( i - 1, j    , k - 1 ) ) ] : 0;
			L[  5 ] = i + 1 < w &&              k > 0 ? T[ static_cast< size_type >( in( i + 1, j    , k - 1 ) ) ] : 0;
			L[  6 ] =              j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i    , j + 1, k - 1 ) ) ] : 0;
			L[  7 ] = i     > 0 && j     > 0          ? T[ static_cast< size_type >( in( i - 1, j - 1, k     ) ) ] : 0;
			L[  8 ] = i + 1 < w && j     > 0          ? T[ static_cast< size_type >( in( i + 1, j - 1, k     ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 6 >
	{
		_MIST_CONST( size_t, array_num, 3 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[  0 ] =                           k > 0 ? T[ static_cast< size_type >( in( i    , j    , k - 1 ) ) ] : 0;
			L[  1 ] =              j     > 0          ? T[ static_cast< size_type >( in( i    , j - 1, k     ) ) ] : 0;
			L[  2 ] = i     > 0                       ? T[ static_cast< size_type >( in( i - 1, j    , k     ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 8 >
	{
		_MIST_CONST( size_t, array_num, 4 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[ 0 ] = i     > 0 && j > 0 ? T[ static_cast< size_type >( in( i - 1, j - 1 ) ) ] : 0;
			L[ 1 ] =              j > 0 ? T[ static_cast< size_type >( in( i    , j - 1 ) ) ] : 0;
			L[ 2 ] = i + 1 < w && j > 0 ? T[ static_cast< size_type >( in( i + 1, j - 1 ) ) ] : 0;
			L[ 3 ] = i     > 0          ? T[ static_cast< size_type >( in( i - 1, j     ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 4 >
	{
		_MIST_CONST( size_t, array_num, 2 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[ 0 ] = j > 0 ? T[ static_cast< size_type >( in( i    , j - 1 ) ) ] : 0;
			L[ 1 ] = i > 0 ? T[ static_cast< size_type >( in( i - 1, j     ) ) ] : 0;

			return( 0 );
		}
	};


	template < class Array, class neighbor, class Functor >
	typename Array::size_type labeling( Array &in, typename Array::size_type label_max, const neighbor dmy, Functor f )
	{
		typedef typename Array::size_type size_type;
		typedef typename Array::difference_type difference_type;
		typedef typename Array::value_type value_type;

//		typedef difference_type label_value_type;
//		typedef size_type label_value_type;
		typedef unsigned int label_value_type;

		typedef std::vector< label_value_type >::value_type vector_label_value_type;

		size_type label_num = 0;
		size_type i, j, k, l, count;

		std::vector< label_value_type > T;
		label_value_type L[ neighbor::array_num ];
		const size_type width = in.width( );
		const size_type height = in.height( );
		const size_type depth = in.depth( );

		T.reserve( label_max );
		T.push_back( 0 );	// T[ 0 ]

		const bool bprogress1 = depth == 1;
		const bool bprogress2 = depth >  1;

		if( is_float< value_type >::value )
		{
			label_max = type_limits< size_type >::maximum( );
		}
		else
		{
			label_max = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}

		f( 0.0 );

		for( k = 0 ; k < depth ; k++ )
		{
			for( j = 0 ; j < height ; j++ )
			{
				for( i = 0 ; i < width ; i++ )
				{
					// 0画素はラベリングしない
					if( in( i, j, k ) == 0 )
					{
						continue;
					}

					// 操作済みの要素のラベルを取得する
					neighbor::neighbor( in, T, L, i, j, k, width, height, depth );

					// 近傍で最小のラベル番号を持つものを取得し，ラベル番号が0で無い物の数を数え上げる
					label_value_type L1 = static_cast< label_value_type >( label_max );
					for( l = 0, count = 0 ; l < neighbor::array_num ; l++ )
					{
						if( L[ l ] > 0 )
						{
							if( L1 != L[ l ] )
							{
								count++;
							}
							if( L1 > L[ l ] )
							{
								L1 = L[ l ];
							}
						}
					}

					if( count == 0 )
					{
						// 近傍に，すでに割り当てられているラベルは存在しないため，新規にラベルを割り当てる
						// 出力ラベル値が出力データ型の最大値を超えなければ，ラベル数を更新する
						if( label_num < label_max )
						{
							label_num++;
						}
						T.push_back( static_cast< label_value_type >( label_num ) );

						in( i, j, k ) = static_cast< value_type >( label_num );
					}
					else if( count == 1 )
					{
						// 近傍に，割り当てられているラベルが1つしか存在しないため，そのラベルを割り当てる
						in( i, j, k ) = static_cast< value_type >( L1 );
					}
					else
					{
						in( i, j, k ) = static_cast< value_type >( L1 );

						// 複数のラベルが結合するため，テーブルを修正する
						for( l = 0 ; l < neighbor::array_num ; l++ )
						{
							if( T[ L[ l ] ] != L1 )
							{
								for( size_type m = 1 ; m <= label_num ; m++ )
								{
									if( T[ m ] == L[ l ] )
									{
										T[ m ] = static_cast< vector_label_value_type >( L1 );
									}
								}
							}
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

		f( 100.0 );

		// ラベルの振り直しを行う
		size_type *NT = new size_type[ label_num + 1 ];
		for( i = 0 ; i <= label_num ; i++ )
		{
			NT[ i ] = 0;
		}

		// 使用しているラベルをチェックする
		for( i = 1 ; i <= label_num ; i++ )
		{
			NT[ T[ i ] ] = 1;
		}

		// 使用しているラベルをチェックする
		for( i = 1, count = 1 ; i <= label_num ; i++ )
		{
			if( NT[ i ] != 0 )
			{
				NT[ i ] = count++;
			}
		}

		label_num = 0;
		for( i = 0 ; i < in.size( ) ; i++ )
		{
			if( in[ i ] == 0 )
			{
				continue;
			}

			size_type label = NT[ T[ static_cast< size_type >( in[ i ] ) ] ];
			in[ i ] = static_cast< value_type >( label );
			if( label_num < label )
			{
				label_num = label;
			}
		}

		f( 100.1 );

		delete [] NT;

		return( label_num );
	}
}




//! @addtogroup labeling_group ラベリング
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/labeling.h>
//! @endcode
//!
//!  @{


/// @brief 2次元画像に対する4近傍型ラベリング
//! 
//! 2次元画像に対する4近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! @param[in]  f         … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array2< T2, Allocator2 >::size_type labeling4( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 4 >( ), f ) );
}


/// @brief 2次元画像に対する4近傍型ラベリング
//! 
//! 2次元画像に対する4近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling4( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	return( labeling4( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 2次元画像に対する8近傍型ラベリング
//! 
//! 2次元画像に対する8近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! @param[in]  f         … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 8 >( ), f ) );
}


/// @brief 2次元画像に対する8近傍型ラベリング
//! 
//! 2次元画像に対する8近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	return( labeling8( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 3次元画像に対する6近傍型ラベリング
//! 
//! 3次元画像に対する6近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! @param[in]  f         … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array3< T2, Allocator2 >::size_type labeling6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 6 >( ), f ) );
}

/// @brief 3次元画像に対する6近傍型ラベリング
//! 
//! 3次元画像に対する6近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array3< T2, Allocator2 >::size_type labeling6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	return( labeling6( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 3次元画像に対する18近傍型ラベリング
//! 
//! 3次元画像に対する18近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! @param[in]  f         … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array3< T2, Allocator2 >::size_type labeling18( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 18 >( ), f ) );
}

/// @brief 3次元画像に対する18近傍型ラベリング
//! 
//! 3次元画像に対する18近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array3< T2, Allocator2 >::size_type labeling18( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	return( labeling18( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 3次元画像に対する26近傍型ラベリング
//! 
//! 3次元画像に対する26近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! @param[in]  f         … 進行状況を返すコールバック関数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 26 >( ), f ) );
}


/// @brief 3次元画像に対する26近傍型ラベリング
//! 
//! 3次元画像に対する26近傍型ラベリング
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
//! 
//! ただし，データ型が char 型を利用する場合は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
//! また，最大ラベル数を超えた場合は自動的に最大ラベル数を更新する．そのため，メモリ使用量が増加する可能性がある．
//! 初期値の max_label を大きくとることでメモリの再確保が減り高速に動作するようになる（そのかわり，初期メモリ量が増加する）．
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数のデフォルト値
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	return( labeling26( in, out, max_label, __mist_dmy_callback__( ) ) );
}




/// @brief ある範囲内での最大のラベルを抽出する
//! 
//! 2次元画像に対する8近傍型ラベリングを用いて，最大連結成分を抽出する
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  sx        … X軸方向の小さい側の位置
//! @param[in]  ex        … X軸方向の大きい側の位置
//! @param[in]  sy        … Y軸方向の小さい側の位置
//! @param[in]  ey        … Y軸方向の大きい側の位置
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region(
						const array2< T1, Allocator1 > &in,
						array2< T2, Allocator2 > &out,
						typename array2< T1, Allocator1 >::size_type sx,
						typename array2< T1, Allocator1 >::size_type ex,
						typename array2< T1, Allocator1 >::size_type sy,
						typename array2< T1, Allocator1 >::size_type ey,
						typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value
					)
{
	typedef typename array2< T1, Allocator1 >::size_type size_type;
	typedef typename array2< T1, Allocator1 >::difference_type difference_type;

	array2< size_type > tmp;
	size_type i, j;

	size_type label_num = mist::labeling8( in, tmp, max_label );

	// 指定された範囲内の最大ラベルを探索
	sx = sx < 0 ? 0 : sx;
	sy = sy < 0 ? 0 : sy;
	sx = sx < in.width( )  ? sx : in.width( )  - 1;
	sy = sy < in.height( ) ? sy : in.height( ) - 1;

	ex = ex < 0 ? 0 : ex;
	ey = ey < 0 ? 0 : ey;
	ex = ex < in.width( )  ? ex : in.width( )  - 1;
	ey = ey < in.height( ) ? ey : in.height( ) - 1;

	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( j = sy ; j <= ey ; j++ )
	{
		for( i = sx ; i <= ex ; i++ )
		{
			menseki[ tmp( i, j ) ]++;
		}
	}

	max_label = 1;
	for( i = 2 ; i <= label_num ; i++ )
	{
		max_label = menseki[ i ] > menseki[ max_label ] ? i : max_label;
	}
	delete [] menseki;

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = tmp[ i ] == max_label ? 1 : 0;
	}
}


/// @brief 画像の全範囲内での最大のラベルを抽出する
//! 
//! 3次元画像に対する8近傍型ラベリングを用いて，最大連結成分を抽出する
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	maximum_region( in, out, 0, in.width( ) - 1, 0, in.height( ) - 1, max_label );
}



/// @brief ある範囲内での最大のラベルを抽出する
//! 
//! 3次元画像に対する26近傍型ラベリングを用いて，最大連結成分を抽出する
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  sx        … X軸方向の小さい側の位置
//! @param[in]  ex        … X軸方向の大きい側の位置
//! @param[in]  sy        … Y軸方向の小さい側の位置
//! @param[in]  ey        … Y軸方向の大きい側の位置
//! @param[in]  sz        … Z軸方向の小さい側の位置
//! @param[in]  ez        … Z軸方向の大きい側の位置
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region(
						const array3< T1, Allocator1 > &in,
						array3< T2, Allocator2 > &out,
						typename array3< T1, Allocator1 >::size_type sx,
						typename array3< T1, Allocator1 >::size_type ex,
						typename array3< T1, Allocator1 >::size_type sy,
						typename array3< T1, Allocator1 >::size_type ey,
						typename array3< T1, Allocator1 >::size_type sz,
						typename array3< T1, Allocator1 >::size_type ez,
						typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value
					)
{
	typedef typename array3< T1, Allocator1 >::size_type size_type;
	typedef typename array3< T1, Allocator1 >::difference_type difference_type;

	array3< size_type > tmp;
	size_type i, j, k;

	size_type label_num = mist::labeling26( in, tmp, max_label );

	// 指定された範囲内の最大ラベルを探索
	sx = sx < 0 ? 0 : sx;
	sy = sy < 0 ? 0 : sy;
	sz = sz < 0 ? 0 : sz;
	sx = sx < in.width( )  ? sx : in.width( )  - 1;
	sy = sy < in.height( ) ? sy : in.height( ) - 1;
	sz = sz < in.depth( )  ? sz : in.depth( )  - 1;

	ex = ex < 0 ? 0 : ex;
	ey = ey < 0 ? 0 : ey;
	ez = ez < 0 ? 0 : ez;
	ex = ex < in.width( )  ? ex : in.width( )  - 1;
	ey = ey < in.height( ) ? ey : in.height( ) - 1;
	ez = ez < in.depth( )  ? ez : in.depth( )  - 1;

	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( k = sz ; k <= ez ; k++ )
	{
		for( j = sy ; j <= ey ; j++ )
		{
			for( i = sx ; i <= ex ; i++ )
			{
				menseki[ tmp( i, j, k ) ]++;
			}
		}
	}

	max_label = 1;
	for( i = 2 ; i <= label_num ; i++ )
	{
		max_label = menseki[ i ] > menseki[ max_label ] ? i : max_label;
	}
	delete [] menseki;

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( tmp[ i ] == max_label ? 1 : 0 );
	}
}


/// @brief 画像の全範囲内での最大のラベルを抽出する
//! 
//! 3次元画像に対する6近傍型ラベリングを用いて，最大連結成分を抽出する
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
//! @return 割り当てられたラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	maximum_region( in, out, 0, in.width( ) - 1, 0, in.height( ) - 1, 0, in.depth( ) - 1, max_label );
}




/// @brief 画像の0/1を反転させて穴埋め処理を行う
//! 
//! 2次元画像に対する4近傍型ラベリングを用いて，穴埋め処理を行う
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  include_corner_labels … whether to also remove holes of the labels containing the corners of the image
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const bool include_corner_labels, typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	typedef typename array2< T1, Allocator1 >::size_type size_type;
	typedef typename array2< T1, Allocator1 >::difference_type difference_type;

	array2< size_type > tmp;
	size_type i;

	tmp.resize( in.size1( ), in.size2( ) );
	tmp.reso1( in.reso1( ) );
	tmp.reso2( in.reso2( ) );

	// 反転した画像を作成する
	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		tmp[ i ] = in[ i ] == 0;
	}

	size_type label_num = mist::labeling4( tmp, tmp, max_label );

	if ( include_corner_labels )
	{
		size_type *L = new size_type[ label_num + 1 ];

		for( size_type i = 0 ; i <= label_num ; i++ )
		{
			L[ i ] = static_cast< size_type >( i );
		}

		L[ tmp( 0, 0 ) ] = 1;
		L[ tmp( tmp.width() - 1, 0 ) ] = 1;
		L[ tmp( 0, tmp.height() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, tmp.height() - 1 ) ] = 1;
		
		for( size_type i = 0 ; i < tmp.size( ) ; i++ )
		{
			tmp[ i ] = L[ tmp[ i ] ];
		}

		delete [] L;
	}

	// 指定された範囲内の最大ラベルを探索
	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		menseki[ tmp[ i ] ]++;
	}

	max_label = 1;
	for( i = 2 ; i <= label_num ; i++ )
	{
		max_label = menseki[ i ] > menseki[ max_label ] ? i : max_label;
	}
	delete [] menseki;

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	// 最大成分を残し，反転した画像を出力する
	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = tmp[ i ] == max_label ? 0 : 1;
	}
}


/// @brief 画像の0/1を反転させて穴埋め処理を行う
//! 
//! 2次元画像に対する4近傍型ラベリングを用いて，穴埋め処理を行う
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	remove_hole_region( in, out, false, max_label );
}


/// @brief 画像の0/1を反転させて穴埋め処理を行う
//! 
//! 3次元画像に対する4近傍型ラベリングを用いて，穴埋め処理を行う
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  include_corner_labels … whether to also remove holes of the labels containing the corners of the image
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const bool include_corner_labels, typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	typedef typename array2< T1, Allocator1 >::size_type size_type;
	typedef typename array2< T1, Allocator1 >::difference_type difference_type;

	array3< size_type > tmp;
	size_type i;

	tmp.resize( in.size1( ), in.size2( ), in.size3( ) );
	tmp.reso1( in.reso1( ) );
	tmp.reso2( in.reso2( ) );
	tmp.reso3( in.reso3( ) );

	// 反転した画像を作成する
	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		tmp[ i ] = in[ i ] == 0;
	}

	size_type label_num = mist::labeling6( tmp, tmp, max_label );

	if ( include_corner_labels )
	{
		size_type *L = new size_type[ label_num + 1 ];

		for( size_type i = 0 ; i <= label_num ; i++ )
		{
			L[ i ] = static_cast< size_type >( i );
		}

		L[ tmp( 0, 0, 0 ) ] = 1;
		L[ tmp( tmp.width() - 1, 0, 0 ) ] = 1;
		L[ tmp( 0, tmp.height() - 1, 0 ) ] = 1;
		L[ tmp( 0, 0, tmp.depth() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, tmp.height() - 1, 0 ) ] = 1;
		L[ tmp( 0, tmp.height() - 1, tmp.depth() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, 0, tmp.depth() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, tmp.height() - 1, tmp.depth() - 1 ) ] = 1;
		
		for( size_type i = 0 ; i < tmp.size( ) ; i++ )
		{
			tmp[ i ] = L[ tmp[ i ] ];
		}

		delete [] L;
	}

	// 指定された範囲内の最大ラベルを探索
	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		menseki[ tmp[ i ] ]++;
	}

	max_label = 1;
	for( i = 2 ; i <= label_num ; i++ )
	{
		max_label = menseki[ i ] > menseki[ max_label ] ? i : max_label;
	}
	delete [] menseki;

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	// 最大成分を残し，反転した画像を出力する
	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = tmp[ i ] == max_label ? 0 : 1;
	}
}

/// @brief 画像の0/1を反転させて穴埋め処理を行う
//! 
//! 3次元画像に対する4近傍型ラベリングを用いて，穴埋め処理を行う
//! 
//! @attention 入力と出力が同じ画像オブジェクトでも正しく動作する
//! 
//! @param[in]  in        … 入力画像
//! @param[out] out       … 出力画像
//! @param[in]  max_label … 最大で割り当てるラベル数
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	remove_hole_region( in, out, false, max_label );
}

namespace __he__
{
	template< typename L >
	inline void resolve( const L u, const L v, mist::array< L > &rl_table, mist::array< L > &next_label, mist::array< L > &tail_label )
	{
		typedef L label_type;
		if( rl_table[ u ] != rl_table[ v ] )
		{
			label_type lu, lv;
			if( rl_table[ u ] < rl_table[ v ] )
			{
				lu = rl_table[ u ];
				lv = rl_table[ v ];
			}
			else
			{
				lu = rl_table[ v ];
				lv = rl_table[ u ];
			}
			next_label[ tail_label[ lu ] ] = lv;
			tail_label[ lu ] = tail_label[ lv ];
			label_type i = lv;
			while( i != 0 )
			{
				rl_table[ i ] = lu;
				i = next_label[ i ];
			}
		}
	}

	template< typename L >
	inline void update( L &m, mist::array< L > &rl_table, mist::array< L > &next_label, mist::array< L > &tail_label )
	{
		rl_table[ m ] = m;
		tail_label[ m ] = m;
		m ++;
	}
}

namespace he
{

	/// @brief Heらの2次元画像に対する8近傍型ラベリング
	//! 
	//! Heらの2次元画像に対する8近傍型ラベリング
	//! 
	//! @attention 入力と出力は異なる画像オブジェクトである必要がある
	//! 
	//! 出力画像のデータ型は，ラベル数がオーバーフローしないように注意が必要なため，short や int を利用することをお勧めします．
	//! ここで，ラベル数とは最終的に得られる領域の数ではなく，作業中に発生する一時的なラベル数を指すため，データ型が char の場合にはオーバーフローする可能性が非常に大きくなります．
	//! この関数は，以下に挙げる文献のアルゴリズムを実装したものです。
	//! 何 立風, 巣 宇燕, 鈴木 賢治, 中村 剛士, 伊藤 英則, "高速2回走査ラベル付けアルゴリズム," 電子情報通信学会論文誌D, Vol.J91-D, No.4, pp.1016-1024, 2008.
	//! 
	//! @param[in]  in        … 入力画像
	//! @param[out] out       … 出力画像
	//! 
	//! @return 割り当てられたラベル数
	//! 
	template< typename V, typename L >
	inline L labeling8( const mist::array2< V > &b, mist::array2< L > &v )
	{
		typedef V value_type;
		typedef L label_type;
		const size_t size = ( ( b.width( ) + 1 ) / 2 ) * ( ( b.height( ) + 1 ) / 2 ); 
		mist::array< label_type > rl_table( size ), next_label( size ), tail_label( size );
		v.resize( b.width( ), b.height( ) );
		label_type m = 1;
		{
			const size_t j = 0;
			{
				const size_t i = 0;
				if( b( i, j ) != 0 )
				{
					v( i, j ) = m;
					__he__::update( m, rl_table, next_label, tail_label );
				}
			}
			for( size_t i = 1 ; i != b.width( ) ; ++ i )
			{
				const label_type &v1 = v( i - 1, j );
				if( b( i, j ) != 0 )
				{
					if( v1 != 0 )
					{
						v( i, j ) = v1;
					}
					else
					{
						v( i, j ) = m;
						__he__::update( m, rl_table, next_label, tail_label );
					}				
				}
			}
		}
		for( size_t j = 1 ; j != b.height( ) ; ++ j )
		{
			{
				const size_t i = 0;
				const label_type &v3 = v( i, j - 1 );
				const label_type &v4 = v( i + 1, j - 1 );
				if( b( i, j ) != 0 )
				{
					if( v3 != 0 )
					{
						v( i, j ) = v3;
					}
					else if( v4 != 0 )
					{
						v( i, j ) = v4;
					}
					else
					{
						v( i, j ) = m;
						__he__::update( m, rl_table, next_label, tail_label );
					}				
				}
			}
			for( size_t i = 1 ; i != b.width( ) - 1 ; ++ i )
			{
				const label_type &v1 = v( i - 1, j );
				const label_type &v2 = v( i - 1, j - 1 );
				const label_type &v3 = v( i, j - 1 );
				const label_type &v4 = v( i + 1, j - 1 );
				if( b( i, j ) != 0 )
				{
					if( v3 != 0 )
					{
						v( i, j ) = v3;
					}
					else if( v1 != 0 )
					{
						v( i, j ) = v1;
						if( v4 != 0 )
						{
							__he__::resolve( v1, v4, rl_table, next_label, tail_label );
						}
					}
					else if( v2 != 0 )
					{
						v( i, j ) = v2;
						if( v4 != 0 )
						{
							__he__::resolve( v2, v4, rl_table, next_label, tail_label );
						}
					}
					else if( v4 != 0 )
					{
						v( i, j ) = v4;
					}
					else
					{
						v( i, j ) = m;
						__he__::update( m, rl_table, next_label, tail_label );
					}				
				}
			}
			{
				const size_t i = b.width( ) - 1;
				const label_type &v1 = v( i - 1, j );
				const label_type &v2 = v( i - 1, j - 1 );
				const label_type &v3 = v( i, j - 1 );
				if( b( i, j ) != 0 )
				{
					if( v3 != 0 )
					{
						v( i, j ) = v3;
					}
					else if( v1 != 0 )
					{
						v( i, j ) = v1;
					}
					else if( v2 != 0 )
					{
						v( i, j ) = v2;
					}
					else
					{
						v( i, j ) = m;
						__he__::update( m, rl_table, next_label, tail_label );
					}				
				}
			}
		}
		mist::array< label_type > l_table( m );
		label_type l = 0;
		for( size_t i = 1 ; i != m ; ++ i )
		{
			if( l_table[ rl_table[ i ] ] == 0 )
			{
				l ++;
				l_table[ rl_table[ i ] ] = l;
			}
		}
		for( size_t j = 0 ; j != v.height( ) ; ++ j )
		{
			for( size_t i = 0 ; i != v.width( ) ; ++ i )
			{
				v( i, j ) = l_table[ rl_table[ v( i, j ) ] ];
			}
		}
		return l;
	}
}

/// @}
//  ラベリンググループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_LABELING__
