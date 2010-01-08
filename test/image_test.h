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
void euclidean_distance_skeleton_test( );

void figure_decomposition_test( );

void thresholding_test( );

void labeling4_test( );
void labeling8_test( );

void boundary4_test( );
void boundary8_test( );

void thinning_test( );

void median_test( );
void mode_test( );

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
