#ifndef __INCLUDE_MIST_FIGURE_DECOMPOSITION__
#define __INCLUDE_MIST_FIGURE_DECOMPOSITION__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_BINARY_H__
#include "../config/binary.h"
#endif

#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#include "distance.h"
#endif


#include <deque>


// mist名前空間の始まり
_MIST_BEGIN


// ユークリッド型距離変換
namespace __distance_figure_dedomposition__
{
	typedef struct
	{
		short x;
		short y;
		short label;
	} Position;

	template < class T, class Allocator1, class Allocator2 >
	double search_max( const array2< T, Allocator1 > &ia, const array2< binary, Allocator2 > &mask, std::deque< Position > &list )
	{
		list.clear( );

		if( ia.empty( ) ) return( 0.0 );
		if( ia.size( ) != mask.size( ) ) return( 0.0 );

		typedef typename array2< T, Allocator1 >::size_type  size_type;
		typedef typename array2< T, Allocator1 >::value_type value_type;

		value_type max;
		max = ia[0];
		size_type i, j;
		for( i = 1 ; i < ia.size( ) ; i++ )
		{
			if( !mask[i] )
			{
				max = max > ia[i] ? max : ia[i];
			}
		}

		if( max == 0 ) return( 0.0 );

		for( j = 0 ; j < ia.height( ) ; j++ )
		{
			for( i = 0 ; i < ia.width( ) ; i++ )
			{
				if( ia( i, j ) == max )
				{
					Position pt;
					pt.x = i;
					pt.y = j;
					pt.label = 0;
					list.push_back( pt );
				}
			}
		}
		return( max );
	}

	template < class T, class Allocator >
	typename array2< T, Allocator >::size_type figure_decomposition( const array2< T, Allocator > &in, array2< T, Allocator > &out,
																					typename array2< T, Allocator >::value_type label_max )
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		array2< unsigned short > dist( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );

		int i, j;
		double ii, jj;
		int rx, ry;
		double rr, rrr;
		size_type px, py;
		double aspect = in.reso2( ) / in.reso1( );

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			dist[i] = in[i];
		}

		printf( "disteu start\n" );
		euclidean_distance_transform( dist, dist );

		//out = dist;
		//return(0);

		std::deque< Position > list;
		size_type label_count = 0;
		size_type loop_count = 0;
		value_type current_label = 0;
		Position pt;

		array2< float >   mask( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
		array2< binary >  mmmm( in.width( ), in.height( ), in.reso1( ), in.reso2( ), false );
		array2< binary >  mtmp( in.width( ), in.height( ), in.reso1( ), in.reso2( ), false );

		out.fill( );
		mask.fill( 512.0f * 512.0f );

		printf( "looping start\n" );
		while( loop_count < 500 )
		{
			rr = search_max( dist, mmmm, list );

			if( list.empty( ) ) break;

			rx = static_cast< int >( ceil( sqrt( (double)rr ) ) );
			ry = static_cast< int >( ceil( sqrt( (double)rr ) / aspect ) );

			printf( "                                                                   \r" );
			printf( "looping ... % 4d, label =% 4d, size =% 3d, radius =%.3f )\r", ++loop_count, label_count, list.size( ), sqrt( (double)rr ) );

			while( !list.empty( ) )
			{
				std::deque< Position >::iterator ite = list.begin( );
				for( ; ite != list.end( ) ; ++ite )
				{
					Position &ppt = *ite;
					if( out( ppt.x, ppt.y ) != 0 ) break;
				}
				if( ite == list.end( ) )
				{
					// すでにラベルが割り振られているものが見つからなかった場合
					pt = list.front( );
					list.pop_front( );
				}
				else
				{
					pt = *ite;
					list.erase( ite );
				}

				// 他の領域から塗られていないので，新しいラベルとする
				if( out( pt.x, pt.y ) == 0 )
				{
					label_count++;
					if( label_count > label_max ) label_count = label_max + 1;
					current_label = label_count;
				}
				else
				{
					current_label = out( pt.x, pt.y );
				}

				for( j = -ry ; j <= ry ; j++ )
				{
					jj = j * j * aspect * aspect;
					py = j + pt.y;
					if( py < 0 || py >= in.height( ) ) continue;

					for( i = -rx ; i <= rx ; i++ )
					{
						ii = i * i;
						px = i + pt.x;
						if( px < 0 || px >= in.width( ) ) continue;

						rrr = ii + jj;
						if( rrr < rr )
						{
							if( dist( px, py ) <= rr )
							{
								if( mask( px, py ) > rrr )
								{
									mask( px, py ) = static_cast< float >( rrr );
									// 現在の復元における近い領域のほうにラベルを割り当てる
	//								if( out( px, py ) < current_label )
									{
										out( px, py ) = current_label;
	//									out( px, py ) = mmmm( px, py ) ? out( px, py ) : current_label;
									}
								}
							}
						}
					}
				}

				// 次回の最大値検索から外れるようにする
				mmmm( pt.x, pt.y ) = true;
			}
		}

		// MAXラベル数を超えたものを除去
		for( i = 0 ; i < out.size( ) ; i++ )
		{
			out[i] = out[i] > label_max ? 0 : out[i];
		}

		return( current_label );
	}

}




// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FIGURE_DECOMPOSITION__
