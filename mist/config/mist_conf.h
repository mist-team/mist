/// @file mist/config/mist_conf.h
//!
//! @brief MISTの設定ファイル
//!

#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__


#include <iostream>


// MIST名前空間を定義するためのマクロ
#ifndef _MIST_BEGIN
/// @namespace mist
//!
//! MISTプロジェクトの名前空間．
//! すべての関数・クラスは mist 名前空間に含まれる．
//! 
#define _MIST_BEGIN		namespace mist{		///< @brief MIST名前空間の始まり
#endif

#ifndef _MIST_END
#define _MIST_END		}					///< @brief MIST名前空間の終わり
#endif



// mist名前空間の始まり
_MIST_BEGIN



// Microsoft Visual C++ のバージョンをチェック
#if defined( _MSC_VER ) && _MSC_VER <= 1200
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


#define _ARRAY_BIND_OPERATION_SUPPORT_		0	///< MISTの array  コンテナ同士の四則演算を有効にするかどうか
#define _ARRAY1_BIND_OPERATION_SUPPORT_		0	///< MISTの array1 コンテナ同士の四則演算を有効にするかどうか
#define _ARRAY2_BIND_OPERATION_SUPPORT_		0	///< MISTの array2 コンテナ同士の四則演算を有効にするかどうか
#define _ARRAY3_BIND_OPERATION_SUPPORT_		0	///< MISTの array3 コンテナ同士の四則演算を有効にするかどうか


#define _MIST_THREAD_SUPPORT_				1	///< MISTの各アルゴリズムでのスレッドサポートをするかどうか．サポートする場合は，LINUX系の場合は pthread ライブラリが必要

#define _MIST_VECTOR_SUPPORT_				0	///< MISTのVECTORを有効にする．MATRIXとの掛け算等もサポートする．STLのvectorとクラス名がかぶるため，名前空間を正しくインポートする必要がある．

#define _MIST_GIF_SUPPORT_					0	///< GIF画像の入出力をサポートするかどうか

#define _LZW_COMPRESSION_SUPPORT_		false	///< TIFFとGIF画像の出力で，LZW圧縮を有効にするオプションをデフォルトにするかどうか


// 行列演算のオプション
#define _DESCENDING_ORDER_EIGEN_VALUE_		0	///< 固有値・固有ベクトルを計算した時に，降順に並べる時は 1，昇順に並べる時は 0 にする
#define _USE_BALANCING_MATRIX_EIGEN_		1	///< 行列の対角化を行うことで計算精度を上げる（若干メモリを大目に食う）
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	///< 分割統治法を用いた高速な特異値分解を利用する（若干メモリを大目に食う）


/// @brief 入力された2つのオブジェクトが同じもの（メモリ上で同じ位置にある）かどうかを判定する
//!
//! @param[in] o1 … オブジェクト1
//! @param[in] o2 … オブジェクト2
//!
//! @return true  … 入力された2つのオブジェクトは，メモリ上で同じ位置に存在している
//! @return false … 入力された2つのオブジェクトは，別のメモリ上に存在している
//!
template < class Object1, class Object2 >
inline bool is_same_object( const Object1 &o1, const Object2 &o2 )
{
	return( reinterpret_cast< const void * >( &o1 ) == reinterpret_cast< const void * >( &o2 ) );
}


/// @brief MISTのアルゴリズム全般で利用するダミーコールバックファンクタ
//!
//! MISTのアルゴリズムが提供するコールバックは0～100の間の数を返し，アルゴリズムが終了する際に100よりも大きい数値を返す
//! また，コールバックファンクタの戻り値が false の場合はMISTのアルゴリズムは処理を中断できる場合は中断し，直ちに制御を返します．
//! その際に，途中の処理結果は失われるかもしくは意味のない結果となる可能性があります
//!
struct __mist_dmy_callback__
{
	/// @brief MISTのアルゴリズムの進行状況を，0～100パーセントで受け取り，何もしない関数．
	//!
	//! @param[in] percent … アルゴリズムの進行状況
	//!
	//! @return true  … アルゴリズムの実行を継続
	//! @return false … ユーザー側からのキャンセルにより，アルゴリズムの実行を中止
	//!
	bool operator()( double percent ) const { return( true ); }
};


/// @brief 指定したコールバックの戻り値 [0,100] を指定した区間に変換する
//!
//! MISTのアルゴリズムが提供するコールバックは0～100の間の数を返し，アルゴリズムが終了する際に100よりも大きい数値を返す
//! また，コールバックファンクタの戻り値が false の場合はMISTのアルゴリズムは処理を中断できる場合は中断し，直ちに制御を返します．
//! その際に，途中の処理結果は失われるかもしくは意味のない結果となる可能性があります
//!
template < class Functor >
struct __mist_convert_callback__
{
	Functor f_;
	double lower_;
	double upper_;

	/// @brief MISTのアルゴリズムの進行状況を，0～100パーセントで受け取り，指定した区間に変換する関数．
	//!
	//! @param[in] percent … アルゴリズムの進行状況
	//!
	//! @return true  … アルゴリズムの実行を継続
	//! @return false … ユーザー側からのキャンセルにより，アルゴリズムの実行を中止
	//!
	bool operator()( double percent ) const
	{
		percent = lower_ + percent / 100.0 * ( upper_ - lower_ );
		return( f_( percent ) );
	}

	__mist_convert_callback__( ) : lower_( 0.0 ), upper_( 100.0 ){ }
	__mist_convert_callback__( Functor f, double l = 0.0, double u = 100.0 ) : f_( f ), lower_( l ), upper_( u ){ }
};


/// @brief MISTのアルゴリズム全般で利用可能な，標準出力型ダミーコールバックファンクタ
//!
//! MISTのアルゴリズムが提供するコールバックは0～100の間の数を返し，アルゴリズムが終了する際に100よりも大きい数値を返す
//! また，コールバックファンクタの戻り値が false の場合はMISTのアルゴリズムは処理を中断できる場合は中断し，直ちに制御を返します．
//! その際に，途中の処理結果は失われるかもしくは意味のない結果となる可能性があります
//!
struct __mist_console_callback__
{
	/// @brief MISTのアルゴリズムの進行状況を，0～100パーセントで受け取り，標準出力へ出力する．
	//!
	//! @param[in] percent … アルゴリズムの進行状況
	//!
	//! @return true  … アルゴリズムの実行を継続
	//! @return false … ユーザー側からのキャンセルにより，アルゴリズムの実行を中止
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

	/// @brief DEBUGでMISTをビルドした際に，1次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param[in] index … MISTコンテナに対するアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index ) << " )" << ::std::endl;
	}


	/// @brief DEBUGでMISTをビルドした際に，2次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param[in] index1 … MISTコンテナに対するX軸でのアクセス要求位置
	//! @param[in] index2 … MISTコンテナに対するY軸でのアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2 )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << " )" << ::std::endl;
	}


	/// @brief DEBUGでMISTをビルドした際に，3次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param[in] index1 … MISTコンテナに対するX軸でのアクセス要求位置
	//! @param[in] index2 … MISTコンテナに対するY軸でのアクセス要求位置
	//! @param[in] index3 … MISTコンテナに対するZ軸でのアクセス要求位置
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
*
* @section class MISTで提供する基本コンテナ
*
*
* - @ref mist::array          "array"          : STLに準拠したメモリ上で連続となる1次元配列であり，音声・画像の全てのデータの基底クラス
* - @ref mist::array1         "array1"         : 音声を扱うための1次元配列
* - @ref mist::array2         "array2"         : 2次元画像を扱うための画像クラス
* - @ref mist::array3         "array3"         : 3次元画像を扱うための画像クラス
* - @ref mist::marray         "marray"         : 1・2・3次元画像の縁にマージンを持った画像を扱うクラス
* - @ref mist::buffered_array "buffered_array" : 画像の一辺が2の指数乗となる画像を扱うクラス
* - @ref mist::matrix         "matrix"         : 任意の行列を扱うためのクラス（Expression template を利用した高速な演算が可能）
* - @ref vector_group         "ベクトル演算を可能とするクラス"
*   - @ref mist::vector2      "vector2"        : 2次元ベクトルを扱うクラス
*   - @ref mist::vector3      "vector3"        : 3次元ベクトルを扱うクラス
*   - @ref mist::vector       "vector"         : N次元ベクトルを扱うクラス
*
*
* @section class MISTで提供する基本データ型
* - @ref mist::binary						"バイナリ画素"
* - @ref mist::rgb							"カラー画素"
* - @ref mist::stereo						"ステレオ音声"
* - @ref mist::quaternion					"クォータニオン（四元数）"
* - @ref set_group							"集合"
*
*
* @section algorithm MISTで提供するアルゴリズム
*
*
* @subsection implement	実装補助
* - @ref mist::type_limits					"型に対する情報"
* - @ref mist::timer						"時間計測"
* - @ref operator_group						"演算子の実装補助"
* - @ref thread_group						"マルチスレッド"
* - @ref mist::singleton					"シングルトン"
* - @ref free_form_group					"自由曲線・曲面"
*   - @ref bspline_group					"Bスプライン曲線"
*   - @ref spline_group						"3次スプライン曲線"
* - @ref crc_group							"CRC"
*
*
* @subsection numeric ベクトル・行列演算
* - @ref numeric_group "行列演算"
*   - @ref mist::trace						"トレース"
*   - @ref mist::det						"行列式"
*   - @ref mist::inverse					"逆行列"
*   - @ref mist::solve						"連立方程式を解く"
*   - @ref mist::qr_factorization			"QR 分解"
*   - @ref mist::lu_factorization			"LU 分解"
*   - @ref mist::eigen						"固有値・固有ベクトル"
*   - @ref mist::svd						"特異値分解"
*
*
* @subsection common 共通の処理
* - @ref fourier_group						"フーリエ変換"
*   - @ref fft_group						"高速フーリエ変換 (FFT)"
*   - @ref dct_group						"離散コサイン変換(DCT)"
*   - @ref dst_group						"離散サイン変換 (DST) "
* - @ref a									"ウェーブレット変換"
* - @ref statistics_group					"統計処理"
* - @ref a									"ヒストグラム制御"
* - @ref random_group						"擬似乱数生成"
*   - @ref mist::uniform::random			"一様乱数"
*   - @ref mist::gauss::random				"正規乱数"
* - @ref a									"グラフ描画(2次元)"
* - @ref image_draw_group					"2次元画像描画"
* - @ref minimization_group					"関数の最小化"
*
*
* @subsection image 主に画像に対して適用される処理
*
*
* @subsubsection image-io 入出力系
* - @ref image_group						"任意の画像データの読み込み・書き出し"
*   - @ref image_raw_group					"RAWデータの読み込み・書き出し"
*   - @ref image_bmp_group					"BMPデータの読み込み・書き出し"
*   - @ref image_pnm_group					"PNMデータの読み込み・書き出し"
*   - @ref image_jpeg_group					"JPEGデータの読み込み・書き出し"
*   - @ref image_png_group					"PNGデータの読み込み・書き出し"
*   - @ref image_gif_group					"GIFデータの読み込み・書き出し"
*   - @ref image_tiff_group					"TIFFデータの読み込み・書き出し"
*   - @ref dicom_group						"DICOMデータの読み込み・書き出し"
*
*
* @subsubsection image-filter フィルタ系
* - @ref linear_group						"線形フィルタ"
* - @ref median_group						"メディアンフィルタ"
* - @ref interpolate_group					"画像補間"
*   - @ref mist::nearest					"最近傍型補間"
*   - @ref mist::linear						"線形補間"
*   - @ref mist::cubic						"3次補間"
* - カラー画像に対する処理
*   - @ref interlace_group					"インターレス除去"
* - @ref threshold_group					"閾値選択"
* - @ref morphology_group					"モルフォロジー演算"
* - 2値画像に対する処理
*   - @ref labeling_group					"ラベリング"
*   - @ref thinning_group					"細線化"
*   - @ref distance_group					"距離変換"
*     - @ref euclidean_distance_group		"ユークリッド距離変換"
*     - @ref a								"4近傍型距離変換"
*     - @ref a								"8近傍型距離変換"
*     - @ref a								"6近傍型距離変換"
*     - @ref a								"26近傍型距離変換"
*   - @ref fusion_group						"図形融合"
*     - @ref fusion_mdt_group				"マンハッタン距離（L1ノルム，シティーブロック距離）"
*
*
* @subsection audio 主に音声に対して適用される処理
*
*
* @subsubsection audio-io 入出力系
* - @ref audio_group						"音声データの読み込み・書き出し"
*   - @ref audio_wav_group					"WAVデータの読み込み・書き出し"
*
*
* @subsubsection audio-filter フィルタ系
* - @ref a									"サンプリングレート変更"
* - @ref a									"再生"
*
*
****************************************************************************************************************************/





// mist名前空間の終わり
_MIST_END


#endif
