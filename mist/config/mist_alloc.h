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

/// @file mist/config/mist_alloc.h
//!
//! @brief MIST�R���e�i�ŗ��p����A���P�[�^����сC���������샋�[�`��
//!

#ifndef __INCLUDE_MIST_ALLOC_H__
#define __INCLUDE_MIST_ALLOC_H__

#include <cstring>
#include <memory>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "type_trait.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



/// @brief ���[�U�[��`�^�p�̃���������֐��Q
template < bool b >
struct mist_memory_operator
{
	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�f�t�H���g�̒l�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�� 0 �ŏ���������
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] num       �c �m�ۂ���I�u�W�F�N�g��
	//! 
	//! @return �m�ۂ����������̈�̐擪�A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Allocator" << ::std::endl;
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
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] num       �c �m�ۂ���I�u�W�F�N�g��
	//! @param[in] obj       �c �R�s�[�R���X�g���N�^�ɓn�������l
	//! 
	//! @return �m�ۂ����������̈�̐擪�A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.construct( &( ptr[i] ), obj );
		return( ptr );
	}


	/// @brief �|�C���^ s ���� e �͈̔͂̃f�[�^�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] s         �c �J�n�|�C���^
	//! @param[in] e         �c �I���|�C���^
	//! 
	//! @return �m�ۂ����������̈�̐擪�A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( e - s, 0 );
		for( typename Allocator::pointer p = ptr ; s != e ; p++, s++ ) allocator.construct( p, *s );
		return( ptr );
	}


	/// @brief ptr ���� num �̃I�u�W�F�N�g���J�����f�X�g���N�^���Ăяo��
	//! 
	//! �g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ăяo���Ȃ�
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �J�����郁�����̈�̐擪�A�h���X
	//! @param[in] num       �c �J������I�u�W�F�N�g��
	//! 
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Deallocator" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.destroy( &( ptr[i] ) );
		allocator.deallocate( ptr, num );
	}


	/// @brief �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memcpy �𗘗p����
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] s         �c �R�s�[���̐擪�A�h���X
	//! @param[in] e         �c �R�s�[���̖����A�h���X
	//! @param[in] x         �c �R�s�[��̐擪�A�h���X
	//! 
	//! @return �R�s�[��̖����A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator & /* allocator */, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Copy Function" << ::std::endl;
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
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �R�s�[���̐擪�A�h���X
	//! @param[in] num       �c �R�s�[����I�u�W�F�N�g��
	//! @param[in] to        �c �R�s�[��̐擪�A�h���X
	//! 
	//! @return �R�s�[��̖����A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator & /* allocator */, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Copy Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) to[i] = ptr[i];
		return( to + num );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^��obj��������
	//! 
	//! char �^�̏ꍇ�� memset �𗘗p����
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �������擪�A�h���X
	//! @param[in] num       �c �������I�u�W�F�N�g��
	//! @param[in] obj       �c �������I�u�W�F�N�g
	//! 
	template < class Allocator >
	static void fill_objects1( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Clean Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	//! 
	//! �g�ݍ��݌^�̏ꍇ�� memset �𗘗p����
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �������擪�A�h���X
	//! @param[in] num       �c �������I�u�W�F�N�g��
	//! 
	template < class Allocator >
	static void fill_objects2( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Normal Clean Function" << ::std::endl;
#endif
		typename Allocator::value_type obj;
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}
};




/// @brief �g�ݍ��݌^�p�̃���������֐��Q
template <>
struct mist_memory_operator< true >
{
	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�f�t�H���g�̒l�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�� 0 �ŏ���������
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] num       �c �m�ۂ���I�u�W�F�N�g��
	//! 
	//! @return �m�ۂ����������̈�̐擪�A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memset Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		::memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}


	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�����̒l�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] num       �c �m�ۂ���I�u�W�F�N�g��
	//! @param[in] obj       �c �R�s�[�R���X�g���N�^�ɓn�������l
	//! 
	//! @return �m�ۂ����������̈�̐擪�A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Specialized Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
		return( ptr );
	}


	/// @brief �|�C���^ s ���� e �͈̔͂̃f�[�^�ŏ���������
	//! 
	//! �g�ݍ��݌^�̃f�[�^�̏ꍇ�͒l�������ď���������
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] s         �c �J�n�|�C���^
	//! @param[in] e         �c �I���|�C���^
	//! 
	//! @return �m�ۂ����������̈�̐擪�A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( e - s, 0 );
		memcpy( ptr, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}


	/// @brief ptr ���� num �̃I�u�W�F�N�g���J�����f�X�g���N�^���Ăяo��
	//! 
	//! �g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ăяo���Ȃ�
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �J�����郁�����̈�̐擪�A�h���X
	//! @param[in] num       �c �J������I�u�W�F�N�g��
	//! 
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Specialized Deallocator" << ::std::endl;
#endif
		allocator.deallocate( ptr, num );
	}


	/// @brief �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	//! 
	//! memcpy �𗘗p���ăo�C�g�P�ʂŃf�[�^���R�s�[����
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] s         �c �R�s�[���̐擪�A�h���X
	//! @param[in] e         �c �R�s�[���̖����A�h���X
	//! @param[in] x         �c �R�s�[��̐擪�A�h���X
	//! 
	//! @return �R�s�[��̖����A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator & /* allocator */, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( x, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( x + ( e - s ) );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	//! 
	//! memcpy �𗘗p���ăo�C�g�P�ʂŃf�[�^���R�s�[����
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �R�s�[���̐擪�A�h���X
	//! @param[in] num       �c �R�s�[����I�u�W�F�N�g��
	//! @param[in] to        �c �R�s�[��̐擪�A�h���X
	//! 
	//! @return �R�s�[��̖����A�h���X
	//! 
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator & /* allocator */, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( to, ptr, num * sizeof( typename Allocator::value_type ) );
		return( to + num );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^��obj��������
	//! 
	//! char �^�̏ꍇ�� memset �𗘗p����
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �������擪�A�h���X
	//! @param[in] num       �c �������I�u�W�F�N�g��
	//! @param[in] obj       �c �������I�u�W�F�N�g
	//! 
	template < class Allocator >
	static void fill_objects1( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, obj, num * sizeof( typename Allocator::value_type ) );
	}


	/// @brief �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	//! 
	//! memset �𗘗p���āC�S�v�f��0�ɏ���������
	//! 
	//! @param[in] allocator �c �g�p����A���P�[�^
	//! @param[in] ptr       �c �������擪�A�h���X
	//! @param[in] num       �c �������I�u�W�F�N�g��
	//! 
	template < class Allocator >
	static void fill_objects2( Allocator & /* allocator */, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cerr << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
	}
};



/// @brief MIST������STL�̃A���P�[�^�����ʓI�ɗ��p���邽�߂̎d�g��
//! 
//! �f�[�^�̌^�ɂ���āC�������������ʂ���������}���Ă���
//! 
//! @param T         �c �f�[�^�^
//! @param Allocator �c �A���P�[�^�^�C�v
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

protected:
	pointer   shared_pointer;		// ���p�\�ȃ������ւ̃|�C���^�i�O���ƃ����������L����ꍇ�̂ݗ��p�j 
	size_type shared_memory;		// ���p�\�ȃ������ʁi�O���ƃ����������L����ꍇ�̂ݗ��p�j 


public:
	/// @brief num �̃I�u�W�F�N�g�̃��������m�ۂ����������s���i�g�ݍ��݌^���ǂ����œ����̎����𕪊�j
	pointer allocate_objects( size_type num )
	{
		if( num == 0 || num > max_size( ) )
		{
			return( NULL );
		}
		else if( !is_memory_shared( ) )
		{
			// �����Ń����������蓖�Ă�ꍇ
			return( mist_memory_operator< is_builtin< T >::value >::allocate_objects1( *this, num ) );
		}
		else
		{
			// �O�����蓖�Ẵ������𗘗p����ꍇ
			return( shared_pointer );
		}
	}

	/// @brief num �̃I�u�W�F�N�g�̃��������m�ۂ� obj ��p���ď��������s���i�g�ݍ��݌^���ǂ����œ����̎����𕪊�j
	pointer allocate_objects( size_type num, const_reference obj )
	{
		if( num == 0 || num > max_size( ) )
		{
			return( NULL );
		}
		else if( !is_memory_shared( ) )
		{
			// �����Ń����������蓖�Ă�ꍇ
			return( mist_memory_operator< is_builtin< T >::value >::allocate_objects2( *this, num, obj ) );
		}
		else
		{
			// �O�����蓖�Ẵ������𗘗p����ꍇ
			mist_memory_operator< type_and< is_char< T >::value, is_builtin< T >::value >::value >::fill_objects1( *this, shared_pointer, num, obj );
			return( shared_pointer );
		}
	}

	/// @brief num �̃I�u�W�F�N�g���m�ۂ��C�|�C���^�ň͂܂ꂽ�Ԃ̃f�[�^�ŏ���������(�g�ݍ��݌^�̃f�[�^�̏ꍇ�� memcpy �𗘗p����)
	pointer allocate_objects( const_pointer s, const_pointer e )
	{
		if( s >= e || e - s > max_size( ) )
		{
			return( NULL );
		}
		else if( !is_memory_shared( ) )
		{
			// �����Ń����������蓖�Ă�ꍇ
			return( mist_memory_operator< is_builtin< T >::value >::allocate_objects3( *this, s, e ) );
		}
		else
		{
			// �O�����蓖�Ẵ������𗘗p����ꍇ
			mist_memory_operator< type_and< is_char< T >::value, is_builtin< T >::value >::value >::copy_objects2( *this, s, e - s, shared_pointer );
			return( shared_pointer );
		}
	}

	/// @brief num �̃I�u�W�F�N�g�̃��������J�����C�f�X�g���N�^���Ăяo���i�g�ݍ��݌^�̏ꍇ�̓f�X�g���N�^�͌Ă΂Ȃ��j
	void deallocate_objects( pointer ptr, size_type num )
	{
		if( !is_memory_shared( ) )
		{
			// �����Ń����������蓖�Ă�ꍇ
			if( num <= 0 ) return;
			mist_memory_operator< is_builtin< T >::value >::deallocate_objects( *this, ptr, num );
		}
	}

	/// @brief �|�C���^ s ���� e �܂ł̊Ԃ̃f�[�^�� x �ɃR�s�[����
	pointer copy_objects( const_pointer s, const_pointer e, pointer x ) const
	{
		if( s >= e ) return( x );
		return( mist_memory_operator< is_builtin< T >::value >::copy_objects1( *this, s, e, x ) );
	}

	/// @brief �|�C���^ ptr ���� num �̃f�[�^�� to �ɃR�s�[����
	pointer copy_objects( const_pointer ptr, size_type num, pointer to ) const
	{
		if( num <= 0 ) return( to );
		return( mist_memory_operator< is_builtin< T >::value >::copy_objects2( *this, ptr, num, to ) );
	}

	/// @brief �|�C���^ ptr ���� num �̃f�[�^��obj��������
	void fill_objects( pointer ptr, size_type num, const_reference obj ) const
	{
		if( num <= 0 ) return;
		mist_memory_operator< type_and< is_char< T >::value, is_builtin< T >::value >::value >::fill_objects1( *this, ptr, num, obj );
	}

	/// @brief �|�C���^ ptr ���� num �̃f�[�^�ɏ����l��������
	void fill_objects( pointer ptr, size_type num ) const
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_builtin< T >::value >::fill_objects2( *this, ptr, num );
	}

	/// @brief ptr ����n�܂� num ���݂���I�u�W�F�N�g�� dest_num �܂ŏk�߁C�S�Ă̗v�f���f�t�H���g�̒l�ŏ���������
	//! 
	//! @attention MSVC�̏ꍇ�́C�������̈�̃g�������삪���������삵�Ȃ����߁C�������̍Ċm�ۂ��s��
	//! 
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num )
	{
		if( num < dest_num ) return( ptr );			// �J�����悤�Ƃ��Ă���z��̗v�f���́C�g������̔z��T�C�Y���T�C�Y������������O
		//if( num < 0 ) return( NULL );				// �g������̔z��T�C�Y��0��菬������O
		if( num == dest_num ) return( ptr );		// �g�����ɂ��ύX�̕K�v�Ȃ�
		if( num == 0 ) return( NULL );

		if( !is_memory_shared( ) )
		{
			// �����Ń����������蓖�Ă�ꍇ
#if _MIST_ALLOCATOR_MEMORY_TRIM_ != 0
			deallocate_objects( ptr + dest_num, num - dest_num );
			fill_objects( ptr, dest_num );
#else
			deallocate_objects( ptr, num );
			ptr = allocate_objects( dest_num );
#endif
		}
		return( dest_num == 0 ? NULL : ptr );
	}

	/// @brief ptr ����n�܂� num ���݂���I�u�W�F�N�g�� dest_num �܂ŏk�߁C�S�Ă̗v�f��l obj �ŏ���������
	//! 
	//! @attention MSVC�̏ꍇ�́C�������̈�̃g�������삪���������삵�Ȃ����߁C�������̍Ċm�ۂ��s��
	//! 
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num, const_reference obj )
	{
		if( num < dest_num ) return( ptr );			// �J�����悤�Ƃ��Ă���z��̗v�f���́C�g������̔z��T�C�Y���T�C�Y������������O
		if( num < 0 ) return( NULL );				// �g������̔z��T�C�Y��0��菬������O
		if( num == dest_num ) return( ptr );		// �g�����ɂ��ύX�̕K�v�Ȃ�
		if( num == 0 ) return( NULL );

		if( !is_memory_shared( ) )
		{
			// �����Ń����������蓖�Ă�ꍇ
#if _MIST_ALLOCATOR_MEMORY_TRIM_ != 0
			deallocate_objects( ptr + dest_num, num - dest_num );
			fill_objects( ptr, dest_num, obj );
#else
			deallocate_objects( ptr, num );
			ptr = allocate_objects( dest_num, obj );
#endif
		}
		return( dest_num == 0 ? NULL : ptr );
	}

	/// @brief �g�p���Ă���A���P�[�^���m�ۉ\�ȃ������̍ő�l��Ԃ�
	size_type max_size( ) const
	{
		return( shared_memory == 0 ? base::max_size( ) : shared_memory );
	}

	/// @brief �O���Ŋ��蓖�Ă�ꂽ�������̈���g�p���Ă��邩�ǂ���
	bool is_memory_shared( ) const
	{
		return( shared_memory != 0 );
	}

	/// @brief ���̃A���P�[�^��������
	const Allocator &operator=( const Allocator &alloc )
	{
		if( &alloc != this )
		{
			// �O���������𗘗p���Ă��邩�ǂ����͎󂯌p���Ȃ�
			base::operator=( alloc );
		}
		return( *this );
	}

	/// @brief ���̃A���P�[�^��������
	const mist_allocator &operator=( const mist_allocator &alloc )
	{
		if( &alloc != this )
		{
			// �O���������𗘗p���Ă��邩�ǂ����͎󂯌p���Ȃ�
			base::operator=( alloc );
		}
		return( *this );
	}


	/// @brief �f�t�H���g�R���X�g���N�^
	mist_allocator( ) : base( ), shared_pointer( NULL ), shared_memory( 0 ){}


	/// @brief �f�t�H���g�R���X�g���N�^
	//! 
	//! @attention �O���Ŋ��蓖�Ă��������̈�𗘗p����ꍇ
	//! 
	/// @param[in] ptr       �c �O���������̐擪�̃|�C���^
	/// @param[in] mem_shared �c �O���������Ɋ��蓖�Ă���������
	//! 
	mist_allocator( pointer ptr, size_type mem_shared ) : base( ),  shared_pointer( mem_shared == 0 ? NULL : ptr ), shared_memory( ptr == NULL ? 0 : mem_shared ){}


	/// @brief ���̃A���P�[�^��p���ď���������
	mist_allocator( const Allocator &alloc ) : base( alloc ), shared_pointer( NULL ), shared_memory( 0 ){}


	/// @brief ���̃A���P�[�^��p���ď���������
	mist_allocator( const mist_allocator &alloc ) : base( alloc ), shared_pointer( NULL ), shared_memory( 0 ){}
};



// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_ALLOC_H__
