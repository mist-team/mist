#include <iostream>
#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/numeric.h>
#include <mist/random.h>
#include <mist/timer.h>

int main( )
{
	typedef mist::matrix< float > matrix;
	typedef mist::matrix< float >::size_type size_type;
	//typedef mist::matrix< std::complex< double > > matrix;
	matrix a( 200, 200 );

	mist::random r( 123 );      //—”¶¬Ší‚Ì\’z

	size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		a[ i ] = r.real3( );    //®”—”‚Ì¶¬
	}

	{
		std::cout << "Eigen value and vectors" << std::endl;
		mist::timer t;

		matrix eval, evec;
		mist::eigen( a, eval, evec, mist::matrix_style::ge );

		std::cout << "Computation Time: " << t << " sec" << std::endl;
	}

	return( 0 );
}

