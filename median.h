#ifndef __INCLUDE_MIST_MEDIAN_FILTER__
#define __INCLUDE_MIST_MEDIAN_FILTER__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_THREAD__
#include "../thread.h"
#endif

#include <algorithm>


// mist名前空間の始まり
_MIST_BEGIN


// メディアンフィルタ
namespace __median_filter__
{
	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh : フィルタサイズ
	// min, max : 濃淡範囲
	template < class T, class Allocator >
	void median_filter( const array< T, Allocator > &in, array< T, Allocator > &out, typename array< T, Allocator >::size_type fw,
							typename array< T, Allocator >::value_type min, typename array< T, Allocator >::value_type max,
							typename array< T, Allocator >::size_type thread_id = 0, typename array< T, Allocator >::size_type thread_num = 1 )
	{
		typedef typename array< T, Allocator >::size_type  size_type;
		typedef typename array< T, Allocator >::value_type value_type;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a = 0, i, x, ri;
		signed int pth, th, lt_med;
		value_type med;

		size_type w = in.size( );
		size_type num = w / thread_num;

		i = num * thread_id;

		if( thread_id == thread_num - 1 )
		{
			// 一番最後のスレッドだけ割り当てる量を増やす
			num += w % thread_num;
		}

		size_type bw = fw / 2;

		value_type leftmost;
		value_type *sort = new value_type[ fw + 1 ];
		unsigned short *hist = new unsigned short[ range ];

		memset( hist, 0, sizeof( unsigned short ) * range );
		pth = 0;

		if( i < bw )
		{
			leftmost = in[ i ] - min;
			for( x = 0 ; x <= bw ; x++ )
			{
				sort[ pth++ ] = in[ x ] - min;
				hist[ in[ x ] - min ]++;
			}
		}
		else
		{
			leftmost = in[ i - bw ] - min;
			for( x = i - bw ; x <= i + bw ; x++ )
			{
				sort[ pth++ ] = in[ x ] - min;
				hist[ in[ x ] - min ]++;
			}
		}

		// 昇順に並べ替える
		std::sort( sort, sort + pth );
		pth--;

		th = pth / 2;
		med = sort[ th ];

		if( sort[ 0 ] == sort[ th ] )
		{
			lt_med = 0;
		}
		else
		{
			a = th;
			while( med <= sort[ --a ] );
			lt_med = static_cast< signed int >( a + 1 );
		}

		out[ i ] = med + min;

		size_type end = i + num;
		for( i = i + 1 ; i < end ; i++ )
		{
			ri = i + bw;

			if( bw < i )
			{
				hist[ leftmost ]--;
				pth--;
				if( leftmost < med )
				{
					lt_med--;
				}
			}

			if( ri < w )
			{
				hist[ in[ ri ] - min ]++;
				pth++;
				if( in[ ri ] - min < med )
				{
					lt_med++;
				}
			}

			if ( bw <= i )
			{
				leftmost = in[ i - bw ] - min;
			}

			th = pth / 2;
			if( lt_med <= th )
			{
				while( lt_med + hist[ med ] <= th )
				{
					lt_med += hist[ med ];
					med++;
				}
			}
			else
			{
				while( th < lt_med )
				{
					med--;
					lt_med -= hist[ med ];
				}
			}

			out[ i ] = med + min;
		}

		delete [] hist;
		delete [] sort;
	}

	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh : フィルタサイズ
	// min, max : 濃淡範囲
	template < class T, class Allocator >
	void median_filter( const array2< T, Allocator > &in, array2< T, Allocator > &out,
						typename array2< T, Allocator >::size_type fw, typename array2< T, Allocator >::size_type fh,
						typename array2< T, Allocator >::value_type min, typename array2< T, Allocator >::value_type max,
						typename array2< T, Allocator >::size_type thread_id = 0, typename array2< T, Allocator >::size_type thread_num = 1 )
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a = 0, i, j, x, y, ri, leftnum;
		signed int pth, th, lt_med;
		value_type med;

		size_type w = in.width( );
		size_type h = in.height( );

		size_type bw = fw / 2;
		size_type bh = fh / 2;

		value_type *leftmost = new value_type[ fh ];
		value_type *sort = new value_type[ fw * fh + 1 ];
		unsigned short *hist = new unsigned short[ range ];

		for( j = thread_id ; j < h ; j += thread_num )
		{
			i = 0;

			memset( hist, 0, sizeof( unsigned short ) * range );
			pth = 0;
			leftnum = 0;
			for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
			{
				leftmost[ leftnum++ ] = in( 0, y ) - min;
				for( x = 0 ; x <= bw ; x++ )
				{
					sort[ pth++ ] = in( x, y ) - min;
					hist[ in( x, y ) - min ]++;
				}
			}

			// 昇順に並べ替える
			std::sort( sort, sort + pth );
			pth--;

			th = pth / 2;
			med = sort[ th ];

			if( sort[ 0 ] == sort[ th ] )
			{
				lt_med = 0;
			}
			else
			{
				a = th;
				while( med <= sort[ --a ] );
				lt_med = static_cast< signed int >( a + 1 );
			}

			out( i, j ) = med + min;

			for( i = 1 ; i < w ; i++ )
			{
				ri = i + bw;

				if( bw < i )
				{
					for( a = 0 ; a < leftnum ; a++ )
					{
						hist[ leftmost[ a ] ]--;
						pth--;
						if( leftmost[ a ] < med )
						{
							lt_med--;
						}
					}
				}

				leftnum = 0;
				for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
				{
					if( ri < w )
					{
						hist[ in( ri, y ) - min ]++;
						pth++;
						if( in( ri, y ) - min < med )
						{
							lt_med++;
						}
					}

					if( bw <= i )
					{
						leftmost[ leftnum++ ] = in( i - bw, y ) - min;
					}
				}

				th = pth / 2;
				if( lt_med <= th )
				{
					while( lt_med + hist[ med ] <= th )
					{
						lt_med += hist[ med ];
						med++;
					}
				}
				else
				{
					while( th < lt_med )
					{
						med--;
						lt_med -= hist[ med ];
					}
				}

				out( i, j ) = med + min;
			}
		}

		delete [] hist;
		delete [] sort;
		delete [] leftmost;
	}



	// in  : 入力画像. 入力画像の画素値は min と max の間とする
	// out : 出力画像. 出力画像のメモリはあらかじめ割り当てられているものとする
	// fw, fh, fd : フィルタサイズ
	// min, max : 濃淡範囲
	template < class T, class Allocator >
	void median_filter( const array3< T, Allocator > &in, array3< T, Allocator > &out,
						typename array3< T, Allocator >::size_type fw, typename array3< T, Allocator >::size_type fh, typename array3< T, Allocator >::size_type fd,
						typename array3< T, Allocator >::value_type min, typename array3< T, Allocator >::value_type max,
						typename array3< T, Allocator >::size_type thread_id = 0, typename array3< T, Allocator >::size_type thread_num = 1 )
	{
		typedef typename array3< T, Allocator >::size_type  size_type;
		typedef typename array3< T, Allocator >::value_type value_type;

		size_type range = static_cast< size_type >( max - min + 1 );
		size_type a = 0, i, j, k, x, y, z, ri, leftnum;
		signed int pth, th, lt_med;
		value_type med;

		size_type w = in.width( );
		size_type h = in.height( );
		size_type d = in.depth( );

		size_type bw = fw / 2;
		size_type bh = fh / 2;
		size_type bd = fd / 2;

		value_type *leftmost = new value_type[ fh * fd ];
		value_type *sort = new value_type[ fw * fh * fd + 1 ];
		unsigned short *hist = new unsigned short[range];

		for( k = thread_id ; k < d ; k += thread_num )
		{
			for( j = 0 ; j < h ; j++ )
			{
				i = 0;

				memset( hist, 0, sizeof( unsigned short ) * range );
				pth = 0;
				leftnum = 0;
				for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
				{
					for(y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
					{
						leftmost[ leftnum++ ] = in( 0, y, z ) - min;
						for( x = 0 ; x <= bw ; x++ )
						{
							sort[ pth++ ] = in( x, y, z ) - min;
							hist[ in( x, y, z ) - min ]++;
						}
					}
				}

				// 昇順に並べ替える
				std::sort( sort, sort + pth );
				pth--;

				th = pth / 2;
				med = sort[ th ];

				if( sort[ 0 ] == sort[ th ] )
				{
					lt_med = 0;
				}
				else
				{
					a = th;
					while( med <= sort[ --a ] );
					lt_med = static_cast< signed int >( a + 1 );
				}

				out( i, j, k ) = med + min;

				for( i = 1 ; i < w ; i++ )
				{
					ri = i + bw;

					if( bw < i )
					{
						for( a = 0 ; a < leftnum ; a++ )
						{
							hist[ leftmost[ a ] ]--;
							pth--;
							if( leftmost[ a ] < med )
							{
								lt_med--;
							}
						}
					}

					leftnum = 0;
					for( z = k < bd ? 0 : k - bd ; z <= k + bd && z < d ; z++ )
					{
						for( y = j < bh ? 0 : j - bh ; y <= j + bh && y < h ; y++ )
						{
							if( ri < w )
							{
								hist[ in( ri, y, z ) - min ]++;
								pth++;
								if( in( ri, y, z ) - min < med )
								{
									lt_med++;
								}
							}

							if ( bw <= i )
							{
								leftmost[ leftnum++ ] = in( i - bw, y, z ) - min;
							}
						}
					}

					th = pth / 2;

					if( lt_med <= th )
					{
						while( lt_med + hist[ med ] <= th )
						{
							lt_med += hist[ med ];
							med++;
						}
					}
					else
					{
						while( th < lt_med )
						{
							med--;
							lt_med -= hist[ med ];
						}
					}

					out( i, j, k ) = med + min;
				}
			}
		}

		delete [] hist;
		delete [] sort;
		delete [] leftmost;
	}
}


// メディアンフィルタのスレッド実装
namespace __median_filter_controller__
{
	template < class T, class Allocator >
	void get_min_max( const array< T, Allocator > &in, typename array< T, Allocator >::value_type &min, typename array< T, Allocator >::value_type &max )
	{
		min = max = in[ 0 ];
		for( typename array< T, Allocator >::size_type i = 0 ; i < in.size( ) ; i++ )
		{
			if( min > in[ i ] )
			{
				min = in[ i ];
			}
			else if( max < in[ i ] )
			{
				max = in[ i ];
			}
		}
	}

	template < class T, class Allocator >
	void median_filter( const array< T, Allocator > &in, array< T, Allocator > &out,
						typename array< T, Allocator >::size_type fw, typename array< T, Allocator >::size_type fh, typename array< T, Allocator >::size_type fd,
						typename array< T, Allocator >::value_type min, typename array< T, Allocator >::value_type max,
						typename array< T, Allocator >::size_type thread_id, typename array< T, Allocator >::size_type thread_num )
	{
		__median_filter__::median_filter( in, out, fw, min, max, thread_id, thread_num );
	}

	template < class T, class Allocator >
	void median_filter( const array2< T, Allocator > &in, array2< T, Allocator > &out,
						typename array2< T, Allocator >::size_type fw, typename array2< T, Allocator >::size_type fh, typename array2< T, Allocator >::size_type fd,
						typename array2< T, Allocator >::value_type min, typename array2< T, Allocator >::value_type max,
						typename array2< T, Allocator >::size_type thread_id, typename array2< T, Allocator >::size_type thread_num )
	{
		__median_filter__::median_filter( in, out, fw, fh, min, max, thread_id, thread_num );
	}

	template < class T, class Allocator >
	void median_filter( const array3< T, Allocator > &in, array3< T, Allocator > &out,
						typename array3< T, Allocator >::size_type fw, typename array3< T, Allocator >::size_type fh, typename array3< T, Allocator >::size_type fd,
						typename array3< T, Allocator >::value_type min, typename array3< T, Allocator >::value_type max,
						typename array3< T, Allocator >::size_type thread_id, typename array3< T, Allocator >::size_type thread_num )
	{
		__median_filter__::median_filter( in, out, fw, fh, fd, min, max, thread_id, thread_num );
	}

	template < class T >
	class median_thread : public mist::thread_object< median_thread< T > >
	{
	public:
		typedef mist::thread_object< median_thread< T > > base;
		typedef typename base::thread_exit_type thread_exit_type;
		typedef typename T::size_type size_type;
		typedef typename T::value_type value_type;

	private:
		size_t thread_id_;
		size_t thread_num_;

		// 入出力用の画像へのポインタ
		const T *in_;
		T *out_;
		size_type fw_;
		size_type fh_;
		size_type fd_;

	public:
		void setup_parameters( const T &in, T &out, size_t fw, size_type fh, size_type fd, size_type thread_id, size_type thread_num )
		{
			in_  = &in;
			out_ = &out;
			fw_ = fw;
			fh_ = fh;
			fd_ = fd;
			thread_id_ = thread_id;
			thread_num_ = thread_num;
		}

		const median_thread& operator =( const median_thread &p )
		{
			if( &p != this )
			{
				base::operator =( p );
				thread_id_ = p.thread_id_;
				thread_num_ = p.thread_num_;
				in_ = p.in_;
				out_ = p.out_;
				fw_ = p.fw_;
				fh_ = p.fh_;
				fd_ = p.fd_;
			}
			return( *this );
		}

		median_thread( size_type id = 0, size_type num = 1 ) : thread_id_( id ), thread_num_( num ),
													in_( NULL ), out_( NULL ), fw_( 3 ), fh_( 3 ), fd_( 3 )
		{
		}
		median_thread( const median_thread &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ),
													in_( p.in_ ), out_( p.out_ ), fw_( p.fw_ ), fh_( p.fh_ ), fd_( p.fd_ )
		{
		}

	protected:
		// 継承した先で必ず実装されるスレッド関数
		virtual thread_exit_type thread_function( const median_thread &p )
		{
			value_type min = (*in_)[ 0 ];
			value_type max = (*in_)[ 0 ];
			get_min_max( *in_, min, max );
			median_filter( *in_, *out_, fw_, fh_, fd_, min, max, thread_id_, thread_num_ );
			return( true );
		}
	};
}


template < class T, class Allocator >
void median( const array< T, Allocator > &in, array< T, Allocator > &out, typename array< T, Allocator >::size_type fw, typename array< T, Allocator >::size_type thread_num = 0 )
{
	typedef typename array< T, Allocator >::size_type  size_type;
	typedef typename array< T, Allocator >::value_type value_type;
	typedef __median_filter_controller__::median_thread< array< T, Allocator > > median_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ) );

	median_thread *thread = new median_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, fw, 3, 3, i, thread_num );
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
void median( const array1< T, Allocator > &in, array1< T, Allocator > &out, typename array1< T, Allocator >::size_type fw, typename array1< T, Allocator >::size_type thread_num = 0 )
{
	typedef typename array1< T, Allocator >::size_type  size_type;
	typedef typename array1< T, Allocator >::value_type value_type;
	typedef __median_filter_controller__::median_thread< array1< T, Allocator > > median_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ) );
	out.reso1( in.reso1( ) );

	median_thread *thread = new median_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, fw, 3, 3, i, thread_num );
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
void median( const array2< T, Allocator > &in, array2< T, Allocator > &out,
				   typename array2< T, Allocator >::size_type fw, typename array2< T, Allocator >::size_type fh,
				   typename array2< T, Allocator >::size_type thread_num = 0 )
{
	typedef typename array2< T, Allocator >::size_type  size_type;
	typedef typename array2< T, Allocator >::value_type value_type;
	typedef __median_filter_controller__::median_thread< array2< T, Allocator > > median_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	median_thread *thread = new median_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, fw, fh, 3, i, thread_num );
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
void median( const array3< T, Allocator > &in, array3< T, Allocator > &out,
				   typename array3< T, Allocator >::size_type fw, typename array3< T, Allocator >::size_type fh, typename array3< T, Allocator >::size_type fd,
				   typename array3< T, Allocator >::size_type thread_num = 0 )
{
	typedef typename array3< T, Allocator >::size_type  size_type;
	typedef typename array3< T, Allocator >::value_type value_type;
	typedef __median_filter_controller__::median_thread< array3< T, Allocator > > median_thread;

	if( thread_num == 0 )
	{
		thread_num = static_cast< size_type >( get_cpu_num( ) );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	median_thread *thread = new median_thread[ thread_num ];

	size_type i;
	for( i = 0 ; i < thread_num ; i++ )
	{
		thread[ i ].setup_parameters( in, out, fw, fh, fd, i, thread_num );
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



// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_MEDIAN_FILTER__
