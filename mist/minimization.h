/// @file mist/minimization.h
//!
//! @brief �֐��̍ŏ������������߂̃��C�u����
//!
//! @section �֐��̍ŏ���
//! -# W. H. Press, S. A. Teukolsky, W. T. Vetterling, and B. P. Flannery, ``Numerical Recipes in C, The Art of Scientific Computing Second Edition,'' Cambridge University Press, pp. 321--336, 1999.
//! -# Richard P. Brent, "Algorithms for Minimization Without Derivatives", DOVER PUBLICATIONS, Mineola, New York.
//!

#ifndef __INCLUDE_MIST_MINIMIZATION__
#define __INCLUDE_MIST_MINIMIZATION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif


#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif


#ifndef __INCLUDE_MIST_LIMITS__
#include "limits.h"
#endif


#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __minimization_utility__
{
	template < class T, class Allocator, class Functor >
	struct __convert_to_vector_functor__
	{
		typedef typename matrix< T, Allocator >::size_type size_type;
		const matrix< T, Allocator > &ori_;
		const matrix< T, Allocator > &dir_;
		matrix< T, Allocator > &tmp_;
		Functor &f_;

		__convert_to_vector_functor__( const matrix< T, Allocator > &ori, const matrix< T, Allocator > &dir, matrix< T, Allocator > &tmp, Functor &f ) : ori_( ori ), dir_( dir ), tmp_( tmp ), f_( f ){ }

		double operator ()( double x )
		{
			for( size_type i = 0 ; i < ori_.size( ) ; i++ )
			{
				tmp_[ i ] = ori_[ i ] + dir_[ i ] * x;
			}
			return( f_( tmp_ ) );
		}
	};


	template < class Functor >
	struct __no_copy_constructor_functor__
	{
		Functor &f_;
		__no_copy_constructor_functor__( Functor &f ) : f_( f ){ }

		template < class PARAMETER >
		double operator ()( const PARAMETER &x )
		{
			return( f_( x ) );
		}

		template < class PARAMETER >
		double operator ()( const PARAMETER &x ) const
		{
			return( f_( x ) );
		}

		template < class PARAMETER >
		double operator ()( const PARAMETER &x, size_t index )
		{
			return( f_( x, index ) );
		}

		template < class PARAMETER >
		double operator ()( const PARAMETER &x, size_t index ) const
		{
			return( f_( x, index ) );
		}
	};


	template < class T, class Allocator, class Functor >
	struct __gradient_vector_functor__
	{
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef matrix< T, Allocator > matrix_type;
		Functor &f_;
		double d_;

		__gradient_vector_functor__( Functor &f, double d ) : f_( f ), d_( d ){ }

		const matrix_type operator ()( const matrix_type &v ) const
		{
			matrix_type dir( v.size( ), 1 ), tmp( v );
			double len = 0.0, v1, v2;
			size_type i;

			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				tmp[ i ] = v[ i ] + d_;
				v1 = f_( tmp );

				tmp[ i ] = v[ i ] - d_;
				v2 = f_( tmp );

				tmp[ i ] = v[ i ];

				dir[ i ] = v1 - v2;
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// ���z�����x�N�g���̐��K��
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}

			return( dir );
		}
	};

	template < class T, class Allocator >
	inline bool clipping( matrix< T, Allocator > &p1, matrix< T, Allocator > &p2, const double d1, const double d2, const double d )
	{
		typedef matrix< T, Allocator > matrix_type;

		bool c1 = d + d1 >= 0;
		bool c2 = d + d2 >= 0;

		if( c1 && c2 )
		{
			return( true );
		}
		else if( !c1 && !c2 )
		{
			return( false );
		}

		matrix_type v1 = p1;
		matrix_type v2 = p2;
		if( !c1 )
		{
			p1 = v2 + ( v1 - v2 ) * ( ( d + d2 ) / ( d2 - d1 ) );
		}
		if( !c2 )
		{
			p2 = v1 + ( v2 - v1 ) * ( ( d + d1 ) / ( d1 - d2 ) );
		}

		return( true );
	}

	template < class T, class Allocator >
	inline bool clipping( matrix< T, Allocator > &p1, matrix< T, Allocator > &p2, const matrix< T, Allocator > &p, const matrix< T, Allocator > &dir, const matrix< T, Allocator > &box )
	{
		typedef matrix< T, Allocator > matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef typename matrix_type::size_type size_type;

		double infinity = 0.0;
		size_type r;

		matrix_type offset( p.size( ), 1 );

		// �����Ƃ������Ίp�����̒������擾���C�o�E���f�B���O�{�b�N�X�̒��������߂�
		for( r = 0 ; r < box.rows( ) ; r++ )
		{
			double l = ( box( r, 0 ) - box( r, 1 ) );
			infinity += l * l;
			offset[ r ] = ( box( r, 0 ) + box( r, 1 ) ) / 2.0;
		}

		infinity = std::sqrt( infinity );

		// �܂��C�\���ɉ����V��ݒ肷��
		p1 = p - dir * infinity - offset;
		p2 = p + dir * infinity - offset;

		bool flag = true;

		for( r = 0 ; r < box.rows( ) ; r++ )
		{
			// �܂��C�������`�F�b�N����
			flag = flag && clipping( p1, p2, p1[ r ], p2[ r ], std::abs( box( r, 0 ) - offset[ r ] ) );

			// ���ɁC������`�F�b�N����
			flag = flag && clipping( p1, p2, -p1[ r ], -p2[ r ], std::abs( box( r, 1 ) - offset[ r ] ) );
		}

		p1 += offset;
		p2 += offset;

		return( flag );
	}

	template < class T, class Allocator >
	inline bool clipping_length( double &l1, double &l2, const matrix< T, Allocator > &p, const matrix< T, Allocator > &dir, const matrix< T, Allocator > &box )
	{
		typedef matrix< T, Allocator > matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef typename matrix_type::size_type size_type;

		matrix_type p1, p2;

		if( !clipping( p1, p2, p, dir, box ) )
		{
			return( false );
		}

		l1 = l2 = 0.0;

		// �����Ƃ������Ίp�����̒������擾���C�o�E���f�B���O�{�b�N�X�̒��������߂�
		for( size_type r = 0 ; r < p1.size( ) ; r++ )
		{
			l1 += ( p[ r ] - p1[ r ] ) * ( p[ r ] - p1[ r ] );
			l2 += ( p[ r ] - p2[ r ] ) * ( p[ r ] - p2[ r ] );
		}

		// ���l�v�Z�덷�̊֌W�ŁC�o�E���f�B���O�{�b�N�X���z����̂��������
		l1 = - std::sqrt( l1 ) + 0.000000000001;
		l2 = + std::sqrt( l2 ) - 0.000000000001;

		//std::cout << ( p + l1 * dir ).t( ) << std::endl;
		//std::cout << ( p + l2 * dir ).t( ) << std::endl;


		return( l1 < 0.0 || l2 > 0.0 );
	}

}



//! @addtogroup minimization_group �֐��̍ŏ���
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/minimization.h>
//! @endcode
//!
//!  @{



/// @brief f(x)�̈ꎟ���T���ɂ����āC�ɏ��l��^���� x ���͂ދ�Ԃ����肷��
//! 
//! @f$ a \ge b \ge c @f$ �������� @f$ c \ge b \ge a @f$ ���ŋɏ��l������Ԃ����肷��D
//!
//! @attention ���͂ƂȂ� a, b �͈قȂ�l��ݒ肷��K�v����
//! @attention �]���֐��������Ȃǂŋɏ��l�������Ȃ��ꍇ�́C��Ԃ�����ł��Ȃ�
//! 
//! @param[in,out] a  �c ��Ԃ̍��[
//! @param[in,out] b  �c ��Ԃ̐^��
//! @param[out]    c  �c ��Ԃ̉E�[
//! @param[out]	   fa �c �]���֐��l @f$ f(a) @f$
//! @param[out]    fb �c �]���֐��l @f$ f(b) @f$
//! @param[out]    fc �c �]���֐��l @f$ f(c) @f$
//! @param[in]     f  �c �]���֐�
//! 
template < class Functor >
void enclose( double &a, double &b, double &c, double &fa, double &fb, double &fc, Functor f )
{
	const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;
	const double _1_gold = 1.0 / gold;
	const double dust = type_limits< double >::minimum( );		// �[�����Z������邽�߂̃S�~�l
	const double limit = 100.0;									// ��������O��K�p�����Ԃ̍ő�X�V��

	if( a == b )
	{
		b = a + 1;
	}

	fa = f( a );
	fb = f( b );

	// fa < fb �̏ꍇ�́Ca�̕����ɋɏ�������Ǝv����̂ŁCa��b�����ւ���
	if( fa < fb )
	{
		double tmp = a;
		a = b;
		b = tmp;
		tmp = fa;
		fa = fb;
		fb = tmp;
	}

	// �����������p���āC���ɒT������_c�����肷��
	c = a + _1_gold * ( b - a );
	fc = f( c );

	// f( a ) > f( b ) < f( c ) �ƂȂ�܂ŁC��Ԃ̍X�V�𑱂���
	while( fb > fc )
	{
		// a, b, c, fa, fb, fc �̒l�ɁC�������𓖂Ă͂߂ċɏ������݂���ʒu���v�Z����
		double ba = b - a;
		double cb = c - b;
		double fcb = fc - fb;
		double fba = fb - fa;

		// �[�����Z��h�����߂ɁC���ݒl�𑫂��āC��������Ԃ��s��
		double l1 = 2.0 * ( cb * fba - ba * fcb );
		double l2 = std::abs( l1 );
		double x = b + ( ba * ba * fcb + cb * cb * fba ) / ( l1 / l2 * ( l2  + dust ) );

		if( ( c - x ) * ( x - b ) > 0 )
		{
			// ��ԓ_ x ����� b < x < c �𖞂����Ă��āC�ɏ����͂��ދ�Ԃ������邱�Ƃɐ���
			double fx = f( x );
			if( fx < fc )
			{
				// ��� ( b, c ) �ɋɏ������݂���
				a = b;
				b = x;
				fa = fb;
				fb = fx;
			}
			else
			{
				// ��� ( a, x ) �ɋɏ������݂���
				c = x;
				fc = fx;
			}

			// ��Ԃ��͂����ނ��Ƃɐ��������̂ŏI������
			break;
		}
		else if( ( b + limit * cb ) * ( x - c ) > 0 )
		{
			// ��ԓ_ x ����� b < c < x �𖞂����Ă��āC���e�͈͓��ɕ�ԓ_�����݂���
			double fx = f( x );

			if( fx < fc )
			{
				// ���݂̒[�ł��� fc �����֐��l���������ꍇ�́C��Ԃ��X�V����
				// a <- b, b <- x �Ƃ��Ă���
				a = b;
				b = x;
				fa = fb;
				fb = fx;
			}
			else
			{
				// �����łȂ���΁C�s�K�؂ȕ�������O���s�����̂ŉ����������p���čX�V����
				// a <- b, b <- c �Ƃ���
				a = b;
				b = c;
				fa = fb;
				fb = fc;
			}

			// �V������ԓ_ c �������������p���ċ��߂�
			c = a + _1_gold * ( b - a );
			fc = f( c );
		}
		else
		{
			// ��ԓ_ x ����� b < c < x �𖞂����Ă��邪�C�]��ɕ�ԓ_����������̂ŉ�����������
			// a <- b, b <- c �Ƃ��āC�V�����_ c ���Čv�Z����
			a = b;
			b = c;
			c = a + _1_gold * ( b - a );
			fa = fb;
			fb = fc;
			fc = f( c );
		}
	}
}


/// @brief ����������p�����ɏ��l��1�����T���i1�������j
namespace gold
{
	/// @brief ���������𗘗p���āCf(x)�̋ɏ��l�̈ꎟ���T�����s���i1�������j
	//!
	//! �����̋ɏ����܂ނƎv������ [a, b] ��K�؂ɐݒ肷�邱�ƂŁC�����𑁂߂邱�Ƃ��ł���
	//!
	//! @note �w�肳�ꂽ��� [a, b] ���ł̋ɏ��l��T������
	//! @note ��ԓ��ɋɏ������݂��Ȃ��ꍇ�́C��ԓ��ōŏ��ƂȂ�ʒu���o�͂���
	//! @note use_enclose ��^�Ɏw�肵���ꍇ�́C�ŏ��ɋ�Ԃ��g�����ċɏ���T������i�{�w�b�_���� enclose �֐��𗘗p����j
	//! 
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[out] iterations     �c ���ۂ̔�����
	//! @param[in]  max_iterations �c �ő唽����
	//! @param[in]  use_enclose    �c �ɏ����͂��ދ�Ԃ̍X�V���s�����ǂ���
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t &iterations, size_t max_iterations, bool use_enclose = true )
	{
		double p, q, fp, fq;
		const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;

		if( use_enclose )
		{
			p = b;
			double fa, fb;
			enclose( a, p, b, fa, fp, fb, f );

			// a <= b <= c �ƂȂ�悤�ɋ�Ԃ�ύX����
			if( a > b )
			{
				double tmp = a;
				a = b;
				b = tmp;
				tmp = fa;
				fa = fb;
				fb = tmp;
			}

			// ��Ԃ̒����ق������߁C���������ɂ�������̓_�����肷��
			if( std::abs( p - a ) > std::abs( p - b ) )
			{
				q = ( p + a ) * 0.5;
				fq = f( p );
			}
			else
			{
				q = ( p + b ) * 0.5;
				fq = f( p );
			}
		}
		else
		{
			if( a > b )
			{
				double tmp = a;
				a = b;
				b = tmp;
			}
			p = a + gold * ( b - a );
			q = b - gold * ( b - a );

			fp = f( p );
			fq = f( q );
		}

		size_t ite = 0;
		for( ; std::abs( a - b ) > tolerance * ( std::abs( p ) + std::abs( q ) ) && ite < max_iterations ; ite++ )
		{
			if( fp > fq )
			{
				// ��� p < f( x ) < c �̊Ԃɍŏ��l�����݂���
				a = p;
				p = q;
				q = p + gold * ( b - p );
				fp = fq;
				fq = f( q );
			}
			else
			{
				// ��� a < f( x ) < q �̊Ԃɍŏ��l�����݂���
				b = q;
				q = p;
				p = q - gold * ( q - a );
				fq = fp;
				fp = f( p );
			}
		}

		iterations = ite;

		if( fp < fq )
		{
			x = p;
			return( fp );
		}
		else
		{
			x = q;
			return( fq );
		}
	}


	/// @brief ���������𗘗p���āCf(x)�̋ɏ��l�̈ꎟ���T�����s���i1�������j
	//! 
	//! �����̋ɏ����܂ނƎv������ [a, b] ��K�؂ɐݒ肷�邱�ƂŁC�����𑁂߂邱�Ƃ��ł���
	//! 
	//! @note �w�肳�ꂽ��� [a, b] ���ł̋ɏ��l��T������
	//! @note ��ԓ��ɋɏ������݂��Ȃ��ꍇ�́C��ԓ��ōŏ��ƂȂ�ʒu���o�͂���
	//! @note use_enclose ��^�Ɏw�肵���ꍇ�́C�ŏ��ɋ�Ԃ��g�����ċɏ���T������i�{�w�b�_���� enclose �֐��𗘗p����j
	//! 
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[in]  max_iterations �c �ő唽���񐔁C���ۂ̔�����
	//! @param[in]  use_enclose    �c �ɏ����͂��ދ�Ԃ̍X�V���s�����ǂ���
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 200, bool use_enclose = true )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( a, b, x, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations, use_enclose ) );
	}
}



/// @brief Brent �̕�������Ԃ�p�����ɏ��l��1�����T���i2�������j
namespace brent
{
	/// @brief Brent �̕�������Ԃ�p���āC�w�肳�ꂽ��ԓ��� f(x) �̋ɏ��l�̈ꎟ���T�����s���i2�������j
	//! 
	//! - �Q�l����
	//!   - Richard P. Brent, "Algorithms for Minimization Without Derivatives", DOVER PUBLICATIONS, Mineola, New York
	//! 
	//! @note �w�肳�ꂽ��� [a, b] ���ł̋ɏ��l��T������
	//! @note ��ԓ��ɋɏ������݂��Ȃ��ꍇ�́C��ԓ��ōŏ��ƂȂ�ʒu���o�͂���
	//! @note use_enclose ��^�Ɏw�肵���ꍇ�́C�ŏ��ɋ�Ԃ��g�����ċɏ���T������i�{�w�b�_���� enclose �֐��𗘗p����j
	//! 
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[out] iterations     �c ���ۂ̔�����
	//! @param[in]  max_iterations �c �ő唽����
	//! @param[in]  use_enclose    �c �ɏ����͂��ދ�Ԃ̍X�V���s�����ǂ���
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t &iterations, size_t max_iterations, bool use_enclose = true )
	{
		double u, v, w, fu, fv, fw, fx, e = 0.0, d = 0.0;
		const double c = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;

		if( use_enclose )
		{
			double fa, fb;
			x = b;
			enclose( a, x, b, fa, fx, fb, f );

			// a <= x <= b �ŋɏ����͂���ԂɕύX����
			if( a > b )
			{
				double tmp = a;
				a = b;
				b = tmp;
				tmp = fa;
				fa = fb;
				fb = tmp;
			}

			v = a;
			w = b;
			u = x;
			fv = fa;
			fw = fb;
			fu = fx;
		}
		else
		{
			// a <= x <= b �ŋɏ����͂���ԂɕύX����
			if( a > b )
			{
				double tmp = a;
				a = b;
				b = tmp;
			}

			v = w = x = a + c * ( b - a );
			fv = fw = fx = f( x );
		}

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			double m = ( a + b ) * 0.5;
			double tol = 1.0e-12 * std::abs( x ) + tolerance;
			double t2 = 2.0 * tol;

			// �Ō�ɔ��肵���ŏ��l���_�ƁC��� [a, b] �̒��ԂƂ̍������e�덷���ɂȂ�����I������
			if( std::abs( x - m ) <= t2 - 0.5 * ( b - a ) )
			{
				break;
			}

			double p = 0.0, q = 0.0, r = 0.0;

			if( std::abs( e ) > tol )
			{
				// ��������Ԃ��s��
				r = ( x - w ) * ( fx - fv );
				q = ( x - v ) * ( fx - fw );
				p = ( x - v ) * q - ( x - w ) * r;
				q = 2.0 * ( q - r );

				if( q > 0 )
				{
					p = -p;
				}
				else
				{
					q = -q;
				}

				r = e;
				e = d;
			}

			if( std::abs( p ) < std::abs( 0.5 * q * r ) && p < q * ( a - x ) && p < q * ( b - x ) )
			{
				// ��������Ԃ��K�؂ɍs��ꂽ�̂ŋ�Ԃ��X�V����
				d = p / q;
				u = x + d;

				if( u - a < t2 || b - u < t2 )
				{
					d = x < m ? tol : -tol;
				}
			}
			else
			{
				// ��������Ԃ͕s�K�؂Ȃ̂ŉ�����������
				// ��Ԃ̑傫���ق���������������
				e = ( x < m ? b : a ) - x;
				d = c * e;
			}

			u = x + ( std::abs( d ) >= tol ? d : ( d > 0 ? tol : -tol ) );
			fu = f( u );

			if( fu <= fx )
			{
				// ��菬�����l�����������̂� a, b, v, w ���X�V����
				if( u < x )
				{
					// ��� a < u < x �ɋɏ��l������
					b = x;
				}
				else
				{
					// ��� x < u < b �ɋɏ��l������
					a = x;
				}
				v  = w;
				w  = x;
				fv = fw;
				fw = fx;
				x  = u;
				fx = fu;
			}
			else
			{
				// ���傫���l�����������̂� a, b ���X�V����
				if( u < x )
				{
					// ��� u < x < b �ɋɏ��l������
					a = u;
				}
				else
				{
					// ��� a < x < u �ɋɏ��l������
					b = u;
				}

				// �V�����]�������_�Ə]���]�������_�̑召�֌W�𒲂ׂ�
				if( fu <= fw || w == x )
				{
					v = w;
					w = u;
					fv = fw;
					fw = fu;
				}
				else if( fu <= fv || v == x || v == w )
				{
					v = u;
					fv = fu;
				}
			}
		}

		iterations = ite;

		return( fx );
	}


	/// @brief Brent �̕�������Ԃ�p���āCf(x)�̋ɏ��l�̈ꎟ���T�����s���i2�������j
	//! 
	//! �����̋ɏ����܂ނƎv������ [a, b] ��K�؂ɐݒ肷�邱�ƂŁC�����𑁂߂邱�Ƃ��ł���
	//! 
	//! @note �w�肳�ꂽ��� [a, b] ���ł̋ɏ��l��T������
	//! @note ��ԓ��ɋɏ������݂��Ȃ��ꍇ�́C��ԓ��ōŏ��ƂȂ�ʒu���o�͂���
	//! @note use_enclose ��^�Ɏw�肵���ꍇ�́C�ŏ��ɋ�Ԃ��g�����ċɏ���T������i�{�w�b�_���� enclose �֐��𗘗p����j
	//! 
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[in]  max_iterations �c �ő唽����
	//! @param[in]  use_enclose    �c �ɏ����͂��ދ�Ԃ̍X�V���s�����ǂ���
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 200, bool use_enclose = true )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( a, b, x, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations, use_enclose ) );
	}
}


/// @brief ���z�֐������[�U�[����`����ŋ}�~���@�i���z��p�������ϐ��֐��̋ɏ��l�̒T���j
namespace gradient_with_vector
{
	/// @brief �T���̊J�n�_���w�肵�C���[�U�[���w�肵�����z�v�Z�֐���p���čŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     g              �c ���z�֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[out]    iterations     �c ���ۂ̔�����
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, Functor1 f, Functor2 g, double tolerance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x, err, old_err = f( p );

		// ���ϐ��֐����P�ϐ��֐��ɕϊ�����
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor1 > functor( p, dir, tmp, f );

		size_t ite, i;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// ���z�������v�Z����
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				// ���z�������v�Z����
				dir[ i ] = g( p[ i ], i );
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// ���z�����x�N�g���̐��K��
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// ���z�̌v�Z���ł��Ȃ��Ȃ����̂ŏI������
				break;
			}

			// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, 200, true );

			//std::cout << p.t( ) << ", " << dir.t( ) << std::endl;

			if( old_err - err < tolerance )
			{
				// �O��̍ŏ����̌��ʂ���̕ω��ʂ��A���e�덷���ł������̂ŏI������
				if( err < old_err )
				{
					p += dir * x;
				}
				break;
			}
			else
			{
				old_err = err;
				p += dir * x;
			}
		}

		iterations = ite;

		//std::cout << ite << std::endl;

		return( err );
	}


	/// @brief �T���̊J�n�_���w�肵�C���[�U�[���w�肵�����z�v�Z�֐���p���čŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     bound          �c �T���ɗp����e�v�f�̒T���͈�
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     g              �c ���z�֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[out]    iterations     �c ���ۂ̔�����
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor1 f, Functor2 g, double tolerance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x, err, old_err = f( p );

		// ���ϐ��֐����P�ϐ��֐��ɕϊ�����
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor1 > functor( p, dir, tmp, f );

		size_t ite, i;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// ���z�������v�Z����
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				// ���z�������v�Z����
				dir[ i ] = g( p[ i ], i );
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// ���z�����x�N�g���̐��K��
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// ���z�̌v�Z���ł��Ȃ��Ȃ����̂ŏI������
				break;
			}

			double l1, l2;
			if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
			{
				// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
				err = brent::minimization( l1, l2, x, functor, tolerance, 200, false );
			}

			//std::cout << p.t( ) << ", " << dir.t( ) << std::endl;

			if( old_err - err < tolerance )
			{
				// �O��̍ŏ����̌��ʂ���̕ω��ʂ��A���e�덷���ł������̂ŏI������
				if( err < old_err )
				{
					p += dir * x;
				}
				break;
			}
			else
			{
				old_err = err;
				p += dir * x;
			}
		}

		iterations = ite;

		//std::cout << ite << std::endl;

		return( err );
	}


	/// @brief �T���̊J�n�_���w�肵�C���[�U�[���w�肵�����z�v�Z�֐���p���čŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     bound          �c �T���ɗp����e�v�f�̒T���͈�
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     g              �c ���z�֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor1 f, Functor2 g, double tolerance, size_t max_iterations = 200 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor1 > __no_copy_constructor_functor1__;
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor2 > __no_copy_constructor_functor2__;
		size_t itenum = 0;
		return( minimization( p, bound, __no_copy_constructor_functor1__( f ), __no_copy_constructor_functor2__( g ), tolerance, itenum, max_iterations ) );
	}


	/// @brief �T���̊J�n�_���w�肵�C���[�U�[���w�肵�����z�v�Z�֐���p���čŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     g              �c ���z�֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, Functor1 f, Functor2 g, double tolerance, size_t max_iterations = 200 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor1 > __no_copy_constructor_functor1__;
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor2 > __no_copy_constructor_functor2__;
		size_t itenum = 0;
		return( minimization( p, __no_copy_constructor_functor1__( f ), __no_copy_constructor_functor2__( g ), tolerance, itenum, max_iterations ) );
	}
}


/// @brief �ŋ}�~���@�i���z��p�������ϐ��֐��̋ɏ��l�̒T���j
namespace gradient
{

	/// @brief �T���̊J�n�_���w�肵�C���z���v�Z���Ȃ���ŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     distance       �c ���z���v�Z����ۂ̕�
	//! @param[out]    iterations     �c ���ۂ̔�����
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x, v1, v2, err = 1.0e100, old_err = f( p );
		size_type i;

		// ���ϐ��֐����P�ϐ��֐��ɕϊ�����
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ;  )
		{
			// ���z�������v�Z����
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				tmp[ i ] = p[ i ] + distance;
				v1 = f( tmp );

				tmp[ i ] = p[ i ] - distance;
				v2 = f( tmp );

				// ���ɖ߂�
				tmp[ i ] = p[ i ];

				dir[ i ] = v2 - v1;
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// ���z�����x�N�g���̐��K��
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// ���z�̌v�Z���ł��Ȃ��Ȃ����̂ŏI������
				break;
			}

			// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, 200, true );

			//std::cout << err << ", " << p.t( ) << ", " << dir.t( ) << std::endl;

			ite++;

			if( ite > max_iterations || 2.0 * std::abs( old_err - err ) < tolerance * ( std::abs( old_err ) + std::abs( err ) ) )
			{
				// �O��̍ŏ����̌��ʂ���̕ω��ʂ��A���e�덷���ł������̂ŏI������
				if( err < old_err )
				{
					p += dir * x;
				}
				break;
			}
			else
			{
				old_err = err;
				p += dir * x;
			}
		}

		iterations = ite;
		//std::cout << ite << std::endl;

		return( err );
	}


	/// @brief �T���̊J�n�_���w�肵�C���z���v�Z���Ȃ���ŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     bound          �c �T���ɗp����e�v�f�̒T���͈�
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     distance       �c ���z���v�Z����ۂ̕�
	//! @param[out]    iterations     �c ���ۂ̔�����
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x = 0.0, v1, v2, err = 1.0e100, old_err = f( p );
		size_type i;

		// ���ϐ��֐����P�ϐ��֐��ɕϊ�����
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; )
		{
			// ���z�������v�Z����
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				tmp[ i ] = p[ i ] + distance;
				v1 = f( tmp );

				tmp[ i ] = p[ i ] - distance;
				v2 = f( tmp );

				// ���ɖ߂�
				tmp[ i ] = p[ i ];

				dir[ i ] = v2 - v1;
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// ���z�����x�N�g���̐��K��
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// ���z�̌v�Z���ł��Ȃ��Ȃ����̂ŏI������
				break;
			}

			double l1, l2;
			if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
			{
				// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
				err = brent::minimization( l1, l2, x, functor, tolerance, 200, false );
			}

			//std::cout << err << ", " << p.t( ) << ", " << dir.t( ) << std::endl;

			ite++;

			if( ite > max_iterations || 2.0 * std::abs( old_err - err ) < tolerance * ( std::abs( old_err ) + std::abs( err ) ) )
			{
				// �O��̍ŏ����̌��ʂ���̕ω��ʂ��A���e�덷���ł������̂ŏI������
				if( err < old_err )
				{
					p += dir * x;
				}
				break;
			}
			else
			{
				old_err = err;
				p += dir * x;
			}
		}

		iterations = ite;
		//std::cout << ite << std::endl;

		return( err );
	}



	/// @brief �T���̊J�n�_���w�肵�C���z���v�Z���Ȃ���ŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     distance       �c ���z���v�Z����ۂ̕�
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance = 1.0, size_t max_iterations = 200 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, __no_copy_constructor_functor__( f ), tolerance, distance, itenum, max_iterations ) );
	}


	/// @brief �T���̊J�n�_���w�肵�C���z���v�Z���Ȃ���ŏ��l��T������
	//! 
	//! �T���̊J�n�_���w�肵���̈ʒu�ł̌��z�����Ɍ��������ŏ������J��Ԃ��C�ŏ��l��T������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in]     bound          �c �T���ɗp����e�v�f�̒T���͈�
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     distance       �c ���z���v�Z����ۂ̕�
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor f, double tolerance, double distance = 1.0, size_t max_iterations = 200 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, bound, __no_copy_constructor_functor__( f ), tolerance, distance, itenum, max_iterations ) );
	}
}


/// @brief Powell�@�i�����W����p�������ϐ��֐��̋ɏ��l�̒T���j
namespace powell
{
	/// @brief Powell �@�ɂ�鑽�����ϐ��ɂ��ɏ��l�̒T�����s��
	//! 
	//! ��@�ɂ��ĉ�������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in,out] dirs           �c �T���ɗp��������W��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[out]    iterations     �c ���ۂ̔�����
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 ), p0( p ), pn( p );
		double x, fp0 = 1.0e100, fp = f( p ), delta;

		// ���ϐ��֐����P�ϐ��֐��ɕϊ�����
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		size_type r, c;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// �T���J�n�O�̕]���l���o���Ă���
			fp0 = fp;
			delta = 0.0;
			size_type index = 0;

			for( c = 0 ; c < dirs.cols( ) ; c++ )
			{
				// �T���ɗp��������W�����R�s�[����
				for( r = 0 ; r < dirs.rows( ) ; r++ )
				{
					dir[ r ] = dirs( r, c );
				}

				double old_fp = fp;

				// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
				fp = brent::minimization( -0.5, 0.5, x, functor, tolerance, 200, true );

				for( r = 0 ; r < p.size( ) ; r++ )
				{
					p[ r ] += dir[ r ] * x;
				}

				double d = std::abs( fp - old_fp );
				if( d > delta )
				{
					index = c;
					delta = d;
				}

				//std::cout << fp << p.t( ) << std::endl;
			}

			// ���Ό덷��p������������
			if( 2.0 * std::abs( fp - fp0 ) <= tolerance * ( std::abs( fp ) + std::abs( fp0 ) ) )
			{
				break;
			}

			// Acton �̕��@��p���āC�V���������W�������߂�
			if( ite <= max_iterations )
			{
				// �V�������������߂�
				double len = 0.0;
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					double l = p[ r ] - p0[ r ];
					len += l;
					dir[ r ] = l;

					pn[ r ]  = 2.0 * p[ r ] - p0[ r ];
					p0[ r ]  = p[ r ];
				}

				// ���̒T�������𐳋K������
				if( len > 0.0 )
				{
					len = std::sqrt( len );
					for( r = 0 ; r < p.size( ) ; r++ )
					{
						dir[ r ] /= len;
					}
				}

				double fe = f( pn );

				if( fe < fp )
				{
					// ���݂̕����W�����X�V����
					double tmp = fp0 - fp - delta;
					double ttt = 2.0 * ( fp0 - 2.0 * fp + fe ) * tmp * tmp - delta * ( fp0 - fe ) * ( fp0 - fe );
					if( ttt >= 0 )
					{
						// Brent ��2�������A���S���Y����p���āC�V���� dir �����ւ̍ŏ������s��
						fp = brent::minimization( -0.5, 0.5, x, functor, tolerance, 200, true );
						p += dir * x;

						// �����W���̈�ԍŌ�ɁC�V����������ǉ�����
						if( index < dirs.rows( ) - 1 )
						{
							for( r = 0 ; r < dirs.rows( ) ; r++ )
							{
								dirs( r, index ) = dirs( r, dirs.rows( ) - 1 );
								dirs( r, dirs.rows( ) - 1 ) = dir[ r ];
							}
						}
						else
						{
							for( r = 0 ; r < dirs.rows( ) ; r++ )
							{
								dirs( r, index ) = dir[ r ];
							}
						}
					}
				}
			}
		}

		iterations = ite;

		return( fp );
	}


	/// @brief Powell �@�ɂ�鑽�����ϐ��ɂ��ɏ��l�̒T�����s��
	//! 
	//! ��@�ɂ��ĉ�������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in,out] dirs           �c �T���ɗp��������W��
	//! @param[in,out] bound          �c �T���ɗp����e�v�f�̒T���͈�
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[out]    iterations     �c ���ۂ̔�����
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, matrix< T, Allocator > &bound, Functor f, double tolerance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 ), p0( p ), pn( p );
		double x = 0.0, fp0 = 1.0e100, fp = f( p ), delta;
		double l1, l2;

		// ���ϐ��֐����P�ϐ��֐��ɕϊ�����
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		size_type r, c;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// �T���J�n�O�̕]���l���o���Ă���
			fp0 = fp;
			delta = 0.0;
			size_type index = 0;

			for( c = 0 ; c < dirs.cols( ) ; c++ )
			{
				// �T���ɗp��������W�����R�s�[����
				for( r = 0 ; r < dirs.rows( ) ; r++ )
				{
					dir[ r ] = dirs( r, c );
				}

				double old_fp = fp;


				if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
				{
					// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
					fp = brent::minimization( l1, l2, x, functor, tolerance, 200, false );

					for( r = 0 ; r < p.size( ) ; r++ )
					{
						p[ r ] += dir[ r ] * x;
					}

					double d = std::abs( fp - old_fp );
					if( d > delta )
					{
						index = c;
						delta = d;
					}
				}

				//std::cout << fp << p.t( ) << std::endl;
			}

			// ���Ό덷��p������������
			if( 2.0 * std::abs( fp - fp0 ) <= tolerance * ( std::abs( fp ) + std::abs( fp0 ) ) )
			{
				break;
			}

			// Acton �̕��@��p���āC�V���������W�������߂�
			if( ite <= max_iterations )
			{
				// �V�������������߂�
				double len = 0.0;
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					double l = p[ r ] - p0[ r ];
					len += l;
					dir[ r ] = l;

					pn[ r ]  = 2.0 * p[ r ] - p0[ r ];
					p0[ r ]  = p[ r ];
				}

				// ���̒T�������𐳋K������
				if( len > 0.0 )
				{
					len = std::sqrt( len );
					for( r = 0 ; r < p.size( ) ; r++ )
					{
						dir[ r ] /= len;
					}
				}

				double fe = f( pn );

				if( fe < fp )
				{
					// ���݂̕����W�����X�V����
					double tmp = fp0 - fp - delta;
					double ttt = 2.0 * ( fp0 - 2.0 * fp + fe ) * tmp * tmp - delta * ( fp0 - fe ) * ( fp0 - fe );
					if( ttt >= 0 )
					{
						if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
						{
							// Brent ��2�������A���S���Y����p���āC�V���� dir �����ւ̍ŏ������s��
							fp = brent::minimization( l1, l2, x, functor, tolerance, 200, false );
							p += dir * x;
						}

						// �����W���̈�ԍŌ�ɁC�V����������ǉ�����
						if( index < dirs.rows( ) - 1 )
						{
							for( r = 0 ; r < dirs.rows( ) ; r++ )
							{
								dirs( r, index ) = dirs( r, dirs.rows( ) - 1 );
								dirs( r, dirs.rows( ) - 1 ) = dir[ r ];
							}
						}
						else
						{
							for( r = 0 ; r < dirs.rows( ) ; r++ )
							{
								dirs( r, index ) = dir[ r ];
							}
						}
					}
				}
			}
		}

		iterations = ite;

		return( fp );
	}

	/// @brief Powell �@�ɂ�鑽�����ϐ��ɂ��ɏ��l�̒T�����s��
	//! 
	//! ��@�ɂ��ĉ�������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in,out] dirs           �c �T���ɗp��������W��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t max_iterations = 200 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, dirs, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations ) );
	}

	/// @brief Powell �@�ɂ�鑽�����ϐ��ɂ��ɏ��l�̒T�����s��
	//! 
	//! ��@�ɂ��ĉ�������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in,out] dirs           �c �T���ɗp��������W��
	//! @param[in,out] bound          �c �T���ɗp����e�v�f�̒T���͈�
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, matrix< T, Allocator > &bound, Functor f, double tolerance, size_t max_iterations = 200 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, dirs, bound, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations ) );
	}
}



/// @}
//  �֐��̍ŏ����O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_MINIMIZATION__
