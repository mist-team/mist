/// @file mist/mixture.h
//!
//! @brief EMアルゴリズムを用いた混合分布の推定
//!
//! - 参考文献
//!   - ・・・
//!
#ifndef __INCLUDE_MIXTURE__
#define __INCLUDE_MIXTURE__

#include <iostream>
#include <cmath>
#include <vector>

#include "mist/mist.h"


// mist名前空間の始まり
_MIST_BEGIN


#define EMALGORITHM_DEBUG	0


//! @defgroup statistics_group 統計処理
//!
//!  @{


//! @addtogroup mixture_group EMアルゴリズムを用いた混合分布の推定
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/mixture.h>
//! @endcode
//!
//! @{


/// @brief 混合分布推定に用いるパラメータ
namespace mixture
{
	/// @brief 混合分布を推定するための分布パラメータ
	struct distribution
	{
		double	weight; 	///< 混合重み
		double	av; 		///< 平均
		double	sd; 		///< 標準偏差

		distribution( ) : weight( 1.0 ), av( 0.0 ), sd( 1.0 ){ }
	};

	inline std::ostream &operator <<( std::ostream &out, const distribution &a )
	{
		out << "( " << a.weight << ", " << a.av << ", " << a.sd << " )";
		return( out );
	}
}


/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in]     pdp           … 分布パラメータ
//! @param[in]     nComponents   … 推定する混合分布の数
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in,out] fEpsilon      … 対数尤度の打ち切り精度
//! @param[out]    nIteration    … 実際のループ回数
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator > &rSamples,
							mixture::distribution *pdp,
							typename array< T, Allocator >::size_type nComponents,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon, typename array< T, Allocator >::size_type &nIteration
						)
{
	if( rSamples.empty( ) || nComponents == 0 )
	{
		return( false );
	}

	typedef typename array< T, Allocator >::size_type size_type;

	size_type nSamples = rSamples.size( );
	size_type k, m, n;
	//const double	pi = atan( 1.0f ) * 4.0f;
	const double pi = 3.1415926535897932384626433832795;
	const double _2pi = std::sqrt( 2.0 * pi );
	double fLikelihood;
	double fLastLikelihood = -1.0e30;
	double tmp;

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	// VC6ではSTLのアロケータの定義が、標準に準拠していないので、デフォルトで代用する
	array2< double > Weight( nSamples, nComponents );
#else
	array2< double, typename Allocator::template rebind< double >::other > Weight( nSamples, nComponents );
#endif



#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
	printf( "%d, %f, %f, %f, %f, %f, %f\n", 0, pdp[ 0 ].av, pdp[ 0 ].sd, pdp[ 0 ].weight, pdp[ 1 ].av, pdp[ 1 ].sd, pdp[ 1 ].weight );
#endif


	// 初期分布データの重みの和を１に正規化する
	tmp = 0.0;
	for( m = 0 ; m < nComponents ; m++ )
	{
		tmp += pdp[ m ].weight;
	}

	if( tmp <= 0.0 )
	{
		return( false );
	}

	for( m = 0 ; m < nComponents ; m++ )
	{
		pdp[ m ].weight /= tmp;
	}


	// EMアルゴリズムの開始
	for( n = 0 ; n < nMaxIteration ; n++ )
	{
		// E-step

		for( k = 0 ; k < nSamples ; k++ )
		{
			tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				double v = pdp[ m ].weight * ( 1.0 / ( _2pi * pdp[ m ].sd ) ) * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) );
				Weight( k, m ) = v;
				tmp += v;
			}

			if( tmp == 0.0 )
			{
				// 重みの合計が１にならないエラー
				return( false );
			}
			else
			{
				for( size_type m = 0 ; m < nComponents ; m++ )
				{
					Weight( k, m ) /= tmp;
				}
			}
		}

		// M-step
		for( m = 0 ; m < nComponents ; m++ )
		{
			double	weight_sum = 0;
			double	average = 0;
			double	variance = 0;

			for( k = 0 ; k < nSamples ; k++ )
			{
				weight_sum += Weight( k, m );
				average += static_cast< double >( rSamples[ k ] ) * Weight( k, m );
			}

			if( weight_sum > 0.0 )
			{
				pdp[ m ].weight = weight_sum / static_cast< double >( nSamples );
				pdp[ m ].av = average / weight_sum;

				for( k = 0 ; k < nSamples ; k++ )
				{
					variance += Weight( k, m ) * ( rSamples[ k ] * rSamples[ k ] - pdp[ m ].av * pdp[ m ].av );
				}

				variance /= weight_sum;
			}
			else
			{
				// 重みの合計が１にならないエラー
				return( false );
			}


			pdp[ m ].sd = std::sqrt( variance );
		}


		double weight_sum = 0;
		for( m = 0 ; m < nComponents ; m++ )
		{
			weight_sum += pdp[ m ].weight;
		}

		if( std::abs( weight_sum - 1.0 ) > 0.1 )
		{
			// 重みの合計が１にならないエラー
			return( false );
		}

		fLikelihood = 0.0;

		for( k = 0 ; k < nSamples ; k++ )
		{
			tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				tmp += Weight( k, m ) * pdp[ m ].weight * ( 1.0 / ( _2pi * pdp[ m ].sd ) ) * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) );
			}

			if( tmp == 0.0 )
			{
				return( false );
			}

			fLikelihood += std::log( tmp );
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		printf( "%d (%f %f %f) (%f %f %f) %f\n", n, pdp[ 0 ].av, pdp[ 0 ].sd, pdp[ 0 ].weight, pdp[ 1 ].av, pdp[ 1 ].sd, pdp[ 1 ].weight, fLikelihood );
#endif

		if( fLikelihood - fLastLikelihood < fEpsilon )
		{
			break;
		}

		fLastLikelihood = fLikelihood;
	}

	return( true );
}



/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in]     pdp           … 分布パラメータ
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in,out] fEpsilon      … 対数尤度の打ち切り精度
//! @param[out]    nIteration    … 実際のループ回数
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator >& rSamples, std::vector< mixture::distribution > &pdp,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon, typename array< T, Allocator >::size_type &nIteration
						)
{
	return( estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), nMaxIteration, fEpsilon, nIteration ) );
}


/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in]     pdp           … 分布パラメータ
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in,out] fEpsilon      … 対数尤度の打ち切り精度
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator >& rSamples,
							std::vector< mixture::distribution > &pdp,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon
						)
{
	typename array< T, Allocator >::size_type nIteration = 0;
	return( estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), nMaxIteration, fEpsilon, nIteration ) );
}


/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in]     pdp           … 分布パラメータ
//! @param[in]     nComponents   … 推定する混合分布の数
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in,out] fEpsilon      … 対数尤度の打ち切り精度
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class T, class Allocator >
bool estimate_mixture(
							const mist::array< T, Allocator > &rSamples,
							mixture::distribution *pdp,
							typename array< T, Allocator >::size_type nComponents,
							typename array< T, Allocator >::size_type nMaxIteration,
							double	fEpsilon
						)
{
	typename array< T, Allocator >::size_type nIteration = 0;
	return( estimate_mixture( rSamples, pdp, nComponents, nMaxIteration, fEpsilon, nIteration ) );
}


/// @}
//  EMアルゴリズムを用いた混合分布の推定グループの終わり

/// @}
//  統計処理グループの終わり



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIXTURE__
