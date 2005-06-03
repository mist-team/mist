#include <iostream>
#include <string>
#include <mist/config/pointer.h>

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
		mist::scoped_ptr< A > a( new A( "test" ) );

		std::cout << *a << std::endl;
		std::cout << a->text << std::endl;
	}

	std::cout << std::endl << std::endl;

	{
		mist::scoped_array< A > a( new A[ 2 ] );

		std::cout << *a << std::endl;
		std::cout << a->text << std::endl;
	}

	std::cout << std::endl << std::endl;


	mist::weak_ptr< A > w;
	std::cout << w << std::endl;
	{
		mist::shared_ptr< A > a = new A( "abcdefg" );

		w = a;
		std::cout << w << std::endl;

		std::cout << *a << std::endl;

		{
			mist::shared_ptr< A > b = a;

			std::cout << *a << std::endl;
		}

		std::cout << w << std::endl;
	}

	std::cout << w << std::endl;

	return( 0 );
}
