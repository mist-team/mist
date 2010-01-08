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

#include <mist/timer.h>
#include <mist/mist.h>
#include <mist/fft/fft.h>
#include <mist/fft/dct.h>
#include <mist/fft/dst.h>
#include <mist/io/raw.h>

#define TEST_MODE	2
#define FFT_TEST	1
#define DCT_TEST	2
#define DST_TEST	3

int main( int argc, char *argv[ ] )
{
	mist::array3< short >::size_type i;

	mist::array3< short > in;
	mist::array3< float > a3df;
	mist::array2< float > a2df;
	mist::array1< float > a1df;
	mist::array3< std::complex< float > > work3d;
	mist::array2< std::complex< float > > work2d;
	mist::array1< std::complex< float > > work1d;

	//mist::array3< float > work3d;
	//mist::array2< float > work2d;
	//mist::array1< float > work1d;

#if TEST_MODE == DCT_TEST
	std::string mode = "dct";
#elif TEST_MODE == DST_TEST
	std::string mode = "dst";
#else
	std::string mode = "fft";
#endif

	std::string fname = "";
	if( argc < 2 )
	{
#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
		fname = "//eagle/data4/CTLung/haigeta/haigeta212.512.gz";
#else
		fname = "/data4/CTLung/haigeta/haigeta212.512.gz";
#endif
	}
	else
	{
		fname = argv[ 1 ];
	}

	mist::read_raw( in, fname, 512, 512, 128 );


	///////////////////////////////////////////////////////////////////////////////////////////
	//1ŽŸŒ³dst
	std::cout << "1D-" << mode << " check( data size " << in.width( ) << " )" << std::endl;
	a1df.resize( in.width( ) );

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		a1df[ i ] = static_cast< float >( in[ i ] );
	}

	{
		std::cout << mode << " start" << std::endl;
		mist::timer t;

#if TEST_MODE == DCT_TEST
		mist::dct( a1df, work1d );
#elif TEST_MODE == DST_TEST
		mist::dst( a1df, work1d );
#else
		mist::fft( a1df, work1d );
#endif

		std::cout << mode << " computation Time: " << t.elapse( ) << " (sec)" << std::endl;
	}

	a1df.fill( 0.0f );

	{
		std::cout << mode << "_inverse start" << std::endl;
		mist::timer t;

#if TEST_MODE == DCT_TEST
		mist::idct( work1d, a1df );
#elif TEST_MODE == DST_TEST
		mist::idst( work1d, a1df );
#else
		mist::ifft( work1d, a1df );
#endif

		std::cout << mode << "_inverse computation Time: " << t.elapse( ) << " (sec)" << std::endl;
	}

	for( i = 0 ; i < in.width( ) ; i++ )
	{
		if( in[ i ] != ( short ) ( a1df[ i ] + 0.5f ) )
		{
			break;
		}
	}

	std::cout << "result: " << ( i == in.width( ) ? "match" : "miss-match" ) << std::endl << std::endl;

	a1df.clear( );
	work1d.clear( );



	///////////////////////////////////////////////////////////////////////////////////////////
	//2ŽŸŒ³dst
	std::cout << "2D-" << mode << " check( data size " << in.width( ) << "x" << in.height( ) << " )" << std::endl;
	a2df.resize( in.width( ), in.height( ) );

	for( i = 0 ; i < in.width( ) * in.height( ) ; i++ )
	{
		a2df[ i ] = static_cast< float >( in[ i ] );
	}

	{
		std::cout << mode << " start" << std::endl;
		mist::timer t;

#if TEST_MODE == DCT_TEST
		mist::dct( a2df, work2d );
#elif TEST_MODE == DST_TEST
		mist::dst( a2df, work2d );
#else
		mist::fft( a2df, work2d );
#endif

		std::cout << mode << " computation Time: " << t.elapse( ) << " (sec)" << std::endl;
	}

	a2df.fill( 0.0f );

	{
		std::cout << mode << "_inverse start" << std::endl;
		mist::timer t;

#if TEST_MODE == DCT_TEST
		mist::idct( work2d, a2df );
#elif TEST_MODE == DST_TEST
		mist::idst( work2d, a2df );
#else
		mist::ifft( work2d, a2df );
#endif

		std::cout << mode << "_inverse computation Time: " << t.elapse( ) << " (sec)" << std::endl;
	}

	for( i = 0 ; i < in.width( ) * in.height( ) ; i++ )
	{
		if( in[ i ] != ( short ) ( a2df[ i ] + 0.5f ) )
		{
			break;
		}
	}

	std::cout << "result: " << ( i == in.width( ) * in.height( ) ? "match" : "miss-match" ) << std::endl << std::endl;

	a2df.clear( );
	work2d.clear( );




	///////////////////////////////////////////////////////////////////////////////////////////
	//3ŽŸŒ³dst
	std::cout << "3D-" << mode << " check( data size " << in.width( ) << "x" << in.height( ) << "x" << in.depth( ) << " )" << std::endl;
	a3df.resize( in.width( ), in.height( ), in.depth( ) );

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		a3df[ i ] = static_cast< float >( in[ i ] );
	}

	{
		std::cout << mode << " start" << std::endl;
		mist::timer t;

#if TEST_MODE == DCT_TEST
		mist::dct( a3df, work3d );
#elif TEST_MODE == DST_TEST
		mist::dst( a3df, work3d );
#else
		mist::fft( a3df, work3d );
#endif

		std::cout << mode << " computation Time: " << t.elapse( ) << " (sec)" << std::endl;
	}

	a3df.fill( 0.0f );

	{
		std::cout << mode << "_inverse start" << std::endl;
		mist::timer t;

#if TEST_MODE == DCT_TEST
		mist::idct( work3d, a3df );
#elif TEST_MODE == DST_TEST
		mist::idst( work3d, a3df );
#else
		mist::ifft( work3d, a3df );
#endif

		std::cout << mode << "_inverse computation Time: " << t.elapse( ) << " (sec)" << std::endl;
	}

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		if( in[ i ] != ( short ) ( a3df[ i ] + 0.5f ) )
		{
			break;
		}
	}

	std::cout << "result: " << ( i == in.size( ) ? "match" : "miss-match" ) << std::endl << std::endl;

	a3df.clear( );
	work3d.clear( );

	return ( 0 );
}
