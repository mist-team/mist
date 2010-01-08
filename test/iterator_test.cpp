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
#include <mist/mist.h>
#include <mist/matrix.h>
#include <mist/vector.h>

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
		cout << endl << endl << "MIST Vector Part." << endl;
		{
			cout << "Forward Iterator Test row axis" << endl;

			mist::matrix< mist::vector2< double > > a( 3, 3 );
			mist::matrix< mist::vector2< double > >::iterator ite = a.row_begin( 1 );

			for( int i = 1 ; ite != a.row_end( 1 ) ; ite++, i++ )
			{
				ite->x = i;
				ite->y = i;
			}

			cout << a << endl << endl;

			sort( a.row_rbegin( 1 ), a.row_rend( 1 ) );

			cout << "sorted result" << endl << a << endl << endl;
		}
		
		{
			cout << "Reverse Iterator Test row axis" << endl;

			mist::matrix< mist::vector2< double > > a( 3, 3 );
			mist::matrix< mist::vector2< double > >::reverse_iterator ite = a.row_rbegin( 1 );

			for( int i = 1 ; ite != a.row_rend( 1 ) ; ite++, i++ )
			{
				ite->x = i;
				ite->y = i;
			}

			cout << a << endl << endl;

			sort( a.row_begin( 1 ), a.row_end( 1 ) );

			cout << "sorted result" << endl << a << endl << endl;
		}

		{
			cout << "Forward Iterator Test col axis" << endl;

			mist::matrix< mist::vector2< double > > a( 3, 3 );
			mist::matrix< mist::vector2< double > >::iterator ite = a.col_begin( 1 );

			for( int i = 1 ; ite != a.col_end( 1 ) ; ite++, i++ )
			{
				ite->x = i;
				ite->y = i;
			}

			cout << a << endl << endl;

			sort( a.col_rbegin( 1 ), a.col_rend( 1 ) );

			cout << "sorted result" << endl << a << endl << endl;
		}

		{
			cout << "Reverse Iterator Test col axis" << endl;

			mist::matrix< mist::vector2< double > > a( 3, 3 );
			mist::matrix< mist::vector2< double > >::reverse_iterator ite = a.col_rbegin( 1 );

			for( int i = 1 ; ite != a.col_rend( 1 ) ; ite++, i++ )
			{
				ite->x = i;
				ite->y = i;
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
