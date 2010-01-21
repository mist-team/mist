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

_MIST_BEGIN

/*!
  @brief calculate translation matrix
  @param[out] mat is translation matrix
  @param[in]  x,y is position
*/
template< typename T >
void translation_matrix( mist::matrix< T > &mat,
		  T x, T y )
{
  mat = mist::matrix< T >::_33( 1.0, 0.0, x,
				0.0, 1.0, y,
				0.0, 0.0, 1.0 );
}

/*!
  @brief calculate rotation matrix
  @param[out] mat    is rotation matrix
  @param[in]  radian is rotation angle
*/
template< typename T >
void rotation_matrix( mist::matrix< T > &mat,
	       T radian )
{
  mat = mist::matrix< T >::_33( cos( radian ), -sin( radian ), 0.0,
				sin( radian ), cos( radian ), 0.0,
				0.0, 0.0, 1.0 );
}

/*!
  @brief calculate scaling matrix
  @param[out] mat   is scaling matrix
  @param[in]  sx,sy is scale
*/
template< typename T >
void scaling_matrix( mist::matrix< T > &mat,
	      T x, T y )
{
  mat = mist::matrix< T >::_33( x, 0.0, 0.0,
				0.0, y, 0.0,
				0.0, 0.0, 1.0 );
}

/*!
  @brief calculate homography matrix
  @param[out] mat   is homography matrix
  @param[in]  p1,p2 are input matrix(2x4)
  @return success of calculation
*/
template < typename T >
bool homography_matrix( mist::matrix< T > &mat, 
			const mist::matrix< T > &p1, 
			const mist::matrix< T > &p2 )
{
  if( p1.rows() != 2 || p1.cols() != 4 ||
      p2.rows() != 2 || p2.cols() != 4 )
    {
      throw( numerical_exception( 1, "Incorrect input matrix size is specified." ) );
      return false;
    }
    
  mist::matrix< double > tm( 8, 9 );
  double w = 1.0;
  for( int i = 0; i < 4; ++i )
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
    
  // 特異値分解
  mist::matrix<double> u, s, vt;
  mist::svd( tm, u, s, vt );

  // 最終行から射影行列を作成
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
    
  return true;
}

namespace detail
{
  template< typename T >
  void transform_point( int sx, int sy, 
			double &dx, double &dy,
			const mist::matrix< T > &mat )
  {
    dx = ( mat( 0, 0 ) * sx + mat( 0, 1 ) * sy + mat( 0, 2 ) ) / ( mat( 2, 0 ) * sx + mat( 2, 1 ) * sy + mat( 2, 2 ) );
    dy = ( mat( 1, 0 ) * sx + mat( 1, 1 ) * sy + mat( 1, 2 ) ) / ( mat( 2, 0 ) * sx + mat( 2, 1 ) * sy + mat( 2, 2 ) );
  }

  template< typename T >
  void clipping( int sx0, int sy0, int sx1, int sy1, 
		int &dx0, int &dy0, int &dx1, int &dy1,
		const mist::matrix< T > &mat )
  {
    double tx[ 4 ], ty[ 4 ];
    transform_point( sx0, sy0, tx[ 0 ], ty[ 0 ], mat );
    transform_point( sx1, sy0, tx[ 1 ], ty[ 1 ], mat );
    transform_point( sx0, sy1, tx[ 2 ], ty[ 2 ], mat );
    transform_point( sx1, sy1, tx[ 3 ], ty[ 3 ], mat );

    dx0 = 1e6;
    dx1 = 0;
    dy0 = 1e6;
    dy1 = 0;
    for( int i = 0 ; i < 4 ; ++i )
      {
	dx0 = std::min( dx0, static_cast< int >( tx[ i ] ) );
	dx1 = std::max( dx1, static_cast< int >( tx[ i ] ) );
	dy0 = std::min( dy0, static_cast< int >( ty[ i ] ) );
	dy1 = std::max( dy1, static_cast< int >( ty[ i ] ) );
      }
  }
}

namespace nearest
{
  /*!
    @brief transform array2
    @param[in]  in  is input array
    @param[out] out is output array
    @param[int] mat is transformation matrix
  */
  template< typename T, typename T2 >
  void transform( const mist::array2< T > &in, mist::array2< T > &out, const mist::matrix< T2 > &mat )
  {
    mist::matrix< double > m = mist::inverse( mat );

    int dx0, dy0, dx1, dy1;
    detail::clipping( 0, 0, static_cast< int >( in.width() ), static_cast< int >( in.height() ),
		      dx0, dy0, dx1, dy1, mat );
    int w = static_cast< int >( out.width() );
    int h = static_cast< int >( out.height() );
    for( int y = ( ( dy0 > 0 ) ? dy0 : 0 ) ; y < ( ( dy1 < h ) ? dy1 : h ) ; ++y )
      {
	for( int x = ( ( dx0 > 0 ) ? dx0 : 0 ) ; x < ( ( dx1 < w ) ? dx1 : w ) ; ++x )
	  {
	    double sx =  (  m( 0, 0 ) * x + m( 0, 1 ) * y + m( 0, 2 ) ) / ( m( 2, 0 ) * x + m( 2, 1 ) * y + m( 2, 2 ) );
	    double sy =  (  m( 1, 0 ) * x + m( 1, 1 ) * y + m( 1, 2 ) ) / ( m( 2, 0 ) * x + m( 2, 1 ) * y + m( 2, 2 ) );

	    int ix = static_cast< int >( sx );
	    int iy = static_cast< int >( sy );

	    if( 0 <= ix && ix < static_cast< int >( in.width() ) &&
		0 <= iy && iy < static_cast< int >( in.height() ) )
	      {
		out( x, y ) = in( ix, iy );
	      }
	  }
      }
  }
}

namespace linear
{
  /*!
    @brief transform array2
    @param[in]  in  is input array
    @param[out] out is output array
    @param[int] mat is transformation matrix
  */
  template< typename T, typename T2 >
  void transform( const mist::array2< T > &in, mist::array2< T > &out, const mist::matrix< T2 > &mat )
  {
    mist::matrix< double > m = mist::inverse( mat );

    int dx0, dy0, dx1, dy1;
    detail::clipping( 0, 0, static_cast< int >( in.width() ), static_cast< int >( in.height() ),
		      dx0, dy0, dx1, dy1, mat );
    int w = static_cast< int >( out.width() );
    int h = static_cast< int >( out.height() );
    for( int y = ( ( dy0 > 0 ) ? dy0 : 0 ) ; y < ( ( dy1 < h ) ? dy1 : h ) ; ++y )
      {
	for( int x = ( ( dx0 > 0 ) ? dx0 : 0 ) ; x < ( ( dx1 < w ) ? dx1 : w ) ; ++x )
	  {
	    double sx =  (  m( 0, 0 ) * x + m( 0, 1 ) * y + m( 0, 2 ) ) / ( m( 2, 0 ) * x + m( 2, 1 ) * y + m( 2, 2 ) );
	    double sy =  (  m( 1, 0 ) * x + m( 1, 1 ) * y + m( 1, 2 ) ) / ( m( 2, 0 ) * x + m( 2, 1 ) * y + m( 2, 2 ) );

	    int ix = static_cast< int >( sx );
	    int iy = static_cast< int >( sy );
	    double dx = sx - ix;
	    double dy = sy - iy;

	    if( 0 <= ix && ix < static_cast< int >( in.width() - 1 ) &&
		0 <= iy && iy < static_cast< int >( in.height() ) - 1 )
	      {		
		out( x, y ) = static_cast< T >( static_cast< double >( in( ix, iy ) ) * ( 1.0 - dx ) * ( 1.0 - dy ) +
						static_cast< double >( in( ix + 1, iy ) ) * dx * ( 1.0 - dy ) +
						static_cast< double >( in( ix, iy + 1 ) ) * ( 1.0 - dx ) * dy +
						static_cast< double >( in( ix + 1, iy + 1 ) ) * dx * dy );
	      }
	  }
      }
  }
}


_MIST_END


#endif
