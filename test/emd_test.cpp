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
#include <mist/fft/fft.h>
#include <mist/hht.h>

int main( int argc, char *argv[] )
{

	mist::array1< double > y( 100 );
	mist::array1< mist::array1 < double > > imf;
	for( size_t i = 0 ; i < y.size1() ; i++ )
	{
		y( i ) = sin( 3.14 * ( static_cast< double >( i ) + 40.0) / 20.0)
			+ sin(3.14 * ( static_cast< double >( i ) + 5.0) / 80.0 ) * 3.0
			+ sin(3.14 * ( static_cast< double >( i ) + 0.1) / 4.6 ) * 0.4;
	}
	
	// Empirical mode decomposition
	mist::emd( y, imf );
	for( size_t i = 0 ; i < ( imf( 0 ) ).size1( ) ; i++ )
	{
		std::cout << i << " " << y( i );
		for( size_t j = 0 ; j < imf.size1( ) ; j++ )
		{
			std::cout << " " << ( imf( j ) )( i );
		}
		std::cout << std::endl;
	}
	
	return 0;
}
