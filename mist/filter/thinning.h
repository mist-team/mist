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

/// @file mist/filter/thinning.h
//!
//! @brief 各次元の画像に対応した，細線化アルゴリズム
//!
//! - 参考文献
//!   - 鳥脇純一郎, ``画像理解のためのディジタル画像処理〔I〕〔II〕，'' 昭晃堂，1988
//!   - 鈴木智, 阿部圭一, ``距離変換の結果を利用した二値画像の逐次細線化,'' 電子情報通信学会論文誌D, vol.68-D, no.4, pp.473-480, 1985
//!   - 米倉達広, 横井茂樹, 鳥脇純一郎, 福村晃夫, ``3次元ディジタル画像における1-要素の消去可能性と図形収縮について,'' 電子情報通信学会論文誌D, vol.65-D, no.12, pp.1543-1549, 1982
//!   - 斉藤豊文, 森健策, 鳥脇純一郎, ``ユークリッド距離変換を用いた3次元ディジタル画像の薄面化および細線化の逐次型アルゴリズムとその諸性質,'' 電子情報通信学会論文誌D-II, vol.J79-D-II, no.10, pp.1675-1685, 1996
//!

#ifndef __INCLUDE_MIST_THINNING__
#define __INCLUDE_MIST_THINNING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "../matrix.h"
#endif

#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#include "distance.h"
#endif

#include <vector>


// mist名前空間の始まり
_MIST_BEGIN


namespace __thinning_controller__
{
	template < class T, class Allocator >
	inline void val9( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
	typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
	{
		p[ 0 ] = in( i , j );
		p[ 1 ] = in( i + 1, j );
		p[ 2 ] = in( i + 1, j - 1 );
		p[ 3 ] = in( i , j - 1 );
		p[ 4 ] = in( i - 1, j - 1 );
		p[ 5 ] = in( i - 1, j );
		p[ 6 ] = in( i - 1, j + 1 );
		p[ 7 ] = in( i , j + 1 );
		p[ 8 ] = in( i + 1, j + 1 );
	}

	template < class T > inline T pixel( T p )
	{
		return ( ( p > 0 ) ? 1 : 0 );
	}

	template < class T > inline T pixel( T p[ 9 ], int index )
	{
		if( index <= 0 )
		{
			return ( pixel( p[ index + 8 ] ) );
		}
		else if( index >= 9 )
		{
			return ( pixel( p[ index - 8 ] ) );
		}
		else
		{
			return ( pixel( p[ index ] ) );
		}
	}

	template < class T > inline T pixel_( T p[ 9 ], int index )
	{
		return ( 1 - pixel( p, index ) );
	}

	template < class T > inline int Nc4( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel( p, i ) - pixel( p, i ) * pixel( p, i + 1 ) * pixel( p, i + 2 );
		}

		return ( ret );
	}

	template < class T > inline int Nc8( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel_( p, i ) - pixel_( p, i ) * pixel_( p, i + 1 ) * pixel_( p, i + 2 );
		}

		return ( ret );
	}

	// C. J. Hilditch, ``Linear Skeleton from Square Cupboards,'' In: Machine Intelligence 6, B. Meltzer and D. Michie eds., Edinburgh Univ. Press, pp.403?420, 1969
	// 鳥脇純一郎, ``画像理解のためのディジタル画像処理〔II〕，'' 昭晃堂，pp.56-59，1988
	template < class T, class Allocator >
	void thinning( array2< T, Allocator > &ia )
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		bool flag;
		size_type w = ia.width( );
		size_type h = ia.height( );
		size_type i, j;
		char p[ 9 ], q[ 9 ], value;
		array2< char > ic( w, h );
		array2< char > id( w, h );

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			id[ i ] = ia[ i ] == 0 ? 0 : 1;
		}

		flag = true;

		while( flag )
		{
			flag = false;
			ic = id;

			for( j = 1 ; j < h - 1 ; j++ )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9( ic, p, i, j );
					val9( id, q, i, j );

					if( pixel( p[ 0 ] ) != 1 )
					{
						continue;
					}

					// 条件1 = 入力画像Fにおいて，x0の4近傍に少なくとも1個の0画素が存在する
					if( p[ 1 ] == 1 && p[ 3 ] == 1 && p[ 5 ] == 1 && p[ 7 ] == 1 )
					{
						continue;
					}

					// 条件2 = 入力画像Fにおいて，x0の8近傍に2個以上の1画素がある(x0が端点でない)
					value = pixel( p[ 1 ] ) + pixel( p[ 2 ] ) + pixel( p[ 3 ] ) + pixel( p[ 4 ] ) + pixel( p[ 5 ] ) + pixel( p[ 6 ] ) + pixel( p[ 7 ] ) + pixel( p[ 8 ] );

					if( value <= 1 )
					{
						continue;
					}

					// 条件3 = 作業画像F'において，-1の画素も0画素とみなした時，x0が孤立点ではない
					value = pixel( q[ 1 ] ) + pixel( q[ 2 ] ) + pixel( q[ 3 ] ) + pixel( q[ 4 ] ) + pixel( q[ 5 ] ) + pixel( q[ 6 ] ) + pixel( q[ 7 ] ) + pixel( q[ 8 ] );

					if( value == 0 )
					{
						continue;
					}

					// 条件4 = 入力画像Fにおいて，x0におけるNc[8]の連結数が1である
					if( Nc8( p ) != 1 )
					{
						continue;
					}

					// 条件5 = i=3,5に対して次の事柄が成り立つ
					//         xiがちょうどこの反復で1から0に変えられてはいない(xiが-1になっていない)か，
					//         または，それが-1になっていても，-1を全て0とみなして計算したx0の連結数Nc[8]が1に等しい
					if( q[ 3 ] == -1 || q[ 5 ] == -1 )
					{
						if( Nc8( q ) != 1 )
						{
							continue;
						}
					}

					// 全ての条件を満たした画素は-1にする
					id( i, j ) = -1;

					flag = true;
				}
			}

			for( i = 0 ; i < id.size( ) ; i++ )
			{
				id[ i ] = id[ i ] == -1 ? 0 : id[ i ];
			}
		}

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			ia[ i ] = id[ i ] > 0 ? 1 : 0;
		}
	}
}



namespace __euclidean_utility__
{
	template < class T, class Allocator >
	inline void val9_1( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
					typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
	{
		p[ 0 ] = in( i    , j     );
		p[ 1 ] = in( i + 1, j     );
		p[ 2 ] = in( i + 1, j - 1 );
		p[ 3 ] = in( i    , j - 1 );
		p[ 4 ] = in( i - 1, j - 1 );
		p[ 5 ] = in( i - 1, j     );
		p[ 6 ] = in( i - 1, j + 1 );
		p[ 7 ] = in( i    , j + 1 );
		p[ 8 ] = in( i + 1, j + 1 );
	}

	template < class T, class Allocator >
	inline void val9_2( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
					typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
	{
		p[ 0 ] = in( i    , j     );
		p[ 1 ] = in( i - 1, j     );
		p[ 2 ] = in( i - 1, j + 1 );
		p[ 3 ] = in( i    , j + 1 );
		p[ 4 ] = in( i + 1, j + 1 );
		p[ 5 ] = in( i + 1, j     );
		p[ 6 ] = in( i + 1, j - 1 );
		p[ 7 ] = in( i    , j - 1 );
		p[ 8 ] = in( i - 1, j - 1 );
	}

	template < class T, class Allocator >
	inline void val9_3( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
					typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
	{
		p[ 0 ] = in( i    , j     );
		p[ 1 ] = in( i + 1, j     );
		p[ 2 ] = in( i + 1, j + 1 );
		p[ 3 ] = in( i    , j + 1 );
		p[ 4 ] = in( i - 1, j + 1 );
		p[ 5 ] = in( i - 1, j     );
		p[ 6 ] = in( i - 1, j - 1 );
		p[ 7 ] = in( i    , j - 1 );
		p[ 8 ] = in( i + 1, j - 1 );
	}

	template < class T > inline T max_pixel( T p[ 9 ] )
	{
		T max = 0;

		for( int i = 1 ; i < 9 ; i++ )
		{
			if( std::abs( p[ i ] ) > max )
			{
				max = std::abs( p[ i ] );
			}
		}

		return ( max );
	}

	template < class T > inline T plus_pixel_num( T p[ 9 ] )
	{
		int num = 0;

		for( int i = 1 ; i < 9 ; i++ )
		{
			if( p[ i ] > 0 )
			{
				num++;
			}
		}

		return ( num );
	}

	template < class T > inline T pixel( T p )
	{
		return ( ( p > 0 ) ? 1 : 0 );
	}

	template < class T > inline T pixel( T p[ 9 ], int index )
	{
		if( index <= 0 )
		{
			return ( pixel( p[ index + 8 ] ) );
		}
		else if( index >= 9 )
		{
			return ( pixel( p[ index - 8 ] ) );
		}
		else
		{
			return ( pixel( p[ index ] ) );
		}
	}

	template < class T > inline T pixel_( T p[ 9 ], int index )
	{
		return ( 1 - pixel( p, index ) );
	}

	template < class T > inline int Nc4( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel( p, i ) - pixel( p, i ) * pixel( p, i + 1 ) * pixel( p, i + 2 );
		}

		return ( ret );
	}

	template < class T > inline T Nc8( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel_( p, i ) - pixel_( p, i ) * pixel_( p, i + 1 ) * pixel_( p, i + 2 );
		}

		return ( ret );
	}

	// ユークリッド距離を用いた細線化の実装 by 林さん
	//
	// - 参考文献
	//   - 鈴木智, 阿部圭一, ``距離変換の結果を利用した二値画像の逐次細線化,'' 電子情報通信学会論文誌D, vol.68-D, no.4, pp.473-480, 1985.
	//
	template < class T, class Allocator >
	void thinning( array2< T, Allocator > &ia )
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		size_type w = ia.width( );
		size_type h = ia.height( );
		size_type i, j;
		double p[ 9 ], q[ 9 ];
		array2< double > ic( w, h, ia.reso1( ), ia.reso2( ) );
		array2< double > id( w, h, ia.reso1( ), ia.reso2( ) );

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			id[ i ] = ia[ i ] == 0 ? 0 : 1;
		}

		//距離変換
		euclidean::distance_transform( id, id );

		//細め処理　1回目
		ic = id;
		for( j = 1 ; j < h - 1 ; j++ )
		{
			for( i = 1 ; i < w - 1 ; i++ )
			{
				val9_1( ic, p, i, j );
				val9_1( id, q, i, j );

				if( q[ 0 ] <= 0 )
				{
					continue;
				}

				if( p[ 3 ] > 0 && p[ 5 ] > 0 )
				{
					continue;
				}

				if( Nc8( p ) != 1 )
				{
					continue;
				}

				if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
				{
					continue;
				}

				if( ( q[ 0 ] > q[ 1 ] && q[ 1 ] > 0 ) && ( ( p[ 2 ] > 0 ) || ( p[ 3 ] > 0 ) ) && ( ( q[ 7 ] > 0 ) || ( q[ 8 ] > 0 ) ) )
				{
					continue;
				}

				if( ( q[ 0 ] > q[ 7 ] && q[ 7 ] > 0 ) && ( ( p[ 5 ] > 0 ) || ( q[ 6 ] > 0 ) ) && ( ( q[ 8 ] > 0 ) || ( q[ 1 ] > 0 ) ) )
				{
					continue;
				}

				ic( i, j ) = - id( i, j );
			}
		}

		//細め処理　2回目
		id = ic;
		for( j = h - 2 ; j > 0 ; j-- )
		{
			for( i = w - 2 ; i > 0 ; i-- )
			{
				val9_2( id, p, i, j );
				val9_2( ic, q, i, j );

				if( q[ 0 ] <= 0 )
				{
					continue;
				}

				if( p[ 3 ] > 0 && p[ 5 ] > 0 )
				{
					continue;
				}

				val9_1( id, p, i, j );

				if( Nc8( p ) != 1 )
				{
					continue;
				}

				val9_2( id, p, i, j );

				if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
				{
					continue;
				}

				if( ( q[ 0 ] > q[ 1 ] && q[ 1 ] > 0 ) && ( ( p[ 2 ] > 0 ) || ( p[ 3 ] > 0 ) ) && ( ( q[ 7 ] > 0 ) || ( q[ 8 ] > 0 ) ) )
				{
					continue;
				}

				if( ( q[ 0 ] > q[ 7 ] && q[ 7 ] > 0 ) && ( ( p[ 5 ] > 0 ) || ( q[ 6 ] >= q[ 0 ] ) ) && ( ( q[ 8 ] > 0 ) || ( q[ 1 ] > 0 ) ) )
				{
					continue;
				}

				id( i, j ) = - ic( i, j );
			}
		}

		//細め処理　3回目
		ic = id;
		for( j = h - 2 ; j > 0 ; j-- )
		{
			for( i = 1 ; i < w - 1 ; i++ )
			{
				val9_3( ic, p, i, j );
				val9_3( id, q, i, j );

				if( q[ 0 ] <= 0 )
				{
					continue;
				}

				if( p[ 3 ] > 0 && p[ 5 ] > 0 )
				{
					continue;
				}

				val9_1( ic, p, i, j );

				if( Nc8( p ) != 1 )
				{
					continue;
				}

				val9_3( ic, p, i, j );

				if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
				{
					continue;
				}

				ic( i, j ) = - id( i, j );
			}
		}

		//後処理
		id = ic;
		for( j = 1 ; j < h - 1 ; j++ )
		{
			for( i = 1 ; i < w - 1 ; i++ )
			{
				val9_1( id, p, i, j );
				val9_1( ic, q, i, j );

				if( q[ 0 ] <= 0 )
				{
					id( i, j ) = 0;
					continue;
				}

				if( ( p[ 1 ] > 0 ) && ( p[ 3 ] > 0 ) && ( p[ 5 ] > 0 ) && ( p[ 7 ] > 0 ) )
				{
					continue;
				}

				if( Nc8( p ) != 1 )
				{
					continue;
				}

				if( plus_pixel_num( p ) < 2 )
				{
					continue;
				}

				if( plus_pixel_num( p ) == 2 && ( q[ 4 ] > 0 && p[ 4 ] == 0 ) )
				{
					p[ 4 ] = 1;

					if( Nc8( p ) != 1 )
					{
						continue;
					}
				}

				id( i, j ) = 0;
			}
		}

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			ia[ i ] = id[ i ] != 0 ? 1 : 0;
		}
	}
	
	//  消去可能性判定
	//	- 参考文献
	//  - 米倉達広, 横井茂樹, 鳥脇純一郎, 福村晃夫, ``3次元ディジタル画像における1-要素の消去可能性と図形収縮について,'' 電子情報通信学会論文誌D, vol.65-D, no.12, pp.1543-1549, 1982
	//
	template < size_t Nc >
	struct neighbor
	{
		typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		template < class T >
		static T P1( T *p[ 4 ], difference_type i, difference_type j )
		{
			if( j >= 9 )
			{
				return( p[ i ][ j - 8 ] );
			}
			else
			{
				return( p[ i ][ j ] );
			}
		}

		template < class T >
		static T P2( T *p[ 4 ], difference_type i, difference_type j )
		{
			if( j >= 9 )
			{
				return( p[ i ][ j - 8 ] );
			}
			else if( j <= 0 )
			{
				return( p[ i ][ j + 8 ] );
			}
			else
			{
				return( p[ i ][ j ] );
			}
		}

		//連結数 6連結
		template < class T >
		static difference_type Nc6( T *p[ 4 ] )
		{
			static size_type S0[] = { 1, 3 };
			static size_type S1[] = { 1, 3, 5, 7 };

			difference_type ret = 0;

			for( size_type h = 0 ; h < 2 ; h++ )
			{
				difference_type tmp = 0;
				for( size_type k = 0 ; k < 4 ; k++ )
				{
					tmp += P1( p, S0[ h ], S1[ k ] ) * P1( p, 2, S1[ k ] );
				}
				ret += P1( p, S0[ h ], 0 ) * ( 1 - tmp );
			}

			for( size_type k = 0 ; k < 4 ; k++ )
			{
				difference_type tmp = 0;
				for( size_type h = 0 ; h < 2 ; h++ )
				{
					tmp += P1( p, S0[ h ], 0 ) * P1( p, S0[ h ], S1[ k ] ) * P1( p, S0[ h ], S1[ k ] + 1 ) * P1( p, S0[ h ], S1[ k ] + 2 );
				}
				ret += P1( p, 2, S1[ k ] ) * ( 1 - P1( p, 2, S1[ k ] + 1 ) * P1( p, 2, S1[ k ] + 2 ) * ( 1 - tmp ) );
			}

			return( ret );
		}

		//連結数 6連結
		template < class T >
		static difference_type Sx( T *p[ 4 ] )
		{
			static size_type S0[] = { 1, 3 };
			static size_type S1[] = { 1, 3, 5, 7 };

			difference_type sx = 0;

			for( size_type k = 0 ; k < 4 ; k++ )
			{
				for( size_type h = 0 ; h < 2 ; h++ )
				{
					sx += ( 1 - P1( p, S0[ h ], S1[ k ] + 1 ) ) * P1( p, S0[ h ], S1[ k ] ) * P1( p, S0[ h ], S1[ k ] + 2 ) * P1( p, S0[ h ], 0 ) * P1( p, 2, S1[ k ] ) * P1( p, 2, S1[ k ] + 1 ) * P1( p, 2, S1[ k ] + 2 );
				}
			}

			return( sx );
		}
	
		static bool is_deletable( const int v[ 27 ] )
		{
			// 近傍情報を設定
			const int *p[ 4 ] = { NULL, v + 0, v + 9, v + 18 };

			//１．連結数
			if( Nc6( p ) != 1 )
			{
				return( false );
			}

			//２．6近傍にある1-要素の個数
			difference_type num = p[ 1 ][ 0 ] + p[ 2 ][ 1 ] + p[ 2 ][ 3 ] + p[ 2 ][ 5 ] + p[ 2 ][ 7 ] + p[ 3 ][ 0 ];
			if( num <= 3 )
			{
				return( true );
			}
			//３．
			else if( num == 4 )
			{
				if( Sx( p ) == 1 )
				{
					return( false );
				}
				else
				{
					return( true );
				}
			}
			//４．
			else if( num == 5 )
			{
				if( Sx( p ) == 1 )
				{
					return( false );
				}
				else
				{
					//射影グラフに孤立点があるか
					if( p[ 2 ][ 0 ] == 0 )
					{
						return( false );
					}

					static size_type S1[] = { 1, 3, 5, 7 };
					difference_type hole = 0;
					if( p[ 1 ][ 0 ] != 0 )
					{
						for( size_type h = 0 ; h < 4 ; h++ )
						{
							hole += p[ 1 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
						}
						if( hole == 0 )
						{
							return( false );
						}
					}

					hole = 0;
					if( p[ 3 ][ 0 ] != 0 )
					{
						for( size_type h = 0 ; h < 4 ; h++ )
						{
							hole += p[ 3 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
						}
						if( hole == 0 )
						{
							return( false );
						}
					}

					for( size_type h = 0 ; h < 4 ; h++ )
					{
						hole = 0;
						if( p[ 2 ][ S1[ h ] ] != 0 )
						{
							hole = p[ 1 ][ 0 ] * p[ 1 ][ S1[ h ] ] + p[ 3 ][ 0 ] * p[ 3 ][ S1[ h ] ] + P2( p, 2, S1[ h ] - 1 ) * P2( p, 2, S1[ h ] - 2 ) + P2( p, 2, S1[ h ] + 1 ) * P2( p, 2, S1[ h ] + 2 );
							if( hole == 0 )
							{
								return( false );
							}
						}
					}
					return( true );
				}
			}
			//５．
			else if( num == 6 )
			{
				typedef matrix< difference_type > matrix_type;

				//隣接行列
				matrix_type M( 6, 6 );

				M( 0, 0 ) = 0          ; M( 0, 1 ) = p[ 1 ][ 1 ]; M( 0, 2 ) = p[ 1 ][ 3 ]; M( 0, 3 ) = p[ 1 ][ 7 ]; M( 0, 4 ) = p[ 1 ][ 5 ]; M( 0, 5 ) = 0;
				M( 1, 0 ) = p[ 1 ][ 1 ]; M( 1, 1 ) = 0          ; M( 1, 2 ) = p[ 2 ][ 2 ]; M( 1, 3 ) = p[ 2 ][ 8 ]; M( 1, 4 ) = 0          ; M( 1, 5 ) = p[ 3 ][ 1 ];
				M( 2, 0 ) = p[ 1 ][ 3 ]; M( 2, 1 ) = p[ 2 ][ 2 ]; M( 2, 2 ) = 0          ; M( 2, 3 ) = 0          ; M( 2, 4 ) = p[ 2 ][ 4 ]; M( 2, 5 ) = p[ 3 ][ 3 ];
				M( 3, 0 ) = p[ 1 ][ 7 ]; M( 3, 1 ) = p[ 2 ][ 8 ]; M( 3, 2 ) = 0          ; M( 3, 3 ) = 0          ; M( 3, 4 ) = p[ 2 ][ 6 ]; M( 3, 5 ) = p[ 3 ][ 7 ];
				M( 4, 0 ) = p[ 1 ][ 5 ]; M( 4, 1 ) = 0          ; M( 4, 2 ) = p[ 2 ][ 4 ]; M( 4, 3 ) = p[ 2 ][ 6 ]; M( 4, 4 ) = 0          ; M( 4, 5 ) = p[ 3 ][ 5 ];
				M( 5, 0 ) = 0          ; M( 5, 1 ) = p[ 3 ][ 1 ]; M( 5, 2 ) = p[ 3 ][ 3 ]; M( 5, 3 ) = p[ 3 ][ 7 ]; M( 5, 4 ) = p[ 3 ][ 5 ]; M( 5, 5 ) = 0;

				matrix_type I = matrix_type::identity( 6, 6 );
				matrix_type N = M * matrix_type( I + M * matrix_type( I + M * matrix_type( I + M * ( I + M ) ) ));

				for( size_type r = 0 ; r < N.size( ) ; r++ )
				{
					if( N[ r ] == 0 )
					{
						return( false );
					}
				}
				return( true );
			}
			else
			{
				std::cout << "Error" << std::endl;
			}

			return( true );
		}
	
		//３次元単体のチェック
		static bool is_3d_simplex( const int v[ 27 ] )
		{
			// 近傍情報を設定
			int p[ 3 ][ 3 ][ 3 ];

			p[ 0 ][ 0 ][ 0 ] = v[  4 ];
			p[ 1 ][ 0 ][ 0 ] = v[  5 ];
			p[ 2 ][ 0 ][ 0 ] = v[  6 ];
			p[ 0 ][ 1 ][ 0 ] = v[  3 ];
			p[ 1 ][ 1 ][ 0 ] = v[  0 ];
			p[ 2 ][ 1 ][ 0 ] = v[  7 ];
			p[ 0 ][ 2 ][ 0 ] = v[  2 ];
			p[ 1 ][ 2 ][ 0 ] = v[  1 ];
			p[ 2 ][ 2 ][ 0 ] = v[  8 ];
			p[ 0 ][ 0 ][ 1 ] = v[ 13 ];
			p[ 1 ][ 0 ][ 1 ] = v[ 14 ];
			p[ 2 ][ 0 ][ 1 ] = v[ 15 ];
			p[ 0 ][ 1 ][ 1 ] = v[ 12 ];
			p[ 1 ][ 1 ][ 1 ] = v[  9 ];
			p[ 2 ][ 1 ][ 1 ] = v[ 16 ];
			p[ 0 ][ 2 ][ 1 ] = v[ 11 ];
			p[ 1 ][ 2 ][ 1 ] = v[ 10 ];
			p[ 2 ][ 2 ][ 1 ] = v[ 17 ];
			p[ 0 ][ 0 ][ 2 ] = v[ 22 ];
			p[ 1 ][ 0 ][ 2 ] = v[ 23 ];
			p[ 2 ][ 0 ][ 2 ] = v[ 24 ];
			p[ 0 ][ 1 ][ 2 ] = v[ 21 ];
			p[ 1 ][ 1 ][ 2 ] = v[ 18 ];
			p[ 2 ][ 1 ][ 2 ] = v[ 25 ];
			p[ 0 ][ 2 ][ 2 ] = v[ 20 ];
			p[ 1 ][ 2 ][ 2 ] = v[ 19 ];
			p[ 2 ][ 2 ][ 2 ] = v[ 26 ];

			if( p[ 1 ][ 1 ][ 1 ] == 0 ) return( false );

			for( size_type k = 1 ; k < 3 ; k++ ) 
			{
				for( size_type j = 1 ; j < 3 ; j++ ) 
				{
					for( size_type i = 1 ; i < 3 ; i++ ) 
					{
						if( p[ i - 1 ][ j ][ k ] * p[ i ][ j - 1 ][ k ] * p[ i ][ j ][ k - 1 ] *
							p[ i - 1 ][ j - 1 ][ k ] * p[ i ][ j - 1 ][ k - 1 ] * p[ i - 1 ][ j ][ k - 1 ] * 
							p[ i ][ j ][ k ] * p[ i - 1 ][ j - 1 ][ k - 1 ] != 0 )
						{
							return( true );
						}
					}
				}
			}
			
			return( false );
		}
	};

	template < >
	struct neighbor< 26 >
	{
		typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		template < class T >
		static T P1( T *p[ 4 ], difference_type i, difference_type j )
		{
			if( j >= 9 )
			{
				return( p[ i ][ j - 8 ] );
			}
			else
			{
				return( p[ i ][ j ] );
			}
		}

		template < class T >
		static T P2( T *p[ 4 ], difference_type i, difference_type j )
		{
			if( j >= 9 )
			{
				return( p[ i ][ j - 8 ] );
			}
			else if( j <= 0 )
			{
				return( p[ i ][ j + 8 ] );
			}
			else
			{
				return( p[ i ][ j ] );
			}
		}

		template < class T >
		static T P1_( T *p[ 4 ], difference_type i, difference_type j )
		{
			if( j >= 9 )
			{
				return( 1 - p[ i ][ j - 8 ] );
			}
			else
			{
				return( 1 - p[ i ][ j ] );
			}
		}

		template < class T >
		static T P2_( T *p[ 4 ], difference_type i, difference_type j )
		{
			if( j >= 9 )
			{
				return( 1 - p[ i ][ j - 8 ] );
			}
			else if( j <= 0 )
			{
				return( 1 - p[ i ][ j + 8 ] );
			}
			else
			{
				return( 1 - p[ i ][ j ] );
			}
		}


		// 連結数 26連結
		template < class T >
		static difference_type Nc26( T *p[ 4 ] )
		{
			static size_type S0[] = { 1, 3 };
			static size_type S1[] = { 1, 3, 5, 7 };

			difference_type ret = 0;

			for( size_type h = 0 ; h < 2 ; h++ )
			{
				difference_type tmp = 0;
				for( size_type k = 0 ; k < 4 ; k++ )
				{
					tmp += P1_( p, S0[ h ], S1[ k ] ) * P1_( p, 2, S1[ k ] );
				}
				ret += P1_( p, S0[ h ], 0 ) * ( 1 - tmp );
			}

			for( size_type k = 0 ; k < 4 ; k++ )
			{
				difference_type tmp = 0;
				for( size_type h = 0 ; h < 2 ; h++ )
				{
					tmp += P1_( p, S0[ h ], 0 ) * P1_( p, S0[ h ], S1[ k ] ) * P1_( p, S0[ h ], S1[ k ] + 1 ) * P1_( p, S0[ h ], S1[ k ] + 2 );
				}
				ret += P1_( p, 2, S1[ k ] ) * ( 1 - P1_( p, 2, S1[ k ] + 1 ) * P1_( p, 2, S1[ k ] + 2 ) * ( 1 - tmp ) );
			}

			ret = 2 - ret;

			return( ret );
		}

		//
		template < class T >
		static difference_type Sx( T *p[ 4 ] )
		{
			static size_type S0[] = { 1, 3 };
			static size_type S1[] = { 1, 3, 5, 7 };

			difference_type sx = 0;

			for( size_type k = 0 ; k < 4 ; k++ )
			{
				for( size_type h = 0 ; h < 2 ; h++ )
				{
					sx += ( 1 - P1( p, S0[ h ], S1[ k ] + 1 ) ) * P1( p, S0[ h ], S1[ k ] ) * P1( p, S0[ h ], S1[ k ] + 2 ) * P1( p, S0[ h ], 0 ) * P1( p, 2, S1[ k ] ) * P1( p, 2, S1[ k ] + 1 ) * P1( p, 2, S1[ k ] + 2 );
				}
			}

			return( sx );
		}


		static bool is_deletable( const int v[ 27 ] )
		{
			// 近傍情報を設定
			int pp[ 27 ];
			const int *p[ 4 ] = { NULL, v + 0, v + 9, v + 18 };
			memcpy( pp, v, sizeof( int ) * 27 );

			//１．連結数
			if( Nc26( p ) != 1 )
			{
				return( false );
			}

			// 近傍情報を取得 0-1反転
			for( size_t i = 0 ; i < 9 ; i++ )
			{
				pp[ i ] = v[ i ] == 0 ? 1 : 0;
			}
			pp[ 9 ] = v[ 9 ];
			for( size_t i = 10 ; i < 27 ; i++ )
			{
				pp[ i ] = v[ i ] == 0 ? 1 : 0;
			}

			p[ 1 ] = pp;
			p[ 2 ] = pp + 9;
			p[ 3 ] = pp + 18;

			//２．6近傍にある1-要素の個数
			difference_type num = p[ 1 ][ 0 ] + p[ 2 ][ 1 ] + p[ 2 ][ 3 ] + p[ 2 ][ 5 ] + p[ 2 ][ 7 ] + p[ 3 ][ 0 ];
			if( num <= 3 )
			{
				return( true );
			}
			//３．
			else if( num == 4 )
			{
				if( Sx( p ) == 1 )
				{
					return( false );
				}
				else
				{
					return( true );
				}
			}
			//４．
			else if( num == 5 )
			{
				if( Sx( p ) == 1 )
				{
					return( false );
				}
				else
				{
					//射影グラフに孤立点があるか
					if( p[ 2 ][ 0 ] == 0 )
					{
						return( false );
					}

					static size_type S1[] = { 1, 3, 5, 7 };
					difference_type hole = 0;
					if( p[ 1 ][ 0 ] != 0 )
					{
						for( size_type h = 0 ; h < 4 ; h++ )
						{
							hole += p[ 1 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
						}
						if( hole == 0 )
						{
							return( false );
						}
					}

					hole = 0;
					if( p[ 3 ][ 0 ] != 0 )
					{
						for( size_type h = 0 ; h < 4 ; h++ )
						{
							hole += p[ 3 ][ S1[ h ] ] * p[ 2 ][ S1[ h ] ];
						}
						if( hole == 0 )
						{
							return( false );
						}
					}

					for( size_type h = 0 ; h < 4 ; h++ )
					{
						hole = 0;
						if( p[ 2 ][ S1[ h ] ] != 0 )
						{
							hole = p[ 1 ][ 0 ] * p[ 1 ][ S1[ h ] ] + p[ 3 ][ 0 ] * p[ 3 ][ S1[ h ] ] + P2( p, 2, S1[ h ] - 1 ) * P2( p, 2, S1[ h ] - 2 ) + P2( p, 2, S1[ h ] + 1 ) * P2( p, 2, S1[ h ] + 2 );
							if( hole == 0 )
							{
								return( false );
							}
						}
					}
					return( true );
				}
			}
			//５．
			else if( num == 6 )
			{
				typedef matrix< int > matrix_type;

				//隣接行列
				matrix_type M( 6, 6 );

				M( 0, 0 ) = 0          ; M( 0, 1 ) = p[ 1 ][ 1 ]; M( 0, 2 ) = p[ 1 ][ 3 ]; M( 0, 3 ) = p[ 1 ][ 7 ]; M( 0, 4 ) = p[ 1 ][ 5 ]; M( 0, 5 ) = 0;
				M( 1, 0 ) = p[ 1 ][ 1 ]; M( 1, 1 ) = 0          ; M( 1, 2 ) = p[ 2 ][ 2 ]; M( 1, 3 ) = p[ 2 ][ 8 ]; M( 1, 4 ) = 0          ; M( 1, 5 ) = p[ 3 ][ 1 ];
				M( 2, 0 ) = p[ 1 ][ 3 ]; M( 2, 1 ) = p[ 2 ][ 2 ]; M( 2, 2 ) = 0          ; M( 2, 3 ) = 0          ; M( 2, 4 ) = p[ 2 ][ 4 ]; M( 2, 5 ) = p[ 3 ][ 3 ];
				M( 3, 0 ) = p[ 1 ][ 7 ]; M( 3, 1 ) = p[ 2 ][ 8 ]; M( 3, 2 ) = 0          ; M( 3, 3 ) = 0          ; M( 3, 4 ) = p[ 2 ][ 6 ]; M( 3, 5 ) = p[ 3 ][ 7 ];
				M( 4, 0 ) = p[ 1 ][ 5 ]; M( 4, 1 ) = 0          ; M( 4, 2 ) = p[ 2 ][ 4 ]; M( 4, 3 ) = p[ 2 ][ 6 ]; M( 4, 4 ) = 0          ; M( 4, 5 ) = p[ 3 ][ 5 ];
				M( 5, 0 ) = 0          ; M( 5, 1 ) = p[ 3 ][ 1 ]; M( 5, 2 ) = p[ 3 ][ 3 ]; M( 5, 3 ) = p[ 3 ][ 7 ]; M( 5, 4 ) = p[ 3 ][ 5 ]; M( 5, 5 ) = 0;

				matrix_type I = matrix_type::identity( 6, 6 );
				matrix_type N = M * matrix_type( I + M * matrix_type( I + M * matrix_type( I + M * ( I + M ) ) ));

				for( size_type r = 0 ; r < N.size( ) ; r++ )
				{
					if( N[ r ] == 0 )
					{
						return( false );
					}
				}
				return( true );
			}
			else
			{
				std::cout << "Error" << std::endl;
			}

			return( true );
		}
	
		//３次元単体のチェック
		static bool is_3d_simplex( const int v[ 27 ] )
		{
			// 近傍情報を設定
			int p[ 3 ][ 3 ][ 3 ];

			p[ 0 ][ 0 ][ 0 ] = v[  4 ];
			p[ 1 ][ 0 ][ 0 ] = v[  5 ];
			p[ 2 ][ 0 ][ 0 ] = v[  6 ];
			p[ 0 ][ 1 ][ 0 ] = v[  3 ];
			p[ 1 ][ 1 ][ 0 ] = v[  0 ];
			p[ 2 ][ 1 ][ 0 ] = v[  7 ];
			p[ 0 ][ 2 ][ 0 ] = v[  2 ];
			p[ 1 ][ 2 ][ 0 ] = v[  1 ];
			p[ 2 ][ 2 ][ 0 ] = v[  8 ];
			p[ 0 ][ 0 ][ 1 ] = v[ 13 ];
			p[ 1 ][ 0 ][ 1 ] = v[ 14 ];
			p[ 2 ][ 0 ][ 1 ] = v[ 15 ];
			p[ 0 ][ 1 ][ 1 ] = v[ 12 ];
			p[ 1 ][ 1 ][ 1 ] = v[  9 ];
			p[ 2 ][ 1 ][ 1 ] = v[ 16 ];
			p[ 0 ][ 2 ][ 1 ] = v[ 11 ];
			p[ 1 ][ 2 ][ 1 ] = v[ 10 ];
			p[ 2 ][ 2 ][ 1 ] = v[ 17 ];
			p[ 0 ][ 0 ][ 2 ] = v[ 22 ];
			p[ 1 ][ 0 ][ 2 ] = v[ 23 ];
			p[ 2 ][ 0 ][ 2 ] = v[ 24 ];
			p[ 0 ][ 1 ][ 2 ] = v[ 21 ];
			p[ 1 ][ 1 ][ 2 ] = v[ 18 ];
			p[ 2 ][ 1 ][ 2 ] = v[ 25 ];
			p[ 0 ][ 2 ][ 2 ] = v[ 20 ];
			p[ 1 ][ 2 ][ 2 ] = v[ 19 ];
			p[ 2 ][ 2 ][ 2 ] = v[ 26 ];

			if( p[ 1 ][ 1 ][ 1 ] == 0 ) return( false );

			for( size_type k = 0 ; k < 3 ; k += 2 ) 
			{
				for( size_type j = 0 ; j < 3 ; j += 2 ) 
				{
					for( size_type i = 0 ; i < 3 ; i += 2 ) 
					{
						int num = 0;
						
						if( p[ i ][ j ][ 1 ] != 0 ) num++;
						if( p[ i ][ j ][ k ] != 0 ) num++;
						if( p[ i ][ 1 ][ 1 ] != 0 ) num++;
						if( p[ i ][ 1 ][ k ] != 0 ) num++;
						if( p[ 1 ][ j ][ 1 ] != 0 ) num++;
						if( p[ 1 ][ j ][ k ] != 0 ) num++;
						if( p[ 1 ][ 1 ][ k ] != 0 ) num++;

						if( num > 3 ) return( true );
						if( num < 3 ) continue;

						if( p[ 1 ][ 1 ][ k ] * ( p[ 1 ][ 1 ][ k ] * p[ i ][ 1 ][ k ] + 
												 p[ 1 ][ j ][ 1 ] * p[ 1 ][ j ][ k ] + 
												 p[ i ][ j ][ 1 ] * p[ i ][ j ][ k ] ) != 0 )
						{
							continue;
						}
						if( p[ i ][ 1 ][ 1 ] * ( p[ 1 ][ j ][ 1 ] * p[ i ][ j ][ 1 ] +
												 p[ 1 ][ j ][ k ] * p[ i ][ j ][ k ] ) != 0 )
						{
							continue;
						}
						if( p[ 1 ][ j ][ 1 ] * ( p[ i ][ 1 ][ k ] * p[ i ][ j ][ k ] ) != 0 )
						{
							continue;
						}

						return( true );

					}
				}
			}
			return( false );
		}
	};


	template < class Tin, class Tout, class Difference >
	void create_neighbor_list( const Tin *pin, Tout *pout, Difference p[ 27 ] )
	{
		for( size_t i = 0 ; i < 27 ; i++ )
		{
			pout[ i ] = pin[ p[ i ] ] > 0 ? 1 : 0;
		}
	}

	template < class T, class Allocator, class Neighbor >
	void shrink_skelton( array3< T, Allocator > &in, Neighbor __dmy__ )
	{
		typedef typename array3< T, Allocator >::size_type size_type;
		typedef typename array3< T, Allocator >::difference_type difference_type;
		typedef typename array3< T, Allocator >::const_pointer const_pointer;
		typedef typename array3< T, Allocator >::value_type value_type;
		typedef Neighbor neighbor_type;

		difference_type diff[ 27 ];
		int val[ 27 ];

		{
			difference_type ox = in.width( ) / 2;
			difference_type oy = in.height( ) / 2;
			difference_type oz = in.depth( ) / 2;

			const_pointer p0 = &in( ox, oy, oz );

			diff[  0 ] = &in( ox    , oy    , oz - 1 ) - p0;
			diff[  1 ] = &in( ox    , oy + 1, oz - 1 ) - p0;
			diff[  2 ] = &in( ox - 1, oy + 1, oz - 1 ) - p0;
			diff[  3 ] = &in( ox - 1, oy    , oz - 1 ) - p0;
			diff[  4 ] = &in( ox - 1, oy - 1, oz - 1 ) - p0;
			diff[  5 ] = &in( ox    , oy - 1, oz - 1 ) - p0;
			diff[  6 ] = &in( ox + 1, oy - 1, oz - 1 ) - p0;
			diff[  7 ] = &in( ox + 1, oy    , oz - 1 ) - p0;
			diff[  8 ] = &in( ox + 1, oy + 1, oz - 1 ) - p0;

			diff[  9 ] = &in( ox    , oy    , oz     ) - p0;
			diff[ 10 ] = &in( ox    , oy + 1, oz     ) - p0;
			diff[ 11 ] = &in( ox - 1, oy + 1, oz     ) - p0;
			diff[ 12 ] = &in( ox - 1, oy    , oz     ) - p0;
			diff[ 13 ] = &in( ox - 1, oy - 1, oz     ) - p0;
			diff[ 14 ] = &in( ox    , oy - 1, oz     ) - p0;
			diff[ 15 ] = &in( ox + 1, oy - 1, oz     ) - p0;
			diff[ 16 ] = &in( ox + 1, oy    , oz     ) - p0;
			diff[ 17 ] = &in( ox + 1, oy + 1, oz     ) - p0;

			diff[ 18 ] = &in( ox    , oy    , oz + 1 ) - p0;
			diff[ 19 ] = &in( ox    , oy + 1, oz + 1 ) - p0;
			diff[ 20 ] = &in( ox - 1, oy + 1, oz + 1 ) - p0;
			diff[ 21 ] = &in( ox - 1, oy    , oz + 1 ) - p0;
			diff[ 22 ] = &in( ox - 1, oy - 1, oz + 1 ) - p0;
			diff[ 23 ] = &in( ox    , oy - 1, oz + 1 ) - p0;
			diff[ 24 ] = &in( ox + 1, oy - 1, oz + 1 ) - p0;
			diff[ 25 ] = &in( ox + 1, oy    , oz + 1 ) - p0;
			diff[ 26 ] = &in( ox + 1, oy + 1, oz + 1 ) - p0;
		}

		// 図形の端は0
		for( size_type j = 0 ; j < in.height( ) ; j++ )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				in( i, j, 0 ) = 0;
				in( i, j, in.depth( ) - 1 ) = 0;
			}
		}

		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				in( 0, j, k ) = 0;
				in( in.width( ) - 1, j, k ) = 0;
			}
		}

		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				in( i, 0, k ) = 0;
				in( i, in.height( ) - 1, k ) = 0;
			}
		}

		bool loop;
		do
		{
			loop = false;
			for( size_type k = 1 ; k < in.depth( ) - 1 ; k++ )
			{
				for( size_type j = 1 ; j < in.height( ) - 1 ; j++ )
				{
					for( size_type i = 1 ; i < in.width( ) - 1 ; i++ )
					{
						value_type &v = in( i, j, k );
						if( v != 0 )
						{
							create_neighbor_list( &v, val, diff );

							if( neighbor_type::is_deletable( val ) )
							{
								v = 0;
								loop = true;
							}
						}
					}
				}
			}
		} while( loop );
	}

	//　ユークリッド距離変換を用いた3次元ディジタル画像の細線化と薄面化
	//	- 参考文献
	//   - 斉藤豊文, 森健策, 鳥脇純一郎, ``ユークリッド距離変換を用いた3次元ディジタル画像の薄面化および細線化の逐次型アルゴリズムとその諸性質,'' 電子情報通信学会論文誌D-II, vol.J79-D-II, no.10, pp.1675-1685, 1996
	//
	template< class T >
	struct border
	{
		typedef ptrdiff_t difference_type;

		difference_type diff;
		T value;
		T distance;

		border( difference_type d, T val ) : diff( d ), value( val ), distance( val ) {}
	};

	template < class T >
	inline std::ostream &operator <<( std::ostream &out, const border< T > &b )
	{
		out << "( ";
		out << b.i << ", ";
		out << b.j << ", ";
		out << b.k;
		out << " ) = ( ";
		out << b.value << ", ";
		out << b.distance;
		out << " )";
		return( out );
	}

	template < class T, class Allocator, class Neighbor >
	void thinning( array3< T, Allocator > &in, Neighbor __dmy__ )
	{
		typedef typename array3< T, Allocator >::size_type size_type;
		typedef typename array3< T, Allocator >::difference_type difference_type;
		typedef typename array3< T, Allocator >::const_pointer const_pointer;
		typedef typename array3< T, Allocator >::pointer pointer;
		typedef T value_type;
		typedef Neighbor neighbor_type;
		typedef border< T > border_type;
		typedef std::vector< border_type > border_list_type;

		// Step1 距離変換
		euclidean::distance_transform( in, in );

		// 図形の端は0
		for( size_type j = 0 ; j < in.height( ) ; j++ )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				in( i, j, 0 ) = 0;
				in( i, j, in.depth( ) - 1 ) = 0;
			}
		}

		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				in( 0, j, k ) = 0;
				in( in.width( ) - 1, j, k ) = 0;
			}
		}

		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				in( i, 0, k ) = 0;
				in( i, in.height( ) - 1, k ) = 0;
			}
		}

		value_type min = type_limits< value_type >::maximum( ), max = type_limits< value_type >::minimum( );
		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			value_type &v = in[ i ];
			if( v != 0 )
			{
				v += 20;
				if( v > max ) max = v;
				if( v < min ) min = v;
			}
		}

		border_list_type blist, slist[ 9 ];
		difference_type diff[ 27 ], nc6[ 6 ];
		int val[ 27 ];

		{
			difference_type ox = in.width( ) / 2;
			difference_type oy = in.height( ) / 2;
			difference_type oz = in.depth( ) / 2;

			const_pointer p0 = &in( ox, oy, oz );


			diff[  0 ] = &in( ox    , oy    , oz - 1 ) - p0;
			diff[  1 ] = &in( ox    , oy + 1, oz - 1 ) - p0;
			diff[  2 ] = &in( ox - 1, oy + 1, oz - 1 ) - p0;
			diff[  3 ] = &in( ox - 1, oy    , oz - 1 ) - p0;
			diff[  4 ] = &in( ox - 1, oy - 1, oz - 1 ) - p0;
			diff[  5 ] = &in( ox    , oy - 1, oz - 1 ) - p0;
			diff[  6 ] = &in( ox + 1, oy - 1, oz - 1 ) - p0;
			diff[  7 ] = &in( ox + 1, oy    , oz - 1 ) - p0;
			diff[  8 ] = &in( ox + 1, oy + 1, oz - 1 ) - p0;

			diff[  9 ] = &in( ox    , oy    , oz     ) - p0;
			diff[ 10 ] = &in( ox    , oy + 1, oz     ) - p0;
			diff[ 11 ] = &in( ox - 1, oy + 1, oz     ) - p0;
			diff[ 12 ] = &in( ox - 1, oy    , oz     ) - p0;
			diff[ 13 ] = &in( ox - 1, oy - 1, oz     ) - p0;
			diff[ 14 ] = &in( ox    , oy - 1, oz     ) - p0;
			diff[ 15 ] = &in( ox + 1, oy - 1, oz     ) - p0;
			diff[ 16 ] = &in( ox + 1, oy    , oz     ) - p0;
			diff[ 17 ] = &in( ox + 1, oy + 1, oz     ) - p0;

			diff[ 18 ] = &in( ox    , oy    , oz + 1 ) - p0;
			diff[ 19 ] = &in( ox    , oy + 1, oz + 1 ) - p0;
			diff[ 20 ] = &in( ox - 1, oy + 1, oz + 1 ) - p0;
			diff[ 21 ] = &in( ox - 1, oy    , oz + 1 ) - p0;
			diff[ 22 ] = &in( ox - 1, oy - 1, oz + 1 ) - p0;
			diff[ 23 ] = &in( ox    , oy - 1, oz + 1 ) - p0;
			diff[ 24 ] = &in( ox + 1, oy - 1, oz + 1 ) - p0;
			diff[ 25 ] = &in( ox + 1, oy    , oz + 1 ) - p0;
			diff[ 26 ] = &in( ox + 1, oy + 1, oz + 1 ) - p0;

			nc6[ 0 ] = diff[ 12 ];
			nc6[ 1 ] = diff[ 16 ];
			nc6[ 2 ] = diff[ 14 ];
			nc6[ 3 ] = diff[ 10 ];
			nc6[ 4 ] = diff[  0 ];
			nc6[ 5 ] = diff[ 18 ];
		}

		{
			const_pointer p0 = &in[ 0 ];

			// Step2 境界画素の検出
			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				value_type &v = in[ i ];
				if( v > 20 )
				{
					const_pointer p = &v;
					if( p[ nc6[ 0 ] ] == 0 || p[ nc6[ 1 ] ] == 0 ||
						p[ nc6[ 2 ] ] == 0 || p[ nc6[ 3 ] ] == 0 ||
						p[ nc6[ 4 ] ] == 0 || p[ nc6[ 5 ] ] == 0 )
					{
						blist.push_back( border_type( p - p0, v ) );
						v = 1;
					}
				}
			}
		}

		// 以降の処理のためにメモリの予備を確保する
		blist.reserve( blist.size( ) * 2 + 1 );
		slist[ 0 ].reserve( blist.size( ) );
		slist[ 1 ].reserve( blist.size( ) );
		slist[ 2 ].reserve( blist.size( ) );
		slist[ 3 ].reserve( blist.size( ) );
		slist[ 4 ].reserve( blist.size( ) );
		slist[ 5 ].reserve( blist.size( ) );
		slist[ 6 ].reserve( blist.size( ) );
		slist[ 7 ].reserve( blist.size( ) );
		slist[ 8 ].reserve( blist.size( ) );

		size_type count = 0, loop = 0;

		do
		{
			size_type __length__ = 0;

			//Step3 メインサイクル
			for( size_type l = 0 ; l < blist.size( ) ; l++ )
			{
				border_type &b = blist[ l ];
				pointer p = &in[ b.diff ];

				if( b.value <= min )
				{
					create_neighbor_list( p, val, diff );

					// 消去不可能なら一時保存点
					if( !neighbor_type::is_deletable( val ) )
					{
						b.value = 16;
					}
					else
					{
						// 自分自身を除く
						size_type num = val[  0 ];
						num += val[  1 ];
						num += val[  2 ];
						num += val[  3 ];
						num += val[  4 ];
						num += val[  5 ];
						num += val[  6 ];
						num += val[  7 ];
						num += val[  8 ];
						num += val[ 10 ];
						num += val[ 11 ];
						num += val[ 12 ];
						num += val[ 13 ];
						num += val[ 14 ];
						num += val[ 15 ];
						num += val[ 16 ];
						num += val[ 17 ];
						num += val[ 18 ];
						num += val[ 19 ];
						num += val[ 20 ];
						num += val[ 21 ];
						num += val[ 22 ];
						num += val[ 23 ];
						num += val[ 24 ];
						num += val[ 25 ];
						num += val[ 26 ];

						// 端点なら永久保存点
						if( num == 1 )
						{
							continue;
						}
						else
						{
							difference_type val = num / 3 + 7;
							b.value = static_cast< value_type >( val );

							if( 7 <= val && val <= 15 )
							{
								slist[ val - 7 ].push_back( b );
								continue;
							}
						}
					}
				}

				blist[ __length__++ ] = b;
			}

			blist.erase( blist.begin( ) + __length__, blist.end( ) );

			// Step4 サブサイクル
			for( size_type ll = 0 ; ll < 9 ; ll++ )
			{
				border_list_type &list = slist[ ll ];

				for( size_type l = 0 ; l < list.size( ) ; l++ )
				{
					border_type &b = list[ l ];
					pointer p = &in[ b.diff ];

					create_neighbor_list( p, val, diff );

					//消去不可能なら一時保存点
					if( !neighbor_type::is_deletable( val ) )
					{
						b.value = 16;
						blist.push_back( b );
					}
					else
					{
						size_type num = 0, i;

						// 自分自身を除く周りを調べる
						for( i = 0 ; i < 9 && num < 2 ; i++ )
						{
							if( val[ i ] > 0 )
							{
								num++;
							}
						}
						for( i = 10 ; i < 27 && num < 2 ; i++ )
						{
							if( val[ i ] > 0 )
							{
								num++;
							}
						}

						//端点なら永久保存点
						if( num > 1 )
						{
							//画素の消去
							p[ 0 ] = 0;

							if( p[ nc6[ 0 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 0 ], p[ nc6[ 0 ] ] ) );
								p[ nc6[ 0 ] ] = 1;
							}
							if( p[ nc6[ 1 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 1 ], p[ nc6[ 1 ] ] ) );
								p[ nc6[ 1 ] ] = 1;
							}
							if( p[ nc6[ 2 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 2 ], p[ nc6[ 2 ] ] ) );
								p[ nc6[ 2 ] ] = 1;
							}
							if( p[ nc6[ 3 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 3 ], p[ nc6[ 3 ] ] ) );
								p[ nc6[ 3 ] ] = 1;
							}
							if( p[ nc6[ 4 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 4 ], p[ nc6[ 4 ] ] ) );
								p[ nc6[ 4 ] ] = 1;
							}
							if( p[ nc6[ 5 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 5 ], p[ nc6[ 5 ] ] ) );
								p[ nc6[ 5 ] ] = 1;
							}
						}
					}
				}

				// 次以降の処理のためにリスト空にする（内部で使用するメモリ容量は変化しない）
				list.clear( );
			}

			for( size_type l = 0 ; l < blist.size( ) ; l++ )
			{
				border_type &b = blist[ l ];
				pointer p = &in[ b.diff ];

				if( b.value == 16 )
				{
					create_neighbor_list( p, val, diff );

					if( neighbor_type::is_deletable( val ) )
					{
						b.value = b.distance;
					}
				}
			}

			//Step5 終了判定
			min = max;
			count = 0;
			for( size_type l = 0 ; l < blist.size( ) ; l++ )
			{
				border_type &b = blist[ l ];
				if( b.value < min && b.value > 20 )
				{
					min = b.value;
				}

				if( b.value == 16 )
				{
					count++;
				}
			}

			std::cout << loop++ << "          \r";

		} while( min < max || count != blist.size( ) );

		std::cout << std::endl;

		// Step6 後処理
		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			if( in[ i ] != 0 )
			{
				in[ i ] = 1;
			}
		}
	}

	//薄面化
	template < class T, class Allocator, class Neighbor >
	void surface_thinning( array3< T, Allocator > &in, Neighbor __dmy__ )
	{
		typedef typename array3< T, Allocator >::size_type size_type;
		typedef typename array3< T, Allocator >::difference_type difference_type;
		typedef typename array3< T, Allocator >::const_pointer const_pointer;
		typedef typename array3< T, Allocator >::pointer pointer;
		typedef T value_type;
		typedef Neighbor neighbor_type;
		typedef border< T > border_type;
		typedef std::vector< border_type > border_list_type;

		// 図形の端は０
		for( size_type j = 0 ; j < in.height( ) ; j++ )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				in( i, j, 0 ) = 0;
				in( i, j, in.depth( ) - 1 ) = 0;
			}
		}

		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				in( 0, j, k ) = 0;
				in( in.width( ) - 1, j, k ) = 0;
			}
		}

		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				in( i, 0, k ) = 0;
				in( i, in.height( ) - 1, k ) = 0;
			}
		}

		// Step1 距離変換
		euclidean::distance_transform( in, in );

		value_type min = type_limits< value_type >::maximum( ), max = type_limits< value_type >::minimum( );
		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			value_type &v = in[ i ];
			if( v != 0 )
			{
				v += 20;
				if( v > max ) max = v;
				if( v < min ) min = v;
			}
		}

		border_list_type blist, slist[ 9 ];
		difference_type diff[ 27 ], nc6[ 6 ];
		int val[ 27 ];

		{
			difference_type ox = in.width( ) / 2;
			difference_type oy = in.height( ) / 2;
			difference_type oz = in.depth( ) / 2;

			const_pointer p0 = &in( ox, oy, oz );


			diff[  0 ] = &in( ox    , oy    , oz - 1 ) - p0;
			diff[  1 ] = &in( ox    , oy + 1, oz - 1 ) - p0;
			diff[  2 ] = &in( ox - 1, oy + 1, oz - 1 ) - p0;
			diff[  3 ] = &in( ox - 1, oy    , oz - 1 ) - p0;
			diff[  4 ] = &in( ox - 1, oy - 1, oz - 1 ) - p0;
			diff[  5 ] = &in( ox    , oy - 1, oz - 1 ) - p0;
			diff[  6 ] = &in( ox + 1, oy - 1, oz - 1 ) - p0;
			diff[  7 ] = &in( ox + 1, oy    , oz - 1 ) - p0;
			diff[  8 ] = &in( ox + 1, oy + 1, oz - 1 ) - p0;

			diff[  9 ] = &in( ox    , oy    , oz     ) - p0;
			diff[ 10 ] = &in( ox    , oy + 1, oz     ) - p0;
			diff[ 11 ] = &in( ox - 1, oy + 1, oz     ) - p0;
			diff[ 12 ] = &in( ox - 1, oy    , oz     ) - p0;
			diff[ 13 ] = &in( ox - 1, oy - 1, oz     ) - p0;
			diff[ 14 ] = &in( ox    , oy - 1, oz     ) - p0;
			diff[ 15 ] = &in( ox + 1, oy - 1, oz     ) - p0;
			diff[ 16 ] = &in( ox + 1, oy    , oz     ) - p0;
			diff[ 17 ] = &in( ox + 1, oy + 1, oz     ) - p0;

			diff[ 18 ] = &in( ox    , oy    , oz + 1 ) - p0;
			diff[ 19 ] = &in( ox    , oy + 1, oz + 1 ) - p0;
			diff[ 20 ] = &in( ox - 1, oy + 1, oz + 1 ) - p0;
			diff[ 21 ] = &in( ox - 1, oy    , oz + 1 ) - p0;
			diff[ 22 ] = &in( ox - 1, oy - 1, oz + 1 ) - p0;
			diff[ 23 ] = &in( ox    , oy - 1, oz + 1 ) - p0;
			diff[ 24 ] = &in( ox + 1, oy - 1, oz + 1 ) - p0;
			diff[ 25 ] = &in( ox + 1, oy    , oz + 1 ) - p0;
			diff[ 26 ] = &in( ox + 1, oy + 1, oz + 1 ) - p0;

			nc6[ 0 ] = diff[ 12 ];
			nc6[ 1 ] = diff[ 16 ];
			nc6[ 2 ] = diff[ 14 ];
			nc6[ 3 ] = diff[ 10 ];
			nc6[ 4 ] = diff[  0 ];
			nc6[ 5 ] = diff[ 18 ];
		}

		{
			const_pointer p0 = &in[ 0 ];

			// Step2 境界画素の検出
			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				value_type &v = in[ i ];
				if( v > 20 )
				{
					const_pointer p = &v;
					if( p[ nc6[ 0 ] ] == 0 || p[ nc6[ 1 ] ] == 0 ||
						p[ nc6[ 2 ] ] == 0 || p[ nc6[ 3 ] ] == 0 ||
						p[ nc6[ 4 ] ] == 0 || p[ nc6[ 5 ] ] == 0 )
					{
						blist.push_back( border_type( p - p0, v ) );
						v = 1;
					}
				}
			}
		}

		// 以降の処理のためにメモリの予備を確保する
		blist.reserve( blist.size( ) * 2 + 1 );
		slist[ 0 ].reserve( blist.size( ) );
		slist[ 1 ].reserve( blist.size( ) );
		slist[ 2 ].reserve( blist.size( ) );
		slist[ 3 ].reserve( blist.size( ) );
		slist[ 4 ].reserve( blist.size( ) );
		slist[ 5 ].reserve( blist.size( ) );
		slist[ 6 ].reserve( blist.size( ) );
		slist[ 7 ].reserve( blist.size( ) );
		slist[ 8 ].reserve( blist.size( ) );

		size_type count = 0, loop = 0;

		do
		{
			size_type __length__ = 0;

			//Step3 メインサイクル
			for( size_type l = 0 ; l < blist.size( ) ; l++ )
			{
				border_type &b = blist[ l ];
				pointer p = &in[ b.diff ];

				if( b.value <= min )
				{
					create_neighbor_list( p, val, diff );

					// 消去不可能なら永久保存点
					if( !neighbor_type::is_deletable( val ) )
					{
						continue;
					}
					else
					{
						// 図形の厚さが１なら永久保存点
						if( !neighbor_type::is_3d_simplex( val ) )
						{
							continue;
						}
						else
						{
							// 自分自身を除く
							size_type num = val[  0 ];
							num += val[  1 ];
							num += val[  2 ];
							num += val[  3 ];
							num += val[  4 ];
							num += val[  5 ];
							num += val[  6 ];
							num += val[  7 ];
							num += val[  8 ];
							num += val[ 10 ];
							num += val[ 11 ];
							num += val[ 12 ];
							num += val[ 13 ];
							num += val[ 14 ];
							num += val[ 15 ];
							num += val[ 16 ];
							num += val[ 17 ];
							num += val[ 18 ];
							num += val[ 19 ];
							num += val[ 20 ];
							num += val[ 21 ];
							num += val[ 22 ];
							num += val[ 23 ];
							num += val[ 24 ];
							num += val[ 25 ];
							num += val[ 26 ];

							difference_type val = num / 3 + 7;
							b.value = static_cast< value_type >( val );

							if( 7 <= val && val <= 15 )
							{
								slist[ val - 7 ].push_back( b );
								continue;
							}
						}
					}
				}

				blist[ __length__++ ] = b;
			}

			blist.erase( blist.begin( ) + __length__, blist.end( ) );

			// Step4 サブサイクル
			for( size_type ll = 0 ; ll < 9 ; ll++ )
			{
				border_list_type &list = slist[ ll ];

				for( size_type l = 0 ; l < list.size( ) ; l++ )
				{
					border_type &b = list[ l ];
					pointer p = &in[ b.diff ];

					create_neighbor_list( p, val, diff );

					//消去不可能なら永久保存点
					if( !neighbor_type::is_deletable( val ) )
					{
						continue;
					}
					else
					{
						//図形の厚さが１なら永久保存点
						if( !neighbor_type::is_3d_simplex( val ) )
						{
							continue;
						}
						else
						{
							//画素の消去
							p[ 0 ] = 0;

							if( p[ nc6[ 0 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 0 ], p[ nc6[ 0 ] ] ) );
								p[ nc6[ 0 ] ] = 1;
							}
							if( p[ nc6[ 1 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 1 ], p[ nc6[ 1 ] ] ) );
								p[ nc6[ 1 ] ] = 1;
							}
							if( p[ nc6[ 2 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 2 ], p[ nc6[ 2 ] ] ) );
								p[ nc6[ 2 ] ] = 1;
							}
							if( p[ nc6[ 3 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 3 ], p[ nc6[ 3 ] ] ) );
								p[ nc6[ 3 ] ] = 1;
							}
							if( p[ nc6[ 4 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 4 ], p[ nc6[ 4 ] ] ) );
								p[ nc6[ 4 ] ] = 1;
							}
							if( p[ nc6[ 5 ] ] > 20 )
							{
								blist.push_back( border_type( b.diff + nc6[ 5 ], p[ nc6[ 5 ] ] ) );
								p[ nc6[ 5 ] ] = 1;
							}
						}
					}
				}

				// 次以降の処理のためにリスト空にする（内部で使用するメモリ容量は変化しない）
				list.clear( );
			}

			//Step5 終了判定
			min = max;
			count = 0;
			for( size_type l = 0 ; l < blist.size( ) ; l++ )
			{
				border_type &b = blist[ l ];
				if( b.value < min && b.value > 20 )
				{
					min = b.value;
				}
			}

			std::cout << loop++ << "          \r";

		} while( min < max || blist.size( ) > 0 );

		std::cout << std::endl;

		// Step6 後処理
		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			if( in[ i ] != 0 )
			{
				in[ i ] = 1;
			}
		}
	}
}



/// @brief C. J. Hilditch の細線化アルゴリズム
namespace hilditch
{
	//! @addtogroup thinning_group 細線化
	//!
	//! @code 次のヘッダをインクルードする
	//! #include <mist/filter/thinning.h>
	//! @endcode
	//!
	//!  @{

	/// @brief 2次元画像に対する細線化アルゴリズム
	//!
	//! 細線化結果は8連結となる
	//!
	//! @attention 入力と出力が同じ画像オブジェクトでも正しく細線化を行うことが可能です
	//! @attention 3次元画像に対して適用した場合の結果は正しくないので注意が必要です
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．もし，画像として保存したい場合は本関数の実行後に画素値が1のものを255に置き換えてください．
	//!
	//! - 参考文献
	//!   - C. J. Hilditch, ``Linear Skeleton from Square Cupboards,'' In: Machine Intelligence 6, B. Meltzer and D. Michie eds., Edinburgh Univ. Press, pp.403?420, 1969
	//!   - 鳥脇純一郎, ``画像理解のためのディジタル画像処理〔II〕，'' 昭晃堂，pp.56-59，1988
	//! 
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
	{
		typedef typename array2< T2, Allocator2 >::size_type size_type;
		typedef typename array2< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] > 0 ? 1 : 0;
		}

		__thinning_controller__::thinning( out );
	}

	/// @}
	//  細線化グループの終わり
}


/// @brief ユークリッド距離変換を用いた細線化アルゴリズム
namespace euclidean
{
	//! @addtogroup thinning_group 細線化
	//!  @{

	/// @brief ユークリッド距離を用いた2次元画像に対する細線化アルゴリズム
	//!
	//! 細線化結果は8連結となる
	//!
	//! - 参考文献
	//! - 鈴木智, 阿部圭一, ``距離変換の結果を利用した二値画像の逐次細線化,'' 電子情報通信学会論文誌D, vol.68-D, no.4, pp.473-480, 1985.
	//!
	//! @attention 入力と出力が同じ画像オブジェクトでも正しく細線化を行うことが可能です
	//! @attention 3次元画像に対して適用した場合の結果は正しくないので注意が必要です
	//!
	//! @attention 画素のアスペクト比が1でない場合は，出力画像の型をfloatかdoubleにしないと正確な結果が得られません．十分に注意してください．
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．もし，画像として保存したい場合は本関数の実行後に画素値が1のものを255に置き換えてください．
	//!
	//! 細線化を行う際に，出力画像の要素のデータ型で一度ユークリッド2乗距離変換した後，細線化処理が実行されます．
	//! そのため，出力画像のデータ型が unsinged char の場合には，画像のサイズによっては距離変換が最後まで正しく行われません．
	//! 細線化を実行する場合は，unsigned short かそれよりも大きい値を表現できるデータ型を利用してください． 
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
	{
		typedef typename array2< T2, Allocator2 >::size_type size_type;
		typedef typename array2< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ) );

		if( in.reso1( ) <= in.reso2( ) || is_float< value_type >::value )
		{
			out.reso1( in.reso1( ) );
			out.reso2( in.reso2( ) );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::thinning( out );
		}
		else
		{
			double r1 = in.reso1( );
			double r2 = in.reso2( );

			out.reso1( 1.0 );
			out.reso2( 1.0 );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::thinning( out );

			out.reso1( r1 );
			out.reso2( r2 );
		}
	}


	/// @brief 3次元画像に対する収縮スケルトンアルゴリズム（6連結）
	//!
	//!	- 参考文献
	//! - 米倉達広, 横井茂樹, 鳥脇純一郎, 福村晃夫, ``3次元ディジタル画像における1-要素の消去可能性と図形収縮について,'' 電子情報通信学会論文誌D, vol.65-D, no.12, pp.1543-1549, 1982
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void shrink_skelton6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );

		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
		}
		__euclidean_utility__::shrink_skelton( out, __euclidean_utility__::neighbor< 6 >( ) );
	}


	/// @brief 3次元画像に対する収縮スケルトンアルゴリズム（26連結）
	//!
	//!	- 参考文献
	//! - 米倉達広, 横井茂樹, 鳥脇純一郎, 福村晃夫, ``3次元ディジタル画像における1-要素の消去可能性と図形収縮について,'' 電子情報通信学会論文誌D, vol.65-D, no.12, pp.1543-1549, 1982
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void shrink_skelton26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );

		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
		}
		__euclidean_utility__::shrink_skelton( out, __euclidean_utility__::neighbor< 26 >( ) );
	}


	/// @brief ユークリッド距離を用いた3次元画像に対する細線化アルゴリズム
	//!
	//! 細線化結果は6連結となる
	//!
	//!	- 参考文献
	//!	- 斉藤豊文, 森健策, 鳥脇純一郎, ``ユークリッド距離変換を用いた3次元ディジタル画像の薄面化および細線化の逐次型アルゴリズムとその諸性質,'' 電子情報通信学会論文誌D-II, vol.J79-D-II, no.10, pp.1675-1685, 1996
	//!
	//! @attention 入力と出力が同じ画像オブジェクトでも正しく細線化することが可能です
	//!
	//! @attention 画素のアスペクト比が1でない場合は，出力画像の型をfloatかdoubleにしないと正確な結果が得られません．十分に注意してください．
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．
	//!
	//! 細線化を行う際に，出力画像の要素のデータ型で一度ユークリッド2乗距離変換した後，細線化処理が実行されます．
	//! そのため，出力画像のデータ型が unsinged char の場合には，画像のサイズによっては距離変換が最後まで正しく行われません．
	//! 細線化を実行する場合は，unsigned short かそれよりも大きい値を表現できるデータ型を利用してください． 
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );

		if( ( in.reso1( ) <= in.reso2( ) && in.reso1( ) <= in.reso3( ) ) || is_float< value_type >::value )
		{
			out.reso1( in.reso1( ) );
			out.reso2( in.reso2( ) );
			out.reso3( in.reso3( ) );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::thinning( out, __euclidean_utility__::neighbor< 6 >( ) );
		}
		else
		{
			double r1 = in.reso1( );
			double r2 = in.reso2( );
			double r3 = in.reso3( );

			out.reso1( 1.0 );
			out.reso2( 1.0 );
			out.reso3( 1.0 );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::thinning( out, __euclidean_utility__::neighbor< 6 >( ) );

			out.reso1( r1 );
			out.reso2( r2 );
			out.reso3( r3 );
		}
	}


	/// @brief ユークリッド距離を用いた3次元画像に対する細線化アルゴリズム
	//!
	//! 細線化結果は26連結となる
	//!
	//!	- 参考文献
	//!	- 斉藤豊文, 森健策, 鳥脇純一郎, ``ユークリッド距離変換を用いた3次元ディジタル画像の薄面化および細線化の逐次型アルゴリズムとその諸性質,'' 電子情報通信学会論文誌D-II, vol.J79-D-II, no.10, pp.1675-1685, 1996
	//!
	//! @attention 入力と出力が同じ画像オブジェクトでも正しく細線化することが可能です
	//!
	//! @attention 画素のアスペクト比が1でない場合は，出力画像の型をfloatかdoubleにしないと正確な結果が得られません．十分に注意してください．
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．
	//!
	//! 細線化を行う際に，出力画像の要素のデータ型で一度ユークリッド2乗距離変換した後，細線化処理が実行されます．
	//! そのため，出力画像のデータ型が unsinged char の場合には，画像のサイズによっては距離変換が最後まで正しく行われません．
	//! 細線化を実行する場合は，unsigned short かそれよりも大きい値を表現できるデータ型を利用してください． 
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );

		if( ( in.reso1( ) <= in.reso2( ) && in.reso1( ) <= in.reso3( ) ) || is_float< value_type >::value )
		{
			out.reso1( in.reso1( ) );
			out.reso2( in.reso2( ) );
			out.reso3( in.reso3( ) );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::thinning( out, __euclidean_utility__::neighbor< 26 >( ) );
		}
		else
		{
			double r1 = in.reso1( );
			double r2 = in.reso2( );
			double r3 = in.reso3( );

			out.reso1( 1.0 );
			out.reso2( 1.0 );
			out.reso3( 1.0 );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::thinning( out, __euclidean_utility__::neighbor< 26 >( ) );

			out.reso1( r1 );
			out.reso2( r2 );
			out.reso3( r3 );
		}
	}

	/// @brief ユークリッド距離を用いた3次元画像に対する薄面化アルゴリズム
	//!
	//! 薄面化結果は6連結となる
	//!
	//!	- 参考文献
	//!	- 斉藤豊文, 森健策, 鳥脇純一郎, ``ユークリッド距離変換を用いた3次元ディジタル画像の薄面化および細線化の逐次型アルゴリズムとその諸性質,'' 電子情報通信学会論文誌D-II, vol.J79-D-II, no.10, pp.1675-1685, 1996
	//!
	//! @attention 入力と出力が同じ画像オブジェクトでも正しく細線化することが可能です
	//!
	//! @attention 画素のアスペクト比が1でない場合は，出力画像の型をfloatかdoubleにしないと正確な結果が得られません．十分に注意してください．
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void surface_thinning6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );

		if( ( in.reso1( ) <= in.reso2( ) && in.reso1( ) <= in.reso3( ) ) || is_float< value_type >::value )
		{
			out.reso1( in.reso1( ) );
			out.reso2( in.reso2( ) );
			out.reso3( in.reso3( ) );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::surface_thinning( out, __euclidean_utility__::neighbor< 6 >( ) );
		}
		else
		{
			double r1 = in.reso1( );
			double r2 = in.reso2( );
			double r3 = in.reso3( );

			out.reso1( 1.0 );
			out.reso2( 1.0 );
			out.reso3( 1.0 );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::surface_thinning( out, __euclidean_utility__::neighbor< 6 >( ) );

			out.reso1( r1 );
			out.reso2( r2 );
			out.reso3( r3 );
		}
	}


	/// @brief ユークリッド距離を用いた3次元画像に対する薄面化アルゴリズム
	//!
	//! 薄面化結果は26連結となる
	//!
	//!	- 参考文献
	//!	- 斉藤豊文, 森健策, 鳥脇純一郎, ``ユークリッド距離変換を用いた3次元ディジタル画像の薄面化および細線化の逐次型アルゴリズムとその諸性質,'' 電子情報通信学会論文誌D-II, vol.J79-D-II, no.10, pp.1675-1685, 1996
	//!
	//! @attention 入力と出力が同じ画像オブジェクトでも正しく細線化することが可能です
	//!
	//! @attention 画素のアスペクト比が1でない場合は，出力画像の型をfloatかdoubleにしないと正確な結果が得られません．十分に注意してください．
	//!
	//! @attention 出力画像では，背景に0，前景に1が代入されています．
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像（0か1の2値）
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void surface_thinning26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out )
	{
		typedef typename array3< T2, Allocator2 >::size_type  size_type;
		typedef typename array3< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ), in.size3( ) );

		if( ( in.reso1( ) <= in.reso2( ) && in.reso1( ) <= in.reso3( ) ) || is_float< value_type >::value )
		{
			out.reso1( in.reso1( ) );
			out.reso2( in.reso2( ) );
			out.reso3( in.reso3( ) );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::surface_thinning( out, __euclidean_utility__::neighbor< 26 >( ) );
		}
		else
		{
			double r1 = in.reso1( );
			double r2 = in.reso2( );
			double r3 = in.reso3( );

			out.reso1( 1.0 );
			out.reso2( 1.0 );
			out.reso3( 1.0 );

			for( size_type i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
			}
			__euclidean_utility__::surface_thinning( out, __euclidean_utility__::neighbor< 26 >( ) );

			out.reso1( r1 );
			out.reso2( r2 );
			out.reso3( r3 );
		}
	}

	/// @}
	//  細線化グループの終わり
}



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_THINNING__
