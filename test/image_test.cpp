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
#include <mist/filter/distance.h>
//#include <mist/filter/figure_decomposition.h>
#include <mist/filter/labeling.h>
#include <mist/filter/thinning.h>


mist::array2< unsigned char > image_object( 100, 100 );
//mist::array2< mist::rgb< unsigned char > > image_object( 100, 100 );

void image_draw_area::draw( )
{
	mist::draw_image( image_object, w( ), h( ), 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, interpolate_ );
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

void euclidean_distance_transform_test( )
{
	{
		mist::array2< double > tmp( image_object );
		mist::euclidean_distance_transform( tmp, image_object );
	}
}

void figure_decomposition_test( )
{
	//mist::__distance_figure_dedomposition__::figure_decomposition( image_object, image_object, 255 );

	for( mist::array2< unsigned char >::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		switch( image_object[i] )
		{
		case 1:
			image_object[i] = 50;
			break;

		case 2:
			image_object[i] = 100;
			break;

		case 3:
			image_object[i] = 150;
			break;

		case 4:
			image_object[i] = 200;
			break;

		case 5:
			image_object[i] = 255;
			break;
		}
	}
}

void labeling4_test( )
{
	mist::labeling4( image_object, image_object, 255 );

	for( mist::array2< unsigned char >::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		switch( image_object[i] )
		{
		case 1:
			image_object[i] = 50;
			break;

		case 2:
			image_object[i] = 100;
			break;

		case 3:
			image_object[i] = 150;
			break;

		case 4:
			image_object[i] = 200;
			break;

		case 5:
			image_object[i] = 255;
			break;
		}
	}
}

void labeling8_test( )
{
	mist::labeling8( image_object, image_object, 255 );

	for( mist::array2< unsigned char >::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		switch( image_object[i] )
		{
		case 1:
			image_object[i] = 50;
			break;

		case 2:
			image_object[i] = 100;
			break;

		case 3:
			image_object[i] = 150;
			break;

		case 4:
			image_object[i] = 200;
			break;

		case 5:
			image_object[i] = 255;
			break;
		}
	}
}


void thinning_test( )
{
	mist::thinning( image_object, image_object );

	for( mist::array2< unsigned char >::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[i] *= 255;
	}
}