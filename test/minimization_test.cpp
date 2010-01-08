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

#include <mist/minimization.h>
#include <iostream>
#include <string>

struct f1
{
	int count;
	f1( ) : count( 0 )
	{
	}

	f1( const f1 &f ) : count( f.count )
	{
		std::cout << "copy constructor" << std::endl;
	}

	double operator ()( const mist::matrix< double > &v )
	{
//		std::cout << count++ << std::endl;
//		std::cout << count++ << "                                     \r";
		double x = v[ 0 ];
		double y = v[ 1 ];
		count++;
		return( 4.0 * ( x - 1.0 ) * ( x - 1.0 ) * ( x - 1.0 ) * ( x - 1.0 ) + ( y - 2.0 ) * ( y - 2.0 ) + 100 );
	}
};

struct f2
{
	int count;
	f2( ) : count( 0 )
	{
	}

	f2( const f2 &f ) : count( f.count )
	{
		std::cout << "copy constructor" << std::endl;
	}

	double operator ()( double x )
	{
		count++;
		const double pai = 3.1415926535897932384626433832795;
		//	return( ( x - 100 ) * ( x - 100 ) );
//		return( x );
		return( std::sin( x * pai / 180.0 ) );
		//	return( std::sin( x ) + x / 5.0 );
	}

	template < class Matrix >
	double operator ()( const Matrix &x )
	{
		return( this->operator ()( x[ 0 ] ) );
	}
};


int main( int argc, char *argv[] )
{
	using namespace std;

	typedef mist::matrix< double > matrix_type;

	double ftol = 1.0e-8;

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 );
		f1 func;
		p[ 0 ] = 1000;
		p[ 1 ] = -2000;
		double err = mist::gradient::minimization( p, functor_reference( func ), ftol );
		cout << "Gradient descent" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		f1 func;
		p[ 0 ] = 1000;
		p[ 1 ] = -2000;
		double err = mist::powell::minimization( p, d, functor_reference( func ), ftol );
		cout << "Powell's method" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		f1 func;
		p[ 0 ] = 1000;
		p[ 1 ] = -2000;
		double err = mist::lucidi::minimization( p, d, functor_reference( func ), ftol );
		cout << "Lucidi's method" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 );
		f1 func;
		p[ 0 ] = 1000;
		p[ 1 ] = -2000;
		double err = mist::condor::minimization( p, functor_reference( func ), ftol, 1000 );
		cout << "CONDOR algorithm" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f2 > functor_reference;
		double x;
		f2 func;
		double err = mist::gold::minimization( -90, 270, x, functor_reference( func ), 0.00001, 100, true );
		cout << "Golden-ratio divide method" << endl;
		cout << "f( " << x << " ) = " << err << " , count= " << func.count << endl << endl;
	}
	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f2 > functor_reference;
		double x;
		f2 func;
		double err = mist::brent::minimization( -90, 270, x, functor_reference( func ), 0.00001, 100, true );
		cout << "Brent's method" << endl;
		cout << "f( " << x << " ) = " << err << " , count= " << func.count << endl << endl;
	}
	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f2 > functor_reference;
		matrix_type p( 1, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		f2 func;
		p[ 0 ] = 0;
		double err = mist::condor::minimization( p, functor_reference( func ), ftol, 1000 );
		cout << "CONDOR algorithm" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		matrix_type bound( 2, 2 );
		f1 func;
		bound( 0, 0 ) = -10;
		bound( 0, 1 ) = 20;
		bound( 1, 0 ) = 3;
		bound( 1, 1 ) = 5;
		p[ 0 ] = -5;
		p[ 1 ] = 4;
		double err = mist::gradient::minimization( p, bound, functor_reference( func ), ftol );
		cout << "Gradient descent [constrained]" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	{
		typedef mist::__minimization_utility__::__no_copy_constructor_functor__< f1 > functor_reference;
		matrix_type p( 2, 1 ), d = mist::matrix< double >::identity( 2, 2 );
		matrix_type bound( 2, 2 );
		f1 func;
		bound( 0, 0 ) = -10;
		bound( 0, 1 ) = 20;
		bound( 1, 0 ) = 3;
		bound( 1, 1 ) = 5;
		p[ 0 ] = -5;
		p[ 1 ] = 4;
		double err = mist::powell::minimization( p, d, bound, functor_reference( func ), ftol );
		cout << "Powell's method [constrained]" << endl;
		cout << "f( " << p.t( ) << " ) = " << err << " , count= " << func.count << endl << endl;
	}

	return( 0 );
}
