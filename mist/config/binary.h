/// @file mist/config/binary.h
//!
//! @brief �o�C�i�����Z���s�����߂̃N���X
//!

#ifndef __INCLUDE_MIST_BINARY_H__
#define __INCLUDE_MIST_BINARY_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// MIST�ŗ��p������̃f�[�^�^


/// @brief binary���Z���s�����߂̂���
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
class binary
{
public:
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef bool& reference;				///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const bool& const_reference;	///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef bool value_type;				///< @brief �����f�[�^�^�Dbool �Ɠ���
	typedef bool* pointer;					///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef const bool* const_pointer;		///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

private:
	bool value_;		///< @brief �ϐ��̐���������

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	binary( ) : value_( false ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	binary( const value_type &b ) : value_( b ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	binary( const binary &b ) : value_( b.value_ ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator  =( const binary &b ){ value_ = b.value_;  return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator  =( const value_type &b ){ value_ = b;   return( *this ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator +=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator -=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator *=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator /=( const binary &b ){ value_ = value_ ==  b.value_; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator %=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator |=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator &=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator ^=( const binary &b ){ value_ = value_ !=  b.value_; return( *this ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator +=( const value_type &b ){ return( operator +=( binary( b ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator -=( const value_type &b ){ return( operator -=( binary( b ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator *=( const value_type &b ){ return( operator *=( binary( b ) ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const binary &operator /=( const value_type &b ){ return( operator /=( binary( b ) ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false  �c �����̐���
	//! 
	bool operator ==( const binary &b ) const { return( value_ == b.value_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false  �c �����̐���
	//! 
	bool operator !=( const binary &b ) const { return( value_ != b.value_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false  �c �����̐���
	//! 
	bool operator < ( const binary &b ) const { return( value_ <  b.value_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false  �c �����̐���
	//! 
	bool operator <=( const binary &b ) const { return( value_ <= b.value_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false  �c �����̐���
	//! 
	bool operator > ( const binary &b ) const { return( value_ >  b.value_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] b �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false  �c �����̐���
	//! 
	bool operator >=( const binary &b ) const { return( value_ >= b.value_ ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type get_value( ) const { return( value_ ); }

	// bool�ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator bool( ) const { return( value_ ); }
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator +( const binary &b1, const binary &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator -( const binary &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator *( const binary &b1, const binary &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator /( const binary &b1, const binary &b2 ){ return( binary( b1 ) /= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator %( const binary &b1, const binary &b2 ){ return( binary( b1 ) %= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator |( const binary &b1, const binary &b2 ){ return( binary( b1 ) |= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator &( const binary &b1, const binary &b2 ){ return( binary( b1 ) &= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator ^( const binary &b1, const binary &b2 ){ return( binary( b1 ) ^= b2 ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator *( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator *( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) *= b1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator /( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) /= b2 ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator +( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator +( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) += b1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator -( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] b1 �c �����̐���
//! @param[in] b2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline const binary operator -( const binary::value_type &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     v   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
inline std::ostream &operator <<( std::ostream &out, const binary &v )
{
	out << v.get_value( );
	return( out );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_BINARY_H__
