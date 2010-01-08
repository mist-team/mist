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

/// @file mist/io/image.h
//!
//! @brief BMP�CPNG�CPNM�CTIFF�CJPEG�CGIF �̉摜��ǂݏ������邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_IMAGE__
#define __INCLUDE_MIST_IMAGE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_BMP__
#include "bmp.h"
#endif

#ifndef __INCLUDE_MIST_PNG__
#include "png.h"
#endif

#ifndef __INCLUDE_MIST_GIF__
#include "gif.h"
#endif

#ifndef __INCLUDE_MIST_TIFF__
#include "tiff.h"
#endif

#ifndef __INCLUDE_MIST_JPEG__
#include "jpeg.h"
#endif

#ifndef __INCLUDE_MIST_PNM__
#include "pnm.h"
#endif

#ifndef __INCLUDE_MIST_TGA__
#include "tga.h"
#endif

#include <iostream>
#include <string>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __image_controller__
{
	inline std::string to_lower_case( const std::string &str )
	{
		std::string s = "";
		for( std::string::size_type i = 0 ; i < str.size( ) ; i++ )
		{
			s += static_cast< char >( tolower( str[ i ] ) );
		}
		return( s );
	}

	inline std::string get_ext( const std::string &str )
	{
		std::string::size_type index = str.rfind( '.' );
		if( index == str.npos )
		{
			return( "" );
		}
		return( str.substr( index ) );
	}
}


//! @addtogroup image_group �摜���o��
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/image.h>
//! @endcode
//!
//!  @{



/// @brief JPEG�CTIFF�CPNG�CBMP�CPNM�CGIF �摜���g���q�Ɋ�Â���MIST�R���e�i�ɓǂݍ���
//! 
//! @attention �g���q�Ɋ�Â��ĉ摜�`���𔻕ʂ��邽�߁C�������t�@�C���g���q���t���Ă���K�v����
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
inline bool read_image( mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );
	if( ext == "" )
	{
		return( false );
	}

	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::read_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::read_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::read_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::read_png( image, filename );
	}
	else if( ext == ".pbm" || ext == ".pgm" || ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::read_pnm( image, filename );
	}
	else if( ext == ".tga" )
	{
		ret = mist::read_tga( image, filename );
	}
	else if( ext == ".gif" )
	{
		ret = mist::read_gif( image, filename );
	}
	else
	{
		ret = false;
	}
	return( ret );
}

/// @brief JPEG�CTIFF�CPNG�CBMP�CPNM�CGIF �摜���g���q�Ɋ�Â���MIST�R���e�i�ɓǂݍ���
//! 
//! @attention �g���q�Ɋ�Â��ĉ摜�`���𔻕ʂ��邽�߁C�������t�@�C���g���q���t���Ă���K�v����
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
inline bool read_image( mist::array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_image( image, wstr2str( filename ) ) );
}

/// @brief MIST�R���e�i�̉摜��TIFF�`����JPEG�CTIFF�CPNG�CBMP�CPNM�CGIF �摜�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention �g���q�Ɋ�Â��ĉ摜�`���𔻕ʂ��邽�߁C�������t�@�C���g���q���t���Ă���K�v����
//! @attention �o�͂����t�@�C���̈��k���́C�e read_jpeg ���̃f�t�H���g�l���p������
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
inline bool write_image( mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );
	if( ext == "" )
	{
		std::cerr << "Unknown file type!" << std::endl;
		return( false );
	}

	if( image.width( ) == 0 )
	{
		std::cerr << "Image width is zero!" << std::endl;
		return( false );
	}
	else if( image.height( ) == 0 )
	{
		std::cerr << "Image height is zero!" << std::endl;
		return( false );
	}

	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::write_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::write_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::write_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::write_png( image, filename );
	}
	else if( ext == ".pbm" )
	{
		ret = mist::write_pnm( image, filename, 1 );
	}
	else if( ext == ".pgm" )
	{
		ret = mist::write_pnm( image, filename, 2 );
	}
	else if( ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::write_pnm( image, filename, 3 );
	}
	else if( ext == ".tga" )
	{
		ret = mist::write_tga( image, filename );
	}
	else
	{
		ret = false;
	}
	return( ret );
}

/// @brief MIST�R���e�i�̉摜��TIFF�`����JPEG�CTIFF�CPNG�CBMP�CPNM�CGIF �摜�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention �g���q�Ɋ�Â��ĉ摜�`���𔻕ʂ��邽�߁C�������t�@�C���g���q���t���Ă���K�v����
//! @attention �o�͂����t�@�C���̈��k���́C�e read_jpeg ���̃f�t�H���g�l���p������
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
inline bool write_image( mist::array2< T, Allocator > &image, const std::wstring &filename )
{
	return( write_image( image, wstr2str( filename ) ) );
}

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_IMAGE__
