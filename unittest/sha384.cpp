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
#include <mist/hash_algorithm.h>

static std::string tststr =	"Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University\n"
							"All rights reserved.\n"
							"\n"
							"Redistribution and use in source and binary forms, with or without modification,\n"
							"are permitted provided that the following conditions are met:\n"
							"\n"
							"    1. Redistributions of source code must retain the above copyright notice,\n"
							"       this list of conditions and the following disclaimer. \n"
							"\n"
							"    2. Redistributions in binary form must reproduce the above copyright notice,\n"
							"       this list of conditions and the following disclaimer in the documentation\n"
							"       and/or other materials provided with the distribution.\n"
							"\n"
							"    3. Neither the name of the Nagoya University nor the names of its contributors\n"
							"       may be used to endorse or promote products derived from this software\n"
							"       without specific prior written permission. \n"
							"\n"
							"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR\n"
							"IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND\n"
							"FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR\n"
							"CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
							"DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n"
							"DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER\n"
							"IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF\n"
							"THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";


class TestSHA384 : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::sha384 hash_type;

private:
	CPPUNIT_TEST_SUITE( TestSHA384 );
	CPPUNIT_TEST( Constructor_001 );
	CPPUNIT_TEST( Constructor_002 );
	CPPUNIT_TEST( Constructor_003 );
	CPPUNIT_TEST( Constructor_004 );
	CPPUNIT_TEST( Constructor_005 );
	CPPUNIT_TEST( Constructor_006 );
	CPPUNIT_TEST( Constructor_007 );
	CPPUNIT_TEST( Constructor_008 );
	CPPUNIT_TEST( Constructor_009 );
	CPPUNIT_TEST_SUITE_END( );

protected:
	void Constructor_001( )
	{
		hash_type m;
		std::string ans = "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_002( )
	{
		hash_type m( "" );
		std::string ans = hash_type( ).to_string( );

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_003( )
	{
		hash_type m( "a" );
		std::string ans = "54a59b9f22b0b80880d8427e548b7c23abd873486e1f035dce9cd697e85175033caa88e6d57bc35efae0b5afd3145f31";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_004( )
	{
		hash_type m( "abc" );
		std::string ans = "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_005( )
	{
		hash_type m( "message digest" );
		std::string ans = "473ed35167ec1f5d8e550368a3db39be54639f828868e9454c239fc8b52e3c61dbd0d8b4de1390c256dcbb5d5fd99cd5";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_006( )
	{
		hash_type m( "abcdefghijklmnopqrstuvwxyz" );
		std::string ans = "feb67349df3db6f5924815d6c3dc133f091809213731fe5c7b5f4999e463479ff2877f5f2936fa63bb43784b12f3ebb4";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_007( )
	{
		hash_type m( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" );
		std::string ans = "1761336e3f7cbfe51deb137f026f89e01a448e3b1fafa64039c1464ee8732f11a5341a6f41e0c202294736ed64db1a84";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_008( )
	{
		hash_type m( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" );
		std::string ans = "b12932b0627d1c060942f5447764155655bd4da0c9afa6dd9b9ef53129af1b8fb0195996d2de9ca0df9d821ffee67026";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_009( )
	{
		hash_type m( tststr );
		std::string ans = "3e0297de7e165288120db74da8e55615a35218457dc0701d6bd59d6d6e34eb58c6f471259ffb841e2c387f9e19ecf745";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestSHA384 );

