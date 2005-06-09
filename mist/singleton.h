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
//! �Q�ڂ̈������ȗ����Csingleton< int > �Ƃ���� ID �� 0 �̃V���O���g���K�쐬�����D
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
