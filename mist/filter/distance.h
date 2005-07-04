/// @file mist/filter/distance.h
//!
//! @brief 各次元の画像に対応した，各種距離変換・逆距離変換アルゴリズム
//!
//! - 参考文献
//!   - ユークリッド距離変換
//!     - 齋藤豊文, 鳥脇純一郎, "3次元ディジタル画像に対するユークリッド距離変換," 電子情報通信学会論文誌, J76-D-II, No. 3, pp.445-453, 1993
//!     - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
//!

#ifndef __INCLUDE_MIST_DISTANCE_TRANSFORM__
#define __INCLUDE_MIST_DISTANCE_TRANSFORM__

#include <cmath>

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


// 斉藤先生によるユークリッド型距離変換の実装部分
namespace __euclidean_distance_transform__
{
	template < class T >
	void euclidean_distance_transform_x( T &in, double max_length = -1.0, typename T::size_type thread_id = 0, typename T::size_type thread_num = 1 )
	{
		typedef typename T::difference_type difference_type;
		typedef typename T::value_type value_type;

		difference_type i, j, k;
		value_type nd;

		const difference_type w = in.width( );
		const difference_type h = in.height( );
		const difference_type d = in.depth( );

		value_type max = static_cast< value_type >( max_length <= 0 ? std::sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

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
						in( i, j, k ) = nd * nd;
					}
					else
					{
						nd = 0;
					}
				}

				if( in( w - 1, j, k ) != 0 )
				{
					nd = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
					in( w - 1, j, k ) = nd * nd;
				}
				else
				{
					nd = 0;
				}

				for( i = w - 2 ; i >= 0 ; i-- )
				{
					if( in( i, j, k ) != 0 )
					{
						nd = nd + 1 < max ? nd + 1 : nd;
						in( i, j, k ) = in( i, j, k ) < nd * nd ? in( i, j, k ) : nd * nd;
					}
					else
					{
						nd = 0;
					}
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

		value_type max = static_cast< value_type >( max_length <= 0 ? std::sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

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
						irange = static_cast< size_type >( std::sqrt( wmin ) / vy ) + 1;
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

		value_type max = static_cast< value_type >( max_length <= 0 ? std::sqrt( static_cast< double >( type_limits< value_type >::maximum( ) ) ) : max_length );

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
						irange = static_cast< size_type >( std::sqrt( wmin ) / vz ) + 1;
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


// 斉藤先生によるユークリッド型距離変換のスレッド部分
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



// Calvinによるユークリッド型距離変換の実装部分
namespace __calvin__
{
	inline bool remove_edt( const double uR, const double vR, const double wR, const double ud, const double vd, const double wd )
	{
		const double a = vd - ud;
		const double b = wd - vd;
		const double c = a + b;
		return( c * vR - b * uR - a * wR - a * b * c > 0 );
	}


	template < int DIMENSION >
	struct __access__
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _1, _2, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static double aspect( const Array &in ){ return( 1.0 ); }
	};

	template < >
	struct __access__< 2 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _1, _3 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static double aspect( const Array &in ){ return( in.reso2( ) / in.reso1( ) ); }
	};

	template < >
	struct __access__< 3 >
	{
		template < class Array >
		inline static typename Array::value_type &at( Array &in, typename Array::size_type _1, typename Array::size_type _2, typename Array::size_type _3 )
		{
			return( in( _2, _3, _1 ) );
		}

		template < class Array >
		inline static typename Array::size_type size1( const Array &in ){ return( in.size3( ) ); }

		template < class Array >
		inline static typename Array::size_type size2( const Array &in ){ return( in.size1( ) ); }

		template < class Array >
		inline static typename Array::size_type size3( const Array &in ){ return( in.size2( ) ); }

		template < class Array >
		inline static double aspect( const Array &in ){ return( in.reso3( ) / in.reso1( ) ); }
	};

	/// @brief Y,Z軸方向用の距離伝播関数（1次以外の全ての次元）
	template < int DIMENSION >
	struct __distance_transform__
	{
		template < class Array >
		static void distance_transform( Array &in, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;
			typedef typename Array::difference_type difference_type;
			typedef __access__< DIMENSION > access;

			size_type i1, i2, i3;

			size_type _1 = access::size1( in );
			size_type _2 = access::size2( in );
			size_type _3 = access::size3( in );

			value_type infinity = type_limits< value_type >::maximum( );

			double as = access::aspect( in );

			double *f = new double[ _1 + 1 ];
			double *g = new double[ _1 + 1 ];
			double *h = new double[ _1 + 1 ];

			for( i3 = thread_id ; i3 < _3 ; i3 += thread_num )
			{
				for( i2 = 0 ; i2 < _2 ; i2++ )
				{
					difference_type l = 0;

					for( i1 = 0 ; i1 < _1 ; i1++ )
					{
						difference_type n = i1 + 1;
						double nd = static_cast< double >( n ) * as;

						f[ n ] = access::at( in, i1, i2, i3 );
						if( f[ n ] == infinity )
						{
							continue;
						}

						if( l < 2 )
						{
							l++;
							g[ l ] = f[ n ];
							h[ l ] = nd;
						}
						else
						{
							while( l >= 2 && remove_edt( g[ l - 1 ], g[ l ], f[ n ], h[ l - 1 ], h[ l ], nd ) )
							{
								l--;
							}
							l++;
							g[ l ] = f[ n ];
							h[ l ] = nd;
						}
					}

					if( l == 0 )
					{
						continue;
					}

					difference_type ns = l;
					l = 1;

					for( i1 = 0 ; i1 < _1 ; i1++ )
					{
						double n = i1 + 1;
						double nd = n * as;
						double len = h[ l ] - nd;
						
						len = g[ l ] + len * len;
						for( ; l < ns ; l++ )
						{
							double len_ = h[ l + 1 ] - nd;
							len_ = g[ l + 1 ] + len_ * len_;
							if( len > len_ )
							{
								len = len_;
							}
							else
							{
								break;
							}
						}
						access::at( in, i1, i2, i3 ) = static_cast< value_type >( len );
					}
				}
			}

			delete [] f;
			delete [] g;
			delete [] h;
		}
	};


	/// @brief X軸方向専用の距離伝播関数
	template < >
	struct __distance_transform__< 1 >
	{
		template < class Array >
			static void distance_transform( Array &in, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
		{
			typedef typename Array::size_type  size_type;
			typedef typename Array::value_type value_type;
			typedef typename Array::difference_type difference_type;

			difference_type i, j, k;
			value_type len;

			const difference_type w = in.width( );
			const difference_type h = in.height( );
			const difference_type d = in.depth( );

			const value_type max = type_limits< value_type >::maximum( );

			for( k = 0 ; k < d ; k++ )
			{
				for( j = thread_id ; j < h ; j += thread_num )
				{
					if( in( 0, j, k ) != 0 )
					{
						len = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
						in( 0, j, k ) = len * len;
					}
					else
					{
						len = 0;
					}

					for( i = 1 ; i < w ; i++ )
					{
						if( in( i, j, k ) != 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							in( i, j, k ) = len * len;
						}
						else
						{
							len = 0;
						}
					}

					if( in( w - 1, j, k ) != 0 )
					{
						len = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;
						in( w - 1, j, k ) = len * len;
					}
					else
					{
						len = 0;
					}

					for( i = w - 2 ; i >= 0 ; i-- )
					{
						if( in( i, j, k ) != 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							in( i, j, k ) = in( i, j, k ) < len * len ? in( i, j, k ) : len * len;
						}
						else
						{
							len = 0;
						}
					}
				}
			}
		}
	};


	template < class T >
	class calvin_distance_transform_thread : public mist::thread< calvin_distance_transform_thread< T > >
	{
	public:
		typedef mist::thread< calvin_distance_transform_thread< T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		T *in_;
		size_type axis_;

	public:
		void setup_parameters( T &in, size_type axis, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			axis_ = axis;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		void setup_axis( size_type axis )
		{
			axis_ = axis;
		}

		const calvin_distance_transform_thread& operator =( const calvin_distance_transform_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				axis_ = p.axis_;
			}
			return( *this );
		}

		calvin_distance_transform_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), in_( NULL ), axis_( 0 )
		{
		}
		calvin_distance_transform_thread( const calvin_distance_transform_thread &p )
			: base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ), in_( NULL ), axis_( p.axis_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			switch( axis_ )
			{
			case 0:
				__distance_transform__< 1 >::distance_transform( *in_, thread_id_, thread_num_ );
				break;

			case 1:
				__distance_transform__< 2 >::distance_transform( *in_, thread_id_, thread_num_ );
				break;

			case 2:
			default:
				__distance_transform__< 3 >::distance_transform( *in_, thread_id_, thread_num_ );
				break;
			}
			return( true );
		}
	};
}





//! @addtogroup distance_group 距離変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{


//! @addtogroup euclidean_distance_group ユークリッド距離変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{



/// @brief 斉藤先生によるユークリッド距離変換
namespace saito
{
	/// @brief ユークリッド距離変換
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  max_length … 伝播させる距離の最大値
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void distance_transform( const Array1 &in, Array2 &out, double max_length = -1.0, typename Array1::size_type thread_num = 0 )
	{
		typedef typename Array2::size_type  size_type;
		typedef typename Array2::value_type value_type;
		typedef __distance_transform_controller__::euclidean_distance_transform_thread< Array2 > euclidean_distance_transform_thread;

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
			out[ i ] = static_cast< value_type >( in[ i ] != 0 ? 1 : 0 );
		}

		euclidean_distance_transform_thread *thread = new euclidean_distance_transform_thread[ thread_num ];

		if( in.width( ) > 1 )
		{
			// X軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, max_length, 0, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.height( ) > 1 )
		{
			// Y軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, max_length, 1, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.depth( ) > 1 )
		{
			// Y軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, max_length, 2, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		delete [] thread;
	}
}


/// @brief Calvinによるユークリッド距離変換
namespace calvin
{
	/// @brief ユークリッド距離変換
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! - 参考文献
	//!   - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void distance_transform( const Array1 &in, Array2 &out, typename Array1::size_type thread_num = 0 )
	{
		typedef typename Array2::size_type  size_type;
		typedef typename Array2::value_type value_type;
		typedef __calvin__::calvin_distance_transform_thread< Array2 > calvin_distance_transform_thread;

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
			out[ i ] = static_cast< value_type >( in[ i ] != 0 ? 1 : 0 );
		}

		calvin_distance_transform_thread *thread = new calvin_distance_transform_thread[ thread_num ];

		if( in.width( ) > 1 )
		{
			// X軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, 0, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.height( ) > 1 )
		{
			// Y軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, 1, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.depth( ) > 1 )
		{
			// Y軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, 2, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		delete [] thread;
	}
}


/// @brief ユークリッド距離変換
namespace euclidean
{
	/// @brief ユークリッド距離変換
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void distance_transform( const Array1 &in, Array2 &out, typename Array1::size_type thread_num = 0 )
	{
		calvin::distance_transform( in, out, thread_num );
	}
}

/// @}
//  ユークリッド距離変換グループの終わり



//! @addtogroup skeleton_group スケルトン抽出
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{

/// @brief ユークリッド2乗距離画像を入力とするスケルトン抽出
//! 
//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
//! 
//! @param[in]  in         … ユークリッド2乗距離画像
//! @param[out] out        … スケルトン点のみを残したユークリッド2乗距離画像
//! 
template < class Array1, class Array2 >
void skeleton( const Array1 &in, Array2 &out )
{
	typedef typename Array1::size_type        size_type;
	typedef typename Array1::difference_type  difference_type;
	typedef typename Array2::value_type       value_type;

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	// 作業用配列
	array3< difference_type > temp( in.width( ), in.height( ), in.depth( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		temp[ i ] = i;
	}

	difference_type w = in.width( );
	difference_type h = in.height( );
	difference_type d = in.depth( );
	for( difference_type k = 0 ; k < d ; k++ )
	{
		for( difference_type j = 0 ; j < h ; j++ )
		{
			for( difference_type i = 0 ; i < w ; i++ )
			{
				difference_type index = &in( i, j, k ) - &in[ 0 ];
				difference_type rr = in[ index ];

				if( rr == 0 )
				{
					continue;
				}

				difference_type rx = static_cast< difference_type >( std::ceil( std::sqrt( static_cast< double >( rr ) ) ) );
				difference_type ry = rx;
				difference_type rz = rx;

				for( difference_type z = -rz ; z <= rz ; z++ )
				{
					difference_type pz = k + z;
					if( pz < 0 || d <= pz )
					{
						continue;
					}
					difference_type zz = z * z;

					for( difference_type y = -ry ; y <= ry ; y++ )
					{
						difference_type py = j + y;
						if( py < 0 || h <= py )
						{
							continue;
						}
						difference_type yy = y * y;

						for( difference_type x = -rx ; x <= rx ; x++ )
						{
							difference_type px = i + x;
							if( px < 0 || w <= px )
							{
								continue;
							}
							difference_type xx = x * x;

							if( xx + yy + zz < rr )
							{
								difference_type &p = temp( px, py, pz );
								if( in[ p ] < rr )
								{
									p = index;
								}
							}
						}
					}
				}
			}
		}
	}

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		if( temp[ i ] > 0 )
		{
			temp[ temp[ i ] ] = -2;
			temp[ i ] = -1;
		}
	}

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		if( temp[ i ] != -2 )
		{
			out[ i ] = 0;
		}
		else
		{
			out[ i ] = static_cast< value_type >( in[ i ] );
		}
	}
}

/// @}
//  スケルトン抽出グループの終わり


/// @}
//  距離変換グループの終わり




// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_DISTANCE_TRANSFORM__
