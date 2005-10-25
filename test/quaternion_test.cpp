#include <mist/quaternion.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::quaternion< signed int > v1( 1, 2, 3, 4 );
	mist::quaternion< float > v2( 1.5, 3.5, 2.5, 4.5 );

	cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << endl;
	cout << " 1. Operator A + B = \t"; cout << ( v1 + v2 ) << endl;
	cout << " 2. Operator A + 3 = \t"; cout << ( v1 + 3  ) << endl;
	cout << " 3. Operator 3 + B = \t"; cout << ( 3  + v2 ) << endl;
	cout << " 4. Operator A - B = \t"; cout << ( v1 - v2 ) << endl;
	cout << " 5. Operator A - 3 = \t"; cout << ( v1 - 3  ) << endl;
	cout << " 6. Operator 3 - B = \t"; cout << ( 3  - v2 ) << endl;
	cout << " 7. Operator A * B = \t"; cout << ( v1 * v2 ) << endl;
	cout << " 8. Operator A * 2 = \t"; cout << ( v1 *  2 ) << endl;
	cout << " 9. Operator 2 * B = \t"; cout << ( 2  * v2 ) << endl;
	cout << "10. Operator A / B = \t"; cout << ( v1 / v2 ) << endl;
	cout << "11. Operator A / 2 = \t"; cout << ( v1 / 2  ) << endl;
	cout << "12. Operator   - A = \t"; cout << - v1 << endl;

	//cout << "12. Operator A % B = \t"; cout << ( v1 % v2 ) << endl;
	//cout << "13. Operator A | B = \t"; cout << ( v1 | v2 ) << endl;
	//cout << "14. Operator A & B = \t"; cout << ( v1 & v2 ) << endl;
	//cout << "15. Operator A ^ B = \t"; cout << ( v1 ^ v2 ) << endl;

	cout << "16. Operator A == B = \t"; cout << ( v1 == v2 ) << endl;
	cout << "17. Operator A != B = \t"; cout << ( v1 != v2 ) << endl;
	cout << "18. Operator A <  B = \t"; cout << ( v1 <  v2 ) << endl;
	cout << "19. Operator A <= B = \t"; cout << ( v1 <= v2 ) << endl;
	cout << "20. Operator A >  B = \t"; cout << ( v1 >  v2 ) << endl;
	cout << "21. Operator A >= B = \t"; cout << ( v1 >= v2 ) << endl;


	cout << "22. Operator 2.5 * A + 3.8 * B = \t"; cout << ( 2.5 * v1 + 3.8 * v2 ) << endl;
	cout << "23. Operator 2.5 * ( A + B ) = \t"; cout << ( 2.5 * ( v1 + v2 ) ) << endl;

	v1 = 2.5 * ( v1 + v2 );

	mist::quaternion< double > q = mist::quaternion< double >( mist::vector3< double >( 1, 0, 0 ), mist::vector3< double >( 0, 1, 0 ) );
	cout << "30. Quaternion ( X, Y ) = \t"; cout << q << endl;
	cout << "31. Quaternion ( 1, 0, 0 ) = \t"; cout << q.rotate( mist::vector3< double >( 1, 0, 0 ) ) << endl;
	cout << "32. ( A , B ) -> C = \t"; cout << mist::interpolate( v1, v2, 0.2 ) << endl;

	return( 0 );
}
