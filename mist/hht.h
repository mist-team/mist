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

/// @file mist/hht.h
//!
//! @brief �o���I���[�h��������уq���x���g�ϊ��̂��߂̃��C�u����
//!

#include <vector>

#ifndef __INCLUDE_HHT_H__
#define __INCLUDE_HHT_H___

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif


#ifndef __INCLUDE_FFT_H__
#include "fft/fft.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

//! @addtogroup hht_group �o���I���[�h��������уq���x���g�ϊ�
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/hht.h>
//! @endcode
//!
//!  @{


template < class T >
bool emd_envelope(const array< T > &x, const array< T > &y, size_t xsize, array< T > &yout)
{
	T p, sig;

	if( x.size( ) < xsize || y.size( ) < xsize )
	{
		return ( false );
	}

	array< T > u( xsize );
	array< T > y2( xsize );

	y2[ 0 ] = u[ 0 ] = 0.0;


	for( size_t i = 1 ; i < xsize - 1 ; i++ )
	{
		sig = ( x[ i ] - x[ i - 1 ] ) / ( x[ i + 1 ] - x[ i - 1 ] );
		p = sig * y2[ i - 1 ] + 2.0;
		y2[ i ] = ( sig - 1.0 ) / p;
		u[ i ] = ( y[ i + 1 ] - y[ i ] ) / ( x[ i + 1 ] - x[ i ] ) - ( y[ i ] - y[ i - 1 ] ) / ( x[ i ] - x[ i - 1 ] );
		u[ i ] = ( 6.0 * u[ i ] / ( x[ i + 1 ] - x[ i - 1 ] ) - sig * u[ i - 1 ] ) / p;
	}

	T qn = 0.0, un = 0.0;

	y2[ xsize - 1 ] = ( un - qn * u[ xsize - 2 ] ) / ( qn * y2[ xsize - 2 ] + 1.0 );

	for( size_t i = xsize - 1 ; i > 0 ; i-- )
	{
		y2[ i - 1 ] = y2[ i - 1 ] * y2[ i ] + u[ i - 1 ];
	}

	size_t k = 0;
	T h, b, a;

	for( size_t i = 0 ; i < yout.size( ) ; i++ )
	{
		if( x[ k + 1 ] < static_cast< T >( i ) )
		{
			k++;
		}
		h = x[ k + 1 ] - x[ k ];
		a = ( x[ k + 1 ] - static_cast< T >( i ) ) / h;
		b = ( static_cast< T >( i ) - x[ k ] ) / h;
		yout[ i ] = a * y[ k ] + b * y[ k + 1 ] + ( ( a * a * a - a ) * y2[ k ] + ( b * b * b - b ) * y2[ k + 1 ] ) * ( h * h ) / 6.0;
	}

	return ( true );
}


/// @brief �q���x���g�ϊ�
//!
//! ��͐M��(���f��)�̌n����o�͂���
//!
//! @attention �o�͌n��̋����������̃q���x���g�ϊ��ƂȂ��Ă���
//!
//! @param[in]  in  �c ���͌n��
//! @param[out] out �c �o�͌n��
//! @param[in]  mirror  �c �[���Ȃ߂炩�ɂȂ�悤�ɐ܂�Ԃ����������邩�ǂ��� (�f�t�H���g��false)
//!
//! @return �ϊ��ɐ����������ǂ���
//!
template < class T >
bool hilbert( const array< T > &in, array< std::complex< T > > &out, bool mirror = false )
{

	double f_index = std::log( static_cast< double >( in.size( ) ) ) / std::log( 2.0 );
	if( mirror )
	{
		f_index += 1.0;
	}
	array< std::complex< T > > as( static_cast< size_t >( std::pow( 2.0, std::floor( f_index ) + 1.0 ) ) );

	for( size_t i = 0 ; i < in.size( ) ; i ++ )
	{
		as[ i ] = in[ i ];
	}

	for( size_t i = in.size( ) ; i < as.size( ) ; i ++ )
	{
		as[ i ] = 0.0;
	}

	// �[���Ȃ߂炩�ɂȂ�悤�ɐ܂�Ԃ��ꍇ
	if( mirror )
	{
		size_t j;

		j = in.size( ) - 3;
		while( j > 0 && ( in[ in.size( ) - 1 ] - in[ j ] ) * ( in[ in.size( ) - 1 ] - in[ j + 1 ] ) > 0 )
		{
			j --;
		}
		if( j < in.size( ) / 4 )
		{
			j = in.size( ) - 2;
		}

		for( size_t i = in.size( ) ; i < ( in.size( ) + as.size( ) ) / 2 ; i ++ )
		{
			if( j >= 0 )
			{
				as[ i ] = std::complex< T >( in[ j -- ], as[ i ].imag( ) );
			}
		}

		j = 2;
		while( j < in.size( ) - 1 && ( in[ 0 ] - in[ j ] ) * ( in [ 0 ] - in[ j - 1 ] ) > 0 )
		{
			j ++;
		}
		if( j > in.size( ) * 3 / 4 )
		{
			j = 1;
		}

		for( size_t i = as.size( ) - 1 ; i >= ( in.size( ) + as.size( ) ) / 2 ; i -- )
		{
			if( j < in.size( ) )
			{
				as[ i ] = std::complex< T >( in[ j ++ ], as[ i ].imag( ) );
			}
		}
	}

	_fft( as, as );

	// ���̎��g��������2�{��
	for( size_t i = 1 ; i < as.size( ) / 2 ; i ++ )
	{
		as[ i ] *= 2.0;
	}

	// ���̎��g��������0��
	for( size_t i = as.size( ) / 2 ; i < as.size( ) ; i ++ )
	{
		as( i ) = 0.0;
	}

	_ifft( as, as );

	out.resize( in.size( ) );

	for( size_t i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = std::complex< T >( in[ i ], as[ i ].imag( ) );
	}

	return ( true );
}

/// @brief �o���I���[�h����
//!
//! ���͌n��Ɍo���I���[�h������K�p���C�ŗL���[�h�֐����o�͂���
//!
//! @attention �o�͂̌ŗL���[�h�֐��͍����g���������珇�ɕ���
//! @attention �֋X��C�c�����Ō�̔z��v�f�Ƃ��ĕԂ�
//!
//! imf[ 0 ] -> 1st IMF
//! imf[ 1 ] -> 2nd IMF
//! imf[ 2 ] -> 3rd IMF
//!    :        :
//! imf( imf.size( ) - 1 ) -> residual
//!
//! @param[in]  in  �c ���͌n��
//! @param[out] imf �c �ŗL���[�h�֐��̔z��
//! @param[in]  sd  �c �V�t�g�����̏I����̒l (�f�t�H���g��0.3)
//!
//! @return �ŗL���[�h�֐��̐� (�c���͊܂܂Ȃ�)
//!
template < class T1, class T2 >
size_t emd( const array< T1 > &in, array< T2 > &imf, double sd = 0.3 )
{
	std::vector< T2 > imf_v;
	
	// ������
	T2 r( in.size( ) );
	std::copy( in.begin( ), in.end( ), r.begin( ) );
	size_t minmax_old = in.size( );

	while( true )
	{
		T2 h, h_new;
		size_t minmax;

		h = h_new = r;
		while( true )
		{
			size_t max_i = 0, min_i = 0;
			minmax = 0;
			array< T1 > max_x( in.size( ) );
			array< T1 > max_y( in.size( ) );
			array< T1 > min_x( in.size( ) );
			array< T1 > min_y( in.size( ) );
			max_x[ 0 ] = min_x[ 0 ] = 0.0;
			max_x[ 1 ] = min_x[ 1 ] = static_cast< T1 >( in.size( ) - 1 );

			// �ɒl�𒊏o
			for( size_t i = 1 ; i < in.size( ) - 1 ; i ++ )
			{
				if( h[ i ] > h[ i - 1 ] && h[ i ] > h[ i + 1 ] )
				{
					max_i ++;
					max_x[ max_i ] = static_cast< T1 >( i );
					max_y[ max_i ] = h[ i ];
					minmax ++;
				}
				if( h[ i ] < h[ i - 1 ] && h[ i ] < h[ i + 1 ] )
				{
					min_i ++;
					min_x[ min_i ] = static_cast< T1 >( i );
					min_y[ min_i ] = h[ i ];
					minmax ++;
				}
			}

			if( minmax < 1 )
			{
				break;
			}

			// ���[�𒲐�
			if( max_x[ 1 ] < min_x[ 1 ] )
			{
			if( min_i > 0 && min_y[ 1 ] < h[ 0 ] )
			{
				min_x[ 0 ] = max_x[ 1 ] * 2.0 - min_x[ 1 ];
				min_y[ 0 ] = min_y[ 1 ];
			}
			else
			{
				min_x[ 0 ] = 0.0;
				min_y[ 0 ] = h[ 0 ];
			}
			max_x[ 0 ] = min_x[ 0 ] * 2.0 - max_x[ 1 ];
			max_y[ 0 ] = max_y[ 1 ];
		}
		else
		{
			if( max_i > 0 && max_y[ 1 ] > h[ 0 ] )
			{
				max_x[ 0 ] = min_x[ 1 ] * 2.0 - max_x[ 1 ];
				max_y[ 0 ] = max_y[ 1 ];
			}
			else
			{
				max_x[ 0 ] = 0.0;
				max_y[ 0 ] = h[ 0 ];
			}
			min_x[ 0 ] = max_x[ 0 ] * 2.0 - min_x[ 1 ];
			min_y[ 0 ] = min_y[ 1 ];
		}
		if( max_x[ 0 ] > 0 )
		{
			max_x[ 0 ] = 0.0;
		}
		if( min_x[ 0 ] > 0 )
		{
			min_x[ 0 ] = 0.0;
		}

		// �E�[�𒲐�
		if( max_x[ max_i ] > min_x[ min_i ] )
		{
			if( min_i > 0 && min_y[ min_i ] < h[ in.size( ) - 1 ] )
			{
				min_x[ min_i + 1 ] = max_x[ max_i ] * 2.0 - min_x[ min_i ];
				min_y[ min_i + 1 ] = min_y[ min_i ];
			}
			else
			{
				min_x[ min_i + 1 ] = static_cast< T1 >( in.size( ) - 1 );
				min_y[ min_i + 1 ] = h[ in.size( ) - 1 ];
			}
			max_x[ max_i + 1 ] = min_x[ min_i + 1 ] * 2.0 - max_x[ max_i ];
			max_y[ max_i + 1 ] = max_y[ max_i ];
			}
			else
			{
				if( max_i > 0 && max_y[ max_i ] > h[ in.size( ) - 1 ] )
				{
					max_x[ max_i + 1 ] = min_x[ min_i ] * 2.0 - max_x[ max_i ];
					max_y[ max_i + 1 ] = max_y[ max_i ];
				}
				else
				{
					max_x[ max_i + 1 ] = static_cast< T1 >( in.size( ) - 1 );
					max_y[ max_i + 1 ] = h[ in.size( ) - 1 ];
				}
				min_x[ min_i + 1 ] = max_x[ max_i + 1 ] * 2.0 - min_x[ min_i ];
				min_y[ min_i + 1 ] = min_y[ min_i ];
			}
			if( max_x[ max_i + 1 ] < static_cast< T1 >( in.size( ) - 1 ) )
			{
				max_x[ max_i + 1 ] = static_cast< T1 >( in.size( ) - 1 );
			}
			if( min_x[ min_i + 1 ] < static_cast< T1 >( in.size( ) - 1 ) )
			{
				min_x[ min_i + 1 ] = static_cast< T1 >( in.size( ) - 1 );
			}
			array< T1 > max_e( in.size( ) );
			array< T1 > min_e( in.size( ) );
			array< T1 > mean_e( in.size( ) );

			emd_envelope( max_x, max_y, max_i + 2, max_e );
			emd_envelope( min_x, min_y, min_i + 2, min_e );

			T1 sd1 = 0.0;

			// ��Z���̒��Ԃ̐��𒊏o���C�V�t�g����
			for( size_t i = 0 ; i < in.size( ) ; i ++ )
			{
				mean_e[ i ] = ( max_e[ i ] + min_e[ i ] ) / 2.0;
				h_new[ i ] = h[ i ] - mean_e[ i ];
				sd1 += ( h[ i ] - h_new[ i ] ) * ( h[ i ] - h_new[ i ] ) / ( h[ i ] * h[ i ] + 0.00001 );
			}

			// IMF���ǂ����̔���
			if( static_cast< double >( sd1 ) < sd )
			{
				break;
			}

			h = h_new;
		}

		if( minmax < 1 || minmax > minmax_old )
		{
			break;
		}
		minmax_old = minmax;

		for( size_t i = 0 ; i < in.size( ) ; i ++ )
		{
			r[ i ] -= h[ i ];
		}
		imf_v.push_back( h );

	}

	imf.resize( imf_v.size( ) + 1 );
	for( size_t i = 0 ; i < imf_v.size( ) ; i ++ )
	{
		imf[ i ] = imf_v[ i ];
	}
	imf( imf_v.size( ) ) = r;

	return ( imf_v.size( ) );
}

/// @}
//  �O���[�v�̏I���

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_HHT_H__
