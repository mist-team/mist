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
	} Position;

	template < class T, class Allocator1, class Allocator2 >
	typename array2< T, Allocator1 >::value_type search_max( const array2< T, Allocator1 > &in, const array2< binary, Allocator2 > &mask, std::deque< Position > &list )
	{
		list.clear( );

		if( in.empty( ) ) return( 0 );
		if( in.size( ) != mask.size( ) ) return( 0 );

		typedef typename array2< T, Allocator1 >::size_type  size_type;
		typedef typename array2< T, Allocator1 >::value_type value_type;

		value_type max;
		max = in[0];
		size_type i, j;
		for( i = 1 ; i < in.size( ) ; i++ )
		{
			if( !mask[i] )
			{
				max = max > in[i] ? max : in[i];
			}
		}

		if( max == 0 ) return( max );

		for( j = 0 ; j < in.height( ) ; j++ )
		{
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				if( in( i, j ) == max )
				{
					Position pt;
					pt.x = i;
					pt.y = j;
					list.push_back( pt );
				}
			}
		}
		return( max );
	}


	template < class T1, class T2, class Allocator1, class Allocator2 >
	void paint_surroundings( array2< T1, Allocator1 > &out, const array2< T2, Allocator2 > &dist,
								typename array2< T1, Allocator1 >::size_type x, typename array2< T1, Allocator1 >::size_type y,
								typename array2< T1, Allocator1 >::value_type label )
	{
		typename array2< T1, Allocator1 >::size_type px, py;
		array2< T2, Allocator2 >::value_type length = dist( x, y );
		int i, j;
		std::deque< Position > list;
		Position pt;
		pt.x = x;
		pt.y = y;

		list.push_back( pt );

		out( x, y ) = label;

		while( !list.empty( ) )
		{
			Position p = list.front( );
			list.pop_front( );

			for( j = -5 ; j <= 5 ; j++ )
			{
				py = j + p.y;
				if( py < 0 || py >= out.height( ) ) continue;

				for( i = -5 ; i <= 5 ; i++ )
				{
					px = i + p.x;
					if( px < 0 || px >= out.width( ) ) continue;
					if( i == 0 && j == 0 ) continue;
					if( out( px, py ) == 0 && dist( px, py ) == length )
					{
						out( px, py ) = label;
						pt.x = px;
						pt.y = py;
						list.push_back( pt );
					}
				}
			}
		}
	}


	template < class T, class Allocator >
	typename array2< T, Allocator >::size_type figure_decomposition( const array2< T, Allocator > &in, array2< T, Allocator > &out,
																					typename array2< T, Allocator >::value_type label_max )
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		array2< double > dist( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );

		int i, j;
		double ii, jj;
		int rx, ry;
		double rr, rrr, max;
		size_type px, py, l;
		double aspect = in.reso2( ) / in.reso1( );

		for( l = 0 ; l < in.size( ) ; l++ )
		{
			dist[l] = in[l];
		}

		printf( "disteu start\n" );
		euclidean_distance_transform( dist, dist );

		//for( l = 0 ; l < in.size( ) ; l++ )
		//{
		//	if( dist[l] >= 0 && dist[l] < 256 )
		//	{
		//		out[l] = dist[l];
		//	}
		//}
		//return(0);

		std::deque< Position > list;
		std::deque< Position > que1, que2;
		std::deque< Position >::iterator ite;

		size_type label_count = 0;
		size_type loop_count = 0;
		value_type current_label = 0;
		Position pt;

		array2< double >  mask( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
		array2< binary >  mmmm( in.width( ), in.height( ), in.reso1( ), in.reso2( ), false );

		out.fill( );
		mask.fill( 512.0f * 512.0f );
		mmmm.fill( false );

		printf( "looping start\n" );
		while( true )
//		while( loop_count < 100 )
		{
			rr = search_max( dist, mmmm, list );

			if( list.empty( ) ) break;

			rx = static_cast< int >( ceil( sqrt( (double)rr ) ) );
			ry = static_cast< int >( ceil( sqrt( (double)rr ) / aspect ) );

			printf( "                                                                   \r" );
			printf( "looping ... % 4d, label =% 4d, size =% 3d, radius =%.3f )\r", ++loop_count, label_count, list.size( ), sqrt( (double)rr ) );

			ite = list.begin( );
			for( ; ite != list.end( ) ; ++ite )
			{
				Position &ppt = *ite;
				if( out( ppt.x, ppt.y ) != 0 )
				{
					que1.push_back( ppt );
				}
				else
				{
					que2.push_back( ppt );
				}
			}

			while( !que1.empty( ) || !que2.empty( ) )
			{
				if( !que1.empty( ) )
				{
					// すでにラベルが割り振られているものが見つからなかった場合
					pt = que1.front( );
					que1.pop_front( );
				}
				else
				{
					// すでにラベルが割り振られているものが見つからなかった場合
					pt = que2.front( );
					que2.pop_front( );
				}

				// 他の領域から塗られていないので，新しいラベルとする
				if( out( pt.x, pt.y ) == 0 )
				{
					label_count++;
					if( label_count > label_max ) label_count = label_max + 1;
					current_label = label_count;

					// 近傍で同じ距離値に同じラベルを割り当てる
					//paint_surroundings( out, dist, pt.x, pt.y, current_label );
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
							if( mask( px, py ) > rrr - 1 )
							{
								// 現在の復元における近い領域のほうにラベルを割り当てる
								mask( px, py ) = rrr;
								out( px, py ) = current_label;
							}
						}
					}
				}


				// 次回の最大値検索から外れるようにする
				mmmm( pt.x, pt.y ) = true;

				// 前段階でラベルの割り当てられている画素がない場合のみを検出する
				if( que1.empty( ) )
				{
					list = que2;
					que1.clear( );
					que2.clear( );
					ite = list.begin( );
					for( ; ite != list.end( ) ; ++ite )
					{
						Position &ppt = *ite;
						if( out( ppt.x, ppt.y ) != 0 )
						{
							que1.push_back( ppt );
						}
						else
						{
							que2.push_back( ppt );
						}
					}
				}
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
