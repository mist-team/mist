#ifndef __INCLUDE_MIST_TIMER__
#define __INCLUDE_MIST_TIMER__


#include "config/mist_conf.h"


#ifdef WIN32

#include <windows.h>
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )

#else

#include <ctime>

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
		_start_time = std::clock( );
#endif
	} // postcondition: elapsed()==0

	void reset( )
	{
#ifdef WIN32
		_start_time = timeGetTime( );
#else
		_start_time = std::clock( );
#endif
	}

	double elapse( ) const
	{
#ifdef WIN32
		return( double(timeGetTime( ) - _start_time) / 1000.0 );
#else
		return( double(std::clock( ) - _start_time) / CLOCKS_PER_SEC );
#endif
	}

private:
#ifdef WIN32
	DWORD _start_time;
#else
	std::clock_t _start_time;
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