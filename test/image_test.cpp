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

#include "image_test.h"
#include "image_test_window.h"

image_draw_area *draw_area;

#include <mist/mist.h>
#include <mist/draw.h>
#include <mist/bitmap.h>
#include <mist/io/image.h>

#include <mist/filter/distance.h>
#include <mist/filter/decomposition.h>
#include <mist/filter/thinning.h>
#include <mist/filter/labeling.h>
#include <mist/filter/boundary.h>
#include <mist/filter/median.h>
#include <mist/filter/morphology.h>
#include <mist/filter/interlace.h>
#include <mist/filter/fusion.h>
#include <mist/filter/mode.h>
#include <mist/interpolate.h>
#include <mist/threshold.h>
#include <mist/timer.h>

#include <mist/converter.h>

#define __INCLUDE_DICOM_TAG_ALL__	// 全てのDICOMタグをインポートする
#define __SHOW_DICOM_TAG__			// コンソールにインクルードしたタグを表示する
#define __SHOW_DICOM_UNKNOWN_TAG__	// コンソールに認識出来なかったタグを表示する
#define __SHOW_DICOM_ZEROBYTE_TAG__
#include <mist/io/dicom.h>


#define USE_COLOR_IMAGE		1

#if USE_COLOR_IMAGE == 1
typedef mist::rgba< unsigned char > color_type;
#else
typedef unsigned char color_type;
#endif

typedef mist::array2< color_type > image_type;


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
//	mist::draw_pixels( image_object, w( ), h( ), -100, 1 );
	mist::draw_image( image_object, w( ), h( ), 1.0, 0.0, 0.0, 128.0, 128.0, 128.0, interpolate_ );

//	mist::bitmap< 24 > bmp( image_object );
//	mist::draw_pixels( bmp, w( ), h( ), 0, 0 );
//	mist::draw_image( bmp, w( ), h( ), 1.0, 0.0, 0.0, 128.0, 128.0, 128.0, interpolate_ );
}


struct progress_callback
{
	Fl_Progress *f_;
	bool operator()( double percent ) const
	{
		if( percent > 100.0 )
		{
			f_->hide( );
			Fl::wait( 0 );
			return( true );
		}
		else if( !f_->visible( ) )
		{
			f_->show( );
		}
		f_->value( static_cast< float >( percent ) );
		Fl::wait( 0 );
		return( true );
	}

	progress_callback( ){ }
	progress_callback( Fl_Progress *f ) : f_( f ){ }
};

struct fd_progress_callback
{
	/// @brief 図形分割の進行状況を返す
	//!
	//! @param[in] loop      … 現在の繰り返し回数
	//! @param[in] label_num … 現在の最大ラベル番号
	//! @param[in] radius    … 現在処理中の半径
	//! @param[in] in        … 入力画像
	//! @param[in] out       … 出力ラベル画像
	//!
	template < class Array >
	void operator()( size_t loop, size_t label_num, double radius, const Array &in, const Array &out ) const
	{
		std::cerr << "                                                                   \r";
		std::cerr << "looping ... " << loop << ", label = " << label_num << ", radius = " << radius << "\r";
	}
};

struct fd_save_image_callback
{
	std::string path;
	size_t max_label;

	fd_save_image_callback( const std::string pname, size_t mlabel = 1 ) : path( pname ), max_label( mlabel )
	{
	}

	fd_save_image_callback( const fd_save_image_callback &c ) : path( c.path ), max_label( c.max_label )
	{
	}

	template < class Array >
	void operator()( size_t loop, size_t label_num, double radius, const Array &in, const Array &out ) const
	{
		char buff[ 256 ];
		sprintf( buff, "%06ld.bmp", loop );

		Array tmp( in );
		for( size_t i = 0 ; i < in.size( ) ; i++ )
		{
			if( out[ i ] != 0 )
			{
				tmp[ i ] = static_cast< unsigned char >( ( out[ i ] + 1 ) / static_cast< double >( max_label + 1 ) * 255.0 );
			}
			else if( in[ i ] != 0 )
			{
				tmp[ i ] = static_cast< unsigned char >( 1.0 / static_cast< double >( max_label + 1 ) * 255.0 );;
			}
		}

		mist::write_image( tmp, path + buff );

		std::cerr << "                                                                   \r";
		std::cerr << "looping ... " << loop << ", label = " << label_num << ", radius = " << radius << "\r";
	}
};



image_test_window *pwindow;

int main( int argc, char *argv[] )
{
	image_test_window window;

	pwindow = &window;

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

	image_type::size_type i = 0;

	mist::convert( image_object, tmp1 );
	mist::convert( image_object, tmp2 );

	{
		mist::timer t;
		mist::saito::distance_transform( tmp1, tmp1 );
		std::cout << "Computation time for Saito: " << t << " sec" << std::endl;
	}
	{
		mist::timer t;
		mist::calvin::distance_transform( tmp2, tmp2, 1 );
		std::cout << "Computation time for Calvin: " << t << " sec" << std::endl;
	}

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

	double err = 0.0;
	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		err += ( tmp1[ i ] - tmp2[ i ] ) * ( tmp1[ i ] - tmp2[ i ] );
		if( tmp2[ i ] == 0 )
		{
			image_object[ i ] = 0;
		}
		else
		{
			image_object[ i ] = static_cast< unsigned char >( ( tmp2[ i ] - min ) / range * 200.0 + 55.0 );
		}
	}
	std::cout << "Error: " << err << std::endl;
	pwindow->draw_area->redraw( );
}


void euclidean_distance_skeleton_test( )
{
	mist::array2< int > tmp1( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );
	mist::array2< int > tmp2( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );

	image_type::size_type i = 0;

	mist::convert( image_object, tmp1 );

	tmp1.reso( 1.0, 1.0 );
	mist::euclidean::distance_transform( tmp1, tmp2 );

	{
		mist::timer t;
		//mist::skeleton1( tmp2, tmp2 );
		mist::skeleton( tmp2, tmp2 );
		std::cout << "Computation time for Calvin: " << t << " sec" << std::endl;
	}

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
	pwindow->draw_area->redraw( );
}

void figure_decomposition_test( )
{
	mist::array2< unsigned char > label( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );
	mist::array2< unsigned char > tmp( image_object.width( ), image_object.height( ), image_object.reso1( ), image_object.reso2( ) );

	image_type::size_type i;

	mist::convert( image_object, tmp );

	mist::timer t;
	size_t label_num = mist::figure_decomposition( tmp, label );
	//size_t label_num = mist::figure_decomposition( tmp, label, fd_progress_callback( ) );
	//mist::figure_decomposition( tmp, label, fd_save_image_callback( "D:\\hoge\\", label_num ) );
	std::cerr << "Label: " << label_num << ", Computation time: " << t << " sec" << std::endl;

	if( label_num == 0 )
	{
		return;
	}

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		if( label[ i ] == 0 )
		{
			image_object[ i ] = 0;
		}
		else
		{
			image_object[ i ] = static_cast< unsigned char >( label[ i ] / static_cast< double >( label_num ) * 255.0 );
		}
	}
	pwindow->draw_area->redraw( );
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
	pwindow->draw_area->redraw( );
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
	pwindow->draw_area->redraw( );
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
	pwindow->draw_area->redraw( );
}

void boundary4_test( )
{
	mist::array2< unsigned int > label;

	mist::convert( image_object, label );

	size_t label_num = mist::boundary4( label, 255 );

	std::cout << "Label Num: " << label_num << std::endl;

	if( label_num == 0 )
	{
		return;
	}

	for( image_type::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = label[ i ];
	}
	pwindow->draw_area->redraw( );
}

void boundary8_test( )
{
	mist::array2< unsigned int > label;

	mist::convert( image_object, label );

	size_t label_num = mist::boundary8( label, 255 );

	std::cout << "Label Num: " << label_num << std::endl;

	if( label_num == 0 )
	{
		return;
	}

	for( image_type::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = label[ i ];
	}
	pwindow->draw_area->redraw( );
}

void thinning_test( )
{
	mist::array2< unsigned char > label;

	mist::convert( image_object, label );

	mist::euclidean::thinning8( label, label );
//	mist::thinning( label, label );

	for( image_type::size_type i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = label[ i ] * 255;
	}
	pwindow->draw_area->redraw( );
}

void median_test( )
{
	image_type tmp( image_object );
	{
		mist::timer t;
		mist::median( tmp, image_object, 5, 5, progress_callback( pwindow->progress_bar ), 0 );
		std::cout << "Computation Time: " << t.elapse( ) << " sec." << std::endl;
	}
	pwindow->draw_area->redraw( );
}

void mode_test( )
{
	mist::array2< unsigned char > tmp;

	mist::convert( image_object, tmp );

	{
		mist::timer t;
		mist::mode( tmp, 5, progress_callback( pwindow->progress_bar ), 0 );
		std::cout << "Computation Time: " << t.elapse( ) << " sec." << std::endl;
	}

	for( size_t i = 0 ; i < image_object.size( ) ; i++ )
	{
		image_object[ i ] = tmp[ i ] == 0 ? 0 : 255;
	}
	pwindow->draw_area->redraw( );
}

void erosion_test( )
{
	mist::erosion( image_object, 3, progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void dilation_test( )
{
	mist::dilation( image_object, 3, progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void opening_test( )
{
	mist::opening( image_object, 3, progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void closing_test( )
{
	mist::closing( image_object, 3, progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void erosion_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::erosion( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ), progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void dilation_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::dilation( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ), progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void opening_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::opening( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ), progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void closing_triangle_test( )
{
	mist::array2< unsigned char > element( 9, 5 );
	element( 4, 0 ) = 1;
	element( 3, 1 ) = 1; element( 4, 1 ) = 1; element( 5, 1 ) = 1;
	element( 2, 2 ) = 1; element( 3, 2 ) = 1; element( 4, 2 ) = 1; element( 5, 2 ) = 1; element( 6, 2 ) = 1;
	element( 1, 3 ) = 1; element( 2, 3 ) = 1; element( 3, 3 ) = 1; element( 4, 3 ) = 1; element( 5, 3 ) = 1; element( 6, 3 ) = 1; element( 7, 3 ) = 1;
	element( 0, 4 ) = 1; element( 1, 4 ) = 1; element( 2, 4 ) = 1; element( 3, 4 ) = 1; element( 4, 4 ) = 1; element( 5, 4 ) = 1; element( 6, 4 ) = 1; element( 7, 4 ) = 1; element( 8, 4 ) = 1;

	mist::closing( image_object, mist::morphology::create_morphology_structure( element, 4, 2 ), progress_callback( pwindow->progress_bar ), 0 );
	pwindow->draw_area->redraw( );
}

void interpolate_test( int mode, bool reso_up )
{
	image_type tmp( image_object );
	image_type::size_type w = reso_up ? tmp.width( ) * 2 : tmp.width( ) / 2;
	image_type::size_type h = reso_up ? tmp.height( ) * 2 : tmp.height( ) / 2;

	mist::timer t;

	switch( mode )
	{
	case 0:
		mist::nearest::interpolate( tmp, image_object, w, h, 1 );
		break;

	case 1:
		mist::mean::interpolate( tmp, image_object, w, h );
		break;

	case 2:
		mist::linear::interpolate( tmp, image_object, w, h );
		break;

	case 4:
		mist::sinc::interpolate( tmp, image_object, w, h );
		break;

	case 5:
		mist::BSpline::interpolate( tmp, image_object, w, h );
		break;

	case 3:
	default:
		mist::cubic::interpolate( tmp, image_object, w, h );
		break;
	}

	std::cout << "Computation Time: " << t << " sec." << std::endl;
}

void interlace_test( bool is_odd_line )
{
	image_type tmp( image_object );
	mist::interlace( tmp, image_object, is_odd_line );
	pwindow->draw_area->redraw( );
}


void expand_test( )
{
	mist::array2< unsigned char > original, figure, tmp;
	image_type::size_type size = 3, i;

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
		err += tmp[ i ] != figure[ i ] ? 1 : 0;
	}
	std::cout << "Error: " << err << std::endl;

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
//		if( tmp[ i ] != 0 )
		if( figure[ i ] != 0 )
		{
			image_object[ i ] = original[ i ] != 0 ? 255 : 127;
		}
	}
	pwindow->draw_area->redraw( );
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
		err += tmp[ i ] != figure[ i ] ? 1 : 0;
	}
	std::cout << "Error: " << err << std::endl;

	for( i = 0 ; i < image_object.size( ) ; i++ )
	{
		if( original[ i ] != 0 )
		{
			image_object[ i ] = figure[ i ] == 0 ? 255 : 127;
		}
	}
	pwindow->draw_area->redraw( );
}
