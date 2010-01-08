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

#ifndef __INCLUDE_IMAGE_TEST__
#define __INCLUDE_IMAGE_TEST__


#include <FL/Fl_Gl_Window.H>
#include <mist/mist.h>
#include <mist/volumerender.h>


class volr_image_window;

typedef mist::vector2< double > point2;
typedef mist::vector3< double > point3;
typedef point3 vector_type;

typedef mist::volumerender::attribute_table< mist::rgb< double > > volr_table_type;
typedef mist::volumerender::parameter volr_parameter_type;


struct rendering_camera
{
	point3 pos;
	point3 dir;
	point3 up;
};


class volr_draw_area : public Fl_Gl_Window
{
public:
	typedef short ct_value_type;
	typedef mist::array3< ct_value_type >::size_type size_type;
	typedef mist::array3< ct_value_type >::difference_type difference_type;

public:
	volr_draw_area( int x, int y, int w, int h, const char *l ) : Fl_Gl_Window( x, y, w, h, l ), zoom_( 300.0 ), fps_( 1.0 )
	{
		is_high_resolution_ = true;
		draw_flag_ = false;
		inside_mode_ = false;
		high_reso_ = 256;
		low_reso_  = 128;
		mirror_view = true;
	}
	volr_draw_area( int x, int y, int w, int h ) : Fl_Gl_Window( x, y, w, h ), zoom_( 300.0 ), fps_( 1.0 )
	{
		is_high_resolution_ = true;
		draw_flag_ = false;
		inside_mode_ = false;
		high_reso_ = 256;
		low_reso_  = 128;
		mirror_view = true;
	}
	virtual ~volr_draw_area(){ }

private:
	mist::array3< ct_value_type > ct;
	mist::array3< double > depth_map;
	mist::array2< mist::rgb< unsigned char > > image_;

	bool is_high_resolution_;
	bool draw_flag_;
	bool inside_mode_;
	double zoom_;
	double fps_;
	int drag_beg_x;
	int drag_beg_y;

	bool mirror_view;

	size_type high_reso_;
	size_type low_reso_;

	rendering_camera camera_;
	rendering_camera old_camera_;

	volr_table_type volr_table;
	volr_parameter_type volr_parameter;

public:
	int handle( int event );
	void draw( );
	void draw_image( );

public:
	void onKeyDown( int key );
	void onMouseDown( int x, int y );
	void onMouseDrag( int button, int x, int y );
	void onMouseUp( int x, int y );
	void onInsideViewCallBack( );

	static void __onInsideViewCallBack__( void *p )
	{
		( ( volr_draw_area * )p )->onInsideViewCallBack( );
	}

public:
	size_type high_reso( size_type reso )
	{
		if( is_high_resolution_ && high_reso_ != reso )
		{
			high_reso_ = reso;
			draw_flag_ = true;
			redraw( );
			Fl::wait( 0 );
		}
		return( high_reso_ = reso );
	}

	size_type low_reso( size_type reso )
	{
		if( !is_high_resolution_ && low_reso_ != reso )
		{
			low_reso_ = reso;
			draw_flag_ = true;
			redraw( );
			Fl::wait( 0 );
		}
		return( low_reso_  = reso );
	}

	void specular( bool b )
	{
		volr_parameter.specular = b ? 1.0 : 0.0;
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
	}

	void light_attenuation( double lightAtten )
	{
		volr_parameter.light_attenuation = lightAtten;
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
	}

	void sampling_step( double sstep )
	{
		volr_parameter.sampling_step = sstep;
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
	}

	void termination( double terminate )
	{
		volr_parameter.termination = terminate;
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
	}

	void fovy( double ffovy )
	{
		volr_parameter.fovy = ffovy;
		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
	}

	void inside_mode( bool is_inside )
	{
		inside_mode_ = is_inside;
	}

public:
	void read_image( volr_image_window *wnd );
	void write_image( volr_image_window *wnd  );
	void render_test( volr_image_window *wnd );

	void rotate_camera( int sx, int sy, int x, int y );
	void rotate_camera( int x, int y, double step );
	void move_camera( double x, double y, double z );
	void initialize( );
};

#endif // __INCLUDE_IMAGE_TEST__
