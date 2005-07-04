/// @file mist/filter/linear.h
//!
//! @brief 線形フィルタのためのライブラリ
//!



#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__



#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <algorithm>
#include <cmath>
#include <mist/config/color.h>



// mist名前空間の始まり
_MIST_BEGIN



namespace __linear__
{

	//
	// カーネル配列を受け取って関数オブジェクトを作る
	//
	template< typename In, typename Out, typename Kernel_array >
	class function_type
	{
		Kernel_array ka_;
		array< int > pd_;

	public:

		function_type( const Kernel_array &ka, const size_t o ) : ka_( ka )
		{
			// std::cout << "function_type_1" << std::endl;
			pd_.resize( ka_.size( ) );
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{
				pd_[ i ] = i - o;
			}
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t w ) : ka_( ka )
		{
			// std::cout << "function_type_2" << std::endl;
			array2< size_t > pd_2( ka_.width( ), ka_.height( ) );
			for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
			{
				for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
				{
					pd_2( i, j ) = ( j - oj ) * w + i - oi;
				}
			}
			pd_ = pd_2;
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t ok, const size_t w, const size_t h ) : ka_( ka )
		{
			// std::cout << "function_type_3" << std::endl;
			array3< size_t > pd_3( ka_.width( ), ka_.height( ), ka_.depth( ) );
			for( size_t k = 0 ; k < ka_.depth( ) ; k ++ )
			{
				for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
				{
					for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
					{
						pd_3( i, j, k ) = ( k - ok ) * w * h + ( j - oj ) * w + i - oi;
					}
				}
			}
			pd_ = pd_3;
		}

		const Out operator ( )( const In &v ) const
		{
			const In * const pv = &v;
			double res = 0.0;
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{ 
				res += *( pv + pd_[ i ] ) * ka_[ i ];
			}
			return ( static_cast< Out >( res ) );
		}

	};



	//
	// カーネル配列を受け取って関数オブジェクトを作る（mist::rgb用）
	//
	template < typename In, typename Out, typename Kernel_array >
	class function_type< In, rgb< Out >, Kernel_array >
	{
		Kernel_array ka_;
		array< int > pd_;

	public:

		function_type( const Kernel_array &ka, const size_t o ) : ka_( ka )
		{
			// std::cout << "function_type_1" << std::endl;
			pd_.resize( ka_.size( ) );
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{
				pd_[ i ] = i - o;
			}
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t w ) : ka_( ka )
		{
			// std::cout << "function_type_2" << std::endl;
			array2< size_t > pd_2( ka_.width( ), ka_.height( ) );
			for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
			{
				for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
				{
					pd_2( i, j ) = ( j - oj ) * w + i - oi;
				}
			}
			pd_ = pd_2;
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t ok, const size_t w, const size_t h ) : ka_( ka )
		{
			// std::cout << "function_type_3" << std::endl;
			array3< size_t > pd_3( ka_.width( ), ka_.height( ), ka_.depth( ) );
			for( size_t k = 0 ; k < ka_.depth( ) ; k ++ )
			{
				for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
				{
					for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
					{
						pd_3( i, j, k ) = ( k - ok ) * w * h + ( j - oj ) * w + i - oi;
					}
				}
			}
			pd_ = pd_3;
		}

		const rgb< Out > operator ( )( const In &v ) const
		{
			const In * const pv = &v;
			rgb< double > res = rgb< double >( );
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{ 
				res += *( pv + pd_[ i ] ) * ka_[ i ];
			}
			return ( static_cast< rgb< Out > >( res ) );
		}

	};


	template< typename In, typename Out, typename Func >
	class abs
	{
		Func f_;
	public:
		abs( const Func &f ) : f_( f )
		{
			// std::cout << "abs" << std::endl;
		}
		const Out operator ( )( const In &v ) const
		{
			return static_cast< Out >( std::fabs( f_( v ) ) );
		}
	};

	template< typename In, typename Out, typename Func >
	class abs< In, rgb< Out >, Func >
	{
		Func f_;
	public:
		abs( const Func &f ) : f_( f )
		{
			// std::cout << "abs" << std::endl;
		}
		const rgb< Out > operator ( )( const In &v ) const
		{
			const rgb< Out > res = f_( v );
			return ( rgb< Out >( static_cast< Out >( std::fabs( static_cast< double >( res.r ) ) ) , static_cast< Out >( std::fabs( static_cast< double >( res.g ) ) ) , static_cast< Out >( std::fabs( static_cast< double >( res.b ) ) ) ) );
		}
	};



	inline void uniw( array< double > &a, const size_t size )
	{
		// std::cout << "uniw_0" << std::endl;
		a.resize( size );
		a.fill( 1.0 / a.size( ) );
	}

	inline void uniw_1( array1< double > &a, const size_t size )
	{
		// std::cout << "uniw_1" << std::endl;
		a.resize( size );
		a.fill( 1.0 / a.size( ) );
	}

	inline void uniw_2( array2< double > &a, const size_t size )
	{
		// std::cout << "uniw_2" << std::endl;
		a.resize( size, size );
		a.fill( 1.0 / a.size( ) );
	}

	inline void uniw_3( array3< double > &a, const size_t size )
	{
		// std::cout << "uniw_3" << std::endl;
		a.resize( size, size, size );
		a.fill( 1.0 / a.size( ) );
	}



	template< typename Array >
	inline void normalize( Array &a )
	{
		double nrm  = 0;
		for( size_t i = 0 ; i < a.size( ) ; i ++ )
		{
			nrm += a[ i ];
		}
		for( size_t i = 0 ; i < a.size( ) ; i ++ )
		{
			a[ i ] /= nrm;
		}
	}


	inline void gaus( array< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_0" << std::endl;
		a.resize( size );
		const int o = size / 2;
		for( size_t i = 0 ; i < size ; i ++ )
		{
			const int ii = static_cast< int >( i );
			a[ i ] = std::exp( -( ( ii - o ) * ( ii - o ) ) / ( 2 * sigma * sigma ) );
		}
		normalize( a );
	}

	inline void gaus_1( array1< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_1" << std::endl;
		a.resize( size );
		const int o = size / 2;
		for( size_t i = 0 ; i < size ; i ++ )
		{
			const int ii = static_cast< int >( i );
			a[ i ] = std::exp( -( ( ii - o ) * ( ii - o ) ) / ( 2 * sigma * sigma ) );
		}
		normalize( a );
	}

	inline void gaus_2( array2< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_2" << std::endl;
		a.resize( size, size );
		const int o = size / 2;
		for( size_t j = 0 ; j < size ; j ++ )
		{
			for( size_t i = 0 ; i < size ; i ++ )
			{
				const int ii = static_cast< int >( i );
				const int jj = static_cast< int >( j );
				a( i, j ) = std::exp( -( ( ii - o ) * ( ii - o ) + ( jj - o ) * ( jj - o ) ) / ( 2 * sigma * sigma ) );
			}
		}
		normalize( a );
	}

	inline void gaus_3( array3< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_3" << std::endl;
		a.resize( size, size, size );
		const int o = size / 2;
		for( size_t k = 0 ; k < size ; k ++ )
		{
			for( size_t j = 0 ; j < size ; j ++ )
			{
				for( size_t i = 0 ; i < size ; i ++ )
				{
					const int ii = static_cast< int >( i );
					const int jj = static_cast< int >( j );
					const int kk = static_cast< int >( k );
					a( i, j, k ) = std::exp( -( ( ii - o ) * ( ii - o ) + ( jj - o ) * ( jj - o ) + ( kk - o ) * ( kk - o ) ) / ( 2 * sigma * sigma ) );
				}
			}
		}
		normalize( a );
	}

	

	inline void lapl( array< double > &a )
	{
		// std::cout << "lapl_0" << std::endl;
		a.resize( 3 );
		a[ 0 ] = 1.0;  a[ 1 ] = -2.0;  a[ 2 ] = 1.0;
	}

	inline void lapl_1( array1< double > &a )
	{
		// std::cout << "lapl_1" << std::endl;
		a.resize( 3 );
		a[ 0 ] = 1.0;  a[ 1 ] = -2.0;  a[ 2 ] = 1.0;
	}

	inline void lapl_2( array2< double > &a )
	{
		// std::cout << "lapl_2" << std::endl;
		a.resize( 3, 3 );
		a( 0, 0 ) = 1.0;  a( 1, 0 ) = 1.0;  a( 2, 0 ) = 1.0;
		a( 0, 1 ) = 1.0;  a( 1, 1 ) = -8.0;  a( 2, 1 ) = 1.0;
		a( 0, 2 ) = 1.0;  a( 1, 2 ) = 1.0;  a( 2, 2 ) = 1.0;
	}

	inline void lapl_3( array3< double > &a )
	{
		// std::cout << "lapl_3" << std::endl;
		a.resize( 3, 3, 3 );
		a( 0, 0, 0 ) = 1.0;  a( 1, 0, 0 ) = 1.0;  a( 2, 0, 0 ) = 1.0;
		a( 0, 1, 0 ) = 1.0;  a( 1, 1, 0 ) = 1.0;  a( 2, 1, 0 ) = 1.0;
		a( 0, 2, 0 ) = 1.0;  a( 1, 2, 0 ) = 1.0;  a( 2, 2, 0 ) = 1.0;
		a( 0, 0, 1 ) = 1.0;  a( 1, 0, 1 ) = 1.0;  a( 2, 0, 1 ) = 1.0;
		a( 0, 1, 1 ) = 1.0;  a( 1, 1, 1 ) = -26.0;  a( 2, 1, 1 ) = 1.0;
		a( 0, 2, 1 ) = 1.0;  a( 1, 2, 1 ) = 1.0;  a( 2, 2, 1 ) = 1.0;
		a( 0, 0, 2 ) = 1.0;  a( 1, 0, 2 ) = 1.0;  a( 2, 0, 2 ) = 1.0;
		a( 0, 1, 2 ) = 1.0;  a( 1, 1, 2 ) = 1.0;  a( 2, 1, 2 ) = 1.0;
		a( 0, 2, 2 ) = 1.0;  a( 1, 2, 2 ) = 1.0;  a( 2, 2, 2 ) = 1.0;
	}

}



// 
// std::transformのインタフェースを変えたもの
//
template< typename In_array, typename Out_array, typename Func >
inline void filtering(
				   const In_array &in,
				   Out_array &out,
				   const Func func,
				   const size_t begin,
				   const size_t end )
{
	std::transform( &in[ begin ], &in[ end ], &out[ begin ], func );
}



//! @addtogroup linear_group 線形フィルタ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/linear.h>
//! @endcode
//!
//! 配列の端（カーネルがはみ出すところ）の要素の計算は行いません．
//!
//! 使用可能な要素型は，算術型か mist::rgb< 算術型 > に限ります．
//! 内部の計算は，出力配列の要素型が算術型の場合は double 型，
//! mist::rgb< 算術型 > の場合は mist::rgb< double > で行います．
//!
//! 自分で定義した線形フィルタ用カーネルを各次元の画像に適用できます．
//! 
//! @code 使用例(5×5一様重み平滑化フィルタ)
//! mist::array2< unsigned char > in, out;
//!
//! mist::read_bmp( in, "hoge.bmp" );  // 適当な入力配列 
//!
//! mist::array2< double > kernel( 5, 5 ); // 適当なカーネル配列
//! kernel.fill( 1.0 / kernel.size( ) );  // 5×5一様重み平滑化フィルタ用のカーネルを作成
//!
//! mist::linear_filter( in, out, kernel, 2, 2 );  // フィルタの適用（カーネル配列とカーネル中心を渡します）
//! @endcode
//!
//! カーネルサイズ3（1次元），3×3（2次元），3×3×3（3次元）に特化した線形フィルタを用意してあります．
//!
//! @code 使用例(5×5一様重み平滑化フィルタ)
//! mist::array2< unsigned char > in, out;
//!
//! mist::read_bmp( in, "hoge.bmp" );  // 適当な入力配列 
//!
//! mist::array2< double > size_3_kernel( 3, 3 ); // 適当なカーネル配列
//! size_3_kernel.fill( 1.0 / size_3_kernel.size( ) );  // 3×3一様重み平滑化フィルタ用のカーネルを作成
//!
//! mist::size_3_filter( in, out, size_3_kernel );  // フィルタの適用（カーネル配列とカーネル中心を渡します）
//! @endcode
//!
//! 一様重み平滑化フィルタ，ガウシアンフィルタ，ラプラシアンフィルタ（絶対値）を array, array1, array2, array3 で用意してあります．
//!
//!  @{



/// @brief 一般の線形フィルタ( array )
//! 
//! カーネル配列とその中心位置を指定する
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  kernel …カーネル配列
//! @param[in]  offset …カーネル中心位置
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool linear_filter(
				   const array< In_value, In_alloc > &in,
				   array< Out_value, Out_alloc > &out,
				   const array< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset )
{
	// std::cout << "linear_0" << std::endl;
	typedef array< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.size( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset );
	const size_t begin = offset;
	const size_t end = in.size( ) - ( kernel.size( ) - offset - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
}

/// @brief 一般の線形フィルタ( array1 )
//! 
//! カーネル配列とその中心位置を指定する
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  kernel …カーネル配列
//! @param[in]  offset …カーネル中心位置
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool linear_filter(
				   const array1< In_value, In_alloc > &in,
				   array1< Out_value, Out_alloc > &out,
				   const array1< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset )
{
	// std::cout << "linear_1" << std::endl;
	typedef array1< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.width( ) );
	out.reso1( in.reso1( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset );
	const size_t begin = offset;
	const size_t end = in.size( ) - ( kernel.width( ) - offset - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
}

/// @brief 一般の線形フィルタ( array2 )
//! 
//! カーネル配列とその中心位置を指定する
//!
//! @param[in]  in       … 入力配列
//! @param[out] out      … 出力配列
//! @param[in]  kernel   …カーネル配列
//! @param[in]  offset_i …i方向のカーネル中心位置
//! @param[in]  offset_j …j方向のカーネル中心位置
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool linear_filter(
				   const array2< In_value, In_alloc > &in,
				   array2< Out_value, Out_alloc > &out,
				   const array2< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset_i,
				   const size_t offset_j )
{
	// std::cout << "linear_2" << std::endl;
	typedef array2< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.width( ), in.height( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset_i, offset_j, in.width( ) );
	const size_t begin = offset_j * in.width( ) + offset_i;
	const size_t end = in.size( ) - ( ( kernel.height( ) - offset_j - 1 ) * in.width( ) + kernel.width( ) - offset_i - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
}

/// @brief 一般の線形フィルタ( array2 )
//! 
//! カーネル配列とその中心位置を指定する
//!
//! @param[in]  in       … 入力配列
//! @param[out] out      … 出力配列
//! @param[in]  kernel   …カーネル配列
//! @param[in]  offset_i …i方向のカーネル中心位置
//! @param[in]  offset_j …j方向のカーネル中心位置
//! @param[in]  offset_j …k方向のカーネル中心位置
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool linear_filter(
				   const array3< In_value, In_alloc > &in,
				   array3< Out_value, Out_alloc > &out,
				   const array3< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset_i,
				   const size_t offset_j,
				   const size_t offset_k )
{
	// std::cout << "linear_3" << std::endl;
	typedef array3< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.width( ), in.height( ), in.depth( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset_i, offset_j, offset_k, in.width( ), in.height( ) );
	const size_t begin = offset_k * in.height( ) * in.width( ) + offset_j * in.width( ) + offset_i;
	const size_t end = in.size( ) - ( ( kernel.depth( ) - offset_k - 1 ) * in.height( ) * in.width( ) + ( kernel.height( ) - offset_j - 1 ) * in.width( ) + kernel.width( ) - offset_i - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
}



/// @brief サイズ3の線形フィルタ( array, array1, array2, array3 )
//! 
//! サイズ3(1次元)，3×3(2次元)，3×3×3(3次元)に特化した線形フィルタ
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  kernel …カーネル配列
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array< In_value, In_alloc > &in,
				   array< Out_value, Out_alloc > &out,
				   const array< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_0" << std::endl;
	if( kernel.width( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array1< In_value, In_alloc > &in,
				   array1< Out_value, Out_alloc > &out,
				   const array1< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_1" << std::endl;
	if( kernel.width( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array2< In_value, In_alloc > &in,
				   array2< Out_value, Out_alloc > &out,
				   const array2< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_2" << std::endl;
	if( kernel.width( ) - 3 || kernel.height( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array3< In_value, In_alloc > &in,
				   array3< Out_value, Out_alloc > &out,
				   const array3< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_3" << std::endl;
	if( kernel.width( ) - 3 || kernel.height( ) - 3 || kernel.depth( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1, 1, 1 ) );
}


/// @brief 一様重み平滑化( array, array1, array2, array3 )
//! 
//! サイズ指定可能な一様重み平滑化
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  size    … フィルタのサイズ（デフォルト 3）
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_0" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array< double > kernel;
	__linear__::uniw( kernel, _size );
	return ( linear_filter( in, out, kernel, offset ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_1" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array1< double > kernel;
	__linear__::uniw_1( kernel, _size );
	return ( linear_filter( in, out, kernel, offset ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_2" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array2< double > kernel;
	__linear__::uniw_2( kernel, _size );
	return ( linear_filter( in, out, kernel, offset, offset ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_3" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array3< double > kernel;
	__linear__::uniw_3( kernel, _size );
	return ( linear_filter( in, out, kernel, offset, offset, offset ) );
}


/// @brief ガウシアン( array, array1, array2, array3 )
//! 
//! 範囲と強度を指定可能なガウシアン
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//! @param[in]  range  … ガウシアンの範囲（デフォルト 1）
//! @param[out] sigma  … ガウシアンの強度（デフォルト 1.0）
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_0" << std::endl;
	array< double > kernel;
	__linear__::gaus( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_1" << std::endl;
	array1< double > kernel;
	__linear__::gaus_1( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_2" << std::endl;
	array2< double > kernel;
	__linear__::gaus_2( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range, range ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_3" << std::endl;
	array3< double > kernel;
	__linear__::gaus_3( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range, range, range ) );
}



/// @brief ラプラシアン( array, array1, array2, array3 )
//! 
//! サイズ3のラプラシアン
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_0" << std::endl;
	array< double > kernel;
	__linear__::lapl( kernel );
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_1" << std::endl;
	array1< double > kernel;
	__linear__::lapl_1( kernel );
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_2" << std::endl;
	array2< double > kernel;
	__linear__::lapl_2( kernel );
	return ( linear_filter( in, out, kernel, 1, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_3" << std::endl;
	array3< double > kernel;
	__linear__::lapl_3( kernel );
	return ( linear_filter( in, out, kernel, 1, 1, 1 ) );
}



/// @brief 絶対値を返すラプラシアン( array, array1, array2, array3 )
//! 
//! サイズ3の絶対値を返すラプラシアン
//!
//! @param[in]  in     … 入力配列
//! @param[out] out    … 出力配列
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array< double > > Function_type;
	// std::cout << "lapl_abs_0" << std::endl;
	out.resize( in.size( ) );
	array< double > kernel;
	__linear__::lapl( kernel );
	const Function_type func( kernel, 1 );
	const size_t begin = 1;
	const size_t end = in.size( ) - 1; 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array1< double > > Function_type;
	// std::cout << "lapl_abs_1" << std::endl;
	out.resize( in.width( ) );
	out.reso1( in.reso1( ) );
	array1< double > kernel;
	__linear__::lapl_1( kernel );
	const __linear__::function_type< In_value, Out_value, array1< double > > func( kernel, 1 );
	const size_t begin = 1;
	const size_t end = in.size( ) - 1; 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array2< double > > Function_type; 
	// std::cout << "lapl_abs_2" << std::endl;
	out.resize( in.width( ), in.height( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	array2< double > kernel;
	__linear__::lapl_2( kernel );
	const Function_type func( kernel, 1, 1, in.width( ) );
	const size_t begin = in.width( ) + 1;
	const size_t end = in.size( ) - ( in.width( ) + 1 ); 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array3< double > > Function_type;
	// std::cout << "lapl_abs_3" << std::endl;
	out.resize( in.width( ), in.height( ), in.depth( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	array3< double > kernel;
	__linear__::lapl_3( kernel );
	const Function_type func( kernel, 1, 1, 1, in.width( ), in.height( ) );
	const size_t begin = in.height( ) * in.width( ) + in.width( ) + 1;
	const size_t end = in.size( ) - ( in.height( ) * in.width( ) + in.width( ) + 1 ); 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}



/// @}
//  線形グループの終わり



// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
