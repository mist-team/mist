/// @file mist/io/tga.h
//!
//! @brief �r�b�g�}�b�v�摜��ǂݏ������邽�߂̃��C�u����
//!

#ifndef __INCLUDE_MIST_TGA__
#define __INCLUDE_MIST_TGA__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "../config/endian.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#include <iostream>
#include <string>

#include <deque>
#include <map>
#include <algorithm>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __tga_controller__
{
	// �\���̓��̃A���C�����g��1�o�C�g�ɐݒ肵�C�p�f�B���O���֎~����
#if defined( __MIST_MSVC__ )
	#pragma pack( push, tga_align, 1 )
#endif
		struct _tga_header_
		{
			enum{ bytes = 18 };

			// ID Length Field 1 ( 1 byte )
			unsigned char	id_length;

			// Color Map Type Field 2 ( 1 byte )
			unsigned char	color_map_type;

			// Image Type Field 3 ( 1 byte )
			unsigned char	image_type;

			// Color Map Specification Field 4 ( 5 bytes )
			unsigned short	first_entry_index;
			unsigned short	color_map_length;
			unsigned char	color_map_entry_size;

			// Image Specification Field 4 ( 10 bytes )
			unsigned short	x_origin;
			unsigned short	y_origin;
			unsigned short	image_width;
			unsigned short	image_height;
			unsigned char	pixel_depth;
			unsigned char	image_descriptor;

		} _MIST_PACKED;

#if defined( __MIST_MSVC__ )
	#pragma pack( pop, tga_align )
#endif
	// �\���̓��̃A���C�����g��1�o�C�g�ɐݒ肵�C�p�f�B���O���֎~���� �` �����܂� �`


	template < class T, class Allocator >
	struct tga_controller
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		typedef typename array2< T, Allocator >::difference_type difference_type;
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool is_supported( size_type tga_bits )
		{
			bool ret = false;
			switch( tga_bits )
			{
			case 16:
			case 24:
			case 32:
				ret = true;
				break;

			default:
				break;
			}
			return( ret );
		}

		static unsigned char *decode_RLE( const unsigned char *pixel, size_type num_bytes, size_type pixel_bytes )
		{
			unsigned char *buff = new unsigned char[ num_bytes ];
			memset( buff, 0, sizeof( unsigned char ) * num_bytes );

			for( size_type i = 0, j = 0 ; j < num_bytes ; )
			{
				unsigned char byte = pixel[ i++ ];
				if( ( byte & 0x80 ) != 0 )
				{
					// ��ʃr�b�g���P�̂Ƃ������B���ɑ����f�[�^�o�C�g�i�s�N�Z���̃o�C�g����P�ʂƂ��āj���J��Ԃ��B
					// �J��Ԃ��񐔂́Abyte & 0x7F + 1
					// ��F 83 0A -> 0A 0A 0A 0A

					size_type num = ( byte & 0x7f ) + 1;
					for( size_type l = 0 ; l < num ; l++ )
					{
						for( size_type m = 0 ; m < pixel_bytes ; m++ )
						{
							buff[ j + l * pixel_bytes + m ] = pixel[ i + m ];
						}
					}

					i += pixel_bytes;
					j += num * pixel_bytes;
				}
				else
				{
					// ��ʃr�b�g��0�̂Ƃ��A���e�����O���[�v(Literal group)�������B����o�C�g�̌��
					// byte+1 �̃f�[�^�i�s�N�Z���̃o�C�g����P�ʂƂ��āj���R�s�[����B
					// ��F 04 0A 0B 0C 0D 0E ->  0A 0B 0C 0D 0E
					size_type num = byte + 1;
					memcpy( buff + j, pixel + i, sizeof( unsigned char ) * num * pixel_bytes );

					i += num * pixel_bytes;
					j += num * pixel_bytes;
				}
			}

			return( buff );
		}


		static bool convert_from_tga_data( unsigned char *tga, size_type num_bytes, array2< T, Allocator > &image )
		{
			// TGA�p�̃w�b�_�̈ʒu���w�肷��
			_tga_header_	*pheader		= reinterpret_cast < _tga_header_ * >( tga );
			_tga_header_	&header			= *pheader;

			difference_type width  = header.image_width;
			difference_type height = header.image_height;
			difference_type pixel_bytes = header.pixel_depth / 8;

			unsigned char	*color_map_data	= tga + _tga_header_::bytes;
			unsigned char	*image_data		= color_map_data + ( header.color_map_type == 0 ? 0 : header.color_map_length * header.color_map_entry_size / 8 );

//			unsigned char	*pfooter		= image_data + width * height * pixel_bytes;

			bool is_RLE		= ( header.image_type & 0x08 ) != 0;
			bool is_Huffman	= ( header.image_type & 0x20 ) != 0;
			bool from_top	= ( header.image_descriptor & 0x20 ) != 0;
			bool from_left	= ( header.image_descriptor & 0x10 ) == 0;

			if( is_Huffman || header.image_type > 11 )
			{
				// ���T�|�[�g
				return( false );
			}

			unsigned char *buff = NULL, *pixels = NULL;

			if( is_RLE )
			{
				buff = decode_RLE( image_data, width * height * pixel_bytes, pixel_bytes );
				pixels = buff + ( from_top ? 0 : width * ( height - 1 ) * pixel_bytes );
			}
			else
			{
				pixels = image_data + ( from_top ? 0 : width * ( height - 1 ) * pixel_bytes );
			}

			difference_type line_skip  = from_top  ? width * pixel_bytes : - width * pixel_bytes;
			difference_type pixel_skip = from_left ? pixel_bytes : - pixel_bytes;

			image.resize( width, height );

			bool ret = true;

			switch( header.image_type & 0x07 )
			{
			case 1:	// Color-mapped Image
				for( difference_type j = 0 ; j < height ; j++ )
				{
					unsigned char *pixel = pixels + line_skip * j + ( from_left ? 0 : pixel_bytes ) * ( width - 1 );

					for( difference_type i = 0 ; i < width ; i++ )
					{
						difference_type tmp = 0;
						unsigned char *pix = pixel + pixel_skip * i;

						for( difference_type k = pixel_bytes - 1 ; k >= 0 ; k-- )
						{
							tmp = ( tmp << 8 ) + pix[ k ];
						}

						difference_type index = tmp - header.first_entry_index;

						switch( header.color_map_entry_size )
						{
						case 16:
							{
								unsigned short v = reinterpret_cast< unsigned short * >( color_map_data + index * 2 )[ 0 ];
								//unsigned char a = ( v & 0x8000 ) >> 15;
								unsigned char r = ( ( v & 0x7c00 ) >> 10 ) * 8;
								unsigned char g = ( ( v & 0x03e0 ) >> 5 ) * 8;
								unsigned char b = ( v & 0x001f ) * 8;
								image( i, j ) = pixel_converter::convert_to( r, g, b );
							}
							break;

						case 24:
							{
								unsigned char r = color_map_data[ index * 3 + 2 ];
								unsigned char g = color_map_data[ index * 3 + 1 ];
								unsigned char b = color_map_data[ index * 3 + 0 ];
								image( i, j ) = pixel_converter::convert_to( r, g, b );
							}
							break;

						case 32:
							for( difference_type i = 0 ; i < width ; i++ )
							{
								//unsigned char a = color_map_data[ index * 4 + 3 ];
								unsigned char r = color_map_data[ index * 4 + 2 ];
								unsigned char g = color_map_data[ index * 4 + 1 ];
								unsigned char b = color_map_data[ index * 4 + 0 ];
								image( i, j ) = pixel_converter::convert_to( r, g, b );
							}
							break;

						default:
							break;
						}
					}
				}
				break;

			case 2:	// True-color Image
				for( difference_type j = 0 ; j < height ; j++ )
				{
					unsigned char *pixel = pixels + line_skip * j + ( from_left ? 0 : pixel_bytes ) * ( width - 1 );

					switch( header.pixel_depth )
					{
					case 16:
						for( difference_type i = 0 ; i < width ; i++ )
						{
							unsigned short pix = reinterpret_cast< unsigned short * >( pixel + pixel_skip * i )[ 0 ];
							//unsigned char a = ( pix & 0x8000 ) >> 15;
							unsigned char r = ( ( pix & 0x7c00 ) >> 10 ) * 8;
							unsigned char g = ( ( pix & 0x03e0 ) >> 5 ) * 8;
							unsigned char b = ( pix & 0x001f ) * 8;
							image( i, j ) = pixel_converter::convert_to( r, g, b );
						}
						break;

					case 24:
						for( difference_type i = 0 ; i < width ; i++ )
						{
							unsigned char *pix = pixel + pixel_skip * i;
							image( i, j ) = pixel_converter::convert_to( pix[ 2 ], pix[ 1 ], pix[ 0 ] );
						}
						break;

					case 32:
						for( difference_type i = 0 ; i < width ; i++ )
						{
							unsigned char *pix = pixel + pixel_skip * i;
							image( i, j ) = pixel_converter::convert_to( pix[ 2 ], pix[ 1 ], pix[ 0 ] );
						}
						break;

					default:
						break;
					}
				}
				break;

			case 3:	// Black and White Image
				// ���T�|�[�g
				ret = false;
				break;
			}

			delete [] buff;

			return( ret );
		}

		static difference_type convert_to_tga_data( const array2< T, Allocator > &image, unsigned char * &tga, size_type tga_bits, bool is_encode_RLE, bool from_top, bool from_left )
		{
			if( image.empty( ) || is_encode_RLE )
			{
				return( -1 );
			}

			difference_type pixel_bytes = tga_bits / 8;
			difference_type width  = image.width( );
			difference_type height = image.height( );

			tga = new unsigned char[ _tga_header_::bytes + width * height * pixel_bytes ];

			// TGA�p�̃w�b�_�̈ʒu���w�肷��
			_tga_header_	*pheader		= reinterpret_cast < _tga_header_ * >( tga );
			_tga_header_	&header			= *pheader;

			// ID Length Field 1 ( 1 byte )
			header.id_length = 0;

			// Color Map Type Field 2 ( 1 byte )
			header.color_map_type = 0;

			// Image Type Field 3 ( 1 byte )
			header.image_type = 2 + ( is_encode_RLE ? 8 : 0 );

			// Color Map Specification Field 4 ( 5 bytes )
			header.first_entry_index = 0;
			header.color_map_length = 0;
			header.color_map_entry_size = 0;

			// Image Specification Field 4 ( 10 bytes )
			header.x_origin = 0;
			header.y_origin = 0;
			header.image_width = static_cast< unsigned short >( width );
			header.image_height = static_cast< unsigned short >( height );
			header.pixel_depth = static_cast< unsigned char >( tga_bits );
			header.image_descriptor = ( is_encode_RLE ? 0x08 : 0 ) | ( from_top ? 0x20 : 0 ) | ( from_left ? 0 : 0x10 );

			unsigned char *image_data		= tga + _tga_header_::bytes;

			unsigned char *pixels = image_data + ( from_top ? 0 : width * ( height - 1 ) * pixel_bytes );

			difference_type line_skip  = from_top  ? width * pixel_bytes : - width * pixel_bytes;
			difference_type pixel_skip = from_left ? pixel_bytes : - pixel_bytes;

			for( difference_type j = 0 ; j < height ; j++ )
			{
				unsigned char *pixel = pixels + line_skip * j + ( from_left ? 0 : pixel_bytes ) * ( width - 1 );

				switch( header.pixel_depth )
				{
				case 16:
					for( difference_type i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						unsigned short &pix = reinterpret_cast< unsigned short * >( pixel + pixel_skip * i )[ 0 ];
						unsigned short a = ( 1 << 15 ) & 0x8000;
						unsigned short r = ( ( c.r / 8 ) << 10 ) & 0x7c00;
						unsigned short g = ( ( c.g / 8 ) << 5 ) & 0x03e0;
						unsigned short b = ( c.b / 8 ) & 0x001f;
						pix = a | r | g | b;
					}
					break;

				case 24:
					for( difference_type i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						unsigned char *pix = pixel + pixel_skip * i;
						pix[ 0 ] = static_cast< unsigned char >( c.b );
						pix[ 1 ] = static_cast< unsigned char >( c.g );
						pix[ 2 ] = static_cast< unsigned char >( c.r );
					}
					break;

				case 32:
					for( difference_type i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						unsigned char *pix = pixel + pixel_skip * i;
						pix[ 0 ] = static_cast< unsigned char >( c.b );
						pix[ 1 ] = static_cast< unsigned char >( c.g );
						pix[ 2 ] = static_cast< unsigned char >( c.r );
					}
					break;

				default:
					break;
				}
			}

			if( is_encode_RLE )
			{
				// ����T�|�[�g�\��
				return( _tga_header_::bytes + width * height * pixel_bytes );
			}
			else
			{
				return( _tga_header_::bytes + width * height * pixel_bytes );
			}
		}

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			size_type filesize;
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );

			// �t�@�C���T�C�Y���擾
			fseek( fp, 0, SEEK_END );
			filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

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
			fclose( fp );

			bool ret = convert_from_tga_data( buff, filesize, image );
			delete [] buff;
			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, size_type tga_bits, bool is_encode_RLE )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			if( !is_supported( tga_bits ) )
			{
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			unsigned char *buff = NULL;
			difference_type size = convert_to_tga_data( image, buff, tga_bits, is_encode_RLE, true, true );

			if( size <= 0 )
			{
				delete [] buff;
				return( false );
			}

			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				delete [] buff;
				return( false );
			}


			// �t�@�C���֏����o��
			unsigned char *pointer = buff;
			size_type write_size = 0, writed_size = 0;
			while( size > 0 )
			{
				write_size = size < 1024 ? size : 1024;

				writed_size = fwrite( pointer, sizeof( unsigned char ), write_size, fp );
				pointer += writed_size;
				size -= writed_size;
				if( write_size != writed_size )
				{
					fclose( fp );
					delete [] buff;
					return( false );
				}
			}
			fclose( fp );

			delete [] buff;
			return( true );
		}
	};
}

//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_tga_group TGA �摜���o��
//!
//! TGA�t�@�C����ǂݍ���ŁCMIST�R���e�i�֊i�[����D
//! �܂��CMIST�R���e�i����TGA�t�@�C���ւ̏����o�����T�|�[�g�D
//! 16�C24�C32�r�b�g�̃r�b�g�}�b�v�ɑΉ��D
//! ���k���ꂽ�r�b�g�}�b�v�͖��T�|�[�g�D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/tga.h>
//! @endcode
//!
//!  @{


/// @brief TGA�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @attention RLE���k�̂�������TGA���T�|�[�g�i�n�t�}����������p�������̂ɑ΂��Ă͖��Ή��j
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_tga( array2< T, Allocator > &image, const std::string &filename )
{
	return( __tga_controller__::tga_controller< T, Allocator >::read( image, filename ) );
}


/// @brief MIST�R���e�i�̉摜��TGA�`���Ńt�@�C���ɏo�͂���
//! 
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] tga_bits �c �ȉ��̃r�b�g�}�b�v�̃r�b�g���̂����ꂩ
//!
//! -# 16�r�b�g �c �e�F 5 �r�b�g�̃r�b�g�}�b�v
//! -# 24�r�b�g �c �t���J���[�r�b�g�}�b�v
//! -# 32�r�b�g �c �t���J���[�r�b�g�}�b�v�i�A���t�@�`�����l���t�j
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_tga( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type tga_bits = 24 )
{
	return( __tga_controller__::tga_controller< T, Allocator >::write( image, filename, tga_bits, false ) );
}

/// @}
//  TGA �摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_TGA__
