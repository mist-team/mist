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
	struct Position
	{
		ptrdiff_t	x;
		ptrdiff_t	y;
		size_t		count;
	};

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
			if( mask[i] == false )
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
					pt.count = 0;
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
		double rr, rrr, minimum_length;
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

		typedef Position position_type;
		std::deque< position_type > list;
		size_type label_count = 0;
		size_type loop_count = 0;
		value_type current_label = 0;
		std::deque< position_type >::iterator ite;

		array2< binary >  mmmm( in.width( ), in.height( ), in.reso1( ), in.reso2( ), false );

		out.fill( );

		printf( "looping start\n" );
		while( true )
//		while( loop_count < 500 )
		{
			rr = search_max( dist, mmmm, list );

			if( list.empty( ) ) break;

			rx = static_cast< int >( ceil( sqrt( (double)rr ) ) );
			ry = static_cast< int >( ceil( sqrt( (double)rr ) / aspect ) );

			printf( "                                                                   \r" );
			printf( "looping ... % 4d, label =% 4d, size =% 3d, radius =%.3f )\r", ++loop_count, label_count, list.size( ), sqrt( (double)rr ) );

			for( ite = list.begin( ) ; ite != list.end( ) ; ++ite )
			{
				position_type &pt = *ite;

				current_label = 0;
				minimum_length = 1.0e30;
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
						if( rrr <= rr )
						{
							if( out( px, py ) != 0 )
							{
								if( out( px, py ) != current_label )
								{
									pt.count++;
								}
								if( minimum_length > rrr )
								{
									minimum_length = rrr;
									current_label = out( px, py );
								}
							}
						}
					}
				}

				// 他の領域から塗られていないので，新しいラベルとする
				if( current_label == 0 )
				{
					label_count++;
					if( label_count > label_max ) label_count = label_max + 1;
					unsigned char ll[] = { 0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250 };
					out( pt.x, pt.y ) = ll[ label_count ];
				}
				else
				{
					out( pt.x, pt.y ) = current_label;
				}

				// 次回の最大値検索から外れるようにする
				mmmm( pt.x, pt.y ) = true;
			}

			//for( ite = list.begin( ) ; ite != list.end( ) ; ++ite )
			//{
			//	position_type &pt = *ite;
			//	if( pt.count == 1 ) continue;

			//	value_type start_label = out( pt.x, pt.y );
			//	current_label = 0;
			//	minimum_length = 1.0e30;
			//	for( j = -ry ; j <= ry ; j++ )
			//	{
			//		jj = j * j * aspect * aspect;
			//		py = j + pt.y;
			//		if( py < 0 || py >= in.height( ) ) continue;

			//		for( i = -rx ; i <= rx ; i++ )
			//		{
			//			ii = i * i;
			//			px = i + pt.x;
			//			if( px < 0 || px >= in.width( ) ) continue;

			//			rrr = ii + jj;
			//			if( rrr <= rr )
			//			{
			//				if( out( px, py ) != 0 )
			//				{
			//					if( minimum_length > rrr )
			//					{
			//						minimum_length = rrr;
			//						current_label = out( px, py );
			//					}
			//					else if( minimum_length == rrr && out( px, py ) != start_label )
			//					{
			//						current_label = out( px, py );
			//					}
			//				}
			//			}
			//		}
			//	}
			//	out( pt.x, pt.y ) = current_label;
			//}

			draw_area->redraw( );
			Fl::wait( 0 );
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
