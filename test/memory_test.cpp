#include <iostream>
#include <mist/mist.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	typedef size_t size_type;
	size_type i, j;

	char memories[ 40 ];
	size_type num = sizeof( memories ) - 1;

	// ‹¤—Lƒƒ‚ƒŠ—Ìˆæ‚ğ‰Šú‰»‚·‚é
	for( i = 0 ; i < num ; i++ )
	{
		memories[ i ] = '?';
	}
	memories[ i ] = '\0';

	cout << "shared memory: " << memories << endl;

	// ‚PŸŒ³”z—ñ
	{
		mist::array< char > a( 5, memories, num ), b( 8 );
		for( i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = static_cast< char >( '0' + ( i % 16 ) );
		}

		for( i = 0 ; i < b.size( ) ; i++ )
		{
			b[ i ] = static_cast< char >( 'A' + ( i % ( 'z' - 'A' ) ) );
		}

		cout << "shared memory: " << memories << endl;
		cout << a << endl << endl;
		cout << "a <- b" << endl;
		cout << b << endl;

		a = b;

		cout << a << endl;
		cout << "shared memory: " << memories << endl;
	}

	// ‚QŸŒ³”z—ñ
	{
		mist::array2< char > a( 5, 5, memories, num ), b( 6, 6 );
		for( i = 0 ; i < a.size( ) ; i++ )
		{
			a[ i ] = static_cast< char >( '0' + ( i % 16 ) );
		}

		for( i = 0 ; i < b.size( ) ; i++ )
		{
			b[ i ] = static_cast< char >( 'A' + ( i % ( 'z' - 'A' ) ) );
		}

		cout << "shared memory: " << memories << endl;
		cout << a << endl << endl;
		cout << "a <- b" << endl;
		cout << b << endl << endl;

		a = b;

		cout << a << endl;
		cout << "shared memory: " << memories << endl;
	}


	return( 0 );
}
