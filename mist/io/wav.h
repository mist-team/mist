#ifndef __INCLUDE_MIST_WAV__
#define __INCLUDE_MIST_WAV__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif


#include <iostream>
#include <string>

#include <deque>
#include <map>
#include <algorithm>


// mist名前空間の始まり
_MIST_BEGIN


namespace __wav_controller__
{
	template < class T >
	union byte_array
	{
		typedef T value_type;
		value_type value;
		unsigned char byte[ sizeof( value_type ) ];
		byte_array( const value_type &v = 0 ) : value( v ){ }
	};

	// 構造体内のアライメントを1バイトに設定し，パッディングを禁止する
#if defined( __MIST_MSVC__ )
	#pragma pack( push, wav_align, 1 )
#endif
		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 8 };
			unsigned char	riff[4];			// WAVの場合は必ず 'RIFF'
			unsigned  int	file_size;			// RIFFヘッダの後のファイルサイズ　つまりファイルサイズ－8
		} _riffheader_;

		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 4 };
			unsigned char	wav[4];				// WAVの場合は必ず 'WAVE'
		} _wavheader_;

		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 24 };
			unsigned char	fmt[4];				// WAVの場合は必ず 'fmt ' ←スペース重要
			unsigned  int	fmt_size;			// fmtヘッダの後のファイルサイズ
			unsigned short  format_id;			// WAVファイルの形式
			unsigned short  channel_num;		// チャンネル数（1:モノラル，2:ステレオ）
			unsigned  int   sampling_rate;		// サンプリング周波数（11025、22050、44100）
			unsigned  int   bytes_per_second;	// １秒あたりの音声データのバイト数
			unsigned short  bytes_per_sample;	// １サンプルあたりのバイト数
			unsigned short  bits_per_sample;	// 量子化ビット数（8、16）
		} _fmtheader_;

		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 8 };
			unsigned char	data[4];			// WAVの場合は必ず 'data'
			unsigned  int	data_size;			// 本ヘッダの後のファイルサイズ
		} _wavdata_;

#if defined( __MIST_MSVC__ )
	#pragma pack( pop, wav_align )
#endif
	// 構造体内のアライメントを1バイトに設定し，パッディングを禁止する ～ ここまで ～

	template < class T, class Allocator >
	struct wav_controller
	{
		typedef typename array< T, Allocator >::size_type size_type;
		typedef typename array< T, Allocator >::value_type value_type;

		
		static size_type get_wav_bytes( const array< T, Allocator > &sound, size_type bits_per_sample, size_type channel_num )
		{
			switch( bits_per_sample )
			{
			case 8:
				return( _riffheader_::bytes + _wavheader_::bytes + _fmtheader_::bytes + _wavdata_::bytes + sound.size( ) * channel_num );
				break;

			case 16:
				return( _riffheader_::bytes + _wavheader_::bytes + _fmtheader_::bytes + _wavdata_::bytes + sound.size( ) * 2 * channel_num );
				break;

			default:
				return( 0 );
			}
		}

		static bool extract_wav_data8( unsigned char *wav, array< T, Allocator > &sound, size_type num_bytes, size_type channel, size_type channel_num )
		{
			size_type data_num = num_bytes / channel_num;
			sound.resize( data_num );
			for( size_type i = 0 ; i < sound.size( ) ; i++ )
			{
				sound[ i ] = static_cast< value_type >( wav[ i * channel_num + channel ] );
			}
			return( true );
		}

		static bool extract_wav_data16( unsigned char *wav, array< T, Allocator > &sound, size_type num_bytes, size_type channel, size_type channel_num )
		{
			signed short *wave = reinterpret_cast< signed short * >( wav );
			size_type data_num = num_bytes / channel_num / 2;
			sound.resize( data_num );
			for( size_type i = 0 ; i < sound.size( ) ; i++ )
			{
				sound[ i ] = static_cast< value_type >( wave[ i * channel_num + channel ] );
			}
			return( true );
		}

		static bool compose_wav_data8( const array< T, Allocator > &sound, unsigned char *wav, size_type channel, size_type channel_num )
		{
			for( size_type i = 0 ; i < sound.size( ) ; i++ )
			{
				wav[ i * channel_num + channel ] = static_cast< unsigned char >( sound[ i ] );
			}
			return( true );
		}

		static bool compose_wav_data16( const array< T, Allocator > &sound, unsigned char *wav, size_type channel, size_type channel_num )
		{
			signed short *wave = reinterpret_cast< signed short * >( wav );
			for( size_type i = 0 ; i < sound.size( ) ; i++ )
			{
				wave[ i * channel_num + channel ] = static_cast< signed short >( sound[ i ] );
			}
			return( true );
		}

		static bool convert_from_wav_data( unsigned char *wav, array< T, Allocator > &sound, size_type channel, unsigned int &sampling_rate )
		{
			// WAV用のヘッダの位置を指定する
			_riffheader_	*priffhead	= reinterpret_cast < _riffheader_ * >( wav );
			_wavheader_		*pwavhead	= reinterpret_cast < _wavheader_ * > ( wav + _riffheader_::bytes );
			_fmtheader_		*pfmthead	= reinterpret_cast < _fmtheader_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes );

			_riffheader_	&riffhead	= *priffhead;
			_wavheader_		&wavhead	= *pwavhead;
			_fmtheader_		&fmthead	= *pfmthead;

			if( !( riffhead.riff[0] == 'R' && riffhead.riff[1] == 'I' && riffhead.riff[2] == 'F' && riffhead.riff[3] == 'F' ) )
			{
				// WAVではありません
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( !( wavhead.wav[0] == 'W' && wavhead.wav[1] == 'A' && wavhead.wav[2] == 'V' && wavhead.wav[3] == 'E' ) )
			{
				// WAVではありません
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( !( fmthead.fmt[0] == 'f' && fmthead.fmt[1] == 'm' && fmthead.fmt[2] == 't' && fmthead.fmt[3] == ' ' ) )
			{
				// WAVではありません
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( fmthead.channel_num < channel && channel <= 0 )
			{
				// 指定されたチャンネルは存在しません
				std::cerr << "The specified channel does not exist!" << std::endl;
				return( false );
			}

			_wavdata_		*pwavdata	= reinterpret_cast < _wavdata_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 );
			_wavdata_		&wavdata	= *pwavdata;

			sampling_rate = fmthead.sampling_rate;

			if( fmthead.format_id != 0x0001 )
			{
				// リニア PCM のみサポート
				std::cerr << "This is not a Linear PCM format!" << std::endl;
				return( false );
			}

			bool ret = false;
			switch( fmthead.bits_per_sample )
			{
			case 8:
				ret = extract_wav_data8( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes,
																				sound, wavdata.data_size, channel, fmthead.channel_num );
				break;

			case 16:
				ret = extract_wav_data16( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes,
																				sound, wavdata.data_size, channel, fmthead.channel_num );
				break;

			default:
				// 未サポートの量子化ビット数
				std::cerr << "This sampling format is not supported!" << std::endl;
				break;
			}

			return( ret );
		}

		static bool convert_to_wav_data( const array< T, Allocator > &sound, unsigned char *wav, size_type wav_bits, size_type num_bytes,
																			size_type channel, size_type channel_num, size_type sampling_rate )
		{
			if( channel_num <= channel )
			{
				// 指定されたチャンネルは存在しません
				std::cerr << "The specified channel does not exist!" << std::endl;
				return( false );
			}

			// WAV用のヘッダの位置を指定する
			_riffheader_	*priffhead	= reinterpret_cast < _riffheader_ * >( wav );
			_wavheader_		*pwavhead	= reinterpret_cast < _wavheader_ * > ( wav + _riffheader_::bytes );
			_fmtheader_		*pfmthead	= reinterpret_cast < _fmtheader_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes );
			_wavdata_		*pwavdata	= reinterpret_cast < _wavdata_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes + _fmtheader_::bytes );

			_riffheader_	&riffhead	= *priffhead;
			_wavheader_		&wavhead	= *pwavhead;
			_fmtheader_		&fmthead	= *pfmthead;
			_wavdata_		&wavdata	= *pwavdata;

			riffhead.riff[0] = 'R';
			riffhead.riff[1] = 'I';
			riffhead.riff[2] = 'F';
			riffhead.riff[3] = 'F';
			riffhead.file_size = static_cast< unsigned  int >( num_bytes - 8 );

			wavhead.wav[0] = 'W';
			wavhead.wav[1] = 'A';
			wavhead.wav[2] = 'V';
			wavhead.wav[3] = 'E';

			fmthead.fmt[0]				= 'f';
			fmthead.fmt[1]				= 'm';
			fmthead.fmt[2]				= 't';
			fmthead.fmt[3]				= ' ';
			fmthead.fmt_size			= 16;
			fmthead.format_id			= 0x0001;
			fmthead.channel_num			= static_cast< unsigned short >( channel_num );
			fmthead.sampling_rate		= static_cast< unsigned int >( sampling_rate );
			fmthead.bytes_per_sample	= wav_bits == 8 ? 1 : 2;
			fmthead.bytes_per_second	= static_cast< unsigned int >( sampling_rate * fmthead.bytes_per_sample );
			fmthead.bits_per_sample		= static_cast< unsigned short >( wav_bits );

			wavdata.data[0] = 'd';
			wavdata.data[1] = 'a';
			wavdata.data[2] = 't';
			wavdata.data[3] = 'a';
			wavdata.data_size = static_cast< unsigned int >( sound.size( ) * channel_num * fmthead.bytes_per_sample );

			bool ret = false;
			switch( fmthead.bits_per_sample )
			{
			case 8:
				ret = compose_wav_data8( sound, wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes, channel, fmthead.channel_num );
				break;

			case 16:
				ret = compose_wav_data16( sound, wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes, channel, fmthead.channel_num );
				break;

			default:
				// 未サポートの量子化ビット数
				std::cerr << "This sampling format is not supported!" << std::endl;
				break;
			}

			return( true );
		}

		static bool read( array< T, Allocator > &sound, const std::string &filename, unsigned int &sampling_rate )
		{
			size_type filesize;
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );
			// ファイルサイズを取得
			fseek( fp, 0, SEEK_END );
			filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			unsigned char *buff = new unsigned char[ filesize + 1 ];
			unsigned char *pointer = buff;
			size_type read_size = 0;
			while( feof( fp ) == 0 )
			{
                read_size = fread( pointer, sizeof( unsigned char ), 4096, fp );
				if( read_size < 4096 )
				{
					break;
				}
				pointer += read_size;
			}
			fclose( fp );

			bool ret = convert_from_wav_data( buff, sound, 0, sampling_rate );
			delete [] buff;
			return( ret );
		}

		static bool read( array< T, Allocator > &left, array< T, Allocator > &right, const std::string &filename, unsigned int &sampling_rate )
		{
			size_type filesize;
			FILE *fp;
			if( ( fp = fopen( filename.c_str( ), "rb" ) ) == NULL ) return( false );
			// ファイルサイズを取得
			fseek( fp, 0, SEEK_END );
			filesize = ftell( fp );
			fseek( fp, 0, SEEK_SET );

			unsigned char *buff = new unsigned char[ filesize + 1 ];
			unsigned char *pointer = buff;
			size_type read_size = 0;
			while( feof( fp ) == 0 )
			{
                read_size = fread( pointer, sizeof( unsigned char ), 4096, fp );
				if( read_size < 4096 )
				{
					break;
				}
				pointer += read_size;
			}
			fclose( fp );

			bool ret = false;
			ret = convert_from_wav_data( buff, left,  0, sampling_rate );
			ret = ret && convert_from_wav_data( buff, right, 1, sampling_rate );
			delete [] buff;
			return( ret );
		}

		static bool write( const array< T, Allocator > &sound, const std::string &filename, size_type wav_bits, size_type sampling_rate )
		{
			size_type size = get_wav_bytes( sound, wav_bits, 1 );
			unsigned char *buff = new unsigned char[ size + 1 ];
			bool ret = convert_to_wav_data( sound, buff, wav_bits, size, 0, 1, sampling_rate );

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

			// ファイルへ書き出し
			unsigned char *pointer = buff;
			size_type write_size = 0;
			while( size > 0 )
			{
                write_size = fwrite( pointer, sizeof( unsigned char ), 4096, fp );
				pointer += write_size;
				size -= write_size;
				if( write_size != 4096 )
				{
					fclose( fp );
					delete [] buff;
					return( false );
				}
			}
			fclose( fp );

			delete [] buff;
			return( true );
		}

		static bool write( const array< T, Allocator > &left, const array< T, Allocator > &right, const std::string &filename, size_type wav_bits, size_type sampling_rate )
		{
			size_type size = 0;
			if( left.size( ) > right.size( ) )
			{
				size = get_wav_bytes( left, wav_bits, 2 );
			}
			else
			{
				size = get_wav_bytes( right, wav_bits, 2 );
			}

			unsigned char *buff = new unsigned char[ size + 1 ];
			bool ret = false;

			// データを 0 で初期化する
			memset( buff, 0, sizeof( unsigned char ) * ( size + 1 ) );

			ret = convert_to_wav_data( left, buff, wav_bits, size, 0, 2, sampling_rate );
			ret = ret && convert_to_wav_data( right, buff, wav_bits, size, 1, 2, sampling_rate );

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

			// ファイルへ書き出し
			unsigned char *pointer = buff;
			size_type write_size = 0;
			while( size > 0 )
			{
                write_size = fwrite( pointer, sizeof( unsigned char ), 4096, fp );
				pointer += write_size;
				size -= write_size;
				if( write_size != 4096 )
				{
					fclose( fp );
					delete [] buff;
					return( false );
				}
			}

			fclose( fp );

			delete [] buff;
			return( true );
		}
	};
}


template < class T, class Allocator >
bool read_wav( array< T, Allocator > &sound, const std::string &filename, unsigned int &sampling_rate )
{
	return( __wav_controller__::wav_controller< T, Allocator >::read( sound, filename, sampling_rate ) );
}

template < class T, class Allocator >
bool read_wav( array< T, Allocator > &sound, const std::string &filename )
{
	unsigned int sampling_rate = 44100;
	return( read_wav( sound, filename, sampling_rate ) );
}

template < class T, class Allocator >
bool write_wav( const array< T, Allocator > &sound, const std::string &filename, typename array< T, Allocator >::size_type wav_bits,
						typename array< T, Allocator >::size_type sampling_rate )
{
	return( __wav_controller__::wav_controller< T, Allocator >::write( sound, filename, wav_bits, sampling_rate ) );
}



template < class T, class Allocator >
bool read_wav( array< T, Allocator > &left, array< T, Allocator > &right, const std::string &filename, unsigned int &sampling_rate )
{
	return( __wav_controller__::wav_controller< T, Allocator >::read( left, right, filename, sampling_rate ) );
}

template < class T, class Allocator >
bool read_wav( array< T, Allocator > &left, array< T, Allocator > &right, const std::string &filename )
{
	unsigned int sampling_rate = 44100;
	return( read_wav( left, right, filename, sampling_rate ) );
}

template < class T, class Allocator >
bool write_wav( const array< T, Allocator > &left, const array< T, Allocator > &right, const std::string &filename, typename array< T, Allocator >::size_type wav_bits,
						typename array< T, Allocator >::size_type sampling_rate )
{
	return( __wav_controller__::wav_controller< T, Allocator >::write( left, right, filename, wav_bits, sampling_rate ) );
}

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_WAV__
