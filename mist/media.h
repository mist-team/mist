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

/// @file mist/media.h
//!
//! @brief ビデオファイルに対する処理を行うライブラリ
//!

#ifndef __INCLUDE_MEDIA_CUT_H__
#define __INCLUDE_MEDIA_CUT_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_IO_VIDEO_H__
#include "io/video.h"
#endif

#include <vector>


// mist名前空間の始まり
_MIST_BEGIN

namespace __media_utility__
{
	template < class Array >
	void __compute_histogram__( const Array &img, array3< double > &hist )
	{
		typedef typename Array::size_type size_type;

		hist.fill( );

		size_type binR = 256 / hist.size1( );
		size_type binG = 256 / hist.size2( );
		size_type binB = 256 / hist.size3( );

		for( size_type i = 0 ; i < img.size( ) ; i++ )
		{
			hist( img[ i ].r / binR, img[ i ].g / binG, img[ i ].b / binB ) += 1.0;
		}

		for( size_type i = 0 ; i < hist.size( ) ; i++ )
		{
			hist[ i ] /= static_cast< double >( img.size( ) );
		}
	}

	template < class Histogram >
	double __compute_similarity__( const Histogram &hist1, const Histogram &hist2 )
	{
		double ave1 = 1.0 / hist1.size( ), ave2 = 1.0 / hist2.size( );
		double r = 0.0, s1 = 0.0, s2 = 0.0;

		for( size_t i = 0 ; i < hist1.size( ) ; i++ )
		{
			double h1 = hist1[ i ];
			double h2 = hist2[ i ];
			r  += ( h1 - ave1 ) * ( h2 - ave2 );
			s1 += ( h1 - ave1 ) * ( h1 - ave1 );
			s2 += ( h2 - ave2 ) * ( h2 - ave2 );
		}

		return ( r / std::sqrt( s1 * s2 ) );
	}
}

//! @addtogroup video_group ビデオファイルの入出力を行うライブラリ
//!

//! @addtogroup media_group メディア処理に関するグループ
//!


/// @brief ビデオストリームを入力し，ビデオ内のカット位置を検出する
//! 
//! @code 使い方
//! #include <mist/mist.h>
//! #include <mist/io/video.h>
//! #include <mist/io/bmp.h>
//! #include <mist/timer.h>
//! #include <mist/media/cut.h>
//! #include <vector>
//! 
//! int main( int argc, char *argv[] )
//! {
//!     typedef mist::video::decoder::size_type       size_type;
//!     typedef mist::video::decoder::difference_type difference_type;
//!     mist::video::decoder iv;
//!     std::string filename = "入力ビデオファイル名";
//! 
//!     if( !iv.open( filename ) )
//!     {
//!         std::cerr << "Failed to open video file [" << filename << "]" << std::endl;
//!         return( 1 );
//!     }
//! 
//!     std::vector< int > indeces;
//!     mist::cut_detection( iv, indeces, 8, 8, 8, 0.95, 30 );
//! 
//!     iv.close( );
//! 
//!     // 再度ファイルを開く
//!     if( !iv.open( filename ) )
//!     {
//!         std::cerr << "Failed to open video file [" << filename << "]" << std::endl;
//!         return( 1 );
//!     }
//! 
//!     mist::array2< mist::rgb< unsigned char > > img;
//!     for( size_type i = 1 ; i < indeces.size( ) ; i++ )
//!     {
//!         std::cout << indeces[ i ] << std::endl;
//! 
//!         char fname[ 256 ];
//!         sprintf( fname, "%04d-%04d.avi", indeces[ i - 1 ], indeces[ i ] );
//!         mist::video::mpeg4::encoder ov( fname, iv.width( ), iv.height( ), 1, 30 );
//! 
//!         while( !iv.is_eof( ) && iv.frame_id( ) < indeces[ i ] )
//!         {
//!             iv >> img;
//!             ov << img;
//!         }
//! 
//!         ov.close( );
//!     }
//! 
//!     return( 0 );
//! }
//! @endcode
//! 
//! @param[in]  iv                       … カット検出を行うビデオファイルを開いた mist::video::decoder オブジェクト
//! @param[out] frame_indeces            … カット位置を出力するリスト
//! @param[in]  red_bin                  … 赤色成分のビン数
//! @param[in]  green_bin                … 緑色成分のビン数
//! @param[in]  blue_bin                 … 青色成分のビン数
//! @param[in]  threshold_of_similarity  … 色ヒストグラムの正規化相関に対するしきい値
//! @param[in]  number_of_between_frames … カット間の最小フレーム間隔
//!
//! @retval true  … カットの検出に成功
//! @retval false … カットの検出に失敗
//! 
bool cut_detection( video::decoder &iv, std::vector< int > &frame_indeces, size_t red_bin = 8, size_t green_bin = 8, size_t blue_bin = 8, double threshold_of_similarity = 0.95, ptrdiff_t number_of_between_frames = 30 )
{
	typedef video::decoder::size_type       size_type;
	typedef video::decoder::difference_type difference_type;
	typedef rgb< unsigned char > pixel_type;
	typedef array2< pixel_type > image_type;
	typedef array3< double > histogram_type;

	difference_type frameNo = 0;
	image_type img;

	// 先頭フレームを読み込む
	if( !iv.read( img, frameNo ) )
	{
		return( false );
	}

	const difference_type num_playback = 5;
	histogram_type h[ 2 ], *hh = new histogram_type[ num_playback ];
	double osim = 1.0;

	h[ 0 ].resize( red_bin, green_bin, blue_bin );
	h[ 1 ].resize( red_bin, green_bin, blue_bin );

	for( difference_type i = 0 ; i < num_playback ; i++ )
	{
		hh[ i ].resize( red_bin, green_bin, blue_bin );
	}

	frame_indeces.push_back( static_cast< int >( frameNo ) );

	__media_utility__::__compute_histogram__( img, h[ 0 ] );
	hh[ 0 ] = h[ 0 ];

	for( difference_type i = 0 ; !iv.is_eof( ) ; i++ )
	{
		if( !iv.read( img, frameNo ) )
		{
			break;
		}

		histogram_type &ch = h[ ( i + 1 ) % 2 ];
		__media_utility__::__compute_histogram__( img, ch );
		double sim1 = __media_utility__::__compute_similarity__( h[ 0 ], h[ 1 ] );
		double sim2 = 1.0, sim = sim1;

		if( i >= num_playback - 1 )
		{
			sim2 = __media_utility__::__compute_similarity__( ch, hh[ ( i + 1 ) % num_playback ] );
			sim = ( sim1 + sim2 ) * 0.5;
		}

		hh[ ( i + 1 ) % num_playback ] = ch;

		if( sim1 < threshold_of_similarity || sim2 < threshold_of_similarity )
		{
			difference_type oindx = frame_indeces.back( );
			difference_type nindx = frameNo;
			if( nindx < oindx + number_of_between_frames )
			{
				if( sim < osim )
				{
					frame_indeces.back( ) = static_cast< int >( nindx );
					osim = sim;
				}
			}
			else
			{
				frame_indeces.push_back( static_cast< int >( nindx ) );
				osim = sim;
			}
		}
	}

	delete [] hh;

	frame_indeces.push_back( static_cast< int >( iv.frame_id( ) ) );

	return( true );
}

/// @}
//  メディア処理に関するグループの終わり

/// @}
//  ビデオファイルの入出力を行うライブラリの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MEDIA_CUT_H__
