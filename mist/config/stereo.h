/// @file mist/config/stereo.h
//!
//! @brief ステレオ音声を扱うためのクラス
//!

#ifndef __INCLUDE_MIST_STEREO__
#define __INCLUDE_MIST_STEREO__


#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

#include <iostream>

// mist名前空間の始まり
_MIST_BEGIN


// MISTで利用する基底のデータ型


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T … 引数の説明
//! 
template< class T >
struct stereo
{
public:
	typedef size_t size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef T& reference;					///< @brief データ型の参照．data の場合，data & となる
	typedef const T& const_reference;		///< @brief データ型の const 参照．data の場合，const data & となる
	typedef T value_type;					///< @brief 内部データ型．T と同じ
	typedef T* pointer;						///< @brief データ型のポインター型．data の場合，data * となる
	typedef const T* const_pointer;			///< @brief データ型の const ポインター型．data の場合，const data * となる

public:
	value_type l;		///< @brief 変数の説明を書く
	value_type r;		///< @brief 変数の説明を書く

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	stereo( ) : l( 0 ), r( 0 ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] mono … 引数の説明
	//! 
	stereo( const value_type &mono ) : l( mono ), r( mono ){ }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] left  … 引数の説明
	//! @param[in] right … 引数の説明
	//! 
	stereo( const value_type &left, const value_type &right ) : l( left ), r( right ){ }



	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator  =( const stereo &s )
	{
		if( &s != this )
		{
			l = s.l;
			r = s.r;
		}
		return( *this );
	}


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] mono … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator  =( const value_type &mono )
	{
		l = mono;
		r = mono;
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
	const stereo &operator +=( const stereo &s ){ l += s.l; r += s.r; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator -=( const stereo &s ){ l -= s.l; r -= s.r; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator *=( const stereo &s ){ l *= s.l; r *= s.r; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator /=( const stereo &s ){ l /= s.l; r /= s.r; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator %=( const stereo &s ){ l %= s.l; r %= s.r; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator |=( const stereo &s ){ l |= s.l; r |= s.r; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator &=( const stereo &s ){ l &= s.l; r &= s.r; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator ^=( const stereo &s ){ l ^= s.l; r ^= s.r; return( *this ); }



	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] mono … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator +=( const value_type &mono ){ l += mono; r += mono; return( +this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] mono … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator -=( const value_type &mono ){ l -= mono; r -= mono; return( -this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] mono … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator *=( const value_type &mono ){ l *= mono; r *= mono; return( *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] mono … 引数の説明
	//! 
	//! @return 戻り値の説明
	//! 
	const stereo &operator /=( const value_type &mono ){ l /= mono; r /= mono; return( *this ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator ==( const stereo &s ) const { return( l == s.l && r == s.r ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator !=( const stereo &s ) const { return( !( *this == s ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator < ( const stereo &s ) const { return( !( *this >= s ) ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator <=( const stereo &s ) const { return( s >= *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator > ( const stereo &s ) const { return( s < *this ); }

	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @param[in] s … 引数の説明
	//! 
	//! @retval true  … 戻り値の説明
	//! @retval false … 戻り値の説明
	//! 
	bool operator >=( const stereo &s ) const { return( l >= s.l && r >= s.r ); }


	/// @brief 関数・クラスの概要を書く
	//! 
	//! 詳細な説明や関数の使用例を書く
	//! 
	//! @return 戻り値の説明
	//! 
	value_type get_value( ) const
	{
		return( static_cast< value_type >( ( l + r ) / 2.0 ) );
	}

	// ステレオからモノラルへの自動キャスト演算子（危険のため一時停止）
	//operator value_type( ) const { return( get_value( ) ); }
};


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator +( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) += c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator -( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) -= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator *( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) *= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator /( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) /= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator %( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) %= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator |( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) |= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator &( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) &= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator ^( const stereo< T > &c1, const stereo< T > &c2 ){ return( stereo< T >( c1 ) ^= c2 ); }




/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator *( const stereo< T > &c1, const typename stereo< T >::value_type &c2 ){ return( stereo< T >( c1 ) *= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator *( const typename stereo< T >::value_type &c1, const stereo< T > &c2 ){ return( stereo< T >( c2 ) *= c1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator /( const stereo< T > &c1, const typename stereo< T >::value_type &c2 ){ return( stereo< T >( c1 ) /= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator +( const stereo< T > &c1, const typename stereo< T >::value_type &c2 ){ return( stereo< T >( c1 ) += c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator +( const typename stereo< T >::value_type &c1, const stereo< T > &c2 ){ return( stereo< T >( c2 ) += c1 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator -( const stereo< T > &c1, const typename stereo< T >::value_type &c2 ){ return( stereo< T >( c1 ) -= c2 ); }

/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] c1 … 引数の説明
//! @param[in] c2 … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline const stereo< T > operator -( const typename stereo< T >::value_type &c1, const stereo< T > &c2 ){ return( stereo< T >( c2 ) -= c1 ); }



/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in,out] out … 引数の説明
//! @param[in]     s   … 引数の説明
//! 
//! @return 戻り値の説明
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const stereo< T > &s )
{
	out << "( ";
	out << s.l << ", ";
	out << s.r << " )";
	return( out );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param T  … 引数の説明
//! 
template < class T >
struct is_stereo
{
	_MIST_CONST( bool, value, false );
};


// 画素の変換をサポートするためのコンバータ
template < class T >
struct _stereo_converter_
{
	typedef T value_type;
	typedef stereo< T > stereo_type;
	enum{ channel_num = 1 };

	static value_type convert_to( value_type l, value_type r )
	{
		return( stereo_type( l, r ).get_value( ) );
	}

	static stereo_type convert_from( const value_type &mono )
	{
		return( stereo_type( mono, mono ) );
	}
};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_STEREO( type ) \
		template < >\
		struct is_stereo< stereo< type > >\
		{\
			enum{ value = true };\
		};\


	#define __STEREO_CONVERTER__( type ) \
		template < >\
		struct _stereo_converter_< stereo< type > >\
		{\
			typedef type value_type;\
			typedef stereo< type > stereo_type;\
			enum{ channel_num = 2 };\
			\
			static stereo_type convert_to( value_type l, value_type r )\
			{\
				return( stereo_type( l, r ) );\
			}\
			\
			static stereo_type convert_from( const stereo_type &sound )\
			{\
				return( sound );\
			}\
		};\

		// 各型に対する特殊化
		IS_STEREO(unsigned char)
		IS_STEREO(unsigned short)
		IS_STEREO(unsigned int)
		IS_STEREO(unsigned long)
		IS_STEREO(signed char)
		IS_STEREO(signed short)
		IS_STEREO(signed int)
		IS_STEREO(signed long)
		IS_STEREO(bool)
		IS_STEREO(char)
		IS_STEREO(float)
		IS_STEREO(double)
		IS_STEREO(long double)
		__STEREO_CONVERTER__(unsigned char)
		__STEREO_CONVERTER__(unsigned short)
		__STEREO_CONVERTER__(unsigned int)
		__STEREO_CONVERTER__(unsigned long)
		__STEREO_CONVERTER__(signed char)
		__STEREO_CONVERTER__(signed short)
		__STEREO_CONVERTER__(signed int)
		__STEREO_CONVERTER__(signed long)
		__STEREO_CONVERTER__(bool)
		__STEREO_CONVERTER__(char)
		__STEREO_CONVERTER__(float)
		__STEREO_CONVERTER__(double)
		__STEREO_CONVERTER__(long double)

		#undef IS_STEREO
		#undef __STEREO_CONVERTER__

#else

	template < class T >
	struct is_stereo< stereo< T > >
	{
		_MIST_CONST( bool, value, true );
	};

	template < class T >
	struct _stereo_converter_< stereo< T > >
	{
		typedef T value_type;
		typedef stereo< T > stereo_type;
		enum{ channel_num = 2 };

		static stereo_type convert_to( value_type l, value_type r )
		{
			return( stereo_type( l, r ) );
		}

		static stereo_type convert_from( const stereo_type &sound )
		{
			return( sound );
		}
	};

#endif

// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_STEREO__
