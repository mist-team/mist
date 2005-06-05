#include <iostream>
#include <mist/md5.h>
#include <mist/sha.h>


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


int main( int argc, char *argv[] )
{
	using namespace std;

	md5_test( "", "d41d8cd98f00b204e9800998ecf8427e" );
	md5_test( "a", "0cc175b9c0f1b6a831c399e269772661" );
	md5_test( "abc", "900150983cd24fb0d6963f7d28e17f72" );
	md5_test( "message digest", "f96b697d7cb7938d525a2f31aaf161d0" );
	md5_test( "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" );
	md5_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f" );
	md5_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a" );

	std::cout << std::endl;

	sha1_test( "", "da39a3ee5e6b4b0d3255bfef95601890afd80709" );
	sha1_test( "abc", "a9993e364706816aba3e25717850c26c9cd0d89d" );
	sha1_test( "message digest", "c12252ceda8be8994d5fa0290a47231c1d16aae3" );
	sha1_test( "abcdefghijklmnopqrstuvwxyz", "32d10c7b8cf96570ca04ce37f2a19d84240d3a89" );
	sha1_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "761c457bf73b14d27e9e9265c46f4b4dda11f940" );
	sha1_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "50abf5706a150990a08b2c5ea40fa0e585554732" );
	return( 0 );
}

