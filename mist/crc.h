#ifndef __INCLUDE_CRC__
#define __INCLUDE_CRC__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


template < int BIT >
struct crc
{
	/// @brief pdata[0] ���� pdata[len - 1] �� len bytes�� CRC-8 �� ��������D
	//!
	//! ���������� x^8 + x^2 + x^1 + 1
	//!
	size_t generate( const unsigned char * pdata, size_t len, unsigned char gen_poly = 0x07 )
	{
		size_t			crc = 0, n, m;
		unsigned char	data;

		// �f�[�^��+0��̃��[�v

		for( n = 0 ; n < len + 1 ; n++ )
		{
			data = pdata[ n ];

			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80 ) != 0 )
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

		for( ; n < len + BIT / 8 ; n++ )
		{
			data = 0;

			for( m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80 ) != 0 )
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

		return( crc );
	}


	bool check( unsigned char * pdata, size_t len, size_t crc_, unsigned char gen_poly = 0x07 )
	{
		return( generate( pdata, len, gen_poly ) == crc_ );
	}
};

namespace __crc__
{

	/*
	pdata[0] ���� pdata[len - 2] �� len - 1 bytes�� CRC-8 �� pdata[len - 1] �ɐ�������D

	���������� x^8 + x^2 + x^1 + 1
	*/
	unsigned char generate_crc_8( unsigned char * pdata, long len, unsigned char gen_poly = 0x07 )
	{
		unsigned char	crc = 0;
		unsigned char	data;

		// CRC�̃N���A
		*( pdata + len - 1 ) = 0;

		for( long n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80 ) != 0 )
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

		// CRC�̃Z�b�g
		*( pdata + len - 1 ) = crc;

		return crc;
	}

	bool check_crc_8( unsigned char * pdata, long len, unsigned char gen_poly = 0x07 )
	{
		unsigned char	crc = 0;
		unsigned char	data;

		for( long n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80 ) != 0 )
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

		return crc == 0;
	}

	/*
	pdata[0] ���� pdata[len - 1] �� len bytes�� CRC-8 �� ��������D

	���������� x^8 + x^2 + x^1 + 1
	*/
	unsigned char generate_crc_8ex( unsigned char * pdata, long len, unsigned char gen_poly = 0x07 )
	{
		unsigned char	crc = 0;
		unsigned char	data;

		// �f�[�^��+0��̃��[�v

		for( long n = 0 ; n < len + 1 ; n++ )
		{
			if( n < len )
			{
				data = pdata[ n ];
			}
			else
			{
				data = 0;
			}

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80 ) != 0 )
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

		return crc;
	}

	bool check_crc_8ex( unsigned char * pdata, long len, unsigned char _crc, unsigned char gen_poly = 0x07 )
	{
		unsigned char	crc = 0;
		unsigned char	data;

		for( long n = 0 ; n < len + 1 ; n++ )
		{
			if( n < len )
			{
				data = pdata[ n ];
			}
			else
			{
				data = 0;
			}

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80 ) != 0 )
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

		return crc == _crc;
	}

	/*
	pdata[0] ���� pdata[len - 3] �� len - 2 bytes�� CRC-16 �� pdata[len - 2] �� pdata[len - 1] �ɐ�������D

	���������� x^16 + x^15 + x^2 + 1
	*/
	unsigned short generate_crc_16( unsigned char * pdata, long len, unsigned short gen_poly = 0x8005 )
	{
		unsigned short	crc = 0;
		unsigned char	data;

		// CRC�̃N���A
		*( pdata + len - 2 ) = 0;
		*( pdata + len - 1 ) = 0;

		for( long n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x8000 ) != 0 )
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
					crc ^= 0x0001;
				}

				data <<= 1;
			}
		}

		// CRC�̃Z�b�g
		*( pdata + len - 1 ) = crc & 0xff;

		*( pdata + len - 2 ) = ( crc >> 8 ) & 0xff;

		return crc;
	}

	bool check_crc_16( unsigned char * pdata, long len, unsigned short gen_poly = 0x8005 )
	{
		unsigned short	crc = 0;
		unsigned char	data;

		for( long n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x8000 ) != 0 )
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
					crc ^= 0x0001;
				}

				data <<= 1;
			}
		}

		return crc == 0;
	}

	/*
	pdata[0] ���� pdata[len - 3] �� len - 2 bytes�� CRC-16 �� pdata[len - 2] �� pdata[len - 1] �ɐ�������D

	���������� x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x^1 + 1
	*/
	unsigned generate_crc_32( unsigned char * pdata, long len, unsigned gen_poly = 0x04C11DB7 )
	{
		unsigned	crc = 0;
		unsigned char	data;

		// CRC�̃N���A
		*( pdata + len - 4 ) = 0;
		*( pdata + len - 3 ) = 0;
		*( pdata + len - 2 ) = 0;
		*( pdata + len - 1 ) = 0;

		for( long n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80000000 ) != 0 )
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
					crc ^= 0x0001;
				}

				data <<= 1;
			}
		}

		// CRC�̃Z�b�g
		*( pdata + len - 1 ) = crc & 0xff;

		*( pdata + len - 2 ) = ( crc >> 8 ) & 0xff;

		*( pdata + len - 3 ) = ( crc >> 16 ) & 0xff;

		*( pdata + len - 4 ) = ( crc >> 24 ) & 0xff;

		return crc;
	}

	bool check_crc_32( unsigned char * pdata, long len, unsigned gen_poly = 0x04C11DB7 )
	{
		unsigned	crc = 0;
		unsigned char	data;

		for( long n = 0 ; n < len ; n++ )
		{
			data = pdata[ n ];

			for( int m = 0 ; m < 8 ; m++ )
			{
				if( ( crc & 0x80000000 ) != 0 )
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
					crc ^= 0x0001;
				}

				data <<= 1;
			}
		}

		return crc == 0;
	}

}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_CRC__
