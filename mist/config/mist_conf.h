#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__

#include <iostream>

#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

#if _MSC_VER <= 1200

#define _MIST_CONST( type, name, value ) enum{ name = value }

#else

#define _MIST_CONST( type, name, value ) static const type name = value

#endif


// �^�����𒲂ׂ�

// char ����
// T���P�o�C�g�̕�����^�ł���ΐ^�ɕ]������
template< class T > struct is_char        { _MIST_CONST( bool, value, false ); };
template<> struct is_char< unsigned char >{ _MIST_CONST( bool, value, true  ); };
template<> struct is_char< signed char >  { _MIST_CONST( bool, value, true  ); };
template<> struct is_char< char >         { _MIST_CONST( bool, value, true  ); };

// arithmetic ����
// T ���Z�p�^�ł���ΐ^�ɕ]������B�Đ����^�����������_�^�̂����ꂩ������ɂ�����
template< class T > struct is_arithmetic                  { _MIST_CONST( bool, value, false ); };
template<>          struct is_arithmetic< unsigned char > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned short >{ _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned int >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< unsigned long > { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed char >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed short >  { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed int >    { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< signed long >   { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< bool >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< char >          { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< float >         { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< double >        { _MIST_CONST( bool, value, true  ); };
template<>          struct is_arithmetic< long double >   { _MIST_CONST( bool, value, true  ); };

// �f�[�^�^�̂Ɋւ�����
template< class T >
struct type_trait{ typedef T value_type; };



#define _MIST_ALLOCATE_TEST_			0

#ifdef WIN32

#define _MIST_ALLOCATOR_MEMORY_TRIM__	0	// VC�W����STL�ł̓T�|�[�g���Ă��Ȃ��̂ŃI�t�ɂ���

#else

#define _MIST_ALLOCATOR_MEMORY_TRIM__	1	// VC�ȊO��STL�ł̓T�|�[�g���Ă���̂ŃI���ɂ���

#endif


#ifndef NDEBUG

#define _CHECK_ACCESS_VIOLATION_		1	// �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY_OPERATION_			1	// 1�����z��̉��Z�̐��������`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY1_OPERATION_		1	// 1�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY2_OPERATION_		1	// 2�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY3_OPERATION_		1	// 3�����摜�̉��Z�̐��������`�F�b�N���邩�ǂ���

#define _CHECK_MATRIX_OPERATION_		1	// �s�񉉎Z�̐��������`�F�b�N���邩�ǂ���
#define _USE_EXPRESSION_TEMPLATE_		0	// Expression Template�𗘗p���邩�ǂ���

#else

#define _CHECK_ACCESS_VIOLATION_		0	// �z��v�f�ւ̃A�N�Z�X�ᔽ���`�F�b�N���邩�ǂ���
#define _CHECK_ARRAY_OPERATION_			0
#define _CHECK_ARRAY1_OPERATION_		0
#define _CHECK_ARRAY2_OPERATION_		0
#define _CHECK_ARRAY3_OPERATION_		0

#define _CHECK_MATRIX_OPERATION_		0
#define _USE_EXPRESSION_TEMPLATE_		1

#endif



// MIST�ŗ��p������̃f�[�^�^

// boolean���Z���s�����߂̂���
class boolean
{
public:
	typedef bool& reference;
	typedef const bool& const_reference;
	typedef bool value_type;
	typedef bool* pointer;
	typedef const bool* const_pointer;

private:
	bool value_;

public:
	boolean( ) : value_( false ){ }
	boolean( const boolean &b ) : value_( b.value_ ){ }
	boolean( const value_type &b ) : value_( b ){ }

	const boolean &operator  =( const boolean &b ){ value_ = b.value_;  return( *this ); }
	const boolean &operator  =( const value_type &b ){ value_ = b;   return( *this ); }

	const boolean &operator +=( const boolean &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const boolean &operator -=( const boolean &b ){ value_ = value_ && !b.value_; return( *this ); }
	const boolean &operator *=( const boolean &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const boolean &operator /=( const boolean &b ){ value_ = value_ ==  b.value_; return( *this ); }
	const boolean &operator %=( const boolean &b ){ value_ = value_ && !b.value_; return( *this ); }
	const boolean &operator |=( const boolean &b ){ value_ = value_ ||  b.value_; return( *this ); }
	const boolean &operator &=( const boolean &b ){ value_ = value_ &&  b.value_; return( *this ); }
	const boolean &operator ^=( const boolean &b ){ value_ = value_ !=  b.value_; return( *this ); }

	bool operator ==( const boolean &b ) const { return( value_ == b.value_ ); }
	bool operator !=( const boolean &b ) const { return( value_ != b.value_ ); }
	bool operator < ( const boolean &b ) const { return( value_ <  b.value_ ); }
	bool operator <=( const boolean &b ) const { return( value_ <= b.value_ ); }
	bool operator > ( const boolean &b ) const { return( value_ >  b.value_ ); }
	bool operator >=( const boolean &b ) const { return( value_ >= b.value_ ); }

	value_type get_value( ) const { return( value_ ); }

	operator bool( ) const { return( value_ ); }
};

inline const boolean operator +( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) += b2 ); }
inline const boolean operator -( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) -= b2 ); }
inline const boolean operator *( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) *= b2 ); }
inline const boolean operator /( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) /= b2 ); }
inline const boolean operator %( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) %= b2 ); }
inline const boolean operator |( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) |= b2 ); }
inline const boolean operator &( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) &= b2 ); }
inline const boolean operator ^( const boolean &b1, const boolean &b2 ){ return( boolean( b1 ) ^= b2 ); }

inline std::ostream &operator <<( std::ostream &out, const boolean &v )
{
	out << v.get_value( );
	return( out );
}


// �J���[�摜�p
template< class T >
struct rgb
{
public:
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;

public:
	value_type r;
	value_type g;
	value_type b;

	rgb( const value_type &rr, const value_type &gg, const value_type &bb ) : r( rr ), g( gg ), b( bb ){ }

	const rgb &operator +=( const rgb &c ){ r += c.r; g += c.g; b += c.b; return( *this ); }
	const rgb &operator -=( const rgb &c ){ r -= c.r; g -= c.g; b -= c.b; return( *this ); }
	const rgb &operator *=( const rgb &c ){ r *= c.r; g *= c.g; b *= c.b; return( *this ); }
	const rgb &operator /=( const rgb &c ){ r /= c.r; g /= c.g; b /= c.b; return( *this ); }
	const rgb &operator %=( const rgb &c ){ r %= c.r; g %= c.g; b %= c.b; return( *this ); }
	const rgb &operator |=( const rgb &c ){ r |= c.r; g |= c.g; b |= c.b; return( *this ); }
	const rgb &operator &=( const rgb &c ){ r &= c.r; g &= c.g; b &= c.b; return( *this ); }
	const rgb &operator ^=( const rgb &c ){ r ^= c.r; g ^= c.g; b ^= c.b; return( *this ); }

	bool operator ==( const rgb &c ) const { return( r == c.r && g == c.g && c == c.b ); }
	bool operator !=( const rgb &c ) const { return( !( *this == c ) ); }
	bool operator < ( const rgb &c ) const { return( r <  c.r || ( r == c.r && g <  c.g ) || ( r == c.r && g == c.g  &&c <  c.b ) ); }
	bool operator <=( const rgb &c ) const { return( !( c < *this ) ); }
	bool operator > ( const rgb &c ) const { return( c < *this ); }
	bool operator >=( const rgb &c ) const { return( !( *this < c ) ); }

	// NTSC�n���d���ϖ@�ɂ��C�O���[�X�P�[���֕ϊ�����
	value_type get_value( ) const { return( static_cast< value_type >( r * 0.298912 + g * 0.586611 + b + 0.114478 ) ); }

	operator value_type( ) const { return( get_value( ) ); }
};

template < class T > inline const rgb< T > operator +( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) += c2 ); }
template < class T > inline const rgb< T > operator -( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) -= c2 ); }
template < class T > inline const rgb< T > operator *( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) *= c2 ); }
template < class T > inline const rgb< T > operator /( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) /= c2 ); }
template < class T > inline const rgb< T > operator %( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) %= c2 ); }
template < class T > inline const rgb< T > operator |( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) |= c2 ); }
template < class T > inline const rgb< T > operator &( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) &= c2 ); }
template < class T > inline const rgb< T > operator ^( const rgb< T > &c1, const rgb< T > &c2 ){ return( rgb< T >( c1 ) ^= c2 ); }

template < class T > inline std::ostream &operator <<( std::ostream &out, const rgb< T > &c )
{
	out << "( ";
	out << c.r << ", ";
	out << c.g << ", ";
	out << c.b << " )";
	return( out );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif
