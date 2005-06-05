/// @file mist/md5.h
//!
//! @brief �C�ӂ̃o�C�g���MD5���v�Z����
//!
//! - �Q�l����
//!   - RFC3174, http://www.ietf.org/rfc/rfc3174.txt
//!
#ifndef __INCLUDE_SHA__
#define __INCLUDE_SHA__

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


/// @brief SHA-1 �𐶐�����N���X
class sha1
{
public:
	typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	unsigned char digest[ 20 ];

protected:
	unsigned int R( unsigned int a, unsigned int s ){ return( ( a << s ) | ( a >> ( 32 - s ) ) ); }

	unsigned int left_most( unsigned int x )
	{
		return( ( x << 24 ) | ( ( x & 0x0000ff00 ) << 8 ) | ( ( x & 0x00ff0000 ) >> 8 ) | ( x  >> 24 ) );
	}

	void ToCurrentEndian( unsigned int *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = to_current_endian( byte_array< unsigned int >( x[ i ] ), false ).get_value( );
		}
	}

	void FromCurrentEndian( unsigned int *x, size_type len )
	{
		for( size_type i = 0 ; i < len ; i++ )
		{
			x[ i ] = from_current_endian( byte_array< unsigned int >( x[ i ] ), false ).get_value( );
		}
	}

	void Round( unsigned int &a, unsigned int &b, unsigned int &c, unsigned int &d, unsigned int &e, unsigned int x[ 16 ] )
	{
		size_type i;
		unsigned int W[ 80 ];

		memcpy( W, x, sizeof( unsigned int ) * 16 );

		for( i = 16 ; i < 80 ; i++ )
		{
			W[ i ] = R( W[ i - 3 ] ^ W[ i - 8 ] ^ W[ i - 14 ] ^ W[ i- 16 ], 1 );
		}

		unsigned int A = a;
		unsigned int B = b;
		unsigned int C = c;
		unsigned int D = d;
		unsigned int E = e;

		// ���[�h�u���b�N���Ƃ̏������s��
		for( i = 0 ; i < 20 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( ( B & C ) | ( ~B & D ) ) + E + W[ i ] + 0x5a827999;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 40 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( B ^ C ^ D ) + E + W[ i ] + 0x6ed9eba1;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 60 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( ( B & C ) | ( B & D ) | ( C & D ) ) + E + W[ i ] + 0x8f1bbcdc;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		for( ; i < 80 ; i++ )
		{
			unsigned int temp = R( A, 5 ) + ( B ^ C ^ D ) + E + W[ i ] + 0xca62c1d6;
			E = D;
			D = C;
			C = R( B, 30 );
			B = A;
			A = temp;
		}

		a += A;
		b += B;
		c += C;
		d += D;
		e += E;
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
		unsigned int &E = *reinterpret_cast< unsigned int * >( digest + 16 );

		// �_�C�W�F�X�g�o�C�g��̏�����
		const unsigned int H0 = 0x67452301;
		const unsigned int H1 = 0xefcdab89;
		const unsigned int H2 = 0x98badcfe;
		const unsigned int H3 = 0x10325476;
		const unsigned int H4 = 0xc3d2e1f0;

		A = H0;
		B = H1;
		C = H2;
		D = H3;
		E = H4;

		size_type i;
		unsigned int x[ 16 ];
		unsigned char *xx = reinterpret_cast< unsigned char * >( x );
		const unsigned char *data = reinterpret_cast< const unsigned char * >( data_ );

		// ���̓f�[�^�ɑ΂��ă��b�Z�[�W�������s��
		for( i = 0 ; i + 64 < len ; i += 64 )
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * 64 );
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, E, x );
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
			ToCurrentEndian( x, 16 );
			Round( A, B, C, D, E, x );

			// �o�C�g���̕��̏������s��
			memset( xx, 0, sizeof( unsigned char ) * 64 );
			x[ 14 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );

			// ���b�Z�[�W�������s��
			Round( A, B, C, D, E, x );
		}
		else
		{
			memcpy( xx, data + i, sizeof( unsigned char ) * rest );
			memset( xx + rest, 0, sizeof( unsigned char ) * ( 64 - rest ) );
			// �擪�̃r�b�g�� 1 �ɂ���
			xx[ rest ] = 0x80;

			ToCurrentEndian( x, 16 );

			// �o�C�g���̕��̒l��t������
			x[ 14 ] = static_cast< unsigned int >( ( len & 0xc0000000 ) >> 27 );
			x[ 15 ] = static_cast< unsigned int >( ( len & 0x3fffffff ) << 3 );

			// ���b�Z�[�W�������s��
			Round( A, B, C, D, E, x );
		}

		FromCurrentEndian( reinterpret_cast< unsigned int * >( digest ), 5 );
	}

	/// @brief �_�C�W�F�X�g�o�C�g��̒���
	size_type size( ) const { return( 20 ); }

	/// @brief �_�C�W�F�X�g������̊e�o�C�g���擾����
	unsigned char operator []( size_type index ) const { return( digest[ index ] ); }


	/// @brief �_�C�W�F�X�g�������Ԃ�
	std::string to_string( ) const
	{
		static char x16[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		std::string str;

		for( size_type i = 0 ; i < size( ) ; i++ )
		{
			str += x16[ ( digest[ i ] >> 4 ) & 0x0f ];
			str += x16[ digest[ i ] & 0x0f ];
		}

		return( str );
	}

	/// @brief �󕶎��̃_�C�W�F�X�g������ŏ���������
	sha1( )
	{
		unsigned int *D = reinterpret_cast< unsigned int * >( digest );
		D[ 0 ] = 0xd98c1dd4;
		D[ 1 ] = 0x04b2008f;
		D[ 2 ] = 0x980980e9;
		D[ 3 ] = 0x7e42f8ec;
		FromCurrentEndian( D, 20 );
	}

	/// @brief �w�肳�ꂽ������̃_�C�W�F�X�g������ŏ���������
	sha1( const std::string &str ){ generate( str.c_str( ), str.length( ) ); }

	/// @brief �w�肳�ꂽ�o�C�g��̃_�C�W�F�X�g������ŏ���������
	sha1( const void *data, size_type len ){ generate( data, len ); }


public:
	/// @brief 2��MD5�����ꂩ�ǂ����𔻒肷��
	bool operator ==( const sha1 &m ) const
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
inline std::ostream &operator <<( std::ostream &out, const sha1 &m )
{
	out << m.to_string( );
	return( out );
}


/// @}
//  �n�b�V���֐��O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_SHA__
