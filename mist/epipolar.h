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

/// @file mist/epipolar.h
//!
//! @brief epipolar geometry
//!
#ifndef __INCLUDE_MIST_EPIPOLAR__
#define __INCLUDE_MIST_EPIPOLAR__

#ifndef __INCLUDE_MIST_H__
#include <mist/mist.h>
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include <mist/matrix.h>
#endif

#ifndef __INCLUDE_NUMERIC__
#include <mist/numeric.h>
#endif

#ifndef __INCLUDE_MIST_RANDOM__
#include <mist/random.h>
#endif

#include <queue>

_MIST_BEGIN


namespace __epipolar__
{


	/// @brief enforce rank 2 on a fundamental matrix of rank 3.
	//! 
	//! The rank is forced to be 2 using SVD.
	//! in and out can be the same instance.
	//!
	//! @param[in]   in is a input fundamental matrix.
	//! @param[out]  out is a output fundamental matrix.
	//!
	//! @return return true if success
	//! 
	template < typename T >
	inline bool enforce_rank2( matrix< T > &in, matrix< T > &out )
	{
		matrix < T > u, s, vt;

		if ( (in.rows() != 3) || (out.cols() != 3) )
		{
			return false;
		}

		svd( in, u, s, vt );
		s( 2, 2 ) = static_cast< T > ( 0.0 );
		out = u * s * vt;

		return true;
	}




} // namespace __epipolar__


/// @brief compute a fundamental matrix from 8 or more correspondence of the points.
//!
//! The fundamental matrix is computed using 8 point algorithm.
//! 
//! @param[in]  points1 is an array of points on image1. The size is 2xN or 3xN.
//! @param[in]  points2 is an array of points on image2. The size is 2xN or 3xN.
//! @param[out] fundamental_matrix is a fundamental matrix. The size is 3x3.
//!
//! @return return true if success
//! 
template < typename T1, typename T2 >
inline bool compute_fundamental_matrix( matrix< T1 > &points1,
						   				matrix< T1 > &points2,
										matrix< T2 > &fundamental_matrix )
{

	if ( (points1.cols() != points2.cols()) || (points1.cols() < 8) )
	{
		return false;
	}

	size_t point_num = points1.cols();

	matrix< double > m( point_num, 9 );

	for ( size_t i = 0; i < point_num; i++ )
	{
		m( i, 0 ) = points1( 0, i ) * points2( 0, i );
		m( i, 1 ) = points1( 1, i ) * points2( 0, i );
		m( i, 2 ) =                   points2( 0, i );
		m( i, 3 ) = points1( 0, i ) * points2( 1, i );
		m( i, 4 ) = points1( 1, i ) * points2( 1, i );
		m( i, 5 ) =                   points2( 1, i );
		m( i, 6 ) = points1( 0, i );
		m( i, 7 ) = points1( 1, i );
		m( i, 8 ) = 1.0;
	}

	m = m.t() * m;
	
	// compute an eigen vector for the minimnum eigen value
	matrix< double > eval, evec;

	eigen( m, eval, evec );

	fundamental_matrix.resize( 3, 3 );

	if ( _DESCENDING_ORDER_EIGEN_VALUE_ == 0 )
	{
		fundamental_matrix( 0, 0 ) = static_cast< T2 > ( evec( 0, 0 ) );
		fundamental_matrix( 0, 1 ) = static_cast< T2 > ( evec( 1, 0 ) );
		fundamental_matrix( 0, 2 ) = static_cast< T2 > ( evec( 2, 0 ) );
		fundamental_matrix( 1, 0 ) = static_cast< T2 > ( evec( 3, 0 ) );
		fundamental_matrix( 1, 1 ) = static_cast< T2 > ( evec( 4, 0 ) );
		fundamental_matrix( 1, 2 ) = static_cast< T2 > ( evec( 5, 0 ) );
		fundamental_matrix( 2, 0 ) = static_cast< T2 > ( evec( 6, 0 ) );
		fundamental_matrix( 2, 1 ) = static_cast< T2 > ( evec( 7, 0 ) );
		fundamental_matrix( 2, 2 ) = static_cast< T2 > ( evec( 8, 0 ) );
	}
	else
	{
		fundamental_matrix( 0, 0 ) = static_cast< T2 > ( evec( 0, evec.cols() - 1 ) );
		fundamental_matrix( 0, 1 ) = static_cast< T2 > ( evec( 1, evec.cols() - 1 ) );
		fundamental_matrix( 0, 2 ) = static_cast< T2 > ( evec( 2, evec.cols() - 1 ) );
		fundamental_matrix( 1, 0 ) = static_cast< T2 > ( evec( 3, evec.cols() - 1 ) );
		fundamental_matrix( 1, 1 ) = static_cast< T2 > ( evec( 4, evec.cols() - 1 ) );
		fundamental_matrix( 1, 2 ) = static_cast< T2 > ( evec( 5, evec.cols() - 1 ) );
		fundamental_matrix( 2, 0 ) = static_cast< T2 > ( evec( 6, evec.cols() - 1 ) );
		fundamental_matrix( 2, 1 ) = static_cast< T2 > ( evec( 7, evec.cols() - 1 ) );
		fundamental_matrix( 2, 2 ) = static_cast< T2 > ( evec( 8, evec.cols() - 1 ) );
	}

	__epipolar__::enforce_rank2( fundamental_matrix, fundamental_matrix );

	return true;
}




/// @brief compute epipolar lines on the image.
//!
//! Epipolar line is calculated by "l = F * p" if which_image is 0 or "l = F^T * p" if which_image is 1.
//! The line is represented as "a*x + b*y + c = 0", where "l = [a, b, c]^T".
//!
//! @param[in]   points is a array of points. The size is 2xN or 3xN.
//! @param[in]   which_image is an index of the image including the points (0 or 1).
//! @param[in]   fundamental matrix is a input fundamental matrix.
//! @param[out]  correspondent_lines are epipolar lines which corresponds to the points. The size is 3xN.
//!
//! @return return true if success
//!
template < typename T1, typename T2 >
inline bool compute_correspond_epilines( matrix< T1 > &points,
								   		 int which_image,
										 matrix< T2 > &fundamental_matrix,
										 matrix< T1 > &correspondent_lines )
{

	size_t point_num = points.cols();	

	matrix< T1 > p( 3, point_num );

	for ( size_t i = 0; i < point_num; i++ )
	{
		p( 0, i ) = points( 0, i );
		p( 1, i ) = points( 1, i );
		p( 2, i ) = static_cast< T1 > ( 1.0 );
	}


	if ( which_image == 0 )
	{
		correspondent_lines = fundamental_matrix * p;
	}
	else
	{
		correspondent_lines = fundamental_matrix.t() * p;
	}

	return true;
}



_MIST_END


#endif // __INCLUDE_MIST_EPIPOLAR__