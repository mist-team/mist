#ifndef __INCLUDE_IMAGE_TEST__
#define __INCLUDE_IMAGE_TEST__


#include <FL/Fl_Gl_Window.H>

class image_draw_area : public Fl_Gl_Window
{
public:
	image_draw_area( int x, int y, int w, int h, const char *l ) : Fl_Gl_Window( x, y, w, h, l){ }
	image_draw_area( int x, int y, int w, int h ) : Fl_Gl_Window( x, y, w, h ){ }
	virtual ~image_draw_area(){ }

public:
	void draw();
};

void read_jpeg_test( const char *filename );
void write_jpeg_test( const char *filename );

void read_png_test( const char *filename );
void write_png_test( const char *filename );

void read_tiff_test( const char *filename );
void write_tiff_test( const char *filename );

void read_bmp_test( const char *filename );
void write_bmp_test( const char *filename );

void read_dicom_test( const char *filename );
void write_dicom_test( const char *filename );

void euclidean_distance_transform_test( );

#endif // __INCLUDE_IMAGE_TEST__
