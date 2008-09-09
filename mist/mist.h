/****************************************************************************************************************************
**       MIST ( Media Integration Standard Toolkit )
**
**        We defined following template classes.
**
**          array  : template class of one dimensionl array with STL support.
**          array1 : template class of one dimensionl array containing resolution.
**          array2 : template class of two dimensionl array containing pixel resolution.
**          array3 : template class of three dimensionl array containing voxel resolution.
**          matrix : template class of matrix with its operations, and impremented using expression template technique.
**          vector : template class of vector with its operations, and impremented using expression template technique.
**
**
**         We developed these programs since 2003/09/05.
**
**             $LastChangedDate::                      $
**             $LastChangedRevision$
**             $LastChangedBy$
**             $HeadURL$
**
**
**              Copyright MIST Project Team.
**                    All Rights Reserved.
**
****************************************************************************************************************************/

// 
// Copyright (c) 2003-2008, MIST Project, Intelligent Media Integration COE, Nagoya University
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


/// @file mist/mist.h
//!
//! @brief MIST�̊�{�ƂȂ鉹���E�摜�������R���e�i
//!
//! MIST�Œ񋟂���S�ẴA���S���Y�����ŗ��p����R���e�i�iSTL���j�̏W�܂�ł��D
//! ��{�I�ɂ́CSTL�iStandard Template Library�j�Œ񋟂���Ă���R���e�i�Ƃقڌ݊����������Ă��܂��D
//! �����Ƃ��ẮC�R���e�i���̃f�[�^�̓�������ԏ�ŘA���ƂȂ�悤�Ɏ�������Ă��܂��D
//! �����āCSTL�̃A���S���Y�����ŗp����C�e���[�^�̓��C�����_���A�N�Z�X�C�e���[�^���������Ă���܂��D
//! �C�e���[�^�̏ڍׂɊւ��ẮCSTL�̉���{�ɏ���܂��D
//! �܂��CMIST�̃R���e�i�ł͑g�ݍ��݌^�ƃ��[�U�[��`�^�ŁC���p���郁�����������ʂ��Ă��܂��D
//! �������邱�ƂŁC�R���e�i�̑�����Ŏ኱�����Ȏ��s���\�ƂȂ��Ă��܂��D
//!
//! MIST�v���W�F�N�g�Œ񋟂���R���e�i�́C�ȉ��̍��v6�̃R���e�i�ō\������Ă��܂��D
//!
//! - ���1�����z��iSTL����1�����z��p�R���e�i�j
//! - �𑜓x�t1�����z��i�e�v�f�̉𑜓x�̃f�[�^��ێ��C�Ⴆ��1mm�Ȃǁj
//! - �𑜓x�t2�����z��
//! - �𑜓x�t3�����z��
//! - �s��
//! - �x�N�g��
//!

#ifndef __INCLUDE_MIST_H__
#define __INCLUDE_MIST_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif


#ifndef __INCLUDE_MIST_ALLOC_H__
#include "config/mist_alloc.h"
#endif

#ifndef __INCLUDE_MIST_ITERATOR_H__
#include "config/iterator.h"
#endif



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



/// @brief �v�f�����ς�1�����z��
//! 
//! �S�Ă�MIST�R���e�i�̊�{�ƂȂ�N���X
//! �i@ref mist/mist.h ���C���N���[�h����j
//! 
//! @param T         �c MIST�̃R���e�i���Ɋi�[����f�[�^�^
//! @param Allocator �c MIST�R���e�i�����p����A���P�[�^�^�D�ȗ������ꍇ�́CSTL�̃f�t�H���g�A���P�[�^���g�p����
//! 
template < class T, class Allocator = std::allocator< T > >
class array
{
public:
	typedef Allocator allocator_type;								///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Allocator::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Allocator::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Allocator::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename Allocator::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Allocator::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Allocator::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array< TT, AAllocator > other;
	};


protected:
	mist_allocator< T, Allocator > allocator_;		///< @brief �e�R���e�i�ŗ��p����A���P�[�^�I�u�W�F�N�g

private:
	size_type size_;								///< @brief �R���e�i�Ɋi�[����Ă���v�f��

protected:
	T* data_;	///< @brief �R���e�i�ň����f�[�^�̐擪��\���|�C���^

public:
	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �R���e�i�̃T�C�Y�� num �ɕύX����D
	//! ���̃R���e�i�T�C�Y�ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f�̒l�̓f�t�H���g�R���X�g���N�^�ŏ����������D
	//! �܂��C�g�ݍ��݌^�iint �� double �Ȃǁj�̏ꍇ�́C�S�Ă̗v�f��0�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! @param[in] num  �c ���T�C�Y��̃R���e�i���̗v�f��
	//! @param[in] dmy1 �c �_�~�[�ϐ��i�g�p���Ȃ��j
	//! @param[in] dmy2 �c �_�~�[�ϐ��i�g�p���Ȃ��j
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		if( size_ < num )
		{
			allocator_.deallocate_objects( data_, size_ );
			data_ = allocator_.allocate_objects( num );
			size_ = data_ == NULL ? 0 : num;
		}
		else if( size_ > num )
		{
			data_ = allocator_.trim_objects( data_, size_, num );
			size_ = data_ == NULL ? 0 : num;
		}

		return( data_ != NULL );
	}

	/// @brief �R���e�i���̗v�f���g���~���O����
	//!
	//! �������̊m�ۂɎ��s������C��̃R���e�i�ƂȂ�
	//!
	//! @param[in] index �c �g���~���O�̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] num   �c �g���~���O��̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! 
	//! @retval true  �c �g���~���O�ɐ��������ꍇ�i���ƃT�C�Y���ς��Ȃ��ꍇ���܂ށj
	//! @retval false �c �s���ȃg���~���O���s�����Ƃ����ꍇ
	//! 
	bool trim( size_type index, difference_type num = -1 )
	{
		difference_type num_ = size( );
		if( num_ <= static_cast< difference_type >( index ) || num_ < static_cast< difference_type >( index + num ) )
		{
			return( false );
		}
		else if( num_ == num )
		{
			return( true );
		}

		if( num < 0 )
		{
			num = size( ) - index;
		}

		if( is_memory_shared( ) )
		{
			// �O���������𗘗p���Ă���ꍇ
			array o( *this );

			if( resize( num ) )
			{
				for( difference_type i = 0 ; i < num ; i++ )
				{
					operator []( i ) = o( i + index );
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			array o( num );
			for( difference_type i = 0 ; i < num ; i++ )
			{
				o[ i ] = operator []( i + index );
			}

			swap( o );
		}

		return( true );
	}


	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! @attention �ǂ��炩�̃R���e�i���O���������𗘗p���Ă���ꍇ�́C�X���b�v�͕K�����s����
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( array &a )
	{
		if( is_memory_shared( ) || a.is_memory_shared( ) )
		{
			return( false );
		}
		else
		{
			size_type _dmy_size = size_;
			size_ = a.size_;
			a.size_ = _dmy_size;

			value_type *dmy_data_ = data_;
			data_ = a.data_;
			a.data_ = dmy_data_;

			return( true );
		}
	}


	/// @brief �R���e�i�̗v�f����ɂ���
	//! 
	//! �R���e�i�Ɋi�[����Ă���f�[�^��S�č폜���C�R���e�i����i�v�f��0�j�ɂ���
	//! 
	void clear( )
	{
		allocator_.deallocate_objects( data_, size_ );
		size_ = 0;
		data_ = NULL;
	}


	/// @brief �R���e�i���̃f�[�^�v�f���f�t�H���g�l�ŏ���������
	//! 
	//! �f�t�H���g�R���X�g���N�^�̒l�ŏ���������D
	//! �g�ݍ��݌^�iint �� double �Ȃǁj�̏ꍇ�́C�S�Ă̗v�f��0�ŏ���������D
	//! 
	void fill( )
	{
		allocator_.fill_objects( data_, size_ );
	}


	/// @brief �R���e�i���̃f�[�^�v�f���w�肳�ꂽ�l�ŏ���������
	//! 
	//! �S�Ă̗v�f��l val �ŏ���������D
	//! 
	//! @param[in] val �c �v�f������������l
	//! 
	void fill( const value_type &val )
	{
		allocator_.fill_objects( data_, size_, val );
	}


	/// @brief �R���e�i���󂩂ǂ����𔻒�
	//! 
	//! @retval true  �c �R���e�i�̗v�f������i0�j�̏ꍇ
	//! @retval false �c �R���e�i�̗v�f������i0�j�łȂ��ꍇ
	//! 
	bool empty( ) const { return( size_ == 0 ); }


	size_type size( ) const { return( size_ ); }	///< @brief �R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size1( ) const { return( size_ ); }	///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size2( ) const { return( 1 ); }		///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j
	size_type size3( ) const { return( 1 ); }		///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j
	size_type width( ) const { return( size_ ); }	///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const { return( 1 ); }		///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j
	size_type depth( ) const { return( 1 ); }		///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j

	double reso1( double r1 ){ return( 1.0 ); }		///< @brief X�������̉𑜓x��Ԃ�
	double reso1( ) const { return( 1.0 ); }		///< @brief X�������̉𑜓x��ݒ肷��
	double reso2( double r2 ){ return( 1.0 ); }		///< @brief Y�������̉𑜓x��Ԃ�
	double reso2( ) const { return( 1.0 ); }		///< @brief Y�������̉𑜓x��ݒ肷��
	double reso3( double r3 ){ return( 1.0 ); }		///< @brief Z�������̉𑜓x��Ԃ�
	double reso3( ) const { return( 1.0 ); }		///< @brief Z�������̉𑜓x��ݒ肷��j


	/// @brief �R���e�i���̗v�f����߂�f�[�^�ʂ��o�C�g�P�ʂŕԂ�
	size_type byte( ) const { return( size_ * sizeof( value_type ) ); }

	/// @brief �O���Ŋ��蓖�Ă�ꂽ�������̈���g�p���Ă��邩�ǂ���
	bool is_memory_shared( ) const { return( allocator_.is_memory_shared( ) ); }


	/// @brief �R���e�i�̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �����������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.begin( );
	//! for( ; ite1 != a.end( ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator begin( ){ return( iterator( paccess( 0 ), 1 ) ); }

	/// @brief �R���e�i�̐擪���w���R���X�g�^�̃R���e�i�𑀍삷�郉���_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator begin( ) const { return( const_iterator( paccess( 0 ), 1 ) ); }


	/// @brief �R���e�i�̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator end( ){ return( iterator( paccess( size( ) ), 1 ) ); }

	/// @brief �R���e�i�̖������w���R���X�g�^�̃R���e�i�𑀍삷�郉���_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator end( ) const { return( const_iterator( paccess( size( ) ), 1 ) ); }



	/// @brief �R���e�i�̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.rbegin( );
	//! for( ; ite2 != a.rend( ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }

	/// @brief �R���e�i�̖������w���R���X�g�^�̃R���e�i�𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }


	/// @brief �R���e�i�̐擪���w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }

	/// @brief �R���e�i�̐擪���w���R���X�g�^�̃R���e�i�𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }


	/// @brief �g�p���Ă���A���P�[�^���m�ۉ\�ȃ������̍ő�l��Ԃ�
	size_type max_size( ) const { return( allocator_.max_size( ) ); }



private: // �T�|�[�g���Ȃ��r�s�k�̊֐��i�����E�g�p�����Ⴞ�߁j
	iterator erase( iterator i );
	iterator erase( iterator s, iterator e );
	iterator insert( iterator i, const value_type &val );
	void insert( iterator i, size_type num, const value_type &val );

public:
	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �R�s�[���ƃR�s�[��Ńf�[�^�^�iarray< data > �� data�j���قȂ�ꍇ�̑�����s���D
	//! �������̊m�ۂɎ��s������C��̃R���e�i�ƂȂ�
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const array& operator =( const array< TT, AAlocator >  &o )
	{
		if( size_ > o.size( ) )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size( ) );
			size_ = data_ == NULL ? 0 : o.size( );
		}
		else if( size_ < o.size( ) )
		{
			allocator_.deallocate_objects( data_, size_ );
			data_ = allocator_.allocate_objects( o.size( ) );
			size_ = data_ == NULL ? 0 : o.size( );
		}

		if( data_ != NULL )
		{
			for( size_type i = 0 ; i < size_ ; i++ )
			{
				data_[ i ] = static_cast< value_type >( o[ i ] );
			}
		}

		return( *this );
	}

	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �������̊m�ۂɎ��s������C��̃R���e�i�ƂȂ�
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	const array& operator =( const array  &o )
	{
		if( this == &o ) return( *this );

		// �܂��C�A���P�[�^���R�s�[����
		allocator_ = o.allocator_;

		if( size_ > o.size_ )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size_ );
			size_ = data_ == NULL ? 0 : o.size( );
		}
		else if( size_ < o.size_ )
		{
			allocator_.deallocate_objects( data_, size_ );
			data_ = allocator_.allocate_objects( o.size( ) );
			size_ = data_ == NULL ? 0 : o.size( );
		}

		if( data_ != NULL )
		{
			allocator_.copy_objects( o.data_, size_, data_ );
		}

		return( *this );
	}

// �R���e�i���̗v�f�ւ̃A�N�Z�X���Z�q
protected:
	/// @brief index �Ŏ������ʒu�̗v�f�̃|�C���^��Ԃ�
	//! 
	//! @param[in] index  �c �R���e�i���̗v�f�ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	pointer paccess( size_type index )
	{
		return( data_ + index );
	}

	/// @brief index �Ŏ������ʒu�̗v�f�� const �|�C���^��Ԃ�
	//! 
	//! @param[in] index  �c �R���e�i���̗v�f�ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f������ const �|�C���^
	//! 
	const_pointer paccess( size_type index ) const
	{
		return( data_ + index );
	}

public:
	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	//! 
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	//! 
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator []( size_type index )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator []( size_type index ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[ index ] );
	}

public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0 �̃R���e�i���쐬����
	array( ) : allocator_( ), size_( 0 ), data_( NULL ){}

	/// @brief �A���P�[�^ a �̃R�s�[�𗘗p����
	explicit array( const Allocator &a ) : allocator_( a ), size_( 0 ), data_( NULL ){}


	/// @brief �v�f�� num �̃R���e�i���쐬���C�f�t�H���g�l�ŗv�f������������
	explicit array( size_type num ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief �v�f�� num �̃R���e�i���쐬���C�A���P�[�^ a �̃R�s�[�𗘗p����
	array( size_type num, const Allocator &a ) : allocator_( a ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief �v�f�� num �̃R���e�i���쐬���C�l val �ŏ���������
	array( size_type num, const value_type &val ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief �v�f�� num �̃R���e�i���쐬���C�l val �ŏ��������C�A���P�[�^ a �̃R�s�[�𗘗p����
	array( size_type num, const value_type &val, const Allocator &a ) : allocator_( a ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief �C�e���[�^ s �� e �͈̔͂̒l��p���āC�z�������������
	array( const_iterator s, const_iterator e ) : allocator_( ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief �C�e���[�^ s �� e �͈̔͂̒l��p���āC�z������������C�A���P�[�^ a �̃R�s�[�𗘗p����
	array( const_iterator s, const_iterator e, const Allocator &a ) : allocator_( a ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief ptr ���w���������̈�ɁC�v�f�� num �̃R���e�i���쐬����iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array( size_type num, pointer ptr, size_type mem_available ) : allocator_( ptr, mem_available ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL ) size_ = 0;
	}
	/// @brief ptr ���w���������̈�ɁC�v�f�� num �̃R���e�i���쐬���l val �ŏ���������iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array( size_type num, const value_type &val, pointer ptr, size_type mem_available ) : allocator_( ptr, mem_available ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
		if( data_ == NULL ) size_ = 0;
	}


	/// @brief ���� array �z��ŗv�f�̌^���قȂ���̂��瓯���v�f���̔z����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	array( const array< TT, AAlocator > &o ) : allocator_( ), size_( o.size( ) ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL )
		{
			size_ = 0;
		}
		else
		{
			for( size_type i = 0 ; i < size_ ; i++ ) data_[i] = static_cast< value_type >( o[i] );
		}
	}

	/// @brief ���� array �z��œ����v�f�^�̂��̂�p���ď���������
	array( const array< T, Allocator > &o ) : allocator_( o.allocator_ ), size_( o.size_ ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		if( data_ == NULL )
		{
			size_ = 0;
		}
		else
		{
			allocator_.copy_objects( o.data_, size_, data_ );
		}
	}

	/// @brief �R���e�i�����p���Ă��郊�\�[�X��S�ĊJ������
	~array( )
	{
		clear( );
	}
};



/// @brief �v�f�����ς̉𑜓x�t1�����z��
//! 
//! ��ɉ������������߂̊�{�N���X
//! �i@ref mist/mist.h ���C���N���[�h����j
//! 
//! @param T         �c MIST�̃R���e�i���Ɋi�[����f�[�^�^
//! @param Allocator �c MIST�R���e�i�����p����A���P�[�^�^�D�ȗ������ꍇ�́CSTL�̃f�t�H���g�A���P�[�^���g�p����
//! 
template < class T, class Allocator = std::allocator< T > >
class array1 : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Allocator::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Allocator::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Allocator::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename Allocator::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Allocator::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Allocator::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array1< TT, AAllocator > other;
	};

private:
	typedef array< T, Allocator > base;		///< @brief ���N���X�̕ʖ�

protected:
	double reso1_;							///< @brief �R���e�i���̗v�f�̉𑜓x�i�~������g���Ȃǁj

public:
	double reso1( double r1 ){ return( reso1_ = r1 ); }								//< @brief X�������̉𑜓x�� r1 �ɐݒ肵�C�ݒ��̒l��Ԃ�
	double reso1( ) const { return( reso1_ ); }										//< @brief X�������̉𑜓x��Ԃ�
	void reso( double r1, double dmy1 = 1.0, double dmy2 = 1.0 ){ reso1_ = r1; }	//< @brief X�������̉𑜓x�� r1 �ɐݒ肵�C�ݒ��̒l��Ԃ�


/************************************************************************************************************
**
**      X�����ɑ΂��鏇�����E�t�����̔����q
**
************************************************************************************************************/

	/// @brief X�������̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �����������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.x_begin( );
	//! for( ; ite1 != a.x_end( ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator x_begin( ){ return( base::begin( ) ); }

	/// @brief X�������̐擪���w���R���X�g�^�̃R���e�i�𑀍삷�郉���_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator x_begin( ) const { return( base::begin( ) ); }


	/// @brief X�������̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator x_end( ){ return( base::end( ) ); }

	/// @brief X�������̖������w���R���X�g�^�̃R���e�i�𑀍삷�郉���_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator x_end( ) const { return( base::end( ) ); }



	/// @brief X�������̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.x_rbegin( );
	//! for( ; ite2 != a.x_rend( ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator x_rbegin( ){ return( base::rbegin( ) ); }

	/// @brief X�������̖������w���R���X�g�^�̃R���e�i�𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator x_rbegin( ) const { return( base::rbegin( ) ); }


	/// @brief X�������̐擪���w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator x_rend( ){ return( base::rend( ) ); }

	/// @brief X�������̐擪���w���R���X�g�^�̃R���e�i�𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator x_rend( ) const { return( base::rend( )  ); }


public: // �z��ɑ΂��鑀��

	/// @brief �R���e�i���̗v�f���g���~���O����
	//! 
	//! @param[in] index �c �g���~���O�̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] num   �c �g���~���O��̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! 
	//! @retval true  �c �g���~���O�ɐ��������ꍇ�i���ƃT�C�Y���ς��Ȃ��ꍇ���܂ށj
	//! @retval false �c �s���ȃg���~���O���s�����Ƃ����ꍇ
	//! 
	bool trim( size_type index, difference_type num = -1 )
	{
		double r = reso1( );
		if( base::trim( index, num ) )
		{
			reso( r );
			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! @attention �ǂ��炩�̃R���e�i���O���������𗘗p���Ă���ꍇ�́C�X���b�v�͕K�����s����
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( array1 &a )
	{
		if( base::swap( a ) )
		{
			double dmy_reso1_ = reso1_;
			reso1_ = a.reso1_;
			a.reso1_ = dmy_reso1_;
			return( true );
		}
		else
		{
			return( false );
		}
	}

public:
	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �R�s�[���ƃR�s�[��Ńf�[�^�^�iarray< data > �� data�j���قȂ�ꍇ�̑�����s���D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const array1& operator =( const array1< TT, AAlocator > &o )
	{
		base::operator =( o );
		reso1_ = o.reso1( );

		return( *this );
	}


	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	const array1& operator =( const array1 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		reso1_ = o.reso1_;

		return( *this );
	}

public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0�C�𑜓x 1.0 �̃R���e�i���쐬����
	array1( ) : base( ), reso1_( 1.0 ) {}

	/// @brief �A���P�[�^ a �̃R�s�[�𗘗p����
	explicit array1( const Allocator &a ) : base( a ), reso1_( 1.0 ) {}

	/// @brief �v�f�� num �̃R���e�i���쐬���C�f�t�H���g�l�ŗv�f������������
	explicit array1( size_type num ) : base( num ), reso1_( 1.0 ) {}


	/// @brief �v�f�� num �̃R���e�i���쐬���C�𑜓x�� r1 �ɐݒ肷��
	array1( size_type num, double r1 ) : base( num ), reso1_( r1 ) {}

	/// @brief �v�f�� num �̃R���e�i���쐬���C�g�p����A���P�[�^�� a �ɐݒ肷��
	array1( size_type num, const Allocator &a ) : base( num, a ), reso1_( 1.0 ) {}

	/// @brief �v�f�� num �̃R���e�i���쐬���C�𑜓x�� r1�C�A���P�[�^�� a �ɐݒ肷��
	array1( size_type num, double r1, const Allocator &a ) : base( num, a ), reso1_( r1 ) {}


	/// @brief �v�f�� num �̃R���e�i���쐬���C�𑜓x�� r1 �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array1( size_type num, double r1, const value_type &val ) : base( num, val ), reso1_( r1 ) {}

	/// @brief �v�f�� num �̃R���e�i���쐬���C�𑜓x�� r1 �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array1( size_type num, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( 1.0 ) {}

	/// @brief �v�f�� num �̃R���e�i���쐬���C�𑜓x�� r1�C�A���P�[�^�� a  �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array1( size_type num, double r1, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( r1 ) {}


	/// @brief ptr ���w���������̈�ɁC�v�f�� num �̃R���e�i���쐬����iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array1( size_type num, pointer ptr, size_type mem_available ) : base( num, ptr, mem_available ), reso1_( 1.0 ) {}

	/// @brief ptr ���w���������̈�ɁC�v�f�� num �̃R���e�i���쐬���C�𑜓x�� r1 �ɐݒ肷��iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array1( size_type num, double r1, pointer ptr, size_type mem_available ) : base( num, ptr, mem_available ), reso1_( r1 ) {}

	/// @brief ptr ���w���������̈�ɁC�v�f�� num �̃R���e�i���쐬���C�𑜓x�� r1�C�l val �ŏ���������iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array1( size_type num, double r1, const value_type &val, pointer ptr, size_type mem_available ) : base( num, val, ptr, mem_available ), reso1_( r1 ) {}

	/// @brief ���� array1 �z��ŗv�f�̌^���قȂ���̂��瓯���v�f���̔z����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	explicit array1( const array1< TT, AAlocator > &o ) : base( o ), reso1_( o.reso1( ) ) {}

	/// @brief ���� array1 �z��œ����v�f�^�̂��̂�p���ď���������
	array1( const array1< T, Allocator > &o ) : base( o ), reso1_( o.reso1_ ) {}
};



/// @brief �v�f�����ς̉𑜓x�t��2�����z��
//! 
//! 2�����摜���������߂̊�{�N���X
//! �i@ref mist/mist.h ���C���N���[�h����j
//! 
//! @param T         �c MIST�̃R���e�i���Ɋi�[����f�[�^�^
//! @param Allocator �c MIST�R���e�i�����p����A���P�[�^�^�D�ȗ������ꍇ�́CSTL�̃f�t�H���g�A���P�[�^���g�p����
//! 
template < class T, class Allocator = std::allocator< T > >
class array2 : public array1< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Allocator::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Allocator::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Allocator::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename Allocator::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Allocator::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Allocator::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array2< TT, AAllocator > other;
	};


private:
	typedef array1< T, Allocator > base;	///< @brief ���N���X
	size_type size2_;						///< @brief Y�������̗v�f��
	size_type size1_;						///< @brief X�������̗v�f��

protected:
	double reso2_;							///< @brief X�������̉𑜓x

public:
	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �v�f���� num1 �~ num2 �ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f��l�f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! @param[in] num2 �c ���T�C�Y���Y�������̗v�f��
	//! @param[in] dmy1 �c �_�~�[�ϐ��i�g�p���Ȃ��j
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num1, size_type num2, size_type dmy1 = 0 )
	{
		if( base::resize( num1 * num2 ) )
		{
			size1_ = num1;
			size2_ = num2;
			return( true );
		}
		else
		{
			size1_ = size2_ = 0;
			return( false );
		}
	}

	/// @brief �R���e�i���̗v�f���g���~���O����
	//! 
	//! @param[in] x �c �g���~���O��X�������̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] y �c �g���~���O��Y�������̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] w �c �g���~���O���X�������̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! @param[in] h �c �g���~���O���Y�������̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! 
	//! @retval true  �c �g���~���O�ɐ��������ꍇ�i���ƃT�C�Y���ς��Ȃ��ꍇ���܂ށj
	//! @retval false �c �s���ȃg���~���O���s�����Ƃ����ꍇ
	//! 
	bool trim( size_type x, size_type y, difference_type w = -1, difference_type h = -1 )
	{
		difference_type w_ = width( );
		difference_type h_ = height( );
		if( w_ <= static_cast< difference_type >( x ) || w_ < static_cast< difference_type >( x + w ) )
		{
			return( false );
		}
		else if( h_ <= static_cast< difference_type >( y ) || h_ < static_cast< difference_type >( y + h ) )
		{
			return( false );
		}
		else if( w_ == w && h_ == h )
		{
			return( true );
		}

		if( w < 0 )
		{
			w = w_ - x;
		}
		if( h < 0 )
		{
			h = h_ - y;
		}

		if( base::is_memory_shared( ) )
		{
			// �O���������𗘗p���Ă���ꍇ
			array2 o( *this );
			if( resize( w, h ) )
			{
				const_pointer pi = o.paccess( x, y );
				pointer       po = paccess( 0, 0 );
				for( difference_type j = 0 ; j < h ; j++ )
				{
					po = allocator_.copy_objects( pi, w, po );
					pi += o.width( );
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			array2 o( w, h, base::reso1( ), reso2( ) );

			const_pointer pi = paccess( x, y );
			pointer       po = o.paccess( 0, 0 );
			for( difference_type j = 0 ; j < h ; j++ )
			{
				po = allocator_.copy_objects( pi, w, po );
				pi += this->width( );
			}

			swap( o );
		}

		return( true );
	}


	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! @attention �ǂ��炩�̃R���e�i���O���������𗘗p���Ă���ꍇ�́C�X���b�v�͕K�����s����
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( array2 &a )
	{
		if( base::swap( a ) )
		{
			double dmy_reso2_ = reso2_;
			reso2_ = a.reso2_;
			a.reso2_ = dmy_reso2_;

			size_type _dmy_size1 = size1_;
			size_type _dmy_size2 = size2_;
			size1_ = a.size1_;
			size2_ = a.size2_;
			a.size1_ = _dmy_size1;
			a.size2_ = _dmy_size2;
			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief �R���e�i�̗v�f����ɂ���
	//! 
	//! �R���e�i�Ɋi�[����Ă���f�[�^��S�č폜���C�R���e�i����i�v�f��0�j�ɂ���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}


	size_type size1( )  const { return( size1_ ); }			///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size2( )  const { return( size2_ ); }			///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type width( )  const { return( size1_ ); }			///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const { return( size2_ ); }			///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�

	double reso2( double r2 ){ return( reso2_ = r2 ); }		///< @brief Y�������̉𑜓x�� r2 �ɐݒ肵�C�ݒ��̒l��Ԃ�
	double reso2( ) const { return( reso2_ ); }				///< @brief Y�������̉𑜓x��Ԃ�

	/// @brief X����Y�������̉𑜓x��1�x�ɐݒ肷��
	//!
	//! @param[in] r1  �c �V����X�������̉𑜓x
	//! @param[in] r2  �c �V����Y�������̉𑜓x
	//! @param[in] dmy �c ���p���Ȃ�
	//!
	void reso( double r1, double r2, double dmy = 1.0 ){ base::reso1_ = r1; reso2_ = r2; }



/************************************************************************************************************
**
**      X�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief X���������Œ肵�����́CY�������̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code X���������Œ肵�CY�������̐擪���w�������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.x_begin( 1 );
	//! for( ; ite1 != a.x_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0 ), width( ) ) ); }

	/// @brief X���������Œ肵�����́CY�������̐擪���w���R���X�g�^�̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0 ), width( ) ) ); }

	/// @brief X���������Œ肵�����́CY�������̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator x_end( size_type i ){ return( iterator( paccess( i, height( ) ), width( ) ) ); }

	/// @brief X���������Œ肵�����́CY�������̖������w���R���X�g�^�̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, height( ) ), width( ) ) ); }


	/// @brief X���������Œ肵�����́CY�������̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code X���������Œ肵�����́CY�������̋t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.x_rbegin( 2 );
	//! for( ; ite2 != a.x_rend( 2 ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }

	/// @brief X���������Œ肵�����́CY�������̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }


	/// @brief X���������Œ肵�����́CY�������̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }

	/// @brief X���������Œ肵�����́CY�������̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }

/************************************************************************************************************
**
**      Y�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief Y���������Œ肵�����́CX�������̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code Y���������Œ肵�����́CX�������̐擪���w�������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_begin( 1 );
	//! for( ; ite1 != a.y_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j ), 1 ) ); }

	/// @brief Y���������Œ肵�����́CX�������̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j ), 1 ) ); }

	/// @brief Y���������Œ肵�����́CX�������̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator y_end( size_type j ){ return( iterator( paccess( width( ), j ), 1 ) ); }

	/// @brief Y���������Œ肵�����́CX�������̖������w���R���X�g�^�̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( width( ), j ), 1 ) ); }


	/// @brief Y���������Œ肵�����́CX�������̐擪���w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code Y���������Œ肵�����́CX�������̐擪���w���t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_rbegin( 1 );
	//! for( ; ite1 != a.y_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }

	/// @brief Y���������Œ肵�����́CX�������̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief Y���������Œ肵�����́CX�������̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }

	/// @brief Y���������Œ肵�����́CX�������̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


public:
	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �R�s�[���ƃR�s�[��Ńf�[�^�^�iarray< data > �� data�j���قȂ�ꍇ�̑�����s���D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const array2& operator =( const array2< TT, AAlocator > &o )
	{
		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
		}

		reso2_ = o.reso2( );

		return( *this );
	}


	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	const array2& operator =( const array2 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
		}

		reso2_ = o.reso2( );

		return( *this );
	}

// �v�f�ւ̃A�N�Z�X
protected:
	/// @brief index �Ŏ������ʒu�̗v�f�̃|�C���^��Ԃ�
	//! 
	//! @param[in] i �c �R���e�i����X�������̈ʒu
	//! @param[in] j �c �R���e�i����Y�������̈ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	pointer paccess( size_type i, size_type j )
	{
		return( base::data_ + i + j * size1_ );
	}

	/// @brief index �Ŏ������ʒu�̗v�f�� const �|�C���^��Ԃ�
	//! 
	//! @param[in] i �c �R���e�i����X�������̈ʒu
	//! @param[in] j �c �R���e�i����Y�������̈ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	const_pointer paccess( size_type i, size_type j ) const
	{
		return( base::data_ + i + j * size1_ );
	}

public:
	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0�C�𑜓x 1.0 �~ 1.0 �̃R���e�i���쐬����
	array2( ) : base( ), size2_( 0 ), size1_( 0 ), reso2_( 1.0 ) {}

	/// @brief �A���P�[�^ a �̃R�s�[�𗘗p����
	explicit array2( const Allocator &a ) : base( a ), size2_( 0 ), size1_( 0 ), reso2_( 1.0 ) {}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�f�t�H���g�l�ŗv�f������������
	array2( size_type num1, size_type num2 ) : base( num1 * num2 ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �ɐݒ肷��
	array2( size_type num1, size_type num2, double r1, double r2 ) : base( num1 * num2, r1 ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�g�p����A���P�[�^�� a �ɐݒ肷��
	array2( size_type num1, size_type num2, const Allocator &a ) : base( num1 * num2, a ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �ɐݒ肵�C�g�p����A���P�[�^�� a �ɐݒ肷��
	array2( size_type num1, size_type num2, double r1, double r2, const Allocator &a ) : base( num1 * num2, r1, a ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� 1.0 �~ 1.0 �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array2( size_type num1, size_type num2, const value_type &val ) : base( num1 * num2, val ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val ) : base( num1 * num2, r1, val ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �ɐݒ肵�C�g�p����A���P�[�^�� a �ɐݒ肷��
	array2( size_type num1, size_type num2, const value_type &val, const Allocator &a ) : base( num1 * num2, val, a ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2�C�g�p����A���P�[�^�� a �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val, const Allocator &a ) : base( num1 * num2, r1, val, a ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �̃R���e�i���쐬����iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array2( size_type num1, size_type num2, pointer ptr, size_type mem_available ) : base( num1 * num2, ptr, mem_available ), size2_( num2 ), size1_( num1 ), reso2_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �ɐݒ肷��iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array2( size_type num1, size_type num2, double r1, double r2, pointer ptr, size_type mem_available ) : base( num1 * num2, r1, ptr, mem_available ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2�C�l val �ŏ���������iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val, pointer ptr, size_type mem_available ) : base( num1 * num2, r1, val, ptr, mem_available ), size2_( num2 ), size1_( num1 ), reso2_( r2 )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}



	/// @brief ���� array2 �z��ŗv�f�̌^���قȂ���̂��瓯���v�f���̔z����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	array2( const array2< TT, AAlocator > &o ) : base( o ), size2_( o.size2( ) ), size1_( o.size1( ) ), reso2_( o.reso2( ) )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief ���� array2 �z��œ����v�f�^�̂��̂�p���ď���������
	array2( const array2< T, Allocator > &o ) : base( o ), size2_( o.size2_ ), size1_( o.size1_ ), reso2_( o.reso2_ )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}
};





/// @brief �v�f�����ς̉𑜓x�t��3�����z��
//! 
//! 3�����摜���������߂̊�{�N���X
//! �i@ref mist/mist.h ���C���N���[�h����j
//! 
//! @param T         �c MIST�̃R���e�i���Ɋi�[����f�[�^�^
//! @param Allocator �c MIST�R���e�i�����p����A���P�[�^�^�D�ȗ������ꍇ�́CSTL�̃f�t�H���g�A���P�[�^���g�p����
//! 
template < class T, class Allocator = std::allocator< T > >
class array3 : public array2< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Allocator::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Allocator::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Allocator::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename Allocator::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Allocator::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Allocator::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef mist_iterator2< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_iterator2< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef array3< TT, AAllocator > other;
	};


private:
	typedef array2< T, Allocator > base;	///< @brief ���N���X
	size_type size3_;						///< @brief Z�������̗v�f��
	size_type size2_;						///< @brief Y�������̗v�f��
	size_type size1_;						///< @brief X�������̗v�f��

protected:
	double reso3_;							///< @brief Z�������̉𑜓x

public:
	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �v�f���� num1 �~ num2 �~ num3 �ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f���f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! @param[in] num2 �c ���T�C�Y���Y�������̗v�f��
	//! @param[in] num3 �c ���T�C�Y���Z�������̗v�f��
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num1, size_type num2, size_type num3 )
	{
		if( base::resize( num1 * num2, num3 ) )
		{
			size1_ = num1;
			size2_ = num2;
			size3_ = num3;
			return( true );
		}
		else
		{
			size1_ = size2_ = size3_ = 0;
			return( false );
		}
	}

	/// @brief �R���e�i���̗v�f���g���~���O����
	//! 
	//! @param[in] x �c �g���~���O��X�������̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] y �c �g���~���O��Y�������̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] z �c �g���~���O��Z�������̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] w �c �g���~���O���X�������̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! @param[in] h �c �g���~���O���Y�������̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! @param[in] d �c �g���~���O���Z�������̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! 
	//! @retval true  �c �g���~���O�ɐ��������ꍇ�i���ƃT�C�Y���ς��Ȃ��ꍇ���܂ށj
	//! @retval false �c �s���ȃg���~���O���s�����Ƃ����ꍇ
	//! 
	bool trim( size_type x, size_type y, size_type z, difference_type w = -1, difference_type h = -1, difference_type d = -1 )
	{
		difference_type w_ = width( );
		difference_type h_ = height( );
		difference_type d_ = depth( );
		if( w_ <= static_cast< difference_type >( x ) || w_ < static_cast< difference_type >( x + w ) )
		{
			return( false );
		}
		else if( h_ <= static_cast< difference_type >( y ) || h_ < static_cast< difference_type >( y + h ) )
		{
			return( false );
		}
		else if( d_ <= static_cast< difference_type >( z ) || d_ < static_cast< difference_type >( z + d ) )
		{
			return( false );
		}
		else if( w_ == w && h_ == h && d_ == d )
		{
			return( true );
		}

		if( w < 0 )
		{
			w = w_ - x;
		}
		if( h < 0 )
		{
			h = h_ - y;
		}
		if( d < 0 )
		{
			d = d_ - z;
		}

		if( base::is_memory_shared( ) )
		{
			// �O���������𗘗p���Ă���ꍇ
			array3 o( *this );
			if( resize( w, h, d ) )
			{
				const_pointer pi = o.paccess( x, y, z );
				pointer       po = paccess( 0, 0, 0 );
				size_type     s1 = o.paccess( x, y + 1, z ) - pi;
				size_type     s2 = o.paccess( x, y, z + 1 ) - o.paccess( x, y + h, z );

				for( difference_type k = 0 ; k < d ; k++ )
				{
					for( difference_type j = 0 ; j < h ; j++ )
					{
						po = allocator_.copy_objects( pi, w, po );
						pi += s1;
					}

					pi += s2;
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			array3 o( w, h, d, base::reso1( ), base::reso2( ), reso3( ) );

			const_pointer pi = paccess( x, y, z );
			pointer       po = o.paccess( 0, 0, 0 );
			size_type     s1 = paccess( x, y + 1, z ) - pi;
			size_type     s2 = paccess( x, y, z + 1 ) - paccess( x, y + h, z );

			for( difference_type k = 0 ; k < d ; k++ )
			{
				for( difference_type j = 0 ; j < h ; j++ )
				{
					po = allocator_.copy_objects( pi, w, po );
					pi += s1;
				}

				pi += s2;
			}

			swap( o );
		}

		return( true );
	}


	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! @attention �ǂ��炩�̃R���e�i���O���������𗘗p���Ă���ꍇ�́C�X���b�v�͕K�����s����
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( array3 &a )
	{
		if( base::swap( a ) )
		{
			double dmy_reso3_ = reso3_;
			reso3_ = a.reso3_;
			a.reso3_ = dmy_reso3_;

			size_type _dmy_size1 = size1_;
			size_type _dmy_size2 = size2_;
			size_type _dmy_size3 = size3_;
			size1_ = a.size1_;
			size2_ = a.size2_;
			size3_ = a.size3_;
			a.size1_ = _dmy_size1;
			a.size2_ = _dmy_size2;
			a.size3_ = _dmy_size3;

			return( true );
		}
		else
		{
			return( true );
		}
	}


	/// @brief �R���e�i�̗v�f����ɂ���
	//! 
	//! �R���e�i�Ɋi�[����Ă���f�[�^��S�č폜���C�R���e�i����i�v�f��0�j�ɂ���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}


	size_type size1( ) const { return( size1_ ); }			///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size2( ) const { return( size2_ ); }			///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size3( ) const { return( size3_ ); }			///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type width( ) const { return( size1_ ); }			///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const { return( size2_ ); }			///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type depth( ) const { return( size3_ ); }			///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�

	double reso3( double r3 ){ return( reso3_ = r3 ); }		///< @brief Z�������̉𑜓x�� r3 �ɐݒ肵�C�ݒ��̒l��Ԃ�
	double reso3( ) const { return( reso3_ ); }				///< @brief Z�������̉𑜓x��Ԃ�

	/// @brief X����Y����Z�������̉𑜓x��1�x�ɐݒ肷��
	//!
	//! @param[in] r1 �c �V����X�������̉𑜓x
	//! @param[in] r2 �c �V����Y�������̉𑜓x
	//! @param[in] r3 �c �V����Y�������̉𑜓x
	//!
	void reso( double r1, double r2, double r3 ){ base::reso1_ = r1; base::reso2_ = r2; reso3_ = r3; }



	/// @brief �R���e�i�̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �����������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.begin( );
	//! for( ; ite1 != a.end( ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator begin( ){ return( iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }

	/// @brief �R���e�i�̐擪���w���R���X�g�^�̃R���e�i�𑀍삷�郉���_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator begin( ) const { return( const_iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }

	/// @brief �R���e�i�̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator end( ){ return( iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }

	/// @brief �R���e�i�̖������w���R���X�g�^�̃R���e�i�𑀍삷�郉���_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator end( ) const { return( const_iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }


	/// @brief �R���e�i�̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.rbegin( );
	//! for( ; ite2 != a.rend( ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }

	/// @brief �R���e�i�̖������w���R���X�g�^�̃R���e�i�𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }

	/// @brief �R���e�i�̐擪���w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }

	/// @brief �g�p���Ă���A���P�[�^���m�ۉ\�ȃ������̍ő�l��Ԃ�
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }

/************************************************************************************************************
**
**      X�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code X���������Œ肵�CYZ����(Sagittal)�̐擪���w�������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.x_begin( 1 );
	//! for( ; ite1 != a.x_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }

	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̐擪���w���R���X�g�^�̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }

	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator x_end( size_type i ){ return( iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }

	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̖������w���R���X�g�^�̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }



	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code X���������Œ肵�����́CYZ����(Sagittal)�̋t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::reverse_iterator ite2 = a.x_rbegin( 2 );
	//! for( ; ite2 != a.x_rend( 2 ) ; ite2++ )
	//! {
	//! 	std::cout << *ite2 << " ";
	//! }
	//! std::cout << std::endl;
	//! @endcode
	//!
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }

	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }

	/// @brief X���������Œ肵�����́CYZ����(Sagittal)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }


/************************************************************************************************************
**
**      Y�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code Y���������Œ肵�����́CXZ���ʂ̐擪���w�������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_begin( 1 );
	//! for( ; ite1 != a.y_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }

	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }

	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator y_end( size_type j ){ return( iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }

	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̖������w���R���X�g�^�̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }


	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̐擪���w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code Y���������Œ肵�����́CXZ����(Coronal)�̐擪���w���t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.y_rbegin( 1 );
	//! for( ; ite1 != a.y_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }

	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }

	/// @brief Y���������Œ肵�����́CXZ����(Coronal)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


/************************************************************************************************************
**
**      Z�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief Z���������Œ肵�����́CXY����(Axial)�̐擪���w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code Z���������Œ肵�����́CXY���ʂ̐擪���w�������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.z_begin( 1 );
	//! for( ; ite1 != a.z_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator z_begin( size_type k ){ return( iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }

	/// @brief Z���������Œ肵�����́CXY����(Axial)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator z_begin( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }

	/// @brief Z���������Œ肵�����́CXY����(Axial)�̖������w�������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator z_end( size_type k ){ return( iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }

	/// @brief Z���������Œ肵�����́CXY����(Axial)�̖������w���R���X�g�^�̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator z_end( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }


	/// @brief Z���������Œ肵�����́CXY����(Axial)�̐擪���w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code Z���������Œ肵�����́CXY����(Axial)�̐擪���w���t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.z_rbegin( 1 );
	//! for( ; ite1 != a.z_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator z_rbegin( size_type k ){ return( reverse_iterator( z_end( k ) ) ); }

	/// @brief Z���������Œ肵�����́CXY����(Axial)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator z_rbegin( size_type k ) const { return( const_reverse_iterator( z_end( k )) ); }

	/// @brief Z���������Œ肵�����́CXY����(Axial)�̖������w���t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator z_rend( size_type k ){ return( reverse_iterator( z_begin( k ) ) ); }

	/// @brief Z���������Œ肵�����́CXY����(Axial)�̖������w���R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator z_rend( size_type k ) const { return( const_reverse_iterator( z_begin( k ) ) ); }


public:
	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �R�s�[���ƃR�s�[��Ńf�[�^�^�iarray3< data > �� data�j���قȂ�ꍇ�̑�����s���D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const array3& operator =( const array3< TT, AAlocator > &o )
	{
		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = size3_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
			size3_ = o.size3( );
		}

		reso3_ = o.reso3( );

		return( *this );
	}


	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	const array3& operator =( const array3 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = size3_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
			size3_ = o.size3( );
		}

		reso3_ = o.reso3( );

		return( *this );
	}


// �v�f�ւ̃A�N�Z�X
protected:
	/// @brief index �Ŏ������ʒu�̗v�f�̃|�C���^��Ԃ�
	//! 
	//! @param[in] i �c �R���e�i����X�������̈ʒu
	//! @param[in] j �c �R���e�i����Y�������̈ʒu
	//! @param[in] k �c �R���e�i����Z�������̈ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	pointer paccess( size_type i, size_type j, size_type k )
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}

	/// @brief index �Ŏ������ʒu�̗v�f�� const �|�C���^��Ԃ�
	//! 
	//! @param[in] i �c �R���e�i����X�������̈ʒu
	//! @param[in] j �c �R���e�i����Y�������̈ʒu
	//! @param[in] k �c �R���e�i����Z�������̈ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	const_pointer paccess( size_type i, size_type j, size_type k ) const
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}

public:
	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j, k ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//! @param[in] k �c �R���e�i����Z�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j, k ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//! @param[in] k �c �R���e�i����Z�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j, k ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//! @param[in] k �c �R���e�i����Z�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j, k ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//! @param[in] k �c �R���e�i����Z�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0�C�𑜓x 1.0 �~ 1.0 �~ 1.0 �̃R���e�i���쐬����
	array3( ) : base( ), size3_( 0 ), size2_( 0 ), size1_( 0 ), reso3_( 1.0 ) {}

	/// @brief �A���P�[�^ a �̃R�s�[�𗘗p����
	explicit array3( const Allocator &a ) : base( a ), size3_( 0 ), size2_( 0 ), size1_( 0 ), reso3_( 1.0 ) {}

	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�f�t�H���g�l�ŗv�f������������
	array3( size_type num1, size_type num2, size_type num3 ) : base( num1 * num2, num3 ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �~ r3 �ɐݒ肷��
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3 ) : base( num1 * num2, num3, r1, r2 ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�g�p����A���P�[�^�� a �ɐݒ肷��
	array3( size_type num1, size_type num2, size_type num3, const Allocator &a ) : base( num1 * num2, num3, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �~ r3 �ɐݒ肵�C�g�p����A���P�[�^�� a �ɐݒ肷��
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const Allocator &a ) : base( num1 * num2, num3, r1, r2, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}


	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� 1.0 �~ 1.0 �~ 1.0 �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array3( size_type num1, size_type num2, size_type num3, const value_type &val ) : base( num1 * num2, num3, val ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �~ r3 �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val ) : base( num1 * num2, num3, r1, r2, val ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �~ r3 �ɐݒ肵�C�g�p����A���P�[�^�� a �ɐݒ肷��
	array3( size_type num1, size_type num2, size_type num3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, val, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �~ r3�C�g�p����A���P�[�^�� a �ɐݒ肵�C�S�v�f�� val �ŏ���������
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, r1, r2, val, a ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}



	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬����iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array3( size_type num1, size_type num2, size_type num3, pointer ptr, size_type mem_available ) : base( num1 * num2, num3, ptr, mem_available ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( 1.0 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �~ r3 �ɐݒ肷��iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, pointer ptr, size_type mem_available ) : base( num1 * num2, num3, r1, r2, ptr, mem_available ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �~ num3 �̃R���e�i���쐬���C�𑜓x�� r1 �~ r2 �~ r3�C�l val �ŏ���������iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val, pointer ptr, size_type mem_available ) : base( num1 * num2, num3, r1, r2, val, ptr, mem_available ), size3_( num3 ), size2_( num2 ), size1_( num1 ), reso3_( r3 )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}



	/// @brief ���� array3 �z��ŗv�f�̌^���قȂ���̂��瓯���v�f���̔z����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	array3( const array3< TT, AAlocator > &o ) : base( o ), size3_( o.size3( ) ), size2_( o.size2( ) ), size1_( o.size1( ) ), reso3_( o.reso3( ) )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}

	/// @brief ���� array3 �z��œ����v�f�^�̂��̂�p���ď���������
	array3( const array3< T, Allocator > &o ) : base( o ), size3_( o.size3_ ), size2_( o.size2_ ), size1_( o.size1_ ), reso3_( o.reso3_ )
	{
		if( base::empty( ) ) size1_ = size2_ = size3_ = 0;
	}
};




/// @brief �摜�̂ӂ��Ƀ}�[�W�����������z��
//! 
//! 1�E2�E3�����摜�Ƀ}�[�W�����������邽�߂̊�{�N���X
//! �i@ref mist/mist.h ���C���N���[�h����j
//! 
//! @param Array �c 1�E2�E3�����摜�N���X���w�肷��
//! 
template < class Array >
class marray : public Array
{
public:
	typedef typename Array::allocator_type allocator_type;		///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Array::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Array::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Array::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename Array::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Array::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Array::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Array::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

	typedef typename Array::iterator iterator;								///< @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef typename Array::const_iterator const_iterator;					///< @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename Array::reverse_iterator reverse_iterator;				///< @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef typename Array::const_reverse_iterator const_reverse_iterator;	///< @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��


protected:
	typedef Array base;						///< @brief ���N���X
	size_type margin1_;						///< @brief X�������̃}�[�W��
	size_type margin2_;						///< @brief Y�������̃}�[�W��
	size_type margin3_;						///< @brief Z�������̃}�[�W��

public:
	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �v�f���� num1 �ɕύX���C�S�Ă̗v�f���f�t�H���g�l�ŏ���������D
	//! 
	//! @param[in] num1 �c ���T�C�Y��̑S�v�f��
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num1 )
	{
		return( base::resize( num1 + margin1_ * 2 ) );
	}


	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �v�f���� num1 �~ num2 �ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f���f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! @param[in] num2 �c ���T�C�Y���Y�������̗v�f��
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num1, size_type num2 )
	{
		return( base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2 ) );
	}


	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �v�f���� num1 �~ num2 �~ num3 �ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f���f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! @param[in] num2 �c ���T�C�Y���Y�������̗v�f��
	//! @param[in] num3 �c ���T�C�Y���Z�������̗v�f��
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num1, size_type num2, size_type num3 )
	{
		return( base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, num3 + margin3_ * 2 ) );
	}



	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! @attention �ǂ��炩�̃R���e�i���O���������𗘗p���Ă���ꍇ�́C�X���b�v�͕K�����s����
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( marray &a )
	{
		if( base::swap( a ) )
		{
			size_type tmp = margin1_;
			margin1_ = a.margin1_;
			a.margin1_ = tmp;

			tmp = margin2_;
			margin2_ = a.margin2_;
			a.margin2_ = tmp;

			tmp = margin3_;
			margin3_ = a.margin3_;
			a.margin3_ = tmp;

			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief �R���e�i�̗v�f����ɂ���
	//! 
	//! �R���e�i�Ɋi�[����Ă���f�[�^��S�č폜���C�R���e�i����i�v�f��0�j�ɂ���
	//! 
	void clear( )
	{
		base::clear( );
		margin1_ = margin2_ = margin3_ = 0;
	}


	/// @brief �R���e�i���̃}�[�W�������̃f�[�^�v�f���w�肳�ꂽ�l�ŏ���������
	//! 
	//! �}�[�W�������̗v�f��l val �ŏ���������D
	//! 
	//! @param[in] val �c �v�f������������l
	//! 
	void fill_margin( const value_type &val = value_type( ) )
	{
		if( !base::empty( ) )
		{
			base &o = *this;

			size_type i, j, k;
			for( k = 0 ; k < margin3( ) ; k++ )
			{
				for( j = 0 ; j < o.size2( ) ; j++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
			for( k = o.size3( ) - margin3( ) ; k < o.size3( ) ; k++ )
			{
				for( j = 0 ; j < o.size2( ) ; j++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}

			for( j = 0 ; j < margin2( ) ; j++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
			for( j = o.size2( ) - margin2( ) ; j < o.size2( ) ; j++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( i = 0 ; i < o.size1( ) ; i++ )
					{
						o( i, j, k ) = val;
					}
				}
			}

			for( i = 0 ; i < margin1( ) ; i++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( j = 0 ; j < o.size2( ) ; j++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
			for( i = o.size1( ) - margin1( ) ; i < o.size1( ) ; i++ )
			{
				for( k = 0 ; k < o.size3( ) ; k++ )
				{
					for( j = 0 ; j < o.size2( ) ; j++ )
					{
						o( i, j, k ) = val;
					}
				}
			}
		}
	}


	size_type size1( ) const { return( base::empty( ) ? 0 : base::size1( ) - 2 * margin1_ ); }	///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size2( ) const { return( base::empty( ) ? 0 : base::size2( ) - 2 * margin2_ ); }	///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size3( ) const { return( base::empty( ) ? 0 : base::size3( ) - 2 * margin3_ ); }	///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�

	size_type width( ) const { return( size1( ) ); }		///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const { return( size2( ) ); }		///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type depth( ) const { return( size3( ) ); }		///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�

	size_type margin1( ) const { return( margin1_ ); }		///< @brief X�������̃}�[�W������Ԃ�
	size_type margin2( ) const { return( margin2_ ); }		///< @brief Y�������̃}�[�W������Ԃ�
	size_type margin3( ) const { return( margin3_ ); }		///< @brief Z�������̃}�[�W������Ԃ�

private:
	/// @brief array �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �}�[�W�������ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& copy( const array< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array1 �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �}�[�W�������ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& copy( const array1< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array2 �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �}�[�W�������ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& copy( const array2< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		for( size_type j = 0 ; j < h ; j++ )
		{
			for( size_type i = 0 ; i < w ; i++ )
			{
				( *this )( i, j ) = static_cast< value_type >( o( i, j ) );
			}
		}
		return( *this );
	}


	/// @brief array3 �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �}�[�W�������ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& copy( const array3< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		size_type d = depth( )  < o.depth( )  ? depth( )  : o.depth( );
		for( size_type k = 0 ; k < d ; k++ )
		{
			for( size_type j = 0 ; j < h ; j++ )
			{
				for( size_type i = 0 ; i < w ; i++ )
				{
					( *this )( i, j, k ) = static_cast< value_type >( o( i, j, k ) );
				}
			}
		}
		return( *this );
	}

public:
	/// @brief �����^�� marray �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� marray �R���e�i
	//! 
	//! @return �������g
	//! 
	const marray& operator =( const marray &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		margin1_ = o.margin1_;
		margin2_ = o.margin2_;
		margin3_ = o.margin3_;

		return( *this );
	}


	/// @brief �v�f�̌^���قȂ� array �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array< T, Allocator > &o )
	{
		if( base::resize( o.size( ) + margin1_ * 2 ) )
		{
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief �v�f�̌^���قȂ� array1 �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array1 �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array1< T, Allocator > &o )
	{
		if( base::resize( o.size( ) + margin1_ * 2 ) )
		{
			reso1( o.reso1( ) );
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief �v�f�̌^���قȂ� array2 �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array2 �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array2< T, Allocator > &o )
	{
		if( base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2 ) )
		{
			reso1( o.reso1( ) );
			reso2( o.reso2( ) );
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief �v�f�̌^���قȂ� array3 �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array3 �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array3< T, Allocator > &o )
	{
		if( base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2, o.size3( ) + margin3_ * 2 ) )
		{
			reso1( o.reso1( ) );
			reso2( o.reso2( ) );
			reso3( o.reso3( ) );
			return( copy( o ) );
		}
		else
		{
			return( *this );
		}
	}

// �v�f�ւ̃A�N�Z�X
public:
	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( difference_type index )
	{
		return( base::at( index + margin1_ ) );
	}

	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( difference_type i, difference_type j )
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	/// @param[in] k �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( difference_type i, difference_type j, difference_type k )
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( difference_type index ) const
	{
		return( base::at( index + margin1_ ) );
	}

	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( difference_type i, difference_type j ) const
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	/// @param[in] k �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator []( difference_type index )
	{
		return( base::operator []( index ) );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( difference_type index )
	{
		return( base::operator ()( index + margin1_ ) );
	}

	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( difference_type i, difference_type j )
	{
		return( base::operator ()( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	/// @param[in] k �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( difference_type i, difference_type j, difference_type k )
	{
		return( base::operator ()( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	const_reference operator []( difference_type index ) const
	{
		return( base::operator []( index ) );
	}

	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( difference_type index ) const
	{
		return( base::operator ()( index + margin1_ ) );
	}

	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( difference_type i, difference_type j ) const
	{
		return( base::operator ()( i + margin1_, j + margin2_ ) );
	}

	/// @brief ( i, j, k ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	/// @param[in] i �c �R���e�i����X�������̈ʒu
	/// @param[in] j �c �R���e�i����Y�������̈ʒu
	/// @param[in] k �c �R���e�i����Y�������̈ʒu
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::operator ()( i + margin1_, j + margin2_, k + margin3_ ) );
	}


public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0�C�}�[�W�� 0 �̃R���e�i���쐬����
	marray( ) : base( ), margin1_( 0 ), margin2_( 0 ), margin3_( 0 ) {}

	/// @brief �}�[�W�� margin �̃R���e�i���쐬����
	marray( size_type margin ) : base( ), margin1_( margin ), margin2_( margin ), margin3_( margin ) {}

	/// @brief �R�s�[�R���X�g���N�^
	marray( const marray &o ) : base( o ), margin1_( o.margin1( ) ), margin2_( o.margin2( ) ), margin3_( o.margin3( ) ) {}



	/// @brief �}�[�W�� margin �̃R���e�i���쐬����
	marray( size_type w, size_type margin ) : base( w + margin * 2 ), margin1_( margin ), margin2_( 0 ), margin3_( 0 ) {}

	/// @brief �}�[�W�� margin �̃R���e�i���쐬����
	marray( size_type w, size_type h, size_type margin ) : base( w + margin * 2, h + margin * 2 ), margin1_( margin ), margin2_( margin ), margin3_( 0 ) {}

	/// @brief �}�[�W�� margin �̃R���e�i���쐬����
	marray( size_type w, size_type h, size_type d, size_type margin ) : base( w + margin * 2, h + margin * 2, d + margin * 2 ), margin1_( margin ), margin2_( margin ), margin3_( margin ) {}




	/// @brief array �z�� o �̔z��̑傫���ƁCX�������̃}�[�W�� margin1 ��p���ď��������C�S�v�f�� val �ŏ���������
	template < class T, class Allocator >
	marray( const array< T, Allocator > &o, size_type margin1, const value_type &val = value_type( 0 ) )
		: base( o.size( ) + margin1 * 2 ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array1 �z�� o �̔z��̑傫���ƁCX�������̃}�[�W�� margin1 ��p���ď��������C�S�v�f�� val �ŏ���������
	template < class T, class Allocator >
	marray( const array1< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size( ) + margin1 * 2, o.reso1( ) ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array2 �z�� o �̔z��̑傫���ƁCX�������̃}�[�W�� margin1�CY�������̃}�[�W�� margin2 ��p���ď��������C�S�v�f�� val �ŏ���������
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, size_type margin2, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief array2 �z�� o �̔z��̑傫���ƁCX�������̃}�[�W�� margin1�CY�������̃}�[�W�� margin2 ��p���ď��������C�S�v�f�� val �ŏ���������
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, size_type margin2, size_type margin3, const value_type &val )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array3 �z�� o �̔z��̑傫���ƁCX�������̃}�[�W�� margin1�CY�������̃}�[�W�� margin2�CZ�������̃}�[�W�� margin3 ��p���ď��������C�S�v�f�� val �ŏ���������
	//! 
	template < class T, class Allocator >
	marray( const array3< T, Allocator > &o, size_type margin1, size_type margin2, size_type margin3, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.size3( ) + margin3 * 2, o.reso1( ), o.reso2( ), o.reso3( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( margin3 )
	{
		fill_margin( val );
		copy( o );
	}


	/// @brief array2 �z�� o �̔z��̑傫���ƁCX�EY�������̃}�[�W�� margin1 ��p���ď��������C�S�v�f�� val �ŏ���������
	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin1 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin1 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	/// @brief array3 �z�� o �̔z��̑傫���ƁCX�EY�EZ�������̃}�[�W�� margin1 ��p���ď��������C�S�v�f�� val �ŏ���������
	template < class T, class Allocator >
	marray( const array3< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin1 * 2, o.size3( ) + margin1 * 2, o.reso1( ), o.reso2( ), o.reso3( ) ), margin1_( margin1 ), margin2_( margin1 ), margin3_( margin1 )
	{
		fill_margin( val );
		copy( o );
	}
};





/// @brief �P�ӂ�2�̎w����̃o�b�t�@���������摜���쐬����
//! 
//! 1�E2�E3�����摜�̂P�ӂ�2�̎w����ɂ��邽�߂̊�{�N���X
//! 
//! @param Array �c 1�E2�E3�����摜�N���X���w�肷��
//! 
template < class Array >
class buffered_array : public Array
{
public:
	typedef typename Array::allocator_type allocator_type;		///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Array::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Array::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Array::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename Array::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Array::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Array::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Array::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

	typedef typename Array::iterator iterator;								///< @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef typename Array::const_iterator const_iterator;					///< @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename Array::reverse_iterator reverse_iterator;				///< @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef typename Array::const_reverse_iterator const_reverse_iterator;	///< @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��

protected:
	typedef Array base;						///< @brief ���N���X
	size_type size1_;						///< @brief X�������̗v�f��
	size_type size2_;						///< @brief Y�������̗v�f��
	size_type size3_;						///< @brief Z�������̗v�f��

	/// @brief �w�肳�ꂽ���l�ȏ�̐������ōł��ŏ���2�̎w������v�Z���C���̎w������Ԃ�
	//! 
	//! @param[in] v �c �C�ӂ̔񕉂̐���
	//!
	/// @return ���͂��ꂽ�������𒴂���ł��ŏ���2�̎w����̎w����
	//! 
	inline size_t floor_square_index( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( i );
			}
			_2 *= 2;
		}

		return( 0 );
	}

	/// @brief �w�肳�ꂽ���l�ȏ�̐������ōł��ŏ���2�̎w������v�Z����
	//! 
	//! @param[in] v �c �C�ӂ̔񕉂̐���
	//!
	/// @return ���͂��ꂽ�������𒴂���ł��ŏ���2�̎w�����\������
	//! 
	inline size_t floor_square( size_t v )
	{
		if( v == 0 )
		{
			return( 0 );
		}

		for( size_t i = 1, _2 = 2 ; i < 64 ; i++ )
		{
			if( v <= _2 )
			{
				return( _2 );
			}
			_2 *= 2;
		}

		return( 0 );
	}

public:
	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �e���̗v�f�����w�肳�ꂽ�����ȏ�ōŏ���2�̎w����ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f���f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! 
	bool resize( size_type num1 )
	{
		if( base::resize( floor_square( num1 ) ) )
		{
			size1_ = num1;
			return( true );
		}
		else
		{
			size1_ = 0;
			return( false );
		}
	}


	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �e���̗v�f�����w�肳�ꂽ�����ȏ�ōŏ���2�̎w����ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f���f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! @param[in] num2 �c ���T�C�Y���Y�������̗v�f��
	//! 
	bool resize( size_type num1, size_type num2 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size1_ = num1;
		size2_ = num2;
		s1 = s1 > s2 ? s1 : s2;
		if( !base::resize( s1, s1 ) )
		{
			size1_ = size2_ = 0;
			return( false );
		}

		return( true );
	}


	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �e���̗v�f�����w�肳�ꂽ�����ȏ�ōŏ���2�̎w����ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f���f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! @param[in] num2 �c ���T�C�Y���Y�������̗v�f��
	//! @param[in] num3 �c ���T�C�Y���Z�������̗v�f��
	//! 
	bool resize( size_type num1, size_type num2, size_type num3 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size_type s3 = floor_square( num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
		s1 = s1 > s2 ? s1 : s2;
		s1 = s1 > s3 ? s1 : s3;

		if( !base::resize( s1, s1, s1 ) )
		{
			size1_ = size2_ = size3_ = 0;
			return( false );
		}

		return( true );
	}


	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! @attention �ǂ��炩�̃R���e�i���O���������𗘗p���Ă���ꍇ�́C�X���b�v�͕K�����s����
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( buffered_array &a )
	{
		if( base::swap( a ) )
		{
			size_type tmp = size1_;
			size1_ = a.size1_;
			a.size1_ = tmp;

			tmp = size2_;
			size2_ = a.size2_;
			a.size2_ = tmp;

			tmp = size3_;
			size3_ = a.size3_;
			a.size3_ = tmp;

			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief �R���e�i�̗v�f����ɂ���
	//! 
	//! �R���e�i�Ɋi�[����Ă���f�[�^��S�č폜���C�R���e�i����i�v�f��0�j�ɂ���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}

	size_type size1( ) const { return( size1_ > 0 ? size1_ : base::size1( ) ); }	///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size2( ) const { return( size2_ > 0 ? size2_ : base::size2( ) ); }	///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size3( ) const { return( size3_ > 0 ? size3_ : base::size3( ) ); }	///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type width( ) const { return( size1( ) ); }								///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const { return( size2( ) ); }								///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type depth( ) const { return( size3( ) ); }								///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�


private:
	/// @brief array �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �]���ȉ摜�����ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array1 �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �]���ȉ摜�����ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array1< T, Allocator > &o )
	{
		size_type length = base::size( ) < o.size( ) ? base::size( ) : o.size( );
		for( size_type i = 0 ; i < length ; i++ )
		{
			( *this )[ i ] = static_cast< value_type >( o[ i ] );
		}
		return( *this );
	}


	/// @brief array2 �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �]���ȉ摜�����ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array2< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		for( size_type j = 0 ; j < h ; j++ )
		{
			for( size_type i = 0 ; i < w ; i++ )
			{
				( *this )( i, j ) = static_cast< value_type >( o( i, j ) );
			}
		}
		return( *this );
	}


	/// @brief array3 �R���e�i����f�[�^���R�s�[����
	//! 
	//! @attention �]���ȉ摜�����ɂ͕ύX�͉������Ȃ�
	//!
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& copy( const array3< T, Allocator > &o )
	{
		size_type w = width( )  < o.width( )  ? width( )  : o.width( );
		size_type h = height( ) < o.height( ) ? height( ) : o.height( );
		size_type d = depth( )  < o.depth( )  ? depth( )  : o.depth( );
		for( size_type k = 0 ; k < d ; k++ )
		{
			for( size_type j = 0 ; j < h ; j++ )
			{
				for( size_type i = 0 ; i < w ; i++ )
				{
					( *this )( i, j, k ) = static_cast< value_type >( o( i, j, k ) );
				}
			}
		}
		return( *this );
	}


public:
	/// @brief �����^�� buffered_array �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� buffered_array �R���e�i
	//! 
	//! @return �������g
	//! 
	const buffered_array& operator =( const buffered_array &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = size3_ = 0;
		}
		else
		{
			size1_ = o.size1_;
			size2_ = o.size2_;
			size3_ = o.size3_;
		}

		return( *this );
	}


	/// @brief �v�f�̌^���قȂ� array �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array< T, Allocator > &o )
	{
		resize( o.size( ) );
		return( copy( o ) );
	}


	/// @brief �v�f�̌^���قȂ� array1 �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array1 �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array1< T, Allocator > &o )
	{
		resize( o.size( ) );
		reso1( o.reso1( ) );
		return( copy( o ) );
	}


	/// @brief �v�f�̌^���قȂ� array2 �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array2 �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array2< T, Allocator > &o )
	{
		resize( o.size1( ), o.size2( ) );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		return( copy( o ) );
	}


	/// @brief �v�f�̌^���قȂ� array3 �R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���� array3 �R���e�i
	//! 
	//! @return �������g
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array3< T, Allocator > &o )
	{
		resize( o.size1( ), o.size2( ), o.size3( ) );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		reso3( o.reso3( ) );
		return( copy( o ) );
	}

public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0 �̃R���e�i���쐬����
	buffered_array( ) : base( ), size1_( 0 ), size2_( 0 ), size3_( 0 ) {}

	/// @brief�R�s�[�R���X�g���N�^
	buffered_array( const buffered_array &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) ) {}

	/// @brief array �z�� o ��p���ď��������C��ӂ̒�����2�̎w����ɂ���
	template < class T, class Allocator >
	buffered_array( const array< T, Allocator > &o )
		: base( floor_square( o.size( ) ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	/// @brief array1 �z�� o ��p���ď��������C��ӂ̒�����2�̎w����ɂ���
	template < class T, class Allocator >
	buffered_array( const array1< T, Allocator > &o )
		: base( floor_square( o.size( ) ), o.reso1( ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	/// @brief array2 �z�� o ��p���ď��������C��ӂ̒�����2�̎w����ɂ���
	template < class T, class Allocator >
	buffered_array( const array2< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), o.reso1( ), o.reso2( ) ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( 0 )
	{
		copy( o );
	}

	/// @brief array3 �z�� o ��p���ď��������C��ӂ̒�����2�̎w����ɂ���
	template < class T, class Allocator >
	buffered_array( const array3< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), floor_square( o.size3( ) ), o.reso1( ), o.reso2( ), o.reso3( ) ),
			size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) )
	{
		copy( o );
	}
};


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array< T, Allocator > &a )
{
	typename array< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[ i ];
		if( i != a.size1( ) - 1 ) out << ", ";
	}

	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array1 �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array1< T, Allocator > &a )
{
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[ i ];
		if( i != a.size1( ) - 1 ) out << ", ";
	}

	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array2 �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array2< T, Allocator > &a )
{
	typename array2< T, Allocator >::size_type i, j;
	for( j = 0 ; j < a.size2( ) ; j++ )
	{
		if( j != 0 )
		{
			out << std::endl;
		}
		for( i = 0 ; i < a.size1( ) ; i++ )
		{
			out << a( i, j );
			if( i != a.size1( ) - 1 ) out << ", ";
		}
	}

	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array3 �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! ----- separator -----
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array3< T, Allocator > &a )
{
	typename array3< T, Allocator >::size_type i, j, k;
	for( k = 0 ; k < a.size3( ) ; k++ )
	{
		for( j = 0 ; j < a.size2( ) ; j++ )
		{
			for( i = 0 ; i < a.size1( ) ; i++ )
			{
				out << a( i, j, k );
				if( i != a.size1( ) - 1 ) out << ", ";
			}
			out << std::endl;
		}
		if( k != a.size3( ) - 1 )
		{
			out << "----- separator -----";
		}
		out << std::endl;
	}

	return( out );
}



// �e�R���e�i�ɑ΂���摜�ԉ��Z

#if defined(_ARRAY_BIND_OPERATION_SUPPORT_) && _ARRAY_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array.h"
#endif

#if defined(_ARRAY1_BIND_OPERATION_SUPPORT_) && _ARRAY1_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array1.h"
#endif

#if defined(_ARRAY2_BIND_OPERATION_SUPPORT_) && _ARRAY2_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array2.h"
#endif

#if defined(_ARRAY3_BIND_OPERATION_SUPPORT_) && _ARRAY3_BIND_OPERATION_SUPPORT_ != 0
#include "operator/operator_array3.h"
#endif


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_H__
