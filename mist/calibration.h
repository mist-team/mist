/// @file mist/calibration.h
//!
//! @brief �J�����L�����u���[�V�������s���֐��Q
//!
//! @section �J�����L�����u���[�V����
//! -# R. Y. Tsai, ``A Versatile Camera Calibration Technique for High-Accuracy 3D Machine Vision Metrology Using Off-the-Shelf TV Cameras and Lenses,'' IEEE Journal of Robotics and Automation, Vol. RA-3, No. 4, pp. 323--344, 1987.
//!

#ifndef __INCLUDE_MIST_CALIBRATION__
#define __INCLUDE_MIST_CALIBRATION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_NUMERIC__
#include "numeric.h"
#endif

#ifndef __INCLUDE_MIST_MINIMIZATION__
#include "minimization.h"
#endif

#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// @brief Tsai �̃J�����L�����u���[�V������@
namespace Tsai
{
	/// @brief �L�����u���[�V�����ɗp����p�����[�^�y�уL�����u���[�V�������ʂ�������\����
	struct parameter
	{
		// �L�����u���[�V�����O�ɓ��͂���K�v������ϐ�
		double Ncx;		///< @brief �J������X�������̃Z���T�[�f�q�� [sel]
		double Ncy;		///< @brief �J������Y�������̃Z���T�[�f�q�� [sel]
		double Nfx;		///< @brief �B�e�����摜��X�������̉�f�� [pixel]
		double Nfy;		///< @brief �B�e�����摜��Y�������̉�f�� [pixel]
		double dx;		///< @brief �J������X�������̃Z���T�[�f�q�̑傫�� [mm/sel]
		double dy;		///< @brief �J������Y�������̃Z���T�[�f�q�̑傫�� [mm/sel]
		double Cx;		///< @brief �J�������W�n�ɂ�����Z���Ɖ摜���ʂ̌�_��X���W�i�摜���S��^����j[pixel]
		double Cy;		///< @brief �J�������W�n�ɂ�����Z���Ɖ摜���ʂ̌�_��Y���W�i�摜���S��^����j[pixel]
		double sx;		///< @brief �������ʂ�p�����L�����u���[�V�����̍ۂɗp����CX�������̉𑜓x�𒲐�����W��

		// �L�����u���[�V������ɓ�����p�����[�^
		double dpx;		///< @brief �J������X��������1��f������̑傫�� [mm/pixel]
		double dpy;		///< @brief �J������Y��������1��f������̑傫�� [mm/pixel]
		double r1;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 0, 0 ) ����
		double r2;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 0, 1 ) ����
		double r3;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 0, 2 ) ����
		double r4;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 1, 0 ) ����
		double r5;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 1, 1 ) ����
		double r6;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 1, 2 ) ����
		double r7;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 2, 0 ) ����
		double r8;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 2, 1 ) ����
		double r9;		///< @brief �L�����u���[�V�����̌��ʓ������]�s��� ( 2, 2 ) ����
		double Tx;		///< @brief �L�����u���[�V�����̌��ʓ�����J�����̕��s�ړ���X����
		double Ty;		///< @brief �L�����u���[�V�����̌��ʓ�����J�����̕��s�ړ���Y����
		double Tz;		///< @brief �L�����u���[�V�����̌��ʓ�����J�����̕��s�ړ���Z����
		double f;		///< @brief �L�����u���[�V�����̌��ʓ�����œ_����
		double ka1;		///< @brief �L�����u���[�V�����̌��ʓ�����~�`�̘c����

		parameter( )
			: Ncx( 640 ), Ncy( 480 ), Nfx( 640 ), Nfy( 640 ), dx( 1.0 ), dy( 1.0 ), Cx( 320 ), Cy( 240 ), sx( 1.0 ),
			  r1( 1.0 ), r2( 0.0 ), r3( 0.0 ), r4( 0.0 ), r5( 1.0 ), r6( 0.0 ), r7( 0.0 ), r8( 0.0 ), r9( 1.0 ),
			  Tx( 0.0 ), Ty( 0.0 ), Tz( 0.0 ), f( 1.0 ), ka1( 0.0 )
		{
		}
	};

	/// @brief �J�����L�����u���[�V�����p�����[�^�𐮌`���ďo�͂���
	//! 
	//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
	//! @param[in]     p   �c �o�͂���L�����u���[�V�����p�����[�^
	//! 
	//! @return ���͂��ꂽ�X�g���[��
	//! 
	inline ::std::ostream &operator <<( ::std::ostream &out, const parameter &p )
	{
		out << "Ncx : " << p.Ncx << ::std::endl;
		out << "Ncy : " << p.Ncy << ::std::endl;
		out << "Nfx : " << p.Nfx << ::std::endl;
		out << "Nfy : " << p.Nfy << ::std::endl;
		out << "dx  : " << p.dx << ::std::endl;
		out << "dy  : " << p.dy << ::std::endl;
		out << "dpx : " << p.dpx << ::std::endl;
		out << "dpy : " << p.dpy << ::std::endl;
		out << "Cx  : " << p.Cx << ::std::endl;
		out << "Cy  : " << p.Cy << ::std::endl;
		out << "sx  : " << p.sx << ::std::endl;

		out << ::std::endl;
		out << "R =" << ::std::endl;
		out << p.r1 << ", " << p.r2 << ", " << p.r3 << ::std::endl;
		out << p.r4 << ", " << p.r5 << ", " << p.r6 << ::std::endl;
		out << p.r7 << ", " << p.r8 << ", " << p.r9 << ::std::endl;

		out << ::std::endl;
		out << "T = ( " << p.Tx << ", " << p.Ty << ", " << p.Tz << " )" << ::std::endl;
		out << "focal length = " << p.f << ::std::endl;
		out << "kappa1 = " << p.ka1 << ::std::endl;

		return( out );
	}


	template < class T1, class T2 >
	struct __parameter__ : public parameter
	{
		typedef parameter base;
		typedef vector3< T1 > point3;
		typedef vector2< T2 > point2;

		const std::vector< point3 > &world;
		const std::vector< point2 > &image;

		__parameter__( const parameter &p, const std::vector< point3 > &w, const std::vector< point2 > &i )
			: base( p ), world( w ), image( i )
		{
		}


		template < class TT >
		double operator( )( const matrix< TT > &v ) const
		{
			typedef matrix< TT >::size_type size_type;

			double err = 0.0;
			double f = v[ 0 ];
			double Tz = v[ 1 ];
			double ka1 = v[ 2 ];

			for( size_type i = 0 ; i < world.size( ) ; i++ )
			{
				const point3 &w = world[ i ];
				const point2 &p = image[ i ];
				double Xd = ( p.x - Cx ) * dpx / sx;
				double Yd = ( p.y - Cy ) * dpy;
				double x = r1 * w.x + r2 * w.y + r3 * w.z + Tx;
				double y = r4 * w.x + r5 * w.y + r6 * w.z + Ty;
				double z = r7 * w.x + r8 * w.y + r9 * w.z + Tz;
				double rr = Xd * Xd + Yd * Yd;

				double e1 = f * x - Xd * ( 1 + ka1 * rr ) * z;
				double e2 = f * y - Yd * ( 1 + ka1 * rr ) * z;

				//std::cout << x << ", " << y << ", " << z << std::endl;

				//			err += std::sqrt( e1 * e1 );
				err += std::sqrt( e2 * e2 );
				//			err += std::sqrt( e1 * e1 + e2 * e2 );
			}
			return( err );
		}
	};


	/// @brief Tsai �̃J�����L�����u���[�V����
	//! 
	//! @param[in,out] p       �c �J�����L�����u���[�V�����ɗp����p�����[�^�C�y�ь��ʂ̏o�͗p
	//! @param[in]     world   �c �L�����u���[�V�����_�̎��ۂ̍��W
	//! @param[in]     image   �c �L�����u���[�V�����_�̉摜��̍��W
	//! 
	//! @attention ���͂̃L�����u���[�V�����_��Z���W�l�̓[���łȂ��Ă͂Ȃ�Ȃ�
	//!
	//! @return �L�����u���[�V�����ɐ����������ǂ���
	//! 
	template < class T1, class T2 >
	bool calibration( parameter &p, const std::vector< vector3< T1 > > &world, const std::vector< vector2< T2 > > &image )
	{
		typedef vector3< T1 > point3;
		typedef vector2< T2 > point2;
		typedef matrix< double > matrix_type;
		typedef matrix< double >::size_type size_type;

		double r1, r2, r3, r4, r5, r6, r7, r8, r9;
		double Tx, Ty, Tz;
		double f;
		double ka1 = 0.0;

		double Ncx = p.Ncx;		// �J������X�������̃Z���T�[�f�q�� [sel]
		double Ncy = p.Ncy;		// �J������Y�������̃Z���T�[�f�q�� [sel]
		double Nfx = p.Nfx;		// �B�e�����摜��X�������̉�f�� [pixel]
		double Nfy = p.Nfy;		// �B�e�����摜��Y�������̉�f�� [pixel]
		double dx  = p.dx;		// �J������X�������̃Z���T�[�f�q�̑傫�� [mm/sel]
		double dy  = p.dy;		// �J������Y�������̃Z���T�[�f�q�̑傫�� [mm/sel]
		double Cx  = p.Cx;		// �J�������W�n�ɂ�����Z���Ɖ摜���ʂ̌�_��X���W�i�摜���S��^����j[pixel]
		double Cy  = p.Cy;		// �J�������W�n�ɂ�����Z���Ɖ摜���ʂ̌�_��Y���W�i�摜���S��^����j[pixel]
		double sx  = p.sx;		// �������ʂ�p�����L�����u���[�V�����̍ۂɗp����CX�������̉𑜓x�𒲐�����W��

		double dpx = dx * Ncx / Nfx;	// �J������X��������1��f������̑傫�� [mm/pixel]
		double dpy = dy * Ncy / Nfy;	// �J������Y��������1��f������̑傫�� [mm/pixel]

		size_type i, num = image.size( );

		matrix_type A( num, 5 ), B( num, 1 );

		// (i) �摜���W�n�ɂ�����_���v�Z����
		for( i = 0 ; i < num ; i++ )
		{
			const point3 &w = world[ i ];
			const point2 &p = image[ i ];
			double Xd = ( p.x - Cx ) * dpx / sx;
			double Yd = ( p.y - Cy ) * dpy;

			A( i, 0 ) = Yd * w.x;
			A( i, 1 ) = Yd * w.y;
			A( i, 2 ) = Yd;
			A( i, 3 ) = - Xd * w.x;
			A( i, 4 ) = - Xd * w.y;
			B( i, 0 ) = Xd;
		}

		// (ii) 5�̖��m��������
		matrix_type L = mist::inverse( A.t( ) * A ) * A.t( ) * B;

		// (iii.1) Ty�����߂�
		double r1_ = L[ 0 ];
		double r2_ = L[ 1 ];
		double Ty_Tx = L[ 2 ];
		double r4_ = L[ 3 ];
		double r5_ = L[ 4 ];

		bool b1 = std::abs( r1_ ) > 1.0e-12;
		bool b2 = std::abs( r2_ ) > 1.0e-12;
		bool b4 = std::abs( r4_ ) > 1.0e-12;
		bool b5 = std::abs( r5_ ) > 1.0e-12;

		if( b1 && b2 && b4 && b5 )
		{
			double Sr = r1_ * r1_ + r2_ * r2_ + r4_ * r4_ + r5_ * r5_;
			double Br = r1_ * r5_ - r4_ * r2_;
			Ty = ( Sr - std::sqrt( Sr * Sr - 4.0 * Br * Br ) ) / ( 2.0 * Br * Br );
		}
		else
		{
			double rr[ 4 ];
			size_type count = 0;
			if( b1 )
			{
				rr[ count++ ] = r1_;
			}
			if( b2 )
			{
				rr[ count++ ] = r2_;
			}
			if( b4 )
			{
				rr[ count++ ] = r4_;
			}
			if( b5 )
			{
				rr[ count++ ] = r5_;
			}
			Ty = 1.0 / ( rr[ 0 ] *  rr[ 0 ] +  rr[ 1 ] *  rr[ 1 ] );
		}


		// (iii.2) Ty�̕��������肷��
		{
			// �摜���S����ł�����Ă�����̂�I������
			point3 w = world[ 0 ];
			point2 p = image[ 0 ];
			double len = ( p.x - Cx ) * ( p.x - Cx ) + ( p.y - Cy ) * ( p.y - Cy );
			for( i = 1 ; i < num ; i++ )
			{
				point3 ww = world[ i ];
				point2 pp = image[ i ];
				double l = ( pp.x - Cx ) * ( pp.x - Cx ) + ( pp.y - Cy ) * ( pp.y - Cy );
				if( len < l )
				{
					w = ww;
					p = pp;
				}
			}

			// Ty�̕����𐳂ɂ���
			Ty = std::sqrt( Ty );
			r1 = r1_ * Ty;
			r2 = r2_ * Ty;
			r4 = r4_ * Ty;
			r5 = r5_ * Ty;
			Tx = Ty_Tx * Ty;

			double x = r1 * w.x + r2 * w.y + Tx;
			double y =  r4 * w.x + r5 * w.y + Ty;
			if( x * p.x > 0 && y * p.y > 0 )
			{
				// Ty�̕����͐��ɂ���
			}
			else
			{
				// Ty�̕����͕��ɂ���
				Ty = -Ty;
			}
		}

		// Ty�̌v�Z���ʂ�p���Ēl���X�V����
		r1 = r1_ * Ty;
		r2 = r2_ * Ty;
		r4 = r4_ * Ty;
		r5 = r5_ * Ty;
		Tx = Ty_Tx * Ty;

		std::cout << "( Tx, Ty ) = ( " << Tx << ", " << Ty << " )" << std::endl;

		// (iii.3) ��]�s��R�����肷��
		{
			double s = -1.0 * ( r1 * r4 + r2 * r5 < 0.0 ? -1.0 : 1.0 );

			r3 = std::sqrt( 1.0 - r1 * r1 - r2 * r2 );
			r6 = s * std::sqrt( 1.0 - r4 * r4 - r5 * r5 );
			r7 = r2 * r6 - r3 * r5;

			// ���s�s��ɂȂ�悤�ɕό`���s��
			double Rz = std::atan2( r4, r1 );

			double s1, s2, s3, c1, c2, c3;

			s3 = std::sin( Rz );
			c3 = std::cos( Rz );

			double Ry = std::atan2( -r7, r1 * c3 + r4 * s3 );
			double Rx = std::atan2( r3 * s3 - r6 * c3, r5 * c3 - r2 * s3 );

			s1 = std::sin( Rx );
			c1 = std::cos( Rx );
			s2 = std::sin( Ry );
			c2 = std::cos( Ry );

			r1 = c2 * c3;
			r2 = c3 * s1 * s2 - c1 * s3;
			r3 = s1 * s3 + c1 * c3 * s2;
			r4 = c2 * s3;
			r5 = s1 * s2 * s3 + c1 * c3;
			r6 = c1 * s2 * s3 - c3 * s1;
			r7 = -s2;
			r8 = c2 * s1;
			r9 = c1 * c2;
		}


		A.resize( num, 2 );

		// (iv) �œ_������Tz�̋ߎ��l���C�����Y�c�𖳎����ċ��߂�
		for( i = 0 ; i < num ; i++ )
		{
			const point3 &w = world[ i ];
			const point2 &p = image[ i ];
			double Yd = ( p.y - Cy ) * dpy;

			A( i, 0 ) = r4 * w.x + r5 * w.y + r6 * 0 + Ty;
			A( i, 1 ) = -Yd;
			B( i, 0 ) = ( r7 * w.x + r8 * w.y + r9 * 0 ) * Yd;
		}

		L = mist::inverse( A.t( ) * A ) * A.t( ) * B;

		f = L[ 0 ];
		Tz = L[ 1 ];

		// (v) �ŋ}�~���@��p���Đ��m�ȏœ_������Tz�̒l�����߂�
		__parameter__< T1, T2 > param( p, world, image );
		param.dpx = dpx;	// �J������X��������1��f������̑傫�� [mm/pixel]
		param.dpy = dpy;	// �J������Y��������1��f������̑傫�� [mm/pixel]
		param.r1  = r1;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 0, 0 ) ����
		param.r2  = r2;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 0, 1 ) ����
		param.r3  = r3;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 0, 2 ) ����
		param.r4  = r4;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 1, 0 ) ����
		param.r5  = r5;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 1, 1 ) ����
		param.r6  = r6;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 1, 2 ) ����
		param.r7  = r7;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 2, 0 ) ����
		param.r8  = r8;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 2, 1 ) ����
		param.r9  = r9;		// �L�����u���[�V�����̌��ʓ������]�s��� ( 2, 2 ) ����
		param.Tx  = Tx;		// �L�����u���[�V�����̌��ʓ�����J�����̕��s�ړ���X����
		param.Ty  = Ty;		// �L�����u���[�V�����̌��ʓ�����J�����̕��s�ړ���Y����
		param.Tz  = Tz;		// �L�����u���[�V�����̌��ʓ�����J�����̕��s�ړ���Z����
		param.f   = f;		// �L�����u���[�V�����̌��ʓ�����œ_����
		param.ka1 = ka1;	// �L�����u���[�V�����̌��ʓ�����~�`�̘c����

		{
			mist::matrix< double > ppp( 3, 1 );
			ppp[ 0 ] = f;
			ppp[ 1 ] = Tz;
			ppp[ 2 ] = ka1;

			//std::cout << "( f, Tz, ka1 ) = " << ppp.t( ) << std::endl;

			mist::gradient::minimization( ppp, param, 0.001, 0.1 );

			//std::cout << "( f, Tz, ka1 ) = " << ppp.t( ) << std::endl;

			param.f   = ppp[ 0 ];
			param.Tz  = ppp[ 1 ];
			param.ka1 = ppp[ 2 ];
		}

		p = param;

		return( true );
	}
}

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_CALIBRATION__