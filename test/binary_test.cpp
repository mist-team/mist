#include <mist/config/binary.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::binary v1 = false, v2 = true;

	cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << endl;
	cout << " 1. Operator A + B = \t" << ( v1 + v2 ) << endl;
	cout << " 2. Operator A - B = \t" << ( v1 - v2 ) << endl;
	cout << " 3. Operator A * B = \t" << ( v1 * v2 ) << endl;
	cout << " 4. Operator A / B = \t" << ( v1 / v2 ) << endl;

	cout << " 5. Operator A % B = \t" << ( v1 % v2 ) << endl;
	cout << " 6. Operator A | B = \t" << ( v1 | v2 ) << endl;
	cout << " 7. Operator A & B = \t" << ( v1 & v2 ) << endl;
	cout << " 8. Operator A ^ B = \t" << ( v1 ^ v2 ) << endl;

	cout << " 9. Operator A == B = \t" << ( v1 == v2 ) << endl;
	cout << "10. Operator A != B = \t" << ( v1 != v2 ) << endl;
	cout << "11. Operator A <  B = \t" << ( v1 <  v2 ) << endl;
	cout << "12. Operator A <= B = \t" << ( v1 <= v2 ) << endl;
	cout << "13. Operator A >  B = \t" << ( v1 >  v2 ) << endl;
	cout << "14. Operator A >= B = \t" << ( v1 >= v2 ) << endl;

	//cout << "15. Size of bool = \t" << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t" << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t" << sizeof( value< char > ) << endl;

	return( 0 );
}
