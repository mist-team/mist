/// @file mist/config/mist_alloc.h
//!
//! @brief MIST�R���e�i�ŗ��p����A���P�[�^
//!

#ifndef __INCLUDE_MIST_ALLOC_H__
#define __INCLUDE_MIST_ALLOC_H__

#include <memory>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "type_trait.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param b �c �����̐���
//! 
template < bool b >
struct mist_memory_operator
{
	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�f�t�H���g�̒l�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�� 0 �ŏ���������
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] num       �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		typename Allocator::value_type obj;
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.construct( &( ptr[i] ), obj );
		return( ptr );
	}


	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�����̒l�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] num       �c �����̐���
	//! @param[in] obj       �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.construct( &( ptr[i] ), obj );
		return( ptr );
	}


	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�|�C���^�ň͂܂ꂽ�Ԃ̃f�[�^�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] s         �c �����̐���
	//! @param[in] e         �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( e - s, 0 );
		for( typename Allocator::pointer p = ptr ; s != e ; p++, s++ ) allocator.construct( p, *s );
		return( ptr );
	}


	/// @brief ptr ���� num �̃I�u�W�F�N�g���J�����f�X�g���N�^���Ăяo��
	//! 
	//! �g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ăяo���Ȃ�
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! 
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Deallocator" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.destroy( &( ptr[i] ) );
		allocator.deallocate( ptr, num );
	}


	/// @brief �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] s         �c �����̐���
	//! @param[in] e         �c �����̐���
	//! @param[in] x         �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Copy Function" << ::std::endl;
#endif
		while( s != e )
		{
			*x = *s;
			++x;
			++s;
		}
		return( x );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! @param[in] to        �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator &allocator, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Copy Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) to[i] = ptr[i];
		return( to + num );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^��obj��������
	//! 
	//! char �^�̏ꍇ�� memset �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! @param[in] obj       �c �����̐���
	//! 
	template < class Allocator >
	static void fill_objects1( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Clean Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memset �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! 
	template < class Allocator >
	static void fill_objects2( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Clean Function" << ::std::endl;
#endif
		typename Allocator::value_type obj;
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}
};




/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
template <>
struct mist_memory_operator< true >
{
	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�f�t�H���g�̒l�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�� 0 �ŏ���������
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] num       �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memset Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		::memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}


	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�����̒l�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] num       �c �����̐���
	//! @param[in] obj       �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Specialized Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
		return( ptr );
	}


	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�|�C���^�ň͂܂ꂽ�Ԃ̃f�[�^�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] s         �c �����̐���
	//! @param[in] e         �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( e - s, 0 );
		memcpy( ptr, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}


	/// @brief ptr ���� num �̃I�u�W�F�N�g���J�����f�X�g���N�^���Ăяo��
	//! 
	//! �g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ăяo���Ȃ�
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! 
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Specialized Deallocator" << ::std::endl;
#endif
		allocator.deallocate( ptr, num );
	}


	/// @brief �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] s         �c �����̐���
	//! @param[in] e         �c �����̐���
	//! @param[in] x         �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( x, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( x + ( e - s ) );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! @param[in] to        �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator &allocator, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( to, ptr, num * sizeof( typename Allocator::value_type ) );
		return( to + num );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^��obj��������
	//! 
	//! char �^�̏ꍇ�� memset �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! @param[in] obj       �c �����̐���
	//! 
	template < class Allocator >
	static void fill_objects1( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, obj, num * sizeof( typename Allocator::value_type ) );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memset �𗘗p����
	//! 
	//! @param[in] allocator �c �����̐���
	//! @param[in] ptr       �c �����̐���
	//! @param[in] num       �c �����̐���
	//! 
	template < class Allocator >
	static void fill_objects2( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
	}
};



/// @brief MIST������STL�̃A���P�[�^�����ʓI�ɗ��p���邽�߂̎d�g��
//! 
//! �f�[�^�̌^�ɂ���āC�������R�s�[�������������Ă���
//! 
//! @param T         �c �����̐���
//! @param Allocator �c �����̐���
//! 
template < class T, class Allocator >
class mist_allocator : public Allocator
{
public:
	typedef Allocator base;											///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Allocator::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Allocator::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Allocator::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename Allocator::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Allocator::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Allocator::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

public:
	/// @brief num �̃I�u�W�F�N�g�̃��������m�ۂ����������s���i�g�ݍ��݌^���ǂ����œ����̎����𕪊�j
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] num �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	pointer allocate_objects( size_type num )
	{
		if( num <= 0 ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects1( *this, num ) );
	}

	/// @brief num �̃I�u�W�F�N�g�̃��������m�ۂ� obj ��p���ď��������s���i�g�ݍ��݌^���ǂ����œ����̎����𕪊�j
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] num �c �����̐���
	//! @param[in] obj �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	pointer allocate_objects( size_type num, const_reference obj )
	{
		if( num <= 0 ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects2( *this, num, obj ) );
	}

	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�|�C���^�ň͂܂ꂽ�Ԃ̃f�[�^�ŏ���������(�g�ݍ��݌^�̃f�[�^�̏ꍇ�� memcpy �𗘗p����)
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! @param[in] e �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	pointer allocate_objects( const_pointer s, const_pointer e )
	{
		if( s >= e ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects3( *this, s, e ) );
	}

	/// @brief num �̃I�u�W�F�N�g�̃��������J�����C�f�X�g���N�^���Ăяo���i�g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ă΂Ȃ��j
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ptr �c �����̐���
	//! @param[in] num �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	void deallocate_objects( pointer ptr, size_type num )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_arithmetic< T >::value >::deallocate_objects( *this, ptr, num );
	}

	/// @brief �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! @param[in] e �c �����̐���
	//! @param[in] x �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	pointer copy_objects( const_pointer s, const_pointer e, pointer x )
	{
		if( s >= e ) return( x );
		return( mist_memory_operator< is_arithmetic< T >::value >::copy_objects1( *this, s, e, x ) );
	}

	/// @brief �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ptr �c �����̐���
	//! @param[in] num �c �����̐���
	//! @param[in] to  �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	pointer copy_objects( const_pointer ptr, size_type num, pointer to )
	{
		if( num <= 0 ) return( to );
		return( mist_memory_operator< is_arithmetic< T >::value >::copy_objects2( *this, ptr, num, to ) );
	}

	/// @brief �|�C���^ ptr ���� num �̃f�[�^��obj��������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ptr �c �����̐���
	//! @param[in] num �c �����̐���
	//! @param[in] obj �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	void fill_objects( pointer ptr, size_type num, const_reference obj )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_char< T >::value >::fill_objects1( *this, ptr, num, obj );
	}

	/// @brief �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ptr �c �����̐���
	//! @param[in] num �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	void fill_objects( pointer ptr, size_type num )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_arithmetic< T >::value >::fill_objects2( *this, ptr, num );
	}

	/// @brief ptr ����n�܂� num ���݂���I�u�W�F�N�g�� dest_num �܂ŏk�߁C�S�Ă̗v�f���f�t�H���g�̒l�ŏ���������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ptr      �c �����̐���
	//! @param[in] num      �c �����̐���
	//! @param[in] dest_num �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num )
	{
		if( num < dest_num ) return( ptr );			// �J�����悤�Ƃ��Ă���z��̗v�f���́C�g������̔z��T�C�Y���T�C�Y������������O
		if( num < 0 ) return( NULL );				// �g������̔z��T�C�Y��0��菬������O
		if( num == dest_num ) return( ptr );		// �g�����ɂ��ύX�̕K�v�Ȃ�
		if( num == 0 ) return( NULL );
#if _MIST_ALLOCATOR_MEMORY_TRIM_ != 0
		deallocate_objects( ptr + dest_num, num - dest_num );
		fill_objects( ptr, dest_num );
#else
		deallocate_objects( ptr, num );
		ptr = allocate_objects( num );
#endif
		return( dest_num == 0 ? NULL : ptr );
	}

	/// @brief ptr ����n�܂� num ���݂���I�u�W�F�N�g�� dest_num �܂ŏk�߁C�S�Ă̗v�f��l obj �ŏ���������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ptr      �c �����̐���
	//! @param[in] num      �c �����̐���
	//! @param[in] dest_num �c �����̐���
	//! @param[in] obj      �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num, const_reference obj )
	{
		if( num < dest_num ) return( ptr );			// �J�����悤�Ƃ��Ă���z��̗v�f���́C�g������̔z��T�C�Y���T�C�Y������������O
		if( num < 0 ) return( NULL );				// �g������̔z��T�C�Y��0��菬������O
		if( num == dest_num ) return( ptr );		// �g�����ɂ��ύX�̕K�v�Ȃ�
		if( num == 0 ) return( NULL );
#if _MIST_ALLOCATOR_MEMORY_TRIM_ != 0
		deallocate_objects( ptr + dest_num, num - dest_num );
		fill_objects( ptr, dest_num, obj );
#else
		deallocate_objects( ptr, num );
		ptr = allocate_objects( num, obj );
#endif
		return( dest_num == 0 ? NULL : ptr );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] alloc �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const Allocator &operator=( const Allocator &alloc )
	{
		if( &alloc != this ) base::operator=( alloc );
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	mist_allocator( ) : base( ){}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] alloc �c �����̐���
	//! 
	mist_allocator( const Allocator &alloc ) : base( alloc ){}
};



// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_ALLOC_H__
