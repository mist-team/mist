#ifndef __INCLUDE_MATRIX_UTIL__
#define __INCLUDE_MATRIX_UTIL__

#include <complex>


// �s��N���X���C���N���[�h����Ă��Ȃ��ꍇ�̓C���|�[�g����D
#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __matrix_utility__
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
		#include <f2c.h>
		// ��ʐ����s��̘A���������������֐�
		int sgesv_( integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info );
		int dgesv_( integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info );
		int cgesv_( integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info );
		int zgesv_( integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info );

		// LU����
		int sgetrf_( integer *m, integer *n, real *a, integer *lda, integer *ipiv, integer *info );
		int dgetrf_( integer *m, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info );
		int cgetrf_( integer *m, integer *n, complex *a, integer *lda, integer *ipiv, integer *info );
		int zgetrf_( integer *m, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info );

		// QR����
		int sgeqrf_( integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info );
		int dgeqrf_( integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info );
		int cgeqrf_( integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info );
		int zgeqrf_( integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info );

		// LU�����̌��ʂ�p�����t�s��̌v�Z
		int sgetri_( integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info );
		int dgetri_( integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info );
		int cgetri_( integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int zgetri_( integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );

		// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int sgeev_( char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info );
		int dgeev_( char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info );
		int cgeev_( char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info );
		int zgeev_( char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );

		// ��ʍs��ɑ΂�����ْl�������v�Z
		int sgesvd_( char *jobu, char *jobvt, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *info );
		int dgesvd_( char *jobu, char *jobvt, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *info );
		int cgesvd_( char *jobu, char *jobvt, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *info );
		int zgesvd_( char *jobu, char *jobvt, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );

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
	inline int solve( integer &n, integer &nrhs, real *a, integer &lda, integer *ipiv, real *b, integer &ldb, integer &info )
	{
		return( sgesv_( &n, &nrhs, a, &lda, ipiv, b, &ldb, &info ) );
	}
	inline int solve( integer &n, integer &nrhs, doublereal *a, integer &lda, integer *ipiv, doublereal *b, integer &ldb, integer &info )
	{
		return( dgesv_( &n, &nrhs, a, &lda, ipiv, b, &ldb, &info ) );
	}
	inline int solve( integer &n, integer &nrhs, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *b, integer &ldb, integer &info )
	{
		return( cgesv_( &n, &nrhs, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( b ), &ldb, &info ) );
	}
	inline int solve( integer &n, integer &nrhs, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *b, integer &ldb, integer &info )
	{
		return( zgesv_( &n, &nrhs, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, &info ) );
	}


	// LU����
	inline int lu_factorization( integer &m, integer &n, real *a, integer &lda, integer *ipiv, integer &info )
	{
		return( sgetrf_( &m, &n, a, &lda, ipiv, &info ) );
	}
	inline int lu_factorization( integer &m, integer &n, doublereal *a, integer &lda, integer *ipiv, integer &info )
	{
		return( dgetrf_( &m, &n, a, &lda, ipiv, &info ) );
	}
	inline int lu_factorization( integer &m, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, integer &info )
	{
		return( cgetrf_( &m, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, &info ) );
	}
	inline int lu_factorization( integer &m, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, integer &info )
	{
		return( zgetrf_( &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, &info ) );
	}


	// QR����
	inline int qr_factorization( integer &m, integer &n, real *a, integer &lda, real *tau, real *work, integer &lwork, integer &info )
	{
		return( sgeqrf_( &m, &n, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int qr_factorization( integer &m, integer &n, doublereal *a, integer &lda, doublereal *tau, doublereal *work, integer &lwork, integer &info )
	{
		return( dgeqrf_( &m, &n, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int qr_factorization( integer &m, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *tau, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( cgeqrf_( &m, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( tau ), reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int qr_factorization( integer &m, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *tau, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( zgeqrf_( &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( tau ), reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// LU�����̌��ʂ�p�����t�s��̌v�Z
	inline int inverse( integer &n, real *a, integer &lda, integer *ipiv, real *work, integer &lwork, integer &info )
	{
		return( sgetri_( &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int inverse( integer &n, doublereal *a, integer &lda, integer *ipiv, doublereal *work, integer &lwork, integer &info )
	{
		return( dgetri_( &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int inverse( integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( cgetri_( &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int inverse( integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( zgetri_( &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int eigen( char *jobvl, char *jobvr, integer &n, real *a, integer &lda, real *wr, real *wi,
							real *vl, integer &ldvl, real *vr, integer &ldvr, real *work, integer &lwork, integer &info )
	{
		return( sgeev_( jobvl, jobvr, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info ) );
	}
	inline int eigen( char *jobvl, char *jobvr, integer &n, doublereal *a, integer &lda, doublereal *wr, doublereal *wi,
							doublereal *vl, integer &ldvl, doublereal *vr, integer &ldvr, doublereal *work, integer &lwork, integer &info )
	{
		return( dgeev_( jobvl, jobvr, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info ) );
	}
	inline int eigen( char *jobvl, char *jobvr, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *w, std::complex< real > *vl, integer &ldvl,
										std::complex< real > *vr, integer &ldvr, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( cgeev_( jobvl, jobvr, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( w ), reinterpret_cast< complex* >( vl ), &ldvl,
							reinterpret_cast< complex* >( vr ), &ldvr, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int eigen( char *jobvl, char *jobvr, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *w, std::complex< doublereal > *vl, integer &ldvl,
										std::complex< doublereal > *vr, integer &ldvr, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( zgeev_( jobvl, jobvr, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( w ), reinterpret_cast< doublecomplex* >( vl ), &ldvl,
							reinterpret_cast< doublecomplex* >( vr ), &ldvr, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}


	// ��ʍs��ɑ΂�����ْl�������v�Z
	inline int svd( char *jobu, char *jobvt, integer &m, integer &n, real *a, integer &lda, real *s, real *u,
								integer &ldu, real *vt, integer &ldvt, real *work, integer &lwork, integer &info )
	{
		return( sgesvd_( jobu, jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, &info ) );
	}
	inline int svd( char *jobu, char *jobvt, integer &m, integer &n, doublereal *a, integer &lda, doublereal *s, doublereal *u,
								integer &ldu, doublereal *vt, integer &ldvt, doublereal *work, integer &lwork, integer &info )
	{
		return( dgesvd_( jobu, jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, &info ) );
	}
	inline int svd( char *jobu, char *jobvt, integer &m, integer &n, std::complex< real > *a, integer &lda, real *s, std::complex< real > *u,
								integer &ldu, std::complex< real > *vt, integer &ldvt, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( cgesvd_( jobu, jobvt, &m, &n, reinterpret_cast< complex* >( a ), &lda, s, reinterpret_cast< complex* >( u ),
								&ldu, reinterpret_cast< complex* >( vt ), &ldvt, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int svd( char *jobu, char *jobvt, integer &m, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *s, std::complex< doublereal > *u,
								integer &ldu, std::complex< doublereal > *vt, integer &ldvt, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( zgesvd_( jobu, jobvt, &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, s, reinterpret_cast< doublecomplex* >( u ),
								&ldu, reinterpret_cast< doublecomplex* >( vt ), &ldvt, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}

	inline int svd( char *jobz, integer &m, integer &n, real *a, integer &lda, real *s, real *u, integer &ldu,
								real *vt, integer &ldvt, real *work, integer &lwork, integer *iwork, integer &info )
	{
		return( sgesdd_( jobz, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, iwork, &info ) );
	}
	inline int svd( char *jobz, integer &m, integer &n, doublereal *a, integer &lda, doublereal *s, doublereal *u, integer &ldu,
								doublereal *vt, integer &ldvt, doublereal *work, integer &lwork, integer *iwork, integer &info )
	{
		return( dgesdd_( jobz, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, iwork, &info ) );
	}
	inline int svd( char *jobz, integer &m, integer &n, std::complex< real > *a, integer &lda, real *s, std::complex< real > *u, integer &ldu,
								std::complex< real > *vt, integer &ldvt, std::complex< real > *work, integer &lwork, real *rwork, integer *iwork, integer &info )
	{
		return( cgesdd_( jobz, &m, &n, reinterpret_cast< complex* >( a ), &lda, s, reinterpret_cast< complex* >( u ), &ldu,
							reinterpret_cast< complex* >( vt ), &ldvt, reinterpret_cast< complex* >( work ), &lwork, rwork, iwork, &info ) );
	}
	inline int svd( char *jobz, integer &m, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *s, std::complex< doublereal > *u, integer &ldu,
								std::complex< doublereal > *vt, integer &ldvt, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer *iwork, integer &info )
	{
		return( zgesdd_( jobz, &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, s, reinterpret_cast< doublecomplex* >( u ), &ldu,
							reinterpret_cast< doublecomplex* >( vt ), &ldvt, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, iwork, &info ) );
	}
}

// ��ʍs��̘A���ꎟ�������������֐�
template < class T, class Allocator >
matrix< T, Allocator >& solve( matrix< T, Allocator > &a, matrix< T, Allocator > &b )
{
	typedef __clapack__::integer integer;

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
	__clapack__::solve( n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, info );
	delete [] ipiv;
	return( b );
}


// ��ʍs���LU�������s��
template < class T, class Allocator1, class Allocator2 >
matrix< T, Allocator1 >& lu_factorization( matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot )
{
	typedef __clapack__::integer integer;

	// LAPACK�֐��̈���
	integer m      = static_cast< integer >( a.rows( ) );
	integer n      = static_cast< integer >( a.cols( ) );
	integer lda    = m;
	integer info   = 0;

	pivot.resize( n, 1 );

	// LU�������s��
	__clapack__::lu_factorization( m, n, &( a[0] ), lda, &( pivot[0] ), info );

	return( a );
}

// ��ʍs���LU�������s��
template < class T, class Allocator >
matrix< T, Allocator >& lu_factorization( matrix< T, Allocator > &a )
{
	typedef __clapack__::integer integer;
	matrix< __clapack__::integer > pivot( a.cols( ), 1 );
	return( lu_factorization( a, pivot ) );
}



// ��ʍs���QR�������s��
template < class T, class Allocator >
matrix< T, Allocator >& qr_factorization( matrix< T, Allocator > &a, matrix< T, Allocator > &tau )
{
	typedef __clapack__::integer integer;

	tau.resize( a.rows( ), a.cols( ) );

	// LAPACK�֐��̈���
	integer m      = static_cast< integer >( a.rows( ) );
	integer n      = static_cast< integer >( a.cols( ) );
	integer lda    = m;
	typename matrix< T, Allocator >::value_type dmy;
	integer lwork  = -1;
	integer info   = 0;

	// QR�������s���O�ɁC�K�v�ȍ�Ɨp�z��̃T�C�Y���擾����
	__clapack__::qr_factorization( m, n, NULL, lda, NULL, &dmy, lwork, info );
	if( info == 0 )
	{
		lwork = __clapack__::get_real( dmy );
		matrix< T, Allocator > work( lwork, 1 );
		__clapack__::qr_factorization( m, n, &( a[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );
	}

	return( a );
}

// ��ʍs���LU�������s��
template < class T, class Allocator >
matrix< T, Allocator >& qr_factorization( matrix< T, Allocator > &a )
{
	typedef __clapack__::integer integer;
	matrix< T, Allocator > tau( a.rows( ), a.cols( ) );
	return( qr_factorization( a, tau ) );
}


// ��ʍs��̋t�s���LU������p���Čv�Z����
template < class T, class Allocator >
matrix< T, Allocator >& inverse( matrix< T, Allocator > &a )
{
	typedef __clapack__::integer integer;

	// LAPACK�֐��̈���
	integer n      = static_cast< integer >( a.cols( ) );
	integer lda    = static_cast< integer >( a.rows( ) );
	integer *ipiv  = new integer[ n ];
	typename matrix< T, Allocator >::value_type dmy;
	integer lwork  = -1;
	integer info   = 0;

	// LU�������s��
	__clapack__::lu_factorization( lda, n, &( a[0] ), lda, ipiv, info );
	if( info == 0 )
	{
		// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
		__clapack__::inverse( n, NULL, lda, NULL, &dmy, lwork, info );
		if( info == 0 )
		{
			lwork = __clapack__::get_real( dmy );
			matrix< T, Allocator > work( lwork, 1 );
			__clapack__::inverse( n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );
		}
	}
	delete [] ipiv;
	return( a );
}


namespace __eigen__
{
	// ��ʍs��̌ŗL�l�E�ŗL�x�N�g�����v�Z����
	template < bool b >
	struct __eigen__
	{
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector )
		{
			typedef __clapack__::integer integer;

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
			__clapack__::eigen( jobvl, jobvr, n, NULL, lda, NULL, NULL, NULL, ldvl, NULL, ldvr, &dmy, lwork, info );
			if( info == 0 )
			{
				eigen_value.resize( n, 1 );
				matrix< T, Allocator > tmp( n, 1 );
				eigen_vector.resize( n, n );

				lwork = __clapack__::get_real( dmy );
				matrix< T, Allocator > work( lwork, 1 );
				__clapack__::eigen( jobvl, jobvr, n, &( a[0] ), lda, &( eigen_value[0] ), &( tmp[0] ),
					NULL, ldvl, &( eigen_vector[0] ), ldvr, &( work[0] ), lwork, info );
			}

			return( eigen_value );
		}
	};

	template < >
	struct __eigen__< true >
	{
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

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
			__clapack__::eigen( jobvl, jobvr, n, NULL, lda, NULL, NULL, ldvl, NULL, ldvr, &dmy, lwork, NULL, info );
			if( info == 0 )
			{
				eigen_value.resize( n, 1 );
				eigen_vector.resize( n, n );
				value_type *rwork = new value_type[ 2 * n ];

				lwork = __clapack__::get_real( dmy );
				matrix< T, Allocator > work( lwork, 1 );
				__clapack__::eigen( jobvl, jobvr, n, &( a[0] ), lda, &( eigen_value[0] ),
										NULL, ldvl, &( eigen_vector[0] ), ldvr, &( work[0] ), lwork, rwork, info );

				delete [] rwork;
			}

			return( eigen_value );
		}
	};
}


// ��ʍs��̌ŗL�l�E�ŗL�x�N�g�����v�Z����
template < class T, class Allocator >
matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector )
{
	return( __eigen__::__eigen__< __matrix_utility__::is_complex< T >::value >::eigen( a, eigen_value, eigen_vector ) );
}


namespace __svd__
{
#if _USE_DIVIDE_AND_CONQUER_SVD_ == 0 // ���������@��p���Ȃ��o�[�W�����̓��ْl����

	// ��ʍs��̓��ْl�������v�Z����
	template < bool b >
	struct __svd__
	{
		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &u, matrix< T, Allocator > &s, matrix< T, Allocator > &vt )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

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
			__clapack__::svd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, info );
			if( info == 0 )
			{
				u.resize( ldu, m );
				matrix< T, Allocator > ss( m < n ? m : n, 1 );
				vt.resize( ldvt, n );

				lwork = __clapack__::get_real( dmy );
				matrix< T, Allocator > work( lwork, 1 );
				__clapack__::svd( jobu, jobvt, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, info );

				s.resize( m, n );
				for( size_type i = 0 ; i < ss.rows( ) ; i++ )
				{
					s( i, i ) = ss[ i ];
				}
			}

			return( s );
		}
	};

	template < >
	struct __svd__< true >
	{
		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename T1::value_type value_type;

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
			__clapack__::svd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
			if( info == 0 )
			{
				u.resize( ldu, m );
				matrix< typename T1::value_type > ss( m < n ? m : n, 1 );
				vt.resize( ldvt, n );
				value_type *rwork = new value_type[ 5 * ( m < n ? m : n ) ];

				lwork = __clapack__::get_real( dmy );
				matrix< T1, Allocator1 > work( lwork, 1 );
				__clapack__::svd( jobu, jobvt, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, rwork, info );

				s.resize( m, n );
				for( size_type i = 0 ; i < ss.rows( ) ; i++ )
				{
					s( i, i ) = ss[ i ];
				}

				delete [] rwork;
			}

			return( s );
		}
	};

#else

	template < bool b >
	struct __svd__
	{
		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &u, matrix< T, Allocator > &s, matrix< T, Allocator > &vt )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

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
			__clapack__::svd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
			if( info == 0 )
			{
				u.resize( ldu, m );
				matrix< T, Allocator > ss( size, 1 );
				vt.resize( ldvt, n );
				integer *iwork = new integer[ 8 * size ];

				lwork = __clapack__::get_real( dmy );
				matrix< T, Allocator > work( lwork, 1 );
				__clapack__::svd( jobz, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, iwork, info );

				delete [] iwork;

				s.resize( m, n );
				for( size_type i = 0 ; i < ss.rows( ) ; i++ )
				{
					s( i, i ) = ss[ i ];
				}
			}

			return( s );
		}
	};

	template < >
	struct __svd__< true >
	{
		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename T1::value_type value_type;

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
			__clapack__::svd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, NULL, info );
			if( info == 0 )
			{
				u.resize( ldu, m );
				matrix< typename T1::value_type > ss( size, 1 );
				vt.resize( ldvt, n );
				value_type *rwork = new value_type[ 5 * size * size + 5 * size ];
				integer *iwork = new integer[ 8 * size ];

				lwork = __clapack__::get_real( dmy );
				matrix< T1, Allocator1 > work( lwork, 1 );
				__clapack__::svd( jobz, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, rwork, iwork, info );

				delete [] rwork;
				delete [] iwork;

				s.resize( m, n );
				for( size_type i = 0 ; i < ss.rows( ) ; i++ )
				{
					s( i, i ) = ss[ i ];
				}
			}

			return( s );
		}
	};

#endif

}

// ��ʍs��̓��ْl�������v�Z����
template < class T1, class T2, class Allocator1, class Allocator2 >
matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt )
{
	return( __svd__::__svd__< __matrix_utility__::is_complex< T1 >::value >::svd( a, u, s, vt ) );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MATRIX_UTIL__
