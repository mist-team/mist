/// @file mist/filter/iir.h
//!
//! @brief IIR�t�B���^��K�p���邽�߂̃��C�u����
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

// mist���O��Ԃ̎n�܂�
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


// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_FILTER_IIR_FILTER_H__
