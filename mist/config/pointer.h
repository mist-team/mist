/// @file mist/config/pointer.h
//!
//! @brief �������̎����J���Ȃǂ��s�����߂̃N���X
//!

#ifndef __INCLUDE_MIST_POINTER_H__
#define __INCLUDE_MIST_POINTER_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// MIST�ŗ��p������̃f�[�^�^


/// @brief �I�u�W�F�N�g�̃X�R�[�v���؂��Ǝ����I�Ƀ������J�����s���|�C���^
//! 
//! @attention �z��ɑ΂��Ă͗��p�ł��Ȃ�
//! 
template < class T >
class scoped_ptr
{
public:
	typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef T value_type;				///< @brief �����f�[�^�^�DT �Ɠ���
	typedef T* pointer;					///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef T& reference;				///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const T& const_reference;	///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef const T* const_pointer;		///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

private:
	pointer ptr_;		///< @brief �Ǘ�����|�C���^

public:
	/// @brief ������ b �ŏ���������
	scoped_ptr( pointer p ) : ptr_( p ){ }

	~scoped_ptr( ){ delete ptr_; }


public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }


private:
	scoped_ptr( const scoped_ptr &p );
	const scoped_ptr &operator  =( const scoped_ptr &p );
};

/// @brief �I�u�W�F�N�g�̃X�R�[�v���؂��Ɣz��p�Ɋm�ۂ����������̎����J�����s���|�C���^
//! 
//! @attention �z��̃|�C���^�ɑ΂��Ă������p�ł��Ȃ�
//! 
template < class T >
class scoped_array
{
public:
	typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef T value_type;				///< @brief �����f�[�^�^�DT �Ɠ���
	typedef T* pointer;					///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef T& reference;				///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const T& const_reference;	///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef const T* const_pointer;		///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

private:
	pointer ptr_;		///< @brief �Ǘ�����|�C���^

public:
	/// @brief ������ b �ŏ���������
	scoped_array( pointer p ) : ptr_( p ){ }

	~scoped_array( ){ delete [] ptr_; }


public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }

	reference operator []( difference_type index ){ return( ptr_[ index ] ); }
	const_reference operator []( difference_type index ) const { return( ptr_[ index ] ); }

private:
	scoped_array( const scoped_array &p );
	const scoped_array &operator  =( const scoped_array &p );
};



///// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
////! 
////! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
////! @param[in]     b   �c �o�C�i����f
////! 
////! @return ���͂��ꂽ�X�g���[��
////! 
////! @code �o�͗�
////! 1
////! @endcode
////! 
//inline std::ostream &operator <<( std::ostream &out, const binary &b )
//{
//	out << b.get_value( );
//	return( out );
//}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_POINTER_H__
