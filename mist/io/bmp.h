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

/// @file mist/io/bmp.h
//!
//! @brief �r�b�g�}�b�v�摜��ǂݏ������邽�߂̃��C�u����
//!

#ifndef __INCLUDE_MIST_BMP__
#define __INCLUDE_MIST_BMP__


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

#include <cstdio>
#include <iostream>
#include <string>

#include <deque>
#include <map>
#include <algorithm>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __bmp_controller__
{
	// �\���̓��̃A���C�����g��1�o�C�g�ɐݒ肵�C�p�f�B���O���֎~����
#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( push, bmp_align, 1 )
#endif
		struct _rgbquad_
		{
			enum{ bytes = 4 };
			unsigned char	rgbBlue;
			unsigned char	rgbGreen;
			unsigned char	rgbRed;
			unsigned char	rgbReserved;
		} _MIST_PACKED;

		struct _bitmapinfoheader_
		{
			enum{ bytes = 40 };
			unsigned int	biSize;
			signed   int	biWidth;
			signed   int	biHeight;
			unsigned short	biPlanes;
			unsigned short	biBitCount;
			unsigned int	biCompression;
			unsigned int	biSizeImage;
			signed   int	biXPelsPerMeter;
			signed   int	biYPelsPerMeter;
			unsigned int	biClrUsed;
			unsigned int	biClrImportant;
		} _MIST_PACKED;

		struct _bitmapfileheader_
		{
			enum{ bytes = 14 };
			unsigned short	bfType;
			unsigned int	bfSize;
			unsigned short	bfReserved1;
			unsigned short	bfReserved2;
			unsigned int	bfOffBits;
		} _MIST_PACKED;

#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( pop, bmp_align )
#endif
	// �\���̓��̃A���C�����g��1�o�C�g�ɐݒ肵�C�p�f�B���O���֎~���� �` �����܂� �`

	template < class T, class Allocator >
	struct bmp_controller
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool is_supported( size_type bmp_bits )
		{
			bool ret = false;
			switch( bmp_bits )
			{
			case 1:
			case 4:
			case 8:
//			case 16:
			case 24:
			case 32:
				ret = true;
				break;

			default:
				break;
			}
			return( ret );
		}

		static size_type get_bmp_line_bytes( size_type width, size_type bmp_bits )
		{
			switch( bmp_bits )
			{
			case 1:
				{
					size_type tmp = width / 8;
					width = width == tmp * 8 ? tmp : tmp + 1;
				}
				break;

			case 4:
				{
					size_type tmp = width / 2;
					width = width == tmp * 2 ? tmp : tmp + 1;
				}
				break;

			case 8:
				break;

			case 16:
				width *= 2;
				break;

			case 24:
				width *= 3;
				break;

			case 32:
				width *= 4;
				break;

			default:
				width = 0;
				break;
			}
			return( width );
		}

		static size_type get_bmp_palette_num( size_type bmp_bits )
		{
			size_type color_num;
			switch( bmp_bits )
			{
			case 1:
				color_num = 2;
				break;

			case 4:
				color_num = 16;
				break;

			case 8:
				color_num = 256;
				break;

			default:
				color_num = 0;
				break;
			}
			return( color_num );
		}

		static size_type get_bmp_line_strip( size_type width, size_type bmp_bits )
		{
			size_type line_bytes = get_bmp_line_bytes( width, bmp_bits );
			size_type rest = line_bytes % 4;
			if( rest == 0 )
			{
				return( line_bytes );
			}
			else
			{
				return( line_bytes + 4 - rest );
			}
		}

		static size_type get_bmp_bytes( const array2< T, Allocator > &image, size_type bmp_bits )
		{
			return( _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes + _rgbquad_::bytes * get_bmp_palette_num( bmp_bits ) + image.height( ) * get_bmp_line_strip( image.width( ), bmp_bits ) );
		}


		class __PALETTE__
		{
		public:
			unsigned char red;
			unsigned char green;
			unsigned char blue;
			unsigned long value;

			const __PALETTE__ &operator =( const __PALETTE__ &c )
			{
				if( this == &c ) return( *this );
				red = c.red;
				green = c.green;
				blue = c.blue;
				value = c.value;
				return( *this );
			}
			bool operator ==( const __PALETTE__ &c ) const { return( red == c.red && green == c.green && blue == c.blue ); }
			bool operator  <( const __PALETTE__ &c ) const
			{
				if( red < c.red ) return( true );
				if( red != c.red && green < c.green ) return( true );
				if( red != c.red && green != c.green && blue < c.blue ) return( true );
				return( false );
			}
			static bool compare_value( const __PALETTE__ &c1, const __PALETTE__ &c2 ){ return( c1.value > c2.value ); }

			__PALETTE__( unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned long v = 0 ) : red( r ), green( g ), blue( b ), value( v ){ }
		};

		static double distance( const __PALETTE__ &c1, const __PALETTE__ &c2 )
		{
			return( ( c1.red - c2.red )  * ( c1.red - c2.red ) + ( c1.green - c2.green ) * ( c1.green - c2.green ) + ( c1.blue - c2.blue )   * ( c1.blue - c2.blue ) );
		}

		// �ł���{�ƂȂ�256�F�̃p���b�g���쐬����
		static void create_default_palette( std::deque< __PALETTE__ > &palette )
		{
			int i, j, k;

			palette.clear( );

			for( i = 0 ; i < 6 ; i++ ) /* �J���[�e�[�u���쐬 */
			{
				for( j = 0 ; j < 6 ; j++ )
				{
					for( k = 0 ; k < 6 ; k++ )
					{
						palette.push_back( __PALETTE__( k * 51, j * 51, i * 51 ) );
					}
				}
			}
			for( i = 1 ; i < 32 ; i++ )
			{
				palette.push_back( __PALETTE__( i * 8, i * 8, i * 8 ) );
			}
			palette.push_back( __PALETTE__( 255, 255, 255 ) );
		}

		static void eliminate_palette( std::deque< __PALETTE__ > &palette, size_type palette_num )
		{
			if( palette.size( ) == palette_num ) return;
			if( palette.size( ) < palette_num )
			{
				size_type num = palette_num - palette.size( );
				for( size_type i = 0 ; i < num ; i++ ) palette.push_back( __PALETTE__( ) );
				return;
			}

			// �g�p�񐔂̑������ɕ��ׂ�
			std::sort( palette.begin( ), palette.end( ), __PALETTE__::compare_value );

			// �擪���� palette_num �܂ł̃p���b�g���c��
			palette.erase( palette.begin( ) + palette_num, palette.end( ) );
		}

		// �J���[�p���b�g���쐬
		static void create_adaptive_palette( const array2< T, Allocator > &image, _rgbquad_ *palette, size_type palette_num )
		{
			size_type width = image.width( );
			size_type height = image.height( );

			std::deque< __PALETTE__ > adaptive_palette;
			std::map< __PALETTE__, __PALETTE__ > palette_table;

			unsigned char r, g, b;
			size_type i;

			// �g�p����Ă���F��S�ė񋓂���
			for( i = 0 ; i < width * height ; i++ )
			{
				color_type c = pixel_converter::convert_from( image[i] );
				r = static_cast< unsigned char >( c.r );
				g = static_cast< unsigned char >( c.g );
				b = static_cast< unsigned char >( c.b );

				if( palette_table.find( __PALETTE__( r, g, b ) ) != palette_table.end( ) )
				{
					palette_table[ __PALETTE__( r, g, b ) ].value += 1;
				}
				else
				{
					palette_table[ __PALETTE__( r, g, b ) ] = __PALETTE__( r, g, b, 0 );
				}
			}

			typename std::map< __PALETTE__, __PALETTE__ >::iterator ite = palette_table.begin( );
			for( ; ite != palette_table.end( ) ; ++ite )
			{
				adaptive_palette.push_back( ite->second );
			}

			eliminate_palette( adaptive_palette, palette_num );

			// �p���b�g�������o��
			for( i = 0 ; i < palette_num ; i++ )
			{
				palette[i].rgbRed      = adaptive_palette[i].red;
				palette[i].rgbGreen    = adaptive_palette[i].green;
				palette[i].rgbBlue     = adaptive_palette[i].blue;
				palette[i].rgbReserved = 0;
			}

			adaptive_palette.clear( );
			palette_table.clear( );
		}

		static size_type find_palette_index( const T &pixel, _rgbquad_ *palette, size_type palette_num )
		{
			unsigned char r, g, b, pr, pg, pb;
			color_type c = pixel_converter::convert_from( pixel );
			r = static_cast< unsigned char >( c.r );
			g = static_cast< unsigned char >( c.g );
			b = static_cast< unsigned char >( c.b );

			double minimum, distance;
			size_type k, index = 0;
			/* �ł��߂��F������ */
			pr = palette[0].rgbRed;
			pg = palette[0].rgbGreen;
			pb = palette[0].rgbBlue;

			/* �s�N�Z���ƃJ���[�e�[�u���v�f�̋����v�Z */
			minimum = ( r - pr ) * ( r - pr ) + ( g - pg ) * ( g - pg ) + ( b - pb ) * ( b - pb );

			for( k = 1 ; k < palette_num ; k++ )
			{
				/* �ł��߂��F������ */
				pr = palette[k].rgbRed;
				pg = palette[k].rgbGreen;
				pb = palette[k].rgbBlue;

				/* �s�N�Z���ƃJ���[�e�[�u���v�f�̋����v�Z */
				distance = ( r - pr ) * ( r - pr ) + ( g - pg ) * ( g - pg ) + ( b - pb ) * ( b - pb );

				/* �F��ԓ��̋������ŏ��̐F��ۑ� */
				if( minimum > distance )
				{
					/* ����܂łŋ����ŏ��̐F */
					minimum = distance; /* �ŏ��l�ۑ� */
					index = k; /* �J���[�e�[�u���ԍ��ۑ� */
				}
			}
			return( index );
		}

		static bool convert_from_bmp_data( unsigned char *bmp, size_type num_bytes, array2< T, Allocator > &image )
		{
			// �r�b�g�}�b�v�p�̃w�b�_�̈ʒu���w�肷��
			_bitmapfileheader_	*pfilehead  = reinterpret_cast < _bitmapfileheader_ * >	( bmp );
			_bitmapinfoheader_	*pinfohead  = reinterpret_cast < _bitmapinfoheader_ * >	( bmp + _bitmapfileheader_::bytes );
			_rgbquad_			*palette    = reinterpret_cast < _rgbquad_ * >			( bmp + _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes );

			_bitmapfileheader_	&filehead     = *pfilehead;
			_bitmapinfoheader_	&infohead     = *pinfohead;

			if( filehead.bfType != 'M' * 256 + 'B' && filehead.bfType != 'm' * 256 + 'b' )
			{
				// �r�b�g�}�b�v�ł͂���܂���
				std::cerr << "This is not a bitmap format!" << std::endl;
				return( false );
			}

			size_type bmp_bits = infohead.biBitCount;
			size_type width    = infohead.biWidth;
			size_type height   = infohead.biHeight;

			if( infohead.biCompression != 0 )
			{
				// ���k�̂��������r�b�g�}�b�v�͖��T�|�[�g
				std::cerr << "Compressed bitmap format is not supported." << std::endl;
				return( false );
			}

			if( !is_supported( bmp_bits ) )
			{
				// ���T�|�[�g�̃r�b�g�}�b�v
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			image.resize( width, height );

			size_type i, j, jj, line_bytes = get_bmp_line_strip( width, bmp_bits );
			unsigned char *pixel = bmp + static_cast< size_type >( filehead.bfOffBits );

			if( line_bytes * height > num_bytes - static_cast< size_type >( filehead.bfOffBits ) )
			{
				// �s���ȃr�b�g�}�b�v�w�b�_
				// �K��̃f�[�^�o�C�g���������݂��Ă��Ȃ�
				std::cerr << "This bitmap has incorrect BMP header." << std::endl;
				return( false );
			}

			for( jj = 0 ; jj < height ; jj++ )
			{
				j = height - jj - 1;
				switch( bmp_bits )
				{
				case 1:
					{
						size_type rest = width % 8;
						size_type w = ( width - rest ) / 8;
						size_type index, k;
						unsigned int pix;
						for( i = 0 ; i < w ; i++ )
						{
							pix = pixel[ i ];
							for( k = 0 ; k < 8 ; k++ )
							{
								index = ( pix & 0x80 ) == 0 ? 0 : 1;
								image( i * 8 + k, j ) = pixel_converter::convert_to( palette[ index ].rgbRed, palette[ index ].rgbGreen, palette[ index ].rgbBlue );
								pix <<= 1;
							}
						}
						if( rest > 0 )
						{
							pix = pixel[ w ];
							for( i = 0 ; i < rest ; i++ )
							{
								index = ( pix & 0x80 ) == 0 ? 0 : 1;
								image( w * 8 + i, j ) = pixel_converter::convert_to( palette[ index ].rgbRed, palette[ index ].rgbGreen, palette[ index ].rgbBlue );
								pix <<= 1;
							}
						}
					}
					break;

				case 4:
					{
						size_type rest = width % 2;
						size_type w = ( width - rest ) / 2;
						size_type index;
						for( i = 0 ; i < w ; i++ )
						{
							index = ( pixel[ i ] >> 4 ) & 0x0f;
							image( i * 2 + 0, j ) = pixel_converter::convert_to( palette[ index ].rgbRed, palette[ index ].rgbGreen, palette[ index ].rgbBlue );
							index = ( pixel[ i ]      ) & 0x0f;
							image( i * 2 + 1, j ) = pixel_converter::convert_to( palette[ index ].rgbRed, palette[ index ].rgbGreen, palette[ index ].rgbBlue );
						}
						if( rest == 1 )
						{
							index = ( pixel[ w ] >> 4 ) & 0x0f;
							image( i * 2 + 0, j ) = pixel_converter::convert_to( palette[ index ].rgbRed, palette[ index ].rgbGreen, palette[ index ].rgbBlue );
						}
					}
					break;

				case 8:
					for( i = 0 ; i < width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( palette[ pixel[ i ] ].rgbRed, palette[ pixel[ i ] ].rgbGreen, palette[ pixel[ i ] ].rgbBlue );
					}
					break;

				case 16:
					break;

				case 24:
					for( i = 0 ; i < width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( pixel[ i * 3 + 2 ], pixel[ i * 3 + 1 ], pixel[ i * 3 + 0 ] );
					}
					break;

				case 32:
					for( i = 0 ; i < width ; i++ )
					{
						image( i, j ) = pixel_converter::convert_to( pixel[ i * 4 + 2 ], pixel[ i * 4 + 1 ], pixel[ i * 4 + 0 ] );
					}
					break;

				default:
					break;
				}
				pixel += line_bytes;
			}

			return( true );
		}

		static bool convert_to_bmp_data( const array2< T, Allocator > &image, unsigned char *bmp, size_type bmp_bits )
		{
			size_type num_bytes = get_bmp_bytes( image, bmp_bits );
			size_type color_num = get_bmp_palette_num( bmp_bits );
			size_type width = image.width( );
			size_type height = image.height( );

			// �S�Ẵo�b�t�@���N���A����
			memset( bmp, 0, sizeof( unsigned char ) * num_bytes );

			// �r�b�g�}�b�v�p�̃w�b�_�̈ʒu���w�肷��
			_bitmapfileheader_	*pfilehead  = reinterpret_cast < _bitmapfileheader_ * >	( bmp );
			_bitmapinfoheader_	*pinfohead  = reinterpret_cast < _bitmapinfoheader_ * >	( bmp + _bitmapfileheader_::bytes );
			_rgbquad_			*palette    = reinterpret_cast < _rgbquad_ * >			( bmp + _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes );

			_bitmapfileheader_	&filehead     = *pfilehead;
			_bitmapinfoheader_	&infohead     = *pinfohead;

			// BITMAPFILEHEADER�̏���ݒ�
			filehead.bfType    = 0x4d42;	//'M'*256+'B';
			filehead.bfSize    = static_cast< unsigned int >( num_bytes );
			filehead.bfOffBits = static_cast< unsigned int >( _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes + _rgbquad_::bytes * color_num );

			// BITMAPINFOHEADER�����Z�b�g
			infohead.biSize    = _bitmapinfoheader_::bytes;
			infohead.biWidth = static_cast< int >( width );
			infohead.biHeight = static_cast< int >( height );
			infohead.biPlanes = 1;
			infohead.biBitCount = static_cast< unsigned short >( bmp_bits );
			infohead.biCompression = 0;

			switch( bmp_bits )
			{
			case 1:
			case 4:
			case 8:
				create_adaptive_palette( image, palette, color_num );
				break;
			default:
				break;
			}

			size_type i, j, jj, line_bytes = get_bmp_line_strip( width, bmp_bits );
			unsigned char *pixel = bmp + _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes + _rgbquad_::bytes * color_num;
			for( jj = 0 ; jj < height ; jj++ )
			{
				j = height - jj - 1;
				switch( bmp_bits )
				{
				case 1:
					{
						size_type rest = width % 8;
						size_type w = ( width - rest ) / 8;
						for( i = 0 ; i < w ; i++ )
						{
							pixel[ i ]  = ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 0, j ), palette, color_num ) ) & 0x01 ) << 7;
							pixel[ i ] |= ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 1, j ), palette, color_num ) ) & 0x01 ) << 6;
							pixel[ i ] |= ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 2, j ), palette, color_num ) ) & 0x01 ) << 5;
							pixel[ i ] |= ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 3, j ), palette, color_num ) ) & 0x01 ) << 4;
							pixel[ i ] |= ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 4, j ), palette, color_num ) ) & 0x01 ) << 3;
							pixel[ i ] |= ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 5, j ), palette, color_num ) ) & 0x01 ) << 2;
							pixel[ i ] |= ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 6, j ), palette, color_num ) ) & 0x01 ) << 1;
							pixel[ i ] |= ( static_cast< unsigned char >( find_palette_index( image( i * 8 + 7, j ), palette, color_num ) ) & 0x01 );
						}
						unsigned char pix = 0;
						for( i = 0 ; i < rest ; i++ )
						{
							pix |= ( static_cast< unsigned char >( find_palette_index( image( w * 8 + i, j ), palette, color_num ) ) & 0x01 ) << ( 7 - i );
						}
						pixel[ w ] = pix;
					}
					break;

				case 4:
					{
						size_type rest = width % 2;
						size_type w = ( width - rest ) / 2;
						for( i = 0 ; i < w ; i++ )
						{
							pixel[ i ]  = static_cast< unsigned char >( find_palette_index( image( i * 2 + 0, j ), palette, color_num ) ) << 4;
							pixel[ i ] |= static_cast< unsigned char >( find_palette_index( image( i * 2 + 1, j ), palette, color_num ) );
						}
						if( rest == 1 )
						{
							pixel[ w ] = static_cast< unsigned char >( find_palette_index( image( w * 2 + i, j ), palette, color_num ) ) << 4;
						}
					}
					break;

				case 8:
					for( i = 0 ; i < width ; i++ )
					{
						pixel[ i ]  = static_cast< unsigned char >( find_palette_index( image( i, j ), palette, color_num ) );
					}
					break;

				case 16:
					break;

				case 24:
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						pixel[ i * 3 + 0 ] = static_cast< unsigned char >( c.b );
						pixel[ i * 3 + 1 ] = static_cast< unsigned char >( c.g );
						pixel[ i * 3 + 2 ] = static_cast< unsigned char >( c.r );
					}
					break;

				case 32:
					for( i = 0 ; i < width ; i++ )
					{
						color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
						pixel[ i * 4 + 0 ] = static_cast< unsigned char >( c.b );
						pixel[ i * 4 + 1 ] = static_cast< unsigned char >( c.g );
						pixel[ i * 4 + 2 ] = static_cast< unsigned char >( c.r );
					}
					break;

				default:
					break;
				}
				pixel += line_bytes;
			}

			return( true );
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

			bool ret = convert_from_bmp_data( buff, filesize, image );
			delete [] buff;
			return( ret );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, size_type bmp_bits )
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
			else if( !is_supported( bmp_bits ) )
			{
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			size_type size = get_bmp_bytes( image, bmp_bits );
			unsigned char *buff = new unsigned char[ size + 1 ];
			bool ret = convert_to_bmp_data( image, buff, bmp_bits );

			if( !ret )
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

//! @addtogroup image_bmp_group BMP �摜���o��
//!
//! BMP�t�@�C����ǂݍ���ŁCMIST�R���e�i�֊i�[����D
//! �܂��CMIST�R���e�i����BMP�t�@�C���ւ̏����o�����T�|�[�g�D
//! 1�C4�C8�C24�C32�r�b�g�̃r�b�g�}�b�v�ɑΉ��D
//! ���k���ꂽ�r�b�g�}�b�v�͖��T�|�[�g�D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/bmp.h>
//! @endcode
//!
//!  @{


/// @brief BMP�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @attention RLE���k�̂�������BMP�ɂ͖��Ή�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_bmp( array2< T, Allocator > &image, const std::string &filename )
{
	return( __bmp_controller__::bmp_controller< T, Allocator >::read( image, filename ) );
}


/// @brief BMP�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @attention RLE���k�̂�������BMP�ɂ͖��Ή�
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_bmp( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_bmp( image, wstr2str( filename ) ) );
}


/// @brief MIST�R���e�i�̉摜��BMP�`���Ńt�@�C���ɏo�͂���
//! 
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] bmp_bits �c �ȉ��̃r�b�g�}�b�v�̃r�b�g���̂����ꂩ
//!
//! -#  1�r�b�g �c ������2�l�r�b�g�}�b�v
//! -#  4�r�b�g �c 8�F�̃J���[�e�[�u�������r�b�g�}�b�v
//! -#  8�r�b�g �c 256�F�̃J���[�e�[�u�������r�b�g�}�b�v
//! -# 24�r�b�g �c �t���J���[�r�b�g�}�b�v
//! -# 32�r�b�g �c �t���J���[�r�b�g�}�b�v�i�A���t�@�`�����l���t�j
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_bmp( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type bmp_bits = 24 )
{
	return( __bmp_controller__::bmp_controller< T, Allocator >::write( image, filename, bmp_bits ) );
}


/// @brief MIST�R���e�i�̉摜��BMP�`���Ńt�@�C���ɏo�͂���
//! 
//! @param[in] image    �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename �c �o�̓t�@�C����
//! @param[in] bmp_bits �c �ȉ��̃r�b�g�}�b�v�̃r�b�g���̂����ꂩ
//!
//! -#  1�r�b�g �c ������2�l�r�b�g�}�b�v
//! -#  4�r�b�g �c 8�F�̃J���[�e�[�u�������r�b�g�}�b�v
//! -#  8�r�b�g �c 256�F�̃J���[�e�[�u�������r�b�g�}�b�v
//! -# 24�r�b�g �c �t���J���[�r�b�g�}�b�v
//! -# 32�r�b�g �c �t���J���[�r�b�g�}�b�v�i�A���t�@�`�����l���t�j
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_bmp( const array2< T, Allocator > &image, const std::wstring &filename, typename array2< T, Allocator >::size_type bmp_bits = 24 )
{
	return( write_bmp( image, wstr2str( filename ), bmp_bits ) );
}

/// @}
//  BMP �摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_BMP__
