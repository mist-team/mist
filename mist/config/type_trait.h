#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#define __INCLUDE_MIST_TYPE_TRAIT_H__

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif


// mist–¼‘O‹óŠÔ‚Ìn‚Ü‚è
_MIST_BEGIN


// Œ^‘®«‚ğ’²‚×‚é

// char ”»’è
// T‚ª‚PƒoƒCƒg‚Ì•¶š—ñŒ^‚Å‚ ‚ê‚Î^‚É•]‰¿‚·‚é
template< class T > struct is_char        { _MIST_CONST( bool, value, false ); };
template<> struct is_char< unsigned char >{ _MIST_CONST( bool, value, true  ); };
template<> struct is_char< signed char >  { _MIST_CONST( bool, value, true  ); };
template<> struct is_char< char >         { _MIST_CONST( bool, value, true  ); };

// •‚“®¬”“_ ”»’è
// T ‚ª float ‚© double Œ^‚Å‚ ‚ê‚Î^‚É•]‰¿‚·‚é
template< class T > struct is_float       { _MIST_CONST( bool, value, false ); };
template<> struct is_float< float >       { _MIST_CONST( bool, value, true  ); };
template<> struct is_float< double >      { _MIST_CONST( bool, value, true  ); };
template<> struct is_float< long double > { _MIST_CONST( bool, value, true  ); };

// arithmetic ”»’è
// T ‚ªZpŒ^‚Å‚ ‚ê‚Î^‚É•]‰¿‚·‚éB”Ä®”Œ^‚©•‚“®¬”“_Œ^‚Ì‚¢‚¸‚ê‚©‚ª‚±‚ê‚É‚ ‚½‚é
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

// ƒf[ƒ^Œ^‚Ì‚ÉŠÖ‚·‚éî•ñ
template< class T >
struct type_trait{ typedef T value_type; };

// Œ^‚ÌAND‰‰Z‚ğs‚¤Œ^
template < bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true, bool b6 = true, bool b7 = true >
struct type_and
{
    _MIST_CONST( bool, value, false );
};

template <>
struct type_and< true, true, true, true, true, true, true >
{
    _MIST_CONST( bool, value, true  );
};

// Œ^‚ÌOR‰‰Z‚ğs‚¤Œ^
template < bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false, bool b6 = false, bool b7 = false >
struct type_or
{
    _MIST_CONST( bool, value, true );
};

template <>
struct type_or< false, false, false, false, false, false, false >
{
    _MIST_CONST( bool, value, false  );
};

// Œ^‚ÌNOT‰‰Z‚ğs‚¤Œ^
template < bool b1 >
struct type_not
{
    _MIST_CONST( bool, value, true  );
};

template <>
struct type_not< true >
{
    _MIST_CONST( bool, value, false );
};


// Œ^‚ÌNOT‰‰Z‚ğs‚¤Œ^
template < bool b1, bool b2 >
struct type_equal
{
    _MIST_CONST( bool, value, false );
};

template <>
struct type_equal< true, true >
{
    _MIST_CONST( bool, value, true  );
};

template <>
struct type_equal< false, false >
{
    _MIST_CONST( bool, value, true  );
};




// mist–¼‘O‹óŠÔ‚ÌI‚í‚è
_MIST_END


#endif // __INCLUDE_MIST_TYPE_TRAIT_H__
