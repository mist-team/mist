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
#include <string>
#include <mist/config/pointer.h>

struct A
{
	std::string text;

	A( const std::string &t = "hoge" ) : text( t )
	{
		std::cout << "constructor" << std::endl;
	}

	~A( )
	{
		std::cout << "destructor" << std::endl;
	}
};

inline std::ostream &operator <<( std::ostream &out, const A &a )
{
	out << a.text;
	return( out );
}

int main( int argc, char *argv[] )
{
	using namespace std;

	{
		mist::scoped_ptr< A > a( new A( "test" ) );

		std::cout << *a << std::endl;
		std::cout << a->text << std::endl;
	}

	std::cout << std::endl << std::endl;

	{
		mist::scoped_array< A > a( new A[ 2 ] );

		std::cout << *a << std::endl;
		std::cout << a->text << std::endl;
	}

	std::cout << std::endl << std::endl;


	mist::weak_ptr< A > w;
	std::cout << w << std::endl;
	{
		mist::shared_ptr< A > a = new A( "abcdefg" );

		w = a;
		std::cout << w << std::endl;

		std::cout << *a << std::endl;

		{
			mist::shared_ptr< A > b = a;

			std::cout << *a << std::endl;
		}

		std::cout << w << std::endl;
	}

	std::cout << w << std::endl;
	std::cout << std::endl << std::endl;

	{
		mist::shared_array< A > a = new A[ 2 ];

		w = a;
		std::cout << w << std::endl;

		std::cout << *a << std::endl;

		{
			mist::shared_array< A > b = a;

			std::cout << *a << std::endl;
		}

		std::cout << w << std::endl;
	}

	std::cout << w << std::endl;

	return( 0 );
}
