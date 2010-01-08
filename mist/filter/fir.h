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

/// @file mist/filter/fir.h
//!
//! @brief FIRフィルタを適用するためのライブラリ
//!

#ifndef __INCLUDE_FILTER_FIR_FILTER_H__
#define __INCLUDE_FILTER_FIR_FILTER_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "../config/type_trait.h"
#endif

#include <cmath>
#include <functional>

// mist名前空間の始まり
_MIST_BEGIN


namespace __fir_filter__
{

	template<class T, class Allocator>
	void fir(
		const array<T, Allocator>& in,
		array<T, Allocator>& out,
		int order,
		const array<T, Allocator>& coef,
	{
		array<double> buf(order, 0.0);

		out.resize(in.size());

		double output;
		for(int n = 0; n < in.size(); n ++)
		{
			for(int k = 0; k < order - 1; k ++)
			{
				buf[k] = coef[k] * in[n] + buf[k + 1];
			}
			buf[order - 1] = coef[order - 1] * in[n];

			out[n] = buf[0];
		}
	}

}


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_FIR_FILTER_H__
