#include "image_test.h"
#include "image_test_window.h"


image_draw_area *draw_area;

#include <mist/mist.h>
#include <mist/draw.h>
#include <mist/io/image.h>

//#define __INCLUDE_DICOM_TAG_ALL__	// 全てのDICOMタグをインポートする
#define __SHOW_DICOM_TAG__			// コンソールにインクルードしたタグを表示する
//#define __SHOW_DICOM_UNKNOWN_TAG__	// コンソールに認識出来なかったタグを表示する
#include <mist/io/dicom.h>

#include <mist/filter/distance.h>
#include <mist/filter/figure_decomposition.h>
#include <mist/filter/labeling.h>
#include <mist/filter/thinning.h>
#include <mist/filter/median.h>
#include <mist/filter/morphology.h>
#include <mist/filter/interlace.h>
#include <mist/filter/fusion.h>
#include <mist/interpolate.h>
#include <mist/threshold.h>
#include <mist/timer.h>

#include <mist/converter.h>


#define USE_COLOR_IMAGE		1

#if USE_COLOR_IMAGE == 1
typedef mist::array2< mist::rgb< unsigned char > > image_type;
#else
typedef mist::array2< unsigned char > image_type;
#endif

image_type image_object( 100, 100 );


image_draw_area::image_draw_area( int x, int y, int w, int h, const char *l ) : Fl_Gl_Window( x, y, w, h, l), interpolate_( false )
{
	draw_area = this;
}

image_draw_area::image_draw_area( int x, int y, int w, int h ) : Fl_Gl_Window( x, y, w, h ), interpolate_( false )
{
	draw_area = this;
}

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


void read_image_test( const char *filename )
{
	mist::read_image( image_object, filename );
}

void write_image_test( const char *filename )
{
	mist::write_image( image_object, filename );
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
	mist::array2< int > tmp3( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );
	image_type::size_type i = 0;

	mist::convert( image_object, tmp1 );

	tmp1.reso( 1.0, 2.0 );
	{
		mist::timer t;
		mist::calvin::distance_transform( tmp1, tmp2 );
		std::cout << "Computation time for Calvin: " << t << " sec" << std::endl;
	}
	{
		mist::timer t;
		mist::euclidean::distance_transform( tmp1, tmp3, -1, 1 );
		std::cout << "Computation time for Saito: " << t << " sec" << std::endl;
	}

	double err = 0.0;
	for( i = 0 ; i < tmp2.size( ) ; i++ )
	{
		err += ( tmp2[ i ] - tmp3[ i ] ) * ( tmp2[ i ] - tmp3[ i ] );
	}

	std::cout << "Error: " << err << std::endl;

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
		if( tmp2[ i ] == 0 )
		{
			image_object[ i ] = 0;
		}
		else
		{
			image_object[ i ] = static_cast< unsigned char >( ( tmp2[ i ] - min ) / range * 200.0 + 55.0 );
		}
	}
}

void figure_decomposition_test( )
{
	mist::array2< unsigned char > label( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );

	image_type::size_type i;

	mist::convert( image_object, label );

//	size_t label_num = mist::__distance_figure_dedomposition__::figure_decomposition( image_object, image_object, 255 );
	size_t label_num = mist::__distance_figure_dedomposition__::figure_decomposition( label, label, 255 );

	if( label_num == 0 )
	{
		return;
	}

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		//switch( label[ i ] )
		//{
		//case 1:
		//	image_object[ i ] = 128.0;
		//	break;

		//case 2:
		//	image_object[ i ] = 255.0;
		//	break;

		//default:
		//	image_object[ i ] = 0;
		//	break;
		//}
		image_object[ i ] = static_cast< unsigned char >( static_cast< double >( label[ i ] ) * 255.0 / static_cast< double >( label_num ) );
	}
}

void thresholding_test( )
{
	mist::array2< unsigned char > tmp( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );

	image_type::size_type i;

	mist::convert( image_object, tmp );

	int threshold = mist::discriminant_analysis::threshold( tmp );
	std::cout << "threshold value: " << threshold << std::endl;

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = tmp[ i ] < threshold ? 0 : 255;
	}
}

void labeling4_test( )
{
	mist::array2< unsigned int > label;

	mist::convert( image_object, label );

	size_t label_num = mist::labeling4( label, label, 255 );

	std::cout << "Label Num: " << label_num << std::endl;

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
	mist::array2< unsigned int > label;

	mist::convert( image_object, label );

	size_t label_num = mist::labeling8( label, label, 255 );

	std::cout << "Label Num: " << label_num << std::endl;

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
	mist::array2< unsigned char > label;

	mist::convert( image_object, label );

	mist::thinning( label, label );

	for( image_type::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = label[ i ] * 255;
	}
}

void median_test( )
{
	image_type tmp( image_object );
	{
		mist::timer t;
		mist::median( tmp, image_object, 10 );
		std::cout << "Computation Time: " << t.elapse( ) << " sec." << std::endl;
	}
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

void erosion_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::erosion( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ) );
}

void dilation_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::dilation( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ) );
}

void opening_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::opening( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ) );
}

void closing_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::closing( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ) );
}

void interpolate_test( int mode, bool reso_up )
{
	image_type tmp( image_object );
	image_type::size_type w = reso_up ? tmp.width( ) * 2 : tmp.width( ) / 2;
	image_type::size_type h = reso_up ? tmp.height( ) * 2 : tmp.height( ) / 2;

	switch( mode )
	{
	case 0:
		mist::nearest::interpolate( tmp, image_object, w, h, 1 );
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

void interlace_test( bool is_odd_line )
{
	image_type tmp( image_object );
	mist::interlace( tmp, image_object, is_odd_line );
}


void expand_test( )
{
	mist::array2< unsigned char > original, figure, tmp;
	image_type::size_type size = 5, i;

	mist::convert( image_object, original );

	figure = tmp = original;

	mist::expand_mdt( figure, figure, size );

	for( i = 0 ; i < size ; i++ )
	{
		mist::expand_mdt( tmp, tmp, 1 );
	}

	double err = 0;
	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		err = ( tmp[ i ] - figure[ i ] ) * ( tmp[ i ] - figure[ i ] );
	}
	std::cout << "Error: " << err << std::endl;

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		if( figure[ i ] != 0 )
		{
			image_object[ i ] = original[ i ] != 0 ? 255 : 127;
		}
	}
}


void shrink_test( )
{
	mist::array2< unsigned char > original, figure, tmp;
	image_type::size_type size = 5, i;

	mist::convert( image_object, original );
	mist::convert( image_object, figure );

	figure = tmp = original;

	mist::shrink_mdt( figure, figure, size );

	for( i = 0 ; i < size ; i++ )
	{
		mist::shrink_mdt( tmp, tmp, 1 );
	}

	double err = 0;
	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		err = ( tmp[ i ] - figure[ i ] ) * ( tmp[ i ] - figure[ i ] );
	}
	std::cout << "Error: " << err << std::endl;

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		if( original[ i ] != 0 )
		{
			image_object[ i ] = figure[ i ] == 0 ? 255 : 127;
		}
	}
}
