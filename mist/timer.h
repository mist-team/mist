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


class timer
{
public:
	timer( )
	{
#ifdef WIN32
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	} // postcondition: elapsed()==0

	void reset( )
	{
#ifdef WIN32
		_start_time = timeGetTime( );
#else
		gettimeofday( &_start_time, NULL );
#endif
	}

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
	DWORD _start_time;
#else
	timeval  _start_time;
#endif

};


inline ::std::ostream &operator <<( ::std::ostream &out, const timer &t )
{
	out << t.elapse( );
	return( out );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIMER__
