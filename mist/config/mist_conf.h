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

/// @file mist/config/mist_conf.h
//!
//! @brief MISTの設定ファイル
//!

#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__

#include <iostream>
#include <cstdlib>
#include <string>


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
#if defined( _MSC_VER )
	#if _MSC_VER <= 1200
		// Visual C++6.0
		#define __MIST_MSVC__		6
	#elif _MSC_VER < 1400
		// Visual Studio .NET 2002 - 2003バージョン
		#define __MIST_MSVC__		7
	#else
		// Visual Studio 2005 バージョン
		#define __MIST_MSVC__		8
	#endif
#endif


#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ >= 8
	// セキュリティ上の理由で、sprintf や fopen などの関数呼び出しが警告となる問題への対処
	// ここでの設定は、MISTをインクルードするすべてのファイルに影響があります
	//
	// 次のいずれかを選択する

	// 挙動が不明なので、今のところ警告を無視する
	#pragma warning( disable : 4996 )

	//#undef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
	//#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES		0
	////#define _CRT_SECURE_NO_DEPRECATE					1	// セキュリティが弱い古い関数を利用する
	//#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
	//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES		1	// セキュリティが強化された関数を利用する（実行に時間が必要となる可能性あり）
#endif

// Microsoft Windows かどうかをチェック
#if defined( WIN64 ) || defined( _WIN64 )
	// 64bit Windows
	#define __MIST_WINDOWS__	2
#elif defined( WIN32 ) || defined( _WIN32 )
	// 32bit Windows
	#define __MIST_WINDOWS__	1
#endif

#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( __AMD64__ )
	// 64bit OS
	#define __MIST64__	64
#else
	// 32bit OS
	#define __MIST32__	32
#endif



// Microsoft Visual C++ 6.0 では，static const を未サポートのため，enum で代用する
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	#define _MIST_CONST( type, name, value ) enum{ name = value }
#else
	#define _MIST_CONST( type, name, value ) static const type name = value
#endif


// 構造体のアライメントを合わせるためのマクロ
#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#define _MIST_PACKED
#else
	#define _MIST_PACKED __attribute__( ( packed ) )
#endif



// MISTヘッダ内で利用する設定
#define _MIST_ALLOCATE_TEST_			0

//#ifdef __MIST_MSVC__
//	#define _MIST_ALLOCATOR_MEMORY_TRIM_	0		///< VC標準のSTLではサポートしていないのでオフにする
//#else
//	#define _MIST_ALLOCATOR_MEMORY_TRIM_	1		///< VC以外のSTLではサポートしているのでオンにする
//#endif

#define _MIST_ALLOCATOR_MEMORY_TRIM_	0			///< すでに確保されているメモリ領域の切り詰めを行うアロケータをサポートする場合はオンにする

#define _MIST_UNUSED_( arg )	( void )( arg )		///< 未使用の変数に関する警告を表示しなくするためのマクロ

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6


// mist名前空間の終わり
_MIST_END

	#include <cmath>

	// VC6では、STD名前空間にABS関数等が入っていない問題を修正
	#define STD_CMATH_FUNC1( name, func, value ) inline value name ( value v ){ return( ::func( v ) ); }
	#define STD_CMATH_FUNC2( name, func, value ) inline value name ( value v1, value v2 ){ return( ::func( v1, v2 ) ); }
	namespace std
	{
		STD_CMATH_FUNC1(   abs,    abs, int )
		STD_CMATH_FUNC1(   abs,   labs, long )
		STD_CMATH_FUNC1(   abs,   fabs, double )
		STD_CMATH_FUNC1(  sqrt,   sqrt, double )
		STD_CMATH_FUNC1(   sin,    sin, double )
		STD_CMATH_FUNC1(   cos,    cos, double )
		STD_CMATH_FUNC1(   log,    log, double )
		STD_CMATH_FUNC1( log10,  log10, double )
		STD_CMATH_FUNC1(   exp,    exp, double )
		STD_CMATH_FUNC2(   pow,    pow, double )
	}
	#undef STD_CMATH_FUNC1
	#undef STD_CMATH_FUNC2

// mist名前空間の始まり
_MIST_BEGIN

#endif


//! @addtogroup config_group MIST全般で利用される設定
//!
//! @code 次のヘッダを参照
//! #include <mist/config/mist_conf.h>
//! @endcode
//!
//! @{


#ifndef _MIST_USER_CONFIG_

#if !defined( NDEBUG ) || defined( DEBUG )

	// デバッグ用の設定を全てオンにする
	#define _CHECK_ACCESS_VIOLATION_		1	///< 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			1	///< 1次元配列の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY1_OPERATION_		1	///< 1次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY2_OPERATION_		1	///< 2次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY3_OPERATION_		1	///< 3次元画像の演算の整合性をチェックするかどうか

	#define _CHECK_MATRIX_OPERATION_		1	///< 行列演算の整合性をチェックするかどうか

#else

	// デバッグ用の設定を全てオフにする
	#define _CHECK_ACCESS_VIOLATION_		0	///< 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			0	///< 1次元配列の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY1_OPERATION_		0	///< 1次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY2_OPERATION_		0	///< 2次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY3_OPERATION_		0	///< 3次元画像の演算の整合性をチェックするかどうか

	#define _CHECK_MATRIX_OPERATION_		0	/// <行列演算の整合性をチェックするかどうか

#endif


#define __CHECK_HARD_CASE__					0	///< CONDOR アルゴリズムにおいて，Trust Region を解く際に Hard Case のチェックをするかどうか．チェックをするほうが安定した最適化が可能になる場合があります．

#define _ARRAY_BIND_OPERATION_SUPPORT_		1	///< MISTの array  コンテナ同士の四則演算を有効にするかどうか
#define _ARRAY1_BIND_OPERATION_SUPPORT_		1	///< MISTの array1 コンテナ同士の四則演算を有効にするかどうか
#define _ARRAY2_BIND_OPERATION_SUPPORT_		1	///< MISTの array2 コンテナ同士の四則演算を有効にするかどうか
#define _ARRAY3_BIND_OPERATION_SUPPORT_		1	///< MISTの array3 コンテナ同士の四則演算を有効にするかどうか


#define _MIST_THREAD_SUPPORT_				1	///< MISTの各アルゴリズムでのスレッドサポートをするかどうか．サポートする場合は，LINUX系の場合は pthread ライブラリが必要

#define _MIST_VECTOR_SUPPORT_				0	///< MISTのVECTORを有効にする．MATRIXとの掛け算等もサポートする．STLのvectorとクラス名がかぶるため，名前空間を正しくインポートする必要がある．

#define _LZW_COMPRESSION_SUPPORT_		false	///< TIFFの出力で，LZW圧縮を有効にするオプションをデフォルトにするかどうか

#define _COMPRESSED_CSV_SUPPORT_			0	///< ZLIB圧縮されたCSVファイルの読み込みをサポートするかどうか


// 行列演算のオプション
#define _USE_INTEL_MATH_KERNEL_LIBRARY_		0	///< インテルが提供している Math Kernel Library を利用する場合は 1 にする
#define _DESCENDING_ORDER_EIGEN_VALUE_		0	///< 固有値・固有ベクトルを計算した時に，降順に並べる時は 1，昇順に並べる時は 0 にする
#define _USE_BALANCING_MATRIX_EIGEN_		1	///< 行列の対角化を行うことで計算精度を上げる（若干メモリを大目に食う）
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	///< 分割統治法を用いた高速な特異値分解を利用する（若干メモリを大目に食う）

#endif


/// @}
//  MIST全般で利用される設定の終わり


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

/// @brief UNICODE文字列をマルチバイト文字列に変換する
//!
//! @param[in] src … UNICODE文字列
//!
//! @return マルチバイト文字列
//!
inline const std::string wstr2str( const std::wstring &src )
{
	static bool is_initialized = false;
	if( !is_initialized )
	{
		is_initialized = true;
		setlocale( LC_CTYPE, "" );
	}

	size_t nbytes = wcstombs( NULL, src.c_str( ), 0 );
	char *buff = new char[ nbytes + 1 ];
	wcstombs( buff, src.c_str( ), nbytes + 1 );
	std::string dst( buff );
	delete [] buff;
	return( dst );
}



//! @addtogroup callback_group MIST全般で利用できるコールバック関数
//!
//! @code 次のヘッダを参照
//! #include <mist/config.h>
//! @endcode
//!
//! @{


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
	bool operator()( long double /* percent */ ) const { return( true ); }
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
	bool operator()( long double percent )
	{
		percent = lower_ + percent / 100.0 * ( upper_ - lower_ );
		return( f_( percent ) );
	}

	__mist_convert_callback__( ) : lower_( 0.0 ), upper_( 100.0 ){ }
	__mist_convert_callback__( Functor f, double l = 0.0, double u = 100.0 ) : f_( f ), lower_( l ), upper_( u ){ }
};


/// @brief MISTのアルゴリズム全般で利用可能な，標準出力型コールバックファンクタ
//!
//! @code 進行状況の表示例
//! busy... 99%
//! @endcode
//!
struct __mist_console_callback__
{
private:
	int __percent__;

public:
	/// @brief 標準出力型コールバックファンクタのデフォルトコンストラクタ．
	__mist_console_callback__( ) : __percent__( -1 )
	{
	}

	/// @brief MISTのアルゴリズムの進行状況を，0～100パーセントで受け取り，標準出力へ出力する．
	//!
	//! @param[in] percent … アルゴリズムの進行状況
	//!
	//! @return true  … アルゴリズムの実行を継続
	//! @return false … ユーザー側からのキャンセルにより，アルゴリズムの実行を中止
	//!
	bool operator()( long double percent )
	{
		int ppp = static_cast< int >( percent );
		if( __percent__ != ppp )
		{
			__percent__ = ppp;

			int k3 = static_cast< int >( percent / 100.0 );
			percent -= k3 * 100.0;
			int k2 = static_cast< int >( percent / 10.0 );
			percent -= k2 * 10.0;
			int k1 = static_cast< int >( percent );

			std::cerr << "busy... ";
			if( k3 == 0 )
			{
				std::cerr << " ";
				if( k2 == 0 )
				{
					std::cerr << " " << k1;
				}
				else
				{
					std::cerr << k2 << k1;
				}
			}
			else
			{
				std::cerr << 1 << k2 << k1;
			}
			if( percent > 100.0 )
			{
				std::cerr << "%" << std::endl;
			}
			else
			{
				std::cerr << "%\r";
			}
		}
		return( true );
	}
};




/// @brief MISTのアルゴリズム全般で利用可能な，進行状況表示型ダミーコールバックファンクタ
//!
//! 進行状況のバーは，コンストラクタが呼び出された際に，コンソールに表示される
//!
//! @code 進行状況の表示例
//! 0%   10   20   30   40   50   60   70   80   90   100%
//! |----|----|----|----|----|----|----|----|----|----|
//! *********************************
//! @endcode
//!
struct __mist_progress_callback__
{
	__mist_progress_callback__( )
	{
		std::cerr << "0%   10   20   30   40   50   60   70   80   90   100%" << std::endl;
		std::cerr << "|----|----|----|----|----|----|----|----|----|----|" << std::endl;
	}

	/// @brief MISTのアルゴリズムの進行状況を，0～100パーセントで受け取り，標準出力へ出力する．
	//!
	//! @param[in] percent … アルゴリズムの進行状況
	//!
	//! @return true  … アルゴリズムの実行を継続
	//! @return false … ユーザー側からのキャンセルにより，アルゴリズムの実行を中止
	//!
	bool operator()( long double percent ) const
	{
		if( percent > 100.0 )
		{
			std::cerr << "***************************************************" << std::endl;
		}
		else
		{
			int i, p = static_cast< int >( percent / 2.0 );

			for( i = 0 ; i < p ; i++ )
			{
				std::cerr << "*";
			}
			for( ; i < 51 ; i++ )
			{
				std::cerr << " ";
			}

			std::cerr << "\r";
		}

		return( true );
	}
};



/// @}
//  MIST全般で利用できるコールバック関数



#if _CHECK_ACCESS_VIOLATION_

	/// @brief DEBUGでMISTをビルドした際に，1次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param[in] index … MISTコンテナに対するアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index )
	{
		::std::cerr << "Access Violation at ( " << static_cast< int >( index ) << " )" << ::std::endl;
	}


	/// @brief DEBUGでMISTをビルドした際に，2次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param[in] index1 … MISTコンテナに対するX軸でのアクセス要求位置
	//! @param[in] index2 … MISTコンテナに対するY軸でのアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2 )
	{
		::std::cerr << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << " )" << ::std::endl;
	}


	/// @brief DEBUGでMISTをビルドした際に，3次元アクセスでの範囲外アクセスをチェックし，エラーの際に呼ばれる関数．
	//!
	//! @param[in] index1 … MISTコンテナに対するX軸でのアクセス要求位置
	//! @param[in] index2 … MISTコンテナに対するY軸でのアクセス要求位置
	//! @param[in] index3 … MISTコンテナに対するZ軸でのアクセス要求位置
	//!
	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2, ptrdiff_t index3 )
	{
		::std::cerr << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << ", " << static_cast< int >( index3 ) << " )" << ::std::endl;
	}


	#define _CHECK_ACCESS_VIOLATION1U_( index ) \
		if( index >= size( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION1S_( index ) \
		if( index < 0 || index >= size( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION2U_( index1, index2 ) \
		if( index1 >= size1( ) || index2 >= size2( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2 );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION2S_( index1, index2 ) \
		if( index1 < 0 || index1 >= size1( ) || index2 < 0 || index2 >= size2( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2 );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION3U_( index1, index2, index3 ) \
		if( index1 >= size1( ) || index2 >= size2( ) || index3 >= size3( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2, index3 );\
			return( dmy );\
		}

	#define _CHECK_ACCESS_VIOLATION3S_( index1, index2, index3 ) \
		if( index1 < 0 || index1 >= size1( ) || index2 < 0 || index2 >= size2( ) || index3 < 0 || index3 >= size3( ) )\
		{\
			static value_type dmy;\
			mist_debug_assertion( index1, index2, index3 );\
			return( dmy );\
		}

#else

	#define _CHECK_ACCESS_VIOLATION1U_( index ) 
	#define _CHECK_ACCESS_VIOLATION1S_( index ) 
	#define _CHECK_ACCESS_VIOLATION2U_( index1, index2 )
	#define _CHECK_ACCESS_VIOLATION2S_( index1, index2 )
	#define _CHECK_ACCESS_VIOLATION3U_( index1, index2, index3 )
	#define _CHECK_ACCESS_VIOLATION3S_( index1, index2, index3 )

#endif



/*** Doxygen 用のメインページ ***********************************************************************************************/
/*! @mainpage MIST ( Media Integration Standard Toolkit )
*
*
* @section config MISTの基本設定
*
* - @ref config_group			"基本設定"
* - @ref callback_group			"MIST全般で利用できるコールバック関数
*
* @section class MISTで提供する基本コンテナ
*
* - @ref mist::array			"array"          : STLに準拠したメモリ上で連続となる1次元配列であり，音声・画像の全てのデータの基底クラス
* - @ref mist::array1			"array1"         : 音声を扱うための1次元配列
* - @ref mist::array2			"array2"         : 2次元画像を扱うための画像クラス
* - @ref mist::array3			"array3"         : 3次元画像を扱うための画像クラス
* - @ref mist::bitmap			"bitmap"         : ビットマップ画像を扱うためのクラス
* - @ref mist::marray			"marray"         : 1・2・3次元画像の縁にマージンを持った画像を扱うクラス
* - @ref mist::buffered_array	"buffered_array" : 画像の一辺が2の指数乗となる画像を扱うクラス
* - @ref mist::matrix			"matrix"         : 任意の行列を扱うためのクラス（Expression template を利用した高速な演算が可能）
* - @ref vector_group			"ベクトル演算を可能とするクラス"
*   - @ref mist::vector2		"vector2"        : 2次元ベクトルを扱うクラス
*   - @ref mist::vector3		"vector3"        : 3次元ベクトルを扱うクラス
*   - @ref mist::vector			"vector"         : N次元ベクトルを扱うクラス
*
*
* @section element MISTで提供する基本データ型
* - @ref mist::binary						"バイナリ画素"
* - @ref color_group						"色空間を扱うクラス"
*   - @ref mist::rgb						"カラー画素"
*   - @ref mist::bgr						"Windows用のビットマップで利用されるカラー画素"
*   - @ref mist::rgba						"カラー画素（アルファ付き）"
* - @ref mist::stereo						"ステレオ音声"
* - @ref mist::quaternion					"クォータニオン（四元数）"
* - @ref set_group							"集合"
* - @ref facet_group						"ポリゴン関連"
*   - @ref mist::facet						"ポリゴンを扱うクラス"
*   - @ref mist::facet_list					"ポリゴンの集合を扱うクラス"
*   - @ref mist::surface_simplification		"ポリゴン数の削減"
*
*
* @section algorithm MISTで提供するアルゴリズム
*
*
* @subsection implement	実装補助
* - @ref limits_group						"型に対する情報"
* - @ref pointer_group						"自動的にメモリ管理を行うポインタ"
* - @ref mist::timer						"時間計測"
* - @ref environment_group					"計算機環境情報"
* - @ref operator_group						"演算子の実装補助"
* - @ref thread_group						"マルチスレッド"
*   - @ref mist::thread_pool				"スレッドプール"
*   - @ref mist::singleton					"シングルトン"
* - @ref free_form_group					"自由曲線・曲面"
*   - @ref bspline_group					"Bスプライン曲線"
*   - @ref spline_group						"3次スプライン曲線"
* - @ref mist::crc							"CRC"
* - @ref hash_group							"ハッシュ関数"
*   - @ref mist::md2						"MD2"
*   - @ref mist::md4						"MD4"
*   - @ref mist::md5						"MD5"
*   - @ref mist::sha1						"SHA-1"
*   - @ref mist::sha256						"SHA-256"
*   - @ref mist::sha384						"SHA-384"
*   - @ref mist::sha512						"SHA-512"
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
* - @ref fourier_group							"フーリエ変換"
*   - @ref fft_group							"高速フーリエ変換 (FFT)"
*   - @ref dct_group							"離散コサイン変換(DCT)"
*   - @ref dst_group							"離散サイン変換 (DST) "
* - @ref a										"ウェーブレット変換"
* - @ref statistics_group						"統計処理"
*   - @ref mist::statistics::average			"平均値計算"
*   - @ref mist::statistics::variance			"分散計算"
*   - @ref mist::statistics::generate_histogram	"ヒストグラム作成"
*   - @ref mixture_group						"混合正規分布の推定"
* - @ref random_group							"擬似乱数生成"
*   - @ref mist::uniform::random				"一様乱数"
*   - @ref mist::gauss::random					"正規乱数"
* - @ref a										"グラフ描画(2次元)"
* - 画像や基本オブジェクトの描画
*   - @ref image_draw_group						"OpenGLを用いた2次元画像描画"
*   - @ref image_drawing_group					"直線・円の描画"
* - @ref minimization_group						"関数の最小化"
* - @ref registration_group						"レジストレーション"
*   - @ref mist::non_rigid						"非剛体レジストレーション"
* - @ref visualization_group					"可視化"
*   - @ref volumerendering_group				"ボリュームレンダリング"
*   - @ref mist::marching_cubes					"等濃度面表示"
* - キャリブレーション
*   - @ref calibration_group					"カメラキャリブレーション"
* - @ref machine_learning_group					"機械学習"
*   - @ref mist::machine_learning::adaboost		"Multiclass AdaBoost"
* - @ref hht_group								"経験的モード分解およびヒルベルト変換"
*   - @ref mist::hilbert						"ヒルベルト変換"
*   - @ref mist::emd							"経験的モード分解"
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
*   - @ref image_gif_group					"GIFデータの読み込み"
*   - @ref image_tiff_group					"TIFFデータの読み込み・書き出し"
*   - @ref image_tga_group					"TGAデータの読み込み・書き出し"
*   - @ref dicom_group						"DICOMデータの読み込み・書き出し"
*
*
* @subsubsection image-filter フィルタ系
* - @ref linear_group								"線形フィルタ"
* - @ref median_group								"メディアンフィルタ"
* - @ref region_growing_group						"領域拡張法"
* - @ref interpolate_group							"画像補間"
*   - @ref mist::nearest							"最近傍型補間"
*   - @ref mist::mean								"平均値型補間"
*   - @ref mist::linear								"線形補間"
*   - @ref mist::cubic								"3次補間（sinc関数の3次近似を利用した補間）"
*   - @ref mist::BSpline							"3次のBスプライン補間"
*   - @ref mist::sinc								"sinc関数補間（全画素を畳み込むので計算コスト大）"
* - @ref edge_group									"エッジ検出"
*   - @ref mist::canny								"Cannyエッジ検出器"
* - @ref corner_group								"コーナー検出"
*   - @ref mist::harris								"Harrisコーナー検出器"
* - カラー画像に対する処理
*   - @ref interlace_group							"インターレス除去"
* - @ref threshold_group							"閾値選択"
* - @ref morphology_group							"モルフォロジー演算"
* - 2値画像に対する処理
*   - @ref labeling_group							"ラベリング"
*   - @ref decomposition_group						"図形分割"
*   - @ref boundary_group							"境界画素抽出"
*   - @ref mode_group								"最頻値フィルタ"
*   - @ref thinning_group							"細線化"
*   - @ref skeleton_group							"スケルトン抽出"
*   - @ref distance_group									"距離変換"
*     - @ref voronoi_distance_group							"ボロノイ分割"
*     - @ref euclidean_distance_group						"ユークリッド2乗距離変換"
*     - @ref mist::euclidean::inverse_distance_transform	"ユークリッド2乗逆距離変換"
*     - @ref mist::manhattan::distance_transform			"マンハッタン距離（シティーブロック距離）変換"
*     - @ref mist::chessboard::distance_transform			"チェスボード距離変換"
*   - @ref fusion_group										"図形融合"
*     - @ref fusion_mdt_group								"マンハッタン距離（L1ノルム，シティーブロック距離）"
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
