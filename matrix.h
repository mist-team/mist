#ifndef __INCLUDE_MIST_MATRIX__
#define __INCLUDE_MIST_MATRIX__

#include "mist.h"


// mist名前空間の始まり
_MIST_BEGIN



// 行列
template < class T, class Allocator = ::std::allocator< T > >
class matrix : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;
	typedef typename array< T, Allocator >::reference reference;
	typedef typename array< T, Allocator >::const_reference const_reference;
	typedef typename array< T, Allocator >::value_type value_type;
	typedef typename array< T, Allocator >::size_type size_type;
	typedef typename array< T, Allocator >::difference_type difference_type;
	typedef typename array< T, Allocator >::pointer pointer;
	typedef typename array< T, Allocator >::const_pointer const_pointer;

private:
	typedef array< T, Allocator > base;
	size_type size1_;
	size_type size2_;

public:
	void resize( size_type num1, size_type num2 )
	{
		base::resize( num1 * num2 );
		size1_ = num1;
		size2_ = num2;
	}

	void resize( size_type num1, size_type num2, const T &val )
	{
		base::resize( num1 * num2, val );
		size1_ = num1;
		size2_ = num2;
	}

	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}

	size_type size1( ) const { return( size1_ ); }
	size_type size2( ) const { return( size2_ ); }
	size_type rows( ) const { return( size1_ ); }
	size_type cols( ) const { return( size2_ ); }

public: // 配列に対する算術演算
	matrix t() const
	{
		const matrix &m = *this;
		matrix o( size2_, size1_ );
		for( size_type r = 0 ; r < o.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < o.cols( ) ; c++ )
			{
				o( r, c ) = m( c, r );
			}
		}
		return( o );
	}


public:
	template < class TT, class AAlocator >
	const matrix& operator =( const matrix< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );

		return( *this );
	}

	const matrix& operator =( const matrix &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );

		return( *this );
	}

	void swap( matrix &m )
	{
		base::swap( m );
		size_type _dmy_size1 = size1_;
		size_type _dmy_size2 = size2_;
		size1_ = m.size1_;
		size2_ = m.size2_;
		m.size1_ = _dmy_size1;
		m.size2_ = _dmy_size2;
	}


public: // 要素へのアクセス
	T &at( size_type r, size_type c )
	{
		return( data_[ r + c * size1_ ] );
	}

	const T &at( size_type r, size_type c ) const
	{
		return( data_[ r + c * size1_ ] );
	}

	T &operator []( size_type index )
	{
		return( data_[index] );
	}

	const T &operator []( size_type index ) const
	{
		return( data_[index] );
	}

	T &operator ()( size_type r, size_type c )
	{
		return( data_[ r + c * size1_ ] );
	}

	const T &operator ()( size_type r, size_type c ) const
	{
		return( data_[ r + c * size1_ ] );
	}


public:
	// 構築
	matrix( ) : base( ), size1_( 0 ), size2_( 0 ) {}
	explicit matrix( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ) {}

	matrix( size_type rnum, size_type cnum ) : base( rnum * cnum ), size1_( rnum ), size2_( cnum ) {}
	matrix( size_type rnum, size_type cnum, const Allocator &a ) : base( rnum * cnum, a ), size1_( rnum ), size2_( cnum ) {}

	matrix( size_type rnum, size_type cnum, const T &val ) : base( rnum * cnum, val ), size1_( rnum ), size2_( cnum ) {}
	matrix( size_type rnum, size_type cnum, const T &val, const Allocator &a ) : base( rnum * cnum, val, a ), size1_( rnum ), size2_( cnum ) {}

	template < class TT, class AAlocator >
	explicit matrix( const matrix< TT, AAlocator > &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ){ }

	matrix( const matrix< T, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ){ }

};


template < class T, class Allocator >
inline ::std::ostream &operator <<( ::std::ostream &out, const matrix< T, Allocator > &m )
{
	typename array2< T, Allocator >::size_type r, c;
	for( r = 0 ; r < m.rows( ) ; r++ )
	{
		for( c = 0 ; c < m.cols( ) ; c++ )
		{
			out << m( r, c );
			if( c != m.cols( ) - 1 ) out << ", ";
		}
		out << ::std::endl;
	}

	return( out );
}


#include "operator/operator_matrix.h"


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_MATRIX__