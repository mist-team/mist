#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__

#include "../mist.h"
#include "../config/mist_conf.h"
#include "../config/mist_alloc.h"
#include "../config/type_trait.h"
#include "../limits.h"
#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN

namespace linear_filter
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
	template < class T_in >
	inline double margin( const mist::array1< T_in >& in, const int& i )
	{
		return( in( i ) );
	}

	template < class T_in >
	inline double margin( const mist::array2< T_in >& in, const int& i, const int& j )
	{
		return( in( i, j ) );
	}

	template < class T_in >
	inline double margin( const mist::array3< T_in >& in, const int& i, const int& j, const int& k )
	{
		return( in( i, j, k ) );
	}

	//各要素に対する演算を画面端とそれ以外に場合分け
	template < class T_in, class T_kernel >
	inline double get_value( const mist::array1< T_in >& in, const mist::array1< T_kernel >& kernel, const int& i )
	{
		if( ( i >= (kernel.size1( ) / 2) ) && (i < (in.size1() - kernel.size1( ) / 2) ) )
		{
			int		ii;
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

	template < class T_in, class T_kernel >
	inline double get_value( const mist::array2< T_in >& in, const mist::array2< T_kernel >& kernel, const int& i, const int& j )
	{
		if( ( j >= (kernel.size2( ) / 2) ) && (j < (in.size2() - kernel.size2( ) / 2) ) && ( i >= (kernel.size1( ) / 2) ) && (i < (in.size1() - kernel.size1( ) / 2) ) )
		{
			int		ii, jj;
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

	template < class T_in, class T_kernel >
	inline double get_value( const mist::array3< T_in >& in, const mist::array3< T_kernel >& kernel, const int& i, const int& j, const int& k )
	{
		if( ( k >= (kernel.size3( ) / 2) ) && (k < (in.size3() - kernel.size3( ) / 2) ) && ( j >= (kernel.size2( ) / 2) ) && (j < (in.size2() - kernel.size2( ) / 2) ) && ( i >= (kernel.size1( ) / 2) ) && (i < (in.size1() - kernel.size1( ) / 2) ) )
		{
			int		ii, jj, kk;
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
	template < class T_in, class T_out, class T_kernel >
	void apply( const mist::array1< T_in >& in, mist::array1< T_out >& out, const mist::array1< T_kernel >& kernel )
	{
		out.resize( in.size1( ) );
		out.reso1( in.reso1( ) );
		int		i;
		for( i = 0 ; i < in.size1() ; i++ )
		{
			 out( i ) = type_limits_2< T_out >::cast( get_value( in, kernel, i ) );	
		}
	}

	template < class T_in, class T_out, class T_kernel >
	void apply( const mist::array2< T_in >& in, mist::array2< T_out >& out, const mist::array2< T_kernel >& kernel )
	{
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		int		i, j;
		for( j = 0 ; j < in.size2() ; j++ )
		{
			for( i = 0 ; i < in.size1() ; i++ )
			{
				out( i, j ) = type_limits_2< T_out >::cast( get_value( in, kernel, i, j ) );
			}
		}
	}

	template < class T_in, class T_out, class T_kernel >
	void apply( const mist::array3< T_in >& in, mist::array3< T_out >& out, const mist::array3< T_kernel >& kernel )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
		int		i, j, k;
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
	}

	//カーネルの正規化
	template < class T_in, class T_out >
	void normalize( const mist::array1< T_in >& in, mist::array1< T_out >& out)
	{
		double	norm;
		int		i;
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
	}

	template < class T_in, class T_out >
	void normalize( const mist::array2< T_in >& in, mist::array2< T_out >& out )
	{
		double	norm;
		int		i, j;
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
	}

	template < class T_in, class T_out >
	void normalize( const mist::array3< T_in >& in, mist::array3< T_out >& out )
	{
		double	norm;
		int		i, j, k;
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
	}

	//array1用ガウシアン
	template < class T_in, class T_out >
	void gaussian( const mist::array1< T_in >& in, mist::array1< T_out >& out )
	{
		mist::array1< double > kernel( 3 );

		kernel( 0 ) = 0.60653066;
		kernel( 1 ) = 1.0;
		kernel( 2 ) = 0.60653066;

		mist::array1< double >	n_kernel;
		normalize( kernel, n_kernel );

		apply( in, out, n_kernel );
	}

	//array2用ガウシアン
	template < class T_in, class T_out >
	void gaussian( const mist::array2< T_in >& in, mist::array2< T_out >& out )
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
		normalize( kernel, n_kernel );

		apply( in, out, n_kernel );
	}

	//array3用ガウシアン
	template < class T_in, class T_out >
	void gaussian( const mist::array3< T_in >& in, mist::array3< T_out >& out )
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
		normalize( kernel, n_kernel );

		apply( in, out, n_kernel );
	}

	//array1用ラプラシアン
	template < class T_in, class T_out >
	void laplacian( const mist::array1< T_in >& in, mist::array1< T_out >& out )
	{
		mist::array1< double > kernel( 3 );

		kernel( 0 ) = 1.0;
		kernel( 1 ) = -2.0;
		kernel( 2 ) = 1.0;
		
		apply( in, out, kernel );
	}

	//array2用ラプラシアン
	template < class T_in, class T_out >
	void laplacian( const mist::array2< T_in >& in, mist::array2< T_out >& out )
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

		apply( in, out, kernel );
	}

	//array3用ラプラシアン
	template < class T_in, class T_out >
	void laplacian( const mist::array3< T_in >& in, mist::array3< T_out >& out )
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

		apply( in, out, kernel );
	}
}
	

// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
