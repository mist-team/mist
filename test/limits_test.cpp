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

	cout << " 1. The minimum value of          char  is\t"; cout << (int)type_limits< char >::minimum( ) << endl;
	cout << " 2. The minimum value of  signed  char  is\t"; cout << (int)type_limits< signed char >::minimum( ) << endl;
	cout << " 3. The minimum value of unsigned char  is\t"; cout << (int)type_limits< unsigned char >::minimum( ) << endl;
	cout << " 4. The minimum value of  signed  short is\t"; cout << (int)type_limits< signed short >::minimum( ) << endl;
	cout << " 5. The minimum value of unsigned short is\t"; cout << type_limits< unsigned short >::minimum( ) << endl;
	cout << " 6. The minimum value of  signed  int   is\t"; cout << type_limits< signed int >::minimum( ) << endl;
	cout << " 7. The minimum value of unsigned int   is\t"; cout << type_limits< unsigned int >::minimum( ) << endl;
	cout << " 8. The minimum value of  signed  long  is\t"; cout << type_limits< signed long >::minimum( ) << endl;
	cout << " 9. The minimum value of unsigned long  is\t"; cout << type_limits< unsigned long >::minimum( ) << endl;
	cout << "10. The minimum value of          float is\t"; cout << type_limits< float >::minimum( ) << endl;
	cout << "11. The minimum value of         double is\t"; cout << type_limits< double >::minimum( ) << endl;
	cout << "12. The minimum value of    long double is\t"; cout << type_limits< long double >::minimum( ) << endl;
	cout << "13. The minimum value of         size_t is\t"; cout << type_limits< size_t >::minimum( ) << endl;
	cout << "14. The minimum value of      ptrdiff_t is\t"; cout << type_limits< ptrdiff_t >::minimum( ) << endl;

	cout << "15. The maximum value of          char  is\t"; cout << (int)type_limits< char >::maximum( ) << endl;
	cout << "16. The maximum value of  signed  char  is\t"; cout << (int)type_limits< signed char >::maximum( ) << endl;
	cout << "17. The maximum value of unsigned char  is\t"; cout << (int)type_limits< unsigned char >::maximum( ) << endl;
	cout << "18. The maximum value of  signed  short is\t"; cout << type_limits< signed short >::maximum( ) << endl;
	cout << "19. The maximum value of unsigned short is\t"; cout << type_limits< unsigned short >::maximum( ) << endl;
	cout << "20. The maximum value of  signed  int   is\t"; cout << type_limits< signed int >::maximum( ) << endl;
	cout << "21. The maximum value of unsigned int   is\t"; cout << type_limits< unsigned int >::maximum( ) << endl;
	cout << "22. The maximum value of  signed  long  is\t"; cout << type_limits< signed long >::maximum( ) << endl;
	cout << "23. The maximum value of unsigned long  is\t"; cout << type_limits< unsigned long >::maximum( ) << endl;
	cout << "24. The maximum value of          float is\t"; cout << type_limits< float >::maximum( ) << endl;
	cout << "25. The maximum value of         double is\t"; cout << type_limits< double >::maximum( ) << endl;
	cout << "26. The maximum value of    long double is\t"; cout << type_limits< long double >::maximum( ) << endl;
	cout << "27. The maximum value of         size_t is\t"; cout << type_limits< size_t >::maximum( ) << endl;
	cout << "28. The maximum value of      ptrdiff_t is\t"; cout << type_limits< ptrdiff_t >::maximum( ) << endl;

	cout << "29. The tiny value of          char  is\t"; cout << (int)type_limits< char >::tiny( ) << endl;
	cout << "30. The tiny value of  signed  char  is\t"; cout << (int)type_limits< signed char >::tiny( ) << endl;
	cout << "31. The tiny value of unsigned char  is\t"; cout << (int)type_limits< unsigned char >::tiny( ) << endl;
	cout << "32. The tiny value of  signed  short is\t"; cout << type_limits< signed short >::tiny( ) << endl;
	cout << "33. The tiny value of unsigned short is\t"; cout << type_limits< unsigned short >::tiny( ) << endl;
	cout << "34. The tiny value of  signed  int   is\t"; cout << type_limits< signed int >::tiny( ) << endl;
	cout << "35. The tiny value of unsigned int   is\t"; cout << type_limits< unsigned int >::tiny( ) << endl;
	cout << "36. The tiny value of  signed  long  is\t"; cout << type_limits< signed long >::tiny( ) << endl;
	cout << "37. The tiny value of unsigned long  is\t"; cout << type_limits< unsigned long >::tiny( ) << endl;
	cout << "38. The tiny value of          float is\t"; cout << type_limits< float >::tiny( ) << endl;
	cout << "39. The tiny value of         double is\t"; cout << type_limits< double >::tiny( ) << endl;
	cout << "40. The tiny value of    long double is\t"; cout << type_limits< long double >::tiny( ) << endl;
	cout << "41. The tiny value of         size_t is\t"; cout << type_limits< size_t >::tiny( ) << endl;
	cout << "42. The tiny value of      ptrdiff_t is\t"; cout << type_limits< ptrdiff_t >::tiny( ) << endl;

	cout << "43. Is          char  signed ?\t"; cout << type_limits< char >::is_signed << endl;
	cout << "44. Is  signed  char  signed ?\t"; cout << type_limits< signed char >::is_signed << endl;
	cout << "45. Is unsigned char  signed ?\t"; cout << type_limits< unsigned char >::is_signed << endl;
	cout << "46. Is  signed  short signed ?\t"; cout << type_limits< signed short >::is_signed << endl;
	cout << "47. Is unsigned short signed ?\t"; cout << type_limits< unsigned short >::is_signed << endl;
	cout << "48. Is  signed  int   signed ?\t"; cout << type_limits< signed int >::is_signed << endl;
	cout << "49. Is unsigned int   signed ?\t"; cout << type_limits< unsigned int >::is_signed << endl;
	cout << "50. Is  signed  long  signed ?\t"; cout << type_limits< signed long >::is_signed << endl;
	cout << "51. Is unsigned long  signed ?\t"; cout << type_limits< unsigned long >::is_signed << endl;
	cout << "52. Is          float signed ?\t"; cout << type_limits< float >::is_signed << endl;
	cout << "53. Is         double signed ?\t"; cout << type_limits< double >::is_signed << endl;
	cout << "54. Is    long double signed ?\t"; cout << type_limits< long double >::is_signed << endl;
	cout << "55. Is         size_t signed ?\t"; cout << type_limits< size_t >::is_signed << endl;
	cout << "56. Is      ptrdiff_t signed ?\t"; cout << type_limits< ptrdiff_t >::is_signed << endl;

	cout << "57. Is          char  integer ?\t"; cout << type_limits< char >::is_integer << endl;
	cout << "58. Is  signed  char  integer ?\t"; cout << type_limits< signed char >::is_integer << endl;
	cout << "59. Is unsigned char  integer ?\t"; cout << type_limits< unsigned char >::is_integer << endl;
	cout << "60. Is  signed  short integer ?\t"; cout << type_limits< signed short >::is_integer << endl;
	cout << "61. Is unsigned short integer ?\t"; cout << type_limits< unsigned short >::is_integer << endl;
	cout << "62. Is  signed  int   integer ?\t"; cout << type_limits< signed int >::is_integer << endl;
	cout << "63. Is unsigned int   integer ?\t"; cout << type_limits< unsigned int >::is_integer << endl;
	cout << "64. Is  signed  long  integer ?\t"; cout << type_limits< signed long >::is_integer << endl;
	cout << "65. Is unsigned long  integer ?\t"; cout << type_limits< unsigned long >::is_integer << endl;
	cout << "66. Is          float integer ?\t"; cout << type_limits< float >::is_integer << endl;
	cout << "67. Is         double integer ?\t"; cout << type_limits< double >::is_integer << endl;
	cout << "68. Is    long double integer ?\t"; cout << type_limits< long double >::is_integer << endl;
	cout << "69. Is         size_t integer ?\t"; cout << type_limits< size_t >::is_integer << endl;
	cout << "70. Is      ptrdiff_t integer ?\t"; cout << type_limits< ptrdiff_t >::is_integer << endl;

	return( 0 );
}
