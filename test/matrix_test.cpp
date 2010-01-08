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

#include <cmath>
#include <mist/mist.h>

#include <mist/matrix.h>
#include <mist/numeric.h>
#include <mist/timer.h>


void test_matrix_multiply1( )
{
	using namespace std;

	mist::matrix< double > m( 1000, 500 );
	mist::matrix< double > n( 1000, 500 );
	mist::matrix< double > l1( 1000, 1000 );
	mist::matrix< double > l2( 1000, 1000 );

	mist::matrix< double >::size_type r, c;

	for( r = 0 ; r < m.rows( ) ; r++ )
	{
		for( c = 0 ; c < m.cols( ) ; c++ )
		{
			m( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
		}
	}

	for( r = 0 ; r < n.rows( ) ; r++ )
	{
		for( c = 0 ; c < n.cols( ) ; c++ )
		{
			n( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
		}
	}

	cout << "mat1( 1000, 1000 ) * mat2( 1000, 1000 )" << endl << endl;

	{
		mist::timer t;
		l1 = m * n.t( );
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	{
		mist::timer t;
		mist::multiply( m, n, l2, false, true );
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	double err = 0.0;
	for( r = 0 ; r < l1.size( ) ; r++ )
	{
		err += std::abs( l1[ r ] - l2[ r ] );
	}
	std::cout << "Err: " << err << std::endl;

}

void test_matrix_multiply2( )
{
	using namespace std;

	mist::matrix< double > a( 3, 3 );

	a( 0, 0 ) = 1.0; a( 0, 1 ) =  1.0; a( 0, 2 ) =  1.0;
	a( 1, 0 ) = 3.0; a( 1, 1 ) =  1.0; a( 1, 2 ) = -3.0;
	a( 2, 0 ) = 1.0; a( 2, 1 ) = -2.0; a( 2, 2 ) = -5.0;

	cout << a << endl;

	cout << "mat1( 3, 3 ) * mat2( 3, 3 ).t( )" << endl << endl;

	cout << a * a.t( ) << endl << endl;
	cout << ( - a + a.t( ) ).t( ) << endl << endl;
}

void test_matrix_operation1( )
{
	using namespace std;

	cout << "transpose( mat1 + mat2 * mat3 + mat4 )" << endl << endl;

	mist::matrix< double > mat1( 100, 100 );
	mist::matrix< double > mat2( 100, 100 );
	mist::matrix< double > mat3( 100, 100 );
	mist::matrix< double > mat4( 100, 100 );
	mist::matrix< double > mat( 100, 100 );
	mist::matrix< double > dmy;

	// transpose( mat1 + mat2 * mat3 + mat4 )
	mist::matrix< double >::size_type i, j, k, l, loop = 1000;
	{
		mist::timer t;
		double tmp;
		for( l = 0 ; l < loop ; l++ )
		{
			for( j = 0 ; j < mat.rows( ) ; j++ )
			{
				for( i = 0 ; i < mat.cols( ) ; i++ )
				{
					tmp = 0.0;
					for( k = 0 ; k < mat3.rows( ) ; k++ )
					{
						tmp += mat2( i, k ) * mat3( k, j );
					}
					mat( j, i ) = mat1( i, j ) + tmp + mat4( i, j );
				}
			}
			dmy = mat;
		}
		cout << "< In the case of Non Expression Template >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	{
		mist::timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			mat = ( mat1 + mat2 * mat3 + mat4 ).t( );
		}
		cout << "< In the case of Expression Template >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	{
		mist::timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			mist::multiply( mat2, mat3, dmy );
			mat = ( mat1 + dmy + mat4 ).t( );
		}
		cout << "< In the case of BLAS Level 3 >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}
}


void test_matrix_operation2( )
{
	using namespace std;

	cout << "A <- M x MT" << endl << endl;

	mist::matrix< double > mat1( 1000000, 1 );
	mist::matrix< double > mat2( 1000000, 1 );
	mist::matrix< double > mat( 1000, 1000 );
	mist::matrix< double > dmy;

	mist::matrix< double >::size_type i, j, l, loop = 100;
	{
		mist::timer t;
		for( l = 0 ; l < loop ; l++ )
		{
			for( i = 0 ; i < mat.cols( ) ; i++ )
			{
				const double &x = mat1[ i ];
				for( j = 0 ; j < mat.rows( ) ; j++ )
				{
					mat( i, j ) = x * mat2[ j ];
				}
			}
		}
		cout << "< In the case of A <- M x MT operation 1 >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	{
		mist::timer t;
		for( l = 0 ; l < loop ; l++ )
		{
			for( i = 0 ; i < mat.cols( ) ; i++ )
			{
				for( j = 0 ; j < mat.rows( ) ; j++ )
				{
					mat( i, j ) = mat1[ i ] * mat2[ j ];
				}
			}
		}
		cout << "< In the case of A <- M x MT operation 2 >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}
}


void evaluate_matrix_operation1( )
{
	using namespace std;
	typedef mist::matrix< double >::size_type size_type;

	cout << "transpose( mat1 + mat2 * mat3 + mat4 )" << endl << endl;

	mist::matrix< double > mat1( 100, 100 );
	mist::matrix< double > mat2( 100, 100 );
	mist::matrix< double > mat3( 100, 100 );
	mist::matrix< double > mat4( 100, 100 );
	mist::matrix< double > A( 100, 100 );
	mist::matrix< double > B( 100, 100 );

	size_type r, c;
	for( r = 0 ; r < mat1.rows( ) ; r++ )
	{
		for( c = 0 ; c < mat1.cols( ) ; c++ )
		{
			mat1( r, c ) = rand( ) / 3.0 * 10000.0;
			mat2( r, c ) = rand( ) / 3.0 * 10000.0;
			mat3( r, c ) = rand( ) / 3.0 * 10000.0;
			mat4( r, c ) = rand( ) / 3.0 * 10000.0;
			//mat1( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
			//mat2( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
			//mat3( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
			//mat4( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
		}
	}

	// transpose( mat1 + mat2 * mat3 + mat4 )
	size_type i, j, k;
	{
		mist::timer t;
		double tmp;
		double error = 0.0;
		for( j = 0 ; j < A.rows( ) ; j++ )
		{
			for( i = 0 ; i < A.cols( ) ; i++ )
			{
				tmp = 0.0;
				for( k = 0 ; k < mat3.rows( ) ; k++ )
				{
					tmp += mat2( i, k ) * mat3( k, j );
				}
				double tt = mat1( i, j ) + tmp + mat4( i, j );
				double ttt = mat1( i, j ) + tmp;
				ttt = ttt + mat4( i, j );
				A( j, i ) = mat1( i, j ) + tmp;
				A( j, i ) = A( j, i ) + mat4( i, j );
//				A( j, i ) = mat1( i, j ) + tmp + mat4( i, j );
				error += ( tt - ttt ) * ( tt - ttt );
			}
		}
		cout << "< In the case of Non Expression Template >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << error << endl;
	}

	{
		mist::timer t;

		B = ( mat1 + mat2 * mat3 + mat4 ).t( );

		cout << "< In the case of Expression Template >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	double err = 0.0;
	for( r = 0 ; r < mat1.rows( ) ; r++ )
	{
		for( c = 0 ; c < mat1.cols( ) ; c++ )
		{
			err += ( A( r, c ) - B( r, c ) ) * ( A( r, c ) - B( r, c ) );
		}
	}

	cout << "Estimate Error: " << err << endl;
}



void evaluate_matrix_operation2( )
{
	using namespace std;
	typedef mist::matrix< double >::size_type size_type;

	mist::matrix< double > a( 3, 3 );

	a( 0, 0 ) = 1.0; a( 0, 1 ) =  1.0; a( 0, 2 ) =  1.0;
	a( 1, 0 ) = 3.0; a( 1, 1 ) =  1.0; a( 1, 2 ) = -3.0;
	a( 2, 0 ) = 1.0; a( 2, 1 ) = -2.0; a( 2, 2 ) = -5.0;

	mist::matrix< double > mat1( 3, 3 );
	mist::matrix< double > mat2( 3, 3 );
	mist::matrix< double > mat3( 3, 3 );
	mist::matrix< double > mat4( 3, 3 );
	mist::matrix< double > A( 3, 3 );
	mist::matrix< double > B( 3, 3 );

	mat1 = mat2 = mat3 = mat4 = a;

	// transpose( mat1 + mat2 * mat3 + mat4 )
	size_type i, j, k, l;
	{
		mist::timer t;
		double tmp;
		for( j = 0 ; j < A.rows( ) ; j++ )
		{
			for( i = 0 ; i < A.cols( ) ; i++ )
			{
				tmp = 0.0;
				for( k = 0 ; k < mat3.rows( ) ; k++ )
				{
					tmp += mat2( i, k ) * mat3( k, j );
				}
				A( j, i ) = mat1( i, j ) + tmp + mat4( i, j );
			}
		}
		cout << "< In the case of Non Expression Template >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	{
		mist::timer t;

		B = ( mat1 + mat2 * mat3 + mat4 ).t( );

		cout << "< In the case of Expression Template >" << endl;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}

	double err = 0.0;
	for( l = 0 ; l < A.size( ) ; l++ )
	{
		err += ( A[ l ] - B[ l ] ) * ( A[ l ] - B[ l ] );
	}

	cout << "Estimate Error: " << err << endl;
}

int main( )
{
	using namespace std;

	cout << "Examination of matrix operation" << endl << endl;
	cout << "1) Check the calculation time of matrix operation." << endl;
	cout << "2) Check the calculation time of matrix multiply." << endl;
	cout << "3) Check matrix multiply operation." << endl;
	cout << "4) Check A <- M x MT operation." << endl;
	cout << "5) Evaluate Matrix operation 1." << endl;
	cout << "6) Evaluate Matrix operation 2." << endl;

	int number = -1;

	cin >> number;

	switch( number )
	{
	case 1:
		test_matrix_operation1( );
		break;

	case 2:
		test_matrix_multiply1( );
		break;

	case 3:
		test_matrix_multiply2( );
		break;

	case 4:
		test_matrix_operation2( );
		break;

	case 5:
		evaluate_matrix_operation1( );
		break;

	case 6:
		evaluate_matrix_operation2( );
		break;

	default:
		break;
	}

	return( 0 );
}
