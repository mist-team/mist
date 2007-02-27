// 
// Copyright (c) 2003-2007, MIST Project, Intelligent Media Integration COE, Nagoya University
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
			return( value_type( 0 ) );
		}
		else
		{
			value_type v = value_type( 0 );
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
	template < class Array >
	inline typename __utility__::__value_type__< typename Array::value_type >::value_type variance( const Array &a, double ave )
	{
		typedef typename Array::size_type size_type;
		typedef typename __utility__::__value_type__< typename Array::value_type >::value_type value_type;

		if( a.empty( ) )
		{
			return( value_type( 0 ) );
		}
		else
		{
			value_type v = value_type( 0 );
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


	/// @}
	//  ���v�����̏I���
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_STATISTICS__
