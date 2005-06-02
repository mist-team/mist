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
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	#include <windows.h>
#else
	#include <pthread.h>
	#include <unistd.h>
	#include <time.h>
#endif

#ifndef INFINITE
#define INFINITE	( ( unsigned long ) -1 )
#endif

#include <map>
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
	virtual bool suspend( ) = 0;
	virtual bool resume( ) = 0;
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


inline bool sleep( size_t dwMilliseconds )
{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	return( SleepEx( dwMilliseconds, true ) == 0 );
#else
	timespec treq, trem;
	treq.tv_sec = static_cast< time_t >( dwMilliseconds / 1000 );
	treq.tv_nsec = static_cast< long >( ( dwMilliseconds % 1000 ) * 1000 );
	return( nanosleep( &treq, &trem ) == 0 );
#endif
}


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
	HANDLE    thread_handle_;			// Windows�p�̃X���b�h�����ʂ���n���h��
	DWORD     thread_id_;				// Windows�p�̃X���b�h�����ʂ���ID
#else
	pthread_t thread_id_;				// pthread���C�u�����ŃX���b�h�����ʂ���ID
	bool      thread_finished_;			// �X���b�h���I���������ǂ�����ێ�
#endif

	thread_exit_type thread_exit_code_;	// �X���b�h�̖߂�l

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
		thread_finished_ = t.thread_finished_;
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
	thread( const thread &t ) :thread_exit_code_( t.thread_exit_code_ ){ }
	thread( ) : thread_exit_code_( 0 ){ }
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	thread( const thread &t ) : thread_handle_( t.thread_handle_ ), thread_id_( t.thread_id_ ), thread_exit_code_( t.thread_exit_code_ ){ }
	thread( ) : thread_handle_( NULL ), thread_id_( -1 ), thread_exit_code_( 0 ){ }
#else
	thread( const thread &t ) : thread_id_( t.thread_id ), thread_finished_( t.thread_finished ), thread_exit_code_( t.thread_exit_code ){ }
	thread(  ) : thread_id_( ( pthread_t ) ( -1 ) ), thread_finished_( false ), thread_exit_code_( 0 ){ }
#endif

	virtual ~thread( ){ }


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
		thread_handle_ = CreateThread( NULL, 0, map_thread_function, ( void * )this, 0, &( thread_id_ ) );
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
		BOOL ret = CloseHandle( thread_handle_ );
		thread_handle_ = NULL;
		return ( ret != 0 );
#else
		int ret = pthread_detach( thread_id_ );
		thread_id_ = ( pthread_t ) ( -1 );
		return ( ret == 0 );
#endif
	}


	/// @brief �X���b�h���T�X�y���h������
	//! 
	//! @attention Windows�ȊO�̊��ł́Cpthread ���C�u�������ŃT�|�[�g����Ă��Ȃ����Ƃ����邽�߁C���݂̂Ƃ���Windows�̂݃T�|�[�g
	//! 
	//! @retval true  �c �T�X�y���h�ɐ���
	//! @retval false �c �T�X�y���h�Ɏ��s
	//! 
	virtual bool suspend( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂ŏ�� true ��Ԃ�
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( SuspendThread( thread_handle_ ) != static_cast< DWORD >( -1 ) );
#else
//		return( pthread_suspend_np( thread_id_ ) == 0 );
		return( false );
#endif
	}


	/// @brief �X���b�h�����W���[������
	//! 
	//! @attention Windows�ȊO�̊��ł́Cpthread ���C�u�������ŃT�|�[�g����Ă��Ȃ����Ƃ����邽�߁C���݂̂Ƃ���Windows�݂̂̃T�|�[�g
	//! 
	//! @retval true  �c ���W���[���ɐ���
	//! @retval false �c ���W���[���Ɏ��s
	//! 
	virtual bool resume( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// �X���b�h�T�|�[�g�͂��Ȃ��̂ŏ�� true ��Ԃ�
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( ResumeThread( thread_handle_ ) == 1 );
#else
//		return( pthread_resume_np( thread_id_ ) == 0 );
		return( false );
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
	static DWORD WINAPI map_thread_function( void *p )
	{
		thread *obj = static_cast< thread * >( p );
		obj->thread_exit_code_ = obj->thread_function( );
		return( 0 );
	}
#else
	static void *map_thread_function( void *p )
	{
		thread *obj = static_cast< thread * >( p );
		obj->thread_exit_code_ = obj->thread_function( );
		obj->thread_finished_ = true;
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
	template < class Param, class Functor >
	class thread_object_functor : public thread< thread_object_functor< Param, Functor > >
	{
	public:
		typedef thread< thread_object_functor< Param, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;

	private:
		Param &param_;
		Functor func_;

	public:
		thread_object_functor( Param &p, Functor f ) : param_( p ), func_( f ){ }

	protected:
		// �p��������ŕK�����������X���b�h�֐�
		virtual thread_exit_type thread_function( )
		{
			func_( param_ );
			return( 0 );
		}
	};
}


//! @addtogroup thread_group �X���b�h
//!
//!  @{



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
	bool wait( unsigned long dwMilliseconds = INFINITE ){ return( thread_ == NULL ? false : thread_->wait( INFINITE ) ); }


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


	/// @brief �X���b�h���T�X�y���h����
	bool suspend( ){ return( thread_ == NULL ? false : thread_->suspend( ) ); }


	/// @brief �X���b�h�����W���[������
	bool resume( ){ return( thread_ == NULL ? false : thread_->resume( ) ); }

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
		handles[ i ] = thread_handle( new __thread_controller__::thread_object_functor< Param, Functor >( param, f ) );
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
//! @retval true  �c �X���b�h���^�C���A���Ǝ��ԓ��ɐ���I�������ꍇ
//! @retval false �c �X���b�h���^�C���A���Ǝ��ԓ��ɏI�����Ȃ������ꍇ
//! 
inline bool wait_thread( thread_handle &thread_, unsigned long dwMilliseconds = INFINITE ){ return( thread_.wait( dwMilliseconds ) ); }

/// @brief �����̃X���b�h���g�p���Ă������\�[�X���J������
//! 
//! @param[out]    handles        �c �X���b�h�I�u�W�F�N�g
//! @param[in]     num_threads    �c �X���b�h��
//! @param[in]     dwMilliseconds �c �^�C���A�E�g���ԁi�~���b�P�ʁj
//!
//! @retval true  �c �����̃X���b�h���^�C���A���Ǝ��ԓ��ɐ���I�������ꍇ
//! @retval false �c �����̃X���b�h���^�C���A���Ǝ��ԓ��ɏI�����Ȃ������ꍇ
//! 
inline bool wait_threads( thread_handle *handles, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	for( size_t i = 0 ; i < num_threads ; i++ )
	{
        if( !handles[ i ].wait( dwMilliseconds ) )
		{
			ret = false;
		}
	}
	return( ret );
}


/// @brief �X���b�h���T�X�y���h������
//! 
//! @attention Windows���ł̂݃T�|�[�g
//! 
//! @param[in,out] thread_ �c �X���b�h�I�u�W�F�N�g
//!
//! @retval true  �c �T�X�y���h�ɐ���
//! @retval false �c �T�X�y���h�Ɏ��s
//! 
inline bool suspend_thread( thread_handle &thread_ ){ return( thread_.suspend( ) ); }

/// @brief �����̃X���b�h���g�p���Ă������\�[�X���J������
//! 
//! @param[out]    handles     �c �X���b�h�I�u�W�F�N�g
//! @param[in]     num_threads �c �X���b�h��
//!
//! @retval true  �c �����̃X���b�h�̃T�X�y���h�ɐ���
//! @retval false �c �����̃X���b�h�̃T�X�y���h�Ɏ��s
//! 
inline bool suspend_threads( thread_handle *handles, size_t num_threads )
{
	bool ret = true;
	for( size_t i = 0 ; i < num_threads ; i++ )
	{
        if( !handles[ i ].suspend( ) )
		{
			ret = false;
		}
	}
	return( ret );
}


/// @brief �X���b�h�����W���[������
//! 
//! @attention Windows���ł̂݃T�|�[�g
//! 
//! @param[in,out] thread_ �c �X���b�h�I�u�W�F�N�g
//!
//! @retval true  �c ���W���[���ɐ���
//! @retval false �c ���W���[���Ɏ��s
//! 
inline bool resume_thread( thread_handle &thread_ ){ return( thread_.resume( ) ); }

/// @brief �����̃X���b�h���g�p���Ă������\�[�X���J������
//! 
//! @param[out]    handles     �c �X���b�h�I�u�W�F�N�g
//! @param[in]     num_threads �c �X���b�h��
//!
//! @retval true  �c �����̃X���b�h�̃��W���[���ɐ���
//! @retval false �c �����̃X���b�h�̃��W���[���Ɏ��s
//! 
inline bool resume_threads( thread_handle *handles, size_t num_threads )
{
	bool ret = true;
	for( size_t i = 0 ; i < num_threads ; i++ )
	{
        if( !handles[ i ].resume( ) )
		{
			ret = false;
		}
	}
	return( ret );
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

	return( ret );
}





/// @brief �X���b�h�̔r��������T�|�[�g����N���X
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

	typedef ::std::map< ::std::string, lock_object_type > lock_table;

public:
	lock_object( ) : lock_name_( "mist default lock object!!" ){ }
	lock_object( const std::string &name ) : lock_name_( name ){ }

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0

	// �X���b�h�T�|�[�g�͂��Ȃ��̂œ��ɕK�������Ȃ�
	bool lock( ){ return( true ); }
	bool unlock( ){ return( true ); }

#else

	/// @brief �r������p�̃I�u�W�F�N�g�����b�N����
	//! 
	//! @retval true  �c ���b�N�ɐ���
	//! @retval false �c ���b�N�I�u�W�F�N�g�̐����Ɏ��s
	//! 
	bool lock( )
	{
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
				lock( obj );
			}
			else
			{
				// ���b�N�I�u�W�F�N�g���e�[�u���ɒǉ����邱�Ƃ��ł��܂���ł����E�E�E
				return( false );
			}
		}
		else
		{
			// ���łɓ����̃��b�N�I�u�W�F�N�g�����݂���̂ł���������b�N����
			lock( ite->second );
		}
		return( true );
	}


	/// @brief �r������p�̃I�u�W�F�N�g�����b�N����������
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
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


/// @}
//  �X���b�h�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_THREAD__
