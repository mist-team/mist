#include <mist/minimization.h>
#include <iostream>
#include <string>

struct f1
{
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
		count++;
		return( 4.0 * ( x - 1.0 ) * ( x - 1.0 ) * ( x - 1.0 ) * ( x - 1.0 ) + ( y - 2.0 ) * ( y - 2.0 ) + 100 );
	}
};

struct f2
{
	int count;
	f2( ) : count( 0 )
	{
	}

	f2( const f2 &f ) : count( f.count )
	{
		std::cout << "copy constructor" << std::endl;
	}

	double operator ()( double x )
	{
		count++;
		const double pai = 3.1415926535897932384626433832795;
		//	return( ( x - 100 ) * ( x - 100 ) );
		return( std::sin( x * pai / 180.0 ) );
		//	return( std::sin( x ) + x / 5.0 );
	}
};


int main( int argc, char *argv[] )
{
	using namespace std;

	typedef mist::matrix< double > matrix_type;

	double ftol = 1.0e-3;

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 );
		f1 func;
		p[ 0 ] = 100;
		p[ 1 ] = 200;
		double err = mist::gradient::minimization( p, functor_reference( func ), ftol );
		cout << "Gradient descent" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		f1 func;
		p[ 0 ] = 100;
		p[ 1 ] = 200;
		double err = mist::powell::minimization( p, d, functor_reference( func ), ftol );
		cout << "Powell's method" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		f1 func;
		p[ 0 ] = 100;
		p[ 1 ] = 200;
		double err = mist::lucidi::minimization( p, d, functor_reference( func ), ftol );
		cout << "Lucidi's method" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f2 > functor_reference;
		double x;
		f2 func;
		double err = mist::gold::minimization( -90, 270, x, functor_reference( func ), 0.00001, 100, false );
		cout << "Golden-ratio divide method" << endl;
		cout << "f( " << x << " ) = " << err << " , count= " << func.count << endl << endl;
	}
	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f2 > functor_reference;
		double x;
		f2 func;
		double err = mist::brent::minimization( -90, 270, x, functor_reference( func ), 0.00001, 100, false );
		cout << "Brent's method" << endl;
		cout << "f( " << x << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		matrix_type bound( 2, 2 );
		f1 func;
		bound( 0, 0 ) = -10;
		bound( 0, 1 ) = 20;
		bound( 1, 0 ) = 3;
		bound( 1, 1 ) = 5;
		p[ 0 ] = -5;
		p[ 1 ] = 4;
		double err = mist::gradient::minimization( p, bound, functor_reference( func ), ftol );
		cout << "Gradient descent [constrained]" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		matrix_type bound( 2, 2 );
		f1 func;
		bound( 0, 0 ) = -10;
		bound( 0, 1 ) = 20;
		bound( 1, 0 ) = 3;
		bound( 1, 1 ) = 5;
		p[ 0 ] = -5;
		p[ 1 ] = 4;
		double err = mist::powell::minimization( p, d, bound, functor_reference( func ), ftol );
		cout << "Powell's method [constrained]" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	return( 0 );
}
