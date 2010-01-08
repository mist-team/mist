// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
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

/// @file mist/interpolate.h
//!
//! @brief 各次元の画像に対応した，画像補間ライブラリ
//!
#ifndef __INCLUDE_MIST_INTERPOLATE__
#define __INCLUDE_MIST_INTERPOLATE__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "thread.h"
#endif


#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN

// 補間関数で利用するユーティリティー関数群
namespace __interpolate_utility__
{
	// 浮動小数の場合は四捨五入しない
	template < bool b >
	struct _round_
	{
		template < class T >
		static void convert( double in, T &out )
		{
			out = static_cast< T >( in );
		}

		template < class T >
		static void convert( const rgb< double > &in, rgb< T > &out )
		{
			out.r = static_cast< T >( in.r );
			out.g = static_cast< T >( in.g );
			out.b = static_cast< T >( in.b );
		}

		template < class T >
		static void convert( const rgb< double > &in, rgba< T > &out )
		{
			out.r = static_cast< T >( in.r );
			out.g = static_cast< T >( in.g );
			out.b = static_cast< T >( in.b );
		}

		template < class T >
		static void convert( const bgr< double > &in, bgr< T > &out )
		{
			out.r = static_cast< T >( in.r );
			out.g = static_cast< T >( in.g );
			out.b = static_cast< T >( in.b );
		}

		template < class T >
		static void convert( const bgr< double > &in, bgra< T > &out )
		{
			out.r = static_cast< T >( in.r );
			out.g = static_cast< T >( in.g );
			out.b = static_cast< T >( in.b );
		}
	};

	// 整数の場合は四捨五入する
	template < >
	struct _round_< true >
	{
		template < class T >
		static void convert( double in, T &out )
		{
			out = static_cast< T >( in + 0.5 );
		}

		template < class T >
		static void convert( const rgb< double > &in, rgb< T > &out )
		{
			out.r = static_cast< T >( in.r + 0.5 );
			out.g = static_cast< T >( in.g + 0.5 );
			out.b = static_cast< T >( in.b + 0.5 );
		}

		template < class T >
		static void convert( const rgb< double > &in, rgba< T > &out )
		{
			out.r = static_cast< T >( in.r + 0.5 );
			out.g = static_cast< T >( in.g + 0.5 );
			out.b = static_cast< T >( in.b + 0.5 );
		}

		template < class T >
		static void convert( const bgr< double > &in, bgr< T > &out )
		{
			out.r = static_cast< T >( in.r + 0.5 );
			out.g = static_cast< T >( in.g + 0.5 );
			out.b = static_cast< T >( in.b + 0.5 );
		}

		template < class T >
		static void convert( const bgr< double > &in, bgra< T > &out )
		{
			out.r = static_cast< T >( in.r + 0.5 );
			out.g = static_cast< T >( in.g + 0.5 );
			out.b = static_cast< T >( in.b + 0.5 );
		}
	};

	template < class T >
	inline void round( const double in, T &out )
	{
		_round_< is_integer< T >::value >::convert( in, out );
	}

	template < class T >
	inline void round( const rgb< double > &in, rgb< T > &out )
	{
		_round_< is_integer< T >::value >::convert( in, out );
	}

	template < class T >
	inline void round( const rgb< double > &in, rgba< T > &out )
	{
		_round_< is_integer< T >::value >::convert( in, out );
	}

	template < class T >
	inline void round( const bgr< double > &in, bgr< T > &out )
	{
		_round_< is_integer< T >::value >::convert( in, out );
	}

	template < class T >
	inline void round( const bgr< double > &in, bgra< T > &out )
	{
		_round_< is_integer< T >::value >::convert( in, out );
	}
}

// 最近傍型補間
namespace __nearest__
{
	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
						typename Array1::size_type thread_idx, typename Array1::size_type thread_numx,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array2::value_type out_value_type;

		size_type i, j, k;
		size_type iw = in.width( );
		size_type ih = in.height( );
		size_type id = in.depth( );
		size_type ow = out.width( );
		size_type oh = out.height( );
		size_type od = out.depth( );

		double sx = static_cast< double >( iw ) / static_cast< double >( ow );
		double sy = static_cast< double >( ih ) / static_cast< double >( oh );
		double sz = static_cast< double >( id ) / static_cast< double >( od );
		size_type x, y, z;

		for( k = thread_idz ; k < od ; k += thread_numz )
		{
			z = static_cast< size_type >( sz * k + 0.5 );
			z = z < id ? z : id - 1;
			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				y = static_cast< size_type >( sy * j + 0.5 );
				y = y < ih ? y : ih - 1;
				for( i = thread_idx ; i < ow ; i += thread_numx )
				{
					x = static_cast< size_type >( sx * i + 0.5 );
					x = x < iw ? x : iw - 1;

					__interpolate_utility__::round( in( x, y, z ), out( i, j, k ) );
				}
			}
		}
	}
}


// 平均値型補間
namespace __mean__
{
	template < bool b >
	struct _mean_
	{
		template < class T, class Allocator >
		static double mean___( const array< T, Allocator > &in,
									typename array< T, Allocator >::size_type i1,
									typename array< T, Allocator >::size_type i2,
									typename array< T, Allocator >::size_type j1,
									typename array< T, Allocator >::size_type j2,
									typename array< T, Allocator >::size_type k1,
									typename array< T, Allocator >::size_type k2,
									double xs,
									double xe,
									double ys,
									double ye,
									double zs,
									double ze )
		{
			typedef typename array< T, Allocator >::value_type value_type;
			typedef typename array< T, Allocator >::size_type  size_type;
			double pix = 0.0;

			{
				double a = ( i1 + 1 - xs ) * 0.5;
				pix += ( in[ i1 ] + in[ i1 + 1 ] ) * a;
			}

			for( size_type i = i1 + 1 ; i < i2 - 1 ; i++ )
			{
				pix += ( in[ i ] + in[ i + 1 ] ) * 0.5;
			}

			{
				double a = ( xe - i2 + 1 ) * 0.5;
				pix += ( in[ i2 - 1 ] + in[ i2 ] ) * a;
			}

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix /= ( xe - xs ) * ( ye - ys ) * ( ze - zs );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}

		template < class T, class Allocator >
		static double mean___( const array2< T, Allocator > &in,
									typename array2< T, Allocator >::size_type i1,
									typename array2< T, Allocator >::size_type i2,
									typename array2< T, Allocator >::size_type j1,
									typename array2< T, Allocator >::size_type j2,
									typename array2< T, Allocator >::size_type k1,
									typename array2< T, Allocator >::size_type k2,
									double xs,
									double xe,
									double ys,
									double ye,
									double zs,
									double ze )
		{
			typedef typename array2< T, Allocator >::value_type    value_type;
			typedef typename array2< T, Allocator >::size_type     size_type;
			typedef typename array2< T, Allocator >::const_pointer const_pointer;
			double pix = 0.0;
			double yy;

			if( i2 - i1 < 3 )
			{
				double xx[ 2 ] = { ( i1 + 1 - xs ) * 0.25, ( xe - i2 + 1 ) * 0.25 };
				for( size_type j = j1 ; j < j2 ; j++ )
				{
					if( j == j1 )
					{
						yy = j1 + 1 - ys;
					}
					else if( j == j2 - 1 )
					{
						yy = ye - j;
					}
					else
					{
						yy = 1.0;
					}

					double aa[ 2 ] = { xx[ 0 ] * yy, xx[ 1 ] * yy };

					const_pointer p0 = &in( 0, j     );
					const_pointer p1 = &in( 0, j + 1 );

					pix += ( p0[ i1 ] + p1[ i1 ] + p0[ i1 + 1 ] + p1[ i1 + 1 ] ) * aa[ 0 ];
					pix += ( p0[ i2 - 1 ] + p1[ i2 - 1 ] + p0[ i2 ] + p1[ i2 ] ) * aa[ 1 ];
				}
			}
			else
			{
				double xx[ 3 ] = { ( i1 + 1 - xs ) * 0.25, 0.25, ( xe - i2 + 1 ) * 0.25 };
				for( size_type j = j1 ; j < j2 ; j++ )
				{
					if( j == j1 )
					{
						yy = j1 + 1 - ys;
					}
					else if( j == j2 - 1 )
					{
						yy = ye - j;
					}
					else
					{
						yy = 1.0;
					}

					double aa[ 3 ] = { xx[ 0 ] * yy, xx[ 1 ] * yy, xx[ 2 ] * yy };

					const_pointer p0 = &in( 0, j     );
					const_pointer p1 = &in( 0, j + 1 );
					double ppp;
					size_type i = i1;

					{
						pix += ( p0[ i ] + p1[ i ] + p0[ i + 1 ] + p1[ i + 1 ] ) * aa[ 0 ];
						ppp  = p0[ i + 1 ] + p1[ i + 1 ];
					}

					for( i = i1 + 2 ; i < i2 - 1 ; i++ )
					{
						ppp += ( p0[ i ] + p1[ i ] ) * 2.0;
					}

					{
						ppp += p0[ i2 - 1 ] + p1[ i2 - 1 ];
						pix += ppp * aa[ 1 ];
						pix += ( p0[ i2 - 1 ] + p1[ i2 - 1 ] + p0[ i2 ] + p1[ i2 ] ) * aa[ 2 ];
					}
				}
			}

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix /= ( xe - xs ) * ( ye - ys ) * ( ze - zs );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}

		template < class T, class Allocator >
		static double mean___( const array3< T, Allocator > &in,
										typename array3< T, Allocator >::size_type i1,
										typename array3< T, Allocator >::size_type i2,
										typename array3< T, Allocator >::size_type j1,
										typename array3< T, Allocator >::size_type j2,
										typename array3< T, Allocator >::size_type k1,
										typename array3< T, Allocator >::size_type k2,
										double xs,
										double xe,
										double ys,
										double ye,
										double zs,
										double ze )
		{
			typedef typename array3< T, Allocator >::value_type    value_type;
			typedef typename array3< T, Allocator >::size_type     size_type;
			typedef typename array3< T, Allocator >::const_pointer const_pointer;
			double pix = 0.0;
			double xx[ 3 ] = { ( i1 + 1 - xs ) * 0.125, i2 - i1 < 3 ? 0.0 : 1.0 * 0.125, ( xe - i2 + 1 ) * 0.125 };
			double yy, zz;

			for( size_type k = k1 ; k < k2 ; k++ )
			{
				if( k == k1 )
				{
					zz = k1 + 1 - zs;
				}
				else if( k == k2 - 1 )
				{
					zz = ze - k;
				}
				else
				{
					zz = 1.0;
				}

				for( size_type j = j1 ; j < j2 ; j++ )
				{
					if( j == j1 )
					{
						yy = j1 + 1 - ys;
					}
					else if( j == j2 - 1 )
					{
						yy = ye - j;
					}
					else
					{
						yy = 1.0;
					}

					double aa[ 3 ] = { xx[ 0 ] * yy * zz, xx[ 1 ] * yy * zz, xx[ 2 ] * yy * zz };

					const_pointer p0 = &in( 0, j    , k     );
					const_pointer p1 = &in( 0, j + 1, k     );
					const_pointer p2 = &in( 0, j    , k + 1 );
					const_pointer p3 = &in( 0, j + 1, k + 1 );
					double ppp = 0.0;

					{
						const value_type &c1 = p0[ i1 ];
						const value_type &c2 = p1[ i1 ];
						const value_type &c3 = p2[ i1 ];
						const value_type &c4 = p3[ i1 ];
						const value_type &c5 = p0[ i1 + 1 ];
						const value_type &c6 = p1[ i1 + 1 ];
						const value_type &c7 = p2[ i1 + 1 ];
						const value_type &c8 = p3[ i1 + 1 ];

						pix += ( c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 ) * aa[ 0 ];
						ppp += c5 + c6 + c7 + c8;
					}

					for( size_type i = i1 + 2 ; i < i2 - 1 ; i++ )
					{
						const value_type &c1 = p0[ i ];
						const value_type &c2 = p1[ i ];
						const value_type &c3 = p2[ i ];
						const value_type &c4 = p3[ i ];

						ppp += ( c1 + c2 + c3 + c4 ) * 2.0;
					}

					{
						const value_type &c1 = p0[ i2 - 1 ];
						const value_type &c2 = p1[ i2 - 1 ];
						const value_type &c3 = p2[ i2 - 1 ];
						const value_type &c4 = p3[ i2 - 1 ];
						const value_type &c5 = p0[ i2 ];
						const value_type &c6 = p1[ i2 ];
						const value_type &c7 = p2[ i2 ];
						const value_type &c8 = p3[ i2 ];

						ppp += c1 + c2 + c3 + c4;
						pix += ppp * aa[ 1 ];
						pix += ( c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 ) * aa[ 2 ];
					}
				}
			}

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix /= ( xe - xs ) * ( ye - ys ) * ( ze - zs );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}
	};

	template < >
	struct _mean_< true >
	{
		template < class T, class Allocator >
		static const typename T::template rebind< double >::other mean___( const array< T, Allocator > &in,
												typename array< T, Allocator >::size_type i1,
												typename array< T, Allocator >::size_type i2,
												typename array< T, Allocator >::size_type /* j1 */,
												typename array< T, Allocator >::size_type /* j2 */,
												typename array< T, Allocator >::size_type /* k1 */,
												typename array< T, Allocator >::size_type /* k2 */,
												double xs,
												double xe,
												double ys,
												double ye,
												double zs,
												double ze )
		{
			typedef typename array< T, Allocator >::value_type color;
			typedef typename array< T, Allocator >::size_type size_type;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;
			double r = 0.0;
			double g = 0.0;
			double b = 0.0;

			{
				double rr = 0.0;
				double gg = 0.0;
				double bb = 0.0;

				const color &c1 = in[ i1     ];
				const color &c2 = in[ i1 + 1 ];

				rr += c1.r; gg += c1.g; bb += c1.b;
				rr += c2.r; gg += c2.g; bb += c2.b;

				double a = ( i1 + 1 - xs ) * 0.5;
				r += rr * a;
				g += gg * a;
				b += bb * a;
			}

			for( size_type i = i1 + 1 ; i < i2 - 1 ; i++ )
			{
				double rr = 0.0;
				double gg = 0.0;
				double bb = 0.0;

				const color &c1 = in[ i     ];
				const color &c2 = in[ i + 1 ];

				rr += c1.r; gg += c1.g; bb += c1.b;
				rr += c2.r; gg += c2.g; bb += c2.b;

				double a = 0.5;
				r += rr * a;
				g += gg * a;
				b += bb * a;
			}

			{
				double rr = 0.0;
				double gg = 0.0;
				double bb = 0.0;

				const color &c1 = in[ i2 - 1 ];
				const color &c2 = in[ i2     ];

				rr += c1.r; gg += c1.g; bb += c1.b;
				rr += c2.r; gg += c2.g; bb += c2.b;

				double a = ( xe - i2 + 1 ) * 0.5;
				r += rr * a;
				g += gg * a;
				b += bb * a;
			}

			double numPixels = ( xe - xs ) * ( ye - ys ) * ( ze - zs );
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			r /= numPixels;
			r = r > min ? r : min;
			r = r < max ? r : max;
			g /= numPixels;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b /= numPixels;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other mean___( const array2< T, Allocator > &in,
												typename array2< T, Allocator >::size_type i1,
												typename array2< T, Allocator >::size_type i2,
												typename array2< T, Allocator >::size_type j1,
												typename array2< T, Allocator >::size_type j2,
												typename array2< T, Allocator >::size_type /* k1 */,
												typename array2< T, Allocator >::size_type /* k2 */,
												double xs,
												double xe,
												double ys,
												double ye,
												double zs,
												double ze )
		{
			typedef typename array2< T, Allocator >::value_type    color;
			typedef typename array2< T, Allocator >::size_type     size_type;
			typedef typename array2< T, Allocator >::const_pointer const_pointer;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;
			double r = 0.0;
			double g = 0.0;
			double b = 0.0;
			double yy;

			if( i2 - i1 < 3 )
			{
				double xx[ 2 ] = { ( i1 + 1 - xs ) * 0.25, ( xe - i2 + 1 ) * 0.25 };
				for( size_type j = j1 ; j < j2 ; j++ )
				{
					if( j == j1 )
					{
						yy = j1 + 1 - ys;
					}
					else if( j == j2 - 1 )
					{
						yy = ye - j;
					}
					else
					{
						yy = 1.0;
					}

					double aa[ 2 ] = { xx[ 0 ] * yy, xx[ 1 ] * yy };

					const_pointer p0 = &in( 0, j     );
					const_pointer p1 = &in( 0, j + 1 );

					{
						const color &c1 = p0[ i1 ];
						const color &c2 = p1[ i1 ];
						const color &c3 = p0[ i1 + 1 ];
						const color &c4 = p1[ i2 - 1 ];
						const color &c5 = p0[ i2 ];
						const color &c6 = p1[ i2 ];

						r += ( c1.r + c2.r + c3.r + c4.r ) * aa[ 0 ];
						g += ( c1.g + c2.g + c3.g + c4.g ) * aa[ 0 ];
						b += ( c1.b + c2.b + c3.b + c4.b ) * aa[ 0 ];

						r += ( c3.r + c4.r + c5.r + c6.r ) * aa[ 1 ];
						g += ( c3.g + c4.g + c5.g + c6.g ) * aa[ 1 ];
						b += ( c3.b + c4.b + c5.b + c6.b ) * aa[ 1 ];
					}
				}
			}
			else
			{
				double xx[ 3 ] = { ( i1 + 1 - xs ) * 0.25, 0.25, ( xe - i2 + 1 ) * 0.25 };
				for( size_type j = j1 ; j < j2 ; j++ )
				{
					if( j == j1 )
					{
						yy = j1 + 1 - ys;
					}
					else if( j == j2 - 1 )
					{
						yy = ye - j;
					}
					else
					{
						yy = 1.0;
					}

					double aa[ 3 ] = { xx[ 0 ] * yy, xx[ 1 ] * yy, xx[ 2 ] * yy };

					const_pointer p0 = &in( 0, j     );
					const_pointer p1 = &in( 0, j + 1 );
					double rr, gg, bb;

					{
						const color &c1 = p0[ i1 ];
						const color &c2 = p1[ i1 ];
						const color &c3 = p0[ i1 + 1 ];
						const color &c4 = p1[ i1 + 1 ];

						r += ( c1.r + c2.r + c3.r + c4.r ) * aa[ 0 ];
						g += ( c1.g + c2.g + c3.g + c4.g ) * aa[ 0 ];
						b += ( c1.b + c2.b + c3.b + c4.b ) * aa[ 0 ];

						rr = c3.r + c4.r;
						gg = c3.g + c4.g;
						bb = c3.b + c4.b;
					}

					for( size_type i = i1 + 2 ; i < i2 - 1 ; i++ )
					{
						const color &c1 = p0[ i ];
						const color &c2 = p1[ i ];

						rr += ( c1.r + c2.r ) * 2.0;
						gg += ( c1.g + c2.g ) * 2.0;
						bb += ( c1.b + c2.b ) * 2.0;
					}

					{
						const color &c1 = p0[ i2 - 1 ];
						const color &c2 = p1[ i2 - 1 ];
						const color &c3 = p0[ i2 ];
						const color &c4 = p1[ i2 ];

						rr += c1.r + c2.r;
						gg += c1.g + c2.g;
						bb += c1.b + c2.b;

						r += rr * aa[ 1 ];
						g += gg * aa[ 1 ];
						b += bb * aa[ 1 ];

						r += ( c1.r + c2.r + c3.r + c4.r ) * aa[ 2 ];
						g += ( c1.g + c2.g + c3.g + c4.g ) * aa[ 2 ];
						b += ( c1.b + c2.b + c3.b + c4.b ) * aa[ 2 ];
					}
				}
			}

			double numPixels = ( xe - xs ) * ( ye - ys ) * ( ze - zs );
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			r /= numPixels;
			r = r > min ? r : min;
			r = r < max ? r : max;
			g /= numPixels;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b /= numPixels;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other mean___( const array3< T, Allocator > &in,
												typename array3< T, Allocator >::size_type i1,
												typename array3< T, Allocator >::size_type i2,
												typename array3< T, Allocator >::size_type j1,
												typename array3< T, Allocator >::size_type j2,
												typename array3< T, Allocator >::size_type k1,
												typename array3< T, Allocator >::size_type k2,
												double xs,
												double xe,
												double ys,
												double ye,
												double zs,
												double ze )
		{
			typedef typename array3< T, Allocator >::value_type    color;
			typedef typename array3< T, Allocator >::size_type     size_type;
			typedef typename array3< T, Allocator >::const_pointer const_pointer;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;
			double r = 0.0;
			double g = 0.0;
			double b = 0.0;
			double yy, zz;
			double xx[ 3 ] = { ( i1 + 1 - xs ) * 0.125, i2 - i1 < 3 ? 0.0 : 1.0 * 0.125, ( xe - i2 + 1 ) * 0.125 };

			for( size_type k = k1 ; k < k2 ; k++ )
			{
				if( k == k1 )
				{
					zz = k1 + 1 - zs;
				}
				else if( k == k2 - 1 )
				{
					zz = ze - k;
				}
				else
				{
					zz = 1.0;
				}

				for( size_type j = j1 ; j < j2 ; j++ )
				{
					if( j == j1 )
					{
						yy = j1 + 1 - ys;
					}
					else if( j == j2 - 1 )
					{
						yy = ye - j;
					}
					else
					{
						yy = 1.0;
					}

					double aa[ 3 ] = { xx[ 0 ] * yy * zz, xx[ 1 ] * yy * zz, xx[ 2 ] * yy * zz };

					const_pointer p0 = &in( 0, j    , k     );
					const_pointer p1 = &in( 0, j + 1, k     );
					const_pointer p2 = &in( 0, j    , k + 1 );
					const_pointer p3 = &in( 0, j + 1, k + 1 );
					double rr = 0.0;
					double gg = 0.0;
					double bb = 0.0;

					{
						const color &c1 = p0[ i1 ];
						const color &c2 = p1[ i1 ];
						const color &c3 = p2[ i1 ];
						const color &c4 = p3[ i1 ];
						const color &c5 = p0[ i1 + 1 ];
						const color &c6 = p1[ i1 + 1 ];
						const color &c7 = p2[ i1 + 1 ];
						const color &c8 = p3[ i1 + 1 ];

						r += ( c1.r + c2.r + c3.r + c4.r + c5.r + c6.r + c7.r + c8.r ) * aa[ 0 ];
						g += ( c1.g + c2.g + c3.g + c4.g + c5.g + c6.g + c7.g + c8.g ) * aa[ 0 ];
						b += ( c1.b + c2.b + c3.b + c4.b + c5.b + c6.b + c7.b + c8.b ) * aa[ 0 ];

						rr += c5.r + c6.r + c7.r + c8.r;
						gg += c5.g + c6.g + c7.g + c8.g;
						bb += c5.b + c6.b + c7.b + c8.b;
					}

					for( size_type i = i1 + 2 ; i < i2 - 1 ; i++ )
					{
						const color &c1 = p0[ i ];
						const color &c2 = p1[ i ];
						const color &c3 = p2[ i ];
						const color &c4 = p3[ i ];

						rr += ( c1.r + c2.r + c3.r + c4.r ) * 2.0;
						gg += ( c1.g + c2.g + c3.g + c4.g ) * 2.0;
						bb += ( c1.b + c2.b + c3.b + c4.b ) * 2.0;
					}

					{
						const color &c1 = p0[ i2 - 1 ];
						const color &c2 = p1[ i2 - 1 ];
						const color &c3 = p2[ i2 - 1 ];
						const color &c4 = p3[ i2 - 1 ];
						const color &c5 = p0[ i2     ];
						const color &c6 = p1[ i2     ];
						const color &c7 = p2[ i2     ];
						const color &c8 = p3[ i2     ];

						rr += c1.r + c2.r + c3.r + c4.r;
						gg += c1.g + c2.g + c3.g + c4.g;
						bb += c1.b + c2.b + c3.b + c4.b;

						r += rr * aa[ 1 ];
						g += gg * aa[ 1 ];
						b += bb * aa[ 1 ];

						r += ( c1.r + c2.r + c3.r + c4.r + c5.r + c6.r + c7.r + c8.r ) * aa[ 2 ];
						g += ( c1.g + c2.g + c3.g + c4.g + c5.g + c6.g + c7.g + c8.g ) * aa[ 2 ];
						b += ( c1.b + c2.b + c3.b + c4.b + c5.b + c6.b + c7.b + c8.b ) * aa[ 2 ];
					}
				}
			}

			double numPixels = ( xe - xs ) * ( ye - ys ) * ( ze - zs );
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			r /= numPixels;
			r = r > min ? r : min;
			r = r < max ? r : max;
			g /= numPixels;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b /= numPixels;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( ovalue_type( r, g, b ) );
		}
	};

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type thread_idx, typename array< T1, Allocator1 >::size_type thread_numx,
						typename array< T1, Allocator1 >::size_type thread_idy, typename array< T1, Allocator1 >::size_type thread_numy,
						typename array< T1, Allocator1 >::size_type thread_idz, typename array< T1, Allocator1 >::size_type thread_numz )
	{
		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef typename array< T1, Allocator1 >::value_type value_type;
		typedef typename array< T2, Allocator2 >::value_type out_value_type;

		size_type i, i1, i2;
		size_type iw = in.width( );
		size_type ow = out.width( );

		double sx = static_cast< double >( iw - 1 ) / static_cast< double >( ow );

		for( i = thread_idx ; i < ow ; i += thread_numx )
		{
			double xs = sx * i;
			double xe = xs + sx;

			i1 = static_cast< size_type >( xs );
			i2 = static_cast< size_type >( xe );
			i2 = xe > i2 ? i2 + 1 : i2;
			i2 = i2 < iw - 1 ? i2 : iw - 1;

			__interpolate_utility__::round( _mean_< is_color< value_type >::value >::mean___( in, i1, i2, 0, 1, 0, 1, xs, xe, 0, 1, 0, 1 ), out[ i ] );
		}
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
						typename array2< T1, Allocator1 >::size_type thread_idx, typename array2< T1, Allocator1 >::size_type thread_numx,
						typename array2< T1, Allocator1 >::size_type thread_idy, typename array2< T1, Allocator1 >::size_type thread_numy,
						typename array2< T1, Allocator1 >::size_type /* thread_idz */, typename array2< T1, Allocator1 >::size_type /* thread_numz */ )
	{
		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef typename array2< T1, Allocator1 >::value_type value_type;
		typedef typename array2< T2, Allocator2 >::value_type out_value_type;

		size_type i, j, i1, i2, j1, j2;
		size_type iw = in.width( );
		size_type ih = in.height( );
		size_type ow = out.width( );
		size_type oh = out.height( );

		double sx = static_cast< double >( iw - 1 ) / static_cast< double >( ow );
		double sy = static_cast< double >( ih - 1 ) / static_cast< double >( oh );

		for( j = thread_idy ; j < oh ; j += thread_numy )
		{
			double ys = sy * j;
			double ye = ys + sy;

			j1 = static_cast< size_type >( ys );
			j2 = static_cast< size_type >( ye );
			j2 = ye > j2 ? j2 + 1 : j2;
			j2 = j2 < ih - 1 ? j2 : ih - 1;

			for( i = thread_idx ; i < ow ; i += thread_numx )
			{
				double xs = sx * i;
				double xe = xs + sx;

				i1 = static_cast< size_type >( xs );
				i2 = static_cast< size_type >( xe );
				i2 = xe > i2 ? i2 + 1 : i2;
				i2 = i2 < iw - 1 ? i2 : iw - 1;

				__interpolate_utility__::round( _mean_< is_color< value_type >::value >::mean___( in, i1, i2, j1, j2, 0, 1, xs, xe, ys, ye, 0, 1 ), out( i, j ) );
			}
		}
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
						typename array3< T1, Allocator1 >::size_type thread_idx, typename array3< T1, Allocator1 >::size_type thread_numx,
						typename array3< T1, Allocator1 >::size_type thread_idy, typename array3< T1, Allocator1 >::size_type thread_numy,
						typename array3< T1, Allocator1 >::size_type thread_idz, typename array3< T1, Allocator1 >::size_type thread_numz )
	{
		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef typename array3< T1, Allocator1 >::value_type value_type;
		typedef typename array3< T2, Allocator2 >::value_type out_value_type;

		size_type i, j, k, i1, i2, j1, j2, k1, k2;
		size_type iw = in.width( );
		size_type ih = in.height( );
		size_type id = in.depth( );
		size_type ow = out.width( );
		size_type oh = out.height( );
		size_type od = out.depth( );

		double sx = static_cast< double >( iw - 1 ) / static_cast< double >( ow );
		double sy = static_cast< double >( ih - 1 ) / static_cast< double >( oh );
		double sz = static_cast< double >( id - 1 ) / static_cast< double >( od );

		for( k = thread_idz ; k < od ; k += thread_numz )
		{
			double zs = sz * k;
			double ze = zs + sz;

			k1 = static_cast< size_type >( zs );
			k2 = static_cast< size_type >( ze );
			k2 = ze > k2 ? k2 + 1 : k2;
			k2 = k2 < id - 1 ? k2 : id - 1;

			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				double ys = sy * j;
				double ye = ys + sy;

				j1 = static_cast< size_type >( ys );
				j2 = static_cast< size_type >( ye );
				j2 = ye > j2 ? j2 + 1 : j2;
				j2 = j2 < ih - 1 ? j2 : ih - 1;

				for( i = thread_idx ; i < ow ; i += thread_numx )
				{
					double xs = sx * i;
					double xe = xs + sx;

					i1 = static_cast< size_type >( xs );
					i2 = static_cast< size_type >( xe );
					i2 = xe > i2 ? i2 + 1 : i2;
					i2 = i2 < iw - 1 ? i2 : iw - 1;

					__interpolate_utility__::round( _mean_< is_color< value_type >::value >::mean___( in, i1, i2, j1, j2, k1, k2, xs, xe, ys, ye, zs, ze ), out( i, j, k ) );
				}
			}
		}
	}
}

// 線形補間
namespace __linear__
{
	template < bool b >
	struct _linear_
	{
		template < class T, class Allocator >
		static double interpolate( const array< T, Allocator > &in,
									typename array< T, Allocator >::size_type i1,
									typename array< T, Allocator >::size_type i2,
									typename array< T, Allocator >::size_type j1,
									typename array< T, Allocator >::size_type j2,
									typename array< T, Allocator >::size_type k1,
									typename array< T, Allocator >::size_type k2,
									double x, double y, double z )
		{
			typedef typename array< T, Allocator >::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double pix = in[ i1 ] * ( 1.0 - x ) + in[ i2 ] * x;
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array2< T, Allocator > &in,
									typename array2< T, Allocator >::size_type i1,
									typename array2< T, Allocator >::size_type i2,
									typename array2< T, Allocator >::size_type j1,
									typename array2< T, Allocator >::size_type j2,
									typename array2< T, Allocator >::size_type k1,
									typename array2< T, Allocator >::size_type k2,
									double x, double y, double z )
		{
			typedef typename array2< T, Allocator >::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double pix = ( in( i1, j1 ) * ( 1.0 - x ) + in( i2, j1 ) * x ) * ( 1.0 - y ) + ( in( i1, j2 ) * ( 1.0 - x ) + in( i2, j2 ) * x ) * y;
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array3< T, Allocator > &in,
									typename array3< T, Allocator >::size_type i1,
									typename array3< T, Allocator >::size_type i2,
									typename array3< T, Allocator >::size_type j1,
									typename array3< T, Allocator >::size_type j2,
									typename array3< T, Allocator >::size_type k1,
									typename array3< T, Allocator >::size_type k2,
									double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double pix = ( ( in( i1, j1, k1 ) * ( 1.0 - x ) + in( i2, j1, k1 ) * x ) * ( 1.0 - y ) + ( in( i1, j2, k1 ) * ( 1.0 - x ) + in( i2, j2, k1 ) * x ) * y ) * ( 1.0 - z )
								+ ( ( in( i1, j1, k2 ) * ( 1.0 - x ) + in( i2, j1, k2 ) * x ) * ( 1.0 - y ) + ( in( i1, j2, k2 ) * ( 1.0 - x ) + in( i2, j2, k2 ) * x ) * y ) * z;
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}
	};

	template < >
	struct _linear_< true >
	{
		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array< T, Allocator > &in,
												typename array< T, Allocator >::size_type i1,
												typename array< T, Allocator >::size_type i2,
												typename array< T, Allocator >::size_type /* j1 */,
												typename array< T, Allocator >::size_type /* j2 */,
												typename array< T, Allocator >::size_type /* k1 */,
												typename array< T, Allocator >::size_type /* k2 */,
												double x, double /* y */, double /* z */ )
		{
			typedef typename array< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double r = in[ i1 ].r * ( 1.0 - x ) + in[ i2 ].r * x;
			double g = in[ i1 ].g * ( 1.0 - x ) + in[ i2 ].g * x;
			double b = in[ i1 ].b * ( 1.0 - x ) + in[ i2 ].b * x;
			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array2< T, Allocator > &in,
												typename array2< T, Allocator >::size_type i1,
												typename array2< T, Allocator >::size_type i2,
												typename array2< T, Allocator >::size_type j1,
												typename array2< T, Allocator >::size_type j2,
												typename array2< T, Allocator >::size_type /* k1 */,
												typename array2< T, Allocator >::size_type /* k2 */,
												double x, double y, double /* z */ )
		{
			typedef typename array2< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double r = ( in( i1, j1 ).r * ( 1.0 - x ) + in( i2, j1 ).r * x ) * ( 1.0 - y ) + ( in( i1, j2 ).r * ( 1.0 - x ) + in( i2, j2 ).r * x ) * y;
			double g = ( in( i1, j1 ).g * ( 1.0 - x ) + in( i2, j1 ).g * x ) * ( 1.0 - y ) + ( in( i1, j2 ).g * ( 1.0 - x ) + in( i2, j2 ).g * x ) * y;
			double b = ( in( i1, j1 ).b * ( 1.0 - x ) + in( i2, j1 ).b * x ) * ( 1.0 - y ) + ( in( i1, j2 ).b * ( 1.0 - x ) + in( i2, j2 ).b * x ) * y;
			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array3< T, Allocator > &in,
												typename array3< T, Allocator >::size_type i1,
												typename array3< T, Allocator >::size_type i2,
												typename array3< T, Allocator >::size_type j1,
												typename array3< T, Allocator >::size_type j2,
												typename array3< T, Allocator >::size_type k1,
												typename array3< T, Allocator >::size_type k2,
												double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double r = ( ( in( i1, j1, k1 ).r * ( 1.0 - x ) + in( i2, j1, k1 ).r * x ) * ( 1.0 - y ) + ( in( i1, j2, k1 ).r * ( 1.0 - x ) + in( i2, j2, k1 ).r * x ) * y ) * ( 1.0 - z )
								+ ( ( in( i1, j1, k2 ).r * ( 1.0 - x ) + in( i2, j1, k2 ).r * x ) * ( 1.0 - y )	+ ( in( i1, j2, k2 ).r * ( 1.0 - x ) + in( i2, j2, k2 ).r * x ) * y ) * z;
			double g = ( ( in( i1, j1, k1 ).g * ( 1.0 - x ) + in( i2, j1, k1 ).g * x ) * ( 1.0 - y ) + ( in( i1, j2, k1 ).g * ( 1.0 - x ) + in( i2, j2, k1 ).g * x ) * y ) * ( 1.0 - z )
								+ ( ( in( i1, j1, k2 ).g * ( 1.0 - x ) + in( i2, j1, k2 ).g * x ) * ( 1.0 - y )	+ ( in( i1, j2, k2 ).g * ( 1.0 - x ) + in( i2, j2, k2 ).g * x ) * y ) * z;
			double b = ( ( in( i1, j1, k1 ).b * ( 1.0 - x ) + in( i2, j1, k1 ).b * x ) * ( 1.0 - y ) + ( in( i1, j2, k1 ).b * ( 1.0 - x ) + in( i2, j2, k1 ).b * x ) * y ) * ( 1.0 - z )
								+ ( ( in( i1, j1, k2 ).b * ( 1.0 - x ) + in( i2, j1, k2 ).b * x ) * ( 1.0 - y )	+ ( in( i1, j2, k2 ).b * ( 1.0 - x ) + in( i2, j2, k2 ).b * x ) * y ) * z;
			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( ovalue_type( r, g, b ) );
		}
	};

	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
						typename Array1::size_type thread_idx, typename Array1::size_type thread_numx,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array2::value_type out_value_type;

		size_type i, j, k, i1, i2, j1, j2, k1, k2;
		size_type iw = in.width( );
		size_type ih = in.height( );
		size_type id = in.depth( );
		size_type ow = out.width( );
		size_type oh = out.height( );
		size_type od = out.depth( );

		double sx = static_cast< double >( iw ) / static_cast< double >( ow );
		double sy = static_cast< double >( ih ) / static_cast< double >( oh );
		double sz = static_cast< double >( id ) / static_cast< double >( od );
		double x, y, z;

		for( k = thread_idz ; k < od ; k += thread_numz )
		{
			z = sz * k;
			k1 = static_cast< size_type >( z );
			z -= k1;
			k2 = k1 < id - 1 ? k1 + 1 : k1;
			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				y = sy * j;
				j1 = static_cast< size_type >( y );
				y -= j1;
				j2 = j1 < ih - 1 ? j1 + 1 : j1;
				for( i = thread_idx ; i < ow ; i += thread_numx )
				{
					x = sx * i;
					i1 = static_cast< size_type >( x );
					x -= i1;
					i2 = i1 < iw - 1 ? i1 + 1 : i1;

					__interpolate_utility__::round( _linear_< is_color< value_type >::value >::interpolate( in, i1, i2, j1, j2, k1, k2, x, y, z ), out( i, j, k ) );
				}
			}
		}
	}
}



// 3次のsinc関数による内挿補間
namespace __cubic__
{
	// 0 <= t < 1 の場合
	inline double sinc1( double t ){ return( 1.0 + ( t - 2.0 ) * t * t ); }

	// 1 <= t < 2 の場合
	inline double sinc2( double t ){ return( 4.0 + ( -8.0 + ( 5.0 - t ) * t ) * t ); }


	template < bool b >
	struct _cubic_
	{
		template < class T, class Allocator >
		static double interpolate( const array< T, Allocator > &in,
								typename array< T, Allocator >::size_type i[4],
								typename array< T, Allocator >::size_type /* j */[4],
								typename array< T, Allocator >::size_type /* k */[4],
								double x, double /* y */, double /* z */ )
		{
			typedef typename array< T, Allocator >::value_type value_type;

			double u0 = sinc2( 1 + x );
			double u1 = sinc1( x );
			double u2 = sinc1( 1 - x );
			double u3 = sinc2( 2 - x );
			double pix = in[ i[ 0 ] ] * u0 + in[ i[ 1 ] ] * u1 + in[ i[ 2 ] ] * u2 + in[ i[ 3 ] ] * u3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array2< T, Allocator > &in,
								typename array2< T, Allocator >::size_type i[4],
								typename array2< T, Allocator >::size_type j[4],
								typename array2< T, Allocator >::size_type /* k */[4],
								double x, double y, double /* z */ )
		{
			typedef typename array2< T, Allocator >::value_type value_type;

			double u0 = sinc2( 1 + x );
			double u1 = sinc1( x );
			double u2 = sinc1( 1 - x );
			double u3 = sinc2( 2 - x );
			double v0 = sinc2( 1 + y );
			double v1 = sinc1( y );
			double v2 = sinc1( 1 - y );
			double v3 = sinc2( 2 - y );
			double pix = ( in( i[ 0 ], j[ 0 ] ) * u0 + in( i[ 1 ], j[ 0 ] ) * u1 + in( i[ 2 ], j[ 0 ] ) * u2 + in( i[ 3 ], j[ 0 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ) * u0 + in( i[ 1 ], j[ 1 ] ) * u1 + in( i[ 2 ], j[ 1 ] ) * u2 + in( i[ 3 ], j[ 1 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ) * u0 + in( i[ 1 ], j[ 2 ] ) * u1 + in( i[ 2 ], j[ 2 ] ) * u2 + in( i[ 3 ], j[ 2 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ) * u0 + in( i[ 1 ], j[ 3 ] ) * u1 + in( i[ 2 ], j[ 3 ] ) * u2 + in( i[ 3 ], j[ 3 ] ) * u3 ) * v3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array3< T, Allocator > &in,
								typename array3< T, Allocator >::size_type i[4],
								typename array3< T, Allocator >::size_type j[4],
								typename array3< T, Allocator >::size_type k[4],
								double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type value_type;

			double u0 = sinc2( 1 + x );
			double u1 = sinc1( x );
			double u2 = sinc1( 1 - x );
			double u3 = sinc2( 2 - x );
			double v0 = sinc2( 1 + y );
			double v1 = sinc1( y );
			double v2 = sinc1( 1 - y );
			double v3 = sinc2( 2 - y );
			double w0 = sinc2( 1 + z );
			double w1 = sinc1( z );
			double w2 = sinc1( 1 - z );
			double w3 = sinc2( 2 - z );

			double p0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ) * u3 ) * v3 );
			double p1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ) * u3 ) * v3 );
			double p2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ) * u3 ) * v3 );
			double p3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ) * u3 ) * v3 );
			double pix = p0 * w0 + p1 * w1 + p2 * w2 + p3 * w3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}
	};

	template < >
	struct _cubic_< true >
	{
		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array< T, Allocator > &in,
																			   typename array< T, Allocator >::size_type i[4],
																			   typename array< T, Allocator >::size_type /* j */[4],
																			   typename array< T, Allocator >::size_type /* k */[4],
																			   double x, double /* y */, double /* z */ )
		{
			typedef typename array< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;

			double u0 = sinc2( 1 + x );
			double u1 = sinc1( x );
			double u2 = sinc1( 1 - x );
			double u3 = sinc2( 2 - x );
			double r = in[ i[ 0 ] ].r * u0 + in[ i[ 1 ] ].r * u1 + in[ i[ 2 ] ].r * u2 + in[ i[ 3 ] ].r * u3;
			double g = in[ i[ 0 ] ].g * u0 + in[ i[ 1 ] ].g * u1 + in[ i[ 2 ] ].g * u2 + in[ i[ 3 ] ].g * u3;
			double b = in[ i[ 0 ] ].b * u0 + in[ i[ 1 ] ].b * u1 + in[ i[ 2 ] ].b * u2 + in[ i[ 3 ] ].b * u3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );

			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;

			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array2< T, Allocator > &in,
																			   typename array2< T, Allocator >::size_type i[4],
																			   typename array2< T, Allocator >::size_type j[4],
																			   typename array2< T, Allocator >::size_type /* k */[4],
																			   double x, double y, double /* z */ )
		{
			typedef typename array2< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;

			double u0 = sinc2( 1 + x );
			double u1 = sinc1( x );
			double u2 = sinc1( 1 - x );
			double u3 = sinc2( 2 - x );
			double v0 = sinc2( 1 + y );
			double v1 = sinc1( y );
			double v2 = sinc1( 1 - y );
			double v3 = sinc2( 2 - y );

			double r  =   ( in( i[ 0 ], j[ 0 ] ).r * u0 + in( i[ 1 ], j[ 0 ] ).r * u1 + in( i[ 2 ], j[ 0 ] ).r * u2 + in( i[ 3 ], j[ 0 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ).r * u0 + in( i[ 1 ], j[ 1 ] ).r * u1 + in( i[ 2 ], j[ 1 ] ).r * u2 + in( i[ 3 ], j[ 1 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ).r * u0 + in( i[ 1 ], j[ 2 ] ).r * u1 + in( i[ 2 ], j[ 2 ] ).r * u2 + in( i[ 3 ], j[ 2 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ).r * u0 + in( i[ 1 ], j[ 3 ] ).r * u1 + in( i[ 2 ], j[ 3 ] ).r * u2 + in( i[ 3 ], j[ 3 ] ).r * u3 ) * v3;
			double g  =   ( in( i[ 0 ], j[ 0 ] ).g * u0 + in( i[ 1 ], j[ 0 ] ).g * u1 + in( i[ 2 ], j[ 0 ] ).g * u2 + in( i[ 3 ], j[ 0 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ).g * u0 + in( i[ 1 ], j[ 1 ] ).g * u1 + in( i[ 2 ], j[ 1 ] ).g * u2 + in( i[ 3 ], j[ 1 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ).g * u0 + in( i[ 1 ], j[ 2 ] ).g * u1 + in( i[ 2 ], j[ 2 ] ).g * u2 + in( i[ 3 ], j[ 2 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ).g * u0 + in( i[ 1 ], j[ 3 ] ).g * u1 + in( i[ 2 ], j[ 3 ] ).g * u2 + in( i[ 3 ], j[ 3 ] ).g * u3 ) * v3;
			double b  =   ( in( i[ 0 ], j[ 0 ] ).b * u0 + in( i[ 1 ], j[ 0 ] ).b * u1 + in( i[ 2 ], j[ 0 ] ).b * u2 + in( i[ 3 ], j[ 0 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ).b * u0 + in( i[ 1 ], j[ 1 ] ).b * u1 + in( i[ 2 ], j[ 1 ] ).b * u2 + in( i[ 3 ], j[ 1 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ).b * u0 + in( i[ 1 ], j[ 2 ] ).b * u1 + in( i[ 2 ], j[ 2 ] ).b * u2 + in( i[ 3 ], j[ 2 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ).b * u0 + in( i[ 1 ], j[ 3 ] ).b * u1 + in( i[ 2 ], j[ 3 ] ).b * u2 + in( i[ 3 ], j[ 3 ] ).b * u3 ) * v3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );

			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;

			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array3< T, Allocator > &in,
																			   typename array3< T, Allocator >::size_type i[4],
																			   typename array3< T, Allocator >::size_type j[4],
																			   typename array3< T, Allocator >::size_type k[4],
																			   double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;

			double u0 = sinc2( 1 + x );
			double u1 = sinc1( x );
			double u2 = sinc1( 1 - x );
			double u3 = sinc2( 2 - x );
			double v0 = sinc2( 1 + y );
			double v1 = sinc1( y );
			double v2 = sinc1( 1 - y );
			double v3 = sinc2( 2 - y );
			double w0 = sinc2( 1 + z );
			double w1 = sinc1( z );
			double w2 = sinc1( 1 - z );
			double w3 = sinc2( 2 - z );

			double r0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ).r * u3 ) * v3 );
			double r1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ).r * u3 ) * v3 );
			double r2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ).r * u3 ) * v3 );
			double r3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ).r * u3 ) * v3 );
			double g0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ).g * u3 ) * v3 );
			double g1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ).g * u3 ) * v3 );
			double g2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ).g * u3 ) * v3 );
			double g3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ).g * u3 ) * v3 );
			double b0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ).b * u3 ) * v3 );
			double b1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ).b * u3 ) * v3 );
			double b2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ).b * u3 ) * v3 );
			double b3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ).b * u3 ) * v3 );

			double r = r0 * w0 + r1 * w1 + r2 * w2 + r3 * w3;
			double g = g0 * w0 + g1 * w1 + g2 * w2 + g3 * w3;
			double b = b0 * w0 + b1 * w1 + b2 * w2 + b3 * w3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );

			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;

			return( ovalue_type( r, g, b ) );
		}
	};

	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
						typename Array1::size_type thread_idx, typename Array1::size_type thread_numx,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array2::value_type out_value_type;

		size_type i, j, k, ii[ 4 ], jj[ 4 ], kk[ 4 ];
		size_type iw = in.width( );
		size_type ih = in.height( );
		size_type id = in.depth( );
		size_type ow = out.width( );
		size_type oh = out.height( );
		size_type od = out.depth( );

		double sx = static_cast< double >( iw ) / static_cast< double >( ow );
		double sy = static_cast< double >( ih ) / static_cast< double >( oh );
		double sz = static_cast< double >( id ) / static_cast< double >( od );
		double x, y, z;

		for( k = thread_idz ; k < od ; k += thread_numz )
		{
			z = sz * k;
			kk[ 1 ] = static_cast< size_type >( z );
			kk[ 0 ] = kk[ 1 ] > 0      ? kk[ 1 ] - 1 : kk[ 1 ];
			kk[ 2 ] = kk[ 1 ] < id - 1 ? kk[ 1 ] + 1 : kk[ 1 ];
			kk[ 3 ] = kk[ 2 ] < id - 1 ? kk[ 2 ] + 1 : kk[ 2 ];
			z -= kk[ 1 ];

			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				y = sy * j;
				jj[ 1 ] = static_cast< size_type >( y );
				jj[ 0 ] = jj[ 1 ] > 0      ? jj[ 1 ] - 1 : jj[ 1 ];
				jj[ 2 ] = jj[ 1 ] < ih - 1 ? jj[ 1 ] + 1 : jj[ 1 ];
				jj[ 3 ] = jj[ 2 ] < ih - 1 ? jj[ 2 ] + 1 : jj[ 2 ];
				y -= jj[ 1 ];

				for( i = thread_idx ; i < ow ; i += thread_numx )
				{
					x = sx * i;
					ii[ 1 ] = static_cast< size_type >( x );
					ii[ 0 ] = ii[ 1 ] > 0      ? ii[ 1 ] - 1 : ii[ 1 ];
					ii[ 2 ] = ii[ 1 ] < iw - 1 ? ii[ 1 ] + 1 : ii[ 1 ];
					ii[ 3 ] = ii[ 2 ] < iw - 1 ? ii[ 2 ] + 1 : ii[ 2 ];
					x -= ii[ 1 ];

					__interpolate_utility__::round( _cubic_< is_color< value_type >::value >::interpolate( in, ii, jj, kk, x, y, z ), out( i, j, k ) );
				}
			}
		}
	}
}






// 3次のBスプライン関数による内挿補間
namespace __bspline__
{
	// 0 <= t < 1 の場合
	inline double bspline1( double t ){ return( 2.0 / 3.0 + ( 0.5 * t - 1.0 ) * t * t ); }

	// 1 <= t < 2 の場合
	inline double bspline2( double t ){ return( 4.0 / 3.0 + ( -2.0 + ( 1.0 - t / 6.0 ) * t ) * t ); }


	template < bool b >
	struct _bspline_
	{
		template < class T, class Allocator >
		static double interpolate( const array< T, Allocator > &in,
								typename array< T, Allocator >::size_type i[4],
								typename array< T, Allocator >::size_type /* j */[4],
								typename array< T, Allocator >::size_type /* k */[4],
								double x, double /* y */, double /* z */ )
		{
			typedef typename array< T, Allocator >::value_type value_type;

			double u0 = bspline2( 1 + x );
			double u1 = bspline1( x );
			double u2 = bspline1( 1 - x );
			double u3 = bspline2( 2 - x );
			double pix = in[ i[ 0 ] ] * u0 + in[ i[ 1 ] ] * u1 + in[ i[ 2 ] ] * u2 + in[ i[ 3 ] ] * u3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array2< T, Allocator > &in,
								typename array2< T, Allocator >::size_type i[4],
								typename array2< T, Allocator >::size_type j[4],
								typename array2< T, Allocator >::size_type /* k */[4],
								double x, double y, double /* z */ )
		{
			typedef typename array2< T, Allocator >::value_type value_type;

			double u0 = bspline2( 1 + x );
			double u1 = bspline1( x );
			double u2 = bspline1( 1 - x );
			double u3 = bspline2( 2 - x );
			double v0 = bspline2( 1 + y );
			double v1 = bspline1( y );
			double v2 = bspline1( 1 - y );
			double v3 = bspline2( 2 - y );
			double pix = ( in( i[ 0 ], j[ 0 ] ) * u0 + in( i[ 1 ], j[ 0 ] ) * u1 + in( i[ 2 ], j[ 0 ] ) * u2 + in( i[ 3 ], j[ 0 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ) * u0 + in( i[ 1 ], j[ 1 ] ) * u1 + in( i[ 2 ], j[ 1 ] ) * u2 + in( i[ 3 ], j[ 1 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ) * u0 + in( i[ 1 ], j[ 2 ] ) * u1 + in( i[ 2 ], j[ 2 ] ) * u2 + in( i[ 3 ], j[ 2 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ) * u0 + in( i[ 1 ], j[ 3 ] ) * u1 + in( i[ 2 ], j[ 3 ] ) * u2 + in( i[ 3 ], j[ 3 ] ) * u3 ) * v3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array3< T, Allocator > &in,
								typename array3< T, Allocator >::size_type i[4],
								typename array3< T, Allocator >::size_type j[4],
								typename array3< T, Allocator >::size_type k[4],
								double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type value_type;

			double u0 = bspline2( 1 + x );
			double u1 = bspline1( x );
			double u2 = bspline1( 1 - x );
			double u3 = bspline2( 2 - x );
			double v0 = bspline2( 1 + y );
			double v1 = bspline1( y );
			double v2 = bspline1( 1 - y );
			double v3 = bspline2( 2 - y );
			double w0 = bspline2( 1 + z );
			double w1 = bspline1( z );
			double w2 = bspline1( 1 - z );
			double w3 = bspline2( 2 - z );

			double p0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ) * u3 ) * v3 );
			double p1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ) * u3 ) * v3 );
			double p2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ) * u3 ) * v3 );
			double p3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ) * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ) * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ) * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ) * u3 ) * v3 );
			double pix = p0 * w0 + p1 * w1 + p2 * w2 + p3 * w3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}
	};

	template < >
	struct _bspline_< true >
	{
		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array< T, Allocator > &in,
												typename array< T, Allocator >::size_type i[4],
												typename array< T, Allocator >::size_type /* j */[4],
												typename array< T, Allocator >::size_type /* k */[4],
												double x, double /* y */, double /* z */ )
		{
			typedef typename array< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;

			double u0 = bspline2( 1 + x );
			double u1 = bspline1( x );
			double u2 = bspline1( 1 - x );
			double u3 = bspline2( 2 - x );
			double r = in[ i[ 0 ] ].r * u0 + in[ i[ 1 ] ].r * u1 + in[ i[ 2 ] ].r * u2 + in[ i[ 3 ] ].r * u3;
			double g = in[ i[ 0 ] ].g * u0 + in[ i[ 1 ] ].g * u1 + in[ i[ 2 ] ].g * u2 + in[ i[ 3 ] ].g * u3;
			double b = in[ i[ 0 ] ].b * u0 + in[ i[ 1 ] ].b * u1 + in[ i[ 2 ] ].b * u2 + in[ i[ 3 ] ].b * u3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );

			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;

			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array2< T, Allocator > &in,
												typename array2< T, Allocator >::size_type i[4],
												typename array2< T, Allocator >::size_type j[4],
												typename array2< T, Allocator >::size_type /* k */[4],
												double x, double y, double /* z */ )
		{
			typedef typename array2< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;

			double u0 = bspline2( 1 + x );
			double u1 = bspline1( x );
			double u2 = bspline1( 1 - x );
			double u3 = bspline2( 2 - x );
			double v0 = bspline2( 1 + y );
			double v1 = bspline1( y );
			double v2 = bspline1( 1 - y );
			double v3 = bspline2( 2 - y );

			double r  =   ( in( i[ 0 ], j[ 0 ] ).r * u0 + in( i[ 1 ], j[ 0 ] ).r * u1 + in( i[ 2 ], j[ 0 ] ).r * u2 + in( i[ 3 ], j[ 0 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ).r * u0 + in( i[ 1 ], j[ 1 ] ).r * u1 + in( i[ 2 ], j[ 1 ] ).r * u2 + in( i[ 3 ], j[ 1 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ).r * u0 + in( i[ 1 ], j[ 2 ] ).r * u1 + in( i[ 2 ], j[ 2 ] ).r * u2 + in( i[ 3 ], j[ 2 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ).r * u0 + in( i[ 1 ], j[ 3 ] ).r * u1 + in( i[ 2 ], j[ 3 ] ).r * u2 + in( i[ 3 ], j[ 3 ] ).r * u3 ) * v3;
			double g  =   ( in( i[ 0 ], j[ 0 ] ).g * u0 + in( i[ 1 ], j[ 0 ] ).g * u1 + in( i[ 2 ], j[ 0 ] ).g * u2 + in( i[ 3 ], j[ 0 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ).g * u0 + in( i[ 1 ], j[ 1 ] ).g * u1 + in( i[ 2 ], j[ 1 ] ).g * u2 + in( i[ 3 ], j[ 1 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ).g * u0 + in( i[ 1 ], j[ 2 ] ).g * u1 + in( i[ 2 ], j[ 2 ] ).g * u2 + in( i[ 3 ], j[ 2 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ).g * u0 + in( i[ 1 ], j[ 3 ] ).g * u1 + in( i[ 2 ], j[ 3 ] ).g * u2 + in( i[ 3 ], j[ 3 ] ).g * u3 ) * v3;
			double b  =   ( in( i[ 0 ], j[ 0 ] ).b * u0 + in( i[ 1 ], j[ 0 ] ).b * u1 + in( i[ 2 ], j[ 0 ] ).b * u2 + in( i[ 3 ], j[ 0 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ).b * u0 + in( i[ 1 ], j[ 1 ] ).b * u1 + in( i[ 2 ], j[ 1 ] ).b * u2 + in( i[ 3 ], j[ 1 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ).b * u0 + in( i[ 1 ], j[ 2 ] ).b * u1 + in( i[ 2 ], j[ 2 ] ).b * u2 + in( i[ 3 ], j[ 2 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ).b * u0 + in( i[ 1 ], j[ 3 ] ).b * u1 + in( i[ 2 ], j[ 3 ] ).b * u2 + in( i[ 3 ], j[ 3 ] ).b * u3 ) * v3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );

			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;

			return( ovalue_type( r, g, b ) );
		}

		template < class T, class Allocator >
		static const typename T::template rebind< double >::other interpolate( const array3< T, Allocator > &in,
												typename array3< T, Allocator >::size_type i[4],
												typename array3< T, Allocator >::size_type j[4],
												typename array3< T, Allocator >::size_type k[4],
												double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			typedef typename T::template rebind< double >::other ovalue_type;

			double u0 = bspline2( 1 + x );
			double u1 = bspline1( x );
			double u2 = bspline1( 1 - x );
			double u3 = bspline2( 2 - x );
			double v0 = bspline2( 1 + y );
			double v1 = bspline1( y );
			double v2 = bspline1( 1 - y );
			double v3 = bspline2( 2 - y );
			double w0 = bspline2( 1 + z );
			double w1 = bspline1( z );
			double w2 = bspline1( 1 - z );
			double w3 = bspline2( 2 - z );

			double r0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ).r * u3 ) * v3 );
			double r1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ).r * u3 ) * v3 );
			double r2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ).r * u3 ) * v3 );
			double r3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ).r * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ).r * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ).r * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ).r * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ).r * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ).r * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ).r * u3 ) * v3 );
			double g0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ).g * u3 ) * v3 );
			double g1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ).g * u3 ) * v3 );
			double g2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ).g * u3 ) * v3 );
			double g3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ).g * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ).g * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ).g * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ).g * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ).g * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ).g * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ).g * u3 ) * v3 );
			double b0 = ( ( in( i[ 0 ], j[ 0 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 0 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 0 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 0 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 0 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 0 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 0 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 0 ] ).b * u3 ) * v3 );
			double b1 = ( ( in( i[ 0 ], j[ 0 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 1 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 1 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 1 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 1 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 1 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 1 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 1 ] ).b * u3 ) * v3 );
			double b2 = ( ( in( i[ 0 ], j[ 0 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 2 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 2 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 2 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 2 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 2 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 2 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 2 ] ).b * u3 ) * v3 );
			double b3 = ( ( in( i[ 0 ], j[ 0 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 0 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 0 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 0 ], k[ 3 ] ).b * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 1 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 1 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 1 ], k[ 3 ] ).b * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 2 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 2 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 2 ], k[ 3 ] ).b * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ], k[ 3 ] ).b * u0 + in( i[ 1 ], j[ 3 ], k[ 3 ] ).b * u1 + in( i[ 2 ], j[ 3 ], k[ 3 ] ).b * u2 + in( i[ 3 ], j[ 3 ], k[ 3 ] ).b * u3 ) * v3 );

			double r = r0 * w0 + r1 * w1 + r2 * w2 + r3 * w3;
			double g = g0 * w0 + g1 * w1 + g2 * w2 + g3 * w3;
			double b = b0 * w0 + b1 * w1 + b2 * w2 + b3 * w3;

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );

			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;

			return( ovalue_type( r, g, b ) );
		}
	};

	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
						typename Array1::size_type thread_idx, typename Array1::size_type thread_numx,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type  size_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array2::value_type out_value_type;

		size_type i, j, k, ii[ 4 ], jj[ 4 ], kk[ 4 ];
		size_type iw = in.width( );
		size_type ih = in.height( );
		size_type id = in.depth( );
		size_type ow = out.width( );
		size_type oh = out.height( );
		size_type od = out.depth( );

		double sx = static_cast< double >( iw ) / static_cast< double >( ow );
		double sy = static_cast< double >( ih ) / static_cast< double >( oh );
		double sz = static_cast< double >( id ) / static_cast< double >( od );
		double x, y, z;

		for( k = thread_idz ; k < od ; k += thread_numz )
		{
			z = sz * k;
			kk[ 1 ] = static_cast< size_type >( z );
			kk[ 0 ] = kk[ 1 ] > 0      ? kk[ 1 ] - 1 : kk[ 1 ];
			kk[ 2 ] = kk[ 1 ] < id - 1 ? kk[ 1 ] + 1 : kk[ 1 ];
			kk[ 3 ] = kk[ 2 ] < id - 1 ? kk[ 2 ] + 1 : kk[ 2 ];
			z -= kk[ 1 ];

			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				y = sy * j;
				jj[ 1 ] = static_cast< size_type >( y );
				jj[ 0 ] = jj[ 1 ] > 0      ? jj[ 1 ] - 1 : jj[ 1 ];
				jj[ 2 ] = jj[ 1 ] < ih - 1 ? jj[ 1 ] + 1 : jj[ 1 ];
				jj[ 3 ] = jj[ 2 ] < ih - 1 ? jj[ 2 ] + 1 : jj[ 2 ];
				y -= jj[ 1 ];

				for( i = thread_idx ; i < ow ; i += thread_numx )
				{
					x = sx * i;
					ii[ 1 ] = static_cast< size_type >( x );
					ii[ 0 ] = ii[ 1 ] > 0      ? ii[ 1 ] - 1 : ii[ 1 ];
					ii[ 2 ] = ii[ 1 ] < iw - 1 ? ii[ 1 ] + 1 : ii[ 1 ];
					ii[ 3 ] = ii[ 2 ] < iw - 1 ? ii[ 2 ] + 1 : ii[ 2 ];
					x -= ii[ 1 ];

					__interpolate_utility__::round( _bspline_< is_color< value_type >::value >::interpolate( in, ii, jj, kk, x, y, z ), out( i, j, k ) );
				}
			}
		}
	}
}



// フルスケールでのsinc関数を用いた内挿補間
namespace __sinc__
{
	// 厳密な定義での sinc 関数
	inline double sinc( double t )
	{
		return( t == 0.0 ? 1.0 : std::sin( 3.1415926535897932384626433832795 * t ) / ( 3.1415926535897932384626433832795 * t ) );
	}

	template < bool b >
	struct _sinc_
	{
		template < class T, class Allocator >
		static double interpolate( const array< T, Allocator > &in,
								typename array< T, Allocator >::difference_type ix,
								typename array< T, Allocator >::difference_type iy,
								typename array< T, Allocator >::difference_type iz,
								double x, double y, double z )
		{
			typedef typename array< T, Allocator >::difference_type difference_type;
			typedef typename array< T, Allocator >::value_type value_type;

			double pix = 0.0;
			difference_type i, num = in.size( );
			for( i = 0 ; i <= ix ; i++ )
			{
				pix += in[ i ] * sinc( ix - i - x );
			}
			for( ; i < num ; i++ )
			{
				pix += in[ i ] * sinc( i - ix + x );
			}

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array2< T, Allocator > &in,
								typename array2< T, Allocator >::difference_type ix,
								typename array2< T, Allocator >::difference_type iy,
								typename array2< T, Allocator >::difference_type iz,
								double x, double y, double z )
		{
			typedef typename array2< T, Allocator >::difference_type difference_type;
			typedef typename array2< T, Allocator >::value_type value_type;
			typedef typename array2< T, Allocator >::const_pointer const_pointer;

			double pix = 0.0;
			difference_type width = in.width( );
			difference_type height = in.height( );

			difference_type j;
			for( j = 0 ; j < iy ; j++ )
			{
				difference_type i;
				const_pointer p = &in( 0, j );
				double tmpX = 0.0;
				for( i = 0 ; i <= ix ; i++ )
				{
					tmpX += p[ i ] * sinc( ix - i - x );
				}
				for( ; i < width ; i++ )
				{
					tmpX += p[ i ] * sinc( i - ix + x );
				}

				pix += tmpX * sinc( iy - j - y );
			}
			for( ; j < height ; j++ )
			{
				difference_type i;
				const_pointer p = &in( 0, j );
				double tmpX = 0.0;
				for( i = 0 ; i <= ix ; i++ )
				{
					tmpX += p[ i ] * sinc( ix - i - x );
				}
				for( ; i < width ; i++ )
				{
					tmpX += p[ i ] * sinc( i - ix + x );
				}

				pix += tmpX * sinc( j - iy + y );
			}

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}

		template < class T, class Allocator >
		static double interpolate( const array3< T, Allocator > &in,
								typename array3< T, Allocator >::difference_type ix,
								typename array3< T, Allocator >::difference_type iy,
								typename array3< T, Allocator >::difference_type iz,
								double x, double y, double z )
		{
			typedef typename array2< T, Allocator >::difference_type difference_type;
			typedef typename array2< T, Allocator >::value_type value_type;
			typedef typename array2< T, Allocator >::const_pointer const_pointer;

			double pix = 0.0;
			difference_type width = in.width( );
			difference_type height = in.height( );
			difference_type depth = in.depth( );

			difference_type k;
			for( k = 0 ; k < iz ; k++ )
			{
				difference_type j;
				double tmpY = 0.0;
				for( j = 0 ; j < iy ; j++ )
				{
					difference_type i;
					const_pointer p = &in( 0, j, k );
					double tmpX = 0.0;
					for( i = 0 ; i <= ix ; i++ )
					{
						tmpX += p[ i ] * sinc( ix - i - x );
					}
					for( ; i < width ; i++ )
					{
						tmpX += p[ i ] * sinc( i - ix + x );
					}

					tmpY += tmpX * sinc( iy - j - y );
				}
				for( ; j < height ; j++ )
				{
					difference_type i;
					const_pointer p = &in( 0, j, k );
					double tmpX = 0.0;
					for( i = 0 ; i <= ix ; i++ )
					{
						tmpX += p[ i ] * sinc( ix - i - x );
					}
					for( ; i < width ; i++ )
					{
						tmpX += p[ i ] * sinc( i - ix + x );
					}

					tmpY += tmpX * sinc( j - iy + y );
				}

				pix += tmpY * sinc( iz - k - z );
			}
			for( ; k < depth ; k++ )
			{
				difference_type j;
				double tmpY = 0.0;
				for( j = 0 ; j < iy ; j++ )
				{
					difference_type i;
					const_pointer p = &in( 0, j, k );
					double tmpX = 0.0;
					for( i = 0 ; i <= ix ; i++ )
					{
						tmpX += p[ i ] * sinc( ix - i - x );
					}
					for( ; i < width ; i++ )
					{
						tmpX += p[ i ] * sinc( i - ix + x );
					}

					tmpY += tmpX * sinc( iy - j - y );
				}
				for( ; j < height ; j++ )
				{
					difference_type i;
					const_pointer p = &in( 0, j, k );
					double tmpX = 0.0;
					for( i = 0 ; i <= ix ; i++ )
					{
						tmpX += p[ i ] * sinc( ix - i - x );
					}
					for( ; i < width ; i++ )
					{
						tmpX += p[ i ] * sinc( i - ix + x );
					}

					tmpY += tmpX * sinc( j - iy + y );
				}

				pix += tmpY * sinc( k - iz + z );
			}

			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;

			return( pix );
		}
	};


	template < >
	struct _sinc_< true >
	{
		template < class T, class Allocator >
		static typename T::template rebind< double >::other interpolate( const array< T, Allocator > &in,
								typename array< T, Allocator >::difference_type ix,
								typename array< T, Allocator >::difference_type iy,
								typename array< T, Allocator >::difference_type iz,
								double /* x */, double /* y */, double /* z */ )
		{
			return( in( ix, iy, iz ) );
		}

		template < class T, class Allocator >
		static typename T::template rebind< double >::other interpolate( const array2< T, Allocator > &in,
								typename array2< T, Allocator >::difference_type ix,
								typename array2< T, Allocator >::difference_type iy,
								typename array2< T, Allocator >::difference_type iz,
								double /* x */, double /* y */, double /* z */ )
		{
			return( in( ix, iy, iz ) );
		}

		template < class T, class Allocator >
		static typename T::template rebind< double >::other interpolate( const array3< T, Allocator > &in,
								typename array3< T, Allocator >::difference_type ix,
								typename array3< T, Allocator >::difference_type iy,
								typename array3< T, Allocator >::difference_type iz,
								double /* x */, double /* y */, double /* z */ )
		{
			return( in( ix, iy, iz ) );
		}
	};


	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
						typename Array1::size_type thread_idx, typename Array1::size_type thread_numx,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type			size_type;
		typedef typename Array1::difference_type	difference_type;
		typedef typename Array1::value_type			value_type;
		typedef typename Array2::value_type			out_value_type;

		size_type i, j, k;
		difference_type ii, jj, kk;
		size_type iw = in.width( );
		size_type ih = in.height( );
		size_type id = in.depth( );
		size_type ow = out.width( );
		size_type oh = out.height( );
		size_type od = out.depth( );

		double sx = static_cast< double >( iw ) / static_cast< double >( ow );
		double sy = static_cast< double >( ih ) / static_cast< double >( oh );
		double sz = static_cast< double >( id ) / static_cast< double >( od );
		double x, y, z;

		for( k = thread_idz ; k < od ; k += thread_numz )
		{
			z = sz * k;
			kk = static_cast< difference_type >( z );
			z -= kk;

			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				y = sy * j;
				jj = static_cast< difference_type >( y );
				y -= jj;

				for( i = thread_idx ; i < ow ; i += thread_numx )
				{
					x = sx * i;
					ii = static_cast< difference_type >( x );
					x -= ii;

					__interpolate_utility__::round( _sinc_< is_color< value_type >::value >::interpolate( in, ii, jj, kk, x, y, z ), out( i, j, k ) );
				}
			}
		}
	}
}




// 画像補間のスレッド実装
namespace __interpolate_controller__
{
	// 最近傍型補間
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void nearest__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
					typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__nearest__::interpolate( in, out, thread_id, thread_num, 0, 1, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void nearest__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__nearest__::interpolate( in, out, 0, 1, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void nearest__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__nearest__::interpolate( in, out, 0, 1, 0, 1, thread_id, thread_num );
	}


	// 平均値型補間
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void mean__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
					typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__mean__::interpolate( in, out, thread_id, thread_num, 0, 1, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void mean__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__mean__::interpolate( in, out, 0, 1, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void mean__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__mean__::interpolate( in, out, 0, 1, 0, 1, thread_id, thread_num );
	}


	// 線形補間
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void linear__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
					typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__linear__::interpolate( in, out, thread_id, thread_num, 0, 1, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void linear__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__linear__::interpolate( in, out, 0, 1, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void linear__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__linear__::interpolate( in, out, 0, 1, 0, 1, thread_id, thread_num );
	}


	// 3次のsinc関数を用いた補間
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void cubic__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
					typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__cubic__::interpolate( in, out, thread_id, thread_num, 0, 1, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void cubic__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__cubic__::interpolate( in, out, 0, 1, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void cubic__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__cubic__::interpolate( in, out, 0, 1, 0, 1, thread_id, thread_num );
	}


	// 3次のbpline関数を用いた補間
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void bspline__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
					typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__bspline__::interpolate( in, out, thread_id, thread_num, 0, 1, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void bspline__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__bspline__::interpolate( in, out, 0, 1, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void bspline__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__bspline__::interpolate( in, out, 0, 1, 0, 1, thread_id, thread_num );
	}


	// フルスケールの sinc 関数を用いた補間
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void sinc__( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
					typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__sinc__::interpolate( in, out, thread_id, thread_num, 0, 1, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void sinc__( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__sinc__::interpolate( in, out, 0, 1, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void sinc__( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__sinc__::interpolate( in, out, 0, 1, 0, 1, thread_id, thread_num );
	}


	template < class T1, class T2 >
	class interpolate_thread : public mist::thread< interpolate_thread< T1, T2 > >
	{
	public:
		typedef mist::thread< interpolate_thread< T1, T2 > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;

		enum Mode
		{
			Nearest,
			Mean,
			Linear,
			Cubic,
			Bspline,
			Sinc,
		};

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const T1 *in_;
		T2 *out_;
		Mode interpolate_;

	public:
		void setup_parameters( const T1 &in, T2 &out, Mode interpolate, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			interpolate_ = interpolate;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		const interpolate_thread& operator =( const interpolate_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				out_ = p.out_;
				interpolate_ = p.interpolate_;
			}
			return( *this );
		}

		interpolate_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
															in_( NULL ), out_( NULL ), interpolate_( Linear )
		{
		}
		interpolate_thread( const interpolate_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
															in_( p.in_ ), out_( p.out_ ), interpolate_( Linear )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			switch( interpolate_ )
			{
			case Nearest:
				nearest__( *in_, *out_, thread_id_, thread_num_ );
				break;

			case Mean:
				mean__( *in_, *out_, thread_id_, thread_num_ );
				break;

			case Cubic:
				cubic__( *in_, *out_, thread_id_, thread_num_ );
				break;

			case Bspline:
				bspline__( *in_, *out_, thread_id_, thread_num_ );
				break;

			case Sinc:
				sinc__( *in_, *out_, thread_id_, thread_num_ );
				break;

			case Linear:
			default:
				linear__( *in_, *out_, thread_id_, thread_num_ );
				break;
			}

			return( true );
		}
	};
}



//! @addtogroup interpolate_group 画像補間
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/interpolate.h>
//! @endcode
//!
//!  @{


/// @brief 最近傍型補間
namespace nearest
{
	/// @brief 1次元配列に対する最近傍型補間
	//!
	//! 拡大縮小に伴う画素の内挿を，最近傍に決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array< T1, Allocator1 >, array< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 1次元配列に対する最近傍型補間
	//!
	//! 拡大縮小に伴う画素の内挿を，最近傍に決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array1< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array1< T1, Allocator1 >, array1< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );
		out.reso1( in.reso1( ) * static_cast< double >( in.size( ) ) / static_cast< double >( width ) );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array1< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 2次元配列に対する最近傍型補間
	//!
	//! 拡大縮小に伴う画素の内挿を，最近傍に決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );

		if( in.width( ) == width && in.height( ) == height )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array2< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 3次元配列に対する最近傍型補間
	//!
	//! 拡大縮小に伴う画素の内挿を，最近傍に決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  depth      … 出力データのZ軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 || depth == 0 )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height, depth );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );
		out.reso3( in.reso3( ) * static_cast< double >( in.depth( ) ) / static_cast< double >( depth ) );

		if( in.width( ) == width && in.height( ) == height && in.depth( ) == depth )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}
}

/// @brief 平均値型補間
namespace mean
{
	/// @brief 1次元配列に対する平均値型補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の平均値を用いて決定する
	//!
	//! - 参考文献
	//!   - A. Hore, F. Deschenes, D. Ziou, "A Simple Scaling Algorithm Based on Areas Pixels," Proceedings of ICIAR 2008, pp.53-64, 2008
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，出力サイズが0の場合，もしくは入力サイズが1以下の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || in.size( ) < 2 )
		{
			return( false );
		}

		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array< T1, Allocator1 >, array< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Mean, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 1次元配列に対する線形補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の平均値を用いて決定する
	//!
	//! - 参考文献
	//!   - A. Hore, F. Deschenes, D. Ziou, "A Simple Scaling Algorithm Based on Areas Pixels," Proceedings of ICIAR 2008, pp.53-64, 2008
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，出力サイズが0の場合，もしくは入力サイズが1以下の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || in.width( ) < 2 )
		{
			return( false );
		}

		typedef typename array1< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array1< T1, Allocator1 >, array1< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );
		out.reso1( in.reso1( ) * static_cast< double >( in.size( ) ) / static_cast< double >( width ) );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array1< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Mean, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 2次元配列に対する線形補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の平均値を用いて決定する
	//!
	//! - 参考文献
	//!   - A. Hore, F. Deschenes, D. Ziou, "A Simple Scaling Algorithm Based on Areas Pixels," Proceedings of ICIAR 2008, pp.53-64, 2008
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，出力サイズが0の場合，もしくは入力サイズが1以下の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 || in.width( ) < 2 || in.height( ) < 2 )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );

		if( in.width( ) == width && in.height( ) == height )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array2< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Mean, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 3次元配列に対する線形補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の平均値を用いて決定する
	//!
	//! - 参考文献
	//!   - A. Hore, F. Deschenes, D. Ziou, "A Simple Scaling Algorithm Based on Areas Pixels," Proceedings of ICIAR 2008, pp.53-64, 2008
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  depth      … 出力データのZ軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，出力サイズが0の場合，もしくは入力サイズが1以下の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 || depth == 0 || in.width( ) < 2 || in.height( ) < 2 || in.depth( ) < 2 )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height, depth );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );
		out.reso3( in.reso3( ) * static_cast< double >( in.depth( ) ) / static_cast< double >( depth ) );

		if( in.width( ) == width && in.height( ) == height && in.depth( ) == depth )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Mean, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}
}



/// @brief 線形補間
namespace linear
{
	/// @brief 1次元配列に対する線形補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の2点を用いて線形関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array< T1, Allocator1 >, array< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 1次元配列に対する線形補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の2点を用いて線形関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array1< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array1< T1, Allocator1 >, array1< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );
		out.reso1( in.reso1( ) * static_cast< double >( in.size( ) ) / static_cast< double >( width ) );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array1< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 2次元配列に対する線形補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の4点を用いて線形関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );

		if( in.width( ) == width && in.height( ) == height )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array2< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 3次元配列に対する線形補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の8点を用いて線形関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  depth      … 出力データのZ軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 || depth == 0 )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height, depth );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );
		out.reso3( in.reso3( ) * static_cast< double >( in.depth( ) ) / static_cast< double >( depth ) );

		if( in.width( ) == width && in.height( ) == height && in.depth( ) == depth )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}
}



/// @brief 3次のsinc関数を用いた補間
namespace cubic
{
	/// @brief 1次元配列に対する3次のsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の4点を用いて3次のsinc関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array< T1, Allocator1 >, array< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}

	/// @brief 1次元配列に対する3次のsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の4点を用いて3次のsinc関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array1< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array1< T1, Allocator1 >, array1< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );
		out.reso1( in.reso1( ) * static_cast< double >( in.size( ) ) / static_cast< double >( width ) );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array1< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 2次元配列に対する3次のsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の16点を用いて3次のsinc関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );

		if( in.width( ) == width && in.height( ) == height )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array2< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 3次元配列に対する3次のsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の64点を用いて3次のsinc関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  depth      … 出力データのZ軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 || depth == 0 )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height, depth );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );
		out.reso3( in.reso3( ) * static_cast< double >( in.depth( ) ) / static_cast< double >( depth ) );

		if( in.width( ) == width && in.height( ) == height && in.depth( ) == depth )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}
}



/// @brief 3次のBスプライン関数を用いた補間
namespace BSpline
{
	/// @brief 1次元配列に対する3次のBスプライン関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の4点を用いて3次のBスプライン関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array< T1, Allocator1 >, array< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Bspline, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}

	/// @brief 1次元配列に対する3次のBスプライン関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の4点を用いて3次のBスプライン関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array1< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array1< T1, Allocator1 >, array1< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );
		out.reso1( in.reso1( ) * static_cast< double >( in.size( ) ) / static_cast< double >( width ) );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array1< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Bspline, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 2次元配列に対する3次のBスプライン関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の16点を用いて3次のBスプライン関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );

		if( in.width( ) == width && in.height( ) == height )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array2< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Bspline, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 3次元配列に対する3次のBスプライン関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，近傍の64点を用いて3次のBスプライン関数で決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  depth      … 出力データのZ軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 || depth == 0 )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height, depth );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );
		out.reso3( in.reso3( ) * static_cast< double >( in.depth( ) ) / static_cast< double >( depth ) );

		if( in.width( ) == width && in.height( ) == height && in.depth( ) == depth )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Bspline, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}
}




/// @brief sinc関数を用いた補間
namespace sinc
{
	/// @brief 1次元配列に対するsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，全点列を用いて決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array< T1, Allocator1 >, array< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Sinc, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}

	/// @brief 1次元配列に対するsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，全点列を用いて決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 )
		{
			return( false );
		}

		typedef typename array1< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array1< T1, Allocator1 >, array1< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width );
		out.reso1( in.reso1( ) * static_cast< double >( in.size( ) ) / static_cast< double >( width ) );

		if( in.width( ) == width )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array1< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Sinc, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 2次元配列に対するsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，全点列を用いて決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 )
		{
			return( false );
		}

		typedef typename array2< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array2< T1, Allocator1 >, array2< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );

		if( in.width( ) == width && in.height( ) == height )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array2< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Sinc, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}


	/// @brief 3次元配列に対するsinc関数を用いた補間
	//!
	//! 拡大縮小に伴う画素の内挿を，全点列を用いて決定する
	//!
	//! @attention 拡大縮小に伴い，画像の解像度も自動的に変換される
	//!
	//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! @param[in]  in         … 入力データ
	//! @param[out] out        … 出力データ
	//! @param[in]  width      … 出力データのX軸方向のサイズ
	//! @param[in]  height     … 出力データのY軸方向のサイズ
	//! @param[in]  depth      … 出力データのZ軸方向のサイズ
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	//! @retval true  … 補間に成功
	//! @retval false … 入力と出力が同じオブジェクトを指定した場合，もしくは出力サイズが0の場合
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	bool interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
		if( is_same_object( in, out ) || width == 0 || height == 0 || depth == 0 )
		{
			return( false );
		}

		typedef typename array3< T1, Allocator1 >::size_type  size_type;
		typedef __interpolate_controller__::interpolate_thread< array3< T1, Allocator1 >, array3< T2, Allocator2 > > interpolate_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		size_type i;
		out.resize( width, height, depth );
		out.reso1( in.reso1( ) * static_cast< double >( in.width( ) ) / static_cast< double >( width ) );
		out.reso2( in.reso2( ) * static_cast< double >( in.height( ) ) / static_cast< double >( height ) );
		out.reso3( in.reso3( ) * static_cast< double >( in.depth( ) ) / static_cast< double >( depth ) );

		if( in.width( ) == width && in.height( ) == height && in.depth( ) == depth )
		{
			for( i = 0 ; i < in.size( ) ; i++ )
			{
				out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( in[ i ] );
			}
			return( true );
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Sinc, i, thread_num );
		}

		do_threads( thread, thread_num );

		delete [] thread;

		return( true );
	}
}

/// @}
//  画像補間グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_INTERPOLATE__
