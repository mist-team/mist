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

#include <mist/active_search.h>
#include <mist/io/bmp.h>
#include <mist/interpolate.h>
#include <mist/drawing.h>
#include <mist/timer.h>


static const char *input_filename = "test.bmp";

struct f_rgb : std::unary_function< mist::rgb< unsigned char >, unsigned int >
{
	typedef mist::rgb< unsigned char >	argument_type;
	typedef unsigned int				result_type;
	unsigned int l_;
	unsigned int nob_;
	f_rgb( const unsigned int &nob ) : l_( ( 256 % nob == 0 ) ? ( 256 / nob ) : ( 256 / ( nob - 1 ) ) ), nob_( nob )
	{
	}
	result_type operator ( )( const argument_type &v ) const
	{
		return ( v.r / l_ ) * nob_ * nob_ + ( v.g / l_ ) * nob_ + ( v.b / l_ );
	}
};

struct f_uchar : std::unary_function< unsigned char, unsigned char >
{
	typedef unsigned char	argument_type;
	typedef unsigned char	result_type;
	f_uchar( )
	{
	}
	const result_type &operator ( )( const argument_type &v ) const
	{
		return v;
	}
};


template< typename Value_type >
void write_result( const mist::array2< Value_type > &in, const mist::array2< Value_type > &ref, mist::array2< Value_type > &out, const size_t x, const size_t y, const double scale, const Value_type &line_color )
{
	out = in;
	const size_t h = static_cast< size_t >( ref.height( ) * scale );
	const size_t w = static_cast< size_t >( ref.width( ) * scale );
	mist::draw_line( out, x,     y,     x + w, y,     line_color );
	mist::draw_line( out, x,     y,     x,     y + h, line_color );
	mist::draw_line( out, x + w, y,     x + w, y + h, line_color );
	mist::draw_line( out, x,     y + h, x + w, y + h, line_color );
}



int main( void )
{
	mist::timer t;
	unsigned int x, y, z;
	double scale, sim;
	double max_scale, min_scale, scale_factor;
	size_t num_of_bins;



	std::cout << std::endl << std::endl << std::endl << "*** array2< rgb< unsigned char > > ( x: 100, y: 100, scale: 1.0 ) ***" << std::endl << std::endl;

	mist::array2< mist::rgb< unsigned char > > tmp_rgb, in_rgb, ref_rgb( 100, 100 );
	mist::read_bmp( tmp_rgb, input_filename );
	mist::linear::interpolate( tmp_rgb, in_rgb, 300, 300 );
	tmp_rgb.clear( );
	for( size_t j = 0 ; j < ref_rgb.height( ) ; j ++ )
	{
		for( size_t i = 0 ; i < ref_rgb.width( ) ; i ++ )
		{
			ref_rgb( i, j ) = in_rgb( 100 + i, 100 + j );
		}
	}

	scale = 1.0; num_of_bins = 16;
	sim = 0.0;
	t.reset( );
	mist::active_search( in_rgb, ref_rgb, x, y, sim, scale, num_of_bins );
	std::cout << "x: " << x << " y: " << y << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 16;
	sim = 0.0;
	t.reset( );
	mist::active_search( in_rgb, ref_rgb, x, y, scale, sim, min_scale, max_scale, scale_factor, num_of_bins );
	std::cout << "x: " << x << " y: " << y << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	//mist::array2< mist::rgb< unsigned char > > out;
	//write_result( in_rgb, ref_rgb, out, x, y, scale, mist::rgb< unsigned char >( 255, 0, 0 ) );
	//mist::write_bmp( out, "out.bmp" );




	mist::array2< unsigned char > tmp2;
	mist::read_bmp( tmp2, input_filename );


	std::cout << std::endl << std::endl << std::endl << "*** array< unsigned char > ( x: 50000 scale: 1.0 )" << std::endl << std::endl;

	mist::array< unsigned char > tmp0( tmp2.size( ) ), in0, ref0( 10000 );
	for( size_t i = 0 ; i < tmp0.size( ) ; i ++ )
	{
		tmp0[ i ] = tmp2[ i ];
	}
	mist::linear::interpolate( tmp0, in0, 100000 );
	tmp0.clear( );
	for( size_t i = 0 ; i < ref0.size( ) ; i ++ )
	{
		ref0[ i ] = in0[ 50000 + i ];
	}
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in0, ref0, x, sim, scale, num_of_bins );
	std::cout << "x: " << x << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
		
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in0, ref0, x, scale, sim, min_scale, max_scale, scale_factor, num_of_bins );
	std::cout << "x: " << x << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	

	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in0, ref0, x, sim, scale, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in0, ref0, x, sim, scale, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in0, ref0, x, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in0, ref0, x, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;




	std::cout << std::endl << std::endl << std::endl << "*** array1< unsigned char > ( x: 50000 scale: 1.0 )" << std::endl << std::endl;

	mist::array1< unsigned char > tmp1( tmp2.size( ) ), in1, ref1( 10000 );
	for( size_t i = 0 ; i < tmp1.size( ) ; i ++ )
	{
		tmp1[ i ] = tmp2[ i ];
	}
	mist::linear::interpolate( tmp1, in1, 100000 );
	tmp1.clear( );
	for( size_t i = 0 ; i < ref1.size( ) ; i ++ )
	{
		ref1[ i ] = in1[ 50000 + i ];
	}
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in1, ref1, x, sim, scale, num_of_bins );
	std::cout << "x: " << x << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
		
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in1, ref1, x, scale, sim, min_scale, max_scale, scale_factor, num_of_bins );
	std::cout << "x: " << x << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	

	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in1, ref1, x, sim, scale, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in1, ref1, x, sim, scale, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in1, ref1, x, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in1, ref1, x, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;




	std::cout << std::endl << std::endl << std::endl << "*** array2< unsigned char > ( x: 100 y: 100 scale: 1.0 )  ***" << std::endl << std::endl;

	mist::array2< unsigned char > in2, ref2( 100, 100 );
	mist::read_bmp( tmp2, input_filename );
	mist::linear::interpolate( tmp2, in2, 300, 300 );
	for( size_t j = 0 ; j < ref2.height( ) ; j ++ )
	{
		for( size_t i = 0 ; i < ref2.width( ) ; i ++ )
		{
			ref2( i, j ) = in2( 100 + i, 100 + j );
		}
	}
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in2, ref2, x, y, sim, scale, num_of_bins );
	std::cout << "style: memory" << " x: " << x << " y: " << y << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in2, ref2, x, y, scale, sim, min_scale, max_scale, scale_factor, num_of_bins );
	std::cout << "style: memory" << " x: " << x << " y: " << y << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;

	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in2, ref2, x, y, sim, scale, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " y: " << y << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in2, ref2, x, y, sim, scale, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " y: " << y << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in2, ref2, x, y, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " y: " << y << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in2, ref2, x, y, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " y: " << y << " scale: " << scale << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	



	std::cout << std::endl << std::endl << std::endl << "*** array3< unsigned char > ( x: 20 y: 20 z: 20 scale: 1.0 )" << std::endl << std::endl;

	mist::array3< unsigned char > tmp3( tmp2.width( ), tmp2.height( ), 60 ), in3, ref3( 20, 20, 20 );
	for( size_t k = 20 ; k < 40 ; k ++ )
	{
		for( size_t j = 0 ; j < tmp3.height( ) ; j ++ )
		{
			for( size_t i = 0 ; i < tmp3.width( ) ; i ++ )
			{
				tmp3( i, j, k ) = tmp2( i, j );
			}
		}
	}
	mist::linear::interpolate( tmp3, in3, 60, 60, 60 );
	tmp3.clear( );
	for( size_t k = 0 ; k < ref3.depth( ) ; k ++ )
	{
		for( size_t j = 0 ; j < ref3.height( ) ; j ++ )
		{
			for( size_t i = 0 ; i < ref3.width( ) ; i ++ )
			{
				ref3( i, j, k ) = in3( 20 + i, 20 + j, 20 + k );
			}
		}
	}
	
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in3, ref3, x, y, z, sim, scale, num_of_bins );
	std::cout << "x: " << x << " y: " << y << " z: " << z << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
		
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in3, ref3, x, y, z, scale, sim, min_scale, max_scale, scale_factor, num_of_bins );
	std::cout << "x: " << x << " y: " << y << " z: " << z << " scale: " << scale <<" similarity: " << sim << " ( time: " << t << " )" << std::endl;
		

	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in3, ref3, x, y, z, sim, scale, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " y: " << y << " z: " << z << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	scale = 1.0; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in3, ref3, x, y, z, sim, scale, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " y: " << y << " z: " << z << " similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in3, ref3, x, y, z, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ) );
	std::cout << "style: memory" << " x: " << x << " y: " << y << " z: " << z << " scale: " << scale <<" similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	max_scale = 0.5; min_scale = 1.5; scale_factor = 1.1; num_of_bins = 256;
	sim = 0.0;
	t.reset( );
	mist::active_search( in3, ref3, x, y, z, scale, sim, min_scale, max_scale, scale_factor, num_of_bins, f_uchar( ), mist::active_search_style::speed );
	std::cout << "style: speed " << " x: " << x << " y: " << y << " z: " << z << " scale: " << scale <<" similarity: " << sim << " ( time: " << t << " )" << std::endl;
	
	return 0;
}