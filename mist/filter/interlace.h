// 
// Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/filter/interlace.h
//!
//! @brief インターレスの解除を行うためのライブラリ
//!

#ifndef __INCLUDE_MIST_INTERLACE__
#define __INCLUDE_MIST_INTERLACE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif



// mist名前空間の始まり
_MIST_BEGIN


#include <math.h>

namespace __donotuse__
{
	class WazaFIR{
	public:
		enum FILTER{
			FIR_LPF,			// 低域通過
			FIR_HPF,			// 高域通過
			FIR_BPF,			// 帯域通過
			FIR_BRF,			// 帯域除去
		}Filter;
	private:
		FILTER	mode;			// フィルタ種別
		int		nOrder;			// フィルタ次数
		double	f_smp;			// サンプリング周波数
		double	f_lc;			// 低域遮断周波数
		double	f_hc;			// 高域遮断周波数
		double	attenuation;	// 減衰量（dB）
		double	alpha;
		double*	pKaiserWnd;
		double*	pCoefs;
	public:
		WazaFIR();
		virtual ~WazaFIR();
		virtual void SetMode(FILTER mode);
		virtual int SetParameter(double fsmp, double f1, double f2, double att);
		virtual void SetFilterLength(int length);
		virtual void Compute();
		virtual void GetCoefs(double* pCoef);
		virtual double GetAmpSpec(double f);
	private:
		double ModZeroBessel(double x);
	};

	WazaFIR::WazaFIR()
	{
		pKaiserWnd = NULL;
		pCoefs = NULL;
	}

	WazaFIR::~WazaFIR()
	{
		delete [] pKaiserWnd;
		delete [] pCoefs;
	}

	void WazaFIR::SetMode(FILTER filter_mode)
	{
		mode = filter_mode;
	}

	int WazaFIR::SetParameter(double fsmp, double f1, double f2, double att)
	{
		// ナイキスト条件
		if(f1 >= fsmp / 2 || f2 >= fsmp / 2)
			return 0;

		// 減衰量
		if(att < 0)
			return 0;

		const double	pi = 4.0 * atan(1.0);

		f_smp = fsmp;
		f_lc = f1;
		f_hc = f2;
		attenuation = att;

		// 式の値より大きい偶数にする．これで本当にいいのか？
		nOrder = (int)ceil((attenuation - 7.95) / (2.285 * (2.0 * pi * (f_hc - f_lc) / f_smp)));
		if(nOrder % 2 != 0)
			nOrder ++;

		// 実際にはN+1個の係数が必要だ．
		nOrder ++;

		if(att >= 50)
			alpha = 0.1102 * (att - 8.7);
		else if(att > 21)
			alpha = 0.5842 * std::pow(att - 21, 0.4) + 0.07886 * (att - 21);
		else
			alpha = 0;

		return nOrder;
	}

	void WazaFIR::SetFilterLength(int length)
	{
		nOrder = length;
	}

	#include <stdio.h>

	void WazaFIR::Compute()
	{
		int	n;
		int	n_fix;

		if(pKaiserWnd){
			delete [] pKaiserWnd;
			pKaiserWnd = NULL;
		}
		if(pCoefs){
			delete [] pCoefs;
			pCoefs = NULL;
		}

		pKaiserWnd = new double[nOrder];
		pCoefs = new double[nOrder];

		// カイザー窓の計算だよ。
		for(n = 0; n < nOrder; n ++){
			n_fix = n - (nOrder / 2);
			double	root_val = 1 - std::pow((n_fix / (double)(nOrder / 2)), 2);

			pKaiserWnd[n] = ModZeroBessel(alpha * std::sqrt(root_val)) / ModZeroBessel(alpha);
		}

		const double	pi = 4.0 * atan(1.0);
		double	w_smp = 2.0 * pi * f_smp;
		double	wl, wh, wc, w0 = 1;

		// 周波数変換
		switch(mode){
		case WazaFIR::FIR_LPF:
			wl = f_lc * 2.0 * pi;
			wh = f_hc * 2.0 * pi;
			wc = (wl + wh) / 2.0;
			break;

		case WazaFIR::FIR_HPF:
			f_lc = f_smp / 2.0 - f_lc;
			f_hc = f_smp / 2.0 - f_hc;
			wl = f_lc * 2.0 * pi;
			wh = f_hc * 2.0 * pi;
			wc = (wl + wh) / 2.0;
			break;

		case WazaFIR::FIR_BPF:
		case WazaFIR::FIR_BRF:
		default:
			wl = f_lc * 2.0 * pi;
			wh = f_hc * 2.0 * pi;
			wc = (wh - wl) / 2.0;
			w0 = (wh + wl) / 2.0;
			break;
		}

		// フィルタ係数を計算するよ。
		for(n = 0; n < nOrder; n ++){
			n_fix = n - (nOrder / 2);

			if(n_fix == 0)
				pCoefs[n] = 2.0 * wc / w_smp;
			else
				pCoefs[n] = std::sin((2.0 * n_fix * pi * wc) / w_smp) / (n_fix * pi);

			pCoefs[n] *= pKaiserWnd[n];
		}

		double	gain = 0;
		for(n = 0; n < nOrder; n ++){
			gain += pCoefs[n];
		}
		for(n = 0; n < nOrder; n ++){
			pCoefs[n] /= gain;
		}

	/*
		for(n = 0; n < nOrder; n ++){
			n_fix = n - (nOrder / 2);

			if(n_fix == 0)
				pCoefs[n] = 2.0 * ((f_hc + f_lc) / 2.0) / f_smp;
			else
				pCoefs[n] = std::sin((2.0 * n_fix * pi * ((f_hc + f_lc) / 2.0)) / f_smp) / (n_fix * pi);

			pCoefs[n] *= pKaiserWnd[n];
		}
	*/

		// 係数変換
		switch(mode){
		case WazaFIR::FIR_LPF:
			break;

		case WazaFIR::FIR_HPF:
			for(n = 0; n < nOrder; n ++){
				n_fix = n - (nOrder / 2);
				if(n_fix % 2 != 0)
					pCoefs[n] *= - 1;
			}
			break;

		case WazaFIR::FIR_BPF:
			for(n = 0; n < nOrder; n ++){
				n_fix = n - (nOrder / 2);
				pCoefs[n] *= 2.0 * std::cos(n_fix * w0 / f_smp);
			}
			break;

		default:
		case WazaFIR::FIR_BRF:
			for(n = 0; n < nOrder; n ++){
				n_fix = n - (nOrder / 2);
				if(n_fix == 0)
					pCoefs[n] = 1.0 - 2.0 * pCoefs[n] * std::cos(n_fix * w0 / f_smp);
				else
					pCoefs[n] *= - 2.0 * std::cos(n_fix * w0 / f_smp);
			}
			break;
		}
	}

	void WazaFIR::GetCoefs(double* pCoef)
	{
		memmove(pCoef, pCoefs, sizeof(double) * nOrder);
	}

	double WazaFIR::GetAmpSpec(double f)
	{
		// 振幅特性の計算だよ。
		double	gain;
		double	ws, t;
		const double	pi = 4 * atan(1.0);
		double	w_smp = 2.0 * pi * f_smp;

		ws = 2.0 * pi * f;
		t = 2.0 * pi / w_smp;
		gain = 0;

		for(int n = 0; n < nOrder; n ++){
			int	n_fix = n - (nOrder / 2);
			gain += pCoefs[n] * (std::cos(n_fix * ws * t) - std::sin(n_fix * ws * t));
		}

		return gain;
	}

	double WazaFIR::ModZeroBessel(double x)
	{
		double	ret = 1;
		double	val = 1;

		for(int n = 1; n < 20; n ++){
			val = val * (x / 2) / n;
			ret += val * val;
		}

		return ret;
	}
}



namespace __interlace_controller__
{
	const double	a[145] = {
-3.973665173664618e-19,
-1.126871940717711e-04,
1.081392878093926e-18,
1.661200995494522e-04,
5.806505953909648e-19,
-2.319386485663537e-04,
1.318909591445624e-19,
3.117767329350236e-04,
-1.400928524565706e-18,
-4.073818425943347e-04,
3.392851055584217e-18,
5.206210760868185e-04,
-6.300164192272503e-18,
-6.534895636323179e-04,
-9.865508702817430e-22,
8.081221524809110e-04,
-3.066494710188480e-18,
-9.868094319691043e-04,
7.448606926165091e-18,
1.192019475531332e-03,
5.119051506283555e-18,
-1.426427077496941e-03,
-7.640491918230903e-19,
1.692952805473677e-03,
-5.405664133565353e-18,
-1.994814935723270e-03,
1.375834792294123e-17,
2.335598380452481e-03,
-2.471280887291696e-17,
-2.719346190443236e-03,
-2.873221535028269e-18,
3.150681336158443e-03,
-8.292633577319232e-18,
-3.634969567394209e-03,
2.292346946959600e-17,
4.178538755162610e-03,
-9.863907759242679e-18,
-4.788977092057936e-03,
-7.533988647949567e-18,
5.475543305999689e-03,
-1.146400231917755e-17,
-6.249739083922753e-03,
3.597087815541321e-17,
7.126121527494693e-03,
-1.304403538975571e-17,
-8.123479513614832e-03,
-1.700922532064955e-17,
9.266577049844868e-03,
-1.455240404058461e-17,
-1.058880794101096e-02,
5.551879657881333e-17,
1.213636837513455e-02,
-1.593724232843570e-17,
-1.397506483336812e-02,
1.656769115855287e-17,
1.620192638890920e-02,
-1.714899543728943e-17,
-1.896610413615148e-02,
1.767573818683316e-17,
2.250905774673122e-02,
-1.814295409930719e-17,
-2.724857467241255e-02,
1.854619559692718e-17,
3.397480412206919e-02,
-1.888159240832208e-17,
-4.438233301160727e-02,
1.914590370575518e-17,
6.287921847364857e-02,
-1.933656196394635e-17,
-1.056321033172101e-01,
1.945170783219903e-17,
3.181525653197714e-01,
5.000000000000000e-01,
3.181525653197714e-01,
1.945170783219903e-17,
-1.056321033172101e-01,
-1.933656196394635e-17,
6.287921847364857e-02,
1.914590370575518e-17,
-4.438233301160727e-02,
-1.888159240832208e-17,
3.397480412206919e-02,
1.854619559692718e-17,
-2.724857467241255e-02,
-1.814295409930719e-17,
2.250905774673122e-02,
1.767573818683316e-17,
-1.896610413615148e-02,
-1.714899543728943e-17,
1.620192638890920e-02,
1.656769115855287e-17,
-1.397506483336812e-02,
-1.593724232843570e-17,
1.213636837513455e-02,
5.551879657881333e-17,
-1.058880794101096e-02,
-1.455240404058461e-17,
9.266577049844868e-03,
-1.700922532064955e-17,
-8.123479513614832e-03,
-1.304403538975571e-17,
7.126121527494693e-03,
3.597087815541321e-17,
-6.249739083922753e-03,
-1.146400231917755e-17,
5.475543305999689e-03,
-7.533988647949567e-18,
-4.788977092057936e-03,
-9.863907759242679e-18,
4.178538755162610e-03,
2.292346946959600e-17,
-3.634969567394209e-03,
-8.292633577319232e-18,
3.150681336158443e-03,
-2.873221535028269e-18,
-2.719346190443236e-03,
-2.471280887291696e-17,
2.335598380452481e-03,
1.375834792294123e-17,
-1.994814935723270e-03,
-5.405664133565353e-18,
1.692952805473677e-03,
-7.640491918230903e-19,
-1.426427077496941e-03,
5.119051506283555e-18,
1.192019475531332e-03,
7.448606926165091e-18,
-9.868094319691043e-04,
-3.066494710188480e-18,
8.081221524809110e-04,
-9.865508702817430e-22,
-6.534895636323179e-04,
-6.300164192272503e-18,
5.206210760868185e-04,
3.392851055584217e-18,
-4.073818425943347e-04,
-1.400928524565706e-18,
3.117767329350236e-04,
1.318909591445624e-19,
-2.319386485663537e-04,
5.806505953909648e-19,
1.661200995494522e-04,
1.081392878093926e-18,
-1.126871940717711e-04,
-3.973665173664618e-19
	};
	template < bool b >
	struct interlace_controller
	{
		template < class T, class Allocator >
		static bool interlace_odd( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行異常ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;

			// 奇数ラインを補間するため，先頭行を次の行で補完する
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				out( i, 0 ) = in( i, 1 );
			}
			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 2 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = static_cast< value_type >( ( in( i, j - 1 ) + in( i, j + 1 ) ) / 2.0 );
				}
			}
			// 最終行が奇数のため，1行前で補完する
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行異常ない場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;

			// 各偶数行を前後の奇数行で線形に補完してうめる
			for( j = 1 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = static_cast< value_type >( ( in( i, j - 1 ) + in( i, j + 1 ) ) / 2.0 );
				}
			}
			// 最終行が偶数のため，1行前で補完する
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}
	};

	template < >
	struct interlace_controller< true >
	{
		template < class T, class Allocator >
		static bool interlace_odd( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上無い場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			// 奇数ラインを補間するため，先頭行を次の行で補完する
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				out( i, 0 ) = in( i, 1 );
			}
			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 2 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					r = ( in( i, j - 1 ).r + in( i, j + 1 ).r ) / 2.0;
					g = ( in( i, j - 1 ).g + in( i, j + 1 ).g ) / 2.0;
					b = ( in( i, j - 1 ).b + in( i, j + 1 ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}
			}
			// 最終行が奇数のため，1行前で補完する
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上無い場合はインターレス除去はできない
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			for( j = 1 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					r = ( in( i, j - 1 ).r + in( i, j + 1 ).r ) / 2.0;
					g = ( in( i, j - 1 ).g + in( i, j + 1 ).g ) / 2.0;
					b = ( in( i, j - 1 ).b + in( i, j + 1 ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}
			}
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool deinterlace_bob1_odd( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上無い場合はインターレス除去はできない
				return false;
			}

			//out = in;

			size_type i, j;
			double r, g, b;

			// 縦1ラインバッファ
			double*	rbuf;
			double*	gbuf;
			double*	bbuf;

			// LPFのセットアップ
			__donotuse__::WazaFIR	fir;
			double*	pCoef;
			int	nOrder;

			fir.SetMode(__donotuse__::WazaFIR::FIR_LPF);
			//nOrder = fir.SetParameter(40000.0, 10000.0 * 0.8, 10000.0, 60.0);
			//nOrder = fir.SetParameter(400.0, 100.0 * 0.9, 100.0 * 1.0, 60.0);
			nOrder = fir.SetParameter(400.0, 100.0 * 0.95, 100.0 * 1.0, 60.0);

			fprintf(stderr, "FIR filter length; %d\n", nOrder);

			fir.Compute();

//nOrder = 145;
			pCoef = new double[nOrder];
			fir.GetCoefs(pCoef);
//for(int n = 0; n < nOrder; n ++)
//	pCoef[n] = a[n];
//			for(int n = 0; n < nOrder; n ++){
//				printf("%d,%f\n",n,pCoef[n]);
//			}

#if	0
			// f特の計算・出力
			const int	smppoint = 1000;
			double	fspec[smppoint];
			double	fr;
			double	max_amp = 0;
			for(int n = 0; n < smppoint; n ++){
				fr = n * (200 / (double)smppoint);
				fspec[n] = fir.GetAmpSpec(fr);
				if(fspec[n] > max_amp)
					max_amp = fspec[n];
			}
			for(int n = 0; n < smppoint; n ++){
				fr = n * (200 / (double)smppoint);
				printf("%f\t\t%f\n", fr, 20.0 * log10(fabs(fspec[n]) / max_amp));
			}
#endif

			rbuf = new double[in.height() + nOrder - 1];
			gbuf = new double[in.height() + nOrder - 1];
			bbuf = new double[in.height() + nOrder - 1];
/*
			// 奇数ラインを補間する（つまり消し去るのだ！）
			for( j = 0 ; j < in.height( ) ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i ++ )
				{
					out( i, j ) = 0;
				}
			}
*/
//			out.resize(out.width(), out.height() + nOrder - 1);

			double	min_ = 10000;
			double	max_ = -10000;
			for( i = 0 ; i < in.width( ) ; i ++ )
			{
				const int delta = 1;

				memset(rbuf, 0, sizeof(double) * (in.height() + nOrder - 1));
				memset(gbuf, 0, sizeof(double) * (in.height() + nOrder - 1));
				memset(bbuf, 0, sizeof(double) * (in.height() + nOrder - 1));

				for(j = 0; j < nOrder / 2 - 1 + delta; j ++)
				{
					rbuf[j] = in(i, 1).r - 127.5;
					gbuf[j] = in(i, 1).g - 127.5;
					bbuf[j] = in(i, 1).b - 127.5;
				}

				int	line = in.height() - 1;
				if(line % 2 == 0)	line --;
				for(j = nOrder / 2 - 1 + in.height() + delta; j < in.height() + nOrder - 1; j ++)
				{
					rbuf[j] = in(i, line).r - 127.5;
					gbuf[j] = in(i, line).g - 127.5;
					bbuf[j] = in(i, line).b - 127.5;
				}


				for(j = 0; j < in.height(); j ++)
				{
					rbuf[nOrder / 2 - 1 + j + delta] = in(i, j).r - 127.5;
					gbuf[nOrder / 2 - 1 + j + delta] = in(i, j).g - 127.5;
					bbuf[nOrder / 2 - 1 + j + delta] = in(i, j).b - 127.5;
				}

				// これじゃ画像の先頭ラインが偶数なのか奇数なのかわかんないじゃないｗ
				if((nOrder / 2 - 1 + delta) % 2 == 0)	line = 0;
				else	line = 1;
				for(j = line; j < in.height() + nOrder - 1; j += 2)
				{
					rbuf[j] = 0;
					gbuf[j] = 0;
					bbuf[j] = 0;
				}
#if	1
				for(j = 0; j < in.height(); j ++)
				{
					//if(j % 2 == 0){
						double	rsum = 0, gsum = 0, bsum = 0;
						for(int n = 0; n < nOrder; n ++)
						{
							rsum += pCoef[n] * rbuf[j + n];
							gsum += pCoef[n] * gbuf[j + n];
							bsum += pCoef[n] * bbuf[j + n];
						}
						// -64 - 64
						// 64 - 192
						rsum *= 2;
						gsum *= 2;
						bsum *= 2;
						rsum += 127.5;
						gsum += 127.5;
						bsum += 127.5;
						rsum = floor(rsum + 0.5);
						gsum = floor(gsum + 0.5);
						bsum = floor(bsum + 0.5);
						//rsum *= 1.335;//(2.0 - 0.5);
						//gsum *= 1.335;//(2.0 - 0.5);
						//bsum *= 1.335;//(2.0 - 0.5);

						if(rsum < min_)			min_ = rsum;
						else if(rsum > max_)	max_ = rsum;

						if(rsum < 0){
							static int	under_count = 0;
							//fprintf(stderr, "R underflow[%5d] : %f\n", under_count ++, rsum);
						}else if(rsum > 256){
							static int	over_count = 0;
							//fprintf(stderr, "R overflow[%5d]  : %f\n", over_count ++, rsum);
						}
						if(rsum < 0)	rsum = 0;
						else if(rsum > 255)	rsum = 255;
						if(gsum < 0)	gsum = 0;
						else if(gsum > 255)	gsum = 255;
						if(bsum < 0)	bsum = 0;
						else if(bsum > 255)	bsum = 255;

						out(i, j).r = rsum;
						out(i, j).g = gsum;
						out(i, j).b = bsum;
						//out( i, j ) = color_type( static_cast< value_type >( rsum ), static_cast< value_type >( gsum ), static_cast< value_type >( bsum ) );
					//}else{
					//	out( i, j ) = in( i, j );
					//}
				}
#else
				for(j = 0; j < out.height(); j ++)
				{
					out( i, j ) = color_type( static_cast< value_type >( rbuf[j]+127.5 ), static_cast< value_type >( (int)gbuf[j]+127.5 ), static_cast< value_type >( (int)bbuf[j]+127.5 ) );
					//out( i, j ) = color_type( static_cast< value_type >( (int)rbuf[j] ), static_cast< value_type >( (int)gbuf[j] ), static_cast< value_type >( (int)bbuf[j] ) );
				}
#endif
			}
			fprintf(stderr, "min %f, max %f\n", min_, max_);

			delete [] pCoef;

			delete [] rbuf;
			delete [] gbuf;
			delete [] bbuf;

#if	0
			// 奇数ラインを補間するため，先頭行を次の行で補完する
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				out( i, 0 ) = in( i, 1 );
			}
			// 各奇数行を前後の偶数行で線形に補完してうめる
			for( j = 2 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					r = ( in( i, j - 1 ).r + in( i, j + 1 ).r ) / 2.0;
					g = ( in( i, j - 1 ).g + in( i, j + 1 ).g ) / 2.0;
					b = ( in( i, j - 1 ).b + in( i, j + 1 ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}
			}
			// 最終行が奇数のため，1行前で補完する
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
#endif
			return true;
		}

		template < class T, class Allocator >
		static bool deinterlace_bob1_even( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2行以上無い場合はインターレス除去はできない
				return false;
			}

			out = in;

			size_type i, j;
			double r, g, b;

			// 縦1ラインバッファ
			double*	rbuf;
			double*	gbuf;
			double*	bbuf;

			// LPFのセットアップ
			__donotuse__::WazaFIR	fir;
			double*	pCoef;
			int	nOrder;

			fir.SetMode(__donotuse__::WazaFIR::FIR_LPF);
			//nOrder = fir.SetParameter(40000.0, 10000.0 * 0.8, 10000.0, 60.0);
			//nOrder = fir.SetParameter(400.0, 100.0 * 0.9, 100.0 * 1.0, 60.0);
			nOrder = fir.SetParameter(400.0, 100.0 * 0.95, 100.0 * 1.0, 60.0);

			fprintf(stderr, "FIR filter length; %d\n", nOrder);

			fir.Compute();

//nOrder = 145;
			pCoef = new double[nOrder];
			fir.GetCoefs(pCoef);
//for(int n = 0; n < nOrder; n ++)
//	pCoef[n] = a[n];
//			for(int n = 0; n < nOrder; n ++){
//				printf("%d,%f\n",n,pCoef[n]);
//			}

#if	0
			// f特の計算・出力
			const int	smppoint = 1000;
			double	fspec[smppoint];
			double	fr;
			double	max_amp = 0;
			for(int n = 0; n < smppoint; n ++){
				fr = n * (200 / (double)smppoint);
				fspec[n] = fir.GetAmpSpec(fr);
				if(fspec[n] > max_amp)
					max_amp = fspec[n];
			}
			for(int n = 0; n < smppoint; n ++){
				fr = n * (200 / (double)smppoint);
				printf("%f\t\t%f\n", fr, 20.0 * log10(fabs(fspec[n]) / max_amp));
			}
#endif

			rbuf = new double[in.height() + nOrder - 1];
			gbuf = new double[in.height() + nOrder - 1];
			bbuf = new double[in.height() + nOrder - 1];

//			out.resize(out.width(), out.height() + nOrder - 1);

			double	min_ = 10000;
			double	max_ = -10000;
			for( i = 0 ; i < in.width( ) ; i ++ )
			{
				const int delta = 1;

				memset(rbuf, 0, sizeof(double) * (in.height() + nOrder - 1));
				memset(gbuf, 0, sizeof(double) * (in.height() + nOrder - 1));
				memset(bbuf, 0, sizeof(double) * (in.height() + nOrder - 1));

				for(j = 0; j < nOrder / 2 - 1 + delta; j ++)
				{
					rbuf[j] = in(i, 0).r - 127.5;
					gbuf[j] = in(i, 0).g - 127.5;
					bbuf[j] = in(i, 0).b - 127.5;
				}

				int	line = in.height() - 1;
				if(line % 2 == 1)	line --;
				for(j = nOrder / 2 - 1 + in.height() + delta; j < in.height() + nOrder - 1; j ++)
				{
					rbuf[j] = in(i, line).r - 127.5;
					gbuf[j] = in(i, line).g - 127.5;
					bbuf[j] = in(i, line).b - 127.5;
				}


				for(j = 0; j < in.height(); j ++)
				{
					rbuf[nOrder / 2 - 1 + j + delta] = in(i, j).r - 127.5;
					gbuf[nOrder / 2 - 1 + j + delta] = in(i, j).g - 127.5;
					bbuf[nOrder / 2 - 1 + j + delta] = in(i, j).b - 127.5;
				}

				// これじゃ画像の先頭ラインが偶数なのか奇数なのかわかんないじゃないｗ
				if((nOrder / 2 - 1 + delta) % 2 == 1)	line = 0;
				else	line = 1;
				for(j = line; j < in.height() + nOrder - 1; j += 2)
				{
					rbuf[j] = 0;
					gbuf[j] = 0;
					bbuf[j] = 0;
				}
#if	1
				for(j = 0; j < in.height(); j ++)
				{
					//if(j % 2 == 0){
						double	rsum = 0, gsum = 0, bsum = 0;
						for(int n = 0; n < nOrder; n ++)
						{
							rsum += pCoef[n] * rbuf[j + n];
							gsum += pCoef[n] * gbuf[j + n];
							bsum += pCoef[n] * bbuf[j + n];
						}
						// -64 - 64
						// 64 - 192
						rsum *= 2;
						gsum *= 2;
						bsum *= 2;
						rsum += 127.5;
						gsum += 127.5;
						bsum += 127.5;
						rsum = floor(rsum + 0.5);
						gsum = floor(gsum + 0.5);
						bsum = floor(bsum + 0.5);
						//rsum *= 1.335;//(2.0 - 0.5);
						//gsum *= 1.335;//(2.0 - 0.5);
						//bsum *= 1.335;//(2.0 - 0.5);

						if(rsum < min_)			min_ = rsum;
						else if(rsum > max_)	max_ = rsum;

						if(rsum < 0){
							static int	under_count = 0;
							//fprintf(stderr, "R underflow[%5d] : %f\n", under_count ++, rsum);
						}else if(rsum > 256){
							static int	over_count = 0;
							//fprintf(stderr, "R overflow[%5d]  : %f\n", over_count ++, rsum);
						}
						if(rsum < 0)	rsum = 0;
						else if(rsum > 255)	rsum = 255;
						if(gsum < 0)	gsum = 0;
						else if(gsum > 255)	gsum = 255;
						if(bsum < 0)	bsum = 0;
						else if(bsum > 255)	bsum = 255;

						out(i, j).r = rsum;
						out(i, j).g = gsum;
						out(i, j).b = bsum;
						//out( i, j ) = color_type( static_cast< value_type >( rsum ), static_cast< value_type >( gsum ), static_cast< value_type >( bsum ) );
					//}else{
					//	out( i, j ) = in( i, j );
					//}
				}
#else
				for(j = 0; j < out.height(); j ++)
				{
					out( i, j ) = color_type( static_cast< value_type >( rbuf[j]+127.5 ), static_cast< value_type >( (int)gbuf[j]+127.5 ), static_cast< value_type >( (int)bbuf[j]+127.5 ) );
					//out( i, j ) = color_type( static_cast< value_type >( (int)rbuf[j] ), static_cast< value_type >( (int)gbuf[j] ), static_cast< value_type >( (int)bbuf[j] ) );
				}
#endif
			}
			fprintf(stderr, "min %f, max %f\n", min_, max_);

			delete [] pCoef;

			delete [] rbuf;
			delete [] gbuf;
			delete [] bbuf;

			return true;
		}
	};
}


//! @addtogroup interlace_group インターレス除去
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/interlace.h>
//! @endcode
//!
//!  @{


/// @brief 2次元画像のインターレス除去を行う
//! 
//! 偶数ライン・奇数ラインのどちらかを補間する
//! @attention インターレス除去を行うためには，画像が2行以上でなくてはならない
//! 
//! @param[in]  in          … 入力画像
//! @param[out] out         … 出力画像
//! @param[in]  is_odd_line … 奇数ラインを偶数ラインで補間する場合は true，偶数ラインを奇数ラインで補間する場合は false を指定する
//! 
//! @return インターレス除去がうまくいったかどうか
//! 
template < class T, class Allocator >
bool interlace( const array2< T, Allocator > &in, array2< T, Allocator > &out, bool is_odd_line = false )
{
	if( is_odd_line )
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_odd( in, out ) );
	}
	else
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_even( in, out ) );
	}
}


/// @brief 2次元画像のノンインターレス変換（Bob）を行う
//! 
//! 偶数ライン・奇数ラインのどちらかを補間する
//! @attention インターレス除去を行うためには，画像が2行以上でなくてはならない
//! 
//! @param[in]  in          … 入力画像
//! @param[out] out         … 出力画像
//! @param[in]  is_odd_line … 奇数ラインを偶数ラインで補間する場合は true，偶数ラインを奇数ラインで補間する場合は false を指定する
//! 
//! @return インターレス除去がうまくいったかどうか
//! 
template < class T, class Allocator >
bool deinterlace_bob1( const array2< T, Allocator > &in, array2< T, Allocator > &out, bool is_odd_line = false )
{
	if( is_odd_line )
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::deinterlace_bob1_odd( in, out ) );
	}
	else
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::deinterlace_bob1_even( in, out ) );
	}
}



/// @}
//  インターレス除去グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_INTERLACE__
