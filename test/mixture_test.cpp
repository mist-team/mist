#include <iostream>
#include <ctime>

#include <mist/random.h>
#include <mist/mixture.h>
#include <mist/timer.h>
#include <mist/statistics.h>

int main( int argc, char *argv[] )
{
	using namespace std;
	using namespace mist;


	{
//		array< int > data( 20000 );
		std::vector< int > data( 20000 );

//		array< int > hist;
		std::vector< int > hist( 20000 );

		//gauss::random rnd1( std::clock( ), 100, 30.0 );
		//gauss::random rnd2( std::clock( ), 50, 5.0 );
		gauss::random rnd1( 1, 120, 30.0 );
		gauss::random rnd2( 2, 60, 10.0 );

		int minimum = 0;
		int maximum = 200;

		size_t i;
		for( i = 0 ; i < data.size( ) / 3 ; i++ )
		{
			double v = rnd1.generate( );
			while( v < minimum || maximum < v )
			{
				v = rnd1.generate( );
			}
			data[ i ] = static_cast< int >( v );
		}

		for( ; i < data.size( ) ; i++ )
		{
			double v = rnd2.generate( );
			while( v < minimum || maximum < v )
			{
				v = rnd2.generate( );
			}
			data[ i ] = static_cast< int >( v );
		}

		// ヒストグラムを作成
		generate_histogram( data, hist, minimum, maximum, 3 );

		mixture::distribution dp[ 2 ];

		{
			dp[ 0 ].av = 135.0;
			dp[ 0 ].sd = 50.0;

			dp[ 1 ].av = 22.0;
			dp[ 1 ].sd = 50.0;

			mist::timer t;
			if( !estimate_mixture( data, dp, 2, 100, 0.001 ) )
			{
				std::cout << "Estimation is failed during EM algorithm." << std::endl;
			}
			std::cout << "Computation time (Original Data): " << t << " sec" << std::endl;

			std::cout << dp[ 0 ] << std::endl;
			std::cout << dp[ 1 ] << std::endl << std::endl;
		}

		{
			dp[ 0 ].av = 135.0;
			dp[ 0 ].sd = 50.0;

			dp[ 1 ].av = 22.0;
			dp[ 1 ].sd = 50.0;

			mist::timer t;
			if( !histogram::estimate_mixture( hist, dp, 2, 0, 3, 100, 0.001 ) )
			{
				std::cout << "Estimation is failed during EM algorithm." << std::endl;
			}
			std::cout << "Computation time (Histogram): " << t << " sec" << std::endl;

			std::cout << dp[ 0 ] << std::endl;
			std::cout << dp[ 1 ] << std::endl << std::endl;
		}
	}


	{
		array< vector2< int > > data( 200000 );
		array< int > data1( 200000 );
		array< int > data2( 200000 );
		array2< int > hist;

		gauss::random rnd1( clock( ), 100, 30.0 );
		gauss::random rnd2( clock( ), 20, 5.0 );

		gauss::random rnd3( clock( ), 20, 25.0 );
		gauss::random rnd4( clock( ), 80, 13.0 );

		int minimum = 0;
		int maximum = 200;

		size_t i;
		double v;
		for( i = 0 ; i < data.size( ) / 3 ; i++ )
		{
			v = rnd1.generate( );
			while( v < minimum || maximum < v )
			{
				v = rnd1.generate( );
			}
			data[ i ].x = data1[ i ] = static_cast< int >( v );

			v = rnd2.generate( );
			while( v < minimum || maximum < v )
			{
				v = rnd2.generate( );
			}
			data[ i ].y = data2[ i ] =  static_cast< int >( v );
		}

		for( ; i < data.size( ) ; i++ )
		{
			v = rnd3.generate( );
			while( v < minimum || maximum < v )
			{
				v = rnd3.generate( );
			}
			data[ i ].x = data1[ i ] = static_cast< int >( v );

			v = rnd4.generate( );
			while( v < minimum || maximum < v )
			{
				v = rnd4.generate( );
			}
			data[ i ].y = data2[ i ] = static_cast< int >( v );
		}

		// ヒストグラムを作成
		generate_histogram( data1, data2, hist, minimum, maximum, minimum, maximum, 3 );

		mixture::distribution2 dp[ 2 ];

		{
			dp[ 0 ].av.x = 135.0;
			//		dp[ 0 ].av.y = 125.0;
			dp[ 0 ].av.y = 25.0;
			dp[ 0 ].v[ 0 ] = 50.0;
			dp[ 0 ].v[ 3 ] = 20.0;

			dp[ 1 ].av.x = 22.0;
			//		dp[ 1 ].av.y = 18.0;
			dp[ 1 ].av.y = 78.0;
			dp[ 1 ].v[ 0 ] = 30.0;
			dp[ 1 ].v[ 3 ] = 10.0;

			mist::timer t;
			if( !estimate_mixture( data, dp, 2, 100, 0.0001 ) )
			{
				std::cout << "Estimation is failed during EM algorithm." << std::endl;
			}
			std::cout << "Computation time (Original Data): " << t << " sec" << std::endl;

			std::cout << dp[ 0 ] << std::endl;
			std::cout << dp[ 1 ] << std::endl << std::endl;
		}

		{
			dp[ 0 ].av.x = 135.0;
			//		dp[ 0 ].av.y = 125.0;
			dp[ 0 ].av.y = 25.0;
			dp[ 0 ].v[ 0 ] = 50.0;
			dp[ 0 ].v[ 3 ] = 20.0;

			dp[ 1 ].av.x = 22.0;
			//		dp[ 1 ].av.y = 18.0;
			dp[ 1 ].av.y = 78.0;
			dp[ 1 ].v[ 0 ] = 30.0;
			dp[ 1 ].v[ 3 ] = 10.0;

			mist::timer t;
			if( !histogram::estimate_mixture( hist, dp, 2, 0, 0, 3, 100, 0.0001 ) )
			{
				std::cout << "Estimation is failed during EM algorithm." << std::endl;
			}
			std::cout << "Computation time (Histogram): " << t << " sec" << std::endl;

			std::cout << dp[ 0 ] << std::endl;
			std::cout << dp[ 1 ] << std::endl << std::endl;
		}
	}

	return( 0 );
}
