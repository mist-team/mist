#ifndef __INCLUDE_RANDOM__
#define __INCLUDE_RANDOM__

#include <mist.h>
#include <cmath>
#include <utility>

// mist名前空間の始まり
_MIST_BEGIN


//MT(Mersenne Twister)法による乱数発生

class random
{

	const double pai_timed_by_2_;

	/* Period parameters */
	const unsigned long number_n_;
	const unsigned long number_m_;
	const unsigned long matrix_a_;    /* constant vector a */
	const unsigned long upper_mask_;  /* most significant w-r bits */
	const unsigned long lower_mask_;  /* least significant r bits */

	//static unsigned long mt_[number_n_]; /* the array for the state vector  */
	//static int mti_=number_n_+1; /* mti_==number_n_+1 means mt_[number_n_] is not initialized */
	array< unsigned long > mt_;
	int mti_;


public:

	//コンストラクタ
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

	//コンストラクタ(seedで初期化)
	random( const unsigned long seed ) :
		pai_timed_by_2_( 6.283185307179586 ),
		number_n_( 624 ),
		number_m_( 397 ),
		matrix_a_( 0x9908b0dfUL ),
		upper_mask_( 0x80000000UL ),
		lower_mask_( 0x7fffffffUL ),
		mt_( number_n_ ),
		mti_( number_n_ + 1 )
	{
		init_genrand( seed );
	}

	//コンストラクタ(init_keyで初期化)
	random( const array< unsigned long > init_key ) :
		pai_timed_by_2_( 6.283185307179586 ),
		number_n_( 624 ),
		number_m_( 397 ),
		matrix_a_( 0x9908b0dfUL ),
		upper_mask_( 0x80000000UL ),
		lower_mask_( 0x7fffffffUL ),
		mt_( number_n_ ),
		mti_( number_n_ + 1 )
	{
		init_by_array( init_key );
	}

	//seedで初期化
	/* initializes mt_[number_n_] with a seed */
	void init_genrand( const unsigned long seed )
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

	//init_keyで初期化
	/* initialize by an array with array-length */
	/* init_key is the array for initializing keys */
	/* key_length is its length */
	void init_by_array( const array< unsigned long > init_key )
	{
		int i, j, k;
		init_genrand( 19650218UL );
		i = 1;
		j = 0;

		for( k = ( number_n_ > init_key.size( ) ? number_n_ : init_key.size( ) ) ; k > 0 ; k-- )
		{
			mt_[ i ] = ( mt_[ i ] ^ ( ( mt_[ i - 1 ] ^ ( mt_[ i - 1 ] >> 30 ) ) * 1664525UL ) ) + init_key[ j ] + j;  /* non linear */
			mt_[ i ] &= 0xffffffffUL;  /* for WORDSIZE > 32 machines */
			i++;
			j++;

			if( i >= number_n_ )
			{
				mt_[ 0 ] = mt_[ number_n_ -1 ];
				i = 1;
			}

			if( j >= init_key.size( ) )
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

	//32bit符号無し整数乱数の発生
	/* generates a random number on [0,0xffffffff]-interval */
	const unsigned long genrand_int32( )
	{
		unsigned long y;
		static unsigned long mag01[ 2 ] = {0x0UL, matrix_a_};
		/* mag01[x] = x * matrix_a_  for x=0,1 */

		if( mti_ >= number_n_ ) /* generate number_n_ words at one time */
		{
			int kk;

			if( mti_ == number_n_ + 1 ) /* if init_genrand() has not been called, */
			{
				init_genrand( 5489UL );  /* a default initial seed is used */
			}
			for( kk = 0 ; kk < number_n_ -number_m_ ; kk++ )
			{
				y = ( mt_[ kk ] & upper_mask_ ) | ( mt_[ kk + 1 ] & lower_mask_ );
				mt_[ kk ] = mt_[ kk + number_m_ ] ^ ( y >> 1 ) ^ mag01[ y & 0x1UL ];
			}

			for( ; kk < number_n_ -1 ; kk++ )
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

	//31bit符号無し整数乱数の発生
	/* generates a random number on [0,0x7fffffff]-interval */
	const long genrand_int31( )
	{
		return ( genrand_int32( ) >> 1 );
	}

	//[0,1]区間浮動小数点乱数の発生
	/* generates a random number on [0,1]-real-interval */
	const double genrand_real1( )
	{
		return ( genrand_int32( ) * ( 1.0 / 4294967295.0 ) );
		/* divided by 2^32-1 */
	}

	//[0,1)区間浮動小数点乱数の発生
	/* generates a random number on [0,1)-real-interval */
	const double genrand_real2( )
	{
		return ( genrand_int32( ) * ( 1.0 / 4294967296.0 ) );
		/* divided by 2^32 */
	}

	//(0,1)区間浮動小数点乱数の発生
	/* generates a random number on (0,1)-real-interval */
	const double genrand_real3( )
	{
		return ( ( ( double ) genrand_int32( ) ) + 0.5 ) * ( 1.0 / 4294967296.0 );
		/* divided by 2^32 */
	}

	//[0,1)区間浮動小数点乱数の発生(53bit分解能)
	/* generates a random number on [0,1) with 53-bit resolution*/
	const double genrand_res53( )
	{
		const unsigned long a = genrand_int32( ) >> 5;
		const unsigned long b = genrand_int32( ) >> 6;

		return ( ( a * 67108864.0 + b ) * ( 1.0 / 9007199254740992.0 ) );
	}

	/* These real versions are due to Isaku Wada, 2002/01/09 added */


	//正規乱数発生(Box-Muller法)

	//2つの一様乱数から1つの正規乱数を生成
	const double genrand_gauss( const double av = 0.0 , const double st = 1.0 )
	{
		const double r1 = genrand_real2( );
		const double r2 = genrand_real2( );

		return ( st * sqrt( -2.0 * log( r1 ) ) * cos( pai_timed_by_2_ * r2 ) + av );
	}

	//2つの一様乱数から2つの正規乱数を生成
	const std::pair< double, double > genrand_gauss_pair( const double av = 0.0 , const double st = 1.0 )
	{
		const double r1 = genrand_real2( );
		const double r2 = genrand_real2( );
		const double z1 = st * sqrt( -2.0 * log( r1 ) ) * cos( pai_timed_by_2_ * r2 ) + av;
		const double z2 = st * sqrt( -2.0 * log( r1 ) ) * sin( pai_timed_by_2_ * r2 ) + av;

		return ( std::make_pair( z1, z2 ) );
	}

};

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
