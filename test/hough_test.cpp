// 
// Copyright (c) 2003-2010, Shuichirou Kitou, MIST Project, Nagoya University
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
#include <mist/vector.h>
#include <mist/io/image.h>
#include <mist/filter/edge.h>
#include <mist/filter/hough.h>
#include <mist/drawing.h>

int main( int argc, char * argv[] )
{
	typedef mist::rgb< unsigned char >    pixel_type;
	typedef mist::array2< pixel_type >    color_image_type;
	typedef mist::array2< unsigned char > gray_image_type;
	typedef mist::vector2< double >       vector_type;

	if( argc < 2 )
	{
		std::cerr << "This program requires a path to an input image." << std::endl;
		return( 1 );
	}

	color_image_type orginal_img;
	gray_image_type  canny_img;

	if( !read_image( orginal_img, argv[ 1 ] ) )
	{
		std::cerr << "Could not open the image [" << argv[ 1 ] << "]." << std::endl;
		return( 1 );
	}

	// エッジを検出する
	canny( orginal_img, canny_img, 100, 200 );

	// Hough変換により直線のパラメータを求める
	std::vector< std::complex< double > > lines;
	mist::line::hough_transform( canny_img, lines, 100, 1, 3.14159 / 360.0, 500 );

	// 直線を描画する
	double scale = orginal_img.width( ) > orginal_img.height( ) ? orginal_img.width( ) : orginal_img.height( );
	for( size_t i = 0 ; i < lines.size( ) ; i++ )
	{
		double rho = lines[ i ].real( );
		double theta = lines[ i ].imag( );

		vector_type p1( std::cos( theta ) * rho, std::sin( theta ) * rho );
		vector_type p2( p1.x - rho * std::sin( theta ), p1.y + rho * std::cos( theta ) );
		vector_type d = ( p2 - p1 ).unit( );

		p1 -= d * scale;
		p2 += d * scale;

		int x1 = static_cast< int >( p1.x );
		int y1 = static_cast< int >( p1.y );
		int x2 = static_cast< int >( p2.x );
		int y2 = static_cast< int >( p2.y );

		mist::draw_line( orginal_img, x1, y1, x2, y2, mist::colors< pixel_type >::red( ) );
	}

	write_image( orginal_img, "output.png" );

	return( 0 );
}
