#include <iostream>
#include <mist/config/set.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::set< int > s1, s2;
	mist::set< int >::size_type i;

	for( i = 0 ; i < 10 ; i++ )
	{
		s1.insert( static_cast< int >( i ) );
	}

	for( i = 5 ; i < 10 ; i++ )
	{
		s2.insert( static_cast< int >( i ) );
	}


	cout << " ( A ) = ( " << s1 << " )" << endl;
	cout << " ( B ) = ( " << s2 << " )" << endl;
	cout << " 1. Operator A  + B  = \t"; cout << ( s1 + s2 ) << endl;
	cout << " 2. Operator A  + 3  = \t"; cout << ( s1 + 21 ) << endl;
	cout << " 3. Operator 21 + B  = \t"; cout << ( 21 + s2 ) << endl;
	cout << " 4. Operator A  - B  = \t"; cout << ( s1 - s2 ) << endl;
	cout << " 5. Operator A  - 21 = \t"; cout << ( s1 - 21 ) << endl;
	cout << " 6. Operator 21 - B  = \t"; cout << ( 21 - s2 ) << endl;
	cout << " 7. Operator A  * B  = \t"; cout << ( s1 * s2 ) << endl;
	//cout << " 8. Operator A * 2 = \t"; cout << ( s1 *  2 ) << endl;
	//cout << " 9. Operator 2 * B = \t"; cout << ( 2  * s2 ) << endl;
	//cout << "10. Operator A / B = \t"; cout << ( s1 / s2 ) << endl;
	//cout << "11. Operator A / 2 = \t"; cout << ( s1 / 2  ) << endl;

	//cout << "12. Operator A % B = \t"; cout << ( s1 % s2 ) << endl;
	//cout << "13. Operator A | B = \t"; cout << ( s1 | s2 ) << endl;
	//cout << "14. Operator A & B = \t"; cout << ( s1 & s2 ) << endl;
	//cout << "15. Operator A ^ B = \t"; cout << ( s1 ^ s2 ) << endl;

	cout << "16. Operator A == B = \t"; cout << ( s1 == s2 ) << endl;
	cout << "17. Operator A != B = \t"; cout << ( s1 != s2 ) << endl;
	cout << "18. Operator A <  B = \t"; cout << ( s1 <  s2 ) << endl;
	cout << "19. Operator A <= B = \t"; cout << ( s1 <= s2 ) << endl;
	cout << "20. Operator A >  B = \t"; cout << ( s1 >  s2 ) << endl;
	cout << "21. Operator A >= B = \t"; cout << ( s1 >= s2 ) << endl;

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;

	return( 0 );
}
