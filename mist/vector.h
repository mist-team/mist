/// @file mist/vector.h
//!
//! @brief �e�����ɑΉ������x�N�g�����Z���s�����߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_VECTOR__
#define __INCLUDE_MIST_VECTOR__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

//! @addtogroup vector_group �x�N�g�����C�u����
//!  @{



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
class vector3
{
public:
	typedef T value_type;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

public:
	value_type x;		///< @brief �ϐ��̐���������
	value_type y;		///< @brief �ϐ��̐���������
	value_type z;		///< @brief �ϐ��̐���������

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	vector3( ) : x( 0 ), y( 0 ), z( 0 ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] xx �c �����̐���
	//! @param[in] yy �c �����̐���
	//! @param[in] zz �c �����̐���
	//! 
	vector3( const value_type &xx, const value_type &yy, const value_type &zz ) : x( xx ), y( yy ), z( zz ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	template < class TT >
	vector3( const vector3< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	vector3( const vector3< T > &v ) : x( v.x ), y( v.y ), z( v.z ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector3 &operator =( const vector3 &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		return ( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	vector3 operator -( ) const { return ( vector3( -x, -y, -z ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector3 &operator +=( const vector3 &v )
	{
		x = x + v.x;
		y = y + v.y;
		z = z + v.z;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector3 &operator -=( const vector3 &v )
	{
		x = x - v.x;
		y = y - v.y;
		z = z - v.z;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] a �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector3 &operator *=( const value_type &a )
	{
		x *= a;
		y *= a;
		z *= a;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] a �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector3 &operator /=( const value_type &a )
	{
		x /= a;
		y /= a;
		z /= a;
		return( *this );
	}


	/// @brief ����
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type operator ^( const vector3 &v ) const { return( inner( v ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator ==( const vector3 &v ) const { return( x == v.x && y == v.y && z == v.z ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator !=( const vector3 &v ) const { return( !( *this == v ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator < ( const vector3 &v ) const { return( !( *this >= v ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator <=( const vector3 &v ) const { return( v >= *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator > ( const vector3 &v ) const { return( v < *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator >=( const vector3 &v ) const { return( x >= v.x && y >= v.y && z >= v.z ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	vector3 unit( ) const
	{
		value_type length_ = length( );
		return vector3( x / length_, y / length_, z / length_ );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type inner( const vector3 &v ) const { return( x * v.x + y * v.y + z * v.z ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	vector3 outer( const vector3 &v ) const { return( vector3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type length( ) const { return ( value_type( sqrt( (double)( x * x + y * y + z * z ) ) ) ); }

	/// @brief �x�N�g���̉�]
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v     �c �����̐���
	//! @param[in] theta �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	vector3 rotate( const vector3 &v, double theta ) const
	{
		theta *= 3.1415926535897932384626433832795 / 180.0;
		double cs = std::cos( theta ), sn = std::sin( theta );
		value_type xx = static_cast< value_type >( ( v.x * v.x * ( 1.0 - cs ) + cs ) * x + ( v.x * v.y * ( 1.0 - cs ) - v.z * sn ) * y + ( v.x * v.z * ( 1.0 - cs ) + v.y * sn ) * z );
		value_type yy = static_cast< value_type >( ( v.x * v.y * ( 1.0 - cs ) + v.z * sn ) * x + ( v.y * v.y * ( 1.0 - cs ) + cs ) * y + ( v.y * v.z * ( 1.0 - cs ) - v.x * sn ) * z );
		value_type zz = static_cast< value_type >( ( v.x * v.z * ( 1.0 - cs ) - v.y * sn ) * x + ( v.y * v.z * ( 1.0 - cs ) + v.x * sn ) * y + ( v.z * v.z * ( 1.0 - cs ) + cs ) * z );
		return ( vector3( xx, yy, zz ) );
	}
};



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector3< T > operator +( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1 ) += v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector3< T > operator -( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1 ) -= v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector3< T > operator /( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1 ) /= v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector3< T > operator *( const vector3< T > &v1, const vector3< T > &v2 ){ return( v1.outer( v2 ) ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const typename vector3< T >::value_type operator ^( const vector3< T > &v1, const vector3< T > &v2 ){ return( v1.inner( v2 ) ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector3< T > operator *( const vector3< T > &v1, const typename vector3< T >::value_type &v2 ){ return( vector3< T >( v1 ) *= v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector3< T > operator *( const typename vector3< T >::value_type &v1, const vector3< T > &v2 ){ return( vector3< T >( v2 ) *= v1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector3< T > operator /( const vector3< T > &v1, const typename vector3< T >::value_type &v2 ){ return( vector3< T >( v1 ) /= v2 ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     v   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const vector3< T > &v )
{
	out << "( ";
	out << v.x << ", ";
	out << v.y << ", ";
	out << v.z << " )";
	return( out );
}



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T  �c �����̐���
//! 
template < class T >
class vector2
{
public:
	typedef T value_type;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

public:
	value_type x;		///< @brief �ϐ��̐���������
	value_type y;		///< @brief �ϐ��̐���������

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	vector2( ) : x( 0 ), y( 0 ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] xx �c �����̐���
	//! @param[in] yy �c �����̐���
	//! 
	vector2( const value_type &xx, const value_type &yy ) : x( xx ), y( yy ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	template < class TT >
	vector2( const vector2< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	vector2( const vector2< T > &v ) : x( v.x ), y( v.y ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector2 &operator =( const vector2 &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
		}
		return ( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	vector2 operator -( ) const { return ( vector2( -x, -y ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector2 &operator +=( const vector2 &v )
	{
		x = x + v.x;
		y = y + v.y;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector2 &operator -=( const vector2 &v )
	{
		x = x - v.x;
		y = y - v.y;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] a �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector2 &operator *=( const value_type &a )
	{
		x *= a;
		y *= a;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] a �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const vector2 &operator /=( const value_type &a )
	{
		x /= a;
		y /= a;
		return( *this );
	}


	/// @brief ����
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type operator ^( const vector2 &v ) const { return( inner( v ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator ==( const vector2 &v ) const { return( x == v.x && y == v.y ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator !=( const vector2 &v ) const { return( !( *this == v ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator < ( const vector2 &v ) const { return( !( *this >= v ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator <=( const vector2 &v ) const { return( v >= *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator > ( const vector2 &v ) const { return( v < *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator >=( const vector2 &v ) const { return( x >= v.x && y >= v.y ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	vector2 unit( ) const
	{
		value_type length_ = length( );
		return vector2( x / length_, y / length_ );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type inner( const vector2 &v ) const { return( x * v.x + y * v.y ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type outer( const vector2 &v ) const { return( x * v.y - y * v.x ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type length( ) const { return ( value_type( sqrt( (double)( x * x + y * y ) ) ) ); }

	//// �x�N�g���̉�]
	//vector2 rotate( const vector2 &a, double theta ) const
	//{
	//	theta *= 3.1415926535897932384626433832795 / 180.0;
	//	double cs = std::cos( theta ), sn = std::sin( theta );
	//	value_type xx = static_cast< value_type >( ( v.x * v.x * ( 1.0 - cs ) + cs ) * x + ( v.x * v.y * ( 1.0 - cs ) - v.z * sn ) * y + ( v.x * v.z * ( 1.0 - cs ) + v.y * sn ) * z );
	//	value_type yy = static_cast< value_type >( ( v.x * v.y * ( 1.0 - cs ) + v.z * sn ) * x + ( v.y * v.y * ( 1.0 - cs ) + cs ) * y + ( v.y * v.z * ( 1.0 - cs ) - v.x * sn ) * z );
	//	value_type zz = static_cast< value_type >( ( v.x * v.z * ( 1.0 - cs ) - v.y * sn ) * x + ( v.y * v.z * ( 1.0 - cs ) + v.x * sn ) * y + ( v.z * v.z * ( 1.0 - cs ) + cs ) * z );
	//	return ( vector2( xx, yy, zz ) );
	//}
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector2< T > operator +( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1 ) += v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector2< T > operator -( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1 ) -= v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector2< T > operator /( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1 ) /= v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const typename vector2< T >::value_type operator *( const vector2< T > &v1, const vector2< T > &v2 ){ return( v1.outer( v2 ) ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const typename vector2< T >::value_type operator ^( const vector2< T > &v1, const vector2< T > &v2 ){ return( v1.inner( v2 ) ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector2< T > operator *( const vector2< T > &v1, const typename vector2< T >::value_type &v2 ){ return( vector2< T >( v1 ) *= v2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector2< T > operator *( const typename vector2< T >::value_type &v1, const vector2< T > &v2 ){ return( vector2< T >( v2 ) *= v1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const vector2< T > operator /( const vector2< T > &v1, const typename vector2< T >::value_type &v2 ){ return( vector2< T >( v1 ) /= v2 ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     v   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const vector2< T > &v )
{
	out << "( ";
	out << v.x << ", ";
	out << v.y << ", ";
	out << v.z << " )";
	return( out );
}



#if defined( _MIST_VECTOR_SUPPORT_ ) && _MIST_VECTOR_SUPPORT_ != 0


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T         �c �����̐���
//! @param Allocator �c �����̐���
//! 
template < class T, class Allocator = ::std::allocator< T > >
class vector : public matrix< T, Allocator >
{
private:
	typedef matrix< T, Allocator > base;		///< @brief ���N���X�������s��N���X

public:
	typedef typename base::allocator_type allocator_type;		///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename base::reference reference;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::array< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename base::const_reference const_reference;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename base::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef typename base::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename base::pointer pointer;						///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename base::const_pointer const_pointer;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�

	typedef typename base::iterator iterator;								///< @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_iterator const_iterator;					///< @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��


	//template < class TT, class AAlocator >
	//const vector& operator +=( const vector< TT, AAlocator > &v2 )
	//{
	//	base::operator +=( v2 );
	//	return( v1 );
	//}

	//template < class TT, class AAlocator >
	//const vector& operator -=( const vector< TT, AAlocator > &v2 )
	//{
	//	base::operator -=( v2 );
	//	return( v1 );
	//}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v2 �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class TT, class AAlocator >
	const vector& operator *=( const vector< TT, AAlocator > &v2 )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( base::size( ) != v2.size( ) || base::size( ) < 3 )
		{
			// �O�ς̌v�Z���ł��܂����O
			::std::cout << "can't calculate outer product of two vectors." << ::std::endl;
			return( *this );
		}
#endif

		typedef typename vector< T, Allocator >::size_type size_type;
		vector &v1 = *this;
		vector v( v1.size( ) );

		size_type i;
		for( i = 0 ; i < v.size( ) - 2 ; i++ )
		{
			v[ i ] = v1[ i + 1 ] * v2[ i + 2 ] - v1[ i + 2 ] * v2[ i + 1 ];
		}

		v[ i ] = v1[ i + 1 ] * v2[ 0 ] - v1[ 0 ] * v2[ i + 1 ];
		v[ i + 1 ] = v1[ 0 ] * v2[ 1 ] - v1[ 1 ] * v2[ 0 ];

		v1.swap( v );

		return( *this );
	}


	/// @brief ����
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type operator ^( const vector &v ) const { return( inner( v ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v2 �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type inner( const vector &v2 ) const
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( base::size( ) != v2.size( ) )
		{
			// ���ς̌v�Z���ł��܂����O
			::std::cout << "can't calculate inner product of two vectors." << ::std::endl;
			return( value_type( ) );
		}
#endif

		typedef typename vector< T, Allocator >::size_type size_type;
		const vector &v1 = *this;
		value_type v = value_type( );
		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			v += v1[ i ] * v2[ i ];
		}

		return( v );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	vector outer( const vector &v ) const
	{
		return( vector( *this ) *= v );
	}


public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	vector( ) : base( ) {}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] a �c �����̐���
	//! 
	explicit vector( const Allocator &a ) : base( a ) {}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] num �c �����̐���
	//! 
	vector( size_type num ) : base( num, 1 ) {}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] num �c �����̐���
	//! @param[in] a   �c �����̐���
	//! 
	vector( size_type num, const Allocator &a ) : base( num, 1, a ) {}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] num �c �����̐���
	//! @param[in] val �c �����̐���
	//! 
	vector( size_type num, const T &val ) : base( num, 1, val ) {}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] num �c �����̐���
	//! @param[in] val �c �����̐���
	//! @param[in] a   �c �����̐���
	//! 
	vector( size_type num, const T &val, const Allocator &a ) : base( num, 1, val, a ) {}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] o �c �����̐���
	//! 
	template < class TT, class AAlocator >
	vector( const vector< TT, AAlocator > &o ) : base( o ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] o �c �����̐���
	//! 
	vector( const vector< T, Allocator > &o ) : base( o ){ }

#if _USE_EXPRESSION_TEMPLATE_ != 0
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] expression �c �����̐���
	//! 
	template < class Expression >
	vector( const matrix_expression< Expression > &expression ) : base( expression.size( ), 1 )
	{
		vector &v = *this;
		for( size_type indx = 0 ; indx < v.size( ) ; indx++ )
		{
			v[ indx ] = expression[ indx ];
		}
	}
#endif
};



/// @brief �W���o�͂ւ̃x�N�g���̏o��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     v   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline ::std::ostream &operator <<( ::std::ostream &out, const vector< T, Allocator > &v )
{
	typename vector< T, Allocator >::size_type indx;
	for( indx = 0 ; indx < v.size( ) ; indx++ )
	{
		out << v[ indx ];
		if( indx != v.size( ) - 1 ) out << ", ";
	}
	out << ::std::endl;

	return( out );
}

#if 0
#else

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] v1 �c �����̐���
//! @param[in] v2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T, class Allocator >
inline vector< T, Allocator > operator *( const vector< T, Allocator > &v1, const vector< T, Allocator > &v2 )
{
	return( vector< T, Allocator >( v1 ) *= v2 );
}

#endif



#endif // _MIST_VECTOR_SUPPORT_

/// @}
//  �x�N�g�����C�u�����O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_VECTOR__
