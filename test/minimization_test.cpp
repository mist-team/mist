#include <mist/minimization.h>
#include <iostream>
#include <string>

struct f1
{
	std::string str;
	f1( const std::string &s ) : str( s )
	{
	}

	f1( const f1 &f ) : str( f.str )
	{
		std::cout << "ƒRƒs[‚ª”­¶‚µ‚Ü‚µ‚½" << std::endl;
	}

	double operator ()( const mist::matrix< double > &v )
	{
		//std::cout << str << std::endl;
		double x = v[ 0 ];
		double y = v[ 1 ];
		return( 4.0 * ( x - 1.0 ) * ( x - 1.0 ) + ( y - 2.0 ) * ( y - 2.0 ) + 100 );
	}
};

double f2( double x )
{
	return( std::sin( x ) + x / 5.0 );
}

int main( int argc, char *argv[] )
{
	using namespace std;

	{
		mist::matrix< double > p( 2, 1 );
		//p[ 0 ] = 0.1;
		//p[ 1 ] = 0.2;
		double err = mist::gradient::minimization( p, f1( "‚ ‚¢‚¤‚¦‚¨" ), 0.0 );
		cout << "f( " << p.t( ) << " ) = " << err << endl;
	}

	{
		mist::matrix< double > p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		//p[ 0 ] = 0.1;
		//p[ 1 ] = 0.2;
		double err = mist::powell::minimization( p, d, f1( "‚ ‚¢‚¤‚¦‚¨" ), 0.0 );
		cout << "f( " << p.t( ) << " ) = " << err << endl;
	}

	{
		double x;
		double err = mist::gold::minimization( 0, 1, x, f2, 0.00001 );
		cout << "f( " << x << " ) = " << err << endl;
	}
	{
		double x;
		double err = mist::brent::minimization( 0, 1, x, f2, 0.00001 );
		cout << "f( " << x << " ) = " << err << endl;
	}

	return( 0 );
}
