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
#include <map>
#include <string>


// mist名前空間の始まり
_MIST_BEGIN


namespace dicom_controller
{
	enum compress_type
	{
		RAW,		// 無圧縮
		JPEG,		// JPEG圧縮
		RLE,		// ランレングス（RLE）圧縮
	};

	class dicom_element : public dicom_tag
	{
	private:
		typedef dicom_tag base;
		
	public:
		unsigned char *data;
		size_type num_bytes;

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

		void copy( unsigned char *p, size_type nbytes )
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

		void release()
		{
			delete [] data;
			data = NULL;
			num_bytes = 0;
		}

		bool operator <( const dicom_element &dicom ) const { return( base::operator <( dicom ) ); }
		const dicom_element &operator =( const dicom_element &dicom )
		{
			if( &dicom != this )
			{
				base::operator =( dicom );
				create( dicom.num_bytes );
				memcpy( data, dicom.data, sizeof( unsigned char ) * num_bytes );
			}
			return( *this );
		}

		// 変換オペレータ
		double         to_double( ) const { return( ( vr == FD && num_bytes == 8 )? byte_array< double >( data ).get_value( )        : static_cast< double >        ( atof( to_string( ).c_str( ) ) ) ); }
		float          to_float( )  const { return( ( vr == FL && num_bytes == 4 )? byte_array< float >( data ).get_value( )         : static_cast< float >         ( atof( to_string( ).c_str( ) ) ) ); }
		signed int     to_int( )    const { return( ( vr == SL && num_bytes == 4 )? byte_array< signed int >( data ).get_value( )    : static_cast< signed int >    ( atoi( to_string( ).c_str( ) ) ) ); }
		unsigned int   to_uint( )   const { return( ( vr == UL && num_bytes == 4 )? byte_array< unsigned int >( data ).get_value( )  : static_cast< unsigned int >  ( atoi( to_string( ).c_str( ) ) ) ); }
		signed short   to_short( )  const { return( ( vr == SS && num_bytes == 2 )? byte_array< signed short >( data ).get_value( )  : static_cast< signed short >  ( atoi( to_string( ).c_str( ) ) ) ); }
		unsigned short to_ushort( ) const { return( ( vr == US && num_bytes == 2 )? byte_array< unsigned short >( data ).get_value( ): static_cast< unsigned short >( atoi( to_string( ).c_str( ) ) ) ); }
		std::string    to_string( ) const
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
				printf( "( %04x, %04x, %s, % 8d, %s ) = undefined!!\n", get_group( ), get_element( ), get_dicom_vr( vr ).c_str(), static_cast< unsigned int >( num_bytes ), comment.c_str() );
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

		dicom_element( ) : base( ), data( NULL ), num_bytes( 0 )
		{
		}
		dicom_element( const dicom_element &dicom ) : base( dicom ), data( NULL ), num_bytes( 0 )
		{
			create( dicom.num_bytes );
			memcpy( data, dicom.data, sizeof( unsigned char ) * num_bytes );
		}
		dicom_element( unsigned short group, unsigned short element, const unsigned char *d = NULL, size_type nbytes = 0 )
						: base( construct_dicom_tag( group, element ), "", 1, "" ), data( NULL ), num_bytes( 0 )
		{
			create( nbytes );
			memcpy( data, d, sizeof( unsigned char ) * num_bytes );
		}
		dicom_element( const dicom_tag &t, const unsigned char *d = NULL, size_type nbytes = 0 ) : base( t ), data( NULL ), num_bytes( 0 )
		{
			create( nbytes );
			memcpy( data, d, sizeof( unsigned char ) * num_bytes );
		}
	};


	class dicom_tag_container : public std::map< unsigned int, dicom_element >
	{
	private:
		typedef std::map< unsigned int, dicom_element > base;
		typedef std::pair< unsigned int, dicom_element > element_pair;

	public:
		typedef base::iterator iterator;
		typedef base::const_iterator const_iterator;
		typedef base::const_reference const_reference;
		typedef base::reference reference;

	public:
		mapped_type &operator ()( unsigned short group, unsigned short element ){ return( base::operator []( construct_dicom_tag( group, element ) ) ); }
		const mapped_type &operator ()( unsigned short group, unsigned short element ) const
		{
			const_iterator cite = find( group, element );
			return( cite->second );
		}

	public:
		bool add( const dicom_element &element )
		{
			std::pair< iterator, bool > ite = base::insert( element_pair( element.tag, element ) );
			return( ite.second );
		}

		iterator append( const dicom_element &element )
		{
			std::pair< iterator, bool > ite = base::insert( element_pair( element.tag, element ) );
			return( ite.first );
		}

		void erase( const dicom_element &element )
		{
			base::erase( element.tag );
		}

		iterator find( unsigned short group, unsigned short element )
		{
			return( base::find( construct_dicom_tag( group, element ) ) );
		}

		const_iterator find( unsigned short group, unsigned short element ) const
		{
			return( base::find( construct_dicom_tag( group, element ) ) );
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

	class dicom_image_info
	{
	public:
		compress_type	compression_type;
		unsigned short	samples_per_pixel;
		signed int		number_of_frames;
		unsigned short	rows;
		unsigned short	cols;
		double			pixel_spacing_x;
		double			pixel_spacing_y;
		unsigned short	bits_allocated;
		unsigned short	bits_stored;
		unsigned short	high_bits;
		unsigned short	pixel_representation;
		double			window_center;
		double			window_width;

		dicom_image_info( ) :
			compression_type( RAW ),
			samples_per_pixel( 1 ),
			number_of_frames( 1 ),
			rows( 0 ),
			cols( 0 ),
			pixel_spacing_x( 1.0 ),
			pixel_spacing_y( 1.0 ),
			bits_allocated( 8 ),
			bits_stored( 8 ),
			high_bits( 7 ),
			pixel_representation( 0 ),
			window_center( 128 ),
			window_width( 256 )
		{
		}
	};

	class dicom_info : public dicom_image_info
	{
	public:

	public:
		dicom_info( ){ }
		dicom_info( const dicom_info &info ) : dicom_image_info( info ) { }

		const dicom_info &operator =( const dicom_info &info )
		{
			if( &info != this )
			{
				dicom_image_info::operator =( info );
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
		static __dicom_compress_type__ compress_type_list[] = {
			{ "1.2.840.10008.1.2.4.50", JPEG, }, // JPEG 基準（処理 1）：非可逆 JPEG 8 ビット画像圧縮用デフォルト転送構文
			{ "1.2.840.10008.1.2.4.51", JPEG, }, // JPEG 拡張（処理 2 & 4）：非可逆 JPEG 12 ビット画像圧縮用デフォルト転送構文（処理4のみ）
			{ "1.2.840.10008.1.2.4.52", JPEG, }, // JPEG 拡張（処理 3 & 5）
			{ "1.2.840.10008.1.2.4.53", JPEG, }, // JPEG スペクトル選択，非階層（処理 6 & 8）
			{ "1.2.840.10008.1.2.4.54", JPEG, }, // JPEG スペクトル選択，非階層（処理 7 & 9）
			{ "1.2.840.10008.1.2.4.55", JPEG, }, // JPEG 全数列，非階層（処理 10 & 12）
			{ "1.2.840.10008.1.2.4.56", JPEG, }, // JPEG 全数列,非階層（処理 11 & 13）
			{ "1.2.840.10008.1.2.4.57", JPEG, }, // JPEG 可逆,非階層（処理 14）
			{ "1.2.840.10008.1.2.4.58", JPEG, }, // JPEG 可逆,非階層（処理 15）
			{ "1.2.840.10008.1.2.4.59", JPEG, }, // JPEG 拡張,階層（処理 16 & 18）
			{ "1.2.840.10008.1.2.4.60", JPEG, }, // JPEG 拡張,階層（処理 17 & 19）
			{ "1.2.840.10008.1.2.4.61", JPEG, }, // JPEG スペクトル選択,階層（処理 20 & 22）
			{ "1.2.840.10008.1.2.4.62", JPEG, }, // JPEG スペクトル選択，階層（処理 21 & 23）
			{ "1.2.840.10008.1.2.4.63", JPEG, }, // JPEG 全数列，階層（処理 24 & 26）
			{ "1.2.840.10008.1.2.4.64", JPEG, }, // JPEG 全数列，階層（処理 25 & 27）
			{ "1.2.840.10008.1.2.4.65", JPEG, }, // JPEG 可逆，階層（処理 28）
			{ "1.2.840.10008.1.2.4.66", JPEG, }, // JPEG 可逆，階層（処理 29）
			{ "1.2.840.10008.1.2.4.70", JPEG, }, // JPEG 可逆，非階層，一次予測（処理 14 [選択値 1]）：可逆 JPEG 画像圧縮用デフォルト転送構文
			{ "1.2.840.10008.1.2.4.80", JPEG, }, // JPEG-LS 可逆画像圧縮
			{ "1.2.840.10008.1.2.4.81", JPEG, }, // JPEG-LS 非可逆（準可逆）画像圧縮
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
			list++;
		}

		return( RAW );
	}

	// コンテナからタグを検索し，double値を返す
	double find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, double default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_double( ) );
	}
	// コンテナからタグを検索し，float値を返す
	float find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, float default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_float( ) );
	}
	// コンテナからタグを検索し，signed int値を返す
	signed int find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, signed int default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_int( ) );
	}
	// コンテナからタグを検索し，unsigned int値を返す
	unsigned int find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, unsigned int default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_uint( ) );
	}
	// コンテナからタグを検索し，signed short値を返す
	signed short find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, signed short default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_short( ) );
	}
	// コンテナからタグを検索し，unsigned short値を返す
	unsigned short find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, unsigned short default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_ushort( ) );
	}
	// コンテナからタグを検索し，std::string値を返す
	std::string find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, const std::string &default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_string( ) );
	}
	// コンテナからタグを検索し，const char*値を返す
	std::string find_tag( const dicom_tag_container &dicom, unsigned short group, unsigned short element, const char *default_value )
	{
		dicom_tag_container::const_iterator cite = dicom.find( group, element );
		return( cite == dicom.end( ) ? default_value : cite->second.to_string( ) );
	}

	bool get_dicom_info( const dicom_tag_container &dicom, dicom_info &info )
	{
		info.compression_type		= get_compress_type( find_tag( dicom, 0x0002, 0x0010, "" ) );
		info.samples_per_pixel		= find_tag( dicom, 0x0028, 0x0002, info.samples_per_pixel );
		info.number_of_frames		= find_tag( dicom, 0x0028, 0x0008, info.number_of_frames );
		info.rows					= find_tag( dicom, 0x0028, 0x0010, info.rows );
		info.cols					= find_tag( dicom, 0x0028, 0x0011, info.cols );

		std::string pixel_spacing	= find_tag( dicom, 0x0028, 0x0030, "" );
		if( pixel_spacing != "" )
		{
			double resoX = 1.0, resoY = 1.0;
			sscanf( pixel_spacing.c_str( ), "%lf\\%lf", &resoX, &resoY );
			info.pixel_spacing_x	= resoX;
			info.pixel_spacing_y	= resoY;
		}

		info.bits_allocated			= find_tag( dicom, 0x0028, 0x0100, info.bits_allocated );
		info.bits_stored			= find_tag( dicom, 0x0028, 0x0101, info.bits_stored );
		info.high_bits				= find_tag( dicom, 0x0028, 0x0102, info.high_bits );
		info.pixel_representation	= find_tag( dicom, 0x0028, 0x0103, info.pixel_representation );
		info.window_center			= find_tag( dicom, 0x0028, 0x1050, info.window_center );
		info.window_width			= find_tag( dicom, 0x0028, 0x1051, info.window_width );

		return( true );
	}

	inline bool is_element_begin( const unsigned char *p, const unsigned char *e )
	{
		if( p + 4 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0x00 && p[ 3 ] == 0xe0 );
	}
	inline bool is_element_end( const unsigned char *p, const unsigned char *e )
	{
		if( p + 8 > e )
		{
			return( false );
		}
		return( p[ 0 ] == 0xfe && p[ 1 ] == 0xff && p[ 2 ] == 0xdd && p[ 3 ] == 0xe0 && p[ 4 ] == 0x00 && p[ 5 ] == 0x00 && p[ 6 ] == 0x00 && p[ 7 ] == 0x00 );
	}

	// RLE圧縮ファイルのデコーダ
	inline unsigned char *decode_RLE( unsigned char *psrc, unsigned char *psrc_end, unsigned char *pdst, unsigned char *pdst_end )
	{
		if( psrc + 64 >= psrc_end )
		{
			// RLE圧縮がかかっていません
			return( NULL );
		}

		// RLEのヘッダ情報を読み込む
		size_type number_of_segments = to_current_endian( byte_array< unsigned int >( psrc ), true ).get_value( );
		difference_type frame_offset[ 15 ] = {
			to_current_endian( byte_array< unsigned int >( psrc +  4 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc +  8 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 12 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 16 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 20 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 24 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 28 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 32 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 36 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 40 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 44 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 48 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 52 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 56 ), true ).get_value( ),
			to_current_endian( byte_array< unsigned int >( psrc + 60 ), true ).get_value( ),
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

	// RLE圧縮ファイルのデコーダ
	inline bool decode( dicom_element &element, const dicom_info &info )
	{
		switch( info.compression_type )
		{
		case RAW:
			return( true );

		case JPEG:
			// 今のところ未サポート
			return( false );

		default:
			break;
		}

		if( element.num_bytes < 8 + 8 )
		{
			// 圧縮がかかっていません
			return( false );
		}

		unsigned char *pointer = element.data;
		unsigned char *end_pointer = element.data + element.num_bytes;
		difference_type num_bytes = element.num_bytes;
		difference_type frame_offset[ 16 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		size_type number_of_fragments = 0;

		// ベーシックオフセットテーブルをスキップ
		if( !is_element_begin( pointer, end_pointer ) )
		{
			return( false );
		}
		pointer += 4;
		num_bytes = to_current_endian( byte_array< unsigned int >( pointer ), true ).get_value( );
		pointer += 4;
		if( num_bytes > 0 )
		{
			// 各フレームへのポインタが発見されたので調査
			unsigned char *p = pointer;
			while( p < pointer + num_bytes && number_of_fragments < 16 )
			{
				frame_offset[ number_of_fragments++ ] = to_current_endian( byte_array< unsigned int >( p ), true ).get_value( );
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

		size_type dstBytes = info.rows * info.cols * info.number_of_frames * info.bits_allocated / 8;
		unsigned char *buff = new unsigned char[ dstBytes + 1 ];
		unsigned char *dst_pointer = buff;
		unsigned char *p = pointer;
		bool ret = true;

		// ベーシックオフセットテーブルをスキップ
		size_type i = 0;
		while( dst_pointer < buff + dstBytes && i < number_of_fragments && ret )
		{
			p = pointer + frame_offset[ i ];
			if( !is_element_begin( p, end_pointer ) )
			{
				return( false );
			}
			p += 4;
			num_bytes = to_current_endian( byte_array< unsigned int >( p ), true ).get_value( );
			p += 4;

			switch( info.compression_type )
			{
			case RLE:
				dst_pointer = decode_RLE( p, p + num_bytes, dst_pointer, buff + dstBytes );
				if( dst_pointer == NULL )
				{
					ret = false;
				}
				break;

			default:
				break;
			}
			i++;
		}

		if( ret )
		{
			element.copy( buff, dstBytes );
		}

		delete [] buff;
		return( ret );
	}

}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DICOM_INFO__
