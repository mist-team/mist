#ifndef __INCLUDE_MIST_DICOM_INFO__
#define __INCLUDE_MIST_DICOM_INFO__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "../config/endian.h"
#endif

#ifndef __INCLUDE_MIST_DICOM_TAG__
#include "./dicom_tag.h"
#endif

#include <iostream>
#include <string>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace dicom_controller
{
	enum compress_type
	{
		RAW,		// �����k
		JPEG,		// JPEG���k
		RLE,		// ���������O�X�iRLE�j���k
	};

	class dicom_element
	{
	public:
		dicom_tag tag;
		unsigned char *data;
		size_type num_bytes;

		void create( size_type nbytes )
		{
			if( num_bytes != nbytes )
			{
				release();
			}
			num_bytes = nbytes;
			data = new unsigned char[ num_bytes + 1 ];
			data[ num_bytes ] = '\0';
		}

		void copy( unsigned char *p, size_type nbytes )
		{
			if( num_bytes != nbytes )
			{
				release();
			}
			num_bytes = nbytes;
			data = new unsigned char[ num_bytes + 1 ];
			data[ num_bytes ] = '\0';
			memcpy( data, p, num_bytes );
		}

		void release()
		{
			delete [] data;
			data = NULL;
			num_bytes = 0;
		}

		bool operator <( const dicom_element &dicom ) const { return( tag < dicom.tag ); }
		const dicom_element &operator =( const dicom_element &dicom )
		{
			if( &dicom != this )
			{
				tag = dicom.tag;
				create( dicom.num_bytes );
				memcpy( data, dicom.data, sizeof( unsigned char ) * num_bytes );
			}
			return( *this );
		}

		// �ϊ��I�y���[�^
		double         to_double( ) const { return( ( tag.vr == FD && num_bytes == 8 )? byte_array< double >( data ).get_value( )        : static_cast< double >        ( atof( to_string( ).c_str( ) ) ) ); }
		float          to_float( )  const { return( ( tag.vr == FL && num_bytes == 4 )? byte_array< float >( data ).get_value( )         : static_cast< float >         ( atof( to_string( ).c_str( ) ) ) ); }
		signed int     to_int( )    const { return( ( tag.vr == SL && num_bytes == 4 )? byte_array< signed int >( data ).get_value( )    : static_cast< signed int >    ( atoi( to_string( ).c_str( ) ) ) ); }
		unsigned int   to_uint( )   const { return( ( tag.vr == UL && num_bytes == 4 )? byte_array< unsigned int >( data ).get_value( )  : static_cast< unsigned int >  ( atoi( to_string( ).c_str( ) ) ) ); }
		signed short   to_short( )  const { return( ( tag.vr == SS && num_bytes == 2 )? byte_array< signed short >( data ).get_value( )  : static_cast< signed short >  ( atoi( to_string( ).c_str( ) ) ) ); }
		unsigned short to_ushort( ) const { return( ( tag.vr == US && num_bytes == 2 )? byte_array< unsigned short >( data ).get_value( ): static_cast< unsigned short >( atoi( to_string( ).c_str( ) ) ) ); }
		std::string    to_string( ) const
		{
			static char buff[ 128 ];
			switch( tag.vr )
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

			default:
				return( std::string( reinterpret_cast< char * >( data ) ) );
				break;
			}
			return( buff );
		}

		void show_tag( ) const
		{
			if( data == NULL || num_bytes == 0 )
			{
				printf( "( %04x, %04x, %s, % 8d, %s ) = undefined!!\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str() );
			}
			else
			{
				switch( tag.vr )
				{
				case FL:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %f\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str(), to_float( ) );
					break;
				case FD:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %f\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str(), to_double( ) );
					break;
				case SL:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str(), to_int( ) );
					break;
				case SS:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str(), to_short( ) );
					break;
				case UL:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str(), to_uint( ) );
					break;
				case US:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %d\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str(), to_ushort( ) );
					break;

				case OB:
				case OW:
				case SQ:
				case UN:
					printf( "( %04x, %04x, %s, % 8d, %s ) = ...\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str() );
					break;

				default:
					printf( "( %04x, %04x, %s, % 8d, %s ) = %s\n", tag.get_group( ), tag.get_element( ), get_dicom_vr( tag.vr ).c_str(), static_cast< unsigned int >( num_bytes ), tag.comment.c_str(), data );
					break;
				}
			}
		}

		dicom_element( ) : tag( ), data( NULL ), num_bytes( 0 )
		{
		}
		dicom_element( const dicom_element &dicom ) : tag( dicom.tag ), data( NULL ), num_bytes( 0 )
		{
			create( dicom.num_bytes );
			memcpy( data, dicom.data, sizeof( unsigned char ) * num_bytes );
		}
		dicom_element( unsigned short group, unsigned short element, const unsigned char *d = NULL, size_type nbytes = 0 ) : tag( dicom_tag( construct_dicom_tag( group, element ), "", 1, "" ) ), data( NULL ), num_bytes( 0 )
		{
			create( nbytes );
			memcpy( data, d, sizeof( unsigned char ) * num_bytes );
		}
		dicom_element( const dicom_tag &t, const unsigned char *d = NULL, size_type nbytes = 0 ) : tag( t ), data( NULL ), num_bytes( 0 )
		{
			create( nbytes );
			memcpy( data, d, sizeof( unsigned char ) * num_bytes );
		}
	};


	class dicom_tag_container : public std::set< dicom_element >
	{
	private:
		typedef std::set< dicom_element > base;

	public:
		typedef base::iterator iterator;
		typedef base::const_iterator const_iterator;

	public:
		bool add( const dicom_element &element )
		{
			std::pair< iterator, bool > ite = base::insert( element );
			return( ite.second );
		}

		iterator append( const dicom_element &element )
		{
			std::pair< iterator, bool > ite = base::insert( element );
			return( ite.first );
		}

		void erase( const dicom_element &element )
		{
			base::erase( element );
		}

		iterator find( unsigned short group, unsigned short element )
		{
			return( base::find( dicom_element( construct_dicom_tag( group, element ) ) ) );
		}

		const_iterator find( unsigned short group, unsigned short element ) const
		{
			return( base::find( dicom_element( construct_dicom_tag( group, element ) ) ) );
		}

		bool contain( unsigned short group, unsigned short element ) const
		{
			return( find( group, element ) != base::end( ) );
		}

		dicom_tag_container( )
		{
		}
		dicom_tag_container( const dicom_tag_container &dicom ) : base( dicom )
		{
		}
	};


	class dicom_info
	{
	public:
		compress_type compression_type;

	public:
		dicom_info( compress_type c = RAW ) : compression_type( c ) { }
		dicom_info( const dicom_info &info ) : compression_type( info.compress_type ) { }

		const dicom_info &operator =( const dicom_info &info )
		{
			if( &info != this )
			{
				compression_type = info.compression_type;
			}
			return( *this );
		}
	};

	typedef struct
	{
		const char *uid;
		compress_type type;
	} __dicom_compress_type__;

	compress_type get_compress_type( const std::string &uid )
	{
		compress_type compress_type = RAW;
		static __dicom_compress_type__ compress_type_list[] = {
			{ "1.2.840.10008.1.2.4.50", JPEG, }, // JPEG ��i���� 1�j�F��t JPEG 8 �r�b�g�摜���k�p�f�t�H���g�]���\��
			{ "1.2.840.10008.1.2.4.51", JPEG, }, // JPEG �g���i���� 2 & 4�j�F��t JPEG 12 �r�b�g�摜���k�p�f�t�H���g�]���\���i����4�̂݁j
			{ "1.2.840.10008.1.2.4.52", JPEG, }, // JPEG �g���i���� 3 & 5�j
			{ "1.2.840.10008.1.2.4.53", JPEG, }, // JPEG �X�y�N�g���I���C��K�w�i���� 6 & 8�j
			{ "1.2.840.10008.1.2.4.54", JPEG, }, // JPEG �X�y�N�g���I���C��K�w�i���� 7 & 9�j
			{ "1.2.840.10008.1.2.4.55", JPEG, }, // JPEG �S����C��K�w�i���� 10 & 12�j
			{ "1.2.840.10008.1.2.4.56", JPEG, }, // JPEG �S����,��K�w�i���� 11 & 13�j
			{ "1.2.840.10008.1.2.4.57", JPEG, }, // JPEG �t,��K�w�i���� 14�j
			{ "1.2.840.10008.1.2.4.58", JPEG, }, // JPEG �t,��K�w�i���� 15�j
			{ "1.2.840.10008.1.2.4.59", JPEG, }, // JPEG �g��,�K�w�i���� 16 & 18�j
			{ "1.2.840.10008.1.2.4.60", JPEG, }, // JPEG �g��,�K�w�i���� 17 & 19�j
			{ "1.2.840.10008.1.2.4.61", JPEG, }, // JPEG �X�y�N�g���I��,�K�w�i���� 20 & 22�j
			{ "1.2.840.10008.1.2.4.62", JPEG, }, // JPEG �X�y�N�g���I���C�K�w�i���� 21 & 23�j
			{ "1.2.840.10008.1.2.4.63", JPEG, }, // JPEG �S����C�K�w�i���� 24 & 26�j
			{ "1.2.840.10008.1.2.4.64", JPEG, }, // JPEG �S����C�K�w�i���� 25 & 27�j
			{ "1.2.840.10008.1.2.4.65", JPEG, }, // JPEG �t�C�K�w�i���� 28�j
			{ "1.2.840.10008.1.2.4.66", JPEG, }, // JPEG �t�C�K�w�i���� 29�j
			{ "1.2.840.10008.1.2.4.70", JPEG, }, // JPEG �t�C��K�w�C�ꎟ�\���i���� 14 [�I��l 1]�j�F�t JPEG �摜���k�p�f�t�H���g�]���\��
			{ "1.2.840.10008.1.2.4.80", JPEG, }, // JPEG-LS �t�摜���k
			{ "1.2.840.10008.1.2.4.81", JPEG, }, // JPEG-LS ��t�i���t�j�摜���k
			{ "1.2.840.10008.1.2.4.90", JPEG, }, // JPEG 2000 Image Compression (Lossless Only)
			{ "1.2.840.10008.1.2.4.91", JPEG, }, // JPEG 2000 Image Compression
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
		}

		return( RAW );
	}


	// RLE���k�t�@�C���̃f�R�[�_
	inline bool decode_RLE( unsigned char *pointer, unsigned char *end_pointer, dicom_element &element )
	{
		return( false );
	}

}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_DICOM_INFO__
