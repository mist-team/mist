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
#include <mist/hash_algorithm.h>


void hash_test( const mist::hash_algorithm &h, const std::string &in, const std::string &true_result, bool show_input = true )
{
	if( show_input )
	{
		std::cout << h.name( ) << "( " << in << " ) = " << h << ( h == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
	}
	else
	{
		std::cout << h.name( ) << "( ... ) = " << h << ( h == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
	}
}


int main( int argc, char *argv[] )
{
	using namespace std;

	std::string str =	"Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University\n"
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


	hash_test( mist::md2( ), "", "8350e5a3e24c153df2275c9f80692773" );
	hash_test( mist::md2( "" ), "", "8350e5a3e24c153df2275c9f80692773" );
	hash_test( mist::md2( "a" ), "a", "32ec01ec4a6dac72c0ab96fb34c0b5d1" );
	hash_test( mist::md2( "abc" ), "abc", "da853b0d3f88d99b30283a69e6ded6bb" );
	hash_test( mist::md2( "message digest" ), "message digest", "ab4f496bfb2a530b219ff33031fe06b0" );
	hash_test( mist::md2( "abcdefghijklmnopqrstuvwxyz" ), "abcdefghijklmnopqrstuvwxyz", "4e8ddff3650292ab5a4108c3aa47940b" );
	hash_test( mist::md2( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "da33def2a42df13975352846c30338cd" );
	hash_test( mist::md2( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ), "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "d5976f79d83d3a0dc9806c3c66f3efd8" );
	hash_test( mist::md2( str ), str, "6b181421fa2fa8bc09b3aaf47982e4ef", false );

	std::cout << std::endl << std::endl;

	hash_test( mist::md4( ), "", "31d6cfe0d16ae931b73c59d7e0c089c0" );
	hash_test( mist::md4( "" ), "", "31d6cfe0d16ae931b73c59d7e0c089c0" );
	hash_test( mist::md4( "a" ), "a", "bde52cb31de33e46245e05fbdbd6fb24" );
	hash_test( mist::md4( "abc" ), "abc", "a448017aaf21d8525fc10ae87aa6729d" );
	hash_test( mist::md4( "message digest" ), "message digest", "d9130a8164549fe818874806e1c7014b" );
	hash_test( mist::md4( "abcdefghijklmnopqrstuvwxyz" ), "abcdefghijklmnopqrstuvwxyz", "d79e1c308aa5bbcdeea8ed63df412da9" );
	hash_test( mist::md4( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "043f8582f241db351ce627e153e7f0e4" );
	hash_test( mist::md4( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ), "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "e33b4ddc9c38f2199c3e7b164fcc0536" );
	hash_test( mist::md4( str ), str, "5951d64bdd9e7f052282b7f9769f91b9", false );

	std::cout << std::endl << std::endl;

	hash_test( mist::md5( ), "", "d41d8cd98f00b204e9800998ecf8427e" );
	hash_test( mist::md5( "" ), "", "d41d8cd98f00b204e9800998ecf8427e" );
	hash_test( mist::md5( "a" ), "a", "0cc175b9c0f1b6a831c399e269772661" );
	hash_test( mist::md5( "abc" ), "abc", "900150983cd24fb0d6963f7d28e17f72" );
	hash_test( mist::md5( "message digest" ), "message digest", "f96b697d7cb7938d525a2f31aaf161d0" );
	hash_test( mist::md5( "abcdefghijklmnopqrstuvwxyz" ), "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" );
	hash_test( mist::md5( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f" );
	hash_test( mist::md5( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ), "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a" );
	hash_test( mist::md5( str ), str, "cdadb30196e4ba73b66725a8dd7d8c7f", false );

	std::cout << std::endl << std::endl;

	hash_test( mist::sha1( ), "", "da39a3ee5e6b4b0d3255bfef95601890afd80709" );
	hash_test( mist::sha1( "" ), "", "da39a3ee5e6b4b0d3255bfef95601890afd80709" );
	hash_test( mist::sha1( "a" ), "abc", "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8" );
	hash_test( mist::sha1( "abc" ), "abc", "a9993e364706816aba3e25717850c26c9cd0d89d" );
	hash_test( mist::sha1( "message digest" ), "message digest", "c12252ceda8be8994d5fa0290a47231c1d16aae3" );
	hash_test( mist::sha1( "abcdefghijklmnopqrstuvwxyz" ), "abcdefghijklmnopqrstuvwxyz", "32d10c7b8cf96570ca04ce37f2a19d84240d3a89" );
	hash_test( mist::sha1( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "761c457bf73b14d27e9e9265c46f4b4dda11f940" );
	hash_test( mist::sha1( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ), "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "50abf5706a150990a08b2c5ea40fa0e585554732" );
	hash_test( mist::sha1( str ), str, "f882018b8a447d7d0d10c4a3e9762158f63ef42e", false );

	std::cout << std::endl << std::endl;

	hash_test( mist::sha256( ), "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" );
	hash_test( mist::sha256( "" ), "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" );
	hash_test( mist::sha256( "a" ), "a", "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb" );
	hash_test( mist::sha256( "abc" ), "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" );
	hash_test( mist::sha256( "message digest" ), "message digest", "f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650" );
	hash_test( mist::sha256( "abcdefghijklmnopqrstuvwxyz" ), "abcdefghijklmnopqrstuvwxyz", "71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73" );
	hash_test( mist::sha256( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "db4bfcbd4da0cd85a60c3c37d3fbd8805c77f15fc6b1fdfe614ee0a7c8fdb4c0" );
	hash_test( mist::sha256( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ), "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "f371bc4a311f2b009eef952dd83ca80e2b60026c8e935592d0f9c308453c813e" );
	hash_test( mist::sha256( str ), str, "4d438205e2bbeff6e1ae631402480767165f4125b8f41984273e79e43ef8e994", false );

	std::cout << std::endl << std::endl;

	hash_test( mist::sha384( ), "", "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" );
	hash_test( mist::sha384( "" ), "", "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" );
	hash_test( mist::sha384( "a" ), "a", "54a59b9f22b0b80880d8427e548b7c23abd873486e1f035dce9cd697e85175033caa88e6d57bc35efae0b5afd3145f31" );
	hash_test( mist::sha384( "abc" ), "abc", "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7" );
	hash_test( mist::sha384( "message digest" ), "message digest", "473ed35167ec1f5d8e550368a3db39be54639f828868e9454c239fc8b52e3c61dbd0d8b4de1390c256dcbb5d5fd99cd5" );
	hash_test( mist::sha384( "abcdefghijklmnopqrstuvwxyz" ), "abcdefghijklmnopqrstuvwxyz", "feb67349df3db6f5924815d6c3dc133f091809213731fe5c7b5f4999e463479ff2877f5f2936fa63bb43784b12f3ebb4" );
	hash_test( mist::sha384( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "1761336e3f7cbfe51deb137f026f89e01a448e3b1fafa64039c1464ee8732f11a5341a6f41e0c202294736ed64db1a84" );
	hash_test( mist::sha384( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ), "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "b12932b0627d1c060942f5447764155655bd4da0c9afa6dd9b9ef53129af1b8fb0195996d2de9ca0df9d821ffee67026" );
	hash_test( mist::sha384( str ), str, "3e0297de7e165288120db74da8e55615a35218457dc0701d6bd59d6d6e34eb58c6f471259ffb841e2c387f9e19ecf745", false );

	std::cout << std::endl << std::endl;

	hash_test( mist::sha512( ), "", "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" );
	hash_test( mist::sha512( "" ), "", "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" );
	hash_test( mist::sha512( "a" ), "a", "1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75" );
	hash_test( mist::sha512( "abc" ), "abc", "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" );
	hash_test( mist::sha512( "message digest" ), "message digest", "107dbf389d9e9f71a3a95f6c055b9251bc5268c2be16d6c13492ea45b0199f3309e16455ab1e96118e8a905d5597b72038ddb372a89826046de66687bb420e7c" );
	hash_test( mist::sha512( "abcdefghijklmnopqrstuvwxyz" ), "abcdefghijklmnopqrstuvwxyz", "4dbff86cc2ca1bae1e16468a05cb9881c97f1753bce3619034898faa1aabe429955a1bf8ec483d7421fe3c1646613a59ed5441fb0f321389f77f48a879c7b1f1" );
	hash_test( mist::sha512( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" ), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "1e07be23c26a86ea37ea810c8ec7809352515a970e9253c26f536cfc7a9996c45c8370583e0a78fa4a90041d71a4ceab7423f19c71b9d5a3e01249f0bebd5894" );
	hash_test( mist::sha512( "12345678901234567890123456789012345678901234567890123456789012345678901234567890" ), "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "72ec1ef1124a45b047e8b7c75a932195135bb61de24ec0d1914042246e0aec3a2354e093d76f3048b456764346900cb130d2a4fd5dd16abb5e30bcb850dee843" );
	hash_test( mist::sha512( str ), str, "6f48984323ba59da05c1f52bf198e8448344a9f2affaaa55ead4849d558a60bd89a62847f651a8bb58390d88e88526224db65eb727d6c62f23ad655877c1451f", false );

	return( 0 );
}



