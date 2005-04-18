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

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup mesh_group ���b�V�����o
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/utility/mesh.h>
//! @endcode
//!
//!  @{


/// @brief �~���i�q��ɕ��ׂ��摜����C�e�X�𒊏o�����b�V����̓_�ƑΉ��t����
//! 
//! @attention ���͉摜�͂��炩���߁C�������l�������s���Ă����K�v������D
//! @attention �O���b�h�̓_�͍�������_�Ƃ���D
//! @attention ���b�V���̏㉺���E�����߂�_�͂R�_���͂��C�����ɑ傫�ȉ~���Q�߂Â��Ĕz�u����D
//! 
//! �o�͂���郁�b�V���̍��W�̂����C�Ή�����_��������Ȃ����̂ɂ� (-1,-1) ���o�͂���D
//! 
//! @param[in]     chart           �c ���͉摜�i���b�V�����f���Ă���摜�j
//! @param[in,out] grid            �c ���b�V���̊e�X�̍��W����͂Ƃ��C�摜���̍��W���i�[�������
//! @param[in]     left_circularX  �c ���b�V���̏㉺���E�����߂邽�߂̍����̓_�̃��b�V����ł̗�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     right_circularX �c ���b�V���̏㉺���E�����߂邽�߂̉E���̓_�̃��b�V����ł̗�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     circularY       �c ���b�V���̏㉺���E�����߂邽�߂̂Q�̓_�̃��b�V����ł̍s�����̈ʒu�i�O���琔�������̈ʒu�j
//! 
//! @return ���b�V���̒��o�ɐ����������ǂ���
//! 
template < class T, class Allocator >
bool extract_mesh( const array2< T, Allocator > &chart, matrix< vector2< double > > &grid, size_t left_circularX, size_t right_circularX, size_t circularY )
{
	typedef array2< T, Allocator >::size_type size_type;
	typedef array2< T, Allocator >::difference_type difference_type;
	typedef vector2< double > vector_type;
	array2< unsigned short > binary;


	// �Q�l�摜�ɕϊ�
	convert( chart, binary );

	size_type i, j;

	size_type labelnum = labeling4( binary, binary );
	//std::cout << "LabelNum: " << labelnum << std::endl;

	array< bool > mask( labelnum + 1 );
	difference_type *count = new difference_type[ labelnum + 1 ];
	difference_type *round = new difference_type[ labelnum + 1 ];
	vector_type *pos = new vector2< double >[ labelnum + 1 ];

	for( i = 0 ; i <= labelnum ; i++ )
	{
		count[ i ] = 0;
		round[ i ] = 0;
		pos[ i ].x = 0;
		pos[ i ].y = 0;
	}

	// �e���x���̉�f���Əd�S�ʒu�����߂�
	for( j = 0 ; j < chart.height( ) ; j++ )
	{
		for( i = 0 ; i < chart.width( ) ; i++ )
		{
			size_type label = binary( i, j );
			if( label > 0 )
			{
				count[ label ]++;
				pos[ label ].x += i;
				pos[ label ].y += j;
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

	// �~�`�łȂ��̈����菜�����߂ɁC�e�̈�̋��E��f�̐����J�E���g����
	for( j = 1 ; j < chart.height( ) - 1 ; j++ )
	{
		for( i = 1 ; i < chart.width( ) - 1 ; i++ )
		{
			if( binary( i, j ) != 0 && ( binary( i - 1, j ) == 0 || binary( i + 1, j ) == 0 || binary( i, j - 1 ) == 0 || binary( i, j + 1 ) == 0 ) )
			{
				round[ binary( i, j ) ]++;
			}
		}
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
		const double pai = 3.1415926535897932384626433832795;
		double e = 4.0 * pai * count[ i ] / static_cast< double >( round[ i ] * round[ i ] );
		if( count[ i ] < minimum_count || e < 0.95 )
		{
			count[ i ] = 0;
			mask[ i ] = true;
		}
	}

	delete [] round;

	// �s�v�ȗ̈���摜�����菜��
	//for( j = 0 ; j < chart.height( ) - 0 ; j++ )
	//{
	//	for( i = 0 ; i < chart.width( ) - 0 ; i++ )
	//	{
	//		if( count[ binary( i, j ) ] == 0 )
	//		{
	//			chart( i, j ) = 255;
	//		}
	//	}
	//}

	// �̈�̖ʐς̂P�ԑ傫�����̂�I��
	difference_type index1 = 0;
	difference_type index2 = 0;
	difference_type index3 = 0;
	difference_type max = 0;
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( count[ i ] > max )
		{
			max = count[ i ];
			index1 = i;
		}
	}
	// 2�Ԗڂɑ傫�ȗ̈��������
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( i != index1 && count[ i ] > max )
		{
			max = count[ i ];
			index2 = i;
		}
	}
	// 3�Ԗڂɑ傫�ȗ̈��������
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( i != index1 && i != index2 && count[ i ] > max )
		{
			max = count[ i ];
			index3 = i;
		}
	}

	difference_type lindex, rindex;
	vector_type p1, p2;
	vector_type p1_ = grid( circularY, left_circularX );
	vector_type p2_ = grid( circularY, right_circularX );
	// �ʐς̑傫���̈�ŁC�ׂ荇���Ă���ق����P�C�����łȂ��ق���2�ƃ��x������
	{
		vector_type pp1 = pos[ index1 ];
		vector_type pp2 = pos[ index2 ];
		vector_type pp3 = pos[ index3 ];
		double len12 = ( pp1 - pp2 ).length( );
		double len13 = ( pp1 - pp3 ).length( );
		double len23 = ( pp2 - pp3 ).length( );

		if( len12 < len13 && len12 < len23 )
		{
			p2 = pp3;
			rindex = index3;
			if( len13 < len23 )
			{
				p1 = pp2;
				lindex = index2;
			}
			else
			{
				p1 = pp1;
				lindex = index1;
			}
		}
		else if( len13 < len12 && len13 < len23 )
		{
			p2 = pp2;
			rindex = index2;
			if( len12 < len23 )
			{
				p1 = pp3;
				lindex = index3;
			}
			else
			{
				p1 = pp1;
				lindex = index1;
			}
		}
		else
		{
			p2 = pp1;
			rindex = index1;
			if( len12 < len13 )
			{
				p1 = pp3;
				lindex = index3;
			}
			else
			{
				p1 = pp2;
				lindex = index2;
			}
		}
	}


	// ���܂����傫�ȂQ�̉~�̔z�u�����ɃO���b�h�̍��W���X�V����
	{
		double len1 = ( p2  - p1  ).length( );
		double len2 = ( p2_ - p1_ ).length( );
		double scale = len1 / len2;
		vector_type dp = p1 - p1_;
		vector_type d1 = ( p2  - p1  ).unit( );
		vector_type d2 = ( p2_ - p1_ ).unit( );
		double c = d1 ^ d2;
		double s = std::sqrt( 1.0 - c * c );
		if( ( d2.x * c - d1.x ) * d2.y < 0 || ( d1.y - d2.y * c ) * d2.x < 0 )
		{
			s = -s;
		}

		for( i = 0 ; i < grid.size( ) ; i++ )
		{
			vector_type v = grid[ i ] - p1_;
			grid[ i ].x = ( v.x * c - v.y * s ) * scale + p1.x;
			grid[ i ].y = ( v.x * s + v.y * c ) * scale + p1.y;
		}
	}

	// �ϊ���̃O���b�h�ʒu���B�e���ꂽ�G�̉~���ɓ����Ă��āC������C������O���2���C���ȓ��̂��̂�D��I�Ɋ��蓖�Ă�
	difference_type r, c, rows = grid.rows( ), cols = grid.cols( );
	difference_type rrr[ 3 ] = { 0, 1, -1 };
	matrix< int > found( rows, cols );
	matrix< double > flength( rows, cols );
	for( r = 0 ; r <= 2 ; r++ )
	{
		for( c = left_circularX ; c <= static_cast< difference_type >( right_circularX ) ; c++ )
		{
			vector_type &p = grid( circularY - rrr[ r ], c );
			size_type x = static_cast< size_type >( p.x );
			size_type y = static_cast< size_type >( p.y );

			difference_type index = -1;
			double min = 1.0e10;
			for( i = 1 ; i <= labelnum ; i++ )
			{
				if( !mask[ i ] )
				{
					double l = ( pos[ i ] - p ).length( );
					if( l < min )
					{
						min = l;
						index = i;
					}
				}
			}

			if( index < 0 )
			{
				// �Ή��X��������Ȃ������̂ŁC������Ȃ������}�[�N������
				p.x = -1;
				p.y = -1;
			}
			else
			{
				// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O����
				p = pos[ index ];
				found( circularY - rrr[ r ], c ) = 1;
				mask[ index ] = true;
			}
		}
	}

	while( true )
	{
		difference_type ncount = 0;
		for( r = 0 ; r < rows ; r++ )
		{
			for( c = 0 ; c < cols ; c++ )
			{
				if( found( r, c ) >= 1 )
				{
					ncount++;
					continue;
				}

				// �ߖT�̏�Ԃ��g���āC�O���b�h��̓_�����݂���Ǝv����ʒu��\������
				if( 0 < c && c < cols - 1 && found( r, c - 1 ) == 1 && found( r, c + 1 ) == 1 )
				{
					grid( r, c ) = ( grid( r, c - 1 ) + grid( r, c + 1 ) ) / 2.0;
					found( r, c ) = -1;
					flength( r, c ) = ( grid( r, c - 1 ) - grid( r, c + 1 ) ).length( ) / 2.0;
				}
				else if( 0 < r && r < rows - 1 && found( r - 1, c ) == 1 && found( r + 1, c ) == 1 )
				{
					grid( r, c ) = ( grid( r - 1, c ) + grid( r + 1, c ) ) / 2.0;
					found( r, c ) = -1;
					flength( r, c ) = ( grid( r - 1, c ) - grid( r + 1, c ) ).length( ) / 2.0;
				}
				else if( c < cols - 3 && found( r, c + 1 ) == 1 && found( r, c + 2 ) == 1 && found( r, c + 3 ) == 1 )
				{
					double l1 = ( grid( r, c + 2 ) - grid( r, c + 3 ) ).length( );
					double l2 = ( grid( r, c + 1 ) - grid( r, c + 2 ) ).length( );
					grid( r, c ) = grid( r, c + 1 ) + ( grid( r, c + 1 ) - grid( r, c + 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( c > 3 && found( r, c - 1 ) == 1 && found( r, c - 2 ) == 1 && found( r, c - 3 ) == 1 )
				{
					double l1 = ( grid( r, c - 2 ) - grid( r, c - 3 ) ).length( );
					double l2 = ( grid( r, c - 1 ) - grid( r, c - 2 ) ).length( );
					grid( r, c ) = grid( r, c - 1 ) + ( grid( r, c - 1 ) - grid( r, c - 2 ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r < rows - 3 && found( r + 1, c ) == 1 && found( r + 2, c ) == 1 && found( r + 3, c ) == 1 )
				{
					double l1 = ( grid( r + 2, c ) - grid( r + 3, c ) ).length( );
					double l2 = ( grid( r + 1, c ) - grid( r + 2, c ) ).length( );
					grid( r, c ) = grid( r + 1, c ) + ( grid( r + 1, c ) - grid( r + 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else if( r > 3 && found( r - 1, c ) == 1 && found( r - 2, c ) == 1 && found( r - 3, c ) == 1 )
				{
					double l1 = ( grid( r - 2, c ) - grid( r - 3, c ) ).length( );
					double l2 = ( grid( r - 1, c ) - grid( r - 2, c ) ).length( );
					grid( r, c ) = grid( r - 1, c ) + ( grid( r - 1, c ) - grid( r - 2, c ) ).unit( ) * ( 2.0 * l2 - l1 );
					found( r, c ) = -1;
					flength( r, c ) = ( 2.0 * l2 - l1 ) / 2.0;
				}
				else
				{
					// �ߖT�̓_��������Ȃ�����
					ncount++;
				}
			}
		}

		if( ncount == rows * cols )
		{
			// �X�V���ׂ��_��������Ȃ������̂ŏI������
			break;
		}

		// �\�����ꂽ�ʒu����ɂ��āC�Ŋ�̓_�����߂�
		for( r = 0 ; r < rows ; r++ )
		{
			for( c = 0 ; c < cols ; c++ )
			{
				if( found( r, c ) != -1 )
				{
					continue;
				}

				vector_type &p = grid( r, c );
				size_type x = static_cast< size_type >( p.x );
				size_type y = static_cast< size_type >( p.y );
				double length_threshold = flength( r, c );

				difference_type index = -1;
				double min = 1.0e10;
				for( i = 1 ; i <= labelnum ; i++ )
				{
					if( !mask[ i ] )
					{
						double l = ( pos[ i ] - p ).length( );
						if( l < min )
						{
							min = l;
							index = i;
						}
					}
				}

				if( index < 0 || min > length_threshold )
				{
					// �Ή��X��������Ȃ������̂ŁC������Ȃ������}�[�N������
					p.x = -1;
					p.y = -1;
					found( r, c ) = 2;
				}
				else
				{
					// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O����
					p = pos[ index ];
					found( r, c ) = 1;
					mask[ index ] = true;
				}
			}
		}
	}


	delete [] count;
	delete [] pos;

	{
		difference_type width = chart.width( );
		difference_type height = chart.height( );
		for( size_type r = 0 ; r < grid.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < grid.cols( ) ; c++ )
			{
				difference_type x = static_cast< difference_type >( grid( r, c ).x );
				difference_type y = static_cast< difference_type >( grid( r, c ).y );
				if( found( r, c ) != 1 || x < 0 || y < 0 )
				{
					// �_�̌�����Ȃ��������̂̍��W�Ƀ}�[�N������
					grid( r, c ).x = -1;
					grid( r, c ).y = -1;
					continue;
				}
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
