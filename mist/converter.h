#ifndef __INCLUDE_CONVERTER__
#define __INCLUDE_CONVERTER__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


namespace __converter__
{
	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array< T1, Allocator1 > &in, mist::array< T2, Allocator2 > &out )
	{
		out.resize( in.size( ) );
	}

	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array1< T1, Allocator1 > &in, mist::array1< T2, Allocator2 > &out )
	{
		out.resize( in.size( ) );
		out.reso1( in.reso1( ) );
	}

	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array2< T1, Allocator1 > &in, mist::array2< T2, Allocator2 > &out )
	{
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
	}

	template < class T1, class T2, class Allocator1, class Allocator2 >
	inline void resize( const mist::array3< T1, Allocator1 > &in, mist::array3< T2, Allocator2 > &out )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
	}

	template < bool b1, bool b2 >
	struct _converter_
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = static_cast< T2 >( in );
		}
	};

	template < >
	struct _converter_< true, false >
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = static_cast< T2 >( in.get_value( ) );
		}
	};

	template < >
	struct _converter_< false, true >
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = in;
		}
	};

	template < >
	struct _converter_< true, true >
	{
		template < class T1, class T2 >
		static void convert( const T1 &in, T2 &out )
		{
			out = in;
		}
	};

	template < class T1, class T2 >
	inline void converter( const T1 &in, T2 &out )
	{
		_converter_< is_color< T1 >::value, is_color< T2 >::value >::convert( in, out );
	}
}

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class Array1, class Array2 >
inline void convert( const Array1 &in, Array2 &out )
{
	typedef typename Array1::size_type  size_type;
	typedef typename Array1::value_type value_type;
	typedef typename Array2::value_type out_value_type;
	typedef unsigned short hist_value;

	__converter__::resize( in, out );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		__converter__::converter( in[ i ], out[ i ] );
	}
}



// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_CONVERTER__
