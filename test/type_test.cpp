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
#include <mist/limits.h>


int main( int argc, char *argv[] )
{
	using namespace std;
	using namespace mist;

	cout << "  1. The type is char [          char  ]: \t"; cout << (int)is_char< char >::value << endl;
	cout << "  2. The type is char [  signed  char  ]: \t"; cout << (int)is_char< signed char >::value << endl;
	cout << "  3. The type is char [ unsigned char  ]: \t"; cout << (int)is_char< unsigned char >::value << endl;
	cout << "  4. The type is char [  signed  short ]: \t"; cout << (int)is_char< signed short >::value << endl;
	cout << "  5. The type is char [ unsigned short ]: \t"; cout << (int)is_char< unsigned short >::value << endl;
	cout << "  6. The type is char [  signed  int   ]: \t"; cout << (int)is_char< signed int >::value << endl;
	cout << "  7. The type is char [ unsigned int   ]: \t"; cout << (int)is_char< unsigned int >::value << endl;
	cout << "  8. The type is char [  signed  long  ]: \t"; cout << (int)is_char< signed  long >::value << endl;
	cout << "  9. The type is char [ unsigned long  ]: \t"; cout << (int)is_char< unsigned long >::value << endl;
	cout << " 10. The type is char [          float ]: \t"; cout << (int)is_char< float >::value << endl;
	cout << " 11. The type is char [         double ]: \t"; cout << (int)is_char< double >::value << endl;
	cout << " 12. The type is char [    long double ]: \t"; cout << (int)is_char< long double >::value << endl;
	cout << " 13. The type is char [         size_t ]: \t"; cout << (int)is_char< size_t >::value << endl;
	cout << " 14. The type is char [      ptrdiff_t ]: \t"; cout << (int)is_char< ptrdiff_t >::value << endl;

	cout << "  1. The type is float [          char  ]: \t"; cout << (int)is_float< char >::value << endl;
	cout << "  2. The type is float [  signed  char  ]: \t"; cout << (int)is_float< signed char >::value << endl;
	cout << "  3. The type is float [ unsigned char  ]: \t"; cout << (int)is_float< unsigned char >::value << endl;
	cout << "  4. The type is float [  signed  short ]: \t"; cout << (int)is_float< signed short >::value << endl;
	cout << "  5. The type is float [ unsigned short ]: \t"; cout << (int)is_float< unsigned short >::value << endl;
	cout << "  6. The type is float [  signed  int   ]: \t"; cout << (int)is_float< signed int >::value << endl;
	cout << "  7. The type is float [ unsigned int   ]: \t"; cout << (int)is_float< unsigned int >::value << endl;
	cout << "  8. The type is float [  signed  long  ]: \t"; cout << (int)is_float< signed  long >::value << endl;
	cout << "  9. The type is float [ unsigned long  ]: \t"; cout << (int)is_float< unsigned long >::value << endl;
	cout << " 10. The type is float [          float ]: \t"; cout << (int)is_float< float >::value << endl;
	cout << " 11. The type is float [         double ]: \t"; cout << (int)is_float< double >::value << endl;
	cout << " 12. The type is float [    long double ]: \t"; cout << (int)is_float< long double >::value << endl;
	cout << " 13. The type is float [         size_t ]: \t"; cout << (int)is_float< size_t >::value << endl;
	cout << " 14. The type is float [      ptrdiff_t ]: \t"; cout << (int)is_float< ptrdiff_t >::value << endl;

	cout << "  1. The type is arithmetic [          char  ]: \t"; cout << (int)is_arithmetic< char >::value << endl;
	cout << "  2. The type is arithmetic [  signed  char  ]: \t"; cout << (int)is_arithmetic< signed char >::value << endl;
	cout << "  3. The type is arithmetic [ unsigned char  ]: \t"; cout << (int)is_arithmetic< unsigned char >::value << endl;
	cout << "  4. The type is arithmetic [  signed  short ]: \t"; cout << (int)is_arithmetic< signed short >::value << endl;
	cout << "  5. The type is arithmetic [ unsigned short ]: \t"; cout << (int)is_arithmetic< unsigned short >::value << endl;
	cout << "  6. The type is arithmetic [  signed  int   ]: \t"; cout << (int)is_arithmetic< signed int >::value << endl;
	cout << "  7. The type is arithmetic [ unsigned int   ]: \t"; cout << (int)is_arithmetic< unsigned int >::value << endl;
	cout << "  8. The type is arithmetic [  signed  long  ]: \t"; cout << (int)is_arithmetic< signed  long >::value << endl;
	cout << "  9. The type is arithmetic [ unsigned long  ]: \t"; cout << (int)is_arithmetic< unsigned long >::value << endl;
	cout << " 10. The type is arithmetic [          float ]: \t"; cout << (int)is_arithmetic< float >::value << endl;
	cout << " 11. The type is arithmetic [         double ]: \t"; cout << (int)is_arithmetic< double >::value << endl;
	cout << " 12. The type is arithmetic [    long double ]: \t"; cout << (int)is_arithmetic< long double >::value << endl;
	cout << " 13. The type is arithmetic [         size_t ]: \t"; cout << (int)is_arithmetic< size_t >::value << endl;
	cout << " 14. The type is arithmetic [      ptrdiff_t ]: \t"; cout << (int)is_arithmetic< ptrdiff_t >::value << endl;

	cout << "  1. sizeof(          char  ) = "; cout << (int)sizeof( char ) << endl;
	cout << "  2. sizeof(  signed  char  ) = "; cout << (int)sizeof( signed char ) << endl;
	cout << "  3. sizeof( unsigned char  ) = "; cout << (int)sizeof( unsigned char ) << endl;
	cout << "  4. sizeof(  signed  short ) = "; cout << (int)sizeof( signed short ) << endl;
	cout << "  5. sizeof( unsigned short ) = "; cout << (int)sizeof( unsigned short ) << endl;
	cout << "  6. sizeof(  signed  int   ) = "; cout << (int)sizeof( signed int ) << endl;
	cout << "  7. sizeof( unsigned int   ) = "; cout << (int)sizeof( unsigned int ) << endl;
	cout << "  8. sizeof(  signed  long  ) = "; cout << (int)sizeof( signed  long ) << endl;
	cout << "  9. sizeof( unsigned long  ) = "; cout << (int)sizeof( unsigned long ) << endl;
	cout << " 10. sizeof(          float ) = "; cout << (int)sizeof( float ) << endl;
	cout << " 11. sizeof(         double ) = "; cout << (int)sizeof( double ) << endl;
	cout << " 12. sizeof(    long double ) = "; cout << (int)sizeof( long double ) << endl;
	cout << " 13. sizeof(         size_t ) = "; cout << (int)sizeof( size_t ) << endl;
	cout << " 14. sizeof(      ptrdiff_t ) = "; cout << (int)sizeof( ptrdiff_t ) << endl;

	return( 0 );
}
