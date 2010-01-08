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

/// @file mist/calibration.h
//!
//! @brief カメラキャリブレーションを行う関数群
//!
//! - 参考文献
//!   - R. Y. Tsai, ``A Versatile Camera Calibration Technique for High-Accuracy 3D Machine Vision Metrology Using Off-the-Shelf TV Cameras and Lenses,'' IEEE Journal of Robotics and Automation, Vol. RA-3, No. 4, pp. 323--344, 1987.
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


// mist名前空間の始まり
_MIST_BEGIN



//! @defgroup calibration_group カメラキャリブレーション
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/calibration.h>
//! @endcode
//!
//!  @{


/// @brief Tsai のカメラキャリブレーション手法
namespace Tsai
{
	/// @brief キャリブレーションに用いるパラメータ及びキャリブレーション結果を代入する構造体
	struct parameter
	{
		// キャリブレーション前に入力する必要がある変数
		double Ncx;		///< @brief カメラのX軸方向のセンサー素子数 [cell]
		double Ncy;		///< @brief カメラのY軸方向のセンサー素子数 [cell]
		double Nfx;		///< @brief 撮影される画像のX軸方向の画素数 [pixel]
		double Nfy;		///< @brief 撮影される画像のY軸方向の画素数 [pixel]
		double dx;		///< @brief カメラのX軸方向のセンサー素子の大きさ [mm/cell]
		double dy;		///< @brief カメラのY軸方向のセンサー素子の大きさ [mm/cell]
		double Cx;		///< @brief カメラ座標系におけるZ軸と画像平面の交点のX座標（画像中心を与える）[pixel]
		double Cy;		///< @brief カメラ座標系におけるZ軸と画像平面の交点のY座標（画像中心を与える）[pixel]
		double sx;		///< @brief 複数平面を用いたキャリブレーションの際に用いる，X軸方向の解像度を調整する係数

		// キャリブレーション後に得られるパラメータ
		double dpx;		///< @brief カメラのX軸方向の1画素あたりの大きさ [mm/pixel]
		double dpy;		///< @brief カメラのY軸方向の1画素あたりの大きさ [mm/pixel]
		double r1;		///< @brief キャリブレーションの結果得られる回転行列の ( 0, 0 ) 成分
		double r2;		///< @brief キャリブレーションの結果得られる回転行列の ( 0, 1 ) 成分
		double r3;		///< @brief キャリブレーションの結果得られる回転行列の ( 0, 2 ) 成分
		double r4;		///< @brief キャリブレーションの結果得られる回転行列の ( 1, 0 ) 成分
		double r5;		///< @brief キャリブレーションの結果得られる回転行列の ( 1, 1 ) 成分
		double r6;		///< @brief キャリブレーションの結果得られる回転行列の ( 1, 2 ) 成分
		double r7;		///< @brief キャリブレーションの結果得られる回転行列の ( 2, 0 ) 成分
		double r8;		///< @brief キャリブレーションの結果得られる回転行列の ( 2, 1 ) 成分
		double r9;		///< @brief キャリブレーションの結果得られる回転行列の ( 2, 2 ) 成分
		double Tx;		///< @brief キャリブレーションの結果得られるカメラの平行移動のX成分
		double Ty;		///< @brief キャリブレーションの結果得られるカメラの平行移動のY成分
		double Tz;		///< @brief キャリブレーションの結果得られるカメラの平行移動のZ成分
		double f;		///< @brief キャリブレーションの結果得られる焦点距離
		double ka1;		///< @brief キャリブレーションの結果得られる円形の歪成分

		parameter( )
			: Ncx( 640 ), Ncy( 480 ), Nfx( 640 ), Nfy( 640 ), dx( 1.0 ), dy( 1.0 ), Cx( 320 ), Cy( 240 ), sx( 1.0 ),
			  r1( 1.0 ), r2( 0.0 ), r3( 0.0 ), r4( 0.0 ), r5( 1.0 ), r6( 0.0 ), r7( 0.0 ), r8( 0.0 ), r9( 1.0 ),
			  Tx( 0.0 ), Ty( 0.0 ), Tz( 0.0 ), f( 1.0 ), ka1( 0.0 )
		{
		}
	};


	inline std::string to_string( double v, int f1, int f2 )
	{
		char format[ 20 ];
		char buff[ 256 ];

		if( f2 == 0 )
		{
			sprintf( format, "%%f" );
		}
		else
		{
			sprintf( format, "%%%d.%df", f1 + f2 + 1, f2 );
		}

		sprintf( buff, format, v );

		return( buff );
	}

	inline std::string fixed_string( double v, int f1, int f2, size_t len )
	{
		std::string str = to_string( v, f1, f2 );
		size_t i = str.length( );
		for( ; i < len ; i++ )
		{
			str += " ";
		}

		return( str );
	}

	/// @brief カメラキャリブレーションパラメータを整形して出力する
	//! 
	//! @param[in,out] out … 入力と出力を行うストリーム
	//! @param[in]     p   … 出力するキャリブレーションパラメータ
	//! 
	//! @return 入力されたストリーム
	//! 
	inline ::std::ostream &operator <<( ::std::ostream &out, const parameter &p )
	{
		out << "Ncx : " << fixed_string( p.Ncx, 4, 6, 12 ) << " [cell]" << ::std::endl;
		out << "Ncy : " << fixed_string( p.Ncy, 4, 6, 12 ) << " [cell]" << ::std::endl;
		out << "Nfx : " << fixed_string( p.Nfx, 4, 6, 12 ) << " [pixels]" << ::std::endl;
		out << "Nfy : " << fixed_string( p.Nfy, 4, 6, 12 ) << " [pixels]" << ::std::endl;
		out << "dx  : " << fixed_string( p.dx,  4, 6, 12 ) << " [mm/cell]" << ::std::endl;
		out << "dy  : " << fixed_string( p.dy,  4, 6, 12 ) << " [mm/cell]" << ::std::endl;
		out << "dpx : " << fixed_string( p.dpx, 4, 6, 12 ) << " [mm/pixel]" << ::std::endl;
		out << "dpy : " << fixed_string( p.dpy, 4, 6, 12 ) << " [mm/pixel]" << ::std::endl;
		out << "Cx  : " << fixed_string( p.Cx,  4, 6, 12 ) << " [pixel]" << ::std::endl;
		out << "Cy  : " << fixed_string( p.Cy,  4, 6, 12 ) << " [pixel]" << ::std::endl;
		out << "sx  : " << fixed_string( p.sx,  4, 6, 12 ) << ::std::endl;

		out << ::std::endl;
		out << "R =" << ::std::endl;
		out << to_string( p.r1, 2, 6 ) << ", " << to_string( p.r2, 2, 6 ) << ", " << to_string( p.r3, 2, 6 ) << ::std::endl;
		out << to_string( p.r4, 2, 6 ) << ", " << to_string( p.r5, 2, 6 ) << ", " << to_string( p.r6, 2, 6 ) << ::std::endl;
		out << to_string( p.r7, 2, 6 ) << ", " << to_string( p.r8, 2, 6 ) << ", " << to_string( p.r2, 2, 6 ) << ::std::endl;

		out << ::std::endl;
		out << "T [mm] = ( " << p.Tx << ", " << p.Ty << ", " << p.Tz << " )" << ::std::endl;
		out << "focal length = " << p.f << " [mm]" << ::std::endl;
		out << "Tz / f = " << p.Tz / p.f << ::std::endl;
		out << "kappa1 = " << p.ka1 << " [1/mm^2]" << ::std::endl;

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

				//err += std::sqrt( e1 * e1 );
				//err += std::sqrt( e2 * e2 );
				err += std::sqrt( e1 * e1 + e2 * e2 );
			}
			return( err );
		}
	};


	/// @brief Tsai のカメラキャリブレーション
	//! 
	//! @param[in,out] p       … カメラキャリブレーションに用いるパラメータ，及び結果の出力用
	//! @param[in]     world   … キャリブレーション点の実際の座標
	//! @param[in]     image   … キャリブレーション点の画像上の座標
	//! 
	//! @attention 入力のキャリブレーション点のZ座標値はゼロでなくてはならない
	//!
	//! @return キャリブレーションに成功したかどうか
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

		double Ncx = p.Ncx;		// カメラのX軸方向のセンサー素子数 [cell]
		double Ncy = p.Ncy;		// カメラのY軸方向のセンサー素子数 [cell]
		double Nfx = p.Nfx;		// 撮影される画像のX軸方向の画素数 [pixel]
		double Nfy = p.Nfy;		// 撮影される画像のY軸方向の画素数 [pixel]
		double dx  = p.dx;		// カメラのX軸方向のセンサー素子の大きさ [mm/cell]
		double dy  = p.dy;		// カメラのY軸方向のセンサー素子の大きさ [mm/cell]
		double Cx  = p.Cx;		// カメラ座標系におけるZ軸と画像平面の交点のX座標（画像中心を与える）[pixel]
		double Cy  = p.Cy;		// カメラ座標系におけるZ軸と画像平面の交点のY座標（画像中心を与える）[pixel]
		double sx  = p.sx;		// 複数平面を用いたキャリブレーションの際に用いる，X軸方向の解像度を調整する係数

		double dpx = dx * Ncx / Nfx;	// カメラのX軸方向の1画素あたりの大きさ [mm/pixel]
		double dpy = dy * Ncy / Nfy;	// カメラのY軸方向の1画素あたりの大きさ [mm/pixel]

		size_type i, num = image.size( );

		matrix_type A( num, 5 ), B( num, 1 );

		// (i) 画像座標系における点を計算する
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

		// (ii) 5つの未知数を解く
		matrix_type L = inverse( A ) * B;

		// (iii.1) Tyを求める
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


		// (iii.2) Tyの符合を決定する
		{
			// 画像中心から最も離れているものを選択する
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

			// Tyの符号を正にする
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
				// Tyの符号は正にする
			}
			else
			{
				// Tyの符号は負にする
				Ty = -Ty;
			}
		}

		// Tyの計算結果を用いて値を更新する
		r1 = r1_ * Ty;
		r2 = r2_ * Ty;
		r4 = r4_ * Ty;
		r5 = r5_ * Ty;
		Tx = Ty_Tx * Ty;

		//std::cout << "( Tx, Ty ) = ( " << Tx << ", " << Ty << " )" << std::endl;

		// (iii.3) 回転行列Rを決定する
		{
			double s = -1.0 * ( r1 * r4 + r2 * r5 < 0.0 ? -1.0 : 1.0 );

			r3 = std::sqrt( 1.0 - r1 * r1 - r2 * r2 );
			r6 = s * std::sqrt( 1.0 - r4 * r4 - r5 * r5 );
			r7 = r2 * r6 - r3 * r5;

			// 直行行列になるように変形を行う
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

		// (iv) 焦点距離とTzの近似値を，レンズ歪を無視して求める
		for( i = 0 ; i < num ; i++ )
		{
			const point3 &w = world[ i ];
			const point2 &p = image[ i ];
			double Yd = ( p.y - Cy ) * dpy;

			A( i, 0 ) = r4 * w.x + r5 * w.y + r6 * 0 + Ty;
			A( i, 1 ) = -Yd;
			B( i, 0 ) = ( r7 * w.x + r8 * w.y + r9 * 0 ) * Yd;
		}

		L = inverse( A ) * B;

		f = L[ 0 ];
		Tz = L[ 1 ];

		if( f < 0.0 )
		{
			// 求まった焦点距離が負となるため，回転行列を再設定して計算しなおす
			r3 = -r3;
			r6 = -r6;
			r7 = -r7;
			r8 = -r8;

			// 回転行列Rを再決定する
			{
				double Rz = atan2( r4, r1 );
				double s3 = std::sin( Rz );
				double c3 = std::cos( Rz );
				double Ry = atan2( -r7, r1 * c3 + r4 * s3 );
				double Rx = atan2( r3 * s3 - r6 * c3, r5 * c3 - r2 * s3 );

				double s1 = std::sin( Rx );
				double c1 = std::cos( Rx );
				double s2 = std::sin( Ry );
				double c2 = std::cos( Ry );

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

			for( i = 0 ; i < num ; i++ )
			{
				const point3 &w = world[ i ];
				const point2 &p = image[ i ];
				double Yd = ( p.y - Cy ) * dpy;

				A( i, 0 ) = r4 * w.x + r5 * w.y + r6 * 0 + Ty;
				A( i, 1 ) = -Yd;
				B( i, 0 ) = ( r7 * w.x + r8 * w.y + r9 * 0 ) * Yd;
			}

			L = inverse( A ) * B;

			f = L[ 0 ];
			Tz = L[ 1 ];

			if( f < 0 )
			{
				// なんかおかしいね･･･
				return( false );
			}
		}

		// (v) 最急降下法を用いて正確な焦点距離とTzの値を求める
		__parameter__< T1, T2 > param( p, world, image );
		param.dpx = dpx;	// カメラのX軸方向の1画素あたりの大きさ [mm/pixel]
		param.dpy = dpy;	// カメラのY軸方向の1画素あたりの大きさ [mm/pixel]
		param.r1  = r1;		// キャリブレーションの結果得られる回転行列の ( 0, 0 ) 成分
		param.r2  = r2;		// キャリブレーションの結果得られる回転行列の ( 0, 1 ) 成分
		param.r3  = r3;		// キャリブレーションの結果得られる回転行列の ( 0, 2 ) 成分
		param.r4  = r4;		// キャリブレーションの結果得られる回転行列の ( 1, 0 ) 成分
		param.r5  = r5;		// キャリブレーションの結果得られる回転行列の ( 1, 1 ) 成分
		param.r6  = r6;		// キャリブレーションの結果得られる回転行列の ( 1, 2 ) 成分
		param.r7  = r7;		// キャリブレーションの結果得られる回転行列の ( 2, 0 ) 成分
		param.r8  = r8;		// キャリブレーションの結果得られる回転行列の ( 2, 1 ) 成分
		param.r9  = r9;		// キャリブレーションの結果得られる回転行列の ( 2, 2 ) 成分
		param.Tx  = Tx;		// キャリブレーションの結果得られるカメラの平行移動のX成分
		param.Ty  = Ty;		// キャリブレーションの結果得られるカメラの平行移動のY成分
		param.Tz  = Tz;		// キャリブレーションの結果得られるカメラの平行移動のZ成分
		param.f   = f;		// キャリブレーションの結果得られる焦点距離
		param.ka1 = ka1;	// キャリブレーションの結果得られる円形の歪成分

		{
			matrix< double > ppp( 3, 1 ), dirs = matrix_type::identity( 3, 3 );
			ppp[ 0 ] = f;
			ppp[ 1 ] = Tz;
			ppp[ 2 ] = ka1;

			//std::cout << "( f, Tz, ka1 ) = " << ppp.t( ) << std::endl;

			lucidi::minimization( ppp, dirs, param, 1.0e-16 );
			//powell::minimization( ppp, dirs, param, 0.0000001 );
			//gradient::minimization( ppp, param, 0.0000001, 0.01 );

			//std::cout << "( f, Tz, ka1 ) = " << ppp.t( ) << std::endl;

			param.f   = ppp[ 0 ];
			param.Tz  = ppp[ 1 ];
			param.ka1 = ppp[ 2 ];
		}

		p = param;

		if( p.Tz < 0 )
		{
			// カメラの正方向の符号が逆転しているので，その他の符号を正しく設定する
			// なんか知らないけどこれでうまく行くみたい
			// どっかで座標系がひっくり返ったのか？
			p.r1 = -p.r1;
			p.r2 = -p.r2;
			p.r4 = -p.r4;
			p.r5 = -p.r5;
			p.r7 = -p.r7;
			p.r8 = -p.r8;
			p.Tx = -p.Tx;
			p.Ty = -p.Ty;
			p.Tz = -p.Tz;
		}

		return( true );
	}
}



/// @}
//  キャリブレーションの終わり



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_CALIBRATION__
