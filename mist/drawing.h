/// @file mist/drawing.h
//!
//! @brief �摜�ɑ΂��āC������~�Ȃǂ�`�悷�郉�C�u����
//!
#ifndef __INCLUDE_MIST_DRAWING__
#define __INCLUDE_MIST_DRAWING__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#include <cmath>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN




//! @addtogroup image_drawing_group ������~�̕`��
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/drawing.h>
//! @endcode
//!
//!  @{



/// @brief 2�����摜�ɒ�����`�悷��
//! 
//! ��@�ɂ��ĉ�������
//! 
//! @param[in,out] image �c ���o�͉摜
//! @param[in]     x1    �c �����̒[�_1��X���W
//! @param[in]     y1    �c �����̒[�_1��Y���W
//! @param[in]     x2    �c �����̒[�_2��X���W
//! @param[in]     y2    �c �����̒[�_2��Y���W
//! @param[in]     color �c �����̐F�������͒l
//!
template < class T, class Allocator >
void draw_line( array2< T, Allocator > &image,
			   typename array2< T, Allocator >::difference_type x1,
			   typename array2< T, Allocator >::difference_type y1,
			   typename array2< T, Allocator >::difference_type x2,
			   typename array2< T, Allocator >::difference_type y2,
			   typename array2< T, Allocator >::value_type color = typename array2< T, Allocator >::value_type( ) )
{
	typedef typename array2< T, Allocator >::difference_type difference_type;
	difference_type dx, dy;
	difference_type sx, sy;
	difference_type x, y, e, i;
	difference_type w = static_cast< difference_type >( image.width( ) );
	difference_type h = static_cast< difference_type >( image.height( ) );

	dx = std::abs( static_cast< int >( x2 - x1 ) );
	dy = std::abs( static_cast< int >( y2 - y1 ) );
	sx = ( x2 - x1 >=0 ) ? 1 : -1;
	sy = ( y2 - y1 >=0 ) ? 1 : -1;
	x = x1;
	y = y1;
	if( dx >= dy )
	{
		e = -dx;
		for( i = 0 ; i <= dx ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h )
			{
				image( x, y ) = color;
			}
			x += sx;
			e += 2 * dy;

			if( e >= 0 )
			{
				y += sy;
				e -= 2 * dx;
			}
		}
	}
	else
	{
		e = -dy;
		for( i = 0 ; i <= dy ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h )
			{
				image( x, y ) = color;
			}
			y += sy;
			e += 2 * dx;
			if( e >= 0 )
			{
				x += sx;
				e -= 2 * dy;
			}
		}
	}
}



/// @brief 3�����摜�ɒ�����`�悷��
//! 
//! ��@�ɂ��ĉ�������
//! 
//! @param[in,out] image �c ���o�͉摜
//! @param[in]     x1    �c �����̒[�_1��X���W
//! @param[in]     y1    �c �����̒[�_1��Y���W
//! @param[in]     z1    �c �����̒[�_1��Z���W
//! @param[in]     x2    �c �����̒[�_2��X���W
//! @param[in]     y2    �c �����̒[�_2��Y���W
//! @param[in]     z2    �c �����̒[�_2��Z���W
//! @param[in]     color �c �����̐F�������͒l
//!
template < class T, class Allocator >
void draw_line( array3< T, Allocator > &image,
			   typename array3< T, Allocator >::difference_type x1,
			   typename array3< T, Allocator >::difference_type y1,
			   typename array3< T, Allocator >::difference_type z1,
			   typename array3< T, Allocator >::difference_type x2,
			   typename array3< T, Allocator >::difference_type y2,
			   typename array3< T, Allocator >::difference_type z2,
			   typename array3< T, Allocator >::value_type value = typename array3< T, Allocator >::value_type( ) )
{
	typedef typename array3< T, Allocator >::difference_type difference_type;
	difference_type dx, dy, dz;
	difference_type sx, sy, sz;
	difference_type x, y, z, e1, e2, i;
	difference_type w = static_cast< difference_type >( image.width( ) );
	difference_type h = static_cast< difference_type >( image.height( ) );
	difference_type d = static_cast< difference_type >( image.depth( ) );

	dx = std::abs( static_cast< int >( x2 - x1 ) );
	dy = std::abs( static_cast< int >( y2 - y1 ) );
	dz = std::abs( static_cast< int >( z2 - z1 ) );
	sx = ( x2 - x1 >=0 ) ? 1 : -1;
	sy = ( y2 - y1 >=0 ) ? 1 : -1;
	sz = ( z2 - z1 >=0 ) ? 1 : -1;
	x = x1;
	y = y1;
	z = z1;
	if( dx >= dy && dx >= dz )
	{
		e1 = e2 = -dx;
		for( i = 0 ; i <= dx ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h && z >= 0 && z < d )
			{
				image( x, y, z ) = value;
			}
			x += sx;
			e1 += 2 * dy;
			e2 += 2 * dz;

			if( e1 >= 0 )
			{
				y += sy;
				e1 -= 2 * dx;
			}

			if( e2 >= 0 )
			{
				z += sz;
				e2 -= 2 * dx;
			}
		}
	}
	else if( dy >= dx && dy >= dz )
	{
		e1 = e2 = -dy;
		for( i = 0 ; i <= dy ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h && z >= 0 && z < d )
			{
				image( x, y, z ) = value;
			}
			y += sy;
			e1 += 2 * dx;
			e2 += 2 * dz;

			if( e1 >= 0 )
			{
				x += sx;
				e1 -= 2 * dy;
			}

			if( e2 >= 0 )
			{
				z += sz;
				e2 -= 2 * dy;
			}
		}
	}
	else
	{
		e1 = e2 = -dz;
		for( i = 0 ; i <= dz ; i++ )
		{
			if( x >= 0 && x < w && y >= 0 && y < h && z >= 0 && z < d )
			{
				image( x, y, z ) = value;
			}
			z += sz;
			e1 += 2 * dx;
			e2 += 2 * dy;

			if( e1 >= 0 )
			{
				x += sx;
				e1 -= 2 * dz;
			}

			if( e2 >= 0 )
			{
				y += sy;
				e2 -= 2 * dz;
			}
		}
	}
}



/// @}
//  ������~�̕`��O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DRAWING__
