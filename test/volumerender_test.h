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
		draw_flag_ = false;
		high_reso_ = 256;
		low_reso_  = 128;
		is_high_resolution_ = true;
	}
	volr_draw_area( int x, int y, int w, int h ) : Fl_Gl_Window( x, y, w, h ), zoom_( 300.0 ), fps_( 1.0 )
	{
		draw_flag_ = false;
		high_reso_ = 256;
		low_reso_  = 128;
		is_high_resolution_ = true;
	}
	virtual ~volr_draw_area(){ }

private:
	mist::array3< ct_value_type > ct;
	mist::array3< double > depth_map;
	mist::array2< mist::rgb< unsigned char > > image_;

	bool is_high_resolution_;
	bool draw_flag_;
	double zoom_;
	double fps_;
	int drag_beg_x;
	int drag_beg_y;

	size_type high_reso_;
	size_type low_reso_;

	rendering_camera camera_;
	rendering_camera old_camera_;

	volr_table_type volr_table;
	volr_parameter_type volr_parameter;
	mist::volumerender::barrel_distortion barrel_distortion_;

public:
	int handle( int event );
	void draw( );
	void draw_image( );

public:
	void onMouseDown( int x, int y );
	void onMouseDrag( int button, int x, int y );
	void onMouseUp( int x, int y );

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

	void barrel_distortion( double bdistortion )
	{
		barrel_distortion_.kappa = bdistortion;
		barrel_distortion_.update( );

		draw_flag_ = true;
		redraw( );
		Fl::wait( 0 );
	}

public:
	void read_image( volr_image_window *wnd );
	void write_image( volr_image_window *wnd  );
	void rotate_camera( int sx, int sy, int x, int y );
	void initialize( );
};

#endif // __INCLUDE_IMAGE_TEST__
