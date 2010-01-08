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

/// @file mist/config/set.h
//!
//! @brief �W�����Z���������߂̃��C�u����
//!

#ifndef __INCLUDE_MIST_SET_H__
#define __INCLUDE_MIST_SET_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <set>
#include <algorithm>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @addtogroup set_group �W�����Z
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/config/set.h>
//! @endcode
//!
//!  @{


/// @brief �W�����������߂̊�{�N���X
//! 
//! @param SetType �c ���N���X�istd::set �� std::multiset�j
//! 
template< class SetType >
class set_base : public SetType
{
protected:
	typedef SetType base;			///< @brief ���N���X�istd::set �� std::multiset�j

public:
	typedef typename base::key_type key_type;					///< @brief ����������
	typedef typename base::key_compare key_compare;				///< @brief ����������
	typedef typename base::value_compare value_compare;			///< @brief ����������
	typedef typename base::allocator_type allocator_type;		///< @brief STL�R���e�i�����p����A���P�[�^�^
	typedef typename base::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

#if __MIST_MSVC__ != 6
	typedef typename base::pointer pointer;						///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dstd::set< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename base::const_pointer const_pointer;			///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dstd::set< data > �̏ꍇ�Cconst data * �ƂȂ�
#endif

	typedef typename base::reference reference;								///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDstd::set< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename base::const_reference const_reference;					///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDstd::set< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename base::iterator iterator;								///< @brief STL�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_iterator const_iterator;					///< @brief STL�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief STL�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief STL�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename base::value_type value_type;							///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�Dstd::set< data > �� data �Ɠ���


public:
	/// @brief �f�t�H���g�R���X�g���N�^�i��W�����쐬����j
	set_base( ) : base( ){ }

	/// @brief �W���̗v�f�̔�r���Z�q�Ƃ��� pred ��p����
	explicit set_base( const key_compare &pred ) : base( pred ){ }

	/// @brief 1�v�f key �����W�����쐬����
	explicit set_base( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief �W���̗v�f�̔�r���Z�q�Ƃ��� pred ��p���Calloc �̃R�s�[�𗘗p����
	set_base( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬����
	template< class Iterator >
	set_base( Iterator first, Iterator last ) : base( first, last ){ }


	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬���C�v�f�̔�r���Z�q�Ƃ��� pred ��p����
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }


	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬���C�v�f�̔�r���Z�q�Ƃ��� pred ��p���Calloc �̃R�s�[�𗘗p����
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }

public:
	/// @brief �W���a
	//! 
	//! \f[ a = a \bigcup b \f]
	//! 
	set_base &operator +=( const set_base &s )
	{
#if __MIST_MSVC__ != 6
		base::insert( s.begin( ), s.end( ) );
#else
		// MSVC6 �t����STL��SET�ł̓C�e���[�^�̎������قȂ�
		const_iterator site = s.begin( );
		const_iterator eite = s.end( );
		for( ; site != eite ; ++site )
		{
			base::insert( *site );
		}
#endif
		return( *this );
	}

	/// @brief �W����1�v�f�ǉ�����
	set_base &operator +=( const key_type &s ){ base::insert( s ); return( *this ); }


	/// @brief ���W�����v�Z����
	//! 
	//! \f[ a = a - \left( a \bigcap b \right) \f]
	//! 
	set_base &operator -=( const set_base &s )
	{
		if( &s == this )
		{
			base::clear( );
		}
		else
		{
			set_base a = *this;
			const_iterator site1 = a.begin( );
			const_iterator eite1 = a.end( );
			const_iterator site2 = s.begin( );
			const_iterator eite2 = s.end( );

			// �������g�̓��e������ۂɂ���
			base::clear( );

			key_compare compare;
			while( site1 != eite1 && site2 != eite2 )
			{
				if( compare( *site1, *site2 ) )
				{
					base::insert( *site1 );
					++site1;
				}
				else if( compare( *site2, *site1 ) )
				{
					++site2;
				}
				else
				{
					++site1;
					++site2;
				}
			}

			if( site1 != eite1 )
			{
#if __MIST_MSVC__ != 6
				base::insert( site1, eite1 );
#else
				// MSVC6 �t����STL��SET�ł̓C�e���[�^�̎������قȂ�
				for( ; site1 != eite1 ; ++site1 )
				{
					base::insert( *site1 );
				}
#endif
			}
		}
		return( *this );
	}

	/// @brief �W������1�v�f��菜��
	set_base &operator -=( const key_type &s ){ base::erase( s ); return( *this ); }


	/// @brief �ϏW�����v�Z����
	//! 
	//! \f[ a = a \bigcap b \f]
	//! 
	set_base &operator *=( const set_base &s )
	{
		if( &s != this )
		{
			set_base a = *this;
			const_iterator site1 = a.begin( );
			const_iterator eite1 = a.end( );
			const_iterator site2 = s.begin( );
			const_iterator eite2 = s.end( );

			// �������g�̓��e������ۂɂ���
			base::clear( );

			key_compare compare;
			while( site1 != eite1 && site2 != eite2 )
			{
				if( compare( *site1, *site2 ) )
				{
					++site1;
				}
				else if ( compare( *site2, *site1 ) )
				{
					++site2;
				}
				else
				{
					base::insert( *site1 );
					++site1;
					++site2;
				}
			}
		}
		return( *this );
	}


	/// @brief �W����1�v�f�̏W���Ԃ̐ς����
	set_base &operator *=( const key_type &s )
	{
		set_base a = *this;
		const_iterator site = a.begin( );
		const_iterator eite = a.end( );

		// �������g�̓��e������ۂɂ���
		base::clear( );

		key_compare compare;
		while( site != eite )
		{
			if( compare( *site, s ) )
			{
				++site;
			}
			else if ( compare( s, *site ) )
			{
				break;
			}
			else
			{
				base::insert( *site );
				++site;
			}
		}
		return( *this );
	}


	/// @brief �W���ɏW����ǉ�����
	set_base &operator <<=( const set_base &s ){ return( operator +=( s ) ); }

	/// @brief �W����1�v�f��ǉ�����
	set_base &operator <<=( const key_type &s ){ return( operator +=( s ) ); }


	/// @brief �W����1�v�f�ǉ������W�����o�͂���
	set_base &operator <<( const key_type &s ) const { return( set_base( *this )+=( s ) ); }


	/// @brief �S�Ă̗v�f�����������ǂ����𔻒肷��i\f$ a = b \f$�j
	bool operator ==( const set_base &s ) const
	{
		if( base::size( ) != s.size( ) )
		{
			return( false );
		}

		key_compare compare;
		const_iterator ite1 = base::begin( );
		const_iterator eite1 = base::end( );
		const_iterator ite2 = s.begin( );
		for( ; ite1 != eite1 ; ++ite1, ++ite2 )
		{
			if( compare( *ite1, *ite2 ) || compare( *ite2, *ite1 ) )
			{
				return( false );
			}
		}
		return( true );
	}


	/// @brief �S�Ă̗v�f�����������ǂ����𔻒肷��i1�v�f�Łj
	bool operator ==( const key_type &s ) const
	{
		if( base::size( ) != 1 )
		{
			return( false );
		}

		key_compare compare;
		const_iterator ite = base::begin( );
		const_iterator eite = base::end( );
		for( ; ite != eite ; ++ite )
		{
			if( compare( *ite, s ) || compare( s, *ite ) )
			{
				return( false );
			}
		}
		return( true );
	}


	/// @brief �S�Ă̗v�f���������Ȃ����ǂ����𔻒肷��i\f$ a \neq b \f$�j
	bool operator !=( const set_base &s ) const { return( !( *this == s ) ); }

	/// @brief �S�Ă̗v�f���������Ȃ����ǂ����𔻒肷��i1�v�f�Łj
	bool operator !=( const key_type &s ) const { return( !( *this == s ) ); }

	/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \subset b \f$�j
	bool operator < ( const set_base &s ) const
	{
		if( base::size( ) >= s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \subseteq b \f$�j
	bool operator <=( const set_base &s ) const
	{
		if( base::size( ) > s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \supset b \f$�j
	bool operator > ( const set_base &s ) const { return( s < *this ); }

	/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \supseteq b \f$�j
	bool operator >=( const set_base &s ) const { return( s <= *this ); }
};


/// @brief �a�W�����v�Z����
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief ���W�����v�Z����
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief �ϏW�����v�Z����
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }


//inline const set_base operator /( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) /= s2 ); }
//inline const set_base operator %( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) %= s2 ); }
//inline const set_base operator |( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) |= s2 ); }
//inline const set_base operator &( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) &= s2 ); }
//inline const set_base operator ^( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) ^= s2 ); }



/// @brief �W����1�v�f�̐ς��v�Z����
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief 1�v�f�ƏW���̐ς��v�Z����
template< class SetType > inline const set_base< SetType > operator *( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief �W����1�v�f�̘a���v�Z����
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief 1�v�f�ƏW���̘a���v�Z����
template< class SetType > inline const set_base< SetType > operator +( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s2 ) += s1 ); }

/// @brief �W����1�v�f�̍����v�Z����
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief 1�v�f�ƏW���̍����v�Z����
template< class SetType > inline const set_base< SetType > operator -( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }



/// @brief �S�Ă̗v�f�����������ǂ����𔻒肷��i\f$ a = b \f$�j
template< class SetType > bool operator ==( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) == s1 ); }

/// @brief �S�Ă̗v�f���������Ȃ����ǂ����𔻒肷��i\f$ a \neq b \f$�j
template< class SetType > bool operator !=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) != s1 ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \subset b \f$�j
template< class SetType > bool operator < ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <  set_base< SetType >( s2 ) ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \subseteq b \f$�j
template< class SetType > bool operator <=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <= set_base< SetType >( s2 ) ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \supset b \f$�j
template< class SetType > bool operator > ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >  set_base< SetType >( s2 ) ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \supseteq b \f$�j
template< class SetType > bool operator >=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >= set_base< SetType >( s2 ) ); }



/// @brief �S�Ă̗v�f�����������ǂ����𔻒肷��i\f$ a = b \f$�j
template< class SetType > bool operator ==( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) == s2 ); }

/// @brief �S�Ă̗v�f���������Ȃ����ǂ����𔻒肷��i\f$ a \neq b \f$�j
template< class SetType > bool operator !=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) != s2 ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \subset b \f$�j
template< class SetType > bool operator < ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <  s2 ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \subseteq b \f$�j
template< class SetType > bool operator <=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <= s2 ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \supset b \f$�j
template< class SetType > bool operator > ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >  s2 ); }

/// @brief �W���̕�܊֌W�𒲂ׂ�i\f$ a \supseteq b \f$�j
template< class SetType > bool operator >=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >= s2 ); }


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     s   �c �W��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, ... , n
//! @endcode
//! 
template< class SetType >
inline std::ostream &operator <<( std::ostream &out, const set_base< SetType > &s )
{
	typename set_base< SetType >::const_iterator ite = s.begin( );
	if( ite != s.end( ) )
	{
		out << *ite++;
	}

	for( ; ite != s.end( ) ; ++ite )
	{
		out << ", " << *ite;
	}
	return( out );
}



/// @brief �W�����Z���s�����Ƃ��ł���CSTL�� set �̊g����
//! 
//! �v�f�̏d���������Ȃ��W��������
//! 
//! @param Key       �c �W���̊e�v�f��\���^
//! @param Comp      �c �v�f�Ԃ̑召�֌W��\����r�֐��i�ȗ������ < ���K�p�����j
//! @param Allocator �c �A���P�[�^�i�ȗ������STL�̃f�t�H���g�A���P�[�^���K�p�����j
//! 
template< class Key, class Comp = std::less< Key >, class Allocator = std::allocator< Key > >
class set : public set_base< std::set< Key, Comp, Allocator > >
{
protected:
	typedef set_base< std::set< Key, Comp, Allocator > > base;		///< @brief ����������

public:
	typedef typename base::key_type key_type;					///< @brief ����������
	typedef typename base::key_compare key_compare;				///< @brief ����������
	typedef typename base::value_compare value_compare;			///< @brief ����������
	typedef typename base::allocator_type allocator_type;		///< @brief STL�R���e�i�����p����A���P�[�^�^
	typedef typename base::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

#if __MIST_MSVC__ != 6
	typedef typename base::pointer pointer;						///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dstd::set< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename base::const_pointer const_pointer;			///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dstd::set< data > �̏ꍇ�Cconst data * �ƂȂ�
#endif

	typedef typename base::reference reference;								///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDstd::set< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename base::const_reference const_reference;					///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDstd::set< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename base::iterator iterator;								///< @brief STL�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_iterator const_iterator;					///< @brief STL�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief STL�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief STL�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename base::value_type value_type;							///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�Dstd::set< data > �� data �Ɠ���

public:
	/// @brief �f�t�H���g�R���X�g���N�^�i��W�����쐬����j
	set( ) : base( ){ }

	/// @brief �W���̗v�f�̔�r���Z�q�Ƃ��� pred ��p����
	explicit set( const key_compare &pred ) : base( pred ){ }

	/// @brief 1�v�f key �����W�����쐬����
	explicit set( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief �W���̗v�f�̔�r���Z�q�Ƃ��� pred ��p���Calloc �̃R�s�[�𗘗p����
	set( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬����
	template< class Iterator >
	set( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬���C�v�f�̔�r���Z�q�Ƃ��� pred ��p����
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬���C�v�f�̔�r���Z�q�Ƃ��� pred ��p���Calloc �̃R�s�[�𗘗p����
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};



/// @brief �W�����Z���s�����Ƃ��ł���CSTL�� set �̊g����
//! 
//! �v�f�̏d���������W��������
//! 
//! @param Key       �c �W���̊e�v�f��\���^
//! @param Comp      �c �v�f�Ԃ̑召�֌W��\����r�֐��i�ȗ������ < ���K�p�����j
//! @param Allocator �c �A���P�[�^�i�ȗ������STL�̃f�t�H���g�A���P�[�^���K�p�����j
//! 
template< class Key, class Comp = std::less< Key >, class Allocator = std::allocator< Key > >
class multiset : public set_base< std::multiset< Key, Comp, Allocator > >
{
protected:
	typedef set_base< std::multiset< Key, Comp, Allocator > > base;

public:
	typedef typename base::key_type key_type;					///< @brief ����������
	typedef typename base::key_compare key_compare;				///< @brief ����������
	typedef typename base::value_compare value_compare;			///< @brief ����������
	typedef typename base::allocator_type allocator_type;		///< @brief STL�R���e�i�����p����A���P�[�^�^
	typedef typename base::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

#if __MIST_MSVC__ != 6
	typedef typename base::pointer pointer;						///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dstd::set< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename base::const_pointer const_pointer;			///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dstd::set< data > �̏ꍇ�Cconst data * �ƂȂ�
#endif

	typedef typename base::reference reference;								///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDstd::set< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename base::const_reference const_reference;					///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDstd::set< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename base::iterator iterator;								///< @brief STL�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_iterator const_iterator;					///< @brief STL�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief STL�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief STL�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef typename base::value_type value_type;							///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�Dstd::set< data > �� data �Ɠ���

public:
	/// @brief �f�t�H���g�R���X�g���N�^�i��W�����쐬����j
	multiset( ) : base( ){ }

	/// @brief �W���̗v�f�̔�r���Z�q�Ƃ��� pred ��p����
	explicit multiset( const key_compare &pred ) : base( pred ){ }

	/// @brief 1�v�f key �����W�����쐬����
	explicit multiset( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief �W���̗v�f�̔�r���Z�q�Ƃ��� pred ��p���Calloc �̃R�s�[�𗘗p����
	multiset( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬����
	template< class Iterator >
	multiset( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬���C�v�f�̔�r���Z�q�Ƃ��� pred ��p����
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief first �� last �̊Ԃ̗v�f��p���ďW�����쐬���C�v�f�̔�r���Z�q�Ƃ��� pred ��p���Calloc �̃R�s�[�𗘗p����
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};



/// @}
//  �W�����Z�O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_SET_H__
