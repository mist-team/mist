#include <iostream>
#include <mist/matrix.h>
#include <mist/thread.h>


struct parameter
{
	unsigned int thread_id_;
	unsigned int thread_num_;
};

void thread_function( const parameter &p )
{
	if( p.thread_id_ % 2 != 1 )
	{
		mist::lock l( "hoge" );
		for( int i = 0 ; i < 10 ; i++ )
		{
			std::cout << p.thread_id_;
//			std::cout << "( " << thread_id_ << ", " << thread_num_ << " )" << std::endl;
			for( int j = 0, k = 0 ; j < 1000000 ; j++ )
			{
				k++;
			}
		}
	}
	else
	{
		mist::lock l( "hoge2" );
		for( int i = 0 ; i < 10 ; i++ )
		{
			std::cout << p.thread_id_;
//			std::cout << "( " << thread_id_ << ", " << thread_num_ << " )" << std::endl;
			for( int j = 0, k = 0 ; j < 1000000 ; j++ )
			{
				k++;
			}
		}
	}
}

#define CREATE_THREAD_NUM	5

int main( int argc, char *argv[] )
{
	using namespace std;

	parameter param[ CREATE_THREAD_NUM ];
	mist::thread_handle t[ CREATE_THREAD_NUM ];

	int i;
	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		param[ i ].thread_id_ = i + 1;
		param[ i ].thread_num_ = CREATE_THREAD_NUM;
	}

	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		t[ i ] = mist::create_thread( param[ i ], thread_function );
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

	return( 0 );
}
