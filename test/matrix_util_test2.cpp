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
#include <mist/matrix.h>
#include <mist/numeric.h>
#include <mist/random.h>
#include <mist/timer.h>

int main( )
{
	typedef mist::matrix< float > matrix;
	typedef mist::matrix< float >::size_type size_type;
	//typedef mist::matrix< std::complex< double > > matrix;
	matrix a( 200, 200 );

	mist::uniform::random r( 123 );      //óêêîê∂ê¨äÌÇÃç\íz

	size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		a[ i ] = r.real3( );    //êÆêîóêêîÇÃê∂ê¨
	}

	{
		std::cout << "Eigen value and vectors" << std::endl;
		mist::timer t;

		matrix eval, evec;
		mist::eigen( a, eval, evec, mist::matrix_style::ge );

		std::cout << "Computation Time: " << t << " sec" << std::endl;
	}

	return( 0 );
}

