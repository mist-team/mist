#ifndef __INCLUDE_MIST_PNG__
#define __INCLUDE_MIST_PNG__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
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
		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			array2< rgb< T >, Allocator > img;
			bool ret = png_controller< rgb< T >, Allocator >::read_png( img, filename );
			if( !ret ) return( false );

			typename array2< T, Allocator >::size_type i;
			image.resize( img.width( ), img.height( ) );
			for( i = 0 ; i < img.size( ) ; i++ )
			{
				image[i] = img[i].get_value( );
			}

			return( true );
		}

		static bool write( array2< T, Allocator > &image, const std::string &filename, int compression_level )
		{
			array2< rgb< T >, Allocator > img( image.width( ), image.height( ) );
			typename array2< T, Allocator >::size_type i;
			for( i = 0 ; i < img.size( ) ; i++ )
			{
				img[i].r = img[i].get_value( );
				img[i].g = img[i].get_value( );
				img[i].b = img[i].get_value( );
			}

			return( png_controller< rgb< T >, Allocator >::write_png( img, filename, compression_level ) );
		}
	};

	template < class T, class Allocator >
	struct png_controller< rgb< T >, Allocator >
	{
		static bool read( array2< rgb< T >, Allocator > &image, const std::string &filename )
		{
			typedef typename array2< rgb< T >, Allocator >::size_type size_type;
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
			if( color_type == PNG_COLOR_TYPE_GRAY )
			{
				for( j = 0 ; j < (size_type)height ; j++ )
				{
					for( i = 0 ; i < (size_type )width ; i++ )
					{
						image( i, j ).r = png_buff[j][i];
						image( i, j ).g = png_buff[j][i];
						image( i, j ).b = png_buff[j][i];
					}
				}
			}
			else if( color_type == PNG_COLOR_TYPE_RGB )
			{
				for( j = 0 ; j < (size_type)height ; j++ )
				{
					for( i = 0 ; i < (size_type )width ; i++ )
					{
						image( i, j ).r = png_buff[j][ i * 3 + 0 ];
						image( i, j ).g = png_buff[j][ i * 3 + 1 ];
						image( i, j ).b = png_buff[j][ i * 3 + 2 ];
					}
				}
			}

			for( i = 0 ; i < (size_type)height ; i++ ) free( png_buff[i] );            // �ȉ��Q�s�͂Q�����z���������܂�
			free( png_buff );

			// �Q�̍\���̂̃�������������܂�
			png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)NULL );
			fclose( fp );
			return( true );
		}

		static bool write( array2< rgb< T >, Allocator > &image, const std::string &filename, int compression_level )
		{
			typedef typename array2< rgb< T >, Allocator >::size_type size_type;
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
			png_set_IHDR( png_ptr, info_ptr, static_cast< png_uint_32 >( width ), static_cast< png_uint_32 >( height ), 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
			png_set_gAMA( png_ptr, info_ptr, 1.0 );

			{
				time_t		gmt;		// G.M.T.
				png_time	mod_time;
				png_text	text_ptr[5];

				time( &gmt );
				png_convert_from_time_t( &mod_time, gmt );
				png_set_tIME( png_ptr, info_ptr, &mod_time );

				text_ptr[0].key = "Title";
				text_ptr[0].text = "slipapng library";
				text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[1].key = "Author";
				text_ptr[1].text = "mist project team";
				text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[2].key = "Description";
				text_ptr[2].text = "created by mist project team";
				text_ptr[2].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[3].key = "Creation Time";
				text_ptr[3].text = png_convert_to_rfc1123( png_ptr, &mod_time );
				text_ptr[3].compression = PNG_TEXT_COMPRESSION_NONE;
				text_ptr[4].key = "Software";
				text_ptr[4].text = "mist png converter";
				text_ptr[4].compression = PNG_TEXT_COMPRESSION_NONE;
				png_set_text( png_ptr, info_ptr, text_ptr, 5 );
			}

			png_bytepp png_buff;
			png_buff = new png_byte*[height];

			size_type i, j;
			for( j = 0 ; j < height ; j++ )
			{
				png_buff[j] = new png_byte[width * 3];
				for( i = 0 ; i < width ; i++ )
				{
					png_buff[j][ i * 3 + 0 ] = static_cast< unsigned char >( image( i, j ).r );
					png_buff[j][ i * 3 + 1 ] = static_cast< unsigned char >( image( i, j ).g );
					png_buff[j][ i * 3 + 2 ] = static_cast< unsigned char >( image( i, j ).b );
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


template < class T, class Allocator >
bool read_png( array2< T, Allocator > &image, const std::string &filename )
{
	return( __png_controller__::png_controller< T, Allocator >::read( image, filename ) );
}

template < class T, class Allocator >
bool write_png( array2< T, Allocator > &image, const std::string &filename, int compression_level = -1 )
{
	return( __png_controller__::png_controller< T, Allocator >::write( image, filename, compression_level ) );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_PNG__
