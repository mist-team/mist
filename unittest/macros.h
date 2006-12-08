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




// condition が true の場合に message を表示し，false の場合にテスト失敗
#define CPPUNIT_ASSERTION_MESSAGE( condition, message ) CPPUNIT_ASSERT_MESSAGE( message, condition )

// expected と actual が等しい場合でテストに成功し message を表示する
#define CPPUNIT_ASSERTION_EQUAL_MESSAGE( expected, actual, message ) CPPUNIT_ASSERT_EQUAL_MESSAGE( message, expected, actual ) 

// | expected - actual | < delta でテストに成功し message を表示する
#define CPPUNIT_ASSERTION_DOUBLES_EQUAL_MESSAGE( expected, actual, delta, message ) CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( message, expected, actual, delta )

// 指定した expression が ExceptionType 型の例外を送出した場合にテスト成功で message を表示する
// 例： CPPUNIT_ASSERT_THROW_MESSAGE( v.at( 50 ), std::out_of_range, "- std::vector<int> v;" );
#define CPPUNIT_ASSERTION_THROW_MESSAGE( expression, ExceptionType, message ) CPPUNIT_ASSERT_THROW_MESSAGE( message, expression, ExceptionType )

// 指定した expression が例外を送出しない場合にテスト成功し message を表示
#define CPPUNIT_ASSERTION_NO_THROW_MESSAGE( expression, message ) CPPUNIT_ASSERT_NO_THROW_MESSAGE( message, expression ) 

// テスト assertion に失敗した場合にテスト成功し message を表示
// 例： CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( CPPUNIT_ASSERT( 1 == 2 ), "1 == 2" );
#define CPPUNIT_ASSERTION_ASSERT_FAIL_MESSAGE( assertion, message ) CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( message, assertion )

// テスト assertion に成功した場合にテスト成功し message を表示
// 例： CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( CPPUNIT_ASSERT( 1 == 1 ), "1 != 1" );
#define CPPUNIT_ASSERTION_ASSERT_PASS_MESSAGE( assertion, message ) CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( message, assertion )

#endif