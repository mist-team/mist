/// @file mist/operator/operator_array1.h
//!
//! @brief 1�����摜�p�̃I�y���[�^��`
//!
#ifndef __INCLUDE_MIST_OPERATOR_ARRAY1__
#define __INCLUDE_MIST_OPERATOR_ARRAY1__



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator +=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't add array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator -=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't subtract array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator *=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �|���Z�ł��܂����O
		::std::cout << "can't multiply array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator /=( array1< T, Allocator > &a1, const array1< T, Allocator >  &a2 )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY1_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// ����Z�ł��܂����O
		::std::cout << "can't divide array1s." << ::std::endl;
		return( a1 );
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= static_cast< T >( a2[i] );
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator +=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator -=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator *=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array1< T, Allocator >& operator /=( array1< T, Allocator > &a1, typename array1< T, Allocator >::value_type val )
{
	typedef typename array1< T, Allocator >::size_type size_type;
#ifndef _CHECK_ARRAY_OPERATION_
	if( val == 0 )
	{
		// �[�����Z����
		::std::cout << "zero division occured." << ::std::endl;
		return;
	}
#endif
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] /= val;
	return( a1 );
}




/// @brief �������]
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a )
{
	typedef typename array1< T, Allocator >::size_type size_type;
	array1< T, Allocator > o( a.size1( ), a.reso1( ) );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -a[i];
	return( o );
}



/// @brief �����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator +( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) += a2 );
}




/// @brief �����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) -= a2 );
}




/// @brief �|���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator *( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) *= a2 );
}



/// @brief ����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator /( const array1< T, Allocator > &a1, const array1< T, Allocator > &a2 )
{
	return( array1< T, Allocator >( a1 ) /= a2 );
}



/// @brief �萔�Ƃ̑����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator +( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) += val );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator +( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &a )
{
	return( array1< T, Allocator >( a ) += val );
}




/// @brief �萔�Ƃ̈����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator -( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) -= val );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator -( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &a )
{
	return( array1< T, Allocator >( a ) -= val );
}



/// @brief �萔�Ƃ̊|���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator *( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) *= val );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator *( typename array1< T, Allocator >::value_type val, const array1< T, Allocator > &a )
{
	return( array1< T, Allocator >( a ) *= val );
}



/// @brief �萔�Ƃ̊���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline array1< T, Allocator > operator /( const array1< T, Allocator > &a, typename array1< T, Allocator >::value_type val )
{
	return( array1< T, Allocator >( a ) /= val );
}


#endif // __INCLUDE_MIST_OPERATOR_ARRAY1__
