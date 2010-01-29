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
#include <list>



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
		typedef std::list< label_value_type > label_list_type;

		size_type label_num = 0;
		size_type i, j, k, l, count;

		std::vector< label_value_type > T;
		std::vector< label_list_type > TBL;
		label_value_type L[ neighbor::array_num ];
		const size_type width = in.width( );
		const size_type height = in.height( );
		const size_type depth = in.depth( );

		T.reserve( label_max );
		T.push_back( 0 );	// T[ 0 ]
		TBL.push_back( label_list_type( ) );	// T[ 0 ]

		const bool bprogress1 = depth == 1;
		const bool bprogress2 = depth >  1;

		if( is_float< value_type >::value )
		{
			label_max = type_limits< size_type >::maximum( );
		}
		else
		{
			label_max = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}

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

						label_list_type ll;
						ll.push_back( static_cast< label_value_type >( label_num ) );
						TBL.push_back( ll );

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
							label_value_type L0 = L[ l ];
							if( T[ L0 ] != L1 )
							{
								typename label_list_type::iterator ite  = TBL[ L0 ].begin( );
								typename label_list_type::iterator eite = TBL[ L0 ].end( );

								for( ; ite != eite ; ++ite )
								{
									T[ *ite ] = static_cast< vector_label_value_type >( L1 );
								}

								label_list_type &TBL1 = TBL[ L0 ];
								TBL[ L1 ].insert( TBL[ L1 ].end( ), TBL[ L0 ].begin( ), TBL[ L0 ].end( ) );
								TBL[ L0 ].clear( );
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
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 4 >( ), f ) );
}


/// @brief 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 8 >( ), f ) );
}


/// @brief 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 6 >( ), f ) );
}

/// @brief 3�����摜�ɑ΂���6�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���6�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 18 >( ), f ) );
}

/// @brief 3�����摜�ɑ΂���18�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���18�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
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
		if( is_float< value_type >::value )
		{
			max_label = type_limits< size_type >::maximum( );
		}
		else
		{
			max_label = static_cast< size_type >( type_limits< value_type >::maximum( ) );
		}
	}

	out.resize( in.size1( ), in.size2( ), in.size3( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );

	for( size_type i = 0 ; i < in.size( ) ; i++ )
	{
		out[ i ] = static_cast< value_type >( in[ i ] > 0 ? 1 : 0 );
	}
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 26 >( ), f ) );
}


/// @brief 3�����摜�ɑ΂���26�ߖT�^���x�����O
//! 
//! 3�����摜�ɑ΂���26�ߖT�^���x�����O
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! 
//! �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
//! �܂��C�ő僉�x�����𒴂����ꍇ�͎����I�ɍő僉�x�������X�V����D���̂��߁C�������g�p�ʂ���������\��������D
//! �����l�� max_label ��傫���Ƃ邱�ƂŃ������̍Ċm�ۂ����荂���ɓ��삷��悤�ɂȂ�i���̂����C�����������ʂ���������j�D
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x�����̃f�t�H���g�l
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
inline typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	return( labeling26( in, out, max_label, __mist_dmy_callback__( ) ) );
}




/// @brief ����͈͓��ł̍ő�̃��x���𒊏o����
//! 
//! 2�����摜�ɑ΂���8�ߖT�^���x�����O��p���āC�ő�A�������𒊏o����
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

	array2< size_type > tmp;
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
		out[ i ] = static_cast< typename array3< T2, Allocator2 >::value_type >( tmp[ i ] == max_label ? 1 : 0 );
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




/// @brief �摜��0/1�𔽓]�����Č����ߏ������s��
//! 
//! 2�����摜�ɑ΂���4�ߖT�^���x�����O��p���āC�����ߏ������s��
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  include_corner_labels �c whether to also remove holes of the labels containing the corners of the image
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, const bool include_corner_labels, typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	typedef typename array2< T1, Allocator1 >::size_type size_type;
	typedef typename array2< T1, Allocator1 >::difference_type difference_type;

	array2< size_type > tmp;
	size_type i;

	tmp.resize( in.size1( ), in.size2( ) );
	tmp.reso1( in.reso1( ) );
	tmp.reso2( in.reso2( ) );

	// ���]�����摜���쐬����
	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		tmp[ i ] = in[ i ] == 0;
	}

	size_type label_num = mist::labeling4( tmp, tmp, max_label );

	if ( include_corner_labels )
	{
		size_type *L = new size_type[ label_num + 1 ];

		for( size_type i = 0 ; i <= label_num ; i++ )
		{
			L[ i ] = static_cast< size_type >( i );
		}

		L[ tmp( 0, 0 ) ] = 1;
		L[ tmp( tmp.width() - 1, 0 ) ] = 1;
		L[ tmp( 0, tmp.height() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, tmp.height() - 1 ) ] = 1;
		
		for( size_type i = 0 ; i < tmp.size( ) ; i++ )
		{
			tmp[ i ] = L[ tmp[ i ] ];
		}

		delete [] L;
	}

	// �w�肳�ꂽ�͈͓��̍ő僉�x����T��
	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		menseki[ tmp[ i ] ]++;
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

	// �ő听�����c���C���]�����摜���o�͂���
	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = tmp[ i ] == max_label ? 0 : 1;
	}
}


/// @brief �摜��0/1�𔽓]�����Č����ߏ������s��
//! 
//! 2�����摜�ɑ΂���4�ߖT�^���x�����O��p���āC�����ߏ������s��
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
{
	remove_hole_region( in, out, false, max_label );
}


/// @brief �摜��0/1�𔽓]�����Č����ߏ������s��
//! 
//! 3�����摜�ɑ΂���4�ߖT�^���x�����O��p���āC�����ߏ������s��
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  include_corner_labels �c whether to also remove holes of the labels containing the corners of the image
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, const bool include_corner_labels, typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	typedef typename array2< T1, Allocator1 >::size_type size_type;
	typedef typename array2< T1, Allocator1 >::difference_type difference_type;

	array3< size_type > tmp;
	size_type i;

	tmp.resize( in.size1( ), in.size2( ), in.size3( ) );
	tmp.reso1( in.reso1( ) );
	tmp.reso2( in.reso2( ) );
	tmp.reso3( in.reso3( ) );

	// ���]�����摜���쐬����
	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		tmp[ i ] = in[ i ] == 0;
	}

	size_type label_num = mist::labeling6( tmp, tmp, max_label );

	if ( include_corner_labels )
	{
		size_type *L = new size_type[ label_num + 1 ];

		for( size_type i = 0 ; i <= label_num ; i++ )
		{
			L[ i ] = static_cast< size_type >( i );
		}

		L[ tmp( 0, 0, 0 ) ] = 1;
		L[ tmp( tmp.width() - 1, 0, 0 ) ] = 1;
		L[ tmp( 0, tmp.height() - 1, 0 ) ] = 1;
		L[ tmp( 0, 0, tmp.depth() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, tmp.height() - 1, 0 ) ] = 1;
		L[ tmp( 0, tmp.height() - 1, tmp.depth() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, 0, tmp.depth() - 1 ) ] = 1;
		L[ tmp( tmp.width() - 1, tmp.height() - 1, tmp.depth() - 1 ) ] = 1;
		
		for( size_type i = 0 ; i < tmp.size( ) ; i++ )
		{
			tmp[ i ] = L[ tmp[ i ] ];
		}

		delete [] L;
	}

	// �w�肳�ꂽ�͈͓��̍ő僉�x����T��
	size_type *menseki = new size_type[ label_num + 1 ];
	for( i = 0 ; i <= label_num ; i++ )
	{
		menseki[ i ] = 0;
	}

	for( i = 0 ; i < tmp.size( ) ; i++ )
	{
		menseki[ tmp[ i ] ]++;
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

	// �ő听�����c���C���]�����摜���o�͂���
	for( i = 0 ; i < out.size( ) ; i++ )
	{
		out[ i ] = tmp[ i ] == max_label ? 0 : 1;
	}
}

/// @brief �摜��0/1�𔽓]�����Č����ߏ������s��
//! 
//! 3�����摜�ɑ΂���4�ߖT�^���x�����O��p���āC�����ߏ������s��
//! 
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������삷��
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
template < class T1,  class T2, class Allocator1, class Allocator2 >
void remove_hole_region( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T1, Allocator1 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
{
	remove_hole_region( in, out, false, max_label );
}


namespace __he__
{
	template< class L >
	struct table_type
	{
		typedef typename array< L >::value_type label_type;

		label_type label;
		table_type *next;
		table_type *tail;

		table_type( ) : label( 0 ), next( NULL ), tail( NULL ){}
	};

	template< class L, class Allocator >
	inline void resolve( const typename array< L, Allocator >::value_type::label_type u, const typename array< L, Allocator >::value_type::label_type v, array< table_type< L >, Allocator > &table )
	{
		typedef typename array< L, Allocator >::value_type	table_type;
		typedef typename table_type::label_type				label_type;
		if( table[ u ].label != table[ v ].label )
		{
			table_type *pu;
			table_type *pv;
			if( table[ u ].label < table[ v ].label )
			{
				pu = &table[ table[ u ].label ];
				pv = &table[ table[ v ].label ];
			}
			else
			{
				pu = &table[ table[ v ].label ];
				pv = &table[ table[ u ].label ];
			}
			pu->tail->next = pv;
			pu->tail = pv->tail;
			while( pv )
			{
				pv->label = pu->label;
				pv = pv->next;
			}
		}
	}

	template< class L, class Allocator >
	inline void update( typename array< L, Allocator >::value_type::label_type &m, array< table_type< L >, Allocator > &table )
	{
		table[ m ].label = m;
		table[ m ].tail  = &table[ m ];
		m++;
	}
}


namespace he
{
	/// @brief He���2�����摜�ɑ΂���8�ߖT�^���x�����O
	//! 
	//! He���2�����摜�ɑ΂���8�ߖT�^���x�����O
	//! 
	//! @attention ���͂Əo�͈͂قȂ�摜�I�u�W�F�N�g�ł���K�v������
	//! 
	//! �o�͉摜�̃f�[�^�^�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
	//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
	//! ���̊֐��́C�ȉ��ɋ����镶���̃A���S���Y���������������̂ł��B
	//! �� ����, �� �F��, ��� ����, ���� ���m, �ɓ� �p��, "����2�񑖍����x���t���A���S���Y��," �d�q���ʐM�w��_����D, Vol.J91-D, No.4, pp.1016-1024, 2008.
	//! 
	//! @param[in]  in        �c ���͉摜
	//! @param[out] out       �c �o�͉摜
	//! 
	//! @return ���蓖�Ă�ꂽ���x����
	//! 
	template< typename T1, class Allocator1, typename T2, class Allocator2 >
	inline typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &b, array2< T2, Allocator2 > &v )
	{
		typedef typename array2< T1, Allocator1 >::size_type		size_type;
		typedef typename array2< T1, Allocator1 >::difference_type	difference_type;
		typedef typename array2< T1, Allocator1 >::value_type		value_type;
		typedef typename array2< T2, Allocator2 >::value_type		label_type;
		typedef typename array2< T1, Allocator1 >::const_pointer	ipointer;
		typedef typename array2< T2, Allocator2 >::pointer			opointer;

		typedef typename __he__::table_type< label_type >			table_type;

		const size_type size = ( ( b.width( ) + 1 ) / 2 ) * ( ( b.height( ) + 1 ) / 2 ); 
		array< table_type > table( size );
		label_type m = 1;

		v.resize( b.width( ), b.height( ) );

		ipointer ip  = &b( 0, 0 );
		opointer op1 = &v( 0, 0 );

		if( ip[ 0 ] != 0 )
		{
			op1[ 0 ] = m;
			__he__::update( m, table );
		}
		else
		{
			op1[ 0 ] = 0;
		}

		for( size_type i = 1 ; i < b.width( ) ; i++ )
		{
			if( ip[ i ] != 0 )
			{
				if( op1[ i - 1 ] != 0 )
				{
					op1[ i ] = op1[ i - 1 ];
				}
				else
				{
					op1[ i ] = m;
					__he__::update( m, table );
				}				
			}
			else
			{
				op1[ 0 ] = 0;
			}
		}

		// ��O�̃��C�����o����
		opointer op0 = op1;

		// 1���C�����ɐi�߂�
		ip  += b.width( );
		op1 += v.width( );

		for( size_type j = 1 ; j < b.height( ) ; j++ )
		{
			if( ip[ 0 ] != 0 )
			{
				if( op0[ 0 ] != 0 )
				{
					op1[ 0 ] = op0[ 0 ];
				}
				else if( op0[ 1 ] != 0 )
				{
					op1[ 0 ] = op0[ 1 ];
				}
				else
				{
					op1[ 0 ] = m;
					__he__::update( m, table );
				}				
			}
			else
			{
				op1[ 0 ] = 0;
			}

			size_type i = 1;
			for( ; i < b.width( ) - 1 ; i++ )
			{
				if( ip[ i ] != 0 )
				{
					if( op0[ i ] != 0 )
					{
						op1[ i ] = op0[ i ];
					}
					else if( op1[ i - 1 ] != 0 )
					{
						op1[ i ] = op1[ i - 1 ];
						if( op0[ i + 1 ] != 0 )
						{
							__he__::resolve( op1[ i - 1 ], op0[ i + 1 ], table );
						}
					}
					else if( op0[ i - 1 ] != 0 )
					{
						op1[ i ] = op0[ i - 1 ];
						if( op0[ i + 1 ] != 0 )
						{
							__he__::resolve( op0[ i - 1 ], op0[ i + 1 ], table );
						}
					}
					else if( op0[ i + 1 ] != 0 )
					{
						op1[ i ] = op0[ i + 1 ];
					}
					else
					{
						op1[ i ] = m;
						__he__::update( m, table );
					}
				}
				else
				{
					op1[ i ] = 0;
				}
			}

			if( ip[ i ] != 0 )
			{
				if( op0[ i ] != 0 )
				{
					op1[ i ] = op0[ i ];
				}
				else if( op1[ i - 1 ] != 0 )
				{
					op1[ i ] = op1[ i - 1 ];
				}
				else if( op0[ i - 1 ] != 0 )
				{
					op1[ i ] = op0[ i - 1 ];
				}
				else
				{
					op1[ i ] = m;
					__he__::update( m, table );
				}				
			}
			else
			{
				op1[ i ] = 0;
			}

			op0  = op1;
			ip  += b.width( );
			op1 += v.width( );
		}

		array< label_type > l_table( m );
		label_type l = 0;
		for( size_type i = 1 ; i < m ; i++ )
		{
			if( l_table[ table[ i ].label ] == 0 )
			{
				l++;
				l_table[ table[ i ].label ] = l;
			}
		}

		for( size_t i = 0 ; i < v.size( ) ; i++ )
		{
			v[ i ] = l_table[ table[ v[ i ] ].label ];
		}

		return( static_cast< size_type >( l ) );
	}

	/// @brief He���3�����摜�ɑ΂���26�ߖT�^���x�����O
	//! 
	//! He���3�����摜�ɑ΂���26�ߖT�^���x�����O
	//! 
	//! @attention ���͂Əo�͈͂قȂ�摜�I�u�W�F�N�g�ł���K�v������
	//! 
	//! �o�͉摜�̃f�[�^�^�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v�Ȃ��߁Cshort �� int �𗘗p���邱�Ƃ������߂��܂��D
	//! �����ŁC���x�����Ƃ͍ŏI�I�ɓ�����̈�̐��ł͂Ȃ��C��ƒ��ɔ�������ꎞ�I�ȃ��x�������w�����߁C�f�[�^�^�� char �̏ꍇ�ɂ̓I�[�o�[�t���[����\�������ɑ傫���Ȃ�܂��D
	//! ���̊֐��́C�ȉ��ɋ����镶���̃A���S���Y���������������̂ł��B
	//! �� ����, �� �F��, ��� ����, ���� ���m, �ɓ� �p��, "�O����2�l�摜�ɂ����鍂�����x���t���A���S���Y��," �d�q���ʐM�w��_����D, Vol.J92-D, No.12, pp.2261-2269, 2009.
	//! 
	//! @param[in]  in        �c ���͉摜
	//! @param[out] out       �c �o�͉摜
	//! 
	//! @return ���蓖�Ă�ꂽ���x����
	//! 
	template< typename T1, class Allocator1, typename T2, class Allocator2 >
	inline typename array3< T2, Allocator2 >::value_type labeling26( const array3< T1, Allocator1 > &b, array3< T2, Allocator2 > &l )
	{
		typedef typename array3< T1, Allocator1 >::size_type		size_type;
		typedef typename array3< T1, Allocator1 >::difference_type	difference_type;
		typedef typename array3< T1, Allocator1 >::value_type		value_type;
		typedef typename array3< T2, Allocator2 >::value_type		label_type;
		typedef typename array3< T1, Allocator1 >::const_pointer	ipointer;
		typedef typename array3< T2, Allocator2 >::pointer			opointer;

		typedef typename __he__::table_type< label_type >			table_type;

		const size_type size = ( ( b.width( ) + 1 ) / 2 ) * ( ( b.height( ) + 1 ) / 2 ) * ( ( b.depth( ) + 1 ) / 2 ); 
		array< table_type > table( size );
		l.resize( b.width( ), b.height( ), b.depth( ) );
		label_type label = 1;
		{// k = 0
			const size_t k = 0;
			{// j = 0
				const size_t j = 0;
				{// i = 0
					const size_t i = 0;
					if( b( i, j, k ) != 0 )
					{
						l( i, j, k ) = label;
						__he__::update( label, table );
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				for( size_t i = 1 ; i != b.width( ) ; ++ i )
				{
					const label_type &l1  = l( i - 1, j    , k );
					if( b( i, j, k ) != 0 )
					{
						if( l1 != 0 )
						{
							l( i, j, k ) = l1;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
			}
			for( size_t j = 1 ; j != b.height( ) ; ++ j )
			{
				{// i = 0
					const size_t i = 0;
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l4  = l( i + 1, j - 1, k );
					if( b( i, j, k ) != 0 )
					{
						if( l3 != 0 )
						{
							l( i, j, k ) = l3;
						}
						else if( l4 != 0 )
						{
							l( i, j, k ) = l4;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				for( size_t i = 1 ; i != b.width( ) - 1 ; ++ i )
				{
					const label_type &l1  = l( i - 1, j    , k );
					const label_type &l2  = l( i - 1, j - 1, k );
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l4  = l( i + 1, j - 1, k );
					if( b( i, j, k ) != 0 )
					{
						if( l3 != 0 )
						{
							l( i, j, k ) = l3;
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
							if( l4 != 0 )
							{
								__he__::resolve( l1, l4, table );
							}
						}
						else if( l2 != 0 )
						{
							l( i, j, k ) = l2;
							if( l4 != 0 )
							{
								__he__::resolve( l2, l4, table );
							}
						}
						else if( l4 != 0 )
						{
							l( i, j, k ) = l4;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}				
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				{// i = width - 1
					const size_t i = b.width( ) - 1;
					const label_type &l1 = l( i - 1, j,     k );
					const label_type &l2 = l( i - 1, j - 1, k );
					const label_type &l3 = l( i,     j - 1, k );
					if( b( i, j, k ) != 0 )
					{
						if( l3 != 0 )
						{
							l( i, j, k ) = l3;
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
						}
						else if( l2 != 0 )
						{
							l( i, j, k ) = l2;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}				
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
			}
		}
		for( size_t k = 1 ; k != b.depth( ) ; ++ k )
		{
			{// j = 0
				const size_t j = 0;
				{// i = 0
					const size_t i = 0;
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l10 = l( i + 1, j,     k - 1 );
					const label_type &l12 = l( i,     j + 1, k - 1 );
					const label_type &l13 = l( i + 1, j + 1, k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l10 != 0 )
						{
							l( i, j, k ) = l10;
						}
						else if( l12 != 0 )
						{
							l( i, j, k ) = l12;
						}
						else if( l13 != 0 )
						{
							l( i, j, k ) = l13;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				for( size_t i = 1 ; i != b.width( ) - 1 ; ++ i )
				{
					const label_type &l1  = l( i - 1, j    , k );
					const label_type &l8  = l( i - 1, j,     k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l10 = l( i + 1, j,     k - 1 );
					const label_type &l11 = l( i - 1, j + 1, k - 1 );
					const label_type &l12 = l( i,     j + 1, k - 1 );
					const label_type &l13 = l( i + 1, j + 1, k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
							if( l10 != 0 && l12 == 0 )
							{
								__he__::resolve( l1, l10, table );
							}
							else if( l13 != 0 && l12 == 0 )
							{
								__he__::resolve( l1, l13, table );
							}
						}
						else if( l8 != 0 )
						{
							l( i, j, k ) = l8;
							if( l10 != 0 && l12 == 0 )
							{
								__he__::resolve( l8, l10, table );
							}
							else if( l13 != 0 && l12 == 0 )
							{
								__he__::resolve( l8, l13, table );
							}
						}
						else if( l10 != 0 )
						{
							l( i, j, k ) = l10;
							if( l11 != 0 && l12 == 0 )
							{
								__he__::resolve( l10, l11, table );
							}
						}
						else if( l12 != 0 )
						{
							l( i, j, k ) = l12;
						}
						else if( l11 != 0 )
						{
							l( i, j, k ) = l11;
							if( l13 != 0 )
							{
								__he__::resolve( l11, l13, table );
							}
						}
						else if( l13 != 0 )
						{
							l( i, j, k ) = l13;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				{// i = width - 1
					const size_t i = b.width( ) - 1;
					const label_type &l1  = l( i - 1, j    , k );
					const label_type &l8  = l( i - 1, j,     k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l11 = l( i - 1, j + 1, k - 1 );
					const label_type &l12 = l( i,     j + 1, k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
						}
						else if( l8 != 0 )
						{
							l( i, j, k ) = l8;
						}
						else if( l12 != 0 )
						{
							l( i, j, k ) = l12;
						}
						else if( l11 != 0 )
						{
							l( i, j, k ) = l11;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
			}
			for( size_t j = 1 ; j != b.height( ) - 1 ; ++ j )
			{
				{// i = 0
					const size_t i = 0;
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l4  = l( i + 1, j - 1, k );
					const label_type &l6  = l( i,     j - 1, k - 1 );
					const label_type &l7  = l( i + 1, j - 1, k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l10 = l( i + 1, j,     k - 1 );
					const label_type &l12 = l( i,     j + 1, k - 1 );
					const label_type &l13 = l( i + 1, j + 1, k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l3 != 0 )
						{
							l( i, j, k ) = l3;
							if( l12 != 0 && l10 == 0 )
							{
								__he__::resolve( l3, l12, table );
							}
							else
							{
								if( l13 != 0 && l10 == 0 )
								{
									__he__::resolve( l3, l13, table );
								}
							}
						}
						else if( l6 != 0 )
						{
							l( i, j, k ) = l6;
							if( l12 != 0 && l10 == 0 )
							{
								__he__::resolve( l6, l12, table );
							}
							else
							{
								if( l13 != 0 && l10 == 0 )
								{
									__he__::resolve( l6, l13, table );
								}
							}
						}
						else if( l10 != 0 )
						{
							l( i, j, k ) = l10;
						}
						else if( l12 != 0 )
						{
							l( i, j, k ) = l12;
							if( l4 != 0 )
							{
								__he__::resolve( l12, l4, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l12, l7, table );
							}
						}
						else if( l4 != 0 )
						{
							l( i, j, k ) = l4;
							if( l13 != 0 )
							{
								__he__::resolve( l4, l13, table );
							}
						}
						else if( l7 != 0 )
						{
							l( i, j, k ) = l7;
							if( l13 != 0 )
							{
								__he__::resolve( l7, l13, table );
							}
						}
						else if( l13 != 0 )
						{
							l( i, j, k ) = l13;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				for( size_t i = 1 ; i != b.width( ) - 1 ; ++ i )
				{
					const label_type &l1  = l( i - 1, j    , k );
					const label_type &l2  = l( i - 1, j - 1, k );
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l4  = l( i + 1, j - 1, k );
					const label_type &l5  = l( i - 1, j - 1, k - 1 );
					const label_type &l6  = l( i,     j - 1, k - 1 );
					const label_type &l7  = l( i + 1, j - 1, k - 1 );
					const label_type &l8  = l( i - 1, j,     k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l10 = l( i + 1, j,     k - 1 );
					const label_type &l11 = l( i - 1, j + 1, k - 1 );
					const label_type &l12 = l( i,     j + 1, k - 1 );
					const label_type &l13 = l( i + 1, j + 1, k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l3 != 0 )
						{
							l( i, j, k ) = l3;
							if( l12 != 0 && l8 == 0 && l10 == 0 )
							{
								__he__::resolve( l3, l12, table );
							}
							else
							{
								if( l11 != 0 && l8 == 0 )
								{
									__he__::resolve( l3, l11, table );
								}
								if( l13 != 0 && l10 == 0 )
								{
									__he__::resolve( l3, l13, table );
								}
							}
						}
						else if( l6 != 0 )
						{
							l( i, j, k ) = l6;
							if( l12 != 0 && l8 == 0 && l10 == 0 )
							{
								__he__::resolve( l6, l12, table );
							}
							else
							{
								if( l11 != 0 && l8 == 0 )
								{
									__he__::resolve( l6, l11, table );
								}
								if( l13 != 0 && l10 == 0 )
								{
									__he__::resolve( l6, l13, table );
								}
							}
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
							if( l10 != 0 && l12 == 0 )
							{
								__he__::resolve( l1, l10, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l1, l7, table );
								if( l13 != 0 )
								{
									__he__::resolve( l1, l13, table );
								}
							}
							else if( l4 != 0 )
							{
								__he__::resolve( l1, l4, table );
								if( l13 != 0 )
								{
									__he__::resolve( l1, l13, table );
								}
							}
							else if( l13 != 0 && l12 == 0 )
							{
								__he__::resolve( l1, l13, table );
							}
						}
						else if( l8 != 0 )
						{
							l( i, j, k ) = l8;
							if( l10 != 0 && l12 == 0 )
							{
								__he__::resolve( l8, l10, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l8, l7, table );
								if( l13 != 0 )
								{
									__he__::resolve( l8, l13, table );
								}
							}
							else if( l4 != 0 )
							{
								__he__::resolve( l8, l4, table );
								if( l13 != 0 )
								{
									__he__::resolve( l8, l13, table );
								}
							}
							else if( l13 != 0 && l12 == 0 )
							{
								__he__::resolve( l8, l13, table );
							}
						}
						else if( l10 != 0 )
						{
							l( i, j, k ) = l10;
							if( l11 != 0 && l12 == 0 )
							{
								__he__::resolve( l10, l11, table );
							}
							if( l5 != 0 )
							{
								__he__::resolve( l10, l5, table );
							}
							else if( l2 != 0 )
							{
								__he__::resolve( l10, l2, table );
							}
						}
						else if( l12 != 0 )
						{
							l( i, j, k ) = l12;
							if( l4 != 0 )
							{
								__he__::resolve( l12, l4, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l12, l7, table );
							}
							if( l2 != 0 )
							{
								__he__::resolve( l12, l2, table );
							}
							else if( l5 != 0 )
							{
								__he__::resolve( l12, l5, table );
							}
						}
						else if( l5 != 0 )
						{
							l( i, j, k ) = l5;
							if( l4 != 0 )
							{
								__he__::resolve( l5, l4, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l5, l7, table );
							}
							if( l11 != 0 )
							{
								__he__::resolve( l5, l11, table );	
							}
							if( l13 != 0 )
							{
								__he__::resolve( l5, l13, table );
							}
						}
						else if( l2 != 0 )
						{
							l( i, j, k ) = l2;
							if( l4 != 0 )
							{
								__he__::resolve( l2, l4, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l2, l7, table );
							}
							if( l11 != 0 )
							{
								__he__::resolve( l2, l11, table );
							}
							if( l13 != 0 )
							{
								__he__::resolve( l2, l13, table );
							}
						}
						else if( l4 != 0 )
						{
							l( i, j, k ) = l4;
							if( l11 != 0 )
							{
								__he__::resolve( l4, l11, table );
							}
							if( l13 != 0 )
							{
								__he__::resolve( l4, l13, table );
							}
						}
						else if( l7 != 0 )
						{
							l( i, j, k ) = l7;
							if( l11 != 0 )
							{
								__he__::resolve( l7, l11, table );
							}
							if( l13 != 0 )
							{
								__he__::resolve( l7, l13, table );
							}
						}
						else if( l11 != 0 )
						{
							l( i, j, k ) = l11;
							if( l13 != 0 )
							{
								__he__::resolve( l11, l13, table );
							}
						}
						else if( l13 != 0 )
						{
							l( i, j, k ) = l13;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				{// i = width - 1
					const size_t i = b.width( ) - 1;
					const label_type &l1  = l( i - 1, j    , k );
					const label_type &l2  = l( i - 1, j - 1, k );
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l5  = l( i - 1, j - 1, k - 1 );
					const label_type &l6  = l( i,     j - 1, k - 1 );
					const label_type &l8  = l( i - 1, j,     k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l11 = l( i - 1, j + 1, k - 1 );
					const label_type &l12 = l( i,     j + 1, k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l3 != 0 )
						{
							l( i, j, k ) = l3;
							if( l12 != 0 && l8 == 0 )
							{
								__he__::resolve( l3, l12, table );
							}
							else
							{
								if( l11 != 0 && l8 == 0 )
								{
									__he__::resolve( l3, l11, table );
								}
							}
						}
						else if( l6 != 0 )
						{
							l( i, j, k ) = l6;
							if( l12 != 0 && l8 == 0 )
							{
								__he__::resolve( l6, l12, table );
							}
							else
							{
								if( l11 != 0 && l8 == 0 )
								{
									__he__::resolve( l6, l11, table );
								}
							}
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
						}
						else if( l8 != 0 )
						{
							l( i, j, k ) = l8;
						}
						else if( l12 != 0 )
						{
							l( i, j, k ) = l12;
							if( l2 != 0 )
							{
								__he__::resolve( l12, l2, table );
							}
							else if( l5 != 0 )
							{
								__he__::resolve( l12, l5, table );
							}
						}
						else if( l5 != 0 )
						{
							l( i, j, k ) = l5;
							if( l11 != 0 )
							{
								__he__::resolve( l5, l11, table );	
							}
						}
						else if( l2 != 0 )
						{
							l( i, j, k ) = l2;
							if( l11 != 0 )
							{
								__he__::resolve( l2, l11, table );
							}
						}
						else if( l11 != 0 )
						{
							l( i, j, k ) = l11;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
			}
			{// j = height - 1
				const size_t j = b.height( ) - 1;
				{// i = 0
					const size_t i = 0;
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l4  = l( i + 1, j - 1, k );
					const label_type &l6  = l( i,     j - 1, k - 1 );
					const label_type &l7  = l( i + 1, j - 1, k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l10 = l( i + 1, j,     k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l3 != 0 )
						{
							l( i, j, k ) = l3;
						}
						else if( l6 != 0 )
						{
							l( i, j, k ) = l6;
						}
						else if( l10 != 0 )
						{
							l( i, j, k ) = l10;
						}
						else if( l4 != 0 )
						{
							l( i, j, k ) = l4;
						}
						else if( l7 != 0 )
						{
							l( i, j, k ) = l7;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				for( size_t i = 1 ; i != b.width( ) - 1 ; ++ i )
				{
					const label_type &l1  = l( i - 1, j    , k );
					const label_type &l2  = l( i - 1, j - 1, k );
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l4  = l( i + 1, j - 1, k );
					const label_type &l5  = l( i - 1, j - 1, k - 1 );
					const label_type &l6  = l( i,     j - 1, k - 1 );
					const label_type &l7  = l( i + 1, j - 1, k - 1 );
					const label_type &l8  = l( i - 1, j,     k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					const label_type &l10 = l( i + 1, j,     k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l3 != 0 )
						{
							l( i, j, k ) = l3;
						}
						else if( l6 != 0 )
						{
							l( i, j, k ) = l6;
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
							if( l10 != 0 )
							{
								__he__::resolve( l1, l10, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l1, l7, table );
							}
							else if( l4 != 0 )
							{
								__he__::resolve( l1, l4, table );
							}
						}
						else if( l8 != 0 )
						{
							l( i, j, k ) = l8;
							if( l10 != 0 )
							{
								__he__::resolve( l8, l10, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l8, l7, table );
							}
							else if( l4 != 0 )
							{
								__he__::resolve( l8, l4, table );
							}
						}
						else if( l10 != 0 )
						{
							l( i, j, k ) = l10;
							if( l5 != 0 )
							{
								__he__::resolve( l10, l5, table );
							}
							else if( l2 != 0 )
							{
								__he__::resolve( l10, l2, table );
							}
						}
						else if( l5 != 0 )
						{
							l( i, j, k ) = l5;
							if( l4 != 0 )
							{
								__he__::resolve( l5, l4, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l5, l7, table );
							}
						}
						else if( l2 != 0 )
						{
							l( i, j, k ) = l2;
							if( l4 != 0 )
							{
								__he__::resolve( l2, l4, table );
							}
							else if( l7 != 0 )
							{
								__he__::resolve( l2, l7, table );
							}
						}
						else if( l4 != 0 )
						{
							l( i, j, k ) = l4;
						}
						else if( l7 != 0 )
						{
							l( i, j, k ) = l7;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
				{// i = width - 1
					const size_t i = b.width( ) - 1;
					const label_type &l1  = l( i - 1, j    , k );
					const label_type &l2  = l( i - 1, j - 1, k );
					const label_type &l3  = l( i,     j - 1, k );
					const label_type &l5  = l( i - 1, j - 1, k - 1 );
					const label_type &l6  = l( i,     j - 1, k - 1 );
					const label_type &l8  = l( i - 1, j,     k - 1 );
					const label_type &l9  = l( i,     j,     k - 1 );
					if( b( i, j, k ) != 0 )
					{
						if( l9 != 0 )
						{
							l( i, j, k ) = l9;
						}
						else if( l3 != 0 )
						{
							l( i, j, k ) = l3;
						}
						else if( l6 != 0 )
						{
							l( i, j, k ) = l6;
						}
						else if( l1 != 0 )
						{
							l( i, j, k ) = l1;
						}
						else if( l8 != 0 )
						{
							l( i, j, k ) = l8;
						}
						else if( l5 != 0 )
						{
							l( i, j, k ) = l5;
						}
						else if( l2 != 0 )
						{
							l( i, j, k ) = l2;
						}
						else
						{
							l( i, j, k ) = label;
							__he__::update( label, table );
						}
					}
					else
					{
						l( i, j, k ) = 0;
					}
				}
			}
		}
		mist::array< label_type > l_table( label );
		label_type ret = 0;
		for( size_t i = 1 ; i != label ; ++ i )
		{
			if( l_table[ table[ i ].label ] == 0 )
			{
				ret ++;
				l_table[ table[ i ].label ] = ret;
			}
		}
		for( size_t i = 0 ; i != l.size( ) ; ++ i )
		{
			l[ i ] = l_table[ table[ l[ i ] ].label ];
		}
		return ret;
	}
}


/// @}
//  ���x�����O�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_LABELING__
