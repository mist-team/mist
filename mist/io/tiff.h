#ifndef __INCLUDE_MIST_TIFF__
#define __INCLUDE_MIST_TIFF__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#include <iostream>
#include <string>

#define ORIENTATION_BOTTOMLEFT 4
#include <tiffio.h>


// mist名前空間の始まり
_MIST_BEGIN


namespace __tiff_controller__
{
	template < class T, class Allocator >
	struct tiff_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			int						cols, rows;
			register TIFF			*tif;
			register unsigned char*	inP;
			register unsigned char	sample, r, g, b;
			register int			bitsleft;
			unsigned short			bps, spp, photomet;
			unsigned short			*redcolormap;
			unsigned short			*greencolormap;
			unsigned short			*bluecolormap;

			tif = TIFFOpen( filename.c_str( ), "r" );
			if( tif == NULL )
			{
				std::cerr << "Can't Open [" << filename << "]!!" << std::endl;
				return( false );
			}
			if( !TIFFGetField( tif, TIFFTAG_BITSPERSAMPLE, &bps ) ) bps = 1;
			if( !TIFFGetField( tif, TIFFTAG_SAMPLESPERPIXEL, &spp ) ) spp = 1;
			if( !TIFFGetField( tif, TIFFTAG_PHOTOMETRIC, &photomet ) ) std::cerr << "error getting photometric" << std::endl;

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

			unsigned char *buf = ( unsigned char* )malloc( TIFFScanlineSize( tif ) );

			if( buf == NULL )
			{
				std::cerr << "can't allocate memory for scanline buffer" << std::endl;
			}

			unsigned int i, j;
			size_type width = cols;
			size_type height = rows;

			image.resize( width, height );

			for( j = 0 ; j < height ; ++j )
			{
				if( TIFFReadScanline( tif, buf, j, 0 ) < 0 )
				{
					std::cerr << "bad data read on line " << j << std::endl;
				}

				inP = buf;
				bitsleft = 8;

				switch( photomet )
				{
				case PHOTOMETRIC_MINISBLACK:
					for( i = 0 ; i < width ; ++i )
					{
						if( bitsleft == 0 )
						{
							++inP;
							bitsleft = 8;
						}
						bitsleft -= bps;
						sample = ( *inP >> bitsleft ) & maxval;
						image( i, j ) = pixel_converter::convert_to( sample, sample, sample );
					}
					break;

				case PHOTOMETRIC_MINISWHITE:
					for( i = 0 ; i < width ; ++i )
					{
						if( bitsleft == 0 )
						{
							++inP;
							bitsleft = 8;
						}
						bitsleft -= bps;
						sample = maxval - ( ( *inP >> bitsleft ) & maxval );
						image( i, j ) = pixel_converter::convert_to( sample, sample, sample );
					}
					break;

				case PHOTOMETRIC_PALETTE:
					for( i = 0 ; i < width ; ++i )
					{
						if( bitsleft == 0 )
						{
							++inP;
							bitsleft = 8;
						}
						bitsleft -= bps;
						sample = ( *inP >> bitsleft ) & maxval;

						image( i, j ) = pixel_converter::convert_to( static_cast< unsigned char >( redcolormap[sample] ),
																			static_cast< unsigned char >( greencolormap[sample] ),
																			static_cast< unsigned char >( bluecolormap[sample] ) );
					}
					break;

				case PHOTOMETRIC_RGB:
					for( i = 0 ; i < width ; ++i )
					{
						// register long r, g, b;
						if( bitsleft == 0 )
						{
							++inP;
							bitsleft = 8;
						}
						bitsleft -= bps;
						r = static_cast< unsigned char >( (*inP >> bitsleft) & maxval );

						if( bitsleft == 0 )
						{
							++inP;
							bitsleft = 8;
						}
						bitsleft -= bps;
						g = static_cast< unsigned char >( (*inP >> bitsleft) & maxval );

						if( bitsleft == 0 )
						{
							++inP;
							bitsleft = 8;
						}
						bitsleft -= bps;
						b = static_cast< unsigned char >( (*inP >> bitsleft) & maxval );

						image( i, j ) = pixel_converter::convert_to( r, g, b );

						if( spp == 4 )
						{
							/* skip alpha channel */
							if( bitsleft == 0 )
							{
								++inP;
								bitsleft = 8;
							}
							bitsleft -= bps;
						}
					}
					break;
				default:
					break;
				}
			}

			free( buf );
			TIFFClose( tif );

			return( true );
		}

		static bool write( array2< T, Allocator > &image, const std::string &filename )
		{
			TIFF *tif;
			size_type tiffW, tiffH;
			size_type rowsPerStrip;

			tif = TIFFOpen( filename.c_str( ), "w" );
			if( tif == NULL ) return(false);

			tiffW = image.width( );
			tiffH = image.height( );

			rowsPerStrip = ( 8 * 1024 ) / ( 3 * tiffW );
			if( rowsPerStrip == 0 ) rowsPerStrip = 1;

			TIFFSetField( tif, TIFFTAG_IMAGEWIDTH, tiffW );
			TIFFSetField( tif, TIFFTAG_IMAGELENGTH, tiffH );
			TIFFSetField( tif, TIFFTAG_BITSPERSAMPLE, 8 );
//			TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW );
			TIFFSetField( tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB );
			TIFFSetField( tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB );
			TIFFSetField( tif, TIFFTAG_DOCUMENTNAME, "mist project team" );
			TIFFSetField( tif, TIFFTAG_IMAGEDESCRIPTION, "Created by mist tiff conveter" );
			TIFFSetField( tif, TIFFTAG_SAMPLESPERPIXEL, 3 );
			TIFFSetField( tif, TIFFTAG_ROWSPERSTRIP, rowsPerStrip );
			TIFFSetField( tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );

			size_type size = image.width( ) * image.height( ) * 3;
			size_type lsize = image.width( ) * 3;
			unsigned char *buf = new unsigned char[size];
			size_type i;

			for( i = 0 ; i < image.width( ) * image.height( ) ; i++ )
			{
				color_type c = pixel_converter::convert_from( image[i] );
				buf[ i * 3 + 0 ] = c.r;
				buf[ i * 3 + 1 ] = c.g;
				buf[ i * 3 + 2 ] = c.b;
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
}


template < class T, class Allocator >
bool read_tiff( array2< T, Allocator > &image, const std::string &filename )
{
	return( __tiff_controller__::tiff_controller< T, Allocator >::read( image, filename ) );
}

template < class T, class Allocator >
bool write_tiff( array2< T, Allocator > &image, const std::string &filename )
{
	return( __tiff_controller__::tiff_controller< T, Allocator >::write( image, filename ) );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_TIFF__
