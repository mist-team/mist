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

// 固定演算子
template< class T >
struct matrix_static_operation
{
	typedef typename T::value_type		value_type;
	typedef typename T::size_type		size_type;
	typedef typename T::allocator_type	allocator_type;

	const size_type rows_;
	const size_type cols_;

	size_type size( ) const { return( rows_ * cols_ ); };
	size_type rows( ) const { return( rows_ ); }
	size_type cols( ) const { return( cols_ ); }

	explicit matrix_static_operation( const size_type rows, const size_type cols ) : rows_( rows ), cols_( cols ){}
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


// 単位行列オペレータ
template< class T >
struct matrix_identity : public matrix_static_operation< T >
{
	typedef typename T::value_type		value_type;
	typedef typename T::size_type		size_type;
	typedef typename T::allocator_type	allocator_type;
	typedef matrix_static_operation< T > base;

	explicit matrix_identity( const size_type rows, const size_type cols ) : base( rows, cols ){}
	value_type operator()( size_type r, size_type c ) const { return( r == c ? 1 : 0 ); }
	value_type operator[]( size_type indx ) const { return( 0 ); }
};


// ゼロ行列オペレータ
template< class T >
struct matrix_zero : public matrix_static_operation< T >
{
	typedef typename T::value_type		value_type;
	typedef typename T::size_type		size_type;
	typedef typename T::allocator_type	allocator_type;
	typedef matrix_static_operation< T > base;

	explicit matrix_zero( const size_type rows, const size_type cols ): base( rows, cols ){}
	value_type operator()( size_type r, size_type c ) const { return( 0 ); }
	value_type operator[]( size_type indx ) const { return( 0 ); }
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


/// @brief 行と列の要素数が可変の行列
//! 
//! - _USE_EXPRESSION_TEMPLATE_  マクロが1の場合は，Expression Template を利用して，行列演算を高速化する
//! - _CHECK_MATRIX_OPERATION_   マクロが1の場合は，行列演算の左辺と右辺で行列のサイズが適切であるかをチェックする
//! - _CHECK_ACCESS_VIOLATION2_  マクロが1の場合は，行列の要素へアクセスする際に，範囲外かどうかを判定する
//! 
//! @param T         … 行列内に格納するデータ型
//! @param Allocator … MISTコンテナが利用するアロケータ型．省略した場合は，STLのデフォルトアロケータを使用する
//! 
template < class T, class Allocator = ::std::allocator< T > >
class matrix : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MISTコンテナが利用するアロケータ型
	typedef typename Allocator::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::matrix< data > の場合，data & となる
	typedef typename Allocator::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::matrix< data > の場合，const data & となる
	typedef typename Allocator::value_type value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::matrix< data > の data と同じ
	typedef typename Allocator::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename Allocator::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef typename Allocator::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::matrix< data > の場合，data * となる
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::matrix< data > の場合，const data * となる

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
	/// @brief 行列のサイズを num 行 1 列に変更する
	//! 
	//! 元の行列と異なる場合は，自動的にサイズを調整する．
	//! 全ての要素の値はデフォルトコンストラクタで初期化される．
	//! また，組み込み型（int や double など）の場合は，全ての要素を0で初期化する．
	//! 
	//! @param[in] num … リサイズ後の行列の行数
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

	/// @brief 行列のサイズを num1 行 num2 列に変更する
	//! 
	//! 元の行列サイズと異なる場合は，自動的にサイズを調整する．
	//! 全ての要素の値はデフォルトコンストラクタで初期化される．
	//! また，組み込み型（int や double など）の場合は，全ての要素を0で初期化する．
	//! 
	//! @param[in] num1 … リサイズ後の行列の行数
	//! @param[in] num2 … リサイズ後の行列の列数
	//! 
	void resize( size_type num1, size_type num2 )
	{
		size1_ = num1;
		size2_ = num2;
		base::resize( size1_ * size2_ );
	}

	/// @brief 行列のサイズを num1 行 num2 列に変更する
	//! 
	//! 行列サイズを num1 行 num2 列に変更し，全ての要素を値 val で初期化する．
	//! 
	//! @param[in] num1 … リサイズ後の行列の行数
	//! @param[in] num2 … リサイズ後の行列の列数
	//! @param[in] val  … リサイズ後に各要素を初期化する値
	//! 
	void resize( size_type num1, size_type num2, const T &val )
	{
		size1_ = num1;
		size2_ = num2;
		base::resize( size1_ * size2_, val );
	}

	/// @brief 行列内の全ての内容を入れ替える．
	//! 
	//! 入れ替え元の行列 m の中身と全て入れ替える
	//! 
	//! @param[in] m … 内容を入れ替える対象
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

	/// @brief 行列の要素を空にする
	//! 
	//! 行列に格納されているデータを全て削除し，行列を空（0行0列）にする
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

	/// @brief 行方向を操作する順方向ランダムアクセスイテレータを返す
	//!
	//! @code 行方向を操作する順方向ランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_begin( 1 );
	//! for( ; ite1 != a.row_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator row_begin( size_type r ){ return( iterator( paccess( r, 0 ), rows( ) ) ); }

	/// @brief 行方向を操作するコンスト型の順方向ランダムアクセスイテレータを返す
	const_iterator row_begin( size_type r ) const { return( const_iterator( paccess( r, 0 ), rows( ) ) ); }

	/// @brief 行方向を操作する順方向ランダムアクセスイテレータを返す
	iterator row_end( size_type r ){ return( iterator( paccess( r, cols( ) ), rows( ) ) ); }

	/// @brief 行方向を操作するコンスト型の順方向ランダムアクセスイテレータを返す
	const_iterator row_end( size_type r ) const { return( const_iterator( paccess( r, cols( ) ), rows( ) ) ); }


	/// @brief 行方向を操作する逆方向ランダムアクセスイテレータを返す
	//!
	//! @code 行方向を操作する逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_rbegin( 1 );
	//! for( ; ite1 != a.row_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator row_rbegin( size_type r ){ return( reverse_iterator( row_end( r )  ) ); }

	/// @brief 行方向を操作するコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator row_rbegin( size_type r ) const { return( const_reverse_iterator( row_end( r ) ) ); }

	/// @brief 行方向を操作する逆方向ランダムアクセスイテレータを返す
	reverse_iterator row_rend( size_type r ){ return( reverse_iterator( row_begin( r ) ) ); }

	/// @brief 行方向を操作するコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator row_rend( size_type r ) const { return( const_reverse_iterator( row_begin( r ) ) ); }


/************************************************************************************************************
**
**      列に対する順方向・逆方向の反復子
**
************************************************************************************************************/

	/// @brief 列方向を操作する順方向ランダムアクセスイテレータを返す
	//!
	//! @code 列方向を操作する順方向ランダムアクセスイテレータの使用例
	//! std::cout << "順方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_begin( 1 );
	//! for( ; ite1 != a.row_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator col_begin( size_type c ){ return( iterator( paccess( 0, c ), 1 ) ); }

	/// @brief 列方向を操作するコンスト型の順方向ランダムアクセスイテレータを返す
	const_iterator col_begin( size_type c ) const { return( const_iterator( paccess( 0, c ), 1 ) ); }

	/// @brief 列方向を操作する順方向ランダムアクセスイテレータを返す
	iterator col_end( size_type c ){ return( iterator( paccess( rows( ), c ), 1 ) ); }

	/// @brief 列方向を操作するコンスト型の順方向ランダムアクセスイテレータを返す
	const_iterator col_end( size_type c ) const { return( const_iterator( paccess( rows( ), c ), 1 ) ); }


	/// @brief 列方向を操作する逆方向ランダムアクセスイテレータを返す
	//!
	//! @code 列方向を操作する逆方向ランダムアクセスイテレータの使用例
	//! std::cout << "逆方向ランダムアクセスイテレータ" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_rbegin( 1 );
	//! for( ; ite1 != a.row_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator col_rbegin( size_type c ){ return( reverse_iterator( col_end( c ) ) ); }

	/// @brief 列方向を操作するコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator col_rbegin( size_type c ) const { return( const_reverse_iterator(  col_end( c ) ) ); }

	/// @brief 列方向を操作する逆方向ランダムアクセスイテレータを返す
	reverse_iterator col_rend( size_type c ){ return( reverse_iterator( col_begin( c ) ) ); }

	/// @brief 列方向を操作するコンスト型の逆方向ランダムアクセスイテレータを返す
	const_reverse_iterator col_rend( size_type c ) const { return( const_reverse_iterator( col_begin( c ) ) ); }





public:

	
// 単位行列とゼロ行列を生成する
#if _USE_EXPRESSION_TEMPLATE_ != 0

	/// @brief 任意サイズの単位行列を返す
	static matrix_expression< matrix_identity< matrix > > identity( size_type rows, size_type cols )
	{
		return( matrix_expression< matrix_identity< matrix > >( matrix_identity< matrix >( rows, cols ) ) );
	}

	/// @brief 任意サイズのゼロ行列を返す
	static matrix_expression< matrix_zero< matrix > > zero( size_type rows, size_type cols )
	{
		return( matrix_expression< matrix_zero< matrix > >( matrix_zero< matrix >( rows, cols ) ) );
	}

#else

	/// @brief 任意サイズの単位行列を返す
	static const matrix identity( size_type rows, size_type cols )
	{
		size_type size = rows < cols ? rows : cols;
		matrix o( rows, cols );
		for( size_type i = 0 ; i < size ; i++ )
		{
			o( i, i ) = 1;
		}
		return( o );
	}

	/// @brief 任意サイズのゼロ行列を返す
	static const matrix zero( size_type rows, size_type cols )
	{
		return( matrix( rows, cols ) );
	}

#endif


// 符号反転オペレータと転置行列オペレータ
#if _USE_EXPRESSION_TEMPLATE_ != 0

	/// @brief 符号反転した行列を返す
	matrix_expression< matrix_minus< matrix > > operator -( ) const
	{
		return( matrix_expression< matrix_minus< matrix > >( matrix_minus< matrix >( *this ) ) );
	}

	/// @brief 転置行列を返す
	matrix_expression< matrix_transpose< matrix > > t( ) const
	{
		return( matrix_expression< matrix_transpose< matrix > >( matrix_transpose< matrix >( *this ) ) );
	}

#else

	/// @brief 符号反転した行列を返す
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

	/// @brief 転置行列を返す
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


// 行列に対する演算子
//   += 行列
//   += 定数
//   -= 行列
//   -= 定数
//   *= 行列
//   *= 定数
//   /= 定数

	/// @brief 行列の足し算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} + {\bf B}
	//! \f]
	//! 
	//! @param[in] m2 … 足し算を行う行列
	//! 
	//! @return 自分自身
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


	/// @brief 行列の引き算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} - {\bf B}
	//! \f]
	//! 
	//! @param[in] m2 … 引き算を行う行列
	//! 
	//! @return 自分自身
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


	/// @brief 行列の引き算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} * {\bf B}
	//! \f]
	//! 
	//! @param[in] m2 … 右から掛ける行列
	//! 
	//! @return 自分自身
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


	/// @brief 行列への定数（単位行列の定数倍）の足し算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} + val * {\bf I}
	//! \f]
	//! 
	//! @param[in] val … 単位行列の定数倍を表す数
	//! 
	//! @return 自分自身
	//! 
	const matrix& operator +=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) += val;
		return( m );
	}


	/// @brief 行列への定数（単位行列の定数倍）の足し算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} - val * {\bf I}
	//! \f]
	//! 
	//! @param[in] val … 単位行列の定数倍を表す数
	//! 
	//! @return 自分自身
	//! 
	const matrix& operator -=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) -= val;
		return( m );
	}


	/// @brief 行列への定数（単位行列の定数倍）の足し算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} * val
	//! \f]
	//! 
	//! @param[in] val … 行列を定数倍する数
	//! 
	//! @return 自分自身
	//! 
	const matrix& operator *=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		for( size_type i = 0 ; i < m.size( ) ; i++ ) m[i] *= val;
		return( m );
	}


	/// @brief 行列への定数（単位行列の定数倍）の足し算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} / val
	//! \f]
	//! 
	//! @param[in] val … 行列を割る定数
	//! 
	//! @return 自分自身
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


	/// @brief 行列と数式の足し算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} + Expression
	//! \f]
	//! 
	//! @param[in] m2 … 複数の行列演算を含む式
	//! 
	//! @return 自分自身
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


	/// @brief 行列と数式の引き算
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} - Expression
	//! \f]
	//! 
	//! @param[in] m2 … 複数の行列演算を含む式
	//! 
	//! @return 自分自身
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


	/// @brief 行列と数式の積
	//! 
	//! \f[
	//! 	{\bf A} = {\bf A} * Expression
	//! \f]
	//! 
	//! @param[in] m2 … 複数の行列演算を含む式
	//! 
	//! @return 自分自身
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
	/// @brief 要素の型が異なる行列を代入する
	//! 
	//! コピー元である行列 o と全く同じ行列を作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! コピー元とコピー先でデータ型（array< data > の data）が異なる場合の代入を行う．
	//! 
	//! @param[in] o  … コピー元の行列
	//! 
	//! @return 自分自身
	//! 
	template < class TT, class AAlocator >
	const matrix& operator =( const matrix< TT, AAlocator > &o )
	{
		base::operator =( o );
		size1_ = o.size1( );
		size2_ = o.size2( );

		return( *this );
	}

	/// @brief 要素の型が同じ行列を代入する
	//! 
	//! コピー元である行列 o と全く同じ行列を作成する．
	//! コピー先（ここでは自分自身）の要素数が o と異なる場合は，自動的にサイズを調整する．
	//! 
	//! @param[in] o  … コピー元の行列
	//! 
	//! @return 自分自身
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
	/// @brief 複数の行列演算を含む式の結果を代入する
	//! 
	//! 複数の行列演算の結果の行列サイズに自動的にリサイズが行われる
	//! 
	//! @param[in] expression … 複数の行列演算を含む式
	//! 
	//! @return 自分自身
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
	/// @brief index で示される位置の要素のポインタを返す
	//! 
	//! @param[in] r … 行位置
	//! @param[in] c … 列位置
	//! 
	//! @return 指定された要素を示すポインタ
	//! 
	pointer paccess( size_type r, size_type c )
	{
		return( base::data_ + r + c * size1_ );
	}

	/// @brief index で示される位置の要素の const ポインタを返す
	//! 
	//! @param[in] r … 行位置
	//! @param[in] c … 列位置
	//! 
	//! @return 指定された要素を示す const ポインタ
	//! 
	const_pointer paccess( size_type r, size_type c ) const
	{
		return( base::data_ + r + c * size1_ );
	}


public:
	/// @brief r 行 c 列 で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された r 行 c 列 が有効な範囲内にあるかをチェックする
	//! 
	//! @param[in] r … 行位置
	//! @param[in] c … 列位置
	//! 
	//! @return 指定された要素を示す参照
	//! 
	reference at( size_type r, size_type c )
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief r 行 c 列 で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された r 行 c 列 が有効な範囲内にあるかをチェックする
	//! 
	//! @param[in] r … 行位置
	//! @param[in] c … 列位置
	//! 
	//! @return 指定された要素を示す const 参照
	//! 
	const_reference at( size_type r, size_type c ) const
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief r 行 c 列 で示される位置の要素の参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された r 行 c 列 が有効な範囲内にあるかをチェックする
	//! 
	//! @param[in] r … 行位置
	//! @param[in] c … 列位置
	//! 
	//! @return 指定された要素を示す参照
	//! 
	reference operator ()( size_type r, size_type c )
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief r 行 c 列 で示される位置の要素の const 参照を返す
	//!
	//! DEBUG マクロを有効にした（NDEBUGマクロを定義しない）場合は，指定された r 行 c 列 が有効な範囲内にあるかをチェックする
	//! 
	//! @param[in] r … 行位置
	//! @param[in] c … 列位置
	//! 
	//! @return 指定された要素を示す const 参照
	//! 
	const_reference operator ()( size_type r, size_type c ) const
	{
		_CHECK_ACCESS_VIOLATION2_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}


public:
	/// @brief ディフォルトコンストラクタ．0 行 0 列 の行列を作成する
	matrix( ) : base( ), size1_( 0 ), size2_( 0 ) {}

	/// @brief アロケータ a のコピーを利用し，0 行 0 列 の行列を作成する
	explicit matrix( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ) {}


	/// @brief 要素数 rnum 行 cnum 列 の行列を作成し，デフォルト値で要素を初期化する
	matrix( size_type rnum, size_type cnum ) : base( rnum * cnum ), size1_( rnum ), size2_( cnum ) {}

	/// @brief 要素数 rnum 行 cnum 列 でアロケータ a のコピーを利用した行列を作成し，デフォルト値で要素を初期化する
	matrix( size_type rnum, size_type cnum, const Allocator &a ) : base( rnum * cnum, a ), size1_( rnum ), size2_( cnum ) {}


	/// @brief 要素数 rnum 行 cnum 列 の行列を作成し，全要素を値 val で要素を初期化する
	matrix( size_type rnum, size_type cnum, const T &val ) : base( rnum * cnum, val ), size1_( rnum ), size2_( cnum ) {}

	/// @brief 要素数 rnum 行 cnum 列 でアロケータ a のコピーを利用した行列を作成し，全要素を値 val で要素を初期化する
	matrix( size_type rnum, size_type cnum, const T &val, const Allocator &a ) : base( rnum * cnum, val, a ), size1_( rnum ), size2_( cnum ) {}


	/// @brief 他の行列で要素の型が異なるものから同じサイズの行列を作成する
	//!
	//! @attention 異なる要素型間でデータの変換が可能でなくてはならない
	//!
	template < class TT, class AAlocator >
	matrix( const matrix< TT, AAlocator > &o ) : base( o ), size1_( o.size1( ) ), size2_( o.size2( ) ){ }


	/// @brief 他の行列で同じ要素型のものを用いて初期化する
	matrix( const matrix< T, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ){ }


#if _USE_EXPRESSION_TEMPLATE_ != 0
	/// @brief 複数の行列演算の結果を用いて行列を初期化する
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



/// @brief 指定されたストリームに，行列内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     m   … 出力する行列
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 4, 7
//! 2, 5, 8
//! 3, 6, 9
//! @endcode
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


/// @brief 指定されたストリームに，複数の行列演算の結果を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     m   … 出力する複数の行列演算式
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 4, 7
//! 2, 5, 8
//! 3, 6, 9
//! @endcode
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
//! @param[in] m1 … 左辺値
//! @param[in] m2 … 右辺値
//! 
//! @return 足し算結果
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) += m2 );
}


/// @brief 引き算
//! 
//! @param[in] m1 … 左辺値
//! @param[in] m2 … 右辺値
//! 
//! @return 引き算結果
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) -= m2 );
}


/// @brief 掛け算
//! 
//! @param[in] m1 … 左辺値
//! @param[in] m2 … 右辺値
//! 
//! @return 掛け算結果
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


/// @brief 単位行列の定数倍との足し算
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] m   … 行列
//! @param[in] val … 定数
//! 
//! @return 左辺値の行列に単位行列の定数倍を足し算した結果
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
//! @param[in] val … 定数
//! @param[in] m   … 行列
//! 
//! @return 右辺値の行列に単位行列の定数倍を足し算した結果
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
//! @param[in] m   … 行列
//! @param[in] val … 定数
//! 
//! @return 左辺値の行列から，単位行列を定数倍したものを引き算した結果
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
//! @param[in] val … 定数
//! @param[in] m   … 行列
//! 
//! @return 単位行列を定数倍したものから右辺値の行列を引き算した結果
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
//! @param[in] m   … 行列
//! @param[in] val … 定数
//! 
//! @return 左辺値の行列を定数倍した結果
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
//! @param[in] val … 定数
//! @param[in] m   … 行列
//! 
//! @return 右辺値の行列を定数倍した結果
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
//! @param[in] m   … 行列
//! @param[in] val … 定数
//! 
//! @return 左辺値の行列を定数で割り算した結果
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
