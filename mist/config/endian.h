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

/// @file mist/config/endian.h
//!
//! @brief �v�Z�@�̃G���f�B�A�����擾���C�����I�ɕϊ����郉�C�u����
//!

#ifndef __INCLUDE_MIST_ENDIAN__
#define __INCLUDE_MIST_ENDIAN__

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup endian_group �G���f�B�A���Ɋւ�����̎擾�y�ѕϊ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/config/endian.h>
//! @endcode
//!
//! @{




/// @brief �o�C�g�z��ƒl�̃y�A��\������N���X
//! 
//! �e�f�[�^�^�ƃo�C�g��𓯎��ɗ��p���邱�Ƃ��\�ł�
//! 
//! @param T �c �g�p����f�[�^�^
//! 
template < class T >
union byte_array
{
public:
	typedef T      value_type;		///< @brief �f�[�^�^�^�C�v
	typedef size_t size_type;		///< @brief size_t�ɓ���

private:
	value_type value;								///< @brief �f�[�^�^�^�C�v���\���l
	unsigned char byte[ sizeof( value_type ) ];		///< @brief �l���o�C�g�\���ɒ���������

public:
	/// @brief �f�t�H���g�R���X�g���N�^
	byte_array( ) : value( 0 ){ }


	/// @brief �l v ��p���ď���������
	byte_array( const value_type v ) : value( v ){ }


	/// @brief ���� byte_array ��p���ď���������
	byte_array( const byte_array &v ) : value( v.value ){ }


	/// @brief �o�C�g���p���ď��������s��
	//!
	//! @attention ���͂����o�C�g��̗v�f���́Csizeof( value_type ) �ȏ�łȂ��Ă͂Ȃ�Ȃ�
	//!
	byte_array( const unsigned char *b )
	{
		for( size_type i = 0 ; i < sizeof( value_type ) ; i++ )
		{
			byte[ i ] = b[ i ];
		}
	}

	/// @brief �����Ŏg�p����o�C�g����Ԃ�
	size_type length( ) const { return( sizeof( value_type ) ); }

	/// @brief index �Ŏw�肳��Ĉʒu�̃o�C�g���擾����
	unsigned char &operator[]( size_type index ){ return( byte[ index ] ); }

	/// @brief index �Ŏw�肳��Ĉʒu�� const �o�C�g���擾����
	const unsigned char &operator[]( size_type index ) const { return( byte[ index ] ); }

	/// @brief �l���擾����
	const value_type get_value( ) const { return( value ); }

	/// @brief �l��ݒ肷��
	value_type set_value( const value_type &v ) { return( value = v ); }

	/// @brief �o�C�g����擾����
	const unsigned char * get_bytes( ) const { return( byte ); }
};


/// @brief ���݂̃}�V���̃G���f�B�A�������g���G���f�B�A�����ǂ����𒲂ׂ�
//! 
//! @retval true  �c ���g���G���f�B�A��
//! @retval false �c �r�b�O�G���f�B�A��
//! 
inline bool _is_little_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 1 );
}


/// @brief ���݂̃}�V���̃G���f�B�A�����r�b�O�G���f�B�A�����ǂ����𒲂ׂ�
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @retval true  �c �r�b�O�G���f�B�A��
//! @retval false �c ���g���G���f�B�A��
//! 
inline bool _is_big_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 0 );
}


/// @brief byte_array���̃o�C�g�X���b�v������
//! 
//! @param[in,out] bytes �c �o�C�g�X���b�v�����f�[�^
//! 
template < class T >
inline void swap_bytes( byte_array< T > &bytes )
{
	typedef typename byte_array< T >::size_type size_type;
	byte_array< T > tmp( bytes );
	for( size_type i = 0 ; i < sizeof( T ) ; ++i )
	{
		bytes[ i ] = tmp[ sizeof( T ) - i - 1 ];
	}
}


/// @brief byte_array���̃f�[�^�����݂̌v�Z�@�̃G���f�B�A���ɍ��킹��
//! 
//! - ���g���G���f�B�A���̌v�Z�@�ŁC�r�b�O�G���f�B�A���̃f�[�^����͂��Cfrom_little_endian = false �Ƃ���ƁC�ϊ���̃f�[�^�̓��g���G���f�B�A���ƂȂ�
//! - �r�b�O�G���f�B�A���̌v�Z�@�ŁC���g���G���f�B�A���̃f�[�^����͂��Cfrom_little_endian = true  �Ƃ���ƁC�ϊ���̃f�[�^�̓r�b�O�G���f�B�A���ƂȂ�
//! - ����ȊO�̏ꍇ�͉����ϊ�����Ȃ�
//! 
//! @param[in] bytes              �c �ϊ������f�[�^
//! @param[in] from_little_endian �c �ϊ��O�̃G���f�B�A���^�C�v
//! 
//! @return �ϊ���̃f�[�^
//! 
template < class T >
inline byte_array< T > to_current_endian( const byte_array< T > &bytes, bool from_little_endian )
{
	static bool current_endian = _is_little_endian_( );
	if( current_endian != from_little_endian )
	{
		byte_array< T > tmp( bytes );
		swap_bytes( tmp );
		return( tmp );
	}
	else
	{
		return( bytes );
	}
}


/// @brief byte_array���̃f�[�^�����݂̌v�Z�@�̃G���f�B�A������ړI�̃G���f�B�A���ɍ��킹��
//! 
//! - ���g���G���f�B�A���̌v�Z�@�ŁCto_little_endian = false �Ƃ���ƁC�ϊ���̃f�[�^�̓r�b�O�G���f�B�A���ƂȂ�
//! - �r�b�O�G���f�B�A���̌v�Z�@�ŁCto_little_endian = true  �Ƃ���ƁC�ϊ���̃f�[�^�̓��g���G���f�B�A���ƂȂ�
//! - ����ȊO�̏ꍇ�́C���ꂼ��̌v�Z�@�̃G���f�B�A���̂܂܂ŉ����ϊ�����Ȃ�
//! 
//! @param[in] bytes            �c �ϊ������f�[�^
//! @param[in] to_little_endian �c �ϊ���̃G���f�B�A���^�C�v
//! 
//! @return �ϊ���̃f�[�^
//! 
template < class T >
inline byte_array< T > from_current_endian( const byte_array< T > &bytes, bool to_little_endian )
{
	static bool current_endian = _is_little_endian_( );
	if( current_endian != to_little_endian )
	{
		byte_array< T > tmp( bytes );
		swap_bytes( tmp );
		return( tmp );
	}
	else
	{
		return( bytes );
	}
}



/// @}
//  �G���f�B�A���Ɋւ�����̎擾�y�ѕϊ��O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_ENDIAN__
