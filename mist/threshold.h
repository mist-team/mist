/// @file mist/threshold.h
//!
//! @brief 任意の画像から自動的にしきい値を決定するライブラリ
//!
#ifndef __INCLUDE_MIST_THRESHOLD__
#define __INCLUDE_MIST_THRESHOLD__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN



//! @addtogroup threshold_group しきい値
//!  @{


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
namespace ptile
{
	/// @brief p-tile法による閾値決定
	//!
	//! ratio は 0 ～ 100％の範囲で指定する
	//!
	//! @param[in] in    … 引数の説明
	//! @param[in] ratio … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	template < class T, class Allocator >
	typename array< T, Allocator >::value_type threshold( const array< T, Allocator > &in, double ratio )
	{
		typedef typename array< T, Allocator >::size_type  size_type;
		typedef typename array< T, Allocator >::value_type value_type;

		value_type min = in[ 0 ];
		value_type max = in[ 0 ];

		size_type i;

		for( i = 1 ; i < in.size( ) ; i++ )
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

		size_type level = static_cast< size_type >( max - min ) + 1;
		size_type *hist = new size_type[ level ];
		size_type N = in.size( );

		for( i = 0 ; i < level ; i++ )
		{
			hist[ i ] = 0;
		}
		for( i = 0 ; i < N ; i++ )
		{
			hist[ static_cast< size_type >( in[ i ] - min ) ]++;
		}

		size_type pix_num = 0;
		size_type th = 0;

		while( ( pix_num * 100.0 ) / static_cast< double >( N ) < ratio )
		{
			pix_num += hist[ th ];
			th++;
		}

		delete [] hist;

		return( static_cast< value_type >( th + min ) );
	}
}



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
namespace discriminant_analysis
{
	/// @brief 判別分析法による閾値決定
	//!
	//! ratio は 0 ～ 100％の範囲で指定する
	//!
	//! @param[in] in    … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	template < class T, class Allocator >
	typename array< T, Allocator >::value_type threshold( const array< T, Allocator > &in )
	{
		typedef typename array< T, Allocator >::size_type  size_type;
		typedef typename array< T, Allocator >::value_type value_type;

		value_type min = in[ 0 ];
		value_type max = in[ 0 ];

		size_type i, k;

		for( i = 1 ; i < in.size( ) ; i++ )
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

		size_type level = static_cast< size_type >( max - min ) + 1;
		size_type max_k;
		double *p   = new double[ level ];
		double myu, omg;
		double myuT, sig, max_sig = 0.0;

		size_type N = in.size( );
		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] = 0.0;
		}
		for( k = 0 ; k < N ; k++ )
		{
			p[ static_cast< size_type >( in[ k ] - min ) ]++;
		}
		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] /= static_cast< double >( N );
		}

		myuT = 0.0;
		for( k = 0 ; k < level ; k++ )
		{
			myuT += k * p[ k ];
		}

		myu = 0.0;
		omg = p[ 0 ];
		max_k = 0;
		max_sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
		for( k = 1 ; k < level ; k++ )
		{
			omg = omg + p[ k ];
			myu = myu + k * p[ k ];
			sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
			if( sig > max_sig )
			{
				max_sig = sig;
				max_k = k;
			}
		}

		delete [] p;

		return( static_cast< value_type >( max_k + min ) );
	}
}

/// @}
//  しきい値グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_THRESHOLD__
