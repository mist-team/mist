#include <iostream>
#include <mist/mist.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	typedef size_t size_type;

	{
		mist::array1< size_type > a( 2, 1.0 ), b( 1, 0.5 );
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = i;
		}
		for( size_type j = 0 ; j < b.size( ) ; j++, i++ )
		{
			b[ j ] = i;
		}
		cout << "a: " << a.reso1( ) <<endl;
		cout << a;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) <<endl;
		cout << b << endl;

		a.swap( b );

		cout << "a: " << a.reso1( ) <<endl;
		cout << a;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) <<endl;
		cout << b << endl;
	}

	{
		mist::array2< size_type > a( 2, 2, 1.0, 1.0 ), b( 3, 3, 0.5, 0.5 );
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = i;
		}
		for( size_type j = 0 ; j < b.size( ) ; j++, i++ )
		{
			b[ j ] = i;
		}
		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) <<endl;
		cout << a;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) <<endl;
		cout << b << endl;

		a.swap( b );

		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) <<endl;
		cout << a;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) <<endl;
		cout << b << endl;
	}

	{
		mist::array3< size_type > a( 2, 2, 2, 1.0, 1.0, 1.0 ), b( 3, 3, 3, 0.5, 0.5, 0.5 );
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = i;
		}
		for( size_type j = 0 ; j < b.size( ) ; j++, i++ )
		{
			b[ j ] = i;
		}
		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << ", " << a.reso3( ) <<endl;
		cout << a;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << ", " << b.reso3( ) <<endl;
		cout << b << endl;

		a.swap( b );

		cout << "a: " << a.reso1( ) << ", " << a.reso2( ) << ", " << a.reso3( ) <<endl;
		cout << a;
		cout << "-----------------------" << endl;
		cout << "b: " << b.reso1( ) << ", " << b.reso2( ) << ", " << b.reso3( ) <<endl;
		cout << b << endl;
	}


	return( 0 );
}
