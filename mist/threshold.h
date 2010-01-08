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

/// @file mist/threshold.h
//!
//! @brief �C�ӂ̉摜���玩���I�ɂ������l�����肷�郉�C�u����
//!
#ifndef __INCLUDE_MIST_THRESHOLD__
#define __INCLUDE_MIST_THRESHOLD__


#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "limits.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @addtogroup threshold_group �������l
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/threshold.h>
//! @endcode
//!

//!  @{


/// @brief P�^�C���@�Ɋ�Â��������l����
namespace ptile
{
	/// @brief p-tile�@�ɂ�邵�����l����
	//!
	//! ratio �� 0 �` 100���͈̔͂Ŏw�肷��
	//!
	//! @param[in] in    �c ���̓f�[�^
	//! @param[in] ratio �c �Z�x�l�̗ݐϕ��z���S�̂ɐ�߂銄��
	//!
	//! @return p-tile�@�ɂ���Č��肳�ꂽ�������l
	//!
	template < class Array >
	typename Array::value_type threshold( const Array &in, double ratio )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;

		if( in.empty( ) )
		{
			return( value_type( 0 ) );
		}

		value_type min = in[ 0 ];
		value_type max = in[ 0 ];

		size_type i;

		for( i = 1 ; i < in.size( ) ; i++ )
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

		size_type level = static_cast< size_type >( max - min ) + 1;
		size_type *hist = new size_type[ level ];
		size_type N = in.size( );

		for( i = 0 ; i < level ; i++ )
		{
			hist[ i ] = 0;
		}
		for( i = 0 ; i < N ; i++ )
		{
			hist[ static_cast< size_type >( in[ i ] - min ) ]++;
		}

		size_type pix_num = 0;
		size_type th = 0;

		while( ( pix_num * 100.0 ) / static_cast< double >( N ) < ratio )
		{
			pix_num += hist[ th ];
			th++;
		}

		delete [] hist;

		return( static_cast< value_type >( th + min ) );
	}

	/// @brief p-tile�@�ɂ�邵�����l����
	//!
	//! ratio �� 0 �` 100���͈̔͂Ŏw�肷��
	//!
	//! @param[in] in    �c ���̓f�[�^
	//! @param[in] mask  �c �}�X�N�f�[�^
	//! @param[in] ratio �c �Z�x�l�̗ݐϕ��z���S�̂ɐ�߂銄��
	//!
	//! @return p-tile�@�ɂ���Č��肳�ꂽ�������l
	//!
	template < class Array, class Mask >
	typename Array::value_type threshold( const Array &in, const Mask &mask, double ratio )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;

		if( in.empty( ) )
		{
			return( value_type( 0 ) );
		}

		value_type min = type_limits< value_type >::maximum( );
		value_type max = type_limits< value_type >::minimum( );

		size_type i, count = 0;

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			if( mask[ i ] != 0 )
			{
				count++;
				if( min > in[ i ] )
				{
					min = in[ i ];
				}
				if( max < in[ i ] )
				{
					max = in[ i ];
				}
			}
		}

		if( count == 0 )
		{
			return( value_type( 0 ) );
		}

		size_type level = static_cast< size_type >( max - min ) + 1;
		size_type *hist = new size_type[ level ];

		for( i = 0 ; i < level ; i++ )
		{
			hist[ i ] = 0;
		}
		for( i = 0 ; i < in.size( ) ; i++ )
		{
			if( mask[ i ] != 0 )
			{
				hist[ static_cast< size_type >( in[ i ] - min ) ]++;
			}
		}

		size_type pix_num = 0;
		size_type th = 0;

		while( ( pix_num * 100.0 ) / static_cast< double >( count ) < ratio )
		{
			pix_num += hist[ th ];
			th++;
		}

		delete [] hist;

		return( static_cast< value_type >( th + min ) );
	}
}



/// @brief ���ʕ��͖@�Ɋ�Â��������l����
namespace discriminant_analysis
{
	/// @brief ���ʕ��͖@�ɂ�邵�����l����
	//!
	//! @param[in] in �c ���̓f�[�^
	//!
	//! @return ���ʕ��͖@�ɂ���Č��肵���������l
	//!
	template < class T, class Allocator >
	typename array< T, Allocator >::value_type threshold( const array< T, Allocator > &in )
	{
		typedef typename array< T, Allocator >::size_type  size_type;
		typedef typename array< T, Allocator >::value_type value_type;

		if( in.empty( ) )
		{
			return( value_type( 0 ) );
		}

		value_type min = in[ 0 ];
		value_type max = in[ 0 ];

		size_type i, k;

		for( i = 1 ; i < in.size( ) ; i++ )
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

		size_type level = static_cast< size_type >( max - min ) + 1;
		size_type max_k;
		double *p   = new double[ level ];
		double myu, omg;
		double myuT, sig, max_sig = 0.0;

		size_type N = in.size( );
		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] = 0.0;
		}
		for( k = 0 ; k < N ; k++ )
		{
			p[ static_cast< size_type >( in[ k ] - min ) ]++;
		}
		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] /= static_cast< double >( N );
		}

		myuT = 0.0;
		for( k = 0 ; k < level ; k++ )
		{
			myuT += k * p[ k ];
		}

		myu = 0.0;
		omg = p[ 0 ];
		max_k = 0;
		max_sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
		for( k = 1 ; k < level ; k++ )
		{
			omg = omg + p[ k ];
			myu = myu + k * p[ k ];
			sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
			if( sig > max_sig )
			{
				max_sig = sig;
				max_k = k;
			}
		}

		delete [] p;

		return( static_cast< value_type >( max_k + min ) );
	}

	/// @brief �}�X�N�����̈���݂̂Ŕ��ʕ��͖@�ɂ�邵�����l������s��
	//!
	//! @param[in] in   �c ���̓f�[�^
	//! @param[in] mask �c �}�X�N�f�[�^
	//!
	//! @return ���ʕ��͖@�ɂ���Č��肵���������l
	//!
	template < class T1, class Allocator1, class T2, class Allocator2 >
	typename array< T1, Allocator1 >::value_type threshold( const array< T1, Allocator1 > &in, const array< T2, Allocator2 > &mask )
	{
		typedef typename array< T1, Allocator1 >::size_type  size_type;
		typedef typename array< T1, Allocator1 >::value_type value_type;

		if( in.empty( ) || in.size( ) != mask.size( ) )
		{
			return( value_type( 0 ) );
		}

		value_type min = type_limits< value_type >::maximum( );
		value_type max = type_limits< value_type >::minimum( );

		size_type i, k, count = 0;

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			if( mask[ i ] != 0 )
			{
				count++;
				if( min > in[ i ] )
				{
					min = in[ i ];
				}
				if( max < in[ i ] )
				{
					max = in[ i ];
				}
			}
		}

		if( count == 0 )
		{
			return( value_type( 0 ) );
		}

		size_type level = static_cast< size_type >( max - min ) + 1;
		size_type max_k;
		double *p   = new double[ level ];
		double myu, omg;
		double myuT, sig, max_sig = 0.0;

		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] = 0.0;
		}
		for( k = 0 ; k < in.size( ) ; k++ )
		{
			if( mask[ k ] != 0 )
			{
				p[ static_cast< size_type >( in[ k ] - min ) ]++;
			}
		}
		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] /= static_cast< double >( count );
		}

		myuT = 0.0;
		for( k = 0 ; k < level ; k++ )
		{
			myuT += k * p[ k ];
		}

		myu = 0.0;
		omg = p[ 0 ];
		max_k = 0;
		max_sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
		for( k = 1 ; k < level ; k++ )
		{
			omg = omg + p[ k ];
			myu = myu + k * p[ k ];
			sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
			if( sig > max_sig )
			{
				max_sig = sig;
				max_k = k;
			}
		}

		delete [] p;

		return( static_cast< value_type >( max_k + min ) );
	}

	/// @brief ���ʕ��͖@�ɂ�邵�����l����
	//!
	//! �w�肵���͈͓��ł̂�臒l�����肷��
	//!
	//! @param[in] in �c ���̓f�[�^
	//! @param[in] x  �c �������l���v�Z���鍶���X���W
	//! @param[in] y  �c �������l���v�Z���鍶���Y���W
	//! @param[in] w  �c �������l���v�Z����͈͂̕�
	//! @param[in] h  �c �������l���v�Z����͈͂̍���
	//!
	//! @return ���ʕ��͖@�ɂ���Č��肵���������l
	//!
	template < class T, class Allocator >
	typename array2< T, Allocator >::value_type threshold(  const array2< T, Allocator > &in,
															typename array2< T, Allocator >::size_type x,
															typename array2< T, Allocator >::size_type y,
															typename array2< T, Allocator >::size_type w,
															typename array2< T, Allocator >::size_type h )
	{
		typedef typename array2< T, Allocator >::size_type  size_type;
		typedef typename array2< T, Allocator >::value_type value_type;

		if( in.empty( ) )
		{
			return( value_type( 0 ) );
		}
		else if( in.width( ) <= x || in.height( ) <= y )
		{
			return( value_type( 0 ) );
		}

		if( in.width( ) < x + w )
		{
			w = in.width( ) - x;
		}
		if( in.height( ) < y + h )
		{
			h = in.height( ) - y;
		}

		value_type min = in( x, y );
		value_type max = min;

		size_type i, j, k;

		for( j = y ; j < y + h ; j++ )
		{
			for( i = x ; i < x + w ; i++ )
			{
				if( min > in( i, j ) )
				{
					min = in( i, j );
				}
				else if( max < in( i, j ) )
				{
					max = in( i, j );
				}
			}
		}

		size_type level = static_cast< size_type >( max - min ) + 1;
		size_type max_k;
		double *p   = new double[ level ];
		double myu, omg;
		double myuT, sig, max_sig = 0.0;

		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] = 0.0;
		}

		for( j = y ; j < y + h ; j++ )
		{
			for( i = x ; i < x + w ; i++ )
			{
				p[ static_cast< size_type >( in( i, j ) - min ) ]++;
			}
		}

		size_type N = w * h;
		for( k = 0 ; k < level ; k++ )
		{
			p[ k ] /= static_cast< double >( N );
		}

		myuT = 0.0;
		for( k = 0 ; k < level ; k++ )
		{
			myuT += k * p[ k ];
		}

		myu = 0.0;
		omg = p[ 0 ];
		max_k = 0;
		max_sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
		for( k = 1 ; k < level ; k++ )
		{
			omg = omg + p[ k ];
			myu = myu + k * p[ k ];
			sig = ( myuT * omg - myu ) * ( myuT * omg - myu ) / ( omg * ( 1.0 - omg ) );
			if( sig > max_sig )
			{
				max_sig = sig;
				max_k = k;
			}
		}

		delete [] p;

		return( static_cast< value_type >( max_k + min ) );
	}
}

/// @}
//  �������l�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_THRESHOLD__
