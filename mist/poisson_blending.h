// 
// Copyright (c) 2003-2011, MIST Project, Nagoya University
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

/// @file mist/poisson_blending.h
//!
//! @brief Poisson Image Editing (SIGGRAPH 2003)
//!
#ifndef __INCLUDE_POISSON_BLENDING__
#define __INCLUDE_POISSON_BLENDING__


#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_NUMERIC__
#include "numeric.h"
#endif

#ifndef __INCLUDE_CONVERTER__
#include "converter.h"
#endif

#include <map>
#include <stdio.h>



// mist名前空間の始まり
_MIST_BEGIN


namespace __poissonblender__
{
	typedef unsigned char					mono_type;
	typedef rgb< unsigned char >			color_type;
	typedef rgb< double >					differencial_type;
	typedef array2< mono_type >				mono_image_type;
	typedef array2< color_type >			color_image_type;
	typedef array2< differencial_type >		differencial_image_type;
	typedef vector2< int >					point_type;		// ( x, y )
	typedef vector2< point_type >			rectangle_type;	// ( ( tl_x, tl_y ), ( br_x, br_y ) )

	template <typename T>
	inline void _recTrim( array2< T > &out, const array2< T > &in, const rectangle_type &rec )
	{
		in.trim ( out, rec.x.x, rec.x.y, ( rec.y.x - rec.x.x ), ( rec.y.y - rec.x.y ) );
	}

	template <typename T>
	inline void _recTrim( array2< T > &in, const rectangle_type &rec )
	{
		in.trim ( rec.x.x, rec.x.y, ( rec.y.x - rec.x.x ), ( rec.y.y - rec.x.y ) );
	}

	// clipping
	template <typename T>
	inline unsigned char _saturate_cast( T num )
	{
		if		( num > 255 ) return 255;
		else if ( num < 0 ) return 0;
		else return ( static_cast< unsigned char >(num) );
	}

	// calc differencial
	template <typename T> // return im1 -= im2
	inline void _differencial( array2< T > &im1, const array2< T > &im2, const bool integral = false )
	{
		unsigned int w = im1.width() < im2.width() ? im1.width() : im2.width();
		unsigned int h = im1.height() < im2.height() ? im1.height() : im2.height();
		array2< T > tmp = im1;
		im1.resize( w, h );
		for ( unsigned int j = 0; j < h; j++ ) {
			for ( unsigned int i = 0; i < w; i++ ) {
				if ( integral )	im1( i, j ) = tmp( i, j ) + im2( i, j );
				else			im1( i, j ) = tmp( i, j ) - im2( i, j );
			}				
		}
	}

	// build matrix as linear system
	template <typename T>
	inline bool _buildMatrix(matrix< T > &A, matrix< T > &b, matrix< T > &u,
		const unsigned int		&color,
		const mono_image_type	&_mask1,
		std::map<int, int>		&_mp,
		differencial_image_type	&_drvxy,
		const color_image_type	&_target1 )
	{
		int w = _mask1.width();
		int h = _mask1.height();

		int nz=0;
		_mp.clear();
		for ( int y = 0; y < h - 1; ++y )
		{
			for ( int x = 0; x < w - 1; ++x )
			{
				if ( _mask1( x, y ) == 0 ) continue;

				int id = y * w + x;
				_mp[ id ] = nz++; //
			}
		}
		if ( !A.resize( w - 1, nz ) ) {
			std::cout << "cant allocate matrix( "<< w-4 <<", "<< nz <<" )" << std::endl;
			std::cout << "mask size is too large." << std::endl;
			return false;
		}
		if ( !b.resize( nz, 1 ) ) {
			std::cout << "mask size is too large." << std::endl;
			return false;
		}
		if ( !u.resize( nz, 1 ) ){
			std::cout << "mask size is too large." << std::endl;
			return false;
		}

		int rowA = 0;

		for ( int y = 1; y < h - 1; ++y )
		{
			for ( int x = 1; x < w - 1; ++x )
			{
				if ( _mask1( x, y ) == 0 ) continue;
				differencial_type &drv = _drvxy( x, y );

				int id = y * w + x;
				int tidx = id - w, lidx = id - 1;

				// to omtimize insertion
				unsigned char tlrb = 15; // 0b1111
				if ( _mask1( x, y - 1 ) == 0 ) {
					if ( color == 0 ) drv.r -= static_cast< double >( _target1( x, y - 1 ).r );
					if ( color == 1 ) drv.g -= static_cast< double >( _target1( x, y - 1 ).g );
					if ( color == 2 ) drv.b -= static_cast< double >( _target1( x, y - 1 ).b );
					tlrb &= 7; //0b0111
				}
				if ( _mask1( x - 1, y ) == 0 ) {
					if ( color == 0 ) drv.r -= static_cast< double >( _target1( x - 1, y ).r );
					if ( color == 1 ) drv.g -= static_cast< double >( _target1( x - 1, y ).g );
					if ( color == 2 ) drv.b -= static_cast< double >( _target1( x - 1, y ).b );
					tlrb &= 11; //0b1011
				}
				if ( _mask1( x + 1, y ) == 0 ) {
					if ( color == 0 ) drv.r -= static_cast< double >( _target1( x + 1, y ).r );
					if ( color == 1 ) drv.g -= static_cast< double >( _target1( x + 1, y ).g );
					if ( color == 2 ) drv.b -= static_cast< double >( _target1( x + 1, y ).b );
					tlrb &= 13; //0b1101
				}
				if ( _mask1( x, y + 1 ) == 0 ) {
					if ( color == 0 ) drv.r -= static_cast< double >( _target1( x, y + 1 ).r );
					if ( color == 1 ) drv.g -= static_cast< double >( _target1( x, y + 1 ).g );
					if ( color == 2 ) drv.b -= static_cast< double >( _target1( x, y + 1 ).b );
					tlrb &= 14; //0b1110
				}

				// transform upper triangle matrix
				if ( tlrb & 8 )	A( _mp[ tidx ] - rowA + ( w - 2 ), rowA ) =  -1.0; // top
				if ( tlrb & 4 )	A( _mp[ lidx ] - rowA + ( w - 2 ), rowA ) =  -1.0; // left
				A( _mp[ id   ] - rowA + ( w - 2 ), rowA ) =  4.0; // center

				if ( color == 0 ) b( rowA, 0 ) = drv.r;
				if ( color == 1 ) b( rowA, 0 ) = drv.g;
				if ( color == 2 ) b( rowA, 0 ) = drv.b;
				rowA++;

			}
		}

		if ( nz != rowA ){
			std::cout << "unexpected error!" << std::endl;
			return false;
		}

		return true;
	}

	// solver sparse linear system
	template < typename T > 
	inline bool _solve(const matrix< T > &A, matrix< T > &b, matrix< T > &u)
	{
		try
		{
			u = solve( A, b, matrix_style::pb );
		}
		catch( numerical_exception &e )
		{
			std::cerr << "solving failed" << std::endl;
			std::cerr << e.message << std::endl;
			return false;
		}

		return true;
	}

	template <typename T>
	inline bool _copyResult(matrix< T > &u, const unsigned int &color, const mono_image_type &_mask1,
		std::map< int, int > &_mp, color_image_type &_dst1 )
	{
		int w = _mask1.width();
		int h = _mask1.height();
		for ( int y = 1; y < h - 1; ++y )
		{
			for ( int x = 1; x < w - 1; ++x )
			{
				if( _mask1( x, y ) == 0 ) continue;
				else
				{
					int idx = _mp[ y * w + x ];
					color_type &pd = _dst1( x, y );
					if ( color == 0 ) pd.r = _saturate_cast( -u( idx, 0 ) );
					if ( color == 1 ) pd.g = _saturate_cast( -u( idx, 0 ) );
					if ( color == 2 ) pd.b = _saturate_cast( -u( idx, 0 ) );
				}
			}
		}

		return true;
	}
}


//! @addtogroup poisson_image_editing_group PoissonImageEditing
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/poisson_blending.h>
//! @endcode
//!
//!  @{

/// @brief 入力画像のマスク領域を対象画像に合成する
//! 
//! 入力画像のマスク領域を対象画像に，オフセットを加えて合成する
//! @note mixをtrueにすると入力画像と対象画像の両方の輝度勾配を保持する
//! 
//! @param[in] src … 入力画像
//! @param[in] target … 対象画像
//! @param[out] out … 出力画像
//! @param[in] mask … マスク画像
//! @param[in] offx … 入力画像から出力画像にマップする際のオフセットのx座標
//! @param[in] offy … 入力画像から出力画像にマップする際のオフセットのy座標
//! @param[in] mix … 対象画像の輝度勾配を保持するか否か
//! 
//! @retval true  … 合成に成功した場合
//! @retval false … 不正な合成を行おうとした場合
//! 
template< class Array1, class Array2, class Array3 >
inline bool seamlessCloning(
	const Array1 &src, const Array2 &target, Array3 &out,
	const array2< unsigned char > &mask,
	const int offx = 0, const int offy = 0, const bool mix = false )
{	
	__poissonblender__::mono_image_type		_mask = mask;
	//__poissonblender__::color_image_type	_dst;

	if ( src.empty() || target.empty() || _mask.empty() ) return false;

	// 
	__poissonblender__::point_type offset( offx, offy );
	__poissonblender__::point_type tl( _mask.size1(), _mask.size2() ), br( -1, -1 );

	// binary transform
	for ( unsigned int y = 0; y < _mask.height(); ++y ) {
		for ( unsigned int x = 0; x < _mask.width(); ++x )
		{
			if ( _mask( x, y ) < 50 )	_mask( x, y ) = 0;
			else						_mask( x, y ) = 255;
		}
	}

	// calc bounding box
	for ( unsigned int y = 0; y < _mask.height(); ++y ) {
		for ( unsigned int x = 0; x < _mask.width(); ++x )
		{
			if ( _mask( x, y ) == 0 ) continue;
			if ( tl.x > (int)x ) tl.x = (int)x;
			if ( tl.y > (int)y ) tl.y = (int)y;
			if ( br.x < (int)x ) br.x = (int)x;
			if ( br.y < (int)y ) br.y = (int)y;
		}
	}
	br.x += 1;
	br.y += 1;

	// add borders
	__poissonblender__::rectangle_type mask_roi( tl, br );
	__poissonblender__::rectangle_type mask_roi2( tl - __poissonblender__::point_type( 2, 2 ), br + __poissonblender__::point_type( 2, 2 ) );
	marray< __poissonblender__::color_image_type > _srcUP( 2 ), _targetUP( 2 ), _dstUP;
	marray< __poissonblender__::mono_image_type > _maskUP( 1 );
	_srcUP		= src;
	_targetUP	= target;
	_maskUP		= _mask;

	// allocate destination image
	__poissonblender__::color_image_type dst = target;
	_dstUP	= _targetUP;

	__poissonblender__::rectangle_type			_mask_roi1;
	__poissonblender__::mono_image_type			_mask1;
	__poissonblender__::color_image_type		_dst1;
	__poissonblender__::color_image_type		_target1;

	_mask_roi1 = __poissonblender__::rectangle_type( tl - __poissonblender__::point_type( 1, 1 ), br + __poissonblender__::point_type( 1, 1 ) );
	__poissonblender__::_recTrim( _mask1,	_mask,		_mask_roi1 );
	__poissonblender__::_recTrim( _target1, _targetUP,	__poissonblender__::rectangle_type( _mask_roi1.x + offset - __poissonblender__::point_type( 1, 1 ), _mask_roi1.y + offset - __poissonblender__::point_type( 1, 1 ) ) );
	__poissonblender__::_recTrim( _dst1,	_dstUP,		__poissonblender__::rectangle_type(	_mask_roi1.x + offset - __poissonblender__::point_type( 1, 1 ),	_mask_roi1.y + offset - __poissonblender__::point_type( 1, 1 ) ) );

	__poissonblender__::color_image_type _src, _target, _dst;
	__poissonblender__::_recTrim( _src,		_srcUP,		mask_roi2 );
	__poissonblender__::_recTrim( _target,	_targetUP,	__poissonblender__::rectangle_type(	mask_roi2.x + offset - __poissonblender__::point_type(2,2),	mask_roi2.y + offset - __poissonblender__::point_type(2,2) ) );
	__poissonblender__::_recTrim( _mask,	_maskUP,	mask_roi2 );
	__poissonblender__::_recTrim( _dst,		_dstUP,		__poissonblender__::rectangle_type( mask_roi2.x + offset - __poissonblender__::point_type(2,2),	mask_roi2.y + offset - __poissonblender__::point_type(2,2) ) );
	if ( _src.height() != _dst.height() || _src.width() != _dst.width() ) return false;

	// calc differential image
	__poissonblender__::differencial_image_type src64, target64;
	__poissonblender__::differencial_image_type _src64_00, _target64_00;
	int pw = mask_roi2.y.x - mask_roi2.x.x - 1, ph = mask_roi2.y.y - mask_roi2.x.y -1;
	src64		= _src;
	target64	= _target;
	__poissonblender__::rectangle_type roi00( __poissonblender__::point_type( 0, 0 ), __poissonblender__::point_type( pw, ph ) );
	__poissonblender__::rectangle_type roi10( __poissonblender__::point_type( 1, 0 ), __poissonblender__::point_type( pw, ph ) );
	__poissonblender__::rectangle_type roi01( __poissonblender__::point_type( 0, 1 ), __poissonblender__::point_type( pw, ph ) );
	__poissonblender__::_recTrim( _src64_00,	src64,		roi00 );
	__poissonblender__::_recTrim( _target64_00,	target64,	roi00 );

	__poissonblender__::differencial_image_type src_dx, src_dy, target_dx, target_dy;
	__poissonblender__::_recTrim( src_dx, src64, roi10 );		__poissonblender__::_differencial( src_dx, _src64_00 );
	__poissonblender__::_recTrim( src_dy, src64, roi01 );		__poissonblender__::_differencial( src_dy, _src64_00 );
	__poissonblender__::_recTrim( target_dx, target64, roi10 );	__poissonblender__::_differencial( target_dx, _target64_00 );
	__poissonblender__::_recTrim( target_dy, target64, roi01 );	__poissonblender__::_differencial( target_dy, _target64_00 );

	// gradient mixture
	__poissonblender__::differencial_image_type Dx, Dy;
	if ( mix )
	{  // with gradient mixture
		unsigned int wx = src_dx.width();
		unsigned int hx = src_dx.height();
		Dx.resize( wx, hx );
		unsigned int wy = src_dy.width();
		unsigned int hy = src_dy.height();
		Dy.resize( wy, hy );

		for ( unsigned int j = 0; j < hx; j++ ) {
			for ( unsigned int i = 0; i < wx; i++ )
			{	//
				Dx( i, j ).r = abs( src_dx( i, j ).r ) > abs( target_dx( i, j ).r ) ? src_dx( i, j ).r : target_dx( i, j ).r;
				Dx( i, j ).g = abs( src_dx( i, j ).g ) > abs( target_dx( i, j ).g ) ? src_dx( i, j ).g : target_dx( i, j ).g;
				Dx( i, j ).b = abs( src_dx( i, j ).b ) > abs( target_dx( i, j ).b ) ? src_dx( i, j ).b : target_dx( i, j ).b;
			}
		}
		for ( unsigned int j = 0; j < hy; j++ ) {
			for ( unsigned int i = 0; i < wy; i++ )
			{	//
				Dy( i, j ).r = abs( src_dy( i, j ).r ) > abs( target_dy( i, j ).r ) ? src_dy( i, j ).r : target_dy( i, j ).r;
				Dy( i, j ).g = abs( src_dy( i, j ).g ) > abs( target_dy( i, j ).g ) ? src_dy( i, j ).g : target_dy( i, j ).g;
				Dy( i, j ).b = abs( src_dy( i, j ).b ) > abs( target_dy( i, j ).b ) ? src_dy( i, j ).b : target_dy( i, j ).b;
			}
		}
	}
	else
	{   // without gradient mixture
		Dx = src_dx;
		Dy = src_dy;
	}

	// lapilacian
	int w = pw-1, h = ph-1;
	__poissonblender__::differencial_image_type Dx_00 = Dx, Dy_00 = Dy;
	__poissonblender__::differencial_image_type	_drvxy;

	__poissonblender__::_recTrim( Dx,		__poissonblender__::rectangle_type( __poissonblender__::point_type( 1, 0 ), __poissonblender__::point_type( w, h ) ) );
	__poissonblender__::_recTrim( Dx_00,	__poissonblender__::rectangle_type( __poissonblender__::point_type( 0, 0 ), __poissonblender__::point_type( w, h ) ) );
	__poissonblender__::_differencial( Dx, Dx_00 );
	__poissonblender__::_recTrim( Dy,		__poissonblender__::rectangle_type( __poissonblender__::point_type( 0, 1 ), __poissonblender__::point_type( w, h ) ) );
	__poissonblender__::_recTrim( Dy_00,	__poissonblender__::rectangle_type( __poissonblender__::point_type( 0, 0 ), __poissonblender__::point_type( w, h ) ) );
	__poissonblender__::_differencial( Dy, Dy_00 );
	__poissonblender__::_differencial( Dx, Dy, true );
	_drvxy = Dx;

	// 
	// solve an poisson's equation
	//

	matrix< double > A[ 3 ], b[ 3 ], u[ 3 ];
	std::map<int,int> _mp;

	for ( unsigned int i = 0; i < 3; i++ )
	{
		// build right-hand and left-hand matrix
		if ( !__poissonblender__::_buildMatrix( A[ i ], b[ i ], u[ i ], i, _mask1, _mp, _drvxy, _target1 ) ) {
			std::cerr << "cant build matrix." << std::endl;	
			return false;
		}

		// solve sparse linear system
		if ( !__poissonblender__::_solve(A[ i ], b[ i ], u[ i ]) ) {
			std::cerr << "cant solve poissson matrix." << std::endl;	
			return false;
		}

		// copy computed result to destination image
		if ( !__poissonblender__::_copyResult( u[ i ], i, _mask1, _mp, _dst1 ) ) {
			std::cerr << "cant reconstruct cloned image." << std::endl;						
			return false;
		}
	}

	// reconstruct whole image
	for ( unsigned int j = 0; j < _dst1.height(); j++ ) {
		for ( unsigned int i = 0; i < _dst1.width(); i++ ) {
			dst( i + _mask_roi1.x.x + offset.x - 3, j + _mask_roi1.x.y + offset.y - 3 ) = _dst1( i, j );
		}
	}

	// convert to "out" color type
	convert( dst, out );

	return true;
}

/// @brief 入力画像のマスク領域以外をグレースケールにする
//! 
//! 入力画像のマスク領域を保存し，入力画像をグレースケール化した対象画像にseamlessCloningする
//! 
//! @param[in] in … 入力画像
//! @param[out] out … 出力画像
//! @param[in] mask … マスク画像
//! 
//! @retval true  … 合成に成功した場合
//! @retval false … 不正な合成を行おうとした場合
//! 
template< class Array1, class Array2 >
inline bool localColorChange( const Array1 &in, Array2 &out, const array2< unsigned char > &mask )
{
	if ( in.empty() || mask.empty() ) return false;
	
	__poissonblender__::mono_image_type _target;
	convert( in, _target );

	return seamlessCloning( in, _target, out, mask );
}

/// @brief 入力画像のマスク領域だけ色成分を変更する
//! 
//! 入力画像のマスク領域内のオブジェクトのみ色成分を変更する
//! 
//! @param[in] in … 入力画像
//! @param[out] out … 出力画像
//! @param[in] mask … マスク画像
//! @param[in] multiplier … 各色成分の変更比率
//! 
//! @retval true  … 合成に成功した場合
//! @retval false … 不正な合成を行おうとした場合
//! 
template< class Array1, class Array2 >
inline bool localColorChange( const Array1 &in, Array2 &out, const array2< unsigned char > &mask,
	const rgb< double > multiplier )
{
	if ( in.empty() || mask.empty() ) return false;

	__poissonblender__::color_image_type _src = in;
	for ( unsigned int j = 0; j < _src.height(); j++ ) {
		for ( unsigned int i = 0; i < _src.width(); i++ )
		{
			_src( i, j ).r = __poissonblender__::_saturate_cast( in( i, j ).r * multiplier.r );
			_src( i, j ).g = __poissonblender__::_saturate_cast( in( i, j ).g * multiplier.g );
			_src( i, j ).b = __poissonblender__::_saturate_cast( in( i, j ).b * multiplier.b );
		}
	}

	return seamlessCloning( _src, in, out, mask );
}

/// @}
//  PoissonImageEditingグループの終わり

_MIST_END

#endif	/*__INCLUDE_POISSON_BLENDING__*/