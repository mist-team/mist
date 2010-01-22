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


#ifndef __INCLUDE_MIST_WINDOW_FUNCTION_H__
#define __INCLUDE_MIST_WINDOW_FUNCTION_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <cmath>

_MIST_BEGIN


namespace window_function
{
	/// @brief Input data is multiplied by Hamming window function
	//! @param[in]     in is input data
	//! @param[out]    out is output data
	//!
	template < typename T, class Allocator >
	inline void hamming( const array1< T, Allocator > &in, array1< T, Allocator > &out )
	{
		out.resize( in.size( ) );

		size_t length = in.size( );

		for( size_t i = 0 ; i < length ; i++ )
		{
			double x = static_cast< double >( i ) / static_cast< double >( length );
			out[ i ] = in[ i ] * ( 0.54 - 0.46 * cos( 2.0 * 3.141592 * x ) );
		}
	}


	/// @brief Input data is multiplied by Blackman window function
	//! @param[in]     in is input data
	//! @param[out]    out is output data
	//!
	template < typename T, class Allocator >
	inline void blackman( const array1< T, Allocator > &in, array1< T, Allocator > &out )
	{
		out.resize( in.size( ) );

		size_t length = in.size( );

		for( size_t i = 0 ; i < length ; i++ )
		{
			double x = static_cast< double >( i ) / static_cast< double >( length );
			out[ i ] = in[ i ] * ( 0.42 - 0.5 * cos( 2.0 * 3.141592 * x ) + 0.08 * cos( 4.0 * 3.141592 * x ) );
		}
	}


	/// @brief Input data is multiplied by hann window function
	//! @param[in]     in is input data
	//! @param[out]    out is output data
	//!
	template < typename T, class Allocator >
	inline void hann( const array1< T, Allocator > &in, array1< T, Allocator > &out )
	{
		out.resize( in.size( ) );

		size_t length = in.size( );

		for( size_t i = 0 ; i < length ; i++ )
		{
			double x = static_cast< double >( i ) / static_cast< double >( length );
			out[ i ] = in[ i ] * ( 0.5 - 0.5 * cos( 2.0 * 3.141592 * x ) );
		}
	}

} // namespace window_function


_MIST_END


#endif
