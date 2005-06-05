/// @file mist/md5.h
//!
//! @brief �C�ӂ̃o�C�g���MD5���v�Z����
//!
#ifndef __INCLUDE_MD5__
#define __INCLUDE_MD5__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_ENDIAN__
#include "config/endian.h"
#endif

#include <string>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @defgroup hash_group �n�b�V���֐�
//!  @{


/// @brief MD5 �𐶐�����N���X
class md5
{
public:
	typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	unsigned char digest[ 16 ];

protected:
	unsigned int F( unsigned int a, unsigned int b, unsigned int c ){ return( ( a & b ) | ( ~a & c ) ); }
	unsigned int G( unsigned int a, unsigned int b, unsigned int c ){ return( ( a & c ) | ( ~c & b ) ); }
	unsigned int H( unsigned int a, unsigned int b, unsigned int c ){ return( a ^ b ^ c ); }
	unsigned int I( unsigned int a, unsigned int b, unsigned int c ){ return( b ^ ( a | ~c ) ); }
	unsigned int R( unsigned int a, unsigned int s ){ return( ( a << s ) | ( a >> ( 32 - s ) ) ); }

	void FF( unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int xk, unsigned int s, unsigned int ti )
	{
		a = b + R( a + F( b, c, d ) + xk + ti, s );
	}

	void GG( unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int xk, unsigned int s, unsigned int ti )
	{
		a = b + R( a + G( b, c, d ) + xk + ti, s );
	}

	void HH( unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int xk, unsigned int s, unsigned int ti )
	{
		a = b + R( a + H( b, c, d ) + xk + ti, s );
	}

	void II( unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int xk, unsigned int s, unsigned int ti )
	{
		a = b + R( a + I( b, c, d ) + xk + ti, s );
	}

	void Round( unsigned int &a, unsigned int &b, unsigned int &c, unsigned int &d, unsigned int x[ 16 ] )
	{
		unsigned int A = a;
		unsigned int B = b;
		unsigned int C = c;
		unsigned int D = d;

		// ���[�h�u���b�N���Ƃ̏������s��
		FF( A, B, C, D, x[  0 ],  7, 0xd76aa478 );
		FF( D, A, B, C, x[  1 ], 12, 0xe8c7b756 );
		FF( C, D, A, B, x[  2 ], 17, 0x242070db );
		FF( B, C, D, A, x[  3 ], 22, 0xc1bdceee );
		FF( A, B, C, D, x[  4 ],  7, 0xf57c0faf );
		FF( D, A, B, C, x[  5 ], 12, 0x4787c62a );
		FF( C, D, A, B, x[  6 ], 17, 0xa8304613 );
		FF( B, C, D, A, x[  7 ], 22, 0xfd469501 );
		FF( A, B, C, D, x[  8 ],  7, 0x698098d8 );
		FF( D, A, B, C, x[  9 ], 12, 0x8b44f7af );
		FF( C, D, A, B, x[ 10 ], 17, 0xffff5bb1 );
		FF( B, C, D, A, x[ 11 ], 22, 0x895cd7be );
		FF( A, B, C, D, x[ 12 ],  7, 0x6b901122 );
		FF( D, A, B, C, x[ 13 ], 12, 0xfd987193 );
		FF( C, D, A, B, x[ 14 ], 17, 0xa679438e );
		FF( B, C, D, A, x[ 15 ], 22, 0x49b40821 );

		GG( A, B, C, D, x[  1 ],  5, 0xf61e2562 );
		GG( D, A, B, C, x[  6 ],  9, 0xc040b340 );
		GG( C, D, A, B, x[ 11 ], 14, 0x265e5a51 );
		GG( B, C, D, A, x[  0 ], 20, 0xe9b6c7aa );
		GG( A, B, C, D, x[  5 ],  5, 0xd62f105d );
		GG( D, A, B, C, x[ 10 ],  9, 0x02441453 );
		GG( C, D, A, B, x[ 15 ], 14, 0xd8a1e681 );
		GG( B, C, D, A, x[  4 ], 20, 0xe7d3fbc8 );
		GG( A, B, C, D, x[  9 ],  5, 0x21e1cde6 );
		GG( D, A, B, C, x[ 14 ],  9, 0xc33707d6 );
		GG( C, D, A, B, x[  3 ], 14, 0xf4d50d87 );
		GG( B, C, D, A, x[  8 ], 20, 0x455a14ed );
		GG( A, B, C, D, x[ 13 ],  5, 0xa9e3e905 );
		GG( D, A, B, C, x[  2 ],  9, 0xfcefa3f8 );
		GG( C, D, A, B, x[  7 ], 14, 0x676f02d9 );
		GG( B, C, D, A, x[ 12 ], 20, 0x8d2a4c8a );

		HH( A, B, C, D, x[  5 ],  4, 0xfffa3942 );
		HH( D, A, B, C, x[  8 ], 11, 0x8771f681 );
		HH( C, D, A, B, x[ 11 ], 16, 0x6d9d6122 );
		HH( B, C, D, A, x[ 14 ], 23, 0xfde5380c );
		HH( A, B, C, D, x[  1 ],  4, 0xa4beea44 );
		HH( D, A, B, C, x[  4 ], 11, 0x4bdecfa9 );
		HH( C, D, A, B, x[  7 ], 16, 0xf6bb4b60 );
		HH( B, C, D, A, x[ 10 ], 23, 0xbebfbc70 );
		HH( A, B, C, D, x[ 13 ],  4, 0x289b7ec6 );
		HH( D, A, B, C, x[  0 ], 11, 0xeaa127fa );
		HH( C, D, A, B, x[  3 ], 16, 0xd4ef3085 );
		HH( B, C, D, A, x[  6 ], 23, 0x04881d05 );
		HH( A, B, C, D, x[  9 ],  4, 0xd9d4d039 );
		HH( D, A, B, C, x[ 12 ], 11, 0xe6db99e5 );
		HH( C, D, A, B, x[ 15 ], 16, 0x1fa27cf8 );
		HH( B, C, D, A, x[  2 ], 23, 0xc4ac5665 );

		II( A, B, C, D, x[  0 ],  6, 0xf4292244 );
		II( D, A, B, C, x[  7 ], 10, 0x432aff97 );
		II( C, D, A, B, x[ 14 ], 15, 0xab9423a7 );
		II( B, C, D, A, x[  5 ], 21, 0xfc93a039 );
		II( A, B, C, D, x[ 12 ],  6, 0x655b59c3 );
		II( D, A, B, C, x[  3 ], 10, 0x8f0ccc92 );
		II( C, D, A, B, x[ 10 ], 15, 0xffeff47d );
		II( B, C, D, A, x[  1 ], 21, 0x85845dd1 );
		II( A, B, C, D, x[  8 ],  6, 0x6fa87e4f );
		II( D, A, B, C, x[ 15 ], 10, 0xfe2ce6e0 );
		II( C, D, A, B, x[  6 ], 15, 0xa3014314 );
		II( B, C, D, A, x[ 13 ], 21, 0x4e0811a1 );
		II( A, B, C, D, x[  4 ],  6, 0xf7537e82 );
		II( D, A, B, C, x[ 11 ], 10, 0xbd3af235 );
		II( C, D, A, B, x[  2 ], 15, 0x2ad7d2bb );
		II( B, C, D, A, x[  9 ], 21, 0xeb86d391 );

		a += A;
		b += B;
		c += C;
		d += D;
	}

public:
	/// @brief data[ 0 ] ���� data[ len - 1 ] �� len �o�C�g�� MD5 ���v�Z����D
	void generate( const void *data_, size_type len )
	{
		// �o�͗p�̃_�C�W�F�X�g�o�C�g��� 32 �r�b�g�P�ʂŏ����ł���悤�ɂ���
		unsigned int &A = *reinterpret_cast< unsigned int * >( digest );
		unsigned int &B = *reinterpret_cast< unsigned int * >( digest + 4 );
		unsigned int &C = *reinterpret_cast< unsigned int * >( digest + 8 );
		unsigned int &D = *reinterpret_cast< unsigned int * >( digest + 12 );

		// �_�C�W�F�X�g�o�C�g��̏�����
		A = 0x67452301;
		B = 0xefcdab89;
		C = 0x98badcfe;
		D = 0x10325476;

		size_type i;
		unsigned char xx[ 64 ];
		unsigned int *x = reinterpret_cast< unsigned int * >( xx );
		//unsigned int x[ 16 ];
		//unsigned char *xx = reinterpret_cast< unsigned char * >( x );
		const unsigned char *data = reinterpret_cast< const unsigned char * >( data_ );

		// ���̓f�[�^�ɑ΂��ă��b�Z�[�W�������s��
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * 64 );
			Round( A, B, C, D, x );
		}

		size_type rest = len - i;

		// �Ō�Ƀo�C�g���𑫂��������݂��Ȃ���΁C64�o�C�g�Ɋg�����ď�������
		if( rest >= 64 - 8 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 64 - rest ) );
			// �擪�̃r�b�g�� 1 �ɂ���
			xx[ rest ] = 0x80;

			// ���b�Z�[�W�������s��
			Round( A, B, C, D, x );

			// �o�C�g���̕��̏������s��
			memset( xx, 0, sizeof( unsigned char ) * 64 );
			x[ 14 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );

			// ���b�Z�[�W�������s��
			Round( A, B, C, D, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 64 - rest ) );
			// �擪�̃r�b�g�� 1 �ɂ���
			xx[ rest ] = 0x80;

			// �o�C�g���̕��̒l��t������
			x[ 14 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );

			// ���b�Z�[�W�������s��
			Round( A, B, C, D, x );
		}
	}

	/// @brief �_�C�W�F�X�g�o�C�g��̒���
	size_type size( ) const { return( 64 ); }

	/// @brief �_�C�W�F�X�g������̊e�o�C�g���擾����
	unsigned char operator []( size_type index ) const { return( digest[ index ] ); }


	/// @brief �_�C�W�F�X�g�������Ԃ�
	std::string to_string( ) const
	{
		static char x16[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		std::string str;

		for( size_type i = 0 ; i < 16 ; i++ )
		{
			str += x16[ ( digest[ i ] >> 4 ) & 0x0f ];
			str += x16[ digest[ i ] & 0x0f ];
		}

		return( str );
	}

	/// @brief �󕶎��̃_�C�W�F�X�g������ŏ���������
	md5( )
	{
		unsigned int *D = reinterpret_cast< unsigned int * >( digest );
		D[ 0 ] = 0xd98c1dd4;
		D[ 1 ] = 0x04b2008f;
		D[ 2 ] = 0x980980e9;
		D[ 3 ] = 0x7e42f8ec;
	}

	/// @brief �w�肳�ꂽ������̃_�C�W�F�X�g������ŏ���������
	md5( const std::string &str ){ generate( str.c_str( ), str.length( ) ); }

	/// @brief �w�肳�ꂽ�o�C�g��̃_�C�W�F�X�g������ŏ���������
	md5( const void *data, size_type len ){ generate( data, len ); }


public:
	/// @brief 2��MD5�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const md5 &m ) const
	{
		for( size_type i = 0 ; i < size( ) ; i++ )
		{
			if( digest[ i ] != m[ i ] )
			{
				return( false );
			}
		}
		return( true );
	}

	/// @brief 2�̃_�C�W�F�X�g�����񂪓��ꂩ�ǂ����𔻒肷��
	bool operator ==( const std::string &str ) const
	{
		return( str == to_string( ) );
	}
};


/// @brief �w�肳�ꂽ�X�g���[���Ƀf�[�^���o�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     p   �c weak_ptr �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
inline std::ostream &operator <<( std::ostream &out, const md5 &m )
{
	out << m.to_string( );
	return( out );
}


/// @}
//  �n�b�V���֐��O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MD5__
