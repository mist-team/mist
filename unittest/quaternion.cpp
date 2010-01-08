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
#include <mist/quaternion.h>

template < class T >
class TestQuaternion : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::quaternion< T > quaternion_type;
	typedef mist::vector3< T > vector_type;
	typedef T value_type;

	CPPUNIT_TEST_SUITE( TestQuaternion< T > );
	CPPUNIT_TEST( Constructor_001 );
	CPPUNIT_TEST( Constructor_002 );
	CPPUNIT_TEST( Constructor_003 );
	CPPUNIT_TEST( Constructor_004 );
	CPPUNIT_TEST( Constructor_005 );
	CPPUNIT_TEST( Constructor_006 );
	CPPUNIT_TEST( Constructor_007 );
	CPPUNIT_TEST( Op_Equal_001 );
	CPPUNIT_TEST( Op_NotEqual_001 );
	CPPUNIT_TEST( Op_LT_001 );
	CPPUNIT_TEST( Op_LT_002 );
	CPPUNIT_TEST( Op_LT_003 );
	CPPUNIT_TEST( Op_LT_004 );
	CPPUNIT_TEST( Op_LE_001 );
	CPPUNIT_TEST( Op_LE_002 );
	CPPUNIT_TEST( Op_LE_003 );
	CPPUNIT_TEST( Op_LE_004 );
	CPPUNIT_TEST( Op_LE_005 );
	CPPUNIT_TEST( Op_GT_001 );
	CPPUNIT_TEST( Op_GT_002 );
	CPPUNIT_TEST( Op_GT_003 );
	CPPUNIT_TEST( Op_GT_004 );
	CPPUNIT_TEST( Op_GE_001 );
	CPPUNIT_TEST( Op_GE_002 );
	CPPUNIT_TEST( Op_GE_003 );
	CPPUNIT_TEST( Op_GE_004 );
	CPPUNIT_TEST( Op_GE_005 );
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
	CPPUNIT_TEST( Inner_001 );
	CPPUNIT_TEST( Inner_002 );
	CPPUNIT_TEST( Inner_003 );
	CPPUNIT_TEST( Unit_001 );
	CPPUNIT_TEST( Unit_002 );
	CPPUNIT_TEST( Unit_003 );
	CPPUNIT_TEST( Conjugate_001 );
	CPPUNIT_TEST( Invert_001 );
	CPPUNIT_TEST( Rotate_001 );
	CPPUNIT_TEST( Rotate_002 );
	CPPUNIT_TEST( Rotate_003 );
	CPPUNIT_TEST_SUITE_END( );

protected:
	void Constructor_001( )
	{
		quaternion_type q;

		CPPUNIT_ASSERTION_EQUAL( q.w, value_type( 0 ) );
		CPPUNIT_ASSERTION_EQUAL( q.x, value_type( 0 ) );
		CPPUNIT_ASSERTION_EQUAL( q.y, value_type( 0 ) );
		CPPUNIT_ASSERTION_EQUAL( q.z, value_type( 0 ) );
	}

	void Constructor_002( )
	{
		quaternion_type q( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION_EQUAL( q.w, value_type( 1 ) );
		CPPUNIT_ASSERTION_EQUAL( q.x, value_type( 2 ) );
		CPPUNIT_ASSERTION_EQUAL( q.y, value_type( 3 ) );
		CPPUNIT_ASSERTION_EQUAL( q.z, value_type( 4 ) );
	}

	void Constructor_003( )
	{
		quaternion_type q( -1, -2, -3, -4 );

		CPPUNIT_ASSERTION_EQUAL( q.w, value_type( -1 ) );
		CPPUNIT_ASSERTION_EQUAL( q.x, value_type( -2 ) );
		CPPUNIT_ASSERTION_EQUAL( q.y, value_type( -3 ) );
		CPPUNIT_ASSERTION_EQUAL( q.z, value_type( -4 ) );
	}

	void Constructor_004( )
	{
		quaternion_type q( -1, vector_type( -2, -3, -4 ) );

		CPPUNIT_ASSERTION_EQUAL( q.w, value_type( -1 ) );
		CPPUNIT_ASSERTION_EQUAL( q.x, value_type( -2 ) );
		CPPUNIT_ASSERTION_EQUAL( q.y, value_type( -3 ) );
		CPPUNIT_ASSERTION_EQUAL( q.z, value_type( -4 ) );
	}

	void Constructor_005( )
	{
		quaternion_type q( -1 );

		CPPUNIT_ASSERTION_EQUAL( q.w, value_type( -1 ) );
		CPPUNIT_ASSERTION_EQUAL( q.x, value_type( 0 ) );
		CPPUNIT_ASSERTION_EQUAL( q.y, value_type( 0 ) );
		CPPUNIT_ASSERTION_EQUAL( q.z, value_type( 0 ) );
	}

	void Constructor_006( )
	{
		quaternion_type q( vector_type( 1, 0, 0 ), 180 );

		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.w, value_type( 0 ), 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.x, value_type( 1 ), 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.y, value_type( 0 ), 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.z, value_type( 0 ), 1.0e-10 );
	}

	void Constructor_007( )
	{
		quaternion_type q( vector_type( 1, 0, 0 ), 60 );

		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.w, value_type( 0.5 * std::sqrt( 3.0 ) ), 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.x, value_type( 0.5 ), 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.y, value_type( 0 ), 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.z, value_type( 0 ), 1.0e-10 );
	}

	void Op_Equal_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );
		quaternion_type q3( 3, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 == q2 );
		CPPUNIT_ASSERTION( !( q1 == q3 ) );
		CPPUNIT_ASSERTION( !( q2 == q3 ) );
	}

	void Op_NotEqual_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 2, 1, 4, 4 );
		quaternion_type q3( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 != q2 );
		CPPUNIT_ASSERTION( q2 != q3 );
		CPPUNIT_ASSERTION( !( q1 != q3 ) );
	}

	void Op_LT_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 2, 3, 3, 4 );

		CPPUNIT_ASSERTION( q1 < q2 );
	}

	void Op_LT_002( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 3, 3, 4 );

		CPPUNIT_ASSERTION( q1 < q2 );
	}

	void Op_LT_003( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 4, 4 );

		CPPUNIT_ASSERTION( q1 < q2 );
	}

	void Op_LT_004( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 5 );

		CPPUNIT_ASSERTION( q1 < q2 );
	}

	void Op_LE_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 2, 3, 3, 4 );

		CPPUNIT_ASSERTION( q1 <= q2 );
	}

	void Op_LE_002( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 3, 3, 4 );

		CPPUNIT_ASSERTION( q1 <= q2 );
	}

	void Op_LE_003( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 <= q2 );
	}

	void Op_LE_004( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 4, 4 );

		CPPUNIT_ASSERTION( q1 <= q2 );
	}

	void Op_LE_005( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 5 );

		CPPUNIT_ASSERTION( q1 <= q2 );
	}

	void Op_GT_001( )
	{
		quaternion_type q1( 2, 3, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 > q2 );
	}

	void Op_GT_002( )
	{
		quaternion_type q1( 1, 3, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 > q2 );
	}

	void Op_GT_003( )
	{
		quaternion_type q1( 1, 2, 4, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 > q2 );
	}

	void Op_GT_004( )
	{
		quaternion_type q1( 1, 2, 3, 5 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 > q2 );
	}

	void Op_GE_001( )
	{
		quaternion_type q1( 2, 3, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 > q2 );
	}

	void Op_GE_002( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 >= q2 );
	}

	void Op_GE_003( )
	{
		quaternion_type q1( 1, 3, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 >= q2 );
	}

	void Op_GE_004( )
	{
		quaternion_type q1( 1, 2, 4, 4 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 >= q2 );
	}

	void Op_GE_005( )
	{
		quaternion_type q1( 1, 2, 3, 5 );
		quaternion_type q2( 1, 2, 3, 4 );

		CPPUNIT_ASSERTION( q1 >= q2 );
	}

	void Op_Add_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );
		quaternion_type q3( 2, 4, 6, 8 );
		quaternion_type q4 = q1 + q2;

		CPPUNIT_ASSERTION_EQUAL( q3, q4 );
	}

	void Op_Add_002( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( -1, -2, -3, -4 );
		quaternion_type q3( 0, 0, 0, 0 );
		quaternion_type q4 = q1 + q2;

		CPPUNIT_ASSERTION_EQUAL( q3, q4 );
	}

	void Op_Subtract_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );
		quaternion_type q3( 0, 0, 0, 0 );
		quaternion_type q4 = q1 - q2;

		CPPUNIT_ASSERTION_EQUAL( q3, q4 );
	}

	void Op_Subtract_002( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( -1, -2, -3, -4 );
		quaternion_type q3( 2, 4, 6, 8 );
		quaternion_type q4 = q1 - q2;

		CPPUNIT_ASSERTION_EQUAL( q3, q4 );
	}

	void Op_Negative_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( -1, -2, -3, -4 );
		quaternion_type q3 = -q1;

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Negative_002( )
	{
		quaternion_type q1( -1, -2, -3, -4 );
		quaternion_type q2( 1, 2, 3, 4 );
		quaternion_type q3 = -q1;

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Negative_003( )
	{
		quaternion_type q1( 0, 0, 0, 0 );
		quaternion_type q2( 0, 0, 0, 0 );
		quaternion_type q3 = -q1;

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Multiply_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 2, 4, 6, 8 );
		quaternion_type q3 = q1 * 2;

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Multiply_002( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 2, 4, 6, 8 );
		quaternion_type q3 = 2 * q1;

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Multiply_003( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( -2, -4, -6, -8 );
		quaternion_type q3 = q1 * -2;

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Multiply_004( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( -2, -4, -6, -8 );
		quaternion_type q3 = -2 * q1;

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Divide_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( value_type( 0.5 ), 1, value_type( 1.5 ), 2 );
		quaternion_type q3 = q1 / value_type( 2 );

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Op_Divide_002( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( value_type( -0.5 ), -1, value_type( -1.5 ), -2 );
		quaternion_type q3 = q1 / value_type( -2 );

		CPPUNIT_ASSERTION_EQUAL( q2, q3 );
	}

	void Inner_001( )
	{
		quaternion_type q1( 1, 2, 3, 4 );
		quaternion_type q2( 1, 2, 3, 4 );
		value_type s = q1.inner( q2 );

		CPPUNIT_ASSERTION_EQUAL( s, value_type( 30 ) );
	}

	void Inner_002( )
	{
		quaternion_type q1( 1, 0, 1, 0 );
		quaternion_type q2( 0, 1, 0, 1 );
		value_type s = q1.inner( q2 );

		CPPUNIT_ASSERTION_EQUAL( s, value_type( 0 ) );
	}

	void Inner_003( )
	{
		quaternion_type q1( 0, 1, 0, 1 );
		quaternion_type q2( 1, 0, 1, 0 );
		value_type s = q1.inner( q2 );

		CPPUNIT_ASSERTION_EQUAL( s, value_type( 0 ) );
	}

	void Unit_001( )
	{
		value_type len = static_cast< value_type >( 0.5 );
		quaternion_type q1( 1, 1, 1, 1 );
		quaternion_type q2( len, len, len, len );

		CPPUNIT_ASSERTION_EQUAL( q1.unit( ), q2 );
	}

	void Unit_002( )
	{
		value_type len = static_cast< value_type >( 0.5 );
		quaternion_type q1( -1, -1, -1, -1 );
		quaternion_type q2( -len, -len, -len, -len );

		CPPUNIT_ASSERTION_EQUAL( q1.unit( ), q2 );
	}

	void Unit_003( )
	{
		quaternion_type q1( 0, 0, 0, 0 );
		quaternion_type q2( 0, 0, 0, 0 );

		CPPUNIT_ASSERTION_NO_THROW( q1.unit( ) );
		CPPUNIT_ASSERTION_EQUAL( q1.unit( ), q2 );
	}

	void Conjugate_001( )
	{
		quaternion_type q( 1, 2, 3, 4 );
		quaternion_type ans( 1, -2, -3, -4 );

		CPPUNIT_ASSERTION_EQUAL( q.conjugate( ), ans );
	}

	void Invert_001( )
	{
		quaternion_type val( 2, 3, 4, 5 );

		double length = val.length( );
		quaternion_type ans = val.conjugate( ) / ( length * length );
		quaternion_type q = val.inv( );

		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.w, ans.w, 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.x, ans.x, 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.y, ans.y, 1.0e-10 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( q.z, ans.z, 1.0e-10 );
	}

	void Rotate_001( )
	{
		vector_type v( 1, 0, 0 );
		vector_type axis( 0, 0, 1 );
		quaternion_type q( axis, 90 );

		v = q.rotate( v );

		CPPUNIT_ASSERTION_DOUBLES_EQUAL( v.x, value_type( 0 ), 1.0e-6 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( v.y, value_type( 1 ), 1.0e-6 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( v.z, value_type( 0 ), 1.0e-6 );
	}

	void Rotate_002( )
	{
		vector_type eX( 1, 0, 0 );
		vector_type eY( 0, 1, 0 );
		quaternion_type q = quaternion_type::rotate( eX, eY );

		vector_type _eX = q.rotate( eY );

		CPPUNIT_ASSERTION_DOUBLES_EQUAL( _eX.x, value_type( -1 ), 1.0e-6 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( _eX.y, value_type( 0 ), 1.0e-6 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( _eX.z, value_type( 0 ), 1.0e-6 );
	}

	void Rotate_003( )
	{
		vector_type eX( 1, 0, 0 );
		vector_type eY( 0, 1, 0 );
		vector_type eZ( 0, 0, 1 );
		quaternion_type q = quaternion_type::rotate( eX, eY, eZ );

		vector_type _eX = q.rotate( eY );

		CPPUNIT_ASSERTION_DOUBLES_EQUAL( _eX.x, value_type( -1 ), 1.0e-6 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( _eX.y, value_type( 0 ), 1.0e-6 );
		CPPUNIT_ASSERTION_DOUBLES_EQUAL( _eX.z, value_type( 0 ), 1.0e-6 );
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( TestQuaternion< float > );
CPPUNIT_TEST_SUITE_REGISTRATION( TestQuaternion< double > );
