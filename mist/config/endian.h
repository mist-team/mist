/// @file mist/config/endian.h
//!
//! @brief �v�Z�@�̃G���f�B�A�����擾���C�����I�ɕϊ����郉�C�u����
//!

#ifndef __INCLUDE_MIST_ENDIAN__
#define __INCLUDE_MIST_ENDIAN__

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



/// @brief �o�C�g�z��ƒl�̃y�A��\������N���X
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T >
union byte_array
{
public:
	typedef T      value_type;
	typedef size_t size_type;

private:
	value_type value;
	unsigned char byte[ sizeof( value_type ) ];

public:
	byte_array( ) : value( 0 ){ }
	byte_array( const value_type v ) : value( v ){ }
	byte_array( const byte_array &v ) : value( v.value ){ }
	byte_array( const unsigned char *b )
	{
		for( size_type i = 0 ; i < sizeof( value_type ) ; i++ )
		{
			byte[ i ] = b[ i ];
		}
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	unsigned char &operator[]( size_type index ){ return( byte[ index ] ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const unsigned char &operator[]( size_type index ) const { return( byte[ index ] ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const value_type get_value( ) const { return( value ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	value_type set_value( const value_type &v ) { return( value = v ); }
};


/// @brief ���݂̃}�V���̃G���f�B�A�������g���G���f�B�A�����ǂ����𒲂ׂ�
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
inline bool _is_little_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 1 );
}


/// @brief ���݂̃}�V���̃G���f�B�A�����r�b�O�G���f�B�A�����ǂ����𒲂ׂ�
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
inline bool _is_big_endian_( )
{
	return( byte_array< unsigned short >( 1 )[ 0 ] == 0 );
}


/// @brief byte_array���̃o�C�g�X���b�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T >
inline void swap_bytes( byte_array< T > &bytes )
{
	typedef typename byte_array< T >::size_type size_type;
	byte_array< T > tmp( bytes );
	for( size_type i = 0 ; i < sizeof( T ) ; ++i )
	{
		bytes[ i ] = tmp[ sizeof( T ) - i - 1 ];
	}
}


/// @brief byte_array���̃f�[�^�����݂̌v�Z�@�̃G���f�B�A���ɍ��킹��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T >
inline byte_array< T > to_current_endian( const byte_array< T > &bytes, bool from_little_endian )
{
	static bool current_endian = _is_little_endian_( );
	if( current_endian != from_little_endian )
	{
		byte_array< T > tmp( bytes );
		swap_bytes( tmp );
		return( tmp );
	}
	else
	{
		return( bytes );
	}
}


/// @brief byte_array���̃f�[�^�����݂̌v�Z�@�̃G���f�B�A������ړI�̃G���f�B�A���ɍ��킹��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T >
inline byte_array< T > from_current_endian( const byte_array< T > &bytes, bool to_little_endian )
{
	static bool current_endian = _is_little_endian_( );
	if( current_endian != to_little_endian )
	{
		byte_array< T > tmp( bytes );
		swap_bytes( tmp );
		return( tmp );
	}
	else
	{
		return( bytes );
	}
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_ENDIAN__
