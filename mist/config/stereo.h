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

/// @file mist/config/stereo.h
//!
//! @brief �X�e���I�������������߂̃N���X
//!

#ifndef __INCLUDE_MIST_STEREO__
#define __INCLUDE_MIST_STEREO__


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


/// @brief �X�e���I�����p�̃f�[�^�^
//! 
//! @code �X�e���I�����̍쐬��
//! mist::array< mist::stereo< double > > sound;
//! @endcode
//! 
//! @param T �c �e���������̃f�[�^�^
//! 
template< class T >
struct stereo
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
	value_type l;		///< @brief Left�`�����l����
	value_type r;		///< @brief Right�`�����l��

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�Ă̗v�f��0�ŏ���������j
	stereo( ) : l( 0 ), r( 0 ){ }

	/// @brief �S�Ă̐����� mono �ŏ���������
	stereo( const value_type &mono ) : l( mono ), r( mono ){ }

	/// @brief �� left�C�E right ��p���ď���������
	stereo( const value_type &left, const value_type &right ) : l( left ), r( right ){ }

	/// @brief �قȂ�^�̃X�e���I�����f�[�^��p���ď���������
	template < class TT >
	stereo( const stereo< TT > &s ) : l( static_cast< value_type >( s.l ) ), r( static_cast< value_type >( s.r ) ){ }

	/// @brief ���̃X�e���I�����f�[�^��p���ď���������
	stereo( const stereo< T > &s ) : l( s.l ), r( s.r ){ }



	/// @brief ���̉���������������
	template < class TT >
	const stereo &operator =( const stereo< TT > &s )
	{
		l = static_cast< value_type >( s.l );
		r = static_cast< value_type >( s.r );
		return( *this );
	}

	/// @brief ���̉���������������
	const stereo &operator =( const stereo< T > &s )
	{
		if( &s != this )
		{
			l = s.l;
			r = s.r;
		}
		return( *this );
	}


	/// @brief �S�Ă̗v�f�� mono ��������
	const stereo &operator =( const value_type &mono )
	{
		l = mono;
		r = mono;
		return( *this );
	}


	/// @brief �e���������̘a
	template < class TT >
	const stereo &operator +=( const stereo< TT > &s ){ l = static_cast< value_type >( l + s.l ); r = static_cast< value_type >( r + s.r ); return( *this ); }

	/// @brief �e���������̍�
	template < class TT >
	const stereo &operator -=( const stereo< TT > &s ){ l = static_cast< value_type >( l - s.l ); r = static_cast< value_type >( r - s.r ); return( *this ); }

	/// @brief �e���������̐�
	template < class TT >
	const stereo &operator *=( const stereo< TT > &s ){ l = static_cast< value_type >( l * s.l ); r = static_cast< value_type >( r * s.r ); return( *this ); }

	/// @brief �e���������̊���Z
	template < class TT >
	const stereo &operator /=( const stereo< TT > &s ){ l = static_cast< value_type >( l / s.l ); r = static_cast< value_type >( r / s.r ); return( *this ); }

	/// @brief �e���������̏�]
	const stereo &operator %=( const stereo &s ){ l %= s.l; r %= s.r; return( *this ); }

	/// @brief �e���������� | ���Z
	const stereo &operator |=( const stereo &s ){ l |= s.l; r |= s.r; return( *this ); }

	/// @brief �e���������� & ���Z
	const stereo &operator &=( const stereo &s ){ l &= s.l; r &= s.r; return( *this ); }

	/// @brief �e���������� & ���Z
	const stereo &operator ^=( const stereo &s ){ l ^= s.l; r ^= s.r; return( *this ); }



	/// @brief �e���������� mono �l�𑫂�
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator +=( const double &mono )
#else
	template < class TT >
	const stereo &operator +=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l + mono );
		r = static_cast< value_type >( r + mono );
		return( *this );
	}

	/// @brief �e������������ mono �l������
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator -=( const double &mono )
#else
	template < class TT >
	const stereo &operator -=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l - mono );
		r = static_cast< value_type >( r - mono );
		return( *this );
	}

	/// @brief �e���������� mono �l���|����
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator *=( const double &mono )
#else
	template < class TT >
	const stereo &operator *=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l * mono );
		r = static_cast< value_type >( r * mono );
		return( *this );
	}

	/// @brief �e���������� mono �l�Ŋ���
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	const stereo &operator /=( const double &mono )
#else
	template < class TT >
	const stereo &operator /=( const TT &mono )
#endif
	{
		l = static_cast< value_type >( l / mono );
		r = static_cast< value_type >( r / mono );
		return( *this );
	}


	/// @brief 2�̉����������������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} = \mbox{\boldmath q} \rightarrow p_l = q_l \; \wedge \; p_r = q_r \f]
	//! 
	//! @param[in] s �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̉����������������ꍇ
	//! @retval false �c 2�̉����������قȂ�ꍇ
	//! 
	bool operator ==( const stereo &s ) const { return( l == s.l && r == s.r ); }

	/// @brief 2�̉����������������Ȃ��i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \neq \mbox{\boldmath q} \rightarrow \overline{ p_l = q_l \; \wedge \; p_r = q_r } \f]
	//! 
	//! @param[in] s �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̉����������قȂ�ꍇ
	//! @retval false �c 2�̉����������������ꍇ
	//! 
	bool operator !=( const stereo &s ) const { return( !( *this == s ) ); }

	/// @brief 2�̉��������� < �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow \overline{ p_l \ge q_l \; \wedge \; p_r \ge q_r } \f]
	//! 
	//! @param[in] s �c �E�Ӓl
	//! 
	//! @retval true  �c s1 <  s2 �̏ꍇ
	//! @retval false �c s1 >= s2 �̏ꍇ
	//! 
	bool operator < ( const stereo &s ) const
	{
		if( l == s.l )
		{
			return( r < s.r );
		}
		else
		{
			return( l < s.l );
		}
	}

	/// @brief 2�̉��������� <= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow p_l \le q_l \; \wedge \; p_r \le q_r \f]
	//! 
	//! @param[in] s �c �E�Ӓl
	//! 
	//! @retval true  �c s1 <= s2 �̏ꍇ
	//! @retval false �c s1 >  s2 �̏ꍇ
	//! 
	bool operator <=( const stereo &s ) const { return( s >= *this ); }

	/// @brief 2�̉��������� > �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \le \mbox{\boldmath q} \rightarrow \overline{ p_l \le q_l \; \wedge \; p_r \le q_r } \f]
	//! 
	//! @param[in] s �c �E�Ӓl
	//! 
	//! @retval true  �c s1 >  s2 �̏ꍇ
	//! @retval false �c s1 <= s2 �̏ꍇ
	//! 
	bool operator > ( const stereo &s ) const { return( s < *this ); }

	/// @brief 2�̉��������� >= �𔻒肷��
	//! 
	//! \f[ \mbox{\boldmath p} \ge \mbox{\boldmath q} \rightarrow p_l \ge q_l \; \wedge \; p_r \ge q_r \f]
	//! 
	//! @param[in] s �c �E�Ӓl
	//! 
	//! @retval true  �c s1 >= s2 �̏ꍇ
	//! @retval false �c s1 <  s2 �̏ꍇ
	//! 
	bool operator >=( const stereo &s ) const { return( !( *this < s ) ); }


	/// @brief �X�e���I�������烂�m���������ɕϊ�����
	value_type get_value( ) const
	{
		return( static_cast< value_type >( ( l + r ) / 2.0 ) );
	}

	// �X�e���I���烂�m�����ւ̎����L���X�g���Z�q�i�댯�̂��߈ꎞ��~�j
	//operator value_type( ) const { return( get_value( ) ); }

};

/// @brief �X�e���I�����f�[�^�̘a
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, + )

/// @brief �X�e���I�����f�[�^�ƒ萔�̘a
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, + )

/// @brief �萔�ƃX�e���I�����f�[�^�̘a
DEFINE_PROMOTE_BIND_OPERATOR3( stereo, + )

/// @brief �X�e���I�����f�[�^�̍�
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, - )

/// @brief �X�e���I�����f�[�^�ƒ萔�̍�
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, - )

/// @brief �萔�ƃX�e���I�����f�[�^�̍�
DEFINE_PROMOTE_BIND_OPERATOR4( stereo, - )

/// @brief �X�e���I�����f�[�^�̐�
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, * )

/// @brief �X�e���I�����f�[�^�ƒ萔�̐�
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, * )

/// @brief �萔�ƃX�e���I�����f�[�^�̐�
DEFINE_PROMOTE_BIND_OPERATOR3( stereo, * )

/// @brief �X�e���I�����f�[�^�̊���Z
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, / )

/// @brief �X�e���I�����f�[�^��萔�Ŋ���
DEFINE_PROMOTE_BIND_OPERATOR2( stereo, / )

/// @brief �X�e���I�����f�[�^�̏�]
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, % )

/// @brief �X�e���I�����f�[�^�� | ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, | )

/// @brief �X�e���I�����f�[�^�� & ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, & )

/// @brief �X�e���I�����f�[�^�� ^ ���Z
DEFINE_PROMOTE_BIND_OPERATOR1( stereo, ^ )



/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     s   �c �X�e���I����
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const stereo< T > &s )
{
	out << "( ";
	out << s.l << ", ";
	out << s.r << " )";
	return( out );
}


/// @brief ���͂��ꂽ�^�������f�[�^���ǂ������ׂ�
template < class T >
struct is_stereo
{
	_MIST_CONST( bool, value, false );
};


// ��f�̕ϊ����T�|�[�g���邽�߂̃R���o�[�^
template < class T >
struct _stereo_converter_
{
	typedef T value_type;
	typedef stereo< T > stereo_type;
	enum{ channel_num = 1 };

	static value_type convert_to( value_type l, value_type r )
	{
		return( stereo_type( l, r ).get_value( ) );
	}

	static stereo_type convert_from( const value_type &mono )
	{
		return( stereo_type( mono, mono ) );
	}
};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_STEREO( type ) \
		template < >\
		struct is_stereo< stereo< type > >\
		{\
			enum{ value = true };\
		};\


	#define __STEREO_CONVERTER__( type ) \
		template < >\
		struct _stereo_converter_< stereo< type > >\
		{\
			typedef type value_type;\
			typedef stereo< type > stereo_type;\
			enum{ channel_num = 2 };\
			\
			static stereo_type convert_to( value_type l, value_type r )\
			{\
				return( stereo_type( l, r ) );\
			}\
			\
			static stereo_type convert_from( const stereo_type &sound )\
			{\
				return( sound );\
			}\
		};\

		// �e�^�ɑ΂�����ꉻ
		IS_STEREO(unsigned char)
		IS_STEREO(unsigned short)
		IS_STEREO(unsigned int)
		IS_STEREO(unsigned long)
		IS_STEREO(signed char)
		IS_STEREO(signed short)
		IS_STEREO(signed int)
		IS_STEREO(signed long)
		IS_STEREO(bool)
		IS_STEREO(char)
		IS_STEREO(float)
		IS_STEREO(double)
		IS_STEREO(long double)
		__STEREO_CONVERTER__(unsigned char)
		__STEREO_CONVERTER__(unsigned short)
		__STEREO_CONVERTER__(unsigned int)
		__STEREO_CONVERTER__(unsigned long)
		__STEREO_CONVERTER__(signed char)
		__STEREO_CONVERTER__(signed short)
		__STEREO_CONVERTER__(signed int)
		__STEREO_CONVERTER__(signed long)
		__STEREO_CONVERTER__(bool)
		__STEREO_CONVERTER__(char)
		__STEREO_CONVERTER__(float)
		__STEREO_CONVERTER__(double)
		__STEREO_CONVERTER__(long double)

		#undef IS_STEREO
		#undef __STEREO_CONVERTER__

#else

	template < class T >
	struct is_stereo< stereo< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct _stereo_converter_< stereo< T > >
	{
		typedef T value_type;
		typedef stereo< T > stereo_type;
		enum{ channel_num = 2 };

		static stereo_type convert_to( value_type l, value_type r )
		{
			return( stereo_type( l, r ) );
		}

		static stereo_type convert_from( const stereo_type &sound )
		{
			return( sound );
		}
	};

#endif

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_STEREO__
