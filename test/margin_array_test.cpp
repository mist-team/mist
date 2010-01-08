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

	typedef mist::array2< double >::size_type size_type;

	mist::array2< double > a( 5, 5 );
	a( 0, 0 ) = 0.1; a( 0, 1 ) = 0.2; a( 0, 2 ) = 0.1; a( 0, 3 ) = 0.6; a( 0, 4 ) = 0.4;
	a( 1, 0 ) = 0.3; a( 1, 1 ) = 4.1; a( 1, 2 ) = 5.2; a( 1, 3 ) = 4.7; a( 1, 4 ) = 0.3;
	a( 2, 0 ) = 0.2; a( 2, 1 ) = 3.5; a( 2, 2 ) = 2.8; a( 2, 3 ) = 3.9; a( 2, 4 ) = 0.8;
	a( 3, 0 ) = 0.4; a( 3, 1 ) = 4.2; a( 3, 2 ) = 5.9; a( 3, 3 ) = 4.2; a( 3, 4 ) = 0.9;
	a( 4, 0 ) = 0.2; a( 4, 1 ) = 0.3; a( 4, 2 ) = 0.1; a( 4, 3 ) = 0.1; a( 4, 4 ) = 0.4;

	mist::marray< mist::array2< int > > m( a, 2, 1, 1 );

	for( size_type j = 0 ; j < m.height( ) ; j++ )
	{
		for( size_type i = 0 ; i < m.width( ) ; i++ )
		{
			m( i, j ) = static_cast< int >( i + j * m.width( ) );
		}
	}

	std::cout << a << std::endl << std::endl;
	std::cout << m << std::endl << std::endl;

	m = a;

	std::cout << m << std::endl << std::endl;

	m.fill_margin( 9 );

	std::cout << m << std::endl << std::endl;

	return( 0 );
}
