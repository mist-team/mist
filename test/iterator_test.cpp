#include <iostream>
#include <mist/mist.h>
#include <mist/matrix.h>


int main( int argc, char *argv[] )
{
	using namespace std;

	{
		{
			cout << "Forward Iterator Test row axis" << endl;

			mist::matrix< double > mat( 3, 3 );
			mist::matrix< double >::iterator ite = mat.row_begin( 1 );

			for( int i = 1 ; ite != mat.row_end( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << mat << endl;
		}

		{
			cout << "Reverse Iterator Test row axis" << endl;

			mist::matrix< double > mat( 3, 3 );
			mist::matrix< double >::reverse_iterator ite = mat.row_rbegin( 1 );

			for( int i = 1 ; ite != mat.row_rend( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << mat << endl;
		}

		{
			cout << "Forward Iterator Test col axis" << endl;

			mist::matrix< double > mat( 3, 3 );
			mist::matrix< double >::iterator ite = mat.col_begin( 1 );

			for( int i = 1 ; ite != mat.col_end( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << mat << endl;
		}

		{
			cout << "Reverse Iterator Test col axis" << endl;

			mist::matrix< double > mat( 3, 3 );
			mist::matrix< double >::reverse_iterator ite = mat.col_rbegin( 1 );

			for( int i = 1 ; ite != mat.col_rend( 1 ) ; ite++, i++ )
			{
				*ite = i;
			}

			cout << mat << endl;
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

		cout << a << endl;
	}

	{
		cout << "Forward Iterator Test Y axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::iterator ite = a.y_begin( 1 );

		for( int i = 1 ; ite != a.y_end( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl;
	}

	{
		cout << "Forward Iterator Test Z axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::iterator ite = a.z_begin( 1 );

		for( int i = 1 ; ite != a.z_end( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl;
	}

	{
		cout << "Reverse Iterator Test X axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::reverse_iterator ite = a.x_rbegin( 1 );

		for( int i = 1 ; ite != a.x_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl;
	}

	{
		cout << "Reverse Iterator Test Y axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::reverse_iterator ite = a.y_rbegin( 1 );

		for( int i = 1 ; ite != a.y_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl;
	}

	{
		cout << "Reverse Iterator Test Z axis" << endl;

		mist::array3< int > a( 3, 3, 3, 0 );
		mist::array3< int >::reverse_iterator ite = a.z_rbegin( 1 );

		for( int i = 1 ; ite != a.z_rend( 1 ) ; ite++, i++ )
		{
			*ite = i;
		}

		cout << a << endl;
	}
}
