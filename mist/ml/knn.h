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

#ifndef __INCLUDE_MIST_KNN__
#define __INCLUDE_MIST_KNN__

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX_H__
#include "../matrix.h"
#endif

#include <map>
#include <algorithm>

_MIST_BEGIN

namespace knn
{
	//! @brief knn classifier
	class classifier
	{
	private:
		struct distance_pair
		{
			int index;
			double distance;

			bool operator<( const distance_pair &a ) const
			{
				return distance < a.distance;
			}
		};

	protected:
		matrix< double > in_;
		array1< int > response_;
		int category_count_;

	public:
		classifier( ) : category_count_( 0 )
		{
		}

		/// @biref train dataset
		//! @param[in]     in is input data
		//! @param[in]     response is target value of in
		//! 
		bool train( const matrix< double > &in, const array1< int > &response )
		{
			if( in.cols( ) != response.size( ) )
			{
				return false;
			}

			in_ = in;
			response_ = response;

			// calculate # of category
			std::map< int, bool > rmap;
			category_count_ = 0;
			for( size_t i = 0 ; i < response.size() ; ++i )
			{
				if( rmap.find( response( i ) ) == rmap.end() )
				{
					rmap[ response( i ) ] = true;
					++category_count_;
				}
			}

			return true;
		}

		/// @biref predict data
		//! @param[in]     in is input data
		//! @param[in]     k is # of knn
		//! @return prediction of in
		//! 
		int predict( const matrix< double > &in, int k = 1 )
		{
			array< distance_pair > mes( in_.cols() );
			array< int > vote( category_count_ );

			// calculate distance from training dataset
			for( size_t i = 0 ; i < in_.cols() ; ++i )
			{
				double dist = 0;
				for( size_t j = 0 ; j < in_.rows() ; ++j )
				{
					dist += pow( in_( j, i ) - in( j, 0 ), 2.0 );
				}
				mes( i ).index = i;
				mes( i ).distance = dist;
			}

			// sort by ascending order
			std::sort( mes.begin(), mes.end() );

			for( int i = 0 ; i < k ; ++i )
			{
				++vote( response_( mes( i ).index ) );
			}

			// predict category
			int max_count = 0;
			int max_idx = 0;
			for( int i = 0 ; i < category_count_ ; ++i )
			{
				if( vote( i ) > max_count )
				{
					max_count = vote( i );
					max_idx = i;
				}
			}

			return max_idx;
		}
	};
}

_MIST_END

#endif
