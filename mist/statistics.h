/// @file mist/statistics.h
//!
//! @brief �f�[�^�̓��v����
//!
#ifndef __INCLUDE_MIST_STATISTICS__
#define __INCLUDE_MIST_STATISTICS__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// �f�[�^�̕��ϒl���v�Z����
//!
//! @param[in] a �c ���ϒl���v�Z����f�[�^�z��
//! 
template < class T, class Allocator >
inline double average( const array< T, Allocator > &a )
{
	typedef typename array< T, Allocator >::size_type size_type;
	if( a.empty( ) )
	{
		return( 0.0 );
	}
	else
	{
		double v = 0.0;
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			v += static_cast< double >( a[ i ] );
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// �f�[�^�̕W���΍����v�Z����
//!
//! @param[in] a   �c �W���΍����v�Z����f�[�^�z��
//! @param[in] ave �c �f�[�^�z��̕��ϒl
//! 
template < class T, class Allocator >
inline double standard_deviation( const array< T, Allocator > &a, double ave )
{
	typedef typename array< T, Allocator >::size_type size_type;
	if( a.empty( ) )
	{
		return( 0.0 );
	}
	else
	{
		double v = 0.0;
		for( size_type i = 0 ; i < a.size( ) ; i++ )
		{
			double x = static_cast< double >( a[ i ] ) - ave;
			v += x * x;
		}
		return( v / static_cast< double >( a.size( ) ) );
	}
}


/// �f�[�^�̕W���΍����v�Z����
//!
//! @param[in] a �c �W���΍����v�Z����f�[�^�z��
//! 
template < class T, class Allocator >
inline double standard_deviation( const array< T, Allocator > &a )
{
	return( standard_deviation( a, average( a ) ) );
}

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
