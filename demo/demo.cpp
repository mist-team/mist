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

#include <iostream>
#include <mist/mist.h>
#include <mist/vector.h>
#include <mist/converter.h>
#include <mist/threshold.h>
#include <mist/interpolate.h>
#include <mist/filter/morphology.h>
#include <mist/filter/labeling.h>

#include <mist/io/jpeg.h>
#include <mist/io/bmp.h>
#include <mist/io/png.h>
#include <mist/io/tiff.h>
#include <mist/io/pnm.h>
#include <mist/timer.h>

#include <vector>

typedef struct
{
	size_t left;
	size_t right;
	size_t top;
	size_t bottom;
	double cx;
	double cy;
	size_t pixel_num;
	size_t label;
} face_parts;

typedef std::vector< face_parts > face_parts_list;


template < class T, class Allocator >
inline typename mist::array2< T, Allocator >::value_type vertical_ave( const mist::array2< T, Allocator > &in,
																		typename mist::array2< T, Allocator >::size_type i,
																		typename mist::array2< T, Allocator >::size_type j1,
																		typename mist::array2< T, Allocator >::size_type j2 )
{
	typedef typename mist::array2< T, Allocator >::size_type  size_type;
	typedef typename mist::array2< T, Allocator >::value_type value_type;
	double v = 0;
	for( size_type j = j1 ; j <= j2 ; j++ )
	{
		v += in( i, j );
	}
	return( static_cast< value_type >( v / ( j2 - j1 + 1 ) ) );
}

template < class T1, class T2, class Allocator1, class Allocator2 >
inline void vertical( const mist::array2< T1, Allocator1 > &in, mist::array2< T2, Allocator2 > &out )
{
	typedef typename mist::array2< T1, Allocator1 >::size_type  size_type;
	typedef typename mist::array2< T2, Allocator2 >::value_type out_value_type;

	out.resize( in.width( ), in.height( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	size_type i, j;
	double A1, A2, A3;
	size_type js = in.height( ) / 5;
	size_type je = in.height( ) / 5 * 4;
	js = js < 7 ? 7 : js;
	je = je > in.height( ) - 8 ? in.height( ) - 8 : je;
	for( j = js ; j < je ; j++ )
	{
		for( i = 0 ; i < in.width( ) ; i++ )
		{
			A1 = vertical_ave( in, i, j - 7, j - 4 );
			A2 = vertical_ave( in, i, j - 3, j + 4 );
			A3 = vertical_ave( in, i, j + 5, j + 8 );
			out( i, j ) = static_cast< out_value_type >( A1 - A2 < A3 - A2 ? A1 - A2 : A3 - A2 );
		}
	}
}

template < class T, class Allocator >
inline void enum_face_parts( mist::array2< T, Allocator > &in, face_parts_list &parts_list )
{
	typedef typename mist::array2< T, Allocator >::size_type  size_type;

	mist::array2< unsigned int > label;

	face_parts_list list;

	size_type label_num = mist::labeling8( in, label );
	size_type i, j;
	face_parts f0;

	f0.left   = in.width( );
	f0.right  = 0;
	f0.top    = in.height( );
	f0.bottom = 0;
	f0.cx     = 0.0;
	f0.cy     = 0.0;
	f0.pixel_num = 0;
	f0.label     = 0;

	for( i = 0 ; i <= label_num ; i++ )
	{
		f0.label = i;
		list.push_back( f0 );
	}

	// 画像の縁5分の1のところは計算しない
	for( j = in.height( ) / 5 ; j < in.height( ) / 5 * 4 ; j++ )
	{
		for( i = in.width( ) / 5 ; i < in.width( ) / 5 * 4 ; i++ )
		{
			if( label( i, j ) == 0 ) continue;
			face_parts &f = list[ label( i, j ) ];
			f.left   = f.left < i ? f.left : i;
			f.right  = f.right > i ? f.right : i;
			f.top    = f.top < j ? f.top : j;
			f.bottom = f.bottom > j ? f.bottom : j;
			f.cx    += i;
			f.cy    += j;
			f.pixel_num++;
		}
	}

	for( i = 1 ; i <= label_num ; i++ )
	{
		face_parts &f = list[ i ];
		f.cx /= static_cast< double >( f.pixel_num );
		f.cy /= static_cast< double >( f.pixel_num );
	}

	const size_type LENWMAX	= 50;				// 顔部品候補領域として許容する最大xサイズ
	const size_type LENHMAX	= 30;				// 顔部品候補領域として許容する最大yサイズ
	const size_type LENWMIN	= 5;				// 顔部品候補領域として許容する最小xサイズ
	const size_type LENHMIN	= 3;				// 顔部品候補領域として許容する最小yサイズ

	size_type w, h;
	for( i = 1 ; i <= label_num ; i++ )
	{
		face_parts &f = list[ i ];
		w = f.right - f.left + 1;
		h = f.bottom - f.top + 1;
		if( w < LENWMIN || w > LENWMAX || h < LENHMIN || h > LENHMAX )
		{
			f.label = 0;
		}
	}

	for( i = 0 ; i < label.size( ) ; i++ )
	{
		in[ i ] = list[ label[ i ] ].label == 0 ? 0 : 1;
	}

	parts_list.clear( );
	for( i = 1 ; i <= label_num ; i++ )
	{
		if( list[ i ].label != 0 )
		{
			parts_list.push_back( list[ i ] );
		}
	}
}


template < class T1, class T2, class Allocator1, class Allocator2 >
inline double matching( const mist::array2< T1, Allocator1 > &img1, const mist::array2< T2, Allocator2 > &img2,
							typename mist::array2< T1, Allocator1 >::difference_type x, typename mist::array2< T1, Allocator1 >::difference_type y )
{
	typedef typename mist::array2< T1, Allocator1 >::size_type  size_type;

	double v = 0;
	size_type i, j;
	size_type w = x + img2.width( ) < img1.width( ) ? img2.width( ) : img1.width( ) - x;
	size_type h = y + img2.height( ) < img1.height( ) ? img2.height( ) : img1.height( ) - y;
	for( j = 0 ; j < h ; j++ )
	{
		for( i = 0 ; i < w ; i++ )
		{
			v += img1( x + i, y + j ) * img2( i, j );
		}
	}
	return( v );
}


template < class T1, class T2, class Allocator1, class Allocator2 >
inline bool eye_template_matching( mist::array2< T1, Allocator1 > &in, const mist::array2< T2, Allocator2 > &face_template, face_parts_list &list )
{
	typedef typename mist::array2< T1, Allocator1 >::size_type  size_type;
	typedef typename mist::array2< T1, Allocator1 >::difference_type difference_type;
	const size_type TREYEX = 24;			// テンプレート右目領域重心位置x座標
	const size_type TREYEY = 30;			// テンプレート右目領域重心位置y座標
	const size_type TLEYEX = 68;			// テンプレート左目領域重心位置x座標
	const size_type TLEYEY = 33;			// テンプレート左目領域重心位置y座標

	face_parts eye_right, eye_left;

	size_type i, index = 0;
	double max_score = -1.0, score;

	// 右目を抽出
	for( i = 0 ; i < list.size( ) ; i++ )
	{
		face_parts &f = list[ i ];
		if( f.cx > TREYEX && f.cy > TREYEY )
		{
			score = matching( in, face_template, static_cast< difference_type >( f.cx - TREYEX ), static_cast< difference_type >( f.cy - TREYEY ) );
			if( max_score < score )
			{
				max_score = score;
				index = i;
			}
		}
	}

	if( max_score < 0.0 )
	{
		return( false );
	}

	eye_right = list[ index ];

	max_score = -1.0;
	index = 0;

	// 左目を抽出
	for( i = 0 ; i < list.size( ) ; i++ )
	{
		face_parts &f = list[ i ];
		if( f.cx > TLEYEX && f.cy > TLEYEY )
		{
			score = matching( in, face_template, static_cast< difference_type >( f.cx - TLEYEX ), static_cast< difference_type >( f.cy - TLEYEY ) );
			if( max_score < score )
			{
				max_score = score;
				index = i;
			}
		}
	}

	if( max_score < 0.0 )
	{
		return( false );
	}

	eye_left = list[ index ];

	const double MAXDME = 70.0;		// 棄却判定の距離閾値[pixel]
	const double MINDME = 5.0;		// 棄却判定の距離閾値[pixel]
	double d_re_le = std::sqrt( ( eye_left.cx - eye_right.cx ) * ( eye_left.cx - eye_right.cx ) + ( eye_left.cy - eye_right.cy ) * ( eye_left.cy - eye_right.cy ) );

	// 両目が離れすぎている場合，右目と左目が入れ替わっている場合，両目の距離が近すぎる場合は棄却する
	if( d_re_le > MAXDME || eye_left.cx < eye_right.cx || eye_left.cx - eye_right.cx < MINDME )
	{
		// 目領域無し
		list.clear( );
		return( false );
	}
	else
	{
		list.clear( );
		list.push_back( eye_left );
		list.push_back( eye_right );
		return( true );
	}
}


// 右目と左目を囲む四角形領域を黒く塗りつぶす
template < class T, class Allocator >
inline void eye_masking( mist::array2< T, Allocator > &in, const face_parts &leye, const face_parts &reye )
{
	typedef typename mist::array2< T, Allocator >::size_type  size_type;
	typedef mist::vector3< double > vector_type;

	double margin = 2;
	vector_type p0, vec;
	vector_type p1( reye.left  - margin, reye.top    - margin, 0.0 );
	vector_type p2( reye.left  - margin, reye.bottom + margin, 0.0 );
	vector_type p3( leye.right + margin, leye.bottom + margin, 0.0 );
	vector_type p4( leye.right + margin, leye.top    - margin, 0.0 );

	for( size_type j = 0 ; j < in.height( ) ; j++ )
	{
		for( size_type i = 0 ; i < in.width( ) ; i++ )
		{
			// 四角形との衝突判定
			p0 = vector_type( i, j, 0 );
			vec = ( p2 - p1 ) * ( p1 - p0 );
			if( ( vec ^ ( ( p3 - p2 ) * ( p2 - p0 ) ) ) <= 0.0 )
			{
				continue;
			}
			if( ( vec ^ ( ( p4 - p3 ) * ( p3 - p0 ) ) ) <= 0.0 )
			{
				continue;
			}
			if( ( vec ^ ( ( p1 - p4 ) * ( p4 - p0 ) ) ) <= 0.0 )
			{
				continue;
			}
			in( i, j ) = 0;
		}
	}
}

template < class T, class Allocator >
inline bool eye_masking( mist::array2< T, Allocator > &in )
{
	typedef typename mist::array2< T, Allocator >::size_type  size_type;

	size_type i;
	mist::array2< double > image1, image2;

	// テンプレート画像の初期化
	static unsigned char ftemplate[] = {
			#include "./face_template.dat"
	};
	const size_type fwidth = 90;
	const size_type fheight = 95;

	mist::array2< unsigned char > face_template( fwidth, fheight );
	for( i = 0 ; i < face_template.size( ) ; i++ )
	{
		face_template[ i ] = ftemplate[ i ];
	}
	// テンプレート画像の初期化はここまで


	// カラー画像からグレースケールに変換
	mist::convert( in, image2 );

	// 画像を320×240に線形補間する
	mist::nearest::interpolate( image2, image1, 320, 240 );
	double ax = image1.width( ) / static_cast< double >( in.width( ) );
	double ay = image1.height( ) / static_cast< double >( in.height( ) );

	// 最小値フィルタ
	mist::erosion( image1, mist::morphology::square( 1.0, image1.reso1( ), image1.reso2( ) ) );
	vertical( image1, image2 );

	// Pタイル法を用いた閾値処理
	double th = mist::ptile::threshold( image2, 95.0 );
	for( i = 0 ; i < image2.size( ) ; i++ )
	{
		image2[ i ] = image2[ i ] < th ? 0 : 1;
	}

	face_parts_list list;

	// 顔部品候補領域を抽出する却
	enum_face_parts( image2, list );

	// 顔部品候補領域に対し，テンプレートマッチングを用いて最も目と思われる部分を選択する
	if( !eye_template_matching( image2, face_template, list ) )
	{
		// 目領域にマッチする部分が存在しないので棄却
		//for( i = 0 ; i < image2.size( ) ; i++ )
		//{
		//	in[ i ] = image2[ i ] == 0 ? 0 : 255;
		//}
		return( false );
	}
	else
	{
		list[ 0 ].left   = static_cast< size_type >( list[ 0 ].left   / ax );
		list[ 0 ].right  = static_cast< size_type >( list[ 0 ].right  / ax );
		list[ 0 ].top    = static_cast< size_type >( list[ 0 ].top    / ay );
		list[ 0 ].bottom = static_cast< size_type >( list[ 0 ].bottom / ay );
		list[ 1 ].left   = static_cast< size_type >( list[ 0 ].left   / ax );
		list[ 1 ].right  = static_cast< size_type >( list[ 0 ].right  / ax );
		list[ 1 ].top    = static_cast< size_type >( list[ 0 ].top    / ay );
		list[ 1 ].bottom = static_cast< size_type >( list[ 0 ].bottom / ay );
		// 目領域にマッチする部分が存在しないので棄却
		eye_masking( in, list[ 0 ], list[ 1 ] );
		return( true );
	}
}



// ここから，ユーティリティ関数群
inline std::string to_lower_case( const std::string &str )
{
	std::string s = "";
	for( std::string::size_type i = 0 ; i < str.size( ) ; i++ )
	{
		s += static_cast< char >( tolower( str[ i ] ) );
	}
	return( s );
}

inline std::string get_ext( const std::string &str )
{
	std::string::size_type index = str.find_last_of( "." );
	if( index == str.npos )
	{
		return( "" );
	}
	return( str.substr( index ) );
}

template < class T, class Allocator >
inline bool read_image( mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = to_lower_case( get_ext( filename ) );
	if( ext == "" )
	{
		return( false );
	}

	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::read_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::read_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::read_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::read_png( image, filename );
	}
	else if( ext == ".pbm" || ext == ".pgm" || ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::read_pnm( image, filename );
	}
	else
	{
		ret = false;
	}
	return( ret );
}

template < class T, class Allocator >
inline bool write_image( mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = to_lower_case( get_ext( filename ) );
	if( ext == "" )
	{
		return( false );
	}

	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::write_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::write_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::write_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::write_png( image, filename );
	}
	else if( ext == ".pbm" )
	{
		ret = mist::write_pnm( image, filename, 1 );
	}
	else if( ext == ".pgm" )
	{
		ret = mist::write_pnm( image, filename, 2 );
	}
	else if( ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::write_pnm( image, filename, 3 );
	}
	else
	{
		ret = false;
	}
	return( ret );
}

int main( int argc, char *argv[] )
{
	if( argc == 2 )
	{
		mist::array2< mist::rgb< unsigned char > > image;
		if( read_image( image, argv[ 1 ] ) )
		{
			mist::timer t;
			eye_masking( image );
			std::cout << "Computation time: " << t.elapse( ) << " sec." << std::endl;
		}
		write_image( image, std::string( argv[1] ) + get_ext( argv[1] ) );
	}

	return( 0 );
}
