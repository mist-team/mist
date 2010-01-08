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

#include <iostream>
#include <mist/matrix.h>
#include <mist/thread.h>


struct parameter
{
	unsigned int thread_id_;
	unsigned int thread_num_;
};

void thread_function1( const parameter &p )
{
	if( ( p.thread_id_ % 2 ) != 0 )
	{
		//mist::lock l( "hoge" );
		for( int i = 0 ; i < 10 ; i++ )
		{
			printf( "%d", p.thread_id_ );
			mist::sleep( 20 );
		}
	}
	else
	{
		//mist::lock l( "hoge" );
		for( int i = 0 ; i < 10 ; i++ )
		{
			printf( "%d", p.thread_id_ );
			mist::sleep( 1 );
		}
	}
}

void thread_function2( )
{
	for( int i = 0 ; i < 20 ; i++ )
	{
		printf( "A" );
		mist::sleep( 1 );
	}
}

#define CREATE_THREAD_NUM	8

int main( int argc, char *argv[] )
{
	using namespace std;

	parameter param[ CREATE_THREAD_NUM ];
	mist::thread_handle t[ CREATE_THREAD_NUM ];

	// 標準的なスレッド呼び出しを利用したサンプル
	int i;
	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		param[ i ].thread_id_ = i + 1;
		param[ i ].thread_num_ = CREATE_THREAD_NUM;
	}

	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		t[ i ] = mist::create_thread( param[ i ], thread_function1 );
	}

	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		mist::wait_thread( t[ i ] );
	}

	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		mist::close_thread( t[ i ] );
	}

	std::cout << std::endl;
	std::cout << std::endl;

	// スレッドプールを利用したサンプル
	{
		mist::thread_pool pool( 3 );
		pool.execute( thread_function2 );
		pool.executes( thread_function1, param, CREATE_THREAD_NUM );
		pool.wait( );
		std::cout << std::endl;
	}

	return( 0 );
}
