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
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/statistics.h>
//! @endcode
//!
//!  @{



/// データの平均値を計算する
//!
//! @param[in] a … 平均値を計算するデータ配列
//! 
//! @return データの平均値
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
//! @return データの分散
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
//! @return データの分散
//! 
template < class T, class Allocator >
inline typename __utility__::__value_type__< T >::value_type deviation( const array< T, Allocator > &a )
{
	return( deviation( a, average( a ) ) );
}




/// データの範囲を指定してヒストグラムを作成する
//!
//! @param[in] in  … ヒストグラムを作成するデータ配列
//! @param[in] min … ヒストグラムを作成するデータの最小値
//! @param[in] max … ヒストグラムを作成するデータの最大値
//! @param[in] bin … ヒストグラムを作成するビン幅
//! 
//! @retval true  … ヒストグラムの作成に成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくはヒストグラムの作成用のデータが空
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool histogram( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
				typename array< T1, Allocator1 >::value_type min,
				typename array< T1, Allocator1 >::value_type max,
				typename array< T1, Allocator1 >::value_type bin = array< T1, Allocator1 >::value_type( 1 )
			  )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename array< T1, Allocator1 >::size_type size_type;
	typedef typename array< T1, Allocator1 >::difference_type difference_type;
	typedef typename array< T1, Allocator1 >::value_type value_type;

	if( bin <= 0 )
	{
		return( false );
	}

	if( min > max )
	{
		value_type tmp = min;
		min = max;
		max = tmp;
	}

	difference_type num = static_cast< size_type >( ( max - min + 1 ) / bin );
	out.resize( num );
	out.fill( );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		difference_type index = static_cast< difference_type >( ( in[ i ] - min ) / bin );
		if( index >= 0 && index < num )
		{
			out[ index ]++;
		}
	}

	return( true );
}


/// データ全体からヒストグラムを作成する
//!
//! @param[in] in  … ヒストグラムを作成するデータ配列
//! @param[in] bin … ヒストグラムを作成するビン幅
//! 
//! @retval true  … ヒストグラムの作成に成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくはヒストグラムの作成用のデータが空
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool histogram( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, typename array< T1, Allocator1 >::value_type bin = array< T1, Allocator1 >::value_type( 1 ) )
{
	typedef typename array< T1, Allocator1 >::size_type size_type;
	typedef typename array< T1, Allocator1 >::difference_type difference_type;
	typedef typename array< T1, Allocator1 >::value_type value_type;

	if( in.empty( ) )
	{
		return( false );
	}

	value_type min = in[ 0 ];
	value_type max = in[ 0 ];
	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		if( min > in[ i ] )
		{
			min = in[ i ];
		}
		else if( max < in[ i ] )
		{
			max = in[ i ];
		}
	}

	return( histogram( in, out, min, max, bin ) );
}



/// @}
//  統計処理の終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
