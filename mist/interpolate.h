#ifndef __INCLUDE_MIST_INTERPOLATE__
#define __INCLUDE_MIST_INTERPOLATE__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "thread.h"
#endif


// mistñºëOãÛä‘ÇÃénÇ‹ÇË
_MIST_BEGIN


// ç≈ãﬂñTå^ï‚ä‘
namespace __nearest__
{
	template < class T, class Allocator >
	inline typename array< T, Allocator >::value_type nearest( const array< T, Allocator > &in, double x, double y, double z )
	{
		typedef typename array< T, Allocator >::size_type size_type;
		return( in[ static_cast< size_type >( x + 0.5 ) ] );
	}

	template < class T, class Allocator >
	inline typename array2< T, Allocator >::value_type nearest( const array2< T, Allocator > &in, double x, double y, double z )
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		return( in( static_cast< size_type >( x + 0.5 ), static_cast< size_type >( y + 0.5 ) ) );
	}

	template < class T, class Allocator >
	inline typename array3< T, Allocator >::value_type nearest( const array3< T, Allocator > &in, double x, double y, double z )
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		return( in( static_cast< size_type >( x + 0.5 ), static_cast< size_type >( y + 0.5 ), static_cast< size_type >( z + 0.5 ) ) );
	}

	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
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

		double sx = static_cast< double >( ow ) / static_cast< double >( iw );
		double sy = static_cast< double >( oh ) / static_cast< double >( ih );
		double sz = static_cast< double >( od ) / static_cast< double >( id );
		double x, y, z;

		for( k = thread_idz ; k < od ; k += thread_numz )
		{
			z = sz * k;
			for( j = thread_idy ; j < oh ; j += thread_numy )
			{
				y = sy * j;
				for( i = 0 ; i < ow ; i++ )
				{
					x = sx * i;
					out( i, j, k ) = static_cast< out_value_type >( nearest( in, x, y, z ) );
				}
			}
		}
	}
}


// ç≈ãﬂñTå^ï‚ä‘
namespace __linear__
{
	template < class T, class Allocator >
	inline double linear( const array< T, Allocator > &in,
							typename array< T, Allocator >::size_type i1,
							typename array< T, Allocator >::size_type i2,
							typename array< T, Allocator >::size_type j1,
							typename array< T, Allocator >::size_type j2,
							typename array< T, Allocator >::size_type k1,
							typename array< T, Allocator >::size_type k2,
							double x, double y, double z )
	{
		return( in[ i1 ] * ( 1.0 - x ) + in[ i2 ] * x );
	}

	template < class T, class Allocator >
	inline double linear( const array2< T, Allocator > &in,
							typename array2< T, Allocator >::size_type i1,
							typename array2< T, Allocator >::size_type i2,
							typename array2< T, Allocator >::size_type j1,
							typename array2< T, Allocator >::size_type j2,
							typename array2< T, Allocator >::size_type k1,
							typename array2< T, Allocator >::size_type k2,
							double x, double y, double z )
	{
		return( ( in( i1, j1 ) * ( 1.0 - x ) + in( i2, j1 ) * x ) * ( 1.0 - y ) + ( in( i1, j2 ) * ( 1.0 - x ) + in( i2, j2 ) * x ) * y );
	}

	template < class T, class Allocator >
	inline double linear( const array3< T, Allocator > &in,
							typename array3< T, Allocator >::size_type i1,
							typename array3< T, Allocator >::size_type i2,
							typename array3< T, Allocator >::size_type j1,
							typename array3< T, Allocator >::size_type j2,
							typename array3< T, Allocator >::size_type k1,
							typename array3< T, Allocator >::size_type k2,
							double x, double y, double z )
	{
		return( ( ( in( i1, j1, k1 ) * ( 1.0 - x ) + in( i2, j1, k1 ) * x ) * ( 1.0 - y )	+ ( in( i1, j2, k1 ) * ( 1.0 - x ) + in( i2, j2, k1 ) * x ) * y ) * ( 1.0 - z )
							+ ( ( in( i1, j1, k2 ) * ( 1.0 - x ) + in( i2, j1, k2 ) * x ) * ( 1.0 - y )	+ ( in( i1, j2, k2 ) * ( 1.0 - x ) + in( i2, j2, k2 ) * x ) * y ) * z );
	}

	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
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

		double sx = static_cast< double >( ow ) / static_cast< double >( iw );
		double sy = static_cast< double >( oh ) / static_cast< double >( ih );
		double sz = static_cast< double >( od ) / static_cast< double >( id );
		double x, y, z;

		for( k = thread_idz ; k < od - 1 ; k += thread_numz )
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
				for( i = 0 ; i < ow ; i++ )
				{
					x = sx * i;
					i1 = static_cast< size_type >( x );
					x -= i1;
					i2 = i1 < iw - 1 ? i1 + 1 : i1;
					out( i, j, k ) = static_cast< out_value_type >( linear( in, i1, i2, j1, j2, k1, k2, x, y, z ) );
				}
			}
		}
	}
}



// 3éüì‡ë}ï‚ä‘
namespace __cubic__
{
	inline double sinc( double t )
	{
		double ret = 0.0;
		t = fabs( t );

		if( t >= 0.0 && t < 1.0 )
		{
			ret = 1.0 + ( -2.0 + t ) * t * t;
		}
		else if( t >= 1.0 && t < 2.0 )
		{
			ret = 4.0 + ( -8.0 + ( 5.0 - t ) * t ) * t;
		}

		return ( ret );
	}

	template < class T, class Allocator >
	inline double cubic( const array< T, Allocator > &in,
							typename array< T, Allocator >::size_type i[4],
							typename array< T, Allocator >::size_type j[4],
							typename array< T, Allocator >::size_type k[4],
							double x, double y, double z )
	{
		double u0 = sinc( 1 + x );
		double u1 = sinc( x );
		double u2 = sinc( 1 - x );
		double u3 = sinc( 2 - x );
		return( in[ i[ 0 ] ] * u0 + in[ i[ 1 ] ] * u1 + in[ i[ 2 ] ] * u2 + in[ i[ 3 ] ] * u3 );
	}

	template < class T, class Allocator >
	inline double cubic( const array2< T, Allocator > &in,
							typename array2< T, Allocator >::size_type i[4],
							typename array2< T, Allocator >::size_type j[4],
							typename array2< T, Allocator >::size_type k[4],
							double x, double y, double z )
	{
		double u0 = sinc( 1 + x );
		double u1 = sinc( x );
		double u2 = sinc( 1 - x );
		double u3 = sinc( 2 - x );
		double v0 = sinc( 1 + y );
		double v1 = sinc( y );
		double v2 = sinc( 1 - y );
		double v3 = sinc( 2 - y );
		return( ( in( i[ 0 ], j[ 0 ] ) * u0 + in( i[ 1 ], j[ 0 ] ) * u1 + in( i[ 2 ], j[ 0 ] ) * u2 + in( i[ 3 ], j[ 0 ] ) * u3 ) * v0
					+ ( in( i[ 0 ], j[ 1 ] ) * u0 + in( i[ 1 ], j[ 1 ] ) * u1 + in( i[ 2 ], j[ 1 ] ) * u2 + in( i[ 3 ], j[ 1 ] ) * u3 ) * v1
					+ ( in( i[ 0 ], j[ 2 ] ) * u0 + in( i[ 1 ], j[ 2 ] ) * u1 + in( i[ 2 ], j[ 2 ] ) * u2 + in( i[ 3 ], j[ 2 ] ) * u3 ) * v2
					+ ( in( i[ 0 ], j[ 3 ] ) * u0 + in( i[ 1 ], j[ 3 ] ) * u1 + in( i[ 2 ], j[ 3 ] ) * u2 + in( i[ 3 ], j[ 3 ] ) * u3 ) * v3 );
	}

	template < class T, class Allocator >
	inline double cubic( const array3< T, Allocator > &in,
							typename array3< T, Allocator >::size_type i[4],
							typename array3< T, Allocator >::size_type j[4],
							typename array3< T, Allocator >::size_type k[4],
							double x, double y, double z )
	{
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
		double p1 = (  ( in( i[ 0 ], j[ 0 ] ) * u0 + in( i[ 1 ], j[ 0 ] ) * u1 + in( i[ 2 ], j[ 0 ] ) * u2 + in( i[ 3 ], j[ 0 ] ) * u3 ) * v0
					+ ( in( i[ 0 ], j[ 1 ] ) * u0 + in( i[ 1 ], j[ 1 ] ) * u1 + in( i[ 2 ], j[ 1 ] ) * u2 + in( i[ 3 ], j[ 1 ] ) * u3 ) * v1
					+ ( in( i[ 0 ], j[ 2 ] ) * u0 + in( i[ 1 ], j[ 2 ] ) * u1 + in( i[ 2 ], j[ 2 ] ) * u2 + in( i[ 3 ], j[ 2 ] ) * u3 ) * v2
					+ ( in( i[ 0 ], j[ 3 ] ) * u0 + in( i[ 1 ], j[ 3 ] ) * u1 + in( i[ 2 ], j[ 3 ] ) * u2 + in( i[ 3 ], j[ 3 ] ) * u3 ) * v3 );
	}

	template < class Array1, class Array2 >
	void interpolate( const Array1 &in, Array2 &out,
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

		double sx = static_cast< double >( ow ) / static_cast< double >( iw );
		double sy = static_cast< double >( oh ) / static_cast< double >( ih );
		double sz = static_cast< double >( od ) / static_cast< double >( id );
		double x, y, z;

		for( k = thread_idz ; k < od - 1 ; k += thread_numz )
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
				for( i = 0 ; i < ow ; i++ )
				{
					x = sx * i;
					i1 = static_cast< size_type >( x );
					x -= i1;
					i2 = i1 < iw - 1 ? i1 + 1 : i1;
					out( i, j, k ) = static_cast< out_value_type >( linear( in, i1, i2, j1, j2, k1, k2, x, y, z ) );
				}
			}
		}
	}
}

// mistñºëOãÛä‘ÇÃèIÇÌÇË
_MIST_END


#endif // __INCLUDE_MIST_INTERPOLATE__
