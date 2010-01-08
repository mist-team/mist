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

/// @file mist/filter/mode.h
//!
//! @brief �e�����̉摜�ɑΉ����C�C�ӂ̍\���v�f�ɑΉ������Q�l�摜�ɑ΂���ŕp�l�t�B���^�i�������t�B���^�j���s�����߂̃��C�u����
//!
//! @attention �\���v�f�̌`�͓ʌ`��łȂ��Ă͂Ȃ�Ȃ�
//!

#ifndef __INCLUDE_MIST_MODE__
#define __INCLUDE_MIST_MODE__


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



//! @addtogroup mode_group �ŕp�l�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/mode.h>
//! @endcode
//!
//!  @{


/// @brief �ŕp�l�t�B���^�ɗ��p����\���v�f�֘A
//! 
//! �ŕp�l�t�B���^�ɗ��p����\���v�f���쐬���邽�߂̊֐��E�\���v�f���`���Ă���
//! 
namespace mode_filter
{
	/// @brief �\���v�f�̊e�v�f�̍��W��\���\����
	struct point
	{
		typedef ptrdiff_t difference_type;	///< @brief �����t�����^
		difference_type x;					///< @brief X�������̍��W�l
		difference_type y;					///< @brief Y�������̍��W�l
		difference_type z;					///< @brief Z�������̍��W�l

		/// @brief �R���X�g���N�^
		//! 
		//! @param[in] xx �c X���W�l
		//! @param[in] yy �c Y���W�l
		//! @param[in] zz �c Z���W�l
		//! 
		point( difference_type xx, difference_type yy, difference_type zz ) : x( xx ), y( yy ), z( zz )
		{
		}
	};


	/// @brief �ŕp�l�t�B���^�ɗ��p����\���v�f
	//! 
	//! �{�\���̂��C�ŕp�l�t�B���^�֐��ɓn���D
	//! �{�f�[�^�\���ɂ���āC�C�ӂ̓ʌ`��̍\���v�f�̍ŕp�l�t�B���^�����s�\�D
	//! 
	struct mode_structure
	{
		typedef std::vector< point > list_type;		///< @brief �\���v�f���̍��W�l���i�[���郊�X�g�^
		list_type object;							///< @brief �\���v�f���̑S�v�f�̍��W�l���i�[���郊�X�g
		list_type update_in;						///< @brief �\���v�f��X��������1��f�ړ��������ɁC�V�K�ɍ\���v�f�Ɋ܂܂��v�f�̍��W�l���i�[���郊�X�g
		list_type update_out;						///< @brief �\���v�f��X��������1��f�ړ��������ɁC�V�K�ɍ\���v�f�Ɋ܂܂��v�f�̍��W�l���i�[���郊�X�g
		size_t margin_x;							///< @brief X�������ō\���v�f�̒��S����[�܂ł̋����ő傫�����̋���
		size_t margin_y;							///< @brief Y�������ō\���v�f�̒��S����[�܂ł̋����ő傫�����̋���
		size_t margin_z;							///< @brief Z�������ō\���v�f�̒��S����[�܂ł̋����ő傫�����̋���
	};


	/// @brief �ŕp�l�t�B���^�ɗp����~�\���v�f
	//!
	//! resoX�CresoY �̒l��ς��邱�ƂŁC�ȉ~����\���\�ł�
	//!
	//! @param[in] radius �c ���a�i�P�ʂ͉�f�j
	//! @param[in] resoX  �c X�������̉𑜓x
	//! @param[in] resoY  �c Y�������̉𑜓x
	//!
	//! @return �~�\���v�f
	//!
	inline mode_structure circle( double radius, double resoX, double resoY )
	{
		typedef array2< bool >::size_type size_type;
		typedef array2< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double xx, yy, rr = radius * radius;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type x, y;

		size_type ox = rx + 1;
		size_type oy = ry + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;

		mode_structure s;
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

		// �~�̍\���v�f�̊e�_��ݒ肷��
		for( y = -ry ; y <= ry ; y++ )
		{
			for( x = -rx ; x <= rx ; x++ )
			{
				if( m( x + ox, y + oy ) )
				{
					s.object.push_back( point( x, y, 0 ) );
					if( !m( x + ox + 1, y + oy ) )
					{
						s.update_in.push_back( point( x, y, 0 ) );
					}
					if( !m( x + ox - 1, y + oy ) )
					{
						s.update_out.push_back( point( x - 1, y, 0 ) );
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = 0;

		return( s );
	}



	/// @brief �ŕp�l�t�B���^�ɗp���鋅�\���v�f
	//!
	//! resoX�CresoY�CresoZ �̒l��ς��邱�ƂŁC�ȉ~�̓���\���\�ł�
	//!
	//! @param[in] radius �c ���a�i�P�ʂ͉�f�j
	//! @param[in] resoX  �c X�������̉𑜓x
	//! @param[in] resoY  �c Y�������̉𑜓x
	//! @param[in] resoZ  �c Z�������̉𑜓x
	//!
	//! @return ���\���v�f
	//!
	inline mode_structure sphere( double radius, double resoX, double resoY, double resoZ )
	{
		typedef array3< bool >::size_type size_type;
		typedef array3< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;
		max_reso = max_reso > resoZ ? max_reso : resoZ;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double az = resoZ / max_reso;
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

		mode_structure s;
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

		// ���̍\���v�f�̊e�_��ݒ肷��
		for( z = -rz ; z <= rz ; z++ )
		{
			for( y = -ry ; y <= ry ; y++ )
			{
				for( x = -rx ; x <= rx ; x++ )
				{
					if( m( x + ox, y + oy, z + oz ) )
					{
						s.object.push_back( point( x, y, z ) );
						if( !m( x + ox + 1, y + oy, z + oz ) )
						{
							s.update_in.push_back( point( x, y, z ) );
						}
						if( !m( x + ox - 1, y + oy, z + oz ) )
						{
							s.update_out.push_back( point( x - 1, y, z ) );
						}
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = rz;

		return( s );
	}



	/// @brief �ŕp�l�t�B���^�ɗp���鐳���`�\���v�f
	//!
	//! resoX�CresoY�CresoZ �̒l��ς��邱�ƂŁC�����`����\���\�ł�
	//!
	//! @param[in] radius �c �����`�̈�ӂ̒����̔����i�P�ʂ͉�f�j
	//! @param[in] resoX  �c X�������̉𑜓x
	//! @param[in] resoY  �c Y�������̉𑜓x
	//!
	//! @return �����`�\���v�f
	//!
	inline mode_structure square( double radius, double resoX, double resoY )
	{
		using namespace std;
		typedef array2< bool >::size_type size_type;
		typedef array2< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double xx, yy;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type x, y;

		size_type ox = rx + 1;
		size_type oy = ry + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;

		mode_structure s;
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

		// �����`�̍\���v�f�̊e�_��ݒ肷��
		for( y = -ry ; y <= ry ; y++ )
		{
			for( x = -rx ; x <= rx ; x++ )
			{
				if( m( x + ox, y + oy ) )
				{
					s.object.push_back( point( x, y, 0 ) );
					if( !m( x + ox + 1, y + oy ) )
					{
						s.update_in.push_back( point( x, y, 0 ) );
					}
					if( !m( x + ox - 1, y + oy ) )
					{
						s.update_out.push_back( point( x - 1, y, 0 ) );
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = 0;

		return( s );
	}


	/// @brief �ŕp�l�t�B���^�ɗp���闧���̍\���v�f
	//!
	//! resoX�CresoY�CresoZ �̒l��ς��邱�ƂŁC�����̓���\���\�ł�
	//!
	//! @param[in] radius �c �����̂̈�ӂ̒����̔����i�P�ʂ͉�f�j
	//! @param[in] resoX  �c X�������̉𑜓x
	//! @param[in] resoY  �c Y�������̉𑜓x
	//! @param[in] resoZ  �c Z�������̉𑜓x
	//!
	//! @return �����̍\���v�f
	//!
	inline mode_structure cube( double radius, double resoX, double resoY, double resoZ )
	{
		using namespace std;
		typedef array3< bool >::size_type size_type;
		typedef array3< bool >::difference_type difference_type;

		double max_reso = resoX > resoY ? resoX: resoY;
		max_reso = max_reso > resoZ ? max_reso : resoZ;

		double ax = resoX / max_reso;
		double ay = resoY / max_reso;
		double az = resoZ / max_reso;
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

		mode_structure s;
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

		// �����̂̍\���v�f�̊e�_��ݒ肷��
		for( z = -rz ; z <= rz ; z++ )
		{
			for( y = -ry ; y <= ry ; y++ )
			{
				for( x = -rx ; x <= rx ; x++ )
				{
					if( m( x + ox, y + oy, z + oz ) )
					{
						s.object.push_back( point( x, y, z ) );
						if( !m( x + ox + 1, y + oy, z + oz ) )
						{
							s.update_in.push_back( point( x, y, z ) );
						}
						if( !m( x + ox - 1, y + oy, z + oz ) )
						{
							s.update_out.push_back( point( x - 1, y, z ) );
						}
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = rz;

		return( s );
	}



	/// @brief �C�ӂ̉摜����C�ŕp�l�t�B���^�p�̍\���v�f���쐬����
	//!
	//! @attention resoX�CresoY�CresoZ �̒l�͖��������
	//!
	//! @param[in] in �c �\���v�f�̉摜
	//! @param[in] cx �c �\���v�f�̒��S��\��X���W�l
	//! @param[in] cy �c �\���v�f�̒��S��\��Y���W�l
	//! @param[in] cz �c �\���v�f�̒��S��\��Z���W�l
	//!
	//! @return �ŕp�l�t�B���^�p�̍\���v�f
	//!
	template < class Array >
	inline mode_structure create_mode_structure( const Array &in, typename Array::size_type cx, typename Array::size_type cy = 0, typename Array::size_type cz = 0 )
	{
		using namespace std;
		typedef typename Array::size_type size_type;
		typedef typename Array::difference_type difference_type;
		difference_type x, y, z;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		mode_structure s;
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

		// �\���v�f�̊e�_��ݒ肷��
		for( z = 0 ; z < d ; z++ )
		{
			for( y = 0 ; y < h ; y++ )
			{
				size_t life = 0;
				for( x = 0 ; x < w ; x++ )
				{
					if( m( x, y, z ) )
					{
						s.object.push_back( point( x - cx, y - cy, z - cz ) );
						if( !m( x + 1, y, z ) )
						{
							s.update_in.push_back( point( x - cx, y - cy, z - cz ) );
						}
						if( !m( x - 1, y, z ) )
						{
							s.update_out.push_back( point( x - cx - 1, y - cy, z - cz ) );
						}
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
	inline std::vector< ptrdiff_t > create_pointer_diff_list( const Array &in, const std::vector< point > &list )
	{
		typedef typename Array::size_type     size_type;
		typedef typename Array::const_pointer const_pointer;
		size_type cx = in.width( ) / 2;
		size_type cy = in.height( ) / 2;
		size_type cz = in.depth( ) / 2;
		const_pointer p = &( in( cx, cy, cz ) );

		std::vector< ptrdiff_t > out;

		for( size_type i = 0 ; i < list.size( ) ; i++ )
		{
			const point &pt = list[ i ];
			const_pointer pp = &in( cx + pt.x, cy + pt.y, cz + pt.z );
			out.push_back( pp - p );
		}

		return( out );
	}
}


/// @}
//  �ŕp�l�t�B���^�O���[�v�̏I���


namespace __mode__
{
	template < class Array1, class Array2, class Functor >
	void mode( const Array1 &in, Array2 &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f )
	{
		typedef typename Array1::size_type       size_type;
		typedef typename Array1::value_type      value_type;
		typedef typename Array1::const_pointer   const_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type      out_value_type;
		typedef typename Array2::value_type      out_value_type;
		typedef std::vector< ptrdiff_t >		 list_type;
		typedef list_type::const_iterator        const_iterator;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		const bool bprogress1 = thread_idy == 0 && d == 1;
		const bool bprogress2 = thread_idz == 0 && d > 1;

		for( size_type k = thread_idz ; k < d ; k += thread_numz )
		{
			for( size_type j = thread_idy ; j < h ; j += thread_numy )
			{
				const_pointer p = &in( 0, j, k );
				difference_type count[ 2 ] = { 0, 0 };
				for( const_iterator ite = object.begin( ) ; ite != object.end( ) ; ++ite )
				{
					count[ p[ *ite ] > 0 ]++;
				}

				out( 0, j, k ) = static_cast< out_value_type >( count[ 0 ] > count[ 1 ] ? 0 : 1 );

				for( size_type i = 1 ; i < w ; i++ )
				{
					p = &in( i, j, k );

					// �\���v�f���甲�������������Z����
					for( const_iterator ite = update_out.begin( ) ; ite != update_out.end( ) ; ++ite )
					{
						count[ p[ *ite ] > 0 ]--;
					}

					// �\���v�f�ɐV��������������������
					for( const_iterator ite = update_in.begin( ) ; ite != update_in.end( ) ; ++ite )
					{
						count[ p[ *ite ] > 0 ]++;
					}

					out( i, j, k ) = static_cast< out_value_type >( count[ 0 ] > count[ 1 ] ? 0 : 1 );
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




// �ŕp�l�t�B���^�̃X���b�h����
namespace __mode_controller__
{
	// �e���������b�s���O���邽�߂̊֐�
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array< T1, Allocator1 > > &in, array< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array1< T1, Allocator1 > > &in, array1< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array1< T1, Allocator1 >::size_type thread_id, typename array1< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array2< T1, Allocator1 > > &in, array2< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, thread_id, thread_num, 0, 1, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	void mode( const marray< array3< T1, Allocator1 > > &in, array3< T2, Allocator2 > &out,
						const std::vector< ptrdiff_t > &object, const std::vector< ptrdiff_t > &update_in, const std::vector< ptrdiff_t > &update_out,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		__mode__::mode( in, out, object, update_in, update_out, 0, 1, thread_id, thread_num, f );
	}


	template < class T1, class T2, class Functor >
	class mode_thread : public mist::thread< mode_thread< T1, T2, Functor > >
	{
	public:
		typedef mist::thread< mode_thread< T1, T2, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;
		typedef typename T1::difference_type difference_type;
		typedef std::vector< difference_type >  list_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// ���o�͗p�̉摜�ւ̃|�C���^
		const T1 *in_;
		T2 *out_;
		list_type *object_;
		list_type *update_in_;
		list_type *update_out_;

		Functor f_;

	public:
		void setup_parameters( const T1 &in, T2 &out, list_type &object, list_type &update_in, list_type &update_out, size_type thread_id, size_type thread_num, Functor f )
		{
			in_  = &in;
			out_ = &out;
			object_ = &object;
			update_in_ = &update_in;
			update_out_ = &update_out;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
			f_ = f;
		}

		const mode_thread& operator =( const mode_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				out_ = p.out_;
				object_ = p.object_;
				update_in_ = p.update_in_;
				update_out_ = p.update_out_;
				f_ = p.f_;
			}
			return( *this );
		}

		mode_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), object_( NULL ), update_in_( NULL ),  update_out_( NULL )
		{
		}
		mode_thread( const mode_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
												in_( p.in_ ), out_( p.out_ ), object_( p.object_ ), update_in_( p.update_in_ ), update_out_( p.update_out_ )
		{
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			mode( *in_, *out_, *object_, *update_in_, *update_out_, thread_id_, thread_num_, f_ );
			return( true );
		}
	};
}


//! @addtogroup mode_group �ŕp�l�t�B���^
//!  @{


/// @brief �C�ӂ̍\���v�f�ɑΉ������ŕp�l�t�B���^
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �ŕp�l�t�B���^�ɗp����\���v�f
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class T, class Allocator, class Functor >
bool mode( array2< T, Allocator > &in, const mode_filter::mode_structure &s, Functor f, typename array2< T, Allocator >::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename array2< T, Allocator >::value_type value_type;
	typedef typename array2< T, Allocator >::size_type  size_type;
	typedef typename array2< T, Allocator >::difference_type  difference_type;
	typedef __mode_controller__::mode_thread< marray< array2< T, Allocator > >, array2< T, Allocator >, Functor > mode_thread;
	typedef std::vector< difference_type >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	marray< array2< T, Allocator > > out( in, s.margin_x, s.margin_y, 0 );

	list_type object     = mode_filter::create_pointer_diff_list( out, s.object );
	list_type update_in  = mode_filter::create_pointer_diff_list( out, s.update_in );
	list_type update_out = mode_filter::create_pointer_diff_list( out, s.update_out );

	mode_thread *thread = new mode_thread[ thread_num ];

	for( size_type i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update_in, update_out, i, thread_num, f );
	}

	f( 0.0 );

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ������ŕp�l�t�B���^
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �ŕp�l�t�B���^�ɗp����\���v�f
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class T, class Allocator >
bool mode( array2< T, Allocator > &in, const mode_filter::mode_structure &s, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, s, __mist_dmy_callback__( ), thread_num ) );
}



/// @brief �~���\���v�f�Ƃ���ŕp�l�t�B���^
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c �~�̔��a�i�P�ʂ͉�f�j
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class T, class Allocator, class Functor >
inline bool mode( array2< T, Allocator > &in, double radius, Functor f, typename array2< T, Allocator >::size_type thread_num )
{
	return( mode( in, mode_filter::circle( radius, in.reso1( ), in.reso2( ) ), f, thread_num ) );
}


/// @brief �~���\���v�f�Ƃ���ŕp�l�t�B���^
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] radius     �c �~�̔��a�i�P�ʂ͉�f�j
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class T, class Allocator >
inline bool mode( array2< T, Allocator > &in, double radius, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, mode_filter::circle( radius, in.reso1( ), in.reso2( ) ), __mist_dmy_callback__( ), thread_num ) );
}







/// @brief �C�ӂ̍\���v�f�ɑΉ������ŕp�l�t�B���^
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �ŕp�l�t�B���^�ɗp����\���v�f
//! @param[in] f          �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class T, class Allocator, class Functor >
bool mode( array3< T, Allocator > &in, const mode_filter::mode_structure &s, Functor f, typename array3< T, Allocator >::size_type thread_num )
{
	if( in.empty( ) )
	{
		return( false );
	}

	typedef typename array3< T, Allocator >::value_type       value_type;
	typedef typename array3< T, Allocator >::size_type        size_type;
	typedef typename array3< T, Allocator >::difference_type  difference_type;
	typedef __mode_controller__::mode_thread< marray< array3< T, Allocator > >, array3< T, Allocator >, Functor > mode_thread;
	typedef std::vector< difference_type >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	marray< array3< T, Allocator > > out( in, s.margin_x, s.margin_y, s.margin_z, 0 );

	list_type object     = mode_filter::create_pointer_diff_list( out, s.object );
	list_type update_in  = mode_filter::create_pointer_diff_list( out, s.update_in );
	list_type update_out = mode_filter::create_pointer_diff_list( out, s.update_out );

	mode_thread *thread = new mode_thread[ thread_num ];

	for( size_type i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update_in, update_out, i, thread_num, f );
	}

	f( 0.0 );

	// �X���b�h�����s���āC�I���܂őҋ@����
	do_threads_( thread, thread_num );

	f( 100.1 );

	delete [] thread;

	return( true );
}


/// @brief �C�ӂ̍\���v�f�ɑΉ������ŕp�l�t�B���^
//! 
//! @attention ���͂Əo�͂́C����MIST�R���e�i�I�u�W�F�N�g���g�p����
//! @attention CPU����0���w�肵���ꍇ�́C�g�p�\��CPU���������I�Ɏ擾����
//! 
//! @param[in,out] in     �c ���o�͉摜
//! @param[in] s          �c �ŕp�l�t�B���^�ɗp����\���v�f
//! @param[in] thread_num �c �g�p����X���b�h��
//! 
//! @retval true  �c �t�B���^�����O�ɐ���
//! @retval false �c ���͉摜����̏ꍇ
//! 
template < class T, class Allocator >
bool mode( array3< T, Allocator > &in, const mode_filter::mode_structure &s, typename array3< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, s, __mist_dmy_callback__( ), thread_num ) );
}




/// @brief �����\���v�f�Ƃ���ŕp�l�t�B���^
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
template < class T, class Allocator, class Functor >
inline bool mode( array3< T, Allocator > &in, double radius, Functor f, typename array3< T, Allocator >::size_type thread_num )
{
	return( mode( in, mode_filter::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), f, thread_num ) );
}




/// @brief �����\���v�f�Ƃ���ŕp�l�t�B���^
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
template < class T, class Allocator >
inline bool mode( array3< T, Allocator > &in, double radius, typename array3< T, Allocator >::size_type thread_num = 0 )
{
	return( mode( in, mode_filter::sphere( radius, in.reso1( ), in.reso2( ), in.reso3( ) ), __mist_dmy_callback__( ), thread_num ) );
}



/// @}
//  �ŕp�l�t�B���^�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_MODE__
