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


// MISTで利用する基底のデータ型


/// @brief オブジェクトのスコープが切れると自動的にメモリ開放を行うポインタ
//! 
//! @attention 配列に対しては利用できない
//! @attention 管理しているポインタを他のオブジェクトに譲渡することはできない
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



namespace __shared_memory__
{
	template < class T >
	struct shared_memory_controller
	{
	private:
		typedef T* pointer;

		struct shared_memory_conter
		{
			typedef T* pointer;
			bool isArray;
			pointer ptr;
			size_t ref_count;
			size_t ref_weak_count;

			shared_memory_conter( ) : isArray( false ), ptr( NULL ), ref_count( 0 ), ref_weak_count( 0 ){ }
			shared_memory_conter( bool barray, pointer p, size_t rc, size_t rwc ) : isArray( barray ), ptr( p ), ref_count( rc ), ref_weak_count( rwc ){ }
		};

		typedef std::map< pointer, shared_memory_conter > ref_table_type;


	public:
		static ref_table_type &get_ref_table( )
		{
			static ref_table_type singleton_;
			return( singleton_ );
		}

		static void add_ref( pointer p, bool isArray )
		{
			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite == table.end( ) )
				{
					// 最初の追加のため，テーブルを初期化する
					table[ p ] = shared_memory_conter( isArray, p, 1, 0 );
				}
				else
				{
					// 参照カウントを増やす
					table[ p ].ref_count++;
				}
			}
		}

		// 弱参照用の参照カウントを増加させ，参照用ポインタを返す
		static pointer * add_weak_ref( pointer p )
		{

			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite != table.end( ) )
				{
					// 弱参照カウントを増やす
					shared_memory_conter &c = ite->second;
					c.ref_weak_count++;

					return( &c.ptr );
				}
			}

			// 不適切な参照の場合用
			static pointer dmy_ptr = NULL;
			return( &dmy_ptr );
		}

		static void release( pointer p )
		{
			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite != table.end( ) )
				{
					shared_memory_conter &c = ite->second;

					// 参照カウントを減らす
					c.ref_count --;

					// 参照カウントによって，メモリの開放を行う
					if( c.ref_count == 0 )
					{
						// 参照カウントが 0 になったので，メモリを開放しテーブルから削除する
						if( c.isArray )
						{
							delete [] p;
						}
						else
						{
							delete p;
						}

						// ポインタに対する弱参照が存在しない場合は，NULLを代入する
						if( c.ref_weak_count == 0 )
						{
							table.erase( ite );
						}
						else
						{
							c.ptr = NULL;
						}
					}
				}
			}
		}

		static void release_weak( pointer p )
		{
			if( p != NULL )
			{
				ref_table_type &table = get_ref_table( );
				typename ref_table_type::iterator ite = table.find( p );

				if( ite != table.end( ) )
				{
					shared_memory_conter &c = ite->second;

					// 弱参照カウントを減らす
					c.ref_weak_count--;

					if( c.ref_weak_count == 0 && c.ref_count == 0 )
					{
						// ポインタに対する弱参照が存在しない場合は，NULLを代入する
						table.erase( ite );
					}
				}
			}
		}
	};
}


/// @brief オブジェクトへのポインタの参照カウントを管理し，どこからも参照されなくなったときにメモリを自動開放するポインタ
//! 
//! @attention 配列に対しては利用できない
//! @attention 循環参照が発生した場合は，正しく管理できない
//! 
template < class T >
class shared_ptr
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
	typedef __shared_memory__::shared_memory_controller< T > shared_memory_controller;

	pointer ptr_;		///< @brief 管理するポインタ


public:
	/// @brief 未管理のポインタで初期化
	shared_ptr( ) : ptr_( NULL ){ }

	/// @brief 管理するポインタを設定し，参照カウントを 1 増やす
	shared_ptr( pointer p ) : ptr_( p ){ shared_memory_controller::add_ref( ptr_, false ); }

	/// @brief 他の共有メモリポインタを用いて初期化し，参照カウントを 1 増やす
	shared_ptr( const shared_ptr &p ) : ptr_( p.ptr_ ){ shared_memory_controller::add_ref( ptr_, false ); }

	/// @brief 管理するポインタの参照カウントを1減らす
	//!
	//! どこからも参照されなくなったらメモリを開放する
	//! 
	~shared_ptr( ){ shared_memory_controller::release( ptr_ ); }


	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const shared_ptr &operator =( const shared_ptr &p )
	{
		if( &p != this )
		{
			shared_memory_controller::release( ptr_ );
			shared_memory_controller::add_ref( ptr_ = p.ptr_, false );
		}

		return( *this );
	}

public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }


public:
	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const shared_ptr &p ) const { return( ptr_ == p.ptr_ ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const shared_ptr &p ) const { return( ptr_ != p.ptr_ ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const shared_ptr &p ) const { return( ptr_ <  p.ptr_ ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const shared_ptr &p ) const { return( ptr_ <= p.ptr_ ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const shared_ptr &p ) const { return( ptr_ >  p.ptr_ ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const shared_ptr &p ) const { return( ptr_ >= p.ptr_ ); }


	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const pointer &p ) const { return( ptr_ == p ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const pointer &p ) const { return( ptr_ != p ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const pointer &p ) const { return( ptr_ <  p ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const pointer &p ) const { return( ptr_ <= p ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const pointer &p ) const { return( ptr_ >  p ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const pointer &p ) const { return( ptr_ >= p ); }

};

template < class T > inline bool operator ==( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename shared_ptr< T >::pointer p1, const shared_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }




/// @brief 配列へのポインタの参照カウントを管理し，どこからも参照されなくなったときにメモリを自動開放するポインタ
//! 
//! @attention 配列に対してしか利用できない
//! @attention 循環参照が発生した場合は，正しく管理できない
//! 
template < class T >
class shared_array
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
	typedef __shared_memory__::shared_memory_controller< T > shared_memory_controller;

	pointer ptr_;		///< @brief 管理するポインタ


public:
	/// @brief 未管理のポインタで初期化
	shared_array( ) : ptr_( NULL ){ }

	/// @brief 管理するポインタを設定し，参照カウントを 1 増やす
	shared_array( pointer p ) : ptr_( p ){ shared_memory_controller::add_ref( ptr_, true ); }

	/// @brief 他の共有メモリポインタを用いて初期化し，参照カウントを 1 増やす
	shared_array( const shared_array &p ) : ptr_( p.ptr_ ){ shared_memory_controller::add_ref( ptr_, true ); }

	/// @brief 管理するポインタの参照カウントを1減らす
	//!
	//! どこからも参照されなくなったらメモリを開放する
	//! 
	~shared_array( ){ shared_memory_controller::release( ptr_ ); }


	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const shared_array &operator =( const shared_array &p )
	{
		if( &p != this )
		{
			shared_memory_controller::release( ptr_ );
			shared_memory_controller::add_ref( ptr_ = p.ptr_, true );
		}

		return( *this );
	}

public:
	reference operator *( ){ return( *ptr_ ); }
	const_reference operator *( ) const { return( *ptr_ ); }

	pointer operator ->( ){ return( ptr_ ); }
	const_pointer operator ->( ) const { return( ptr_ ); }

	reference operator []( difference_type index ){ return( ptr_[ index ] ); }
	const_reference operator []( difference_type index ) const { return( ptr_[ index ] ); }


public:
	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const shared_array &p ) const { return( ptr_ == p.ptr_ ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const shared_array &p ) const { return( ptr_ != p.ptr_ ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const shared_array &p ) const { return( ptr_ <  p.ptr_ ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const shared_array &p ) const { return( ptr_ <= p.ptr_ ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const shared_array &p ) const { return( ptr_ >  p.ptr_ ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const shared_array &p ) const { return( ptr_ >= p.ptr_ ); }


	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const pointer &p ) const { return( ptr_ == p ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const pointer &p ) const { return( ptr_ != p ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const pointer &p ) const { return( ptr_ <  p ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const pointer &p ) const { return( ptr_ <= p ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const pointer &p ) const { return( ptr_ >  p ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const pointer &p ) const { return( ptr_ >= p ); }

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
template < class T >
class weak_ptr
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
	typedef __shared_memory__::shared_memory_controller< T > shared_memory_controller;

	pointer ptr_;		///< @brief 弱参照先のポインタ(弱参照の開放用)
	pointer *pptr_;		///< @brief 弱参照先のポインタ


public:
	/// @brief 未管理のポインタで初期化
	weak_ptr( ) : ptr_( NULL ), pptr_( shared_memory_controller::add_weak_ref( NULL ) ){ }

	/// @brief 共有メモリポインタの弱参照を用いて初期化する
	weak_ptr( shared_ptr< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief 共有メモリポインタの弱参照を用いて初期化する
	weak_ptr( const shared_ptr< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief 共有メモリ型の配列ポインタの弱参照を用いて初期化する
	weak_ptr( shared_array< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief 共有メモリ型の配列ポインタの弱参照を用いて初期化する
	weak_ptr( const shared_array< T > &p ) : pptr_( shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) ) ){ }

	/// @brief 弱参照の参照カウントを1減らす
	~weak_ptr( ){ shared_memory_controller::release_weak( ptr_ ); }


	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( weak_ptr &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}

	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( shared_ptr< T > &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}

	/// @brief 他の共有メモリポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( const shared_ptr< T > &p )
	{
		if( &p != this )
		{
			shared_memory_controller::release_weak( *pptr_ );
			pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );
		}

		return( *this );
	}

	/// @brief 他の共有メモリ型の配列ポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( shared_array< T > &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}

	/// @brief 他の共有メモリ型の配列ポインタを代入する
	//!
	//! @attention すでに管理しているポインタがある場合は，参照カウントを1減らす
	//! @attention その際に，どこからも参照されなくなったらメモリを開放する
	//! 
	const weak_ptr &operator =( const shared_array< T > &p )
	{
		shared_memory_controller::release_weak( *pptr_ );
		pptr_ = shared_memory_controller::add_weak_ref( ptr_ = ( p == NULL ? NULL : &( *p ) ) );

		return( *this );
	}


public:
	reference operator *( ){ return( **pptr_ ); }
	const_reference operator *( ) const { return( **pptr_ ); }

	pointer operator ->( ){ return( *pptr_ ); }
	const_pointer operator ->( ) const { return( *pptr_ ); }

	reference operator []( difference_type index ){ return( ptr_[ index ] ); }
	const_reference operator []( difference_type index ) const { return( ptr_[ index ] ); }


public:
	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const weak_ptr &p ) const { return( *pptr_ == *p.pptr_ ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const weak_ptr &p ) const { return( *pptr_ != *p.pptr_ ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const weak_ptr &p ) const { return( *pptr_ <  *p.pptr_ ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const weak_ptr &p ) const { return( *pptr_ <= *p.pptr_ ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const weak_ptr &p ) const { return( *pptr_ >  *p.pptr_ ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const weak_ptr &p ) const { return( *pptr_ >= *p.pptr_ ); }


	/// @brief 2つのポインタが同一かどうかを判定する
	bool operator ==( const pointer &p ) const { return( *pptr_ == p ); }

	/// @brief 2つのポインタが等しくないどうかを判定する
	bool operator !=( const pointer &p ) const { return( *pptr_ != p ); }

	/// @brief 2つのポインタの <  を判定する
	bool operator < ( const pointer &p ) const { return( *pptr_ <  p ); }

	/// @brief 2つのポインタの <= を判定する
	bool operator <=( const pointer &p ) const { return( *pptr_ <= p ); }

	/// @brief 2つのポインタの >  を判定する
	bool operator > ( const pointer &p ) const { return( *pptr_ >  p ); }

	/// @brief 2つのポインタの >= を判定する
	bool operator >=( const pointer &p ) const { return( *pptr_ >= p ); }

};


template < class T > inline bool operator ==( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 == p1 ); }
template < class T > inline bool operator !=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( p2 != p1 ); }

template < class T > inline bool operator < ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <= p1 ) ); }
template < class T > inline bool operator <=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 <  p1 ) ); }
template < class T > inline bool operator > ( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 >= p1 ) ); }
template < class T > inline bool operator >=( const typename weak_ptr< T >::pointer p1, const weak_ptr< T > &p2 ){ return( !( p2 >  p1 ) ); }


/// @brief 指定されたストリームにデータを出力する
//! 
//! @param[in,out] out … 入力と出力を行うストリーム
//! @param[in]     a   … scoped_ptr 配列
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
//! @param[in]     a   … scoped_array 配列
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
//! @param[in]     a   … shared_ptr 配列
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
//! @param[in]     a   … weak_ptr 配列
//! 
//! @return 入力されたストリーム
//! 
template < class T >
inline std::ostream &operator <<( std::ostream &out, const weak_ptr< T > &p )
{
	out << &( *p );
	return( out );
}



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_POINTER_H__
