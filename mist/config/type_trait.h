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


//
//
///// @brief �^�̏��i�^��Ԃ�
////! 
////! T ���Z�p�^�ł���ΐ^�ɕ]������B�Đ����^�����������_�^�̂����ꂩ������ɂ�����
////! 
////! @param T  �c ��������^
////! 
//template< class T > struct promote_trait                  { _MIST_CONST( bool, value, false ); };
//template<>          struct is_arithmetic< unsigned char > { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< unsigned short >{ _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< unsigned int >  { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< unsigned long > { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed char >   { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed short >  { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed int >    { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< signed long >   { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< bool >          { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< char >          { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< float >         { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< double >        { _MIST_CONST( bool, value, true  ); };
//template<>          struct is_arithmetic< long double >   { _MIST_CONST( bool, value, true  ); };



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
