#include <iostream>
#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/matrix_util.h>

int main( )
{
	typedef mist::matrix< double > matrix;
	//typedef mist::matrix< std::complex< double > > matrix;
	matrix a( 3, 3 ), b( 3, 1 );

	a( 0, 0 ) = 1.0; a( 0, 1 ) =  1.0; a( 0, 2 ) =  1.0;
	a( 1, 0 ) = 3.0; a( 1, 1 ) =  1.0; a( 1, 2 ) = -3.0;
	a( 2, 0 ) = 1.0; a( 2, 1 ) = -2.0; a( 2, 2 ) = -5.0;

	b[0] =  3.0;
	b[1] =  1.0;
	b[2] = -6.0;

	std::cout << "Original Matrix" << std::endl;
	std::cout << a << std::endl;
	std::cout << b << std::endl;

	{
		std::cout << "Solve equation" << std::endl;

		matrix aa = a, bb = b;

		mist::solve( aa, bb );

		std::cout << aa << std::endl;
		std::cout << bb << std::endl;
	}

	{
		std::cout << "Inverse Matrix Calculation" << std::endl;

		std::cout << inverse( matrix( a ) ) * b << std::endl;
	}

	{
		std::cout << "LU factorization" << std::endl;

		std::cout << lu_factorization( matrix( a ) ) << std::endl;
	}

	{
		std::cout << "QR factorization" << std::endl;

		std::cout << qr_factorization( matrix( a ) ) << std::endl;
	}

	{
		std::cout << "Eigen value and vectors" << std::endl;

		matrix aa = a, eval, evec;
		mist::eigen( aa, eval, evec );
		std::cout << aa << std::endl;
		std::cout << eval << std::endl;
		std::cout << evec << std::endl;
	}

	{
		std::cout << "Singular value decomposition (SVD)s" << std::endl;

		a.resize( 4, 4 );
		a( 0, 0 ) = 5.0; a( 0, 1 ) = 6.0; a( 0, 2 ) = 8.0; a( 0, 3 ) = 4.0;
		a( 1, 0 ) = 6.0; a( 1, 1 ) = 2.0; a( 1, 2 ) = 4.0; a( 1, 3 ) = 2.0;
		a( 2, 0 ) = 8.0; a( 2, 1 ) = 2.0; a( 2, 2 ) = 3.0; a( 2, 3 ) = 5.0;
		a( 3, 0 ) = 1.0; a( 3, 1 ) = 7.0; a( 3, 2 ) = 2.0; a( 3, 3 ) = 3.0;

		matrix aa = a, u, s, vt;

		mist::svd( aa, u, s, vt );
		std::cout << a << std::endl;
		std::cout << u << std::endl;
		std::cout << s << std::endl;
		std::cout << vt << std::endl;
		std::cout << u * s * vt << std::endl;
	}
	return( 0 );
}

