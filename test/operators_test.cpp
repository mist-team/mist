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
	cout << " 1. Operator A + B = \t" << ( v1 + v2 ) << endl;
	cout << " 2. Operator A - B = \t" << ( v1 - v2 ) << endl;
	cout << " 3. Operator A * B = \t" << ( v1 * v2 ) << endl;
	cout << " 4. Operator A / B = \t" << ( v1 / v2 ) << endl;

	cout << " 5. Operator A % B = \t" << ( v1 % v2 ) << endl;
	cout << " 6. Operator A | B = \t" << ( v1 | v2 ) << endl;
	cout << " 7. Operator A & B = \t" << ( v1 & v2 ) << endl;
	cout << " 8. Operator A ^ B = \t" << ( v1 ^ v2 ) << endl;

	cout << " 9. Operator A == B = \t" << ( v1 == v2 ) << endl;
	cout << "10. Operator A != B = \t" << ( v1 != v2 ) << endl;
	cout << "11. Operator A <  B = \t" << ( v1 <  v2 ) << endl;
	cout << "12. Operator A <= B = \t" << ( v1 <= v2 ) << endl;
	cout << "13. Operator A >  B = \t" << ( v1 >  v2 ) << endl;
	cout << "14. Operator A >= B = \t" << ( v1 >= v2 ) << endl;

	return( 0 );
}
