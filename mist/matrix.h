#ifndef __INCLUDE_MIST_MATRIX__
#define __INCLUDE_MIST_MATRIX__

#include "mist.h"


// mist名前空間の始まり
_MIST_BEGIN

#if _USE_EXPRESSION_TEMPLATE_ != 0

template < class T > struct matrix_minus;
template < class T > struct matrix_transpose;

template< class T, class Allocator, class Derive >
struct matrix_expression
{
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef typename Allocator::size_type size_type;

	const Derive &get_ref( ) const { return( static_cast< const Derive & >( *this ) ); }
	value_type operator()( size_type r, size_type c ) const { return( get_ref( ).operator()( r, c ) ); }
	size_type rows( ) const { return( get_ref( ).rows( ) ); };
	size_type cols( ) const { return( get_ref( ).cols( ) ); };

	matrix_minus< matrix_expression< T, Allocator, Derive > > operator -() const { return( matrix_minus< matrix_expression< T, Allocator, Derive > >( *this ) ); }
	matrix_transpose< matrix_expression< T, Allocator, Derive > > t( ) const { return( matrix_transpose< matrix_expression< T, Allocator, Derive > >( *this ) ); }
};

// 単項演算子
template< class T, class Derive >
struct matrix_single_operation : public matrix_expression< typename T::value_type, typename T::allocator_type, Derive > 
{
	typedef typename T::value_type value_type;
	typedef typename T::size_type size_type;

	const T &middle_;

	matrix_single_operation( const T &mhs ) : middle_( mhs ){}
};

// ２項演算子
template< class T1, class T2, class Derive >
struct matrix_bind_operation : public matrix_expression< typename T1::value_type, typename T1::allocator_type, Derive > 
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	const T1 &left_;
	const T2 &right_;

	matrix_bind_operation( const T1 &lhs, const T2 &rhs ) : left_( lhs ), right_( rhs ){}
};


// 負の符号オペレータ
template< class T >
struct matrix_minus : public matrix_single_operation< T, matrix_minus< T > >
{
	typedef typename T::value_type value_type;
	typedef typename T::size_type size_type;

	matrix_minus( const T &mhs ) : matrix_single_operation< T, matrix_minus< T > >( mhs ){}
	size_type rows( ) const { return( middle_.rows( ) ); }
	size_type cols( ) const { return( middle_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( - middle_( r, c ) ); }
};


// 転置オペレータ
template< class T >
struct matrix_transpose : public matrix_single_operation< T, matrix_transpose< T > >
{
	typedef typename T::value_type value_type;
	typedef typename T::size_type size_type;

	matrix_transpose( const T &mhs ) : matrix_single_operation< T, matrix_transpose< T > >( mhs ){}
	size_type rows( ) const { return( middle_.cols( ) ); }
	size_type cols( ) const { return( middle_.rows( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( middle_( c, r ) ); }
};


// 足し算オペレータ
template< class T1, class T2 >
struct matrix_add : public matrix_bind_operation< T1, T2, matrix_add< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_add( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_add< T1, T2 > >( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( lhs.rows( ) != rhs.rows( ) || lhs.cols( ) != rhs.cols( ) )
		{
			// 足し算できません例外
			::std::cout << "足し算を行おうとする行列の行と列の数が一致しません．" << ::std::endl;
		}
#endif
	}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( left_( r, c ) + right_( r, c ) ); }
};


// 引き算オペレータ
template< class T1, class T2 >
struct matrix_sub : public matrix_bind_operation< T1, T2, matrix_sub< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_sub( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_sub< T1, T2 > >( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( lhs.rows( ) != rhs.rows( ) || lhs.cols( ) != rhs.cols( ) )
		{
			// 引き算できません例外
			::std::cout << "引き算を行おうとする行列の行と列の数が一致しません．" << ::std::endl;
		}
#endif
	}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( left_( r, c ) - right_( r, c ) ); }
};


//掛け算オペレータ
template< class T1, class T2 >
struct matrix_mul : public matrix_bind_operation< T1, T2, matrix_mul< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_mul( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_mul< T1, T2 > >( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( lhs.cols( ) != rhs.rows( ) )
		{
			// 掛け算できません例外
			::std::cout << "掛け算を行おうとする行列の行と列の数が一致しません．" << ::std::endl;
		}
#endif
	}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( right_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const
	{
		value_type v = 0;
		size_type size = left_.cols( );
		for( size_type t = 0 ; t < size ; ++t )
		{
			v += left_( r, t ) * right_( t, c );
		}
		return( v );
	}
};


// 定数の足し算オペレータ
template< class T1, class T2 >
struct matrix_add_const : public matrix_bind_operation< T1, T2, matrix_add_const< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_add_const( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_add_const< T1, T2 > >( lhs, rhs ){}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( left_( r, c ) + ( r == c ? right_ : 0 ) ); }
};

// 定数の足し算オペレータ（右側が定数）
template< class T1, class T2 >
struct matrix_sub_const1 : public matrix_bind_operation< T1, T2, matrix_sub_const1< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_sub_const1( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_sub_const1< T1, T2 > >( lhs, rhs ){}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( left_( r, c ) - ( r == c ? right_ : 0 ) ); }
};

// 定数の足し算オペレータ（左側が定数）
template< class T1, class T2 >
struct matrix_sub_const2 : public matrix_bind_operation< T1, T2, matrix_sub_const2< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_sub_const2( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_sub_const2< T1, T2 > >( lhs, rhs ){}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( ( r == c ? right_ : 0 ) - left_( r, c ) ); }
};

// 定数の掛け算オペレータ
template< class T1, class T2 >
struct matrix_mul_const : public matrix_bind_operation< T1, T2, matrix_mul_const< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_mul_const( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_mul_const< T1, T2 > >( lhs, rhs ){}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( left_( r, c ) * right_ ); }
};

// 定数の割り算オペレータ
template< class T1, class T2 >
struct matrix_div_const : public matrix_bind_operation< T1, T2, matrix_div_const< T1, T2 > >
{
	typedef typename T1::value_type value_type;
	typedef typename T1::size_type size_type;

	matrix_div_const( const T1 &lhs, const T2 &rhs ) : matrix_bind_operation< T1, T2, matrix_div_const< T1, T2 > >( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( rhs == 0 )
		{
			// ゼロ除算を行おうとしている例外
			::std::cout << "ゼロ除算を行おうとしています．" << ::std::endl;
		}
#endif
	}
	size_type rows( ) const { return( left_.rows( ) ); }
	size_type cols( ) const { return( left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( left_( r, c ) / right_ ); }
};

#endif


// 行列
template < class T, class Allocator = ::std::allocator< T > >
class matrix : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

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
#if _USE_EXPRESSION_TEMPLATE_ != 0
	matrix_transpose< matrix > t( )
	{
		return( matrix_transpose< matrix >( *this ) );
	}
#else
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
#endif


/************************************************************************************************************
**
**      行列に対する演算子
**        += 行列
**        += 定数
**
**        -= 行列
**        -= 定数
**
**        *= 行列
**        *= 定数
**
**        /= 定数
**
************************************************************************************************************/
	template < class TT, class AAlocator >
	const matrix& operator +=( const matrix< TT, AAlocator > &m2 )
	{
		matrix &m1 = *this;
#if _CHECK_ARRAY_OPERATION_ != 0
		if( m1.size( ) != m2.size( ) )
		{
			// 足し算できません例外
			::std::cout << "can't add arrays." << ::std::endl;
			return( *this );
		}
#endif
		for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] += static_cast< T >( m2[i] );
		return( m1 );
	}

	template < class TT, class AAlocator >
	const matrix& operator -=( const matrix< TT, AAlocator > &m2 )
	{
		matrix &m1 = *this;
#ifdef _CHECK_ARRAY_OPERATION_
		if( m1.size( ) != m2.size( ) )
		{
			// 引き算できません例外
			::std::cout << "can't subtract matrixs." << ::std::endl;
			return( m1 );
		}
#endif
		for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] -= static_cast< T >( m2[i] );
		return( m1 );
	}

	template < class TT, class AAlocator >
	const matrix& operator *=( const matrix< TT, AAlocator > &m2 )
	{
		matrix &m1 = *this;
		typedef typename matrix< T, Allocator >::size_type size_type;
#ifdef _CHECK_MATRIX_OPERATION_
		if( m1.cols( ) != m2.rows( ) )
		{
			// 掛け算できません例外
			::std::cout << "can't multiply matrices." << ::std::endl;
			return( m1 );
		}
#endif

		matrix< T, Allocator > mat( m1.rows( ), m2.cols( ) );
		size_type r, c, t;

		for( r = 0 ; r < mat.rows( ) ; r++ )
		{
			for( c = 0 ; c < mat.cols( ) ; c++ )
			{
				for( t = 0 ; t < m1.cols( ) ; t++ )
				{
					mat( r, c ) += m1( r, t ) * static_cast< T >( m2( t, c ) );
				}
			}
		}

		m1.swap( mat );

		return( m1 );
	}

	const matrix& operator +=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) += val;
		return( m );
	}

	const matrix& operator -=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) -= val;
		return( m );
	}

	const matrix& operator *=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		for( size_type i = 0 ; i < m.size( ) ; i++ ) m[i] *= val;
		return( m );
	}

	const matrix& operator /=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
#ifdef _CHECK_ARRAY_OPERATION_
		if( val == 0 )
		{
			// ゼロ除算発生
			::std::cout << "zero division occured." << ::std::endl;
			return;
		}
#endif
		for( size_type i = 0 ; i < m.size( ) ; i++ ) m[i] /= val;
		return( m );
	}

#if _USE_EXPRESSION_TEMPLATE_ != 0

	template < class Derive >
	matrix& operator +=( const matrix_expression< T, Allocator, Derive > &m2 )
	{
		matrix &m1 = *this;
#ifndef _CHECK_MATRIX_OPERATION_
		if( m1.cols( ) != m2.cols( ) || m1.rows( ) != m2.rows( ) )
		{
			// 足し算できません例外
			::std::cout << "can't add matrices." << ::std::endl;
			return( m1 );
		}
#endif
		for( size_type r = 0 ; r < m1.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < m1.cols( ) ; c++ )
			{
				m1( r, c ) += m2( r, c );
			}
		}
		return( m1 );
	}

	template < class Derive >
	matrix& operator -=( const matrix_expression< T, Allocator, Derive > &m2 )
	{
		matrix &m1 = *this;
#ifndef _CHECK_MATRIX_OPERATION_
		if( m1.cols( ) != m2.cols( ) || m1.rows( ) != m2.rows( ) )
		{
			// 引き算できません例外
			::std::cout << "can't subtract matrices." << ::std::endl;
			return( m1 );
		}
#endif
		for( size_type r = 0 ; r < m1.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < m1.cols( ) ; c++ )
			{
				m1( r, c ) -= m2( r, c );
			}
		}
		return( m1 );
	}

	template < class Derive >
	matrix& operator *=( const matrix_expression< T, Allocator, Derive > &m2 )
	{
		matrix &m1 = *this;
#ifndef _CHECK_MATRIX_OPERATION_
		if( m1.cols( ) != m2.rows( ) )
		{
			// 掛け算できません例外
			::std::cout << "can't multiply matrices." << ::std::endl;
			return( m1 );
		}
#endif

		matrix< T, Allocator > mat( m1.rows( ), m2.cols( ) );

		for( size_type r = 0 ; r < mat.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < mat.cols( ) ; c++ )
			{
				for( size_type t = 0 ; t < m1.cols( ) ; t++ )
				{
					mat( r, c ) += m1( r, t ) * m2( t, c );
				}
			}
		}

		m1.swap( mat );

		return( m1 );
	}

#endif

public:
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

public:
	template < class TT, class AAlocator >
	const matrix& operator =( const matrix< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );

		return( *this );
	}

	const matrix< T, Allocator >& operator =( const matrix< T, Allocator > &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );

		return( *this );
	}

#if _USE_EXPRESSION_TEMPLATE_ != 0
	template < class Derive >
	const matrix& operator =( const matrix_expression< T, Allocator, Derive > &expression )
	{
		matrix m( expression.rows( ), expression.cols( ) );
		for( size_type r = 0 ; r < m.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < m.cols( ) ; c++ )
			{
				m( r, c ) = expression( r, c );
			}
		}
		m.swap( *this );

		return( *this );
	}
#endif

public: // 要素へのアクセス
	T &at( size_type r, size_type c )
	{
#if _CHECK_ACCESS_VIOLATION_ != 0
		if( r < 0 || r >= size1_ || c < 0 || c >= size2_ )
		{
			static T dmy;
			std::cout << "Access Violation at ( " << r << ", " << c << " )" << std::endl;
			return( dmy );
		}
#endif
		return( data_[ r + c * size1_ ] );
	}

	const T &at( size_type r, size_type c ) const
	{
#if _CHECK_ACCESS_VIOLATION_ != 0
		if( r < 0 || r >= size1_ || c < 0 || c >= size2_ )
		{
			static T dmy;
			std::cout << "Access Violation at ( " << r << ", " << c << " )" << std::endl;
			return( dmy );
		}
#endif
		return( data_[ r + c * size1_ ] );
	}

	T &operator ()( size_type r, size_type c )
	{
#if _CHECK_ACCESS_VIOLATION_ != 0
		if( r < 0 || r >= size1_ || c < 0 || c >= size2_ )
		{
			static T dmy;
			std::cout << "Access Violation at ( " << r << ", " << c << " )" << std::endl;
			return( dmy );
		}
#endif
		return( data_[ r + c * size1_ ] );
	}

	const T &operator ()( size_type r, size_type c ) const
	{
#if _CHECK_ACCESS_VIOLATION_ != 0
		if( r < 0 || r >= size1_ || c < 0 || c >= size2_ )
		{
			static T dmy;
			std::cout << "Access Violation at ( " << r << ", " << c << " )" << std::endl;
			return( dmy );
		}
#endif
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

#if _USE_EXPRESSION_TEMPLATE_ != 0
	template < class Derive >
	explicit matrix( const matrix_expression< T, Allocator, Derive > &expression ) : base( expression.rows( ) * expression.cols( ) ), size1_( expression.rows( ) ), size2_( expression.cols( ) )
	{
		matrix &m = *this;
		for( size_type r = 0 ; r < m.rows( ) ; r++ )
		{
			for( size_type c = 0 ; c < m.cols( ) ; c++ )
			{
				m( r, c ) = expression( r, c );
			}
		}
	}
#endif
};



/************************************************************************************************************
**
** 標準出力への行列の出力
**
************************************************************************************************************/
template < class T, class Allocator >
inline ::std::ostream &operator <<( ::std::ostream &out, const matrix< T, Allocator > &m )
{
	typename matrix< T, Allocator >::size_type r, c;
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


#if _USE_EXPRESSION_TEMPLATE_ != 0

template < class Middle, class Allocator, class Derive >
inline ::std::ostream &operator <<( ::std::ostream &out,  const matrix_expression< Middle, Allocator, Derive > &m )
{
	typedef typename matrix_expression< Middle, Allocator, Derive >::size_type size_type;
	typedef typename matrix_expression< Middle, Allocator, Derive >::value_type value_type;
	return( operator <<( out, matrix< value_type, Allocator >( m ) ) );
}

#endif



/************************************************************************************************************
**
**      符号反転オペレータと転置行列オペレータ
**
************************************************************************************************************/
#if _USE_EXPRESSION_TEMPLATE_ != 0

// 符号反転
template< class T, class A >
inline matrix_minus< matrix< T, A > > operator -( const matrix< T, A > &mhs )
{
	return( matrix_minus< matrix< T, A > >( mhs ) );
}


// 転置行列
template< class T, class A >
inline matrix_transpose< matrix< T, A > > transpose( const matrix< T, A > &mhs )
{
	return( matrix_transpose< matrix< T, A > >( mhs ) );
}

#else

// 符号反転
template < class T, class A >
inline matrix< T, A > operator -( const matrix< T, A > &m )
{
	typedef typename matrix< T, A >::size_type size_type;
	matrix< T, A > o( m );
	for( size_type i = 0 ; i < o.size( ) ; i++ ) o[i] = -o[i];
	return( o );
}


// 転置行列
template< class T, class A >
inline matrix< T, A > transpose( const matrix< T, A > &mhs )
{
	return( mhs.t( ) );
}

#endif




/************************************************************************************************************
**
**      行列に対する演算子
**        行列 + 行列
**        行列 + 定数
**        定数 + 行列
**
**        行列 - 行列
**        行列 - 定数
**        定数 - 行列
**
**        行列 * 行列
**        行列 * 定数
**        定数 * 行列
**
**        行列 / 定数
**
************************************************************************************************************/

#if _USE_EXPRESSION_TEMPLATE_ != 0

// 足し算
template< class T, class A >
inline matrix_add< matrix< T, A >, matrix< T, A > > operator +( const matrix< T, A > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_add< matrix< T, A >, matrix< T, A > >( lhs, rhs ) );
}

template< class T, class A, class Left, class Derive >
inline matrix_add< matrix_expression< Left, A, Derive >, matrix< T, A > > operator +( const matrix_expression< Left, A, Derive > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_add< matrix_expression< Left, A, Derive >, matrix< T, A > >( lhs, rhs ) );
}

template< class T, class A, class Right, class Derive >
inline matrix_add< matrix< T, A >, matrix_expression< Right, A, Derive > > operator +( const matrix< T, A > &lhs, const matrix_expression< Right, A, Derive > &rhs )
{
	return( matrix_add< matrix< T, A >, matrix_expression< Right, A, Derive > >( lhs, rhs ) );
}

template< class A, class Left, class Right, class Derive1, class Derive2 >
inline matrix_add< matrix_expression< Left, A, Derive1 >, matrix_expression< Right, A, Derive2 > > operator +( const matrix_expression< Left, A, Derive1 > &lhs, const matrix_expression< Right, A, Derive2 > &rhs )
{
	return( matrix_add< matrix_expression< Left, A, Derive1 >, matrix_expression< Right, A, Derive2 > >( lhs, rhs ) );
}

template< class T, class A >
inline matrix_add_const< matrix< T, A >, typename matrix< T, A >::value_type > operator +( const matrix< T, A > &lhs, const typename type_trait< T >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_add_const< matrix< T, A >, value_type >( lhs, rhs ) );
}

template< class T, class A >
inline matrix_add_const< matrix< T, A >, typename matrix< T, A >::value_type > operator +( const typename matrix< T, A >::value_type &lhs, const matrix< T, A > &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_add_const< matrix< T, A >, value_type >( rhs, lhs ) );
}

template< class A, class Left, class Derive >
inline matrix_add_const< matrix_expression< Left, A, Derive >, typename matrix_expression< Left, A, Derive >::value_type > operator +( const matrix_expression< Left, A, Derive > &lhs, const typename A::value_type &rhs )
{
	typedef typename matrix_expression< Left, A, Derive >::value_type value_type;
	return( matrix_add_const< matrix_expression< Left, A, Derive >, value_type >( lhs, rhs ) );
}

template< class A, class Right, class Derive >
inline matrix_add_const< matrix_expression< Right, A, Derive >, typename matrix_expression< Right, A, Derive >::value_type > operator +( const typename A::value_type &lhs, const matrix_expression< Right, A, Derive > &rhs )
{
	typedef typename matrix_expression< Right, A, Derive >::value_type value_type;
	return( matrix_add_const< matrix_expression< Right, A, Derive >, value_type >( rhs, lhs ) );
}


// 引き算
template< class T, class A >
inline matrix_sub< matrix< T, A >, matrix< T, A > > operator -( const matrix< T, A > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_sub< matrix< T, A >, matrix< T, A > >( lhs, rhs ) );
}

template< class T, class A, class Left, class Derive >
inline matrix_sub< matrix_expression< Left, A, Derive >, matrix< T, A > > operator -( const matrix_expression< Left, A, Derive > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_sub< matrix_expression< Left, A, Derive >, matrix< T, A > >( lhs, rhs ) );
}

template< class T, class A, class Right, class Derive >
inline matrix_sub< matrix< T, A >, matrix_expression< Right, A, Derive > > operator -( const matrix< T, A > &lhs, const matrix_expression< Right, A, Derive > &rhs )
{
	return( matrix_sub< matrix< T, A >, matrix_expression< Right, A, Derive > >( lhs, rhs ) );
}

template< class A, class Left, class Right, class Derive1, class Derive2 >
inline matrix_sub< matrix_expression< Left, A, Derive1 >, matrix_expression< Right, A, Derive2 > > operator -( const matrix_expression< Left, A, Derive1 > &lhs, const matrix_expression< Right, A, Derive2 > &rhs )
{
	return( matrix_sub< matrix_expression< Left, A, Derive1 >, matrix_expression< Right, A, Derive2 > >( lhs, rhs ) );
}

template< class T, class A >
inline matrix_sub_const1< matrix< T, A >, typename matrix< T, A >::value_type > operator -( const matrix< T, A > &lhs, const typename type_trait< T >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_sub_const1< matrix< T, A >, value_type >( lhs, rhs ) );
}

template< class T, class A >
inline matrix_sub_const2< matrix< T, A >, typename matrix< T, A >::value_type > operator -( const typename matrix< T, A >::value_type &lhs, const matrix< T, A > &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_sub_const2< matrix< T, A >, value_type >( rhs, lhs ) );
}

template< class A, class Left, class Derive >
inline matrix_sub_const1< matrix_expression< Left, A, Derive >, typename matrix_expression< Left, A, Derive >::value_type > operator -( const matrix_expression< Left, A, Derive > &lhs, const typename A::value_type &rhs )
{
	typedef typename matrix_expression< Left, A, Derive >::value_type value_type;
	return( matrix_sub_const1< matrix_expression< Left, A, Derive >, value_type >( lhs, rhs ) );
}

template< class A, class Right, class Derive >
inline matrix_sub_const2< matrix_expression< Right, A, Derive >, typename matrix_expression< Right, A, Derive >::value_type > operator -( const typename A::value_type &lhs, const matrix_expression< Right, A, Derive > &rhs )
{
	typedef typename matrix_expression< Right, A, Derive >::value_type value_type;
	return( matrix_sub_const2< matrix_expression< Right, A, Derive >, value_type >( rhs, lhs ) );
}


// 掛け算
template< class T, class A >
inline matrix_mul< matrix< T, A >, matrix< T, A > > operator *( const matrix< T, A > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_mul< matrix< T, A >, matrix< T, A > >( lhs, rhs ) );
}

template< class T, class A, class Left, class Derive >
inline matrix_mul< matrix_expression< Left, A, Derive >, matrix< T, A > > operator *( const matrix_expression< Left, A, Derive > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_mul< matrix_expression< Left, A, Derive >, matrix< T, A > >( lhs, rhs ) );
}

template< class T, class A, class Right, class Derive >
inline matrix_mul< matrix< T, A >, matrix_expression< Right, A, Derive > > operator *( const matrix< T, A > &lhs, const matrix_expression< Right, A, Derive > &rhs )
{
	return( matrix_mul< matrix< T, A >, matrix_expression< Right, A, Derive > >( lhs, rhs ) );
}

template< class A, class Left, class Right, class Derive1, class Derive2 >
inline matrix_mul< matrix_expression< Left, A, Derive1 >, matrix_expression< Right, A, Derive2 > > operator *( const matrix_expression< Left, A, Derive1 > &lhs, const matrix_expression< Right, A, Derive2 > &rhs )
{
	return( matrix_mul< matrix_expression< Left, A, Derive1 >, matrix_expression< Right, A, Derive2 > >( lhs, rhs ) );
}

template< class T, class A >
inline matrix_mul_const< matrix< T, A >, typename matrix< T, A >::value_type > operator *( const matrix< T, A > &lhs, const typename type_trait< T >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_mul_const< matrix< T, A >, value_type >( lhs, rhs ) );
}

template< class T, class A >
inline matrix_mul_const< matrix< T, A >, typename matrix< T, A >::value_type > operator *( const typename matrix< T, A >::value_type &lhs, const matrix< T, A > &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_mul_const< matrix< T, A >, value_type >( rhs, lhs ) );
}

template< class A, class Left, class Derive >
inline matrix_mul_const< matrix_expression< Left, A, Derive >, typename matrix_expression< Left, A, Derive >::value_type > operator *( const matrix_expression< Left, A, Derive > &lhs, const typename A::value_type &rhs )
{
	typedef typename matrix_expression< Left, A, Derive >::value_type value_type;
	return( matrix_mul_const< matrix_expression< Left, A, Derive >, value_type >( lhs, rhs ) );
}

template< class A, class Right, class Derive >
inline matrix_mul_const< matrix_expression< Right, A, Derive >, typename matrix_expression< Right, A, Derive >::value_type > operator *( const typename A::value_type &lhs, const matrix_expression< Right, A, Derive > &rhs )
{
	typedef typename matrix_expression< Right, A, Derive >::value_type value_type;
	return( matrix_mul_const< matrix_expression< Right, A, Derive >, value_type >( rhs, lhs ) );
}


// 割り算
template< class T, class A >
inline matrix_div_const< matrix< T, A >, typename matrix< T, A >::value_type > operator /( const matrix< T, A > &lhs, const typename type_trait< T >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_div_const< matrix< T, A >, value_type >( lhs, rhs ) );
}

template< class A, class Left, class Derive >
inline matrix_div_const< matrix_expression< Left, A, Derive >, typename matrix_expression< Left, A, Derive >::value_type > operator /( const matrix_expression< Left, A, Derive > &lhs, const typename A::value_type &rhs )
{
	typedef typename matrix_expression< Left, A, Derive >::value_type value_type;
	return( matrix_div_const< matrix_expression< Left, A, Derive >, value_type >( lhs, rhs ) );
}


#else


// 足し算
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) += m2 );
}


// 引き算
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) -= m2 );
}


// 掛け算
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
#ifdef _CHECK_MATRIX_OPERATION_
	if( m1.cols( ) != m2.rows( ) )
	{
		// 掛け算できません例外
		::std::cout << "can't multiply matrices." << ::std::endl;
		return( m1 );
	}
#endif

	matrix< T, Allocator > mat( m1.rows( ), m2.cols( ) );
	typename matrix< T, Allocator >::size_type r, c, t;

	for( r = 0 ; r < mat.rows( ) ; r++ )
	{
		for( c = 0 ; c < mat.cols( ) ; c++ )
		{
			for( t = 0 ; t < m1.cols( ) ; t++ )
			{
				mat( r, c ) += m1( r, t ) * m2( t, c );
			}
		}
	}

	return( mat );
}


// 定数との足し算
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) += val );
}

template < class T, class Allocator >
inline matrix< T, Allocator > operator +( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) += val );
}



// 定数との引き
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) -= val );
}

template < class T, class Allocator >
inline matrix< T, Allocator > operator -( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) -= val );
}


// 定数との掛け算
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) *= val );
}

template < class T, class Allocator >
inline matrix< T, Allocator > operator *( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) *= val );
}


// 定数との割り算
template < class T, class Allocator >
inline matrix< T, Allocator > operator /( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) /= val );
}

#endif



// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_MATRIX__