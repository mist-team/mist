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

#if _MSC_VER <= 1200

#define _MIST_CONST( type, name, value ) enum{ name = value }

#else

#define _MIST_CONST( type, name, value ) static const type name = value

#endif


// 型属性を調べる

// char 判定
// Tが１バイトの文字列型であれば真に評価する
template< class T > struct is_char        { _MIST_CONST( bool, value, false ); };
template<> struct is_char< unsigned char >{ _MIST_CONST( bool, value, true  ); };
template<> struct is_char< signed char >  { _MIST_CONST( bool, value, true  ); };
template<> struct is_char< char >         { _MIST_CONST( bool, value, true  ); };

// arithmetic 判定
// T が算術型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
template< class T > struct is_arithmetic                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_arithmetic< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< char >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< float >         { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< double >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< long double >   { _MIST_CONST( bool, value, true  ); };

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