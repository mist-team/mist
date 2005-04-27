/// @file mist/filter/decomposition.h
//!
//! @brief ���[�N���b�h�����Ɋ�Â��}�`����
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


#include <deque>
#include <vector>
#include <set>
#include <algorithm>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// �}�`�����p�̃c�[��
namespace __figure_dedomposition__
{
	struct Position
	{
		typedef ptrdiff_t difference_type;
		difference_type	index;
		difference_type	radius;

		Position( difference_type r = 0, difference_type indx = 0 ) : index( indx ), radius( r )
		{
		}

		bool operator <( const Position &p ) const
		{
			return( radius > p.radius );
		}
	};
}



/// @brief ���[�N���b�h�����Ɋ�Â��}�`����
//!
//! �}�`�����тꕔ���ŕ�������
//!
//! @param[in] in  �c ���̓f�[�^
//! @param[in] out �c �o�͉摜
//!
//! @return �������ꂽ�̈�̐�
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
	typedef typename Array1::const_pointer		const_pointer;
	typedef typename Array2::value_type			value_type;
	typedef typename Array2::template rebind< difference_type >::other	distance_type;
	typedef typename Array2::template rebind< double >::other			mask_type;

	distance_type dist;
	dist.resize( in.width( ), in.height( ), in.depth( ) );
	dist.reso1( 1 );
	dist.reso2( 1 );
	dist.reso3( 1 );

	// �����ϊ��p�̉摜���쐬
	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		dist[ i ] = in[ i ] != 0 ? 1 : 0;
	}

	{
		marray< distance_type > dist_tmp( dist, 1 );
		distance_type &dist_tmp_map = dist_tmp;

		// �摜�̊O���ɂ͂O��f�����݂�����̂Ƃ��ċ����ϊ����s��
		calvin::distance_transform( dist_tmp_map, dist_tmp_map );

		for( size_type k = 0 ; k < dist.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < dist.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < dist.width( ) ; i++ )
				{
					dist( i, j, k ) = dist_tmp( i, j, k );
				}
			}
		}
	}

	typedef __figure_dedomposition__::Position position_type;

	// �摜���Ɋ܂܂�鋗���l�̃��X�g���쐬����
	std::multiset< position_type > distance_list;
	for( size_type l = 0 ; l < dist.size( ) ; l++ )
	{
		difference_type d = dist[ l ];
		if( d > 1 )
		{
			distance_list.insert( position_type( dist[ l ], l ) );
		}
	}

	size_type label_count = 0;		// ���݂̃��x����
	size_type loop_count = 0;		// ���݂̃��x����
	size_type current_label = 0;	// ���ݏ������̃��x��
	size_type label_max = type_limits< value_type >::maximum( );	// ���x���̍ő�l
	position_type pt;

	mask_type mask( in );
	mask.fill( );
	out.resize( in.width( ), in.height( ), in.depth( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	out.fill( );

	typedef std::multiset< position_type >::iterator distance_iterator_type;
	distance_iterator_type dite = distance_list.begin( );
	for( ; dite != distance_list.end( ) ; )
	{
		difference_type rr = dite->radius;
		double r = std::sqrt( static_cast< double >( rr ) );

		std::deque< difference_type > list;

		std::pair< distance_iterator_type, distance_iterator_type > range = distance_list.equal_range( position_type( rr, 0 ) );
		std::multiset< position_type >::iterator rite = range.first;
		for( ; rite != range.second ; ++rite )
		{
			list.push_back( rite->index );
		}
		dite = rite;

		difference_type rx = static_cast< difference_type >( std::ceil( r ) );
		difference_type ry = in.height( ) < 2 ? 0 : rx;
		difference_type rz = in.depth( ) < 2 ? 0 : rx;
		difference_type RR = ( rx + 1 ) * ( rx + 1 );

#ifdef WIN32//_DEBUG
		std::cerr << "                                                                   \r";
		std::cerr << "looping ... " << ++loop_count << ", label = " << label_count << ", size = " << list.size( ) << ", radius = " << r << "\r";
#endif

		std::vector< position_type > sphere;
		{
			difference_type cx = out.width( ) / 2;
			difference_type cy = out.height( ) / 2;
			difference_type cz = out.depth( ) / 2;
			const_pointer cp = &out( cx, cy, cz );
			for( difference_type k = -rz ; k <= rz ; k++ )
			{
				size_type kk = k * k;

				for( difference_type j = -ry ; j <= ry ; j++ )
				{
					size_type jj = j * j;

					for( difference_type i = -rx ; i <= rx ; i++ )
					{
						size_type ii = i * i;

						difference_type rrr = ii + jj + kk;
						if( rrr < rr )
						{
							// �C���f�b�N�X�Ƃ��āC���̒��S����̍���������
							sphere.push_back( position_type( rrr, &out( cx + i, cy + j, cz + k ) - cp ) );
						}
					}
				}
			}
		}

		while( !list.empty( ) )
		{
			std::deque< difference_type >::iterator ite = list.begin( );
			difference_type index = 0;
			for( ; ite != list.end( ) ; ++ite )
			{
				difference_type index = *ite;
				if( out[ index ] != 0 )
				{
					break;
				}
			}
			if( ite == list.end( ) )
			{
				// ���łɃ��x��������U���Ă�����̂�������Ȃ������ꍇ
				index = list.front( );
				list.pop_front( );
			}
			else
			{
				index = *ite;
				list.erase( ite );
			}

			if( r <= mask[ index ] )
			{
				continue;
			}

			if( out[ index ] == 0 )
			{
				// ���̗̈悩��h���Ă��Ȃ��̂ŁC�V�������x���Ƃ���
				label_count++;
				if( label_count > label_max )
				{
					// �ő�̃��x�����𒴂����ꍇ�ɂ́C�ő僉�x���Ƃ���
					label_count = label_max + 1; 
				}
				current_label = static_cast< value_type >( label_count );
			}
			else
			{
				// ���Ƀ��x�������蓖�Ă��Ă���̂ŁC���̃��x���œh��Ԃ�
				current_label = out[ index ];
			}

			for( size_type i = 0 ; i < sphere.size( ) ; i++ )
			{
				const position_type &pt = sphere[ i ];
				difference_type indx = index + pt.index;
				difference_type rrr = pt.radius;
				if( dist[ indx ] + rrr < RR )
				{
					double l = r - std::sqrt( static_cast< double >( rrr ) );
					if( mask[ indx ] < l )
					{
						mask[ indx ] = l;
						out[ indx ] = static_cast< value_type >( current_label );
					}
				}
			}
		}
	}

	// MAX���x�����𒴂������̂�����
	for( size_type j = 0 ; j < out.size( ) ; j++ )
	{
		out[ j ] = static_cast< size_type >( out[ j ] ) > label_max ? 0 : out[ j ];
	}

	return( label_count );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_FIGURE_DECOMPOSITION__
