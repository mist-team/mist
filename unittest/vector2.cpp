#include <iostream>
#include "macros.h"
#include <mist/vector.h>

template < class T >
class TestVector2 : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::vector2< T > vector_type;
	typedef T value_type;

	CPPUNIT_TEST_SUITE( TestVector2< T > );
	CPPUNIT_TEST( Constructor_001 );
	CPPUNIT_TEST( Constructor_002 );
	CPPUNIT_TEST( Constructor_003 );
	CPPUNIT_TEST( Op_Equal_001 );
	CPPUNIT_TEST( Op_NotEqual_001 );
	CPPUNIT_TEST( Op_LT_001 );
	CPPUNIT_TEST( Op_LT_002 );
	CPPUNIT_TEST( Op_LE_001 );
	CPPUNIT_TEST( Op_LE_002 );
	CPPUNIT_TEST( Op_LE_003 );
	CPPUNIT_TEST( Op_GT_001 );
	CPPUNIT_TEST( Op_GT_002 );
	CPPUNIT_TEST( Op_GE_001 );
	CPPUNIT_TEST( Op_GE_002 );
	CPPUNIT_TEST( Op_GE_003 );
	CPPUNIT_TEST( Op_NotEqual_001 );
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

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v.x, value_type( 0 ), "v.x is zero" );
		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v.y, value_type( 0 ), "v.y is zero" );
	}

	void Constructor_002( )
	{
		vector_type v( 1, 2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v.x, value_type( 1 ), "v.x is 1" );
		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v.y, value_type( 2 ), "v.y is 2" );
	}

	void Constructor_003( )
	{
		vector_type v( -1, -2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v.x, value_type( -1 ), "v.x is -1" );
		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v.y, value_type( -2 ), "v.y is -2" );
	}

	void Op_Equal_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 2 );
		vector_type v3( 3, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 == v2, "( 1, 2 ) == ( 1, 2 )" );
		CPPUNIT_ASSERTION_MESSAGE( !( v1 == v3 ), "( 1, 2 ) != ( 3, 2 )" );
	}

	void Op_NotEqual_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 2, 1 );
		vector_type v3( 1, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 != v2, "( 1, 2 ) != ( 2, 1 )" );
		CPPUNIT_ASSERTION_MESSAGE( !( v1 != v3 ), "( 1, 2 ) == ( 1, 2 )" );
	}

	void Op_LT_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 2, 3 );

		CPPUNIT_ASSERTION_MESSAGE( v1 < v2, "( 1, 2 ) < ( 2, 3 )" );
	}

	void Op_LT_002( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 3 );

		CPPUNIT_ASSERTION_MESSAGE( v1 < v2, "( 1, 2 ) < ( 1, 3 )" );
	}

	void Op_LE_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 2, 3 );

		CPPUNIT_ASSERTION_MESSAGE( v1 <= v2, "( 1, 2 ) <= ( 2, 3 )" );
	}

	void Op_LE_002( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 3 );

		CPPUNIT_ASSERTION_MESSAGE( v1 <= v2, "( 1, 2 ) <= ( 1, 3 )" );
	}

	void Op_LE_003( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 <= v2, "( 1, 2 ) <= ( 1, 2 )" );
	}

	void Op_GT_001( )
	{
		vector_type v1( 2, 3 );
		vector_type v2( 1, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 > v2, "( 2, 3 ) > ( 1, 2 )" );
	}

	void Op_GT_002( )
	{
		vector_type v1( 1, 3 );
		vector_type v2( 1, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 > v2, "( 1, 3 ) > ( 1, 2 )" );
	}

	void Op_GE_001( )
	{
		vector_type v1( 2, 3 );
		vector_type v2( 1, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 > v2, "( 2, 3 ) >= ( 1, 2 )" );
	}

	void Op_GE_002( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 >= v2, "( 1, 2 ) >= ( 1, 2 )" );
	}

	void Op_GE_003( )
	{
		vector_type v1( 1, 3 );
		vector_type v2( 1, 2 );

		CPPUNIT_ASSERTION_MESSAGE( v1 >= v2, "( 1, 3 ) >= ( 1, 2 )" );
	}

	void Op_Add_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 2 );
		vector_type v3( 2, 4 );
		vector_type v4 = v1 + v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v3, v4, "( 1, 2 ) + ( 1, 2 ) = ( 2, 4 )" );
	}

	void Op_Add_002( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( -1, -2 );
		vector_type v3( 0, 0 );
		vector_type v4 = v1 + v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v3, v4, "( 1, 2 ) + ( -1, -2 ) = ( 0, 0 )" );
	}

	void Op_Subtract_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 2 );
		vector_type v3( 0, 0 );
		vector_type v4 = v1 - v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v3, v4, "( 1, 2 ) - ( 1, 2 ) = ( 0, 0 )" );
	}

	void Op_Subtract_002( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( -1, -2 );
		vector_type v3( 2, 4 );
		vector_type v4 = v1 - v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v3, v4, "( 1, 2 ) - ( -1, -2 ) = ( 2, 4 )" );
	}

	void Op_Negative_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( -1, -2 );
		vector_type v3 = -v1;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "-( 1, 2 ) = ( -1, -2 )" );
	}

	void Op_Negative_002( )
	{
		vector_type v1( -1, -2 );
		vector_type v2( 1, 2 );
		vector_type v3 = -v1;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "-( -1, -2 ) = ( 1, 2 )" );
	}

	void Op_Negative_003( )
	{
		vector_type v1( 0, 0 );
		vector_type v2( 0, 0 );
		vector_type v3 = -v1;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "-( 0, 0 ) = ( 0, 0 )" );
	}

	void Op_Multiply_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 2, 4 );
		vector_type v3 = v1 * 2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "( 1, 2 ) * 2 = ( 2, 4 )" );
	}

	void Op_Multiply_002( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 2, 4 );
		vector_type v3 = 2 * v1;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "2 * ( 1, 2 ) = ( 2, 4 )" );
	}

	void Op_Multiply_003( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( -2, -4 );
		vector_type v3 = v1 * -2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "( 1, 2 ) * ( -2 ) = ( -2, -4 )" );
	}

	void Op_Multiply_004( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( -2, -4);
		vector_type v3 = -2 * v1;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "( -2 ) * ( 1, 2 ) = ( -2, -4 )" );
	}

	void Op_Divide_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( value_type( 0.5 ), 1 );
		vector_type v3 = v1 / value_type( 2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "( 1, 2 ) / 2 = ( 0.5, 1 )" );
	}

	void Op_Divide_002( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( value_type( -0.5 ), -1 );
		vector_type v3 = v1 / -2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v2, v3, "( 1, 2 ) / 2 = ( -0.5, -1 )" );
	}

	void Op_Inner_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 2 );
		value_type s  = v1 ^ v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 5 ), "( 1, 2 ) ^ ( 1, 2 ) = 5" );
	}

	void Op_Inner_002( )
	{
		vector_type v1( 1, 0 );
		vector_type v2( 0, 1 );
		value_type s  = v1 ^ v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 1, 0 ) ^ ( 0, 1 ) = 0" );
	}

	void Op_Inner_003( )
	{
		vector_type v1( 0, 1 );
		vector_type v2( 1, 0 );
		value_type s = v1 ^ v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 0, 1 ) ^ ( 1, 0 ) = 0" );
	}

	void Op_Outer_001( )
	{
		vector_type v1( 1, 0 );
		vector_type v2( 0, 1 );
		value_type s = v1 * v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 1 ), "( 1, 0 ) * ( 0, 1 ) = 1" );
	}

	void Op_Outer_002( )
	{
		vector_type v1( 0, 1 );
		vector_type v2( 1, 0 );
		value_type s = v1 * v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( -1 ), "( 0, 1 ) * ( 1, 0 ) = -1" );
	}

	void Op_Outer_003( )
	{
		vector_type v1( 1, 0 );
		vector_type v2( 1, 0 );
		value_type s = v1 * v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 1, 0 ) * ( 1, 0 ) = 0" );
	}

	void Op_Outer_004( )
	{
		vector_type v1( 0, 1 );
		vector_type v2( 0, 1 );
		value_type s = v1 * v2;

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 0, 1 ) * ( 0, 1 ) = 0" );
	}

	void Inner_001( )
	{
		vector_type v1( 1, 2 );
		vector_type v2( 1, 2 );
		value_type s = v1.inner( v2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 5 ), "( 1, 2 ) ^ ( 1, 2 ) = 5" );
	}

	void Inner_002( )
	{
		vector_type v1( 1, 0 );
		vector_type v2( 0, 1 );
		value_type s = v1.inner( v2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 1, 0 ) ^ ( 0, 1 ) = 0" );
	}

	void Inner_003( )
	{
		vector_type v1( 0, 1 );
		vector_type v2( 1, 0 );
		value_type s = v1.inner( v2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 0, 1 ) ^ ( 1, 0 ) = 0" );
	}

	void Outer_001( )
	{
		vector_type v1( 1, 0 );
		vector_type v2( 0, 1 );
		value_type s = v1.outer( v2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 1 ), "( 1, 0 ) * ( 0, 1 ) = 1" );
	}

	void Outer_002( )
	{
		vector_type v1( 0, 1 );
		vector_type v2( 1, 0 );
		value_type s = v1.outer( v2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( -1 ), "( 0, 1 ) * ( 1, 0 ) = -1" );
	}

	void Outer_003( )
	{
		vector_type v1( 1, 0 );
		vector_type v2( 1, 0 );
		value_type s = v1.outer( v2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 1, 0 ) * ( 1, 0 ) = 0" );
	}

	void Outer_004( )
	{
		vector_type v1( 0, 1 );
		vector_type v2( 0, 1 );
		value_type s = v1.outer( v2 );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( s, value_type( 0 ), "( 0, 1 ) * ( 0, 1 ) = 0" );
	}

	void Unit_001( )
	{
		value_type len = static_cast< value_type >( 1.0 / std::sqrt( 2.0 ) );
		vector_type v1( 1, 1 );
		vector_type v2( len, len );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v1.unit( ), v2, "unit of ( 1, 1 ) is correct." );
	}

	void Unit_002( )
	{
		value_type len = static_cast< value_type >( 1.0 / std::sqrt( 2.0 ) );
		vector_type v1( -1, -1 );
		vector_type v2( -len, -len );

		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v1.unit( ), v2, "unit of ( -1, -1 ) is correct." );
	}

	void Unit_003( )
	{
		vector_type v1( 0, 0 );
		vector_type v2( 0, 0 );

		CPPUNIT_ASSERTION_NO_THROW_MESSAGE( v1.unit( ), "unit of ( 0, 0 ) can be calculated." );
		CPPUNIT_ASSERTION_EQUAL_MESSAGE( v1.unit( ), v2, "unit of ( 0, 0 ) is ( 0, 0 )." );
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( TestVector2< char > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector2< short > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector2< int > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector2< float > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestVector2< double > );
