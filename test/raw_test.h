#ifndef __INCLUDE_IMAGE_TEST__
#define __INCLUDE_IMAGE_TEST__


#include <FL/Fl_Gl_Window.H>
#include <mist/mist.h>

class ct_image_window;

class ct_draw_area : public Fl_Gl_Window
{
public:
	typedef short ct_value_type;
	typedef mist::array3< ct_value_type >::size_type size_type;

public:
	ct_draw_area( int x, int y, int w, int h, const char *l ) : Fl_Gl_Window( x, y, w, h, l )
	{
		index_ = 0;
		window_level_ = 40;
		window_width_ = 300;
		draw_flag_ = false;
	}
	ct_draw_area( int x, int y, int w, int h ) : Fl_Gl_Window( x, y, w, h )
	{
		index_ = 0;
		window_level_ = 40;
		window_width_ = 300;
		draw_flag_ = false;
	}
	virtual ~ct_draw_area(){ }

private:
	mist::array3< ct_value_type > ct;
	mist::array2< unsigned char > buff;
	size_type index_;
	double window_level_;
	double window_width_;
	bool draw_flag_;

public:
	void draw( );
	void draw_image( );

public:
	void median_filter1D( ct_image_window *wnd );
	void median_filter2D( ct_image_window *wnd );
	void median_filter3D( ct_image_window *wnd );
	void normalization_filter3D( ct_image_window *wnd );
	void euclidean_distance_transform( ct_image_window *wnd );
	void labeling6( ct_image_window *wnd );
	void labeling26( ct_image_window *wnd );
	void erosion( ct_image_window *wnd );
	void dilation( ct_image_window *wnd );
	void opening( ct_image_window *wnd );
	void closing( ct_image_window *wnd );

public:
	void read_image( ct_image_window *wnd );
	void write_image( ct_image_window *wnd  );
	void change_index( size_type index );
	void change_window_level( double wl );
	void change_window_width( double ww );
};

#endif // __INCLUDE_IMAGE_TEST__
