#include <iostream>
#include <mist/matrix.h>
#include <mist/thread.h>

template < class T >
class thread_parameter : public mist::thread< thread_parameter< T > >
{
public:
	typedef mist::thread< thread_parameter< T > > base;
	typedef typename base::thread_exit_type thread_exit_type;

private:
	unsigned int thread_id_;
	unsigned int thread_num_;
	mist::matrix< T > matrix_;

public:
	unsigned int thread_id( ) const { return( thread_id ); }
	unsigned int thread_num( ) const { return( thread_num ); }

	const thread_parameter& operator =( const thread_parameter &p )
	{
		base::operator =( p );
		thread_id_ = p.thread_id_;
		thread_num_ = p.thread_num_;
		matrix_ = p.matrix_;
		return( *this );
	}

	thread_parameter( unsigned int id = 0, unsigned int num = 0 ) : thread_id_( id ), thread_num_( num ), matrix_( )
	{
	}
	thread_parameter( const thread_parameter &p ) : base( p ), thread_id_( p.thread_id_ ), thread_num_( p.thread_num_ ), matrix_( p.matrix_ )
	{
	}

	virtual ~thread_parameter( ){ }

protected:
	// 継承した先で必ず実装されるスレッド関数
	virtual thread_exit_type thread_function( const thread_parameter &p )
	{
		if( thread_id_ % 2 == 1 )
		{
			mist::lock l( "hoge" );
			for( int i = 0 ; i < 10 ; i++ )
			{
				std::cout << thread_id_;
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
				std::cout << thread_id_;
	//			std::cout << "( " << thread_id_ << ", " << thread_num_ << " )" << std::endl;
				for( int j = 0, k = 0 ; j < 1000000 ; j++ )
				{
					k++;
				}
			}
		}
		return( true );
	}
};

#define CREATE_THREAD_NUM	5

int main( int argc, char *argv[] )
{
	using namespace std;

	thread_parameter< double > param[ CREATE_THREAD_NUM ];

	int i;
	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		param[i] = thread_parameter< double >( i + 1, CREATE_THREAD_NUM );
	}

	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		param[i].create( );
	}

	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		param[i].wait( );
	}

	for( i = 0 ; i < CREATE_THREAD_NUM ; i++ )
	{
		param[i].close( );
	}

	std::cout << std::endl;

	return( 0 );
}
