#include <mist/config/integer.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

//	mist::integer< 21 > v1( 100000 ), v2( 2 );
//	mist::integer< 20 > v1( "1234567890123456788" ), v2( "1234567890123456789" );
	mist::integer< 20 > v1( "1234567890123456788" ), v2( "-1234567890123456787" );
//	mist::integer< 2 > v1( 900 ), v2( 1000 );


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

	cout << "12. Operator A % B = \t"; cout << ( v1 % v2 ) << endl;
	cout << "13. Operator A | B = \t"; cout << ( v1 | v2 ) << endl;
	cout << "14. Operator A & B = \t"; cout << ( v1 & v2 ) << endl;
	cout << "15. Operator A ^ B = \t"; cout << ( v1 ^ v2 ) << endl;

	cout << "16. Operator A == B = \t"; cout << ( v1 == v2 ) << endl;
	cout << "17. Operator A != B = \t"; cout << ( v1 != v2 ) << endl;
	cout << "18. Operator A <  B = \t"; cout << ( v1 <  v2 ) << endl;
	cout << "19. Operator A <= B = \t"; cout << ( v1 <= v2 ) << endl;
	cout << "20. Operator A >  B = \t"; cout << ( v1 >  v2 ) << endl;
	cout << "21. Operator A >= B = \t"; cout << ( v1 >= v2 ) << endl;

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;

	return( 0 );
}
