#ifndef __INCLUDE_UNIT_TEST_HELPER_MACROS__
#define __INCLUDE_UNIT_TEST_HELPER_MACROS__

#include <cppunit/extensions/HelperMacros.h>


// condition �� true �̏ꍇ�Ƀe�X�g����
// CPPUNIT_ASSERT( condition )

// condition �� true �̏ꍇ�� message ��\�����Cfalse �̏ꍇ�Ƀe�X�g���s
// CPPUNIT_ASSERT_MESSAGE( message, condition )


// �w�肵�����b�Z�[�W��\�����ăe�X�g�����s������
// CPPUNIT_FAIL( message )

// expected �� actual ���������ꍇ�Ƀe�X�g����
// CPPUNIT_ASSERT_EQUAL( expected, actual )

// expected �� actual ���������ꍇ�Ńe�X�g�ɐ����� message ��\������
// CPPUNIT_ASSERT_EQUAL_MESSAGE( message, expected, actual ) 

// | expected - actual | < delta �̏ꍇ�Ƀe�X�g����
// CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, actual, delta )

// | expected - actual | < delta �Ńe�X�g�ɐ����� message ��\������
// CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( message, expected, actual, delta )

// �w�肵�� expression �� ExceptionType �^�̗�O�𑗏o�����ꍇ�Ƀe�X�g����
// ��F CPPUNIT_ASSERT_THROW( v.at( 50 ), std::out_of_range );
// CPPUNIT_ASSERT_THROW( expression, ExceptionType )

// �w�肵�� expression �� ExceptionType �^�̗�O�𑗏o�����ꍇ�Ƀe�X�g������ message ��\������
// ��F CPPUNIT_ASSERT_THROW_MESSAGE( "- std::vector<int> v;", v.at( 50 ), std::out_of_range );
// CPPUNIT_ASSERT_THROW_MESSAGE( message, expression, ExceptionType )

// �w�肵�� expression ����O�𑗏o���Ȃ��ꍇ�Ƀe�X�g����
// CPPUNIT_ASSERT_NO_THROW( expression )

// �w�肵�� expression ����O�𑗏o���Ȃ��ꍇ�Ƀe�X�g������ message ��\��
// CPPUNIT_ASSERT_NO_THROW_MESSAGE( message, expression ) 

// �e�X�g assertion �Ɏ��s�����ꍇ�Ƀe�X�g����
// ��F CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT( 1 == 2 ) );
// CPPUNIT_ASSERT_ASSERTION_FAIL( assertion )

// �e�X�g assertion �Ɏ��s�����ꍇ�Ƀe�X�g������ message ��\��
// ��F CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( "1 == 2", CPPUNIT_ASSERT( 1 == 2 ) );
// CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( message, assertion )

// �e�X�g assertion �ɐ��������ꍇ�Ƀe�X�g����
// ��F CPPUNIT_ASSERT_ASSERTION_PASS( CPPUNIT_ASSERT( 1 == 1 ) );
// CPPUNIT_ASSERT_ASSERTION_PASS( assertion )

// �e�X�g assertion �ɐ��������ꍇ�Ƀe�X�g������ message ��\��
// ��F CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( "1 != 1", CPPUNIT_ASSERT( 1 == 1 ) );
// CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( message, assertion )




// condition �� true �̏ꍇ�� message ��\�����Cfalse �̏ꍇ�Ƀe�X�g���s
#define CPPUNIT_ASSERTION_MESSAGE( condition, message ) CPPUNIT_ASSERT_MESSAGE( message, condition )

// expected �� actual ���������ꍇ�Ńe�X�g�ɐ����� message ��\������
#define CPPUNIT_ASSERTION_EQUAL_MESSAGE( expected, actual, message ) CPPUNIT_ASSERT_EQUAL_MESSAGE( message, expected, actual ) 

// | expected - actual | < delta �Ńe�X�g�ɐ����� message ��\������
#define CPPUNIT_ASSERTION_DOUBLES_EQUAL_MESSAGE( expected, actual, delta, message ) CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE( message, expected, actual, delta )

// �w�肵�� expression �� ExceptionType �^�̗�O�𑗏o�����ꍇ�Ƀe�X�g������ message ��\������
// ��F CPPUNIT_ASSERT_THROW_MESSAGE( v.at( 50 ), std::out_of_range, "- std::vector<int> v;" );
#define CPPUNIT_ASSERTION_THROW_MESSAGE( expression, ExceptionType, message ) CPPUNIT_ASSERT_THROW_MESSAGE( message, expression, ExceptionType )

// �w�肵�� expression ����O�𑗏o���Ȃ��ꍇ�Ƀe�X�g������ message ��\��
#define CPPUNIT_ASSERTION_NO_THROW_MESSAGE( expression, message ) CPPUNIT_ASSERT_NO_THROW_MESSAGE( message, expression ) 

// �e�X�g assertion �Ɏ��s�����ꍇ�Ƀe�X�g������ message ��\��
// ��F CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( CPPUNIT_ASSERT( 1 == 2 ), "1 == 2" );
#define CPPUNIT_ASSERTION_ASSERT_FAIL_MESSAGE( assertion, message ) CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE( message, assertion )

// �e�X�g assertion �ɐ��������ꍇ�Ƀe�X�g������ message ��\��
// ��F CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( CPPUNIT_ASSERT( 1 == 1 ), "1 != 1" );
#define CPPUNIT_ASSERTION_ASSERT_PASS_MESSAGE( assertion, message ) CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE( message, assertion )

#endif