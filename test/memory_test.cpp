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
#include <mist/mist.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	typedef size_t size_type;
	size_type i;

	char memories[ 40 ];
	size_type num = sizeof( memories ) - 1;

	// ã§óLÉÅÉÇÉäóÃàÊÇèâä˙âªÇ∑ÇÈ
	for( i = 0 ; i < num ; i++ )
	{
		memories[ i ] = '?';
	}
	memories[ i ] = '\0';

	cout << "shared memory: " << memories << endl;

	// ÇPéüå≥îzóÒ
	{
		mist::array< char > a( 5, memories, num ), b( 8 );
		for( i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = static_cast< char >( '0' + ( i % 16 ) );
		}

		for( i = 0 ; i < b.size( ) ; i++ )
		{
			b[ i ] = static_cast< char >( 'A' + ( i % ( 'z' - 'A' ) ) );
		}

		cout << "shared memory: " << memories << endl;
		cout << a << endl << endl;
		cout << "a <- b" << endl;
		cout << b << endl;

		a = b;

		cout << a << endl;
		cout << "shared memory: " << memories << endl;
	}

	// ÇQéüå≥îzóÒ
	{
		mist::array2< char > a( 5, 5, memories, num ), b( 6, 6 );
		for( i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = static_cast< char >( '0' + ( i % 16 ) );
		}

		for( i = 0 ; i < b.size( ) ; i++ )
		{
			b[ i ] = static_cast< char >( 'A' + ( i % ( 'z' - 'A' ) ) );
		}

		cout << "shared memory: " << memories << endl;
		cout << a << endl << endl;
		cout << "a <- b" << endl;
		cout << b << endl << endl;

		a = b;

		cout << a << endl;
		cout << "shared memory: " << memories << endl;
	}


	return( 0 );
}
