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


#ifndef __INCLUDE_MIST_PARCOR_H__
#define __INCLUDE_MIST_PARCOR_H__

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <cmath>

_MIST_BEGIN



/// @brief Calculate PARCOR and linear predict cofficients from autocorrelation values
//! @param[in]     cor is autocorrelation values (the length should be p + 1)
//! @param[out]    alf is linear predict cofficients
//! @param[out]    ref is PARCOR cofficients
//! @param[in]     p is number of cofficients
//!
//! @return residual
//!
template < class T1, class Allocator1, class T2, class Allocator2, class T3, class Allocator3 >
inline double parcor( const array1< T1, Allocator1 > &cor, array1< T2, Allocator2 > &alf, array1< T3, Allocator3 > &ref, int p )
{
	// by Levinson-Durbin algorithm
	double res = cor[ 0 ]; // residual

	alf.resize( p );
	ref.resize( p );

	array1< double > alf_old;

	size_t length = p;

	for( size_t i = 0 ; i < length ; i++ )
	{
		double r = cor[ i + 1 ];

		for( size_t j = 0 ; j < i ; j++ )
		{
			r += alf_old[ j ] * cor[ i - j ];
		}

		ref[ i ] = static_cast < T3 >( r / res );

		alf[ i ] = -ref[ i ];

		for( size_t j = 0 ; j < i ; j++ )
		{
			alf[ j ] = static_cast < T2 >( alf_old[ j ] - ref[ i ] * alf_old[ i - j - 1 ] );
		}

		res *= ( 1.0 - ref[ i ] ) * ( 1.0 + ref[ i ] );

		alf_old = alf;

	}

	return( res );
}


_MIST_END


#endif
