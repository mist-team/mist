/// @file mist/filter/interlace.h
//!
//! @brief �C���^�[���X�̉������s�����߂̃��C�u����
//!

#ifndef __INCLUDE_MIST_INTERLACE__
#define __INCLUDE_MIST_INTERLACE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// �J���[�摜�̐ݒ��ǂݍ���
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __interlace_controller__
{
	template < bool b >
	struct interlace_controller
	{
		template < class T, class Allocator >
		static bool interlace_odd( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2�s�ُ�Ȃ��ꍇ�̓C���^�[���X�����͂ł��Ȃ�
				return( false );
			}

			out = in;

			size_type i, j;

			// ����C�����Ԃ��邽�߁C�擪�s�����̍s�ŕ⊮����
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				out( i, 0 ) = in( i, 1 );
			}
			// �e��s��O��̋����s�Ő��`�ɕ⊮���Ă��߂�
			for( j = 2 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = static_cast< value_type >( ( in( i, j - 1 ) + in( i, j + 1 ) ) / 2.0 );
				}
			}
			// �ŏI�s����̂��߁C1�s�O�ŕ⊮����
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2�s�ُ�Ȃ��ꍇ�̓C���^�[���X�����͂ł��Ȃ�
				return( false );
			}

			out = in;

			size_type i, j;

			// �e�����s��O��̊�s�Ő��`�ɕ⊮���Ă��߂�
			for( j = 1 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = static_cast< value_type >( ( in( i, j - 1 ) + in( i, j + 1 ) ) / 2.0 );
				}
			}
			// �ŏI�s�������̂��߁C1�s�O�ŕ⊮����
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}
	};

	template < >
	struct interlace_controller< true >
	{
		template < class T, class Allocator >
		static bool interlace_odd( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2�s�ُ�Ȃ��ꍇ�̓C���^�[���X�����͂ł��Ȃ�
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			// ����C�����Ԃ��邽�߁C�擪�s�����̍s�ŕ⊮����
			for( i = 0 ; i < in.width( ) ; i++ )
			{
				out( i, 0 ) = in( i, 1 );
			}
			// �e��s��O��̋����s�Ő��`�ɕ⊮���Ă��߂�
			for( j = 2 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					r = ( in( i, j - 1 ).r + in( i, j + 1 ).r ) / 2.0;
					g = ( in( i, j - 1 ).g + in( i, j + 1 ).g ) / 2.0;
					b = ( in( i, j - 1 ).b + in( i, j + 1 ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}
			}
			// �ŏI�s����̂��߁C1�s�O�ŕ⊮����
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}

		template < class T, class Allocator >
		static bool interlace_even( const array2< T, Allocator > &in, array2< T, Allocator > &out )
		{
			typedef typename array2< T, Allocator >::size_type size_type;
			typedef typename array2< T, Allocator >::value_type color_type;
			typedef typename color_type::value_type value_type;

			if( in.height( ) < 2 )
			{
				// 2�s�ُ�Ȃ��ꍇ�̓C���^�[���X�����͂ł��Ȃ�
				return( false );
			}

			out = in;

			size_type i, j;
			double r, g, b;

			for( j = 1 ; j < in.height( ) - 1 ; j += 2 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					r = ( in( i, j - 1 ).r + in( i, j + 1 ).r ) / 2.0;
					g = ( in( i, j - 1 ).g + in( i, j + 1 ).g ) / 2.0;
					b = ( in( i, j - 1 ).b + in( i, j + 1 ).b ) / 2.0;
					out( i, j ) = color_type( static_cast< value_type >( r ), static_cast< value_type >( g ), static_cast< value_type >( b ) );
				}
			}
			if( j == in.height( ) - 1 )
			{
				for( i = 0 ; i < in.width( ) ; i++ )
				{
					out( i, j ) = in( i, j - 1 );
				}
			}
			return( true );
		}
	};
}


//! @addtogroup interlace_group �C���^�[���X����
//!  @{


/// @brief �֐��E�N���X�̊T�v������
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] in  �c �����̐���
//! @param[in] out �c �����̐���
//! @return        �c �߂�l�̐���
//! 
template < class T, class Allocator >
bool interlace( const array2< T, Allocator > &in, array2< T, Allocator > &out, bool is_odd_line = false )
{
	if( is_odd_line )
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_odd( in, out ) );
	}
	else
	{
		return( __interlace_controller__::interlace_controller< is_color< T >::value >::interlace_even( in, out ) );
	}
}



/// @}
//  �C���^�[���X�����O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_INTERLACE__
