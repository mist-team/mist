#include <mist/io/wav.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::array< signed int > sound;

	mist::read_wav( sound, argv[1] );
}
