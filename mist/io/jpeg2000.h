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
//! @brief JPEG2000�摜��ǂݏ������邽�߂̃��C�u����
//!
//! �{���C�u�����́Chttp://www.openjpeg.org/ �ŊJ�����s���Ă��� OpenJPEG ���C�u�����𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͖{���C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!
#ifndef __INCLUDE_MIST_JPEG2000__
#define __INCLUDE_MIST_JPEG2000__


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

extern "C"
{
#ifndef OPJ_STATIC
	#define OPJ_STATIC
#endif

#include <openjpeg.h>
}


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __jpeg2000_controller__
{
	template < bool b >
	struct _create_opj_image_
	{
		// �O���[�X�P�[���̉摜���쐬����
		template < class T, class Allocator >
		static opj_image_t *create( const array2< T, Allocator > &image, const opj_cparameters_t &param )
		{
			opj_image_cmptparm_t cmptparm[ 1 ];

			memset( cmptparm, 0, 1 * sizeof( opj_image_cmptparm_t ) );

			// �e�F�v���[���̐ݒ���s��
			for( size_t i = 0 ; i < 1 ; i++ )
			{
				cmptparm[ i ].prec = 8;
				cmptparm[ i ].bpp  = 8;
				cmptparm[ i ].sgnd = 0;
				cmptparm[ i ].dx   = param.subsampling_dx;
				cmptparm[ i ].dy   = param.subsampling_dy;
				cmptparm[ i ].w    = static_cast< int >( image.width( ) );
				cmptparm[ i ].h    = static_cast< int >( image.height( ) );
			}

			// �J���[�摜�Ƃ��č쐬����
			opj_image_t *img = opj_image_create( 1, cmptparm, CLRSPC_GRAY );

			// �摜�̃I�t�Z�b�g�ƃ����_�����O�O���b�h��ݒ肷��
			img->x0 = param.image_offset_x0;
			img->y0 = param.image_offset_y0;
			img->x1 = img->x0 + ( static_cast< int >( image.width( ) )  - 1 ) * param.subsampling_dx + 1;
			img->y1 = img->y0 + ( static_cast< int >( image.height( ) ) - 1 ) * param.subsampling_dy + 1;

			// �摜�f�[�^���R�s�[����
			for( size_t i = 0 ; i < image.size( ) ; i++ )
			{
				img->comps[ 0 ].data[ i ] = static_cast< int >( image[ i ] );
			}

			return( img );
		}
	};

	template < >
	struct _create_opj_image_< true >
	{
		// �J���[�摜���쐬����
		template < class T, class Allocator >
		static opj_image_t *create( const array2< T, Allocator > &image, const opj_cparameters_t &param )
		{
			opj_image_cmptparm_t cmptparm[ 3 ];

			memset( cmptparm, 0, 3 * sizeof( opj_image_cmptparm_t ) );

			// �e�F�v���[���̐ݒ���s��
			for( size_t i = 0 ; i < 3 ; i++ )
			{
				cmptparm[ i ].prec = 8;
				cmptparm[ i ].bpp  = 8;
				cmptparm[ i ].sgnd = 0;
				cmptparm[ i ].dx   = param.subsampling_dx;
				cmptparm[ i ].dy   = param.subsampling_dy;
				cmptparm[ i ].w    = static_cast< int >( image.width( ) );
				cmptparm[ i ].h    = static_cast< int >( image.height( ) );
			}

			// �J���[�摜�Ƃ��č쐬����
			opj_image_t *img = opj_image_create( 3, cmptparm, CLRSPC_SRGB );

			// �摜�̃I�t�Z�b�g�ƃ����_�����O�O���b�h��ݒ肷��
			img->x0 = param.image_offset_x0;
			img->y0 = param.image_offset_y0;
			img->x1 = img->x0 + ( static_cast< int >( image.width( ) )  - 1 ) * param.subsampling_dx + 1;
			img->y1 = img->y0 + ( static_cast< int >( image.height( ) ) - 1 ) * param.subsampling_dy + 1;

			// �摜�f�[�^���R�s�[����
			for( size_t i = 0 ; i < image.size( ) ; i++ )
			{
				img->comps[ 0 ].data[ i ] = static_cast< int >( image[ i ].r );
				img->comps[ 1 ].data[ i ] = static_cast< int >( image[ i ].g );
				img->comps[ 2 ].data[ i ] = static_cast< int >( image[ i ].b );
			}

			return( img );
		}
	};

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

	template < class T, class Allocator >
	struct jpeg2000_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			typedef typename array2< T, Allocator >::size_type       size_type;
			typedef typename array2< T, Allocator >::difference_type difference_type;

			// �t�@�C������f�[�^�����ׂēǂݍ���
			FILE *fp = fopen( filename.c_str( ), "rb" );
			if( fp == NULL )
			{
				std::cerr << "Can't open file " << filename << std::endl;
				return( false );
			}

			// �t�@�C���T�C�Y���擾
			fseek( fp, 0, SEEK_END );
			size_type filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			// �f�[�^���������ǂݍ���
			unsigned char *buff = new unsigned char[ filesize + 1 ];
			unsigned char *pointer = buff;
			size_type read_size = 0;
			while( feof( fp ) == 0 )
			{
				read_size = fread( pointer, sizeof( unsigned char ), 1024, fp );
				if( read_size < 1024 )
				{
					break;
				}
				pointer += read_size;
			}

			// �t�@�C�������
			fclose( fp );

			// �f�R�[�_�����肷��
			std::string ext = to_lower_case( get_ext( filename ) );
			OPJ_CODEC_FORMAT codec;
			if( ext == ".jp2" )
			{
				codec = CODEC_JP2;
			}
			else if( ext == ".j2k" || ext == ".j2c" )
			{
				codec = CODEC_J2K;
			}
			else if( ext == ".jpt" )
			{
				codec = CODEC_JPT;
			}

			// OpenJPEG���C�u�����̈��k�p�����[�^
			opj_dparameters_t param;

			// �f�R�[�h�p�����[�^���f�t�H���g�ɐݒ肷��
			opj_set_default_decoder_parameters( &param );

			// �f�R�[�_�G���W��������������
			//opj_dinfo_t *dinfo = opj_create_decompress( CODEC_UNKNOWN );
			opj_dinfo_t *dinfo = opj_create_decompress( codec );

			// �G���[�n���h�����O�̊֐���ݒ肷��
			opj_event_mgr_t event_mgr;
			memset( &event_mgr, 0, sizeof( opj_event_mgr_t ) );
			//event_mgr.error_handler   = error;
			//event_mgr.warning_handler = warning;
			//event_mgr.info_handler    = info;
			opj_set_event_mgr( (opj_common_ptr)dinfo, &event_mgr, stderr );

			// �f�R�[�_�Ƀp�����[�^��ݒ肷��
			opj_setup_decoder( dinfo, &param );

			// �f�R�[�h�p�̃������X�g���[������������
			opj_cio_t *cio = opj_cio_open( ( opj_common_ptr )dinfo, buff, static_cast< int >( filesize ) );

			// �摜���f�R�[�h����
			opj_image_t *img = opj_decode( dinfo, cio );			
			if( img == NULL )
			{
				std::cerr << "Failed to read JPEG2000 image from " << filename << std::endl;

				opj_destroy_decompress( dinfo );
				opj_cio_close( cio );

				delete [] buff;

				return( false );
			}
			else
			{
				delete [] buff;
			}

			if( img->numcomps != 1 && img->numcomps != 3 )
			{
				// �������ȉ摜���ǂݍ��܂ꂽ�H
				opj_destroy_decompress( dinfo );
				opj_cio_close( cio );

				std::cerr << "Invalid image is loaded." << std::endl;

				return( false );
			}

			// OpenJPEG ���C�u�����̉摜�`������MIST�̌`���ɕϊ�����
			image.resize( img->comps[ 0 ].w, img->comps[ 0 ].h );

			if( img->numcomps == 1 )
			{
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					int pix = img->comps[ 0 ].data[ i ];
					image[ i ] = pixel_converter::convert_to( pix, pix, pix );
				}
			}
			else // img->numcomps == 3
			{
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[ i ] = pixel_converter::convert_to( img->comps[ 0 ].data[ i ], img->comps[ 1 ].data[ i ], img->comps[ 2 ].data[ i ] );
				}
			}


			// OpenJPEG ���C�u�����Ŏg�p�������\�[�X�����J������
			opj_cio_close( cio );
			opj_destroy_decompress( dinfo );
			opj_image_destroy( img );

			return( true );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, double compression_level )
		{
			typedef typename array2< T, Allocator >::size_type       size_type;
			typedef typename array2< T, Allocator >::difference_type difference_type;

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

			// OpenJPEG���C�u�����̈��k�p�����[�^
			opj_cparameters_t param;

			// �G���R�[�h�p�����[�^���f�t�H���g�ɐݒ肷��
			opj_set_default_encoder_parameters( &param );

			// ���k����ݒ肷��
			// 0��Lossless���k
			param.tcp_rates[ 0 ] = static_cast< float >( compression_level );
			param.tcp_numlayers  = 1;
			param.cp_disto_alloc = 1;

			// �R�����g�𖄂ߍ���
			static char comment[] = "Created by MIST with OpenJPEG Library";
			param.cp_comment = comment;

			// OpenJPEG���C�u���������Ŏg�p����摜�`���ɕϊ�����
			opj_image_t *img = _create_opj_image_< is_color< T >::value >::create( image, param );

			// �F�̃R���|�[�l���g�̐��𒲂ׂ�
			param.tcp_mct = img->numcomps == 3 ? 1 : 0;

			// �G���[�n���h�����O�̊֐���ݒ肷��
			opj_event_mgr_t event_mgr;
			memset( &event_mgr, 0, sizeof( opj_event_mgr_t ) );
			//event_mgr.error_handler   = error;
			//event_mgr.warning_handler = warning;
			//event_mgr.info_handler    = info;

			// JPEG2000�̈��k�G���W��������������
			opj_cinfo_t * cinfo = opj_create_compress( CODEC_JP2 );

			// �G���[�����p�̊֐���ݒ肷��
			opj_set_event_mgr( ( opj_common_ptr )cinfo, &event_mgr, stderr );

			// JPEG2000 �̈��k�G���W���Ƀp�����[�^�Ɖ摜��ݒ肷��
			opj_setup_encoder( cinfo, &param, img );

			// �摜����������ɓW�J���邽�߂̃I�u�W�F�N�g������������
			opj_cio_t *cio = opj_cio_open( ( opj_common_ptr )cinfo, NULL, 0 );

			// JPEG2000�ŉ摜�����k����
			if( !opj_encode( cinfo, cio, img, NULL ) )
			{
				opj_cio_close( cio );
				opj_destroy_compress( cinfo );
				opj_image_destroy( img );

				// ���k�Ɏ��s
				return( false );
			}

			// ���k��̃f�[�^�����擾����
			difference_type codestream_length = cio_tell( cio );

			// �摜�o�͗p�̃t�@�C���|�C���^���쐬����
			FILE *fp = fopen( filename.c_str( ), "wb" );

			if( fp == NULL )
			{
				// �摜���쐬���邱�Ƃ��ł��Ȃ������E�E�E
				opj_cio_close( cio );
				opj_destroy_compress( cinfo );
				opj_image_destroy( img );
				std::cerr << "Failed to write image to " << filename << std::endl;
				return( false );
			}

			// �摜�f�[�^���t�@�C���ɏ����������o��
			unsigned char *pointer = cio->buffer;
			while( codestream_length > 0 )
			{
				size_type write_size = fwrite( pointer, sizeof( unsigned char ), codestream_length > 1024 ? 1024 : codestream_length, fp );
				pointer           += write_size;
				codestream_length -= write_size;
			}

			// �t�@�C�������
			fclose( fp );

			// OpenJPEG ���C�u�����Ŏg�p�������\�[�X�����J������
			opj_cio_close( cio );
			opj_destroy_compress( cinfo );
			opj_image_destroy( img );

			return( true );
		}
	};
}


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_jpeg2000_group JPEG2000 �摜���o��
//!
//! JPEG2000�t�@�C����ǂݍ���ŁCMIST�R���e�i�֊i�[����D
//! �܂��CMIST�R���e�i����JPEG2000�t�@�C���ւ̏����o�����T�|�[�g�D
//! �ʓr OpenJPEG (http://www.openjpeg.org/) ���񋟂���t���[�̃��C�u�������K�v�ł��D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/jpeg2000.h>
//! @endcode
//!
//!  @{


/// @brief JPEG2000�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! �S�Ă�JPEG2000�摜�`����ǂݍ��ނ��Ƃ��\�ł�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_jpeg2000( array2< T, Allocator > &image, const std::string &filename )
{
	return( __jpeg2000_controller__::jpeg2000_controller< T, Allocator >::read( image, filename ) );
}


/// @brief JPEG2000�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! �S�Ă�JPEG2000�摜�`����ǂݍ��ނ��Ƃ��\�ł�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_jpeg2000( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_jpeg2000( image, wstr2str( filename ) ) );
}


/// @brief MIST�R���e�i�̉摜��JPEG2000�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention JPEG2000�̈��k���͐��l���傫���قǈ��k���������Ȃ�D0���w�肷���Lossless���k���s���D�i�f�t�H���g�l��0�j
//! 
//! @param[in] image             �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename          �c �o�̓t�@�C����
//! @param[in] compression_level �c �摜�̈��k���i�Ⴆ��2���w�肷��Ɖ摜�T�C�Y��2����1�ɂ���D�܂��C0���w�肷���Lossless���k���s���j
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_jpeg2000( const array2< T, Allocator > &image, const std::string &filename, double compression_level = 0 )
{
	return( __jpeg2000_controller__::jpeg2000_controller< T, Allocator >::write( image, filename, compression_level ) );
}


/// @brief MIST�R���e�i�̉摜��JPEG2000�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention JPEG2000�̈��k���͐��l���傫���قǈ��k���������Ȃ�D0���w�肷���Lossless���k���s���D�i�f�t�H���g�l��0�j
//! 
//! @param[in] image             �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename          �c �o�̓t�@�C����
//! @param[in] compression_level �c �摜�̈��k���i�Ⴆ��2���w�肷��Ɖ摜�T�C�Y��2����1�ɂ���D�܂��C0���w�肷���Lossless���k���s���j
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_jpeg2000( const array2< T, Allocator > &image, const std::wstring &filename, double compression_level = 0 )
{
	return( write_jpeg2000( image, wstr2str( filename ), compression_level ) );
}


/// @}
//  JPEG2000 �摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_JPEG2000__
