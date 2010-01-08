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
#include "macros.h"
#include <mist/matrix.h>
#include <mist/numeric.h>
#include <mist/vector.h>

template < class T >
class TestMatrix : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::matrix< T > matrix_type;
	typedef mist::vector3< float > vector_type;
	typedef T value_type;

	CPPUNIT_TEST_SUITE( TestMatrix< T > );
	CPPUNIT_TEST( Constructor_001 );
	CPPUNIT_TEST( Constructor_002 );
	CPPUNIT_TEST( Constructor_003 );
	CPPUNIT_TEST( Op_Equal_001 );
	CPPUNIT_TEST( Op_Equal_002 );
	CPPUNIT_TEST( Op_NotEqual_001 );
	CPPUNIT_TEST( Op_NotEqual_002 );
	CPPUNIT_TEST( Identity_001 );
	CPPUNIT_TEST( Diag_001 );
	CPPUNIT_TEST( Op_Add_001 );
	CPPUNIT_TEST( Op_Add_002 );
	CPPUNIT_TEST( Op_Minus_001 );
	CPPUNIT_TEST( Op_Minus_002 );
	CPPUNIT_TEST( Op_Negative_001 );
	CPPUNIT_TEST( Op_Multiply_001 );
	CPPUNIT_TEST( Op_Multiply_002 );
	CPPUNIT_TEST( Op_Multiply_003 );
	CPPUNIT_TEST( Op_Multiply_004 );
	CPPUNIT_TEST( Op_Divide_001 );
	CPPUNIT_TEST( Invert_001 );
	CPPUNIT_TEST( Invert_002 );
	CPPUNIT_TEST( Invert_003 );
	CPPUNIT_TEST( Invert_004 );
	CPPUNIT_TEST( Transpose_001 );
	CPPUNIT_TEST( Determinant_001 );
	CPPUNIT_TEST( Determinant_002 );
	CPPUNIT_TEST( Eigen_001 );
	CPPUNIT_TEST( Eigen_002 );
	CPPUNIT_TEST_SUITE_END( );

protected:
	void Constructor_001( )
	{
		matrix_type m = matrix_type::zero( 4, 4 );

		CPPUNIT_ASSERT_EQUAL( m( 0, 0 ), value_type( 0 ) ); 
		CPPUNIT_ASSERT_EQUAL( m( 0, 1 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 0, 2 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 0, 3 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 0 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 1 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 2 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 3 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 2, 0 ), value_type( 0 ) ); 
		CPPUNIT_ASSERT_EQUAL( m( 2, 1 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 2, 2 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 2, 3 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 3, 0 ), value_type( 0 ) ); 
		CPPUNIT_ASSERT_EQUAL( m( 3, 1 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 3, 2 ), value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( m( 3, 3 ), value_type( 0 ) );
	}

	void Constructor_002( )
	{
		matrix_type m = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );

		CPPUNIT_ASSERT_EQUAL( m( 0, 0 ), value_type(  1 ) ); 
		CPPUNIT_ASSERT_EQUAL( m( 0, 1 ), value_type(  2 ) );
		CPPUNIT_ASSERT_EQUAL( m( 0, 2 ), value_type(  3 ) );
		CPPUNIT_ASSERT_EQUAL( m( 0, 3 ), value_type(  4 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 0 ), value_type(  5 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 1 ), value_type(  6 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 2 ), value_type(  7 ) );
		CPPUNIT_ASSERT_EQUAL( m( 1, 3 ), value_type(  8 ) );
		CPPUNIT_ASSERT_EQUAL( m( 2, 0 ), value_type(  9 ) ); 
		CPPUNIT_ASSERT_EQUAL( m( 2, 1 ), value_type( 10 ) );
		CPPUNIT_ASSERT_EQUAL( m( 2, 2 ), value_type( 11 ) );
		CPPUNIT_ASSERT_EQUAL( m( 2, 3 ), value_type( 12 ) );
		CPPUNIT_ASSERT_EQUAL( m( 3, 0 ), value_type( 13 ) ); 
		CPPUNIT_ASSERT_EQUAL( m( 3, 1 ), value_type( 14 ) );
		CPPUNIT_ASSERT_EQUAL( m( 3, 2 ), value_type( 15 ) );
		CPPUNIT_ASSERT_EQUAL( m( 3, 3 ), value_type( 16 ) );
	}

	void Constructor_003( )
	{
		matrix_type m = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );

		CPPUNIT_ASSERT_EQUAL( m[  0 ], value_type(  1 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  1 ], value_type(  5 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  2 ], value_type(  9 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  3 ], value_type( 13 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  4 ], value_type(  2 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  5 ], value_type(  6 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  6 ], value_type( 10 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  7 ], value_type( 14 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  8 ], value_type(  3 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[  9 ], value_type(  7 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[ 10 ], value_type( 11 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[ 11 ], value_type( 15 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[ 12 ], value_type(  4 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[ 13 ], value_type(  8 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[ 14 ], value_type( 12 ) ); 
		CPPUNIT_ASSERT_EQUAL( m[ 15 ], value_type( 16 ) ); 
	}

	void Op_Equal_001( )
	{
		matrix_type m1 = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2 = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );

		CPPUNIT_ASSERT( m1 == m2 );
	}

	void Op_Equal_002( )
	{
		matrix_type m1 = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2 = matrix_type::_44( 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 );

		CPPUNIT_ASSERT( !( m1 == m2 ) );
	}

	void Op_NotEqual_001( )
	{
		matrix_type m1 = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2 = matrix_type::_44( 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 );

		CPPUNIT_ASSERT( m1 != m2 );
	}

	void Op_NotEqual_002( )
	{
		matrix_type m1 = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2 = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );

		CPPUNIT_ASSERT( !( m1 != m2 ) );
	}

	void Identity_001( )
	{
		matrix_type m   = matrix_type::identity( 4, 4 );
		matrix_type ans = matrix_type::_44( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );

		CPPUNIT_ASSERT_EQUAL( m, ans );
	}

	void Diag_001( )
	{
		matrix_type m   = matrix_type::diag( 3, 1, 2 );
		matrix_type ans = matrix_type::_33( 3, 0, 0, 0, 2, 0, 0, 0, 1 );

		CPPUNIT_ASSERT_EQUAL( m, ans );
	}

	void Op_Add_001( )
	{
		matrix_type m1  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::_44( 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m1 + m2 ), ans );
	}

	void Op_Add_002( )
	{
		matrix_type m1  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2  = matrix_type::_44( -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16 );
		matrix_type ans = matrix_type::zero( 4, 4 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m1 + m2 ), ans );
	}

	void Op_Minus_001( )
	{
		matrix_type m1  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::zero( 4, 4 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m1 - m2 ), ans );
	}

	void Op_Minus_002( )
	{
		matrix_type m1  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2  = matrix_type::_44( -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16 );
		matrix_type ans = matrix_type::_44( 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m1 - m2 ), ans );
	}

	void Op_Negative_001( )
	{
		matrix_type m   = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::_44( -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( -m ), ans );
	}

	void Op_Multiply_001( )
	{
		matrix_type m   = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::_44( 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m * 2.0 ), ans );
	}

	void Op_Multiply_002( )
	{
		matrix_type m   = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::_44( 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( 2.0 * m ), ans );
	}

	void Op_Multiply_003( )
	{
		matrix_type m1  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2  = matrix_type::_44( 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 );
		matrix_type ans = matrix_type::_44( 80, 70, 60, 50, 240, 214, 188, 162, 400, 358, 316, 274, 560, 502, 444, 386 );
		CPPUNIT_ASSERT_EQUAL( matrix_type( m1 * m2 ), ans );
	}

	void Op_Multiply_004( )
	{
		matrix_type m1  = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type m2  = matrix_type::zero( 4, 4 );
		matrix_type ans = matrix_type::zero( 4, 4 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m1 * m2 ), ans );
	}

	void Op_Divide_001( )
	{
		matrix_type m   = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::_44( 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m / 2.0 ), ans );
	}
	
	void Invert_001( )
	{
		matrix_type m   = matrix_type::_33( 1, 2, 3, 2, 4, 1, 4, 1, 2 );
		matrix_type ans = matrix_type::_33( -7, 1, 10, 0, 10, -5, 14, -7, 0 ) / 35.0;

		CPPUNIT_ASSERT( mist::inverse( m ).is_equal( ans, 1.0e-4 ) );
	}
	
	void Invert_002( )
	{
		matrix_type m   = matrix_type::_44( 1, 0, 2, -1, 8, 3, -1, -1, 3, 0, 3, -1, 0, -2, 3, 1 );
		matrix_type ans = matrix_type::_44( -10, -2, 9, -3, 42, 9, -38, 13, 19, 4, -17, 6, 27, 6, -25, 9 );

		CPPUNIT_ASSERT( mist::inverse( m ).is_equal( ans, 1.0e-4 ) );
	}

	void Invert_003( )
	{
		matrix_type m   = matrix_type::_44( 1, -1, 0, 0, 0, 1, -1, 0, 0, 0, 1, -1, 0, 0, 0, 1 );
		matrix_type ans = matrix_type::_44( 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1 );

		CPPUNIT_ASSERT_EQUAL( mist::inverse( m ), ans );
	}

	void Invert_004( )
	{
		matrix_type m0   = matrix_type::_44( 1, 0, 2, -1, 8, 3, -1, -1, 3, 0, 3, -1, 0, -2, 3, 1 );
		matrix_type m( 5, 5 );

		for( size_t r = 0 ; r < m0.rows( ) ; r++ )
		{
			for( size_t c = 0 ; c < m0.rows( ) ; c++ )
			{
				m( r, c ) = m0( r, c );
			}
		}

		m( 4, 4 ) = value_type( 1 );

		CPPUNIT_ASSERT( matrix_type( m * mist::inverse( m ) ).is_equal( matrix_type::identity( 5, 5 ), 1.0e-4 ) );
	}

	void Transpose_001( )
	{
		matrix_type m   = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::_44( 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m.t( ) ), ans );
	}

	void Determinant_001( )
	{
		matrix_type m   = matrix_type::_33( 32, 2, 5, 2, 3, -2, 14, 5, -1 );
		value_type ans = 12;

		CPPUNIT_ASSERT_EQUAL( mist::det( m ), ans );
	}

	void Determinant_002( )
	{
		matrix_type m   = matrix_type::_44( 1, 3, 6, 3, 4, 7, 1, 3, 0, 9, 4, 7, 3, 8, 9, 2 );
		value_type ans = -770;

		CPPUNIT_ASSERT_EQUAL( mist::det( m ), ans );
	}

	template < class T >
	bool different_sign( const T &v1, const vector_type::value_type &v2 )
	{
		return( v1 * v2 < 0 );
	}

	template < class T >
	bool different_sign( const std::complex< T > &v1, const vector_type::value_type &v2 )
	{
		return( v1.real( ) * v2 < 0 );
	}

	void CheckEigen( const matrix_type &m, const matrix_type &aeval, vector_type aevec0, vector_type aevec1, vector_type aevec2 )
	{
		matrix_type eval, evec;
		mist::eigen( m, eval, evec );

		if( different_sign( evec( 0, 0 ), aevec0.x ) )
		{
			aevec0 = -aevec0;
		}
		if( different_sign( evec( 0, 1 ), aevec1.x ) )
		{
			aevec1 = -aevec1;
		}
		if( different_sign( evec( 0, 2 ), aevec2.x ) )
		{
			aevec2 = -aevec2;
		}

		matrix_type aevec( 3, 3 );
		aevec( 0, 0 ) = value_type( aevec0.x );
		aevec( 1, 0 ) = value_type( aevec0.y );
		aevec( 2, 0 ) = value_type( aevec0.z );
		aevec( 0, 1 ) = value_type( aevec1.x );
		aevec( 1, 1 ) = value_type( aevec1.y );
		aevec( 2, 1 ) = value_type( aevec1.z );
		aevec( 0, 2 ) = value_type( aevec2.x );
		aevec( 1, 2 ) = value_type( aevec2.y );
		aevec( 2, 2 ) = value_type( aevec2.z );

		CPPUNIT_ASSERT( eval.is_equal( aeval, 1.0e-2 ) );
		CPPUNIT_ASSERT( evec.is_equal( aevec, 1.0e-4 ) );
		//CPPUNIT_ASSERTION_EQUAL( eval, aeval );
		//CPPUNIT_ASSERTION_EQUAL( evec, aevec );
	}

	void Eigen_001( )
	{
		matrix_type m    = matrix_type::_33( 5, -1, -1, 4, -9, 8, 4, -7, 6 );

#if defined( _DESCENDING_ORDER_EIGEN_VALUE_ ) && _DESCENDING_ORDER_EIGEN_VALUE_ != 0
		matrix_type aeval = matrix_type::_31( 3, 1, -2 );

		vector_type aevec0 = vector_type( 1, 1, 1 ).unit( );
		vector_type aevec1 = vector_type( 1, 2, 2 ).unit( );
		vector_type aevec2 = vector_type( 1, 4, 3 ).unit( );
#else
		matrix_type aeval = matrix_type::_31( -2, 1, 3 );

		vector_type aevec2 = vector_type( 1, 1, 1 ).unit( );
		vector_type aevec1 = vector_type( 1, 2, 2 ).unit( );
		vector_type aevec0 = vector_type( 1, 4, 3 ).unit( );
#endif

		CheckEigen( m, aeval, aevec0, aevec1, aevec2 );
	}

	void Eigen_002( )
	{
		matrix_type m    = matrix_type::_33( 3, 1, -3, 4, 5, -10, 2, 1, -2 );

#if defined( _DESCENDING_ORDER_EIGEN_VALUE_ ) && _DESCENDING_ORDER_EIGEN_VALUE_ != 0
		matrix_type aeval = matrix_type::_31( 3, 2, 1 );

		vector_type aevec0 = vector_type( 1, 3, 1 ).unit( );
		vector_type aevec1 = vector_type( 1, 2, 1 ).unit( );
		vector_type aevec2 = vector_type( 1, 4, 2 ).unit( );
#else
		matrix_type aeval = matrix_type::_31( 1, 2, 3 );

		vector_type aevec2 = vector_type( 1, 3, 1 ).unit( );
		vector_type aevec1 = vector_type( 1, 2, 1 ).unit( );
		vector_type aevec0 = vector_type( 1, 4, 2 ).unit( );
#endif

		CheckEigen( m, aeval, aevec0, aevec1, aevec2 );
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< float > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< double > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< std::complex< float > > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< std::complex< double > > );
