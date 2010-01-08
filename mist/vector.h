// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/// @file mist/vector.h
//!
//! @brief �e�����ɑΉ������x�N�g�����Z���s�����߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_VECTOR__
#define __INCLUDE_MIST_VECTOR__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup vector_group �x�N�g�����C�u����
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/vector.h>
//! @endcode
//!
//!  @{



/// @brief 3�����x�N�g���������N���X
//! 
//! �x�N�g���̓��ρE�O�ϓ����ȕւɈ������߂̃N���X
//! 
//! @param T �c �x�N�g�����Ɋe���W��\���f�[�^�^
//! 
template < class T >
class vector3
{
public:
	typedef T value_type;										///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef size_t size_type;									///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;							///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename float_type< T >::value_type float_type;	///< @brief �����Ȃǂ��v�Z����Ƃ��ɗp���镂�������_�^

public:
	value_type x;		///< @brief X���W�l
	value_type y;		///< @brief Y���W�l
	value_type z;		///< @brief Z���W�l

	/// @brief �f�t�H���g�R���X�g���N�^�D( 0, 0, 0 ) �ɏ���������
	vector3( ) : x( 0 ), y( 0 ), z( 0 ){ }

	/// @brief ( xx, yy, zz ) �̃x�N�g�����쐬����
	vector3( const value_type &xx, const value_type &yy, const value_type &zz ) : x( xx ), y( yy ), z( zz ){ }

	/// @brief ( vv, vv, vv ) �̃x�N�g�����쐬����
	explicit vector3( const value_type &vv ) : x( vv ), y( vv ), z( vv ){ }


	/// @brief ����3�����x�N�g���ŗv�f�̌^���قȂ���̂��瓯���v�f�����x�N�g�����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT >
	vector3( const vector3< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }


	/// @brief ����3�����x�N�g���œ����v�f�^�̂��̂�p���ď���������
	vector3( const vector3< T > &v ) : x( v.x ), y( v.y ), z( v.z ){ }


	/// @brief ���f�[�^�^��3�����x�N�g����������
	//! 
	//! �R�s�[���ł���x�N�g�� v �ƑS�������x�N�g�����쐬����D
	//! 
	//! @param[in] v �c �R�s�[���̃x�N�g��
	//! 
	//! @return �������g
	//! 
	template < class TT >
	const vector3 &operator =( const vector3< TT > &v )
	{
		x = static_cast< value_type >( v.x );
		y = static_cast< value_type >( v.y );
		z = static_cast< value_type >( v.z );
		return ( *this );
	}

	/// @brief ����3�����x�N�g����������
	//! 
	//! �R�s�[���ł���x�N�g�� v �ƑS�������x�N�g�����쐬����D
	//! 
	//! @param[in] v �c �R�s�[���̃x�N�g��
	//! 
	//! @return �������g
	//! 
	const vector3 &operator =( const vector3< T > &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		return ( *this );
	}


	/// @brief �������]�����x�N�g����Ԃ�
	vector3 operator -( ) const { return ( vector3( -x, -y, -z ) ); }


	/// @brief �x�N�g���a
	template < class TT >
	vector3 &operator +=( const vector3< TT > &v )
	{
		x = static_cast< value_type >( x + v.x );
		y = static_cast< value_type >( y + v.y );
		z = static_cast< value_type >( z + v.z );
		return( *this );
	}

	/// @brief �x�N�g����
	template < class TT >
	vector3 &operator -=( const vector3< TT > &v )
	{
		x = static_cast< value_type >( x - v.x );
		y = static_cast< value_type >( y - v.y );
		z = static_cast< value_type >( z - v.z );
		return( *this );
	}

	/// @brief �x�N�g���̊O��
	template < class TT >
	vector3 &operator *=( const vector3< TT > &v )
	{
		value_type xx = static_cast< value_type >( y * v.z - z * v.y );
		value_type yy = static_cast< value_type >( z * v.x - x * v.z );
		value_type zz = static_cast< value_type >( x * v.y - y * v.x );
		x = xx;
		y = yy;
		z = zz;
		return( *this );
	}

	/// @brief �x�N�g���̒萔�{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector3 &operator *=( const double &a )
#else
	template < class TT >
	vector3 &operator *=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x * a );
		y = static_cast< value_type >( y * a );
		z = static_cast< value_type >( z * a );
		return( *this );
	}


	/// @brief �x�N�g����萔�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector3 &operator /=( const double &a )
#else
	template < class TT >
	vector3 &operator /=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x / a );
		y = static_cast< value_type >( y / a );
		z = static_cast< value_type >( z / a );
		return( *this );
	}


	/// @brief 2�̃x�N�g�����������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_x = q_x \; \wedge \; p_y = q_y \; \wedge \; p_z = q_z \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃x�N�g�����������ꍇ
	//! @retval false �c 2�̃x�N�g�����قȂ�ꍇ
	//! 
	bool operator ==( const vector3 &v ) const { return( x == v.x && y == v.y && z == v.z ); }

	/// @brief 2�̃x�N�g�����������Ȃ��i�ǂꂩ�P�ł��قȂ�v�f�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_x = q_x \; \wedge \; p_y = q_y \; \wedge \; p_z = q_z} \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃x�N�g�����قȂ�ꍇ
	//! @retval false �c 2�̃x�N�g�����������ꍇ
	//! 
	bool operator !=( const vector3 &v ) const { return( !( *this == v ) ); }

	/// @brief 2�̃x�N�g���� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z } \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 <  v2 �̏ꍇ
	//! @retval false �c v1 >= v2 �̏ꍇ
	//! 
	bool operator < ( const vector3 &v ) const
	{
		if( x == v.x )
		{
			if( y == v.y )
			{
				return( z < v.z );
			}
			else
			{
				return( y < v.y );
			}
		}
		else
		{
			return( x < v.x );
		}
	}

	/// @brief 2�̃x�N�g���� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 <= v2 �̏ꍇ
	//! @retval false �c v1 >  v2 �̏ꍇ
	//! 
	bool operator <=( const vector3 &v ) const { return( v >= *this ); }

	/// @brief 2�̃x�N�g���� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z } \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 >  v2 �̏ꍇ
	//! @retval false �c v1 <= v2 �̏ꍇ
	//! 
	bool operator > ( const vector3 &v ) const { return( v < *this ); }

	/// @brief 2�̃x�N�g���� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 >= v2 �̏ꍇ
	//! @retval false �c v1 <  v2 �̏ꍇ
	//! 
	bool operator >=( const vector3 &v ) const { return( !( *this < v ) ); }


	/// @brief �P�ʃx�N�g�����v�Z����
	//! 
	//! \f[ \frac{\mbox{\boldmath v}}{\left\|{\mbox{\boldmath v}}\right\|} \f]
	//! 
	vector3 unit( ) const
	{
		float_type length_ = length( );
		if( length_ > 0 )
		{
			return( vector3( static_cast< value_type >( x / length_ ), static_cast< value_type >( y / length_ ), static_cast< value_type >( z / length_ ) ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief �x�N�g���̓��ς��v�Z����
	//! 
	//! \f[ \mbox{\boldmath p} \cdot \mbox{\boldmath q} = p_x \times q_x + p_y \times q_y + p_z \times q_z \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	template < class TT >
	typename promote_trait< T, TT >::value_type inner( const vector3< TT > &v ) const
	{
		return( static_cast< typename promote_trait< T, TT >::value_type >( x * v.x + y * v.y + z * v.z ) );
	}


	/// @brief �x�N�g���̊O�ς��v�Z����
	//! 
	//! \f[ \mbox{\boldmath p} \times \mbox{\boldmath q} = \left( p_y \times q_z - p_z \times q_y \;,\; p_z \times q_x - p_x \times q_z \;,\; p_x \times q_y - p_y \times q_x \right)^T \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	template < class TT >
	vector3< typename promote_trait< value_type, TT >::value_type > outer( const vector3< TT > &v ) const
	{
		return( vector3< typename promote_trait< value_type, TT >::value_type >( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ) );
	}


	/// @brief �x�N�g���̑傫�����v�Z����
	//! 
	//! \f[ \left\|\mbox{\boldmath v}\right\| = \sqrt{v_x^2 + v_y^2 + v_z^2} \f]
	//! 
	//! @return �x�N�g���̑傫��
	//! 
	float_type length( ) const { return ( static_cast< float_type >( std::sqrt( static_cast< double >( x * x + y * y + z * z ) ) ) ); }

	/// @brief �C�ӎ�����̃x�N�g���̉�]
	//! 
	//! @param[in] v     �c ��]��
	//! @param[in] theta �c �E�˂������H�ő�������]�p�x
	//! 
	//! @return ��]��̃x�N�g��
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


/// @brief �x�N�g���̓���
template < class T1, class T2 >
inline typename promote_trait< T1, T2 >::value_type operator ^( const vector3< T1 > &v1, const vector3< T2 > &v2 )
{
	typedef typename promote_trait< T1, T2 >::value_type value_type;
	return( vector3< value_type >( v1 ).inner( v2 ) );
}

// �^�̏��i���s�����Z�̒�`

/// @brief �x�N�g���̘a
DEFINE_PROMOTE_BIND_OPERATOR1( vector3, + )

/// @brief �x�N�g���̍�
DEFINE_PROMOTE_BIND_OPERATOR1( vector3, - )

/// @brief �x�N�g���̊O��
DEFINE_PROMOTE_BIND_OPERATOR1( vector3, * )

/// @brief �x�N�g���ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( vector3, * )

/// @brief �萔�ƃx�N�g���̐�
DEFINE_PROMOTE_BIND_OPERATOR3( vector3, * )

/// @brief �x�N�g����萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( vector3, / )



/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     v   �c 3�����x�N�g��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2, 3 )
//! @endcode
//!
template < class T > inline std::ostream &operator <<( std::ostream &out, const vector3< T > &v )
{
	out << v.x << ", ";
	out << v.y << ", ";
	out << v.z;
	return( out );
}

/// @brief Method for reading the vector comma separated from an istream
//! 
//! @param[in,out] in �c input stream
//! @param[out]    v  �c 3-dimensional vector
//! 
//! @return the input stream
//! 
//! @code Marco Feuerstein
//! ( 1, 2, 3 )
//! @endcode
//!
template < class T > inline std::istream &operator >>( std::istream &in, vector3< T > &v )
{
	std::string comma;
	in >> v.x >> comma;
	in >> v.y >> comma;
	in >> v.z;
	return( in );
}



/// @brief 2�����x�N�g���������N���X
//! 
//! �x�N�g���̓��ρE�O�ϓ����ȕւɈ������߂̃N���X
//! 
//! @param T �c �x�N�g�����Ɋe���W��\���f�[�^�^
//! 
template < class T >
class vector2
{
public:
	typedef T value_type;										///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef size_t size_type;									///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;							///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename float_type< T >::value_type float_type;	///< @brief �����Ȃǂ��v�Z����Ƃ��ɗp���镂�������_�^

public:
	value_type x;		///< @brief X�����W�l
	value_type y;		///< @brief Y�����W�l

	/// @brief �f�t�H���g�R���X�g���N�^�D( 0, 0 ) �ɏ���������
	vector2( ) : x( 0 ), y( 0 ){ }

	/// @brief ( xx, yy ) �̃x�N�g�����쐬����
	vector2( const value_type &xx, const value_type &yy ) : x( xx ), y( yy ){ }

	/// @brief ( vv, vv ) �̃x�N�g�����쐬����
	vector2( const value_type &vv ) : x( vv ), y( vv ){ }


	/// @brief ����2�����x�N�g���ŗv�f�̌^���قȂ���̂��瓯���v�f�����x�N�g�����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT >
	vector2( const vector2< TT > &v ) : x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ){ }


	/// @brief ����2�����x�N�g���œ����v�f�^�̂��̂�p���ď���������
	vector2( const vector2< T > &v ) : x( v.x ), y( v.y ){ }


	/// @brief ���̃f�[�^�^��2�����x�N�g����������
	//! 
	//! �R�s�[���ł���x�N�g�� v �ƑS�������x�N�g�����쐬����D
	//! 
	//! @param[in] v �c �R�s�[���̃x�N�g��
	//! 
	//! @return �������g
	//! 
	template < class TT >
	const vector2 &operator =( const vector2< TT > &v )
	{
		x = static_cast< value_type >( v.x );
		y = static_cast< value_type >( v.y );
		return ( *this );
	}

	/// @brief ����2�����x�N�g����������
	//! 
	//! �R�s�[���ł���x�N�g�� v �ƑS�������x�N�g�����쐬����D
	//! 
	//! @param[in] v �c �R�s�[���̃x�N�g��
	//! 
	//! @return �������g
	//! 
	const vector2 &operator =( const vector2< T > &v )
	{
		if( &v != this )
		{
			x = v.x;
			y = v.y;
		}
		return ( *this );
	}


	/// @brief �������]�����x�N�g����Ԃ�
	vector2 operator -( ) const { return ( vector2( -x, -y ) ); }


	/// @brief �x�N�g���a
	template < class TT >
	vector2 &operator +=( const vector2< TT > &v ){ x = static_cast< value_type >( x + v.x ); y = static_cast< value_type >( y + v.y ); return( *this ); }

	/// @brief �x�N�g����
	template < class TT >
	vector2 &operator -=( const vector2< TT > &v ){ x = static_cast< value_type >( x - v.x ); y = static_cast< value_type >( y - v.y ); return( *this ); }


	/// @brief �x�N�g���̒萔�{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector2 &operator *=( const double &a )
#else
	template < class TT >
	vector2 &operator *=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x * a );
		y = static_cast< value_type >( y * a );
		return( *this );
	}


	/// @brief �x�N�g����萔�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	vector2 &operator /=( const double &a )
#else
	template < class TT >
	vector2 &operator /=( const TT &a )
#endif
	{
		x = static_cast< value_type >( x / a );
		y = static_cast< value_type >( y / a );
		return( *this );
	}


	/// @brief 2�̃x�N�g�����������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_x = q_x \; \wedge \; p_y = q_y \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃x�N�g�����������ꍇ
	//! @retval false �c 2�̃x�N�g�����قȂ�ꍇ
	//! 
	bool operator ==( const vector2 &v ) const { return( x == v.x && y == v.y ); }

	/// @brief 2�̃x�N�g�����������Ȃ��i�ǂꂩ�P�ł��قȂ�v�f�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_x = q_x \; \wedge \; p_y = q_y } \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃x�N�g�����قȂ�ꍇ
	//! @retval false �c 2�̃x�N�g�����������ꍇ
	//! 
	bool operator !=( const vector2 &v ) const { return( !( *this == v ) ); }

	/// @brief 2�̃x�N�g���� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_x \ge q_x \; \wedge \; p_y \ge q_y } \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 <  v2 �̏ꍇ
	//! @retval false �c v1 >= v2 �̏ꍇ
	//! 
	bool operator < ( const vector2 &v ) const
	{
		if( x == v.x )
		{
			return( y < v.y );
		}
		else
		{
			return( x < v.x );
		}
	}

	/// @brief 2�̃x�N�g���� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_x \le q_x \; \wedge \; p_y \le q_y \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 <= v2 �̏ꍇ
	//! @retval false �c v1 >  v2 �̏ꍇ
	//! 
	bool operator <=( const vector2 &v ) const { return( v >= *this ); }

	/// @brief 2�̃x�N�g���� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_x \le q_x \; \wedge \; p_y \le q_y } \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 >  v2 �̏ꍇ
	//! @retval false �c v1 <= v2 �̏ꍇ
	//! 
	bool operator > ( const vector2 &v ) const { return( v < *this ); }

	/// @brief 2�̃x�N�g���� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_x \ge q_x \; \wedge \; p_y \ge q_y \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @retval true  �c v1 >= v2 �̏ꍇ
	//! @retval false �c v1 <  v2 �̏ꍇ
	//! 
	bool operator >=( const vector2 &v ) const { return( !( *this < v ) ); }


	/// @brief �P�ʃx�N�g�����v�Z����
	//! 
	//! \f[ \frac{\mbox{\boldmath v}}{\left\|\mbox{\boldmath v}\right\|} \f]
	//! 
	vector2 unit( ) const
	{
		float_type length_ = length( );
		if( length_ > 0 )
		{
			return( vector2( static_cast< value_type >( x / length_ ), static_cast< value_type >( y / length_ ) ) );
		}
		else
		{
			return( *this );
		}
	}


	/// @brief �x�N�g���̓��ς��v�Z����
	//! 
	//! \f[ \mbox{\boldmath p} \cdot \mbox{\boldmath q} = p_x \times q_x + p_y \times q_y \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	template < class TT >
	typename promote_trait< T, TT >::value_type inner( const vector2< TT > &v ) const { return( static_cast< typename promote_trait< T, TT >::value_type >( x * v.x + y * v.y ) ); }


	/// @brief �x�N�g���̊O�ς��v�Z����
	//! 
	//! \f[ \mbox{\boldmath p} \times \mbox{\boldmath q} = p_x \times q_y - p_y \times q_x \f]
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	template < class TT >
	typename promote_trait< T, TT >::value_type outer( const vector2< TT > &v ) const { return( static_cast< typename promote_trait< T, TT >::value_type >( x * v.y - y * v.x ) ); }


	/// @brief �x�N�g���̑傫�����v�Z����
	//! 
	//! \f[ \left\|\mbox{\boldmath v}\right\| = \sqrt{v_x^2+v_y^2} \f]
	//! 
	//! @return �x�N�g���̑傫��
	//! 
	float_type length( ) const { return ( static_cast< float_type >( std::sqrt( static_cast< double >( x * x + y * y ) ) ) ); }

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


/// @brief �x�N�g���̓���
template < class T1, class T2 >
inline typename promote_trait< T1, T2 >::value_type operator ^( const vector2< T1 > &v1, const vector2< T2 > &v2 )
{
	typedef typename promote_trait< T1, T2 >::value_type value_type;
	return( vector2< value_type >( v1 ).inner( v2 ) );
}

/// @brief �x�N�g���̊O��
template < class T1, class T2 >
inline typename promote_trait< T1, T2 >::value_type operator *( const vector2< T1 > &v1, const vector2< T2 > &v2 )
{
	typedef typename promote_trait< T1, T2 >::value_type value_type;
	return( vector2< value_type >( v1 ).outer( v2 ) );
}


// �^�̏��i���s�����Z�̒�`
/// @brief �x�N�g���̘a
DEFINE_PROMOTE_BIND_OPERATOR1( vector2, + )

/// @brief �x�N�g���̍�
DEFINE_PROMOTE_BIND_OPERATOR1( vector2, - )

/// @brief �x�N�g���ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( vector2, * )

/// @brief �萔�ƃx�N�g���̐�
DEFINE_PROMOTE_BIND_OPERATOR3( vector2, * )

/// @brief �x�N�g����萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( vector2, / )


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     v   �c 2�����x�N�g��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2 )
//! @endcode
//!
template < class T > inline std::ostream &operator <<( std::ostream &out, const vector2< T > &v )
{
	out << v.x << ", ";
	out << v.y;
	return( out );
}

/// @brief Method for reading the vector comma separated from an istream
//! 
//! @param[in,out] in �c input stream
//! @param[out]    v  �c 2-dimensional vector
//! 
//! @return the input stream
//! 
//! @code Marco Feuerstein
//! ( 1, 2 )
//! @endcode
//!
template < class T > inline std::istream &operator >>( std::istream &in, vector2< T > &v )
{
	std::string comma;
	in >> v.x >> comma;
	in >> v.y;
	return( in );
}



#if defined( _MIST_VECTOR_SUPPORT_ ) && _MIST_VECTOR_SUPPORT_ != 0


/// @brief N�����x�N�g���������N���X
//! 
//! - _USE_EXPRESSION_TEMPLATE_  �}�N����1�̏ꍇ�́CExpression Template �𗘗p���āC�s��E�x�N�g�����Z������������
//! - _CHECK_MATRIX_OPERATION_   �}�N����1�̏ꍇ�́C�s��E�x�N�g�����Z�̍��ӂƉE�ӂŃx�N�g���̃T�C�Y���K�؂ł��邩���`�F�b�N����
//! - _CHECK_ACCESS_VIOLATION_   �}�N����1�̏ꍇ�́C�x�N�g���̗v�f�փA�N�Z�X����ۂɁC�͈͊O���ǂ����𔻒肷��
//! 
//! @param T         �c �s����Ɋi�[����f�[�^�^
//! @param Allocator �c MIST�R���e�i�����p����A���P�[�^�^�D�ȗ������ꍇ�́CSTL�̃f�t�H���g�A���P�[�^���g�p����
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

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef vector< TT, AAllocator > other;
	};


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


	/// @brief �x�N�g���̊O��
	//! 
	//! @attention �O�ς��v�Z����2�̃x�N�g���̎������������Ȃ��Ă͂Ȃ�Ȃ�
	//! 
	//! @param[in] v2 �c �E�Ӓl
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const vector& operator *=( const vector< TT, AAlocator > &v2 )
	{
#if defined( _CHECK_MATRIX_OPERATION_ ) && _CHECK_MATRIX_OPERATION_ != 0
		if( base::size( ) != v2.size( ) || base::size( ) < 3 )
		{
			// �O�ς̌v�Z���ł��܂����O
			::std::cerr << "can't calculate outer product of two vectors." << ::std::endl;
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


	/// @brief �x�N�g���̓���
	//! 
	//! @attention ���ς��v�Z����2�̃x�N�g���̎������������Ȃ��Ă͂Ȃ�Ȃ�
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @return ���ό��ʂ̃X�J���[�l
	//! 
	value_type operator ^( const vector &v ) const { return( inner( v ) ); }


	/// @brief �x�N�g���̓���
	//! 
	//! @attention ���ς��v�Z����2�̃x�N�g���̎������������Ȃ��Ă͂Ȃ�Ȃ�
	//! 
	//! @param[in] v2 �c �E�Ӓl
	//! 
	//! @return ���ό��ʂ̃X�J���[�l
	//! 
	value_type inner( const vector &v2 ) const
	{
#if defined( _CHECK_MATRIX_OPERATION_ ) && _CHECK_MATRIX_OPERATION_ != 0
		if( base::size( ) != v2.size( ) )
		{
			// ���ς̌v�Z���ł��܂����O
			::std::cerr << "can't calculate inner product of two vectors." << ::std::endl;
			return( value_type( 0 ) );
		}
#endif

		typedef typename vector< T, Allocator >::size_type size_type;
		const vector &v1 = *this;
		value_type v = value_type( 0 );
		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			v += v1[ i ] * v2[ i ];
		}

		return( v );
	}


	/// @brief �x�N�g���̊O��
	//! 
	//! @attention �O�ς��v�Z����2�̃x�N�g���̎������������Ȃ��Ă͂Ȃ�Ȃ�
	//! 
	//! @param[in] v �c �E�Ӓl
	//! 
	//! @return �������g
	//! 
	vector outer( const vector &v ) const
	{
		return( vector( *this ) *= v );
	}


public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D0 �����̃x�N�g�����쐬����
	vector( ) : base( ) {}

	/// @brief �A���P�[�^ a �̃R�s�[�𗘗p���C0 �����̃x�N�g�����쐬����
	explicit vector( const Allocator &a ) : base( a ) {}


	/// @brief num �����̃x�N�g�����쐬���C�f�t�H���g�l�ŗv�f������������
	vector( size_type num ) : base( num, 1 ) {}

	/// @brief num �����ŁC�A���P�[�^ a �̃R�s�[�𗘗p�����x�N�g�����쐬���C�f�t�H���g�l�ŗv�f������������
	vector( size_type num, const Allocator &a ) : base( num, 1, a ) {}


	/// @brief num �����̃x�N�g�����쐬���C�S�v�f��l val �ŗv�f������������
	vector( size_type num, const T &val ) : base( num, 1, val ) {}

	/// @brief num �����ŁC�A���P�[�^ a �̃R�s�[�𗘗p�����x�N�g�����쐬���C�S�v�f��l val �ŗv�f������������
	vector( size_type num, const T &val, const Allocator &a ) : base( num, 1, val, a ) {}


	/// @brief ptr ���w���������̈�ɁCnum �����̃x�N�g�����쐬���쐬����iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	vector( size_type num, pointer ptr, size_type mem_available ) : base( num, 1, ptr, mem_available ) {}

	/// @brief ptr ���w���������̈�ɁCnum �����̃x�N�g�����쐬���쐬���C�S�v�f��l val �ŗv�f������������iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	vector( size_type num, const T &val, pointer ptr, size_type mem_available ) : base( num, 1, val, ptr, mem_available ) {}


	/// @brief ���̃x�N�g���ŗv�f�̌^���قȂ���̂��瓯���T�C�Y�̃x�N�g�����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	vector( const vector< TT, AAlocator > &o ) : base( o ){ }


	/// @brief ���̃x�N�g���œ����v�f�^�̂��̂�p���ď���������
	vector( const vector< T, Allocator > &o ) : base( o ){ }

#if _USE_EXPRESSION_TEMPLATE_ != 0
	/// @brief �����̍s��E�x�N�g�����Z�̌��ʂ�p���ăx�N�g��������������
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



/// @brief �w�肳�ꂽ�X�g���[���ɁC�s����̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     v   �c �o�͂���s��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! @endcode
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

	return( out );
}

#if 0
#else

/// @brief �x�N�g���̊O�ς��v�Z����
//! 
//! @attention �O�ς��v�Z����2�̃x�N�g���̎������������Ȃ��Ă͂Ȃ�Ȃ�
//! 
//! @param[in] v1 �c ���Ӓl�x�N�g��
//! @param[in] v2 �c �E�Ӓl�x�N�g��
//! 
//! @return �O�ό���
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
