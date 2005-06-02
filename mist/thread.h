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
	virtual bool suspend( ) = 0;
	virtual bool resume( ) = 0;
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
	HANDLE    thread_handle_;			// Windows用のスレッドを識別するハンドル
	DWORD     thread_id_;				// Windows用のスレッドを識別するID
#else
	pthread_t thread_id_;				// pthreadライブラリでスレッドを識別するID
	bool      thread_finished_;			// スレッドが終了したかどうかを保持
#endif

	thread_exit_type thread_exit_code_;	// スレッドの戻り値

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
		thread_finished_ = t.thread_finished_;
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
		thread_handle_ = CreateThread( NULL, 0, map_thread_function, ( void * )this, 0, &( thread_id_ ) );
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
		BOOL ret = CloseHandle( thread_handle_ );
		thread_handle_ = NULL;
		return ( ret != 0 );
#else
		int ret = pthread_detach( thread_id_ );
		thread_id_ = ( pthread_t ) ( -1 );
		return ( ret == 0 );
#endif
	}


	/// @brief スレッドをサスペンドさせる
	//! 
	//! @attention Windows以外の環境では，pthread ライブラリ側でサポートされていないことがあるため，現在のところWindowsのみサポート
	//! 
	//! @retval true  … サスペンドに成功
	//! @retval false … サスペンドに失敗
	//! 
	virtual bool suspend( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので常に true を返す
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( SuspendThread( thread_handle_ ) != static_cast< DWORD >( -1 ) );
#else
//		return( pthread_suspend_np( thread_id_ ) == 0 );
		return( false );
#endif
	}


	/// @brief スレッドをリジュームする
	//! 
	//! @attention Windows以外の環境では，pthread ライブラリ側でサポートされていないことがあるため，現在のところWindowsのみのサポート
	//! 
	//! @retval true  … リジュームに成功
	//! @retval false … リジュームに失敗
	//! 
	virtual bool resume( )
	{
#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0
		// スレッドサポートはしないので常に true を返す
		return( true );
#elif defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		return( ResumeThread( thread_handle_ ) == 1 );
#else
//		return( pthread_resume_np( thread_id_ ) == 0 );
		return( false );
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
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			func_( param_ );
			return( 0 );
		}
	};
}


//! @addtogroup thread_group スレッド
//!
//!  @{



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
	bool wait( unsigned long dwMilliseconds = INFINITE ){ return( thread_ == NULL ? false : thread_->wait( INFINITE ) ); }


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


	/// @brief スレッドをサスペンドする
	bool suspend( ){ return( thread_ == NULL ? false : thread_->suspend( ) ); }


	/// @brief スレッドをリジュームする
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
		handles[ i ] = thread_handle( new __thread_controller__::thread_object_functor< Param, Functor >( param, f ) );
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
//! @retval true  … スレッドがタイムアンと時間内に正常終了した場合
//! @retval false … スレッドがタイムアンと時間内に終了しなかった場合
//! 
inline bool wait_thread( thread_handle &thread_, unsigned long dwMilliseconds = INFINITE ){ return( thread_.wait( dwMilliseconds ) ); }

/// @brief 複数のスレッドが使用していたリソースを開放する
//! 
//! @param[out]    handles        … スレッドオブジェクト
//! @param[in]     num_threads    … スレッド数
//! @param[in]     dwMilliseconds … タイムアウト時間（ミリ秒単位）
//!
//! @retval true  … 複数のスレッドがタイムアンと時間内に正常終了した場合
//! @retval false … 複数のスレッドがタイムアンと時間内に終了しなかった場合
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


/// @brief スレッドをサスペンドさせる
//! 
//! @attention Windows環境でのみサポート
//! 
//! @param[in,out] thread_ … スレッドオブジェクト
//!
//! @retval true  … サスペンドに成功
//! @retval false … サスペンドに失敗
//! 
inline bool suspend_thread( thread_handle &thread_ ){ return( thread_.suspend( ) ); }

/// @brief 複数のスレッドが使用していたリソースを開放する
//! 
//! @param[out]    handles     … スレッドオブジェクト
//! @param[in]     num_threads … スレッド数
//!
//! @retval true  … 複数のスレッドのサスペンドに成功
//! @retval false … 複数のスレッドのサスペンドに失敗
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


/// @brief スレッドをリジュームする
//! 
//! @attention Windows環境でのみサポート
//! 
//! @param[in,out] thread_ … スレッドオブジェクト
//!
//! @retval true  … リジュームに成功
//! @retval false … リジュームに失敗
//! 
inline bool resume_thread( thread_handle &thread_ ){ return( thread_.resume( ) ); }

/// @brief 複数のスレッドが使用していたリソースを開放する
//! 
//! @param[out]    handles     … スレッドオブジェクト
//! @param[in]     num_threads … スレッド数
//!
//! @retval true  … 複数のスレッドのリジュームに成功
//! @retval false … 複数のスレッドのリジュームに失敗
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

	return( ret );
}





/// @brief スレッドの排他制御をサポートするクラス
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

	typedef ::std::map< ::std::string, lock_object_type > lock_table;

public:
	lock_object( ) : lock_name_( "mist default lock object!!" ){ }
	lock_object( const std::string &name ) : lock_name_( name ){ }

#if !defined( _MIST_THREAD_SUPPORT_ ) || _MIST_THREAD_SUPPORT_ == 0

	// スレッドサポートはしないので特に必何もしない
	bool lock( ){ return( true ); }
	bool unlock( ){ return( true ); }

#else

	/// @brief 排他制御用のオブジェクトをロックする
	//! 
	//! @retval true  … ロックに成功
	//! @retval false … ロックオブジェクトの生成に失敗
	//! 
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


	/// @brief 排他制御用のオブジェクトをロックを解除する
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
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


/// @}
//  スレッドグループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_THREAD__
