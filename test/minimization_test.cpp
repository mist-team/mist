#include <mist/minimization.h>
#include <iostream>


double f1( const mist::matrix< double > &v )
{
	double x = v[ 0 ];
	double y = v[ 1 ];
	return( 4.0 * ( x - 1.0 ) * ( x - 1.0 ) + ( y - 2.0 ) * ( y - 2.0 ) );
} // �S�i��1�|�P�j2�{�i��2�|�Q�j2 

double f2( double x )
{
	return( std::sin( x ) + x / 5.0 );
}

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::matrix< double > p( 2, 1 );
	//p[ 0 ] = 0.1;
	//p[ 1 ] = 0.2;
	double err = mist::gradient::minimization( p, f1, 0.0 );
	cout << "f( " << p.t( ) << " ) = " << err << endl;

	double x;
	err = mist::gold::minimization( 0, 1, x, f2, 0.00001 );
	cout << "f( " << x << " ) = " << err << endl;
	err = mist::brent::minimization( 0, 1, x, f2, 0.00001 );
	cout << "f( " << x << " ) = " << err << endl;


	return( 0 );
}
