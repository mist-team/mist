#ifndef __INCLUDE_MIST_RAW__
#define __INCLUDE_MIST_RAW__


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

#include <iostream>
#include <string>
#include <zlib.h>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __raw_controller__
{
	template < class T, class Allocator, class Functor >
	struct raw_controller1
	{
		typedef typename array1< T, Allocator >::value_type value_type;
		typedef typename array1< T, Allocator >::size_type size_type;

		static bool read( array1< T, Allocator > &image, const std::string &filename, size_type w, 
										double x, value_type offset, bool from_little_endian, Functor f )
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
			size_type size = w * byte; // �X���C�X1�����̃��������m��
			byte_array< T > data;

			unsigned char *tmparray = new unsigned char[ size + 1 ];
			unsigned char *pointer = tmparray;
			size_type read_size = 0;
			while( gzeof( fp ) == 0 )
			{
				read_size = gzread( pointer, 1024, fp );
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
				image[ i ] = to_current_endian( data, from_little_endian ).get_value( ) + offset;
			}
			delete [] tmparray;

			// �t�@�C������ǂݏo���ꂽ�f�[�^�ʂ��C�w�肳�ꂽ���̂������Ȃ��ꍇ
			if( w > i )
			{
				array1< T, Allocator > tmp( image );
				image.resize( i );
				for( i = 0 ; i < image.size( ) ; i++ )
				{
					image[i] = tmp[i];
				}
			}

			return( true );
		}


		static bool write( const array1< T, Allocator > &image, const std::string &filename, value_type offset, bool to_little_endian, Functor f )
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
			size_type size = w * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( i = 0 ; i < w ; i++ )
			{
				data.set_value( image[ i ] - offset );
				data = from_current_endian( data, to_little_endian );
				for( l = 0 ; l < byte ; l++ )
				{
					tmparray[ i * byte + l ] = data[ l ];
				}
			}

			// �t�@�C���֏����o��
			unsigned char *pointer = tmparray;
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
					delete [] tmparray;
					return( false );
				}
			}

			delete [] tmparray;
			fclose( fp );

			return( true );
		}

		static bool write_gz( const array1< T, Allocator > &image, const std::string &filename, value_type offset, bool to_little_endian, Functor f )
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
			size_type size = w * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( i = 0 ; i < w ; i++ )
			{
				data.set_value( image[ i ] - offset );
				data = from_current_endian( data, to_little_endian );
				for( l = 0 ; l < byte ; l++ )
				{
					tmparray[ i * byte + l ] = data[ l ];
				}
			}

			// �t�@�C���֏����o��
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
					delete [] tmparray;
					return( false );
				}
			}

			delete [] tmparray;
			gzclose( fp );

			return( true );
		}
	};


	template < class T, class Allocator, class Functor >
	struct raw_controller2
	{
		typedef typename array2< T, Allocator >::value_type value_type;
		typedef typename array2< T, Allocator >::size_type size_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename, size_type w, size_type h,
										double x, double y, value_type offset, bool from_little_endian, Functor f )
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
			size_type size = w * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			f( 0.0 );

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
					image( i, j ) = to_current_endian( data, from_little_endian ).get_value( ) + offset;
				}

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				if( !f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 ) )
				{
					delete [] tmparray;
					gzclose( fp );
					return( false );
				}
			}

			f( 100.0 );

			delete [] tmparray;
			gzclose( fp );

			// �t�@�C������ǂݏo���ꂽ�f�[�^�ʂ��C�w�肳�ꂽ���̂������Ȃ��ꍇ
			if( h > j )
			{
				array2< T, Allocator > tmp( image );
				image.resize( w, j );
				for( i = 0 ; i < image.size( ) ; i++ )
				{
					image[i] = tmp[i];
				}
			}

			f( 100.1 );

			return( true );
		}


		static bool write( const array2< T, Allocator > &image, const std::string &filename, value_type offset, bool to_little_endian, Functor f )
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
			size_type size = w * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			f( 0.0 );

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					data.set_value( image( i, j ) - offset );
					data = from_current_endian( data, to_little_endian );
					for( l = 0 ; l < byte ; l++ )
					{
						tmparray[ i * byte + l ] = data[ l ];
					}
				}
				fwrite( tmparray, 1, size, fp );

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				if( !f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 ) )
				{
					delete [] tmparray;
					gzclose( fp );
					return( false );
				}
			}

			f( 100.0 );

			delete [] tmparray;
			fclose( fp );

			f( 100.1 );

			return( true );
		}

		static bool write_gz( const array2< T, Allocator > &image, const std::string &filename, value_type offset, bool to_little_endian, Functor f )
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
			size_type size = w * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					data.set_value( image( i, j ) - offset );
					data = from_current_endian( data, to_little_endian );
					for( l = 0 ; l < byte ; l++ )
					{
						tmparray[ i * byte + l ] = data[ l ];
					}
				}
				gzwrite( fp, tmparray, static_cast< unsigned int >( size ) );
			}

			delete [] tmparray;
			gzclose( fp );

			return(true);
		}
	};

	template < class T, class Allocator, class Functor >
	struct raw_controller3
	{
		typedef typename array3< T, Allocator >::value_type value_type;
		typedef typename array3< T, Allocator >::size_type size_type;

		static bool read( array3< T, Allocator > &image, const std::string &filename, size_type w, size_type h, size_type d,
										double x, double y, double z, value_type offset, bool from_little_endian, Functor f )
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
			size_type size = w * h * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			f( 0.0 );

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
						image( i, j, k ) = to_current_endian( data, from_little_endian ).get_value( ) + offset;
					}
				}

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				if( !f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 ) )
				{
					image.clear( );
					delete [] tmparray;
					gzclose( fp );
					return( false );
				}
			}
			delete [] tmparray;
			gzclose( fp );

			f( 100.0 );

			// �t�@�C������ǂݏo���ꂽ�f�[�^�ʂ��C�w�肳�ꂽ���̂������Ȃ��ꍇ
			if( d > k )
			{
				array3< T, Allocator > tmp( image );
				image.resize( w, h, k );
				for( i = 0 ; i < image.size( ) ; i++ )
				{
					image[ i ] = tmp[ i ];
				}
			}

			f( 100.1 );

			return( true );
		}


		static bool write( const array3< T, Allocator > &image, const std::string &filename, value_type offset, bool to_little_endian, Functor f )
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
			size_type size = w * h * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			f( 0.0 );

			for( k = 0 ; k < d ; k++ )
			{
				for( j = 0 ; j < h ; j++ )
				{
					for( i = 0 ; i < w ; i++ )
					{
						data.set_value( image( i, j, k ) - offset );
						data = from_current_endian( data, to_little_endian );
						for( l = 0 ; l < byte ; l++ )
						{
							tmparray[ ( i + j * w ) * byte + l ] = data[l];
						}
					}
				}
				fwrite( tmparray, 1, size, fp );

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				if( !f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 ) )
				{
					delete [] tmparray;
					fclose( fp );
					return( false );
				}
			}

			f( 100.0 );

			delete [] tmparray;
			fclose( fp );

			f( 100.1 );

			return( true );
		}

		static bool write_gz( const array3< T, Allocator > &image, const std::string &filename, value_type offset, bool to_little_endian, Functor f )
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
			size_type size = w * h * byte; // �X���C�X1�����̃��������m��
			unsigned char *tmparray =  new unsigned char[ size ];
			byte_array< T > data;

			f( 0.0 );

			for( k = 0 ; k < d ; k++ )
			{
				for( j = 0 ; j < h ; j++ )
				{
					for( i = 0 ; i < w ; i++ )
					{
						data.set_value( image( i, j, k ) - offset );
						data = from_current_endian( data, to_little_endian );
						for( l = 0 ; l < byte ; l++ )
						{
							tmparray[ ( i + j * w ) * byte + l ] = data[l];
						}
					}
				}

				gzwrite( fp, tmparray, static_cast< unsigned int >( size ) );

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				if( !f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 ) )
				{
					delete [] tmparray;
					gzclose( fp );
					return( false );
				}
			}

			f( 100.0 );

			delete [] tmparray;
			gzclose( fp );

			f( 100.1 );

			return(true);
		}
	};
}




// 1�����摜�z��p�̓ǂݍ��݁E�������݊֐�
template < class T, class Allocator, class Functor >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x, typename array1< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller1< T, Allocator, Functor >::read( image, filename, w, x, offset, from_little_endian, callback ) );
}

template < class T, class Allocator, class Functor >
bool write_raw( const array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller1< T, Allocator, Functor >::write( image, filename, offset, to_little_endian, callback ) );
}

template < class T, class Allocator, class Functor >
bool write_raw_gz( const array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller1< T, Allocator, Functor >::write_gz( image, filename, offset, to_little_endian, callback ) );
}

template < class T, class Allocator >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x = 1.0, typename array1< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, x, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}

template < class T, class Allocator >
bool write_raw( const array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}

template < class T, class Allocator >
bool write_raw_gz( const array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw_gz( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}




// 2�����摜�z��p�̓ǂݍ��݁E�������݊֐�
template < class T, class Allocator, class Functor >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x, double y, typename array2< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller2< T, Allocator, Functor >::read( image, filename, w, h, x, y, offset, from_little_endian, callback ) );
}

template < class T, class Allocator, class Functor >
bool write_raw( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller2< T, Allocator, Functor >::write( image, filename, offset, to_little_endian, callback ) );
}

template < class T, class Allocator, class Functor >
bool write_raw_gz( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller2< T, Allocator, Functor >::write_gz( image, filename, offset, to_little_endian, callback ) );
}

template < class T, class Allocator >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x = 1.0, double y = 1.0, typename array2< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, h, x, y, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}

template < class T, class Allocator >
bool write_raw( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}

template < class T, class Allocator >
bool write_raw_gz( const array2< T, Allocator > &image, const std::string &filename, typename array2< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw_gz( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}




// 3�����摜�z��p�̓ǂݍ��݁E�������݊֐�
template < class T, class Allocator, class Functor >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x, double y, double z, typename array3< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller3< T, Allocator, Functor >::read( image, filename, w, h, d, x, y, z, offset, from_little_endian, callback ) );
}

template < class T, class Allocator, class Functor >
bool write_raw( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller3< T, Allocator, Functor >::write( image, filename, offset, to_little_endian, callback ) );
}

template < class T, class Allocator, class Functor >
bool write_raw_gz( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	return( __raw_controller__::raw_controller3< T, Allocator, Functor >::write_gz( image, filename, offset, to_little_endian, callback ) );
}

template < class T, class Allocator >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x = 1.0, double y = 1.0, double z = 1.0, typename array3< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, h, d, x, y, z, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}

template < class T, class Allocator >
bool write_raw( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}

template < class T, class Allocator >
bool write_raw_gz( const array3< T, Allocator > &image, const std::string &filename, typename array3< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw_gz( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_RAW__
