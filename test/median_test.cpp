#include <iostream>

#include <cmath>
#include <mist/mist.h>

#include <mist/filter/median.h>
#include <mist/timer.h>

#include <mist/io/image.h>


namespace orthodox
{
	template < class Array1, class Array2 >
	void median( const Array1 &in, Array2 &out, typename Array1::size_type fw, typename Array1::size_type fh, typename Array1::size_type fd )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array2::value_type out_value_type;

		size_type i, j, k, x, y, z, index;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		size_type bw = fw / 2;
		size_type bh = fh / 2;
		size_type bd = fd / 2;

		size_type size = fw * fh * fd;

		value_type *work  = new value_type[ size + 1 ];

		for( k = 0 ; k < d ; k++ )
		{
			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0; i < w ; i++ )
				{
					index = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							for( x = i < bw ? 0 : i - bw ; x <= i + bw && x < w ; x++ )
							{
								work[ index++ ] = in( x, y, z );
							}
						}
					}

					std::sort( work, work + index );
					out( i, j, k ) = static_cast< out_value_type >( work[ ( index - 1 ) / 2 ] );
				}
			}
		}

		delete [] work;
	}
}



int main( int argc, char *argv[] )
{
	using namespace std;

	if( argc != 3 )
	{
		return( 1 );
	}

	mist::array2< unsigned char > a;

	mist::read_image( a, argv[ 1 ] );

	mist::array2< unsigned char >::size_type i, loop = 10, radius = atoi( argv[ 2 ] );

	mist::array2< unsigned char > tmp1( a ), tmp2( a );

	{
		cout << "Sorting Median Filter" << endl;
		mist::timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			orthodox::median( a, tmp1, radius, radius, 1 );
		}
		cout << "Calculation Time: " << t.elapse( ) / static_cast< double >( loop ) << " (sec)" << endl;
	}

	{
		cout << "MIST Median Filter" << endl;
		mist::timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			mist::median( a, tmp2, radius, radius, 1 );
		}
		cout << "Calculation Time: " << t.elapse( ) / static_cast< double >( loop ) << " (sec)" << endl;
	}
	
	{
		double err = 0.0;
		for( i = 0 ; i < tmp1.size( ) ; i++ )
		{
			err += abs( static_cast< double >( tmp1[ i ] - tmp2[ i ] ) );
		}
		cout << "Difference: " << err << endl;
	}

	return( 0 );
}
