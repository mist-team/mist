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

/// @file mist/io/lines.h
//!
//! @brief �t�@�C��������s�ŋ�؂�ꂽ������̃��X�g��ǂݍ��ނ��߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_LINES__
#define __INCLUDE_MIST_LINES__


#ifndef __INCLUDE_MIST_CONF_H__
#include "../config/mist_conf.h"
#endif

#include <iostream>
#include <string>
#include <vector>

#include <zlib.h>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __lines_controller__
{
	struct lines_controller
	{
		static const unsigned char *get_line( const unsigned char *s, const unsigned char *e, std::string &line, bool &__has_eol__ )
		{
			__has_eol__ = false;
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						__has_eol__ = true;
						s = s + 2;
					}
					else
					{
						s++;
					}
					break;
				}
				else if( s[ 0 ] == '\n' )
				{
					__has_eol__ = true;
					s = s + 1;
					break;
				}

				line += *s;
				s++;
			}

			return( s > e ? e : s );
		}

		template < template < typename T, typename A > class Array, class Allocator >
		static bool read( Array< std::string, Allocator > &lines, const std::string &filename )
		{
			gzFile fp;
			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				return( false );
			}

			std::string line = "";
			size_t numBytes = 4096;
			unsigned char *buff = new unsigned char[ numBytes ];

			while( gzeof( fp ) == 0 )
			{
				ptrdiff_t read_size = gzread( fp, ( void * )buff, static_cast< unsigned int >( sizeof( unsigned char ) * numBytes ) );

				const unsigned char *sp = buff;
				const unsigned char *ep = sp + read_size;

				while( sp < ep )
				{
					bool __has_eol__ = false;
					sp = get_line( sp, ep, line, __has_eol__ );

					if( !line.empty( ) && __has_eol__ )
					{
						lines.push_back( line );
						line.resize( 0 );
					}
				}
			}

			if( !line.empty( ) )
			{
				lines.push_back( line );
			}

			gzclose( fp );

			delete [] buff;

			return( true );
		}
	};
}


//! @addtogroup lines_data_group ���s��؂�̃f�[�^�̓��o��
//!
//! ���s��؂�̃t�@�C����ǂݍ���ŁCMIST�R���e�i��������STL�̃R���e�i�֊i�[����D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/lines.h>
//! @endcode
//!
//!
//!  @{


/// @brief ���s��؂�̃t�@�C����STL�R���e�i�ɓǂݍ���
//! 
//! �f�[�^�̋�؂�Ƃ��āC�R���}�������͔��p�󔒂��f�t�H���g�Ŏ��ʂ���悤�ɂȂ��Ă���D
//! �f�[�^�̋�؂��ύX����ꍇ�́Cseparator ������ύX����D
//! 
//! @param[out] lines     �c ���s��؂�̃t�@�C����ǂݍ��ސ��STL�R���e�i
//! @param[in]  filename  �c ���̓t�@�C����
//!
//! @retval true  �c �f�[�^�̓ǂݍ��݂ɐ���
//! @retval false �c �f�[�^�̓ǂݍ��݂Ɏ��s
//! 
template < template < typename T, typename A > class Array, class Allocator >
bool read_lines( Array< std::string, Allocator > &lines, const std::string &filename )
{
	// �f�[�^���N���A����
	lines.clear( );
	return( __lines_controller__::lines_controller::read( lines, filename ) );
}


/// @brief ���s��؂�̃t�@�C����STL�R���e�i�ɓǂݍ���
//! 
//! �f�[�^�̋�؂�Ƃ��āC�R���}�������͔��p�󔒂��f�t�H���g�Ŏ��ʂ���悤�ɂȂ��Ă���D
//! �f�[�^�̋�؂��ύX����ꍇ�́Cseparator ������ύX����D
//! 
//! @param[out] lines     �c ���s��؂�̃t�@�C����ǂݍ��ސ��STL�R���e�i
//! @param[in]  filename  �c ���̓t�@�C����
//!
//! @retval true  �c �f�[�^�̓ǂݍ��݂ɐ���
//! @retval false �c �f�[�^�̓ǂݍ��݂Ɏ��s
//! 
template < template < typename T, typename A > class Array, class Allocator >
bool read_lines( Array< std::string, Allocator > &lines, const std::wstring &filename )
{
	return( read_lines( lines, wstr2str( filename ) ) );
}

/// @}
//  ���s��؂�̃f�[�^�̓��o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_LINES__
