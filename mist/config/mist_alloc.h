#ifndef __INCLUDE_MIST_ALLOC_H__
#define __INCLUDE_MIST_ALLOC_H__

#include "mist_conf.h"

// mist名前空間の始まり
_MIST_BEGIN


template < bool b >
struct mist_memory_operator
{
	// num 個のオブジェクトを確保し，デフォルトの値で初期化する
	// 組み込み型のデータの場合は 0 で初期化する
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		typename Allocator::value_type obj;
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.construct( &( ptr[i] ), obj );
		return( ptr );
	}

	// num 個のオブジェクトを確保し，引数の値で初期化する
	// 組み込み型のデータの場合は値を代入して初期化する
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.construct( &( ptr[i] ), obj );
		return( ptr );
	}

	// num 個のオブジェクトを確保し，ポインタで囲まれた間のデータで初期化する
	// 組み込み型のデータの場合は値を代入して初期化する
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::pointer p = ptr ; s != e ; p++, s++ ) allocator.construct( p, *s );
		return( ptr );
	}

	// ptr から num 個のオブジェクトを開放しデストラクタを呼び出す
	// 組み込み型の場合はデストラクタは呼び出さない
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Deallocator" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) allocator.destroy( &( ptr[i] ) );
		allocator.deallocate( ptr, num );
	}

	// ポインタ s から e までの間のデータを x にコピーする
	// 組み込み型の場合は memcpy を利用する
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Copy Function" << ::std::endl;
#endif
		while( s != e )
		{
			*x = *s;
			++x;
			++s;
		}
		return( x );
	}

	// ポインタ ptr から num 個のデータを to にコピーする
	// 組み込み型の場合は memcpy を利用する
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator &allocator, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Copy Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) to[i] = ptr[i];
		return( to + num );
	}

	// ポインタ ptr から num 個のデータにobjを代入する
	// char 型の場合は memset を利用する
	template < class Allocator >
	static void fill_objects1( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Clean Function" << ::std::endl;
#endif
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}

	// ポインタ ptr から num 個のデータに初期値を代入する
	// 組み込み型の場合は memset を利用する
	template < class Allocator >
	static void fill_objects2( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Normal Clean Function" << ::std::endl;
#endif
		typename Allocator::value_type obj;
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
	}
};




template <>
struct mist_memory_operator< true >
{
	// num 個のオブジェクトを確保し，デフォルトの値で初期化する
	// 組み込み型のデータの場合は 0 で初期化する
	template < class Allocator >
	static typename Allocator::pointer allocate_objects1( Allocator &allocator, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memset Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		::memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}

	// num 個のオブジェクトを確保し，引数の値で初期化する
	// 組み込み型のデータの場合は値を代入して初期化する
	template < class Allocator >
	static typename Allocator::pointer allocate_objects2( Allocator &allocator, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Specialized Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		for( typename Allocator::size_type i = 0 ; i < num ; i++ ) ptr[i] = obj;
		return( ptr );
	}

	// num 個のオブジェクトを確保し，ポインタで囲まれた間のデータで初期化する
	// 組み込み型のデータの場合は値を代入して初期化する
	template < class Allocator >
	static typename Allocator::pointer allocate_objects3( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Allocator" << ::std::endl;
#endif
		typename Allocator::pointer ptr = allocator.allocate( num, 0 );
		memcpy( ptr, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( ptr );
	}

	// ptr から num 個のオブジェクトを開放しデストラクタを呼び出す
	// 組み込み型の場合はデストラクタは呼び出さない
	template < class Allocator >
	static void deallocate_objects( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Specialized Deallocator" << ::std::endl;
#endif
		allocator.deallocate( ptr, num );
	}

	// ポインタ s から e までの間のデータを x にコピーする
	// 組み込み型の場合は memcpy を利用する
	template < class Allocator >
	static typename Allocator::pointer copy_objects1( Allocator &allocator, typename Allocator::const_pointer s, typename Allocator::const_pointer e, typename Allocator::pointer x )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( x, s, ( e - s ) * sizeof( typename Allocator::value_type ) );
		return( x + ( e - s ) );
	}

	// ポインタ ptr から num 個のデータを to にコピーする
	// 組み込み型の場合は memcpy を利用する
	template < class Allocator >
	static typename Allocator::pointer copy_objects2( Allocator &allocator, typename Allocator::const_pointer ptr, typename Allocator::size_type num, typename Allocator::pointer to )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Copy Function" << ::std::endl;
#endif
		memcpy( to, ptr, num * sizeof( typename Allocator::value_type ) );
		return( to + num );
	}

	// ポインタ ptr から num 個のデータにobjを代入する
	// char 型の場合は memset を利用する
	template < class Allocator >
	static void fill_objects1( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num, typename Allocator::const_reference obj )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, obj, num * sizeof( typename Allocator::value_type ) );
	}

	// ポインタ ptr から num 個のデータに初期値を代入する
	// 組み込み型の場合は memset を利用する
	template < class Allocator >
	static void fill_objects2( Allocator &allocator, typename Allocator::pointer ptr, typename Allocator::size_type num )
	{
#if _MIST_ALLOCATE_TEST_ != 0
		::std::cout << "Memcpy Clean Function" << ::std::endl;
#endif
		memset( ptr, 0, num * sizeof( typename Allocator::value_type ) );
	}
};



// MIST内部でSTLのアロケータを効果的に利用するための仕組み
// データの方によって，メモリコピー等を高速化してある
template < class T, class Allocator >
class mist_allocator : public Allocator
{
public:
	typedef Allocator base;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;

public:
	// num 個のオブジェクトのメモリを確保し初期化を行う（組み込み型かどうかで内部の実装を分岐）
	pointer allocate_objects( size_type num )
	{
		if( num <= 0 ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects1( *this, num ) );
	}

	// num 個のオブジェクトのメモリを確保し obj を用いて初期化を行う（組み込み型かどうかで内部の実装を分岐）
	pointer allocate_objects( size_type num, const_reference obj )
	{
		if( num <= 0 ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects2( *this, num, obj ) );
	}

	// num 個のオブジェクトを確保し，ポインタで囲まれた間のデータで初期化する(組み込み型のデータの場合は memcpy を利用する)
	pointer allocate_objects( const_pointer s, const_pointer e )
	{
		if( s >= e ) return( NULL );
		return( mist_memory_operator< is_arithmetic< T >::value >::allocate_objects3( *this, s, e ) );
	}

	// num 個のオブジェクトのメモリを開放し，デストラクタを呼び出す（組み込み型の場合はデストラクタは呼ばない）
	void deallocate_objects( pointer ptr, size_type num )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_arithmetic< T >::value >::deallocate_objects( *this, ptr, num );
	}

	// ポインタ s から e までの間のデータを x にコピーする
	pointer copy_objects( const_pointer s, const_pointer e, pointer x )
	{
		if( s >= e ) return( x );
		return( mist_memory_operator< is_arithmetic< T >::value >::copy_objects1( *this, s, e, x ) );
	}

	// ポインタ ptr から num 個のデータを to にコピーする
	pointer copy_objects( const_pointer ptr, size_type num, pointer to )
	{
		if( num <= 0 ) return( to );
		return( mist_memory_operator< is_arithmetic< T >::value >::copy_objects2( *this, ptr, num, to ) );
	}

	// ポインタ ptr から num 個のデータにobjを代入する
	void fill_objects( pointer ptr, size_type num, const_reference obj )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_char< T >::value >::fill_objects1( *this, ptr, num, obj );
	}

	// ポインタ ptr から num 個のデータに初期値を代入する
	void fill_objects( pointer ptr, size_type num )
	{
		if( num <= 0 ) return;
		mist_memory_operator< is_arithmetic< T >::value >::fill_objects2( *this, ptr, num );
	}

	// ptr から始まり num 個存在するオブジェクトを dest_num 個まで縮める
	pointer trim_objects( pointer ptr, size_type num, size_type dest_num )
	{
		if( num < dest_num ) return( ptr );			// 開放しようとしている配列の要素数は，トリム後の配列サイズよりサイズよりも小さい例外
		if( num < 0 ) return( NULL );				// トリム先の配列サイズが0より小さい例外
		if( num == dest_num ) return( ptr );		// トリムによる変更の必要なし
		if( num == 0 ) return( NULL );
#if _MIST_ALLOCATOR_MEMORY_TRIM__ != 0
		deallocate_objects( ptr + dest_num, num - dest_num );
#else
		deallocate_objects( ptr, num );
		ptr = allocate_objects( num );
#endif
		return( dest_num == 0 ? NULL : ptr );
	}


	const Allocator &operator=( const Allocator &alloc )
	{
		if( &alloc != this ) base::operator=( alloc );
		return( *this );
	}
	mist_allocator( ) : base( ){}
	mist_allocator( const Allocator &alloc ) : base( alloc ){}
};



// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_MIST_ALLOC_H__
