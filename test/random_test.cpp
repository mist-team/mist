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
#include <ctime>

#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/random.h>
#include <mist/statistics.h>


int main( int argc, char *argv[] )
{
	typedef mist::array< double >::size_type size_type;
	size_type num = 100000;



	
	mist::array< double > data( num );




	std::cout << "***** mist::uniform::random test *****" << std::endl;

	mist::uniform::random u_rnd( std::clock( ) );

	for( size_t i = 0 ; i < num ; i++ )
	{
		data[ i ] = u_rnd.real1( );
	}

	std::cout << "Mean:     " << mist::statistics::average( data ) << std::endl;
	std::cout << "Variance: " << mist::statistics::variance( data ) << std::endl << std::endl;




	std::cout << "***** mist::gauss::random test *****" << std::endl;

	double mean = 0.0;
	double variance = 1.0;
	mist::gauss::random g_rnd( std::clock( ), mean, variance );

	for( size_t i = 0 ; i < num ; i++ )
	{
		data[ i ] = g_rnd.generate( );
	}

	std::cout << "Mean:     " << mist::statistics::average( data ) << std::endl;
	std::cout << "Variance: " << std::sqrt( mist::statistics::variance( data ) ) << std::endl << std::endl;




	mist::array< mist::matrix< double > > vec_data( num );	



	std::cout << "***** mist::multivariate_gauss::random test *****" << std::endl;



	mist::matrix< double > mean_vec = mist::matrix< double >::_21( 0.0, 0.0 );
	mist::matrix< double > covariance_mat = mist::matrix< double >::_22( 1.0, 0.0, 0.0, 1.0 );
	mist::multivariate_gauss::random mg_rnd( std::clock( ), mean_vec, covariance_mat );

	for( size_t i = 0 ; i < num ; i++ )
	{
		vec_data[ i ] = mg_rnd.generate( );
	}

	mist::matrix< double > m( mean_vec.rows( ), 1 );
	for( size_t i = 0 ; i != vec_data.size( ) ; ++ i )
	{
		m += vec_data[ i ];
	}
	m /= vec_data.size( );
	std::cout << "Mean vector:" << std::endl << m << std::endl;
	mist::matrix< double > cov( covariance_mat.rows( ), num );
	
	for( size_t i = 0 ; i != vec_data.size( ) ; ++ i )
	{
		vec_data[ i ] -= m;
		for( size_t r = 0 ; r != cov.rows( ) ; ++ r )
		{
			cov( r, i ) = vec_data[ i ]( r, 0 );
		}
	}
	cov *= cov.t( );
	cov /= vec_data.size( );
	std::cout << "Covariance matrix: " << std::endl << cov << std::endl;


	return( 0 );
}
