#ifndef __INCLUDE_MIST_SINGLETON__
#define __INCLUDE_MIST_SINGLETON__


#include "config/mist_conf.h"


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

template < class T >
class singleton
{
public:
	typedef T value_type;

	static value_type &get_singleton( )
	{
		static value_type singleton_;
		return( singleton_ );
	}

private:
	singleton( );
	singleton( const singleton &s );
	const singleton & operator =( const singleton &s );
};


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_SINGLETON__
