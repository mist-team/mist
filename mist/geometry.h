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

/// @file mist/geometry.h
//!
//! @brief geometric calculation
//!
#ifndef __INCLUDE_GEOMETRY__
#define __INCLUDE_GEOMETRY__

#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif

#ifndef __INCLUDE_MIST_NUMERIC__
#include "numeric.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

_MIST_BEGIN


/// @brief calculate translation matrix
//! @param[out] mat is translation matrix
//! @param[in]  x,y is position
//! 
template< class T, class Allocator >
void translation_matrix( matrix< T, Allocator > &mat, typename matrix< T, Allocator >::value_type x, typename matrix< T, Allocator >::value_type y )
{
	typedef matrix< T > matrix_type;
	mat = matrix_type::_33(
		1.0, 0.0,  x,
		0.0, 1.0,  y,
		0.0, 0.0, 1.0
	);
}

/// @brief calculate rotation matrix
//! @param[out] mat    is rotation matrix
//! @param[in]  radian is rotation angle
//! 
template< class T, class Allocator >
void rotation_matrix( matrix< T, Allocator > &mat, typename matrix< T, Allocator >::value_type radian )
{
	typedef matrix< T > matrix_type;
	mat = matrix_type::_33(
			cos( radian ), -sin( radian ), 0.0,
			sin( radian ),  cos( radian ), 0.0,
			0.0,            0.0,           1.0
		);
}

/// @brief calculate rotation matrix
//! @param[out] mat    is rotation matrix
//! @param[in]  radian is rotation angle
//! @param[in]  cx     is X coordinate of the rotation center
//! @param[in]  cy     is Y coordinate of the rotation center
//! 
template< class T, class Allocator >
void rotation_matrix( matrix< T, Allocator > &mat, typename matrix< T, Allocator >::value_type radian, typename matrix< T, Allocator >::value_type cx, typename matrix< T, Allocator >::value_type cy )
{
	typedef matrix< T > matrix_type;
	mat  = matrix_type::_33(
			1.0, 0.0,  cx,
			0.0, 1.0,  cy,
			0.0, 0.0, 1.0
		);
	mat *= matrix_type::_33(
			cos( radian ), -sin( radian ), 0.0,
			sin( radian ),  cos( radian ), 0.0,
			0.0,            0.0,           1.0
		);
	mat *= matrix_type::_33(
			1.0, 0.0, -cx,
			0.0, 1.0, -cy,
			0.0, 0.0, 1.0
		);
}

/// @brief calculate scaling matrix
//! @param[out] mat   is scaling matrix
//! @param[in]  sx,sy is scale
//! 
template< class T, class Allocator >
void scaling_matrix( matrix< T, Allocator > &mat, typename matrix< T, Allocator >::value_type x, typename matrix< T, Allocator >::value_type y )
{
	typedef matrix< T > matrix_type;
	mat = matrix_type::_33(
			 x,  0.0, 0.0,
			0.0,  y,  0.0,
			0.0, 0.0, 1.0
		);
}

/// @brief calculate homography matrix
//! @param[out] mat   is homography matrix
//! @param[in]  p1,p2 are input matrix(2x4)
//! @return success of calculation
//! 
template< class T, class Allocator >
bool homography_matrix( matrix< T, Allocator > &mat, const matrix< T, Allocator > &p1, const matrix< T, Allocator > &p2 )
{
	if( p1.rows() != 2 || p1.cols() != 4 ||
		p2.rows() != 2 || p2.cols() != 4 )
	{
		throw( numerical_exception( 1, "Incorrect input matrix size is specified." ) );
		return( false );
	}

	typedef matrix< double > matrix_type;

	matrix_type tm( 8, 9 );
	double w = 1.0;
	for( int i = 0 ; i < 4 ; ++i )
	{
		tm( i * 2 + 0, 0 ) = 0;
		tm( i * 2 + 0, 1 ) = 0;
		tm( i * 2 + 0, 2 ) = 0;
		tm( i * 2 + 0, 3 ) = - w * p1( 0, i );
		tm( i * 2 + 0, 4 ) = - w * p1( 1, i );
		tm( i * 2 + 0, 5 ) = -w;
		tm( i * 2 + 0, 6 ) = p2( 1, i ) * p1( 0, i );
		tm( i * 2 + 0, 7 ) = p2( 1, i ) * p1( 1, i );
		tm( i * 2 + 0, 8 ) = p2( 1, i ) * w;

		tm( i * 2 + 1, 0 ) = w * p1( 0, i );
		tm( i * 2 + 1, 1 ) = w * p1( 1, i );
		tm( i * 2 + 1, 2 ) = w;
		tm( i * 2 + 1, 3 ) = 0;
		tm( i * 2 + 1, 4 ) = 0;
		tm( i * 2 + 1, 5 ) = 0;
		tm( i * 2 + 1, 6 ) = -p2( 0, i ) * p1( 0, i );
		tm( i * 2 + 1, 7 ) = -p2( 0, i ) * p1( 1, i );
		tm( i * 2 + 1, 8 ) = -p2( 0, i ) * w;
	}

	// Singular Value Decomposition
	matrix_type u, s, vt;
	svd( tm, u, s, vt );

	// Compute projection matrix from last row of the matrix
	mat.resize( 3, 3 );
	w = 1.0 / vt( 8, 8 );
	mat( 0, 0 ) = vt( 8, 0 ) * w;
	mat( 0, 1 ) = vt( 8, 1 ) * w;
	mat( 0, 2 ) = vt( 8, 2 ) * w;

	mat( 1, 0 ) = vt( 8, 3 ) * w;
	mat( 1, 1 ) = vt( 8, 4 ) * w;
	mat( 1, 2 ) = vt( 8, 5 ) * w;

	mat( 2, 0 ) = vt( 8, 6 ) * w;
	mat( 2, 1 ) = vt( 8, 7 ) * w;
	mat( 2, 2 ) = vt( 8, 8 ) * w;

	return( true );
}

namespace detail
{
	template< class T, class Allocator >
	void transform_point( int sx, int sy, double &dx, double &dy, const matrix< T, Allocator > &mat )
	{
		dx = ( mat( 0, 0 ) * sx + mat( 0, 1 ) * sy + mat( 0, 2 ) ) / ( mat( 2, 0 ) * sx + mat( 2, 1 ) * sy + mat( 2, 2 ) );
		dy = ( mat( 1, 0 ) * sx + mat( 1, 1 ) * sy + mat( 1, 2 ) ) / ( mat( 2, 0 ) * sx + mat( 2, 1 ) * sy + mat( 2, 2 ) );
	}

	template< class T, class Allocator >
	void clipping( int sx0, int sy0, int sx1, int sy1, 
				   int &dx0, int &dy0, int &dx1, int &dy1,
				   const matrix< T, Allocator > &mat )
	{
		double tx[ 4 ], ty[ 4 ];
		transform_point( sx0, sy0, tx[ 0 ], ty[ 0 ], mat );
		transform_point( sx1, sy0, tx[ 1 ], ty[ 1 ], mat );
		transform_point( sx0, sy1, tx[ 2 ], ty[ 2 ], mat );
		transform_point( sx1, sy1, tx[ 3 ], ty[ 3 ], mat );

		dx0 = 1000000;
		dx1 = 0;
		dy0 = 1000000;
		dy1 = 0;
		for( int i = 0 ; i < 4 ; ++i )
		{
			dx0 = ( dx0 < static_cast< int >( tx[ i ] ) ) ? dx0 : static_cast< int >( tx[ i ] );
			dx1 = ( dx1 > static_cast< int >( tx[ i ] ) ) ? dx1 : static_cast< int >( tx[ i ] );
			dy0 = ( dy0 < static_cast< int >( ty[ i ] ) ) ? dy0 : static_cast< int >( ty[ i ] );
			dy1 = ( dy1 > static_cast< int >( ty[ i ] ) ) ? dy1 : static_cast< int >( ty[ i ] );
		}
	}
}

namespace nearest
{
	/// @brief transform array2
	//! @param[in]  in  is input array
	//! @param[out] out is output array
	//! @param[int] mat is transformation matrix
	//! 
	template< class T1, class Allocator1, class T2, class Allocator2 >
	void transform( const array2< T1, Allocator1 > &in, array2< T1, Allocator1 > &out, const matrix< T2, Allocator2 > &mat )
	{
		matrix< double > m = inverse( mat );

		int w = static_cast< int >( out.width() );
		int h = static_cast< int >( out.height() );
		int iw = static_cast< int >( in.width( ) );
		int ih = static_cast< int >( in.height( ) );

		int dx0, dy0, dx1, dy1;
		detail::clipping( 0, 0, iw, ih, dx0, dy0, dx1, dy1, mat );

		//	for( int y = ( ( dy0 > 0 ) ? dy0 : 0 ) ; y < ( ( dy1 < h ) ? dy1 : h ) ; ++y )
		for( int y = 0 ; y < h ; ++y )
		{
		  //			for( int x = ( ( dx0 > 0 ) ? dx0 : 0 ) ; x < ( ( dx1 < w ) ? dx1 : w ) ; ++x )
			for( int x = 0 ; x < w ; ++x )
			{
			  double sx, sy;
			  detail::transform_point( x, y, sx, sy, m );
			  int ix = static_cast< int >( sx );
			  int iy = static_cast< int >( sy );			  

			  if( 0 <= ix && ix < iw && 0 <= iy && iy < ih )
			    {
			      out( x, y ) = in( ix, iy );
			    }
			}
		}
	}
}

namespace linear
{
	/// @brief transform array2
	//! @param[in]  in  is input array
	//! @param[out] out is output array
	//! @param[int] mat is transformation matrix
	//! 
	template< class T1, class Allocator1, class T2, class Allocator2 >
	void transform( const array2< T1, Allocator1 > &in, array2< T1, Allocator1 > &out, const matrix< T2, Allocator2 > &mat )
	{
		matrix< double > m = inverse( mat );

		int w = static_cast< int >( out.width( ) );
		int h = static_cast< int >( out.height( ) );
		int iw = static_cast< int >( in.width( ) );
		int ih = static_cast< int >( in.height( ) );

		int dx0, dy0, dx1, dy1;
		detail::clipping( 0, 0, iw, ih, dx0, dy0, dx1, dy1, mat );
		
#pragma omp parallel for schedule( guided )
		for( int y = 0 ; y < h ; ++y )		
		  //		for( int y = ( ( dy0 > 0 ) ? dy0 : 0 ) ; y < ( ( dy1 < h ) ? dy1 : h ) ; ++y )
		{
		  for( int x = 0 ; x < w ; ++x )
		    //			for( int x = ( ( dx0 > 0 ) ? dx0 : 0 ) ; x < ( ( dx1 < w ) ? dx1 : w ) ; ++x )
			{
				double sx, sy;
				detail::transform_point( x, y, sx, sy, m );

				int ix = static_cast< int >( sx );
				int iy = static_cast< int >( sy );
				double dx = sx - ix;
				double dy = sy - iy;

				if( 0 <= ix && ix < iw - 1 && 0 <= iy && iy < ih - 1 )
				{
					typedef typename array2< T1, Allocator1 >::value_type value_type; 
					value_type tmp = in( ix, iy ) * ( 1.0 - dx ) * ( 1.0 - dy )
								   + in( ix + 1, iy ) * dx * ( 1.0 - dy )
								   + in( ix, iy + 1 ) * ( 1.0 - dx ) * dy
								   + in( ix + 1, iy + 1 ) * dx * dy;
					out( x, y ) = tmp;
				}
			}
		}
	}

	/// @brief 平行移動と拡大を考慮して変形する
	//!
	//! @param[in]  in              … 入力画像
	//! @param[out]  out            … 基準画像
	//! @param[in] delta_x          … x方向位置ずれ
	//! @param[in] delta_y          … y方向位置ずれ
	//! @param[in] factor           … 拡大率
	//!
	//! @return なし
	//! 
	template< class Value_type1, class Allocator1, class Value_type2, class Allocator2 >
	void transform( const mist::array2< Value_type1, Allocator1 > &in, mist::array2< Value_type2, Allocator2 > &out, const double &delta_x, const double &delta_y )
	{
		vector2< double > factor( out.width() / in.width(), out.height() / in.height() );
		//out.fill( 0 );

#pragma omp parallel for schedule( guided )
		for( int y = 0 ; y < static_cast< int >(out.height()) ; y++ )
		{
			size_t ii[ 4 ], jj[ 4 ];
			double fy = y / factor.y - delta_y;
			if( fy < 0 )
			{
				fy = 0;
			}
			else if( fy > static_cast< int >(in.height()) - 1 )
			{
				fy = in.height() - 1;
			}

			jj[ 1 ] = static_cast< size_t >( fy );
			jj[ 2 ] = jj[ 1 ] < in.height() - 1 ? jj[ 1 ] + 1 : jj[ 1 ];
			fy -= jj[ 1 ];
			for( size_t x = 0 ; x < out.width() ; x++ )
			{
				double fx = x / factor.x - delta_x;

				if( fx < 0 )
				{
					fx = 0;
				}
				else if( fx > in.width() - 1 )
				{
					fx = in.width() - 1;
				}
				ii[ 1 ] = static_cast< size_t >( fx );
				ii[ 2 ] = ii[ 1 ] < in.width() - 1 ? ii[ 1 ] + 1 : ii[ 1 ];
				fx -= ii[ 1 ];

				out( x, y ) = mist::__linear__::_linear_< is_color< Value_type2 >::value >::interpolate( in, ii[ 1 ], ii[ 2 ], jj[ 1 ], jj[ 2 ], 0, 0, fx, fy, 0 ) ;
			}
		}
	}
}

namespace cubic
{
	/// @brief transform array2
	//! @param[in]  in  is input array
	//! @param[out] out is output array
	//! @param[int] mat is transformation matrix
	//! 
	template< class T1, class Allocator1, class T2, class Allocator2 >
	void transform( const array2< T1, Allocator1 > &in, array2< T1, Allocator1 > &out, const matrix< T2, Allocator2 > &mat )
	{
		matrix< double > m = inverse( mat );

		int w = static_cast< int >( out.width( ) );
		int h = static_cast< int >( out.height( ) );
		int iw = static_cast< int >( in.width( ) );
		int ih = static_cast< int >( in.height( ) );

		int dx0, dy0, dx1, dy1;
		detail::clipping( 0, 0, iw, ih, dx0, dy0, dx1, dy1, mat );
		
#pragma omp parallel for schedule( guided )
		for( int y = 0 ; y < h ; ++y )	
		  //		for( int y = ( ( dy0 > 0 ) ? dy0 : 0 ) ; y < ( ( dy1 < h ) ? dy1 : h ) ; ++y )
		{
			size_t ii[ 4 ], jj[ 4 ], kk[ 4 ];
		  for( int x = 0 ; x < w ; ++x )
		    //			for( int x = ( ( dx0 > 0 ) ? dx0 : 0 ) ; x < ( ( dx1 < w ) ? dx1 : w ) ; ++x )
			{
				double sx, sy;
				detail::transform_point( x, y, sx, sy, m );

				int ix = static_cast< int >( sx );
				int iy = static_cast< int >( sy );
				jj[ 1 ] = static_cast< size_t >( sy );
				jj[ 0 ] = jj[ 1 ] > 0      ? jj[ 1 ] - 1 : jj[ 1 ];
				jj[ 2 ] = jj[ 1 ] < ih - 1 ? jj[ 1 ] + 1 : jj[ 1 ];
				jj[ 3 ] = jj[ 2 ] < ih - 1 ? jj[ 2 ] + 1 : jj[ 2 ];
				sy -= jj[ 1 ];

				ii[ 1 ] = static_cast< size_t >( sx );
				ii[ 0 ] = ii[ 1 ] > 0      ? ii[ 1 ] - 1 : ii[ 1 ];
				ii[ 2 ] = ii[ 1 ] < iw - 1 ? ii[ 1 ] + 1 : ii[ 1 ];
				ii[ 3 ] = ii[ 2 ] < iw - 1 ? ii[ 2 ] + 1 : ii[ 2 ];
				sx -= ii[ 1 ];

				if( 0 <= ix && ix < iw && 0 <= iy && iy < ih )
				{
					out( x, y ) = mist::__cubic__::_cubic_< false >::interpolate( in, ii, jj, kk, sx, sy, 0 ) ;
				}
			}
		}
	}

	/// @brief 平行移動と拡大を考慮して変形する
	//!
	//! @param[in]  in              … 入力画像
	//! @param[out]  out            … 基準画像
	//! @param[in] delta_x          … x方向位置ずれ
	//! @param[in] delta_y          … y方向位置ずれ
	//! @param[in] factor           … 拡大率
	//!
	//! @return なし
	//! 
	template< class Value_type1, class Allocator1, class Value_type2, class Allocator2 >
	void transform( const mist::array2< Value_type1, Allocator1 > &in, mist::array2< Value_type2, Allocator2 > &out, const double &delta_x, const double &delta_y )
	{
		vector2< double > factor( out.width() / in.width(), out.height() / in.height() );
		//out.fill( 0 );
#pragma omp parallel for schedule( guided )
		for( int y = 0 ; y < static_cast< int >(out.height()) ; y++ )
		{
			size_t ii[ 4 ], jj[ 4 ], kk[ 4 ];
			double fy = y / factor.y - delta_y;
			if( fy < 0 )
			{
				fy = 0;
			}
			else if( fy > static_cast< int >(in.height()) - 1 )
			{
				fy = in.height() - 1;
			}

			jj[ 1 ] = static_cast< size_t >( fy );
			jj[ 0 ] = jj[ 1 ] > 0      ? jj[ 1 ] - 1 : jj[ 1 ];
			jj[ 2 ] = jj[ 1 ] < in.height() - 1 ? jj[ 1 ] + 1 : jj[ 1 ];
			jj[ 3 ] = jj[ 2 ] < in.height() - 1 ? jj[ 2 ] + 1 : jj[ 2 ];
			fy -= jj[ 1 ];
			for( size_t x = 0 ; x < out.width() ; x++ )
			{
				double fx = x / factor.x - delta_x;

				if( fx < 0 )
				{
					fx = 0;
				}
				else if( fx > in.width() - 1 )
				{
					fx = in.width() - 1;
				}
				ii[ 1 ] = static_cast< size_t >( fx );
				ii[ 0 ] = ii[ 1 ] > 0      ? ii[ 1 ] - 1 : ii[ 1 ];
				ii[ 2 ] = ii[ 1 ] < in.width() - 1 ? ii[ 1 ] + 1 : ii[ 1 ];
				ii[ 3 ] = ii[ 2 ] < in.width() - 1 ? ii[ 2 ] + 1 : ii[ 2 ];
				fx -= ii[ 1 ];

				out( x, y ) = mist::__cubic__::_cubic_< is_color< Value_type2 >::value >::interpolate( in, ii, jj, kk, fx, fy, 0 ) ;
			}
		}
	}
}

_MIST_END


#endif
