/// @file mist/quaternion.h
//!
//! @brief �N�H�[�^�j�I���i�l�����j���������߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_QUATERNION__
#define __INCLUDE_MIST_QUATERNION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T �c �����̐���
//! 
template < class T >
class quaternion
{
public:
	typedef T value_type;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

public:
	value_type w;		///< @brief �ϐ��̐���������
	value_type x;		///< @brief �ϐ��̐���������
	value_type y;		///< @brief �ϐ��̐���������
	value_type z;		///< @brief �ϐ��̐���������

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	quaternion( ) : w( 0 ), x( 0 ), y( 0 ), z( 0 ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ww �c �����̐���
	//! @param[in] xx �c �����̐���
	//! @param[in] yy �c �����̐���
	//! @param[in] zz �c �����̐���
	//! 
	quaternion( const value_type &ww, const value_type &xx, const value_type &yy, const value_type &zz ) : w( ww ), x( xx ), y( yy ), z( zz ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	template < class TT >
	quaternion( const quaternion< TT > &q ) : w( static_cast< value_type >( q.w ) ), x( static_cast< value_type >( q.x ) ), y( static_cast< value_type >( q.y ) ), z( static_cast< value_type >( q.z ) ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	quaternion( const quaternion< T > &q ) : w( q.w ), x( q.x ), y( q.y ), z( q.z ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] ww �c �����̐���
	//! @param[in] v  �c �����̐���
	//! 
	template < class TT >
	quaternion( value_type ww, const vector3< TT > &v ) : w( ww ), x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }


	/// @brief �N�H�[�^�j�I����p�����C�ӎ�����̉�]
	//! 
	//! @attention �E��n�̏ꍇ�͉E�˂���]�C����n�̏ꍇ�͍��˂���]�ƂȂ�̂Œ��ӁI�I
	//! 
	//! @param[in] axis  �c �����̐���
	//! @param[in] theta �c �����̐���
	//! 
	template < class TT >
	quaternion( const vector3< TT > &axis, value_type theta )
	{
		double t = theta * 3.1415926535897932384626433832795 / 180.0 / 2.0;
		double c = std::cos( t );
		double s = std::sin( t );
		w = static_cast< value_type >( c );
		x = static_cast< value_type >( s * axis.x );
		y = static_cast< value_type >( s * axis.y );
		z = static_cast< value_type >( s * axis.z );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion &operator =( const quaternion &q )
	{
		if( &q != this )
		{
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
		}
		return ( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion &operator +=( const quaternion &q )
	{
		w = w + q.w;
		x = x + q.x;
		y = y + q.y;
		z = z + q.z;
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
	const quaternion &operator +=( const value_type &a )
	{
		w += a;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion &operator -=( const quaternion &q )
	{
		w = w - q.w;
		x = x - q.x;
		y = y - q.y;
		z = z - q.z;
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
	const quaternion &operator -=( const value_type &a )
	{
		w -= a;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion &operator *=( const quaternion &q )
	{
		value_type ww = w * q.w - x * q.x - y * q.y - z * q.z;
		value_type xx = w * q.x + x * q.w + y * q.z - z * q.y;
		value_type yy = w * q.y + y * q.w + z * q.x - x * q.z;
		value_type zz = w * q.z + z * q.w + x * q.y - y * q.x;
		w = ww;
		x = xx;
		y = yy;
		z = zz;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion &operator *=( const value_type &a )
	{
		w *= a;
		x *= a;
		y *= a;
		z *= a;
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion &operator /=( const quaternion &q )
	{
		return( this->operator *=( q.inv( ) ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] a �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion &operator /=( const value_type &a )
	{
		w /= a;
		x /= a;
		y /= a;
		z /= a;
		return( *this );
	}


	/// @brief ��r�֐�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator ==( const quaternion &q ) const { return( w == q.w && x == q.x && y == q.y && z == q.z ); }

	/// @brief ��r�֐�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator !=( const quaternion &q ) const { return( !( *this == q ) ); }

	/// @brief ��r�֐�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator < ( const quaternion &q ) const { return( !( *this >= q ) ); }

	/// @brief ��r�֐�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator <=( const quaternion &q ) const { return( q >= *this ); }

	/// @brief ��r�֐�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator > ( const quaternion &q ) const { return( q < *this ); }

	/// @brief ��r�֐�
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] q �c �����̐���
	//! 
	//! @retval true  �c �߂�l�̐���
	//! @retval false �c �߂�l�̐���
	//! 
	bool operator >=( const quaternion &q ) const { return( w >= q.w && x >= q.x && y >= q.y && z >= q.z ); }


public:	// ���̑��̊֐�

	/// @brief �����N�H�[�^�j�I��
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion conjugate( ) const 
	{
		return( quaternion( w, -x, -y, -z ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion inv( ) const
	{
		value_type length_ = length( );
		return( conjugate( ) / ( length_ * length_ ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const quaternion unit( ) const
	{
		value_type length_ = length( );
		return quaternion( w / length_, x / length_, y / length_, z / length_ );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type length( ) const { return ( static_cast< value_type >( sqrt( (double)( w * w + x * x + y * y + z * z ) ) ) ); }


	/// @brief �x�N�g���̉�]
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] v �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	template < class TT >
	const vector3< TT > rotate( const vector3< TT > &v ) const
	{
		quaternion q = ( *this ) * quaternion( 0, static_cast< value_type >( v.x ), static_cast< value_type >( v.y ), static_cast< value_type >( v.z ) ) * inv( );
		return( vector3< TT >( static_cast< TT >( q.x ), static_cast< TT >( q.y ), static_cast< TT >( q.z ) ) );
	}
};

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q1 �c �����̐���
//! @param[in] q2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator +( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) += q2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q1 �c �����̐���
//! @param[in] q2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator -( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) -= q2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q1 �c �����̐���
//! @param[in] q2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator /( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) /= q2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q1 �c �����̐���
//! @param[in] q2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator *( const quaternion< T > &q1, const quaternion< T > &q2 ){ return( quaternion< T >( q1 ) *= q2 ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q �c �����̐���
//! @param[in] a �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator *( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) *= a ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] a �c �����̐���
//! @param[in] q �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator *( const typename quaternion< T >::value_type &a, const quaternion< T > &q ){ return( quaternion< T >( q ) *= a ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q �c �����̐���
//! @param[in] a �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator /( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) /= a ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q �c �����̐���
//! @param[in] a �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator +( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) += a ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] q �c �����̐���
//! @param[in] a �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const quaternion< T > operator -( const quaternion< T > &q, const typename quaternion< T >::value_type &a ){ return( quaternion< T >( q ) -= a ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     q   �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const quaternion< T > &q )
{
	out << "( ";
	out << q.w << ", ";
	out << q.x << ", ";
	out << q.y << ", ";
	out << q.z << " )";
	return( out );
}

// �N�H�[�^�j�I������s��֕ϊ�����
//template < class T >
//matrix< T > rotate_matrix( const quaternion< T > &q )
//{
//	matrix< T > mat(  4, 4  );
//	mat( 0, 0 ) = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
//	mat( 0, 1 ) = 2 * ( q.x * q.y - q.w * q.z );
//	mat( 0, 2 ) = 2 * ( q.x * q.z + q.w * q.y );
//	mat( 0, 3 ) = 0;
//	mat( 1, 0 ) = 2 * ( q.x * q.y + q.w * q.z );
//	mat( 1, 1 ) = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
//	mat( 1, 2 ) = 2 * ( q.y * q.z - q.w * q.x );
//	mat( 1, 3 ) = 0;
//	mat( 2, 0 ) = 2 * ( q.x * q.z - q.w * q.y );
//	mat( 2, 1 ) = 2 * ( q.y * q.z + q.w * q.x );
//	mat( 2, 2 ) = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
//	mat( 2, 3 ) = 0;
//	mat( 3, 0 ) = 0;
//	mat( 3, 1 ) = 0;
//	mat( 3, 2 ) = 0;
//	mat( 3, 3 ) = 1;
//	return( mat );
//}

// 
// 
//
//


/// @brief ���z�g���b�N�{�[���̎���(������W�n)
//! 
//! @note �ȉ��̃\�[�X�R�[�h���Q�l�ɂ���
//! 
//! Trackball code:
//! 
//! Implementation of a virtual trackball.
//! Implemented by Gavin Bell, lots of ideas from Thant Tessman and
//!   the August '88 issue of Siggraph's "Computer Graphics," pp. 121-129.
//! 
//! Vector manip code:
//! 
//! Original code from:
//! David M. Ciemiewicz, Mark Grossman, Henry Moreton, and Paul Haeberli
//! 
//! Much mucking with by:
//! Gavin Bell
//! 
//! @param[in] p1             �c �����̐���
//! @param[in] p2             �c �����̐���
//! @param[in] axisX          �c �����̐���
//! @param[in] axisY          �c �����̐���
//! @param[in] axisZ          �c �����̐���
//! @param[in] trackball_size �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T >
const quaternion< T > track_ball( const vector2< T > &p1, const vector2< T > &p2, const vector3< T > &axisX, const vector3< T > axisY, const vector3< T > axisZ,
									const typename vector3< T >::value_type &trackball_size = typename vector3< T >::value_type( 0.8 ) )
{
	typedef typename quaternion< T >::value_type value_type;

	if( p1 == p2 )
	{
		return( quaternion< T >( 1, 0, 0, 0 ) );
	}

	vector3< T > sp1( p1.x, p1.y, 0 ), sp2( p2.x, p2.y, 0 );
	value_type l, _2 = std::sqrt( value_type( 2.0 ) );

	// �_1�̍��W�����z�g���b�N�{�[����ɓ��e
	l = p1.length( );
	if( l < trackball_size / _2 )
	{
		sp1.z = - std::sqrt( trackball_size * trackball_size - l * l );
	}
	else
	{
		sp1.z = - trackball_size * trackball_size / 2.0 / l;
	}

	// �_2�̍��W�����z�g���b�N�{�[����ɓ��e
	l = p2.length( );
	if( l < trackball_size / _2 )
	{
		sp2.z = - std::sqrt( trackball_size * trackball_size - l * l );
	}
	else
	{
		sp2.z = - trackball_size * trackball_size / 2.0 / l;
	}

	//	sp1 = sp1.unit();
	//	sp2 = sp2.unit();

	//	Vector3<double> axis = (sp2 * sp1).unit();
	vector3< T > axis = ( sp1 * sp2 ).unit( );
	axis = ( axis.x * axisX + axis.y * axisY + axis.z * axisZ ).unit( );

	l = ( sp2 - sp1 ).length( ) / ( 2 * trackball_size );
	//	l = (l < -1.0)? -1.0: l;
	l = l > 1 ? 1: l;

	double phi = std::asin( l );
	//	fprintf(stdout, "axis(%.1f, %.1f, %.1f)   theta = %.1f\n", axis.x, axis.y, axis.z, phi * 180 / PAI);
	//	printf("%.1f\n", phi * 180 / PAI);
	return( quaternion< T >( std::cos( phi ), std::sin( phi ) * axis ) );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] x1             �c �����̐���
//! @param[in] y1             �c �����̐���
//! @param[in] x2             �c �����̐���
//! @param[in] y2             �c �����̐���
//! @param[in] axisX          �c �����̐���
//! @param[in] axisY          �c �����̐���
//! @param[in] axisZ          �c �����̐���
//! @param[in] trackball_size �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T >
const quaternion< T > track_ball( const typename vector3< T >::value_type &x1, const typename vector3< T >::value_type &y1, const typename vector3< T >::value_type &x2, const typename vector3< T >::value_type &y2,
									const vector3< T > &axisX, const vector3< T > &axisY, const vector3< T > &axisZ, const typename vector3< T >::value_type &trackball_size = typename vector3< T >::value_type( 0.8 ) )
{
	return( track_ball( vector2< T >( x1, y1 ), vector2< T >( x2, y2 ), axisX, axisY, axisZ, trackball_size ) );
}


// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_QUATERNION__
