#ifndef __INCLUDE_MIST_WAV__
#define __INCLUDE_MIST_WAV__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif


#include <iostream>
#include <string>

#include <deque>
#include <map>
#include <algorithm>


// mist���O��Ԃ̎n�܂�
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

	// �\���̓��̃A���C�����g��1�o�C�g�ɐݒ肵�C�p�b�f�B���O���֎~����
#if defined( __MIST_MSVC__ )
	#pragma pack( push, wav_align, 1 )
#endif
		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 8 };
			unsigned char	riff[4];			// WAV�̏ꍇ�͕K�� 'RIFF'
			unsigned  int	file_size;			// RIFF�w�b�_�̌�̃t�@�C���T�C�Y�@�܂�t�@�C���T�C�Y�|8
		} _riffheader_;

		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 4 };
			unsigned char	wav[4];				// WAV�̏ꍇ�͕K�� 'WAVE'
		} _wavheader_;

		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 24 };
			unsigned char	fmt[4];				// WAV�̏ꍇ�͕K�� 'fmt ' ���X�y�[�X�d�v
			unsigned  int	fmt_size;			// fmt�w�b�_�̌�̃t�@�C���T�C�Y
			unsigned short  format_id;			// WAV�t�@�C���̌`��
			unsigned short  channel_num;		// �`�����l�����i1:���m�����C2:�X�e���I�j
			unsigned  int   sampling_rate;		// �T���v�����O���g���i11025�A22050�A44100�j
			unsigned  int   bytes_per_second;	// �P�b������̉����f�[�^�̃o�C�g��
			unsigned short  bytes_per_sample;	// �P�T���v��������̃o�C�g��
			unsigned short  bits_per_sample;	// �ʎq���r�b�g���i8�A16�j
		} _fmtheader_;

		typedef _MIST_ALIGN( struct, 1 )
		{
			enum{ bytes = 8 };
			unsigned char	data[4];			// WAV�̏ꍇ�͕K�� 'data'
			unsigned  int	data_size;			// �{�w�b�_�̌�̃t�@�C���T�C�Y
		} _wavdata_;

#if defined( __MIST_MSVC__ )
	#pragma pack( pop, wav_align )
#endif
	// �\���̓��̃A���C�����g��1�o�C�g�ɐݒ肵�C�p�b�f�B���O���֎~���� �` �����܂� �`

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
			// WAV�p�̃w�b�_�̈ʒu���w�肷��
			_riffheader_	*priffhead	= reinterpret_cast < _riffheader_ * >( wav );
			_wavheader_		*pwavhead	= reinterpret_cast < _wavheader_ * > ( wav + _riffheader_::bytes );
			_fmtheader_		*pfmthead	= reinterpret_cast < _fmtheader_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes );

			_riffheader_	&riffhead	= *priffhead;
			_wavheader_		&wavhead	= *pwavhead;
			_fmtheader_		&fmthead	= *pfmthead;

			if( !( riffhead.riff[0] == 'R' && riffhead.riff[1] == 'I' && riffhead.riff[2] == 'F' && riffhead.riff[3] == 'F' ) )
			{
				// WAV�ł͂���܂���
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( !( wavhead.wav[0] == 'W' && wavhead.wav[1] == 'A' && wavhead.wav[2] == 'V' && wavhead.wav[3] == 'E' ) )
			{
				// WAV�ł͂���܂���
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( !( fmthead.fmt[0] == 'f' && fmthead.fmt[1] == 'm' && fmthead.fmt[2] == 't' && fmthead.fmt[3] == ' ' ) )
			{
				// WAV�ł͂���܂���
				std::cerr << "This is not a wav format!" << std::endl;
				return( false );
			}

			if( fmthead.channel_num < channel && channel <= 0 )
			{
				// �w�肳�ꂽ�`�����l���͑��݂��܂���
				std::cerr << "The specified channel does not exist!" << std::endl;
				return( false );
			}

			_wavdata_		*pwavdata	= reinterpret_cast < _wavdata_ * > ( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 );
			_wavdata_		&wavdata	= *pwavdata;

			sampling_rate = fmthead.sampling_rate;

			if( fmthead.format_id != 0x0001 )
			{
				// ���j�A PCM �̂݃T�|�[�g
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
				// ���T�|�[�g�̗ʎq���r�b�g��
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
				// �w�肳�ꂽ�`�����l���͑��݂��܂���
				std::cerr << "The specified channel does not exist!" << std::endl;
				return( false );
			}

			// WAV�p�̃w�b�_�̈ʒu���w�肷��
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
				// ���T�|�[�g�̗ʎq���r�b�g��
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
			// �t�@�C���T�C�Y���擾
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
			// �t�@�C���T�C�Y���擾
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

			// �t�@�C���֏����o��
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

			// �f�[�^�� 0 �ŏ���������
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

			// �t�@�C���֏����o��
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

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_WAV__
