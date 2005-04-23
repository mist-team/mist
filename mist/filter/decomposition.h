/// @file mist/filter/decomposition.h
//!
//! @brief ユークリッド距離に基づく図形分割
//!
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


#include <vector>
#include <set>
#include <algorithm>


// mist名前空間の始まり
_MIST_BEGIN


// 図形分割用のツール
namespace __figure_dedomposition__
{
	struct Position
	{
		typedef ptrdiff_t difference_type;
		difference_type	x;
		difference_type	y;
		difference_type	z;
		bool has_label;

		Position( difference_type xx = 0, difference_type yy = 0, difference_type zz = 0, bool l = false ) : x( xx ), y( yy ), z( zz ), has_label( l )
		{
		}

		bool operator <( const Position &p ) const
		{
			return( has_label > p.has_label );
		}
	};

	template < class Array, class Label >
	typename Array::value_type search_max( const Array &in, const Label &label, std::vector< Position > &list, typename Array::value_type max )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;

		list.clear( );

		size_type pos = 0;
		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( in[ pos ] == max )
					{
						list.push_back( Position( i, j, k, label[ pos ] != 0 ) );
					}
					pos++;
				}
			}
		}

		std::sort( list.begin( ), list.end( ) );

		return( max );
	}
}



/// @brief ユークリッド距離に基づく図形分割
//!
//! 図形をくびれ部分で分割する
//!
//! @param[in] in  … 入力データ
//! @param[in] out … 出力画像
//!
//! @return 分割された領域の数
//!
template < class Array1, class Array2 >
typename Array1::size_type figure_decomposition( const Array1 &in, Array2 &out )
{
	if( in.empty( ) )
	{
		return( 0 );
	}

	typedef typename Array1::size_type			size_type;
	typedef typename Array1::difference_type	difference_type;
	typedef typename Array2::value_type			value_type;

	const typename array3< size_type >::value_type infinity = type_limits< typename array3< size_type >::value_type >::maximum( );

	array3< difference_type > dist( in.width( ), in.height( ), in.depth( ) );

	// 距離変換用の画像を作成
	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		dist[ i ] = in[ i ] != 0 ? 1 : 0;
	}

	calvin::distance_transform( dist, dist );

	typedef __figure_dedomposition__::Position position_type;
	std::vector< position_type > list;

	// 画像中に含まれる距離値のリストを作成する
	std::set< size_type > distance_list;
	for( size_type l = 0 ; l < dist.size( ) ; l++ )
	{
		distance_list.insert( dist[ l ] );
	}

	size_type  label_count = 0;		// 現在のラベル数
	size_type  loop_count = 0;		// 現在のラベル数
	value_type current_label = 0;	// 現在処理中のラベル
	value_type label_max = type_limits< value_type >::maximum( );	// ラベルの最大値
	position_type pt;

	array3< double > mask( in.width( ), in.height( ), in.depth( ) );
	out.fill( );

#ifdef DEBUG
	size_type count[ 2 ] = { 0, 0 };
#endif
	std::set< size_type >::reverse_iterator dite = distance_list.rbegin( );
	for( ; dite != distance_list.rend( ) ; ++dite )
	{
		difference_type rr = search_max( dist, out, list, *dite );

		double r = std::sqrt( static_cast< double >( rr ) );
		difference_type rx = static_cast< difference_type >( std::ceil( r ) );
		difference_type ry = in.height( ) < 2 ? 0 : rx;
		difference_type rz = in.depth( ) < 2 ? 0 : rx;
		difference_type RR = ( rx + 1 ) * ( rx + 1 );

#ifdef DEBUG
		std::cerr << "                                                                   \r";
		std::cerr << "looping ... " << ++loop_count << ", label = " << label_count << ", size = " << list.size( ) << ", radius = " << r << "\r";
#endif

		std::vector< position_type >::iterator ite = list.begin( );
		for( ; ite != list.end( ) ; ++ite )
		{
			position_type &pt = *ite;


#ifdef DEBUG
			count[ 0 ]++;
			if( r <= mask( pt.x, pt.y, pt.z ) )
			{
				count[ 1 ]++;
				continue;
			}
#else
			if( r <= mask( pt.x, pt.y, pt.z ) )
			{
				continue;
			}
#endif

			if( out( pt.x, pt.y, pt.z ) == 0 )
			{
				// 他の領域から塗られていないので，新しいラベルとする
				label_count++;
				if( label_count > label_max )
				{
					// 最大のラベル数を超えた場合には，最大ラベルとする
					label_count = label_max + 1; 
				}
				current_label = static_cast< value_type >( label_count );
			}
			else
			{
				// 既にラベルが割り当てられているので，そのラベルで塗りつぶす
				current_label = out( pt.x, pt.y, pt.z );
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
							if( dist( px, py, pz ) + rrr < RR )
							{
								double l = r - std::sqrt( static_cast< double >( rrr ) );
								if( mask( px, py, pz ) < l )
								{
									mask( px, py, pz ) = l;
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

#ifdef DEBUG
	std::cout << std::endl << "Skip Ratio: " << count[ 1 ] / ( double )count[ 0 ] << std::endl;
#endif

	return( label_count );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_FIGURE_DECOMPOSITION__
