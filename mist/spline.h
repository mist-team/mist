/// @file mist/spline.h
//!
//! @brief 3���X�v���C���Ȑ���`�悷�邽�߂̃��C�u����
//!

#ifndef __INCLUDE_SPLINE__
#define __INCLUDE_SPLINE__

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#include <vector>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


// ��ʂ̃f�[�^�^�p�̓_���Z
template < class T >
inline const T add( const T &v1, const T &v2 ){ return( v1 + v2 ); }

template < class T >
inline const T sub( const T &v1, const T &v2 ){ return( v1 - v2 ); }

template < class T >
inline const T mul( const T &v1, const T &v2 ){ return( v1 * v2 ); }

template < class T >
inline const T div( const T &v1, const T &v2 ){ return( v1 / v2 ); }



// vector3 �p�̓_���Z
template < class T >
inline const vector3< T > add( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ) ); }

template < class T >
inline const vector3< T > add( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x + val, v1.y + val, v1.z + val ) ); }

template < class T >
inline const vector3< T > sub( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ) ); }

template < class T >
inline const vector3< T > sub( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x - val, v1.y - val, v1.z - val ) ); }

template < class T >
inline const vector3< T > mul( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ) ); }

template < class T >
inline const vector3< T > mul( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x * val, v1.y * val, v1.z * val ) ); }

template < class T >
inline const vector3< T > div( const vector3< T > &v1, const vector3< T > &v2 ){ return( vector3< T >( v1.x / v2.x, v1.y / v2.y, v1.z / v2.z ) ); }

template < class T >
inline const vector3< T > div( const vector3< T > &v1, const typename vector3< T >::value_type &val ){ return( vector3< T >( v1.x / val, v1.y / val, v1.z / val ) ); }


// vector2 �p�̓_���Z
template < class T >
inline const vector2< T > add( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x + v2.x, v1.y + v2.y ) ); }

template < class T >
inline const vector2< T > add( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x + val, v1.y + val ) ); }

template < class T >
inline const vector2< T > sub( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x - v2.x, v1.y - v2.y ) ); }

template < class T >
inline const vector2< T > sub( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x - val, v1.y - val ) ); }

template < class T >
inline const vector2< T > mul( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x * v2.x, v1.y * v2.y ) ); }

template < class T >
inline const vector2< T > mul( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x * val, v1.y * val ) ); }

template < class T >
inline const vector2< T > div( const vector2< T > &v1, const vector2< T > &v2 ){ return( vector2< T >( v1.x / v2.x, v1.y / v2.y ) ); }

template < class T >
inline const vector2< T > div( const vector2< T > &v1, const typename vector2< T >::value_type &val ){ return( vector2< T >( v1.x / val, v1.y / val ) ); }



//! @addtogroup free_form_group ���R�Ȑ��E�Ȗ�
//!  @{



//! @addtogroup spline_group 3���X�v���C���Ȑ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/spline.h>
//! @endcode
//!
//!  @{



/// @brief 3���X�v���C���Ȑ����������߂̃N���X
//!
//! 3���X�v���C���Ȑ���`��\�D�ό��̐���_�ɑΉ����C�J�Ȑ��ƕȐ���3���X�v���C���Ȑ���`��
//! 
//! @param T  �c �e����_�E��ԓ_��\���f�[�^�\�����w��idouble �� vector3< double > �Ȃǁj
//!
//! @code 3���X�v���C���Ȑ��̍쐬��
//! // 3���ŕϐ��̌^�� double ��3���X�v���C�����쐬����
//! mist::spline< double, 3 > b;
//! 
//! // ����_��ǉ�
//! b.push_back( 2.0 );
//! b.push_back( 3.0 );
//! ...
//! 
//! // �Ȑ��̎�ނ��ȉ��̕��@�̂����ꂩ��p���Đݒ�
//! // �E�Ȑ��̐ݒ�
//! b.mode( mist::spline< double >::CLOSED );
//! // �E�ŏ��ƍŌ�̐���_��ʂ�ꍇ�̐ݒ�
//! b.mode( mist::spline< double >::OPEN );
//! 
//! // ��ԓ_���v�Z�i��Ԃ͂O�`�P�j
//! double p1 = b( 0.0 );
//! double p2 = b( 0.4 );
//! double p3 = b( 0.7 );
//! ...
//! 
//! @endcode
//!
template < class T, class Allocator = std::allocator< T > >
class spline : public std::vector< T, Allocator >
{
private:
	typedef std::vector< T, Allocator > base;
	typedef typename base::allocator_type allocator_type;		///< @brief STL�R���e�i�����p����A���P�[�^�^
	typedef typename base::reference reference;					///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̎Q��
	typedef typename base::const_reference const_reference;		///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q��
	typedef typename base::value_type value_type;				///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^
	typedef typename base::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename base::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename base::pointer pointer;						///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^
	typedef typename base::const_pointer const_pointer;			///< @brief STL�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^

	typedef std::vector< value_type > point_list;

public:
	/// @brief 3���X�v���C����`�悷��ۂ̃p�����[�^
	enum SplineMode
	{
		CLOSED,				///< @brief �Ȑ��̐ݒ�
		OPEN,				///< @brief �J�Ȑ��̐ݒ�
	};

protected:
	SplineMode mode_;			///< @brief �X�v���C���Ȑ��̐ݒ�
	point_list p1_;				///< @brief �e����_��ł̔��W��

	/// @brief �Ȑ��̊e����_��ł̔��W�����v�Z����
	void closed_spline( )
	{
		base p = *this;

		p.push_back( p[ 0 ] );

		difference_type n, num = p.size( );

		value_type *a = new value_type[ num ];
		value_type *b = new value_type[ num ];
		value_type *c = new value_type[ num ];

		p1_.clear( );

		// �J�n�_�ƏI�_�ɂ���
		a[ 0 ]   = value_type( 1 );
		b[ 0 ]   = value_type( 4 );
		c[ 0 ]   = value_type( 1 );
		p1_.push_back( mul( sub( p[ 1 ], p[ num - 2 ] ), 3 ) );

		// �J�n�_�ƏI�_�ȊO�̐���_�Ɋւ��ď�����
		for( n = 1 ; n < num - 1 ; n++ )
		{
			a[ n ]   = value_type( 1 );
			b[ n ]   = value_type( 4 );
			c[ n ]   = value_type( 1 );
			p1_.push_back( mul( sub( p[ n + 1 ], p[ n - 1 ] ), 3 ) );
		}

		a[ num - 1 ]   = value_type( 1 );
		b[ num - 1 ]   = value_type( 4 );
		c[ num - 1 ]   = value_type( 1 );
		p1_.push_back( p1_[ 0 ] );

		// �ꎟ���W���̌v�Z
		for( n = 1; n < num - 1; n++ )		// �J�n�_�ƏI�_�ȊO�Ɋւ���
		{
			a[ n ]   = div( a[ n ], b[ n - 1 ] );
			b[ n ]   = sub( b[ n ], mul( a[ n ], c[ n - 1 ] ) );
			p1_[ n ] = sub( p1_[ n ],  mul( a[ n ],  p1_[ n - 1 ] ) );
			a[ n ]   = mul( mul( a[ n - 1 ],  a[ n ] ), -1 );
		}

		a[ num - 2 ] = mul( div( c[ num - 2 ], add( a[ num - 2 ], b[ num - 2 ] ) ), -1 );
		b[ num - 2 ] = mul( mul( a[ num - 2 ], div( p1_[ num - 2 ], c[ num - 2 ] ) ), -1 );

		value_type tmp( a[ num - 3 ] );
		a[ num - 3 ] = mul( div( mul( add( a[ num - 3 ], c[ num - 3 ] ), a[ num - 2 ] ), b[ num - 3 ] ), -1 );
		b[ num - 3 ] = div( sub( p1_[ num - 3 ], mul( add( tmp, c[ num - 2 ] ), b[ num - 2 ] ) ), b[ num - 3 ] );

		for( n = num - 4 ; n >= 0 ; n-- )
		{
			value_type tmp( a[ n ] );
			a[ n ] = div( mul( add( mul( c[ n ], a[ n + 1 ] ), mul( a[ n ], a[ num - 2 ] ) ), -1 ), b[ n ] );
			b[ n ] = div( sub( sub( p1_[ n ], mul( c[ n ], b[ n + 1 ] ) ), mul( tmp, b[ num - 2 ] ) ), b[ n ] );
		}

		p1_[ 0 ] = div( b[ 0 ], add( a[ 0 ], 1 ) );

		for( n = 1 ; n < num - 1 ; n++ )
		{
			p1_[ n ] = add( mul( a[ n ], p1_[ 0 ] ), b[ n ] );
		}

		p1_.pop_back( );

		delete [] a;
		delete [] b;
		delete [] c;
	}

	/// @brief �J�Ȑ��̊e����_��ł�1�����W�����v�Z����
	void open_spline( )
	{
		const base &p = *this;

		difference_type n, num = p.size( );

		value_type *a = new value_type[ num ];
		value_type *b = new value_type[ num ];
		value_type *c = new value_type[ num ];

		p1_.clear( );

		// �J�n�_�ƏI�_�ɂ���
		a[ 0 ]   = value_type( 0 );
		b[ 0 ]   = value_type( 2 );
		c[ 0 ]   = value_type( 1 );
		p1_.push_back( mul( sub( p[ 1 ], p[ 0 ] ), 3 ) );

		// �J�n�_�ƏI�_�ȊO�̐���_�Ɋւ��ď�����
		for( n = 1 ; n < num - 1 ; n++ )
		{
			a[ n ]   = value_type( 1 );
			b[ n ]   = value_type( 4 );
			c[ n ]   = value_type( 1 );
			p1_.push_back( mul( sub( p[ n + 1 ], p[ n - 1 ] ), 3 ) );
		}

		a[ num - 1 ]   = value_type( 1 );
		b[ num - 1 ]   = value_type( 2 );
		c[ num - 1 ]   = value_type( 0 );
		p1_.push_back( mul( sub( p[ num-1 ], p[ num-2 ] ), 3 ) );

		// �ꎟ���W���̌v�Z
		for( n = 1; n < num; n++ )
		{
			a[ n ]   = div( a[ n ], b[ n - 1 ] );
			b[ n ]   = sub( b[ n ], mul( a[ n ], c[ n - 1 ] ) );
			p1_[ n ] = sub( p1_[ n ],  mul( a[ n ],  p1_[ n - 1 ] ) );
		}

		p1_[ num-1 ] = div( p1_[ num-1 ], b[ num-1 ] );

		for( n = num-2; n >= 0; n-- )
		{
			p1_[ n ] = div( sub( p1_[n], mul( c[n], p1_[ n+1 ] ) ), b[n] );
		}

		delete [] a;
		delete [] b;
		delete [] c;
	}

public:
	/// @brief �X�v���C����`�悷��O�ɕK���Ăяo��
	void construct_spline( )
	{
		switch( mode_ )
		{
		case CLOSED:
            closed_spline( );
			break;

		case OPEN:
		default:
			open_spline( );
			break;
		}
	}

	/// @brief �w�肳�ꂽ�ʒu�i��ԂO�`�P�j�ɑΉ�����C3���X�v���C����Ԍ��ʂ�Ԃ�
	//!
	//! @param[in] t  �c �S�̂̋Ȑ�����ԂO�`�P�Ƃ������ɁC��Ԃ����_�̈ʒu
	//!
	//! @return �w�肳�ꂽ�_�̈ʒu�ɑΉ�����3���X�v���C���Ȑ��̕�Ԍ���
	//!
	value_type operator( )( double t )
	{
		// �Ȑ����\�z����̂ɕK�v�ȓ_�������݂��Ȃ��ꍇ
		if( size( ) < 3 || p1_.size( ) < size( ) )
		{
			return( empty( ) ? value_type( ) : at( 0 ) );
		}

		if( t < 0.0 )
		{
			t = 0.0;
		}
		else if( t > 1.0 )
		{
			t = 1.0;
		}

		size_type num;

		switch( mode_ )
		{
		case CLOSED:
			num = p1_.size( );
			break;

		case OPEN:
		default:
			num = p1_.size( ) - 1;
			break;
		}

		size_type n = static_cast< size_type >( t * static_cast< double >( num ) );
		double step = 1.0 / static_cast< double >( num );

		value_type a0, a1, a2, a3;
		const base &p = *this;

		switch( mode_ )
		{
		case CLOSED:
			if( n >= num - 1 )
			{
				a3 = add( mul( sub( p[ num - 1 ], p[ 0 ] ), 2 ), add( p1_[ num - 1 ], p1_[ 0 ] ) );
				a2 = sub( add( mul( sub( p[ num - 1 ], p[ 0 ] ), -3 ), mul( p1_[ num - 1 ], -2 ) ), p1_[ 0 ] );
				a1 = p1_[ num - 1 ];
				a0 = p[ num - 1 ];
				if( n == num )
				{
					n--;
				}
			}
			else
			{
				a3 = add( mul( sub( p[ n ], p[ n + 1 ] ), 2 ), add( p1_[ n ], p1_[ n + 1 ] ) );
				a2 = sub( add( mul( sub( p[ n ], p[ n + 1 ] ), -3 ), mul( p1_[ n ], -2 ) ), p1_[ n + 1 ] );
				a1 = p1_[ n ];
				a0 = p[ n ];
			}
			break;

		case OPEN:
		default:
			if( n == num )
			{
				return( p[ n ] );
			}
			else
			{
				a3 = add( mul( sub( p[ n ], p[ n + 1 ] ), 2 ), add( p1_[ n ], p1_[ n + 1 ] ) );
				a2 = sub( add( mul( sub( p[ n ], p[ n + 1 ] ), -3 ), mul( p1_[ n ], -2 ) ), p1_[ n + 1 ] );
				a1 = p1_[ n ];
				a0 = p[ n ];
			}
			break;
		}


		double s1 = t / step - static_cast< double >( n );
		double s2 = s1 * s1;
		double s3 = s2 * s1;

		return( add( add( add( mul( a3, s3 ), mul( a2, s2 ) ), mul( a1, s1 ) ), a0 ) );
	}


	/// @brief �X�v���C���Ȑ��̎�ނ��擾
	SplineMode mode( ) const { return( mode_ ); }

	/// @brief �X�v���C���Ȑ��̎�ނ�ݒ�
	SplineMode mode( SplineMode m ){ return( mode_ = m ); }


	/// @brief ������Z�q
	const spline &operator =( const spline &b )
	{
		if( this != &b )
		{
			base::operator =( b );
			p1_   = b.p1_;
			mode_ = b.mode_;
		}
		return( *this );
	}

	/// @brief �R�s�[�R���X�g���N�^
	spline( const spline &b ) : base( b ), mode_( b.mode_ ), p1_( b.p1_ ){ }

	/// @brief �f�t�H���g�̃R���X�g���N�^
	//!
	//! �f�t�H���g�l���u�J�Ȑ��v�ɐݒ肷��
	//!
	spline( ) : mode_( OPEN ){ }
};

/// @}
//  3���X�v���C���O���[�v�̏I���


/// @}
//  ���R�Ȑ��E�ȖʃO���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_SPLINE__
