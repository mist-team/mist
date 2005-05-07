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
//! @attention ���b�V���̏㉺���E�����߂�_��L���^��4�_�z�u���CL�̒�ӂ�3�_��z�u����D
//! 
//! �o�͂���郁�b�V���̍��W�̂����C�Ή�����_��������Ȃ����̂ɂ� (-1,-1) ���o�͂���D
//! 
//! @param[in]     chart  �c ���͉摜�i���b�V�����f���Ă���摜�j
//! @param[in,out] grid   �c ���b�V���̊e�X�̍��W����͂Ƃ��C�摜���̍��W���i�[�������
//! @param[in]     leftX  �c ���b�V���̏㉺���E�����߂邽�߂̍����̓_�̃��b�V����ł̗�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     leftY  �c ���b�V���̏㉺���E�����߂邽�߂̍����̓_�̃��b�V����ł̍s�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     rightX �c ���b�V���̏㉺���E�����߂邽�߂̉E���̓_�̃��b�V����ł̗�����̈ʒu�i�O���琔�������̈ʒu�j
//! @param[in]     rightY �c ���b�V���̏㉺���E�����߂邽�߂̉E���̓_�̃��b�V����ł̍s�����̈ʒu�i�O���琔�������̈ʒu�j
//! 
//! @return ���b�V���̒��o�ɐ����������ǂ���
//! 
template < class T, class Allocator >
bool extract_mesh( const array2< T, Allocator > &chart, matrix< vector2< double > > &grid,
					typename array2< T, Allocator >::difference_type leftX, typename array2< T, Allocator >::difference_type leftY,
					typename array2< T, Allocator >::difference_type rightX, typename array2< T, Allocator >::difference_type rightY )
{
	typedef array2< T, Allocator >::size_type size_type;
	typedef array2< T, Allocator >::difference_type difference_type;
	typedef vector2< double > vector_type;
	array2< size_type > binary;


	// �Q�l�摜�ɕϊ�
	convert( chart, binary );

	size_type i, j;

	size_type labelnum = labeling4( binary, binary );
	//std::cout << "LabelNum: " << labelnum << std::endl;

	array< bool > mask( labelnum + 1 );
	array< difference_type > count( labelnum + 1 );
	array< difference_type > round( labelnum + 1 );
	array< vector_type > pos( labelnum + 1 );

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
			unsigned short label = binary( i, j );
			if( label != 0 )
			{
				double x = i - pos[ label ].x;
				double y = j - pos[ label ].y;
				const double pai = 3.1415926535897932384626433832795;
				if( x * x + y * y <= count[ label ] / pai * 2.25 )
				{
					round[ binary( i, j ) ]++;
				}
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
		if( count[ i ] < minimum_count )
		{
			count[ i ] = 0;
			mask[ i ] = true;
		}
		else
		{
			const double pai = 3.1415926535897932384626433832795;
			double e = round[ i ] / static_cast< double >( count[ i ] );
			if( e < 0.95 )
			{
				count[ i ] = 0;
				mask[ i ] = true;
			}
			//std::cout << "���x��: " << i << ", �~�`�x: " << e << std::endl;
		}
	}

	// �̈�̖ʐς̂P�ԑ傫�����̂�I��
	difference_type index1 = -1;
	difference_type index2 = -1;
	difference_type index3 = -1;
	difference_type index4 = -1;
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
	// 4�Ԗڂɑ傫�ȗ̈��������
	for( i = 1, max = 0 ; i <= labelnum ; i++ )
	{
		if( i != index1 && i != index2 && i != index3 && count[ i ] > max )
		{
			max = count[ i ];
			index4 = i;
		}
	}

	vector_type p1, p2, p3;
	vector_type p1_ = grid( leftX, leftY );
	vector_type p2_ = grid( leftX, rightY );
	vector_type p3_ = grid( rightX, rightY );
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

		for( r = 0 ; r < rows ; r++ )
		{
			for( c = 0 ; c < cols ; c++ )
			{
				vector_type &p = grid( r, c );
				p = p2 + dX * ( c - leftX ) + dY * ( rightY - r );
			}
		}
	}


	// �ϊ���̃O���b�h�ʒu���B�e���ꂽ�G�̉~���ɓ����Ă��āC������C������O���2���C���ȓ��̂��̂�D��I�Ɋ��蓖�Ă�
	matrix< int > found( rows, cols );
	matrix< double > flength( rows, cols );
	for( r = rightY ; r >= rightY - 1 ; r-- )
	{
		for( c = leftX ; c <= rightX ; c++ )
		{
			if( found( r, c ) == 1 )
			{
				continue;
			}

			vector_type &p = grid( r, c );
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
				// �Ή��_��������Ȃ������̂ŁC������Ȃ������}�[�N������
				p.x = -1;
				p.y = -1;
			}
			else
			{
				// �Ή��_�����������̂ŁC�ȍ~�̒T�����珜�O����
				vector_type diff = pos[ index ] - p;
				p = pos[ index ];
				found( r, c ) = 1;
				mask[ index ] = true;

				if( r == rightY )
				{
					for( difference_type rr = 0 ; rr < rows ; rr++ )
					{
						if( found( rr, c ) == 0 )
						{
							grid( rr, c ) += diff;
						}
					}
				}
			}
		}
	}

	for( r = rightY ; r >= leftY ; r-- )
	{
		for( c = leftX ; c <= leftX + 1 ; c++ )
		{
			if( found( r, c ) == 1 )
			{
				continue;
			}

			vector_type &p = grid( r, c );
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
				// �Ή��_��������Ȃ������̂ŁC������Ȃ������}�[�N������
				p.x = -1;
				p.y = -1;
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

	// �ēx�C�O���b�h�̃A�b�v�f�[�g���s��
	for( r = rightY ; r >= leftY ; r-- )
	{
		for( c = leftX ; c <= rightX ; c++ )
		{
			if( found( r, c ) == 1 )
			{
				continue;
			}

			vector_type &p = grid( r, c );
			grid( r, c ) = grid( r + 1, c ) + grid( r, c - 1 ) - grid( r + 1, c - 1 );

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
				// �Ή��_��������Ȃ������̂ŁC������Ȃ������}�[�N������
				p.x = -1;
				p.y = -1;
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
					// �Ή��_��������Ȃ������̂ŁC������Ȃ������}�[�N������
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
