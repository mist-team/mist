#include <mist/environment.h>
#include <iostream>

int main( int argc, char *argv[] )
{
	using namespace std;


	cout << " 1. OS     : "; cout << mist::os( ) << endl;
	cout << " 2. CPU    : "; cout << mist::cpu( ) << endl;
	cout << " 2. CPUNUM : "; cout << mist::cpu_num( ) << endl;
	cout << " 3. MACHINE: "; cout << mist::machine( ) << endl;
	cout << " 4. USER   : "; cout << mist::user( ) << endl;

	return( 0 );
}
