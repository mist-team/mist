// 
// Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University
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

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup mesh_group ���b�V�����o
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/utility/mesh.h>
//! @endcode
//!
//!  @{


/// @brief �~���i�q��ɕ��ׂ��摜����C�e�_�𒊏o�����b�V����̓_�ƑΉ��t����
//! 
//! @attention ���͉摜�͂��炩���߁C�������l�������s���Ă����K�v������D
//! @attention �O���b�h�̓_�͍�������_�Ƃ���D
//! @attention ���b�V���̏㉺���E�����߂�_��L���^��4�_�z�u���CL�̒�ӂ�3�_��z�u����D
//! 
//! �o�͂���郁�b�V���̍��W�̂����C�Ή�����_��������Ȃ����̂ɂ� (-1,-1) ���o�͂���D
//! 
//! @param[in]     chart  �c ���͉摜�i���b�V�����f���Ă���摜�j
//! @param[in,out] grid   �c ���b�V���̊e�_�̍��W����͂Ƃ��C�摜���̍��W���i�[�������
//! @param[in]     leftX  �c ���b�V���̏㉺���E�����߂邽�߂̍����̓_�̃��b�V����ł̗�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     leftY  �c ���b�V���̏㉺���E�����߂邽�߂̍����̓_�̃��b�V����ł̍s�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     rightX �c ���b�V���̏㉺���E�����߂邽�߂̉E���̓_�̃��b�V����ł̗�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     rightY �c ���b�V���̏㉺���E�����߂邽�߂̉E���̓_�̃��b�V����ł̍s�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     threshold_for_circular_ratio �c ���b�V���̌����i�荞�ނ��߂̉~�`�x�̂������l�i�����菬�����l�̂��̂͏��O�j
//! 
//! @return ���b�V���̒��o�ɐ����������ǂ���
//! 
template < class T, class Allocator >
bool extract_mesh( const array2< T, Allocator > &chart, matrix< vector2< double > > &grid,
					typename array2< T, Allocator >::difference_type leftX, typename array2< T, Allocator >::difference_type leftY,
					typename array2< T, Allocator >::difference_type rightX, typename array2< T, Allocator >::difference_type rightY,
					double threshold_for_circular_ratio = 0.4 )
{
	typedef array2< T, Allocator >::size_type size_type;
	typedef array2< T, Allocator >::difference_type difference_type;
	typedef vector2< double > vector_type;
	typedef std::deque< size_type > label_list_type;
	typedef typename label_list_type::iterator label_iterator;
	array2< size_type > binary;


	// �Q�l�摜�ɕϊ�
	convert( chart, binary );

	size_type i, j;

	size_type labelnum = labeling4( binary, binary );
	//std::cout << "LabelNum: " << labelnum << std::endl;

	label_list_type label_list;
	array< difference_type > count( labelnum + 1 );
	array< double > round( labelnum + 1 );
	array< vector_type > pos( labelnum + 1 );

	for( i = 0 ; i <= labelnum ; i++ )
	{
		count[ i ] = 0;
		round[ i ] = 0;
		pos[ i ].x = 0;
		pos[ i ].y = 0;
	}

	// �e���x���̉�f���C�d�S�ʒu�C���͒������߂�
	const double _2 = std::sqrt( 2.0 );
	for( j = 0 ; j < chart.height( ) - 1 ; j++ )
	{
		for( i = 0 ; i < chart.width( ) - 1 ; i++ )
		{
			size_type label = binary( i, j );
			if( label > 0 )
			{
				count[ label ]++;
				pos[ label ].x += i;
				pos[ label ].y += j;
			}

			size_type l0 = label;
			size_type l1 = binary( i + 1, j     );
			size_type l2 = binary( i    , j + 1 );
			size_type l3 = binary( i + 1, j + 1 );

			if( l0 > 0 )
			{
				size_type num_eq = 0;
				if( l1 == l0 ) num_eq++;
				if( l2 == l0 ) num_eq++;
				if( l3 == l0 ) num_eq++;

				if( num_eq == 2 )
				{
					round[ l0 ] += _2;
				}
				else if( num_eq == 1 && l0 != l3 )
				{
					round[ l0 ] += 1.0;
				}
			}
			else if( l1 > 0 )
			{
				if( l1 == l3 )
				{
					if( l1 == l2 )
					{
						round[ l1 ] += _2;
					}
					else
					{
						round[ l1 ] += 1.0;
					}
				}
			}
			else if( l2 > 0 )
			{
				if( l2 == l3 )
				{
					round[ l2 ] += 1.0;
				}
			}
		}
	}


	// �e�̈�̏d�S�ʒu���v�Z����
	for( i = 1 ; i <= labelnum ; i++ )
	{
		double num = count[ i ];
		pos[ i ].x /= num;
		pos[ i ].y /= num;
	}

	// ��ʂ̋��E�ɐڂ��Ă���̈����菜��
	for( i = 0 ; i < chart.width( ) ; i++ )
	{
		if( binary( i, 0 ) != 0 )
		{
			count[ binary( i, 0 ) ] = 0;
		}
		if( binary( i, chart.height( ) - 1 ) != 0 )
		{
			count[ binary( i, chart.height( ) - 1 ) ] = 0;
		}
	}

	// ��ʂ̋��E�ɐڂ��Ă���̈����菜��
	for( j = 0 ; j < chart.height( ) ; j++ )
	{
		if( binary( 0, j ) != 0 )
		{
			count[ binary( 0, j ) ] = 0;
		}
		if( binary( chart.width( ) - 1, j ) != 0 )
		{
			count[ binary( chart.width( ) - 1, j ) ] = 0;
		}
	}

	// �~�`�x��]�����āC�s�v�ȗ̈����������
	difference_type minimum_count = 15; // ���̌��ɖ����Ȃ��̈�͍폜����
	for( i = 1 ; i <= labelnum ; i++ )
	{
		if( count[ i ] < minimum_count )
		{
			count[ i ] = 0;
			round[ i ] = 0;
		}
		else
		{
			const double pai = 3.1415926535897932384626433832795;
			const double S = count[ i ];
			const double L = round[ i ];
			double e = 4.0 * pai * S / ( L * L );
			if( e < threshold_for_circular_ratio )
			{
				count[ i ] = 0;
				round[ i ] = 0;
			}
			else
			{
				label_list.push_back( i );
			}
			//std::cout << "���x��: " << i << ", �~�`�x: " << e << std::endl;
		}
	}

	// �̈�̎��͒����傫�����̂��珇�Ԃ�4�I��
	difference_type index1 = -1;
	difference_type index2 = -1;
	difference_type index3 = -1;
	difference_type index4 = -1;
	double max1 = 0, max2 = 0, max3 = 0, max4 = 0;
	for( i = 1 ; i <= labelnum ; i++ )
	{
		double len = round[ i ];
		if( len > max1 )
		{
			index4 = index3;
			index3 = index2;
			index2 = index1;
			index1 = i;
			max4 = max3;
			max3 = max2;
			max2 = max1;
			max1 = len;
		}
		else if( len > max2 )
		{
			index4 = index3;
			index3 = index2;
			index2 = i;
			max4 = max3;
			max3 = max2;
			max2 = len;
		}
		else if( len > max3 )
		{
			index4 = index3;
			index3 = i;
			max4 = max3;
			max3 = len;
		}
		else if( len > max4 )
		{
			index4 = i;
			max4 = len;
		}
	}

	// ���S�̉~�����������Ȃ�
	if( index1 < 0 || index2 < 0 || index3 < 0 || index4 < 0 )
	{
		return( false );
	}


	vector_type p1, p2, p3;
	// �ʐς̑傫���̈�ŁC��4�̓_�����肷��
	{
		difference_type i1 = index1;
		difference_type i2 = index2;
		difference_type i3 = index3;
		difference_type i4 = index4;
		vector_type pp1 = pos[ index1 ];
		vector_type pp2 = pos[ index2 ];
		vector_type pp3 = pos[ index3 ];
		vector_type pp4 = pos[ index4 ];
		double len12 = ( pp1 - pp2 ).length( );
		double len13 = ( pp1 - pp3 ).length( );
		double len14 = ( pp1 - pp4 ).length( );
		double len23 = ( pp2 - pp3 ).length( );
		double len24 = ( pp2 - pp4 ).length( );
		double len34 = ( pp3 - pp4 ).length( );

		// �O�p�`���\������R�_��I��
		double l123 = len12 + len23 + len13;
		double l124 = len12 + len24 + len14;
		double l134 = len13 + len34 + len14;
		double l234 = len23 + len34 + len24;

		if( l123 >= l124 && l123 >= l134 && l123 >= l234 )
		{
			index1 = i1;
			index2 = i2;
			index3 = i3;
			index4 = i4;
		}
		else if( l124 >= l123 && l124 >= l134 && l124 >= l234 )
		{
			index1 = i1;
			index2 = i2;
			index3 = i4;
			index4 = i3;
		}
		else if( l134 >= l123 && l134 >= l124 && l134 >= l234 )
		{
			index1 = i1;
			index2 = i4;
			index3 = i3;
			index4 = i2;
		}
		else
		{
			index1 = i4;
			index2 = i2;
			index3 = i3;
			index4 = i1;
		}
	}

	// �ʐς̑傫���̈�ŁCL�̍���̓_�����肷��
	{
		difference_type i1 = index1;
		difference_type i2 = index2;
		difference_type i3 = index3;
		difference_type i4 = index4;
		vector_type pp1 = pos[ index1 ];
		vector_type pp2 = pos[ index2 ];
		vector_type pp3 = pos[ index3 ];
		vector_type pp4 = pos[ index4 ];

		// ��4�̓_����ł������_��I�сC����̔ԍ����P�ɂ���
		double len14 = ( pp1 - pp4 ).length( );
		double len24 = ( pp2 - pp4 ).length( );
		double len34 = ( pp3 - pp4 ).length( );

		if( len14 >= len24 && len14 >= len34 )
		{
			index1 = i1;
			index2 = i2;
			index3 = i3;
		}
		else if( len24 >= len14 && len24 >= len34 )
		{
			index1 = i2;
			index2 = i1;
			index3 = i3;
		}
		else
		{
			index1 = i3;
			index2 = i1;
			index3 = i2;
		}
	}

	{
		// �x�N�g���̊O�ς����ʂ̎�O�������悤�Ɋ��蓖�Ă�
		vector_type p12 = pos[ index1 ] - pos[ index2 ];
		vector_type p32 = pos[ index3 ] - pos[ index2 ];
		vector3< double > v12( p12.x, p12.y, 0 );
		vector3< double > v32( p32.x, p32.y, 0 );
		//�@vector3< double > v = v32.outer( v12 );
		if( v32.outer( v12 ).z >= 0 )
		{
			difference_type tmp = index2;
			index2 = index3;
			index3 = tmp;
		}

		p1 = pos[ index1 ];
		p2 = pos[ index2 ];
		p3 = pos[ index3 ];
	}


	// ���܂����傫�ȂR�̉~�̔z�u�����ɃO���b�h�̍��W���X�V����
	difference_type r, c, rows = grid.rows( ), cols = grid.cols( );
	{
		double scaleX = ( p3  - p2 ).length( ) / static_cast< double >( rightX - leftX );
		double scaleY = ( p1  - p2 ).length( ) / static_cast< double >( leftY - rightY );

		if( scaleX < 0 )
		{
			scaleX = -scaleX;
		}
		if( scaleY < 0 )
		{
			scaleY = -scaleY;
		}

		vector_type dX = ( p3  - p2 ).unit( ) * scaleX;
		vector_type dY = ( p1  - p2 ).unit( ) * scaleY;

		// �S�ẴO���b�h������������
		for( size_type i = 0 ; i < grid.size( ) ; i++ )
		{
			grid[ i ].x = -1;
			grid[ i ].y = -1;
		}

		// �}�[�N�����_�ɂ̐���ɏ����̂��Ɋ��蓖�Ă�
		for( c = leftX, r = rightY ; c <= rightX ; c++ )
		{
			vector_type &p = grid( r, c );
			p = p2 + dX * ( c - leftX ) + dY * ( rightY - r );

			double min = 1.0e10;
			label_iterator cur = label_list.end( );
			for( label_iterator ite = label_list.begin( ) ; ite != label_list.end( ) ; ++ite )
			{
				double l = ( pos[ *ite ] - p ).length( );
				if( l < min )
				{
					min = l;
					cur = ite;
				}
			}

			if( cur != label_list.end( ) )
			{
				// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O����
				p = pos[ *cur ];
				label_list.erase( cur );
			}
		}

		for( r = rightY - 1, c = leftX ; r >= leftY ; r-- )
		{
			vector_type &p = grid( r, c );
			p = p2 + dX * ( c - leftX ) + dY * ( rightY - r );

			double min = 1.0e10;
			label_iterator cur = label_list.end( );
			for( label_iterator ite = label_list.begin( ) ; ite != label_list.end( ) ; ++ite )
			{
				double l = ( pos[ *ite ] - p ).length( );
				if( l < min )
				{
					min = l;
					cur = ite;
				}
			}

			if( cur != label_list.end( ) )
			{
				// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O����
				p = pos[ *cur ];
				label_list.erase( cur );
			}
		}
	}


	while( true )
	{
		difference_type ncount = 0;
		for( r = 0 ; r < rows && ncount < rows * cols ; r++ )
		{
			for( c = 0 ; c < cols && ncount < rows * cols ; c++ )
			{
				vector_type &p = grid( r, c );

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
				label_iterator cur = label_list.end( );
				for( label_iterator ite = label_list.begin( ) ; ite != label_list.end( ) ; ++ite )
				{
					double l = ( pos[ *ite ] - p ).length( );
					if( l < min )
					{
						min = l;
						cur = ite;
					}
				}

				if( cur != label_list.end( ) && min < search_length )
				{
					// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O����
					p = pos[ *cur ];
					label_list.erase( cur );
				}
				else
				{
					// �Ή��_��������Ȃ������̂ŁC������Ȃ������}�[�N������
					p.x = p.y = -2;
				}
			}
		}

		if( ncount == rows * cols )
		{
			// �X�V���ׂ��_��������Ȃ������̂ŏI������
			break;
		}
	}

	return( true );
}


/// @}
//  ���b�V�����o�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
