#include <iostream>
#include <mist/limits.h>


int main( int argc, char *argv[] )
{
	using namespace std;
	using namespace mist;

	cout << " 1. The minimum value of          char  is\t" << (int)numeric_limits< char >::min( ) << endl;
	cout << " 2. The minimum value of  signed  char  is\t" << (int)numeric_limits< signed char >::min( ) << endl;
	cout << " 3. The minimum value of unsigned char  is\t" << (int)numeric_limits< unsigned char >::min( ) << endl;
	cout << " 4. The minimum value of  signed  short is\t" << (int)numeric_limits< signed short >::min( ) << endl;
	cout << " 5. The minimum value of unsigned short is\t" << numeric_limits< unsigned short >::min( ) << endl;
	cout << " 6. The minimum value of  signed  int   is\t" << numeric_limits< signed int >::min( ) << endl;
	cout << " 7. The minimum value of unsigned int   is\t" << numeric_limits< unsigned int >::min( ) << endl;
	cout << " 8. The minimum value of  signed  long  is\t" << numeric_limits< signed long >::min( ) << endl;
	cout << " 9. The minimum value of unsigned long  is\t" << numeric_limits< unsigned long >::min( ) << endl;
	cout << "10. The minimum value of          float is\t" << numeric_limits< float >::min( ) << endl;
	cout << "11. The minimum value of         double is\t" << numeric_limits< double >::min( ) << endl;
	cout << "12. The minimum value of    long double is\t" << numeric_limits< long double >::min( ) << endl;

	cout << "13. The maximum value of          char  is\t" << (int)numeric_limits< char >::max( ) << endl;
	cout << "14. The maximum value of  signed  char  is\t" << (int)numeric_limits< signed char >::max( ) << endl;
	cout << "15. The maximum value of unsigned char  is\t" << (int)numeric_limits< unsigned char >::max( ) << endl;
	cout << "16. The maximum value of  signed  short is\t" << numeric_limits< signed short >::max( ) << endl;
	cout << "17. The maximum value of unsigned short is\t" << numeric_limits< unsigned short >::max( ) << endl;
	cout << "18. The maximum value of  signed  int   is\t" << numeric_limits< signed int >::max( ) << endl;
	cout << "19. The maximum value of unsigned int   is\t" << numeric_limits< unsigned int >::max( ) << endl;
	cout << "20. The maximum value of  signed  long  is\t" << numeric_limits< signed long >::max( ) << endl;
	cout << "21. The maximum value of unsigned long  is\t" << numeric_limits< unsigned long >::max( ) << endl;
	cout << "22. The maximum value of          float is\t" << numeric_limits< float >::max( ) << endl;
	cout << "23. The maximum value of         double is\t" << numeric_limits< double >::max( ) << endl;
	cout << "24. The maximum value of    long double is\t" << numeric_limits< long double >::max( ) << endl;

	cout << "25. Is          char  signed ?\t" << numeric_limits< char >::is_signed << endl;
	cout << "26. Is  signed  char  signed ?\t" << numeric_limits< signed char >::is_signed << endl;
	cout << "27. Is unsigned char  signed ?\t" << numeric_limits< unsigned char >::is_signed << endl;
	cout << "28. Is  signed  short signed ?\t" << numeric_limits< signed short >::is_signed << endl;
	cout << "29. Is unsigned short signed ?\t" << numeric_limits< unsigned short >::is_signed << endl;
	cout << "30. Is  signed  int   signed ?\t" << numeric_limits< signed int >::is_signed << endl;
	cout << "31. Is unsigned int   signed ?\t" << numeric_limits< unsigned int >::is_signed << endl;
	cout << "32. Is  signed  long  signed ?\t" << numeric_limits< signed long >::is_signed << endl;
	cout << "33. Is unsigned long  signed ?\t" << numeric_limits< unsigned long >::is_signed << endl;
	cout << "34. Is          float signed ?\t" << numeric_limits< float >::is_signed << endl;
	cout << "35. Is         double signed ?\t" << numeric_limits< double >::is_signed << endl;
	cout << "36. Is    long double signed ?\t" << numeric_limits< long double >::is_signed << endl;

	cout << "37. Is          char  integer ?\t" << numeric_limits< char >::is_integer << endl;
	cout << "38. Is  signed  char  integer ?\t" << numeric_limits< signed char >::is_integer << endl;
	cout << "39. Is unsigned char  integer ?\t" << numeric_limits< unsigned char >::is_integer << endl;
	cout << "40. Is  signed  short integer ?\t" << numeric_limits< signed short >::is_integer << endl;
	cout << "41. Is unsigned short integer ?\t" << numeric_limits< unsigned short >::is_integer << endl;
	cout << "42. Is  signed  int   integer ?\t" << numeric_limits< signed int >::is_integer << endl;
	cout << "43. Is unsigned int   integer ?\t" << numeric_limits< unsigned int >::is_integer << endl;
	cout << "44. Is  signed  long  integer ?\t" << numeric_limits< signed long >::is_integer << endl;
	cout << "45. Is unsigned long  integer ?\t" << numeric_limits< unsigned long >::is_integer << endl;
	cout << "46. Is          float integer ?\t" << numeric_limits< float >::is_integer << endl;
	cout << "47. Is         double integer ?\t" << numeric_limits< double >::is_integer << endl;
	cout << "48. Is    long double integer ?\t" << numeric_limits< long double >::is_integer << endl;
}
