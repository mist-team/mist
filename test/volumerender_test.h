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
	}
	volr_draw_area( int x, int y, int w, int h ) : Fl_Gl_Window( x, y, w, h ), zoom_( 300.0 ), fps_( 1.0 )
	{
		draw_flag_ = false;
	}
	virtual ~volr_draw_area(){ }

private:
	mist::array3< ct_value_type > ct;
	mist::array2< mist::rgb< unsigned char > > image_;

	bool draw_flag_;
	double zoom_;
	double fps_;
	int drag_beg_x;
	int drag_beg_y;

	rendering_camera camera_;
	rendering_camera old_camera_;

	volr_table_type volr_table;
	volr_parameter_type volr_parameter;

public:
	int handle( int event );
	void draw( );
	void draw_image( );

public:
	void onMouseDown( int x, int y );
	void onMouseDrag( int button, int x, int y );
	void onMouseUp( int x, int y );



public:
	void read_image( volr_image_window *wnd );
	void write_image( volr_image_window *wnd  );
	void rotate_camera( int sx, int sy, int x, int y );
	void initialize( );
};

#endif // __INCLUDE_IMAGE_TEST__
