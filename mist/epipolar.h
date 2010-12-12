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
#include <cmath>

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


	mist::matrix< double >
	skew_symmetric_matrix3( mist::matrix< double > &vec )
	{
	    mist::matrix< double > mat(3, 3);
	    mat(0, 0) = 0.0;     mat(0, 1) = -vec[2];  mat(0, 2) = vec[1];
	    mat(1, 0) = vec[2];  mat(1, 1) = 0.0;      mat(1, 2) = -vec[0];
	    mat(2, 0) = -vec[1]; mat(2, 1) = vec[0];   mat(2, 2) = 0.0;
	    return mat;
	}	

	double levi_civita3( int i, int j, int k )
	{
	    if ( (i == j) || (j == k) || (k == i) )
	    {
		return 0.0;
	    }

	    if ( ((j - i + 3) % 3 == 1 ) &&
		 ((k - j + 3) % 3 == 1 ) &&
		 ((i - k + 3) % 3 == 1 )  )
	    {
		return 1.0;
	    }

	    return -1.0;
	}

	/// @brief test whether the line and the triangle are intersected
	//! 
	//! This is the implimentation of Tomas's algorithm.
	//!
	//! @param[in]   origin is an arbitrary point on the line (3x1)
	//! @param[in]   direction is an direction vector of the line (3x1)
	//! @param[in]   vert0 is the first vertix of the triangle (3x1)
	//! @param[in]   vert1 is the second vertix of the triangle (3x1)
	//! @param[in]   vert2 is the third vertix of the triangle (3x1)
	//! @param[out]  intersection is parameters of the intersection (3x1)
	//!
	//! @return return true if intersected
	//! 
	bool line_triangle_intersection( matrix< double > &origin,
					 matrix< double > &direction,
					 matrix< double > &vert0,
					 matrix< double > &vert1,
					 matrix< double > &vert2,
					 matrix< double > &intersection )
	{
	    matrix< double > e1 = vert1 - vert0;
	    matrix< double > e2 = vert2 - vert0;
	    matrix< double > t = origin - vert0;
	    matrix< double > p = skew_symmetric_matrix3( direction ) * e2;
	    matrix< double > q = skew_symmetric_matrix3( t ) * e1;

	    intersection.resize(3, 1);
	    intersection[0] = (q.t() * e2)[0];
	    intersection[1] = (p.t() * t)[0];
	    intersection[2] = (q.t() * direction)[0];

	    intersection /= (p.t() * e1)[0];

	    if ( (intersection[1] >= 0.0) && (intersection[2] >= 0.0) &&
		 (intersection[1] + intersection[2] <= 1 ) )
	    {
		return true;
	    }
	    
	    return false;
	}
	
					

} // namespace __epipolar__


/// @brief trifocal tensor
template < typename T >
class trifocal_tensor
{
private:
    array1< matrix< T > > data_;

public:

    /// @brief default constructor
    trifocal_tensor()
    {
	data_.resize( 3 );
	data_[0].resize( 3, 3 );
	data_[1].resize( 3, 3 );
	data_[2].resize( 3, 3 );	
    }

    /// @brief Return reference of the i'th slice of the tensor
    matrix< T >& operator[] (size_t index)
    {
	return data_[ index ];
    }

    /// @brief Return the reference of the element of the tensor
    T& operator() ( size_t i, size_t q, size_t r )
    {
	return data_[i](q, r);
    }

};


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


/// @brief compute epipole from a fundamental matrix
//!
//! @param[in]   fundamental matrix is a input fundamental matrix.
//! @param[out]  epipole is a (3, 1) matrix
//!
//! @return return true if success
//!
template < typename T1, typename T2 >
inline bool compute_epipole( matrix< T1 > &fundamental_matrix,
			     matrix< T2 > &epipole )
{

    if ( (fundamental_matrix.rows() != 3) ||
	 (fundamental_matrix.cols() != 3) )
    {
	return false;
    }

    epipole.resize(3, 1);

    mist::matrix< double > u, s, vt;
    svd( fundamental_matrix, u, s, vt );
    epipole[0] = u(0, 2);
    epipole[1] = u(1, 2);
    epipole[2] = u(2, 2);

    // normalize the norm
    T2 norm = 0.0;
    norm += epipole[0] * epipole[0];
    norm += epipole[1] * epipole[1];
    norm += epipole[2] * epipole[2];
    norm = sqrt( norm );
    epipole /= norm;

    return true;
}


/// @brief factorize essential matrix
//!
//! factorize E to SR,
//! where
//!   S = U Z U^t
//!   R1 = U W V^t    or    R2 = U W^t V^t
//!   E = U diag(1, 1, 0) V^t
//!   W = { (0,1,0)^t, (-1,0,0)^t, (0,0,1)^t }
//!   Z = { (0,-1,0)^t, (1,0,0)^t, (0,0,0)^t }
//!
//! @param[in]   essential_matrix is a input
//! @param[out]  t is the traslation vector
//! @param[out]  r1 is one of the solution of the rotation R
//! @param[out]  r2 is the other solution of the rotaion R
//!
//! @return return true if success
//!
template < typename T1, typename T2 >
inline bool factorize_essential_matrix( matrix< T1 > &essential_matrix,
					matrix< T2 > &t,
					matrix< T2 > &r1,
					matrix< T2 > &r2 )		
{

   

    if ( (essential_matrix.rows() != 3) ||
	 (essential_matrix.cols() != 3) )
    {
	return false;
    }


    matrix< T1 > essential = essential_matrix;
  
    mist::matrix< T1 > du, ds, dvt;
    svd( essential, du, ds, dvt );
    ds = mist::matrix< T1 >::_33( (ds(0, 0) + ds(1, 1)) * 0.5, 0.0, 0.0,
				  0.0, (ds(0, 0) + ds(1, 1)) * 0.5, 0.0,
				  0.0, 0.0, 0.0 );
    essential = du * ds * dvt;
    svd( essential, du, ds, dvt );


    mist::matrix< T1 > w;
    w = matrix< T1 >::_33( 0.0, -1.0, 0.0,
			   1.0,  0.0, 0.0,
			   0.0,  0.0, 1.0 );
    mist::matrix< T1 > z;    
    z = matrix< T1 >::_33( 0.0,  1.0, 0.0,
			   -1.0, 0.0, 0.0,
			   0.0,  0.0, 0.0 );
 
    mist::matrix< T1 > s = du * z * du.t();
    t.resize(3, 1);
    t[0] = s(2, 1);
    t[1] = s(0, 2);
    t[2] = s(1, 0);

    // normalize t
    double norm = sqrt(t[0] * t[0] + t[1] * t[1] + t[2] * t[2]);
    t[0] /= norm;
    t[1] /= norm;
    t[2] /= norm;    
    
    r1 = du * w * dvt;
    r2 = du * w.t() * dvt;
  

    return true;
}



/// @brief Estimate a coordinate of a point in world coordinate. The triangulation is performed by DLT method.
//!
//! @param[in]   xc1 is the point on the first camera
//! @param[in]   xc2 is the point on the second camera
//! @param[in]   camera_matrix1 is that for the first camera
//! @param[in]   camera_matrix2 is that for the second camera
//! @param[out]  xw is the point on the world coordinate
//!
//! @return return true if success
//!
template < typename T1, typename T2 >
inline bool triangulation_dlt( matrix< T1 > &xc1,
			       matrix< T1 > &xc2,
			       matrix< T2 > &camera_matrix1,
			       matrix< T2 > &camera_matrix2,
			       matrix< T1 > &xw )
{

    if ( (camera_matrix1.rows() != 3) || (camera_matrix1.cols() != 4) ||
	 (camera_matrix2.rows() != 3) || (camera_matrix2.cols() != 4) ||
	 (xc1.size() < 3) || (xc2.size() < 3) )
    {
	return false;
    }

    matrix< T1 > mat( 4, 4 );
    
    matrix< T1 > row;
    matrix< T1 > pt1( 1, 4 ), pt2( 1, 4 );

    // use four constraints

    // x coordinate, first camera
    for (size_t i = 0; i < 4; i++)
    {
	pt1(0, i) = camera_matrix1( 2, i );
    }
    for (size_t i = 0; i < 4; i++)
    {
	pt2(0, i) = camera_matrix1( 0, i );
    }
    row = xc1[0] * pt1 - pt2;
    for (size_t i = 0; i < 4; i++)
    {
	mat(0, i) = row[i];
    }

    // y coordinate, first camera
    for (size_t i = 0; i < 4; i++)
    {
	pt2(0, i) = camera_matrix1( 1, i );
    }
    row = xc1[1] * pt1 - pt2;
    for (size_t i = 0; i < 4; i++)
    {
	mat(1, i) = row[i];
    }    

    // x coordinate, second camera    
    for (size_t i = 0; i < 4; i++)
    {
	pt1(0, i) = camera_matrix2( 2, i );
    }
    for (size_t i = 0; i < 4; i++)
    {
	pt2(0, i) = camera_matrix2( 0, i );
    }
    row = xc2[0] * pt1 - pt2;
    for (size_t i = 0; i < 4; i++)
    {
	mat(2, i) = row[i];
    }
    
    // y coordinate, first camera    
    for (size_t i = 0; i < 4; i++)
    {
	pt2(0, i) = camera_matrix2( 1, i );
    }
    row = xc2[1] * pt1 - pt2;
    for (size_t i = 0; i < 4; i++)
    {
	mat(3, i) = row[i];
    }

    // the equation is solve by SVD
    matrix< T2 > u, s, vt;
    svd( mat, u, s, vt );
    
    xw.resize( 4, 1 );
    xw[0] = vt( 3, 0 ) / vt(3, 3);
    xw[1] = vt( 3, 1 ) / vt(3, 3);
    xw[2] = vt( 3, 2 ) / vt(3, 3);
    xw[3] = 1.0;

   
    return true;
}




/// @brief Generate trilinear relations from 3-view point-point-pont correspondences
//!
//!  Return A, where At=0 and t is the vector of the trifocal tensor components.
//!
//! @param[in]   p1 is a point on the first camera (3x1)
//! @param[in]   p2 is a point on the second camera (3x1)
//! @param[in]   p3 is a point on the third camera (3x1)
//!
//! @return Return trilinear relations (9x27)
//!
template < typename T1 >
matrix< T1 > trilinear_three_points( matrix< T1 > &p1,
				     matrix< T1 > &p2,
				     matrix< T1 > &p3 )
{
    matrix< T1 > mat(9, 27);

    for (int s = 0; s < 3; s++)
    {
	for (int t = 0; t < 3; t++)
	{
	    // each equation
	    for (int q = 0; q < 3; q++)
	    {
		for (int r = 0; r < 3; r++)
		{
		    for (int i = 0; i < 3; i++)
		    {
			// each tensor coefficient
			double sum = 0.0;
		
			for (int j = 0; j < 3; j++)
			{
			    for (int k = 0; k < 3; k++)
			    {
				double val = 1.0;			
				val *= p1[i];
				val *= p2[j];
				val *= p3[k];
				val *= __epipolar__::levi_civita3( j, q, s );
				val *= __epipolar__::levi_civita3( k, r, t );
				sum += val;
			    }
			}
			mat( 3 * s + t, 3 * 3 * i + 3 * q + r ) = sum;
		    }
		}
	    }
	}
    }

    return mat;
}


/// @brief Generate trilinear relations from 3-view line-line-line correspondence
//!
//!  Return A, where At=0 and t is the vector of the trifocal tensor components.
//!
//! @param[in]   l1 is a line on the first camera (3x1)
//! @param[in]   l2 is a line on the second camera (3x1)
//! @param[in]   l3 is a line on the third camera (3x1)
//!
//! @return trilinear relations (3x27)
//!
template < typename T1 >
matrix< T1 > trilinear_three_lines( matrix< T1 > &l1,
				    matrix< T1 > &l2,
				    matrix< T1 > &l3 )

{
    matrix< T1 > mat(3, 27);

    for (int w = 0; w < 3; w++)
    {
	// each equation
	for (int i = 0; i < 3; i++)
	{
	    for (int q = 0; q < 3; q++)
	    {
		for (int r = 0; r < 3; r++)
		{	
		    // each tensor coefficient
		    double sum = 0.0;
		    for (int p = 0; p < 3; p++)
		    {
			double val = 1.0;
			val *= l1[p];
			val *= l2[q];
			val *= l3[r];
			val *= __epipolar__::levi_civita3(p, i, w);
			sum += val;
		    }
		    mat( w, i * 3 * 3 + q * 3 + r ) = sum;
		}
	    }
	}
    }

    return mat;
}



/// @brief Compute trifocal tensor from trilinear relations.
//!
//! Solve At=0 using SVD, where t is the vector of the trifocal tensor components and A are the coefficients of the trilinear relations.
//!
//! @param[in]   relations is the trilinear relations (Nx27)
//! @param[out]  tensor is the output trifocal tensor
//!
//! @return Return true if success
//!
template < typename T1, typename T2 >
bool compute_trifocal_tensor( mist::matrix< T1 >    &relations,
			      trifocal_tensor< T2 > &tensor )
{
    if ( relations.cols() != 27 )
    {
	return false;
    }
    
    
    mist::matrix< T1 > u, s, vt;
    mist::svd( relations, u, s, vt );

   
    for (size_t i = 0; i < 3; i++)
    {
	for (size_t q = 0; q < 3; q++)
	{
	    for (size_t r = 0; r < 3; r++)
	    {
		tensor[i].resize(3, 3);
		tensor[i](q, r) = vt( vt.rows() - 1, i * 3 * 3 + q * 3 + r);
	    }
	}
    }

    return true;
}


/// @brief Compute epipoles from trifocal tensor 
//!
//! @param[in]   tensor is the input trifocal tensor
//! @param[out]  epi21 is the epipole in the second image (3x1)
//! @param[out]  epi31 is the epipole in the third image (3x1)
//!
//! @return Return true if success
//!
template < typename T1, typename T2 >
bool trifocal_tensor_to_epipoles( trifocal_tensor< T1 > &tensor,
				  mist::matrix< T2 > &epi21,
				  mist::matrix< T2 > &epi31 )
{
    epi21.resize(1, 3);
    epi31.resize(1, 3);

    mist::matrix< T1 > u, v;

    u.resize(3, 3);
    for (int i = 0; i < 3; i++)
    {

	mist::matrix< T1 > uu, s, vt;
	mist::svd( tensor[i], uu, s, vt );
	for (int j = 0; j < 3; j++)
	{
	    u(j, i) = uu(j, 2);
	}

	mist::matrix< T1 > vec(3, 1);
	vec[0] = u(0, i);
	vec[1] = u(1, i);
	vec[2] = u(2, i);
    }


    v.resize(3, 3);
    for (int i = 0; i < 3; i++)
    {
	mist::matrix< T1 > uu, s, vt;
	mist::svd( tensor[i], uu, s, vt );
	for (int j = 0; j < 3; j++)
	{
	    v(j, i) = vt(2, j);
	}
    }


    epi21.resize(3, 1);
    {

	mist::matrix< T1 > uu, s, vt;
	mist::svd( u, uu, s, vt );
	for (int j  = 0; j < 3; j++)
	{
	    epi21[j] = uu(j, 2);
	}

	double norm = sqrt( epi21[0] * epi21[0]
			    + epi21[1] * epi21[1]
			    + epi21[2] * epi21[2] );
	epi21 /= norm;

    }
    epi31.resize(3, 1);
    {

	mist::matrix< T1 > uu, s, vt;
	mist::svd( v, uu, s, vt );
	for (int j  = 0; j < 3; j++)
	{
	    epi31[j] = uu(j, 2);
	}
	double norm = sqrt( epi31[0] * epi31[0]
			    + epi31[1] * epi31[1]
			    + epi31[2] * epi31[2] );
	epi31 /= norm;


    }

    return true;
}



/// @brief Compute fundamental matrices from trifocal tensor
//!
//! @param[in]   tensor is the input trifocal tensor
//! @param[out]  fundamental21 is the fundamental matrix with the second image (3x3)
//! @param[out]  fundamental31 is the fundamental matrix with the third image (3x3)
//!
//! @return Return true if success
//!
template <typename T1, typename T2>
bool compute_fundamental_matrices( trifocal_tensor< T1 > &tensor,
				   mist::matrix< T2 > &fundamental21,
				   mist::matrix< T2 > &fundamental31 )
{

    // compute epipoles
    mist::matrix< T2 > epi21, epi31;
    trifocal_tensor_to_epipoles(tensor, epi21, epi31);

    fundamental21.resize(3, 3);
    fundamental31.resize(3, 3);    
    
    for (size_t i = 0; i < 3; i++)
    {
	mist::matrix< T2 > temp;
	
	temp = __epipolar__::skew_symmetric_matrix3( epi21 ) * tensor[i] * epi31;
	for (int j = 0; j < 3; j++)
	{
	    fundamental21(j, i) = temp(j, 0);
	}
	
	temp = __epipolar__::skew_symmetric_matrix3( epi31 ) * tensor[i].t() * epi21;
	for (int j = 0; j < 3; j++)
	{
	    fundamental31(j, i) = temp(j, 0);
	}
    }

    return true;
}



/// @brief Compute camera matrices from trifocal tensor 
//!
//! The first camera matrix is fixed as P = [I | 0].
//! The camera matrices are obtained up to projection ambiguity.
//!
//! @param[in]   tensor is the input trifocal tensor
//! @param[out]  mat2 is the camera matrix for the second image (3x4)
//! @param[out]  mat1 is the camera matrix for the third image (3x4)
//!
//! @return Return true if success
//!
template < typename T1, typename T2 >
bool trifocal_tensor_to_camera_matrices( trifocal_tensor< T1 > &tensor,
				       mist::matrix< T2 > &mat2,
				       mist::matrix< T2 > &mat3 )
{

    mat2.resize( 3, 4 );
    mat3.resize( 3, 4 );        
    matrix< T1 > vec1( 3, 1 ), vec2( 3, 1 );

    // epipoles
    matrix< T1 > epi21, epi31;
    if ( !trifocal_tensor_to_epipoles( tensor, epi21, epi31 ) )
    {
	return false;
    }


    // camera matrix for the second camera
    for (size_t i = 0; i < 3; i++)
    {
	vec2 = tensor[i] * epi31;
	mat2( 0, i ) = vec2[0];
	mat2( 1, i ) = vec2[1];
	mat2( 2, i ) = vec2[2];	
    }
    for (size_t i = 0; i < 3; i++)
    {
	mat2( i, 3 ) = epi21[i];
    }
    
    // camera matrix for third camera
    mist::matrix< T1 > temp_mat;
    temp_mat = epi31 * epi31.t() - matrix< T1 >::_33( 1.0, 0.0, 0.0,
						      0.0, 1.0, 0.0,
						      0.0, 0.0, 1.0 );
    for (size_t i = 0; i < 3; i++)
    {
	vec1 = temp_mat * tensor[i].t() * epi21;
	mat3( 0, i ) = vec1[0];
	mat3( 1, i ) = vec1[1];
	mat3( 2, i ) = vec1[2];	
    }
    for (size_t i = 0; i < 3; i++)
    {
	mat3( i, 3 ) = epi31[i];
    }    
    
    return true;
}


_MIST_END




#endif // __INCLUDE_MIST_EPIPOLAR__
