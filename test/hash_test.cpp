#include <iostream>
#include <mist/md5.h>
#include <mist/sha.h>


void md2_test( const std::string &in, const std::string &true_result )
{
	mist::md2 md2( in );
	std::cout << "MD2( " << in << " ) = " << md2 << ( md2 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}

void md4_test( const std::string &in, const std::string &true_result )
{
	mist::md4 md4( in );
	std::cout << "MD4( " << in << " ) = " << md4 << ( md4 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}

void md5_test( const std::string &in, const std::string &true_result )
{
	mist::md5 md5( in );
	std::cout << "MD5( " << in << " ) = " << md5 << ( md5 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}

void sha1_test( const std::string &in, const std::string &true_result )
{
	mist::sha1 sha1( in );
	std::cout << "SHA-1( " << in << " ) = " << sha1 << ( sha1 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}

void sha256_test( const std::string &in, const std::string &true_result )
{
	mist::sha256 sha256( in );
	std::cout << "SHA-256( " << in << " ) = " << sha256 << ( sha256 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}

void sha384_test( const std::string &in, const std::string &true_result )
{
	mist::sha384 sha384( in );
	std::cout << "SHA-384( " << in << " ) = " << sha384 << ( sha384 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}

void sha512_test( const std::string &in, const std::string &true_result )
{
	mist::sha512 sha512( in );
	std::cout << "SHA-512( " << in << " ) = " << sha512 << ( sha512 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}


int main( int argc, char *argv[] )
{
	using namespace std;

	md2_test( "", "8350e5a3e24c153df2275c9f80692773" );
	md2_test( "a", "32ec01ec4a6dac72c0ab96fb34c0b5d1" );
	md2_test( "abc", "da853b0d3f88d99b30283a69e6ded6bb" );
	md2_test( "message digest", "ab4f496bfb2a530b219ff33031fe06b0" );
	md2_test( "abcdefghijklmnopqrstuvwxyz", "4e8ddff3650292ab5a4108c3aa47940b" );
	md2_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "da33def2a42df13975352846c30338cd" );
	md2_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "d5976f79d83d3a0dc9806c3c66f3efd8" );

	std::cout << std::endl << std::endl;

	md4_test( "", "31d6cfe0d16ae931b73c59d7e0c089c0" );
	md4_test( "a", "bde52cb31de33e46245e05fbdbd6fb24" );
	md4_test( "abc", "a448017aaf21d8525fc10ae87aa6729d" );
	md4_test( "message digest", "d9130a8164549fe818874806e1c7014b" );
	md4_test( "abcdefghijklmnopqrstuvwxyz", "d79e1c308aa5bbcdeea8ed63df412da9" );
	md4_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "043f8582f241db351ce627e153e7f0e4" );
	md4_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "e33b4ddc9c38f2199c3e7b164fcc0536" );

	std::cout << std::endl << std::endl;

	md5_test( "", "d41d8cd98f00b204e9800998ecf8427e" );
	md5_test( "a", "0cc175b9c0f1b6a831c399e269772661" );
	md5_test( "abc", "900150983cd24fb0d6963f7d28e17f72" );
	md5_test( "message digest", "f96b697d7cb7938d525a2f31aaf161d0" );
	md5_test( "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" );
	md5_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f" );
	md5_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a" );

	std::cout << std::endl << std::endl;

	sha1_test( "", "da39a3ee5e6b4b0d3255bfef95601890afd80709" );
	sha1_test( "abc", "a9993e364706816aba3e25717850c26c9cd0d89d" );
	sha1_test( "message digest", "c12252ceda8be8994d5fa0290a47231c1d16aae3" );
	sha1_test( "abcdefghijklmnopqrstuvwxyz", "32d10c7b8cf96570ca04ce37f2a19d84240d3a89" );
	sha1_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "761c457bf73b14d27e9e9265c46f4b4dda11f940" );
	sha1_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "50abf5706a150990a08b2c5ea40fa0e585554732" );

	std::cout << std::endl << std::endl;

	sha256_test( "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" );
	sha256_test( "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" );
	sha256_test( "message digest", "f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650" );
	sha256_test( "abcdefghijklmnopqrstuvwxyz", "71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73" );
	sha256_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "db4bfcbd4da0cd85a60c3c37d3fbd8805c77f15fc6b1fdfe614ee0a7c8fdb4c0" );
	sha256_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "f371bc4a311f2b009eef952dd83ca80e2b60026c8e935592d0f9c308453c813e" );

	std::cout << std::endl << std::endl;

	sha384_test( "", "38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b" );
	sha384_test( "abc", "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7" );
	sha384_test( "message digest", "473ed35167ec1f5d8e550368a3db39be54639f828868e9454c239fc8b52e3c61dbd0d8b4de1390c256dcbb5d5fd99cd5" );
	sha384_test( "abcdefghijklmnopqrstuvwxyz", "feb67349df3db6f5924815d6c3dc133f091809213731fe5c7b5f4999e463479ff2877f5f2936fa63bb43784b12f3ebb4" );
	sha384_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "1761336e3f7cbfe51deb137f026f89e01a448e3b1fafa64039c1464ee8732f11a5341a6f41e0c202294736ed64db1a84" );
	sha384_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "b12932b0627d1c060942f5447764155655bd4da0c9afa6dd9b9ef53129af1b8fb0195996d2de9ca0df9d821ffee67026" );

	std::cout << std::endl << std::endl;

	sha512_test( "", "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" );
	sha512_test( "abc", "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f" );
	sha512_test( "message digest", "107dbf389d9e9f71a3a95f6c055b9251bc5268c2be16d6c13492ea45b0199f3309e16455ab1e96118e8a905d5597b72038ddb372a89826046de66687bb420e7c" );
	sha512_test( "abcdefghijklmnopqrstuvwxyz", "4dbff86cc2ca1bae1e16468a05cb9881c97f1753bce3619034898faa1aabe429955a1bf8ec483d7421fe3c1646613a59ed5441fb0f321389f77f48a879c7b1f1" );
	sha512_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "1e07be23c26a86ea37ea810c8ec7809352515a970e9253c26f536cfc7a9996c45c8370583e0a78fa4a90041d71a4ceab7423f19c71b9d5a3e01249f0bebd5894" );
	sha512_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "72ec1ef1124a45b047e8b7c75a932195135bb61de24ec0d1914042246e0aec3a2354e093d76f3048b456764346900cb130d2a4fd5dd16abb5e30bcb850dee843" );

	return( 0 );
}



