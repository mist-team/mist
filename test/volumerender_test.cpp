#include <mist/io/raw.h>
#include <mist/draw.h>
#include <iostream>
#include "volr_image_window.h"
#include <FL/Fl_File_Chooser.H>
#include <FL/gl_draw.H>

#ifdef WIN32
#include <io.h>
#include <tchar.h>
#else
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#endif
#include <FL/filename.H>


#include <mist/timer.h>
#include <mist/quaternion.h>


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

	progress_callback( Fl_Progress *f = NULL ) : f_( f ){ }
};


int volr_draw_area::handle( int event )
{
	switch( event )
	{
	case FL_PUSH:
		onMouseDown( Fl::event_x( ), Fl::event_y( ) );
		return( 1 );
		break;

	case FL_DRAG:
		onMouseDrag( Fl::event_state( ), Fl::event_x( ), Fl::event_y( ) );
		return( 1 );
		break;

	case FL_RELEASE:
		onMouseUp( Fl::event_x( ), Fl::event_y( ) );
		return( 1 );
		break;

	case FL_ENTER:
		return( 1 );
		break;

	case FL_MOVE:
		return( 1 );
		break;

	default:
		return( Fl_Gl_Window::handle( event ) );
	}
	return( 1 );
}

void volr_draw_area::draw( )
{
	if( draw_flag_ )
	{
		draw_image( );
	}

	static bool is_first = true;
	if( is_first )
	{
		is_first = false;
		gl_font( FL_TIMES_BOLD, 18 );
	}

	mist::draw_image( image_, w( ), h( ) );

	{
		int width = ( int )w( ) / 2;
		int height = ( int )h( ) / 2;
		glMatrixMode( GL_PROJECTION );
		glPushMatrix( );
		glLoadIdentity( );
		gluOrtho2D( -width, width, -height, height );

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix( );
		glLoadIdentity( );

		// 左上を原点とする画像座標系を設定
		gluLookAt( width, height, -1, width, height, 0, 0, -1, 0 );

		glColor3d( 1.0, 1.0, 1.0 );

		char str[ 256 ];
		sprintf( str, "FPS: %.1f", fps_ );
		gl_draw( str, 5, ( int )h( ) - 5 );
	}

	glFlush( );
}

void volr_draw_area::draw_image( )
{
	if( ct.empty( ) )
	{
		return;
	}

	draw_flag_ = false;

	volr_parameter_type p( volr_parameter );
	volr_table_type &table = volr_table;

	if( is_high_resolution_ )
	{
		image_.resize( high_reso_, high_reso_ );
	}
	else
	{
		image_.resize( low_reso_, low_reso_ );
	}

	image_.reso( w( ) / static_cast< double >( h( ) ), 1.0 );

	p.pos.x	= camera_.pos.x - p.offset.x;
	p.pos.y	= p.offset.y - camera_.pos.y;
	p.pos.z	= camera_.pos.z - p.offset.z;
	p.dir	= camera_.dir;
	p.up	= camera_.up;

	{
		mist::timer t;
//		mist::volumerendering( ct, image_, mist::volumerender::no_depth_map( ), barrel_distortion_, p, table );
		mist::volumerendering( ct, image_, mist::volumerender::depth_map< mist::array3< double > >( depth_map ), p, table );
		fps_ = 1.0 / t.elapse( );
	}
}

void volr_draw_area::read_image( volr_image_window *wnd )
{
	const char *filename = fl_file_chooser( "Open", "*", "" );
	if( filename == NULL ) return;

	size_type w = 512;
	size_type h = 512;
	size_type d = 189;
	double x = 0.625;
	double y = 0.625;
	double z = 1.0;
	ct_value_type offset = 0;

	parameter_window window;
	window.width->value( w );
	window.height->value( h );
	window.depth->value( d );
	window.sizeX->value( x );
	window.sizeY->value( y );
	window.sizeZ->value( z );
	window.offset->value( offset );

	if( window.show( ) )
	{
		w = static_cast< size_type >( window.width->value( ) );
		h = static_cast< size_type >( window.height->value( ) );
		d = static_cast< size_type >( window.depth->value( ) );
		x = window.sizeX->value( );
		y = window.sizeY->value( );
		z = window.sizeZ->value( );
		offset = static_cast< ct_value_type >( window.offset->value( ) );

		if( mist::read_raw( ct, filename, w, h, d, x, y, z, offset, false, progress_callback( wnd->progress_bar ) ) )
		{
			initialize( );
		}
	}
}

void volr_draw_area::initialize( )
{
	double ax = ct.reso1( );
	double ay = ct.reso2( );
	double az = ct.reso3( );

	difference_type sx = 0;
	difference_type ex = ct.width( ) - 1;
	difference_type sy = 0;
	difference_type ey = ct.height( ) - 1;
	difference_type sz = 0;
	difference_type ez = ct.depth( ) - 1;

	volr_parameter.offset.x = ( sx + ex ) / 2.0 * ax;
	volr_parameter.offset.y = ( sy + ey ) / 2.0 * ay;
	volr_parameter.offset.z = ( sz + ez ) / 2.0 * az;

	typedef mist::volumerender::boundingbox boundingbox;
	volr_parameter.box[ 0 ] = boundingbox(  0,  0,  1, volr_parameter.offset.z - az * 2.0 );
	volr_parameter.box[ 1 ] = boundingbox(  1,  0,  0, volr_parameter.offset.x - ax * 2.0 );
	volr_parameter.box[ 2 ] = boundingbox(  0,  0, -1, volr_parameter.offset.z - az * 2.0 );
	volr_parameter.box[ 3 ] = boundingbox( -1,  0,  0, volr_parameter.offset.x - ax * 2.0 );
	volr_parameter.box[ 4 ] = boundingbox(  0, -1,  0, volr_parameter.offset.y - ay * 2.0 );
	volr_parameter.box[ 5 ] = boundingbox(  0,  1,  0, volr_parameter.offset.y - ay * 2.0 );

	difference_type offset = 2000;
	volr_table.create( -offset, 7000 );
	//volr_table.append( mist::rgb< double >( 255.0, 128.0, 102.0 ), -800 + offset, 150 + offset, 0.0, 1.0 );
	volr_table.append( mist::rgb< double >( 255.0, 128.0, 102.0 ), -600 + offset, -400 + offset, 0.0, 0.8 );
	volr_table.append( mist::rgb< double >( 255.0, 128.0, 102.0 ), -400 + offset,  150 + offset, 0.8, 0.8 );
	volr_table.append( mist::rgb< double >( 255.0, 255.0, 255.0 ),  150 + offset, 1000 + offset, 1.0, 1.0 );

	// 距離マップを作成する
	mist::generate_depth_map( ct, depth_map, volr_table );

	volr_parameter.fovy					= 80.0;
	volr_parameter.ambient_ratio		= 0.3;
	volr_parameter.diffuse_ratio		= 0.7;
	volr_parameter.light_attenuation	= 0.0;
	volr_parameter.sampling_step		= 1.0;
	volr_parameter.termination			= 0.01;
	volr_parameter.specular				= 0.8;
	volr_parameter.distortion			= 0.0;
	
	//	image_.resize( 512, 512 );
	image_.resize( 256, 256 );
//	image_.resize( 128, 128 );
	image_.reso( w( ) / static_cast< double >( h( ) ), 1.0 );

	camera_.pos.x = ct.width( ) / 2.0 * ct.reso1( );
	camera_.pos.y = ct.height( ) / 2.0 * ct.reso2( );
	camera_.pos.z = 0.0;
	camera_.dir.x = 0.0;
	camera_.dir.y = 0.0;
	camera_.dir.z = 1.0;
	camera_.up.x  = 0.0;
	camera_.up.y  = 1.0;
	camera_.up.z  = 0.0;

	old_camera_ = camera_;
	old_camera_.dir = - ( camera_.pos - volr_parameter.offset ).unit( );
	rotate_camera( 0, 0, 0, 0 );

	draw_flag_ = true;

	redraw( );
	Fl::wait( 0 );
}


void volr_draw_area::rotate_camera( int sx, int sy, int x, int y )
{
	volr_parameter_type &p = volr_parameter;

	double winW = w( );
	double winH = h( );

	double cx = winW / 2.0, cy = winH / 2.0;
	point2 p1( ( sx - cx ) * 2.0 / winW, ( cy - sy ) * 2.0 / winH ), p2( ( x - cx ) * 2.0 / winW, ( cy - y ) * 2.0 / winH );
	point3 tdir  = old_camera_.dir;
	point3 tup   = old_camera_.up;
	point3 tyoko = old_camera_.dir * old_camera_.up;
	mist::quaternion< double > q = mist::track_ball( p1, p2, tyoko, tup, tdir );

	point3 pos = q.rotate( old_camera_.dir ) * ( - zoom_ );
	camera_.pos.x	= pos.x + p.offset.x;
	camera_.pos.y	= p.offset.y - pos.y;
	camera_.pos.z	= pos.z + p.offset.z;
	camera_.dir		= - pos.unit( );
	camera_.up		= q.rotate( old_camera_.up ).unit( );
}

// Mouse button was pressed somewhere
void volr_draw_area::onMouseDown( int x, int y )
{
	drag_beg_x = x;
	drag_beg_y = y;

	old_camera_ = camera_;

	is_high_resolution_ = false;

	draw_flag_ = true;

	redraw( );
	Fl::wait( 0 );
}

// The mouse has moved, draw a line
void volr_draw_area::onMouseDrag( int button, int x, int y )
{
	if( FL_BUTTON1 & button )
	{
		rotate_camera( drag_beg_x, drag_beg_y, x, y );

		draw_flag_ = true;

		redraw( );
		Fl::wait( 0 );
	}
	else if( FL_BUTTON3 & button )
	{
		zoom_ += x - drag_beg_x;
		zoom_ = zoom_ <= 0.1 ? 0.1 : zoom_;
		zoom_ = zoom_ > 10000.0 ? 10000.0: zoom_;
		rotate_camera( 0, 0, 0, 0 );
		drag_beg_x = x;
		drag_beg_y = y;

		draw_flag_ = true;

		redraw( );
		Fl::wait( 0 );
	}
}

// The mouse button was released again
void volr_draw_area::onMouseUp( int x, int y )
{
	old_camera_ = camera_;

	is_high_resolution_ = true;

	draw_flag_ = true;

	redraw( );
	Fl::wait( 0 );
}



void volr_draw_area::write_image( volr_image_window *w )
{
	const char *filename = fl_file_chooser( "Save", "*", "" );
	if( filename == NULL ) return;

	mist::write_raw_gz( ct, filename, 0, false, progress_callback( w->progress_bar ) );
}

int main( int argc, char *argv[] )
{
	volr_image_window window;
	Fl::gl_visual( FL_RGB );
	window.show( );
	Fl::background( 212, 208, 200 );
	Fl::run();

	return( 0 );
}




