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


/// @brief �o�C�i�����Z���s�����߂̂���
//! 
//! @code �o�C�i���摜�̍쐬��
//! mist::array2< mist::binary > image;
//! @endcode
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
	bool value_;		///< @brief �o�C�i���̊�{����

public:
	/// @brief �f�t�H���g�R���X�g���N�^�i�v�f�� false �ŏ���������j
	binary( ) : value_( false ){ }

	/// @brief ������ b �ŏ���������
	binary( const value_type &b ) : value_( b ){ }

	/// @brief ���̃o�C�i����f��p���ď���������
	binary( const binary &b ) : value_( b.value_ ){ }


	/// @brief ���̃o�C�i����f��������
	const binary &operator  =( const binary &b ){ value_ = b.value_; return( *this ); }

	/// @brief �v�f�� b ��������
	const binary &operator  =( const value_type &b ){ value_ = b; return( *this ); }


	/// @brief �_���a
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> true
	//! - true  + false -> true
	//! - false + true  -> true
	//! - false + false -> false
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator +=( const binary &b ){ value_ = value_ || b.value_; return( *this ); }

	/// @brief �_����
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> false
	//! - true  + false -> true
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator -=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief �_����
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> true
	//! - true  + false -> false
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator *=( const binary &b ){ value_ = value_ && b.value_; return( *this ); }

	/// @brief �r���_����
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> true
	//! - true  + false -> false
	//! - false + true  -> false
	//! - false + false -> true
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator /=( const binary &b ){ value_ = value_ == b.value_; return( *this ); }

	/// @brief �_����
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> false
	//! - true  + false -> true
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator %=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }

	/// @brief �_���a
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> true
	//! - true  + false -> true
	//! - false + true  -> true
	//! - false + false -> false
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator |=( const binary &b ){ value_ = value_ || b.value_; return( *this ); }

	/// @brief �_����
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> true
	//! - true  + false -> false
	//! - false + true  -> false
	//! - false + false -> false
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator &=( const binary &b ){ value_ = value_ && b.value_; return( *this ); }

	/// @brief �r���_���a
	//! 
	//! �ȉ��̉��Z���s��
	//! - true  + true  -> false
	//! - true  + false -> true
	//! - false + true  -> true
	//! - false + false -> false
	//! 
	//! @param[in] b �c �E�Ӓl
	//! 
	//! @return ���Z����
	//! 
	const binary &operator ^=( const binary &b ){ value_ = value_ != b.value_; return( *this ); }


	/// @brief �_���a
	const binary &operator +=( const value_type &b ){ return( operator +=( binary( b ) ) ); }

	/// @brief �_����
	const binary &operator -=( const value_type &b ){ return( operator -=( binary( b ) ) ); }

	/// @brief �_����
	const binary &operator *=( const value_type &b ){ return( operator *=( binary( b ) ) ); }

	/// @brief �r���_����
	const binary &operator /=( const value_type &b ){ return( operator /=( binary( b ) ) ); }


	/// @brief ���͂��ꂽ2�̃I�u�W�F�N�g�����������ǂ����𔻒肷��
	bool operator ==( const binary &b ) const { return( value_ == b.value_ ); }

	/// @brief ���͂��ꂽ2�̃I�u�W�F�N�g���������Ȃ����ǂ����𔻒肷��
	bool operator !=( const binary &b ) const { return( value_ != b.value_ ); }

	/// @brief ���͂��ꂽ2�̃I�u�W�F�N�g�� < �֌W�𔻒肷��
	bool operator < ( const binary &b ) const { return( value_ <  b.value_ ); }

	/// @brief ���͂��ꂽ2�̃I�u�W�F�N�g�� <= �֌W�𔻒肷��
	bool operator <=( const binary &b ) const { return( value_ <= b.value_ ); }

	/// @brief ���͂��ꂽ2�̃I�u�W�F�N�g�� > �֌W�𔻒肷��
	bool operator > ( const binary &b ) const { return( value_ >  b.value_ ); }

	/// @brief ���͂��ꂽ2�̃I�u�W�F�N�g�� >= �֌W�𔻒肷��
	bool operator >=( const binary &b ) const { return( value_ >= b.value_ ); }


	/// @brief �����f�[�^���擾����
	value_type get_value( ) const { return( value_ ); }

	// bool�ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator bool( ) const { return( value_ ); }
};


/// @brief �_�����Z
inline const binary operator +( const binary &b1, const binary &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief �_�����Z
inline const binary operator -( const binary &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief �_�����Z
inline const binary operator *( const binary &b1, const binary &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief �_�����Z
inline const binary operator /( const binary &b1, const binary &b2 ){ return( binary( b1 ) /= b2 ); }

/// @brief �_�����Z
inline const binary operator %( const binary &b1, const binary &b2 ){ return( binary( b1 ) %= b2 ); }

/// @brief �_�����Z
inline const binary operator |( const binary &b1, const binary &b2 ){ return( binary( b1 ) |= b2 ); }

/// @brief �_�����Z
inline const binary operator &( const binary &b1, const binary &b2 ){ return( binary( b1 ) &= b2 ); }

/// @brief �_�����Z
inline const binary operator ^( const binary &b1, const binary &b2 ){ return( binary( b1 ) ^= b2 ); }


/// @brief �_�����Z
inline const binary operator *( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) *= b2 ); }

/// @brief �_�����Z
inline const binary operator *( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) *= b1 ); }

/// @brief �_�����Z
inline const binary operator /( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) /= b2 ); }


/// @brief �_�����Z
inline const binary operator +( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) += b2 ); }

/// @brief �_�����Z
inline const binary operator +( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) += b1 ); }

/// @brief �_�����Z
inline const binary operator -( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) -= b2 ); }

/// @brief �_�����Z
inline const binary operator -( const binary::value_type &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }



/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     b   �c �o�C�i����f
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1
//! @endcode
//! 
inline std::ostream &operator <<( std::ostream &out, const binary &b )
{
	out << b.get_value( );
	return( out );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_BINARY_H__
