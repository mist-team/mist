#include <iostream>
#include <string>
#include <mist/config/pointer.h>
#include <windows.h>
struct A
{
	std::string text;

	A( const std::string &t = "hoge" ) : text( t )
	{
		std::cout << "constructor" << std::endl;
	}

	~A( )
	{
		std::cout << "destructor" << std::endl;
	}
};

inline std::ostream &operator <<( std::ostream &out, const A &a )
{
	out << a.text;
	return( out );
}

int main( int argc, char *argv[] )
{
	using namespace std;

	{
		mist::scoped_ptr< A > a = new A( "test" );

		std::cout << *a << std::endl;
		std::cout << a->text << std::endl;
	}

	{
		mist::scoped_array< A > a = new A[ 10 ];

		std::cout << *a << std::endl;
		std::cout << a->text << std::endl;
	}

	mist::__mist_progress_callback__ callback;
	for( int i = 0 ; i <= 80 ; i++ )
	{
		callback( i );
		Sleep( 20 );
	}
		Sleep( 200 );
	for( int i = 80 ; i >= 20 ; i-- )
	{
		callback( i );
		Sleep( 20 );
	}
		Sleep( 200 );
	for( int i = 20 ; i <= 101 ; i++ )
	{
		callback( i );
		Sleep( 20 );
	}

	return( 0 );
}
