// 
// Copyright (c) 2003-2009, MIST Project, Intelligent Media Integration COE, Nagoya University
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

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
//#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TextOutputter.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int main( int argc, char **argv )
{
	// �C�x���g�}�l�[�W�����쐬����
	CPPUNIT_NS::TestResult controller;

	// �e�X�g���ʂ����W����I�u�W�F�N�g���쐬����
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );        

	// �e�X�g���s���ɐi�s�󋵂�\������I�u�W�F�N�g���쐬����
	//CPPUNIT_NS::BriefTestProgressListener progress;
	//controller.addListener( &progress );

	// �e�X�g�����i�[�Ƀe�X�g��ݒ肷��
	CPPUNIT_NS::TestRunner runner;
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( ).makeTest( ) );
	runner.run( controller );

	// �e�X�g���ʂ��o�͂���
	CPPUNIT_NS::TextOutputter outputter( &result, std::cout );
	outputter.write( );

	// �e�X�g�ɐ��������ꍇ�� 0 ��Ԃ��C���s������ 1 ��Ԃ�
	return( result.wasSuccessful( ) ? 0 : 1 );
}
