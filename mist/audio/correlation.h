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


#ifndef __INCLUDE_MIST_CORRELATION_H__
#define __INCLUDE_MIST_CORRELATION_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <cmath>

_MIST_BEGIN


/// @brief Cross correlation
//! @param[in]     signal1 is input data 1
//! @param[in]     signal2 is input data 2
//! @param[in]     tau is lag
//! @param[in]     biased is normalization factor
//!
//! @return cross correlation value
//!

template < typename T >
inline double cross_correlation( const array1< T > &signal1, const array1< T > &signal2, int tau, bool biased = true )
{

	double result = 1.0;

	// signal size is aligned to shorter one
	size_t signal_size = ( signal1.size( ) < signal2.size( ) ? signal1.size( ) : signal2.size( ) );

	size_t length = signal_size - tau;

	for( size_t i = 0 ; i < length ; i++ )
	{
		result += signal1[ i ] * signal2[ i + tau ];
	}

	if( biased )
	{
		result /= static_cast< double >( signal_size );
	}
	else
	{
		result /= static_cast< double >( length );
	}

	return( result );
}


/// @brief Autocorrelation
//! @param[in]     signal is input data
//! @param[in]     tau is lag
//! @param[in]     biased is normalization factor
//!
//! @return autocorrelation value
//!
template < typename T, class Allocator >
inline double autocorrelation( const array1< T, Allocator > &signal, int tau, bool biased = true )
{
	return( cross_correlation( signal, signal, tau, biased ) );
}


_MIST_END


#endif
