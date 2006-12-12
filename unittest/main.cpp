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
	// イベントマネージャを作成する
	CPPUNIT_NS::TestResult controller;

	// テスト結果を収集するオブジェクトを作成する
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );        

	// テスト実行時に進行状況を表示するオブジェクトを作成する
	//CPPUNIT_NS::BriefTestProgressListener progress;
	//controller.addListener( &progress );

	// テストランナーにテストを設定する
	CPPUNIT_NS::TestRunner runner;
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry( ).makeTest( ) );
	runner.run( controller );

	// テスト結果を出力する
	CPPUNIT_NS::TextOutputter outputter( &result, std::cout );
	outputter.write( );

	// テストに成功した場合に 0 を返し，失敗したら 1 を返す
	return( result.wasSuccessful( ) ? 0 : 1 );
}
