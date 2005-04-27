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
		difference_type	RADIUS;
		double			radius;
		difference_type	index;

		Position( difference_type r = 0, double R = 0, difference_type indx = 0 ) : RADIUS( r ), radius( R - std::sqrt( static_cast< double >( r ) ) ), index( indx )
		{
		}

		bool operator <( const Position &p ) const
		{
			return( RADIUS > p.RADIUS );
		}
	};

	// �摜�̒[�ɉ�f�������Ă��邩�ǂ����𒲂ׂ�
	template < class T, class Allocator >
	bool has_voxel_at_side( const array2< T, Allocator > &in )
	{
		typedef typename array2< T, Allocator >::size_type size_type;

		for( size_type i = 0 ; i < in.width( ) ; i++ )
		{
			if( in( i, 0 ) != 0 || in( i, in.height( ) - 1 ) != 0 )
			{
				return( true );
			}
		}

		for( size_type j = 0 ; j < in.height( ) ; j++ )
		{
			if( in( 0, j ) != 0 || in( in.width( ) - 1, j ) != 0 )
			{
				return( true );
			}
		}

		return( false );
	}

	// �摜�̒[�ɉ�f�������Ă��邩�ǂ����𒲂ׂ�
	template < class T, class Allocator >
	bool has_voxel_at_side( const array3< T, Allocator > &in )
	{
		typedef typename array2< T, Allocator >::size_type size_type;

		if( in.depth( ) == 1 )
		{
			for( size_type i = 0 ; i < in.width( ) ; i++ )
			{
				if( in( i, 0, 0 ) != 0 || in( i, in.height( ) - 1, 0 ) != 0 )
				{
					return( true );
				}
			}

			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				if( in( 0, j, 0 ) != 0 || in( in.width( ) - 1, j, 0 ) != 0 )
				{
					return( true );
				}
			}
		}
		else
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( in( i, j, 0 ) != 0 || in( i, j, in.depth( ) - 1 ) != 0 )
					{
						return( true );
					}
				}
			}

			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( in( i, 0, k ) != 0 || in( i, in.height( ) - 1, k ) != 0 )
					{
						return( true );
					}
				}
			}

			for( size_type k = 0 ; k < in.depth( ) ; k++ )
			{
				for( size_type j = 0 ; j < in.height( ) ; j++ )
				{
					if( in( 0, j, k ) != 0 || in( in.width( ) - 1, j, k ) != 0 )
					{
						return( true );
					}
				}
			}
		}

		return( false );
	}

	struct __mist_dmy_fd_callback__
	{
		/// @brief �}�`�����̐i�s�󋵂�Ԃ�
		//!
		//! @param[in] loop      �c ���݂̌J��Ԃ���
		//! @param[in] label_num �c ���݂̍ő僉�x���ԍ�
		//! @param[in] radius    �c ���ݏ������̔��a
		//! @param[in] in        �c ���͉摜
		//! @param[in] out       �c �o�̓��x���摜
		//!
		template < class Array >
		void operator()( size_t loop, size_t label_num, double radius, const Array &in, const Array &out ) const
		{
		}
	};
}



/// @brief ���[�N���b�h�����Ɋ�Â��}�`����
//!
//! �}�`�����тꕔ���ŕ�������
//!
//! @param[in] in  �c ���̓f�[�^
//! @param[in] out �c �o�͉摜
//! @param[in] f   �c �i�s�󋵂�^����R�[���o�b�N
//!
//! @return �������ꂽ�̈�̐�
//!
template < class Array1, class Array2, class Functor >
typename Array1::size_type figure_decomposition( const Array1 &in, Array2 &out, Functor f )
{
	if( in.empty( ) )
	{
		return( 0 );
	}

	typedef typename Array1::size_type									size_type;
	typedef typename Array1::difference_type							difference_type;
	typedef typename Array1::const_pointer								const_pointer;
	typedef typename Array2::value_type									value_type;
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

	if( __figure_dedomposition__::has_voxel_at_side( in ) )
	{
		// �摜�̒[�ɂP��f������ꍇ
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
	else
	{
		calvin::distance_transform( dist, dist );
	}


	typedef __figure_dedomposition__::Position position_type;

	// �摜���Ɋ܂܂�鋗���l�̃��X�g���쐬����
	std::set< difference_type > distance_list;
	for( size_type l = 0 ; l < dist.size( ) ; l++ )
	{
		distance_list.insert( dist[ l ] );
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

	typename std::set< difference_type >::reverse_iterator dite = distance_list.rbegin( );
	for( ; dite != distance_list.rend( ) ; ++dite )
	{
		difference_type rr = *dite;

		if( rr == 0 )
		{
			// �O�̉�f�̓A�N�Z�X���Ȃ�
			break;
		}

		double r = std::sqrt( static_cast< double >( rr ) );

		std::deque< difference_type > list;
		for( size_type i = 0 ; i < dist.size( ) ; i++ )
		{
			// ���i�K�ōő�̋����l������f��T��
			if( dist[ i ] == rr && r > mask[ i ] )
			{
				list.push_back( i );
			}
		}

		difference_type rx = static_cast< difference_type >( std::ceil( r ) );
		difference_type ry = in.height( ) < 2 ? 0 : rx;
		difference_type rz = in.depth( ) < 2 ? 0 : rx;
		difference_type RR = ( rx + 1 ) * ( rx + 1 );

		f( ++loop_count, label_count, r, in, out );

		if( rr > 1 )
		{
			std::vector< position_type > sphere;
			sphere.reserve( ( 2 * rx + 1 ) * ( 2 * ry + 1 ) * ( 2 * rz + 1 ) );
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
								sphere.push_back( position_type( rrr, r, &out( cx + i, cy + j, cz + k ) - cp ) );
							}
						}
					}
				}
			}

			while( !list.empty( ) )
			{
				typename std::deque< difference_type >::iterator ite = list.begin( );
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
					const difference_type indx = index + pt.index;
					if( mask[ indx ] < pt.radius )
					{
						if( dist[ indx ] + pt.RADIUS < RR )
						{
							mask[ indx ] = pt.radius;
							out[ indx ] = static_cast< value_type >( current_label );
						}
					}
				}
			}
		}
		else
		{
			// �����l�P�̉�f��������ȏ������s��
			std::vector< difference_type > sphere;
			sphere.reserve( ( 2 * rx + 1 ) * ( 2 * ry + 1 ) * ( 2 * rz + 1 ) );
			{
				difference_type cx = out.width( ) / 2;
				difference_type cy = out.height( ) / 2;
				difference_type cz = out.depth( ) / 2;
				const_pointer cp = &out( cx, cy, cz );
				for( difference_type k = -rz ; k <= rz ; k++ )
				{
					for( difference_type j = -ry ; j <= ry ; j++ )
					{
						for( difference_type i = -rx ; i <= rx ; i++ )
						{
							// �C���f�b�N�X�Ƃ��āC�W�E�Q�U�ߖT���̉�f��������
							sphere.push_back( &out( cx + i, cy + j, cz + k ) - cp );
						}
					}
				}
			}

			while( !list.empty( ) )
			{
				typename std::deque< difference_type >::iterator ite = list.begin( );
				difference_type index = 0;

				current_label = 0;
				for( ; ite != list.end( ) ; ++ite )
				{
					difference_type index = *ite;
					const_pointer p = &out[ index ];

					// �W�E�Q�U�ߖT������ɓh���Ă�����̂�T���Ă���
					for( size_type i = 0 ; i < sphere.size( ) ; i++ )
					{
						size_type cl = static_cast< size_type >( p[ sphere[ i ] ] );
						if( cl != 0 )
						{
							current_label = cl;
							break;
						}
					}
					if( current_label != 0 )
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

				if( current_label == 0 )
				{
					// ���̗̈悩��h���Ă��Ȃ��̂ŁC�V�������x���Ƃ���
					label_count++;
					if( label_count > label_max )
					{
						// �ő�̃��x�����𒴂����ꍇ�ɂ́C�ő僉�x���Ƃ���
						label_count = label_max + 1; 
					}
					out[ index ] = static_cast< value_type >( label_count );
				}
				else
				{
					// ���Ƀ��x�������蓖�Ă��Ă���̂ŁC���̃��x���œh��Ԃ�
					out[ index ] = static_cast< value_type >( current_label );
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
	figure_decomposition( in, out, __figure_dedomposition__::__mist_dmy_fd_callback__( ) );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_FIGURE_DECOMPOSITION__
