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


// MIST�ŗ��p������̃f�[�^�^


/// @brief �I�u�W�F�N�g�̃X�R�[�v���؂��Ǝ����I�Ƀ������J�����s���|�C���^
//! 
//! @attention �z��ɑ΂��Ă͗��p�ł��Ȃ�
//! @attention �Ǘ����Ă���|�C���^�𑼂̃I�u�W�F�N�g�ɏ��n���邱�Ƃ͂ł��Ȃ�
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



namespace __shared_memory__
{
	template < class T >
	struct shared_memory_controller
	{
	private:
		typedef T* pointer;

		struct shared_memory_conter
		{
			typedef T* pointer;
			bool isArray;
			pointer ptr;
			size_t ref_count;
			size_t ref_weak_count;

			shared_memory_conter( ) : isArray( false ), ptr( NULL ), ref_count( 0 ), ref_weak_count( 0 ){ }
			shared_memory_conter( bool barray, pointer p, size_t rc, size_t rwc ) : isArray( barray ), ptr( p ), ref_count( rc ), ref_weak_count( rwc ){ }
		};

		typedef std::map< pointer, shared_memory_conter > ref_table_type;


	public:
		static ref_table_type &get_ref_table( )
		{
			static ref_table_type singleton_;
			return( singleton_ );
		}

		static void add_ref( pointer p, bool isArray )
		{
			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite == table.end( ) )
				{
					// �ŏ��̒ǉ��̂��߁C�e�[�u��������������
					table[ p ] = shared_memory_conter( isArray, p, 1, 0 );
				}
				else
				{
					// �Q�ƃJ�E���g�𑝂₷
					table[ p ].ref_count++;
				}
			}
		}

		// ��Q�Ɨp�̎Q�ƃJ�E���g�𑝉������C�Q�Ɨp�|�C���^��Ԃ�
		static pointer * add_weak_ref( pointer p )
		{

			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite != table.end( ) )
				{
					// ��Q�ƃJ�E���g�𑝂₷
					shared_memory_conter &c = ite->second;
					c.ref_weak_count++;

					return( &c.ptr );
				}
			}

			// �s�K�؂ȎQ�Ƃ̏ꍇ�p
			static pointer dmy_ptr = NULL;
			return( &dmy_ptr );
		}

		static void release( pointer p )
		{
			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite != table.end( ) )
				{
					shared_memory_conter &c = ite->second;

					// �Q�ƃJ�E���g�����炷
					c.ref_count --;

					// �Q�ƃJ�E���g�ɂ���āC�������̊J�����s��
					if( c.ref_count == 0 )
					{
						// �Q�ƃJ�E���g�� 0 �ɂȂ����̂ŁC���������J�����e�[�u������폜����
						if( c.isArray )
						{
							delete [] p;
						}
						else
						{
							delete p;
						}

						// �|�C���^�ɑ΂����Q�Ƃ����݂��Ȃ��ꍇ�́CNULL��������
						if( c.ref_weak_count == 0 )
						{
							table.erase( ite );
						}
						else
						{
							c.ptr = NULL;
						}
					}
				}
			}
		}

		static void release_weak( pointer p )
		{
			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite != table.end( ) )
				{
					shared_memory_conter &c = ite->second;

					// ��Q�ƃJ�E���g�����炷
					c.ref_weak_count--;

					if( c.ref_weak_count == 0 && c.ref_count == 0 )
					{
						// �|�C���^�ɑ΂����Q�Ƃ����݂��Ȃ��ꍇ�́CNULL��������
						table.erase( ite );
					}
				}
			}
		}
	};
}


/// @brief �I�u�W�F�N�g�ւ̃|�C���^�̎Q�ƃJ�E���g���Ǘ����C�ǂ�������Q�Ƃ���Ȃ��Ȃ����Ƃ��Ƀ������������J������|�C���^
//! 
//! @attention �z��ɑ΂��Ă͗��p�ł��Ȃ�
//! @attention �z�Q�Ƃ����������ꍇ�́C�������Ǘ��ł��Ȃ�
//! 
template < class T >
class shared_ptr
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
	typedef __shared_memory__::shared_memory_controller< T > shared_memory_controller;

	pointer ptr_;		///< @brief �Ǘ�����|�C���^


public:
	/// @brief ���Ǘ��̃|�C���^�ŏ�����
	shared_ptr( ) : ptr_( NULL ){ }

	/// @brief �Ǘ�����|�C���^��ݒ肵�C�Q�ƃJ�E���g�� 1 ���₷
	shared_ptr( pointer p ) : ptr_( p ){ shared_memory_controller::add_ref( ptr_, false ); }

	/// @brief ���̋��L�������|�C���^��p���ď��������C�Q�ƃJ�E���g�� 1 ���₷
	shared_ptr( const shared_ptr &p ) : ptr_( p.ptr_ ){ shared_memory_controller::add_ref( ptr_, false ); }

	/// @brief �Ǘ�����|�C���^�̎Q�ƃJ�E���g��1���炷
	//!
	//! �ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	~shared_ptr( ){ shared_memory_controller::release( ptr_ ); }


	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const shared_ptr &operator =( const shared_ptr &p )
	{
		if( &p != this )
		{
			shared_memory_controller::release( ptr_ );
			shared_memory_controller::add_ref( ptr_ = p.ptr_, false );
		}

		return( *this );
	}

public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }


public:
	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const shared_ptr &p ) const { return( ptr_ == p.ptr_ ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const shared_ptr &p ) const { return( ptr_ != p.ptr_ ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const shared_ptr &p ) const { return( ptr_ <  p.ptr_ ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const shared_ptr &p ) const { return( ptr_ <= p.ptr_ ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const shared_ptr &p ) const { return( ptr_ >  p.ptr_ ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const shared_ptr &p ) const { return( ptr_ >= p.ptr_ ); }


	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const pointer &p ) const { return( ptr_ == p ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const pointer &p ) const { return( ptr_ != p ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const pointer &p ) const { return( ptr_ <  p ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const pointer &p ) const { return( ptr_ <= p ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const pointer &p ) const { return( ptr_ >  p ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const pointer &p ) const { return( ptr_ >= p ); }

};

template < class T > inline bool operator ==( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }




/// @brief �z��ւ̃|�C���^�̎Q�ƃJ�E���g���Ǘ����C�ǂ�������Q�Ƃ���Ȃ��Ȃ����Ƃ��Ƀ������������J������|�C���^
//! 
//! @attention �z��ɑ΂��Ă������p�ł��Ȃ�
//! @attention �z�Q�Ƃ����������ꍇ�́C�������Ǘ��ł��Ȃ�
//! 
template < class T >
class shared_array
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
	typedef __shared_memory__::shared_memory_controller< T > shared_memory_controller;

	pointer ptr_;		///< @brief �Ǘ�����|�C���^


public:
	/// @brief ���Ǘ��̃|�C���^�ŏ�����
	shared_array( ) : ptr_( NULL ){ }

	/// @brief �Ǘ�����|�C���^��ݒ肵�C�Q�ƃJ�E���g�� 1 ���₷
	shared_array( pointer p ) : ptr_( p ){ shared_memory_controller::add_ref( ptr_, true ); }

	/// @brief ���̋��L�������|�C���^��p���ď��������C�Q�ƃJ�E���g�� 1 ���₷
	shared_array( const shared_array &p ) : ptr_( p.ptr_ ){ shared_memory_controller::add_ref( ptr_, true ); }

	/// @brief �Ǘ�����|�C���^�̎Q�ƃJ�E���g��1���炷
	//!
	//! �ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	~shared_array( ){ shared_memory_controller::release( ptr_ ); }


	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const shared_array &operator =( const shared_array &p )
	{
		if( &p != this )
		{
			shared_memory_controller::release( ptr_ );
			shared_memory_controller::add_ref( ptr_ = p.ptr_, true );
		}

		return( *this );
	}

public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }

	reference operator []( difference_type index ){ return( ptr_[ index ] ); }
	const_reference operator []( difference_type index ) const { return( ptr_[ index ] ); }


public:
	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const shared_array &p ) const { return( ptr_ == p.ptr_ ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const shared_array &p ) const { return( ptr_ != p.ptr_ ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const shared_array &p ) const { return( ptr_ <  p.ptr_ ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const shared_array &p ) const { return( ptr_ <= p.ptr_ ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const shared_array &p ) const { return( ptr_ >  p.ptr_ ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const shared_array &p ) const { return( ptr_ >= p.ptr_ ); }


	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const pointer &p ) const { return( ptr_ == p ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const pointer &p ) const { return( ptr_ != p ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const pointer &p ) const { return( ptr_ <  p ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const pointer &p ) const { return( ptr_ <= p ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const pointer &p ) const { return( ptr_ >  p ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const pointer &p ) const { return( ptr_ >= p ); }

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
template < class T >
class weak_ptr
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
	typedef __shared_memory__::shared_memory_controller< T > shared_memory_controller;

	pointer ptr_;		///< @brief ��Q�Ɛ�̃|�C���^(��Q�Ƃ̊J���p)
	pointer *pptr_;		///< @brief ��Q�Ɛ�̃|�C���^


public:
	/// @brief ���Ǘ��̃|�C���^�ŏ�����
	weak_ptr( ) : ptr_( NULL ), pptr_( shared_memory_controller::add_weak_ref( NULL ) ){ }

	/// @brief ���L�������|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( shared_ptr< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief ���L�������|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( const shared_ptr< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief ���L�������^�̔z��|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( shared_array< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief ���L�������^�̔z��|�C���^�̎�Q�Ƃ�p���ď���������
	weak_ptr( const shared_array< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief ��Q�Ƃ̎Q�ƃJ�E���g��1���炷
	~weak_ptr( ){ shared_memory_controller::release_weak( ptr_ ); }


	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( weak_ptr &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}

	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( shared_ptr< T > &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}

	/// @brief ���̋��L�������|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( const shared_ptr< T > &p )
	{
		if( &p != this )
		{
			shared_memory_controller::release_weak( *pptr_ );
			pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );
		}

		return( *this );
	}

	/// @brief ���̋��L�������^�̔z��|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( shared_array< T > &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}

	/// @brief ���̋��L�������^�̔z��|�C���^��������
	//!
	//! @attention ���łɊǗ����Ă���|�C���^������ꍇ�́C�Q�ƃJ�E���g��1���炷
	//! @attention ���̍ۂɁC�ǂ�������Q�Ƃ���Ȃ��Ȃ����烁�������J������
	//! 
	const weak_ptr &operator =( const shared_array< T > &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}


public:
	reference operator *( ){ return( **pptr_ ); }
	const_reference operator *( ) const { return( **pptr_ ); }

	pointer operator ->( ){ return( *pptr_ ); }
	const_pointer operator ->( ) const { return( *pptr_ ); }

	reference operator []( difference_type index ){ return( ptr_[ index ] ); }
	const_reference operator []( difference_type index ) const { return( ptr_[ index ] ); }


public:
	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const weak_ptr &p ) const { return( *pptr_ == *p.pptr_ ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const weak_ptr &p ) const { return( *pptr_ != *p.pptr_ ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const weak_ptr &p ) const { return( *pptr_ <  *p.pptr_ ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const weak_ptr &p ) const { return( *pptr_ <= *p.pptr_ ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const weak_ptr &p ) const { return( *pptr_ >  *p.pptr_ ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const weak_ptr &p ) const { return( *pptr_ >= *p.pptr_ ); }


	/// @brief 2�̃|�C���^�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const pointer &p ) const { return( *pptr_ == p ); }

	/// @brief 2�̃|�C���^���������Ȃ��ǂ����𔻒肷��
	bool operator !=( const pointer &p ) const { return( *pptr_ != p ); }

	/// @brief 2�̃|�C���^�� <  �𔻒肷��
	bool operator < ( const pointer &p ) const { return( *pptr_ <  p ); }

	/// @brief 2�̃|�C���^�� <= �𔻒肷��
	bool operator <=( const pointer &p ) const { return( *pptr_ <= p ); }

	/// @brief 2�̃|�C���^�� >  �𔻒肷��
	bool operator > ( const pointer &p ) const { return( *pptr_ >  p ); }

	/// @brief 2�̃|�C���^�� >= �𔻒肷��
	bool operator >=( const pointer &p ) const { return( *pptr_ >= p ); }

};


template < class T > inline bool operator ==( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }


/// @brief �w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c scoped_ptr �z��
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
//! @param[in]     a   �c scoped_array �z��
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
//! @param[in]     a   �c shared_ptr �z��
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
//! @param[in]     a   �c weak_ptr �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const weak_ptr< T > &p )
{
	out << &( *p );
	return( out );
}



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_POINTER_H__
