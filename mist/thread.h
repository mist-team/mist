#ifndef __INCLUDE_MIST_THREAD__
#define __INCLUDE_MIST_THREAD__


// �X���b�h�p���C�u�����̃C���N���[�h
// UNIX�n��Windows�p���g��������
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#ifndef INFINITE
#define INFINITE	( ( unsigned long ) -1 )
#endif

#include "config/mist_conf.h"


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


inline unsigned int get_cpu_num( )
{
#ifdef WIN32
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );
	return ( sysInfo.dwNumberOfProcessors );
#else
	return ( ( unsigned int ) sysconf( _SC_NPROCESSORS_ONLN ) );
#endif
}


// �X���b�h�p�֐�
typedef unsigned long ThreadExitCode;
typedef ThreadExitCode( LPTHREADFUNC ) ( void *thread_param );


// �X���b�h�𑀍삷��C���ʂ���ϐ�
struct thread_dmy_class{ };

template < class thread_parameter = thread_dmy_class >
class thread_object
{
public:
	typedef unsigned long thread_exit_type;

private:

#ifdef WIN32
	HANDLE    thread_handle_;			// Windows�p�̃X���b�h�����ʂ���n���h��
	DWORD     thread_id_;				// Windows�p�̃X���b�h�����ʂ���ID
#else
	pthread_t thread_id_;				// pthread���C�u�����ŃX���b�h�����ʂ���ID
	bool      thread_finished_;			// �X���b�h���I���������ǂ�����ێ�
#endif

	thread_exit_type thread_exit_code_;	// �X���b�h�̖߂�l

public:
	thread_exit_type exit_code( ) const { return( thread_exit_code_ ); }

	const thread_object &operator =( const thread_object &t )
	{
#ifdef WIN32
		thread_handle_ = t.thread_handle_;
#else
		thread_finished_ = t.thread_finished_;
#endif
		thread_id_ = t.thread_id_;
		thread_exit_code_ = t.thread_exit_code_;
		return( *this );
	}

	// thread_object�̔�r���Z�q
	bool operator ==( const thread_object &t ) const
	{
#ifdef WIN32
		return ( thread_id_ == t.thread_id_ );
#else
		return ( pthread_equal( thread_id_, t.thread_id_ ) != 0 );
#endif
	}

	bool operator !=( const thread_object &t ) const
	{
		return( !( *this == t ) );
	}


#ifdef WIN32
	thread_object( const thread_object &t ) : thread_handle_( t.thread_handle_ ), thread_id_( t.thread_id_ ), thread_exit_code_( t.thread_exit_code_ ){ }
	thread_object( ) : thread_handle_( NULL ), thread_id_( -1 ), thread_exit_code_( 0 ){ }
#else
	thread_object( const thread_object &t ) : thread_id_( t.thread_id ), thread_finished_( t.thread_finished ), thread_exit_code_( t.thread_exit_code ){ }
	thread_object(  ) : thread_id_( ( pthread_t ) ( -1 ) ), thread_finished_( false ), thread_exit_code_( 0 ){ }
#endif


	bool create_thread( )
	{
#ifdef WIN32
		if( thread_handle_ != NULL ) return( false );
		thread_handle_ = CreateThread( NULL, 0, map_thread_function, ( void * )this, 0, &( thread_id_ ) );
		bool ret = thread_handle_ != NULL ? true : false;
#else
		if( thread_id_ != ( pthread_t ) ( -1 ) ) return( false );
		bool ret = pthread_create( &( thread_id_ ), NULL, map_thread_function, ( void * )this ) == 0 ? true : false;
#endif

		return ( ret );
	}

	bool wait_thread( unsigned long dwMilliseconds = INFINITE )
	{
#ifdef WIN32
		DWORD ret = WaitForSingleObject( thread_handle_, dwMilliseconds );
		return ( SUCCEEDED( ret ) );
#else
		if( dwMilliseconds == INFINITE )
		{
			return ( pthread_join( thread_id_, NULL ) == 0 );
		}
		else
		{
			unsigned long count = 0;

			while( true )
			{
				usleep( 1 );

				if( count < dwMilliseconds )
				{
					if( thread_finished_ )
					{
						return ( true );
					}
				}
				else
				{
					return ( false );
				}

				count++;
			}
		}
#endif
	}

	bool close_thread( )
	{
#ifdef WIN32
		BOOL ret = CloseHandle( thread_handle_ );
		thread_handle_ = NULL;
		return ( ret != 0 );
#else
		int ret = pthread_detach( thread_id_ );
		thread_id_ = ( pthread_t ) ( -1 );
		return ( ret == 0 );
#endif
	}


protected:
	// �p��������ŕK�����������X���b�h�֐�
	virtual thread_exit_type thread_function( const thread_parameter &p ) = 0;


#ifdef WIN32
	static DWORD WINAPI map_thread_function( void *p )
	{
		thread_object *obj = (thread_object *)p;
		obj->thread_exit_code_ = obj->thread_function( static_cast< thread_parameter & >( *obj ) );
		return( 0 );
	}
#else
	static void *map_thread_function( void *p )
	{
		thread_object *obj = (thread_object *)p;
		obj->thread_exit_code_ = obj->thread_function( static_cast< thread_parameter & >( *obj ) );
		obj->thread_finished_ = true;
		return ( NULL );
	}
#endif
};



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_THREAD__
