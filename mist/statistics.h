/// @file mist/statistics.h
//!
//! @brief データの統計処理
//!
#ifndef __INCLUDE_MIST_STATISTICS__
#define __INCLUDE_MIST_STATISTICS__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


namespace __utility__
{
	template < bool b >
	struct ____value_type____
	{
		typedef double value_type;
	};

	template < >
	struct ____value_type____< true >
	{
		typedef rgb< double > value_type;
	};

	template < class T >
	struct __value_type__
	{
		typedef typename ____value_type____< is_color< T >::value >::value_type value_type;
	};
}

//! @defgroup statistics_group 統計処理
//!  @{



/// データの平均値を計算する
//!
//! @param[in] a … 平均値を計算するデータ配列
//! 
template < class T, class Allocator >
inline typename __utility__::__value_type__< T >::value_type average( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	typedef typename array< T, Allocator >::value_type value_type;
	typedef typename __utility__::__value_type__< T >::value_type value_type;

	if( a.empty( ) )
	{
		return( value_type( ) );
	}
	else
	{
		value_type v = value_type( );
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			v += a[ i ];
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// データの分散を計算する
//!
//! @param[in] a   … 分散を計算するデータ配列
//! @param[in] ave … データ配列の平均値
//! 
template < class T, class Allocator >
inline typename __utility__::__value_type__< T >::value_type deviation( const array< T, Allocator > &a, double ave )
{
	typedef typename array< T, Allocator >::size_type size_type;
	typedef typename array< T, Allocator >::value_type value_type;
	typedef typename __utility__::__value_type__< T >::value_type value_type;

	if( a.empty( ) )
	{
		return( value_type( ) );
	}
	else
	{
		value_type v = value_type( );
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			value_type x = a[ i ] - ave;
			v += x * x;
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// データの分散を計算する
//!
//! @param[in] a … 分散を計算するデータ配列
//! 
template < class T, class Allocator >
inline typename __utility__::__value_type__< T >::value_type deviation( const array< T, Allocator > &a )
{
	return( deviation( a, average( a ) ) );
}



/// @}
//  統計処理の終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
