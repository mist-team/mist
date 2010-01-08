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

/// @file mist/crc.h
//!
//! @brief 任意のバイト列のCRCを計算する
//!
#ifndef __INCLUDE_CRC__
#define __INCLUDE_CRC__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN


namespace crc_generator_polynominal
{

	template < int BIT > struct crc_default
	{
		_MIST_CONST( size_t, gen_poly, 0 );
	};

	template < > struct crc_default< 8 >
	{
		_MIST_CONST( size_t, gen_poly, 0x07 );
	};

	template < > struct crc_default< 16 >
	{
		_MIST_CONST( size_t, gen_poly, 0x8005 );
	};

	template < > struct crc_default< 32 >
	{
		_MIST_CONST( size_t, gen_poly, 0x04c11db7 );
	};
}


/// @brief CRC を生成・検証を行うクラス
template < int BIT >
struct crc
{

	/// @brief pdata[0] から pdata[len - 1] の len bytes の CRC を生成する．
	//!
	//! デフォルトCRC-8 生成多項式 x^8 + x^2 + x^1 + 1
	//! デフォルトCRC-16生成多項式 x^16 + x^15 + x^2 + 1
	//! デフォルトCRC-32生成多項式 x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @return データから生成したCRC
	//!
	static size_t generate( const unsigned char * pdata, size_t len, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		size_t	crc = 0;
		size_t	mask;
		size_t	n, m;
		unsigned char	data;

		mask = 1 << ( BIT - 1 );

		for( n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}

				if( ( data & 0x80 ) != 0 )
				{
					crc ^= 0x01;
				}

				data <<= 1;
			}
		}

		for( n = 0 ; n < ( BIT >> 3 ) ; n++ )
		{
			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}
			}
		}

		return ( crc & ( ( mask << 1 ) - 1 ) );
	}


	/// @brief pdata[0] から pdata[len - 1] の len bytes の CRC を検証する．
	//!
	//! デフォルトCRC-8 生成多項式 x^8 + x^2 + x^1 + 1
	//! デフォルトCRC-16生成多項式 x^16 + x^15 + x^2 + 1
	//! デフォルトCRC-32生成多項式 x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @retval true  … CRCが一致した
	//! @retval false … CRCが一致しないため，データが改変された可能性あり
	//!
	static bool check( const unsigned char * pdata, size_t len, size_t crc_, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		return ( generate( pdata, len, gen_poly ) == crc_ );
	}


	/// @brief pdata[0] から pdata[len - BIT / 8 - 1] の len - BIT / 8 bytes の CRC を生成し，pdata[len - BIT / 8] 以降にビッグエンディアンで CRC を埋め込む．
	//!
	//! デフォルトCRC-8 生成多項式 x^8 + x^2 + x^1 + 1
	//! デフォルトCRC-16生成多項式 x^16 + x^15 + x^2 + 1
	//! デフォルトCRC-32生成多項式 x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @return データから生成したCRC
	//!
	static size_t generate_implant( unsigned char * pdata, size_t len, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		size_t	crc = 0;
		size_t	mask;
		size_t	n, m;
		unsigned char	data;

		mask = 1 << ( BIT - 1 );

		for( n = 0 ; n < len - BIT / 8 ; n++ )
		{
			data = pdata[ n ];

			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}

				if( ( data & 0x80 ) != 0 )
				{
					crc ^= 0x01;
				}

				data <<= 1;
			}
		}

		for( n = 0 ; n < ( BIT >> 3 ) ; n++ )
		{
			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & mask ) != 0 )
				{
					crc <<= 1;
					crc ^= gen_poly;
				}
				else
				{
					crc <<= 1;
				}
			}
		}

		// endian 変換．要修正．
		for( n = 0 ; n < ( BIT >> 3 ) ; n++ )
		{
			pdata[ len - ( BIT >> 3 ) + n ] = static_cast< unsigned char >( crc >> ( ( ( BIT >> 3 ) - 1 - n ) << 3 ) ) & 0xff;
		}

		return ( crc & ( ( mask << 1 ) - 1 ) );
	}


	/// @brief pdata[0] から pdata[len - 1] の len bytes （CRC 埋め込み済み）の CRC を検証する．
	//!
	//! デフォルトCRC-8 生成多項式 x^8 + x^2 + x^1 + 1
	//! デフォルトCRC-16生成多項式 x^16 + x^15 + x^2 + 1
	//! デフォルトCRC-32生成多項式 x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	//!
	//! @retval true  … CRCが一致した
	//! @retval false … CRCが一致しないため，データが改変された可能性あり
	//!
	static bool check_implant( const unsigned char * pdata, size_t len, size_t gen_poly = crc_generator_polynominal::crc_default< BIT >::gen_poly )
	{
		return ( generate( pdata, len, gen_poly ) == 0 );
	}
};


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_CRC__
