#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__

#include "../mist.h"
#include "../config/mist_conf.h"
#include "../config/mist_alloc.h"

// mist名前空間の始まり
_MIST_BEGIN

template < class Top, class Allocator = std::allocator< Top > > 

class linear_operator
{
	array< Top, Allocator >	l_operator_;
	Allocator::size_type	size1_;
	Allocator::size_type	size2_;
	Allocator::size_type	size3_;

public:

	//コンストラクタ

	linear_operator( ) :
		l_operator_( 1 ),
		size1_( 1 ),
		size2_( 1 ),
		size3_( 1 )
	{
	}

	linear_operator( mist::array1< Top, Allocator >& l_operator ) :
		l_operator_( l_operator ),
		size1_( l_operator.size1( ) ),
		size2_( 1 ),
		size3_( 1 )
	{
	}

	linear_operator( mist::array2< Top, Allocator >& l_operator ) :
		l_operator_( l_operator ),
		size1_( l_operator.size1( ) ),
		size2_( l_operator.size2( ) ),
		size3_( 1 )
	{
	}

	linear_operator( mist::array3< Top, Allocator >& l_operator ) :
		l_operator_( l_operator ),
		size1_( l_operator.size1( ) ),
		size2_( l_operator.size2( ) ),
		size3_( l_operator.size3( ) )
	{
	}

	//オペレータ定義

	void define( )
	{
		l_operator_.resize( 1 );
		size1_ = size2_ = size3_ = 1;
	}

	void define( mist::array1< Top, Allocator >& l_operator )
	{
		l_operator_ = l_operator;
		size1_ = l_operator.size1( );
		size2_ = size3_ = 1;
	}

	void define( mist::array2< Top, Allocator >& l_operator )
	{
		l_operator_ = l_operator;
		size1_ = l_operator.size1( );
		size2_ = l_operator.size2( );
		size3_ = 1;
	}

	void define( mist::array3< Top, Allocator >& l_operator )
	{
		l_operator_ = l_operator;
		size1_ = l_operator.size1( );
		size2_ = l_operator.size2( );
		size3_ = l_operator.size3( );
	}

	void normalize()
	{
		Top norm = Top( );

		int		i;
		for( i = 0 ; i < l_operator_.size() ; i++ )
		{
			norm += l_operator_( i );
		}
		for( i = 0 ; i < l_operator_.size() ; i++ )
		{
			l_operator_( i ) /= norm;
		}

	}

	//オペレータ適用

	template < class Tin, class Tout >
	void apply( const mist::array1< Tin >& input, mist::array1< Tout >& output ) const
	{
		output.resize( input.size1() );

		int		i, ii;
		Top		result;
		for( i = size1_ / 2 ; i < (input.size1() - size1_ / 2) ; i++ )
		{
			result = Top( );
			for( ii = 0 ; ii < size1_ ; ii++ ){
				result += input( i - size1_ / 2 + ii) * l_operator_( ii );
			}
			output( i ) = result;
		}
	}

	template < class Tin, class Tout >
	void apply( const mist::array2< Tin >& input, mist::array2< Tout >& output ) const
	{
		output.resize( input.size1( ), input.size2( ) );

		int		i, j, ii, jj;
		Top		result;
		for( j = (size2_ / 2) ; j < (input.size2() - size2_ / 2) ; j++ )
		{
			for( i = (size1_ / 2) ; i < (input.size1() - size1_ / 2) ; i++ )
			{
				result = Top( );
				for( jj = 0 ; jj < size2_ ; jj++ )
				{
					for( ii = 0 ; ii < size1_ ; ii++ )
					{
						result += input( i - size1_ / 2 + ii, j - size2_ / 2 + jj) * l_operator_( jj * size1_ + ii );
					}
				}
				output( i, j ) = result;
			}
		}
	}

	template < class Tin, class Tout >
	void apply( const mist::array3< Tin >& input, mist::array3< Tout >& output ) const
	{
		output.resize( input.size1( ), input.size2( ), input.size3( ) );

		int		i, j, k, ii, jj, kk;
		Top		result;
		for( k = (size3_ / 2) ; k < (input.size3() - size3_ / 2) ; k++ )
		{
			for( j = (size2_ / 2) ; j < (input.size2() - size2_ / 2) ; j++ )
			{
				for( i = (size1_ / 2) ; i < (input.size1() - size1_ / 2) ; i++ )
				{
					result = Top( );
					for( kk = 0 ; kk < size3_ ; kk++ )
					{
						for( jj = 0 ; jj < size2_ ; jj++ )
						{
							for( ii = 0 ; ii < size1_ ; ii++ )
							{
								result += input( i - size1_ / 2 + ii, j - size2_ / 2 + jj, k - size3_ / 2 + kk ) * l_operator_( kk * size1_ * size2_ + jj * size1_ + ii);
							}
						}
					}
					output( i, j, k ) = result;
				}
			}
		}
	}

	~linear_operator( ) 
	{
		l_operator_.clear( );
	}

};

namespace linear_filter
{
	
	//array1用ガウシアン
	template < class Tin, class Tout >
	void gaussian( const mist::array1< Tin >& input, mist::array1< Tout >& output )
	{
		mist::array1< double > gaussian_array( 3 );

		gaussian_array( 0 ) = 0.60653066;
		gaussian_array( 1 ) = 1.0;
		gaussian_array( 2 ) = 0.60653066;

		linear_operator< double > l_operator( gaussian_array );
		l_operator.normalize();

		l_operator.apply( input, output );
	}

	//array2用ガウシアン
	template < class Tin, class Tout >
	void gaussian( const mist::array2< Tin >& input, mist::array2< Tout >& output )
	{
		mist::array2< double > gaussian_array( 3, 3 );

		gaussian_array( 0, 0 ) = 0.367879441;
		gaussian_array( 1, 0 ) = 0.60653066;
		gaussian_array( 2, 0 ) = 0.367879441;
		gaussian_array( 0, 1 ) = 0.60653066;
		gaussian_array( 1, 1 ) = 1.0;
		gaussian_array( 2, 1 ) = 0.60653066;
		gaussian_array( 0, 2 ) = 0.367879441;
		gaussian_array( 1, 2 ) = 0.60653066;
		gaussian_array( 2, 2 ) = 0.367879441;

		linear_operator< double > l_operator( gaussian_array );
		l_operator.normalize();

		l_operator.apply( input, output );
	}

	//array3用ガウシアン
	template < class Tin, class Tout >
	void gaussian( const mist::array3< Tin >& input, mist::array3< Tout >& output )
	{
		mist::array3< double > gaussian_array( 3, 3, 3 );

		gaussian_array( 0, 0, 0 ) = 0.22313016;
		gaussian_array( 1, 0, 0 ) = 0.367879441;
		gaussian_array( 2, 0, 0 ) = 0.22313016;
		gaussian_array( 0, 1, 0 ) = 0.367879441;
		gaussian_array( 1, 1, 0 ) = 0.60653066;
		gaussian_array( 2, 1, 0 ) = 0.367879441;
		gaussian_array( 0, 2, 0 ) = 0.22313016;
		gaussian_array( 1, 2, 0 ) = 0.367879441;
		gaussian_array( 2, 2, 0 ) = 0.22313016;

		gaussian_array( 0, 0, 1 ) = 0.367879441;
		gaussian_array( 1, 0, 1 ) = 0.60653066;
		gaussian_array( 2, 0, 1 ) = 0.367879441;
		gaussian_array( 0, 1, 1 ) = 0.60653066;
		gaussian_array( 1, 1, 1 ) = 1.0;
		gaussian_array( 2, 1, 1 ) = 0.60653066;
		gaussian_array( 0, 2, 1 ) = 0.367879441;
		gaussian_array( 1, 2, 1 ) = 0.60653066;
		gaussian_array( 2, 2, 1 ) = 0.367879441;

		gaussian_array( 0, 0, 2 ) = 0.22313016;
		gaussian_array( 1, 0, 2 ) = 0.367879441;
		gaussian_array( 2, 0, 2 ) = 0.22313016;
		gaussian_array( 0, 1, 2 ) = 0.367879441;
		gaussian_array( 1, 1, 2 ) = 0.60653066;
		gaussian_array( 2, 1, 2 ) = 0.367879441;
		gaussian_array( 0, 2, 2 ) = 0.22313016;
		gaussian_array( 1, 2, 2 ) = 0.367879441;
		gaussian_array( 2, 2, 2 ) = 0.22313016;

		linear_operator< double > l_operator( gaussian_array );
		l_operator.normalize();

		l_operator.apply( input, output );
	}

	//array1用ラプラシアン
	template < class Tin, class Tout >
	void laplacian( const mist::array1< Tin >& input, mist::array1< Tout >& output )
	{
		mist::array1< double > laplacian_array( 3 );

		laplacian_array( 0 ) = 1.0;
		laplacian_array( 1 ) = -2.0;
		laplacian_array( 2 ) = 1.0;

		linear_operator< double > l_operator( laplacian_array );
		
		l_operator.apply( input, output );
	}

	//array2用ラプラシアン
	template < class Tin, class Tout >
	void laplacian( const mist::array2< Tin >& input, mist::array2< Tout >& output )
	{
		mist::array2< double > laplacian_array( 3, 3 );

		laplacian_array( 0, 0 ) = 1.0;
		laplacian_array( 1, 0 ) = 1.0;
		laplacian_array( 2, 0 ) = 1.0;
		laplacian_array( 0, 1 ) = 1.0;
		laplacian_array( 1, 1 ) = -8.0;
		laplacian_array( 2, 1 ) = 1.0;
		laplacian_array( 0, 2 ) = 1.0;
		laplacian_array( 1, 2 ) = 1.0;
		laplacian_array( 2, 2 ) = 1.0;

		linear_operator< double > l_operator( laplacian_array );

		l_operator.apply( input, output );
	}

	//array3用ラプラシアン
	template < class Tin, class Tout >
	void laplacian( const mist::array3< Tin >& input, mist::array3< Tout >& output )
	{
		mist::array3< double > laplacian_array( 3, 3, 3 );

		laplacian_array( 0, 0, 0 ) = 1.0;
		laplacian_array( 1, 0, 0 ) = 1.0;
		laplacian_array( 2, 0, 0 ) = 1.0;
		laplacian_array( 0, 1, 0 ) = 1.0;
		laplacian_array( 1, 1, 0 ) = 1.0;
		laplacian_array( 2, 1, 0 ) = 1.0;
		laplacian_array( 0, 2, 0 ) = 1.0;
		laplacian_array( 1, 2, 0 ) = 1.0;
		laplacian_array( 2, 2, 0 ) = 1.0;

		laplacian_array( 0, 0, 1 ) = 1.0;
		laplacian_array( 1, 0, 1 ) = 1.0;
		laplacian_array( 2, 0, 1 ) = 1.0;
		laplacian_array( 0, 1, 1 ) = 1.0;
		laplacian_array( 1, 1, 1 ) = -26.0;
		laplacian_array( 2, 1, 1 ) = 1.0;
		laplacian_array( 0, 2, 1 ) = 1.0;
		laplacian_array( 1, 2, 1 ) = 1.0;
		laplacian_array( 2, 2, 1 ) = 1.0;

		laplacian_array( 0, 0, 2 ) = 1.0;
		laplacian_array( 1, 0, 2 ) = 1.0;
		laplacian_array( 2, 0, 2 ) = 1.0;
		laplacian_array( 0, 1, 2 ) = 1.0;
		laplacian_array( 1, 1, 2 ) = 1.0;
		laplacian_array( 2, 1, 2 ) = 1.0;
		laplacian_array( 0, 2, 2 ) = 1.0;
		laplacian_array( 1, 2, 2 ) = 1.0;
		laplacian_array( 2, 2, 2 ) = 1.0;

		linear_operator< double > l_operator( laplacian_array );

		l_operator.apply( input, output );
	}
}
	

// mist名前空間の終わり
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
