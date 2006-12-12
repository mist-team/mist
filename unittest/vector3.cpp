#include <iostream>
#include "macros.h"
#include <mist/vector.h>

template < class T >
class TestVector3 : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::vector3< T > vector_type;
	typedef T value_type;

	CPPUNIT_TEST_SUITE( TestVector3< T > );
	CPPUNIT_TEST( Constructor_001 );
	CPPUNIT_TEST( Constructor_002 );
	CPPUNIT_TEST( Constructor_003 );
	CPPUNIT_TEST( Op_Equal_001 );
	CPPUNIT_TEST( Op_NotEqual_001 );
	CPPUNIT_TEST( Op_LT_001 );
	CPPUNIT_TEST( Op_LT_002 );
	CPPUNIT_TEST( Op_LT_003 );
	CPPUNIT_TEST( Op_LE_001 );
	CPPUNIT_TEST( Op_LE_002 );
	CPPUNIT_TEST( Op_LE_003 );
	CPPUNIT_TEST( Op_LE_004 );
	CPPUNIT_TEST( Op_GT_001 );
	CPPUNIT_TEST( Op_GT_002 );
	CPPUNIT_TEST( Op_GT_003 );
	CPPUNIT_TEST( Op_GE_001 );
	CPPUNIT_TEST( Op_GE_002 );
	CPPUNIT_TEST( Op_GE_003 );
	CPPUNIT_TEST( Op_GE_004 );
	CPPUNIT_TEST( Op_Add_001 );
	CPPUNIT_TEST( Op_Add_002 );
	CPPUNIT_TEST( Op_Subtract_001 );
	CPPUNIT_TEST( Op_Subtract_002 );
	CPPUNIT_TEST( Op_Negative_001 );
	CPPUNIT_TEST( Op_Negative_002 );
	CPPUNIT_TEST( Op_Negative_003 );
	CPPUNIT_TEST( Op_Multiply_001 );
	CPPUNIT_TEST( Op_Multiply_002 );
	CPPUNIT_TEST( Op_Multiply_003 );
	CPPUNIT_TEST( Op_Multiply_004 );
	CPPUNIT_TEST( Op_Divide_001 );
	CPPUNIT_TEST( Op_Divide_002 );
	CPPUNIT_TEST( Op_Inner_001 );
	CPPUNIT_TEST( Op_Inner_002 );
	CPPUNIT_TEST( Op_Inner_003 );
	CPPUNIT_TEST( Op_Outer_001 );
	CPPUNIT_TEST( Op_Outer_002 );
	CPPUNIT_TEST( Op_Outer_003 );
	CPPUNIT_TEST( Op_Outer_004 );
	CPPUNIT_TEST( Inner_001 );
	CPPUNIT_TEST( Inner_002 );
	CPPUNIT_TEST( Inner_003 );
	CPPUNIT_TEST( Outer_001 );
	CPPUNIT_TEST( Outer_002 );
	CPPUNIT_TEST( Outer_003 );
	CPPUNIT_TEST( Outer_004 );
	CPPUNIT_TEST( Unit_001 );
	CPPUNIT_TEST( Unit_002 );
	CPPUNIT_TEST( Unit_003 );
	CPPUNIT_TEST_SUITE_END( );

protected:
	void Constructor_001( )
	{
		vector_type v;

		CPPUNIT_ASSERT_EQUAL( v.x, value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( v.y, value_type( 0 ) );
		CPPUNIT_ASSERT_EQUAL( v.z, value_type( 0 ) );
	}

	void Constructor_002( )
	{
		vector_type v( 1, 2, 3 );

		CPPUNIT_ASSERT_EQUAL( v.x, value_type( 1 ) );
		CPPUNIT_ASSERT_EQUAL( v.y, value_type( 2 ) );
		CPPUNIT_ASSERT_EQUAL( v.z, value_type( 3 ) );
	}

	void Constructor_003( )
	{
		vector_type v( -1, -2, -3 );

		CPPUNIT_ASSERT_EQUAL( v.x, value_type( -1 ) );
		CPPUNIT_ASSERT_EQUAL( v.y, value_type( -2 ) );
		CPPUNIT_ASSERT_EQUAL( v.z, value_type( -3 ) );
	}

	void Op_Equal_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 3 );
		vector_type v3( 3, 2, 3 );

		CPPUNIT_ASSERT( v1 == v2 );
		CPPUNIT_ASSERT( !( v1 == v3 ) );
		CPPUNIT_ASSERT( !( v2 == v3 ) );
	}

	void Op_NotEqual_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 2, 1, 4 );
		vector_type v3( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 != v2 );
		CPPUNIT_ASSERT( v2 != v3 );
		CPPUNIT_ASSERT( !( v1 != v3 ) );
	}

	void Op_LT_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 2, 3, 3 );

		CPPUNIT_ASSERT( v1 < v2 );
	}

	void Op_LT_002( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 3, 3 );

		CPPUNIT_ASSERT( v1 < v2 );
	}

	void Op_LT_003( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 4 );

		CPPUNIT_ASSERT( v1 < v2 );
	}

	void Op_LE_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 2, 3, 3 );

		CPPUNIT_ASSERT( v1 <= v2 );
	}

	void Op_LE_002( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 3, 3 );

		CPPUNIT_ASSERT( v1 <= v2 );
	}

	void Op_LE_003( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 <= v2 );
	}

	void Op_LE_004( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 4 );

		CPPUNIT_ASSERT( v1 <= v2 );
	}

	void Op_GT_001( )
	{
		vector_type v1( 2, 3, 3 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 > v2 );
	}

	void Op_GT_002( )
	{
		vector_type v1( 1, 3, 3 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 > v2 );
	}

	void Op_GT_003( )
	{
		vector_type v1( 1, 2, 4 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 > v2 );
	}

	void Op_GE_001( )
	{
		vector_type v1( 2, 3, 3 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 > v2 );
	}

	void Op_GE_002( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 >= v2 );
	}

	void Op_GE_003( )
	{
		vector_type v1( 1, 3, 3 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 >= v2 );
	}

	void Op_GE_004( )
	{
		vector_type v1( 1, 2, 4 );
		vector_type v2( 1, 2, 3 );

		CPPUNIT_ASSERT( v1 >= v2 );
	}

	void Op_Add_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 3 );
		vector_type v3( 2, 4, 6 );
		vector_type v4 = v1 + v2;

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Op_Add_002( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( -1, -2, -3 );
		vector_type v3( 0, 0, 0 );
		vector_type v4 = v1 + v2;

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Op_Subtract_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 3 );
		vector_type v3( 0, 0, 0 );
		vector_type v4 = v1 - v2;

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Op_Subtract_002( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( -1, -2, -3 );
		vector_type v3( 2, 4, 6 );
		vector_type v4 = v1 - v2;

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Op_Negative_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( -1, -2, -3 );
		vector_type v3 = -v1;

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Negative_002( )
	{
		vector_type v1( -1, -2, -3 );
		vector_type v2( 1, 2, 3 );
		vector_type v3 = -v1;

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Negative_003( )
	{
		vector_type v1( 0, 0, 0 );
		vector_type v2( 0, 0, 0 );
		vector_type v3 = -v1;

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Multiply_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 2, 4, 6 );
		vector_type v3 = v1 * 2;

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Multiply_002( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 2, 4, 6 );
		vector_type v3 = 2 * v1;

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Multiply_003( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( -2, -4, -6 );
		vector_type v3 = v1 * -2;

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Multiply_004( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( -2, -4, -6);
		vector_type v3 = -2 * v1;

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Divide_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( value_type( 0.5 ), 1, value_type( 1.5 ) );
		vector_type v3 = v1 / value_type( 2 );

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Divide_002( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( value_type( -0.5 ), -1, value_type( -1.5 ) );
		vector_type v3 = v1 / value_type( -2 );

		CPPUNIT_ASSERT_EQUAL( v2, v3 );
	}

	void Op_Inner_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 3 );
		value_type s  = v1 ^ v2;

		CPPUNIT_ASSERT_EQUAL( s, value_type( 14 ) );
	}

	void Op_Inner_002( )
	{
		vector_type v1( 1, 0, 1 );
		vector_type v2( 0, 1, 0 );
		value_type s  = v1 ^ v2;

		CPPUNIT_ASSERT_EQUAL( s, value_type( 0 ) );
	}

	void Op_Inner_003( )
	{
		vector_type v1( 0, 1, 0 );
		vector_type v2( 1, 0, 1 );
		value_type s = v1 ^ v2;

		CPPUNIT_ASSERT_EQUAL( s, value_type( 0 ) );
	}

	void Op_Outer_001( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 0, 1, 0 );
		vector_type v3( 0, 0, 1 );
		vector_type v4 = v1 * v2;

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Op_Outer_002( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 0, 1, 0 );
		vector_type v3( 0, 0, 1 );
		vector_type v4 = v2 * v3;

		CPPUNIT_ASSERT_EQUAL( v1, v4 );
	}

	void Op_Outer_003( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 0, 1, 0 );
		vector_type v3( 0, 0, 1 );
		vector_type v4 = v3 * v1;

		CPPUNIT_ASSERT_EQUAL( v2, v4 );
	}

	void Op_Outer_004( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 1, 0, 0 );
		vector_type v3( 0, 0, 0 );
		vector_type v4 = v1 * v2;

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Inner_001( )
	{
		vector_type v1( 1, 2, 3 );
		vector_type v2( 1, 2, 3 );
		value_type s = v1.inner( v2 );

		CPPUNIT_ASSERT_EQUAL( s, value_type( 14 ) );
	}

	void Inner_002( )
	{
		vector_type v1( 1, 0, 1 );
		vector_type v2( 0, 1, 0 );
		value_type s = v1.inner( v2 );

		CPPUNIT_ASSERT_EQUAL( s, value_type( 0 ) );
	}

	void Inner_003( )
	{
		vector_type v1( 0, 1, 0 );
		vector_type v2( 1, 0, 1 );
		value_type s = v1.inner( v2 );

		CPPUNIT_ASSERT_EQUAL( s, value_type( 0 ) );
	}

	void Outer_001( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 0, 1, 0 );
		vector_type v3( 0, 0, 1 );
		vector_type v4 = v1.outer( v2 );

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Outer_002( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 0, 1, 0 );
		vector_type v3( 0, 0, 1 );
		vector_type v4 = v2.outer( v3 );

		CPPUNIT_ASSERT_EQUAL( v1, v4 );
	}

	void Outer_003( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 0, 1, 0 );
		vector_type v3( 0, 0, 1 );
		vector_type v4 = v3.outer( v1 );

		CPPUNIT_ASSERT_EQUAL( v2, v4 );
	}

	void Outer_004( )
	{
		vector_type v1( 1, 0, 0 );
		vector_type v2( 1, 0, 0 );
		vector_type v3( 0, 0, 0 );
		vector_type v4 = v1.outer( v2 );

		CPPUNIT_ASSERT_EQUAL( v3, v4 );
	}

	void Unit_001( )
	{
		value_type len = static_cast< value_type >( 1.0 / std::sqrt( 3.0 ) );
		vector_type v1( 1, 1, 1 );
		vector_type v2( len, len, len );

		CPPUNIT_ASSERT_EQUAL( v1.unit( ), v2 );
	}

	void Unit_002( )
	{
		value_type len = static_cast< value_type >( 1.0 / std::sqrt( 3.0 ) );
		vector_type v1( -1, -1, -1 );
		vector_type v2( -len, -len, -len );

		CPPUNIT_ASSERT_EQUAL( v1.unit( ), v2 );
	}

	void Unit_003( )
	{
		vector_type v1( 0, 0, 0 );
		vector_type v2( 0, 0, 0 );

		CPPUNIT_ASSERT_NO_THROW( v1.unit( ) );
		CPPUNIT_ASSERT_EQUAL( v1.unit( ), v2 );
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3< char > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3< short > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3< int > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3< float > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector3< double > );
