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
	template < class TT >
	const rgb &operator +=( const TT &pix ){ r = static_cast< value_type >( r + pix ); g = static_cast< value_type >( g + pix ); b = static_cast< value_type >( b + pix ); return( *this ); }

	/// @brief RGB�������� pix �l������
	template < class TT >
	const rgb &operator -=( const TT &pix ){ r = static_cast< value_type >( r - pix ); g = static_cast< value_type >( g - pix ); b = static_cast< value_type >( b - pix ); return( *this ); }

	/// @brief RGB������ pix �l���|����
	template < class TT >
	const rgb &operator *=( const TT &pix ){ r = static_cast< value_type >( r * pix ); g = static_cast< value_type >( g * pix ); b = static_cast< value_type >( b * pix ); return( *this ); }

	/// @brief RGB������ pix �l�Ŋ���
	template < class TT >
	const rgb &operator /=( const TT &pix ){ r = static_cast< value_type >( r / pix ); g = static_cast< value_type >( g / pix ); b = static_cast< value_type >( b / pix ); return( *this ); }


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


DEFINE_PROMOTE_BIND_OPERATOR1( rgb, + )			///< @brief �J���[��f�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, + )			///< @brief �J���[��f�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, + )			///< @brief �萔�ƃJ���[��f�̘a

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, - )			///< @brief �J���[��f�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, - )			///< @brief �J���[��f�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR4( rgb, - )			///< @brief �萔�ƃJ���[��f�̍�

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, * )			///< @brief �J���[��f�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, * )			///< @brief �J���[��f�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR3( rgb, * )			///< @brief �萔�ƃJ���[��f�̐�

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, / )			///< @brief �J���[��f�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR2( rgb, / )			///< @brief �J���[��f��萔�Ŋ���

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, % )			///< @brief �J���[��f�̏�]

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, | )			///< @brief �J���[��f�� | ���Z

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, & )			///< @brief �J���[��f�� & ���Z

DEFINE_PROMOTE_BIND_OPERATOR1( rgb, ^ )			///< @brief �J���[��f�� ^ ���Z


DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, == )	///< @brief ��r���Z�q ==
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, != )	///< @brief ��r���Z�q !=
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, <  )	///< @brief ��r���Z�q <
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, <= )	///< @brief ��r���Z�q <=
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, >  )	///< @brief ��r���Z�q >
DEFINE_PROMOTE_CONDITION_OPERATOR( rgb, >= )	///< @brief ��r���Z�q >=



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


/// @brief ���͂��ꂽ�^���J���[�摜���ǂ������ׂ�
template < class T >
struct is_color
{
	_MIST_CONST( bool, value, false );
};


// ��f�̕ϊ����T�|�[�g���邽�߂̃R���o�[�^
template < class T >
struct _pixel_converter_
{
	typedef T value_type;
	typedef rgb< T > color_type;
	enum{ color_num = 1 };

	static value_type convert_to( value_type r, value_type g, value_type b )
	{
		return( color_type( r, g, b ).get_value( ) );
	}

	static color_type convert_from( const value_type &pixel )
	{
		return( color_type( pixel, pixel, pixel ) );
	}
};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_COLOR( type ) \
		template < >\
		struct is_color< rgb< type > >\
		{\
			enum{ value = true };\
		};\


	#define __PIXEL_CONVERTER__( type ) \
		template < >\
		struct _pixel_converter_< rgb< type > >\
		{\
			typedef type value_type;\
			typedef rgb< type > color_type;\
			enum{ color_num = 3 };\
			\
			static color_type convert_to( value_type r, value_type g, value_type b )\
			{\
				return( color_type( r, g, b ) );\
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
	struct _pixel_converter_< rgb< T > >
	{
		typedef T value_type;
		typedef rgb< T > color_type;
		enum{ color_num = 3 };

		static color_type convert_to( value_type r, value_type g, value_type b )
		{
			return( color_type( r, g, b ) );
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
