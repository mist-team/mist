#ifndef __INCLUDE_MIST_CONF_H__
#define __INCLUDE_MIST_CONF_H__

#include <iostream>
#include <iterator>


#ifndef _MIST_BEGIN
#define _MIST_BEGIN		namespace mist{
#endif

#ifndef _MIST_END
#define _MIST_END		}
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



#if _MSC_VER <= 1200

#define __MIST_MSVC__		6

#elif _MSC_VER > 1200

#define __MIST_MSVC__		7

#endif


#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6

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


// mist�R���e�i�ŗ��p����1��������p�����_���A�N�Z�X�C�e���[�^
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef Reference reference;
	typedef size_t size_type;
	typedef Distance difference_type;

private:
	pointer data_;
	size_type diff_pointer_;

public:
	// �R���X�g���N�^
	mist_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }
	mist_iterator1( const mist_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }

	// �R�s�[���Z�q
	const mist_iterator1& operator =( const mist_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}

	// �v�f�̃A�N�Z�X
	reference operator *(){ return( *data_ ); }
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	// �ړ�
	mist_iterator1& operator ++( ) // �O�u�^
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator1 operator ++( int ) // ��u�^
	{
		mist_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator1& operator --( ) // �O�u�^
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator1 operator --( int ) // ��u�^
	{
		mist_iterator1 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_iterator1& operator +=( difference_type dist )
	{
		data_ += dist * diff_pointer_;
		return( *this );
	}
	const mist_iterator1& operator -=( difference_type dist )
	{
		data_ -= dist * diff_pointer_;
		return( *this );
	}

	// ��r
	bool operator ==( const mist_iterator1 &ite ) const { return( data_ == ite.data_ ); }
	bool operator !=( const mist_iterator1 &ite ) const { return( data_ != ite.data_ ); }
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
};


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite1, const mist_iterator1< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}



// mist�R���e�i�ŗ��p����2��������p�����_���A�N�Z�X�C�e���[�^
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;
	typedef Pointer pointer;
	typedef size_t size_type;
	typedef Reference reference;
	typedef Distance difference_type;

private:
	pointer sdata_;
	pointer data_;
	size_type diff_pointer1_;
	size_type diff_pointer2_;
	size_type diff_boundary_;

public:
	// �R���X�g���N�^
	mist_iterator2( pointer p = NULL, size_type diff1 = 1, size_type diff2 = 1, size_type bound = 1 )
				: sdata_( p ), data_( p ), diff_pointer1_( diff1 ), diff_pointer2_( diff2 ), diff_boundary_( bound )
	{
	}

	mist_iterator2( const mist_iterator2 &ite ) : sdata_( ite.sdata_ ), data_( ite.data_ ),
				diff_pointer1_( ite.diff_pointer1_ ), diff_pointer2_( ite.diff_pointer2_ ), diff_boundary_( ite.diff_boundary_ )
	{
	}

	// �R�s�[���Z�q
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		sdata_ = ite.sdata_;
		data_ = ite.data_;
		diff_pointer1_ = ite.diff_pointer1_;
		diff_pointer2_ = ite.diff_pointer2_;
		diff_boundary_ = ite.diff_boundary_;
		return( *this );
	}

	// �v�f�̃A�N�Z�X
	reference operator *(){ return( *data_ ); }
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	// �ړ�
	mist_iterator2& operator ++( ) // �O�u�^
	{
		*this += 1;
		return( *this );
	}
	const mist_iterator2 operator ++( int ) // ��u�^
	{
		mist_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}
	mist_iterator2& operator --( ) // �O�u�^
	{
		*this -= 1;
		return( *this );
	}
	const mist_iterator2 operator --( int ) // ��u�^
	{
		mist_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	const mist_iterator2& operator +=( difference_type dist )
	{
		data_ += dist * diff_pointer1_;

		if( data_ > sdata_ )
		{
			if( data_ >= diff_boundary_ + sdata_ )
			{
				sdata_ += diff_boundary_ + diff_pointer2_;
				data_  += diff_pointer2_;
			}
		}
		else if( data_ < sdata_ )
		{
			sdata_ -= diff_boundary_ + diff_pointer2_;
			data_  -= diff_pointer2_;
		}

		return( *this );
	}
	const mist_iterator2& operator -=( difference_type dist )
	{
		data_ -= dist * diff_pointer1_;

		if( data_ > sdata_ )
		{
			if( data_ >= diff_boundary_ + sdata_ )
			{
				sdata_ += diff_boundary_ + diff_pointer2_;
				data_  += diff_pointer2_;
			}
		}
		else if( data_ < sdata_ )
		{
			sdata_ -= diff_boundary_ + diff_pointer2_;
			data_  -= diff_pointer2_;
		}

		return( *this );
	}

	// ��r
	bool operator ==( const mist_iterator2 &ite ) const { return( data_ == ite.data_ ); }
	bool operator !=( const mist_iterator2 &ite ) const { return( data_ != ite.data_ ); }
	bool operator < ( const mist_iterator2 &ite ) const { return( data_ <  ite.data_ ); }
	bool operator <=( const mist_iterator2 &ite ) const { return( data_ <= ite.data_ ); }
	bool operator > ( const mist_iterator2 &ite ) const { return( data_ >  ite.data_ ); }
	bool operator >=( const mist_iterator2 &ite ) const { return( data_ >= ite.data_ ); }
};



template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite1 ) += ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( Distance dist, const mist_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}


template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite1, const mist_iterator2< T, Distance, Pointer, Reference > ite2 )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite1 ) -= ite2 );
}

template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, Distance dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}



// mist�ŗp����
template< class _Ite >
class mist_reverse_iterator :
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type
	>
#else
	public std::iterator<
		typename _Ite::iterator_category,
		typename _Ite::value_type,
		typename _Ite::difference_type,
		typename _Ite::pointer,
		typename _Ite::reference
	>
#endif
{
public:
 	typedef typename _Ite::difference_type difference_type;
	typedef typename _Ite::pointer pointer;
	typedef typename _Ite::reference reference;

protected:
	_Ite current_iterator_;

public:
	// �R���X�g���N�^
	mist_reverse_iterator( ){ }
	mist_reverse_iterator( const _Ite &ite ) : current_iterator_( ite ){ }
	mist_reverse_iterator( const mist_reverse_iterator &ite ) : current_iterator_( ite.current_iterator_ ){ }

	// �R�s�[���Z�q
	const mist_reverse_iterator& operator =( const _Ite &ite )
	{
		current_iterator_ = ite;
		return( *this );
	}
	const mist_reverse_iterator& operator =( const mist_reverse_iterator &ite )
	{
		current_iterator_ = ite.current_iterator_;
		return( *this );
	}

	// �v�f�̃A�N�Z�X
	reference operator *()
	{
		_Ite _tmp = current_iterator_;
		return( *( --_tmp ) );
	}
	reference operator []( difference_type dist ){ return( *( *this + dist ) ); }

	// �ړ�
	mist_reverse_iterator& operator ++( ) // �O�u�^
	{
		--current_iterator_;
		return( *this );
	}
	const mist_reverse_iterator operator ++( int ) // ��u�^
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_--;
		return( old_val );
	}
	mist_reverse_iterator& operator --( ) // �O�u�^
	{
		++current_iterator_;
		return( *this );
	}
	const mist_reverse_iterator operator --( int ) // ��u�^
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_++;
		return( old_val );
	}

	const mist_reverse_iterator& operator +=( difference_type dist )
	{
		current_iterator_ -= dist;
		return( *this );
	}
	const mist_reverse_iterator& operator -=( difference_type dist )
	{
		current_iterator_ += dist;
		return( *this );
	}


	// ��r
	bool operator ==( const mist_reverse_iterator &ite ) const { return( current_iterator_ == ite.current_iterator_ ); }
	bool operator !=( const mist_reverse_iterator &ite ) const { return( current_iterator_ != ite.current_iterator_ ); }
	bool operator < ( const mist_reverse_iterator &ite ) const { return( current_iterator_ <  ite.current_iterator_ ); }
	bool operator <=( const mist_reverse_iterator &ite ) const { return( current_iterator_ <= ite.current_iterator_ ); }
	bool operator > ( const mist_reverse_iterator &ite ) const { return( current_iterator_ >  ite.current_iterator_ ); }
	bool operator >=( const mist_reverse_iterator &ite ) const { return( current_iterator_ >= ite.current_iterator_ ); }
};


template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
{
	return( mist_reverse_iterator< _Ite >( ite1 ) += ite2 );
}

template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}

template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator +( typename _Ite::difference_type dist, const mist_reverse_iterator< _Ite > &ite )
{
	return( mist_reverse_iterator< _Ite >( ite ) += dist );
}


template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator -( const mist_reverse_iterator< _Ite > &ite1, const mist_reverse_iterator< _Ite > ite2 )
{
	return( mist_reverse_iterator< _Ite >( ite1 ) -= ite2 );
}

template< class _Ite >
inline const mist_reverse_iterator< _Ite > operator -( const mist_reverse_iterator< _Ite > &ite, typename _Ite::difference_type dist )
{
	return( mist_reverse_iterator< _Ite >( ite ) -= dist );
}

// mist���O��Ԃ̏I���
_MIST_END


#endif
