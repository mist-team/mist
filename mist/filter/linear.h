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
	//double型から任意の型への変換
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
			if( ( arg >= minimum( ) ) && ( arg <= maximum( ) ) )
			{
				return( arg );
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

	//画面端の処理
	template < class T_in, class Allocator_in >
	inline double margin( const mist::array< T_in, Allocator_in >& in, typename mist::array< T_in, Allocator_in >::size_type &i )
	{
		return( in( i ) );
	} 

	template < class T_in, class Allocator_in >
	inline double margin( const mist::array1< T_in, Allocator_in >& in, typename mist::array1< T_in, Allocator_in >::size_type &i )
	{
		return( in( i ) );
	}

	template < class T_in, class Allocator_in >
	inline double margin( const mist::array2< T_in, Allocator_in >& in, const typename mist::array2< T_in, Allocator_in >::size_type &i, 
							const typename mist::array2< T_in, Allocator_in >::size_type &j )
	{
		return( in( i, j ) );
	}

	template < class T_in, class Allocator_in >
	inline double margin( const mist::array3< T_in, Allocator_in >& in, const typename mist::array3< T_in, Allocator_in >::size_type &i,
							const typename mist::array3< T_in, Allocator_in >::size_type &j, const typename mist::array3< T_in, Allocator_in >::size_type &k )
	{
		return( in( i, j, k ) );
	}

	//各要素に対する演算を画面端とそれ以外に場合分け
	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array< T_in, Allocator_in > &in, const mist::array< T_kernel, Allocator_kernel > &kernel, const typename mist::array< T_kernel, Allocator_kernel >::size_type &i )
	{
		if( ( i >= (kernel.size( ) / 2) ) && (i < (in.size() - kernel.size( ) / 2) ) )
		{
			typedef typename mist::array< T_kernel, Allocator_kernel >::size_type size_type; 
			syze_type ii;
			double	val = 0;
			for( ii = 0 ; ii < kernel.size( ) ; ii++ )
			{
				val += in( i - kernel.size( ) / 2 + ii ) * kernel( ii );
			}
			return( val );
		}
		else
		{
			return( margin( in, i ) );
		}
	}

	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array1< T_in, Allocator_in > &in, const mist::array1< T_kernel, Allocator_kernel > &kernel, const typename mist::array1< T_kernel, Allocator_kernel >::size_type &i )
	{
		if( ( i >= (kernel.size1( ) / 2) ) && (i < (in.size1() - kernel.size1( ) / 2) ) )
		{
			typedef typename mist::array1< T_kernel, Allocator_kernel >::size_type size_type; 
			syze_type ii;
			double	val = 0;
			for( ii = 0 ; ii < kernel.size1( ) ; ii++ )
			{
				val += in( i - kernel.size1( ) / 2 + ii ) * kernel( ii );
			}
			return( val );
		}
		else
		{
			return( margin( in, i ) );
		}
	}

	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array2< T_in, Allocator_in > &in, const mist::array2< T_kernel, Allocator_kernel > &kernel,
								const typename mist::array2< T_kernel, Allocator_kernel >::size_type &i, const typename mist::array2< T_kernel, Allocator_kernel >::size_type &j )
	{
		if( ( j >= (kernel.size2( ) / 2) ) && (j < (in.size2() - kernel.size2( ) / 2) ) && ( i >= (kernel.size1( ) / 2) ) && (i < (in.size1() - kernel.size1( ) / 2) ) )
		{
			typedef typename mist::array2< T_kernel, Allocator_kernel >::size_type size_type;
			size_type ii, jj;
			double	val = 0;
			for( jj = 0 ; jj < kernel.size2( ) ; jj++ )
			{
				for( ii = 0 ; ii < kernel.size1( ) ; ii++ )
				{
					val += in( i - kernel.size1( ) / 2 + ii, j - kernel.size2( ) / 2 + jj ) * kernel( ii, jj );
				}
			}
			return( val );
		}
		else
		{
			return( margin( in, i, j ) );
		}
	}

	template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
	inline double get_value( const mist::array3< T_in, Allocator_in >& in, const mist::array3< T_kernel,Allocator_kernel >& kernel,
							const typename mist::array3< T_kernel, Allocator_kernel >::size_type &i, const typename mist::array3< T_kernel, Allocator_kernel >::size_type &j, const typename mist::array3< T_kernel, Allocator_kernel >::size_type &k )
	{
		if( ( k >= (kernel.size3( ) / 2) ) && (k < (in.size3() - kernel.size3( ) / 2) ) && ( j >= (kernel.size2( ) / 2) ) && (j < (in.size2() - kernel.size2( ) / 2) ) && ( i >= (kernel.size1( ) / 2) ) && (i < (in.size1() - kernel.size1( ) / 2) ) )
		{
			typedef typename mist::array3< T_kernel, Allocator_kernel >::size_type size_type;
			size_type ii, jj, kk;
			double	val = 0;
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
		else
		{
			return( margin( in, i, j, k ) );
		}
	}

	//カーネル適用
	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out, const mist::array< T_kernel, Allocator_kernel >& kernel )
	{
		out.resize( in.size( ) );

		typedef typename mist::array< T_in, Allocator_in >::size_type size_type;
		size_type i;
		for( i = 0 ; i < in.size() ; i++ )
		{
			 out( i ) = type_limits_2< T_out >::cast( get_value( in, kernel, i ) );	
		}		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out, const mist::array1< T_kernel, Allocator_kernel >& kernel )
	{
		out.resize( in.size1( ) );

		typedef typename mist::array1< T_in, Allocator_in >::size_type size_type;
		size_type i;
		for( i = 0 ; i < in.size1() ; i++ )
		{
			 out( i ) = type_limits_2< T_out >::cast( get_value( in, kernel, i ) );	
		}

		out.reso1( in.reso1( ) );		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out, const mist::array2< T_kernel, Allocator_kernel >& kernel )
	{
		out.resize( in.size1( ), in.size2( ) );
		
		typedef typename mist::array2< T_in, Allocator_in >::size_type size_type;
		size_type i, j;
		for( j = 0 ; j < in.size2() ; j++ )
		{
			for( i = 0 ; i < in.size1() ; i++ )
			{
				out( i, j ) = type_limits_2< T_out >::cast( get_value( in, kernel, i, j ) );
			}
		}

		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out, const mist::array3< T_kernel, Allocator_kernel >& kernel )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		
		typedef typename mist::array3< T_in, Allocator_in >::size_type size_type;
		size_type i, j, k;
		for( k = 0 ; k < in.size3( ) ; k++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				for( i = 0 ; i < in.size1( ) ; i++ )
				{
					out( i, j, k ) = type_limits_2< T_out >::cast( get_value( in, kernel, i, j, k ) );
				}
			}
		}

		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
	}

	//カーネルの正規化
	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out )
	{
		typedef typename mist::array< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array< T_out, Allocaotr_out >::value_type value_type;
		size_type i;
		value_type norm;
		norm = 0;
		for( i = 0 ; i < in.size( ) ; i++ )
		{
			norm += in( i );
		}
		out.resize( in.size( ) );
		for( i = 0 ; i < out.size( ) ; i++ )
		{
			out( i ) =  in( i ) / norm;
		}
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out )
	{
		typedef typename mist::array1< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array1< T_out, Allocator_out >::value_type value_type;
		size_type i;
		value_type norm;
		norm = 0;
		for( i = 0 ; i < in.size1( ) ; i++ )
		{
			norm += in( i );
		}
		out.resize( in.size1( ) );
		for( i = 0 ; i < out.size1( ) ; i++ )
		{
			out( i ) =  in( i ) / norm;
		}
		out.reso1( in.reso1( ) );
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out )
	{
		typedef typename mist::array2< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array2< T_out, Allocator_out >::value_type value_type;
		size_type i, j;
		value_type norm;
		norm = 0;
		for( i = 0 ; i < in.size1( ) ; i++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				norm += in( i, j );
			}
		}
		out.resize( in.size1( ), in.size2( ) );
		for( i = 0 ; i < out.size1( ) ; i++ )
		{
			for( j = 0 ; j < out.size2() ; j++ )
			{
				out( i, j ) = in( i, j ) / norm;
			}
		}
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out >
	void normalize( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out )
	{
		typedef typename mist::array3< T_in, Allocator_in >::size_type size_type;
		typedef typename mist::array3< T_out, Allocator_out >::value_type value_type;
		size_type i, j, k;
		value_type norm;
		norm = 0;
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
		for( i = 0 ; i < out.size1() ; i++ )
		{
			for( j = 0 ; j < out.size2() ; j++ )
			{
				for( k = 0 ; k < out.size3() ; k++ )
				{
					out( i, j, k ) = in( i, j, k ) / norm;
				}
			}
		}
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
	}
}

//kernelを渡す場合
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out, const mist::array1< T_kernel, Allocator_kernel >& kernel )
{
	__linear_filter__::apply( in, out, kernel );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const mist::array2< T_in, Allocator_in >& in, mist::array2< T_out, Allocator_out >& out, const mist::array2< T_kernel, Allocator_kernel >& kernel )
{
	__linear_filter__::apply( in, out, kernel );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const mist::array3< T_in, Allocator_in >& in, mist::array3< T_out, Allocator_out >& out, const mist::array3< T_kernel, Allocator_kernel >& kernel )
{
	__linear_filter__::apply( in, out, kernel );
}

//array1用ガウシアン
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

//array2用ガウシアン
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

//array3用ガウシアン
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

//array1用ラプラシアン
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const mist::array< T_in, Allocator_in >& in, mist::array< T_out, Allocator_out >& out )
{
	mist::array< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const mist::array1< T_in, Allocator_in >& in, mist::array1< T_out, Allocator_out >& out )
{
	mist::array1< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel );
}

//array2用ラプラシアン
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

//array3用ラプラシアン
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

	

// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
