/// @file mist/operator/operator_array.h
//!
//! @brief 1�����z��p�̃I�y���[�^��`
//!
#ifndef __INCLUDE_MIST_OPERATOR_ARRAY__
#define __INCLUDE_MIST_OPERATOR_ARRAY__


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
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't add arrays." << ::std::endl;
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
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �����Z�ł��܂����O
		::std::cout << "can't subtract arrays." << ::std::endl;
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
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// �|���Z�ł��܂����O
		::std::cout << "can't multiply arrays." << ::std::endl;
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
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, const array< T, Allocator >  &a2 )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
	if( a1.size( ) != a2.size( ) )
	{
		// ����Z�ł��܂����O
		::std::cout << "can't divide arrays." << ::std::endl;
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
inline const array< T, Allocator >& operator +=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
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
inline const array< T, Allocator >& operator -=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
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
inline const array< T, Allocator >& operator *=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
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
inline const array< T, Allocator >& operator /=( array< T, Allocator > &a1, typename array< T, Allocator >::value_type val )
{
	typedef typename array< T, Allocator >::size_type size_type;
#if _CHECK_ARRAY_OPERATION_ != 0
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
inline array< T, Allocator > operator -( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	array< T, Allocator > o( a.size( ) );
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
inline array< T, Allocator > operator +( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) += a2 );
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
inline array< T, Allocator > operator -( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) -= a2 );
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
inline array< T, Allocator > operator *( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) *= a2 );
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
inline array< T, Allocator > operator /( const array< T, Allocator > &a1, const array< T, Allocator > &a2 )
{
	return( array< T, Allocator >( a1 ) /= a2 );
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
inline array< T, Allocator > operator +( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) += val );
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
inline array< T, Allocator > operator +( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) += val );
}




/// @brief �萔�Ƃ̈���
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a   �c �����̐���
//! @param[in] val �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline array< T, Allocator > operator -( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) -= val );
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
inline array< T, Allocator > operator -( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array2< T, Allocator >( a.size( ), val ) -= a );
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
inline array< T, Allocator > operator *( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) *= val );
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
inline array< T, Allocator > operator *( typename array< T, Allocator >::value_type val, const array< T, Allocator > &a )
{
	return( array< T, Allocator >( a ) *= val );
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
inline array< T, Allocator > operator /( const array< T, Allocator > &a, typename array< T, Allocator >::value_type val )
{
	return( array< T, Allocator >( a ) /= val );
}

#endif // __INCLUDE_MIST_OPERATOR_ARRAY__
