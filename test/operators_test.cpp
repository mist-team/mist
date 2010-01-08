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
#include <mist/operator/operators.h>

template < class T >
class value : public mist::implement_operators< value< T > >
{
private:
	T val_;

public:
	T get_value( ) const { return( val_ ); }

	value( const T &v ){ val_ = v; }
	value( const value &v ){ val_ = v.val_; }

	bool   operator  <( const value& v ) const { return val_ <  v.val_; }
	bool   operator ==( const value& v ) const { return val_ == v.val_; }
	value& operator +=( const value& v ) { val_+=v.val_; return( *this ); }
	value& operator -=( const value& v ) { val_-=v.val_; return( *this ); }
	value& operator *=( const value& v ) { val_*=v.val_; return( *this ); }
	value& operator /=( const value& v ) { val_/=v.val_; return( *this ); }
	value& operator %=( const value& v ) { val_%=v.val_; return( *this ); }
	value& operator |=( const value& v ) { val_|=v.val_; return( *this ); }
	value& operator &=( const value& v ) { val_&=v.val_; return( *this ); }
	value& operator ^=( const value& v ) { val_^=v.val_; return( *this ); }
	value& operator ++( )                { ++val_;       return( *this ); }
	value& operator --( )                { --val_;       return( *this ); }
};

template < class T >
inline ::std::ostream &operator <<( ::std::ostream &out, const value< T > &v )
{
	out << v.get_value( );
	return( out );
}

int main( int argc, char *argv[] )
{
	using namespace std;

	value< int > v1( 2 ), v2( 2 );

	cout << " ( A, B ) = ( " << v1 << ", " << v2 << " )" << endl;
	cout << " 1. Operator A + B = \t"; cout << ( v1 + v2 ) << endl;
	cout << " 2. Operator A - B = \t"; cout << ( v1 - v2 ) << endl;
	cout << " 3. Operator A * B = \t"; cout << ( v1 * v2 ) << endl;
	cout << " 4. Operator A / B = \t"; cout << ( v1 / v2 ) << endl;

	cout << " 5. Operator A % B = \t"; cout << ( v1 % v2 ) << endl;
	cout << " 6. Operator A | B = \t"; cout << ( v1 | v2 ) << endl;
	cout << " 7. Operator A & B = \t"; cout << ( v1 & v2 ) << endl;
	cout << " 8. Operator A ^ B = \t"; cout << ( v1 ^ v2 ) << endl;

	cout << " 9. Operator A == B = \t"; cout << ( v1 == v2 ) << endl;
	cout << "10. Operator A != B = \t"; cout << ( v1 != v2 ) << endl;
	cout << "11. Operator A <  B = \t"; cout << ( v1 <  v2 ) << endl;
	cout << "12. Operator A <= B = \t"; cout << ( v1 <= v2 ) << endl;
	cout << "13. Operator A >  B = \t"; cout << ( v1 >  v2 ) << endl;
	cout << "14. Operator A >= B = \t"; cout << ( v1 >= v2 ) << endl;

	return( 0 );
}
