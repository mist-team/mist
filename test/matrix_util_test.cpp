#include <iostream>
#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/numeric.h>

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
	std::cout << a << std::endl << std::endl;
	std::cout << b << std::endl << std::endl;

	std::cout << matrix::identity( 3, 3 ) * 3 + a * matrix::zero( 3, 3 ) << std::endl << std::endl;

	{
		std::cout << "Solve equation" << std::endl;

		matrix aa = a, bb = b;

		mist::solve( aa, bb );

		std::cout << aa << std::endl << std::endl;
		std::cout << bb << std::endl << std::endl;
	}

	{
		std::cout << "Inverse Matrix Calculation" << std::endl;

		matrix aa = a;
		std::cout << mist::inverse( aa ) * b << std::endl << std::endl;
	}

	{
		std::cout << "Inverse Matrix Calculation Symmetry" << std::endl;

		matrix aa( 2, 2 );
		aa( 0, 0 ) = 1.0; aa( 0, 1 ) = 2.0;
		aa( 1, 0 ) = 2.0; aa( 1, 1 ) = 3.0;
		matrix aaa = aa;
		std::cout << aa * mist::inverse( aaa, mist::matrix_style::sy ) << std::endl << std::endl;
	}

	{
		std::cout << "Inverse Matrix Calculation Symmetry" << std::endl;

		matrix aa( 2, 2 );
		aa( 0, 0 ) = 1.0; aa( 0, 1 ) = 2.0;
		aa( 1, 0 ) = 2.0; aa( 1, 1 ) = 3.0;
		matrix aaa = aa;
		std::cout << aa * mist::inverse( aaa, mist::matrix_style::ge ) << std::endl << std::endl;
	}

	{
		std::cout << "LU factorization" << std::endl;

		matrix aa = a;
		std::cout << mist::lu_factorization( aa ) << std::endl << std::endl;
	}

	{
		std::cout << "QR factorization" << std::endl;

		matrix aa = a;
		std::cout << mist::qr_factorization( aa ) << std::endl << std::endl;
	}

	{
		std::cout << "Eigen value and vectors" << std::endl << std::endl;

		matrix aa = a, eval, evec;
		mist::eigen( aa, eval, evec, mist::matrix_style::ge );
		std::cout << aa << std::endl << std::endl;
		std::cout << eval << std::endl << std::endl;
		std::cout << evec << std::endl << std::endl;
	}

	{
		std::cout << "Eigen value and vectors symmetry" << std::endl;

		matrix aa = a * a.t( ), eval, evec;
		std::cout << aa << std::endl << std::endl;
		mist::eigen( aa, eval, evec, mist::matrix_style::sy );
		std::cout << eval << std::endl << std::endl;
		std::cout << evec << std::endl << std::endl;
	}

	{
		std::cout << "Determinant" << std::endl;

		std::cout << mist::det( a ) << std::endl << std::endl;
		std::cout << mist::det( a * a.t( ) ) << std::endl << std::endl;
	}

	{
		std::cout << "Trace" << std::endl;

		std::cout << mist::trace( a ) << std::endl << std::endl;
		std::cout << mist::trace( a * a.t( ) ) << std::endl << std::endl;
	}

	{
		std::cout << "Singular value decomposition (SVD)s" << std::endl;

		a.resize( 4, 4 );
		a( 0, 0 ) = 5.0; a( 0, 1 ) = 6.0; a( 0, 2 ) = 8.0; a( 0, 3 ) = 4.0;
		a( 1, 0 ) = 6.0; a( 1, 1 ) = 2.0; a( 1, 2 ) = 4.0; a( 1, 3 ) = 2.0;
		a( 2, 0 ) = 8.0; a( 2, 1 ) = 2.0; a( 2, 2 ) = 3.0; a( 2, 3 ) = 5.0;
		a( 3, 0 ) = 1.0; a( 3, 1 ) = 7.0; a( 3, 2 ) = 2.0; a( 3, 3 ) = 3.0;

		matrix aa = a, u, s, vt;

		std::cout << a << std::endl << std::endl;

		mist::svd( aa, u, s, vt );
		std::cout << u << std::endl << std::endl;
		std::cout << s << std::endl << std::endl;
		std::cout << vt << std::endl << std::endl;
		std::cout << u * s * vt << std::endl << std::endl;
	}
	return( 0 );
}

