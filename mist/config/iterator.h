/// @file mist/config/iterator.h
//!
//! @brief MIST�R���e�i�𑀍삷�邽�߂̃C�e���[�^
//!

#ifndef __INCLUDE_MIST_ITERATOR_H__
#define __INCLUDE_MIST_ITERATOR_H__


#include <iterator>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



/// @brief mist�R���e�i�ŗ��p����1��������p�����_���A�N�Z�X�C�e���[�^
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T         �c �����̐���
//! @param Distance  �c �����̐���
//! @param Pointer   �c �����̐���
//! @param Reference �c �����̐���
//! 
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;					///< @brief �����f�[�^�^�Dbool �Ɠ���
	typedef Pointer pointer;				///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef Reference reference;			///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef Distance difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef Reference const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�

private:
	pointer data_;					///< @brief �ϐ��̐���������
	size_type diff_pointer_;		///< @brief �ϐ��̐���������

public:
	/// @brief �R�s�[���Z�q
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] p    �c �����̐���
	//! @param[in] diff �c �����̐���
	//! 
	mist_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }

	/// @brief �R�s�[���Z�q
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	mist_iterator1( const mist_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }


	/// @brief �R�s�[���Z�q
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator1& operator =( const mist_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}


	/// @brief �v�f�̃A�N�Z�X
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	reference operator *(){ return( *data_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const_reference operator *() const { return( *data_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const_reference operator []( difference_type dist ) const { return( data_[ dist * diff_pointer_ ] ); }


	/// @brief �O�u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	mist_iterator1& operator ++( )
	{
		*this += 1;
		return( *this );
	}

	/// @brief ��u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator1 operator ++( int )
	{
		mist_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}

	/// @brief �O�u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	mist_iterator1& operator --( )
	{
		*this -= 1;
		return( *this );
	}

	/// @brief ��u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator1 operator --( int )
	{
		mist_iterator1 old_val( *this );
		*this -= 1;
		return( old_val );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator1& operator +=( difference_type dist )
	{
		data_ += dist * diff_pointer_;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator1& operator -=( difference_type dist )
	{
		data_ -= dist * diff_pointer_;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const difference_type operator -( const mist_iterator1 &ite ) const
	{
		return( ( data_ - ite.data_ ) / diff_pointer_ );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator ==( const mist_iterator1 &ite ) const { return( data_ == ite.data_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator !=( const mist_iterator1 &ite ) const { return( !( *this == ite )  ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] ite  �c �����̐���
//! @param[in] dist �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] dist �c �����̐���
//! @param[in] ite  �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] ite  �c �����̐���
//! @param[in] dist �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}



/// @brief mist�R���e�i�ŗ��p����1��������p�����_���A�N�Z�X�C�e���[�^
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T         �c �����̐���
//! @param Distance  �c �����̐���
//! @param Pointer   �c �����̐���
//! @param Reference �c �����̐���
//! 
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;					///< @brief �����f�[�^�^�Dbool �Ɠ���
	typedef Pointer pointer;				///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef Reference reference;			///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef Distance difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef Reference const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�

private:
	pointer data_;				///< @brief �ϐ��̐���������
	difference_type index_;		///< @brief �ϐ��̐���������
	difference_type width_;		///< @brief �ϐ��̐���������
	difference_type step_;		///< @brief �ϐ��̐���������

public:
	/// @brief �֐��̐���������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] p     �c �����̐���
	//! @param[in] index �c �����̐���
	//! @param[in] width �c �����̐���
	//! @param[in] step  �c �����̐���
	//! 
	mist_iterator2( pointer p = NULL, difference_type index = 0, difference_type width = 1, difference_type step = 1 )
												: data_( p ), index_( index ), width_( width ), step_( step )
	{
	}

	/// @brief �֐��̐���������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	mist_iterator2( const mist_iterator2 &ite ) : data_( ite.data_ ), index_( ite.index_ ), width_( ite.width_ ), step_( ite.step_ )
	{
	}


	/// @brief �R�s�[���Z�q
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		if( &ite != this )
		{
			data_  = ite.data_;
			index_ = ite.index_;
			width_ = ite.width_;
			step_  = ite.step_;
		}
		return( *this );
	}


	/// @brief �v�f�̃A�N�Z�X
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	reference operator *()
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const_reference operator *() const
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	reference operator []( difference_type dist ){ return( *( *this += dist ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const_reference operator []( difference_type dist ) const { return( *( *this += dist ) ); }


	/// @brief �O�u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	mist_iterator2& operator ++( )
	{
		*this += 1;
		return( *this );
	}

	/// @brief ��u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator2 operator ++( int )
	{
		mist_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}

	/// @brief �O�u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	mist_iterator2& operator --( )
	{
		*this -= 1;
		return( *this );
	}

	/// @brief ��u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator2 operator --( int )
	{
		mist_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator2& operator +=( difference_type dist )
	{
		index_ += dist;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_iterator2& operator -=( difference_type dist )
	{
		index_ -= dist;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const difference_type operator -( const mist_iterator2 &ite ) const
	{
		return( index_ - ite.index_ );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator ==( const mist_iterator2 &ite ) const { return( *this - ite == 0 ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator !=( const mist_iterator2 &ite ) const { return( !( *this == ite ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator < ( const mist_iterator2 &ite ) const { return( *this - ite < 0 ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator <=( const mist_iterator2 &ite ) const { return( !( *this > ite ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator > ( const mist_iterator2 &ite ) const { return( ite < *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator >=( const mist_iterator2 &ite ) const { return( !( *this < ite ) ); }
};



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] ite  �c �����̐���
//! @param[in] dist �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] dist �c �����̐���
//! @param[in] ite  �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] ite  �c �����̐���
//! @param[in] dist �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] _Ite �c �����̐���
//! 
template< class _Ite >
class mist_reverse_iterator :
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type
	>
#else
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type,
		typename _Ite::pointer,
		typename _Ite::reference
	>
#endif
{
public:
 	typedef typename _Ite::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
 	typedef typename _Ite::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename _Ite::value_type value_type;				///< @brief ///< @brief �����f�[�^�^�Dbool �Ɠ���
	typedef typename _Ite::pointer pointer;						///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef typename _Ite::reference reference;					///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef typename _Ite::const_reference const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�

protected:
	_Ite current_iterator_;		///< @brief �ϐ��̐���������

public:
	/// @brief �R���X�g���N�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	mist_reverse_iterator( ){ }

	/// @brief �R���X�g���N�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	mist_reverse_iterator( const _Ite &ite ) : current_iterator_( ite ){ }

	/// @brief �R���X�g���N�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//! 
	mist_reverse_iterator( const mist_reverse_iterator &ite ) : current_iterator_( ite.current_iterator_ ){ }


	/// @brief �R�s�[���Z�q
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @return �߂�l�̐���
	//! 
	const mist_reverse_iterator& operator =( const _Ite &ite )
	{
		current_iterator_ = ite;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @return �߂�l�̐���
	//! 
	const mist_reverse_iterator& operator =( const mist_reverse_iterator &ite )
	{
		current_iterator_ = ite.current_iterator_;
		return( *this );
	}


	/// @brief �v�f�̃A�N�Z�X
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	reference operator *()
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//!
	//! @return �߂�l�̐���
	//! 
	const_reference operator *() const
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//!
	//! @return �߂�l�̐���
	//! 
	reference operator []( difference_type dist ){ return( *( *this + dist ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//!
	//! @return �߂�l�̐���
	//! 
	const_reference operator []( difference_type dist ) const { return( *( *this + dist ) ); }


	/// @brief �O�u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	mist_reverse_iterator& operator ++( )
	{
		--current_iterator_;
		return( *this );
	}

	/// @brief ��u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_reverse_iterator operator ++( int )
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_--;
		return( old_val );
	}

	/// @brief �O�u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	mist_reverse_iterator& operator --( )
	{
		++current_iterator_;
		return( *this );
	}

	/// @brief ��u�^
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const mist_reverse_iterator operator --( int )
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_++;
		return( old_val );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//!
	//! @return �߂�l�̐���
	//! 
	const mist_reverse_iterator& operator +=( difference_type dist )
	{
		current_iterator_ -= dist;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] dist �c �����̐���
	//!
	//! @return �߂�l�̐���
	//! 
	const mist_reverse_iterator& operator -=( difference_type dist )
	{
		current_iterator_ += dist;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @return �߂�l�̐���
	//! 
	const difference_type operator -( const mist_reverse_iterator &ite ) const
	{
		return( ite.current_iterator_ - current_iterator_ );
	}



	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator ==( const mist_reverse_iterator &ite ) const { return( current_iterator_ == ite.current_iterator_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator !=( const mist_reverse_iterator &ite ) const { return( !( *this == ite ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator < ( const mist_reverse_iterator &ite ) const { return( ite.current_iterator_ < current_iterator_  ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator <=( const mist_reverse_iterator &ite ) const { return( !( *this > ite ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator > ( const mist_reverse_iterator &ite ) const { return( ite < *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ite �c �����̐���
	//!
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator >=( const mist_reverse_iterator &ite ) const { return( !( *this < ite ) ); }
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] ite1 �c �����̐���
//! @param[in] ite2 �c �����̐���
//!
//! @return �߂�l�̐���
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
{
	return( mist_reverse_iterator< _Ite >( ite1 ) += ite2 );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] ite  �c �����̐���
//! @param[in] dist �c �����̐���
//!
//! @return �߂�l�̐���
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] dist �c �����̐���
//! @param[in] ite  �c �����̐���
//!
//! @return �߂�l�̐���
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( typename _Ite::difference_type dist, const mist_reverse_iterator< _Ite > &ite )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}


//template< class _Ite >
//inline const typename mist_reverse_iterator< _Ite >::difference_type operator -( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
//{
//	return( ite1 - ite2 );
//}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] ite  �c �����̐���
//! @param[in] dist �c �����̐���
//!
//! @return �߂�l�̐���
//! 
template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator -( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) -= dist );
}

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_ITERATOR_H__
