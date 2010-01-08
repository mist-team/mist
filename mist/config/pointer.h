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
#include <map>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN




namespace __shared_memory__
{
	template < class T >
	struct shared_base
	{
	private:
		typedef T* pointer;

		struct shared_memory_conter
		{
			typedef T* pointer;
			pointer ptr;
			size_t ref_count;
			size_t ref_weak_count;

			shared_memory_conter( ) : ptr( NULL ), ref_count( 0 ), ref_weak_count( 0 ){ }
			shared_memory_conter( pointer p, size_t rc, size_t rwc ) : ptr( p ), ref_count( rc ), ref_weak_count( rwc ){ }
		};

		typedef shared_memory_conter counter_type;
		typedef std::map< pointer, counter_type > ref_table_type;
		typedef typename ref_table_type::iterator ref_table_iterator;

	protected:
		static ref_table_type &get_ref_table( )
		{
			static ref_table_type ref_table_singleton_;
			return( ref_table_singleton_ );
		}

		static ref_table_iterator get_ref_iterator( pointer p )
		{
			ref_table_type &ref_table_ = get_ref_table( );

			if( p == NULL )
			{
				return( get_null_reference( ) ); 
			}
			else
			{
				ref_table_iterator ite = ref_table_.find( p );

				if( ite == ref_table_.end( ) )
				{
					// �ŏ��̒ǉ��̂��߁C�e�[�u��������������
					ite = ref_table_.insert( typename ref_table_type::value_type( p, counter_type( p, 0, 0 ) ) ).first;
				}
				return( ite ); 
			}
		}

		static ref_table_iterator get_null_reference( )
		{
			ref_table_type &ref_table_ = get_ref_table( );
			// NULL�Q�Ɨp�̃f�[�^�����炩���ߑ}�����Ă���
			static ref_table_iterator null_ite_ = ref_table_.insert( typename ref_table_type::value_type( NULL, counter_type( NULL, 0, 0 ) ) ).first;
			return( null_ite_ ); 
		}

		ref_table_iterator ref_ite_;

		shared_base( ) : ref_ite_( get_null_reference( ) ){ }
		shared_base( const shared_base &b ) : ref_ite_( b.ref_ite_ ){}

		pointer get_pointer( ) const { return( ref_ite_->second.ptr ); }

	protected:
		// null �Q�Ƃɐݒ肷��
		void null_ref( )
		{
			ref_ite_ = get_null_reference( );
		}

		// �Q�Ɨp�̎Q�ƃJ�E���g�𑝉�������
		void add_ref( )
		{
			// �Q�ƃJ�E���g�𑝂₷
			ref_ite_->second.ref_count++;
		}

		// �Q�Ɨp�̎Q�ƃJ�E���g�𑝉�������
		void add_ref( pointer p )
		{
			ref_ite_ = get_ref_iterator( p );

			// �Q�ƃJ�E���g�𑝂₷
			ref_ite_->second.ref_count++;
		}

		// �Q�Ɨp�̎Q�ƃJ�E���g�𑝉�������
		void add_ref( const shared_base &p )
		{
			ref_ite_ = p.ref_ite_;

			// �Q�ƃJ�E���g�𑝂₷
			ref_ite_->second.ref_count++;
		}

		// ��Q�Ɨp�̎Q�ƃJ�E���g�𑝉�������
		void add_weak_ref( )
		{
			// �Q�ƃJ�E���g�𑝂₷
			ref_ite_->second.ref_weak_count++;
		}

		// ��Q�Ɨp�̎Q�ƃJ�E���g�𑝉�������
		void add_weak_ref( pointer p )
		{
			ref_ite_ = get_ref_iterator( p );

			// �Q�ƃJ�E���g�𑝂₷
			ref_ite_->second.ref_weak_count++;
		}

		// ��Q�Ɨp�̎Q�ƃJ�E���g�𑝉�������
		void add_weak_ref( const shared_base &p )
		{
			ref_ite_ = p.ref_ite_;

			// �Q�ƃJ�E���g�𑝂₷
			ref_ite_->second.ref_weak_count++;
		}

		void release( bool isArray )
		{
			counter_type &c = ref_ite_->second;
			if( c.ptr != NULL )
			{
				// �Q�ƃJ�E���g�����炷
				c.ref_count--;

				// �Q�ƃJ�E���g�ɂ���āC�������̊J�����s��
				if( c.ref_count == 0 )
				{
					// �Q�ƃJ�E���g�� 0 �ɂȂ����̂ŁC���������J�����e�[�u������폜����
					if( isArray )
					{
						delete [] c.ptr;
					}
					else
					{
						delete c.ptr;
					}

					// �|�C���^�ɑ΂����Q�Ƃ����݂��Ȃ��ꍇ�́CNULL��������
					if( c.ref_weak_count == 0 )
					{
						ref_table_type &table = get_ref_table( );
						table.erase( ref_ite_ );
					}
					else
					{
						c.ptr = NULL;
					}
				}
			}
		}

		void release_weak( )
		{
			counter_type &c = ref_ite_->second;

			if( c.ptr != NULL )
			{
				// ��Q�ƃJ�E���g�����炷
				c.ref_weak_count--;

				if( c.ref_weak_count == 0 && c.ref_count == 0 )
				{
					// �|�C���^�ɑ΂���Q�ƂƎ�Q�Ƃ����݂��Ȃ��ꍇ�́C�e�[�u������폜����
					ref_table_type &table = get_ref_table( );
					table.erase( ref_ite_ );
				}
			}
		}
	};
}



//! @addtogroup pointer_group �������̈�̎����J�����s���|�C���^
//!
//! @code ���̃w�b�_���Q��
//! #include <mist/config/pointer.h>
//! @endcode
//!
//! @{


/// @brief �I�u�W�F�N�g�̃X�R�[�v���؂��Ǝ����I�Ƀ������J�����s���|�C���^
//! 
//! @attention �z��ɑ΂��Ă͗��p�ł��Ȃ�
//! @attention �Ǘ����Ă���|�C���^�𑼂̃I�u�W�F�N�g�ɏ��n���邱�Ƃ͂ł��Ȃ�
//! 
//! @code �|�C���^�̎g�p��
//! {
//!     mist::scoped_ptr< std::string > a( new std::string( "test" ) );
//!	    std::cout << *a << std::endl;
//! } // <- ������ a �Ɋ��蓖�Ă��������̈悪�C�����I�ɊJ�������
//! @endcode
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
	/// @brief �Ǘ�����|�C���^��ݒ�
	scoped_ptr( pointer p ) : ptr_( p ){ }

	/// @brief �Ǘ�����|�C���^���J��
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
//! @attention �Ǘ����Ă���|�C���^�𑼂̃I�u�W�F�N�g�ɏ��n���邱�Ƃ͂ł��Ȃ�
//! 
//! @code �|�C���^�̎g�p��
//! {
//!     mist::scoped_array< std::string > a( new std::string[ 2 ] );
//!	
//!	    a[ 0 ] = "test1";
//!	    a[ 1 ] = "test2";
//!	
//!	    std::cout << a[ 0 ] << a[ 1 ] << std::endl;
//! } // <- ������ a �Ɋ��蓖�Ă��������̈悪�C�����I�ɊJ�������
//! @endcode
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



/// @brief �I�u�W�F�N�g�ւ̃|�C���^�̎Q�ƃJ�E���g���Ǘ����C�ǂ�������Q�Ƃ���Ȃ��Ȃ����Ƃ��Ƀ������������J������|�C���^
//! 
//! @attention �z��ɑ΂��Ă͗��p�ł��Ȃ�
//! @attention �z�Q�Ƃ����������ꍇ�́C�������Ǘ��ł��Ȃ�
//! 
//! @code �|�C���^�̎g�p��
//! {
//!     mist::shared_ptr< std::string > a;
//!     {
//!         mist::shared_ptr< std::string > b = new std::string( "test" );
//!	
//!	        a = b;    // a �� b �Ɠ����������̈���Q�Ƃ���悤�ɂ��C�Q�ƃJ�E���g�𑝂₷
//!	
//!	    } // <- �����ł� b �Ɋ��蓖�Ă��������̈�̎Q�ƃJ�E���g�����邪�Ca �������̈���Q�Ƃ��Ă���̂ŁC�������̈�͊J������Ȃ�
//!	
//!  ...
//!	
//! } // <- ������ a �Ɋ��蓖�Ă��������̈悪�ǂ�������Q�Ƃ���Ȃ��Ȃ邽�߁C�������̈悪�����I�ɊJ�������
//! @endcode
//! 
template < class T >
class shared_ptr : public __shared_memory__::shared_base< T >
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
	typedef __shared_memory__::shared_base< T > base;


public:
	/// @brief ���Ǘ��̃|�C���^�ŏ�����
	shared_ptr( ){ }

	/// @brief �Ǘ�����|�C���^��ݒ肵�C�Q�ƃJ�E���g�� 1 ���₷
	shared_ptr( pointer p ){ base::add_ref( p ); }

	/// @brief ���̋��L�������|�C���^��p���ď��������C�Q�ƃJ�E���g�� 1 ���₷
	shared_ptr( const shared_ptr &p ) : base( p ){ base::add_ref( ); }

	/// @brief �Ǘ�����|�C���^�̎Q�ƃJ�E���g��1���炷
	//!
	//! �ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	~shared_ptr( ){ base::release( false ); }


	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const shared_ptr &operator =( const shared_ptr &p )
	{
		if( &p != this )
		{
			base::release( false );
			base::add_ref( p );
		}

		return( *this );
	}

public:
	/// @brief �|�C���^���w�����e�ւ̎Q�Ƃ�Ԃ�
	reference operator *( ){ return( *base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̎Q�Ƃ�Ԃ�
	const_reference operator *( ) const { return( *base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̃A�N�Z�X���s��
	pointer operator ->( ){ return( base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̃A�N�Z�X���s��
	const_pointer operator ->( ) const { return( base::get_pointer( ) ); }



	/// @brief �Ǘ����Ă���|�C���^�̎Q�ƃJ�E���g�����炷
	//!
	//! @attention �ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	void reset( )
	{ 
		base::release( false );
		base::null_ref( );
	}

public:
	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const shared_ptr &p ) const { return( base::get_pointer( ) == p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const shared_ptr &p ) const { return( base::get_pointer( ) != p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const shared_ptr &p ) const { return( base::get_pointer( ) <  p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const shared_ptr &p ) const { return( base::get_pointer( ) <= p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const shared_ptr &p ) const { return( base::get_pointer( ) >  p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const shared_ptr &p ) const { return( base::get_pointer( ) >= p.get_pointer( ) ); }


	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const pointer &p ) const { return( base::get_pointer( ) == p ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const pointer &p ) const { return( base::get_pointer( ) != p ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const pointer &p ) const { return( base::get_pointer( ) <  p ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const pointer &p ) const { return( base::get_pointer( ) <= p ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const pointer &p ) const { return( base::get_pointer( ) >  p ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const pointer &p ) const { return( base::get_pointer( ) >= p ); }

};

template < class T > inline bool operator ==( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }




/// @brief �z��ւ̃|�C���^�̎Q�ƃJ�E���g���Ǘ����C�ǂ�������Q�Ƃ���Ȃ��Ȃ����Ƃ��Ƀ������������J������|�C���^
//! 
//! �Ώۂ��z��Ƃ������Ƃ������΁Cshared_ptr �Ɠ���
//! 
//! @attention �z��ɑ΂��Ă������p�ł��Ȃ�
//! @attention �z�Q�Ƃ����������ꍇ�́C�������Ǘ��ł��Ȃ�
//! 
//! @code �|�C���^�̎g�p��
//! {
//!     mist::shared_ptr< std::string > a;
//!     {
//!         mist::shared_ptr< std::string > b = new std::string[ 2 ];
//!	
//!	        a = b;    // a �� b �Ɠ����������̈���Q�Ƃ���悤�ɂ��C�Q�ƃJ�E���g�𑝂₷
//!	
//!	        a[ 0 ] = a[ 1 ] = "test";
//!	
//!	    } // <- �����ł� b �Ɋ��蓖�Ă��������̈�̎Q�ƃJ�E���g�����邪�Ca �������̈���Q�Ƃ��Ă���̂ŁC�������̈�͊J������Ȃ�
//!	
//!  ...
//!	
//! } // <- ������ a �Ɋ��蓖�Ă��������̈悪�ǂ�������Q�Ƃ���Ȃ��Ȃ邽�߁C�������̈悪�����I�ɊJ�������
//! @endcode
//! 
template < class T >
class shared_array : public __shared_memory__::shared_base< T >
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
	typedef __shared_memory__::shared_base< T > base;

public:
	/// @brief ���Ǘ��̃|�C���^�ŏ�����
	shared_array( ){ }

	/// @brief �Ǘ�����|�C���^��ݒ肵�C�Q�ƃJ�E���g�� 1 ���₷
	shared_array( pointer p ){ base::add_ref( p ); }

	/// @brief ���̋��L�������|�C���^��p���ď��������C�Q�ƃJ�E���g�� 1 ���₷
	shared_array( const shared_array &p ) : base( ) { base::add_ref( p ); }

	/// @brief �Ǘ�����|�C���^�̎Q�ƃJ�E���g��1���炷
	//!
	//! �ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	~shared_array( ){ base::release( true ); }


	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const shared_array &operator =( const shared_array &p )
	{
		if( &p != this )
		{
			base::release( true );
			base::add_ref( p );
		}

		return( *this );
	}

public:
	/// @brief �|�C���^���w�����e�ւ̎Q�Ƃ�Ԃ�
	reference operator *( ){ return( *base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̎Q�Ƃ�Ԃ�
	const_reference operator *( ) const { return( *base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̃A�N�Z�X���s��
	pointer operator ->( ){ return( base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̃A�N�Z�X���s��
	const_pointer operator ->( ) const { return( base::get_pointer( ) ); }

	/// @brief �z��̗v�f�ւ̃A�N�Z�X���s��
	reference operator []( difference_type index ){ return( base::get_pointer( )[ index ] ); }

	/// @brief �z��̗v�f�ւ̃A�N�Z�X���s��
	const_reference operator []( difference_type index ) const { return( base::get_pointer( )[ index ] ); }


	/// @brief �Ǘ����Ă���|�C���^�̎Q�ƃJ�E���g�����炷
	//!
	//! @attention �ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	void reset( )
	{ 
		base::release( true );
		base::null_ref( );
	}


public:
	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const shared_array &p ) const { return( base::get_pointer( ) == p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const shared_array &p ) const { return( base::get_pointer( ) != p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const shared_array &p ) const { return( base::get_pointer( ) <  p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const shared_array &p ) const { return( base::get_pointer( ) <= p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const shared_array &p ) const { return( base::get_pointer( ) >  p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const shared_array &p ) const { return( base::get_pointer( ) >= p.get_pointer( ) ); }


	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const pointer &p ) const { return( base::get_pointer( ) == p ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const pointer &p ) const { return( base::get_pointer( ) != p ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const pointer &p ) const { return( base::get_pointer( ) <  p ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const pointer &p ) const { return( base::get_pointer( ) <= p ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const pointer &p ) const { return( base::get_pointer( ) >  p ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const pointer &p ) const { return( base::get_pointer( ) >= p ); }

};

template < class T > inline bool operator ==( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 >  p1 ) ); }



/// @brief �Q�ƃJ�E���g�^�̃������ɑ΂���C��Q�Ƃ������|�C���^
//! 
//! �Q�ƃJ�E���g�� 0 �ɂ����ۂɁC�ێ�����|�C���^�� NULL �ƂȂ�
//! 
//! @attention weak_ptr ���g�͎Q�ƃJ�E���g��ω������Ȃ�
//! 
//! @code ��Q�ƃ|�C���^�̎g�p��
//! {
//! mist::weak_ptr< std::string > a;
//!	
//! std::cout << a << std::endl;    // <- a �͂ǂ����Q�Ƃ��Ă��Ȃ��̂� NULL  �������Ă���
//!	
//! {
//!     mist::shared_ptr< std::string > b = new std::string[ 2 ];
//!	
//!	    a = b;    // b �̎�Q�Ƃ� a �ɐݒ肷��
//!	
//!	    std::cout << a << std::endl;    // <- a �� b ���Q�Ƃ��Ă���̂� b �̎Q�Ɛ悪�\�������
//!	
//!	} // <- ������ b �Ɋ��蓖�Ă��������̈悪�ǂ�������Q�Ƃ���Ȃ��Ȃ邽�߁C�������̈悪�����I�ɊJ�������
//!	
//! std::cout << a << std::endl;    // <- a ���Q�Ƃ��Ă��� b �͂������݂��Ȃ��̂ŁC NULL �������Ă���
//! @endcode
//! 
template < class T >
class weak_ptr : public __shared_memory__::shared_base< T >
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
	typedef __shared_memory__::shared_base< T > base;

public:
	/// @brief ���Ǘ��̃|�C���^�ŏ�����
	weak_ptr( ){ base::add_weak_ref( NULL ); }

	/// @brief ���L�������|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( shared_ptr< T > &p ){ base::add_weak_ref( p ); }

	/// @brief ���L�������|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( const shared_ptr< T > &p ){ base::add_weak_ref( p ); }

	/// @brief ���L�������^�̔z��|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( shared_array< T > &p ){ base::add_weak_ref( p ); }

	/// @brief ���L�������^�̔z��|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( const shared_array< T > &p ){ base::add_weak_ref( p ); }

	/// @brief ��Q�Ƃ̎Q�ƃJ�E���g��1���炷
	~weak_ptr( ){ base::release_weak( ); }


	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( const weak_ptr &p )
	{
		if( this != &p )
		{
			base::release_weak( );
			base::add_weak_ref( p );
		}

		return( *this );
	}

	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( const shared_ptr< T > &p )
	{
		base::release_weak( );
		base::add_weak_ref( p );

		return( *this );
	}

	/// @brief ���̋��L�������^�̔z��|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( const shared_array< T > &p )
	{
		base::release_weak( );
		base::add_weak_ref( p );

		return( *this );
	}


public:
	/// @brief �|�C���^���w�����e�ւ̎Q�Ƃ�Ԃ�
	reference operator *( ){ return( *base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̎Q�Ƃ�Ԃ�
	const_reference operator *( ) const { return( *base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̃A�N�Z�X���s��
	pointer operator ->( ){ return( base::get_pointer( ) ); }

	/// @brief �|�C���^���w�����e�ւ̃A�N�Z�X���s��
	const_pointer operator ->( ) const { return( base::get_pointer( ) ); }

	/// @brief �z��̗v�f�ւ̃A�N�Z�X���s��
	reference operator []( difference_type index ){ return( base::get_pointer( )[ index ] ); }

	/// @brief �z��̗v�f�ւ̃A�N�Z�X���s��
	const_reference operator []( difference_type index ) const { return( base::get_pointer( )[ index ] ); }


	/// @brief �Ǘ����Ă���|�C���^�̎Q�ƃJ�E���g�����炷
	//!
	//! @attention �ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	void reset( )
	{ 
		base::release_weak( );
		base::null_ref( );
	}


public:
	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const weak_ptr &p ) const { return( base::get_pointer( ) == p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const weak_ptr &p ) const { return( base::get_pointer( ) != p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const weak_ptr &p ) const { return( base::get_pointer( ) <  p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const weak_ptr &p ) const { return( base::get_pointer( ) <= p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const weak_ptr &p ) const { return( base::get_pointer( ) >  p.get_pointer( ) ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const weak_ptr &p ) const { return( base::get_pointer( ) >= p.get_pointer( ) ); }


	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const pointer &p ) const { return( base::get_pointer( ) == p ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const pointer &p ) const { return( base::get_pointer( ) != p ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const pointer &p ) const { return( base::get_pointer( ) <  p ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const pointer &p ) const { return( base::get_pointer( ) <= p ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const pointer &p ) const { return( base::get_pointer( ) >  p ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const pointer &p ) const { return( base::get_pointer( ) >= p ); }

};


template < class T > inline bool operator ==( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename weak_ptr< T >::pointer p1, const
											 
											 weak_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }


/// @brief �w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     p   �c scoped_ptr �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const scoped_ptr< T > &p )
{
	out << &( *p );
	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     p   �c scoped_array �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const scoped_array< T > &p )
{
	out << &( *p );
	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     p   �c shared_ptr �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const shared_ptr< T > &p )
{
	out << &( *p );
	return( out );
}

/// @brief �w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     p   �c weak_ptr �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const weak_ptr< T > &p )
{
	out << &( *p );
	return( out );
}

/// @}
//  �������̈�̎����J�����s���|�C���^



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_POINTER_H__
