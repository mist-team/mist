#include "image_test.h"
#include "image_test_window.h"
#include <mist/mist.h>
#include <mist/draw.h>
#include <mist/io/jpeg.h>

mist::array2< mist::rgb< unsigned char > > image_object( 100, 100 );

void image_draw_area::draw( )
{
	mist::draw_image( image_object, w( ), h( ) );
}



int main( int argc, char *argv[] )
{
	image_test_window window;
	Fl::gl_visual(FL_RGB);
	window.show( );
	Fl::background( 212, 208, 200 );
	Fl::run();
}


void read_jpeg_test( const std::string &filename )
{
	mist::read_jpeg( image_object, filename );
	mist::write_jpeg( image_object, filename + "jpg" );
}
