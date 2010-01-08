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

/// @file mist/bspline.h
//!
//! @brief B�X�v���C���Ȑ���`�悷�邽�߂̃��C�u����
//!

#ifndef __INCLUDE_BSPLINE__
#define __INCLUDE_BSPLINE__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// @brief B�X�v���C���̊��֐����v�Z����֐������������\����
//!
//! K����B�X�v���C�����֐��̌v�Z���s��
//!
template < int K >
struct bspline_base
{
	/// @brief K����B�X�v���C�����֐��̌v�Z���s��
	//! 
	//! @param[in] knot �c B�X�v���C���ŗ��p����m�b�g�x�N�g��
	//! @param[in] i    �c B�X�v���C���̐���_�ԍ�
	//! @param[in] t    �c B�X�v���C���̋�Ԉʒu
	//! 
	//! @return �w�肳�ꂽ�m�b�g�x�N�g���C����_�ԍ��C��Ԉʒu�Ɋ�Â����֐��̒l
	//! 
	static double Base( const std::vector< double > &knot, std::vector< double >::size_type i, double t )
	{
		double B = 0.0;

		if( knot[ i + K - 1 ] - knot[ i ] != 0 )
		{
			B = ( t - knot[ i ] ) * bspline_base< K - 1 >::Base( knot, i, t ) / ( knot[ i + K - 1 ] - knot[ i ] );
		}

		if( knot[ i + K ] - knot[ i + 1 ] != 0 )
		{
			B += ( knot[ i + K ] - t ) * bspline_base< K - 1 >::Base( knot, i + 1, t ) / ( knot[ i + K ] - knot[ i + 1 ] );
		}
		return( B );
	}
};


/// @brief B�X�v���C���̊��֐����v�Z����֐������������\���̂̓��ꉻ
//!
//! 1����B�X�v���C�����֐��̌v�Z���s�����߂̓��ꉻ
//!
template < >
struct bspline_base< 1 >
{
	/// @brief K����B�X�v���C�����֐��̌v�Z���s��
	//! 
	//! @param[in] knot �c B�X�v���C���ŗ��p����m�b�g�x�N�g��
	//! @param[in] i    �c B�X�v���C���̐���_�ԍ�
	//! @param[in] t    �c B�X�v���C���̋�Ԉʒu
	//! 
	//! @return �w�肳�ꂽ�m�b�g�x�N�g���C����_�ԍ��C��Ԉʒu�Ɋ�Â����֐��̒l
	//! 
	static double Base( const std::vector< double > &knot, std::vector< double >::size_type i, double t )
	{
		return( ( knot[ i ] <= t && t < knot[ i + 1 ] ) ? 1.0 : 0.0 );
	}
};


//! @addtogroup free_form_group ���R�Ȑ��E�Ȗ�
//!  @{



//! @addtogroup bspline_group B�X�v���C���Ȑ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/bspline.h>
//! @endcode
//!
//!  @{


/// @brief B�X�v���C���Ȑ����������߂̃N���X
//!
//! K����B�X�v���C���Ȑ���`��\�D�ό��̐���_�ɑΉ����C�C�ӂ̃m�b�g�x�N�g���ɑΉ�����B�X�v���C���Ȑ���`��
//!
//! @attention ���͂����f�[�^�^���C�����Z�ƒP��̃X�J���[�l��S�Ăɑ�����鑀��ƕ��������Ƃ̊|���Z���T�|�[�g����K�v������D
//! @attention �������Cvector2�Cvector3 �͐��������삷��悤�ɂȂ��Ă���D
//! @attention array ����MIST�R���e�i�𗘗p����ۂɂ́C�I�y���[�^��L���ɂ���K�v������D
//! 
//! @param T  �c �e����_�E��ԓ_��\���f�[�^�\�����w��idouble �� vector3< double > �Ȃǁj
//! @param K  �c B�X�v���C���Ȑ��̎���
//!
//! @code B�X�v���C���Ȑ��̍쐬��
//! // 3���ŕϐ��̌^�� double ��B�X�v���C�����쐬����
//! mist::bspline< double, 3 > b;
//! 
//! // ����_��ǉ�
//! b.push_back( 2.0 );
//! b.push_back( 3.0 );
//! ...
//! 
//! // �m�b�g�x�N�g�����ȉ��̕��@�̂����ꂩ��p���Đݒ�
//! // �E�Ȑ��̐ݒ�
//! b.knot( mist::bspline< double >::ROUND );
//! // �E�ŏ��ƍŌ�̐���_��ʂ�ꍇ�̐ݒ�
//! b.knot( mist::bspline< double >::THROUGH );
//! // �E�C�ӂ̃m�b�g�x�N�g����ݒ�
//! b.knot( STL�̃x�N�g���^�C�v�̃m�b�g�̃��X�g );
//! 
//! // ��ԓ_���v�Z�i��Ԃ͂O�`�P�j
//! double p1 = b( 0.0 );
//! double p2 = b( 0.4 );
//! double p3 = b( 0.7 );
//! ...
//! 
//! @endcode
//!
template < class T, int K, class Allocator = std::allocator< T > >
class bspline : public std::vector< T, Allocator >
{
private:
	typedef std::vector< T, Allocator > base;
	typedef typename base::allocator_type allocator_type;		///< @brief STL�R���e�i�����p����A���P�[�^�^
	typedef typename base::reference reference;					///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̎Q��
	typedef typename base::const_reference const_reference;		///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q��
	typedef typename base::value_type value_type;				///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^
	typedef typename base::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename base::pointer pointer;						///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^
	typedef typename base::const_pointer const_pointer;			///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^

	typedef std::vector< double > knot_list;

public:
	/// @brief �m�b�g�x�N�g�����蓮�Őݒ肵�Ȃ��ꍇ�̃f�t�H���g�̃��[�h
	enum BSplineMode
	{
		ROUND,					///< @brief �Ȑ��̐ݒ�
		THROUGH,				///< @brief �ŏ��ƍŌ�̐���_��ʂ�ꍇ�̐ݒ�
	};

protected:
	knot_list knot_;			///< @brief �m�b�g�x�N�g��
	BSplineMode mode_;			///< @brief �f�t�H���g�̃m�b�g�x�N�g���ݒ�


public:
	/// @brief �w�肳�ꂽ�ʒu�i��ԂO�`�P�j�ɑΉ�����CB�X�v���C����Ԍ��ʂ�Ԃ�
	//!
	//! @attention �����{�P�̐���_�������͂���Ă���K�v����D����Ȃ��ꍇ�́C�f�t�H���g�̃m�b�g�x�N�g���ōŏ����������D
	//!
	//! @param[in] t  �c �S�̂̋Ȑ�����ԂO�`�P�Ƃ������ɁC��Ԃ����_�̈ʒu
	//!
	//! @return �w�肳�ꂽ�_�̈ʒu�ɑΉ�����B�X�v���C���Ȑ��̕�Ԍ���
	//!
	value_type operator( )( double t )
	{
		size_type n = base::size( ) - 1;	// n + 1 �͐���_�̐�
		size_type m = n + K; 				// m + 1 �̓m�b�g�x�N�g���̐�
		if( knot_.size( ) < m + 1 )
		{
			// �s�K�؂ȃm�b�g�x�N�g�����ݒ肳��Ă��܂�
			knot( mode_ );
		}
		else if( base::empty( ) )
		{
			return( value_type( 0 ) );
		}

		t *= static_cast< double >( m - 2 * K + 2 );

		// �܂��C�[���v�f���쐬����
		value_type p = value_type( base::operator[]( 0 ) ) * 0;
		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			double B = bspline_base< K >::Base( knot_, i, t );
			p += B * base::operator[]( i );
		}

		return( p );
	}

	/// @brief �C�ӂ̃m�b�g�x�N�g����ݒ肷��
	//!
	//! @attention ���� K �Ő���_�� N �Ƃ������ɁC�m�b�g�x�N�g���̐��� N + K + 1 �K�v�ł���
	//! 
	//! @param[in] kknot  �c �m�b�g�x�N�g��
	//!
	void knot( const knot_list &kknot )
	{
		knot_ = kknot;
	}

	/// @brief �f�t�H���g�̃��[�h�Ńm�b�g�x�N�g����ݒ肷��
	//!
	//! �{�N���X���Œ�`����Ă���񋓌^ BSplineMode �̂����̈�𗘗p����
	//! 
	//! @param[in] mode  �c �Ȑ��̃^�C�v
	//!
	void knot( BSplineMode mode )
	{
		size_type n = base::size( ) - 1;	// n + 1 �͐���_�̐�
		size_type m = n + K; 				// m + 1 �̓m�b�g�x�N�g���̐�
		size_type i;

		knot_list kknot( m + 1 );
		switch( mode )
		{
		case ROUND:
			// �����O�̏ꍇ
			for( i = 0 ; i <= m ; i++ )
			{
				kknot[ i ] = static_cast< double >( i - K + 1 );
			}
			break;

		case THROUGH:
			// �ŏ��ƍŌ��ʂ�Ȑ��̏ꍇ
			for( i = 0 ; i < K ; i++ )
			{
				kknot[ i ] = 0.0;
			}
			for( i = K ; i < m - K + 1 ; i++ )
			{
				kknot[ i ] = static_cast< double >( i - K + 1 );
			}
			for( i = m - K + 1 ; i <= m ; i++ )
			{
				kknot[ i ] = static_cast< double >( 2 + m - 2 * K );
			}
			break;
		}

		knot( kknot );
	}

	/// @brief ������Z�q
	const bspline &operator =( const bspline &b )
	{
		if( this != &b )
		{
			base::operator =( b );
			knot_ = b.knot_;
			mode_ = b.mode_;
		}
		return( *this );
	}

	/// @brief �R�s�[�R���X�g���N�^
	bspline( const bspline &b ) : base( b ), knot_( b.knot_ ), mode_( b.mode_ )
	{
	}

	/// @brief �f�t�H���g�̃R���X�g���N�^
	//!
	//! �m�b�g�x�N�g���̃f�t�H���g�l���u�ŏ��ƍŌ��ʂ�Ȑ��v�ɐݒ肷��
	//!
	bspline( ) : mode_( THROUGH )
	{
	}
};

/// @}
//  B�X�v���C���O���[�v�̏I���


/// @}
//  ���R�Ȑ��E�ȖʃO���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_BSPLINE__
