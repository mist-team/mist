#ifndef __INCLUDE_MIST_ALLOC_H__
#define __INCLUDE_MIST_ALLOC_H__

#include "mist_conf.h"

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


template < bool b >
struct mist_memory_operator
{
	// num �̃I�u�W�F�N�g���m�ۂ��C�f�t�H���g�̒l�ŏ���������
	// �g�ݍ��݌^�̃f�[�^�̏ꍇ�� 0 �ŏ���������
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

	// num �̃I�u�W�F�N�g���m�ۂ��C�����̒l�ŏ���������
	// �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
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

	// num �̃I�u�W�F�N�g���m�ۂ��C�|�C���^�ň͂܂ꂽ�Ԃ̃f�[�^�ŏ���������
	// �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::pointer p = ptr ; s != e ; p++, s++ ) allocator.construct( p, *s );
		return( ptr );
	}

	// ptr ���� num �̃I�u�W�F�N�g���J�����f�X�g���N�^���Ăяo��
	// �g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ăяo���Ȃ�
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Deallocator" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.destroy( &( ptr[i] ) );
		allocator.deallocate( ptr, num );
	}

	// �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	// �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
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

	// �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	// �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator &allocator, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Copy Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) to[i] = ptr[i];
		return( to + num );
	}

	// �|�C���^ ptr ���� num �̃f�[�^��obj��������
	// char �^�̏ꍇ�� memset �𗘗p����
	template < class Allocator >
	static void fill_objects1( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Clean Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}

	// �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	// �g�ݍ��݌^�̏ꍇ�� memset �𗘗p����
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




template <>
struct mist_memory_operator< true >
{
	// num �̃I�u�W�F�N�g���m�ۂ��C�f�t�H���g�̒l�ŏ���������
	// �g�ݍ��݌^�̃f�[�^�̏ꍇ�� 0 �ŏ���������
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

	// num �̃I�u�W�F�N�g���m�ۂ��C�����̒l�ŏ���������
	// �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
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

	// num �̃I�u�W�F�N�g���m�ۂ��C�|�C���^�ň͂܂ꂽ�Ԃ̃f�[�^�ŏ���������
	// �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		memcpy( ptr, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}

	// ptr ���� num �̃I�u�W�F�N�g���J�����f�X�g���N�^���Ăяo��
	// �g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ăяo���Ȃ�
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Specialized Deallocator" << ::std::endl;
#endif
		allocator.deallocate( ptr, num );
	}

	// �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	// �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( x, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( x + ( e - s ) );
	}

	// �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	// �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator &allocator, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( to, ptr, num * sizeof( typename Allocator::value_type ) );
		return( to + num );
	}

	// �|�C���^ ptr ���� num �̃f�[�^��obj��������
	// char �^�̏ꍇ�� memset �𗘗p����
	template < class Allocator >
	static void fill_objects1( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, obj, num * sizeof( typename Allocator::value_type ) );
	}

	// �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	// �g�ݍ��݌^�̏ꍇ�� memset �𗘗p����
	template < class Allocator >
	static void fill_objects2( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
	}
};



// MIST������STL�̃A���P�[�^�����ʓI�ɗ��p���邽�߂̎d�g��
// �f�[�^�̕��ɂ���āC�������R�s�[�������������Ă���
template < class T, class Allocator >
class mist_allocator : public Allocator
{
public:
	typedef Allocator base;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

public:
	// num �̃I�u�W�F�N�g�̃��������m�ۂ����������s���i�g�ݍ��݌^���ǂ����œ����̎����𕪊�j
	pointer allocate_objects( size_type num )
	{
		if( num <= 0 ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects1( *this, num ) );
	}

	// num �̃I�u�W�F�N�g�̃��������m�ۂ� obj ��p���ď��������s���i�g�ݍ��݌^���ǂ����œ����̎����𕪊�j
	pointer allocate_objects( size_type num, const_reference obj )
	{
		if( num <= 0 ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects2( *this, num, obj ) );
	}

	// num �̃I�u�W�F�N�g���m�ۂ��C�|�C���^�ň͂܂ꂽ�Ԃ̃f�[�^�ŏ���������(�g�ݍ��݌^�̃f�[�^�̏ꍇ�� memcpy �𗘗p����)
	pointer allocate_objects( const_pointer s, const_pointer e )
	{
		if( s >= e ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects3( *this, s, e ) );
	}

	// num �̃I�u�W�F�N�g�̃��������J�����C�f�X�g���N�^���Ăяo���i�g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ă΂Ȃ��j
	void deallocate_objects( pointer ptr, size_type num )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_arithmetic< T >::value >::deallocate_objects( *this, ptr, num );
	}

	// �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	pointer copy_objects( const_pointer s, const_pointer e, pointer x )
	{
		if( s >= e ) return( x );
		return( mist_memory_operator< is_arithmetic< T >::value >::copy_objects1( *this, s, e, x ) );
	}

	// �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	pointer copy_objects( const_pointer ptr, size_type num, pointer to )
	{
		if( num <= 0 ) return( to );
		return( mist_memory_operator< is_arithmetic< T >::value >::copy_objects2( *this, ptr, num, to ) );
	}

	// �|�C���^ ptr ���� num �̃f�[�^��obj��������
	void fill_objects( pointer ptr, size_type num, const_reference obj )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_char< T >::value >::fill_objects1( *this, ptr, num, obj );
	}

	// �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	void fill_objects( pointer ptr, size_type num )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_arithmetic< T >::value >::fill_objects2( *this, ptr, num );
	}

	// ptr ����n�܂� num ���݂���I�u�W�F�N�g�� dest_num �܂ŏk�߂�
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num )
	{
		if( num < dest_num ) return( ptr );			// �J�����悤�Ƃ��Ă���z��̗v�f���́C�g������̔z��T�C�Y���T�C�Y������������O
		if( num < 0 ) return( NULL );				// �g������̔z��T�C�Y��0��菬������O
		if( num == dest_num ) return( ptr );		// �g�����ɂ��ύX�̕K�v�Ȃ�
		if( num == 0 ) return( NULL );
#if _MIST_ALLOCATOR_MEMORY_TRIM__ != 0
		deallocate_objects( ptr + dest_num, num - dest_num );
#else
		deallocate_objects( ptr, num );
		ptr = allocate_objects( num );
#endif
		return( dest_num == 0 ? NULL : ptr );
	}


	const Allocator &operator=( const Allocator &alloc )
	{
		if( &alloc != this ) base::operator=( alloc );
		return( *this );
	}
	mist_allocator( ) : base( ){}
	mist_allocator( const Allocator &alloc ) : base( alloc ){}
};



// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_ALLOC_H__
