/// @file mist/statistics.h
//!
//! @brief データの統計処理
//!
#ifndef __INCLUDE_MIST_STATISTICS__
#define __INCLUDE_MIST_STATISTICS__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN


/// データの平均値を計算する
//!
//! @param[in] a … 平均値を計算するデータ配列
//! 
template < class T, class Allocator >
inline double average( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	if( a.empty( ) )
	{
		return( 0.0 );
	}
	else
	{
		double v = 0.0;
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			v += static_cast< double >( a[ i ] );
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// データの標準偏差を計算する
//!
//! @param[in] a   … 標準偏差を計算するデータ配列
//! @param[in] ave … データ配列の平均値
//! 
template < class T, class Allocator >
inline double standard_deviation( const array< T, Allocator > &a, double ave )
{
	typedef typename array< T, Allocator >::size_type size_type;
	if( a.empty( ) )
	{
		return( 0.0 );
	}
	else
	{
		double v = 0.0;
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			double x = static_cast< double >( a[ i ] ) - ave;
			v += x * x;
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// データの標準偏差を計算する
//!
//! @param[in] a … 標準偏差を計算するデータ配列
//! 
template < class T, class Allocator >
inline double standard_deviation( const array< T, Allocator > &a )
{
	return( standard_deviation( a, average( a ) ) );
}

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
