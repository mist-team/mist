#ifndef __INCLUDE_MIST_BMP__
#define __INCLUDE_MIST_BMP__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <string>

#include <deque>
#include <algorithm>


// mist名前空間の始まり
_MIST_BEGIN


namespace __bmp_controller__
{
	template < class T >
	union byte_array
	{
		typedef T value_type;
		value_type value;
		unsigned char byte[ sizeof( value_type ) ];
		byte_array( const value_type &v = 0 ) : value( v ){ }
	};

	struct _rgbquad_
	{
		enum{ bytes = 4 };
		unsigned char	rgbBlue;
		unsigned char	rgbGreen;
		unsigned char	rgbRed;
		unsigned char	rgbReserved;
	};

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
	};

	struct _bitmapinfo_
	{
		enum{ bytes = _rgbquad_::bytes + _bitmapinfoheader_::bytes };
		_bitmapinfoheader_	bmiHeader;
		_rgbquad_			bmiColors[1];
	};

	struct _bitmapfileheader_
	{
		enum{ bytes = 14 };
		unsigned short	bfType;
		unsigned int	bfSize;
		unsigned short	bfReserved1;
		unsigned short	bfReserved2;
		unsigned int	bfOffBits;
	};

	// 最も基本となるパレットを作成する
	inline void create_default_palette( _rgbquad_ palette[256] )
	{
		int i, j, k;

		for( i = 0 ; i < 6 ; i++ ) /* カラーテーブル作成 */
		{
			for( j = 0 ; j < 6 ; j++ )
			{
				for( k = 0 ; k < 6 ; k++ )
				{	
					palette[ i * 36 + j * 6 + k ].rgbRed   = k * 51;
					palette[ i * 36 + j * 6 + k ].rgbGreen = j * 51;
					palette[ i * 36 + j * 6 + k ].rgbBlue  = i * 51;
				}
			}
		}
		for( i = 1 ; i < 32 ; i++ )
		{
			palette[ i + 223 ].rgbRed   = i * 8;
			palette[ i + 223 ].rgbGreen = i * 8;
			palette[ i + 223 ].rgbBlue  = i * 8;
		}
		palette[255].rgbRed   = 255;
		palette[255].rgbGreen = 255;
		palette[255].rgbBlue  = 255;
	}

	template < class T, class Allocator >
	struct bmp_controller
	{
		typedef typename array2< T, Allocator >::size_type  size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
			array2< rgb< T >, Allocator > img;
			bool ret = bmp_controller< rgb< T >, Allocator >::read( img, filename );
			if( !ret ) return( false );

			typename array2< T, Allocator >::size_type i;
			image.resize( img.width( ), img.height( ) );
			for( i = 0 ; i < img.size( ) ; i++ )
			{
				image[i] = img[i].get_value( );
			}

			return( true );
		}

		static bool write( array2< T, Allocator > &image, const std::string &filename, size_type bmp_bits )
		{
			array2< rgb< T >, Allocator > img( image.width( ), image.height( ) );
			typename array2< T, Allocator >::size_type i;
			for( i = 0 ; i < img.size( ) ; i++ )
			{
				img[i].r = img[i].get_value( );
				img[i].g = img[i].get_value( );
				img[i].b = img[i].get_value( );
			}

			return( bmp_controller< rgb< T >, Allocator >::write( img, filename, quality ) );
		}
	};

	template < class T, class Allocator >
	struct bmp_controller< rgb< T >, Allocator >
	{
		typedef typename array2< T, Allocator >::size_type  size_type;

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

		static size_type get_bmp_bytes( const array2< rgb< T >, Allocator > &image, size_type bmp_bits )
		{
			return( _bitmapfileheader_::bytes + _bitmapinfo_::bytes + _rgbquad_::bytes + image.height( ) * get_bmp_line_strip( image.width( ), bmp_bits ) );
		}


		class __PALETTE__
		{
		public:
			unsigned char red;
			unsigned char green;
			unsigned char blue;

			const __PALETTE__ &operator =( const __PALETTE__ &c )
			{
				if( this == &c ) return( *this );
				red = c.red;
				green = c.green;
				blue = c.blue;
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
			__PALETTE__( unsigned char r = 0, unsigned char g = 0, unsigned char b = 0 ) : red( r ), green( g ), blue( b ){ }
		};

		static void eliminate_palette( std::deque< __PALETTE__ > &palette, size_type palette_num )
		{
			if( palette.size( ) == palette_num ) return;
			if( palette.size( ) < palette_num )
			{
				size_type num = palette_num - palette.size( );
				for( size_type i = 0 ; i < num ; i++ ) palette.push_back( __PALETTE__( ) );
				return;
			}

			long  i, j;

			size_type current_size = palette.size( );
			long num = static_cast< long >( palette.size( ) );

			unsigned int *diff = new unsigned int[ num * num ];

			while( palette.size( ) > palette_num )
			{
				memset( diff, 0xffffffff, sizeof( unsigned int ) * num * num );

				for( i = 1 ; i < num ; i++ )
				{
					for( j = i + 1 ; j < num ; j++ )
					{
						diff[ i + j * num ] = diff[ j + i * num ] =   ( palette[i].red - palette[j].red )     * ( palette[i].red - palette[j].red )
																	+ ( palette[i].green - palette[j].green ) * ( palette[i].green - palette[j].green )
																	+ ( palette[i].blue - palette[j].blue )   * ( palette[i].blue - palette[j].blue );
					}
				}

				unsigned int global_min = 0xffffffff;
				unsigned int min;
				long index1 = -1, index2 = -1, iii;

				for( i = 1 ; i < num ; i++ )
				{
					min = 0xffffffff;
					iii = -1;
					for( j = 1 ; j < num ; j++ )
					{
						if( i == j ) continue;
						if( iii < 0 )
						{
							iii = j;
							min = diff[ i + j * num ];
							continue;
						}
						if( min > diff[ i + j * num ] )
						{
							iii = j;
							min = diff[ i + j * num ];
						}
					}

					if( index1 < 0 )
					{
						index1 = i;
						index2 = iii;
						global_min = min;
						continue;
					}
					if( global_min > min )
					{
						index1 = i;
						index2 = iii;
						global_min = min;
					}
				}

				palette[index1].red   = ( palette[index1].red   + palette[index2].red   ) / 2;
				palette[index1].green = ( palette[index1].green + palette[index2].green ) / 2;
				palette[index1].blue  = ( palette[index1].blue  + palette[index2].blue  ) / 2;
				palette.erase( palette.begin( ) + index2 );
			}
		}

		// カラーパレットを作成
		static void create_adaptive_palette( const array2< rgb< T >, Allocator > &image, unsigned char *bitmap, _rgbquad_ *palette, size_type palette_num )
		{
			size_type width = image.width( );
			size_type height = image.height( );

			_rgbquad_ default_palette[256];
			create_default_palette( default_palette );

			std::deque< __PALETTE__ > adaptive_palette;


			unsigned char r, g, b, pr, pg, pb;
			double distance, minimum;
			unsigned char index, k;
			size_type i, j;

			for( i = 0 ; i < width * height ; i++ )
			{
				r = static_cast< unsigned char >( image[i].r );
				g = static_cast< unsigned char >( image[i].g );
				b = static_cast< unsigned char >( image[i].b );
				adaptive_palette.push_back( __PALETTE__( r, g, b ) );
			}

			eliminate_palette( adaptive_palette, palette_num );

			// パレットを書き出す
			for( i = 0 ; i < palette_num ; i++ )
			{
				palette[i].rgbRed      = adaptive_palette[i].red;
				palette[i].rgbGreen    = adaptive_palette[i].green;
				palette[i].rgbBlue     = adaptive_palette[i].blue;
				palette[i].rgbReserved = 0;
			}

			for( j = 0 ; j < height ; j++ )
			{
				for( i = 0 ; i < width ; i++ )
				{
					r = static_cast< unsigned char >( image( i, j ).r );
					g = static_cast< unsigned char >( image( i, j ).g );
					b = static_cast< unsigned char >( image( i, j ).b );

					index = 0;
					/* 最も近い色を検索 */
					pr = adaptive_palette[0].red;
					pg = adaptive_palette[0].green;
					pb = adaptive_palette[0].blue;

					/* ピクセルとカラーテーブル要素の距離計算 */
					minimum = ( r - pr ) * ( r - pr ) + ( g - pg ) * ( g - pg ) + ( b - pb ) * ( b - pb );

					for( k = 1 ; k < adaptive_palette.size( ) ; k++ )
					{
						/* 最も近い色を検索 */
						pr = adaptive_palette[0].red;
						pg = adaptive_palette[0].green;
						pb = adaptive_palette[0].blue;

						/* ピクセルとカラーテーブル要素の距離計算 */
						distance = ( r - pr ) * ( r - pr ) + ( g - pg ) * ( g - pg ) + ( b - pb ) * ( b - pb );

						/* 色空間内の距離が最小の色を保存 */
						if( minimum > distance )
						{
							/* これまでで距離最小の色 */
							minimum = distance; /* 最小値保存 */
							index = k; /* カラーテーブル番号保存 */
						}
					}
					bitmap[ i + j * width ] = index; // ピクセルが参照するカラーテーブルを決定
				}
			}
		}

		static bool convert_from_bmp_data( unsigned char *bmp, array2< rgb< T >, Allocator > &image )
		{
			// ビットマップ用のヘッダの位置を指定する
			_bitmapfileheader_	*pfilehead  = reinterpret_cast < _bitmapfileheader_ * >	( bmp );
			_bitmapinfoheader_	*pinfohead  = reinterpret_cast < _bitmapinfoheader_ * >	( bmp + _bitmapfileheader_::bytes );
			_rgbquad_			*palette    = reinterpret_cast < _rgbquad_ * >			( bmp + _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes );

			_bitmapfileheader_	&filehead     = *pfilehead;
			_bitmapinfoheader_	&infohead     = *pinfohead;

			size_type bmp_bits = infohead.biBitCount;
			size_type width    = infohead.biWidth;
			size_type height   = infohead.biHeight;

			size_type num_bytes = get_bmp_bytes( image, bmp_bits );
			size_type color_num = get_bmp_palette_num( bmp_bits );

			if( infohead.biCompression != BI_RGB )
			{
				// 圧縮のかかったビットマップは未サポート
				return( false );
			}

			image.resize( width, height );

			size_type i, j, jj, line_bytes = get_bmp_line_bytes( width, bmp_bits );
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
							image( i * 8 + 0, j ).r = palette[ ( pixel[ i ] >> 7 ) & 0x01 ].rgbRed;
							image( i * 8 + 0, j ).g = palette[ ( pixel[ i ] >> 7 ) & 0x01 ].rgbGreen;
							image( i * 8 + 0, j ).b = palette[ ( pixel[ i ] >> 7 ) & 0x01 ].rgbBlue;
							image( i * 8 + 1, j ).r = palette[ ( pixel[ i ] >> 6 ) & 0x01 ].rgbRed;
							image( i * 8 + 1, j ).g = palette[ ( pixel[ i ] >> 6 ) & 0x01 ].rgbGreen;
							image( i * 8 + 1, j ).b = palette[ ( pixel[ i ] >> 6 ) & 0x01 ].rgbBlue;
							image( i * 8 + 2, j ).r = palette[ ( pixel[ i ] >> 5 ) & 0x01 ].rgbRed;
							image( i * 8 + 2, j ).g = palette[ ( pixel[ i ] >> 5 ) & 0x01 ].rgbGreen;
							image( i * 8 + 2, j ).b = palette[ ( pixel[ i ] >> 5 ) & 0x01 ].rgbBlue;
							image( i * 8 + 3, j ).r = palette[ ( pixel[ i ] >> 4 ) & 0x01 ].rgbRed;
							image( i * 8 + 3, j ).g = palette[ ( pixel[ i ] >> 4 ) & 0x01 ].rgbGreen;
							image( i * 8 + 3, j ).b = palette[ ( pixel[ i ] >> 4 ) & 0x01 ].rgbBlue;
							image( i * 8 + 4, j ).r = palette[ ( pixel[ i ] >> 3 ) & 0x01 ].rgbRed;
							image( i * 8 + 4, j ).g = palette[ ( pixel[ i ] >> 3 ) & 0x01 ].rgbGreen;
							image( i * 8 + 4, j ).b = palette[ ( pixel[ i ] >> 3 ) & 0x01 ].rgbBlue;
							image( i * 8 + 5, j ).r = palette[ ( pixel[ i ] >> 2 ) & 0x01 ].rgbRed;
							image( i * 8 + 5, j ).g = palette[ ( pixel[ i ] >> 2 ) & 0x01 ].rgbGreen;
							image( i * 8 + 5, j ).b = palette[ ( pixel[ i ] >> 2 ) & 0x01 ].rgbBlue;
							image( i * 8 + 6, j ).r = palette[ ( pixel[ i ] >> 1 ) & 0x01 ].rgbRed;
							image( i * 8 + 6, j ).g = palette[ ( pixel[ i ] >> 1 ) & 0x01 ].rgbGreen;
							image( i * 8 + 6, j ).b = palette[ ( pixel[ i ] >> 1 ) & 0x01 ].rgbBlue;
							image( i * 8 + 7, j ).r = palette[ ( pixel[ i ]      ) & 0x01 ].rgbRed;
							image( i * 8 + 7, j ).g = palette[ ( pixel[ i ]      ) & 0x01 ].rgbGreen;
							image( i * 8 + 7, j ).b = palette[ ( pixel[ i ]      ) & 0x01 ].rgbBlue;
						}
						unsigned char pix = 0;
						for( i = 0 ; i < rest ; i++ )
						{
							image( w * 8 + i, j ).r = palette[ ( pixel[ i ] >> ( 7 - i ) ) & 0x01 ].rgbRed;
							image( w * 8 + i, j ).g = palette[ ( pixel[ i ] >> ( 7 - i ) ) & 0x01 ].rgbGreen;
							image( w * 8 + i, j ).b = palette[ ( pixel[ i ] >> ( 7 - i ) ) & 0x01 ].rgbBlue;
						}
					}
					break;

				case 4:
					{
						size_type rest = width % 2;
						size_type w = ( width - rest ) / 2;
						for( i = 0 ; i < w ; i++ )
						{
							image( i * 2 + 0, j ).r = palette[ ( pixel[ i ] >> 4 ) & 0x0f ].rgbRed;
							image( i * 2 + 0, j ).g = palette[ ( pixel[ i ] >> 4 ) & 0x0f ].rgbGreen;
							image( i * 2 + 0, j ).b = palette[ ( pixel[ i ] >> 4 ) & 0x0f ].rgbBlue;
							image( i * 2 + 1, j ).r = palette[ ( pixel[ i ]      ) & 0x0f ].rgbRed;
							image( i * 2 + 1, j ).g = palette[ ( pixel[ i ]      ) & 0x0f ].rgbGreen;
							image( i * 2 + 1, j ).b = palette[ ( pixel[ i ]      ) & 0x0f ].rgbBlue;
						}
						if( rest == 1 )
						{
							image( i * 2 + 0, j ).r = palette[ ( pixel[ w ] >> 4 ) & 0x0f ].rgbRed;
							image( i * 2 + 0, j ).g = palette[ ( pixel[ w ] >> 4 ) & 0x0f ].rgbGreen;
							image( i * 2 + 0, j ).b = palette[ ( pixel[ w ] >> 4 ) & 0x0f ].rgbBlue;
						}
					}
					break;

				case 8:
					for( i = 0 ; i < width ; i++ )
					{
						image( i, j ).r = palette[ pixel[ i ] ].rgbRed;
						image( i, j ).g = palette[ pixel[ i ] ].rgbGreen;
						image( i, j ).b = palette[ pixel[ i ] ].rgbBlue;
					}
					break;

				case 16:
					break;

				case 24:
					for( i = 0 ; i < width ; i++ )
					{
						image( i, j ).r = pixel[ i * 3 + 2 ];
						image( i, j ).g = pixel[ i * 3 + 1 ];
						image( i, j ).b = pixel[ i * 3 + 0 ];
					}
					break;

				case 32:
					for( i = 0 ; i < width ; i++ )
					{
						image( i, j ).r = pixel[ i * 4 + 2 ];
						image( i, j ).g = pixel[ i * 4 + 1 ];
						image( i, j ).b = pixel[ i * 4 + 0 ];
					}
					break;

				default:
					break;
				}
				pixel += line_bytes;
			}

			return( true );
		}

		static bool convert_to_bmp_data( const array2< rgb< T >, Allocator > &image, unsigned char *bmp, size_type bmp_bits )
		{
			size_type num_bytes = get_bmp_bytes( image, bmp_bits );
			size_type color_num = get_bmp_palette_num( bmp_bits );
			size_type width = image.width( );
			size_type height = image.height( );

			// 全てのバッファをクリアする
			memset( bmp, 0, sizeof( unsigned char ) * num_bytes );

			// ビットマップ用のヘッダの位置を指定する
			_bitmapfileheader_	*pfilehead  = reinterpret_cast < _bitmapfileheader_ * >	( bmp );
			_bitmapinfoheader_	*pinfohead  = reinterpret_cast < _bitmapinfoheader_ * >	( bmp + _bitmapfileheader_::bytes );
			_rgbquad_			*palette    = reinterpret_cast < _rgbquad_ * >			( bmp + _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes );

			_bitmapfileheader_	&filehead     = *pfilehead;
			_bitmapinfoheader_	&infohead     = *pinfohead;

			// BITMAPFILEHEADERの情報を設定
			filehead.bfType    = 0x4d42;	//'M'*256+'B';
			filehead.bfSize    = static_cast< unsigned int >( num_bytes );
			filehead.bfOffBits = static_cast< unsigned int >( _bitmapfileheader_::bytes + _bitmapinfoheader_::bytes + _rgbquad_::bytes * color_num );

			// BITMAPINFOHEADER情報をセット
			infohead.biSize    = _bitmapinfoheader_::bytes;
			infohead.biWidth = static_cast< int >( width );
			infohead.biHeight = static_cast< int >( height );
			infohead.biPlanes = 1;
			infohead.biBitCount = static_cast< unsigned short >( bmp_bits );
			infohead.biCompression = BI_RGB;

			unsigned char *bitmap = NULL;
			switch( bmp_bits )
			{
			case 1:
			case 4:
			case 8:
				bitmap = new unsigned char[ width * height ];
				create_adaptive_palette( image, bitmap, palette, color_num );
				break;
			default:
				break;
			}

			size_type i, j, jj, line_bytes = get_bmp_line_bytes( width, bmp_bits );
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
							pixel[ i ]  = ( bitmap[ i * 8 + 0 + j * width ] != 0 ) << 7;
							pixel[ i ] |= ( bitmap[ i * 8 + 1 + j * width ] != 0 ) << 6;
							pixel[ i ] |= ( bitmap[ i * 8 + 2 + j * width ] != 0 ) << 5;
							pixel[ i ] |= ( bitmap[ i * 8 + 3 + j * width ] != 0 ) << 4;
							pixel[ i ] |= ( bitmap[ i * 8 + 4 + j * width ] != 0 ) << 3;
							pixel[ i ] |= ( bitmap[ i * 8 + 5 + j * width ] != 0 ) << 2;
							pixel[ i ] |= ( bitmap[ i * 8 + 6 + j * width ] != 0 ) << 1;
							pixel[ i ] |= ( bitmap[ i * 8 + 7 + j * width ] != 0 );
						}
						unsigned char pix = 0;
						for( i = 0 ; i < rest ; i++ )
						{
							pix |= ( bitmap[ i + w * 8 + j * width ] != 0 ) << ( 7 - i );
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
							pixel[ i ]  = bitmap[ i * 2 + 0 + j * width ] << 4;
							pixel[ i ] |= bitmap[ i * 2 + 1 + j * width ];
						}
						if( rest == 1 )
						{
							pixel[ w ] = bitmap[ w * 2 + i + j * width ] << 4;
						}
					}
					break;

				case 8:
					for( i = 0 ; i < width ; i++ )
					{
						pixel[ i ]  = bitmap[ i + j * width ];
					}
					break;

				case 16:
					break;

				case 24:
					for( i = 0 ; i < width ; i++ )
					{
						pixel[ i * 3 + 0 ] = image( i, j ).b;
						pixel[ i * 3 + 1 ] = image( i, j ).g;
						pixel[ i * 3 + 2 ] = image( i, j ).r;
					}
					break;

				case 32:
					for( i = 0 ; i < width ; i++ )
					{
						pixel[ i * 4 + 0 ] = image( i, j ).b;
						pixel[ i * 4 + 1 ] = image( i, j ).g;
						pixel[ i * 4 + 2 ] = image( i, j ).r;
					}
					break;

				default:
					break;
				}
				pixel += line_bytes;
			}

			delete [] bitmap;

			return( true );
		}
		static bool read( array2< rgb< T >, Allocator > &image, const std::string &filename )
		{
			typedef typename array2< rgb< T >, Allocator >::size_type size_type;

			size_type filesize;
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );
			// ファイルサイズを取得
			fseek( fp, 0, SEEK_END );
			filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			unsigned char *buff = new unsigned char[ filesize + 1 ];
			fread( buff, sizeof( unsigned char ), filesize, fp );
			fclose( fp );

			bool ret = convert_from_bmp_data( buff, image );
			delete [] buff;
			return( ret );
		}

		static bool write( const array2< rgb< T >, Allocator > &image, const std::string &filename, size_type bmp_bits )
		{
			typedef typename array2< rgb< T >, Allocator >::size_type size_type;

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

			// ファイルサイズを取得
			fwrite( buff, sizeof(unsigned char), size, fp);
			fclose( fp );

			delete [] buff;
			return( true );
		}
	};
}


template < class T, class Allocator >
bool read_bmp( array2< T, Allocator > &image, const std::string &filename )
{
	return( __bmp_controller__::bmp_controller< T, Allocator >::read( image, filename ) );
}

template < class T, class Allocator >
bool write_bmp( array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::size_type bmp_bits = 24 )
{
	return( __bmp_controller__::bmp_controller< T, Allocator >::write( image, filename, bmp_bits ) );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_BMP__
