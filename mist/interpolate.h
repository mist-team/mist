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


// mist名前空間の始まり
_MIST_BEGIN


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
			z = z < id ? z : id;
			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				y = static_cast< size_type >( sy * j + 0.5 );
				y = y < ih ? y : ih;
				for( i = thread_idx ; i < ow ; i += thread_numx )
				{
					x = static_cast< size_type >( sx * i + 0.5 );
					x = x < iw ? x : iw;
					out( i, j, k ) = static_cast< out_value_type >( in( x, y, z ) );
				}
			}
		}
	}
}


// 最近傍型補間
namespace __linear__
{
	template < bool b >
	struct _linear_
	{
		template < class T, class Allocator >
		static double linear___( const array< T, Allocator > &in,
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
		static double linear___( const array2< T, Allocator > &in,
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
		static double linear___( const array3< T, Allocator > &in,
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
			double pix = ( ( in( i1, j1, k1 ) * ( 1.0 - x ) + in( i2, j1, k1 ) * x ) * ( 1.0 - y )	+ ( in( i1, j2, k1 ) * ( 1.0 - x ) + in( i2, j2, k1 ) * x ) * y ) * ( 1.0 - z )
								+ ( ( in( i1, j1, k2 ) * ( 1.0 - x ) + in( i2, j1, k2 ) * x ) * ( 1.0 - y )	+ ( in( i1, j2, k2 ) * ( 1.0 - x ) + in( i2, j2, k2 ) * x ) * y ) * z;
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}
	};

	template < >
	struct _linear_< true >
	{
		template < class T, class Allocator >
		static typename array< T, Allocator >::value_type linear___( const array< T, Allocator > &in,
																	typename array< T, Allocator >::size_type i1,
																	typename array< T, Allocator >::size_type i2,
																	typename array< T, Allocator >::size_type j1,
																	typename array< T, Allocator >::size_type j2,
																	typename array< T, Allocator >::size_type k1,
																	typename array< T, Allocator >::size_type k2,
																	double x, double y, double z )
		{
			typedef typename array< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double r = in[ i1 ].r * ( 1.0 - x ) + in[ i2 ].r * x;
			double g = in[ i1 ].g * ( 1.0 - x ) + in[ i2 ].g * x;
			double b = in[ i1 ].b * ( 1.0 - x ) + in[ i2 ].b * x;
			return( color( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) ) );
		}

		template < class T, class Allocator >
		static typename array2< T, Allocator >::value_type linear___( const array2< T, Allocator > &in,
																	typename array2< T, Allocator >::size_type i1,
																	typename array2< T, Allocator >::size_type i2,
																	typename array2< T, Allocator >::size_type j1,
																	typename array2< T, Allocator >::size_type j2,
																	typename array2< T, Allocator >::size_type k1,
																	typename array2< T, Allocator >::size_type k2,
																	double x, double y, double z )
		{
			typedef typename array2< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
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
			return( color( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) ) );
		}

		template < class T, class Allocator >
		static typename array3< T, Allocator >::value_type linear___( const array3< T, Allocator > &in,
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
			return( color( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) ) );
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
					out( i, j, k ) = static_cast< out_value_type >( _linear_< is_color< value_type >::value >::linear___( in, i1, i2, j1, j2, k1, k2, x, y, z ) );
				}
			}
		}
	}
}



// 3次内挿補間
namespace __cubic__
{
	inline double sinc( double t )
	{
		double ret = 0.0;
		t = fabs( t );

		if( t < 1.0 )
		{
			ret = 1.0 + ( -2.0 + t ) * t * t;
		}
		else if( t < 2.0 )
		{
			ret = 4.0 + ( -8.0 + ( 5.0 - t ) * t ) * t;
		}

		return ( ret );
	}

	template < bool b >
	struct _cubic_
	{
		template < class T, class Allocator >
		static double cubic___( const array< T, Allocator > &in,
								typename array< T, Allocator >::size_type i[4],
								typename array< T, Allocator >::size_type j[4],
								typename array< T, Allocator >::size_type k[4],
								double x, double y, double z )
		{
			typedef typename array< T, Allocator >::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double u0 = sinc( 1 + x );
			double u1 = sinc( x );
			double u2 = sinc( 1 - x );
			double u3 = sinc( 2 - x );
			double pix = in[ i[ 0 ] ] * u0 + in[ i[ 1 ] ] * u1 + in[ i[ 2 ] ] * u2 + in[ i[ 3 ] ] * u3;
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}

		template < class T, class Allocator >
		static double cubic___( const array2< T, Allocator > &in,
								typename array2< T, Allocator >::size_type i[4],
								typename array2< T, Allocator >::size_type j[4],
								typename array2< T, Allocator >::size_type k[4],
								double x, double y, double z )
		{
			typedef typename array2< T, Allocator >::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double u0 = sinc( 1 + x );
			double u1 = sinc( x );
			double u2 = sinc( 1 - x );
			double u3 = sinc( 2 - x );
			double v0 = sinc( 1 + y );
			double v1 = sinc( y );
			double v2 = sinc( 1 - y );
			double v3 = sinc( 2 - y );
			double pix = ( in( i[ 0 ], j[ 0 ] ) * u0 + in( i[ 1 ], j[ 0 ] ) * u1 + in( i[ 2 ], j[ 0 ] ) * u2 + in( i[ 3 ], j[ 0 ] ) * u3 ) * v0
						+ ( in( i[ 0 ], j[ 1 ] ) * u0 + in( i[ 1 ], j[ 1 ] ) * u1 + in( i[ 2 ], j[ 1 ] ) * u2 + in( i[ 3 ], j[ 1 ] ) * u3 ) * v1
						+ ( in( i[ 0 ], j[ 2 ] ) * u0 + in( i[ 1 ], j[ 2 ] ) * u1 + in( i[ 2 ], j[ 2 ] ) * u2 + in( i[ 3 ], j[ 2 ] ) * u3 ) * v2
						+ ( in( i[ 0 ], j[ 3 ] ) * u0 + in( i[ 1 ], j[ 3 ] ) * u1 + in( i[ 2 ], j[ 3 ] ) * u2 + in( i[ 3 ], j[ 3 ] ) * u3 ) * v3;
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}

		template < class T, class Allocator >
		static double cubic___( const array3< T, Allocator > &in,
								typename array3< T, Allocator >::size_type i[4],
								typename array3< T, Allocator >::size_type j[4],
								typename array3< T, Allocator >::size_type k[4],
								double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double u0 = sinc( 1 + x );
			double u1 = sinc( x );
			double u2 = sinc( 1 - x );
			double u3 = sinc( 2 - x );
			double v0 = sinc( 1 + y );
			double v1 = sinc( y );
			double v2 = sinc( 1 - y );
			double v3 = sinc( 2 - y );
			double w0 = sinc( 1 + z );
			double w1 = sinc( z );
			double w2 = sinc( 1 - z );
			double w3 = sinc( 2 - z );
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
			pix = pix > min ? pix : min;
			pix = pix < max ? pix : max;
			return( pix );
		}
	};

	template < >
	struct _cubic_< true >
	{
		template < class T, class Allocator >
		static typename array< T, Allocator >::value_type cubic___( const array< T, Allocator > &in,
																	typename array< T, Allocator >::size_type i[4],
																	typename array< T, Allocator >::size_type j[4],
																	typename array< T, Allocator >::size_type k[4],
																	double x, double y, double z )
		{
			typedef typename array< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double u0 = sinc( 1 + x );
			double u1 = sinc( x );
			double u2 = sinc( 1 - x );
			double u3 = sinc( 2 - x );
			double r = in[ i[ 0 ] ].r * u0 + in[ i[ 1 ] ].r * u1 + in[ i[ 2 ] ].r * u2 + in[ i[ 3 ] ].r * u3;
			double g = in[ i[ 0 ] ].g * u0 + in[ i[ 1 ] ].g * u1 + in[ i[ 2 ] ].g * u2 + in[ i[ 3 ] ].g * u3;
			double b = in[ i[ 0 ] ].b * u0 + in[ i[ 1 ] ].b * u1 + in[ i[ 2 ] ].b * u2 + in[ i[ 3 ] ].b * u3;
			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( color( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) ) );
		}

		template < class T, class Allocator >
		static typename array2< T, Allocator >::value_type cubic___( const array2< T, Allocator > &in,
																	typename array2< T, Allocator >::size_type i[4],
																	typename array2< T, Allocator >::size_type j[4],
																	typename array2< T, Allocator >::size_type k[4],
																	double x, double y, double z )
		{
			typedef typename array2< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double u0 = sinc( 1 + x );
			double u1 = sinc( x );
			double u2 = sinc( 1 - x );
			double u3 = sinc( 2 - x );
			double v0 = sinc( 1 + y );
			double v1 = sinc( y );
			double v2 = sinc( 1 - y );
			double v3 = sinc( 2 - y );
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
			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( color( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) ) );
		}

		template < class T, class Allocator >
		static typename array3< T, Allocator >::value_type cubic___( const array3< T, Allocator > &in,
																	typename array3< T, Allocator >::size_type i[4],
																	typename array3< T, Allocator >::size_type j[4],
																	typename array3< T, Allocator >::size_type k[4],
																	double x, double y, double z )
		{
			typedef typename array3< T, Allocator >::value_type color;
			typedef typename color::value_type value_type;
			double min = type_limits< value_type >::minimum( );
			double max = type_limits< value_type >::maximum( );
			double u0 = sinc( 1 + x );
			double u1 = sinc( x );
			double u2 = sinc( 1 - x );
			double u3 = sinc( 2 - x );
			double v0 = sinc( 1 + y );
			double v1 = sinc( y );
			double v2 = sinc( 1 - y );
			double v3 = sinc( 2 - y );
			double w0 = sinc( 1 + z );
			double w1 = sinc( z );
			double w2 = sinc( 1 - z );
			double w3 = sinc( 2 - z );
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
			r = r > min ? r : min;
			r = r < max ? r : max;
			g = g > min ? g : min;
			g = g < max ? g : max;
			b = b > min ? b : min;
			b = b < max ? b : max;
			return( color( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) ) );
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

					out( i, j, k ) = static_cast< out_value_type >( _cubic_< is_color< value_type >::value >::cubic___( in, ii, jj, kk, x, y, z ) );
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


	// 3次補間
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
			Linear,
			Cubic,
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
		virtual thread_exit_type thread_function( const interpolate_thread &p )
		{
			switch( interpolate_ )
			{
			case Nearest:
				nearest__( *in_, *out_, thread_id_, thread_num_ );
				break;

			case Cubic:
				cubic__( *in_, *out_, thread_id_, thread_num_ );
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


namespace nearest
{
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Nearest, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}
}


namespace linear
{
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Linear, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}
}



namespace cubic
{
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
						typename array< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out,
					typename array1< T1, Allocator1 >::size_type width, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out,
					typename array2< T1, Allocator1 >::size_type width, typename array2< T1, Allocator1 >::size_type height,
					typename array2< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void interpolate( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out,
					typename array3< T1, Allocator1 >::size_type width,
					typename array3< T1, Allocator1 >::size_type height,
					typename array3< T1, Allocator1 >::size_type depth,
					typename array3< T1, Allocator1 >::size_type thread_num = 0 )
	{
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
			return;
		}

		interpolate_thread *thread = new interpolate_thread[ thread_num ];

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( in, out, interpolate_thread::Cubic, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}

		delete [] thread;
	}
}


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_INTERPOLATE__
