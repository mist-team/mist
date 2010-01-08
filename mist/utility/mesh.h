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

/// @file mist/utility/mesh.h
//!
//! @brief �L�����u���[�V�����`���[�g����_��؂�o��
//!
#ifndef __INCLUDE_MIST_MESH__
#define __INCLUDE_MIST_MESH__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "../matrix.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "../vector.h"
#endif

#ifndef __INCLUDE_CONVERTER__
#include "../converter.h"
#endif

#ifndef __INCLUDE_MIST_LABELING__
#include "../filter/labeling.h"
#endif

#ifndef __INCLUDE_MIST_DRAWING__
#include "../drawing.h"
#endif


#include <deque>
#include <set>
#include <algorithm>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup mesh_group ���b�V�����o
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/utility/mesh.h>
//! @endcode
//!
//!  @{

namespace __mesh_utility__
{
	struct __mesh_information__
	{
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef vector2< double > vector_type;

		difference_type label;
		difference_type count;
		double          round;
		vector_type     pos;
		bool            enabled;
		difference_type mark;
		difference_type row;
		difference_type col;
		double          length;

		__mesh_information__( ) : label( -1 ), count( 0 ), round( 0.0 ), pos( 0.0, 0.0 ), enabled( true ), mark( -1 ), row( -1 ), col( -1 ), length( 1.0e10 )
		{
		}

		// ���͒��̑傫���ŕ��בւ���
		bool operator <( const __mesh_information__ &m ) const
		{
			return( this->round > m.round );
		}

		static bool sort_by_round( const __mesh_information__ &m1, const __mesh_information__ &m2 )
		{
			return( m1.round > m2.round );
		}

		static bool sort_by_length( const __mesh_information__ &m1, const __mesh_information__ &m2 )
		{
			return( m1.length < m2.length );
		}
	};

	inline double __minimum__( double v1, double v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}

	inline double __compute_border_distance__( const vector2< double > &pos, double image_width, double image_height )
	{
		double xmin = __minimum__( std::abs( pos.x ), std::abs( image_width - pos.x ) );
		double ymin = __minimum__( std::abs( pos.y ), std::abs( image_height - pos.y ) );
		return( __minimum__( xmin, ymin ) );
	}
}

/// @brief �~���i�q��ɕ��ׂ��摜����C�e�_�𒊏o�����b�V����̓_�ƑΉ��t����
//! 
//! @attention ���͉摜�͂��炩���߁C�������l�������s���Ă����K�v������D
//! 
//! �O���b�h�̓_�͍�������_�Ƃ��C���b�V���̊���C�����߂�_���z�u����D
//! �_��̃T���v���́Cmist/utility/mesh.ai ���Q�l�ɍ쐬���Ă��������D
//! 
//! �o�͂���郁�b�V���̍��W�̂����C�Ή�����_��������Ȃ����̂ɂ� (-1,-1) ���o�͂���D
//! 
//! @param[in]     chart �c ���͉摜�i���b�V�����f���Ă���摜�j
//! @param[in,out] grid  �c �B�e�����摜�Ɋ܂܂�郁�b�V���̓_���ŏ��������ꂽ�z��i�摜�Ɉڂ��Ă��Ȃ��_���܂߂������j����͂Ƃ��C�摜���̍��W�����ʂƂ��ďo�͂����
//! @param[in]     row   �c ���b�V���̊���C�����߂�_��̒��S��\���s�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     col   �c ���b�V���̊���C�����߂�_��̒��S��\��������̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     threshold_for_circular_ratio �c ���b�V���̌����i�荞�ނ��߂̉~�`�x�̂������l�i�����菬�����l�̂��̂͏��O�j
//! 
//! @return ���b�V���̒��o�ɐ����������ǂ���
//! 
template < class T, class Allocator >
bool extract_mesh( array2< T, Allocator > &chart, matrix< vector2< double > > &grid, typename array2< T, Allocator >::difference_type row, typename array2< T, Allocator >::difference_type col, double threshold_for_circular_ratio = 0.4 )
{
	typedef __mesh_utility__::__mesh_information__ __mesh_information__;
	typedef array2< T, Allocator >::size_type size_type;
	typedef array2< T, Allocator >::difference_type difference_type;
	typedef vector2< double > vector_type;
	typedef std::deque< __mesh_information__ > mesh_list_type;
	typedef typename mesh_list_type::iterator mesh_iterator;
	typedef typename mesh_list_type::reverse_iterator mesh_reverse_iterator;
	array2< size_type > binary;


	// �Q�l�摜�ɕϊ�
	convert( chart, binary );

	size_type i, j;
	difference_type r, c;
	difference_type rows = grid.rows( );
	difference_type cols = grid.cols( );

	size_type labelnum = labeling4( binary, binary );
	//std::cout << "LabelNum: " << labelnum << std::endl;

	mesh_list_type mesh_list;
	array< __mesh_information__ > meshes( labelnum + 1 );

	for( i = 0 ; i <= labelnum ; i++ )
	{
		meshes[ i ].label = i;
	}

	// �S�ẴO���b�h������������
	for( size_type i = 0 ; i < grid.size( ) ; i++ )
	{
		grid[ i ].x = -1;
		grid[ i ].y = -1;
	}

	// �w�i�͖����ɂ���
	meshes[ 0 ].enabled = false;

	// �e���x���̉�f���C�d�S�ʒu�C���͒������߂�
	const double _2 = std::sqrt( 2.0 );
	for( j = 0 ; j < chart.height( ) - 1 ; j++ )
	{
		for( i = 0 ; i < chart.width( ) - 1 ; i++ )
		{
			__mesh_information__ &mesh = meshes[ binary( i, j ) ];
			 mesh.count++;
			 mesh.pos.x += i;
			 mesh.pos.y += j;

			// �摜�̎��͒����v�Z����
			__mesh_information__ &m0 = mesh;
			__mesh_information__ &m1 = meshes[ binary( i + 1, j     ) ];
			__mesh_information__ &m2 = meshes[ binary( i    , j + 1 ) ];
			__mesh_information__ &m3 = meshes[ binary( i + 1, j + 1 ) ];
			size_type l0 = m0.label;
			size_type l1 = m1.label;
			size_type l2 = m2.label;
			size_type l3 = m3.label;

			if( l0 > 0 )
			{
				size_type num_eq = 0;
				if( l1 == l0 ) num_eq++;
				if( l2 == l0 ) num_eq++;
				if( l3 == l0 ) num_eq++;

				if( num_eq == 2 )
				{
					m0.round += _2;
				}
				else if( num_eq == 1 && l0 != l3 )
				{
					m0.round += 1.0;
				}
			}
			else if( l1 > 0 )
			{
				if( l1 == l3 )
				{
					if( l1 == l2 )
					{
						m1.round += _2;
					}
					else
					{
						m1.round += 1.0;
					}
				}
			}
			else if( l2 > 0 )
			{
				if( l2 == l3 )
				{
					m2.round += 1.0;
				}
			}
		}
	}


	// �e�̈�̏d�S�ʒu���v�Z����
	for( i = 1 ; i <= labelnum ; i++ )
	{
		__mesh_information__ &mesh = meshes[ i ];
		mesh.pos.x /= mesh.count;
		mesh.pos.y /= mesh.count;
	}

	// ��ʂ̋��E�ɐڂ��Ă���̈����菜��
	for( i = 0 ; i < chart.width( ) ; i++ )
	{
		meshes[ binary( i, 0 ) ].enabled = false;
		meshes[ binary( i, chart.height( ) - 1 ) ].enabled = false;
	}

	// ��ʂ̋��E�ɐڂ��Ă���̈����菜��
	for( j = 0 ; j < chart.height( ) ; j++ )
	{
		meshes[ binary( 0, j ) ].enabled = false;
		meshes[ binary( chart.width( ) - 1, j ) ].enabled = false;
	}

	// �~�`�x��]�����āC�s�v�ȗ̈����������
	difference_type minimum_count = 15; // ���̌��ɖ����Ȃ��̈�͍폜����
	for( i = 1 ; i <= labelnum ; i++ )
	{
		__mesh_information__ &mesh = meshes[ i ];
		if( mesh.count < minimum_count )
		{
			mesh.enabled = false;
		}
		else if( mesh.enabled )
		{
			const double pai = 3.1415926535897932384626433832795;
			const double S = mesh.count;
			const double L = mesh.round;
			double e = 4.0 * pai * S / ( L * L );
			if( e < threshold_for_circular_ratio )
			{
				mesh.enabled = false;
			}
			else
			{
				mesh_list.push_back( mesh );
			}
			//std::cout << "���x��: " << i << ", �~�`�x: " << e << std::endl;
		}
	}

	if( mesh_list.size( ) < 10 )
	{
		return( false );
	}

	// ���͒��̑傫������Ƃ��ĕ��בւ���
	std::sort( mesh_list.begin( ), mesh_list.end( ), __mesh_information__::sort_by_round );

	// ���͒��̑傫������Ƃ��āC��_��𒊏o����
	{
		mesh_iterator ite = mesh_list.begin( );
		difference_type base_mesh_num = 1;
		double oround = ite->round;
		double maxround = ite->round;
	
		ite->mark = 1;
		++ite;

		for( ; ite != mesh_list.end( ) ; ++ite )
		{
			double round = ite->round;

			//std::cout << "���͒�: " << round << ", ��: " << round / oround << std::endl;

			// ��O�̎��͒���90���ȏ�ŁC�ő�̉~�̎��͒���80%�ȏ�̒���������ꍇ�C��_�Ƃ��ă}�[�N����
			if( round > oround * 0.9 && round > maxround * 0.5 )
			{
				ite->mark = 1;
				base_mesh_num++;
				oround = round;
			}
			else
			{
				break;
			}
		}
		for( ; ite != mesh_list.end( ) ; ++ite )
		{
			ite->mark = 100;
		}

		/// ��ƂȂ钆�S�_��������
		// ��_�̐������Ȃ��ꍇ�ɂ͏I��
		if( base_mesh_num < 2 )
		{
			for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
			{
				meshes[ ite->label ].mark = ite->mark;
			}

			for( i = 0 ; i < chart.size( ) ; i++ )
			{
				if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
				{
					chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
				}
				else
				{
					chart[ i ] = 255;
				}
			}

			return( false );
		}

		// �܂��C�傫����_�̍��W��p���Ċ�_�����Ԓ��������߂�
		double x1 = mesh_list[ 0 ].pos.x;
		double y1 = mesh_list[ 0 ].pos.y;
		double x2 = mesh_list[ 1 ].pos.x;
		double y2 = mesh_list[ 1 ].pos.y;
		double A = y2 - y1;
		double B = x1 - x2;
		double C = x2 * y1 - x1 * y2;

		mesh_iterator site = mesh_list.begin( );
		mesh_iterator eite = site + base_mesh_num;

		// ����~�X�̊�_��r������
		{
			double __threshold__ = std::sqrt( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) ) * 0.2;
			double D = 1.0 / std::sqrt( A * A + B * B );
			for( ite = site ; ite != eite ; ++ite )
			{
				if( std::abs( A * ite->pos.x + B * ite->pos.y + C ) * D > __threshold__ )
				{
					ite->mark = 100;
					base_mesh_num--;
				}
			}
		}

		// ��_�����Ԃɕ��בւ���]���l���v�Z����
		for( ite = site ; ite != eite ; ++ite )
		{
			if( ite->mark == 1 )
			{
				ite->length = - B * ite->pos.x + A * ite->pos.y;
			}
			else
			{
				ite->length = 1.0e10;
			}
		}

		// ����ɉ����ĕ��ёւ���
		std::sort( site, eite, __mesh_information__::sort_by_length );
		site = mesh_list.begin( );
		eite = site + base_mesh_num;

		// �傫�����ω�����ʒu���ēx���o����
		{
			// �܂��C�����Ƃ����͒��̒������̂�������
			mesh_iterator mite = mesh_list.begin( );
			mesh_iterator oite = mesh_list.begin( );
			mesh_iterator cite = oite;
			for( ; cite != eite ; ++cite )
			{
				if( mite->round < cite->round )
				{
					mite = cite;
				}
			}

			for( cite = mite, oite = mite ; cite != eite ; ++cite )
			{
				double r1 = oite->round;
				double r2 = cite->round;

				// ��O�̎��͒���80���ȏ�ł���΁C��_�Ƃ��ă}�[�N����
				//std::cout << "���͒�: " << r1 << ", ��: " << r2 / r1 << std::endl;
				if( ( r1 > r2 && r2 < r1 * 0.8 ) || ( r1 < r2 && r1 < r2 * 0.8 ) )
				{
					break;
				}

				oite = cite;
			}

			for( ; cite != eite ; ++cite )
			{
				cite->mark = 100;
				cite->length = 1.0e10;
			}

			mesh_reverse_iterator rmite( mite + 1 );
			mesh_reverse_iterator reite = mesh_list.rend( );
			mesh_reverse_iterator rcite( rmite );
			for( mesh_reverse_iterator roite = rmite ; rcite != reite ; ++rcite )
			{
				double r1 = roite->round;
				double r2 = rcite->round;

				// ��O�̎��͒���80���ȏ�ł���΁C��_�Ƃ��ă}�[�N����
				//std::cout << "���͒�: " << r1 << ", ��: " << r2 / r1 << std::endl;
				if( ( r1 > r2 && r2 < r1 * 0.8 ) || ( r1 < r2 && r1 < r2 * 0.8 ) )
				{
					break;
				}

				roite = rcite;
			}

			for( ; rcite != reite ; ++rcite )
			{
				rcite->mark = 100;
				rcite->length = 1.0e10;
			}

			for( cite = site, base_mesh_num = 0 ; cite != eite ; ++cite )
			{
				if( cite->mark == 1 )
				{
					base_mesh_num++;
				}
			}

			// ����ɉ����ĕ��ёւ���
			std::sort( site, eite, __mesh_information__::sort_by_length );
			site = mesh_list.begin( );
			eite = site + base_mesh_num;

			// ��_�����o���ꂷ�����Ƃ��͏I������
			if( col + 1 < base_mesh_num )
			{
				for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
				{
					meshes[ ite->label ].mark = ite->mark;
				}

				for( i = 0 ; i < chart.size( ) ; i++ )
				{
					if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
					{
						chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
					}
					else
					{
						chart[ i ] = 255;
					}
				}

				return( false );
			}
		}

		// ���S������킷��_�����肷��
		// ����C���̍��E�̓_�̂ǂ��炩����ɂ���
		{
			mesh_iterator ite1 = mesh_list.begin( );
			mesh_iterator ite2 = ite1 + base_mesh_num - 1;
			double lth = 2.0 * ( ite1->pos - ite2->pos ).length( ) / static_cast< double >( base_mesh_num - 1 );

			double ratio1 = 1.0e10;
			double ratio2 = 1.0e10;

			double len0 = __mesh_utility__::__compute_border_distance__( mesh_list[ 0 ].pos, chart.width( ), chart.height( ) );
			double len1 = __mesh_utility__::__compute_border_distance__( mesh_list[ base_mesh_num - 1 ].pos, chart.width( ), chart.height( ) );

			// �[�����苗���ȏ㗣��Ă���ꍇ�ɔ�����s��
			if( len0 > lth )
			{
				vector_type p1  = ite1->pos;
				vector_type p2  = ( ite1 + 1 )->pos;
				double length   = ( p1 - p2 ).length( );
				vector_type dir = ( p1 - p2 ).unit( );
				vector_type p   = p1 + dir * length * 0.8;

				double min = 1.0e10;
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min && cite != ite1 && l < length )
					{
						min = l;
						ratio1 = cite->round / ite1->round;
					}
				}
			}

			// �[�����苗���ȏ㗣��Ă���ꍇ�ɔ�����s��
			if( len1 > lth )
			{
				vector_type p1  = ite2->pos;
				vector_type p2  = ( ite2 - 1 )->pos;
				double length   = ( p1 - p2 ).length( );
				vector_type dir = ( p1 - p2 ).unit( );
				vector_type p   = p1 + dir * length * 0.8;

				double min = 1.0e10;
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min && cite != ite2 && l < length )
					{
						min = l;
						ratio2 = cite->round / ite2->round;
					}
				}
			}

			if( ratio1 > ratio2 )
			{
				// ���я����t�]������
				std::reverse( mesh_list.begin( ), mesh_list.begin( ) + base_mesh_num );
				site = mesh_list.begin( );
				eite = site + base_mesh_num;
			}
			else if( ratio1 == ratio2 && len0 < len1 )
			{
				// ���я����t�]������
				std::reverse( mesh_list.begin( ), mesh_list.begin( ) + base_mesh_num );
				site = mesh_list.begin( );
				eite = site + base_mesh_num;
			}
		}

		// ���Ԃɒl��ݒ肷��
		difference_type mark = 1;
		difference_type column = col;
		for( ite = site ; ite != eite ; ++ite )
		{
			ite->mark = mark++;
			ite->row = row;
			ite->col = column--;
			//std::cout << "( " << ite->row << ", " << ite->col << " )" << std::endl;
			grid( ite->row, ite->col ) = ite->pos;
		}
		for( ; ite != mesh_list.end( ) ; ++ite )
		{
			ite->mark = 100;
		}

		// �o�̓f�[�^�𐮌`����
		for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
		{
			meshes[ ite->label ].mark = ite->mark;
		}

		// ��_�̐������Ȃ��ꍇ�ɂ͏I��
		if( base_mesh_num < 3 )
		{
			for( ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; ++ite )
			{
				meshes[ ite->label ].mark = ite->mark;
			}

			for( i = 0 ; i < chart.size( ) ; i++ )
			{
				if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
				{
					chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
				}
				else
				{
					chart[ i ] = 255;
				}
			}

			return( false );
		}


		// ���܂����傫�ȂR�̉~�̔z�u�����ɃO���b�h�̍��W���X�V����
		// ���̍ہC�傫�����̂���R�_�݂̂��ŏ��Ɋm�肳����
		__mesh_information__ m1 = mesh_list[ 0 ];
		__mesh_information__ m2 = mesh_list[ 2 ];
		site = mesh_list.begin( );
		eite = site + 3;

		vector_type p1 = m1.pos;
		vector_type p2 = m2.pos;
		vector_type dir = ( p1 - p2 ).unit( );
		vector_type norm( -dir.y, dir.x );
		double length = ( p1 - p2 ).length( ) / static_cast< double >( 2 );

		// �s�p�ȃf�[�^���폜����
		mesh_list.erase( site, eite );

		// ����C���Ŋm�肵�Ă�����̂̋ߖT�݂̂��m�肳����
		difference_type rindex[] = { -1, 1 };
		for( r = 0 ; r < 2 ; r++ )
		{
			for( c = m1.col ; c >= m2.col ; c-- )
			{
				vector_type &op = grid( m1.row + rindex[ r ], c );
				vector_type p = grid( m1.row, c ) + rindex[ r ] * norm * length * 0.5;

				double min = 1.0e10;
				mesh_iterator cur = mesh_list.begin( );
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min )
					{
						min = l;
						cur = cite;
					}
				}

				// ��苗���ȏ�͂Ȃꂽ�T���_�͖�������
				if( cur != mesh_list.end( ) && min < length && min < cur->length )
				{
					// �ȑO�Ɋ��蓖�Ă��Ă����ꍇ�͐؂�ւ���
					if( cur->row >= 0 || cur->col >= 0 )
					{
						grid( cur->row, cur->col ).x = -1;
						grid( cur->row, cur->col ).y = -1;
					}

					cur->row = r;
					cur->col = c;
					cur->length = min;

					// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O���ɒǉ�����
					op = cur->pos;
					cur->mark = -2;
				}
			}
		}

		for( mesh_iterator ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; )
		{
			if( ite->mark == -2 )
			{
				ite = mesh_list.erase( ite );
			}
			else
			{
				++ite;
			}
		}
	}

	{
		for( i = 0 ; i < chart.size( ) ; i++ )
		{
			if( meshes[ binary[ i ] ].mark >= 0 && meshes[ binary[ i ] ].enabled )
			{
				chart[ i ] = static_cast< unsigned char >( meshes[ binary[ i ] ].mark );
			}
			else
			{
				chart[ i ] = 255;
			}
		}
	}


	// �����I�ɑΉ��_��������
	while( true )
	{
		difference_type ncount = 0;
		for( r = 0 ; r < rows && ncount < rows * cols ; r++ )
		{
			for( c = 0 ; c < cols && ncount < rows * cols ; c++ )
			{
				vector_type p = grid( r, c );

				if( p.x != -1 || p.y != -1 )
				{
					ncount++;
					continue;
				}

				double search_length = 0.0;

				// �ߖT�̏�Ԃ��g���āC�O���b�h��̓_�����݂���Ǝv����ʒu��\������
				if( 0 < c && c < cols - 1 && grid( r, c - 1 ).x >= 0 && grid( r, c + 1 ).x >= 0 )
				{
					p = ( grid( r, c - 1 ) + grid( r, c + 1 ) ) / 2.0;
					search_length = ( grid( r, c - 1 ) - grid( r, c + 1 ) ).length( ) / 2.0;
				}
				else if( 0 < r && r < rows - 1 && grid( r - 1, c ).x >= 0 && grid( r + 1, c ).x >= 0 )
				{
					p = ( grid( r - 1, c ) + grid( r + 1, c ) ) / 2.0;
					search_length = ( grid( r - 1, c ) - grid( r + 1, c ) ).length( ) / 2.0;
				}
				else if( c < cols - 3 && grid( r, c + 1 ).x >= 0 && grid( r, c + 2 ).x >= 0 && grid( r, c + 3 ).x >= 0 )
				{
					double l1 = ( grid( r, c + 2 ) - grid( r, c + 3 ) ).length( );
					double l2 = ( grid( r, c + 1 ) - grid( r, c + 2 ) ).length( );
					p = grid( r, c + 1 ) + ( grid( r, c + 1 ) - grid( r, c + 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( c > 3 && grid( r, c - 1 ).x >= 0 && grid( r, c - 2 ).x >= 0 && grid( r, c - 3 ).x >= 0 )
				{
					double l1 = ( grid( r, c - 2 ) - grid( r, c - 3 ) ).length( );
					double l2 = ( grid( r, c - 1 ) - grid( r, c - 2 ) ).length( );
					p = grid( r, c - 1 ) + ( grid( r, c - 1 ) - grid( r, c - 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r < rows - 3 && grid( r + 1, c ).x >= 0 && grid( r + 2, c ).x >= 0 && grid( r + 3, c ).x >= 0 )
				{
					double l1 = ( grid( r + 2, c ) - grid( r + 3, c ) ).length( );
					double l2 = ( grid( r + 1, c ) - grid( r + 2, c ) ).length( );
					p = grid( r + 1, c ) + ( grid( r + 1, c ) - grid( r + 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r > 3 && grid( r - 1, c ).x >= 0 && grid( r - 2, c ).x >= 0 && grid( r - 3, c ).x >= 0 )
				{
					double l1 = ( grid( r - 2, c ) - grid( r - 3, c ) ).length( );
					double l2 = ( grid( r - 1, c ) - grid( r - 2, c ) ).length( );
					p = grid( r - 1, c ) + ( grid( r - 1, c ) - grid( r - 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					search_length = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( c > 1 && r < rows - 1 && grid( r + 1, c ).x >= 0 && grid( r, c - 1 ).x >= 0 && grid( r + 1, c - 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r + 1, c - 1 );
					vector_type &p1 = grid( r + 1, c );
					vector_type &p2 = grid( r, c - 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else if( c > 1 && r > 1 && grid( r - 1, c ).x >= 0 && grid( r, c - 1 ).x >= 0 && grid( r - 1, c - 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r - 1, c - 1 );
					vector_type &p1 = grid( r - 1, c );
					vector_type &p2 = grid( r, c - 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else if( c < cols - 1 && r > 1 && grid( r, c + 1 ).x >= 0 && grid( r - 1, c ).x >= 0 && grid( r - 1, c + 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r - 1, c + 1 );
					vector_type &p1 = grid( r - 1, c );
					vector_type &p2 = grid( r, c + 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else if( c < cols - 1 && r < rows - 1 && grid( r, c + 1 ).x >= 0 && grid( r + 1, c ).x >= 0 && grid( r + 1, c + 1 ).x >= 0 )
				{
					vector_type &p0 = grid( r + 1, c + 1 );
					vector_type &p1 = grid( r + 1, c );
					vector_type &p2 = grid( r, c + 1 );
					vector_type d = p1 + p2 - 2.0 * p0;

					p = d + p0;
					search_length = 0.5 * d.length( );
				}
				else
				{
					// �ߖT�̓_��������Ȃ�����
					ncount++;
					continue;
				}

				// �\�����ꂽ�_��p���čŊ�̓_��T��
				double min = 1.0e10;
				mesh_iterator cur = mesh_list.begin( );
				for( mesh_iterator cite = mesh_list.begin( ) ; cite != mesh_list.end( ) ; ++cite )
				{
					double l = ( cite->pos - p ).length( );
					if( l < min )
					{
						min = l;
						cur = cite;
					}
				}

				if( cur != mesh_list.end( ) && min < search_length && min < cur->length )
				{
					// �ȑO�Ɋ��蓖�Ă��Ă����ꍇ�͐؂�ւ���
					if( cur->row >= 0 || cur->col >= 0 )
					{
						grid( cur->row, cur->col ).x = -1;
						grid( cur->row, cur->col ).y = -1;
					}

					cur->row = r;
					cur->col = c;
					cur->length = min;

					// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O���ɒǉ�����
					grid( r, c ) = cur->pos;
					cur->mark = -2;
				}
				else
				{
					// �Ή��_��������Ȃ������̂ŁC������Ȃ������}�[�N������
					grid( r, c ).x = -2;
					grid( r, c ).y = -2;
				}
			}
		}

		for( mesh_iterator ite = mesh_list.begin( ) ; ite != mesh_list.end( ) ; )
		{
			if( ite->mark == -2 )
			{
				ite = mesh_list.erase( ite );
			}
			else
			{
				++ite;
			}
		}

		if( ncount == rows * cols )
		{
			// �X�V���ׂ��_��������Ȃ������̂ŏI������
			break;
		}
	}

	// �Ǘ��_���폜����
	for( r = 0 ; r < rows ; r++ )
	{
		for( c = 0 ; c < cols ; c++ )
		{
			if( grid( r, c ).x < 0 )
			{
				continue;
			}

			bool b1 = r > 0    ? grid( r - 1, c ).x >= 0 : false;
			bool b2 = r < rows ? grid( r + 1, c ).x >= 0 : false;
			bool b3 = c > 0    ? grid( r, c - 1 ).x >= 0 : false;
			bool b4 = c < cols ? grid( r, c + 1 ).x >= 0 : false;

			size_t count = 0;
			if( r > 0    && grid( r - 1, c ).x >= 0 )
			{
				count++;
			}
			if( r < rows && grid( r + 1, c ).x >= 0 )
			{
				count++;
			}
			if( c > 0    && grid( r, c - 1 ).x >= 0 )
			{
				count++;
			}
			if( c < cols && grid( r, c + 1 ).x >= 0 )
			{
				count++;
			}

			if( count <= 1 )
			{
				grid( r, c ).x = -2;
				grid( r, c ).y = -2;
			}
		}
	}

	return( true );
}


/// @}
//  ���b�V�����o�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
