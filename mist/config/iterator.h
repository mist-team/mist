#ifndef __INCLUDE_MIST_ITERATOR_H__
#define __INCLUDE_MIST_ITERATOR_H__


#include <iterator>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN



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
	typedef Reference const_reference;

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
	bool operator !=( const mist_iterator1 &ite ) const { return( !( *this == ite )  ); }
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
};


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
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
	typedef Reference const_reference;

private:
	pointer data_;
	difference_type index_;
	difference_type width_;
	difference_type step_;

public:
	// コンストラクタ
	mist_iterator2( pointer p = NULL, difference_type index = 0, difference_type width = 1, difference_type step = 1 )
												: data_( p ), index_( index ), width_( width ), step_( step )
	{
	}

	mist_iterator2( const mist_iterator2 &ite ) : data_( ite.data_ ), index_( ite.index_ ), width_( ite.width_ ), step_( ite.step_ )
	{
	}

	// コピー演算子
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		if( &ite != this )
		{
			data_  = ite.data_;
			index_ = ite.index_;
			width_ = ite.width_;
			step_  = ite.step_;
		}
		return( *this );
	}

	// 要素のアクセス
	reference operator *()
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}
	const_reference operator *() const
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}
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
		index_ += dist;
		return( *this );
	}
	const mist_iterator2& operator -=( difference_type dist )
	{
		index_ -= dist;
		return( *this );
	}

	const difference_type operator -( const mist_iterator2 &ite ) const
	{
		return( index_ - ite.index_ );
	}

	// 比較
	bool operator ==( const mist_iterator2 &ite ) const { return( *this - ite == 0 ); }
	bool operator !=( const mist_iterator2 &ite ) const { return( !( *this == ite ) ); }
	bool operator < ( const mist_iterator2 &ite ) const { return( *this - ite < 0 ); }
	bool operator <=( const mist_iterator2 &ite ) const { return( !( *this > ite ) ); }
	bool operator > ( const mist_iterator2 &ite ) const { return( ite < *this ); }
	bool operator >=( const mist_iterator2 &ite ) const { return( !( *this < ite ) ); }
};



template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
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
	typedef typename _Ite::const_reference const_reference;

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
	bool operator !=( const mist_reverse_iterator &ite ) const { return( !( *this == ite ) ); }
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
