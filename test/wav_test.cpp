#include <mist/io/wav.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

	unsigned int sampling_rate = 44100;

	if( argc == 2 )
	{
		mist::array< signed int > sound;
		mist::read_wav( sound, argv[1], sampling_rate );
		mist::write_wav( sound, sound, std::string( argv[1] ) + ".wav", 8, sampling_rate );
	}
	else if( argc == 3 )
	{
		mist::array< signed int > left, right;
		mist::read_wav( left, argv[1], sampling_rate );
		mist::read_wav( right, argv[2] );
		mist::write_wav( left, right, std::string( argv[1] ) + ".wav", 8, sampling_rate );
	}
}
