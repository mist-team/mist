#include <iostream>

#include <mist/mist.h>
//#include <mist/matrix.h>
#include <mist/matrix_et.h>

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


/*
int main( )
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
	long i, j, k, l, loop = 100;
	{
		timer t;
		double tmp;
		for( l = 0 ; l < loop ; l++ )
		{
			for( i = 0 ; i < mat.cols( ) ; i++ )
			{
				for( j = 0 ; j < mat.rows( ) ; j++ )
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

	return( 0 );
}


/*/

int main( )
{
	using namespace std;

	cout << "transpose( mat1 + mat2 * mat3 + mat4 )" << endl << endl;

	mist::matrix< double > mat1( 3, 3 );
	mist::matrix< double > mat2( 3, 3 );
	mist::matrix< double > mat3( 3, 3 );
	mist::matrix< double > mat4( 3, 3 );
	mist::matrix< double > mat( 3, 3, 1.0 );

	mat1( 0, 0 ) = 1;
	mat1( 0, 1 ) = 2;
	mat1( 0, 2 ) = 3;
	mat1( 1, 0 ) = 4;
	mat1( 1, 1 ) = 5;
	mat1( 1, 2 ) = 6;
	mat1( 2, 0 ) = 7;
	mat1( 2, 1 ) = 8;
	mat1( 2, 2 ) = 9;

	mat2 = mat3 = mat4 = mat1;
	long i, j, k;
	double tmp;

	for( i = 0 ; i < mat.rows( ) ; i++ )
	{
		for( j = 0 ; j < mat.cols( ) ; j++ )
		{
			tmp = 0.0;
			for( k = 0 ; k < mat3.rows( ) ; k++ )
			{
				tmp += mat2( i, k ) * mat3( k, j );
			}
			mat( j, i ) = mat1( i, j ) + tmp + mat4( i, j );
		}
	}

	cout << "< In the case of Non Expression Template >" << endl;
	cout << mat << endl << endl;


	cout << "< In the case of Expression Template >" << endl;
	mat = ( mat1 + 1 + ( mat2 * mat3 + mat4 ) ).t( );
	cout << mat << endl << endl << endl;


	cout << "< In the case of Expression Template >" << endl;
	mat = ( mat1 + ( mat2 * mat3 + mat4 ) - 1 ).t( );
	cout << mat << endl << endl << endl;

	cout << "< In the case of Expression Template >" << endl;
	mat = ( mat1 + ( mat2 * mat3 + mat4 ) * 2 ).t( );
	cout << mat << endl << endl << endl;

	cout << "< In the case of Expression Template >" << endl;
	mat = ( mat1 + 2 ).t( );
	cout << mat << endl << endl << endl;

	return( 0 );
}

//*/
