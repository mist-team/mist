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
#include <mist/filter/median.h>
#include <mist/filter/morphology.h>
#include <mist/interpolate.h>
#include <mist/threshold.h>

//typedef mist::array2< unsigned char > image_type;
typedef mist::array2< mist::rgb< unsigned char > > image_type;
image_type image_object( 100, 100 );

void image_draw_area::draw( )
{
	mist::draw_image( image_object, w( ), h( ), 1.0, 0.0, 0.0, 128.0, 128.0, 128.0, interpolate_ );
}



int main( int argc, char *argv[] )
{
	image_test_window window;
	Fl::gl_visual( FL_RGB );
	window.show( );
	Fl::background( 212, 208, 200 );
	Fl::run();
	return( 0 );
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
	mist::array2< int > tmp1( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );
	mist::array2< int > tmp2( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );
	image_type::size_type i = 0;
	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		tmp1[ i ] = image_object[ i ].get_value( );
	}

	mist::euclidean_distance_transform( tmp1, tmp2 );

	double min = tmp2[ 0 ];
	double max = tmp2[ 0 ];
	for( i = 1 ; i < tmp2.size( ) ; i++ )
	{
		if( min > tmp2[ i ] )
		{
			min = tmp2[ i ];
		}
		else if( max < tmp2[ i ] )
		{
			max = tmp2[ i ];
		}
	}

	double range = max == min ? 1 : max - min; 

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = static_cast< unsigned char >( ( tmp2[ i ] - min ) / range * 255.0 );
	}
}

void figure_decomposition_test( )
{
	//mist::array2< unsigned char > label( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );

	//image_type::size_type i;

	//for( i = 0 ; i < image_object.size( ) ; i++ )
	//{
	//	label[ i ] = image_object[ i ].get_value( );
	//}

	//size_t label_num = mist::__distance_figure_dedomposition__::figure_decomposition( label, label, 255 );

	//if( label_num == 0 )
	//{
	//	return;
	//}

	//for( i = 0 ; i < image_object.size( ) ; i++ )
	//{
	//	image_object[ i ] = static_cast< unsigned char >( label[ i ] * 255.0 / static_cast< double >( label_num ) );
	//}
}

void thresholding_test( )
{
	mist::array2< unsigned char > tmp( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );

	image_type::size_type i;

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		tmp[ i ] = image_object[ i ].get_value( );
	}

	int threshold = mist::threshold( tmp );
	std::cout << "threshold value: " << threshold << std::endl;

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = tmp[ i ] < threshold ? 0 : 255;
	}
}

void labeling4_test( )
{
	mist::array2< unsigned char > label;
	size_t label_num = mist::labeling4( image_object, label, 255 );

	if( label_num == 0 )
	{
		return;
	}

	for( image_type::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = static_cast< unsigned char >( label[ i ] * 255.0 / static_cast< double >( label_num ) );
	}
}

void labeling8_test( )
{
	mist::array2< unsigned char > label;
	size_t label_num = mist::labeling8( image_object, label, 255 );

	if( label_num == 0 )
	{
		return;
	}

	for( image_type::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = static_cast< unsigned char >( label[ i ] * 255.0 / static_cast< double >( label_num ) );
	}
}


void thinning_test( )
{
	mist::thinning( image_object, image_object );

	for( image_type::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[i] *= 255;
	}
}

void median_test( )
{
	image_type tmp( image_object );
	mist::median( tmp, image_object, 3 );
}

void erosion_test( )
{
	mist::erosion( image_object, 3 );
}

void dilation_test( )
{
	mist::dilation( image_object, 3 );
}

void opening_test( )
{
	mist::opening( image_object, 3 );
}

void closing_test( )
{
	mist::closing( image_object, 3 );
}

void interpolate_test( int mode, bool reso_up )
{
	image_type tmp( image_object );
	image_type::size_type w = reso_up ? tmp.width( ) * 2 : tmp.width( ) / 2;
	image_type::size_type h = reso_up ? tmp.height( ) * 2 : tmp.height( ) / 2;

	switch( mode )
	{
	case 0:
		mist::nearest::interpolate( tmp, image_object, w, h );
		break;

	case 2:
		mist::linear::interpolate( tmp, image_object, w, h );
		break;

	case 1:
	default:
		mist::cubic::interpolate( tmp, image_object, w, h );
		break;
	}
}



