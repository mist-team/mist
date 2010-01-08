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

#include <mist/config/binary.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::binary v1 = false, v2 = true;

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

	//cout << "15. Size of bool = \t"; cout << sizeof( value< bool > ) << endl;
	//cout << "16. Size of int  = \t"; cout << sizeof( value< int > ) << endl;
	//cout << "17. Size of char = \t"; cout << sizeof( value< char > ) << endl;

	return( 0 );
}
