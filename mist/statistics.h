/// @file mist/statistics.h
//!
//! @brief �f�[�^�̓��v����
//!
#ifndef __INCLUDE_MIST_STATISTICS__
#define __INCLUDE_MIST_STATISTICS__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __utility__
{
	template < bool b >
	struct ____value_type____
	{
		typedef double value_type;
	};

	template < >
	struct ____value_type____< true >
	{
		typedef rgb< double > value_type;
	};

	template < class T >
	struct __value_type__
	{
		typedef typename ____value_type____< is_color< T >::value >::value_type value_type;
	};
}

//! @defgroup statistics_group ���v����
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/statistics.h>
//! @endcode
//!
//!  @{



/// �f�[�^�̕��ϒl���v�Z����
//!
//! @param[in] a �c ���ϒl���v�Z����f�[�^�z��
//! 
//! @return �f�[�^�̕��ϒl
//! 
template < class T, class Allocator >
inline typename __utility__::__value_type__< T >::value_type average( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	typedef typename array< T, Allocator >::value_type value_type;
	typedef typename __utility__::__value_type__< T >::value_type value_type;

	if( a.empty( ) )
	{
		return( value_type( ) );
	}
	else
	{
		value_type v = value_type( );
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			v += a[ i ];
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// �f�[�^�̕��U���v�Z����
//!
//! @param[in] a   �c ���U���v�Z����f�[�^�z��
//! @param[in] ave �c �f�[�^�z��̕��ϒl
//! 
//! @return �f�[�^�̕��U
//! 
template < class T, class Allocator >
inline typename __utility__::__value_type__< T >::value_type deviation( const array< T, Allocator > &a, double ave )
{
	typedef typename array< T, Allocator >::size_type size_type;
	typedef typename array< T, Allocator >::value_type value_type;
	typedef typename __utility__::__value_type__< T >::value_type value_type;

	if( a.empty( ) )
	{
		return( value_type( ) );
	}
	else
	{
		value_type v = value_type( );
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			value_type x = a[ i ] - ave;
			v += x * x;
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// �f�[�^�̕��U���v�Z����
//!
//! @param[in] a �c ���U���v�Z����f�[�^�z��
//! 
//! @return �f�[�^�̕��U
//! 
template < class T, class Allocator >
inline typename __utility__::__value_type__< T >::value_type deviation( const array< T, Allocator > &a )
{
	return( deviation( a, average( a ) ) );
}




/// �f�[�^�͈̔͂��w�肵�ăq�X�g�O�������쐬����
//!
//! @param[in] in  �c �q�X�g�O�������쐬����f�[�^�z��
//! @param[in] min �c �q�X�g�O�������쐬����f�[�^�̍ŏ��l
//! @param[in] max �c �q�X�g�O�������쐬����f�[�^�̍ő�l
//! @param[in] bin �c �q�X�g�O�������쐬����r����
//! 
//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool histogram( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out,
				typename array< T1, Allocator1 >::value_type min,
				typename array< T1, Allocator1 >::value_type max,
				typename array< T1, Allocator1 >::value_type bin = array< T1, Allocator1 >::value_type( 1 )
			  )
{
	if( is_same_object( in, out ) || in.empty( ) )
	{
		return( false );
	}

	typedef typename array< T1, Allocator1 >::size_type size_type;
	typedef typename array< T1, Allocator1 >::difference_type difference_type;
	typedef typename array< T1, Allocator1 >::value_type value_type;

	if( bin <= 0 )
	{
		return( false );
	}

	if( min > max )
	{
		value_type tmp = min;
		min = max;
		max = tmp;
	}

	difference_type num = static_cast< size_type >( ( max - min + 1 ) / bin );
	out.resize( num );
	out.fill( );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		difference_type index = static_cast< difference_type >( ( in[ i ] - min ) / bin );
		if( index >= 0 && index < num )
		{
			out[ index ]++;
		}
	}

	return( true );
}


/// �f�[�^�S�̂���q�X�g�O�������쐬����
//!
//! @param[in] in  �c �q�X�g�O�������쐬����f�[�^�z��
//! @param[in] bin �c �q�X�g�O�������쐬����r����
//! 
//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
bool histogram( const array< T1, Allocator1 > &in, array< T2, Allocator2 > &out, typename array< T1, Allocator1 >::value_type bin = array< T1, Allocator1 >::value_type( 1 ) )
{
	typedef typename array< T1, Allocator1 >::size_type size_type;
	typedef typename array< T1, Allocator1 >::difference_type difference_type;
	typedef typename array< T1, Allocator1 >::value_type value_type;

	if( in.empty( ) )
	{
		return( false );
	}

	value_type min = in[ 0 ];
	value_type max = in[ 0 ];
	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		if( min > in[ i ] )
		{
			min = in[ i ];
		}
		else if( max < in[ i ] )
		{
			max = in[ i ];
		}
	}

	return( histogram( in, out, min, max, bin ) );
}



/// @}
//  ���v�����̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
