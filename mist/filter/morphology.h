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

/// @file mist/filter/morphology.h
//!
//! @brief �e�����̉摜�ɑΉ����C�C�ӂ̍\���v�f�ɑΉ����������t�H���W���Z���s�����߂̃��C�u����
//!
//! @attention �\���v�f�̌`�͓ʌ`��łȂ��Ă͂Ȃ�Ȃ�
//!

#ifndef __INCLUDE_MIST_MORPHOLOGY__
#define __INCLUDE_MIST_MORPHOLOGY__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#include <vector>
#include <cmath>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @addtogroup morphology_group �����t�H���W���Z�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/morphology.h>
//! @endcode
//!
//!  @{


/// @brief �����t�H���W���Z�ɗ��p����\���v�f�֘A
//! 
//! �����t�H���W���Z�ɗ��p����\���v�f���쐬���邽�߂̊֐��E�\���v�f���`���Ă���
//! 
namespace morphology
{
	/// @brief �\���v�f�̊e�v�f�̍��W�Ɛ������Ԃ�\���\����
	struct point
	{
		typedef ptrdiff_t difference_type;	///< @brief �����t�����^
		difference_type x;					///< @brief X�������̍��W�l
		difference_type y;					///< @brief Y�������̍��W�l
		difference_type z;					///< @brief Z�������̍��W�l
		size_t life;						///< @brief ���̗v�f���\���̂��甲����܂ł̐�������

		/// @brief �R���X�g���N�^
		//! 
		//! @param[in] xx �c X���W�l
		//! @param[in] yy �c Y���W�l
		//! @param[in] zz �c Z���W�l
		//! @param[in] l  �c ��������
		//! 
		point( difference_type xx, difference_type yy, difference_type zz, size_t l ) : x( xx ), y( yy ), z( zz ), life( l )
		{
		}
	};


	/// @brief �\���v�f�̊e�v�f�̍��W�Ɛ������Ԃ�\���\����
	//!
	//! @attention MIST�֐������ŗ��p
	//!
	struct pointer_diff
	{
		ptrdiff_t diff;		///< @brief ���S�v�f����̃|�C���^�̍�
		size_t    life;		///< @brief ��������

		/// @brief �R���X�g���N�^
		pointer_diff( ptrdiff_t d, size_t l ) : diff( d ), life( l )
		{
		}
	};


	/// @brief �����t�H���W���Z�ɗ��p����\���v�f
	//! 
	//! �{�\���̂��CErosion�CDilation�COpening�CClosing�̊֐��ɓn���D
	//! �{�f�[�^�\���ɂ���āC�C�ӂ̓ʌ`��̍\���v�f�̃����t�H���W���Z�����s�\�D
	//! 
	struct morphology_structure
	{
		typedef std::vector< point > list_type;		///< @brief �\���v�f���̍��W�l���i�[���郊�X�g�^
		list_type object;							///< @brief �\���v�f���̑S�v�f�̍��W�l���i�[���郊�X�g
		list_type update;							///< @brief �\���v�f��X��������1��f�ړ��������ɁC�V�K�ɍ\���v�f�Ɋ܂܂��v�f�̍��W�l���i�[���郊�X�g
		size_t margin_x;							///< @brief X�������ō\���v�f�̒��S����[�܂ł̋����ő傫�����̋���
		size_t margin_y;							///< @brief Y�������ō\���v�f�̒��S����[�܂ł̋����ő傫�����̋���
		size_t margin_z;							///< @brief Z�������ō\���v�f�̒��S����[�܂ł̋����ő傫�����̋���
	};


	/// @brief �����t�H���W���Z�ɗp����~�\���v�f
	//!
	//! resoX�CresoY �̒l��ς��邱�ƂŁC�ȉ~����\���\�ł�
	//!
	//! @param[in] radius                 �c ���a�i�P�ʂ͉�f�j
	//! @param[in] resoX                  �c X�������̉𑜓x
	//! @param[in] resoY                  �c Y�������̉𑜓x
	//! @param[in] radiusInPhysicalCoords �c radius�̒P�ʂ���f�̏ꍇ�� false�CresoX��resoY�̒P�ʂ̏ꍇ�� true ���w�肷��i�f�t�H���g�� false�j
	//!
	//! @return �~�\���v�f
	//!
	inline morphology_structure circle( double radius, double resoX, double resoY, bool radiusInPhysicalCoords = false )
	{
		typedef array2< bool >::size_type size_type;
		typedef array2< bool >::difference_type difference_type;

		double ax, ay;
		if( radiusInPhysicalCoords )
		{
			ax = resoX;
			ay = resoY;
		}
		else
		{
			double max_reso = resoX > resoY ? resoX: resoY;

			ax = resoX / max_reso;
			ay = resoY / max_reso;
		}

		double xx, yy, rr = radius * radius;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type x, y;

		size_type ox = rx + 1;
		size_type oy = ry + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;

		morphology_structure s;
		array2< bool > m( w, h );

		// �~�̍\���v�f���쐬����
		for( y = -ry ; y <= ry ; y++ )
		{
			yy = y * y * ay * ay;
			for( x = -rx ; x <= rx ; x++ )
			{
				xx = x * x * ax * ax;
				if( xx + yy <= rr )
				{
					m( x + ox, y + oy ) = true;
				}
			}
		}

		// �~�̍\���v�f�̊e�_�ɐ������Ԃ�ݒ肷��
		for( y = -ry ; y <= ry ; y++ )
		{
			size_t life = 0;
			for( x = -rx ; x <= rx ; x++ )
			{
				if( m( x + ox, y + oy ) )
				{
					s.object.push_back( point( x, y, 0, ++life ) );
				}
				else
				{
					life = 0;
				}
				if( m( x + ox, y + oy ) && !m( x + ox + 1, y + oy ) )
				{
					s.update.push_back( point( x, y, 0, life ) );
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = 0;

		return( s );
	}



	/// @brief �����t�H���W���Z�ɗp���鋅�\���v�f
	//!
	//! resoX�CresoY�CresoZ �̒l��ς��邱�ƂŁC�ȉ~�̓���\���\�ł�
	//!
	//! @param[in] radius �c ���a�i�P�ʂ͉�f�j
	//! @param[in] resoX  �c X�������̉𑜓x
	//! @param[in] resoY  �c Y�������̉𑜓x
	//! @param[in] resoZ  �c Z�������̉𑜓x
	//! @param[in] radiusInPhysicalCoords �c radius�̒P�ʂ���f�̏ꍇ�� false�CresoX��resoY��resoZ�̒P�ʂ̏ꍇ�� true ���w�肷��i�f�t�H���g�� false�j
	//!
	//! @return ���\���v�f
	//!
	inline morphology_structure sphere( double radius, double resoX, double resoY, double resoZ, bool radiusInPhysicalCoords = false )
	{
		typedef array3< bool >::size_type size_type;
		typedef array3< bool >::difference_type difference_type;

		double ax, ay, az;
		if( radiusInPhysicalCoords )
		{
			ax = resoX;
			ay = resoY;
			az = resoZ;
		}
		else
		{
			double max_reso = resoX > resoY ? resoX: resoY;
			max_reso = max_reso > resoZ ? max_reso : resoZ;

			ax = resoX / max_reso;
			ay = resoY / max_reso;
			az = resoZ / max_reso;
		}

		double xx, yy, zz, rr = radius * radius;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type rz = static_cast< size_type >( ceil( radius / az ) );
		difference_type x, y, z;

		size_type ox = rx + 1;
		size_type oy = ry + 1;
		size_type oz = rz + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;
		size_type d = 2 * oz + 1;

		morphology_structure s;
		array3< bool > m( w, h, d );

		// ���̍\���v�f���쐬����
		for( z = -rz ; z <= rz ; z++ )
		{
			zz = z * z * az * az;
			for( y = -ry ; y <= ry ; y++ )
			{
				yy = y * y * ay * ay;
				for( x = -rx ; x <= rx ; x++ )
				{
					xx = x * x * ax * ax;
					if( xx + yy + zz <= rr )
					{
						m( x + ox, y + oy, z + oz ) = true;
					}
				}
			}
		}

		// ���̍\���v�f�̊e�_�ɐ������Ԃ�ݒ肷��
		for( z = -rz ; z <= rz ; z++ )
		{
			for( y = -ry ; y <= ry ; y++ )
			{
				size_t life = 0;
				for( x = -rx ; x <= rx ; x++ )
				{
					if( m( x + ox, y + oy, z + oz ) )
					{
						s.object.push_back( point( x, y, z, ++life ) );
					}
					else
					{
						life = 0;
					}
					if( m( x + ox, y + oy, z + oz ) && !m( x + ox + 1, y + oy, z + oz ) )
					{
						s.update.push_back( point( x, y, z, life ) );
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = rz;

		return( s );
	}



	/// @brief �����t�H���W���Z�ɗp���鐳���`�\���v�f
	//!
	//! resoX�CresoY�CresoZ �̒l��ς��邱�ƂŁC�����`����\���\�ł�
	//!
	//! @param[in] radius �c �����`�̈�ӂ̒����̔����i�P�ʂ͉�f�j
	//! @param[in] resoX  �c X�������̉𑜓x
	//! @param[in] resoY  �c Y�������̉𑜓x
	//! @param[in] radiusInPhysicalCoords �c radius�̒P�ʂ���f�̏ꍇ�� false�CresoX��resoY�̒P�ʂ̏ꍇ�� true ���w�肷��i�f�t�H���g�� false�j
	//!
	//! @return �����`�\���v�f
	//!
	inline morphology_structure square( double radius, double resoX, double resoY, bool radiusInPhysicalCoords = false )
	{
		using namespace std;
		typedef array2< bool >::size_type size_type;
		typedef array2< bool >::difference_type difference_type;

		double ax, ay;
		if( radiusInPhysicalCoords )
		{
			ax = resoX;
			ay = resoY;
		}
		else
		{
			double max_reso = resoX > resoY ? resoX: resoY;

			ax = resoX / max_reso;
			ay = resoY / max_reso;
		}

		double xx, yy;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type x, y;

		size_type ox = rx + 1;
		size_type oy = ry + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;

		morphology_structure s;
		array2< bool > m( w, h );

		// �����`�̍\���v�f���쐬����
		for( y = -ry ; y <= ry ; y++ )
		{
			yy = y * ay;
			for( x = -rx ; x <= rx ; x++ )
			{
				xx = x * ax;
				if( std::abs( xx ) <= radius && std::abs( yy ) <= radius )
				{
					m( x + ox, y + oy ) = true;
				}
			}
		}

		// �����`�̍\���v�f�̊e�_�ɐ������Ԃ�ݒ肷��
		for( y = -ry ; y <= ry ; y++ )
		{
			size_t life = 0;
			for( x = -rx ; x <= rx ; x++ )
			{
				if( m( x + ox, y + oy ) )
				{
					s.object.push_back( point( x, y, 0, ++life ) );
				}
				else
				{
					life = 0;
				}
				if( m( x + ox, y + oy ) && !m( x + ox + 1, y + oy ) )
				{
					s.update.push_back( point( x, y, 0, life ) );
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = 0;

		return( s );
	}


	/// @brief �����t�H���W���Z�ɗp���闧���̍\���v�f
	//!
	//! resoX�CresoY�CresoZ �̒l��ς��邱�ƂŁC�����̓���\���\�ł�
	//!
	//! @param[in] radius �c �����̂̈�ӂ̒����̔����i�P�ʂ͉�f�j
	//! @param[in] resoX  �c X�������̉𑜓x
	//! @param[in] resoY  �c Y�������̉𑜓x
	//! @param[in] resoZ  �c Z�������̉𑜓x
	//! @param[in] radiusInPhysicalCoords �c radius�̒P�ʂ���f�̏ꍇ�� false�CresoX��resoY��resoZ�̒P�ʂ̏ꍇ�� true ���w�肷��i�f�t�H���g�� false�j
	//!
	//! @return �����̍\���v�f
	//!
	inline morphology_structure cube( double radius, double resoX, double resoY, double resoZ, bool radiusInPhysicalCoords = false )
	{
		using namespace std;
		typedef array3< bool >::size_type size_type;
		typedef array3< bool >::difference_type difference_type;

		double ax, ay, az;
		if( radiusInPhysicalCoords )
		{
			ax = resoX;
			ay = resoY;
			az = resoZ;
		}
		else
		{
			double max_reso = resoX > resoY ? resoX: resoY;
			max_reso = max_reso > resoZ ? max_reso : resoZ;

			ax = resoX / max_reso;
			ay = resoY / max_reso;
			az = resoZ / max_reso;
		}

		double xx, yy, zz;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type rz = static_cast< size_type >( ceil( radius / az ) );
		difference_type x, y, z;

		size_type ox = rx + 1;
		size_type oy = ry + 1;
		size_type oz = rz + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;
		size_type d = 2 * oz + 1;

		morphology_structure s;
		array3< bool > m( w, h, d );

		// �����̂̍\���v�f���쐬����
		for( z = -rz ; z <= rz ; z++ )
		{
			zz = z * az;
			for( y = -ry ; y <= ry ; y++ )
			{
				yy = y * ay;
				for( x = -rx ; x <= rx ; x++ )
				{
					xx = x * ax;
					if( std::abs( xx ) <= radius && std::abs( yy ) <= radius && std::abs( zz ) <= radius )
					{
						m( x + ox, y + oy, z + oz ) = true;
					}
				}
			}
		}

		// �����̂̍\���v�f�̊e�_�ɐ������Ԃ�ݒ肷��
		for( z = -rz ; z <= rz ; z++ )
		{
			for( y = -ry ; y <= ry ; y++ )
			{
				size_t life = 0;
				for( x = -rx ; x <= rx ; x++ )
				{
					if( m( x + ox, y + oy, z + oz ) )
					{
						s.object.push_back( point( x, y, z, ++life ) );
					}
					else
					{
						life = 0;
					}
					if( m( x + ox, y + oy, z + oz ) && !m( x + ox + 1, y + oy, z + oz ) )
					{
						s.update.push_back( point( x, y, z, life ) );
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = rz;

		return( s );
	}



	/// @brief �C�ӂ̉摜����C�����t�H���W���Z�p�̍\���v�f���쐬����
	//!
	//! @attention resoX�CresoY�CresoZ �̒l�͖��������
	//!
	//! @param[in] in �c �\���v�f�̉摜
	//! @param[in] cx �c �\���v�f�̒��S��\��X���W�l
	//! @param[in] cy �c �\���v�f�̒��S��\��Y���W�l
	//! @param[in] cz �c �\���v�f�̒��S��\��Z���W�l
	//!
	//! @return �����t�H���W���Z�p�̍\���v�f
	//!
	template < class Array >
	inline morphology_structure create_morphology_structure( const Array &in, typename Array::size_type cx, typename Array::size_type cy = 0, typename Array::size_type cz = 0 )
	{
		using namespace std;
		typedef typename Array::size_type size_type;
		typedef typename Array::difference_type difference_type;
		difference_type x, y, z;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		morphology_structure s;
		marray< Array > m( in, 1 );

		// �\���v�f�̌`���쐬����
		for( z = 0 ; z < d ; z++ )
		{
			for( y = 0 ; y < h ; y++ )
			{
				for( x = 0 ; x < w ; x++ )
				{
					m( x, y, z ) = in( x, y, z ) == 0 ? false : true;
				}
			}
		}

		// �\���v�f�̊e�_�ɐ������Ԃ�ݒ肷��
		for( z = 0 ; z < d ; z++ )
		{
			for( y = 0 ; y < h ; y++ )
			{
				size_t life = 0;
				for( x = 0 ; x < w ; x++ )
				{
					if( m( x, y, z ) )
					{
						s.object.push_back( point( x - cx, y - cy, z - cz, ++life ) );
					}
					else
					{
						life = 0;
					}
					if( m( x, y, z ) && !m( x + 1, y, z ) )
					{
						s.update.push_back( point( x - cx, y - cy, z - cz, life ) );
					}
				}
			}
		}

		s.margin_x = cx > w - cx - 1 ? cx : w - cx;
		s.margin_y = cy > h - cy - 1 ? cy : h - cy;
		s.margin_z = cz > d - cz - 1 ? cz : d - cz;

		return( s );
	}


	/// @brief �e�摜�ɂ����āC�\���v�f�̒��S�Ɗe�v�f�Ԃ̃|�C���^�̍����v�Z���C���̃��X�g���쐬����
	//!
	//! @attention MIST�̓����֐��Ƃ��ė��p
	//!
	//! @param[in] in   �c ���͉摜
	//! @param[in] list �c �\���v�f�̍��W���X�g
	//!
	//! @return �\���v�f�̒��S�Ɗe�v�f�Ԃ̃|�C���^�̍��̃��X�g
	//!
	template < class Array >
	inline std::vector< pointer_diff > create_pointer_diff_list( const Array &in, const std::vector< point > &list )
	{
		typedef typename Array::size_type     size_type;
		typedef typename Array::const_pointer const_pointer;
		size_type cx = in.width( ) / 2;
		size_type cy = in.height( ) / 2;
		size_type cz = in.depth( ) / 2;
		const_pointer p = &( in( cx, cy, cz ) );

		std::vector< pointer_diff > out;

		for( size_type i = 0 ; i < list.size( ) ; i++ )
		{
			const point &pt = list[ i ];
			const_pointer pp = &( in( cx + pt.x, cy + pt.y, cz + pt.z ) );
			out.push_back( pointer_diff( pp - p, pt.life ) );
		}

		return( out );
	}
}


/// @}
//  �����t�H���W���Z�O���[�v�̏I���


namespace __erosion__
{
	template < class Array1, class Array2, class Functor >
	void erosion( const Array1 &in, Array2 &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f )
	{
		typedef typename Array1::size_type       size_type;
		typedef typename Array1::value_type      value_type;
		typedef typename Array1::const_pointer   const_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type      out_value_type;
		typedef morphology::pointer_diff         pointer_diff;
		typedef std::vector< pointer_diff >      list_type;

		list_type::const_iterator ite;

		size_type  i, j, k;
		value_type min;
		size_type life;
		const_pointer p;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		const bool bprogress1 = thread_idy == 0 && d == 1;
		const bool bprogress2 = thread_idz == 0 && d > 1;

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				p = &( in( 0, j, k ) );
				ite = object.begin( );
				min = p[ ite->diff ];
				life = ite->life;
				++ite;
				for( ; ite != object.end( ) ; ++ite )
				{
					if( min > p[ ite->diff ] || ( min == p[ ite->diff ] && life < ite->life ) )
					{
						min = p[ ite->diff ];
						life = ite->life;
					}
				}

				out( 0, j, k ) = static_cast< out_value_type >( min );
				life--;

				for( i = 1 ; i < w ; i++ )
				{
					p = &( in( i, j, k ) );

					if( life == 0 )
					{
						// �O��܂ł̍ŏ��l���\���v�f����o�Ă��܂����̂ŁC�V���ɑO�T�����s��
						ite = object.begin( );
						min = p[ ite->diff ];
						life = ite->life;
						++ite;
						for( ; ite != object.end( ) ; ++ite )
						{
							if( min > p[ ite->diff ] || ( min == p[ ite->diff ] && life < ite->life ) )
							{
								min = p[ ite->diff ];
								life = ite->life;
							}
						}
					}
					else
					{
						// �O��܂ł̍ŏ��l���\���v�f���ɑ��݂���̂ŁC�X�V���݂̂���T�����s��
						for( ite = update.begin( ) ; ite != update.end( ) ; ++ite )
						{
							if( min > p[ ite->diff ] || ( min == p[ ite->diff ] && life < ite->life ) )
							{
								min = p[ ite->diff ];
								life = ite->life;
							}
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( min );
					life--;
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 );
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 );
			}
		}
	}
}


namespace __dilation__
{
	template < class Array1, class Array2, class Functor >
	void dilation( const Array1 &in, Array2 &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f )
	{
		typedef typename Array1::size_type       size_type;
		typedef typename Array1::value_type      value_type;
		typedef typename Array1::const_pointer   const_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type      out_value_type;
		typedef morphology::pointer_diff         pointer_diff;
		typedef std::vector< pointer_diff >      list_type;

		list_type::const_iterator ite;

		size_type  i, j, k;
		value_type max;
		size_type life;
		const_pointer p;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		const bool bprogress1 = thread_idy == 0 && d == 1;
		const bool bprogress2 = thread_idz == 0 && d > 1;

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				p = &( in( 0, j, k ) );
				ite = object.begin( );
				max = p[ ite->diff ];
				life = ite->life;
				++ite;
				for( ; ite != object.end( ) ; ++ite )
				{
					if( max < p[ ite->diff ] || ( max == p[ ite->diff ] && life < ite->life ) )
					{
						max = p[ ite->diff ];
						life = ite->life;
					}
				}

				out( 0, j, k ) = static_cast< out_value_type >( max );
				life--;

				for( i = 1 ; i < w ; i++ )
				{
					p = &( in( i, j, k ) );

					if( life == 0 )
					{
						// �O��܂ł̍ŏ��l���\���v�f����o�Ă��܂����̂ŁC�V���ɑO�T�����s��
						ite = object.begin( );
						max = p[ ite->diff ];
						life = ite->life;
						++ite;
						for( ; ite != object.end( ) ; ++ite )
						{
							if( max < p[ ite->diff ] || ( max == p[ ite->diff ] && life < ite->life ) )
							{
								max = p[ ite->diff ];
								life = ite->life;
							}
						}
					}
					else
					{
						// �O��܂ł̍ŏ��l���\���v�f���ɑ��݂���̂ŁC�X�V���݂̂���T�����s��
						for( ite = update.begin( ) ; ite != update.end( ) ; ++ite )
						{
							if( max < p[ ite->diff ] || ( max == p[ ite->diff ] && life < ite->life ) )
							{
								max = p[ ite->diff ];
								life = ite->life;
							}
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( max );
					life--;
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 );
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 );
			}
		}
	}
}


// �����t�H���W���Z�̃X���b�h����
namespace __morphology_controller__
{
	// Erosion ���Z
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void erosion( const marray< array< T1, Allocator1 > > &in, array< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__erosion__::erosion( in, out, object, update, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void erosion( const marray< array1< T1, Allocator1 > > &in, array1< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array1< T1, Allocator1 >::size_type thread_id, typename array1< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__erosion__::erosion( in, out, object, update, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void erosion( const marray< array2< T1, Allocator1 > > &in, array2< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__erosion__::erosion( in, out, object, update, thread_id, thread_num, 0, 1, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void erosion( const marray< array3< T1, Allocator1 > > &in, array3< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__erosion__::erosion( in, out, object, update, 0, 1, thread_id, thread_num, f );
	}


	// Dilation ���Z
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void dilation( const marray< array< T1, Allocator1 > > &in, array< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__dilation__::dilation( in, out, object, update, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void dilation( const marray< array1< T1, Allocator1 > > &in, array1< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array1< T1, Allocator1 >::size_type thread_id, typename array1< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__dilation__::dilation( in, out, object, update, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void dilation( const marray< array2< T1, Allocator1 > > &in, array2< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__dilation__::dilation( in, out, object, update, thread_id, thread_num, 0, 1, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void dilation( const marray< array3< T1, Allocator1 > > &in, array3< T2, Allocator2 > &out,
						const std::vector< morphology::pointer_diff > &object, const std::vector< morphology::pointer_diff > &update,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__dilation__::dilation( in, out, object, update, 0, 1, thread_id, thread_num, f );
	}


	template < class T1, class T2, class Functor >
	class morphology_thread : public mist::thread< morphology_thread< T1, T2, Functor > >
	{
	public:
		typedef mist::thread< morphology_thread< T1, T2, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;
		typedef morphology::pointer_diff pointer_diff;
		typedef std::vector< pointer_diff >  list_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		const T1 *in_;
		T2 *out_;
		list_type *object_;
		list_type *update_;
		bool is_erosion_;

		Functor f_;

	public:
		void setup_parameters( const T1 &in, T2 &out, list_type &object, list_type &update, bool is_erosion, size_type thread_id, size_type thread_num, Functor f )
		{
			in_  = &in;
			out_ = &out;
			object_ = &object;
			update_ = &update;
			is_erosion_ = is_erosion;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
			f_ = f;
		}

		const morphology_thread& operator =( const morphology_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				out_ = p.out_;
				is_erosion_ = p.is_erosion_;
				object_ = p.object_;
				update_ = p.update_;
				f_ = p.f_;
			}
			return( *this );
		}

		morphology_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), object_( NULL ), update_( NULL ), is_erosion_( true )
		{
		}
		morphology_thread( const morphology_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), object_( p.object_ ), update_( p.update_ ), is_erosion_( p.is_erosion_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			if( is_erosion_ )
			{
				erosion( *in_, *out_, *object_, *update_, thread_id_, thread_num_, f_ );
			}
			else
			{
				dilation( *in_, *out_, *object_, *update_, thread_id_, thread_num_, f_ );
			}
			return( true );
		}
	};
}


//! @addtogroup morphology_group �����t�H���W���Z�t�B���^
//!  @{


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Erosion���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
bool erosion( Array &in, const morphology::morphology_structure &s, Functor f, typename Array::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename Array::value_type value_type;
	typedef typename Array::size_type  size_type;
	typedef __morphology_controller__::morphology_thread< marray< Array >, Array, Functor > morphology_thread;
	typedef morphology::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	value_type max = type_limits< value_type >::maximum( );

	marray< Array > out( in, s.margin_x, s.margin_y, s.margin_z, max );

	list_type object = morphology::create_pointer_diff_list( out, s.object );
	list_type update = morphology::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update, true, i, thread_num, f );
	}

	f( 0.0 );

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Erosion���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
bool erosion( Array &in, const morphology::morphology_structure &s, typename Array::size_type thread_num = 0 )
{
	return( erosion( in, s, __mist_dmy_callback__( ), thread_num ) );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Dilation���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
bool dilation( Array &in, const morphology::morphology_structure &s, Functor f, typename Array::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename Array::value_type value_type;
	typedef typename Array::size_type  size_type;
	typedef __morphology_controller__::morphology_thread< marray< Array >, Array, Functor > morphology_thread;
	typedef morphology::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	value_type min = type_limits< value_type >::minimum( );

	marray< Array > out( in, s.margin_x, s.margin_y, s.margin_z, min );

	list_type object = morphology::create_pointer_diff_list( out, s.object );
	list_type update = morphology::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update, false, i, thread_num, f );
	}

	f( 0.0 );

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Dilation���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
bool dilation( Array &in, const morphology::morphology_structure &s, typename Array::size_type thread_num = 0 )
{
	return( dilation( in, s, __mist_dmy_callback__( ), thread_num ) );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Opening���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
bool opening( Array &in, const morphology::morphology_structure &s, Functor f, typename Array::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename Array::value_type value_type;
	typedef typename Array::size_type  size_type;
	typedef __mist_convert_callback__< Functor > CallBack;
	typedef __morphology_controller__::morphology_thread< marray< Array >, Array, CallBack > morphology_thread;
	typedef morphology::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	value_type max = type_limits< value_type >::maximum( );
	value_type min = type_limits< value_type >::minimum( );

	marray< Array > out( in, s.margin_x, s.margin_y, s.margin_z, max );

	list_type object = morphology::create_pointer_diff_list( out, s.object );
	list_type update = morphology::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	f( 0.0 );

	size_type i;
	{
		// Erosion ���Z
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, true, i, thread_num, CallBack( f, 0, 50 ) );
		}

		// �X���b�h�����s���āC�I���܂őҋ@����
		do_threads_( thread, thread_num );
	}

	out = in;
	out.fill_margin( min );

	{
		// Dilation ���Z
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, false, i, thread_num, CallBack( f, 50, 100 ) );
		}

		// �X���b�h�����s���āC�I���܂őҋ@����
		do_threads_( thread, thread_num );
	}

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Opening���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
bool opening( Array &in, const morphology::morphology_structure &s, typename Array::size_type thread_num = 0 )
{
	return( opening( in, s, __mist_dmy_callback__( ), thread_num ) );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Closing���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
bool closing( Array &in, const morphology::morphology_structure &s, Functor f, typename Array::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename Array::value_type value_type;
	typedef typename Array::size_type  size_type;
	typedef __mist_convert_callback__< Functor > CallBack;
	typedef __morphology_controller__::morphology_thread< marray< Array >, Array, CallBack > morphology_thread;
	typedef morphology::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	value_type max = type_limits< value_type >::maximum( );
	value_type min = type_limits< value_type >::minimum( );

	marray< Array > out( in, s.margin_x, s.margin_y, s.margin_z, min );

	list_type object = morphology::create_pointer_diff_list( out, s.object );
	list_type update = morphology::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	f( 0.0 );

	size_type i;
	{
		// Dilation ���Z
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, false, i, thread_num, CallBack( f, 0, 50 ) );
		}

		// �X���b�h�����s���āC�I���܂őҋ@����
		do_threads_( thread, thread_num );
	}

	out = in;
	out.fill_margin( max );

	{
		// Erosion ���Z
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, true, i, thread_num, CallBack( f, 50, 100 ) );
		}

		// �X���b�h�����s���āC�I���܂őҋ@����
		do_threads_( thread, thread_num );
	}

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ�����Closing���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �����t�H���W���Z�ɗp����\���v�f
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
bool closing( Array &in, const morphology::morphology_structure &s, typename Array::size_type thread_num = 0 )
{
	return( closing( in, s, __mist_dmy_callback__( ), thread_num ) );
}



/// @brief �����\���v�f�Ƃ���Erosion���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
inline bool erosion( Array &in, double radius, Functor f, typename Array::size_type thread_num )
{
	if( in.depth( ) == 1 )
	{
		return( erosion( in, morphology::circle( radius, in.reso1( ), in.reso2( ) ), f, thread_num ) );
	}
	else
	{
		return( erosion( in, morphology::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), f, thread_num ) );
	}
}


/// @brief �����\���v�f�Ƃ���Dilation���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
inline bool dilation( Array &in, double radius, Functor f, typename Array::size_type thread_num )
{
	if( in.depth( ) == 1 )
	{
		return( dilation( in, morphology::circle( radius, in.reso1( ), in.reso2( ) ), f, thread_num ) );
	}
	else
	{
		return( dilation( in, morphology::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), f, thread_num ) );
	}
}


/// @brief �����\���v�f�Ƃ���Opening���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
inline bool opening( Array &in, double radius, Functor f, typename Array::size_type thread_num )
{
	if( in.depth( ) == 1 )
	{
		return( opening( in, morphology::circle( radius, in.reso1( ), in.reso2( ) ), f, thread_num ) );
	}
	else
	{
		return( opening( in, morphology::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), f, thread_num ) );
	}
}


/// @brief �����\���v�f�Ƃ���Closing���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] thread_num �c �g�p����X���b�h��
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array, class Functor >
inline bool closing( Array &in, double radius, Functor f, typename Array::size_type thread_num )
{
	if( in.depth( ) == 1 )
	{
		return( closing( in, morphology::circle( radius, in.reso1( ), in.reso2( ) ), f, thread_num ) );
	}
	else
	{
		return( closing( in, morphology::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), f, thread_num ) );
	}
}



/// @brief �����\���v�f�Ƃ���Erosion���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
inline bool erosion( Array &in, double radius, typename Array::size_type thread_num = 0 )
{
	return( erosion( in, radius, __mist_dmy_callback__( ), thread_num ) );
}


/// @brief �����\���v�f�Ƃ���Dilation���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
inline bool dilation( Array &in, double radius, typename Array::size_type thread_num = 0 )
{
	return( dilation( in, radius, __mist_dmy_callback__( ), thread_num ) );
}


/// @brief �����\���v�f�Ƃ���Opening���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
inline bool opening( Array &in, double radius, typename Array::size_type thread_num = 0 )
{
	return( opening( in, radius, __mist_dmy_callback__( ), thread_num ) );
}


/// @brief �����\���v�f�Ƃ���Closing���Z
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c ���̔��a�i�P�ʂ͉�f�j
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class Array >
inline bool closing( Array &in, double radius, typename Array::size_type thread_num = 0 )
{
	return( closing( in, radius, __mist_dmy_callback__( ), thread_num ) );
}


/// @}
//  �����t�H���W���Z�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_MORPHOLOGY__
