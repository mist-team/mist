#include "image_test.h"
#include "image_test_window.h"

#include <mist/mist.h>
#include <mist/draw.h>
#include <mist/io/jpeg.h>
#include <mist/io/png.h>
#include <mist/io/tiff.h>
#include <mist/io/bmp.h>
#include <mist/io/raw.h>
#include <mist/io/dicom.h>

mist::array2< unsigned char > image_object( 100, 100 );
//mist::array2< mist::rgb< unsigned char > > image_object( 100, 100 );

void image_draw_area::draw( )
{
	mist::draw_image( image_object, w( ), h( ) );
}



int main( int argc, char *argv[] )
{
	image_test_window window;
	Fl::gl_visual( FL_RGB );
	window.show( );
	Fl::background( 212, 208, 200 );
	Fl::run();
}


void read_jpeg_test( const char *filename )
{
	mist::read_jpeg( image_object, filename );
}

void write_jpeg_test( const char *filename )
{
	mist::write_jpeg( image_object, filename );
}

void read_png_test( const char *filename )
{
	mist::read_png( image_object, filename );
}

void write_png_test( const char *filename )
{
	mist::write_png( image_object, filename );
}

void read_tiff_test( const char *filename )
{
	mist::read_tiff( image_object, filename );
}

void write_tiff_test( const char *filename )
{
	mist::write_tiff( image_object, filename );
}

void read_bmp_test( const char *filename )
{
	mist::read_bmp( image_object, filename );
}

void write_bmp_test( const char *filename )
{
	mist::write_bmp( image_object, filename, 1 );
}

void read_dicom_test( const char *filename )
{
	mist::read_dicom( image_object, filename );
}

void write_dicom_test( const char *filename )
{
//	mist::write_dicom( image_object, filename, 1 );
}