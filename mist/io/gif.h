/// @file mist/io/gif.h
//!
//! @brief GIF�摜��ǂݏ������邽�߂̃��C�u����
//!
//! �{���C�u�����́Chttp://sourceforge.net/projects/libungif/ �ŊJ�����s���Ă��� giflib �������� libungif �𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͖{���C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!

#ifndef __INCLUDE_MIST_GIF__
#define __INCLUDE_MIST_GIF__


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


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __gif_controller__
{
	extern "C"{
		#include <gif_lib.h>
	}

	template < class T, class Allocator >
	struct gif_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			size_type i, j;

			// GIF�t�@�C����ǂݍ���ŁC�w�b�_�����擾����
			GifFileType *GifFile = DGifOpenFileName( filename.c_str( ) );

			if( GifFile == NULL )
			{
				return( false );
			}

			size_type width  = static_cast< size_type >( GifFile->SWidth );
			size_type height = static_cast< size_type >( GifFile->SHeight );
			GifRowType *image_buffer = new GifRowType[ height ];

			// 1�s������̃o�C�g��
			size_type size = width * sizeof( GifPixelType );

			image_buffer[ 0 ] = new GifPixelType[ size ];

			// �w�i�F��ݒ肷��
			for( i = 0 ; i < width ; i++ )
			{
				image_buffer[ 0 ][ i ] = GifFile->SBackGroundColor;
			}

			for( i = 1 ; i < height ; i++ )
			{
				image_buffer[ i ] = new GifPixelType[ size ];
				memcpy( image_buffer[ i ], image_buffer[ 0 ], size );
			}

			// GIF�t�@�C���̓��e��ǂݍ���
			GifRecordType record_type;
			do
			{
				if( DGifGetRecordType( GifFile, &record_type ) == GIF_ERROR )
				{
					// �G���[�������I�I
					// ���炩�̃G���[����������
					std::cout << "Error!!" << std::endl;
				}

				switch( record_type )
				{
				case IMAGE_DESC_RECORD_TYPE:
					{
						if( DGifGetImageDesc( GifFile ) == GIF_ERROR )
						{
							// �G���[�������I�I
							// ���炩�̃G���[����������
							std::cout << "Error!!" << std::endl;
						}

						size_type x = GifFile->Image.Left;
						size_type y = GifFile->Image.Top;
						size_type w = GifFile->Image.Width;
						size_type h = GifFile->Image.Height;

						if( x + w > width || y + h > height )
						{
							// Image is not confined to screen dimension, aborted
							// �G���[�������I�I
							std::cout << "Error!!" << std::endl;
						}

						if( GifFile->Image.Interlace )
						{
							// �摜�ɑ΂���4�p�X�̏���������炵��
							static size_type InterlacedOffset[] = { 0, 4, 2, 1 };	// The way Interlaced image should.
							static size_type InterlacedJumps[] = { 8, 8, 4, 2 };    // be read - offsets and jumps...
							for( i = 0 ; i < 4 ; i++ )
							{
								for( j = y + InterlacedOffset[ i ] ; j < y + h ; j += InterlacedJumps[ i ] )
								{
									if( DGifGetLine( GifFile, &image_buffer[ j ][ x ], static_cast< int >( w ) ) == GIF_ERROR )
									{
										// �G���[�������I�I
										// ���炩�̃G���[����������
										std::cout << "Error!!" << std::endl;
									}
								}
							}
						}
						else
						{
							for( i = 0 ; i < h ; i++ )
							{
								if( DGifGetLine( GifFile, &image_buffer[ y++ ][ x ], static_cast< int >( w ) ) == GIF_ERROR )
								{
									// �G���[�������I�I
									// ���炩�̃G���[����������
									std::cout << "Error!!" << std::endl;
								}
							}
						}

						if( x == 0 && y == 0 && w == width && h == height )
						{
							// ��ԍŏ��̃t���[�������������̂ŒT�����I������
							record_type = TERMINATE_RECORD_TYPE;
						}
					}
					break;

				case EXTENSION_RECORD_TYPE:
					{
						// �S�Ă̊g���u���b�N���X�L�b�v����
						GifByteType *extention;
						int ext_code;
						if( DGifGetExtension( GifFile, &ext_code, &extention ) == GIF_ERROR )
						{
							// �G���[�������I�I
							// ���炩�̃G���[����������
							std::cout << "Error!!" << std::endl;
						}

						while( extention != NULL )
						{
							if( DGifGetExtensionNext( GifFile, &extention ) == GIF_ERROR )
							{
								// �G���[�������I�I
								// ���炩�̃G���[����������
								std::cout << "Error!!" << std::endl;
							}
						}
					}
					break;

				case TERMINATE_RECORD_TYPE:
					break;

				default:
		 		    // �s���ȃ��R�[�h�^�C�v
					break;
				}
			} while( record_type != TERMINATE_RECORD_TYPE );

			ColorMapObject *color_map = GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap;

			size_type color_map_size = static_cast< size_type >( color_map->ColorCount );

			image.resize( width, height );

			for( j = 0 ; j < height ; j++ )
			{
				for( i = 0 ; i < width ; i++ )
				{
					GifColorType *element = color_map->Colors + image_buffer[ j ][ i ];
					image( i, j ) = pixel_converter::convert_to( element->Red, element->Green, element->Blue );
				}
			}

			if( DGifCloseFile( GifFile ) == GIF_ERROR )
			{
				// �G���[�������I�I
				// ���炩�̃G���[����������
				std::cout << "Error!!" << std::endl;
			}

			for( i = 0 ; i < height ; i++ )
			{
				delete [] image_buffer[ i ];
			}
			delete [] image_buffer;

			return( true );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			int color_map_size = 256;
			ColorMapObject *color_map = MakeMapObject( color_map_size, NULL );

			if( color_map == NULL )
			{
				// �G���[�������I�I
				// ���炩�̃G���[����������
				std::cout << "Error!!" << std::endl;
			}

			size_type width  = image.width( );
			size_type height = image.height( );

			GifByteType *buffer = new GifByteType[ width * height ];
			GifByteType *red_buffer   = new GifByteType[ width * height ];
			GifByteType *green_buffer = new GifByteType[ width * height ];
			GifByteType *blue_buffer  = new GifByteType[ width * height ];

			size_type i;
			for( i = 0 ; i < width * height ; i++ )
			{
				color_type c = limits_0_255( pixel_converter::convert_from( image[ i ] ) );
				red_buffer[ i ]   = c.r;
				green_buffer[ i ] = c.g;
				blue_buffer[ i ]  = c.b;
			}

			if( QuantizeBuffer( static_cast< unsigned int >( width ), static_cast< unsigned int >( height ), &color_map_size, red_buffer, green_buffer, blue_buffer, buffer, color_map->Colors ) == GIF_ERROR )
			{
				// �G���[�������I�I
				// ���炩�̃G���[����������
				std::cout << "Error!!" << std::endl;
			}

			GifFileType *GifFile = EGifOpenFileName( filename.c_str( ), false );
			if( GifFile == NULL )
			{
				// �G���[�������I�I
				// ���炩�̃G���[����������
				std::cout << "Error!!" << std::endl;
			}

			EGifSetGifVersion( "89a" );

			if( EGifPutScreenDesc( GifFile, static_cast< int >( width ), static_cast< int >( height ), 8, 0, color_map ) == GIF_ERROR ||
					EGifPutImageDesc( GifFile, 0, 0, static_cast< int >( width ), static_cast< int >( height ), false, NULL ) == GIF_ERROR )
			{
				// �G���[�������I�I
				// ���炩�̃G���[����������
				std::cout << "Error!!" << std::endl;
			}

			GifByteType *pointer = buffer;
			for( i = 0 ; i < height ; i++ )
			{
				if( EGifPutLine( GifFile, pointer, static_cast< int >( width ) ) == GIF_ERROR )
				{
					// �G���[�������I�I
					// ���炩�̃G���[����������
					std::cout << "Error!!" << std::endl;
				}

				pointer += width;
			}

			if( EGifCloseFile( GifFile ) == GIF_ERROR )
			{
				// �G���[�������I�I
				// ���炩�̃G���[����������
			}

			FreeMapObject( color_map );

			delete [] buffer;
			delete [] red_buffer;
			delete [] green_buffer;
			delete [] blue_buffer;

			return( true );
		}
	};
}


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_gif_group GIF �摜���o��
//!  @{



/// @brief GIF�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! LZW���k�̂�������GIF���̂��܂��܂ȃt�H�[�}�b�g��ǂݍ��ނ��Ƃ��\�D
//! �܂��C�o�͂���MIST�R���e�i���J���[�摜���O���[�X�P�[���摜���ɂ���āC�s�N�Z���f�[�^��K�؂ɕϊ�����
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_gif( array2< T, Allocator > &image, const std::string &filename )
{
	return( __gif_controller__::gif_controller< T, Allocator >::read( image, filename ) );
}


/// @brief MIST�R���e�i�̉摜��GIF�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention LZW���k�̂�������GIF�摜���o�͂���ꍇ�́Cgiflib ���C�u��������LZW���k���L���ɂȂ��Ă���K�v������
//! 
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] use_lzw_compression �c LZW���k���ꂽGIF�摜���o�͂��邩�ǂ���
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//!
template < class T, class Allocator >
bool write_gif( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression = _LZW_COMPRESSION_SUPPORT_ )
{
	return( __gif_controller__::gif_controller< T, Allocator >::write( image, filename, use_lzw_compression ) );
}


/// @}
//  GIF �摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_GIF__
