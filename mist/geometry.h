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
    for( int i = 0; i < 8; ++i )
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


_MIST_END


#endif
