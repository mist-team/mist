#include <iostream>
#include <ctime>

#include <mist/mist.h>
#include <mist/random.h>
#include <mist/statistics.h>


int main( int argc, char *argv[] )
{
	typedef mist::array< double >::size_type size_type;

	size_type num = 10000, i;

	mist::array< double > data( num );
	mist::uniform::random rnd( std::clock( ) );

	for( i = 0 ; i < num ; i++ )
	{
		data[ i ] = rnd.real1( );
	}

	std::cout << "Average: " << mist::average( data ) << std::endl;
	std::cout << "SD:      " << std::sqrt( mist::variance( data ) ) << std::endl;

	return( 0 );
}
