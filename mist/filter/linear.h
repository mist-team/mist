/// @file mist/filter/linear.h
//!
//! @brief 各次元の画像に対応した線形フィルタを計算するためのライブラリ
//!
//!自分で定義した線形フィルタ用カーネルを各次元の画像に適用します．
//!また，3×3ガウシアンフィルタ，3×3ラプラシアンフィルタを各次元で用意してあります．
//!
//!差分絶対値フィルタのように結果を絶対値で出力するフィルタを扱う場合には，
//!_MIST_LINEAR_FILTER_RETURN_ABSOLUTE_VALUE_ 
//!というマクロを mist/filter/linear.h のインクルードより上に定義して下さい．
//!
//!関数内の計算はすべて double 型で行います．
//!最終的に結果を出力用の配列に格納するときのデータ型のキャストは，
//!static_cast< out_value_type > を用いますが，オーバーフローを抑制したい場合は，
//!_MIST_LINEAR_FILTER_USE_SAFE_CAST_
//!というマクロを mist/filter/linear.h のインクルードより上に定義して下さい．
//!データ型の上限値・下限値を調べ，それを超える場合は，上限値・下限値に置き換えます．
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
	//オーバーフロー抑制
	template< class T >
	struct type_limits_2 : public type_limits < T >
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
	struct type_limits_2< float > : public type_limits < float >
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

	//double型から任意の型への変換（オーバーフローの抑制）
	template < class T >
	inline T safe_cast( const double& arg )
	{
		return( type_limits_2< T >::cast( arg )  );
	}

	//画面端の要素に対する演算
	template < class T_out >
	struct edge_element
	{
		template < class T_in, class Allocator_in >
		static T_out copy_value( const array< T_in, Allocator_in >& in, typename array< T_in, Allocator_in >::size_type i )
		{
			#ifdef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( safe_cast< T_out >( in[ i ] ) );
			#endif

			#ifndef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( static_cast< T_out >( in[ i ] ) );
			#endif
		}
	}; 

	//画面端以外の各要素に対する演算
	template < class T_out >
	struct non_edge_element
	{
		template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
		static T_out get_value( const array< T_in, Allocator_in >& in, const typename array< T_in, Allocator_in >::difference_type *pindex, const array< T_kernel, Allocator_kernel >& kernel, const typename array< T_in, Allocator_in >::size_type ii )
		{
			double	val = 0;

			typedef typename array< T_kernel, Allocator_kernel >::size_type size_type;
		
			typename array< T_in, Allocator_in >::const_pointer p = &in[ ii ];
			for( size_type i = 0 ; i < kernel.size( ) ; i++ )
			{
				val += p[ pindex[ i ] ] * kernel[ i ];
			}

			#ifdef _MIST_LINEAR_FILTER_RETURN_ABSOLUTE_VALUE_
			val = fabs( val );
			#endif

			#ifdef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( safe_cast< T_out >( val ) );
			#endif

			#ifndef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( static_cast< T_out >( val ) );
			#endif
		}
	};

	//カーネル適用
	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out, const array< T_kernel, Allocator_kernel >& kernel,
					const typename array< T_kernel, Allocator_kernel >::size_type kernel_center )
	{
		out.resize( in.size( ) );

		typedef typename array< T_in, Allocator_in >::size_type size_type;
		typedef typename array< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel_center;
		const size_type e_i = in.size( ) - ( kernel.size( ) - kernel_center ) + 1;  

		size_type i, count = 0;
		const_pointer p = &in( in.size( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
				pindex[ count++ ] = &in( in.size( ) / 2 + i - kernel_center ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			 out( i ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, i );
		}

		//画像端の処理
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}
		for( i = e_i ; i < in.size( ) ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out, const array1< T_kernel, Allocator_kernel >& kernel,
					const typename array< T_kernel, Allocator_kernel >::size_type kernel_center_i )
	{
		out.resize( in.size1( ) );
		out.reso1( in.reso1( ) );

		typedef typename array1< T_in, Allocator_in >::size_type size_type;
		typedef typename array1< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array1< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel_center_i;
		const size_type e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;

		typedef array1< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type i, count = 0;
		const_pointer p = &in( in.width( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
				pindex[ count++ ] = &in( in.width( ) / 2 + i - kernel_center_i ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			out( i ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, i );	
		}

		//画像端の処理
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}
		for( i = e_i ; i < in.size1( ) ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out, const array2< T_kernel, Allocator_kernel >& kernel,
					const typename array2< T_kernel, Allocator_kernel >::size_type kernel_center_i, const typename array2< T_kernel, Allocator_kernel >::size_type kernel_center_j ) 
	{	
		std::cout << "apply2" << std::endl;
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		typedef typename array2< T_in, Allocator_in >::size_type size_type;
		typedef typename array2< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array2< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel_center_i;
		const size_type e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type s_j = kernel_center_j;
		const size_type e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;

		typedef array2< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type i, j, count = 0;
		const_pointer p = &in( in.width( ) / 2, in.height( ) / 2 );
		for( j = 0 ; j < kernel.size2( ) ; j++ )
		{
			for( i = 0 ; i < kernel.size1( ) ; i++ )
			{
				pindex[ count++ ] = &in( in.width( ) / 2 + i - kernel_center_i, in.height( ) / 2 + j - kernel_center_j ) - p;
			}
		}

		for( j = s_j ; j < e_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, j * in.size1( ) + i );
			}
		}

		delete [] pindex;

		//画像端の処理
		for( j = 0 ; j < in.size2( ) ; j++ )
		{
			for( i = 0 ; i < s_i ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
			for( i = e_i ; i < in.size1( ) ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
		}
		for( j = 0 ; j < s_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
		}
		for( j = e_j ; j < in.size2( ) ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
		}
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out, const array3< T_kernel, Allocator_kernel >& kernel,
					const typename array3< T_kernel, Allocator_kernel >::size_type kernel_center_i, const typename array3< T_kernel, Allocator_kernel >::size_type kernel_center_j, const typename array3< T_kernel, Allocator_kernel >::size_type kernel_center_k )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
		
		typedef typename array3< T_in, Allocator_in >::size_type size_type;
		typedef typename array3< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array3< T_out, Allocator_out >::value_type out_value_type;
		
		const size_type s_i = kernel_center_i;
		const size_type e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type s_j = kernel_center_j;
		const size_type e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;
		const size_type s_k = kernel_center_k;
		const size_type e_k = in.size3( ) - ( kernel.size3( ) - kernel_center_k ) + 1;

		typedef array3< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type i, j, k, count = 0;
		const_pointer p = &in( in.width( ) / 2, in.height( ) / 2, in.depth( ) / 2 );
		for( k = 0 ; k < kernel.size3( ) ; k++ )
		{
			for( j = 0 ; j < kernel.size2( ) ; j++ )
			{
				for( i = 0 ; i < kernel.size1( ) ; i++ )
				{
					pindex[ count++ ] = &in( in.width( ) / 2 + i - kernel_center_i, in.height( ) / 2 + j - kernel_center_j, in.depth( ) / 2 + k - kernel_center_k ) - p;
				}
			}
		}

		for( k = s_k ; k < e_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, k * in.size1() *in.size2() + j *in.size1() + i );
				}
			}
		}

		delete [] pindex;


		//画像端の処理
		for( k = 0 ; k < in.size3( ) ; k++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				for( i = 0 ; i < s_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
				for( i = e_i ; i < in.size1( ) ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
			for( j = 0 ; j < s_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
			for( j = e_j ; j < in.size2( ) ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
		}
		for( k = 0 ; k < s_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
		}
		for( k = e_k ; k < in.size3() ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
		}
	}

	//カーネルの正規化
	template < class T, class Allocator >
	void normalize( array< T, Allocator >& kernel )
	{
		typedef typename array< T, Allocator >::size_type size_type;
		typedef typename array< T, Allocator >::value_type value_type;
		
		double norm = 0;

		size_type i;
		for( i = 0 ; i < kernel.size( ) ; i++ )
		{
			norm += kernel[ i ];
		}
		for( i = 0 ; i < kernel.size( ) ; i++ )
		{
			kernel[ i ] =  static_cast< value_type >( kernel[ i ] / norm );
		}
	}

}

//! @addtogroup linear_group 線形フィルタ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/linear.h>
//! @endcode
//!
//!  @{


/// @brief 自分で定義したkernelを用いた線形フィルタ(arrayの場合)
//! 
//! 予め，線形フィルタ用のカーネル配列を自分で定義しておき，それを用いたフィルタ処理を行う．
//! 
//! @code 使用例(5×5一様重み平滑化フィルタ)
//! mist::array2< unsigned char > in, out;
//! mist::array2< double > kernel( 5, 5 );
//!
//! 一様重み平滑化フィルタ用のカーネルを作成
//! kernel.fill( 1.0 / kernel.size( ) ); 
//!
//! 適当な入力データ
//! mist::read_bmp( in, "hoge.bmp" ); 
//!
//! フィルタの適用
//! mist::linear_filter( in, out, kernel );
//! @endcode
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  kernel … カーネル配列
//! @param[in]  kernel_center … カーネルの中心位置(何も渡さない場合は kernel.size( ) / 2 が入る)
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out, const array< T_kernel, Allocator_kernel >& kernel, int kernel_center = -1 )
{
	if( kernel_center == -1 )
	{
		kernel_center = kernel.size( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center );
}

/// @brief 自分で定義したkernelを用いた線形フィルタ(array1の場合)
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  kernel … カーネル配列
//! @param[in]  kernel_center_i … カーネルの中心位置(何も渡さない場合は kernel.width( ) / 2 が入る)
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out, const array1< T_kernel, Allocator_kernel > kernel, int kernel_center_i = -1 )
 {
	if( kernel_center_i == -1 )
	{
		kernel_center_i = kernel.size1( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center_i );
}

/// @brief 自分で定義したkernelを用いた線形フィルタ(array2の場合)
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  kernel … カーネル配列
//! @param[in]  kernel_center_i … カーネルの中心位置(何も渡さない場合は kernel.width( ) / 2 が入る)
//! @param[in]  kernel_center_j … カーネルの中心位置(何も渡さない場合は kernel.height( ) / 2 が入る)
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out, const array2< T_kernel, Allocator_kernel >& kernel, int kernel_center_i = -1, int kernel_center_j = -1 )
{
	if( kernel_center_i == -1 )
	{
		kernel_center_i = kernel.size1( ) / 2;
	}
	if( kernel_center_j == -1 )
	{
		kernel_center_j = kernel.size2( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center_i, kernel_center_j );
}

/// @brief 自分で定義したkernelを用いた線形フィルタ(array3の場合)
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  kernel … カーネル配列
//! @param[in]  kernel_center_i … カーネルの中心位置(何も渡さない場合は kernel.width( ) / 2 が入る)
//! @param[in]  kernel_center_j … カーネルの中心位置(何も渡さない場合は kernel.height( ) / 2 が入る)
//! @param[in]  kernel_center_k … カーネルの中心位置(何も渡さない場合は kernel.depth( ) / 2 が入る)
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out, const array3< T_kernel, Allocator_kernel >& kernel, 
						int kernel_center_i = -1, int kernel_center_j = -1, int kernel_center_k = -1 )
{
	if( kernel_center_i == -1 )
	{
		kernel_center_i = kernel.size1( ) / 2;
	}
	if( kernel_center_j == -1 )
	{
		kernel_center_j = kernel.size2( ) / 2;
	}
	if( kernel_center_k == -1 )
	{
		kernel_center_k = kernel.size3( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k );
}

/// @brief ガウシアン(array)
//! 
//! kernelサイズ3の1次元ガウシアン
//!
//! @param[in]  in  … 入力配列
//! @param[out] out … 出力配列
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out )
{
	array< double > kernel( 3 );

	kernel( 0 ) = 0.60653066;
	kernel( 1 ) = 1.0;
	kernel( 2 ) = 0.60653066;

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief ガウシアン(array1)
//! 
//! kernelサイズ3の1次元ガウシアン
//!
//! @param[in]  in  … 入力配列
//! @param[out] out … 出力配列
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out )
{
	array1< double > kernel( 3 );

	kernel( 0 ) = 0.60653066;
	kernel( 1 ) = 1.0;
	kernel( 2 ) = 0.60653066;

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief ガウシアン(array2)
//! 
//! kernelサイズ3×3の2次元ガウシアン
//!
//! @param[in]  in  … 入力配列
//! @param[out] out … 出力配列
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out )
{
	array2< double > kernel( 3, 3 );

	kernel( 0, 0 ) = 0.367879441;
	kernel( 1, 0 ) = 0.60653066;
	kernel( 2, 0 ) = 0.367879441;
	kernel( 0, 1 ) = 0.60653066;
	kernel( 1, 1 ) = 1.0;
	kernel( 2, 1 ) = 0.60653066;
	kernel( 0, 2 ) = 0.367879441;
	kernel( 1, 2 ) = 0.60653066;
	kernel( 2, 2 ) = 0.367879441;

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1, 1 );
}


/// @brief ガウシアン(array3)
//! 
//! kernelサイズ3×3×3の3次元ガウシアン
//!
//! @param[in]  in  … 入力配列
//! @param[out] out … 出力配列
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out )
{
	array3< double > kernel( 3, 3, 3 );

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

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1, 1, 1 );
}


/// @brief ラプラシアン(array)
//! 
//! kernelサイズ3の1次元ラプラシアン
//!
//! 出力配列の要素型を unsigned とする場合は注意が必要．
//! 絶対値出力マクロやオーバーフロー抑制マクロ
//! _MIST_LINEAR_FILTER_RETURN_ABSOLUTE_VALUE_
//! _MIST_LINEAR_FILTER_USE_SAFE_CAST_
//! を併用しない場合，想定する出力が得られない場合があります．
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out )
{
	array< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief ラプラシアン(array1)
//! 
//! kernelサイズ3の1次元ラプラシアン
//!
//! 出力配列の要素型を unsigned とする場合は注意が必要．
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out )
{
	array1< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief ラプラシアン(array2)
//! 
//! kernelサイズ3×3の2次元ラプラシアン
//!
//! 出力配列の要素型を unsigned とする場合は注意が必要．
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out )
{
	array2< double > kernel( 3, 3 );

	kernel( 0, 0 ) = 1.0;
	kernel( 1, 0 ) = 1.0;
	kernel( 2, 0 ) = 1.0;
	kernel( 0, 1 ) = 1.0;
	kernel( 1, 1 ) = -8.0;
	kernel( 2, 1 ) = 1.0;
	kernel( 0, 2 ) = 1.0;
	kernel( 1, 2 ) = 1.0;
	kernel( 2, 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel, 1, 1 );
}


/// @brief ラプラシアン(array3)
//! 
//! kernelサイズ3×3×3の3次元ラプラシアン
//!
//! 出力配列の要素型を unsigned とする場合は注意が必要．
//! 
//! @param[in]  in  … 引数の説明
//! @param[out] out … 引数の説明
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out )
{
	array3< double > kernel( 3, 3, 3 );

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

	__linear_filter__::apply( in, out, kernel, 1, 1, 1 );
}	

/// @}
//  線形グループの終わり


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
