#include <iostream>
#include <mist/mist.h>
#include <mist/matrix.h>

#include <algorithm>

int main( int argc, char *argv[] )
{
	using namespace std;

	{
		{
			cout << "Forward Iterator Test row axis" << endl;

			mist::matrix< double > a( 3, 3 );
			mist::matrix< double >::iterator ite = a.row_begin( 1 );

			for( int i = 1 ; ite != a.row_end( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.row_rbegin( 1 ), a.row_rend( 1 ) );

			cout << "sorted result" << endl << a << endl << endl;
		}
		
		{
			cout << "Reverse Iterator Test row axis" << endl;

			mist::matrix< double > a( 3, 3 );
			mist::matrix< double >::reverse_iterator ite = a.row_rbegin( 1 );

			for( int i = 1 ; ite != a.row_rend( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.row_begin( 1 ), a.row_end( 1 ) );

			cout << "sorted result" << endl << a << endl << endl;
		}

		{
			cout << "Forward Iterator Test col axis" << endl;

			mist::matrix< double > a( 3, 3 );
			mist::matrix< double >::iterator ite = a.col_begin( 1 );

			for( int i = 1 ; ite != a.col_end( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.col_rbegin( 1 ), a.col_rend( 1 ) );

			cout << "sorted result" << endl << a << endl << endl;
		}

		{
			cout << "Reverse Iterator Test col axis" << endl;

			mist::matrix< double > a( 3, 3 );
			mist::matrix< double >::reverse_iterator ite = a.col_rbegin( 1 );

			for( int i = 1 ; ite != a.col_rend( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.col_begin( 1 ), a.col_end( 1 ) );

			cout << "sorted result" << endl << a << endl << endl;
		}
	}

	{

		{
			cout << "Forward Iterator Test all element" << endl;

			mist::array2< int > a( 3, 3 );
			mist::array2< int >::iterator ite = a.begin( );

			for( int i = 1 ; ite != a.end( ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.rbegin( ), a.rend( ) );

			cout << "sorted result" << endl << a << endl << endl;
		}

		{
			cout << "Reverse Iterator Test all element" << endl;

			mist::array2< int > a( 3, 3 );
			mist::array2< int >::reverse_iterator ite = a.rbegin( );

			for( int i = 1 ; ite != a.rend( ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.begin( ), a.end( ) );

			cout << "sorted result" << endl << a << endl << endl;
		}
	}

	{
		cout << "Forward Iterator Test X axis" << endl;

		mist::array2< int > a( 3, 3, 0 );
		mist::array2< int >::iterator ite = a.x_begin( 1 );

		for( int i = 1 ; ite != a.x_end( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.x_rbegin( 1 ), a.x_rend( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Forward Iterator Test Y axis" << endl;

		mist::array2< int > a( 3, 3, 0 );
		mist::array2< int >::iterator ite = a.y_begin( 1 );

		for( int i = 1 ; ite != a.y_end( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.y_rbegin( 1 ), a.y_rend( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Reverse Iterator Test X axis" << endl;

		mist::array2< int > a( 3, 3, 0 );
		mist::array2< int >::reverse_iterator ite = a.x_rbegin( 1 );

		for( int i = 1 ; ite != a.x_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.x_begin( 1 ), a.x_end( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Reverse Iterator Test Y axis" << endl;

		mist::array2< int > a( 3, 3, 0 );
		mist::array2< int >::reverse_iterator ite = a.y_rbegin( 1 );

		for( int i = 1 ; ite != a.y_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.y_begin( 1 ), a.y_end( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{

		{
			cout << "Forward Iterator Test all element 3D" << endl;

			mist::array3< int > a( 3, 3, 3 );
			mist::array3< int >::iterator ite = a.begin( );

			for( int i = 1 ; ite != a.end( ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.rbegin( ), a.rend( ) );

			cout << "sorted result" << endl << a << endl << endl;
		}

		{
			cout << "Reverse Iterator Test all element 3D" << endl;

			mist::array3< int > a( 3, 3, 3 );
			mist::array3< int >::reverse_iterator ite = a.rbegin( );

			for( int i = 1 ; ite != a.rend( ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << a << endl << endl;

			sort( a.begin( ), a.end( ) );

			cout << "sorted result" << endl << a << endl << endl;
		}
	}

	{
		cout << "Forward Iterator Test X axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::iterator ite = a.x_begin( 1 );

		for( int i = 1 ; ite != a.x_end( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.x_rbegin( 1 ), a.x_rend( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Forward Iterator Test Y axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::iterator ite = a.y_begin( 1 );

		for( int i = 1 ; ite != a.y_end( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.y_rbegin( 1 ), a.y_rend( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Forward Iterator Test Z axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::iterator ite = a.z_begin( 1 );

		for( int i = 1 ; ite != a.z_end( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.z_rbegin( 1 ), a.z_rend( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Reverse Iterator Test X axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::reverse_iterator ite = a.x_rbegin( 1 );

		for( int i = 1 ; ite != a.x_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.x_begin( 1 ), a.x_end( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Reverse Iterator Test Y axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::reverse_iterator ite = a.y_rbegin( 1 );

		for( int i = 1 ; ite != a.y_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.y_begin( 1 ), a.y_end( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	{
		cout << "Reverse Iterator Test Z axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::reverse_iterator ite = a.z_rbegin( 1 );

		for( int i = 1 ; ite != a.z_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl << endl;

		sort( a.z_begin( 1 ), a.z_end( 1 ) );

		cout << "sorted result" << endl << a << endl << endl;
	}

	return( 0 );
}
