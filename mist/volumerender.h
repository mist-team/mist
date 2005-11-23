// 
// Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/volumerender.h
//!
//! @brief ３次元画像のボリュームレンダリングエンジン
//! 
//! - 参考文献
//!   - 林雄一郎，樋口義剛，森健策，長谷川純一，末永康仁，鳥脇純一郎, "ボリュームレンダリング画像における3次元関心領域の指定法と開発とその仮想化内視鏡システムへの応用," 電子通信学会論文誌D, J87-D-II, No.1, pp.361-367，2004
//!   - K. Mori, Y. Suenaga and J. Toriwaki, ``Fast volume rendering based on software optimization using multimedia instructions on PC platform,'' Proceedings of Computer Assisted Radiology and Surgery 2002, pp.467-472, 2002
//!   - K. Mori, Y. Suenaga and J. Toriwaki, ``Fast software-based volume rendering using multimedia instructions on PC platforms and its application to virtual endoscopy,'' Proceedings of SPIE, Vol. 5031, pp.111-122, 2003
//!
#ifndef __INCLUDE_MIST_VOLUMERENDER__
#define __INCLUDE_MIST_VOLUMERENDER__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "thread.h"
#endif

#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#include "filter/distance.h"
#endif


#include <vector>


// mist名前空間の始まり
_MIST_BEGIN




//! @addtogroup visualization_group 可視化
//!
//!  @{


//! @addtogroup volumerendering_group ボリュームレンダリング
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/volumerender.h>
//! @endcode
//!
//! @{


namespace volumerender
{
	template < class T >
	struct attribute
	{
		typedef T value_type;

		value_type pixel;
		double alpha;
		bool   has_alpha;

		attribute( ) : pixel( ), alpha( 0 ), has_alpha( false ){ }

		//template < class TT >
		//attribute( const attribute &a ) : pixel( a.pixel ), alpha( a.alpha ), has_alpha( a.has_alpha ){ }

		attribute( const attribute &a ) : pixel( a.pixel ), alpha( a.alpha ), has_alpha( a.has_alpha ){ }

		attribute( const value_type &pix, double a ) : pixel( pix ), alpha( a ), has_alpha( a > 0.0 ){ }
	
	};

	template < class T >
	struct attribute_element
	{
		typedef T value_type;
		typedef ptrdiff_t difference_type;

		value_type pixel;
		difference_type value_lower;
		difference_type value_upper;
		double alpha_lower;
		double alpha_upper;

		attribute_element( ) : pixel( ), value_lower( 0 ), value_upper( 0 ), alpha_lower( 0 ), alpha_upper( 0 ){ }
		attribute_element( const value_type &pixel, difference_type vs, difference_type ve, double as, double ae )
			: pixel( pixel ), value_lower( vs ), value_upper( ve ), alpha_lower( as ), alpha_upper( ae ){ }

		attribute_element( const attribute_element &a ) : pixel( a.pixel ), value_lower( a.value_lower ), value_upper( a.value_upper ), alpha_lower( a.alpha_lower ), alpha_upper( a.alpha_upper ){ }
	
	};

	template < class T >
	class attribute_table : protected std::vector< attribute< T > >
	{
	public:
		typedef std::vector< attribute< T > > base;
		typedef attribute< T > attribute_type;
		typedef attribute_element< T > attribute_element_type;
		typedef typename base::allocator_type allocator_type;	///< @brief MISTコンテナが利用するアロケータ型
		typedef typename base::reference reference;				///< @brief MISTのコンテナ内に格納するデータ型の参照．mist::array< data > の場合，data & となる
		typedef typename base::const_reference const_reference;	///< @brief MISTのコンテナ内に格納するデータ型の const 参照．mist::array< data > の場合，const data & となる
		typedef typename base::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
		typedef typename base::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef typename base::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
		typedef typename base::pointer pointer;					///< @brief MISTのコンテナ内に格納するデータ型のポインター型．mist::array< data > の場合，data * となる
		typedef typename base::const_pointer const_pointer;		///< @brief MISTのコンテナ内に格納するデータ型の const ポインター型．mist::array< data > の場合，const data * となる

		typedef T pixel_type;
		typedef std::vector< attribute_element_type > attribute_element_list;

	protected:
		difference_type sindex_;
		difference_type eindex_;
		difference_type zero_index_;

		attribute_element_list attribute_element_table;

	public:
		void append( const pixel_type &pixel, difference_type vs, difference_type ve, double as, double ae )
		{
			if( base::empty( ) )
			{
				return;
			}
			else if( vs > ve )
			{
				difference_type tmp = vs;
				vs = ve;
				ve = tmp;
			}
			vs = vs < sindex_ ? sindex_: vs;
			vs = vs > eindex_ ? eindex_: vs;
			ve = ve < sindex_ ? sindex_: ve;
			ve = ve > eindex_ ? eindex_: ve;

			if( as > ae )
			{
				double tmp = as;
				as = ae;
				ae = tmp;
			}

			attribute_element_table.push_back( attribute_element_type( pixel, vs, ve, as, ae ) );

			double step = ( ae - as ) / static_cast< double >( ve - vs + 1 );
			for( difference_type i = vs ; i <= ve ; i++ )
			{
				value_type &p = at( i );
				p.pixel = pixel;
				p.alpha = as + static_cast< double >( i - vs ) * step;
				p.has_alpha = p.alpha > 0.0;
			}
		}

	public:
		const difference_type minimum( ) const
		{
			return( sindex_ );
		}

		const difference_type maximum( ) const
		{
			return( eindex_ );
		}

		const bool empty( ) const
		{
			return( eindex_ < sindex_ );
		}

		value_type &operator []( difference_type index )
		{
			return( base::operator []( zero_index_ + index ) );
		}

		value_type &at( difference_type index )
		{
			return( base::at( zero_index_ + index ) );
		}

		const value_type &operator []( difference_type index ) const
		{
			return( base::operator []( zero_index_ + index ) );
		}

		const value_type &at( difference_type index ) const
		{
			return( base::at( zero_index_ + index ) );
		}

		bool has_alpha( difference_type index ) const
		{
			return( operator []( index ).has_alpha );
		}

		const attribute_table &operator =( const attribute_table &a )
		{
			if( &a != this )
			{
				base::operator =( a );
				sindex_ = a.sindex_;
				eindex_ = a.eindex_;
				zero_index_ = a.zero_index_;
				attribute_element_table = a.attribute_element_table;
			}
			return( *this );
		}

		const attribute_element_list &attribute_elements( ) const { return( attribute_element_table ); }


	public:
		void create( difference_type si, difference_type ei )
		{
			sindex_ = si;
			eindex_ = ei;

			base::resize( ei - si + 1 );
			zero_index_ = - si;

			fill( );
			attribute_element_table.clear( );
		}

		void clear( )
		{
			base::clear( );
			sindex_ = 0;
			eindex_ = -1;
			zero_index_ = 0;
			attribute_element_table.clear( );
		}

		void fill( )
		{
			for( size_type i = 0 ; i < base::size( ) ; i++ )
			{
				base::operator []( i ) = attribute_type( );
			}
			attribute_element_table.clear( );
		}

		attribute_table( ) : sindex_( 0 ), eindex_( -1 ), zero_index_( 0 ){ }

		attribute_table( difference_type si, difference_type ei ) : base( ei - si + 1 ), sindex_( si ), eindex_( ei ), zero_index_( 0 )
		{
			zero_index_ = - si;
		}

		attribute_table( const attribute_table &a ) : base( a ), sindex_( a.sindex_ ), eindex_( a.eindex_ ), zero_index_( a.zero_index_ ), attribute_element_table( a.attribute_element_table )
		{
		}
	};

	struct boundingbox
	{
	public:
		double a;
		double b;
		double c;
		double d;

		void clear( )
		{
			a = 0;
			b = 0;
			c = 0;
			d = 0;
		}

		boundingbox( ) : a( 0 ), b( 0 ), c( 0 ), d( 0 ){ }
		boundingbox( double aa, double bb, double cc, double dd ) : a( aa ), b( bb ), c( cc ), d( dd ){ }
	};


	template < class T >
	inline bool check_intersection( vector3< T > &p1, vector3< T > &p2, const boundingbox &box )
	{
		typedef vector3< T > vector_type;
		typedef typename vector3< T >::value_type value_type;

		vector_type n( box.a, box.b, box.c );

		value_type d1 = p1.inner( n );
		value_type d2 = p2.inner( n );
		bool c1 = box.d + d1 >= 0;
		bool c2 = box.d + d2 >= 0;

		if( c1 && c2 )
		{
			return( true );
		}
		else if( !c1 && !c2 )
		{
			return( false );
		}
		vector_type v1 = p1;
		vector_type v2 = p2;
		if( !c1 )
		{
			p1 = v2 + ( v1 - v2 ) * ( ( box.d + d2 ) / ( d2 - d1 ) );
		}
		if( !c2 )
		{
			p2 = v1 + ( v2 - v1 ) * ( ( box.d + d1 ) / ( d1 - d2 ) );
		}
		return( true );
	}

	struct parameter
	{
		typedef vector3< double > vector_type;

		vector_type	pos;
		vector_type	dir;
		vector_type	up;
		vector_type	offset;
		bool	perspective_view;
		bool	value_interpolation;
		double	fovy;
		double	ambient_ratio;
		double	diffuse_ratio;
		double	light_attenuation;
		double	sampling_step;
		double	termination;
		double	specular;

		bool left_to_right;
		bool top_to_bottom;
		bool front_to_back;

		boundingbox box[ 6 ];

		parameter( ) : perspective_view( true ), value_interpolation( true ), fovy( 80.0 ), ambient_ratio( 0.4 ), diffuse_ratio( 0.6 ), light_attenuation( 0.0 ),
						sampling_step( 1.0 ), termination( 0.01 ), specular( 1.0 ), left_to_right( true ), top_to_bottom( true ), front_to_back( true )
		{
		}
	};

	inline std::ostream &operator <<( std::ostream &out, const parameter &p )
	{
		out << "Pos = ( " << p.pos << " )" << std::endl;
		out << "Dir = ( " << p.dir << " )" << std::endl;
		out << "Up  = ( " << p.up  << " )" << std::endl;

		out << "Center = ( " << p.offset  << " )" << std::endl;

		out << "Perspective = " << p.perspective_view << std::endl;
		out << "ValueInterpolation = " << p.value_interpolation << std::endl;
		out << "Fovy = " << p.fovy << std::endl;
		out << "Ambient = " << p.ambient_ratio << std::endl;
		out << "Diffuse = " << p.diffuse_ratio << std::endl;
		out << "Specular = " << p.specular << std::endl;
		out << "LightAtten = " << p.light_attenuation << std::endl;
		out << "SamplingStep = " << p.sampling_step << std::endl;
		out << "Termination = " << p.termination << std::endl;

		return( out );
	}

	struct no_depth_map
	{
		typedef ptrdiff_t difference_type;
		double operator()( difference_type i, difference_type j, difference_type k ) const
		{
			return( 2.0 );
		}
	};

	template < class DepthMap >
	struct depth_map
	{
		typedef DepthMap depth_map_type;
		typedef typename depth_map_type::difference_type difference_type;

		const depth_map_type &depth_map_;

		depth_map( const depth_map_type &dmap ) : depth_map_( dmap )
		{
		}

		double operator()( difference_type i, difference_type j, difference_type k ) const
		{
			double l = depth_map_( i >> 1, j >> 1, k >> 1 );
			return( l < 1.0 ? 2.0 : l * 2.0 );
		}
	};


	// 倍精度浮動小数を整数に丸める
	// 正の数の場合は，通常の int キャストと同じ動作をする
	// 負の数のときは，値の小さいほうに丸められる
	inline long to_integer( double val )
	{
		return( static_cast< long >( val ) );
		//val += 68719476736.0 * 1.5;
		//return( ( ( long * )&val )[ 0 ] >> 16 );
	}

	template < class T >
	inline void normalize_vector( vector3< T > &v )
	{
		double len = std::sqrt( v.x * v.x + v.y * v.y + v.z * v.z ) + 1.0e-10;
		v.x /= len;
		v.y /= len;
		v.z /= len;
	}

	inline double _3( double v )
	{
		if( v < 0.0 )
		{
			return( - std::pow( -v, 1.0 / 3.0 ) );
		}
		else if( v > 0.0 )
		{
			return( std::pow( v, 1.0 / 3.0 ) );
		}
		else
		{
			return( 0.0 );
		}
	}

	inline double __maximum__( double v1, double v2 )
	{
		return( v1 > v2 ? v1 : v2 );
	}

	inline double __maximum__( double v1, double v2, double v3 )
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

	inline double __minimum__( double v1, double v2 )
	{
		return( v1 < v2 ? v1 : v2 );
	}

	inline double __minimum__( double v1, double v2, double v3 )
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

	inline size_t solve_equation( double p, double q, double &v1, double &v2, double &v3 )
	{
		const double pai = 3.1415926535897932384626433832795;
		double p_3 = p / 3.0;
		double q_2 = q / 2.0;
		double D = - ( q_2 * q_2 + p_3 * p_3 * p_3 );

		if( D > 0.0 )
		{
			double r  = std::sqrt( - 4.0 * p / 3.0 );
			double t1 = 1.0 / ( 3.0 * std::cos( 3.0 * q / ( p * r ) ) );
			double t2 = 2.0 * pai / 3.0 + t1;
			double t3 = 2.0 * pai / 3.0 - t1;
			v1 = r * std::cos( t1 );
			v2 = r * std::cos( t2 );
			v3 = r * std::cos( t3 );
			return( 3 );
		}
		else if( D == 0.0 )
		{
			if( p == 0.0 )
			{
				v1 = v2 = v3 = 0.0;
				return( 1 );
			}
			else
			{
				double r  = std::sqrt( - 4.0 * p / 3.0 );
				v1 = r;
				v2 = r * std::cos( 2.0 * pai / 3.0 );
				v3 = 0.0;
				return( 2 );
			}
		}
		//else if( D < 0.0 && p < 0.0 )
		//{
		//	double _D = std::sqrt( -D );
		//	v1 = _3( - q / 2.0 + _D ) + _3( - q / 2.0 - _D );
		//	v2 = v3 = 0.0;
		//	return( 1 );
		//}
		else
		{
			double _D = std::sqrt( -D );
			v1 = _3( - q / 2.0 + _D ) + _3( - q / 2.0 - _D );
			v2 = v3 = 0.0;
			return( 1 );
		}
	}

	inline size_t solve_equation( double a, double b, double c, double d, double &v1, double &v2, double &v3 )
	{
		if( a == 0.0 )
		{
			return( 0 );
		}
		else
		{
			return( solve_equation( ( - b * b / ( 3.0 * a ) + c ) / a, ( 2.0 * b * b * b / ( 27.0 * a * a ) - b * c / ( 3.0 * a ) + d ) / a, v1, v2, v3 ) );
		}
	}
}


/// @}
// ボリュームレンダリンググループの終わり

/// @}
//  可視化グループの終わり

namespace rendering_helper
{
	template < class Array, class T >
	class value_interpolation
	{
	protected:
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename Array::size_type size_type;
		typedef typename Array::const_pointer const_pointer;
		typedef typename Array::difference_type difference_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;

	public:
		const Array &in;
		const volumerender::parameter &p;
		const volumerender::attribute_table< T > &table;
		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;

		value_interpolation( const Array &image, const volumerender::parameter &param, const volumerender::attribute_table< T > &tbl ) : in( image ), p( param ), table( tbl )
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		bool check( difference_type i, difference_type j, difference_type k ) const
		{
			const_pointer p = &in( i, j, k );

			// この位置における物体が不透明の場合は次のステップへ移行する
			return( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) ||
					table.has_alpha( p[ d2 ] ) || table.has_alpha( p[ d3 ] ) ||
					table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
					table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) );
		}

		bool render( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz, attribute_type &oc ) const
		{
			const_pointer p = &in( i, j, k );

			// CT値に対応する色と不透明度を取得
			double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
			ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

			oc = table[ volumerender::to_integer( ct ) ];

			return( oc.has_alpha );
		}

		vector_type normal( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz ) const
		{
			const_pointer p0 = &in( i, j, k );
			const_pointer p1 = p0 + d1;
			const_pointer p2 = p0 + d2;
			const_pointer p3 = p0 + d3;
			const_pointer p4 = p0 + d4;
			const_pointer p5 = p0 + d5;
			const_pointer p6 = p0 + d6;
			const_pointer p7 = p0 + d7;

			double n0x = p3[  0  ] - p0[ -_1 ];
			double n0y = p1[  0  ] - p0[ -_2 ];
			double n0z = p4[  0  ] - p0[ -_3 ];
			double n1x = p2[  0  ] - p1[ -_1 ];
			double n1y = p1[  _2 ] - p0[  0  ];
			double n1z = p5[  0  ] - p1[ -_3 ];
			double n2x = p2[  _1 ] - p1[  0  ];
			double n2y = p2[  _2 ] - p3[  0  ];
			double n2z = p6[  0  ] - p2[ -_3 ];
			double n3x = p3[  _1 ] - p0[  0  ];
			double n3y = p2[  0  ] - p3[ -_2 ];
			double n3z = p7[  0  ] - p3[ -_3 ];
			double n4x = p7[  0  ] - p4[ -_1 ];
			double n4y = p5[  0  ] - p4[ -_2 ];
			double n4z = p4[  _3 ] - p0[  0  ];
			double n5x = p6[  0  ] - p5[ -_1 ];
			double n5y = p5[  _2 ] - p4[  0  ];
			double n5z = p5[  _3 ] - p1[  0  ];
			double n6x = p6[  _1 ] - p5[  0  ];
			double n6y = p6[  _2 ] - p7[  0  ];
			double n6z = p6[  _3 ] - p2[  0  ];
			double n7x = p7[  _1 ] - p4[  0  ];
			double n7y = p6[  0  ] - p7[ -_2 ];
			double n7z = p7[  _3 ] - p3[  0  ];

			double nx  = ( n0x + ( n3x - n0x ) * xx ) + ( n1x - n0x + ( n0x - n1x + n2x - n3x ) * xx ) * yy;
			nx += ( ( n4x + ( n7x - n4x ) * xx ) + ( n5x - n4x + ( n4x - n5x + n6x - n7x ) * xx ) * yy - nx ) * zz;
			double ny  = ( n0y + ( n3y - n0y ) * xx ) + ( n1y - n0y + ( n0y - n1y + n2y - n3y ) * xx ) * yy;
			ny += ( ( n4y + ( n7y - n4y ) * xx ) + ( n5y - n4y + ( n4y - n5y + n6y - n7y ) * xx ) * yy - ny ) * zz;
			double nz  = ( n0z + ( n3z - n0z ) * xx ) + ( n1z - n0z + ( n0z - n1z + n2z - n3z ) * xx ) * yy;
			nz += ( ( n4z + ( n7z - n4z ) * xx ) + ( n5z - n4z + ( n4z - n5z + n6z - n7z ) * xx ) * yy - nz ) * zz;

			nx /= in.reso1( );
			ny /= in.reso2( );
			nz /= in.reso3( );
			double len = std::sqrt( nx * nx + ny * ny + nz * nz ) + type_limits< double >::minimum( );
			nx /= len;
			ny /= len;
			nz /= len;

			return( vector_type( nx, ny, nz ) );
		}
	};


	template < class Array, class T >
	class color_interpolation
	{
	protected:
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename Array::size_type size_type;
		typedef typename Array::const_pointer const_pointer;
		typedef typename Array::difference_type difference_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;

	public:
		const Array &in;
		const volumerender::parameter &p;
		const volumerender::attribute_table< T > &table;
		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;

		color_interpolation( const Array &image, const volumerender::parameter &param, const volumerender::attribute_table< T > &tbl ) : in( image ), p( param ), table( tbl )
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		bool check( difference_type i, difference_type j, difference_type k ) const
		{
			const_pointer p = &in( i, j, k );

			// この位置における物体が不透明の場合は次のステップへ移行する
			return( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) ||
					table.has_alpha( p[ d2 ] ) || table.has_alpha( p[ d3 ] ) ||
					table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
					table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) );
		}

		bool render( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz, attribute_type &oc ) const
		{
			const_pointer p = &in( i, j, k );

			// 近傍の8点に対応する色と不透明度を取得
			attribute_type oc0 = table[ volumerender::to_integer( p[ d0 ] ) ];
			attribute_type oc1 = table[ volumerender::to_integer( p[ d1 ] ) ];
			attribute_type oc2 = table[ volumerender::to_integer( p[ d2 ] ) ];
			attribute_type oc3 = table[ volumerender::to_integer( p[ d3 ] ) ];
			attribute_type oc4 = table[ volumerender::to_integer( p[ d4 ] ) ];
			attribute_type oc5 = table[ volumerender::to_integer( p[ d5 ] ) ];
			attribute_type oc6 = table[ volumerender::to_integer( p[ d6 ] ) ];
			attribute_type oc7 = table[ volumerender::to_integer( p[ d7 ] ) ];

			int number_of_visible_voxels = oc0.has_alpha;
			number_of_visible_voxels += oc1.has_alpha;
			number_of_visible_voxels += oc2.has_alpha;
			number_of_visible_voxels += oc3.has_alpha;
			number_of_visible_voxels += oc4.has_alpha;
			number_of_visible_voxels += oc5.has_alpha;
			number_of_visible_voxels += oc6.has_alpha;
			number_of_visible_voxels += oc7.has_alpha;

			if( number_of_visible_voxels == 0 )
			{
				return( false );
			}
			else
			{
				// まず平均的な色を決定する
				oc.pixel = ( oc0.pixel * oc0.has_alpha + oc1.pixel * oc1.has_alpha
							+ oc2.pixel * oc2.has_alpha + oc3.pixel * oc3.has_alpha
							+ oc4.pixel * oc4.has_alpha + oc5.pixel * oc5.has_alpha
							+ oc6.pixel * oc6.has_alpha + oc7.pixel * oc7.has_alpha ) / static_cast< double >( number_of_visible_voxels );

				// 透明物体があった場合は，周りの不透明物体の色で置き換えることでもあれを回避
				if( !oc0.has_alpha ){ oc0.pixel = oc.pixel; }
				if( !oc1.has_alpha ){ oc1.pixel = oc.pixel; }
				if( !oc2.has_alpha ){ oc2.pixel = oc.pixel; }
				if( !oc3.has_alpha ){ oc3.pixel = oc.pixel; }
				if( !oc4.has_alpha ){ oc4.pixel = oc.pixel; }
				if( !oc5.has_alpha ){ oc5.pixel = oc.pixel; }
				if( !oc6.has_alpha ){ oc6.pixel = oc.pixel; }
				if( !oc7.has_alpha ){ oc7.pixel = oc.pixel; }

				oc.pixel = ( oc0.pixel + ( oc3.pixel - oc0.pixel ) * xx ) + ( oc1.pixel - oc0.pixel + ( oc0.pixel - oc1.pixel + oc2.pixel - oc3.pixel ) * xx ) * yy;
				oc.pixel = oc.pixel + ( ( oc4.pixel + ( oc7.pixel - oc4.pixel ) * xx ) + ( oc5.pixel - oc4.pixel + ( oc4.pixel - oc5.pixel + oc6.pixel - oc7.pixel ) * xx ) * yy - oc.pixel ) * zz;
				oc.alpha = ( oc0.alpha + ( oc3.alpha - oc0.alpha ) * xx ) + ( oc1.alpha - oc0.alpha + ( oc0.alpha - oc1.alpha + oc2.alpha - oc3.alpha ) * xx ) * yy;
				oc.alpha = oc.alpha + ( ( oc4.alpha + ( oc7.alpha - oc4.alpha ) * xx ) + ( oc5.alpha - oc4.alpha + ( oc4.alpha - oc5.alpha + oc6.alpha - oc7.alpha ) * xx ) * yy - oc.alpha ) * zz;

				return( true );
			}
		}

		vector_type normal( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz ) const
		{
			const_pointer p0 = &in( i, j, k );
			const_pointer p1 = p0 + d1;
			const_pointer p2 = p0 + d2;
			const_pointer p3 = p0 + d3;
			const_pointer p4 = p0 + d4;
			const_pointer p5 = p0 + d5;
			const_pointer p6 = p0 + d6;
			const_pointer p7 = p0 + d7;

			double n0x = p3[  0  ] - p0[ -_1 ];
			double n0y = p1[  0  ] - p0[ -_2 ];
			double n0z = p4[  0  ] - p0[ -_3 ];
			double n1x = p2[  0  ] - p1[ -_1 ];
			double n1y = p1[  _2 ] - p0[  0  ];
			double n1z = p5[  0  ] - p1[ -_3 ];
			double n2x = p2[  _1 ] - p1[  0  ];
			double n2y = p2[  _2 ] - p3[  0  ];
			double n2z = p6[  0  ] - p2[ -_3 ];
			double n3x = p3[  _1 ] - p0[  0  ];
			double n3y = p2[  0  ] - p3[ -_2 ];
			double n3z = p7[  0  ] - p3[ -_3 ];
			double n4x = p7[  0  ] - p4[ -_1 ];
			double n4y = p5[  0  ] - p4[ -_2 ];
			double n4z = p4[  _3 ] - p0[  0  ];
			double n5x = p6[  0  ] - p5[ -_1 ];
			double n5y = p5[  _2 ] - p4[  0  ];
			double n5z = p5[  _3 ] - p1[  0  ];
			double n6x = p6[  _1 ] - p5[  0  ];
			double n6y = p6[  _2 ] - p7[  0  ];
			double n6z = p6[  _3 ] - p2[  0  ];
			double n7x = p7[  _1 ] - p4[  0  ];
			double n7y = p6[  0  ] - p7[ -_2 ];
			double n7z = p7[  _3 ] - p3[  0  ];

			double nx  = ( n0x + ( n3x - n0x ) * xx ) + ( n1x - n0x + ( n0x - n1x + n2x - n3x ) * xx ) * yy;
			nx += ( ( n4x + ( n7x - n4x ) * xx ) + ( n5x - n4x + ( n4x - n5x + n6x - n7x ) * xx ) * yy - nx ) * zz;
			double ny  = ( n0y + ( n3y - n0y ) * xx ) + ( n1y - n0y + ( n0y - n1y + n2y - n3y ) * xx ) * yy;
			ny += ( ( n4y + ( n7y - n4y ) * xx ) + ( n5y - n4y + ( n4y - n5y + n6y - n7y ) * xx ) * yy - ny ) * zz;
			double nz  = ( n0z + ( n3z - n0z ) * xx ) + ( n1z - n0z + ( n0z - n1z + n2z - n3z ) * xx ) * yy;
			nz += ( ( n4z + ( n7z - n4z ) * xx ) + ( n5z - n4z + ( n4z - n5z + n6z - n7z ) * xx ) * yy - nz ) * zz;

			nx /= in.reso1( );
			ny /= in.reso2( );
			nz /= in.reso3( );
			double len = std::sqrt( nx * nx + ny * ny + nz * nz ) + type_limits< double >::minimum( );
			nx /= len;
			ny /= len;
			nz /= len;

			return( vector_type( nx, ny, nz ) );
		}
	};


	template < class Array1, class Array2, class T >
	class value_interpolation_with_mark
	{
	protected:
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::const_pointer const_mk_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;

	public:
		const Array1 &in;
		const Array2 &mk;
		const volumerender::parameter &p;
		const volumerender::attribute_table< T > &table;
		const volumerender::attribute_table< T > &mktable;
		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;

		value_interpolation_with_mark( const Array1 &image, const Array2 &mark, const volumerender::parameter &param, const volumerender::attribute_table< T > &tbl, const volumerender::attribute_table< T > &mktbl )
			: in( image ), mk( mark ), p( param ), table( tbl ), mktable( mktbl )
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		bool check( difference_type i, difference_type j, difference_type k ) const
		{
			const_pointer p = &in( i, j, k );

			// この位置における物体が不透明の場合は次のステップへ移行する
			return( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) ||
					table.has_alpha( p[ d2 ] ) || table.has_alpha( p[ d3 ] ) ||
					table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
					table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) );
		}

		bool render( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz, attribute_type &oc ) const
		{
			const_pointer p = &in( i, j, k );

			// CT値に対応する色と不透明度を取得
			double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
			ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

			oc = table[ volumerender::to_integer( ct ) ];

			if( !oc.has_alpha )
			{
				return( false );
			}
			else
			{
				// マーキング結果を重ねる
				const_mk_pointer pm = &mk( i, j, k );

				// 近傍の8点に対応する色と不透明度を取得
				attribute_type mc0 = mktable[ volumerender::to_integer( pm[ d0 ] ) ];
				attribute_type mc1 = mktable[ volumerender::to_integer( pm[ d1 ] ) ];
				attribute_type mc2 = mktable[ volumerender::to_integer( pm[ d2 ] ) ];
				attribute_type mc3 = mktable[ volumerender::to_integer( pm[ d3 ] ) ];
				attribute_type mc4 = mktable[ volumerender::to_integer( pm[ d4 ] ) ];
				attribute_type mc5 = mktable[ volumerender::to_integer( pm[ d5 ] ) ];
				attribute_type mc6 = mktable[ volumerender::to_integer( pm[ d6 ] ) ];
				attribute_type mc7 = mktable[ volumerender::to_integer( pm[ d7 ] ) ];

				int number_of_visible_voxels = mc0.has_alpha;
				number_of_visible_voxels += mc1.has_alpha;
				number_of_visible_voxels += mc2.has_alpha;
				number_of_visible_voxels += mc3.has_alpha;
				number_of_visible_voxels += mc4.has_alpha;
				number_of_visible_voxels += mc5.has_alpha;
				number_of_visible_voxels += mc6.has_alpha;
				number_of_visible_voxels += mc7.has_alpha;

				if( number_of_visible_voxels > 0 )
				{
					attribute_type mc;
					// まず平均的な色を決定する
					mc.pixel = ( mc0.pixel * mc0.has_alpha + mc1.pixel * mc1.has_alpha
						+ mc2.pixel * mc2.has_alpha + mc3.pixel * mc3.has_alpha
						+ mc4.pixel * mc4.has_alpha + mc5.pixel * mc5.has_alpha
						+ mc6.pixel * mc6.has_alpha + mc7.pixel * mc7.has_alpha ) / static_cast< double >( number_of_visible_voxels );

					// 透明物体があった場合は，周りの不透明物体の色で置き換えることでもあれを回避
					if( !mc0.has_alpha ){ mc0.pixel = mc.pixel; }
					if( !mc1.has_alpha ){ mc1.pixel = mc.pixel; }
					if( !mc2.has_alpha ){ mc2.pixel = mc.pixel; }
					if( !mc3.has_alpha ){ mc3.pixel = mc.pixel; }
					if( !mc4.has_alpha ){ mc4.pixel = mc.pixel; }
					if( !mc5.has_alpha ){ mc5.pixel = mc.pixel; }
					if( !mc6.has_alpha ){ mc6.pixel = mc.pixel; }
					if( !mc7.has_alpha ){ mc7.pixel = mc.pixel; }

					mc.pixel = ( mc0.pixel + ( mc3.pixel - mc0.pixel ) * xx ) + ( mc1.pixel - mc0.pixel + ( mc0.pixel - mc1.pixel + mc2.pixel - mc3.pixel ) * xx ) * yy;
					mc.pixel = mc.pixel + ( ( mc4.pixel + ( mc7.pixel - mc4.pixel ) * xx ) + ( mc5.pixel - mc4.pixel + ( mc4.pixel - mc5.pixel + mc6.pixel - mc7.pixel ) * xx ) * yy - mc.pixel ) * zz;
					mc.alpha = ( mc0.alpha + ( mc3.alpha - mc0.alpha ) * xx ) + ( mc1.alpha - mc0.alpha + ( mc0.alpha - mc1.alpha + mc2.alpha - mc3.alpha ) * xx ) * yy;
					mc.alpha = mc.alpha + ( ( mc4.alpha + ( mc7.alpha - mc4.alpha ) * xx ) + ( mc5.alpha - mc4.alpha + ( mc4.alpha - mc5.alpha + mc6.alpha - mc7.alpha ) * xx ) * yy - mc.alpha ) * zz;

					// マーキングの色を合成する
					oc.pixel = oc.pixel * ( 1.0 - mc.alpha ) + mc.pixel * mc.alpha;
				}

				return( true );
			}
		}

		vector_type normal( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz ) const
		{
			const_pointer p0 = &in( i, j, k );
			const_pointer p1 = p0 + d1;
			const_pointer p2 = p0 + d2;
			const_pointer p3 = p0 + d3;
			const_pointer p4 = p0 + d4;
			const_pointer p5 = p0 + d5;
			const_pointer p6 = p0 + d6;
			const_pointer p7 = p0 + d7;

			double n0x = p3[  0  ] - p0[ -_1 ];
			double n0y = p1[  0  ] - p0[ -_2 ];
			double n0z = p4[  0  ] - p0[ -_3 ];
			double n1x = p2[  0  ] - p1[ -_1 ];
			double n1y = p1[  _2 ] - p0[  0  ];
			double n1z = p5[  0  ] - p1[ -_3 ];
			double n2x = p2[  _1 ] - p1[  0  ];
			double n2y = p2[  _2 ] - p3[  0  ];
			double n2z = p6[  0  ] - p2[ -_3 ];
			double n3x = p3[  _1 ] - p0[  0  ];
			double n3y = p2[  0  ] - p3[ -_2 ];
			double n3z = p7[  0  ] - p3[ -_3 ];
			double n4x = p7[  0  ] - p4[ -_1 ];
			double n4y = p5[  0  ] - p4[ -_2 ];
			double n4z = p4[  _3 ] - p0[  0  ];
			double n5x = p6[  0  ] - p5[ -_1 ];
			double n5y = p5[  _2 ] - p4[  0  ];
			double n5z = p5[  _3 ] - p1[  0  ];
			double n6x = p6[  _1 ] - p5[  0  ];
			double n6y = p6[  _2 ] - p7[  0  ];
			double n6z = p6[  _3 ] - p2[  0  ];
			double n7x = p7[  _1 ] - p4[  0  ];
			double n7y = p6[  0  ] - p7[ -_2 ];
			double n7z = p7[  _3 ] - p3[  0  ];

			double nx  = ( n0x + ( n3x - n0x ) * xx ) + ( n1x - n0x + ( n0x - n1x + n2x - n3x ) * xx ) * yy;
			nx += ( ( n4x + ( n7x - n4x ) * xx ) + ( n5x - n4x + ( n4x - n5x + n6x - n7x ) * xx ) * yy - nx ) * zz;
			double ny  = ( n0y + ( n3y - n0y ) * xx ) + ( n1y - n0y + ( n0y - n1y + n2y - n3y ) * xx ) * yy;
			ny += ( ( n4y + ( n7y - n4y ) * xx ) + ( n5y - n4y + ( n4y - n5y + n6y - n7y ) * xx ) * yy - ny ) * zz;
			double nz  = ( n0z + ( n3z - n0z ) * xx ) + ( n1z - n0z + ( n0z - n1z + n2z - n3z ) * xx ) * yy;
			nz += ( ( n4z + ( n7z - n4z ) * xx ) + ( n5z - n4z + ( n4z - n5z + n6z - n7z ) * xx ) * yy - nz ) * zz;

			nx /= in.reso1( );
			ny /= in.reso2( );
			nz /= in.reso3( );
			double len = std::sqrt( nx * nx + ny * ny + nz * nz ) + type_limits< double >::minimum( );
			nx /= len;
			ny /= len;
			nz /= len;

			return( vector_type( nx, ny, nz ) );
		}
	};


	template < class Array1, class Array2, class T >
	class value_interpolation_and_mark
	{
	protected:
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::const_pointer const_mk_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;

	public:
		const Array1 &in;
		const Array2 &mk;
		const volumerender::parameter &p;
		const volumerender::attribute_table< T > &table;
		const volumerender::attribute_table< T > &mktable;
		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;

		value_interpolation_and_mark( const Array1 &image, const Array2 &mark, const volumerender::parameter &param, const volumerender::attribute_table< T > &tbl, const volumerender::attribute_table< T > &mktbl )
			: in( image ), mk( mark ), p( param ), table( tbl ), mktable( mktbl )
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		bool check( difference_type i, difference_type j, difference_type k ) const
		{
			const_pointer p = &in( i, j, k );
			const_mk_pointer pm = &mk( i, j, k );

			// この位置における物体が不透明の場合は次のステップへ移行する
			return( ( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
						table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
						table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) ) && 
					( mktable.has_alpha( pm[ d0 ] ) || mktable.has_alpha( pm[ d1 ] ) || mktable.has_alpha( pm[ d2 ] ) ||
						mktable.has_alpha( pm[ d3 ] ) || mktable.has_alpha( pm[ d4 ] ) || mktable.has_alpha( pm[ d5 ] ) ||
						mktable.has_alpha( pm[ d6 ] ) || mktable.has_alpha( pm[ d7 ] ) ) );
		}

		bool render( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz, attribute_type &oc ) const
		{
			// マーキング結果を重ねる
			const_mk_pointer pm = &mk( i, j, k );

			// 近傍の8点に対応する色と不透明度を取得
			const attribute_type &mc0 = mktable[ volumerender::to_integer( pm[ d0 ] ) ];
			const attribute_type &mc1 = mktable[ volumerender::to_integer( pm[ d1 ] ) ];
			const attribute_type &mc2 = mktable[ volumerender::to_integer( pm[ d2 ] ) ];
			const attribute_type &mc3 = mktable[ volumerender::to_integer( pm[ d3 ] ) ];
			const attribute_type &mc4 = mktable[ volumerender::to_integer( pm[ d4 ] ) ];
			const attribute_type &mc5 = mktable[ volumerender::to_integer( pm[ d5 ] ) ];
			const attribute_type &mc6 = mktable[ volumerender::to_integer( pm[ d6 ] ) ];
			const attribute_type &mc7 = mktable[ volumerender::to_integer( pm[ d7 ] ) ];

			int number_of_visible_voxels = mc0.has_alpha;
			number_of_visible_voxels += mc1.has_alpha;
			number_of_visible_voxels += mc2.has_alpha;
			number_of_visible_voxels += mc3.has_alpha;
			number_of_visible_voxels += mc4.has_alpha;
			number_of_visible_voxels += mc5.has_alpha;
			number_of_visible_voxels += mc6.has_alpha;
			number_of_visible_voxels += mc7.has_alpha;

			if( number_of_visible_voxels > 0 )
			{
				const_pointer p = &in( i, j, k );

				// CT値に対応する色と不透明度を取得
				double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
				ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

				oc = table[ volumerender::to_integer( ct ) ];

				// マークデータの透明度を線形補間する
				double alpha = ( mc0.alpha + ( mc3.alpha - mc0.alpha ) * xx ) + ( mc1.alpha - mc0.alpha + ( mc0.alpha - mc1.alpha + mc2.alpha - mc3.alpha ) * xx ) * yy;
				alpha = alpha + ( ( mc4.alpha + ( mc7.alpha - mc4.alpha ) * xx ) + ( mc5.alpha - mc4.alpha + ( mc4.alpha - mc5.alpha + mc6.alpha - mc7.alpha ) * xx ) * yy - alpha ) * zz;

				// マーキングの色を合成する
				oc.alpha *= alpha;

				return( true );
			}
			else
			{
				return( false );
			}
		}

		vector_type normal( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz ) const
		{
			const_pointer p0 = &in( i, j, k );
			const_pointer p1 = p0 + d1;
			const_pointer p2 = p0 + d2;
			const_pointer p3 = p0 + d3;
			const_pointer p4 = p0 + d4;
			const_pointer p5 = p0 + d5;
			const_pointer p6 = p0 + d6;
			const_pointer p7 = p0 + d7;

			double n0x = p3[  0  ] - p0[ -_1 ];
			double n0y = p1[  0  ] - p0[ -_2 ];
			double n0z = p4[  0  ] - p0[ -_3 ];
			double n1x = p2[  0  ] - p1[ -_1 ];
			double n1y = p1[  _2 ] - p0[  0  ];
			double n1z = p5[  0  ] - p1[ -_3 ];
			double n2x = p2[  _1 ] - p1[  0  ];
			double n2y = p2[  _2 ] - p3[  0  ];
			double n2z = p6[  0  ] - p2[ -_3 ];
			double n3x = p3[  _1 ] - p0[  0  ];
			double n3y = p2[  0  ] - p3[ -_2 ];
			double n3z = p7[  0  ] - p3[ -_3 ];
			double n4x = p7[  0  ] - p4[ -_1 ];
			double n4y = p5[  0  ] - p4[ -_2 ];
			double n4z = p4[  _3 ] - p0[  0  ];
			double n5x = p6[  0  ] - p5[ -_1 ];
			double n5y = p5[  _2 ] - p4[  0  ];
			double n5z = p5[  _3 ] - p1[  0  ];
			double n6x = p6[  _1 ] - p5[  0  ];
			double n6y = p6[  _2 ] - p7[  0  ];
			double n6z = p6[  _3 ] - p2[  0  ];
			double n7x = p7[  _1 ] - p4[  0  ];
			double n7y = p6[  0  ] - p7[ -_2 ];
			double n7z = p7[  _3 ] - p3[  0  ];

			double nx  = ( n0x + ( n3x - n0x ) * xx ) + ( n1x - n0x + ( n0x - n1x + n2x - n3x ) * xx ) * yy;
			nx += ( ( n4x + ( n7x - n4x ) * xx ) + ( n5x - n4x + ( n4x - n5x + n6x - n7x ) * xx ) * yy - nx ) * zz;
			double ny  = ( n0y + ( n3y - n0y ) * xx ) + ( n1y - n0y + ( n0y - n1y + n2y - n3y ) * xx ) * yy;
			ny += ( ( n4y + ( n7y - n4y ) * xx ) + ( n5y - n4y + ( n4y - n5y + n6y - n7y ) * xx ) * yy - ny ) * zz;
			double nz  = ( n0z + ( n3z - n0z ) * xx ) + ( n1z - n0z + ( n0z - n1z + n2z - n3z ) * xx ) * yy;
			nz += ( ( n4z + ( n7z - n4z ) * xx ) + ( n5z - n4z + ( n4z - n5z + n6z - n7z ) * xx ) * yy - nz ) * zz;

			nx /= in.reso1( );
			ny /= in.reso2( );
			nz /= in.reso3( );
			double len = std::sqrt( nx * nx + ny * ny + nz * nz ) + type_limits< double >::minimum( );
			nx /= len;
			ny /= len;
			nz /= len;

			return( vector_type( nx, ny, nz ) );
		}
	};


	template < class Array1, class Array2, class T >
	class value_interpolation_or_mark
	{
	protected:
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::const_pointer const_mk_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;

	public:
		const Array1 &in;
		const Array2 &mk;
		const volumerender::parameter &p;
		const volumerender::attribute_table< T > &table;
		const volumerender::attribute_table< T > &mktable;
		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;

		value_interpolation_or_mark( const Array1 &image, const Array2 &mark, const volumerender::parameter &param, const volumerender::attribute_table< T > &tbl, const volumerender::attribute_table< T > &mktbl )
			: in( image ), mk( mark ), p( param ), table( tbl ), mktable( mktbl )
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		bool check( difference_type i, difference_type j, difference_type k ) const
		{
			const_pointer p = &in( i, j, k );
			const_mk_pointer pm = &mk( i, j, k );

			// この位置における物体が不透明の場合は次のステップへ移行する
			return( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) ||
					table.has_alpha( p[ d2 ] ) || table.has_alpha( p[ d3 ] ) ||
					table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
					table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) ||
					mktable.has_alpha( pm[ d0 ] ) || mktable.has_alpha( pm[ d1 ] ) ||
					mktable.has_alpha( pm[ d2 ] ) || mktable.has_alpha( pm[ d3 ] ) ||
					mktable.has_alpha( pm[ d4 ] ) || mktable.has_alpha( pm[ d5 ] ) ||
					mktable.has_alpha( pm[ d6 ] ) || mktable.has_alpha( pm[ d7 ] ) );
		}

		bool render( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz, attribute_type &oc ) const
		{
			const_pointer p = &in( i, j, k );

			// CT値に対応する色と不透明度を取得
			double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
			ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

			oc = table[ volumerender::to_integer( ct ) ];

			// マーキング結果を重ねる
			const_mk_pointer pm = &mk( i, j, k );

			// 近傍の8点に対応する色と不透明度を取得
			 attribute_type mc0 = mktable[ volumerender::to_integer( pm[ d0 ] ) ];
			 attribute_type mc1 = mktable[ volumerender::to_integer( pm[ d1 ] ) ];
			 attribute_type mc2 = mktable[ volumerender::to_integer( pm[ d2 ] ) ];
			 attribute_type mc3 = mktable[ volumerender::to_integer( pm[ d3 ] ) ];
			 attribute_type mc4 = mktable[ volumerender::to_integer( pm[ d4 ] ) ];
			 attribute_type mc5 = mktable[ volumerender::to_integer( pm[ d5 ] ) ];
			 attribute_type mc6 = mktable[ volumerender::to_integer( pm[ d6 ] ) ];
			 attribute_type mc7 = mktable[ volumerender::to_integer( pm[ d7 ] ) ];

			int number_of_visible_voxels = mc0.has_alpha;
			number_of_visible_voxels += mc1.has_alpha;
			number_of_visible_voxels += mc2.has_alpha;
			number_of_visible_voxels += mc3.has_alpha;
			number_of_visible_voxels += mc4.has_alpha;
			number_of_visible_voxels += mc5.has_alpha;
			number_of_visible_voxels += mc6.has_alpha;
			number_of_visible_voxels += mc7.has_alpha;

			if( number_of_visible_voxels > 0 )
			{

				attribute_type mc;
				// まず平均的な色を決定する
				mc.pixel = ( mc0.pixel * mc0.has_alpha + mc1.pixel * mc1.has_alpha
					+ mc2.pixel * mc2.has_alpha + mc3.pixel * mc3.has_alpha
					+ mc4.pixel * mc4.has_alpha + mc5.pixel * mc5.has_alpha
					+ mc6.pixel * mc6.has_alpha + mc7.pixel * mc7.has_alpha ) / static_cast< double >( number_of_visible_voxels );

				// 透明物体があった場合は，周りの不透明物体の色で置き換えることでもあれを回避
				if( !mc0.has_alpha ){ mc0.pixel = mc.pixel; }
				if( !mc1.has_alpha ){ mc1.pixel = mc.pixel; }
				if( !mc2.has_alpha ){ mc2.pixel = mc.pixel; }
				if( !mc3.has_alpha ){ mc3.pixel = mc.pixel; }
				if( !mc4.has_alpha ){ mc4.pixel = mc.pixel; }
				if( !mc5.has_alpha ){ mc5.pixel = mc.pixel; }
				if( !mc6.has_alpha ){ mc6.pixel = mc.pixel; }
				if( !mc7.has_alpha ){ mc7.pixel = mc.pixel; }

				mc.pixel = ( mc0.pixel + ( mc3.pixel - mc0.pixel ) * xx ) + ( mc1.pixel - mc0.pixel + ( mc0.pixel - mc1.pixel + mc2.pixel - mc3.pixel ) * xx ) * yy;
				mc.pixel = mc.pixel + ( ( mc4.pixel + ( mc7.pixel - mc4.pixel ) * xx ) + ( mc5.pixel - mc4.pixel + ( mc4.pixel - mc5.pixel + mc6.pixel - mc7.pixel ) * xx ) * yy - mc.pixel ) * zz;
				mc.alpha = ( mc0.alpha + ( mc3.alpha - mc0.alpha ) * xx ) + ( mc1.alpha - mc0.alpha + ( mc0.alpha - mc1.alpha + mc2.alpha - mc3.alpha ) * xx ) * yy;
				mc.alpha = mc.alpha + ( ( mc4.alpha + ( mc7.alpha - mc4.alpha ) * xx ) + ( mc5.alpha - mc4.alpha + ( mc4.alpha - mc5.alpha + mc6.alpha - mc7.alpha ) * xx ) * yy - mc.alpha ) * zz;

				// マーキングの色を合成する
				oc.pixel = oc.pixel * ( 1.0 - mc.alpha ) + mc.pixel * mc.alpha;
				oc.alpha = ( oc.alpha + mc.alpha ) / 2.0;

				return( true );
			}
			else
			{
				return( oc.has_alpha );
			}
		}

		template < class Pointer >
		vector_type normal( Pointer p, double xx, double yy, double zz ) const
		{
			const_pointer p0 = p;
			const_pointer p1 = p0 + d1;
			const_pointer p2 = p0 + d2;
			const_pointer p3 = p0 + d3;
			const_pointer p4 = p0 + d4;
			const_pointer p5 = p0 + d5;
			const_pointer p6 = p0 + d6;
			const_pointer p7 = p0 + d7;

			double n0x = p3[  0  ] - p0[ -_1 ];
			double n0y = p1[  0  ] - p0[ -_2 ];
			double n0z = p4[  0  ] - p0[ -_3 ];
			double n1x = p2[  0  ] - p1[ -_1 ];
			double n1y = p1[  _2 ] - p0[  0  ];
			double n1z = p5[  0  ] - p1[ -_3 ];
			double n2x = p2[  _1 ] - p1[  0  ];
			double n2y = p2[  _2 ] - p3[  0  ];
			double n2z = p6[  0  ] - p2[ -_3 ];
			double n3x = p3[  _1 ] - p0[  0  ];
			double n3y = p2[  0  ] - p3[ -_2 ];
			double n3z = p7[  0  ] - p3[ -_3 ];
			double n4x = p7[  0  ] - p4[ -_1 ];
			double n4y = p5[  0  ] - p4[ -_2 ];
			double n4z = p4[  _3 ] - p0[  0  ];
			double n5x = p6[  0  ] - p5[ -_1 ];
			double n5y = p5[  _2 ] - p4[  0  ];
			double n5z = p5[  _3 ] - p1[  0  ];
			double n6x = p6[  _1 ] - p5[  0  ];
			double n6y = p6[  _2 ] - p7[  0  ];
			double n6z = p6[  _3 ] - p2[  0  ];
			double n7x = p7[  _1 ] - p4[  0  ];
			double n7y = p6[  0  ] - p7[ -_2 ];
			double n7z = p7[  _3 ] - p3[  0  ];

			double nx  = ( n0x + ( n3x - n0x ) * xx ) + ( n1x - n0x + ( n0x - n1x + n2x - n3x ) * xx ) * yy;
			nx += ( ( n4x + ( n7x - n4x ) * xx ) + ( n5x - n4x + ( n4x - n5x + n6x - n7x ) * xx ) * yy - nx ) * zz;
			double ny  = ( n0y + ( n3y - n0y ) * xx ) + ( n1y - n0y + ( n0y - n1y + n2y - n3y ) * xx ) * yy;
			ny += ( ( n4y + ( n7y - n4y ) * xx ) + ( n5y - n4y + ( n4y - n5y + n6y - n7y ) * xx ) * yy - ny ) * zz;
			double nz  = ( n0z + ( n3z - n0z ) * xx ) + ( n1z - n0z + ( n0z - n1z + n2z - n3z ) * xx ) * yy;
			nz += ( ( n4z + ( n7z - n4z ) * xx ) + ( n5z - n4z + ( n4z - n5z + n6z - n7z ) * xx ) * yy - nz ) * zz;

			nx /= in.reso1( );
			ny /= in.reso2( );
			nz /= in.reso3( );
			double len = std::sqrt( nx * nx + ny * ny + nz * nz ) + type_limits< double >::minimum( );
			nx /= len;
			ny /= len;
			nz /= len;

			return( vector_type( nx, ny, nz ) );
		}


		vector_type normal( difference_type i, difference_type j, difference_type k, double xx, double yy, double zz ) const
		{
			const_pointer p = &in( i, j, k );
			const_mk_pointer pm = &mk( i, j, k );

			// 近傍の8点に対応する色と不透明度を取得
			const attribute_type &oc0 = table[ volumerender::to_integer( p[ d0 ] ) ];
			const attribute_type &oc1 = table[ volumerender::to_integer( p[ d1 ] ) ];
			const attribute_type &oc2 = table[ volumerender::to_integer( p[ d2 ] ) ];
			const attribute_type &oc3 = table[ volumerender::to_integer( p[ d3 ] ) ];
			const attribute_type &oc4 = table[ volumerender::to_integer( p[ d4 ] ) ];
			const attribute_type &oc5 = table[ volumerender::to_integer( p[ d5 ] ) ];
			const attribute_type &oc6 = table[ volumerender::to_integer( p[ d6 ] ) ];
			const attribute_type &oc7 = table[ volumerender::to_integer( p[ d7 ] ) ];

			const attribute_type &mc0 = mktable[ volumerender::to_integer( pm[ d0 ] ) ];
			const attribute_type &mc1 = mktable[ volumerender::to_integer( pm[ d1 ] ) ];
			const attribute_type &mc2 = mktable[ volumerender::to_integer( pm[ d2 ] ) ];
			const attribute_type &mc3 = mktable[ volumerender::to_integer( pm[ d3 ] ) ];
			const attribute_type &mc4 = mktable[ volumerender::to_integer( pm[ d4 ] ) ];
			const attribute_type &mc5 = mktable[ volumerender::to_integer( pm[ d5 ] ) ];
			const attribute_type &mc6 = mktable[ volumerender::to_integer( pm[ d6 ] ) ];
			const attribute_type &mc7 = mktable[ volumerender::to_integer( pm[ d7 ] ) ];

			double oc_alpha = oc0.alpha + oc1.alpha + oc2.alpha + oc3.alpha + oc4.alpha + oc5.alpha + oc6.alpha + oc7.alpha;
			double mc_alpha = mc0.alpha + mc1.alpha + mc2.alpha + mc3.alpha + mc4.alpha + mc5.alpha + mc6.alpha + mc7.alpha;
			if( oc_alpha > mc_alpha )
			{
				return( normal( p, xx, yy, zz ) );
			}
			else
			{
				return( normal( pm, xx, yy, zz ) );
			}
		}
	};
}


// ボリュームレンダリング
namespace __volumerendering_controller__
{
	// いろいろなレンダラ（色の決定方法）を組み合わせることが可能なボリュームレンダリングエンジン
	template < class Array1, class Array2, class DepthMap, class Renderer, class T >
	bool volumerendering( const Array1 &in, Array2 &out, const DepthMap &depth_map, const Renderer &renderer, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;
		typedef typename volumerender::attribute_table< T >::pixel_type pixel_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::value_type out_value_type;

		vector_type offset  = param.offset;
		vector_type osffset = vector_type( offset.x / in.reso1( ), offset.y / in.reso2( ), offset.z / in.reso3( ) );
		vector_type pos     = param.pos - offset;
		vector_type dir     = param.dir.unit( );
		vector_type up      = param.up.unit( );
		const  volumerender::boundingbox *box = param.box;
		double fovy          = param.fovy;
		double ambient_ratio = param.ambient_ratio;
		double diffuse_ratio = param.diffuse_ratio;
		double specular      = param.specular;
		bool   bSpecular     = specular > 0.0;
		double lightAtten    = param.light_attenuation;
		double sampling_step = param.sampling_step;
		double termination   = param.termination;
		bool   bperspective  = param.perspective_view;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		// スライス座標系の実寸をワールドと考える
		vector_type casting_start, casting_end;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double left_to_right = param.left_to_right ? 1.0 : -1.0;
		double top_to_bottom = param.top_to_bottom ? 1.0 : -1.0;
		double front_to_back = param.front_to_back ? 1.0 : -1.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );
		double asx = ax * left_to_right;
		double asy = ay * top_to_bottom;
		double asz = az * front_to_back;

		double asp = out.reso2( ) / out.reso1( );

		double masp = ax < ay ? ax : ay;
		masp = masp < az ? masp : az;

		vector_type yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
			focal *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		for( size_type j = thread_id ; j < image_height ; j += thread_num )
		{
			for( size_type i = 0 ; i < image_width ; i++ )
			{
				// 投影面上の点をカメラ座標系に変換
				vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), focal );

				// レイ方向をカメラ座標系からワールド座標系に変換
				vector_type light;
				if( bperspective )
				{
					light = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );
				}
				else
				{
					pos = param.pos + yoko * Pos.x + up * Pos.y;
					light = dir;
				}

				pixel_type add_intensity( 0 );
				double add_opacity = 1;

				casting_start = pos;
				casting_end = pos + light * max_distance;

				// 物体との衝突判定
				if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
				{
					// 光の減衰を実現するために，カメラからの距離を測る
					Pos.x = pos.x / asx + osffset.x;
					Pos.y = pos.y / asy + osffset.y;
					Pos.z = pos.z / asz + osffset.z;

					// ワールド座標系（左手）からスライス座標系（右手）に変換
					// 以降は，全てスライス座標系で計算する
					casting_start.x = casting_start.x / asx + osffset.x;
					casting_start.y = casting_start.y / asy + osffset.y;
					casting_start.z = casting_start.z / asz + osffset.z;
					casting_end.x   = casting_end.x   / asx + osffset.x;
					casting_end.y   = casting_end.y   / asy + osffset.y;
					casting_end.z   = casting_end.z   / asz + osffset.z;

					// 光源の向きもスライス座標にあわせる
					vector_type slight = vector_type( light.x * left_to_right, light.y * top_to_bottom, light.z * front_to_back );

					vector_type spos = casting_start;
					vector_type ray = ( casting_end - casting_start ).unit( );

					// 光の減衰の距離を実測に直すためのパラメータ
					double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );

					// 直方体画素の画像上では方向によってサンプリング間隔が変わってしまう問題に対応
					double ray_sampling_step = sampling_step * masp / dlen;

					vector_type ray_step = ray * ray_sampling_step;

					double n = ( casting_end - casting_start ).length( );
					double l = 0, of = ( Pos - casting_start ).length( );

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						// この位置における物体が不透明の場合は次のステップへ移行する
						if( renderer.check( si, sj, sk ) )
						{
							if( l > 0 )
							{
								spos.x -= ray.x;
								spos.y -= ray.y;
								spos.z -= ray.z;
								l -= 1.0;
							}
							break;
						}

						double current_step = depth_map( si, sj, sk );
						l += current_step;
						spos.x += ray.x * current_step;
						spos.y += ray.y * current_step;
						spos.z += ray.z * current_step;
					}

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						attribute_type oc;

						if( renderer.render( si, sj, sk, xx, yy, zz, oc ) )
						{
							double lAtten = 1.0;
							if( lightAtten > 0.0 )
							{
								double len = ( l + of ) * dlen;
								lAtten /= 1.0 + lightAtten * ( len * len );
							}

							double c = slight.inner( renderer.normal( si, sj, sk, xx, yy, zz ) );
							c = c < 0.0 ? -c : c;

							double spec = 0.0;
							if( bSpecular )
							{
								spec = 2.0 * c * c - 1.0;

								if( spec <= 0.0 )
								{
									spec = 0;
								}
								else
								{
									spec *= spec;	//  2乗
									spec *= spec;	//  4乗
									spec *= spec;	//  8乗
									spec *= spec;	// 16乗
									spec *= spec;	// 32乗
									spec *= spec;	// 64乗
									//spec *= spec;	// 128乗
									spec *= specular * 255.0;
								}
							}

							c = c * diffuse_ratio + ambient_ratio;

							double alpha = oc.alpha * sampling_step;
							add_intensity += alpha * add_opacity * ( oc.pixel * c + spec ) * lAtten;
							add_opacity *= ( 1.0 - alpha );

							// 画素がレンダリング結果に与える影響がしきい値以下になった場合は終了
							if( add_opacity < termination )
							{
								break;
							}

							spos.x += ray_step.x;
							spos.y += ray_step.y;
							spos.z += ray_step.z;
							l += ray_sampling_step;
						}
						else
						{
							// この位置における物体が透明の場合は次のステップへ移行する
							spos += ray_step;
							l += ray_sampling_step;

							size_t count = 0;
							while( l < n )
							{
								difference_type si = volumerender::to_integer( spos.x );
								difference_type sj = volumerender::to_integer( spos.y );
								difference_type sk = volumerender::to_integer( spos.z );

								// この位置における物体が不透明の場合は次のステップへ移行する
								if( renderer.check( si, sj, sk ) )
								{
									if( count > 0 )
									{
										spos.x -= ray.x;
										spos.y -= ray.y;
										spos.z -= ray.z;
										l -= 1.0;
									}
									break;
								}

								double current_step = depth_map( si, sj, sk );
								l += current_step;
								spos.x += ray.x * current_step;
								spos.y += ray.y * current_step;
								spos.z += ray.z * current_step;

								count++;
							}
						}
					}
					out( i, j ) = static_cast< out_value_type >( mist::limits_0_255( add_intensity ) );
				}
				else
				{
					out( i, j ) = 0;
				}
			}
		}
		return( true );
	}

	// いろいろなレンダラ（色の決定方法）を組み合わせ，指定した位置のレイが最初にあたる位置を返す関数
	template < class Array1, class Array2, class DepthMap, class Renderer, class T >
	typename volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &out, const DepthMap &depth_map, const Renderer &renderer, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, typename Array1::size_type i, typename Array1::size_type j )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;
		typedef typename volumerender::attribute_table< T >::pixel_type pixel_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::value_type out_value_type;

		vector_type offset  = param.offset;
		vector_type osffset = vector_type( offset.x / in.reso1( ), offset.y / in.reso2( ), offset.z / in.reso3( ) );
		vector_type pos     = param.pos - offset;
		vector_type dir = param.dir.unit( );
		vector_type up = param.up.unit( );
		double fovy = param.fovy;
		double ambient_ratio = param.ambient_ratio;
		double diffuse_ratio = param.diffuse_ratio;
		double specular = param.specular;
		bool   bSpecular = specular > 0.0;
		const  volumerender::boundingbox *box = param.box;
		double lightAtten = param.light_attenuation;
		double sampling_step = param.sampling_step;
		double termination = param.termination;
		bool   bperspective = param.perspective_view;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		// スライス座標系の実寸をワールドと考える
		vector_type casting_start, casting_end;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double left_to_right = param.left_to_right ? 1.0 : -1.0;
		double top_to_bottom = param.top_to_bottom ? 1.0 : -1.0;
		double front_to_back = param.front_to_back ? 1.0 : -1.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );
		double asx = ax * left_to_right;
		double asy = ay * top_to_bottom;
		double asz = az * front_to_back;

		double asp = out.reso2( ) / out.reso1( );

		double masp = ax < ay ? ax : ay;
		masp = masp < az ? masp : az;

		vector_type yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
			focal *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		{
			// 投影面上の点をカメラ座標系に変換
			vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), focal );

			// レイ方向をカメラ座標系からワールド座標系に変換
			vector_type light;
			if( bperspective )
			{
				light = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );
			}
			else
			{
				pos = param.pos + yoko * Pos.x + up * Pos.y;
				light = dir;
			}

			double add_opacity = 1;
			double old_add_opacity = 0.0;

			casting_start = pos;
			casting_end = pos + light * max_distance;

			// 物体との衝突判定
			if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
				&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
				&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
				&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
				&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
				&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
			{
				// 光の減衰を実現するために，カメラからの距離を測る
				Pos.x = pos.x / asx + osffset.x;
				Pos.y = pos.y / asy + osffset.y;
				Pos.z = pos.z / asz + osffset.z;

				// ワールド座標系（左手）からスライス座標系（右手）に変換
				// 以降は，全てスライス座標系で計算する
				casting_start.x = casting_start.x / asx + osffset.x;
				casting_start.y = casting_start.y / asy + osffset.y;
				casting_start.z = casting_start.z / asz + osffset.z;
				casting_end.x   = casting_end.x   / asx + osffset.x;
				casting_end.y   = casting_end.y   / asy + osffset.y;
				casting_end.z   = casting_end.z   / asz + osffset.z;

				vector_type opos = casting_start;
				vector_type spos = casting_start;
				vector_type ray = ( casting_end - casting_start ).unit( );

				// 光の減衰の距離を実測に直すためのパラメータ
				double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );

				// 直方体画素の画像上では方向によってサンプリング間隔が変わってしまう問題に対応
				double ray_sampling_step = sampling_step * masp / dlen;

				vector_type ray_step = ray * ray_sampling_step;

				double n = ( casting_end - casting_start ).length( );
				double l = 0, of = ( Pos - casting_start ).length( );

				while( l < n )
				{
					difference_type si = volumerender::to_integer( spos.x );
					difference_type sj = volumerender::to_integer( spos.y );
					difference_type sk = volumerender::to_integer( spos.z );

					// この位置における物体が不透明の場合は次のステップへ移行する
					if( renderer.check( si, sj, sk ) )
					{
						if( l > 0 )
						{
							spos.x -= ray.x;
							spos.y -= ray.y;
							spos.z -= ray.z;
							l -= 1.0;
						}
						break;
					}

					double current_step = depth_map( si, sj, sk );
					l += current_step;
					spos.x += ray.x * current_step;
					spos.y += ray.y * current_step;
					spos.z += ray.z * current_step;
				}

				while( l < n )
				{
					difference_type si = volumerender::to_integer( spos.x );
					difference_type sj = volumerender::to_integer( spos.y );
					difference_type sk = volumerender::to_integer( spos.z );

					double xx = spos.x - si;
					double yy = spos.y - sj;
					double zz = spos.z - sk;

					attribute_type oc;

					if( renderer.render( si, sj, sk, xx, yy, zz, oc ) )
					{

						double alpha = oc.alpha * sampling_step;
						double aopacity = alpha * add_opacity;

						// 画素がレンダリング結果に与える影響がしきい値以下になった場合は終了
						if( old_add_opacity < aopacity )
						{
							opos = spos;
							old_add_opacity = aopacity;
						}

						add_opacity = add_opacity * ( 1.0 - alpha );

						// 画素がレンダリング結果に与える影響がしきい値以下になった場合は終了
						if( add_opacity < termination )
						{
							break;
						}

						spos.x += ray_step.x;
						spos.y += ray_step.y;
						spos.z += ray_step.z;
						l += ray_sampling_step;
					}
					else
					{
						// この位置における物体が透明の場合は次のステップへ移行する
						spos += ray_step;
						l += ray_sampling_step;

						size_t count = 0;
						while( l < n )
						{
							difference_type si = volumerender::to_integer( spos.x );
							difference_type sj = volumerender::to_integer( spos.y );
							difference_type sk = volumerender::to_integer( spos.z );

							// この位置における物体が不透明の場合は次のステップへ移行する
							if( renderer.check( si, sj, sk ) )
							{
								if( count > 0 )
								{
									spos.x -= ray.x;
									spos.y -= ray.y;
									spos.z -= ray.z;
									l -= 1.0;
								}
								break;
							}

							double current_step = depth_map( si, sj, sk );
							l += current_step;
							spos.x += ray.x * current_step;
							spos.y += ray.y * current_step;
							spos.z += ray.z * current_step;

							count++;
						}
					}
				}

				spos.x *= ax;
				spos.y *= ay;
				spos.z *= az;
				return( spos );
			}
			else
			{
				double infinity = type_limits< double >::maximum( );
				return( vector_type( infinity, infinity, infinity ) );
			}
		}
	}


	template < class Array1, class Array2, class DepthMap, class Renderer, class T >
	class volumerendering_thread : public mist::thread< volumerendering_thread< Array1, Array2, DepthMap, Renderer, T > >
	{
	public:
		typedef mist::thread< volumerendering_thread< Array1, Array2, DepthMap, Renderer, T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const Array1 *in_;
		Array2 *out_;
		const DepthMap *depth_map_;
		const Renderer *renderer_;
		const volumerender::parameter *param_;
		const volumerender::attribute_table< T > *table_;

	public:
		void setup_parameters( const Array1 &in, Array2 &out, const DepthMap &depth_map, const Renderer &renderer, const volumerender::parameter &p, const volumerender::attribute_table< T > &t, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			depth_map_ = &depth_map;
			renderer_ = &renderer;
			param_ = &p;
			table_ = &t;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		volumerendering_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), depth_map_( NULL ), renderer_( NULL ), param_( NULL ), table_( NULL )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			volumerendering( *in_, *out_, *depth_map_, *renderer_, *param_, *table_, thread_id_, thread_num_ );
			return( true );
		}
	};
}


namespace __mip_controller__
{
	template < class Array1, class Array2 >
	bool mip( const Array1 &in, Array2 &out, const volumerender::parameter &p, typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::value_type out_value_type;

		vector_type pos = p.pos;
		vector_type dir = p.dir.unit( );
		vector_type up = p.up.unit( );
		vector_type offset = p.offset;
		const volumerender::boundingbox *box = p.box;
		double sampling_step = p.sampling_step;

		const size_type w = in.width( );
		const size_type h = in.height( );
		const size_type d = in.depth( );

		const size_type image_width  = out.width( );
		const size_type image_height = out.height( );

		difference_type d0, d1, d2, d3, d4, d5, d6, d7, _1, _2, _3;
		{
			difference_type cx = in.width( ) / 2;
			difference_type cy = in.height( ) / 2;
			difference_type cz = in.depth( ) / 2;
			const_pointer ppp = &in( cx, cy, cz );
			d0 = 0;
			d1 = &in( cx    , cy + 1, cz     ) - ppp;
			d2 = &in( cx + 1, cy + 1, cz     ) - ppp;
			d3 = &in( cx + 1, cy    , cz     ) - ppp;
			d4 = &in( cx    , cy    , cz + 1 ) - ppp;
			d5 = &in( cx    , cy + 1, cz + 1 ) - ppp;
			d6 = &in( cx + 1, cy + 1, cz + 1 ) - ppp;
			d7 = &in( cx + 1, cy    , cz + 1 ) - ppp;
			_1 = &in( cx + 1, cy    , cz     ) - ppp;
			_2 = &in( cx    , cy + 1, cz     ) - ppp;
			_3 = &in( cx    , cy    , cz + 1 ) - ppp;
		}

		// スライス座標系の実寸をワールドと考える
		vector_type casting_start, casting_end;

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );

		double asp = out.reso2( ) / out.reso1( );

		double masp = ax < ay ? ax : ay;
		masp = masp < az ? masp : az;

		vector_type yoko = ( dir * up ).unit( );

		if( out.reso1( ) < out.reso2( ) )
		{
			yoko *= out.reso1( ) / out.reso2( );
		}
		else
		{
			up *= out.reso2( ) / out.reso1( );
		}

		double max_distance = pos.length( ) + std::sqrt( static_cast< double >( w * w + h * h + d * d ) );

		for( size_type j = thread_id ; j < image_height ; j += thread_num )
		{
			for( size_type i = 0 ; i < image_width ; i++ )
			{
				// 投影面上の点をカメラ座標系に変換
				vector_type Pos( static_cast< double >( i ) - cx, cy - static_cast< double >( j ), 0 );

				// 平行投影にする
				vector_type light;
				pos = p.pos + yoko * Pos.x + up * Pos.y;
				light = dir;

				double maximum_intensity = type_limits< double >::minimum( );

				casting_start = pos;
				casting_end = pos + light * max_distance;

				// 物体との衝突判定
				if( volumerender::check_intersection( casting_start, casting_end, box[ 0 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 1 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 2 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 3 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 4 ] )
					&& volumerender::check_intersection( casting_start, casting_end, box[ 5 ] ) )
				{
					// ワールド座標系（左手）からスライス座標系（右手）に変換
					// 以降は，全てスライス座標系で計算する
					casting_start.x = (  casting_start.x + offset.x ) / ax;
					casting_start.y = ( -casting_start.y + offset.y ) / ay;
					casting_start.z = (  casting_start.z + offset.z ) / az;
					casting_end.x   = (  casting_end.x   + offset.x ) / ax;
					casting_end.y   = ( -casting_end.y   + offset.y ) / ay;
					casting_end.z   = (  casting_end.z   + offset.z ) / az;

					vector_type spos = casting_start;
					vector_type ray = ( casting_end - casting_start ).unit( );

					// 光の減衰の距離を実測に直すためのパラメータ
					double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );

					// 直方体画素の画像上では方向によってサンプリング間隔が変わってしまう問題に対応
					double ray_sampling_step = sampling_step * masp / dlen;

					vector_type ray_step = ray * ray_sampling_step;

					double n = ( casting_end - casting_start ).length( );
					double l = 0;

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						const_pointer p = &in( si, sj, sk );
						double ct;

						// CT値に対応する色と不透明度を取得
						ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

						if( maximum_intensity < ct )
						{
							maximum_intensity = ct;
						}

						spos.x += ray_step.x;
						spos.y += ray_step.y;
						spos.z += ray_step.z;
						l += ray_sampling_step;
					}
				}

				out( i, j ) = maximum_intensity;
			}
		}
		return( true );
	}


	template < class Array1, class Array2 >
	class mip_thread : public mist::thread< mip_thread< Array1, Array2 > >
	{
	public:
		typedef mist::thread< mip_thread< Array1, Array2 > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const Array1 *in_;
		Array2 *out_;
		const volumerender::parameter *param_;

	public:
		void setup_parameters( const Array1 &in, Array2 &out, const volumerender::parameter &p, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			param_ = &p;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		mip_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ), in_( NULL ), out_( NULL ), param_( NULL )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			mip( *in_, *out_, *param_, thread_id_, thread_num_ );
			return( true );
		}
	};
}



//! @addtogroup visualization_group
//!
//!  @{


//! @addtogroup volumerendering_group
//!
//! @{


/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in         … 入力画像
//! @param[out] out        … 出力画像
//! @param[in]  dmap       … レンダリングを高速化するための距離画像
//! @param[in]  renderer   … ボリュームレンダリングの実装
//! @param[in]  param      … ボリュームレンダリングのパラメータ
//! @param[in]  table      … ボリュームレンダリングの色－値テーブル
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2, class DepthMap, class Renderer, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, Array2 &out, const DepthMap &dmap, const Renderer &renderer, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, typename Array1::size_type thread_num = 0 )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array1::size_type size_type;
	typedef __volumerendering_controller__::volumerendering_thread< Array1, Array2, DepthMap, Renderer, ATTRIBUTETYPE > volumerendering_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	volumerendering_thread *thread = new volumerendering_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, dmap, renderer, param, table, i, thread_num );
	}

	// スレッドを実行して，終了まで待機する
	do_threads_( thread, thread_num );

	delete [] thread;
	
	return( true );
}


/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in         … 入力画像
//! @param[out] out        … 出力画像
//! @param[in]  dmap       … レンダリングを高速化するための距離画像
//! @param[in]  param      … ボリュームレンダリングのパラメータ
//! @param[in]  table      … ボリュームレンダリングの色－値テーブル
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2, class DepthMap, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, Array2 &out, const DepthMap &dmap, const volumerender::parameter &param,
						const volumerender::attribute_table< ATTRIBUTETYPE > &table, typename Array1::size_type thread_num = 0 )
{
	if( param.value_interpolation )
	{
		typedef rendering_helper::value_interpolation< Array1, ATTRIBUTETYPE > Renderer;
		return( volumerendering( in, out, dmap, Renderer( in, param, table ), param, table, thread_num ) );
	}
	else
	{
		typedef rendering_helper::color_interpolation< Array1, ATTRIBUTETYPE > Renderer;
		return( volumerendering( in, out, dmap, Renderer( in, param, table ), param, table, thread_num ) );
	}
	
	return( true );
}


/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in         … 入力画像
//! @param[out] out        … 出力画像
//! @param[in]  param      … ボリュームレンダリングのパラメータ
//! @param[in]  table      … ボリュームレンダリングの色－値テーブル
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, Array2 &out, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, typename Array1::size_type thread_num = 0 )
{
	return( volumerendering( in, out, volumerender::no_depth_map( ), param, table, thread_num ) );
}



/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in         … 入力画像
//! @param[in]  mk         … マーク画像
//! @param[out] out        … 出力画像
//! @param[in]  dmap       … レンダリングを高速化するための距離画像
//! @param[in]  param      … ボリュームレンダリングのパラメータ
//! @param[in]  table      … ボリュームレンダリングの色－値テーブル
//! @param[in]  mktable    … マークデータ用の色－値テーブル
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2, class Array3, class DepthMap, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, typename Array1::size_type thread_num = 0 )
{
	if( in.width( ) != mk.width( ) || in.height( ) != mk.height( ) || in.depth( ) != mk.depth( ) )
	{
		return( false );
	}

	typedef rendering_helper::value_interpolation_with_mark< Array1, Array2, ATTRIBUTETYPE > Renderer;
	return( volumerendering( in, out, dmap, Renderer( in, mk, param, table, mktable ), param, table, thread_num ) );
}



/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in                  … 入力画像
//! @param[in]  mk                  … マーク画像
//! @param[out] out                 … 出力画像
//! @param[in]  dmap                … レンダリングを高速化するための距離画像
//! @param[in]  param               … ボリュームレンダリングのパラメータ
//! @param[in]  table               … ボリュームレンダリングの色－値テーブル
//! @param[in]  mktable             … マークデータ用の色－値テーブル
//! @param[in]  apply_and_operation … CTとマークの共通部分のみをレンダリングするか，和集合をレンダリングするか
//! @param[in]  thread_num          … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2, class Array3, class DepthMap, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, bool apply_and_operation, typename Array1::size_type thread_num = 0 )
{
	if( in.width( ) != mk.width( ) || in.height( ) != mk.height( ) || in.depth( ) != mk.depth( ) )
	{
		return( false );
	}

	if( apply_and_operation )
	{
		typedef rendering_helper::value_interpolation_and_mark< Array1, Array2, ATTRIBUTETYPE > Renderer;
		return( volumerendering( in, out, dmap, Renderer( in, mk, param, table, mktable ), param, table, thread_num ) );
	}
	else
	{
		typedef rendering_helper::value_interpolation_or_mark< Array1, Array2, ATTRIBUTETYPE > Renderer;
		return( volumerendering( in, out, dmap, Renderer( in, mk, param, table, mktable ), param, table, thread_num ) );
	}
}


/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in         … 入力画像
//! @param[in]  mk         … マーク画像
//! @param[out] out        … 出力画像
//! @param[in]  param      … ボリュームレンダリングのパラメータ
//! @param[in]  table      … ボリュームレンダリングの色－値テーブル
//! @param[in]  mktable    … マークデータ用の色－値テーブル
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2, class Array3, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, typename Array1::size_type thread_num = 0 )
{
	return( volumerendering( in, mk, out, volumerender::no_depth_map( ), param, table, mktable, thread_num ) );
}


/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in                  … 入力画像
//! @param[in]  mk                  … マーク画像
//! @param[out] out                 … 出力画像
//! @param[in]  param               … ボリュームレンダリングのパラメータ
//! @param[in]  table               … ボリュームレンダリングの色－値テーブル
//! @param[in]  mktable             … マークデータ用の色－値テーブル
//! @param[in]  apply_and_operation … CTとマークの共通部分のみをレンダリングするか，和集合をレンダリングするか
//! @param[in]  thread_num          … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2, class Array3, class ATTRIBUTETYPE >
bool volumerendering( const Array1 &in, const Array2 &mk, Array3 &out, const volumerender::parameter &param, const volumerender::attribute_table< ATTRIBUTETYPE > &table, const volumerender::attribute_table< ATTRIBUTETYPE > &mktable, bool apply_and_operation, typename Array1::size_type thread_num = 0 )
{
	return( volumerendering( in, mk, out, volumerender::no_depth_map( ), param, table, mktable, apply_and_operation, thread_num ) );
}



/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in         … 入力画像
//! @param[out] out        … 出力画像
//! @param[in]  p          … レンダリングパラメータ
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array1, class Array2 >
bool mip( const Array1 &in, Array2 &out, const volumerender::parameter &p, typename Array1::size_type thread_num = 0 )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array1::size_type size_type;
	typedef __mip_controller__::mip_thread< Array1, Array2 > mip_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	mip_thread *thread = new mip_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, p, i, thread_num );
	}

	// スレッドを実行して，終了まで待機する
	do_threads_( thread, thread_num );

	delete [] thread;
	
	return( true );
}

/// @brief 衝突判定ルーチン（MISTのボリュームレンダリングエンジンで利用）
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in] in         … 入力画像
//! @param[in] out        … 出力ボリュームレンダリング画像
//! @param[in] dmap       … レンダリングを高速化するための距離画像
//! @param[in] renderer   … ボリュームレンダリングの実装
//! @param[in] param      … ボリュームレンダリングのパラメータ
//! @param[in] table      … ボリュームレンダリングの色－値テーブル
//! @param[in] i          … 衝突判定を行う出力画像中のX軸方向の位置
//! @param[in] j          … 衝突判定を行う出力画像中のY軸方向の位置
//! 
//! @return 出力画像中の画素 (i, j) にもっとも影響を与える入力画像中の画素の座標
//! 
template < class Array1, class Array2, class DepthMap, class Renderer, class T >
volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &out, const DepthMap &dmap, const Renderer &renderer, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, typename Array1::size_type i, typename Array1::size_type j )
{
	return( __volumerendering_controller__::collision_detection( in, out, dmap, renderer, param, table, i, j ) );
}


/// @brief 衝突判定ルーチン（MISTのボリュームレンダリングエンジンで利用）
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in] in         … 入力画像
//! @param[in] out        … ボリュームレンダリング結果画像
//! @param[in] dmap       … レンダリングを高速化するための距離画像
//! @param[in] param      … ボリュームレンダリングのパラメータ
//! @param[in] table      … ボリュームレンダリングの色－値テーブル
//! @param[in] i          … 衝突判定を行う出力画像中のX軸方向の位置
//! @param[in] j          … 衝突判定を行う出力画像中のY軸方向の位置
//! 
//! @return 出力画像中の画素 (i, j) にもっとも影響を与える入力画像中の画素の座標
//! 
template < class Array1, class Array2, class DepthMap, class T >
volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, typename Array1::size_type i, typename Array1::size_type j )
{
	if( param.value_interpolation )
	{
		typedef rendering_helper::value_interpolation< Array1, T > Renderer;
		return( collision_detection( in, out, dmap, Renderer( in, param, table ), param, table, i, j ) );
	}
	else
	{
		typedef rendering_helper::color_interpolation< Array1, T > Renderer;
		return( collision_detection( in, out, dmap, Renderer( in, param, table ), param, table, i, j ) );
	}
}


/// @brief 衝突判定ルーチン（MISTのボリュームレンダリングエンジンで利用）
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in] in         … 入力画像
//! @param[in] out        … ボリュームレンダリング結果画像
//! @param[in] param      … ボリュームレンダリングのパラメータ
//! @param[in] table      … ボリュームレンダリングの色－値テーブル
//! @param[in] i          … 衝突判定を行う出力画像中のX軸方向の位置
//! @param[in] j          … 衝突判定を行う出力画像中のY軸方向の位置
//! 
//! @return 出力画像中の画素 (i, j) にもっとも影響を与える入力画像中の画素の座標
//! 
template < class Array1, class Array2, class T >
volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &out, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, typename Array1::size_type i, typename Array1::size_type j )
{
	return( collision_detection( in, out, volumerender::no_depth_map( ), param, table, i, j ) );
}


/// @brief 衝突判定ルーチン（MISTのボリュームレンダリングエンジンで利用）
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in] in         … 入力画像
//! @param[in] mk         … マーク画像
//! @param[in] out        … ボリュームレンダリング結果画像
//! @param[in] dmap       … レンダリングを高速化するための距離画像
//! @param[in] param      … ボリュームレンダリングのパラメータ
//! @param[in] table      … ボリュームレンダリングの色－値テーブル
//! @param[in] mktable    … マークデータ用の色－値テーブル
//! @param[in] i          … 衝突判定を行う出力画像中のX軸方向の位置
//! @param[in] j          … 衝突判定を行う出力画像中のY軸方向の位置
//! 
//! @return 出力画像中の画素 (i, j) にもっとも影響を与える入力画像中の画素の座標
//! 
template < class Array1, class Array2, class Array3, class DepthMap, class T >
volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &mk, const Array3 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, const volumerender::attribute_table< T > &mktable, typename Array1::size_type i, typename Array1::size_type j )
{
	typedef rendering_helper::value_interpolation_with_mark< Array1, Array2, T > Renderer;
	return( collision_detection( in, out, dmap, Renderer( in, mk, param, table, mktable ), param, table, i, j ) );
}


/// @brief 衝突判定ルーチン（MISTのボリュームレンダリングエンジンで利用）
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in] in                  … 入力画像
//! @param[in] mk                  … マーク画像
//! @param[in] out                 … ボリュームレンダリング結果画像
//! @param[in] dmap                … レンダリングを高速化するための距離画像
//! @param[in] param               … ボリュームレンダリングのパラメータ
//! @param[in] table               … ボリュームレンダリングの色－値テーブル
//! @param[in] mktable             … マークデータ用の色－値テーブル
//! @param[in] apply_and_operation … CTとマークの共通部分のみをレンダリングするか，和集合をレンダリングするか
//! @param[in] i                   … 衝突判定を行う出力画像中のX軸方向の位置
//! @param[in] j                   … 衝突判定を行う出力画像中のY軸方向の位置
//! 
//! @return 出力画像中の画素 (i, j) にもっとも影響を与える入力画像中の画素の座標
//! 
template < class Array1, class Array2, class Array3, class DepthMap, class T >
volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &mk, const Array3 &out, const DepthMap &dmap, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, const volumerender::attribute_table< T > &mktable, bool apply_and_operation, typename Array1::size_type i, typename Array1::size_type j )
{
	if( apply_and_operation )
	{
		typedef rendering_helper::value_interpolation_and_mark< Array1, Array2, T > Renderer;
		return( collision_detection( in, out, dmap, Renderer( in, mk, param, table, mktable ), param, table, i, j ) );
	}
	else
	{
		typedef rendering_helper::value_interpolation_or_mark< Array1, Array2, T > Renderer;
		return( collision_detection( in, out, dmap, Renderer( in, mk, param, table, mktable ), param, table, i, j ) );
	}
}


/// @brief 衝突判定ルーチン（MISTのボリュームレンダリングエンジンで利用）
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in] in         … 入力画像
//! @param[in] mk         … マーク画像
//! @param[in] out        … ボリュームレンダリング結果画像
//! @param[in] param      … ボリュームレンダリングのパラメータ
//! @param[in] table      … ボリュームレンダリングの色－値テーブル
//! @param[in] mktable    … マークデータ用の色－値テーブル
//! @param[in] i          … 衝突判定を行う出力画像中のX軸方向の位置
//! @param[in] j          … 衝突判定を行う出力画像中のY軸方向の位置
//! 
//! @return 出力画像中の画素 (i, j) にもっとも影響を与える入力画像中の画素の座標
//! 
template < class Array1, class Array2, class Array3, class T >
volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &mk, const Array3 &out, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, const volumerender::attribute_table< T > &mktable, typename Array1::size_type i, typename Array1::size_type j )
{
	return( collision_detection( in, mk, out, volumerender::no_depth_map( ), param, table, mktable, i, j ) );
}


/// @brief 衝突判定ルーチン（MISTのボリュームレンダリングエンジンで利用）
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in] in                  … 入力画像
//! @param[in] mk                  … マーク画像
//! @param[in] out                 … ボリュームレンダリング結果画像
//! @param[in] param               … ボリュームレンダリングのパラメータ
//! @param[in] table               … ボリュームレンダリングの色－値テーブル
//! @param[in] mktable             … マークデータ用の色－値テーブル
//! @param[in] apply_and_operation … CTとマークの共通部分のみをレンダリングするか，和集合をレンダリングするか
//! @param[in] i                   … 衝突判定を行う出力画像中のX軸方向の位置
//! @param[in] j                   … 衝突判定を行う出力画像中のY軸方向の位置
//! 
//! @return 出力画像中の画素 (i, j) にもっとも影響を与える入力画像中の画素の座標
//! 
template < class Array1, class Array2, class Array3, class T >
volumerender::parameter::vector_type collision_detection( const Array1 &in, const Array2 &mk, const Array3 &out, const volumerender::parameter &param, const volumerender::attribute_table< T > &table, const volumerender::attribute_table< T > &mktable, bool apply_and_operation, typename Array1::size_type i, typename Array1::size_type j )
{
	return( collision_detection( in, mk, out, volumerender::no_depth_map( ), param, table, mktable, apply_and_operation, i, j ) );
}



/// @brief ボリュームレンダリング
//! 
//! @attention 入力と出力は，別のMISTコンテナオブジェクトでなくてはならない
//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
//!
//! @param[in]  in         … 入力画像
//! @param[out] dmap       … レンダリングを高速化するための距離画像の計算結果
//! @param[in]  table      … ボリュームレンダリングの色－値テーブル
//! @param[in]  thread_num … 使用するスレッド数
//! 
//! @retval true  … ボリュームレンダリングに成功
//! @retval false … 入力と出力が同じオブジェクトを指定した場合
//! 
template < class Array, class DepthMap, class ATTRIBUTETYPE >
bool generate_depth_map( const Array &in, DepthMap &dmap, const volumerender::attribute_table< ATTRIBUTETYPE > &table, typename Array::size_type thread_num = 0 )
{
	if( is_same_object( in, dmap ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array::size_type size_type;

	dmap.resize( in.width( ) / 2, in.height( ) / 2, in.depth( ) / 2 );
	dmap.reso( 1.0, 1.0, 1.0 );

	for( size_type k = 0 ; k < dmap.depth( ) ; k++ )
	{
		size_type _3 = k * 2;
		for( size_type j = 0 ; j < dmap.height( ) ; j++ )
		{
			size_type _2 = j * 2;
			for( size_type i = 0 ; i < dmap.width( ) ; i++ )
			{
				size_type _1 = i * 2;
				bool b1 = table.has_alpha( in( _1    , _2    , _3     ) );
				bool b2 = table.has_alpha( in( _1 + 1, _2    , _3     ) );
				bool b3 = table.has_alpha( in( _1    , _2 + 1, _3     ) );
				bool b4 = table.has_alpha( in( _1 + 1, _2 + 1, _3     ) );
				bool b5 = table.has_alpha( in( _1    , _2    , _3 + 1 ) );
				bool b6 = table.has_alpha( in( _1 + 1, _2    , _3 + 1 ) );
				bool b7 = table.has_alpha( in( _1    , _2 + 1, _3 + 1 ) );
				bool b8 = table.has_alpha( in( _1 + 1, _2 + 1, _3 + 1 ) );

				if( b1 || b2 || b3 || b4 || b5 || b6 || b7 || b8 )
				{
					dmap( i, j, k ) = 0;
				}
				else
				{
					dmap( i, j, k ) = 1;
				}
			}
		}
	}

	euclidean::distance_transform( dmap, dmap, thread_num );

	for( size_type i = 0 ; i < dmap.size( ) ; i++ )
	{
		dmap[ i ] = std::sqrt( dmap[ i ] );
	}
	
	return( true );
}


/// @}
// ボリュームレンダリンググループの終わり

/// @}
//  可視化グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_VOLUMERENDER__
