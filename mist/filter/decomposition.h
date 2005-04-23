#ifndef __INCLUDE_MIST_FIGURE_DECOMPOSITION__
#define __INCLUDE_MIST_FIGURE_DECOMPOSITION__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
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
		typedef ptrdiff_t difference_type;
		difference_type	x;
		difference_type	y;
		difference_type	z;

		Position( difference_type xx = 0, difference_type yy = 0, difference_type zz = 0 ) : x( xx ), y( yy ), z( zz )
		{
		}
	};

	template < class Array >
	typename Array::value_type search_max( const Array &in, std::deque< Position > &list )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;

		list.clear( );

		if( in.empty( ) )
		{
			return( value_type( 0 ) );
		}

		const value_type infinity = type_limits< value_type >::maximum( );
		value_type max = in[ 0 ];
		// 現在の最大値を探索する
		for( size_type i = 1 ; i < in.size( ) ; i++ )
		{
			if( in[ i ] != infinity )
			{
				max = max > in[ i ] ? max : in[ i ];
			}
		}

		if( max == 0 )
		{
			return( value_type( 0 ) );
		}

		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( in( i, j, k ) == max )
					{
						list.push_back( Position( i, j, k ) );
					}
				}
			}
		}
		return( max );
	}

	template < class Array >
	typename Array::size_type figure_decomposition( const Array &in, Array &out, typename Array::value_type label_max, typename Array::size_type max_iterations = -1 )
	{
		typedef typename Array::size_type		size_type;
		typedef typename Array::difference_type	difference_type;
		typedef typename Array::value_type		value_type;

		const typename array3< size_type >::value_type infinity = type_limits< typename array3< size_type >::value_type >::maximum( );

		array3< size_type > dist( in.width( ), in.height( ), in.depth( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			dist[ i ] = in[ i ];
		}

		calvin::distance_transform( dist, dist );

		typedef Position position_type;
		std::deque< position_type > list;

		size_type label_count = 0;
		size_type loop_count = 0;
		value_type current_label = 0;
		position_type pt;

		array3< difference_type > mask( in.width( ), in.height( ), in.depth( ) );
		out.fill( );

		printf( "looping start\n" );
		while( loop_count++ < max_iterations )
		{
			difference_type rr = search_max( dist, list );

			if( list.empty( ) )
			{
				// 全ての点を捜査し終わったので終了する
				break;
			}

			difference_type rx = static_cast< difference_type >( std::ceil( std::sqrt( static_cast< double >( rr ) ) ) );
			difference_type ry = rx;
			difference_type rz = in.depth( ) < 2 ? 1 : rx;

			printf( "                                                                   \r" );
			printf( "looping ... % 4d, label =% 4d, size =% 3d, radius =%.3f )\r", ++loop_count, label_count, list.size( ), sqrt( (double)rr ) );

			while( !list.empty( ) )
			{
				std::deque< position_type >::iterator ite = list.begin( );
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

				if( out( pt.x, pt.y ) == 0 )
				{
					// 他の領域から塗られていないので，新しいラベルとする
					label_count++;
					if( label_count > label_max )
					{
						label_count = label_max + 1;
					}
					current_label = static_cast< value_type >( label_count );
				}
				else
				{
					// 既にラベルが割り当てられているので，そのラベルで塗りつぶす
					current_label = out( pt.x, pt.y );
				}

				for( difference_type k = -rz ; k <= rz ; k++ )
				{
					size_type kk = k * k;
					size_type pz = k + pt.z;
					if( pz < 0 || pz >= in.depth( ) ) continue;

					for( difference_type j = -ry ; j <= ry ; j++ )
					{
						size_type jj = j * j;
						size_type py = j + pt.y;
						if( py < 0 || py >= in.height( ) ) continue;

						for( difference_type i = -rx ; i <= rx ; i++ )
						{
							size_type ii = i * i;
							size_type px = i + pt.x;
							if( px < 0 || px >= in.width( ) ) continue;

							difference_type rrr = ii + jj + kk;
							if( rrr < rr )
							{
								if( dist( px, py, pz ) + rrr <= rr )
								{
									if( mask( px, py, pz ) < rr - rrr )
									{
										mask( px, py, pz ) = rr - rrr;
										out( px, py, pz ) = current_label;
									}
								}
							}
						}
					}
				}

				// 次回の最大値検索から外れるようにする
				dist( pt.x, pt.y, pt.z ) = infinity;
			}
		}

		// MAXラベル数を超えたものを除去
		for( size_type j = 0 ; j < out.size( ) ; j++ )
		{
			out[ j ] = out[ j ] > label_max ? 0 : out[ j ];
		}

		return( label_count );
	}

}




// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FIGURE_DECOMPOSITION__
