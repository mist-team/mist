#include <iostream>
#include <mist/config/integer.h>

int main( int argc, char *argv[] )
{
//	mist::integer< 21 > v1( 100000 ), v2( 2 );
//	mist::integer< 20 > v1( "1234567890123456788" ), v2( "1234567890123456789" );
//	mist::integer< 20 > v1( "1049" ), v2( "1048" );
	mist::integer< 10 > v1( "1234567890" ), v2( "1234567890" );
//	mist::integer< 10 > v1( "1234567890" ), v2( "-1234567890" );
//	mist::integer< 20 > v1( "1234567890123456788" ), v2( "-1234567890123456787" );
//	mist::integer< 4 > v1( 900 ), v2( 9000 );


	std::cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << std::endl;
	std::cout << " 1. Operator A + B = \t"; std::cout << ( v1 + v2 ) << std::endl;
	std::cout << " 2. Operator A + 3 = \t"; std::cout << ( v1 + 3  ) << std::endl;
	std::cout << " 3. Operator 3 + B = \t"; std::cout << ( 3  + v2 ) << std::endl;
	std::cout << " 4. Operator A - B = \t"; std::cout << ( v1 - v2 ) << std::endl;
	std::cout << " 5. Operator A - 3 = \t"; std::cout << ( v1 - 3  ) << std::endl;
	std::cout << " 6. Operator 3 - B = \t"; std::cout << ( 3  - v2 ) << std::endl;
	std::cout << " 7. Operator A * B = \t"; std::cout << ( v1 * v2 ) << std::endl;
	std::cout << " 8. Operator A * 2 = \t"; std::cout << ( v1 *  2 ) << std::endl;
	std::cout << " 9. Operator 2 * B = \t"; std::cout << ( 2  * v2 ) << std::endl;
	std::cout << "10. Operator A / B = \t"; std::cout << ( v1 / v2 ) << std::endl;
	std::cout << "11. Operator A / 2 = \t"; std::cout << ( v1 / 2  ) << std::endl;

	std::cout << "12. Operator A % B = \t"; std::cout << ( v1 % v2 ) << std::endl;
	std::cout << "13. Operator A | B = \t"; std::cout << ( v1 | v2 ) << std::endl;
	std::cout << "14. Operator A & B = \t"; std::cout << ( v1 & v2 ) << std::endl;
	std::cout << "15. Operator A ^ B = \t"; std::cout << ( v1 ^ v2 ) << std::endl;

	std::cout << "16. Operator A == B = \t"; std::cout << ( v1 == v2 ) << std::endl;
	std::cout << "17. Operator A != B = \t"; std::cout << ( v1 != v2 ) << std::endl;
	std::cout << "18. Operator A <  B = \t"; std::cout << ( v1 <  v2 ) << std::endl;
	std::cout << "19. Operator A <= B = \t"; std::cout << ( v1 <= v2 ) << std::endl;
	std::cout << "20. Operator A >  B = \t"; std::cout << ( v1 >  v2 ) << std::endl;
	std::cout << "21. Operator A >= B = \t"; std::cout << ( v1 >= v2 ) << std::endl;

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;

	return( 0 );
}




