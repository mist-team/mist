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
//! @brief 簡便にスレッドを利用可能にするためのライブラリ
//!
#ifndef __INCLUDE_MIST_THREAD__
#define __INCLUDE_MIST_THREAD__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_SINGLETON__
#include "singleton.h"
#endif


// スレッド用ライブラリのインクルード
// UNIX系とWindows用を使い分ける
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないので特に必要なインクルードファイルは無し
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


// mist名前空間の始まり
_MIST_BEGIN



// スレッド用関数
typedef unsigned long ThreadExitCode;
typedef ThreadExitCode ( LPTHREADFUNC ) ( void *thread_param );


// スレッドを操作する，識別する変数
struct thread_dmy_class{ };

// スレッドを操作する一番ベースとなるクラス
struct thread_object
{
	virtual bool create( ) = 0;
	virtual bool wait( unsigned long dwMilliseconds = INFINITE ) = 0;
	virtual bool close( ) = 0;
	virtual ~thread_object( ){ }
};


//! @addtogroup thread_group スレッド
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/thread.h>
//! @endcode
//!
//!  @{


/// @brief 利用可能なCPU数を取得する
//! 
//! Windowsの場合は GetSystemInfo 関数を利用してCPU数を取得する．
//! Linux系の場合は，sysconf 関数を利用してCPU数を取得する．
//! 
//! @return 利用可能なCPU数
//! 
inline size_t get_cpu_num( )
{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないのでCPU数は常に1
	return( 1 );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	SYSTEM_INFO sysInfo;
	GetSystemInfo( &sysInfo );
	return( static_cast< size_t >( sysInfo.dwNumberOfProcessors ) );
#else
	return( static_cast< size_t >( sysconf( _SC_NPROCESSORS_ONLN ) ) );
#endif
}


/// @brief 指定した時間だけスリープする（ミリ秒単位）
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


/// @brief スレッドの排他制御をサポートする最も単純なクラス
//! 
//! ロックするオブジェクトを共有する場合に使用する最もシンプルなクラス
//! 
//! @attention ロックオブジェクトの生成時点では，ロックはされないことに注意
//! 
//! - Windows … クリティカルセクションを利用
//! - Linux系 … ミューテックスを利用
//! 
//! @code 使用例
//! simple_lock_object l;
//! 
//! ... 何らかの処理 ...
//! 
//! l.lock( );   // <- これ以降の処理1を排他制御する
//! 
//! ... 処理1 ...
//! 
//! l.unlock( ); // <- ここまでの処理1が排他制御される
//! 
//! 
//! ... 何らかの処理 ...
//! 
//! 
//! l.lock( );   // <- これ以降の処理2を排他制御する
//! 
//! ... 処理2 ...
//! 
//! l.unlock( ); // <- ここまでの処理2が排他制御される
//! 
//! @endcode
//! 
struct simple_lock_object
{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	typedef char lock_object_type;					// スレッドサポートはしないのでダミー変数用意
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	typedef CRITICAL_SECTION lock_object_type;		// Windows用のロックオブジェクト(CRITIFCALSECTIONを利用)
#else
	typedef pthread_mutex_t lock_object_type;		// pthreadライブラリでのロックオブジェクト
#endif

	lock_object_type __lock__;	///< @brief ロック用オブジェクト

public:
	/// @brief コンストラクタ
	simple_lock_object( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		//InitializeCriticalSectionAndSpinCount( &__lock__, 4000 );	// クリティカルセクションオブジェクトを初期化
		InitializeCriticalSection( &__lock__ );	// クリティカルセクションオブジェクトを初期化
#else
		pthread_mutex_init( &__lock__, NULL );		// pthread用のMutexオブジェクトを初期化
#endif
	}

	/// @brief デストラクタ
	~simple_lock_object( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		DeleteCriticalSection( &__lock__ );		// クリティカルセクションオブジェクトを削除
#else
		pthread_mutex_destroy( &__lock__ );		// pthread用のMutexオブジェクトを初期化
#endif
	}

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0

	// スレッドサポートはしないので特に必何もしない
	bool lock( ){ return( true ); }
	bool unlock( ){ return( true ); }
	bool try_lock( ){ return( true ); }

#else

	/// @brief 排他制御用オブジェクトをロックする
	//! 
	//! @retval true  … ロックに成功
	//! @retval false … ロックに失敗
	//! 
	bool lock( )
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		EnterCriticalSection( &__lock__ );		// クリティカルセクションオブジェクトをロック
#else
		pthread_mutex_lock( &__lock__ );		// pthread用のMutexオブジェクトをロック
#endif

		return( true );
	}

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
	/// @brief 排他制御用オブジェクトのロックを試みる
	//! 
	//! 既にロックされていた場合は，ロックせずに false を返す．
	//! 
	//! @retval true  … ロックに成功
	//! @retval false … ロックに失敗
	//! 
	bool try_lock( )
	{
		// クリティカルセクションオブジェクトのロックが可能かを調べる
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( TryEnterCriticalSection( &__lock__ ) != FALSE );
#else
		return( pthread_mutex_trylock( &__lock__ ) == 0 );
#endif
	}
#endif

	/// @brief 排他制御用オブジェクトのロックを解除する
	bool unlock( )
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		LeaveCriticalSection( &__lock__ );		// クリティカルセクションオブジェクトをアンロック
#else
		pthread_mutex_unlock( &__lock__ );		// pthread用のMutexオブジェクトをアンロック
#endif

		return( true );
	}

#endif
};

/// @}
//  スレッドグループの終わり

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
		// スレッドサポートはしないので特に必何もしない
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		DeleteCriticalSection( &l );		// クリティカルセクションオブジェクトを削除
#else
		pthread_mutex_destroy( &l );		// pthread用のMutexオブジェクトを初期化
#endif
	}
};

//! @addtogroup thread_group スレッド
//!
//!  @{


/// @brief スレッドの排他制御をサポートするクラス
//! 
//! ロックするオブジェクトを名前を用いて共有化することが可能なクラス．
//! 同名のロックオブジェクトを作成することで別々のスレッド関数内での排他制御が可能となる．
//! 
//! @attention ロックオブジェクトの生成時点では，ロックはされないことに注意
//! 
//! - Windows … クリティカルセクションを利用
//! - Linux系 … ミューテックスを利用
//! 
//! @code 使用例
//! lock_object l( "ロックオブジェクトの名前" );  // <- 名前を指定しないと，デフォルトのロックオブジェクトが使用される
//! 
//! ... 何らかの処理 ...
//! 
//! l.lock( );   // <- これ以降の処理1を排他制御する
//! 
//! ... 処理1 ...
//! 
//! l.unlock( ); // <- ここまでの処理1が排他制御される
//! 
//! 
//! ... 何らかの処理 ...
//! 
//! 
//! l.lock( );   // <- これ以降の処理2を排他制御する
//! 
//! ... 処理2 ...
//! 
//! l.unlock( ); // <- ここまでの処理2が排他制御される
//! 
//! @endcode
//! 
struct lock_object
{
protected:
	::std::string lock_name_;

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	typedef char lock_object_type;					// スレッドサポートはしないのでダミー変数用意
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	typedef CRITICAL_SECTION lock_object_type;		// Windows用のロックオブジェクト(CRITIFCALSECTIONを利用)
#else
	typedef pthread_mutex_t lock_object_type;		// pthreadライブラリでのロックオブジェクト
#endif

	typedef lock_object_table< lock_object_type > lock_table;

public:
	lock_object( ) : lock_name_( "mist default lock object!!" ){ }
	lock_object( const std::string &name ) : lock_name_( name ){ }

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0

	// スレッドサポートはしないので特に必何もしない
	bool lock( ){ return( true ); }
	bool unlock( ){ return( true ); }

#else

	/// @brief 排他制御用オブジェクトをロックする
	//! 
	//! @retval true  … ロックに成功
	//! @retval false … ロックに失敗
	//! 
	bool lock( )
	{
		static lock_object_type &__double_lock__ = double_lock_object( );

		// テーブルの検索でスレッドが衝突しないようにロックする
		lock( __double_lock__ );

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

				// テーブル検索用のロックを開放する
				unlock( __double_lock__ );

				lock( obj );
			}
			else
			{
				// テーブル検索用のロックを開放する
				unlock( __double_lock__ );

				// ロックオブジェクトをテーブルに追加することができませんでした・・・
				return( false );
			}
		}
		else
		{
			// テーブル検索用のロックを開放する
			unlock( __double_lock__ );

			// すでに同名のロックオブジェクトが存在するのでそちらをロックする
			lock( ite->second );
		}

		return( true );
	}

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
	/// @brief 排他制御用オブジェクトのロックを試みる
	//! 
	//! 既にロックされていた場合は，ロックせずに false を返す．
	//! 
	//! @retval true  … ロックに成功
	//! @retval false … ロックに失敗
	//! 
	bool try_lock( )
	{
		static lock_object_type &__double_lock__ = double_lock_object( );

		// テーブルの検索でスレッドが衝突しないようにロックする
		lock( __double_lock__ );

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

				// テーブル検索用のロックを開放する
				unlock( __double_lock__ );

				return( try_lock( obj ) );
			}
			else
			{
				// テーブル検索用のロックを開放する
				unlock( __double_lock__ );

				// ロックオブジェクトをテーブルに追加することができませんでした・・・
				return( false );
			}
		}
		else
		{
			// テーブル検索用のロックを開放する
			unlock( __double_lock__ );

			// すでに同名のロックオブジェクトが存在するのでそちらをロックする
			return( try_lock( ite->second ) );
		}
	}
#endif

	/// @brief 排他制御用オブジェクトのロックを解除する
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
	/// @brief 内部で使用するダブルロックオブジェクトを取得する
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

	/// @brief ロックオブジェクトの初期化を行う
	//! 
	//! @param[in,out] l … ロックオブジェクト
	//! 
	static void initialize( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		InitializeCriticalSection( &l );	// クリティカルセクションオブジェクトを初期化
#else
		pthread_mutex_init( &l, NULL );		// pthread用のMutexオブジェクトを初期化
#endif
	}


	/// @brief ロックオブジェクトをロックする
	//! 
	//! @param[in,out] l … ロックオブジェクト
	//! 
	static void lock( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		EnterCriticalSection( &l );		// クリティカルセクションオブジェクトをロック
#else
		pthread_mutex_lock( &l );		// pthread用のMutexオブジェクトをロック
#endif
	}

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
	/// @brief 排他制御用オブジェクトのロックを試みる
	//! 
	//! 既にロックされていた場合は，ロックせずに false を返す．
	//! 
	//! @param[in,out] l … ロックオブジェクト
	//! 
	//! @retval true  … ロックに成功
	//! @retval false … ロックに失敗
	//! 
	static bool try_lock( lock_object_type &l )
	{
		// クリティカルセクションオブジェクトのロックが可能かを調べる
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( TryEnterCriticalSection( &l ) != FALSE );
#else
		return( pthread_mutex_trylock( &l ) == 0 );
#endif
	}
#endif

	/// @brief ロックオブジェクトをロックを解除する
	//! 
	//! @param[in,out] l … ロックオブジェクト
	//! 
	static void unlock( lock_object_type &l )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので特に必何もしない
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		LeaveCriticalSection( &l );		// クリティカルセクションオブジェクトをアンロック
#else
		pthread_mutex_unlock( &l );		// pthread用のMutexオブジェクトをアンロック
#endif
	}
};




/// @brief スレッドの排他制御を簡便に記述するためのヘルパクラス
//! 
//! @code 使用例
//! ... 何らかの処理 ...
//! {
//!     // このブロック内を排他制御する
//!     lock l( "ロックオブジェクトの名前" );  // <- 名前を指定しないと，デフォルトのロックオブジェクトが使用される
//! 
//!     ... 排他制御したい処理 ...
//! 
//!     // ここまでの処理が排他制御される
//! }
//! @endcode
//! 
class lock
{
protected:
	lock_object lock_object_;	///< @brief ロックオブジェクト

public:
	/// @brief デフォルトの名前でロックオブジェクトを生成し，ロックオブジェクトをロックする
	lock( ) : lock_object_( )
	{
		lock_object_.lock( );
	}


	/// @brief 指定した name の名前を持つロックオブジェクトを生成し，ロックオブジェクトをロックする
	lock( const std::string &name ) : lock_object_( name )
	{
		lock_object_.lock( );
	}


	/// @brief 開放時に，ロックオブジェクトのロックを解除する
	~lock( )
	{
		lock_object_.unlock( );
	}
};


#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
/// @brief スレッド間でシグナルを共有するためのクラス
class signal
{
protected:
	HANDLE handle_;

public:
	/// @brief コンストラクタ
	signal( ) : handle_( CreateEvent( NULL, FALSE, FALSE, NULL ) )
	{
	}

	/// @brief デストラクタ
	~signal( )
	{
		CloseHandle( handle_ );
	}

	/// @brief シグナル状態になるまで待機する
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

	/// @brief シグナルを送信する
	void send( )
	{
		SetEvent( handle_ );
	}

	/// @brief シグナルをクリアにする
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
	/// @brief コンストラクタ
	signal( ) : flag_( false )
	{
		pthread_mutex_init( &mutex_, NULL );
		pthread_cond_init( &cond_, NULL );
		pthread_mutex_lock ( &mutex_ );
	}

	/// @brief デストラクタ
	~signal( )
	{
		pthread_cond_destroy( &cond_ );
		pthread_mutex_destroy( &mutex_ );
	}

	/// @brief シグナル状態になるまで待機する
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

	/// @brief シグナルを送信する
	void send( )
	{
		flag_ = true;
		pthread_cond_broadcast( &cond_ );
	}

	/// @brief シグナルをクリアにする（pthreadの場合は何もしない）
	void reset( )
	{
		flag_ = false;
	}
};
#endif


/// @brief template 型のデータを扱うことができるスレッドクラス
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @attention _MIST_THREAD_SUPPORT_ が 1 の場合は，スレッドサポートは行わないため，シングルスレッド用に内部実装が変更される
//! 
//! @param thread_parameter … 引数の説明
//! 
template < class thread_parameter = thread_dmy_class >
class thread : public thread_object
{
public:
	typedef unsigned long thread_exit_type;		///< @brief スレッド終了時の戻り値の型

private:

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないので特に必要な変数は無し
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	HANDLE       thread_handle_;		// Windows用のスレッドを識別するハンドル
	unsigned int thread_id_;			// Windows用のスレッドを識別するID
#else
	pthread_t thread_id_;				// pthreadライブラリでスレッドを識別するID
	signal    finish_;					// スレッドの終了待ちを行うシグナル
#endif

	simple_lock_object exit_;				// 終了待ちシグナル
	thread_exit_type   thread_exit_code_;	// スレッドの戻り値

public:

	/// @brief スレッドが終了した時に返した戻り値を取得する
	thread_exit_type exit_code( ) const { return( thread_exit_code_ ); }


	/// @brief 他のスレッドオブジェクトと同じものを作成する
	//! 
	//! @attention 単なるコピーであり，別途スレッドが生成されるわけではない
	//! 
	//! @param[in] t … コピー元のスレッドオブジェクト
	//!
	//! @return 自分自身
	//! 
	const thread &operator =( const thread &t )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしない
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		thread_handle_ = t.thread_handle_;
		thread_id_ = t.thread_id_;
#else
		thread_id_ = t.thread_id_;
#endif
		thread_exit_code_ = t.thread_exit_code_;
		return( *this );
	}


	/// @brief 2つのスレッドオブジェクトが同じものかどうかを判定する
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] t … 比較対象のスレッドオブジェクト
	//!
	//! @retval true  … 同じスレッド
	//! @retval false … 別のスレッド
	//! 
	bool operator ==( const thread &t ) const
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしない
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
	// スレッドサポートはしない
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


	/// @brief スレッドを生成する
	//! 
	//! @retval true  … スレッドの作成に成功
	//! @retval false … スレッドの作成に失敗
	//! 
	virtual bool create( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので直接関数を呼び出す
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


	/// @brief スレッド無しで関数を実行する
	//! 
	//! @retval true  … スレッドの作成に成功
	//! @retval false … スレッドの作成に失敗
	//! 
	virtual bool create_without_thread( )
	{
		thread_exit_code_ = thread_function( );
		return ( true );
	}


	/// @brief スレッドが終了するか，タイムアウトになるまで待機する
	//! 
	//! タイムアウトを INFINITE に設定することで，スレッドが終了するまで待ち続ける
	//! 
	//! @param[in] dwMilliseconds … タイムアウト時間（ミリ秒単位）
	//! 
	//! @retval true  … スレッドがタイムアウト前に終了した
	//! @retval false … タイムアウトが発生したか，その他のエラーが発生
	//! 
	virtual bool wait( unsigned long dwMilliseconds = INFINITE )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので何もしない
		// スレッドかされないため，dwMilliseconds は常に INFINITE 扱いとなる
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


	/// @brief スレッドが使用していたリソースを開放する
	//! 
	//! スレッドが使用したリソースを開放するために，必ず1つのスレッドに対し1度だけ呼ばれる必要がある
	//! 
	//! @retval true  … 正常にスレッドを終了
	//! @retval false … スレッドの終了に失敗
	//! 
	virtual bool close( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので常に true を返す
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		if( thread_handle_ != NULL )
		{
			// スレッドの完全終了を待機する
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
			// スレッドの完全終了を待機する
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
	/// @brief 継承した先で必ず実装されるスレッド関数
	//! 
	//! 本クラスを継承し，スレッドの機能を持たせる場合に必ず実装しなくてはならない関数．
	//! スレッドの生成とともに本関数が呼ばれる．
	//! 
	//! @return スレッド終了時の戻り値
	//! 
	virtual thread_exit_type thread_function( ) = 0;


#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
	// スレッドサポートはしないので何もしない
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


/// @brief スレッド生成・終了待機・スレッドの破棄までを一連の流れとして行う
//! 
//! @param[in] threads        … スレッドオブジェクト
//! @param[in] num_threads    … スレッド数
//! @param[in] dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … 複数のスレッドの実行に成功
//! @retval false … 複数のスレッドの実行に失敗
//! 
template < class Thread >
inline bool do_threads_( Thread *threads, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	size_t i = 0;

	// スレッドの生成
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].create( ) )
		{
			ret = false;
		}
	}
	if( num_threads > 0 )
	{
		// 先頭のパラメータのみ実行スレッドで行う
		threads[ 0 ].create_without_thread( );
	}

	// スレッドの終了待ち
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].wait( dwMilliseconds ) )
		{
			ret = false;
		}
	}

	// リソースの開放
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].close( ) )
		{
			ret = false;
		}
	}

	return( ret );
}

/// @brief スレッド生成・終了待機・スレッドの破棄までを一連の流れとして行う
//! 
//! @param[in] threads        … スレッドオブジェクト
//! @param[in] num_threads    … スレッド数
//! @param[in] dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … 複数のスレッドの実行に成功
//! @retval false … 複数のスレッドの実行に失敗
//! 
template < class Thread >
inline bool do_threads_( Thread **threads, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	size_t i = 0;

	// スレッドの生成
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ]->create( ) )
		{
			ret = false;
		}
	}
	if( num_threads > 0 )
	{
		// 先頭のパラメータのみ実行スレッドで行う
		threads[ 0 ]->create_without_thread( );
	}

	// スレッドの終了待ち
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ]->wait( dwMilliseconds ) )
		{
			ret = false;
		}
	}

	// リソースの開放
	for( i = 1 ; i < num_threads ; i++ )
	{
		if( !threads[ i ]->close( ) )
		{
			ret = false;
		}
	}

	return( ret );
}



/// @brief スレッド生成・終了待機・スレッドの破棄までを一連の流れとして行う
//! 
//! @param[in] threads        … スレッドオブジェクト
//! @param[in] num_threads    … スレッド数
//! @param[in] dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … 複数のスレッドの実行に成功
//! @retval false … 複数のスレッドの実行に失敗
//! 
template < class Thread >
inline bool do_threads( Thread *threads, size_t num_threads, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	size_t i = 0;

	// スレッドの生成
	for( i = 0 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].create( ) )
		{
			ret = false;
		}
	}

	// スレッドの終了待ち
	for( i = 0 ; i < num_threads ; i++ )
	{
		if( !threads[ i ].wait( dwMilliseconds ) )
		{
			ret = false;
		}
	}

	// リソースの開放
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
//  スレッドグループの終わり

// スレッドを普通の関数形式の呼び出しで簡便に利用するための関数群
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
		// 継承した先で必ず実装されるスレッド関数
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
		// 継承した先で必ず実装されるスレッド関数
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
		// 継承した先で必ず実装されるスレッド関数
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
		// 継承した先で必ず実装されるスレッド関数
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
		// 継承した先で必ず実装されるスレッド関数
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
		// 継承した先で必ず実装されるスレッド関数
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
			// ロックした状態でスタートする
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
				// スレッドの開始まで待機する
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
			// 終了フラグを立ててスレッドを再起動する
			is_end_ = true;
			this->resume( );
			lock_.unlock( );

			// スレッドが正常終了するまで待つ
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
		// 継承した先で必ず実装されるスレッド関数
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
					// 処理待ちをする
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
					// キューの先頭からデータを取り出す
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


//! @addtogroup thread_group スレッド
//!
//!  @{

#if defined( __THREAD_POOL_SUPPORT__ ) && __THREAD_POOL_SUPPORT__ != 0
/// @brief スレッドプールを利用してスレッドの再利用を可能にするクラス
//! 
//! スレッドプールを利用して処理を実行した場合，処理用にスレッドプール内のスレッドを割り当てます．
//! 処理が終了した場合は，スレッドはスレッドプールに回収され，別の処理へ割り当てられます．
//! スレッドを再利用することにより，スレッドの生成破棄に必要なオーバーヘッドを削減します．
//! 
//! @attention 初回起動時はスレッド生成に伴うオーバーヘッドがあります
//! 
//! @code 使用例
//! struct parameter{ ... 何らかのパラメータ ... };   // スレッド関数に渡すパラメータ（特に構造体である必要は無い）
//! 
//! void thread_function( const parameter &p );       // スレッド関数（内部で何らかの処理をする）
//! 
//! // これ以降でスレッドを作成する
//! parameter param[ 処理を行う数 ];
//! 
//! for( i = 0 ; i < 処理を行う数 ; i++ )
//! {
//!     パラメータに何らかの値を設定する
//! }
//! 
//! // スレッドプール内のスレッド数を指定して初期化
//! mist::thread_pool pool( 2 );
//! 
//! // 関数をパラメータを変えて複数回処理する
//! pool.executes( thread_function, param, 処理を行う数 );
//! 
//! // スレッドプール内のスレッドが実行している処理が全て終了するまで待機
//! pool.wait( );
//! @endcode
//! 
class thread_pool
{
public:
	typedef size_t    size_type;		///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	typedef __thread_controller__::thread_pool_functor thread_pool_functor;
	typedef __thread_controller__::__thread_pool_functor__ __thread_pool_functor__;

	std::vector< thread_pool_functor * >   threads_;		///< @brief スレッドプール内のスレッドを管理する変数
	std::list< __thread_pool_functor__ * > functors_;		///< @brief スレッドプール内の処理待ちのジョブを管理するリスト
	simple_lock_object                     lock_;			///< @brief スレッドプール内のスレッド間で同期を取るためのロックオブジェクト
	bool                                   initialized_;	///< @brief スレッドプールの初期化が終了しているかどうか

public:
	/// @brief スレッドプール内のオブジェクトを作成するが，スレッドプール内のスレッドは0の状態で初期化する（initialize関数を呼び出す必要あり）
	thread_pool( ) : initialized_( false )
	{
	}

	/// @brief スレッドプール内のスレッド数を指定し，すぐにスレッドプールを使える状態で初期化する
	thread_pool( size_type number_of_max_threads ) : initialized_( false )
	{
		initialize( number_of_max_threads );
	}

	/// @brief スレッドプールで使用している全てのリソースを開放する
	virtual ~thread_pool( )
	{
		uninitialize( );
	}

	/// @brief スレッドプール内で使用するスレッドの数を指定してスレッドプールを初期化する
	//! 
	//! 既にスレッドプールを初期化している場合は，現在のスレッドが全て終了するまで待機する
	//! 
	//! @param[in] number_of_max_threads … スレッドプールにためておくスレッドの数
	//! 
	bool initialize( size_type number_of_max_threads )
	{
		// 現在実行中のスレッドを全て終了させる
		uninitialize( );

		if( number_of_max_threads == 0 )
		{
			// 1以上を指定する必要あり
			return( false );
		}

		threads_.resize( number_of_max_threads );

		// スレッドを実行してアイドル状態にする
		for( size_type i = 0 ; i < threads_.size( ) ; i++ )
		{
			threads_[ i ] = new thread_pool_functor( functors_, lock_, i, threads_.size( ) );
			threads_[ i ]->create( );
			//threads_[ i ]->wait( );
		}

		initialized_ = true;

		return( true );
	}

	/// @brief スレッドプールで使用しているリソースを全て開放する
	//! 
	//! 現在のスレッドが全て終了するまで待機し，スレッドプールで使用しているリソースを全て開放する
	//! 
	bool uninitialize( )
	{
		if( initialized_ )
		{
			for( size_type i = 0 ; i < threads_.size( ) ; i++ )
			{
				// スレッドのハンドルを閉じる
				threads_[ i ]->close( );

				// 使用していたメモリ領域を開放する
				delete threads_[ i ];
			}

			threads_.clear( );

			// キューに残っているデータを削除する
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] p … スレッドの関数に渡すパラメータ
	//! @param[in]     f … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] p1 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p2 … スレッドの関数に渡すパラメータ
	//! @param[in]     f  … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] p1 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p2 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p3 … スレッドの関数に渡すパラメータ
	//! @param[in]     f  … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] p1 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p2 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p3 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p4 … スレッドの関数に渡すパラメータ
	//! @param[in]     f  … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! 
	//! @param[in] f … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを複数指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] param       … スレッドの関数に渡すパラメータ
	//! @param[in]     num_threads … スレッド数
	//! @param[in]     f           … 実行されるスレッド関数
	//! 
	template < class Functor, class Param >
	bool executes( Functor f, Param *param, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// キューに追加する
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base< Param, Functor >( param[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief 関数とパラメータを複数指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] param1      … スレッドの関数に渡すパラメータ
	//! @param[in,out] param2      … スレッドの関数に渡すパラメータ
	//! @param[in]     num_threads … スレッド数
	//! @param[in]     f           … 実行されるスレッド関数
	//! 
	template < class Functor, class Param1, class Param2 >
	bool executes( Functor f, Param1 *param1, Param2 *param2, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// キューに追加する
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base2< Param1, Param2, Functor >( param1[ i ], param2[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief 関数とパラメータを複数指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] param1      … スレッドの関数に渡すパラメータ
	//! @param[in,out] param2      … スレッドの関数に渡すパラメータ
	//! @param[in,out] param3      … スレッドの関数に渡すパラメータ
	//! @param[in]     num_threads … スレッド数
	//! @param[in]     f           … 実行されるスレッド関数
	//! 
	template < class Functor, class Param1, class Param2, class Param3 >
	bool executes( Functor f, Param1 *param1, Param2 *param2, Param3 *param3, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// キューに追加する
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base3< Param1, Param2, Param3, Functor >( param1[ i ], param2[ i ], param3[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief 関数とパラメータを複数指定してスレッドを実行する
	//! 
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] param1      … スレッドの関数に渡すパラメータ
	//! @param[in,out] param2      … スレッドの関数に渡すパラメータ
	//! @param[in,out] param3      … スレッドの関数に渡すパラメータ
	//! @param[in,out] param4      … スレッドの関数に渡すパラメータ
	//! @param[in]     num_threads … スレッド数
	//! @param[in]     f           … 実行されるスレッド関数
	//! 
	template < class Functor, class Param1, class Param2, class Param3, class Param4 >
	bool executes( Functor f, Param1 *param1, Param2 *param2, Param3 *param3, Param4 *param4, size_t num_threads )
	{
		if( threads_.empty( ) || !initialized_ )
		{
			return( false );
		}

		// キューに追加する
		lock_.lock( );
		for( size_type i = 0 ; i < num_threads ; i++ )
		{
			functors_.push_back( new __thread_controller__::thread_pool_functor_base4< Param1, Param2, Param3, Param4, Functor >( param1[ i ], param2[ i ], param3[ i ], param4[ i ], f ) );
		}
		lock_.unlock( );

		resume_thread_from_queue( num_threads );

		return( true );
	}

	/// @brief 全てのスレッドが終了するか，タイムアウトになるまで待機する
	//! 
	//! タイムアウトを INFINITE に設定することで，スレッドが終了するまで待ち続ける．
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! 
	//! @param[in] dwMilliseconds … タイムアウト時間（ミリ秒単位）
	//! 
	//! @retval true  … スレッドがタイムアウト前に終了した
	//! @retval false … タイムアウトが発生したか，その他のエラーが発生
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
	/// @brief 関数をスレッドプール内のスレッドに割り当てて実行する
	//! 
	//! @param[in] func … 実行されるスレッド関数
	//! 
	bool exec( __thread_controller__::__thread_pool_functor__ *func )
	{
		// キューに追加する
		{
			// 排他制御する
			lock_.lock( );
			functors_.push_back( func );
			lock_.unlock( );
		}

		resume_thread_from_queue( 1 );

		return( true );
	}

	/// @brief スレッドプールから必要な数だけスレッドを再開する
	//! 
	//! @param[in] num_threads … リジュームするスレッド数
	//! 
	void resume_thread_from_queue( size_type num_threads )
	{
		size_type count = 0;
		for( size_type i = 0 ; i < threads_.size( ) ; i++ )
		{
			thread_pool_functor &t = *threads_[ i ];

			if( t.is_suspended( ) )
			{
				// アイドル状態のスレッドがあれば再開する
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



/// @brief バックグラウンドで実行するワーカースレッドを実現するクラス
//! 
//! ワーカースレッドには複数のジョブを設定することができ，各ジョブを順次実行します．
//! 全てのジョブが終了すると，ワーカースレッドは待機状態に入り，次のジョブを待ちます．
//! 
//! @attention 初回起動時はスレッド生成に伴うオーバーヘッドがあります
//! 
//! @code 使用例
//! struct parameter{ ... 何らかのパラメータ ... };   // スレッド関数に渡すパラメータ（特に構造体である必要は無い）
//! 
//! void thread_function( const parameter &p );       // スレッド関数（内部で何らかの処理をする）
//! 
//! // これ以降でスレッドを作成する
//! parameter param;
//! 
//! // ワーカースレッドを初期化
//! mist::worker_thread work;
//! 
//! // 関数を実行する
//! work.execute( thread_function, param );
//! 
//! // ワーカースレッドが実行している処理が全て終了するまで待機
//! work.wait( );
//! @endcode
//! 
class worker_thread
{
public:
	typedef size_t    size_type;		///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	__thread_controller__::thread_pool_functor                    *thread_;		///< @brief ワーカースレッドが管理するメインスレッド
	std::list< __thread_controller__::__thread_pool_functor__ * >  functors_;	///< @brief ワーカースレッド内の処理待ちのジョブを管理するリスト
	simple_lock_object                                             lock_;		///< @brief ワーカースレッド内のスレッド間で同期を取るためのロックオブジェクト

public:
	/// @brief ワーカースレッド用のスレッドを作成した状態で待機する
	worker_thread( ) : thread_( NULL )
	{
		thread_ = new __thread_controller__::thread_pool_functor( functors_, lock_, 0, 1 );
		thread_->create( );
		//thread_->wait( );
	}

	/// @brief ワーカースレッドで使用している全てのリソースを開放する
	virtual ~worker_thread( )
	{
		close( );
	}

	/// @brief ワーカースレッドがサスペンドしているかどうか
	bool is_suspended( )
	{
		if( thread_ == NULL )
		{
			return( true );
		}

		return( thread_->is_suspended( ) );
	}

	/// @brief ワーカースレッドで使用しているリソースを全て開放する
	//! 
	//! 直ちにワーカースレッドを終了し，ワーカースレッドで使用しているリソースを全て開放する
	//! 
	bool close( )
	{
		if( thread_ == NULL )
		{
			return( false );
		}

		// スレッドのハンドルを閉じる
		thread_->close( );

		// 使用していたメモリ領域を開放する
		delete thread_;
		thread_ = NULL;

		// キューに残っているデータを削除する
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! ワーカースレッドが終了した状態では false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] p … スレッドの関数に渡すパラメータ
	//! @param[in]     f … 実行されるスレッド関数
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


	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! ワーカースレッドが終了した状態では false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in] p1 … スレッドの関数に渡すパラメータ1
	//! @param[in] p2 … スレッドの関数に渡すパラメータ2
	//! @param[in] f  … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! ワーカースレッドが終了した状態では false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] p1 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p2 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p3 … スレッドの関数に渡すパラメータ
	//! @param[in]     f  … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! ワーカースレッドが終了した状態では false を返す．
	//! スレッドが実行する関数の引数はコピーして渡される．
	//! 
	//! @param[in,out] p1 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p2 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p3 … スレッドの関数に渡すパラメータ
	//! @param[in,out] p4 … スレッドの関数に渡すパラメータ
	//! @param[in]     f  … 実行されるスレッド関数
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

	/// @brief 関数とパラメータを指定してスレッドを実行する
	//! 
	//! ワーカースレッドが終了した状態では false を返す．
	//! 
	//! @param[in]     f     … 実行されるスレッド関数
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

	/// @brief 全ての処理が終了するか，タイムアウトになるまで待機する
	//! 
	//! タイムアウトを INFINITE に設定することで，スレッドが終了するまで待ち続ける．
	//! スレッドプールの初期化が終了していない場合は false を返す．
	//! 
	//! @param[in] dwMilliseconds … タイムアウト時間（ミリ秒単位）
	//! 
	//! @retval true  … スレッドがタイムアウト前に終了した
	//! @retval false … タイムアウトが発生したか，その他のエラーが発生
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
	/// @brief 関数をスレッドプール内のスレッドに割り当てて実行する
	//! 
	//! @param[in] func … 実行されるスレッド関数
	//! 
	bool exec( __thread_controller__::__thread_pool_functor__ *func )
	{
		// キューに追加する
		{
			// 排他制御する
			lock_.lock( );
			functors_.push_back( func );
			lock_.unlock( );
		}

		// スレッドを再開する
		thread_->resume( );


		return( true );
	}
};
#endif

/// @brief 生成したスレッドを管理するクラス
class thread_handle
{
private:
	thread_object *thread_;		///< @brief スレッドの実態

public:

	/// @brief スレッドを作成する
	bool create( ){ return( thread_ == NULL ? false : thread_->create( ) ); }


	/// @brief スレッドが終了するか，タイムアウトが発生するまで待機する
	//! 
	//! @param[in] dwMilliseconds … タイムアウト時間（ミリ秒単位）
	//! 
	bool wait( unsigned long dwMilliseconds = INFINITE ){ return( thread_ == NULL ? false : thread_->wait( dwMilliseconds ) ); }


	/// @brief スレッドが使用していたリソースを開放する
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

/// @brief 指定したスレッド関数を利用する，スレッドを作成する
//! 
//! @param[in,out] param … スレッドの関数に渡すパラメータ
//! @param[in]     f     … 実行されるスレッド関数
//!
//! @return スレッドを管理する thread_handle オブジェクト
//! 
//! @code 使用例
//! struct parameter{ ... 何らかのパラメータ ... };   // スレッド関数に渡すパラメータ（特に構造体である必要は無い）
//! 
//! void thread_function( const parameter &p );       // スレッド関数（内部で何らかの処理をする）
//! 
//! // これ以降でスレッドを作成する
//! parameter param[ 作成スレッド数 ];
//! mist::thread_handle t[ 作成スレッド数 ];
//! 
//! for( i = 0 ; i < 作成スレッド数 ; i++ )
//! {
//!     パラメータに何らかの値を設定する
//! }
//! 
//! for( i = 0 ; i < 作成スレッド数 ; i++ )
//! {
//!     // スレッドを作成する
//!     t[ i ] = mist::create_thread( param[ i ], thread_function );
//! }
//! 
//! for( i = 0 ; i < 作成スレッド数 ; i++ )
//! {
//!     // スレッドが終了するまで待ち続ける
//!     mist::wait_thread( t[ i ] );
//! }
//! 
//! for( i = 0 ; i < 作成スレッド数 ; i++ )
//! {
//!     // スレッドが使用していたリソースを開放する
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

/// @brief 指定したスレッド関数を利用する，スレッドを複数作成する
//! 
//! @param[out]    handles     … スレッドオブジェクト
//! @param[in,out] param       … スレッドの関数に渡すパラメータ
//! @param[in]     num_threads … スレッド数
//! @param[in]     f           … 実行されるスレッド関数
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


/// @brief スレッドが使用していたリソースを開放する
//! 
//! スレッドを使用した後で，必ず呼び出す必要がある
//! 
//! @param[in,out] thread_ … スレッドオブジェクト
//!
//! @retval true  … リソースの開放に成功
//! @retval false … リソースの開放に失敗
//! 
inline bool close_thread( thread_handle &thread_ ){ return( thread_.close( ) ); }

/// @brief 複数のスレッドが使用していたリソースを開放する
//! 
//! @param[out]    handles     … スレッドオブジェクト
//! @param[in]     num_threads … スレッド数
//!
//! @retval true  … 全てのリソースの開放に成功
//! @retval false … 全てのリソースの開放に失敗
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


/// @brief スレッドが終了するか，タイムアウトが発生するまで待機する
//! 
//! タイムアウトを INFINITE に設定することで，スレッドが終了するまで待ち続ける
//! 
//! @param[in,out] thread_        … スレッドオブジェクト
//! @param[in]     dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … スレッドがタイムアウト時間内に正常終了した場合
//! @retval false … スレッドがタイムアウト時間内に終了しなかった場合
//! 
inline bool wait_thread( thread_handle &thread_, unsigned long dwMilliseconds = INFINITE ){ return( thread_.wait( dwMilliseconds ) ); }

/// @brief 複数のスレッドが使用していたリソースを開放する
//! 
//! @param[out]    handles        … スレッドオブジェクト
//! @param[in]     num_threads    … スレッド数
//! @param[in]     dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … 複数のスレッドがタイムアウト時間内に正常終了した場合
//! @retval false … 複数のスレッドがタイムアウト時間内に終了しなかった場合
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


/// @brief スレッド生成・終了待機・スレッドの破棄までを一連の流れとして行う
//! 
//! @param[in,out] param          … スレッドの関数に渡すパラメータ
//! @param[in]     f              … 実行されるスレッド関数
//! @param[in]     dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … スレッドの実行に成功
//! @retval false … スレッドの実行に失敗
//! 
template < class Param, class Functor >
inline bool do_thread( Param &param, Functor f, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;

	// スレッドの生成
	thread_handle thread_ = create_thread( param, f );

	// スレッドの終了待ち
	if( !wait_thread( thread_, dwMilliseconds ) )
	{
		ret = false;
	}

	// リソースの開放
	if( !close_thread( thread_ ) )
	{
		ret = false;
	}
	return( ret );
}


/// @brief スレッド生成・終了待機・スレッドの破棄までを一連の流れとして行う
//! 
//! @param[in,out] params         … スレッドの関数に渡すパラメータ
//! @param[in]     num_threads    … スレッド数
//! @param[in]     f              … 実行されるスレッド関数
//! @param[in]     dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … 複数のスレッドの実行に成功
//! @retval false … 複数のスレッドの実行に失敗
//! 
template < class Param, class Functor >
inline bool do_threads( Param *params, size_t num_threads, Functor f, unsigned long dwMilliseconds = INFINITE )
{
	bool ret = true;
	thread_handle *threads_ = new thread_handle[ num_threads ];

	// スレッドの生成
	create_threads( threads_, params, num_threads, f );

	// スレッドの終了待ち
	if( !wait_threads( threads_, num_threads, dwMilliseconds ) )
	{
		ret = false;
	}

	// リソースの開放
	if( !close_threads( threads_, num_threads ) )
	{
		ret = false;
	}

	delete [] threads_;

	return( ret );
}


/// @}
//  スレッドグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_THREAD__
