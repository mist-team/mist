#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__


#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
#endif

// mist名前空間の始まり
_MIST_BEGIN


// 型属性を調べる
#if _MSC_VER <= 1200

// char 判定
// Tが１バイトの文字列型であれば真に評価する
template< class T > struct is_char{ enum{ value = false }; };
template<> struct is_char< unsigned char >{ enum{ value = true }; };
template<> struct is_char< signed char >{ enum{ value = true }; };
template<> struct is_char< char >{ enum{ value = true }; };

// arithmetic 判定
// T が算術型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
template< class T > struct is_arithmetic{ enum{ value = false }; };
template<> struct is_arithmetic< unsigned char >{ enum{ value = true }; };
template<> struct is_arithmetic< unsigned short >{ enum{ value = true }; };
template<> struct is_arithmetic< unsigned int >{ enum{ value = true }; };
template<> struct is_arithmetic< unsigned long >{ enum{ value = true }; };
template<> struct is_arithmetic< signed char >{ enum{ value = true }; };
template<> struct is_arithmetic< signed short >{ enum{ value = true }; };
template<> struct is_arithmetic< signed int >{ enum{ value = true }; };
template<> struct is_arithmetic< signed long >{ enum{ value = true }; };
template<> struct is_arithmetic< bool >{ enum{ value = true }; };
template<> struct is_arithmetic< char >{ enum{ value = true }; };
template<> struct is_arithmetic< float >{ enum{ value = true }; };
template<> struct is_arithmetic< double >{ enum{ value = true }; };
template<> struct is_arithmetic< long double >{ enum{ value = true }; };

#else


// char 判定
// Tが１バイトの文字列型であれば真に評価する
template< class T > struct is_char{ static const bool value = false; };
template<> struct is_char< unsigned char >{ static const bool value = true; };
template<> struct is_char< signed char >{ static const bool value = true; };
template<> struct is_char< char >{ static const bool value = true; };

// arithmetic 判定
// T が算術型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
template< class T > struct is_arithmetic{ static const bool value = false; };
template<> struct is_arithmetic< unsigned char >{ static const bool value = true; };
template<> struct is_arithmetic< unsigned short >{ static const bool value = true; };
template<> struct is_arithmetic< unsigned int >{ static const bool value = true; };
template<> struct is_arithmetic< unsigned long >{ static const bool value = true; };
template<> struct is_arithmetic< signed char >{ static const bool value = true; };
template<> struct is_arithmetic< signed short >{ static const bool value = true; };
template<> struct is_arithmetic< signed int >{ static const bool value = true; };
template<> struct is_arithmetic< signed long >{ static const bool value = true; };
template<> struct is_arithmetic< bool >{ static const bool value = true; };
template<> struct is_arithmetic< char >{ static const bool value = true; };
template<> struct is_arithmetic< float >{ static const bool value = true; };
template<> struct is_arithmetic< double >{ static const bool value = true; };
template<> struct is_arithmetic< long double >{ static const bool value = true; };


#endif


// データ型のに関する情報
template< class T >
struct type_trait{ typedef T value_type; };



#define _MIST_ALLOCATE_TEST_			0

#ifdef WIN32

#define _MIST_ALLOCATOR_MEMORY_TRIM__	0	// VC標準のSTLではサポートしていないのでオフにする

#else

#define _MIST_ALLOCATOR_MEMORY_TRIM__	1	// VC標準のSTLではサポートしていないのでオフにする

#endif


#ifndef NDEBUG

#define _CHECK_ACCESS_VIOLATION_		1	// 配列要素へのアクセス違反をチェックするかどうか
#define _CHECK_ARRAY_OPERATION_			1	// 1次元配列の演算の整合性をチェックするかどうか
#define _CHECK_ARRAY1_OPERATION_		1	// 1次元画像の演算の整合性をチェックするかどうか
#define _CHECK_ARRAY2_OPERATION_		1	// 2次元画像の演算の整合性をチェックするかどうか
#define _CHECK_ARRAY3_OPERATION_		1	// 3次元画像の演算の整合性をチェックするかどうか

#define _CHECK_MATRIX_OPERATION_		1	// 行列演算の整合性をチェックするかどうか
#define _USE_EXPRESSION_TEMPLATE_		0	// Expression Templateを利用するかどうか

#else

#define _CHECK_ACCESS_VIOLATION_		0	// 配列要素へのアクセス違反をチェックするかどうか
#define _CHECK_ARRAY_OPERATION_			0
#define _CHECK_ARRAY1_OPERATION_		0
#define _CHECK_ARRAY2_OPERATION_		0
#define _CHECK_ARRAY3_OPERATION_		0

#define _CHECK_MATRIX_OPERATION_		0
#define _USE_EXPRESSION_TEMPLATE_		1

#endif



// mist名前空間の終わり
_MIST_END


#endif