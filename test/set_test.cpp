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
#include <mist/config/set.h>

void set_test( )
{
	using namespace std;

	mist::set< int > s1, s2;
	int i;

	for( i = 0 ; i < 10 ; i++ )
	{
		s1.insert( i );
	}

	for( i = 5 ; i < 7 ; i++ )
	{
		s2.insert( i );
	}

	for( i = 9 ; i < 15 ; i++ )
	{
		s2.insert( i );
	}


	cout << " ( A ) = ( " << s1 << " )" << endl;
	cout << " ( B ) = ( " << s2 << " )" << endl;
	cout << " 1. Operator A  + B  = \t"; cout << ( s1 + s2 ) << endl;
	cout << " 2. Operator A  + 3  = \t"; cout << ( s1 + 3  ) << endl;
	cout << " 3. Operator 3  + B  = \t"; cout << ( 3  + s2 ) << endl;
	cout << " 4. Operator A  - B  = \t"; cout << ( s1 - s2 ) << endl;
	cout << " 5. Operator A  - 3  = \t"; cout << ( s1 - 3  ) << endl;
	cout << " 6. Operator 3  - B  = \t"; cout << ( 3  - s2 ) << endl;
	cout << " 7. Operator A  * B  = \t"; cout << ( s1 * s2 ) << endl;
	cout << " 8. Operator A  * 2  = \t"; cout << ( s1 *  2 ) << endl;
	cout << " 9. Operator 2  * B  = \t"; cout << ( 2  * s2 ) << endl;
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

	cout << "22. Operator A == 3 = \t"; cout << ( s1 == 3  ) << endl;
	cout << "23. Operator 3 == A = \t"; cout << ( 3  == s1 ) << endl;
	cout << "24. Operator A != 3 = \t"; cout << ( s1 != 3  ) << endl;
	cout << "25. Operator 3 != A = \t"; cout << ( 3  != s1 ) << endl;
	cout << "26. Operator A <  3 = \t"; cout << ( s1 <  3  ) << endl;
	cout << "27. Operator 3 <  A = \t"; cout << ( 3  <  s1 ) << endl;
	cout << "28. Operator A <= 3 = \t"; cout << ( s1 <= 3  ) << endl;
	cout << "29. Operator 3 <= A = \t"; cout << ( 3  <= s1 ) << endl;
	cout << "30. Operator A >  3 = \t"; cout << ( s1 >  3  ) << endl;
	cout << "31. Operator 3 >  A = \t"; cout << ( 3  >  s1 ) << endl;
	cout << "32. Operator A >= 3 = \t"; cout << ( s1 >= 3  ) << endl;
	cout << "33. Operator 3 >= A = \t"; cout << ( 3  >= s1 ) << endl;

	//cout << "**. Operator A * { 1, 2, 3 } = \t"; cout << s1 * ( mist::set< int >( 1 ) << 2 << 3 ) << endl;

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;
}

void multiset_test( )
{
	using namespace std;

	mist::multiset< int > s1, s2;
	int i;

	for( i = 0 ; i < 10 ; i++ )
	{
		s1.insert( i );
		s1.insert( i );
	}

	for( i = 5 ; i < 15 ; i++ )
	{
		s2.insert( i );
	}


	cout << " ( A ) = ( " << s1 << " )" << endl;
	cout << " ( B ) = ( " << s2 << " )" << endl;
	cout << " 1. Operator A  + B  = \t"; cout << ( s1 + s2 ) << endl;
	cout << " 2. Operator A  + 3  = \t"; cout << ( s1 + 3  ) << endl;
	cout << " 3. Operator 3  + B  = \t"; cout << ( 3  + s2 ) << endl;
	cout << " 4. Operator A  - B  = \t"; cout << ( s1 - s2 ) << endl;
	cout << " 5. Operator A  - 3  = \t"; cout << ( s1 - 3  ) << endl;
	cout << " 6. Operator 3  - B  = \t"; cout << ( 3  - s2 ) << endl;
	cout << " 7. Operator A  * B  = \t"; cout << ( s1 * s2 ) << endl;
	cout << " 8. Operator A  * 2  = \t"; cout << ( s1 *  2 ) << endl;
	cout << " 9. Operator 2  * B  = \t"; cout << ( 2  * s2 ) << endl;
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

	cout << "22. Operator A == 3 = \t"; cout << ( s1 == 3  ) << endl;
	cout << "23. Operator 3 == A = \t"; cout << ( 3  == s1 ) << endl;
	cout << "24. Operator A != 3 = \t"; cout << ( s1 != 3  ) << endl;
	cout << "25. Operator 3 != A = \t"; cout << ( 3  != s1 ) << endl;
	cout << "26. Operator A <  3 = \t"; cout << ( s1 <  3  ) << endl;
	cout << "27. Operator 3 <  A = \t"; cout << ( 3  <  s1 ) << endl;
	cout << "28. Operator A <= 3 = \t"; cout << ( s1 <= 3  ) << endl;
	cout << "29. Operator 3 <= A = \t"; cout << ( 3  <= s1 ) << endl;
	cout << "30. Operator A >  3 = \t"; cout << ( s1 >  3  ) << endl;
	cout << "31. Operator 3 >  A = \t"; cout << ( 3  >  s1 ) << endl;
	cout << "32. Operator A >= 3 = \t"; cout << ( s1 >= 3  ) << endl;
	cout << "33. Operator 3 >= A = \t"; cout << ( 3  >= s1 ) << endl;

	//cout << "**. Operator A * { 1, 2, 3 } = \t"; cout << s1 * ( mist::multiset< int >( 1 ) << 2 << 3 ) << endl;

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;
}

int main( int argc, char *argv[] )
{
	std::cout << "Set Test" << std::endl;
	set_test( );

	std::cout << std::endl << std::endl;

	std::cout << "Multi Set Test" << std::endl;
	multiset_test( );

	return( 0 );
}
