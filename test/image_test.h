#ifndef __INCLUDE_IMAGE_TEST__
#define __INCLUDE_IMAGE_TEST__


#include <FL/Fl_Gl_Window.H>

class image_draw_area : public Fl_Gl_Window
{
public:
	image_draw_area( int x, int y, int w, int h, const char *l );
	image_draw_area( int x, int y, int w, int h );
	virtual ~image_draw_area(){ }

private:
	bool interpolate_;

public:
	void draw();
	bool interpolate( bool b )
	{
		return( interpolate_ = b );
	}
};

void read_image_test( const char *filename );
void write_image_test( const char *filename );

void read_dicom_test( const char *filename );
void write_dicom_test( const char *filename );

void euclidean_distance_transform_test( );

void figure_decomposition_test( );

void thresholding_test( );

void labeling4_test( );
void labeling8_test( );

void boundary4_test( );
void boundary8_test( );

void thinning_test( );

void median_test( );

void erosion_test( );
void dilation_test( );
void opening_test( );
void closing_test( );

void interpolate_test( int mode, bool reso_up );

void interlace_test( bool is_odd_line );

void expand_test( );
void shrink_test( );

void erosion_triangle_test( );
void dilation_triangle_test( );
void opening_triangle_test( );
void closing_triangle_test( );

#endif // __INCLUDE_IMAGE_TEST__
