// 
// Copyright (c) 2003-2009, MIST Project, Intelligent Media Integration COE, Nagoya University
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

#pragma once

#ifndef __INCLUDE_MIST_KMEANS__
#define __INCLUDE_MIST_KMEANS__

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_RANDOM__
#include "../random.h"
#endif

#ifndef __INCLUDE_MIST_CRITERIA__
#include "criteria.h"
#endif

_MIST_BEGIN

namespace kmeans
{
  /*!
    @brief kmeans clastering algorithm
    @param[in] in is input data
    @param[in,out] k is number of claster
    @param[out] response is kmeans result
    @param[out] center is centroid of claster
    @param[in] crt is criteria
  */
  inline void clastering( 
			 const mist::matrix< double > &in, 
			 int &k,
			 mist::array1< int > &response,
			 mist::matrix< double > &center,
			 mist::criteria crt = mist::criteria( mist::criteria::iteration, 0.0, 20 ) )
  {
    response.resize( in.cols() );
    center.resize( in.rows(), k );

    // calculate value range(min-max)
    mist::matrix< double > min( in.rows(), 1 );
    mist::matrix< double > max( in.rows(), 1 );
    for( size_t i = 0 ; i < in.rows() ; ++i )
      {
	min( i, 0 ) =  1e12;
	max( i, 0 ) = -1e12;
	for( size_t j = 0 ; j < in.cols() ; ++j )
	  {
	    min( i, 0 ) = std::min( min( i, 0 ), in( i, j ) );
	    max( i, 0 ) = std::max( max( i, 0 ), in( i, j ) );
	  }
      }

    // initialize centroid[0-1]
    mist::uniform::random rnd;
    for( int i = 0 ; i < k ; ++i )
      {
	for( size_t j = 0 ; j < in.rows() ; ++j )
	  {
	    center( j, i ) = min( j, 0 ) + rnd.real3() * ( max( j, 0 ) - min( j, 0 ) );
	  }
      }

    mist::array1< bool > is_set( k );									

    for( int m = 0 ; ( crt.type & mist::criteria::iteration ) ? ( m < crt.max_itr ) : true ; ++m )
      {
	bool is_finish = true;
	is_set.fill( false );

	// allocate to centroid
	for( size_t i = 0 ; i < in.cols() ; ++i )
	  {
	    int minidx = 0;
	    double mindist = 1e12;
	    for( int j = 0 ; j < k ; ++j )
	      {
		double dist = 0;
		for( size_t l = 0 ; l < in.rows() ; ++l )
		  {	
		    dist += pow( center( l, j ) - in( l, i ), 2.0 );
		  }

		if( dist < mindist )
		  {
		    mindist = dist;
		    minidx = j;
		  }
	      }

	    //
	    if( response( i ) != minidx )
	      {
		response( i ) = minidx;
		is_finish = false;
	      }
	    is_set( minidx ) = true;
	  }

	// re-calculate centroid
	for( int i = 0 ; i < k ; ++i )
	  {		
	    for( size_t j = 0 ; j < center.rows() ; ++j )
	      {
		center( j, i ) = 0;
	      }

	    int cnt = 0;
	    for( size_t j = 0 ; j < in.cols() ; ++j )
	      {
		if( response( j ) == i )
		  {
		    for( size_t l = 0 ; l < center.rows() ; ++l )
		      {
			center( l, i ) += in( l, j );
		      }
		    ++cnt;
		  }
	      }

	    if( cnt > 0 )
	      {
		for( size_t j = 0 ; j < center.rows() ; ++j )
		  {
		    center( j, i ) /= cnt;
		  }
	      }
	  }

	// check finish condition
	if( is_finish )
	  {
	    break;
	  }
      }


    mist::array1< int > idx_map( k );
    int cnt = 0;
    for( size_t i = 0 ; i < is_set.size() ; ++i )
      {
	if( is_set( i ) )
	  {
	    idx_map( i ) = cnt++;
	  }
	else
	  {
	    idx_map( i ) = -1;
	  }
      }

    if( k > cnt )
      {
	// reduce k
	mist::matrix< double > tmp = center;
	center.resize( in.rows(), cnt );

	int c = 0;
	for( size_t i = 0 ; i < tmp.cols() ; ++i )
	  {
	    if( is_set( i ) )
	      {
		for( size_t j = 0 ; j < tmp.rows() ; ++j )
		  {
		    center( j, c ) = tmp( j, i );
		  }
		++c;
	      }
	  }

	for( size_t i = 0 ; i < response.size() ; ++i )
	  {
	    response( i ) = idx_map( response( i ) );
	  }
	k = cnt;
      }
  }
}


_MIST_END

#endif

