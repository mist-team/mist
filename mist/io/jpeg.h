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

/// @file mist/io/jpeg.h
//!
//! @brief JPEG�摜��ǂݏ������邽�߂̃��C�u����
//!
//! �{���C�u�����́Chttp://www.libpng.org/pub/png/ �ŊJ�����s���Ă��� libjpeg �𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͖{���C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!
#ifndef __INCLUDE_MIST_JPEG__
#define __INCLUDE_MIST_JPEG__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#include <iostream>
#include <string>


#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0

	#define XMD_H
	#define HAVE_INT32			// JPEG�pINT32�^�������Ă���錾
	#define HAVE_BOOLEAN		// JPEG�pboolean�^�������Ă���錾

#endif

extern "C"
{
#include <jpeglib.h>
}


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __jpeg_controller__
{
	template < class T, class Allocator >
	struct jpeg_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			FILE *fin;				// �ǂݏ����p�t�@�C���|�C���^�[
			fin = fopen( filename.c_str( ), "rb" );
			if( fin == NULL ) return( false );

			JDIMENSION i, j;
			JSAMPROW bitmap[1];				// �r�b�g�}�b�v�f�[�^�z��ւ̃|�C���^�[
			jpeg_decompress_struct dinfo;	// JPEG�𓀏��\����
			jpeg_error_mgr jerr;			// JPEG�G���[�����p�\����
			int scanlen;					// �r�b�g�}�b�v�P�s�̃o�C�g��

			dinfo.err = jpeg_std_error( &jerr );
			jpeg_create_decompress( &dinfo );

			//jpeg_stdio_src(&dinfo, fin, 0);
			jpeg_stdio_src( &dinfo, fin );

			int n = jpeg_read_header( &dinfo, true );
			if( n < 1 )
			{
				jpeg_destroy_decompress( &dinfo );
				return(false);
			}

			jpeg_start_decompress( &dinfo );
			scanlen = dinfo.output_width * dinfo.output_components;

			image.resize( dinfo.output_width, dinfo.output_height );

			JSAMPLE *buffer = new JSAMPLE[ scanlen ];
			for( j = 0 ; j < dinfo.output_height ; j++ )
			{
				bitmap[0] = &buffer[0];
				if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
				for( i = 0 ; i < dinfo.output_width ; i++ )
				{
					switch( dinfo.output_components )
					{
					case 1:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i ], buffer[ i ], buffer[ i ] );
						break;

					case 3:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i * 3 + 0 ], buffer[ i * 3 + 1 ], buffer[ i * 3 + 2 ] );
						break;
						
					default:
						break;
					}
				}
			}

			jpeg_finish_decompress( &dinfo );

			fclose( fin );
			delete [] buffer;

			jpeg_destroy_decompress( &dinfo );

			return( true );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, int quality )
		{
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
			
			FILE *fout;						// �ǂݏ����p�t�@�C���|�C���^�[
			fout = fopen( filename.c_str( ), "wb" );
			if( fout == NULL ) return( false );

			JDIMENSION i, j;
			JDIMENSION w = static_cast< JDIMENSION >( image.width( ) ), h = static_cast< JDIMENSION >( image.height( ) );
			JSAMPROW bitmap[1];				/* pointer to JSAMPLE row[s] */
			jpeg_compress_struct cinfo;		// JPEG�𓀏��\����
			jpeg_error_mgr jerr;			// JPEG�G���[�����p�\����
			//int linelen;					// �r�b�g�}�b�v�P�s�̐����o�C�g��
			int scanlen = w * 3;			// �r�b�g�}�b�v�P�s�̃o�C�g��

			cinfo.err = jpeg_std_error( &jerr );
			jpeg_create_compress( &cinfo );
			//jpeg_stdio_dest(&cinfo, fout, 0);
			jpeg_stdio_dest( &cinfo, fout );
			cinfo.image_width = w;
			cinfo.image_height = h;
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;
			jpeg_set_defaults( &cinfo );
			jpeg_set_quality( &cinfo, quality, true );
			jpeg_start_compress( &cinfo, true );

			JSAMPLE *buffer = new JSAMPLE[ w * h * 3 ];
			JSAMPLE *p = buffer;

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
					*p++ = static_cast< JSAMPLE >( c.r );
					*p++ = static_cast< JSAMPLE >( c.g );
					*p++ = static_cast< JSAMPLE >( c.b );
				}
			}

			while( cinfo.next_scanline < cinfo.image_height )
			{
				bitmap[0] = &buffer[ cinfo.next_scanline * scanlen ];
				jpeg_write_scanlines( &cinfo, bitmap, 1 );
			}

			jpeg_finish_compress( &cinfo );

			fclose( fout );
			delete [] buffer;

			jpeg_destroy_compress( &cinfo );

			return( true );
		}
	};
}


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_jpeg_group JPEG �摜���o��
//!
//! JPEG�t�@�C����ǂݍ���ŁCMIST�R���e�i�֊i�[����D
//! �܂��CMIST�R���e�i����JPEG�t�@�C���ւ̏����o�����T�|�[�g�D
//! �ʓr Independent JPEG Group (http://www.ijg.org/) ���񋟂���t���[��JPEG���C�u�������K�v�ł��D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/jpeg.h>
//! @endcode
//!
//!  @{


/// @brief JPEG�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! �S�Ă�JPEG�摜�`���i���X���X��JPEG�������j��ǂݍ��ނ��Ƃ��\�ł�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_jpeg( array2< T, Allocator > &image, const std::string &filename )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::read( image, filename ) );
}


/// @brief JPEG�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! �S�Ă�JPEG�摜�`���i���X���X��JPEG�������j��ǂݍ��ނ��Ƃ��\�ł�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_jpeg( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_jpeg( image, wstr2str( filename ) ) );
}


/// @brief MIST�R���e�i�̉摜��JPEG�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention JPEG�̕i����0����100�܂ł���C���l���傫���قǈ��k���͒Ⴍ�Ȃ�D����������ƁC���k�����オ�邩���ɉ掿�������Ȃ�̂Œ��ӁD�i�f�t�H���g�l��100�j
//! 
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] quality  �c �o�͂���JPEG�摜�̕i��
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_jpeg( const array2< T, Allocator > &image, const std::string &filename, int quality = 100 )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::write( image, filename, quality ) );
}


/// @brief MIST�R���e�i�̉摜��JPEG�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention JPEG�̕i����0����100�܂ł���C���l���傫���قǈ��k���͒Ⴍ�Ȃ�D����������ƁC���k�����オ�邩���ɉ掿�������Ȃ�̂Œ��ӁD�i�f�t�H���g�l��100�j
//! 
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] quality  �c �o�͂���JPEG�摜�̕i��
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_jpeg( const array2< T, Allocator > &image, const std::wstring &filename, int quality = 100 )
{
	return( write_jpeg( image, wstr2str( filename ), quality ) );
}


/// @}
//  JPEG �摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_JPEG__
