#ifndef __INCLUDE_MIST_RAW__
#define __INCLUDE_MIST_RAW__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#include <iostream>
#include <string>
#include <zlib.h>



// mist名前空間の始まり
_MIST_BEGIN


namespace __raw_controller__
{
	template < class T >
	union byte_array
	{
	private:
		typedef T value_type;
		value_type value;
		unsigned char byte[ sizeof( value_type ) ];

	public:
		byte_array( ) : value( 0 ){ }
		byte_array( const value_type v ) : value( v ){ }
		unsigned char &operator[]( size_t index ){ return( byte[index] ); }
		const unsigned char &operator[]( size_t index ) const { return( byte[index] ); }
		const value_type get_value( ) const { return( value ); }
		value_type set_value( const value_type &v ) { return( value = v ); }
	};

	bool _is_little_endian_( )
	{
		return( byte_array< unsigned short >( 1 )[ 0 ] == 1 );
	}

	bool _is_big_endian_( )
	{
		return( byte_array< unsigned short >( 1 )[ 0 ] != 1 );
	}

	template < class T >
	void swap_bytes( byte_array< T > &bytes )
	{
		byte_array< T > tmp( bytes );
		for( size_t i = 0 ; i < sizeof( T ) ; ++i )
		{
			bytes[ i ] = tmp[ sizeof( T ) - i - 1 ];
		}
	}

	template < class T >
	void to_current_endian( byte_array< T > &bytes, bool is_little_endian )
	{
		static bool current_endian = _is_little_endian_( );
		if( current_endian != is_little_endian )
		{
			swap_bytes( bytes );
		}
	}

	template < class T >
	void from_current_endian( byte_array< T > &bytes, bool is_little_endian )
	{
		static bool current_endian = _is_little_endian_( );
		if( current_endian != is_little_endian )
		{
			swap_bytes( bytes );
		}
	}


	template < class T, class Allocator >
	struct raw_controller
	{
		typedef typename array3< T, Allocator >::value_type value_type;
		typedef typename array3< T, Allocator >::size_type size_type;

		static bool read( array3< T, Allocator > &image, const std::string &filename, size_type w, size_type h, size_type d,
						double x = 1.0, double y = 1.0, double z = 1.0, value_type offset = 0, bool raw_is_little_endian = true )
		{
			gzFile fp;
			size_type i, j, k, l;

			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			image.resize( w, h, d );
			image.reso1( x );
			image.reso2( y );
			image.reso3( z );

			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * h * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( k = 0 ; k < d ; k++ )
			{
				if( gzeof( fp ) ) break;
				ret = gzread(fp, (void*)tmparray, size);
				if( ret != size ) break;

				for( j = 0 ; j < h ; j++ )
				{
					for( i = 0 ; i < w ; i++ )
					{
						for( l = 0 ; l < byte ; l++ )
						{
							data[l] = tmparray[ ( i + j * w ) * byte + l ];
						}
						to_current_endian( data, raw_is_little_endian );
						image( i, j, k ) = data.get_value( ) + offset;
					}
				}
			}
			delete [] tmparray;
			gzclose( fp );

			// ファイルから読み出されたデータ量が，指定されたものよりも少ない場合
			if( d > k )
			{
				array3< T, Allocator > tmp( image );
				image.resize( w, h, k );
				for( i = 0 ; i < image.size( ) ; i++ )
				{
					image[i] = tmp[i];
				}
			}

			return(true);
		}


		static bool write( const array3< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = true )
		{
			FILE *fp;
			size_type i, j, k, l;

			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			size_type w = image.width( );
			size_type h = image.height( );
			size_type d = image.depth( );
			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * h * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( k = 0 ; k < d ; k++ )
			{
				for( j = 0 ; j < h ; j++ )
				{
					for( i = 0 ; i < w ; i++ )
					{
						data.set_value( image( i, j, k ) - offset );
						from_current_endian( data, raw_is_little_endian );
						for( l = 0 ; l < byte ; l++ )
						{
							tmparray[ ( i + j * w ) * byte + l ] = data[l];
						}
					}
				}
				fwrite( tmparray, 1, size, fp );
			}

			delete [] tmparray;
			fclose( fp );

			return(true);
		}

		static bool write_gz( const array3< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = true )
		{
			gzFile fp;
			size_type i, j, k, l;

			if( ( fp = gzopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			size_type w = image.width( );
			size_type h = image.height( );
			size_type d = image.depth( );
			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * h * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( k = 0 ; k < d ; k++ )
			{
				for( j = 0 ; j < h ; j++ )
				{
					for( i = 0 ; i < w ; i++ )
					{
						data.set_value( image( i, j, k ) - offset );
						from_current_endian( data, raw_is_little_endian );
						for( l = 0 ; l < byte ; l++ )
						{
							tmparray[ ( i + j * w ) * byte + l ] = data[l];
						}
					}
				}

				gzwrite( fp, tmparray, size );
			}

			delete [] tmparray;
			gzclose( fp );

			return(true);
		}
	};
}


template < class T, class Allocator >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x = 1.0, double y = 1.0, double z = 1.0, typename array3< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = true )
{
	return( __raw_controller__::raw_controller< T, Allocator >::read( image, filename, w, h, d, x, y, z, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = true )
{
	return( __raw_controller__::raw_controller< T, Allocator >::write( image, filename, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw_gz( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = true )
{
	return( __raw_controller__::raw_controller< T, Allocator >::write_gz( image, filename, offset, raw_is_little_endian ) );
}

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_RAW__
