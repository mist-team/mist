/// @file mist/io/image.h
//!
//! @brief BMP�CPNG�CPNM�CTIFF�CJPEG�̉摜��ǂݏ������邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_IMAGE__
#define __INCLUDE_MIST_IMAGE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_JPEG__
#include "jpeg.h"
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


/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_IMAGE__
