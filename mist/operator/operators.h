/// @file mist/operator/operators.h
//!
//! @brief ���Z�q�̎�����⏕���邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_OPERATORS__
#define __INCLUDE_MIST_OPERATORS__

#include "../config/mist_conf.h"


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

//! @addtogroup operator_group ���Z�q�̎����⏕
//!  @{



/// @brief �����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_add
{
	friend T operator +( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj += obj2;
		return( obj );
	}
};


/// @brief �����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_sub
{
	friend T operator -( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj -= obj2;
		return( obj );
	}
};



/// @brief �|���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_mul
{
	friend T operator *( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj *= obj2;
		return( obj );
	}
};



/// @brief ����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_div
{
	friend T operator /( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj /= obj2;
		return( obj );
	}
};



/// @brief �������Ȃ�
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_ne
{
	friend bool operator !=( const T &obj1, const T &obj2 )
	{
		return( !( obj1 == obj2 ) );
	}
};



/// @brief ���傫��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_gt
{
	friend bool operator >( const T &obj1, const T &obj2 )
	{
		return( obj2 < obj1 );
	}
};



/// @brief �ȉ�
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_le
{
	friend bool operator <=( const T &obj1, const T &obj2 )
	{
		return( !( obj2 < obj1 ) );
	}
};



/// @brief �ȏ�
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_ge
{
	friend bool operator >=( const T &obj1, const T &obj2 )
	{
		return( !( obj1 < obj2 ) );
	}
};



/// @brief %���Z�q
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_modulo
{
	friend T operator %( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj %= obj2;
		return( obj );
	}
};



/// @brief |���Z�q
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_bitor
{
	friend T operator |( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj |= obj2;
		return( obj );
	}
};



/// @brief &���Z�q
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_bitand
{
	friend T operator &( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj &= obj2;
		return( obj );
	}
};



/// @brief ^���Z�q
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operator_exclusiveor
{
	friend T operator ^( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj ^= obj2;
		return( obj );
	}
};



/// @brief �l�����Z�I�y���[�^����������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_arithmetic_operators : public
	implement_operator_add< T >,
	implement_operator_sub< T >,
	implement_operator_mul< T >,
	implement_operator_div< T >
{
};



/// @brief ��r�I�y���[�^����������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_comparison_operators : public
	implement_operator_ne < T >,
	implement_operator_gt < T >,
	implement_operator_le < T >,
	implement_operator_ge < T >
{
};




/// @brief �r�b�g���Z�I�y���[�^����������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_bit_operators : public
	implement_operator_bitor< T >,
	implement_operator_bitand< T >,
	implement_operator_exclusiveor< T >
{
};



/// @brief �S�ẴI�y���[�^����������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
struct implement_operators : public
	implement_arithmetic_operators< T >,
	implement_comparison_operators< T >,
	implement_operator_modulo< T >,
	implement_bit_operators< T >
{
};


/// @}
//  ���Z�q�̎����⏕�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_OPERATORS__
