#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__

#include <iostream>
#include <iterator>


#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
#endif

// mist名前空間の始まり
_MIST_BEGIN

#if _MSC_VER <= 1200

#define _MIST_CONST( type, name, value ) enum{ name = value }

#else

#define _MIST_CONST( type, name, value ) static const type name = value

#endif


// 型属性を調べる

// char 判定
// Tが１バイトの文字列型であれば真に評価する
template< class T > struct is_char        { _MIST_CONST( bool, value, false ); };
template<> struct is_char< unsigned char >{ _MIST_CONST( bool, value, true  ); };
template<> struct is_char< signed char >  { _MIST_CONST( bool, value, true  ); };
template<> struct is_char< char >         { _MIST_CONST( bool, value, true  ); };

// arithmetic 判定
// T が算術型であれば真に評価する。汎整数型か浮動小数点型のいずれかがこれにあたる
template< class T > struct is_arithmetic                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_arithmetic< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< char >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< float >         { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< double >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< long double >   { _MIST_CONST( bool, value, true  ); };

// データ型のに関する情報
template< class T >
struct type_trait{ typedef T value_type; };



#define _MIST_ALLOCATE_TEST_			0

#ifdef WIN32

#define _MIST_ALLOCATOR_MEMORY_TRIM__	0	// VC標準のSTLではサポートしていないのでオフにする

#else

#define _MIST_ALLOCATOR_MEMORY_TRIM__	1	// VC以外のSTLではサポートしているのでオンにする

#endif


#ifndef NDEBUG

#define _CHECK_ACCESS_VIOLATION_		1	// 配列要素へのアクセス違反をチェックするかどうか
#define _CHECK_ARRAY_OPERATION_			1	// 1次元配列の演算の整合性をチェックするかどうか
#define _CHECK_ARRAY1_OPERATION_		1	// 1次元画像の演算の整合性をチェックするかどうか
#define _CHECK_ARRAY2_OPERATION_		1	// 2次元画像の演算の整合性をチェックするかどうか
#define _CHECK_ARRAY3_OPERATION_		1	// 3次元画像の演算の整合性をチェックするかどうか

#define _CHECK_MATRIX_OPERATION_		1	// 行列演算の整合性をチェックするかどうか
#define _USE_EXPRESSION_TEMPLATE_		0	// Expression Templateを利用するかどうか

#else

#define _CHECK_ACCESS_VIOLATION_		0	// 配列要素へのアクセス違反をチェックするかどうか
#define _CHECK_ARRAY_OPERATION_			0
#define _CHECK_ARRAY1_OPERATION_		0
#define _CHECK_ARRAY2_OPERATION_		0
#define _CHECK_ARRAY3_OPERATION_		0

#define _CHECK_MATRIX_OPERATION_		0
#define _USE_EXPRESSION_TEMPLATE_		1

#endif



// MISTで利用する基底のデータ型

// boolean演算を行うためのもの
class boolean
{
public:
	typedef bool& reference;
	typedef const bool& const_reference;
	typedef bool value_type;
	typedef bool* pointer;
	typedef const bool* const_pointer;

private:
	bool value_;

public:
	boolean( ) : value_( false ){ }
	boolean( const boolean &b ) : value_( b.value_ ){ }
	boolean( const value_type &b ) : value_( b ){ }

	const boolean &operator  =( const boolean &b ){ value_ = b.value_;  return( *this ); }
	const boolean &operator  =( const value_type &b ){ value_ = b;   return( *this ); }

	const boolean &operator +=( const boolean &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const boolean &operator -=( const boolean &b ){ value_ = value_ && !b.value_; return( *this ); }
	const boolean &operator *=( const boolean &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const boolean &operator /=( const boolean &b ){ value_ = value_ ==  b.value_; return( *this ); }
	const boolean &operator %=( const boolean &b ){ value_ = value_ && !b.value_; return( *this ); }
	const boolean &operator |=( const boolean &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const boolean &operator &=( const boolean &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const boolean &operator ^=( const boolean &b ){ value_ = value_ !=  b.value_; return( *this ); }

	bool operator ==( const boolean &b ) const { return( value_ == b.value_ ); }
	bool operator !=( const boolean &b ) const { return( value_ != b.value_ ); }
	bool operator < ( const boolean &b ) const { return( value_ <  b.value_ ); }
	bool operator <=( const boolean &b ) const { return( value_ <= b.value_ ); }
	bool operator > ( const boolean &b ) const { return( value_ >  b.value_ ); }
	bool operator >=( const boolean &b ) const { return( value_ >= b.value_ ); }

	value_type get_value( ) const { return( value_ ); }

	operator bool( ) const { return( value_ ); }
};

inline const boolean operator +( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) += b2 ); }
inline const boolean operator -( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) -= b2 ); }
inline const boolean operator *( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) *= b2 ); }
inline const boolean operator /( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) /= b2 ); }
inline const boolean operator %( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) %= b2 ); }
inline const boolean operator |( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) |= b2 ); }
inline const boolean operator &( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) &= b2 ); }
inline const boolean operator ^( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) ^= b2 ); }

inline std::ostream &operator <<( std::ostream &out, const boolean &v )
{
	out << v.get_value( );
	return( out );
}


// カラー画像用
template< class T >
struct rgb
{
public:
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;

public:
	value_type r;
	value_type g;
	value_type b;

	rgb( const value_type &rr, const value_type &gg, const value_type &bb ) : r( rr ), g( gg ), b( bb ){ }

	const rgb &operator +=( const rgb &c ){ r += c.r; g += c.g; b += c.b; return( *this ); }
	const rgb &operator -=( const rgb &c ){ r -= c.r; g -= c.g; b -= c.b; return( *this ); }
	const rgb &operator *=( const rgb &c ){ r *= c.r; g *= c.g; b *= c.b; return( *this ); }
	const rgb &operator /=( const rgb &c ){ r /= c.r; g /= c.g; b /= c.b; return( *this ); }
	const rgb &operator %=( const rgb &c ){ r %= c.r; g %= c.g; b %= c.b; return( *this ); }
	const rgb &operator |=( const rgb &c ){ r |= c.r; g |= c.g; b |= c.b; return( *this ); }
	const rgb &operator &=( const rgb &c ){ r &= c.r; g &= c.g; b &= c.b; return( *this ); }
	const rgb &operator ^=( const rgb &c ){ r ^= c.r; g ^= c.g; b ^= c.b; return( *this ); }

	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && c == c.b ); }
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }
	bool operator < ( const rgb &c ) const { return( r <  c.r || ( r == c.r && g <  c.g ) || ( r == c.r && g == c.g  &&c <  c.b ) ); }
	bool operator <=( const rgb &c ) const { return( !( c < *this ) ); }
	bool operator > ( const rgb &c ) const { return( c < *this ); }
	bool operator >=( const rgb &c ) const { return( !( *this < c ) ); }

	// NTSC系加重平均法により，グレースケールへ変換する
	value_type get_value( ) const { return( static_cast< value_type >( r * 0.298912 + g * 0.586611 + b + 0.114478 ) ); }

	operator value_type( ) const { return( get_value( ) ); }
};

template < class T > inline const rgb< T > operator +( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) += c2 ); }
template < class T > inline const rgb< T > operator -( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) -= c2 ); }
template < class T > inline const rgb< T > operator *( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) *= c2 ); }
template < class T > inline const rgb< T > operator /( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) /= c2 ); }
template < class T > inline const rgb< T > operator %( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) %= c2 ); }
template < class T > inline const rgb< T > operator |( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) |= c2 ); }
template < class T > inline const rgb< T > operator &( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) &= c2 ); }
template < class T > inline const rgb< T > operator ^( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) ^= c2 ); }

template < class T > inline std::ostream &operator <<( std::ostream &out, const rgb< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << " )";
	return( out );
}


// mistコンテナで利用するランダムアクセスイテレータ
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
{
// Distance, Pointer, Referenceはほとんどの場合省略可能
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef std::size_t size_type;
	typedef Reference reference;

private:
	pointer data_;
	size_type diff_pointer_;

public:
	// コンストラクタ
	mist_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }
	mist_iterator1( const mist_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }

	// コピー演算子
	const mist_iterator1& operator =( const mist_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}

	// 要素のアクセス
	reference operator *(){ return( *data_ ); }
	reference operator []( Distance dist ){ return( data_[ dist * diff_pointer_ ] ); }

	// 移動
	mist_iterator1& operator ++( ) // 前置型
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator1 operator ++( int ) // 後置型
	{
		mist_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator1& operator --( ) // 前置型
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator1 operator --( int ) // 後置型
	{
		mist_iterator1 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_iterator1& operator +=( Distance dist )
	{
		data_ += dist * diff_pointer_;
		return( *this );
	}
	const mist_iterator1& operator -=( Distance dist )
	{
		data_ -= dist * diff_pointer_;
		return( *this );
	}

	// 比較
	bool operator ==( const mist_iterator1 &ite ) const { return( data_ == ite.data_ ); }
	bool operator !=( const mist_iterator1 &ite ) const { return( data_ != ite.data_ ); }
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
};


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}




// mistコンテナで利用する逆方向ランダムアクセスイテレータ
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
class mist_reverse_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
{
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef std::size_t size_type;
	typedef Reference reference;

private:
	pointer data_;
	size_type diff_pointer_;

public:
	// コンストラクタ
	mist_reverse_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }
	mist_reverse_iterator1( const mist_reverse_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }

	// コピー演算子
	const mist_reverse_iterator1& operator =( const mist_reverse_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}

	// 要素のアクセス
	reference operator *(){ return( *data_ ); }
	reference operator []( Distance dist ){ return( *( data_ - dist * diff_pointer_ ) ); }

	// 移動
	mist_reverse_iterator1& operator ++( ) // 前置型
	{
		*this += 1;
		return( *this );
	}
	const mist_reverse_iterator1 operator ++( int ) // 後置型
	{
		mist_reverse_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_reverse_iterator1& operator --( ) // 前置型
	{
		*this -= 1;
		return( *this );
	}
	const mist_reverse_iterator1 operator --( int ) // 後置型
	{
		mist_reverse_iterator1 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_reverse_iterator1& operator +=( Distance dist )
	{
		data_ -= dist * diff_pointer_;
		return( *this );
	}
	const mist_reverse_iterator1& operator -=( Distance dist )
	{
		data_ += dist * diff_pointer_;
		return( *this );
	}

	// 比較
	bool operator ==( const mist_reverse_iterator1 &ite ) const { return( data_ == ite.data_ ); }
	bool operator !=( const mist_reverse_iterator1 &ite ) const { return( data_ != ite.data_ ); }
	bool operator < ( const mist_reverse_iterator1 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator <=( const mist_reverse_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
	bool operator > ( const mist_reverse_iterator1 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator >=( const mist_reverse_iterator1 &ite ) const { return( data_ <= ite.data_ ); }
};


template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator1< T, Distance, Pointer, Reference > operator +( const mist_reverse_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_reverse_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_reverse_iterator1< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator1< T, Distance, Pointer, Reference > operator +( const mist_reverse_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_reverse_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator1< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_reverse_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_reverse_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator1< T, Distance, Pointer, Reference > operator -( const mist_reverse_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_reverse_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_reverse_iterator1< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator1< T, Distance, Pointer, Reference > operator -( const mist_reverse_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_reverse_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}



// mistコンテナで利用するランダムアクセスイテレータ
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
{
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef std::size_t size_type;
	typedef Reference reference;

private:
	pointer data_;
	size_type current_position_;
	size_type diff_pointer1_;
	size_type diff_pointer2_;
	size_type diff_boundary_;

private:
	const pointer address( ) const 
	{
		size_type j = diff_boundary_ == 0 ? 0 : current_position_ / diff_boundary_;
		size_type i = current_position_ - j * diff_boundary_;
		return( data_ + i * diff_pointer1_ + j * diff_pointer2_ );
	}

public:
	// コンストラクタ
	mist_iterator2( pointer p = NULL, size_type diff1 = 1, size_type diff2 = 1, size_type bound = 1 )
				: data_( p ), current_position_( 0 ), diff_pointer1_( diff1 ), diff_pointer2_( diff2 ), diff_boundary_( bound )
	{
	}

	mist_iterator2( const mist_iterator2 &ite ) : data_( ite.data_ ), current_position_( ite.current_position_ ),
				diff_pointer1_( ite.diff_pointer1_ ), diff_pointer2_( ite.diff_pointer2_ ), diff_boundary_( ite.diff_boundary_ )
	{
	}

	// コピー演算子
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		data_ = ite.data_;
		current_position_ = ite.current_position_;
		diff_pointer1_ = ite.diff_pointer1_;
		diff_pointer2_ = ite.diff_pointer2_;
		diff_boundary_ = ite.diff_boundary_;
		return( *this );
	}

	// 要素のアクセス
	reference operator *()
	{
		size_type j = diff_boundary_ == 0 ? 0 : current_position_ / diff_boundary_;
		size_type i = current_position_ - j * diff_boundary_;
		return( data_[ i * diff_pointer1_ + j * diff_pointer2_ ] );
	}
	reference operator []( Distance dist )
	{
		size_type cpos = current_position_ + dist;
		size_type j = diff_boundary_ == 0 ? 0 : cpos / diff_boundary_
		size_type i = cpos - j * diff_boundary_;
		return( data_[ i * diff_pointer1_ + j * diff_pointer2_ ] );
	}

	// 移動
	mist_iterator2& operator ++( ) // 前置型
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator2 operator ++( int ) // 後置型
	{
		mist_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator2& operator --( ) // 前置型
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator2 operator --( int ) // 後置型
	{
		mist_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_iterator2& operator +=( Distance dist )
	{
		current_position_ += dist;
		return( *this );
	}
	const mist_iterator2& operator -=( Distance dist )
	{
		current_position_ -= dist;
		return( *this );
	}

	// 比較
	bool operator ==( const mist_iterator2 &ite ) const { return( address( ) == ite.address( ) ); }
	bool operator !=( const mist_iterator2 &ite ) const { return( address( ) != ite.address( ) ); }
	bool operator < ( const mist_iterator2 &ite ) const { return( address( ) <  ite.address( ) ); }
	bool operator <=( const mist_iterator2 &ite ) const { return( address( ) <= ite.address( ) ); }
	bool operator > ( const mist_iterator2 &ite ) const { return( address( ) >  ite.address( ) ); }
	bool operator >=( const mist_iterator2 &ite ) const { return( address( ) >= ite.address( ) ); }
};



template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}


// mistコンテナで利用するランダムアクセスイテレータ
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
class mist_reverse_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
{
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef std::size_t size_type;
	typedef Reference reference;

private:
	pointer data_;
	size_type current_position_;
	size_type diff_pointer1_;
	size_type diff_pointer2_;
	size_type diff_boundary_;

private:
	const pointer address( ) const 
	{
		size_type j = diff_boundary_ == 0 ? 0 : current_position_ / diff_boundary_;
		size_type i = current_position_ - j * diff_boundary_;
		return( data_ + i * diff_pointer1_ + j * diff_pointer2_ );
	}

public:
	// コンストラクタ
	mist_reverse_iterator2( pointer p = NULL, size_type diff1 = 1, size_type diff2 = 1, size_type bound = 0 )
				: data_( p ), current_position_( 0 ), diff_pointer1_( diff1 ), diff_pointer2_( diff2 ), diff_boundary_( bound )
	{
	}

	mist_reverse_iterator2( const mist_reverse_iterator2 &ite ) : data_( ite.data_ ), current_position_( ite.current_position_ ),
				diff_pointer1_( ite.diff_pointer1_ ), diff_pointer2_( ite.diff_pointer2_ ), diff_boundary_( ite.diff_boundary_ )
	{
	}

	// コピー演算子
	const mist_reverse_iterator2& operator =( const mist_reverse_iterator2 &ite )
	{
		data_ = ite.data_;
		current_position_ = ite.current_position_;
		diff_pointer1_ = ite.diff_pointer1_;
		diff_pointer2_ = ite.diff_pointer2_;
		diff_boundary_ = ite.diff_boundary_;
		return( *this );
	}

	// 要素のアクセス
	reference operator *()
	{
		size_type j = diff_boundary_ == 0 ? 0 : current_position_ / diff_boundary_;
		size_type i = current_position_ - j * diff_boundary_;
		return( data_[ i * diff_pointer1_ + j * diff_pointer2_ ] );
	}
	reference operator []( Distance dist )
	{
		size_type cpos = current_position_ + dist;
		size_type j = diff_boundary_ == 0 ? 0 : cpos / diff_boundary_
		size_type i = cpos - j * diff_boundary_;
		return( *( data_ - i * diff_pointer1_ + j * diff_pointer2_ ) );
	}

	// 移動
	mist_reverse_iterator2& operator ++( ) // 前置型
	{
		*this += 1;
		return( *this );
	}
	const mist_reverse_iterator2 operator ++( int ) // 後置型
	{
		mist_reverse_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_reverse_iterator2& operator --( ) // 前置型
	{
		*this -= 1;
		return( *this );
	}
	const mist_reverse_iterator2 operator --( int ) // 後置型
	{
		mist_reverse_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_reverse_iterator2& operator +=( Distance dist )
	{
		current_position_ += dist;
		return( *this );
	}
	const mist_reverse_iterator2& operator -=( Distance dist )
	{
		current_position_ -= dist;
		return( *this );
	}

	// 比較
	bool operator ==( const mist_reverse_iterator2 &ite ) const { return( address( ) == ite.address( ) ); }
	bool operator !=( const mist_reverse_iterator2 &ite ) const { return( address( ) != ite.address( ) ); }
	bool operator < ( const mist_reverse_iterator2 &ite ) const { return( address( ) <  ite.address( ) ); }
	bool operator <=( const mist_reverse_iterator2 &ite ) const { return( address( ) <= ite.address( ) ); }
	bool operator > ( const mist_reverse_iterator2 &ite ) const { return( address( ) >  ite.address( ) ); }
	bool operator >=( const mist_reverse_iterator2 &ite ) const { return( address( ) >= ite.address( ) ); }
};


template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator2< T, Distance, Pointer, Reference > operator +( const mist_reverse_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_reverse_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_reverse_iterator2< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator2< T, Distance, Pointer, Reference > operator +( const mist_reverse_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_reverse_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator2< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_reverse_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_reverse_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator2< T, Distance, Pointer, Reference > operator -( const mist_reverse_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_reverse_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_reverse_iterator2< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_reverse_iterator2< T, Distance, Pointer, Reference > operator -( const mist_reverse_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_reverse_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}

// mist名前空間の終わり
_MIST_END


#endif
