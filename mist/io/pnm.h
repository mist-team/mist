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

/// @file mist/io/pnm.h
//!
//! @brief PBM�CPGM�CPPM�CPNM�摜��ǂݏ������邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_PNM__
#define __INCLUDE_MIST_PNM__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif


#include <iostream>
#include <string>
#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __pnm_controller__
{
	enum PNM_TYPE
	{
		UNKNOWN = 0,
		P1,
		P2,
		P3,
		P4,
		P5,
		P6
	};

	template < bool b >
	struct _gray_converter_
	{
		template < class T >
		static T get_value( const T &v ){ return( v ); }
	};

	template < >
	struct _gray_converter_< true >
	{
		template < class T >
		static typename T::value_type get_value( const T &v ){ return( v.get_value( ) ); }
	};

	template < class T, class Allocator >
	struct pnm_controller
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef _pixel_converter_< T > pixel_converter;
		typedef _gray_converter_< is_color< T >::value > gray_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename pixel_converter::value_type value_type;

		static PNM_TYPE pnm_format( const std::string &str )
		{
			if( str == "P1" )
			{
				return( P1 );
			}
			else if( str == "P2" )
			{
				return( P2 );
			}
			else if( str == "P3" )
			{
				return( P3 );
			}
			else if( str == "P4" )
			{
				return( P4 );
			}
			else if( str == "P5" )
			{
				return( P5 );
			}
			else if( str == "P6" )
			{
				return( P6 );
			}
			else
			{
				return( UNKNOWN );
			}
		}

		static const unsigned char *get_line( const unsigned char *s, const unsigned char *e, std::string &line )
		{
			line = "";
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
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
					s = s + 1;
					break;
				}
				line += *s;
				s++;
			}
			return( s > e ? e : s );
		}

		static const unsigned char *get_value( const unsigned char *s, const unsigned char *e, std::string &line, bool &flag )
		{
			line = "";
			// �擪�̋󔒁i���s��^�u���܂ށj���΂�
			while( s < e )
			{
				if( flag && s[ 0 ] == '#' )
				{
					// �R�����g�s�Ȃ̂ł�����΂�
					while( s < e )
					{
						if( s[ 0 ] == '\r' )
						{
							if( s + 1 != e && s[ 1 ] == '\n' )
							{
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
							s++;
							break;
						}
						s++;
					}
				}
				else if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s = s + 1;
					}
					flag = true;
				}
				else if( s[ 0 ] == '\n' )
				{
					s++;
					flag = true;
				}
				else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
				{
					s++;
				}
				else
				{
					break;
				}
			}
			// ���ɋ󔒂�����O�܂Ői�߂�
			flag = false;
			while( s < e )
			{
				if( s[ 0 ] == '\r' )
				{
					if( s + 1 != e && s[ 1 ] == '\n' )
					{
						s = s + 2;
					}
					else
					{
						s++;
					}
					flag = true;
					break;
				}
				else if( s[ 0 ] == '\n' )
				{
					s++;
					flag = true;
					break;
				}
				else if( s[ 0 ] == ' ' || s[ 0 ] == '\t' )
				{
					s++;
					break;
				}
				line += *s;
				s++;
			}
			return( s > e ? e : s );
		}

		static size_type split_string( const std::string &line, const char ch, std::vector< std::string > &elements )
		{
			std::string str = "";
			size_type i = 0;
			elements.clear( );
			while( i < line.size( ) )
			{
				for( ; i < line.size( ) && line[ i ] == ch ; i++ ){}

				str = "";
				for( ; i < line.size( ) && line[ i ] != ch ; i++ )
				{
					str += line[ i ];
				}

				if( str != "" )
				{
					elements.push_back( str );
				}
			}
			return( elements.size( ) );
		}

		static bool is_number( const std::string &line )
		{
			for( size_t i = 0 ; i < line.size( ) ; i++ )
			{
				if( line[ i ] < '0' || '9' < line[ i ] )
				{
					return( false );
				}
			}
			return( true );
		}

		static bool convert_from_pnm_data( array2< T, Allocator > &image, const unsigned char *buff, size_type len, typename array2< T, Allocator >::size_type level )
		{
			// PNM�`���̃w�b�_����
			const unsigned char *p = buff;
			const unsigned char *e = buff + len;
			std::string line = "";
			std::vector< std::string > elements;

			bool flag = true;
			p = get_value( p, e, line, flag );
			PNM_TYPE pnm_type = pnm_format( line );
			if( pnm_type == UNKNOWN )
			{
				// ���T�|�[�g�̃r�b�g�}�b�v
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			// �摜�T�C�Y���擾����
			size_type w = 0, h = 0, gray_level = level;
			p = get_value( p, e, line, flag );
			if( is_number( line ) )
			{
				w = atoi( line.c_str( ) );
			}
			else
			{
				// ���T�|�[�g�̃r�b�g�}�b�v
				std::cerr << "Image size is unknown!" << std::endl;
			}

			p = get_value( p, e, line, flag );
			if( is_number( line ) )
			{
				h = atoi( line.c_str( ) );
			}
			else
			{
				// ���T�|�[�g�̃r�b�g�}�b�v
				std::cerr << "Image size is unknown!" << std::endl;
			}

			image.resize( w, h );

			// �Z�W�̃��x�������擾����
			switch( pnm_type )
			{
			case P2:
			case P3:
			case P5:
			case P6:
				p = get_value( p, e, line, flag );
				if( !is_number( line ) )
				{
					// ���T�|�[�g�̃r�b�g�}�b�v
					std::cerr << "Image size is unknown!" << std::endl;
					return( false );
				}
				gray_level = atoi( line.c_str( ) );
				break;

			default:
				break;
			}

			// ��f�f�[�^���擾����
			size_type i = 0;
			double scale = static_cast< double >( level ) / static_cast< double >( gray_level );
			switch( pnm_type )
			{
			case P1:
			case P2:
				while( i < image.size( ) && p < e )
				{
					p = get_line( p, e, line );
					split_string( line, ' ', elements );
					for( size_type j = 0 ; i < image.size( ) && j < elements.size( ) ; j++ )
					{
						image[ i++ ] = static_cast< value_type >( atoi( elements[ j ].c_str( ) ) * scale );
					}
				}
				break;

			case P3:
				while( i < image.size( ) && p < e )
				{
					p = get_line( p, e, line );
					split_string( line, ' ', elements );
					for( size_type j = 0 ; i < image.size( ) && j < elements.size( ) ; j += 3 )
					{
						value_type r = static_cast< value_type >( atoi( elements[ j + 0 ].c_str( ) ) * scale );
						value_type g = static_cast< value_type >( atoi( elements[ j + 1 ].c_str( ) ) * scale );
						value_type b = static_cast< value_type >( atoi( elements[ j + 2 ].c_str( ) ) * scale );
						image[ i++ ] = pixel_converter::convert_to( r, g, b );
					}
				}
				break;

			case P4:
			case P5:
				while( i < image.size( ) && p < e )
				{
					image[ i++ ] = static_cast< value_type >( *p++ * scale );
				}
				break;

			case P6:
				while( i < image.size( ) && p < e )
				{
					value_type r = static_cast< value_type >( p[ 0 ] * scale );
					value_type g = static_cast< value_type >( p[ 1 ] * scale );
					value_type b = static_cast< value_type >( p[ 2 ] * scale );
					image[ i++ ] = pixel_converter::convert_to( r, g, b );
					p += 3;
				}
				break;

			default:
				return( false );
			}

			return( p == e );
		}

		static bool read( array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type level )
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

			bool ret = convert_from_pnm_data( image, buff, filesize, level );
			delete [] buff;
			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, PNM_TYPE pnm_type, typename array2< T, Allocator >::size_type level )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

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
			else if( pnm_type == UNKNOWN )
			{
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				return( false );
			}

			// �w�b�_������������
			fprintf( fp, "P%1d\n", pnm_type );
			fprintf( fp, "# Created by MIST\n" );
			fprintf( fp, "%d %d\n", static_cast< int >( image.width( ) ), static_cast< int >( image.height( ) ) );

			size_type i, j;

			switch( pnm_type )
			{
			case P2:
			case P3:
			case P5:
			case P6:
				{
					typename array2< T, Allocator >::value_type max = image[ 0 ];
					for( i = 1 ; i < image.size( ) ; i++ )
					{
						max = max > image[ i ] ? max : image[ i ];
					}
					int max_level = static_cast< int >( gray_converter::get_value( max ) );
					max_level = max_level > static_cast< int >( level ) ? max_level : static_cast< int >( level );
					fprintf( fp, "%d\n", max_level );
				}
				break;

			default:
				break;
			}


			// ��f�f�[�^�������o��
			switch( pnm_type )
			{
			case P1:
			case P2:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						fprintf( fp, "%d ", static_cast< int >( gray_converter::get_value( image( i, j ) ) ) );
					}
					fputc( '\n', fp );
				}
				break;

			case P3:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						color_type c = pixel_converter::convert_from( image( i, j ) );
						fprintf( fp, "%d ", static_cast< int >( c.r ) );
						fprintf( fp, "%d ", static_cast< int >( c.g ) );
						fprintf( fp, "%d ", static_cast< int >( c.b ) );
					}
					fputc( '\n', fp );
				}
				break;

			case P4:
			case P5:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						fprintf( fp, "%c", static_cast< unsigned char >( gray_converter::get_value( image( i, j ) ) ) );
					}
				}
				break;

			case P6:
				for( j = 0 ; j < image.height( ) ; j++ )
				{
					for( i = 0 ; i < image.width( ) ; i++ )
					{
						color_type c = pixel_converter::convert_from( image( i, j ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.r ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.g ) );
						fprintf( fp, "%c", static_cast< unsigned char >( c.b ) );
					}
				}
				break;

			default:
				fclose( fp );
				return( false );
			}

			fclose( fp );

			return( true );
		}
	};
}


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_pnm_group PBM�CPGM�CPPM�CPNM �摜���o��
//!
//! PNM�iPBM�CPGM�CPPM�j�`���̃t�@�C����ǂݍ���ŁCMIST�R���e�i�֊i�[����D
//! �܂��CMIST�R���e�i����PNM�t�@�C���ւ̏����o�����T�|�[�g�D
//! P1�CP2�CP3�CP4�CP5�CP6�`����PNM�ɑΉ��D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/pnm.h>
//! @endcode
//!
//!
//!  @{


/// @brief PNM�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! P1�`P6�܂ł̑S�Ă�PNM�摜��ǂݍ��ނ��Ƃ��\�ł�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//! @param[in]  level    �c �ǂݍ���PNM�t�@�C����MIST�R���e�i�Ɋi�[����ۂ̗ʎq�����x��
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_pnm( array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type level = 255 )
{
	return( __pnm_controller__::pnm_controller< T, Allocator >::read( image, filename, level ) );
}


/// @brief PNM�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! P1�`P6�܂ł̑S�Ă�PNM�摜��ǂݍ��ނ��Ƃ��\�ł�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//! @param[in]  level    �c �ǂݍ���PNM�t�@�C����MIST�R���e�i�Ɋi�[����ۂ̗ʎq�����x��
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_pnm( array2< T, Allocator > &image, const std::wstring &filename, typename array2< T, Allocator >::size_type level = 255 )
{
	return( read_pnm( image, wstr2str( filename ), level ) );
}


/// @brief MIST�R���e�i�̉摜��PNM�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention �o�͂���摜�`���ɂ���āC�K�v�ȃ}�W�b�N�i���o�[��I������K�v����
//! @attention �f�t�H���g�̃}�W�b�N�i���o�[��P6�̃J���[�摜�ŁC�ʎq�����x����256�K��
//! 
//! - PNM�摜�̃}�W�b�N�i���o�[
//!   -# bitmap (ASCII) 2�l 
//!   -# graymap (ASCII) �O���[�X�P�[�� 
//!   -# pixmap (ASCII) �J���[ 
//!   -# bitmap (Binary) 2�l 
//!   -# graymap (Binary) �O���[�X�P�[�� 
//!   -# pixmap (Binary) �J���[ 
//!
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] pnm_type �c PNM�摜�̃}�W�b�N�i���o�[�̂�����1��
//! @param[in] level    �c �o�͂���PNM�t�@�C���̗ʎq���r�b�g��
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_pnm( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type pnm_type = 6, typename array2< T, Allocator >::size_type level = 255 )
{
	if( pnm_type > 6 ) 
	{
		std::cerr << "This format is not supported currently!" << std::endl;
		return( false );
	}
	return( __pnm_controller__::pnm_controller< T, Allocator >::write( image, filename, static_cast< __pnm_controller__::PNM_TYPE >( pnm_type ), level ) );
}


/// @brief MIST�R���e�i�̉摜��PNM�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention �o�͂���摜�`���ɂ���āC�K�v�ȃ}�W�b�N�i���o�[��I������K�v����
//! @attention �f�t�H���g�̃}�W�b�N�i���o�[��P6�̃J���[�摜�ŁC�ʎq�����x����256�K��
//! 
//! - PNM�摜�̃}�W�b�N�i���o�[
//!   -# bitmap (ASCII) 2�l 
//!   -# graymap (ASCII) �O���[�X�P�[�� 
//!   -# pixmap (ASCII) �J���[ 
//!   -# bitmap (Binary) 2�l 
//!   -# graymap (Binary) �O���[�X�P�[�� 
//!   -# pixmap (Binary) �J���[ 
//!
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] pnm_type �c PNM�摜�̃}�W�b�N�i���o�[�̂�����1��
//! @param[in] level    �c �o�͂���PNM�t�@�C���̗ʎq���r�b�g��
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_pnm( const array2< T, Allocator > &image, const std::wstring &filename, typename array2< T, Allocator >::size_type pnm_type = 6, typename array2< T, Allocator >::size_type level = 255 )
{
	return( write_pnm( image, wstr2str( filename ), static_cast< __pnm_controller__::PNM_TYPE >( pnm_type ), level ) );
}


/// @}
//  PNM�摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_PNM__
