#include <mist/io/raw.h>
#include <mist/draw.h>
#include <iostream>
#include "ct_image_window.h"
#include <FL/Fl_File_Chooser.H>


#include <mist/filter/median.h>
#include <mist/filter/distance.h>
#include <mist/filter/labeling.h>
#include <mist/filter/morphology.h>
#include <mist/timer.h>


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
		f_->value( percent );
		Fl::wait( 0 );
		return( true );
	}

	progress_callback( Fl_Progress *f ) : f_( f ){ }
};


void ct_draw_area::draw( )
{
	mist::draw_image( buff, w( ), h( ) );
}

void ct_draw_area::draw_image( )
{
	if( ct.empty( ) )
	{
		return;
	}

	if( draw_flag_ )
	{
		return;
	}

	draw_flag_ = true;

	if( buff.width( ) != ct.width( ) || buff.height( ) != ct.height( ) )
	{
		buff.resize( ct.width( ), ct.height( ) );
	}
	buff.reso1( ct.reso1( ) );
	buff.reso2( ct.reso2( ) );

	mist::array3< short >::size_type i, j;
	double pixel;
	for( j = 0 ; j < ct.height( ) ; j++ )
	{
		for( i = 0 ; i < ct.width( ) ; i++ )
		{
			pixel = ( ct( i, j, index_ ) - window_level_ ) / window_width_ * 256.0 + 128.0;
			pixel = pixel <   0.0 ?   0.0 : pixel;
			pixel = pixel > 255.0 ? 255.0 : pixel;
			buff( i, j ) = static_cast< unsigned char >( pixel );
		}
	}

	draw_flag_ = false;
}

void ct_draw_area::read_image( ct_image_window *wnd )
{
	const char *filename = fl_file_chooser( "Open", "*", "" );
	if( filename == NULL ) return;

	size_type w = 512;
	size_type h = 512;
	size_type d = 189;
	double x = 0.625;
	double y = 0.625;
	double z = 1.0;

	parameter_window window;
	window.width->value( w );
	window.height->value( h );
	window.depth->value( d );
	window.sizeX->value( x );
	window.sizeY->value( y );
	window.sizeZ->value( z );

	if( window.show( ) )
	{
		w = static_cast< size_type >( window.width->value( ) );
		h = static_cast< size_type >( window.height->value( ) );
		d = static_cast< size_type >( window.depth->value( ) );
		x = window.sizeX->value( );
		y = window.sizeY->value( );
		z = window.sizeZ->value( );

		if( mist::read_raw( ct, filename, w, h, d, x, y, z, 0, false, progress_callback( wnd->progress_bar ) ) )
		{
			wnd->Indx->range( 0, ct.depth( ) - 1 );
		}
		else
		{
			wnd->Indx->range( 0, 0 );
		}
		wnd->Indx->value( 0 );
		change_index( 0 );
	}
}

void ct_draw_area::write_image( ct_image_window *w )
{
	const char *filename = fl_file_chooser( "Save", "*", "" );
	if( filename == NULL ) return;

	mist::write_raw_gz( ct, filename );
}

void ct_draw_area::change_index( size_type index )
{
	if( 0 <= index && index < ct.depth( ) )
	{
		index_ = index;
		draw_image( );
		redraw( );
	}
}

void ct_draw_area::change_window_level( double wl )
{
	window_level_ = wl;
	draw_image( );
	redraw( );
}

void ct_draw_area::change_window_width( double ww )
{
	window_width_ = ww;
	draw_image( );
	redraw( );
}

void ct_draw_area::median_filter1D( ct_image_window *wnd )
{
	mist::array< double > a( 7 ), b( 7 );
	a[ 0 ] = 0.2;
	a[ 1 ] = 4.1;
	a[ 2 ] = 2.5;
	a[ 3 ] = 3.6;
	a[ 4 ] = 2.2;
	a[ 5 ] = 4.3;
	a[ 6 ] = 0.1;

	std::cout << a << std::endl;
	mist::median( a, b, 3 );
	std::cout << b << std::endl;

	mist::array< int > aa( a ), bb( 7 );

	std::cout << aa << std::endl;
	mist::median( aa, bb, 3 );
	std::cout << bb << std::endl;
}

void ct_draw_area::median_filter2D( ct_image_window *wnd )
{
	mist::array2< double > a( 5, 5 ), b( 5, 5 );
	a( 0, 0 ) = 0.1; a( 0, 1 ) = 0.2; a( 0, 2 ) = 0.1; a( 0, 3 ) = 0.6; a( 0, 4 ) = 0.4;
	a( 1, 0 ) = 0.3; a( 1, 1 ) = 4.1; a( 1, 2 ) = 5.2; a( 1, 3 ) = 4.7; a( 1, 4 ) = 0.3;
	a( 2, 0 ) = 0.2; a( 2, 1 ) = 3.5; a( 2, 2 ) = 2.8; a( 2, 3 ) = 3.9; a( 2, 4 ) = 0.8;
	a( 3, 0 ) = 0.4; a( 3, 1 ) = 4.2; a( 3, 2 ) = 5.9; a( 3, 3 ) = 4.2; a( 3, 4 ) = 0.9;
	a( 4, 0 ) = 0.2; a( 4, 1 ) = 0.3; a( 4, 2 ) = 0.1; a( 4, 3 ) = 0.1; a( 4, 4 ) = 0.4;

	std::cout << a << std::endl;
	mist::median( a, b, 3, 1 );
	std::cout << b << std::endl;

	mist::array2< int > aa( a );
	mist::array2< unsigned int > bb;

	std::cout << aa << std::endl;
	mist::median( aa, bb, 3, 1 );
	std::cout << bb << std::endl;
}

void ct_draw_area::median_filter3D( ct_image_window *wnd )
{
	if( ct.empty( ) ) return;

	mist::array3< short > tmp = ct;
	{
		mist::timer t;
		mist::median( tmp, ct, 3, 3, 3, 0, progress_callback( wnd->progress_bar ) );
		std::cout << "Computation Time: " << t.elapse( ) << std::endl;
	}
	redraw( );
	Fl::wait( 0 );
}

void ct_draw_area::euclidean_distance_transform( ct_image_window *wnd )
{
	if( ct.empty( ) ) return;

	mist::array3< double > tmp1 = ct, tmp2 = ct;
	{
		mist::timer t;
		mist::calvin::distance_transform( tmp1, tmp1 );
		std::cout << "Computation time for Calvin: " << t << " sec" << std::endl;
	}
	{
		mist::timer t;
		mist::euclidean::distance_transform( tmp2, tmp2 );
		std::cout << "Computation time for Saito: " << t << " sec" << std::endl;
	}

	double err = 0.0;
	for( size_t i = 0 ; i < tmp1.size( ) ; i++ )
	{
		err += ( tmp1[ i ] - tmp2[ i ] ) * ( tmp1[ i ] - tmp2[ i ] );
		ct[ i ] = static_cast< short >( tmp1[ i ] - tmp2[ i ] );
	}

	std::cout << "Error: " << err << std::endl;

	redraw( );
	Fl::wait( 0 );
}

void ct_draw_area::labeling6( ct_image_window *wnd )
{
	if( ct.empty( ) ) return;

	size_t label_num = mist::labeling6( ct, ct );
	printf( "label_num = %d\n", label_num );

	redraw( );
	Fl::wait( 0 );
}

void ct_draw_area::labeling26( ct_image_window *wnd )
{
	if( ct.empty( ) ) return;

	size_t label_num = mist::labeling26( ct, ct );
	printf( "label_num = %d\n", label_num );

	redraw( );
	Fl::wait( 0 );
}

void ct_draw_area::erosion( ct_image_window *wnd )
{
	{
		mist::timer t;
		mist::erosion( ct, 7 );
		std::cout << "Computation Time: " << t.elapse( ) << std::endl;
	}
}

void ct_draw_area::dilation( ct_image_window *wnd )
{
	{
		mist::timer t;
		mist::dilation( ct, 7 );
		std::cout << "Computation Time: " << t.elapse( ) << std::endl;
	}
}

void ct_draw_area::opening( ct_image_window *wnd )
{
	{
		mist::timer t;
		mist::opening( ct, 7 );
		std::cout << "Computation Time: " << t.elapse( ) << std::endl;
	}
}

void ct_draw_area::closing( ct_image_window *wnd )
{
	{
		mist::timer t;
		mist::closing( ct, 7 );
		std::cout << "Computation Time: " << t.elapse( ) << std::endl;
	}
}

int main( int argc, char *argv[] )
{
	ct_image_window window;
	Fl::gl_visual( FL_RGB );
	window.show( );
	Fl::background( 212, 208, 200 );
	Fl::run();

	return( 0 );
}
