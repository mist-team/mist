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
	size_type i, j;

	{
		mist::array1< size_type > a( 2, 1.0 ), b( 1, 0.5 );
		for( i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = i;
		}
		for( j = 0 ; j < b.size( ) ; j++, i++ )
		{
			b[ j ] = i;
		}
		cout << "a: " << a.reso1( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << endl;
		cout << b << endl << endl;

		a.swap( b );

		cout << "a: " << a.reso1( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << endl;
		cout << b << endl << endl;

		a.trim( 1 );
		b.trim( 1 );

		cout << "triming" << endl;
		cout << "a: " << a.reso1( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << endl;
		cout << b << endl << endl;
	}

	{
		mist::array2< size_type > a( 2, 2, 1.0, 1.0 ), b( 3, 3, 0.5, 0.5 );
		for( i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = i;
		}
		for( j = 0 ; j < b.size( ) ; j++, i++ )
		{
			b[ j ] = i;
		}
		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << endl;
		cout << b << endl << endl;

		a.swap( b );

		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << endl;
		cout << b << endl << endl;

		a.trim( 1, 1 );
		b.trim( 1, 1 );

		cout << "triming" << endl;
		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << endl;
		cout << b << endl << endl;
	}

	{
		mist::array3< size_type > a( 2, 2, 2, 1.0, 1.0, 1.0 ), b( 3, 3, 3, 0.5, 0.5, 0.5 );
		for( i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = i;
		}
		for( j = 0 ; j < b.size( ) ; j++, i++ )
		{
			b[ j ] = i;
		}
		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << ", " << a.reso3( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << ", " << b.reso3( ) << endl;
		cout << b << endl << endl;

		a.swap( b );

		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << ", " << a.reso3( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << ", " << b.reso3( ) << endl;
		cout << b << endl << endl;

		a.trim( 1, 1, 1 );
		b.trim( 1, 1, 0 );

		cout << "triming" << endl;
		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << ", " << a.reso3( ) << endl;
		cout << a << endl;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << ", " << b.reso3( ) << endl;
		cout << b << endl << endl;
	}


	return( 0 );
}
