/// @file mist/minimization.h
//!
//! @brief �֐��̍ŏ������������߂̃��C�u����
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
		const matrix< T, Allocator > &ori_;
		const matrix< T, Allocator > &dir_;
		Functor f_;

		__convert_to_vector_functor__( const matrix< T, Allocator > &ori, const matrix< T, Allocator > &dir, Functor f ) : ori_( ori ), dir_( dir ), f_( f ){ }

		double operator ()( double x ) const
		{
			return( f_( ori_ + dir_ * x ) );
		}
	};

	template < class T, class Allocator, class Functor >
	struct __gradient_vector_functor__
	{
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef matrix< T, Allocator > matrix_type;
		Functor f_;
		double d_;

		__gradient_vector_functor__( Functor f, double d ) : f_( f ), d_( d ){ }

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

}



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
		double x = b + ( ba * ba * fcb + cb * cb * fba ) / ( 2.0 * ( cb * fba - ba * fcb ) + dust );

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


namespace gold
{
	/// @brief ���������𗘗p���āCf(x)�̋ɏ��l�̈ꎟ���T�����s���i1�������j
	//! 
	//! �����̋ɏ����܂ނƎv������ [a, b] ��K�؂ɐݒ肷�邱�ƂŁC�����𑁂߂邱�Ƃ��ł���
	//! 
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[out] iterations     �c ���ۂ̔�����
	//! @param[in]  max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t &iterations, size_t max_iterations )
	{
		double c, p, q, fa, fb, fc, fp, fq;
		const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;

		enclose( a, b, c, fa, fb, fc, f );

		// a <= b <= c �ƂȂ�悤�ɋ�Ԃ�ύX����
		if( a > c )
		{
			double tmp = a;
			a = c;
			c = tmp;
			tmp = fa;
			fa = fc;
			fc = tmp;
		}

		// ��Ԃ̒����ق������߁C���������ɂ�������̓_�����肷��
		if( std::abs( b - a ) > std::abs( c - b ) )
		{
			p = a + gold * ( b - a );
			q = b;
			fp = f( p );
			fq = fb;
		}
		else
		{
			p = b;
			q = c - gold * ( c - b );
			fp = fb;
			fq = f( q );
		}

		size_t ite = 1;
		for( ite = 1 ; std::abs( p - q ) > tolerance && ite <= max_iterations ; ite++ )
		{
			if( fp > fq )
			{
				// ��� p < f( x ) < c �̊Ԃɍŏ��l�����݂���
				a = p;
				p = q;
				q = c - gold * ( c - a );
				fp = fq;
				fq = f( q );
			}
			else
			{
				// ��� a < f( x ) < q �̊Ԃɍŏ��l�����݂���
				c = q;
				q = p;
				p = a + gold * ( c - a );
				fq = fp;
				fp = f( p );
			}
			if( p > q )
			{
				double tmp = p;
				p = q;
				q = tmp;
				tmp = fp;
				fp = fq;
				fq = tmp;
			}
		}

		//std::cout << ite << std::endl;
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
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[in]  max_iterations �c �ő唽���񐔁C���ۂ̔�����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 200 )
	{
		size_t itenum = 0;
		return( minimization( a, b, x, f, tolerance, itenum, max_iterations ) );
	}
}


namespace brent
{
	/// @brief Brent �̕�������Ԃ�p���āCf(x)�̋ɏ��l�̈ꎟ���T�����s���i2�������j
	//! 
	//! �����̋ɏ����܂ނƎv������ [a, b] ��K�؂ɐݒ肷�邱�ƂŁC�����𑁂߂邱�Ƃ��ł���
	//! 
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[out] iterations     �c ���ۂ̔�����
	//! @param[in]  max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t &iterations, size_t max_iterations )
	{
		double u, v, w, xm, fa, fb, fu, fv, fw, fx;
		double len, len1, len2;
		const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;
		const double dust = type_limits< double >::minimum( );		// �[�����Z������邽�߂̃S�~�l

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

		len1 = len2 = type_limits< double >::maximum( );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			xm = ( a + b ) / 2.0;

			// �Ō�ɔ��肵���ŏ��l���_�ƁC��� [a, b] �̒��ԂƂ̍������e�덷���ɂȂ�����I������
			if( std::abs( xm - x ) < tolerance )
			{
				break;
			}

			// ��������Ԃ��s��
			double xv = x - v;
			double wx = w - x;
			double fwx = fw - fx;
			double fxv = fx - fv;
			len = ( xv * xv * fwx + wx * wx * fxv ) / ( 2.0 * ( wx * fxv - xv * fwx ) + dust );
			u = x + len;

			// ���[a, b] ���ɓ���C
			len = std::abs( len );
			if( ( b - u ) * ( u - a ) > 0.0 && len < len2 / 2.0 )
			{
				// ��������Ԃ��K�؂ɍs��ꂽ
			}
			else
			{
				// ��������Ԃ͕s�K�؂Ȃ̂ŉ�����������
				// ��Ԃ̑傫���ق���������������
				if( xm < x )
				{
					// ��� [a, x] �𕪊�����
					u = a + gold * ( x - a );
				}
				else
				{
					// ��� [x, b] �𕪊�����
					u = x + gold * ( b - x );
				}
			}

			fu = f( u );
			if( fu <= fx )
			{
				// ��菬�����l�����������̂� a, b, v, w ���X�V����
				v = w;
				w = x;
				fv = fw;
				fw = fx;
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
				x = u;
				fx = fu;

				len2 = len1;
				len1 = len;
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
				else if( fu <= fv || w == x || v == w )
				{
					v = u;
					fv = fu;
				}
			}
		}

		iterations = ite;

//		std::cout << ite << std::endl;

		return( fx );
	}

	/// @brief Brent �̕�������Ԃ�p���āCf(x)�̋ɏ��l�̈ꎟ���T�����s���i2�������j
	//! 
	//! �����̋ɏ����܂ނƎv������ [a, b] ��K�؂ɐݒ肷�邱�ƂŁC�����𑁂߂邱�Ƃ��ł���
	//! 
	//! @param[in]  a              �c ��Ԃ̍��[
	//! @param[in]  b              �c ��Ԃ̉E�[
	//! @param[out] x              �c �ɏ���^������W�l
	//! @param[in]  f              �c �]���֐�
	//! @param[in]  tolerance      �c ���e�덷
	//! @param[in]  max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 200 )
	{
		size_t itenum = 0;
		return( minimization( a, b, x, f, tolerance, itenum, max_iterations ) );
	}
}


namespace gradient
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
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor1 > functor( p, dir, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// ���z�������v�Z����
			dir = g( p );

			// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, max_iterations );

			std::cout << p.t( ) << ", " << dir.t( ) << std::endl;

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
		size_t itenum = 0;
		return( minimization( p, f, g, tolerance, itenum, max_iterations ) );
	}

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
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x, v1, v2, err = 1.0e100, old_err = f( p );
		size_type i;

		// ���ϐ��֐����P�ϐ��֐��ɕϊ�����
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// ���z�������v�Z����
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				tmp[ i ] = p[ i ] + distance;
				v1 = f( tmp );

				tmp[ i ] = p[ i ] - distance;
				v2 = f( tmp );

				tmp[ i ] = p[ i ];

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
			else
			{
				// ���z�̌v�Z���ł��Ȃ��Ȃ����̂ŏI������
				break;
			}

			// Brent ��2�������A���S���Y����p���� dir �����ւ̍ŏ������s��
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, max_iterations );

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
		size_t itenum = 0;
		return( minimization( p, f, tolerance, distance, itenum, max_iterations ) );
	}
}

namespace powell
{
	/// @brief Powell �@�ɂ�鑽�����ϐ��ɂ��ɏ��l�̒T�����s��
	//! 
	//! ��@�ɂ��ĉ�������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in,out] dir            �c �T���ɗp��������W��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[out]    iterations     �c ���ۂ̔�����
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t &iterations, size_t max_iterations )
	{
		return( 0 );
	}

	/// @brief Powell �@�ɂ�鑽�����ϐ��ɂ��ɏ��l�̒T�����s��
	//! 
	//! ��@�ɂ��ĉ�������
	//! 
	//! @param[in,out] p              �c �T���̊J�n�x�N�g���C�ŏ��l��^����x�N�g��
	//! @param[in,out] dir            �c �T���ɗp��������W��
	//! @param[in]     f              �c �]���֐�
	//! @param[in]     tolerance      �c ���e�덷
	//! @param[in]     max_iterations �c �ő唽����
	//!
	//! @return �ɏ���^������W�l�ɂ�����]���l
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t max_iterations = 200 )
	{
		size_t itenum = 0;
		return( minimization( p, dirs, f, tolerance, itenum, max_iterations ) );
	}
}



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_MINIMIZATION__
