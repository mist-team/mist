#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#define __INCLUDE_MIST_DISTANCE_TRANSFORM__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif



// mist名前空間の始まり
_MIST_BEGIN


// ユークリッド型距離変換
namespace __euclidean_distance_transform__
{
	template < class T >
	void euclidean_distance_transform_x( T &in, double max_length = -1.0, typename T::size_type thread_id = 0, typename T::size_type thread_num = 1 )
	{
		typedef typename T::size_type  size_type;
		typedef typename T::value_type value_type;

		size_type i, j, k;
		value_type nd, n;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		value_type max = static_cast< value_type >( max_length <= 0 ? sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

		for( k = 0 ; k < d ; k++ )
		{
			for( j = thread_id ; j < h ; j += thread_num )
			{
				if( in( 0, j, k ) != 0 )
				{
					nd = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
					in( 0, j, k ) = nd * nd;
				}
				else
				{
					nd = 0;
				}

				for( i = 1 ; i < w ; i++ )
				{
					if( in( i, j, k ) != 0 )
					{
						nd = nd + 1 < max ? nd + 1 : nd;
					}
					else
					{
						nd = 0;
					}

					in( i, j, k ) = nd * nd;
				}

				if( in( w - 1, j, k ) != 0 )
				{
					nd = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
				}
				else
				{
					nd = 0;
				}

				for( i = w - 1 ; i > 0 ; i-- )
				{
					if( in( i - 1, j, k ) != 0 )
					{
						nd = nd + 1 < max ? nd + 1 : nd;
					}
					else
					{
						nd = 0;
					}

					n = nd * nd;
					in( i - 1, j, k ) = in( i - 1, j, k ) < n ? in( i - 1, j, k ) : n;
				}
			}
		}
	}

	template < class T >
	void euclidean_distance_transform_y( T &in, double max_length = -1.0, typename T::size_type thread_id = 0, typename T::size_type thread_num = 1 )
	{
		typedef typename T::size_type  size_type;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;

		size_type i, j, k;
		difference_type n;

		double wmin, wtmp;
		size_type irange;
		difference_type irange1, irange2;
		double *work;
		double vy, vyvy;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		value_type max = static_cast< value_type >( max_length <= 0 ? sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

		vy = in.reso2( ) / in.reso1( );
		vyvy = vy * vy;
		work = new double[ h ];

		for( k = 0 ; k < d ; k++ )
		{
			for( i = thread_id ; i < w ; i += thread_num )
			{
				for( j = 0 ; j < h ; j++ )
				{
					work[ j ] = static_cast< double >( in( i, j, k ) );
				}

				for( j = 0 ; j < h ; j++ )
				{
					wmin = work[ j ];

					if( wmin != 0.0 )
					{
						irange = static_cast< size_type >( sqrt( wmin ) / vy ) + 1;
						irange = static_cast< size_type >( static_cast< value_type >( irange ) < max ? irange : max );

						irange1 = j < irange  ? j : irange;
						irange2 = j + irange >= h ? h - 1 - j : irange;
						for( n = -irange1 ; n <= irange2 ; n++ )
						{
							wtmp = work[ j + n ] + static_cast< double >( n * n * vyvy );
							wmin = wmin > wtmp ? wtmp : wmin;
						}

						in( i, j, k ) = static_cast< value_type >( wmin );
					}
				}
			}
		}

		delete [] work;
	}


	template < class T >
	void euclidean_distance_transform_z( T &in, double max_length = -1.0, typename T::size_type thread_id = 0, typename T::size_type thread_num = 1 )
	{
		typedef typename T::size_type  size_type;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;

		size_type i, j, k;
		difference_type n;
		double wmin, wtmp;
		size_type irange;
		difference_type irange1, irange2;
		double *work;
		double vz, vzvz;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		vz = in.reso3( ) / in.reso1( );
		vzvz = vz * vz;
		work = new double[ d ];

		value_type max = static_cast< value_type >( max_length <= 0 ? sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

		for( j = 0 ; j < h ; j++ )
		{
			for( i = thread_id ; i < w ; i += thread_num )
			{
				for( k = 0 ; k < d ; k++ )
				{
					work[ k ] = static_cast< double >( in( i, j, k ) );
				}

				for( k = 0 ; k < d ; k++ )
				{
					wmin = work[ k ];

					if( wmin != 0.0 )
					{
						irange = static_cast< size_type >( sqrt( wmin ) / vz ) + 1;
						irange = static_cast< size_type >( static_cast< value_type >( irange ) < max ? irange : max );

						irange1 = k < irange ? k : irange;
						irange2 = k + irange >= d ? d - 1 - k : irange;
						for( n = -irange1 ; n <= irange2 ; n++ )
						{
							wtmp = work[ k + n ] + static_cast< double >( n * n * vzvz );
							wmin = wmin > wtmp ? wtmp : wmin;
						}

						in( i, j, k ) = static_cast< value_type >( wmin );
					}
				}
			}
		}

		delete [] work;
	}
}


// ユークリッド型距離変換のスレッド部分
namespace __distance_transform_controller__
{
	template < class T >
	class euclidean_distance_transform_thread : public mist::thread< euclidean_distance_transform_thread< T > >
	{
	public:
		typedef mist::thread< euclidean_distance_transform_thread< T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		T *in_;
		double max_length_;
		size_type axis_;

	public:
		void setup_parameters( T &in, double max_length, size_type axis, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			max_length_ = max_length;
			axis_ = axis;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		void setup_axis( size_type axis )
		{
			axis_ = axis;
		}

		const euclidean_distance_transform_thread& operator =( const euclidean_distance_transform_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				max_length_ = p.max_length;
				axis_ = p.axis_;
			}
			return( *this );
		}

		euclidean_distance_transform_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), in_( NULL ), max_length_( -1.0 ), axis_( 0 )
		{
		}
		euclidean_distance_transform_thread( const euclidean_distance_transform_thread &p )
			: base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ), in_( NULL ), max_length_( -1.0 ), axis_( p.axis_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			switch( axis_ )
			{
			case 1:
				__euclidean_distance_transform__::euclidean_distance_transform_y( *in_, max_length_, thread_id_, thread_num_ );
				break;

			case 2:
				__euclidean_distance_transform__::euclidean_distance_transform_z( *in_, max_length_, thread_id_, thread_num_ );
				break;

			case 0:
			default:
				__euclidean_distance_transform__::euclidean_distance_transform_x( *in_, max_length_, thread_id_, thread_num_ );
				break;
			}
			return( true );
		}
	};
}


template < class T1, class T2, class Allocator1, class Allocator2 >
void euclidean_distance_transform( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, double max_length = -1.0, typename array< T1, Allocator1 >::size_type thread_num = 0 )
{
	typedef typename array< T2, Allocator2 >::size_type  size_type;
	typedef typename array< T2, Allocator2 >::value_type value_type;
	typedef __distance_transform_controller__::euclidean_distance_transform_thread< array< T2, Allocator2 > > euclidean_distance_transform_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ) );

	size_type i;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] != 0 ? 1 : 0;
	}

	euclidean_distance_transform_thread *thread = new euclidean_distance_transform_thread[ thread_num ];

	{
		// X軸方向の処理
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, max_length, 0, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}
	}

	delete [] thread;
}

template < class T1, class T2, class Allocator1, class Allocator2 >
void euclidean_distance_transform( const array1< T1, Allocator1 > &in, array1< T2, Allocator2 > &out, double max_length = -1.0, typename array1< T1, Allocator1 >::size_type thread_num = 0 )
{
	typedef typename array1< T2, Allocator2 >::size_type  size_type;
	typedef typename array1< T2, Allocator2 >::value_type value_type;
	typedef __distance_transform_controller__::euclidean_distance_transform_thread< array1< T2, Allocator2 > > euclidean_distance_transform_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ) );
	out.reso1( in.reso1( ) );

	size_type i;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] != 0 ? 1 : 0;
	}

	euclidean_distance_transform_thread *thread = new euclidean_distance_transform_thread[ thread_num ];

	{
		// X軸方向の処理
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, max_length, 0, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}
	}

	delete [] thread;
}

template < class T1, class T2, class Allocator1, class Allocator2 >
void euclidean_distance_transform( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, double max_length = -1.0, typename array2< T1, Allocator1 >::size_type thread_num = 0 )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;
	typedef __distance_transform_controller__::euclidean_distance_transform_thread< array2< T2, Allocator2 > > euclidean_distance_transform_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	size_type i;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] != 0 ? 1 : 0;
	}

	euclidean_distance_transform_thread *thread = new euclidean_distance_transform_thread[ thread_num ];

	{
		// X軸方向の処理
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, max_length, 0, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}
	}
	{
		// Y軸方向の処理
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, max_length, 1, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}
	}

	delete [] thread;
}

template < class T1, class T2, class Allocator1, class Allocator2 >
void euclidean_distance_transform( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, double max_length = -1.0, typename array3< T1, Allocator1 >::size_type thread_num = 0 )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;
	typedef __distance_transform_controller__::euclidean_distance_transform_thread< array3< T2, Allocator2 > > euclidean_distance_transform_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	size_type i;

	for( i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] != 0 ? 1 : 0;
	}

	euclidean_distance_transform_thread *thread = new euclidean_distance_transform_thread[ thread_num ];

	{
		// X軸方向の処理
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, max_length, 0, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}
	}
	{
		// Y軸方向の処理
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, max_length, 1, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}
	}
	{
		// Y軸方向の処理
		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].setup_parameters( out, max_length, 2, i, thread_num );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].create( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].wait( );
		}

		for( i = 0 ; i < thread_num ; i++ )
		{
			thread[ i ].close( );
		}
	}

	delete [] thread;
}



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DISTANCE_TRANSFORM__
