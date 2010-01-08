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

/// @file mist/thread.h
//!
//! @brief �ȕւɃX���b�h�𗘗p�\�ɂ��邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_THREAD__
#define __INCLUDE_MIST_THREAD__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_SINGLETON__
#include "singleton.h"
#endif


// �X���b�h�p���C�u�����̃C���N���[�h
// UNIX�n��Windows�p���g��������
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�v�ȃC���N���[�h�t�@�C���͖���
	#define __THREAD_POOL_SUPPORT__		0
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	#if !defined( _WIN32_WINNT )
		#define __THREAD_POOL_SUPPORT__		0
		#pragma message( "To use mist thread pool functionality, you must define _WIN32_WINNT (>=0x0400) in the project setting page." )
	#elif _WIN32_WINNT < 0x0400
		#define __THREAD_POOL_SUPPORT__		0
		#pragma message( "To use mist thread pool functionality, _WIN32_WINNT must be greater than 0x0400." )
	#else
		#define __THREAD_POOL_SUPPORT__		1
	#endif

	#include <windows.h>
	#include <process.h>
	#include <mmsystem.h>
	#pragma comment ( lib, "winmm.lib" )
#else
	#include <pthread.h>
	#include <unistd.h>
	#include <time.h>
	#include <errno.h>
	#include <sys/time.h>
	#define __THREAD_POOL_SUPPORT__		1
#endif

#ifndef INFINITE
#define INFINITE	( ( unsigned long ) -1 )
#endif

#include <map>
#include <vector>
#include <list>
#include <string>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



// �X���b�h�p�֐�
typedef unsigned long ThreadExitCode;
typedef ThreadExitCode ( LPTHREADFUNC ) ( void *thread_param );


// �X���b�h�𑀍삷��C���ʂ���ϐ�
struct thread_dmy_class{ };

// �X���b�h�𑀍삷���ԃx�[�X�ƂȂ�N���X
struct thread_object
{
	virtual bool create( ) = 0;
	virtual bool wait( unsigned long dwMilliseconds = INFINITE ) = 0;
	virtual bool close( ) = 0;
	virtual ~thread_object( ){ }
};


//! @addtogroup thread_group �X���b�h
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/thread.h>
//! @endcode
//!
//!  @{


/// @brief ���p�\��CPU�����擾����
//! 
//! Windows�̏ꍇ�� GetSystemInfo �֐��𗘗p����CPU�����擾����D
//! Linux�n�̏ꍇ�́Csysconf �֐��𗘗p����CPU�����擾����D
//! 
//! @return ���p�\��CPU��
//! 
inline size_t get_cpu_num( )
{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// �X���b�h�T�|�[�g�͂��Ȃ��̂�CPU���͏��1
	return( 1 );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );
	return( static_cast< size_t >( sysInfo.dwNumberOfProcessors ) );
#else
	return( static_cast< size_t >( sysconf( _SC_NPROCESSORS_ONLN ) ) );
#endif
}


/// @brief �w�肵�����Ԃ����X���[�v����i�~���b�P�ʁj
inline void sleep( size_t dwMilliseconds )
{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	SleepEx( static_cast< DWORD >( dwMilliseconds ), false );
#else
	timespec treq, trem;
	treq.tv_sec = static_cast< time_t >( dwMilliseconds / 1000 );
	treq.tv_nsec = static_cast< long >( ( dwMilliseconds % 1000 ) * 1000000 );
	
	while( nanosleep( &treq, &trem ) != 0 )
	{
		treq = trem;
	}
#endif
}


/// @brief �X���b�h�̔r��������T�|�[�g����ł��P���ȃN���X
//! 
//! ���b�N����I�u�W�F�N�g�����L����ꍇ�Ɏg�p����ł��V���v���ȃN���X
//! 
//! @attention ���b�N�I�u�W�F�N�g�̐������_�ł́C���b�N�͂���Ȃ����Ƃɒ���
//! 
//! - Windows �c �N���e�B�J���Z�N�V�����𗘗p
//! - Linux�n �c �~���[�e�b�N�X�𗘗p
//! 
//! @code �g�p��
//! simple_lock_object l;
//! 
//! ... ���炩�̏��� ...
//! 
//! l.lock( );   // <- ����ȍ~�̏���1��r�����䂷��
//! 
//! ... ����1 ...
//! 
//! l.unlock( ); // <- �����܂ł̏���1���r�����䂳���
//! 
//! 
//! ... ���炩�̏��� ...
//! 
//! 
//! l.lock( );   // <- ����ȍ~�̏���2��r�����䂷��
//! 
//! ... ����2 ...
//! 
//! l.unlock( ); // <- �����܂ł̏���2���r�����䂳���
//! 
//! @endcode
//! 
struct simple_lock_object
{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	typedef char lock_object_type;					// �X���b�h�T�|�[�g�͂��Ȃ��̂Ń_�~�[�ϐ��p��
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	typedef CRITICAL_SECTION lock_object_type;		// Windows�p�̃��b�N�I�u�W�F�N�g(CRITIFCALSECTION�𗘗p)
#else
	typedef pthread_mutex_t lock_object_type;		// pthread���C�u�����ł̃��b�N�I�u�W�F�N�g
#endif

	lock_object_type __lock__;	///< @brief ���b�N�p�I�u�W�F�N�g

public:
	/// @brief �R���X�g���N�^
	simple_lock_object( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		//InitializeCriticalSectionAndSpinCount( &__lock__, 4000 );	// �N���e�B�J���Z�N�V�����I�u�W�F�N�g��������
		InitializeCriticalSection( &__lock__ );	// �N���e�B�J���Z�N�V�����I�u�W�F�N�g��������
#else
		pthread_mutex_init( &__lock__, NULL );		// pthread�p��Mutex�I�u�W�F�N�g��������
#endif
	}

	/// @brief �f�X�g���N�^
	~simple_lock_object( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		DeleteCriticalSection( &__lock__ );		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g���폜
#else
		pthread_mutex_destroy( &__lock__ );		// pthread�p��Mutex�I�u�W�F�N�g��������
#endif
	}

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0

	// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
	bool lock( ){ return( true ); }
	bool unlock( ){ return( true ); }
	bool try_lock( ){ return( true ); }

#else

	/// @brief �r������p�I�u�W�F�N�g�����b�N����
	//! 
	//! @retval true  �c ���b�N�ɐ���
	//! @retval false �c ���b�N�Ɏ��s
	//! 
	bool lock( )
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		EnterCriticalSection( &__lock__ );		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g�����b�N
#else
		pthread_mutex_lock( &__lock__ );		// pthread�p��Mutex�I�u�W�F�N�g�����b�N
#endif

		return( true );
	}

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
	/// @brief �r������p�I�u�W�F�N�g�̃��b�N�����݂�
	//! 
	//! ���Ƀ��b�N����Ă����ꍇ�́C���b�N������ false ��Ԃ��D
	//! 
	//! @retval true  �c ���b�N�ɐ���
	//! @retval false �c ���b�N�Ɏ��s
	//! 
	bool try_lock( )
	{
		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g�̃��b�N���\���𒲂ׂ�
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( TryEnterCriticalSection( &__lock__ ) != FALSE );
#else
		return( pthread_mutex_trylock( &__lock__ ) == 0 );
#endif
	}
#endif

	/// @brief �r������p�I�u�W�F�N�g�̃��b�N����������
	bool unlock( )
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		LeaveCriticalSection( &__lock__ );		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g���A�����b�N
#else
		pthread_mutex_unlock( &__lock__ );		// pthread�p��Mutex�I�u�W�F�N�g���A�����b�N
#endif

		return( true );
	}

#endif
};

/// @}
//  �X���b�h�O���[�v�̏I���

template < class MUTEX >
class lock_object_table : public ::std::map< ::std::string, MUTEX >
{
private:
	typedef ::std::map< ::std::string, MUTEX > base;
	typedef MUTEX lock_object_type;

public:
	lock_object_table( )
	{
	}

	~lock_object_table( )
	{
		typename base::iterator ite = base::begin( );
		for( ; ite != base::end( ) ; ++ite )
		{
			destroy( ite->second );
		}
		base::clear( );
	}

protected:
	static void destroy( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		DeleteCriticalSection( &l );		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g���폜
#else
		pthread_mutex_destroy( &l );		// pthread�p��Mutex�I�u�W�F�N�g��������
#endif
	}
};

//! @addtogroup thread_group �X���b�h
//!
//!  @{


/// @brief �X���b�h�̔r��������T�|�[�g����N���X
//! 
//! ���b�N����I�u�W�F�N�g�𖼑O��p���ċ��L�����邱�Ƃ��\�ȃN���X�D
//! �����̃��b�N�I�u�W�F�N�g���쐬���邱�ƂŕʁX�̃X���b�h�֐����ł̔r�����䂪�\�ƂȂ�D
//! 
//! @attention ���b�N�I�u�W�F�N�g�̐������_�ł́C���b�N�͂���Ȃ����Ƃɒ���
//! 
//! - Windows �c �N���e�B�J���Z�N�V�����𗘗p
//! - Linux�n �c �~���[�e�b�N�X�𗘗p
//! 
//! @code �g�p��
//! lock_object l( "���b�N�I�u�W�F�N�g�̖��O" );  // <- ���O���w�肵�Ȃ��ƁC�f�t�H���g�̃��b�N�I�u�W�F�N�g���g�p�����
//! 
//! ... ���炩�̏��� ...
//! 
//! l.lock( );   // <- ����ȍ~�̏���1��r�����䂷��
//! 
//! ... ����1 ...
//! 
//! l.unlock( ); // <- �����܂ł̏���1���r�����䂳���
//! 
//! 
//! ... ���炩�̏��� ...
//! 
//! 
//! l.lock( );   // <- ����ȍ~�̏���2��r�����䂷��
//! 
//! ... ����2 ...
//! 
//! l.unlock( ); // <- �����܂ł̏���2���r�����䂳���
//! 
//! @endcode
//! 
struct lock_object
{
protected:
	::std::string lock_name_;

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	typedef char lock_object_type;					// �X���b�h�T�|�[�g�͂��Ȃ��̂Ń_�~�[�ϐ��p��
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	typedef CRITICAL_SECTION lock_object_type;		// Windows�p�̃��b�N�I�u�W�F�N�g(CRITIFCALSECTION�𗘗p)
#else
	typedef pthread_mutex_t lock_object_type;		// pthread���C�u�����ł̃��b�N�I�u�W�F�N�g
#endif

	typedef lock_object_table< lock_object_type > lock_table;

public:
	lock_object( ) : lock_name_( "mist default lock object!!" ){ }
	lock_object( const std::string &name ) : lock_name_( name ){ }

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0

	// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
	bool lock( ){ return( true ); }
	bool unlock( ){ return( true ); }

#else

	/// @brief �r������p�I�u�W�F�N�g�����b�N����
	//! 
	//! @retval true  �c ���b�N�ɐ���
	//! @retval false �c ���b�N�Ɏ��s
	//! 
	bool lock( )
	{
		static lock_object_type &__double_lock__ = double_lock_object( );

		// �e�[�u���̌����ŃX���b�h���Փ˂��Ȃ��悤�Ƀ��b�N����
		lock( __double_lock__ );

		lock_table &table = singleton< lock_table >::get_instance( );
		lock_table::iterator ite = table.find( lock_name_ );
		if( ite == table.end( ) )
		{
			// �܂����b�N�I�u�W�F�N�g�����������Ă��Ȃ��̂ŏ���������
			::std::pair< lock_table::iterator, bool > p = table.insert( lock_table::value_type( lock_name_, lock_object_type( ) ) );
			if( p.second )
			{
				lock_object_type &obj = p.first->second;
				initialize( obj );

				// �e�[�u�������p�̃��b�N���J������
				unlock( __double_lock__ );

				lock( obj );
			}
			else
			{
				// �e�[�u�������p�̃��b�N���J������
				unlock( __double_lock__ );

				// ���b�N�I�u�W�F�N�g���e�[�u���ɒǉ����邱�Ƃ��ł��܂���ł����E�E�E
				return( false );
			}
		}
		else
		{
			// �e�[�u�������p�̃��b�N���J������
			unlock( __double_lock__ );

			// ���łɓ����̃��b�N�I�u�W�F�N�g�����݂���̂ł���������b�N����
			lock( ite->second );
		}

		return( true );
	}

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
	/// @brief �r������p�I�u�W�F�N�g�̃��b�N�����݂�
	//! 
	//! ���Ƀ��b�N����Ă����ꍇ�́C���b�N������ false ��Ԃ��D
	//! 
	//! @retval true  �c ���b�N�ɐ���
	//! @retval false �c ���b�N�Ɏ��s
	//! 
	bool try_lock( )
	{
		static lock_object_type &__double_lock__ = double_lock_object( );

		// �e�[�u���̌����ŃX���b�h���Փ˂��Ȃ��悤�Ƀ��b�N����
		lock( __double_lock__ );

		lock_table &table = singleton< lock_table >::get_instance( );
		lock_table::iterator ite = table.find( lock_name_ );
		if( ite == table.end( ) )
		{
			// �܂����b�N�I�u�W�F�N�g�����������Ă��Ȃ��̂ŏ���������
			::std::pair< lock_table::iterator, bool > p = table.insert( lock_table::value_type( lock_name_, lock_object_type( ) ) );
			if( p.second )
			{
				lock_object_type &obj = p.first->second;
				initialize( obj );

				// �e�[�u�������p�̃��b�N���J������
				unlock( __double_lock__ );

				return( try_lock( obj ) );
			}
			else
			{
				// �e�[�u�������p�̃��b�N���J������
				unlock( __double_lock__ );

				// ���b�N�I�u�W�F�N�g���e�[�u���ɒǉ����邱�Ƃ��ł��܂���ł����E�E�E
				return( false );
			}
		}
		else
		{
			// �e�[�u�������p�̃��b�N���J������
			unlock( __double_lock__ );

			// ���łɓ����̃��b�N�I�u�W�F�N�g�����݂���̂ł���������b�N����
			return( try_lock( ite->second ) );
		}
	}
#endif

	/// @brief �r������p�I�u�W�F�N�g�̃��b�N����������
	bool unlock( )
	{
		lock_table &table = singleton< lock_table >::get_instance( );
		lock_table::iterator ite = table.find( lock_name_ );
		if( ite == table.end( ) )
		{
			// �w�肳�ꂽ���b�N�I�u�W�F�N�g��������Ȃ������̂ŉ������Ȃ�
			return( false );
		}
		else
		{
			unlock( ite->second );
		}
		return( true );
	}

#endif


protected:
	/// @brief �����Ŏg�p����_�u�����b�N�I�u�W�F�N�g���擾����
	static lock_object_type &double_lock_object( )
	{
		static bool isFirst = true;
		static lock_object_type __double_lock__;
		if( isFirst )
		{
			isFirst = false;
			initialize( __double_lock__ );
		}
		return( __double_lock__ );
	}

	/// @brief ���b�N�I�u�W�F�N�g�̏��������s��
	//! 
	//! @param[in,out] l �c ���b�N�I�u�W�F�N�g
	//! 
	static void initialize( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		InitializeCriticalSection( &l );	// �N���e�B�J���Z�N�V�����I�u�W�F�N�g��������
#else
		pthread_mutex_init( &l, NULL );		// pthread�p��Mutex�I�u�W�F�N�g��������
#endif
	}


	/// @brief ���b�N�I�u�W�F�N�g�����b�N����
	//! 
	//! @param[in,out] l �c ���b�N�I�u�W�F�N�g
	//! 
	static void lock( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		EnterCriticalSection( &l );		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g�����b�N
#else
		pthread_mutex_lock( &l );		// pthread�p��Mutex�I�u�W�F�N�g�����b�N
#endif
	}

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
	/// @brief �r������p�I�u�W�F�N�g�̃��b�N�����݂�
	//! 
	//! ���Ƀ��b�N����Ă����ꍇ�́C���b�N������ false ��Ԃ��D
	//! 
	//! @param[in,out] l �c ���b�N�I�u�W�F�N�g
	//! 
	//! @retval true  �c ���b�N�ɐ���
	//! @retval false �c ���b�N�Ɏ��s
	//! 
	static bool try_lock( lock_object_type &l )
	{
		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g�̃��b�N���\���𒲂ׂ�
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( TryEnterCriticalSection( &l ) != FALSE );
#else
		return( pthread_mutex_trylock( &l ) == 0 );
#endif
	}
#endif

	/// @brief ���b�N�I�u�W�F�N�g�����b�N����������
	//! 
	//! @param[in,out] l �c ���b�N�I�u�W�F�N�g
	//! 
	static void unlock( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		LeaveCriticalSection( &l );		// �N���e�B�J���Z�N�V�����I�u�W�F�N�g���A�����b�N
#else
		pthread_mutex_unlock( &l );		// pthread�p��Mutex�I�u�W�F�N�g���A�����b�N
#endif
	}
};




/// @brief �X���b�h�̔r��������ȕւɋL�q���邽�߂̃w���p�N���X
//! 
//! @code �g�p��
//! ... ���炩�̏��� ...
//! {
//!     // ���̃u���b�N����r�����䂷��
//!     lock l( "���b�N�I�u�W�F�N�g�̖��O" );  // <- ���O���w�肵�Ȃ��ƁC�f�t�H���g�̃��b�N�I�u�W�F�N�g���g�p�����
//! 
//!     ... �r�����䂵�������� ...
//! 
//!     // �����܂ł̏������r�����䂳���
//! }
//! @endcode
//! 
class lock
{
protected:
	lock_object lock_object_;	///< @brief ���b�N�I�u�W�F�N�g

public:
	/// @brief �f�t�H���g�̖��O�Ń��b�N�I�u�W�F�N�g�𐶐����C���b�N�I�u�W�F�N�g�����b�N����
	lock( ) : lock_object_( )
	{
		lock_object_.lock( );
	}


	/// @brief �w�肵�� name �̖��O�������b�N�I�u�W�F�N�g�𐶐����C���b�N�I�u�W�F�N�g�����b�N����
	lock( const std::string &name ) : lock_object_( name )
	{
		lock_object_.lock( );
	}


	/// @brief �J�����ɁC���b�N�I�u�W�F�N�g�̃��b�N����������
	~lock( )
	{
		lock_object_.unlock( );
	}
};


#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
/// @brief �X���b�h�ԂŃV�O�i�������L���邽�߂̃N���X
class signal
{
protected:
	HANDLE handle_;

public:
	/// @brief �R���X�g���N�^
	signal( ) : handle_( CreateEvent( NULL, FALSE, FALSE, NULL ) )
	{
	}

	/// @brief �f�X�g���N�^
	~signal( )
	{
		CloseHandle( handle_ );
	}

	/// @brief �V�O�i����ԂɂȂ�܂őҋ@����
	bool wait( unsigned long dwMilliseconds = INFINITE )
	{
		DWORD ret = WaitForSingleObject( handle_, dwMilliseconds );
		if( SUCCEEDED( ret ) )
		{
			ResetEvent( handle_ );
			return ( true );
		}
		else
		{
			return( false );
		}
	}

	/// @brief �V�O�i���𑗐M����
	void send( )
	{
		SetEvent( handle_ );
	}

	/// @brief �V�O�i�����N���A�ɂ���
	void reset( )
	{
		ResetEvent( handle_ );
	}
};
#else
class signal
{
protected:
	pthread_cond_t     cond_;
	pthread_mutex_t    mutex_;
	volatile bool      flag_;

public:
	/// @brief �R���X�g���N�^
	signal( ) : flag_( false )
	{
		pthread_mutex_init( &mutex_, NULL );
		pthread_cond_init( &cond_, NULL );
		pthread_mutex_lock ( &mutex_ );
	}

	/// @brief �f�X�g���N�^
	~signal( )
	{
		pthread_cond_destroy( &cond_ );
		pthread_mutex_destroy( &mutex_ );
	}

	/// @brief �V�O�i����ԂɂȂ�܂őҋ@����
	bool wait( unsigned long dwMilliseconds = INFINITE )
	{
		if( dwMilliseconds == INFINITE )
		{
			while( !flag_ )
			{
				if( pthread_cond_wait( &cond_, &mutex_ ) == 0 )
				{
					break;
				}
			}
			flag_ = false;
			return( true );
		}
		else
		{
			timeval now;
			gettimeofday( &now, NULL );

			timespec tm;
			tm.tv_sec  = now.tv_sec + static_cast< time_t >( dwMilliseconds / 1000 );
			tm.tv_nsec = now.tv_usec * 1000 + static_cast< long >( ( dwMilliseconds % 1000 ) * 1000000 );

			tm.tv_sec += tm.tv_nsec / 1000000000;
			tm.tv_nsec = tm.tv_nsec % 1000000000;

			while( !flag_ )
			{
				if( pthread_cond_timedwait( &cond_, &mutex_, &tm ) == 0 )
				{
					break;
				}
				else
				{
					return( false );
				}
			}

			flag_ = false;
			return( true );
		}
	}

	/// @brief �V�O�i���𑗐M����
	void send( )
	{
		flag_ = true;
		pthread_cond_broadcast( &cond_ );
	}

	/// @brief �V�O�i�����N���A�ɂ���ipthread�̏ꍇ�͉������Ȃ��j
	void reset( )
	{
		flag_ = false;
	}
};
#endif


/// @brief template �^�̃f�[�^���������Ƃ��ł���X���b�h�N���X
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @attention _MIST_THREAD_SUPPORT_ �� 1 �̏ꍇ�́C�X���b�h�T�|�[�g�͍s��Ȃ����߁C�V���O���X���b�h�p�ɓ����������ύX�����
//! 
//! @param thread_parameter �c �����̐���
//! 
template < class thread_parameter = thread_dmy_class >
class thread : public thread_object
{
public:
	typedef unsigned long thread_exit_type;		///< @brief �X���b�h�I�����̖߂�l�̌^

private:

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�v�ȕϐ��͖���
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	HANDLE       thread_handle_;		// Windows�p�̃X���b�h�����ʂ���n���h��
	unsigned int thread_id_;			// Windows�p�̃X���b�h�����ʂ���ID
#else
	pthread_t thread_id_;				// pthread���C�u�����ŃX���b�h�����ʂ���ID
	signal    finish_;					// �X���b�h�̏I���҂����s���V�O�i��
#endif

	simple_lock_object exit_;				// �I���҂��V�O�i��
	thread_exit_type   thread_exit_code_;	// �X���b�h�̖߂�l

public:

	/// @brief �X���b�h���I���������ɕԂ����߂�l���擾����
	thread_exit_type exit_code( ) const { return( thread_exit_code_ ); }


	/// @brief ���̃X���b�h�I�u�W�F�N�g�Ɠ������̂��쐬����
	//! 
	//! @attention �P�Ȃ�R�s�[�ł���C�ʓr�X���b�h�����������킯�ł͂Ȃ�
	//! 
	//! @param[in] t �c �R�s�[���̃X���b�h�I�u�W�F�N�g
	//!
	//! @return �������g
	//! 
	const thread &operator =( const thread &t )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		thread_handle_ = t.thread_handle_;
		thread_id_ = t.thread_id_;
#else
		thread_id_ = t.thread_id_;
#endif
		thread_exit_code_ = t.thread_exit_code_;
		return( *this );
	}


	/// @brief 2�̃X���b�h�I�u�W�F�N�g���������̂��ǂ����𔻒肷��
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] t �c ��r�Ώۂ̃X���b�h�I�u�W�F�N�g
	//!
	//! @retval true  �c �����X���b�h
	//! @retval false �c �ʂ̃X���b�h
	//! 
	bool operator ==( const thread &t ) const
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ�
		return( this == &t );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( thread_id_ == t.thread_id_ );
#else
		return( pthread_equal( thread_id_, t.thread_id_ ) != 0 );
#endif
	}

	bool operator !=( const thread &t ) const
	{
		return( !( *this == t ) );
	}


#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// �X���b�h�T�|�[�g�͂��Ȃ�
	thread( const thread &t ) : thread_exit_code_( t.thread_exit_code_ ){ }
	thread( ) : thread_exit_code_( 0 ){ }
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	thread( const thread &t ) : thread_handle_( t.thread_handle_ ), thread_id_( t.thread_id_ ), thread_exit_code_( t.thread_exit_code_ ){ }
	thread( ) : thread_handle_( NULL ), thread_id_( ( unsigned int )-1 ), thread_exit_code_( 0 ){ }
#else
	thread( const thread &t ) : thread_id_( t.thread_id ), thread_exit_code_( t.thread_exit_code ){ }
	thread( ) : thread_id_( ( pthread_t ) ( -1 ) ), thread_exit_code_( 0 ){ }
#endif

	virtual ~thread( )
	{
		this->close( );
	}


	/// @brief �X���b�h�𐶐�����
	//! 
	//! @retval true  �c �X���b�h�̍쐬�ɐ���
	//! @retval false �c �X���b�h�̍쐬�Ɏ��s
	//! 
	virtual bool create( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂Œ��ڊ֐����Ăяo��
		bool ret = true;
		thread_exit_code_ = thread_function( );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		if( thread_handle_ != NULL ) return( false );
		thread_handle_ = ( HANDLE )_beginthreadex( NULL, 0, map_thread_function, ( void * )this, 0, &thread_id_ );
		bool ret = thread_handle_ != NULL ? true : false;
#else
		if( thread_id_ != ( pthread_t ) ( -1 ) ) return( false );
		bool ret = pthread_create( &( thread_id_ ), NULL, map_thread_function, ( void * )this ) == 0 ? true : false;
#endif

		return ( ret );
	}


	/// @brief �X���b�h�����Ŋ֐������s����
	//! 
	//! @retval true  �c �X���b�h�̍쐬�ɐ���
	//! @retval false �c �X���b�h�̍쐬�Ɏ��s
	//! 
	virtual bool create_without_thread( )
	{
		thread_exit_code_ = thread_function( );
		return ( true );
	}


	/// @brief �X���b�h���I�����邩�C�^�C���A�E�g�ɂȂ�܂őҋ@����
	//! 
	//! �^�C���A�E�g�� INFINITE �ɐݒ肷�邱�ƂŁC�X���b�h���I������܂ő҂�������
	//! 
	//! @param[in] dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
	//! 
	//! @retval true  �c �X���b�h���^�C���A�E�g�O�ɏI������
	//! @retval false �c �^�C���A�E�g�������������C���̑��̃G���[������
	//! 
	virtual bool wait( unsigned long dwMilliseconds = INFINITE )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂ŉ������Ȃ�
		// �X���b�h������Ȃ����߁CdwMilliseconds �͏�� INFINITE �����ƂȂ�
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		DWORD ret = WaitForSingleObject( thread_handle_, dwMilliseconds );
		return ( ret == WAIT_OBJECT_0 );
#else
		if( dwMilliseconds == INFINITE )
		{
			return ( pthread_join( thread_id_, NULL ) == 0 );
		}
		else
		{
			if( finish_.wait( dwMilliseconds ) )
			{
				return ( pthread_join( thread_id_, NULL ) == 0 );
			}
			else
			{
				return( false );
			}
		}
#endif
	}


	/// @brief �X���b�h���g�p���Ă������\�[�X���J������
	//! 
	//! �X���b�h���g�p�������\�[�X���J�����邽�߂ɁC�K��1�̃X���b�h�ɑ΂�1�x�����Ă΂��K�v������
	//! 
	//! @retval true  �c ����ɃX���b�h���I��
	//! @retval false �c �X���b�h�̏I���Ɏ��s
	//! 
	virtual bool close( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂ŏ�� true ��Ԃ�
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		if( thread_handle_ != NULL )
		{
			// �X���b�h�̊��S�I����ҋ@����
#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
			while( !exit_.try_lock( ) ){}
			exit_.unlock( );
#endif

			BOOL ret = CloseHandle( thread_handle_ );
			thread_handle_ = NULL;
			return ( ret != 0 );
		}
		else
		{
			return( true );
		}
#else
		if( thread_id_ != ( pthread_t ) ( -1 ) )
		{
			// �X���b�h�̊��S�I����ҋ@����
			while( !exit_.try_lock( ) ){}
			exit_.unlock( );

			pthread_join( thread_id_, NULL );

			thread_id_ = ( pthread_t ) ( -1 );
			return( true );
		}
		else
		{
			return( true );
		}
#endif
	}


protected:
	/// @brief �p��������ŕK�����������X���b�h�֐�
	//! 
	//! �{�N���X���p�����C�X���b�h�̋@�\����������ꍇ�ɕK���������Ȃ��Ă͂Ȃ�Ȃ��֐��D
	//! �X���b�h�̐����ƂƂ��ɖ{�֐����Ă΂��D
	//! 
	//! @return �X���b�h�I�����̖߂�l
	//! 
	virtual thread_exit_type thread_function( ) = 0;


#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// �X���b�h�T�|�[�g�͂��Ȃ��̂ŉ������Ȃ�
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	static unsigned int __stdcall map_thread_function( void *p )
	{
		thread *obj = static_cast< thread * >( p );
		obj->exit_.lock( );
		obj->thread_exit_code_ = obj->thread_function( );
		obj->exit_.unlock( );
		return( 0 );
	}
#else
	static void *map_thread_function( void *p )
	{
		thread *obj = static_cast< thread * >( p );
		obj->exit_.lock( );
		obj->thread_exit_code_ = obj->thread_function( );
		obj->exit_.unlock( );
		obj->finish_.send( );
		return ( NULL );
	}
#endif
};


/// @brief �X���b�h�����E�I���ҋ@�E�X���b�h�̔j���܂ł���A�̗���Ƃ��čs��
//! 
//! @param[in] threads        �c �X���b�h�I�u�W�F�N�g
//! @param[in] num_threads    �c �X���b�h��
//! @param[in] dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �����̃X���b�h�̎��s�ɐ���
//! @retval false �c �����̃X���b�h�̎��s�Ɏ��s
//! 
template < class Thread >
inline bool do_threads_( Thread *threads, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	size_t i = 0;

	// �X���b�h�̐���
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].create( ) )
		{
			ret = false;
		}
	}
	if( num_threads > 0 )
	{
		// �擪�̃p�����[�^�̂ݎ��s�X���b�h�ōs��
		threads[ 0 ].create_without_thread( );
	}

	// �X���b�h�̏I���҂�
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].wait( dwMilliseconds ) )
		{
			ret = false;
		}
	}

	// ���\�[�X�̊J��
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].close( ) )
		{
			ret = false;
		}
	}

	return( ret );
}

/// @brief �X���b�h�����E�I���ҋ@�E�X���b�h�̔j���܂ł���A�̗���Ƃ��čs��
//! 
//! @param[in] threads        �c �X���b�h�I�u�W�F�N�g
//! @param[in] num_threads    �c �X���b�h��
//! @param[in] dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �����̃X���b�h�̎��s�ɐ���
//! @retval false �c �����̃X���b�h�̎��s�Ɏ��s
//! 
template < class Thread >
inline bool do_threads_( Thread **threads, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	size_t i = 0;

	// �X���b�h�̐���
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ]->create( ) )
		{
			ret = false;
		}
	}
	if( num_threads > 0 )
	{
		// �擪�̃p�����[�^�̂ݎ��s�X���b�h�ōs��
		threads[ 0 ]->create_without_thread( );
	}

	// �X���b�h�̏I���҂�
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ]->wait( dwMilliseconds ) )
		{
			ret = false;
		}
	}

	// ���\�[�X�̊J��
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ]->close( ) )
		{
			ret = false;
		}
	}

	return( ret );
}



/// @brief �X���b�h�����E�I���ҋ@�E�X���b�h�̔j���܂ł���A�̗���Ƃ��čs��
//! 
//! @param[in] threads        �c �X���b�h�I�u�W�F�N�g
//! @param[in] num_threads    �c �X���b�h��
//! @param[in] dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �����̃X���b�h�̎��s�ɐ���
//! @retval false �c �����̃X���b�h�̎��s�Ɏ��s
//! 
template < class Thread >
inline bool do_threads( Thread *threads, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	size_t i = 0;

	// �X���b�h�̐���
	for( i = 0 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].create( ) )
		{
			ret = false;
		}
	}

	// �X���b�h�̏I���҂�
	for( i = 0 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].wait( dwMilliseconds ) )
		{
			ret = false;
		}
	}

	// ���\�[�X�̊J��
	for( i = 0 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].close( ) )
		{
			ret = false;
		}
	}

	return( ret );
}


/// @}
//  �X���b�h�O���[�v�̏I���

// �X���b�h�𕁒ʂ̊֐��`���̌Ăяo���Ŋȕւɗ��p���邽�߂̊֐��Q
namespace __thread_controller__
{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	inline unsigned long _timeGetTime_( )
	{
		return( timeGetTime( ) );
	}
#else
	inline unsigned long _timeGetTime_( )
	{
		timeval dmy;
		gettimeofday( &dmy, NULL );
		return( dmy.tv_sec );
	}
#endif

	template < class Param, class Functor >
	class thread_object_functor : public thread< thread_object_functor< Param, Functor > >
	{
	public:
		typedef thread< thread_object_functor< Param, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;

	private:
		Param   &param_;
		Functor func_;

	public:
		thread_object_functor( Param &p, Functor f ) : param_( p ), func_( f ){ }
		virtual ~thread_object_functor( ){ }

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			func_( param_ );
			return( 0 );
		}
	};

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
	struct __thread_pool_functor__
	{
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;

		virtual void run( size_type id, size_type nthreads ) = 0;

		virtual ~__thread_pool_functor__( ){}
	};

	template < class Param, class Functor >
	class thread_pool_functor_base : public __thread_pool_functor__
	{
	private:
		Param   param_;
		Functor func_;

	public:
		thread_pool_functor_base( Param p, Functor f ) : param_( p ), func_( f ){ }
		virtual ~thread_pool_functor_base( ){}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual void run( size_type /* id */, size_type /* nthreads */ )
		{
			func_( param_ );
		}
	};

	template < class Param1, class Param2, class Functor >
	class thread_pool_functor_base2 : public __thread_pool_functor__
	{
	private:
		Param1  param1_;
		Param2  param2_;
		Functor func_;

	public:
		thread_pool_functor_base2( Param1 p1, Param2 p2, Functor f ) : param1_( p1 ), param2_( p2 ), func_( f ){ }
		virtual ~thread_pool_functor_base2( ){ }

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual void run( size_type /* id */, size_type /* nthreads */ )
		{
			func_( param1_, param2_ );
		}
	};

	template < class Param1, class Param2, class Param3, class Functor >
	class thread_pool_functor_base3 : public __thread_pool_functor__
	{
	private:
		Param1  param1_;
		Param2  param2_;
		Param3  param3_;
		Functor func_;

	public:
		thread_pool_functor_base3( Param1 p1, Param2 p2, Param3 p3, Functor f ) : param1_( p1 ), param2_( p2 ), param3_( p3 ), func_( f ){ }
		virtual ~thread_pool_functor_base3( ){ }

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual void run( size_type /* id */, size_type /* nthreads */ )
		{
			func_( param1_, param2_, param3_ );
		}
	};

	template < class Param1, class Param2, class Param3, class Param4, class Functor >
	class thread_pool_functor_base4 : public __thread_pool_functor__
	{
	private:
		Param1  param1_;
		Param2  param2_;
		Param3  param3_;
		Param4  param4_;
		Functor func_;

	public:
		thread_pool_functor_base4( Param1 p1, Param2 p2, Param3 p3, Param4 p4, Functor f ) : param1_( p1 ), param2_( p2 ), param3_( p3 ), param4_( p4 ), func_( f ){ }
		virtual ~thread_pool_functor_base4( ){ }

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual void run( size_type /* id */, size_type /* nthreads */ )
		{
			func_( param1_, param2_, param3_, param4_ );
		}
	};


	template < class Functor >
	class thread_pool_void_functor_base : public __thread_pool_functor__
	{
	private:
		Functor func_;

	public:
		thread_pool_void_functor_base( Functor f ) : func_( f ){ }
		virtual ~thread_pool_void_functor_base( ){ }

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual void run( size_type /* id */, size_type /* nthreads */ )
		{
			func_( );
		}
	};

	class thread_pool_functor : public thread< thread_pool_functor >
	{
	public:
		typedef thread< thread_pool_functor > base;
		typedef base::thread_exit_type thread_exit_type;
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;

	private:
		std::list< __thread_pool_functor__ * > &functors_;
		simple_lock_object                     &lock_;
		simple_lock_object                     suspend_lock_;
		simple_lock_object                     wait_lock_;
		bool                                   is_running_;
		bool                                   is_end_;
		size_type                              id_;
		size_type                              nthreads_;
		signal                                 signal_;

	public:
		thread_pool_functor( std::list< __thread_pool_functor__ * > &functors, simple_lock_object &l, size_type id, size_type nthreads )
			: functors_( functors ), lock_( l ), is_running_( false ), is_end_( false ), id_( id ), nthreads_( nthreads )
		{
			// ���b�N������ԂŃX�^�[�g����
			suspend_lock_.lock( );
		}

		virtual ~thread_pool_functor( ){ }

		size_type num_jobs( ) const { return( functors_.size( ) ); }
		bool is_end( ) const { return( is_end_ ); }

		bool is_suspended( )
		{
			if( wait_lock_.try_lock( ) )
			{
				wait_lock_.unlock( );
				return( true );
			}
			else
			{
				return( false );
			}
		}

		virtual bool create( )
		{
			if( base::create( ) )
			{
				// �X���b�h�̊J�n�܂őҋ@����
				while( true )
				{
					lock_.lock( );
					if( is_running_ )
					{
						lock_.unlock( );
						break;
					}
					else
					{
						lock_.unlock( );
					}
				}

				return( true );
			}
			else
			{
				return( false );
			}
		}

		virtual bool close( )
		{
			lock_.lock( );
			// �I���t���O�𗧂ĂăX���b�h���ċN������
			is_end_ = true;
			this->resume( );
			lock_.unlock( );

			// �X���b�h������I������܂ő҂�
			while( !wait_lock_.try_lock( ) ){}
			wait_lock_.unlock( );

			return( base::close( ) );
		}

		void resume( )
		{
			if( is_suspended( ) )
			{
				suspend_lock_.unlock( );
				while( wait_lock_.try_lock( ) )
				{
					wait_lock_.unlock( );
				}
				suspend_lock_.lock( );
			}
		}

		bool wait( unsigned long dwMilliseconds = INFINITE )
		{
			lock_.lock( );
			if( !wait_lock_.try_lock( ) )
			{
				lock_.unlock( );
				return( signal_.wait( dwMilliseconds ) );
			}
			else
			{
				wait_lock_.unlock( );
				lock_.unlock( );
				return( true );
			}
		}

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			wait_lock_.lock( );

			lock_.lock( );
			is_running_ = true;
			lock_.unlock( );

			while( true )
			{
				lock_.lock( );
				if( is_end_ )
				{
					lock_.unlock( );
					break;
				}
				else if( functors_.empty( ) )
				{
					// �����҂�������
					wait_lock_.unlock( );
					lock_.unlock( );
					signal_.send( );
					suspend_lock_.lock( );
					signal_.reset( );
					wait_lock_.lock( );
					suspend_lock_.unlock( );
				}
				else
				{
					// �L���[�̐擪����f�[�^�����o��
					__thread_pool_functor__ *f = functors_.front( );
					functors_.pop_front( );
					lock_.unlock( );

					if( f != NULL )
					{
						f->run( id_, nthreads_ );
						delete f;
					}
				}
			}

			wait_lock_.unlock( );
			signal_.send( );

			return( 0 );
		}
	};
#endif
}


//! @addtogroup thread_group �X���b�h
//!
//!  @{

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
/// @brief �X���b�h�v�[���𗘗p���ăX���b�h�̍ė��p���\�ɂ���N���X
//! 
//! �X���b�h�v�[���𗘗p���ď��������s�����ꍇ�C�����p�ɃX���b�h�v�[�����̃X���b�h�����蓖�Ă܂��D
//! �������I�������ꍇ�́C�X���b�h�̓X���b�h�v�[���ɉ������C�ʂ̏����֊��蓖�Ă��܂��D
//! �X���b�h���ė��p���邱�Ƃɂ��C�X���b�h�̐����j���ɕK�v�ȃI�[�o�[�w�b�h���팸���܂��D
//! 
//! @attention ����N�����̓X���b�h�����ɔ����I�[�o�[�w�b�h������܂�
//! 
//! @code �g�p��
//! struct parameter{ ... ���炩�̃p�����[�^ ... };   // �X���b�h�֐��ɓn���p�����[�^�i���ɍ\���̂ł���K�v�͖����j
//! 
//! void thread_function( const parameter &p );       // �X���b�h�֐��i�����ŉ��炩�̏���������j
//! 
//! // ����ȍ~�ŃX���b�h���쐬����
//! parameter param[ �������s���� ];
//! 
//! for( i = 0 ; i < �������s���� ; i++ )
//! {
//!     �p�����[�^�ɉ��炩�̒l��ݒ肷��
//! }
//! 
//! // �X���b�h�v�[�����̃X���b�h�����w�肵�ď�����
//! mist::thread_pool pool( 2 );
//! 
//! // �֐����p�����[�^��ς��ĕ����񏈗�����
//! pool.executes( thread_function, param, �������s���� );
//! 
//! // �X���b�h�v�[�����̃X���b�h�����s���Ă��鏈�����S�ďI������܂őҋ@
//! pool.wait( );
//! @endcode
//! 
class thread_pool
{
public:
	typedef size_t    size_type;		///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	typedef __thread_controller__::thread_pool_functor thread_pool_functor;
	typedef __thread_controller__::__thread_pool_functor__ __thread_pool_functor__;

	std::vector< thread_pool_functor * >   threads_;		///< @brief �X���b�h�v�[�����̃X���b�h���Ǘ�����ϐ�
	std::list< __thread_pool_functor__ * > functors_;		///< @brief �X���b�h�v�[�����̏����҂��̃W���u���Ǘ����郊�X�g
	simple_lock_object                     lock_;			///< @brief �X���b�h�v�[�����̃X���b�h�Ԃœ�������邽�߂̃��b�N�I�u�W�F�N�g
	bool                                   initialized_;	///< @brief �X���b�h�v�[���̏��������I�����Ă��邩�ǂ���

public:
	/// @brief �X���b�h�v�[�����̃I�u�W�F�N�g���쐬���邪�C�X���b�h�v�[�����̃X���b�h��0�̏�Ԃŏ���������iinitialize�֐����Ăяo���K�v����j
	thread_pool( ) : initialized_( false )
	{
	}

	/// @brief �X���b�h�v�[�����̃X���b�h�����w�肵�C�����ɃX���b�h�v�[�����g�����Ԃŏ���������
	thread_pool( size_type number_of_max_threads ) : initialized_( false )
	{
		initialize( number_of_max_threads );
	}

	/// @brief �X���b�h�v�[���Ŏg�p���Ă���S�Ẵ��\�[�X���J������
	virtual ~thread_pool( )
	{
		uninitialize( );
	}

	/// @brief �X���b�h�v�[�����Ŏg�p����X���b�h�̐����w�肵�ăX���b�h�v�[��������������
	//! 
	//! ���ɃX���b�h�v�[�������������Ă���ꍇ�́C���݂̃X���b�h���S�ďI������܂őҋ@����
	//! 
	//! @param[in] number_of_max_threads �c �X���b�h�v�[���ɂ��߂Ă����X���b�h�̐�
	//! 
	bool initialize( size_type number_of_max_threads )
	{
		// ���ݎ��s���̃X���b�h��S�ďI��������
		uninitialize( );

		if( number_of_max_threads == 0 )
		{
			// 1�ȏ���w�肷��K�v����
			return( false );
		}

		threads_.resize( number_of_max_threads );

		// �X���b�h�����s���ăA�C�h����Ԃɂ���
		for( size_type i = 0 ; i < threads_.size( ) ; i++ )
		{
			threads_[ i ] = new thread_pool_functor( functors_, lock_, i, threads_.size( ) );
			threads_[ i ]->create( );
			//threads_[ i ]->wait( );
		}

		initialized_ = true;

		return( true );
	}

	/// @brief �X���b�h�v�[���Ŏg�p���Ă��郊�\�[�X��S�ĊJ������
	//! 
	//! ���݂̃X���b�h���S�ďI������܂őҋ@���C�X���b�h�v�[���Ŏg�p���Ă��郊�\�[�X��S�ĊJ������
	//! 
	bool uninitialize( )
	{
		if( initialized_ )
		{
			for( size_type i = 0 ; i < threads_.size( ) ; i++ )
			{
				// �X���b�h�̃n���h�������
				threads_[ i ]->close( );

				// �g�p���Ă����������̈���J������
				delete threads_[ i ];
			}

			threads_.clear( );

			// �L���[�Ɏc���Ă���f�[�^���폜����
			lock_.lock( );
			while( !functors_.empty( ) )
			{
				__thread_pool_functor__ *f = functors_.front( );
				functors_.pop_front( );
				delete f;
			}
			lock_.unlock( );
		}

		initialized_ = false;

		return( true );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] p �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     f �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param >
	bool execute( Functor f, Param p )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base< Param, Functor >( p, f ) ) );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] p1 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p2 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     f  �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2 >
	bool execute( Functor f, Param1 p1, Param2 p2 )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base2< Param1, Param2, Functor >( p1, p2, f ) ) );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] p1 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p2 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p3 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     f  �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2, class Param3 >
	bool execute( Functor f, Param1 p1, Param2 p2, Param3 p3 )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base3< Param1, Param2, Param3, Functor >( p1, p2, p3, f ) ) );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] p1 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p2 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p3 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p4 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     f  �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2, class Param3, class Param4 >
	bool execute( Functor f, Param1 p1, Param2 p2, Param3 p3, Param4 p4 )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base4< Param1, Param2, Param3, Param4, Functor >( p1, p2, p3, p4, f ) ) );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! 
	//! @param[in] f �c ���s�����X���b�h�֐�
	//! 
	template < class Functor >
	bool execute( Functor f )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_void_functor_base< Functor >( f ) ) );
	}

	/// @brief �֐��ƃp�����[�^�𕡐��w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] param       �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     num_threads �c �X���b�h��
	//! @param[in]     f           �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param >
	bool executes( Functor f, Param *param, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// �L���[�ɒǉ�����
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base< Param, Functor >( param[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief �֐��ƃp�����[�^�𕡐��w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] param1      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] param2      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     num_threads �c �X���b�h��
	//! @param[in]     f           �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2 >
	bool executes( Functor f, Param1 *param1, Param2 *param2, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// �L���[�ɒǉ�����
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base2< Param1, Param2, Functor >( param1[ i ], param2[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief �֐��ƃp�����[�^�𕡐��w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] param1      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] param2      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] param3      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     num_threads �c �X���b�h��
	//! @param[in]     f           �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2, class Param3 >
	bool executes( Functor f, Param1 *param1, Param2 *param2, Param3 *param3, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// �L���[�ɒǉ�����
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base3< Param1, Param2, Param3, Functor >( param1[ i ], param2[ i ], param3[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief �֐��ƃp�����[�^�𕡐��w�肵�ăX���b�h�����s����
	//! 
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] param1      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] param2      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] param3      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] param4      �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     num_threads �c �X���b�h��
	//! @param[in]     f           �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2, class Param3, class Param4 >
	bool executes( Functor f, Param1 *param1, Param2 *param2, Param3 *param3, Param4 *param4, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// �L���[�ɒǉ�����
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base4< Param1, Param2, Param3, Param4, Functor >( param1[ i ], param2[ i ], param3[ i ], param4[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief �S�ẴX���b�h���I�����邩�C�^�C���A�E�g�ɂȂ�܂őҋ@����
	//! 
	//! �^�C���A�E�g�� INFINITE �ɐݒ肷�邱�ƂŁC�X���b�h���I������܂ő҂�������D
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! 
	//! @param[in] dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
	//! 
	//! @retval true  �c �X���b�h���^�C���A�E�g�O�ɏI������
	//! @retval false �c �^�C���A�E�g�������������C���̑��̃G���[������
	//! 
	virtual bool wait( unsigned long dwMilliseconds = INFINITE )
	{
		if( !initialized_ )
		{
			return( false );
		}
		else if( threads_.empty( ) )
		{
			return( false );
		}

		unsigned long st = __thread_controller__::_timeGetTime_( );
		size_type i = 0;
		for( ; i < threads_.size( ) ; i++ )
		{
			if( !threads_[ i ]->wait( dwMilliseconds ) )
			{
				break;
			}
			else if( dwMilliseconds != INFINITE )
			{
				unsigned long ct = __thread_controller__::_timeGetTime_( );
				dwMilliseconds -= ct - st;
				st = ct;
			}
		}

		return( i < threads_.size( ) );
	}

protected:
	/// @brief �֐����X���b�h�v�[�����̃X���b�h�Ɋ��蓖�ĂĎ��s����
	//! 
	//! @param[in] func �c ���s�����X���b�h�֐�
	//! 
	bool exec( __thread_controller__::__thread_pool_functor__ *func )
	{
		// �L���[�ɒǉ�����
		{
			// �r�����䂷��
			lock_.lock( );
			functors_.push_back( func );
			lock_.unlock( );
		}

		resume_thread_from_queue( 1 );

		return( true );
	}

	/// @brief �X���b�h�v�[������K�v�Ȑ������X���b�h���ĊJ����
	//! 
	//! @param[in] num_threads �c ���W���[������X���b�h��
	//! 
	void resume_thread_from_queue( size_type num_threads )
	{
		size_type count = 0;
		for( size_type i = 0 ; i < threads_.size( ) ; i++ )
		{
			thread_pool_functor &t = *threads_[ i ];

			if( t.is_suspended( ) )
			{
				// �A�C�h����Ԃ̃X���b�h������΍ĊJ����
				t.resume( );

				count++;
			}

			if( count >= num_threads )
			{
				break;
			}
		}
	}
};



/// @brief �o�b�N�O���E���h�Ŏ��s���郏�[�J�[�X���b�h����������N���X
//! 
//! ���[�J�[�X���b�h�ɂ͕����̃W���u��ݒ肷�邱�Ƃ��ł��C�e�W���u���������s���܂��D
//! �S�ẴW���u���I������ƁC���[�J�[�X���b�h�͑ҋ@��Ԃɓ���C���̃W���u��҂��܂��D
//! 
//! @attention ����N�����̓X���b�h�����ɔ����I�[�o�[�w�b�h������܂�
//! 
//! @code �g�p��
//! struct parameter{ ... ���炩�̃p�����[�^ ... };   // �X���b�h�֐��ɓn���p�����[�^�i���ɍ\���̂ł���K�v�͖����j
//! 
//! void thread_function( const parameter &p );       // �X���b�h�֐��i�����ŉ��炩�̏���������j
//! 
//! // ����ȍ~�ŃX���b�h���쐬����
//! parameter param;
//! 
//! // ���[�J�[�X���b�h��������
//! mist::worker_thread work;
//! 
//! // �֐������s����
//! work.execute( thread_function, param );
//! 
//! // ���[�J�[�X���b�h�����s���Ă��鏈�����S�ďI������܂őҋ@
//! work.wait( );
//! @endcode
//! 
class worker_thread
{
public:
	typedef size_t    size_type;		///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	__thread_controller__::thread_pool_functor                    *thread_;		///< @brief ���[�J�[�X���b�h���Ǘ����郁�C���X���b�h
	std::list< __thread_controller__::__thread_pool_functor__ * >  functors_;	///< @brief ���[�J�[�X���b�h���̏����҂��̃W���u���Ǘ����郊�X�g
	simple_lock_object                                             lock_;		///< @brief ���[�J�[�X���b�h���̃X���b�h�Ԃœ�������邽�߂̃��b�N�I�u�W�F�N�g

public:
	/// @brief ���[�J�[�X���b�h�p�̃X���b�h���쐬������Ԃőҋ@����
	worker_thread( ) : thread_( NULL )
	{
		thread_ = new __thread_controller__::thread_pool_functor( functors_, lock_, 0, 1 );
		thread_->create( );
		//thread_->wait( );
	}

	/// @brief ���[�J�[�X���b�h�Ŏg�p���Ă���S�Ẵ��\�[�X���J������
	virtual ~worker_thread( )
	{
		close( );
	}

	/// @brief ���[�J�[�X���b�h���T�X�y���h���Ă��邩�ǂ���
	bool is_suspended( )
	{
		if( thread_ == NULL )
		{
			return( true );
		}

		return( thread_->is_suspended( ) );
	}

	/// @brief ���[�J�[�X���b�h�Ŏg�p���Ă��郊�\�[�X��S�ĊJ������
	//! 
	//! �����Ƀ��[�J�[�X���b�h���I�����C���[�J�[�X���b�h�Ŏg�p���Ă��郊�\�[�X��S�ĊJ������
	//! 
	bool close( )
	{
		if( thread_ == NULL )
		{
			return( false );
		}

		// �X���b�h�̃n���h�������
		thread_->close( );

		// �g�p���Ă����������̈���J������
		delete thread_;
		thread_ = NULL;

		// �L���[�Ɏc���Ă���f�[�^���폜����
		lock_.lock( );
		while( !functors_.empty( ) )
		{
			__thread_controller__::__thread_pool_functor__ *f = functors_.front( );
			functors_.pop_front( );
			delete f;
		}
		lock_.unlock( );

		return( true );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! ���[�J�[�X���b�h���I��������Ԃł� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] p �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     f �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param >
	bool execute(  Functor f, Param p )
	{
		if( thread_ == NULL )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base< Param, Functor >( p, f ) ) );
	}


	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! ���[�J�[�X���b�h���I��������Ԃł� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in] p1 �c �X���b�h�̊֐��ɓn���p�����[�^1
	//! @param[in] p2 �c �X���b�h�̊֐��ɓn���p�����[�^2
	//! @param[in] f  �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2 >
	bool execute( Functor f, Param1 p1, Param2 p2 )
	{
		if( thread_ == NULL )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base2< Param1, Param2, Functor >( p1, p2, f ) ) );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! ���[�J�[�X���b�h���I��������Ԃł� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] p1 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p2 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p3 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     f  �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2, class Param3 >
	bool execute( Functor f, Param1 p1, Param2 p2, Param3 p3 )
	{
		if( thread_ == NULL )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base3< Param1, Param2, Param3, Functor >( p1, p2, p3, f ) ) );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! ���[�J�[�X���b�h���I��������Ԃł� false ��Ԃ��D
	//! �X���b�h�����s����֐��̈����̓R�s�[���ēn�����D
	//! 
	//! @param[in,out] p1 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p2 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p3 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in,out] p4 �c �X���b�h�̊֐��ɓn���p�����[�^
	//! @param[in]     f  �c ���s�����X���b�h�֐�
	//! 
	template < class Functor, class Param1, class Param2, class Param3, class Param4 >
	bool execute( Functor f, Param1 p1, Param2 p2, Param3 p3, Param4 p4 )
	{
		if( thread_ == NULL )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_functor_base4< Param1, Param2, Param3, Param4, Functor >( p1, p2, p3, p4, f ) ) );
	}

	/// @brief �֐��ƃp�����[�^���w�肵�ăX���b�h�����s����
	//! 
	//! ���[�J�[�X���b�h���I��������Ԃł� false ��Ԃ��D
	//! 
	//! @param[in]     f     �c ���s�����X���b�h�֐�
	//! 
	template < class Functor >
	bool execute( Functor f )
	{
		if( thread_ == NULL )
		{
			return( false );
		}

		return( exec( new __thread_controller__::thread_pool_void_functor_base< Functor >( f ) ) );
	}

	/// @brief �S�Ă̏������I�����邩�C�^�C���A�E�g�ɂȂ�܂őҋ@����
	//! 
	//! �^�C���A�E�g�� INFINITE �ɐݒ肷�邱�ƂŁC�X���b�h���I������܂ő҂�������D
	//! �X���b�h�v�[���̏��������I�����Ă��Ȃ��ꍇ�� false ��Ԃ��D
	//! 
	//! @param[in] dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
	//! 
	//! @retval true  �c �X���b�h���^�C���A�E�g�O�ɏI������
	//! @retval false �c �^�C���A�E�g�������������C���̑��̃G���[������
	//! 
	virtual bool wait( unsigned long dwMilliseconds = INFINITE )
	{
		if( thread_ == NULL )
		{
			return( true );
		}
		else
		{
			return( thread_->wait( dwMilliseconds ) );
		}
	}

protected:
	/// @brief �֐����X���b�h�v�[�����̃X���b�h�Ɋ��蓖�ĂĎ��s����
	//! 
	//! @param[in] func �c ���s�����X���b�h�֐�
	//! 
	bool exec( __thread_controller__::__thread_pool_functor__ *func )
	{
		// �L���[�ɒǉ�����
		{
			// �r�����䂷��
			lock_.lock( );
			functors_.push_back( func );
			lock_.unlock( );
		}

		// �X���b�h���ĊJ����
		thread_->resume( );


		return( true );
	}
};
#endif

/// @brief ���������X���b�h���Ǘ�����N���X
class thread_handle
{
private:
	thread_object *thread_;		///< @brief �X���b�h�̎���

public:

	/// @brief �X���b�h���쐬����
	bool create( ){ return( thread_ == NULL ? false : thread_->create( ) ); }


	/// @brief �X���b�h���I�����邩�C�^�C���A�E�g����������܂őҋ@����
	//! 
	//! @param[in] dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
	//! 
	bool wait( unsigned long dwMilliseconds = INFINITE ){ return( thread_ == NULL ? false : thread_->wait( dwMilliseconds ) ); }


	/// @brief �X���b�h���g�p���Ă������\�[�X���J������
	bool close( )
	{
		if( thread_ == NULL )
		{
			return( false );
		}
		bool b = thread_->close( );
		delete thread_;
		thread_ = NULL;
		return( b );
	}

public:
	thread_handle( ) : thread_( NULL ){ }
	thread_handle( thread_object *t ) : thread_( t ){ }

	const thread_handle &operator =( const thread_handle &t )
	{
		if( &t != this )
		{
			thread_ = t.thread_;
		}
		return( *this );
	}
};

/// @brief �w�肵���X���b�h�֐��𗘗p����C�X���b�h���쐬����
//! 
//! @param[in,out] param �c �X���b�h�̊֐��ɓn���p�����[�^
//! @param[in]     f     �c ���s�����X���b�h�֐�
//!
//! @return �X���b�h���Ǘ����� thread_handle �I�u�W�F�N�g
//! 
//! @code �g�p��
//! struct parameter{ ... ���炩�̃p�����[�^ ... };   // �X���b�h�֐��ɓn���p�����[�^�i���ɍ\���̂ł���K�v�͖����j
//! 
//! void thread_function( const parameter &p );       // �X���b�h�֐��i�����ŉ��炩�̏���������j
//! 
//! // ����ȍ~�ŃX���b�h���쐬����
//! parameter param[ �쐬�X���b�h�� ];
//! mist::thread_handle t[ �쐬�X���b�h�� ];
//! 
//! for( i = 0 ; i < �쐬�X���b�h�� ; i++ )
//! {
//!     �p�����[�^�ɉ��炩�̒l��ݒ肷��
//! }
//! 
//! for( i = 0 ; i < �쐬�X���b�h�� ; i++ )
//! {
//!     // �X���b�h���쐬����
//!     t[ i ] = mist::create_thread( param[ i ], thread_function );
//! }
//! 
//! for( i = 0 ; i < �쐬�X���b�h�� ; i++ )
//! {
//!     // �X���b�h���I������܂ő҂�������
//!     mist::wait_thread( t[ i ] );
//! }
//! 
//! for( i = 0 ; i < �쐬�X���b�h�� ; i++ )
//! {
//!     // �X���b�h���g�p���Ă������\�[�X���J������
//!     mist::close_thread( t[ i ] );
//! }
//! @endcode
//! 
template < class Param, class Functor >
inline thread_handle create_thread( Param &param, Functor f )
{
	thread_handle thread_( new __thread_controller__::thread_object_functor< Param, Functor >( param, f ) );
	thread_.create( );
	return( thread_ );
}

/// @brief �w�肵���X���b�h�֐��𗘗p����C�X���b�h�𕡐��쐬����
//! 
//! @param[out]    handles     �c �X���b�h�I�u�W�F�N�g
//! @param[in,out] param       �c �X���b�h�̊֐��ɓn���p�����[�^
//! @param[in]     num_threads �c �X���b�h��
//! @param[in]     f           �c ���s�����X���b�h�֐�
//! 
template < class Param, class Functor >
inline void create_threads( thread_handle *handles, Param *param, size_t num_threads, Functor f )
{
	for( size_t i = 0 ; i < num_threads ; i++ )
	{
		handles[ i ] = thread_handle( new __thread_controller__::thread_object_functor< Param, Functor >( param[ i ], f ) );
        handles[ i ].create( );
	}
}


/// @brief �X���b�h���g�p���Ă������\�[�X���J������
//! 
//! �X���b�h���g�p������ŁC�K���Ăяo���K�v������
//! 
//! @param[in,out] thread_ �c �X���b�h�I�u�W�F�N�g
//!
//! @retval true  �c ���\�[�X�̊J���ɐ���
//! @retval false �c ���\�[�X�̊J���Ɏ��s
//! 
inline bool close_thread( thread_handle &thread_ ){ return( thread_.close( ) ); }

/// @brief �����̃X���b�h���g�p���Ă������\�[�X���J������
//! 
//! @param[out]    handles     �c �X���b�h�I�u�W�F�N�g
//! @param[in]     num_threads �c �X���b�h��
//!
//! @retval true  �c �S�Ẵ��\�[�X�̊J���ɐ���
//! @retval false �c �S�Ẵ��\�[�X�̊J���Ɏ��s
//! 
inline bool close_threads( thread_handle *handles, size_t num_threads )
{
	bool ret = true;
	for( size_t i = 0 ; i < num_threads ; i++ )
	{
        if( !handles[ i ].close( ) )
		{
			ret = false;
		}
	}
	return( ret );
}


/// @brief �X���b�h���I�����邩�C�^�C���A�E�g����������܂őҋ@����
//! 
//! �^�C���A�E�g�� INFINITE �ɐݒ肷�邱�ƂŁC�X���b�h���I������܂ő҂�������
//! 
//! @param[in,out] thread_        �c �X���b�h�I�u�W�F�N�g
//! @param[in]     dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �X���b�h���^�C���A�E�g���ԓ��ɐ���I�������ꍇ
//! @retval false �c �X���b�h���^�C���A�E�g���ԓ��ɏI�����Ȃ������ꍇ
//! 
inline bool wait_thread( thread_handle &thread_, unsigned long dwMilliseconds = INFINITE ){ return( thread_.wait( dwMilliseconds ) ); }

/// @brief �����̃X���b�h���g�p���Ă������\�[�X���J������
//! 
//! @param[out]    handles        �c �X���b�h�I�u�W�F�N�g
//! @param[in]     num_threads    �c �X���b�h��
//! @param[in]     dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �����̃X���b�h���^�C���A�E�g���ԓ��ɐ���I�������ꍇ
//! @retval false �c �����̃X���b�h���^�C���A�E�g���ԓ��ɏI�����Ȃ������ꍇ
//! 
inline bool wait_threads( thread_handle *handles, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	if( dwMilliseconds == INFINITE )
	{
		bool ret = true;
		for( size_t i = 0 ; i < num_threads ; i++ )
		{
			if( !handles[ i ].wait( INFINITE ) )
			{
				ret = false;
			}
		}
		return( ret );
	}
	else
	{
		bool ret = true;
		for( size_t i = 0 ; i < num_threads ; i++ )
		{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
			DWORD st = timeGetTime( );
#else
			timeval dmy;
			gettimeofday( &dmy, NULL );
			unsigned long st = dmy.tv_sec * 1000 + dmy.tv_usec / 1000;
#endif
			if( !handles[ i ].wait( dwMilliseconds ) )
			{
				ret = false;
			}

#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
			DWORD et = timeGetTime( );
#else
			gettimeofday( &dmy, NULL );
			unsigned long et = dmy.tv_sec * 1000 + dmy.tv_usec / 1000;
#endif

			if( st + dwMilliseconds <= et )
			{
				break;
			}
			else
			{
				dwMilliseconds -= et - st;
			}
		}
		return( ret );
	}
}


/// @brief �X���b�h�����E�I���ҋ@�E�X���b�h�̔j���܂ł���A�̗���Ƃ��čs��
//! 
//! @param[in,out] param          �c �X���b�h�̊֐��ɓn���p�����[�^
//! @param[in]     f              �c ���s�����X���b�h�֐�
//! @param[in]     dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �X���b�h�̎��s�ɐ���
//! @retval false �c �X���b�h�̎��s�Ɏ��s
//! 
template < class Param, class Functor >
inline bool do_thread( Param &param, Functor f, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;

	// �X���b�h�̐���
	thread_handle thread_ = create_thread( param, f );

	// �X���b�h�̏I���҂�
	if( !wait_thread( thread_, dwMilliseconds ) )
	{
		ret = false;
	}

	// ���\�[�X�̊J��
	if( !close_thread( thread_ ) )
	{
		ret = false;
	}
	return( ret );
}


/// @brief �X���b�h�����E�I���ҋ@�E�X���b�h�̔j���܂ł���A�̗���Ƃ��čs��
//! 
//! @param[in,out] params         �c �X���b�h�̊֐��ɓn���p�����[�^
//! @param[in]     num_threads    �c �X���b�h��
//! @param[in]     f              �c ���s�����X���b�h�֐�
//! @param[in]     dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �����̃X���b�h�̎��s�ɐ���
//! @retval false �c �����̃X���b�h�̎��s�Ɏ��s
//! 
template < class Param, class Functor >
inline bool do_threads( Param *params, size_t num_threads, Functor f, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	thread_handle *threads_ = new thread_handle[ num_threads ];

	// �X���b�h�̐���
	create_threads( threads_, params, num_threads, f );

	// �X���b�h�̏I���҂�
	if( !wait_threads( threads_, num_threads, dwMilliseconds ) )
	{
		ret = false;
	}

	// ���\�[�X�̊J��
	if( !close_threads( threads_, num_threads ) )
	{
		ret = false;
	}

	delete [] threads_;

	return( ret );
}


/// @}
//  �X���b�h�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_THREAD__
