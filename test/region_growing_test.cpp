#include <iostream>
#include <mist/mist.h>
#include <mist/io/bmp.h>
#include <mist/filter/region_growing.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	typedef mist::array2< unsigned int > image_type;
	typedef image_type::size_type size_type;
	typedef image_type::difference_type difference_type;

	image_type input( 100, 100 ), output;
	difference_type w = input.width( );
	difference_type h = input.height( );

	input.fill( 255 );

	for( difference_type j = 0 ; j < h ; j++ )
	{
		double ry = j - h / 2.0;

		for( difference_type i = 0 ; i < w ; i++ )
		{
			double rx = i - w / 2.0;

			if( std::abs( rx ) + std::abs( ry ) <= w * 0.4 )
			//if( rx * rx + ry * ry <= w * w  / 5.0 )
			{
				input( i, j ) = 0;
			}
		}
	}

	mist::region_growing_utility::point_type s( w / 2, h / 2, 0 );
	mist::region_growing( input, output, s, 128, mist::region_growing_utility::circle( 20 ), mist::region_growing_utility::Range< unsigned char >( 0, 128 ) );

	mist::write_bmp( output, "hoge.bmp" );

	return( 0 );
}
