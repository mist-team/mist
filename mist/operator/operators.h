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
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/operator/operators.h>
//! @endcode
//!
//!  @{



/// @brief �����Z
//! 
//! �u+=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u+�v�������I�Ɏ�������
//! 
//! @attention �u+=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_add< hoge >
//! {
//!      hoge &operator +=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *hoge );
//!      }
//! };
//! @endcode
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
//! �u-=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u-�v�������I�Ɏ�������
//! 
//! @attention �u-=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_sub< hoge >
//! {
//!      hoge &operator -=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *hoge );
//!      }
//! };
//! @endcode
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
//! �u*=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u*�v�������I�Ɏ�������
//! 
//! @attention �u*=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_mul< hoge >
//! {
//!      hoge &operator *=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *hoge );
//!      }
//! };
//! @endcode
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
//! �u/=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u/�v�������I�Ɏ�������
//! 
//! @attention �u/=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_div< hoge >
//! {
//!      hoge &operator /=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *hoge );
//!      }
//! };
//! @endcode
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
//! �u==�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u!=�v�������I�Ɏ�������
//! 
//! @attention �u==�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_ne< hoge >
//! {
//!      bool operator ==( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( ... );
//!      }
//! };
//! @endcode
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
//! �u<�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u>�v�������I�Ɏ�������
//! 
//! @attention �u<�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_gt< hoge >
//! {
//!      bool operator <( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( ... );
//!      }
//! };
//! @endcode
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
//! �u<�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u<=�v�������I�Ɏ�������
//! 
//! @attention �u<�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_le< hoge >
//! {
//!      bool operator <( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( ... );
//!      }
//! };
//! @endcode
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
//! �u<�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u>=�v�������I�Ɏ�������
//! 
//! @attention �u<�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_ge< hoge >
//! {
//!      bool operator <( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( ... );
//!      }
//! };
//! @endcode
//! 
template < class T >
struct implement_operator_ge
{
	friend bool operator >=( const T &obj1, const T &obj2 )
	{
		return( !( obj1 < obj2 ) );
	}
};



/// @brief ��]���Z�q
//! 
//! �u%=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u%�v�������I�Ɏ�������
//! 
//! @attention �u%=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_mod< hoge >
//! {
//!      hoge &operator %=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *this );
//!      }
//! };
//! @endcode
//! 
template < class T >
struct implement_operator_mod
{
	friend T operator %( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj %= obj2;
		return( obj );
	}
};



/// @brief �r�b�gOR���Z�q
//! 
//! �u|=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u|�v�������I�Ɏ�������
//! 
//! @attention �u|=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_or< hoge >
//! {
//!      hoge &operator |=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *this );
//!      }
//! };
//! @endcode
//! 
template < class T >
struct implement_operator_or
{
	friend T operator |( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj |= obj2;
		return( obj );
	}
};



/// @brief �r�b�gAND���Z�q
//! 
//! �u&=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u&�v�������I�Ɏ�������
//! 
//! @attention �u&=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_and< hoge >
//! {
//!      hoge &operator &=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *this );
//!      }
//! };
//! @endcode
//! 
template < class T >
struct implement_operator_and
{
	friend T operator &( const T &obj1, const T &obj2 )
	{
		T obj( obj1 );
		obj &= obj2;
		return( obj );
	}
};



/// @brief XOR���Z�q
//! 
//! �u^=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u^�v�������I�Ɏ�������
//! 
//! @attention �u^=�v�I�y���[�^�́C�K���������Ă����K�v������܂��D
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
//! @code �g�p��
//! class hoge : mist::implement_operator_xor< hoge >
//! {
//!      hoge &operator ^=( const hoge &h )
//!      {
//!          ... �������������� ...
//!          return( *this );
//!      }
//! };
//! @endcode
//! 
template < class T >
struct implement_operator_xor
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
//! �u+=�v�C�u-=�v�C�u*=�v�C�u/=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u+�v�C�u-�v�C�u*�v�C�u/�v�������I�Ɏ�������
//! 
//! @param T  �c �{�I�y���[�^����������N���X
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
//! �u==�v�C�u<�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u!=�v�C�u>�v�C�u<=�v�C�u>=�v�������I�Ɏ�������
//! 
//! @param T  �c �{�I�y���[�^����������N���X
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
//! �u|=�v�C�u&=�v�C�u^=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC�Q�����Z�q�́u|�v�C�u&�v�C�u^�v�������I�Ɏ�������
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
template < class T >
struct implement_bit_operators : public
	implement_operator_or< T >,
	implement_operator_and< T >,
	implement_operator_xor< T >
{
};



/// @brief �S�ẴI�y���[�^����������
//! 
//! �u+=�v�C�u-=�v�C�u*=�v�C�u/=�v�C�u==�v�C�u<�v�C�u|=�v�C�u&=�v�C�u^=�v�I�y���[�^���������Ă���N���X�ɑ΂��āC
//! �Q�����Z�q�́u+�v�C�u-�v�C�u*�v�C�u/�v�C�u!=�v�C�u>�v�C�u<=�v�C�u>=�v�C�u|�v�C�u&�v�C�u^�v�������I�Ɏ�������
//! 
//! @param T  �c �{�I�y���[�^����������N���X
//! 
template < class T >
struct implement_operators : public
	implement_arithmetic_operators< T >,
	implement_comparison_operators< T >,
	implement_operator_mod< T >,
	implement_bit_operators< T >
{
};


/// @}
//  ���Z�q�̎����⏕�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_OPERATORS__
