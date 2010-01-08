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
#include <ctime>
#include <mist/timer.h>
#include <mist/random.h>
#include <mist/machine_learning.h>


int main( int argc, char *argv[] )
{
	typedef mist::machine_learning::feature feature_type;
	std::vector< feature_type > train, test;

	mist::uniform::random rnd( std::clock( ) );

	double cx1 = 2.0;
	double cy1 = 2.0;
	double cx2 = 17.0;
	double cy2 = 17.0;
	double cx  = ( cx1 + cx2 ) * 0.5;
	double cy  = ( cy1 + cy2 ) * 0.5;
	double r1 = 2.0;
	double r2 = 2.0;

	// 学習用サンプルデータを作成する
	for( size_t i = 0 ; i < 1000 ; i++ )
	{
		double x = ( rnd.real1( ) - 0.5 ) * 20.0 + cx;
		double y = ( rnd.real1( ) - 0.5 ) * 20.0 + cy;

		double R1 = ( x - cx1 ) * ( x - cx1 ) + ( y - cy1 ) * ( y - cy1 );
		double R2 = ( x - cx2 ) * ( x - cx2 ) + ( y - cy2 ) * ( y - cy2 );

		if( R1 > r1 * r1 && R2 > r2 * r2 )
		{
			feature_type f( 2, "0" );
			f[ 0 ] = x;
			f[ 1 ] = y;
			train.push_back( feature_type( f ) );
		}
		else
		{
			feature_type f( 2, R1 < R2 ? "1" : "2" );
			f[ 0 ] = x;
			f[ 1 ] = y;
			train.push_back( feature_type( f ) );
		}
	}

	// テスト用サンプルデータを作成する
	for( size_t i = 0 ; i < 1000 ; i++ )
	{
		double x = ( rnd.real1( ) - 0.5 ) * 20.0 + cx;
		double y = ( rnd.real1( ) - 0.5 ) * 20.0 + cy;

		double R1 = ( x - cx1 ) * ( x - cx1 ) + ( y - cy1 ) * ( y - cy1 );
		double R2 = ( x - cx2 ) * ( x - cx2 ) + ( y - cy2 ) * ( y - cy2 );

		if( R1 > r1 * r1 && R2 > r2 * r2 )
		{
			feature_type f( 2, "0" );
			f[ 0 ] = x;
			f[ 1 ] = y;
			test.push_back( feature_type( f ) );
		}
		else
		{
			feature_type f( 2, R1 < R2 ? "1" : "2" );
			f[ 0 ] = x;
			f[ 1 ] = y;
			test.push_back( feature_type( f ) );
		}
	}

	// AdaBoost を使用する識別器
	//mist::machine_learning::adaboost::classifier< mist::machine_learning::adaboost::mahalanobis_classifier > classifier;
	//mist::machine_learning::adaboost::classifier< mist::machine_learning::adaboost::rating_classifier > classifier;
	mist::machine_learning::adaboost::classifier< > classifier;

	{
		mist::timer t;

		// 学習データを用いて識別器を構築します
		classifier.learn( train, 50 );

		std::cout << "Computation Time: " << t << " sec." << std::endl;
	}

	std::cout << "Classification error for training data = " << classifier.error_rate( train ) << std::endl;
	std::cout << "Classification error for test data     = " << classifier.error_rate( test ) << std::endl;

	// 識別器を保存する際は以下のように書きます
	classifier.save( "filename.dat" );


	// 識別器を読み込む際は以下のように書きます
	classifier.load( "filename.dat" );


	// 実際に分類する際は，以下のような書き方も可能です．
	std::vector< double > feature( test[ 10 ] );
	std::cout << "Result: " << classifier( feature ) << " should be " << test[ 10 ].category << std::endl;


	return( 0 );
}
