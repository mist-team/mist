#ifndef __INCLUDE_CRC__
#define __INCLUDE_CRC__

//#include "mist/mist.h"

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __crc__
{

	/*
	pdata[0] ���� pdata[len - 2] �� len - 1 bytes�� CRC-8 �� pdata[len - 1] �ɐ�������D

	���������� x^8 + x^2 + x^1 + 1
	100000111 = 107H
	*/
	void generate_crc_8(unsigned char* pdata, long len)
	{
		unsigned char	crc = 0;
		unsigned char	data = 0;

		// CRC�̃N���A
		*(pdata + len - 1) = 0;

		for(long n = 0; n < len; n ++)
		{
			data = pdata[n];

			for(int m = 0; m < 8; m ++)
			{
				if((crc & 0x80) != 0)
				{
					crc <<= 1;
					crc ^= 0x07;
				}
				else
				{
					crc <<= 1;
				}

				if((data & 0x80) != 0)
				{
					crc ^= 0x01;
				}

				data <<= 1;
			}
		}

		// CRC�̃Z�b�g
		*(pdata + len - 1) = crc;
	}

	bool check_crc_8(unsigned char* pdata, long len)
	{
		unsigned char	crc = 0;
		unsigned char	data = 0;

		for(long n = 0; n < len; n ++)
		{
			data = pdata[n];

			for(int m = 0; m < 8; m ++)
			{
				if((crc & 0x80) != 0)
				{
					crc <<= 1;
					crc ^= 0x07;
				}
				else
				{
					crc <<= 1;
				}

				if((data & 0x80) != 0){
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
	100000111 = 107H
	*/
	unsigned char generate_crc_8ex(unsigned char* pdata, long len)
	{
		unsigned char	crc = 0;
		unsigned char	data = 0;

		// �f�[�^��+0��̃��[�v
		for(long n = 0; n < len + 1; n ++)
		{
			if(n < len)
			{
				data = pdata[n];
			}
			else
			{
				data = 0;
			}

			for(int m = 0; m < 8; m ++)
			{
				if((crc & 0x80) != 0)
				{
					crc <<= 1;
					crc ^= 0x07;
				}
				else
				{
					crc <<= 1;
				}

				if((data & 0x80) != 0)
				{
					crc ^= 0x01;
				}

				data <<= 1;
			}
		}

		return crc;
	}

	bool check_crc_8ex(unsigned char* pdata, long len, unsigned char _crc)
	{
		unsigned char	crc = 0;
		unsigned char	data = 0;

		for(long n = 0; n < len + 1; n ++)
		{
			if(n < len)
			{
				data = pdata[n];
			}
			else
			{
				data = 0;
			}

			for(int m = 0; m < 8; m ++)
			{
				if((crc & 0x80) != 0)
				{
					crc <<= 1;
					crc ^= 0x07;
				}
				else
				{
					crc <<= 1;
				}

				if((data & 0x80) != 0){
					crc ^= 0x01;
				}

				data <<= 1;
			}
		}

		return crc == _crc;
	}

}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_CRC__
