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

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


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
	//! \f[
	//! 	\mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && b == c.b ); }

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const rgb &c ) const { return( !( *this >= c ) ); }

	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const rgb &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const rgb &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const rgb &c ) const { return( r >= c.r && g >= c.g && b >= c.b ); }


	/// @brief NTSC�n���d���ϖ@�ɂ��C�O���[�X�P�[���֕ϊ�����
	value_type get_value( ) const
	{
		return( static_cast< value_type >( r * 0.298912 + g * 0.586610 + b * 0.114478 ) );
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
	//! \f[
	//! 	\mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const bgr &c ) const { return( b == c.b && g == c.g && r == c.r ); }

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const bgr &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const bgr &c ) const { return( !( *this >= c ) ); }

	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const bgr &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const bgr &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const bgr &c ) const { return( b >= c.b && g >= c.g && r >= c.r ); }


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
	//! \f[
	//! 	\mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���������ꍇ
	//! @retval false �c 2�̃J���[��f���قȂ�ꍇ
	//! 
	bool operator ==( const rgba &c ) const { return( base::operator ==( c ) ); }

	/// @brief 2�̃J���[��f���������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_r = q_r \; \wedge \; p_g = q_g \; \wedge \; p_b = q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̃J���[��f���قȂ�ꍇ
	//! @retval false �c 2�̃J���[��f���������ꍇ
	//! 
	bool operator !=( const rgba &c ) const { return( !( *this == c ) ); }

	/// @brief 2�̃J���[��f�� < �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <  c2 �̏ꍇ
	//! @retval false �c c1 >= c2 �̏ꍇ
	//! 
	bool operator < ( const rgba &c ) const { return( !( *this >= c ) ); }

	/// @brief 2�̃J���[��f�� <= �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 <= c2 �̏ꍇ
	//! @retval false �c c1 >  c2 �̏ꍇ
	//! 
	bool operator <=( const rgba &c ) const { return( c >= *this ); }

	/// @brief 2�̃J���[��f�� > �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_r \le q_r \; \wedge \; p_g \le q_g \; \wedge \; p_b \le q_b }
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >  c2 �̏ꍇ
	//! @retval false �c c1 <= c2 �̏ꍇ
	//! 
	bool operator > ( const rgba &c ) const { return( c < *this ); }

	/// @brief 2�̃J���[��f�� >= �𔻒肷��
	//! 
	//! \f[
	//! 	\mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_r \ge q_r \; \wedge \; p_g \ge q_g \; \wedge \; p_b \ge q_b
	//! \f]
	//! 
	//! @param[in] c �c �E�Ӓl
	//! 
	//! @retval true  �c c1 >= c2 �̏ꍇ
	//! @retval false �c c1 <  c2 �̏ꍇ
	//! 
	bool operator >=( const rgba &c ) const { return( base::operator >=( c ) ); }

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


namespace __color_utility__
{
	template < class T > inline T maximum( const T &v1, const T &v2 )
	{
		return( v1 > v2 ? v1 : v2 );
	}

	template < class T > inline T minimum( const T &v1, const T &v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}
}


/// @brief RGB�F��Ԃ�HSV�F��Ԃɕϊ�����
//! 
//! @param[in]  r �c RGB�F��Ԃ�R(��)�����i�O�`�Q�T�T�j
//! @param[in]  g �c RGB�F��Ԃ�G(��)�����i�O�`�Q�T�T�j
//! @param[in]  b �c RGB�F��Ԃ�B(��)�����i�O�`�Q�T�T�j
//! @param[out] h �c HSV�F��Ԃ�H(�F��)�����i�O�`�R�U�O�j
//! @param[out] s �c HSV�F��Ԃ�S(�ʓx)�����i�O�`�P�j
//! @param[out] v �c HSV�F��Ԃ�V(���x��)�����i�O�`�P�j
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
//! @param[in]  h �c HSV�F��Ԃ�H(�F��)�����i�O�`�R�U�O�j
//! @param[in]  s �c HSV�F��Ԃ�S(�ʓx)�����i�O�`�P�j
//! @param[in]  v �c HSV�F��Ԃ�V(���x��)�����i�O�`�P�j
//! @param[out] r �c RGB�F��Ԃ�R(��)�����i�O�`�Q�T�T�j
//! @param[out] g �c RGB�F��Ԃ�G(��)�����i�O�`�Q�T�T�j
//! @param[out] b �c RGB�F��Ԃ�B(��)�����i�O�`�Q�T�T�j
//! 
inline void hsv2rgb( double h, double s, double v, double &r, double &g, double &b )
{
	if( s == 0.0 )
	{
		r = g = b = v;
	}
	else
	{
		int ht = static_cast< int >( h * 6.0 );
		int d  = ht % 360;

		ht /= 360;

		double t1 = v * ( 255.0 - s ) / 255.0;
		double t2 = v * ( 255.0 - s * d / 360.0 ) / 255.0;
		double t3 = v * ( 255.0 - s * ( 360.0 - d ) / 360.0 ) / 255.0;

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
	}
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

	static value_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
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
	struct _pixel_converter_< rgb< T > >
	{
		typedef T value_type;
		typedef rgba< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b, value_type a = 255 )
		{
			return( color_type( r, g, b, a ) );
		}

		static color_type convert_from( const color_type &pixel )
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

		static color_type convert_from( const color_type &pixel )
		{
			return( pixel );
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

		static color_type convert_from( const color_type &pixel )
		{
			return( pixel );
		}
	};

#endif


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_COLOR_H__
