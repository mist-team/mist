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

/// @file mist/io/wav.h
//!
//! @brief WAV�����t�@�C����ǂݏ������邽�߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_WAV__
#define __INCLUDE_MIST_WAV__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �X�e���I�����̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_STEREO__
#include "../config/stereo.h"
#endif

// �e�^�̏���ǂݍ���
#ifndef __INCLUDE_MIST_LIMITS__
#include "../config/stereo.h"
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
#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( push, wav_align, 1 )
#endif
		struct _riffheader_
		{
			enum{ bytes = 8 };
			unsigned char	riff[4];			// WAV�̏ꍇ�͕K�� 'RIFF'
			unsigned  int	file_size;			// RIFF�w�b�_�̌�̃t�@�C���T�C�Y�@�܂�t�@�C���T�C�Y�|8
		} _MIST_PACKED;

		struct _wavheader_
		{
			enum{ bytes = 4 };
			unsigned char	wav[4];				// WAV�̏ꍇ�͕K�� 'WAVE'
		} _MIST_PACKED;

		struct _fmtheader_
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
		} _MIST_PACKED;

		struct _wavdata_
		{
			enum{ bytes = 8 };
			unsigned char	data[4];			// WAV�̏ꍇ�͕K�� 'data'
			unsigned  int	data_size;			// �{�w�b�_�̌�̃t�@�C���T�C�Y
		} _MIST_PACKED;

#if defined(__MIST_MSVC__) || defined(__INTEL_COMPILER)
	#pragma pack( pop, wav_align )
#endif
	// �\���̓��̃A���C�����g��1�o�C�g�ɐݒ肵�C�p�b�f�B���O���֎~���� �` �����܂� �`

	template < class T >
	struct sampling_bits_typedef
	{
		typedef T value_type;
	};

	#define __SAMPLING_BITS_CONVERTER__( from, to, expression ) \
		inline void sampling_bits_convert( const from &a, to &b ){ b = static_cast< to >( expression ); }\

	#define __SAMPLING_BITS_TYPEDEF__( from, to ) \
		template < >\
		struct sampling_bits_typedef< from >\
		{\
			typedef to value_type;\
		};\

	__SAMPLING_BITS_CONVERTER__( unsigned char, unsigned  char, a )							// 8�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,   signed  char, a - 128 )					// 8�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,           char, a - 128 )					// 8�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char, unsigned short, a * 257.0 )					// 8�r�b�g��16�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,   signed short, a * 257.0 - 32768.0 )		// 8�r�b�g��16�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char, unsigned   int, a * 65793.0 )				// 8�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,   signed   int, a * 65793.0 - 8388607.0 )	// 8�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char, unsigned  long, a * 65793.0 )				// 8�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,   signed  long, a * 65793.0 - 8388607.0 )	// 8�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,          float, a * 65793.0 )				// 8�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,         double, a * 65793.0 - 8388607.0 )	// 8�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( unsigned char,    long double, a * 65793.0 - 8388607.0 )	// 8�r�b�g��24�r�b�g�ϊ�

	__SAMPLING_BITS_CONVERTER__( signed short, unsigned  char, a / 257.0 + 128.0 )			// 16�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short,   signed  char, a / 257.0 )					// 16�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short,           char, a / 257.0 )					// 16�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short, unsigned short, a + 32768.0 )				// 16�r�b�g��16�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short,   signed short, a )							// 16�r�b�g��16�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short, unsigned   int, a * 256.0 + 8388607.0 )		// 16�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short,   signed   int, a * 256.0 )					// 16�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short,          float, a * 256.0 )					// 16�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short,         double, a * 256.0 )					// 16�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed short,    long double, a * 256.0 )					// 16�r�b�g��24�r�b�g�ϊ�

	__SAMPLING_BITS_CONVERTER__( signed int, unsigned  char, a / 65793.0 + 128.0 )			// 24�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int,   signed  char, a / 65793.0 )					// 24�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int,           char, a / 65793.0 )					// 24�r�b�g��8�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int, unsigned short, a / 256.0 + 32768.0 )			// 24�r�b�g��16�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int,   signed short, a / 256.0 )					// 24�r�b�g��16�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int, unsigned   int, a + 8388607.0 )				// 24�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int,   signed   int, a )							// 24�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int,          float, a )							// 24�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int,         double, a )							// 24�r�b�g��24�r�b�g�ϊ�
	__SAMPLING_BITS_CONVERTER__( signed int,    long double, a )							// 24�r�b�g��24�r�b�g�ϊ�


	__SAMPLING_BITS_TYPEDEF__( unsigned short, signed short );
	__SAMPLING_BITS_TYPEDEF__( unsigned   int, signed int );
	__SAMPLING_BITS_TYPEDEF__( unsigned  long, signed int );
	__SAMPLING_BITS_TYPEDEF__(   signed  char, unsigned char );
	__SAMPLING_BITS_TYPEDEF__(   signed  long, signed int );
	__SAMPLING_BITS_TYPEDEF__(           char, unsigned char );
	__SAMPLING_BITS_TYPEDEF__(          float, signed int );
	__SAMPLING_BITS_TYPEDEF__(         double, signed int );
	__SAMPLING_BITS_TYPEDEF__(    long double, signed int );

	#undef __SAMPLING_BITS_CONVERTER__
	#undef __SAMPLING_BITS_TYPEDEF__

	template < class T, class Allocator >
	struct wav_controller
	{
		typedef typename array< T, Allocator >::size_type size_type;
		typedef typename array< T, Allocator >::value_type value_type;
		typedef _stereo_converter_< T > stereo_converter;
		typedef typename stereo_converter::stereo_type stereo_type;

		
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

			case 24:
				return( _riffheader_::bytes + _wavheader_::bytes + _fmtheader_::bytes + _wavdata_::bytes + sound.size( ) * 3 * channel_num );
				break;

			case 32:
				return( _riffheader_::bytes + _wavheader_::bytes + _fmtheader_::bytes + _wavdata_::bytes + sound.size( ) * 4 * channel_num );
				break;

			default:
				return( 0 );
			}
		}

		static bool extract_wav_data8( unsigned char *wav, array< T, Allocator > &sound, size_type num_bytes, size_type channel_num )
		{
			size_type data_num = num_bytes / channel_num;
			sound.resize( data_num );

			unsigned char lin, rin;
			value_type lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = wav[ i ];
					sampling_bits_convert( lin, lout );
					sound[ i ] = stereo_converter::convert_to( lout, lout );
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = wav[ i * 2 + 0 ];
					rin = wav[ i * 2 + 1 ];
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					sound[ i ] = stereo_converter::convert_to( lout, rout );
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool extract_wav_data16( unsigned char *wav, array< T, Allocator > &sound, size_type num_bytes, size_type channel_num )
		{
			signed short *wave = reinterpret_cast< signed short * >( wav );
			size_type data_num = num_bytes / channel_num / 2;
			sound.resize( data_num );

			signed short lin, rin;
			value_type lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = wave[ i ];
					sampling_bits_convert( lin, lout );
					sound[ i ] = stereo_converter::convert_to( lout, lout );
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = wave[ i * 2 + 0 ];
					rin = wave[ i * 2 + 1 ];
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					sound[ i ] = stereo_converter::convert_to( lout, rout );
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool extract_wav_data24( unsigned char *wav, array< T, Allocator > &sound, size_type num_bytes, size_type channel_num )
		{
			size_type data_num = num_bytes / channel_num / 3;
			sound.resize( data_num );

			signed int lin, rin;
			value_type lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = ( wav[ i * 3 + 0 ] << 16 ) | ( wav[ i * 3 + 1 ] << 8 ) | wav[ i * 3 + 2 ];
					sampling_bits_convert( lin, lout );
					sound[ i ] = stereo_converter::convert_to( lout, lout );
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = ( wav[ i * 3 * 2 + 0 ] << 16 ) | ( wav[ i * 3 * 2 + 1 ] << 8 ) | wav[ i * 3 * 2 + 2 ];
					rin = ( wav[ i * 3 * 2 + 3 ] << 16 ) | ( wav[ i * 3 * 2 + 4 ] << 8 ) | wav[ i * 3 * 2 + 5 ];
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					sound[ i ] = stereo_converter::convert_to( lout, rout );
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool extract_wav_data32( unsigned char *wav, array< T, Allocator > &sound, size_type num_bytes, size_type channel_num )
		{
			signed int *wave = reinterpret_cast< signed int * >( wav );
			size_type data_num = num_bytes / channel_num / 4;
			sound.resize( data_num );

			signed short lin, rin;
			value_type lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = wave[ i ];
					sampling_bits_convert( lin, lout );
					sound[ i ] = stereo_converter::convert_to( lout, lout );
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					lin = wave[ i * 2 + 0 ];
					rin = wave[ i * 2 + 1 ];
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					sound[ i ] = stereo_converter::convert_to( lout, rout );
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool compose_wav_data8( const array< T, Allocator > &sound, unsigned char *wav, size_type channel_num )
		{
			typedef typename sampling_bits_typedef< value_type >::value_type compose_type;
			compose_type lin, rin;
			unsigned char lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					sampling_bits_convert( stereo_converter::convert_from( sound[ i ] ).l, lin );
					sampling_bits_convert( lin, lout );
					wav[ i ] = lout;
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					stereo_type s( sound[ i ] );
					sampling_bits_convert( s.l, lin );
					sampling_bits_convert( s.r, rin );
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					wav[ i * 2 + 0 ] = lout;
					wav[ i * 2 + 1 ] = rout;
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool compose_wav_data16( const array< T, Allocator > &sound, unsigned char *wav, size_type channel_num )
		{
			signed short *wave = reinterpret_cast< signed short * >( wav );
			typedef typename sampling_bits_typedef< value_type >::value_type compose_type;
			compose_type lin, rin;
			signed short lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					sampling_bits_convert( stereo_converter::convert_from( sound[ i ] ).l, lin );
					sampling_bits_convert( lin, lout );
					wave[ i ] = lout;
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					stereo_type s( sound[ i ] );
					sampling_bits_convert( s.l, lin );
					sampling_bits_convert( s.r, rin );
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					wave[ i * 2 + 0 ] = lout;
					wave[ i * 2 + 1 ] = rout;
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool compose_wav_data24( const array< T, Allocator > &sound, unsigned char *wav, size_type channel_num )
		{
			typedef typename sampling_bits_typedef< value_type >::value_type compose_type;
			compose_type lin, rin;
			signed int lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					sampling_bits_convert( stereo_converter::convert_from( sound[ i ] ).l, lin );
					sampling_bits_convert( lin, lout );
					wav[ i * 3 + 0 ] = static_cast< unsigned char >( ( lout >> 16 ) & 0x000000ff );
					wav[ i * 3 + 1 ] = static_cast< unsigned char >( ( lout >> 8 ) & 0x000000ff );
					wav[ i * 3 + 2 ] = static_cast< unsigned char >( lout & 0x000000ff );
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					stereo_type s( sound[ i ] );
					sampling_bits_convert( s.l, lin );
					sampling_bits_convert( s.r, rin );
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					wav[ i * 3 * 2 + 0 ] = static_cast< unsigned char >( ( lout >> 16 ) & 0x000000ff );
					wav[ i * 3 * 2 + 1 ] = static_cast< unsigned char >( ( lout >> 8 ) & 0x000000ff );
					wav[ i * 3 * 2 + 2 ] = static_cast< unsigned char >( lout & 0x000000ff );
					wav[ i * 3 * 2 + 3 ] = static_cast< unsigned char >( ( rout >> 16 ) & 0x000000ff );
					wav[ i * 3 * 2 + 4 ] = static_cast< unsigned char >( ( rout >> 8 ) & 0x000000ff );
					wav[ i * 3 * 2 + 5 ] = static_cast< unsigned char >( rout & 0x000000ff );
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool compose_wav_data32( const array< T, Allocator > &sound, unsigned char *wav, size_type channel_num )
		{
			signed int *wave = reinterpret_cast< signed int * >( wav );
			typedef typename sampling_bits_typedef< value_type >::value_type compose_type;
			compose_type lin, rin;
			signed int lout, rout;
			size_type i;

			switch( channel_num )
			{
			case 1:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					sampling_bits_convert( stereo_converter::convert_from( sound[ i ] ).l, lin );
					sampling_bits_convert( lin, lout );
					wave[ i ] = lout;
				}
				break;

			case 2:
				for( i = 0 ; i < sound.size( ) ; i++ )
				{
					stereo_type s( sound[ i ] );
					sampling_bits_convert( s.l, lin );
					sampling_bits_convert( s.r, rin );
					sampling_bits_convert( lin, lout );
					sampling_bits_convert( rin, rout );
					wave[ i * 2 + 0 ] = lout;
					wave[ i * 2 + 1 ] = rout;
				}
				break;

			default:
				return( false );
			}

			return( true );
		}

		static bool convert_from_wav_data( unsigned char *wav, array< T, Allocator > &sound, unsigned int &sampling_rate )
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
																					sound, wavdata.data_size, fmthead.channel_num );
				break;

			case 16:
				ret = extract_wav_data16( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes,
																					sound, wavdata.data_size, fmthead.channel_num );
				break;

			case 24:
				ret = extract_wav_data24( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes,
																					sound, wavdata.data_size, fmthead.channel_num );
				break;

			case 32:
				ret = extract_wav_data32( wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes,
																					sound, wavdata.data_size, fmthead.channel_num );
				break;

			default:
				// ���T�|�[�g�̗ʎq���r�b�g��
				std::cerr << "This sampling format is not supported!" << std::endl;
				break;
			}

			return( ret );
		}

		static bool convert_to_wav_data( const array< T, Allocator > &sound, unsigned char *wav, size_type wav_bits, size_type num_bytes, size_type sampling_rate )
		{
			size_t channel_num = stereo_converter::channel_num;

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
				ret = compose_wav_data8( sound, wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes, fmthead.channel_num );
				break;

			case 16:
				ret = compose_wav_data16( sound, wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes, fmthead.channel_num );
				break;

			case 24:
				ret = compose_wav_data24( sound, wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes, fmthead.channel_num );
				break;

			case 32:
				ret = compose_wav_data32( sound, wav + _riffheader_::bytes + _wavheader_::bytes + fmthead.fmt_size + 8 + _wavdata_::bytes, fmthead.channel_num );
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
				read_size = fread( pointer, sizeof( unsigned char ), 1024, fp );
				if( read_size < 1024 )
				{
					break;
				}
				pointer += read_size;
			}
			fclose( fp );

			bool ret = convert_from_wav_data( buff, sound, sampling_rate );
			delete [] buff;
			return( ret );
		}

		static bool write( const array< T, Allocator > &sound, const std::string &filename, size_type wav_bits, size_type sampling_rate )
		{
			size_type size = get_wav_bytes( sound, wav_bits, 1 );

			unsigned char *buff = new unsigned char[ size + 1 ];
			bool ret = convert_to_wav_data( sound, buff, wav_bits, size, sampling_rate );

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
			fclose( fp );

			delete [] buff;
			return( true );
		}
	};
}


//! @addtogroup audio_group �������o��
//!  @{

//! @addtogroup audio_wav_group WAV �������o��
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/io/wav.h>
//! @endcode
//!
//!  @{


/// @brief WAV�t�@�C�����特���f�[�^����MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] sound         �c �����f�[�^���o�͂���MIST�R���e�i
//! @param[in]  filename      �c WAV�t�@�C����
//! @param[out] sampling_rate �c WAV�t�@�C���ɋL�^����Ă���T���v�����O���[�g
//!
//! @retval true  �c WAV�t�@�C���̓ǂݍ��݂ɐ���
//! @retval false �c �t�@�C���̓ǂݍ��݂Ɏ��s�i�t�@�C�������݂��Ȃ��C�s�K�؂ȃt�@�C�����E�E�E�j
//! 
template < class T, class Allocator >
bool read_wav( array< T, Allocator > &sound, const std::string &filename, unsigned int &sampling_rate )
{
	return( __wav_controller__::wav_controller< T, Allocator >::read( sound, filename, sampling_rate ) );
}


/// @brief WAV�t�@�C�����特���f�[�^����MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] sound         �c �����f�[�^���o�͂���MIST�R���e�i
//! @param[in]  filename      �c WAV�t�@�C����
//! @param[out] sampling_rate �c WAV�t�@�C���ɋL�^����Ă���T���v�����O���[�g
//!
//! @retval true  �c WAV�t�@�C���̓ǂݍ��݂ɐ���
//! @retval false �c �t�@�C���̓ǂݍ��݂Ɏ��s�i�t�@�C�������݂��Ȃ��C�s�K�؂ȃt�@�C�����E�E�E�j
//! 
template < class T, class Allocator >
bool read_wav( array< T, Allocator > &sound, const std::wstring &filename, unsigned int &sampling_rate )
{
	return( read_wav( sound, wstr2str( filename ), sampling_rate ) );
}


/// @brief WAV�t�@�C�����特���f�[�^����MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] sound         �c �����f�[�^���o�͂���MIST�R���e�i
//! @param[in]  filename      �c WAV�t�@�C����
//!
//! @retval true  �c WAV�t�@�C���̓ǂݍ��݂ɐ���
//! @retval false �c �t�@�C���̓ǂݍ��݂Ɏ��s�i�t�@�C�������݂��Ȃ��C�s�K�؂ȃt�@�C�����E�E�E�j
//! 
template < class T, class Allocator >
bool read_wav( array< T, Allocator > &sound, const std::string &filename )
{
	unsigned int sampling_rate = 44100;
	return( read_wav( sound, filename, sampling_rate ) );
}


/// @brief WAV�t�@�C�����特���f�[�^����MIST�R���e�i�ɓǂݍ���
//! 
//! @param[out] sound         �c �����f�[�^���o�͂���MIST�R���e�i
//! @param[in]  filename      �c WAV�t�@�C����
//!
//! @retval true  �c WAV�t�@�C���̓ǂݍ��݂ɐ���
//! @retval false �c �t�@�C���̓ǂݍ��݂Ɏ��s�i�t�@�C�������݂��Ȃ��C�s�K�؂ȃt�@�C�����E�E�E�j
//! 
template < class T, class Allocator >
bool read_wav( array< T, Allocator > &sound, const std::wstring &filename )
{
	unsigned int sampling_rate = 44100;
	return( read_wav( sound, wstr2str( filename ), sampling_rate ) );
}


/// @brief MIST�R���e�i�̃f�[�^��WAV�t�@�C���֏o�͂���
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] sound         �c �����f�[�^�̓�����MIST�R���e�i
//! @param[in] filename      �c �o�̓t�@�C����
//! @param[in] wav_bits      �c �o�͂���WAV�t�@�C���̗ʎq���r�b�g��
//! @param[in] sampling_rate �c �o�͂���WAV�t�@�C���̃T���v�����O���[�g
//!
//! @retval true  �c WAV�t�@�C���̏o�͂ɐ���
//! @retval false �c �t�@�C���̏o�͂Ɏ��s
//! 
template < class T, class Allocator >
bool write_wav( const array< T, Allocator > &sound, const std::string &filename, typename array< T, Allocator >::size_type wav_bits,
						typename array< T, Allocator >::size_type sampling_rate )
{
	return( __wav_controller__::wav_controller< T, Allocator >::write( sound, filename, wav_bits, sampling_rate ) );
}


/// @brief MIST�R���e�i�̃f�[�^��WAV�t�@�C���֏o�͂���
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] sound         �c �����f�[�^�̓�����MIST�R���e�i
//! @param[in] filename      �c �o�̓t�@�C����
//! @param[in] wav_bits      �c �o�͂���WAV�t�@�C���̗ʎq���r�b�g��
//! @param[in] sampling_rate �c �o�͂���WAV�t�@�C���̃T���v�����O���[�g
//!
//! @retval true  �c WAV�t�@�C���̏o�͂ɐ���
//! @retval false �c �t�@�C���̏o�͂Ɏ��s
//! 
template < class T, class Allocator >
bool write_wav( const array< T, Allocator > &sound, const std::wstring &filename, typename array< T, Allocator >::size_type wav_bits,
						typename array< T, Allocator >::size_type sampling_rate )
{
	return( write_wav( sound, wstr2str( filename ), wav_bits, sampling_rate ) );
}


/// @}
//  WAV �������o�̓O���[�v�̏I���

/// @}
//  �������o�̓O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_WAV__
