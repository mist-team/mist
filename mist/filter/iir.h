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
		array1<double> buf(order + 2, 0);

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

}


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_IIR_FILTER_H__
