/// @file mist/filter/linear.h
//!
//! @brief 各次元の画像に対応した線形フィルタを計算するためのライブラリ
//!

#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN

namespace __linear_filter__
{
	//オーバーフロー制御
	template< class T >
	struct type_limits_2 : public mist::type_limits < T >
	{
		static T cast( const double& arg )
		{
			if( ( arg >= minimum( ) ) && ( arg <= maximum( ) ) )
			{
				if( arg >= 0 )
				{
					return( static_cast< T >( arg + 0.5 ) );
				}
				else
				{
					return( static_cast< T >( arg - 0.5 ) );
				}
			}
			else
			if( arg < minimum( ) )
			{
				return( minimum( ) );
			}
			else //if( arg > maximum( ) )
			{
				return( maximum( ) );
			}
		}
	};

	template < >
	struct type_limits_2< float > : public mist::type_limits < float >
	{
		static float cast( const float& arg )
		{
			if( ( arg >= -maximum( ) ) && ( arg <= maximum( ) ) )
			{
				return( static_cast< float >( arg ) );
			}
			if( arg < -maximum( ) )
			{
				return( -maximum( ) );
			}
			if( arg > maximum( ) )
			{
				return( maximum( ) );
			}
		}
	};

	template< >
	struct type_limits_2< double >
	{
		static double cast( const double& arg )
		{
			return( arg );
		}
	};

	template< >
	struct type_limits_2< long double >
	{
		static long double cast( const double& arg )
		{
			return( arg );
		}
	};

	//double型から任意の型への変換
	template < class T >
	inline T cast_to( const double& arg )
	{
		return( type_limits_2< T >::cast( arg )  );
	}

	//画面端の処理
	template < class T_in, class Allocator_in >
	inline double copy_value( const mist::array< T_in, Allocator_in >& in, typename mist::array< T_in, Allocator_in >::size_type &i )
	{
		return( in( i ) );
	} 

	template < class T_in, class Allocator_in >
	inline double copy_value( const mist::array1< T_in, Allocator_in >& in, typename mist::array1< T_in, Allocator_in >::size_type &i )
	{
		return( in( i ) );
	}

	template < class T_in, class Allocator_in >
	inline double copy_value( const mist::array2< T_in, Allocator_in >& in, const typename mist::array2< T_in, Allocator_in >::size_type &i, 
							const typename mist::array2< T_in, Allocator_in >::size_type &j )
	{
		return( in( i, j ) );
	}

	template < class T_in, class Allocator_in >
	inline double copy_value( const mist::array3< T_in, Allocator_in >& in, const typename mist::array3< T_in, Allocator_in >::size_type &i,
							const typename mist::array3< T_in, Allocator_in >::size_type &j, const typename mist::array3< T_in, Allocator_in >::size_type &k )
	{
		return( in( i, j, k ) );
	}

	//各要素に対する演算
	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array< T_in, Allocator_in > &in, const mist::array< T_kernel, Allocator_kernel > &kernel, const typename mist::array< T_kernel, Allocator_kernel >::size_type &i )
	{	
		double	val = 0;

		typedef typename mist::array< T_kernel, Allocator_kernel >::size_type size_type; 

		size_type ii;
		for( ii = 0 ; ii < kernel.size( ) ; ii++ )
		{
			val += in( i - kernel.size( ) / 2 + ii ) * kernel( ii );
		}
		return( val );
	}

	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array1< T_in, Allocator_in > &in, const mist::array1< T_kernel, Allocator_kernel > &kernel, const typename mist::array1< T_kernel, Allocator_kernel >::size_type &i )
	{
		double	val = 0;

		typedef typename mist::array1< T_kernel, Allocator_kernel >::size_type size_type;
		
		size_type ii;
		for( ii = 0 ; ii < kernel.size1( ) ; ii++ )
		{
			val += in( i - kernel.size1( ) / 2 + ii ) * kernel( ii );
		}
		return( val );
	}

	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array2< T_in, Allocator_in > &in, const mist::array2< T_kernel, Allocator_kernel > &kernel,
								const typename mist::array2< T_kernel, Allocator_kernel >::size_type &i, const typename mist::array2< T_kernel, Allocator_kernel >::size_type &j )
	{
		double	val = 0;
		
		typedef typename mist::array2< T_kernel, Allocator_kernel >::size_type size_type;
		
		size_type ii, jj;
		for( jj = 0 ; jj < kernel.size2( ) ; jj++ )
		{
			for( ii = 0 ; ii < kernel.size1( ) ; ii++ )
			{
				val += in( i - kernel.size1( ) / 2 + ii, j - kernel.size2( ) / 2 + jj ) * kernel( ii, jj );
			}
		}
		return( val );
	}

	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array3< T_in, Allocator_in >& in, const mist::array3< T_kernel,Allocator_kernel >& kernel,
							const typename mist::array3< T_kernel, Allocator_kernel >::size_type &i, const typename mist::array3< T_kernel, Allocator_kernel >::size_type &j, const typename mist::array3< T_kernel, Allocator_kernel >::size_type &k )
	{
		double	val = 0;

		typedef typename mist::array3< T_kernel, Allocator_kernel >::size_type size_type;
		
		size_type ii, jj, kk;
		for( kk = 0 ; kk < kernel.size3( ) ; kk++ )
		{
			for( jj = 0 ; jj < kernel.size2( ) ; jj++ )
			{
				for( ii = 0 ; ii < kernel.size1( ) ; ii++ )
				{
					val += in( i - kernel.size1( ) / 2 + ii, j - kernel.size2( ) / 2 + jj, k - kernel.size3( ) / 2 + kk ) * kernel( ii, jj, kk );
				}
			}
		}
		return( val );
	}

	//カーネル適用
	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out, const mist::array< T_kernel, Allocator_kernel >& kernel )
	{
		out.resize( in.size( ) );

		typedef typename mist::array< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel.size( ) / 2;
		const size_type e_i = in.size( ) - (kernel.size( ) - 1) / 2;  

		size_type i;
		for( i = s_i ; i < e_i ; i++ )
		{
			out( i ) = cast_to< out_value_type >( get_value( in, kernel, i ) );	
		}

		//画像端の処理
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = cast_to< out_value_type >( copy_value( in, i ) );	
		}
		for( i = e_i ; i < in.size( ) ; i++ )
		{
			out( i ) = cast_to< out_value_type >( copy_value( in, i ) );	
		}		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out, const mist::array1< T_kernel, Allocator_kernel >& kernel )
	{
		out.resize( in.size1( ) );
		out.reso1( in.reso1( ) );

		typedef typename mist::array1< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array1< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel.size1( ) / 2;
		const size_type e_i = in.size1( ) - (kernel.size1( ) - 1) / 2;

		size_type i;
		for( i = s_i ; i < e_i ; i++ )
		{
			 out( i ) = cast_to< out_value_type >( get_value( in, kernel, i ) );	
		}

		//画像端の処理
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = cast_to< out_value_type >( copy_value( in, i ) );	
		}
		for( i = e_i ; i < in.size1( ) ; i++ )
		{
			out( i ) = cast_to< out_value_type >( copy_value( in, i ) );	
		}		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out, const mist::array2< T_kernel, Allocator_kernel >& kernel )
	{	
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		typedef typename mist::array2< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array2< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel.size1( ) / 2;
		const size_type e_i = in.size1( ) - (kernel.size1( ) - 1) / 2;
		const size_type s_j = kernel.size2( ) / 2;
		const size_type e_j = in.size2( ) - (kernel.size2( ) - 1) / 2;

		size_type i, j;
		for( j = s_j ; j < e_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = cast_to< out_value_type >( get_value( in, kernel, i, j ) );
			}
		}

		//画像端の処理
		for( j = 0 ; j < in.size2( ) ; j++ )
		{
			for( i = 0 ; i < s_i ; i++ )
			{
				out( i, j ) = cast_to< out_value_type >( copy_value( in, i, j ) );
			}
			for( i = e_i ; i < in.size1( ) ; i++ )
			{
				out( i, j ) = cast_to< out_value_type >( copy_value( in, i, j ) );
			}
		}
		for( j = 0 ; j < s_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = cast_to< out_value_type >( copy_value( in, i, j ) );
			}
		}
		for( j = e_j ; j < in.size2( ) ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = cast_to< out_value_type >( copy_value( in, i, j ) );
			}
		}
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out, const mist::array3< T_kernel, Allocator_kernel >& kernel )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
		
		typedef typename mist::array3< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array3< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel.size1( ) / 2;
		const size_type e_i = in.size1( ) - (kernel.size1( ) - 1) / 2;
		const size_type s_j = kernel.size2( ) / 2;
		const size_type e_j = in.size2( ) - (kernel.size2( ) - 1) / 2;
		const size_type s_k = kernel.size3( ) / 2;
		const size_type e_k = in.size3( ) - (kernel.size3( ) - 1) / 2;

		size_type i, j, k;
		for( k = s_k ; k < e_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( get_value( in, kernel, i, j, k ) );
				}
			}
		}

		//画像端の処理
		for( k = 0 ; k < in.size3( ) ; k++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				for( i = 0 ; i < s_i ; i++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( copy_value( in, i, j, k ) );
				}
				for( i = e_i ; i < in.size1( ) ; i++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( copy_value( in, i, j, k ) );
				}
			}
			for( j = 0 ; j < s_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( copy_value( in, i, j, k ) );
				}
			}
			for( j = e_j ; j < in.size2( ) ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( copy_value( in, i, j, k ) );
				}
			}
		}
		for( k = 0 ; k < s_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( copy_value( in, i, j, k ) );
				}
			}
		}
		for( k = e_k ; k < in.size3() ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( copy_value( in, i, j, k ) );
				}
			}
		}
	}

	//カーネルの正規化
	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out )
	{
		typedef typename mist::array< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array< T_out, Allocator_out >::value_type out_value_type;
		
		double norm = 0;

		size_type i;
		for( i = 0 ; i < in.size( ) ; i++ )
		{
			norm += in( i );
		}

		out.resize( in.size( ) );

		for( i = 0 ; i < out.size( ) ; i++ )
		{
			out( i ) =  cast_to< out_value_type >( in( i ) / norm );
		}
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out )
	{
		typedef typename mist::array1< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array1< T_out, Allocator_out >::value_type out_value_type;
		
		double norm = 0;

		size_type i;
		for( i = 0 ; i < in.size1( ) ; i++ )
		{
			norm += in( i );
		}

		out.resize( in.size1( ) );
		out.reso1( in.reso1( ) );

		for( i = 0 ; i < out.size1( ) ; i++ )
		{
			out( i ) =  cast_to< out_value_type >( in( i ) / norm );
		}
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out )
	{
		typedef typename mist::array2< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array2< T_out, Allocator_out >::value_type out_value_type;
		
		double norm = 0;

		size_type i, j;
		for( i = 0 ; i < in.size1( ) ; i++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				norm += in( i, j );
			}
		}

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		for( i = 0 ; i < out.size1( ) ; i++ )
		{
			for( j = 0 ; j < out.size2() ; j++ )
			{
				out( i, j ) = cast_to< out_value_type >( in( i, j ) / norm );
			}
		}
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out )
	{
		typedef typename mist::array3< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array3< T_out, Allocator_out >::value_type out_value_type;
		
		double norm = 0;

		size_type i, j, k;
		for( i = 0 ; i < in.size1( ) ; i++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				for( k = 0 ; k < in.size3( ) ; k++ )
				{
					norm += in( i, j, k );
				}
			}
		}

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		for( i = 0 ; i < out.size1() ; i++ )
		{
			for( j = 0 ; j < out.size2() ; j++ )
			{
				for( k = 0 ; k < out.size3() ; k++ )
				{
					out( i, j, k ) = cast_to< out_value_type >( in( i, j, k ) / norm );
				}
			}
		}
	}
}


//! @addtogroup linear_group 線形フィルタ
//!  @{


/// @brief kernelを渡す場合
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out, const mist::array< T_kernel, Allocator_kernel >& kernel )
{
	__linear_filter__::apply( in, out, kernel );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out, const mist::array1< T_kernel, Allocator_kernel >& kernel )
{
	__linear_filter__::apply( in, out, kernel );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out, const mist::array2< T_kernel, Allocator_kernel >& kernel )
{
	__linear_filter__::apply( in, out, kernel );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out, const mist::array3< T_kernel, Allocator_kernel >& kernel )
{
	__linear_filter__::apply( in, out, kernel );
}


/// @brief ガウシアン
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out )
{
	mist::array< double > kernel( 3 );

	kernel( 0 ) = 0.60653066;
	kernel( 1 ) = 1.0;
	kernel( 2 ) = 0.60653066;

	mist::array1< double >	n_kernel;
	__linear_filter__::normalize( kernel, n_kernel );

	__linear_filter__::apply( in, out, n_kernel );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out )
{
	mist::array1< double > kernel( 3 );

	kernel( 0 ) = 0.60653066;
	kernel( 1 ) = 1.0;
	kernel( 2 ) = 0.60653066;

	mist::array1< double >	n_kernel;
	__linear_filter__::normalize( kernel, n_kernel );

	__linear_filter__::apply( in, out, n_kernel );
}


/// @brief ガウシアン
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out )
{
	mist::array2< double > kernel( 3, 3 );

	kernel( 0, 0 ) = 0.367879441;
	kernel( 1, 0 ) = 0.60653066;
	kernel( 2, 0 ) = 0.367879441;
	kernel( 0, 1 ) = 0.60653066;
	kernel( 1, 1 ) = 1.0;
	kernel( 2, 1 ) = 0.60653066;
	kernel( 0, 2 ) = 0.367879441;
	kernel( 1, 2 ) = 0.60653066;
	kernel( 2, 2 ) = 0.367879441;

	mist::array2< double >	n_kernel;
	__linear_filter__::normalize( kernel, n_kernel );

	__linear_filter__::apply( in, out, n_kernel );
}


/// @brief ガウシアン
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out )
{
	mist::array3< double > kernel( 3, 3, 3 );

	kernel( 0, 0, 0 ) = 0.22313016;
	kernel( 1, 0, 0 ) = 0.367879441;
	kernel( 2, 0, 0 ) = 0.22313016;
	kernel( 0, 1, 0 ) = 0.367879441;
	kernel( 1, 1, 0 ) = 0.60653066;
	kernel( 2, 1, 0 ) = 0.367879441;
	kernel( 0, 2, 0 ) = 0.22313016;
	kernel( 1, 2, 0 ) = 0.367879441;
	kernel( 2, 2, 0 ) = 0.22313016;

	kernel( 0, 0, 1 ) = 0.367879441;
	kernel( 1, 0, 1 ) = 0.60653066;
	kernel( 2, 0, 1 ) = 0.367879441;
	kernel( 0, 1, 1 ) = 0.60653066;
	kernel( 1, 1, 1 ) = 1.0;
	kernel( 2, 1, 1 ) = 0.60653066;
	kernel( 0, 2, 1 ) = 0.367879441;
	kernel( 1, 2, 1 ) = 0.60653066;
	kernel( 2, 2, 1 ) = 0.367879441;

	kernel( 0, 0, 2 ) = 0.22313016;
	kernel( 1, 0, 2 ) = 0.367879441;
	kernel( 2, 0, 2 ) = 0.22313016;
	kernel( 0, 1, 2 ) = 0.367879441;
	kernel( 1, 1, 2 ) = 0.60653066;
	kernel( 2, 1, 2 ) = 0.367879441;
	kernel( 0, 2, 2 ) = 0.22313016;
	kernel( 1, 2, 2 ) = 0.367879441;
	kernel( 2, 2, 2 ) = 0.22313016;

	mist::array3< double >	n_kernel;
	__linear_filter__::normalize( kernel, n_kernel );

	__linear_filter__::apply( in, out, n_kernel );
}


/// @brief ラプラシアン
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out )
{
	mist::array< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel );
}


/// @brief 関数・クラスの概要を書く
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out )
{
	mist::array1< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel );
}


/// @brief ラプラシアン
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out )
{
	mist::array2< double > kernel( 3, 3 );

	kernel( 0, 0 ) = 1.0;
	kernel( 1, 0 ) = 1.0;
	kernel( 2, 0 ) = 1.0;
	kernel( 0, 1 ) = 1.0;
	kernel( 1, 1 ) = -8.0;
	kernel( 2, 1 ) = 1.0;
	kernel( 0, 2 ) = 1.0;
	kernel( 1, 2 ) = 1.0;
	kernel( 2, 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel );
}


/// @brief ラプラシアン
//! 
//! 詳細な説明や関数の使用例を書く
//! 
//! @param[in] in  … 引数の説明
//! @param[in] out … 引数の説明
//! @return        … 戻り値の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out )
{
	mist::array3< double > kernel( 3, 3, 3 );

	kernel( 0, 0, 0 ) = 1.0;
	kernel( 1, 0, 0 ) = 1.0;
	kernel( 2, 0, 0 ) = 1.0;
	kernel( 0, 1, 0 ) = 1.0;
	kernel( 1, 1, 0 ) = 1.0;
	kernel( 2, 1, 0 ) = 1.0;
	kernel( 0, 2, 0 ) = 1.0;
	kernel( 1, 2, 0 ) = 1.0;
	kernel( 2, 2, 0 ) = 1.0;

	kernel( 0, 0, 1 ) = 1.0;
	kernel( 1, 0, 1 ) = 1.0;
	kernel( 2, 0, 1 ) = 1.0;
	kernel( 0, 1, 1 ) = 1.0;
	kernel( 1, 1, 1 ) = -26.0;
	kernel( 2, 1, 1 ) = 1.0;
	kernel( 0, 2, 1 ) = 1.0;
	kernel( 1, 2, 1 ) = 1.0;
	kernel( 2, 2, 1 ) = 1.0;

	kernel( 0, 0, 2 ) = 1.0;
	kernel( 1, 0, 2 ) = 1.0;
	kernel( 2, 0, 2 ) = 1.0;
	kernel( 0, 1, 2 ) = 1.0;
	kernel( 1, 1, 2 ) = 1.0;
	kernel( 2, 1, 2 ) = 1.0;
	kernel( 0, 2, 2 ) = 1.0;
	kernel( 1, 2, 2 ) = 1.0;
	kernel( 2, 2, 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel );
}	

/// @}
//  線形グループの終わり


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
