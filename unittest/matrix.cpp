#include <iostream>
#include "macros.h"
#include <mist/matrix.h>
#include <mist/numeric.h>

template < class T >
class TestMatrix : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::matrix< T > matrix_type;
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
	CPPUNIT_TEST( Transpose_001 );
	CPPUNIT_TEST( Determinant_001 );
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
		matrix_type m   = matrix_type::_44( 1, 0, 2, -1, 8, 3, -1, -1, 3, 0, 3, -1, 0, -2, 3, 1 );
		matrix_type ans = matrix_type::_44( -10, -2, 9, -3, 42, 9, -38, 13, 19, 4, -17, 6, 27, 6, -25, 9 );

		CPPUNIT_ASSERT( mist::inverse( m ).is_equal( ans, 1.0e-4 ) );
	}

	void Invert_002( )
	{
		matrix_type m   = matrix_type::_44( 1, -1, 0, 0, 0, 1, -1, 0, 0, 0, 1, -1, 0, 0, 0, 1 );
		matrix_type ans = matrix_type::_44( 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1 );

		CPPUNIT_ASSERT_EQUAL( mist::inverse( m ), ans );
	}

	void Transpose_001( )
	{
		matrix_type m   = matrix_type::_44( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 );
		matrix_type ans = matrix_type::_44( 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16 );

		CPPUNIT_ASSERT_EQUAL( matrix_type( m.t( ) ), ans );
	}

	void Determinant_001( )
	{
		matrix_type m   = matrix_type::_44( 1, 3, 6, 3, 4, 7, 1, 3, 0, 9, 4, 7, 3, 8, 9, 2 );
		value_type ans = -770;

		CPPUNIT_ASSERT_EQUAL( mist::det( m ), ans );
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< float > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< double > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< std::complex< float > > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMatrix< std::complex< double > > );
