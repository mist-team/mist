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
//! @attention 入力・出力となるデータの配列として，MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナが利用可能です．
//!
//! @param[in] in  … ヒストグラムを作成するための元となるデータ配列（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] out … 作成されたヒストグラム（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] min … ヒストグラムを作成するデータの最小値
//! @param[in] max … ヒストグラムを作成するデータの最大値
//! @param[in] bin … ヒストグラムを作成するビン幅
//! 
//! @retval true  … ヒストグラムの作成に成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくはヒストグラムの作成用のデータが空
//! 
template < class Array1, class Array2 >
bool generate_histogram( const Array1 &in, Array2 &out, typename Array1::value_type min, typename Array1::value_type max, typename Array1::value_type bin = Array1::value_type( 1 ) )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array1::size_type size_type;
	typedef typename Array1::difference_type difference_type;
	typedef typename Array1::value_type value_type;

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

	for( size_type i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = 0;
	}

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		difference_type index = static_cast< difference_type >( ( in[ i ] - min ) / bin );
		if( index >= 0 && index < num )
		{
			out[ index ] += 1;
		}
	}

	return( true );
}


/// データ全体からヒストグラムを作成する
//!
//! @attention 入力・出力となるデータの配列として，MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナが利用可能です．
//!
//! @param[in] in  … ヒストグラムを作成するための元となるデータ配列（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] out … 作成されたヒストグラム（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] bin … ヒストグラムを作成するビン幅
//! 
//! @retval true  … ヒストグラムの作成に成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくはヒストグラムの作成用のデータが空
//! 
template < class Array1, class Array2 >
bool generate_histogram( const Array1 &in, Array2 &out, typename Array1::value_type bin = Array1::value_type( 1 ) )
{
	typedef typename Array1::size_type size_type;
	typedef typename Array1::difference_type difference_type;
	typedef typename Array1::value_type value_type;

	if( in.empty( ) )
	{
		return( false );
	}

	value_type min = in[ 0 ];
	value_type max = in[ 0 ];
	for( size_type i = 1 ; i < in.size( ) ; i++ )
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

	return( generate_histogram( in, out, min, max, bin ) );
}





/// データの範囲を指定して2次元ヒストグラムを作成する
//!
//! @attention 入力・出力となるデータの配列として，MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナが利用可能です．
//!
//! @param[in] in1  … ヒストグラムを作成するための元となるデータ配列1（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] in2  … ヒストグラムを作成するための元となるデータ配列2（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] out  … 作成されたヒストグラム（MISTで提供する2次元コンテナ array2 のみ）
//! @param[in] min1 … ヒストグラム1のデータの最小値
//! @param[in] max1 … ヒストグラム1のデータの最大値
//! @param[in] min2 … ヒストグラム2のデータの最小値
//! @param[in] max2 … ヒストグラム2のデータの最大値
//! @param[in] bin  … ヒストグラムを作成するビン幅
//! 
//! @retval true  … ヒストグラムの作成に成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくはヒストグラムの作成用のデータが空，入力となる2つのデータ数が異なる
//! 
template < class Array1, class Array2, class T, class Allocator >
bool generate_histogram(
							const Array1 &in1, const Array2 &in2, array2< T, Allocator > &out,
							typename Array1::value_type min1, typename Array1::value_type max1,
							typename Array1::value_type min2, typename Array1::value_type max2,
							typename Array1::value_type bin = Array1::value_type( 1 )
						)
{
	if( is_same_object( in1, out ) || is_same_object( in2, out ) || in1.empty( ) || in2.empty( ) || in1.size( ) != in2.size( ) )
	{
		return( false );
	}

	typedef typename Array1::size_type size_type;
	typedef typename Array1::difference_type difference_type;
	typedef typename Array1::value_type value_type;

	if( bin <= 0 )
	{
		return( false );
	}

	if( min1 > max1 )
	{
		value_type tmp = min1;
		min1 = max1;
		max1 = tmp;
	}

	if( min2 > max2 )
	{
		value_type tmp = min2;
		min2 = max2;
		max2 = tmp;
	}

	difference_type num1 = static_cast< size_type >( ( max1 - min1 + 1 ) / bin );
	difference_type num2 = static_cast< size_type >( ( max2 - min2 + 1 ) / bin );
	out.resize( num1, num2 );
	out.fill( );

	for( size_type i = 0 ; i < in1.size( ) ; i++ )
	{
		difference_type index1 = static_cast< difference_type >( ( in1[ i ] - min1 ) / bin );
		difference_type index2 = static_cast< difference_type >( ( in2[ i ] - min2 ) / bin );
		if( index1 >= 0 && index1 < num1 && index2 >= 0 && index2 < num2 )
		{
			out( index1, index2 ) += 1;
		}
	}

	return( true );
}


/// データ全体からヒストグラムを作成する
//!
//! @attention 入力・出力となるデータの配列として，MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナが利用可能です．
//!
//! @param[in] in1  … ヒストグラムを作成するための元となるデータ配列1（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] in2  … ヒストグラムを作成するための元となるデータ配列2（MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナ）
//! @param[in] out  … 作成されたヒストグラム（MISTで提供する2次元コンテナ array2 のみ）
//! @param[in] bin … ヒストグラムを作成するビン幅
//! 
//! @retval true  … ヒストグラムの作成に成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくはヒストグラムの作成用のデータが空，入力となる2つのデータ数が異なる
//! 
template < class Array1, class Array2, class T, class Allocator >
bool generate_histogram( const Array1 &in1, const Array2 &in2, array2< T, Allocator > &out, typename Array1::value_type bin = Array1::value_type( 1 ) )
{
	typedef typename Array1::size_type size_type;
	typedef typename Array1::difference_type difference_type;
	typedef typename Array1::value_type value_type;

	if( is_same_object( in1, out ) || is_same_object( in2, out ) || in1.empty( ) || in2.empty( ) || in1.size( ) != in2.size( ) )
	{
		return( false );
	}

	value_type min1 = in1[ 0 ];
	value_type max1 = in1[ 0 ];
	for( size_type i = 1 ; i < in1.size( ) ; i++ )
	{
		if( min1 > in1[ i ] )
		{
			min1 = in1[ i ];
		}
		else if( max1 < in1[ i ] )
		{
			max1 = in1[ i ];
		}
	}

	value_type min2 = in2[ 0 ];
	value_type max2 = in2[ 0 ];
	for( size_type i = 1 ; i < in2.size( ) ; i++ )
	{
		if( min2 > in2[ i ] )
		{
			min2 = in2[ i ];
		}
		else if( max2 < in2[ i ] )
		{
			max2 = in2[ i ];
		}
	}

	return( generate_histogram( in1, in2, out, min1, max1, min2, max2, bin ) );
}


/// @}
//  統計処理の終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
