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

/// @file mist/config/pointer.h
//!
//! @brief メモリの自動開放などを行うためのクラス
//!

#ifndef __INCLUDE_MIST_POINTER_H__
#define __INCLUDE_MIST_POINTER_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>
#include <map>


// mist名前空間の始まり
_MIST_BEGIN




namespace __shared_memory__
{
	template < class T >
	struct shared_base
	{
	private:
		typedef T* pointer;

		struct shared_memory_conter
		{
			typedef T* pointer;
			pointer ptr;
			size_t ref_count;
			size_t ref_weak_count;

			shared_memory_conter( ) : ptr( NULL ), ref_count( 0 ), ref_weak_count( 0 ){ }
			shared_memory_conter( pointer p, size_t rc, size_t rwc ) : ptr( p ), ref_count( rc ), ref_weak_count( rwc ){ }
		};

		typedef shared_memory_conter counter_type;
		typedef std::map< pointer, counter_type > ref_table_type;
		typedef typename ref_table_type::iterator ref_table_iterator;

	protected:
		static ref_table_type &get_ref_table( )
		{
			static ref_table_type ref_table_singleton_;
			return( ref_table_singleton_ );
		}

		static ref_table_iterator get_ref_iterator( pointer p )
		{
			ref_table_type &ref_table_ = get_ref_table( );

			if( p == NULL )
			{
				return( get_null_reference( ) ); 
			}
			else
			{
				ref_table_iterator ite = ref_table_.find( p );

				if( ite == ref_table_.end( ) )
				{
					// 最初の追加のため，テーブルを初期化する
					ite = ref_table_.insert( typename ref_table_type::value_type( p, counter_type( p, 0, 0 ) ) ).first;
				}
				return( ite ); 
			}
		}

		static ref_table_iterator get_null_reference( )
		{
			ref_table_type &ref_table_ = get_ref_table( );
			// NULL参照用のデータをあらかじめ挿入しておく
			static ref_table_iterator null_ite_ = ref_table_.insert( typename ref_table_type::value_type( NULL, counter_type( NULL, 0, 0 ) ) ).first;
			return( null_ite_ ); 
		}

		ref_table_iterator ref_ite_;

		shared_base( ) : ref_ite_( get_null_reference( ) ){ }
		shared_base( const shared_base &b ) : ref_ite_( b.ref_ite_ ){}

		pointer get_pointer( ) const { return( ref_ite_->second.ptr ); }

	protected:
		// null 参照に設定する
		void null_ref( )
		{
			ref_ite_ = get_null_reference( );
		}

		// 参照用の参照カウントを増加させる
		void add_ref( )
		{
			// 参照カウントを増やす
			ref_ite_->second.ref_count++;
		}

		// 参照用の参照カウントを増加させる
		void add_ref( pointer p )
		{
			ref_ite_ = get_ref_iterator( p );

			// 参照カウントを増やす
			ref_ite_->second.ref_count++;
		}

		// 参照用の参照カウントを増加させる
		void add_ref( const shared_base &p )
		{
			ref_ite_ = p.ref_ite_;

			// 参照カウントを増やす
			ref_ite_->second.ref_count++;
		}

		// 弱参照用の参照カウントを増加させる
		void add_weak_ref( )
		{
			// 参照カウントを増やす
			ref_ite_->second.ref_weak_count++;
		}

		// 弱参照用の参照カウントを増加させる
		void add_weak_ref( pointer p )
		{
			ref_ite_ = get_ref_iterator( p );

			// 参照カウントを増やす
			ref_ite_->second.ref_weak_count++;
		}

		// 弱参照用の参照カウントを増加させる
		void add_weak_ref( const shared_base &p )
		{
			ref_ite_ = p.ref_ite_;

			// 参照カウントを増やす
			ref_ite_->second.ref_weak_count++;
		}

		void release( bool isArray )
		{
			counter_type &c = ref_ite_->second;
			if( c.ptr != NULL )
			{
				// 参照カウントを減らす
				c.ref_count--;

				// 参照カウントによって，メモリの開放を行う
				if( c.ref_count == 0 )
				{
					// 参照カウントが 0 になったので，メモリを開放しテーブルから削除する
					if( isArray )
					{
						delete [] c.ptr;
					}
					else
					{
						delete c.ptr;
					}

					// ポインタに対する弱参照が存在しない場合は，NULLを代入する
					if( c.ref_weak_count == 0 )
					{
						ref_table_type &table = get_ref_table( );
						table.erase( ref_ite_ );
					}
					else
					{
						c.ptr = NULL;
					}
				}
			}
		}

		void release_weak( )
		{
			counter_type &c = ref_ite_->second;

			if( c.ptr != NULL )
			{
				// 弱参照カウントを減らす
				c.ref_weak_count--;

				if( c.ref_weak_count == 0 && c.ref_count == 0 )
				{
					// ポインタに対する参照と弱参照が存在しない場合は，テーブルから削除する
					ref_table_type &table = get_ref_table( );
					table.erase( ref_ite_ );
				}
			}
		}
	};
}



//! @addtogroup pointer_group メモリ領域の自動開放を行うポインタ
//!
//! @code 次のヘッダを参照
//! #include <mist/config/pointer.h>
//! @endcode
//!
//! @{


/// @brief オブジェクトのスコープが切れると自動的にメモリ開放を行うポインタ
//! 
//! @attention 配列に対しては利用できない
//! @attention 管理しているポインタを他のオブジェクトに譲渡することはできない
//! 
//! @code ポインタの使用例
//! {
//!     mist::scoped_ptr< std::string > a( new std::string( "test" ) );
//!	    std::cout << *a << std::endl;
//! } // <- ここで a に割り当てたメモリ領域が，自動的に開放される
//! @endcode
//! 
template < class T >
class scoped_ptr
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T value_type;				///< @brief 内部データ型．T と同じ
	typedef T* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef T& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef const T* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	pointer ptr_;		///< @brief 管理するポインタ

public:
	/// @brief 管理するポインタを設定
	scoped_ptr( pointer p ) : ptr_( p ){ }

	/// @brief 管理するポインタを開放
	~scoped_ptr( ){ delete ptr_; }


public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }


private:
	scoped_ptr( const scoped_ptr &p );
	const scoped_ptr &operator  =( const scoped_ptr &p );
};



/// @brief オブジェクトのスコープが切れると配列用に確保したメモリの自動開放を行うポインタ
//! 
//! @attention 配列のポインタに対してしか利用できない
//! @attention 管理しているポインタを他のオブジェクトに譲渡することはできない
//! 
//! @code ポインタの使用例
//! {
//!     mist::scoped_array< std::string > a( new std::string[ 2 ] );
//!	
//!	    a[ 0 ] = "test1";
//!	    a[ 1 ] = "test2";
//!	
//!	    std::cout << a[ 0 ] << a[ 1 ] << std::endl;
//! } // <- ここで a に割り当てたメモリ領域が，自動的に開放される
//! @endcode
//! 
template < class T >
class scoped_array
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T value_type;				///< @brief 内部データ型．T と同じ
	typedef T* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef T& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef const T* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	pointer ptr_;		///< @brief 管理するポインタ

public:
	/// @brief 成分を b で初期化する
	scoped_array( pointer p ) : ptr_( p ){ }

	~scoped_array( ){ delete [] ptr_; }


public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }

	reference operator []( difference_type index ){ return( ptr_[ index ] ); }
	const_reference operator []( difference_type index ) const { return( ptr_[ index ] ); }

private:
	scoped_array( const scoped_array &p );
	const scoped_array &operator  =( const scoped_array &p );
};



/// @brief オブジェクトへのポインタの参照カウントを管理し，どこからも参照されなくなったときにメモリを自動開放するポインタ
//! 
//! @attention 配列に対しては利用できない
//! @attention 循環参照が発生した場合は，正しく管理できない
//! 
//! @code ポインタの使用例
//! {
//!     mist::shared_ptr< std::string > a;
//!     {
//!         mist::shared_ptr< std::string > b = new std::string( "test" );
//!	
//!	        a = b;    // a が b と同じメモリ領域を参照するようにし，参照カウントを増やす
//!	
//!	    } // <- ここでは b に割り当てたメモリ領域の参照カウントが減るが，a が同じ領域を参照しているので，メモリ領域は開放されない
//!	
//!  ...
//!	
//! } // <- ここで a に割り当てたメモリ領域がどこからも参照されなくなるため，メモリ領域が自動的に開放される
//! @endcode
//! 
template < class T >
class shared_ptr : public __shared_memory__::shared_base< T >
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T value_type;				///< @brief 内部データ型．T と同じ
	typedef T* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef T& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef const T* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	typedef __shared_memory__::shared_base< T > base;


public:
	/// @brief 未管理のポインタで初期化
	shared_ptr( ){ }

	/// @brief 管理するポインタを設定し，参照カウントを 1 増やす
	shared_ptr( pointer p ){ base::add_ref( p ); }

	/// @brief 他の共有メモリポインタを用いて初期化し，参照カウントを 1 増やす
	shared_ptr( const shared_ptr &p ) : base( p ){ base::add_ref( ); }

	/// @brief 管理するポインタの参照カウントを1減らす
	//!
	//! どこからも参照されなくなったらメモリを開放する
	//! 
	~shared_ptr( ){ base::release( false ); }


	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const shared_ptr &operator =( const shared_ptr &p )
	{
		if( &p != this )
		{
			base::release( false );
			base::add_ref( p );
		}

		return( *this );
	}

public:
	/// @brief ポインタが指す内容への参照を返す
	reference operator *( ){ return( *base::get_pointer( ) ); }

	/// @brief ポインタが指す内容への参照を返す
	const_reference operator *( ) const { return( *base::get_pointer( ) ); }

	/// @brief ポインタが指す内容へのアクセスを行う
	pointer operator ->( ){ return( base::get_pointer( ) ); }

	/// @brief ポインタが指す内容へのアクセスを行う
	const_pointer operator ->( ) const { return( base::get_pointer( ) ); }



	/// @brief 管理しているポインタの参照カウントを減らす
	//!
	//! @attention どこからも参照されなくなったらメモリを開放する
	//! 
	void reset( )
	{ 
		base::release( false );
		base::null_ref( );
	}

public:
	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const shared_ptr &p ) const { return( base::get_pointer( ) == p.get_pointer( ) ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const shared_ptr &p ) const { return( base::get_pointer( ) != p.get_pointer( ) ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const shared_ptr &p ) const { return( base::get_pointer( ) <  p.get_pointer( ) ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const shared_ptr &p ) const { return( base::get_pointer( ) <= p.get_pointer( ) ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const shared_ptr &p ) const { return( base::get_pointer( ) >  p.get_pointer( ) ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const shared_ptr &p ) const { return( base::get_pointer( ) >= p.get_pointer( ) ); }


	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const pointer &p ) const { return( base::get_pointer( ) == p ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const pointer &p ) const { return( base::get_pointer( ) != p ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const pointer &p ) const { return( base::get_pointer( ) <  p ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const pointer &p ) const { return( base::get_pointer( ) <= p ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const pointer &p ) const { return( base::get_pointer( ) >  p ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const pointer &p ) const { return( base::get_pointer( ) >= p ); }

};

template < class T > inline bool operator ==( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }




/// @brief 配列へのポインタの参照カウントを管理し，どこからも参照されなくなったときにメモリを自動開放するポインタ
//! 
//! 対象が配列ということを除けば，shared_ptr と同じ
//! 
//! @attention 配列に対してしか利用できない
//! @attention 循環参照が発生した場合は，正しく管理できない
//! 
//! @code ポインタの使用例
//! {
//!     mist::shared_ptr< std::string > a;
//!     {
//!         mist::shared_ptr< std::string > b = new std::string[ 2 ];
//!	
//!	        a = b;    // a が b と同じメモリ領域を参照するようにし，参照カウントを増やす
//!	
//!	        a[ 0 ] = a[ 1 ] = "test";
//!	
//!	    } // <- ここでは b に割り当てたメモリ領域の参照カウントが減るが，a が同じ領域を参照しているので，メモリ領域は開放されない
//!	
//!  ...
//!	
//! } // <- ここで a に割り当てたメモリ領域がどこからも参照されなくなるため，メモリ領域が自動的に開放される
//! @endcode
//! 
template < class T >
class shared_array : public __shared_memory__::shared_base< T >
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T value_type;				///< @brief 内部データ型．T と同じ
	typedef T* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef T& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef const T* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	typedef __shared_memory__::shared_base< T > base;

public:
	/// @brief 未管理のポインタで初期化
	shared_array( ){ }

	/// @brief 管理するポインタを設定し，参照カウントを 1 増やす
	shared_array( pointer p ){ base::add_ref( p ); }

	/// @brief 他の共有メモリポインタを用いて初期化し，参照カウントを 1 増やす
	shared_array( const shared_array &p ) : base( ) { base::add_ref( p ); }

	/// @brief 管理するポインタの参照カウントを1減らす
	//!
	//! どこからも参照されなくなったらメモリを開放する
	//! 
	~shared_array( ){ base::release( true ); }


	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const shared_array &operator =( const shared_array &p )
	{
		if( &p != this )
		{
			base::release( true );
			base::add_ref( p );
		}

		return( *this );
	}

public:
	/// @brief ポインタが指す内容への参照を返す
	reference operator *( ){ return( *base::get_pointer( ) ); }

	/// @brief ポインタが指す内容への参照を返す
	const_reference operator *( ) const { return( *base::get_pointer( ) ); }

	/// @brief ポインタが指す内容へのアクセスを行う
	pointer operator ->( ){ return( base::get_pointer( ) ); }

	/// @brief ポインタが指す内容へのアクセスを行う
	const_pointer operator ->( ) const { return( base::get_pointer( ) ); }

	/// @brief 配列の要素へのアクセスを行う
	reference operator []( difference_type index ){ return( base::get_pointer( )[ index ] ); }

	/// @brief 配列の要素へのアクセスを行う
	const_reference operator []( difference_type index ) const { return( base::get_pointer( )[ index ] ); }


	/// @brief 管理しているポインタの参照カウントを減らす
	//!
	//! @attention どこからも参照されなくなったらメモリを開放する
	//! 
	void reset( )
	{ 
		base::release( true );
		base::null_ref( );
	}


public:
	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const shared_array &p ) const { return( base::get_pointer( ) == p.get_pointer( ) ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const shared_array &p ) const { return( base::get_pointer( ) != p.get_pointer( ) ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const shared_array &p ) const { return( base::get_pointer( ) <  p.get_pointer( ) ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const shared_array &p ) const { return( base::get_pointer( ) <= p.get_pointer( ) ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const shared_array &p ) const { return( base::get_pointer( ) >  p.get_pointer( ) ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const shared_array &p ) const { return( base::get_pointer( ) >= p.get_pointer( ) ); }


	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const pointer &p ) const { return( base::get_pointer( ) == p ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const pointer &p ) const { return( base::get_pointer( ) != p ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const pointer &p ) const { return( base::get_pointer( ) <  p ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const pointer &p ) const { return( base::get_pointer( ) <= p ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const pointer &p ) const { return( base::get_pointer( ) >  p ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const pointer &p ) const { return( base::get_pointer( ) >= p ); }

};

template < class T > inline bool operator ==( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename shared_array< T >::pointer p1, const shared_array< T > &p2 ){ return( !( p2 >  p1 ) ); }



/// @brief 参照カウント型のメモリに対する，弱参照を扱うポインタ
//! 
//! 参照カウントが 0 にった際に，保持するポインタは NULL となる
//! 
//! @attention weak_ptr 自身は参照カウントを変化させない
//! 
//! @code 弱参照ポインタの使用例
//! {
//! mist::weak_ptr< std::string > a;
//!	
//! std::cout << a << std::endl;    // <- a はどこも参照していないので NULL  が入っている
//!	
//! {
//!     mist::shared_ptr< std::string > b = new std::string[ 2 ];
//!	
//!	    a = b;    // b の弱参照を a に設定する
//!	
//!	    std::cout << a << std::endl;    // <- a は b を参照しているので b の参照先が表示される
//!	
//!	} // <- ここで b に割り当てたメモリ領域がどこからも参照されなくなるため，メモリ領域が自動的に開放される
//!	
//! std::cout << a << std::endl;    // <- a が参照していた b はもう存在しないので， NULL が入っている
//! @endcode
//! 
template < class T >
class weak_ptr : public __shared_memory__::shared_base< T >
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T value_type;				///< @brief 内部データ型．T と同じ
	typedef T* pointer;					///< @brief データ型のポインター型．data の場合，data * となる
	typedef T& reference;				///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;	///< @brief データ型の const 参照．data の場合，const data & となる
	typedef const T* const_pointer;		///< @brief データ型の const ポインター型．data の場合，const data * となる

private:
	typedef __shared_memory__::shared_base< T > base;

public:
	/// @brief 未管理のポインタで初期化
	weak_ptr( ){ base::add_weak_ref( NULL ); }

	/// @brief 共有メモリポインタの弱参照を用いて初期化する
	weak_ptr( shared_ptr< T > &p ){ base::add_weak_ref( p ); }

	/// @brief 共有メモリポインタの弱参照を用いて初期化する
	weak_ptr( const shared_ptr< T > &p ){ base::add_weak_ref( p ); }

	/// @brief 共有メモリ型の配列ポインタの弱参照を用いて初期化する
	weak_ptr( shared_array< T > &p ){ base::add_weak_ref( p ); }

	/// @brief 共有メモリ型の配列ポインタの弱参照を用いて初期化する
	weak_ptr( const shared_array< T > &p ){ base::add_weak_ref( p ); }

	/// @brief 弱参照の参照カウントを1減らす
	~weak_ptr( ){ base::release_weak( ); }


	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( const weak_ptr &p )
	{
		if( this != &p )
		{
			base::release_weak( );
			base::add_weak_ref( p );
		}

		return( *this );
	}

	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( const shared_ptr< T > &p )
	{
		base::release_weak( );
		base::add_weak_ref( p );

		return( *this );
	}

	/// @brief 他の共有メモリ型の配列ポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( const shared_array< T > &p )
	{
		base::release_weak( );
		base::add_weak_ref( p );

		return( *this );
	}


public:
	/// @brief ポインタが指す内容への参照を返す
	reference operator *( ){ return( *base::get_pointer( ) ); }

	/// @brief ポインタが指す内容への参照を返す
	const_reference operator *( ) const { return( *base::get_pointer( ) ); }

	/// @brief ポインタが指す内容へのアクセスを行う
	pointer operator ->( ){ return( base::get_pointer( ) ); }

	/// @brief ポインタが指す内容へのアクセスを行う
	const_pointer operator ->( ) const { return( base::get_pointer( ) ); }

	/// @brief 配列の要素へのアクセスを行う
	reference operator []( difference_type index ){ return( base::get_pointer( )[ index ] ); }

	/// @brief 配列の要素へのアクセスを行う
	const_reference operator []( difference_type index ) const { return( base::get_pointer( )[ index ] ); }


	/// @brief 管理しているポインタの参照カウントを減らす
	//!
	//! @attention どこからも参照されなくなったらメモリを開放する
	//! 
	void reset( )
	{ 
		base::release_weak( );
		base::null_ref( );
	}


public:
	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const weak_ptr &p ) const { return( base::get_pointer( ) == p.get_pointer( ) ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const weak_ptr &p ) const { return( base::get_pointer( ) != p.get_pointer( ) ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const weak_ptr &p ) const { return( base::get_pointer( ) <  p.get_pointer( ) ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const weak_ptr &p ) const { return( base::get_pointer( ) <= p.get_pointer( ) ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const weak_ptr &p ) const { return( base::get_pointer( ) >  p.get_pointer( ) ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const weak_ptr &p ) const { return( base::get_pointer( ) >= p.get_pointer( ) ); }


	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const pointer &p ) const { return( base::get_pointer( ) == p ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const pointer &p ) const { return( base::get_pointer( ) != p ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const pointer &p ) const { return( base::get_pointer( ) <  p ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const pointer &p ) const { return( base::get_pointer( ) <= p ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const pointer &p ) const { return( base::get_pointer( ) >  p ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const pointer &p ) const { return( base::get_pointer( ) >= p ); }

};


template < class T > inline bool operator ==( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename weak_ptr< T >::pointer p1, const
											 
											 weak_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }


/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … scoped_ptr 配列
//! 
//! @return 入力されたストリーム
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const scoped_ptr< T > &p )
{
	out << &( *p );
	return( out );
}


/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … scoped_array 配列
//! 
//! @return 入力されたストリーム
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const scoped_array< T > &p )
{
	out << &( *p );
	return( out );
}


/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … shared_ptr 配列
//! 
//! @return 入力されたストリーム
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const shared_ptr< T > &p )
{
	out << &( *p );
	return( out );
}

/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     p   … weak_ptr 配列
//! 
//! @return 入力されたストリーム
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const weak_ptr< T > &p )
{
	out << &( *p );
	return( out );
}

/// @}
//  メモリ領域の自動開放を行うポインタ



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_POINTER_H__
