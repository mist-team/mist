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

/// @file mist/hash_algorithm.h
//!
//! @brief �C�ӂ̃o�C�g��̃n�b�V���֐��l���v�Z����
//!
//! - �Q�l����
//!   - MD2, RFC1319, http://www.ietf.org/rfc/rfc1319.txt
//!   - MD4, RFC1320, http://www.ietf.org/rfc/rfc1320.txt
//!   - MD5, RFC1321, http://www.ietf.org/rfc/rfc1321.txt
//!   - SHA-1, RFC3174, http://www.ietf.org/rfc/rfc3174.txt
//!   - SHA-256, 384, 512, "Descriptions of SHA-256, SHA-384, and SHA-512." http://csrc.nist.gov/cryptval/shs/sha256-384-512.pdf 
//!
#ifndef __INCLUDE_HASH_ALGORITHM__
#define __INCLUDE_HASH_ALGORITHM__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <string>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @defgroup hash_group �n�b�V���֐�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/hash_algorithm.h>
//! @endcode
//!
//!  @{


/// @brief �n�b�V���֐����v�Z������N���X
//!
//! MD2�CMD4�CMD5�CSHA1�CSHA-256�CSHA-384�CSHA-512 ���v�Z���邽�߂̊��N���X
//!
//! @attention SHA-384�CSHA-512 ���v�Z����ۂ� 64 �r�b�g�����𗘗p���邽�߁C�R���p�C���� 64 �r�b�g���T�|�[�g����K�v����D
//!
//! @code �n�b�V���֐��̌v�Z��
//! // �n�b�V���֐��� SHA1 �ŏ���������
//! mist::hash_algorithm &h = mist::sha1( "a" );
//! 
//! // �n�b�V���֐��l�̃_�C�W�F�X�g��������o�͂���
//! std::cout << h << std::endl;
//! 
//! // �ʂ̕�����ōČv�Z����
//! h.compute_hash( "����������" );
//! 
//! // �n�b�V���֐��l�̃_�C�W�F�X�g��������o�͂���
//! std::cout << h << std::endl;
//! 
//! @endcode
//!
class hash_algorithm
{
public:
	typedef size_t					size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t				difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef unsigned char			uint8;				///< @brief �����Ȃ�8�r�b�g������\���^�D�����̃n�b�V���֐��l���v�Z����̂ɗ��p�D�����I�ɂ� unsigned char �^�Ɠ����D
	typedef unsigned int			uint32;				///< @brief �����Ȃ�32�r�b�g������\���^�D�����̃n�b�V���֐��l���v�Z����̂ɗ��p�D�����I�ɂ� unsigned int �^�Ɠ����D
	typedef unsigned long long int	uint64;				///< @brief �����Ȃ�64�r�b�g������\���^�D�����̃n�b�V���֐��l���v�Z����̂ɗ��p�D�����I�ɂ� unsigned long long int �^�Ɠ����D

protected:
	size_type nbytes;
	unsigned char *digest;

public:
	/// @brief data[ 0 ] ���� data[ len - 1 ] �� len �o�C�g�̃n�b�V���֐��l���v�Z����D
	virtual void compute_hash( const void *bytes, uint64 len ) = 0;

	/// @brief ������ str �̃n�b�V���֐��l���v�Z����D
	void compute_hash( const std::string &str ){ compute_hash( str.c_str( ), str.length( ) ); }


	/// @brief �n�b�V���֐�����Ԃ�
	virtual const std::string name( ) const = 0;

	/// @brief �_�C�W�F�X�g�o�C�g��̒���
	size_type size( ) const { return( nbytes ); }

	/// @brief �_�C�W�F�X�g������̊e�o�C�g���擾����
	unsigned char operator []( size_type index ) const { return( digest[ index ] ); }


	/// @brief �_�C�W�F�X�g�������Ԃ�
	std::string to_string( ) const
	{
		static char x16[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		std::string str;

		for( size_type i = 0 ; i < size( ) ; i++ )
		{
			str += x16[ ( digest[ i ] >> 4 ) & 0x0f ];
			str += x16[ digest[ i ] & 0x0f ];
		}

		return( str );
	}

protected:
	// �n�b�V���֐��̃R�s�[�͋֎~�I�I
	hash_algorithm &operator =( const hash_algorithm &h );
	hash_algorithm( const hash_algorithm &h );

protected:
	/// @brief �w�肳�ꂽ�_�C�W�F�X�g�̃o�C�g���ŏ���������i�h���N���X�ł̂ݗ��p�\�j
	hash_algorithm( size_type num_digest_bytes ) : nbytes( num_digest_bytes ), digest( new unsigned char[ nbytes ] ){ }

	/// @brief �w�肳�ꂽ�_�C�W�F�X�g������ŏ���������i�h���N���X�ł̂ݗ��p�\�j
	hash_algorithm( const std::string &str ) : nbytes( str.length( ) / 2 ), digest( new unsigned char[ nbytes ] )
	{
		static uint8 x16[ 256 ] = {	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		for( size_type i = 0, indx = 0 ; i < size( ) ; i++, indx += 2 )
		{
			size_type s1 = static_cast< uint8 >( str[ indx ] );
			size_type s2 = static_cast< uint8 >( str[ indx + 1 ] );
			digest[ i ] = ( ( x16[ s1 ] << 4 ) & 0xf0 ) | ( x16[ s2 ] & 0x0f );
		}
	}

	/// @brief �_�C�W�F�X�g�ɗ��p�����f�[�^���폜����
	virtual ~hash_algorithm( )
	{
		delete [] digest;
	}


public:
	/// @brief 2�̃n�b�V���֐��l�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const hash_algorithm &h ) const
	{
		if( size( ) != h.size( ) )
		{
			return( false );
		}
		else
		{
			for( size_type i = 0 ; i < size( ) ; i++ )
			{
				if( digest[ i ] != h[ i ] )
				{
					return( false );
				}
			}
			return( true );
		}
	}

	/// @brief 2�̃_�C�W�F�X�g�����񂪓��ꂩ�ǂ����𔻒肷��
	bool operator ==( const std::string &str ) const
	{
		return( str == to_string( ) );
	}
};


/// @brief �w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     h   �c �n�b�V���֐��I�u�W�F�N�g
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
inline std::ostream &operator <<( std::ostream &out, const hash_algorithm &h )
{
	out << h.to_string( );
	return( out );
}


/// @}
//  �n�b�V���֐��O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#ifndef __INCLUDE_MD5__
#include "md5.h"
#endif

#ifndef __INCLUDE_SHA__
#include "sha.h"
#endif


#endif // __INCLUDE_HASH_ALGORITHM__
