#include <iostream>
#include <mist/md5.h>


void hash_test( const std::string &in, const std::string &true_result )
{
	mist::md5 md5( in );
	std::cout << "MD5( " << in << " ) = " << md5 << ( md5 == true_result ? " ... OK!" : " ... NO!" ) << std::endl;
}


int main( int argc, char *argv[] )
{
	using namespace std;

	hash_test( "", "d41d8cd98f00b204e9800998ecf8427e" );
	hash_test( "a", "0cc175b9c0f1b6a831c399e269772661" );
	hash_test( "abc", "900150983cd24fb0d6963f7d28e17f72" );
	hash_test( "message digest", "f96b697d7cb7938d525a2f31aaf161d0" );
	hash_test( "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" );
	hash_test( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f" );
	hash_test( "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a" );

	return( 0 );
}
