#include <mist/singleton.h>
#include <iostream>

void foo( )
{
	int &s = mist::singleton< int >::get_instance( );
	s++;
	std::cout << s << std::endl;
}

void bar( )
{
	int &s = mist::singleton< int, 1 >::get_instance( );
	s++;
	std::cout << s << std::endl;
}

int main( int argc, char *argv[] )
{
	mist::singleton< int >::get_instance( ) = 0;
	mist::singleton< int, 1 >::get_instance( ) = 20;

	for( int i = 0 ; i < 10 ; i++ )
	{
		foo( );
		bar( );
	}
	return( 0 );
}
