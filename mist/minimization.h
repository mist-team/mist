/// @file mist/minimization.h
//!
//! @brief 関数の最小化を扱うためのライブラリ
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

}



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
		double x = b + ( ba * ba * fcb + cb * cb * fba ) / ( 2.0 * ( cb * fba - ba * fcb ) + dust );

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


namespace gold
{
	/// @brief 黄金分割を利用して，f(x)の極小値の一次元探索を行う（1次収束）
	//! 
	//! 初期の極小を含むと思われる区間 [a, b] を適切に設定することで，収束を早めることができる
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[out] iterations     … 実際の反復回数
	//! @param[in]  max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t &iterations, size_t max_iterations )
	{
		double c, p, q, fa, fb, fc, fp, fq;
		const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;

		enclose( a, b, c, fa, fb, fc, f );

		// a <= b <= c となるように区間を変更する
		if( a > c )
		{
			double tmp = a;
			a = c;
			c = tmp;
			tmp = fa;
			fa = fc;
			fc = tmp;
		}

		// 区間の長いほうを決め，黄金分割によりもう一つの点を決定する
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
				// 区間 p < f( x ) < c の間に最小値が存在する
				a = p;
				p = q;
				q = c - gold * ( c - a );
				fp = fq;
				fq = f( q );
			}
			else
			{
				// 区間 a < f( x ) < q の間に最小値が存在する
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


	/// @brief 黄金分割を利用して，f(x)の極小値の一次元探索を行う（1次収束）
	//! 
	//! 初期の極小を含むと思われる区間 [a, b] を適切に設定することで，収束を早めることができる
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[in]  max_iterations … 最大反復回数，実際の反復回数
	//!
	//! @return 極小を与える座標値における評価値
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
	/// @brief Brent の放物線補間を用いて，f(x)の極小値の一次元探索を行う（2次収束）
	//! 
	//! 初期の極小を含むと思われる区間 [a, b] を適切に設定することで，収束を早めることができる
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[out] iterations     … 実際の反復回数
	//! @param[in]  max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	template < class Functor >
	double minimization( double a, double b, double &x, Functor f, double tolerance, size_t &iterations, size_t max_iterations )
	{
		double u, v, w, xm, fa, fb, fu, fv, fw, fx;
		double len, len1, len2;
		const double gold = ( 3.0 - std::sqrt( 5.0 ) ) / 2.0;
		const double dust = type_limits< double >::minimum( );		// ゼロ除算を避けるためのゴミ値

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

		len1 = len2 = type_limits< double >::maximum( );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			xm = ( a + b ) / 2.0;

			// 最後に判定した最小値候補点と，区間 [a, b] の中間との差が許容誤差内になったら終了する
			if( std::abs( xm - x ) < tolerance )
			{
				break;
			}

			// 放物線補間を行う
			double xv = x - v;
			double wx = w - x;
			double fwx = fw - fx;
			double fxv = fx - fv;
			len = ( xv * xv * fwx + wx * wx * fxv ) / ( 2.0 * ( wx * fxv - xv * fwx ) + dust );
			u = x + len;

			// 区間[a, b] 内に入り，
			len = std::abs( len );
			if( ( b - u ) * ( u - a ) > 0.0 && len < len2 / 2.0 )
			{
				// 放物線補間が適切に行われた
			}
			else
			{
				// 放物線補間は不適切なので黄金分割する
				// 区間の大きいほうを黄金分割する
				if( xm < x )
				{
					// 区間 [a, x] を分割する
					u = a + gold * ( x - a );
				}
				else
				{
					// 区間 [x, b] を分割する
					u = x + gold * ( b - x );
				}
			}

			fu = f( u );
			if( fu <= fx )
			{
				// より小さい値が見つかったので a, b, v, w を更新する
				v = w;
				w = x;
				fv = fw;
				fw = fx;
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
				x = u;
				fx = fu;

				len2 = len1;
				len1 = len;
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

	/// @brief Brent の放物線補間を用いて，f(x)の極小値の一次元探索を行う（2次収束）
	//! 
	//! 初期の極小を含むと思われる区間 [a, b] を適切に設定することで，収束を早めることができる
	//! 
	//! @param[in]  a              … 区間の左端
	//! @param[in]  b              … 区間の右端
	//! @param[out] x              … 極小を与える座標値
	//! @param[in]  f              … 評価関数
	//! @param[in]  tolerance      … 許容誤差
	//! @param[in]  max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
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
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor1 > functor( p, dir, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// 勾配方向を計算する
			dir = g( p );

			// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, max_iterations );

			std::cout << p.t( ) << ", " << dir.t( ) << std::endl;

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
		size_t itenum = 0;
		return( minimization( p, f, g, tolerance, itenum, max_iterations ) );
	}

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
	double minimization( matrix< T, Allocator > &p, Functor f, double tolerance, double distance, size_t &iterations, size_t max_iterations )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;
		typedef typename matrix< T, Allocator >::size_type size_type;
		typedef typename matrix< T, Allocator >::difference_type difference_type;
		typedef matrix< T, Allocator > matrix_type;

		matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
		double x, v1, v2, err = 1.0e100, old_err = f( p );
		size_type i;

		// 他変数関数を１変数関数に変換する
		__minimization_utility__::__convert_to_vector_functor__< T, Allocator, Functor > functor( p, dir, f );

		size_t ite;
		for( ite = 1 ; ite <= max_iterations ; ite++ )
		{
			// 勾配方向を計算する
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
			err = brent::minimization( -0.5, 0.5, x, functor, tolerance, max_iterations );

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
		size_t itenum = 0;
		return( minimization( p, f, tolerance, distance, itenum, max_iterations ) );
	}
}

namespace powell
{
	/// @brief Powell 法による多次元変数による極小値の探索を行う
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dir            … 探索に用いる方向集合
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[out]    iterations     … 実際の反復回数
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t &iterations, size_t max_iterations )
	{
		return( 0 );
	}

	/// @brief Powell 法による多次元変数による極小値の探索を行う
	//! 
	//! 手法について何か書く
	//! 
	//! @param[in,out] p              … 探索の開始ベクトル，最小値を与えるベクトル
	//! @param[in,out] dir            … 探索に用いる方向集合
	//! @param[in]     f              … 評価関数
	//! @param[in]     tolerance      … 許容誤差
	//! @param[in]     max_iterations … 最大反復回数
	//!
	//! @return 極小を与える座標値における評価値
	//! 
	template < class T, class Allocator, class Functor >
	double minimization( matrix< T, Allocator > &p, matrix< T, Allocator > &dirs, Functor f, double tolerance, size_t max_iterations = 200 )
	{
		size_t itenum = 0;
		return( minimization( p, dirs, f, tolerance, itenum, max_iterations ) );
	}
}



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MINIMIZATION__
