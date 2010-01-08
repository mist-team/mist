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

/// @file mist/io/raw.h
//!
//! @brief �o�C�i���摜��ǂݏ������邽�߂̃��C�u����
//!
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
	struct resize_image
	{
		// �t�@�C������ǂݏo���ꂽ�f�[�^�ʂ��C�w�肳�ꂽ���̂������Ȃ��ꍇ
		template < class T, class Allocator >
		static bool resize( array1< T, Allocator > &image, typename array1< T, Allocator >::size_type num_elements )
		{
			typedef typename array1< T, Allocator >::size_type size_type;

			if( num_elements == 0 )
			{
				return( false );
			}
			else if( image.size( ) == num_elements )
			{
				return( true );
			}
			else
			{
				array1< T, Allocator > tmp( image );
				image.resize( num_elements );
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[ i ] = tmp[ i ];
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool resize( array2< T, Allocator > &image, typename array2< T, Allocator >::size_type num_elements )
		{
			typedef typename array2< T, Allocator >::size_type size_type;

			size_type num_lines = num_elements / image.width( );
			if( num_lines == 0 )
			{
				return( false );
			}
			else if( image.height( ) == num_lines )
			{
				return( true );
			}
			else
			{
				array2< T, Allocator > tmp( image );
				image.resize( image.width( ), num_lines );
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[i] = tmp[i];
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool resize( array3< T, Allocator > &image, typename array3< T, Allocator >::size_type num_elements )
		{
			typedef typename array3< T, Allocator >::size_type size_type;

			size_type num_slices = num_elements / ( image.width( ) * image.height( ) );
			if( num_slices == 0 )
			{
				return( false );
			}
			else if( image.depth( ) == num_slices )
			{
				return( true );
			}
			else
			{
				array3< T, Allocator > tmp( image );
				image.resize( image.width( ), image.height( ), num_slices );
				for( size_type i = 0 ; i < image.size( ) ; i++ )
				{
					image[ i ] = tmp[ i ];
				}
			}
			return( true );
		}
	};

	struct raw_controller
	{
		template < class Array, class Functor, class ValueType >
		static bool read( Array &image,
							const std::string &filename,
							typename Array::size_type w,
							typename Array::size_type h,
							typename Array::size_type d,
							typename Array::value_type offset,
							bool from_little_endian,
							typename Array::size_type skip_num_bytes,
							Functor f,
							ValueType /* v */
						)
		{
			typedef typename Array::value_type value_type;
			typedef typename Array::size_type size_type;

			gzFile fp;

			if( ( fp = gzopen( filename.c_str( ), "rb" ) ) == NULL )
			{
				std::cerr << "Error occured while opening RAW file format in [" << filename << "]" << std::endl;
				return( false );
			}

			image.resize( w, h, d );

			size_type byte = sizeof( ValueType );
			unsigned char tmparray[ sizeof( ValueType ) * 4096 ];
			byte_array< ValueType > data;

			f( 0.0 );

			size_type i = 0;
			unsigned int pre_progress = 0;
			while( !gzeof( fp ) )
			{
				size_type num = gzread( fp, ( void * )tmparray, sizeof( ValueType ) * 4096 );

				if( num <= skip_num_bytes )
				{
					skip_num_bytes -= num;
					continue;
				}

				size_type j = skip_num_bytes;
				skip_num_bytes = 0;

				for( ; i < image.size( ) && j < num ; j += byte )
				{
					for( size_type l = 0 ; l < byte ; l++ )
					{
						data[ l ] = tmparray[ j + l ];
					}
					image[ i++ ] = static_cast< value_type >( to_current_endian( data, from_little_endian ).get_value( ) ) + offset;
				}

				if( i >= image.size( ) )
				{
					break;
				}

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				unsigned int progress = static_cast< unsigned int >( static_cast< double >( i + 1 ) / static_cast< double >( image.size( ) ) * 100.0 );
				if( progress != pre_progress )
				{
					pre_progress = progress;
					if( !f( progress ) )
					{
						image.clear( );
						gzclose( fp );
						return( false );
					}
				}
			}

			gzclose( fp );

			// �t�@�C������ǂݏo���ꂽ�f�[�^�ʂ��C�w�肳�ꂽ���̂������Ȃ��ꍇ
			if( !resize_image::resize( image, i ) )
			{
				// 1�����f�[�^���ǂݍ��܂�Ȃ������ꍇ
				f( 100.1 );
				return( false );
			}
			else
			{
				f( 100.1 );
				return( true );
			}
		}


		template < class Array, class Functor, class ValueType >
		static bool write( const Array &image, const std::string &filename, typename Array::value_type offset, bool to_little_endian, Functor f, ValueType v )
		{
			typedef typename Array::value_type value_type;
			typedef typename Array::size_type size_type;

			FILE *fp;

			if( ( fp = fopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "Error occured while opening RAW file format in [" << filename << "]" << std::endl;
				return( false );
			}

			size_type w = image.width( );
			size_type h = image.height( );
			size_type d = image.depth( );
			unsigned char tmparray[ sizeof( ValueType ) * 4096 ];
			byte_array< ValueType > data;

			f( 0.0 );

			size_type i = 0, n, l, pre_progress = 0;
			while( i < image.size( ) )
			{
				for( n = 0 ; i < image.size( ) && n < sizeof( ValueType ) * 4096 ; i++, n += sizeof( ValueType ) )
				{
					data.set_value( static_cast< ValueType >( image[ i ] + offset ) );
					data = from_current_endian( data, to_little_endian );
					for( l = 0 ; l < sizeof( ValueType ) ; l++ )
					{
						tmparray[ n + l ] = data[ l ];
					}
				}
				fwrite( tmparray, 1, n, fp );

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				size_type progress = static_cast< size_type >( static_cast< double >( i ) / static_cast< double >( image.size( ) ) * 100.0 );
				if( progress != pre_progress )
				{
					pre_progress = progress;
					if( !f( static_cast< double >( progress ) ) )
					{
						fclose( fp );
						return( false );
					}
				}
			}

			fclose( fp );

			f( 100.1 );

			return( true );
		}

		template < class Array, class Functor, class ValueType >
		static bool write_gz( const Array &image, const std::string &filename, typename Array::value_type offset, bool to_little_endian, Functor f, ValueType v )
		{
			typedef typename Array::value_type value_type;
			typedef typename Array::size_type size_type;

			gzFile fp;

			if( ( fp = gzopen( filename.c_str( ), "wb" ) ) == NULL )
			{
				std::cerr << "Error occured while opening RAW + GZ file format in [" << filename << "]" << std::endl;
				return( false );
			}

			unsigned char tmparray[ sizeof( ValueType ) * 4096 ];
			byte_array< ValueType > data;

			f( 0.0 );

			size_type i = 0, n, l, pre_progress = 0;
			while( i < image.size( ) )
			{
				for( n = 0 ; i < image.size( ) && n < sizeof( ValueType ) * 4096 ; i++, n += sizeof( ValueType ) )
				{
					data.set_value( static_cast< ValueType >( image[ i ] + offset ) );
					data = from_current_endian( data, to_little_endian );
					for( l = 0 ; l < sizeof( ValueType ) ; l++ )
					{
						tmparray[ n + l ] = data[ l ];
					}
				}
				gzwrite( fp, tmparray, static_cast< unsigned int >( n ) );

				// �i�s�󋵂�0�`100���ŕ\������
				// �R�[���o�b�N�֐��̖߂�l�� false �ɂȂ����ꍇ�͏����𒆒f���C�����Ԃ�
				size_type progress = static_cast< size_type >( static_cast< double >( i ) / static_cast< double >( image.size( ) ) * 100.0 );
				if( progress != pre_progress )
				{
					pre_progress = progress;
					if( !f( static_cast< double >( progress ) ) )
					{
						gzclose( fp );
						return( false );
					}
				}
			}

			gzclose( fp );

			f( 100.1 );

			return( true );
		}
	};
}


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup image_raw_group RAW �摜���o��
//!
//! RAW�t�@�C���iGZIP���k�ɑΉ��j�̓ǂݍ��݂Ə������݃T�|�[�g��MIST�ɒǉ�������̂ł��D
//! �ʓr http://www.zlib.org ���񋟂���t���[��ZLIB���C�u�������K�v�ł��D
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/raw.h>
//! @endcode
//!
//!  @{




/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜�̕�
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  __dmy__            �c �ǂݍ������Ƃ��Ă���f�[�^�̌^������킷�_�~�[�I�u�W�F�N�g�ishort�^�̃I�u�W�F�N�g�Ȃǁj
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::size_type w,
							typename array< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	return( __raw_controller__::raw_controller::read( image, filename, w, 1, 1, offset, from_little_endian, 0, callback, __dmy__ ) );
}

/// @brief MIST�R���e�i���̉摜�� �����kRAW �摜�Ƃ��ďo�͂���
//! 
//! @attention �f�[�^��ϊ����ĕۑ�����ۂɁC�l�̃I�[�o�[�t���[�Ȃǂ͖�������̂Œ��ӂ��K�v
//! 
//! @param[in] image            �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in] filename         �c ���̓t�@�C����
//! @param[in] offset           �c �摜��������Z����I�t�Z�b�g�l
//! @param[in] to_little_endian �c �o�͉摜�̃f�[�^�`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  __dmy__         �c �o�͂���f�[�^�̌^������킷�_�~�[�I�u�W�F�N�g�ishort�^�̃I�u�W�F�N�g�Ȃǁj
//! @param[in] callback         �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool write_raw( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, ValueType __dmy__, Functor callback )
{
	return( __raw_controller__::raw_controller::write( image, filename, offset, to_little_endian, callback, __dmy__ ) );
}

/// @brief MIST�R���e�i���̉摜�� GZ���kRAW �摜�Ƃ��ďo�͂���
//! 
//! @attention �f�[�^��ϊ����ĕۑ�����ۂɁC�l�̃I�[�o�[�t���[�Ȃǂ͖�������̂Œ��ӂ��K�v
//! 
//! @param[in] image            �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in] filename         �c ���̓t�@�C����
//! @param[in] offset           �c �摜��������Z����I�t�Z�b�g�l
//! @param[in] to_little_endian �c �o�͉摜�̃f�[�^�`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  __dmy__         �c �o�͂���f�[�^�̌^������킷�_�~�[�I�u�W�F�N�g�ishort�^�̃I�u�W�F�N�g�Ȃǁj
//! @param[in] callback         �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool write_raw_gz( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, ValueType __dmy__, Functor callback )
{
	return( __raw_controller__::raw_controller::write_gz( image, filename, offset, to_little_endian, callback, __dmy__ ) );
}



/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜�̕�
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::size_type w,
							typename array< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, offset, from_little_endian, v, callback ) );
}



/// @brief MIST�R���e�i���̉摜�� �����kRAW �摜�Ƃ��ďo�͂���
//! 
//! @param[in] image            �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in] filename         �c ���̓t�@�C����
//! @param[in] offset           �c �摜��������Z����I�t�Z�b�g�l
//! @param[in] to_little_endian �c �o�͉摜�̃f�[�^�`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in] callback         �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class Functor >
bool write_raw( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	typename array< T, Allocator >::value_type v( 0 );
	return( write_raw( image, filename, offset, to_little_endian, v, callback ) );
}


/// @brief MIST�R���e�i���̉摜�� GZ���kRAW �摜�Ƃ��ďo�͂���
//! 
//! @param[in] image            �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in] filename         �c ���̓t�@�C����
//! @param[in] offset           �c �摜��������Z����I�t�Z�b�g�l
//! @param[in] to_little_endian �c �o�͉摜�̃f�[�^�`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in] callback         �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class Functor >
bool write_raw_gz( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset, bool to_little_endian, Functor callback )
{
	typename array< T, Allocator >::value_type v( 0 );
	return( write_raw_gz( image, filename, offset, to_little_endian, v, callback ) );
}


/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜�̕�
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_raw( array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::size_type w, typename array< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}


/// @brief MIST�R���e�i���̉摜�� �����kRAW �摜�Ƃ��ďo�͂���
//! 
//! @param[in] image            �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in] filename         �c ���̓t�@�C����
//! @param[in] offset           �c �摜��������Z����I�t�Z�b�g�l
//! @param[in] to_little_endian �c �o�͉摜�̃f�[�^�`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_raw( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset = 0, bool to_little_endian = false )
{
	return( write_raw( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}


/// @brief MIST�R���e�i���̉摜�� GZ���kRAW �摜�Ƃ��ďo�͂���
//! 
//! @param[in] image            �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in] filename         �c ���̓t�@�C����
//! @param[in] offset           �c �摜��������Z����I�t�Z�b�g�l
//! @param[in] to_little_endian �c �o�͉摜�̃f�[�^�`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool write_raw_gz( const array< T, Allocator > &image, const std::string &filename, typename array< T, Allocator >::value_type offset = array< T, Allocator >::value_type( 0 ), bool to_little_endian = false )
{
	return( write_raw_gz( image, filename, offset, to_little_endian, __mist_dmy_callback__( ) ) );
}






/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜�̕�
//! @param[in]  x                  �c ���͉摜�̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  __dmy__            �c �ǂݍ������Ƃ��Ă���f�[�^�̌^������킷�_�~�[�I�u�W�F�N�g�ishort�^�̃I�u�W�F�N�g�Ȃǁj
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x, typename array1< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	image.reso1( x );
	return( __raw_controller__::raw_controller::read( image, filename, w, 1, 1, offset, from_little_endian, 0, callback, __dmy__ ) );
}


/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜�̕�
//! @param[in]  x                  �c ���͉摜�̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x, typename array1< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array1< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, x, offset, from_little_endian, v, callback ) );
}


/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜�̕�
//! @param[in]  x                  �c ���͉摜�̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_raw( array1< T, Allocator > &image, const std::string &filename, typename array1< T, Allocator >::size_type w,
				double x = 1.0, typename array1< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, x, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}




/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜��X�������̃T�C�Y
//! @param[in]  h                  �c ���͉摜��Y�������̃T�C�Y
//! @param[in]  x                  �c ���͉摜��X�������̉�f�T�C�Y
//! @param[in]  y                  �c ���͉摜��Y�������̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  __dmy__            �c �ǂݍ������Ƃ��Ă���f�[�^�̌^������킷�_�~�[�I�u�W�F�N�g�ishort�^�̃I�u�W�F�N�g�Ȃǁj
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x, double y, typename array2< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	image.reso1( x );
	image.reso2( y );
	return( __raw_controller__::raw_controller::read( image, filename, w, h, 1, offset, from_little_endian, 0, callback, __dmy__ ) );
}


/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜��X�������̃T�C�Y
//! @param[in]  h                  �c ���͉摜��Y�������̃T�C�Y
//! @param[in]  x                  �c ���͉摜��X�������̉�f�T�C�Y
//! @param[in]  y                  �c ���͉摜��Y�������̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x, double y, typename array2< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array2< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, h, x, y, offset, from_little_endian, 0, v, callback ) );
}



/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜��X�������̃T�C�Y
//! @param[in]  h                  �c ���͉摜��Y�������̃T�C�Y
//! @param[in]  x                  �c ���͉摜��X�������̉�f�T�C�Y
//! @param[in]  y                  �c ���͉摜��Y�������̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_raw( array2< T, Allocator > &image, const std::string &filename,
				typename array2< T, Allocator >::size_type w, typename array2< T, Allocator >::size_type h,
				double x = 1.0, double y = 1.0, typename array2< T, Allocator >::value_type offset = 0, bool from_little_endian = false )
{
	return( read_raw( image, filename, w, h, x, y, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}




/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜��X�������̃T�C�Y
//! @param[in]  h                  �c ���͉摜��Y�������̃T�C�Y
//! @param[in]  d                  �c ���͉摜��Z�������̃T�C�Y
//! @param[in]  x                  �c ���͉摜��X�������̉�f�T�C�Y
//! @param[in]  y                  �c ���͉摜��Y�������̉�f�T�C�Y
//! @param[in]  z                  �c ���͉摜��Z�������̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  __dmy__            �c �ǂݍ������Ƃ��Ă���f�[�^�̌^������킷�_�~�[�I�u�W�F�N�g�ishort�^�̃I�u�W�F�N�g�Ȃǁj
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class ValueType, class Functor >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x, double y, double z, typename array3< T, Allocator >::value_type offset, bool from_little_endian, ValueType __dmy__, Functor callback )
{
	image.reso1( x );
	image.reso2( y );
	image.reso3( z );
	return( __raw_controller__::raw_controller::read( image, filename, w, h, d, offset, from_little_endian, 0, callback, __dmy__ ) );
}




/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜��X�������̃T�C�Y
//! @param[in]  h                  �c ���͉摜��Y�������̃T�C�Y
//! @param[in]  d                  �c ���͉摜��Z�������̃T�C�Y
//! @param[in]  x                  �c ���͉摜��X�������̉�f�T�C�Y
//! @param[in]  y                  �c ���͉摜��Y�������̉�f�T�C�Y
//! @param[in]  z                  �c ���͉摜��Z�������̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//! @param[in]  callback           �c �i�s�󋵂�ʒm����R�[���o�b�N�֐�
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator, class Functor >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x, double y, double z, typename array3< T, Allocator >::value_type offset, bool from_little_endian, Functor callback )
{
	typename array3< T, Allocator >::value_type v( 0 );
	return( read_raw( image, filename, w, h, d, x, y, z, offset, from_little_endian, v, callback ) );
}



/// @brief �����kRAW�CGZ���kRAW �摜��MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] image              �c �摜��ǂݍ��ސ��MIST�R���e�i
//! @param[in]  filename           �c ���̓t�@�C����
//! @param[in]  w                  �c ���͉摜��X�������̃T�C�Y
//! @param[in]  h                  �c ���͉摜��Y�������̃T�C�Y
//! @param[in]  d                  �c ���͉摜��Z�������̃T�C�Y
//! @param[in]  x                  �c ���͉摜��X�������̉�f�T�C�Y
//! @param[in]  y                  �c ���͉摜��Y�������̉�f�T�C�Y
//! @param[in]  z                  �c ���͉摜��Z�������̉�f�T�C�Y
//! @param[in]  offset             �c �摜�ɑ������ރI�t�Z�b�g�l
//! @param[in]  from_little_endian �c ���͉摜���L�^����Ă���`���i���g���G���f�B�A�� �c true�C�r�b�O�G���f�B�A�� �c false�j
//!
//! @retval true  �c �摜�̓ǂݍ��݂ɐ���
//! @retval false �c �摜�̓ǂݍ��݂Ɏ��s
//! 
template < class T, class Allocator >
bool read_raw( array3< T, Allocator > &image, const std::string &filename,
				typename array3< T, Allocator >::size_type w, typename array3< T, Allocator >::size_type h, typename array3< T, Allocator >::size_type d,
				double x = 1.0, double y = 1.0, double z = 1.0, typename array3< T, Allocator >::value_type offset = typename array3< T, Allocator >::value_type( 0 ), bool from_little_endian = false )
{
	return( read_raw( image, filename, w, h, d, x, y, z, offset, from_little_endian, __mist_dmy_callback__( ) ) );
}




/// @}
//  RAW�摜���o�̓O���[�v�̏I���

/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_RAW__
