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
			unsigned short  channel;			// チャンネル数（1:モノラル，2:ステレオ）
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

	template < class T, class Allocator >検討
	struct wav_controller
	{
		typedef typename array< T, Allocator >::size_type size_type;
		typedef typename array< T, Allocator >::value_type value_type;

		static bool is_supported( size_type wav_bits )
		{
			bool ret = false;
			switch( wav_bits )
			{
			case 1:
			case 4:
			case 8:
//			case 16:
			case 24:
			case 32:
				ret = true;
				break;

			default:
				break;
			}
			return( ret );
		}

		static bool convert_from_wav_data( unsigned char *wav, array< T, Allocator > &sound )
		{
			// ビットマップ用のヘッダの位置を指定する
			_riffheader_	*priffhead	= reinterpret_cast < _riffheader_ * >( wav );
			_wavheader_		*pwavhead	= reinterpret_cast < _wavheader_ * > ( wav + _riffheader_::bytes );
			_fmtheader_		*pfmthead	= reinterpret_cast < _fmtheader_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes );

			_riffheader_	&riffhead	= *priffhead;
			_wavheader_		&wavhead	= *pwavhead;
			_fmtheader_		&fmthead	= *pfmthead;

			if( !( riffhead.riff[0] == 'R' && riffhead.riff[1] == 'I' && riffhead.riff[2] == 'F' && riffhead.riff[3] == 'F' ) )
			{
				// ビットマップではありません
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( !( wavhead.wav[0] == 'W' && wavhead.wav[1] == 'A' && wavhead.wav[2] == 'V' && wavhead.wav[3] == 'E' ) )
			{
				// ビットマップではありません
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( !( fmthead.fmt[0] == 'f' && fmthead.fmt[1] == 'm' && fmthead.fmt[2] == 't' && fmthead.fmt[3] == ' ' ) )
			{
				// ビットマップではありません
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			_wavdata_		*pwavdata	= reinterpret_cast < _wavdata_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 );
			_wavdata_		&wavdata	= *pwavdata;

			return( true );
		}

		static bool convert_to_wav_data( const array< T, Allocator > &sound, unsigned char *wav, size_type wav_bits )
		{
			return( true );
		}
		static bool read( array< T, Allocator > &sound, const std::string &filename )
		{
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

			bool ret = convert_from_wav_data( buff, sound );
			delete [] buff;
			return( ret );
		}

		static bool write( const array< T, Allocator > &sound, const std::string &filename, size_type wav_bits )
		{
			if( !is_supported( wav_bits ) )
			{
				std::cerr << "This format is not supported currently!" << std::endl;
				return( false );
			}

			size_type size = get_wav_bytes( sound, wav_bits );
			unsigned char *buff = new unsigned char[ size + 1 ];
			bool ret = convert_to_wav_data( sound, buff, wav_bits );

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
bool read_wav( array< T, Allocator > &sound, const std::string &filename )
{
	return( __wav_controller__::wav_controller< T, Allocator >::read( sound, filename ) );
}

template < class T, class Allocator >
bool write_wav( array< T, Allocator > &sound, const std::string &filename, typename array2< T, Allocator >::size_type wav_bits = 24 )
{
	return( __wav_controller__::wav_controller< T, Allocator >::write( sound, filename, wav_bits ) );
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_WAV__
