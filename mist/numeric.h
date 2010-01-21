// 
// Copyright (c) 2003-2010, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/// @file mist/numeric.h
//!
//! @brief Lapack��p�����s�񉉎Z���C�u����
//!
#ifndef __INCLUDE_NUMERIC__
#define __INCLUDE_NUMERIC__


#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

// �s��N���X���C���N���[�h����Ă��Ȃ��ꍇ�̓C���|�[�g����D
#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif

#include <vector>
#include <algorithm>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @addtogroup numeric_group �s�񉉎Z
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/numeric.h>
//! @endcode
//!
//!  @{


/// @brief �s��v�Z������ۂ̓��͂ƂȂ�s��̌`��
//! 
//! @attention ���̌`���ɏ]���āC�����ŌĂяo���֐����ύX�����
//! 
struct matrix_style
{
	/// @brief �s��v�Z������ۂ̓��͂ƂȂ�s��̌`��
	enum style
	{
		ge,		///< ��ʍs��
		gb,		///< ��ʑэs��
		gt,		///< ���3�d�Ίp�s��
		sy,		///< �Ώ̍s��
		sb,		///< �Ώ̑эs��
		st,		///< �Ώ�3�d�Ίp�s��
		he,		///< �G���~�[�g�s��
		hb,		///< �G���~�[�g�эs��
		ht,		///< �G���~�[�g3�d�Ίp�s��
	};
};

/// @}
//  �s�񉉎Z�O���[�v�̏I���


struct numerical_exception
{
	int error;
	std::string message;

	numerical_exception( ) : error( 0 ), message( )
	{
	}

	numerical_exception( int err, const std::string &msg = "" ) : error( err ), message( msg )
	{
	}

	numerical_exception( const std::string &msg ) : error( 0 ), message( msg )
	{
	}

	numerical_exception( const numerical_exception &e ) : error( e.error ), message( e.message )
	{
	}
};


namespace __clapack__
{

// �C���e����MKL�Ƃ̌݊�����ۂ��߂́C�֐����̕ϊ��}�N��
#if defined(_USE_INTEL_MATH_KERNEL_LIBRARY_) && _USE_INTEL_MATH_KERNEL_LIBRARY_ != 0
	#define LPFNAME( name ) name	// LAPACK�p
	#define BLFNAME( name ) name	// BLAS�p
#else
	#define LPFNAME( name ) name ## _	// LAPACK�p
	#define BLFNAME( name ) f2c_ ## name	// BLAS�p
#endif


	extern "C"
	{
		typedef long int integer;
		typedef float real;
		typedef double doublereal;
		struct complex { real r, i; };
		struct doublecomplex { doublereal r, i; };
		//typedef struct { real r, i; } complex;
		//typedef struct { doublereal r, i; } doublecomplex;

		// ��ʍs�񓯎m�̊|���Z���s��
		int BLFNAME( sgemm ) ( char *transa, char *transb, integer *m, integer *n, integer *k, real *alpha, real *a, integer *lda, real *b, integer *ldb, real *beta, real *c__, integer *ldc );
		int BLFNAME( dgemm ) ( char *transa, char *transb, integer *m, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc );
		int BLFNAME( cgemm ) ( char *transa, char *transb, integer *m, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc );
		int BLFNAME( zgemm ) ( char *transa, char *transb, integer *m, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc );

		// ��ʍs��ƃx�N�g���̊|���Z���s��
		int BLFNAME( sgemv ) ( char *trans, integer *m, integer *n, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy );
		int BLFNAME( dgemv ) ( char *trans, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy );
		int BLFNAME( cgemv ) ( char *trans, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy );
		int BLFNAME( zgemv ) ( char *trans, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy );

		// ��ʐ����s��̘A���������������֐�
		int LPFNAME( sgesv ) ( integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info );
		int LPFNAME( dgesv ) ( integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info );
		int LPFNAME( cgesv ) ( integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info );
		int LPFNAME( zgesv ) ( integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info );
		// ��ʑэs��̘A���������������֐�
		int LPFNAME( sgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, integer *ipiv, real *b, integer *ldb, integer *info );
		int LPFNAME( dgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, integer *ipiv, doublereal *b, integer *ldb, integer *info );
		int LPFNAME( cgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, integer *ipiv, complex *b, integer *ldb, integer *info );
		int LPFNAME( zgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, integer *ipiv, doublecomplex *b, integer *ldb, integer *info );
		// ��ʎO�d�Ίp�s��̘A���������������֐�
		int LPFNAME( sgtsv ) ( integer *n, integer *nrhs, real *dl, real *d__, real *du, real *b, integer *ldb, integer *info );
		int LPFNAME( dgtsv ) ( integer *n, integer *nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *b, integer *ldb, integer *info );
		int LPFNAME( cgtsv ) ( integer *n, integer *nrhs, complex *dl, complex *d__, complex *du, complex *b, integer *ldb, integer *info );
		int LPFNAME( zgtsv ) ( integer *n, integer *nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *b, integer *ldb, integer *info );
		// �Ώ̐����s��̘A���������������֐�
		int LPFNAME( ssysv ) ( char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info );
		int LPFNAME( dsysv ) ( char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( csysv ) ( char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info );
		int LPFNAME( zsysv ) ( char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info );
		// �G���~�[�g�s��̘A���������������֐�
		int LPFNAME( chesv ) ( char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info );
		int LPFNAME( zhesv ) ( char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info );


		// ��ʍs���LU����
		int LPFNAME( sgetrf ) ( integer *m, integer *n, real *a, integer *lda, integer *ipiv, integer *info );
		int LPFNAME( dgetrf ) ( integer *m, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info );
		int LPFNAME( cgetrf ) ( integer *m, integer *n, complex *a, integer *lda, integer *ipiv, integer *info );
		int LPFNAME( zgetrf ) ( integer *m, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info );
		// ��ʑэs��̘A���������������֐�
		int LPFNAME( sgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, integer *ipiv, integer *info );
		int LPFNAME( dgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, integer *ipiv, integer *info );
		int LPFNAME( cgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, integer *ipiv, integer *info );
		int LPFNAME( zgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, integer *ipiv, integer *info );
		// ��ʎO�d�Ίp�s��̘A���������������֐�
		int LPFNAME( sgttrf ) ( integer *n, real *dl, real *d__, real *du, real *du2, integer *ipiv, integer *info );
		int LPFNAME( dgttrf ) ( integer *n, doublereal *dl, doublereal *d__, doublereal *du, doublereal *du2, integer *ipiv, integer *info );
		int LPFNAME( cgttrf ) ( integer *n, complex *dl, complex *d__, complex *du, complex *du2, integer *ipiv, integer *info );
		int LPFNAME( zgttrf ) ( integer *n, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *du2, integer *ipiv, integer *info );
		// �Ώ̍s���LU����
		int LPFNAME( ssytrf ) ( char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info );
		int LPFNAME( dsytrf ) ( char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( csytrf ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int LPFNAME( zsytrf ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );
		// �G���~�[�g�s���LU����
		int LPFNAME( chetrf ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int LPFNAME( zhetrf ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );

		// ���Ώ̍s��̃R���X�L�[����
		int LPFNAME( spotrf ) ( char *uplo, integer *n, real *a, integer *lda, integer *info );
		int LPFNAME( dpotrf ) ( char *uplo, integer *n, doublereal *a, integer *lda, integer *info );
		int LPFNAME( cpotrf ) ( char *uplo, integer *n, complex *a, integer *lda, integer *info );
		int LPFNAME( zpotrf ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *info );

		// ��ʍs���QR����
		int LPFNAME( sgeqrf ) ( integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info );
		int LPFNAME( dgeqrf ) ( integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgeqrf ) ( integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info );
		int LPFNAME( zgeqrf ) ( integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info );
		// ��ʍs���QR����(Level3 Blas)
		int LPFNAME( sgeqp3 ) ( integer *m, integer *n, real *a, integer *lda, integer *jpvt, real *tau, real *work, integer *lwork, integer *info );
		int LPFNAME( dgeqp3 ) ( integer *m, integer *n, doublereal *a, integer *lda, integer *jpvt, doublereal *tau, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgeqp3 ) ( integer *m, integer *n, complex *a, integer *lda, integer *jpvt, complex *tau, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zgeqp3 ) ( integer *m, integer *n, doublecomplex *a, integer *lda, integer *jpvt, doublecomplex *tau, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// ��ʍs���QR�������Q�s����쐬���郋�[�`��
		int LPFNAME( sorgqr ) ( integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info );
		int LPFNAME( dorgqr ) ( integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cungqr ) ( integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info );
		int LPFNAME( zungqr ) ( integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info );

		// LU�����̌��ʂ�p������ʍs��̋t�s��̌v�Z
		int LPFNAME( sgetri ) ( integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info );
		int LPFNAME( dgetri ) ( integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgetri ) ( integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int LPFNAME( zgetri ) ( integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );
		// LU�����̌��ʂ�p�����Ώ̍s��̋t�s��̌v�Z
		int LPFNAME( ssytri ) ( char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *info );
		int LPFNAME( dsytri ) ( char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *info );
		int LPFNAME( csytri ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info );
		int LPFNAME( zsytri ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info );
		// LU�����̌��ʂ�p�����G���~�[�g�s��̋t�s��̌v�Z
		int LPFNAME( chetri ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info );
		int LPFNAME( zhetri ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info );


		// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int LPFNAME( sgeev ) ( char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info );
		int LPFNAME( dgeev ) ( char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgeev ) ( char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zgeev ) ( char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int LPFNAME( ssyev ) ( char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *info );
		int LPFNAME( dsyev ) ( char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *info );
		// �Ώ̑эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int LPFNAME( ssbev ) ( char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *info );
		int LPFNAME( dsbev ) ( char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *info );
		// �Ώ�3�d�Ίp�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int LPFNAME( sstev ) ( char *jobz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *info );
		int LPFNAME( dstev ) ( char *jobz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *info );
		// �G���~�[�g�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int LPFNAME( cheev ) ( char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zheev ) ( char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// �G���~�[�g�эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
		int LPFNAME( chbev ) ( char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *info );
		int LPFNAME( zhbev ) ( char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *info );


		// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
		int LPFNAME( sgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, real *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( dgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublereal *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( cgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
		int LPFNAME( ssyevx ) ( char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( dsyevx ) ( char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *ifail, integer *info );
		// �Ώ̑эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
		int LPFNAME( ssbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( dsbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info );
		// �Ώ�3�d�Ίp�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
		int LPFNAME( sstevx ) ( char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( dstevx ) ( char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info );
		// �G���~�[�g�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
		int LPFNAME( cheevx ) ( char *jobz, char *range, char *uplo, integer *n, complex *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( zheevx ) ( char *jobz, char *range, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *ifail, integer *info );
		// �G���~�[�g�эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
		int LPFNAME( chbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, complex *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( zhbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublecomplex *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *iwork, integer *ifail, integer *info );
		// �G���~�[�g3�d�Ίp�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����


		// ��ʍs��ɑ΂�����ْl�������v�Z
		int LPFNAME( sgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *info );
		int LPFNAME( dgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );


		// ��ʍs��ɑ΂�����ْl�������v�Z�D���������@��p�����������o�[�W����
		int LPFNAME( sgesdd ) ( char *jobz, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( dgesdd ) ( char *jobz, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( cgesdd ) ( char *jobz, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *iwork, integer *info );
		int LPFNAME( zgesdd ) ( char *jobz, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *info );



		// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
		int LPFNAME( ssyevd ) ( char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( dsyevd ) ( char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		// �Ώ̑эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
		int LPFNAME( ssbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( dsbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		// �Ώ�3�d�Ίp�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
		int LPFNAME( sstevd ) ( char *jobz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( dstevd ) ( char *jobz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		// �G���~�[�g�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
		int LPFNAME( cheevd ) ( char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( zheevd ) ( char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
		// �G���~�[�g�эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
		int LPFNAME( chbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( zhbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
	}


	// ���f���y�ю����̗�������ʂ��邱�ƂȂ��C�������̒l�����o���֐�
	inline real       get_real( const real &r ){ return( r ); }
	inline doublereal get_real( const doublereal &r ){ return( r ); }
	inline real       get_real( const std::complex< real > &r ){ return( r.real( ) ); }
	inline doublereal get_real( const std::complex< doublereal > &r ){ return( r.real( ) ); }

	// �ȍ~�CBLAS���[�`��

	// ��ʍs�񓯎m�̊|���Z���s��
	inline int gemm( char *transa, char *transb, integer &m, integer &n, integer &k, real &alpha, real *a, integer &lda, real *b, integer &ldb, real &beta, real *c__, integer &ldc )
	{
		return( BLFNAME( sgemm ) ( transa, transb, &m, &n, &k, &alpha, a, &lda, b, &ldb, &beta, c__, &ldc ) );
	}
	inline int gemm( char *transa, char *transb, integer &m, integer &n, integer &k, doublereal &alpha, doublereal *a, integer &lda, doublereal *b, integer &ldb, doublereal &beta, doublereal *c__, integer &ldc )
	{
		return( BLFNAME( dgemm ) ( transa, transb, &m, &n, &k, &alpha, a, &lda, b, &ldb, &beta, c__, &ldc ) );
	}
	inline int gemm( char *transa, char *transb, integer &m, integer &n, integer &k, std::complex< real > &alpha, std::complex< real > *a, integer &lda, std::complex< real > *b, integer &ldb, std::complex< real > &beta, std::complex< real > *c__, integer &ldc )
	{
		return( BLFNAME( cgemm ) ( transa, transb, &m, &n, &k, reinterpret_cast< complex* >( &alpha ), reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( b ), &ldb, reinterpret_cast< complex* >( &beta ), reinterpret_cast< complex* >( c__ ), &ldc ) );
	}
	inline int gemm( char *transa, char *transb, integer &m, integer &n, integer &k, std::complex< doublereal > &alpha, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *b, integer &ldb, std::complex< doublereal > &beta, std::complex< doublereal > *c__, integer &ldc )
	{
		return( BLFNAME( zgemm ) ( transa, transb, &m, &n, &k, reinterpret_cast< doublecomplex* >( &alpha ), reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( b ), &ldb, reinterpret_cast< doublecomplex* >( &beta ), reinterpret_cast< doublecomplex* >( c__ ), &ldc ) );
	}

	// ��ʍs��ƃx�N�g���̊|���Z���s��
	inline int gemv( char *trans, integer &m, integer &n, real &alpha, real *a, integer &lda, real *x, integer &incx, real &beta, real *y, integer &incy )
	{
		return( BLFNAME( sgemv ) ( trans, &m, &n, &alpha, a, &lda, x, &incx, &beta, y, &incy ) );
	}
	inline int gemv( char *trans, integer &m, integer &n, doublereal &alpha, doublereal *a, integer &lda, doublereal *x, integer &incx, doublereal &beta, doublereal *y, integer &incy )
	{
		return( BLFNAME( dgemv ) ( trans, &m, &n, &alpha, a, &lda, x, &incx, &beta, y, &incy ) );
	}
	inline int gemv( char *trans, integer &m, integer &n, std::complex< real > &alpha, std::complex< real > *a, integer &lda, std::complex< real > *x, integer &incx, std::complex< real > &beta, std::complex< real > *y, integer &incy )
	{
		return( BLFNAME( cgemv ) ( trans, &m, &n, reinterpret_cast< complex* >( &alpha ), reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( x ), &incx, reinterpret_cast< complex* >( &beta ), reinterpret_cast< complex* >( y ), &incy ) );
	}
	inline int gemv( char *trans, integer &m, integer &n, std::complex< doublereal > &alpha, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *x, integer &incx, std::complex< doublereal > &beta, std::complex< doublereal > *y, integer &incy )
	{
		return( BLFNAME( zgemv ) ( trans, &m, &n, reinterpret_cast< doublecomplex* >( &alpha ), reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( x ), &incx, reinterpret_cast< doublecomplex* >( &beta ), reinterpret_cast< doublecomplex* >( y ), &incy ) );
	}


	// �ȍ~�CLAPACK���[�`��

	// ��ʐ����s��̘A���������������֐�
	inline int gesv( integer &n, integer &nrhs, real *a, integer &lda, integer *ipiv, real *b, integer &ldb, integer &info )
	{
		return( LPFNAME( sgesv ) ( &n, &nrhs, a, &lda, ipiv, b, &ldb, &info ) );
	}
	inline int gesv( integer &n, integer &nrhs, doublereal *a, integer &lda, integer *ipiv, doublereal *b, integer &ldb, integer &info )
	{
		return( LPFNAME( dgesv ) ( &n, &nrhs, a, &lda, ipiv, b, &ldb, &info ) );
	}
	inline int gesv( integer &n, integer &nrhs, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *b, integer &ldb, integer &info )
	{
		return( LPFNAME( cgesv ) ( &n, &nrhs, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( b ), &ldb, &info ) );
	}
	inline int gesv( integer &n, integer &nrhs, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *b, integer &ldb, integer &info )
	{
		return( LPFNAME( zgesv ) ( &n, &nrhs, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, &info ) );
	}
	// ��ʑэs��̘A���������������֐�
	inline int gbsv( integer &n, integer &kl, integer &ku, integer &nrhs, real *ab, integer &ldab, integer *ipiv, real *b, integer &ldb, integer &info )
	{
		return( LPFNAME( sgbsv ) ( &n, &kl, &ku, &nrhs, ab, &ldab, ipiv, b, &ldb, &info ) );
	}
	inline int gbsv( integer &n, integer &kl, integer &ku, integer &nrhs, doublereal *ab, integer &ldab, integer *ipiv, doublereal *b, integer &ldb, integer &info )
	{
		return( LPFNAME( dgbsv ) ( &n, &kl, &ku, &nrhs, ab, &ldab, ipiv, b, &ldb, &info ) );
	}
	inline int gbsv( integer &n, integer &kl, integer &ku, integer &nrhs, complex *ab, integer &ldab, integer *ipiv, complex *b, integer &ldb, integer &info )
	{
		return( LPFNAME( cgbsv ) ( &n, &kl, &ku, &nrhs, reinterpret_cast< complex* >( ab ), &ldab, ipiv, reinterpret_cast< complex* >( b ), &ldb, &info ) );
	}
	inline int gbsv( integer &n, integer &kl, integer &ku, integer &nrhs, doublecomplex *ab, integer &ldab, integer *ipiv, doublecomplex *b, integer &ldb, integer &info )
	{
		return( LPFNAME( zgbsv ) ( &n, &kl, &ku, &nrhs, reinterpret_cast< doublecomplex* >( ab ), &ldab, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, &info ) );
	}
	// ��ʎO�d�Ίp�s��̘A���������������֐�
	inline int gtsv( integer &n, integer &nrhs, real *dl, real *d__, real *du, real *b, integer &ldb, integer &info )
	{
		return( LPFNAME( sgtsv ) ( &n, &nrhs, dl, d__, du, b, &ldb, &info ) );
	}
	inline int gtsv( integer &n, integer &nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *b, integer &ldb, integer &info )
	{
		return( LPFNAME( dgtsv ) ( &n, &nrhs, dl, d__, du, b, &ldb, &info ) );
	}
	inline int gtsv( integer &n, integer &nrhs, complex *dl, complex *d__, complex *du, complex *b, integer &ldb, integer &info )
	{
		return( LPFNAME( cgtsv ) ( &n, &nrhs, reinterpret_cast< complex* >( dl ), reinterpret_cast< complex* >( d__ ), reinterpret_cast< complex* >( du ), reinterpret_cast< complex* >( b ), &ldb, &info ) );
	}
	inline int gtsv( integer &n, integer &nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *b, integer &ldb, integer &info )
	{
		return( LPFNAME( zgtsv ) ( &n, &nrhs, reinterpret_cast< doublecomplex* >( dl ), reinterpret_cast< doublecomplex* >( d__ ), reinterpret_cast< doublecomplex* >( du ), reinterpret_cast< doublecomplex* >( b ), &ldb, &info ) );
	}
	// �Ώ̐����s��̘A���������������֐�
	inline int sysv( char *uplo, integer &n, integer &nrhs, real *a, integer &lda, integer *ipiv, real *b, integer &ldb, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( ssysv ) ( uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, work, &lwork, &info ) );
	}
	inline int sysv( char *uplo, integer &n, integer &nrhs, doublereal *a, integer &lda, integer *ipiv, doublereal *b, integer &ldb, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dsysv ) ( uplo, &n, &nrhs, a, &lda, ipiv, b, &ldb, work, &lwork, &info ) );
	}
	inline int sysv( char *uplo, integer &n, integer &nrhs, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *b, integer &ldb, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( csysv ) ( uplo, &n, &nrhs, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( b ), &ldb, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int sysv( char *uplo, integer &n, integer &nrhs, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *b, integer &ldb, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zsysv ) ( uplo, &n, &nrhs, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}
	// �G���~�[�g�s��̘A���������������֐�
	inline int hesv( char *uplo, integer &n, integer &nrhs, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *b, integer &ldb, std::complex< real > *work, integer &lwork, integer&info )
	{
		return( LPFNAME( chesv ) ( uplo, &n, &nrhs, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( b ), &ldb, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int hesv( char *uplo, integer &n, integer &nrhs, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *b, integer &ldb, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zsysv ) ( uplo, &n, &nrhs, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// ��ʍs���LU����
	inline int getrf( integer &m, integer &n, real *a, integer &lda, integer *ipiv, integer &info )
	{
		return( LPFNAME( sgetrf ) ( &m, &n, a, &lda, ipiv, &info ) );
	}
	inline int getrf( integer &m, integer &n, doublereal *a, integer &lda, integer *ipiv, integer &info )
	{
		return( LPFNAME( dgetrf ) ( &m, &n, a, &lda, ipiv, &info ) );
	}
	inline int getrf( integer &m, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, integer &info )
	{
		return( LPFNAME( cgetrf ) ( &m, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, &info ) );
	}
	inline int getrf( integer &m, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, integer &info )
	{
		return( LPFNAME( zgetrf ) ( &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, &info ) );
	}
	// ��ʑэs��̘A���������������֐�
	inline int gbtrf( integer &m, integer &n, integer &kl, integer &ku, real *ab, integer &ldab, integer *ipiv, integer &info )
	{
		return( LPFNAME( sgbtrf ) ( &m, &n, &kl, &ku, ab, &ldab, ipiv, &info ) );
	}
	inline int gbtrf( integer &m, integer &n, integer &kl, integer &ku, doublereal *ab, integer &ldab, integer *ipiv, integer &info )
	{
		return( LPFNAME( dgbtrf ) ( &m, &n, &kl, &ku, ab, &ldab, ipiv, &info ) );
	}
	inline int gbtrf( integer &m, integer &n, integer &kl, integer &ku, std::complex< real > *ab, integer &ldab, integer *ipiv, integer &info )
	{
		return( LPFNAME( cgbtrf ) ( &m, &n, &kl, &ku, reinterpret_cast< complex* >( ab ), &ldab, ipiv, &info ) );
	}
	inline int gbtrf( integer &m, integer &n, integer &kl, integer &ku, std::complex< doublereal > *ab, integer &ldab, integer *ipiv, integer &info )
	{
		return( LPFNAME( zgbtrf ) ( &m, &n, &kl, &ku, reinterpret_cast< doublecomplex* >( ab ), &ldab, ipiv, &info ) );
	}
	// ��ʎO�d�Ίp�s��̘A���������������֐�
	inline int gttrf( integer &n, real *dl, real *d__, real *du, real *du2, integer *ipiv, integer &info )
	{
		return( LPFNAME( sgttrf ) ( &n, dl, d__, du, du2, ipiv, &info ) );
	}
	inline int gttrf( integer &n, doublereal *dl, doublereal *d__, doublereal *du, doublereal *du2, integer *ipiv, integer &info )
	{
		return( LPFNAME( dgttrf ) ( &n, dl, d__, du, du2, ipiv, &info ) );
	}
	inline int gttrf( integer &n, std::complex< real > *dl, std::complex< real > *d__, std::complex< real > *du, std::complex< real > *du2, integer *ipiv, integer &info )
	{
		return( LPFNAME( cgttrf ) ( &n, reinterpret_cast< complex* >( dl ), reinterpret_cast< complex* >( d__ ), reinterpret_cast< complex* >( du ), reinterpret_cast< complex* >( du2 ), ipiv, &info ) );
	}
	inline int gttrf( integer &n, std::complex< doublereal > *dl, std::complex< doublereal > *d__, std::complex< doublereal > *du, std::complex< doublereal > *du2, integer *ipiv, integer &info )
	{
		return( LPFNAME( zgttrf ) ( &n, reinterpret_cast< doublecomplex* >( dl ), reinterpret_cast< doublecomplex* >( d__ ), reinterpret_cast< doublecomplex* >( du ), reinterpret_cast< doublecomplex* >( du2 ), ipiv, &info ) );
	}
	// �Ώ̍s���LU����
	inline int sytrf( char *uplo, integer &n, real *a, integer &lda, integer *ipiv, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( ssytrf ) ( uplo, &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int sytrf( char *uplo, integer &n, doublereal *a, integer &lda, integer *ipiv, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dsytrf ) ( uplo, &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int sytrf( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( csytrf ) ( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int sytrf( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zsytrf ) ( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}
	// �G���~�[�g�s���LU����
	inline int hetrf( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( chetrf ) ( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int hetrf( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zhetrf ) ( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}

	// �Ώ̍s��̃R���X�L�[����
	inline int potrf( char *uplo, integer &n, real *a, integer &lda, integer &info )
	{
		return( LPFNAME( spotrf ) ( uplo, &n, a, &lda, &info ) );
	}
	inline int potrf( char *uplo, integer &n, doublereal *a, integer &lda, integer &info )
	{
		return( LPFNAME( dpotrf ) ( uplo, &n, a, &lda, &info ) );
	}
	inline int potrf( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer &info )
	{
		return( LPFNAME( cpotrf ) ( uplo, &n, reinterpret_cast< complex* >( a ), &lda, &info ) );
	}
	inline int potrf( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer &info )
	{
		return( LPFNAME( zpotrf ) ( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, &info ) );
	}

	// QR����
	inline int geqrf( integer &m, integer &n, real *a, integer &lda, real *tau, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( sgeqrf ) ( &m, &n, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int geqrf( integer &m, integer &n, doublereal *a, integer &lda, doublereal *tau, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dgeqrf ) ( &m, &n, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int geqrf( integer &m, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *tau, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( cgeqrf ) ( &m, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( tau ), reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int geqrf( integer &m, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *tau, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zgeqrf ) ( &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( tau ), reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}

	// QR����(Level3 Blas)
	inline int geqp3( integer &m, integer &n, real *a, integer &lda, integer *jpvt, real *tau, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( sgeqp3 ) ( &m, &n, a, &lda, jpvt, tau, work, &lwork, &info ) );
	}
	inline int geqp3( integer &m, integer &n, doublereal *a, integer &lda, integer *jpvt, doublereal *tau, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dgeqp3 ) ( &m, &n, a, &lda, jpvt, tau, work, &lwork, &info ) );
	}
	inline int geqp3( integer &m, integer &n, std::complex< real > *a, integer &lda, integer *jpvt, std::complex< real > *tau, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( LPFNAME( cgeqp3 ) ( &m, &n, reinterpret_cast< complex* >( a ), &lda, jpvt, reinterpret_cast< complex* >( tau ), reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int geqp3( integer &m, integer &n, std::complex< doublereal > *a, integer &lda, integer *jpvt, std::complex< doublereal > *tau, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zgeqp3 ) ( &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, jpvt, reinterpret_cast< doublecomplex* >( tau ), reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}

	// ��ʍs���QR�������Q�s����쐬���郋�[�`��
	inline int orgqr( integer &m, integer &n, integer &k, real *a, integer &lda, real *tau, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( sorgqr ) ( &m, &n, &k, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int orgqr( integer &m, integer &n, integer &k, doublereal *a, integer &lda, doublereal *tau, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dorgqr ) ( &m, &n, &k, a, &lda, tau, work, &lwork, &info ) );
	}
	inline int ungqr( integer &m, integer &n, integer &k, std::complex< real > *a, integer &lda, std::complex< real > *tau, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( cungqr ) ( &m, &n, &k, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( tau ), reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int ungqr( integer &m, integer &n, integer &k, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *tau, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zungqr ) ( &m, &n, &k, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( tau ), reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}

	// LU�����̌��ʂ�p������ʍs��̋t�s��̌v�Z
	inline int getri( integer &n, real *a, integer &lda, integer *ipiv, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( sgetri ) ( &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int getri( integer &n, doublereal *a, integer &lda, integer *ipiv, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dgetri ) ( &n, a, &lda, ipiv, work, &lwork, &info ) );
	}
	inline int getri( integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( cgetri ) ( &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int getri( integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zgetri ) ( &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}
	// LU�����̌��ʂ�p�����Ώ̍s��̋t�s��̌v�Z
	inline int sytri( char *uplo, integer &n, real *a, integer &lda, integer *ipiv, real *work, integer &info )
	{
		return( LPFNAME( ssytri ) ( uplo, &n, a, &lda, ipiv, work, &info ) );
	}
	inline int sytri( char *uplo, integer &n, doublereal *a, integer &lda, integer *ipiv, doublereal *work, integer &info )
	{
		return( LPFNAME( dsytri ) ( uplo, &n, a, &lda, ipiv, work, &info ) );
	}
	inline int sytri( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &info )
	{
		return( LPFNAME( csytri ) ( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &info ) );
	}
	inline int sytri( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &info )
	{
		return( LPFNAME( zsytri ) ( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &info ) );
	}
	// LU�����̌��ʂ�p�����G���~�[�g�s��̋t�s��̌v�Z
	inline int hetri( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &info )
	{
		return( LPFNAME( chetri ) ( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &info ) );
	}
	inline int hetri( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &info )
	{
		return( LPFNAME( zhetri ) ( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &info ) );
	}


	// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int geev( char *jobvl, char *jobvr, integer &n, real *a, integer &lda, real *wr, real *wi,
							real *vl, integer &ldvl, real *vr, integer &ldvr, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( sgeev ) ( jobvl, jobvr, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info ) );
	}
	inline int geev( char *jobvl, char *jobvr, integer &n, doublereal *a, integer &lda, doublereal *wr, doublereal *wi,
							doublereal *vl, integer &ldvl, doublereal *vr, integer &ldvr, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dgeev ) ( jobvl, jobvr, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, work, &lwork, &info ) );
	}
	inline int geev( char *jobvl, char *jobvr, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *w, std::complex< real > *vl, integer &ldvl,
										std::complex< real > *vr, integer &ldvr, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( LPFNAME( cgeev ) ( jobvl, jobvr, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( w ), reinterpret_cast< complex* >( vl ), &ldvl,
							reinterpret_cast< complex* >( vr ), &ldvr, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int geev( char *jobvl, char *jobvr, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *w, std::complex< doublereal > *vl, integer &ldvl,
										std::complex< doublereal > *vr, integer &ldvr, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zgeev ) ( jobvl, jobvr, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( w ), reinterpret_cast< doublecomplex* >( vl ), &ldvl,
							reinterpret_cast< doublecomplex* >( vr ), &ldvr, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}
	// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int syev( char *jobz, char *uplo, integer &n, real *a, integer &lda, real *w, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( ssyev ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, &info ) );
	}
	inline int syev( char *jobz, char *uplo, integer &n, doublereal *a, integer &lda, doublereal *w, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dsyev ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, &info ) );
	}
	// �Ώ̑эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int sbev( char *jobz, char *uplo, integer &n, integer &kd, real *ab, integer &ldab, real *w, real *z__, integer &ldz, real *work, integer &info )
	{
		return( LPFNAME( ssbev ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &info ) );
	}
	inline int sbev( char *jobz, char *uplo, integer &n, integer &kd, doublereal *ab, integer &ldab, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer &info )
	{
		return( LPFNAME( dsbev ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &info ) );
	}
	// �Ώ�3�d�Ίp�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int stev( char *jobz, integer &n, real *d__, real *e, real *z__, integer &ldz, real *work, integer &info )
	{
		return( LPFNAME( sstev ) ( jobz, &n, d__, e, z__, &ldz, work, &info ) );
	}
	inline int stev( char *jobz, integer &n, doublereal *d__, doublereal *e, doublereal *z__, integer &ldz, doublereal *work, integer &info )
	{
		return( LPFNAME( dstev ) ( jobz, &n, d__, e, z__, &ldz, work, &info ) );
	}
	// �G���~�[�g�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int heev( char *jobz, char *uplo, integer &n, std::complex< real > *a, integer &lda, real *w, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( LPFNAME( cheev ) ( jobz, uplo, &n, reinterpret_cast< complex* >( a ), &lda, w, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int heev( char *jobz, char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *w, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zheev ) ( jobz, uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, w, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}
	// �G���~�[�g�эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int hbev( char *jobz, char *uplo, integer &n, integer &kd, std::complex< real > *ab, integer &ldab, real *w, std::complex< real > *z__, integer &ldz, std::complex< real > *work, real *rwork, integer &info )
	{
		return( LPFNAME( chbev ) ( jobz, uplo, &n, &kd, reinterpret_cast< complex* >( ab ), &ldab, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), rwork, &info ) );
	}
	inline int hbev( char *jobz, char *uplo, integer &n, integer &kd, std::complex< doublereal > *ab, integer &ldab, doublereal *w, std::complex< doublereal > *z__, integer &ldz, std::complex< doublereal > *work, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zhbev ) ( jobz, uplo, &n, &kd, reinterpret_cast< doublecomplex* >( ab ), &ldab, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), rwork, &info ) );
	}



	// ��ʍs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, real *a, integer &lda, real *wr, real *wi,
										real *vl, integer &ldvl, real *vr, integer &ldvr, integer &ilo, integer &ihi, real *scale,
										real &abnrm, real *rconde, real *rcondv, real *work, integer &lwork, integer *iwork, integer &info )
	{
		return( LPFNAME( sgeevx ) ( balanc, jobvl, jobvr, sense, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, work, &lwork, iwork, &info ) );
	}
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, doublereal *a, integer &lda, doublereal *wr, doublereal *wi,
										doublereal *vl, integer &ldvl, doublereal *vr, integer &ldvr, integer &ilo, integer &ihi, doublereal *scale,
										doublereal &abnrm, doublereal *rconde, doublereal *rcondv, doublereal *work, integer &lwork, integer *iwork, integer &info )
	{
		return( LPFNAME( dgeevx ) ( balanc, jobvl, jobvr, sense, &n, a, &lda, wr, wi, vl, &ldvl, vr, &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, work, &lwork, iwork, &info ) );
	}
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, std::complex< real > *a, integer &lda, std::complex< real > *w,
										std::complex< real > *vl, integer &ldvl, std::complex< real > *vr, integer &ldvr, integer &ilo, integer &ihi, real *scale,
										real &abnrm, real *rconde, real *rcondv, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( LPFNAME( cgeevx ) ( balanc, jobvl, jobvr, sense, &n, reinterpret_cast< complex* >( a ), &lda, reinterpret_cast< complex* >( w ), reinterpret_cast< complex* >( vl ), &ldvl,
								reinterpret_cast< complex* >( vr ), &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int geevx( char *balanc, char *jobvl, char *jobvr, char *sense, integer &n, std::complex< doublereal > *a, integer &lda, std::complex< doublereal > *w,
										std::complex< doublereal > *vl, integer &ldvl, std::complex< doublereal > *vr, integer &ldvr, integer &ilo, integer &ihi, doublereal *scale,
										doublereal &abnrm, doublereal *rconde, doublereal *rcondv, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zgeevx ) ( balanc, jobvl, jobvr, sense, &n, reinterpret_cast< doublecomplex* >( a ), &lda, reinterpret_cast< doublecomplex* >( w ), reinterpret_cast< doublecomplex* >( vl ), &ldvl,
								reinterpret_cast< doublecomplex* >( vr ), &ldvr, &ilo, &ihi, scale, &abnrm, rconde, rcondv, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}
	// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z
	inline int syevx( char *jobz, char *range, char *uplo, integer &n, real *a, integer &lda,
						real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, real *z__,
						integer &ldz, real *work, integer &lwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( ssyevx ) ( jobz, range, uplo, &n, a, &lda, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, &lwork, iwork, ifail, &info ) );
	}
	inline int syevx( char *jobz, char *range, char *uplo, integer &n, doublereal *a, integer &lda,
						doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, doublereal *z__,
						integer &ldz, doublereal *work, integer &lwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( dsyevx ) ( jobz, range, uplo, &n, a, &lda, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, &lwork, iwork, ifail, &info ) );
	}
	// �Ώ̑эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
	inline int sbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, real *ab, integer &ldab, real *q, integer &ldq, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, real *z__, integer &ldz, real *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( ssbevx ) ( jobz, range, uplo, &n, &kd, ab, &ldab, q, &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	inline int dsbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, doublereal *ab, integer &ldab, doublereal *q, integer &ldq, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( dsbevx ) ( jobz, range, uplo, &n, &kd, ab, &ldab, q, &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	// �Ώ�3�d�Ίp�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
	inline int stevx( char *jobz, char *range, integer &n, real *d__, real *e, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, real *z__, integer &ldz, real *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( sstevx ) ( jobz, range, &n, d__, e, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	inline int stevx( char *jobz, char *range, integer &n, doublereal *d__, doublereal *e, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( dstevx ) ( jobz, range, &n, d__, e, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	// �G���~�[�g�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
	inline int heevx( char *jobz, char *range, char *uplo, integer &n, std::complex< real > *a, integer &lda, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, std::complex< real > *z__, integer &ldz, std::complex< real > *work, integer &lwork, real *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( cheevx ) ( jobz, range, uplo, &n, reinterpret_cast< complex* >( a ), &lda, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), &lwork, rwork, iwork, ifail, &info ) );
	}
	inline int heevx( char *jobz, char *range, char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, std::complex< doublereal > *z__, integer &ldz, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( zheevx ) ( jobz, range, uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, iwork, ifail, &info ) );
	}
	// �G���~�[�g�эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z�D�s���Ίp�����Đ��x���グ��o�[�W����
	inline int hbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, complex *ab, integer &ldab, complex *q, integer &ldq, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, complex *z__, integer &ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( chbevx ) ( jobz, range, uplo, &n, &kd, reinterpret_cast< complex* >( ab ), &ldab, reinterpret_cast< complex* >( q ), &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), rwork, iwork, ifail, &info ) );
	}
	inline int hbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, complex *ab, integer &ldab, complex *q, integer &ldq, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, complex *z__, integer &ldz, complex *work, doublereal *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( zhbevx ) ( jobz, range, uplo, &n, &kd, reinterpret_cast< doublecomplex* >( ab ), &ldab, reinterpret_cast< doublecomplex* >( q ), &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), rwork, iwork, ifail, &info ) );
	}




	// ��ʍs��ɑ΂�����ْl�������v�Z
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, real *a, integer &lda, real *s, real *u,
								integer &ldu, real *vt, integer &ldvt, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( sgesvd ) ( jobu, jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, &info ) );
	}
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, doublereal *a, integer &lda, doublereal *s, doublereal *u,
								integer &ldu, doublereal *vt, integer &ldvt, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dgesvd ) ( jobu, jobvt, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, &info ) );
	}
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, std::complex< real > *a, integer &lda, real *s, std::complex< real > *u,
								integer &ldu, std::complex< real > *vt, integer &ldvt, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( LPFNAME( cgesvd ) ( jobu, jobvt, &m, &n, reinterpret_cast< complex* >( a ), &lda, s, reinterpret_cast< complex* >( u ),
								&ldu, reinterpret_cast< complex* >( vt ), &ldvt, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int gesvd( char *jobu, char *jobvt, integer &m, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *s, std::complex< doublereal > *u,
								integer &ldu, std::complex< doublereal > *vt, integer &ldvt, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zgesvd ) ( jobu, jobvt, &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, s, reinterpret_cast< doublecomplex* >( u ),
								&ldu, reinterpret_cast< doublecomplex* >( vt ), &ldvt, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}

	// ��ʍs��ɑ΂�����ْl�������v�Z�D���������@��p���������o�[�W����
	inline int gesdd( char *jobz, integer &m, integer &n, real *a, integer &lda, real *s, real *u, integer &ldu,
								real *vt, integer &ldvt, real *work, integer &lwork, integer *iwork, integer &info )
	{
		return( LPFNAME( sgesdd ) ( jobz, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, iwork, &info ) );
	}
	inline int gesdd( char *jobz, integer &m, integer &n, doublereal *a, integer &lda, doublereal *s, doublereal *u, integer &ldu,
								doublereal *vt, integer &ldvt, doublereal *work, integer &lwork, integer *iwork, integer &info )
	{
		return( LPFNAME( dgesdd ) ( jobz, &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork, iwork, &info ) );
	}
	inline int gesdd( char *jobz, integer &m, integer &n, std::complex< real > *a, integer &lda, real *s, std::complex< real > *u, integer &ldu,
								std::complex< real > *vt, integer &ldvt, std::complex< real > *work, integer &lwork, real *rwork, integer *iwork, integer &info )
	{
		return( LPFNAME( cgesdd ) ( jobz, &m, &n, reinterpret_cast< complex* >( a ), &lda, s, reinterpret_cast< complex* >( u ), &ldu,
							reinterpret_cast< complex* >( vt ), &ldvt, reinterpret_cast< complex* >( work ), &lwork, rwork, iwork, &info ) );
	}
	inline int gesdd( char *jobz, integer &m, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *s, std::complex< doublereal > *u, integer &ldu,
								std::complex< doublereal > *vt, integer &ldvt, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer *iwork, integer &info )
	{
		return( LPFNAME( zgesdd ) ( jobz, &m, &n, reinterpret_cast< doublecomplex* >( a ), &lda, s, reinterpret_cast< doublecomplex* >( u ), &ldu,
							reinterpret_cast< doublecomplex* >( vt ), &ldvt, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, iwork, &info ) );
	}


	// �Ώ̍s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
	inline int syevd( char *jobz, char *uplo, integer &n, real *a, integer &lda, real *w, real *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( ssyevd ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, iwork, &liwork, &info ) );
	}
	inline int syevd( char *jobz, char *uplo, integer &n, doublereal *a, integer &lda, doublereal *w, doublereal *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( dsyevd ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, iwork, &liwork, &info ) );
	}
	// �Ώ̑эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
	inline int sbevd( char *jobz, char *uplo, integer &n, integer &kd, real *ab, integer &ldab, real *w, real *z__, integer &ldz, real *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( ssbevd ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	inline int sbevd( char *jobz, char *uplo, integer &n, integer &kd, doublereal *ab, integer &ldab, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( dsbevd ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	// �Ώ�3�d�Ίp�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
	inline int stevd( char *jobz, integer &n, real *d__, real *e, real *z__, integer &ldz, real *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( sstevd ) ( jobz, &n, d__, e, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	inline int stevd( char *jobz, integer &n, doublereal *d__, doublereal *e, doublereal *z__, integer &ldz, doublereal *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( dstevd ) ( jobz, &n, d__, e, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	// �G���~�[�g�s��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
	inline int heevd( char *jobz, char *uplo, integer &n, std::complex< real > *a, integer &lda, real *w, std::complex< real > *work, integer &lwork, real *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( cheevd ) ( jobz, uplo, &n, reinterpret_cast< complex* >( a ), &lda, w, reinterpret_cast< complex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}
	inline int heevd( char *jobz, char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *w, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( zheevd ) ( jobz, uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, w, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}
	// �G���~�[�g�эs��ɑ΂���ŗL�l�E�ŗL�x�N�g�����v�Z���v�Z�D���������@��p���������o�[�W����
	inline int hbevd( char *jobz, char *uplo, integer &n, integer &kd, std::complex< real > *ab, integer &ldab, real *w, std::complex< real > *z__, integer &ldz, std::complex< real > *work, integer &lwork, real *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( chbevd ) ( jobz, uplo, &n, &kd, reinterpret_cast< complex* >( ab ), &ldab, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}
	inline int hbevd( char *jobz, char *uplo, integer &n, integer &kd, std::complex< doublereal > *ab, integer &ldab, doublereal *w, std::complex< doublereal > *z__, integer &ldz, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( zhbevd ) ( jobz, uplo, &n, &kd, reinterpret_cast< doublecomplex* >( ab ), &ldab, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}


	// �C���e����MKL�Ƃ̌݊�����ۂ��߂́C�֐����̕ϊ��}�N��
	#undef LPFNAME


	template < class T >
	class __value_index_pair__
	{
	public:
		typedef size_t size_type;
		typedef T value_type;

		value_type value;
		size_type  index;

		__value_index_pair__( ){ }

		__value_index_pair__( value_type v, size_type indx ) : value( v ), index( indx ){ }

		static bool ascending( const __value_index_pair__ &a, const __value_index_pair__ &b )
		{
			return( a.value < b.value );
		}

		static bool descending( const __value_index_pair__ &a, const __value_index_pair__ &b )
		{
			return( a.value > b.value );
		}
	};

	template < class T >
	class __value_index_pair__< std::complex< T > >
	{
	public:
		typedef size_t size_type;
		typedef std::complex< T > value_type;

		value_type value;
		size_type  index;

		__value_index_pair__( ){ }

		__value_index_pair__( value_type v, size_type indx ) : value( v ), index( indx ){ }

		static bool ascending( const __value_index_pair__ &a, const __value_index_pair__ &b )
		{
			if( a.value.real( ) < b.value.real( ) )
			{
				return( true );
			}
			else if( a.value.real( ) > b.value.real( ) )
			{
				return( false );
			}
			else
			{
				return( a.value.imag( ) < b.value.imag( ) );
			}
		}

		static bool descending( const __value_index_pair__ &a, const __value_index_pair__ &b )
		{
			if( a.value.real( ) > b.value.real( ) )
			{
				return( true );
			}
			else if( a.value.real( ) < b.value.real( ) )
			{
				return( false );
			}
			else
			{
				return( a.value.imag( ) > b.value.imag( ) );
			}
		}
	};

	inline const std::string to_string( integer value )
	{
		char buff[ 100 ];
		sprintf( buff, "%ld", value );
		return( std::string( buff ) );
	}
}


namespace __solve__
{
	// �s��̘A���ꎟ�������������֐�
	template < bool >
	struct __solve__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& solve( matrix< T, Allocator > &a, matrix< T, Allocator > &b, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;

			if( a.rows( ) != b.rows( ) || a.empty( ) || b.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Incorrect matrix size is specified." ) );
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					value_type dmy;
					integer *ipiv = new integer[ n ];
					integer lwork = -1;
					char uplo[]   = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sysv( uplo, n, nrhs, NULL, lda, NULL, NULL, ldb, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sysv( uplo, n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, &( work[0] ), lwork, info );
					}
					delete [] ipiv;

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						std::string val = __clapack__::to_string( info );
						std::string msg = "D( " + val + ", " + val + " ) is exactly zero. The factorization has been completed, but the block diagonal matrix D is exactly singular, so the solution could not be computed.";
						throw( numerical_exception( info, msg ) );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					integer *ipiv = new integer[ n ];

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

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						std::string val = __clapack__::to_string( info );
						std::string msg = "U( " + val + ", " + val + " ) is exactly zero. The factorization has been completed, but the factor U is exactly singular, so the solution could not be computed";
						throw( numerical_exception( info, msg ) );
					}
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

			if( a.rows( ) != b.rows( ) || a.empty( ) || b.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Incorrect matrix size is specified." ) );
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					value_type dmy;
					integer *ipiv = new integer[ n ];
					integer lwork = -1;
					char uplo[]   = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sysv( uplo, n, nrhs, NULL, lda, NULL, NULL, ldb, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sysv( uplo, n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, &( work[0] ), lwork, info );
					}
					delete [] ipiv;

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						std::string val = __clapack__::to_string( info );
						std::string msg = "D( " + val + ", " + val + " ) is exactly zero. The factorization has been completed, but the block diagonal matrix D is exactly singular, so the solution could not be computed.";
						throw( numerical_exception( info, msg ) );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					integer *ipiv = new integer[ n ];

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

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						std::string val = __clapack__::to_string( info );
						std::string msg = "U( " + val + ", " + val + " ) is exactly zero. The factorization has been completed, but the factor U is exactly singular, so the solution could not be computed";
						throw( numerical_exception( info, msg ) );
					}
				}
				break;
			}

			return( b );
		}
	};
}




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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;

					pivot.resize( n, 1 );

					// LU�������s��
					__clapack__::getrf( m, n, &( a[0] ), lda, &( pivot[0] ), info );
				}
				break;
			}

			if( info < 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
			}
			else if( info > 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				std::string val = __clapack__::to_string( info );
				std::string msg = "U( " + val + ", " + val + " ) is exactly zero. The factorization has been completed, but the factor U is exactly singular, and division by zero will occur if it is used to solve a system of equations.";
				throw( numerical_exception( info, msg ) );
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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;

					pivot.resize( n, 1 );

					// LU�������s��
					__clapack__::getrf( m, n, &( a[0] ), lda, &( pivot[0] ), info );
				}
				break;
			}

			if( info < 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
			}
			else if( info > 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				std::string val = __clapack__::to_string( info );
				std::string msg = "U( " + val + ", " + val + " ) is exactly zero. The factorization has been completed, but the factor U is exactly singular, and division by zero will occur if it is used to solve a system of equations.";
				throw( numerical_exception( info, msg ) );
			}

			return( a );
		}
	};
}




namespace __qr__
{
	// �s��̘A���ꎟ�������������֐�
	template < bool b >
	struct __qr__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static void qr_factorization( matrix< T, Allocator > &A, matrix< T, Allocator > &Q, matrix< T, Allocator > &R, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;
			typedef typename matrix< T, Allocator >::size_type size_type;

			if( A.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}
			else if( A.rows( ) < A.cols( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "The number of rows should be larger than the number of columns." ) );
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					matrix< T, Allocator > &a = A;
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer dim    = m < n ? m : n;
					integer lda    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;

					// QR�������s���O�ɁC�K�v�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geqrf( m, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 ), tau( dim, 1 );
						__clapack__::geqrf( m, n, &( a[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );

						if( info == 0 )
						{
							// QR�����ɐ��������̂ŁA���ʂ���Q��R�̍s������߂�
							R.resize( m, n );
							Q.resize( m, m );
							for( size_type c = 0 ; c < R.cols( ) ; c++ )
							{
								for( size_type r = 0 ; r <= c ; r++ )
								{
									R( r, c ) = a( r, c );
								}
								for( size_type r = 0 ; r < R.rows( ) ; r++ )
								{
									Q( r, c ) = a( r, c );
								}
							}

							__clapack__::orgqr( m, m, dim, &( Q[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );
						}
					}
				}
				break;
			}

			if( info != 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
			}
		}
	};

	template < >
	struct __qr__< true >
	{
		// ���f���o�[�W����
		template < class T, class Allocator >
		static void qr_factorization( matrix< T, Allocator > &A, matrix< T, Allocator > &Q, matrix< T, Allocator > &R, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;
			typedef typename matrix< T, Allocator >::size_type size_type;

			if( A.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}
			else if( A.rows( ) < A.cols( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "The number of rows should be larger than the number of columns." ) );
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK�֐��̈���
					matrix< T, Allocator > &a = A;
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer dim    = m < n ? m : n;
					integer lda    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;

					// QR�������s���O�ɁC�K�v�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geqrf( m, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 ), tau( dim, 1 );
						__clapack__::geqrf( m, n, &( a[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );

						if( info == 0 )
						{
							// QR�����ɐ��������̂ŁA���ʂ���Q��R�̍s������߂�
							R.resize( m, n );
							Q.resize( m, m );
							for( size_type c = 0 ; c < R.cols( ) ; c++ )
							{
								for( size_type r = 0 ; r <= c ; r++ )
								{
									R( r, c ) = a( r, c );
								}
								for( size_type r = 0 ; r < R.rows( ) ; r++ )
								{
									Q( r, c ) = a( r, c );
								}
							}

							__clapack__::ungqr( m, n, dim, &( Q[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );
						}
					}
				}
				break;
			}

			if( info != 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
			}
		}
	};
}


namespace __cholesky__
{
	// �R���X�L�[�������s��
	template < class T, class Allocator >
	static matrix< T, Allocator >& cholesky_factorization( matrix< T, Allocator > &a, matrix_style::style style )
	{
		typedef matrix< T, Allocator > matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef typename matrix_type::size_type size_type;
		typedef __clapack__::integer integer;

		if( a.empty( ) )
		{
			// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
			throw( numerical_exception( 1, "Empty matrix." ) );
		}
		else if( a.rows( ) != a.cols( ) )
		{
			// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
			throw( numerical_exception( 1, "The number of rows should be equal to the number of columns." ) );
		}

		integer info = 0;

		// �R���X�L�[�����ŏ�O�p�͎Q�Ƃ��Ȃ��̂ŁC���O�p�s��ɂ��Ă���
		for( size_type r = 0 ; r < a.rows( ) ; r++ )
		{
			for( size_type c = r + 1 ; c < a.cols( ) ; c++ )
			{
				a( r, c ) = static_cast< value_type >( 0 );
			}
		}

		switch( style )
		{
		case matrix_style::sy:
		//case matrix_style::ge:
		default:
			{
				// LAPACK�֐��̈���
				integer n     = static_cast< integer >( a.cols( ) );
				integer lda   = static_cast< integer >( a.rows( ) );
				char uplo[]   = "L";

				// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
				__clapack__::potrf( uplo, n, &( a[0] ), lda, info );
			}
			break;
		}

		if( info < 0 )
		{
			// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
			throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
		}
		else if( info > 0 )
		{
			// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
			std::string val = __clapack__::to_string( info );
			std::string msg = "The leading minor of order " + val + " is not positive definite, and the factorization could not be completed.";
			throw( numerical_exception( info, msg ) );
		}

		return( a );
	}
}


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
			typedef matrix< T, Allocator > matrix_type;
			typedef typename matrix< T, Allocator >::value_type value_type;
			typedef __clapack__::integer integer;

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

			if( a.rows( ) == a.cols( ) )
			{
				// �Q�s�Q��ƂR�s�R��p�̓���o�[�W����
				switch( a.rows( ) )
				{
				case 2:
					{
						double a11 = a( 0, 0 );
						double a12 = a( 0, 1 );
						double a21 = a( 1, 0 );
						double a22 = a( 1, 1 );
						double detA = a11 * a22 - a12 * a21;
						if( detA != 0 )
						{
							a( 0, 0 ) = static_cast< value_type >(  a22 / detA );
							a( 0, 1 ) = static_cast< value_type >( -a12 / detA );
							a( 1, 0 ) = static_cast< value_type >( -a21 / detA );
							a( 1, 1 ) = static_cast< value_type >(  a11 / detA );
							return( a );
						}
						else
						{
							// �s�񎮂��[��
							throw( numerical_exception( 1, "Determinant of the matrix is zero." ) );
						}
					}
					break;

				case 3:
					{
						double a11 = a( 0, 0 ), a12 = a( 0, 1 ), a13 = a( 0, 2 );
						double a21 = a( 1, 0 ), a22 = a( 1, 1 ), a23 = a( 1, 2 );
						double a31 = a( 2, 0 ), a32 = a( 2, 1 ), a33 = a( 2, 2 );

						// ���ʂŗ��p����W���̌v�Z
						double _22x33_23x32_ = a22 * a33 - a23 * a32;
						double _21x32_22x31_ = a21 * a32 - a22 * a31;
						double _23x31_21x33_ = a23 * a31 - a21 * a33;

						// �s�񎮂��v�Z
						double detA = a11 * _22x33_23x32_ + a13 * _21x32_22x31_ + a12 * _23x31_21x33_;

						// �t�s�񂪑��݂���ꍇ�̂��t�s����v�Z����
						if( detA != 0 )
						{
							// �e�v�f�̒l���v�Z
							double A11 = _22x33_23x32_;
							double A12 = a13 * a32 - a12 * a33;
							double A13 = a12 * a23 - a13 * a22;
							double A21 = _23x31_21x33_;
							double A22 = a11 * a33 - a13 * a31;
							double A23 = a13 * a21 - a11 * a23;
							double A31 = _21x32_22x31_;
							double A32 = a12 * a31 - a11 * a32;
							double A33 = a11 * a22 - a12 * a21;

							double _1_detA = 1.0 / detA;
							a( 0, 0 ) = static_cast< value_type >( A11 * _1_detA );
							a( 0, 1 ) = static_cast< value_type >( A12 * _1_detA );
							a( 0, 2 ) = static_cast< value_type >( A13 * _1_detA );
							a( 1, 0 ) = static_cast< value_type >( A21 * _1_detA );
							a( 1, 1 ) = static_cast< value_type >( A22 * _1_detA );
							a( 1, 2 ) = static_cast< value_type >( A23 * _1_detA );
							a( 2, 0 ) = static_cast< value_type >( A31 * _1_detA );
							a( 2, 1 ) = static_cast< value_type >( A32 * _1_detA );
							a( 2, 2 ) = static_cast< value_type >( A33 * _1_detA );

							return( a );
						}
						else
						{
							// �s�񎮂��[��
							throw( numerical_exception( 1, "Determinant of the matrix is zero." ) );
						}
					}
					break;

				case 4:
					{
						double a11 = a( 0, 0 ), a12 = a( 0, 1 ), a13 = a( 0, 2 ), a14 = a( 0, 3 );
						double a21 = a( 1, 0 ), a22 = a( 1, 1 ), a23 = a( 1, 2 ), a24 = a( 1, 3 );
						double a31 = a( 2, 0 ), a32 = a( 2, 1 ), a33 = a( 2, 2 ), a34 = a( 2, 3 );
						double a41 = a( 3, 0 ), a42 = a( 3, 1 ), a43 = a( 3, 2 ), a44 = a( 3, 3 );

						// ���ʂŗ��p����W���̌v�Z
						double _33x44_34x43_ = a33 * a44 - a34 * a43;
						double _34x42_32x44_ = a34 * a42 - a32 * a44;
						double _32x43_33x42_ = a32 * a43 - a33 * a42;
						double _31x44_34x41_ = a31 * a44 - a34 * a41;
						double _33x41_31x43_ = a33 * a41 - a31 * a43;
						double _31x42_32x41_ = a31 * a42 - a32 * a41;

						// �s�񎮂��v�Z
						double detA = a11 * ( a22 * _33x44_34x43_ + a23 * _34x42_32x44_ + a24 * _32x43_33x42_ )
									- a12 * ( a21 * _33x44_34x43_ - a23 * _31x44_34x41_ - a24 * _33x41_31x43_ )
									- a13 * ( a21 * _34x42_32x44_ + a22 * _31x44_34x41_ - a24 * _31x42_32x41_ )
									- a14 * ( a21 * _32x43_33x42_ + a22 * _33x41_31x43_ + a23 * _31x42_32x41_ );

						// �t�s�񂪑��݂���ꍇ�̂��t�s����v�Z����
						if( detA != 0 )
						{
							// ���ʂŗ��p����W���̌v�Z
							double _23x44_24x43_ = a23 * a44 - a24 * a43;
							double _24x42_22x44_ = a24 * a42 - a22 * a44;
							double _22x43_23x42_ = a22 * a43 - a23 * a42;
							double _24x33_23x34_ = a24 * a33 - a23 * a34;
							double _22x34_24x32_ = a22 * a34 - a24 * a32;
							double _23x32_22x33_ = a23 * a32 - a22 * a33;
							double _21x44_24x41_ = a21 * a44 - a24 * a41;
							double _23x41_21x43_ = a23 * a41 - a21 * a43;
							double _24x31_21x34_ = a24 * a31 - a21 * a34;
							double _21x33_23x31_ = a21 * a33 - a23 * a31;
							double _21x42_22x41_ = a21 * a42 - a22 * a41;
							double _22x31_21x32_ = a22 * a31 - a21 * a32;

							// �e�v�f�̒l���v�Z
							double A11 =  a22 * _33x44_34x43_ + a23 * _34x42_32x44_ + a24 * _32x43_33x42_;
							double A12 = -a12 * _33x44_34x43_ - a13 * _34x42_32x44_ - a14 * _32x43_33x42_;
							double A13 =  a12 * _23x44_24x43_ + a13 * _24x42_22x44_ + a14 * _22x43_23x42_;
							double A14 =  a12 * _24x33_23x34_ + a13 * _22x34_24x32_ + a14 * _23x32_22x33_;
							double A21 = -a21 * _33x44_34x43_ + a23 * _31x44_34x41_ + a24 * _33x41_31x43_;
							double A22 =  a11 * _33x44_34x43_ - a13 * _31x44_34x41_ - a14 * _33x41_31x43_;
							double A23 = -a11 * _23x44_24x43_ + a13 * _21x44_24x41_ + a14 * _23x41_21x43_;
							double A24 = -a11 * _24x33_23x34_ + a13 * _24x31_21x34_ + a14 * _21x33_23x31_;
							double A31 = -a21 * _34x42_32x44_ - a22 * _31x44_34x41_ + a24 * _31x42_32x41_;
							double A32 =  a11 * _34x42_32x44_ + a12 * _31x44_34x41_ - a14 * _31x42_32x41_;
							double A33 = -a11 * _24x42_22x44_ - a12 * _21x44_24x41_ + a14 * _21x42_22x41_;
							double A34 = -a11 * _22x34_24x32_ - a12 * _24x31_21x34_ + a14 * _22x31_21x32_;
							double A41 = -a21 * _32x43_33x42_ - a22 * _33x41_31x43_ - a23 * _31x42_32x41_;
							double A42 =  a11 * _32x43_33x42_ + a12 * _33x41_31x43_ + a13 * _31x42_32x41_;
							double A43 = -a11 * _22x43_23x42_ - a12 * _23x41_21x43_ - a13 * _21x42_22x41_;
							double A44 = -a11 * _23x32_22x33_ - a12 * _21x33_23x31_ - a13 * _22x31_21x32_;

							double _1_detA = 1.0 / detA;
							a( 0, 0 ) = static_cast< value_type >( A11 * _1_detA );
							a( 0, 1 ) = static_cast< value_type >( A12 * _1_detA );
							a( 0, 2 ) = static_cast< value_type >( A13 * _1_detA );
							a( 0, 3 ) = static_cast< value_type >( A14 * _1_detA );
							a( 1, 0 ) = static_cast< value_type >( A21 * _1_detA );
							a( 1, 1 ) = static_cast< value_type >( A22 * _1_detA );
							a( 1, 2 ) = static_cast< value_type >( A23 * _1_detA );
							a( 1, 3 ) = static_cast< value_type >( A24 * _1_detA );
							a( 2, 0 ) = static_cast< value_type >( A31 * _1_detA );
							a( 2, 1 ) = static_cast< value_type >( A32 * _1_detA );
							a( 2, 2 ) = static_cast< value_type >( A33 * _1_detA );
							a( 2, 3 ) = static_cast< value_type >( A34 * _1_detA );
							a( 3, 0 ) = static_cast< value_type >( A41 * _1_detA );
							a( 3, 1 ) = static_cast< value_type >( A42 * _1_detA );
							a( 3, 2 ) = static_cast< value_type >( A43 * _1_detA );
							a( 3, 3 ) = static_cast< value_type >( A44 * _1_detA );
							return( a );
						}
						else
						{
							// �s�񎮂��[��
							throw( numerical_exception( 1, "Determinant of the matrix is zero." ) );
						}
					}
					break;

				default:
					break;
				}
			}

			if( info == 0 )
			{
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
						char uplo[]    = "U";

						// LU�������s��
						// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
						__clapack__::sytrf( uplo, n, NULL, lda, ipiv, &dmy, lwork, info );
						if( info == 0 )
						{
							lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
							matrix< T, Allocator > work( lwork, 1 );
							__clapack__::sytrf( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );

							if( info == 0 )
							{
								if( lwork < n )
								{
									work.resize( n, 1 );
								}
								__clapack__::sytri( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), info );

								// ���ʂ���O�p�݂̂ɂ����������Ă��Ȃ����߁C���O�p�ɂ��f�[�^���R�s�[����
								if( info == 0 )
								{
									typedef typename matrix< T, Allocator >::size_type size_type;
									for( size_type c = 0 ; c < a.rows( ) ; c++ )
									{
										for( size_type r = c + 1 ; r < a.rows( ) ; r++ )
										{
											a( r, c ) = a( c, r );
										}
									}
								}
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

						// LU�������s��
						__clapack__::getrf( lda, n, &( a[0] ), lda, ipiv, info );
						if( info == 0 )
						{
							// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
							__clapack__::getri( n, NULL, lda, NULL, &dmy, lwork, info );
							if( info == 0 )
							{
								lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
								matrix< T, Allocator > work( lwork, 1 );
								__clapack__::getri( n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );
							}
						}
						delete [] ipiv;
					}
					break;
				}
			}

			if( info < 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
			}
			else if( info > 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				std::string val = __clapack__::to_string( info );
				std::string msg = "U( " + val + ", " + val + " ) is exactly zero. the matrix is singular and its inverse could not be computed";
				throw( numerical_exception( info, msg ) );
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
			typedef typename T::value_type value_type;

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char uplo[]    = "U";

					// LU�������s��
					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::sytrf( uplo, n, NULL, lda, ipiv, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sytrf( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );

						if( info == 0 )
						{
							if( lwork < 2 * n )
							{
								work.resize( 2 * n, 1 );
							}
							__clapack__::sytri( uplo, n, &( a[0] ), lda, ipiv, &( work[0] ), info );

							// ���ʂ���O�p�݂̂ɂ����������Ă��Ȃ����߁C���O�p�ɂ��f�[�^���R�s�[����
							if( info == 0 )
							{
								typedef typename matrix< T, Allocator >::size_type size_type;
								for( size_type c = 0 ; c < a.rows( ) ; c++ )
								{
									for( size_type r = c + 1 ; r < a.rows( ) ; r++ )
									{
										a( r, c ) = a( c, r );
									}
								}
							}
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

					// LU�������s��
					__clapack__::getrf( lda, n, &( a[0] ), lda, ipiv, info );
					if( info == 0 )
					{
						// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
						__clapack__::getri( n, NULL, lda, NULL, &dmy, lwork, info );
						if( info == 0 )
						{
							lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
							matrix< T, Allocator > work( lwork, 1 );
							__clapack__::getri( n, &( a[0] ), lda, ipiv, &( work[0] ), lwork, info );
						}
					}
					delete [] ipiv;
				}
				break;
			}

			if( info < 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
			}
			else if( info > 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				std::string val = __clapack__::to_string( info );
				std::string msg = "U( " + val + ", " + val + " ) is exactly zero. the matrix is singular and its inverse could not be computed";
				throw( numerical_exception( info, msg ) );
			}

			return( a );
		}
	};
}



namespace __eigen__
{
	// ��ʍs��̌ŗL�l�E�ŗL�x�N�g�����v�Z����
#if defined(_USE_BALANCING_MATRIX_EIGEN_) && _USE_BALANCING_MATRIX_EIGEN_ != 0

	// �s��̑Ίp�����s���o�[�W����
	template < bool b >
	struct __eigen__
	{
		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					value_type vl     = 0;
					value_type vu     = 0;
					integer il        = 0;
					integer iu        = 0;
					value_type abstol = 0;
					char jobz[]        = "V";
					char range[]       = "A";
					char uplo[]        = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::syevx( jobz, range, uplo, n, NULL, lda, vl, vu, il, iu, abstol, m, NULL, NULL, ldz, &dmy, lwork, NULL, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						matrix< integer > iwork( 5 * n, 1 );
						matrix< integer > ifail( n, 1 );
						eigen_vector.resize( n, n );

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::syevx( jobz, range, uplo, n, &( a[0] ), lda, vl, vu, il, iu, abstol, m,
										&( eigen_value[0] ), &( eigen_vector[0] ), ldz, &( work[0] ), lwork, &( iwork[ 0 ] ), &( ifail[ 0 ] ), info );
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + " eigenvectors failed to converge." ) );
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
					integer ilo    = 0;
					integer ihi    = 0;
					char balanc[] = "B";
					char jobvl[]  = "N";
					char jobvr[]  = "V";
					char sense[]  = "N";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geevx( balanc, jobvl, jobvr, sense, n, NULL, lda, NULL, NULL, NULL, ldvl, NULL, ldvr, ilo, ihi, NULL, abnrm, NULL, NULL, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						matrix< T, Allocator > tmp( n, 1 );
						matrix< T, Allocator > scale( n, 1 );
						eigen_vector.resize( n, n );

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geevx( balanc, jobvl, jobvr, sense, n, &( a[0] ), lda, &( eigen_value[0] ), &( tmp[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, ilo, ihi, &( scale[0] ), abnrm, NULL, NULL, &( work[0] ), lwork, NULL, info );
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "QR algorithm failed to compute all the eigenvalues, and no eigenvectors or condition numbers have been computed." ) );
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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					integer ilo    = 0;
					integer ihi    = 0;
					char balanc[]  = "B";
					char jobvl[]   = "N";
					char jobvr[]   = "V";
					char sense[]   = "N";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geevx( balanc, jobvl, jobvr, sense, n, NULL, lda, NULL, NULL, ldvl, NULL, ldvr, ilo, ihi, NULL, abnrm, NULL, NULL, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						eigen_vector.resize( n, n );
						value_type *scale = new value_type[ n ];
						value_type *rwork = new value_type[ 2 * n ];

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geevx( balanc, jobvl, jobvr, sense, n, &( a[0] ), lda, &( eigen_value[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, ilo, ihi, scale, abnrm, NULL, NULL, &( work[0] ), lwork, rwork, info );

						delete [] scale;
						delete [] rwork;
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						if( info <= n )
						{
							std::string val = __clapack__::to_string( info );
							std::string msg = "U( " + val + ", " + val + " ) is exactly zero. The factorization has been completed, but the factor U is exactly singular, so the solution and error bounds could not be computed. reciprocal condition number = 0 is returned.";
							throw( numerical_exception( info, msg ) );
						}
						else
						{
							std::string msg = "U is nonsingular, but reciprocal condition number (RCOND) is less than machine precision, meaning that the matrix is singular to working precision.  Nevertheless, the solution and error bounds are computed because there are a number of situations where the computed solution can be more accurate than the value of RCOND would suggest.";
							throw( numerical_exception( info, msg ) );
						}
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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK�֐��̈���
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					char jobz[]    = "V";
					char uplo[]    = "U";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::syev( jobz, uplo, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						eigen_vector = a;

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::syev( jobz, uplo, n, &( eigen_vector[0] ), lda, &( eigen_value[0] ), &( work[0] ), lwork, info );
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "The algorithm failed to converge. " + __clapack__::to_string( info ) + "off-diagonal elements of an intermediate tridiagonal form did not converge to zero." ) );
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
					char jobvl[]   = "N";
					char jobvr[]   = "V";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geev( jobvl, jobvr, n, NULL, lda, NULL, NULL, NULL, ldvl, NULL, ldvr, &dmy, lwork, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						matrix< T, Allocator > tmp( n, 1 );
						eigen_vector.resize( n, n );

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geev( jobvl, jobvr, n, &( a[0] ), lda, &( eigen_value[0] ), &( tmp[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, &( work[0] ), lwork, info );
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "QR algorithm failed to compute all the eigenvalues, and no eigenvectors have been computed." ) );
					}
				}
				break;
			}

			if( info != 0 )
			{
				// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
				throw( numerical_exception( info, "Failed to compute the eigen values and vectors." ) );
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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobvl[]   = "N";
					char jobvr[]   = "V";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::geev( jobvl, jobvr, n, NULL, lda, NULL, NULL, ldvl, NULL, ldvr, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						eigen_vector.resize( n, n );
						value_type *rwork = new value_type[ 2 * n ];

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geev( jobvl, jobvr, n, &( a[0] ), lda, &( eigen_value[0] ),
							NULL, ldvl, &( eigen_vector[0] ), ldvr, &( work[0] ), lwork, rwork, info );

						delete [] rwork;
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "QR algorithm failed to compute all the eigenvalues, and no eigenvectors have been computed." ) );
					}
				}
				break;
			}

			return( eigen_value );
		}
	};

#endif
}



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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobz[] = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesdd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
						matrix< T, Allocator > ss( size, 1 );
						vt.resize( ldvt, n );
						integer *iwork = new integer[ 8 * size ];

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::gesdd( jobz, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, iwork, info );

						delete [] iwork;

						s.resize( m, n );
						for( size_type i = 0 ; i < ss.rows( ) ; i++ )
						{
							s( i, i ) = ss[ i ];
						}
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "DBDSDC did not converge, updating process failed." ) );
					}
				}
				break;
			}

			return( s );
		}

		// �����o�[�W����
		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &s, matrix< T, Allocator > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobz[]    = "O";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesdd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						integer *iwork = new integer[ 8 * size ];
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						s.resize( size, 1 );

						if( m >= n )
						{
							vt.resize( ldvt, n );
							__clapack__::gesdd( jobz, m, n, &( a[0] ), lda, &( s[0] ), NULL, ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, iwork, info );
						}
						else
						{
							vt.resize( ldu, m );
							__clapack__::gesdd( jobz, m, n, &( a[0] ), lda, &( s[0] ), &( vt[0] ), ldu, NULL, ldvt, &( work[0] ), lwork, iwork, info );

							vt.resize( ldvt, n );
							vt.fill( );
							for( integer r = 0 ; r < m ; r++ )
							{
								for( size_type c = 0 ; c < vt.cols( ) ; c++ )
								{
									vt( r, c ) = a( r, c );
								}
							}
						}

						delete [] iwork;
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "DBDSDC did not converge, updating process failed." ) );
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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobz[]    = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesdd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
						// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
						matrix< value_type > ss( size, 1 );
#else
						matrix< value_type, typename Allocator1::template rebind< value_type >::other > ss( size, 1 );
#endif
						vt.resize( ldvt, n );
						value_type *rwork = new value_type[ 5 * size * size + 5 * size ];
						integer *iwork = new integer[ 8 * size ];

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
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

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "DBDSDC did not converge, updating process failed." ) );
					}
				}
				break;
			}

			return( s );
		}


		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename matrix< T1, Allocator1 >::value_type complex_type;
			typedef typename T1::value_type value_type;

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobz[]    = "O";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesdd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, NULL, info );
					if( info == 0 )
					{
						value_type *rwork = new value_type[ 5 * size * size + 5 * size ];
						integer *iwork = new integer[ 8 * size ];

#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
						// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
						matrix< value_type > ss( size, 1 );
#else
						matrix< value_type, typename Allocator1::template rebind< value_type >::other > ss( size, 1 );
#endif

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T1, Allocator1 > work( lwork, 1 );
						complex_type *dmy = NULL;

						if( m >= n )
						{
							vt.resize( ldvt, n );
							__clapack__::gesdd( jobz, m, n, &( a[0] ), lda, &( ss[0] ), NULL, ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, rwork, iwork, info );
						}
						else
						{
							vt.resize( ldu, m );
							__clapack__::gesdd( jobz, m, n, &( a[0] ), lda, &( ss[0] ), &( vt[0] ), ldu, NULL, ldvt, &( work[0] ), lwork, rwork, iwork, info );

							vt.resize( ldvt, n );
							vt.fill( );
							for( integer r = 0 ; r < m ; r++ )
							{
								for( size_type c = 0 ; c < vt.cols( ) ; c++ )
								{
									vt( r, c ) = a( r, c );
								}
							}
						}

						delete [] rwork;
						delete [] iwork;

						s.resize( size, 1 );
						for( integer i = 0 ; i < size ; i++ )
						{
							s[ i ] = ss[ i ];
						}
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "DBDSDC did not converge, updating process failed." ) );
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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobu[]    = "A";
					char jobvt[]   = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesvd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
						matrix< T, Allocator > ss( m < n ? m : n, 1 );
						vt.resize( ldvt, n );

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::gesvd( jobu, jobvt, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, info );

						s.resize( m, n );
						for( size_type i = 0 ; i < ss.rows( ) ; i++ )
						{
							s( i, i ) = ss[ i ];
						}
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "DBDSQR did not converge, INFO specifies how many superdiagonals of an intermediate bidiagonal form B did not converge to zero. See the description of WORK above for details." ) );
					}
				}
				break;
			}

			return( s );
		}


		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &s, matrix< T, Allocator > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobu[]    = "N";
					char jobvt[]   = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesvd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, info );
					if( info == 0 )
					{
						s.resize( m < n ? m : n, 1 );
						vt.resize( ldvt, n );

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::gesvd( jobu, jobvt, m, n, &( a[0] ), lda, &( s[0] ), NULL, ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, info );
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "DBDSQR did not converge, INFO specifies how many superdiagonals of an intermediate bidiagonal form B did not converge to zero. See the description of WORK above for details." ) );
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

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobu[]    = "A";
					char jobvt[]   = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesvd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
						// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
						matrix< value_type > ss( size, 1 );
#else
						matrix< value_type, typename Allocator1::template rebind< value_type >::other > ss( size, 1 );
#endif
						vt.resize( ldvt, n );
						value_type *rwork = new value_type[ 5 * size ];

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T1, Allocator1 > work( lwork, 1 );
						__clapack__::gesvd( jobu, jobvt, m, n, &( a[0] ), lda, &( ss[0] ), &( u[0] ), ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, rwork, info );

						s.resize( m, n );
						for( size_type i = 0 ; i < ss.rows( ) ; i++ )
						{
							s( i, i ) = ss[ i ];
						}

						delete [] rwork;
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "ZBDSQR did not converge, INFO specifies how many superdiagonals of an intermediate bidiagonal form B did not converge to zero. See the description of WORK above for details." ) );
					}
				}
				break;
			}

			return( s );
		}


		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename T1::value_type value_type;

			if( a.empty( ) )
			{
				// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
				throw( numerical_exception( 1, "Empty matrix." ) );
			}

			integer info = 0;

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
					char jobu[]    = "N";
					char jobvt[]   = "A";

					// �܂��œK�ȍ�Ɨp�z��̃T�C�Y���擾����
					__clapack__::gesvd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
						// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
						s.resize( size, 1 );
#else
						matrix< value_type, typename Allocator1::template rebind< value_type >::other > ss( size, 1 );
#endif
						vt.resize( ldvt, n );
						value_type *rwork = new value_type[ 5 * size ];

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T1, Allocator1 > work( lwork, 1 );
						__clapack__::gesvd( jobu, jobvt, m, n, &( a[0] ), lda, &( s[0] ), NULL, ldu, &( vt[0] ), ldvt, &( work[0] ), lwork, rwork, info );

						delete [] rwork;
					}

					if( info < 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, __clapack__::to_string( info ) + "-th argument had an illegal value" ) );
					}
					else if( info > 0 )
					{
						// �s��v�Z���������I�����Ȃ������̂ŗ�O���X���[����
						throw( numerical_exception( info, "ZBDSQR did not converge, INFO specifies how many superdiagonals of an intermediate bidiagonal form B did not converge to zero. See the description of WORK above for details." ) );
					}
				}
				break;
			}

			return( s );
		}
	};

#endif
}



//! @addtogroup numeric_group �s�񉉎Z
//!  @{



/// @brief �s��~�s��̉��Z���s��
//! 
//! \f[ {\bf C} = \alpha \times {\bf A} \times {\bf B} + \beta \times {\bf C} \f]
//! 
//! @param[in]  a �c ���͍s�� \f${\bf A}\f$
//! @param[in]  b �c ���͍s�� \f${\bf B}\f$
//! @param[out] c �c �v�Z���ʂ��o�͂���s�� \f${\bf C}\f$
//! @param[in]  a_is_transpose �c �s�� \f${\bf A}\f$ ��]�u�s��Ƃ��Ċ|���Z����ꍇ�� true �Ƃ���
//! @param[in]  b_is_transpose �c �s�� \f${\bf B}\f$ ��]�u�s��Ƃ��Ċ|���Z����ꍇ�� true �Ƃ���
//! @param[in]  alpha �c �s�񉉎Z�̘a���v�Z����Ƃ��̌W��
//! @param[in]  beta  �c �s�񉉎Z�̘a���v�Z����Ƃ��̌W��
//!
//! @return �s��̐ς��������v�Z�ł������ǂ���
//! 
template < class T, class Allocator >
inline bool multiply( const matrix< T, Allocator > &a, const matrix< T, Allocator > &b, matrix< T, Allocator > &c, bool a_is_transpose, bool b_is_transpose, typename matrix< T, Allocator >::value_type alpha, typename matrix< T, Allocator >::value_type beta )
{
	typedef __clapack__::integer integer;
	typedef typename matrix< T, Allocator >::value_type value_type;

	if( a.empty( ) || b.empty( ) )
	{
		// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
		return( false );
	}

	// LAPACK�֐��̈���
	integer m     = static_cast< integer >( a_is_transpose ? a.cols( ) : a.rows( ) );
	integer n     = static_cast< integer >( b_is_transpose ? b.rows( ) : b.cols( ) );
	integer k     = static_cast< integer >( a_is_transpose ? a.rows( ) : a.cols( ) );
	integer k_    = static_cast< integer >( b_is_transpose ? b.cols( ) : b.rows( ) );
	integer lda   = a_is_transpose ? k : m;
	integer ldb   = b_is_transpose ? n : k;
	integer ldc   = m;
	char transa[ 2 ];
	char transb[ 2 ];

	if( k != k_ )
	{
		// �s��̃T�C�Y���������Ȃ��̂ŗ�O���X���[����
		return( false );
	}

	transa[ 0 ] = a_is_transpose ? 'T' : 'N';
	transa[ 1 ] = '\0';
	transb[ 0 ] = b_is_transpose ? 'T' : 'N';
	transb[ 1 ] = '\0';

	c.resize( m, n );
	c.fill( );

	// BLAS���[�`���ł́C���͍s��A��B�̓��e�͕ω����Ȃ����C
	// �C���^�[�t�F�[�X�� const �C�����󂯂Ă��Ȃ��̂ŃL���X�g���s��
	__clapack__::gemm( transa, transb, m, n, k, alpha, const_cast< value_type * >( &( a[ 0 ] ) ), lda, const_cast< value_type * >( &( b[ 0 ] ) ), ldb, beta, &( c[ 0 ] ), ldc );

	return( true );
}



/// @brief �s��~�s��̉��Z���s��
//! 
//! \f[ {\bf C} = {\bf A} \times {\bf B} \f]
//! 
//! @param[in]  a �c ���͍s�� \f${\bf A}\f$
//! @param[in]  b �c ���͍s�� \f${\bf B}\f$
//! @param[out] c �c �v�Z���ʂ��o�͂���s�� \f${\bf C}\f$
//! @param[in]  a_is_transpose �c �s�� \f${\bf A}\f$ ��]�u�s��Ƃ��Ċ|���Z����ꍇ�� true �Ƃ���i�f�t�H���g�� false �j
//! @param[in]  b_is_transpose �c �s�� \f${\bf B}\f$ ��]�u�s��Ƃ��Ċ|���Z����ꍇ�� true �Ƃ���i�f�t�H���g�� false �j
//!
//! @return �s��̐ς��������v�Z�ł������ǂ���
//! 
template < class T, class Allocator >
inline bool multiply( const matrix< T, Allocator > &a, const matrix< T, Allocator > &b, matrix< T, Allocator > &c, bool a_is_transpose = false, bool b_is_transpose = false )
{
	return( multiply( a, b, c, a_is_transpose, b_is_transpose, 1, 0 ) );
}



/// @brief LU�����Ȃǂœ�����s�{�b�g�z�񂩂�s�{�b�g�s����쐬
//! 
//! @param[in]  pivot �c �s�{�b�g�z��
//! @param[out] out   �c �s�{�b�g�s��
//!
//! @return �s�{�b�g�s��
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline void permutation_matrix( const matrix< T1, Allocator1 > &pivot, matrix< T2, Allocator2 > &out )
{
	typedef typename matrix< T1, Allocator1 >::size_type size_type;
	typedef typename matrix< T1, Allocator1 >::value_type value_type;

	size_type i;
	matrix< T1, Allocator1 > p( pivot );
	for( i = 0 ; i < pivot.size( ) ; i++ )
	{
		p[ i ] = static_cast< value_type >( i );
	}

	// �s�{�b�g�Ńf�[�^�����ւ���
	for( i = 0 ; i < pivot.size( ) ; i++ )
	{
		value_type tmp = p[ i ];
		p[ i ] = p[ pivot[ i ] - 1 ];
		p[ pivot[ i ] - 1 ] = tmp;
	}

	out.resize( pivot.size( ), pivot.size( ) );
	out.fill( );

	// �s�{�b�g�s����쐬����
	for( i = 0 ; i < pivot.size( ) ; i++ )
	{
		out( p[ i ], i ) = 1;
	}
}


/// @brief �g���[�X�̌v�Z�i�Ίp�����̘a�j
//! 
//! \f[ tr\left( {\bf A} \right) = \sum^{n}_{i=1}{ a_{ii} } \f]
//! 
//! @param[in] a �c ���͍s��
//!
//! @return \f$tr\left( {\bf A} \right)\f$
//! 
template < class T, class Allocator >
inline const typename matrix< T, Allocator >::value_type trace( const matrix< T, Allocator > &a )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef typename matrix< T, Allocator >::value_type value_type;
	value_type v = value_type( 0 );
	size_type size = a.rows( ) < a.cols( ) ? a.rows( ) : a.cols( );
	for( size_type i = 0 ; i < size ; ++i )
	{
		v += a( i, i );
	}
	return( v );
}


/** @brief �s�񎮂̌v�Z
 *  
 *  \f[
 *  	\left| {\bf A} \right|
 *      =
 *      \left|
 *           \begin{array}{ccccc}
 *             a_{11} & \cdots & a_{1j} & \cdots & a_{1n} \\
 *             a_{21} & \cdots & a_{2j} & \cdots & a_{2n} \\
 *             \vdots & \cdots & \vdots & \cdots & \vdots \\
 *             a_{n1} & \cdots & a_{nj} & \cdots & a_{nn}
 *           \end{array}
 *      \right|
 *      =
 *      \sum^{n}_{j=1}{
 *        \left( -1 \right)^{j+1} a_{1j}
 *        \left|
 *             \begin{array}{cccccc}
 *               a_{21} & \cdots & a_{2,j-1} & a_{2,j+1} & \cdots & a_{2n} \\
 *               \vdots & \cdots & \vdots    & \vdots    & \cdots & \vdots \\
 *               a_{n1} & \cdots & a_{n,j-1} & a_{n,j+1} & \cdots & a_{nn}
 *             \end{array}
 *        \right|
 *     }
 *  \f]
 *  
 *  @param[in] a     �c ���͍s��
 *  @param[in] style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
 * 
 *  @return \f$\left| {\bf A} \right|\f$
 */
template < class T, class Allocator >
inline const typename matrix< T, Allocator >::value_type det( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef typename matrix< T, Allocator >::value_type value_type;

	if( a.empty( ) || a.rows( ) != a.cols( ) )
	{
		return( value_type( 0 ) );
	}

	switch( a.rows( ) )
	{
	case 1:
		return( a( 0, 0 ) );
		break;

	case 2:
		return( a( 0, 0 ) * a( 1, 1 ) - a( 0, 1 ) * a( 1, 0 ) );
		break;

	case 3:
		return( a( 0, 0 ) * a( 1, 1 ) * a( 2, 2 ) + a( 0, 1 ) * a( 1, 2 ) * a( 2, 0 ) + a( 0, 2 ) * a( 1, 0 ) * a( 2, 1 )
				- a( 0, 2 ) * a( 1, 1 ) * a( 2, 0 ) - a( 0, 1 ) * a( 1, 0 ) * a( 2, 2 ) - a( 0, 0 ) * a( 1, 2 ) * a( 2, 1 ) );
		break;

	default:
		{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
			// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
			matrix< __clapack__::integer > pivot;
#else
			matrix< __clapack__::integer, typename Allocator::template rebind< __clapack__::integer >::other > pivot;
#endif

			try
			{
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

				if( count % 2 == 0 )
				{
					return( v );
				}
				else
				{
					return( -v );
				}
			}
			catch( const numerical_exception &e )
			{
				// error �����̏ꍇ�͍��{�I�Ɍv�Z�s�\�D
				if( e.error < 0 )
				{
					throw( e );
				}
				else
				{
					return( 0 );
				}
			}
		}
		break;
	}
}



/// @brief �s��̘A���ꎟ�������������֐�
//!
//! \f[ {\bf A}\mbox{\boldmath x} = \mbox{\boldmath b} \f]
//!
//! @param[in]  a     �c �s�� \f${\bf A}\f$
//! @param[out] b     �c �x�N�g�� \f$\mbox{\boldmath b}\f$
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return �x�N�g�� \f$\mbox{\boldmath x}\f$
//!
template < class T, class Allocator >
inline const matrix< T, Allocator >& solve( const matrix< T, Allocator > &a, matrix< T, Allocator > &b, matrix_style::style style = matrix_style::ge )
{
	try
	{
		matrix< T, Allocator > a_( a );
		return( __solve__::__solve__< __numeric__::is_complex< T >::value >::solve( a_, b, style ) );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}



/// @brief �s���LU�������s��
//! 
//! @param[in]  a     �c ���͍s��
//! @param[out] pivot �c �s�{�b�g�I�����s�������ʂ�������x�N�g��
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return LU�������ꂽ����
//! 
template < class T, class Allocator1, class Allocator2 >
const matrix< T, Allocator1 > lu_factorization( const matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style = matrix_style::ge )
{
	try
	{
		matrix< T, Allocator1 > a_( a );
		return( __lu__::__lu__< __numeric__::is_complex< T >::value >::lu_factorization( a_, pivot, style ) );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}


/// @brief �s���LU�������s��
//! 
//! \f[ {\bf A} = {\bf P} \; \times \; {\bf L} \; \times \; {\bf U} \f]
//! 
//! @param[in]  a     �c ���͍s�� \f$\mbox{\bf A}\f$
//! @param[out] L     �c ���O�p�s�� \f$\mbox{\bf L}\f$
//! @param[out] U     �c ��O�p�s�� \f$\mbox{\bf U}\f$
//! @param[out] pivot �c �s�{�b�g�I�����s�������ʂ����ɖ߂��s�� \f$\mbox{\bf P}\f$
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return LU�����ɐ����������ǂ���
//! 
template < class T, class Allocator >
bool lu_factorization( const matrix< T, Allocator > &a, matrix< T, Allocator > &L, matrix< T, Allocator > &U, matrix< T, Allocator > &pivot, matrix_style::style style = matrix_style::ge )
{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
	matrix< __clapack__::integer > piv( a.cols( ), 1 );
#else
	matrix< __clapack__::integer, typename Allocator::template rebind< __clapack__::integer >::other > piv( a.cols( ), 1 );
#endif

	try
	{
		L = a;
		__lu__::__lu__< __numeric__::is_complex< T >::value >::lu_factorization( L, piv, style );

		typedef typename matrix< T, Allocator >::size_type size_type;

		U.resize( L.rows( ), L.cols( ) );
		for( size_type r = 0 ; r < a.rows( ) ; r++ )
		{
			for( size_type c = r ; c < a.cols( ) ; c++ )
			{
				U( r, c ) = L( r, c );
				L( r, c ) = 0;
			}
			L( r, r ) = 1;
		}

		permutation_matrix( piv, pivot );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}

	return( true );
}


/// @brief �s���LU�������s��
//! 
//! @param[in]  a     �c ���͍s��
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return LU�������ꂽ����
//! 
template < class T, class Allocator >
const matrix< T, Allocator > lu_factorization( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	// VC6�ł�STL�̃A���P�[�^�̒�`���A�W���ɏ������Ă��Ȃ��̂ŁA�f�t�H���g�ő�p����
	matrix< __clapack__::integer > pivot( a.cols( ), 1 );
#else
	matrix< __clapack__::integer, typename Allocator::template rebind< __clapack__::integer >::other > pivot( a.cols( ), 1 );
#endif

	try
	{
		matrix< T, Allocator > a_( a );
		return( __lu__::__lu__< __numeric__::is_complex< T >::value >::lu_factorization( a_, pivot, style ) );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}



/// @brief �Ώ̍s��̃R���X�L�[�������s��
//! 
//! @param[in]  a     �c ���͑Ώ̍s��
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return �R���X�L�[�������ꂽ����
//! 
template < class T, class Allocator >
const matrix< T, Allocator > cholesky_factorization( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::sy )
{
	try
	{
		matrix< T, Allocator > a_( a );
		return( __cholesky__::cholesky_factorization( a_, style ) );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}



/// @brief �s���QR�������s��
//! 
//! \f[ {\bf A} = {\bf Q} \; {\bf R} \f]
//! 
//! @param[in]  a     �c ���͍s��
//! @param[out] Q     �c �s��Q
//! @param[out] R     �c �s��R
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//! 
template < class T, class Allocator >
void qr_factorization( const matrix< T, Allocator > &a, matrix< T, Allocator > &Q, matrix< T, Allocator > &R, matrix_style::style style = matrix_style::ge )
{
	try
	{
		matrix< T, Allocator > a_( a );
		__qr__::__qr__< __numeric__::is_complex< T >::value >::qr_factorization( a_, Q, R, style );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}



/// @brief �s��̋t�s���LU������p���Čv�Z����
//! 
//! �����s��Ŗ����ꍇ�́C���ْl������p���Ĉ�ʉ��t�s����v�Z����
//! 
//! \f[ {\bf A}^{-1} \f]
//! 
//! @param[in] a     �c ���͍s��
//! @param[in] style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return �t�s�� \f$ {\bf A}^{-1} \f$
//! 
template < class T, class Allocator >
matrix< T, Allocator > inverse( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef matrix< T, Allocator > matrix_type;
	typedef typename matrix_type::size_type size_type;

	try
	{
		if( a.rows( ) != a.cols( ) )
		{
			// ���ْl������p���Ĉ�ʉ��t�s����v�Z����
			matrix_type u, s, vt;
			svd( a, u, s, vt, style );
			size_type num = s.rows( ) < s.cols( ) ? s.rows( ) : s.cols( );
			for( size_type i = 0 ; i < num ; i++ )
			{
				if( __clapack__::get_real( s( i, i ) ) != 0 )
				{
					s( i, i ) = 1 / __clapack__::get_real( s( i, i ) );
				}
			}
			return( ( u * s * vt ).t( ) );
		}
		else
		{
			matrix_type a_( a );
			return( __inverse__::__inverse__< __numeric__::is_complex< T >::value >::inverse( a_, style ) );
		}
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}



/// @brief �s��̌ŗL�l�E�ŗL�x�N�g�����v�Z����
//! 
//! \f[ {\bf A}\mbox{\boldmath x} = \lambda\mbox{\boldmath x} \f]
//! 
//! �v�Z���ʂ́C_DESCENDING_ORDER_EIGEN_VALUE_ �̒l�ɂ���āC�ŗL�l�������E�~���̂ǂ��炩�ŕ��Ԃ悤�ɕϊ������
//! 
//! @param[in]  a            �c ���͍s�� \f${\bf A}\f$
//! @param[out] eigen_value  �c �ŗL�l�������E�~���̂ǂ��炩�œ������x�N�g�� \f$\lambda\f$
//! @param[out] eigen_vector �c �ŗL�l�̏����E�~���̂ǂ��炩�ɑΉ����C������ŗL�x�N�g�������񂾍s�� \f$\mbox{\boldmath x}\f$
//! @param[in]  style        �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return �ŗL�l�����񂾃x�N�g���ieigen_value�Ɠ����j \f$\lambda\f$
//! 
template < class T, class Allocator >
const matrix< T, Allocator >& eigen( const matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix< T, Allocator >::size_type size_type;

	matrix< T, Allocator > a_( a );

	try
	{
		__eigen__::__eigen__< __numeric__::is_complex< T >::value >::eigen( a_, eigen_value, eigen_vector, style );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}

	typedef __clapack__::__value_index_pair__< T > value_index_pair;

	std::vector< value_index_pair > vips( eigen_value.size( ) );

	for( size_type i = 0 ; i < eigen_value.size( ) ; i++ )
	{
		value_index_pair &vi = vips[ i ];
		vi.value = eigen_value[ i ];
		vi.index = i;
	}


#if defined( _DESCENDING_ORDER_EIGEN_VALUE_ ) && _DESCENDING_ORDER_EIGEN_VALUE_ == 1
	// �ŗL�l�ƌŗL�x�N�g�����~���ɕ��ёւ���
	std::sort( vips.begin( ), vips.end( ), value_index_pair::descending );
#else
	// �ŗL�l�ƌŗL�x�N�g���������ɕ��ёւ���
	std::sort( vips.begin( ), vips.end( ), value_index_pair::ascending );
#endif

	a_ = eigen_vector;

	for( size_type i = 0 ; i < vips.size( ) ; i++ )
	{
		const value_index_pair &vi = vips[ i ];
		eigen_value[ i ] = vi.value;

		for( size_type j = 0 ; j < eigen_vector.rows( ) ; j++ )
		{
			eigen_vector( j, i ) = a_( j, vi.index );
		}
	}

	return( eigen_value );
}


/// @brief �s��̓��ْl�������v�Z����
//! 
//! \f[ {\bf A} = {\bf U}{\bf \Sigma}{\bf V}^T \f]
//! 
//! @note �Ίp�s��̐����́C���ォ��l�̑傫�����ɕ���
//! 
//! @param[in]  a     �c ���͍s�� \f$ {\bf A} \f$
//! @param[out] u     �c �񒼍s�s�� \f$ {\bf U} \f$
//! @param[out] s     �c �Ίp�s�� \f$ {\bf \Sigma} \f$
//! @param[out] vt    �c ���s�s��̓]�u \f$ {\bf V}^T \f$
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return �Ίp�s�� \f$ {\bf \Sigma} \f$
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
const matrix< T2, Allocator2 >& svd( const matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style = matrix_style::ge )
{
	try
	{
		matrix< T1, Allocator1 > a_( a );
		return( __svd__::__svd__< __numeric__::is_complex< T1 >::value >::svd( a_, u, s, vt, style ) );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}


/// @brief �s��̓��ْl�������v�Z����
//! 
//! \f[ {\bf A} = {\bf U}{\bf \Sigma}{\bf V}^T \f]
//! 
//! \f$ {\bf \Sigma} \f$ �̑̊i������ \f$ {\bf V}^T \f$ �݂̂��v�Z����
//! 
//! @note �Ίp�s��̐����́C���ォ��l�̑傫�����ɕ���
//! 
//! @param[in]  a     �c ���͍s�� \f$ {\bf A} \f$
//! @param[out] s     �c �Ίp�s�� \f$ {\bf \Sigma} \f$ �̑̊i�����݂̂̃x�N�g��
//! @param[out] vt    �c ���s�s��̓]�u \f$ {\bf V}^T \f$
//! @param[in]  style �c ���͍s��̌`���i�f�t�H���g�͈�ʍs����w��j
//!
//! @return �Ίp�s�� \f$ {\bf \Sigma} \f$
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
const matrix< T2, Allocator2 >& svd( const matrix< T1, Allocator1 > &a, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style = matrix_style::ge )
{
	try
	{
		matrix< T1, Allocator1 > a_( a );
		return( __svd__::__svd__< __numeric__::is_complex< T1 >::value >::svd( a_, s, vt, style ) );
	}
	catch( const numerical_exception &e )
	{
		throw( e );
	}
}


/// @}
//  �s�񉉎Z�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_NUMERIC__
