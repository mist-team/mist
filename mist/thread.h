#ifndef __INCLUDE_MIST_THREAD__
#define __INCLUDE_MIST_THREAD__


// スレッド用ライブラリのインクルード
// UNIX系とWindows用を使い分ける
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないので特に必要なインクルードファイルは無し
#elif defined( WIN32 )
	#include <windows.h>
#else
	#include <pthread.h>
	#include <unistd.h>
#endif

#ifndef INFINITE
#define INFINITE	( ( unsigned long ) -1 )
#endif

#include <map>
#include <string>

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_SINGLETON__
#include "singleton.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


inline unsigned int get_cpu_num( )
{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないのでCPU数は常に1
	return( 1 );
#elif defined( WIN32 )
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );
	return ( sysInfo.dwNumberOfProcessors );
#else
	return ( ( unsigned int ) sysconf( _SC_NPROCESSORS_ONLN ) );
#endif
}


// スレッド用関数
typedef unsigned long ThreadExitCode;
typedef ThreadExitCode( LPTHREADFUNC ) ( void *thread_param );


// スレッドを操作する，識別する変数
struct thread_dmy_class{ };

// スレッドを操作する一番ベースとなるクラス
struct thread_object
{
	virtual bool create( ) = 0;
	virtual bool wait( unsigned long dwMilliseconds = INFINITE ) = 0;
	virtual bool close( ) = 0;
	virtual bool suspend( ) = 0;
	virtual bool resume( ) = 0;
};


template < class thread_parameter = thread_dmy_class >
class thread : public thread_object
{
public:
	typedef unsigned long thread_exit_type;

private:

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないので特に必要な変数は無し
#elif defined( WIN32 )
	HANDLE    thread_handle_;			// Windows用のスレッドを識別するハンドル
	DWORD     thread_id_;				// Windows用のスレッドを識別するID
#else
	pthread_t thread_id_;				// pthreadライブラリでスレッドを識別するID
	bool      thread_finished_;			// スレッドが終了したかどうかを保持
#endif

	thread_exit_type thread_exit_code_;	// スレッドの戻り値

public:
	thread_exit_type exit_code( ) const { return( thread_exit_code_ ); }

	const thread &operator =( const thread &t )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしない
#elif defined( WIN32 )
		thread_handle_ = t.thread_handle_;
		thread_id_ = t.thread_id_;
#else
		thread_finished_ = t.thread_finished_;
		thread_id_ = t.thread_id_;
#endif
		thread_exit_code_ = t.thread_exit_code_;
		return( *this );
	}

	// threadの比較演算子
	bool operator ==( const thread &t ) const
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしない
		return( this == &t );
#elif defined( WIN32 )
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
	// スレッドサポートはしない
	thread( const thread &t ) :thread_exit_code_( t.thread_exit_code_ ){ }
	thread( ) : thread_exit_code_( 0 ){ }
#elif defined( WIN32 )
	thread( const thread &t ) : thread_handle_( t.thread_handle_ ), thread_id_( t.thread_id_ ), thread_exit_code_( t.thread_exit_code_ ){ }
	thread( ) : thread_handle_( NULL ), thread_id_( -1 ), thread_exit_code_( 0 ){ }
#else
	thread( const thread &t ) : thread_id_( t.thread_id ), thread_finished_( t.thread_finished ), thread_exit_code_( t.thread_exit_code ){ }
	thread(  ) : thread_id_( ( pthread_t ) ( -1 ) ), thread_finished_( false ), thread_exit_code_( 0 ){ }
#endif

	virtual ~thread( ){ }


	virtual bool create( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので直接関数を呼び出す
		bool ret = true;
		thread_exit_code_ = thread_function( static_cast< const thread_parameter & >( *this ) );
#elif defined( WIN32 )
		if( thread_handle_ != NULL ) return( false );
		thread_handle_ = CreateThread( NULL, 0, map_thread_function, ( void * )this, 0, &( thread_id_ ) );
		bool ret = thread_handle_ != NULL ? true : false;
#else
		if( thread_id_ != ( pthread_t ) ( -1 ) ) return( false );
		bool ret = pthread_create( &( thread_id_ ), NULL, map_thread_function, ( void * )this ) == 0 ? true : false;
#endif

		return ( ret );
	}

	virtual bool wait( unsigned long dwMilliseconds = INFINITE )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので何もしない
		// スレッドかされないため，dwMilliseconds は常に INFINITE 扱いとなる
		return( true );
#elif defined( WIN32 )
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

	virtual bool close( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので常に true を返す
		return( true );
#elif defined( WIN32 )
		BOOL ret = CloseHandle( thread_handle_ );
		thread_handle_ = NULL;
		return ( ret != 0 );
#else
		int ret = pthread_detach( thread_id_ );
		thread_id_ = ( pthread_t ) ( -1 );
		return ( ret == 0 );
#endif
	}

	virtual bool suspend( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので常に true を返す
		return( true );
#elif defined( WIN32 )
		return( SuspendThread( thread_handle_ ) != static_cast< DWORD >( -1 ) );
#else
		return( pthread_suspend_np( thread_id_ ) == 0 );
#endif
	}

	virtual bool resume( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので常に true を返す
		return( true );
#elif defined( WIN32 )
		return( ResumeThread( thread_handle_ ) == 1 );
#else
		return( pthread_resume_np( thread_id_ ) == 0 );
#endif
	}


protected:
	// 継承した先で必ず実装されるスレッド関数
	virtual thread_exit_type thread_function( const thread_parameter &p ) = 0;


#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないので何もしない
#elif defined( WIN32 )
	static DWORD WINAPI map_thread_function( void *p )
	{
		thread *obj = static_cast< thread * >( p );
		obj->thread_exit_code_ = obj->thread_function( static_cast< thread_parameter & >( *obj ) );
		return( 0 );
	}
#else
	static void *map_thread_function( void *p )
	{
		thread *obj = static_cast< thread * >( p );
		obj->thread_exit_code_ = obj->thread_function( static_cast< thread_parameter & >( *obj ) );
		obj->thread_finished_ = true;
		return ( NULL );
	}
#endif
};



// スレッドを普通の関数形式の呼び出しで簡便に利用するための関数群
namespace __thread_controller__
{
	template < class Param, class Functor >
	class thread_object_functor : public thread< thread_object_functor< Param, Functor > >
	{
	private:
		const Param &param_;
		Functor func_;

	public:
		thread_object_functor( const Param &p, Functor f ) : param_( p ), func_( f ){ }

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( const thread_object_functor &p )
		{
			func_( param_ );
			return( 0 );
		}
	};
}

template < class Param, class Functor >
inline thread_object *create_thread( const Param &param, Functor f )
{
	thread_object *t = new __thread_controller__::thread_object_functor< Param, Functor >( param, f );
	t->create( );
	return( t );
}

inline bool close_thread( thread_object *t )
{
	if( t == NULL )
	{
		return( false );
	}
	bool b = t->close( );
	delete t;
	return( b );
}

inline bool wait_thread( thread_object *t, unsigned long dwMilliseconds = INFINITE )
{
	return( t == NULL ? false : t->wait( dwMilliseconds ) );
}

inline bool suspend_thread( thread_object *t )
{
	return( t == NULL ? false : t->suspend( ) );
}

inline bool resume_thread( thread_object *t )
{
	return( t == NULL ? false : t->resume( ) );
}


// スレッドの排他制御をサポートするクラス
struct lock_object
{
protected:
	::std::string lock_name_;

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	typedef char lock_object_type;					// スレッドサポートはしないのでダミー変数用意
#elif defined( WIN32 )
	typedef CRITICAL_SECTION lock_object_type;		// Windows用のロックオブジェクト(CRITIFCALSECTIONを利用)
#else
	typedef pthread_mutex_t lock_object_type;		// pthreadライブラリでのロックオブジェクト
#endif

	typedef ::std::map< ::std::string, lock_object_type > lock_table;

public:
	lock_object( ) : lock_name_( "mist default lock object!!" ){ }
	lock_object( const std::string &name ) : lock_name_( name ){ }

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0

	// スレッドサポートはしないので特に必何もしない
	bool lock( ){ return( true ); }
	bool unlock( ){ return( true ); }

#else

	bool lock( )
	{
		lock_table &table = singleton< lock_table >::get_instance( );
		lock_table::iterator ite = table.find( lock_name_ );
		if( ite == table.end( ) )
		{
			// まだロックオブジェクトを初期化していないので初期化する
			::std::pair< lock_table::iterator, bool > p = table.insert( lock_table::value_type( lock_name_, lock_object_type( ) ) );
			if( p.second )
			{
				lock_object_type &obj = p.first->second;
				initialize( obj );
				lock( obj );
			}
			else
			{
				// ロックオブジェクトをテーブルに追加することができませんでした・・・
				return( false );
			}
		}
		else
		{
			// すでに同名のロックオブジェクトが存在するのでそちらをロックする
			lock( ite->second );
		}
		return( true );
	}

	bool unlock( )
	{
		lock_table &table = singleton< lock_table >::get_instance( );
		lock_table::iterator ite = table.find( lock_name_ );
		if( ite == table.end( ) )
		{
			// 指定されたロックオブジェクトが見つからなかったので何もしない
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
	static void initialize( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( WIN32 )
		InitializeCriticalSection( &l );	// クリティカルセクションオブジェクトを初期化
#else
		pthread_mutex_init( &l, NULL );		// pthread用のMutexオブジェクトを初期化
#endif
	}

	static void lock( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( WIN32 )
		EnterCriticalSection( &l );		// クリティカルセクションオブジェクトをロック
#else
		pthread_mutex_lock( &l );		// pthread用のMutexオブジェクトをロック
#endif
	}

	static void unlock( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( WIN32 )
		LeaveCriticalSection( &l );		// クリティカルセクションオブジェクトをアンロック
#else
		pthread_mutex_unlock( &l );		// pthread用のMutexオブジェクトをアンロック
#endif
	}
};




// スレッドの排他制御を簡便に記述するためのヘルパクラス
class lock
{
protected:
	lock_object lock_object_;

public:
	lock( ) : lock_object_( )
	{
		lock_object_.lock( );
	}

	lock( const std::string &name ) : lock_object_( name )
	{
		lock_object_.lock( );
	}

	~lock( )
	{
		lock_object_.unlock( );
	}
};


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_THREAD__
