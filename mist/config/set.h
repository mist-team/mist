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


// MIST�ŗ��p������̃f�[�^�^


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param SetType �c �����̐���
//! 
template< class SetType >
class set_base : public SetType
{
protected:
	typedef SetType base;			///< @brief ����������

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
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	set_base( ) : base( ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pred �c �����̐���
	//! 
	explicit set_base( const key_compare &pred ) : base( pred ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] key �c �����̐���
	//! 
	explicit set_base( const key_type &key ) : base( ){ base::insert( key ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pred  �c �����̐���
	//! @param[in] alloc �c �����̐���
	//! 
	set_base( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! 
	template< class Iterator >
	set_base( Iterator first, Iterator last ) : base( first, last ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! @param[in] pred  �c �����̐���
	//! 
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! @param[in] pred  �c �����̐���
	//! @param[in] alloc �c �����̐���
	//! 
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }

public:
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
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

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	set_base &operator +=( const key_type &s ){ base::insert( s ); return( *this ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
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

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	set_base &operator -=( const key_type &s ){ base::erase( s ); return( *this ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	set_base &operator <<=( const set_base &s ){ return( operator +=( s ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	set_base &operator <<=( const key_type &s ){ return( operator +=( s ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @return �߂�l�̐���
	//! 
	set_base &operator <<( const key_type &s ){ return( operator +=( s ) ); }


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
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


	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator !=( const set_base &s ) const { return( !( *this == s ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator !=( const key_type &s ) const { return( !( *this == s ) ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator < ( const set_base &s ) const
	{
		if( base::size( ) >= s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator <=( const set_base &s ) const
	{
		if( base::size( ) > s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator > ( const set_base &s ) const { return( s < *this ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] s �c �����̐���
	//! 
	//! @retval true  �c �����̐���
	//! @retval false �c �����̐���
	//! 
	bool operator >=( const set_base &s ) const { return( s <= *this ); }
};


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }


//inline const set_base operator /( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) /= s2 ); }
//inline const set_base operator %( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) %= s2 ); }
//inline const set_base operator |( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) |= s2 ); }
//inline const set_base operator &( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) &= s2 ); }
//inline const set_base operator ^( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) ^= s2 ); }



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator *( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator +( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s2 ) += s1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @return �߂�l�̐���
//! 
template< class SetType > inline const set_base< SetType > operator -( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator ==( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) == s1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator !=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) != s1 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator < ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <  set_base< SetType >( s2 ) ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator <=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <= set_base< SetType >( s2 ) ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator > ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >  set_base< SetType >( s2 ) ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator >=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >= set_base< SetType >( s2 ) ); }



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator ==( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) == s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator !=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) != s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator < ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <  s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator <=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <= s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator > ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >  s2 ); }

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] s1 �c �����̐���
//! @param[in] s2 �c �����̐���
//! 
//! @retval true  �c �߂�l�̐���
//! @retval false �c �߂�l�̐���
//! 
template< class SetType > bool operator >=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >= s2 ); }


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in,out] out �c �����̐���
//! @param[in]     s   �c �����̐���
//! 
//! @return �߂�l�̐���
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



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param Key       �c �����̐���
//! @param Comp      �c �����̐���
//! @param Allocator �c �����̐���
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
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	set( ) : base( ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pred �c �����̐���
	//! 
	explicit set( const key_compare &pred ) : base( pred ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] key �c �����̐���
	//! 
	explicit set( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pred  �c �����̐���
	//! @param[in] alloc �c �����̐���
	//! 
	set( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! 
	template< class Iterator >
	set( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! @param[in] pred  �c �����̐���
	//! 
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! @param[in] pred  �c �����̐���
	//! @param[in] alloc �c �����̐���
	//! 
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param Key       �c �����̐���
//! @param Comp      �c �����̐���
//! @param Allocator �c �����̐���
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
	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	multiset( ) : base( ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pred  �c �����̐���
	//! 
	explicit multiset( const key_compare &pred ) : base( pred ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] key �c �����̐���
	//! 
	explicit multiset( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] pred  �c �����̐���
	//! @param[in] alloc �c �����̐���
	//! 
	multiset( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! 
	template< class Iterator >
	multiset( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! @param[in] pred  �c �����̐���
	//! 
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief �֐��E�N���X�̊T�v������
	//! 
	//! �ڍׂȐ�����֐��̎g�p�������
	//! 
	//! @param[in] first �c �����̐���
	//! @param[in] last  �c �����̐���
	//! @param[in] pred  �c �����̐���
	//! @param[in] alloc �c �����̐���
	//! 
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_SET_H__
