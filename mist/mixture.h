// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/// @file mist/mixture.h
//!
//! @brief EMアルゴリズムを用いた混合分布の推定
//!
//! - 参考文献
//!   - ・・・
//!
#ifndef __INCLUDE_MIXTURE__
#define __INCLUDE_MIXTURE__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#include <cmath>
#include <vector>


// mist名前空間の始まり
_MIST_BEGIN


#define EMALGORITHM_DEBUG	0


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

	/// @brief 混合分布を推定するための分布パラメータ
	struct distribution2
	{
		typedef vector2< double > vector_type;

		double			weight;		///< 混合重み
		vector_type		av;			///< 平均
		double			v[ 4 ];		///< 共分散行列

		distribution2( ) : weight( 1.0 )
		{
			v[ 0 ] = v[ 3 ] = 1.0;
			v[ 1 ] = v[ 2 ] = 0.0;
		}
	};

	inline std::ostream &operator <<( std::ostream &out, const distribution2 &a )
	{
		out << "( " << a.weight << ", " << a.av << ", < " << a.v[ 0 ] << ", " << a.v[ 1 ] << ", " << a.v[ 2 ] << ", " << a.v[ 3 ] << " > )";
		return( out );
	}

	// dpで与えられる正規分布の(x,y)における値を返す。
	inline double gauss( const mist::mixture::distribution2 &dp, double x, double y )
	{
		double t = dp.v[ 0 ] * dp.v[ 3 ] - dp.v[ 1 ] * dp.v[ 2 ];
		double a = dp.v[ 3 ] / t;
		double b = -dp.v[ 1 ] / t;
		double c = -dp.v[ 2 ] / t;
		double d = dp.v[ 0 ] / t;
		x -= dp.av.x;
		y -= dp.av.y;
		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = 2.0 * pi;
		double vvv = ( a * x + b * y ) * x + ( c * x + d * y ) * y;
		return ( 1.0 / ( _2pi * sqrt( t ) ) * std::exp( - vvv / 2.0 ) );
	}

	inline double gauss( const mist::mixture::distribution &dp, double x )
	{
		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = std::sqrt( 2.0 * pi );
		double myu = x - dp.av;
		return ( 1.0 / ( _2pi * dp.sd ) * std::exp( - myu * myu / ( 2.0 * dp.sd * dp.sd ) ) );
	}
}


/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @attention 入力となるデータの配列として，通常のデータ配列，MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナが利用可能です．
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in,out] opdp          … 分布パラメータ
//! @param[in]     nSamples      … 入力サンプル数
//! @param[in]     nComponents   … 推定する混合分布の数
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
//! @param[out]    nIteration    … 実際のループ回数
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution *opdp, size_t nSamples, size_t nComponents, size_t nMaxIteration, double tolerance, size_t &nIteration )
{
	if( rSamples.empty( ) || nComponents == 0 )
	{
		return( false );
	}

	typedef size_t size_type;

	size_type k, m;
	//const double	pi = atan( 1.0f ) * 4.0f;
	const double pi = 3.1415926535897932384626433832795;
	const double _2pi = std::sqrt( 2.0 * pi );
	const double _log_2pi = std::log( _2pi );
	double fLastLikelihood = -1.0e30;

	array2< double > Weight( nSamples, nComponents );
	std::vector< mixture::distribution > pdp( nComponents );


#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
	for( m = 0 ; m < nComponents ; m++ )
	{
		std::cerr << pdp[ m ] << std::endl;
	}
#endif

	// 入力データを作業データにコピーする
	for( m = 0 ; m < nComponents ; m++ )
	{
		pdp[ m ] = opdp[ m ];
	}

	// 初期分布データの重みの和を１に正規化する
	{
		double tmp = 0.0;
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
	}

	// EMアルゴリズムの開始
	for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
	{
		// E-step
		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				double v = pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
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
					double myu = rSamples[ k ] - pdp[ m ].av;
					variance += Weight( k, m ) * myu * myu;
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

		double fLikelihood = 0.0;

		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double myu = rSamples[ k ] - pdp[ m ].av;
				tmp += pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
				//tmp += Weight( k, m ) * pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
			}

			if( tmp == 0.0 )
			{
				return( false );
			}

			fLikelihood += std::log( tmp ) - _log_2pi;
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			std::cerr << pdp[ m ] << std::endl;
		}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
		printf( "%f = ( %f, %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av, pdp[ 0 ].sd );
#endif

		if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
		{
			break;
		}

		// 出力に作業データを反映させる
		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ] = pdp[ m ];
		}

		fLastLikelihood = fLikelihood;
	}

	return( true );
}


/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @attention 入力となるデータの配列として，通常のデータ配列，MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナが利用可能です．
//! @attention 入力データは，MISTで提供する vector2 タイプである必要があります．
//!
//! @param[in]     rSamples      … 入力サンプル
//! @param[in,out] opdp          … 分布パラメータ
//! @param[in]     nSamples      … 入力サンプル数
//! @param[in]     nComponents   … 推定する混合分布の数
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in,out] tolerance     … 対数尤度の打ち切り許容相対誤差
//! @param[out]    nIteration    … 実際のループ回数
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution2 *opdp, size_t nSamples, size_t nComponents, size_t nMaxIteration, double tolerance, size_t &nIteration )
{
	if( rSamples.empty( ) || nComponents == 0 )
	{
		return( false );
	}

	typedef size_t size_type;

	size_type k, m;
	//const double	pi = atan( 1.0f ) * 4.0f;
	const double pi = 3.1415926535897932384626433832795;
	const double _2pi = 2.0 * pi;
	const double _log_2pi = std::log( _2pi );
	double fLastLikelihood = -1.0e30;

	array2< double > Weight( nSamples, nComponents );
	std::vector< mixture::distribution2 > pdp( nComponents );


#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
	for( m = 0 ; m < nComponents ; m++ )
	{
		std::cerr << pdp[ m ] << std::endl;
	}
#endif


	// 入力データを作業データにコピーする
	for( m = 0 ; m < nComponents ; m++ )
	{
		pdp[ m ] = opdp[ m ];
	}


	// 初期分布データの重みの和を１に正規化する
	{
		double tmp = 0.0;
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
	}

	// EMアルゴリズムの開始
	for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
	{
		// E-step

		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
				double a = pdp[ m ].v[ 3 ];
				double b = -pdp[ m ].v[ 1 ];
				double c = -pdp[ m ].v[ 2 ];
				double d = pdp[ m ].v[ 0 ];
				double x = rSamples[ k ].x - pdp[ m ].av.x;
				double y = rSamples[ k ].y - pdp[ m ].av.y;
				double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
				double v = pdp[ m ].weight * ( 1.0 / sqrt( t ) ) * std::exp( - vvv / 2.0 );
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
			vector2< double > average( 0, 0 );
			double	v1 = 0;
			double	v2 = 0;
			double	v3 = 0;

			for( k = 0 ; k < nSamples ; k++ )
			{
				weight_sum += Weight( k, m );
				average += rSamples[ k ] * Weight( k, m );
			}

			if( weight_sum > 0.0 )
			{
				pdp[ m ].weight = weight_sum / static_cast< double >( nSamples );
				pdp[ m ].av = average / weight_sum;

				for( k = 0 ; k < nSamples ; k++ )
				{
					double w  = Weight( k, m );
					double xx = rSamples[ k ].x - pdp[ m ].av.x;
					double yy = rSamples[ k ].y - pdp[ m ].av.y;
					v1 += w * xx * xx;
					v2 += w * yy * yy;
					v3 += w * xx * yy;
				}

				v1 /= weight_sum;
				v2 /= weight_sum;
				v3 /= weight_sum;

				if( v1 * v2 < v3 * v3 )
				{
					v3 = std::sqrt( v1 * v2 ) - 1.0e-10;
				}
			}
			else
			{
				// 重みの合計が１にならないエラー
				return( false );
			}


			pdp[ m ].v[ 0 ] = v1;
			pdp[ m ].v[ 3 ] = v2;
			pdp[ m ].v[ 1 ] = pdp[ m ].v[ 2 ] = v3;
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

		double fLikelihood = 0.0;

		for( k = 0 ; k < nSamples ; k++ )
		{
			double tmp = 0.0;

			for( m = 0 ; m < nComponents ; m++ )
			{
				double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
				double a = pdp[ m ].v[ 3 ];
				double b = -pdp[ m ].v[ 1 ];
				double c = -pdp[ m ].v[ 2 ];
				double d = pdp[ m ].v[ 0 ];
				double x = rSamples[ k ].x - pdp[ m ].av.x;
				double y = rSamples[ k ].y - pdp[ m ].av.y;
				double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
				tmp += Weight( k, m ) * pdp[ m ].weight / sqrt( t ) * std::exp( - vvv / 2.0 );
			}

			if( tmp == 0.0 )
			{
				return( false );
			}

			fLikelihood += std::log( tmp ) - _log_2pi;
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			std::cerr << pdp[ m ] << std::endl;
		}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
		printf( "%f = ( %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av );
#endif

		if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
		{
			break;
		}

		// 出力に作業データを反映させる
		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ] = pdp[ m ];
		}

		fLastLikelihood = fLikelihood;
	}

	return( true );
}


/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
//! @attention 入力データは，MISTで提供する vector2 タイプで分布が mixture::distribution2 の場合は2次元正規分布推定になります．
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in,out] pdp           … 分布パラメータ
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
//! @param[out]    nIteration    … 実際のループ回数
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class Array1, class Array2 >
bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, typename Array1::size_type nMaxIteration, double tolerance, typename Array1::size_type &nIteration )
{
	return( estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), nMaxIteration, tolerance, nIteration ) );
}


/// @brief データ系列から1次元正規分布の混合分布を推定する
//! 
//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
//! @attention 入力データは，MISTで提供する vector2 タイプで分布が mixture::distribution2 の場合は2次元正規分布推定になります．
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in,out] pdp           … 分布パラメータ
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class Array1, class Array2 >
bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, typename Array1::size_type nMaxIteration, double tolerance )
{
	typename Array1::size_type nIteration = 0;
	return( estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), nMaxIteration, tolerance, nIteration ) );
}


/// @brief データ系列から正規分布の混合分布を推定する
//! 
//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in,out] pdp           … 分布パラメータ
//! @param[in]     nComponents   … 推定する混合分布の数
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution *pdp, typename Array::size_type nComponents, typename Array::size_type nMaxIteration, double tolerance )
{
	size_t nIteration = 0;
	return( estimate_mixture( rSamples, pdp, rSamples.size( ), nComponents, nMaxIteration, tolerance, nIteration ) );
}


/// @brief データ系列から2次元正規分布の混合分布を推定する
//! 
//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
//! @attention 入力データは，MISTで提供する vector2 タイプある必要があります．
//! 
//! @param[in]     rSamples      … 入力サンプル
//! @param[in,out] pdp           … 分布パラメータ
//! @param[in]     nComponents   … 推定する混合分布の数
//! @param[in]     nMaxIteration … 最大ループ回数
//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
//!
//! @retval true  … 混合分布の推定に成功
//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
//! 
template < class Array >
bool estimate_mixture( const Array &rSamples, mixture::distribution2 *pdp, typename Array::size_type nComponents, typename Array::size_type nMaxIteration, double tolerance )
{
	size_t nIteration = 0;
	return( estimate_mixture( rSamples, pdp, rSamples.size( ), nComponents, nMaxIteration, tolerance, nIteration ) );
}



/// @brief ヒストグラムから正規分布の混合分布を推定する関数群
namespace histogram
{
	/// @brief 1次元ヒストグラムから正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，通常のデータ配列，MISTで提供するコンテナ，STLで提供されているvector，dequeコンテナが利用可能です．
	//! 
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] opdp          … 分布パラメータ
	//! @param[in]     nSamples      … 入力サンプル数
	//! @param[in]     nComponents   … 推定する混合分布の数
	//! @param[in]     minimum       … ヒストグラムを作成した際の最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
	//! @param[out]    nIteration    … 実際のループ回数
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class Array >
	bool estimate_mixture( const Array &rSamples, mixture::distribution *opdp, size_t nSamples, size_t nComponents, double minimum, double bin, size_t nMaxIteration, double tolerance, size_t &nIteration )
	{
		if( rSamples.empty( ) || nComponents == 0 || bin == 0.0 )
		{
			return( false );
		}

		typedef size_t size_type;

		size_type k, m;

		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = std::sqrt( 2.0 * pi );
		const double _log_2pi = std::log( _2pi );
		double fLastLikelihood = -1.0e30;
		double tmp, number_of_samples;

		array2< double > Weight( nSamples, nComponents );
		std::vector< mixture::distribution > pdp( nComponents );


		// 入力データを作業データにコピーする
		for( m = 0 ; m < nComponents ; m++ )
		{
			pdp[ m ] = opdp[ m ];
		}

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

		// 平均値を0に設定し，重みの和を1にする
		for( m = 0 ; m < nComponents ; m++ )
		{
			pdp[ m ].weight /= tmp;
			pdp[ m ].av -= minimum;
		}

		for( k = 0, number_of_samples = 0.0 ; k < nSamples ; k++ )
		{
			number_of_samples += rSamples[ k ];
		}


#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			std::cerr << pdp[ m ] << std::endl;
		}
#endif


		// EMアルゴリズムの開始
		for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
		{
			// E-step
			for( k = 0 ; k < nSamples ; k++ )
			{
				if( rSamples[ k ] == 0 )
				{
					continue;
				}

				tmp = 0.0;

				for( m = 0 ; m < nComponents ; m++ )
				{
					double myu = ( k + 0.5 ) * bin - pdp[ m ].av;
					double v = pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
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
					if( rSamples[ k ] == 0 )
					{
						continue;
					}
					double w = Weight( k, m ) * rSamples[ k ];
					weight_sum += w;
					average += ( k + 0.5 ) * bin * w;
				}

				if( weight_sum > 0.0 )
				{
					pdp[ m ].weight = weight_sum / number_of_samples;
					pdp[ m ].av = average / weight_sum;

					for( k = 0 ; k < nSamples ; k++ )
					{
						double myu = ( k + 0.5 ) * bin - pdp[ m ].av;
						variance += Weight( k, m ) * rSamples[ k ] * myu * myu;
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

			double fLikelihood = 0.0;

			for( k = 0 ; k < nSamples ; k++ )
			{
				if( rSamples[ k ] == 0 )
				{
					continue;
				}

				tmp = 0.0;

				for( m = 0 ; m < nComponents ; m++ )
				{
					double myu = ( k + 0.5 ) * bin - pdp[ m ].av;
					tmp += Weight( k, m ) * pdp[ m ].weight * std::exp( - myu * myu / ( 2.0 * pdp[ m ].sd * pdp[ m ].sd ) ) / pdp[ m ].sd;
				}

				if( tmp == 0.0 )
				{
					return( false );
				}

				fLikelihood += rSamples[ k ] * ( std::log( tmp ) - _log_2pi );
			}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
			for( m = 0 ; m < nComponents ; m++ )
			{
				std::cerr << pdp[ m ] << std::endl;
			}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
		printf( "%f = ( %f, %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av, pdp[ 0 ].sd );
#endif

			if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
			{
				break;
			}

			// 出力に作業データを反映させる
			for( m = 0 ; m < nComponents ; m++ )
			{
				opdp[ m ] = pdp[ m ];
			}

			fLastLikelihood = fLikelihood;
		}


		// 平均値を元に戻す
		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ].av += minimum;
		}

		return( true );
	}



	/// @brief 2次元ヒストグラムから正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，MISTで提供する2次元コンテナが利用可能です．
	//!
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] opdp          … 分布パラメータ
	//! @param[in]     nComponents   … 推定する混合分布の数
	//! @param[in]     minimum1      … ヒストグラムを作成した際の第1軸方向での最小値
	//! @param[in]     minimum2      … ヒストグラムを作成した際の第2軸方向での最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance      … 対数尤度の打ち切り許容相対誤差
	//! @param[out]    nIteration    … 実際のループ回数
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class T, class Allocator >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, mixture::distribution2 *opdp, size_t nComponents, double minimum1, double minimum2, double bin, size_t nMaxIteration, double tolerance, size_t &nIteration )
	{
		if( rSamples.empty( ) || nComponents == 0 )
		{
			return( false );
		}

		typedef size_t size_type;

		size_type i, j, k, m;

		const double pi = 3.1415926535897932384626433832795;
		const double _2pi = 2.0 * pi;
		const double _log_2pi = std::log( _2pi );
		double fLastLikelihood = -1.0e30;
		double number_of_samples;

		array3< double > Weight( rSamples.width( ), rSamples.height( ), nComponents );
		std::vector< mixture::distribution2 > pdp( nComponents );


		// 入力データを作業データにコピーする
		for( m = 0 ; m < nComponents ; m++ )
		{
			pdp[ m ] = opdp[ m ];
		}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
		for( m = 0 ; m < nComponents ; m++ )
		{
			mixture::distribution2 tmp = pdp[ m ];
			tmp.av.x += minimum1;
			tmp.av.y += minimum2;
			std::cerr << tmp << std::endl;
		}
#endif

		// 初期分布データの重みの和を１に正規化する
		{
			double tmp = 0.0;
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
				pdp[ m ].av.x   -= minimum1;
				pdp[ m ].av.y   -= minimum2;
			}

			for( k = 0, number_of_samples = 0.0 ; k < rSamples.size( ) ; k++ )
			{
				number_of_samples += rSamples[ k ];
			}
		}


		// EMアルゴリズムの開始
		for( nIteration = 0 ; nIteration < nMaxIteration ; nIteration++ )
		{
			// E-step

			for( j = 0 ; j < rSamples.height( ) ; j++ )
			{
				for( i = 0 ; i < rSamples.width( ) ; i++ )
				{
					if( rSamples( i, j ) == 0 )
					{
						continue;
					}

					double tmp = 0.0;

					for( m = 0 ; m < nComponents ; m++ )
					{
						double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
						double a = pdp[ m ].v[ 3 ];
						double b = -pdp[ m ].v[ 1 ];
						double c = -pdp[ m ].v[ 2 ];
						double d = pdp[ m ].v[ 0 ];
						double x = pdp[ m ].av.x - ( i + 0.5 ) * bin;
						double y = pdp[ m ].av.y - ( j + 0.5 ) * bin;
						double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
						double v = pdp[ m ].weight * std::exp( - vvv / 2.0 ) / sqrt( t );
						Weight( i, j, m ) = v;
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
							Weight( i, j, m ) /= tmp;
						}
					}
				}
			}

			// M-step
			for( m = 0 ; m < nComponents ; m++ )
			{
				double	weight_sum = 0;
				double	avex = 0;
				double	avey = 0;
				double	v1 = 0;
				double	v2 = 0;
				double	v3 = 0;

				for( j = 0 ; j < rSamples.height( ) ; j++ )
				{
					for( i = 0 ; i < rSamples.width( ) ; i++ )
					{
						double w = Weight( i, j, m ) * rSamples( i, j );
						weight_sum += w;
						avex += static_cast< double >( ( i + 0.5 ) * bin ) * w;
						avey += static_cast< double >( ( j + 0.5 ) * bin ) * w;
					}
				}

				if( weight_sum > 0.0 )
				{
					pdp[ m ].weight = weight_sum / number_of_samples;
					pdp[ m ].av.x = avex / weight_sum;
					pdp[ m ].av.y = avey / weight_sum;

					for( j = 0 ; j < rSamples.height( ) ; j++ )
					{
						for( i = 0 ; i < rSamples.width( ) ; i++ )
						{
							double w   = Weight( i, j, m );
							double xx  = ( i + 0.5 ) * bin - pdp[ m ].av.x;
							double yy  = ( j + 0.5 ) * bin - pdp[ m ].av.y;
							double num = rSamples( i, j );
							v1 += w * xx * xx * num;
							v2 += w * yy * yy * num;
							v3 += w * xx * yy * num;
						}
					}

					v1 /= weight_sum;
					v2 /= weight_sum;
					v3 /= weight_sum;

					if( v1 * v2 < v3 * v3 )
					{
						v3 = std::sqrt( v1 * v2 ) - 1.0e-10;
					}
				}
				else
				{
					// 重みの合計が１にならないエラー
					return( false );
				}


				pdp[ m ].v[ 0 ] = v1;
				pdp[ m ].v[ 3 ] = v2;
				pdp[ m ].v[ 1 ] = pdp[ m ].v[ 2 ] = v3;
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

			double fLikelihood = 0.0;

			for( j = 0 ; j < rSamples.height( ) ; j++ )
			{
				for( i = 0 ; i < rSamples.width( ) ; i++ )
				{
					if( rSamples( i, j ) == 0 )
					{
						continue;
					}

					double tmp = 0.0;

					for( m = 0 ; m < nComponents ; m++ )
					{
						double t = pdp[ m ].v[ 0 ] * pdp[ m ].v[ 3 ] - pdp[ m ].v[ 1 ] * pdp[ m ].v[ 2 ];
						double a = pdp[ m ].v[ 3 ];
						double b = -pdp[ m ].v[ 1 ];
						double c = -pdp[ m ].v[ 2 ];
						double d = pdp[ m ].v[ 0 ];
						double x = pdp[ m ].av.x - ( i + 0.5 ) * bin;
						double y = pdp[ m ].av.y - ( j + 0.5 ) * bin;
						double vvv = ( ( a * x + b * y ) * x + ( c * x + d * y ) * y ) / t;
						tmp += Weight( i, j, m ) * pdp[ m ].weight * std::exp( - vvv / 2.0 ) / std::sqrt( t );
					}

					if( tmp == 0.0 )
					{
						return( false );
					}

					fLikelihood += rSamples( i, j ) * ( std::log( tmp ) - _log_2pi );
				}
			}

#if defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 1
			for( m = 0 ; m < nComponents ; m++ )
			{
				mixture::distribution2 tmp = pdp[ m ];
				tmp.av.x += minimum1;
				tmp.av.y += minimum2;
				std::cerr << tmp << std::endl;
			}
#elif defined( EMALGORITHM_DEBUG ) && EMALGORITHM_DEBUG == 2
			printf( "%f = ( %f, %f )\n", fLikelihood, pdp[ 0 ].weight, pdp[ 0 ].av );
#endif

			if( fLastLikelihood >= fLikelihood || 2.0 * std::abs( fLastLikelihood - fLikelihood ) < tolerance * ( std::abs( fLastLikelihood ) + std::abs( fLikelihood ) ) )
			{
				break;
			}

			// 出力に作業データを反映させる
			for( m = 0 ; m < nComponents ; m++ )
			{
				opdp[ m ] = pdp[ m ];
			}

			fLastLikelihood = fLikelihood;
		}

		for( m = 0 ; m < nComponents ; m++ )
		{
			opdp[ m ].av.x += minimum1;
			opdp[ m ].av.y += minimum2;
		}

		return( true );
	}


	/// @brief データ系列から正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
	//! 
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] pdp           … 分布パラメータ
	//! @param[in]     minimum       … ヒストグラムを作成した際の最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance      … 対数尤度の打ち切り許容相対誤差
	//! @param[out]    nIteration    … 実際のループ回数
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class Array1, class Array2 >
	bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, double minimum, double bin, typename Array1::size_type nMaxIteration, double tolerance, typename Array1::size_type &nIteration )
	{
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), minimum, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief データ系列から1次元正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
	//! 
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] pdp           … 分布パラメータ
	//! @param[in]     minimum       … ヒストグラムを作成した際の最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class Array1, class Array2 >
	bool estimate_mixture( const Array1 &rSamples, Array2 &pdp, double minimum, double bin, typename Array1::size_type nMaxIteration, double tolerance )
	{
		typename Array1::size_type nIteration = 0;
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], rSamples.size( ), pdp.size( ), minimum, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief データ系列から正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
	//! 
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] pdp           … 分布パラメータ
	//! @param[in]     nComponents   … 推定する混合分布の数
	//! @param[in]     minimum       … ヒストグラムを作成した際の最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class Array >
	bool estimate_mixture( const Array &rSamples, mixture::distribution *pdp, typename Array::size_type nComponents, double minimum, double bin, typename Array::size_type nMaxIteration, double tolerance )
	{
		size_t nIteration = 0;
		return( histogram::estimate_mixture( rSamples, pdp, rSamples.size( ), nComponents, minimum, bin, nMaxIteration, tolerance, nIteration ) );
	}






	/// @brief データ系列から正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
	//! @attention 入力データが array2 で分布が mixture::distribution2 の場合は2次元正規分布推定になります．
	//! @attention 正しい平均値を求めるためには，EMアルゴリズムの推定結果の平均値に，入力されたヒストグラムのオフセットを足してください．
	//! 
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] pdp           … 分布パラメータ
	//! @param[in]     minimum1      … ヒストグラムを作成した際の第1軸方向での最小値
	//! @param[in]     minimum2      … ヒストグラムを作成した際の第2軸方向での最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
	//! @param[out]    nIteration    … 実際のループ回数
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class T, class Allocator, class Array1 >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, Array1 &pdp, double minimum1, double minimum2, double bin, typename Array1::size_type nMaxIteration, double tolerance, typename Array1::size_type &nIteration )
	{
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), minimum1, minimum2, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief データ系列から1次元正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
	//! @attention 入力データが array2 で分布が mixture::distribution2 の場合は2次元正規分布推定になります．
	//! @attention 正しい平均値を求めるためには，EMアルゴリズムの推定結果の平均値に，入力されたヒストグラムのオフセットを足してください．
	//! 
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] pdp           … 分布パラメータ
	//! @param[in]     minimum1      … ヒストグラムを作成した際の第1軸方向での最小値
	//! @param[in]     minimum2      … ヒストグラムを作成した際の第2軸方向での最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class T, class Allocator, class Array1 >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, Array1 &pdp, double minimum1, double minimum2, double bin, typename Array1::size_type nMaxIteration, double tolerance )
	{
		typename array< T, Allocator >::size_type nIteration = 0;
		return( histogram::estimate_mixture( rSamples, &pdp[ 0 ], pdp.size( ), minimum1, minimum2, bin, nMaxIteration, tolerance, nIteration ) );
	}


	/// @brief データ系列から2次元正規分布の混合分布を推定する
	//! 
	//! @attention 入力となるデータの配列として，MISTで提供するコンテナもしくはSTLで提供されているvector，dequeコンテナが利用可能です．
	//! @attention 入力データは，MISTで提供する vector2 タイプある必要があります．
	//! @attention 正しい平均値を求めるためには，EMアルゴリズムの推定結果の平均値に，入力されたヒストグラムのオフセットを足してください．
	//! 
	//! @param[in]     rSamples      … 入力サンプル
	//! @param[in,out] pdp           … 分布パラメータ
	//! @param[in]     nComponents   … 推定する混合分布の数
	//! @param[in]     minimum1      … ヒストグラムを作成した際の第1軸方向での最小値
	//! @param[in]     minimum2      … ヒストグラムを作成した際の第2軸方向での最小値
	//! @param[in]     bin           … ヒストグラムを作成した際のビン幅
	//! @param[in]     nMaxIteration … 最大ループ回数
	//! @param[in]     tolerance     … 対数尤度の打ち切り許容相対誤差
	//!
	//! @retval true  … 混合分布の推定に成功
	//! @retval false … 混合分布の推定に失敗，もしくは入力データが空
	//! 
	template < class T, class Allocator >
	bool estimate_mixture( const array2< T, Allocator > &rSamples, mixture::distribution2 *pdp, typename array2< T, Allocator >::size_type nComponents, double minimum1, double minimum2, double bin, typename array2< T, Allocator >::size_type nMaxIteration, double tolerance )
	{
		size_t nIteration = 0;
		return( histogram::estimate_mixture( rSamples, pdp, nComponents, minimum1, minimum2, bin, nMaxIteration, tolerance, nIteration ) );
	}
}

/// @}
//  EMアルゴリズムを用いた混合分布の推定グループの終わり



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIXTURE__
