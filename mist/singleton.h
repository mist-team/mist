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

/// @file mist/singleton.h
//!
//! @brief Singleton ���������߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_SINGLETON__
#define __INCLUDE_MIST_SINGLETON__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// @brief Singleton �������N���X
//! 
//! singleton< int, 0 > �� singleton< int, 1 > ��ʕ��Ƃ��Ĉ������Ƃ��ł���D
//! �Q�ڂ̈������ȗ����Csingleton< int > �Ƃ���� ID �� 0 �̃V���O���g�����쐬�����D
//! 
//! @attention ���s���̏����������͐����������Ȃ��̂Œ��ӂ��K�v
//! @attention ���̂��߁C���I�ȕϐ���{Singleton�N���X�̃C���X�^���X�ŏ��������邱�Ƃ͂ł��Ȃ�
//! @attention �{�N���X�̎��Ԃ��쐬���邱�Ƃ͂ł��Ȃ�
//! @attention ���Ԃ̏������́C�ŏ��Ɋ֐����Ăяo���ꂽ���_�ōs����D
//! 
//! @param T  �c Singleton���쐬����^
//! @param ID �c �����^�ŕ�����Singleton���쐬����ꍇ�ɗ��p����ID
//! 
template < class T, size_t ID = 0 >
class singleton
{
public:
	typedef T value_type;	///< @brief Singleton���쐬����^

	/// @brief Singleton�̎��Ԃ�Ԃ��֐�
	//! 
	//! @return Singleton�̃C���X�^���X
	//! 
	static value_type &get_instance( )
	{
		static value_type singleton_;
		return( singleton_ );
	}

private:
	singleton( );
	singleton( const singleton &s );
	const singleton & operator =( const singleton &s );
};


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_SINGLETON__
