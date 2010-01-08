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

/// @file mist/registration.h
//!
//! @brief 剛体・非剛体レジストレーション
//!
#ifndef __INCLUDE_MIST_REGISTRATION__
#define __INCLUDE_MIST_REGISTRATION__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_MIST_MINIMIZATION__
#include "minimization.h"
#endif

#ifndef __INCLUDE_MIST_DRAWING__
#include "drawing.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "limits.h"
#endif


#include <vector>


// mist名前空間の始まり
_MIST_BEGIN

namespace __non_rigid_registration_utility__
{
	inline void FFD( double v, double &f0, double &f1, double &f2, double &f3 )
	{
		double v_ = 1.0 - v;
		double v2 = v * v;
		double v3 = v2 * v;
		f0 = v_ * v_ * v_ / 6.0;
		f1 = 0.5 * v3 - v2 + 4.0 / 6.0;
		f2 = ( v2 - v3 + v ) * 0.5 + 1.0 / 6.0;
		f3 = v3 / 6.0;
	}


	// 制御点情報を用いて，ソース画像をターゲット画像に変形する
	template < class TARGETTYPE, class TARGETTYPEA, class SOURCETYPE, class SOURCETYPEA, class CONTROLMESH >
	static void non_rigid_transformation_( array2< TARGETTYPE, TARGETTYPEA > &target, const array2< SOURCETYPE, SOURCETYPEA > &source, const CONTROLMESH &control_mesh,
											typename CONTROLMESH::difference_type mx = -1,
											typename CONTROLMESH::difference_type my = -1,
											typename CONTROLMESH::difference_type mz = -1,
											typename CONTROLMESH::size_type thread_id = 0,
											typename CONTROLMESH::size_type thread_num = 1
										)
	{
		typedef array2< TARGETTYPE, TARGETTYPEA >  target_image_type;
		typedef array2< SOURCETYPE, SOURCETYPEA >  source_image_type;
		typedef CONTROLMESH control_mesh_type;

		typedef typename target_image_type::size_type size_type;
		typedef typename target_image_type::difference_type difference_type;
		typedef typename target_image_type::value_type value_type;
		value_type minimum = type_limits< value_type >::minimum( );

		difference_type i, j;
		double u, v;
		double sx[ 4 ], sy[ 4 ];
		double stepW = control_mesh.width( ) == 1 ? 1.0 : target.width( ) / static_cast< double >( control_mesh.width( ) - 1 );
		double stepH = control_mesh.height( ) == 1 ? 1.0 : target.height( ) / static_cast< double >( control_mesh.height( ) - 1 );
		double _1_stepW = 1.0 / stepW;
		double _1_stepH = 1.0 / stepH;
		double _1_ax = 1.0 / source.reso1( );
		double _1_ay = 1.0 / source.reso2( );


		difference_type d0, d1, d2, d3;
		{
			difference_type cx = source.width( ) / 2;
			difference_type cy = source.height( ) / 2;
			typename source_image_type::const_pointer ppp = &source( cx, cy );
			d0 = 0;
			d1 = &source( cx    , cy + 1 ) - ppp;
			d2 = &source( cx + 1, cy + 1 ) - ppp;
			d3 = &source( cx + 1, cy     ) - ppp;
		}

		difference_type isx, isy, iex, iey;
		difference_type mw = control_mesh.width( );
		difference_type mh = control_mesh.height( );
		if( mx < 0 || mw <= mx || my < 0 || mh <= my )
		{
			isx = 0;
			iex = target.width( ) - 1;
			isy = 0;
			iey = target.height( ) - 1;
		}
		else
		{
			isx = static_cast< difference_type >( ( mx - 2 ) * stepW );
			iex = static_cast< difference_type >( ( mx + 2 ) * stepW );
			isy = static_cast< difference_type >( ( my - 2 ) * stepH );
			iey = static_cast< difference_type >( ( my + 2 ) * stepH );

			difference_type tw = target.width( );
			difference_type th = target.height( );
			isx = isx > 0  ? isx : 0;
			isy = isy > 0  ? isy : 0;
			iex = iex < tw ? iex : tw - 1;
			iey = iey < th ? iey : th - 1;
		}


		for( difference_type y = isy + thread_id ; y <= iey ; y += thread_num )
		{
			v = y * _1_stepH;
			j = static_cast< difference_type >( v );
			v -= j;
			j--;

			FFD( v, sy[ 0 ], sy[ 1 ], sy[ 2 ], sy[ 3 ] );

			for( difference_type x = isx ; x <= iex ; x++ )
			{
				u = x * _1_stepW;
				i = static_cast< difference_type >( u );
				u -= i;
				i--;

				FFD( u, sx[ 0 ], sx[ 1 ], sx[ 2 ], sx[ 3 ] );

				double xx = 0.0, yy = 0.0;
				for( difference_type m = 0 ; m <= 3 ; m++ )
				{
					typename control_mesh_type::const_pointer p = &control_mesh( i, j + m, 0 );
					for( difference_type l = 0 ; l <= 3 ; l++ )
					{
						double val = sx[ l ] * sy[ m ];
						xx += val * p[ l ].x;
						yy += val * p[ l ].y;
					}
				}

				xx *= _1_ax;
				yy *= _1_ay;

				if( 0 <= xx && xx < source.width( ) - 1 && 0 <= yy && yy < source.height( ) - 1 )
				{
					difference_type ixx = static_cast< size_type >( xx );
					difference_type iyy = static_cast< size_type >( yy );
					typename source_image_type::const_pointer p = &source( ixx, iyy, 0 );
					xx -= ixx;
					yy -= iyy;

					target( x, y ) = static_cast< typename target_image_type::value_type >( ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy );
				}
				else
				{
					target( x, y ) = static_cast< typename target_image_type::value_type >( minimum );
				}
			}
		}
	}


	// 制御点情報を用いて，ソース画像をターゲット画像に変形する
	template < class TARGETTYPE, class TARGETTYPEA, class SOURCETYPE, class SOURCETYPEA, class CONTROLMESH >
	static void non_rigid_transformation_( array3< TARGETTYPE, TARGETTYPEA > &target, const array3< SOURCETYPE, SOURCETYPEA > &source, const CONTROLMESH &control_mesh,
											typename CONTROLMESH::difference_type mx = -1,
											typename CONTROLMESH::difference_type my = -1,
											typename CONTROLMESH::difference_type mz = -1,
											typename CONTROLMESH::size_type thread_id = 0,
											typename CONTROLMESH::size_type thread_num = 1
										)
	{
		typedef array3< TARGETTYPE, TARGETTYPEA >  target_image_type;
		typedef array3< SOURCETYPE, SOURCETYPEA >  source_image_type;
		typedef CONTROLMESH control_mesh_type;

		typedef typename target_image_type::size_type size_type;
		typedef typename target_image_type::difference_type difference_type;

		typedef typename target_image_type::value_type value_type;
		value_type minimum = type_limits< value_type >::minimum( );

		difference_type i, j, k;
		double u, v, w;
		double sx[ 4 ], sy[ 4 ], sz[ 4 ];
		double stepW = control_mesh.width( )  == 1 ? 1.0 : target.width( ) / static_cast< double >( control_mesh.width( ) - 1 );
		double stepH = control_mesh.height( ) == 1 ? 1.0 : target.height( ) / static_cast< double >( control_mesh.height( ) - 1 );
		double stepD = control_mesh.depth( )  == 1 ? 1.0 : target.depth( ) / static_cast< double >( control_mesh.depth( ) - 1 );
		double _1_stepW = 1.0 / stepW;
		double _1_stepH = 1.0 / stepH;
		double _1_stepD = 1.0 / stepD;
		double _1_ax = 1.0 / source.reso1( );
		double _1_ay = 1.0 / source.reso2( );
		double _1_az = 1.0 / source.reso3( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7;
		{
			difference_type cx = source.width( ) / 2;
			difference_type cy = source.height( ) / 2;
			difference_type cz = source.depth( ) / 2;
			typename source_image_type::const_pointer ppp = &source( cx, cy, cz );
			d0 = 0;
			d1 = &source( cx    , cy + 1, cz     ) - ppp;
			d2 = &source( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &source( cx + 1, cy    , cz     ) - ppp;
			d4 = &source( cx    , cy    , cz + 1 ) - ppp;
			d5 = &source( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &source( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &source( cx + 1, cy    , cz + 1 ) - ppp;
		}

		difference_type isx, isy, isz, iex, iey, iez;
		difference_type mw = control_mesh.width( );
		difference_type mh = control_mesh.height( );
		difference_type md = control_mesh.depth( );
		if( mx < 0 || mw <= mx || my < 0 || mh <= my || mz < 0 || md <= mz )
		{
			isx = 0;
			iex = target.width( ) - 1;
			isy = 0;
			iey = target.height( ) - 1;
			isz = 0;
			iez = target.depth( ) - 1;
		}
		else
		{
			isx = static_cast< difference_type >( ( mx - 2 ) * stepW );
			iex = static_cast< difference_type >( ( mx + 2 ) * stepW );
			isy = static_cast< difference_type >( ( my - 2 ) * stepH );
			iey = static_cast< difference_type >( ( my + 2 ) * stepH );
			isz = static_cast< difference_type >( ( mz - 2 ) * stepD );
			iez = static_cast< difference_type >( ( mz + 2 ) * stepD );

			difference_type tw = target.width( );
			difference_type th = target.height( );
			difference_type td = target.depth( );
			isx = isx > 0  ? isx : 0;
			isy = isy > 0  ? isy : 0;
			isz = isz > 0  ? isz : 0;
			iex = iex < tw ? iex : tw - 1;
			iey = iey < th ? iey : th - 1;
			iez = iez < td ? iez : td - 1;
		}


		for( difference_type z = isz + thread_id ; z <= iez ; z += thread_num )
		{
			w = z * _1_stepD;
			k = static_cast< difference_type >( w );
			w -= k;
			k--;

			FFD( w, sz[ 0 ], sz[ 1 ], sz[ 2 ], sz[ 3 ] );

			for( difference_type y = isy ; y <= iey ; y++ )
			{
				v = y * _1_stepH;
				j = static_cast< difference_type >( v );
				v -= j;
				j--;

				FFD( v, sy[ 0 ], sy[ 1 ], sy[ 2 ], sy[ 3 ] );

				for( difference_type x = isx ; x <= iex ; x++ )
				{
					u = x * _1_stepW;
					i = static_cast< difference_type >( u );
					u -= i;
					i--;

					FFD( u, sx[ 0 ], sx[ 1 ], sx[ 2 ], sx[ 3 ] );

					double xx = 0.0, yy = 0.0, zz = 0.0;
					for( difference_type n = 0 ; n <= 3 ; n++ )
					{
						for( difference_type m = 0 ; m <= 3 ; m++ )
						{
							typename control_mesh_type::const_pointer p = &control_mesh( i, j + m, k + n );
							double vvv = sy[ m ] * sz[ n ];
							for( difference_type l = 0 ; l <= 3 ; l++ )
							{
								double vv = sx[ l ] * vvv;
								xx += vv * p[ l ].x;
								yy += vv * p[ l ].y;
								zz += vv * p[ l ].z;
							}
						}
					}

					xx *= _1_ax;
					yy *= _1_ay;
					zz *= _1_az;

					if( 0 <= xx && xx < source.width( ) - 1 && 0 <= yy && yy < source.height( ) - 1 && 0 <= zz && zz < source.depth( ) - 1 )
					{
						difference_type ixx = static_cast< size_type >( xx );
						difference_type iyy = static_cast< size_type >( yy );
						difference_type izz = static_cast< size_type >( zz );
						typename source_image_type::const_pointer p = &source( ixx, iyy, izz );
						xx -= ixx;
						yy -= iyy;
						zz -= izz;

						double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;
						target( x, y, z ) = static_cast< typename target_image_type::value_type >( ct );
					}
					else
					{
						target( x, y, z ) = static_cast< typename target_image_type::value_type >( minimum );
					}
				}
			}
		}
	}

	struct ffd_coefficient
	{
		struct coefficient
		{
			double value[ 4 ];
			double &operator []( size_t index ){ return( value[ index ] ); }
			const double &operator []( size_t index ) const { return( value[ index ] ); }
		};

		struct coefficients
		{
			double value[ 16 ];
			double &operator []( size_t index ){ return( value[ index ] ); }
			const double &operator []( size_t index ) const { return( value[ index ] ); }
		};

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef mist::array< coefficient > coefficient_type;

		coefficient_type coeff_z;
		mist::array2< coefficients > coeff_xy;

		template < class CONTROLMESHTYPE >
		void initialize( size_type width, size_type height, size_type depth, const CONTROLMESHTYPE &control_mesh )
		{
			coeff_z.resize( depth );
			coeff_xy.resize( width, height );

			double stepW = control_mesh.width( )  == 1 ? 1.0 : width / static_cast< double >( control_mesh.width( ) - 1 );
			double stepH = control_mesh.height( ) == 1 ? 1.0 : height / static_cast< double >( control_mesh.height( ) - 1 );
			double stepD = control_mesh.depth( )  == 1 ? 1.0 : depth / static_cast< double >( control_mesh.depth( ) - 1 );
			double _1_stepW = 1.0 / stepW;
			double _1_stepH = 1.0 / stepH;
			double _1_stepD = 1.0 / stepD;

			for( size_type z = 0 ; z < coeff_z.size( ) ; z++ )
			{
				double w = z * _1_stepD;
				w -= static_cast< difference_type >( w );

				FFD( w, coeff_z[ z ][ 0 ], coeff_z[ z ][ 1 ], coeff_z[ z ][ 2 ], coeff_z[ z ][ 3 ] );
			}

			double coeff_x[ 4 ], coeff_y[ 4 ];
			for( size_type y = 0 ; y < height ; y++ )
			{
				double v = y * _1_stepH;
				v -= static_cast< difference_type >( v );

				FFD( v, coeff_y[ 0 ], coeff_y[ 1 ], coeff_y[ 2 ], coeff_y[ 3 ] );

				for( size_type x = 0 ; x < width ; x++ )
				{
					double u = x * _1_stepW;
					u -= static_cast< difference_type >( u );

					FFD( u, coeff_x[ 0 ], coeff_x[ 1 ], coeff_x[ 2 ], coeff_x[ 3 ] );

					coeff_xy( x, y )[  0 ] = coeff_x[ 0 ] * coeff_y[ 0 ];
					coeff_xy( x, y )[  1 ] = coeff_x[ 1 ] * coeff_y[ 0 ];
					coeff_xy( x, y )[  2 ] = coeff_x[ 2 ] * coeff_y[ 0 ];
					coeff_xy( x, y )[  3 ] = coeff_x[ 3 ] * coeff_y[ 0 ];
					coeff_xy( x, y )[  4 ] = coeff_x[ 0 ] * coeff_y[ 1 ];
					coeff_xy( x, y )[  5 ] = coeff_x[ 1 ] * coeff_y[ 1 ];
					coeff_xy( x, y )[  6 ] = coeff_x[ 2 ] * coeff_y[ 1 ];
					coeff_xy( x, y )[  7 ] = coeff_x[ 3 ] * coeff_y[ 1 ];
					coeff_xy( x, y )[  8 ] = coeff_x[ 0 ] * coeff_y[ 2 ];
					coeff_xy( x, y )[  9 ] = coeff_x[ 1 ] * coeff_y[ 2 ];
					coeff_xy( x, y )[ 10 ] = coeff_x[ 2 ] * coeff_y[ 2 ];
					coeff_xy( x, y )[ 11 ] = coeff_x[ 3 ] * coeff_y[ 2 ];
					coeff_xy( x, y )[ 12 ] = coeff_x[ 0 ] * coeff_y[ 3 ];
					coeff_xy( x, y )[ 13 ] = coeff_x[ 1 ] * coeff_y[ 3 ];
					coeff_xy( x, y )[ 14 ] = coeff_x[ 2 ] * coeff_y[ 3 ];
					coeff_xy( x, y )[ 15 ] = coeff_x[ 3 ] * coeff_y[ 3 ];
				}
			}
		}
	};



	// 制御点情報を用いて，ソース画像をターゲット画像に変形する
	template < class TARGETTYPE, class TARGETTYPEA, class SOURCETYPE, class SOURCETYPEA, class CONTROLMESH >
	static void non_rigid_transformation_( array2< TARGETTYPE, TARGETTYPEA > &target, const array2< SOURCETYPE, SOURCETYPEA > &source, const CONTROLMESH &control_mesh,
											const ffd_coefficient &ffd_coeff,
											typename CONTROLMESH::difference_type mx = -1,
											typename CONTROLMESH::difference_type my = -1,
											typename CONTROLMESH::difference_type mz = -1,
											typename CONTROLMESH::size_type thread_id = 0,
											typename CONTROLMESH::size_type thread_num = 1
										)
	{
		typedef array2< TARGETTYPE, TARGETTYPEA >  target_image_type;
		typedef array2< SOURCETYPE, SOURCETYPEA >  source_image_type;
		typedef CONTROLMESH control_mesh_type;

		typedef typename target_image_type::size_type size_type;
		typedef typename target_image_type::difference_type difference_type;
		typedef typename target_image_type::value_type value_type;
		value_type minimum = type_limits< value_type >::minimum( );

		difference_type i, j;
		double stepW = control_mesh.width( ) == 1 ? 1.0 : target.width( ) / static_cast< double >( control_mesh.width( ) - 1 );
		double stepH = control_mesh.height( ) == 1 ? 1.0 : target.height( ) / static_cast< double >( control_mesh.height( ) - 1 );
		double _1_stepW = 1.0 / stepW;
		double _1_stepH = 1.0 / stepH;
		double _1_ax = 1.0 / source.reso1( );
		double _1_ay = 1.0 / source.reso2( );

		difference_type d0, d1, d2, d3, dm;
		{
			difference_type cx = source.width( ) / 2;
			difference_type cy = source.height( ) / 2;
			typename source_image_type::const_pointer ppp = &source( cx, cy );
			d0 = 0;
			d1 = &source( cx    , cy + 1 ) - ppp;
			d2 = &source( cx + 1, cy + 1 ) - ppp;
			d3 = &source( cx + 1, cy     ) - ppp;
			dm = &control_mesh( 0, 1, 0 ) - &control_mesh( 0, 0, 0 );
		}

		difference_type isx, isy, iex, iey;
		difference_type mw = control_mesh.width( );
		difference_type mh = control_mesh.height( );
		if( mx < 0 || mw <= mx || my < 0 || mh <= my )
		{
			isx = 0;
			iex = target.width( ) - 1;
			isy = 0;
			iey = target.height( ) - 1;
		}
		else
		{
			isx = static_cast< difference_type >( ( mx - 2 ) * stepW );
			iex = static_cast< difference_type >( ( mx + 2 ) * stepW );
			isy = static_cast< difference_type >( ( my - 2 ) * stepH );
			iey = static_cast< difference_type >( ( my + 2 ) * stepH );

			difference_type tw = target.width( );
			difference_type th = target.height( );
			isx = isx > 0  ? isx : 0;
			isy = isy > 0  ? isy : 0;
			iex = iex < tw ? iex : tw - 1;
			iey = iey < th ? iey : th - 1;
		}


		for( difference_type y = isy + thread_id ; y <= iey ; y += thread_num )
		{
			j = static_cast< difference_type >( y * _1_stepH ) - 1;

			for( difference_type x = isx ; x <= iex ; x++ )
			{
				i = static_cast< difference_type >( x * _1_stepW ) - 1;

				const ffd_coefficient::coefficients &coeff = ffd_coeff.coeff_xy( x, y );

				double xx = 0.0, yy = 0.0;
				typename control_mesh_type::const_pointer p = &control_mesh( i, j, 0 );

				xx += coeff[  0 ] * p[ 0 ].x; yy += coeff[  0 ] * p[ 0 ].y;
				xx += coeff[  1 ] * p[ 1 ].x; yy += coeff[  1 ] * p[ 1 ].y;
				xx += coeff[  2 ] * p[ 2 ].x; yy += coeff[  2 ] * p[ 2 ].y;
				xx += coeff[  3 ] * p[ 3 ].x; yy += coeff[  3 ] * p[ 3 ].y;

				p += dm;

				xx += coeff[  4 ] * p[ 0 ].x; yy += coeff[  4 ] * p[ 0 ].y;
				xx += coeff[  5 ] * p[ 1 ].x; yy += coeff[  5 ] * p[ 1 ].y;
				xx += coeff[  6 ] * p[ 2 ].x; yy += coeff[  6 ] * p[ 2 ].y;
				xx += coeff[  7 ] * p[ 3 ].x; yy += coeff[  7 ] * p[ 3 ].y;

				p += dm;

				xx += coeff[  8 ] * p[ 0 ].x; yy += coeff[  8 ] * p[ 0 ].y;
				xx += coeff[  9 ] * p[ 1 ].x; yy += coeff[  9 ] * p[ 1 ].y;
				xx += coeff[ 10 ] * p[ 2 ].x; yy += coeff[ 10 ] * p[ 2 ].y;
				xx += coeff[ 11 ] * p[ 3 ].x; yy += coeff[ 11 ] * p[ 3 ].y;

				p += dm;

				xx += coeff[ 12 ] * p[ 0 ].x; yy += coeff[ 12 ] * p[ 0 ].y;
				xx += coeff[ 13 ] * p[ 1 ].x; yy += coeff[ 13 ] * p[ 1 ].y;
				xx += coeff[ 14 ] * p[ 2 ].x; yy += coeff[ 14 ] * p[ 2 ].y;
				xx += coeff[ 15 ] * p[ 3 ].x; yy += coeff[ 15 ] * p[ 3 ].y;

				xx *= _1_ax;
				yy *= _1_ay;

				if( 0 <= xx && xx < source.width( ) - 1 && 0 <= yy && yy < source.height( ) - 1 )
				{
					difference_type ixx = static_cast< size_type >( xx );
					difference_type iyy = static_cast< size_type >( yy );
					typename source_image_type::const_pointer p = &source( ixx, iyy, 0 );
					xx -= ixx;
					yy -= iyy;

					target( x, y ) = static_cast< typename target_image_type::value_type >( ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy );
				}
				else
				{
					target( x, y ) = static_cast< typename target_image_type::value_type >( minimum );
				}
			}
		}
	}



	// 制御点情報を用いて，ソース画像をターゲット画像に変形する
	template < class TARGETTYPE, class TARGETTYPEA, class SOURCETYPE, class SOURCETYPEA, class CONTROLMESH >
	static void non_rigid_transformation_( array3< TARGETTYPE, TARGETTYPEA > &target, const array3< SOURCETYPE, SOURCETYPEA > &source, const CONTROLMESH &control_mesh,
											const ffd_coefficient &ffd_coeff,
											typename CONTROLMESH::difference_type mx = -1,
											typename CONTROLMESH::difference_type my = -1,
											typename CONTROLMESH::difference_type mz = -1,
											typename CONTROLMESH::size_type thread_id = 0,
											typename CONTROLMESH::size_type thread_num = 1
										)
	{
		typedef array3< TARGETTYPE, TARGETTYPEA >  target_image_type;
		typedef array3< SOURCETYPE, SOURCETYPEA >  source_image_type;
		typedef CONTROLMESH control_mesh_type;

		typedef typename target_image_type::size_type size_type;
		typedef typename target_image_type::difference_type difference_type;
		typedef typename target_image_type::value_type value_type;
		value_type minimum = type_limits< value_type >::minimum( );

		difference_type i, j, k;
		double sz[ 4 ];
		double stepW = control_mesh.width( )  == 1 ? 1.0 : target.width( ) / static_cast< double >( control_mesh.width( ) - 1 );
		double stepH = control_mesh.height( ) == 1 ? 1.0 : target.height( ) / static_cast< double >( control_mesh.height( ) - 1 );
		double stepD = control_mesh.depth( )  == 1 ? 1.0 : target.depth( ) / static_cast< double >( control_mesh.depth( ) - 1 );
		double _1_stepW = 1.0 / stepW;
		double _1_stepH = 1.0 / stepH;
		double _1_stepD = 1.0 / stepD;
		double _1_ax = 1.0 / source.reso1( );
		double _1_ay = 1.0 / source.reso2( );
		double _1_az = 1.0 / source.reso3( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7, dm;
		{
			difference_type cx = source.width( ) / 2;
			difference_type cy = source.height( ) / 2;
			difference_type cz = source.depth( ) / 2;
			typename source_image_type::const_pointer ppp = &source( cx, cy, cz );
			d0 = 0;
			d1 = &source( cx    , cy + 1, cz     ) - ppp;
			d2 = &source( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &source( cx + 1, cy    , cz     ) - ppp;
			d4 = &source( cx    , cy    , cz + 1 ) - ppp;
			d5 = &source( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &source( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &source( cx + 1, cy    , cz + 1 ) - ppp;
			dm = &control_mesh( 0, 1, 0 ) - &control_mesh( 0, 0, 0 );
		}

		difference_type isx, isy, isz, iex, iey, iez;
		difference_type mw = control_mesh.width( );
		difference_type mh = control_mesh.height( );
		difference_type md = control_mesh.depth( );
		if( mx < 0 || mw <= mx || my < 0 || mh <= my || mz < 0 || md <= mz )
		{
			isx = 0;
			iex = target.width( ) - 1;
			isy = 0;
			iey = target.height( ) - 1;
			isz = 0;
			iez = target.depth( ) - 1;
		}
		else
		{
			isx = static_cast< difference_type >( ( mx - 2 ) * stepW );
			iex = static_cast< difference_type >( ( mx + 2 ) * stepW );
			isy = static_cast< difference_type >( ( my - 2 ) * stepH );
			iey = static_cast< difference_type >( ( my + 2 ) * stepH );
			isz = static_cast< difference_type >( ( mz - 2 ) * stepD );
			iez = static_cast< difference_type >( ( mz + 2 ) * stepD );

			difference_type tw = target.width( );
			difference_type th = target.height( );
			difference_type td = target.depth( );
			isx = isx > 0  ? isx : 0;
			isy = isy > 0  ? isy : 0;
			isz = isz > 0  ? isz : 0;
			iex = iex < tw ? iex : tw - 1;
			iey = iey < th ? iey : th - 1;
			iez = iez < td ? iez : td - 1;
		}

		typedef ffd_coefficient::coefficient_type coefficient_type;
		const coefficient_type &ffd_coeff_z = ffd_coeff.coeff_z;

		for( difference_type z = isz + thread_id ; z <= iez ; z += thread_num )
		{
			k = static_cast< difference_type >( z * _1_stepD ) - 1;

			sz[ 0 ] = ffd_coeff_z[ z ][ 0 ];
			sz[ 1 ] = ffd_coeff_z[ z ][ 1 ];
			sz[ 2 ] = ffd_coeff_z[ z ][ 2 ];
			sz[ 3 ] = ffd_coeff_z[ z ][ 3 ];

			for( difference_type y = isy ; y <= iey ; y++ )
			{
				j = static_cast< difference_type >( y * _1_stepH ) - 1;

				for( difference_type x = isx ; x <= iex ; x++ )
				{
					i = static_cast< difference_type >( x * _1_stepW ) - 1;

					double xx = 0.0, yy = 0.0, zz = 0.0, val;
					const ffd_coefficient::coefficients &coeff = ffd_coeff.coeff_xy( x, y );

					for( difference_type n = 0 ; n <= 3 ; n++ )
					{
						typename control_mesh_type::const_pointer p = &control_mesh( i, j, k + n );

						val = sz[ n ] * coeff[  0 ]; xx += val * p[ 0 ].x; yy += val * p[ 0 ].y; zz += val * p[ 0 ].z;
						val = sz[ n ] * coeff[  1 ]; xx += val * p[ 1 ].x; yy += val * p[ 1 ].y; zz += val * p[ 1 ].z;
						val = sz[ n ] * coeff[  2 ]; xx += val * p[ 2 ].x; yy += val * p[ 2 ].y; zz += val * p[ 2 ].z;
						val = sz[ n ] * coeff[  3 ]; xx += val * p[ 3 ].x; yy += val * p[ 3 ].y; zz += val * p[ 3 ].z;

						p += dm;

						val = sz[ n ] * coeff[  4 ]; xx += val * p[ 0 ].x; yy += val * p[ 0 ].y; zz += val * p[ 0 ].z;
						val = sz[ n ] * coeff[  5 ]; xx += val * p[ 1 ].x; yy += val * p[ 1 ].y; zz += val * p[ 1 ].z;
						val = sz[ n ] * coeff[  6 ]; xx += val * p[ 2 ].x; yy += val * p[ 2 ].y; zz += val * p[ 2 ].z;
						val = sz[ n ] * coeff[  7 ]; xx += val * p[ 3 ].x; yy += val * p[ 3 ].y; zz += val * p[ 3 ].z;

						p += dm;

						val = sz[ n ] * coeff[  8 ]; xx += val * p[ 0 ].x; yy += val * p[ 0 ].y; zz += val * p[ 0 ].z;
						val = sz[ n ] * coeff[  9 ]; xx += val * p[ 1 ].x; yy += val * p[ 1 ].y; zz += val * p[ 1 ].z;
						val = sz[ n ] * coeff[ 10 ]; xx += val * p[ 2 ].x; yy += val * p[ 2 ].y; zz += val * p[ 2 ].z;
						val = sz[ n ] * coeff[ 11 ]; xx += val * p[ 3 ].x; yy += val * p[ 3 ].y; zz += val * p[ 3 ].z;

						p += dm;

						val = sz[ n ] * coeff[ 12 ]; xx += val * p[ 0 ].x; yy += val * p[ 0 ].y; zz += val * p[ 0 ].z;
						val = sz[ n ] * coeff[ 13 ]; xx += val * p[ 1 ].x; yy += val * p[ 1 ].y; zz += val * p[ 1 ].z;
						val = sz[ n ] * coeff[ 14 ]; xx += val * p[ 2 ].x; yy += val * p[ 2 ].y; zz += val * p[ 2 ].z;
						val = sz[ n ] * coeff[ 15 ]; xx += val * p[ 3 ].x; yy += val * p[ 3 ].y; zz += val * p[ 3 ].z;
					}

					xx *= _1_ax;
					yy *= _1_ay;
					zz *= _1_az;

					if( 0 <= xx && xx < source.width( ) - 1 && 0 <= yy && yy < source.height( ) - 1 && 0 <= zz && zz < source.depth( ) - 1 )
					{
						difference_type ixx = static_cast< size_type >( xx );
						difference_type iyy = static_cast< size_type >( yy );
						difference_type izz = static_cast< size_type >( zz );
						typename source_image_type::const_pointer p = &source( ixx, iyy, izz );
						xx -= ixx;
						yy -= iyy;
						zz -= izz;

						double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;
						target( x, y, z ) = static_cast< typename target_image_type::value_type >( ct );
					}
					else
					{
						target( x, y, z ) = static_cast< typename target_image_type::value_type >( minimum );
					}
				}
			}
		}
	}


	template < class TARGETTYPE, class SOURCETYPE, class CONTROLMESH >
	class non_rigid_registration_thread : public thread< non_rigid_registration_thread< TARGETTYPE, SOURCETYPE, CONTROLMESH > >
	{
	public:
		typedef TARGETTYPE  target_image_type;
		typedef SOURCETYPE  source_image_type;
		typedef CONTROLMESH control_mesh_type;
		typedef thread< non_rigid_registration_thread< TARGETTYPE, SOURCETYPE, CONTROLMESH > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename TARGETTYPE::size_type size_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		target_image_type *target_;
		const source_image_type *source_;
		const control_mesh_type *control_mesh_;
		size_type mx_;
		size_type my_;
		size_type mz_;
		const ffd_coefficient *ffd_coefficient_;

	private:
		const non_rigid_registration_thread& operator =( const non_rigid_registration_thread &p );

	public:
		void setup_parameters( target_image_type &target, const source_image_type &source, const control_mesh_type &control_mesh,
								const ffd_coefficient &ffd_coefficient__, size_t mx, size_type my, size_type mz, size_type thread_id, size_type thread_num )
		{
			target_  = &target;
			source_ = &source;
			control_mesh_ = &control_mesh;
			ffd_coefficient_ = &ffd_coefficient__;
			mx_ = mx;
			my_ = my;
			mz_ = mz;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		void setup_parameters( target_image_type &target, const source_image_type &source, const control_mesh_type &control_mesh,
												size_t mx, size_type my, size_type mz, size_type thread_id, size_type thread_num )
		{
			target_  = &target;
			source_ = &source;
			control_mesh_ = &control_mesh;
			ffd_coefficient_ = NULL;
			mx_ = mx;
			my_ = my;
			mz_ = mz;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		non_rigid_registration_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
			target_( NULL ), source_( NULL ), control_mesh_( NULL ), mx_( -1 ), my_( -1 ), mz_( -1 ), ffd_coefficient_( NULL )
		{
		}

		non_rigid_registration_thread( const non_rigid_registration_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
			target_( p.target_ ), source_( p.source_ ), control_mesh_( p.control_mesh_ ), mx_( p.mx_ ), my_( p.my_ ), mz_( p.mz_ ), ffd_coefficient_( p.ffd_coefficient_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			if( ffd_coefficient_ != NULL )
			{
				non_rigid_transformation_( *target_, *source_, *control_mesh_, *ffd_coefficient_, mx_, my_, mz_, thread_id_, thread_num_ );
			}
			else
			{
				non_rigid_transformation_( *target_, *source_, *control_mesh_, mx_, my_, mz_, thread_id_, thread_num_ );
			}
			return( true );
		}
	};


	// 制御点情報を用いて，ソース画像をターゲット画像に変形する
	template < class TARGETTYPE, class SOURCETYPE, class CONTROLMESH >
	static bool transformation( TARGETTYPE &target, const SOURCETYPE &source, const CONTROLMESH &control_mesh,
									typename CONTROLMESH::difference_type mx = -1,
									typename CONTROLMESH::difference_type my = -1,
									typename CONTROLMESH::difference_type mz = -1,
									typename CONTROLMESH::size_type thread_num = 0
								)
	{
		if( is_same_object( target, source ) || target.empty( ) || source.empty( ) )
		{
			return( false );
		}

		typedef TARGETTYPE target_image_type;
		typedef SOURCETYPE source_image_type;
		typedef CONTROLMESH control_mesh_type;
		typedef typename target_image_type::size_type  size_type;
		typedef non_rigid_registration_thread< target_image_type, source_image_type, control_mesh_type > _non_rigid_registration_thread_;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		_non_rigid_registration_thread_ *thread = new _non_rigid_registration_thread_[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( target, source, control_mesh, mx, my, mz, i, thread_num );
		}

		// スレッドを実行して，終了まで待機する
		do_threads_( thread, thread_num );

		delete [] thread;
		
		return( true );
	}

	// 制御点情報を用いて，ソース画像をターゲット画像に変形する
	template < class TARGETTYPE, class SOURCETYPE, class CONTROLMESH >
	static bool transformation( TARGETTYPE &target, const SOURCETYPE &source, const CONTROLMESH &control_mesh, const ffd_coefficient &ffd_coeff,
									typename CONTROLMESH::difference_type mx = -1,
									typename CONTROLMESH::difference_type my = -1,
									typename CONTROLMESH::difference_type mz = -1,
									typename CONTROLMESH::size_type thread_num = 0
								)
	{
		if( is_same_object( target, source ) || target.empty( ) || source.empty( ) )
		{
			return( false );
		}

		typedef TARGETTYPE target_image_type;
		typedef SOURCETYPE source_image_type;
		typedef CONTROLMESH control_mesh_type;
		typedef typename target_image_type::size_type  size_type;
		typedef non_rigid_registration_thread< target_image_type, source_image_type, control_mesh_type > _non_rigid_registration_thread_;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		_non_rigid_registration_thread_ *thread = new _non_rigid_registration_thread_[ thread_num ];

		size_type i;
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( target, source, control_mesh, ffd_coeff, mx, my, mz, i, thread_num );
		}

		// スレッドを実行して，終了まで待機する
		do_threads_( thread, thread_num );

		delete [] thread;
		
		return( true );
	}



	template < class TARGETTYPE, class SOURCETYPE, class CONTROLMESH >
	struct registration_functor : public thread< registration_functor< TARGETTYPE, SOURCETYPE, CONTROLMESH > >
	{
		typedef TARGETTYPE  target_image_type;
		typedef SOURCETYPE  source_image_type;
		typedef CONTROLMESH control_mesh_type;
		typedef typename TARGETTYPE::size_type size_type;
		typedef typename TARGETTYPE::difference_type difference_type;
		typedef typename CONTROLMESH::value_type vector_type;
		typedef matrix< double > matrix_type;							///< @brief 内部で利用する行列の型
		typedef thread< registration_functor< TARGETTYPE, SOURCETYPE, CONTROLMESH > > base;
		typedef typename base::thread_exit_type thread_exit_type;

		array< unsigned int * > target;
		target_image_type transformed_image;		///< @brief レジストレーション時に利用し，制御点情報を用いてソースを目標画像へ変換した一時画像
		const source_image_type &source;
		const control_mesh_type &control_mesh;
		control_mesh_type control_mesh_tmp;
		difference_type x, y, z;

		array2< unsigned int > h;
		array< unsigned int > hh;
		array< unsigned int > __no_data_is_associated__;

		difference_type minimum;
		difference_type maximum;
		size_type BIN;
		ffd_coefficient ffd_coeff;

		double H1;

		template < class IMAGETYPE >
		static difference_type get_minimum( const IMAGETYPE &image )
		{
			typedef typename IMAGETYPE::value_type value_type;
			value_type min = image[ 0 ];
			for( size_type i = 1 ; i < image.size( ) ; i++ )
			{
				if( min > image[ i ] )
				{
					min = image[ i ];
				}
			}
			return( static_cast< difference_type >( min ) );
		}

		template < class T >
		static const T get_minimum( const T &v1, const T &v2 )
		{
			return( v1 < v2 ? v1 : v2 );
		}

		template < class IMAGETYPE >
		static difference_type get_maximum( const IMAGETYPE &image )
		{
			typedef typename IMAGETYPE::value_type value_type;
			value_type max = image[ 0 ];
			for( size_type i = 1 ; i < image.size( ) ; i++ )
			{
				if( max < image[ i ] )
				{
					max = image[ i ];
				}
			}
			return( static_cast< difference_type >( max ) );
		}

		template < class T >
		static const T get_maximum( const T &v1, const T &v2 )
		{
			return( v1 > v2 ? v1 : v2 );
		}

		registration_functor( const target_image_type &tgt, const source_image_type &src, const control_mesh_type &cmesh, size_type bin )
			: target( tgt.size( ) ), transformed_image( tgt ), source( src ), control_mesh ( cmesh ), control_mesh_tmp( cmesh ), x( 0 ), y( 0 ), z( 0 ), BIN( bin )
		{
			// FFD 用の係数テーブルを作成する
			ffd_coeff.initialize( tgt.width( ), tgt.height( ), tgt.depth( ), control_mesh );

			// 初期画像を作成する
			transformation( transformed_image, source, control_mesh, ffd_coeff );

			minimum = get_maximum( get_minimum( get_minimum( tgt ), get_minimum( src ) ), static_cast< difference_type >(-1000) );
			maximum = get_maximum( get_maximum( tgt ), get_maximum( src ) );
			h.resize( ( maximum - minimum + 1 ) / BIN + 1, ( maximum - minimum + 1 ) / BIN + 1 );
			hh.resize( ( maximum - minimum + 1 ) / BIN + 1, 1 );
			__no_data_is_associated__.resize( ( maximum - minimum + 1 ) / BIN + 1, 1 );

			size_type count = 0;
			difference_type upper = h.width( );
			double _1_bin = 1.0 / BIN;
			for( size_t i = 0 ; i < tgt.size( ) ; i++ )
			{
				difference_type v1 = static_cast< difference_type >( ( tgt[ i ] - minimum ) * _1_bin );
				if( v1 < 0 || upper <= v1 )
				{
					target[ i ] = &__no_data_is_associated__[ 0 ];
					continue;
				}
				target[ i ] = &h( 0, v1 );
				hh[ v1 ]++;
				count++;
			}

			H1 = 0.0;

			// ヒストグラムの正規化
			if( count == 0.0 )
			{
				count = 1;
			}

			for( size_type i = 0 ; i < hh.size( ) ; i++ )
			{
				if( hh[ i ] > 0 )
				{
					double v = hh[ i ] / static_cast< double >( count );
					H1 -= v * std::log( v );
				}
			}
		}

		void force_initialize( )
		{
			ffd_coeff.initialize( transformed_image.width( ), transformed_image.height( ), transformed_image.depth( ), control_mesh );
		}

		void initialize( difference_type i, difference_type j, difference_type k )
		{
			x = i;
			y = j;
			z = k;
			control_mesh_tmp = control_mesh;
			transformation( transformed_image, source, control_mesh, ffd_coeff );
		}

		void apply_control_point_to_mesh( control_mesh_type &cmesh ) const
		{
			cmesh( x, y, z ) = control_mesh_tmp( x, y, z );
		}

		template < class PARAMETER >
		double operator ()( const PARAMETER &p )
		{
			return( evaluate_error( p, false ) );
		}

		template < class PARAMETER >
		double evaluate_error( const PARAMETER &p, bool use_thread = true )
		{
			control_mesh_tmp( x, y, z ).x = control_mesh( x, y, z ).x + p[ 0 ];
			control_mesh_tmp( x, y, z ).y = control_mesh( x, y, z ).y + p[ 1 ];
			control_mesh_tmp( x, y, z ).z = control_mesh( x, y, z ).z + p[ 2 ];

			if( !use_thread )
			{
				non_rigid_transformation_( transformed_image, source, control_mesh_tmp, ffd_coeff, x, y, z );
			}
			else
			{
				transformation( transformed_image, source, control_mesh_tmp, ffd_coeff, x, y, z );
			}

			// データを初期化する
			__no_data_is_associated__.fill( );
			h.fill( );
			hh.fill( );
			size_type count = 0;
			double _1_bin = 1.0 / BIN;

			for( size_t i = 0 ; i < target.size( ) ; i++ )
			{
				difference_type v2 = static_cast< difference_type >( ( transformed_image[ i ] - minimum ) * _1_bin );
				if( v2 < 0 )
				{
					continue;
				}

				target[ i ][ v2 ]++;
				hh[ v2 ]++;
				count++;
			}

			// ヒストグラムの正規化
			if( count == 0 )
			{
				return ( 5.0 );
			}

			double H2 = 0.0, H12 = 0.0, _1_count = 1.0 / static_cast< double >( count );

			for( size_type i = 0 ; i < hh.size( ) ; i++ )
			{
				if( hh[ i ] > 0 )
				{
					double v = hh[ i ] * _1_count;
					H2 -= v * std::log( v );

					array2< unsigned int >::const_pointer ph = &h( 0, i );
					for( size_type j = 0 ; j < hh.size( ) ; j++ )
					{
						if( ph[ j ] > 0 )
						{
							double v = ph[ j ] * _1_count;
							H12 -= v * std::log( v );
						}
					}
				}
			}

			return( -( H1 + H2 ) / H12 );
			//return( H1 + H2 - H12 );
		}

	protected:
		// 最小のバウンディングボックス用の距離を計算する
		double min_range( const double &v1, const double &v2, const double &v3 ) const
		{
			double v = v1 < v2 ? v1 : v2;
			v = v < v3 ? v : v3;
			return( v );
		}

		double max_range( const double &v1, const double &v2, const double &v3 ) const
		{
			double v = v1 > v2 ? v1 : v2;
			v = v > v3 ? v : v3;
			return( v );
		}

		double min_range( const double &v1, const double &v2, const double &v3,
							const double &v4, const double &v5, const double &v6,
							const double &v7, const double &v8, const double &v9 ) const
		{
			double vv1 = min_range( v1, v2, v3 );
			double vv2 = min_range( v4, v5, v6 );
			double vv3 = min_range( v7, v8, v9 );
			return( min_range( vv1, vv2, vv3 ) );
		}

		double max_range( const double &v1, const double &v2, const double &v3,
							const double &v4, const double &v5, const double &v6,
							const double &v7, const double &v8, const double &v9 ) const
		{
			double vv1 = max_range( v1, v2, v3 );
			double vv2 = max_range( v4, v5, v6 );
			double vv3 = max_range( v7, v8, v9 );
			return( max_range( vv1, vv2, vv3 ) );
		}

		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			typedef __minimization_utility__::__no_copy_constructor_functor__< registration_functor< TARGETTYPE, SOURCETYPE, CONTROLMESH > > no_constructor_functor_type;

			// 最小化を開始
			double search_length = 0.95;
			matrix_type p( 3, 1 ), dirs = matrix_type::identity( 3, 3 ), bound( 3, 2 );

			//const vector_type &p01 = control_mesh( x - 1, y - 1, z - 1 );
			//const vector_type &p02 = control_mesh( x    , y - 1, z - 1 );
			//const vector_type &p03 = control_mesh( x + 1, y - 1, z - 1 );
			//const vector_type &p04 = control_mesh( x - 1, y    , z - 1 );
			//const vector_type &p05 = control_mesh( x    , y    , z - 1 );
			//const vector_type &p06 = control_mesh( x + 1, y    , z - 1 );
			//const vector_type &p07 = control_mesh( x - 1, y + 1, z - 1 );
			//const vector_type &p08 = control_mesh( x    , y + 1, z - 1 );
			//const vector_type &p09 = control_mesh( x + 1, y + 1, z - 1 );
			//const vector_type &p11 = control_mesh( x - 1, y - 1, z     );
			//const vector_type &p12 = control_mesh( x    , y - 1, z     );
			//const vector_type &p13 = control_mesh( x + 1, y - 1, z     );
			//const vector_type &p14 = control_mesh( x - 1, y    , z     );
			//const vector_type &p15 = control_mesh( x    , y    , z     );
			//const vector_type &p16 = control_mesh( x + 1, y    , z     );
			//const vector_type &p17 = control_mesh( x - 1, y + 1, z     );
			//const vector_type &p18 = control_mesh( x    , y + 1, z     );
			//const vector_type &p19 = control_mesh( x + 1, y + 1, z     );
			//const vector_type &p21 = control_mesh( x - 1, y - 1, z + 1 );
			//const vector_type &p22 = control_mesh( x    , y - 1, z + 1 );
			//const vector_type &p23 = control_mesh( x + 1, y - 1, z + 1 );
			//const vector_type &p24 = control_mesh( x - 1, y    , z + 1 );
			//const vector_type &p25 = control_mesh( x    , y    , z + 1 );
			//const vector_type &p26 = control_mesh( x + 1, y    , z + 1 );
			//const vector_type &p27 = control_mesh( x - 1, y + 1, z + 1 );
			//const vector_type &p28 = control_mesh( x    , y + 1, z + 1 );
			//const vector_type &p29 = control_mesh( x + 1, y + 1, z + 1 );

			//double lower_x = max_range( p01.x, p04.x, p07.x, p11.x, p14.x, p17.x, p21.x, p24.x, p27.x );
			//double lower_y = max_range( p01.y, p02.y, p03.y, p11.y, p12.y, p13.y, p21.y, p22.y, p23.y );
			//double lower_z = max_range( p01.z, p02.z, p03.z, p04.z, p05.z, p06.z, p07.z, p08.z, p09.z );
			//double upper_x = min_range( p03.x, p06.x, p09.x, p13.x, p16.x, p19.x, p23.x, p26.x, p29.x );
			//double upper_y = min_range( p07.y, p08.y, p09.y, p17.y, p18.y, p19.y, p27.y, p28.y, p29.y );
			//double upper_z = min_range( p21.z, p22.z, p23.z, p24.z, p25.z, p26.z, p27.z, p28.z, p29.z );


			//bound( 0, 0 ) = ( lower_x - p15.x ) * search_length;
			//bound( 0, 1 ) = ( upper_x - p15.x ) * search_length;
			//bound( 1, 0 ) = ( lower_y - p15.y ) * search_length;
			//bound( 1, 1 ) = ( upper_y - p15.y ) * search_length;
			//bound( 2, 0 ) = ( lower_z - p15.z ) * search_length;
			//bound( 2, 1 ) = ( upper_z - p15.z ) * search_length;
			bound( 0, 0 ) = ( control_mesh( x - 1, y    , z     ).x - control_mesh( x, y, z ).x ) * search_length;
			bound( 0, 1 ) = ( control_mesh( x + 1, y    , z     ).x - control_mesh( x, y, z ).x ) * search_length;
			bound( 1, 0 ) = ( control_mesh( x    , y - 1, z     ).y - control_mesh( x, y, z ).y ) * search_length;
			bound( 1, 1 ) = ( control_mesh( x    , y + 1, z     ).y - control_mesh( x, y, z ).y ) * search_length;
			bound( 2, 0 ) = ( control_mesh( x    , y    , z - 1 ).z - control_mesh( x, y, z ).z ) * search_length;
			bound( 2, 1 ) = ( control_mesh( x    , y    , z + 1 ).z - control_mesh( x, y, z ).z ) * search_length;
			//bound( 0, 0 ) = -( control_mesh( x - 1, y    , z     ) - control_mesh( x, y, z ) ).length( ) * search_length;
			//bound( 0, 1 ) =  ( control_mesh( x + 1, y    , z     ) - control_mesh( x, y, z ) ).length( ) * search_length;
			//bound( 1, 0 ) = -( control_mesh( x    , y - 1, z     ) - control_mesh( x, y, z ) ).length( ) * search_length;
			//bound( 1, 1 ) =  ( control_mesh( x    , y + 1, z     ) - control_mesh( x, y, z ) ).length( ) * search_length;
			//bound( 2, 0 ) = -( control_mesh( x    , y    , z - 1 ) - control_mesh( x, y, z ) ).length( ) * search_length;
			//bound( 2, 1 ) =  ( control_mesh( x    , y    , z + 1 ) - control_mesh( x, y, z ) ).length( ) * search_length;

			matrix_type dir( p.size( ), 1 ), tmp( p.size( ), 1 );
			double v1, v2;
			size_type i;

			// 他変数関数を１変数関数に変換する
			no_constructor_functor_type f( *this );
			__minimization_utility__::__convert_to_vector_functor__< typename matrix_type::value_type, typename matrix_type::allocator_type, no_constructor_functor_type > functor( p, dir, tmp, f );

			// 勾配方向を計算する
			double len = 0.0, distance = 1.0;
			for( i = 0 ; i < dir.size( ) ; i++ )
			{
				tmp[ i ] = p[ i ] + distance;
				v1 = f( tmp );

				tmp[ i ] = p[ i ] - distance;
				v2 = f( tmp );

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
				return( false );
			}

			double l1, l2, xx = 0.0;
			if( __minimization_utility__::clipping_length( l1, l2, p, dir, bound ) )
			{
				// Brent の2次収束アルゴリズムを用いて dir 方向への最小化を行う
				mist::brent::minimization( l1, l2, xx, functor, 0.1, 100, false );
				p = dir * xx;
			}
			else
			{
				return( false );
			}

//			gradient::minimization( p, bound, no_constructor_functor_type( *this ), 1.0 );
//			gradient::minimization( p, bound, no_constructor_functor_type( *this ), 0.1, 1.0, 1 );

			// 結果を反映
			vector_type &v = control_mesh_tmp( x, y, z );
			v = control_mesh( x, y, z );
			v.x += p[ 0 ];
			v.y += p[ 1 ];
			v.z += p[ 2 ];

			return( true );
		}
	};
}



//! @addtogroup registration_group 画像のレジストレーション
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/registration.h>
//! @endcode
//!
//!  @{



/// @brief FFDを用いた非剛体レジストレーション
namespace non_rigid
{
	/// @brief 非剛体レジストレーションエンジン
	//! 
	//! @param TARGETTYPE  … レジストレーションする目標画像の型
	//! 
	//! @code
	//! mist::non_rigid::registration< 目標画像の型を入力 > r( 目標画像, X軸方向の制御点数, Y軸方向の制御点数, Z軸方向の制御点数, NMIを計算する際のビン幅 );
	//! 
	//! r.apply( ソース画像, レジストレーションの終了判定に用いる許容相対誤差 );
	//! 
	//! r.transform( ソース画像, 非剛体レジストレーションの結果を格納する画像 );
	//! @endcode
	//! 
	template < class TARGETTYPE >
	class registration
	{
	public:
		typedef TARGETTYPE image_type;									///< @brief レジストレーションする目標画像の型
		typedef vector3< double > vector_type;							///< @brief 内部で利用する3次元ベクトルの型
		typedef marray< array3< vector_type > > control_mesh_type;		///< @brief 制御点の配列
		typedef matrix< double > matrix_type;							///< @brief 内部で利用する行列の型
		typedef typename image_type::size_type size_type;				///< @brief 内部で利用する符号なし整数型
		typedef typename image_type::difference_type difference_type;	///< @brief 内部で利用する符号あり整数型

	protected:
		const image_type &target;			///< @brief レジストレーションする目標画像
		control_mesh_type control_mesh;		///< @brief 制御点配列
		size_type BIN;

	public:
		/// @brief 非剛体レジストレーションエンジンの初期化
		//! 
		//! @param[in] tgt  … 目標画像
		//! @param[in] divW … 制御点の初期分割数のX軸方向の個数
		//! @param[in] divH … 制御点の初期分割数のY軸方向の個数
		//! @param[in] divD … 制御点の初期分割数のZ軸方向の個数
		//! @param[in] bin  … NMIを計算する際のビン幅
		//! 
		registration( const image_type &tgt, size_type divW, size_type divH, size_type divD, size_type bin )
			: target( tgt ), control_mesh( divW + 1, divH + 1, divD + 1, 2 ), BIN( bin )
		{
		}


		/// @brief 非剛体レジストレーションの実行
		//! 
		//! @param[in] source              … 目標画像に向けて変形するソース画像
		//! @param[in] tolerance           … レジストレーションの終了判定に用いる許容相対誤差
		//! @param[in] max_loop            … 最適化処理の最大反復回数
		//! @param[in] coarse_to_fine_step … 何段階のメッシュ再分割処理を行うか
		//! @param[in] thread_num          … 使用するスレッド数
		//! 
		template < class SOURCETYPE >
		void apply( const SOURCETYPE &source, double tolerance, size_type max_loop = 3, size_type coarse_to_fine_step = 1, size_type thread_num = 0 )
		{
			apply( source, tolerance, max_loop, coarse_to_fine_step, thread_num, __mist_dmy_callback__( ) );
		}

		/// @brief 非剛体レジストレーションの実行
		//! 
		//! @param[in] source              … 目標画像に向けて変形するソース画像
		//! @param[in] tolerance           … レジストレーションの終了判定に用いる許容相対誤差
		//! @param[in] max_loop            … 最適化処理の最大反復回数
		//! @param[in] coarse_to_fine_step … 何段階のメッシュ再分割処理を行うか
		//! @param[in] thread_num          … 使用するスレッド数
		//! @param[in] callback            … 現在の進行状況を表示するためのコールバック関数
		//! 
		template < class SOURCETYPE, class Functor >
		void apply( const SOURCETYPE &source, double tolerance, size_type max_loop, size_type coarse_to_fine_step, size_type thread_num, Functor callback )
		{
			typedef __non_rigid_registration_utility__::registration_functor< TARGETTYPE, SOURCETYPE, control_mesh_type > non_rigid_registration_functor_type;
			typedef __minimization_utility__::__no_copy_constructor_functor__< non_rigid_registration_functor_type > no_constructor_functor_type;

			size_type divW = control_mesh.width( ) - 1;
			size_type divH = control_mesh.height( ) - 1;
			size_type divD = control_mesh.depth( ) - 1;

			// まず，制御点の初期化
			double stepW = divW == 0 ? 0 : source.width( ) / static_cast< double >( divW );
			double stepH = divH == 0 ? 0 : source.height( ) / static_cast< double >( divH );
			double stepD = divD == 0 ? 0 : source.depth( ) / static_cast< double >( divD );
			difference_type w = control_mesh.width( );
			difference_type h = control_mesh.height( );
			difference_type d = control_mesh.depth( );
			for( difference_type k = -2 ; k <= d + 1 ; k++ )
			{
				for( difference_type j = -2 ; j <= h + 1 ; j++ )
				{
					for( difference_type i = -2 ; i <= w + 1 ; i++ )
					{
						vector_type &v = control_mesh( i, j, k );
						v.x = i * stepW * source.reso1( );
						v.y = j * stepH * source.reso2( );
						v.z = k * stepD * source.reso3( );
					}
				}
			}

			callback( 0.0 );

			if( thread_num == 0 )
			{
				thread_num = static_cast< size_type >( get_cpu_num( ) );
			}

			// ノンリジッドレジストレーションを行うファンクタ
			non_rigid_registration_functor_type **f = new non_rigid_registration_functor_type*[ thread_num ];

			for( size_type i = 0 ; i < thread_num ; i++ )
			{
				f[ i ] = new non_rigid_registration_functor_type( target, source, control_mesh, BIN );
			}

			// 一番初期時点での評価値を計算しておく
			double err = f[ 0 ]->evaluate_error( matrix_type::zero( 3, 1 ) );
			double old_err = err;

			typedef mist::vector3< size_type > control_point_index_type;
			std::vector< control_point_index_type > control_point_list;

			//std::cout << "制御点数1: " << control_point_list.size( ) << std::endl;
			//std::cout << "制御点数2: " << w * h * d << std::endl;


			size_type fine_step_loop = 0;
			while( fine_step_loop++ < coarse_to_fine_step )
			{
				{
					// 1階の探索で，互いに影響しあわない制御点のリストを作成する
					difference_type w = control_mesh.width( );
					difference_type h = control_mesh.height( );
					difference_type d = control_mesh.depth( );
					size_type X[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 };
					size_type Y[] = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2 };
					size_type Z[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

					control_point_list.clear( );

					for( size_type s = 0 ; s < 27 ; s++ )
					{
						for( difference_type k = Z[ s ] ; k < d ; k += 3 )
						{
							for( difference_type j = Y[ s ] ; j < h ; j += 3 )
							{
								for( difference_type i = X[ s ] ; i < w ; i += 3 )
								{
									control_point_list.push_back( mist::vector3< size_type >( i, j, k ) );
								}
							}
						}
					}
				}

				__mist_convert_callback__< Functor > callback_( callback, ( fine_step_loop - 1 ) * 100.0 / coarse_to_fine_step, fine_step_loop * 100.0 / coarse_to_fine_step );

				size_type loop = 0, count = 0;
				double max_iteration_num = control_point_list.size( ) * max_loop / static_cast< double >( thread_num );
				while( loop++ < max_loop )
				{
					for( size_type i = 0 ; i < control_point_list.size( ) ; )
					{
						size_type numthreads;
						for( numthreads = 0 ; numthreads < thread_num && i < control_point_list.size( ) ; numthreads++, i++ )
						{
							control_point_index_type &v = control_point_list[ i ];
							f[ numthreads ]->initialize( v.x, v.y, v.z );
						}

						// スレッドの生成
						for( size_type t = 0 ; t < numthreads ; t++ )
						{
							f[ t ]->create( );
						}

						// スレッドの終了待ち
						for( size_type t = 0 ; t < numthreads ; t++ )
						{
							f[ t ]->wait( INFINITE );
						}

						// リソースの開放
						for( size_type t = 0 ; t < numthreads ; t++ )
						{
							f[ t ]->close( );
						}

						// 探索の結果を，各制御点に反映する
						for( size_type t = 0 ; t < numthreads ; t++ )
						{
							f[ t ]->apply_control_point_to_mesh( control_mesh );
						}

						callback_( 100.0 / max_iteration_num * count++ );
					}

					err = f[ 0 ]->evaluate_error( matrix_type::zero( 3, 1 ) );

					if( 2.0 * std::abs( old_err - err ) < tolerance * ( std::abs( old_err ) + std::abs( err ) ) )
					{
						break;
					}

					old_err = err;
				}


				if( fine_step_loop < coarse_to_fine_step )
				{
					// メッシュの再分割を行う
					control_mesh_type cmesh = control_mesh;
					difference_type w = control_mesh.width( ) * 2 - 1;
					difference_type h = control_mesh.height( ) * 2 - 1;
					difference_type d = control_mesh.depth( ) * 2 - 1;
					w = w <= 0 ? 1 : w;
					h = h <= 0 ? 1 : h;
					d = d <= 0 ? 1 : d;
					control_mesh.resize( w, h, d );

					double stepW = w == 1 ? 0 : source.width( ) / static_cast< double >( w - 1 );
					double stepH = h == 1 ? 0 : source.height( ) / static_cast< double >( h - 1 );
					double stepD = d == 1 ? 0 : source.depth( ) / static_cast< double >( d - 1 );
					for( difference_type k = -2 ; k <= d + 1 ; k++ )
					{
						for( difference_type j = -2 ; j <= h + 1 ; j++ )
						{
							for( difference_type i = -2 ; i <= w + 1 ; i++ )
							{
								vector_type &v = control_mesh( i, j, k );
								v.x = i * stepW * source.reso1( );
								v.y = j * stepH * source.reso2( );
								v.z = k * stepD * source.reso3( );
							}
						}
					}

					for( difference_type k = 0 ; k < d ; k++ )
					{
						bool k_is_odd = ( k % 2 ) == 1;
						difference_type kk = k / 2;

						for( difference_type j = 0 ; j < h ; j++ )
						{
							bool j_is_odd = ( j % 2 ) == 1;
							difference_type jj = j / 2;

							for( difference_type i = 0 ; i < w ; i++ )
							{
								bool i_is_odd = ( i % 2 ) == 1;
								difference_type ii = i / 2;

								if( i_is_odd && j_is_odd && k_is_odd )
								{
									control_mesh( i, j, k ) = ( cmesh( ii    , jj    , kk     ) + cmesh( ii    , jj    , kk + 1 ) +
																cmesh( ii    , jj + 1, kk     ) + cmesh( ii    , jj + 1, kk + 1 ) +
																cmesh( ii + 1, jj    , kk     ) + cmesh( ii + 1, jj    , kk + 1 ) +
																cmesh( ii + 1, jj + 1, kk     ) + cmesh( ii + 1, jj + 1, kk + 1 ) ) / 8.0;
								}
								else if( !i_is_odd && j_is_odd && k_is_odd )
								{
									control_mesh( i, j, k ) = (
																	cmesh( ii - 1, jj    , kk     ) + cmesh( ii - 1, jj    , kk + 1 ) +
																	cmesh( ii - 1, jj + 1, kk     ) + cmesh( ii - 1, jj + 1, kk + 1 ) +
																	cmesh( ii + 1, jj    , kk     ) + cmesh( ii + 1, jj    , kk + 1 ) +
																	cmesh( ii + 1, jj + 1, kk     ) + cmesh( ii + 1, jj + 1, kk + 1 ) + 
																	6.0 * (
																		cmesh( ii    , jj    , kk     ) + cmesh( ii    , jj    , kk + 1 ) +
																		cmesh( ii    , jj + 1, kk     ) + cmesh( ii    , jj + 1, kk + 1 )
																	)
															  ) / 32.0;
								}
								else if( i_is_odd && !j_is_odd && k_is_odd )
								{
									control_mesh( i, j, k ) = (
																	cmesh( ii    , jj - 1, kk     ) + cmesh( ii    , jj - 1, kk + 1 ) +
																	cmesh( ii + 1, jj - 1, kk     ) + cmesh( ii + 1, jj - 1, kk + 1 ) +
																	cmesh( ii    , jj + 1, kk     ) + cmesh( ii    , jj + 1, kk + 1 ) +
																	cmesh( ii + 1, jj + 1, kk     ) + cmesh( ii + 1, jj + 1, kk + 1 ) + 
																	6.0 * (
																		cmesh( ii    , jj    , kk     ) + cmesh( ii    , jj    , kk + 1 ) +
																		cmesh( ii + 1, jj    , kk     ) + cmesh( ii + 1, jj    , kk + 1 )
																	)
															  ) / 32.0;
								}
								else if( i_is_odd && j_is_odd && !k_is_odd )
								{
									control_mesh( i, j, k ) = (
																	cmesh( ii    , jj    , kk - 1 ) + cmesh( ii + 1, jj    , kk - 1 ) +
																	cmesh( ii    , jj + 1, kk - 1 ) + cmesh( ii + 1, jj + 1, kk - 1 ) +
																	cmesh( ii    , jj    , kk + 1 ) + cmesh( ii + 1, jj    , kk + 1 ) +
																	cmesh( ii    , jj + 1, kk + 1 ) + cmesh( ii + 1, jj + 1, kk + 1 ) + 
																	6.0 * (
																		cmesh( ii    , jj    , kk     ) + cmesh( ii + 1, jj    , kk     ) +
																		cmesh( ii    , jj + 1, kk     ) + cmesh( ii + 1, jj + 1, kk     )
																	)
															  ) / 32.0;
								}
								else if( !i_is_odd && !j_is_odd && k_is_odd )
								{
									control_mesh( i, j, k ) = (
																	cmesh( ii - 1, jj - 1, kk     ) + cmesh( ii - 1, jj + 1, kk     ) +
																	cmesh( ii + 1, jj - 1, kk     ) + cmesh( ii + 1, jj + 1, kk     ) +
																	cmesh( ii - 1, jj - 1, kk + 1 ) + cmesh( ii - 1, jj + 1, kk + 1 ) +
																	cmesh( ii + 1, jj - 1, kk + 1 ) + cmesh( ii + 1, jj + 1, kk + 1 ) +
																	6.0 * (
																		cmesh( ii - 1, jj    , kk     ) + cmesh( ii    , jj - 1, kk     ) +
																		cmesh( ii + 1, jj    , kk     ) + cmesh( ii    , jj + 1, kk     ) +
																		cmesh( ii - 1, jj    , kk + 1 ) + cmesh( ii    , jj - 1, kk + 1 ) +
																		cmesh( ii + 1, jj    , kk + 1 ) + cmesh( ii    , jj + 1, kk + 1 )
																	) + 
																	36.0 * (
																		cmesh( ii    , jj    , kk     ) + cmesh( ii    , jj    , kk + 1 )
																	)
																) / 128.0;
								}
								else if( !i_is_odd && j_is_odd && !k_is_odd )
								{
									control_mesh( i, j, k ) = (
																	cmesh( ii - 1, jj    , kk - 1 ) + cmesh( ii - 1, jj    , kk + 1 ) +
																	cmesh( ii + 1, jj    , kk - 1 ) + cmesh( ii + 1, jj    , kk + 1 ) +
																	cmesh( ii - 1, jj + 1, kk - 1 ) + cmesh( ii - 1, jj + 1, kk + 1 ) +
																	cmesh( ii + 1, jj + 1, kk - 1 ) + cmesh( ii + 1, jj + 1, kk + 1 ) +
																	6.0 * (
																		cmesh( ii - 1, jj    , kk     ) + cmesh( ii    , jj    , kk - 1 ) +
																		cmesh( ii + 1, jj    , kk     ) + cmesh( ii    , jj    , kk + 1 ) +
																		cmesh( ii - 1, jj + 1, kk     ) + cmesh( ii    , jj + 1, kk - 1 ) +
																		cmesh( ii + 1, jj + 1, kk     ) + cmesh( ii    , jj + 1, kk + 1 )
																	) + 
																	36.0 * (
																		cmesh( ii    , jj    , kk     ) + cmesh( ii    , jj + 1, kk     )
																	)
																) / 128.0;
								}
								else if( i_is_odd && !j_is_odd && !k_is_odd )
								{
									control_mesh( i, j, k ) = (
																	cmesh( ii    , jj - 1, kk - 1 ) + cmesh( ii    , jj + 1, kk - 1 ) +
																	cmesh( ii    , jj - 1, kk + 1 ) + cmesh( ii    , jj + 1, kk + 1 ) +
																	cmesh( ii + 1, jj - 1, kk - 1 ) + cmesh( ii + 1, jj + 1, kk - 1 ) +
																	cmesh( ii + 1, jj - 1, kk + 1 ) + cmesh( ii + 1, jj + 1, kk + 1 ) +
																	6.0 * (
																		cmesh( ii    , jj    , kk - 1 ) + cmesh( ii    , jj - 1, kk     ) +
																		cmesh( ii    , jj    , kk + 1 ) + cmesh( ii    , jj + 1, kk     ) +
																		cmesh( ii + 1, jj    , kk - 1 ) + cmesh( ii + 1, jj - 1, kk     ) +
																		cmesh( ii + 1, jj    , kk + 1 ) + cmesh( ii + 1, jj + 1, kk     )
																	) + 
																	36.0 * (
																		cmesh( ii    , jj    , kk     ) + cmesh( ii + 1, jj    , kk     )
																	)
																) / 128.0;
								}
								else
								{
									control_mesh( i, j, k ) = (
																	cmesh( ii - 1, jj - 1, kk - 1 ) + cmesh( ii - 1, jj + 1, kk - 1 ) +
																	cmesh( ii + 1, jj - 1, kk - 1 ) + cmesh( ii + 1, jj + 1, kk - 1 ) +
																	cmesh( ii - 1, jj - 1, kk + 1 ) + cmesh( ii - 1, jj + 1, kk + 1 ) +
																	cmesh( ii + 1, jj - 1, kk + 1 ) + cmesh( ii + 1, jj + 1, kk + 1 ) +
																	6.0 * (
																		cmesh( ii - 1, jj - 1, kk     ) + cmesh( ii - 1, jj + 1, kk     ) +
																		cmesh( ii + 1, jj - 1, kk     ) + cmesh( ii + 1, jj + 1, kk     ) +
																		cmesh( ii - 1, jj    , kk - 1 ) + cmesh( ii    , jj - 1, kk - 1 ) +
																		cmesh( ii + 1, jj    , kk - 1 ) + cmesh( ii    , jj + 1, kk - 1 ) +
																		cmesh( ii - 1, jj    , kk + 1 ) + cmesh( ii    , jj - 1, kk + 1 ) +
																		cmesh( ii + 1, jj    , kk + 1 ) + cmesh( ii    , jj + 1, kk + 1 )
																	) + 
																	36.0 * (
																		cmesh( ii    , jj    , kk - 1 ) + cmesh( ii    , jj    , kk + 1 ) +
																		cmesh( ii - 1, jj    , kk     ) + cmesh( ii    , jj - 1, kk     ) +
																		cmesh( ii + 1, jj    , kk     ) + cmesh( ii    , jj + 1, kk     )
																	) +
																	cmesh( ii    , jj    , kk     ) * 216.0
																) / 512.0;
								}
							}
						}
					}

					// メッシュの再分割を行ったので，FFDの係数の再設定を行う
					for( size_type t = 0 ; t < thread_num ; t++ )
					{
						f[ t ]->force_initialize( );
					}
				}
			}

			for( size_type i = 0 ; i < thread_num ; i++ )
			{
				delete f[ i ];
			}
			delete [] f;

			callback( 100.1 );
		}


		/// @brief 非剛体レジストレーション結果を画像に格納する
		//!
		//! 出力画像の解像度に合わせて，非剛体レジストレーション結果の画像を生成する
		//!
		//! @attention 出力画像のサイズと解像度は事前に設定しておく必要あり
		//! 
		//! @param[in]  source … 目標画像に向けて変形するソース画像
		//! @param[out] out    … 出力結果の画像
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array2< SOURCETYPE, SOURCETYPEA > &source, array2< OUTTYPE, OUTTYPEA > &out )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );
		}


		/// @brief 非剛体レジストレーション結果を画像に格納する
		//!
		//! 出力画像の解像度に合わせて，非剛体レジストレーション結果の画像を生成する
		//!
		//! @attention 出力画像のサイズと解像度は事前に設定しておく必要あり
		//! 
		//! @param[in]  source … 目標画像に向けて変形するソース画像
		//! @param[out] out    … 出力結果の画像
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array3< SOURCETYPE, SOURCETYPEA > &source, array3< OUTTYPE, OUTTYPEA > &out )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );
		}


		/// @brief 非剛体レジストレーション結果を画像に格納する
		//!
		//! 出力画像の解像度に合わせて，非剛体レジストレーション結果の画像を生成し，制御点のメッシュを描画する
		//!
		//! @attention 出力画像のサイズと解像度は事前に設定しておく必要あり
		//! 
		//! @param[in]  source … 目標画像に向けて変形するソース画像
		//! @param[out] out    … 出力結果の画像
		//! @param[in]  value  … 制御点メッシュの値
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array2< SOURCETYPE, SOURCETYPEA > &source, array2< OUTTYPE, OUTTYPEA > &out, typename array2< OUTTYPE, OUTTYPEA >::value_type value )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );

			difference_type w = control_mesh.width( );
			difference_type h = control_mesh.height( );
			difference_type d = control_mesh.depth( );
			difference_type i, j;
			double ax = out.reso1( );
			double ay = out.reso2( );

			for( j = 0 ; j < h - 1 ; j++ )
			{
				for( i = 0 ; i < w - 1 ; i++ )
				{
					vector_type &vec0 = control_mesh( i    , j    , 0 );
					vector_type &vec1 = control_mesh( i + 1, j    , 0 );
					vector_type &vec2 = control_mesh( i    , j + 1, 0 );

					size_type x0 = static_cast< size_type >( vec0.x / ax );
					size_type y0 = static_cast< size_type >( vec0.y / ay );
					size_type x1 = static_cast< size_type >( vec1.x / ax );
					size_type y1 = static_cast< size_type >( vec1.y / ay );
					size_type x2 = static_cast< size_type >( vec2.x / ax );
					size_type y2 = static_cast< size_type >( vec2.y / ay );
					draw_line( out, x0, y0, x1, y1, value );
					draw_line( out, x0, y0, x2, y2, value );
				}

				vector_type &vec0 = control_mesh( i    , j    , 0 );
				vector_type &vec2 = control_mesh( i    , j + 1, 0 );
				size_type x0 = static_cast< size_type >( vec0.x / ax );
				size_type y0 = static_cast< size_type >( vec0.y / ay );
				size_type x2 = static_cast< size_type >( vec2.x / ax );
				size_type y2 = static_cast< size_type >( vec2.y / ay );
				draw_line( out, x0, y0, x2, y2, value );
			}
			for( i = 0 ; i < w - 1 ; i++ )
			{
				vector_type &vec0 = control_mesh( i    , j    , 0 );
				vector_type &vec1 = control_mesh( i + 1, j    , 0 );

				size_type x0 = static_cast< size_type >( vec0.x / ax );
				size_type y0 = static_cast< size_type >( vec0.y / ay );
				size_type x1 = static_cast< size_type >( vec1.x / ax );
				size_type y1 = static_cast< size_type >( vec1.y / ay );
				draw_line( out, x0, y0, x1, y1, value );
			}
		}


		/// @brief 非剛体レジストレーション結果を画像に格納する
		//!
		//! 出力画像の解像度に合わせて，非剛体レジストレーション結果の画像を生成し，制御点のメッシュを描画する
		//!
		//! @attention 出力画像のサイズと解像度は事前に設定しておく必要あり
		//! 
		//! @param[in]  source … 目標画像に向けて変形するソース画像
		//! @param[out] out    … 出力結果の画像
		//! @param[in]  value  … 制御点メッシュの値
		//! 
		template < class SOURCETYPE, class SOURCETYPEA, class OUTTYPE, class OUTTYPEA >
		void transform( const array3< SOURCETYPE, SOURCETYPEA > &source, array3< OUTTYPE, OUTTYPEA > &out, typename array2< OUTTYPE, OUTTYPEA >::value_type value )
		{
			__non_rigid_registration_utility__::transformation( out, source, control_mesh );

			difference_type w = control_mesh.width( );
			difference_type h = control_mesh.height( );
			difference_type d = control_mesh.depth( );
			double ax = out.reso1( );
			double ay = out.reso2( );
			double az = out.reso3( );

			for( difference_type k = 0 ; k < d ; k++ )
			{
				for( difference_type j = 0 ; j < h ; j++ )
				{
					for( difference_type i = 0 ; i < w ; i++ )
					{
						const vector_type &vec0 = control_mesh( i, j, k );

						size_type x0 = static_cast< size_type >( vec0.x / ax );
						size_type y0 = static_cast< size_type >( vec0.y / ay );
						size_type z0 = static_cast< size_type >( vec0.z / az );

						if( i < w - 1 )
						{
							const vector_type &vec1 = control_mesh( i + 1, j, k );
							size_type x1 = static_cast< size_type >( vec1.x / ax );
							size_type y1 = static_cast< size_type >( vec1.y / ay );
							size_type z1 = static_cast< size_type >( vec1.z / az );
							draw_line( out, x0, y0, z0, x1, y1, z1, value );
						}

						if( j < h - 1 )
						{
							const vector_type &vec2 = control_mesh( i, j + 1, k );
							size_type x2 = static_cast< size_type >( vec2.x / ax );
							size_type y2 = static_cast< size_type >( vec2.y / ay );
							size_type z2 = static_cast< size_type >( vec2.z / az );
							draw_line( out, x0, y0, z0, x2, y2, z2, value );
						}

						if( k < d - 1 )
						{
							const vector_type &vec3 = control_mesh( i, j, k + 1 );
							size_type x3 = static_cast< size_type >( vec3.x / ax );
							size_type y3 = static_cast< size_type >( vec3.y / ay );
							size_type z3 = static_cast< size_type >( vec3.z / az );
							draw_line( out, x0, y0, z0, x3, y3, z3, value );
						}
					}
				}
			}
		}
	};

} // 名前空間 non_rigid の終わり


/// @}
//  画像のレジストレーショングループの終わり



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_REGISTRATION__
