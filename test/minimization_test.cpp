#include <mist/minimization.h>
#include <iostream>
#include <string>

struct f1
{
	std::string str;
	int count;
	f1( ) : count( 0 )
	{
	}

	f1( const f1 &f ) : count( f.count )
	{
		std::cout << "copy constructor" << std::endl;
	}

	double operator ()( const mist::matrix< double > &v )
	{
//		std::cout << count++ << std::endl;
//		std::cout << count++ << "                                     \r";
		double x = v[ 0 ];
		double y = v[ 1 ];
		return( 4.0 * ( x - 1.0 ) * ( x - 1.0 ) + ( y - 2.0 ) * ( y - 2.0 ) + 100 );
	}
};

double f2( double x )
{
	const double pai = 3.1415926535897932384626433832795;
	return( ( x - 100 ) * ( x - 100 ) );
//	return( std::sin( x * pai / 180.0 ) );
//	return( std::sin( x ) + x / 5.0 );
}

int main( int argc, char *argv[] )
{
	using namespace std;

	{
		mist::matrix< double > p( 2, 1 );
		//p[ 0 ] = 0.1;
		//p[ 1 ] = 0.2;
		double err = mist::gradient::minimization( p, f1( ), 0.0 );
		cout << "f( " << p.t( ) << " ) = " << err << endl;
	}

	{
		mist::matrix< double > p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		//p[ 0 ] = 0.1;
		//p[ 1 ] = 0.2;
		double err = mist::powell::minimization( p, d, f1( ), 0.0 );
		cout << "f( " << p.t( ) << " ) = " << err << endl;
	}

	{
		double x;
		double err = mist::gold::minimization( -80, 20, x, f2, 0.00001, 100, true );
		cout << "f( " << x << " ) = " << err << endl;
	}
	{
		double x;
		double err = mist::brent::minimization( -10, 20, x, f2, 0.00001, 100, false );
		cout << "f( " << x << " ) = " << err << endl;
	}

	return( 0 );
}
