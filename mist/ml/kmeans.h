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

#include <cstdio>

_MIST_BEGIN

namespace kmeans
{
	namespace detail
	{
		double nearestCenter( const matrix< double > &in, int col, const matrix< double > &center, int n, int &idx )
		{
			double minv = 1e12;

			for( int i = 0 ; i < n ; ++i )
			{
				double dist = 0.0;
				for( int j = 0 ; j < static_cast< int >( in.rows() ) ; ++j )
				{
					dist += pow( in( j, col ) - center( j, i ), 2.0 );
				}
				if( dist < minv )
				{
					minv = dist;
					idx = i;
				}
			}

			return minv;
		}
	}

	/// @brief kmeans clustering algorithm
	//! @param[in]     in is input data
	//! @param[in,out] k is number of cluster
	//! @param[out]    response is kmeans result
	//! @param[out]    center is centroid of cluster
	//! @param[in]     crt is criteria
	//! 
	inline void clustering( 
		const matrix< double > &in, 
		int &k,
		array1< int > &response,
		matrix< double > &center,
		criteria crt = criteria( criteria::iteration, 0.0, 20 ) )
	{
		response.resize( in.cols() );
		center.resize( in.rows(), k );

		// calculate value range(min-max)
		matrix< double > minv( in.rows(), 1 );
		matrix< double > maxv( in.rows(), 1 );
		for( size_t i = 0 ; i < in.rows() ; ++i )
		{
			minv( i, 0 ) =  1e12;
			maxv( i, 0 ) = -1e12;
			for( size_t j = 0 ; j < in.cols() ; ++j )
			{
				minv( i, 0 ) = min( minv( i, 0 ), in( i, j ) );
				maxv( i, 0 ) = max( maxv( i, 0 ), in( i, j ) );
			}
		}
		/*
		// initialize centroid[0-1]
		uniform::random rnd;
		for( int i = 0 ; i < k ; ++i )
		{
		for( size_t j = 0 ; j < in.rows() ; ++j )
		{
		center( j, i ) = minv( j, 0 ) + rnd.real3() * ( maxv( j, 0 ) - minv( j, 0 ) );
		}
		}
		*/

		// initialize centroid using kmeans++
		// kmeans++: David Arthur, Sergei Vassilvitskii, "k-means++: The Advantages of Careful Seeding," Proc. SODA, 2007
		uniform::random rnd;
		for( int i = 0 ; i < k ; ++i )
		{
			if( i == 0 )
			{
				for( int j = 0 ; j < static_cast< int >( in.rows() ) ; ++j )
				{
					center( j, i ) = minv( j, 0 ) + rnd.real3() * ( maxv( j, 0 ) - minv( j, 0 ) );
				}
			}
			else
			{
				/*
				double total = 0.0;
				for( int l = 0 ; l < in.cols() ; ++l )
				{
				total += detail::nearestCenter( in, l, center, i );
				}
				*/
				double max_prob = 0.0;
				int max_idx = 0;
				for( int l = 0 ; l < static_cast< int >( in.cols() ) ; ++l )
				{
					int idx;
					double prob = detail::nearestCenter( in, l, center, i, idx );// / total;
					if( prob > max_prob )
					{
						max_prob = prob;
						max_idx = l;
					}
				}

				for( int j = 0 ; j < static_cast< int >( in.rows() ) ; ++j )
				{
					center( j, i ) = in( j, max_idx );
				}
			}
		}

		array1< bool > is_set( k );											
		for( int m = 0 ; ( crt.type & criteria::iteration ) ? ( m < crt.max_itr ) : true ; ++m )
		{
			bool is_finish = true;
			is_set.fill( false );

			// allocate to centroid
			for( size_t i = 0 ; i < in.cols() ; ++i )
			{
				int minidx = 0;
				/*
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
				*/
				detail::nearestCenter( in, i, center, k, minidx );
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


		array1< int > idx_map( k );
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
			matrix< double > tmp = center;
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

