// 
// Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/io/dicom_info.h
//!
//! @brief DICOM�摜��ǂݏ������邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_DICOM_INFO__
#define __INCLUDE_MIST_DICOM_INFO__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "../config/endian.h"
#endif

#ifndef __INCLUDE_MIST_SINGLETON__
#include "../singleton.h"
#endif

#ifndef __INCLUDE_MIST_DICOM_TAG__
#include "./dicom_tag.h"
#endif

#include <iostream>
#include <map>
#include <string>


// ���̃}�N�����`����ƁCJPEG���k���ꂽDICOM�̃f�R�[�h���\�ɂȂ�
// �������C�O����JPEG���C�u������K�v�Ƃ���̂Œ���
//#define __DECODE_JPEG_COMPRESSION__

#ifdef __DECODE_JPEG_COMPRESSION__

#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0

	#define XMD_H
	#define HAVE_INT32			// JPEG�pINT32�^�������Ă���錾
	#define HAVE_BOOLEAN		// JPEG�pboolean�^�������Ă���錾

#endif

extern "C"
{
	#include <jpeglib.h>
}

#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup image_group �摜���o��
//!  @{

//! @addtogroup dicom_group DICOM�摜���o��
//!  @{


/// DICOM�t�@�C���𑀍삷��֐��E�N���X���܂ޖ��O���
namespace dicom
{
	/// @brief DICOM�f�[�^�̈��k�^�C�v
	enum compress_type
	{
		RAW,		///< @brief �����k
		JPEG,		///< @brief JPEG���k
		JPEGLS,		///< @brief ���X���XJPEG���k
		JPEG2000,	///< @brief JPEG2000���k
		RLE,		///< @brief ���������O�X�iRLE�j���k
	};

	/// @brief DICOM�f�[�^�̉�f�̕\���^�C�v
	enum photometric_interpretation_type
	{
		UNKNOWNTYPE,
		MONOCHROME1,		///< @brief 1�o�C�g�̃O���[�X�P�[��
		MONOCHROME2,		///< @brief 2�o�C�g�̃O���[�X�P�[��
		RGB,				///< @brief RGB�̕���
		PALETTE_COLOR,		///< @brief �J���[�e�[�u��
		YBR_FULL_422,		///< @brief �H
		YBR_FULL,			///< @brief �H
	};

	/// @brief DICOM��UID�ɕϊ�����
	inline dicom_uid get_uid( const std::string &uid )
	{
		static dicom_uid_table uid_table;
		return( uid_table.get_uid( uid ) );
	}


	/// @brief DICOM��UID�ɕϊ�����
	inline dicom_uid get_uid( const unsigned char *str, difference_type numBytes )
	{
		return( get_uid( std::string( reinterpret_cast< const char * >( str ), str[ numBytes - 1 ] == 0 ? numBytes - 1 : numBytes ) ) );
	}


	/// @brief DICOM�̃^�O�Ƃ��̗v�f���Ǘ�����N���X
	class dicom_element : public dicom_tag
	{
	private:
		typedef dicom_tag base;
		
	public:
		unsigned char *data;	///< @brief DICOM�^�O�̓��e���w���|�C���^
		size_type num_bytes;	///< @brief DICOM�^�O�̓��e�ɕK�v�ȃo�C�g��


		/// @brief nbytes �o�C�g���̃������̈���m�ۂ���
		void create( size_type nbytes )
		{
			if( num_bytes != nbytes )
			{
				release();
			}
			num_bytes = nbytes;
			if( num_bytes != 0 )
			{
				data = new unsigned char[ num_bytes + 1 ];
				data[ num_bytes ] = '\0';
			}
		}


		/// @brief p ���� nbytes �o�C�g�������f�[�^���R�s�[����
		void copy( const unsigned char *p, size_type nbytes )
		{
			if( num_bytes != nbytes )
			{
				release();
			}
			num_bytes = nbytes;
			if( num_bytes != 0 )
			{
				data = new unsigned char[ num_bytes + 1 ];
				data[ num_bytes ] = '\0';
				memcpy( data, p, num_bytes );
			}
		}


		/// @brief �m�ۂ��Ă��郁�����̈���J������
		void release()
		{
			delete [] data;
			data = NULL;
			num_bytes = 0;
		}


		/// @brief DICOM�^�O�̑召�֌W�𒲂ׂ�
		bool operator <( const dicom_element &dicm ) const { return( base::operator <( dicm ) ); }


		/// @brief ����DICOM�v�f��������
		const dicom_element &operator =( const dicom_element &dicm )
		{
			if( &dicm != this )
			{
				base::operator =( dicm );
				create( dicm.num_bytes );
				memcpy( data, dicm.data, sizeof( unsigned char ) * num_bytes );
			}
			return( *this );
		}


		/// @brief ����DICOM�v�f��������
		void modify( const std::string &data )
		{
			switch( vr )
			{
			case FL:
				{
					float val = ( float )atof( data.c_str( ) );
					*this = dicom_element( get_group( ), get_element( ), reinterpret_cast< unsigned char * >( &val ), sizeof( float ) );
				}
				break;
			case FD:
				{
					double val = atof( data.c_str( ) );
					*this = dicom_element( get_group( ), get_element( ), reinterpret_cast< unsigned char * >( &val ), sizeof( double ) );
				}
				break;
			case SL:
				{
					long val = atoi( data.c_str( ) );
					*this = dicom_element( get_group( ), get_element( ), reinterpret_cast< unsigned char * >( &val ), sizeof( long ) );
				}
				break;
			case SS:
				{
					short val = ( short )atoi( data.c_str( ) );
					*this = dicom_element( get_group( ), get_element( ), reinterpret_cast< unsigned char * >( &val ), sizeof( short ) );
				}
				break;
			case UL:
				{
					unsigned long val = atoi( data.c_str( ) );
					*this = dicom_element( get_group( ), get_element( ), reinterpret_cast< unsigned char * >( &val ), sizeof( unsigned long ) );
				}
				break;
			case US:
				{
					unsigned short val = ( unsigned short )atoi( data.c_str( ) );
					*this = dicom_element( get_group( ), get_element( ), reinterpret_cast< unsigned char * >( &val ), sizeof( unsigned short ) );
				}
				break;

			default:
				*this = dicom_element( get_group( ), get_element( ), reinterpret_cast< const unsigned char * >( data.c_str( ) ), data.size( ) );
				break;
			}
		}


		/// @brief double �^�̃f�[�^�ɕϊ�����iDICOM�^�O�̓��e�� double �^������킵�Ă���K�v������j
		double         to_double( ) const { return( ( vr == FD && num_bytes == 8 )? byte_array< double >( data ).get_value( )         : static_cast< double >        ( atof( to_string( ).c_str( ) ) ) ); }

		/// @brief float �^�̃f�[�^�ɕϊ�����iDICOM�^�O�̓��e�� float �^������킵�Ă���K�v������j
		float          to_float( )  const { return( ( vr == FL && num_bytes == 4 )? byte_array< float >( data ).get_value( )          : static_cast< float >         ( atof( to_string( ).c_str( ) ) ) ); }

		/// @brief signed int �^�̃f�[�^�ɕϊ�����iDICOM�^�O�̓��e�� signed int �^������킵�Ă���K�v������j
		signed int     to_int( )    const { return( ( vr == SL && num_bytes == 4 )? byte_array< signed int >( data ).get_value( )     : static_cast< signed int >    ( atoi( to_string( ).c_str( ) ) ) ); }

		/// @brief unsigned int �^�̃f�[�^�ɕϊ�����iDICOM�^�O�̓��e�� unsigned int �^������킵�Ă���K�v������j
		unsigned int   to_uint( )   const { return( ( vr == UL && num_bytes == 4 )? byte_array< unsigned int >( data ).get_value( )   : static_cast< unsigned int >  ( atoi( to_string( ).c_str( ) ) ) ); }

		/// @brief signed short �^�̃f�[�^�ɕϊ�����iDICOM�^�O�̓��e�� signed short �^������킵�Ă���K�v������j
		signed short   to_short( )  const { return( ( vr == SS && num_bytes == 2 )? byte_array< signed short >( data ).get_value( )   : static_cast< signed short >  ( atoi( to_string( ).c_str( ) ) ) ); }

		/// @brief unsigned short �^�̃f�[�^�ɕϊ�����iDICOM�^�O�̓��e�� ushort �^������킵�Ă���K�v������j
		unsigned short to_ushort( ) const { return( ( vr == US && num_bytes == 2 )? byte_array< unsigned short >( data ).get_value( ) : static_cast< unsigned short >( atoi( to_string( ).c_str( ) ) ) ); }

		/// @brief string �^�̃f�[�^�ɕϊ�����iDICOM�^�O�̓��e�� string �^������킵�Ă���K�v������j
		std::string    to_string( ) const
		{
			if( data != NULL && num_bytes > 0 )
			{
				static char buff[ 128 ];
				switch( vr )
				{
				case FL:
					sprintf( buff, "%f", byte_array< float >( data ).get_value( ) );
					break;
				case FD:
					sprintf( buff, "%f", byte_array< double >( data ).get_value( ) );
					break;
				case SL:
					sprintf( buff, "%d", byte_array< signed int >( data ).get_value( ) );
					break;
				case SS:
					sprintf( buff, "%d", byte_array< signed short >( data ).get_value( ) );
					break;
				case UL:
					sprintf( buff, "%d", byte_array< unsigned int >( data ).get_value( ) );
					break;
				case US:
					sprintf( buff, "%d", byte_array< unsigned short >( data ).get_value( ) );
					break;

				case OB:
				case OW:
				case SQ:
				case UN:
					return( "..." );
					break;

				default:
					return( std::string( reinterpret_cast< char * >( data ) ) );
					break;
				}
				return( buff );
			}
			else
			{
				return( "empty" );
			}
		}

		/// @brief DICOM�^�O�̓��e��\������
		void show_tag( ) const
		{
			if( data == NULL || num_bytes == 0 )
			{
				printf( "( %04x, %04x, %s, % 8d, %s ) = undefined!!\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str() );
			}
			else if( vr == UI )
			{
				// DICOM��UID��ϊ�����
				dicom_uid uid = get_uid( std::string( reinterpret_cast< char * >( data ) ) );
				printf( "( %04x, %04x, %s, % 8d, %s ) = %s\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), uid.name.c_str( ) );
			}
			else
			{
				switch( vr )
				{
				case FL:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %f\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), to_float( ) );
					break;
				case FD:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %f\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), to_double( ) );
					break;
				case SL:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), to_int( ) );
					break;
				case SS:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), to_short( ) );
					break;
				case UL:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), to_uint( ) );
					break;
				case US:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), to_ushort( ) );
					break;

				case OB:
				case OW:
				case SQ:
				case UN:
					printf( "( %04x, %04x, %s, % 8d, %s ) = ...\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str() );
					break;

				default:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %s\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str(), data );
					break;
				}
			}
		}

		/// @brief �f�t�H���g�̃R���X�g���N�^
		dicom_element( ) : base( ), data( NULL ), num_bytes( 0 )
		{
		}

		/// @brief ����DICOM�^�O��p���ď���������
		dicom_element( const dicom_element &dicm ) : base( dicm ), data( NULL ), num_bytes( 0 )
		{
			copy( dicm.data, dicm.num_bytes );
		}

		/// @brief group�Celement�C�f�[�^ d�C�f�[�^�̃o�C�g�� nbytes ����p���ď���������
		dicom_element( unsigned short group, unsigned short element, const unsigned char *d = NULL, size_type nbytes = 0 )
						: base( singleton< dicom_tag_table >::get_instance( ).get_tag( group, element ) ), data( NULL ), num_bytes( 0 )
		{
			copy( d, nbytes );
		}

		/// @brief DICOM�^�O tag�C�f�[�^ d�C�f�[�^�̃o�C�g�� nbytes ����p���ď���������
		dicom_element( const dicom_tag &t, const unsigned char *d = NULL, size_type nbytes = 0 ) : base( t ), data( NULL ), num_bytes( 0 )
		{
			copy( d, nbytes );
		}

		/// @brief DICOM�^�O�p�Ɋm�ۂ����f�[�^�����ׂĊJ������
		~dicom_element( )
		{
			release( );
		}
	};


	/// @brief DICOM�^�O�̃f�[�^���Ǘ�����R���e�i
	class dicom_tag_container : public std::map< unsigned int, dicom_element >
	{
	private:
		typedef std::map< unsigned int, dicom_element > base;
		typedef std::pair< unsigned int, dicom_element > element_pair;

	public:
		typedef base::iterator iterator;				///< @brief ����������
		typedef base::const_iterator const_iterator;	///< @brief ����������
		typedef base::const_reference const_reference;	///< @brief ����������
		typedef base::reference reference;				///< @brief ����������

	public:
		/// @brief (group, element) �̃^�O���擾����
		dicom_element &operator ()( unsigned short group, unsigned short element ){ return( base::operator []( construct_dicom_tag( group, element ) ) ); }

		/// @brief (group, element) �̃^�O���擾����
		const dicom_element &operator ()( unsigned short group, unsigned short element ) const
		{
			const_iterator cite = find( group, element );
			return( cite->second );
		}

	public:
		/// @brief DICOM�f�[�^ element ��ǉ�����
		bool add( const dicom_element &element )
		{
			std::pair< iterator, bool > ite = base::insert( element_pair( element.tag, element ) );
			return( ite.second );
		}

		/// @brief DICOM�f�[�^ element ��ǉ�����
		iterator append( const dicom_element &element )
		{
			std::pair< iterator, bool > ite = base::insert( element_pair( element.tag, element ) );
			return( ite.first );
		}

		/// @brief DICOM�f�[�^ element ���폜����
		void erase( const dicom_element &element )
		{
			base::erase( element.tag );
		}

		/// @brief DICOM�f�[�^ element ���폜����
		void erase( const dicom_tag &tag )
		{
			base::erase( tag.tag );
		}

		/// @brief DICOM�f�[�^ element ���폜����
		void erase( unsigned short group, unsigned short element )
		{
			base::erase( construct_dicom_tag( group, element ) );
		}

		/// @brief DICOM�f�[�^ element ���폜����
		void remove( const dicom_element &element )
		{
			base::erase( element.tag );
		}

		/// @brief DICOM�f�[�^ element ���폜����
		void remove( const dicom_tag &tag )
		{
			base::erase( tag.tag );
		}

		/// @brief DICOM�f�[�^ element ���폜����
		void remove( unsigned short group, unsigned short element )
		{
			base::erase( construct_dicom_tag( group, element ) );
		}


		/// @brief (group, element) �̃^�O����������
		iterator find( unsigned short group, unsigned short element )
		{
			return( base::find( construct_dicom_tag( group, element ) ) );
		}

		/// @brief (group, element) �̃^�O����������
		const_iterator find( unsigned short group, unsigned short element ) const
		{
			return( base::find( construct_dicom_tag( group, element ) ) );
		}


		/// @brief (group, element) �̃^�O���܂܂�Ă��邩�𒲂ׂ�
		bool contain( unsigned short group, unsigned short element ) const
		{
			return( find( group, element ) != base::end( ) );
		}

		dicom_tag_container( )
		{
		}
		dicom_tag_container( const dicom_tag_container &dicm ) : base( dicm )
		{
		}
	};

	/// @brief DICOM�̉摜�����Ǘ�����
	class dicom_image_info
	{
	public:
		compress_type						compression_type;	///< @brief DICOM�f�[�^�̈��k�^�C�v
		photometric_interpretation_type		photometric_type;	///< @brief ��f�̕\�����@

		unsigned short		samples_per_pixel;			///< @brief 1��f������̃T���v����
		signed int			number_of_frames;			///< @brief DICOM�摜���Ɋ܂܂��t���[����
		unsigned short		rows;						///< @brief �摜�̍s���i�摜�̍����j
		unsigned short		cols;						///< @brief �摜�̗񐔁i�摜�̕��j
		double				pixel_spacing_x;			///< @brief X�������̉𑜓x
		double				pixel_spacing_y;			///< @brief Y�������̉𑜓x
		double				image_position_x;			///< @brief X�������̃C���[�W�|�W�V����
		double				image_position_y;			///< @brief Y�������̃C���[�W�|�W�V����
		double				image_position_z;			///< @brief Z�������̃C���[�W�|�W�V����
		double				rescale_intercept;			///< @brief ��f�l�̃I�t�Z�b�g
		unsigned short		bits_allocated;				///< @brief 1��f������Ɋ��蓖�Ă��Ă���r�b�g��
		unsigned short		bits_stored;				///< @brief 1��f������Ɏg�p���Ă���r�b�g��
		unsigned short		high_bits;					///< @brief high bits
		unsigned short		pixel_representation;		///< @brief pixel representation
		double				window_center;				///< @brief �`��ɗp���� Window Center
		double				window_width;				///< @brief �`��ɗp���� Window Width

		double				KVP;						///< @brief �Ǔd��
		double				mA;							///< @brief �Ǔd��
		double				thickness;					///< @brief �X���C�X��

		dicom_image_info( ) :
			compression_type( RAW ),
			photometric_type( UNKNOWNTYPE ),
			samples_per_pixel( 1 ),
			number_of_frames( 1 ),
			rows( 0 ),
			cols( 0 ),
			pixel_spacing_x( 1.0 ),
			pixel_spacing_y( 1.0 ),
			image_position_x( 1.0 ),
			image_position_y( 1.0 ),
			image_position_z( 1.0 ),
			rescale_intercept( 0.0 ),
			bits_allocated( 8 ),
			bits_stored( 8 ),
			high_bits( 7 ),
			pixel_representation( 0 ),
			window_center( 128 ),
			window_width( 256 ),
			KVP( 0.0 ),
			mA( 0.0 ),
			thickness( 0.0 )
		{
		}
	};


	/// @brief DICOM�̑S�����Ǘ�����
	class dicom_info : public dicom_image_info
	{
	public:
		bool			little_endian_encoding;		///< @brief �f�[�^�����g���G���f�B�A���`�����ǂ���
		std::string		study_instance_uid;			///< @brief �����̎��ʗp���j�[�NID
		std::string		series_instance_uid;		///< @brief �V���[�Y�̎��ʗp���j�[�NID
		std::string		study_id;					///< @brief ����ID
		std::string		patient_id;					///< @brief ����ID
		int				series_number;				///< @brief �V���[�Y�ԍ�
		int				acquisition_number;			///< @brief ���W�ԍ�
		int				instance_number;			///< @brief �C���X�^���X�ԍ�

	public:
		/// @brief �f�t�H���g�̃R���X�g���N�^
		dicom_info( ) :
			little_endian_encoding( true ),
			study_instance_uid( "0" ),
			series_instance_uid( "0" ),
			study_id( "0" ),
			patient_id( "0" ),
			series_number( 0 ),
			acquisition_number( 0 ),
			instance_number( 0 )
		{
		}

		/// @brief ���̃I�u�W�F�N�g�ŏ���������
		dicom_info( const dicom_info &info ) :
			dicom_image_info( info ),
			little_endian_encoding( info.little_endian_encoding ),
			study_instance_uid( info.study_instance_uid ),
			series_instance_uid( info.series_instance_uid ),
			study_id( info.study_id ),
			patient_id( info.patient_id ),
			series_number( info.series_number ),
			acquisition_number( info.acquisition_number ),
			instance_number( info.instance_number )
		{
		}


		/// @brief ���̃I�u�W�F�N�g��������
		const dicom_info &operator =( const dicom_info &info )
		{
			if( &info != this )
			{
				dicom_image_info::operator =( info );
				little_endian_encoding = info.little_endian_encoding;
				study_instance_uid = info.study_instance_uid;
				series_instance_uid = info.series_instance_uid;
				study_id = info.study_id;
				patient_id = info.patient_id;
				series_number = info.series_number;
				acquisition_number = info.acquisition_number;
				instance_number = info.instance_number;
			}
			return( *this );
		}
	};

	typedef struct
	{
		const char *uid;
		compress_type type;
	} __dicom_compress_type__;


	/// @brief UID�����Ɉ��k�^�C�v���擾����
	compress_type get_compress_type( const std::string &uid )
	{
		static __dicom_compress_type__ compress_type_list[] = {
			{ "1.2.840.10008.1.2.4.50", JPEG, }, // JPEG ��i���� 1�j�F��t JPEG 8 �r�b�g�摜���k�p�f�t�H���g�]���\��
			{ "1.2.840.10008.1.2.4.51", JPEG, }, // JPEG �g���i���� 2 & 4�j�F��t JPEG 12 �r�b�g�摜���k�p�f�t�H���g�]���\���i����4�̂݁j
			{ "1.2.840.10008.1.2.4.52", JPEG, }, // JPEG �g���i���� 3 & 5�j
			{ "1.2.840.10008.1.2.4.53", JPEG, }, // JPEG �X�y�N�g���I���C��K�w�i���� 6 & 8�j
			{ "1.2.840.10008.1.2.4.54", JPEG, }, // JPEG �X�y�N�g���I���C��K�w�i���� 7 & 9�j
			{ "1.2.840.10008.1.2.4.55", JPEG, }, // JPEG �S����C��K�w�i���� 10 & 12�j
			{ "1.2.840.10008.1.2.4.56", JPEG, }, // JPEG �S����,��K�w�i���� 11 & 13�j
			{ "1.2.840.10008.1.2.4.57", JPEGLS, }, // JPEG �t,��K�w�i���� 14�j
			{ "1.2.840.10008.1.2.4.58", JPEGLS, }, // JPEG �t,��K�w�i���� 15�j
			{ "1.2.840.10008.1.2.4.59", JPEG, }, // JPEG �g��,�K�w�i���� 16 & 18�j
			{ "1.2.840.10008.1.2.4.60", JPEG, }, // JPEG �g��,�K�w�i���� 17 & 19�j
			{ "1.2.840.10008.1.2.4.61", JPEG, }, // JPEG �X�y�N�g���I��,�K�w�i���� 20 & 22�j
			{ "1.2.840.10008.1.2.4.62", JPEG, }, // JPEG �X�y�N�g���I���C�K�w�i���� 21 & 23�j
			{ "1.2.840.10008.1.2.4.63", JPEG, }, // JPEG �S����C�K�w�i���� 24 & 26�j
			{ "1.2.840.10008.1.2.4.64", JPEG, }, // JPEG �S����C�K�w�i���� 25 & 27�j
			{ "1.2.840.10008.1.2.4.65", JPEGLS, }, // JPEG �t�C�K�w�i���� 28�j
			{ "1.2.840.10008.1.2.4.66", JPEGLS, }, // JPEG �t�C�K�w�i���� 29�j
			{ "1.2.840.10008.1.2.4.70", JPEGLS, }, // JPEG �t�C��K�w�C�ꎟ�\���i���� 14 [�I��l 1]�j�F�t JPEG �摜���k�p�f�t�H���g�]���\��
			{ "1.2.840.10008.1.2.4.80", JPEGLS, }, // JPEG-LS �t�摜���k
			{ "1.2.840.10008.1.2.4.81", JPEGLS, }, // JPEG-LS ��t�i���t�j�摜���k
			{ "1.2.840.10008.1.2.4.90", JPEG2000, }, // JPEG 2000 Image Compression (Lossless Only)
			{ "1.2.840.10008.1.2.4.91", JPEG2000, }, // JPEG 2000 Image Compression
			{ "1.2.840.10008.1.2.5",     RLE, }, // Run Length Encoding
			{ NULL, RAW, }, 
		};

		__dicom_compress_type__ *list = compress_type_list;
		while( list->uid != NULL )
		{
			if( uid == list->uid )
			{
				return( list->type );
			}
			list++;
		}

		return( RAW );
	}


	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	double find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, double default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_double( ) );
	}

	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	float find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, float default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_float( ) );
	}

	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	signed int find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, signed int default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_int( ) );
	}

	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	unsigned int find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, unsigned int default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_uint( ) );
	}

	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	signed short find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, signed short default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_short( ) );
	}

	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	unsigned short find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, unsigned short default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_ushort( ) );
	}

	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	std::string find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, const std::string &default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_string( ) );
	}

	/// @brief dicm�R���e�i���� (group, element)�̃f�[�^�����݂��邩�������Ă��̒l��Ԃ��i������Ȃ����� default_value ��Ԃ��j
	std::string find_tag( const dicom_tag_container &dicm, unsigned short group, unsigned short element, const char *default_value )
	{
		dicom_tag_container::const_iterator cite = dicm.find( group, element );
		return( cite == dicm.end( ) ? default_value : cite->second.to_string( ) );
	}

	bool begin_with( const std::string &str1, const std::string &str2 )
	{
		if( str1.size( ) < str2.size( ) )
		{
			return( false );
		}

		for( size_t i = 0 ; i < str2.size( ) ; i++ )
		{
			if( str1[ i ] != str2[ i ] )
			{
				return( false );
			}
		}

		return( true );
	}

	/// @brief DICOM�R���e�i����DICOM�̏����擾����
	bool get_dicom_info( const dicom_tag_container &dicm, dicom_info &info )
	{
		info.little_endian_encoding	= find_tag( dicm, 0x0002, 0x0010, "" ) != "1.2.840.10008.1.2.2";

		info.compression_type		= get_compress_type( find_tag( dicm, 0x0002, 0x0010, "" ) );
		info.samples_per_pixel		= find_tag( dicm, 0x0028, 0x0002, info.samples_per_pixel );
		info.number_of_frames		= find_tag( dicm, 0x0028, 0x0008, info.number_of_frames );
		info.rows					= find_tag( dicm, 0x0028, 0x0010, info.rows );	// X�������̉𑜓x
		info.cols					= find_tag( dicm, 0x0028, 0x0011, info.cols );	// Y�������̉𑜓x

		// ��f�̕\�����@
		std::string photometric_interpretation = find_tag( dicm, 0x0028, 0x0004, "" );
		if( begin_with( photometric_interpretation, "MONOCHROME1" ) )
		{
			info.photometric_type = MONOCHROME1;
		}
		else if( begin_with( photometric_interpretation, "MONOCHROME2" ) )
		{
			info.photometric_type = MONOCHROME2;
		}
		else if( begin_with( photometric_interpretation, "RGB" ) )
		{
			info.photometric_type = RGB;
		}
		else if( begin_with( photometric_interpretation, "PALETTE COLOR" ) )
		{
			info.photometric_type = PALETTE_COLOR;
		}
		else if( begin_with( photometric_interpretation, "YBR FULL 422" ) )
		{
			info.photometric_type = YBR_FULL_422;
		}
		else if( begin_with( photometric_interpretation, "YBR FULL" ) )
		{
			info.photometric_type = YBR_FULL;
		}


		// �摜��XY�����̉𑜓x
		std::string pixel_spacing	= find_tag( dicm, 0x0028, 0x0030, "" );
		if( pixel_spacing != "" )
		{
			double resoX = 1.0, resoY = 1.0;
			sscanf( pixel_spacing.c_str( ), "%lf\\%lf", &resoX, &resoY );
			info.pixel_spacing_x	= resoX;
			info.pixel_spacing_y	= resoY;
		}

		// �C���[�W�|�W�V�������擾
		std::string image_position	= find_tag( dicm, 0x0020, 0x0032, "" );
		if( image_position != "" )
		{
			double posX = 0.0, posY = 0.0, posZ = 0.0;
			sscanf( image_position.c_str( ), "%lf\\%lf\\%lf", &posX, &posY, &posZ );
			info.image_position_x = posX;
			info.image_position_y = posY;
			info.image_position_z = posZ;
		}
		else
		{
			info.image_position_z = find_tag( dicm, 0x2020, 0x0010, ( short )0 );
		}

		// �Ǔd�����擾
		std::string kvp				= find_tag( dicm, 0x0018, 0x0060, "" );
		if( kvp != "" )
		{
			info.KVP = atof( kvp.c_str( ) );
		}

		// �Ǔd�����擾
		std::string mA				= find_tag( dicm, 0x0018, 0x1151, "" );
		if( mA != "" )
		{
			info.mA = atoi( mA.c_str( ) );
		}

		// �X���C�X�����擾
		info.thickness = find_tag( dicm, 0x0018, 0x0050, info.thickness );

		// ��f�ɑ΂���I�t�Z�b�g���擾
		std::string rescale_intercept	= find_tag( dicm, 0x0028, 0x1052, "" );
		if( rescale_intercept != "" )
		{
			info.rescale_intercept = atof( rescale_intercept.c_str( ) );
		}

		info.bits_allocated			= find_tag( dicm, 0x0028, 0x0100, info.bits_allocated );
		info.bits_stored			= find_tag( dicm, 0x0028, 0x0101, info.bits_stored );
		info.high_bits				= find_tag( dicm, 0x0028, 0x0102, info.high_bits );
		info.pixel_representation	= find_tag( dicm, 0x0028, 0x0103, info.pixel_representation );
		info.window_center			= find_tag( dicm, 0x0028, 0x1050, info.window_center );
		info.window_width			= find_tag( dicm, 0x0028, 0x1051, info.window_width );

		// �f�[�^�̃V���[�Y�����ʂ���f�[�^���擾
		info.study_instance_uid		= find_tag( dicm, 0x0020, 0x000D, info.study_instance_uid );
		info.series_instance_uid	= find_tag( dicm, 0x0020, 0x000D, info.series_instance_uid );
		info.series_number			= find_tag( dicm, 0x0020, 0x0011, info.series_number );
		info.acquisition_number		= find_tag( dicm, 0x0020, 0x0012, info.acquisition_number );
		info.instance_number		= find_tag( dicm, 0x0020, 0x0013, info.instance_number );

		info.study_id				= find_tag( dicm, 0x0020, 0x0010, info.study_id );
		info.patient_id				= find_tag( dicm, 0x0010, 0x0020, info.patient_id );

		return( true );
	}

	/// @brief �v�f�^�O�̊J�n�ʒu���ǂ���
	//! 
	//! @param[in] p �c �擪�|�C���^
	//! @param[in] e �c �����|�C���^
	//! 
	//! @retval true  �c �v�f�^�O�̏ꍇ
	//! @retval false �c ����ȊO
	//! 
	inline bool is_element_begin( const unsigned char *p, const unsigned char *e )
	{
		if( p + 4 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x00 && p[ 3 ] == 0xe0 );
	}

	/// @brief �v�f�^�O�̏I���ʒu���ǂ���
	//! 
	//! @param[in] p �c �擪�|�C���^
	//! @param[in] e �c �����|�C���^
	//! 
	//! @retval true  �c �v�f�I���^�O�̏ꍇ
	//! @retval false �c ����ȊO
	//! 
	inline bool is_element_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0xdd && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}


	/// @brief RLE���k�t�@�C���̃f�R�[�_
	//! 
	//! @param[in] psrc     �c ���̓f�[�^�̐擪�|�C���^
	//! @param[in] psrc_end �c ���̓f�[�^�̖����|�C���^
	//! @param[in] pdst     �c �o�̓f�[�^�̐擪�|�C���^
	//! @param[in] pdst_end �c �o�̓f�[�^�̖����|�C���^
	//! @param[in]  from_little_endian �c ���̓f�[�^�����g���G���f�B�A�����ǂ���
	//! 
	//! @return �o�̓f�[�^���̃f�R�[�h�I���ʒu
	//! 
	inline unsigned char *decode_RLE( unsigned char *psrc, unsigned char *psrc_end, unsigned char *pdst, unsigned char *pdst_end, bool from_little_endian = true )
	{
		if( psrc + 64 >= psrc_end )
		{
			// RLE���k���������Ă��܂���
			return( NULL );
		}

		// RLE�̃w�b�_����ǂݍ���
		size_type number_of_segments = to_current_endian( byte_array< unsigned int >( psrc ), true ).get_value( );
		difference_type frame_offset[ 15 ] = {
			to_current_endian( byte_array< unsigned int >( psrc +  4 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc +  8 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 12 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 16 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 20 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 24 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 28 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 32 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 36 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 40 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 44 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 48 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 52 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 56 ), from_little_endian ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 60 ), from_little_endian ).get_value( ),
		};

		if( frame_offset[ 0 ] != 64 )
		{
			frame_offset[ 0 ] = 64;
		}

		size_type n = 0;
		while( n < number_of_segments && psrc < psrc_end && pdst < pdst_end )
		{
			char *p = reinterpret_cast< char * >( psrc + frame_offset[ n ] );
			char *e = reinterpret_cast< char * >( n == number_of_segments - 1 ? psrc_end : psrc + frame_offset[ n + 1 ] );
			while( p < e && pdst < pdst_end )
			{
				difference_type num = *p++;
				if( 0 <= num && num <= 127 )
				{
					num = num + 1;
					if( p + num <= e && pdst + num <= pdst_end )
					{
						for( difference_type i = 0 ; i < num ; i++ )
						{
							pdst[ i ] = p[ i ];
						}
					}
					p += num;
					pdst += num;
				}
				else if( -127 <= num && num <= -1 )
				{
					num = 1 - num;
					if( p + 1 <= e && pdst + num <= pdst_end )
					{
						for( difference_type i = 0 ; i < num ; i++ )
						{
							pdst[ i ] = *p;
						}
					}
					p++;
					pdst += num;
				}
			}
			n++;
		}

		return( pdst );
	}

#ifdef __DECODE_JPEG_COMPRESSION__
	static void JpegInitSource( j_decompress_ptr dinfo )
	{
	}

	static boolean JpegFillInputBuffer( j_decompress_ptr dinfo )
	{
		return TRUE;
	}

	static void JpegSkipInputData( j_decompress_ptr dinfo, long num_bytes )
	{
		jpeg_source_mgr &jpegSrcManager = *( dinfo->src );
		jpegSrcManager.next_input_byte += (size_t) num_bytes;
		jpegSrcManager.bytes_in_buffer -= (size_t) num_bytes;
	}

	static void JpegTermSource( j_decompress_ptr dinfo )
	{
		/* No work necessary here. */
	}
#endif

	/// @brief JPEG���k�t�@�C���̃f�R�[�_
	//! 
	//! @param[in] psrc     �c ���̓f�[�^�̐擪�|�C���^
	//! @param[in] psrc_end �c ���̓f�[�^�̖����|�C���^
	//! @param[in] pdst     �c �o�̓f�[�^�̐擪�|�C���^
	//! @param[in] pdst_end �c �o�̓f�[�^�̖����|�C���^
	//! 
	//! @return �o�̓f�[�^���̃f�R�[�h�I���ʒu
	//! 
	inline unsigned char *decode_JPEG( unsigned char *psrc, unsigned char *psrc_end, unsigned char *pdst, unsigned char *pdst_end )
	{
#ifdef __DECODE_JPEG_COMPRESSION__
		difference_type compressedLen = psrc_end - psrc;
		if( compressedLen <= 0 )
		{
			return( NULL );
		}

		JDIMENSION i, j;
		JSAMPROW bitmap[1];				// �r�b�g�}�b�v�f�[�^�z��ւ̃|�C���^�[
		jpeg_decompress_struct dinfo;	// JPEG�𓀏��\����
		jpeg_error_mgr jerr;			// JPEG�G���[�����p�\����
		int scanlen, c;					// �r�b�g�}�b�v�P�s�̃o�C�g��

		dinfo.err = jpeg_std_error( &jerr );
		jpeg_create_decompress( &dinfo );

		jpeg_source_mgr jpegSrcManager;
		jpegSrcManager.init_source = JpegInitSource;
		jpegSrcManager.fill_input_buffer = JpegFillInputBuffer;
		jpegSrcManager.skip_input_data = JpegSkipInputData;
		jpegSrcManager.resync_to_restart = jpeg_resync_to_restart;
		jpegSrcManager.term_source = JpegTermSource;
		jpegSrcManager.next_input_byte = psrc;
		jpegSrcManager.bytes_in_buffer = compressedLen;
		dinfo.src = &jpegSrcManager;

		jpeg_read_header( &dinfo, TRUE );
//		dinfo.out_color_space = JCS_RGB;

		jpeg_start_decompress( &dinfo );

		scanlen = dinfo.output_width * dinfo.output_components;

		JSAMPLE *buffer = new JSAMPLE[ scanlen ];
		for( j = 0 ; j < dinfo.output_height ; j++ )
		{
			bitmap[ 0 ] = &buffer[ 0 ];
			if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
			for( i = 0 ; i < dinfo.output_width ; i++ )
			{
				for( c = 0 ; c < dinfo.output_components ; c++ )
				{
					*pdst++ = buffer[ i * dinfo.output_components + c ];
				}
			}
		}

		jpeg_finish_decompress(&dinfo);
		jpeg_destroy_decompress(&dinfo);

		return( pdst );
#else
		return( NULL );
#endif
	}


	/// @brief ���k�t�@�C���̃f�R�[�_
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] element �c �f�R�[�h����DICOM�f�[�^
	//! @param[in] info    �c DICOM�̈��k�Ɋւ�����
	//! 
	//! @retval true  �c �f�R�[�h�ɐ���
	//! @retval false �c ���Ή��̈��k�^�C�v
	//! 
	inline bool decode( dicom_element &element, const dicom_info &info )
	{
		switch( info.compression_type )
		{
		case RAW:
			return( true );

//#ifndef __DECODE_JPEG_COMPRESSION__
//		case JPEG:
//		case JPEGLS:
//		case JPEG2000:
//			// ���̂Ƃ��떢�T�|�[�g
//			return( false );
//#endif

		default:
			break;
		}

		if( element.num_bytes < 8 + 8 )
		{
			// ���k���������Ă��܂���
			return( false );
		}

		unsigned char *pointer = element.data;
		unsigned char *end_pointer = element.data + element.num_bytes;
		difference_type num_bytes = element.num_bytes;
		difference_type frame_offset[ 16 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		size_type number_of_fragments = 0;

		// �x�[�V�b�N�I�t�Z�b�g�e�[�u�����X�L�b�v
		if( !is_element_begin( pointer, end_pointer ) )
		{
			return( false );
		}
		pointer += 4;
		num_bytes = to_current_endian( byte_array< unsigned int >( pointer ), info.little_endian_encoding ).get_value( );
		pointer += 4;
		if( num_bytes > 0 )
		{
			// �e�t���[���ւ̃|�C���^���������ꂽ�̂Œ���
			unsigned char *p = pointer;
			while( p < pointer + num_bytes && number_of_fragments < 16 )
			{
				frame_offset[ number_of_fragments++ ] = to_current_endian( byte_array< unsigned int >( p ), info.little_endian_encoding ).get_value( );
				p += 4;
			}
		}
		else
		{
			number_of_fragments = 1;
		}
		if( num_bytes < 0 )
		{
			return( false );
		}
		pointer += num_bytes;
		if( pointer > end_pointer )
		{
			return( false );
		}

		size_type dstBytes = info.rows * info.cols * info.number_of_frames * info.samples_per_pixel * info.bits_allocated / 8;
		unsigned char *buff = new unsigned char[ dstBytes + 1 ];
		unsigned char *dst_pointer = buff;
		unsigned char *p = pointer;
		bool ret = true;

		// �x�[�V�b�N�I�t�Z�b�g�e�[�u�����X�L�b�v
		size_type i = 0;
		while( dst_pointer < buff + dstBytes && i < number_of_fragments && ret )
		{
			p = pointer + frame_offset[ i ];
			if( !is_element_begin( p, end_pointer ) )
			{
				return( false );
			}
			p += 4;
			num_bytes = to_current_endian( byte_array< unsigned int >( p ), info.little_endian_encoding ).get_value( );
			p += 4;

			switch( info.compression_type )
			{
			case RLE:
				dst_pointer = decode_RLE( p, p + num_bytes, dst_pointer, buff + dstBytes, info.little_endian_encoding );
				if( dst_pointer == NULL )
				{
					ret = false;
				}
				break;

			case JPEG:
			case JPEGLS:
			case JPEG2000:
				memcpy( dst_pointer, p, num_bytes );
				dst_pointer += num_bytes;
				break;

			default:
				break;
			}
			i++;
		}

		if( ret )
		{
			element.copy( buff, dst_pointer - buff );
		}

		delete [] buff;
		return( ret );
	}

}


/// @}
//  DICOM�摜���o�̓O���[�v�̏I���


/// @}
//  �摜���o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DICOM_INFO__
