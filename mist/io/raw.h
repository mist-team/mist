#ifndef __INCLUDE_MIST_RAW__
#define __INCLUDE_MIST_RAW__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "../config/endian.h"
#endif

#include <iostream>
#include <string>
#include <zlib.h>



// mist名前空間の始まり
_MIST_BEGIN


namespace __raw_controller__
{
	template < class T, class Allocator >
	struct raw_controller1
	{
		typedef typename array1< T, Allocator >::value_type value_type;
		typedef typename array1< T, Allocator >::size_type size_type;

		static bool read( array1< T, Allocator > &image, const std::string &filename, size_type w, 
							double x = 1.0, value_type offset = 0, bool raw_is_little_endian = false )
		{
			gzFile fp;
			size_type i, l;

			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			image.resize( w );
			image.reso1( x );

			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * byte; // スライス1枚分のメモリを確保
			byte_array< T > data;

			unsigned char *tmparray = new unsigned char[ size + 1 ];
			unsigned char *pointer = tmparray;
			size_type read_size = 0;
			while( gzeof( fp ) == 0 )
			{
				read_size = gzread( fp, pointer, 1024, fp );
				if( read_size < 1024 )
				{
					break;
				}
				pointer += read_size;
			}
			gzclose( fp );

			for( i = 0 ; i < w ; i++ )
			{
				for( l = 0 ; l < byte ; l++ )
				{
					data[ l ] = tmparray[ i * byte + l ];
				}
				image[ i ] = to_current_endian( data, raw_is_little_endian ).get_value( ) + offset;
			}
			delete [] tmparray;

			// ファイルから読み出されたデータ量が，指定されたものよりも少ない場合
			if( w > i )
			{
				array1< T, Allocator > tmp( image );
				image.resize( i );
				for( i = 0 ; i < image.size( ) ; i++ )
				{
					image[i] = tmp[i];
				}
			}

			return(true);
		}


		static bool write( const array1< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = false )
		{
			FILE *fp;
			size_type i, l;

			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			size_type w = image.width( );
			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( i = 0 ; i < w ; i++ )
			{
				data.set_value( image[ i ] - offset );
				data = from_current_endian( data, raw_is_little_endian );
				for( l = 0 ; l < byte ; l++ )
				{
					tmparray[ i * byte + l ] = data[ l ];
				}
			}

			// ファイルへ書き出し
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

			delete [] tmparray;
			fclose( fp );

			return(true);
		}

		static bool write_gz( const array1< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = false )
		{
			gzFile fp;
			size_type i, l;

			if( ( fp = gzopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			size_type w = image.width( );
			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( i = 0 ; i < w ; i++ )
			{
				data.set_value( image[ i ] - offset );
				data = from_current_endian( data, raw_is_little_endian );
				for( l = 0 ; l < byte ; l++ )
				{
					tmparray[ i * byte + l ] = data[ l ];
				}
			}

			// ファイルへ書き出し
			unsigned char *pointer = tmparray;
			size_type write_size = 0;
			while( size > 0 )
			{
				write_size = gzwrite( fp, pointer, 1024 );
				pointer += write_size;
				size -= write_size;
				if( write_size != 1024 )
				{
					fclose( fp );
					delete [] buff;
					return( false );
				}
			}

			delete [] tmparray;
			gzclose( fp );

			return(true);
		}
	};


	template < class T, class Allocator >
	struct raw_controller2
	{
		typedef typename array2< T, Allocator >::value_type value_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename, size_type w, size_type h,
						double x = 1.0, double y = 1.0, value_type offset = 0, bool raw_is_little_endian = false )
		{
			gzFile fp;
			size_type i, j, l;

			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			image.resize( w, h );
			image.reso1( x );
			image.reso2( y );

			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( j = 0 ; j < h ; j++ )
			{
				if( gzeof( fp ) ) break;
				ret = gzread( fp, (void*)tmparray, static_cast< unsigned int >( size ) );
				if( ret != size ) break;

				for( i = 0 ; i < w ; i++ )
				{
					for( l = 0 ; l < byte ; l++ )
					{
						data[ l ] = tmparray[ i * byte + l ];
					}
					image( i, j ) = to_current_endian( data, raw_is_little_endian ).get_value( ) + offset;
				}
			}
			delete [] tmparray;
			gzclose( fp );

			// ファイルから読み出されたデータ量が，指定されたものよりも少ない場合
			if( h > j )
			{
				array2< T, Allocator > tmp( image );
				image.resize( w, j );
				for( i = 0 ; i < image.size( ) ; i++ )
				{
					image[i] = tmp[i];
				}
			}

			return(true);
		}


		static bool write( const array2< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = false )
		{
			FILE *fp;
			size_type i, j, l;

			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			size_type w = image.width( );
			size_type h = image.height( );
			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					data.set_value( image( i, j ) - offset );
					data = from_current_endian( data, raw_is_little_endian );
					for( l = 0 ; l < byte ; l++ )
					{
						tmparray[ i * byte + l ] = data[ l ];
					}
				}
				fwrite( tmparray, 1, size, fp );
			}

			delete [] tmparray;
			fclose( fp );

			return(true);
		}

		static bool write_gz( const array2< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = false )
		{
			gzFile fp;
			size_type i, j, l;

			if( ( fp = gzopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "[" << filename << "] open err" << std::endl;
				return(false);
			}

			size_type w = image.width( );
			size_type h = image.height( );
			size_type byte = sizeof( T );
			size_type ret;
			size_type size = w * byte; // スライス1枚分のメモリを確保
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					data.set_value( image( i, j ) - offset );
					data = from_current_endian( data, raw_is_little_endian );
					for( l = 0 ; l < byte ; l++ )
					{
						tmparray[ i * byte + l ] = data[ l ];
					}
				}
				gzwrite( fp, tmparray, size );
			}

			delete [] tmparray;
			gzclose( fp );

			return(true);
		}
	};

	template < class T, class Allocator >
	struct raw_controller3
	{
		typedef typename array3< T, Allocator >::value_type value_type;
		typedef typename array3< T, Allocator >::size_type size_type;

		static bool read( array3< T, Allocator > &image, const std::string &filename, size_type w, size_type h, size_type d,
						double x = 1.0, double y = 1.0, double z = 1.0, value_type offset = 0, bool raw_is_little_endian = false )
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
				ret = gzread( fp, (void*)tmparray, static_cast< unsigned int >( size ) );
				if( ret != size ) break;

				for( j = 0 ; j < h ; j++ )
				{
					for( i = 0 ; i < w ; i++ )
					{
						for( l = 0 ; l < byte ; l++ )
						{
							data[l] = tmparray[ ( i + j * w ) * byte + l ];
						}
						image( i, j, k ) = to_current_endian( data, raw_is_little_endian ).get_value( ) + offset;
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


		static bool write( const array3< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = false )
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
						data = from_current_endian( data, raw_is_little_endian );
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

		static bool write_gz( const array3< T, Allocator > &image, const std::string &filename, value_type offset = 0, bool raw_is_little_endian = false )
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
						data = from_current_endian( data, raw_is_little_endian );
						for( l = 0 ; l < byte ; l++ )
						{
							tmparray[ ( i + j * w ) * byte + l ] = data[l];
						}
					}
				}

				gzwrite( fp, tmparray, static_cast< unsigned int >( size ) );
			}

			delete [] tmparray;
			gzclose( fp );

			return(true);
		}
	};
}




// 1次元画像配列用の読み込み・書き込み関数
template < class T, class Allocator >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x = 1.0, typename array1< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller1< T, Allocator >::read( image, filename, w, x, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw( const array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller1< T, Allocator >::write( image, filename, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw_gz( const array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller1< T, Allocator >::write_gz( image, filename, offset, raw_is_little_endian ) );
}


// 2次元画像配列用の読み込み・書き込み関数
template < class T, class Allocator >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x = 1.0, double y = 1.0, typename array2< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller2< T, Allocator >::read( image, filename, w, h, x, y, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller2< T, Allocator >::write( image, filename, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw_gz( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller2< T, Allocator >::write_gz( image, filename, offset, raw_is_little_endian ) );
}


// 3次元画像配列用の読み込み・書き込み関数
template < class T, class Allocator >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x = 1.0, double y = 1.0, double z = 1.0, typename array3< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller3< T, Allocator >::read( image, filename, w, h, d, x, y, z, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller3< T, Allocator >::write( image, filename, offset, raw_is_little_endian ) );
}

template < class T, class Allocator >
bool write_raw_gz( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset = 0, bool raw_is_little_endian = false )
{
	return( __raw_controller__::raw_controller3< T, Allocator >::write_gz( image, filename, offset, raw_is_little_endian ) );
}

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_RAW__
