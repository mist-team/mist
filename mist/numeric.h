#ifndef __INCLUDE_NUMERIC__
#define __INCLUDE_NUMERIC__

#include <complex>


#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

// �s��N���X���C���N���[�h����Ă��Ȃ��ꍇ�̓C���|�[�g����D
#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// @enum �s��v�Z������ۂ̓��͂ƂȂ�s��̌`��
//! 
//! @attention ���̌`���ɏ]���āC�����ŌĂяo���֐����ύX�����
//! 
struct matrix_style
{
	enum style
	{
		ge,		///< ��� 
		gb,		///< ��ʑ� 
		gt,		///< ���3�d�Ίp 
		sy,		///< �Ώ� 
		sb,		///< �Ώ̑� 
		st,		///< �Ώ�3�d�Ίp 
		he,		///< �G���~�[�g 
		hb,		///< �G���~�[�g�� 
		ht,		///< �G���~�[�g3�d�Ίp 
	};
};

namespace __numeric__
{
	template < class T >
	struct is_complex
	{
		_MIST_CONST( bool, value, false );
	};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_COMPLEX( type ) \
		template < >\
	struct is_complex< std::complex< type > >\
		{\
		enum{ value = true };\
		};\

		// �e�^�ɑ΂�����ꉻ
		IS_COMPLEX(unsigned char)
		IS_COMPLEX(unsigned short)
		IS_COMPLEX(unsigned int)
		IS_COMPLEX(unsigned long)
		IS_COMPLEX(signed char)
		IS_COMPLEX(signed short)
		IS_COMPLEX(signed int)
		IS_COMPLEX(signed long)
		IS_COMPLEX(bool)
		IS_COMPLEX(char)
		IS_COMPLEX(float)
		IS_COMPLEX(double)
		IS_COMPLEX(long double)

	#undef IS_COLOR

#else

	template < class T >
	struct is_complex< std::complex< T > >
	{
		_MIST_CONST( bool, value, true );
	};

#endif
}

namespace __clapack__
{
	extern "C"
	{
		typedef long int integer;
		typedef float real;
		typedef double doublereal;
		typedef struct { real r, i; } complex;
		typedef struct { doublereal r, i; } doublecomplex;

		// ��ʐ����s��̘A���������������֐�
		int sgesv_( integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info );
		int dgesv_( integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info );
		int cgesv_( integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info );
		int zgesv_( integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info );
		// �Ώ̐����s��̘A���������������֐�
		int ssysv_( char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info );
		int dsysv_( char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info );
		int csysv_( char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info );
		int zsysv_( char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info );


		// ��ʍs���LU����
		int sgetrf_( integer *m, integer *n, real *a, integer *lda, integer *ipiv, integer *info );
		int dgetrf_( integer *m, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info );
		int cgetrf_( integer *m, integer *n, complex *a, integer *lda, integer *ipiv, integer *info );
		int zgetrf_( integer *m, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info );
		// �Ώ̍s���LU����
		int ssytrf_( char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info );
		int dsytrf_( char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info );
		int csytrf_( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int zsytrf_( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );


		// QR����
		int sgeqrf_( integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info );
		int dgeqrf_( integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info );
		int cgeqrf_( integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info );
		int zgeqrf_( integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info );


		// LU�����̌��ʂ�p������ʍs��̋t�s��̌v�Z
		int sgetri_( integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info );
		int dgetri_( integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info );
		int cgetri_( integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int zgetri_( integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );
		// LU�����̌��ʂ�p�����Ώ̍s��̋t�s��̌v�Z
		int ssytri_( char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *info );
		int dsytri_( char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *info );
		int csytri_( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info );
		int zsytri_( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info );


		// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int sgeev_( char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info );
		int dgeev_( char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info );
		int cgeev_( char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info );
		int zgeev_( char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int ssyev_( char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *info );
		int dsyev_( char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *info );


		// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
		int sgeevx_( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, real *work, integer *lwork, integer *iwork, integer *info );
		int dgeevx_( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublereal *work, integer *lwork, integer *iwork, integer *info );
		int cgeevx_( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, complex *work, integer *lwork, real *rwork, integer *info );
		int zgeevx_( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int ssyevx_( char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *ifail, integer *info );
		int dsyevx_( char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *ifail, integer *info );


		// ��ʍs��ɑ΂�����ْl�������v�Z
		int sgesvd_( char *jobu, char *jobvt, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *info );
		int dgesvd_( char *jobu, char *jobvt, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *info );
		int cgesvd_( char *jobu, char *jobvt, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *info );
		int zgesvd_( char *jobu, char *jobvt, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );


		// ��ʍs��ɑ΂�����ْl�������v�Z�D���������@��p�����������o�[�W����
		int sgesdd_( char *jobz, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *iwork, integer *info );
		int dgesdd_( char *jobz, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *iwork, integer *info );
		int cgesdd_( char *jobz, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *iwork, integer *info );
		int zgesdd_( char *jobz, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *info );
	}


	// ���f���y�ю����̗�������ʂ��邱�ƂȂ��C�������̒l�����o���֐�
	inline integer get_real( const real &r ){ return( static_cast< integer >( r ) ); }
	inline integer get_real( const doublereal &r ){ return( static_cast< integer >( r ) ); }
	inline integer get_real( const std::complex< real > &r ){ return( static_cast< integer >( r.real( ) ) ); }
	inline integer get_real( const std::complex< doublereal > &r ){ return( static_cast< integer >( r.real( ) ) ); }


	// ��ʐ����s��̘A���������������֐�
	inline int gesv( integer &n, integer &nrhs, real *a, integer &lda, integer *ipiv, real *b, integer &ldb, integer &info )
	{
		return( sgesv_( &n, &nrhs, a, &lda, ipiv, b, &ldb, &info ) );
	}
	inline int gesv( integer &n, integer &nrhs, doublereal *a, integer &lda, integer *ipiv, doublereal *b, integer &ldb, integer &info )
	{
		return( dgesv_( &n, &nrhs, a, &lda, ipiv, b, &ldb, &info ) );
	}
	inline int gesv( integer &n, integer &nrhs, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *b, integer &ldb, integer &info )
	{
		return( cgesv_( &n, &nrhs, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( b ), &ldb, &info ) );
	}
	inline int gesv( integer &n, integer &nrhs, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *b, integer &ldb, integer &info )
	{
		return( zgesv_( &n, &nrhs, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, &info ) );
	}
	// �Ώ̐����s��̘A���������������֐�
	inline int sysv( char *uplo, integer &n, integer &nrhs, real *a, integer &lda, integer *ipiv, real *b, integer &ldb, real *work, integer &lwork, integer &info )
	{
		return( ssysv_( uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, work, &lwork, &info ) );
	}
	inline int sysv( char *uplo, integer &n, integer &nrhs, doublereal *a, integer &lda, integer *ipiv, doublereal *b, integer &ldb, doublereal *work, integer &lwork, integer &info )
	{
		return( dsysv_( uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, work, &lwork, &info ) );
	}
	inline int sysv( char *uplo, integer &n, integer &nrhs, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *b, integer &ldb, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( csysv_( uplo, &n, &nrhs, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( b ), &ldb, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int sysv( char *uplo, integer &n, integer &nrhs, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *b, integer &ldb, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( zsysv_( uplo, &n, &nrhs, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// ��ʍs���LU����
	inline int getrf( integer &m, integer &n, real *a, integer &lda, integer *ipiv, integer &info )
	{
		return( sgetrf_( &m, &n, a, &lda, ipiv, &info ) );
	}
	inline int getrf( integer &m, integer &n, doublereal *a, integer &lda, integer *ipiv, integer &info )
	{
		return( dgetrf_( &m, &n, a, &lda, ipiv, &info ) );
	}
	inline int getrf( integer &m, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, integer &info )
	{
		return( cgetrf_( &m, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, &info ) );
	}
	inline int getrf( integer &m, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, integer &info )
	{
		return( zgetrf_( &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, &info ) );
	}
	// �Ώ̍s���LU����
	inline int sytrf( char *uplo, integer &n, real *a, integer &lda, integer *ipiv, real *work, integer &lwork, integer &info )
	{
		return( ssytrf_( uplo, &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int sytrf( char *uplo, integer &n, doublereal *a, integer &lda, integer *ipiv, doublereal *work, integer &lwork, integer &info )
	{
		return( dsytrf_( uplo, &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int sytrf( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( csytrf_( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int sytrf( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( zsytrf_( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// QR����
	inline int geqrf( integer &m, integer &n, real *a, integer &lda, real *tau, real *work, integer &lwork, integer &info )
	{
		return( sgeqrf_( &m, &n, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int geqrf( integer &m, integer &n, doublereal *a, integer &lda, doublereal *tau, doublereal *work, integer &lwork, integer &info )
	{
		return( dgeqrf_( &m, &n, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int geqrf( integer &m, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *tau, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( cgeqrf_( &m, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( tau ), reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int geqrf( integer &m, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *tau, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( zgeqrf_( &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( tau ), reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// LU�����̌��ʂ�p������ʍs��̋t�s��̌v�Z
	inline int getri( integer &n, real *a, integer &lda, integer *ipiv, real *work, integer &lwork, integer &info )
	{
		return( sgetri_( &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int getri( integer &n, doublereal *a, integer &lda, integer *ipiv, doublereal *work, integer &lwork, integer &info )
	{
		return( dgetri_( &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int getri( integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( cgetri_( &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int getri( integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( zgetri_( &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}
	// LU�����̌��ʂ�p�����Ώ̍s��̋t�s��̌v�Z
	inline int sytri( char *uplo, integer &n, real *a, integer &lda, integer *ipiv, real *work, integer &info )
	{
		return( ssytri_( uplo, &n, a, &lda, ipiv, work, &info ) );
	}
	inline int sytri( char *uplo, integer &n, doublereal *a, integer &lda, integer *ipiv, doublereal *work, integer &info )
	{
		return( dsytri_( uplo, &n, a, &lda, ipiv, work, &info ) );
	}
	inline int sytri( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &info )
	{
		return( csytri_( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &info ) );
	}
	inline int sytri( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &info )
	{
		return( zsytri_( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &info ) );
	}


	// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int geev( char *jobvl, char *jobvr, integer &n, real *a, integer &lda, real *wr, real *wi,
							real *vl, integer &ldvl, real *vr, integer &ldvr, real *work, integer &lwork, integer &info )
	{
		return( sgeev_( jobvl, jobvr, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info ) );
	}
	inline int geev( char *jobvl, char *jobvr, integer &n, doublereal *a, integer &lda, doublereal *wr, doublereal *wi,
							doublereal *vl, integer &ldvl, doublereal *vr, integer &ldvr, doublereal *work, integer &lwork, integer &info )
	{
		return( dgeev_( jobvl, jobvr, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info ) );
	}
	inline int geev( char *jobvl, char *jobvr, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *w, std::complex< real > *vl, integer &ldvl,
										std::complex< real > *vr, integer &ldvr, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( cgeev_( jobvl, jobvr, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( w ), reinterpret_cast< complex* >( vl ), &ldvl,
							reinterpret_cast< complex* >( vr ), &ldvr, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int geev( char *jobvl, char *jobvr, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *w, std::complex< doublereal > *vl, integer &ldvl,
										std::complex< doublereal > *vr, integer &ldvr, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( zgeev_( jobvl, jobvr, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( w ), reinterpret_cast< doublecomplex* >( vl ), &ldvl,
							reinterpret_cast< doublecomplex* >( vr ), &ldvr, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}

	// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int syev( char *jobz, char *uplo, integer &n, real *a, integer &lda, real *w, real *work, integer &lwork, integer &info )
	{
		return( ssyev_( jobz, uplo, &n, a, &lda, w, work, &lwork, &info ) );
	}
	inline int syev( char *jobz, char *uplo, integer &n, doublereal *a, integer &lda, doublereal *w, doublereal *work, integer &lwork, integer &info )
	{
		return( dsyev_( jobz, uplo, &n, a, &lda, w, work, &lwork, &info ) );
	}


	// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, real *a, integer &lda, real *wr, real *wi,
										real *vl, integer &ldvl, real *vr, integer &ldvr, integer &ilo, integer &ihi, real *scale,
										real &abnrm, real *rconde, real *rcondv, real *work, integer &lwork, integer *iwork, integer &info )
	{
		return( sgeevx_( balanc, jobvl, jobvr, sense, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, work, &lwork, iwork, &info ) );
	}
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, doublereal *a, integer &lda, doublereal *wr, doublereal *wi,
										doublereal *vl, integer &ldvl, doublereal *vr, integer &ldvr, integer &ilo, integer &ihi, doublereal *scale,
										doublereal &abnrm, doublereal *rconde, doublereal *rcondv, doublereal *work, integer &lwork, integer *iwork, integer &info )
	{
		return( dgeevx_( balanc, jobvl, jobvr, sense, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, work, &lwork, iwork, &info ) );
	}
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *w,
										std::complex< real > *vl, integer &ldvl, std::complex< real > *vr, integer &ldvr, integer &ilo, integer &ihi, real *scale,
										real &abnrm, real *rconde, real *rcondv, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( cgeevx_( balanc, jobvl, jobvr, sense, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( w ), reinterpret_cast< complex* >( vl ), &ldvl,
								reinterpret_cast< complex* >( vr ), &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *w,
										std::complex< doublereal > *vl, integer &ldvl, std::complex< doublereal > *vr, integer &ldvr, integer &ilo, integer &ihi, doublereal *scale,
										doublereal &abnrm, doublereal *rconde, doublereal *rcondv, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( zgeevx_( balanc, jobvl, jobvr, sense, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( w ), reinterpret_cast< doublecomplex* >( vl ), &ldvl,
								reinterpret_cast< doublecomplex* >( vr ), &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}


	// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int geevx( char *jobz, char *range, char *uplo, integer &n, real *a, integer &lda,
						real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, real *z__,
						integer &ldz, real *work, integer &lwork, integer *iwork, integer *ifail, integer &info )
	{
		return( ssyevx_( jobz, range, uplo, &n, a, &lda, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, &lwork, iwork, ifail, &info ) );
	}
	inline int geevx( char *jobz, char *range, char *uplo, integer &n, doublereal *a, integer &lda,
						doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, doublereal *z__,
						integer &ldz, doublereal *work, integer &lwork, integer *iwork, integer *ifail, integer &info )
	{
		return( dsyevx_( jobz, range, uplo, &n, a, &lda, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, &lwork, iwork, ifail, &info ) );
	}


	// ��ʍs��ɑ΂�����ْl�������v�Z
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, real *a, integer &lda, real *s, real *u,
								integer &ldu, real *vt, integer &ldvt, real *work, integer &lwork, integer &info )
	{
		return( sgesvd_( jobu, jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, &info ) );
	}
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, doublereal *a, integer &lda, doublereal *s, doublereal *u,
								integer &ldu, doublereal *vt, integer &ldvt, doublereal *work, integer &lwork, integer &info )
	{
		return( dgesvd_( jobu, jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, &info ) );
	}
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, std::complex< real > *a, integer &lda, real *s, std::complex< real > *u,
								integer &ldu, std::complex< real > *vt, integer &ldvt, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( cgesvd_( jobu, jobvt, &m, &n, reinterpret_cast< complex* >( a ), &lda, s, reinterpret_cast< complex* >( u ),
								&ldu, reinterpret_cast< complex* >( vt ), &ldvt, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *s, std::complex< doublereal > *u,
								integer &ldu, std::complex< doublereal > *vt, integer &ldvt, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( zgesvd_( jobu, jobvt, &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, s, reinterpret_cast< doublecomplex* >( u ),
								&ldu, reinterpret_cast< doublecomplex* >( vt ), &ldvt, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}

	inline int gesdd( char *jobz, integer &m, integer &n, real *a, integer &lda, real *s, real *u, integer &ldu,
								real *vt, integer &ldvt, real *work, integer &lwork, integer *iwork, integer &info )
	{
		return( sgesdd_( jobz, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, iwork, &info ) );
	}
	inline int gesdd( char *jobz, integer &m, integer &n, doublereal *a, integer &lda, doublereal *s, doublereal *u, integer &ldu,
								doublereal *vt, integer &ldvt, doublereal *work, integer &lwork, integer *iwork, integer &info )
	{
		return( dgesdd_( jobz, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, iwork, &info ) );
	}
	inline int gesdd( char *jobz, integer &m, integer &n, std::complex< real > *a, integer &lda, real *s, std::complex< real > *u, integer &ldu,
								std::complex< real > *vt, integer &ldvt, std::complex< real > *work, integer &lwork, real *rwork, integer *iwork, integer &info )
	{
		return( cgesdd_( jobz, &m, &n, reinterpret_cast< complex* >( a ), &lda, s, reinterpret_cast< complex* >( u ), &ldu,
							reinterpret_cast< complex* >( vt ), &ldvt, reinterpret_cast< complex* >( work ), &lwork, rwork, iwork, &info ) );
	}
	inline int gesdd( char *jobz, integer &m, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *s, std::complex< doublereal > *u, integer &ldu,
								std::complex< doublereal > *vt, integer &ldvt, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer *iwork, integer &info )
	{
		return( zgesdd_( jobz, &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, s, reinterpret_cast< doublecomplex* >( u ), &ldu,
							reinterpret_cast< doublecomplex* >( vt ), &ldvt, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, iwork, &info ) );
	}
}



/// @brief �g���[�X�̌v�Z�i�Ίp�����̘a�j
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const typename matrix< T, Allocator >::value_type trace( const matrix< T, Allocator > &a )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef typename matrix< T, Allocator >::value_type value_type;
	value_type v = value_type( );
	size_type size = a.rows( ) < a.cols( ) ? a.rows( ) : a.cols( );
	for( size_type i = 0 ; i < size ; ++i )
	{
		v += a( i, i );
	}
	return( v );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

template < class Expression >
inline const typename matrix_expression< Expression >::value_type trace( const matrix_expression< Expression > &expression )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::size_type size_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;

	value_type v = value_type( );
	size_type size = expression.rows( ) < expression.cols( ) ? expression.rows( ) : expression.cols( );
	for( size_type i = 0 ; i < size ; ++i )
	{
		v += expression( i, i );
	}

	return( v );
}

#endif


/// @brief �s�񎮂̌v�Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const typename matrix< T, Allocator >::value_type det( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef typename matrix< T, Allocator >::value_type value_type;

	if( a.empty( ) )
	{
		return( value_type( ) );
	}

	matrix< __clapack__::integer, typename Allocator::template rebind< __clapack__::integer >::other > pivot;
	matrix< T, Allocator > m = lu_factorization( a, pivot, style );

	value_type v = m( 0, 0 );
	size_type size = a.rows( ) < a.cols( ) ? a.rows( ) : a.cols( );
	size_type count = 0, i;

	// LU�������ɍs�̓���ւ����s��ꂽ�񐔂��v�Z����
	for( i = 0 ; i < pivot.size( ) ; ++i )
	{
		count += static_cast< size_type >( pivot[ i ] ) != i + 1 ? 1 : 0;
	}

	// �Ίp�����̐ς��v�Z����
	for( i = 1 ; i < size ; ++i )
	{
		v *= m( i, i );
	}

	return( v * ( count % 2 == 0 ? 1 : -1 ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline const typename matrix_expression< Expression >::value_type det( const matrix_expression< Expression > &expression, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::size_type size_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;

	if( expression.rows( ) * expression.cols( ) == 0 )
	{
		return( value_type( ) );
	}

	matrix< __clapack__::integer, typename allocator_type::template rebind< __clapack__::integer >::other > pivot;
	matrix_type m = lu_factorization( expression, pivot, style );

	value_type v = m( 0, 0 );
	size_type size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
	size_type count = 0, i;

	// LU�������ɍs�̓���ւ����s��ꂽ�񐔂��v�Z����
	for( i = 0 ; i < pivot.size( ) ; ++i )
	{
		count += static_cast< size_type >( pivot[ i ] ) != i + 1 ? 1 : 0;
	}

	// �Ίp�����̐ς��v�Z����
	for( i = 1 ; i < size ; ++i )
	{
		v *= m( i, i );
	}

	return( v * ( count % 2 == 0 ? 1 : -1 ) );
}

#endif



/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T >
inline const matrix< typename type_trait< T >::value_type > diag( const T &s1, const typename type_trait< T >::value_type &s2, const typename type_trait< T >::value_type &s3 )
{
	matrix< typename type_trait< T >::value_type > d( 3, 3 );
	if( s1 < s2 )
	{
		// s1 < s2
		if( s1 < s3 )
		{
			if( s2 < s3 )
			{
				d( 0, 0 ) = s3;
				d( 1, 1 ) = s2;
				d( 2, 2 ) = s1;
			}
			else
			{
				d( 0, 0 ) = s2;
				d( 1, 1 ) = s3;
				d( 2, 2 ) = s1;
			}
		}
		else
		{
			d( 0, 0 ) = s2;
			d( 1, 1 ) = s1;
			d( 2, 2 ) = s3;
		}
	}
	else
	{
		// s2 < s1
		if( s1 < s3 )
		{
			d( 0, 0 ) = s3;
			d( 1, 1 ) = s1;
			d( 2, 2 ) = s2;
		}
		else
		{
			if( s2 < s3 )
			{
				d( 0, 0 ) = s1;
				d( 1, 1 ) = s3;
				d( 2, 2 ) = s2;
			}
			else
			{
				d( 0, 0 ) = s1;
				d( 1, 1 ) = s2;
				d( 2, 2 ) = s3;
			}
		}
	}
	return( d );
}


namespace __solve__
{
	// �s��̘A���ꎟ�������������֐�
	template < bool b >
	struct __solve__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& solve( matrix< T, Allocator > &a, matrix< T, Allocator > &b, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;

			switch( style )
			{
			case matrix_style::sy:
				{
					b.resize( a.rows( ), 1 );

					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = 1;
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					value_type dmy;
					integer *ipiv = new integer[ n ];
					integer info  = 0;
					integer lwork  = -1;
					char *uplo    = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sysv( uplo, n, nrhs, NULL, lda, NULL, NULL, ldb, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sysv( uplo, n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, &( work[0] ), lwork, info );
					}
					delete [] ipiv;
				}
				break;

			case matrix_style::ge:
			default:
				{
					b.resize( a.rows( ), 1 );

					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = 1;
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					integer *ipiv = new integer[ n ];
					integer info  = 0;

					// integer     n: �}�g���b�N�X a �̗�
					// integer  nrhs: �}�g���b�N�X b �̗�
					// double*     a: �A���������������W���̔z��i lda�~n �v�f �j
					// integer   lda: �}�g���b�N�X a �̍s��
					// integer* ipiv: �s�{�b�g�I���ɗp����z��i lda�v�f �j
					// double*     b: �A���������������W���̔z��i ldb�~n �v�f �j
					// integer   ldb: �}�g���b�N�X b �̍s��
					// integer  info: ����I���������ۂ���m�点�Ă����ϐ�
					//
					// ���̊֐����Ăԏꍇ�C���͂ƂȂ� a �̓��e�͕ύX�����
					// �ŏI�I�Ȍ��ʂ� b �ɑ�������
					__clapack__::gesv( n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, info );
					delete [] ipiv;
				}
				break;
			}

			return( b );
		}
	};

	template < >
	struct __solve__< true >
	{
		// ���f���o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& solve( matrix< T, Allocator > a, matrix< T, Allocator > &b, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			switch( style )
			{
			case matrix_style::sy:
				{
					b.resize( a.rows( ), 1 );

					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = 1;
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					value_type dmy;
					integer *ipiv = new integer[ n ];
					integer info  = 0;
					integer lwork  = -1;
					char *uplo    = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sysv( uplo, n, nrhs, NULL, lda, NULL, NULL, ldb, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sysv( uplo, n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, &( work[0] ), lwork, info );
					}
					delete [] ipiv;
				}
				break;

			case matrix_style::ge:
			default:
				{
					b.resize( a.rows( ), 1 );

					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = 1;
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					integer *ipiv = new integer[ n ];
					integer info  = 0;

					// integer     n: �}�g���b�N�X a �̗�
					// integer  nrhs: �}�g���b�N�X b �̗�
					// double*     a: �A���������������W���̔z��i lda�~n �v�f �j
					// integer   lda: �}�g���b�N�X a �̍s��
					// integer* ipiv: �s�{�b�g�I���ɗp����z��i lda�v�f �j
					// double*     b: �A���������������W���̔z��i ldb�~n �v�f �j
					// integer   ldb: �}�g���b�N�X b �̍s��
					// integer  info: ����I���������ۂ���m�点�Ă����ϐ�
					//
					// ���̊֐����Ăԏꍇ�C���͂ƂȂ� a �̓��e�͕ύX�����
					// �ŏI�I�Ȍ��ʂ� b �ɑ�������
					__clapack__::gesv( n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, info );
					delete [] ipiv;
				}
				break;
			}

			return( b );
		}
	};
}


/// @brief �s��̘A���ꎟ�������������֐�
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
inline const matrix< T, Allocator >& solve( const matrix< T, Allocator > &a, matrix< T, Allocator > &b, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	return( __solve__::__solve__< __numeric__::is_complex< T >::value >::solve( a_, b, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >&
			solve( const matrix_expression< Expression > &expression, matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type > &b, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
	return( __solve__::__solve__< __numeric__::is_complex< value_type >::value >::solve( a_, b, style ) );
}

#endif





namespace __lu__
{
	// �s��̘A���ꎟ�������������֐�
	template < bool b >
	struct __lu__
	{
		// �����o�[�W����
		template < class T, class Allocator1, class Allocator2 >
		static matrix< T, Allocator1 >& lu_factorization( matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator1 >::value_type value_type;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer info   = 0;
					integer lwork  = -1;
					value_type dmy;
					char *uplo    = "U";

					pivot.resize( n, 1 );

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sytrf( uplo, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator1 > work( lwork, 1 );

						// LU�������s��
						__clapack__::sytrf( uplo, n, &( a[0] ), lda, &( pivot[0] ), &( work[0] ), lwork, info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer info   = 0;

					pivot.resize( n, 1 );

					// LU�������s��
					__clapack__::getrf( m, n, &( a[0] ), lda, &( pivot[0] ), info );
				}
				break;
			}

			return( a );
		}
	};

	template < >
	struct __lu__< true >
	{
		// ���f���o�[�W����
		template < class T, class Allocator1, class Allocator2 >
		static matrix< T, Allocator1 >& lu_factorization( matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer info   = 0;
					integer lwork  = -1;
					value_type dmy;
					char *uplo    = "U";

					pivot.resize( n, 1 );

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sytrf( uplo, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator1 > work( lwork, 1 );

						// LU�������s��
						__clapack__::sytrf( uplo, n, &( a[0] ), lda, &( pivot[0] ), &( work[0] ), lwork, info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer info   = 0;

					pivot.resize( n, 1 );

					// LU�������s��
					__clapack__::getrf( m, n, &( a[0] ), lda, &( pivot[0] ), info );
				}
				break;
			}

			return( a );
		}
	};
}

/// @brief ��ʍs���LU�������s��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator1, class Allocator2 >
const matrix< T, Allocator1 > lu_factorization( const matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator1 > a_( a );
	return( __lu__::__lu__< __numeric__::is_complex< T >::value >::lu_factorization( a_, pivot, style ) );
}

/// @brief ��ʍs���LU�������s��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
const matrix< T, Allocator > lu_factorization( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef __clapack__::integer integer;
	matrix< T, Allocator > a_( a );
	matrix< __clapack__::integer, typename Allocator::template rebind< __clapack__::integer >::other > pivot( a.cols( ), 1 );
	return( __lu__::__lu__< __numeric__::is_complex< T >::value >::lu_factorization( a_, pivot, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression, class Allocator2 >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >
			lu_factorization( const matrix_expression< Expression > &expression, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
	return( __lu__::__lu__< __numeric__::is_complex< value_type >::value >::lu_factorization( a_, pivot, style ) );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >
						lu_factorization( const matrix_expression< Expression > &expression, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
	matrix< __clapack__::integer, typename allocator_type::template rebind< __clapack__::integer >::other > pivot( a_.cols( ), 1 );
	return( __lu__::__lu__< __numeric__::is_complex< value_type >::value >::lu_factorization( a_, pivot, style ) );
}

#endif



namespace __qr__
{
	// �s��̘A���ꎟ�������������֐�
	template < bool b >
	struct __qr__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& qr_factorization( matrix< T, Allocator > &a, matrix< T, Allocator > &tau, matrix_style::style style )
		{
			typedef __clapack__::integer integer;

			switch( style )
			{
			case matrix_style::sy:
			case matrix_style::ge:
			default:
				{
					tau.resize( a.rows( ), a.cols( ) );

					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					integer info   = 0;

					// QR�������s���O�ɁC�K�v�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geqrf( m, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geqrf( m, n, &( a[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );
					}
				}
				break;
			}

			return( a );
		}
	};

	template < >
	struct __qr__< true >
	{
		// ���f���o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& qr_factorization( matrix< T, Allocator > &a, matrix< T, Allocator > &tau, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::vaqre_type vaqre_type;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					tau.resize( a.rows( ), a.cols( ) );

					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					integer info   = 0;

					// QR�������s���O�ɁC�K�v�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geqrf( m, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geqrf( m, n, &( a[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );
					}
				}
				break;
			}

			return( a );
		}
	};
}

/// @brief ��ʍs���QR�������s��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
const matrix< T, Allocator > qr_factorization( const matrix< T, Allocator > &a, matrix< T, Allocator > &tau, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	return( __qr__::__qr__< __numeric__::is_complex< T >::value >::qr_factorization( a_, tau, style ) );
}

/// @brief ��ʍs���QR�������s��
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
const matrix< T, Allocator > qr_factorization( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef __clapack__::integer integer;
	matrix< T, Allocator > a_( a ), tau( a.rows( ), a.cols( ) );
	return( __qr__::__qr__< __numeric__::is_complex< T >::value >::qr_factorization( a_, tau, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >
			qr_factorization( const matrix_expression< Expression > &expression,
								matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type > &tau,
								matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
	return( __qr__::__qr__< __numeric__::is_complex< value_type >::value >::qr_factorization( a_, tau, style ) );
}

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >
						qr_factorization( const matrix_expression< Expression > &expression, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression ), tau( a_.rows( ), a_.cols( ) );
	return( __qr__::__qr__< __numeric__::is_complex< value_type >::value >::qr_factorization( a_, tau, style ) );
}

#endif


namespace __inverse__
{
	// �s��̘A���ꎟ�������������֐�
	template < bool b >
	struct __inverse__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& inverse( matrix< T, Allocator > &a, matrix_style::style style )
		{
			typedef __clapack__::integer integer;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					integer info   = 0;
					char *uplo    = "U";

					// LU�������s��
					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sytrf( uplo, n, NULL, lda, ipiv, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sytrf( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );

						if( info == 0 )
						{
							work.resize( 2 * n, 1 );
							__clapack__::sytri( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), info );
						}
					}
					delete [] ipiv;
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					integer info   = 0;

					// LU�������s��
					__clapack__::getrf( lda, n, &( a[0] ), lda, ipiv, info );
					if( info == 0 )
					{
						// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
						__clapack__::getri( n, NULL, lda, NULL, &dmy, lwork, info );
						if( info == 0 )
						{
							lwork = __clapack__::get_real( dmy );
							matrix< T, Allocator > work( lwork, 1 );
							__clapack__::getri( n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );
						}
					}
					delete [] ipiv;
				}
				break;
			}

			return( a );
		}
	};

	template < >
	struct __inverse__< true >
	{
		// ���f���o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& inverse( matrix< T, Allocator > &a, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::vaqre_type vaqre_type;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					integer info   = 0;
					char *uplo    = "U";

					// LU�������s��
					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sytrf( uplo, n, NULL, lda, ipiv, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sytrf( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );

						if( info == 0 )
						{
							work.resize( 2 * n, 1 );
							__clapack__::sytri( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), info );
						}
					}
					delete [] ipiv;
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					integer info   = 0;

					// LU�������s��
					__clapack__::getrf( lda, n, &( a[0] ), lda, ipiv, info );
					if( info == 0 )
					{
						// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
						__clapack__::getri( n, NULL, lda, NULL, &dmy, lwork, info );
						if( info == 0 )
						{
							lwork = __clapack__::get_real( dmy );
							matrix< T, Allocator > work( lwork, 1 );
							__clapack__::getri( n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );
						}
					}
					delete [] ipiv;
				}
				break;
			}

			return( a );
		}
	};
}


/// @brief ��ʍs��̋t�s���LU������p���Čv�Z����
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
matrix< T, Allocator > inverse( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	return( __inverse__::__inverse__< __numeric__::is_complex< T >::value >::inverse( a_, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >
						inverse( const matrix_expression< Expression > &expression, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
	return( __inverse__::__inverse__< __numeric__::is_complex< value_type >::value >::inverse( a_, style ) );
}

#endif



namespace __eigen__
{
	// ��ʍs��̌ŗL�l�E�ŗL�x�N�g�����v�Z����
#if defined(_USE_BALANCING_MATRIX_EIGEN_) && _USE_BALANCING_MATRIX_EIGEN_ != 0

	// �s��̑Ίp�����s�키�o�[�W����
	template < bool b >
	struct __eigen__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer n         = static_cast< integer >( a.cols( ) );
					integer m         = n;
					integer lda       = static_cast< integer >( a.rows( ) );
					integer ldz       = n;
					value_type dmy    = 0;
					integer lwork     = -1;
					integer info      = 0;
					value_type vl     = 0;
					value_type vu     = 0;
					integer il        = 0;
					integer iu        = 0;
					value_type abstol = 0;
					char *jobz        = "V";
					char *range       = "A";
					char *uplo        = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geevx( jobz, range, uplo, n, NULL, lda, vl, vu, il, iu, abstol, m, NULL, NULL, ldz, &dmy, lwork, NULL, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						matrix< integer > iwork( 5 * n, 1 );
						matrix< integer > ifail( n, 1 );
						eigen_vector.resize( n, n );

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geevx( jobz, range, uplo, n, &( a[0] ), lda, vl, vu, il, iu, abstol, m,
										&( eigen_value[0] ), &( eigen_vector[0] ), ldz, &( work[0] ), lwork, &( iwork[ 0 ] ), &( ifail[ 0 ] ), info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy, abnrm;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					integer info   = 0;
					integer ilo    = 0;
					integer ihi    = 0;
					char *balanc = "B";
					char *jobvl  = "N";
					char *jobvr  = "V";
					char *sense  = "N";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geevx( balanc, jobvl, jobvr, sense, n, NULL, lda, NULL, NULL, NULL, ldvl, NULL, ldvr, ilo, ihi, NULL, abnrm, NULL, NULL, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						matrix< T, Allocator > tmp( n, 1 );
						matrix< T, Allocator > scale( n, 1 );
						eigen_vector.resize( n, n );

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geevx( balanc, jobvl, jobvr, sense, n, &( a[0] ), lda, &( eigen_value[0] ), &( tmp[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, ilo, ihi, &( scale[0] ), abnrm, NULL, NULL, &( work[0] ), lwork, NULL, info );
					}
				}
				break;
			}

			return( eigen_value );
		}
	};

	template < >
	struct __eigen__< true >
	{
		// ���f���o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{

					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					value_type abnrm;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					integer info   = 0;
					integer ilo    = 0;
					integer ihi    = 0;
					char *balanc = "B";
					char *jobvl  = "N";
					char *jobvr  = "V";
					char *sense  = "N";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geevx( balanc, jobvl, jobvr, sense, n, NULL, lda, NULL, NULL, ldvl, NULL, ldvr, ilo, ihi, NULL, abnrm, NULL, NULL, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						eigen_vector.resize( n, n );
						value_type *scale = new value_type[ n ];
						value_type *rwork = new value_type[ 2 * n ];

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geevx( balanc, jobvl, jobvr, sense, n, &( a[0] ), lda, &( eigen_value[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, ilo, ihi, scale, abnrm, NULL, NULL, &( work[0] ), lwork, rwork, info );

						delete [] scale;
						delete [] rwork;
					}
				}
				break;
			}

			return( eigen_value );
		}
	};


#else

	// �s��̑Ίp�����s��Ȃ��o�[�W����
	template < bool b >
	struct __eigen__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					integer info   = 0;
					char *jobz = "V";
					char *uplo = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::syev( jobz, uplo, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						eigen_vector = a;

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::syev( jobz, uplo, n, &( eigen_vector[0] ), lda, &( eigen_value[0] ), &( work[0] ), lwork, info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					integer info   = 0;
					char *jobvl = "N";
					char *jobvr = "V";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geev( jobvl, jobvr, n, NULL, lda, NULL, NULL, NULL, ldvl, NULL, ldvr, &dmy, lwork, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						matrix< T, Allocator > tmp( n, 1 );
						eigen_vector.resize( n, n );

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geev( jobvl, jobvr, n, &( a[0] ), lda, &( eigen_value[0] ), &( tmp[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, &( work[0] ), lwork, info );
					}
				}
				break;
			}

			return( eigen_value );
		}
	};

	template < >
	struct __eigen__< true >
	{
		// ���f���o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					integer info   = 0;
					char *jobvl = "N";
					char *jobvr = "V";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geev( jobvl, jobvr, n, NULL, lda, NULL, NULL, ldvl, NULL, ldvr, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						eigen_vector.resize( n, n );
						value_type *rwork = new value_type[ 2 * n ];

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geev( jobvl, jobvr, n, &( a[0] ), lda, &( eigen_value[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, &( work[0] ), lwork, rwork, info );

						delete [] rwork;
					}
				}
				break;
			}

			return( eigen_value );
		}
	};

#endif
}


/// @brief ��ʍs��̌ŗL�l�E�ŗL�x�N�g�����v�Z����
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
const matrix< T, Allocator >& eigen( const matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	return( __eigen__::__eigen__< __numeric__::is_complex< T >::value >::eigen( a_, eigen_value, eigen_vector, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >&
						eigen( const matrix_expression< Expression > &expression,
						matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type > &eigen_value,
						matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type > &eigen_vector,
						matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
	return( __eigen__::__eigen__< __numeric__::is_complex< value_type >::value >::eigen( a_, eigen_value, eigen_vector, style ) );
}

#endif



namespace __svd__
{
	// ��ʍs��̓��ْl�������v�Z����
#if defined(_USE_DIVIDE_AND_CONQUER_SVD_) && _USE_DIVIDE_AND_CONQUER_SVD_ != 0

	// ���������@��p����o�[�W�����̓��ْl����
	template < bool b >
	struct __svd__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &u, matrix< T, Allocator > &s, matrix< T, Allocator > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					integer size   = m < n ? m : n;
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					integer info   = 0;
					char *jobz = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesdd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
						matrix< T, Allocator > ss( size, 1 );
						vt.resize( ldvt, n );
						integer *iwork = new integer[ 8 * size ];

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::gesdd( jobz, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, iwork, info );

						delete [] iwork;

						s.resize( m, n );
						for( size_type i = 0 ; i < ss.rows( ) ; i++ )
						{
							s( i, i ) = ss[ i ];
						}
					}
				}
				break;
			}

			return( s );
		}
	};

	template < >
	struct __svd__< true >
	{
		// ���f���o�[�W����
		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename T1::value_type value_type;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					integer size   = m < n ? m : n;
					typename matrix< T1, Allocator1 >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					integer info   = 0;
					char *jobz = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesdd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
						matrix< typename T1::value_type > ss( size, 1 );
						vt.resize( ldvt, n );
						value_type *rwork = new value_type[ 5 * size * size + 5 * size ];
						integer *iwork = new integer[ 8 * size ];

						lwork = __clapack__::get_real( dmy );
						matrix< T1, Allocator1 > work( lwork, 1 );
						__clapack__::gesdd( jobz, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, rwork, iwork, info );

						delete [] rwork;
						delete [] iwork;

						s.resize( m, n );
						for( size_type i = 0 ; i < ss.rows( ) ; i++ )
						{
							s( i, i ) = ss[ i ];
						}
					}
				}
				break;
			}

			return( s );
		}
	};

#else

	template < bool b >
	struct __svd__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &u, matrix< T, Allocator > &s, matrix< T, Allocator > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					integer info   = 0;
					char *jobu = "A";
					char *jobvt = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesvd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
						matrix< T, Allocator > ss( m < n ? m : n, 1 );
						vt.resize( ldvt, n );

						lwork = __clapack__::get_real( dmy );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::gesvd( jobu, jobvt, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, info );

						s.resize( m, n );
						for( size_type i = 0 ; i < ss.rows( ) ; i++ )
						{
							s( i, i ) = ss[ i ];
						}
					}
				}
				break;
			}

			return( s );
		}
	};

	template < >
	struct __svd__< true >
	{
		// ���f���o�[�W����
		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename T1::value_type value_type;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					typename matrix< T1, Allocator1 >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					integer info   = 0;
					char *jobu = "A";
					char *jobvt = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesvd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
						matrix< typename T1::value_type > ss( m < n ? m : n, 1 );
						vt.resize( ldvt, n );
						value_type *rwork = new value_type[ 5 * ( m < n ? m : n ) ];

						lwork = __clapack__::get_real( dmy );
						matrix< T1, Allocator1 > work( lwork, 1 );
						__clapack__::gesvd( jobu, jobvt, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, rwork, info );

						s.resize( m, n );
						for( size_type i = 0 ; i < ss.rows( ) ; i++ )
						{
							s( i, i ) = ss[ i ];
						}

						delete [] rwork;
					}
				}
				break;
			}

			return( s );
		}
	};

#endif

}


/// @brief ��ʍs��̓��ْl�������v�Z����
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
const matrix< T2, Allocator2 >& svd( const matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style = matrix_style::ge )
{
	matrix< T1, Allocator1 > a_( a );
	return( __svd__::__svd__< __numeric__::is_complex< T1 >::value >::svd( a_, u, s, vt, style ) );
}


#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class Expression >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >&
						svd( const matrix_expression< Expression > &expression,
						matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type > &u,
						matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type > &s,
						matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type > &vt,
						matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
	return( __svd__::__svd__< __numeric__::is_complex< value_type >::value >::svd( a_, u, s, vt, style ) );
}

#endif


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_NUMERIC__
