/// @file mist/filter/linear.h
//!
//! @brief 各次元の画像に対応した線形フィルタを計算するためのライブラリ
//!

#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "../config/type_trait.h"
#endif

#include <cmath>
#include <functional>

// mist名前空間の始まり
_MIST_BEGIN


namespace __linear_filter__
{
	
	//////
	////// 配列の端以外の各要素に対する計算
	//////
	template < class Calc_type, class Out_type, class Array_in, class Array_kernel, class Post_func >
	inline Out_type  calc_non_edge( 
										const Array_in								&in, 
										const typename Array_in::difference_type	*pindex, 
										const Array_kernel							&kernel, 
										const typename Array_in::size_type			ii,
										Post_func									p_func )
	{
		typedef typename Array_in::size_type size_type;
		
		Calc_type  ret = Calc_type( );
		typename Array_in::const_pointer p = &in[ ii ];
		for( size_type i = 0 ; i < kernel.size( ) ; i++ )
		{
			ret += p[ pindex[ i ] ] * kernel[ i ];
		}

		return ( p_func( ret ) );
	}		


	//////
	////// 配列の端の要素に対する計算
	//////
	template < class Out_type, class Array_in, class Post_func >
	inline Out_type  calc_edge( 
									const Array_in						&in, 
									const typename Array_in::size_type	i,
									Post_func							p_func )
	{
		return ( p_func( in[ i ]  ) );
	}
	
	
	//////
	////// カーネル適用
	//////

	// カーネル適用 ( array )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	inline void  apply( 
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							const array< T_kernel, Allocator_kernel >	&kernel,
							const unsigned int							kernel_center,
							const Post_func								p_func )
	{
		out.resize( in.size( ) );

		typedef typename array< T_in, Allocator_in >::size_type			size_type;
		typedef typename array< T_in, Allocator_in >::difference_type	difference_type;
		typedef typename array< T_out, Allocator_out >::value_type		out_type;
		typedef typename Calc_type										calc_type;

		const  size_type s_i = kernel_center;
		const  size_type e_i = in.size( ) - ( kernel.size( ) - kernel_center ) + 1;  

		typedef array< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type		i, count = 0;
		const_pointer	p = &in( in.size( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
			pindex[ count++ ] = &in( in.size( ) / 2 + i - kernel_center ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			out( i ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, i, p_func );
		}

		// 端の処理
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}
		for( i = e_i ; i < in.size( ) ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}		
	}

	// カーネル適用 ( array1 )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	inline void  apply(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							const array1< T_kernel, Allocator_kernel >	&kernel,
							const unsigned int							kernel_center_i,
							const Post_func								p_func )
	{
		out.resize( in.size1( ) );
		out.reso1( in.reso1( ) );

		typedef typename array1< T_in, Allocator_in >::size_type		size_type;
		typedef typename array1< T_in, Allocator_in >::difference_type	difference_type;
		typedef typename array1< T_out, Allocator_out >::value_type		out_type;
		typedef typename Calc_type										calc_type;

		const size_type  s_i = kernel_center_i;
		const size_type  e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;

		typedef array1< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type		i, count = 0;
		const_pointer	p = &in( in.width( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
			pindex[ count++ ] = &in( in.size1( ) / 2 + i - kernel_center_i ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			out( i ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, i, p_func );	
		}

		// 端の処理
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}
		for( i = e_i ; i < in.size1( ) ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}		
	}

	// カーネル適用 ( array2 )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	inline void apply( 
						const array2< T_in, Allocator_in >			&in, 
						array2< T_out, Allocator_out >				&out, 
						const array2< T_kernel, Allocator_kernel >	&kernel,
						const unsigned int							kernel_center_i,
						const unsigned int							kernel_center_j,
						Post_func									p_func ) 
	{
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		typedef typename array2< T_in, Allocator_in >::size_type			size_type;
		typedef typename array2< T_in, Allocator_in >::difference_type		difference_type;
		typedef typename array2< T_out, Allocator_out >::value_type			out_type;
		typedef typename Calc_type											calc_type;

		const size_type  s_i = kernel_center_i;
		const size_type  e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type  s_j = kernel_center_j;
		const size_type  e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;

		typedef array2< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type	   i, j, count = 0;
		const_pointer  p = &in( in.width( ) / 2, in.height( ) / 2 );
		for( j = 0 ; j < kernel.size2( ) ; j++ )
		{
			for( i = 0 ; i < kernel.size1( ) ; i++ )
			{
				pindex[ count++ ] = &in( in.size1( ) / 2 + i - kernel_center_i, in.size2( ) / 2 + j - kernel_center_j ) - p;
			}
		}

		for( j = s_j ; j < e_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, j * in.size1( ) + i, p_func );
			}
		}

		delete [] pindex;

		// 端の処理
		for( j = 0 ; j < in.size2( ) ; j++ )
		{
			for( i = 0 ; i < s_i ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
			for( i = e_i ; i < in.size1( ) ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
		}
		for( j = 0 ; j < s_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
		}
		for( j = e_j ; j < in.size2( ) ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
		}
	}

	// カーネル適用 ( array3 )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	void apply( 
					const array3< T_in, Allocator_in >			&in, 
					array3< T_out, Allocator_out >				&out, 
					const array3< T_kernel, Allocator_kernel >	&kernel,
					const unsigned int							kernel_center_i,
					const unsigned int							kernel_center_j,
					const unsigned int							kernel_center_k,
					const Post_func								p_func )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
		
		typedef typename array3< T_in, Allocator_in >::size_type		size_type;
		typedef typename array3< T_in, Allocator_in >::difference_type	difference_type;
		typedef typename array3< T_out, Allocator_out >::value_type		out_type;
		typedef typename Calc_type										calc_type;
		
		const size_type  s_i = kernel_center_i;
		const size_type  e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type  s_j = kernel_center_j;
		const size_type  e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;
		const size_type  s_k = kernel_center_k;
		const size_type  e_k = in.size3( ) - ( kernel.size3( ) - kernel_center_k ) + 1;

		typedef array3< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type		i, j, k, count = 0;
		const_pointer	p = &in( in.width( ) / 2, in.height( ) / 2, in.depth( ) / 2 );
		for( k = 0 ; k < kernel.size3( ) ; k++ )
		{
			for( j = 0 ; j < kernel.size2( ) ; j++ )
			{
				for( i = 0 ; i < kernel.size1( ) ; i++ )
				{
					pindex[ count++ ] = &in( in.size1( ) / 2 + i - kernel_center_i, in.size2( ) / 2 + j - kernel_center_j, in.size3( ) / 2 + k - kernel_center_k ) - p;
				}
			}
		}

		for( k = s_k ; k < e_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, k * in.size1( ) *in.size2( ) + j *in.size1( ) + i, p_func );
				}
			}
		}

		delete [] pindex;


		// 端の処理
		for( k = 0 ; k < in.size3( ) ; k++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				for( i = 0 ; i < s_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
				for( i = e_i ; i < in.size1( ) ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
			for( j = 0 ; j < s_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
			for( j = e_j ; j < in.size2( ) ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
		}
		for( k = 0 ; k < s_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
		}
		for( k = e_k ; k < in.size3() ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
		}
	}


	//////
	////// 出力配列の要素型が算術型なら，内部の計算は double 型で行う．それ以外( mist::rgb< > 等 )の場合は，出力配列の要素型で計算する
	//////

	// 算術型なので double 型で計算する 
	template < bool Is_arithm = true >
	struct _is_arithm
	{
		template < class Out_type >
		struct _type
		{
			typedef double  calc_type;
		};
	};

	// 算術型ではないので出力配列の要素型で計算する
	template <>
	struct _is_arithm< false >
	{
		template < class Out_type >
		struct _type
		{
			typedef Out_type  calc_type;
		};
	};


	//////
	////// デフォルト後処理関数オブジェクトを生成
	//////
	template < class Calc_type, class Out_type >
	struct default_func : public std::unary_function< Calc_type, Out_type > 
	{
		result_type  operator( )( argument_type  v )
		{
			return ( static_cast< result_type >( v ) );
		}
	};


	//////
	////// 指定された単項関数オブジェクトから後処理関数オブジェクトを生成
	//////

	// STLの unary_function を継承した関数オブジェクトを使用するとき
	template < class Calc_type, class Out_type, class Unary_func >
	struct post_func1 : public std::unary_function< Calc_type, Out_type >
	{
		typedef typename Unary_func					unary_func;
		typedef typename unary_func::argument_type	unary_arg;
		
		unary_func  u_func_;

		post_func1( unary_func  u_func ) : u_func_( u_func )
		{
		}

		result_type  operator( )( argument_type  v )
		{
			return ( static_cast< result_type >( u_func_( static_cast< unary_arg >( v ) ) ) );
		}
	};

	// 通常の関数を使用するとき
	template < class Calc_type, class Out_type, class Unary_arg, class Unary_res >
	struct post_func2 : public std::unary_function< Calc_type, Out_type >
	{
		typedef typename Unary_res	unary_func( Unary_arg );
		typedef typename Unary_arg	unary_arg;
		
		unary_func  *u_func_;

		post_func2( unary_func  *u_func ) : u_func_( u_func )
		{
		}

		result_type  operator( )( argument_type  v ) 
		{
			return ( static_cast< result_type >( ( *u_func_ )( static_cast< unary_arg >( v ) ) ) );
		}
	};


	//////
	////// 配列の正規化 ( array, array1, array2, array3 )
	//////
	template < class Array >
	inline void normalize( Array  &ary )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;
		
		double  norm = 0;

		size_type  i;
		for( i = 0 ; i < ary.size( ) ; i++ )
		{
			norm += ary[ i ];
		}
		for( i = 0 ; i < ary.size( ) ; i++ )
		{
			ary[ i ] =  static_cast< value_type >( ary[ i ] / norm );
		}
	}


	//////
    ////// MIST側で予め用意してあるフィルターのカーネル生成のための識別子
	//////
	struct filter_style
	{
		enum styles
		{
			user_defined,
			gaus,
			lapl
		};
	};


	//////
	////// MIST側で予め用意してあるフィルターに関するカーネル生成関数
	//////

	// 該当なし
	template< filter_style::styles  Style = filter_style::user_defined >
	struct pre_defined_kernel
	{
	};

	// 3×3ガウシアンカーネル関数
	template < >
	struct pre_defined_kernel< filter_style::gaus >
	{
		// gaussian ( array )
		inline static void  gen_func( array< double >  &kernel, unsigned int  &center ) 
		{	
			center = 1;

			kernel.resize( 3 );	

			kernel( 0 ) = 0.60653066;
			kernel( 1 ) = 1.0;
			kernel( 2 ) = 0.60653066;

			normalize( kernel );
		}

		// gaussian ( array1 )
		inline static void  gen_func( array1< double >  &kernel, unsigned int  &center_i )
		{
			center_i = 0;

			kernel.resize( 3 );	

			kernel( 0 ) = 0.60653066;
			kernel( 1 ) = 1.0;
			kernel( 2 ) = 0.60653066;

			normalize( kernel );
		}

		// gaussian ( array2 ) 
		inline static void  gen_func( array2< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j )
		{
			center_i = 1;
			center_j = 1;

			kernel.resize( 3, 3 );	

			kernel( 0, 0 ) = 0.367879441;
			kernel( 1, 0 ) = 0.60653066;
			kernel( 2, 0 ) = 0.367879441;
			kernel( 0, 1 ) = 0.60653066;
			kernel( 1, 1 ) = 1.0;
			kernel( 2, 1 ) = 0.60653066;
			kernel( 0, 2 ) = 0.367879441;
			kernel( 1, 2 ) = 0.60653066;
			kernel( 2, 2 ) = 0.367879441;	

			normalize( kernel );
		}

		// gaussian ( array3 )
		inline static void  gen_func( array3< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j, unsigned int  &center_k )
		{
			center_i = 1;
			center_j = 1;
			center_k = 1;

			kernel.resize( 3, 3, 3 );	

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

			normalize( kernel );
		}
	};

	// 3×3ラプラシアンカーネル関数
	template < >
	struct pre_defined_kernel< filter_style::lapl >
	{
		// laplacian ( array )
		inline static void  gen_func( array< double >  &kernel, unsigned int  &center )
		{
			center = 1;

			kernel.resize( 3 );

			kernel( 0 ) = 1.0;
			kernel( 1 ) = -2.0;
			kernel( 2 ) = 1.0;
		}

		// laplacian ( array1 ) 
		inline static void  gen_func( array1< double >  &kernel, unsigned int  &center_i )
		{
			center_i = 1;

			kernel.resize( 3 );		

			kernel( 0 ) = 1.0;
			kernel( 1 ) = -2.0;
			kernel( 2 ) = 1.0;
		}

		// laplacian ( array2 ) 
		inline static void  gen_func( array2< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j )
		{
			center_i = 1;
			center_j = 1;

			kernel.resize( 3, 3 );		

			kernel( 0, 0 ) = 1.0;
			kernel( 1, 0 ) = 1.0;
			kernel( 2, 0 ) = 1.0;
			kernel( 0, 1 ) = 1.0;
			kernel( 1, 1 ) = -8.0;
			kernel( 2, 1 ) = 1.0;
			kernel( 0, 2 ) = 1.0;
			kernel( 1, 2 ) = 1.0;
			kernel( 2, 2 ) = 1.0;	
		}

		// laplacian ( array3 ) 
		inline static void  gen_func( array3< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j, unsigned int  &center_k  )
		{
			center_i = 1;
			center_j = 1;
			center_k = 1;

			kernel.resize( 3, 3, 3 );

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
		}
	};


	//////
	////// MIST側で予め用意してあるフィルター適用
	//////

	// array
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array< T_in, Allocator_in >	&in, 
											array< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array< double >		kernel;
		unsigned int		kernel_center;	

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center );

		apply< Calc_type >( in, out, kernel, kernel_center, p_func );
	}

	// array1
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array1< T_in, Allocator_in >	&in, 
											array1< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array1< double >	kernel;
		unsigned int		kernel_center_i;

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center_i );

		apply< Calc_type >( in, out, kernel, kernel_center_i, p_func );
	}

	// array2
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array2< T_in, Allocator_in >	&in, 
											array2< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array2< double >	kernel;
		unsigned int		kernel_center_i;
		unsigned int		kernel_center_j;

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center_i, kernel_center_j );

		apply< Calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, p_func );
	}

	// array3
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array3< T_in, Allocator_in >	&in, 
											array3< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array3< double >	kernel;
		unsigned int		kernel_center_i;
		unsigned int		kernel_center_j;
		unsigned int		kernel_center_k;

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center_i, kernel_center_j, kernel_center_k );

		apply< Calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, p_func );
	}

}

//! @addtogroup linear_group 線形フィルタ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/linear.h>
//! @endcode
//!
//! 自分で定義した線形フィルタ用カーネルを各次元の画像に適用します．
//! 
//! @code 使用例(5×5一様重み平滑化フィルタ)
//! mist::array2< unsigned char > in, out;
//!
//! mist::read_bmp( in, "hoge.bmp" );  // 適当な入力データ 
//!
//! mist::array2< double > kernel( 5, 5 );
//! kernel.fill( 1.0 / kernel.size( ) );  // 5×5一様重み平滑化フィルタ用のカーネルを作成
//!
//! mist::linear_filter( in, out, kernel );  // フィルタの適用
//! @endcode
//!
//! また，3×3ガウシアンフィルタ，3×3ラプラシアンフィルタを array, array1, array2, array3 で用意してあります．
//!
//! 内部の計算は，出力配列の要素型が算術型の場合は double 型，
//! それ以外（ mist::rgb<> 等 ）の場合は出力配列の要素型で行います．
//!
//! 各関数の引数として適当な単項関数(引数が一つの関数)，
//! またはSTLの単項関数オブジェクト( std::unary_function から派生させた関数オブジェクト)を渡すことにより，
//! 要素の計算が終了して出力配列に格納する直前にはさむ後処理を指定することができます．
//!
//! @code 後処理関数の例1：差分絶対値フィルタの各要素の計算値の絶対値を出力配列に格納したい
//! mist::laplacian( in, out, std::fabsl );
//! @endcode
//!
//! @code 後処理関数の例2：フィルタの出力を unsigned char のレンジに納めたい 
//! unsigned char  func( const double  v )  // v を unsigned char に切り詰める単項関数
//! {
//!     return ( ( v > 255 ) ? 255 : ( ( v < 0 ) ? 0 : static_cast< unsigned char >( val ) ) );
//! }
//!
//!     // 中略 //
//! 
//! mist::array2< unsigned char >  in, out;
//! 
//!     // 適当な 入力配列 in を作成 //
//!
//! mist::array2< double >  kernel;
//!
//!     // 適当な kernel を作成 //
//! 
//! mist::linear_filter( in, out, func, kernel );  // 単項関数 func を引数として渡す
//! @endcode
//!
//! @code 後処理関数の例3：ガウシアンをかけた後，2値化（STLの関数オブジェクトを使用）
//! #include <functional> // std::unary_function<> を用いるため
//! 
//! struct th_func : std::unary_function< double, unsigned char >
//!     // augument_type を double ，result_type を unsigned char とする関数オブジェクト 
//! {
//!     argument_type  th_;  // 2値化閾値
//!     
//!     th_func( const argument_type  th ) : th_( th )  // コンストラクタ
//!     {
//!     }
//!     
//!     result_type  operator( )( const argument_type  v ) const  
//!         // v が th_ より小さければ 0 を，th_ 以上ならば 255 を出力する
//!     {
//!         return ( ( v < th_ ) ? 0 : 255 );
//!     }
//! };
//!
//!     // 中略 //
//! 
//! mist::gaussian( in, out, th_func( 127.5 ) ); 
//!     // 各要素に対するガウシアンの計算結果を閾値 127.5 で2値化した結果を出力配列に格納
//! @endcode
//!
//!
//!  @{

/// @brief 自分で定義したkernelを用いた線形フィルタ( array )
//! 
//! 予め，線形フィルタ用のカーネル配列を自分で定義しておき，それを用いたフィルタ処理を行う．
//! カーネルの中心位置は，中央( kernel.size( ) / 2 )に設定される
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  u_func … 後処理関数 ( 無くても良い )
//! @param[in]  kernel … カーネル配列
//!
// 通常の関数を渡す場合
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	const unsigned int	kernel_center = kernel.size( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// 関数オブジェクトを渡す場合
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	const unsigned int	kernel_center = kernel.size( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// 何も渡さない場合
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							const array< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	const unsigned int	kernel_center = kernel.size( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief 自分で定義したkernelを用いた線形フィルタ( array1 )
//! 
//! 予め，線形フィルタ用のカーネル配列を自分で定義しておき，それを用いたフィルタ処理を行う．
//! カーネルの中心位置は，中央( kernel.width( ) / 2 )に設定される
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  u_func … 後処理関数 ( 無くても良い )
//! @param[in]  kernel … カーネル配列
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array1< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array1< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							const array1< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief 自分で定義したkernelを用いた線形フィルタ( array2 )
//! 
//! 予め，線形フィルタ用のカーネル配列を自分で定義しておき，それを用いたフィルタ処理を行う．
//! カーネルの中心位置は，中央( kernel.width( ) / 2, kernel.height( ) / 2 )に設定される
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  u_func … 後処理関数 ( 無くても良い )
//! @param[in]  kernel … カーネル配列
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array2< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array2< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							const array2< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::default_func< calc_type, out_type >( ) );
}
	

/// @brief 自分で定義したkernelを用いた線形フィルタ( array3 )
//! 
//! 予め，線形フィルタ用のカーネル配列を自分で定義しておき，それを用いたフィルタ処理を行う．
//! カーネルの中心位置は，中央( kernel.width( ) / 2, kernel.height( ) / 2, kernel.depth( ) / 2 )に設定される
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  u_func … 後処理関数 ( 無くても良い )
//! @param[in]  kernel … カーネル配列
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array3< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;
	const unsigned int	kernel_center_k = kernel.depth( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array3< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;
	const unsigned int	kernel_center_k = kernel.depth( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							const array3< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;
	const unsigned int	kernel_center_k = kernel.depth( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief 自分で定義したkernelを用いた線形フィルタ( array : カーネルの中心位置を指定可能 )
//! 
//! @param[in]  in              … 入力配列
//! @param[out] out             … 出力配列
//! @param[in]  u_func          … 後処理関数 ( 無くても良い )
//! @param[in]  kernel          … カーネル配列
//! @param[in]  kernel_center_i … カーネルの中心位置
//!
// 通常の関数を渡す場合
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	kernel_center = ( kernel_center < kernel.size( ) ) ? kernel_center : ( kernel.size( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// 関数オブジェクトを渡す場合
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	kernel_center = ( kernel_center < kernel.size( ) ) ? kernel_center : ( kernel.size( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// 何も渡さない場合
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							const array< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	kernel_center = ( kernel_center < kernel.size( ) ) ? kernel_center : ( kernel.size( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief 自分で定義したkernelを用いた線形フィルタ( array1 : カーネルの中心位置を指定可能 )
//! 
//! @param[in]  in              … 入力配列
//! @param[out] out             … 出力配列
//! @param[in]  u_func          … 後処理関数 ( 無くても良い )
//! @param[in]  kernel          … カーネル配列
//! @param[in]  kernel_center_i … カーネルの中心位置
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array1< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	kernel_center_i = ( kernel_center_i < kernel.width( ) ) ? kernel_center_i : ( kernel.width( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array1< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																						unary_func;
 
	kernel_center_i = ( kernel_center_i < kernel.width( ) ) ? kernel_center_i : ( kernel.width( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							const array1< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	kernel_center_i = ( kernel_center_i < kernel.width( ) ) ? kernel_center_i : ( kernel.width( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief 自分で定義したkernelを用いた線形フィルタ( array2 : カーネルの中心位置を指定可能 )
//! 
//! @param[in]  in              … 入力配列
//! @param[out] out             … 出力配列
//! @param[in]  u_func          … 後処理関数 ( 無くても良い )
//! @param[in]  kernel          … カーネル配列
//! @param[in]  kernel_center_i … カーネルの中心位置( width 方向 )
//! @param[in]  kernel_center_j … カーネルの中心位置( height 方向 )
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array2< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array2< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							const array2< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::default_func< calc_type, out_type >( ) );
}
	

/// @brief 自分で定義したkernelを用いた線形フィルタ( array3 : カーネルの中心位置を指定可能 )
//! 
//! @param[in]  in              … 入力配列
//! @param[out] out             … 出力配列
//! @param[in]  u_func          … 後処理関数 ( 無くても良い )
//! @param[in]  kernel          … カーネル配列
//! @param[in]  kernel_center_i … カーネルの中心位置( width 方向 )
//! @param[in]  kernel_center_j … カーネルの中心位置( height 方向 )
//! @param[in]  kernel_center_k … カーネルの中心位置( depth 方向 )
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array3< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int							kernel_center_i,
							unsigned int							kernel_center_j, 
							unsigned int							kernel_center_k )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );
	kernel_center_k = ( kernel_center_k < kernel.depth( )  ) ? kernel_center_k : ( kernel.depth( )  - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array3< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int							kernel_center_i,
							unsigned int							kernel_center_j, 
							unsigned int							kernel_center_k )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );
	kernel_center_k = ( kernel_center_k < kernel.depth( )  ) ? kernel_center_k : ( kernel.depth( )  - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							const array3< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j,
							unsigned int								kernel_center_k )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief ガウシアン( array, array1, array2, array3 )
//! 
//! kernelサイズ3×3のガウシアン
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  u_func … 後処理関数 ( 無くても良い )
//! 
// 通常の関数を渡す場合
template < class Array_in, class Array_out, class Unary_arg, class Unary_res >
inline void  gaussian(
						const Array_in								&in,
						Array_out									&out,
						Unary_res									u_func( Unary_arg ) )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	__linear_filter__::apply_pre_defined_kernel< filter_style::gaus, calc_type >( in, out, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// 関数オブジェクトを渡す場合
template < class Array_in, class Array_out, class Unary_func >
inline void  gaussian(
						const Array_in								&in,
						Array_out									&out,
						Unary_func									u_func )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	__linear_filter__::apply_pre_defined_kernel< filter_style::gaus, calc_type >( in, out, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// 何も渡さない場合
template < class Array_in, class Array_out >
inline void  gaussian(
							const Array_in		&in,
							Array_out			&out )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	__linear_filter__::apply_pre_defined_kernel< filter_style::gaus, calc_type >( in, out, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief ラプラシアン( array, array1, array2, array3 )
//! 
//! kernelサイズ3×3のラプラシアン
//!
//! 出力配列の要素型を unsigned とする場合は注意が必要．
//! 適切な u_func を定義しない場合，想定する出力が得られない場合があります．
//! 
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  u_func … 後処理関数 ( 無くても良い )
//!
// 通常の関数を渡す場合
template < class Array_in, class Array_out, class Unary_arg, class Unary_res >
inline void  laplacian(
						const Array_in		&in,
						Array_out			&out,
						Unary_res			u_func( Unary_arg ) )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	__linear_filter__::apply_pre_defined_kernel< filter_style::lapl, calc_type >( in, out, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// 関数オブジェクトを渡す場合
template < class Array_in, class Array_out, class Unary_func >
inline void  laplacian(
						const Array_in		&in,
						Array_out			&out,
						Unary_func			u_func )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	__linear_filter__::apply_pre_defined_kernel< filter_style::lapl, calc_type >( in, out, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// 何も渡さない場合
template < class Array_in, class Array_out >
inline void  laplacian(
							const Array_in		&in,
							Array_out			&out )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;

	__linear_filter__::apply_pre_defined_kernel< filter_style::lapl, calc_type >( in, out, __linear_filter__::default_func< calc_type, out_type >( ) );
}

/// @}
//  線形グループの終わり


// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
