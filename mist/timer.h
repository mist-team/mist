#ifndef __INCLUDE_MIST_TIMER__
#define __INCLUDE_MIST_TIMER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifdef WIN32

#include <windows.h>
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )

#else

//#include <time.h>
#include <sys/time.h>

#endif


// mist名前空間の始まり
_MIST_BEGIN


/// 時間計測を行うためのクラス
//! 
//! timerクラスを生成時から時間計測を開始する．
//! @par 複数箇所で時間計測を行う場合は，以下のように記述する
//! @code
//! {
//!     mist::timer t;
//! 
//!     ... 何らかの計算 ...
//! 
//!     std::cout << "計算時間: " << t << std::endl;
//! }
//! @endcode
//!
class timer
{
public:
	/// 時間計測を行うためのクラスのコンストラクタ．
	/// 生成時に，自動的にタイマを初期化する．
	timer( )
	{
#ifdef WIN32
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	} // postcondition: elapsed()==0

	/// 強制的に，タイマーをクリアする．
	void reset( )
	{
#ifdef WIN32
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	}

	/// 時間計測開始時からの経過時間をミリ秒単位で返す．
	//! @return ミリ秒単位での経過時間
	double elapse( ) const
	{
#ifdef WIN32
		return( static_cast< double >( timeGetTime( ) - _start_time ) / 1000.0 );
#else
		timeval  _end_time;
		gettimeofday( &_end_time, NULL );
		return( static_cast< double >( _end_time.tv_sec - _start_time.tv_sec ) + static_cast< double >( _end_time.tv_usec - _start_time.tv_usec ) / 1000000.0 );
#endif
	}

private:
#ifdef WIN32
	DWORD		_start_time;	/// 時間計測開始時刻を保持する変数（Windows用）
#else
	timeval		_start_time;	/// 時間計測開始時刻を保持する変数（Linux用）
#endif

};


/// 経過時間を標準出力に出力する．
inline ::std::ostream &operator <<( ::std::ostream &out, const timer &t )
{
	out << t.elapse( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
