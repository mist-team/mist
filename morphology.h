#ifndef __INCLUDE_MIST_MORPHOLOGY__
#define __INCLUDE_MIST_MORPHOLOGY__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#include <vector>


// mist名前空間の始まり
_MIST_BEGIN


namespace __morphology__
{
	struct point
	{
		typedef ptrdiff_t value_type;
		value_type x;
		value_type y;
		value_type z;
		size_t life;

		point( value_type xx, value_type yy, value_type zz, size_t l ) : x( xx ), y( yy ), z( zz ), life( l )
		{
		}
	};

	struct pointer_diff
	{
		ptrdiff_t diff;
		size_t    life;

		pointer_diff( ptrdiff_t d, size_t l ) : diff( d ), life( l )
		{
		}
	};

	struct morphology_structure
	{
		typedef std::vector< point > list_type;
		list_type object;
		list_type update;
		size_t margin_x;
		size_t margin_y;
		size_t margin_z;
	};

	inline morphology_structure circle( double radius, double resoX, double resoY )
	{
		typedef array2< unsigned short >::size_type size_type;
		typedef array2< unsigned short >::difference_type difference_type;
		double min_reso = resoX < resoY ? resoX: resoY;

		double ax = resoX / min_reso;
		double ay = resoY / min_reso;
		double xx, yy, rr = radius * radius;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type x, y;

		size_type ox = rx + 1;
		size_type oy = ry + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;

		morphology_structure s;
		array2< bool > m( w, h );

		// 円の構造要素を作成する
		for( y = -ry ; y <= ry ; y++ )
		{
			yy = y * y * ay * ay;
			for( x = -rx ; x <= rx ; x++ )
			{
				xx = x * x * ax * ax;
				if( xx + yy <= rr )
				{
					m( x + ox, y + oy ) = true;
				}
			}
		}

		// 円の構造要素の各点に生存期間を設定する
		for( y = -ry ; y <= ry ; y++ )
		{
			size_t life = 0;
			for( x = -rx ; x <= rx ; x++ )
			{
				if( m( x + ox, y + oy ) )
				{
					s.object.push_back( point( x, y, 0, ++life ) );
				}
				else
				{
					life = 0;
				}
				if( m( x + ox, y + oy ) && !m( x + ox + 1, y + oy ) )
				{
					s.update.push_back( point( x, y, 0, life ) );
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = 0;

		return( s );
	}

	inline morphology_structure sphere( double radius, double resoX, double resoY, double resoZ )
	{
		typedef array3< unsigned short >::size_type size_type;
		typedef array3< unsigned short >::difference_type difference_type;
		double min_reso = resoX < resoY ? resoX: resoY;
		min_reso = min_reso < resoZ ? min_reso : resoZ;

		double ax = resoX / min_reso;
		double ay = resoY / min_reso;
		double az = resoZ / min_reso;
		double xx, yy, zz, rr = radius * radius;
		difference_type rx = static_cast< size_type >( ceil( radius / ax ) );
		difference_type ry = static_cast< size_type >( ceil( radius / ay ) );
		difference_type rz = static_cast< size_type >( ceil( radius / az ) );
		difference_type x, y, z;

		size_type ox = rx + 1;
		size_type oy = ry + 1;
		size_type oz = rz + 1;

		size_type w = 2 * ox + 1;
		size_type h = 2 * oy + 1;
		size_type d = 2 * oz + 1;

		morphology_structure s;
		array3< bool > m( w, h, d );

		// 球の構造要素を作成する
		for( z = -rz ; z <= rz ; z++ )
		{
			zz = z * z * az * az;
			for( y = -ry ; y <= ry ; y++ )
			{
				yy = y * y * ay * ay;
				for( x = -rx ; x <= rx ; x++ )
				{
					xx = x * x * ax * ax;
					if( xx + yy + zz <= rr )
					{
						m( x + ox, y + oy, z + oz ) = true;
					}
				}
			}
		}

		// 球の構造要素の各点に生存期間を設定する
		for( z = -rz ; z <= rz ; z++ )
		{
			for( y = -ry ; y <= ry ; y++ )
			{
				size_t life = 0;
				for( x = -rx ; x <= rx ; x++ )
				{
					if( m( x + ox, y + oy, z + oz ) )
					{
						s.object.push_back( point( x, y, z, ++life ) );
					}
					else
					{
						life = 0;
					}
					if( m( x + ox, y + oy, z + oz ) && !m( x + ox + 1, y + oy, z + oz ) )
					{
						s.update.push_back( point( x, y, z, life ) );
					}
				}
			}
		}

		s.margin_x = rx;
		s.margin_y = ry;
		s.margin_z = rz;

		return( s );
	}

	template < class Array >
	std::vector< pointer_diff > create_pointer_diff_list( const Array &in, const std::vector< point > &list )
	{
		typedef typename Array::size_type     size_type;
		typedef typename Array::const_pointer const_pointer;
		size_type cx = in.width( ) / 2;
		size_type cy = in.height( ) / 2;
		size_type cz = in.depth( ) / 2;
		const_pointer p = &( in( cx, cy, cz ) );

		std::vector< pointer_diff > out;

		for( size_type i = 0 ; i < list.size( ) ; i++ )
		{
			const point &pt = list[ i ];
			const_pointer pp = &( in( cx + pt.x, cy + pt.y, cz + pt.z ) );
			out.push_back( pointer_diff( pp - p, pt.life ) );
		}

		return( out );
	}
}

namespace __erosion__
{
	template < class Array1, class Array2 >
	void erosion( const Array1 &in, Array2 &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type       size_type;
		typedef typename Array1::value_type      value_type;
		typedef typename Array1::const_pointer   const_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type      out_value_type;
		typedef __morphology__::pointer_diff     pointer_diff;
		typedef std::vector< pointer_diff >      list_type;

		list_type::const_iterator ite;

		size_type  i, j, k;
		value_type min;
		size_type life;
		const_pointer p;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				p = &( in( 0, j, k ) );
				ite = object.begin( );
				min = p[ ite->diff ];
				life = ite->life;
				++ite;
				for( ; ite != object.end( ) ; ++ite )
				{
					if( min > p[ ite->diff ] || ( min == p[ ite->diff ] && life < ite->life ) )
					{
						min = p[ ite->diff ];
						life = ite->life;
					}
				}

				out( 0, j, k ) = static_cast< out_value_type >( min );
				life--;

				for( i = 1 ; i < w ; i++ )
				{
					p = &( in( i, j, k ) );

					if( life == 0 )
					{
						// 前回までの最小値が構造要素から出てしまったので，新たに前探索を行う
						ite = object.begin( );
						min = p[ ite->diff ];
						life = ite->life;
						++ite;
						for( ; ite != object.end( ) ; ++ite )
						{
							if( min > p[ ite->diff ] || ( min == p[ ite->diff ] && life < ite->life ) )
							{
								min = p[ ite->diff ];
								life = ite->life;
							}
						}
					}
					else
					{
						// 前回までの最小値が構造要素内に存在するので，更新分のみから探索を行う
						for( ite = update.begin( ) ; ite != update.end( ) ; ++ite )
						{
							if( min > p[ ite->diff ] || ( min == p[ ite->diff ] && life < ite->life ) )
							{
								min = p[ ite->diff ];
								life = ite->life;
							}
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( min );
					life--;
				}
			}
		}
	}
}


namespace __dilation__
{
	template < class Array1, class Array2 >
	void dilation( const Array1 &in, Array2 &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename Array1::size_type thread_idy, typename Array1::size_type thread_numy,
						typename Array1::size_type thread_idz, typename Array1::size_type thread_numz )
	{
		typedef typename Array1::size_type       size_type;
		typedef typename Array1::value_type      value_type;
		typedef typename Array1::const_pointer   const_pointer;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array2::value_type      out_value_type;
		typedef __morphology__::pointer_diff     pointer_diff;
		typedef std::vector< pointer_diff >      list_type;

		list_type::const_iterator ite;

		size_type  i, j, k;
		value_type max;
		size_type life;
		const_pointer p;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		for( k = thread_idz ; k < d ; k += thread_numz )
		{
			for( j = thread_idy ; j < h ; j += thread_numy )
			{
				p = &( in( 0, j, k ) );
				ite = object.begin( );
				max = p[ ite->diff ];
				life = ite->life;
				++ite;
				for( ; ite != object.end( ) ; ++ite )
				{
					if( max < p[ ite->diff ] || ( max == p[ ite->diff ] && life < ite->life ) )
					{
						max = p[ ite->diff ];
						life = ite->life;
					}
				}

				out( 0, j, k ) = static_cast< out_value_type >( max );
				life--;

				for( i = 1 ; i < w ; i++ )
				{
					p = &( in( i, j, k ) );

					if( life == 0 )
					{
						// 前回までの最小値が構造要素から出てしまったので，新たに前探索を行う
						ite = object.begin( );
						max = p[ ite->diff ];
						life = ite->life;
						++ite;
						for( ; ite != object.end( ) ; ++ite )
						{
							if( max < p[ ite->diff ] || ( max == p[ ite->diff ] && life < ite->life ) )
							{
								max = p[ ite->diff ];
								life = ite->life;
							}
						}
					}
					else
					{
						// 前回までの最小値が構造要素内に存在するので，更新分のみから探索を行う
						for( ite = update.begin( ) ; ite != update.end( ) ; ++ite )
						{
							if( max < p[ ite->diff ] || ( max == p[ ite->diff ] && life < ite->life ) )
							{
								max = p[ ite->diff ];
								life = ite->life;
							}
						}
					}

					out( i, j, k ) = static_cast< out_value_type >( max );
					life--;
				}
			}
		}
	}
}


// モルフォロジ演算のスレッド実装
namespace __morphology_controller__
{
	// Erosion 演算
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void erosion( const marray< array< T1, Allocator1 > > &in, array< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__erosion__::erosion( in, out, object, update, 0, 1, thread_id, thread_num );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void erosion( const marray< array1< T1, Allocator1 > > &in, array1< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array1< T1, Allocator1 >::size_type thread_id, typename array1< T1, Allocator1 >::size_type thread_num )
	{
		__erosion__::erosion( in, out, object, update, 0, 1, thread_id, thread_num );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void erosion( const marray< array2< T1, Allocator1 > > &in, array2< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__erosion__::erosion( in, out, object, update, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void erosion( const marray< array3< T1, Allocator1 > > &in, array3< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__erosion__::erosion( in, out, object, update, 0, 1, thread_id, thread_num );
	}


	// Dilation 演算
	template < class T1, class Allocator1, class T2, class Allocator2 >
	void dilation( const marray< array< T1, Allocator1 > > &in, array< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array< T1, Allocator1 >::size_type thread_id, typename array< T1, Allocator1 >::size_type thread_num )
	{
		__dilation__::dilation( in, out, object, update, 0, 1, thread_id, thread_num );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void dilation( const marray< array1< T1, Allocator1 > > &in, array1< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array1< T1, Allocator1 >::size_type thread_id, typename array1< T1, Allocator1 >::size_type thread_num )
	{
		__dilation__::dilation( in, out, object, update, 0, 1, thread_id, thread_num );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void dilation( const marray< array2< T1, Allocator1 > > &in, array2< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array2< T1, Allocator1 >::size_type thread_id, typename array2< T1, Allocator1 >::size_type thread_num )
	{
		__dilation__::dilation( in, out, object, update, thread_id, thread_num, 0, 1 );
	}

	template < class T1, class Allocator1, class T2, class Allocator2 >
	void dilation( const marray< array3< T1, Allocator1 > > &in, array3< T2, Allocator2 > &out,
						const std::vector< __morphology__::pointer_diff > &object, const std::vector< __morphology__::pointer_diff > &update,
						typename array3< T1, Allocator1 >::size_type thread_id, typename array3< T1, Allocator1 >::size_type thread_num )
	{
		__dilation__::dilation( in, out, object, update, 0, 1, thread_id, thread_num );
	}


	template < class T1, class T2 >
	class morphology_thread : public mist::thread_object< morphology_thread< T1, T2 > >
	{
	public:
		typedef mist::thread_object< morphology_thread< T1, T2 > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;
		typedef __morphology__::pointer_diff pointer_diff;
		typedef std::vector< pointer_diff >  list_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const T1 *in_;
		T2 *out_;
		list_type *object_;
		list_type *update_;
		bool is_erosion_;

	public:
		void setup_parameters( const T1 &in, T2 &out, list_type &object, list_type &update, bool is_erosion, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			object_ = &object;
			update_ = &update;
			is_erosion_ = is_erosion;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		const morphology_thread& operator =( const morphology_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				out_ = p.out_;
				is_erosion_ = p.is_erosion_;
				object_ = p.object_;
				update_ = p.update_;
			}
			return( *this );
		}

		morphology_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), object_( NULL ), update_( NULL ), is_erosion_( true )
		{
		}
		morphology_thread( const morphology_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), object_( p.object_ ), update_( p.update_ ), is_erosion_( p.is_erosion_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( const morphology_thread &p )
		{
			if( is_erosion_ )
			{
				erosion( *in_, *out_, *object_, *update_, thread_id_, thread_num_ );
			}
			else
			{
				dilation( *in_, *out_, *object_, *update_, thread_id_, thread_num_ );
			}
			return( true );
		}
	};
}


// 2次元モルフォロジ演算はここから
template < class T, class Allocator >
void erosion( array2< T, Allocator > &in, double radius, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	typedef array2< T, Allocator >::value_type value_type;
	typedef array2< T, Allocator >::size_type  size_type;
	typedef __morphology_controller__::morphology_thread< marray< array2< T, Allocator > >, array2< T, Allocator > > morphology_thread;
	typedef __morphology__::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	__morphology__::morphology_structure s = __morphology__::circle( radius, in.reso1( ), in.reso2( ) );

	value_type max = type_limits< value_type >::maximum( );

	marray< array2< T, Allocator > > out( in, s.margin_x, s.margin_y, max );

	list_type object = __morphology__::create_pointer_diff_list( out, s.object );
	list_type update = __morphology__::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update, true, i, thread_num );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].create_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].wait_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].close_thread( );
	}

	delete [] thread;
}


template < class T, class Allocator >
void dilation( array2< T, Allocator > &in, double radius, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	typedef array2< T, Allocator >::value_type value_type;
	typedef array2< T, Allocator >::size_type  size_type;
	typedef __morphology_controller__::morphology_thread< marray< array2< T, Allocator > >, array2< T, Allocator > > morphology_thread;
	typedef __morphology__::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	__morphology__::morphology_structure s = __morphology__::circle( radius, in.reso1( ), in.reso2( ) );

	value_type min = type_limits< value_type >::minimum( );

	marray< array2< T, Allocator > > out( in, s.margin_x, s.margin_y, min );

	list_type object = __morphology__::create_pointer_diff_list( out, s.object );
	list_type update = __morphology__::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( out, in, object, update, false, i, thread_num );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].create_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].wait_thread( );
	}

	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].close_thread( );
	}

	delete [] thread;
}


template < class T, class Allocator >
void opening( array2< T, Allocator > &in, double radius, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	typedef array2< T, Allocator >::value_type value_type;
	typedef array2< T, Allocator >::size_type  size_type;
	typedef __morphology_controller__::morphology_thread< marray< array2< T, Allocator > >, array2< T, Allocator > > morphology_thread;
	typedef __morphology__::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	__morphology__::morphology_structure s = __morphology__::circle( radius, in.reso1( ), in.reso2( ) );

	value_type max = type_limits< value_type >::maximum( );
	value_type min = type_limits< value_type >::minimum( );

	marray< array2< T, Allocator > > out( in, s.margin_x, s.margin_y, max );

	list_type object = __morphology__::create_pointer_diff_list( out, s.object );
	list_type update = __morphology__::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	size_type i;
	{
		// Erosion 演算
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, true, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close_thread( );
		}
	}

	out = in;
	out.fill_margin( min );

	{
		// Dilation 演算
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, false, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close_thread( );
		}
	}

	delete [] thread;
}


template < class T, class Allocator >
void closing( array2< T, Allocator > &in, double radius, typename array2< T, Allocator >::size_type thread_num = 0 )
{
	typedef array2< T, Allocator >::value_type value_type;
	typedef array2< T, Allocator >::size_type  size_type;
	typedef __morphology_controller__::morphology_thread< marray< array2< T, Allocator > >, array2< T, Allocator > > morphology_thread;
	typedef __morphology__::pointer_diff pointer_diff;
	typedef std::vector< pointer_diff >  list_type;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	__morphology__::morphology_structure s = __morphology__::circle( radius, in.reso1( ), in.reso2( ) );

	value_type max = type_limits< value_type >::maximum( );
	value_type min = type_limits< value_type >::minimum( );

	marray< array2< T, Allocator > > out( in, s.margin_x, s.margin_y, min );

	list_type object = __morphology__::create_pointer_diff_list( out, s.object );
	list_type update = __morphology__::create_pointer_diff_list( out, s.update );

	morphology_thread *thread = new morphology_thread[ thread_num ];

	size_type i;
	{
		// Dilation 演算
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, false, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close_thread( );
		}
	}

	out = in;
	out.fill_margin( max );

	{
		// Erosion 演算
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, in, object, update, true, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait_thread( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close_thread( );
		}
	}

	delete [] thread;
}
// 2次元モルフォロジ演算はここまで


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MORPHOLOGY__
