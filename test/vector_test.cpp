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

	mist::vector2< signed int > v1( 1, 2 );
	mist::vector2< float > v2( 3.5, 4.5 );

	cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << endl;
	cout << " 1. Operator A + B = \t"; cout << ( v1 + v2 ) << endl;
	cout << " 4. Operator A - B = \t"; cout << ( v1 - v2 ) << endl;
	cout << " 7. Operator A * B = \t"; cout << ( v1 * v2 ) << endl;
	cout << " 8. Operator A * 2 = \t"; cout << ( v1 *  2 ) << endl;
	cout << " 9. Operator 2 * B = \t"; cout << ( 2  * v2 ) << endl;
	cout << "11. Operator A / 2 = \t"; cout << ( v1 / 2  ) << endl;

	cout << "12. Outer Product A * B = \t"; cout << ( v1 * v2 ) << endl;
	cout << "13. Inner Product A ^ B = \t"; cout << ( v1 ^ v2 ) << endl;

	cout << "16. Operator A == B = \t"; cout << ( v1 == v2 ) << endl;
	cout << "17. Operator A != B = \t"; cout << ( v1 != v2 ) << endl;
	cout << "18. Operator A <  B = \t"; cout << ( v1 <  v2 ) << endl;
	cout << "19. Operator A <= B = \t"; cout << ( v1 <= v2 ) << endl;
	cout << "20. Operator A >  B = \t"; cout << ( v1 >  v2 ) << endl;
	cout << "21. Operator A >= B = \t"; cout << ( v1 >= v2 ) << endl;


	cout << "22. Operator 2.5 * A + 3.8 * B = \t"; cout << ( 2.5 * v1 + 3.8 * v2 ) << endl;
	cout << "23. Operator 2.5 * ( A + B ) = \t"; cout << ( 2.5 * ( v1 + v2 ) ) << endl;
}

void test_vector2( )
{
	using namespace std;

	mist::vector3< signed int > v1( 1, 2, 3 );
	mist::vector3< float > v2( 1.5, 2.5, 4.5 );

	cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << endl;
	cout << " 1. Operator A + B = \t"; cout << ( v1 + v2 ) << endl;
	cout << " 4. Operator A - B = \t"; cout << ( v1 - v2 ) << endl;
	cout << " 7. Operator A * B = \t"; cout << ( v1 * v2 ) << endl;
	cout << " 8. Operator A * 2 = \t"; cout << ( v1 *  2 ) << endl;
	cout << " 9. Operator 2 * B = \t"; cout << ( 2  * v2 ) << endl;
	cout << "11. Operator A / 2 = \t"; cout << ( v1 / 2  ) << endl;

	cout << "12. Outer Product A * B = \t"; cout << ( v1 * v2 ) << endl;
	cout << "13. Inner Product A ^ B = \t"; cout << ( v1 ^ v2 ) << endl;

	cout << "16. Operator A == B = \t"; cout << ( v1 == v2 ) << endl;
	cout << "17. Operator A != B = \t"; cout << ( v1 != v2 ) << endl;
	cout << "18. Operator A <  B = \t"; cout << ( v1 <  v2 ) << endl;
	cout << "19. Operator A <= B = \t"; cout << ( v1 <= v2 ) << endl;
	cout << "20. Operator A >  B = \t"; cout << ( v1 >  v2 ) << endl;
	cout << "21. Operator A >= B = \t"; cout << ( v1 >= v2 ) << endl;


	cout << "22. Operator 2.5 * A + 3.8 * B = \t"; cout << ( 2.5 * v1 + 3.8 * v2 ) << endl;
	cout << "23. Operator 2.5 * ( A + B ) = \t"; cout << ( 2.5 * ( v1 + v2 ) ) << endl;
}

void test_vector3( )
{
	using namespace std;

	mist::vector< double > v1( 3 );
	mist::vector< double > v2( 3 );

	v1[ 0 ] = 1.0;
	v2[ 1 ] = 1.0;

	cout << " v1 * v2 = " << ( v1 * v2 ).t( ) << endl << endl;
}

void test_vector4( )
{
	using namespace std;

	mist::vector< double > v1( 3 );
	mist::vector< double > v2( 3 );

	v1[ 0 ] = 1.0;
	v2[ 1 ] = 1.0;

	cout << " v1 ^ v2 = " << ( v1 ^ v2 ) << endl << endl;
}

void test_vector5( )
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
	cout << "1) Check vector2 operations." << endl;
	cout << "2) Check vector3 operations." << endl;
	cout << "3) Check outer product of two vectors." << endl;
	cout << "4) Check inner product of two vectors." << endl;
	cout << "5) Check matrix-vector multiply operation." << endl;
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
		test_vector4( );
		break;

	case 5:
		test_vector5( );
		break;

	case 6:
		break;

	default:
		break;
	}

	return( 0 );
}
