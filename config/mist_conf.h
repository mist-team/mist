#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__


#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
#endif

// mist–¼‘O‹óŠÔ‚Ìn‚Ü‚è
_MIST_BEGIN


// Œ^‘®«‚ğ’²‚×‚é
#if _MSC_VER <= 1200

// char ”»’è
// T‚ª‚PƒoƒCƒg‚Ì•¶š—ñŒ^‚Å‚ ‚ê‚Î^‚É•]‰¿‚·‚é
template< class T > struct is_char{ enum{ value = false }; };
template<> struct is_char< unsigned char >{ enum{ value = true }; };
template<> struct is_char< signed char >{ enum{ value = true }; };
template<> struct is_char< char >{ enum{ value = true }; };

// arithmetic ”»’è
// T ‚ªZpŒ^‚Å‚ ‚ê‚Î^‚É•]‰¿‚·‚éB”Ä®”Œ^‚©•‚“®¬”“_Œ^‚Ì‚¢‚¸‚ê‚©‚ª‚±‚ê‚É‚ ‚½‚é
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


// char ”»’è
// T‚ª‚PƒoƒCƒg‚Ì•¶š—ñŒ^‚Å‚ ‚ê‚Î^‚É•]‰¿‚·‚é
template< class T > struct is_char{ static const bool value = false; };
template<> struct is_char< unsigned char >{ static const bool value = true; };
template<> struct is_char< signed char >{ static const bool value = true; };
template<> struct is_char< char >{ static const bool value = true; };

// arithmetic ”»’è
// T ‚ªZpŒ^‚Å‚ ‚ê‚Î^‚É•]‰¿‚·‚éB”Ä®”Œ^‚©•‚“®¬”“_Œ^‚Ì‚¢‚¸‚ê‚©‚ª‚±‚ê‚É‚ ‚½‚é
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

// ƒf[ƒ^Œ^‚Ì‚ÉŠÖ‚·‚éî•ñ
template< class T >
struct type_trait{ typedef T type; };



// ƒf[ƒ^Œ^‚ÌƒLƒƒƒXƒg‚ÉŠÖ‚·‚éî•ñ
template< class T1, class T2 >
struct promote_trait{ };
//struct promote_trait{ typedef double promote_type; };

#define DECLARE_PROMOTE( TYPE1, TYPE2, PROMOTE ) template<> struct promote_trait< TYPE1, TYPE2 >{ typedef PROMOTE promote_type; };

// short op char -> short
DECLARE_PROMOTE( signed short, signed char, signed short );
DECLARE_PROMOTE( unsigned short, unsigned char, unsigned short );
DECLARE_PROMOTE( signed char, signed short, signed short );
DECLARE_PROMOTE( unsigned char, unsigned short, unsigned short );

// int op char -> int
DECLARE_PROMOTE( signed int, signed char, signed int );
DECLARE_PROMOTE( unsigned int, unsigned char, unsigned int );
DECLARE_PROMOTE( signed char, signed int, signed int );
DECLARE_PROMOTE( unsigned char, unsigned int, unsigned int );

// int op short -> int
DECLARE_PROMOTE( signed int, signed short, signed int );
DECLARE_PROMOTE( unsigned int, unsigned short, unsigned int );
DECLARE_PROMOTE( signed short, signed int, signed int );
DECLARE_PROMOTE( unsigned short, unsigned int, unsigned int );

// long op char -> long
DECLARE_PROMOTE( signed long, signed char, signed long );
DECLARE_PROMOTE( unsigned long, unsigned char, unsigned long );
DECLARE_PROMOTE( signed char, signed long, signed long );
DECLARE_PROMOTE( unsigned char, unsigned long, unsigned long );

// long op short -> long
DECLARE_PROMOTE( signed long, signed short, signed long );
DECLARE_PROMOTE( unsigned long, unsigned short, unsigned long );
DECLARE_PROMOTE( signed short, signed long, signed long );
DECLARE_PROMOTE( unsigned short, unsigned long, unsigned long );

// long op int -> long
DECLARE_PROMOTE( signed long, signed int, signed long );
DECLARE_PROMOTE( unsigned long, unsigned int, unsigned long );
DECLARE_PROMOTE( signed int, signed long, signed long );
DECLARE_PROMOTE( unsigned int, unsigned long, unsigned long );

// float op char -> float
DECLARE_PROMOTE( float, signed char, float );
DECLARE_PROMOTE( float, unsigned char, float );
DECLARE_PROMOTE( signed char, float, float );
DECLARE_PROMOTE( unsigned char, float, float );

// float op short -> float
DECLARE_PROMOTE( float, signed short, float );
DECLARE_PROMOTE( float, unsigned short, float );
DECLARE_PROMOTE( signed short, float, float );
DECLARE_PROMOTE( unsigned short, float, float );

// float op int -> double
DECLARE_PROMOTE( float, signed int, double );
DECLARE_PROMOTE( float, unsigned int, double );
DECLARE_PROMOTE( signed int, float, double );
DECLARE_PROMOTE( unsigned int, float, double );

// float op long -> double
DECLARE_PROMOTE( float, signed long, double );
DECLARE_PROMOTE( float, unsigned long, double );
DECLARE_PROMOTE( signed long, float, double );
DECLARE_PROMOTE( unsigned long, float, double );

// double op char -> double
DECLARE_PROMOTE( double, signed char, double );
DECLARE_PROMOTE( double, unsigned char, double );
DECLARE_PROMOTE( signed char, double, double );
DECLARE_PROMOTE( unsigned char, double, double );

// double op short -> double
DECLARE_PROMOTE( double, signed short, double );
DECLARE_PROMOTE( double, unsigned short, double );
DECLARE_PROMOTE( signed short, double, double );
DECLARE_PROMOTE( unsigned short, double, double );

// double op int -> double
DECLARE_PROMOTE( double, signed int, double );
DECLARE_PROMOTE( double, unsigned int, double );
DECLARE_PROMOTE( signed int, double, double );
DECLARE_PROMOTE( unsigned int, double, double );

// double op long -> double
DECLARE_PROMOTE( double, signed long, double );
DECLARE_PROMOTE( double, unsigned long, double );
DECLARE_PROMOTE( signed long, double, double );
DECLARE_PROMOTE( unsigned long, double, double );

// float op double -> double
DECLARE_PROMOTE( double, float, double );
DECLARE_PROMOTE( float, double, double );


// char op char -> char
DECLARE_PROMOTE( signed char, signed char, signed char );
DECLARE_PROMOTE( unsigned char, unsigned char, unsigned char );

// short op short -> short
DECLARE_PROMOTE( signed short, signed short, signed short );
DECLARE_PROMOTE( unsigned short, unsigned short, unsigned short );

// int op int -> int
DECLARE_PROMOTE( signed int, signed int, signed int );
DECLARE_PROMOTE( unsigned int, unsigned int, unsigned int );

// long op long -> long
DECLARE_PROMOTE( signed long, signed long, signed long );
DECLARE_PROMOTE( unsigned long, unsigned long, unsigned long );

// float op float -> float
DECLARE_PROMOTE( float, float, float );

// double op double -> double
DECLARE_PROMOTE( double, double, double );



#undef DECLARE_PROMOTE




// mist–¼‘O‹óŠÔ‚ÌI‚í‚è
_MIST_END


#endif