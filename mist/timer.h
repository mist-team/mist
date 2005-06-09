/// @file mist/timer.h
//!
//! @brief 経過時間を計算するクラス
//!
#ifndef __INCLUDE_MIST_TIMER__
#define __INCLUDE_MIST_TIMER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0

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
//! 
//! @par 複数箇所で時間計測を行う場合は，以下のように記述する
//! @code タイマーオブジェクトの利用方法
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
private:
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
	union timeval
	{
		LARGE_INTEGER largeint;
		unsigned long long int uint64;
	};

	// Windows の場合で，高分解能カウンタが利用可能なら利用する
	static timeval __QueryPerformanceFrequency__( )
	{
		timeval dmy;
		QueryPerformanceFrequency( &( dmy.largeint ) );
		return( dmy );
	}

	static timeval _QueryPerformanceFrequency_( )
	{
		static timeval dmy = __QueryPerformanceFrequency__( );
		return( dmy );
	}

	static double _timeGetTime_( )
	{
		timeval dmy;
		if( QueryPerformanceCounter( &( dmy.largeint ) ) )
		{
			return( static_cast< double >( dmy.uint64 ) / static_cast< double >( _QueryPerformanceFrequency_( ).uint64 ) );
		}
		else
		{
			return( 1 );
			//return( static_cast< double >( timeGetTime( ) ) / 1000.0 );
		}
	}
#else
	static double _timeGetTime_( )
	{
		timeval dmy;
		gettimeofday( &dmy, NULL );
		return( static_cast< double >( dmy.tv_sec ) + static_cast< double >( dmy.tv_usec ) / 1000000.0 );
	}
#endif

public:
	/// @brief 時間計測を行うためのクラスのコンストラクタ．
	//! 
	//! 生成時に，自動的にタイマを初期化する．
	//! 
	timer( )
	{
		_start_time = _timeGetTime_( );
	}

	/// @brief 強制的に，タイマーをクリアする．
	void reset( )
	{
		_start_time = _timeGetTime_( );
	}

	/// @brief 時間計測開始時からの経過時間をミリ秒単位で返す．
	//! 
	//! @return ミリ秒単位での経過時間
	//! 
	double elapse( ) const
	{
		return( _timeGetTime_( ) - _start_time );
	}

private:
	double	_start_time;	///< @brief 時間計測開始時刻を保持する変数
};


/// @brief 経過時間を標準出力に出力する．
//! 
//! @param[in,out] out … 引数の説明
//! @param[in]     t   … 引数の説明
//! 
//! @return 戻り値の説明
//! 
inline ::std::ostream &operator <<( ::std::ostream &out, const timer &t )
{
	out << t.elapse( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
