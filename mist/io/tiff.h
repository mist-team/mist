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

/// @file mist/io/tiff.h
//!
//! @brief TIFF�摜��ǂݏ������邽�߂̃��C�u����
//!
//! �{���C�u�����́Chttp://www.libtiff.org/ �ŊJ�����s���Ă��� libtiff �𗘗p���Ă���
//! �G���R�[�h�ƃf�R�[�h�͖{���C�u������API��p���Ď�������Ă��邽�߁C�ʓr���C�u������p�ӂ���K�v������
//!
#ifndef __INCLUDE_MIST_TIFF__
#define __INCLUDE_MIST_TIFF__


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

#define ORIENTATION_BOTTOMLEFT 4
#include <tiffio.h>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __tiff_controller__
{
	// �O���[�X�P�[����
	template < bool b >
	struct _tiff_writer_
	{
		template < class T, class Allocator >
		static bool write( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			if( image.empty( ) )
			{
				return( false );
			}
			else if( image.width( ) == 0 )
			{
				std::cerr << "Image width is zero!" << std::endl;
				return( false );
			}
			else if( image.height( ) == 0 )
			{
				std::cerr << "Image height is zero!" << std::endl;
				return( false );
			}

			TIFF *tif;
			size_type tiffW, tiffH;

			tif = TIFFOpen( filename.c_str( ), "w" );
			if( tif == NULL )
			{
				return( false );
			}

			tiffW = image.width( );
			tiffH = image.height( );

			TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, tiffW );
			TIFFSetField( tif, TIFFTAG_IMAGELENGTH, tiffH );
			TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, 8 );
			TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );
			TIFFSetField( tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB );
			TIFFSetField( tif, TIFFTAG_DOCUMENTNAME, "MIST Project Team" );
			TIFFSetField( tif, TIFFTAG_IMAGEDESCRIPTION, "Created by MIST TIFF Conveter" );
			TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, 1 );
			TIFFSetField( tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize( tif, ( unsigned int )-1 ) );
			TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );
			if( use_lzw_compression )
			{
				// LZW���k�𗘗p���邩�ǂ���
				TIFFSetField( tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW );
			}

			size_type size = image.width( ) * image.height( );
			size_type lsize = image.width( );
			unsigned char *buf = new unsigned char[ size ];
			size_type i;

			for( i = 0 ; i < image.width( ) * image.height( ) ; i++ )
			{
				buf[ i ] = static_cast< unsigned char >( image[ i ] );
			}

			bool ret = true;
			for( i = 0 ; i < tiffH ; i++ )
			{
				if( TIFFWriteScanline( tif, buf + i * lsize, static_cast< unsigned int >( i ), 0 ) < 0 )
				{
					ret = false;
					break;
				}
			}

			delete [] buf;

			TIFFFlushData( tif );
			TIFFClose( tif );

			return( ret );
		}

		template < class Allocator >
		static bool write( const array2< unsigned short, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			typedef typename array2< unsigned short, Allocator >::size_type size_type;

			if( image.empty( ) )
			{
				return( false );
			}
			else if( image.width( ) == 0 )
			{
				std::cerr << "Image width is zero!" << std::endl;
				return( false );
			}
			else if( image.height( ) == 0 )
			{
				std::cerr << "Image height is zero!" << std::endl;
				return( false );
			}

			TIFF *tif;
			size_type tiffW, tiffH;

			tif = TIFFOpen( filename.c_str( ), "w" );
			if( tif == NULL )
			{
				return( false );
			}

			tiffW = image.width( );
			tiffH = image.height( );

			TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, tiffW );
			TIFFSetField( tif, TIFFTAG_IMAGELENGTH, tiffH );
			TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, 16 );
			TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );
			TIFFSetField( tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB );
			TIFFSetField( tif, TIFFTAG_DOCUMENTNAME, "MIST Project Team" );
			TIFFSetField( tif, TIFFTAG_IMAGEDESCRIPTION, "Created by MIST TIFF Conveter" );
			TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, 1 );
			TIFFSetField( tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize( tif, ( unsigned int )-1 ) );
			TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );
			if( use_lzw_compression )
			{
				// LZW���k�𗘗p���邩�ǂ���
				TIFFSetField( tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW );
			}

			size_type size = image.width( ) * image.height( );
			size_type lsize = image.width( );
			unsigned short *buf = new unsigned short[ size ];
			size_type i;

			for( i = 0 ; i < image.width( ) * image.height( ) ; i++ )
			{
				buf[ i ] = image[ i ];
			}

			bool ret = true;
			for( i = 0 ; i < tiffH ; i++ )
			{
				if( TIFFWriteScanline( tif, buf + i * lsize, static_cast< unsigned int >( i ), 0 ) < 0 )
				{
					ret = false;
					break;
				}
			}

			delete [] buf;

			TIFFFlushData( tif );
			TIFFClose( tif );

			return( ret );
		}
	};

	// �J���[��f��
	template < >
	struct _tiff_writer_< true >
	{
		template < class T, class Allocator >
		static bool write( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			typedef _pixel_converter_< T > pixel_converter;
			typedef typename pixel_converter::color_type color_type;
			typedef typename pixel_converter::value_type value_type;
			typedef typename array2< T, Allocator >::size_type size_type;

			if( image.empty( ) )
			{
				return( false );
			}
			else if( image.width( ) == 0 )
			{
				std::cerr << "Image width is zero!" << std::endl;
				return( false );
			}
			else if( image.height( ) == 0 )
			{
				std::cerr << "Image height is zero!" << std::endl;
				return( false );
			}

			TIFF *tif;
			size_type tiffW, tiffH;

			tif = TIFFOpen( filename.c_str( ), "w" );
			if( tif == NULL )
			{
				return( false );
			}

			tiffW = image.width( );
			tiffH = image.height( );

			TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, tiffW );
			TIFFSetField( tif, TIFFTAG_IMAGELENGTH, tiffH );
			TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, 8 );
			TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
			TIFFSetField( tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB );
			TIFFSetField( tif, TIFFTAG_DOCUMENTNAME, "MIST Project Team" );
			TIFFSetField( tif, TIFFTAG_IMAGEDESCRIPTION, "Created by MIST TIFF Conveter" );
			TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, 3 );
			TIFFSetField( tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize( tif, ( unsigned int )-1 ) );
			TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );
			if( use_lzw_compression )
			{
				// LZW���k�𗘗p���邩�ǂ���
				TIFFSetField( tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW );
			}

			size_type size = image.width( ) * image.height( ) * 3;
			size_type lsize = image.width( ) * 3;
			unsigned char *buf = new unsigned char[ size ];
			size_type i;

			for( i = 0 ; i < image.width( ) * image.height( ) ; i++ )
			{
				color_type c = limits_0_255( pixel_converter::convert_from( image[ i ] ) );
				buf[ i * 3 + 0 ] = static_cast< unsigned char >( c.r );
				buf[ i * 3 + 1 ] = static_cast< unsigned char >( c.g );
				buf[ i * 3 + 2 ] = static_cast< unsigned char >( c.b );
			}

			bool ret = true;
			for( i = 0 ; i < tiffH ; i++ )
			{
				if( TIFFWriteScanline( tif, buf + i * lsize, static_cast< unsigned int >( i ), 0 ) < 0 )
				{
					ret = false;
					break;
				}
			}

			delete [] buf;

			TIFFFlushData( tif );
			TIFFClose( tif );

			return( ret );
		}

		template < class Allocator >
		static bool write( const array2< rgb< unsigned short >, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			typedef typename array2< rgb< unsigned short >, Allocator >::size_type size_type;

			if( image.empty( ) )
			{
				return( false );
			}
			else if( image.width( ) == 0 )
			{
				std::cerr << "Image width is zero!" << std::endl;
				return( false );
			}
			else if( image.height( ) == 0 )
			{
				std::cerr << "Image height is zero!" << std::endl;
				return( false );
			}

			TIFF *tif;
			size_type tiffW, tiffH;

			tif = TIFFOpen( filename.c_str( ), "w" );
			if( tif == NULL )
			{
				return( false );
			}

			tiffW = image.width( );
			tiffH = image.height( );

			TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, tiffW );
			TIFFSetField( tif, TIFFTAG_IMAGELENGTH, tiffH );
			TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, 16 );
			TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
			TIFFSetField( tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB );
			TIFFSetField( tif, TIFFTAG_DOCUMENTNAME, "MIST Project Team" );
			TIFFSetField( tif, TIFFTAG_IMAGEDESCRIPTION, "Created by MIST TIFF Conveter" );
			TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, 3 );
			TIFFSetField( tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize( tif, ( unsigned int )-1 ) );
			TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );
			if( use_lzw_compression )
			{
				// LZW���k�𗘗p���邩�ǂ���
				TIFFSetField( tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW );
			}

			size_type size = image.width( ) * image.height( ) * 3;
			size_type lsize = image.width( ) * 3;
			unsigned short *buf = new unsigned short[ size ];
			size_type i;

			for( i = 0 ; i < image.width( ) * image.height( ) ; i++ )
			{
				buf[ i * 3 + 0 ] = image[ i ].r;
				buf[ i * 3 + 1 ] = image[ i ].g;
				buf[ i * 3 + 2 ] = image[ i ].b;
			}

			bool ret = true;
			for( i = 0 ; i < tiffH ; i++ )
			{
				if( TIFFWriteScanline( tif, buf + i * lsize, static_cast< unsigned int >( i ), 0 ) < 0 )
				{
					ret = false;
					break;
				}
			}

			delete [] buf;

			TIFFFlushData( tif );
			TIFFClose( tif );

			return( ret );
		}

		template < class Allocator >
		static bool write( const array2< rgba< unsigned short >, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			typedef typename array2< rgba< unsigned short >, Allocator >::size_type size_type;

			if( image.empty( ) )
			{
				return( false );
			}
			else if( image.width( ) == 0 )
			{
				std::cerr << "Image width is zero!" << std::endl;
				return( false );
			}
			else if( image.height( ) == 0 )
			{
				std::cerr << "Image height is zero!" << std::endl;
				return( false );
			}

			TIFF *tif;
			size_type tiffW, tiffH;

			tif = TIFFOpen( filename.c_str( ), "w" );
			if( tif == NULL )
			{
				return( false );
			}

			tiffW = image.width( );
			tiffH = image.height( );

			TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, tiffW );
			TIFFSetField( tif, TIFFTAG_IMAGELENGTH, tiffH );
			TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, 16 );
			TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
			TIFFSetField( tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB );
			TIFFSetField( tif, TIFFTAG_DOCUMENTNAME, "MIST Project Team" );
			TIFFSetField( tif, TIFFTAG_IMAGEDESCRIPTION, "Created by MIST TIFF Conveter" );
			TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, 3 );
			TIFFSetField( tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize( tif, ( unsigned int )-1 ) );
			TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );
			if( use_lzw_compression )
			{
				// LZW���k�𗘗p���邩�ǂ���
				TIFFSetField( tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW );
			}

			size_type size = image.width( ) * image.height( ) * 3;
			size_type lsize = image.width( ) * 3;
			unsigned short *buf = new unsigned short[ size ];
			size_type i;

			for( i = 0 ; i < image.width( ) * image.height( ) ; i++ )
			{
				buf[ i * 3 + 0 ] = image[ i ].r;
				buf[ i * 3 + 1 ] = image[ i ].g;
				buf[ i * 3 + 2 ] = image[ i ].b;
			}

			bool ret = true;
			for( i = 0 ; i < tiffH ; i++ )
			{
				if( TIFFWriteScanline( tif, buf + i * lsize, static_cast< unsigned int >( i ), 0 ) < 0 )
				{
					ret = false;
					break;
				}
			}

			delete [] buf;

			TIFFFlushData( tif );
			TIFFClose( tif );

			return( ret );
		}
	};

	template < class T, class Allocator >
	struct tiff_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename pixel_converter::value_type value_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			int				cols, rows;
			TIFF			*tif;
			unsigned short	bps, spp, photomet;
			unsigned short	*redcolormap;
			unsigned short	*greencolormap;
			unsigned short	*bluecolormap;

			tif = TIFFOpen( filename.c_str( ), "r" );
			if( tif == NULL )
			{
				std::cerr << "Can't Open [" << filename << "]!!" << std::endl;
				return( false );
			}

			if( !TIFFGetField( tif, TIFFTAG_BITSPERSAMPLE  , &bps ) )
			{
				bps = 1;
			}
			if( !TIFFGetField( tif, TIFFTAG_SAMPLESPERPIXEL, &spp ) )
			{
				spp = 1;
			}
			if( !TIFFGetField( tif, TIFFTAG_PHOTOMETRIC    , &photomet ) )
			{
				std::cerr << "error getting photometric" << std::endl;
			}

			switch( spp )
			{
			case 1:
			case 3:
			case 4:
				break;

			default:
				std::cerr << "can only handle 1-channel gray scale or 1- or 3-channel color" << std::endl;
				break;
			}

			TIFFGetField( tif, TIFFTAG_IMAGEWIDTH, &cols );
			TIFFGetField( tif, TIFFTAG_IMAGELENGTH, &rows );

			int maxval = ( 1 << bps ) - 1;

			if( photomet == PHOTOMETRIC_PALETTE )
			{
				// �J���[�p���b�g�𗘗p����ꍇ�̓p���b�g��ǂݍ���
				if( !TIFFGetField( tif, TIFFTAG_COLORMAP, &redcolormap, &greencolormap, &bluecolormap ) )
				{
					std::cerr << "error getting colormaped" << std::endl;
				}
			}

			unsigned char *buf = new unsigned char[ TIFFScanlineSize( tif ) ];

			if( buf == NULL )
			{
				std::cerr << "can't allocate memory for scanline buffer" << std::endl;
			}

			size_type width = cols;
			size_type height = rows;

			bool isBigEndian = TIFFIsBigEndian( tif ) != 0;

			image.resize( width, height );

			for( size_type j = 0 ; j < height ; ++j )
			{
				if( TIFFReadScanline( tif, buf, static_cast< unsigned int >( j ), 0 ) < 0 )
				{
					std::cerr << "bad data read on line " << j << std::endl;
				}

				switch( photomet )
				{
				case PHOTOMETRIC_MINISBLACK:
					if( bps == 16 )
					{
						if( isBigEndian )
						{
							for( size_type i = 0 ; i < width ; ++i )
							{
								int sample = ( ( buf[ i * 2 ] << 8 ) | ( buf[ i * 2 + 1 ] ) ) & maxval;
								image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( sample ), static_cast< value_type >( sample ), static_cast< value_type >( sample ) );
							}
						}
						else
						{
							for( size_type i = 0 ; i < width ; ++i )
							{
								int sample = ( ( buf[ i * 2 ] ) | ( buf[ i * 2 + 1 ] << 8 ) ) & maxval;
								image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( sample ), static_cast< value_type >( sample ), static_cast< value_type >( sample ) );
							}
						}
					}
					else
					{
						for( size_type i = 0 ; i < width ; ++i )
						{
							unsigned char sample = buf[ i ] & maxval;
							image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( sample ), static_cast< value_type >( sample ), static_cast< value_type >( sample ) );
						}
					}
					break;

				case PHOTOMETRIC_MINISWHITE:
					if( bps == 16 )
					{
						if( isBigEndian )
						{
							for( size_type i = 0 ; i < width ; ++i )
							{
								int sample = maxval - ( ( ( buf[ i * 2 ] << 8 ) | ( buf[ i * 2 + 1 ] ) ) & maxval );
								image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( sample ), static_cast< value_type >( sample ), static_cast< value_type >( sample ) );
							}
						}
						else
						{
							for( size_type i = 0 ; i < width ; ++i )
							{
								int sample = ( ( buf[ i * 2 ] ) | ( buf[ i * 2 + 1 ] << 8 ) ) & maxval;
								image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( sample ), static_cast< value_type >( sample ), static_cast< value_type >( sample ) );
							}
						}
					}
					else
					{
						for( size_type i = 0 ; i < width ; ++i )
						{
							int sample = maxval - ( buf[ i ] & maxval );
							image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( sample ), static_cast< value_type >( sample ), static_cast< value_type >( sample ) );
						}
					}
					break;

				case PHOTOMETRIC_PALETTE:
					for( size_type i = 0 ; i < width ; ++i )
					{
						unsigned char sample = buf[ i ] & maxval;
						image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( redcolormap[ sample ] ),
																	 static_cast< value_type >( greencolormap[ sample ] ),
																	 static_cast< value_type >( bluecolormap[ sample ] ) );
					}
					break;

				case PHOTOMETRIC_RGB:
					if( spp == 3 )
					{
						unsigned char *p = buf;
						if( bps == 16 )
						{
							if( isBigEndian )
							{
								for( size_type i = 0 ; i < width ; ++i, p += 6 )
								{
									unsigned short r = ( ( p[ 0 ] << 8 ) | p[ 1 ] ) & maxval;
									unsigned short g = ( ( p[ 2 ] << 8 ) | p[ 3 ] ) & maxval;
									unsigned short b = ( ( p[ 4 ] << 8 ) | p[ 5 ] ) & maxval;
									image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
								}
							}
							else
							{
								for( size_type i = 0 ; i < width ; ++i, p += 6 )
								{
									unsigned short r = ( p[ 0 ] | ( p[ 1 ] << 8 ) ) & maxval;
									unsigned short g = ( p[ 2 ] | ( p[ 3 ] << 8 ) ) & maxval;
									unsigned short b = ( p[ 4 ] | ( p[ 5 ] << 8 ) ) & maxval;
									image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
								}
							}
						}
						else
						{
							for( size_type i = 0 ; i < width ; ++i )
							{
								unsigned char r = *p++;
								unsigned char g = *p++;
								unsigned char b = *p++;
								image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
							}
						}
					}
					else
					{
						unsigned char *p = buf;
						if( bps == 16 )
						{
							if( isBigEndian )
							{
								for( size_type i = 0 ; i < width ; ++i, p += 8 )
								{
									unsigned short r = ( ( p[ 0 ] << 8 ) | p[ 1 ] ) & maxval;
									unsigned short g = ( ( p[ 2 ] << 8 ) | p[ 3 ] ) & maxval;
									unsigned short b = ( ( p[ 4 ] << 8 ) | p[ 5 ] ) & maxval;
									image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
								}
							}
							else
							{
								for( size_type i = 0 ; i < width ; ++i, p += 8 )
								{
									unsigned short r = ( p[ 0 ] | ( p[ 1 ] << 8 ) ) & maxval;
									unsigned short g = ( p[ 2 ] | ( p[ 3 ] << 8 ) ) & maxval;
									unsigned short b = ( p[ 4 ] | ( p[ 5 ] << 8 ) ) & maxval;
									image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
								}
							}
						}
						else
						{
							for( size_type i = 0 ; i < width ; ++i )
							{
								unsigned char r = *p++;
								unsigned char g = *p++;
								unsigned char b = *p++;
								image( i, j ) = pixel_converter::convert_to( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
								p++;
							}
						}
					}
					break;

				default:
					break;
				}
			}

			delete [] buf;
			TIFFClose( tif );

			return( true );
		}

		static bool write( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression )
		{
			return( _tiff_writer_< is_color< T >::value >::write( image, filename, use_lzw_compression ) );
		}
	};
}

//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_tiff_group TIFF�摜���o��
//!
//! TIFF�t�@�C����ǂݍ���ŁCMIST�R���e�i�֊i�[����D
//! �܂��CMIST�R���e�i����TIFF�t�@�C���ւ̏����o�����T�|�[�g�D
//! �ʓr http://libtiff.org ���񋟂���t���[��TIFF���C�u�������K�v�ł��D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/tiff.h>
//! @endcode
//!
//!  @{


/// @brief TIFF�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! LZW���k�̂�������TIFF��J���[�e�[�u�����g�p����TIFF�t�@�C�����̂��܂��܂ȃt�H�[�}�b�g��ǂݍ��ނ��Ƃ��\�D
//! �܂��C�o�͂���MIST�R���e�i���J���[�摜���O���[�X�P�[���摜���ɂ���āC�s�N�Z���f�[�^��K�؂ɕϊ�����
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_tiff( array2< T, Allocator > &image, const std::string &filename )
{
	return( __tiff_controller__::tiff_controller< T, Allocator >::read( image, filename ) );
}


/// @brief TIFF�摜��MIST�R���e�i�ɓǂݍ���
//! 
//! LZW���k�̂�������TIFF��J���[�e�[�u�����g�p����TIFF�t�@�C�����̂��܂��܂ȃt�H�[�}�b�g��ǂݍ��ނ��Ƃ��\�D
//! �܂��C�o�͂���MIST�R���e�i���J���[�摜���O���[�X�P�[���摜���ɂ���āC�s�N�Z���f�[�^��K�؂ɕϊ�����
//! 
//! @param[out] image    �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename �c ���̓t�@�C����
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_tiff( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_tiff( image, wstr2str( filename ) ) );
}


/// @brief MIST�R���e�i�̉摜��TIFF�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention LZW���k�̂�������TIFF�摜���o�͂���ꍇ�́Clibtiff ���C�u��������LZW���k���L���ɂȂ��Ă���K�v������
//! @attention �f�[�^�̌^��unsigned short�Crgb< unsigned short>�Crgba< unsigned short>�̏ꍇ�́A16�r�b�g�`�����l�����g�p���ĉ摜���ۑ������
//! 
//! @param[in] image               �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename            �c �o�̓t�@�C����
//! @param[in] use_lzw_compression �c LZW���k���ꂽTIFF�摜���o�͂��邩�ǂ���
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_tiff( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression = _LZW_COMPRESSION_SUPPORT_ )
{
	return( __tiff_controller__::tiff_controller< T, Allocator >::write( image, filename, use_lzw_compression ) );
}


/// @brief MIST�R���e�i�̉摜��TIFF�`���Ńt�@�C���ɏo�͂���
//! 
//! @attention LZW���k�̂�������TIFF�摜���o�͂���ꍇ�́Clibtiff ���C�u��������LZW���k���L���ɂȂ��Ă���K�v������
//! @attention �f�[�^�̌^��unsigned short�Crgb< unsigned short>�Crgba< unsigned short>�̏ꍇ�́A16�r�b�g�`�����l�����g�p���ĉ摜���ۑ������
//! 
//! @param[in] image               �c �o�͉摜��ێ�����MIST�R���e�i
//! @param[in] filename            �c �o�̓t�@�C����
//! @param[in] use_lzw_compression �c LZW���k���ꂽTIFF�摜���o�͂��邩�ǂ���
//!
//! @retval true  �c �摜�̏������݂ɐ���
//! @retval false �c �摜�̏������݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_tiff( const array2< T, Allocator > &image, const std::wstring &filename, bool use_lzw_compression = _LZW_COMPRESSION_SUPPORT_ )
{
	return( write_tiff( image, wstr2str( filename ), use_lzw_compression ) );
}


/// @}
//  TIFF�摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_TIFF__
