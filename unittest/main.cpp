#include <iostream>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>


int main( int argc, char **argv )
{
	// �C�x���g�}�l�[�W�����쐬����
	CPPUNIT_NS::TestResult controller;

	// �e�X�g���ʂ����W����I�u�W�F�N�g���쐬����
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );        

	// �e�X�g���s���ɐi�s�󋵂�\������I�u�W�F�N�g���쐬����
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener( &progress );      

	// �e�X�g�����i�[�Ƀe�X�g��ݒ肷��
	CPPUNIT_NS::TestRunner runner;
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( ).makeTest( ) );
	runner.run( controller );

	// �e�X�g�ɐ��������ꍇ�� 0 ��Ԃ��C���s������ 1 ��Ԃ�
	return( result.wasSuccessful( ) ? 0 : 1 );
}
