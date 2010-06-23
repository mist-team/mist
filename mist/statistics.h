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

/// @file mist/statistics.h
//!
//! @brief �f�[�^�̓��v����
//!
#ifndef __INCLUDE_MIST_STATISTICS__
#define __INCLUDE_MIST_STATISTICS__

#include <iostream>
#include <stdexcept>
#include <cstdio>

#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif

#ifndef __INCLUDE_MIST_NUMERIC__
#include "numeric.h"
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

		static value_type t( const value_type &v ){ return( v ); }
	};

	template < class T, class Allocator >
	struct __value_type__< matrix< T, Allocator > >
	{
		typedef matrix< T, Allocator > value_type;

		static value_type t( const value_type &v ){ return( v.t( ) ); }
	};
}


/// @brief ���v����
namespace statistics
{
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
	template < class Array >
	inline typename __utility__::__value_type__< typename Array::value_type >::value_type average( const Array &a )
	{
		typedef typename Array::size_type size_type;
		typedef typename __utility__::__value_type__< typename Array::value_type >::value_type value_type;

		if( a.empty( ) )
		{
			return( value_type( ) );
		}
		else
		{
			value_type v = a[ 0 ];
			for( size_type i = 1 ; i < a.size( ) ; i++ )
			{
				v += a[ i ];
			}
			return( v / static_cast< double >( a.size( ) ) );
		}
	}


	/// �f�[�^�̕��U�i�����U�s��j���v�Z����
	//!
	//! @param[in] a   �c ���U�i�����U�s��j���v�Z����f�[�^�z��
	//! @param[in] ave �c �f�[�^�z��̕��ϒl
	//! 
	//! @return �f�[�^�̕��U�i�����U�s��j
	//! 
	template < class Array >
	inline typename __utility__::__value_type__< typename Array::value_type >::value_type variance( const Array &a, const typename __utility__::__value_type__< typename Array::value_type >::value_type &ave )
	{
		typedef typename Array::size_type size_type;
		typedef __utility__::__value_type__< typename Array::value_type > __utility_type__;
		typedef typename __utility_type__::value_type value_type;

		if( a.empty( ) )
		{
			return( value_type( ) );
		}
		else
		{
			value_type v;
			{
				value_type x = a[ 0 ] - ave;
				v = x * __utility_type__::t( x );
			}

			for( size_type i = 1 ; i < a.size( ) ; i++ )
			{
				value_type x = a[ i ] - ave;
				v += x * __utility_type__::t( x );
			}
			return( v / static_cast< double >( a.size( ) ) );
		}
	}


	/// �f�[�^�̕��U�i�����U�s��j���v�Z����
	//!
	//! @param[in] a �c ���U�i�����U�s��j���v�Z����f�[�^�z��
	//! 
	//! @return �f�[�^�̕��U�i�����U�s��j
	//! 
	template < class Array >
	inline typename __utility__::__value_type__< typename Array::value_type >::value_type variance( const Array &a )
	{
		return( variance( a, average( a ) ) );
	}


	/// �f�[�^�͈̔͂��w�肵�ăq�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] min �c �q�X�g�O�������쐬����f�[�^�̍ŏ��l
	//! @param[in] max �c �q�X�g�O�������쐬����f�[�^�̍ő�l
	//! @param[in] bin �c �q�X�g�O�������쐬����r����
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����
	//! 
	template < class Array1, class Array2 >
	bool generate_histogram( const Array1 &in, Array2 &out, typename Array1::value_type min, typename Array1::value_type max, typename Array1::value_type bin )
	{
		if( is_same_object( in, out ) || in.empty( ) )
		{
			return( false );
		}

		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;

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

		size_type i;
		difference_type num = static_cast< size_type >( ( max - min + 1 ) / bin );
		out.resize( num );

		for( i = 0 ; i < out.size( ) ; i++ )
		{
			out[ i ] = 0;
		}

		for( i = 0 ; i < in.size( ) ; i++ )
		{
			difference_type index = static_cast< difference_type >( ( in[ i ] - min ) / bin );
			if( index >= 0 && index < num )
			{
				out[ index ] += 1;
			}
		}

		return( true );
	}


	/// �f�[�^�͈̔͂��w�肵�ăr���� 1 �̃q�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] min �c �q�X�g�O�������쐬����f�[�^�̍ŏ��l
	//! @param[in] max �c �q�X�g�O�������쐬����f�[�^�̍ő�l
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����
	//! 
	template < class Array1, class Array2 >
	bool generate_histogram( const Array1 &in, Array2 &out, typename Array1::value_type min, typename Array1::value_type max )
	{
		return( generate_histogram( in, out, min, max, 1 ) );
	}


	/// �f�[�^�S�̂���q�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] bin �c �q�X�g�O�������쐬����r����
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����
	//! 
	template < class Array1, class Array2 >
	bool generate_histogram( const Array1 &in, Array2 &out, typename Array1::value_type bin )
	{
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;

		if( in.empty( ) )
		{
			return( false );
		}

		value_type min = in[ 0 ];
		value_type max = in[ 0 ];
		for( size_type i = 1 ; i < in.size( ) ; i++ )
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

		return( generate_histogram( in, out, min, max, bin ) );
	}

	/// �f�[�^�S�̂���r���� 1 �̃q�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����
	//! 
	template < class Array1, class Array2 >
	bool generate_histogram( const Array1 &in, Array2 &out )
	{
		return( generate_histogram( in, out, 1 ) );
	}




	/// �f�[�^�͈̔͂��w�肵��2�����q�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in1  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��1�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] in2  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��2�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out  �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���2�����R���e�i array2 �̂݁j
	//! @param[in] min1 �c �q�X�g�O����1�̃f�[�^�̍ŏ��l
	//! @param[in] max1 �c �q�X�g�O����1�̃f�[�^�̍ő�l
	//! @param[in] min2 �c �q�X�g�O����2�̃f�[�^�̍ŏ��l
	//! @param[in] max2 �c �q�X�g�O����2�̃f�[�^�̍ő�l
	//! @param[in] bin  �c �q�X�g�O�������쐬����r����
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����C���͂ƂȂ�2�̃f�[�^�����قȂ�
	//! 
	template < class Array1, class Array2, class T, class Allocator >
	bool generate_histogram(
								const Array1 &in1, const Array2 &in2, array2< T, Allocator > &out,
								typename Array1::value_type min1, typename Array1::value_type max1,
								typename Array1::value_type min2, typename Array1::value_type max2,
								typename Array1::value_type bin
							)
	{
		if( is_same_object( in1, out ) || is_same_object( in2, out ) || in1.empty( ) || in2.empty( ) || in1.size( ) != in2.size( ) )
		{
			return( false );
		}

		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;

		if( bin <= 0 )
		{
			return( false );
		}

		if( min1 > max1 )
		{
			value_type tmp = min1;
			min1 = max1;
			max1 = tmp;
		}

		if( min2 > max2 )
		{
			value_type tmp = min2;
			min2 = max2;
			max2 = tmp;
		}

		difference_type num1 = static_cast< size_type >( ( max1 - min1 + 1 ) / bin );
		difference_type num2 = static_cast< size_type >( ( max2 - min2 + 1 ) / bin );
		out.resize( num1, num2 );
		out.fill( );

		for( size_type i = 0 ; i < in1.size( ) ; i++ )
		{
			difference_type index1 = static_cast< difference_type >( ( in1[ i ] - min1 ) / bin );
			difference_type index2 = static_cast< difference_type >( ( in2[ i ] - min2 ) / bin );
			if( index1 >= 0 && index1 < num1 && index2 >= 0 && index2 < num2 )
			{
				out( index1, index2 ) += 1;
			}
		}

		return( true );
	}


	/// �f�[�^�͈̔͂��w�肵�ăr���� 1 ��2�����q�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in1  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��1�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] in2  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��2�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out  �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���2�����R���e�i array2 �̂݁j
	//! @param[in] min1 �c �q�X�g�O����1�̃f�[�^�̍ŏ��l
	//! @param[in] max1 �c �q�X�g�O����1�̃f�[�^�̍ő�l
	//! @param[in] min2 �c �q�X�g�O����2�̃f�[�^�̍ŏ��l
	//! @param[in] max2 �c �q�X�g�O����2�̃f�[�^�̍ő�l
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����C���͂ƂȂ�2�̃f�[�^�����قȂ�
	//! 
	template < class Array1, class Array2, class T, class Allocator >
	bool generate_histogram(
								const Array1 &in1, const Array2 &in2, array2< T, Allocator > &out,
								typename Array1::value_type min1, typename Array1::value_type max1,
								typename Array1::value_type min2, typename Array1::value_type max2
							)
	{
		return( generate_histogram( in1, in2, out, min1, max1, min2, max2, 1 ) );
	}


	/// �f�[�^�S�̂���q�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in1  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��1�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] in2  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��2�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out  �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���2�����R���e�i array2 �̂݁j
	//! @param[in] bin �c �q�X�g�O�������쐬����r����
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����C���͂ƂȂ�2�̃f�[�^�����قȂ�
	//! 
	template < class Array1, class Array2, class T, class Allocator >
	bool generate_histogram( const Array1 &in1, const Array2 &in2, array2< T, Allocator > &out, typename Array1::value_type bin )
	{
		typedef typename Array1::size_type size_type;
		typedef typename Array1::difference_type difference_type;
		typedef typename Array1::value_type value_type;

		if( is_same_object( in1, out ) || is_same_object( in2, out ) || in1.empty( ) || in2.empty( ) || in1.size( ) != in2.size( ) )
		{
			return( false );
		}

		value_type min1 = in1[ 0 ];
		value_type max1 = in1[ 0 ];
		for( size_type i = 1 ; i < in1.size( ) ; i++ )
		{
			if( min1 > in1[ i ] )
			{
				min1 = in1[ i ];
			}
			else if( max1 < in1[ i ] )
			{
				max1 = in1[ i ];
			}
		}

		value_type min2 = in2[ 0 ];
		value_type max2 = in2[ 0 ];
		for( size_type i = 1 ; i < in2.size( ) ; i++ )
		{
			if( min2 > in2[ i ] )
			{
				min2 = in2[ i ];
			}
			else if( max2 < in2[ i ] )
			{
				max2 = in2[ i ];
			}
		}

		return( generate_histogram( in1, in2, out, min1, max1, min2, max2, bin ) );
	}


	/// �f�[�^�S�̂���r���� 1 �̃q�X�g�O�������쐬����
	//!
	//! @attention ���́E�o�͂ƂȂ�f�[�^�̔z��Ƃ��āCMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�����p�\�ł��D
	//!
	//! @param[in] in1  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��1�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] in2  �c �q�X�g�O�������쐬���邽�߂̌��ƂȂ�f�[�^�z��2�iMIST�Œ񋟂���R���e�i�CSTL�Œ񋟂���Ă���vector�Cdeque�R���e�i�j
	//! @param[in] out  �c �쐬���ꂽ�q�X�g�O�����iMIST�Œ񋟂���2�����R���e�i array2 �̂݁j
	//! 
	//! @retval true  �c �q�X�g�O�����̍쐬�ɐ���
	//! @retval false �c ���͂Əo�͂������I�u�W�F�N�g���w�肵���ꍇ�C�������̓q�X�g�O�����̍쐬�p�̃f�[�^����C���͂ƂȂ�2�̃f�[�^�����قȂ�
	//! 
	template < class Array1, class Array2, class T, class Allocator >
	bool generate_histogram( const Array1 &in1, const Array2 &in2, array2< T, Allocator > &out )
	{
		return( generate_histogram( in1, in2, out, 1 ) );
	}



	namespace detail
	{
		/// @brief �摜�̒��S���[�����g
		//! @param[in] img      ���͉摜
		//! @param[in] x_order  X�����̃��[�����g�̎���
		//! @param[in] y_order  Y�����̃��[�����g�̎��� 
		//! @param[in] x0       X�����̏d�S
		//! @param[in] y0       Y�����̏d�S
		//! @return ���[�����g
		template< typename T, typename Allocator >
		double central_moment( const array2< T, Allocator > &img, size_t x_order, size_t y_order, double x0, double y0 )
		{
			double r = 0;
			for( size_t y = 0 ; y < img.height() ; ++y )
			{
				for( size_t x = 0 ; x < img.width() ; ++x )
				{
					r += pow( static_cast< double >( x ) - x0, x_order ) * pow( static_cast< double >( y ) - y0, y_order ) * img( x, y );
				}
			}    
			return( r );
		}

		/// @brief �摜���琳�K�����S���[�����g�����߂�
		//! @param[in] img      ���͉摜
		//! @param[in] x_order  X�����̃��[�����g�̎���
		//! @param[in] y_order  Y�����̃��[�����g�̎��� 
		//! @param[in] y00      ���[�����g00(�ʐ�)
		//! @param[in] x0       X�����̏d�S
		//! @param[in] y0       Y�����̏d�S
		//! @return ���[�����g
		template< typename T, typename Allocator >
		double normalized_central_moment( const array2< T, Allocator > &img, size_t x_order, size_t y_order, double u00, double x0, double y0 )
		{
			return( central_moment( img, x_order, y_order, x0, y0 ) / u00 );
		}
	}

	/// @brief �摜�̃��[�����g�����߂�
	//! @param[in] img      ���͉摜
	//! @param[in] x_order  X�����̃��[�����g�̎���
	//! @param[in] y_order  Y�����̃��[�����g�̎��� 
	//! @return ���[�����g
	template< typename T, typename Allocator >
	double moment( const array2< T, Allocator > &img, size_t x_order, size_t y_order )
	{
		double r = 0;

		for( size_t y = 0 ; y < img.height() ; ++y )
		{
			for( size_t x = 0 ; x < img.width() ; ++x )
			{
				r += pow( x, x_order ) * pow( y, y_order ) * img( x, y );
			}
		}

		return( r );
	}

	/// @brief �摜�̒��S���[�����g
	//! @param[in] img      ���͉摜
	//! @param[in] x_order  X�����̃��[�����g�̎���
	//! @param[in] y_order  Y�����̃��[�����g�̎��� 
	//! @return ���[�����g
	template< typename T, typename Allocator >
	double central_moment( const array2< T, Allocator > &img, size_t x_order, size_t y_order )
	{
		double u00 = moment( img, 0, 0 );
		double u10 = moment( img, 1, 0 );
		double u01 = moment( img, 0, 1 );

		double x0 = u10 / u00;
		double y0 = u01 / u00;

		return( detail::central_moment( img, x_order, y_order, x0, y0 ) );
	}



	/// @brief �摜���琳�K�����S���[�����g�����߂�
	//! @param[in] img      ���͉摜
	//! @param[in] x_order  X�����̃��[�����g�̎���
	//! @param[in] y_order  Y�����̃��[�����g�̎��� 
	//! @return ���[�����g
	template< typename T, typename Allocator >
	double normalized_central_moment( const array2< T, Allocator > &img, size_t x_order, size_t y_order )
	{
		double u00 = moment( img, 0, 0 );
		double u10 = moment( img, 1, 0 );
		double u01 = moment( img, 0, 1 );
		double x0 = u10 / u00;
		double y0 = u01 / u00;
		return( detail::normalized_central_moment( img, x_order, y_order, u00, x0, y0 ) );
	}


	/// @brief �摜����Hu���[�����g�����߂�
	//! @param[in]  img      ���͉摜
	//! @param[out] moments  Hu���[�����g(7����)
	template< typename T, typename Allocator >
	void hu_moments( const array2< T, Allocator > &img, array1< double > &moments )
	{
		double u00 = moment( img, 0, 0 );
		double u10 = moment( img, 1, 0 );
		double u01 = moment( img, 0, 1 );
		double x0 = u10 / u00;
		double y0 = u01 / u00;

		// ���K�����S���[�����g���v�Z
		array2< double > ncm( 4, 4 );
		for( size_t x = 0 ; x <= 3 ; ++x )
		{
			for( size_t y = 0 ; y <= 3 ; ++y )
			{
				if( x + y > 3 )
				{
					continue;
				}	  
				ncm( x, y ) = detail::normalized_central_moment( img, x, y, u00, x0, y0 );	  
			}
		}

		// Hu���[�����g���v�Z
		moments.resize( 7 );
		moments( 0 ) = ncm( 2, 0 ) * ncm( 0, 2 );
		moments( 1 ) = pow( ncm( 2, 0 ) - ncm( 0, 2 ), 2.0 ) + 4 * pow( ncm( 1, 1 ), 2.0 );
		moments( 2 ) = pow( ncm( 3, 0 ) - 3 * ncm( 1, 2 ), 2.0 ) + pow( 3 * ncm( 2, 1 ) - ncm( 0, 3 ), 2.0 );
		moments( 3 ) = pow( ncm( 3, 0 ) * ncm( 1, 2 ), 2.0 ) + pow( ncm( 2, 1 ) + ncm( 0, 3 ), 2.0 );
		moments( 4 ) = ( ncm( 3, 0 ) - 3 * ncm( 1, 2 ) ) *
					   ( ncm( 3, 0 ) + ncm( 1, 2 ) ) *
					   ( pow( ncm( 3, 0 ) + ncm( 1, 2 ), 2.0 ) - 3 * pow( ncm( 2, 1 ) + ncm( 0, 3 ), 2.0 ) ) +
					   ( 3 * ncm( 2, 1 ) - ncm( 0, 3 ) ) *
					   ( ncm( 2, 1 ) + ncm( 0, 3 ) ) *
					   ( 3 * pow( ncm( 3, 0 ) + ncm( 1, 2 ), 2.0 ) - pow( ncm( 2, 1 ) + ncm( 0, 3 ), 2.0 ) );    
		moments( 5 ) = ( ncm( 2, 0 ) - ncm( 0, 2 ) ) *
					   ( pow( ncm( 3, 0 ) + ncm( 1, 2 ), 2.0 ) - pow( ncm( 2, 1 ) + ncm( 0, 3 ), 2.0 ) ) +
					   4 * ncm( 1, 1 ) * ( ncm( 3, 0 ) + ncm( 1, 2 ) ) * ( ncm( 2, 1 ) + ncm( 0, 3 ) );
		moments( 6 ) = ( 3 * ncm( 2, 1 ) - ncm( 0, 3 ) ) *
					   ( ncm( 2, 1 ) + ncm( 0, 3 ) ) *
					   ( 3 * pow( ncm( 3, 0 ) + ncm( 1, 2 ), 2.0 ) - pow( ncm( 2, 1 ) + ncm( 0, 3 ), 2.0 ) ) -
					   ( ncm( 3, 0 ) - 3 * ncm( 2, 1 ) ) *
					   ( ncm( 2, 1 ) + ncm( 0, 3 ) ) *
					   ( 3 * pow( ncm( 3, 0 ) + ncm( 1, 2 ), 2.0 ) - pow( ncm( 2, 1 ) + ncm( 0, 3 ), 2.0 ) ); 
	}
  
  namespace normal_distribution
  {
    /// @brief Obtain propability
    //! @param[in] x is input data
    //! @param[in] u is average
    //! @param[in] sigma is variance
    //! @return probability
    template< typename T, typename Allocator >
    double probability( const mist::matrix< T, Allocator > &x, const mist::matrix< T, Allocator > &u, const mist::matrix< T, Allocator > &sigma )
    {
	if( x.rows() != u.rows() || u.cols() != 1 || x.rows() != sigma.rows() || sigma.rows() != sigma.cols() )
	  {
	    throw std::invalid_argument( "" );
	  }
      mist::matrix< T, Allocator > b = x - u;
      double t = ( b.t() * mist::inverse( sigma ) * b )( 0, 0 );      
      double d = mist::det( sigma );      
      return ( 1.0 / pow( 2 * M_PI, 0.5 * x.rows() ) ) * ( 1.0 / d ) * exp( -0.5 * t );
    }

    /// @brief Estimate sample average and variance
    //! @param[in] sample is training sample
    //! @param[in] average
    //! @param[in] variant
    template< typename T, typename Allocator >
    void estimate( const mist::matrix< T, Allocator > &sample, mist::matrix< T, Allocator > &average, mist::matrix< T, Allocator > &variance )
    {
      if( sample.cols() <= 1 )
	{
	  throw std::invalid_argument( "" );
	}

      average.resize( sample.rows(), 1 );
      for( size_t i = 0 ; i < sample.cols() ; ++i )
	{
	  for( size_t j = 0 ; j < sample.rows() ; ++j )
	    {
	      average( j, 0 ) += sample( j, i );
	    }
	}
      average /= sample.cols();

      mist::matrix< T, Allocator > tmp = sample;
      for( size_t i = 0 ; i < sample.cols() ; ++i )
	{
	  for( size_t j = 0 ; j < sample.rows() ; ++j )
	    {
	      tmp( j, i ) -= average( j, 0 );
	    }
	}
      variance = ( tmp * tmp.t() ) / ( sample.cols() - 1 );
    }
  }


  namespace parzen
  {
    /// @brief Estimate probability density
    //! @param[in] sample is training sample
    //! @param[in] x is input data
    //! @param[in] band is band length
    //! @return probability density
    template< typename T, typename Allocator >
    double density( const mist::matrix< T, Allocator > &sample, const mist::matrix< T, Allocator > &x, double band )
    {
      if( sample.cols() == 0 )
	{
	  throw std::invalid_argument( "" );
	}

      if( sample.rows() != x.rows() )
	{
	  throw std::invalid_argument( "" );
	}

      if( band <= 0 )
	{
	  throw std::invalid_argument( "" );
	}

      double t = 0.0;
      for( size_t i = 0 ; i < sample.cols() ; ++i )
	{
	  double a = 1.0;
	  for( size_t j = 0 ; j < sample.rows() ; ++j )
	    {
	      double s = fabs( sample( j, i ) - x( j, 0 ) ) / band;
	      if( s > 0.5 )
		{
		  a = 0.0;
		  break;
		}
	    }
	  t += a;
	}
      return ( 1.0 / ( pow( band, sample.rows() ) * sample.cols() ) ) * t;
    }

    /// @brief Varidate band length
    //! @param[in] sample is training sample
    //! @param[in] band is band length
    //! @param[in] n is number of cross-varidation
    //! @return probability density
    template< typename T, typename Allocator >
    double test( const mist::matrix< T, Allocator > &sample, double band, int n = 5 )
    {
      if( sample.cols() == 0 )
	{
	  throw std::invalid_argument( "" );
	}

      if( n == 0 )
	{
	  throw std::invalid_argument( "" );
	}

      if( band <= 0 )
	{
	  throw std::invalid_argument( "" );
	}

      int ns = sample.cols() / n;
      int n1 = ns * ( n - 1 );

      double J = 0;

      mist::matrix< T, Allocator > a( sample.rows(), n1 );
      for( int i = 0 ; i < n ; ++i )
	{
	  int idx = 0;
	  for( size_t j = 0 ; j < sample.cols() ; ++j )
	    {
	      if( static_cast< int >( j / ns ) != i )
		{
		  for( size_t k = 0 ; k < sample.rows() ; ++k )
		    {
		      a( k, idx ) = sample( k, j );
		    }
		  ++idx;
		}
	    }

	  for( size_t j = 0 ; j < sample.cols() ; ++j )
	    {
	      if( static_cast< int >( j / ns ) == i )
		{
		  mist::matrix< T, Allocator > x( sample.rows(), 1 );
		  sample.trim( x, 0, j, -1, 1 );		  
		  J += log( density( a, x, band ) );
		}	  	  
	    }
	}      
      return J / n;
    }
  }

  namespace kernel
  {
    /// @brief Estimate probability density
    //! @param[in] sample is training sample
    //! @param[in] x is input data
    //! @param[in] band is band length
    //! @return probability density
    template< typename T, typename Allocator >
    double density( const mist::matrix< T, Allocator > &sample, const mist::matrix< T, Allocator > &x, double b )
    {
      double t = 0.0;
      for( size_t i = 0 ; i < sample.cols() ; ++i )
	{
	  mist::matrix< T, Allocator > d = x;
	  for( size_t j = 0 ; j < sample.rows() ; ++j )
	    {
	      d( j, 0 ) -= sample( j, i );
	    }
	  d /= b;	  
	  t += 1.0 / pow( 2.0 * M_PI, 0.5 * sample.rows() ) * exp( -0.5 * ( d.t() * d )( 0, 0 ) );
	}      
      return ( 1.0 / ( pow( b, sample.rows() ) * sample.cols() ) ) * t;
    }

    /// @brief Varidate band length
    //! @param[in] sample is training sample
    //! @param[in] band is band length
    //! @param[in] n is number of cross-varidation
    //! @return probability density
    template< typename T, typename Allocator >
    double test( const mist::matrix< T, Allocator > &sample, double band, int n = 5 )
    {
      if( sample.cols() == 0 )
	{
	  throw std::invalid_argument( "" );
	}

      if( n == 0 )
	{
	  throw std::invalid_argument( "" );
	}

      if( band <= 0 )
	{
	  throw std::invalid_argument( "" );
	}

      int ns = sample.cols() / n;
      int n1 = ns * ( n - 1 );

      double J = 0;

      mist::matrix< T, Allocator > a( sample.rows(), n1 );
      for( int i = 0 ; i < n ; ++i )
	{
	  int idx = 0;
	  for( size_t j = 0 ; j < sample.cols() ; ++j )
	    {
	      if( static_cast< int >( j / ns ) != i )
		{
		  for( size_t k = 0 ; k < sample.rows() ; ++k )
		    {
		      a( k, idx ) = sample( k, j );
		    }
		  ++idx;
		}
	    }

	  for( size_t j = 0 ; j < sample.cols() ; ++j )
	    {
	      if( static_cast< int >( j / ns ) == i )
		{
		  mist::matrix< T, Allocator > x( sample.rows(), 1 );
		  sample.trim( x, 0, j, -1, 1 );		  
		  J += log( density( a, x, band ) );
		}	  	  
	    }
	}      
      return J / n;
    }
  }

  namespace knn
  {
    /// @brief Estimate probability density
    //! @param[in] sample is training sample
    //! @param[in] x is input data
    //! @param[in] k is number of nearest neighbor
    //! @return probability density
    template< typename T, typename Allocator >
    double density( const mist::matrix< T, Allocator > &sample, const mist::matrix< T, Allocator > &x, int k )
    {
      std::vector< T, Allocator > ssample( sample.size() );
      for( int i = 0 ; i < static_cast< int >( sample.cols() ) ; ++i )
	{
	  double dist = 0;
	  for( int j = 0 ; j < static_cast< int >( sample.rows() ) ; ++j )
	    {
	      dist += pow( x( j, 0 ) - sample( j, i ), 2.0 );
	    }
	  ssample[ i ] = dist;
	}
  
      std::sort( ssample.begin(), ssample.end() );

      double r = sqrt( ssample[ k - 1 ] );

      int h = 0;
      int d = static_cast< int >( sample.rows() );
      double g = 1.0;
      if( d % 2 == 0 )
	{
	  d /= 2;
	  while( d != 1 )
	    {
	      g *= d;
	      d -= 1;
	    }
	}
      else
	{
	  g = 0.5;
	  while( d != 1 )
	    {
	      g *= ( 0.5 * d );
	      d -= 2;
	    }
	  h = 1;
	}
      return ( g * k ) / ( pow( M_PI, 0.5 * ( sample.rows() - h ) ) * pow( r, sample.rows() ) * sample.cols() );
    }

    /// @brief Varidate band length
    //! @param[in] sample is training sample
    //! @param[in] k is number of nearest neighbor
    //! @param[in] n is number of cross-varidation
    //! @return probability density
    template< typename T, typename Allocator >
    double test( const mist::matrix< T, Allocator > &sample, int k, int n = 5 )
    {
      if( sample.cols() == 0 )
	{
	  throw std::invalid_argument( "" );
	}

      if( n == 0 )
	{
	  throw std::invalid_argument( "" );
	}

      if( k <= 0 )
	{
	  throw std::invalid_argument( "" );
	}

      int ns = sample.cols() / n;
      int n1 = ns * ( n - 1 );

      double J = 0;

      mist::matrix< T, Allocator > a( sample.rows(), n1 );
      for( int i = 0 ; i < n ; ++i )
	{
	  int idx = 0;
	  for( size_t j = 0 ; j < sample.cols() ; ++j )
	    {
	      if( static_cast< int >( j / ns ) != i )
		{
		  for( size_t k = 0 ; k < sample.rows() ; ++k )
		    {
		      a( k, idx ) = sample( k, j );
		    }
		  ++idx;
		}
	    }

	  for( size_t j = 0 ; j < sample.cols() ; ++j )
	    {
	      if( static_cast< int >( j / ns ) == i )
		{
		  mist::matrix< T, Allocator > x( sample.rows(), 1 );
		  sample.trim( x, 0, j, -1, 1 );
		  J += log( density( a, x, k ) );
		}	  	  
	    }
	}      
      return J / n;
    }
  }
	/// @}
	//  ���v�����̏I���
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
