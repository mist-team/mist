/// @file mist/config/type_trait.h
//!
//! @brief �^�̏����擾�E���肷�邽�߂̃��C�u����
//!

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#define __INCLUDE_MIST_TYPE_TRAIT_H__

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// �^�����𒲂ׂ�


/// @brief char ����
//! 
//! T���P�o�C�g�̕�����^�ł���ΐ^�ɕ]������
//! 
//! @param T  �c ��������^
//! 
template< class T > struct is_char        { _MIST_CONST( bool, value, false ); };
template<> struct is_char< unsigned char >{ _MIST_CONST( bool, value, true  ); };
template<> struct is_char< signed char >  { _MIST_CONST( bool, value, true  ); };
template<> struct is_char< char >         { _MIST_CONST( bool, value, true  ); };


/// @brief ���������_ ����
//! 
//! T �� float �� double �^�ł���ΐ^�ɕ]������
//! 
//! @param T  �c ��������^
//! 
template< class T > struct is_float       { _MIST_CONST( bool, value, false ); };
template<> struct is_float< float >       { _MIST_CONST( bool, value, true  ); };
template<> struct is_float< double >      { _MIST_CONST( bool, value, true  ); };
template<> struct is_float< long double > { _MIST_CONST( bool, value, true  ); };



/// @brief integer ����
//! 
//! T �������^�ł���ΐ^�ɕ]������B�Đ����^������ɂ�����
//! 
//! @param T  �c ��������^
//! 
template< class T > struct is_integer                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_integer< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_integer< char >          { _MIST_CONST( bool, value, true  ); };



/// @brief arithmetic ����
//! 
//! T ���Z�p�^�ł���ΐ^�ɕ]������B�Đ����^�����������_�^�̂����ꂩ������ɂ�����
//! 
//! @param T  �c ��������^
//! 
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




/// @brief �^�̏��i�^��Ԃ�
//! 
//! T1 �� T2 �̎Z�p�^�ŉ��Z���s���ۂɁC�^�̏��i���s�������ʂ̌^��Ԃ��D
//! ���Ƃ��΁Cdouble �� int �̉��Z�ł́C�����v�Z�� double �ōs���悤�ȍۂɗp����D
//! 
//! @param T1  �c ��������^1
//! @param T2  �c ��������^2
//! 
template< class T1, class T2 > struct promote_trait { typedef T1 value_type; };

#define PROMOTE_TRAIT( T1, T2, T3 )	\
		template< > struct promote_trait< T1, T2 > { typedef T3 value_type; };	\
		template< > struct promote_trait< T2, T1 > { typedef T3 value_type; };

PROMOTE_TRAIT( unsigned  char, unsigned short, unsigned short )
PROMOTE_TRAIT( unsigned  char, unsigned   int, unsigned   int )
PROMOTE_TRAIT( unsigned  char, unsigned  long, unsigned  long )
PROMOTE_TRAIT( unsigned  char,   signed  char,   signed short )
PROMOTE_TRAIT( unsigned  char,   signed short,   signed short )
PROMOTE_TRAIT( unsigned  char,   signed   int,   signed   int )
PROMOTE_TRAIT( unsigned  char,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned  char,           bool, unsigned  char )
PROMOTE_TRAIT( unsigned  char,           char,   signed  char )
PROMOTE_TRAIT( unsigned  char,          float,          float )
PROMOTE_TRAIT( unsigned  char,         double,         double )
PROMOTE_TRAIT( unsigned  char,    long double,    long double )
PROMOTE_TRAIT(   signed  char, unsigned short,   signed   int )
PROMOTE_TRAIT(   signed  char, unsigned   int,   signed  long )
PROMOTE_TRAIT(   signed  char, unsigned  long,   signed  long )
PROMOTE_TRAIT(   signed  char,   signed short,   signed short )
PROMOTE_TRAIT(   signed  char,   signed   int,   signed   int )
PROMOTE_TRAIT(   signed  char,   signed  long,   signed  long )
PROMOTE_TRAIT(   signed  char,           bool,   signed  char )
PROMOTE_TRAIT(   signed  char,           char,   signed  char )
PROMOTE_TRAIT(   signed  char,          float,          float )
PROMOTE_TRAIT(   signed  char,         double,         double )
PROMOTE_TRAIT(   signed  char,    long double,    long double )
PROMOTE_TRAIT(           char, unsigned short,   signed   int )
PROMOTE_TRAIT(           char, unsigned   int,   signed  long )
PROMOTE_TRAIT(           char, unsigned  long,   signed  long )
PROMOTE_TRAIT(           char,   signed short,   signed short )
PROMOTE_TRAIT(           char,   signed   int,   signed   int )
PROMOTE_TRAIT(           char,   signed  long,   signed  long )
PROMOTE_TRAIT(           char,           bool,           char )
PROMOTE_TRAIT(           char,          float,          float )
PROMOTE_TRAIT(           char,         double,         double )
PROMOTE_TRAIT(           char,    long double,    long double )
PROMOTE_TRAIT( unsigned short, unsigned   int, unsigned   int )
PROMOTE_TRAIT( unsigned short, unsigned  long, unsigned  long )
PROMOTE_TRAIT( unsigned short,   signed short,   signed   int )
PROMOTE_TRAIT( unsigned short,   signed   int,   signed   int )
PROMOTE_TRAIT( unsigned short,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned short,           bool, unsigned short )
PROMOTE_TRAIT( unsigned short,          float,          float )
PROMOTE_TRAIT( unsigned short,         double,         double )
PROMOTE_TRAIT( unsigned short,    long double,    long double )
PROMOTE_TRAIT(   signed short, unsigned   int,   signed   int )
PROMOTE_TRAIT(   signed short, unsigned  long,   signed  long )
PROMOTE_TRAIT(   signed short,   signed   int,   signed   int )
PROMOTE_TRAIT(   signed short,   signed  long,   signed  long )
PROMOTE_TRAIT(   signed short,           bool,   signed short )
PROMOTE_TRAIT(   signed short,          float,          float )
PROMOTE_TRAIT(   signed short,         double,         double )
PROMOTE_TRAIT(   signed short,    long double,    long double )
PROMOTE_TRAIT( unsigned   int, unsigned  long, unsigned  long )
PROMOTE_TRAIT( unsigned   int,   signed   int,   signed  long )
PROMOTE_TRAIT( unsigned   int,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned   int,           bool, unsigned   int )
PROMOTE_TRAIT( unsigned   int,          float,         double )
PROMOTE_TRAIT( unsigned   int,         double,         double )
PROMOTE_TRAIT( unsigned   int,    long double,    long double )
PROMOTE_TRAIT(   signed   int, unsigned  long,   signed  long )
PROMOTE_TRAIT(   signed   int,   signed  long,   signed  long )
PROMOTE_TRAIT(   signed   int,           bool,   signed   int )
PROMOTE_TRAIT(   signed   int,          float,         double )
PROMOTE_TRAIT(   signed   int,         double,         double )
PROMOTE_TRAIT(   signed   int,    long double,    long double )
PROMOTE_TRAIT( unsigned  long,   signed  long,   signed  long )
PROMOTE_TRAIT( unsigned  long,           bool, unsigned  long )
PROMOTE_TRAIT( unsigned  long,          float,    long double )
PROMOTE_TRAIT( unsigned  long,         double,         double )
PROMOTE_TRAIT( unsigned  long,    long double,    long double )
PROMOTE_TRAIT(   signed  long,           bool,   signed   int )
PROMOTE_TRAIT(   signed  long,          float,         double )
PROMOTE_TRAIT(   signed  long,         double,         double )
PROMOTE_TRAIT(   signed  long,    long double,    long double )
PROMOTE_TRAIT(           bool,          float,          float )
PROMOTE_TRAIT(           bool,         double,         double )
PROMOTE_TRAIT(           bool,    long double,    long double )
PROMOTE_TRAIT(          float,         double,         double )
PROMOTE_TRAIT(          float,    long double,    long double )
PROMOTE_TRAIT(         double,    long double,    long double )

#undef PROMOTE_TRAIT


/// @brief �^�̏��i��p������r���Z�q���`����ۂɗ��p����
#define DEFINE_PROMOTE_CONDITION_OPERATOR( TYPE, OPERATOR )										\
	template < class T1, class T2 >																\
	inline bool operator OPERATOR( const TYPE< T1 > &t1, const TYPE< T2 > &t2 )					\
	{																							\
		return( TYPE< typename promote_trait< T1, T2 >::value_type >( t1 )						\
					OPERATOR TYPE< typename promote_trait< T1, T2 >::value_type >( t2 ) );		\
	}

/// @brief �^�̏��i��p�����l�����Z�q���`����ۂɗ��p����
//!
//! �I�u�W�F�N�g '���Z�q' �I�u�W�F�N�g ����������ۂɗ��p����D
//!
#define DEFINE_PROMOTE_BIND_OPERATOR1( TYPE, OPERATOR )											\
	template < class T1, class T2 >																\
	inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
				operator OPERATOR( const TYPE< T1 > &t1, const TYPE< T2 > &t2 )					\
	{																							\
		return( TYPE< typename promote_trait< T1, T2 >::value_type >( t1 ) OPERATOR ## = t2 );	\
	}

/// @brief �^�̏��i��p�����l�����Z�q���`����ۂɗ��p����
//!
//! �I�u�W�F�N�g '���Z�q' �萔 ����������ۂɗ��p����D
//!
#define DEFINE_PROMOTE_BIND_OPERATOR2( TYPE, OPERATOR )											\
	template < class T1, class T2 >																\
	inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
				operator OPERATOR( const TYPE< T1 > &t1, const T2 &t2 )							\
	{																							\
		return( TYPE< typename promote_trait< T1, T2 >::value_type >( t1 ) OPERATOR ## = t2 );	\
	}

/// @brief �^�̏��i��p�����l�����Z�q���`����ۂɗ��p����
//!
//! �萔 '���Z�q' �I�u�W�F�N�g ����������ۂɗ��p����D��ɁC�����Z�ȊO�ŗ��p����D
//!
#define DEFINE_PROMOTE_BIND_OPERATOR3( TYPE, OPERATOR )											\
	template < class T1, class T2 >																\
	inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
				operator OPERATOR( const T1 &t1, const TYPE< T2 > &t2 )							\
	{																							\
		return( TYPE< typename promote_trait< T1, T2 >::value_type >( t2 ) OPERATOR ## = t1 );	\
	}

/// @brief �^�̏��i��p�����l�����Z�q���`����ۂɗ��p����
//!
//! �萔 '�|' �I�u�W�F�N�g ����������ۂɗ��p����D�������C1�����̃R���X�g���N�^�����p�\�łȂ��Ă͂Ȃ�Ȃ��D
//!
#define DEFINE_PROMOTE_BIND_OPERATOR4( TYPE, OPERATOR )											\
	template < class T1, class T2 >																\
	inline TYPE< typename promote_trait< T1, T2 >::value_type >									\
				operator OPERATOR( const T1 &t1, const TYPE< T2 > &t2 )							\
	{																							\
		return( TYPE< typename promote_trait< T1, T2 >::value_type >( t1 ) OPERATOR ## = t2 );	\
	}


/// @brief �f�[�^�^�̊m�������邽�߂ɗ��p����
//! 
//! @param T  �c �擾�������^
//! 
template< class T >
struct type_trait{ typedef T value_type; };



/// @brief �^��AND���Z���s��
//! 
//! �S�Ă��^�̏ꍇ�̂ݐ^�ƂȂ�
//! 
//! @param b1 �c �p�����[�^1
//! @param b2 �c �p�����[�^2
//! @param b3 �c �p�����[�^3
//! @param b4 �c �p�����[�^4
//! @param b5 �c �p�����[�^5
//! @param b6 �c �p�����[�^6
//! @param b7 �c �p�����[�^7
//! 
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



/// @brief �^��OR���Z���s���^
//! 
//! �ǂꂩ�ЂƂł��^�̏ꍇ�ɐ^�ƂȂ�
//! 
//! @param b1 �c �p�����[�^1
//! @param b2 �c �p�����[�^2
//! @param b3 �c �p�����[�^3
//! @param b4 �c �p�����[�^4
//! @param b5 �c �p�����[�^5
//! @param b6 �c �p�����[�^6
//! @param b7 �c �p�����[�^7
//! 
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




/// @brief �^��NOT���Z���s���^
//! 
//! �^�̏ꍇ�͋U�ɁC�U�̏ꍇ�͐^�ɂȂ�
//! 
//! @param b1 �c �p�����[�^
//! 
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




/// @brief �^��EQUAL���Z���s���^
//! 
//! �w�肳�ꂽ2�̌^���������ꍇ�̂ݐ^�ƂȂ�
//! 
//! @param b1 �c �p�����[�^1
//! @param b2 �c �p�����[�^2
//! 
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




// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_TYPE_TRAIT_H__
