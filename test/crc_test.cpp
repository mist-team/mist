#include <stdio.h>
#include <time.h>
#include <mist/crc.h>
#include <mist/random.h>

int main( void )
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

	crc8 = mist::__crc__::generate_crc_8( data, DATALEN );

	printf( "CRC-8:%02X\n", crc8 );

	if( mist::__crc__::check_crc_8( data, DATALEN ) )
	{
		printf( "��v���܂����D\n" );
	}
	else
	{
		printf( "��v���܂���ł����D��₂���Ă��܂��D��₌��CRC-8�� %02X �ł��D\n", mist::__crc__::generate_crc_8( data, DATALEN ) );
	}

	// ��₂���I
	data[ DATALEN / 2 ] ++;

	if( mist::__crc__::check_crc_8( data, DATALEN ) )
	{
		printf( "��v���܂����D\n" );
	}
	else
	{
		printf( "��v���܂���ł����D��₂���Ă��܂��D��₌��CRC-8�� %02X �ł��D\n", mist::__crc__::generate_crc_8( data, DATALEN ) );
	}

	printf( "----------------------------------\n" );

	crc16 = mist::__crc__::generate_crc_16( data, DATALEN );

	printf( "CRC-16:%X\n", crc16 );

	if( mist::__crc__::check_crc_16( data, DATALEN ) )
	{
		printf( "��v���܂����D\n" );
	}
	else
	{
		printf( "��v���܂���ł����D��₂���Ă��܂��D��₌��CRC-16�� %04X �ł��D\n", mist::__crc__::generate_crc_16( data, DATALEN ) );
	}

	// ��₂���I
	data[ DATALEN / 3 ] --;

	if( mist::__crc__::check_crc_16( data, DATALEN ) )
	{
		printf( "��v���܂����D\n" );
	}
	else
	{
		printf( "��v���܂���ł����D��₂���Ă��܂��D��₌��CRC-16�� %04X �ł��D\n", mist::__crc__::generate_crc_16( data, DATALEN ) );
	}

	printf( "----------------------------------\n" );

	crc32 = mist::__crc__::generate_crc_32( data, DATALEN );

	printf( "CRC-32:%08X\n", crc32 );

	if( mist::__crc__::check_crc_32( data, DATALEN ) )
	{
		printf( "��v���܂����D\n" );
	}
	else
	{
		printf( "��v���܂���ł����D��₂���Ă��܂��D��₌��CRC-32�� %08X �ł��D\n", mist::__crc__::generate_crc_32( data, DATALEN ) );
	}

	// ��₂���I
	data[ DATALEN / 4 ] += 2;

	if( mist::__crc__::check_crc_32( data, DATALEN ) )
	{
		printf( "��v���܂����D\n" );
	}
	else
	{
		printf( "��v���܂���ł����D��₂���Ă��܂��D��₌��CRC-32�� %08X �ł��D\n", mist::__crc__::generate_crc_32( data, DATALEN ) );
	}

	return 0;
}
