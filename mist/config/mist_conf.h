/// @file mist/config/mist_conf.h
//!
//! @brief MISTの設定ファイル
//!

#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__


#include <iostream>


// MIST名前空間を定義するためのマクロ
#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{		///< @brief MIST名前空間の始まり
#endif

#ifndef _MIST_END
#define _MIST_END		}					///< @brief MIST名前空間の終わり
#endif



// mist名前空間の始まり
_MIST_BEGIN



// Microsoft Visual C++ のバージョンをチェック
#if _MSC_VER <= 1200
	// Visual C++6.0
	#define __MIST_MSVC__		6
#elif _MSC_VER > 1200
	// Visual Studio .NETバージョン
	#define __MIST_MSVC__		7
#endif


// Microsoft Visual C++ 6.0 では，static const を見サポートのための enum による代用
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	#define _MIST_CONST( type, name, value ) enum{ name = value }
#else
	#define _MIST_CONST( type, name, value ) static const type name = value
#endif


// 構造体のアライメントを合わせるためのマクロ
#ifdef __MIST_MSVC__
	#define _MIST_ALIGN( type, alignment ) type
#else
	#define _MIST_ALIGN( type, alignment ) type __attribute__( ( aligned( alignment ) ) )
#endif



// MISTヘッダ内で利用する設定
#define _MIST_ALLOCATE_TEST_			0

#ifdef __MIST_MSVC__
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	0	///< VC標準のSTLではサポートしていないのでオフにする
#else
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	1	///< VC以外のSTLではサポートしているのでオンにする
#endif


#ifndef NDEBUG

	// デバッグ用の設定を全てオンにする
	#define _CHECK_ACCESS_VIOLATION_		1	///< 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			1	///< 1次元配列の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY1_OPERATION_		1	///< 1次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY2_OPERATION_		1	///< 2次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY3_OPERATION_		1	///< 3次元画像の演算の整合性をチェックするかどうか

	#define _CHECK_MATRIX_OPERATION_		1	///< 行列演算の整合性をチェックするかどうか
	#define _USE_EXPRESSION_TEMPLATE_		0	///< Expression Templateを利用するかどうか

#else

	// デバッグ用の設定を全てオフにする
	#define _CHECK_ACCESS_VIOLATION_		0	///< 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			0	///< 1次元配列の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY1_OPERATION_		0	///< 1次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY2_OPERATION_		0	///< 2次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY3_OPERATION_		0	///< 3次元画像の演算の整合性をチェックするかどうか

	#define _CHECK_MATRIX_OPERATION_		0	/// <行列演算の整合性をチェックするかどうか
	#define _USE_EXPRESSION_TEMPLATE_		1	///< Expression Templateを利用するかどうか

#endif


#define _MIST_THREAD_SUPPORT_				1	///< MISTの各アルゴリズムでのスレッドサポートをするかどうか．サポートする場合は，LINUX系の場合は pthread ライブラリが必要

#define _MIST_VECTOR_SUPPORT_				0	///< MISTのVECTORを有効にする．MATRIXとの掛け算等もサポートする．STLのvectorとクラス名がかぶるため，名前空間を正しくインポートする必要がある．


// 行列演算のオプション
#define _USE_BALANCING_MATRIX_EIGEN_		1	///< 行列の対角化を行うことで計算精度を上げる（若干メモリを大目に食う）
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	///< 分割統治法を用いた高速な特異値分解を利用する（若干メモリを大目に食う）


/// MISTのアルゴリズム全般で利用するダミーコールバックファンクタ
//!
//! MISTのアルゴリズムが提供するコールバックは0～100の間の数を返し，アルゴリズムが終了する際に100よりも大きい数値を返す
//! また，コールバックファンクタの戻り値が false の場合はMISTのアルゴリズムは処理を中断できる場合は中断し，直ちに制御を返します．
//! その際に，途中の処理結果は失われるかもしくは意味のない結果となる可能性があります
//!
struct __mist_dmy_callback__
{
	/// MISTのアルゴリズムの進行状況を，0～100パーセントで受け取り，何もしない関数．
	//!
	//! @param percent [in] アルゴリズムの進行状況
	//! @return false ユーザー側からのキャンセルにより，アルゴリズムの実行を中止．
	//! @return true  アルゴリズムの実行を継続
	//!
	bool operator()( double percent ) const { return( true ); }
};


/// MISTのアルゴリズム全般で利用可能な，標準出力型ダミーコールバックファンクタ
//!
//! MISTのアルゴリズムが提供するコールバックは0～100の間の数を返し，アルゴリズムが終了する際に100よりも大きい数値を返す
//! また，コールバックファンクタの戻り値が false の場合はMISTのアルゴリズムは処理を中断できる場合は中断し，直ちに制御を返します．
//! その際に，途中の処理結果は失われるかもしくは意味のない結果となる可能性があります
//!
struct __mist_console_callback__
{
	/// MISTのアルゴリズムの進行状況を，0～100パーセントで受け取り，標準出力へ出力する．
	//!
	//! @param percent [in] アルゴリズムの進行状況
	//! @return false ユーザー側からのキャンセルにより，アルゴリズムの実行を中止．
	//! @return true  アルゴリズムの実行を継続
	//!
	bool operator()( double percent ) const
	{
		int k3 = static_cast< int >( percent / 100.0 );
		percent -= k3 * 100.0;
		int k2 = static_cast< int >( percent / 10.0 );
		percent -= k2 * 10.0;
		int k1 = static_cast< int >( percent );

		std::cout << "busy... ";
		if( k3 == 0 )
		{
			std::cout << " ";
			if( k2 == 0 )
			{
				std::cout << " " << k1;
			}
			else
			{
				std::cout << k2 << k1;
			}
		}
		else
		{
			std::cout << 1 << k2 << k1;
		}
		if( percent > 100.0 )
		{
			std::cout << "%" << std::endl;
		}
		else
		{
			std::cout << "%\r";
		}
		return( true );
	}
};




#if _CHECK_ACCESS_VIOLATION_

	/// DEBUGでMISTをビルドした際に，1次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param index [in] MISTコンテナに対するアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index ) << " )" << ::std::endl;
	}

	/// DEBUGでMISTをビルドした際に，2次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param index1 [in] MISTコンテナに対するX軸でのアクセス要求位置
	//! @param index2 [in] MISTコンテナに対するY軸でのアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2 )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << " )" << ::std::endl;
	}

	/// DEBUGでMISTをビルドした際に，3次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param index1 [in] MISTコンテナに対するX軸でのアクセス要求位置
	//! @param index2 [in] MISTコンテナに対するY軸でのアクセス要求位置
	//! @param index3 [in] MISTコンテナに対するZ軸でのアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2, ptrdiff_t index3 )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << ", " << static_cast< int >( index3 ) << " )" << ::std::endl;
	}


	#define _CHECK_ACCESS_VIOLATION1_( index ) \
		if( index < 0 || index >= size_ )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index );\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION2_( index1, index2 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2 );\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION3_( index1, index2, index3 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ || index3 < 0 || index3 >= size3_ )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2, index3 );\
			return( dmy );\
		}\

#else

	#define _CHECK_ACCESS_VIOLATION1_( index ) 
	#define _CHECK_ACCESS_VIOLATION2_( index1, index2 )
	#define _CHECK_ACCESS_VIOLATION3_( index1, index2, index3 )

#endif



/*** Doxygen 用のメインページ ***********************************************************************************************/
/*! @mainpage MIST ( Media Integration Standard Toolkit )
*
* @section class MISTで提供する基本データ型
*
* - @ref mist::array "array"   : STLに準拠したメモリ上で連続となる１次元配列であり，音声・画像の全てのデータの基底クラス．
* - @ref mist::array1 "array1" : 音声を扱うための１次元配列．
* - @ref mist::array2 "array2" : ２次元画像を扱うための画像クラス．
* - @ref mist::array3 "array3" : ３次元画像を扱うための画像クラス．
* - @ref mist::matrix "matrix" : 任意の行列を扱うためのクラス（Expression template teqnique を利用した高速な演算が可能）．
* - @ref mist::vector "vector" : 行列・ベクトル演算を可能とするクラス．
*
*
*
* @section algorithm MISTで提供するアルゴリズム
*
* @subsection implement	実装補助
* - @ref mist::type_limits					"型に対する情報"
* - @ref mist::timer						"時間計測"
* - @ref operator_group						"演算子の実装補助"
* - @ref thread_group						"スレッド"
*
* @subsection numeric ベクトル・行列演算
* - @ref numeric_group "行列演算"
*   - @ref mist::inverse					"逆行列を計算"
*   - @ref mist::solve						"連立方程式を解く"
*   - @ref mist::qr_factorization			"QR 分解"
*   - @ref mist::lu_factorization			"LU 分解"
*   - @ref mist::eigen						"固有値・固有ベクトル"
*   - @ref mist::svd						"特異値分解"
*
* @subsection common 共通の処理
* - @ref fourier_group						"フーリエ変換"
*   - @ref fft_group						"高速フーリエ変換 (FFT)"
*   - @ref dct_group						"離散コサイン変換(DCT)"
*   - @ref dst_group						"離散サイン変換 (DST) "
* - @ref a									"ウェーブレット変換"
* - @ref a									"統計処理"
* - @ref a									"ヒストグラム制御"
* - @ref mist::random						"擬似乱数生成"
* - @ref a									"グラフ描画(2次元)"
* - @ref draw_group							"2次元画像描画"
*
* @subsection image 主に画像に対して適用される処理
*
* @subsubsection image-io 入出力系
* - @ref image_group						"任意の画像データの読み込み・書き出し"
*   - @ref image_raw_group					"RAWデータの読み込み・書き出し"
*   - @ref image_bmp_group					"BMPデータの読み込み・書き出し"
*   - @ref image_pnm_group					"PNMデータの読み込み・書き出し"
*   - @ref image_jpeg_group					"JPEGデータの読み込み・書き出し"
*   - @ref image_png_group					"PNGデータの読み込み・書き出し"
*   - @ref image_tiff_group					"TIFFデータの読み込み・書き出し"
*   - @ref image_dicom_group				"DICOMデータの読み込み・書き出し"
*
* @subsubsection image-filter フィルタ系
* - @ref linear_group						"線形フィルタ"
* - @ref median_group						"メディアンフィルタ"
* - @ref interpolate_group					"画像補間"
* - @ref a									"カラー画像に対する処理"
* - @ref threshold_group					"閾値選択"
* - @ref a									"2値画像に対する処理"
* - @ref morphology_group					"モルフォロジー演算"
*
* @subsection audio 主に音声に対して適用される処理
*
* @subsubsection audio-io 入出力系
* - @ref audio_group						"音声データの読み込み・書き出し"
*   - @ref audio_wav_group					"WAVデータの読み込み・書き出し"
*
* @subsubsection audio-filter フィルタ系
* - @ref a									"サンプリングレート変更"
* - @ref a									"再生"
*
****************************************************************************************************************************/





// mist名前空間の終わり
_MIST_END


#endif
