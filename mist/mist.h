/****************************************************************************************************************************
**       MIST ( Media Integration Standard Toolkit )
**
**        We defined following template classes.
**
**          array  : template class of one dimensionl array with STL support.
**          array1 : template class of one dimensionl array containing resolution.
**          array2 : template class of two dimensionl array containing pixel resolution.
**          array3 : template class of three dimensionl array containing voxel resolution.
**          matrix : template class of matrix with its operation, and impremented using expression template teqnique.
**          vector : template class of vector with its operation, and impremented using expression template teqnique.
**
**
**         We developed these programs since 2003/09/05.
**
**             $LastChangedDate$
**             $LastChangedRevision$
**             $LastChangedBy$
**             $HeadURL$
**
**
**              Copyright MIST Project Team.
**                    All Rights Reserved.
**
****************************************************************************************************************************/



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

#include "config/mist_conf.h"
#include "config/mist_alloc.h"
#include "config/iterator.h"


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



/// @brief �v�f�����ς�1�����z��
//! 
//! �S�Ă�MIST�R���e�i�̊�{�ƂȂ�N���X
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
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;

private:
	mist_allocator< T, Allocator > allocator_;		///< @brief �e�R���e�i�ŗ��p����A���P�[�^�I�u�W�F�N�g
	size_type size_;								///< @brief �R���e�i�Ɋi�[����Ă���v�f��

protected:
	T* data_;	///< @brief �R���e�i�ň����f�[�^�̐擪��\���|�C���^

public:
	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �R���e�i�̃T�C�Y�� num �ɕύX����D
	//! ���̃R���e�i�T�C�Y�ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �S�Ă̗v�f�̒l�̓f�t�H���g�R���X�g���N�^�ŏ����������D
	//! �܂��C�g�ݍ��݌^�iint �� double �Ȃǁj�̏ꍇ�́C�S�Ă̗v�f��0�ŏ���������D
	//! 
	//! @param[in] num  �c ���T�C�Y��̃R���e�i���̗v�f��
	//! 
	void resize( size_type num )
	{
		if( size_ < num )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = num;
			data_ = allocator_.allocate_objects( size_ );
		}
		else if( size_ > num )
		{
			data_ = allocator_.trim_objects( data_, size_, num );
			size_ = num;
		}
	}


	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �v�f���� num �ɕύX���C�S�Ă̗v�f��l val �ŏ���������D
	//! 
	//! @param[in] num �c ���T�C�Y��̃R���e�i���̗v�f��
	//! @param[in] val �c ���T�C�Y��Ɋe�v�f������������l
	//! 
	void resize( size_type num, const value_type &val )
	{
		if( size_ < num )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = num;
			data_ = allocator_.allocate_objects( size_, val );
		}
		else if( size_ > num )
		{
			data_ = allocator_.trim_objects( data_, size_, num, val );
			size_ = num;
		}
	}


	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	void swap( array &a )
	{
		size_type _dmy_size = size_;
		size_ = a.size_;
		a.size_ = _dmy_size;

		value_type *dmy_data_ = data_;
		data_ = a.data_;
		a.data_ = dmy_data_;
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
	size_type size2( ) const { return( 1 ); }		//< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j
	size_type size3( ) const { return( 1 ); }		//< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j
	size_type width( ) const { return( size_ ); }	///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const { return( 1 ); }		//< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j
	size_type depth( ) const { return( 1 ); }		//< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j

	double reso1( double r1 ){ return( 1.0 ); }		//< @brief X�������̉𑜓x��Ԃ�
	double reso1( ) const { return( 1.0 ); }		//< @brief X�������̉𑜓x��ݒ肷��
	double reso2( double r2 ){ return( 1.0 ); }		//< @brief Y�������̉𑜓x��Ԃ�
	double reso2( ) const { return( 1.0 ); }		//< @brief Y�������̉𑜓x��ݒ肷��
	double reso3( double r3 ){ return( 1.0 ); }		//< @brief Z�������̉𑜓x��Ԃ�
	double reso3( ) const { return( 1.0 ); }		//< @brief Z�������̉𑜓x��ݒ肷��j


	/// @brief �R���e�i���̗v�f����߂�f�[�^�ʂ��o�C�g�P�ʂŕԂ�
	size_type byte( ) const { return( size_ * sizeof( value_type ) ); }


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
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! @return       �c �������g
	//! 
	template < class TT, class AAlocator >
	const array& operator =( const array< TT, AAlocator >  &o )
	{
		if( size_ > o.size( ) )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size( ) );
			size_ = o.size( );
		}
		else if( size_ < o.size( ) )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = o.size( );
			data_ = allocator_.allocate_objects( size_ );
		}
		for( size_type i = 0 ; i < size_ ; i++ ) data_[i] = static_cast< value_type >( o[i] );

		return( *this );
	}

	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! @return       �c �������g
	//! 
	const array& operator =( const array  &o )
	{
		if( this == &o ) return( *this );

		if( size_ > o.size_ )
		{
			data_ = allocator_.trim_objects( data_, size_, o.size_ );
			size_ = o.size_;
		}
		else if( size_ < o.size_ )
		{
			allocator_.deallocate_objects( data_, size_ );
			size_ = o.size_;
			data_ = allocator_.allocate_objects( size_ );
		}
		allocator_.copy_objects( o.data_, size_, data_ );

		return( *this );
	}

// �R���e�i���̗v�f�ւ̃A�N�Z�X���Z�q
protected:
	/// @brief index �Ŏ������ʒu�̗v�f�̃|�C���^��Ԃ�
	//! @param[in] index  �c �R���e�i���̗v�f�ʒu
	//! @return           �c �w�肳�ꂽ�v�f�������|�C���^
	pointer paccess( size_type index )
	{
		return( data_ + index );
	}
	/// @brief index �Ŏ������ʒu�̗v�f�� const �|�C���^��Ԃ�
	//! @param[in] index  �c �R���e�i���̗v�f�ʒu
	//! @return           �c �w�肳�ꂽ�v�f������ const �|�C���^
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
	/// @return          �c �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	/// @return          �c �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	/// @return          �c �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	/// @return          �c �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( size_type index, size_type dmy1 = 0, size_type dmy2 = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	/// @return          �c �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator []( size_type index )
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	/// @param[in] dmy1  �c �g�p���Ȃ�
	/// @param[in] dmy2  �c �g�p���Ȃ�
	/// @return          �c �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator []( size_type index ) const
	{
		_CHECK_ACCESS_VIOLATION1_( index )
		return( data_[index] );
	}

public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0 �̃R���e�i���쐬����
	array( ) : allocator_( ), size_( 0 ), data_( NULL ){}

	explicit array( const Allocator &a ) : allocator_( a ), size_( 0 ), data_( NULL ){}


	/// @brief �v�f�� num �̃R���e�i���쐬���C�f�t�H���g�l�ŗv�f������������
	explicit array( size_type num ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
	}
	/// @brief �v�f�� num �̃R���e�i���쐬���C�A���P�[�^ a �̃R�s�[�𗘗p����
	array( size_type num, const Allocator &a ) : allocator_( a ), size_( num ), data_( allocator_.allocate_objects( size_ ) )
	{
		data_ = allocator_.allocate_objects( size_ );
	}

	/// @brief �v�f�� num �̃R���e�i���쐬���C�l val �ŏ���������
	array( size_type num, const value_type &val ) : allocator_( ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
	}
	/// @brief �v�f�� num �̃R���e�i���쐬���C�l val �ŏ��������C�A���P�[�^ a �̃R�s�[�𗘗p����
	array( size_type num, const value_type &val, const Allocator &a ) : allocator_( a ), size_( num ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_, val );
	}

	/// @brief �C�e���[�^ s �� e �͈̔͂̒l��p���āC�z�������������
	array( const_iterator s, const_iterator e ) : allocator_( ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
	}
	/// @brief �C�e���[�^ s �� e �͈̔͂̒l��p���āC�z������������C�A���P�[�^ a �̃R�s�[�𗘗p����
	array( const_iterator s, const_iterator e, const Allocator &a ) : allocator_( a ), size_( e - s ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( s, e );
	}

	/// @brief ���� array �z��ŗv�f�̌^���قȂ���̂��瓯���v�f���̔z����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	array( const array< TT, AAlocator > &o ) : allocator_( ), size_( o.size( ) ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		for( size_type i = 0 ; i < size_ ; i++ ) data_[i] = static_cast< value_type >( o[i] );
	}

	/// @brief ���� array �z��œ����v�f�^�̂��̂�p���ď���������
	array( const array< T, Allocator > &o ) : allocator_( o.allocator_ ), size_( o.size_ ), data_( NULL )
	{
		data_ = allocator_.allocate_objects( size_ );
		allocator_.copy_objects( o.data_, size_, data_ );
	}

	/// @brief �R���e�i�����p���Ă��郊�\�[�X��S�ĊJ������
	~array( )
	{
		clear( );
	}
};



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
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
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;


protected:
	typedef array< T, Allocator > base;
	double reso1_;

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	double reso1( double r1 ){ return( reso1_ = r1 ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	double reso1( ) const { return( reso1_ ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void reso( double r1 ){ reso1_ = r1; }


/************************************************************************************************************
**
**      X�����ɑ΂��鏇�����E�t�����̔����q
**
************************************************************************************************************/

	/// @brief �������̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator x_begin( ){ return( base::begin( ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_iterator x_begin( ) const { return( base::begin( ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator x_end( ){ return( base::end( ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_iterator x_end( ) const { return( base::end( ) ); }



	/// @brief �t�����̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator x_rbegin( ){ return( base::rbegin( ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reverse_iterator x_rbegin( ) const { return( base::rbegin( ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator x_rend( ){ return( base::rend( ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reverse_iterator x_rend( ) const { return( base::rend( )  ); }


public: // �z��ɑ΂���Z�p���Z

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void swap( array1 &a )
	{
		base::swap( a );
		double dmy_reso1_ = reso1_;
		reso1_ = a.reso1_;
		a.reso1_ = dmy_reso1_;
	}

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class TT, class AAlocator >
	const array1& operator =( const array1< TT, AAlocator > &o )
	{
		base::operator =( o );
		reso1_ = o.reso1( );

		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const array1& operator =( const array1 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		reso1_ = o.reso1_;

		return( *this );
	}

public:
	// �\�z
	array1( ) : base( ), reso1_( 1.0 ) {}
	explicit array1( const Allocator &a ) : base( a ), reso1_( 1.0 ) {}

	explicit array1( size_type num ) : base( num ), reso1_( 1.0 ) {}
	array1( size_type num, double r1 ) : base( num ), reso1_( r1 ) {}
	array1( size_type num, const Allocator &a ) : base( num, a ), reso1_( 1.0 ) {}
	array1( size_type num, double r1, const Allocator &a ) : base( num, a ), reso1_( r1 ) {}

	array1( size_type num, double r1, const value_type &val ) : base( num, val ), reso1_( r1 ) {}
	array1( size_type num, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( 1.0 ) {}
	array1( size_type num, double r1, const value_type &val, const Allocator &a ) : base( num, val, a ), reso1_( r1 ) {}

	template < class TT, class AAlocator >
	explicit array1( const array1< TT, AAlocator > &o ) : base( o ), reso1_( o.reso1( ) ) {}

	array1( const array1< T, Allocator > &o ) : base( o ), reso1_( o.reso1_ ) {}
};



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
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
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;


protected:
	typedef array1< T, Allocator > base;
	size_type size1_;
	size_type size2_;
	double reso2_;

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2 )
	{
		base::resize( num1 * num2 );
		size1_ = num1;
		size2_ = num2;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, const value_type &val )
	{
		base::resize( num1 * num2, val );
		size1_ = num1;
		size2_ = num2;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void swap( array2 &a )
	{
		base::swap( a );

		double dmy_reso2_ = reso2_;
		reso2_ = a.reso2_;
		a.reso2_ = dmy_reso2_;

		size_type _dmy_size1 = size1_;
		size_type _dmy_size2 = size2_;
		size1_ = a.size1_;
		size2_ = a.size2_;
		a.size1_ = _dmy_size1;
		a.size2_ = _dmy_size2;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	size_type size1( )  const { return( size1_ ); }
	size_type size2( )  const { return( size2_ ); }
	size_type width( )  const { return( size1_ ); }
	size_type height( ) const { return( size2_ ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	double reso2( double r2 ){ return( reso2_ = r2 ); }
	double reso2( ) const { return( reso2_ ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void reso( double r1, double r2 ){ base::reso1_ = r1; reso2_ = r2; }



/************************************************************************************************************
**
**      X�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief �������̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0 ), width( ) ) ); }
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0 ), width( ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator x_end( size_type i ){ return( iterator( paccess( i, height( ) ), width( ) ) ); }
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, height( ) ), width( ) ) ); }


	/// @brief �t�����̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }

/************************************************************************************************************
**
**      Y�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief �������̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j ), 1 ) ); }
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j ), 1 ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator y_end( size_type j ){ return( iterator( paccess( width( ), j ), 1 ) ); }
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( width( ), j ), 1 ) ); }


	/// @brief �t�����̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class TT, class AAlocator >
	const array2& operator =( const array2< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );
		reso2_ = o.reso2( );

		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const array2& operator =( const array2 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;
		reso2_ = o.reso2_;

		return( *this );
	}

// �v�f�ւ̃A�N�Z�X
protected:
	pointer paccess( size_type i, size_type j )
	{
		return( base::data_ + i + j * size1_ );
	}
	const_pointer paccess( size_type i, size_type j ) const
	{
		return( base::data_ + i + j * size1_ );
	}

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference at( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference at( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference operator ()( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference operator ()( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2_( i, j )
		return( base::data_[ i + j * size1_ ] );
	}


public:
	// �\�z
	array2( ) : base( ), size1_( 0 ), size2_( 0 ), reso2_( 1.0 ) {}
	explicit array2( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ), reso2_( 1.0 ) {}

	array2( size_type num1, size_type num2 ) : base( num1 * num2 ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2 ) : base( num1 * num2, r1 ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}
	array2( size_type num1, size_type num2, const Allocator &a ) : base( num1 * num2, a ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2, const Allocator &a ) : base( num1 * num2, r1, a ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}

	array2( size_type num1, size_type num2, const value_type &val ) : base( num1 * num2, val ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val ) : base( num1 * num2, r1, val ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}
	array2( size_type num1, size_type num2, const value_type &val, const Allocator &a ) : base( num1 * num2, val, a ), size1_( num1 ), size2_( num2 ), reso2_( 1.0 ) {}
	array2( size_type num1, size_type num2, double r1, double r2, const value_type &val, const Allocator &a ) : base( num1 * num2, r1, val, a ), size1_( num1 ), size2_( num2 ), reso2_( r2 ) {}

	template < class TT, class AAlocator >
	array2( const array2< TT, AAlocator > &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), reso2_( o.reso2( ) ) {}

	array2( const array2< T, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ), reso2_( o.reso2_ ) {}
};





/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
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
	typedef mist_iterator2< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator2< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;


protected:
	typedef array2< T, Allocator > base;
	size_type size1_;
	size_type size2_;
	size_type size3_;
	double reso3_;

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, size_type num3 )
	{
		base::resize( num1 * num2, num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, size_type num3, const value_type &val )
	{
		base::resize( num1 * num2, num3, val );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void swap( array3 &a )
	{
		base::swap( a );

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
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	size_type size1( ) const { return( size1_ ); }
	size_type size2( ) const { return( size2_ ); }
	size_type size3( ) const { return( size3_ ); }
	size_type width( ) const { return( size1_ ); }
	size_type height( ) const { return( size2_ ); }
	size_type depth( ) const { return( size3_ ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	double reso3( double r3 ){ return( reso3_ = r3 ); }
	double reso3( ) const { return( reso3_ ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void reso( double r1, double r2, double r3 ){ base::reso1_ = r1; base::reso2_ = r2; reso3_ = r3; }



	/// @brief �������̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator begin( ){ return( iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }
	const_iterator begin( ) const { return( const_iterator( paccess( 0, 0, 0 ), 0, base::size( ), 0 ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator end( ){ return( iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }
	const_iterator end( ) const { return( const_iterator( paccess( 0, 0, 0 ), base::size( ), base::size( ), 0 ) ); }


	/// @brief �t�����̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator rbegin( ){ return( reverse_iterator( end( ) ) ); }
	const_reverse_iterator rbegin( ) const { return( const_reverse_iterator( end( ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator rend( ){ return( reverse_iterator( begin( ) ) ); }
	const_reverse_iterator rend( ) const { return( const_reverse_iterator( begin( ) ) ); }

/************************************************************************************************************
**
**      X�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief �������̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator x_begin( size_type i ){ return( iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }
	const_iterator x_begin( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), 0, 1, width( ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator x_end( size_type i ){ return( iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }
	const_iterator x_end( size_type i ) const { return( const_iterator( paccess( i, 0, 0 ), height( ) * depth( ), 1, width( ) ) ); }



	/// @brief �t�����̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator x_rbegin( size_type i ){ return( reverse_iterator( x_end( i ) ) ); }
	const_reverse_iterator x_rbegin( size_type i ) const { return( const_reverse_iterator( x_end( i ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator x_rend( size_type i ){ return( reverse_iterator( x_begin( i ) ) ); }
	const_reverse_iterator x_rend( size_type i ) const { return( const_reverse_iterator( x_begin( i ) ) ); }


/************************************************************************************************************
**
**      Y�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief �������̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator y_begin( size_type j ){ return( iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }
	const_iterator y_begin( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), 0, height( ), width( ) * height( ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator y_end( size_type j ){ return( iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }
	const_iterator y_end( size_type j ) const { return( const_iterator( paccess( 0, j, 0 ), width( ) * depth( ), height( ), width( ) * height( ) ) ); }


	/// @brief �t�����̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator y_rbegin( size_type j ){ return( reverse_iterator( y_end( j ) ) ); }
	const_reverse_iterator y_rbegin( size_type j ) const { return( const_reverse_iterator( y_end( j ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator y_rend( size_type j ){ return( reverse_iterator( y_begin( j ) ) ); }
	const_reverse_iterator y_rend( size_type j ) const { return( const_reverse_iterator( y_begin( j ) ) ); }


/************************************************************************************************************
**
**      Z�����Œ肵���ꍇ�̏������E�t�����̔����q
**
************************************************************************************************************/

	/// @brief �������̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator z_begin( size_type k ){ return( iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }
	const_iterator z_begin( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), 0, 1, 1 ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	iterator z_end( size_type k ){ return( iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }
	const_iterator z_end( size_type k ) const { return( const_iterator( paccess( 0, 0, k ), width( ) * height( ), 1, 1 ) ); }


	/// @brief �t�����̃����_���A�N�Z�X�C�e���[�^��Ԃ�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator z_rbegin( size_type k ){ return( reverse_iterator( z_end( k ) ) ); }
	const_reverse_iterator z_rbegin( size_type k ) const { return( const_reverse_iterator( z_end( k )) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reverse_iterator z_rend( size_type k ){ return( reverse_iterator( z_begin( k ) ) ); }
	const_reverse_iterator z_rend( size_type k ) const { return( const_reverse_iterator( z_begin( k ) ) ); }


public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class TT, class AAlocator >
	const array3& operator =( const array3< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );
		size3_ = o.size3( );
		reso3_ = o.reso3( );

		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const array3& operator =( const array3 &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;
		size3_ = o.size3_;
		reso3_ = o.reso3_;

		return( *this );
	}


// �v�f�ւ̃A�N�Z�X
protected:
	pointer paccess( size_type i, size_type j, size_type k )
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}
	const_pointer paccess( size_type i, size_type j, size_type k ) const
	{
		return( base::data_ + i + ( j + k * size2_ ) * size1_ );
	}

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference at( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference at( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference operator ()( size_type i, size_type j, size_type k )
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference operator ()( size_type i, size_type j, size_type k ) const
	{
		_CHECK_ACCESS_VIOLATION3_( i, j, k )
		return( base::data_[ i + ( j + k * size2_ ) * size1_ ] );
	}


public:
	// �\�z
	array3( ) : base( ), size1_( 0 ), size2_( 0 ), size3_( 0 ), reso3_( 1.0 ) {}
	explicit array3( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ), size3_( 0 ), reso3_( 1.0 ) {}

	array3( size_type num1, size_type num2, size_type num3 ) : base( num1 * num2, num3 ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3 ) : base( num1 * num2, num3, r1, r2 ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}
	array3( size_type num1, size_type num2, size_type num3, const Allocator &a ) : base( num1 * num2, num3, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const Allocator &a ) : base( num1 * num2, num3, r1, r2, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}

	array3( size_type num1, size_type num2, size_type num3, const value_type &val ) : base( num1 * num2, num3, val ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val ) : base( num1 * num2, num3, r1, r2, val ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}
	array3( size_type num1, size_type num2, size_type num3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, val, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( 1.0 ) {}
	array3( size_type num1, size_type num2, size_type num3, double r1, double r2, double r3, const value_type &val, const Allocator &a ) : base( num1 * num2, num3, r1, r2, val, a ), size1_( num1 ), size2_( num2 ), size3_( num3 ), reso3_( r3 ) {}

	template < class TT, class AAlocator >
	array3( const array3< TT, AAlocator > &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) ), reso3_( o.reso3( ) ) {}

	array3( const array3< T, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ), size3_( o.size3_ ), reso3_( o.reso3_ ) {}
};




/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
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
	typedef Array base;
	size_type margin1_;
	size_type margin2_;
	size_type margin3_;

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1 )
	{
		base::resize( num1 + margin1_ * 2 );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2 )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2 );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, size_type num3 )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, num3 + margin3_ * 2 );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, const value_type &val )
	{
		base::resize( num1 + margin1_ * 2, val );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, const value_type &val )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, val );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, size_type num3, const value_type &val )
	{
		base::resize( num1 + margin1_ * 2, num2 + margin2_ * 2, num3 + margin3_ * 2, val );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void swap( marray &a )
	{
		base::swap( a );

		size_type tmp = margin1_;
		margin1_ = a.margin1_;
		a.margin1_ = tmp;

		tmp = margin2_;
		margin2_ = a.margin2_;
		a.margin2_ = tmp;

		tmp = margin3_;
		margin3_ = a.margin3_;
		a.margin3_ = tmp;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void clear( )
	{
		base::clear( );
		margin1_ = margin2_ = margin3_ = 0;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void fill_margin( const value_type &val = value_type( ) )
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	size_type size1( ) const { return( base::size1( ) - 2 * margin1_ ); }
	size_type size2( ) const { return( base::size2( ) - 2 * margin2_ ); }
	size_type size3( ) const { return( base::size3( ) - 2 * margin3_ ); }
	size_type width( ) const { return( size1( ) ); }
	size_type height( ) const { return( size2( ) ); }
	size_type depth( ) const { return( size3( ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	size_type margin1( ) const { return( margin1_ ); }
	size_type margin2( ) const { return( margin2_ ); }
	size_type margin3( ) const { return( margin3_ ); }

private:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array< T, Allocator > &o )
	{
		base::resize( o.size( ) + margin1_ * 2 );
		return( copy( o ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array1< T, Allocator > &o )
	{
		base::resize( o.size( ) + margin1_ * 2 );
		reso1( o.reso1( ) );
		return( copy( o ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array2< T, Allocator > &o )
	{
		base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2 );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		return( copy( o ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class T, class Allocator >
	const marray& operator =( const array3< T, Allocator > &o )
	{
		base::resize( o.size1( ) + margin1_ * 2, o.size2( ) + margin2_ * 2, o.size3( ) + margin3_ * 2 );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		reso3( o.reso3( ) );
		return( copy( o ) );
	}

// �v�f�ւ̃A�N�Z�X
public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference at( difference_type i )
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference at( difference_type i, difference_type j )
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference at( difference_type i, difference_type j, difference_type k )
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference at( difference_type i ) const
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference at( difference_type i, difference_type j ) const
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference at( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference operator ()( difference_type i )
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference operator ()( difference_type i, difference_type j )
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	reference operator ()( difference_type i, difference_type j, difference_type k )
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference operator ()( difference_type i ) const
	{
		return( base::at( i + margin1_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference operator ()( difference_type i, difference_type j ) const
	{
		return( base::at( i + margin1_, j + margin2_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const_reference operator ()( difference_type i, difference_type j, difference_type k ) const
	{
		return( base::at( i + margin1_, j + margin2_, k + margin3_ ) );
	}


public:
	// �\�z
	marray( ) : base( ), margin1_( 0 ), margin2_( 0 ), margin3_( 0 ) {}

	marray( size_type margin ) : base( ), margin1_( margin ), margin2_( 0 ), margin3_( 0 ) {}

	marray( const marray &o ) : base( o ), margin1_( o.margin1( ) ), margin2_( o.margin2( ) ), margin3_( o.margin3( ) ) {}

	template < class T, class Allocator >
	marray( const array< T, Allocator > &o, size_type margin1, const value_type &val = value_type( 0 ) )
		: base( o.size( ) + margin1 * 2 ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	template < class T, class Allocator >
	marray( const array1< T, Allocator > &o, size_type margin1, const value_type &val = value_type( ) )
		: base( o.size( ) + margin1 * 2, o.reso1( ) ), margin1_( margin1 ), margin2_( 0 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	template < class T, class Allocator >
	marray( const array2< T, Allocator > &o, size_type margin1, size_type margin2, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.reso1( ), o.reso2( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( 0 )
	{
		fill_margin( val );
		copy( o );
	}

	template < class T, class Allocator >
	marray( const array3< T, Allocator > &o, size_type margin1, size_type margin2, size_type margin3, const value_type &val = value_type( ) )
		: base( o.size1( ) + margin1 * 2, o.size2( ) + margin2 * 2, o.size3( ) + margin3 * 2, o.reso1( ), o.reso2( ), o.reso3( ) ), margin1_( margin1 ), margin2_( margin2 ), margin3_( margin3 )
	{
		fill_margin( val );
		copy( o );
	}
};





/// @brief �P�ӂ�2�̎w����̃o�b�t�@���������摜���쐬����
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
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
	typedef Array base;
	size_type size1_;
	size_type size2_;
	size_type size3_;

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1 )
	{
		size1_ = num1;
		base::resize( floor_square( size1_ ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size1_ = num1;
		size2_ = num2;
		s1 = s1 > s2 ? s1 : s2;
		base::resize( s1, s1 );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, size_type num3 )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size_type s3 = floor_square( num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
		s1 = s1 > s2 ? s1 : s2;
		s1 = s1 > s3 ? s1 : s3;
		base::resize( s1, s1, s1 );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, const value_type &val )
	{
		size1_ = num1;
		base::resize( floor_square( size1_ ), val );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, const value_type &val )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size1_ = num1;
		size2_ = num2;
		s1 = s1 > s2 ? s1 : s2;
		base::resize( s1, s1, val );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void resize( size_type num1, size_type num2, size_type num3, const value_type &val )
	{
		size_type s1 = floor_square( num1 );
		size_type s2 = floor_square( num2 );
		size_type s3 = floor_square( num3 );
		size1_ = num1;
		size2_ = num2;
		size3_ = num3;
		s1 = s1 > s2 ? s1 : s2;
		s1 = s1 > s3 ? s1 : s3;
		base::resize( s1, s1, s1, val );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void swap( buffered_array &a )
	{
		base::swap( a );

		size_type tmp = size1_;
		size1_ = a.size1_;
		a.size1_ = tmp;

		tmp = size2_;
		size2_ = a.size2_;
		a.size2_ = tmp;

		tmp = size3_;
		size3_ = a.size3_;
		a.size3_ = tmp;
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = size3_ = 0;
	}

	size_type size1( ) const { return( size1_ > 0 ? size1_ : base::size1( ) ); }
	size_type size2( ) const { return( size2_ > 0 ? size2_ : base::size2( ) ); }
	size_type size3( ) const { return( size3_ > 0 ? size3_ : base::size3( ) ); }
	size_type width( ) const { return( size1( ) ); }
	size_type height( ) const { return( size2( ) ); }
	size_type depth( ) const { return( size3( ) ); }


private:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const buffered_array& operator =( const buffered_array &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;
		size3_ = o.size3_;

		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array< T, Allocator > &o )
	{
		resize( o.size( ) );
		return( copy( o ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array1< T, Allocator > &o )
	{
		resize( o.size( ) );
		reso1( o.reso1( ) );
		return( copy( o ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	template < class T, class Allocator >
	const buffered_array& operator =( const array2< T, Allocator > &o )
	{
		resize( o.size1( ), o.size2( ) );
		reso1( o.reso1( ) );
		reso2( o.reso2( ) );
		return( copy( o ) );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
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
	// �\�z
	buffered_array( ) : base( ), size1_( 0 ), size2_( 0 ), size3_( 0 ) {}

	buffered_array( const buffered_array &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) ) {}

	template < class T, class Allocator >
	buffered_array( const array< T, Allocator > &o )
		: base( floor_square( o.size( ) ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	template < class T, class Allocator >
	buffered_array( const array1< T, Allocator > &o )
		: base( floor_square( o.size( ) ), o.reso1( ) ), size1_( o.size( ) ), size2_( 0 ), size3_( 0 )
	{
		copy( o );
	}

	template < class T, class Allocator >
	buffered_array( const array2< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), o.reso1( ), o.reso2( ) ), size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( 0 )
	{
		copy( o );
	}

	template < class T, class Allocator >
	buffered_array( const array3< T, Allocator > &o )
		: base( floor_square( o.size1( ) ), floor_square( o.size2( ) ), floor_square( o.size3( ) ), o.reso1( ), o.reso2( ), o.reso3( ) ),
			size1_( o.size1( ) ), size2_( o.size2( ) ), size3_( o.size3( ) )
	{
		copy( o );
	}
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array< T, Allocator > &a )
{
	typename array< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[i];
		if( i != a.size1( ) - 1 ) out << ", ";
	}
	out << std::endl;

	return( out );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array1< T, Allocator > &a )
{
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[i];
		if( i != a.size1( ) - 1 ) out << ", ";
	}
	out << std::endl;

	return( out );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const array2< T, Allocator > &a )
{
	typename array2< T, Allocator >::size_type i, j;
	for( j = 0 ; j < a.size2( ) ; j++ )
	{
		for( i = 0 ; i < a.size1( ) ; i++ )
		{
			out << a( i, j );
			if( i != a.size1( ) - 1 ) out << ", ";
		}
		out << std::endl;
	}

	return( out );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
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



// �摜�ԉ��Z
//#include "operator/operator_array.h"
//#include "operator/operator_array1.h"
//#include "operator/operator_array2.h"
//#include "operator/operator_array3.h"

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_H__
