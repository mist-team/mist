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


// MISTで利用する基底のデータ型


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template< class SetType >
class set_base : public SetType
{
protected:
	typedef SetType base;			///< @brief 説明を書く

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
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] in  … 引数の説明
	//! @param[in] out … 引数の説明
	//! 
	set_base( ) : base( ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pred … 引数の説明
	//! 
	explicit set_base( const key_compare &pred ) : base( pred ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] key … 引数の説明
	//! 
	explicit set_base( const key_type &key ) : base( ){ base::insert( key ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pred  … 引数の説明
	//! @param[in] alloc … 引数の説明
	//! 
	set_base( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! 
	template< class Iterator >
	set_base( Iterator first, Iterator last ) : base( first, last ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! @param[in] pred  … 引数の説明
	//! 
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! @param[in] pred  … 引数の説明
	//! @param[in] alloc … 引数の説明
	//! 
	template< class Iterator >
	set_base( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }

public:
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
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

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	set_base &operator +=( const key_type &s ){ base::insert( s ); return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
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

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	set_base &operator -=( const key_type &s ){ base::erase( s ); return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	set_base &operator <<=( const set_base &s ){ return( operator +=( s ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	set_base &operator <<=( const key_type &s ){ return( operator +=( s ) ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	set_base &operator <<( const key_type &s ){ return( operator +=( s ) ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
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


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator !=( const set_base &s ) const { return( !( *this == s ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator !=( const key_type &s ) const { return( !( *this == s ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator < ( const set_base &s ) const
	{
		if( base::size( ) >= s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator <=( const set_base &s ) const
	{
		if( base::size( ) > s.size( ) )
		{
			return( false );
		}

		return( std::includes( s.begin( ), s.end( ), base::begin( ), base::end( ), key_compare( ) ) );
	}

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator > ( const set_base &s ) const { return( s < *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 引数の説明
	//! @retval false … 引数の説明
	//! 
	bool operator >=( const set_base &s ) const { return( s <= *this ); }
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }


//inline const set_base operator /( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) /= s2 ); }
//inline const set_base operator %( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) %= s2 ); }
//inline const set_base operator |( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) |= s2 ); }
//inline const set_base operator &( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) &= s2 ); }
//inline const set_base operator ^( const set_base &s1, const set_base &s2 ){ return( set_base( s1 ) ^= s2 ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator *( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator *( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) *= s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator +( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) += s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator +( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s2 ) += s1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator -( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template< class SetType > inline const set_base< SetType > operator -( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) -= s2 ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator ==( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) == s1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator !=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( set_base< SetType >( s2 ) != s1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator < ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <  set_base< SetType >( s2 ) ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator <=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 <= set_base< SetType >( s2 ) ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator > ( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >  set_base< SetType >( s2 ) ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator >=( const set_base< SetType > &s1, const typename set_base< SetType >::key_type &s2 ){ return( s1 >= set_base< SetType >( s2 ) ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator ==( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) == s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator !=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) != s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator < ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <  s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator <=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) <= s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator > ( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >  s2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] s1 … 引数の説明
//! @param[in] s2 … 引数の説明
//! 
//! @retval true  … 戻り値の説明
//! @retval false … 戻り値の説明
//! 
template< class SetType > bool operator >=( const typename set_base< SetType >::key_type &s1, const set_base< SetType > &s2 ){ return( set_base< SetType >( s1 ) >= s2 ); }


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in,out] out … 引数の説明
//! @param[in]     s   … 引数の説明
//! 
//! @return 戻り値の説明
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



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param Key       … 引数の説明
//! @param Comp      … 引数の説明
//! @param Allocator … 引数の説明
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
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	set( ) : base( ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pred … 引数の説明
	//! 
	explicit set( const key_compare &pred ) : base( pred ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] key … 引数の説明
	//! 
	explicit set( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pred  … 引数の説明
	//! @param[in] alloc … 引数の説明
	//! 
	set( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! 
	template< class Iterator >
	set( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! @param[in] pred  … 引数の説明
	//! 
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! @param[in] pred  … 引数の説明
	//! @param[in] alloc … 引数の説明
	//! 
	template< class Iterator >
	set( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param Key       … 引数の説明
//! @param Comp      … 引数の説明
//! @param Allocator … 引数の説明
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
	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	multiset( ) : base( ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pred  … 引数の説明
	//! 
	explicit multiset( const key_compare &pred ) : base( pred ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] key … 引数の説明
	//! 
	explicit multiset( const key_type &key ) : base( ){ base::insert( key ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] pred  … 引数の説明
	//! @param[in] alloc … 引数の説明
	//! 
	multiset( const key_compare &pred, const allocator_type &alloc ) : base( pred, alloc ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! 
	template< class Iterator >
	multiset( Iterator first, Iterator last ) : base( first, last ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! @param[in] pred  … 引数の説明
	//! 
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred ) : base( first, last, pred ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] first … 引数の説明
	//! @param[in] last  … 引数の説明
	//! @param[in] pred  … 引数の説明
	//! @param[in] alloc … 引数の説明
	//! 
	template< class Iterator >
	multiset( Iterator first, Iterator last, const key_compare &pred, const allocator_type &alloc ) : base( first, last, pred, alloc ){ }
};


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_SET_H__
