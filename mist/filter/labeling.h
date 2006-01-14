// 
// Copyright (c) 2003-2005, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/filter/labeling.h
//!
//! @brief �e�����̉摜�ɑΉ������C���x�����O�A���S���Y��
//! 
//! - �Q�l����
//!   - �đq�B�L, ����Ύ�, ���e����Y, �����W�v, "�O�����f�B�W�^����Ԃɂ�����}�`�̘A�����ƃI�C���[��," �d�q�ʐM�w��_����D, J65-D, No.1, pp.80-87, 1982
//!   - ���e����Y, "3�����f�B�W�^���摜����," ���W��, 2002
//!

#ifndef __INCLUDE_MIST_LABELING__
#define __INCLUDE_MIST_LABELING__

#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#include <vector>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __labeling_controller__
{
	template < class T > struct default_label_num2
	{
		_MIST_CONST( size_t, value, 2560 );
	};
	template < > struct default_label_num2< char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num2< signed char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num2< unsigned char >
	{
		_MIST_CONST( size_t, value, 255 );
	};

	template < class T > struct default_label_num3
	{
		_MIST_CONST( size_t, value, 10000 );
	};
	template < > struct default_label_num3< char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num3< signed char >
	{
		_MIST_CONST( size_t, value, 127 );
	};
	template < > struct default_label_num3< unsigned char >
	{
		_MIST_CONST( size_t, value, 255 );
	};


	template < int nc >
	struct neighbors
	{
		_MIST_CONST( size_t, array_num, 13 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[  0 ] =                           k > 0 ? T[ static_cast< size_type >( in( i    , j    , k - 1 ) ) ] : 0;
			L[  1 ] =              j     > 0          ? T[ static_cast< size_type >( in( i    , j - 1, k     ) ) ] : 0;
			L[  2 ] = i     > 0                       ? T[ static_cast< size_type >( in( i - 1, j    , k     ) ) ] : 0;
			L[  3 ] =              j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i    , j - 1, k - 1 ) ) ] : 0;
			L[  4 ] = i     > 0 &&              k > 0 ? T[ static_cast< size_type >( in( i - 1, j    , k - 1 ) ) ] : 0;
			L[  5 ] = i + 1 < w &&              k > 0 ? T[ static_cast< size_type >( in( i + 1, j    , k - 1 ) ) ] : 0;
			L[  6 ] =              j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i    , j + 1, k - 1 ) ) ] : 0;
			L[  7 ] = i     > 0 && j     > 0          ? T[ static_cast< size_type >( in( i - 1, j - 1, k     ) ) ] : 0;
			L[  8 ] = i + 1 < w && j     > 0          ? T[ static_cast< size_type >( in( i + 1, j - 1, k     ) ) ] : 0;
			L[  9 ] = i     > 0 && j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i - 1, j - 1, k - 1 ) ) ] : 0;
			L[ 10 ] = i + 1 < w && j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i + 1, j - 1, k - 1 ) ) ] : 0;
			L[ 11 ] = i     > 0 && j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i - 1, j + 1, k - 1 ) ) ] : 0;
			L[ 12 ] = i + 1 < w && j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i + 1, j + 1, k - 1 ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 18 >
	{
		_MIST_CONST( size_t, array_num, 9 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[  0 ] =                           k > 0 ? T[ static_cast< size_type >( in( i    , j    , k - 1 ) ) ] : 0;
			L[  1 ] =              j     > 0          ? T[ static_cast< size_type >( in( i    , j - 1, k     ) ) ] : 0;
			L[  2 ] = i     > 0                       ? T[ static_cast< size_type >( in( i - 1, j    , k     ) ) ] : 0;
			L[  3 ] =              j     > 0 && k > 0 ? T[ static_cast< size_type >( in( i    , j - 1, k - 1 ) ) ] : 0;
			L[  4 ] = i     > 0 &&              k > 0 ? T[ static_cast< size_type >( in( i - 1, j    , k - 1 ) ) ] : 0;
			L[  5 ] = i + 1 < w &&              k > 0 ? T[ static_cast< size_type >( in( i + 1, j    , k - 1 ) ) ] : 0;
			L[  6 ] =              j + 1 < h && k > 0 ? T[ static_cast< size_type >( in( i    , j + 1, k - 1 ) ) ] : 0;
			L[  7 ] = i     > 0 && j     > 0          ? T[ static_cast< size_type >( in( i - 1, j - 1, k     ) ) ] : 0;
			L[  8 ] = i + 1 < w && j     > 0          ? T[ static_cast< size_type >( in( i + 1, j - 1, k     ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 6 >
	{
		_MIST_CONST( size_t, array_num, 3 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[  0 ] =                           k > 0 ? T[ static_cast< size_type >( in( i    , j    , k - 1 ) ) ] : 0;
			L[  1 ] =              j     > 0          ? T[ static_cast< size_type >( in( i    , j - 1, k     ) ) ] : 0;
			L[  2 ] = i     > 0                       ? T[ static_cast< size_type >( in( i - 1, j    , k     ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 8 >
	{
		_MIST_CONST( size_t, array_num, 4 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[ 0 ] = i     > 0 && j > 0 ? T[ static_cast< size_type >( in( i - 1, j - 1 ) ) ] : 0;
			L[ 1 ] =              j > 0 ? T[ static_cast< size_type >( in( i    , j - 1 ) ) ] : 0;
			L[ 2 ] = i + 1 < w && j > 0 ? T[ static_cast< size_type >( in( i + 1, j - 1 ) ) ] : 0;
			L[ 3 ] = i     > 0          ? T[ static_cast< size_type >( in( i - 1, j     ) ) ] : 0;

			return( 0 );
		}
	};

	template < >
	struct neighbors< 4 >
	{
		_MIST_CONST( size_t, array_num, 2 );

		template < class Array, class Vector >
		static inline typename Array::size_type neighbor( Array &in, const Vector &T, typename Vector::value_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[ 0 ] = j > 0 ? T[ static_cast< size_type >( in( i    , j - 1 ) ) ] : 0;
			L[ 1 ] = i > 0 ? T[ static_cast< size_type >( in( i - 1, j     ) ) ] : 0;

			return( 0 );
		}
	};


	template < class Array, class neighbor, class Functor >
	typename Array::size_type labeling( Array &in, typename Array::size_type label_max, const neighbor dmy, Functor f )
	{
		typedef typename Array::size_type size_type;
		typedef typename Array::difference_type difference_type;
		typedef typename Array::value_type value_type;

//		typedef difference_type label_value_type;
//		typedef size_type label_value_type;
		typedef unsigned int label_value_type;

		typedef std::vector< label_value_type >::value_type vector_label_value_type;

		size_type label_num = 0;
		size_type i, j, k, l, count;

		std::vector< label_value_type > T;
		label_value_type L[ neighbor::array_num ];
		const size_type width = in.width( );
		const size_type height = in.height( );
		const size_type depth = in.depth( );

		T.reserve( label_max );
		T.push_back( 0 );	// T[ 0 ]

		const bool bprogress1 = depth == 1;
		const bool bprogress2 = depth >  1;

		label_max = type_limits< value_type >::maximum( );

		f( 0.0 );

		for( k = 0 ; k < depth ; k++ )
		{
			for( j = 0 ; j < height ; j++ )
			{
				for( i = 0 ; i < width ; i++ )
				{
					// 0��f�̓��x�����O���Ȃ�
					if( in( i, j, k ) == 0 )
					{
						continue;
					}

					// ����ς݂̗v�f�̃��x�����擾����
					neighbor::neighbor( in, T, L, i, j, k, width, height, depth );

					// �ߖT�ōŏ��̃��x���ԍ��������̂��擾���C���x���ԍ���0�Ŗ������̐��𐔂��グ��
					label_value_type L1 = static_cast< label_value_type >( label_max );
					for( l = 0, count = 0 ; l < neighbor::array_num ; l++ )
					{
						if( L[ l ] > 0 )
						{
							if( L1 != L[ l ] )
							{
								count++;
							}
							if( L1 > L[ l ] )
							{
								L1 = L[ l ];
							}
						}
					}

					if( count == 0 )
					{
						// �ߖT�ɁC���łɊ��蓖�Ă��Ă��郉�x���͑��݂��Ȃ����߁C�V�K�Ƀ��x�������蓖�Ă�
						// �o�̓��x���l���o�̓f�[�^�^�̍ő�l�𒴂��Ȃ���΁C���x�������X�V����
						if( label_num < label_max )
						{
							label_num++;
						}
						T.push_back( static_cast< label_value_type >( label_num ) );

						in( i, j, k ) = static_cast< value_type >( label_num );
					}
					else if( count == 1 )
					{
						// �ߖT�ɁC���蓖�Ă��Ă��郉�x����1�������݂��Ȃ����߁C���̃��x�������蓖�Ă�
						in( i, j, k ) = static_cast< value_type >( L1 );
					}
					else
					{
						in( i, j, k ) = static_cast< value_type >( L1 );

						// �����̃��x�����������邽�߁C�e�[�u�����C������
						for( l = 0 ; l < neighbor::array_num ; l++ )
						{
							if( T[ L[ l ] ] != L1 )
							{
								for( size_type m = 1 ; m <= label_num ; m++ )
								{
									if( T[ m ] == L[ l ] )
									{
										T[ m ] = static_cast< vector_label_value_type >( L1 );
									}
								}
							}
						}
					}
				}

				if( bprogress1 )
				{
					f( static_cast< double >( j + 1 ) / static_cast< double >( height ) * 100.0 );
				}
			}

			if( bprogress2 )
			{
				f( static_cast< double >( k + 1 ) / static_cast< double >( depth ) * 100.0 );
			}
		}

		f( 100.0 );

		// ���x���̐U�蒼�����s��
		size_type *NT = new size_type[ label_num + 1 ];
		for( i = 0 ; i <= label_num ; i++ )
		{
			NT[ i ] = 0;
		}

		// �g�p���Ă��郉�x�����`�F�b�N����
		for( i = 1 ; i <= label_num ; i++ )
		{
			NT[ T[ i ] ] = 1;
		}

		// �g�p���Ă��郉�x�����`�F�b�N����
		for( i = 1, count = 1 ; i <= label_num ; i++ )
		{
			if( NT[ i ] != 0 )
			{
				NT[ i ] = count++;
			}
		}

		label_num = 0;
		for( i = 0 ; i < in.size( ) ; i++ )
		{
			if( in[ i ] == 0 )
			{
				continue;
			}

			size_type label = NT[ T[ static_cast< size_type >( in[ i ] ) ] ];
			in[ i ] = static_cast< value_type >( label );
			if( label_num < label )
			{
				label_num = label;
			}
		}

		f( 100.1 );

		delete [] NT;

		return( label_num );
	}
}




//! @addtogroup labeling_group ���x�����O
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/labeling.h>
//! @endcode
//!
//!  @{


/// @brief 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! @param[in]  f         �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array2< T2, Allocator2 >::size_type labeling4( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] > 0 ? 1 : 0;
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 4 >( ), f ) );
}


/// @brief 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling4( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	return( labeling4( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! @param[in]  f         �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array2< T2, Allocator2 >::size_type  size_type;
	typedef typename array2< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] > 0 ? 1 : 0;
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 8 >( ), f ) );
}


/// @brief 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	return( labeling8( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 3�����摜�ɑ΂���6�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���6�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! @param[in]  f         �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array3< T2, Allocator2 >::size_type labeling6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] > 0 ? 1 : 0;
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 6 >( ), f ) );
}

/// @brief 3�����摜�ɑ΂���6�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���6�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array3< T2, Allocator2 >::size_type labeling6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	return( labeling6( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 3�����摜�ɑ΂���18�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���18�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! @param[in]  f         �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array3< T2, Allocator2 >::size_type labeling18( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] > 0 ? 1 : 0;
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 18 >( ), f ) );
}

/// @brief 3�����摜�ɑ΂���18�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���18�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array3< T2, Allocator2 >::size_type labeling18( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	return( labeling18( in, out, max_label, __mist_dmy_callback__( ) ) );
}


/// @brief 3�����摜�ɑ΂���26�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���26�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! @param[in]  f         �c �i�s�󋵂�Ԃ��R�[���o�b�N�֐�
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2, class Functor >
typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label, Functor f )
{
	typedef typename array3< T2, Allocator2 >::size_type  size_type;
	typedef typename array3< T2, Allocator2 >::value_type value_type;

	if( max_label == 0 )
	{
		max_label = type_limits< value_type >::maximum( );
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = in[ i ] > 0 ? 1 : 0;
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 26 >( ), f ) );
}


/// @brief 3�����摜�ɑ΂���26�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���26�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! @attention �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! @attention �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! @attention �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	return( labeling26( in, out, max_label, __mist_dmy_callback__( ) ) );
}




/// @brief ����͈͓��ł̍ő�̃��x���𒊏o����
//! 
//! 3�����摜�ɑ΂���8�ߖT�^���x�����O��p���āC�ő�A�������𒊏o����
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  sx        �c X�������̏��������̈ʒu
//! @param[in]  ex        �c X�������̑傫�����̈ʒu
//! @param[in]  sy        �c Y�������̏��������̈ʒu
//! @param[in]  ey        �c Y�������̑傫�����̈ʒu
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region(
						const array2< T1, Allocator1 > &in,
						array2< T2, Allocator2 > &out,
						typename array2< T1, Allocator1 >::size_type sx,
						typename array2< T1, Allocator1 >::size_type ex,
						typename array2< T1, Allocator1 >::size_type sy,
						typename array2< T1, Allocator1 >::size_type ey,
						typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value
					)
{
	typedef typename array2< T1, Allocator1 >::size_type size_type;
	typedef typename array2< T1, Allocator1 >::difference_type difference_type;

	array2< int > tmp;
	size_type i, j;

	size_type label_num = mist::labeling8( in, tmp, max_label );

	// �w�肳�ꂽ�͈͓��̍ő僉�x����T��
	sx = sx < 0 ? 0 : sx;
	sy = sy < 0 ? 0 : sy;
	sx = sx < in.width( )  ? sx : in.width( )  - 1;
	sy = sy < in.height( ) ? sy : in.height( ) - 1;

	ex = ex < 0 ? 0 : ex;
	ey = ey < 0 ? 0 : ey;
	ex = ex < in.width( )  ? ex : in.width( )  - 1;
	ey = ey < in.height( ) ? ey : in.height( ) - 1;

	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( j = sy ; j <= ey ; j++ )
	{
		for( i = sx ; i <= ex ; i++ )
		{
			menseki[ tmp( i, j ) ]++;
		}
	}

	max_label = 1;
	for( i = 2 ; i <= label_num ; i++ )
	{
		max_label = menseki[ i ] > menseki[ max_label ] ? i : max_label;
	}
	delete [] menseki;

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = tmp[ i ] == max_label ? 1 : 0;
	}
}


/// @brief �摜�̑S�͈͓��ł̍ő�̃��x���𒊏o����
//! 
//! 3�����摜�ɑ΂���8�ߖT�^���x�����O��p���āC�ő�A�������𒊏o����
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	maximum_region( in, out, 0, in.width( ) - 1, 0, in.height( ) - 1, max_label );
}



/// @brief ����͈͓��ł̍ő�̃��x���𒊏o����
//! 
//! 3�����摜�ɑ΂���26�ߖT�^���x�����O��p���āC�ő�A�������𒊏o����
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  sx        �c X�������̏��������̈ʒu
//! @param[in]  ex        �c X�������̑傫�����̈ʒu
//! @param[in]  sy        �c Y�������̏��������̈ʒu
//! @param[in]  ey        �c Y�������̑傫�����̈ʒu
//! @param[in]  sz        �c Z�������̏��������̈ʒu
//! @param[in]  ez        �c Z�������̑傫�����̈ʒu
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region(
						const array3< T1, Allocator1 > &in,
						array3< T2, Allocator2 > &out,
						typename array3< T1, Allocator1 >::size_type sx,
						typename array3< T1, Allocator1 >::size_type ex,
						typename array3< T1, Allocator1 >::size_type sy,
						typename array3< T1, Allocator1 >::size_type ey,
						typename array3< T1, Allocator1 >::size_type sz,
						typename array3< T1, Allocator1 >::size_type ez,
						typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value
					)
{
	typedef typename array3< T1, Allocator1 >::size_type size_type;
	typedef typename array3< T1, Allocator1 >::difference_type difference_type;

	array3< size_type > tmp;
	size_type i, j, k;

	size_type label_num = mist::labeling26( in, tmp, max_label );

	// �w�肳�ꂽ�͈͓��̍ő僉�x����T��
	sx = sx < 0 ? 0 : sx;
	sy = sy < 0 ? 0 : sy;
	sz = sz < 0 ? 0 : sz;
	sx = sx < in.width( )  ? sx : in.width( )  - 1;
	sy = sy < in.height( ) ? sy : in.height( ) - 1;
	sz = sz < in.depth( )  ? sz : in.depth( )  - 1;

	ex = ex < 0 ? 0 : ex;
	ey = ey < 0 ? 0 : ey;
	ez = ez < 0 ? 0 : ez;
	ex = ex < in.width( )  ? ex : in.width( )  - 1;
	ey = ey < in.height( ) ? ey : in.height( ) - 1;
	ez = ez < in.depth( )  ? ez : in.depth( )  - 1;

	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( k = sz ; k <= ez ; k++ )
	{
		for( j = sy ; j <= ey ; j++ )
		{
			for( i = sx ; i <= ex ; i++ )
			{
				menseki[ tmp( i, j, k ) ]++;
			}
		}
	}

	max_label = 1;
	for( i = 2 ; i <= label_num ; i++ )
	{
		max_label = menseki[ i ] > menseki[ max_label ] ? i : max_label;
	}
	delete [] menseki;

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = tmp[ i ] == max_label ? 1 : 0;
	}
}


/// @brief �摜�̑S�͈͓��ł̍ő�̃��x���𒊏o����
//! 
//! 3�����摜�ɑ΂���6�ߖT�^���x�����O��p���āC�ő�A�������𒊏o����
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void maximum_region( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	maximum_region( in, out, 0, in.width( ) - 1, 0, in.height( ) - 1, 0, in.depth( ) - 1, max_label );
}



/// @}
//  ���x�����O�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_LABELING__
