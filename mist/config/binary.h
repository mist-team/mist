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
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
class binary
{
public:
	typedef bool& reference;
	typedef const bool& const_reference;
	typedef bool value_type;
	typedef bool* pointer;
	typedef const bool* const_pointer;

private:
	bool value_;

public:
	binary( ) : value_( false ){ }
	binary( const value_type &b ) : value_( b ){ }
	binary( const binary &b ) : value_( b.value_ ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const binary &operator  =( const binary &b ){ value_ = b.value_;  return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const binary &operator  =( const value_type &b ){ value_ = b;   return( *this ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	const binary &operator +=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const binary &operator -=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }
	const binary &operator *=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const binary &operator /=( const binary &b ){ value_ = value_ ==  b.value_; return( *this ); }
	const binary &operator %=( const binary &b ){ value_ = value_ && !b.value_; return( *this ); }
	const binary &operator |=( const binary &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const binary &operator &=( const binary &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const binary &operator ^=( const binary &b ){ value_ = value_ !=  b.value_; return( *this ); }

	const binary &operator +=( const value_type &b ){ return( operator +=( binary( b ) ) ); }
	const binary &operator -=( const value_type &b ){ return( operator -=( binary( b ) ) ); }
	const binary &operator *=( const value_type &b ){ return( operator *=( binary( b ) ) ); }
	const binary &operator /=( const value_type &b ){ return( operator /=( binary( b ) ) ); }

	bool operator ==( const binary &b ) const { return( value_ == b.value_ ); }
	bool operator !=( const binary &b ) const { return( value_ != b.value_ ); }
	bool operator < ( const binary &b ) const { return( value_ <  b.value_ ); }
	bool operator <=( const binary &b ) const { return( value_ <= b.value_ ); }
	bool operator > ( const binary &b ) const { return( value_ >  b.value_ ); }
	bool operator >=( const binary &b ) const { return( value_ >= b.value_ ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! @return        �c �߂�l�̐���
	//! 
	value_type get_value( ) const { return( value_ ); }

	// bool�ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator bool( ) const { return( value_ ); }
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
inline const binary operator +( const binary &b1, const binary &b2 ){ return( binary( b1 ) += b2 ); }
inline const binary operator -( const binary &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }
inline const binary operator *( const binary &b1, const binary &b2 ){ return( binary( b1 ) *= b2 ); }
inline const binary operator /( const binary &b1, const binary &b2 ){ return( binary( b1 ) /= b2 ); }
inline const binary operator %( const binary &b1, const binary &b2 ){ return( binary( b1 ) %= b2 ); }
inline const binary operator |( const binary &b1, const binary &b2 ){ return( binary( b1 ) |= b2 ); }
inline const binary operator &( const binary &b1, const binary &b2 ){ return( binary( b1 ) &= b2 ); }
inline const binary operator ^( const binary &b1, const binary &b2 ){ return( binary( b1 ) ^= b2 ); }

inline const binary operator *( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) *= b2 ); }
inline const binary operator *( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) *= b1 ); }
inline const binary operator /( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) /= b2 ); }

inline const binary operator +( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) += b2 ); }
inline const binary operator +( const binary::value_type &b1, const binary &b2 ){ return( binary( b2 ) += b1 ); }
inline const binary operator -( const binary &b1, const binary::value_type &b2 ){ return( binary( b1 ) -= b2 ); }
inline const binary operator -( const binary::value_type &b1, const binary &b2 ){ return( binary( b1 ) -= b2 ); }



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
inline std::ostream &operator <<( std::ostream &out, const binary &v )
{
	out << v.get_value( );
	return( out );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_BINARY_H__
