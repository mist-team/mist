#include <iostream>
#include <mist/mist.h>


int main( int argc, char *argv[] )
{
	using namespace std;

	typedef mist::array2< double >::size_type size_type;

	mist::array2< double > a( 5, 5 );
	a( 0, 0 ) = 0.1; a( 0, 1 ) = 0.2; a( 0, 2 ) = 0.1; a( 0, 3 ) = 0.6; a( 0, 4 ) = 0.4;
	a( 1, 0 ) = 0.3; a( 1, 1 ) = 4.1; a( 1, 2 ) = 5.2; a( 1, 3 ) = 4.7; a( 1, 4 ) = 0.3;
	a( 2, 0 ) = 0.2; a( 2, 1 ) = 3.5; a( 2, 2 ) = 2.8; a( 2, 3 ) = 3.9; a( 2, 4 ) = 0.8;
	a( 3, 0 ) = 0.4; a( 3, 1 ) = 4.2; a( 3, 2 ) = 5.9; a( 3, 3 ) = 4.2; a( 3, 4 ) = 0.9;
	a( 4, 0 ) = 0.2; a( 4, 1 ) = 0.3; a( 4, 2 ) = 0.1; a( 4, 3 ) = 0.1; a( 4, 4 ) = 0.4;

	mist::marray< mist::array2< int > > m( a, 2, 1 );

	for( size_type j = 0 ; j < m.height( ) ; j++ )
	{
		for( size_type i = 0 ; i < m.width( ) ; i++ )
		{
			m( i, j ) = static_cast< int >( i + j * m.width( ) );
		}
	}

	std::cout << a << std::endl;
	std::cout << m << std::endl;

	m = a;

	std::cout << m << std::endl;
}
