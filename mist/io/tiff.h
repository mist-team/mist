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
//! @brief TIFF画像を読み書きするためのライブラリ
//!
//! 本ライブラリは，http://www.libtiff.org/ で開発が行われている libtiff を利用している
//! エンコードとデコードは本ライブラリのAPIを用いて実装されているため，別途ライブラリを用意する必要がある
//!
#ifndef __INCLUDE_MIST_TIFF__
#define __INCLUDE_MIST_TIFF__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
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


// mist名前空間の始まり
_MIST_BEGIN


namespace __tiff_controller__
{
	// グレースケール版
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
				// LZW圧縮を利用するかどうか
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
				// LZW圧縮を利用するかどうか
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

	// カラー画素版
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
				// LZW圧縮を利用するかどうか
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
				// LZW圧縮を利用するかどうか
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
				// LZW圧縮を利用するかどうか
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
				// カラーパレットを利用する場合はパレットを読み込む
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

//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_tiff_group TIFF画像入出力
//!
//! TIFFファイルを読み込んで，MISTコンテナへ格納する．
//! また，MISTコンテナからTIFFファイルへの書き出しをサポート．
//! 別途 http://libtiff.org が提供するフリーのTIFFライブラリが必要です．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/tiff.h>
//! @endcode
//!
//!  @{


/// @brief TIFF画像をMISTコンテナに読み込む
//! 
//! LZW圧縮のかかったTIFFやカラーテーブルを使用したTIFFファイル等のさまざまなフォーマットを読み込むことが可能．
//! また，出力するMISTコンテナがカラー画像かグレースケール画像かによって，ピクセルデータを適切に変換する
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_tiff( array2< T, Allocator > &image, const std::string &filename )
{
	return( __tiff_controller__::tiff_controller< T, Allocator >::read( image, filename ) );
}


/// @brief TIFF画像をMISTコンテナに読み込む
//! 
//! LZW圧縮のかかったTIFFやカラーテーブルを使用したTIFFファイル等のさまざまなフォーマットを読み込むことが可能．
//! また，出力するMISTコンテナがカラー画像かグレースケール画像かによって，ピクセルデータを適切に変換する
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_tiff( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_tiff( image, wstr2str( filename ) ) );
}


/// @brief MISTコンテナの画像をTIFF形式でファイルに出力する
//! 
//! @attention LZW圧縮のかかったTIFF画像を出力する場合は，libtiff ライブラリ側でLZW圧縮が有効になっている必要がある
//! @attention データの型がunsigned short，rgb< unsigned short>，rgba< unsigned short>の場合は、16ビットチャンネルを使用して画像が保存される
//! 
//! @param[in] image               … 出力画像を保持するMISTコンテナ
//! @param[in] filename            … 出力ファイル名
//! @param[in] use_lzw_compression … LZW圧縮されたTIFF画像を出力するかどうか
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_tiff( const array2< T, Allocator > &image, const std::string &filename, bool use_lzw_compression = _LZW_COMPRESSION_SUPPORT_ )
{
	return( __tiff_controller__::tiff_controller< T, Allocator >::write( image, filename, use_lzw_compression ) );
}


/// @brief MISTコンテナの画像をTIFF形式でファイルに出力する
//! 
//! @attention LZW圧縮のかかったTIFF画像を出力する場合は，libtiff ライブラリ側でLZW圧縮が有効になっている必要がある
//! @attention データの型がunsigned short，rgb< unsigned short>，rgba< unsigned short>の場合は、16ビットチャンネルを使用して画像が保存される
//! 
//! @param[in] image               … 出力画像を保持するMISTコンテナ
//! @param[in] filename            … 出力ファイル名
//! @param[in] use_lzw_compression … LZW圧縮されたTIFF画像を出力するかどうか
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_tiff( const array2< T, Allocator > &image, const std::wstring &filename, bool use_lzw_compression = _LZW_COMPRESSION_SUPPORT_ )
{
	return( write_tiff( image, wstr2str( filename ), use_lzw_compression ) );
}


/// @}
//  TIFF画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIFF__
