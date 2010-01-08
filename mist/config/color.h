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

/// @file mist/config/color.h
//!
//! @brief �J���[�f�[�^���������߂̃N���X�Q
//!

#ifndef __INCLUDE_MIST_COLOR_H__
#define __INCLUDE_MIST_COLOR_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "type_trait.h"
#endif

#include <iostream>
#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

namespace __color_utility__
{
	template < bool >
	struct __normalized_color__
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &R, T &G, T &B )
		{
			T sum = r + g + b;

			sum = sum > 0 ? sum : 1;

			R = r / sum;
			G = g / sum;
			B = b / sum;
		}
	};

	template < >
	struct __normalized_color__< false >
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &R, T &G, T &B )
		{
			double sum = static_cast< double >( r ) + static_cast< double >( g ) + static_cast< double >( b );

			sum = sum > 0.0 ? sum : 1.0;

			R = static_cast< T >( r * 255.0 / sum + 0.5 );
			G = static_cast< T >( g * 255.0 / sum + 0.5 );
			B = static_cast< T >( b * 255.0 / sum + 0.5 );
		}
	};

	template < class T > inline T maximum( const T &v1, const T &v2 )
	{
		return( v1 > v2 ? v1 : v2 );
	}

	template < class T > inline T minimum( const T &v1, const T &v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}
}

// MIST�ŗ��p������̃f�[�^�^
template < class T > struct bgr;


//! @addtogroup color_group MIST�S�ʂŗ��p�\�ȐF��\������N���X
//!
//! @code ���̃w�b�_���Q��
//! #include <mist/config/color.h>
//! @endcode
//!
//! @{


/// @brief �J���[�摜�p�̉�f
//! 
//! @code �J���[�摜�̍쐬��
//! mist::array2< mist::rgb< unsigned char > > image;
//! @endcode
//! 
//! @param T �c �e�F�����̃f�[�^�^
//! 
template< class T >
struct rgb
{
public:
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef T& reference;					///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const T& const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef T value_type;					///< @brief �����f�[�^�^�DT �Ɠ���
	typedef T* pointer;						///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef const T* const_pointer;			///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT > 
	struct rebind
	{
		typedef rgb< TT > other;
	};

public:
	value_type r;		///< @brief �ԐF����
	value_type g;		///< @brief �ΐF����
	value_type b;		///< @brief �F����

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�Ă̗v�f��0�ŏ���������j
	rgb( ) : r( 0 ), g( 0 ), b( 0 ){ }

	/// @brief �S�Ă̐����� pix �ŏ���������
	explicit rgb( const value_type &pix ) : r( pix ), g( pix ), b( pix ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	rgb( const rgb< TT > &c ) : r( static_cast< value_type >( c.r ) ), g( static_cast< value_type >( c.g ) ), b( static_cast< value_type >( c.b ) ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	rgb( const bgr< TT > &c ) : r( static_cast< value_type >( c.r ) ), g( static_cast< value_type >( c.g ) ), b( static_cast< value_type >( c.b ) ){ }

	/// @brief ���̃J���[��f��p���ď���������
	rgb( const rgb< T > &c ) : r( c.r ), g( c.g ), b( c.b ){ }

	/// @brief �� rr�C�� gg�C�� bb ��p���ď���������
	rgb( const value_type &rr, const value_type &gg, const value_type &bb ) : r( rr ), g( gg ), b( bb ){ }


	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const rgb &operator =( const rgb< TT > &c )
	{
		r = static_cast< value_type >( c.r );
		g = static_cast< value_type >( c.g );
		b = static_cast< value_type >( c.b );
		return( *this );
	}

	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const rgb &operator =( const bgr< TT > &c )
	{
		r = static_cast< value_type >( c.r );
		g = static_cast< value_type >( c.g );
		b = static_cast< value_type >( c.b );
		return( *this );
	}

	/// @brief ���̃J���[��f��������
	const rgb &operator =( const rgb< T > &c )
	{
		if( &c != this )
		{
			r = c.r;
			g = c.g;
			b = c.b;
		}
		return( *this );
	}

	/// @brief �S�Ă̗v�f�� pix ��������
	const rgb &operator =( const value_type &pix )
	{
		r = pix;
		g = pix;
		b = pix;
		return( *this );
	}


	/// @brief �S�v�f�̕������]
	const rgb  operator -( ) const { return( rgb( -r, -g, -b ) ); }

	/// @brief RGB�����̘a
	template < class TT >
	const rgb &operator +=( const rgb< TT > &c ){ r = static_cast< value_type >( r + c.r ); g = static_cast< value_type >( g + c.g ); b = static_cast< value_type >( b + c.b ); return( *this ); }

	/// @brief RGB�����̍�
	template < class TT >
	const rgb &operator -=( const rgb< TT > &c ){ r = static_cast< value_type >( r - c.r ); g = static_cast< value_type >( g - c.g ); b = static_cast< value_type >( b - c.b ); return( *this ); }

	/// @brief RGB�����̐�
	template < class TT >
	const rgb &operator *=( const rgb< TT > &c ){ r = static_cast< value_type >( r * c.r ); g = static_cast< value_type >( g * c.g ); b = static_cast< value_type >( b * c.b ); return( *this ); }

	/// @brief RGB�����̊���Z
	template < class TT >
	const rgb &operator /=( const rgb< TT > &c ){ r = static_cast< value_type >( r / c.r ); g = static_cast< value_type >( g / c.g ); b = static_cast< value_type >( b / c.b ); return( *this ); }

	/// @brief RGB�����̏�]
	const rgb &operator %=( const rgb &c ){ r %= c.r; g %= c.g; b %= c.b; return( *this ); }

	/// @brief RGB������ | ���Z
	const rgb &operator |=( const rgb &c ){ r |= c.r; g |= c.g; b |= c.b; return( *this ); }

	/// @brief RGB������ & ���Z
	const rgb &operator &=( const rgb &c ){ r &= c.r; g &= c.g; b &= c.b; return( *this ); }

	/// @brief RGB������ ^ ���Z
	const rgb &operator ^=( const rgb &c ){ r ^= c.r; g ^= c.g; b ^= c.b; return( *this ); }


	/// @brief RGB������ pix �l�𑫂�
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator +=( const double &pix )
#else
	template < class TT >
	const rgb &operator +=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r + pix );
		g = static_cast< value_type >( g + pix );
		b = static_cast< value_type >( b + pix );
		return( *this );
	}

	/// @brief RGB�������� pix �l������
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator -=( const double &pix )
#else
	template < class TT >
	const rgb &operator -=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r - pix );
		g = static_cast< value_type >( g - pix );
		b = static_cast< value_type >( b - pix );
		return( *this );
	}

	/// @brief RGB������ pix �l���|����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator *=( const double &pix )
#else
	template < class TT >
	const rgb &operator *=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r * pix );
		g = static_cast< value_type >( g * pix );
		b = static_cast< value_type >( b * pix );
		return( *this );
	}

	/// @brief RGB������ pix �l�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgb &operator /=( const double &pix )
#else
	template < class TT >
	const rgb &operator /=( const TT &pix )
#endif
	{
		r = static_cast< value_type >( r / pix );
		g = static_cast< value_type >( g / pix );
		b = static_cast< value_type >( b / pix );
		return( *this );
	}


	/// @brief 2�̃J���[��f���������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && b == c.b ); }

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const rgb &c ) const
	{
		if( r == c.r )
		{
			if( g == c.g )
			{
				return( b < c.b );
			}
			else
			{
				return( g < c.g );
			}
		}
		else
		{
			return( r < c.r );
		}
	}

	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const rgb &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const rgb &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const rgb &c ) const { return( !( *this < c ) ); }


	/// @brief NTSC�n���d���ϖ@�ɂ��C�O���[�X�P�[���֕ϊ�����
	value_type get_value( ) const
	{
		return( half_adjust< value_type >::convert( r * 0.298912 + g * 0.586610 + b * 0.114478 ) );
	}

	// �J���[����O���[�X�P�[���ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator value_type( ) const { return( get_value( ) ); }
};



/// @brief �J���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, + )

/// @brief �J���[��f�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, + )

/// @brief �萔�ƃJ���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, + )

/// @brief �J���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, - )

/// @brief �J���[��f�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, - )

/// @brief �萔�ƃJ���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR4( rgb, - )

/// @brief �J���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, * )

/// @brief �J���[��f�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, * )

/// @brief �萔�ƃJ���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, * )

/// @brief �J���[��f�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, / )

/// @brief �J���[��f��萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, / )

/// @brief �J���[��f�̏�]
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, % )


/// @brief �J���[��f�� | ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, | )

/// @brief �J���[��f�� & ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, & )

/// @brief �J���[��f�� ^ ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgb, ^ )




/// @brief BGR�̏��Ńf�[�^�����ԃJ���[�摜�p�̉�f
//! 
//! ���Windows�̃r�b�g�}�b�v�ŗ��p����Ă���
//! 
//! @code �J���[�摜�̍쐬��
//! mist::array2< mist::bgr< unsigned char > > image;
//! @endcode
//! 
//! @param T �c �e�F�����̃f�[�^�^
//! 
template< class T >
struct bgr
{
public:
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef T& reference;					///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const T& const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef T value_type;					///< @brief �����f�[�^�^�DT �Ɠ���
	typedef T* pointer;						///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef const T* const_pointer;			///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT > 
	struct rebind
	{
		typedef bgr< TT > other;
	};

public:
	value_type b;		///< @brief �F����
	value_type g;		///< @brief �ΐF����
	value_type r;		///< @brief �ԐF����

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�Ă̗v�f��0�ŏ���������j
	bgr( ) : b( 0 ), g( 0 ), r( 0 ){ }

	/// @brief �S�Ă̐����� pix �ŏ���������
	explicit bgr( const value_type &pix ) : b( pix ), g( pix ), r( pix ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	bgr( const bgr< TT > &c ) : b( static_cast< value_type >( c.b ) ), g( static_cast< value_type >( c.g ) ), r( static_cast< value_type >( c.r ) ){ }

	/// @brief ���̃J���[��f��p���ď���������
	bgr( const bgr< T > &c ) : b( c.b ), g( c.g ), r( c.r ){ }

	/// @brief �� rr�C�� gg�C�� bb ��p���ď���������
	bgr( const value_type &rr, const value_type &gg, const value_type &bb ) : b( bb ), g( gg ), r( rr ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	bgr( const rgb< TT > &c ) : b( static_cast< value_type >( c.b ) ), g( static_cast< value_type >( c.g ) ), r( static_cast< value_type >( c.r ) ){ }


	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const bgr &operator =( const bgr< TT > &c )
	{
		b = static_cast< value_type >( c.b );
		g = static_cast< value_type >( c.g );
		r = static_cast< value_type >( c.r );
		return( *this );
	}

	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const bgr &operator =( const rgb< TT > &c )
	{
		b = static_cast< value_type >( c.b );
		g = static_cast< value_type >( c.g );
		r = static_cast< value_type >( c.r );
		return( *this );
	}

	/// @brief ���̃J���[��f��������
	const bgr &operator =( const bgr< T > &c )
	{
		if( &c != this )
		{
			b = c.b;
			g = c.g;
			r = c.r;
		}
		return( *this );
	}

	/// @brief �S�Ă̗v�f�� pix ��������
	const bgr &operator =( const value_type &pix )
	{
		b = pix;
		g = pix;
		r = pix;
		return( *this );
	}


	/// @brief �S�v�f�̕������]
	const bgr  operator -( ) const { return( bgr( -r, -g, -b ) ); }

	/// @brief RGB�����̘a
	template < class TT >
	const bgr &operator +=( const bgr< TT > &c ){ b = static_cast< value_type >( b + c.b ); g = static_cast< value_type >( g + c.g ); r = static_cast< value_type >( r + c.r ); return( *this ); }

	/// @brief RGB�����̍�
	template < class TT >
	const bgr &operator -=( const bgr< TT > &c ){ b = static_cast< value_type >( b - c.b ); g = static_cast< value_type >( g - c.g ); r = static_cast< value_type >( r - c.r ); return( *this ); }

	/// @brief RGB�����̐�
	template < class TT >
	const bgr &operator *=( const bgr< TT > &c ){ b = static_cast< value_type >( b * c.b ); g = static_cast< value_type >( g * c.g ); r = static_cast< value_type >( r * c.r ); return( *this ); }

	/// @brief RGB�����̊���Z
	template < class TT >
	const bgr &operator /=( const bgr< TT > &c ){ b = static_cast< value_type >( b / c.b ); g = static_cast< value_type >( g / c.g ); r = static_cast< value_type >( r / c.r ); return( *this ); }

	/// @brief RGB�����̏�]
	const bgr &operator %=( const bgr &c ){ b %= c.b; g %= c.g; r %= c.r; return( *this ); }

	/// @brief RGB������ | ���Z
	const bgr &operator |=( const bgr &c ){ b |= c.b; g |= c.g; r |= c.r; return( *this ); }

	/// @brief RGB������ & ���Z
	const bgr &operator &=( const bgr &c ){ b &= c.b; g &= c.g; r &= c.r; return( *this ); }

	/// @brief RGB������ ^ ���Z
	const bgr &operator ^=( const bgr &c ){ b ^= c.b; g ^= c.g; r ^= c.r; return( *this ); }


	/// @brief RGB������ pix �l�𑫂�
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator +=( const double &pix )
#else
	template < class TT >
	const bgr &operator +=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b + pix );
		g = static_cast< value_type >( g + pix );
		r = static_cast< value_type >( r + pix );
		return( *this );
	}

	/// @brief RGB�������� pix �l������
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator -=( const double &pix )
#else
	template < class TT >
	const bgr &operator -=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b - pix );
		g = static_cast< value_type >( g - pix );
		r = static_cast< value_type >( r - pix );
		return( *this );
	}

	/// @brief RGB������ pix �l���|����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator *=( const double &pix )
#else
	template < class TT >
	const bgr &operator *=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b * pix );
		g = static_cast< value_type >( g * pix );
		r = static_cast< value_type >( r * pix );
		return( *this );
	}

	/// @brief RGB������ pix �l�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgr &operator /=( const double &pix )
#else
	template < class TT >
	const bgr &operator /=( const TT &pix )
#endif
	{
		b = static_cast< value_type >( b / pix );
		g = static_cast< value_type >( g / pix );
		r = static_cast< value_type >( r / pix );
		return( *this );
	}


	/// @brief 2�̃J���[��f���������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const bgr &c ) const { return( b == c.b && g == c.g && r == c.r ); }

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const bgr &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const bgr &c ) const
	{
		if( r == c.r )
		{
			if( g == c.g )
			{
				return( b < c.b );
			}
			else
			{
				return( g < c.g );
			}
		}
		else
		{
			return( r < c.r );
		}
	}

	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const bgr &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const bgr &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const bgr &c ) const { return( !( *this < c ) ); }


	/// @brief NTSC�n���d���ϖ@�ɂ��C�O���[�X�P�[���֕ϊ�����
	value_type get_value( ) const
	{
		return( static_cast< value_type >( b * 0.114478 + g * 0.586610 + r * 0.298912 ) );
	}

	// �J���[����O���[�X�P�[���ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator value_type( ) const { return( get_value( ) ); }

};

/// @brief �J���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, + )

/// @brief �J���[��f�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, + )

/// @brief �萔�ƃJ���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR3( bgr, + )

/// @brief �J���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, - )

/// @brief �J���[��f�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, - )

/// @brief �萔�ƃJ���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR4( bgr, - )

/// @brief �J���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, * )

/// @brief �J���[��f�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, * )

/// @brief �萔�ƃJ���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR3( bgr, * )

/// @brief �J���[��f�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, / )

/// @brief �J���[��f��萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( bgr, / )

/// @brief �J���[��f�̏�]
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, % )


/// @brief �J���[��f�� | ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, | )

/// @brief �J���[��f�� & ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, & )

/// @brief �J���[��f�� ^ ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgr, ^ )




/// @brief �A���t�@�`�����l���t���J���[�摜�p�̉�f
//! 
//! @code �A���t�@�`�����l���t���J���[�摜�̍쐬��
//! mist::array2< mist::rgba< unsigned char > > image;
//! @endcode
//! 
//! @param T �c �e�F�����̃f�[�^�^
//! 
template< class T >
struct rgba : public rgb< T >
{
protected:
	typedef rgb< T > base;

public:
	typedef typename base::size_type		size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type	difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename base::reference		reference;				///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef typename base::const_reference	const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename base::value_type		value_type;				///< @brief �����f�[�^�^�DT �Ɠ���
	typedef typename base::pointer			pointer;				///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef typename base::const_pointer	const_pointer;			///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT > 
	struct rebind
	{
		typedef rgba< TT > other;
	};

public:
	value_type a;		///< @brief �A���t�@����

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�Ă̗v�f��0�ŏ���������j
	rgba( ) : base( ), a( 255 ){ }

	/// @brief �S�Ă̐����� pix �ŏ���������
	explicit rgba( const value_type &pix ) : base( pix ), a( 255 ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	rgba( const rgba< TT > &c ) : base( c ), a( static_cast< value_type >( c.a ) ){ }

	/// @brief ���̃J���[��f��p���ď���������
	rgba( const rgba< T > &c ) : base( c ), a( c.a ){ }


	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	rgba( const rgb< TT > &c ) : base( c ), a( 255 ){ }

	/// @brief ���̃J���[��f��p���ď���������
	rgba( const rgb< T > &c ) : base( c ), a( 255 ){ }


	/// @brief �� rr�C�� gg�C�� bb ��p���ď���������
	rgba( const value_type &rr, const value_type &gg, const value_type &bb, const value_type &aa = 255 ) : base( rr, gg, bb ), a( aa ){ }


	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const rgba &operator =( const rgba< TT > &c )
	{
		base::operator =( c );
		a = static_cast< value_type >( c.a );
		return( *this );
	}

	/// @brief ���̃J���[��f��������
	const rgba &operator =( const rgba< T > &c )
	{
		if( &c != this )
		{
			base::operator =( c );
			a = c.a;
		}
		return( *this );
	}

	/// @brief �S�Ă̗v�f�� pix ��������
	const rgba &operator =( const value_type &pix )
	{
		base::operator =( pix );
		return( *this );
	}


	/// @brief �S�v�f�̕������]
	const rgba  operator -( ) const { return( rgba( -base::r, -base::g, -base::b, a ) ); }

	/// @brief RGB�����̘a
	template < class TT >
	const rgba &operator +=( const rgba< TT > &c ){ base::operator +=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̍�
	template < class TT >
	const rgba &operator -=( const rgba< TT > &c ){ base::operator -=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̐�
	template < class TT >
	const rgba &operator *=( const rgba< TT > &c ){ base::operator *=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̊���Z
	template < class TT >
	const rgba &operator /=( const rgba< TT > &c ){ base::operator /=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̏�]
	const rgba &operator %=( const rgba &c ){ base::operator %=( ( const base &)c ); return( *this ); }

	/// @brief RGB������ | ���Z
	const rgba &operator |=( const rgba &c ){ base::operator |=( ( const base &)c ); return( *this ); }

	/// @brief RGB������ & ���Z
	const rgba &operator &=( const rgba &c ){ base::operator &=( ( const base &)c ); return( *this ); }

	/// @brief RGB������ ^ ���Z
	const rgba &operator ^=( const rgba &c ){ base::operator ^=( ( const base &)c ); return( *this ); }


	/// @brief RGB������ pix �l�𑫂�
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator +=( const double &pix )
#else
	template < class TT >
	const rgba &operator +=( const TT &pix )
#endif
	{
		base::operator +=( pix );
		return( *this );
	}

	/// @brief RGB�������� pix �l������
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator -=( const double &pix )
#else
	template < class TT >
	const rgba &operator -=( const TT &pix )
#endif
	{
		base::operator -=( pix );
		return( *this );
	}

	/// @brief RGB������ pix �l���|����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator *=( const double &pix )
#else
	template < class TT >
	const rgba &operator *=( const TT &pix )
#endif
	{
		base::operator *=( pix );
		return( *this );
	}

	/// @brief RGB������ pix �l�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const rgba &operator /=( const double &pix )
#else
	template < class TT >
	const rgba &operator /=( const TT &pix )
#endif
	{
		base::operator /=( pix );
		return( *this );
	}


	/// @brief 2�̃J���[��f���������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const rgba &c ) const { return( base::operator ==( c ) ); }

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const rgba &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const rgba &c ) const
	{
		if( a == c.a )
		{
			return( base::operator <( c ) );
		}
		else
		{
			return( a < c.a );
		}
	}

	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const rgba &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const rgba &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const rgba &c ) const { return( !( *this < c ) ); }

};

/// @brief �J���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, + )

/// @brief �J���[��f�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, + )

/// @brief �萔�ƃJ���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR3( rgba, + )

/// @brief �J���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, - )

/// @brief �J���[��f�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, - )

/// @brief �萔�ƃJ���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR4( rgba, - )

/// @brief �J���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, * )

/// @brief �J���[��f�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, * )

/// @brief �萔�ƃJ���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR3( rgba, * )

/// @brief �J���[��f�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, / )

/// @brief �J���[��f��萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( rgba, / )

/// @brief �J���[��f�̏�]
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, % )


/// @brief �J���[��f�� | ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, | )

/// @brief �J���[��f�� & ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, & )

/// @brief �J���[��f�� ^ ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( rgba, ^ )









/// @brief BGRA�̏��Ńf�[�^�����ԃJ���[�摜�p�̉�f
//! 
//! ���Windows�̃r�b�g�}�b�v�ŗ��p����Ă���
//! 
//! @code �J���[�摜�̍쐬��
//! mist::array2< mist::bgra< unsigned char > > image;
//! @endcode
//! 
//! @param T �c �e�F�����̃f�[�^�^
//! 
template< class T >
struct bgra : public bgr< T >
{
protected:
	typedef bgr< T > base;

public:
	typedef typename base::size_type		size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type	difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename base::reference		reference;				///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef typename base::const_reference	const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename base::value_type		value_type;				///< @brief �����f�[�^�^�DT �Ɠ���
	typedef typename base::pointer			pointer;				///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef typename base::const_pointer	const_pointer;			///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT > 
	struct rebind
	{
		typedef bgra< TT > other;
	};

public:
	value_type a;		///< @brief �A���t�@����

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�Ă̗v�f��0�ŏ���������j
	bgra( ) : base( ), a( 255 ){ }

	/// @brief �S�Ă̐����� pix �ŏ���������
	explicit bgra( const value_type &pix ) : base( pix ), a( 255 ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	bgra( const bgra< TT > &c ) : base( c ), a( static_cast< value_type >( c.a ) ){ }

	/// @brief ���̃J���[��f��p���ď���������
	bgra( const bgra< T > &c ) : base( c ), a( c.a ){ }


	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	bgra( const rgb< TT > &c ) : base( c ), a( 255 ){ }

	/// @brief ���̃J���[��f��p���ď���������
	bgra( const rgb< T > &c ) : base( c ), a( 255 ){ }


	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	bgra( const bgr< TT > &c ) : base( c ), a( 255 ){ }

	/// @brief ���̃J���[��f��p���ď���������
	bgra( const bgr< T > &c ) : base( c ), a( 255 ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	bgra( const rgba< TT > &c ) : base( c ), a( c.a ){ }

	/// @brief �� rr�C�� gg�C�� bb ��p���ď���������
	bgra( const value_type &rr, const value_type &gg, const value_type &bb, const value_type &aa = 255 ) : base( rr, gg, bb ), a( aa ){ }


	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const bgra &operator =( const bgra< TT > &c )
	{
		base::operator =( c );
		a = static_cast< value_type >( c.a );
		return( *this );
	}

	/// @brief ���̃J���[��f��������
	const bgra &operator =( const bgra< T > &c )
	{
		if( &c != this )
		{
			base::operator =( c );
			a = c.a;
		}
		return( *this );
	}

	/// @brief �S�Ă̗v�f�� pix ��������
	const bgra &operator =( const value_type &pix )
	{
		base::operator =( pix );
		return( *this );
	}


	/// @brief �S�v�f�̕������]
	const bgra  operator -( ) const { return( bgra( -base::r, -base::g, -base::b, a ) ); }

	/// @brief RGB�����̘a
	template < class TT >
	const bgra &operator +=( const bgra< TT > &c ){ base::operator +=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̍�
	template < class TT >
	const bgra &operator -=( const bgra< TT > &c ){ base::operator -=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̐�
	template < class TT >
	const bgra &operator *=( const bgra< TT > &c ){ base::operator *=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̊���Z
	template < class TT >
	const bgra &operator /=( const bgra< TT > &c ){ base::operator /=( ( const base &)c ); return( *this ); }

	/// @brief RGB�����̏�]
	const bgra &operator %=( const bgra &c ){ base::operator %=( ( const base &)c ); return( *this ); }

	/// @brief RGB������ | ���Z
	const bgra &operator |=( const bgra &c ){ base::operator |=( ( const base &)c ); return( *this ); }

	/// @brief RGB������ & ���Z
	const bgra &operator &=( const bgra &c ){ base::operator &=( ( const base &)c ); return( *this ); }

	/// @brief RGB������ ^ ���Z
	const bgra &operator ^=( const bgra &c ){ base::operator ^=( ( const base &)c ); return( *this ); }


	/// @brief RGB������ pix �l�𑫂�
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator +=( const double &pix )
#else
	template < class TT >
	const bgra &operator +=( const TT &pix )
#endif
	{
		base::operator +=( pix );
		return( *this );
	}

	/// @brief RGB�������� pix �l������
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator -=( const double &pix )
#else
	template < class TT >
	const bgra &operator -=( const TT &pix )
#endif
	{
		base::operator -=( pix );
		return( *this );
	}

	/// @brief RGB������ pix �l���|����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator *=( const double &pix )
#else
	template < class TT >
	const bgra &operator *=( const TT &pix )
#endif
	{
		base::operator *=( pix );
		return( *this );
	}

	/// @brief RGB������ pix �l�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const bgra &operator /=( const double &pix )
#else
	template < class TT >
	const bgra &operator /=( const TT &pix )
#endif
	{
		base::operator /=( pix );
		return( *this );
	}


	/// @brief 2�̃J���[��f���������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const bgra &c ) const { return( base::operator ==( c ) ); }

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const bgra &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const bgra &c ) const
	{
		if( a == c.a )
		{
			return( base::operator <( c ) );
		}
		else
		{
			return( a < c.a );
		}
	}


	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const bgra &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const bgra &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const bgra &c ) const { return( !( *this < c ) ); }

};

/// @brief �J���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, + )

/// @brief �J���[��f�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, + )

/// @brief �萔�ƃJ���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR3( bgra, + )

/// @brief �J���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, - )

/// @brief �J���[��f�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, - )

/// @brief �萔�ƃJ���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR4( bgra, - )

/// @brief �J���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, * )

/// @brief �J���[��f�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, * )

/// @brief �萔�ƃJ���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR3( bgra, * )

/// @brief �J���[��f�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, / )

/// @brief �J���[��f��萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( bgra, / )

/// @brief �J���[��f�̏�]
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, % )


/// @brief �J���[��f�� | ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, | )

/// @brief �J���[��f�� & ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, & )

/// @brief �J���[��f�� ^ ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( bgra, ^ )


/// @brief �J���[�摜�p�̉�f�i6�����j
//! 
//! @code �J���[�摜�̍쐬��
//! mist::array2< mist::RGB< unsigned char > > image;
//! @endcode
//! 
//! @param T �c �e�F�����̃f�[�^�^
//! 
template< class T >
struct nRGB : public rgb< T >
{
private:
	typedef rgb< T > base;																			///< @brief ���N���X
	typedef __color_utility__::__normalized_color__< is_float< T >::value >	__color_normalizer__;	///< @brief �F�̐��K�����s���N���X

public:
	typedef typename base::size_type		size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type	difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef T& reference;											///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const T& const_reference;								///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef T value_type;											///< @brief �����f�[�^�^�DT �Ɠ���
	typedef T* pointer;												///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef const T* const_pointer;									///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT > 
	struct rebind
	{
		typedef nRGB< TT > other;
	};

public:
	value_type R;		///< @brief �ԐF�����i���K���j
	value_type G;		///< @brief �ΐF�����i���K���j
	value_type B;		///< @brief �F�����i���K���j

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�Ă̗v�f��0�ŏ���������j
	nRGB( ) : base( ), R( 0 ), G( 0 ), B( 0 ){ }

	/// @brief �S�Ă̐����� pix �ŏ���������
	explicit nRGB( const value_type &pix ) : base( pix )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	nRGB( const nRGB< TT > &c ) : base( c ), R( static_cast< T >( c.R ) ), G( static_cast< T >( c.G ) ), B( static_cast< T >( c.B ) ){ }

	/// @brief ���̃J���[��f��p���ď���������
	nRGB( const nRGB< T > &c ) : base( c ), R( c.R ), G( c.G ), B( c.B ){ }

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	nRGB( const rgb< TT > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief ���̃J���[��f��p���ď���������
	nRGB( const rgb< T > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	nRGB( const bgr< TT > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief ���̃J���[��f��p���ď���������
	nRGB( const bgr< T > &c ) : base( c )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}


	/// @brief �� rr�C�� gg�C�� bb ��p���ď���������
	nRGB( const value_type &rr, const value_type &gg, const value_type &bb ) : base( rr, gg, bb )
	{
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
	}

	/// @brief �� rr�C�� gg�C�� bb ��p���ď���������
	nRGB( const value_type &rr, const value_type &gg, const value_type &bb, const value_type &RR, const value_type &GG, const value_type &BB ) : base( rr, gg, bb ), R( RR ), G( GG ), B( BB ){ }


	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const nRGB &operator =( const nRGB< TT > &c )
	{
		base::operator =( c );
		R = static_cast< value_type >( c.R );
		G = static_cast< value_type >( c.G );
		B = static_cast< value_type >( c.B );
		return( *this );
	}

	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const nRGB &operator =( const rgb< TT > &c )
	{
		base::operator =( c );
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
		return( *this );
	}

	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const nRGB &operator =( const bgr< TT > &c )
	{
		base::operator =( c );
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
		return( *this );
	}

	/// @brief ���̃J���[��f��������
	const nRGB &operator =( const nRGB< T > &c )
	{
		if( &c != this )
		{
			base::operator =( c );
			R = c.R;
			G = c.G;
			B = c.B;
		}
		return( *this );
	}

	/// @brief �S�Ă̗v�f�� pix ��������
	const nRGB &operator =( const value_type &pix )
	{
		base::operator =( pix );
		__color_normalizer__::compute( base::r, base::g, base::b, R, G, B );
		return( *this );
	}


	/// @brief �S�v�f�̕������]
	const nRGB  operator -( ) const { return( nRGB( -base::r, -base::g, -base::b, -R, -G, -B ) ); }

	/// @brief nRGB�����̘a
	template < class TT >
	const nRGB &operator +=( const nRGB< TT > &c )
	{
		base::operator +=( ( const base &)c );
		R = static_cast< value_type >( R + c.R );
		G = static_cast< value_type >( G + c.G );
		B = static_cast< value_type >( B + c.B );
		return( *this );
	}

	/// @brief nRGB�����̍�
	template < class TT >
	const nRGB &operator -=( const nRGB< TT > &c )
	{
		base::operator -=( ( const base &)c );
		R = static_cast< value_type >( R - c.R );
		G = static_cast< value_type >( G - c.G );
		B = static_cast< value_type >( B - c.B );
		return( *this );
	}

	/// @brief nRGB�����̐�
	template < class TT >
	const nRGB &operator *=( const nRGB< TT > &c )
	{
		base::operator *=( ( const base &)c );
		R = static_cast< value_type >( R * c.R );
		G = static_cast< value_type >( G * c.G );
		B = static_cast< value_type >( B * c.B );
		return( *this );
	}

	/// @brief nRGB�����̊���Z
	template < class TT >
	const nRGB &operator /=( const nRGB< TT > &c )
	{
		base::operator /=( ( const base &)c );
		R = static_cast< value_type >( R / c.R );
		G = static_cast< value_type >( G / c.G );
		B = static_cast< value_type >( B / c.B );
		return( *this );
	}

	/// @brief nRGB�����̏�]
	const nRGB &operator %=( const nRGB &c )
	{
		base::operator %=( ( const base &)c );
		R %= c.R;
		G %= c.G;
		B %= c.B;
		return( *this );
	}

	/// @brief nRGB������ | ���Z
	const nRGB &operator |=( const nRGB &c )
	{
		base::operator |=( ( const base &)c );
		R |= c.R;
		G |= c.G;
		B |= c.B;
		return( *this );
	}

	/// @brief nRGB������ & ���Z
	const nRGB &operator &=( const nRGB &c )
	{
		base::operator &=( ( const base &)c );
		R &= c.R;
		G &= c.G;
		B &= c.B;
		return( *this );
	}

	/// @brief nRGB������ ^ ���Z
	const nRGB &operator ^=( const nRGB &c )
	{
		base::operator ^=( ( const base &)c );
		R ^= c.R;
		G ^= c.G;
		B ^= c.B;
		return( *this );
	}


	/// @brief nRGB������ pix �l�𑫂�
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator +=( const double &pix )
#else
	template < class TT >
	const nRGB &operator +=( const TT &pix )
#endif
	{
		base::operator +=( pix );
		R = static_cast< value_type >( R + pix );
		G = static_cast< value_type >( G + pix );
		B = static_cast< value_type >( B + pix );
		return( *this );
	}

	/// @brief nRGB�������� pix �l������
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator -=( const double &pix )
#else
	template < class TT >
	const nRGB &operator -=( const TT &pix )
#endif
	{
		base::operator -=( pix );
		R = static_cast< value_type >( R - pix );
		G = static_cast< value_type >( G - pix );
		B = static_cast< value_type >( B - pix );
		return( *this );
	}

	/// @brief nRGB������ pix �l���|����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator *=( const double &pix )
#else
	template < class TT >
	const nRGB &operator *=( const TT &pix )
#endif
	{
		base::operator *=( pix );
		R = static_cast< value_type >( R * pix );
		G = static_cast< value_type >( G * pix );
		B = static_cast< value_type >( B * pix );
		return( *this );
	}

	/// @brief nRGB������ pix �l�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const nRGB &operator /=( const double &pix )
#else
	template < class TT >
	const nRGB &operator /=( const TT &pix )
#endif
	{
		base::operator /=( pix );
		R = static_cast< value_type >( R / pix );
		G = static_cast< value_type >( G / pix );
		B = static_cast< value_type >( B / pix );
		return( *this );
	}
};



/// @brief �J���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, + )

/// @brief �J���[��f�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, + )

/// @brief �萔�ƃJ���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR3( nRGB, + )

/// @brief �J���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, - )

/// @brief �J���[��f�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, - )

/// @brief �萔�ƃJ���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR4( nRGB, - )

/// @brief �J���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, * )

/// @brief �J���[��f�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, * )

/// @brief �萔�ƃJ���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR3( nRGB, * )

/// @brief �J���[��f�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, / )

/// @brief �J���[��f��萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( nRGB, / )

/// @brief �J���[��f�̏�]
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, % )


/// @brief �J���[��f�� | ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, | )

/// @brief �J���[��f�� & ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, & )

/// @brief �J���[��f�� ^ ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( nRGB, ^ )



/// @brief �J���[�摜�p�̉�f
//! 
//! @code �J���[�摜�̍쐬��
//! mist::array2< mist::color< unsigned char, 9 > > image;
//! @endcode
//! 
//! @param T �c �e�F�����̃f�[�^�^
//! 
template< class T, size_t NDIM = 9 >
struct color
{
public:
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef T& reference;					///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const T& const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�
	typedef T value_type;					///< @brief �����f�[�^�^�DT �Ɠ���
	typedef T* pointer;						///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef const T* const_pointer;			///< @brief �f�[�^�^�� const �|�C���^�[�^�Ddata �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT > 
	struct rebind
	{
		typedef color< TT, NDIM > other;
	};

private:
	value_type c_[ NDIM ];		///< @brief �F����

public:

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�Ă̗v�f��0�ŏ���������j
	color( )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = 0;
		}
	}

	/// @brief �S�Ă̐����� pix �ŏ���������
	explicit color( const value_type &pix )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = pix;
		}
	}

	/// @brief �قȂ�^�̃J���[��f��p���ď���������
	template < class TT >
	color( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c[ i ] );
		}
	}

	/// @brief ���̃J���[��f��p���ď���������
	color( const color< T, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = c[ i ];
		}
	}


	/// @brief �قȂ�^�̑��̃J���[��f��������
	template < class TT >
	const color &operator =( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c[ i ] );
		}
		return( *this );
	}

	/// @brief ���̃J���[��f��������
	const color &operator =( const color< T, NDIM > &c )
	{
		if( &c != this )
		{
			for( size_type i = 0 ; i < NDIM ; i++ )
			{
				c_[ i ] = c[ i ];
			}
		}
		return( *this );
	}

	/// @brief �S�Ă̗v�f�� pix ��������
	const color &operator =( const value_type &pix )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = pix;
		}
		return( *this );
	}


	/// @brief �S�v�f�̕������]
	const color  operator -( ) const
	{
		color c;
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c[ i ] = -c_[ i ];
		}
		return( c );
	}

	/// @brief RGB�����̘a
	template < class TT >
	const color &operator +=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] + c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB�����̍�
	template < class TT >
	const color &operator -=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] - c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB�����̐�
	template < class TT >
	const color &operator *=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] * c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB�����̊���Z
	template < class TT >
	const color &operator /=( const color< TT, NDIM > &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] / c[ i ] );
		}
		return( *this );
	}

	/// @brief RGB�����̏�]
	const color &operator %=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] %= c[ i ];
		}
		return( *this );
	}

	/// @brief RGB������ | ���Z
	const color &operator |=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] |= c[ i ];
		}
		return( *this );
	}

	/// @brief RGB������ & ���Z
	const color &operator &=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] &= c[ i ];
		}
		return( *this );
	}

	/// @brief RGB������ ^ ���Z
	const color &operator ^=( const color &c )
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] ^= c[ i ];
		}
		return( *this );
	}


	/// @brief RGB������ pix �l�𑫂�
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator +=( const double &pix )
#else
	template < class TT >
	const color &operator +=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] + pix );
		}
		return( *this );
	}

	/// @brief RGB�������� pix �l������
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator -=( const double &pix )
#else
	template < class TT >
	const color &operator -=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] - pix );
		}
		return( *this );
	}

	/// @brief RGB������ pix �l���|����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator *=( const double &pix )
#else
	template < class TT >
	const color &operator *=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] * pix );
		}
		return( *this );
	}

	/// @brief RGB������ pix �l�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const color &operator /=( const double &pix )
#else
	template < class TT >
	const color &operator /=( const TT &pix )
#endif
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			c_[ i ] = static_cast< value_type >( c_[ i ] / pix );
		}
		return( *this );
	}


	/// @brief 2�̃J���[��f���������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const color &c ) const
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			if( c_[ i ] != c[ i ] )
			{
				return( false );
			}
		}

		return( true );
	}

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const color &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const color &c ) const
	{
		for( size_type i = 0 ; i < NDIM ; i++ )
		{
			if( c_[ i ] != c[ i ] )
			{
				return( c_[ i ] < c[ i ] );
			}
		}

		return( false );
	}

	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const color &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b } \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const color &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const color &c ) const { return( !( *this < c ) ); }

	value_type & operator []( size_type index ){ return( c_[ index ] ); }

	const value_type & operator []( size_type index ) const { return( c_[ index ] ); }

	/// @brief NTSC�n���d���ϖ@�ɂ��C�O���[�X�P�[���֕ϊ�����
	value_type get_value( ) const
	{
		double sum = 0.0;
		for( size_type i = 0 ; i < NDIM - 1 ; i++ )
		{
			sum += c_[ i ];
		}
		return( half_adjust< value_type >::convert( sum ) );
	}

	// �J���[����O���[�X�P�[���ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator value_type( ) const { return( get_value( ) ); }
};



/// @brief �J���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR1_( color, + )

/// @brief �J���[��f�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2_( color, + )

/// @brief �萔�ƃJ���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR3_( color, + )

/// @brief �J���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR1_( color, - )

/// @brief �J���[��f�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2_( color, - )

/// @brief �萔�ƃJ���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR4_( color, - )

/// @brief �J���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR1_( color, * )

/// @brief �J���[��f�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2_( color, * )

/// @brief �萔�ƃJ���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR3_( color, * )

/// @brief �J���[��f�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1_( color, / )

/// @brief �J���[��f��萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2_( color, / )

/// @brief �J���[��f�̏�]
DEFINE_PROMOTE_BIND_OPERATOR1_( color, % )


/// @brief �J���[��f�� | ���Z
DEFINE_PROMOTE_BIND_OPERATOR1_( color, | )

/// @brief �J���[��f�� & ���Z
DEFINE_PROMOTE_BIND_OPERATOR1_( color, & )

/// @brief �J���[��f�� ^ ���Z
DEFINE_PROMOTE_BIND_OPERATOR1_( color, ^ )




/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     c   �c �J���[��f
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2, 3 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const rgb< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << " )";
	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     c   �c �J���[��f
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2, 3 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const rgba< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << ", ";
	out << c.a << " )";
	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     c   �c �J���[��f
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2, 3, 1, 2, 3 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const nRGB< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << ", ";
	out << c.R << ", ";
	out << c.G << ", ";
	out << c.B << " )";
	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     c   �c �J���[��f
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2, 3, 1, 2, 3 )
//! @endcode
//! 
template < class T, size_t NDIM > inline std::ostream &operator <<( std::ostream &out, const color< T, NDIM > &c )
{
	out << "( ";
	for( size_t i = 0 ; i < NDIM - 1 ; i++ )
	{
		out << c[ i ] << ", ";
	}
	out << c[ NDIM - 1 ] << " )";
	return( out );
}



/// @brief RGB�F��Ԃ�HSV�F��Ԃɕϊ�����
//! 
//! @attention ���͂Əo�͂̒l�͈̔͂ɒ��ӁI�I
//! 
//! @param[in]  r �c RGB�F��Ԃ�R(��)����(0�`255)
//! @param[in]  g �c RGB�F��Ԃ�G(��)����(0�`255)
//! @param[in]  b �c RGB�F��Ԃ�B(��)����(0�`255)
//! @param[out] h �c HSV�F��Ԃ�H(�F��)����(0�`360)
//! @param[out] s �c HSV�F��Ԃ�S(�ʓx)����(0�`�P)
//! @param[out] v �c HSV�F��Ԃ�V(���x)����(0�`�P)
//! 
inline void rgb2hsv( double r, double g, double b, double &h, double &s, double &v )
{
	double max = __color_utility__::maximum( r, __color_utility__::maximum( g, b ) );
	double min = __color_utility__::minimum( r, __color_utility__::minimum( g, b ) );

	double d = max - min;
	v = max / 255.0;

	if( d != 0.0 )
	{
		s = d / max;
	}
	else
	{
		s = 0.0;
	}

	if( s == 0.0 )
	{
		h = 0.0;
	}
	else
	{
		double rt = max - r * 60.0 / d;
		double gt = max - g * 60.0 / d;
		double bt = max - b * 60.0 / d;

		if( r == max )
		{
			h = bt - gt;
		}
		else if( g == max )
		{
			h = 120 + rt - bt;
		}
		else
		{
			h = 240 + gt - rt;
		}

		if( h < 0.0 )
		{
			h += 360.0;
		}
	}
}


/// @brief HSV�F��Ԃ�RGB�F��Ԃɕϊ�����
//! 
//! @attention ���͂Əo�͂̒l�͈̔͂ɒ��ӁI�I
//! 
//! @param[in]  h �c HSV�F��Ԃ�H(�F��)����(0�`360)
//! @param[in]  s �c HSV�F��Ԃ�S(�ʓx)����(0�`�P)
//! @param[in]  v �c HSV�F��Ԃ�V(���x)����(0�`�P)
//! @param[out] r �c RGB�F��Ԃ�R(��)����(0�`255)
//! @param[out] g �c RGB�F��Ԃ�G(��)����(0�`255)
//! @param[out] b �c RGB�F��Ԃ�B(��)����(0�`255)
//! 
inline void hsv2rgb( double h, double s, double v, double &r, double &g, double &b )
{
	if( s == 0.0 )
	{
		r = g = b = v * 255.0;
	}
	else
	{
		int ht = static_cast< int >( h * 6.0 );
		int d  = ht % 360;

		ht /= 360;

		double t1 = v * ( 1.0 - s );
		double t2 = v * ( 1.0 - s * d / 360.0 );
		double t3 = v * ( 1.0 - s * ( 360.0 - d ) / 360.0 );

		switch( ht )
		{
		case 0:
			r = v;
			g = t3;
			b = t1;
			break;

		case 1:
			r = t2;
			g = v;
			b = t1;
			break;

		case 2:
			r = t1;
			g = v;
			b = t3;
			break;

		case 3:
			r = t1;
			g = t2;
			b = v;
			break;

		case 4:
			r = t3;
			g = t1;
			b = v;
			break;

		default:
			r = v;
			g = t1;
			b = t2;
			break;
		}

		r *= 255.0;
		g *= 255.0;
		b *= 255.0;
	}
}




/// @brief RGB�F��Ԃ�XYZ�F��Ԃɕϊ�����
//! 
//! @attention ���͂Əo�͂̒l�͈̔͂ɒ��ӁI�I
//! 
//! @param[in]  r �c RGB�F��Ԃ�R(��)����(0�`255)
//! @param[in]  g �c RGB�F��Ԃ�G(��)����(0�`255)
//! @param[in]  b �c RGB�F��Ԃ�B(��)����(0�`255)
//! @param[out] x �c XYZ�F��Ԃ�X����(0�`�P)
//! @param[out] y �c XYZ�F��Ԃ�Y����(0�`�P)
//! @param[out] z �c XYZ�F��Ԃ�Z����(0�`�P)
//! 
inline void rgb2xyz( double r, double g, double b, double &x, double &y, double &z )
{
	x = ( 0.412453 * r + 0.357580 * g + 0.180423 * b ) / 255.0;
	y = ( 0.212671 * r + 0.715160 * g + 0.072169 * b ) / 255.0;
	z = ( 0.019334 * r + 0.119193 * g + 0.950227 * b ) / 255.0;
}


/// @brief XYZ�F��Ԃ�RGB�F��Ԃɕϊ�����
//! 
//! @attention ���͂Əo�͂̒l�͈̔͂ɒ��ӁI�I
//! 
//! @param[in]  x �c XYZ�F��Ԃ�X����(0�`�P)
//! @param[in]  y �c XYZ�F��Ԃ�Y����(0�`�P)
//! @param[in]  z �c XYZ�F��Ԃ�Z����(0�`�P)
//! @param[out] r �c RGB�F��Ԃ�R(��)����(0�`255)
//! @param[out] g �c RGB�F��Ԃ�G(��)����(0�`255)
//! @param[out] b �c RGB�F��Ԃ�B(��)����(0�`255)
//! 
inline void xyz2rgb( double x, double y, double z, double &r, double &g, double &b )
{
	r = (  3.240479 * x - 1.537150 * y - 0.498535 * z ) * 255.0;
	g = ( -0.969256 * x + 1.875991 * y + 0.041556 * z ) * 255.0;
	b = (  0.055648 * x - 0.204043 * y + 1.057311 * z ) * 255.0;
}


/// @brief RGB�F��Ԃ�L*a*b*�F��Ԃɕϊ�����
//! 
//! @attention ���͂Əo�͂̒l�͈̔͂ɒ��ӁI�I
//! 
//! @param[in]  r �c RGB�F��Ԃ�R(��)����(0�`255)
//! @param[in]  g �c RGB�F��Ԃ�G(��)����(0�`255)
//! @param[in]  b �c RGB�F��Ԃ�B(��)����(0�`255)
//! @param[out] l_ �c L*a*b*�F��Ԃ�L(���x�w��)����(0�`100)
//! @param[out] a_ �c L*a*b*�F��Ԃ�a(�m�o�F�x)����(-134�`220)
//! @param[out] b_ �c L*a*b*�F��Ԃ�b(�m�o�F�x)����(-140�`122)
//! 
inline void rgb2lab( double r, double g, double b, double &l_, double &a_, double &b_ )
{
	double x, y, z;

	// XYZ�\�F�n����RGB�\�F�n�֕ϊ�
	rgb2xyz( r, g, b, x, y, z );

	static const double Xr = 0.9504;
	static const double Yr = 1.0;
	static const double Zr = 1.0889;

	x /= Xr;
	y /= Yr;
	z /= Zr;

	static const double th = 216.0 / 24389.0;
	static const double _1_3 = 1.0 / 3.0;
	static const double A = 29.0 * 29.0 / ( 3.0 * 6.0 * 6.0 );
	static const double B = 4.0 / 29.0;
	x = x > th ? std::pow( x, _1_3 ) : A * x + B;
	y = y > th ? std::pow( y, _1_3 ) : A * y + B;
	z = z > th ? std::pow( z, _1_3 ) : A * z + B;

	l_ = 116.0 * y - 16.0;
	a_ = 500.0 * ( x - y );
	b_ = 200.0 * ( y - z );
}

/// @brief L*a*b*�F��Ԃ�RGB�F��Ԃɕϊ�����
//! 
//! @attention ���͂Əo�͂̒l�͈̔͂ɒ��ӁI�I
//! 
//! @param[in]  l_ �c L*a*b*�F��Ԃ�L(���x�w��)����(0�`100)
//! @param[in]  a_ �c L*a*b*�F��Ԃ�a(�m�o�F�x)����(-134�`220)
//! @param[in]  b_ �c L*a*b*�F��Ԃ�b(�m�o�F�x)����(-140�`122)
//! @param[out] r �c RGB�F��Ԃ�R(��)����(0�`255)
//! @param[out] g �c RGB�F��Ԃ�G(��)����(0�`255)
//! @param[out] b �c RGB�F��Ԃ�B(��)����(0�`255)
//! 
inline void lab2rgb( double l_, double a_, double b_, double &r, double &g, double &b )
{
	double fy = ( l_ + 16.0 ) / 116.0;
	double fx = fy + a_ / 500.0 ;
	double fz = fy - b_ / 200.0;

	static const double Xr = 0.9504;
	static const double Yr = 1.0;
	static const double Zr = 1.0889;

	static const double delta = 6.0 / 29.0;
	static const double delta2 = 3.0 * delta * delta;
	double y = fy > delta ? Yr * fy * fy * fy : ( fy - 16.0 / 116.0 ) * delta2 * Yr;
	double x = fx > delta ? Xr * fx * fx * fx : ( fx - 16.0 / 116.0 ) * delta2 * Xr;
	double z = fz > delta ? Zr * fz * fz * fz : ( fz - 16.0 / 116.0 ) * delta2 * Zr;

	// XYZ�\�F�n����RGB�\�F�n�֕ϊ�
	xyz2rgb( x, y, z, r, g, b );
}

/// @brief ���͂��ꂽ�^���J���[�摜���ǂ������ׂ�
template < class T >
struct is_color
{
	_MIST_CONST( bool, value, false );
};


/// @}
//  MIST�S�ʂŗ��p�ł���MIST�S�ʂŗ��p�\�ȐF��\������N���X


// ��f�̕ϊ����T�|�[�g���邽�߂̃R���o�[�^
template < class T >
struct _pixel_converter_
{
	typedef T value_type;
	typedef rgba< T > color_type;
	enum{ color_num = 1 };

	static value_type convert_to( value_type r, value_type g, value_type b, value_type /* a */ = 255 )
	{
		return( color_type( r, g, b ).get_value( ) );
	}

	static color_type convert_from( const value_type &pixel )
	{
		return( color_type( pixel ) );
	}
};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_COLOR( type ) \
		template < >\
		struct is_color< rgb< type > >\
		{\
			enum{ value = true };\
		};\
		template < >\
		struct is_color< bgr< type > >\
		{\
			enum{ value = true };\
		};\
		template < >\
		struct is_color< rgba< type > >\
		{\
			enum{ value = true };\
		};\
		template < >\
		struct is_color< bgra< type > >\
		{\
			enum{ value = true };\
		};\


	#define __PIXEL_CONVERTER__( type ) \
		template < >\
		struct _pixel_converter_< rgb< type > >\
		{\
			typedef type value_type;\
			typedef rgba< type > color_type;\
			enum{ color_num = 3 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\
		template < >\
		struct _pixel_converter_< bgr< type > >\
		{\
			typedef type value_type;\
			typedef rgba< type > color_type;\
			enum{ color_num = 3 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\
		template < >\
		struct _pixel_converter_< rgba< type > >\
		{\
			typedef type value_type;\
			typedef rgba< type > color_type;\
			enum{ color_num = 4 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b, a ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\
		template < >\
		struct _pixel_converter_< bgra< type > >\
		{\
			typedef type value_type;\
			typedef bgra< type > color_type;\
			enum{ color_num = 4 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )\
			{\
				return( color_type( r, g, b, a ) );\
			}\
			\
			static color_type convert_from( const color_type &pixel )\
			{\
				return( pixel );\
			}\
		};\

		// �e�^�ɑ΂�����ꉻ
		IS_COLOR(unsigned char)
		IS_COLOR(unsigned short)
		IS_COLOR(unsigned int)
		IS_COLOR(unsigned long)
		IS_COLOR(signed char)
		IS_COLOR(signed short)
		IS_COLOR(signed int)
		IS_COLOR(signed long)
		IS_COLOR(bool)
		IS_COLOR(char)
		IS_COLOR(float)
		IS_COLOR(double)
		IS_COLOR(long double)
		__PIXEL_CONVERTER__(unsigned char)
		__PIXEL_CONVERTER__(unsigned short)
		__PIXEL_CONVERTER__(unsigned int)
		__PIXEL_CONVERTER__(unsigned long)
		__PIXEL_CONVERTER__(signed char)
		__PIXEL_CONVERTER__(signed short)
		__PIXEL_CONVERTER__(signed int)
		__PIXEL_CONVERTER__(signed long)
		__PIXEL_CONVERTER__(bool)
		__PIXEL_CONVERTER__(char)
		__PIXEL_CONVERTER__(float)
		__PIXEL_CONVERTER__(double)
		__PIXEL_CONVERTER__(long double)

		#undef IS_COLOR
		#undef __PIXEL_CONVERTER__

#else

	template < class T >
	struct is_color< rgb< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< bgr< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< rgba< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< bgra< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct is_color< nRGB< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T, size_t NDIM >
	struct is_color< color< T, NDIM > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct _pixel_converter_< rgb< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const rgb< T > &pixel )
		{
			return( pixel );
		}
	};

	template < class T >
	struct _pixel_converter_< bgr< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const bgr< T > &pixel )
		{
			return( color_type( pixel.r, pixel.g, pixel.b ) );
		}
	};

	template < class T >
	struct _pixel_converter_< rgba< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 4 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const rgba< T > &pixel )
		{
			return( pixel );
		}
	};

	template < class T >
	struct _pixel_converter_< bgra< T > >
	{
		typedef T value_type;
		typedef bgra< T > color_type;
		enum{ color_num = 4 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const bgra< T > &pixel )
		{
			return( color_type( pixel.r, pixel.g, pixel.b, pixel.a ) );
		}
	};

	template < class T >
	struct _pixel_converter_< nRGB< T > >
	{
		typedef T value_type;
		typedef nRGB< T > color_type;
		enum{ color_num = 6 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type /* a */ = 255 )
		{
			return( color_type( r, g, b ) );
		}

		static color_type convert_from( const nRGB< T > &pixel )
		{
			return( color_type( pixel.r, pixel.g, pixel.b ) );
		}
	};

	template < class T, size_t NDIM >
	struct _pixel_converter_< color< T, NDIM > >
	{
		typedef T value_type;
		typedef color< T, NDIM > color_type;
		enum{ color_num = NDIM };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( ( r + g + b ) / 3 ) );
		}

		static color_type convert_from( const color< T, NDIM > &pixel )
		{
			if( NDIM < 3 )
			{
				return( color_type( pixel[ 0 ] ) );
			}
			else
			{
				return( color_type( pixel[ 0 ], pixel[ 1 ], pixel[ 2 ] ) );
			}
		}
	};

#endif


#define __DEFINE_COLOR_TYPE_TRAIT__( function, type ) \
	template<> struct function<  rgb< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function<  bgr< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function< rgba< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function< bgra< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template<> struct function<  nRGB< type > >{ _MIST_CONST( bool, value, true  ); }; \
	template< size_t NDIM > struct function<  color< type, NDIM > >{ _MIST_CONST( bool, value, true  ); }; \

// type_trait ���̋@�\���g������
/// @brief char ����
//! 
//! T���P�o�C�g�̕�����^�ł���ΐ^�ɕ]������
//! 
//! @param T  �c ��������^
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_char, unsigned char )
__DEFINE_COLOR_TYPE_TRAIT__( is_char, signed char )
__DEFINE_COLOR_TYPE_TRAIT__( is_char, char )


/// @brief ���������_ ����
//! 
//! T �� float �� double �^�ł���ΐ^�ɕ]������
//! 
//! @param T  �c ��������^
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_float, float )
__DEFINE_COLOR_TYPE_TRAIT__( is_float, double )
__DEFINE_COLOR_TYPE_TRAIT__( is_float, long double )



/// @brief integer ����
//! 
//! T �������^�ł���ΐ^�ɕ]������B�Đ����^������ɂ�����
//! 
//! @param T  �c ��������^
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned char )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned short )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned int )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, unsigned long )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed char )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed short )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed int )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, signed long )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, bool )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, char )

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, size_t )
__DEFINE_COLOR_TYPE_TRAIT__( is_integer, ptrdiff_t )
#endif



/// @brief arithmetic ����
//! 
//! T ���Z�p�^�ł���ΐ^�ɕ]������B�Đ����^�����������_�^�̂����ꂩ������ɂ�����
//! 
//! @param T  �c ��������^
//! 
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned char )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned short )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned int )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, unsigned long )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed char )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed short )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed int )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, signed long )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, bool )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, char )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, float )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, double )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, long double )

#if defined( __MIST64__ ) && __MIST64__ != 0 && !( defined( __GNUC__ ) || defined( __APPLE__ ) || defined( __ICC ) )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, size_t )
__DEFINE_COLOR_TYPE_TRAIT__( is_arithmetic, ptrdiff_t )
#endif


/// @brief �Ή����� float �^��Ԃ�
//! 
//! T �� float �̏ꍇ�� float ��Ԃ��C����ȊO�� double �i�������� long double�j�Ƃ���
//! 
//! @param T  �c ��������^
//! 
template< class T > struct float_type<  rgb< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };
template< class T > struct float_type<  bgr< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };
template< class T > struct float_type< rgba< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };
template< class T > struct float_type< bgra< T > > { typedef rgb< typename float_type< T >::value_type > value_type; };


#undef __DEFINE_COLOR_TYPE_TRAIT__

/// @brief ��`�ςݐF
template< typename T = rgb< unsigned char > >
struct colors
{
	typedef T color_type;

	static color_type aliceblue( )				{ return color_type( 0xf0, 0xf8, 0xff ); }
	static color_type antiquewhite( )			{ return color_type( 0xfa, 0xeb, 0xd7 ); }
	static color_type aqua( )					{ return color_type( 0x00, 0xff, 0xff ); }
	static color_type aquamarine( )				{ return color_type( 0x7f, 0xff, 0xd4 ); }
	static color_type azure( )					{ return color_type( 0xf0, 0xff, 0xff ); }
	static color_type beige( )					{ return color_type( 0xf5, 0xf5, 0xdc ); }
	static color_type bisque( )					{ return color_type( 0xff, 0xe4, 0xc4 ); }
	static color_type black( )					{ return color_type( 0x00, 0x00, 0x00 ); }
	static color_type blanchedalmond( )			{ return color_type( 0xff, 0xeb, 0xcd ); }
	static color_type blue( )					{ return color_type( 0x00, 0x00, 0xff ); }
	static color_type blueviolet( )				{ return color_type( 0x8a, 0x2b, 0xe2 ); }
	static color_type brown( )					{ return color_type( 0xa5, 0x2a, 0x2a ); }
	static color_type burlywood( )				{ return color_type( 0xde, 0xb8, 0x87 ); }
	static color_type cadetblue( )				{ return color_type( 0x5f, 0x9e, 0xa0 ); }
	static color_type chartreuse( )				{ return color_type( 0x7f, 0xff, 0x00 ); }
	static color_type chocolate( )				{ return color_type( 0xd2, 0x69, 0x1e ); }
	static color_type coral( )					{ return color_type( 0xff, 0x7f, 0x50 ); }
	static color_type cornflowerblue( )			{ return color_type( 0x64, 0x95, 0xed ); }
	static color_type cornsilk( )				{ return color_type( 0xff, 0xf8, 0xdc ); }
	static color_type crimson( )				{ return color_type( 0xdc, 0x14, 0x3c ); }
	static color_type cyan( )					{ return color_type( 0x00, 0xff, 0xff ); }
	static color_type darkblue( )				{ return color_type( 0x00, 0x00, 0x8b ); }
	static color_type darkcyan( )				{ return color_type( 0x00, 0x8b, 0x8b ); }
	static color_type darkgoldenrod( )			{ return color_type( 0xb8, 0x86, 0x0b ); }
	static color_type darkgray( )				{ return color_type( 0xa9, 0xa9, 0xa9 ); }
	static color_type darkgreen( )				{ return color_type( 0x00, 0x64, 0x00 ); }
	static color_type darkkhaki( )				{ return color_type( 0xbd, 0xb7, 0x6b ); }
	static color_type darkmagenta( )			{ return color_type( 0x8b, 0x00, 0x8b ); }
	static color_type darkolivegreen( )			{ return color_type( 0x55, 0x6b, 0x2f ); }
	static color_type darkorange( )				{ return color_type( 0xff, 0x8c, 0x00 ); }
	static color_type darkorchid( )				{ return color_type( 0x99, 0x32, 0xcc ); }
	static color_type darkred( )				{ return color_type( 0x8b, 0x00, 0x00 ); }
	static color_type darksalmon( )				{ return color_type( 0xe9, 0x96, 0x7a ); }
	static color_type darkseagreen( )			{ return color_type( 0x8f, 0xbc, 0x8f ); }
	static color_type darkslateblue( )			{ return color_type( 0x48, 0x3d, 0x8b ); }
	static color_type darkslategray( )			{ return color_type( 0x2f, 0x4f, 0x4f ); }
	static color_type darkturquoise( )			{ return color_type( 0x00, 0xce, 0xd1 ); }
	static color_type darkviolet( )				{ return color_type( 0x94, 0x00, 0xd3 ); }
	static color_type deeppink( )				{ return color_type( 0xff, 0x14, 0x93 ); }
	static color_type deepskyblue( )			{ return color_type( 0x00, 0xbf, 0xff ); }
	static color_type dimgray( )				{ return color_type( 0x69, 0x69, 0x69 ); }
	static color_type dodgerblue( )				{ return color_type( 0x1e, 0x90, 0xff ); }
	static color_type firebrick( )				{ return color_type( 0xb2, 0x22, 0x22 ); }
	static color_type floralwhite( )			{ return color_type( 0xff, 0xfa, 0xf0 ); }
	static color_type forestgreen( )			{ return color_type( 0x22, 0x8b, 0x22 ); }
	static color_type fuchsia( )				{ return color_type( 0xff, 0x00, 0xff ); }
	static color_type gainsboro( )				{ return color_type( 0xdc, 0xdc, 0xdc ); }
	static color_type ghostwhite( )				{ return color_type( 0xf8, 0xf8, 0xff ); }
	static color_type gold( )					{ return color_type( 0xff, 0xd7, 0x00 ); }
	static color_type goldenrod( )				{ return color_type( 0xda, 0xa5, 0x20 ); }
	static color_type gray( )					{ return color_type( 0x80, 0x80, 0x80 ); }
	static color_type green( )					{ return color_type( 0x00, 0xff, 0x00 ); }
	static color_type greenyellow( )			{ return color_type( 0xad, 0xff, 0x2f ); }
	static color_type honeydew( )				{ return color_type( 0xf0, 0xff, 0xf0 ); }
	static color_type hotpink( )				{ return color_type( 0xff, 0x69, 0xb4 ); }
	static color_type indianred( )				{ return color_type( 0xcd, 0x5c, 0x5c ); }
	static color_type indigo( )					{ return color_type( 0x4b, 0x00, 0x82 ); }
	static color_type ivory( )					{ return color_type( 0xff, 0xff, 0xf0 ); }
	static color_type khaki( )					{ return color_type( 0xf0, 0xe6, 0x8c ); }
	static color_type lavender( )				{ return color_type( 0xe6, 0xe6, 0xfa ); }
	static color_type lavenderblush( )			{ return color_type( 0xff, 0xf0, 0xf5 ); }
	static color_type lawngreen( )				{ return color_type( 0x7c, 0xfc, 0x00 ); }
	static color_type lemonchiffon( )			{ return color_type( 0xff, 0xfa, 0xcd ); }
	static color_type lightblue( )				{ return color_type( 0xad, 0xd8, 0xe6 ); }
	static color_type lightcoral( )				{ return color_type( 0xf0, 0x80, 0x80 ); }
	static color_type lightcyan( )				{ return color_type( 0xe0, 0xff, 0xff ); }
	static color_type lightgoldenrodyellow( )	{ return color_type( 0xfa, 0xfa, 0xd2 ); }
	static color_type lightgray( )				{ return color_type( 0xd3, 0xd3, 0xd3 ); }
	static color_type lightgreen( )				{ return color_type( 0x90, 0xee, 0x90 ); }
	static color_type lightpink( )				{ return color_type( 0xff, 0xb6, 0xc1 ); }
	static color_type lightsalmon( )			{ return color_type( 0xff, 0xa0, 0x7a ); }
	static color_type lightseagreen( )			{ return color_type( 0x20, 0xb2, 0xaa ); }
	static color_type lightskyblue( )			{ return color_type( 0x87, 0xce, 0xfa ); }
	static color_type lightslategray( )			{ return color_type( 0x77, 0x88, 0x99 ); }
	static color_type lightsteelblue( )			{ return color_type( 0xb0, 0xc4, 0xde ); }
	static color_type lightyellow( )			{ return color_type( 0xff, 0xff, 0xe0 ); }
	static color_type lime( )					{ return color_type( 0x00, 0xff, 0x00 ); }
	static color_type limegreen( )				{ return color_type( 0x32, 0xcd, 0x32 ); }
	static color_type linen( )					{ return color_type( 0xfa, 0xf0, 0xe6 ); }
	static color_type magenta( )				{ return color_type( 0xff, 0x00, 0xff ); }
	static color_type maroon( )					{ return color_type( 0x80, 0x00, 0x00 ); }
	static color_type mediumaquamarine( )		{ return color_type( 0x66, 0xcd, 0xaa ); }
	static color_type mediumblue( )				{ return color_type( 0x00, 0x00, 0xcd ); }
	static color_type mediumorchid( )			{ return color_type( 0xba, 0x55, 0xd3 ); }
	static color_type mediumpurple( )			{ return color_type( 0x93, 0x70, 0xdb ); }
	static color_type mediumseagreen( )			{ return color_type( 0x3c, 0xb3, 0x71 ); }
	static color_type mediumslateblue( )		{ return color_type( 0x7b, 0x68, 0xee ); }
	static color_type mediumspringgreen( )		{ return color_type( 0x00, 0xfa, 0x9a ); }
	static color_type mediumturquoise( )		{ return color_type( 0x48, 0xd1, 0xcc ); }
	static color_type mediumvioletred( )		{ return color_type( 0xc7, 0x15, 0x85 ); }
	static color_type midnightblue( )			{ return color_type( 0x19, 0x19, 0x70 ); }
	static color_type mintcream( )				{ return color_type( 0xf5, 0xff, 0xfa ); }
	static color_type mistyrose( )				{ return color_type( 0xff, 0xe4, 0xe1 ); }
	static color_type moccasin( )				{ return color_type( 0xff, 0xe4, 0xb5 ); }
	static color_type navajowhite( )			{ return color_type( 0xff, 0xde, 0xad ); }
	static color_type navy( )					{ return color_type( 0x00, 0x00, 0x80 ); }
	static color_type oldlace( )				{ return color_type( 0xfd, 0xf5, 0xe6 ); }
	static color_type olive( )					{ return color_type( 0x80, 0x80, 0x00 ); }
	static color_type olivedrab( )				{ return color_type( 0x6b, 0x8e, 0x23 ); }
	static color_type orange( )					{ return color_type( 0xff, 0xa5, 0x00 ); }
	static color_type orangered( )				{ return color_type( 0xff, 0x45, 0x00 ); }
	static color_type orchid( )					{ return color_type( 0xda, 0x70, 0xd6 ); }
	static color_type palegoldenrod( )			{ return color_type( 0xee, 0xe8, 0xaa ); }
	static color_type palegreen( )				{ return color_type( 0x98, 0xfb, 0x98 ); }
	static color_type paleturquoise( )			{ return color_type( 0xaf, 0xee, 0xee ); }
	static color_type palevioletred( )			{ return color_type( 0xdb, 0x70, 0x93 ); }
	static color_type papayawhip( )				{ return color_type( 0xff, 0xef, 0xd5 ); }
	static color_type peachpuff( )				{ return color_type( 0xff, 0xda, 0xb9 ); }
	static color_type peru( )					{ return color_type( 0xcd, 0x85, 0x3f ); }
	static color_type pink( )					{ return color_type( 0xff, 0xc0, 0xcb ); }
	static color_type plum( )					{ return color_type( 0xdd, 0xa0, 0xdd ); }
	static color_type powderblue( )				{ return color_type( 0xb0, 0xe0, 0xe6 ); }
	static color_type purple( )					{ return color_type( 0x80, 0x00, 0x80 ); }
	static color_type red( )					{ return color_type( 0xff, 0x00, 0x00 ); }
	static color_type rosybrown( )				{ return color_type( 0xbc, 0x8f, 0x8f ); }
	static color_type royalblue( )				{ return color_type( 0x41, 0x69, 0xe1 ); }
	static color_type saddlebrown( )			{ return color_type( 0x8b, 0x45, 0x13 ); }
	static color_type salmon( )					{ return color_type( 0xfa, 0x80, 0x72 ); }
	static color_type sandybrown( )				{ return color_type( 0xf4, 0xa4, 0x60 ); }
	static color_type seagreen( )				{ return color_type( 0x2e, 0x8b, 0x57 ); }
	static color_type seashell( )				{ return color_type( 0xff, 0xf5, 0xee ); }
	static color_type sienna( )					{ return color_type( 0xa0, 0x52, 0x2d ); }
	static color_type silver( )					{ return color_type( 0xc0, 0xc0, 0xc0 ); }
	static color_type skyblue( )				{ return color_type( 0x87, 0xce, 0xeb ); }
	static color_type slateblue( )				{ return color_type( 0x6a, 0x5a, 0xcd ); }
	static color_type slategray( )				{ return color_type( 0x70, 0x80, 0x90 ); }
	static color_type snow( )					{ return color_type( 0xff, 0xfa, 0xfa ); }
	static color_type springgreen( )			{ return color_type( 0x00, 0xff, 0x7f ); }
	static color_type steelblue( )				{ return color_type( 0x46, 0x82, 0xb4 ); }
	static color_type tan( )					{ return color_type( 0xd2, 0xb4, 0x8c ); }
	static color_type teal( )					{ return color_type( 0x00, 0x80, 0x80 ); }
	static color_type thistle( )				{ return color_type( 0xd8, 0xbf, 0xd8 ); }
	static color_type tomato( )					{ return color_type( 0xff, 0x63, 0x47 ); }
	static color_type transparent( )			{ return color_type( 0x00, 0x00, 0xe0 ); }
	static color_type turquoise( )				{ return color_type( 0x40, 0xe0, 0xd0 ); }
	static color_type violet( )					{ return color_type( 0xee, 0x82, 0xee ); }
	static color_type wheat( )					{ return color_type( 0xf5, 0xde, 0xb3 ); }
	static color_type white( )					{ return color_type( 0xff, 0xff, 0xff ); }
	static color_type whitesmoke( )				{ return color_type( 0xf5, 0xf5, 0xf5 ); }
	static color_type yellow( )					{ return color_type( 0xff, 0xff, 0x00 ); }
	static color_type yellowgreen( )			{ return color_type( 0x9a, 0xcd, 0x32 ); }
};


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_COLOR_H__
