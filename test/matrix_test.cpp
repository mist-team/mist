#include <iostream>

#include <cmath>
#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/timer.h>


void test_matrix_multiply( )
{
	using namespace std;

	mist::matrix< double > m( 1000,1000 );
	mist::matrix< double > n( 1000,1000 );
	mist::matrix< double > l( 1000,1000 );

	mist::matrix< double >::size_type r, c;

	for( r = 0 ; r < m.rows( ) ; r++ )
	{
		for( c = 0 ; c < m.cols( ) ; c++ )
		{
			m( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
			n( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
		}
	}

	cout << "mat1( 1000, 1000 ) * mat2( 1000, 1000 )" << endl << endl;

	{
		mist::timer t;
		l = m * n;
		cout << "Calculation Time: " << t << " (sec)" << endl;
	}
}

void test_matrix_operation( )
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
}


int main( )
{
	using namespace std;

	cout << "Examination of matrix operation" << endl << endl;
	cout << "1) Check the calculation time of matrix operation." << endl;
	cout << "2) Check the calculation time of matrix multiply." << endl;

	int number = -1;

	cin >> number;

	switch( number )
	{
	case 1:
		test_matrix_operation( );
		break;

	case 2:
		test_matrix_multiply( );
		break;

	default:
		break;
	}

	return( 0 );
}
