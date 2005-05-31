/// @file mist/io/png.h
//!
//! @brief PNG�摜��ǂݏ������邽�߂̃��C�u����
//!
//! �{���C�u�����́Chttp://www.libpng.org/pub/png/ , http://www.libpng.org/ �ŊJ�����s���Ă��� libpng �𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͖{���C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!
#ifndef __INCLUDE_MIST_PNG__
#define __INCLUDE_MIST_PNG__


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


#include <string>

#include <png.h>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __png_controller__
{
	template < class T, class Allocator >
	struct png_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			FILE			*fp;
			png_structp		png_ptr;
			png_infop		info_ptr;
			unsigned long	width, height;
			int				bit_depth, color_type, interlace_type;
			size_type		i, j;

			fp = fopen( filename.c_str( ), "rb" );                           // �܂��t�@�C�����J���܂�
			if( !fp ) return( false );

			// png_ptr�\���̂��m�ہE���������܂�
			png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
			info_ptr = png_create_info_struct( png_ptr );             // info_ptr�\���̂��m�ہE���������܂�
			png_init_io( png_ptr, fp );                               // libpng��fp��m�点�܂�
			png_read_info( png_ptr, info_ptr );                       // PNG�t�@�C���̃w�b�_��ǂݍ��݂܂�
			png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);         // IHDR�`�����N�����擾���܂�

			png_bytepp png_buff;
			png_buff = ( png_bytepp )malloc( height * sizeof( png_bytep ) ); // �ȉ��R�s�͂Q�����z����m�ۂ��܂�
			for( i = 0 ; i < (size_type)height ; i++ ) png_buff[i] = ( png_bytep )malloc( png_get_rowbytes( png_ptr, info_ptr ) );

			image.resize( width, height );

			png_read_image( png_ptr, png_buff );	// �摜�f�[�^��ǂݍ��݂܂�

			bool ret = true;

			switch( color_type )
			{
			case PNG_COLOR_TYPE_GRAY:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][i], png_buff[j][i], png_buff[j][i] );
					}
				}
				break;

			case PNG_COLOR_TYPE_RGB:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][ i * 3 + 0 ], png_buff[j][ i * 3 + 1 ], png_buff[j][ i * 3 + 2 ] );
					}
				}
				break;

			case PNG_COLOR_TYPE_RGBA:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][ i * 4 + 0 ], png_buff[j][ i * 4 + 1 ], png_buff[j][ i * 4 + 2 ], png_buff[j][ i * 4 + 3 ] );
					}
				}
				break;

			case PNG_COLOR_TYPE_GA:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( png_buff[j][ i * 2 ], png_buff[j][ i * 2 ], png_buff[j][ i * 2 ], png_buff[j][ i * 2 + 1 ] );
					}
				}
				break;

			case PNG_COLOR_TYPE_PALETTE:
				for( j = 0 ; j < ( size_type )height ; j++ )
				{
					for( i = 0 ; i < ( size_type )width ; i++ )
					{
						png_color &p = info_ptr->palette[ png_buff[j][ i ] ];
						image( i, j ) = pixel_converter::convert_to( p.red, p.green, p.blue );
					}
				}
				break;

			default:
				ret = false;
				break;
			}

			for( i = 0 ; i < (size_type)height ; i++ ) free( png_buff[i] );            // �ȉ��Q�s�͂Q�����z���������܂�
			free( png_buff );

			// �Q�̍\���̂̃�������������܂�
			png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
			fclose( fp );

			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, int compression_level )
		{
			FILE *fp;
			png_structp	png_ptr;
			png_infop	info_ptr;
			size_type	width  = image.width( );
			size_type	height = image.height( );

			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL ) return(false);

			png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
			if( png_ptr == NULL )
			{
				fclose( fp );
				return( false );
			}

			info_ptr = png_create_info_struct( png_ptr );
			if( info_ptr == NULL )
			{
				png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
				fclose( fp );
				return( false );
			}
			if( setjmp( png_ptr->jmpbuf ) )
			{
				png_destroy_write_struct( &png_ptr, &info_ptr );
				fclose( fp );
				return( false );
			}

			if( compression_level > 9 ) compression_level = 9;

			png_init_io( png_ptr, fp );
			//png_set_write_status_fn(png_ptr, write_row_callback);
			png_set_filter( png_ptr, 0, PNG_ALL_FILTERS );
			if( compression_level < 0 )
			{
				png_set_compression_level( png_ptr, Z_BEST_COMPRESSION );
			}
			else
			{
				png_set_compression_level( png_ptr, compression_level );
			}

			int png_color_type = pixel_converter::color_num == 4 ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB;
			png_set_IHDR( png_ptr, info_ptr, static_cast< png_uint_32 >( width ), static_cast< png_uint_32 >( height ), 8, png_color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
			png_set_gAMA( png_ptr, info_ptr, 1.0 );

			{
				time_t		gmt;		// G.M.T.
				png_time	mod_time;
				png_text	text_ptr[5];

				time( &gmt );
				png_convert_from_time_t( &mod_time, gmt );
				png_set_tIME( png_ptr, info_ptr, &mod_time );

				text_ptr[ 0 ].key = "Title";
				text_ptr[ 0 ].text = "slipapng library";
				text_ptr[ 0 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 1 ].key = "Author";
				text_ptr[ 1 ].text = "mist project team";
				text_ptr[ 1 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 2 ].key = "Description";
				text_ptr[ 2 ].text = "created by mist project team";
				text_ptr[ 2 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 3 ].key = "Creation Time";
				text_ptr[ 3 ].text = png_convert_to_rfc1123( png_ptr, &mod_time );
				text_ptr[ 3 ].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[ 4 ].key = "Software";
				text_ptr[ 4 ].text = "mist png converter";
				text_ptr[ 4 ].compression = PNG_TEXT_COMPRESSION_NONE;
				png_set_text( png_ptr, info_ptr, text_ptr, 5 );
			}

			png_bytepp png_buff;
			png_buff = new png_byte*[height];

			size_type i, j;
			if( pixel_converter::color_num == 4 )
			{
				for( j = 0 ; j < height ; j++ )
				{
					png_buff[ j ] = new png_byte[width * 4];
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						png_buff[ j ][ i * 4 + 0 ] = static_cast< unsigned char >( c.r );
						png_buff[ j ][ i * 4 + 1 ] = static_cast< unsigned char >( c.g );
						png_buff[ j ][ i * 4 + 2 ] = static_cast< unsigned char >( c.b );
						png_buff[ j ][ i * 4 + 3 ] = static_cast< unsigned char >( c.a );
					}
				}
			}
			else
			{
				for( j = 0 ; j < height ; j++ )
				{
					png_buff[j] = new png_byte[width * 3];
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						png_buff[ j ][ i * 3 + 0 ] = static_cast< unsigned char >( c.r );
						png_buff[ j ][ i * 3 + 1 ] = static_cast< unsigned char >( c.g );
						png_buff[ j ][ i * 3 + 2 ] = static_cast< unsigned char >( c.b );
					}
				}
			}

			png_write_info( png_ptr, info_ptr );
			png_write_image( png_ptr, png_buff );
			png_write_end( png_ptr, info_ptr );
			png_destroy_write_struct( &png_ptr, &info_ptr );
			fclose( fp );

			for( j = 0 ; j < height ; j++ ) delete [] png_buff[j];
			delete [] png_buff;

			return( true );
		}
	};
}


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_png_group PNG �摜���o��
//!
//! PNG�t�@�C����ǂݍ���ŁCMIST�R���e�i�֊i�[����D
//! �܂��CMIST�R���e�i����PNG�t�@�C���ւ̏����o�����T�|�[�g�D
//! �ʓr http://libpng.org ���񋟂���t���[��PNG���C�u�������K�v�ł��D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/png.h>
//! @endcode
//!
//!  @{


/// @brief PNG�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! RGB��RGBA��J���[�e�[�u�����g�p�����S�Ă�PNG�摜��ǂݍ��ނ��Ƃ��\�ł�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_png( array2< T, Allocator > &image, const std::string &filename )
{
	return( __png_controller__::png_controller< T, Allocator >::read( image, filename ) );
}


/// @brief MIST�R���e�i�̉摜��PNG�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention ���k���x����0�`9�܂ł���C�����ɕ��̒l���w�肷��ƁC�K�؂Ȉ��k���x���������I�ɑI�������i�f�t�H���g�j
//! 
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] compression_level �c PNG�摜���o�͂���ۂ̈��k���x��
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_png( const array2< T, Allocator > &image, const std::string &filename, int compression_level = -1 )
{
	return( __png_controller__::png_controller< T, Allocator >::write( image, filename, compression_level ) );
}


/// @}
//  PNG �摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_PNG__
