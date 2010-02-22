// 
// Copyright (c) 2003-2009, MIST Project, Intelligent Media Integration COE, Nagoya University
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

#include <mist/tensor.h>
#include <mist/random.h>

const size_t rank1 = 6;
const size_t rank2 = 5;
const size_t rank3 = 4;
const size_t rank4 = 3;
const size_t rank5 = 2;
mist::array< double > d( rank1 * rank2 * rank3 * rank4 * rank5 );

void second_order_tensor_test( )
{
	std::cout << "****** second order tensor test ******" << std::endl << std::endl;
	mist::array< double > data( rank1 * rank2 );
	std::copy( &d[ 0 ], &d[ 0 ] + data.size( ), &data[ 0 ] );
	mist::array2< double > img( rank1, rank2 );
	std::copy( &d[ 0 ], &d[ 0 ] + img.size( ), &img[ 0 ] );
	mist::matrix< double > mat( rank1, rank2 ); 
	std::copy( &d[ 0 ], &d[ 0 ] + mat.size( ), &mat[ 0 ] );

	mist::tensor< 2, double > t1( rank1, rank2, data );
	mist::tensor< 2, double > t2( img );
	mist::tensor< 2, double > t3( mat );
	mist::tensor< 2, double > t4( rank1, rank2 );
	mist::tensor< 2, double > t5;
	
	if( 
		t1.mode( ) != 2
		|| t1.rank( 1 ) != rank1 || t1.rank( 2 ) != rank2 
		|| t1.size( ) != rank1 * rank2
		|| t1 != t2
		)
	{
		std::cout << "error";
		getchar( );
	}
	
	t1 + t2;
	t1 - t2;
	t1 * 2;
	2 * t1;
	t1 / 2;
	
	mist::tensor< 2, double > t( t1 ), z;
	mist::matrix< double > u1, u2;
	hosvd( t, z, u1, u2 );
	std::cout << "HOSVD: T = T x_1 U1 x_2 U2" << std::endl << std::endl;
	std::cout << "T" << std::endl << t << std::endl << std::endl;
	std::cout << "Z x_1 U1 x_2 U2" << std::endl << z.x( 1, u1 ).x( 2, u2 ) << std::endl;
	std::cout << std::endl;

	mist::matrix< double > a( mat ), u, s, vt;
	mist::svd( a, u, s, vt );
	std::cout << "SVD: A = U S V^t" << std::endl << std::endl;
	std::cout << "A" << std::endl << a.t( ) << std::endl << std::endl;
	std::cout << "U S V^t" << std::endl << ( u * s * vt ).t( ) << std::endl;
	std::cout << std::endl << std::endl;
}

void third_order_tensor_test( )
{
	std::cout << "****** third order tensor test ******" << std::endl << std::endl;
	mist::array< double > data( rank1 * rank2 * rank3 );
	std::copy( &d[ 0 ], &d[ 0 ] + data.size( ), &data[ 0 ] );
	mist::array< double > data2( rank1 * rank2 );
	std::copy( &d[ 0 ], &d[ 0 ] + data2.size( ), &data2[ 0 ] );
	mist::array< mist::tensor< 2, double > > ts( 1 );
	ts[ 0 ] = mist::tensor< 2, double >( rank1, rank2, data2 );
	
	mist::tensor< 3, double > t1( rank1, rank2, rank3, data );
	mist::tensor< 3, double > t2( rank1, rank2, rank3 );
	mist::tensor< 3, double > t3( ts );
	mist::tensor< 3, double > t4;
	
	if( 
		t1.mode( ) != 3 
		|| t1.rank( 1 ) != rank1 || t1.rank( 2 ) != rank2 || t1.rank( 3 ) != rank3 
		|| t1.size( ) != rank1 * rank2 * rank3 
		)
	{
		std::cout << "error";
		getchar( );
	}
	
	t1 + t2;
	t1 - t2;
	t1 * 2;
	2 * t1;
	t1 / 2;
	
	mist::tensor< 3, double > t( t1 ), z;
	mist::matrix< double > u1, u2, u3;
	hosvd( t, z, u1, u2, u3 );
	std::cout << "HOSVD: T = Z x_1 U1 x_2 U2 x_2 U3" << std::endl << std::endl;
	std::cout << "T" << std::endl << t << std::endl << std::endl;
	std::cout << "Z x_1 U1 x_2 U2 x_3 U3" << std::endl << z.x( 1, u1 ).x( 2, u2 ).x( 3, u3 ) << std::endl;
	std::cout << std::endl << std::endl;
}

void forth_order_tensor_test( )
{
	std::cout << "****** forth order tensor test ******" << std::endl << std::endl;
	mist::array< double > data( rank1 * rank2 * rank3 * rank4 );
	std::copy( &d[ 0 ], &d[ 0 ] + data.size( ), &data[ 0 ] );
	mist::array< double > data2( rank1 * rank2 * rank3 );
	std::copy( &d[ 0 ], &d[ 0 ] + data2.size( ), &data2[ 0 ] );
	mist::array< mist::tensor< 3, double > > ts( 1 );
	ts[ 0 ] = mist::tensor< 3, double >( rank1, rank2, rank3, data2 );
	
	mist::tensor< 4, double > t1( rank1, rank2, rank3, rank4, data );
	mist::tensor< 4, double > t2( rank1, rank2, rank3, rank4 );
	mist::tensor< 4, double > t3( ts );
	mist::tensor< 4, double > t4;
	
	if( 
		t1.mode( ) != 4 
		|| t1.rank( 1 ) != rank1 || t1.rank( 2 ) != rank2 || t1.rank( 3 ) != rank3 || t1.rank( 4 ) != rank4 
		|| t1.size( ) != rank1 * rank2 * rank3 * rank4
		)
	{
		std::cout << "error";
		getchar( );
	}
	
	t1 + t2;
	t1 - t2;
	t1 * 2;
	2 * t1;
	t1 / 2;
	
	mist::tensor< 4, double > t( t1 ), z;
	mist::matrix< double > u1, u2, u3, u4;
	hosvd( t, z, u1, u2, u3, u4 );
	std::cout << "HOSVD: T = Z x_1 U1 x_2 U2 x_2 U3 x_4 U4" << std::endl << std::endl;
	std::cout << "T" << std::endl << t << std::endl << std::endl;
	std::cout << "Z x_1 U1 x_2 U2 x_3 U3 x_4 U4" << std::endl << z.x( 1, u1 ).x( 2, u2 ).x( 3, u3 ).x( 4, u4 ) << std::endl;
	std::cout << std::endl << std::endl;
}

void fifth_order_tensor_test( )
{
	std::cout << "****** fifth order tensor test ******" << std::endl << std::endl;
	mist::array< double > data( rank1 * rank2 * rank3 * rank4 * rank5 );
	std::copy( &d[ 0 ], &d[ 0 ] + data.size( ), &data[ 0 ] );
	mist::array< double > data2( rank1 * rank2 * rank3 * rank4 );
	std::copy( &d[ 0 ], &d[ 0 ] + data2.size( ), &data2[ 0 ] );
	mist::array< mist::tensor< 4, double > > ts( 1 );
	ts[ 0 ] = mist::tensor< 4, double >( rank1, rank2, rank3, rank4, data2 );
	
	mist::tensor< 5, double > t1( rank1, rank2, rank3, rank4, rank5, data );
	mist::tensor< 5, double > t2( rank1, rank2, rank3, rank4, rank5 );
	mist::tensor< 5, double > t3( ts );
	mist::tensor< 5, double > t4;
	
	if( 
		t1.mode( ) != 5 
		|| t1.rank( 1 ) != rank1 || t1.rank( 2 ) != rank2 || t1.rank( 3 ) != rank3 || t1.rank( 4 ) != rank4 || t1.rank( 5 ) != rank5 
		|| t1.size( ) != rank1 * rank2 * rank3 * rank4 * rank5
		)
	{
		std::cout << "error";
		getchar( );
	}
	
	t1 + t2;
	t1 - t2;
	t1 * 2;
	2 * t1;
	t1 / 2;
	
	mist::tensor< 5, double > t( t1 ), z;
	mist::matrix< double > u1, u2, u3, u4, u5;
	hosvd( t, z, u1, u2, u3, u4, u5 );
	std::cout << "HOSVD: T = Z x_1 U1 x_2 U2 x_2 U3 x_4 U4 x_5 U5" << std::endl << std::endl;
	std::cout << "T" << std::endl << t << std::endl << std::endl;
	std::cout << "Z x_1 U1 x_2 U2 x_3 U3 x_4 U4 x_5 U5" << std::endl << z.x( 1, u1 ).x( 2, u2 ).x( 3, u3 ).x( 4, u4 ).x( 5, u5 ) << std::endl;
	std::cout << std::endl << std::endl;
}


int main( void )
{
	mist::uniform::random r( 34763786 ); 
	for( size_t i = 0 ; i < d.size( ) ; i ++ )
	{
		d[ i ] = ( static_cast< int >( r.generate( ) ) % 10 ) / 10.0;
	}

	second_order_tensor_test( );
	third_order_tensor_test( );
	forth_order_tensor_test( );
	fifth_order_tensor_test( );

	return 0;
}
