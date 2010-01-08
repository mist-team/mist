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

#ifndef __INCLUDE_UNIT_TEST_HELPER_MACROS__
#define __INCLUDE_UNIT_TEST_HELPER_MACROS__

#include <cppunit/extensions/HelperMacros.h>


// condition が true の場合にテスト成功
// CPPUNIT_ASSERT( condition )

// condition が true の場合に message を表示し，false の場合にテスト失敗
// CPPUNIT_ASSERT_MESSAGE( message, condition )


// 指定したメッセージを表示してテストを失敗させる
// CPPUNIT_FAIL( message )

// expected と actual が等しい場合にテスト成功
// CPPUNIT_ASSERT_EQUAL( expected, actual )

// expected と actual が等しい場合でテストに成功し message を表示する
// CPPUNIT_ASSERT_EQUAL_MESSAGE( message, expected, actual ) 

// | expected - actual | < delta の場合にテスト成功
// CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, actual, delta )

// | expected - actual | < delta でテストに成功し message を表示する
// CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( message, expected, actual, delta )

// 指定した expression が ExceptionType 型の例外を送出した場合にテスト成功
// 例： CPPUNIT_ASSERT_THROW( v.at( 50 ), std::out_of_range );
// CPPUNIT_ASSERT_THROW( expression, ExceptionType )

// 指定した expression が ExceptionType 型の例外を送出した場合にテスト成功で message を表示する
// 例： CPPUNIT_ASSERT_THROW_MESSAGE( "- std::vector<int> v;", v.at( 50 ), std::out_of_range );
// CPPUNIT_ASSERT_THROW_MESSAGE( message, expression, ExceptionType )

// 指定した expression が例外を送出しない場合にテスト成功
// CPPUNIT_ASSERT_NO_THROW( expression )

// 指定した expression が例外を送出しない場合にテスト成功し message を表示
// CPPUNIT_ASSERT_NO_THROW_MESSAGE( message, expression ) 

// テスト assertion に失敗した場合にテスト成功
// 例： CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT( 1 == 2 ) );
// CPPUNIT_ASSERT_ASSERTION_FAIL( assertion )

// テスト assertion に失敗した場合にテスト成功し message を表示
// 例： CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( "1 == 2", CPPUNIT_ASSERT( 1 == 2 ) );
// CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( message, assertion )

// テスト assertion に成功した場合にテスト成功
// 例： CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT( 1 == 1 ) );
// CPPUNIT_ASSERT_ASSERTION_PASS( assertion )

// テスト assertion に成功した場合にテスト成功し message を表示
// 例： CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( "1 != 1", CPPUNIT_ASSERT( 1 == 1 ) );
// CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( message, assertion )


// condition が true の場合にテスト成功
#define CPPUNIT_ASSERTION( condition ) CPPUNIT_ASSERT( condition )

// expected と actual が等しい場合にテスト成功
#define CPPUNIT_ASSERTION_EQUAL( actual, expected ) CPPUNIT_ASSERT_EQUAL( expected, actual )

// | expected - actual | < delta の場合にテスト成功
#define CPPUNIT_ASSERTION_DOUBLES_EQUAL( actual, expected, delta ) CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, actual, delta )

// 指定した expression が例外を送出しない場合にテスト成功
#define CPPUNIT_ASSERTION_NO_THROW( expression ) CPPUNIT_ASSERT_NO_THROW( expression )


#endif