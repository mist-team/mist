// 
// Copyright (c) 2003-2007, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/filter/linear.h
//!
//! @brief 線形フィルタのためのライブラリ
//!



#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__



#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "../config/type_trait.h"
#endif

#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif


#include <cmath>


// mist名前空間の始まり
_MIST_BEGIN


// メディアンフィルタ
namespace __linear__
{
	template < class T >
	struct __promote_pixel_converter_
	{
		typedef T value_type;
		typedef double promote_type;

		static promote_type convert_to( const value_type &pixel )
		{
			return( pixel );
		}

		static value_type convert_from( const promote_type &pixel )
		{
			return( static_cast< value_type >( pixel ) );
		}
	};

	template < class T >
	struct __promote_pixel_converter_< rgb< T > >
	{
		typedef rgb< T > value_type;
		typedef rgb< double > promote_type;

		static promote_type convert_to( const value_type &pixel )
		{
			return( promote_type( pixel.r, pixel.g, pixel.b ) );
		}

		static value_type convert_from( const promote_type &pixel )
		{
			return( value_type( static_cast< T >( pixel.r ), static_cast< T >( pixel.g ), static_cast< T >( pixel.b ) ) );
		}
	};

	template < class T >
	struct __promote_pixel_converter_< bgr< T > >
	{
		typedef bgr< T > value_type;
		typedef bgr< double > promote_type;

		static promote_type convert_to( const value_type &pixel )
		{
			return( promote_type( pixel.r, pixel.g, pixel.b ) );
		}

		static value_type convert_from( const promote_type &pixel )
		{
			return( value_type( static_cast< T >( pixel.r ), static_cast< T >( pixel.g ), static_cast< T >( pixel.b ) ) );
		}
	};

	template < class T >
	struct __promote_pixel_converter_< rgba< T > >
	{
		typedef rgba< T > value_type;
		typedef rgba< double > promote_type;

		static promote_type convert_to( const value_type &pixel )
		{
			return( promote_type( pixel.r, pixel.g, pixel.b ) );
		}

		static value_type convert_from( const promote_type &pixel )
		{
			return( value_type( static_cast< T >( pixel.r ), static_cast< T >( pixel.g ), static_cast< T >( pixel.b ) ) );
		}
	};

	template < class T >
	struct __promote_pixel_converter_< bgra< T > >
	{
		typedef bgra< T > value_type;
		typedef bgra< double > promote_type;

		static promote_type convert_to( const value_type &pixel )
		{
			return( promote_type( pixel.r, pixel.g, pixel.b ) );
		}

		static value_type convert_from( const promote_type &pixel )
		{
			return( value_type( static_cast< T >( pixel.r ), static_cast< T >( pixel.g ), static_cast< T >( pixel.b ) ) );
		}
	};


	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh, fd : フィルタサイズ
	template < class Array1, class Array2, class Kernel, class Functor >
	void linear_filter( const Array1 &in, Array2 &out, const Kernel &kernel,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::difference_type  difference_type;
		typedef __promote_pixel_converter_< typename Array1::value_type > promote_pixel_converter;
		typedef typename promote_pixel_converter::promote_type promote_type;
		typedef typename Array2::value_type out_value_type;
		typedef typename Array1::const_pointer ipointer_type;
		typedef typename Array2::pointer opointer_type;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		difference_type fw = kernel.width( );
		difference_type fh = kernel.height( );
		difference_type fd = kernel.depth( );

		const bool bprogress1 = thread_idy == 0 && d == 1;
		const bool bprogress2 = thread_idz == 0 && d > 1;

		difference_type rw = fw / 2;
		difference_type rh = fh / 2;
		difference_type rd = fd / 2;

		difference_type i, j, k;

		size_type fsize = fw * fh * fd;
		difference_type *pf = new difference_type[ fsize ];

		{
			difference_type count = 0;
			difference_type cx = w / 2;
			difference_type cy = h / 2;
			difference_type cz = d / 2;
			ipointer_type pc = &in( cx, cy, cz );

			for( difference_type z = 0 ; z < fd ; z++ )
			{
				for( difference_type y = 0 ; y < fh ; y++ )
				{
					for( difference_type x = 0 ; x < fw ; x++ )
					{
						pf[ count++ ] = &in( x + cx - rw, y + cy - rh, z + cz - rd ) - pc;
					}
				}
			}
		}


		for( k = thread_idz ; k < rd ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				ipointer_type ip = &in( 0, j, k );
				opointer_type op = &out( 0, j, k );
				for( i = 0 ; i < w ; i++ )
				{
					op[ i ] = ip[ i ];
				}
			}
		}

		for( ; k + rd < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < rh ; j += thread_numy )
			{
				ipointer_type ip = &in( 0, j, k );
				opointer_type op = &out( 0, j, k );
				for( i = 0 ; i < w ; i++ )
				{
					op[ i ] = ip[ i ];
				}
			}

			for( ; j + rh < h ; j += thread_numy )
			{
				ipointer_type ip = &in( 0, j, k );
				opointer_type op = &out( 0, j, k );
				for( i = 0 ; i < rw ; i++ )
				{
					op[ i ] = ip[ i ];
				}

				for( ; i + rw < w ; i++ )
				{
					ipointer_type p = ip + i;
					promote_type value = promote_type( );

					for( size_type n = 0 ; n < fsize ; n++ )
					{
						value += kernel[ n ] * p[ pf[ n ] ];
					}

					op[ i ] = promote_pixel_converter::convert_from( value );
				}

				for( ; i < w ; i++ )
				{
					op[ i ] = ip[ i ];
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 );
				}
			}

			for( ; j < h ; j += thread_numy )
			{
				ipointer_type ip = &in( 0, j, k );
				opointer_type op = &out( 0, j, k );
				for( i = 0 ; i < w ; i++ )
				{
					op[ i ] = ip[ i ];
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 );
			}
		}

		for( ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				ipointer_type ip = &in( 0, j, k );
				opointer_type op = &out( 0, j, k );
				for( i = 0 ; i < w ; i++ )
				{
					op[ i ] = ip[ i ];
				}
			}
		}

		delete [] pf;
	}


	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	static void __linear_filter__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const Kernel &kernel,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		linear_filter( in, out, kernel, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	static void __linear_filter__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const Kernel &kernel,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		linear_filter( in, out, kernel, thread_id, thread_num, 0, 1, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	static void __linear_filter__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const Kernel &kernel,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		linear_filter( in, out, kernel, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class T2, class Kernel, class Functor >
	class linear_thread : public mist::thread< linear_thread< T1, T2, Kernel, Functor > >
	{
	public:
		typedef mist::thread< linear_thread< T1, T2, Kernel, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const T1 *in_;
		T2 *out_;
		const Kernel *kernel_;

		Functor f_;

	public:
		void setup_parameters( const T1 &in, T2 &out, const Kernel &kernel, size_type thread_id, size_type thread_num, Functor f )
		{
			in_  = &in;
			out_ = &out;
			kernel_ = &kernel;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
			f_ = f;
		}

		linear_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), kernel_( NULL )
		{
		}

		linear_thread( const linear_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), kernel_( p.kernel_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			__linear_filter__( *in_, *out_, *kernel_, thread_id_, thread_num_, f_ );
			return( true );
		}
	};


	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh, fd : フィルタサイズ
	template < class Array1, class Array2, class Functor >
	void average_filter( const Array1 &in, Array2 &out,
						typename Array1::size_type fw, typename Array1::size_type fh, typename Array1::size_type fd,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::difference_type  difference_type;
		typedef __promote_pixel_converter_< typename Array1::value_type > promote_pixel_converter;
		typedef typename promote_pixel_converter::promote_type promote_type;
		typedef typename Array2::value_type out_value_type;
		typedef typename Array1::const_pointer ipointer_type;
		typedef typename Array2::pointer opointer_type;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		const bool bprogress1 = thread_idy == 0 && d == 1;
		const bool bprogress2 = thread_idz == 0 && d > 1;

		difference_type rw = fw / 2;
		difference_type rh = fh / 2;
		difference_type rd = fd / 2;
		difference_type rrw = fw - rw;

		difference_type i, j, k;

		promote_type *sum = new promote_type[ fw ];

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			difference_type sz = k - rd;
			difference_type ez = sz + fd - 1;

			if( sz < 0 )
			{
				sz = 0;
			}
			if( ez >= d )
			{
				ez = d - 1;
			}

			difference_type fdd = ez - sz + 1;

			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				difference_type sy = j - rh;
				difference_type ey = sy + fh - 1;

				if( sy < 0 )
				{
					sy = 0;
				}
				if( ey >= h )
				{
					ey = h - 1;
				}

				difference_type fhh = ey - sy + 1;

				// キャッシュ用の総和を計算しておく
				promote_type __sum__ = promote_type( );
				for( i = 0 ; i < rrw - 1 ; i++ )
				{
					promote_type value = promote_type( );
					for( difference_type z = sz ; z <= ez ; z++ )
					{
						for( difference_type y = sy ; y <= ey ; y++ )
						{
							value += in( i, y, z );
						}
					}

					__sum__ += value;
					sum[ i ] = value;
				}

				double fsize = static_cast< double >( fhh * fdd );
				opointer_type op = &out( 0, j, k );
				size_type indx = rrw - 1;

				// 左端の処理を済ませておく
				for( i = 0 ; i <= rw ; i++, indx++ )
				{
					promote_type value = promote_type( );
					for( difference_type z = sz ; z <= ez ; z++ )
					{
						for( difference_type y = sy ; y <= ey ; y++ )
						{
							value += in( indx, y, z );
						}
					}

					__sum__ += value;
					sum[ indx ] = value;

					op[ i ] = promote_pixel_converter::convert_from( __sum__ / ( fsize * ( rrw + i ) ) );
				}

				for( ; i + rrw <= w ; i++, indx++ )
				{
					if( indx >= fw )
					{
						indx = 0;
					}

					__sum__ -= sum[ indx ];

					promote_type value = promote_type( );
					for( difference_type z = sz ; z <= ez ; z++ )
					{
						for( difference_type y = sy ; y <= ey ; y++ )
						{
							value += in( i + rrw - 1, y, z );
						}
					}

					__sum__ += value;
					sum[ indx ] = value;

					op[ i ] = promote_pixel_converter::convert_from( __sum__ / ( fsize * fw ) );
				}

				for( ; i < w ; i++, indx++ )
				{
					if( indx >= fw )
					{
						indx = 0;
					}

					__sum__ -= sum[ indx ];

					op[ i ] = promote_pixel_converter::convert_from( __sum__ / ( fsize * ( rw + w - i ) ) );
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 );
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 );
			}
		}

		delete [] sum;
	}


	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	static void __average_filter__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
									typename array< T1, Allocator1 >::size_type fw, typename array< T1, Allocator1 >::size_type fh, typename array< T1, Allocator1 >::size_type fd,
									typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		average_filter( in, out, fw, fh, fd, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	static void __average_filter__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
									typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh, typename array2< T1, Allocator1 >::size_type fd,
									typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		average_filter( in, out, fw, fh, fd, thread_id, thread_num, 0, 1, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	static void __average_filter__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
									typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
									typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		average_filter( in, out, fw, fh, fd, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class T2, class Functor >
	class average_thread : public mist::thread< average_thread< T1, T2, Functor > >
	{
	public:
		typedef mist::thread< average_thread< T1, T2, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const T1 *in_;
		T2 *out_;
		size_type fw_;
		size_type fh_;
		size_type fd_;

		Functor f_;

	public:
		void setup_parameters( const T1 &in, T2 &out, size_type fw, size_type fh, size_type fd, size_type thread_id, size_type thread_num, Functor f )
		{
			in_  = &in;
			out_ = &out;
			fw_ = fw;
			fh_ = fh;
			fd_ = fd;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
			f_ = f;
		}

		average_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), fw_( 0 ), fh_( 0 ), fd_( 0 )
		{
		}

		average_thread( const average_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), fw_( p.fw_ ), fh_( p.fh_ ), fd_( p.fd_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			__average_filter__( *in_, *out_, fw_, fh_, fd_, thread_id_, thread_num_, f_ );
			return( true );
		}
	};


	template < class Array >
	inline void compute_normalized_kernel( Array &kernel )
	{
		typedef typename Array::value_type value_type;
		typedef typename Array::size_type size_type;

		value_type sum = value_type( );
		for( size_type i = 0 ; i < kernel.size( ) ; i++ )
		{
			sum += kernel[ i ];
		}

		for( size_type i = 0 ; i < kernel.size( ) ; i++ )
		{
			kernel[ i ] /= sum;
		}
	}

	template < class Array >
	inline void compute_gaussian_kernel( Array &kernel, const double radius, double sigma )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::difference_type  difference_type;
		typedef typename Array::value_type  value_type;

		if( sigma == 0.0 )
		{
			sigma = radius * 0.5;
		}

		difference_type R = static_cast< difference_type >( std::ceil( radius ) );

		kernel.resize( R * 2 + 1, R * 2 + 1, R * 2 + 1 );

		difference_type fw = kernel.width( );
		difference_type fh = kernel.height( );
		difference_type fd = kernel.depth( );

		difference_type rw = fw / 2;
		difference_type rh = fh / 2;
		difference_type rd = fd / 2;

		double _1_sig2 = 1.0 / ( sigma * sigma * 2.0 );
		double sum = 0.0;
		for( difference_type z = -rd ; z <= rd ; z++ )
		{
			double zz = static_cast< double >( z * z );
			for( difference_type y = -rh ; y <= rh ; y++ )
			{
				double yy = static_cast< double >( y * y );
				for( difference_type x = -rw ; x <= rw ; x++ )
				{
					double xx = static_cast< double >( x * x );
					double g = std::exp( -( xx + yy + zz ) * _1_sig2 );
					sum += g;
					kernel( x + rw, y + rh, z + rd ) = static_cast< value_type >( g );
				}
			}
		}

		for( size_type i = 0 ; i < kernel.size( ) ; i++ )
		{
			kernel[ i ] /= sum;
		}
	}
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
//! mist::linear_filter( in, out, kernel );  // フィルタの適用（カーネル配列を渡します）
//! @endcode
//!
//! ラプラシアンフィルタ，ガウシアンフィルタ，一様重み平滑化フィルタを array, array1, array2, array3 で用意してあります．
//! 一様重み平滑化フィルタは，キャッシュを利用して高速に計算することができます．
//!
//!  @{


/// @brief 一般カーネルの線形フィルタの実装（コールバック関数を指定することが可能）
namespace linear
{
	/// @brief 一般の線形フィルタ( array )
	//! 
	//! カーネル配列を指定する(中心はカーネルのサイズから計算)
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  kernel     … カーネル配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	bool filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const Kernel &kernel, Functor f, typename array< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		out.resize( in.size( ) );

		__linear__::__linear_filter__( in, out, kernel, 0, 1, __mist_dmy_callback__( ) );
		
		return( true );
	}


	/// @brief 一般の線形フィルタ( array1 )
	//! 
	//! カーネル配列を指定する(中心はカーネルのサイズから計算)
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  kernel     … カーネル配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	bool filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, const Kernel &kernel, Functor f, typename array1< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		out.resize( in.size( ) );
		out.reso1( in.reso1( ) );

		__linear__::__linear_filter__( in, out, kernel, 0, 1, __mist_dmy_callback__( ) );
		
		return( true );
	}


	/// @brief 一般の線形フィルタ( array2 )
	//! 
	//! カーネル配列を指定する(中心はカーネルのサイズから計算)
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  kernel     … カーネル配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const Kernel &kernel, Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __linear__::linear_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 >, Kernel, Functor > linear_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		linear_thread *thread = new linear_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, kernel, i, thread_num, f );
		}

		f( 0.0 );

		// スレッドを実行して，終了まで待機する
		do_threads_( thread, thread_num );

		f( 100.1 );

		delete [] thread;
		
		return( true );
	}


	/// @brief 一般の線形フィルタ( array3 )
	//! 
	//! カーネル配列を指定する(中心はカーネルのサイズから計算)
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  kernel     … カーネル配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const Kernel &kernel, Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __linear__::linear_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 >, Kernel, Functor > linear_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		linear_thread *thread = new linear_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, kernel, i, thread_num, f );
		}

		f( 0.0 );

		// スレッドを実行して，終了まで待機する
		do_threads_( thread, thread_num );

		f( 100.1 );

		delete [] thread;
		
		return( true );
	}
}


/// @brief 一般の線形フィルタ( array )
//! 
//! カーネル配列を指定する(中心はカーネルのサイズから計算)
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//!
//! @note マスクの一辺のサイズは奇数でなくてはならない
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  kernel     … カーネル配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2, class Kernel >
bool linear_filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const Kernel &kernel, typename array< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( linear::filter( in, out, kernel, thread_num ) );
}




/// @brief 一般の線形フィルタ( array1 )
//! 
//! カーネル配列を指定する(中心はカーネルのサイズから計算)
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//!
//! @note マスクの一辺のサイズは奇数でなくてはならない
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  kernel     … カーネル配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2, class Kernel >
bool linear_filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, const Kernel &kernel, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( linear::filter( in, out, kernel, thread_num ) );
}


/// @brief 一般の線形フィルタ( array2 )
//! 
//! カーネル配列を指定する(中心はカーネルのサイズから計算)
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @note マスクの一辺のサイズは奇数でなくてはならない
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  kernel     … カーネル配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2, class Kernel >
bool linear_filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const Kernel &kernel, typename array2< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( linear::filter( in, out, kernel, __mist_dmy_callback__( ), thread_num ) );
}


/// @brief 一般の線形フィルタ( array3 )
//! 
//! カーネル配列を指定する(中心はカーネルのサイズから計算)
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @note マスクの一辺のサイズは奇数でなくてはならない
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  kernel     … カーネル配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2, class Kernel >
bool linear_filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const Kernel &kernel, typename array3< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( linear::filter( in, out, kernel, __mist_dmy_callback__( ), thread_num ) );
}



//! @addtogroup laplacian_group ラプラシアンフィルタ
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
//!  @{

/// @brief ラプラシアンフィルタの実装（コールバック関数を指定することが可能）
namespace laplacian
{
	/// @brief ラプラシアン( array )
	//! 
	//! サイズ 3 のラプラシアン
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, Functor f, typename array< T1, Allocator1 >::size_type thread_num )
	{
		array< double > a( 3 );
		a[ 0 ] = 1.0;  a[ 1 ] = -2.0;  a[ 2 ] = 1.0;

		return( linear::filter( in, out, a, f, thread_num ) );
	}

	/// @brief ラプラシアン( array1 )
	//! 
	//! サイズ 3 のラプラシアン
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, Functor f, typename array1< T1, Allocator1 >::size_type thread_num )
	{
		array< double > a( 3 );
		a[ 0 ] = 1.0;  a[ 1 ] = -2.0;  a[ 2 ] = 1.0;

		return( linear::filter( in, out, a, f, thread_num ) );
	}

	/// @brief ラプラシアン( array2 )
	//! 
	//! サイズ 3×3 のラプラシアン
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		array2< double > a( 3, 3 );

		a( 0, 0 ) = 1.0;  a( 1, 0 ) = 1.0;  a( 2, 0 ) = 1.0;
		a( 0, 1 ) = 1.0;  a( 1, 1 ) = -8.0;  a( 2, 1 ) = 1.0;
		a( 0, 2 ) = 1.0;  a( 1, 2 ) = 1.0;  a( 2, 2 ) = 1.0;

		return( linear::filter( in, out, a, f, thread_num ) );
	}

	/// @brief ラプラシアン( array3 )
	//! 
	//! サイズ 3×3×3 のラプラシアン
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		array3< double > a( 3, 3, 3 );

		a( 0, 0, 0 ) = 1.0;  a( 1, 0, 0 ) =   1.0;  a( 2, 0, 0 ) = 1.0;
		a( 0, 1, 0 ) = 1.0;  a( 1, 1, 0 ) =   1.0;  a( 2, 1, 0 ) = 1.0;
		a( 0, 2, 0 ) = 1.0;  a( 1, 2, 0 ) =   1.0;  a( 2, 2, 0 ) = 1.0;
		a( 0, 0, 1 ) = 1.0;  a( 1, 0, 1 ) =   1.0;  a( 2, 0, 1 ) = 1.0;
		a( 0, 1, 1 ) = 1.0;  a( 1, 1, 1 ) = -26.0;  a( 2, 1, 1 ) = 1.0;
		a( 0, 2, 1 ) = 1.0;  a( 1, 2, 1 ) =   1.0;  a( 2, 2, 1 ) = 1.0;
		a( 0, 0, 2 ) = 1.0;  a( 1, 0, 2 ) =   1.0;  a( 2, 0, 2 ) = 1.0;
		a( 0, 1, 2 ) = 1.0;  a( 1, 1, 2 ) =   1.0;  a( 2, 1, 2 ) = 1.0;
		a( 0, 2, 2 ) = 1.0;  a( 1, 2, 2 ) =   1.0;  a( 2, 2, 2 ) = 1.0;

		return( linear::filter( in, out, a, f, thread_num ) );
	}
}


/// @brief ラプラシアン( array )
//! 
//! サイズ 3 のラプラシアン
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool laplacian_filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, typename array< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( laplacian::filter( in, out, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ラプラシアン( array1 )
//! 
//! サイズ 3 のラプラシアン
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool laplacian_filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( laplacian::filter( in, out, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ラプラシアン( array2 )
//! 
//! サイズ 3×3 のラプラシアン
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool laplacian_filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( laplacian::filter( in, out, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ラプラシアン( array3 )
//! 
//! サイズ 3×3×3 のラプラシアン
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool laplacian_filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( laplacian::filter( in, out, __mist_dmy_callback__( ), thread_num ) );
}

/// @}
//  ラプラシアングループの終わり





//! @addtogroup gaussian_group ガウシアンフィルタ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/linear.h>
//! @endcode
//!
//! 配列の端（カーネルがはみ出すところ）の要素の計算値は保障しません．
//! 使用可能な要素型は，算術型か mist::rgb< 算術型 > に限ります．
//! 内部の計算は，出力配列の要素型が算術型の場合は double 型，
//! mist::rgb< 算術型 > の場合は mist::rgb< double > で行います．
//! 
//!  @{

/// @brief ガウシアンフィルタの実装（コールバック関数を指定することが可能）
namespace gaussian
{
	/// @brief ガウシアンフィルタ( array )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  radius     … フィルタ半径
	//! @param[in]  sigma      … フィルタの標準偏差
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const double radius, const double sigma, Functor f, typename array< T1, Allocator1 >::size_type thread_num )
	{
		array< double > a( 3 );

		__linear__::compute_gaussian_kernel( a, radius, sigma );

		return( linear::filter( in, out, a, f, thread_num ) );
	}

	/// @brief ガウシアンフィルタ( array1 )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  radius     … フィルタ半径
	//! @param[in]  sigma      … フィルタの標準偏差
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, const double radius, const double sigma, Functor f, typename array1< T1, Allocator1 >::size_type thread_num )
	{
		array< double > a( 3 );

		__linear__::compute_gaussian_kernel( a, radius, sigma );

		return( linear::filter( in, out, a, f, thread_num ) );
	}

	/// @brief ガウシアンフィルタ( array2 )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  radius     … フィルタ半径
	//! @param[in]  sigma      … フィルタの標準偏差
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const double radius, const double sigma, Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		array2< double > a( 3, 3 );

		__linear__::compute_gaussian_kernel( a, radius, sigma );

		return( linear::filter( in, out, a, f, thread_num ) );
	}

	/// @brief ガウシアンフィルタ( array3 )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  radius     … フィルタ半径
	//! @param[in]  sigma      … フィルタの標準偏差
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const double radius, const double sigma, Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		array3< double > a( 3, 3, 3 );

		__linear__::compute_gaussian_kernel( a, radius, sigma );

		return( linear::filter( in, out, a, f, thread_num ) );
	}
}


/// @brief ガウシアンフィルタ( array )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  radius     … フィルタ半径
//! @param[in]  sigma      … フィルタの標準偏差
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const double radius = 2.0, const double sigma = 0.0, typename array< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, radius, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ガウシアンフィルタ( array1 )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  radius     … フィルタ半径
//! @param[in]  sigma      … フィルタの標準偏差
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, const double radius = 2.0, const double sigma = 0.0, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, radius, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ガウシアンフィルタ( array2 )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  radius     … フィルタ半径
//! @param[in]  sigma      … フィルタの標準偏差
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const double radius = 2.0, const double sigma = 0.0, typename array2< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, radius, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ガウシアンフィルタ( array3 )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  radius     … フィルタ半径
//! @param[in]  sigma      … フィルタの標準偏差
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const double radius = 2.0, const double sigma = 0.0, typename array3< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, radius, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @}
//  ガウシアングループの終わり



//! @addtogroup average_group 一様重みフィルタ
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/linear.h>
//! @endcode
//!
//! 配列の端（カーネルがはみ出すところ）に対してもフィルタを適用します．
//! 使用可能な要素型は，算術型か mist::rgb< 算術型 > に限ります．
//! 内部の計算は，出力配列の要素型が算術型の場合は double 型，
//! mist::rgb< 算術型 > の場合は mist::rgb< double > で行います．
//! 
//!  @{

/// @brief 一様重み平滑化フィルタの実装（コールバック関数を指定することが可能）
namespace average
{
	/// @brief 一様重み( array )
	//! 
	//! サイズ fw の一様重み
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  fw         … X軸方向のフィルタサイズ
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
				 typename array< T1, Allocator1 >::size_type fw, Functor f, typename array< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		out.resize( in.size( ) );

		__linear__::__average_filter__( in, out, fw, 1, 1, 0, 1, __mist_dmy_callback__( ) );
		
		return( true );
	}

	/// @brief 一様重み( array1 )
	//! 
	//! サイズ fw の一様重み
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  fw         … X軸方向のフィルタサイズ
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
				 typename array1< T1, Allocator1 >::size_type fw, Functor f, typename array1< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		out.resize( in.size( ) );
		out.reso1( in.reso1( ) );

		__linear__::__average_filter__( in, out, fw, 1, 1, 0, 1, __mist_dmy_callback__( ) );
		
		return( true );
	}

	/// @brief 一様重み( array2 )
	//! 
	//! サイズ fw × fh の一様重み
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  fw         … X軸方向のフィルタサイズ
	//! @param[in]  fh         … Y軸方向のフィルタサイズ
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
				   typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh,
				   Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __linear__::average_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 >, Functor > average_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		average_thread *thread = new average_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, fw, fh, 1, i, thread_num, f );
		}

		f( 0.0 );

		// スレッドを実行して，終了まで待機する
		do_threads_( thread, thread_num );

		f( 100.1 );

		delete [] thread;
		
		return( true );
	}

	/// @brief 一様重み( array3 )
	//! 
	//! サイズ fw × fh × fd の一様重み
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  fw         … X軸方向のフィルタサイズ
	//! @param[in]  fh         … Y軸方向のフィルタサイズ
	//! @param[in]  fd         … Z軸方向のフィルタサイズ
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
				   typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
				   Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __linear__::average_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 >, Functor > average_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		average_thread *thread = new average_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, fw, fh, fd, i, thread_num, f );
		}

		f( 0.0 );

		// スレッドを実行して，終了まで待機する
		do_threads_( thread, thread_num );

		f( 100.1 );

		delete [] thread;
		
		return( true );
	}
}


/// @brief 一様重み( array )
//! 
//! サイズ fw の一様重み
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  fw         … X軸方向のフィルタサイズ
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool average_filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, typename array< T1, Allocator1 >::size_type fw, typename array< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( average::filter( in, out, fw, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief 一様重み( array1 )
//! 
//! サイズ fw の一様重み
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数は無視され，常にシングルスレッドで動作する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  fw         … X軸方向のフィルタサイズ
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool average_filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, typename array1< T1, Allocator1 >::size_type fw, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( average::filter( in, out, fw, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief 一様重み( array2 )
//! 
//! サイズ fw × fh の一様重み
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  fw         … X軸方向のフィルタサイズ
//! @param[in]  fh         … Y軸方向のフィルタサイズ
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool average_filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					 typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh, typename array2< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( average::filter( in, out, fw, fh, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief 一様重み( array3 )
//! 
//! サイズ fw × fh × fd の一様重み
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  fw         … X軸方向のフィルタサイズ
//! @param[in]  fh         … Y軸方向のフィルタサイズ
//! @param[in]  fd         … Z軸方向のフィルタサイズ
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool average_filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					 typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
					 typename array3< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( average::filter( in, out, fw, fh, fd, __mist_dmy_callback__( ), thread_num ) );
}

/// @}
//  一様重みグループの終わり



/// @}
//  線形グループの終わり


// mist名前空間の終わり
_MIST_END



#endif // __INCLUDE_FILTER_LINEAR_FILTER__
