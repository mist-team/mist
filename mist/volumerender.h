/// @file mist/volumerender.h
//!
//! @brief ３次元画像のボリュームレンダリングエンジン
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
		value_type &operator []( difference_type index )
		{
			return( base::operator []( zero_index_ + index ) );
		}

		const value_type &operator []( difference_type index ) const
		{
			return( base::operator []( zero_index_ + index ) );
		}

		value_type &at( difference_type index )
		{
			return( base::at( zero_index_ + index ) );
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

		attribute_table( ) : sindex_( 0 ), eindex_( -1 ), zero_index_( NULL ){ }

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
		double	fovy;
		double	ambient_ratio;
		double	diffuse_ratio;
		double	light_attenuation;
		double	sampling_step;
		double	termination;
		double	specular;

		boundingbox box[ 6 ];

		parameter( ) : fovy( 80.0 ), ambient_ratio( 0.4 ), diffuse_ratio( 0.6 ), light_attenuation( 0.0 ), sampling_step( 1.0 ), termination( 0.01 ), specular( 1.0 )
		{
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
}


/// @}
// ボリュームレンダリンググループの終わり

/// @}
//  可視化グループの終わり



// 値補間タイプのボリュームレンダリング
namespace value_interpolation
{
	template < class Array1, class Array2, class T >
	bool volumerendering( const Array1 &in, Array2 &out, const volumerender::parameter &p, const volumerender::attribute_table< T > &table,
																							typename Array1::size_type thread_id, typename Array1::size_type thread_num )
	{
		typedef typename volumerender::parameter::vector_type vector_type;
		typedef typename volumerender::attribute_table< T >::attribute_type attribute_type;
		typedef typename volumerender::attribute_table< T >::pixel_type pixel_type;
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;
		typedef typename Array1::const_pointer const_pointer;
		typedef typename Array2::value_type out_value_type;

		vector_type pos = p.pos;
		vector_type dir = p.dir;
		vector_type up = p.up;
		vector_type offset = p.offset;
		double fovy = p.fovy;
		double ambient_ratio = p.ambient_ratio;
		double diffuse_ratio = p.diffuse_ratio;
		double specular = p.specular;
		bool   bSpecular = specular > 0.0;
		const volumerender::boundingbox *box = p.box;
		double lightAtten = p.light_attenuation;
		double sampling_step = p.sampling_step;
		double termination = p.termination;

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
		vector_type normal, n1, n2, n3, n4, n5, n6, n7, n8;
		vector_type casting_start, casting_end;

		const double pai = 3.1415926535897932384626433832795;
		double focal = ( static_cast< double >( image_height ) / 2.0 ) / std::tan( fovy * pai / 180.0 / 2.0 );

		double cx = static_cast< double >( image_width ) / 2.0;
		double cy = static_cast< double >( image_height ) / 2.0;
		double ax = in.reso1( );
		double ay = in.reso2( );
		double az = in.reso3( );

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
				vector_type light = ( yoko * Pos.x + up * Pos.y + dir * Pos.z ).unit( );

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
					Pos.x = (  pos.x + offset.x ) / ax;
					Pos.y = ( -pos.y + offset.y ) / ay;
					Pos.z = (  pos.z + offset.z ) / az;

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

					double accelerated_step = 2.0;
					vector_type ray_accelerated_step = ray * accelerated_step;
					vector_type ray_step = ray * sampling_step;

					double dlen = vector_type( ray.x * ax, ray.y * ay, ray.z * az ).length( );
					double n = ( casting_end - casting_start ).length( );
					double l = 0, of = ( Pos - casting_start ).length( );

					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						const_pointer p = &in( si, sj, sk );

						// 画素値に対応する色と不透明度を取得
						if( table.has_alpha( p[ d0 ] ) || table.has_alpha( p[ d1 ] ) || table.has_alpha( p[ d2 ] ) ||
							table.has_alpha( p[ d3 ] ) || table.has_alpha( p[ d4 ] ) || table.has_alpha( p[ d5 ] ) ||
							table.has_alpha( p[ d6 ] ) || table.has_alpha( p[ d7 ] ) )
						{
							if( l > 0 )
							{
								double sstep = accelerated_step - sampling_step;
								spos.x -= ray.x * sstep;
								spos.y -= ray.y * sstep;
								spos.z -= ray.z * sstep;
								l -= accelerated_step - sampling_step;
							}
							break;
						}

						l += accelerated_step;
						spos.x += ray_accelerated_step.x;
						spos.y += ray_accelerated_step.y;
						spos.z += ray_accelerated_step.z;
					}


					while( l < n )
					{
						difference_type si = volumerender::to_integer( spos.x );
						difference_type sj = volumerender::to_integer( spos.y );
						difference_type sk = volumerender::to_integer( spos.z );

						double xx = spos.x - si;
						double yy = spos.y - sj;
						double zz = spos.z - sk;

						const_pointer p = &in( si, sj, sk );

						// CT値に対応する色と不透明度を取得
						double ct = ( p[ d0 ] + ( p[ d3 ] - p[ d0 ] ) * xx ) + ( p[ d1 ] - p[ d0 ] + ( p[ d0 ] - p[ d1 ] + p[ d2 ] - p[ d3 ] ) * xx ) * yy;
						ct += ( ( p[ d4 ] + ( p[ d7 ] - p[ d4 ] ) * xx ) + ( p[ d5 ] - p[ d4 ] + ( p[ d4 ] - p[ d5 ] + p[ d6 ] - p[ d7 ] ) * xx ) * yy - ct ) * zz;

						const attribute_type &oc = table[ volumerender::to_integer( ct ) ];

						// この位置における物体が透明の場合は次のステップへ移行する
						if( !oc.has_alpha )
						{
							spos += ray_step;
							l += sampling_step;
							continue;
						}

						const_pointer p1 = p;
						const_pointer p2 = p1 + d1;
						const_pointer p3 = p1 + d2;
						const_pointer p4 = p1 + d3;
						const_pointer p5 = p1 + d4;
						const_pointer p6 = p1 + d5;
						const_pointer p7 = p1 + d6;
						const_pointer p8 = p1 + d7;

						n1.x = p4[  0  ] - p1[ -_1 ];
						n1.y = p1[ -_2 ] - p2[  0  ];
						n1.z = p5[  0  ] - p1[ -_3 ];
						n2.x = p3[  0  ] - p2[ -_1 ];
						n2.y = p1[  0  ] - p2[  _2 ];
						n2.z = p6[  0  ] - p2[ -_3 ];
						n3.x = p3[  _1 ] - p2[  0  ];
						n3.y = p4[  0  ] - p3[  _2 ];
						n3.z = p7[  0  ] - p3[ -_3 ];
						n4.x = p4[  _1 ] - p1[  0  ];
						n4.y = p4[ -_2 ] - p3[  0  ];
						n4.z = p8[  0  ] - p4[ -_3 ];
						n5.x = p8[  0  ] - p5[ -_1 ];
						n5.y = p5[ -_2 ] - p6[  0  ];
						n5.z = p5[  _3 ] - p1[  0  ];
						n6.x = p7[  0  ] - p6[ -_1 ];
						n6.y = p5[  0  ] - p6[  _2 ];
						n6.z = p6[  _3 ] - p2[  0  ];
						n7.x = p7[  _1 ] - p6[  0  ];
						n7.y = p8[  0  ] - p7[  _2 ];
						n7.z = p7[  _3 ] - p3[  0  ];
						n8.x = p8[  _1 ] - p5[  0  ];
						n8.y = p8[ -_2 ] - p7[  0  ];
						n8.z = p8[  _3 ] - p4[  0  ];

						normal.x = ( n1.x + ( n4.x - n1.x ) * xx ) + ( n2.x - n1.x + ( n1.x - n2.x + n3.x - n4.x ) * xx ) * yy;
						normal.x += ( ( n5.x + ( n8.x - n5.x ) * xx ) + ( n6.x - n5.x + ( n5.x - n6.x + n7.x - n8.x ) * xx ) * yy - normal.x ) * zz;
						normal.y = ( n1.y + ( n4.y - n1.y ) * xx ) + ( n2.y - n1.y + ( n1.y - n2.y + n3.y - n4.y ) * xx ) * yy;
						normal.y += ( ( n5.y + ( n8.y - n5.y ) * xx ) + ( n6.y - n5.y + ( n5.y - n6.y + n7.y - n8.y ) * xx ) * yy - normal.y ) * zz;
						normal.z = ( n1.z + ( n4.z - n1.z ) * xx ) + ( n2.z - n1.z + ( n1.z - n2.z + n3.z - n4.z ) * xx ) * yy;
						normal.z += ( ( n5.z + ( n8.z - n5.z ) * xx ) + ( n6.z - n5.z + ( n5.z - n6.z + n7.z - n8.z ) * xx ) * yy - normal.z ) * zz;

						normal.x /= ax;
						normal.y /= ay;
						normal.z /= az;
						double len = std::sqrt( normal.x * normal.x + normal.y * normal.y + normal.z * normal.z ) + 1.0e-10;
						normal.x /= len;
						normal.y /= len;
						normal.z /= len;

						double lAtten = 1.0;
						if( lightAtten > 0.0 )
						{
							double len = ( l + of ) * dlen;
							lAtten /= 1.0 + lightAtten * ( len * len );
						}

						double c = light.inner( normal );
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
								spec *= spec; //^2
								spec *= spec; //^4
								spec *= spec; //^8
								spec *= spec; //^16
								spec *= spec; //^32
								spec *= spec; //^64
								//spec *= spec; //^128
								spec *= specular * 255.0;
							}
						}

						c = c * diffuse_ratio + ambient_ratio;

						double alpha = oc.alpha * sampling_step;
						alpha = alpha < 1.0 ? alpha : 1.0;
						add_intensity += alpha * add_opacity * ( oc.pixel * c + spec ) * lAtten;
						add_opacity *= ( 1 - alpha );

						if( add_opacity < termination )
						{
							break;
						}
						spos.x += ray_step.x;
						spos.y += ray_step.y;
						spos.z += ray_step.z;
						l += sampling_step;
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
}


// ボリュームレンダリングのスレッド実装
namespace __volumerendering_controller__
{
	template < class Array1, class Array2, class T >
	class volumerendering_thread : public mist::thread< volumerendering_thread< Array1, Array2, T > >
	{
	public:
		typedef mist::thread< volumerendering_thread< Array1, Array2, T > > base;
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
		const volumerender::attribute_table< T > *table_;

	public:
		void setup_parameters( const Array1 &in, Array2 &out, const volumerender::parameter &p, const volumerender::attribute_table< T > &t, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			param_ = &p;
			table_ = &t;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		volumerendering_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), param_( NULL ), table_( NULL )
		{
		}
		volumerendering_thread( const volumerendering_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
																		in_( p.in_ ), out_( p.out_ ), param_( p.param_ ), table_( p.table_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			value_interpolation::volumerendering( *in_, *out_, *param_, *table_, thread_id_, thread_num_ );
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
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename Array1::size_type size_type;
	typedef __volumerendering_controller__::volumerendering_thread< Array1, Array2, ATTRIBUTETYPE > volumerendering_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	volumerendering_thread *thread = new volumerendering_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, param, table, i, thread_num );
	}

	// スレッドを実行して，終了まで待機する
	do_threads_( thread, thread_num );

	delete [] thread;
	
	return( true );
}


/// @}
// ボリュームレンダリンググループの終わり

/// @}
//  可視化グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_VOLUMERENDER__
