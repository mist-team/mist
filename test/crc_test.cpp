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
#include <ctime>

#include <mist/crc.h>
#include <mist/random.h>

int main( int argc, char *argv[] )
{
	const int		DATALEN = 1024;
	unsigned char	data[ DATALEN ];
	unsigned char	crc8;
	unsigned short	crc16;
	unsigned int	crc32;

	mist::uniform::random	rnd( clock( ) );

	for( int n = 0 ; n < DATALEN ; n++ )
	{
		data[ n ] = ( unsigned char ) ( rnd.int32( ) >> 24 );
	}

	crc8 = ( unsigned char )mist::crc< 8 >::generate_implant( data, DATALEN );

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

	crc16 = ( unsigned short )mist::crc< 16 >::generate_implant( data, DATALEN );

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

	crc32 = ( unsigned int )mist::crc< 32 >::generate_implant( data, DATALEN );

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
