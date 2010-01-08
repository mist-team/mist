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

	cout << " ( A, B ) = ( " << v1 << " ), ( " << v2 << " )" << endl;
	cout << " 1. Operator A + B      = "; cout << ( v1 + v2 ) << endl;
	cout << " 4. Operator A - B      = "; cout << ( v1 - v2 ) << endl;
	cout << " 7. Operator A * B      = "; cout << ( v1 * v2 ) << endl;
	cout << " 8. Operator A * 2      = "; cout << ( v1 *  2 ) << endl;
	cout << " 9. Operator 2 * B      = "; cout << ( 2  * v2 ) << endl;
	cout << "11. Operator A / 2      = "; cout << ( v1 / 2  ) << endl;

	cout << "12. Outer Product A * B = "; cout << ( v1 * v2 ) << endl;
	cout << "13. Inner Product A ^ B = "; cout << ( v1 ^ v2 ) << endl;

	cout << "16. Operator A == B     = "; cout << ( v1 == v2 ) << endl;
	cout << "17. Operator A != B     = "; cout << ( v1 != v2 ) << endl;
	cout << "18. Operator A <  B     = "; cout << ( v1 <  v2 ) << endl;
	cout << "19. Operator A <= B     = "; cout << ( v1 <= v2 ) << endl;
	cout << "20. Operator A >  B     = "; cout << ( v1 >  v2 ) << endl;
	cout << "21. Operator A >= B     = "; cout << ( v1 >= v2 ) << endl;


	cout << "22. Operator 2.5 * A + 3.8 * B = "; cout << ( 2.5 * v1 + 3.8 * v2 ) << endl;
	cout << "23. Operator 2.5 * ( A + B )   = "; cout << ( 2.5 * ( v1 + v2 ) ) << endl;

	v1 = 2.5 * ( v1 + v2 );
}

void test_vector2( )
{
	using namespace std;

	mist::vector3< signed int > v1( 1, 2, 3 );
	mist::vector3< float > v2( 1.5, 2.5, 4.5 );

	cout << " ( A, B ) = ( " << v1 << " ), ( " << v2 << " )" << endl;
	cout << " 1. Operator A + B      = "; cout << ( v1 + v2 ) << endl;
	cout << " 4. Operator A - B      = "; cout << ( v1 - v2 ) << endl;
	cout << " 7. Operator A * B      = "; cout << ( v1 * v2 ) << endl;
	cout << " 8. Operator A * 2      = "; cout << ( v1 *  2 ) << endl;
	cout << " 9. Operator 2 * B      = "; cout << ( 2  * v2 ) << endl;
	cout << "11. Operator A / 2      = "; cout << ( v1 / 2  ) << endl;

	cout << "12. Outer Product A * B = "; cout << ( v1 * v2 ) << endl;
	cout << "13. Inner Product A ^ B = "; cout << ( v1 ^ v2 ) << endl;

	cout << "16. Operator A == B     = "; cout << ( v1 == v2 ) << endl;
	cout << "17. Operator A != B     = "; cout << ( v1 != v2 ) << endl;
	cout << "18. Operator A <  B     = "; cout << ( v1 <  v2 ) << endl;
	cout << "19. Operator A <= B     = "; cout << ( v1 <= v2 ) << endl;
	cout << "20. Operator A >  B     = "; cout << ( v1 >  v2 ) << endl;
	cout << "21. Operator A >= B     = "; cout << ( v1 >= v2 ) << endl;


	cout << "22. Operator 2.5 * A + 3.8 * B = \t"; cout << ( 2.5 * v1 + 3.8 * v2 ) << endl;
	cout << "23. Operator 2.5 * ( A + B )   = \t"; cout << ( 2.5 * ( v1 + v2 ) ) << endl;

	v1 = 2.5 * ( v1 + v2 );
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
