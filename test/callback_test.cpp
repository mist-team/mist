#include <iostream>
#include <string>
#include <mist/config/pointer.h>
#include <mist/thread.h>

template < class Functor >
inline void callback( Functor f )
{
	for( int i = 0 ; i <= 80 ; i++ )
	{
		f( i );
		mist::sleep( 20 );
	}

	mist::sleep( 20 );

	for( int i = 80 ; i >= 20 ; i-- )
	{
		f( i );
		mist::sleep( 20 );
	}

	mist::sleep( 20 );

	for( int i = 20 ; i <= 101 ; i++ )
	{
		f( i );
		mist::sleep( 20 );
	}
}

int main( int argc, char *argv[] )
{
	using namespace std;

	callback( mist::__mist_console_callback__( ) );

	std::cout << std::endl;
	std::cout << std::endl;

	callback( mist::__mist_progress_callback__( ) );

	return( 0 );
}
