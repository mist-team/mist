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

/// @file mist/filter/distance.h
//!
//! @brief 各次元の画像に対応した，各種距離変換・逆距離変換アルゴリズム
//!
//! - 参考文献
//!   - ユークリッド距離変換
//!     - 齋藤豊文, 鳥脇純一郎, "3次元ユークリッド距離変換および拡張ボロノイ分割のアルゴリズムと肝組織標本画像の解析," 画像電子学会誌, 第21巻，第5号, pp.468-474, 1992
//!     - 齋藤豊文, 鳥脇純一郎, "3次元ディジタル画像に対するユークリッド距離変換," 電子情報通信学会論文誌, J76-D-II, No. 3, pp.445-453, 1993
//!     - 斎藤豊文, 鳥脇純一郎, ``ディジタル画像におけるユークリッド逆距離変換とスケルトンについて,'' 電子情報通信学会パターン認識・理解研究会資料, PRU93-50, No.228, pp.57-64, 1993
//!     - 鳥脇純一郎, "3次元ディジタル画像処理," 昭晃堂, 2002
//!     - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
//!     - A. Meijster, J. Roerdink, and W. Hesselink, "A general algorithm for computing distance transforms in linear time," In: Mathematical Morphology and its Applications to Image and Signal Processing, J. Goutsias, L. Vincent, and D.S. Bloomberg (eds.), Kluwer, 2000, pp. 331-340
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


namespace _distance_utility_
{
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

	template < int DIMENSION >
	struct __range__
	{
		typedef size_t size_type;

		size_type sx;
		size_type ex;
		size_type sy;
		size_type ey;
		size_type sz;
		size_type ez;

		__range__(  ) : sx( 0 ), ex( 0 ), sy( 0 ), ey( 0 ), sz( 0 ), ez( 0 )
		{
		}

		__range__( size_type ssx, size_type eex, size_type ssy, size_type eey, size_type ssz, size_type eez )
					: sx( ssx ), ex( eex ), sy( ssy ), ey( eey ), sz( ssz ), ez( eez )
		{
		}

		template < int DIM >
		__range__( const __range__< DIM > &r )
					: sx( r.sx ), ex( r.ex ), sy( r.sy ), ey( r.ey ), sz( r.sz ), ez( r.ez )
		{
		}

		size_type begin1( ) const { return( sx ); }
		size_type end1( ) const { return( ex ); }

		size_type begin2( ) const { return( sy ); }
		size_type end2( ) const { return( ey ); }

		size_type begin3( ) const { return( sz ); }
		size_type end3( ) const { return( ez ); }
	};

	template <  >
	struct __range__< 2 >
	{
		typedef size_t size_type;

		size_type sx;
		size_type ex;
		size_type sy;
		size_type ey;
		size_type sz;
		size_type ez;

		__range__(  ) : sx( 0 ), ex( 0 ), sy( 0 ), ey( 0 ), sz( 0 ), ez( 0 )
		{
		}

		__range__( size_type ssx, size_type eex, size_type ssy, size_type eey, size_type ssz, size_type eez )
					: sx( ssx ), ex( eex ), sy( ssy ), ey( eey ), sz( ssz ), ez( eez )
		{
		}

		template < int DIM >
		__range__( const __range__< DIM > &r )
					: sx( r.sx ), ex( r.ex ), sy( r.sy ), ey( r.ey ), sz( r.sz ), ez( r.ez )
		{
		}

		size_type begin1( ) const { return( sy ); }
		size_type end1( ) const { return( ey ); }

		size_type begin2( ) const { return( sx ); }
		size_type end2( ) const { return( ex ); }

		size_type begin3( ) const { return( sz ); }
		size_type end3( ) const { return( ez ); }
	};

	template <  >
	struct __range__< 3 >
	{
		typedef size_t size_type;

		size_type sx;
		size_type ex;
		size_type sy;
		size_type ey;
		size_type sz;
		size_type ez;

		__range__(  ) : sx( 0 ), ex( 0 ), sy( 0 ), ey( 0 ), sz( 0 ), ez( 0 )
		{
		}

		__range__( size_type ssx, size_type eex, size_type ssy, size_type eey, size_type ssz, size_type eez )
					: sx( ssx ), ex( eex ), sy( ssy ), ey( eey ), sz( ssz ), ez( eez )
		{
		}

		template < int DIM >
		__range__( const __range__< DIM > &r )
					: sx( r.sx ), ex( r.ex ), sy( r.sy ), ey( r.ey ), sz( r.sz ), ez( r.ez )
		{
		}

		size_type begin1( ) const { return( sz ); }
		size_type end1( ) const { return( ez ); }

		size_type begin2( ) const { return( sx ); }
		size_type end2( ) const { return( ex ); }

		size_type begin3( ) const { return( sy ); }
		size_type end3( ) const { return( ey ); }
	};
}


// 斉藤先生によるユークリッド型距離変換の実装部分
namespace __saito__
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
					in( w - 1, j, k ) = in( w - 1, j, k ) < nd * nd ? in( w - 1, j, k ) : nd * nd;
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


	/// @brief ユークリッド2乗逆距離変換関数
	template < int DIMENSION >
	struct __inverse_distance_transform__
	{
		template < class Array >
		static void distance_transform( Array &in, const _distance_utility_::__range__< DIMENSION > &range, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
		{
			typedef typename Array::size_type		size_type;
			typedef typename Array::value_type		value_type;
			typedef typename Array::pointer			pointer;
			typedef typename Array::difference_type	difference_type;
			typedef _distance_utility_::__access__< DIMENSION > access;

			const value_type max = type_limits< value_type >::maximum( );
			double len = 0.0;

			size_type _1s = range.begin1( );
			size_type _2s = range.begin2( );
			size_type _3s = range.begin3( );
			size_type _1e = range.end1( );
			size_type _2e = range.end2( );
			size_type _3e = range.end3( );
			difference_type inum = _1e - _1s + 1;

			value_type *g = new value_type[ inum + 1 ];

			double as = access::aspect( in );
			double as2 = as * as;
			double _2as = 1.0 / ( 2.0 * as2 );
			difference_type diff = &access::at( in, 1, 0, 0 ) - &access::at( in, 0, 0, 0 );

			for( size_type i3 = _3s + thread_id ; i3 <= _3e ; i3 += thread_num )
			{
				for( size_type i2 = _2s ; i2 <= _2e ; i2++ )
				{
					pointer sp = &access::at( in, _1s, i2, i3 );
					pointer ep = &access::at( in, _1e, i2, i3 );

					memset( g, 0, sizeof( value_type ) * ( _1e - _1s + 1 + 1 ) );

					{
						pointer op = sp;
						pointer p = sp + diff;
						g[ 0 ] = *sp;
						for( difference_type i1 = 1 ; i1 < inum ; i1++, p += diff )
						{
							if( *p < *op )
							{
								difference_type num = static_cast< difference_type >( ( *op - *p - 1 ) * _2as );
								pointer pp = p;
								for( difference_type n = 0 ; n <= num && pp <= ep ; n++, pp += diff )
								{
									double nn = *op - ( n + 1 ) * ( n + 1 ) * as2;
									if( *pp >= nn )
									{
										break;
									}
									else if( g[ i1 + n ] < nn )
									{
										g[ i1 + n ] = static_cast< value_type >( nn );
									}
								}
							}
							else if( g[ i1 ] < *p )
							{
								g[ i1 ] = *p;
							}

							op = p;
						}
					}

					{
						pointer p = ep;
						for( difference_type i1 = inum - 1 ; i1 >= 0 ; i1--, p -= diff )
						{
							if( g[ i1 ] < g[ i1 + 1 ] )
							{
								difference_type num = static_cast< difference_type >( ( g[ i1 + 1 ] - g[ i1 ] - 1 ) * _2as );
								pointer pp = p;
								for( difference_type n = 0 ; n <= num && pp >= sp ; n++, pp -= diff )
								{
									double nn = g[ i1 + 1 ] - ( n + 1 ) * ( n + 1 ) * as2;
									if( g[ i1 - n ] >= nn )
									{
										break;
									}
									else if( *pp < nn )
									{
										*pp = static_cast< value_type >( nn );
									}
								}
							}
							else if( *p < g[ i1 ] )
							{
								*p = g[ i1 ];
							}
						}
					}
				}
			}

			delete [] g;
		}
	};


	// 斉藤先生によるユークリッド型距離変換のスレッド部分
	template < class T >
	class saito_distance_transform_thread : public mist::thread< saito_distance_transform_thread< T > >
	{
	public:
		typedef mist::thread< saito_distance_transform_thread< T > > base;
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

		const saito_distance_transform_thread& operator =( const saito_distance_transform_thread &p )
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

		saito_distance_transform_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), in_( NULL ), max_length_( -1.0 ), axis_( 0 )
		{
		}
		saito_distance_transform_thread( const saito_distance_transform_thread &p )
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
				__saito__::euclidean_distance_transform_y( *in_, max_length_, thread_id_, thread_num_ );
				break;

			case 2:
				__saito__::euclidean_distance_transform_z( *in_, max_length_, thread_id_, thread_num_ );
				break;

			case 0:
			default:
				__saito__::euclidean_distance_transform_x( *in_, max_length_, thread_id_, thread_num_ );
				break;
			}
			return( true );
		}
	};

	template < class T >
	class saito_inverse_distance_transform_thread : public mist::thread< saito_inverse_distance_transform_thread< T > >
	{
	public:
		typedef mist::thread< saito_inverse_distance_transform_thread< T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	protected:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		T *in_;
		size_type axis_;
		_distance_utility_::__range__< 1 > range_;

	public:
		void setup_parameters( T &in, const _distance_utility_::__range__< 1 > &range, size_type axis, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			range_ = range;
			axis_ = axis;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		void setup_axis( size_type axis )
		{
			axis_ = axis;
		}

		const saito_inverse_distance_transform_thread& operator =( const saito_inverse_distance_transform_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				range_ = p.range_;
				axis_ = p.axis_;
			}
			return( *this );
		}

		saito_inverse_distance_transform_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), in_( NULL ), axis_( 0 )
		{
		}
		saito_inverse_distance_transform_thread( const saito_inverse_distance_transform_thread &p )
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
				__inverse_distance_transform__< 1 >::distance_transform( *in_, _distance_utility_::__range__< 1 >( range_ ), thread_id_, thread_num_ );
				break;

			case 1:
				__inverse_distance_transform__< 2 >::distance_transform( *in_, _distance_utility_::__range__< 2 >( range_ ), thread_id_, thread_num_ );
				break;

			case 2:
			default:
				__inverse_distance_transform__< 3 >::distance_transform( *in_, _distance_utility_::__range__< 3 >( range_ ), thread_id_, thread_num_ );
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


	/// @brief Y,Z軸方向用の距離伝播関数（1次以外の全ての次元）
	template < int DIMENSION >
	struct __distance_transform__
	{
		template < class Array >
		static void distance_transform( Array &in, const _distance_utility_::__range__< DIMENSION > &range, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
		{
			typedef typename Array::size_type		size_type;
			typedef typename Array::value_type		value_type;
			typedef typename Array::pointer			pointer;
			typedef typename Array::difference_type	difference_type;
			typedef _distance_utility_::__access__< DIMENSION > access;

			size_type _1s = range.begin1( );
			size_type _2s = range.begin2( );
			size_type _3s = range.begin3( );
			size_type _1e = range.end1( );
			size_type _2e = range.end2( );
			size_type _3e = range.end3( );

			double as = access::aspect( in );
			difference_type diff = &access::at( in, 1, 0, 0 ) - &access::at( in, 0, 0, 0 );

			double *g = new double[ _1e - _1s + 2 ];
			double *h = new double[ _1e - _1s + 2 ];

			for( size_type i3 = _3s + thread_id ; i3 <= _3e ; i3 += thread_num )
			{
				for( size_type i2 = _2s ; i2 <= _2e ; i2++ )
				{
					pointer p = &access::at( in, 0, i2, i3 );
					for( size_type _i1s = _1s ; _i1s <= _1e ; )
					{
						// 最初に画素が始まる位置を検索する
						for( ; _i1s <= _1e ; _i1s++ )
						{
							if( p[ _i1s * diff ] != 0 )
							{
								break;
							}
						}

						if( _i1s > _1e )
						{
							// 処理対象が存在しないのでスキップ
							break;
						}

						// 連続する画素の終端を検索する
						size_type _i1e;
						for( _i1e = _i1s ; _i1e <= _1e ; _i1e++ )
						{
							if( p[ _i1e * diff ] == 0 )
							{
								break;
							}
						}

						if( _i1s > 0 )
						{
							_i1s--;
						}
						if( _i1e > _1e )
						{
							_i1e = _1e;
						}

						difference_type l = 0;

						for( size_type i1 = _i1s ; i1 <= _i1e ; i1++ )
						{
							double nd = static_cast< double >( i1 - _i1s + 1 ) * as;
							double fn = static_cast< double >( p[ i1 * diff ] );

							if( l < 2 )
							{
								l++;
								g[ l ] = fn;
								h[ l ] = nd;
							}
							else
							{
								while( l >= 2 && remove_edt( g[ l - 1 ], g[ l ], fn, h[ l - 1 ], h[ l ], nd ) )
								{
									l--;
								}
								l++;
								g[ l ] = fn;
								h[ l ] = nd;
							}
						}

						if( l == 0 )
						{
							break;
						}

						difference_type ns = l;
						l = 1;

						for( size_type i1 = _i1s ; i1 <= _i1e ; i1++ )
						{
							double nd = ( i1 - _i1s + 1 ) * as;
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
							p[ i1 * diff ] = static_cast< value_type >( len );
						}

						// 次の位置に設定する
						_i1s = _i1e + 1;
					}
				}
			}

			delete [] g;
			delete [] h;
		}
	};


	/// @brief X軸方向専用の距離伝播関数
	template < >
	struct __distance_transform__< 1 >
	{
		template < class Array >
		static void distance_transform( Array &in, const _distance_utility_::__range__< 1 > &range, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
		{
			typedef typename Array::size_type		size_type;
			typedef typename Array::value_type		value_type;
			typedef typename Array::pointer			pointer;
			typedef typename Array::difference_type	difference_type;

			const difference_type w = in.width( );
			value_type max = type_limits< value_type >::maximum( );
			value_type len;

			max = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;

			size_type sx = range.begin1( );
			size_type sy = range.begin2( );
			size_type sz = range.begin3( );
			size_type ex = range.end1( );
			size_type ey = range.end2( );
			size_type ez = range.end3( );

			for( size_type k = sz ; k <= ez ; k++ )
			{
				for( size_type j = sy + thread_id ; j <= ey ; j += thread_num )
				{
					pointer sp = &in( sx, j, k );
					pointer ep = &in( ex, j, k );

					if( sp[ 0 ] != 0 )
					{
						len = max;
						sp[ 0 ] = len * len;
					}
					else
					{
						len = 0;
					}

					for( pointer p = sp + 1 ; p <= ep ; p++ )
					{
						if( *p != 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							*p = len * len;
						}
						else
						{
							len = 0;
						}
					}

					if( ep[ 0 ] != 0 )
					{
						len = max;
						ep[ 0 ] = ep[ 0 ] < len * len ? ep[ 0 ] : len * len;
					}
					else
					{
						len = 0;
					}

					for( pointer p = ep - 1 ; p >= sp ; p-- )
					{
						if( *p != 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							*p = *p < len * len ? *p : len * len;
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


	/// @brief Y,Z軸方向用の距離伝播関数（1次以外の全ての次元）
	template < int DIMENSION >
	struct __voronoi_distance_transform__
	{
		template < class Array1, class Array2 >
		static void distance_transform( Array1 &voronoi, Array2 &dist, const _distance_utility_::__range__< DIMENSION > &range, typename Array2::size_type thread_id = 0, typename Array2::size_type thread_num = 1 )
		{
			typedef typename Array2::size_type			size_type;
			typedef typename Array2::value_type			value_type;
			typedef typename Array1::pointer			ipointer;
			typedef typename Array2::pointer			pointer;
			typedef typename Array2::difference_type	difference_type;
			typedef _distance_utility_::__access__< DIMENSION > access;

			size_type _1s = range.begin1( );
			size_type _2s = range.begin2( );
			size_type _3s = range.begin3( );
			size_type _1e = range.end1( );
			size_type _2e = range.end2( );
			size_type _3e = range.end3( );

			double as = access::aspect( dist );
			difference_type diff = &access::at( dist, 1, 0, 0 ) - &access::at( dist, 0, 0, 0 );

			difference_type *idx = new difference_type[ _1e - _1s + 2 ];
			double *g = new double[ _1e - _1s + 2 ];
			double *h = new double[ _1e - _1s + 2 ];

			for( size_type i3 = _3s + thread_id ; i3 <= _3e ; i3 += thread_num )
			{
				for( size_type i2 = _2s ; i2 <= _2e ; i2++ )
				{
					ipointer vp = &access::at( voronoi, 0, i2, i3 );
					pointer  p  = &access::at( dist, 0, i2, i3 );

					difference_type l = 0;

					for( size_type i1 = _1s ; i1 <= _1e ; i1++ )
					{
						double nd = static_cast< double >( i1 - _1s + 1 ) * as;
						double fn = static_cast< double >( p[ i1 * diff ] );

						if( l < 2 )
						{
							l++;
							g[ l ] = fn;
							h[ l ] = nd;
							idx[ l ] = i1;
						}
						else
						{
							while( l >= 2 && remove_edt( g[ l - 1 ], g[ l ], fn, h[ l - 1 ], h[ l ], nd ) )
							{
								l--;
							}
							l++;
							g[ l ] = fn;
							h[ l ] = nd;
							idx[ l ] = i1;
						}
					}

					if( l == 0 )
					{
						break;
					}

					difference_type ns = l;
					l = 1;

					for( size_type i1 = _1s ; i1 <= _1e ; i1++ )
					{
						double nd = ( i1 - _1s + 1 ) * as;
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
						p[ i1 * diff ]  = static_cast< value_type >( len );
						vp[ i1 * diff ] = vp[ idx[ l ] * diff ];
					}
				}
			}

			delete [] idx;
			delete [] g;
			delete [] h;
		}
	};


	/// @brief X軸方向専用の距離伝播関数
	template < >
	struct __voronoi_distance_transform__< 1 >
	{
		template < class Array1, class Array2 >
		static void distance_transform( Array1 &voronoi, Array2 &dist, const _distance_utility_::__range__< 1 > &range, typename Array2::size_type thread_id = 0, typename Array2::size_type thread_num = 1 )
		{
			typedef typename Array2::size_type			size_type;
			typedef typename Array1::value_type			ivalue_type;
			typedef typename Array2::value_type			value_type;
			typedef typename Array1::pointer			ipointer;
			typedef typename Array2::pointer			pointer;
			typedef typename Array2::difference_type	difference_type;

			const difference_type w = dist.width( );
			value_type max = type_limits< value_type >::maximum( );
			max = static_cast< value_type >( w ) < max ? static_cast< value_type >( w ) : max;

			ivalue_type *val = new ivalue_type[ w ];
			value_type len;

			size_type sx = range.begin1( );
			size_type sy = range.begin2( );
			size_type sz = range.begin3( );
			size_type ex = range.end1( );
			size_type ey = range.end2( );
			size_type ez = range.end3( );

			for( size_type k = sz ; k <= ez ; k++ )
			{
				for( size_type j = sy + thread_id ; j <= ey ; j += thread_num )
				{
					ivalue_type *vp = val;
					ipointer    ip = &voronoi( sx, j, k );
					pointer     sp = &dist( sx, j, k );
					pointer     ep = &dist( ex, j, k );

					*vp++ = *ip;
					if( *ip++ == 0 )
					{
						len = max;
						sp[ 0 ] = len * len;
					}
					else
					{
						len = 0;
					}

					for( pointer p = sp + 1 ; p <= ep ; p++, ip++ )
					{
						*vp++ = *ip;
						if( *ip == 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							*p = len * len;
							*ip = *( ip - 1 );
						}
						else
						{
							len = 0;
						}
					}

					vp--;
					ip -= 2;

					if( *vp-- == 0 )
					{
						len = max;
						value_type len2 = len * len;

						if( ep[ 0 ] > len2 )
						{
							ep[ 0 ] = len2;
						}
					}
					else
					{
						len = 0;
					}

					for( pointer p = ep - 1 ; p >= sp ; p--, ip--, vp-- )
					{
						if( *vp == 0 )
						{
							len = len + 1 < max ? len + 1 : len;
							value_type len2 = len * len;

							if( *p > len2 )
							{
								*p = len2;
								*ip = *( ip + 1 );
							}
						}
						else
						{
							len = 0;
						}
					}
				}
			}

			delete [] val;
		}
	};


	template < class Array, int DIMENSION >
	bool compute_object_range( const Array &in, _distance_utility_::__range__< DIMENSION > &range )
	{
		typedef typename Array::size_type size_type;
		typedef typename Array::difference_type difference_type;

		difference_type w = in.width( );
		difference_type h = in.height( );
		difference_type d = in.depth( );

		difference_type sx, ex, sy, ey, sz, ez, i, j, k;

		sx = sy = sz = 0;
		ex = w - 1;
		ey = h - 1;
		ez = d - 1;

		for( k = 0 ; k < d ; k++ )
		{
			bool found = false;
			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					if( in( i, j, k ) != 0 )
					{
						found = true;
						break;
					}
				}

				if( found )
				{
					break;
				}
			}

			if( found )
			{
				break;
			}
		}

		if( k >= d )
		{
			// 何も見つからなかった・・・
			return( false );
		}

		sz = k;

		for( k = d - 1 ; k > sz ; k-- )
		{
			bool found = false;
			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					if( in( i, j, k ) != 0 )
					{
						found = true;
						break;
					}
				}

				if( found )
				{
					break;
				}
			}

			if( found )
			{
				break;
			}
		}

		ez = k;

		for( j = 0 ; j < h ; j++ )
		{
			bool found = false;
			for( k = sz ; k <= ez ; k++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					if( in( i, j, k ) != 0 )
					{
						found = true;
						break;
					}
				}

				if( found )
				{
					break;
				}
			}

			if( found )
			{
				break;
			}
		}

		sy = j;

		for( j = h - 1 ; j > sy ; j-- )
		{
			bool found = false;
			for( k = sz ; k <= ez ; k++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					if( in( i, j, k ) != 0 )
					{
						found = true;
						break;
					}
				}

				if( found )
				{
					break;
				}
			}

			if( found )
			{
				break;
			}
		}

		ey = j;

		for( i = 0 ; i < w ; i++ )
		{
			bool found = false;
			for( k = sz ; k <= ez ; k++ )
			{
				for( j = sy ; j <= ey ; j++ )
				{
					if( in( i, j, k ) != 0 )
					{
						found = true;
						break;
					}
				}

				if( found )
				{
					break;
				}
			}

			if( found )
			{
				break;
			}
		}

		sx = i;

		for( i = w - 1 ; i > sx ; i-- )
		{
			bool found = false;
			for( k = sz ; k <= ez ; k++ )
			{
				for( j = sy ; j <= ey ; j++ )
				{
					if( in( i, j, k ) != 0 )
					{
						found = true;
						break;
					}
				}

				if( found )
				{
					break;
				}
			}

			if( found )
			{
				break;
			}
		}

		ex = i;

		// 一周り大きい範囲を設定する
		range.sx = sx > 0 ? sx - 1 : 0;
		range.ex = ex < w - 1 ? ex + 1 : w - 1;
		range.sy = sy > 0 ? sy - 1 : 0;
		range.ey = ey < h - 1 ? ey + 1 : h - 1;
		range.sz = sz > 0 ? sz - 1 : 0;
		range.ez = ez < d - 1 ? ez + 1 : d - 1;

		return( true );
	}

	template < class T >
	class calvin_distance_transform_thread : public mist::thread< calvin_distance_transform_thread< T > >
	{
	public:
		typedef mist::thread< calvin_distance_transform_thread< T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	protected:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		T *in_;
		size_type axis_;
		_distance_utility_::__range__< 1 > range_;

	public:
		void setup_parameters( T &in, const _distance_utility_::__range__< 1 > &range, size_type axis, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			range_ = range;
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
				range_ = p.range_;
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
				__distance_transform__< 1 >::distance_transform( *in_, _distance_utility_::__range__< 1 >( range_ ), thread_id_, thread_num_ );
				break;

			case 1:
				__distance_transform__< 2 >::distance_transform( *in_, _distance_utility_::__range__< 2 >( range_ ), thread_id_, thread_num_ );
				break;

			case 2:
			default:
				__distance_transform__< 3 >::distance_transform( *in_, _distance_utility_::__range__< 3 >( range_ ), thread_id_, thread_num_ );
				break;
			}
			return( true );
		}
	};


	template < class T1, class T2 >
	class calvin_voronoi_distance_transform_thread : public mist::thread< calvin_voronoi_distance_transform_thread< T1, T2 > >
	{
	public:
		typedef mist::thread< calvin_voronoi_distance_transform_thread< T1, T2 > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T1::size_type size_type;
		typedef typename T1::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		T1 *voronoi_;
		T2 *dist_;
		size_type axis_;
		_distance_utility_::__range__< 1 > range_;

	public:
		void setup_parameters( T1 &voronoi, T2 &dist, const _distance_utility_::__range__< 1 > &range, size_type axis, size_type thread_id, size_type thread_num )
		{
			voronoi_  = &voronoi;
			dist_  = &dist;
			range_ = range;
			axis_ = axis;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		void setup_axis( size_type axis )
		{
			axis_ = axis;
		}

		const calvin_voronoi_distance_transform_thread& operator =( const calvin_voronoi_distance_transform_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				voronoi_ = p.voronoi_;
				dist_    = p.dist_;
				range_   = p.range_;
				axis_    = p.axis_;
			}
			return( *this );
		}

		calvin_voronoi_distance_transform_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), voronoi_( NULL ), dist_( NULL ), axis_( 0 )
		{
		}
		calvin_voronoi_distance_transform_thread( const calvin_voronoi_distance_transform_thread &p )
			: base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ), voronoi_( NULL ), dist_( NULL ), axis_( p.axis_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			switch( axis_ )
			{
			case 0:
				__voronoi_distance_transform__< 1 >::distance_transform( *voronoi_, *dist_, _distance_utility_::__range__< 1 >( range_ ), thread_id_, thread_num_ );
				break;

			case 1:
				__voronoi_distance_transform__< 2 >::distance_transform( *voronoi_, *dist_, _distance_utility_::__range__< 2 >( range_ ), thread_id_, thread_num_ );
				break;

			case 2:
			default:
				__voronoi_distance_transform__< 3 >::distance_transform( *voronoi_, *dist_, _distance_utility_::__range__< 3 >( range_ ), thread_id_, thread_num_ );
				break;
			}
			return( true );
		}
	};
}


namespace _meijster_distance_transform_
{
	struct MDT
	{
		template < class T, class Val >
		static T Func( const T &x, const T &i, const Val &gi )
		{
			return( static_cast< T >( std::abs( static_cast< int >( x - i ) ) + gi ) );
		}

		template < class T, class Val >
		static T Sep( const T &i, const T &u, const Val &gi, const Val &gu )
		{
			if( gu >= gi + u - i )
			{
				return( type_limits< T >::maximum( ) );
			}
			else if( gi > gu + u - i )
			{
				return( type_limits< T >::minimum( ) );
			}
			else
			{
				return( ( gu - gi + u + i ) / 2 );
			}
		}
	};

	struct CDT
	{
		template < class T, class Val >
		static T Func( const T &x, const T &i, const Val &gi )
		{
			T v = std::abs( static_cast< int >( x - i ) );
			return( static_cast< T >( v > gi ? v : gi ) );
		}

		template < class T, class Val >
		static T Sep( const T &i, const T &u, const Val &gi, const Val &gu )
		{
			if( gi <= gu )
			{
				T igu = static_cast< T >( i + gu );
				T iu = static_cast< T >( ( i + u ) / 2 );
				return( igu > iu ? igu : iu );
			}
			else
			{
				T ugi = static_cast< T >( u - gi );
				T iu = static_cast< T >( ( i + u ) / 2 );
				return( ugi < iu ? ugi : iu );
			}
		}
	};

	template < class Array >
	void distance_transform_x( Array &in, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
	{
		typedef typename Array::difference_type difference_type;
		typedef typename Array::value_type value_type;

		difference_type i, j, k;

		const difference_type w = in.width( );
		const difference_type h = in.height( );
		const difference_type d = in.depth( );

		value_type max = type_limits< value_type >::maximum( );

		for( k = 0 ; k < d ; k++ )
		{
			for( i = thread_id ; i < w ; i += thread_num )
			{
				if( in( i, 0, k ) != 0 )
				{
					in( i, 0, k ) = max;
				}

				for( j = 1 ; j < h ; j++ )
				{
					if( in( i, j, k ) != 0 )
					{
						in( i, j, k ) = in( i, j - 1, k ) + 1;
					}
				}

				for( j = h - 2 ; j >= 0 ; j-- )
				{
					if( in( i, j + 1, k ) < in( i, j, k ) )
					{
						in( i, j, k ) = in( i, j + 1, k ) + 1;
					}
				}
			}
		}
	}

	template < class Array, class Metric >
	void distance_transform_y( Array &in, Metric __dmy__, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
	{
		typedef typename Array::pointer pointer;
		typedef typename Array::difference_type difference_type;
		typedef typename Array::value_type value_type;

		const difference_type w = in.width( );
		const difference_type h = in.height( );
		const difference_type d = in.depth( );

		difference_type i, j, k, q;
		difference_type *s = new difference_type[ w ];
		difference_type *t = new difference_type[ w ];

		for( k = 0 ; k < d ; k++ )
		{
			for( j = thread_id ; j < h ; j += thread_num )
			{
				q = s[ 0 ] = t[ 0 ] = 0;

				pointer pin = &in( 0, j, k );

				for( i = 1 ; i < w ; i++ )
				{
					value_type val = in( i, j, k );

					while( q >= 0 && Metric::Func( t[ q ], s[ q ], pin[ s[ q ] ] ) > Metric::Func( t[ q ], i, val ) )
					{
						q--;
					}

					if( q < 0 )
					{
						q = 0;
						s[ 0 ] = i;
					}
					else
					{
						difference_type tmp = Metric::Sep( s[ q ], i, pin[ s[ q ] ], val );

						if( tmp < w - 1 )
						{
							q++;
							s[ q ] = i;
							t[ q ] = tmp + 1;
						}
					}
				}

				for( i = w - 1 ; i >= 0 ; i-- )
				{
					pin[ i ] = static_cast< value_type >( Metric::Func( i, s[ q ], pin[ s[ q ] ] ) );
					if( i == t[ q ] )
					{
						q--;
					}
				}
			}
		}

		delete [] s;
		delete [] t;
	}

	template < class Array, class Metric >
	void distance_transform_z( Array &in, Metric __dmy__, typename Array::size_type thread_id = 0, typename Array::size_type thread_num = 1 )
	{
		typedef typename Array::pointer pointer;
		typedef typename Array::difference_type difference_type;
		typedef typename Array::value_type value_type;

		const difference_type w = in.width( );
		const difference_type h = in.height( );
		const difference_type d = in.depth( );

		difference_type i, j, k, q;
		difference_type *s = new difference_type[ d ];
		difference_type *t = new difference_type[ d ];
		difference_type diff = &in( 0, 0, 1 ) - &in( 0, 0, 0 );

		for( j = 0 ; j < h ; j++ )
		{
			for( i = thread_id ; i < w ; i += thread_num )
			{
				q = s[ 0 ] = t[ 0 ] = 0;

				pointer pin = &in( i, j, 0 );

				for( k = 1 ; k < d ; k++ )
				{
					value_type val = in( i, j, k );

					while( q >= 0 && Metric::Func( t[ q ], s[ q ], pin[ s[ q ] * diff ] ) > Metric::Func( t[ q ], k, val ) )
					{
						q--;
					}

					if( q < 0 )
					{
						q = 0;
						s[ 0 ] = k;
					}
					else
					{
						difference_type tmp = Metric::Sep( s[ q ], k, pin[ s[ q ] * diff ], val );

						if( tmp < d - 1 )
						{
							q++;
							s[ q ] = k;
							t[ q ] = tmp + 1;
						}
					}
				}

				for( k = d - 1 ; k >= 0 ; k-- )
				{
					pin[ k * diff ] = static_cast< value_type >( Metric::Func( k, s[ q ], pin[ s[ q ] * diff ] ) );
					if( k == t[ q ] )
					{
						q--;
					}
				}
			}
		}

		delete [] s;
		delete [] t;
	}


	template < class T, class Metric >
	class distance_transform_thread : public mist::thread< distance_transform_thread< T, Metric > >
	{
	public:
		typedef mist::thread< distance_transform_thread< T, Metric > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		T *in_;
		size_type axis_;
		Metric dmy_;

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

		const distance_transform_thread& operator =( const distance_transform_thread &p )
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

		distance_transform_thread( size_type id = 0, size_type num = 1 )
			: thread_id_( id ), thread_num_( num ), in_( NULL ), axis_( 0 )
		{
		}
		distance_transform_thread( const distance_transform_thread &p )
			: base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ), in_( NULL ), axis_( p.axis_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( )
		{
			switch( axis_ )
			{
			case 1:
				distance_transform_y( *in_, dmy_, thread_id_, thread_num_ );
				break;

			case 2:
				distance_transform_z( *in_, dmy_, thread_id_, thread_num_ );
				break;

			case 0:
			default:
				distance_transform_x( *in_, thread_id_, thread_num_ );
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



/// @brief 斉藤先生によるユークリッド2乗距離変換
namespace saito
{
	/// @brief ユークリッド2乗距離変換
	//! 
	//! 計算される距離は，ユークリッド2乗距離となります．
	//! ユークリッド距離に変換するためには，計算結果の各値の平方根を求めてください．
	//! ただし，計算に用いる画素が立方体ではなく直方体の場合には，計算されるユークリッド2距離も画素のアスペクトを考慮して計算されます．
	//! 立方体画素を用いている場合には以下を考慮する必要はありません．
	//!
	//! 本関数で計算されるユークリッド2乗距離は，X軸方向の画素の大きさを1としたときの比を用いて計算されます．
	//! ここで，本関数のある画素の出力が @f$l^2@f$ で，画素のX軸方向の解像度（大きさ）が @f$a_x@f$ の場合， @f$l a_x@f$ が画素の大きさを考慮したユークリッド距離値になります．
	//! 
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! - 参考文献
	//!   - 齋藤豊文, 鳥脇純一郎, "3次元ディジタル画像に対するユークリッド距離変換," 電子情報通信学会論文誌, J76-D-II, No. 3, pp.445-453, 1993
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
		typedef __saito__::saito_distance_transform_thread< Array2 > saito_distance_transform_thread;

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

		saito_distance_transform_thread *thread = new saito_distance_transform_thread[ thread_num ];

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


	/// @brief ユークリッド2乗逆距離変換
	//! 
	//! 入力画像はユークリッド2乗距離が入った画像であり，出力画像は2値図形となります．
	//! アスペクトを考慮したユークリッド2乗逆距離変換が可能です．
	//!
	//! 本関数で用いるユークリッド2乗距離は，X軸方向の画素の大きさを1としたときの比を用いて計算されます．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! - 参考文献
	//!   - 鳥脇純一郎, "3次元ディジタル画像処理," 昭晃堂, 2002
	//!   - 斎藤豊文, 鳥脇純一郎, ``ディジタル画像におけるユークリッド逆距離変換とスケルトンについて,'' 電子情報通信学会パターン認識・理解研究会資料, PRU93-50, No.228, pp.57-64, 1993
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void inverse_distance_transform( const Array1 &in, Array2 &out, typename Array1::size_type thread_num = 0 )
	{
		typedef typename Array2::size_type  size_type;
		typedef typename Array2::difference_type  difference_type;
		typedef typename Array2::value_type value_type;
		typedef __saito__::saito_inverse_distance_transform_thread< Array2 > saito_inverse_distance_transform_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		out = in;
		//out.resize( in.size1( ), in.size2( ), in.size3( ) );
		//out.reso1( in.reso1( ) );
		//out.reso2( in.reso2( ) );
		//out.reso3( in.reso3( ) );

		//for( size_type i = 0 ; i < in.size( ) ; i++ )
		//{
		//	out[ i ] = static_cast< value_type >( in[ i ] );
		//}

		_distance_utility_::__range__< 0 > object_range;
		object_range.sx = 0;
		object_range.ex = in.size1( ) - 1;
		object_range.sy = 0;
		object_range.ey = in.size2( ) - 1;
		object_range.sz = 0;
		object_range.ez = in.size3( ) - 1;

		saito_inverse_distance_transform_thread *thread = new saito_inverse_distance_transform_thread[ thread_num ];

		if( in.width( ) > 1 )
		{
			// X軸方向の処理
			for( size_type i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, object_range, 0, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.height( ) > 1 )
		{
			// Y軸方向の処理
			for( size_type i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, object_range, 1, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.depth( ) > 1 )
		{
			// Z軸方向の処理
			for( size_type i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, object_range, 2, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		delete [] thread;

		// 逆距離変換の結果得られる距離値は正しいユークリッド2乗距離ではないため，結果をもとに2値図形に直す．
		for( size_type i = 0 ; i < out.size( ) ; i++ )
		{
			out[ i ] = out[ i ] != 0 ? 1 : 0;
		}
	}
}


/// @brief Calvinによるユークリッド2乗距離変換
namespace calvin
{
	/// @brief ユークリッド2乗距離変換
	//! 
	//! 計算される距離は，ユークリッド2乗距離となります．
	//! ユークリッド距離に変換するためには，計算結果の各値の平方根を求めてください．
	//! ただし，計算に用いる画素が立方体ではなく直方体の場合には，計算されるユークリッド2距離も画素のアスペクトを考慮して計算されます．
	//! 立方体画素を用いている場合には以下を考慮する必要はありません．
	//!
	//! 本関数で計算されるユークリッド2乗距離は，X軸方向の画素の大きさを1としたときの比を用いて計算されます．
	//! ここで，本関数のある画素の出力が @f$l^2@f$ で，画素のX軸方向の解像度（大きさ）が @f$a_x@f$ の場合， @f$l a_x@f$ が画素の大きさを考慮したユークリッド距離値になります．
	//! 
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

		_distance_utility_::__range__< 0 > object_range;
		if( !__calvin__::compute_object_range( out, object_range ) )
		{
			// １つも１画素が見つからなかったので終了する
			return;
		}

		calvin_distance_transform_thread *thread = new calvin_distance_transform_thread[ thread_num ];

		if( in.width( ) > 1 )
		{
			// X軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, object_range, 0, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.height( ) > 1 )
		{
			// Y軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, object_range, 1, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( in.depth( ) > 1 )
		{
			// Z軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, object_range, 2, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		delete [] thread;
	}


	/// @brief ユークリッド距離を用いたボロノイ分割
	//! 
	//! ユークリッド2乗距離変換とボロノイ分割を同時に行うアルゴリズムになります．
	//! 計算されるユークリッド2乗距離は，最も近い母点までの２乗ユークリッド距離を表します．
	//! 画素のアスペクトを考慮したボロノイ分割が可能です．
	//! アルゴリズム自体は，参考文献の Calvin R. Maurer, Jr. の論文を改良したものである．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでは動作しません
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! @attention 正しくボロノイ分割を行うために，入力画像内の各母点に対してラベリングを施しておく必要があります．
	//! 
	//! - 参考文献
	//!   - 齋藤豊文, 鳥脇純一郎, "3次元ユークリッド距離変換および拡張ボロノイ分割のアルゴリズムと肝組織標本画像の解析," 画像電子学会誌, 第21巻，第5号, pp.468-474, 1992
	//!   - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
	//! 
	//! @param[in,out] voronoi    … 母点毎に別々のラベルが割り当てられた画像．ボロノイ分割結果のラベル画像が出力される．
	//! @param[out]    dist       … 出力ユークリッド２乗距離画像
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void voronoi_distance_transform( Array1 &voronoi, Array2 &dist, typename Array1::size_type thread_num = 0 )
	{
		typedef typename Array2::size_type  size_type;
		typedef typename Array2::value_type value_type;
		typedef __calvin__::calvin_voronoi_distance_transform_thread< Array1, Array2 > calvin_voronoi_distance_transform_thread;

		if( thread_num == 0 )
		{
			thread_num = static_cast< size_type >( get_cpu_num( ) );
		}

		dist.resize( voronoi.size1( ), voronoi.size2( ), voronoi.size3( ) );
		dist.reso1( voronoi.reso1( ) );
		dist.reso2( voronoi.reso2( ) );
		dist.reso3( voronoi.reso3( ) );

		size_type i;

		_distance_utility_::__range__< 0 > object_range;
		object_range.sx = 0;
		object_range.ex = voronoi.size1( ) - 1;
		object_range.sy = 0;
		object_range.ey = voronoi.size2( ) - 1;
		object_range.sz = 0;
		object_range.ez = voronoi.size3( ) - 1;

		calvin_voronoi_distance_transform_thread *thread = new calvin_voronoi_distance_transform_thread[ thread_num ];

		if( voronoi.width( ) > 1 )
		{
			// X軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( voronoi, dist, object_range, 0, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( voronoi.height( ) > 1 )
		{
			// Y軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( voronoi, dist, object_range, 1, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		if( voronoi.depth( ) > 1 )
		{
			// Z軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( voronoi, dist, object_range, 2, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		delete [] thread;
	}

	/// @brief ユークリッド距離を用いたボロノイ分割
	//! 
	//! ユークリッド距離に基づくボロノイ分割を行い，画素のアスペクトを考慮したボロノイ分割が可能です．
	//! 内部では，ユークリッド2乗距離を計算するための一時画像を作成します．
	//! アルゴリズム自体は，参考文献の Calvin R. Maurer, Jr. の論文を改良したものである．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでは動作しません
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! @attention 正しくボロノイ分割を行うために，入力画像内の各母点に対してラベリングを施しておく必要があります．
	//! 
	//! - 参考文献
	//!   - 齋藤豊文, 鳥脇純一郎, "3次元ユークリッド距離変換および拡張ボロノイ分割のアルゴリズムと肝組織標本画像の解析," 画像電子学会誌, 第21巻，第5号, pp.468-474, 1992
	//!   - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
	//! 
	//! @param[in,out] voronoi    … 母点毎に別々のラベルが割り当てられた画像．ボロノイ分割結果のラベル画像が出力される．
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	template < class Array >
	void voronoi_transform( Array &voronoi, typename Array::size_type thread_num = 0 )
	{
		double ax = voronoi.reso1( );
		double ay = voronoi.reso2( );
		double az = voronoi.reso3( );
		if( ax == ay && ay == az )
		{
			typename Array::template rebind< unsigned int >::other dist;
			voronoi_distance_transform( voronoi, dist, thread_num );
		}
		else
		{
			typename Array::template rebind< float >::other dist;
			voronoi_distance_transform( voronoi, dist, thread_num );
		}
	}
}


/// @brief ユークリッド距離変換
namespace euclidean
{
	/// @brief ユークリッド距離変換
	//! 
	//! 計算される距離は，ユークリッド2乗距離となります．
	//! ユークリッド距離に変換するためには，計算結果の各値の平方根を求めてください．
	//! ただし，計算に用いる画素が立方体ではなく直方体の場合には，計算されるユークリッド2距離も画素のアスペクトを考慮して計算されます．
	//! 立方体画素を用いている場合には以下を考慮する必要はありません．
	//!
	//! 本関数で計算されるユークリッド2乗距離は，X軸方向の画素の大きさを1としたときの比を用いて計算されます．
	//! ここで，本関数のある画素の出力が @f$l^2@f$ で，画素のX軸方向の解像度（大きさ）が @f$a_x@f$ の場合， @f$l a_x@f$ が画素の大きさを考慮したユークリッド距離値になります．
	//! 
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
		calvin::distance_transform( in, out, thread_num );
	}


	/// @brief ユークリッド2乗逆距離変換
	//! 
	//! 入力画像はユークリッド2乗距離が入った画像であり，出力画像は2値図形となります．
	//! アスペクトを考慮したユークリッド2乗逆距離変換が可能です．
	//!
	//! 本関数で用いるユークリッド2乗距離は，X軸方向の画素の大きさを1としたときの比を用いて計算されます．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! 
	//! - 参考文献
	//!   - 鳥脇純一郎, "3次元ディジタル画像処理," 昭晃堂, 2002
	//!   - 斎藤豊文, 鳥脇純一郎, ``ディジタル画像におけるユークリッド逆距離変換とスケルトンについて,'' 電子情報通信学会パターン認識・理解研究会資料, PRU93-50, No.228, pp.57-64, 1993
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void inverse_distance_transform( const Array1 &in, Array2 &out, typename Array1::size_type thread_num = 0 )
	{
		saito::inverse_distance_transform( in, out, thread_num );
	}


	//! @addtogroup voronoi_distance_group ボロノイ分割
	//!
	//! @code 次のヘッダをインクルードする
	//! #include <mist/filter/distance.h>
	//! @endcode
	//!
	//!  @{


	/// @brief ユークリッド距離を用いたボロノイ分割
	//! 
	//! ユークリッド2乗距離変換とボロノイ分割を同時に行うアルゴリズムになります．
	//! 計算されるユークリッド２乗距離は，最も近い母点までの２乗ユークリッド距離を表します．
	//! 画素のアスペクトを考慮したボロノイ分割が可能です．
	//! アルゴリズム自体は，参考文献の Calvin R. Maurer, Jr. の論文を改良したものである．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! @attention 正しくボロノイ分割を行うために，入力画像内の各母点に対してラベリングを施しておく必要があります．
	//! 
	//! - 参考文献
	//!   - 齋藤豊文, 鳥脇純一郎, "3次元ユークリッド距離変換および拡張ボロノイ分割のアルゴリズムと肝組織標本画像の解析," 画像電子学会誌, 第21巻，第5号, pp.468-474, 1992
	//!   - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
	//! 
	//! @param[in,out] voronoi    … 母点毎に別々のラベルが割り当てられた画像．ボロノイ分割結果のラベル画像が出力される．
	//! @param[out]    dist       … 出力ユークリッド２乗距離画像
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void voronoi_distance_transform( Array1 &voronoi, Array2 &dist, typename Array1::size_type thread_num = 0 )
	{
		calvin::voronoi_distance_transform( voronoi, dist, thread_num );
	}

	/// @brief ユークリッド距離を用いたボロノイ分割
	//! 
	//! ユークリッド距離に基づくボロノイ分割を行い，画素のアスペクトを考慮したボロノイ分割が可能です．
	//! 内部では，ユークリッド2乗距離を計算するための一時画像を作成します．
	//! アルゴリズム自体は，参考文献の Calvin R. Maurer, Jr. の論文を改良したものである．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでは動作しません
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//! @attention 正しくボロノイ分割を行うために，入力画像内の各母点に対してラベリングを施しておく必要があります．
	//! 
	//! - 参考文献
	//!   - 齋藤豊文, 鳥脇純一郎, "3次元ユークリッド距離変換および拡張ボロノイ分割のアルゴリズムと肝組織標本画像の解析," 画像電子学会誌, 第21巻，第5号, pp.468-474, 1992
	//!   - Calvin R. Maurer, Jr., Rensheng Qi, and Vijay Raghavan, "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms of Binary Images in Arbitrary Dimensions", IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 25, No. 2, February 2003
	//! 
	//! @param[in,out] voronoi    … 母点毎に別々のラベルが割り当てられた画像．ボロノイ分割結果のラベル画像が出力される．
	//! @param[in]     thread_num … 使用するスレッド数
	//! 
	template < class Array >
	void voronoi_transform( Array &voronoi, typename Array::size_type thread_num = 0 )
	{
		calvin::voronoi_transform( voronoi, thread_num );
	}

	/// @}
	//  ボロノイ分割グループの終わり
}

/// @}
//  ユークリッド距離変換グループの終わり


//! @addtogroup meijster_distance_group A. Meijster による距離変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{

/// @brief A. Meijster による距離変換（CDT, MDT）
namespace meijster
{
	/// @brief A. Meijster による距離変換（CDT, MDT）
	//! 
	//! 計算される距離は，マンハッタン距離（シティーブロック距離），チェスボード距離，のいずれかになります．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! - 参考文献
	//!     - A. Meijster, J. Roerdink, and W. Hesselink, "A general algorithm for computing distance transforms in linear time," In: Mathematical Morphology and its Applications to Image and Signal Processing, J. Goutsias, L. Vincent, and D.S. Bloomberg (eds.), Kluwer, 2000, pp. 331-340
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  __metric__ … 距離関数
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2, class Metric >
	void distance_transform( const Array1 &in, Array2 &out, Metric __metric__, typename Array1::size_type thread_num = 0 )
	{
		typedef typename Array2::size_type  size_type;
		typedef typename Array2::value_type value_type;
		typedef _meijster_distance_transform_::distance_transform_thread< Array2, Metric > meijster_distance_transform_thread;

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

		meijster_distance_transform_thread *thread = new meijster_distance_transform_thread[ thread_num ];

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
			// Z軸方向の処理
			for( i = 0 ; i < thread_num ; i++ )
			{
				thread[ i ].setup_parameters( out, 2, i, thread_num );
			}

			do_threads_( thread, thread_num );
		}

		delete [] thread;
	}
}

/// @}
//  A. Meijster による距離変換グループの終わり



//! @addtogroup manhattan_distance_group マンハッタン距離（シティーブロック距離）変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{


/// @brief マンハッタン距離（シティーブロック距離）変換
namespace manhattan
{
	/// @brief マンハッタン距離（シティーブロック距離）変換
	//! 
	//! 計算される距離は，マンハッタン距離（シティーブロック距離）となります．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! - 参考文献
	//!     - A. Meijster, J. Roerdink, and W. Hesselink, "A general algorithm for computing distance transforms in linear time," In: Mathematical Morphology and its Applications to Image and Signal Processing, J. Goutsias, L. Vincent, and D.S. Bloomberg (eds.), Kluwer, 2000, pp. 331-340
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void distance_transform( const Array1 &in, Array2 &out, typename Array1::size_type thread_num = 0 )
	{
		_meijster_distance_transform_::MDT metric;
		meijster::distance_transform( in, out, metric, thread_num );
	}
}


/// @brief マンハッタン距離（シティーブロック距離）変換
namespace cityblock
{
	/// @brief マンハッタン距離（シティーブロック距離）変換
	//! 
	//! 計算される距離は，マンハッタン距離（シティーブロック距離）となります．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! - 参考文献
	//!     - A. Meijster, J. Roerdink, and W. Hesselink, "A general algorithm for computing distance transforms in linear time," In: Mathematical Morphology and its Applications to Image and Signal Processing, J. Goutsias, L. Vincent, and D.S. Bloomberg (eds.), Kluwer, 2000, pp. 331-340
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void distance_transform( const Array1 &in, Array2 &out, typename Array1::size_type thread_num = 0 )
	{
		_meijster_distance_transform_::MDT metric;
		meijster::distance_transform( in, out, metric, thread_num );
	}
}

/// @}
//  マンハッタン距離（シティーブロック距離）変換



//! @addtogroup chessboard_distance_group チェスボード距離距離変換
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/distance.h>
//! @endcode
//!
//!  @{

/// @brief チェスボード距離変換
namespace chessboard
{
	/// @brief チェスボード距離変換
	//! 
	//! 計算される距離は，チェスボード距離となります．
	//! 
	//! @attention 入力と出力は，同じMISTコンテナオブジェクトでも正しく動作する
	//! @attention スレッド数に0を指定した場合は，使用可能なCPU数を自動的に取得する
	//!
	//! - 参考文献
	//!     - A. Meijster, J. Roerdink, and W. Hesselink, "A general algorithm for computing distance transforms in linear time," In: Mathematical Morphology and its Applications to Image and Signal Processing, J. Goutsias, L. Vincent, and D.S. Bloomberg (eds.), Kluwer, 2000, pp. 331-340
	//! 
	//! @param[in]  in         … 入力画像
	//! @param[out] out        … 出力画像
	//! @param[in]  thread_num … 使用するスレッド数
	//! 
	template < class Array1, class Array2 >
	void distance_transform( const Array1 &in, Array2 &out, typename Array1::size_type thread_num = 0 )
	{
		_meijster_distance_transform_::CDT metric;
		meijster::distance_transform( in, out, metric, thread_num );
	}
}

/// @}
//  チェスボード距離距離変換



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
		temp[ i ] = -1;
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
								if( p < 0 || in[ p ] < rr )
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
