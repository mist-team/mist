/// @file mist/random.h
//!
//! @brief MT(Mersenne Twister)法による乱数発生を扱うためのライブラリ
//!
#ifndef __INCLUDE_RANDOM__
#define __INCLUDE_RANDOM__

#include "mist.h"
#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN




/// @brief MT(Mersenne Twister)法による一様乱数
namespace uniform
{
	/// @brief MT(Mersenne Twister)法による一様乱数発生
	//! 
	//! 長周期, 高次元均等分布を持つ擬似乱数を生成する．
	//! 周期が2^19937-1で、623次元超立方体の中に 均等に分布することが証明されている．
	//! 
	//! 開発者のページ：
	//! http://www.math.keio.ac.jp/~matumoto/mt.html
	//!
	class random
	{

		// Period parameters
		const unsigned long number_n_;		///< @brief number n
		const unsigned long number_m_;		///< @brief number m
		const unsigned long matrix_a_;		///< @brief constant vector a
		const unsigned long upper_mask_;	///< @brief most significant w-r bits
		const unsigned long lower_mask_;	///< @brief least significant r bits

		// static unsigned long mt_[number_n_]; /* the array for the state vector  */
		// static int mti_=number_n_+1; /* mti_==number_n_+1 means mt_[number_n_] is not initialized */
		// array< unsigned long > mt_;	

		array< unsigned long > mt_;			///< @brief seed array
		unsigned long mti_;					///< @brief counter for seed array initialization


	public:

		const double pai_timed_by_2_;		///< @brief pai timed by two

		/// @brief コンストラクタ
		//! 
		//! デフォルトコンストラクタ
		//! 
		random( ) :
			pai_timed_by_2_( 6.283185307179586 ),
			number_n_( 624 ),
			number_m_( 397 ),
			matrix_a_( 0x9908b0dfUL ),
			upper_mask_( 0x80000000UL ),
			lower_mask_( 0x7fffffffUL ),
			mt_( number_n_ ),
			mti_( number_n_ + 1 )
		{
		}


		/// @brief コンストラクタ(seedで初期化)
		//! 
		//! 
		//! 
		//! @param[in] seed … 乱数のseed(これを用いてseed配列を作る)
		//! 
		random( const unsigned long& seed ) :
			pai_timed_by_2_( 6.283185307179586 ),
			number_n_( 624 ),
			number_m_( 397 ),
			matrix_a_( 0x9908b0dfUL ),
			upper_mask_( 0x80000000UL ),
			lower_mask_( 0x7fffffffUL ),
			mt_( number_n_ ),
			mti_( number_n_ + 1 )
		{
			init( seed );
		}


		/// @brief コンストラクタ(seed_arrayで初期化)
		//! 
		//! 
		//! 
		//! @param[in] seed_array … 乱数のseed配列
		//! 
		random( const array< unsigned long >& seed_array ) :
			pai_timed_by_2_( 6.283185307179586 ),
			number_n_( 624 ),
			number_m_( 397 ),
			matrix_a_( 0x9908b0dfUL ),
			upper_mask_( 0x80000000UL ),
			lower_mask_( 0x7fffffffUL ),
			mt_( number_n_ ),
			mti_( number_n_ + 1 )
		{
			init( seed_array );
		}


		/// @brief seedで初期化
		//! 
		//! initializes mt_[number_n_] with a seed
		//! 
		//! @param[in] seed … 乱数のseed(これを用いてseed配列を作る)
		//! 
		void init( const unsigned long& seed )
		{
			mt_[ 0 ] = seed & 0xffffffffUL;	

			for( mti_ = 1 ; mti_ < number_n_ ; mti_++ )
			{
				mt_[ mti_ ] = ( 1812433253UL * ( mt_[ mti_ -1 ] ^ ( mt_[ mti_ -1 ] >> 30 ) ) + mti_ );
				/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
				/* In the previous versions, MSBs of the seed affect   */
				/* only MSBs of the array mt_[].                        */
				/* 2002/01/09 modified by Makoto Matsumoto             */
				mt_[ mti_ ] &= 0xffffffffUL;
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

			for( k = ( number_n_ > seed_array.size( ) ? number_n_ : seed_array.size( ) ) ; k > 0 ; k-- )
			{
				mt_[ i ] = ( mt_[ i ] ^ ( ( mt_[ i - 1 ] ^ ( mt_[ i - 1 ] >> 30 ) ) * 1664525UL ) ) + seed_array[ j ] + j;  /* non linear */
				mt_[ i ] &= 0xffffffffUL;  /* for WORDSIZE > 32 machines */
				i++;
				j++;

				if( i >= number_n_ )
				{
					mt_[ 0 ] = mt_[ number_n_ -1 ];
					i = 1;
				}	

				if( j >= seed_array.size( ) )
				{
					j = 0;
				}
			}	

			for( k = number_n_ -1 ; k > 0 ; k-- )
			{
				mt_[ i ] = ( mt_[ i ] ^ ( ( mt_[ i - 1 ] ^ ( mt_[ i - 1 ] >> 30 ) ) * 1566083941UL ) ) - i;  /* non linear */
				mt_[ i ] &= 0xffffffffUL;  /* for WORDSIZE > 32 machines */
				i++;

				if( i >= number_n_ )
				{
					mt_[ 0 ] = mt_[ number_n_ - 1 ];
					i = 1;
				}
			}

			mt_[ 0 ] = 0x80000000UL;  /* MSB is 1; assuring non-zero initial array */
		}

		/// @brief 32bit符号無し整数乱数の発生
		//! 
		//! generates a random number on [0,0xffffffff]-interval
		//! 
		//! @return 32bit符号無し整数乱数
		//! 
		const unsigned long int32( )
		{
			unsigned long y;
			static unsigned long mag01[ 2 ] = { 0x0UL, matrix_a_ };
			/* mag01[x] = x * matrix_a_  for x=0,1 */
	
			if( mti_ >= number_n_ ) /* generate number_n_ words at one time */
			{
				unsigned long kk;	

				if( mti_ == number_n_ + 1 ) /* if init_genrand() has not been called, */
				{
					init( 5489UL );  /* a default initial seed is used */
				}
				for( kk = 0 ; kk < number_n_ -number_m_ ; kk++ )
				{
					y = ( mt_[ kk ] & upper_mask_ ) | ( mt_[ kk + 1 ] & lower_mask_ );
					mt_[ kk ] = mt_[ kk + number_m_ ] ^ ( y >> 1 ) ^ mag01[ y & 0x1UL ];
				}	

				for( ; kk < number_n_ - 1 ; kk++ )
				{
					y = ( mt_[ kk ] & upper_mask_ ) | ( mt_[ kk + 1 ] & lower_mask_ );
					mt_[ kk ] = mt_[ kk + ( number_m_ - number_n_ ) ] ^ ( y >> 1 ) ^ mag01[ y & 0x1UL ];
				}

				y = ( mt_[ number_n_ - 1 ] & upper_mask_ ) | ( mt_[ 0 ] & lower_mask_ );
				mt_[ number_n_ - 1 ] = mt_[ number_m_ - 1 ] ^ ( y >> 1 ) ^ mag01[ y & 0x1UL ];	

				mti_ = 0;
			}

			y = mt_[ mti_++ ];

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
		const unsigned long int31( )
		{
			return ( int32( ) >> 1 );
		}


		/// @brief [0,1]区間浮動小数点乱数の発生
		//! 
		//! generates a random number on [0,1]-real-interval
		//! 
		//! @return [0,1]区間浮動小数点乱数
		//! 
		const double real1( )
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
		const double real2( )
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
		const double real3( )
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
		const double res53( )
		{
			const unsigned long a = int32( ) >> 5;
			const unsigned long b = int32( ) >> 6;

			return ( ( a * 67108864.0 + b ) * ( 1.0 / 9007199254740992.0 ) );
		}

	};

} // uniform


/// @brief 正規乱数のジェネレータ
namespace gauss
{

	/// @brief 正規乱数のジェネレータ
	//! 
	//! 平均値と標準偏差を指定し，正規乱数を発生させるクラス．
	//! 
	class random : protected uniform::random 
	{
		typedef uniform::random base;

		double mean_;					///< @brief 生成する正規乱数の平均値
		double standard_deviation_;		///< @brief 生成する正規乱数の標準偏差

	public:

		/// @brief コンストラクタ
		//! 
		//! @param[in] seed … 乱数のseed(これを用いてseed配列を作る)
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] standard_deviation … 正規乱数の標準偏差
		//! 
		random( const unsigned long& seed = 0, const double& mean = 0.0, const double& standard_deviation = 1.0 ) :
			base( seed ),
			mean_( mean ), 
			standard_deviation_( standard_deviation )
		{
		}
		
		/// @brief コンストラクタ
		//! 
		//! @param[in] seed_array … 乱数のseed配列
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] standard_deviation … 正規乱数の標準偏差
		//! 
		random( const array< unsigned long >& seed_array, const double& mean = 0.0, const double& standard_deviation = 1.0 ) :
			base( seed_array ),
			mean_( mean ), 
			standard_deviation_( standard_deviation )
		{
		}

		/// @brief 正規乱数のパラメータ指定
		//! 
		//! @param[in] mean … 正規乱数の平均
		//! @param[in] standard_deviation … 正規乱数の標準偏差
		//! 
		void set_param( const double& mean = 0.0, const double& standard_deviation = 1.0 )
		{
			mean_ = mean;
			standard_deviation_ = standard_deviation;
		}

		/// @brief 指定された平均・標準偏差の正規乱数を生成
		//! 
		//! @return 生成された正規乱数
		//! 
		const double generate( )
		{
			return( standard_deviation_ * std::sqrt( -2.0 * std::log( 1.0 - real2( ) ) ) * std::cos( pai_timed_by_2_ * ( 1.0 - real2( ) ) ) + mean_ );
		}

	};

} // gauss

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
 
 
   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/

#endif // __INCLUDE_RANDOM__
