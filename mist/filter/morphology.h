#ifndef __INCLUDE_MIST_MORPHOLOGY__
#define __INCLUDE_MIST_MORPHOLOGY__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "../config/type_trait.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#include <vector>


// mist名前空間の始まり
_MIST_BEGIN


namespace __morphology__
{
	struct point
	{
		unsigned short x;
		unsigned short y;
		unsigned short z;
	};

	struct morphology_structure
	{
		typedef std::vector< point > list_type;
		list_type object;
		list_type leftside;
		list_type rightside;
	};
}

namespace __erosion__
{
	template < class Array1, class Array2 >
	void erosion( const Array1 &in, Array2 &out, const __morphology__::morphology_structure &object
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type out_value_type;
		typedef __morphology__::point point;
		typedef __morphology__::morphology_structure::list_type list_type;

		const list_type &o = object.object;
		const list_type &l = object.leftside;
		const list_type &r = object.rightside;
		list_type::const_iterator ite;

		difference_type  i, j, k, p, leftnum;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		size_type bw = fw / 2;
		size_type bh = fh / 2;
		size_type bd = fd / 2;

		value_type *leftmost = new value_type[ fh * fd ];
		value_type *sort = new value_type[ fw * fh * fd + 1 ];
		hist_value *hist = new hist_value[range];

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				for( ite = o.begin( ) ; ite != o.end( ) ; ++ite )
				{
				}
			}
		}
	}
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MORPHOLOGY__
