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


// condition �� true �̏ꍇ�Ƀe�X�g����
#define CPPUNIT_ASSERTION( condition ) CPPUNIT_ASSERT( condition )

// expected �� actual ���������ꍇ�Ƀe�X�g����
#define CPPUNIT_ASSERTION_EQUAL( actual, expected ) CPPUNIT_ASSERT_EQUAL( expected, actual )

// | expected - actual | < delta �̏ꍇ�Ƀe�X�g����
#define CPPUNIT_ASSERTION_DOUBLES_EQUAL( actual, expected, delta ) CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, actual, delta )

// �w�肵�� expression ����O�𑗏o���Ȃ��ꍇ�Ƀe�X�g����
#define CPPUNIT_ASSERTION_NO_THROW( expression ) CPPUNIT_ASSERT_NO_THROW( expression )


#endif