#ifndef __INCLUDE_MIST_MEDIAN_FILTER__
#define __INCLUDE_MIST_MEDIAN_FILTER__


#include "../mist.h"
#include "../thread.h"
#include <algorithm>

// mist名前空間の始まり
_MIST_BEGIN


// メディアンフィルタ
// ti : 入力 & 結果画像. 入力画像の画素値は正とする.
// fw, fh, fd : フィルタサイズ.
// range : 濃淡範囲( >= 0 ).
namespace __median_filter__
{
	template < class T, class Allocator >
	void median_filter( const array3< T, Allocator > &in, array3< T, Allocator > &out,
						typename array3< T, Allocator >::size_type fw, typename array3< T, Allocator >::size_type fh, typename array3< T, Allocator >::size_type fd,
						typename array3< T, Allocator >::value_type min, typename array3< T, Allocator >::value_type max )
	{
		typedef array3< T, Allocator >::size_type  size_type;
		typedef T value_type;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a, i, j, k, x, y, z, pth, li, ri, leftnum;
		size_type th, lt_med;
		value_type med;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );
		size_type step = w * h;

		size_type bw = fw / 2;
		size_type bh = fh / 2;
		size_type bd = fd / 2;

		value_type *leftmost = new value_type[ fh * fd ];
		value_type *sort = new value_type[ fw * fh * fd ];
		unsigned short *hist = new unsigned short[range];

		for( k = 0 ; k < d ; k++ )
		{
			for( j = 0 ; j < h ; j++ )
			{
				i = 0;

				memset( hist, 0, sizeof( unsigned short ) * range );
				pth = 0;
				leftnum = 0;
				for( z = k - bd ; z <= k + bd ; z++ )
				{
					for(y = j - bh ; y <= j + bh ; y++ )
					{
						for( x = 0 ; x <= bw ; x++ )
						{
							if( z < 0 || d - 1 < z || y < 0 || h - 1 < y )
							{
								continue;
							}

							if( x == 0 )
							{
								leftmost[ leftnum++ ] = in( x, y, z ) - min;
							}

							sort[ pth++ ] = in( x, y, z ) - min;
							hist[ in( x, y, z ) - min ]++;
						}
					}
				}
				pth--;
				std::sort( sort, sort + pth );

				th = pth / 2;
				med = sort[th];

				if( sort[0] == sort[th] )
				{
					lt_med = 0;
				}
				else
				{
					a = th;
					while( med <= sort[ --a ] );
					lt_med = a + 1;
				}

				out( i, j, k ) = med + min;

				for( i = 1 ; i < w ; i++ )
				{
					li = i - bw;
					ri = i + bw;

					if( 0 < li )
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
					for( z = k - bd ; z <= k + bd ; z++ )
					{
						for( y = j - bh ; y <= j + bh ; y++ )
						{
							if( z < 0 || d - 1 < z || y < 0 || h - 1 < y )
							{
								continue;
							}

							if( ri < w )
							{
								hist[ in( ri, y, z ) - min ]++;
								pth++;
								if( in( ri, y, z ) - min < med )
								{
									lt_med++;
								}
							}

							if ( 0 <= li )
							{
								leftmost[ leftnum++ ] = in( li, y, z ) - min;
							}
						}
					}

					th = pth / 2;
					if( lt_med <= th )
					{
						while( lt_med + hist[med] <= th )
						{
							lt_med += hist[med];
							med++;
						}
					}
					else
					{
						while( th < lt_med )
						{
							med--;
							lt_med -= hist[med];
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
