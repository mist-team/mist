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


class TestMD4 : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::md4 hash_type;

private:
	CPPUNIT_TEST_SUITE( TestMD4 );
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
		std::string ans = "31d6cfe0d16ae931b73c59d7e0c089c0";

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
		std::string ans = "bde52cb31de33e46245e05fbdbd6fb24";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_004( )
	{
		hash_type m( "abc" );
		std::string ans = "a448017aaf21d8525fc10ae87aa6729d";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_005( )
	{
		hash_type m( "message digest" );
		std::string ans = "d9130a8164549fe818874806e1c7014b";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_006( )
	{
		hash_type m( "abcdefghijklmnopqrstuvwxyz" );
		std::string ans = "d79e1c308aa5bbcdeea8ed63df412da9";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_007( )
	{
		hash_type m( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" );
		std::string ans = "043f8582f241db351ce627e153e7f0e4";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_008( )
	{
		hash_type m( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" );
		std::string ans = "e33b4ddc9c38f2199c3e7b164fcc0536";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

	void Constructor_009( )
	{
		hash_type m( tststr );
		std::string ans = "5951d64bdd9e7f052282b7f9769f91b9";

		CPPUNIT_ASSERTION_EQUAL( m.to_string( ), ans );
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION( TestMD4 );

