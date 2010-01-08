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

#include <mist/io/raw.h>
#include <mist/draw.h>
#include <iostream>
#include "volr_image_window.h"
#include <FL/Fl_File_Chooser.H>
#include <FL/gl_draw.H>

#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0
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

	case FL_FOCUS:
	case FL_UNFOCUS:
		return( 1 );
		break;

	case FL_KEYDOWN:
		onKeyDown( Fl::event_key( ) );
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

	if( volr_parameter.perspective_view )
	{
		image_.reso( w( ) / static_cast< double >( h( ) ), 1.0 );
	}
	else
	{
		double W = static_cast< double >( ct.width( ) );
		double H = static_cast< double >( ct.height( ) );
		double D = static_cast< double >( ct.depth( ) );
		double len = std::sqrt( W * W + H * H + D * D );
		double reso = len / static_cast< double >( image_.width( ) ) / std::sqrt( 3.0 );
		image_.reso( w( ) / static_cast< double >( h( ) ) * reso, 1.0 * reso );
	}

	p.pos = camera_.pos;
	p.dir = camera_.dir;
	p.up  = camera_.up;

	{
		mist::timer t;
//		mist::volumerendering( ct, image_, mist::volumerender::no_depth_map( ), p, table );
		mist::volumerendering( ct, image_, mist::volumerender::depth_map< mist::array3< double > >( depth_map ), p, table );
//		mist::specialized::volumerendering( ct, image_, mist::volumerender::depth_map< mist::array3< double > >( depth_map ), p, table );
//		mist::specialized::volumerendering( ct, image_, mist::volumerender::no_depth_map( ), p, table, 1 );
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
	window.width->value( static_cast< double >( w ) );
	window.height->value( static_cast< double >( h ) );
	window.depth->value( static_cast< double >( d ) );
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

	volr_parameter.perspective_view		= true;
	volr_parameter.mirror_view			= mirror_view;
	volr_parameter.value_interpolation	= true;
	volr_parameter.fovy					= 80.0;
	volr_parameter.ambient_ratio		= 0.3;
	volr_parameter.diffuse_ratio		= 0.7;
	volr_parameter.light_attenuation	= 0.0;
	volr_parameter.sampling_step		= 1.0;
	volr_parameter.termination			= 0.01;
	volr_parameter.specular				= 0.8;

	
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
	camera_.up.y  = -1.0;
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

	double cx = w( ) / 2.0, cy = h( ) / 2.0;
	point2 p1( sx / cx - 1.0, 1.0 - sy / cy ), p2( x / cx - 1.0, 1.0 - y / cy );

	if( mirror_view )
	{
		p1.x = -p1.x;
		p2.x = -p2.x;
	}

	point3 tdir  = old_camera_.dir;
	point3 tup   = old_camera_.up;
	point3 tyoko = old_camera_.dir * old_camera_.up;
	mist::quaternion< double > q = mist::track_ball( p1, p2, tyoko, tup, tdir );

	point3 pos = q.rotate( old_camera_.dir ) * ( - zoom_ );
	camera_.pos.x = pos.x + p.offset.x;
	camera_.pos.y = pos.y + p.offset.y;
	camera_.pos.z = pos.z + p.offset.z;
	camera_.dir = - pos.unit( );
	camera_.up  = q.rotate( old_camera_.up ).unit( );
}

void volr_draw_area::rotate_camera( int x, int y, double step )
{
	double cx = w( ) / 2.0, cy = h( ) / 2.0;
	point2 p1( 0, 0 ), p2( x / cx - 1.0, 1.0 - y / cy );

	if( mirror_view )
	{
		p1.x = -p1.x;
		p2.x = -p2.x;
	}

	point3 tdir  = old_camera_.dir;
	point3 tup   = old_camera_.up;
	point3 tyoko = old_camera_.dir * old_camera_.up;
	mist::quaternion< double > q = mist::track_ball( p1, p2, tyoko, tup, tdir, 5.0 );

	camera_.dir    = q.rotate( old_camera_.dir ).unit( );
	camera_.pos.x += old_camera_.dir.x * step;
	camera_.pos.y += old_camera_.dir.y * step;
	camera_.pos.z += old_camera_.dir.z * step;
	camera_.up     = q.rotate( old_camera_.up ).unit( );
}

void volr_draw_area::move_camera( double x, double y, double z )
{
	point3 tdir  = camera_.dir;
	point3 tup   = camera_.up;
	point3 tyoko = camera_.dir * camera_.up;

	if( mirror_view )
	{
		tyoko = -tyoko;
	}

	point3 pos = tyoko * x + tup * y + tdir * z;
	camera_.pos += pos;
}


// Mouse button was pressed somewhere
void volr_draw_area::onKeyDown( int key )
{
	double step = 1.0;
	point3 yoko = camera_.dir * camera_.up;

	switch( key )
	{
	case FL_Down:
		move_camera( 0.0, -step, 0.0 );
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
		break;

	case FL_Up:
		move_camera( 0.0, step, 0.0 );
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
		break;

	case FL_Left:
		move_camera( -step, 0.0, 0.0 );
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
		break;

	case FL_Right:
		move_camera( step, 0.0, 0.0 );
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
		break;

	}
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

	if( inside_mode_ )
	{
		Fl::remove_timeout( __onInsideViewCallBack__, this );
		Fl::add_timeout( 0.1, __onInsideViewCallBack__, this );
	}
}

// The mouse has moved, draw a line
void volr_draw_area::onMouseDrag( int button, int x, int y )
{
	if( inside_mode_ )
	{
	}
	else
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
}

// The mouse button was released again
void volr_draw_area::onMouseUp( int x, int y )
{
	int state = Fl::event_state( );

	old_camera_ = camera_;

	is_high_resolution_ = true;

	if( state & FL_BUTTON1 || state & FL_BUTTON3 )
	{
		is_high_resolution_ = false;
	}
	else if( inside_mode_ )
	{
		Fl::remove_timeout( __onInsideViewCallBack__, this );
	}

	draw_flag_ = true;

	redraw( );
	Fl::wait( 0 );
}

void volr_draw_area::onInsideViewCallBack( )
{
	old_camera_ = camera_;

//	std::cout << camera_.dir << ", " << camera_.up << std::endl;

	int state = Fl::event_state( );

	if( state & FL_ALT )
	{
		rotate_camera( Fl::event_x( ), Fl::event_y( ), 0.0 );
	}
	else
	{
		double step = 1.0;
		if( state & FL_BUTTON3 )
		{
			step = -1.0;
		}

		if( state & FL_CTRL )
		{
			step *= 5.0;
		}

		if( state & FL_SHIFT )
		{
			step *= 2.0;
		}

		rotate_camera( Fl::event_x( ), Fl::event_y( ), step );
	}

	draw_flag_ = true;

	redraw( );
	Fl::wait( 0 );
	Fl::repeat_timeout( 0.1, __onInsideViewCallBack__, this );
}

void volr_draw_area::write_image( volr_image_window *w )
{
	const char *filename = fl_file_chooser( "Save", "*", "" );
	if( filename == NULL ) return;

	mist::write_raw_gz( ct, filename, 0, false, progress_callback( w->progress_bar ) );
}

void volr_draw_area::render_test( volr_image_window *w )
{
	volr_parameter_type p( volr_parameter );
	volr_table_type &table = volr_table;
	mist::array2< mist::rgb< unsigned char > > timage( 32, 32 );

	p.pos = camera_.pos;
	p.dir = camera_.dir;
	p.up  = camera_.up;

	{
		mist::timer t;
	
		for( int i = 0 ; i < 100 ; i++ )
		{
			mist::specialized::volumerendering( ct, timage, mist::volumerender::depth_map< mist::array3< double > >( depth_map ), p, table, 1 );
		}

		std::cout << "Rendering Test [1CPU] : " << t.elapse( ) * 10.0 << " msec." << std::endl;
	}

	int cpunum = mist::get_cpu_num( );

	{
		mist::timer t;
	
		for( int i = 0 ; i < 100 ; i++ )
		{
			mist::specialized::volumerendering( ct, timage, mist::volumerender::depth_map< mist::array3< double > >( depth_map ), p, table, cpunum );
		}

		std::cout << "Rendering Test [" << cpunum << "CPU] : " << t.elapse( ) * 10.0 << " msec." << std::endl;
	}
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




