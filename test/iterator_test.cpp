#include <iostream>
#include <mist/mist.h>
#include <mist/matrix.h>


int main( int argc, char *argv[] )
{
	using namespace std;

	{
		mist::matrix< double > mat( 3, 3 );

		mat( 0, 0 ) = 1; mat( 0, 1 ) = 2; mat( 0, 2 ) = 3;
		mat( 1, 0 ) = 4; mat( 1, 1 ) = 5; mat( 1, 2 ) = 6;
		mat( 2, 0 ) = 7; mat( 2, 1 ) = 8; mat( 2, 2 ) = 9;

		cout << mat << endl;

		mist::matrix< double >::iterator rite = mat.row_begin( 1 );

		for( ; rite != mat.row_end( 1 ) ; rite++ )
		{
			*rite = 3;
		}

		cout << mat << endl;

		mist::matrix< double >::reverse_iterator cite = mat.col_rbegin( 2 );

		for( ; cite != mat.col_rend( 2 ) ; cite++ )
		{
			*cite = 4;
		}

		cout << mat << endl;
	}
//	return( 0 );

	{
		mist::array3< int > a( 3, 3, 3, 1 );

		//a( 0, 0, 0 ) = 1; a( 0, 1, 0 ) = 2; a( 0, 2, 0 ) = 3;
		//a( 1, 0, 0 ) = 4; a( 1, 1, 0 ) = 5; a( 1, 2, 0 ) = 6;
		//a( 2, 0, 0 ) = 7; a( 2, 1, 0 ) = 8; a( 2, 2, 0 ) = 9;

		//a( 0, 0, 1 ) = 1; a( 0, 1, 1 ) = 2; a( 0, 2, 1 ) = 3;
		//a( 1, 0, 1 ) = 4; a( 1, 1, 1 ) = 5; a( 1, 2, 1 ) = 6;
		//a( 2, 0, 1 ) = 7; a( 2, 1, 1 ) = 8; a( 2, 2, 1 ) = 9;

		//a( 0, 0, 2 ) = 1; a( 0, 1, 2 ) = 2; a( 0, 2, 2 ) = 3;
		//a( 1, 0, 2 ) = 4; a( 1, 1, 2 ) = 5; a( 1, 2, 2 ) = 6;
		//a( 2, 0, 0 ) = 7; a( 2, 1, 0 ) = 8; a( 2, 2, 0 ) = 9;


		{
			mist::array3< int >::iterator ite = a.x_begin( 1 );

			for( ; ite != a.x_end( 1 ) ; ite++ )
			{
				*ite = 2;
			}

			cout << a << endl;
		}


		{
			mist::array3< int >::iterator ite = a.y_begin( 1 );

			for( ; ite != a.y_end( 1 ) ; ite++ )
			{
				*ite = 3;
			}

			cout << a << endl;
		}
	}
}
