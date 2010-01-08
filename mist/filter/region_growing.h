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

/// @file mist/filter/region_growing.h
//!
//! @brief �e�����̉摜�ɑΉ����C�C�ӂ̈ʒu����C�ӂ̍\���v�f�ƔC�ӂ̏��������p���ė̈�g���@��K�p���邽�߂̃��C�u����
//!

#ifndef __INCLUDE_MIST_REGIONGROWING__
#define __INCLUDE_MIST_REGIONGROWING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "../vector.h"
#endif

#include <deque>
#include <vector>
#include <cmath>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @addtogroup region_growing_group �̈�g���@
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/region_growing.h>
//! @endcode
//!
//! @code �̈�g���@�̎g�p��
//! typedef mist::region_growing_utility::point_type    point_type;     // �̈�g���̊J�n�_���w�肷��^�imist::vector2 �� mist::vector3 �ő�p�j
//! typedef mist::region_growing_utility::circle        component_type; // �̈�g���ɗp����\���v�f
//! typedef mist::region_growing_utility::less< short > condition_type; // �̈�g���̂̊g������
//!
//! mist::region_growing(
//!                       in,                    // ���͉摜
//!                       out,                   // �̈�g�����s�������ʂ��i�[����摜	
//!                       point_type( x, y ),    // �g���J�n�_�i���X�g�ɂ��邱�Ƃŕ����w��\�j
//!                       128,                   // �̈�g���̌��ʂɑ������l
//!                       component_type( 20 ),  // �̈�g���ɗp����\���v�f�i��f�C�~�C���Ȃǁj
//!                       condition_type( 128 )  // �̈�g���̂̊g�������i�����C�ȏ�C�͈͂Ȃǁj
//!                     );
//! @endcode
//!
//!  @{


/// @brief �̈�g���@�ɗ��p����\���v�f�֘A
//! 
//! �̈�g���@�ɗ��p����\���v�f�̒�`
//! 
namespace region_growing_utility
{
	/// @brief ���̒��ړ_�̒�ߕ�
	enum expand_mode_type
	{
		NC4,	///< @brief �S�ߖT�i�Q�����j
		NC8,	///< @brief �W�ߖT�i�Q�����j
		NC6,	///< @brief �U�ߖT�i�R�����j
		NC18,	///< @brief �P�W�ߖT�i�R�����j
		NC26,	///< @brief �Q�U�ߖT�i�R�����j
		ALL		///< @brief �\���v�f���̂��ׂĂ̓_
	};


	/// @brief �̈�g���@�ŗp����Q������f�̍\���v�f
	struct pixel
	{
		typedef size_t		size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

		size_type width( ) const { return( 1 ); }	///< @brief �\���v�f�̕���Ԃ�
		size_type height( ) const { return( 1 ); }	///< @brief �\���v�f�̍�����Ԃ�
		size_type depth( ) const { return( 1 ); }	///< @brief �\���v�f�̐[����Ԃ�

		expand_mode_type __expand_mode__;

		/// @brief �C�ӂ̈ʒu���\���v�f�����ǂ����𔻒肷��֐�
		bool operator ()( difference_type /* i */, difference_type /* j */, difference_type /* k */ ) const { return( true ); }

		/// @brief ���̒��ړ_�̒�ߕ�
		expand_mode_type expand_mode( ) const { return( __expand_mode__ ); }

		/// @brief NC4 �ߖT�^�̃{�N�Z��
		pixel( ) : __expand_mode__( NC4 ){ }

		/// @brief �w�肵���ߖT�̃{�N�Z���Ƃ��ď���������
		pixel( expand_mode_type mode ) : __expand_mode__( mode ){ }
	};


	/// @brief �̈�g���@�ŗp����R������f�̍\���v�f
	struct voxel
	{
		typedef size_t		size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

		size_type width( ) const { return( 1 ); }	///< @brief �\���v�f�̕���Ԃ�
		size_type height( ) const { return( 1 ); }	///< @brief �\���v�f�̍�����Ԃ�
		size_type depth( ) const { return( 1 ); }	///< @brief �\���v�f�̐[����Ԃ�

		expand_mode_type __expand_mode__;

		/// @brief �C�ӂ̈ʒu���\���v�f�����ǂ����𔻒肷��֐�
		bool operator ()( difference_type /* i */, difference_type /* j */, difference_type /* k */ ) const { return( true ); }

		/// @brief ���̒��ړ_�̒�ߕ�
		expand_mode_type expand_mode( ) const { return( __expand_mode__ ); }

		/// @brief NC6 �ߖT�^�̃{�N�Z��
		voxel( ) : __expand_mode__( NC6 ){ }

		/// @brief �w�肵���ߖT�̃{�N�Z���Ƃ��ď���������
		voxel( expand_mode_type mode ) : __expand_mode__( mode ){ }
	};


	/// @brief �̈�g���@�ŗp����~�̍\���v�f
	class circle
	{
	public:
		typedef size_t		size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

	private:
		double radius_;
		double resoX_;
		double resoY_;
		size_type width_;
		size_type height_;

	public:
		size_type width( ) const { return( width_ ); }		///< @brief �\���v�f�̕���Ԃ�
		size_type height( ) const { return( height_ ); }	///< @brief �\���v�f�̍�����Ԃ�
		size_type depth( ) const { return( 1 ); }			///< @brief �\���v�f�̐[����Ԃ�

		/// @brief �C�ӂ̈ʒu���\���v�f�����ǂ����𔻒肷��֐�
		bool operator ()( difference_type i, difference_type j, difference_type k ) const
		{
			if( k != 0 )
			{
				return( false );
			}
			else
			{
				return( i * i * resoX_ * resoX_ + j * j * resoY_ * resoY_ <= radius_ * radius_ );
			}
		}

		/// @brief ���̒��ړ_�̒�ߕ�
		expand_mode_type expand_mode( ) const { return( NC4 ); }

		/// @brief ���a�Ɖ𑜓x���w�肵�ċ����������i�ȉ~�̍쐬���\�C�������C�P�ʂ͉�f�j
		circle( double radius, double resoX = 1.0, double resoY = 1.0, bool radiusInPhysicalCoords = false ) : radius_( radius )
		{
			if( radiusInPhysicalCoords )
			{
				resoX_ = resoX;
				resoY_ = resoY;
			}
			else
			{
				double max_reso = resoX > resoY ? resoX: resoY;

				resoX_ = resoX / max_reso;
				resoY_ = resoY / max_reso;
			}
			width_  = static_cast< size_type >( std::ceil( radius_ / resoX_ ) ) * 2 + 1;
			height_ = static_cast< size_type >( std::ceil( radius_ / resoY_ ) ) * 2 + 1;
		}
	};


	/// @brief �̈�g���@�ŗp���鋅�̍\���v�f
	class sphere
	{
	public:
		typedef size_t		size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

	private:
		double radius_;		///< @brief ���a
		double resoX_;		///< @brief X�������̉𑜓x
		double resoY_;		///< @brief Y�������̉𑜓x
		double resoZ_;		///< @brief Z�������̉𑜓x
		size_type width_;
		size_type height_;
		size_type depth_;

	public:
		size_type width( ) const { return( width_ ); }		///< @brief �\���v�f�̕���Ԃ�
		size_type height( ) const { return( height_ ); }	///< @brief �\���v�f�̍�����Ԃ�
		size_type depth( ) const { return( depth_ ); }		///< @brief �\���v�f�̐[����Ԃ�

		/// @brief �C�ӂ̈ʒu���\���v�f�����ǂ����𔻒肷��֐�
		bool operator ()( difference_type i, difference_type j, difference_type k ) const
		{
			return( i * i * resoX_ * resoX_ + j * j * resoY_ * resoY_+ k * k * resoZ_ * resoZ_ <= radius_ * radius_ );
		}

		/// @brief ���̒��ړ_�̒�ߕ�
		expand_mode_type expand_mode( ) const { return( NC6 ); }

		/// @brief ���a�Ɖ𑜓x���w�肵�ċ����������i�ȉ~�̂̍쐬���\�C�������C�P�ʂ͉�f�j
		sphere( double radius, double resoX = 1.0, double resoY = 1.0, double resoZ = 1.0, bool radiusInPhysicalCoords = false ) : radius_( radius )
		{
			if (radiusInPhysicalCoords)
			{
				resoX_ = resoX;
				resoY_ = resoY;
				resoZ_ = resoZ;
			}
			else
			{
				double max_reso = resoX > resoY ? resoX: resoY;
				max_reso = max_reso > resoZ ? max_reso : resoZ;

				resoX_ = resoX / max_reso;
				resoY_ = resoY / max_reso;
				resoZ_ = resoZ / max_reso;
			}
			width_  = static_cast< size_type >( std::ceil( radius_ / resoX_ ) ) * 2 + 1;
			height_ = static_cast< size_type >( std::ceil( radius_ / resoY_ ) ) * 2 + 1;
			depth_  = static_cast< size_type >( std::ceil( radius_ / resoZ_ ) ) * 2 + 1;
		}
	};


	/// @brief �̈�g���@�ŗp����g������
	//!
	//! �������胊�X�g���̑S�Ẳ�f������l����
	//!
	template < class T >
	class less
	{
	private:
		typedef size_t		size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
		typedef T			value_type;			///< @brief ��������ɗp����f�[�^�^

	protected:
		value_type th_;		///< @brief �������l

	public:
		/// @brief �������胊�X�g���̉�f�������𖞂������ǂ����𒲂ׂ�
		template < class VerifyList >
		bool operator()( const VerifyList &elements, size_type num ) const
		{
			typedef typename VerifyList::value_type verify_value_type;

			for( size_type i = 0 ; i < num ; i++ )
			{
				const verify_value_type &v = elements[ i ];
				if( !( v < th_ ) )
				{
					return ( false );
				}
			}

			return ( true );
		}

		/// @brief ����������s���ۂɁC�\���v�f���̑S��f���K�v�ȏꍇ�� true ��Ԃ�
		bool require_all_elements( ) const { return( false ); }

		/// @brief �C�ӂ̂������l�ŏ�����
		less( value_type threshold ) : th_( threshold ){ }
	};


	/// @brief �̈�g���@�ŗp����g������
	//!
	//! �������胊�X�g���̑S�Ẳ�f������l���傫��
	//!
	template < class T >
	class greater
	{
	private:
		typedef size_t		size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
		typedef T			value_type;			///< @brief ��������ɗp����f�[�^�^

	protected:
		value_type th_;		///< @brief �������l

	public:
		/// @brief �������胊�X�g���̉�f�������𖞂������ǂ����𒲂ׂ�
		template < class VerifyList >
		bool operator()( const VerifyList &elements, size_type num ) const
		{
			typedef typename VerifyList::value_type verify_value_type;

			for( size_type i = 0 ; i < num ; i++ )
			{
				const verify_value_type &v = elements[ i ];
				if( v < th_ )
				{
					return ( false );
				}
			}

			return ( true );
		}

		/// @brief ����������s���ۂɁC�\���v�f���̑S��f���K�v�ȏꍇ�� true ��Ԃ�
		bool require_all_elements( ) const { return( false ); }

		/// @brief �C�ӂ̂������l�ŏ�����
		greater( value_type threshold ) : th_( threshold ){ }
	};


	/// @brief �̈�g���@�ŗp����g������
	//!
	//! �������胊�X�g���̑S�Ẳ�f������l�Ɠ�����
	//!
	template < class T >
	class equal
	{
	private:
		typedef size_t		size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
		typedef T			value_type;			///< @brief ��������ɗp����f�[�^�^

	protected:
		value_type th_;		///< @brief �������l

	public:
		/// @brief �������胊�X�g���̉�f�������𖞂������ǂ����𒲂ׂ�
		template < class VerifyList >
		bool operator()( const VerifyList &elements, size_type num ) const
		{
			typedef typename VerifyList::value_type verify_value_type;

			for( size_type i = 0 ; i < num ; i++ )
			{
				const verify_value_type &v = elements[ i ];
				if( v < th_ || th_ < v )
				{
					return ( false );
				}
			}

			return ( true );
		}

		/// @brief ����������s���ۂɁC�\���v�f���̑S��f���K�v�ȏꍇ�� true ��Ԃ�
		bool require_all_elements( ) const { return( false ); }

		/// @brief �C�ӂ̂������l�ŏ�����
		equal( value_type threshold ) : th_( threshold ){ }
	};


	/// @brief �̈�g���@�ŗp����g������
	//!
	//! �������胊�X�g���̑S�Ẳ�f������͈͓��ɑ��݂���
	//!
	template < class T >
	class range
	{
	private:
		typedef size_t		size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
		typedef ptrdiff_t	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
		typedef T			value_type;			///< @brief ��������ɗp����f�[�^�^

	protected:
		value_type min_;	///< @brief �͈͂̉���
		value_type max_;	///< @brief �͈͂̏��

	public:
		/// @brief �������胊�X�g���̉�f�������𖞂������ǂ����𒲂ׂ�
		template < class VerifyList >
		bool operator()( const VerifyList &elements, size_type num ) const
		{
			typedef typename VerifyList::value_type verify_value_type;

			for( size_type i = 0 ; i < num ; i++ )
			{
				const verify_value_type &v = elements[ i ];
				if( v < min_ || max_ < v )
				{
					return ( false );
				}
			}

			return ( true );
		}

		/// @brief ����������s���ۂɁC�\���v�f���̑S��f���K�v�ȏꍇ�� true ��Ԃ�
		bool require_all_elements( ) const { return( false ); }

		/// @brief �C�ӂ̉����Ə���Ŕ͈͂�������
		range( value_type min, value_type max ) : min_( min ), max_( max ){ }
	};
}


// �̈�g���@�̓����ŗ��p����^�Ȃ�
namespace __region_growing_utility__
{
	struct pointer_diff2
	{
		typedef ptrdiff_t	difference_type;

		ptrdiff_t diff1;
		ptrdiff_t diff2;

		pointer_diff2( ptrdiff_t d1 = 0, ptrdiff_t d2 = 0 ) : diff1( d1 ), diff2( d2 ){ }
	};

	typedef vector3< ptrdiff_t >			point_type;			///< @brief �̈�g���@�ŗ��p������W��\���^
	typedef std::vector< point_type >		point_list_type;	///< @brief �̈�g���@�ŗ��p������W�̃��X�g�^
	typedef std::vector< pointer_diff2 >	ptrdiff_list_type;


	struct no_mask
	{
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		bool empty( ) const { return( true ); }
		bool operator ()( difference_type /* i */, difference_type /* j */, difference_type /* k */ ) const { return( false ); }
	};


	template < class Array1, class Array2, class Component >
	ptrdiff_list_type create_component_list( const Array1 &in1, const Array2 &in2, const Component &components )
	{
		typedef typename Array1::const_pointer				const_pointer1;
		typedef typename Array2::const_pointer				const_pointer2;
		typedef typename ptrdiff_list_type::size_type		size_type;
		typedef typename ptrdiff_list_type::difference_type	difference_type;
		typedef typename ptrdiff_list_type::value_type		value_type;

		ptrdiff_list_type list;

		difference_type w = components.width( );
		difference_type h = components.height( );
		difference_type d = components.depth( );
		difference_type cx = w / 2;
		difference_type cy = h / 2;
		difference_type cz = d / 2;
		difference_type ox1 = in1.width( ) / 2;
		difference_type oy1 = in1.height( ) / 2;
		difference_type oz1 = in1.depth( ) / 2;
		difference_type ox2 = in2.width( ) / 2;
		difference_type oy2 = in2.height( ) / 2;
		difference_type oz2 = in2.depth( ) / 2;

		const_pointer1 p1 = &in1( ox1, oy1, oz1 );
		const_pointer2 p2 = &in2( ox2, oy2, oz2 );

		for( difference_type k = 0 ; k < d ; k++ )
		{
			for( difference_type j = 0 ; j < h ; j++ )
			{
				for( difference_type i = 0 ; i < w ; i++ )
				{
					if( components( i - cx, j - cy, k - cz ) )
					{
						ptrdiff_t d1 = &in1( ox1 + i - cx, oy1 + j - cy, oz1 + k - cz ) - p1;
						ptrdiff_t d2 = &in2( ox2 + i - cx, oy2 + j - cy, oz2 + k - cz ) - p2;
						list.push_back( pointer_diff2( d1, d2 ) );
					}
				}
			}
		}

		return( list );
	}


	template < class Array1, class Array2, class Component >
	ptrdiff_list_type create_update_list( const Array1 &in1, const Array2 &in2, const Component &components )
	{
		typedef typename Array1::const_pointer				const_pointer1;
		typedef typename Array2::const_pointer				const_pointer2;
		typedef typename ptrdiff_list_type::size_type		size_type;
		typedef typename ptrdiff_list_type::difference_type	difference_type;
		typedef typename ptrdiff_list_type::value_type		value_type;

		ptrdiff_list_type list;

		difference_type ox1 = in1.width( ) / 2;
		difference_type oy1 = in1.height( ) / 2;
		difference_type oz1 = in1.depth( ) / 2;
		difference_type ox2 = in2.width( ) / 2;
		difference_type oy2 = in2.height( ) / 2;
		difference_type oz2 = in2.depth( ) / 2;

		const_pointer1 p1 = &in1( ox1, oy1, oz1 );
		const_pointer2 p2 = &in2( ox2, oy2, oz2 );

		switch( components.expand_mode( ) )
		{
		case region_growing_utility::NC4:
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1, oz1 ) - p1, &in2( ox2 - 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1, oz1 ) - p1, &in2( ox2 + 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 - 1, oz1 ) - p1, &in2( ox2, oy2 - 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 + 1, oz1 ) - p1, &in2( ox2, oy2 + 1, oz2 ) - p2 ) );
			break;

		case region_growing_utility::NC8:
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 - 1, oz1 ) - p1, &in2( ox2 - 1, oy2 - 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 - 1, oz1 ) - p1, &in2( ox2    , oy2 - 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 - 1, oz1 ) - p1, &in2( ox2 + 1, oy2 - 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1    , oz1 ) - p1, &in2( ox2 - 1, oy2    , oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1    , oz1 ) - p1, &in2( ox2 + 1, oy2    , oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 + 1, oz1 ) - p1, &in2( ox2 - 1, oy2 + 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 + 1, oz1 ) - p1, &in2( ox2    , oy2 + 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 + 1, oz1 ) - p1, &in2( ox2 + 1, oy2 + 1, oz2 ) - p2 ) );
			break;

		case region_growing_utility::NC6:
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1, oz1 ) - p1, &in2( ox2 - 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1, oz1 ) - p1, &in2( ox2 + 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 - 1, oz1 ) - p1, &in2( ox2, oy2 - 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 + 1, oz1 ) - p1, &in2( ox2, oy2 + 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1, oz1 - 1 ) - p1, &in2( ox2, oy2, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1, oz1 + 1 ) - p1, &in2( ox2, oy2, oz2 + 1 ) - p2 ) );
			break;

		case region_growing_utility::NC18:
			list.push_back( pointer_diff2( &in1( ox1    , oy1 - 1, oz1 - 1 ) - p1, &in2( ox2    , oy2 - 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1    , oz1 - 1 ) - p1, &in2( ox2 - 1, oy2    , oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1    , oz1 - 1 ) - p1, &in2( ox2    , oy2    , oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1    , oz1 - 1 ) - p1, &in2( ox2 + 1, oy2    , oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 + 1, oz1 - 1 ) - p1, &in2( ox2    , oy2 + 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 - 1, oz1     ) - p1, &in2( ox2 - 1, oy2 - 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 - 1, oz1     ) - p1, &in2( ox2    , oy2 - 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 - 1, oz1     ) - p1, &in2( ox2 + 1, oy2 - 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1    , oz1     ) - p1, &in2( ox2 - 1, oy2    , oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1    , oz1     ) - p1, &in2( ox2 + 1, oy2    , oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 + 1, oz1     ) - p1, &in2( ox2 - 1, oy2 + 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 + 1, oz1     ) - p1, &in2( ox2    , oy2 + 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 + 1, oz1     ) - p1, &in2( ox2 + 1, oy2 + 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 - 1, oz1 + 1 ) - p1, &in2( ox2    , oy2 - 1, oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1    , oz1 + 1 ) - p1, &in2( ox2 - 1, oy2    , oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1    , oz1 + 1 ) - p1, &in2( ox2    , oy2    , oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1    , oz1 + 1 ) - p1, &in2( ox2 + 1, oy2    , oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 + 1, oz1 + 1 ) - p1, &in2( ox2    , oy2 + 1, oz2 + 1 ) - p2 ) );
			break;

		case region_growing_utility::NC26:
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 - 1, oz1 - 1 ) - p1, &in2( ox2 - 1, oy2 - 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 - 1, oz1 - 1 ) - p1, &in2( ox2    , oy2 - 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 - 1, oz1 - 1 ) - p1, &in2( ox2 + 1, oy2 - 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1    , oz1 - 1 ) - p1, &in2( ox2 - 1, oy2    , oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1    , oz1 - 1 ) - p1, &in2( ox2    , oy2    , oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1    , oz1 - 1 ) - p1, &in2( ox2 + 1, oy2    , oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 + 1, oz1 - 1 ) - p1, &in2( ox2 - 1, oy2 + 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 + 1, oz1 - 1 ) - p1, &in2( ox2    , oy2 + 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 + 1, oz1 - 1 ) - p1, &in2( ox2 + 1, oy2 + 1, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 - 1, oz1     ) - p1, &in2( ox2 - 1, oy2 - 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 - 1, oz1     ) - p1, &in2( ox2    , oy2 - 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 - 1, oz1     ) - p1, &in2( ox2 + 1, oy2 - 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1    , oz1     ) - p1, &in2( ox2 - 1, oy2    , oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1    , oz1     ) - p1, &in2( ox2 + 1, oy2    , oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 + 1, oz1     ) - p1, &in2( ox2 - 1, oy2 + 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 + 1, oz1     ) - p1, &in2( ox2    , oy2 + 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 + 1, oz1     ) - p1, &in2( ox2 + 1, oy2 + 1, oz2     ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 - 1, oz1 + 1 ) - p1, &in2( ox2 - 1, oy2 - 1, oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 - 1, oz1 + 1 ) - p1, &in2( ox2    , oy2 - 1, oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 - 1, oz1 + 1 ) - p1, &in2( ox2 + 1, oy2 - 1, oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1    , oz1 + 1 ) - p1, &in2( ox2 - 1, oy2    , oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1    , oz1 + 1 ) - p1, &in2( ox2    , oy2    , oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1    , oz1 + 1 ) - p1, &in2( ox2 + 1, oy2    , oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1 + 1, oz1 + 1 ) - p1, &in2( ox2 - 1, oy2 + 1, oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1    , oy1 + 1, oz1 + 1 ) - p1, &in2( ox2    , oy2 + 1, oz2 + 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1 + 1, oz1 + 1 ) - p1, &in2( ox2 + 1, oy2 + 1, oz2 + 1 ) - p2 ) );
			break;

		case region_growing_utility::ALL:
		default:
			list = create_component_list( in1, in2, components );
			break;
		}

		return( list );
	}


	template < class T, class Allocator, class Array, class PointType >
	bool check_component_list( const array2< T, Allocator > &in1, const Array &in2, const PointType &pt, pointer_diff2 &ptr )
	{
		typedef typename array2< T, Allocator >::const_pointer	const_pointer1;
		typedef typename Array::const_pointer					const_pointer2;
		typedef typename ptrdiff_list_type::difference_type		difference_type;

		difference_type x = static_cast< difference_type >( pt.x );
		difference_type y = static_cast< difference_type >( pt.y );
		difference_type w = in1.width( );
		difference_type h = in1.height( );

		if( x < 0 || x >= w || y < 0 || y >= h )
		{
			return( false );
		}
		else if( in2( x, y ) != 0 )
		{
			return( false );
		}
		else
		{
			ptr.diff1 = &in1( x, y) - &in1[ 0 ];
			ptr.diff2 = &in2( x, y ) - &in2[ 0 ];
			return( true );
		}
	}


	template < class T, class Allocator, class Array, class PointType >
	bool check_component_list( const array3< T, Allocator > &in1, const Array &in2, const PointType &pt, pointer_diff2 &ptr )
	{
		typedef typename array3< T, Allocator >::const_pointer	const_pointer1;
		typedef typename Array::const_pointer					const_pointer2;
		typedef typename ptrdiff_list_type::difference_type		difference_type;

		difference_type x = static_cast< difference_type >( pt.x );
		difference_type y = static_cast< difference_type >( pt.y );
		difference_type z = static_cast< difference_type >( pt.z );
		difference_type w = in1.width( );
		difference_type h = in1.height( );
		difference_type d = in1.depth( );

		if( x < 0 || x >= w || y < 0 || y >= h || z < 0 || z >= d )
		{
			return( false );
		}
		else if( in2( x, y, z ) != 0 )
		{
			return( false );
		}
		else
		{
			ptr.diff1 = &in1( x, y, z ) - &in1[ 0 ];
			ptr.diff2 = &in2( x, y, z ) - &in2[ 0 ];
			return( true );
		}
	}


	template < class PointType >
	struct point_list_converter
	{
		typedef std::vector< PointType > point_list_type;

		static point_list_type create_point_list( const PointType &pt )
		{
			point_list_type list( 1 );
			list[ 0 ] = pt;
			return( list );
		}
	};


	template < class PointType >
	struct point_list_converter< std::vector< PointType > >
	{
		typedef std::vector< PointType > point_list_type;

		static point_list_type create_point_list( const point_list_type &list )
		{
			return( list );
		}
	};

	template < class PointType >
	struct point_list_converter< std::deque< PointType > >
	{
		typedef std::deque< PointType > point_list_type;

		static point_list_type create_point_list( const point_list_type &list )
		{
			return( list );
		}
	};


	template < class T >
	inline const T maximum( const T &v0, const typename type_trait< T >::value_type &v1 )
	{
		return( v0 > v1 ? v0 : v1 );
	}

	template < class T >
	inline const T maximum( const T &v0, const typename type_trait< T >::value_type &v1, const typename type_trait< T >::value_type &v2 )
	{
		return( v0 > v1 ? ( v0 > v2 ? v0 : v2 ) : ( v1 > v2 ? v1 : v2 ) );
	}

	template < class T >
	inline const T maximum( const T &v0, const typename type_trait< T >::value_type &v1, const typename type_trait< T >::value_type &v2, const typename type_trait< T >::value_type &v3 )
	{
		return( maximum( maximum( v0, v1 ), maximum( v2, v3 ) ) );
	}
}


/// @brief �C�ӂ̍\���v�f�Ə�����p���ė̈�g�����s���֐�
//!
//! @attention �����̓s����Cmax_paint �Ŏw�肵����f�����኱���߂ɗ̈悪�g�������\��������܂��D
//!
//! @param[in]  in           �c ���͉摜
//! @param[out] out          �c �o�̓}�[�N�f�[�^
//! @param[in]  mask         �c �����ΏۊO�}�X�N
//! @param[in]  start_points �c �̈�g���̊J�n�_�̃��X�g�i�����w�肷��ꍇ�́Cstd::vector�Ȃǂ̃��X�g�ɑ�����邱�Ɓj
//! @param[in]  output_value �c �o�̓}�[�N�f�[�^�ɏ������ޒl
//! @param[in]  components   �c �̈�g���ɗp����\���v�f
//! @param[in]  condition    �c �\���v�f���̉�f���������ׂ�����
//! @param[in]  max_paint    �c �ő�œh��Ԃ���f��
//!
//! @return ���͉摜���s�K�؂ȏꍇ��C�ő唽���񐔂����s���Ă��I�������𖞂����Ȃ������ꍇ�� -1 ��Ԃ��D����ȊO�̏ꍇ�́C�h��Ԃ�����f����Ԃ��D
//! 
template < class Array1, class Array2, class MaskType, class PointList, class Component, class Condition >
typename Array1::difference_type region_growing( const Array1 &in, Array2 &out, const MaskType &mask, const PointList &start_points, typename Array2::value_type output_value,
														const Component &components, const Condition &condition, typename Array1::size_type max_paint )
{
	if( in.empty( ) || is_same_object( in, out ) )
	{
		return( -1 );
	}

	typedef typename Array1::template rebind< unsigned char >::other mask_type;
	typedef __region_growing_utility__::pointer_diff2 pointer_diff_type;

	typedef typename Array1::size_type			size_type;
	typedef typename Array1::value_type			value_type;
	typedef typename Array1::difference_type	difference_type;
	typedef typename Array2::value_type			out_value_type;

	typedef typename Array1::const_pointer		const_pointer;
	typedef typename mask_type::pointer			work_pointer;
	typedef typename Array2::pointer			output_pointer;

	size_type rx = components.width( ) / 2;
	size_type ry = components.height( ) / 2;
	size_type rz = components.depth( ) / 2;

	// ���ړ_���͈͊O�ɏo�Ȃ����Ƃ��Ď�����}�X�N
	// �Œ�ł��O���ɂP��f�̈��S�̈���쐬����
	marray< mask_type > work( in, __region_growing_utility__::maximum( rx, ry, rz, 1 ) );

	__region_growing_utility__::ptrdiff_list_type clist = __region_growing_utility__::create_component_list( in, work, components );	// �\���v�f���̉�f�̃��X�g
	__region_growing_utility__::ptrdiff_list_type ulist = __region_growing_utility__::create_update_list( in, work, components );		// ���钍�ړ_�̎��ɒ��ړ_�ɂȂ��f�̃��X�g

	std::deque< pointer_diff_type > que;									// ���ړ_��f�̃��X�g
	std::vector< value_type > element( clist.size( ) );						// �g�������̔���ɗp�����f�̔z��
	__region_growing_utility__::ptrdiff_list_type elist( clist.size( ) );	// �g�������̔���ɗp�����f�ւ̃|�C���^���o����z��

	// �摜�̊O���Ɋg�����Ȃ��悤�Ƀ}�X�N��ݒ肷��
	work.fill( );
	work.fill_margin( 255 );

	// �}�X�N�̓��e�𒍖ړ_�̃f�[�^�ɔ��f������
	if( !mask.empty( ) )
	{
		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( mask( i, j, k ) )
					{
						work( i, j, k ) = 255;
					}
				}
			}
		}
	}

	typedef __region_growing_utility__::point_list_converter< PointList > start_point_list_converter;
	typedef typename start_point_list_converter::point_list_type start_point_list_type;
	start_point_list_type sps = start_point_list_converter::create_point_list( start_points );

	// �g���J�n�_���摜���ɑ��݂��Ă��邩�ǂ������`�F�b�N����
	for( typename start_point_list_type::const_iterator ite = sps.begin( ) ; ite != sps.end( ) ; ++ite )
	{
		__region_growing_utility__::pointer_diff2 ptr;
		if( __region_growing_utility__::check_component_list( in, work, *ite, ptr ) )
		{
			que.push_back( ptr );
		}
	}

	// �o�͉摜�̑傫�����`�F�b�N����
	if( in.size( ) != out.size( ) )
	{
		out.resize( in.width( ), in.height( ), in.depth( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
	}

	
	size_type num_painted = 0;	// �h��Ԃ�����f���̃J�E���^


	// �̈�g���̏����ɂ���āC�\���v�f���̑S�v�f�𔻒�ɗp���邩�ǂ��������߂�
	// require_all_elements �� false �̏ꍇ�ɂ́C���łɔ���ς݂̉�f��2�x�ڂ̔�����s��Ȃ�
	if( condition.require_all_elements( ) )
	{
		// �\���v�f���̂��ׂĂ̓_��p���ĕ]�����s��
		while( !que.empty( ) )
		{
			if( num_painted >= max_paint )
			{
				return( num_painted );
			}

			// ���X�g�̐擪��f�����o��
			pointer_diff_type cur = que.front( );
			que.pop_front( );

			const_pointer  pi = &in[ 0 ] + cur.diff1;
			work_pointer   pw = &work[ 0 ] + cur.diff2;
			output_pointer po = &out[ 0 ] + cur.diff1;

			// �ȍ~�̏����Œ��ړ_�ƂȂ�Ȃ��悤�ɁC���݂̒��ړ_���}�X�N����
			pw[ 0 ] |= 0xf0;

			// �g����������ɗp�����f��񋓂���
			// �����ŁC�}�X�N����Ă���̈�͔͈͂Ɋ܂߂Ȃ��i�摜�O�Ȃǁj
			size_type num = 0;
			for( size_type i = 0 ; i < clist.size( ) ; i++ )
			{
				const pointer_diff_type &d = clist[ i ];
				if( ( pw[ d.diff2 ] & 0x0f ) == 0 )
				{
					element[ num ] = pi[ d.diff1 ];
					elist[ num ]   = d;
					num++;
				}
			}

			// �g�������̔�����s��
			if( num != 0 && condition( element, num ) )
			{
				// �����𖞂������\���v�f���̉�f���ׂĂ�h��Ԃ�
				for( size_type i = 0 ; i < num ; i++ )
				{
					const pointer_diff_type &d = elist[ i ];
					num_painted += po[ d.diff1 ] != output_value ? 1 : 0;
					po[ d.diff1 ] = output_value;
				}

				// �\���v�f�ɂ���Ď��̒��ړ_�����肵�C���X�g�̍Ō�ɒǉ�����
				for( size_type i = 0 ; i < ulist.size( ) ; i++ )
				{
					const pointer_diff_type &d = ulist[ i ];
					if( ( pw[ d.diff2 ] & 0xf0 ) == 0 )
					{
						pw[ d.diff2 ] |= 0xf0;
						que.push_back( pointer_diff_type( cur.diff1 + d.diff1, cur.diff2 + d.diff2 ) );
					}
				}
			}
		}
	}
	else
	{
		// ���łɔ��肳�ꂽ��f�ɂ��Ă͕]�����s��Ȃ�
		while( !que.empty( ) )
		{
			if( num_painted >= max_paint )
			{
				return( num_painted );
			}

			// ���X�g�̐擪��f�����o��
			pointer_diff_type cur = que.front( );
			que.pop_front( );

			const_pointer  pi = &in[ 0 ] + cur.diff1;
			work_pointer   pw = &work[ 0 ] + cur.diff2;
			output_pointer po = &out[ 0 ] + cur.diff1;

			// �ȍ~�̏����Œ��ړ_�ƂȂ�Ȃ��悤�ɁC���݂̒��ړ_���}�X�N����
			pw[ 0 ] |= 0xf0;

			// �g����������ɗp�����f��񋓂���
			// �����ŁC�}�X�N����Ă���̈�͔͈͂Ɋ܂߂Ȃ��i�摜�O�Ȃǁj
			// �܂��C���łɔ���ς݂̉�f�����O����
			// �����f�ƍX�V�����̃��X�g���쐬����
			size_type num = 0;
			for( size_type i = 0 ; i < clist.size( ) ; i++ )
			{
				const pointer_diff_type &d = clist[ i ];
				if( ( pw[ d.diff2 ] & 0x0f ) == 0 )
				{
					element[ num ] = pi[ d.diff1 ];
					elist[ num ]   = d;
					num++;
				}
			}

			// �g�������̔�����s��
			if( condition( element, num ) )
			{
				// �����𖞂������\���v�f���̉�f���ׂĂ�h��Ԃ�
				for( size_type i = 0 ; i < num ; i++ )
				{
					const pointer_diff_type &d = elist[ i ];
					num_painted += po[ d.diff1 ] != output_value ? 1 : 0;
					pw[ d.diff2 ] |= 1;
					po[ d.diff1 ] = output_value;
				}

				// �\���v�f�ɂ���Ď��̒��ړ_�����肵�C���X�g�̍Ō�ɒǉ�����
				for( size_type i = 0 ; i < ulist.size( ) ; i++ )
				{
					const pointer_diff_type &d = ulist[ i ];
					if( ( pw[ d.diff2 ] & 0xf0 ) == 0 )
					{
						pw[ d.diff2 ] |= 0xf0;
						que.push_back( pointer_diff_type( cur.diff1 + d.diff1, cur.diff2 + d.diff2 ) );
					}
				}
			}
		}
	}

	return( num_painted );
}



/// @brief �C�ӂ̍\���v�f�Ə�����p���ė̈�g�����s���֐�
//!
//! �����ΏۊO�}�X�N��ݒ肵�Ȃ��̈�g���@
//!
//! @attention �����̓s����Cmax_paint �Ŏw�肵����f�����኱���߂ɗ̈悪�g�������\��������܂��D
//!
//! @param[in]  in           �c ���͉摜
//! @param[out] out          �c �o�̓}�[�N�f�[�^
//! @param[in]  start_points �c �̈�g���̊J�n�_�̃��X�g�i�����w�肷��ꍇ�́Cstd::vector�Ȃǂ̃��X�g�ɑ�����邱�Ɓj
//! @param[in]  output_value �c �o�̓}�[�N�f�[�^�ɏ������ޒl
//! @param[in]  components   �c �̈�g���ɗp����\���v�f
//! @param[in]  condition    �c �\���v�f���̉�f���������ׂ�����
//! @param[in]  max_paint    �c �ő�œh��Ԃ���f���i�ȗ������ꍇ�͏����𖞂����_�����݂��Ȃ��Ȃ�܂Ŏ��s����j
//!
//! @return ���͉摜���s�K�؂ȏꍇ��C�ő唽���񐔂����s���Ă��I�������𖞂����Ȃ������ꍇ�� -1 ��Ԃ��D����ȊO�̏ꍇ�́C�h��Ԃ�����f����Ԃ��D
//!
template < class Array1, class Array2, class PointList, class Component, class Condition >
typename Array1::difference_type region_growing( const Array1 &in, Array2 &out, const PointList &start_points, typename Array2::value_type output_value,
										const Component &components, const Condition &condition, typename Array1::size_type max_paint = type_limits< typename Array1::size_type >::maximum( ) )
{
	return( region_growing( in, out, __region_growing_utility__::no_mask( ), start_points, output_value, components, condition, max_paint ) );
}


/// @}
//  �̈�g���@�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_REGIONGROWING__
