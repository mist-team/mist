/// @file mist/numeric.h
//!
//! @brief Lapackを用いた行列演算ライブラリ
//!
#ifndef __INCLUDE_NUMERIC__
#define __INCLUDE_NUMERIC__

#include <complex>


#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

// 行列クラスがインクルードされていない場合はインポートする．
#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif


// mist名前空間の始まり
_MIST_BEGIN



//! @addtogroup numeric_group 行列演算
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/numeric.h>
//! @endcode
//!
//!  @{


/// @brief 行列計算をする際の入力となる行列の形式
//! 
//! @attention この形式に従って，内部で呼び出す関数が変更される
//! 
struct matrix_style
{
	/// @brief 行列計算をする際の入力となる行列の形式
	enum style
	{
		ge,		///< 一般行列
		gb,		///< 一般帯行列
		gt,		///< 一般3重対角行列
		sy,		///< 対称行列
		sb,		///< 対称帯行列
		st,		///< 対称3重対角行列
		he,		///< エルミート行列
		hb,		///< エルミート帯行列
		ht,		///< エルミート3重対角行列
	};
};

/// @}
//  行列演算グループの終わり


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

		// 各型に対する特殊化
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

// インテルのMKLとの互換性を保つための，関数名の変換マクロ
#if defined(_USE_INTEL_MATH_KERNEL_LIBRARY_) && _USE_INTEL_MATH_KERNEL_LIBRARY_ != 0
	#define LPFNAME( name ) name
#else
	#define LPFNAME( name ) name ## _
#endif

	extern "C"
	{
		typedef long int integer;
		typedef float real;
		typedef double doublereal;
		typedef struct { real r, i; } complex;
		typedef struct { doublereal r, i; } doublecomplex;

		// 一般正方行列の連立方程式を解く関数
		int LPFNAME( sgesv ) ( integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info );
		int LPFNAME( dgesv ) ( integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info );
		int LPFNAME( cgesv ) ( integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info );
		int LPFNAME( zgesv ) ( integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info );
		// 一般帯行列の連立方程式を解く関数
		int LPFNAME( sgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, integer *ipiv, real *b, integer *ldb, integer *info );
		int LPFNAME( dgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, integer *ipiv, doublereal *b, integer *ldb, integer *info );
		int LPFNAME( cgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, integer *ipiv, complex *b, integer *ldb, integer *info );
		int LPFNAME( zgbsv ) ( integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, integer *ipiv, doublecomplex *b, integer *ldb, integer *info );
		// 一般三重対角行列の連立方程式を解く関数
		int LPFNAME( sgtsv ) ( integer *n, integer *nrhs, real *dl, real *d__, real *du, real *b, integer *ldb, integer *info );
		int LPFNAME( dgtsv ) ( integer *n, integer *nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *b, integer *ldb, integer *info );
		int LPFNAME( cgtsv ) ( integer *n, integer *nrhs, complex *dl, complex *d__, complex *du, complex *b, integer *ldb, integer *info );
		int LPFNAME( zgtsv ) ( integer *n, integer *nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *b, integer *ldb, integer *info );
		// 対称正方行列の連立方程式を解く関数
		int LPFNAME( ssysv ) ( char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info );
		int LPFNAME( dsysv ) ( char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( csysv ) ( char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info );
		int LPFNAME( zsysv ) ( char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info );
		// エルミート行列の連立方程式を解く関数
		int LPFNAME( chesv ) ( char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info );
		int LPFNAME( zhesv ) ( char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info );


		// 一般行列のLU分解
		int LPFNAME( sgetrf ) ( integer *m, integer *n, real *a, integer *lda, integer *ipiv, integer *info );
		int LPFNAME( dgetrf ) ( integer *m, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info );
		int LPFNAME( cgetrf ) ( integer *m, integer *n, complex *a, integer *lda, integer *ipiv, integer *info );
		int LPFNAME( zgetrf ) ( integer *m, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info );
		// 一般帯行列の連立方程式を解く関数
		int LPFNAME( sgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, integer *ipiv, integer *info );
		int LPFNAME( dgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, integer *ipiv, integer *info );
		int LPFNAME( cgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, integer *ipiv, integer *info );
		int LPFNAME( zgbtrf ) ( integer *m, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, integer *ipiv, integer *info );
		// 一般三重対角行列の連立方程式を解く関数
		int LPFNAME( sgttrf ) ( integer *n, real *dl, real *d__, real *du, real *du2, integer *ipiv, integer *info );
		int LPFNAME( dgttrf ) ( integer *n, doublereal *dl, doublereal *d__, doublereal *du, doublereal *du2, integer *ipiv, integer *info );
		int LPFNAME( cgttrf ) ( integer *n, complex *dl, complex *d__, complex *du, complex *du2, integer *ipiv, integer *info );
		int LPFNAME( zgttrf ) ( integer *n, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *du2, integer *ipiv, integer *info );
		// 対称行列のLU分解
		int LPFNAME( ssytrf ) ( char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info );
		int LPFNAME( dsytrf ) ( char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( csytrf ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int LPFNAME( zsytrf ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );
		// エルミート行列のLU分解
		int LPFNAME( chetrf ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int LPFNAME( zhetrf ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );


		// 一般行列のQR分解
		int LPFNAME( sgeqrf ) ( integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info );
		int LPFNAME( dgeqrf ) ( integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgeqrf ) ( integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info );
		int LPFNAME( zgeqrf ) ( integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info );


		// LU分解の結果を用いた一般行列の逆行列の計算
		int LPFNAME( sgetri ) ( integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info );
		int LPFNAME( dgetri ) ( integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgetri ) ( integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info );
		int LPFNAME( zgetri ) ( integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info );
		// LU分解の結果を用いた対称行列の逆行列の計算
		int LPFNAME( ssytri ) ( char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *info );
		int LPFNAME( dsytri ) ( char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *info );
		int LPFNAME( csytri ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info );
		int LPFNAME( zsytri ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info );
		// LU分解の結果を用いたエルミート行列の逆行列の計算
		int LPFNAME( chetri ) ( char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info );
		int LPFNAME( zhetri ) ( char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info );


		// 一般行列に対する固有値・固有ベクトルを計算
		int LPFNAME( sgeev ) ( char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info );
		int LPFNAME( dgeev ) ( char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgeev ) ( char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zgeev ) ( char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// 対称行列に対する固有値・固有ベクトルを計算
		int LPFNAME( ssyev ) ( char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *info );
		int LPFNAME( dsyev ) ( char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *info );
		// 対称帯行列に対する固有値・固有ベクトルを計算
		int LPFNAME( ssbev ) ( char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *info );
		int LPFNAME( dsbev ) ( char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *info );
		// 対称3重対角行列に対する固有値・固有ベクトルを計算
		int LPFNAME( sstev ) ( char *jobz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *info );
		int LPFNAME( dstev ) ( char *jobz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *info );
		// エルミート行列に対する固有値・固有ベクトルを計算
		int LPFNAME( cheev ) ( char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zheev ) ( char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// エルミート帯行列に対する固有値・固有ベクトルを計算
		int LPFNAME( chbev ) ( char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *info );
		int LPFNAME( zhbev ) ( char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *info );


		// 一般行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
		int LPFNAME( sgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, real *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( dgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublereal *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( cgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zgeevx ) ( char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );
		// 対称行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
		int LPFNAME( ssyevx ) ( char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( dsyevx ) ( char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *ifail, integer *info );
		// 対称帯行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
		int LPFNAME( ssbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( dsbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info );
		// 対称3重対角行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
		int LPFNAME( sstevx ) ( char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( dstevx ) ( char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info );
		// エルミート行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
		int LPFNAME( cheevx ) ( char *jobz, char *range, char *uplo, integer *n, complex *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( zheevx ) ( char *jobz, char *range, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *ifail, integer *info );
		// エルミート帯行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
		int LPFNAME( chbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, complex *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer *info );
		int LPFNAME( zhbevx ) ( char *jobz, char *range, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublecomplex *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *iwork, integer *ifail, integer *info );
		// エルミート3重対角行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン


		// 一般行列に対する特異値分解を計算
		int LPFNAME( sgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *info );
		int LPFNAME( dgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *info );
		int LPFNAME( cgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *info );
		int LPFNAME( zgesvd ) ( char *jobu, char *jobvt, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info );


		// 一般行列に対する特異値分解を計算．分割統治法を用いた高速化バージョン
		int LPFNAME( sgesdd ) ( char *jobz, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( dgesdd ) ( char *jobz, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *iwork, integer *info );
		int LPFNAME( cgesdd ) ( char *jobz, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *iwork, integer *info );
		int LPFNAME( zgesdd ) ( char *jobz, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *info );



		// 対称行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
		int LPFNAME( ssyevd ) ( char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( dsyevd ) ( char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		// 対称帯行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
		int LPFNAME( ssbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( dsbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		// 対称3重対角行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
		int LPFNAME( sstevd ) ( char *jobz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( dstevd ) ( char *jobz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info );
		// エルミート行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
		int LPFNAME( cheevd ) ( char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( zheevd ) ( char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
		// エルミート帯行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
		int LPFNAME( chbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
		int LPFNAME( zhbevd ) ( char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info );
	}


	// 複素数及び実数の両方を区別することなく，実数部の値を取り出す関数
	inline real       get_real( const real &r ){ return( r ); }
	inline doublereal get_real( const doublereal &r ){ return( r ); }
	inline real       get_real( const std::complex< real > &r ){ return( r.real( ) ); }
	inline doublereal get_real( const std::complex< doublereal > &r ){ return( r.real( ) ); }


	// 一般正方行列の連立方程式を解く関数
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
	// 一般帯行列の連立方程式を解く関数
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
	// 一般三重対角行列の連立方程式を解く関数
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
	// 対称正方行列の連立方程式を解く関数
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
	// エルミート行列の連立方程式を解く関数
	inline int hesv( char *uplo, integer &n, integer &nrhs, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *b, integer &ldb, std::complex< real > *work, integer &lwork, integer&info )
	{
		return( LPFNAME( chesv ) ( uplo, &n, &nrhs, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( b ), &ldb, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int hesv( char *uplo, integer &n, integer &nrhs, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *b, integer &ldb, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zsysv ) ( uplo, &n, &nrhs, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( b ), &ldb, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// 一般行列のLU分解
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
	// 一般帯行列の連立方程式を解く関数
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
	// 一般三重対角行列の連立方程式を解く関数
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
	// 対称行列のLU分解
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
	// エルミート行列のLU分解
	inline int hetrf( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( chetrf ) ( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &lwork, &info ) );
	}
	inline int hetrf( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &lwork, integer &info )
	{
		return( LPFNAME( zhetrf ) ( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &lwork, &info ) );
	}


	// QR分解
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


	// LU分解の結果を用いた一般行列の逆行列の計算
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
	// LU分解の結果を用いた対称行列の逆行列の計算
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
	// LU分解の結果を用いたエルミート行列の逆行列の計算
	inline int hetri( char *uplo, integer &n, std::complex< real > *a, integer &lda, integer *ipiv, std::complex< real > *work, integer &info )
	{
		return( LPFNAME( chetri ) ( uplo, &n, reinterpret_cast< complex* >( a ), &lda, ipiv, reinterpret_cast< complex* >( work ), &info ) );
	}
	inline int hetri( char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, integer *ipiv, std::complex< doublereal > *work, integer &info )
	{
		return( LPFNAME( zhetri ) ( uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, ipiv, reinterpret_cast< doublecomplex* >( work ), &info ) );
	}


	// 一般行列に対する固有値・固有ベクトルを計算
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
	// 対称行列に対する固有値・固有ベクトルを計算
	inline int syev( char *jobz, char *uplo, integer &n, real *a, integer &lda, real *w, real *work, integer &lwork, integer &info )
	{
		return( LPFNAME( ssyev ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, &info ) );
	}
	inline int syev( char *jobz, char *uplo, integer &n, doublereal *a, integer &lda, doublereal *w, doublereal *work, integer &lwork, integer &info )
	{
		return( LPFNAME( dsyev ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, &info ) );
	}
	// 対称帯行列に対する固有値・固有ベクトルを計算
	inline int sbev( char *jobz, char *uplo, integer &n, integer &kd, real *ab, integer &ldab, real *w, real *z__, integer &ldz, real *work, integer &info )
	{
		return( LPFNAME( ssbev ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &info ) );
	}
	inline int sbev( char *jobz, char *uplo, integer &n, integer &kd, doublereal *ab, integer &ldab, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer &info )
	{
		return( LPFNAME( dsbev ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &info ) );
	}
	// 対称3重対角行列に対する固有値・固有ベクトルを計算
	inline int stev( char *jobz, integer &n, real *d__, real *e, real *z__, integer &ldz, real *work, integer &info )
	{
		return( LPFNAME( sstev ) ( jobz, &n, d__, e, z__, &ldz, work, &info ) );
	}
	inline int stev( char *jobz, integer &n, doublereal *d__, doublereal *e, doublereal *z__, integer &ldz, doublereal *work, integer &info )
	{
		return( LPFNAME( dstev ) ( jobz, &n, d__, e, z__, &ldz, work, &info ) );
	}
	// エルミート行列に対する固有値・固有ベクトルを計算
	inline int heev( char *jobz, char *uplo, integer &n, std::complex< real > *a, integer &lda, real *w, std::complex< real > *work, integer &lwork, real *rwork, integer &info )
	{
		return( LPFNAME( cheev ) ( jobz, uplo, &n, reinterpret_cast< complex* >( a ), &lda, w, reinterpret_cast< complex* >( work ), &lwork, rwork, &info ) );
	}
	inline int heev( char *jobz, char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *w, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zheev ) ( jobz, uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, w, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &info ) );
	}
	// エルミート帯行列に対する固有値・固有ベクトルを計算
	inline int hbev( char *jobz, char *uplo, integer &n, integer &kd, std::complex< real > *ab, integer &ldab, real *w, std::complex< real > *z__, integer &ldz, std::complex< real > *work, real *rwork, integer &info )
	{
		return( LPFNAME( chbev ) ( jobz, uplo, &n, &kd, reinterpret_cast< complex* >( ab ), &ldab, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), rwork, &info ) );
	}
	inline int hbev( char *jobz, char *uplo, integer &n, integer &kd, std::complex< doublereal > *ab, integer &ldab, doublereal *w, std::complex< doublereal > *z__, integer &ldz, std::complex< doublereal > *work, doublereal *rwork, integer &info )
	{
		return( LPFNAME( zhbev ) ( jobz, uplo, &n, &kd, reinterpret_cast< doublecomplex* >( ab ), &ldab, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), rwork, &info ) );
	}



	// 一般行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
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
	// 対称行列に対する固有値・固有ベクトルを計算
	inline int geevx( char *jobz, char *range, char *uplo, integer &n, real *a, integer &lda,
						real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, real *z__,
						integer &ldz, real *work, integer &lwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( ssyevx ) ( jobz, range, uplo, &n, a, &lda, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, &lwork, iwork, ifail, &info ) );
	}
	inline int geevx( char *jobz, char *range, char *uplo, integer &n, doublereal *a, integer &lda,
						doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, doublereal *z__,
						integer &ldz, doublereal *work, integer &lwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( dsyevx ) ( jobz, range, uplo, &n, a, &lda, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, &lwork, iwork, ifail, &info ) );
	}
	// 対称帯行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
	inline int sbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, real *ab, integer &ldab, real *q, integer &ldq, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, real *z__, integer &ldz, real *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( ssbevx ) ( jobz, range, uplo, &n, &kd, ab, &ldab, q, &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	inline int dsbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, doublereal *ab, integer &ldab, doublereal *q, integer &ldq, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( dsbevx ) ( jobz, range, uplo, &n, &kd, ab, &ldab, q, &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	// 対称3重対角行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
	inline int stevx( char *jobz, char *range, integer &n, real *d__, real *e, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, real *z__, integer &ldz, real *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( sstevx ) ( jobz, range, &n, d__, e, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	inline int stevx( char *jobz, char *range, integer &n, doublereal *d__, doublereal *e, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( dstevx ) ( jobz, range, &n, d__, e, &vl, &vu, &il, &iu, &abstol, &m, w, z__, &ldz, work, iwork, ifail, &info ) );
	}
	// エルミート行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
	inline int heevx( char *jobz, char *range, char *uplo, integer &n, std::complex< real > *a, integer &lda, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, std::complex< real > *z__, integer &ldz, std::complex< real > *work, integer &lwork, real *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( cheevx ) ( jobz, range, uplo, &n, reinterpret_cast< complex* >( a ), &lda, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), &lwork, rwork, iwork, ifail, &info ) );
	}
	inline int heevx( char *jobz, char *range, char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, std::complex< doublereal > *z__, integer &ldz, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( zheevx ) ( jobz, range, uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, iwork, ifail, &info ) );
	}
	// エルミート帯行列に対する固有値・固有ベクトルを計算．行列を対角化して精度を上げるバージョン
	inline int hbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, complex *ab, integer &ldab, complex *q, integer &ldq, real &vl, real &vu, integer &il, integer &iu, real &abstol, integer &m, real *w, complex *z__, integer &ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( chbevx ) ( jobz, range, uplo, &n, &kd, reinterpret_cast< complex* >( ab ), &ldab, reinterpret_cast< complex* >( q ), &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), rwork, iwork, ifail, &info ) );
	}
	inline int hbevx( char *jobz, char *range, char *uplo, integer &n, integer &kd, complex *ab, integer &ldab, complex *q, integer &ldq, doublereal &vl, doublereal &vu, integer &il, integer &iu, doublereal &abstol, integer &m, doublereal *w, complex *z__, integer &ldz, complex *work, doublereal *rwork, integer *iwork, integer *ifail, integer &info )
	{
		return( LPFNAME( zhbevx ) ( jobz, range, uplo, &n, &kd, reinterpret_cast< doublecomplex* >( ab ), &ldab, reinterpret_cast< doublecomplex* >( q ), &ldq, &vl, &vu, &il, &iu, &abstol, &m, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), rwork, iwork, ifail, &info ) );
	}




	// 一般行列に対する特異値分解を計算
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

	// 一般行列に対する特異値分解を計算．分割統治法を用いた高速バージョン
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


	// 対称行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
	inline int syevd( char *jobz, char *uplo, integer &n, real *a, integer &lda, real *w, real *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( ssyevd ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, iwork, &liwork, &info ) );
	}
	inline int syevd( char *jobz, char *uplo, integer &n, doublereal *a, integer &lda, doublereal *w, doublereal *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( dsyevd ) ( jobz, uplo, &n, a, &lda, w, work, &lwork, iwork, &liwork, &info ) );
	}
	// 対称帯行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
	inline int sbevd( char *jobz, char *uplo, integer &n, integer &kd, real *ab, integer &ldab, real *w, real *z__, integer &ldz, real *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( ssbevd ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	inline int sbevd( char *jobz, char *uplo, integer &n, integer &kd, doublereal *ab, integer &ldab, doublereal *w, doublereal *z__, integer &ldz, doublereal *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( dsbevd ) ( jobz, uplo, &n, &kd, ab, &ldab, w, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	// 対称3重対角行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
	inline int stevd( char *jobz, integer &n, real *d__, real *e, real *z__, integer &ldz, real *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( sstevd ) ( jobz, &n, d__, e, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	inline int stevd( char *jobz, integer &n, doublereal *d__, doublereal *e, doublereal *z__, integer &ldz, doublereal *work, integer &lwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( dstevd ) ( jobz, &n, d__, e, z__, &ldz, work, &lwork, iwork, &liwork, &info ) );
	}
	// エルミート行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
	inline int heevd( char *jobz, char *uplo, integer &n, std::complex< real > *a, integer &lda, real *w, std::complex< real > *work, integer &lwork, real *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( cheevd ) ( jobz, uplo, &n, reinterpret_cast< complex* >( a ), &lda, w, reinterpret_cast< complex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}
	inline int heevd( char *jobz, char *uplo, integer &n, std::complex< doublereal > *a, integer &lda, doublereal *w, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( zheevd ) ( jobz, uplo, &n, reinterpret_cast< doublecomplex* >( a ), &lda, w, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}
	// エルミート帯行列に対する固有値・固有ベクトルを計算を計算．分割統治法を用いた高速バージョン
	inline int hbevd( char *jobz, char *uplo, integer &n, integer &kd, std::complex< real > *ab, integer &ldab, real *w, std::complex< real > *z__, integer &ldz, std::complex< real > *work, integer &lwork, real *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( chbevd ) ( jobz, uplo, &n, &kd, reinterpret_cast< complex* >( ab ), &ldab, w, reinterpret_cast< complex* >( z__ ), &ldz, reinterpret_cast< complex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}
	inline int hbevd( char *jobz, char *uplo, integer &n, integer &kd, std::complex< doublereal > *ab, integer &ldab, doublereal *w, std::complex< doublereal > *z__, integer &ldz, std::complex< doublereal > *work, integer &lwork, doublereal *rwork, integer &lrwork, integer *iwork, integer &liwork, integer &info )
	{
		return( LPFNAME( zhbevd ) ( jobz, uplo, &n, &kd, reinterpret_cast< doublecomplex* >( ab ), &ldab, w, reinterpret_cast< doublecomplex* >( z__ ), &ldz, reinterpret_cast< doublecomplex* >( work ), &lwork, rwork, &lrwork, iwork, &liwork, &info ) );
	}


	// インテルのMKLとの互換性を保つための，関数名の変換マクロ
	#undef LPFNAME
}


namespace __solve__
{
	// 行列の連立一次方程式を解く関数
	template < bool b >
	struct __solve__
	{
		// 実数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& solve( matrix< T, Allocator > &a, matrix< T, Allocator > &b, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;

			if( a.rows( ) != b.rows( ) || a.empty( ) || b.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					value_type dmy;
					integer *ipiv = new integer[ n ];
					integer lwork  = -1;
					char *uplo    = "U";

					// まず最適な作業用配列のサイズを取得する
					__clapack__::sysv( uplo, n, nrhs, NULL, lda, NULL, NULL, ldb, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sysv( uplo, n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, &( work[0] ), lwork, info );
					}
					delete [] ipiv;
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					integer *ipiv = new integer[ n ];

					// integer     n: マトリックス a の列数
					// integer  nrhs: マトリックス b の列数
					// double*     a: 連立方程式を解く係数の配列（ lda×n 要素 ）
					// integer   lda: マトリックス a の行数
					// integer* ipiv: ピボット選択に用いる配列（ lda要素 ）
					// double*     b: 連立方程式を解く係数の配列（ ldb×n 要素 ）
					// integer   ldb: マトリックス b の行数
					// integer  info: 正常終了したか否かを知らせてくれる変数
					//
					// この関数を呼ぶ場合，入力となる a の内容は変更される
					// 最終的な結果は b に代入される
					__clapack__::gesv( n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, info );
					delete [] ipiv;
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( b );
		}
	};

	template < >
	struct __solve__< true >
	{
		// 複素数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& solve( matrix< T, Allocator > a, matrix< T, Allocator > &b, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			if( a.rows( ) != b.rows( ) || a.empty( ) || b.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					value_type dmy;
					integer *ipiv = new integer[ n ];
					integer lwork  = -1;
					char *uplo    = "U";

					// まず最適な作業用配列のサイズを取得する
					__clapack__::sysv( uplo, n, nrhs, NULL, lda, NULL, NULL, ldb, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::sysv( uplo, n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, &( work[0] ), lwork, info );
					}
					delete [] ipiv;
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer n     = static_cast< integer >( a.cols( ) );
					integer nrhs  = static_cast< integer >( b.cols( ) );
					integer lda   = static_cast< integer >( a.rows( ) );
					integer ldb   = static_cast< integer >( b.rows( ) );
					integer *ipiv = new integer[ n ];

					// integer     n: マトリックス a の列数
					// integer  nrhs: マトリックス b の列数
					// double*     a: 連立方程式を解く係数の配列（ lda×n 要素 ）
					// integer   lda: マトリックス a の行数
					// integer* ipiv: ピボット選択に用いる配列（ lda要素 ）
					// double*     b: 連立方程式を解く係数の配列（ ldb×n 要素 ）
					// integer   ldb: マトリックス b の行数
					// integer  info: 正常終了したか否かを知らせてくれる変数
					//
					// この関数を呼ぶ場合，入力となる a の内容は変更される
					// 最終的な結果は b に代入される
					__clapack__::gesv( n, nrhs, &( a[0] ), lda, ipiv, &( b[0] ), ldb, info );
					delete [] ipiv;
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( b );
		}
	};
}




namespace __lu__
{
	// 行列の連立一次方程式を解く関数
	template < bool b >
	struct __lu__
	{
		// 実数バージョン
		template < class T, class Allocator1, class Allocator2 >
		static matrix< T, Allocator1 >& lu_factorization( matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator1 >::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer lwork  = -1;
					value_type dmy;
					char *uplo    = "U";

					pivot.resize( n, 1 );

					// まず最適な作業用配列のサイズを取得する
					__clapack__::sytrf( uplo, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator1 > work( lwork, 1 );

						// LU分解を行う
						__clapack__::sytrf( uplo, n, &( a[0] ), lda, &( pivot[0] ), &( work[0] ), lwork, info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;

					pivot.resize( n, 1 );

					// LU分解を行う
					__clapack__::getrf( m, n, &( a[0] ), lda, &( pivot[0] ), info );
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( a );
		}
	};

	template < >
	struct __lu__< true >
	{
		// 複素数バージョン
		template < class T, class Allocator1, class Allocator2 >
		static matrix< T, Allocator1 >& lu_factorization( matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer lwork  = -1;
					value_type dmy;
					char *uplo    = "U";

					pivot.resize( n, 1 );

					// まず最適な作業用配列のサイズを取得する
					__clapack__::sytrf( uplo, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator1 > work( lwork, 1 );

						// LU分解を行う
						__clapack__::sytrf( uplo, n, &( a[0] ), lda, &( pivot[0] ), &( work[0] ), lwork, info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;

					pivot.resize( n, 1 );

					// LU分解を行う
					__clapack__::getrf( m, n, &( a[0] ), lda, &( pivot[0] ), info );
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( a );
		}
	};
}




namespace __qr__
{
	// 行列の連立一次方程式を解く関数
	template < bool b >
	struct __qr__
	{
		// 実数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& qr_factorization( matrix< T, Allocator > &a, matrix< T, Allocator > &tau, matrix_style::style style )
		{
			typedef __clapack__::integer integer;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
			case matrix_style::ge:
			default:
				{
					tau.resize( a.rows( ), a.cols( ) );

					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;

					// QR分解を行う前に，必要な作業用配列のサイズを取得する
					__clapack__::geqrf( m, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geqrf( m, n, &( a[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );
					}
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( a );
		}
	};

	template < >
	struct __qr__< true >
	{
		// 複素数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& qr_factorization( matrix< T, Allocator > &a, matrix< T, Allocator > &tau, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					tau.resize( a.rows( ), a.cols( ) );

					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;

					// QR分解を行う前に，必要な作業用配列のサイズを取得する
					__clapack__::geqrf( m, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geqrf( m, n, &( a[0] ), lda, &( tau[0] ), &( work[0] ), lwork, info );
					}
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( a );
		}
	};
}



namespace __inverse__
{
	// 行列の連立一次方程式を解く関数
	template < bool b >
	struct __inverse__
	{
		// 実数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& inverse( matrix< T, Allocator > &a, matrix_style::style style )
		{
			typedef __clapack__::integer integer;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					char *uplo    = "U";

					// LU分解を行う
					// まず最適な作業用配列のサイズを取得する
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

							// 結果が上三角のみにしか代入されていないため，下三角にもデータをコピーする
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
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;

					// LU分解を行う
					__clapack__::getrf( lda, n, &( a[0] ), lda, ipiv, info );
					if( info == 0 )
					{
						// まず最適な作業用配列のサイズを取得する
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

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( a );
		}
	};

	template < >
	struct __inverse__< true >
	{
		// 複素数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& inverse( matrix< T, Allocator > &a, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					char *uplo    = "U";

					// LU分解を行う
					// まず最適な作業用配列のサイズを取得する
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

							// 結果が上三角のみにしか代入されていないため，下三角にもデータをコピーする
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
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					integer *ipiv  = new integer[ n ];
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;

					// LU分解を行う
					__clapack__::getrf( lda, n, &( a[0] ), lda, ipiv, info );
					if( info == 0 )
					{
						// まず最適な作業用配列のサイズを取得する
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

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( a );
		}
	};
}


namespace __eigen__
{
	// 一般行列の固有値・固有ベクトルを計算する
#if defined(_USE_BALANCING_MATRIX_EIGEN_) && _USE_BALANCING_MATRIX_EIGEN_ != 0

	// 行列の対角化を行うバージョン
	template < bool b >
	struct __eigen__
	{
		// 実数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
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
					char *jobz        = "V";
					char *range       = "A";
					char *uplo        = "U";

					// まず最適な作業用配列のサイズを取得する
					__clapack__::geevx( jobz, range, uplo, n, NULL, lda, vl, vu, il, iu, abstol, m, NULL, NULL, ldz, &dmy, lwork, NULL, NULL, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						matrix< integer > iwork( 5 * n, 1 );
						matrix< integer > ifail( n, 1 );
						eigen_vector.resize( n, n );

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::geevx( jobz, range, uplo, n, &( a[0] ), lda, vl, vu, il, iu, abstol, m,
										&( eigen_value[0] ), &( eigen_vector[0] ), ldz, &( work[0] ), lwork, &( iwork[ 0 ] ), &( ifail[ 0 ] ), info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy, abnrm;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					integer ilo    = 0;
					integer ihi    = 0;
					char *balanc = "B";
					char *jobvl  = "N";
					char *jobvr  = "V";
					char *sense  = "N";

					// まず最適な作業用配列のサイズを取得する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( eigen_value );
		}
	};

	template < >
	struct __eigen__< true >
	{
		// 複素数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{

					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					value_type abnrm;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					integer ilo    = 0;
					integer ihi    = 0;
					char *balanc = "B";
					char *jobvl  = "N";
					char *jobvr  = "V";
					char *sense  = "N";

					// まず最適な作業用配列のサイズを取得する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( eigen_value );
		}
	};


#else

	// 行列の対角化を行わないバージョン
	template < bool b >
	struct __eigen__
	{
		// 実数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::sy:
				{
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					integer lwork  = -1;
					char *jobz = "V";
					char *uplo = "U";

					// まず最適な作業用配列のサイズを取得する
					__clapack__::syev( jobz, uplo, n, NULL, lda, NULL, &dmy, lwork, info );
					if( info == 0 )
					{
						eigen_value.resize( n, 1 );
						eigen_vector = a;

						lwork = static_cast< integer >( __clapack__::get_real( dmy ) );
						matrix< T, Allocator > work( lwork, 1 );
						__clapack__::syev( jobz, uplo, n, &( eigen_vector[0] ), lda, &( eigen_value[0] ), &( work[0] ), lwork, info );
					}
				}
				break;

			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					char *jobvl = "N";
					char *jobvr = "V";

					// まず最適な作業用配列のサイズを取得する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( eigen_value );
		}
	};

	template < >
	struct __eigen__< true >
	{
		// 複素数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& eigen( matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename T::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = static_cast< integer >( a.rows( ) );
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvl   = 1;
					integer ldvr   = n;
					integer lwork  = -1;
					char *jobvl = "N";
					char *jobvr = "V";

					// まず最適な作業用配列のサイズを取得する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( eigen_value );
		}
	};

#endif
}



namespace __svd__
{
	// 一般行列の特異値分解を計算する
#if defined(_USE_DIVIDE_AND_CONQUER_SVD_) && _USE_DIVIDE_AND_CONQUER_SVD_ != 0

	// 分割統治法を用いるバージョンの特異値分解
	template < bool b >
	struct __svd__
	{
		// 実数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &u, matrix< T, Allocator > &s, matrix< T, Allocator > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					integer size   = m < n ? m : n;
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					char *jobz = "A";

					// まず最適な作業用配列のサイズを取得する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( s );
		}
	};

	template < >
	struct __svd__< true >
	{
		// 複素数バージョン
		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename T1::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					integer size   = m < n ? m : n;
					typename matrix< T1, Allocator1 >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					char *jobz = "A";

					// まず最適な作業用配列のサイズを取得する
					__clapack__::gesdd( jobz, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
						// VC6ではSTLのアロケータの定義が、標準に準拠していないので、デフォルトで代用する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( s );
		}
	};

#else

	template < bool b >
	struct __svd__
	{
		// 実数バージョン
		template < class T, class Allocator >
		static matrix< T, Allocator >& svd( matrix< T, Allocator > &a, matrix< T, Allocator > &u, matrix< T, Allocator > &s, matrix< T, Allocator > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T, Allocator >::size_type size_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					typename matrix< T, Allocator >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					char *jobu = "A";
					char *jobvt = "A";

					// まず最適な作業用配列のサイズを取得する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( s );
		}
	};

	template < >
	struct __svd__< true >
	{
		// 複素数バージョン
		template < class T1, class T2, class Allocator1, class Allocator2 >
		static matrix< T2, Allocator2 >& svd( matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style )
		{
			typedef __clapack__::integer integer;
			typedef typename matrix< T1, Allocator1 >::size_type size_type;
			typedef typename T1::value_type value_type;

			if( a.empty( ) )
			{
				// 行列のサイズが正しくないので例外をスローする
				throw;
			}

			integer info = 0;

			switch( style )
			{
			case matrix_style::ge:
			default:
				{
					// LAPACK関数の引数
					integer m      = static_cast< integer >( a.rows( ) );
					integer n      = static_cast< integer >( a.cols( ) );
					integer lda    = m;
					integer ldu    = m;
					integer size   = m < n ? m : n;
					typename matrix< T1, Allocator1 >::value_type dmy;
					integer ldvt   = n;
					integer lwork  = -1;
					char *jobu = "A";
					char *jobvt = "A";

					// まず最適な作業用配列のサイズを取得する
					__clapack__::gesvd( jobu, jobvt, m, n, NULL, lda, NULL, NULL, ldu, NULL, ldvt, &dmy, lwork, NULL, info );
					if( info == 0 )
					{
						u.resize( ldu, m );
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
						// VC6ではSTLのアロケータの定義が、標準に準拠していないので、デフォルトで代用する
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
				}
				break;
			}

			if( info != 0 )
			{
				// 行列計算が正しく終了しなかったので例外をスローする
				throw;
			}

			return( s );
		}
	};

#endif

}


//! @addtogroup numeric_group 行列演算
//!  @{


/// @brief トレースの計算（対角成分の和）
//! 
//! \f[
//! 	tr\left( {\bf A} \right) = \sum^{n}_{i=1}{ a_{ii} }
//! \f]
//! 
//! @param[in] a … 入力行列
//!
//! @return \f$tr\left( {\bf A} \right)\f$
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

/// @brief トレースの計算（対角成分の和）
//! 
//! \f[
//! 	tr\left( {\bf A} \right) = \sum^{n}_{i=1}{ a_{ii} }
//! \f]
//! 
//! @param[in] expression … 複数の行列演算を表す式
//!
//! @return \f$tr\left( {\bf A} \right)\f$
//! 
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


/// @brief 行列式の計算
//! 
//! \f[
//! 	\left| {\bf A} \right|
//!     =
//!     \left|
//!          \begin{array}{ccccc}
//!            a_{11} & \cdots & a_{1j} & \cdots & a_{1n} \\ //
//!            a_{21} & \cdots & a_{2j} & \cdots & a_{2n} \\ //
//!            \vdots & \cdots & \vdots & \cdots & \vdots \\ //
//!            a_{n1} & \cdots & a_{nj} & \cdots & a_{nn}
//!          \end{array}
//!     \right|
//!     =
//!     \sum^{n}_{j=1}{
//!       \left( -1 \right)^{j+1} a_{1j}
//!       \left|
//!            \begin{array}{cccccc}
//!              a_{21} & \cdots & a_{2,j-1} & a_{2,j+1} & \cdots & a_{2n} \\ //
//!              \vdots & \cdots & \vdots    & \vdots    & \cdots & \vdots \\ //
//!              a_{n1} & \cdots & a_{n,j-1} & a_{n,j+1} & \cdots & a_{nn}
//!            \end{array}
//!       \right|
//!    }
//! \f]
//! 
//! @param[in] a     … 入力行列
//! @param[in] style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return \f$\left| {\bf A} \right|\f$
//! 
template < class T, class Allocator >
inline const typename matrix< T, Allocator >::value_type det( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix< T, Allocator >::size_type size_type;
	typedef typename matrix< T, Allocator >::value_type value_type;

	if( a.empty( ) || a.rows( ) != a.cols( ) )
	{
		return( value_type( ) );
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
				- a( 0, 2 ) * a( 1, 1 ) * a( 2, 0 ) + a( 0, 1 ) * a( 1, 0 ) * a( 2, 2 ) + a( 0, 0 ) * a( 1, 2 ) * a( 2, 1 ) );
		break;

	default:
		{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
			// VC6ではSTLのアロケータの定義が、標準に準拠していないので、デフォルトで代用する
			matrix< __clapack__::integer > pivot;
#else
			matrix< __clapack__::integer, typename Allocator::template rebind< __clapack__::integer >::other > pivot;
#endif
			matrix< T, Allocator > m = lu_factorization( a, pivot, style );

			value_type v = m( 0, 0 );
			size_type size = a.rows( ) < a.cols( ) ? a.rows( ) : a.cols( );
			size_type count = 0, i;

			// LU分解時に行の入れ替えが行われた回数を計算する
			for( i = 0 ; i < pivot.size( ) ; ++i )
			{
				count += static_cast< size_type >( pivot[ i ] ) != i + 1 ? 1 : 0;
			}

			// 対角成分の積を計算する
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
		break;
	}
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief 行列式の計算
//! 
//! @param[in] e     … 複数の行列演算を表す式
//! @param[in] style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return 戻り値の説明
//! 
template < class Expression >
inline const typename matrix_expression< Expression >::value_type det( const matrix_expression< Expression > &e, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::size_type size_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;

	if( e.rows( ) * e.cols( ) == 0 || e.rows( ) != e.cols( ) )
	{
		return( value_type( ) );
	}

	switch( e.rows( ) )
	{
	case 1:
		return( e( 0, 0 ) );
		break;

	case 2:
		return( e( 0, 0 ) * e( 1, 1 ) - e( 0, 1 ) * e( 1, 0 ) );
		break;

	case 3:
		return( e( 0, 0 ) * e( 1, 1 ) * e( 2, 2 ) + e( 0, 1 ) * e( 1, 2 ) * e( 2, 0 ) + e( 0, 2 ) * e( 1, 0 ) * e( 2, 1 )
				- e( 0, 2 ) * e( 1, 1 ) * e( 2, 0 ) + e( 0, 1 ) * e( 1, 0 ) * e( 2, 2 ) + e( 0, 0 ) * e( 1, 2 ) * e( 2, 1 ) );
		break;

	default:
		{
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
			// VC6ではSTLのアロケータの定義が、標準に準拠していないので、デフォルトで代用する
			matrix< __clapack__::integer > pivot;
#else
			matrix< __clapack__::integer, typename allocator_type::template rebind< __clapack__::integer >::other > pivot;
#endif
			matrix_type m = lu_factorization( e, pivot, style );

			value_type v = m( 0, 0 );
			size_type size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
			size_type count = 0, i;

			// LU分解時に行の入れ替えが行われた回数を計算する
			for( i = 0 ; i < pivot.size( ) ; ++i )
			{
				count += static_cast< size_type >( pivot[ i ] ) != i + 1 ? 1 : 0;
			}

			// 対角成分の積を計算する
			for( i = 1 ; i < size ; ++i )
			{
				v *= m( i, i );
			}

			return( count % 2 == 0 ? v : -v );
		}
		break;
	}
}

#endif



/// @brief 3×3 の対角行列を計算する
//! 
//! 3つの入力成分の大きいものから順番に並べ替えて，対角行列を作成する
//! 
//! \f[
//!     \left(
//!          \begin{array}{ccc}
//!            \sigma_1 &    0     &    0     \\ //
//!               0     & \sigma_2 &    0     \\ //
//!               0     &    0     & \sigma_3
//!          \end{array}
//!     \right)
//! \f]
//! ただし，\f$ \sigma_1 \ge \sigma_2 \ge \sigma_3 \f$
//! 
//! @param[in] s1 … 成分1
//! @param[in] s2 … 成分2
//! @param[in] s3 … 成分3
//!
//! @return 対角行列
//! 
template < class T >
inline const matrix< typename type_trait< T >::value_type > diag( const T &s1, const typename type_trait< T >::value_type &s2, const typename type_trait< T >::value_type &s3 )
{
	matrix< typename type_trait< T >::value_type > d( 3, 3 );
	double ss1 = static_cast< double >( __clapack__::get_real( s1 ) );
	double ss2 = static_cast< double >( __clapack__::get_real( s2 ) );
	double ss3 = static_cast< double >( __clapack__::get_real( s3 ) );
	if( ss1 < ss2 )
	{
		// ss1 < ss2
		if( ss1 < ss3 )
		{
			if( ss2 < ss3 )
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
		// ss2 < ss1
		if( ss1 < ss3 )
		{
			d( 0, 0 ) = s3;
			d( 1, 1 ) = s1;
			d( 2, 2 ) = s2;
		}
		else
		{
			if( ss2 < ss3 )
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



/// @brief 行列の連立一次方程式を解く関数
//!
//! \f[ {\bf A}\mbox{\boldmath x} = \mbox{\boldmath b} \f]
//!
//! @param[in]  a     … 行列 \f${\bf A}\f$
//! @param[out] b     … ベクトル \f$\mbox{\boldmath b}\f$
//! @param[in]  style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return ベクトル \f$\mbox{\boldmath x}\f$
//!
template < class T, class Allocator >
inline const matrix< T, Allocator >& solve( const matrix< T, Allocator > &a, matrix< T, Allocator > &b, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	return( __solve__::__solve__< __numeric__::is_complex< T >::value >::solve( a_, b, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief 行列の連立一次方程式を解く関数
//!
//! \f[ {\bf A}\mbox{\boldmath x} = \mbox{\boldmath b} \f]
//! 
//! @param[in]  expression … 複数の行列演算を表す式A
//! @param[out] b          … ベクトル \f$\mbox{\boldmath b}\f$
//! @param[in]  style      … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return ベクトル \f$\mbox{\boldmath x}\f$
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



/// @brief 行列のLU分解を行う
//! 
//! @param[in]  a     … 入力行列
//! @param[out] pivot … ピボット選択を行った結果を代入するベクトル
//! @param[in]  style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return LU分解された結果
//! 
template < class T, class Allocator1, class Allocator2 >
const matrix< T, Allocator1 > lu_factorization( const matrix< T, Allocator1 > &a, matrix< __clapack__::integer, Allocator2 > &pivot, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator1 > a_( a );
	return( __lu__::__lu__< __numeric__::is_complex< T >::value >::lu_factorization( a_, pivot, style ) );
}

/// @brief 行列のLU分解を行う
//! 
//! @param[in]  a     … 入力行列
//! @param[in]  style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return LU分解された結果
//! 
template < class T, class Allocator >
const matrix< T, Allocator > lu_factorization( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef __clapack__::integer integer;
	matrix< T, Allocator > a_( a );
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	// VC6ではSTLのアロケータの定義が、標準に準拠していないので、デフォルトで代用する
	matrix< __clapack__::integer > pivot( a.cols( ), 1 );
#else
	matrix< __clapack__::integer, typename Allocator::template rebind< __clapack__::integer >::other > pivot( a.cols( ), 1 );
#endif
	return( __lu__::__lu__< __numeric__::is_complex< T >::value >::lu_factorization( a_, pivot, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief 行列のLU分解を行う
//! 
//! @param[in]  expression … 複数の行列演算を表す式
//! @param[out] pivot      … ピボット選択を行った結果を代入するベクトル
//! @param[in]  style      … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return LU分解された結果
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


/// @brief 行列のLU分解を行う
//! 
//! @param[in]  expression … 複数の行列演算を表す式
//! @param[in]  style      … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return LU分解された結果
//! 
template < class Expression >
inline const matrix< typename matrix_expression< Expression >::value_type, typename matrix_expression< Expression >::allocator_type >
						lu_factorization( const matrix_expression< Expression > &expression, matrix_style::style style = matrix_style::ge )
{
	typedef typename matrix_expression< Expression >::value_type value_type;
	typedef typename matrix_expression< Expression >::allocator_type allocator_type;
	typedef matrix< value_type, allocator_type > matrix_type;
	matrix_type a_( expression );
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ < 7
	// VC6ではSTLのアロケータの定義が、標準に準拠していないので、デフォルトで代用する
	matrix< __clapack__::integer > pivot( a_.cols( ), 1 );
#else
	matrix< __clapack__::integer, typename allocator_type::template rebind< __clapack__::integer >::other > pivot( a_.cols( ), 1 );
#endif
	return( __lu__::__lu__< __numeric__::is_complex< value_type >::value >::lu_factorization( a_, pivot, style ) );
}

#endif


/// @brief 行列のQR分解を行う
//! 
//! \f[ {\bf Q} = {\bf H}_1 \; {\bf H}_2 \; \cdots \; {\bf H}_k \f]
//! \f[ {\bf H}_i = {\bf I} - tau \cdot \mbox{\boldmath v} \mbox{\boldmath v}^T \f]
//! 
//! @param[in]  a     … 入力行列
//! @param[out] tau   … 基本反射の係数ベクトル
//! @param[in]  style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return QR分解された結果
//! 
template < class T, class Allocator >
const matrix< T, Allocator > qr_factorization( const matrix< T, Allocator > &a, matrix< T, Allocator > &tau, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	return( __qr__::__qr__< __numeric__::is_complex< T >::value >::qr_factorization( a_, tau, style ) );
}

/// @brief 行列のQR分解を行う
//! 
//! @param[in]  a     … 入力行列
//! @param[in]  style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return QR分解された結果
//! 
template < class T, class Allocator >
const matrix< T, Allocator > qr_factorization( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	typedef __clapack__::integer integer;
	matrix< T, Allocator > a_( a ), tau( a.rows( ), a.cols( ) );
	return( __qr__::__qr__< __numeric__::is_complex< T >::value >::qr_factorization( a_, tau, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief 行列のQR分解を行う
//! 
//! @param[in]  expression … 複数の行列演算を表す式
//! @param[out] tau        … 基本反射の係数ベクトル
//! @param[in]  style      … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return QR分解された結果
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

/// @brief 行列のQR分解を行う
//! 
//! @param[in]  expression … 複数の行列演算を表す式
//! @param[in]  style      … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return QR分解された結果
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





/// @brief 行列の逆行列をLU分解を用いて計算する
//! 
//! \f[ {\bf A}^{-1} \f]
//! 
//! @param[in] a     … 入力行列
//! @param[in] style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return 逆行列 \f$ {\bf A}^{-1} \f$
//! 
template < class T, class Allocator >
matrix< T, Allocator > inverse( const matrix< T, Allocator > &a, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	return( __inverse__::__inverse__< __numeric__::is_complex< T >::value >::inverse( a_, style ) );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief 行列の逆行列をLU分解を用いて計算する
//! 
//! @param[in] expression … 複数の行列演算を表す式
//! @param[in] style      … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return 逆行列 \f$ {\bf A}^{-1} \f$
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





/// @brief 行列の固有値・固有ベクトルを計算する
//! 
//! \f[ {\bf A}\mbox{\boldmath x} = \lambda\mbox{\boldmath x} \f]
//! 
//! 計算結果は，_DESCENDING_ORDER_EIGEN_VALUE_ の値によって，固有値が昇順・降順のどちらかで並ぶように変換される
//! 
//! @param[in]  a            … 入力行列 \f${\bf A}\f$
//! @param[out] eigen_value  … 固有値が昇順・降順のどちらかで入ったベクトル \f$\lambda\f$
//! @param[out] eigen_vector … 固有値の昇順・降順のどちらかに対応し，左から固有ベクトルが並んだ行列 \f$\mbox{\boldmath x}\f$
//! @param[in]  style        … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return 固有値が並んだベクトル（eigen_valueと同じ） \f$\lambda\f$
//! 
template < class T, class Allocator >
const matrix< T, Allocator >& eigen( const matrix< T, Allocator > &a, matrix< T, Allocator > &eigen_value, matrix< T, Allocator > &eigen_vector, matrix_style::style style = matrix_style::ge )
{
	matrix< T, Allocator > a_( a );
	__eigen__::__eigen__< __numeric__::is_complex< T >::value >::eigen( a_, eigen_value, eigen_vector, style );

#if defined( _DESCENDING_ORDER_EIGEN_VALUE_ ) && _DESCENDING_ORDER_EIGEN_VALUE_ == 1
	// 固有値が大きい順に並んでいない場合は，並び替える
	if( __clapack__::get_real( eigen_value[ 0 ] ) < __clapack__::get_real( eigen_value[ eigen_value.size( ) - 1 ] ) )
#else
	// 固有値が小さい順に並んでいない場合は，並び替える
	if( __clapack__::get_real( eigen_value[ 0 ] ) > __clapack__::get_real( eigen_value[ eigen_value.size( ) - 1 ] ) )
#endif
	{
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::value_type value_type;

		difference_type i = 0, j = eigen_value.size( ) - 1;
		value_type v;
		while( i < j )
		{
			v = eigen_value[ i ];
			eigen_value[ i ] = eigen_value[ j ];
			eigen_value[ j ] = v;

			for( size_type r = 0 ; r < eigen_vector.rows( ) ; r++ )
			{
				v = eigen_vector( r, i );
				eigen_vector( r, i ) = eigen_vector( r, j );
				eigen_vector( r, j ) = v;
			}
			i++;
			j--;
		}
	}

	return( eigen_value );
}

#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief 行列の固有値・固有ベクトルを計算する
//! 
//! \f[ {\bf A}\mbox{\boldmath x} = \lambda\mbox{\boldmath x} \f]
//! 
//! 計算結果は，_DESCENDING_ORDER_EIGEN_VALUE_ の値によって，固有値が昇順・降順のどちらかで並ぶように変換される
//! 
//! @param[in]  expression   … 複数の行列演算を表す式 \f${\bf A}\f$
//! @param[out] eigen_value  … 固有値が昇順・降順のどちらかで入ったベクトル \f$\lambda\f$
//! @param[out] eigen_vector … 固有値の昇順・降順のどちらかに対応し，左から固有ベクトルが並んだ行列 \f$\mbox{\boldmath x}\f$
//! @param[in]  style        … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return 固有値が並んだベクトル（eigen_valueと同じ） \f$\lambda\f$
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


/// @brief 行列の特異値分解を計算する
//! 
//! \f[ {\bf A} = {\bf U}{\bf \Sigma}{\bf V}^T \f]
//! 
//! @note 対角行列の成分は，左上から値の大きい順に並ぶ
//! 
//! @param[in]  a     … 入力行列 \f$ {\bf A} \f$
//! @param[out] u     … 列直行行列 \f$ {\bf U} \f$
//! @param[out] s     … 対角行列 \f$ {\bf \Sigma} \f$
//! @param[out] vt    … 直行行列の転置 \f$ {\bf V}^T \f$
//! @param[in]  style … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return 対角行列 \f$ {\bf \Sigma} \f$
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
const matrix< T2, Allocator2 >& svd( const matrix< T1, Allocator1 > &a, matrix< T1, Allocator1 > &u, matrix< T2, Allocator2 > &s, matrix< T1, Allocator1 > &vt, matrix_style::style style = matrix_style::ge )
{
	matrix< T1, Allocator1 > a_( a );
	return( __svd__::__svd__< __numeric__::is_complex< T1 >::value >::svd( a_, u, s, vt, style ) );
}


#if _USE_EXPRESSION_TEMPLATE_ != 0

/// @brief 関数・クラスの概要を書く
//! 
//! \f[ {\bf A} = {\bf U}{\bf \Sigma}{\bf V}^T \f]
//! 
//! @note 対角行列の成分は，左上から値の大きい順に並ぶ
//! 
//! @param[in]  expression … 複数の行列演算を表す式 \f$ {\bf A} \f$
//! @param[out] u          … 列直行行列 \f$ {\bf U} \f$
//! @param[out] s          … 対角行列 \f$ {\bf \Sigma} \f$
//! @param[out] vt         … 直行行列の転置 \f$ {\bf V}^T \f$
//! @param[in]  style      … 入力行列の形式（デフォルトは一般行列を指定）
//!
//! @return 対角行列 \f$ {\bf \Sigma} \f$
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


/// @}
//  行列演算グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_NUMERIC__
