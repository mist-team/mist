/// @file mist/operator/operator_array3.h
//!
//! @brief 3�����摜�p�̃I�y���[�^��`
//!
#ifndef __INCLUDE_MIST_OPERATOR_ARRAY3__
#define __INCLUDE_MIST_OPERATOR_ARRAY3__



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't add array3s." << ::std::endl;
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
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator -=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't subtract array3s." << ::std::endl;
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
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator *=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �|���Z�ł��܂����O
		::std::cout << "can't multiply array3s." << ::std::endl;
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
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator /=( array3< T, Allocator > &a1, const array3< T, Allocator >  &a2 )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// ����Z�ł��܂����O
		::std::cout << "can't divide array3s." << ::std::endl;
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
//! @param[in] a1  �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator +=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] += val;
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1  �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator -=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] -= val;
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1  �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator *=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	for( size_type i = 0 ; i < a1.size( ) ; i++ ) a1[i] *= val;
	return( a1 );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1  �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline const array3< T, Allocator >& operator /=( array3< T, Allocator > &a1, typename array3< T, Allocator >::value_type val )
{
	typedef typename array3< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY3_OPERATION_ != 0
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
//! @param[in] a �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a )
{
	typedef typename array3< T, Allocator >::size_type size_type;
	array3< T, Allocator > o( a.size1( ), a.size2( ), a.size3( ), a.reso1( ), a.reso2( ), a.reso3( ) );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -a[i];
	return( o );
}



/// @brief �����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator +( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) += a2 );
}



/// @brief �����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) -= a2 );
}



/// @brief �|���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator *( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) *= a2 );
}



/// @brief ����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a1 �c �����̐���
//! @param[in] a2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator /( const array3< T, Allocator > &a1, const array3< T, Allocator > &a2 )
{
	return( array3< T, Allocator >( a1 ) /= a2 );
}



/// @brief �萔�Ƃ̑����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a   �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator +( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) += val );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] val �c �����̐���
//! @param[in] a   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator +( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) += val );
}




/// @brief �萔�Ƃ̈����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a   �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator -( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) -= val );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] val �c �����̐���
//! @param[in] a   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator -( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) -= val );
}



/// @brief �萔�Ƃ̊|���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a   �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator *( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) *= val );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] val �c �����̐���
//! @param[in] a   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator *( typename array3< T, Allocator >::value_type val, const array3< T, Allocator > &a )
{
	return( array3< T, Allocator >( a ) *= val );
}



/// @brief �萔�Ƃ̊���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a   �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array3< T, Allocator > operator /( const array3< T, Allocator > &a, typename array3< T, Allocator >::value_type val )
{
	return( array3< T, Allocator >( a ) /= val );
}



#endif // __INCLUDE_MIST_OPERATOR_ARRAY3__
