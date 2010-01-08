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

#define INFILE "in.mpeg" // 適当に用意
#define OUTFILE1 "out1.mpeg"
#define OUTFILE2 "out2.mpeg"
#define OUTFILE3 "out3.mpeg"

#include <mist/interpolate.h>
#include <mist/io/bmp.h>
#include <mist/io/mpeg.h>


int main(int argc, char *argv[])
{	
	mist::array2< mist::rgb< unsigned char > > in_img, out_img;
	mist::mpeg::input_video  iv;
	mist::mpeg::output_video ov;


	// フレーム画像読み込み
	std::cout << std::endl << "test1: read frame" << std::endl;

	mist::mpeg::open( iv, INFILE );
	mist::mpeg::dump_format( iv );
	char f_name[ 256 ];
	size_t i = 0;
	while( mist::mpeg::read_frame( iv, out_img ) && i < 180 )
	{
		sprintf( f_name, "tmp/frame%04d.bmp", i ++ );
		mist::write_bmp( out_img, f_name );
	}
	const size_t width = out_img.width( );
	const size_t height = out_img.height( );
	const size_t frame_num = mist::mpeg::frame_number( iv );
	mist::mpeg::close( iv );


	// フレーム画像書き出し
	std::cout << std::endl << "test2: write frame" << std::endl;

	ov.width( width );					
	ov.height( height );				
	mist::mpeg::open( ov, OUTFILE1 );
	mist::mpeg::dump_format( ov );
	for( size_t i = 0 ; i < frame_num ; i ++ )
	{
		sprintf( f_name, "tmp/frame%04d.bmp", i );
		mist::read_bmp( in_img, f_name );
		mist::mpeg::write_frame( ov, in_img );
	}
	mist::mpeg::close( ov );


	// フィルタリング
	std::cout << std::endl << "test3: filtering" << std::endl;
	 
	mist::mpeg::open( iv, INFILE );
	mist::mpeg::copy_format( iv, ov );
	ov.width( width / 2 );
	ov.height( height / 2 );
	mist::mpeg::open( ov, OUTFILE2 );
	mist::mpeg::dump_format( iv );
	mist::mpeg::dump_format( ov );
	while( mist::mpeg::read_frame( iv, in_img ) )
	{
		mist::mean::interpolate( in_img, out_img, width / 2, height / 2 );
		mist::mpeg::write_frame( ov, out_img );
	}
	mist::mpeg::close( iv );
	mist::mpeg::close( ov );


	// フレーム画像のスキップとコピー
	std::cout << std::endl << "test4: skip and copy" << std::endl;

	mist::mpeg::open( iv, INFILE );
	mist::mpeg::copy_format( iv, ov );
	mist::mpeg::open( ov, OUTFILE3 );
	mist::mpeg::dump_format( iv );
	mist::mpeg::dump_format( ov );
	while( mist::mpeg::copy_frame( iv, ov, 1 ) )
	{
		mist::mpeg::skip_frame( iv, 2 );     
	} 
	mist::mpeg::close( iv );
	mist::mpeg::close( ov );
	
	return 0;
}