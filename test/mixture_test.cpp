#include <iostream>
#include <ctime>

#include <mist/random.h>
#include <mist/mixture.h>


int main( int argc, char *argv[] )
{
	using namespace std;
	using namespace mist;


	{
		array< double > data( 20000 );

		//gauss::random rnd1( std::clock( ), 100, 30.0 );
		//gauss::random rnd2( std::clock( ), 50, 5.0 );
		gauss::random rnd1( 1, 100, 30.0 );
		gauss::random rnd2( 2, 50, 5.0 );

		size_t i;
		for( i = 0 ; i < data.size( ) / 2 ; i++ )
		{
			data[ i ] = rnd1.generate( );
		}

		for( ; i < data.size( ) ; i++ )
		{
			data[ i ] = rnd2.generate( );
		}

		mixture::distribution dp[ 2 ];
		dp[ 0 ].av = 135.0;
		dp[ 0 ].sd = 50.0;

		dp[ 1 ].av = 22.0;
		dp[ 1 ].sd = 50.0;

		estimate_mixture( data, dp, 2, 100, 0.0001 );

		std::cout << dp[ 0 ] << std::endl;
		std::cout << dp[ 1 ] << std::endl;
	}

	{
		array< vector2< double > > data( 20000 );

		gauss::random rnd1( std::clock( ), 100, 30.0 );
		gauss::random rnd2( std::clock( ), 50, 5.0 );

		gauss::random rnd3( std::clock( ), 20, 25.0 );
		gauss::random rnd4( std::clock( ), 80, 13.0 );

		size_t i;
		for( i = 0 ; i < data.size( ) / 2 ; i++ )
		{
			data[ i ].x = rnd1.generate( );
			data[ i ].y = rnd2.generate( );
		}

		for( ; i < data.size( ) ; i++ )
		{
			data[ i ].x = rnd3.generate( );
			data[ i ].y = rnd4.generate( );
		}

		mixture::distribution2 dp[ 2 ];
		dp[ 0 ].av.x = 135.0;
		dp[ 0 ].av.y = 25.0;
		dp[ 0 ].v[ 0 ] = 50.0;
		dp[ 0 ].v[ 3 ] = 30.0;

		dp[ 1 ].av.x = 22.0;
		dp[ 1 ].av.y = 78.0;
		dp[ 1 ].v[ 0 ] = 30.0;
		dp[ 1 ].v[ 3 ] = 50.0;

		estimate_mixture( data, dp, 2, 100, 0.0001 );

		std::cout << dp[ 0 ] << std::endl;
		std::cout << dp[ 1 ] << std::endl;
	}

	return( 0 );
}
