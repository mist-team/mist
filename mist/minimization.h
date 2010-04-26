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

/// @file mist/minimization.h
//!
//! @brief 関数の最小化を扱うためのライブラリ
//!
//! - 関数の最小化
//!   - W. H. Press, S. A. Teukolsky, W. T. Vetterling, and B. P. Flannery, ``Numerical Recipes in C, The Art of Scientific Computing Second Edition,'' Cambridge University Press, pp. 321--336, 1999.
//!   - Richard P. Brent, ``Algorithms for Minimization Without Derivatives", DOVER PUBLICATIONS, Mineola, New York.
//!   - F. V. Berghen, H. Bersini, ``CONDOR, a new parallel, constrained extension of Powell's UOBYQA algorithm: Experimental results and comparison with the DFO algorithm,'' Journal of Computational and Applied Mathematics, Elsevier, Volume 181, Issue 1, September 2005, pp. 157--175 
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
#include <vector>
#include <algorithm>

// mist名前空間の始まり
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
				// 勾配方向ベクトルの正規化
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

		// もっとも長い対角方向の長さを取得し，バウンディングボックスの中央を求める
		for( r = 0 ; r < box.rows( ) ; r++ )
		{
			double l = ( box( r, 0 ) - box( r, 1 ) );
			infinity += l * l;
			offset[ r ] = ( box( r, 0 ) + box( r, 1 ) ) / 2.0;
		}

		infinity = std::sqrt( infinity );

		// まず，十分に遠い点を設定する
		p1 = p - dir * infinity - offset;
		p2 = p + dir * infinity - offset;

		bool flag = true;

		for( r = 0 ; r < box.rows( ) ; r++ )
		{
			// まず，下限をチェックする
			flag = flag && clipping( p1, p2, p1[ r ], p2[ r ], std::abs( box( r, 0 ) - offset[ r ] ) );

			// 次に，上限をチェックする
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

		// もっとも長い対角方向の長さを取得し，バウンディングボックスの中央を求める
		for( size_type r = 0 ; r < p1.size( ) ; r++ )
		{
			l1 += ( p[ r ] - p1[ r ] ) * ( p[ r ] - p1[ r ] );
			l2 += ( p[ r ] - p2[ r ] ) * ( p[ r ] - p2[ r ] );
		}

		// 数値計算誤差の関係で，バウンディングボックスを越えるのを回避する
		l1 = - std::sqrt( l1 ) + 0.000000000001;
		l2 = + std::sqrt( l2 ) - 0.000000000001;

		//std::cout << ( p + l1 * dir ).t( ) << std::endl;
		//std::cout << ( p + l2 * dir ).t( ) << std::endl;


		return( l1 < 0.0 || l2 > 0.0 );
	}

}



//! @addtogroup minimization_group 関数の最小化
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/minimization.h>
//! @endcode
//!
//!  @{



/// @brief f(x)の一次元探索において，極小値を与える x を囲む区間を決定する
//! 
//! @f$ a \ge b \ge c @f$ もしくは @f$ c \ge b \ge a @f$ 内で極小値を持つ区間を決定する．
//!
//! @attention 入力となる a, b は異なる値を設定する必要あり
//! @attention 評価関数が直線などで極小値を持たない場合は，区間を決定できない
//! 
//! @param[in,out] a  … 区間の左端
//! @param[in,out] b  … 区間の真ん中
//! @param[out]    c  … 区間の右端
//! @param[out]	   fa … 評価関数値 @f$ f(a) @f$
//! @param[out]    fb … 評価関数値 @f$ f(b) @f$
//! @param[out]    fc … 評価関数値 @f$ f(c) @f$
//! @param[in]     f  … 評価関数
//! @param[in]     max_iterations … 最大反復回数
//!
//! @return 極小を囲む区間を見つけることができたかどうか
//! 
template < class Functor >
bool enclose( double &a, double &b, double &c, double &fa, double &fb, double &fc, Functor f, size_t max_iterations = 100 )
{
	const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;
	const double _1_gold = 1.0 / gold;
	const double dust = type_limits< double >::tiny( );		// ゼロ除算を避けるためのゴミ値
	const double limit = 100.0;								// 放物線補外を適用する区間の最大更新幅

	if( a == b )
	{
		b = a + 1;
	}

	fa = f( a );
	fb = f( b );

	// fa < fb の場合は，aの方向に極小があると思われるので，aとbを入れ替える
	if( fa < fb )
	{
		double tmp = a;
		a = b;
		b = tmp;
		tmp = fa;
		fa = fb;
		fb = tmp;
	}

	// 黄金分割比を用いて，次に探索する点cを決定する
	c = a + _1_gold * ( b - a );
	fc = f( c );

	// 区間の更新が不可能な場合の判定
	if( fa == fb && fb == fc )
	{
		return( false );
	}

	// f( a ) > f( b ) < f( c ) となるまで，区間の更新を続ける
	// また，最大反復回数を超えた場合も終了する
	size_t ite = 0;
	while( fb > fc && ite++ < max_iterations )
	{
		// a, b, c, fa, fb, fc の値に，放物線を当てはめて極小が存在する位置を計算する
		double ba = b - a;
		double cb = c - b;
		double fcb = fc - fb;
		double fba = fb - fa;

		// ゼロ除算を防ぐために，ごみ値を足して，放物線補間を行う
		double l1 = 2.0 * ( cb * fba - ba * fcb );
		double l2 = std::abs( l1 );
		double x = b + ( ba * ba * fcb + cb * cb * fba ) / ( l1 / l2 * ( l2  + dust ) );

		if( ( c - x ) * ( x - b ) > 0 )
		{
			// 補間点 x が区間 b < x < c を満たしていて，極小をはさむ区間を見つけることに成功
			double fx = f( x );
			if( fx < fc )
			{
				// 区間 ( b, c ) に極小が存在する
				a = b;
				b = x;
				fa = fb;
				fb = fx;
			}
			else
			{
				// 区間 ( a, x ) に極小が存在する
				c = x;
				fc = fx;
			}

			// 区間を囲い込むことに成功したので終了する
			break;
		}
		else if( ( b + limit * cb - x ) * ( x - c ) > 0 )
		{
			// 補間点 x が区間 b < c < x を満たしていて，許容範囲内に補間点が存在する
			double fx = f( x );

			if( fx < fc )
			{
				// 現在の端である fc よりも関数値が小さい場合は，区間を更新する
				// a <- b, b <- x てする
				a = b;
				b = x;
				fa = fb;
				fb = fx;

				// 新しい区間点 c を黄金分割比を用いて求める
				c = a + _1_gold * ( b - a );
				fc = f( c );
			}
			else
			{
				// b < c < x かつ fc < fx のため，区間の囲い込みに成功
				// 区間 ( b, x ) に極小が存在する
				a = b;
				b = c;
				c = x;
				fa = fb;
				fb = fc;
				fc = fx;
			}
		}
		else
		{
			// 補間点 x が区間 b < c < x を満たしているが，余りに補間点が遠すぎるので黄金分割する
			// a <- b, b <- c として，新しい点 c を再計算する
			a = b;
			b = c;
			c = a + _1_gold * ( b - a );
			fa = fb;
			fb = fc;
			fc = f( c );
		}
	}

	return( ite < max_iterations );
}


/// @brief 黄金分割を用いた極小値の1次元探索（1次収束）
namespace gold
{
	/// @brief 黄金分割を利用して，f(x)の極小値の一次元探索を行う（1次収束）
	//!
	//! 初期の極小を含むと思われる区間 [a, b] を適切に設定することで，収束を早めることができる
	//!
	//! @note 指定された区間 [a, b] 内での極小値を探索する
	//! @note 区間内に極小が存在しない場合は，区間内で最小となる位置を出力する
	//! @note use_enclose を真に指定した場合は，最初に区間を拡張して極小を探索する（本ヘッダ内の enclose 関数を利用する）
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[out] iterations     … 実際の反復回数
	//! @param[in]  max_iterations … 最大反復回数
	//! @param[in]  use_enclose    … 極小をはさむ区間の更新を行うかどうか
	//!
	//! @return 極小を与える座標値における評価値
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
			if( !enclose( a, p, b, fa, fp, fb, f ) )
			{
				return( fp );
			}

			// a <= b <= c となるように区間を変更する
			if( a > b )
			{
				double tmp = a;
				a = b;
				b = tmp;
				tmp = fa;
				fa = fb;
				fb = tmp;
			}

			// 区間の長いほうを決め，黄金分割によりもう一つの点を決定する
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
				// 区間 p < f( x ) < c の間に最小値が存在する
				a = p;
				p = q;
				q = p + gold * ( b - p );
				fp = fq;
				fq = f( q );
			}
			else
			{
				// 区間 a < f( x ) < q の間に最小値が存在する
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


	/// @brief 黄金分割を利用して，f(x)の極小値の一次元探索を行う（1次収束）
	//! 
	//! 初期の極小を含むと思われる区間 [a, b] を適切に設定することで，収束を早めることができる
	//! 
	//! @note 指定された区間 [a, b] 内での極小値を探索する
	//! @note 区間内に極小が存在しない場合は，区間内で最小となる位置を出力する
	//! @note use_enclose を真に指定した場合は，最初に区間を拡張して極小を探索する（本ヘッダ内の enclose 関数を利用する）
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[in]  max_iterations … 最大反復回数，実際の反復回数
	//! @param[in]  use_enclose    … 極小をはさむ区間の更新を行うかどうか
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 1000, bool use_enclose = true )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( a, b, x, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations, use_enclose ) );
	}
}



/// @brief Brent の放物線補間を用いた極小値の1次元探索（2次収束）
namespace brent
{
	/// @brief Brent の放物線補間を用いて，指定された区間内の f(x) の極小値の一次元探索を行う（2次収束）
	//! 
	//! - 参考文献
	//!   - Richard P. Brent, "Algorithms for Minimization Without Derivatives", DOVER PUBLICATIONS, Mineola, New York
	//! 
	//! @note 指定された区間 [a, b] 内での極小値を探索する
	//! @note 区間内に極小が存在しない場合は，区間内で最小となる位置を出力する
	//! @note use_enclose を真に指定した場合は，最初に区間を拡張して極小を探索する（本ヘッダ内の enclose 関数を利用する）
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[out] iterations     … 実際の反復回数
	//! @param[in]  max_iterations … 最大反復回数
	//! @param[in]  use_enclose    … 極小をはさむ区間の更新を行うかどうか
	//!
	//! @return 極小を与える座標値における評価値
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t &iterations, size_t max_iterations, bool use_enclose = true )
	{
		double u, v, w, fu, fv, fw, fx, e = 0.0, d = 0.0;
		const double c = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;

		if( use_enclose )
		{
			double fa, fb;
			x = b;
			if( !enclose( a, x, b, fa, fx, fb, f ) )
			{
				return( fx );
			}

			// a <= x <= b で極小を囲う区間に変更する
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
			// a <= x <= b で極小を囲う区間に変更する
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

			// 最後に判定した最小値候補点と，区間 [a, b] の中間との差が許容誤差内になったら終了する
			if( std::abs( x - m ) <= t2 - 0.5 * ( b - a ) )
			{
				break;
			}

			double p = 0.0, q = 0.0, r = 0.0;

			if( std::abs( e ) > tol )
			{
				// 放物線補間を行う
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

			if( std::abs( p ) < std::abs( 0.5 * q * r ) && p > q * ( a - x ) && p < q * ( b - x ) )
			//if( std::abs( p ) < std::abs( 0.5 * q * r ) && p < q * ( a - x ) && p < q * ( b - x ) )
			{
				// 放物線補間が適切に行われたので区間を更新する
				d = p / q;
				u = x + d;

				if( u - a < t2 || b - u < t2 )
				{
					d = x < m ? tol : -tol;
				}
			}
			else
			{
				// 放物線補間は不適切なので黄金分割する
				// 区間の大きいほうを黄金分割する
				e = ( x < m ? b : a ) - x;
				d = c * e;
			}

			u = x + ( std::abs( d ) >= tol ? d : ( d > 0 ? tol : -tol ) );
			fu = f( u );

			if( fu <= fx )
			{
				// より小さい値が見つかったので a, b, v, w を更新する
				if( u < x )
				{
					// 区間 a < u < x に極小値がある
					b = x;
				}
				else
				{
					// 区間 x < u < b に極小値がある
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
				// より大きい値が見つかったので a, b を更新する
				if( u < x )
				{
					// 区間 u < x < b に極小値がある
					a = u;
				}
				else
				{
					// 区間 a < x < u に極小値がある
					b = u;
				}

				// 新しく評価した点と従来評価した点の大小関係を調べる
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


	/// @brief Brent の放物線補間を用いて，f(x)の極小値の一次元探索を行う（2次収束）
	//! 
	//! 初期の極小を含むと思われる区間 [a, b] を適切に設定することで，収束を早めることができる
	//! 
	//! @note 指定された区間 [a, b] 内での極小値を探索する
	//! @note 区間内に極小が存在しない場合は，区間内で最小となる位置を出力する
	//! @note use_enclose を真に指定した場合は，最初に区間を拡張して極小を探索する（本ヘッダ内の enclose 関数を利用する）
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[in]  max_iterations … 最大反復回数
	//! @param[in]  use_enclose    … 極小をはさむ区間の更新を行うかどうか
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 1000, bool use_enclose = true )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( a, b, x, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations, use_enclose ) );
	}
}


/// @brief 勾配関数をユーザーが定義する最急降下法（勾配を用いた多変数関数の極小値の探索）
namespace gradient_with_vector
{
	/// @brief 探索の開始点を指定し，ユーザーが指定した勾配計算関数を用いて最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     f              … 評価関数
	//! @param[in]     g              … 勾配関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, Functor1 f, Functor2 g, double tolerance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x, err, old_err = f( p );

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor1 > functor( p, dir, tmp, f );

		size_t ite, i;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// 勾配方向を計算する
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				// 勾配方向を計算する
				dir[ i ] = g( p[ i ], i );
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// 勾配方向ベクトルの正規化
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// 勾配の計算ができなくなったので終了する
				break;
			}

			// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
			err = brent::minimization( 0.0, 1.0, x, functor, tolerance, 1000, true );

			//std::cout << p.t( ) << ", " << dir.t( ) << std::endl;

			if( 2.0 * std::abs( err - old_err ) <= tolerance * ( std::abs( err ) + std::abs( old_err ) ) || old_err <= err )
			{
				// 前回の最小化の結果からの変化量が、許容誤差内であったので終了する
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


	/// @brief 探索の開始点を指定し，ユーザーが指定した勾配計算関数を用いて最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     bound          … 探索に用いる各要素の探索範囲
	//! @param[in]     f              … 評価関数
	//! @param[in]     g              … 勾配関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor1 f, Functor2 g, double tolerance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x, err, old_err = f( p );

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor1 > functor( p, dir, tmp, f );

		size_t ite, i;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// 勾配方向を計算する
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				// 勾配方向を計算する
				dir[ i ] = g( p[ i ], i );
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// 勾配方向ベクトルの正規化
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// 勾配の計算ができなくなったので終了する
				break;
			}

			double l1, l2;
			if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
			{
				// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
				err = brent::minimization( l1, l2, x, functor, tolerance, 1000, false );
			}

			//std::cout << p.t( ) << ", " << dir.t( ) << std::endl;


			// 相対誤差を用いた収束判定
			if( 2.0 * std::abs( err - old_err ) <= tolerance * ( std::abs( err ) + std::abs( old_err ) ) || old_err <= err )
			{
				// 前回の最小化の結果からの変化量が、許容誤差内であったので終了する
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


	/// @brief 探索の開始点を指定し，ユーザーが指定した勾配計算関数を用いて最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     bound          … 探索に用いる各要素の探索範囲
	//! @param[in]     f              … 評価関数
	//! @param[in]     g              … 勾配関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor1 f, Functor2 g, double tolerance, size_t max_iterations = 1000 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor1 > __no_copy_constructor_functor1__;
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor2 > __no_copy_constructor_functor2__;
		size_t itenum = 0;
		return( minimization( p, bound, __no_copy_constructor_functor1__( f ), __no_copy_constructor_functor2__( g ), tolerance, itenum, max_iterations ) );
	}


	/// @brief 探索の開始点を指定し，ユーザーが指定した勾配計算関数を用いて最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     f              … 評価関数
	//! @param[in]     g              … 勾配関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor1, class Functor2 >
	double minimization( matrix< T, Allocator > &p, Functor1 f, Functor2 g, double tolerance, size_t max_iterations = 1000 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor1 > __no_copy_constructor_functor1__;
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor2 > __no_copy_constructor_functor2__;
		size_t itenum = 0;
		return( minimization( p, __no_copy_constructor_functor1__( f ), __no_copy_constructor_functor2__( g ), tolerance, itenum, max_iterations ) );
	}
}


/// @brief 最急降下法（勾配を用いた多変数関数の極小値の探索）
namespace gradient
{

	/// @brief 探索の開始点を指定し，勾配を計算しながら最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     distance       … 勾配を計算する際の幅
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp = p;
		double x, v1, v2, err = 1.0e100, old_err = f( p );
		size_type i;

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ;  )
		{
			// 勾配方向を計算する
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				tmp[ i ] = p[ i ] + distance;
				v1 = f( tmp );

				tmp[ i ] = p[ i ] - distance;
				v2 = f( tmp );

				// 元に戻す
				tmp[ i ] = p[ i ];

				dir[ i ] = v2 - v1;
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// 勾配方向ベクトルの正規化
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// 勾配の計算ができなくなったので終了する
				break;
			}

			// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
			err = brent::minimization( 0.0, 1.0, x, functor, tolerance, 1000, true );

			//std::cout << err << ", " << p.t( ) << ", " << dir.t( ) << std::endl;

			ite++;

			if( ite > max_iterations || 2.0 * std::abs( old_err - err ) < tolerance * ( std::abs( old_err ) + std::abs( err ) ) || err >= old_err )
			{
				// 前回の最小化の結果からの変化量が、許容誤差内であったので終了する
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


	/// @brief 探索の開始点を指定し，勾配を計算しながら最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     bound          … 探索に用いる各要素の探索範囲
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     distance       … 勾配を計算する際の幅
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp = p;
		double x = 0.0, v1, v2, err = 1.0e100, old_err = f( p );
		size_type i;

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; )
		{
			// 勾配方向を計算する
			double len = 0.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				tmp[ i ] = p[ i ] + distance;
				v1 = f( tmp );

				tmp[ i ] = p[ i ] - distance;
				v2 = f( tmp );

				// 元に戻す
				tmp[ i ] = p[ i ];

				dir[ i ] = v2 - v1;
				len += dir[ i ] * dir[ i ];
			}

			if( len > 0 )
			{
				// 勾配方向ベクトルの正規化
				len = std::sqrt( len );
				for( i = 0 ; i < dir.size( ) ; i++ )
				{
					dir[ i ] /= len;
				}
			}
			else
			{
				// 勾配の計算ができなくなったので終了する
				break;
			}

			double l1 = 0.0, l2 = 0.0;
			if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
			{
				// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
				err = brent::minimization( l1, l2, x, functor, tolerance, 1000, false );
			}

			//std::cout << err << ", " << p.t( ) << ", " << dir.t( ) << std::endl;

			ite++;

			if( ite > max_iterations || 2.0 * std::abs( old_err - err ) < tolerance * ( std::abs( old_err ) + std::abs( err ) ) )
			{
				// 前回の最小化の結果からの変化量が、許容誤差内であったので終了する
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



	/// @brief 探索の開始点を指定し，勾配を計算しながら最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     distance       … 勾配を計算する際の幅
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance = 1.0, size_t max_iterations = 1000 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, __no_copy_constructor_functor__( f ), tolerance, distance, itenum, max_iterations ) );
	}


	/// @brief 探索の開始点を指定し，勾配を計算しながら最小値を探索する
	//! 
	//! 探索の開始点を指定しその位置での勾配方向に向かった最小化を繰り返し，最小値を探索する
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in]     bound          … 探索に用いる各要素の探索範囲
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     distance       … 勾配を計算する際の幅
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor f, double tolerance, double distance = 1.0, size_t max_iterations = 1000 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, bound, __no_copy_constructor_functor__( f ), tolerance, distance, itenum, max_iterations ) );
	}
}


/// @brief Powell法（方向集合を用いた多変数関数の極小値の探索）
namespace powell
{
	/// @brief Powell 法による多次元変数による極小値の探索を行う
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dirs           … 探索に用いる方向集合
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type odirs( dirs ), dir( p.size( ), 1 ), tmp( p.size( ), 1 ), p0( p ), pn( p );
		double x, fp = f( p ), fp0 = fp, delta;

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		size_type r, c;
		for( ite = 0 ; ite < max_iterations ; ite++ )
		{
			// 探索開始前の評価値を覚えておく
			fp0 = fp;
			delta = -1.0;
			size_type index = 0;

			for( c = 0 ; c < dirs.cols( ) ; c++ )
			{
				// 探索に用いる方向集合をコピーする
				for( r = 0 ; r < dirs.rows( ) ; r++ )
				{
					dir[ r ] = dirs( r, c );
				}

				double old_fp = fp;

				// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
				fp = brent::minimization( 0.0, 1.0, x, functor, tolerance, 1000, true );

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

			// 相対誤差を用いた収束判定
			if( 2.0 * std::abs( fp - fp0 ) <= tolerance * ( std::abs( fp ) + std::abs( fp0 ) ) || fp0 <= fp )
			{
				break;
			}

			// Acton の方法を用いて，新しい方向集合を求める
			if( ite < max_iterations )
			{
				// 新しい方向を求める
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					dir[ r ] = p[ r ] - p0[ r ];
					pn[ r ]  = 2.0 * p[ r ] - p0[ r ];
				}

				double fe = f( pn );

				if( fe < fp0 )
				{
					// 現在の方向集合を更新する
					double tmp = fp0 - fp - delta;
					double ttt = 2.0 * ( fp0 - 2.0 * fp + fe ) * tmp * tmp - delta * ( fp0 - fe ) * ( fp0 - fe );
					if( ttt < 0 )
					{
						// Brent の2次収束アルゴリズムを用いて，新しい dir 方向への最小化を行う
						fp = brent::minimization( 0.0, 1.0, x, functor, tolerance, 1000, true );

						// 現在のパラメータ更新
						for( r = 0 ; r < p.size( ) ; r++ )
						{
							p[ r ] += dir[ r ] * x;
						}

						// 方向集合の一番最後に，新しい方向を追加する
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

				// 新しい方向を求める
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					p0[ r ]  = p[ r ];
				}
			}
		}

		iterations = ite;

		return( fp );
	}


	/// @brief Powell 法による多次元変数による極小値の探索を行う
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dirs           … 探索に用いる方向集合
	//! @param[in,out] bound          … 探索に用いる各要素の探索範囲
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, matrix< T, Allocator > &bound, Functor f, double tolerance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 ), p0( p ), pn( p );
		double x = 0.0, fp0 = 1.0e100, fp = f( p ), delta;
		double l1 = 0.0, l2 = 0.0;

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		size_t ite;
		size_type r, c;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// 探索開始前の評価値を覚えておく
			fp0 = fp;
			delta = 0.0;
			size_type index = 0;

			for( c = 0 ; c < dirs.cols( ) ; c++ )
			{
				// 探索に用いる方向集合をコピーする
				for( r = 0 ; r < dirs.rows( ) ; r++ )
				{
					dir[ r ] = dirs( r, c );
				}

				double old_fp = fp;


				if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
				{
					// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
					double nfp = brent::minimization( l1, l2, x, functor, tolerance, 1000, false );

					// 関数値が減少した場合のみ値を更新する
					if( nfp < fp )
					{
						fp = nfp;

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
				}
			}

			// 相対誤差を用いた収束判定
			if( 2.0 * std::abs( fp - fp0 ) <= tolerance * ( std::abs( fp ) + std::abs( fp0 ) ) )
			{
				break;
			}

			// Acton の方法を用いて，新しい方向集合を求める
			if( ite <= max_iterations )
			{
				// 新しい方向を求める
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					dir[ r ] = p[ r ] - p0[ r ];
					pn[ r ]  = 2.0 * p[ r ] - p0[ r ];
				}

				double fe = f( pn );

				if( fe < fp0 )
				{
					// 現在の方向集合を更新する
					double tmp = fp0 - fp - delta;
					double ttt = 2.0 * ( fp0 - 2.0 * fp + fe ) * tmp * tmp - delta * ( fp0 - fe ) * ( fp0 - fe );
					if( ttt < 0 )
					{
						if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
						{
							// Brent の2次収束アルゴリズムを用いて，新しい dir 方向への最小化を行う
							double nfp = brent::minimization( l1, l2, x, functor, tolerance, 1000, false );

							// 関数値が減少した場合のみ値を更新する
							if( nfp < fp )
							{
								fp = nfp;
								for( r = 0 ; r < p.size( ) ; r++ )
								{
									p[ r ] += dir[ r ] * x;
								}
							}
						}

						// 方向集合の一番最後に，新しい方向を追加する
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

				// 新しい方向を求める
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					p0[ r ]  = p[ r ];
				}
			}
		}

		iterations = ite;

		return( fp );
	}

	/// @brief Powell 法による多次元変数による極小値の探索を行う
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dirs           … 探索に用いる方向集合
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t max_iterations = 1000 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, dirs, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations ) );
	}

	/// @brief Powell 法による多次元変数による極小値の探索を行う
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dirs           … 探索に用いる方向集合
	//! @param[in,out] bound          … 探索に用いる各要素の探索範囲
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, matrix< T, Allocator > &bound, Functor f, double tolerance, size_t max_iterations = 1000 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, dirs, bound, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations ) );
	}
}


/// @brief Lucidi法（方向集合を用いた多変数関数の極小値の探索）
namespace lucidi
{
	template < class Functor >
	inline double expantion_step( double alpha, double alpha_max, double delta, double gamma, double fp0, Functor f )
	{
		while( true )
		{
			if( alpha >= alpha_max )
			{
				return( alpha_max );
			}
			else
			{
				double a = alpha / delta;
				if( f( a ) > fp0 - gamma * a * a )
				{
					return( alpha );
				}

				alpha = a;
			}
		}
	}


	/// @brief Lucidi 提案の多変数関数の極小値探索
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dirs           … 探索に用いる方向集合
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 ), p0( p ), a( p.size( ), 1 );
		double fp0 = 1.0e100, fp = f( p );

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, tmp, f );

		const double __alpha__ = 0.5;
		const double theta = 0.5;
		const double gamma = 1.0e-6;
		const double delta = 0.25;
		const double infinity = type_limits< double >::maximum( );

		size_t ite;
		size_type r, c;

		for( r = 0 ; r < a.size( ) ; r++ )
		{
			a[ r ] = __alpha__;
		}

		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// 探索開始前の評価値を覚えておく
			fp0 = fp;

			size_type alpha_count = 0;

			for( c = 0 ; c < dirs.cols( ) ; c++ )
			{
				double alpha = a[ c ];

				// 探索に用いる方向集合をコピーする
				for( r = 0 ; r < dirs.rows( ) ; r++ )
				{
					dir[ r ] = dirs( r, c );
				}

				if( alpha > 0.0 )
				{
					double gaa = gamma * alpha * alpha;
					if( functor( alpha ) <= fp0 - gaa )
					{
						a[ c ] = alpha = expantion_step( alpha, infinity, delta, gamma, fp0, functor );

						// 位置を更新
						for( r = 0 ; r < p.size( ) ; r++ )
						{
							p[ r ] += dir[ r ] * alpha;
						}
					}
					else if( functor( -alpha ) <= fp0 - gaa )
					{
						// 探索に用いる方向集合をコピーする
						for( r = 0 ; r < dirs.rows( ) ; r++ )
						{
							dir[ r ] = dirs( r, c ) = -dir[ r ];
						}

						a[ c ] = alpha = expantion_step( alpha, infinity, delta, gamma, fp0, functor );

						// 位置を更新
						for( r = 0 ; r < p.size( ) ; r++ )
						{
							p[ r ] += dir[ r ] * alpha;
						}
					}
					else
					{
						a[ c ] = theta * alpha;
						alpha = 0.0;
						alpha_count++;
					}
				}
				else
				{
					a[ c ] = theta * alpha;
					alpha = 0.0;
					alpha_count++;
				}
			}

			fp = f( p );

			if( alpha_count < dirs.cols( ) )
			{
				// 相対誤差を用いた収束判定
				if( 2.0 * std::abs( fp - fp0 ) <= tolerance * ( std::abs( fp ) + std::abs( fp0 ) ) )
				{
					break;
				}
			}
		}

		iterations = ite;

		return( fp );
	}


	/// @brief Powell 法による多次元変数による極小値の探索を行う
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dirs           … 探索に用いる方向集合
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t max_iterations = 1000 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, dirs, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations ) );
	}
}




/// @brief CONDOR法（多変数関数の極小値の探索）
//!
//! - 参考文献
//!   - F. V. Berghen, H. Bersini, ``CONDOR, a new parallel, constrained extension of Powell's UOBYQA algorithm: Experimental results and comparison with the DFO algorithm,'' Journal of Computational and Applied Mathematics, Elsevier, Volume 181, Issue 1, September 2005, pp. 157--175 
//!
namespace condor
{
	// CONDORアルゴリズム内部で利用する関数など
	namespace __condor_utility__
	{
		inline double minimum( double v1, double v2 )
		{
			return( v1 < v2 ? v1 : v2 );
		}

		inline double maximum( double v1, double v2 )
		{
			return( v1 > v2 ? v1 : v2 );
		}

		inline double minimum( double v1, double v2, double v3 )
		{
			if( v1 < v2 )
			{
				if( v1 < v3 )
				{
					return( v1 );
				}
				else
				{
					return( v3 );
				}
			}
			else if( v2 < v3 )
			{
				return( v2 );
			}
			else
			{
				return( v3 );
			}
		}

		inline double minimum( double v1, double v2, double v3, double v4 )
		{
			return( minimum( minimum( v1, v2 ), minimum( v3, v4 ) ) );
		}

		inline double maximum( double v1, double v2, double v3 )
		{
			if( v1 > v2 )
			{
				if( v1 > v3 )
				{
					return( v1 );
				}
				else
				{
					return( v3 );
				}
			}
			else if( v2 > v3 )
			{
				return( v2 );
			}
			else
			{
				return( v3 );
			}
		}

		inline double maximum( double v1, double v2, double v3, double v4 )
		{
			return( maximum( maximum( v1, v2 ), maximum( v3, v4 ) ) );
		}

		template < class Matrix >
		inline double frobenius_norm( const Matrix &H )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::size_type size_type;

			double val = 0.0;
			for( size_type r = 0 ; r < H.size( ) ; r++ )
			{
				val += H[ r ] * H[ r ];
			}

			return( std::sqrt( val ) );
		}

		template < class Matrix >
		inline double infinitum_norm( const Matrix &H )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::size_type size_type;

			double max = 0.0;
			for( size_type c = 0 ; c < H.cols( ) ; c++ )
			{
				double val = 0.0;
				for( size_type r = 0 ; r < H.rows( ) ; r++ )
				{
					val += std::abs( H( r, c ) );
				}

				if( max < val )
				{
					max = val;
				}
			}

			return( max );
		}

		template < class Matrix >
		inline double inner_product( const Matrix &m1, const Matrix &m2 )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::value_type value_type;
			typedef typename matrix_type::size_type size_type;
			typedef typename matrix_type::difference_type difference_type;

			double sum = 0.0;
			for( size_type i = 0 ; i < m1.size( ) ; i++ )
			{
				sum += m1[ i ] * m2[ i ];
			}

			return( sum );
		}

		template < class Matrix >
		inline double inner_product( const Matrix &m1, const Matrix &H, const Matrix &m2 )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::size_type size_type;

			double sum = 0.0;
			for( size_type c = 0 ; c < H.cols( ) ; c++ )
			{
				double val = 0.0;
				for( size_type r = 0 ; r < H.rows( ) ; r++ )
				{
					val += m1[ r ] * H( r, c );
				}

				sum += val * m2[ c ];
			}
			return( sum );
		}

		template < class Matrix >
		inline double inner_product( const Matrix &m1, const Matrix &H, const Matrix &m2, double lambda )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::size_type size_type;

			double sum = 0.0;
			for( size_type c = 0 ; c < H.cols( ) ; c++ )
			{
				double val = lambda * m1[ c ];
				for( size_type r = 0 ; r < H.rows( ) ; r++ )
				{
					val += m1[ r ] * H( r, c );
				}

				sum += val * m2[ c ];
			}
			return( sum );
		}

		struct __index_value_pair__
		{
			size_t index;
			double value;

			__index_value_pair__( size_t indx, double val ) : index( indx ), value( val ){ }

			bool operator <( const __index_value_pair__ &v ) const
			{
				// 距離値の大きい順に並ぶようにする
				return( value > v.value );
			}
		};

		template < class Matrix >
		inline void solve( const Matrix &A, Matrix &b )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::size_type size_type;
			typedef typename matrix_type::difference_type difference_type;

			for( size_type r = 0 ; r < A.rows( ) ; r++ )
			{
				double sum = b[ r ];
				for( difference_type c = r - 1 ; c >= 0 ; c-- )
				{
					sum -= A( r, c ) * b[ c ];
				}

				b[ r ] = sum / A( r, r );
			}
		}

		template < class Matrix >
		inline void solve_( const Matrix &A, Matrix &b )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::size_type size_type;
			typedef typename matrix_type::difference_type difference_type;

			for( difference_type c = A.cols( ) - 1 ; c >= 0 ; c-- )
			{
				double sum = b[ c ];
				for( size_type r = c + 1 ; r < A.rows( ) ; r++ )
				{
					sum -= A( r, c ) * b[ r ];
				}

				b[ c ] = sum / A( c, c );
			}
		}


		template < class Matrix >
		inline bool cholesky_factorization( const Matrix &H, Matrix &L, double lambda, double &lambda_modified )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::value_type value_type;
			typedef typename matrix_type::size_type size_type;
			typedef typename matrix_type::difference_type difference_type;

			L.fill( 0 );
			for( size_type r = 0 ; r < H.rows( ) ; r++ )
			{
				double scale = H( r, r ) + lambda;

				for( size_type c = 0 ; c < r ; c++ )
				{
					scale -= L( r, c ) * L( r, c );
				}

				if( scale <= 0 ) 
				{
					// Rayleigh quotient trick を使ってλの補正値を計算する
					// CONDORの論文の 4.8 章
					matrix_type v( L.rows( ), 1 );
					v[ r ] = 1.0;
					for( difference_type j = r - 1 ; j >= 0 ; j-- )
					{
						double sum = 0.0;
						for( size_type i = j + 1 ; i <= r ; i++ )
						{
							sum += L( i, j ) * v[ i ];
						}
						v[ j ] = - sum / L( j, j );
					}

					lambda_modified = - scale / frobenius_norm( v );

					return( false );
				}

				scale = std::sqrt( scale );
				L( r, r ) = scale;

				for( size_type c = r + 1 ; c < H.cols( ) ; c++ )
				{
					double val = H( r, c );
					for( size_type l = 0 ; l < r ; l++ )
					{
						val -= L( c, l ) * L( r, l );
					}

					L( c, r ) = val / scale;
				}
			}

			return( true );
		}

		template < class Matrix >
		inline bool cholesky_factorization( const Matrix &H, Matrix &L, double lambda )
		{
			double dmy;
			return( cholesky_factorization( H, L, lambda, dmy ) );
		}

		template < class Matrix >
		inline bool compute_eigen_vector( const Matrix &L, Matrix &w, double lambda )
		{
			typedef Matrix matrix_type;
			typedef typename matrix_type::value_type value_type;
			typedef typename matrix_type::size_type size_type;
			typedef typename matrix_type::difference_type difference_type;

			w.resize( L.rows( ), 1 );
			for( size_type r = 0 ; r < L.rows( ) ; r++ )
			{
				if( L( r, r ) >= 0.0 )
				{
					w[ r ] = 1.0;
				}
				else
				{
					w[ r ] = -1.0;
				}
			}

			solve_( L, w );
			w *= 1.0 / frobenius_norm( w );

			return( true );
		}

		class polynomial : public matrix< double >
		{
		public:
			typedef matrix< double > base;
			typedef matrix< double > matrix_type;
			typedef matrix_type::value_type value_type;
			typedef matrix_type::size_type size_type;
			typedef matrix_type::difference_type difference_type;
			typedef matrix< difference_type > imatrix_type;

		private:
			size_type dimension;
			size_type N;
			imatrix_type alpha;
			imatrix_type alpha_;
			imatrix_type tr;
			imatrix_type tc;
			std::vector< double > r;

		public:
			polynomial( ) : dimension( 0 ), N( 1 )
			{
			}

			polynomial( size_type ndim ) : base( ( ndim + 1 ) * ( ndim + 2 ) / 2, 1 ), dimension( ndim ), N( ( ndim + 1 ) * ( ndim + 2 ) / 2 ), alpha( N, ndim ), alpha_( N, ndim ), tr( N, 1 ), tc( N, 1 ), r( ndim )
			{
				// 多項式補間を行うためのデータを生成する
				compute_polynomial_indeces( );
			}

			polynomial( const polynomial &poly ) : base( poly ), dimension( poly.dimension ), N( poly.N ), alpha( poly.alpha ), alpha_( poly.alpha_ ), tr( poly.tr ), tc( poly.tc ), r( poly.r )
			{
			}

			void reinitialize_polynomial( size_type ndim )
			{
				// 多項式を計算するためのデータをコピーする
				dimension = ndim;
				N = ( ndim + 1 ) * ( ndim + 2 ) / 2;
				alpha.resize( N, ndim );
				alpha_.resize( N, ndim );
				tr.resize( N, 1 );
				tc.resize( N, 1 );
				r.resize( ndim );

				base::resize( N, 1 );

				// 多項式補間を行うためのデータを生成する
				compute_polynomial_indeces( );
			}

			const polynomial & operator =( const polynomial &poly )
			{
				if( &poly == this )
				{
					return( *this );
				}

				// 基底クラスのデータをコピーする
				base::operator =( poly );

				// 多項式を計算するためのデータをコピーする
				dimension = poly.dimension;
				N = poly.N;
				alpha = poly.alpha;
				alpha_ = poly.alpha_;
				tr = poly.tr;
				tc = poly.tc;
				r = poly.r;

				return( *this );
			}

			const polynomial & operator =( const matrix_type &mat )
			{
				// 基底クラスのデータをコピーする
				base::operator =( mat );

				return( *this );
			}

			template < class Matrix >
			double operator ()( const Matrix &x )
			{
				difference_type n = dimension;
				const matrix_type &c = *this;	// 多項式の係数ベクトル

				double r0 = c[ tr[ 0 ] ];
				for( difference_type j = 0 ; j < n ; j++ )
				{
					r[ j ] = 0.0;
				}

				for( size_type i = 1 ; i < N ; i++ )
				{
					difference_type k = tc[ i - 1 ];
					double rsum = r0;
					for( difference_type j = k ; j < n ; j++ )
					{
						rsum += r[ j ];
						r[ j ] = 0.0;
					}

					r0 = c[ tr[ i ] ];
					r[ k ] = x[ k ] * rsum;
				}

				double rsum = r0;
				for( difference_type j = 0 ; j < n ; j++ )
				{
					rsum += r[ j ];
				}

				return( rsum );
			}

			matrix_type compute_hessian_matrix( ) const
			{
				matrix_type H( dimension, dimension );
				const matrix_type &coeff = *this;	// 多項式の係数ベクトル

				size_type indx = dimension + 1;
				for( size_type r = 0 ; r < H.rows( ) ; r++ )
				{
					H( r, r ) = coeff[ indx++ ] * 2.0;
					for( size_type c = r + 1 ; c < H.cols( ) ; c++ )
					{
						H( r, c ) = H( c, r ) = coeff[ indx++ ];
					}
				}

				return( H );
			}

			// 多項式の原点を平行移動する
			void translate( const matrix_type &x )
			{
				// 3.4.6 章参照
				polynomial &c = *this;
				matrix_type Hx = compute_hessian_matrix( ) * x;

				c[ 0 ] = c( x );
				for( size_type r = 0 ; r < Hx.rows( ) ; r++ )
				{
					c[ r + 1 ] += Hx[ r ];
				}
			}


		protected:
			void compute_polynomial_indeces( )
			{
				difference_type n = dimension;
				difference_type R = N;
				difference_type C = n;
				imatrix_type &lex = alpha_;
				imatrix_type &deg = alpha;

				lex.fill( 0 );
				deg.fill( 0 );

				{
					difference_type row = 1;
					for( difference_type c = 0 ; c < C ; c++ )
					{
						deg( row, c ) = 1;
						row++;
					}

					difference_type col = 0;
					difference_type val = 2;
					for( ; row < R && col < C ; )
					{
						switch( val )
						{
						case 2:
							deg( row, col ) = static_cast< imatrix_type::value_type >( val );
							val--;
							row++;
							break;

						case 1:
							for( difference_type c = 1 ; c < C - col ; c++ )
							{
								deg( row, col )     = static_cast< imatrix_type::value_type >( val );;
								deg( row, col + c ) = static_cast< imatrix_type::value_type >( val );;
								row++;
							}
							val--;
							col++;
							break;

						case 0:
						default:
							val = 2;
							break;
						}
					}
				}

				{
					difference_type col = 0;
					difference_type val = 2;
					for( difference_type row = 0 ; row < R - 1 ; )
					{
						switch( val )
						{
						case 2:
							lex( row, col ) = static_cast< imatrix_type::value_type >( val );
							val--;
							row++;
							break;

						case 1:
							for( difference_type c = 1 ; c < C - col ; c++ )
							{
								lex( row, col )     = static_cast< imatrix_type::value_type >( val );
								lex( row, col + c ) = static_cast< imatrix_type::value_type >( val );
								row++;
							}
							lex( row, col ) = static_cast< imatrix_type::value_type >( val );
							row++;
							val--;
							col++;
							break;

						case 0:
						default:
							val = 2;
							break;
						}
					}

					for( difference_type r = 0 ; r < R ; r++ )
					{
						difference_type c = C - 1;
						for( ; c >= 0 ; c-- )
						{
							if( lex( r, c ) != 0 )
							{
								break;
							}
						}
						tc[ r ] = static_cast< imatrix_type::value_type >( c );
					}
				}

				for( difference_type r = 0 ; r < R ; r++ )
				{
					difference_type indx = 0;
					for( ; indx < R ; indx++ )
					{
						difference_type c = 0;
						for( ; c < C ; c++ )
						{
							if( lex( r, c ) != deg( indx, c ) )
							{
								// 不一致
								break;
							}
						}

						if( c == C )
						{
							// 一致しているものを発見
							break;
						}
					}

					tr[ r ] = static_cast< imatrix_type::value_type >( indx );
				}
			}
		};
	}


	/// @brief CONDOR アルゴリズムの初期パラメータ群を求める関数
	//! 
	//! @param[in]  xbase … 探索開始パラメータ
	//! @param[out] x     … 評価値を計算したパラメータ群
	//! @param[out] f     … パラメータに対応した評価値
	//! @param[in]  func  … 評価関数
	//! @param[in]  rho   … 探索ステップ
	//!
	//! @return 最も評価値の良いパラメータのインデックスを返す
	//! 
	template < class Matrix, class Functor >
	size_t generate_first_point_set( const Matrix &xbase, std::vector< Matrix > &x, Matrix &f, Functor func, double rho )
	{
		typedef Matrix matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef typename matrix_type::size_type size_type;
		typedef typename matrix_type::difference_type difference_type;

		difference_type n = xbase.rows( );
		difference_type N = ( n + 1 ) * ( n + 2 ) / 2;

		// データの格納先を用意する
		f.resize( N, 1 );
		x.resize( N );
		for( difference_type i = 0 ; i < N ; i++ )
		{
			x[ i ].resize( n, 1 );
		}

		x[ 0 ] = xbase;
		f[ 0 ] = func( x[ 0 ] );

		for( difference_type j = 0 ; j < n ; j++ )
		{
			size_type k = j + 1;
			x[ k ] = xbase;
			x[ k ][ j ] += rho;
			f[ k ] = func( x[ k ] );
		}

		matrix_type s( n, 1 );
		for( difference_type j = 0 ; j < n ; j++ )
		{
			if( f[ j + 1 ] > f[ 0 ] )
			{
				s[ j ] = -1.0;
			}
			else
			{
				s[ j ] = +1.0;
			}
		}

		for( difference_type j = 0 ; j < n ; j++ )
		{
			difference_type k = j + 1 + n;
			if( s[ j ] < 0.0 )
			{
				x[ k ] = xbase;
				x[ k ][ j ] -= rho;
			}
			else
			{
				x[ k ] = xbase;
				x[ k ][ j ] += rho + rho;
			}
			f[ k ] = func( x[ k ] );
		}

		difference_type k = 2 * n + 1;
		for( difference_type j = 0 ; j < n ; j++ )
		{
			for( difference_type i = 0 ; i < j ; i++ )
			{
				x[ k ] = xbase;
				x[ k ][ i ] += rho * s[ i ];
				x[ k ][ j ] += rho * s[ j ];
				f[ k ] = func( x[ k ] );
				k++;
			}
		}

		// 評価関数値の小さい順に並べ替える
		for( size_type i = 0 ; i < x.size( ) ; i++ )
		{
			for( size_type j = 0 ; j < x.size( ) ; j++ )
			{
				if( f[ i ] < f[ j ] )
				{
					double tmp = f[ i ];
					f[ i ] = f[ j ];
					f[ j ] = tmp;
					x[ i ].swap( x[ j ] );
				}
			}
		}

		return( 0 );
	}

	/// @brief 指定したパラメータと評価値からラグランジェ多項式を構成する
	//! 
	//! @param[in]     x          … 評価値を計算したパラメータ群
	//! @param[in]     f          … パラメータに対応した評価値
	//! @param[in,out] poly_bases … Trust Region を構成するラグランジェ多項式の基底
	//! @param[out]    poly       … Trust Region を構成するラグランジェ多項式
	//!
	//! @return ラグランジェ多項式が正しくもとまったかどうか
	//! 
	template < class Matrix >
	bool compute_polynomial_basis( std::vector< Matrix > &x, Matrix &f, std::vector< __condor_utility__::polynomial > &poly_bases, __condor_utility__::polynomial &poly )
	{
		typedef Matrix matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef typename matrix_type::size_type size_type;
		typedef typename matrix_type::difference_type difference_type;
		typedef __condor_utility__::polynomial polynomial_type;

		if( x.size( ) != poly_bases.size( ) )
		{
			// 入力データと多項式の数が一致しない
			return( false );
		}

		size_type N = poly.size( );
		for( size_type k = 0 ; k < N ; k++ )
		{
			polynomial_type &pk = poly_bases[ k ];

			{
				// 多項式の正規化
				size_type index = k;
				double max = 0.0;
				if( k == 0 )
				{
					max = pk( x[ k ] );
				}
				else
				{
					for( size_type i = k ; i < N ; i++ )
					{
						double val = pk( x[ i ] );
						if( std::abs( val ) > std::abs( max ) )
						{
							max = val;
							index = i;
						}

						if( std::abs( max ) > 1.0 )
						{
							break;
						}
					}
				}

				// 分母が最大となるデータを用いる
				if( index != k )
				{
					// データを入れ替える
					x[ k ].swap( x[ index ] );

					double tmp = f[ k ];
					f[ k ] = f[ index ];
					f[ index ] = tmp;
				}

				pk /= max;
			}

			for( size_type j = 0 ; j < N ; j++ )
			{
				if( j != k )
				{
					polynomial_type &pj = poly_bases[ j ];
					pj -= pj( x[ k ] ) * pk;
				}
			}
		}


		// 最終的な多項式を求める
		poly.fill( 0 );
		for( size_type i = 0 ; i < poly_bases.size( ) ; i++ )
		{
			poly += f[ i ] * poly_bases[ i ];
		}

		return( true );
	}

	/// @brief Trust Region を求める際のλの上下限の計算に用いる補助関数
	//! 
	//! @param[in] H     … ラグランジェ多項式から求めたヘッセ行列
	//!
	//! @return \f$\displaystyle-\min_{i} \left\{ -H_{ii} \right\}\f$
	//! 
	template < class Matrix >
	inline double compute_lambda_function1( const Matrix &H )
	{
		typedef Matrix matrix_type;
		typedef typename matrix_type::size_type size_type;

		double Hmin = H( 0, 0 );
		for( size_type i = 1 ; i < H.rows( ) ; i++ )
		{
			if( Hmin > H( i, i ) )
			{
				Hmin = H( i, i );
			}
		}

		return( -Hmin );
	}

	/// @brief Trust Region を求める際のλの上下限の計算に用いる補助関数
	//! 
	//! @param[in] H     … ラグランジェ多項式から求めたヘッセ行列
	//! @param[in] alpha … H( i, i ) の符号
	//!
	//! @return \f$\displaystyle\max_{i} \left\{ -H_{ii} + \sum_{i \ne j}{\|H_{ij}\|} \right\}\f$
	//! 
	template < class Matrix >
	inline double compute_lambda_function2( const Matrix &H, double alpha = 1.0 )
	{
		typedef Matrix matrix_type;
		typedef typename matrix_type::size_type size_type;

		double max = -1.0e100;
		for( size_type i = 0 ; i < H.rows( ) ; i++ )
		{
			double val = H( i, i ) * alpha;
			for( size_type j = 0 ; j < i ; j++ )
			{
				val += std::abs( H( i, j ) );
			}
			for( size_type j = i + 1 ; j < H.cols( ) ; j++ )
			{
				val += std::abs( H( i, j ) );
			}

			if( max < val )
			{
				max = val;
			}
		}

		return( max );
	}


	/// @brief Trust Region を求める際のλの下限の初期値
	//! 
	//! @param[in] H        … ラグランジェ多項式から求めたヘッセ行列
	//! @param[in] dg_delta … ||g|| / delta
	//!
	//! @return λの下限の初期値
	//! 
	template < class Matrix >
	inline double compute_lambda_lower_bound( const Matrix &H, double dg_delta )
	{
		double Hm = compute_lambda_function1( H );
		double Hg = compute_lambda_function2( H, +1.0 );
		double Hf = __condor_utility__::frobenius_norm( H );
		double Hi = __condor_utility__::infinitum_norm( H );

		return( __condor_utility__::maximum( 0, Hm, dg_delta - __condor_utility__::minimum( Hg, Hf, Hi ) ) );
	}


	/// @brief Trust Region を求める際のλの上限の初期値
	//! 
	//! @param[in] H        … ラグランジェ多項式から求めたヘッセ行列
	//! @param[in] dg_delta … ||g|| / delta
	//!
	//! @return λの上限の初期値
	//! 
	template < class Matrix >
	inline double compute_lambda_upper_bound( const Matrix &H, double dg_delta )
	{
		double Hg = compute_lambda_function2( H, -1.0 );
		double Hf = __condor_utility__::frobenius_norm( H );
		double Hi = __condor_utility__::infinitum_norm( H );

		return( __condor_utility__::maximum( 0, dg_delta + __condor_utility__::minimum( Hg, Hf, Hi ) ) );
	}


	/// @brief CONDOR アルゴリズム内で使用する Trust Region 内の最小値を与える方向を求める関数
	//! 
	//! @param[in] xbest    … 現時点での最も良い評価値を与えるパラメータ
	//! @param[in] s        … Trust Region を探索した結果，評価値を減少させるであろう方向
	//! @param[in] poly     … Trust Region を構成するラグランジェ多項式
	//! @param[in] delta    … Trust Region の半径
	//! @param[in] max_loop … Trust Region 内の探索を行う最大反復回数
	//! 
	template < class Matrix >
	void compute_trust_region_step( const Matrix &xbest, Matrix &s, __condor_utility__::polynomial &poly, double delta, size_t max_loop = 1000 )
	{
		typedef Matrix matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef typename matrix_type::size_type size_type;
		typedef typename matrix_type::difference_type difference_type;
		typedef __condor_utility__::polynomial polynomial_type;

		matrix_type H = poly.compute_hessian_matrix( );
		matrix_type g = H * xbest;
		for( size_type i = 0 ; i < g.rows( ) ; i++ )
		{
			g[ i ] += poly[ i + 1 ];
		}

		double gnorm = __condor_utility__::frobenius_norm( g );
		double lambda  = gnorm / delta;
		double lambdaL = compute_lambda_lower_bound( H, lambda );
		double lambdaU = compute_lambda_upper_bound( H, lambda );

		// lambdaL <= lambda <= lambdaU となるようにする
		lambda = __condor_utility__::maximum( lambdaL, __condor_utility__::minimum( lambda, lambdaU ) );

		bool cholesky_factorization_finished = false;
		matrix_type L( H.rows( ), H.cols( ) ), w( g.rows( ), g.cols( ) );

		// Trust Region step のベクトルを初期化する
		s.resize( g.rows( ), 1 );
		s.fill( 0 );

		size_type loop;
		for( loop = 0 ; loop < max_loop ; loop++ )
		{
			if( !cholesky_factorization_finished )
			{
				double lambdaM = 0.0;
				if( !__condor_utility__::cholesky_factorization( H, L, lambda, lambdaM ) )
				{
					lambdaL = __condor_utility__::maximum( lambdaL, lambda + lambdaM );
					lambda  = __condor_utility__::maximum( std::sqrt( lambdaL * lambdaU ), lambdaL + 0.01 * ( lambdaU - lambdaL ) );
					continue;
				}
			}
			else
			{
				cholesky_factorization_finished = false;
			}

			s = -g;

			// L * L.t( ) * s = -g を解く
			// L が下三角行列であるということを利用して以下の手順で s を求める
			__condor_utility__::solve( L, s );		// まず L * ( L.t( ) * s' ) = -g を解く
			__condor_utility__::solve_( L, s );		// 次に L.t( ) * s = s' を解く
			double snorm = __condor_utility__::frobenius_norm( s );

			// 4.9.1 節の終了判定
			if( std::abs( snorm - delta ) < 0.01 * delta )
			{
				// 十分良い値が求まったと判定する
				s *= delta / snorm;
				break; 
			}
			else if( snorm < delta )
			{
				if( lambda == 0.0 )
				{
					// 十分良い値が求まったと判定する
					break;
				}

				lambdaU = __condor_utility__::minimum( lambdaU, lambda );

#if defined( __CHECK_HARD_CASE__ ) && __CHECK_HARD_CASE__ != 0
				// 複雑な場合かをチェックする
				matrix_type u;
				__condor_utility__::compute_eigen_vector( L, u, lambda );

				double uHu = __condor_utility__::inner_product( u, H, u, lambda );

				// Powell の UOBYQA アルゴリズムでの Hard Case 判定条件
				if( uHu <= 0.01 * ( uHu + lambda * delta * delta ) )
				{
					double a = 0.0;
					double b = 0.0;
					double c = - delta * delta;
					for( size_type i = 0 ; i < u.rows( ) ; i++ )
					{
						a += u[ i ] * u[ i ];
						b += s[ i ] * u[ i ];
						c += s[ i ] * s[ i ];
					}

					double alpha = 0.0;
					double bac = std::sqrt( b * b - a * c );

					double alpha1 = ( -b - bac ) / a;
					double alpha2 = ( -b + bac ) / a;

					matrix_type s1( s ), s2( s ), x1( xbest ), x2( xbest );
					for( size_type r = 0 ; r < s.rows( ) ; r++ )
					{
						s1[ r ] += alpha1 * u[ r ];
						s2[ r ] += alpha2 * u[ r ];
						x1[ r ] += s1[ r ];
						x2[ r ] += s2[ r ];
					}

					double f1 = poly( x1 );
					double f2 = poly( x2 );

					if( f1 >= f2 )
					{
						alpha = alpha1;
						s = s1;
					}
					else
					{
						alpha = alpha2;
						s = s2;
					}

					// Hard Case なので終了する
					break;
				}
#endif
			}
			else// if( snorm > delta )
			{
				lambdaL = __condor_utility__::maximum( lambdaL, lambda );
			}

			w = s;
			__condor_utility__::solve( L, w );

			double wnorm = __condor_utility__::frobenius_norm( w );
			double lambdaN = lambdaL;
			
			// wnorm が限りなくゼロに近い場合への対処
			if( wnorm > 1.0e-20 )
			{
				lambdaN = lambda + ( ( snorm - delta ) / delta ) * ( ( snorm * snorm ) / ( wnorm * wnorm ) );
				lambdaN = __condor_utility__::maximum( lambdaL, __condor_utility__::minimum( lambdaN, lambdaU ) );	// 新しく求めたλの範囲をチェックする
			}
			else
			{
				lambdaN = __condor_utility__::maximum( lambdaL, lambdaU );	// 新しく求めたλの範囲をチェックする
			}

			// 新しく求めたλを使ってコレスキー分解を再度行う
			if( __condor_utility__::cholesky_factorization( H, L, lambdaN ) )
			{
				lambda = lambdaN;
				cholesky_factorization_finished = true;
			}
			else
			{
				lambdaL = __condor_utility__::maximum( lambdaL, lambdaN );
				lambda  = __condor_utility__::maximum( std::sqrt( lambdaL * lambdaU ), lambdaL + 0.01 * ( lambdaU - lambdaL ) );
			}
		}
	}


	/// @brief CONDOR アルゴリズムを用いて評価関数の最小値を探索する
	//! 
	//! 探索の開始点を指定し，その近傍点を評価しながら最小値を探索する．
	//! 関数の導関数が不要であり，値の範囲に制限が無いバージョン．
	//! 
	//! - 参考文献
	//!   - F. V. Berghen, H. Bersini, ``CONDOR, a new parallel, constrained extension of Powell's UOBYQA algorithm: Experimental results and comparison with the DFO algorithm,'' Journal of Computational and Applied Mathematics, Elsevier, Volume 181, Issue 1, September 2005, pp. 157--175 
	//!
	//! @attention 本プログラムはMISTチームが独自に実装したCONDORアルゴリズムのため，Berghen 氏が公開されているプログラムとは同じ結果とならない可能性があります．
	//!
	//! @param[in,out] p              … 探索の開始ベクトル，探索終了後に最小値を与えるベクトルが代入される
	//! @param[in]     func           … 評価関数
	//! @param[in]     rho            … 探索初期のステップ幅
	//! @param[in]     rho_end        … 探索終了時のステップ幅
	//! @param[in]     tolerance      … 探索終了時の許容相対誤差
	//! @param[out]    iterations     … 反復回数の結果が代入される
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, Functor func, double rho, double rho_end, double tolerance, size_t &iterations, size_t max_iterations = 1000 )
	{
		typedef matrix< T, Allocator > matrix_type;
		typedef typename matrix_type::value_type value_type;
		typedef typename matrix_type::size_type size_type;
		typedef typename matrix_type::difference_type difference_type;
		typedef matrix< difference_type > imatrix_type;
		typedef __condor_utility__::polynomial polynomial_type;

		matrix_type f;
		std::vector< matrix_type > x;

		difference_type n = p.rows( );
		difference_type N = ( n + 1 ) * ( n + 2 ) / 2;

		// [Step 1] 2次のラグランジェ補間（超曲面）を計算するための初期点列を求める
		difference_type best_index = generate_first_point_set( p, x, f, func, rho );


		// [Step 2] ラグランジェ補間の基準となる点を決め，ラグランジェ多項式の係数を求める
		// 多項式を扱うオブジェクトを生成する
		// 最初のひとつを生成した後はすべてコピーする
		matrix_type xbase = x[ best_index ];
		polynomial_type poly;
		std::vector< polynomial_type > poly_bases( N );
		poly.reinitialize_polynomial( n );

		for( difference_type i = 0 ; i < N ; i++ )
		{
			poly_bases[ i ] = poly;
		}
		for( difference_type i = 0 ; i < N ; i++ )
		{
			poly_bases[ i ].fill( 0 );
			poly_bases[ i ][ i ] = 1.0;

			x[ i ] -= xbase;
		}

		// 多項式の基底を初期化する
		compute_polynomial_basis( x, f, poly_bases, poly );


		double M     = 0.0;						// 多項式補間のモデル評価値
		double delta = rho;						// Trust Region の半径
		matrix_type tstep;						// Trust Region 問題を解いた結果得られた関数値の減少方向
		double snorm = 0.0;						// Trust Region 問題を解いた結果得られた関数値の減少方向の大きさ
		double Fold  = f[ best_index ];			// 前回の反復操作で得られた関数値の最小値
		double Fold_ = Fold;					// 許容相対誤差での終了判定用
		double Fnew  = 1.0e100;					// 新規に評価した関数値
		bool   is_function_evaluated = false;	// アルゴリズムの途中で，[Step 6]～[Step 8]のスキップが発生したかどうか
		size_type number_of_updateM = 0;

		// 反復回数を初期化する
		iterations = 0;

		while( iterations < max_iterations )
		{
			Fold = __condor_utility__::minimum( Fnew, Fold );
			is_function_evaluated = false;
			for( size_type loop = 0 ; iterations++ < max_iterations ; loop++ )
			{
				double Fbest = f[ best_index ];
				const matrix_type &xbest = x[ best_index ];

				// [Step 5] Trust Region を求める
				compute_trust_region_step( xbest, tstep, poly, delta );

				// Trust Region から得られた新しい探索点を求める
				snorm = __condor_utility__::frobenius_norm( tstep );
				matrix_type xnew = xbest + tstep;


				// [Step 6] Trust Region ステップを終了し，モデルの再検証を行う
				// 1回は必ず以降の処理を実行するようにするため、初回はスキップする
				if( snorm < rho * 0.5 && loop > 0 )
				{
					break;
				}


				// [Step 7]
				// fbest を中心にラグランジェ多項式を当てはめるので，poly( xbest ) == fbest のはず
				double R = Fold - poly( xnew );


				// [Step 8] ノイズを付加して正しく最適化できているかを調べる？
				// 1回は必ず以降の処理を実行するようにするため、初回はスキップする
				double na = 0.0, nr = 0.0;
				double noise = 0.5 * __condor_utility__::maximum( na * ( 1.0 + nr ), nr * std::abs( Fbest ) );
				if( R < noise && loop > 0 )
				{
					// 今のところここにくることは無い
					break;
				}


				// [Step 9] 新しい位置で関数値を評価する
				{
					matrix_type ppp( xbase + xnew );
					Fnew = func( ppp );
					is_function_evaluated = true;
				}


				// [Step 10] モデルとの整合性を表す指標を計算する
				double r = R != 0.0 ? ( Fold - Fnew ) / R : Fold - Fnew;


				// [Step 11] Trust Region の半径を更新する
				if( 0.7 <= r )
				{
					delta = __condor_utility__::maximum( delta, snorm * 1.25, rho + snorm );
				}
				else if( 0.1 <= r )
				{
					delta = __condor_utility__::maximum( 0.5 * delta, snorm );
				}
				else
				{
					delta = 0.5 * snorm;
				}

				if( delta < 1.5 * rho )
				{
					delta = rho;
				}


				// [Step 12] 新しい点を補間点群に含め，多項式の係数を更新する
				// 3.4.3 と 3.4.4 を参照
				double model_step = 0.0;
				bool has_reduction = Fnew < Fbest;
				{
					difference_type index = -1;
					double max = -1.0e100;
					double Pnew = 0.0;
					if( Fnew < Fbest )
					{
						for( difference_type i = 0 ; i < N ; i++ )
						{
							double pnew = poly_bases[ i ]( xnew );
							double norm = __condor_utility__::frobenius_norm( x[ i ] - xnew ) / rho;
							double val  = __condor_utility__::maximum( 1.0, norm * norm * norm ) * std::abs( pnew );

							if( val > max )
							{
								max = val;
								Pnew = pnew;
								index = i;
							}
						}
					}
					else
					{
						for( difference_type i = 0 ; i < N ; i++ )
						{
							if( i != best_index )
							{
								double pnew = poly_bases[ i ]( xnew );
								double norm = __condor_utility__::frobenius_norm( x[ i ] - xbest ) / rho;
								double val  = __condor_utility__::maximum( 1.0, norm * norm * norm ) * std::abs( pnew );

								if( val > max )
								{
									max = val;
									Pnew = pnew;
									index = i;
								}
							}
						}
					}

					if( index >= 0 && Pnew != 0.0 )
					{
						// Model Step を計算する
						model_step = __condor_utility__::frobenius_norm( x[ index ] - xnew );

						// 多項式を更新する
						poly_bases[ index ] /= Pnew;
						for( difference_type i = 0 ; i < N ; i++ )
						{
							if( i != index )
							{
								poly_bases[ i ] -= poly_bases[ i ]( xnew ) * poly_bases[ index ];
							}
						}


						// データの置き換えを行う
						x[ index ] = xnew;
						f[ index ] = Fnew;


						// [Step 13] 最も良い値を持つものを更新する
						if( f[ best_index ] > Fnew )
						{
							Fold = __condor_utility__::minimum( Fnew, Fold );
							best_index = index;
						}

						// 最終的な多項式を求める
						poly.fill( 0 );
						for( size_type i = 0 ; i < poly_bases.size( ) ; i++ )
						{
							poly += f[ i ] * poly_bases[ i ];
						}
					}
				}


				// [Step 14] モデルの評価に用いる値 M を計算する
				{
					double sum = 0.0;
					for( size_type i = 0 ; i < poly_bases.size( ) ; i++ )
					{
						double len = __condor_utility__::frobenius_norm( xnew - x[ i ] );
						sum += std::abs( poly_bases[ i ]( xnew ) ) * len * len * len;
					}

					if( sum != 0.0 )
					{
						M = __condor_utility__::maximum( M, std::abs( poly( xnew ) - Fnew ) / sum );
						number_of_updateM++;
					}
				}


				// [Step 15] 関数の最小化に効果があったかどうかを判定する
				if( model_step > 2.0 * rho || snorm > 2.0 * rho || has_reduction )
				{
				}
				else
				{
					break;
				}
			}

			if( iterations >= max_iterations )
			{
				// 最大反復回数を経過した
				break;
			}


			// [Step 17.1] モデルチェックに使う評価基準値を求める
			double eps = 0.0;
			if( number_of_updateM < 10 )
			{
				eps = 0.0;
			}
			else if( snorm >= rho * 0.5 )
			{
				eps = 0.0;
			}
			else
			{
				// 4.10 章を参照
				// 多項式モデルの最も良い評価地の点付近の傾きを計算する
				matrix_type H = poly.compute_hessian_matrix( );
				matrix_type L( H.rows( ), H.cols( ) );
				double Hg = compute_lambda_function2( H, +1.0 );
				double Hf = __condor_utility__::frobenius_norm( H );
				double Hi = __condor_utility__::infinitum_norm( H );

				double lambdaL = 0.0;
				double lambdaU = __condor_utility__::minimum( Hg, Hf, Hi );
				while( lambdaL < 0.99 * lambdaU )
				{
					double lambda = ( lambdaL + lambdaU ) * 0.5;

					if( __condor_utility__::cholesky_factorization( H, L, -lambda ) )
					{
						lambdaL = lambda;
					}
					else
					{
						lambdaU = lambda;
					}
				}

				eps = 0.5 * rho * rho * ( lambdaL + lambdaU ) * 0.5;
			}


			// [17.2] モデルチェックに用いる点を求める（3.4.2章）
			{
				// まず，モデル評価に用いる点の集合Jを求める
				std::vector< __condor_utility__::__index_value_pair__ > J;
				J.reserve( N );
				for( difference_type i = 0 ; i < N ; i++ )
				{
					if( i != best_index )
					{
						double len = __condor_utility__::frobenius_norm( x[ i ] - x[ best_index ] );
						if( len > 2.0 * rho )
						{
							J.push_back( __condor_utility__::__index_value_pair__( i, len ) );
						}
					}
				}

				// モデルに悪影響を与える点の判定と多項式の更新を行う
				if( !J.empty( ) )
				{
					// 最も良い点からの距離値が大きい順に並べる
					std::sort( J.begin( ), J.end( ) );

					size_type index = 0;
					matrix_type d;
					for( ; index < J.size( ) ; index++ )
					{
						__condor_utility__::__index_value_pair__ &ivpair = J[ index ];
						polynomial_type &p = poly_bases[ ivpair.index ];
						matrix_type H = p.compute_hessian_matrix( );
						matrix_type g = H * x[ best_index ];
						for( size_type i = 0 ; i < g.rows( ) ; i++ )
						{
							g[ i ] += p[ i + 1 ];
						}

						double gnorm = __condor_utility__::frobenius_norm( g );
						if( gnorm == 0.0 )
						{
							continue;
						}

						matrix_type w( H.rows( ), 1 );

						{
							// ヘッセ行列 H の列ベクトルの乗る無が最大となるものを見つける
							// 式(5.5)と(5.6)を参照
							double max = -1.0e10;
							size_type col = 0;
							for( size_type c = 0 ; c < H.cols( ) ; c++ )
							{
								double sum = 0.0;
								for( size_type r = 0 ; r < H.rows( ) ; r++ )
								{
									sum += H( r, c ) * H( r, c );
								}

								if( max < sum )
								{
									max = sum;
									col = c;
								}
							}

							for( size_type r = 0 ; r < H.rows( ) ; r++ )
							{
								w[ r ] = H( r, col );
							}
						}

						matrix_type V = w;
						matrix_type D = H * w;
						double DHD = __condor_utility__::inner_product( D, H, D );
						double VD  = __condor_utility__::inner_product( V, D );
						double DD  = __condor_utility__::inner_product( D, D );
						double VHD = __condor_utility__::inner_product( V, H, D );
						double VHV = __condor_utility__::inner_product( V, H, V );
						double VV  = __condor_utility__::inner_product( V, V );

						{
							double a = DHD * VD - DD * DD;
							double b = ( DHD * VV - DD * VD ) * 0.5;
							double c = DD * VV - VD * VD;
							double bac = std::sqrt( b * b - a * c );
							double r = 0.0;

							double r1 = ( -b - bac ) / a;
							double r2 = ( -b + bac ) / a;

							double f1 = ( r1 * r1 * DHD + 2.0 * r1 * VHD + VHV ) / ( VV + 2.0 * r1 * VD + r1 * r1 * DD );
							double f2 = ( r2 * r2 * DHD + 2.0 * r2 * VHD + VHV ) / ( VV + 2.0 * r2 * VD + r2 * r2 * DD );

							if( f1 > f2 )
							{
								r = r1;
							}
							else
							{
								r = r2;
							}

							// D を更新する
							D = V + r * D;
						}

						DD = __condor_utility__::inner_product( D, D );
						if( gnorm * DD + 2.0 * rho * std::abs( DHD ) < 0.5 )
						{
							double GD = __condor_utility__::inner_product( g, D );
							double scale = rho / std::sqrt( DD );
							if( GD * DHD < 0.0 )
							{
								d = - D * scale;
							}
							else
							{
								d = D * scale;
							}
						}
						else
						{
							// 5.2 章の u の組を作る
							// 大きさが1になるように正規化する
							// 式(5.10) からθを計算する
							matrix_type G = D / __condor_utility__::frobenius_norm( D );
							matrix_type V = g / gnorm;
							double VHG = __condor_utility__::inner_product( V, H, G );
							double VHV = __condor_utility__::inner_product( V, H, V );
							double GHG = __condor_utility__::inner_product( G, H, G );
							double theta = std::atan2( 2.0 * VHG, VHV - GHG ) * 0.5;
		
							// 式(5.9) を元に u を計算する
							double s_ = std::sin( theta );
							double c_ = std::cos( theta );
							matrix_type ut = c_ * G + s_ * V;
							matrix_type uh = -s_ * G + c_ * V;

							// 式(3.38)を最大にする d を得る
							// 5.3 章参照
							const double pai = 3.1415926535897932384626433832795;
							const double phi[ 4 ] = { 0.0, pai * 0.25, pai * 0.5, pai * 0.75 };

							// d の初期方向を設定する
							d = ( std::cos( phi[ 0 ] ) * uh + std::sin( phi[ 0 ] ) * ut );
							d = rho * d / __condor_utility__::frobenius_norm( d );	// 大きさを rho にする

							// 式(5.2)を最大にする d を選ぶ
							double max = std::abs( __condor_utility__::inner_product( g, d ) ) + 0.5 * std::abs( __condor_utility__::inner_product( d, H, d ) );
							for( size_type i = 1 ; i < 4 ; i++ )
							{
								matrix_type tmp = ( std::cos( phi[ i ] ) * uh + std::sin( phi[ i ] ) * ut );
								tmp = rho * tmp / __condor_utility__::frobenius_norm( tmp );	// 大きさを rho にする
								double val = std::abs( __condor_utility__::inner_product( g, tmp ) ) + 0.5 * std::abs( __condor_utility__::inner_product( tmp, H, tmp ) );
								if( val > max )
								{
									max = val;
									d = tmp;
								}
							}

							double GD = __condor_utility__::inner_product( g, d );
							double DHD = __condor_utility__::inner_product( d, H, d );
							if( GD * DHD < 0.0 )
							{
								d = - d;
							}
						}

						// 式(3.37)の評価
						double len = ivpair.value;
						double val = M * len * len * len * p( x[ best_index ] + d );

						if( val > eps )
						{
							break;
						}
					}


					// [17.3] 最もモデルに悪影響を与えると思われる点を入れ替える（3.4.4章）
					if( index < J.size( ) )
					{
						// 補間多項式の条件を満たさない点を発見
						difference_type outindex = J[ index ].index;

						matrix_type xnew = x[ best_index ] + d;
						matrix_type ppp = xbase + xnew;
						double fnew = func( ppp );
						is_function_evaluated = true;

						// 多項式を更新する
						poly_bases[ outindex ] /= poly_bases[ outindex ]( xnew );
						for( difference_type i = 0 ; i < N ; i++ )
						{
							if( i != outindex )
							{
								poly_bases[ i ] -= poly_bases[ i ]( xnew ) * poly_bases[ outindex ];
							}
						}

						// データの置き換えを行う
						x[ outindex ] = xnew;
						f[ outindex ] = fnew;

						// 最終的な多項式を求める
						poly.fill( 0 );
						for( size_type i = 0 ; i < poly_bases.size( ) ; i++ )
						{
							poly += f[ i ] * poly_bases[ i ];
						}

						// [Step 17.4] 最も良い値を持つものを更新する
						Fold = __condor_utility__::minimum( fnew, Fold );
						if( f[ best_index ] > fnew )
						{
							best_index = outindex;
						}

						// [Step 17.5] モデルが不適切と判断されたので値 M を再計算する
						{
							double sum = 0.0;
							for( size_type i = 0 ; i < poly_bases.size( ) ; i++ )
							{
								double len = __condor_utility__::frobenius_norm( xnew - x[ i ] );
								sum += std::abs( poly_bases[ i ]( xnew ) ) * len * len * len;
							}

							if( sum != 0.0 )
							{
								M = __condor_utility__::maximum( M, std::abs( poly( xnew ) - Fnew ) / sum );
								number_of_updateM++;
							}
						}

						// [Step 17.6] Step 4 へ戻る
						continue;
					}
				} // <- モデルに悪影響を与える点の判定と多項式の更新はここまで

				// [Step 17.7] モデルが正しく求まっている場合の判定
				if( snorm > rho )
				{
					continue;
				}
			}

			// [Step 18] ループの終了判定
			{
				if( rho <= rho_end || 2.0 * std::abs( Fold_ - Fold ) < tolerance * ( std::abs( Fold_ ) + std::abs( Fold ) ) )
				{
					break;
				}
				else
				{
					Fold_ = Fold;
				}
			}

			// [Step 19] Trust Region の半径を更新
			{
				double rho_old = rho;
				if( rho <= 16.0 * rho_end )
				{
					rho = rho_end;
				}
				else if( rho <= 250.0 * rho_end )
				{
					rho = std::sqrt( rho_end * rho );
				}
				else
				{
					rho *= 0.1;
				}

				delta = __condor_utility__::maximum( rho_old * 0.5, rho );
			}


			// [Step 20] 多項式の原点を平行移動する
			{
				matrix_type shift = x[ best_index ];
				xbase += x[ best_index ];
				poly.translate( shift );
				for( difference_type i = 0 ; i < N ; i++ )
				{
					poly_bases[ i ].translate( shift );
					if( i == best_index )
					{
						// 最も良い値を持つ者だけは 0 となるようにする
						x[ i ] *= 0.0;
					}
					else
					{
						x[ i ] -= shift;
					}
				}
			}
		}


		// [Step 21] 
		{
			matrix_type xnew = xbase + x[ best_index ] + tstep;

			// 新しい位置で関数値を評価していない場合は評価を行う
			if( !is_function_evaluated )
			{
				Fnew = func( xnew );
			}

			if( Fold < Fnew )
			{
				p = xbase + x[ best_index ];
			}
			else
			{
				p = xnew;
				Fold = Fnew;
			}
		}

		return( Fold );
	}

	/// @brief CONDOR アルゴリズムを用いて評価関数の最小値を探索する
	//! 
	//! 探索の開始点を指定し，その近傍点を評価しながら最小値を探索する．
	//! 関数の導関数が不要であり，値の範囲に制限が無いバージョン．
	//! 
	//! - 参考文献
	//!   - F. V. Berghen, H. Bersini, ``CONDOR, a new parallel, constrained extension of Powell's UOBYQA algorithm: Experimental results and comparison with the DFO algorithm,'' Journal of Computational and Applied Mathematics, Elsevier, Volume 181, Issue 1, September 2005, pp. 157--175 
	//!
	//! @attention 本プログラムはMISTチームが独自に実装したCONDORアルゴリズムのため，Berghen 氏が公開されているプログラムとは同じ結果とならない可能性があります．
	//!
	//! @param[in,out] p              … 探索の開始ベクトル，探索終了後に最小値を与えるベクトルが代入される
	//! @param[in]     f              … 評価関数
	//! @param[in]     rho            … 探索初期のステップ幅
	//! @param[in]     rho_end        … 探索終了時のステップ幅
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, Functor f, double rho, double rho_end, size_t max_iterations )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, __no_copy_constructor_functor__( f ), rho, rho_end, 0.0, itenum, max_iterations ) );
	}

	/// @brief CONDOR アルゴリズムを用いて評価関数の最小値を探索する
	//! 
	//! 探索の開始点を指定し，その近傍点を評価しながら最小値を探索する．
	//! 関数の導関数が不要であり，値の範囲に制限が無いバージョン．
	//! 許容相対誤差のみで終了判定を行うバージョンであり，初期探索ステップ幅は 1.0 である．
	//! 
	//! - 参考文献
	//!   - F. V. Berghen, H. Bersini, ``CONDOR, a new parallel, constrained extension of Powell's UOBYQA algorithm: Experimental results and comparison with the DFO algorithm,'' Journal of Computational and Applied Mathematics, Elsevier, Volume 181, Issue 1, September 2005, pp. 157--175 
	//!
	//! @attention 本プログラムはMISTチームが独自に実装したCONDORアルゴリズムのため，Berghen 氏が公開されているプログラムとは同じ結果とならない可能性があります．
	//!
	//! @param[in,out] p              … 探索の開始ベクトル，探索終了後に最小値を与えるベクトルが代入される
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 探索終了時の許容相対誤差
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, size_t max_iterations )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, __no_copy_constructor_functor__( f ), 1.0, 1.0e-8, tolerance, itenum, max_iterations ) );
	}
}


/// @}
//  関数の最小化グループの終わり



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MINIMIZATION__
