/// @file mist/filter/linear.h
//!
//! @brief �e�����̉摜�ɑΉ��������`�t�B���^���v�Z���邽�߂̃��C�u����
//!
//!�����Œ�`�������`�t�B���^�p�J�[�l�����e�����̉摜�ɓK�p���܂��D
//!�܂��C3�~3�K�E�V�A���t�B���^�C3�~3���v���V�A���t�B���^���e�����ŗp�ӂ��Ă���܂��D
//!
//!������Βl�t�B���^�̂悤�Ɍ��ʂ��Βl�ŏo�͂���t�B���^�������ꍇ�ɂ́C
//!_MIST_LINEAR_FILTER_RETURN_ABSOLUTE_VALUE_ 
//!�Ƃ����}�N���� mist/filter/linear.h �̃C���N���[�h����ɒ�`���ĉ������D
//!
//!�֐����̌v�Z�͂��ׂ� double �^�ōs���܂��D
//!�ŏI�I�Ɍ��ʂ��o�͗p�̔z��Ɋi�[����Ƃ��̃f�[�^�^�̃L���X�g�́C
//!static_cast< out_value_type > ��p���܂����C�I�[�o�[�t���[��}���������ꍇ�́C
//!_MIST_LINEAR_FILTER_USE_SAFE_CAST_
//!�Ƃ����}�N���� mist/filter/linear.h �̃C���N���[�h����ɒ�`���ĉ������D
//!�f�[�^�^�̏���l�E�����l�𒲂ׁC����𒴂���ꍇ�́C����l�E�����l�ɒu�������܂��D
//!

#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#include <cmath>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

namespace __linear_filter__
{
	//�I�[�o�[�t���[�}��
	template< class T >
	struct type_limits_2 : public type_limits < T >
	{
		static T cast( const double& arg )
		{
			if( ( arg >= minimum( ) ) && ( arg <= maximum( ) ) )
			{
				if( arg >= 0 )
				{
					return( static_cast< T >( arg + 0.5 ) );
				}
				else
				{
					return( static_cast< T >( arg - 0.5 ) );
				}
			}
			else
			if( arg < minimum( ) )
			{
				return( minimum( ) );
			}
			else //if( arg > maximum( ) )
			{
				return( maximum( ) );
			}
		}
	};

	template < >
	struct type_limits_2< float > : public type_limits < float >
	{
		static float cast( const float& arg )
		{
			if( ( arg >= -maximum( ) ) && ( arg <= maximum( ) ) )
			{
				return( static_cast< float >( arg ) );
			}
			if( arg < -maximum( ) )
			{
				return( -maximum( ) );
			}
			if( arg > maximum( ) )
			{
				return( maximum( ) );
			}
		}
	};

	template< >
	struct type_limits_2< double >
	{
		static double cast( const double& arg )
		{
			return( arg );
		}
	};

	template< >
	struct type_limits_2< long double >
	{
		static long double cast( const double& arg )
		{
			return( arg );
		}
	};

	//double�^����C�ӂ̌^�ւ̕ϊ��i�I�[�o�[�t���[�̗}���j
	template < class T >
	inline T safe_cast( const double& arg )
	{
		return( type_limits_2< T >::cast( arg )  );
	}

	//��ʒ[�̗v�f�ɑ΂��鉉�Z
	template < class T_out >
	struct edge_element
	{
		template < class T_in, class Allocator_in >
		static T_out copy_value( const array< T_in, Allocator_in >& in, typename array< T_in, Allocator_in >::size_type i )
		{
			#ifdef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( safe_cast< T_out >( in[ i ] ) );
			#endif

			#ifndef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( static_cast< T_out >( in[ i ] ) );
			#endif
		}
	}; 

	//��ʒ[�ȊO�̊e�v�f�ɑ΂��鉉�Z
	template < class T_out >
	struct non_edge_element
	{
		template < class T_in, class Allocator_in, class T_kernel, class Allocator_kernel >
		static T_out get_value( const array< T_in, Allocator_in >& in, const typename array< T_in, Allocator_in >::difference_type *pindex, const array< T_kernel, Allocator_kernel >& kernel, const typename array< T_in, Allocator_in >::size_type ii )
		{
			double	val = 0;

			typedef typename array< T_kernel, Allocator_kernel >::size_type size_type;
		
			typename array< T_in, Allocator_in >::const_pointer p = &in[ ii ];
			for( size_type i = 0 ; i < kernel.size( ) ; i++ )
			{
				val += p[ pindex[ i ] ] * kernel[ i ];
			}

			#ifdef _MIST_LINEAR_FILTER_RETURN_ABSOLUTE_VALUE_
			val = fabs( val );
			#endif

			#ifdef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( safe_cast< T_out >( val ) );
			#endif

			#ifndef _MIST_LINEAR_FILTER_USE_SAFE_CAST_
			return( static_cast< T_out >( val ) );
			#endif
		}
	};

	//�J�[�l���K�p
	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out, const array< T_kernel, Allocator_kernel >& kernel,
					const typename array< T_kernel, Allocator_kernel >::size_type kernel_center )
	{
		out.resize( in.size( ) );

		typedef typename array< T_in, Allocator_in >::size_type size_type;
		typedef typename array< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel_center;
		const size_type e_i = in.size( ) - ( kernel.size( ) - kernel_center ) + 1;  

		size_type i, count = 0;
		const_pointer p = &in( in.size( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
				pindex[ count++ ] = &in( in.size( ) / 2 + i - kernel_center ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			 out( i ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, i );
		}

		//�摜�[�̏���
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}
		for( i = e_i ; i < in.size( ) ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out, const array1< T_kernel, Allocator_kernel >& kernel,
					const typename array< T_kernel, Allocator_kernel >::size_type kernel_center_i )
	{
		out.resize( in.size1( ) );
		out.reso1( in.reso1( ) );

		typedef typename array1< T_in, Allocator_in >::size_type size_type;
		typedef typename array1< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array1< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel_center_i;
		const size_type e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;

		typedef array1< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type i, count = 0;
		const_pointer p = &in( in.width( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
				pindex[ count++ ] = &in( in.width( ) / 2 + i - kernel_center_i ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			out( i ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, i );	
		}

		//�摜�[�̏���
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}
		for( i = e_i ; i < in.size1( ) ; i++ )
		{
			out( i ) = edge_element< out_value_type >::copy_value( in, i );	
		}		
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out, const array2< T_kernel, Allocator_kernel >& kernel,
					const typename array2< T_kernel, Allocator_kernel >::size_type kernel_center_i, const typename array2< T_kernel, Allocator_kernel >::size_type kernel_center_j ) 
	{	
		std::cout << "apply2" << std::endl;
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		typedef typename array2< T_in, Allocator_in >::size_type size_type;
		typedef typename array2< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array2< T_out, Allocator_out >::value_type out_value_type;

		const size_type s_i = kernel_center_i;
		const size_type e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type s_j = kernel_center_j;
		const size_type e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;

		typedef array2< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type i, j, count = 0;
		const_pointer p = &in( in.width( ) / 2, in.height( ) / 2 );
		for( j = 0 ; j < kernel.size2( ) ; j++ )
		{
			for( i = 0 ; i < kernel.size1( ) ; i++ )
			{
				pindex[ count++ ] = &in( in.width( ) / 2 + i - kernel_center_i, in.height( ) / 2 + j - kernel_center_j ) - p;
			}
		}

		for( j = s_j ; j < e_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, j * in.size1( ) + i );
			}
		}

		delete [] pindex;

		//�摜�[�̏���
		for( j = 0 ; j < in.size2( ) ; j++ )
		{
			for( i = 0 ; i < s_i ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
			for( i = e_i ; i < in.size1( ) ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
		}
		for( j = 0 ; j < s_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
		}
		for( j = e_j ; j < in.size2( ) ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = edge_element< out_value_type >::copy_value( in, j * in.size1( ) + i );
			}
		}
	}

	template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
	void apply( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out, const array3< T_kernel, Allocator_kernel >& kernel,
					const typename array3< T_kernel, Allocator_kernel >::size_type kernel_center_i, const typename array3< T_kernel, Allocator_kernel >::size_type kernel_center_j, const typename array3< T_kernel, Allocator_kernel >::size_type kernel_center_k )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
		
		typedef typename array3< T_in, Allocator_in >::size_type size_type;
		typedef typename array3< T_in, Allocator_in >::difference_type difference_type;
		typedef typename array3< T_out, Allocator_out >::value_type out_value_type;
		
		const size_type s_i = kernel_center_i;
		const size_type e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type s_j = kernel_center_j;
		const size_type e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;
		const size_type s_k = kernel_center_k;
		const size_type e_k = in.size3( ) - ( kernel.size3( ) - kernel_center_k ) + 1;

		typedef array3< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type i, j, k, count = 0;
		const_pointer p = &in( in.width( ) / 2, in.height( ) / 2, in.depth( ) / 2 );
		for( k = 0 ; k < kernel.size3( ) ; k++ )
		{
			for( j = 0 ; j < kernel.size2( ) ; j++ )
			{
				for( i = 0 ; i < kernel.size1( ) ; i++ )
				{
					pindex[ count++ ] = &in( in.width( ) / 2 + i - kernel_center_i, in.height( ) / 2 + j - kernel_center_j, in.depth( ) / 2 + k - kernel_center_k ) - p;
				}
			}
		}

		for( k = s_k ; k < e_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = non_edge_element< out_value_type >::get_value( in, pindex, kernel, k * in.size1() *in.size2() + j *in.size1() + i );
				}
			}
		}

		delete [] pindex;


		//�摜�[�̏���
		for( k = 0 ; k < in.size3( ) ; k++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				for( i = 0 ; i < s_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
				for( i = e_i ; i < in.size1( ) ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
			for( j = 0 ; j < s_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
			for( j = e_j ; j < in.size2( ) ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
		}
		for( k = 0 ; k < s_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
		}
		for( k = e_k ; k < in.size3() ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = edge_element< out_value_type >::copy_value( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i );
				}
			}
		}
	}

	//�J�[�l���̐��K��
	template < class T, class Allocator >
	void normalize( array< T, Allocator >& kernel )
	{
		typedef typename array< T, Allocator >::size_type size_type;
		typedef typename array< T, Allocator >::value_type value_type;
		
		double norm = 0;

		size_type i;
		for( i = 0 ; i < kernel.size( ) ; i++ )
		{
			norm += kernel[ i ];
		}
		for( i = 0 ; i < kernel.size( ) ; i++ )
		{
			kernel[ i ] =  static_cast< value_type >( kernel[ i ] / norm );
		}
	}

}

//! @addtogroup linear_group ���`�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/linear.h>
//! @endcode
//!
//!  @{


/// @brief �����Œ�`����kernel��p�������`�t�B���^(array�̏ꍇ)
//! 
//! �\�߁C���`�t�B���^�p�̃J�[�l���z��������Œ�`���Ă����C�����p�����t�B���^�������s���D
//! 
//! @code �g�p��(5�~5��l�d�ݕ������t�B���^)
//! mist::array2< unsigned char > in, out;
//! mist::array2< double > kernel( 5, 5 );
//!
//! ��l�d�ݕ������t�B���^�p�̃J�[�l�����쐬
//! kernel.fill( 1.0 / kernel.size( ) ); 
//!
//! �K���ȓ��̓f�[�^
//! mist::read_bmp( in, "hoge.bmp" ); 
//!
//! �t�B���^�̓K�p
//! mist::linear_filter( in, out, kernel );
//! @endcode
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  kernel �c �J�[�l���z��
//! @param[in]  kernel_center �c �J�[�l���̒��S�ʒu(�����n���Ȃ��ꍇ�� kernel.size( ) / 2 ������)
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out, const array< T_kernel, Allocator_kernel >& kernel, int kernel_center = -1 )
{
	if( kernel_center == -1 )
	{
		kernel_center = kernel.size( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center );
}

/// @brief �����Œ�`����kernel��p�������`�t�B���^(array1�̏ꍇ)
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  kernel �c �J�[�l���z��
//! @param[in]  kernel_center_i �c �J�[�l���̒��S�ʒu(�����n���Ȃ��ꍇ�� kernel.width( ) / 2 ������)
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out, const array1< T_kernel, Allocator_kernel > kernel, int kernel_center_i = -1 )
 {
	if( kernel_center_i == -1 )
	{
		kernel_center_i = kernel.size1( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center_i );
}

/// @brief �����Œ�`����kernel��p�������`�t�B���^(array2�̏ꍇ)
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  kernel �c �J�[�l���z��
//! @param[in]  kernel_center_i �c �J�[�l���̒��S�ʒu(�����n���Ȃ��ꍇ�� kernel.width( ) / 2 ������)
//! @param[in]  kernel_center_j �c �J�[�l���̒��S�ʒu(�����n���Ȃ��ꍇ�� kernel.height( ) / 2 ������)
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out, const array2< T_kernel, Allocator_kernel >& kernel, int kernel_center_i = -1, int kernel_center_j = -1 )
{
	if( kernel_center_i == -1 )
	{
		kernel_center_i = kernel.size1( ) / 2;
	}
	if( kernel_center_j == -1 )
	{
		kernel_center_j = kernel.size2( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center_i, kernel_center_j );
}

/// @brief �����Œ�`����kernel��p�������`�t�B���^(array3�̏ꍇ)
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  kernel �c �J�[�l���z��
//! @param[in]  kernel_center_i �c �J�[�l���̒��S�ʒu(�����n���Ȃ��ꍇ�� kernel.width( ) / 2 ������)
//! @param[in]  kernel_center_j �c �J�[�l���̒��S�ʒu(�����n���Ȃ��ꍇ�� kernel.height( ) / 2 ������)
//! @param[in]  kernel_center_k �c �J�[�l���̒��S�ʒu(�����n���Ȃ��ꍇ�� kernel.depth( ) / 2 ������)
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
void linear_filter( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out, const array3< T_kernel, Allocator_kernel >& kernel, 
						int kernel_center_i = -1, int kernel_center_j = -1, int kernel_center_k = -1 )
{
	if( kernel_center_i == -1 )
	{
		kernel_center_i = kernel.size1( ) / 2;
	}
	if( kernel_center_j == -1 )
	{
		kernel_center_j = kernel.size2( ) / 2;
	}
	if( kernel_center_k == -1 )
	{
		kernel_center_k = kernel.size3( ) / 2;
	}

	__linear_filter__::apply( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k );
}

/// @brief �K�E�V�A��(array)
//! 
//! kernel�T�C�Y3��1�����K�E�V�A��
//!
//! @param[in]  in  �c ���͔z��
//! @param[out] out �c �o�͔z��
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out )
{
	array< double > kernel( 3 );

	kernel( 0 ) = 0.60653066;
	kernel( 1 ) = 1.0;
	kernel( 2 ) = 0.60653066;

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief �K�E�V�A��(array1)
//! 
//! kernel�T�C�Y3��1�����K�E�V�A��
//!
//! @param[in]  in  �c ���͔z��
//! @param[out] out �c �o�͔z��
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out )
{
	array1< double > kernel( 3 );

	kernel( 0 ) = 0.60653066;
	kernel( 1 ) = 1.0;
	kernel( 2 ) = 0.60653066;

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief �K�E�V�A��(array2)
//! 
//! kernel�T�C�Y3�~3��2�����K�E�V�A��
//!
//! @param[in]  in  �c ���͔z��
//! @param[out] out �c �o�͔z��
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out )
{
	array2< double > kernel( 3, 3 );

	kernel( 0, 0 ) = 0.367879441;
	kernel( 1, 0 ) = 0.60653066;
	kernel( 2, 0 ) = 0.367879441;
	kernel( 0, 1 ) = 0.60653066;
	kernel( 1, 1 ) = 1.0;
	kernel( 2, 1 ) = 0.60653066;
	kernel( 0, 2 ) = 0.367879441;
	kernel( 1, 2 ) = 0.60653066;
	kernel( 2, 2 ) = 0.367879441;

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1, 1 );
}


/// @brief �K�E�V�A��(array3)
//! 
//! kernel�T�C�Y3�~3�~3��3�����K�E�V�A��
//!
//! @param[in]  in  �c ���͔z��
//! @param[out] out �c �o�͔z��
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void gaussian( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out )
{
	array3< double > kernel( 3, 3, 3 );

	kernel( 0, 0, 0 ) = 0.22313016;
	kernel( 1, 0, 0 ) = 0.367879441;
	kernel( 2, 0, 0 ) = 0.22313016;
	kernel( 0, 1, 0 ) = 0.367879441;
	kernel( 1, 1, 0 ) = 0.60653066;
	kernel( 2, 1, 0 ) = 0.367879441;
	kernel( 0, 2, 0 ) = 0.22313016;
	kernel( 1, 2, 0 ) = 0.367879441;
	kernel( 2, 2, 0 ) = 0.22313016;

	kernel( 0, 0, 1 ) = 0.367879441;
	kernel( 1, 0, 1 ) = 0.60653066;
	kernel( 2, 0, 1 ) = 0.367879441;
	kernel( 0, 1, 1 ) = 0.60653066;
	kernel( 1, 1, 1 ) = 1.0;
	kernel( 2, 1, 1 ) = 0.60653066;
	kernel( 0, 2, 1 ) = 0.367879441;
	kernel( 1, 2, 1 ) = 0.60653066;
	kernel( 2, 2, 1 ) = 0.367879441;

	kernel( 0, 0, 2 ) = 0.22313016;
	kernel( 1, 0, 2 ) = 0.367879441;
	kernel( 2, 0, 2 ) = 0.22313016;
	kernel( 0, 1, 2 ) = 0.367879441;
	kernel( 1, 1, 2 ) = 0.60653066;
	kernel( 2, 1, 2 ) = 0.367879441;
	kernel( 0, 2, 2 ) = 0.22313016;
	kernel( 1, 2, 2 ) = 0.367879441;
	kernel( 2, 2, 2 ) = 0.22313016;

	__linear_filter__::normalize( kernel );
	__linear_filter__::apply( in, out, kernel, 1, 1, 1 );
}


/// @brief ���v���V�A��(array)
//! 
//! kernel�T�C�Y3��1�������v���V�A��
//!
//! �o�͔z��̗v�f�^�� unsigned �Ƃ���ꍇ�͒��ӂ��K�v�D
//! ��Βl�o�̓}�N����I�[�o�[�t���[�}���}�N��
//! _MIST_LINEAR_FILTER_RETURN_ABSOLUTE_VALUE_
//! _MIST_LINEAR_FILTER_USE_SAFE_CAST_
//! �𕹗p���Ȃ��ꍇ�C�z�肷��o�͂������Ȃ��ꍇ������܂��D
//! 
//! @param[in]  in  �c �����̐���
//! @param[out] out �c �����̐���
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array< T_in, Allocator_in >& in, array< T_out, Allocator_out >& out )
{
	array< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief ���v���V�A��(array1)
//! 
//! kernel�T�C�Y3��1�������v���V�A��
//!
//! �o�͔z��̗v�f�^�� unsigned �Ƃ���ꍇ�͒��ӂ��K�v�D
//! 
//! @param[in]  in  �c �����̐���
//! @param[out] out �c �����̐���
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array1< T_in, Allocator_in >& in, array1< T_out, Allocator_out >& out )
{
	array1< double > kernel( 3 );

	kernel( 0 ) = 1.0;
	kernel( 1 ) = -2.0;
	kernel( 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel, 1 );
}


/// @brief ���v���V�A��(array2)
//! 
//! kernel�T�C�Y3�~3��2�������v���V�A��
//!
//! �o�͔z��̗v�f�^�� unsigned �Ƃ���ꍇ�͒��ӂ��K�v�D
//! 
//! @param[in]  in  �c �����̐���
//! @param[out] out �c �����̐���
//! 
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array2< T_in, Allocator_in >& in, array2< T_out, Allocator_out >& out )
{
	array2< double > kernel( 3, 3 );

	kernel( 0, 0 ) = 1.0;
	kernel( 1, 0 ) = 1.0;
	kernel( 2, 0 ) = 1.0;
	kernel( 0, 1 ) = 1.0;
	kernel( 1, 1 ) = -8.0;
	kernel( 2, 1 ) = 1.0;
	kernel( 0, 2 ) = 1.0;
	kernel( 1, 2 ) = 1.0;
	kernel( 2, 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel, 1, 1 );
}


/// @brief ���v���V�A��(array3)
//! 
//! kernel�T�C�Y3�~3�~3��3�������v���V�A��
//!
//! �o�͔z��̗v�f�^�� unsigned �Ƃ���ꍇ�͒��ӂ��K�v�D
//! 
//! @param[in]  in  �c �����̐���
//! @param[out] out �c �����̐���
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out >
void laplacian( const array3< T_in, Allocator_in >& in, array3< T_out, Allocator_out >& out )
{
	array3< double > kernel( 3, 3, 3 );

	kernel( 0, 0, 0 ) = 1.0;
	kernel( 1, 0, 0 ) = 1.0;
	kernel( 2, 0, 0 ) = 1.0;
	kernel( 0, 1, 0 ) = 1.0;
	kernel( 1, 1, 0 ) = 1.0;
	kernel( 2, 1, 0 ) = 1.0;
	kernel( 0, 2, 0 ) = 1.0;
	kernel( 1, 2, 0 ) = 1.0;
	kernel( 2, 2, 0 ) = 1.0;

	kernel( 0, 0, 1 ) = 1.0;
	kernel( 1, 0, 1 ) = 1.0;
	kernel( 2, 0, 1 ) = 1.0;
	kernel( 0, 1, 1 ) = 1.0;
	kernel( 1, 1, 1 ) = -26.0;
	kernel( 2, 1, 1 ) = 1.0;
	kernel( 0, 2, 1 ) = 1.0;
	kernel( 1, 2, 1 ) = 1.0;
	kernel( 2, 2, 1 ) = 1.0;

	kernel( 0, 0, 2 ) = 1.0;
	kernel( 1, 0, 2 ) = 1.0;
	kernel( 2, 0, 2 ) = 1.0;
	kernel( 0, 1, 2 ) = 1.0;
	kernel( 1, 1, 2 ) = 1.0;
	kernel( 2, 1, 2 ) = 1.0;
	kernel( 0, 2, 2 ) = 1.0;
	kernel( 1, 2, 2 ) = 1.0;
	kernel( 2, 2, 2 ) = 1.0;

	__linear_filter__::apply( in, out, kernel, 1, 1, 1 );
}	

/// @}
//  ���`�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
