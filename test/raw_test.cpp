#include <mist/io/raw.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;

	mist::array3< short > image;
	typedef mist::array3< short >::size_type size_type;
	size_type w = 512, h = 512, d = 189;
	double x = 0.625, y = 0.625, z = 1.0;

	char buff[4096];
	string infilename = "";
	string outfilename = "C:\\test.raw";

	printf( "Input filename:" );
	if( gets( buff ) == NULL || strcmp( buff, "" ) == 0 )
	{
		fprintf( stderr,  "ファイル名を入力する必要があります！！\n" );
		return( 1 );
	}
	infilename = buff;

	printf( "Output filename:" );
	if( gets( buff ) != NULL && strcmp( buff, "" ) != 0 )
	{
		outfilename = buff;
	}

	printf( "Input Image Width( %ld ):", w );
	if( gets( buff ) != NULL && strcmp( buff, "" ) != 0 )
	{
		w = atoi( buff );
	}

	printf( "Input Image Height( %ld ):", h );
	if( gets( buff ) != NULL && strcmp( buff, "" ) != 0 )
	{
		h = atoi( buff );
	}

	printf( "Input Image Depth( %ld ):", d );
	if( gets( buff ) != NULL && strcmp( buff, "" ) != 0 )
	{
		d = atoi( buff );
	}

	printf( "Input Image ResoX( %f ):", x );
	if( gets( buff ) != NULL && strcmp( buff, "" ) != 0 )
	{
		x = atof( buff );
	}

	printf( "Input Image ResoY( %f ):", y );
	if( gets( buff ) != NULL && strcmp( buff, "" ) != 0 )
	{
		y = atof( buff );
	}

	printf( "Input Image ResoZ( %f ):", z );
	if( gets( buff ) != NULL && strcmp( buff, "" ) != 0 )
	{
		z = atof( buff );
	}

	printf( "Input image information: ( %ld, %ld, %ld ) - ( %f, %f, %f )\n", w, h, d, x, y, z );

	mist::read_raw( image, infilename, w, h, d, x, y, z );

	printf( "Input image information: ( %ld, %ld, %ld ) - ( %f, %f, %f )\n", image.width( ), image.height( ), image.depth( ), image.reso1( ), image.reso2( ), image.reso3( ) );

	mist::write_raw_gz( image, outfilename );
}
