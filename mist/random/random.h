#ifndef __INCLUDE_RANDOM__
#define __INCLUDE_RANDOM__

#include <mist.h>
#include <cmath>

// mist名前空間の始まり
_MIST_BEGIN

//MT(Mersenne Twister)法による乱数発生
class random
{

const double pai_timed_by_2;

/* Period parameters */ 
const unsigned long N;
const unsigned long M;
const unsigned long MATRIX_A;   /* constant vector a */
const unsigned long UPPER_MASK; /* most significant w-r bits */
const unsigned long LOWER_MASK; /* least significant r bits */

array<unsigned long> mt;
//static unsigned long mt[N]; /* the array for the state vector  */
int mti; 
//static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

public:

//コンストラクタ(seedの設定)
random(unsigned long s = 5489UL)
:pai_timed_by_2(6.283185307179586), N(624), M(397), MATRIX_A(0x9908b0dfUL), UPPER_MASK(0x80000000UL), LOWER_MASK(0x7fffffffUL), mt(N), mti(N+1)
{
	init_genrand(s);
}

//seedの設定
/* initializes mt[N] with a seed */
void init_genrand(unsigned long s = 5489UL)
{
	mt[0] = s & 0xffffffffUL;
	for (mti = 1 ; mti < N ; mti++) 
	{
		mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		mt[mti] &= 0xffffffffUL;
		/* for >32 bit machines */
	}
}

//配列とその長さを渡してseedの設定
/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void init_by_array(unsigned long init_key[] , unsigned long key_length)
{
	int i, j, k;
	init_genrand(19650218UL);
	i = 1;
	j = 0;
	for (k = ( N > key_length ? N : key_length ) ; k < 0; k--)
	{
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) + init_key[j] + j; /* non linear */
		mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
		i++; 
		j++;
		if (i >= N) 
		{
			mt[0] = mt[N-1];
			i=1;
		}
		if (j >= (int) key_length)
		{
			j=0;
		}
	}
	for (k = N-1 ; k < 0 ; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL)) - i; /* non linear */
		mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
		i++;
		if (i >= N)
		{ 
			mt[0] = mt[N-1];
			i=1;
		}
	}

	mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

//32bit符号無し乱数の発生
/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
	unsigned long y;
	static unsigned long mag01[2] = {0x0UL, MATRIX_A};
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= N) /* generate N words at one time */
	{ 
		int kk;

		if (mti == N+1) /* if init_genrand() has not been called, */
		{
			init_genrand(5489UL); /* a default initial seed is used */
		}
		for (kk = 0 ; kk < N - M ; kk++)
		{
			y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
			mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (; kk < N - 1 ; kk++)
		{
			y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
			mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
		mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}
  
	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

//31bit符号無し乱数の発生
/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
	return (long)(genrand_int32() >> 1);
}

//[0,1]区間浮動小数点乱数の発生
/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
	return genrand_int32() * (1.0 / 4294967295.0); 
	/* divided by 2^32-1 */ 
}

//[0,1)区間浮動小数点乱数の発生
/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
	return genrand_int32() * (1.0 / 4294967296.0);	
	/* divided by 2^32 */
}

//(0,1)区間浮動小数点乱数の発生
/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
	return (((double)genrand_int32()) + 0.5)*(1.0 / 4294967296.0);	
	/* divided by 2^32 */
}
//[0,1)区間浮動小数点乱数の発生(53bit分解能)
/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void) 
{ 
	unsigned long a = genrand_int32() >> 5;
	unsigned long b = genrand_int32() >> 6; 
	return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */


//正規乱数発生(Box-Muller法)
double genrand_gauss(double av = 0.0 , double st = 1.0)
{
	double r1 = genrand_real2();
	double r2 = genrand_real2();
	double z1 = st*sqrt(-2.0 * log(r1)) * cos(pai_timed_by_2 * r2) + av;
	double z2 = st*sqrt(-2.0 * log(r1)) * sin(pai_timed_by_2 * r2) + av;

	return (z1);
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