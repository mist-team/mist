#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__


#include <iostream>


// MIST名前空間を定義するためのマクロ
#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
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
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	0	// VC標準のSTLではサポートしていないのでオフにする
#else
	#define _MIST_ALLOCATOR_MEMORY_TRIM_	1	// VC以外のSTLではサポートしているのでオンにする
#endif


#ifndef NDEBUG

	// デバッグ用の設定を全てオンにする
	#define _CHECK_ACCESS_VIOLATION_		1	// 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			1	// 1次元配列の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY1_OPERATION_		1	// 1次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY2_OPERATION_		1	// 2次元画像の演算の整合性をチェックするかどうか
	#define _CHECK_ARRAY3_OPERATION_		1	// 3次元画像の演算の整合性をチェックするかどうか

	#define _CHECK_MATRIX_OPERATION_		1	// 行列演算の整合性をチェックするかどうか
	#define _USE_EXPRESSION_TEMPLATE_		0	// Expression Templateを利用するかどうか

#else

	// デバッグ用の設定を全てオフにする
	#define _CHECK_ACCESS_VIOLATION_		0	// 配列要素へのアクセス違反をチェックするかどうか
	#define _CHECK_ARRAY_OPERATION_			0
	#define _CHECK_ARRAY1_OPERATION_		0
	#define _CHECK_ARRAY2_OPERATION_		0
	#define _CHECK_ARRAY3_OPERATION_		0

	#define _CHECK_MATRIX_OPERATION_		0
	#define _USE_EXPRESSION_TEMPLATE_		1

#endif

#define _MIST_THREAD_SUPPORT_				1	// MISTの各アルゴリズムでのスレッドサポートをするかどうか
												// サポートする場合は，LINUX系の場合は pthread ライブラリが必要

#define _MIST_VECTOR_SUPPORT_				0	// MISTのVECTORを有効にする．MATRIXとの掛け算等もサポートする．
												// STLのvectorとクラス名がかぶるため，名前空間を正しくインポートする必要がある．


// 行列演算のオプション
#define _USE_BALANCING_MATRIX_EIGEN_		1	// 行列の対角化を行うことで計算精度を上げる（若干メモリを大目に食う）
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	// 分割統治法を用いた高速な特異値分解を利用する（若干メモリを大目に食う）


// MISTのアルゴリズム全般で利用するダミーコールバックファンクタ
// MISTのアルゴリズムが提供するコールバックは0～100の間の数を返し，アルゴリズムが終了する際に100よりも大きい数値を返す
// また，コールバックファンクタの戻り値が false の場合はMISTのアルゴリズムは処理を中断できる場合は中断し，直ちに制御を返します．
// その際に，途中の処理結果は失われるかもしくは意味のない結果となる可能性があります
struct __mist_dmy_callback__
{
	bool operator()( double percent ) const { return( true ); }
};

struct __mist_console_callback__
{
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

	inline void mist_debug_assertion( ptrdiff_t index )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index ) << " )" << ::std::endl;
	}

	inline void mist_debug_assertion( ptrdiff_t index1, ptrdiff_t index2 )
	{
		::std::cout << "Access Violation at ( " << static_cast< int >( index1 ) << ", " << static_cast< int >( index2 ) << " )" << ::std::endl;
	}

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


// mist名前空間の終わり
_MIST_END


#endif
