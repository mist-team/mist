/****************************************************************************************************************************
**
**         MIST ( Media Integration Standard Toolkit )
**
**         matrix template class implementation using expression template teqnique.
**
**
**           All matrix operations are described in row order.
**           All matrix elements are allocated as one dimensional array on the memory space.
**
**             ex) The order of 3x3 matrix are
**
**                   1  4  7
**                   2  5  8
**                   3  6  9
**
**
**         use matrix( row, col ) accesses operation. 
**
**
**
**         We developed these programs since 2003/09/05.
**
**             $LastChangedDate$
**             $LastChangedRevision$
**             $LastChangedBy$
**             $HeadURL$
**
**              Copyright ***********************.
**                    All Rights Reserved.
**
****************************************************************************************************************************/



/// @file mist/matrix.h
//!
//! @brief 一般行列を扱うためのクラス
//!
#ifndef __INCLUDE_MIST_MATRIX__
#define __INCLUDE_MIST_MATRIX__

#include "mist.h"
#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN



#if _USE_EXPRESSION_TEMPLATE_ != 0

template < class T > struct matrix_minus;
template < class T > struct matrix_transpose;
template < class T, class Allocator > class matrix;

template< class T >
struct matrix_expression
{
	typedef typename T::value_type		value_type;
	typedef typename T::size_type		size_type;
	typedef typename T::allocator_type	allocator_type;

	T expression;

	value_type operator()( size_type r, size_type c ) const { return( expression( r, c ) ); }
	value_type operator[]( size_type indx ) const { return( expression[ indx ] ); }
	size_type size( ) const { return( expression.size( ) ); };
	size_type rows( ) const { return( expression.rows( ) ); };
	size_type cols( ) const { return( expression.cols( ) ); };

	explicit matrix_expression( const T &exp ) : expression( exp ){ }

	matrix_expression< matrix_minus< matrix_expression< T > > > operator -() const
	{
		return( matrix_expression< matrix_minus< matrix_expression< T > > >( matrix_minus< matrix_expression< T > >( *this ) ) );
	}
	matrix_expression< matrix_transpose< matrix_expression< T > > > t( ) const
	{
		return( matrix_expression< matrix_transpose< matrix_expression< T > > >( matrix_transpose< matrix_expression< T > >( *this ) ) );
	}
};

// 単項演算子
template< class T >
struct matrix_single_operation
{
	typedef typename T::value_type		value_type;
	typedef typename T::size_type		size_type;
	typedef typename T::allocator_type	allocator_type;

	const T &middle_;

	explicit matrix_single_operation( const T &mhs ) : middle_( mhs ){}
};

// ２項演算子
template< class T1, class T2 >
struct matrix_bind_operation 
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;

	const T1 &left_;
	const T2 &right_;

	matrix_bind_operation( const T1 &lhs, const T2 &rhs ) : left_( lhs ), right_( rhs ){}
};


// 負の符号オペレータ
template< class T >
struct matrix_minus : public matrix_single_operation< T >
{
	typedef typename T::value_type		value_type;
	typedef typename T::size_type		size_type;
	typedef typename T::allocator_type	allocator_type;
	typedef matrix_single_operation< T > base;

	explicit matrix_minus( const T &mhs ) : base( mhs ){}
	size_type size( ) const { return( base::middle_.size( ) ); };
	size_type rows( ) const { return( base::middle_.rows( ) ); }
	size_type cols( ) const { return( base::middle_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( - base::middle_( r, c ) ); }
	value_type operator[]( size_type indx ) const { return( - base::middle_[ indx ] ); }
};


// 転置オペレータ
template< class T >
struct matrix_transpose : public matrix_single_operation< T >
{
	typedef typename T::value_type		value_type;
	typedef typename T::size_type		size_type;
	typedef typename T::allocator_type	allocator_type;
	typedef matrix_single_operation< T > base;

	explicit matrix_transpose( const T &mhs ) : base( mhs ){}
	size_type size( ) const { return( base::middle_.size( ) ); };
	size_type rows( ) const { return( base::middle_.cols( ) ); }
	size_type cols( ) const { return( base::middle_.rows( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( base::middle_( c, r ) ); }
	value_type operator[]( size_type indx ) const { return( base::middle_[ indx ] ); }
};


// 足し算オペレータ
template< class T1, class T2 >
struct matrix_add : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_add( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( lhs.rows( ) != rhs.rows( ) || lhs.cols( ) != rhs.cols( ) )
		{
			// 足し算できません例外
			::std::cout << "足し算を行おうとする行列の行と列の数が一致しません．" << ::std::endl;
		}
#endif
	}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( base::left_( r, c ) + base::right_( r, c ) ); }
	value_type operator[]( size_type indx ) const { return( base::left_[ indx ] + base::right_[ indx ] ); }
};


// 引き算オペレータ
template< class T1, class T2 >
struct matrix_sub : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_sub( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( lhs.rows( ) != rhs.rows( ) || lhs.cols( ) != rhs.cols( ) )
		{
			// 引き算できません例外
			::std::cout << "引き算を行おうとする行列の行と列の数が一致しません．" << ::std::endl;
		}
#endif
	}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( base::left_( r, c ) - base::right_( r, c ) ); }
	value_type operator[]( size_type indx ) const { return( base::left_[ indx ] - base::right_[ indx ] ); }
};


//掛け算オペレータ
template< class T1, class T2 >
struct matrix_mul : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_mul( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( lhs.cols( ) != rhs.rows( ) )
		{
			// 掛け算できません例外
			::std::cout << "掛け算を行おうとする行列の行と列の数が一致しません．" << ::std::endl;
		}
#endif
	}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::right_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const
	{
		value_type v = 0;
		size_type size = base::left_.cols( );
		for( size_type t = 0 ; t < size ; ++t )
		{
			v += base::left_( r, t ) * base::right_( t, c );
		}
		return( v );
	}
	value_type operator[]( size_type indx ) const { return( base::left_[ indx ] * base::right_[ indx ] ); }
};


// 定数の足し算オペレータ
template< class T1, class T2 >
struct matrix_add_const : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_add_const( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs ){}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( base::left_( r, c ) + ( r == c ? base::right_ : 0 ) ); }
	value_type operator[]( size_type indx ) const { return( base::left_[ indx ] + base::right_ ); }
};

// 定数の足し算オペレータ（右側が定数）
template< class T1, class T2 >
struct matrix_sub_const1 : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_sub_const1( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs ){}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( base::left_( r, c ) - ( r == c ? base::right_ : 0 ) ); }
	value_type operator[]( size_type indx ) const { return( base::left_[ indx ] - base::right_ ); }
};

// 定数の足し算オペレータ（左側が定数）
template< class T1, class T2 >
struct matrix_sub_const2 : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_sub_const2( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs ){}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( ( r == c ? base::right_ : 0 ) - base::left_( r, c ) ); }
	value_type operator[]( size_type indx ) const { return( base::right_[ indx ] - base::left_ ); }
};

// 定数の掛け算オペレータ
template< class T1, class T2 >
struct matrix_mul_const : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_mul_const( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs ){}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( base::left_( r, c ) * base::right_ ); }
	value_type operator[]( size_type indx ) const { return( base::left_[ indx ] * base::right_ ); }
};

// 定数の割り算オペレータ
template< class T1, class T2 >
struct matrix_div_const : public matrix_bind_operation< T1, T2 >
{
	typedef typename T1::value_type		value_type;
	typedef typename T1::size_type		size_type;
	typedef typename T1::allocator_type	allocator_type;
	typedef matrix_bind_operation< T1, T2 > base;

	matrix_div_const( const T1 &lhs, const T2 &rhs ) : base( lhs, rhs )
	{
#ifdef _CHECK_MATRIX_OPERATION_
		if( rhs == 0 )
		{
			// ゼロ除算を行おうとしている例外
			::std::cout << "ゼロ除算を行おうとしています．" << ::std::endl;
		}
#endif
	}
	size_type size( ) const { return( base::left_.size( ) ); };
	size_type rows( ) const { return( base::left_.rows( ) ); }
	size_type cols( ) const { return( base::left_.cols( ) ); }
	value_type operator()( size_type r, size_type c ) const { return( base::left_( r, c ) / base::right_ ); }
	value_type operator[]( size_type indx ) const { return( base::left_[ indx ] / base::right_ ); }
};

#endif


/// @brief 行列クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator = ::std::allocator< T > >
class matrix : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

	/// @brief MISTのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MISTのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef mist_iterator1< T, ptrdiff_t, pointer, const_reference > const_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MISTのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, const_reference > > const_reverse_iterator;


private:
	typedef array< T, Allocator > base;
	size_type size1_;
	size_type size2_;

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] num … 引数の説明
	//! 
	void resize( size_type num )
	{
		size1_ = num;
		size2_ = 1;
		base::resize( size1_ * size2_ );
	}

	//void resize( size_type num, const T &val )
	//{
	//	size1_ = num;
	//	size2_ = 1;
	//	base::resize( size1_ * size2_, val );
	//}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] num1 … 引数の説明
	//! @param[in] num2 … 引数の説明
	//! 
	void resize( size_type num1, size_type num2 )
	{
		size1_ = num1;
		size2_ = num2;
		base::resize( size1_ * size2_ );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] num1 … 引数の説明
	//! @param[in] num2 … 引数の説明
	//! @param[in] val  … 引数の説明
	//! 
	void resize( size_type num1, size_type num2, const T &val )
	{
		size1_ = num1;
		size2_ = num2;
		base::resize( size1_ * size2_, val );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}

	size_type size1( ) const { return( size1_ ); }	///< @brief 行数を返す
	size_type size2( ) const { return( size2_ ); }	///< @brief 列数を返す
	size_type rows( ) const { return( size1_ ); }	///< @brief 行数を返す
	size_type cols( ) const { return( size2_ ); }	///< @brief 列数を返す



/************************************************************************************************************
**
**      行に対する順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	iterator row_begin( size_type r ){ return( iterator( paccess( r, 0 ), rows( ) ) ); }

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_iterator row_begin( size_type r ) const { return( const_iterator( paccess( r, 0 ), rows( ) ) ); }

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	iterator row_end( size_type r ){ return( iterator( paccess( r, cols( ) ), rows( ) ) ); }

	/// @brief 順方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_iterator row_end( size_type r ) const { return( const_iterator( paccess( r, cols( ) ), rows( ) ) ); }


	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	reverse_iterator row_rbegin( size_type r ){ return( reverse_iterator( row_end( r )  ) ); }

	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_reverse_iterator row_rbegin( size_type r ) const { return( const_reverse_iterator( row_end( r ) ) ); }

	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	reverse_iterator row_rend( size_type r ){ return( reverse_iterator( row_begin( r ) ) ); }

	/// @brief 逆方向のランダムアクセスイテレータを返す
	//! 
	//! @param[in] r  … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_reverse_iterator row_rend( size_type r ) const { return( const_reverse_iterator( row_begin( r ) ) ); }


/************************************************************************************************************
**
**      列に対する順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 順方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	iterator col_begin( size_type c ){ return( iterator( paccess( 0, c ), 1 ) ); }

	/// @brief 順方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	const_iterator col_begin( size_type c ) const { return( const_iterator( paccess( 0, c ), 1 ) ); }

	/// @brief 順方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	iterator col_end( size_type c ){ return( iterator( paccess( rows( ), c ), 1 ) ); }

	/// @brief 順方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	const_iterator col_end( size_type c ) const { return( const_iterator( paccess( rows( ), c ), 1 ) ); }


	/// @brief 逆方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	reverse_iterator col_rbegin( size_type c ){ return( reverse_iterator( col_end( c ) ) ); }

	/// @brief 逆方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	const_reverse_iterator col_rbegin( size_type c ) const { return( const_reverse_iterator(  col_end( c ) ) ); }

	/// @brief 逆方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	reverse_iterator col_rend( size_type c ){ return( reverse_iterator( col_begin( c ) ) ); }

	/// @brief 逆方向のランダムアクセスイテレータを返す
	//!
	//! @param[in] c   … 引数の説明
	//!
	//! @return 戻り値の説明
	//!
	const_reverse_iterator col_rend( size_type c ) const { return( const_reverse_iterator( col_begin( c ) ) ); }





public: // 配列に対する算術演算

	
/************************************************************************************************************
**
**      符号反転オペレータと転置行列オペレータ
**
************************************************************************************************************/
#if _USE_EXPRESSION_TEMPLATE_ != 0

	/// @brief 符号反転
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	matrix_expression< matrix_minus< matrix > > operator -( ) const
	{
		return( matrix_expression< matrix_minus< matrix > >( matrix_minus< matrix >( *this ) ) );
	}

	/// @brief 転置行列
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	matrix_expression< matrix_transpose< matrix > > t( ) const
	{
		return( matrix_expression< matrix_transpose< matrix > >( matrix_transpose< matrix >( *this ) ) );
	}

#else

	/// @brief 符号反転
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	matrix operator -( ) const
	{
		const matrix &m = *this;
		matrix o( size1_, size2_ );
		for( size_type i = 0 ; i < o.size( ) ; i++ )
		{
			o[ i ] = -m[ i ];
		}
		return( o );
	}

	/// @brief 転置行列
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	matrix t( ) const
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

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] m2 … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] m2 … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] m2 … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] val … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const matrix& operator +=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) += val;
		return( m );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] val … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const matrix& operator -=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) -= val;
		return( m );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] val … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const matrix& operator *=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		for( size_type i = 0 ; i < m.size( ) ; i++ ) m[i] *= val;
		return( m );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] val … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const matrix& operator /=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
#ifdef _CHECK_ARRAY_OPERATION_
		if( val == 0 )
		{
			// ゼロ除算発生
			::std::cout << "zero division occured." << ::std::endl;
			return( m );
		}
#endif
		for( size_type i = 0 ; i < m.size( ) ; i++ ) m[i] /= val;
		return( m );
	}

#if _USE_EXPRESSION_TEMPLATE_ != 0


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] m2 … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	template < class Expression >
	matrix& operator +=( const matrix_expression< Expression > &m2 )
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] m2 … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	template < class Expression >
	matrix& operator -=( const matrix_expression< Expression > &m2 )
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] m2 … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	template < class Expression >
	matrix& operator *=( const matrix_expression< Expression > &m2 )
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
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] m … 引数の説明
	//! 
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
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] o … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	template < class TT, class AAlocator >
	const matrix& operator =( const matrix< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );

		return( *this );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] o … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const matrix< T, Allocator >& operator =( const matrix< T, Allocator > &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );

		return( *this );
	}

#if _USE_EXPRESSION_TEMPLATE_ != 0
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] expression … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	template < class Expression >
	const matrix& operator =( const matrix_expression< Expression > &expression )
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

// 要素へのアクセス
private:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] r … 引数の説明
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	pointer paccess( size_type r, size_type c )
	{
		return( base::data_ + r + c * size1_ );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] r … 引数の説明
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_pointer paccess( size_type r, size_type c ) const
	{
		return( base::data_ + r + c * size1_ );
	}


public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] r … 引数の説明
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	reference at( size_type r, size_type c )
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] r … 引数の説明
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_reference at( size_type r, size_type c ) const
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] r … 引数の説明
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	reference operator ()( size_type r, size_type c )
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] r … 引数の説明
	//! @param[in] c … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const_reference operator ()( size_type r, size_type c ) const
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}


public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	matrix( ) : base( ), size1_( 0 ), size2_( 0 ) {}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] a … 引数の説明
	//! 
	explicit matrix( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ) {}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] rnum … 引数の説明
	//! @param[in] cnum … 引数の説明
	//! 
	matrix( size_type rnum, size_type cnum ) : base( rnum * cnum ), size1_( rnum ), size2_( cnum ) {}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] rnum … 引数の説明
	//! @param[in] cnum … 引数の説明
	//! @param[in] a    … 引数の説明
	//! 
	matrix( size_type rnum, size_type cnum, const Allocator &a ) : base( rnum * cnum, a ), size1_( rnum ), size2_( cnum ) {}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] rnum … 引数の説明
	//! @param[in] cnum … 引数の説明
	//! @param[in] val  … 引数の説明
	//! 
	matrix( size_type rnum, size_type cnum, const T &val ) : base( rnum * cnum, val ), size1_( rnum ), size2_( cnum ) {}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] rnum … 引数の説明
	//! @param[in] cnum … 引数の説明
	//! @param[in] val  … 引数の説明
	//! @param[in] a    … 引数の説明
	//! 
	matrix( size_type rnum, size_type cnum, const T &val, const Allocator &a ) : base( rnum * cnum, val, a ), size1_( rnum ), size2_( cnum ) {}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] o … 引数の説明
	//! 
	template < class TT, class AAlocator >
	matrix( const matrix< TT, AAlocator > &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] o … 引数の説明
	//! 
	matrix( const matrix< T, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ){ }


#if _USE_EXPRESSION_TEMPLATE_ != 0
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] expression … 引数の説明
	//! 
	template < class Expression >
	matrix( const matrix_expression< Expression > &expression ) : base( expression.rows( ) * expression.cols( ) ), size1_( expression.rows( ) ), size2_( expression.cols( ) )
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



/// @brief 標準出力への行列の出力
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in,out] out … 引数の説明
//! @param[in]     m   … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
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


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in,out] out … 引数の説明
//! @param[in]     m   … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T >
inline ::std::ostream &operator <<( ::std::ostream &out,  const matrix_expression< T > &m )
{
	typedef typename matrix_expression< T >::allocator_type	allocator_type;
	typedef typename matrix_expression< T >::value_type		value_type;
	return( operator <<( out, matrix< value_type, allocator_type >( m ) ) );
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
inline matrix_expression< matrix_add< matrix< T, A >, matrix< T, A > > > operator +( const matrix< T, A > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_expression< matrix_add< matrix< T, A >, matrix< T, A > > >( matrix_add< matrix< T, A >, matrix< T, A > >( lhs, rhs ) ) );
}

template< class T, class A, class Left >
inline matrix_expression< matrix_add< matrix_expression< Left >, matrix< T, A > > > operator +( const matrix_expression< Left > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_expression< matrix_add< matrix_expression< Left >, matrix< T, A > > >( matrix_add< matrix_expression< Left >, matrix< T, A > >( lhs, rhs ) ) );
}

template< class T, class A, class Right >
inline matrix_expression< matrix_add< matrix< T, A >, matrix_expression< Right > > > operator +( const matrix< T, A > &lhs, const matrix_expression< Right > &rhs )
{
	return( matrix_expression< matrix_add< matrix< T, A >, matrix_expression< Right > > >( matrix_add< matrix< T, A >, matrix_expression< Right > >( lhs, rhs ) ) );
}

template< class Left, class Right >
inline matrix_expression< matrix_add< matrix_expression< Left >, matrix_expression< Right > > > operator +( const matrix_expression< Left > &lhs, const matrix_expression< Right > &rhs )
{
	return( matrix_expression< matrix_add< matrix_expression< Left >, matrix_expression< Right > > >( matrix_add< matrix_expression< Left >, matrix_expression< Right > >( lhs, rhs ) ) );
}

template< class T, class A >
inline matrix_expression< matrix_add_const< matrix< T, A >, typename matrix< T, A >::value_type > > operator +( const matrix< T, A > &lhs, const typename matrix< T, A >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_expression< matrix_add_const< matrix< T, A >, typename matrix< T, A >::value_type > >( matrix_add_const< matrix< T, A >, value_type >( lhs, rhs ) ) );
}

template< class T, class A >
inline matrix_expression< matrix_add_const< matrix< T, A >, typename matrix< T, A >::value_type > > operator +( const typename matrix< T, A >::value_type &lhs, const matrix< T, A > &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_expression< matrix_add_const< matrix< T, A >, typename matrix< T, A >::value_type > >( matrix_add_const< matrix< T, A >, value_type >( rhs, lhs ) ) );
}

template< class Left >
inline matrix_expression< matrix_add_const< matrix_expression< Left >, typename matrix_expression< Left >::value_type > >
								operator +( const matrix_expression< Left > &lhs, const typename matrix_expression< Left >::value_type &rhs )
{
	typedef typename matrix_expression< Left >::value_type value_type;
	return( matrix_expression< matrix_add_const< matrix_expression< Left >, typename matrix_expression< Left >::value_type > >( matrix_add_const< matrix_expression< Left >, value_type >( lhs, rhs ) ) );
}

template< class Right >
inline matrix_expression< matrix_add_const< matrix_expression< Right >, typename matrix_expression< Right >::value_type > >
								operator +( const typename matrix_expression< Right >::value_type &lhs, const matrix_expression< Right > &rhs )
{
	typedef typename matrix_expression< Right >::value_type value_type;
	return( matrix_expression< matrix_add_const< matrix_expression< Right >, typename matrix_expression< Right >::value_type > >( matrix_add_const< matrix_expression< Right >, value_type >( rhs, lhs ) ) );
}


// 引き算
template< class T, class A >
inline matrix_expression< matrix_sub< matrix< T, A >, matrix< T, A > > > operator -( const matrix< T, A > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_expression< matrix_sub< matrix< T, A >, matrix< T, A > > >( matrix_sub< matrix< T, A >, matrix< T, A > >( lhs, rhs ) ) );
}

template< class T, class A, class Left >
inline matrix_expression< matrix_sub< matrix_expression< Left >, matrix< T, A > > > operator -( const matrix_expression< Left > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_expression< matrix_sub< matrix_expression< Left >, matrix< T, A > > >( matrix_sub< matrix_expression< Left >, matrix< T, A > >( lhs, rhs ) ) );
}

template< class T, class A, class Right >
inline matrix_expression< matrix_sub< matrix< T, A >, matrix_expression< Right > > > operator -( const matrix< T, A > &lhs, const matrix_expression< Right > &rhs )
{
	return( matrix_expression< matrix_sub< matrix< T, A >, matrix_expression< Right > > >( matrix_sub< matrix< T, A >, matrix_expression< Right > >( lhs, rhs ) ) );
}

template< class Left, class Right >
inline matrix_expression< matrix_sub< matrix_expression< Left >, matrix_expression< Right > > > operator -( const matrix_expression< Left > &lhs, const matrix_expression< Right > &rhs )
{
	return( matrix_expression< matrix_sub< matrix_expression< Left >, matrix_expression< Right > > >( matrix_sub< matrix_expression< Left >, matrix_expression< Right > >( lhs, rhs ) ) );
}

template< class T, class A >
inline matrix_expression< matrix_sub_const1< matrix< T, A >, typename matrix< T, A >::value_type > > operator -( const matrix< T, A > &lhs, const typename matrix< T, A >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_expression< matrix_sub_const1< matrix< T, A >, typename matrix< T, A >::value_type > >( matrix_sub_const1< matrix< T, A >, value_type >( lhs, rhs ) ) );
}

template< class T, class A >
inline matrix_expression< matrix_sub_const2< matrix< T, A >, typename matrix< T, A >::value_type > > operator -( const typename matrix< T, A >::value_type &lhs, const matrix< T, A > &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_expression< matrix_sub_const2< matrix< T, A >, typename matrix< T, A >::value_type > >( matrix_sub_const2< matrix< T, A >, value_type >( rhs, lhs ) ) );
}

template< class Left >
inline matrix_expression< matrix_sub_const1< matrix_expression< Left >, typename matrix_expression< Left >::value_type > >
					operator -( const matrix_expression< Left > &lhs, const typename matrix_expression< Left >::value_type &rhs )
{
	typedef typename matrix_expression< Left >::value_type value_type;
	return( matrix_expression< matrix_sub_const1< matrix_expression< Left >, typename matrix_expression< Left >::value_type > >( matrix_sub_const1< matrix_expression< Left >, value_type >( lhs, rhs ) ) );
}

template< class Right >
inline matrix_expression< matrix_sub_const2< matrix_expression< Right >, typename matrix_expression< Right >::value_type > >
					operator -( const typename matrix_expression< Right >::value_type &lhs, const matrix_expression< Right > &rhs )
{
	typedef typename matrix_expression< Right >::value_type value_type;
	return( matrix_expression< matrix_sub_const2< matrix_expression< Right >, typename matrix_expression< Right >::value_type > >( matrix_sub_const2< matrix_expression< Right >, value_type >( rhs, lhs ) ) );
}


// 掛け算
template< class T, class A >
inline matrix_expression< matrix_mul< matrix< T, A >, matrix< T, A > > > operator *( const matrix< T, A > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_expression< matrix_mul< matrix< T, A >, matrix< T, A > > >( matrix_mul< matrix< T, A >, matrix< T, A > >( lhs, rhs ) ) );
}

template< class T, class A, class Left >
inline matrix_expression< matrix_mul< matrix_expression< Left >, matrix< T, A > > > operator *( const matrix_expression< Left > &lhs, const matrix< T, A > &rhs )
{
	return( matrix_expression< matrix_mul< matrix_expression< Left >, matrix< T, A > > >( matrix_mul< matrix_expression< Left >, matrix< T, A > >( lhs, rhs ) ) );
}

template< class T, class A, class Right >
inline matrix_expression< matrix_mul< matrix< T, A >, matrix_expression< Right > > > operator *( const matrix< T, A > &lhs, const matrix_expression< Right > &rhs )
{
	return( matrix_expression< matrix_mul< matrix< T, A >, matrix_expression< Right > > >( matrix_mul< matrix< T, A >, matrix_expression< Right > >( lhs, rhs ) ) );
}

template< class Left, class Right >
inline matrix_expression< matrix_mul< matrix_expression< Left >, matrix_expression< Right > > > operator *( const matrix_expression< Left > &lhs, const matrix_expression< Right > &rhs )
{
	return( matrix_expression< matrix_mul< matrix_expression< Left >, matrix_expression< Right > > >( matrix_mul< matrix_expression< Left >, matrix_expression< Right > >( lhs, rhs ) ) );
}

template< class T, class A >
inline matrix_expression< matrix_mul_const< matrix< T, A >, typename matrix< T, A >::value_type > > operator *( const matrix< T, A > &lhs, const typename matrix< T, A >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_expression< matrix_mul_const< matrix< T, A >, typename matrix< T, A >::value_type > >( matrix_mul_const< matrix< T, A >, value_type >( lhs, rhs ) ) );
}

template< class T, class A >
inline matrix_expression< matrix_mul_const< matrix< T, A >, typename matrix< T, A >::value_type > > operator *( const typename matrix< T, A >::value_type &lhs, const matrix< T, A > &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_expression< matrix_mul_const< matrix< T, A >, typename matrix< T, A >::value_type > >( matrix_mul_const< matrix< T, A >, value_type >( rhs, lhs ) ) );
}

template< class Left >
inline matrix_expression< matrix_mul_const< matrix_expression< Left >, typename matrix_expression< Left >::value_type > > operator *( const matrix_expression< Left > &lhs, const typename matrix_expression< Left >::value_type &rhs )
{
	typedef typename matrix_expression< Left >::value_type value_type;
	return( matrix_expression< matrix_mul_const< matrix_expression< Left >, typename matrix_expression< Left >::value_type > >( matrix_mul_const< matrix_expression< Left >, value_type >( lhs, rhs ) ) );
}

template< class Right >
inline matrix_expression< matrix_mul_const< matrix_expression< Right >, typename matrix_expression< Right >::value_type > > operator *( const typename matrix_expression< Right >::value_type &lhs, const matrix_expression< Right > &rhs )
{
	typedef typename matrix_expression< Right >::value_type value_type;
	return( matrix_expression< matrix_mul_const< matrix_expression< Right >, typename matrix_expression< Right >::value_type > >( matrix_mul_const< matrix_expression< Right >, value_type >( rhs, lhs ) ) );
}


// 割り算
template< class T, class A >
inline matrix_expression< matrix_div_const< matrix< T, A >, typename matrix< T, A >::value_type > > operator /( const matrix< T, A > &lhs, const typename matrix< T, A >::value_type &rhs )
{
	typedef typename matrix< T, A >::value_type value_type;
	return( matrix_expression< matrix_div_const< matrix< T, A >, typename matrix< T, A >::value_type > >( matrix_div_const< matrix< T, A >, value_type >( lhs, rhs ) ) );
}

template< class Left >
inline matrix_expression< matrix_div_const< matrix_expression< Left >, typename matrix_expression< Left >::value_type > > operator /( const matrix_expression< Left > &lhs, const typename matrix_expression< Left >::value_type &rhs )
{
	typedef typename matrix_expression< Left >::value_type value_type;
	return( matrix_expression< matrix_div_const< matrix_expression< Left >, typename matrix_expression< Left >::value_type > >( matrix_div_const< matrix_expression< Left >, value_type >( lhs, rhs ) ) );
}


#else


/// @brief 足し算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m1 … 引数の説明
//! @param[in] m2 … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) += m2 );
}


/// @brief 引き算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m1 … 引数の説明
//! @param[in] m2 … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) -= m2 );
}


/// @brief 掛け算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m1 … 引数の説明
//! @param[in] m2 … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
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


/// @brief 定数との足し算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) += val );
}

/// @brief 定数との足し算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] val … 引数の説明
//! @param[in] m   … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) += val );
}



/// @brief 定数との引き算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) -= val );
}

/// @brief 定数との引き算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] val … 引数の説明
//! @param[in] m   … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) -= val );
}


/// @brief 定数との掛け算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) *= val );
}

/// @brief 定数との掛け算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] val … 引数の説明
//! @param[in] m   … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) *= val );
}


// @brief 定数との割り算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m   … 引数の説明
//! @param[in] val … 引数の説明
//! 
//! @return        … 戻り値の説明
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator /( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) /= val );
}

#endif


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_MATRIX__
