#include <iostream>
#include <mist/io/wav.h>
#include <mist/io/mp3.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	unsigned int sampling_rate = 44100;

	if( argc == 2 )
	{
		mist::array< signed int > sound;
		mist::read_wav( sound, argv[1], sampling_rate );
		mist::write_wav( sound, std::string( argv[1] ) + ".wav", 16, sampling_rate );
	}

	return( 0 );
}
