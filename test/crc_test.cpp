#include <iostream>
#include <ctime>

#include <mist/crc.h>
#include <mist/random.h>

int main( int argc, char *argv[] )
{
	const int		DATALEN = 1024;
	unsigned char	data[ DATALEN ];
	unsigned char	crc8;
	unsigned short	crc16;
	unsigned		crc32;

	mist::uniform::random	rnd( clock( ) );

	for( int n = 0 ; n < DATALEN ; n++ )
	{
		data[ n ] = ( unsigned char ) ( rnd.int32( ) >> 24 );
	}

	crc8 = mist::crc< 8 >::generate_implant( data, DATALEN );

	printf( "CRC-8:%02X\n", crc8 );

	if( mist::crc< 8 >::check_implant( data, DATALEN ) )
	{
		printf( "CRC is matched.\n" );
	}
	else
	{
		printf( "CRC is not matched. CRC-8 is %02X.\n", mist::crc< 8 >::generate_implant( data, DATALEN ) );
	}

	// ‰όβ‚‚·‚ιI
	data[ DATALEN / 2 ] ++;

	if( mist::crc< 8 >::check_implant( data, DATALEN ) )
	{
		printf( "CRC is matched.\n" );
	}
	else
	{
		printf( "CRC is not matched. CRC-8 is %02X.\n", mist::crc< 8 >::generate_implant( data, DATALEN ) );
	}

	printf( "----------------------------------\n" );

	crc16 = mist::crc< 16 >::generate_implant( data, DATALEN );

	printf( "CRC-16:%X\n", crc16 );

	if( mist::crc< 16 >::check_implant( data, DATALEN ) )
	{
		printf( "CRC is matched.\n" );
	}
	else
	{
		printf( "CRC is not matched. CRC-16 is %04X.\n", mist::crc< 16 >::generate_implant( data, DATALEN ) );
	}

	// ‰όβ‚‚·‚ιI
	data[ DATALEN / 3 ] --;

	if( mist::crc< 16 >::check_implant( data, DATALEN ) )
	{
		printf( "CRC is matched.\n" );
	}
	else
	{
		printf( "CRC is not matched. CRC-16 is %04X.\n", mist::crc< 16 >::generate_implant( data, DATALEN ) );
	}

	printf( "----------------------------------\n" );

	crc32 = mist::crc< 32 >::generate_implant( data, DATALEN );

	printf( "CRC-32:%08X\n", crc32 );

	if( mist::crc< 32 >::check_implant( data, DATALEN ) )
	{
		printf( "CRC is matched.\n" );
	}
	else
	{
		printf( "CRC is not matched. CRC-32 is %08X.\n", mist::crc< 32 >::generate_implant( data, DATALEN ) );
	}

	// ‰όβ‚‚·‚ιI
	data[ DATALEN / 4 ] += 2;

	if( mist::crc< 32 >::check_implant( data, DATALEN ) )
	{
		printf( "CRC is matched.\n" );
	}
	else
	{
		printf( "CRC is not matched. CRC-32 is %08X.\n", mist::crc< 32 >::generate_implant( data, DATALEN ) );
	}

	return( 0 );
}
