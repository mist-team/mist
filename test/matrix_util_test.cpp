// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

#include <iostream>
#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/numeric.h>


int main( )
{
	typedef mist::matrix< double > matrix;
	typedef mist::matrix< double >::size_type size_type;
	//typedef mist::matrix< std::complex<float> > matrix;
	//typedef mist::matrix< std::complex< double > > matrix;
	matrix a = matrix::_33(	1.0,  3.0,  1.0,
							3.0,  1.0, -2.0,
							1.0, -2.0, -5.0 );

	matrix A( 3, 4 );

	matrix b( 3, 2 );

	A( 0, 0 ) = 1; A( 0, 1 ) = 3; A( 0, 2 ) = 1; A( 0, 3 ) = 4;
	A( 1, 0 ) = 2; A( 1, 1 ) = 8; A( 1, 2 ) = 2; A( 1, 3 ) = 1;
	A( 2, 0 ) = 7; A( 2, 1 ) = 4; A( 2, 2 ) = 3; A( 2, 3 ) = 5;

	std::cout << A * mist::inverse( A ) << std::endl;

	b( 0, 0 ) =  3.0;
	b( 1, 0 ) =  1.0;
	b( 2, 0 ) = -6.0;
	b( 0, 1 ) =  3.0;
	b( 1, 1 ) =  1.0;
	b( 2, 1 ) = -6.0;

	a = a + 1;

	std::cout << matrix::diag( 2.0, 1.0, 4.0 ) << std::endl;

	std::cout << "Original Matrix" << std::endl;
	std::cout << a << std::endl << std::endl;
	std::cout << b << std::endl << std::endl;

	{
		matrix c;
		mist::multiply( a, a, c, false, false );
		std::cout << c << std::endl << std::endl;
	}

	{
		matrix L, U, pivot;
		mist::lu_factorization( a, L, U, pivot, mist::matrix_style::sy );

		std::cout << L << std::endl << std::endl;
		std::cout << U << std::endl << std::endl;
		std::cout << pivot << std::endl << std::endl;
		std::cout << pivot * L * U << std::endl << std::endl;
	}

	{
		matrix c( a );
		c.trim( 1, 1, 2, 2 );
		std::cout << "triming test" << std::endl;
		std::cout << c << std::endl << std::endl;
	}

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

		matrix aaa = a;
		std::cout << mist::inverse( aaa, mist::matrix_style::sy ) * a << std::endl << std::endl;
		std::cout << mist::inverse( aaa, mist::matrix_style::ge ) * a << std::endl << std::endl;
		std::cout << mist::inverse( aaa, mist::matrix_style::sy ) << std::endl << std::endl;
		std::cout << mist::inverse( aaa, mist::matrix_style::ge ) << std::endl << std::endl;
	}

	{
		std::cout << "LU factorization" << std::endl;

		matrix aa = a;
		std::cout << mist::lu_factorization( aa, mist::matrix_style::sy ) << std::endl << std::endl;
		std::cout << mist::lu_factorization( aa, mist::matrix_style::ge ) << std::endl << std::endl;
	}

	{
		std::cout << "QR factorization" << std::endl;

		matrix aa = a, Q, R;
		mist::qr_factorization( aa, Q, R );
		std::cout << Q << std::endl << std::endl;
		std::cout << R << std::endl << std::endl;
		std::cout << Q * R << std::endl << std::endl;
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

		matrix aa = a, eval, evec;
		mist::eigen( aa, eval, evec, mist::matrix_style::sy );
		std::cout << eval << std::endl << std::endl;
		std::cout << evec << std::endl << std::endl;
	}

	{
		std::cout << "Determinant" << std::endl;

		std::cout << mist::det( a * a.t( ), mist::matrix_style::ge ) << std::endl << std::endl;
		std::cout << mist::det( a * a.t( ), mist::matrix_style::sy ) << std::endl << std::endl;
	}

	{
		std::cout << "Trace" << std::endl;

		std::cout << mist::trace( a * a.t( ) ) << std::endl << std::endl;
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
		//mist::svd( aa, s, vt );
		std::cout << u << std::endl << std::endl;
		std::cout << s << std::endl << std::endl;
		std::cout << vt << std::endl << std::endl;
		std::cout << u * s * vt << std::endl << std::endl;
	}
	return( 0 );
}

