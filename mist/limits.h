#ifndef __INCLUDE_MIST_LIMITS__
#define __INCLUDE_MIST_LIMITS__

#include "config/mist_conf.h"
#include <limits.h>
#include <float.h>



// mist名前空間の始まり
_MIST_BEGIN


// numeric_limitsの特殊化用マクロ
#define _DEFINE_NUMERIC_LIMITS( _type_, _integer_, _signed_, _min_, _max_, _zero_ ) \
	template < >\
	struct numeric_limits< _type_ >\
	{\
		typedef _type_ value_type;\
		\
		_MIST_CONST( bool, is_integer, _integer_ );\
		_MIST_CONST( bool, is_signed,  _signed_ );\
		\
		static value_type min( ) { return(  _min_ ); }\
		static value_type max( ) { return(  _max_ ); }\
		static value_type zero( ){ return( _zero_ ); }\
	};\


template < class T >
struct numeric_limits
{
	typedef T value_type;

	_MIST_CONST( bool, is_integer, false );
	_MIST_CONST( bool, is_signed,  false );

	static value_type min( ) { return( 0 ); }
	static value_type max( ) { return( 1 ); }
	static value_type zero( ){ return( 0 ); }
};


_DEFINE_NUMERIC_LIMITS(          char,  true,  true,  CHAR_MIN,  CHAR_MAX, 0x00 )
_DEFINE_NUMERIC_LIMITS( signed   char,  true,  true, SCHAR_MIN, SCHAR_MAX, 0x00 )
_DEFINE_NUMERIC_LIMITS( unsigned char,  true, false,      0x00, UCHAR_MAX, 0x00 )

_DEFINE_NUMERIC_LIMITS( signed   short, true,  true,  SHRT_MIN,  SHRT_MAX, 0x0000 )
_DEFINE_NUMERIC_LIMITS( unsigned short, true, false,    0x0000, USHRT_MAX, 0x0000 )

_DEFINE_NUMERIC_LIMITS( signed   int,   true,  true,   INT_MIN,   INT_MAX, 0 )
_DEFINE_NUMERIC_LIMITS( unsigned int,   true, false,         0,  UINT_MAX, 0 )

_DEFINE_NUMERIC_LIMITS( signed   long,  true,  true,  LONG_MIN,  LONG_MAX, 0 )
_DEFINE_NUMERIC_LIMITS( unsigned long,  true, false,         0, ULONG_MAX, 0 )

_DEFINE_NUMERIC_LIMITS(       float,   false,  true,   FLT_MIN,   FLT_MAX, 0.0f )
_DEFINE_NUMERIC_LIMITS(      double,   false, false,   DBL_MIN,   DBL_MAX, 0 )
_DEFINE_NUMERIC_LIMITS( long double,   false, false,  LDBL_MIN,  LDBL_MAX, 0 )


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_LIMITS__