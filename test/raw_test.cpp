#include <mist/io/raw.h>
#include <mist/draw.h>
#include <iostream>
#include "ct_image_window.h"
#include <FL/Fl_File_Chooser.H>


#include <mist/filter/median_filter.h>



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

		if( mist::read_raw( ct, filename, w, h, d, x, y, z ) )
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
}

void ct_draw_area::change_index( size_type index )
{
	if( 0 <= index && index < ct.depth( ) )
	{
		index_ = index;
		draw_image( );
		redraw( );
		Fl::wait( 0 );
	}
}

void ct_draw_area::change_window_level( double wl )
{
	window_level_ = wl;
	draw_image( );
	redraw( );
	Fl::wait( 0 );
}

void ct_draw_area::change_window_width( double ww )
{
	window_width_ = ww;
	draw_image( );
	redraw( );
	Fl::wait( 0 );
}

void ct_draw_area::median_filter( ct_image_window *wnd )
{
	if( ct.empty( ) ) return;

	mist::array3< short > tmp = ct;
	mist::__median_filter__::median_filter( tmp, ct, 3, 3, 3, 0, 2000 );
	redraw( );
	Fl::wait( 0 );
}

int main( int argc, char *argv[] )
{
	ct_image_window window;
	Fl::gl_visual( FL_RGB );
	window.show( );
	Fl::background( 212, 208, 200 );
	Fl::run();
}
