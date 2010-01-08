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

/// @file mist/config/set.h
//!
//! @brief 集合演算を扱うためのライブラリ
//!

#ifndef __INCLUDE_MIST_SET_H__
#define __INCLUDE_MIST_SET_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <set>
#include <algorithm>

// mist名前空間の始まり
_MIST_BEGIN



//! @addtogroup set_group 集合演算
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/config/set.h>
//! @endcode
//!
//!  @{


/// @brief 集合を扱うための基本クラス
//! 
//! @param SetType … 基底クラス（std::set か std::multiset）
//! 
template< class SetType >
class set_base : public SetType
{
protected:
	typedef SetType base;			///< @brief 基底クラス（std::set か std::multiset）

public:
	typedef typename base::key_type key_type;					///< @brief 説明を書く
	typedef typename base::key_compare key_compare;				///< @brief 説明を書く
	typedef typename base::value_compare value_compare;			///< @brief 説明を書く
	typedef typename base::allocator_type allocator_type;		///< @brief STLコンテナが利用するアロケータ型
	typedef typename base::size_type size_type;					///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

#if __MIST_MSVC__ != 6
	typedef typename base::pointer pointer;						///< @brief STLのコンテナ内に格納するデータ型のポインター型．std::set< data > の場合，data * となる
	typedef typename base::const_pointer const_pointer;			///< @brief STLのコンテナ内に格納するデータ型の const ポインター型．std::set< data > の場合，const data * となる
#endif

	typedef typename base::reference reference;								///< @brief STLのコンテナ内に格納するデータ型の参照．std::set< data > の場合，data & となる
	typedef typename base::const_reference const_reference;					///< @brief STLのコンテナ内に格納するデータ型の const 参照．std::set< data > の場合，const data & となる
	typedef typename base::iterator iterator;								///< @brief STLのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename base::const_iterator const_iterator;					///< @brief STLのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief STLのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief STLのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef typename base::value_type value_type;							///< @brief STLのコンテナ内に格納するデータ型．std::set< data > の data と同じ


public:
	/// @brief デフォルトコンストラクタ（空集合を作成する）
	set_base( ) : base( ){ }

	/// @brief 集合の要素の比較演算子として pred を用いる
	explicit set_base( const key_compare &pred ) : base( pred ){ }

	/// @brief 1要素 key を持つ集合を作成する
	explicit set_base( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief 集合の要素の比較演算子として pred を用い，alloc のコピーを利用する
	set_base( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief first と last の間の要素を用いて集合を作成する
	template< class Iterator >
	set_base( Iterator first, Iterator last ) : base( first, last ){ }


	/// @brief first と last の間の要素を用いて集合を作成し，要素の比較演算子として pred を用いる
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }


	/// @brief first と last の間の要素を用いて集合を作成し，要素の比較演算子として pred を用い，alloc のコピーを利用する
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }

public:
	/// @brief 集合和
	//! 
	//! \f[ a = a \bigcup b \f]
	//! 
	set_base &operator +=( const set_base &s )
	{
#if __MIST_MSVC__ != 6
		base::insert( s.begin( ), s.end( ) );
#else
		// MSVC6 付属のSTLのSETではイテレータの実装が異なる
		const_iterator site = s.begin( );
		const_iterator eite = s.end( );
		for( ; site != eite ; ++site )
		{
			base::insert( *site );
		}
#endif
		return( *this );
	}

	/// @brief 集合に1要素追加する
	set_base &operator +=( const key_type &s ){ base::insert( s ); return( *this ); }


	/// @brief 差集合を計算する
	//! 
	//! \f[ a = a - \left( a \bigcap b \right) \f]
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

			// 自分自身の内容を空っぽにする
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
				// MSVC6 付属のSTLのSETではイテレータの実装が異なる
				for( ; site1 != eite1 ; ++site1 )
				{
					base::insert( *site1 );
				}
#endif
			}
		}
		return( *this );
	}

	/// @brief 集合から1要素取り除く
	set_base &operator -=( const key_type &s ){ base::erase( s ); return( *this ); }


	/// @brief 積集合を計算する
	//! 
	//! \f[ a = a \bigcap b \f]
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

			// 自分自身の内容を空っぽにする
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


	/// @brief 集合と1要素の集合間の積を取る
	set_base &operator *=( const key_type &s )
	{
		set_base a = *this;
		const_iterator site = a.begin( );
		const_iterator eite = a.end( );

		// 自分自身の内容を空っぽにする
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


	/// @brief 集合に集合を追加する
	set_base &operator <<=( const set_base &s ){ return( operator +=( s ) ); }

	/// @brief 集合に1要素を追加する
	set_base &operator <<=( const key_type &s ){ return( operator +=( s ) ); }


	/// @brief 集合に1要素追加した集合を出力する
	set_base &operator <<( const key_type &s ) const { return( set_base( *this )+=( s ) ); }


	/// @brief 全ての要素が等しいかどうかを判定する（\f$ a = b \f$）
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


	/// @brief 全ての要素が等しいかどうかを判定する（1要素版）
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


	/// @brief 全ての要素が等しくないかどうかを判定する（\f$ a \neq b \f$）
	bool operator !=( const set_base &s ) const { return( !( *this == s ) ); }

	/// @brief 全ての要素が等しくないかどうかを判定する（1要素版）
	bool operator !=( const key_type &s ) const { return( !( *this == s ) ); }

	/// @brief 集合の包含関係を調べる（\f$ a \subset b \f$）
	bool operator < ( const set_base &s ) const
	{
		if( base::size( ) >= s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief 集合の包含関係を調べる（\f$ a \subseteq b \f$）
	bool operator <=( const set_base &s ) const
	{
		if( base::size( ) > s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief 集合の包含関係を調べる（\f$ a \supset b \f$）
	bool operator > ( const set_base &s ) const { return( s < *this ); }

	/// @brief 集合の包含関係を調べる（\f$ a \supseteq b \f$）
	bool operator >=( const set_base &s ) const { return( s <= *this ); }
};


/// @brief 和集合を計算する
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief 差集合を計算する
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief 積集合を計算する
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }


//inline const set_base operator /( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) /= s2 ); }
//inline const set_base operator %( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) %= s2 ); }
//inline const set_base operator |( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) |= s2 ); }
//inline const set_base operator &( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) &= s2 ); }
//inline const set_base operator ^( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) ^= s2 ); }



/// @brief 集合と1要素の積を計算する
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief 1要素と集合の積を計算する
template< class SetType > inline const set_base< SetType > operator *( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief 集合と1要素の和を計算する
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief 1要素と集合の和を計算する
template< class SetType > inline const set_base< SetType > operator +( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s2 ) += s1 ); }

/// @brief 集合と1要素の差を計算する
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief 1要素と集合の差を計算する
template< class SetType > inline const set_base< SetType > operator -( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }



/// @brief 全ての要素が等しいかどうかを判定する（\f$ a = b \f$）
template< class SetType > bool operator ==( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) == s1 ); }

/// @brief 全ての要素が等しくないかどうかを判定する（\f$ a \neq b \f$）
template< class SetType > bool operator !=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) != s1 ); }

/// @brief 集合の包含関係を調べる（\f$ a \subset b \f$）
template< class SetType > bool operator < ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <  set_base< SetType >( s2 ) ); }

/// @brief 集合の包含関係を調べる（\f$ a \subseteq b \f$）
template< class SetType > bool operator <=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <= set_base< SetType >( s2 ) ); }

/// @brief 集合の包含関係を調べる（\f$ a \supset b \f$）
template< class SetType > bool operator > ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >  set_base< SetType >( s2 ) ); }

/// @brief 集合の包含関係を調べる（\f$ a \supseteq b \f$）
template< class SetType > bool operator >=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >= set_base< SetType >( s2 ) ); }



/// @brief 全ての要素が等しいかどうかを判定する（\f$ a = b \f$）
template< class SetType > bool operator ==( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) == s2 ); }

/// @brief 全ての要素が等しくないかどうかを判定する（\f$ a \neq b \f$）
template< class SetType > bool operator !=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) != s2 ); }

/// @brief 集合の包含関係を調べる（\f$ a \subset b \f$）
template< class SetType > bool operator < ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <  s2 ); }

/// @brief 集合の包含関係を調べる（\f$ a \subseteq b \f$）
template< class SetType > bool operator <=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <= s2 ); }

/// @brief 集合の包含関係を調べる（\f$ a \supset b \f$）
template< class SetType > bool operator > ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >  s2 ); }

/// @brief 集合の包含関係を調べる（\f$ a \supseteq b \f$）
template< class SetType > bool operator >=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >= s2 ); }


/// @brief 指定されたストリームに，コンテナ内の要素を整形して出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     s   … 集合
//! 
//! @return 入力されたストリーム
//! 
//! @code 出力例
//! 1, 2, ... , n
//! @endcode
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



/// @brief 集合演算を行うことができる，STLの set の拡張版
//! 
//! 要素の重複を許さない集合を扱う
//! 
//! @param Key       … 集合の各要素を表す型
//! @param Comp      … 要素間の大小関係を表す比較関数（省略すると < が適用される）
//! @param Allocator … アロケータ（省略するとSTLのデフォルトアロケータが適用される）
//! 
template< class Key, class Comp = std::less< Key >, class Allocator = std::allocator< Key > >
class set : public set_base< std::set< Key, Comp, Allocator > >
{
protected:
	typedef set_base< std::set< Key, Comp, Allocator > > base;		///< @brief 説明を書く

public:
	typedef typename base::key_type key_type;					///< @brief 説明を書く
	typedef typename base::key_compare key_compare;				///< @brief 説明を書く
	typedef typename base::value_compare value_compare;			///< @brief 説明を書く
	typedef typename base::allocator_type allocator_type;		///< @brief STLコンテナが利用するアロケータ型
	typedef typename base::size_type size_type;					///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

#if __MIST_MSVC__ != 6
	typedef typename base::pointer pointer;						///< @brief STLのコンテナ内に格納するデータ型のポインター型．std::set< data > の場合，data * となる
	typedef typename base::const_pointer const_pointer;			///< @brief STLのコンテナ内に格納するデータ型の const ポインター型．std::set< data > の場合，const data * となる
#endif

	typedef typename base::reference reference;								///< @brief STLのコンテナ内に格納するデータ型の参照．std::set< data > の場合，data & となる
	typedef typename base::const_reference const_reference;					///< @brief STLのコンテナ内に格納するデータ型の const 参照．std::set< data > の場合，const data & となる
	typedef typename base::iterator iterator;								///< @brief STLのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename base::const_iterator const_iterator;					///< @brief STLのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief STLのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief STLのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef typename base::value_type value_type;							///< @brief STLのコンテナ内に格納するデータ型．std::set< data > の data と同じ

public:
	/// @brief デフォルトコンストラクタ（空集合を作成する）
	set( ) : base( ){ }

	/// @brief 集合の要素の比較演算子として pred を用いる
	explicit set( const key_compare &pred ) : base( pred ){ }

	/// @brief 1要素 key を持つ集合を作成する
	explicit set( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief 集合の要素の比較演算子として pred を用い，alloc のコピーを利用する
	set( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief first と last の間の要素を用いて集合を作成する
	template< class Iterator >
	set( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief first と last の間の要素を用いて集合を作成し，要素の比較演算子として pred を用いる
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief first と last の間の要素を用いて集合を作成し，要素の比較演算子として pred を用い，alloc のコピーを利用する
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};



/// @brief 集合演算を行うことができる，STLの set の拡張版
//! 
//! 要素の重複を許す集合を扱う
//! 
//! @param Key       … 集合の各要素を表す型
//! @param Comp      … 要素間の大小関係を表す比較関数（省略すると < が適用される）
//! @param Allocator … アロケータ（省略するとSTLのデフォルトアロケータが適用される）
//! 
template< class Key, class Comp = std::less< Key >, class Allocator = std::allocator< Key > >
class multiset : public set_base< std::multiset< Key, Comp, Allocator > >
{
protected:
	typedef set_base< std::multiset< Key, Comp, Allocator > > base;

public:
	typedef typename base::key_type key_type;					///< @brief 説明を書く
	typedef typename base::key_compare key_compare;				///< @brief 説明を書く
	typedef typename base::value_compare value_compare;			///< @brief 説明を書く
	typedef typename base::allocator_type allocator_type;		///< @brief STLコンテナが利用するアロケータ型
	typedef typename base::size_type size_type;					///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef typename base::difference_type difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

#if __MIST_MSVC__ != 6
	typedef typename base::pointer pointer;						///< @brief STLのコンテナ内に格納するデータ型のポインター型．std::set< data > の場合，data * となる
	typedef typename base::const_pointer const_pointer;			///< @brief STLのコンテナ内に格納するデータ型の const ポインター型．std::set< data > の場合，const data * となる
#endif

	typedef typename base::reference reference;								///< @brief STLのコンテナ内に格納するデータ型の参照．std::set< data > の場合，data & となる
	typedef typename base::const_reference const_reference;					///< @brief STLのコンテナ内に格納するデータ型の const 参照．std::set< data > の場合，const data & となる
	typedef typename base::iterator iterator;								///< @brief STLのコンテナ内を操作する，STLで言う順方向のランダムアクセスイテレータ
	typedef typename base::const_iterator const_iterator;					///< @brief STLのコンテナ内を操作する，順方向のランダムアクセスイテレータのコンスト版
	typedef typename base::reverse_iterator reverse_iterator;				///< @brief STLのコンテナ内を操作する，逆方向のランダムアクセスイテレータ
	typedef typename base::const_reverse_iterator const_reverse_iterator;	///< @brief STLのコンテナ内を操作する，逆方向のランダムアクセスイテレータのコンスト版
	typedef typename base::value_type value_type;							///< @brief STLのコンテナ内に格納するデータ型．std::set< data > の data と同じ

public:
	/// @brief デフォルトコンストラクタ（空集合を作成する）
	multiset( ) : base( ){ }

	/// @brief 集合の要素の比較演算子として pred を用いる
	explicit multiset( const key_compare &pred ) : base( pred ){ }

	/// @brief 1要素 key を持つ集合を作成する
	explicit multiset( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief 集合の要素の比較演算子として pred を用い，alloc のコピーを利用する
	multiset( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief first と last の間の要素を用いて集合を作成する
	template< class Iterator >
	multiset( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief first と last の間の要素を用いて集合を作成し，要素の比較演算子として pred を用いる
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief first と last の間の要素を用いて集合を作成し，要素の比較演算子として pred を用い，alloc のコピーを利用する
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};



/// @}
//  集合演算グループの終わり



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_SET_H__
