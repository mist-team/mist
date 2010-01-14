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

/// @file mist/interval_tree.h
//!
//! @brief Interval-tree のためのライブラリ
//!

#ifndef __INCLUDE_INTERVAL_TREE_H__
#define __INCLUDE_INTERVAL_TREE_H__

#include <iostream>
#include <vector>
#include <algorithm>

// mist名前空間の始まり
_MIST_BEGIN

//! @addtogroup interval_tree_group Interval-tree
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/interval_tree.h>
//! @endcode
//!
//!  @{

template< typename M, typename T >	class tagged_section;
template< typename M, typename T >	class tagged_section_min_less;
template< typename M, typename T >	class tagged_section_max_greater;

/// @brief 区間の集合から任意の値が含まれる区間のみを高速に探索するためのクラス
//!
//! 区間の集合から2分木を構築し、通常O(n)かかる探索時間をO(log n)に削減する．
//! 線分や面分の重なり判定などに有効．
//!
//! @attention mist::tagged_section< M, T >型のstd::vetorを入力とする．
//! @attention mist::tagged_section< M, T >::tag_type型のstd::vetorを探索結果の出力とする．
//! 
//! @param S  … 区間のデータ型（ mist::tagged_section< M, T >型 ）
//! @param V  … 区間の探索の際に入力する任意の値のデータ型（ double や float など，mist::rgb< M, T >::min_max_typeと同じか、あるいはより精度の高い型 ）
//!
//! @code 使用例
//! // 区間の集合の用意
//! typedef mist::tagged_section< int, size_t > section_type;
//! std::vector< section_type > secs; // int型の最大値，最小値にはさまれた区間の集合，各区間にはsize_t型のタグが付けられる．
//! secs.push_back( section_type( -1, 5, 0 ) );　// タグ0が付いた区間[ -1, 5 )を集合に追加．
//! /* 
//!		区間の集合へ追加処理
//! */
//! 
//! // Interval-treeの構築
//! typedef mist::interval_tree< section_type, double > i_tree_type;
//! mist::i_tree_type i_tree( secs );
//!
//! // 任意の値が含まれる区間のタグの集合を高速に探索
//! std::vector< section_type::tag_type > tags;
//! i_tree.find( 0.0, tags ); // 0.0 が含まれる区間のタグの集合を得る
//!
//! // 探索結果の表示
//! for( size_t i = 0 ; i < tags.size( ) ; i ++ )
//! {
//!		std::cout << secs[ tags[ i ] ] << std::endl;
//! }
//! @endcode
//!
template< typename S, typename V >
class interval_tree
{
public:
	typedef S							section_type;	///< @brief 区間を表す型
	typedef typename S::min_max_type	min_max_type;	///< @brief 区間の両端の値を表す型
	typedef typename S::tag_type		tag_type;		///< @brief 区間のタグ情報をを表す型
	typedef V							value_type;		///< @brief 区間の探索の際に入力される値を表す型

	class node;

	/// @brief 入力された値が含まれる区間の探索
	//!
	//! @param[in]	val		… 入力（この値を間に含む区間を探索）
	//! @param[out] tags	… 探索された区間のタグ情報の集合
	//!
	void find( const value_type &val, std::vector< tag_type > &tags )
	{
		tags.resize( 0 );
		_find( val, tags, root_ );
	}

	/// @brief Interval-treeの構築
	//!
	//! @param[in] secs   … 区間の集合
	//! @return 木が正しく構築された場合に真を返す
	//!
	bool construct( const std::vector< section_type > &secs )
	{
		if( root_ != NULL ) 
		{ 
			destruct( ); 
		}
		if( secs.size( ) > 0 ) 
		{
			size_t tree_size = 0;
			root_= new node( secs, tree_size );
			return ( tree_size == secs.size( ) );
		}
		destruct( );
		return false;
	}

	/// @brief Interval-treeの破棄
	void destruct( )
	{
		if( root_ != NULL ) 
		{ 
			delete root_; 
			root_ = NULL; 
		}
	}

	/// @brief デフォルトコンストラクタ
	interval_tree( ) : root_( NULL )
	{ 
	}

	/// @brief コンストラクタ（Interval-treeの構築）
	//!
	//! @param[in] secs … 区間集合
	//!
	interval_tree( const std::vector< section_type > &secs ) : root_( NULL ) 
	{ 
		construct( secs ); 
	}

	/// @brief コピーコンストラクタ
	//!
	//! @param[in] it … interval_treeオブジェクト
	//!
	interval_tree( const interval_tree< section_type, value_type > &it ) : root_( NULL )
	{
		*this = it;
	}

	/// @brief デストラクタ
	~interval_tree( ) 
	{ 
		destruct( ); 
	}

	/// @brief 代入演算子
	//!
	//! @param[in] it … interval_treeオブジェクト
	//!
	interval_tree &operator =( const interval_tree< section_type, value_type > &it )
	{
		if( &it == this )
		{
			return *this;
		}
		else
		{
			destruct( );
			if( it.root_ != NULL )
			{
				root_ = new node( *it.root_ );
			}
			return *this;
		}
	}

private:
	void _find( const value_type &val, std::vector< tag_type > &tags, node *p )
	{
		if( p != NULL )
		{
			if( val < p->val( ) )
			{	
				for( size_t i = 0 ; i < p->min_inc_secs_size( ) ; i ++ )
				{
					if( static_cast< value_type >( p->min_inc_secs( i ).minimum( ) ) > val )
					{
						break;
					}
					tags.push_back( p->min_inc_secs( i ).tag( ) );
				}
				_find( val, tags, p->left( ) );
			}
			else if( val > p->val( ) )
			{
				for( size_t i = 0 ; i < p->max_dec_secs_size( ) ; i ++ )
				{
					if( static_cast< value_type >( p->max_dec_secs( i ).maximum( ) ) <= val )
					{
						break;
					}
					tags.push_back( p->max_dec_secs( i ).tag( ) );
				}
				_find( val, tags, p->right( ) );
			}
			else
			{
				for( size_t i = 0 ; i < p->min_inc_secs_size( ) ; i ++ )
				{
					tags.push_back( p->min_inc_secs( i ).tag( ) );
				}
			}
		} 
	}

	node *root_;
};


/// @brief Interval-treeのためのタグ情報付き区間を表すクラス
//!
//! 区間の両端を表す2つの値（最大値，最小値）と区間を識別するためのタグ情報を持つ．
//!
//! @attention [ min, max )で表される区間を対象とする．
//! 
//! @param M … 区間の最大値，最小値の型
//! @param T … 区間に付けられるタグ情報のデータ型
//!
template< typename M, typename T >
class tagged_section
{
public:
	typedef M min_max_type;		///< @brief 最大値，最小値を表す型
	typedef T tag_type;			///< @brief タグ情報を表す型

	/// @brief 区間の最小値を返す
	//!
	//! @return 区間の最小値
	//!
	const min_max_type	&minimum( ) const { return min_; }
	
	/// @brief 区間の最大値を返す
	//!
	//! @return 区間の最大値
	//!
	const min_max_type	&maximum( ) const { return max_; }
	
	/// @brief 区間のタグ情報を返す
	//!
	//! @return 区間のタグ情報
	//!
	const tag_type		&tag( ) const { return tag_; }
	
	/// @brief 区間の最小値を返す（入出力可能）
	//!
	//! @return 区間の最小値
	//!
	min_max_type	&minimum( ) { return min_; }
	
	/// @brief 区間の最大値を返す（入出力可能）
	//!
	//! @return 区間の最大値
	//!
	min_max_type	&maximum( ) { return max_; }
	
	/// @brief 区間のタグ情報を返す（入出力可能）
	//!
	//! @return 区間のタグ情報
	//!
	tag_type		&tag( ) { return tag_; }

	/// @brief デフォルトコンストラクタ
	tagged_section( ) : min_( min_max_type( ) ), max_( min_max_type( ) ), tag_( tag_type( ) ) { }
	
	/// @brief コンストラクタ
	//!
	//! @param[in] min … 最小値
	//! @param[in] max … 最大値
	//! @param[in] tag … タグ情報
	//!
	tagged_section( const min_max_type &min, const min_max_type &max, const tag_type &tag ) : min_( min ), max_( max ), tag_( tag ) { }
	
private:
	min_max_type	min_;
	min_max_type	max_;
	tag_type		tag_;
};

/// @brief タグ付き区間オブジェクトの出力演算子
//!
//! @param[in,out] out   … 出力ストリーム
//! @param[in]     in    … 区間
//! @return 出力ストリーム
//!
template< typename M, typename T >
inline std::ostream &operator <<( std::ostream &out, const tagged_section< M, T > &in )
{
	return out << in.tag( ) << " [" << in.minimum( ) << ", " << in.maximum( ) << ")";
}

/// @brief 区間集合のソートに用いる関数オブジェクト
template< typename M, typename T > 
class tagged_section_min_less	
{ 
public: 
	bool operator ( )( const tagged_section< M, T > &s0, const tagged_section< M, T > &s1 ) const { return s0.minimum( ) < s1.minimum( ); } 
};

/// @brief 区間集合のソートに用いる関数オブジェクト
template< typename M, typename T >
class tagged_section_max_greater	
{ 
public: 
	bool operator ( )( const tagged_section< M, T > &s0, const tagged_section< M, T > &s1 ) const { return s0.maximum( ) > s1.maximum( ); } 
};


/// @brief Interval-treeのノードを表すクラス（interval_treeクラスの内部でのみ使用）
//!
//! @param S  … 区間のデータ型（ mist::tagged_section< M, T >型 ）
//! @param V  … 区間に含まれる任意の値のデータ型（ double や float など，mist::rgb< M, T >::min_max_typeと同じか、あるいはより精度の高い型 ）
//!
template< typename S, typename V >
class interval_tree< S, V >::node
{
	friend class interval_tree< S, V >;

private:
	const value_type	&val( )							const { return val_; } 
	const section_type	&min_inc_secs( const size_t i )	const { return min_inc_secs_[ i ]; }
	const section_type	&max_dec_secs( const size_t i )	const { return max_dec_secs_[ i ]; }
	size_t		min_inc_secs_size( )					const { return min_inc_secs_.size( ); }
	size_t		max_dec_secs_size( )					const { return max_dec_secs_.size( ); }
	
	node *left( )	{ return left_; }
	node *right( )	{ return right_; }

	node( const std::vector< section_type > &secs, size_t &tree_size ) : left_( NULL ), right_( NULL )
	{ 
		val_ = middle( secs ); 
		std::vector< section_type > l_secs, r_secs;
		for( size_t i = 0 ; i < secs.size( ) ; i ++ )
		{
			if( static_cast< value_type >( secs[ i ].maximum( ) ) <= val_ )
			{
				l_secs.push_back( secs[ i ] );
				continue;
			}
			if( static_cast< value_type >( secs[ i ] .minimum( ) ) > val_ )
			{
				r_secs.push_back( secs[ i ] );
				continue;
			}
			min_inc_secs_.push_back( secs[ i ] );
		}
		tree_size += min_inc_secs_.size( );
		std::sort( min_inc_secs_.begin( ), min_inc_secs_.end( ), tagged_section_min_less< min_max_type, tag_type >( ) );
		max_dec_secs_ = min_inc_secs_; 
		std::sort( max_dec_secs_.begin( ), max_dec_secs_.end( ), tagged_section_max_greater< min_max_type, tag_type >( ) );
		if( l_secs.size( ) > 0 )
		{
			left_ = new node( l_secs, tree_size );
		}
		if( r_secs.size( ) > 0 )
		{
			right_ = new node( r_secs, tree_size );
		}
	}

	node( const node &nd ) : left_( NULL ), right_( NULL )
	{
		*this = nd;	
	}

	~node( )
	{ 
		if( left_ != NULL )
		{
			delete left_;
			left_ = NULL;
		}
		if( right_ != NULL )
		{
			delete right_;
			right_ = NULL;
		}
	}

	node &operator =( const node &nd )
	{
		if( &nd == this )
		{
			return *this;
		}
		else
		{
			delete left_;
			delete right_;
			val_			= nd.val_;
			min_inc_secs_	= nd.min_inc_secs_;
			max_dec_secs_	= nd.max_dec_secs_;
			if( nd.left_ != NULL )
			{
				left_	= new node( *nd.left_ );
			}
			if( nd.right_ != NULL )
			{
				right_	= new node( *nd.right_ );
			}
			return *this;
		}
	}

	value_type middle( const std::vector< section_type > &secs ) const
	{
		typename section_type::min_max_type minimum = secs[ 0 ].minimum( );
		typename section_type::min_max_type maximum = secs[ 0 ].maximum( );
		for( size_t i = 1 ; i < secs.size( ) ; i ++ )
		{
			minimum = ( secs[ i ].minimum( ) < minimum ) ? secs[ i ].minimum( ) : minimum;
			maximum = ( secs[ i ].maximum( ) > maximum ) ? secs[ i ].maximum( ) : maximum;
		}

		return( static_cast< value_type >( ( minimum + maximum ) / 2.0 ) );
	}

	value_type					val_;
	std::vector< section_type >	min_inc_secs_;
	std::vector< section_type >	max_dec_secs_;
	node						*left_;
	node						*right_;
};


/// @}
//  Interval-tree グループの終わり

// mist名前空間の終わり
_MIST_END

#endif // #ifndef __INCLUDE_INTERVAL_TREE_H__
