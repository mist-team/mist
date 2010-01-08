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

/// @file mist/filter/iir.h
//!
//! @brief IIRフィルタを適用するためのライブラリ
//!

#ifndef __INCLUDE_FILTER_IIR_FILTER_H__
#define __INCLUDE_FILTER_IIR_FILTER_H__


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


namespace __iir_filter__
{

	template<class T, class Allocator>
	void iir(
		const array1<T, Allocator>& in,
		array1<T, Allocator>& out,
		int order,
		const array1<T, Allocator>& a,
		const array1<T, Allocator>& b)
	{
		array1<double> buf(order + 2, 0.0);

		out.resize(in.size());

		double output;
		for(int n = 0; n < in.size(); n ++)
		{
			out[n] = output = b[0] * in[n] + buf[1];

			for(int k = 1; k < order + 1; k ++)
			{
				buf[k] = (b[k] * in[n]) - (a[k] * output) + buf[k + 1];
			}
		}
	}


    template<class T, class Allocator>
    void iir_cascade(
        const array1<T, Allocator>& in,
        array1<T, Allocator>& out,
        int order,
		int blocks,
        const array2<T, Allocator>& a,
        const array2<T, Allocator>& b)
    {
        array2<double> buf(blocks + 1, order + 2, 0.0);

        out.resize(in.size());

        double output;
        for(int n = 0; n < in.size(); n ++)
        {
			buf(0, 0) = in[n];

			for(int blk = 1; blk < blocks + 1; blk ++)
			{
				buf(blk, 0) = output = b(blk - 1, 0) * buf(blk - 1, 0) + buf(blk, 1);

				for(int k = 1; k < order + 1; k ++)
				{
					buf(blk, k) = (b(blk - 1, k) * buf(blk - 1, 0)) - (a(blk - 1, k) * output) + buf(blk, k + 1);
				}
			}

			out[n] = buf(blocks, 0);
        }
    }

}


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_IIR_FILTER_H__
