/// @file mist/minimization.h
//!
//! @brief 関数の最小化を扱うためのライブラリ
//!
//! @section 関数の最小化
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

		// まず，十分に遠い天を設定する
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
//! 
template < class Functor >
void enclose( double &a, double &b, double &c, double &fa, double &fb, double &fc, Functor f )
{
	const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;
	const double _1_gold = 1.0 / gold;
	const double dust = type_limits< double >::minimum( );		// ゼロ除算を避けるためのゴミ値
	const double limit = 100.0;									// 放物線補外を適用する区間の最大更新幅

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

	// f( a ) > f( b ) < f( c ) となるまで，区間の更新を続ける
	while( fb > fc )
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
		else if( ( b + limit * cb ) * ( x - c ) > 0 )
		{
			// 補間点 x が区間 b < c < x を満たしていて，許容範囲内に補間点が存在する
			double fx = f( x );

			if( fx < fc )
			{
				// 現在の端である fc よりも関数値が小さい場合は，区間を更新する
				// a <- b, b <- x としてする
				a = b;
				b = x;
				fa = fb;
				fb = fx;
			}
			else
			{
				// そうでなければ，不適切な放物線補外を行ったので黄金分割比を用いて更新する
				// a <- b, b <- c とする
				a = b;
				b = c;
				fa = fb;
				fb = fc;
			}

			// 新しい区間点 c を黄金分割比を用いて求める
			c = a + _1_gold * ( b - a );
			fc = f( c );
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
			enclose( a, p, b, fa, fp, fb, f );

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
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 200, bool use_enclose = true )
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
			enclose( a, x, b, fa, fx, fb, f );

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

			if( std::abs( p ) < std::abs( 0.5 * q * r ) && p < q * ( a - x ) && p < q * ( b - x ) )
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
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t max_iterations = 200, bool use_enclose = true )
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
	double minimization( matrix< T, Allocator > &p, Functor1 f, Functor2 g, double tolerance, size_t &iterations, size_t max_iterations = 200 )
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
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, 200, true );

			//std::cout << p.t( ) << ", " << dir.t( ) << std::endl;

			if( old_err - err < tolerance )
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
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor1 f, Functor2 g, double tolerance, size_t &iterations, size_t max_iterations = 200 )
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
				err = brent::minimization( l1, l2, x, functor, tolerance, 200, false );
			}

			//std::cout << p.t( ) << ", " << dir.t( ) << std::endl;

			if( old_err - err < tolerance )
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
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor1 f, Functor2 g, double tolerance, size_t max_iterations = 200 )
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
	double minimization( matrix< T, Allocator > &p, Functor1 f, Functor2 g, double tolerance, size_t max_iterations = 200 )
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
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
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
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, 200, true );

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
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
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

			double l1, l2;
			if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
			{
				// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
				err = brent::minimization( l1, l2, x, functor, tolerance, 200, false );
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
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance = 1.0, size_t max_iterations = 200 )
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
	double minimization( matrix< T, Allocator > &p, const matrix< T, Allocator > &bound, Functor f, double tolerance, double distance = 1.0, size_t max_iterations = 200 )
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
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 ), p0( p ), pn( p );
		double x, fp0 = 1.0e100, fp = f( p ), delta;

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

				// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
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

			// 相対誤差を用いた収束判定
			if( 2.0 * std::abs( fp - fp0 ) <= tolerance * ( std::abs( fp ) + std::abs( fp0 ) ) )
			{
				break;
			}

			// Acton の方法を用いて，新しい方向集合を求める
			if( ite <= max_iterations )
			{
				// 新しい方向を求める
				double len = 0.0;
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					double l = p[ r ] - p0[ r ];
					len += l;
					dir[ r ] = l;

					pn[ r ]  = 2.0 * p[ r ] - p0[ r ];
					p0[ r ]  = p[ r ];
				}

				// 次の探索方向を正規化する
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
					// 現在の方向集合を更新する
					double tmp = fp0 - fp - delta;
					double ttt = 2.0 * ( fp0 - 2.0 * fp + fe ) * tmp * tmp - delta * ( fp0 - fe ) * ( fp0 - fe );
					if( ttt >= 0 )
					{
						// Brent の2次収束アルゴリズムを用いて，新しい dir 方向への最小化を行う
						fp = brent::minimization( -0.5, 0.5, x, functor, tolerance, 200, true );
						p += dir * x;

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
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, matrix< T, Allocator > &bound, Functor f, double tolerance, size_t &iterations, size_t max_iterations = 200 )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 ), p0( p ), pn( p );
		double x = 0.0, fp0 = 1.0e100, fp = f( p ), delta;
		double l1, l2;

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

			// 相対誤差を用いた収束判定
			if( 2.0 * std::abs( fp - fp0 ) <= tolerance * ( std::abs( fp ) + std::abs( fp0 ) ) )
			{
				break;
			}

			// Acton の方法を用いて，新しい方向集合を求める
			if( ite <= max_iterations )
			{
				// 新しい方向を求める
				double len = 0.0;
				for( r = 0 ; r < p.size( ) ; r++ )
				{
					double l = p[ r ] - p0[ r ];
					len += l;
					dir[ r ] = l;

					pn[ r ]  = 2.0 * p[ r ] - p0[ r ];
					p0[ r ]  = p[ r ];
				}

				// 次の探索方向を正規化する
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
					// 現在の方向集合を更新する
					double tmp = fp0 - fp - delta;
					double ttt = 2.0 * ( fp0 - 2.0 * fp + fe ) * tmp * tmp - delta * ( fp0 - fe ) * ( fp0 - fe );
					if( ttt >= 0 )
					{
						if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
						{
							// Brent の2次収束アルゴリズムを用いて，新しい dir 方向への最小化を行う
							fp = brent::minimization( l1, l2, x, functor, tolerance, 200, false );
							p += dir * x;
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
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t max_iterations = 200 )
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
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, matrix< T, Allocator > &bound, Functor f, double tolerance, size_t max_iterations = 200 )
	{
		typedef __minimization_utility__::__no_copy_constructor_functor__< Functor > __no_copy_constructor_functor__;
		size_t itenum = 0;
		return( minimization( p, dirs, bound, __no_copy_constructor_functor__( f ), tolerance, itenum, max_iterations ) );
	}
}



/// @}
//  関数の最小化グループの終わり



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MINIMIZATION__
