#ifndef __INCLUDE_BSPLINE__
#define __INCLUDE_BSPLINE__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


template < int K >
struct bspline_base
{
	static double Base( const std::vector< double > &knot, std::vector< double >::size_type i, double t )
	{
		double B = 0.0;

		if( knot[ i + K - 1 ] - knot[ i ] != 0 )
		{
			B = ( t - knot[ i ] ) * bspline_base< K - 1 >::Base( knot, i, t ) / ( knot[ i + K - 1 ] - knot[ i ] );
		}

		if( knot[ i + K ] - knot[ i + 1 ] != 0 )
		{
			B += ( knot[ i + K ] - t ) * bspline_base< K - 1 >::Base( knot, i + 1, t ) / ( knot[ i + K ] - knot[ i + 1 ] );
		}
		return( B );
	}
};

template < >
struct bspline_base< 1 >
{
	static double Base( const std::vector< double > &knot, std::vector< double >::size_type i, double t )
	{
		return( ( knot[ i ] <= t && t < knot[ i + 1 ] ) ? 1.0 : 0.0 );
	}
};


template < class T, int K, class Allocator = std::allocator< T > >
class bspline : public std::vector< T, Allocator >
{
private:
	typedef std::vector< T, Allocator > base;
	typedef typename base::allocator_type allocator_type;
	typedef typename base::reference reference;
	typedef typename base::const_reference const_reference;
	typedef typename base::value_type value_type;
	typedef typename base::size_type size_type;
	typedef typename base::difference_type difference_type;
	typedef typename base::pointer pointer;
	typedef typename base::const_pointer const_pointer;

	typedef std::vector< double > knot_list;

public:
	enum BSplineMode
	{
		ROUND,
		THROUTH_SIDEPOINT,
	};

protected:
	knot_list knot_;
	BSplineMode mode_;


public:
	value_type operator( )( double t )
	{
		size_type n = base::size( ) - 1;	/* n + 1 �͐���_�̐� */
		size_type m = n + K; 				/* m + 1 �̓m�b�g�x�N�g���̐� */
		if( knot_.size( ) < m + 1 )
		{
			// �s�K�؂ȃm�b�g�x�N�g�����ݒ肳��Ă��܂�
			knot( mode_ );
		}

		t *= static_cast< double >( m - 2 * K + 2 );

		value_type p = value_type( );
		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			double B = bspline_base< K >::Base( knot_, i, t );
			p += B * base::operator[]( i );
		}

		return( p );
	}

	void knot( const knot_list &kknot )
	{
		knot_ = kknot;
	}

	void knot( BSplineMode mode )
	{
		size_type n = base::size( ) - 1;	/* n + 1 �͐���_�̐� */
		size_type m = n + K; 				/* m + 1 �̓m�b�g�x�N�g���̐� */
		size_type i;

		knot_list kknot( m + 1 );
		switch( mode )
		{
		case ROUND:
			// �����O�̏ꍇ
			for( i = 0 ; i <= m ; i++ )
			{
				kknot[ i ] = static_cast< double >( i - K + 1 );
			}
			break;

		case THROUTH_SIDEPOINT:
			// �ŏ��ƍŌ��ʂ�Ȑ��̏ꍇ
			for( i = 0 ; i < K ; i++ )
			{
				kknot[ i ] = 0.0;
			}
			for( i = K ; i < m - K + 1 ; i++ )
			{
				kknot[ i ] = static_cast< double >( i - K + 1 );
			}
			for( i = m - K + 1 ; i <= m ; i++ )
			{
				kknot[ i ] = static_cast< double >( 2 + m - 2 * K );
			}
			break;
		}

		knot( kknot );
	}

	const bspline &operator =( const bspline &b )
	{
		if( this != &b )
		{
			base::operator =( b );
			knot_ = b.knot_;
			mode_ = b.mode_;
		}
		return( *this );
	}

	bspline( const bspline &b ) : base( b ), knot_( b.knot_ ), mode_( b.mode_ )
	{
	}

	bspline( ) : mode_( THROUTH_SIDEPOINT )
	{
	}
};



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_BSPLINE__
