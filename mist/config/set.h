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

// set���Z���s�����߂̂���
template< class Key, class Comp = std::less< Key >, class Allocator = std::allocator< Key > >
class set : public std::set< Key, Comp, Allocator >
{
protected:
	typedef std::set< Key, Comp, Allocator > base;

public:
	typedef typename base::key_type key_type;
	typedef typename base::key_compare key_compare;
	typedef typename base::value_compare value_compare;
	typedef typename base::allocator_type allocator_type;
	typedef typename base::size_type size_type;
	typedef typename base::difference_type difference_type;
	typedef typename base::pointer pointer;
	typedef typename base::const_pointer const_pointer;
	typedef typename base::reference reference;
	typedef typename base::const_reference const_reference;
	typedef typename base::iterator iterator;
	typedef typename base::const_iterator const_iterator;
	typedef typename base::reverse_iterator reverse_iterator;
	typedef typename base::const_reverse_iterator const_reverse_iterator;
	typedef typename base::value_type value_type;


public:
	set( ) : base( ){ }

	explicit set( const key_compare &pred ) : base( pred ){ }

	explicit set( const key_type &key ) : base( ){ base::insert( key ); }

	set( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	template< class Iterator >
	set( Iterator first, Iterator last ) : base( first, last ){ }

	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }

public:
	const set &operator +=( const set &s ){ base::insert( s.begin( ), s.end( ) ); return( *this ); }
	const set &operator +=( const key_type &s ){ base::insert( s ); return( *this ); }

	const set &operator -=( const set &s )
	{
		if( &s == this )
		{
			base::clear( );
		}
		else
		{
			set a = *this;
			const_iterator site1 = a.begin( );
			const_iterator eite1 = a.end( );
			const_iterator site2 = s.begin( );
			const_iterator eite2 = s.end( );

			// �������g�̓��e������ۂɂ���
			base::clear( );

			key_compare compare;
			while( site1 != eite1 && site1 != eite1 )
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
				base::insert( site1, eite1 );
			}
		}
		return( *this );
	}
	const set &operator -=( const key_type &s ){ base::erase( s ); return( *this ); }

	const set &operator *=( const set &s )
	{
		if( &s != this )
		{
			set a = *this;
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

	bool operator ==( const set &s ) const
	{
		if( base::size( ) != s.size( ) )
		{
			return( false );
		}

		key_compare compare;
		const_iterator ite1 = base::begin( );
		const_iterator ite2 = s.begin( );
		for( ; ite1 != base::end( ) ; ++ite1, ++ite2 )
		{
			if( compare( *ite1, *ite2 ) || compare( *ite2, *ite1 ) )
			{
				return( false );
			}
		}
		return( true );
	}
	bool operator !=( const set &s ) const { return( !( *this == s ) ); }
	bool operator < ( const set &s ) const
	{
		if( base::size( ) >= s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}
	bool operator <=( const set &s ) const
	{
		if( base::size( ) > s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}
	bool operator > ( const set &s ) const { return( s < *this ); }
	bool operator >=( const set &s ) const { return( s <= *this ); }
};

template< class Key, class Comp, class Allocator > inline const set< Key, Comp, Allocator > operator +( const set< Key, Comp, Allocator > &s1, const set< Key, Comp, Allocator > &s2 ){ return( set< Key, Comp, Allocator >( s1 ) += s2 ); }
template< class Key, class Comp, class Allocator > inline const set< Key, Comp, Allocator > operator -( const set< Key, Comp, Allocator > &s1, const set< Key, Comp, Allocator > &s2 ){ return( set< Key, Comp, Allocator >( s1 ) -= s2 ); }
template< class Key, class Comp, class Allocator > inline const set< Key, Comp, Allocator > operator *( const set< Key, Comp, Allocator > &s1, const set< Key, Comp, Allocator > &s2 ){ return( set< Key, Comp, Allocator >( s1 ) *= s2 ); }
//inline const set operator /( const set &s1, const set &s2 ){ return( set( s1 ) /= s2 ); }
//inline const set operator %( const set &s1, const set &s2 ){ return( set( s1 ) %= s2 ); }
//inline const set operator |( const set &s1, const set &s2 ){ return( set( s1 ) |= s2 ); }
//inline const set operator &( const set &s1, const set &s2 ){ return( set( s1 ) &= s2 ); }
//inline const set operator ^( const set &s1, const set &s2 ){ return( set( s1 ) ^= s2 ); }

template< class Key, class Comp, class Allocator > inline const set< Key, Comp, Allocator > operator +( const set< Key, Comp, Allocator > &s1, const typename set< Key, Comp, Allocator >::key_type &s2 ){ return( set< Key, Comp, Allocator >( s1 ) += s2 ); }
template< class Key, class Comp, class Allocator > inline const set< Key, Comp, Allocator > operator +( const typename set< Key, Comp, Allocator >::key_type &s1, const set< Key, Comp, Allocator > &s2 ){ return( set< Key, Comp, Allocator >( s2 ) += s1 ); }
template< class Key, class Comp, class Allocator > inline const set< Key, Comp, Allocator > operator -( const set< Key, Comp, Allocator > &s1, const typename set< Key, Comp, Allocator >::key_type &s2 ){ return( set< Key, Comp, Allocator >( s1 ) -= s2 ); }
template< class Key, class Comp, class Allocator > inline const set< Key, Comp, Allocator > operator -( const typename set< Key, Comp, Allocator >::key_type &s1, const set< Key, Comp, Allocator > &s2 ){ return( set< Key, Comp, Allocator >( s1 ) -= s2 ); }


template< class Key, class Comp, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const set< Key, Comp, Allocator > &s )
{
	typename set< Key, Comp, Allocator >::const_iterator ite = s.begin( );
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


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_SET_H__
