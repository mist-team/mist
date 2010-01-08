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

/// @file mist/random.h
//!
//! @brief MT(Mersenne Twister)法による乱数発生を扱うためのライブラリ
//!
#ifndef __INCLUDE_MIST_RANDOM__
#define __INCLUDE_MIST_RANDOM__

#include "mist.h"
#include "matrix.h"
#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN


//! @defgroup random_group 擬似乱数の生成
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/random.h>
//! @endcode
//!
//!  @{


namespace __random__
{
	static const unsigned long	n				= 624;					/// number n
	static const unsigned long	m				= 397;					/// number m
	static const unsigned long	vec_a			= 0x9908b0dfUL;			/// constant vector a
	static const unsigned long	upper			= 0x80000000UL;			/// most significant w-r bits
	static const unsigned long	lower			= 0x7fffffffUL;			/// least significant r bits
	static const double			pai_timed_by_2	= 6.283185307179586;	/// pai timed by two
}


/// @brief MT(Mersenne Twister)法による一様乱数
namespace uniform
{

	/// @brief MT(Mersenne Twister)法による一様乱数ジェネレータ
	//! 
	//! 長周期, 高次元均等分布を持つ擬似乱数を生成する．
	//! 周期が2^19937-1で、623次元超立方体の中に 均等に分布することが証明されている．
	//! 
	//! このクラスは，MT法開発者によりCで書かれた乱数生成関数群(MT19937)をクラス化したものである．
	//! 
	//! MT法(MT19937)開発者のページ：
	//! http://www.math.keio.ac.jp/~matumoto/mt.html
	//!
	class random
	{
		// Period parameters
		
		// static unsigned long vec_[n_]; /* the array for the state vector  */
		// static int i_=n_+1; /* i_==n_+1 means vec_[n_] is not initialized */	

		array< unsigned long > vec_;		///< @brief the array for the state vector
		unsigned long i_;					///< @brief i_== __random__::n + 1 means vec_[] is not initialized


	public:


		/// @brief コンストラクタ
		//! 
		//! デフォルトコンストラクタ
		//! 
		random( ) : vec_( __random__::n ), i_( __random__::n + 1 )
		{
		}


		/// @brief コンストラクタ(seedで初期化)
		//! 
		//! 
		//! 
		//! @param[in] seed … 乱数のseed(これを用いてジェネレータの状態を初期化する)
		//! 
		explicit random( const unsigned long seed ) : vec_( __random__::n ), i_( __random__::n + 1 )
		{
			init( seed );
		}


		/// @brief コンストラクタ(seed_arrayで初期化)
		//! 
		//! 
		//! 
		//! @param[in] seed_array … 乱数のseed配列(これを用いてジェネレータの状態を初期化する)
		//! 
		explicit random( const array< unsigned long >& seed_array ) : vec_( __random__::n ), i_( __random__::n + 1 )
		{
			init( seed_array );
		}


		/// @brief seedで初期化
		//! 
		//! initializes vec_[n_] with a seed
		//! 
		//! @param[in] seed … 乱数のseed
		//! 
		void init( const unsigned long& seed )
		{
			vec_[ 0 ] = seed & 0xffffffffUL;

			for( i_ = 1 ; i_ < __random__::n ; i_ ++ )
			{
				vec_[ i_ ] = ( 1812433253UL * ( vec_[ i_ - 1 ] ^ ( vec_[ i_ - 1 ] >> 30 ) ) + i_ );
				/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
				/* In the previous versions, MSBs of the seed affect   */
				/* only MSBs of the array vec_[ ].                        */
				/* 2002/01/09 modified by Makoto Matsumoto             */
				vec_[ i_ ] &= 0xffffffffUL;
				/* for >32 bit machines */
			}
		}

		//
		/// @brief seed_arrayで初期化
		//! 
		//! initialize by an array with array-length
		//! seed_array is the array for initializing seeds
		//! array_length is its length
		//! 
		//! @param[in] seed_array … 乱数のseed配列
		//! 
		void init( const array< unsigned long >& seed_array )
		{
			unsigned long i, j, k;
			init( 19650218UL );
			i = 1;
			j = 0;	

			for( k = ( __random__::n > seed_array.size( ) ? __random__::n : static_cast< unsigned long >( seed_array.size( ) ) ) ; k > 0 ; k -- )
			{
				vec_[ i ] = ( vec_[ i ] ^ ( ( vec_[ i - 1 ] ^ ( vec_[ i - 1 ] >> 30 ) ) * 1664525UL ) ) + seed_array[ j ] + j;  /* non linear */
				vec_[ i ] &= 0xffffffffUL;  /* for WORDSIZE > 32 machines */
				i ++;
				j ++;

				if( i >= __random__::n )
				{
					vec_[ 0 ] = vec_[ __random__::n - 1 ];
					i = 1;
				}	

				if( j >= seed_array.size( ) )
				{
					j = 0;
				}
			}	

			for( k = __random__::n - 1 ; k > 0 ; k -- )
			{
				vec_[ i ] = ( vec_[ i ] ^ ( ( vec_[ i - 1 ] ^ ( vec_[ i - 1 ] >> 30 ) ) * 1566083941UL ) ) - i;  /* non linear */
				vec_[ i ] &= 0xffffffffUL;  /* for WORDSIZE > 32 machines */
				i++;

				if( i >= __random__::n )
				{
					vec_[ 0 ] = vec_[ __random__::n - 1 ];
					i = 1;
				}
			}

			vec_[ 0 ] = 0x80000000UL;  /* MSB is 1; assuring non-zero initial array */
		}

		/// @brief 32bit符号無し整数乱数の発生
		//! 
		//! generates a random number on [0,0xffffffff]-interval
		//! 
		//! @return 32bit符号無し整数乱数
		//! 
		unsigned long int32( )
		{
			unsigned long y;
			static unsigned long mag01[ 2 ] = { 0x0UL, __random__::vec_a };
			/* mag01[x] = x * __random__::vec_a  for x=0,1 */
	
			if( i_ >= __random__::n ) /* generate __random__::n words at one time */
			{
				unsigned long kk;	

				if( i_ == __random__::n + 1 ) /* if init_genrand() has not been called, */
				{
					init( 5489UL );  /* a default initial seed is used */
				}
				for( kk = 0 ; kk < __random__::n - __random__::m  ; kk ++ )
				{
					y = ( vec_[ kk ] & __random__::upper ) | ( vec_[ kk + 1 ] & __random__::lower );
					vec_[ kk ] = vec_[ kk + __random__::m  ] ^ ( y >> 1 ) ^ mag01[ y & 0x1UL ];
				}	

				for( ; kk < __random__::n - 1 ; kk ++ )
				{
					y = ( vec_[ kk ] & __random__::upper ) | ( vec_[ kk + 1 ] & __random__::lower );
					vec_[ kk ] = vec_[ kk + ( __random__::m  - __random__::n ) ] ^ ( y >> 1 ) ^ mag01[ y & 0x1UL ];
				}

				y = ( vec_[ __random__::n - 1 ] & __random__::upper ) | ( vec_[ 0 ] & __random__::lower );
				vec_[ __random__::n - 1 ] = vec_[ __random__::m  - 1 ] ^ ( y >> 1 ) ^ mag01[ y & 0x1UL ];	

				i_ = 0;
			}

			y = vec_[ i_ ++ ];

			/* Tempering */
			y ^= ( y >> 11 );
			y ^= ( y << 7 ) & 0x9d2c5680UL;
			y ^= ( y << 15 ) & 0xefc60000UL;
			y ^= ( y >> 18 );

			return y;
		}


		/// @brief 31bit符号無し整数乱数の発生
		//! 
		//! generates a random number on [0,0x7fffffff]-interval
		//! 
		//! @return 31bit符号無し整数乱数
		//! 
		unsigned long int31( )
		{
			return ( int32( ) >> 1 );
		}


		/// @brief [0,1]区間浮動小数点乱数の発生
		//! 
		//! generates a random number on [0,1]-real-interval
		//! 
		//! @return [0,1]区間浮動小数点乱数
		//! 
		double real1( )
		{
			return ( int32( ) * ( 1.0 / 4294967295.0 ) );
			/* divided by 2^32-1 */
		}


		/// @brief [0,1)区間浮動小数点乱数の発生
		//! 
		//! generates a random number on [0,1)-real-interval
		//! 
		//! @return [0,1)区間浮動小数点乱数
		//! 
		double real2( )
		{
			return ( int32( ) * ( 1.0 / 4294967296.0 ) );
			/* divided by 2^32 */
		}


		/// @brief (0,1)区間浮動小数点乱数の発生
		//! 
		//! generates a random number on (0,1)-real-interval
		//! 
		//! @return (0,1)区間浮動小数点乱数
		//! 
		double real3( )
		{
			return ( ( ( double ) int32( ) ) + 0.5 ) * ( 1.0 / 4294967296.0 );
			/* divided by 2^32 */
		}


		/// @brief [0,1)区間浮動小数点乱数の発生(53bit分解能)
		//! 
		//! generates a random number on [0,1) with 53-bit resolution
		//! 
		//! @return [0,1)区間浮動小数点乱数(53bit分解能)
		//! 
		double res53( )
		{
			const unsigned long a = int32( ) >> 5;
			const unsigned long b = int32( ) >> 6;

			return ( ( a * 67108864.0 + b ) * ( 1.0 / 9007199254740992.0 ) );
		}


		/// @brief 0, 1, ... N-1 の符号無し整数乱数発生
		//! 
		//! @param[in] n … N の値
		//! 
		//! @return 0, 1, ... N-1 の符号無し整数乱数発生
		//! 
		unsigned long  generate( const unsigned int n = 0xffffffffUL )
		{
			return ( static_cast< unsigned long >( real2( ) * n ) );
		}


		/// @brief 0, 1, ... N-1 の符号無し整数乱数発生
		//! 
		//! @param[in] n … N の値
		//! 
		//! @return 0, 1, ... N-1 の符号無し整数乱数発生
		//! 
		unsigned long  operator( )( const unsigned int n = 0xffffffffUL )
		{
			return generate( n );
		}

	};

} // uniform



/// @brief 正規乱数のジェネレータ
namespace gauss
{
	/// @brief 正規乱数のジェネレータ
	//! 
	//! 平均値と分散を指定し，一様乱数ジェネレータを用いて正規乱数を発生させるクラス．
	//! 
	class random 
	{
		uniform::random u_rand_;	///< @brief 一様乱数ジェネレータ

		double mean_;					///< @brief 生成する正規乱数の平均値
		double standard_deviation_;		///< @brief 生成する正規乱数の標準偏差

	public:


		/// @brief デフォルトコンストラクタ
		//! 
		//! 
		random( ) :
			u_rand_( ),
			mean_( 0.0 ), 
			standard_deviation_( 1.0 )
		{
		}

		/// @brief コンストラクタ
		//! 
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] variance … 正規乱数の分散
		//! 
		random( const double mean, const double variance ) :
			u_rand_( ),
			mean_( mean ), 
			standard_deviation_( std::sqrt( variance ) )
		{
		}

		/// @brief コンストラクタ
		//! 
		//! @param[in] seed … u_rand_のseed(これを用いてジェネレータの状態を初期化する)
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] variance … 正規乱数の分散
		//! 
		random( const unsigned long seed, const double mean = 0.0, const double variance = 1.0 ) :
			u_rand_( seed ),
			mean_( mean ), 
			standard_deviation_( std::sqrt( variance ) )
		{
		}

		/// @brief コンストラクタ
		//! 
		//! @param[in] seed_array … u_rand_のseed配列
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] variance … 正規乱数の分散
		//! 
		random( const array< unsigned long >& seed_array, const double mean = 0.0, const double variance = 1.0 ) :
			u_rand_( seed_array ),
			mean_( mean ), 
			standard_deviation_( std::sqrt( variance ) )
		{
		}

		/// @brief seedで初期化
		//! 
		//! initializes vec_[n_] with a seed
		//! 
		//! @param[in] seed … u_rand_のseed(これを用いてseed配列を作る)
		//! 
		void init( const unsigned long& seed )
		{
			u_rand_.init( seed );
		}

		//
		/// @brief seed_arrayで初期化
		//! 
		//! initialize by an array with array-length
		//! seed_array is the array for initializing seeds
		//! array_length is its length
		//! 
		//! @param[in] seed_array … u_rand_のseed配列
		//! 
		void init( const array< unsigned long >& seed_array )
		{
			u_rand_.init( seed_array );
		}

		/// @brief 正規乱数のパラメータ指定
		//! 
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] variance … 正規乱数の分散
		//! 
		void set_param( const double& mean = 0.0, const double& variance = 1.0 )
		{
			mean_ = mean;
			standard_deviation_ = std::sqrt( variance );
		}
		
		/// @brief 指定された平均・分散の正規乱数を生成
		//! 
		//! @return 生成された正規乱数
		//! 
		double generate( )
		{
			return( standard_deviation_ * std::sqrt( -2.0 * std::log( 1.0 - u_rand_.real2( ) ) ) * std::cos( __random__::pai_timed_by_2 * ( 1.0 - u_rand_.real2( ) ) ) + mean_ );
		}

		/// @brief 指定された平均・分散の正規乱数を生成
		//! 
		//! @return 生成された正規乱数
		//! 
		double operator( )( )
		{
			return generate( );
		}

	};

} // gauss


/// @brief 多変量正規乱数のジェネレータ
namespace multivariate_gauss
{
	/// @brief 多変量正規乱数のジェネレータ
	//! 
	//! 平均ベクトルと共分散行列を指定し，正規乱数ジェネレータを用いて多変量正規乱数（ベクトル）を発生させるクラス．
	//! 
	class random 
	{
	private:
		gauss::random g_rand_;	///< @brief 正規乱数ジェネレータ >

		matrix< double > mean_;			///< @brief 生成する正規乱数の平均ベクトル
		matrix< double > l_triangle_;		///< @brief 生成する正規乱数の共分散行列

		const matrix< double > choleski( const matrix< double > &mat1 )
		{
			// coded by h.ishida
			matrix< double > mat2( mat1.rows( ), mat1.cols( ) );
			size_t i, j, m, k = mat1.cols( );

			mat2( 0, 0 ) = std::sqrt( mat1( 0, 0 ) );

			for( i = 0 ; i < k ; i ++ )
			{
				mat2( i, 0 ) = mat1( i, 0 ) / mat2( 0, 0 );
			}

			for( i = 1 ; i < k ; i ++ )
			{
				double s = 0.0;
				for( j = 0 ; j < i ; j ++ )
				{
					s += mat2( i, j ) * mat2( i, j );
				}

				mat2( i, i ) = sqrt( mat1( i, i ) - s );
				for( j = 1 ; j < i ; j ++ )
				{
					s = 0.0;
					for( m = 0 ; m < j ; m ++ )
					{
						s += mat2( i, m ) * mat2( j, m );
					}
					mat2( i, j ) = ( mat1( i, j ) - s ) / mat2( j, j );
					mat2( j, i ) = 0.0;
				}
			}

			return( mat2 );
		}

	public:


		/// @brief デフォルトコンストラクタ
		//! 
		//! 
		random( ) :
			g_rand_( ),
			mean_( matrix< double >( ) ), 
			l_triangle_( matrix< double >( ) )
		{
		}

		/// @brief コンストラクタ
		//! 
		//! @param[in] mean … 多変量正規乱数の平均ベクトル
		//! @param[in] covariance … 多変量正規乱数の共分散行列
		//! 
		random( const matrix< double > &mean, const matrix< double > &covariance ) :
			g_rand_( ),
			mean_( mean ), 
			l_triangle_( choleski( covariance ) )
		{
		}

		/// @brief コンストラクタ
		//! 
		//! @param[in] seed … g_rand_のseed(これを用いてジェネレータの状態を初期化する)
		//! @param[in] mean … 多変量正規乱数の平均ベクトル
		//! @param[in] covariance … 多変量正規乱数の共分散行列
		//! 
		random( const unsigned long seed, const matrix< double > &mean, const matrix< double > &covariance ) :
			g_rand_( seed ),
			mean_( mean ), 
			l_triangle_( choleski( covariance ) )
		{
		}

		/// @brief コンストラクタ
		//! 
		//! @param[in] seed_array … g_rand_のseed配列
		//! @param[in] mean … 多変量正規乱数の平均ベクトル
		//! @param[in] covariance … 多変量正規乱数の共分散行列
		//! 
		random( const array< unsigned long >& seed_array, const matrix< double > &mean, const matrix< double > &covariance ) :
			g_rand_( seed_array ),
			mean_( mean ), 
			l_triangle_( choleski( covariance ) )
		{
		}

		/// @brief seedで初期化
		//! 
		//! initializes vec_[n_] with a seed
		//! 
		//! @param[in] seed … g_rand_のseed(これを用いてseed配列を作る)
		//! 
		void init( const unsigned long& seed )
		{
			g_rand_.init( seed );
		}

		/// @brief 正規乱数のパラメータ指定
		//! 
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] variance … 正規乱数の分散
		//! 
		void set_param( const matrix< double > &mean, const matrix< double > &covariance )
		{
			mean_ = mean, 
			l_triangle_ = choleski( covariance );
		}

		/// @brief 指定された平均・標準偏差の正規乱数を生成
		//! 
		//! @return 生成された正規乱数
		//! 
		matrix< double > generate( )
		{
			matrix< double > r_vec( mean_.rows( ), mean_.cols( ) );
			for( size_t i = 0 ; i < r_vec.size( ) ; i ++ )
			{
				r_vec[ i ] = g_rand_.generate( ); 
			}
			return( mean_ + l_triangle_ * r_vec );
		}

		/// @brief 指定された平均・標準偏差の正規乱数を生成
		//! 
		//! @return 生成された正規乱数
		//! 
		matrix< double > operator( )( )
		{
			return generate( );
		}
	};


} // multivariate_gauss

/// @}
//  擬似乱数の生成の終わり


// mist名前空間の終わり
_MIST_END

/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.
 
   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).
 
   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
 
     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
 
     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
 
     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.
 
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#endif // __INCLUDE_MIST_RANDOM__

