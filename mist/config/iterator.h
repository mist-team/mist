#ifndef __INCLUDE_MIST_ITERATOR_H__
#define __INCLUDE_MIST_ITERATOR_H__


#include <iterator>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// mist�R���e�i�ŗ��p����1��������p�����_���A�N�Z�X�C�e���[�^
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef Reference reference;
	typedef size_t size_type;
	typedef Distance difference_type;

private:
	pointer data_;
	size_type diff_pointer_;

public:
	// �R���X�g���N�^
	mist_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }
	mist_iterator1( const mist_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }

	// �R�s�[���Z�q
	const mist_iterator1& operator =( const mist_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}

	// �v�f�̃A�N�Z�X
	reference operator *(){ return( *data_ ); }
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	// �ړ�
	mist_iterator1& operator ++( ) // �O�u�^
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator1 operator ++( int ) // ��u�^
	{
		mist_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator1& operator --( ) // �O�u�^
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator1 operator --( int ) // ��u�^
	{
		mist_iterator1 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_iterator1& operator +=( difference_type dist )
	{
		data_ += dist * diff_pointer_;
		return( *this );
	}
	const mist_iterator1& operator -=( difference_type dist )
	{
		data_ -= dist * diff_pointer_;
		return( *this );
	}

	// ��r
	bool operator ==( const mist_iterator1 &ite ) const { return( data_ == ite.data_ ); }
	bool operator !=( const mist_iterator1 &ite ) const { return( data_ != ite.data_ ); }
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
};


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}



// mist�R���e�i�ŗ��p����2��������p�����_���A�N�Z�X�C�e���[�^
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef size_t size_type;
	typedef Reference reference;
	typedef Distance difference_type;

private:
	pointer sdata_;
	pointer data_;
	size_type diff_pointer1_;
	size_type diff_pointer2_;
	size_type diff_boundary_;

public:
	// �R���X�g���N�^
	mist_iterator2( pointer p = NULL, size_type diff1 = 1, size_type diff2 = 1, size_type bound = 1 )
				: sdata_( p ), data_( p ), diff_pointer1_( diff1 ), diff_pointer2_( diff2 ), diff_boundary_( bound )
	{
	}

	mist_iterator2( const mist_iterator2 &ite ) : sdata_( ite.sdata_ ), data_( ite.data_ ),
				diff_pointer1_( ite.diff_pointer1_ ), diff_pointer2_( ite.diff_pointer2_ ), diff_boundary_( ite.diff_boundary_ )
	{
	}

	// �R�s�[���Z�q
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		sdata_ = ite.sdata_;
		data_ = ite.data_;
		diff_pointer1_ = ite.diff_pointer1_;
		diff_pointer2_ = ite.diff_pointer2_;
		diff_boundary_ = ite.diff_boundary_;
		return( *this );
	}

	// �v�f�̃A�N�Z�X
	reference operator *(){ return( *data_ ); }
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	// �ړ�
	mist_iterator2& operator ++( ) // �O�u�^
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator2 operator ++( int ) // ��u�^
	{
		mist_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator2& operator --( ) // �O�u�^
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator2 operator --( int ) // ��u�^
	{
		mist_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_iterator2& operator +=( difference_type dist )
	{
		data_ += dist * diff_pointer1_;

		if( data_ > sdata_ )
		{
			if( data_ >= diff_boundary_ + sdata_ )
			{
				sdata_ += diff_boundary_ + diff_pointer2_;
				data_  += diff_pointer2_;
			}
		}
		else if( data_ < sdata_ )
		{
			sdata_ -= diff_boundary_ + diff_pointer2_;
			data_  -= diff_pointer2_;
		}

		return( *this );
	}
	const mist_iterator2& operator -=( difference_type dist )
	{
		data_ -= dist * diff_pointer1_;

		if( data_ > sdata_ )
		{
			if( data_ >= diff_boundary_ + sdata_ )
			{
				sdata_ += diff_boundary_ + diff_pointer2_;
				data_  += diff_pointer2_;
			}
		}
		else if( data_ < sdata_ )
		{
			sdata_ -= diff_boundary_ + diff_pointer2_;
			data_  -= diff_pointer2_;
		}

		return( *this );
	}

	// ��r
	bool operator ==( const mist_iterator2 &ite ) const { return( data_ == ite.data_ ); }
	bool operator !=( const mist_iterator2 &ite ) const { return( data_ != ite.data_ ); }
	bool operator < ( const mist_iterator2 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator <=( const mist_iterator2 &ite ) const { return( data_ <= ite.data_ ); }
	bool operator > ( const mist_iterator2 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator >=( const mist_iterator2 &ite ) const { return( data_ >= ite.data_ ); }
};



template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}



// mist�ŗp����
template< class _Ite >
class mist_reverse_iterator :
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type
	>
#else
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type,
		typename _Ite::pointer,
		typename _Ite::reference
	>
#endif
{
public:
 	typedef typename _Ite::difference_type difference_type;
	typedef typename _Ite::pointer pointer;
	typedef typename _Ite::reference reference;

protected:
	_Ite current_iterator_;

public:
	// �R���X�g���N�^
	mist_reverse_iterator( ){ }
	mist_reverse_iterator( const _Ite &ite ) : current_iterator_( ite ){ }
	mist_reverse_iterator( const mist_reverse_iterator &ite ) : current_iterator_( ite.current_iterator_ ){ }

	// �R�s�[���Z�q
	const mist_reverse_iterator& operator =( const _Ite &ite )
	{
		current_iterator_ = ite;
		return( *this );
	}
	const mist_reverse_iterator& operator =( const mist_reverse_iterator &ite )
	{
		current_iterator_ = ite.current_iterator_;
		return( *this );
	}

	// �v�f�̃A�N�Z�X
	reference operator *()
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}
	reference operator []( difference_type dist ){ return( *( *this + dist ) ); }

	// �ړ�
	mist_reverse_iterator& operator ++( ) // �O�u�^
	{
		--current_iterator_;
		return( *this );
	}
	const mist_reverse_iterator operator ++( int ) // ��u�^
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_--;
		return( old_val );
	}
	mist_reverse_iterator& operator --( ) // �O�u�^
	{
		++current_iterator_;
		return( *this );
	}
	const mist_reverse_iterator operator --( int ) // ��u�^
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_++;
		return( old_val );
	}

	const mist_reverse_iterator& operator +=( difference_type dist )
	{
		current_iterator_ -= dist;
		return( *this );
	}
	const mist_reverse_iterator& operator -=( difference_type dist )
	{
		current_iterator_ += dist;
		return( *this );
	}


	// ��r
	bool operator ==( const mist_reverse_iterator &ite ) const { return( current_iterator_ == ite.current_iterator_ ); }
	bool operator !=( const mist_reverse_iterator &ite ) const { return( current_iterator_ != ite.current_iterator_ ); }
	bool operator < ( const mist_reverse_iterator &ite ) const { return( current_iterator_ <  ite.current_iterator_ ); }
	bool operator <=( const mist_reverse_iterator &ite ) const { return( current_iterator_ <= ite.current_iterator_ ); }
	bool operator > ( const mist_reverse_iterator &ite ) const { return( current_iterator_ >  ite.current_iterator_ ); }
	bool operator >=( const mist_reverse_iterator &ite ) const { return( current_iterator_ >= ite.current_iterator_ ); }
};


template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
{
	return( mist_reverse_iterator< _Ite >( ite1 ) += ite2 );
}

template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}

template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( typename _Ite::difference_type dist, const mist_reverse_iterator< _Ite > &ite )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}


template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator -( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
{
	return( mist_reverse_iterator< _Ite >( ite1 ) -= ite2 );
}

template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator -( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) -= dist );
}

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_ITERATOR_H__