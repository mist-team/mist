// 
// Copyright (c) 2003-2008, MIST Project, Intelligent Media Integration COE, Nagoya University
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

#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
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


// 線形フィルタ
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

	template < int DIMENSION >
	struct __access__
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _1, _2, _3 ) );
		}

		template < class Array >
		inline static const typename Array::value_type &at( const Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _1, _2, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }
	};

	template < >
	struct __access__< 2 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _1, _3 ) );
		}

		template < class Array >
		inline static const typename Array::value_type &at( const Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _1, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }
	};

	template < >
	struct __access__< 3 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _3, _1 ) );
		}

		template < class Array >
		inline static const typename Array::value_type &at( const Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _3, _1 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size2( ) ); }
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
		typedef typename Array2::value_type    out_value_type;
		typedef typename Array1::const_pointer ipointer_type;
		typedef typename Array2::pointer       opointer_type;
		typedef typename Kernel::const_pointer kpointer_type;
		typedef typename Kernel::value_type    kvalue_type;

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

		// 画像の縁の処理を行う
		for( k = thread_idz ; k < rd ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				opointer_type op = &out( 0, j, k );

				for( i = 0 ; i < w ; i++ )
				{
					promote_type value   = promote_type( );
					kvalue_type  sum     = 0;
					difference_type sl   = i < rw ? rw - i : 0;
					difference_type el   = w - i <= rw ? rw + w - i : fw;
					difference_type sm   = j < rh ? rh - j : 0;
					difference_type em   = h - j <= rh ? rh + h - j : fh;
					difference_type sn   = k < rd ? rd - k : 0;

					for( difference_type n = sn ; n < fd ; n++ )
					{
						for( difference_type m = sm ; m < em ; m++ )
						{
							for( difference_type l = sl ; l < el ; l++ )
							{
								kvalue_type kv = kernel( l, m, n );
								value += kernel( l, m, n ) * in( i + l - rw, j + m - rh, k + n - rd );
								sum += kv;
							}
						}
					}

					op[ i ] = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
				}
			}
		}

		// 画像の中心部分の処理を行う
		for( ; k + rd < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < rh ; j += thread_numy )
			{
				opointer_type op = &out( 0, j, k );

				for( i = 0 ; i < w ; i++ )
				{
					promote_type value   = promote_type( );
					kvalue_type  sum     = 0;
					difference_type sl   = i < rw ? rw - i : 0;
					difference_type el   = w - i <= rw ? rw + w - i : fw;
					difference_type sm   = rh - j;

					for( difference_type n = 0 ; n < fd ; n++ )
					{
						for( difference_type m = sm ; m < fh ; m++ )
						{
							for( difference_type l = sl ; l < el ; l++ )
							{
								kvalue_type kv = kernel( l, m, n );
								value += kernel( l, m, n ) * in( i + l - rw, j + m - rh, k + n - rd );
								sum += kv;
							}
						}
					}

					op[ i ] = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
				}
			}

			// 中心部分の処理を行う
			for( ; j + rh < h ; j += thread_numy )
			{
				ipointer_type ip = &in( 0, j, k );
				opointer_type op = &out( 0, j, k );

				for( i = 0 ; i < rw ; i++ )
				{
					difference_type *ppf = pf;
					ipointer_type p      = ip + i;
					kpointer_type kp     = &kernel[ 0 ];
					promote_type value   = promote_type( );
					kvalue_type  sum     = 0;
					difference_type lnum = fh * fd;
					difference_type sn   = rw - i;

					for( difference_type l = 0 ; l < lnum ; l++ )
					{
						for( difference_type n = sn ; n < fw ; n++ )
						{
							value += kp[ n ] * p[ ppf[ n ] ];
							sum += kp[ n ];
						}

						kp  += fw;
						ppf += fw;
					}

					op[ i ] = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
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
					difference_type *ppf = pf;
					ipointer_type p      = ip + i;
					kpointer_type kp     = &kernel[ 0 ];
					promote_type value   = promote_type( );
					kvalue_type  sum     = 0;
					difference_type lnum = fh * fd;
					difference_type en   = rw + w - i;

					for( difference_type l = 0 ; l < lnum ; l++ )
					{
						for( difference_type n = 0 ; n < en ; n++ )
						{
							value += kp[ n ] * p[ ppf[ n ] ];
							sum += kp[ n ];
						}

						kp  += fw;
						ppf += fw;
					}

					op[ i ] = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( h ) * 100.0 );
				}
			}

			for( ; j < h ; j += thread_numy )
			{
				opointer_type op = &out( 0, j, k );

				for( i = 0 ; i < w ; i++ )
				{
					promote_type value   = promote_type( );
					kvalue_type  sum     = 0;
					difference_type sl   = i < rw ? rw - i : 0;
					difference_type el   = w - i <= rw ? rw + w - i : fw;
					difference_type em   = rh + h - j;

					for( difference_type n = 0 ; n < fd ; n++ )
					{
						for( difference_type m = 0 ; m < em ; m++ )
						{
							for( difference_type l = sl ; l < el ; l++ )
							{
								kvalue_type kv = kernel( l, m, n );
								value += kernel( l, m, n ) * in( i + l - rw, j + m - rh, k + n - rd );
								sum += kv;
							}
						}
					}

					op[ i ] = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( d ) * 100.0 );
			}
		}

		// 画像の縁の処理を行う
		for( ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				opointer_type op = &out( 0, j, k );

				for( i = 0 ; i < w ; i++ )
				{
					promote_type value   = promote_type( );
					kvalue_type  sum     = 0;
					difference_type sl   = i < rw ? rw - i : 0;
					difference_type el   = w - i <= rw ? rw + w - i : fw;
					difference_type sm   = j < rh ? rh - j : 0;
					difference_type em   = h - j <= rh ? rh + h - j : fh;
					difference_type en   = d - k <= rd ? rd + d - k : fd;

					for( difference_type n = 0 ; n < en ; n++ )
					{
						for( difference_type m = sm ; m < em ; m++ )
						{
							for( difference_type l = sl ; l < el ; l++ )
							{
								kvalue_type kv = kernel( l, m, n );
								value += kernel( l, m, n ) * in( i + l - rw, j + m - rh, k + n - rd );
								sum += kv;
							}
						}
					}

					op[ i ] = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
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


	template < int DIMENSION >
	struct _1D_linear_filter_
	{
		template < class Array1, class Array2, class Kernel, class Functor >
		static void linear_filter( const Array1 &in, Array2 &out, const Kernel &kernel,
									typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
									typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f, double sp, double ep )
		{
			typedef typename Array1::size_type		 size_type;
			typedef typename Array1::value_type		 value_type;
			typedef typename Kernel::value_type		 kvalue_type;
			typedef typename Array1::const_pointer	 ipointer;
			typedef typename Array2::pointer		 opointer;
			typedef typename Array1::difference_type difference_type;
			typedef __promote_pixel_converter_< typename Array2::value_type > promote_pixel_converter;
			typedef typename promote_pixel_converter::promote_type promote_type;
			typedef __access__< DIMENSION > access;

			difference_type _1e = access::size1( in );
			difference_type _2e = access::size2( in );
			difference_type _3e = access::size3( in );

			const bool bprogress1 = thread_idy == 0 && _3e == 1;
			const bool bprogress2 = thread_idz == 0 && _3e > 1;

			difference_type fw = kernel.size( );
			difference_type rw = fw / 2;

			difference_type i1, i2, i3;
			value_type *tmp = new value_type[ _1e ];
			difference_type diff = &access::at( in, 1, 0, 0 ) - &access::at( in, 0, 0, 0 );

			for( i3 = thread_idz ; i3 < _3e ; i3 += thread_numz )
			{
				for( i2 = thread_idy ; i2 < _2e ; i2 += thread_numy )
				{
					ipointer p0 = &access::at( in,  0, i2, i3 );
					opointer op = &access::at( out, 0, i2, i3 );
					ipointer p  = p0;

					// テンポラリ領域に画素値を一時的に記憶する
					for( i1 = 0 ; i1 + rw + 1 < fw ; i1++ )
					{
						tmp[ i1 ] = *p;
						p += diff;
					}

					for( i1 = 0 ; i1 < rw ; i1++ )
					{
						tmp[ i1 + rw ] = *p;
						p += diff;

						promote_type value = promote_type( );

						value_type *ptmp = tmp + i1 - rw;
						kvalue_type sum = 0;
						for( size_type n = rw - i1 ; n < kernel.size( ) ; n++ )
						{
							value += kernel[ n ] * ptmp[ n ];
							sum += kernel[ n ];
						}

						*op = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
						op += diff;
					}

					for( ; i1 + rw < _1e ; i1++ )
					{
						tmp[ i1 + rw ] = *p;
						p += diff;

						promote_type value = promote_type( );

						value_type *ptmp = tmp + i1 - rw;
						for( size_type n = 0 ; n < kernel.size( ) ; n++ )
						{
							value += kernel[ n ] * ptmp[ n ];
						}

						*op = promote_pixel_converter::convert_from( value );
						op += diff;
					}

					for( ; i1 < _1e ; i1++ )
					{
						promote_type value = promote_type( );

						value_type *ptmp = tmp + i1 - rw;
						kvalue_type sum = 0;
						size_type   ne = rw + _1e - i1;
						for( size_type n = 0 ; n < ne ; n++ )
						{
							value += kernel[ n ] * ptmp[ n ];
							sum += kernel[ n ];
						}

						*op = promote_pixel_converter::convert_from( sum == 0 ? value : value / sum );
						op += diff;
					}

					if( bprogress1 )
					{
						f( sp + static_cast< double >( i2 + 1 ) / static_cast< double >( _2e ) * ( ep - sp ) );
					}
				}

				if( bprogress2 )
				{
					f( sp + static_cast< double >( i3 + 1 ) / static_cast< double >( _3e ) * ( ep - sp ) );
				}
			}

			delete [] tmp;
		}
	};

	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	static void __1D_linear_filter__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const Kernel &k1, const Kernel &k2, const Kernel &k3,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		linear_filter( in, out, k1, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	static void __1D_linear_filter__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const Kernel &k1, const Kernel &k2, const Kernel &k3,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		_1D_linear_filter_< 1 >::linear_filter( in,  out, k1, thread_id, thread_num, 0, 1, f,  0.0,  50.0 );
		_1D_linear_filter_< 2 >::linear_filter( out, out, k2, thread_id, thread_num, 0, 1, f, 50.0, 100.0 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	static void __1D_linear_filter__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const Kernel &k1, const Kernel &k2, const Kernel &k3,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		_1D_linear_filter_< 1 >::linear_filter( in,  out, k1, 0, 1, thread_id, thread_num, f,         0.0,  100.0 / 3.0 );
		_1D_linear_filter_< 2 >::linear_filter( out, out, k2, 0, 1, thread_id, thread_num, f, 100.0 / 3.0,  200.0 / 3.0 );
		_1D_linear_filter_< 3 >::linear_filter( out, out, k3, 0, 1, thread_id, thread_num, f, 200.0 / 3.0,  100.0 );
	}

	template < class T1, class T2, class Kernel, class Functor >
	class _1D_linear_thread : public mist::thread< _1D_linear_thread< T1, T2, Kernel, Functor > >
	{
	public:
		typedef mist::thread< _1D_linear_thread< T1, T2, Kernel, Functor > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const T1 *in_;
		T2 *out_;
		const Kernel *k1_;
		const Kernel *k2_;
		const Kernel *k3_;

		Functor f_;

	public:
		void setup_parameters( const T1 &in, T2 &out, const Kernel &k1, const Kernel &k2, const Kernel &k3, size_type thread_id, size_type thread_num, Functor f )
		{
			in_  = &in;
			out_ = &out;
			k1_ = &k1;
			k2_ = &k2;
			k3_ = &k3;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
			f_ = f;
		}

		_1D_linear_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), k1_( NULL ), k2_( NULL ), k3_( NULL )
		{
		}

		_1D_linear_thread( const _1D_linear_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), k1_( p.k1_ ), k2_( p.k2_ ), k3_( p.k3_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			__1D_linear_filter__( *in_, *out_, *k1_, *k2_, *k3_, thread_id_, thread_num_, f_ );
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
		typedef __promote_pixel_converter_< typename Array2::value_type > promote_pixel_converter;
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

	template < int DIMENSION >
	struct _1D_average_filter_
	{
		template < class Array1, class Array2, class Functor >
		static void average_filter( const Array1 &in, Array2 &out, typename Array1::difference_type fw,
									typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
									typename Array1::size_type thread_idz, typename Array1::size_type thread_numz, Functor f, double sp, double ep )
		{
			typedef typename Array1::size_type		 size_type;
			typedef typename Array1::value_type		 value_type;
			typedef typename Array1::const_pointer	 ipointer;
			typedef typename Array2::pointer		 opointer;
			typedef typename Array1::difference_type difference_type;
			typedef __promote_pixel_converter_< typename Array2::value_type > promote_pixel_converter;
			typedef typename promote_pixel_converter::promote_type promote_type;
			typedef __access__< DIMENSION > access;

			difference_type _1e = access::size1( in );
			difference_type _2e = access::size2( in );
			difference_type _3e = access::size3( in );

			const bool bprogress1 = thread_idy == 0 && _3e == 1;
			const bool bprogress2 = thread_idz == 0 && _3e > 1;

			difference_type rw = fw / 2;

			difference_type i1, i2, i3;
			value_type *tmp = new value_type[ _1e ];
			difference_type diff = &access::at( in, 1, 0, 0 ) - &access::at( in, 0, 0, 0 );

			for( i3 = thread_idz ; i3 < _3e ; i3 += thread_numz )
			{
				for( i2 = thread_idy ; i2 < _2e ; i2 += thread_numy )
				{
					ipointer p0 = &access::at( in,  0, i2, i3 );
					opointer op = &access::at( out, 0, i2, i3 );
					ipointer p  = p0;

					promote_type value = promote_type( );

					// テンポラリ領域に画素値を一時的に記憶する
					for( i1 = 0 ; i1 + rw + 1 < fw ; i1++ )
					{
						value += *p;
						tmp[ i1 ] = *p;
						p += diff;
					}

					for( i1 = 0 ; i1 <= rw ; i1++ )
					{
						value += *p;
						tmp[ i1 + rw ] = *p;
						p += diff;

						*op = promote_pixel_converter::convert_from( value / static_cast< double >( fw - rw + i1 ) );
						op += diff;
					}

					for( ; i1 + rw < _1e ; i1++ )
					{
						value += *p;
						value -= tmp[ i1 - rw - 1 ];
						tmp[ i1 + rw ] = *p;
						p += diff;

						*op = promote_pixel_converter::convert_from( value / static_cast< double >( fw ) );
						op += diff;
					}

					for( ; i1 < _1e ; i1++ )
					{
						value -= tmp[ i1 - rw - 1 ];

						*op = promote_pixel_converter::convert_from( value / static_cast< double >( rw + _1e - i1 ) );
						op += diff;
					}

					if( bprogress1 )
					{
						f( sp + static_cast< double >( i2 + 1 ) / static_cast< double >( _2e ) * ( ep - sp ) );
					}
				}

				if( bprogress2 )
				{
					f( sp + static_cast< double >( i3 + 1 ) / static_cast< double >( _3e ) * ( ep - sp ) );
				}
			}

			delete [] tmp;
		}
	};

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	static void __1D_average_filter__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
									typename array< T1, Allocator1 >::size_type fw, typename array< T1, Allocator1 >::size_type fh, typename array< T1, Allocator1 >::size_type fd,
									typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		average_filter( in, out, fw, fh, fd, 0, 1, thread_id, thread_num, f );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	static void __1D_average_filter__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
									typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh, typename array2< T1, Allocator1 >::size_type fd,
									typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		_1D_average_filter_< 1 >::average_filter( in,  out, fw, thread_id, thread_num, 0, 1, f,  0.0,  50.0 );
		_1D_average_filter_< 2 >::average_filter( out, out, fh, thread_id, thread_num, 0, 1, f, 50.0, 100.0 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	static void __1D_average_filter__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
									typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
									typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num, Functor f )
	{
		_1D_average_filter_< 1 >::average_filter( in,  out, fw, 0, 1, thread_id, thread_num, f,         0.0,  100.0 / 3.0 );
		_1D_average_filter_< 2 >::average_filter( out, out, fh, 0, 1, thread_id, thread_num, f, 100.0 / 3.0,  200.0 / 3.0 );
		_1D_average_filter_< 3 >::average_filter( out, out, fd, 0, 1, thread_id, thread_num, f, 200.0 / 3.0,  100.0 );
	}

	template < class T1, class T2, class Functor >
	class _1D_average_thread : public mist::thread< _1D_average_thread< T1, T2, Functor > >
	{
	public:
		typedef mist::thread< _1D_average_thread< T1, T2, Functor > > base;
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

		_1D_average_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), fw_( 0 ), fh_( 0 ), fd_( 0 )
		{
		}

		_1D_average_thread( const _1D_average_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), fw_( p.fw_ ), fh_( p.fh_ ), fd_( p.fd_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			__1D_average_filter__( *in_, *out_, fw_, fh_, fd_, thread_id_, thread_num_, f_ );
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
	inline void compute_gaussian_kernel( Array &kernel, double sigma )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::difference_type  difference_type;
		typedef typename Array::value_type  value_type;

		if( sigma == 0.0 )
		{
			sigma = 1.0;
		}

		double radius = sigma * 2.0;
		double ax = kernel.reso1( );
		double ay = kernel.reso2( );
		double az = kernel.reso3( );
		double axx = ax * ax;
		double ayy = ay * ay;
		double azz = az * az;

		difference_type Rx = static_cast< difference_type >( std::ceil( radius / ax ) );
		difference_type Ry = static_cast< difference_type >( std::ceil( radius / ay ) );
		difference_type Rz = static_cast< difference_type >( std::ceil( radius / az ) );

		kernel.resize( Rx * 2 + 1, Ry * 2 + 1, Rz * 2 + 1 );

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
			double zz = static_cast< double >( z * z * azz );
			for( difference_type y = -rh ; y <= rh ; y++ )
			{
				double yy = static_cast< double >( y * y * ayy );
				for( difference_type x = -rw ; x <= rw ; x++ )
				{
					double xx = static_cast< double >( x * x * axx );
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

	template < class Array >
	inline void compute_gaussian_kernel( Array &k1, Array &k2, Array &k3, double sigma )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::difference_type  difference_type;
		typedef typename Array::value_type  value_type;

		if( sigma == 0.0 )
		{
			sigma = 1.0;
		}

		double radius = sigma * 2.0;
		double ax = k1.reso1( );
		double ay = k2.reso1( );
		double az = k3.reso1( );
		double axx = ax * ax;
		double ayy = ay * ay;
		double azz = az * az;

		difference_type Rx = static_cast< difference_type >( std::ceil( radius / ax ) );
		difference_type Ry = static_cast< difference_type >( std::ceil( radius / ay ) );
		difference_type Rz = static_cast< difference_type >( std::ceil( radius / az ) );

		k1.resize( Rx * 2 + 1 );
		k2.resize( Ry * 2 + 1 );
		k3.resize( Rz * 2 + 1 );

		difference_type fw = k1.size( );
		difference_type fh = k2.size( );
		difference_type fd = k3.size( );

		difference_type rw = fw / 2;
		difference_type rh = fh / 2;
		difference_type rd = fd / 2;

		double _1_sig2 = 1.0 / ( sigma * sigma * 2.0 );
		{
			double sum = 0.0;
			for( difference_type x = -rw ; x <= rw ; x++ )
			{
				double xx = static_cast< double >( x * x * axx );
				double g = std::exp( -xx * _1_sig2 );
				sum += g;
				k1[ x + rw ] = static_cast< value_type >( g );
			}

			for( size_type i = 0 ; i < k1.size( ) ; i++ )
			{
				k1[ i ] /= sum;
			}
		}

		{
			double sum = 0.0;
			for( difference_type y = -rh ; y <= rh ; y++ )
			{
				double yy = static_cast< double >( y * y * ayy );
				double g = std::exp( -yy * _1_sig2 );
				sum += g;
				k2[ y + rh ] = static_cast< value_type >( g );
			}

			for( size_type i = 0 ; i < k2.size( ) ; i++ )
			{
				k2[ i ] /= sum;
			}
		}

		{
			double sum = 0.0;
			for( difference_type z = -rd ; z <= rd ; z++ )
			{
				double zz = static_cast< double >( z * z * azz );
				double g = std::exp( -zz * _1_sig2 );
				sum += g;
				k3[ z + rd ] = static_cast< value_type >( g );
			}

			for( size_type i = 0 ; i < k3.size( ) ; i++ )
			{
				k3[ i ] /= sum;
			}
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


	/// @brief 一般の線形フィルタ( array2 )
	//! 
	//! 1次元分解した各軸に沿ったカーネル配列を画像に適用する(中心はカーネルのサイズから計算)
	//! ガウシアンフィルタ等の1次元分解可能なフィルタで利用可能です
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくても動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  kernel1    … X軸方向のカーネル配列
	//! @param[in]  kernel2    … Y軸方向のカーネル配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	bool filter1d( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const Kernel &kernel1, const Kernel &kernel2, Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		if( in.empty( ) )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __linear__::_1D_linear_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 >, Kernel, Functor > _1D_linear_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		_1D_linear_thread *thread = new _1D_linear_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, kernel1, kernel2, kernel2, i, thread_num, f );
		}

		f( 0.0 );

		// スレッドを実行して，終了まで待機する
		do_threads_( thread, thread_num );

		f( 100.1 );

		delete [] thread;
		
		return( true );
	}


	/// @brief 一般の線形フィルタ( array2 )
	//! 
	//! 1次元分解した各軸に沿ったカーネル配列を画像に適用する(中心はカーネルのサイズから計算)
	//! ガウシアンフィルタ等の1次元分解可能なフィルタで利用可能です
	//!
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in,out] in         … 入力・出力配列
	//! @param[in]     kernel1    … X軸方向のカーネル配列
	//! @param[in]     kernel2    … Y軸方向のカーネル配列
	//! @param[in]     f          … コールバック関数
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T, class Allocator, class Kernel, class Functor >
	bool filter1d( array2< T, Allocator > &in, const Kernel &kernel1, const Kernel &kernel2, Functor f, typename array2< T, Allocator >::size_type thread_num )
	{
		if( in.empty( ) )
		{
			return( false );
		}

		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef __linear__::_1D_linear_thread< array2< T, Allocator >, array2< T, Allocator >, Kernel, Functor > _1D_linear_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		_1D_linear_thread *thread = new _1D_linear_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, in, kernel1, kernel2, kernel2, i, thread_num, f );
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


	/// @brief 一般の線形フィルタ( array3 )
	//! 
	//! 1次元分解した各軸に沿ったカーネル配列を画像に適用する(中心はカーネルのサイズから計算)
	//! ガウシアンフィルタ等の1次元分解可能なフィルタで利用可能です
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくても動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  kernel1    … X軸方向のカーネル配列
	//! @param[in]  kernel2    … Y軸方向のカーネル配列
	//! @param[in]  kernel3    … Z軸方向のカーネル配列
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Kernel, class Functor >
	bool filter1d( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const Kernel &kernel1, const Kernel &kernel2, const Kernel &kernel3, Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		if( in.empty( ) )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __linear__::_1D_linear_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 >, Kernel, Functor > _1D_linear_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );

		_1D_linear_thread *thread = new _1D_linear_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, kernel1, kernel2, kernel3, i, thread_num, f );
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
	//! 1次元分解した各軸に沿ったカーネル配列を画像に適用する(中心はカーネルのサイズから計算)
	//! ガウシアンフィルタ等の1次元分解可能なフィルタで利用可能です
	//!
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @note マスクの一辺のサイズは奇数でなくてはならない
	//! 
	//! @param[in,out] in         … 入力・出力配列
	//! @param[in]     kernel1    … X軸方向のカーネル配列
	//! @param[in]     kernel2    … Y軸方向のカーネル配列
	//! @param[in]     kernel3    … Z軸方向のカーネル配列
	//! @param[in]     f          … コールバック関数
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T, class Allocator, class Kernel, class Functor >
	bool filter1d( array3< T, Allocator > &in, const Kernel &kernel1, const Kernel &kernel2, const Kernel &kernel3, Functor f, typename array3< T, Allocator >::size_type thread_num )
	{
		if( in.empty( ) )
		{
			return( false );
		}

		typedef typename array3< T, Allocator >::size_type  size_type;
		typedef __linear__::_1D_linear_thread< array3< T, Allocator >, array3< T, Allocator >, Kernel, Functor > _1D_linear_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		_1D_linear_thread *thread = new _1D_linear_thread[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, in, kernel1, kernel2, kernel3, i, thread_num, f );
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
	template < bool b >
	struct gaussian_filter_helper
	{
		template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
		static bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const double sigma, Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
		{
			array2< double > a( 3, 3, in.reso1( ), in.reso2( ) );

			__linear__::compute_gaussian_kernel( a, sigma );

			return( linear::filter( in, out, a, f, thread_num ) );
		}

		template < class T, class Allocator, class Functor >
		static bool filter( array2< T, Allocator > &in, const double sigma, Functor f, typename array2< T, Allocator >::size_type thread_num )
		{
			array2< double > a( 3, 3, in.reso1( ), in.reso2( ) );

			__linear__::compute_gaussian_kernel( a, sigma );

			array2< T, Allocator > tmp( in );
			return( linear::filter( tmp, in, a, f, thread_num ) );
		}

		template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
		static bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const double sigma, Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
		{
			array3< double > a( 3, 3, 3, in.reso1( ), in.reso2( ), in.reso3( ) );

			__linear__::compute_gaussian_kernel( a, sigma );

			return( linear::filter( in, out, a, f, thread_num ) );
		}

		template < class T, class Allocator, class Functor >
		static bool filter( array3< T, Allocator > &in, const double sigma, Functor f, typename array3< T, Allocator >::size_type thread_num )
		{
			array3< double > a( 3, 3, 3, in.reso1( ), in.reso2( ), in.reso3( ) );

			__linear__::compute_gaussian_kernel( a, sigma );

			array2< T, Allocator > tmp( in );
			return( linear::filter( tmp, in, a, f, thread_num ) );
		}
	};

	template < >
	struct gaussian_filter_helper< true >
	{
		template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
		static bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const double sigma, Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
		{
			// データの方が浮動小数点型の場合は高速なバージョンを利用する
			array1< double > a1( 3, in.reso1( ) ), a2( 3, in.reso2( ) ), a3( 3, in.reso3( ) );

			__linear__::compute_gaussian_kernel( a1, a2, a3, sigma );

			return( linear::filter1d( in, out, a1, a2, f, thread_num ) );
		}

		template < class T, class Allocator, class Functor >
		static bool filter( array2< T, Allocator > &in, const double sigma, Functor f, typename array2< T, Allocator >::size_type thread_num )
		{
			// データの方が浮動小数点型の場合は高速なバージョンを利用する
			array1< double > a1( 3, in.reso1( ) ), a2( 3, in.reso2( ) ), a3( 3, in.reso3( ) );

			__linear__::compute_gaussian_kernel( a1, a2, a3, sigma );

			return( linear::filter1d( in, a1, a2, f, thread_num ) );
		}

		template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
		static bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const double sigma, Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
		{
			// データの方が浮動小数点型の場合は高速なバージョンを利用する
			array1< double > a1( 3, in.reso1( ) ), a2( 3, in.reso2( ) ), a3( 3, in.reso3( ) );

			__linear__::compute_gaussian_kernel( a1, a2, a3, sigma );

			return( linear::filter1d( in, out, a1, a2, a3, f, thread_num ) );
		}

		template < class T, class Allocator, class Functor >
		static bool filter( array3< T, Allocator > &in, const double sigma, Functor f, typename array3< T, Allocator >::size_type thread_num )
		{
			// データの方が浮動小数点型の場合は高速なバージョンを利用する
			array1< double > a1( 3, in.reso1( ) ), a2( 3, in.reso2( ) ), a3( 3, in.reso3( ) );

			__linear__::compute_gaussian_kernel( a1, a2, a3, sigma );

			return( linear::filter1d( in, in, a1, a2, a3, f, thread_num ) );
		}
	};

	/// @brief ガウシアンフィルタ( array )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数は無視され，常にシングルスレッドで動作する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const double sigma, Functor f, typename array< T1, Allocator1 >::size_type thread_num )
	{
		array< double > a( 3 );

		__linear__::compute_gaussian_kernel( a, sigma );

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
	//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, const double sigma, Functor f, typename array1< T1, Allocator1 >::size_type thread_num )
	{
		array< double > a( 3, in.reso1( ) );

		__linear__::compute_gaussian_kernel( a, sigma );

		return( linear::filter( in, out, a, f, thread_num ) );
	}

	/// @brief ガウシアンフィルタ( array2 )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
	//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
	//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const double sigma, Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		// データの方が浮動小数点型の場合は高速なバージョンを利用する
		return( gaussian_filter_helper< is_float< T2 >::value >::filter( in, out, sigma, f, thread_num ) );
	}

	/// @brief ガウシアンフィルタ( array2 )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
	//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
	//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
	//!
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in,out] in         … 入力・出力配列
	//! @param[in]     sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
	//! @param[in]     f          … コールバック関数
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T, class Allocator, class Functor >
	bool filter( array2< T, Allocator > &in, const double sigma, Functor f, typename array2< T, Allocator >::size_type thread_num )
	{
		// データの方が浮動小数点型の場合は高速なバージョンを利用する
		return( gaussian_filter_helper< is_float< T >::value >::filter( in, sigma, f, thread_num ) );
	}

	/// @brief ガウシアンフィルタ( array3 )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
	//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
	//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力配列
	//! @param[out] out        … 出力配列
	//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
	//! @param[in]  f          … コールバック関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
	bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const double sigma, Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		// データの方が浮動小数点型の場合は高速なバージョンを利用する
		return( gaussian_filter_helper< is_float< T2 >::value >::filter( in, out, sigma, f, thread_num ) );
	}

	/// @brief ガウシアンフィルタ( array3 )
	//! 
	//! 任意サイズのガウシアンフィルタ
	//!
	//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
	//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
	//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
	//!
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in,out] in         … 入力・出力配列
	//! @param[in]     sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
	//! @param[in]     f          … コールバック関数
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	//! @retval true  … フィルタリングに成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合
	//! 
	template < class T, class Allocator, class Functor >
	bool filter( array3< T, Allocator > &in, const double sigma, Functor f, typename array3< T, Allocator >::size_type thread_num )
	{
		// データの方が浮動小数点型の場合は高速なバージョンを利用する
		return( gaussian_filter_helper< is_float< T >::value >::filter( in, sigma, f, thread_num ) );
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
//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, const double sigma = 1.0, typename array< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, sigma, __mist_dmy_callback__( ), thread_num ) );
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
//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, const double sigma = 1.0, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ガウシアンフィルタ( array2 )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const double sigma = 1.0, typename array2< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ガウシアンフィルタ( array2 )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in         … 入力・出力配列
//! @param[in]     sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
//! @param[in]     thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T, class Allocator >
bool gaussian_filter( array2< T, Allocator > &in, const double sigma = 1.0, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ガウシアンフィルタ( array3 )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in]  in         … 入力配列
//! @param[out] out        … 出力配列
//! @param[in]  sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T1, class Allocator1, class T2, class Allocator2 >
bool gaussian_filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const double sigma = 1.0, typename array3< T1, Allocator1 >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, out, sigma, __mist_dmy_callback__( ), thread_num ) );
}

/// @brief ガウシアンフィルタ( array3 )
//! 
//! 任意サイズのガウシアンフィルタ
//!
//! 出力のデータ方が float や double や rgb< float > などの浮動小数点型の場合は，
//! ガウシアンカーネルを1次元分解し，高速にフィルタを計算する．それ以外のデータ型
//! （int等）の場合は各画素ごとにカーネルを適用する（計算コスト大）
//!
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//! 
//! @param[in,out] in         … 入力・出力配列
//! @param[in]     sigma      … フィルタの標準偏差（画素の大きさを考慮した値を指定）
//! @param[in]     thread_num … 使用するスレッド数
//! 
//! @retval true  … フィルタリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class T, class Allocator >
bool gaussian_filter( array3< T, Allocator > &in, const double sigma = 1.0, typename array3< T, Allocator >::size_type thread_num = 0 )
{
	return( gaussian::filter( in, sigma, __mist_dmy_callback__( ), thread_num ) );
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
	template < bool b >
	struct average_thread_helper
	{
		template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
		static bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
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
	};

	template < >
	struct average_thread_helper< true >
	{
		template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
		static bool filter( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
							   typename array2< T1, Allocator1 >::size_type fw, typename array2< T1, Allocator1 >::size_type fh,
							   Functor f, typename array2< T1, Allocator1 >::size_type thread_num )
		{
			if( in.empty( ) )
			{
				return( false );
			}

			typedef typename array2< T1, Allocator1 >::size_type  size_type;
			typedef __linear__::_1D_average_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 >, Functor > average_thread;

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

		template < class T1, class Allocator1, class T2, class Allocator2, class Functor >
		bool filter( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					   typename array3< T1, Allocator1 >::size_type fw, typename array3< T1, Allocator1 >::size_type fh, typename array3< T1, Allocator1 >::size_type fd,
					   Functor f, typename array3< T1, Allocator1 >::size_type thread_num )
		{
			if( in.empty( ) )
			{
				return( false );
			}

			typedef typename array3< T1, Allocator1 >::size_type  size_type;
			typedef __linear__::_1D_average_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 >, Functor > average_thread;

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
	};

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
		return( average_thread_helper< is_float< T2 >::value >::filter( in, out, fw, fh, f, thread_num ) );
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
		return( average_thread_helper< is_float< T2 >::value >::filter( in, out, fw, fh, fd, f, thread_num ) );
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
