/// @file mist/config/color.h
//!
//! @brief �J���[�f�[�^���������߂̃N���X�Q
//!

#ifndef __INCLUDE_MIST_COLOR_H__
#define __INCLUDE_MIST_COLOR_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// MIST�ŗ��p������̃f�[�^�^


/// @brief �J���[�摜�p
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param T �c �����̐���
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
	value_type r;		///< @brief �ϐ��̐���������
	value_type g;		///< @brief �ϐ��̐���������
	value_type b;		///< @brief �ϐ��̐���������

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] in  �c �����̐���
	//! @param[in] out �c �����̐���
	//! 
	rgb( ) : r( 0 ), g( 0 ), b( 0 ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pix �c �����̐���
	//! 
	explicit rgb( const value_type &pix ) : r( pix ), g( pix ), b( pix ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	rgb( const rgb &c ) : r( c.r ), g( c.g ), b( c.b ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] rr �c �����̐���
	//! @param[in] gg �c �����̐���
	//! @param[in] bb �c �����̐���
	//! 
	rgb( const value_type &rr, const value_type &gg, const value_type &bb ) : r( rr ), g( gg ), b( bb ){ }



	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator  =( const rgb &c )
	{
		if( &c != this )
		{
			r = c.r;
			g = c.g;
			b = c.b;
		}
		return( *this );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pix �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator  =( const value_type &pix )
	{
		r = pix;
		g = pix;
		b = pix;
		return( *this );
	}


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb  operator -( ) const { return( rgb( -r, -g, -b ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator +=( const rgb &c ){ r += c.r; g += c.g; b += c.b; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator -=( const rgb &c ){ r -= c.r; g -= c.g; b -= c.b; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator *=( const rgb &c ){ r *= c.r; g *= c.g; b *= c.b; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator /=( const rgb &c ){ r /= c.r; g /= c.g; b /= c.b; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator %=( const rgb &c ){ r %= c.r; g %= c.g; b %= c.b; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator |=( const rgb &c ){ r |= c.r; g |= c.g; b |= c.b; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator &=( const rgb &c ){ r &= c.r; g &= c.g; b &= c.b; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator ^=( const rgb &c ){ r ^= c.r; g ^= c.g; b ^= c.b; return( *this ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pix �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator +=( const value_type &pix ){ r += pix; g += pix; b += pix; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pix �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator -=( const value_type &pix ){ r -= pix; g -= pix; b -= pix; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pix �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator *=( const value_type &pix ){ r *= pix; g *= pix; b *= pix; return( *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pix �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	const rgb &operator /=( const value_type &pix ){ r /= pix; g /= pix; b /= pix; return( *this ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && b == c.b ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator < ( const rgb &c ) const { return( !( *this >= c ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator <=( const rgb &c ) const { return( c >= *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator > ( const rgb &c ) const { return( c < *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] c �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator >=( const rgb &c ) const { return( r >= c.r && g >= c.g && b >= c.b ); }


	/// @brief NTSC�n���d���ϖ@�ɂ��C�O���[�X�P�[���֕ϊ�����
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @return �߂�l�̐���
	//! 
	value_type get_value( ) const
	{
		return( static_cast< value_type >( r * 0.298912 + g * 0.586610 + b * 0.114478 ) );
	}

	// �J���[����O���[�X�P�[���ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator value_type( ) const { return( get_value( ) ); }
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator +( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) += c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator -( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) -= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator *( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) *= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator /( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) /= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator %( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) %= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator |( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) |= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator &( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) &= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator ^( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) ^= c2 ); }



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator *( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) *= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator *( const typename rgb< T >::value_type &c1, const rgb< T > &c2 ){ return( rgb< T >( c2 ) *= c1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator /( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) /= c2 ); }



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator +( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) += c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator +( const typename rgb< T >::value_type &c1, const rgb< T > &c2 ){ return( rgb< T >( c2 ) += c1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator -( const rgb< T > &c1, const typename rgb< T >::value_type &c2 ){ return( rgb< T >( c1 ) -= c2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] c1 �c �����̐���
//! @param[in] c2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template < class T > inline const rgb< T > operator -( const typename rgb< T >::value_type &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) -= c2 ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const rgb< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << " )";
	return( out );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
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
