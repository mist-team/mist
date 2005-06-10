/// @file mist/filter/thinning.h
//!
//! @brief 各次元の画像に対応した，細線化アルゴリズム
//!
//! - 参考文献
//!   - 鳥脇純一郎, ``画像理解のためのディジタル画像処理〔I〕〔II〕，'' 昭晃堂，1988
//!   - 鈴木智, 阿部圭一, ``距離変換の結果を利用した二値画像の逐次細線化,'' 電子情報通信学会論文誌D, vol.68-D, no.4, pp.473-480, 1985
//!

#ifndef __INCLUDE_MIST_THINNING__
#define __INCLUDE_MIST_THINNING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "distance.h"
#endif

// mist名前空間の始まり
_MIST_BEGIN


namespace __thinning_controller__
{
	template < class T, class Allocator >
	inline void val9( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
					  typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
	{
		p[ 0 ] = in( i , j );
		p[ 1 ] = in( i + 1, j );
		p[ 2 ] = in( i + 1, j - 1 );
		p[ 3 ] = in( i , j - 1 );
		p[ 4 ] = in( i - 1, j - 1 );
		p[ 5 ] = in( i - 1, j );
		p[ 6 ] = in( i - 1, j + 1 );
		p[ 7 ] = in( i , j + 1 );
		p[ 8 ] = in( i + 1, j + 1 );
	}

	template < class T > inline T pixel( T p )
	{
		return ( ( p > 0 ) ? 1 : 0 );
	}

	template < class T > inline T pixel( T p[ 9 ], int index )
	{
		if( index <= 0 )
		{
			return ( pixel( p[ index + 8 ] ) );
		}
		else if( index >= 9 )
		{
			return ( pixel( p[ index - 8 ] ) );
		}
		else
		{
			return ( pixel( p[ index ] ) );
		}
	}

	template < class T > inline T pixel_( T p[ 9 ], int index )
	{
		return ( 1 - pixel( p, index ) );
	}

	template < class T > inline int Nc4( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel( p, i ) - pixel( p, i ) * pixel( p, i + 1 ) * pixel( p, i + 2 );
		}

		return ( ret );
	}

	template < class T > inline int Nc8( T p[ 9 ] )
	{
		T ret = 0;

		for( int i = 1 ; i < 9 ; i += 2 )
		{
			ret += pixel_( p, i ) - pixel_( p, i ) * pixel_( p, i + 1 ) * pixel_( p, i + 2 );
		}

		return ( ret );
	}

	template < class T, class Allocator >
	void thinning( array2< T, Allocator > &ia )
	{
		typedef typename array2< T, Allocator >::size_type size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		bool flag;
		size_type w = ia.width( );
		size_type h = ia.height( );
		size_type i, j;
		char p[ 9 ], q[ 9 ], value;
		array2< char > ic( w, h );
		array2< char > id( w, h );

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			id[ i ] = ia[ i ] == 0 ? 0 : 1;
		}

		flag = true;

		while( flag )
		{
			flag = false;
			ic = id;

			for( j = 1 ; j < h - 1 ; j++ )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9( ic, p, i, j );
					val9( id, q, i, j );

					if( pixel( p[ 0 ] ) != 1 )
					{
						continue;
					}

					// 条件1 = 入力画像Fにおいて，x0の4近傍に少なくとも1個の0画素が存在する
					if( p[ 1 ] == 1 && p[ 3 ] == 1 && p[ 5 ] == 1 && p[ 7 ] == 1 )
					{
						continue;
					}

					// 条件2 = 入力画像Fにおいて，x0の8近傍に2個以上の1画素がある(x0が端点でない)
					value = pixel( p[ 1 ] ) + pixel( p[ 2 ] ) + pixel( p[ 3 ] ) + pixel( p[ 4 ] ) + pixel( p[ 5 ] ) + pixel( p[ 6 ] ) + pixel( p[ 7 ] ) + pixel( p[ 8 ] );

					if( value <= 1 )
					{
						continue;
					}

					// 条件3 = 作業画像F'において，-1の画素も0画素とみなした時，x0が孤立点ではない
					value = pixel( q[ 1 ] ) + pixel( q[ 2 ] ) + pixel( q[ 3 ] ) + pixel( q[ 4 ] ) + pixel( q[ 5 ] ) + pixel( q[ 6 ] ) + pixel( q[ 7 ] ) + pixel( q[ 8 ] );

					if( value == 0 )
					{
						continue;
					}

					// 条件4 = 入力画像Fにおいて，x0におけるNc[8]の連結数が1である
					if( Nc8( p ) != 1 )
					{
						continue;
					}

					// 条件5 = i=3,5に対して次の事柄が成り立つ
					//         xiがちょうどこの反復で1から0に変えられてはいない(xiが-1になっていない)か，
					//         または，それが-1になっていても，-1を全て0とみなして計算したx0の連結数Nc[8]が1に等しい
					if( q[ 3 ] == -1 || q[ 5 ] == -1 )
					{
						if( Nc8( q ) != 1 )
						{
							continue;
						}
					}

					// 全ての条件を満たした画素は-1にする
					id( i, j ) = -1;

					flag = true;
				}
			}

			for( i = 0 ; i < id.size( ) ; i++ )
			{
				id[ i ] = id[ i ] == -1 ? 0 : id[ i ];
			}
		}

		for( i = 0 ; i < ia.size( ) ; i++ )
		{
			ia[ i ] = id[ i ] > 0 ? 1 : 0;
		}
	}
}


//! @addtogroup thinning_group 細線化
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/thinning.h>
//! @endcode
//!
//!  @{


/// @brief 2次元画像に対する細線化アルゴリズム
//!
//! 細線化結果は8連結となる
//!
//! @param[in]  in  … 入力画像
//! @param[out] out … 出力画像
//!
template < class T1, class T2, class Allocator1, class Allocator2 >
void thinning( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
{
	typedef typename array2< T2, Allocator2 >::size_type size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] > 0 ? 1 : 0;
	}

	__thinning_controller__::thinning( out );
}


namespace euclidean
{
	namespace __utility__
	{
		template < class T, class Allocator >
		inline void val9_1( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
							typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
		{
			p[ 0 ] = in( i , j );
			p[ 1 ] = in( i + 1, j );
			p[ 2 ] = in( i + 1, j - 1 );
			p[ 3 ] = in( i , j - 1 );
			p[ 4 ] = in( i - 1, j - 1 );
			p[ 5 ] = in( i - 1, j );
			p[ 6 ] = in( i - 1, j + 1 );
			p[ 7 ] = in( i , j + 1 );
			p[ 8 ] = in( i + 1, j + 1 );
		}

		template < class T, class Allocator >
		inline void val9_2( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
							typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
		{
			p[ 0 ] = in( i , j );
			p[ 1 ] = in( i - 1, j );
			p[ 2 ] = in( i - 1, j + 1 );
			p[ 3 ] = in( i , j + 1 );
			p[ 4 ] = in( i + 1, j + 1 );
			p[ 5 ] = in( i + 1, j );
			p[ 6 ] = in( i + 1, j - 1 );
			p[ 7 ] = in( i , j - 1 );
			p[ 8 ] = in( i - 1, j - 1 );
		}

		template < class T, class Allocator >
		inline void val9_3( const array2< T, Allocator > &in, typename array2< T, Allocator >::value_type p[ 9 ],
							typename array2< T, Allocator >::size_type i, typename array2< T, Allocator >::size_type j )
		{
			p[ 0 ] = in( i , j );
			p[ 1 ] = in( i + 1, j );
			p[ 2 ] = in( i + 1, j + 1 );
			p[ 3 ] = in( i , j + 1 );
			p[ 4 ] = in( i - 1, j + 1 );
			p[ 5 ] = in( i - 1, j );
			p[ 6 ] = in( i - 1, j - 1 );
			p[ 7 ] = in( i , j - 1 );
			p[ 8 ] = in( i + 1, j - 1 );
		}

		template < class T > inline T max_pixel( T p[ 9 ] )
		{
			int max = 0;

			for( int i = 1 ; i < 9 ; i++ )
			{
				if( std::abs( p[ i ] ) > max )
				{
					max = std::abs( p[ i ] );
				}
			}

			return ( max );
		}

		template < class T > inline T plus_pixel_num( T p[ 9 ] )
		{
			int num = 0;

			for( int i = 1 ; i < 9 ; i++ )
			{
				if( p[ i ] > 0 )
				{
					num++;
				}
			}

			return ( num );
		}

		template < class T > inline T pixel( T p )
		{
			return ( ( p > 0 ) ? 1 : 0 );
		}

		template < class T > inline T pixel( T p[ 9 ], int index )
		{
			if( index <= 0 )
			{
				return ( pixel( p[ index + 8 ] ) );
			}
			else if( index >= 9 )
			{
				return ( pixel( p[ index - 8 ] ) );
			}
			else
			{
				return ( pixel( p[ index ] ) );
			}
		}

		template < class T > inline T pixel_( T p[ 9 ], int index )
		{
			return ( 1 - pixel( p, index ) );
		}

		template < class T > inline int Nc4( T p[ 9 ] )
		{
			T ret = 0;

			for( int i = 1 ; i < 9 ; i += 2 )
			{
				ret += pixel( p, i ) - pixel( p, i ) * pixel( p, i + 1 ) * pixel( p, i + 2 );
			}

			return ( ret );
		}

		template < class T > inline T Nc8( T p[ 9 ] )
		{
			T ret = 0;

			for( int i = 1 ; i < 9 ; i += 2 )
			{
				ret += pixel_( p, i ) - pixel_( p, i ) * pixel_( p, i + 1 ) * pixel_( p, i + 2 );
			}

			return ( ret );
		}

		/// @brief ユークリッド距離を用いた細線化の実装 by 林さん
		//!
		//! - 参考文献
		//!   - 鈴木智, 阿部圭一, ``距離変換の結果を利用した二値画像の逐次細線化,'' 電子情報通信学会論文誌D, vol.68-D, no.4, pp.473-480, 1985.
		//!
		template < class T, class Allocator >
		void thinning( array2< T, Allocator > &ia )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			size_type w = ia.width( );
			size_type h = ia.height( );
			size_type i, j;
			double p[ 9 ], q[ 9 ];
			array2< double > ic( w, h, ia.reso1( ), ia.reso2( ) );
			array2< double > id( w, h, ia.reso1( ), ia.reso2( ) );

			for( i = 0 ; i < ia.size( ) ; i++ )
			{
				id[ i ] = ia[ i ] == 0 ? 0 : 1;
			}

			//距離変換
			calvin::distance_transform( id, id );

			//細め処理　1回目
			ic = id;
			for( j = 1 ; j < h - 1 ; j++ )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9_1( ic, p, i, j );
					val9_1( id, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						continue;
					}

					if( p[ 3 ] > 0 && p[ 5 ] > 0 )
					{
						continue;
					}

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 1 ] && q[ 1 ] > 0 ) && ( ( p[ 2 ] > 0 ) || ( p[ 3 ] > 0 ) ) && ( ( q[ 7 ] > 0 ) || ( q[ 8 ] > 0 ) ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 7 ] && q[ 7 ] > 0 ) && ( ( p[ 5 ] > 0 ) || ( q[ 6 ] > 0 ) ) && ( ( q[ 8 ] > 0 ) || ( q[ 1 ] > 0 ) ) )
					{
						continue;
					}

					ic( i, j ) = - id( i, j );
				}
			}

			//細め処理　2回目
			id = ic;
			for( j = h - 2 ; j > 0 ; j-- )
			{
				for( i = w - 2 ; i > 0 ; i-- )
				{
					val9_2( id, p, i, j );
					val9_2( ic, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						continue;
					}

					if( p[ 3 ] > 0 && p[ 5 ] > 0 )
					{
						continue;
					}

					val9_1( id, p, i, j );

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					val9_2( id, p, i, j );

					if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 1 ] && q[ 1 ] > 0 ) && ( ( p[ 2 ] > 0 ) || ( p[ 3 ] > 0 ) ) && ( ( q[ 7 ] > 0 ) || ( q[ 8 ] > 0 ) ) )
					{
						continue;
					}

					if( ( q[ 0 ] > q[ 7 ] && q[ 7 ] > 0 ) && ( ( p[ 5 ] > 0 ) || ( q[ 6 ] >= q[ 0 ] ) ) && ( ( q[ 8 ] > 0 ) || ( q[ 1 ] > 0 ) ) )
					{
						continue;
					}

					id( i, j ) = - ic( i, j );
				}
			}

			//細め処理　3回目
			ic = id;
			for( j = h - 2 ; j > 0 ; j-- )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9_3( ic, p, i, j );
					val9_3( id, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						continue;
					}

					if( p[ 3 ] > 0 && p[ 5 ] > 0 )
					{
						continue;
					}

					val9_1( ic, p, i, j );

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					val9_3( ic, p, i, j );

					if( max_pixel( p ) <= std::abs( q[ 0 ] ) )
					{
						continue;
					}

					ic( i, j ) = - id( i, j );
				}
			}

			//後処理
			id = ic;
			for( j = 1 ; j < h - 1 ; j++ )
			{
				for( i = 1 ; i < w - 1 ; i++ )
				{
					val9_1( id, p, i, j );
					val9_1( ic, q, i, j );

					if( q[ 0 ] <= 0 )
					{
						id( i, j ) = 0;
						continue;
					}

					if( ( p[ 1 ] > 0 ) && ( p[ 3 ] > 0 ) && ( p[ 5 ] > 0 ) && ( p[ 7 ] > 0 ) )
					{
						continue;
					}

					if( Nc8( p ) != 1 )
					{
						continue;
					}

					if( plus_pixel_num( p ) < 2 )
					{
						continue;
					}

					if( plus_pixel_num( p ) == 2 && ( q[ 4 ] > 0 && p[ 4 ] == 0 ) )
					{
						p[ 4 ] = 1;

						if( Nc8( p ) != 1 )
						{
							continue;
						}
					}

					id( i, j ) = 0;
				}
			}

			for( i = 0 ; i < ia.size( ) ; i++ )
			{
				ia[ i ] = id[ i ] != 0 ? 1 : 0;
			}
		}
	}


	/// @brief ユークリッド距離を用いた2次元画像に対する細線化アルゴリズム
	//!
	//! 細線化結果は8連結となる
	//!
	//! @attention 入力と出力が同じ画像オブジェクトでも正しくラベリングすることが可能です
	//!
	//! @param[in]  in  … 入力画像
	//! @param[out] out … 出力画像
	//!
	template < class T1, class T2, class Allocator1, class Allocator2 >
	void thinning( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out )
	{
		typedef typename array2< T2, Allocator2 >::size_type size_type;
		typedef typename array2< T2, Allocator2 >::value_type value_type;

		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		for( size_type i = 0 ; i < in.size( ) ; i++ )
		{
			out[ i ] = in[ i ] > 0 ? 1 : 0;
		}

		__utility__::thinning( out );
	}
}


/// @}
//  細線化グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_THINNING__
