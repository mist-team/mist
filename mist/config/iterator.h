#ifndef __INCLUDE_MIST_ITERATOR_H__
#define __INCLUDE_MIST_ITERATOR_H__


#include <iterator>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN


// CONST
template< class T >
struct const_trait{ typedef const T const_type; };

template< class T >
struct const_trait< const T >{ typedef T const_type; };


// mistコンテナで利用する1次元操作用ランダムアクセスイテレータ
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
	typedef typename const_trait< reference >::const_type const_reference;

private:
	pointer data_;
	size_type diff_pointer_;

public:
	// コンストラクタ
	mist_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }
	mist_iterator1( const mist_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }

	// コピー演算子
	const mist_iterator1& operator =( const mist_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}

	// 要素のアクセス
	reference operator *(){ return( *data_ ); }
	const_reference operator *() const { return( *data_ ); }
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }
	const_reference operator []( difference_type dist ) const { return( data_[ dist * diff_pointer_ ] ); }

	// 移動
	mist_iterator1& operator ++( ) // 前置型
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator1 operator ++( int ) // 後置型
	{
		mist_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator1& operator --( ) // 前置型
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator1 operator --( int ) // 後置型
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

	const difference_type operator -( const mist_iterator1 &ite ) const
	{
		return( ( data_ - ite.data_ ) / diff_pointer_ );
	}

	// 比較
	bool operator ==( const mist_iterator1 &ite ) const { return( data_ == ite.data_ ); }
	bool operator !=( const mist_iterator1 &ite ) const { return( data_ != ite.data_ ); }
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
};


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
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}



// mistコンテナで利用する2次元操作用ランダムアクセスイテレータ
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
	typedef typename const_trait< reference >::const_type const_reference;

private:
	pointer sdata_;
	pointer data_;
	size_type diff_pointer1_;
	size_type diff_pointer2_;
	size_type diff_boundary_;

public:
	// コンストラクタ
	mist_iterator2( pointer p = NULL, size_type diff1 = 1, size_type diff2 = 1, size_type bound = 1 )
				: sdata_( p ), data_( p ), diff_pointer1_( diff1 ), diff_pointer2_( diff2 ), diff_boundary_( bound )
	{
	}

	mist_iterator2( const mist_iterator2 &ite ) : sdata_( ite.sdata_ ), data_( ite.data_ ),
				diff_pointer1_( ite.diff_pointer1_ ), diff_pointer2_( ite.diff_pointer2_ ), diff_boundary_( ite.diff_boundary_ )
	{
	}

	// コピー演算子
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		sdata_ = ite.sdata_;
		data_ = ite.data_;
		diff_pointer1_ = ite.diff_pointer1_;
		diff_pointer2_ = ite.diff_pointer2_;
		diff_boundary_ = ite.diff_boundary_;
		return( *this );
	}

	// 要素のアクセス
	reference operator *(){ return( *data_ ); }
	const_reference operator *() const { return( *data_ ); }
	reference operator []( difference_type dist ){ return( *( *this += dist ) ); }
	const_reference operator []( difference_type dist ) const { return( *( *this += dist ) ); }

	// 移動
	mist_iterator2& operator ++( ) // 前置型
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator2 operator ++( int ) // 後置型
	{
		mist_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator2& operator --( ) // 前置型
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator2 operator --( int ) // 後置型
	{
		mist_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_iterator2& operator +=( difference_type dist )
	{
		if( diff_boundary_ != 0 )
		{
			difference_type diff0 = ( data_ - sdata_ ) / diff_pointer1_;
			difference_type diff1 = diff0 + dist;
			difference_type diff2, diff3;

			if( diff1 < 0 )
			{
				diff2 = -( ( -diff1 ) / diff_boundary_ + 1 );
			}
			else
			{
				diff2 = diff1 / diff_boundary_;
			}
			diff3 = diff1 - diff2 * diff_boundary_;

			sdata_ += diff2 * ( diff_boundary_ + diff_pointer2_ );
			data_   = sdata_ + diff3 * diff_pointer1_;
		}
		else
		{
			data_ += dist * diff_pointer1_;
		}

		return( *this );
	}
	const mist_iterator2& operator -=( difference_type dist )
	{
		return( operator +=( -dist ) );
	}

	const difference_type operator -( const mist_iterator2 &ite ) const
	{
		if( diff_pointer2_ == 0 )
		{
			return( ( data_ - ite.data_ ) / diff_pointer1_ );
		}
		else
		{
			difference_type diff  = sdata_ - ite.sdata_;
			if( diff > 0 )
			{
				difference_type diff1 = diff / ( diff_boundary_ + diff_pointer2_ );
				difference_type diff2 = ( data_ - sdata_ ) / diff_pointer1_;
				difference_type diff3 = diff_boundary_ - ( ite.data_ - ite.sdata_ ) / diff_pointer1_;
				diff = ( diff1 < 1 ? 0 : diff1 - 1 ) * diff_boundary_ + diff2 + diff3;
			}
			else if( diff < 0 )
			{
				return( -( ite - *this ) );
			}
			else
			{
				diff = ( data_ - ite.data_ ) / diff_pointer1_;
			}
			return( diff );
		}
	}

	// 比較
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


//template< class T, class Distance, class Pointer, class Reference >
//inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_iterator2< T, Distance, Pointer, Reference > ite2 )
//{
//	return( mist_iterator2< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
//}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}



// mistで用いる
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
	typedef typename const_trait< reference >::const_type const_reference;

protected:
	_Ite current_iterator_;

public:
	// コンストラクタ
	mist_reverse_iterator( ){ }
	mist_reverse_iterator( const _Ite &ite ) : current_iterator_( ite ){ }
	mist_reverse_iterator( const mist_reverse_iterator &ite ) : current_iterator_( ite.current_iterator_ ){ }

	// コピー演算子
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

	// 要素のアクセス
	reference operator *()
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}
	const_reference operator *() const
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}
	reference operator []( difference_type dist ){ return( *( *this + dist ) ); }
	const_reference operator []( difference_type dist ) const { return( *( *this + dist ) ); }

	// 移動
	mist_reverse_iterator& operator ++( ) // 前置型
	{
		--current_iterator_;
		return( *this );
	}
	const mist_reverse_iterator operator ++( int ) // 後置型
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_--;
		return( old_val );
	}
	mist_reverse_iterator& operator --( ) // 前置型
	{
		++current_iterator_;
		return( *this );
	}
	const mist_reverse_iterator operator --( int ) // 後置型
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

	const difference_type operator -( const mist_reverse_iterator &ite ) const
	{
		return( ite.current_iterator_ - current_iterator_ );
	}


	// 比較
	bool operator ==( const mist_reverse_iterator &ite ) const { return( current_iterator_ == ite.current_iterator_ ); }
	bool operator !=( const mist_reverse_iterator &ite ) const { return( current_iterator_ != ite.current_iterator_ ); }
	bool operator < ( const mist_reverse_iterator &ite ) const { return( ite.current_iterator_ < current_iterator_  ); }
	bool operator <=( const mist_reverse_iterator &ite ) const { return( !( *this > ite ) ); }
	bool operator > ( const mist_reverse_iterator &ite ) const { return( ite < *this ); }
	bool operator >=( const mist_reverse_iterator &ite ) const { return( !( *this < ite ) ); }
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


//template< class _Ite >
//inline const typename mist_reverse_iterator< _Ite >::difference_type operator -( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
//{
//	return( ite1 - ite2 );
//}

template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator -( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) -= dist );
}

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_ITERATOR_H__
