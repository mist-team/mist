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
