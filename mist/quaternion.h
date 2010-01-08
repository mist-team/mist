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

/// @file mist/quaternion.h
//!
//! @brief �N�H�[�^�j�I���i�l�����j���������߂̃��C�u����
//!
#ifndef __INCLUDE_MIST_QUATERNION__
#define __INCLUDE_MIST_QUATERNION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// @brief �N�H�[�^�j�I���i�l�����j�������N���X
//! 
//! @param T �c �����ŗp����f�[�^�^
//! 
template < class T >
class quaternion
{
public:
	typedef T value_type;										///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef size_t size_type;									///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;							///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename float_type< T >::value_type float_type;	///< @brief �����Ȃǂ��v�Z����Ƃ��ɗp���镂�������_�^

public:
	value_type w;		///< @brief ��������
	value_type x;		///< @brief ��������1
	value_type y;		///< @brief ��������2
	value_type z;		///< @brief ��������3

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�v�f��0�ŏ���������j
	quaternion( ) : w( 0 ), x( 0 ), y( 0 ), z( 0 ){ }

	/// @brief ww�Cxx�Cyy�Czz �̒l��p���ď���������
	quaternion( const value_type &ww, const value_type &xx, const value_type &yy, const value_type &zz ) : w( ww ), x( xx ), y( yy ), z( zz ){ }

	/// @brief ww�Cxx�Cyy�Czz �̒l��p���ď���������
	explicit quaternion( const value_type &ww ) : w( ww ), x( 0 ), y( 0 ), z( 0 ){ }

	/// @brief ���̃N�H�[�^�j�I���I�u�W�F�N�g�i�f�[�^�^���قȂ�j��p���ď���������
	template < class TT >
	quaternion( const quaternion< TT > &q ) : w( static_cast< value_type >( q.w ) ), x( static_cast< value_type >( q.x ) ), y( static_cast< value_type >( q.y ) ), z( static_cast< value_type >( q.z ) ){ }

	/// @brief ���̃N�H�[�^�j�I���I�u�W�F�N�g�i�f�[�^�^�������j��p���ď���������
	quaternion( const quaternion< T > &q ) : w( q.w ), x( q.x ), y( q.y ), z( q.z ){ }


	/// @brief �������� ww �Ƌ��������̃x�N�g�� v ��p���ď���������
	template < class TT >
	quaternion( value_type ww, const vector3< TT > &v ) : w( ww ), x( static_cast< value_type >( v.x ) ), y( static_cast< value_type >( v.y ) ), z( static_cast< value_type >( v.z ) ){ }


	/// @brief �N�H�[�^�j�I����p�����C�ӎ�����̉�]
	//! 
	//! @attention �E��n�̏ꍇ�͉E�˂���]�C����n�̏ꍇ�͍��˂���]�ƂȂ�̂Œ��ӁI�I
	//! @attention ��]�p�x�̒P�ʂ͓x��p����i���W�A���ł͂Ȃ��̂Œ��ӁI�I�j
	//! 
	//! @param[in] axis  �c ��]��
	//! @param[in] theta �c ��]�p�x
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


	/// @brief �J�����̎��������Ə����������p���āC�J�����̎p����\���N�H�[�^�j�I�����v�Z����
	//! 
	//! @param[in] dir �c ��]�O�̃x�N�g��
	//! @param[in] up  �c ��]��̃x�N�g��
	//! 
	template < class TT >
	quaternion( vector3< TT > dir, vector3< TT > up )
	{
		// ���[���h���W�̒P�ʃx�N�g��
		vector3< TT > e2( 0, 1, 0 );
		vector3< TT > e3( 0, 0, 1 );

		// �P�ʃx�N�g���ɂ���
		dir = dir.unit( );
		up  = up.unit( );

		// �������������킹��N�H�[�^�j�I�����쐬
		quaternion q1 = quaternion::rotate( e3, dir );

		// Y������]������
		e2 = q1.rotate( e2 );

		// ��������������킹��N�H�[�^�j�I�����쐬
		quaternion q2 = quaternion::rotate( e2, up, dir );

		// ��]����������
		operator =( q2 * q1 );
	}


	/// @brief ���̃N�H�[�^�j�I���I�u�W�F�N�g��������
	template < class TT >
	const quaternion &operator =( const quaternion< TT > &q )
	{
		w = static_cast< value_type >( q.w );
		x = static_cast< value_type >( q.x );
		y = static_cast< value_type >( q.y );
		z = static_cast< value_type >( q.z );
		return ( *this );
	}

	/// @brief ���̃N�H�[�^�j�I���I�u�W�F�N�g��������
	const quaternion &operator =( const quaternion< T > &q )
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

	/// @brief �������]�����N�H�[�^�j�I����Ԃ�
	quaternion operator -( ) const { return ( quaternion( -w, -x, -y, -z ) ); }

	/// @brief �N�H�[�^�j�I���̑����Z
	//! 
	//! \f[ \mbox{\boldmath p} + \mbox{\boldmath q} = \left( p_w + q_w \;,\; p_x + q_x \;,\; p_y + q_y \;,\; p_z + q_z \right)^T \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @return �����Z����
	//! 
	template< class TT >
	const quaternion &operator +=( const quaternion< TT > &q )
	{
		w = static_cast< value_type >( w + q.w );
		x = static_cast< value_type >( x + q.x );
		y = static_cast< value_type >( y + q.y );
		z = static_cast< value_type >( z + q.z );
		return( *this );
	}

	/// @brief �N�H�[�^�j�I���ւ̎��������̑����Z
	//! 
	//! \f[ \mbox{\boldmath p} + a = \left( p_w + a \;,\; p_x \;,\; p_y \;,\; p_z \right)^T \f]
	//! 
	//! @param[in] a �c ��������
	//! 
	//! @return �����Z����
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator +=( const double &a )
#else
	template < class TT >
	const quaternion &operator +=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w + a );
		return( *this );
	}

	/// @brief �N�H�[�^�j�I���̈����Z
	//! 
	//! \f[ \mbox{\boldmath p} - \mbox{\boldmath q} = \left( p_w - q_w \;,\; p_x - q_x \;,\; p_y - q_y \;,\; p_z - q_z \right)^T \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @return �����Z����
	//! 
	template< class TT >
	const quaternion &operator -=( const quaternion< TT > &q )
	{
		w = static_cast< value_type >( w - q.w );
		x = static_cast< value_type >( x - q.x );
		y = static_cast< value_type >( y - q.y );
		z = static_cast< value_type >( z - q.z );
		return( *this );
	}

	/// @brief �N�H�[�^�j�I���ւ̎��������̈����Z
	//! 
	//! \f[ \mbox{\boldmath p} - a = \left( p_w - a \;,\; p_x \;,\; p_y \;,\; p_z \right)^T \f]
	//! 
	//! @param[in] a �c ��������
	//! 
	//! @return �����Z����
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator -=( const double &a )
#else
	template < class TT >
	const quaternion &operator -=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w - a );
		return( *this );
	}

	/** @brief �N�H�[�^�j�I���̊|���Z
	 *  
	 *  \f[
	 *      \mbox{\boldmath p} \times \mbox{\boldmath q} =
	 *          \left(
	 *             p_w \times q_w - p_x \times q_x - p_y \times q_y - p_z \times q_z \;,\;
	 *             p_w \times q_x + p_x \times q_w + p_y \times q_z - p_z \times q_y \;,\;
	 *             p_w \times q_y + p_y \times q_w + p_z \times q_x - p_x \times q_z \;,\;
	 *             p_w \times q_z + p_z \times q_w + p_x \times q_y - p_y \times q_x
	 *          \right)^T
	 *  \f]
	 *  
	 *  @param[in] q �c �E�Ӓl
	 *  
	 *  @return �|���Z����
	 */
	template < class TT >
	const quaternion &operator *=( const quaternion< TT > &q )
	{
		value_type ww = static_cast< value_type >( w * q.w - x * q.x - y * q.y - z * q.z );
		value_type xx = static_cast< value_type >( w * q.x + x * q.w + y * q.z - z * q.y );
		value_type yy = static_cast< value_type >( w * q.y + y * q.w + z * q.x - x * q.z );
		value_type zz = static_cast< value_type >( w * q.z + z * q.w + x * q.y - y * q.x );
		w = ww;
		x = xx;
		y = yy;
		z = zz;
		return( *this );
	}

	/// @brief �N�H�[�^�j�I���ւ̎��������̊|���Z
	//! 
	//! \f[ \mbox{\boldmath p} \times a = \left( p_w \times a \;,\; p_x \times a \;,\; p_y \times a \;,\; p_z \times a \right)^T \f]
	//! 
	//! @param[in] a �c ��������
	//! 
	//! @return �|���Z����
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator *=( const double &a )
#else
	template < class TT >
	const quaternion &operator *=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w * a );
		x = static_cast< value_type >( x * a );
		y = static_cast< value_type >( y * a );
		z = static_cast< value_type >( z * a );
		return( *this );
	}

	/// @brief �N�H�[�^�j�I���̊���Z
	//! 
	//! \f[ \frac{ \mbox{\boldmath p} }{ \mbox{\boldmath q} } = \mbox{\boldmath p} \times \mbox{\boldmath q}^{-1} \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @return �|���Z����
	//! 
	template < class TT >
	const quaternion &operator /=( const quaternion< TT > &q )
	{
		return( this->operator *=( q.inv( ) ) );
	}

	/// @brief �N�H�[�^�j�I�������������Ŋ���
	//! 
	//! \f[ \mbox{\boldmath p} \div a = \left( p_w \div a \;,\; p_x \div a \;,\; p_y \div a \;,\; p_z \div a \right)^T \f]
	//! 
	//! @param[in] a �c ��������
	//! 
	//! @return �|���Z����
	//! 
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const quaternion &operator /=( const double &a )
#else
	template < class TT >
	const quaternion &operator /=( const TT &a )
#endif
	{
		w = static_cast< value_type >( w / a );
		x = static_cast< value_type >( x / a );
		y = static_cast< value_type >( y / a );
		z = static_cast< value_type >( z / a );
		return( *this );
	}


	/// @brief 2�̃N�H�[�^�j�I�������ꂩ�ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} == \mbox{\boldmath q} \rightarrow p_w == q_w \; \wedge \; p_x == q_x \; \wedge \; p_y == q_y \; \wedge \; p_z == q_z \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @retval true  �c �S�Ă̗v�f���������ꍇ
	//! @retval false �c �ǂꂩ1�ł��������Ȃ��ꍇ
	//! 
	bool operator ==( const quaternion &q ) const { return( w == q.w && x == q.x && y == q.y && z == q.z ); }

	/// @brief 2�̃N�H�[�^�j�I�����������Ȃ��ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_w = q_w \; \wedge \; p_x = q_x \; \wedge \; p_y = q_y \; \wedge \; p_z = q_z } \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @retval true  �c �ǂꂩ1�ł��������Ȃ��ꍇ
	//! @retval false �c �S�Ă̗v�f���������ꍇ
	//! 
	bool operator !=( const quaternion &q ) const { return( !( *this == q ) ); }

	/// @brief 2�̃x�N�g���� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} < \mbox{\boldmath q} \rightarrow \overline{ p_w \ge q_w \; \wedge \; p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z } \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @retval true  �c p <  q �̏ꍇ
	//! @retval false �c p >= q �̏ꍇ
	//! 
	bool operator < ( const quaternion &q ) const
	{
		if( w == q.w )
		{
			if( x == q.x )
			{
				if( y == q.y )
				{
					return( z < q.z );
				}
				else
				{
					return( y < q.y );
				}
			}
			else
			{
				return( x < q.x );
			}
		}
		else
		{
			return( w < q.w );
		}
	}

	/// @brief 2�̃x�N�g���� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_w \le q_w \; \wedge \; p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @retval true  �c p <= q �̏ꍇ
	//! @retval false �c p >  q �̏ꍇ
	//! 
	bool operator <=( const quaternion &q ) const { return( q >= *this ); }

	/// @brief 2�̃x�N�g���� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} > \mbox{\boldmath q} \rightarrow \overline{ p_w \le q_w \; \wedge \; p_x \le q_x \; \wedge \; p_y \le q_y \; \wedge \; p_z \le q_z } \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @retval true  �c p >  q �̏ꍇ
	//! @retval false �c p <= q �̏ꍇ
	//! 
	bool operator > ( const quaternion &q ) const { return( q < *this ); }

	/// @brief 2�̃x�N�g���� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_w \ge q_w \; \wedge \; p_x \ge q_x \; \wedge \; p_y \ge q_y \; \wedge \; p_z \ge q_z \f]
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! @retval true  �c p >= q �̏ꍇ
	//! @retval false �c p <  q �̏ꍇ
	//! 
	bool operator >=( const quaternion &q ) const { return( !( *this < q ) ); }


public:	// ���̑��̊֐�

	/// @brief �����N�H�[�^�j�I��
	//! 
	//! \f[ \overline{ \mbox{\boldmath p} } = \left( p_w \;,\; -p_x \;,\; -p_y \;,\; -p_z \right)^T \f]
	//! 
	const quaternion conjugate( ) const 
	{
		return( quaternion( w, -x, -y, -z ) );
	}

	/// @brief �t�N�H�[�^�j�I��
	//! 
	//! \f[ \mbox{\boldmath p}^{-1} = \frac{ \overline{ \mbox{\boldmath p} } }{ \left\| \mbox{\boldmath p} \right\|^2 } \f]
	//! 
	const quaternion inv( ) const
	{
		float_type length_ = length( );
		return( conjugate( ) / ( length_ * length_ ) );
	}

	/// @brief �P�ʃN�H�[�^�j�I��
	//! 
	//! \f[ \frac{ \mbox{\boldmath p} }{ \left\| \mbox{\boldmath p} \right\|^2 } \f]
	//! 
	const quaternion unit( ) const
	{
		float_type length_ = length( );
		if( length_ > 0 )
		{
			return( quaternion( static_cast< value_type >( w / length_ ), static_cast< value_type >( x / length_ ), static_cast< value_type >( y / length_ ), static_cast< value_type >( z / length_ ) ) );
		}
		else
		{
			return( *this );
		}
	}

	/// @brief �N�H�[�^�j�I���̓���
	//! 
	//! @param[in] q �c �E�Ӓl
	//! 
	//! \f[ p_w \times q_w + p_x \times q_x + p_y \times q_y + p_z \times q_z \f]
	//! 
	template < class TT >
	value_type inner( const quaternion< TT > &q ) const
	{
		return( w * q.w + x * q.x + y * q.y + z * q.z );
	}

	/// @brief �N�H�[�^�j�I���̃m����
	//! 
	//! \f[ \left\| \mbox{\boldmath p} \right\| = \sqrt{ p_w^2 + p_x^2 + p_y^2 + p_z^2 } \f]
	//! 
	float_type length( ) const { return ( static_cast< float_type >( std::sqrt( static_cast< double >( w * w + x * x + y * y + z * z ) ) ) ); }


	/// @brief �N�H�[�^�j�I����p�����x�N�g���̉�]
	//! 
	//! @param[in] v �c ��]�����x�N�g��
	//! 
	//! @return ��]��̃x�N�g��
	//! 
	template < class TT >
	const vector3< TT > rotate( const vector3< TT > &v ) const
	{
		quaternion q = ( *this ) * quaternion( 0, static_cast< value_type >( v.x ), static_cast< value_type >( v.y ), static_cast< value_type >( v.z ) ) * inv( );
		return( vector3< TT >( static_cast< TT >( q.x ), static_cast< TT >( q.y ), static_cast< TT >( q.z ) ) );
	}



	/// @brief �x�N�g��1����x�N�g��2�ւ̉�]��\���N�H�[�^�j�I�����쐬����
	//! 
	//! @param[in] v1 �c ��]�O�̃x�N�g��
	//! @param[in] v2 �c ��]��̃x�N�g��
	//! 
	//! @return ��]��\���N�H�[�^�j�I��
	//! 
	template < class TT >
	static quaternion rotate( vector3< TT > v1, vector3< TT > v2 )
	{
		// �P�ʃx�N�g���ɂ���
		v1 = v1.unit( );
		v2 = v2.unit( );

		// ��]�p�x���v�Z����
		double dot = v1.inner( v2 );
		if( dot < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		double c = std::sqrt( ( dot + 1.0 ) * 0.5 );

		if( std::abs( c - 1.0 ) < 1.0e-6 || c > 1.0 )
		{
			return( quaternion( 1, 0, 0, 0 ) );
		}
		else if( std::abs( c + 1.0 ) < 1.0e-6 || c < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		return( quaternion( value_type( c ), std::sqrt( 1.0 - c * c ) * v1.outer( v2 ).unit( ) ) );
	}

	/// @brief �w�肵����]����p���ăx�N�g��1����x�N�g��2�ւ̉�]��\���N�H�[�^�j�I�����쐬����
	//! 
	//! @param[in] v1   �c ��]�O�̃x�N�g��
	//! @param[in] v2   �c ��]��̃x�N�g��
	//! @param[in] axis �c ��]���x�N�g��
	//! 
	//! @return ��]��\���N�H�[�^�j�I��
	//! 
	template < class TT >
	static quaternion rotate( vector3< TT > v1, vector3< TT > v2, const vector3< TT > &axis )
	{
		// �P�ʃx�N�g���ɂ���
		v1 = v1.unit( );
		v2 = v2.unit( );

		// ��]�p�x���v�Z����
		double dot = v1.inner( v2 );
		if( dot < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		double c = std::sqrt( ( dot + 1.0 ) * 0.5 );

		if( std::abs( c - 1.0 ) < 1.0e-6 || c > 1.0 )
		{
			return( quaternion( 1, 0, 0, 0 ) );
		}
		else if( std::abs( c + 1.0 ) < 1.0e-6 || c < -1.0 )
		{
			return( quaternion( -1, 0, 0, 0 ) );
		}

		double s = std::sqrt( 1.0 - c * c );

		if( axis.inner( v1.outer( v2 ) ) < 0.0 )
		{
			s = -s;
		}

		return( quaternion( value_type( c ), s * axis ) );
	}
};

// �^�̏��i���s�����Z�̒�`

/// @brief �N�H�[�^�j�I���̘a
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, + )

/// @brief �N�H�[�^�j�I���ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, + )

/// @brief �萔�ƃN�H�[�^�j�I���̘a
DEFINE_PROMOTE_BIND_OPERATOR3( quaternion, + )

/// @brief �N�H�[�^�j�I���̍�
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, - )

/// @brief �N�H�[�^�j�I���ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, - )

/// @brief �萔�ƃN�H�[�^�j�I���̍�
DEFINE_PROMOTE_BIND_OPERATOR4( quaternion, - )

/// @brief �N�H�[�^�j�I���̐�
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, * )

/// @brief �N�H�[�^�j�I���ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, * )

/// @brief �萔�ƃN�H�[�^�j�I���̐�
DEFINE_PROMOTE_BIND_OPERATOR3( quaternion, * )

/// @brief �N�H�[�^�j�I���̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1( quaternion, / )

/// @brief �N�H�[�^�j�I����萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( quaternion, / )






/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     q   �c �N�H�[�^�j�I��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2, 3, 4 )
//! @endcode
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



/// @brief ���ʐ��`��Ԃ��s��
//! 
//! @param[in] q1 �c ��Ԃ��Ƃ̃N�H�[�^�j�I��1
//! @param[in] q2 �c ��Ԃ��Ƃ̃N�H�[�^�j�I��2
//! @param[in] t  �c [0,1]�̊Ԃ̐��l�ŁC��ԓ_
//! 
//! @return ���ʐ��`��Ԃ��ꂽ�N�H�[�^�j�I��
//! 
template < class T1, class T2 >
const quaternion< double > interpolate( const quaternion< T1 > &q1, const quaternion< T2 > &q2, double t )
{
	typedef quaternion< double > quaternion_type;

	quaternion_type Q1( q1.unit( ) );
	quaternion_type Q2( q2.unit( ) );

	double dot = Q1.inner( Q2 );

	if( std::abs( dot ) < 1.0e-6 )
	{
		return( Q1 );
	}
	else if( dot < 0.0 )
	{
		double theta = std::acos( dot );

		// ���ʐ��`��Ԃ��s��
		return( quaternion_type( Q1 * std::sin( theta * ( 1.0 - t ) ) - Q2 * std::sin( theta * t ) ).unit( ) );
	}
	else
	{
		double theta = std::acos( dot );

		// ���ʐ��`��Ԃ��s��
		return( quaternion_type( Q1 * std::sin( theta * ( 1.0 - t ) ) + Q2 * std::sin( theta * t ) ).unit( ) );
	}
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
//! @param[in] p1             �c ��]�O�̓_
//! @param[in] p2             �c ��]��̓_
//! @param[in] axisX          �c �g���b�N�{�[����X��
//! @param[in] axisY          �c �g���b�N�{�[����Y��
//! @param[in] axisZ          �c �g���b�N�{�[����Z��
//! @param[in] trackball_size �c �g���b�N�{�[���̔��a�i�f�t�H���g��0.8�j
//! 
//! @return ��]��\���N�H�[�^�j�I��
//! 
template < class T >
const quaternion< T > track_ball( const vector2< T > &p1, const vector2< T > &p2, const vector3< T > &axisX, const vector3< T > axisY, const vector3< T > axisZ, const typename vector3< T >::value_type &trackball_size )
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

	// �E��n�ƍ���n�ł����̊O�ς̌����𔽓]������
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
//! @param[in] p1             �c ��]�O�̓_
//! @param[in] p2             �c ��]��̓_
//! @param[in] axisX          �c �g���b�N�{�[����X��
//! @param[in] axisY          �c �g���b�N�{�[����Y��
//! @param[in] axisZ          �c �g���b�N�{�[����Z��
//! 
//! @return ��]��\���N�H�[�^�j�I��
//! 
template < class T >
inline const quaternion< T > track_ball( const vector2< T > &p1, const vector2< T > &p2, const vector3< T > &axisX, const vector3< T > axisY, const vector3< T > axisZ )
{
	return( track_ball( p1, p2, axisX, axisY, axisZ, 0.8 ) );
}


/// @brief ���z�g���b�N�{�[���̎���(������W�n)
//! 
//! �g���b�N�{�[����p���āC�C�Ӄx�N�g���̉�]���s��
//! 
//! @param[in] x1             �c ��]�O��X���W
//! @param[in] y1             �c ��]�O��Y���W
//! @param[in] x2             �c ��]���X���W
//! @param[in] y2             �c ��]���Y���W
//! @param[in] axisX          �c �g���b�N�{�[����X��
//! @param[in] axisY          �c �g���b�N�{�[����Y��
//! @param[in] axisZ          �c �g���b�N�{�[����Z��
//! @param[in] trackball_size �c �g���b�N�{�[���̔��a�i�f�t�H���g��0.8�j
//! 
//! @return ��]��\���N�H�[�^�j�I��
//! 
template < class T >
const quaternion< T > track_ball( const typename vector3< T >::value_type &x1, const typename vector3< T >::value_type &y1, const typename vector3< T >::value_type &x2, const typename vector3< T >::value_type &y2,
												const vector3< T > &axisX, const vector3< T > &axisY, const vector3< T > &axisZ, const typename vector3< T >::value_type &trackball_size )
{
	return( track_ball( vector2< T >( x1, y1 ), vector2< T >( x2, y2 ), axisX, axisY, axisZ, trackball_size ) );
}


/// @brief ���z�g���b�N�{�[���̎���(������W�n)
//! 
//! �g���b�N�{�[����p���āC�C�Ӄx�N�g���̉�]���s��
//! 
//! @param[in] x1             �c ��]�O��X���W
//! @param[in] y1             �c ��]�O��Y���W
//! @param[in] x2             �c ��]���X���W
//! @param[in] y2             �c ��]���Y���W
//! @param[in] axisX          �c �g���b�N�{�[����X��
//! @param[in] axisY          �c �g���b�N�{�[����Y��
//! @param[in] axisZ          �c �g���b�N�{�[����Z��
//! 
//! @return ��]��\���N�H�[�^�j�I��
//! 
template < class T >
const quaternion< T > track_ball( const typename vector3< T >::value_type &x1, const typename vector3< T >::value_type &y1, const typename vector3< T >::value_type &x2,
											const typename vector3< T >::value_type &y2, const vector3< T > &axisX, const vector3< T > &axisY, const vector3< T > &axisZ )
{
	return( track_ball( vector2< T >( x1, y1 ), vector2< T >( x2, y2 ), axisX, axisY, axisZ, 0.8 ) );
}


// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_QUATERNION__
