#ifndef __INCLUDE_MIST_MEDIAN_FILTER__
#define __INCLUDE_MIST_MEDIAN_FILTER__


#include "../mist.h"
#include "../thread.h"
#include <algorithm>
#include <functional>

// mist名前空間の始まり
_MIST_BEGIN


// メディアンフィルタ
namespace __median_filter__
{
	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh : フィルタサイズ
	// min, max : 濃淡範囲
	template < class T, class Allocator >
	void median_filter( const array< T, Allocator > &in, array< T, Allocator > &out, typename array< T, Allocator >::size_type fw,
									typename array< T, Allocator >::value_type min, typename array< T, Allocator >::value_type max )
	{
		typedef array< T, Allocator >::size_type  size_type;
		typedef T value_type;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a = 0, i, x, ri;
		signed int pth, th, lt_med;
		value_type med;

		size_type w = in.size( );
		size_type step = w;

		size_type bw = fw / 2;

		value_type leftmost;
		value_type *sort = new value_type[ fw + 1 ];
		unsigned short *hist = new unsigned short[ range ];

		i = 0;

		memset( hist, 0, sizeof( unsigned short ) * range );
		pth = 0;
		leftmost = in[ 0 ] - min;
		for( x = 0 ; x <= bw ; x++ )
		{
			sort[ pth++ ] = in[ x ] - min;
			hist[ in[ x ] - min ]++;
		}

		// 昇順に並べ替える
		std::sort( sort, sort + pth );
		pth--;

		th = pth / 2;
		med = sort[ th ];

		if( sort[ 0 ] == sort[ th ] )
		{
			lt_med = 0;
		}
		else
		{
			a = th;
			while( med <= sort[ --a ] );
			lt_med = static_cast< signed int >( a + 1 );
		}

		out[ i ] = med + min;

		for( i = 1 ; i < w ; i++ )
		{
			ri = i + bw;

			if( bw < i )
			{
				hist[ leftmost ]--;
				pth--;
				if( leftmost < med )
				{
					lt_med--;
				}
			}

			if( ri < w )
			{
				hist[ in[ ri ] - min ]++;
				pth++;
				if( in[ ri ] - min < med )
				{
					lt_med++;
				}
			}

			if ( bw <= i )
			{
				leftmost = in[ i - bw ] - min;
			}

			th = pth / 2;
			if( lt_med <= th )
			{
				while( lt_med + hist[ med ] <= th )
				{
					lt_med += hist[ med ];
					med++;
				}
			}
			else
			{
				while( th < lt_med )
				{
					med--;
					lt_med -= hist[ med ];
				}
			}

			out[ i ] = med + min;
		}

		delete [] hist;
		delete [] sort;
	}

	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh : フィルタサイズ
	// min, max : 濃淡範囲
	template < class T, class Allocator >
	void median_filter( const array2< T, Allocator > &in, array2< T, Allocator > &out,
						typename array2< T, Allocator >::size_type fw, typename array2< T, Allocator >::size_type fh,
						typename array2< T, Allocator >::value_type min, typename array2< T, Allocator >::value_type max )
	{
		typedef array2< T, Allocator >::size_type  size_type;
		typedef T value_type;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a = 0, i, j, x, y, ri, leftnum;
		signed int pth, th, lt_med;
		value_type med;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type step = w;

		size_type bw = fw / 2;
		size_type bh = fh / 2;

		value_type *leftmost = new value_type[ fh ];
		value_type *sort = new value_type[ fw * fh + 1 ];
		unsigned short *hist = new unsigned short[ range ];

		for( j = 0 ; j < h ; j++ )
		{
			i = 0;

			memset( hist, 0, sizeof( unsigned short ) * range );
			pth = 0;
			leftnum = 0;
			for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
			{
				leftmost[ leftnum++ ] = in( 0, y ) - min;
				for( x = 0 ; x <= bw ; x++ )
				{
					sort[ pth++ ] = in( x, y ) - min;
					hist[ in( x, y ) - min ]++;
				}
			}

			// 昇順に並べ替える
			std::sort( sort, sort + pth );
			pth--;

			th = pth / 2;
			med = sort[ th ];

			if( sort[ 0 ] == sort[ th ] )
			{
				lt_med = 0;
			}
			else
			{
				a = th;
				while( med <= sort[ --a ] );
				lt_med = static_cast< signed int >( a + 1 );
			}

			out( i, j ) = med + min;

			for( i = 1 ; i < w ; i++ )
			{
				ri = i + bw;

				if( bw < i )
				{
					for( a = 0 ; a < leftnum ; a++ )
					{
						hist[ leftmost[ a ] ]--;
						pth--;
						if( leftmost[ a ] < med )
						{
							lt_med--;
						}
					}
				}

				leftnum = 0;
				for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
				{
					if( ri < w )
					{
						hist[ in( ri, y ) - min ]++;
						pth++;
						if( in( ri, y ) - min < med )
						{
							lt_med++;
						}
					}

					if( bw <= i )
					{
						leftmost[ leftnum++ ] = in( i - bw, y ) - min;
					}
				}

				th = pth / 2;
				if( lt_med <= th )
				{
					while( lt_med + hist[ med ] <= th )
					{
						lt_med += hist[ med ];
						med++;
					}
				}
				else
				{
					while( th < lt_med )
					{
						med--;
						lt_med -= hist[ med ];
					}
				}

				out( i, j ) = med + min;
			}
		}

		delete [] hist;
		delete [] sort;
		delete [] leftmost;
	}



	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh, fd : フィルタサイズ
	// min, max : 濃淡範囲
	template < class T, class Allocator >
	void median_filter( const array3< T, Allocator > &in, array3< T, Allocator > &out,
						typename array3< T, Allocator >::size_type fw, typename array3< T, Allocator >::size_type fh, typename array3< T, Allocator >::size_type fd,
						typename array3< T, Allocator >::value_type min, typename array3< T, Allocator >::value_type max )
	{
		typedef array3< T, Allocator >::size_type  size_type;
		typedef T value_type;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a = 0, i, j, k, x, y, z, ri, leftnum;
		signed int pth, th, lt_med;
		value_type med;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );
		size_type step = w * h;

		size_type bw = fw / 2;
		size_type bh = fh / 2;
		size_type bd = fd / 2;

		value_type *leftmost = new value_type[ fh * fd ];
		value_type *sort = new value_type[ fw * fh * fd + 1 ];
		unsigned short *hist = new unsigned short[range];

		for( k = 0 ; k < d ; k++ )
		{
			for( j = 0 ; j < h ; j++ )
			{
				i = 0;

				memset( hist, 0, sizeof( unsigned short ) * range );
				pth = 0;
				leftnum = 0;
				for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
				{
					for(y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
					{
						leftmost[ leftnum++ ] = in( 0, y, z ) - min;
						for( x = 0 ; x <= bw ; x++ )
						{
							sort[ pth++ ] = in( x, y, z ) - min;
							hist[ in( x, y, z ) - min ]++;
						}
					}
				}

				// 昇順に並べ替える
				std::sort( sort, sort + pth );
				pth--;

				th = pth / 2;
				med = sort[ th ];

				if( sort[ 0 ] == sort[ th ] )
				{
					lt_med = 0;
				}
				else
				{
					a = th;
					while( med <= sort[ --a ] );
					lt_med = static_cast< signed int >( a + 1 );
				}

				out( i, j, k ) = med + min;

				for( i = 1 ; i < w ; i++ )
				{
					ri = i + bw;

					if( bw < i )
					{
						for( a = 0 ; a < leftnum ; a++ )
						{
							hist[ leftmost[ a ] ]--;
							pth--;
							if( leftmost[ a ] < med )
							{
								lt_med--;
							}
						}
					}

					leftnum = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							if( ri < w )
							{
								hist[ in( ri, y, z ) - min ]++;
								pth++;
								if( in( ri, y, z ) - min < med )
								{
									lt_med++;
								}
							}

							if ( bw <= i )
							{
								leftmost[ leftnum++ ] = in( i - bw, y, z ) - min;
							}
						}
					}

					th = pth / 2;

					if( lt_med <= th )
					{
						while( lt_med + hist[ med ] <= th )
						{
							lt_med += hist[ med ];
							med++;
						}
					}
					else
					{
						while( th < lt_med )
						{
							med--;
							lt_med -= hist[ med ];
						}
					}

					out( i, j, k ) = med + min;
				}
			}
		}

		delete [] hist;
		delete [] sort;
		delete [] leftmost;
	}
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MEDIAN_FILTER__
