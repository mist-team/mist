#include <iostream>

#include <cmath>

#include <mist/mist.h>

#include <mist/matrix.h>

#undef _MIST_VECTOR_SUPPORT_
#define _MIST_VECTOR_SUPPORT_	1
#include <mist/vector.h>

#include <mist/timer.h>


void test_vector1( )
{
	using namespace std;

	mist::vector< double > v1( 3 );
	mist::vector< double > v2( 3 );

	v1[ 0 ] = 1.0;
	v2[ 1 ] = 1.0;

	cout << " v1 * v2 = " << ( v1 * v2 ).t( ) << endl << endl;
}

void test_vector2( )
{
	using namespace std;

	mist::vector< double > v1( 3 );
	mist::vector< double > v2( 3 );

	v1[ 0 ] = 1.0;
	v2[ 1 ] = 1.0;

	cout << " v1 ^ v2 = " << ( v1 ^ v2 ) << endl << endl;
}

void test_vector3( )
{
	using namespace std;

	mist::vector< double > v1( 3 );
	mist::vector< double > v2( 3 );

	v1[ 0 ] = 1.0;
	v2[ 1 ] = 1.0;

	cout << " v1 * v2 = " << v1 * v2.t( ) << endl << endl;
}


int main( )
{
	using namespace std;

	cout << "Examination of vector operation" << endl << endl;
	cout << "1) Check outer product of two vectors." << endl;
	cout << "2) Check inner product of two vectors." << endl;
	cout << "3) Check matrix-vector multiply operation." << endl;
	//cout << "4) Check A <- M x MT operation." << endl;
	//cout << "5) Evaluate Matrix operation 1." << endl;
	//cout << "6) Evaluate Matrix operation 2." << endl;

	int number = -1;

	cin >> number;

	switch( number )
	{
	case 1:
		test_vector1( );
		break;

	case 2:
		test_vector2( );
		break;

	case 3:
		test_vector3( );
		break;

	case 4:
		break;

	case 5:
		break;

	case 6:
		break;

	default:
		break;
	}

	return( 0 );
}
