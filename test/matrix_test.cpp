#include <iostream>

#include <cmath>
#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/numeric.h>

#ifndef WIN32
#include <ctime>
#else
#include <windows.h>
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )
#endif

class timer
{
public:
	timer( )
	{
#ifndef WIN32
		_start_time = std::clock( );
#else
		_start_time = timeGetTime( );
#endif
	} // postcondition: elapsed()==0

	void restart( )
	{
#ifndef WIN32
		_start_time = std::clock( );
#else
		_start_time = timeGetTime( );
#endif
	} // post: elapsed()==0

	double elapsed( ) const                  // return elapsed time in seconds
	{
#ifndef WIN32
		return( double(std::clock( ) - _start_time) / CLOCKS_PER_SEC );
#else
		return( double(timeGetTime( ) - _start_time) / 1000.0 );
#endif
	}

private:
#ifndef WIN32
	std::clock_t _start_time;
#else
	DWORD _start_time;
#endif
}; // timer


void test_matrix_multiply( )
{
	using namespace std;

	mist::matrix< double > m( 1000,1000 );
	mist::matrix< double > n( 1000,1000 );
	mist::matrix< double > l( 1000,1000 );

	for( int r = 0 ; r < m.rows( ) ; r++ )
	{
		for( int c = 0 ; c < m.cols( ) ; c++ )
		{
			m( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
			n( r, c ) = rand( ) / static_cast< double >( RAND_MAX );
		}
	}

	cout << "mat1( 1000, 1000 ) * mat2( 1000, 1000 )" << endl << endl;

	{
		timer t;
		l = m * n;
		cout << "Calculation Time: " << t.elapsed( ) << " (sec)" << endl;
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
	long i, j, k, l, loop = 1000;
	{
		timer t;
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
		cout << "Calculation Time: " << t.elapsed( ) << " (sec)" << endl;
	}

	{
		timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			mat = ( mat1 + mat2 * mat3 + mat4 ).t( );
		}
		cout << "< In the case of Expression Template >" << endl;
		cout << "Calculation Time: " << t.elapsed( ) << " (sec)" << endl;
	}
}


void test_matrix_inverse1( )
{
	using namespace std;

	cout << "transpose( mat1 + mat2 * mat3 + mat4 )" << endl << endl;

	mist::matrix< double > a( 3, 3 );
	mist::matrix< double > b( 3, 1 );
	mist::matrix< double > a1( 3, 3 );
	mist::matrix< double > a2( 3, 3 );
	mist::matrix< double > b1( 3, 3, 1.0 );
	mist::matrix< double > b2( 3, 3, 1.0 );
	mist::matrix< double > p1, p2;

	a( 0, 0 ) = 1; a( 0, 1 ) =  1; a( 0, 2 ) =  1;
	a( 1, 0 ) = 3; a( 1, 1 ) =  2; a( 1, 2 ) = -2;
	a( 2, 0 ) = 2; a( 2, 1 ) = -1; a( 2, 2 ) =  3;

	b[0] = 6;
	b[1] = 1;
	b[2] = 9;

	a1 = a2 = a;
	b1 = b2 = b;

	mist::inverse( a1 );
	cout << a * a1 << endl << endl;
	mist::inverse1( a2 );
	cout << a * a2 << endl << endl;
}


void test_matrix_inverse2( )
{
	using namespace std;
	mist::matrix< double > mat1( 100, 100 );
	mist::matrix< double > mat2( 100, 100 );
	mist::matrix< double > mat( 100, 100 );
	mist::matrix< double >::size_type i, j, k, l, loop = 100;

	for( i = 0 ; i < mat.size( ) ; i++ )
	{
		mat[i] = (double)rand( ) / (double)RAND_MAX;
	}

	// transpose( mat1 + mat2 * mat3 + mat4 )
	{
		timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			mat1 = mat;
			mist::inverse( mat1 );
		}
		cout << "< In the case of Expression Template >" << endl;
		cout << "Calculation Time: " << t.elapsed( ) << " (sec)" << endl;
	}

	{
		timer t;
		for( i = 0 ; i < loop ; i++ )
		{
			mat1 = mat;
			mist::inverse1( mat2 );
		}
		cout << "< In the case of Expression Template >" << endl;
		cout << "Calculation Time: " << t.elapsed( ) << " (sec)" << endl;
	}
}

int main( )
{
	using namespace std;

	cout << "Examination of matrix operation" << endl << endl;
	cout << "1) Check the calculation time of matrix operation." << endl;
	cout << "2) Compute the inverse matrix." << endl;
	cout << "3) Check the calculation time of inverse matrix." << endl;
	cout << "4) Check the calculation time of matrix multiply." << endl;

	int number = -1;

	cin >> number;

	switch( number )
	{
	case 1:
		test_matrix_operation( );
		break;

	case 2:
		test_matrix_inverse1( );
		break;

	case 3:
		test_matrix_inverse2( );
		break;

	case 4:
		test_matrix_multiply( );
		break;

	default:
		break;
	}
}
