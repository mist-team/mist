// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

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




