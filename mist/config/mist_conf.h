#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__



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


// 行列演算のオプション
#define _USE_BALANCING_MATRIX_EIGEN_		1	// 行列の対角化を行うことで計算精度を上げる（若干メモリを大目に食う）
#define _USE_DIVIDE_AND_CONQUER_SVD_		1	// 分割統治法を用いた高速な特異値分解を利用する（若干メモリを大目に食う）


#if _CHECK_ACCESS_VIOLATION_

	#define _CHECK_ACCESS_VIOLATION1_( index ) \
		if( index < 0 || index >= size_ )\
		{\
			static value_type dmy;\
			std::cout << "Access Violation at ( " << index << " )" << std::endl;\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION2_( index1, index2 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ )\
		{\
			static value_type dmy;\
			std::cout << "Access Violation at ( " << index1 << ", " << index2 << " )" << std::endl;\
			return( dmy );\
		}\

	#define _CHECK_ACCESS_VIOLATION3_( index1, index2, index3 ) \
		if( index1 < 0 || index1 >= size1_ || index2 < 0 || index2 >= size2_ || index3 < 0 || index3 >= size3_ )\
		{\
			static value_type dmy;\
			std::cout << "Access Violation at ( " << index1 << ", " << index2 << ", " << index3 << " )" << std::endl;\
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
