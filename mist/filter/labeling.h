/// @file mist/filter/labeling.h
//!
//! @brief �e�����̉摜�ɑΉ������C���x�����O�A���S���Y��
//! 
//! @section �Q�l����
//! -# �đq�B�L, ����Ύ�, ���e����Y, �����W�v, "�O�����f�B�W�^����Ԃɂ�����}�`�̘A�����ƃI�C���[��," �d�q�ʐM�w��_����D, J65-D, No.1, pp.80-87, 1982
//! -# ���e����Y, "3�����f�B�W�^���摜����," ���W��, 2002
//!

#ifndef __INCLUDE_MIST_LABELING__
#define __INCLUDE_MIST_LABELING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


//! @addtogroup labeling_group ���x�����O
//!  @{


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in]  in        �c �����̐���
//! @param[out] out       �c �����̐���
//! @param[in]  max_label �c �����̐���
//! @return               �c �߂�l�̐���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling4( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 4 >( ) ) );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in]  in        �c �����̐���
//! @param[out] out       �c �����̐���
//! @param[in]  max_label �c �����̐���
//! @return               �c �߂�l�̐���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 8 >( ) ) );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in]  in        �c �����̐���
//! @param[out] out       �c �����̐���
//! @param[in]  max_label �c �����̐���
//! @return               �c �߂�l�̐���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array3< T2, Allocator2 >::size_type labeling6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 6 >( ) ) );
}


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in]  in        �c �����̐���
//! @param[out] out       �c �����̐���
//! @param[in]  max_label �c �����̐���
//! @return               �c �߂�l�̐���
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 26 >( ) ) );
}

/// @}
//  ���x�����O�O���[�v�̏I���
}


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

		template < class Array >
		static inline void neighbor( Array &in, const typename Array::size_type *T, typename Array::size_type *L,
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
		}
	};

	template < >
	struct neighbors< 18 >
	{
		_MIST_CONST( size_t, array_num, 9 );

		template < class Array >
		static inline void neighbor( Array &in, const typename Array::size_type *T, typename Array::size_type *L,
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
		}
	};

	template < >
	struct neighbors< 6 >
	{
		_MIST_CONST( size_t, array_num, 3 );

		template < class Array >
		static inline void neighbor( Array &in, const typename Array::size_type *T, typename Array::size_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[  0 ] =                           k > 0 ? T[ static_cast< size_type >( in( i    , j    , k - 1 ) ) ] : 0;
			L[  1 ] =              j     > 0          ? T[ static_cast< size_type >( in( i    , j - 1, k     ) ) ] : 0;
			L[  2 ] = i     > 0                       ? T[ static_cast< size_type >( in( i - 1, j    , k     ) ) ] : 0;
		}
	};

	template < >
	struct neighbors< 8 >
	{
		_MIST_CONST( size_t, array_num, 4 );

		template < class Array >
		static inline void neighbor( Array &in, const typename Array::size_type *T, typename Array::size_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[ 0 ] = i     > 0 && j > 0 ? T[ static_cast< size_type >( in( i - 1, j - 1 ) ) ] : 0;
			L[ 1 ] =              j > 0 ? T[ static_cast< size_type >( in( i    , j - 1 ) ) ] : 0;
			L[ 2 ] = i + 1 < w && j > 0 ? T[ static_cast< size_type >( in( i + 1, j - 1 ) ) ] : 0;
			L[ 3 ] = i     > 0          ? T[ static_cast< size_type >( in( i - 1, j     ) ) ] : 0;
		}
	};

	template < >
	struct neighbors< 4 >
	{
		_MIST_CONST( size_t, array_num, 2 );

		template < class Array >
		static inline void neighbor( Array &in, const typename Array::size_type *T, typename Array::size_type *L,
										const typename Array::size_type i, const typename Array::size_type j, const typename Array::size_type k,
										const typename Array::size_type w, const typename Array::size_type h, const typename Array::size_type d )
		{
			typedef typename Array::size_type size_type;

			L[ 0 ] = j > 0 ? T[ static_cast< size_type >( in( i    , j - 1 ) ) ] : 0;
			L[ 1 ] = i > 0 ? T[ static_cast< size_type >( in( i - 1, j     ) ) ] : 0;
		}
	};


	template < class Array, class neighbor >
	typename Array::size_type labeling( Array &in, const typename Array::size_type label_max, const neighbor dmy )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;

		size_type label_num = 0;
		size_type i, j, k, l, count;

		size_type *T = new size_type[ label_max * 4 + 1 ];
		size_type L[ neighbor::array_num ];
		const size_type width = in.width( );
		const size_type height = in.height( );
		const size_type depth = in.depth( );

		T[ 0 ] = T[ 1 ] = 0;

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
					size_type L1 = type_limits< value_type >::maximum( );
					for( l = 0, count = 0 ; l < neighbor::array_num ; l++ )
					{
						if( L[ l ] > 0 )
						{
							if( L1 > L[ l ] )
							{
								L1 = L[ l ];
							}
							count++;
						}
					}

					if( count == 0 )
					{
						// �ߖT�ɁC���łɊ��蓖�Ă��Ă��郉�x���͑��݂��Ȃ����߁C�V�K�Ƀ��x�������蓖�Ă�
						if( label_num < label_max )
						{
							label_num++;
						}
						T[ label_num ] = label_num;
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
										T[ m ] = L1;
									}
								}
							}
						}
					}
				}
			}
		}

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

		delete [] T;
		delete [] NT;

		return( label_num );
	}
}


//! @addtogroup labeling_group ���x�����O
//!  @{


/// @brief 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���4�ߖT�^���x�����O
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling4( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 4 >( ) ) );
}


/// @brief 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���8�ߖT�^���x�����O
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array2< T2, Allocator2 >::size_type labeling8( const array2< T1, Allocator1 > &in, array2< T2, Allocator2 > &out, typename array2< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num2< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 8 >( ) ) );
}


/// @brief 3�����摜�ɑ΂���6�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���6�ߖT�^���x�����O
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array3< T2, Allocator2 >::size_type labeling6( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 6 >( ) ) );
}


/// @brief 3�����摜�ɑ΂���18�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���18�ߖT�^���x�����O
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array3< T2, Allocator2 >::size_type labeling18( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 18 >( ) ) );
}


/// @brief 3�����摜�ɑ΂���26�ߖT�^���x�����O
//! 
//! 2�����摜�ɑ΂���26�ߖT�^���x�����O
//! @attention ���͂Əo�͂������摜�I�u�W�F�N�g�ł����������x�����O���邱�Ƃ��\�ł�
//! @attention �������C�f�[�^�^�� char �^�𗘗p����ꍇ�́C���x�������I�[�o�[�t���[���Ȃ��悤�ɒ��ӂ��K�v
//! 
//! @param[in]  in        �c ���͉摜
//! @param[out] out       �c �o�͉摜
//! @param[in]  max_label �c �ő�Ŋ��蓖�Ă郉�x����
//! 
//! @return ���蓖�Ă�ꂽ���x����
//! 
template < class T1, class T2, class Allocator1, class Allocator2 >
typename array3< T2, Allocator2 >::size_type labeling26( const array3< T1, Allocator1 > &in, array3< T2, Allocator2 > &out, typename array3< T2, Allocator2 >::size_type max_label = __labeling_controller__::default_label_num3< T2 >::value )
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
	return( __labeling_controller__::labeling( out, max_label, __labeling_controller__::neighbors< 26 >( ) ) );
}

/// @}
//  ���x�����O�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_LABELING__
