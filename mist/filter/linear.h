/// @file mist/filter/linear.h
//!
//! @brief �e�����̉摜�ɑΉ��������`�t�B���^���v�Z���邽�߂̃��C�u����
//!

#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "../config/type_trait.h"
#endif

#include <cmath>
#include <functional>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __linear_filter__
{
	
	//////
	////// �z��̒[�ȊO�̊e�v�f�ɑ΂���v�Z
	//////
	template < class Calc_type, class Out_type, class Array_in, class Array_kernel, class Post_func >
	inline Out_type  calc_non_edge( 
										const Array_in								&in, 
										const typename Array_in::difference_type	*pindex, 
										const Array_kernel							&kernel, 
										const typename Array_in::size_type			ii,
										Post_func									p_func )
	{
		typedef typename Array_in::size_type size_type;
		
		Calc_type  ret = Calc_type( );
		typename Array_in::const_pointer p = &in[ ii ];
		for( size_type i = 0 ; i < kernel.size( ) ; i++ )
		{
			ret += p[ pindex[ i ] ] * kernel[ i ];
		}

		return ( p_func( ret ) );
	}		


	//////
	////// �z��̒[�̗v�f�ɑ΂���v�Z
	//////
	template < class Out_type, class Array_in, class Post_func >
	inline Out_type  calc_edge( 
									const Array_in						&in, 
									const typename Array_in::size_type	i,
									Post_func							p_func )
	{
		return ( p_func( in[ i ]  ) );
	}
	
	
	//////
	////// �J�[�l���K�p
	//////

	// �J�[�l���K�p ( array )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	inline void  apply( 
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							const array< T_kernel, Allocator_kernel >	&kernel,
							const unsigned int							kernel_center,
							const Post_func								p_func )
	{
		out.resize( in.size( ) );

		typedef typename array< T_in, Allocator_in >::size_type			size_type;
		typedef typename array< T_in, Allocator_in >::difference_type	difference_type;
		typedef typename array< T_out, Allocator_out >::value_type		out_type;
		typedef typename Calc_type										calc_type;

		const  size_type s_i = kernel_center;
		const  size_type e_i = in.size( ) - ( kernel.size( ) - kernel_center ) + 1;  

		typedef array< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type		i, count = 0;
		const_pointer	p = &in( in.size( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
			pindex[ count++ ] = &in( in.size( ) / 2 + i - kernel_center ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			out( i ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, i, p_func );
		}

		// �[�̏���
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}
		for( i = e_i ; i < in.size( ) ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}		
	}

	// �J�[�l���K�p ( array1 )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	inline void  apply(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							const array1< T_kernel, Allocator_kernel >	&kernel,
							const unsigned int							kernel_center_i,
							const Post_func								p_func )
	{
		out.resize( in.size1( ) );
		out.reso1( in.reso1( ) );

		typedef typename array1< T_in, Allocator_in >::size_type		size_type;
		typedef typename array1< T_in, Allocator_in >::difference_type	difference_type;
		typedef typename array1< T_out, Allocator_out >::value_type		out_type;
		typedef typename Calc_type										calc_type;

		const size_type  s_i = kernel_center_i;
		const size_type  e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;

		typedef array1< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type		i, count = 0;
		const_pointer	p = &in( in.width( ) / 2 );
		for( i = 0 ; i < kernel.size1( ) ; i++ )
		{
			pindex[ count++ ] = &in( in.size1( ) / 2 + i - kernel_center_i ) - p;
		}

		for( i = s_i ; i < e_i ; i++ )
		{
			out( i ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, i, p_func );	
		}

		// �[�̏���
		for( i = 0 ; i < s_i ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}
		for( i = e_i ; i < in.size1( ) ; i++ )
		{
			out( i ) = calc_edge< out_type >( in, i, p_func );	
		}		
	}

	// �J�[�l���K�p ( array2 )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	inline void apply( 
						const array2< T_in, Allocator_in >			&in, 
						array2< T_out, Allocator_out >				&out, 
						const array2< T_kernel, Allocator_kernel >	&kernel,
						const unsigned int							kernel_center_i,
						const unsigned int							kernel_center_j,
						Post_func									p_func ) 
	{
		out.resize( in.size1( ), in.size2( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );

		typedef typename array2< T_in, Allocator_in >::size_type			size_type;
		typedef typename array2< T_in, Allocator_in >::difference_type		difference_type;
		typedef typename array2< T_out, Allocator_out >::value_type			out_type;
		typedef typename Calc_type											calc_type;

		const size_type  s_i = kernel_center_i;
		const size_type  e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type  s_j = kernel_center_j;
		const size_type  e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;

		typedef array2< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type	   i, j, count = 0;
		const_pointer  p = &in( in.width( ) / 2, in.height( ) / 2 );
		for( j = 0 ; j < kernel.size2( ) ; j++ )
		{
			for( i = 0 ; i < kernel.size1( ) ; i++ )
			{
				pindex[ count++ ] = &in( in.size1( ) / 2 + i - kernel_center_i, in.size2( ) / 2 + j - kernel_center_j ) - p;
			}
		}

		for( j = s_j ; j < e_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, j * in.size1( ) + i, p_func );
			}
		}

		delete [] pindex;

		// �[�̏���
		for( j = 0 ; j < in.size2( ) ; j++ )
		{
			for( i = 0 ; i < s_i ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
			for( i = e_i ; i < in.size1( ) ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
		}
		for( j = 0 ; j < s_j ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
		}
		for( j = e_j ; j < in.size2( ) ; j++ )
		{
			for( i = s_i ; i < e_i ; i++ )
			{
				out( i, j ) = calc_edge< out_type >( in, j * in.size1( ) + i, p_func );
			}
		}
	}

	// �J�[�l���K�p ( array3 )
	template < class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel, class Post_func >
	void apply( 
					const array3< T_in, Allocator_in >			&in, 
					array3< T_out, Allocator_out >				&out, 
					const array3< T_kernel, Allocator_kernel >	&kernel,
					const unsigned int							kernel_center_i,
					const unsigned int							kernel_center_j,
					const unsigned int							kernel_center_k,
					const Post_func								p_func )
	{
		out.resize( in.size1( ), in.size2( ), in.size3( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
		
		typedef typename array3< T_in, Allocator_in >::size_type		size_type;
		typedef typename array3< T_in, Allocator_in >::difference_type	difference_type;
		typedef typename array3< T_out, Allocator_out >::value_type		out_type;
		typedef typename Calc_type										calc_type;
		
		const size_type  s_i = kernel_center_i;
		const size_type  e_i = in.size1( ) - ( kernel.size1( ) - kernel_center_i ) + 1;
		const size_type  s_j = kernel_center_j;
		const size_type  e_j = in.size2( ) - ( kernel.size2( ) - kernel_center_j ) + 1;
		const size_type  s_k = kernel_center_k;
		const size_type  e_k = in.size3( ) - ( kernel.size3( ) - kernel_center_k ) + 1;

		typedef array3< T_in, Allocator_in >::const_pointer const_pointer;
		difference_type *pindex = new difference_type[ kernel.size( ) ];

		size_type		i, j, k, count = 0;
		const_pointer	p = &in( in.width( ) / 2, in.height( ) / 2, in.depth( ) / 2 );
		for( k = 0 ; k < kernel.size3( ) ; k++ )
		{
			for( j = 0 ; j < kernel.size2( ) ; j++ )
			{
				for( i = 0 ; i < kernel.size1( ) ; i++ )
				{
					pindex[ count++ ] = &in( in.size1( ) / 2 + i - kernel_center_i, in.size2( ) / 2 + j - kernel_center_j, in.size3( ) / 2 + k - kernel_center_k ) - p;
				}
			}
		}

		for( k = s_k ; k < e_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_non_edge< calc_type, out_type >( in, pindex, kernel, k * in.size1( ) *in.size2( ) + j *in.size1( ) + i, p_func );
				}
			}
		}

		delete [] pindex;


		// �[�̏���
		for( k = 0 ; k < in.size3( ) ; k++ )
		{
			for( j = 0 ; j < in.size2( ) ; j++ )
			{
				for( i = 0 ; i < s_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
				for( i = e_i ; i < in.size1( ) ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
			for( j = 0 ; j < s_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
			for( j = e_j ; j < in.size2( ) ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
		}
		for( k = 0 ; k < s_k ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
		}
		for( k = e_k ; k < in.size3() ; k++ )
		{
			for( j = s_j ; j < e_j ; j++ )
			{
				for( i = s_i ; i < e_i ; i++ )
				{
					out( i, j, k ) = calc_edge< out_type >( in, k * in.size1( ) * in.size2( ) + j * in.size1( ) + i, p_func );
				}
			}
		}
	}


	//////
	////// �o�͔z��̗v�f�^���Z�p�^�Ȃ�C�����̌v�Z�� double �^�ōs���D����ȊO( mist::rgb< > �� )�̏ꍇ�́C�o�͔z��̗v�f�^�Ōv�Z����
	//////

	// �Z�p�^�Ȃ̂� double �^�Ōv�Z���� 
	template < bool Is_arithm = true >
	struct _is_arithm
	{
		template < class Out_type >
		struct _type
		{
			typedef double  calc_type;
		};
	};

	// �Z�p�^�ł͂Ȃ��̂ŏo�͔z��̗v�f�^�Ōv�Z����
	template <>
	struct _is_arithm< false >
	{
		template < class Out_type >
		struct _type
		{
			typedef Out_type  calc_type;
		};
	};


	//////
	////// �f�t�H���g�㏈���֐��I�u�W�F�N�g�𐶐�
	//////
	template < class Calc_type, class Out_type >
	struct default_func : public std::unary_function< Calc_type, Out_type > 
	{
		result_type  operator( )( argument_type  v )
		{
			return ( static_cast< result_type >( v ) );
		}
	};


	//////
	////// �w�肳�ꂽ�P���֐��I�u�W�F�N�g����㏈���֐��I�u�W�F�N�g�𐶐�
	//////

	// STL�� unary_function ���p�������֐��I�u�W�F�N�g���g�p����Ƃ�
	template < class Calc_type, class Out_type, class Unary_func >
	struct post_func1 : public std::unary_function< Calc_type, Out_type >
	{
		typedef typename Unary_func					unary_func;
		typedef typename unary_func::argument_type	unary_arg;
		
		unary_func  u_func_;

		post_func1( unary_func  u_func ) : u_func_( u_func )
		{
		}

		result_type  operator( )( argument_type  v )
		{
			return ( static_cast< result_type >( u_func_( static_cast< unary_arg >( v ) ) ) );
		}
	};

	// �ʏ�̊֐����g�p����Ƃ�
	template < class Calc_type, class Out_type, class Unary_arg, class Unary_res >
	struct post_func2 : public std::unary_function< Calc_type, Out_type >
	{
		typedef typename Unary_res	unary_func( Unary_arg );
		typedef typename Unary_arg	unary_arg;
		
		unary_func  *u_func_;

		post_func2( unary_func  *u_func ) : u_func_( u_func )
		{
		}

		result_type  operator( )( argument_type  v ) 
		{
			return ( static_cast< result_type >( ( *u_func_ )( static_cast< unary_arg >( v ) ) ) );
		}
	};


	//////
	////// �z��̐��K�� ( array, array1, array2, array3 )
	//////
	template < class Array >
	inline void normalize( Array  &ary )
	{
		typedef typename Array::size_type  size_type;
		typedef typename Array::value_type value_type;
		
		double  norm = 0;

		size_type  i;
		for( i = 0 ; i < ary.size( ) ; i++ )
		{
			norm += ary[ i ];
		}
		for( i = 0 ; i < ary.size( ) ; i++ )
		{
			ary[ i ] =  static_cast< value_type >( ary[ i ] / norm );
		}
	}


	//////
    ////// MIST���ŗ\�ߗp�ӂ��Ă���t�B���^�[�̃J�[�l�������̂��߂̎��ʎq
	//////
	struct filter_style
	{
		enum styles
		{
			user_defined,
			gaus,
			lapl
		};
	};


	//////
	////// MIST���ŗ\�ߗp�ӂ��Ă���t�B���^�[�Ɋւ���J�[�l�������֐�
	//////

	// �Y���Ȃ�
	template< filter_style::styles  Style = filter_style::user_defined >
	struct pre_defined_kernel
	{
	};

	// 3�~3�K�E�V�A���J�[�l���֐�
	template < >
	struct pre_defined_kernel< filter_style::gaus >
	{
		// gaussian ( array )
		inline static void  gen_func( array< double >  &kernel, unsigned int  &center ) 
		{	
			center = 1;

			kernel.resize( 3 );	

			kernel( 0 ) = 0.60653066;
			kernel( 1 ) = 1.0;
			kernel( 2 ) = 0.60653066;

			normalize( kernel );
		}

		// gaussian ( array1 )
		inline static void  gen_func( array1< double >  &kernel, unsigned int  &center_i )
		{
			center_i = 0;

			kernel.resize( 3 );	

			kernel( 0 ) = 0.60653066;
			kernel( 1 ) = 1.0;
			kernel( 2 ) = 0.60653066;

			normalize( kernel );
		}

		// gaussian ( array2 ) 
		inline static void  gen_func( array2< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j )
		{
			center_i = 1;
			center_j = 1;

			kernel.resize( 3, 3 );	

			kernel( 0, 0 ) = 0.367879441;
			kernel( 1, 0 ) = 0.60653066;
			kernel( 2, 0 ) = 0.367879441;
			kernel( 0, 1 ) = 0.60653066;
			kernel( 1, 1 ) = 1.0;
			kernel( 2, 1 ) = 0.60653066;
			kernel( 0, 2 ) = 0.367879441;
			kernel( 1, 2 ) = 0.60653066;
			kernel( 2, 2 ) = 0.367879441;	

			normalize( kernel );
		}

		// gaussian ( array3 )
		inline static void  gen_func( array3< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j, unsigned int  &center_k )
		{
			center_i = 1;
			center_j = 1;
			center_k = 1;

			kernel.resize( 3, 3, 3 );	

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

			normalize( kernel );
		}
	};

	// 3�~3���v���V�A���J�[�l���֐�
	template < >
	struct pre_defined_kernel< filter_style::lapl >
	{
		// laplacian ( array )
		inline static void  gen_func( array< double >  &kernel, unsigned int  &center )
		{
			center = 1;

			kernel.resize( 3 );

			kernel( 0 ) = 1.0;
			kernel( 1 ) = -2.0;
			kernel( 2 ) = 1.0;
		}

		// laplacian ( array1 ) 
		inline static void  gen_func( array1< double >  &kernel, unsigned int  &center_i )
		{
			center_i = 1;

			kernel.resize( 3 );		

			kernel( 0 ) = 1.0;
			kernel( 1 ) = -2.0;
			kernel( 2 ) = 1.0;
		}

		// laplacian ( array2 ) 
		inline static void  gen_func( array2< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j )
		{
			center_i = 1;
			center_j = 1;

			kernel.resize( 3, 3 );		

			kernel( 0, 0 ) = 1.0;
			kernel( 1, 0 ) = 1.0;
			kernel( 2, 0 ) = 1.0;
			kernel( 0, 1 ) = 1.0;
			kernel( 1, 1 ) = -8.0;
			kernel( 2, 1 ) = 1.0;
			kernel( 0, 2 ) = 1.0;
			kernel( 1, 2 ) = 1.0;
			kernel( 2, 2 ) = 1.0;	
		}

		// laplacian ( array3 ) 
		inline static void  gen_func( array3< double >  &kernel, unsigned int  &center_i, unsigned int  &center_j, unsigned int  &center_k  )
		{
			center_i = 1;
			center_j = 1;
			center_k = 1;

			kernel.resize( 3, 3, 3 );

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
		}
	};


	//////
	////// MIST���ŗ\�ߗp�ӂ��Ă���t�B���^�[�K�p
	//////

	// array
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array< T_in, Allocator_in >	&in, 
											array< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array< double >		kernel;
		unsigned int		kernel_center;	

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center );

		apply< Calc_type >( in, out, kernel, kernel_center, p_func );
	}

	// array1
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array1< T_in, Allocator_in >	&in, 
											array1< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array1< double >	kernel;
		unsigned int		kernel_center_i;

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center_i );

		apply< Calc_type >( in, out, kernel, kernel_center_i, p_func );
	}

	// array2
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array2< T_in, Allocator_in >	&in, 
											array2< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array2< double >	kernel;
		unsigned int		kernel_center_i;
		unsigned int		kernel_center_j;

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center_i, kernel_center_j );

		apply< Calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, p_func );
	}

	// array3
	template< filter_style::styles Filter_style, class Calc_type, class T_in, class Allocator_in, class T_out, class Allocator_out, class Post_func >
	inline void  apply_pre_defined_kernel( 
											const array3< T_in, Allocator_in >	&in, 
											array3< T_out, Allocator_out >		&out, 
											Post_func							p_func )
	{
		array3< double >	kernel;
		unsigned int		kernel_center_i;
		unsigned int		kernel_center_j;
		unsigned int		kernel_center_k;

		pre_defined_kernel< Filter_style >::gen_func( kernel, kernel_center_i, kernel_center_j, kernel_center_k );

		apply< Calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, p_func );
	}

}

//! @addtogroup linear_group ���`�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/linear.h>
//! @endcode
//!
//! �����Œ�`�������`�t�B���^�p�J�[�l�����e�����̉摜�ɓK�p���܂��D
//! 
//! @code �g�p��(5�~5��l�d�ݕ������t�B���^)
//! mist::array2< unsigned char > in, out;
//!
//! mist::read_bmp( in, "hoge.bmp" );  // �K���ȓ��̓f�[�^ 
//!
//! mist::array2< double > kernel( 5, 5 );
//! kernel.fill( 1.0 / kernel.size( ) );  // 5�~5��l�d�ݕ������t�B���^�p�̃J�[�l�����쐬
//!
//! mist::linear_filter( in, out, kernel );  // �t�B���^�̓K�p
//! @endcode
//!
//! �܂��C3�~3�K�E�V�A���t�B���^�C3�~3���v���V�A���t�B���^�� array, array1, array2, array3 �ŗp�ӂ��Ă���܂��D
//!
//! �����̌v�Z�́C�o�͔z��̗v�f�^���Z�p�^�̏ꍇ�� double �^�C
//! ����ȊO�i mist::rgb<> �� �j�̏ꍇ�͏o�͔z��̗v�f�^�ōs���܂��D
//!
//! �e�֐��̈����Ƃ��ēK���ȒP���֐�(��������̊֐�)�C
//! �܂���STL�̒P���֐��I�u�W�F�N�g( std::unary_function ����h���������֐��I�u�W�F�N�g)��n�����Ƃɂ��C
//! �v�f�̌v�Z���I�����ďo�͔z��Ɋi�[���钼�O�ɂ͂��ތ㏈�����w�肷�邱�Ƃ��ł��܂��D
//!
//! @code �㏈���֐��̗�1�F������Βl�t�B���^�̊e�v�f�̌v�Z�l�̐�Βl���o�͔z��Ɋi�[������
//! mist::laplacian( in, out, std::fabsl );
//! @endcode
//!
//! @code �㏈���֐��̗�2�F�t�B���^�̏o�͂� unsigned char �̃����W�ɔ[�߂��� 
//! unsigned char  func( const double  v )  // v �� unsigned char �ɐ؂�l�߂�P���֐�
//! {
//!     return ( ( v > 255 ) ? 255 : ( ( v < 0 ) ? 0 : static_cast< unsigned char >( val ) ) );
//! }
//!
//!     // ���� //
//! 
//! mist::array2< unsigned char >  in, out;
//! 
//!     // �K���� ���͔z�� in ���쐬 //
//!
//! mist::array2< double >  kernel;
//!
//!     // �K���� kernel ���쐬 //
//! 
//! mist::linear_filter( in, out, func, kernel );  // �P���֐� func �������Ƃ��ēn��
//! @endcode
//!
//! @code �㏈���֐��̗�3�F�K�E�V�A������������C2�l���iSTL�̊֐��I�u�W�F�N�g���g�p�j
//! #include <functional> // std::unary_function<> ��p���邽��
//! 
//! struct th_func : std::unary_function< double, unsigned char >
//!     // augument_type �� double �Cresult_type �� unsigned char �Ƃ���֐��I�u�W�F�N�g 
//! {
//!     argument_type  th_;  // 2�l��臒l
//!     
//!     th_func( const argument_type  th ) : th_( th )  // �R���X�g���N�^
//!     {
//!     }
//!     
//!     result_type  operator( )( const argument_type  v ) const  
//!         // v �� th_ ��菬������� 0 ���Cth_ �ȏ�Ȃ�� 255 ���o�͂���
//!     {
//!         return ( ( v < th_ ) ? 0 : 255 );
//!     }
//! };
//!
//!     // ���� //
//! 
//! mist::gaussian( in, out, th_func( 127.5 ) ); 
//!     // �e�v�f�ɑ΂���K�E�V�A���̌v�Z���ʂ�臒l 127.5 ��2�l���������ʂ��o�͔z��Ɋi�[
//! @endcode
//!
//!
//!  @{

/// @brief �����Œ�`����kernel��p�������`�t�B���^( array )
//! 
//! �\�߁C���`�t�B���^�p�̃J�[�l���z��������Œ�`���Ă����C�����p�����t�B���^�������s���D
//! �J�[�l���̒��S�ʒu�́C����( kernel.size( ) / 2 )�ɐݒ肳���
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  u_func �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel �c �J�[�l���z��
//!
// �ʏ�̊֐���n���ꍇ
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	const unsigned int	kernel_center = kernel.size( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// �֐��I�u�W�F�N�g��n���ꍇ
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	const unsigned int	kernel_center = kernel.size( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// �����n���Ȃ��ꍇ
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							const array< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	const unsigned int	kernel_center = kernel.size( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief �����Œ�`����kernel��p�������`�t�B���^( array1 )
//! 
//! �\�߁C���`�t�B���^�p�̃J�[�l���z��������Œ�`���Ă����C�����p�����t�B���^�������s���D
//! �J�[�l���̒��S�ʒu�́C����( kernel.width( ) / 2 )�ɐݒ肳���
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  u_func �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel �c �J�[�l���z��
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array1< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array1< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							const array1< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief �����Œ�`����kernel��p�������`�t�B���^( array2 )
//! 
//! �\�߁C���`�t�B���^�p�̃J�[�l���z��������Œ�`���Ă����C�����p�����t�B���^�������s���D
//! �J�[�l���̒��S�ʒu�́C����( kernel.width( ) / 2, kernel.height( ) / 2 )�ɐݒ肳���
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  u_func �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel �c �J�[�l���z��
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array2< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array2< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							const array2< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;

	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::default_func< calc_type, out_type >( ) );
}
	

/// @brief �����Œ�`����kernel��p�������`�t�B���^( array3 )
//! 
//! �\�߁C���`�t�B���^�p�̃J�[�l���z��������Œ�`���Ă����C�����p�����t�B���^�������s���D
//! �J�[�l���̒��S�ʒu�́C����( kernel.width( ) / 2, kernel.height( ) / 2, kernel.depth( ) / 2 )�ɐݒ肳���
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  u_func �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel �c �J�[�l���z��
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array3< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;
	const unsigned int	kernel_center_k = kernel.depth( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array3< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;
	const unsigned int	kernel_center_k = kernel.depth( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							const array3< T_kernel, Allocator_kernel >	&kernel )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	const unsigned int	kernel_center_i = kernel.width( ) / 2;
	const unsigned int	kernel_center_j = kernel.height( ) / 2;
	const unsigned int	kernel_center_k = kernel.depth( ) / 2;

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief �����Œ�`����kernel��p�������`�t�B���^( array : �J�[�l���̒��S�ʒu���w��\ )
//! 
//! @param[in]  in              �c ���͔z��
//! @param[out] out             �c �o�͔z��
//! @param[in]  u_func          �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel          �c �J�[�l���z��
//! @param[in]  kernel_center_i �c �J�[�l���̒��S�ʒu
//!
// �ʏ�̊֐���n���ꍇ
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	kernel_center = ( kernel_center < kernel.size( ) ) ? kernel_center : ( kernel.size( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// �֐��I�u�W�F�N�g��n���ꍇ
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	kernel_center = ( kernel_center < kernel.size( ) ) ? kernel_center : ( kernel.size( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// �����n���Ȃ��ꍇ
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array< T_in, Allocator_in >			&in, 
							array< T_out, Allocator_out >				&out, 
							const array< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center )
{
	typedef typename array< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	kernel_center = ( kernel_center < kernel.size( ) ) ? kernel_center : ( kernel.size( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief �����Œ�`����kernel��p�������`�t�B���^( array1 : �J�[�l���̒��S�ʒu���w��\ )
//! 
//! @param[in]  in              �c ���͔z��
//! @param[out] out             �c �o�͔z��
//! @param[in]  u_func          �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel          �c �J�[�l���z��
//! @param[in]  kernel_center_i �c �J�[�l���̒��S�ʒu
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array1< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	kernel_center_i = ( kernel_center_i < kernel.width( ) ) ? kernel_center_i : ( kernel.width( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array1< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																						unary_func;
 
	kernel_center_i = ( kernel_center_i < kernel.width( ) ) ? kernel_center_i : ( kernel.width( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array1< T_in, Allocator_in >			&in, 
							array1< T_out, Allocator_out >				&out, 
							const array1< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i )
{
	typedef typename array1< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	kernel_center_i = ( kernel_center_i < kernel.width( ) ) ? kernel_center_i : ( kernel.width( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief �����Œ�`����kernel��p�������`�t�B���^( array2 : �J�[�l���̒��S�ʒu���w��\ )
//! 
//! @param[in]  in              �c ���͔z��
//! @param[out] out             �c �o�͔z��
//! @param[in]  u_func          �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel          �c �J�[�l���z��
//! @param[in]  kernel_center_i �c �J�[�l���̒��S�ʒu( width ���� )
//! @param[in]  kernel_center_j �c �J�[�l���̒��S�ʒu( height ���� )
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array2< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array2< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array2< T_in, Allocator_in >			&in, 
							array2< T_out, Allocator_out >				&out, 
							const array2< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j )
{
	typedef typename array2< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, __linear_filter__::default_func< calc_type, out_type >( ) );
}
	

/// @brief �����Œ�`����kernel��p�������`�t�B���^( array3 : �J�[�l���̒��S�ʒu���w��\ )
//! 
//! @param[in]  in              �c ���͔z��
//! @param[out] out             �c �o�͔z��
//! @param[in]  u_func          �c �㏈���֐� ( �����Ă��ǂ� )
//! @param[in]  kernel          �c �J�[�l���z��
//! @param[in]  kernel_center_i �c �J�[�l���̒��S�ʒu( width ���� )
//! @param[in]  kernel_center_j �c �J�[�l���̒��S�ʒu( height ���� )
//! @param[in]  kernel_center_k �c �J�[�l���̒��S�ʒu( depth ���� )
//!
template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_arg, class Unary_res, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_res									u_func( Unary_arg ),
							const array3< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int							kernel_center_i,
							unsigned int							kernel_center_j, 
							unsigned int							kernel_center_k )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );
	kernel_center_k = ( kernel_center_k < kernel.depth( )  ) ? kernel_center_k : ( kernel.depth( )  - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class Unary_func, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							Unary_func									u_func,
							const array3< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int							kernel_center_i,
							unsigned int							kernel_center_j, 
							unsigned int							kernel_center_k )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	kernel_center_i = ( kernel_center_i < kernel.width( )  ) ? kernel_center_i : ( kernel.width( )  - 1 );
	kernel_center_j = ( kernel_center_j < kernel.height( ) ) ? kernel_center_j : ( kernel.height( ) - 1 );
	kernel_center_k = ( kernel_center_k < kernel.depth( )  ) ? kernel_center_k : ( kernel.depth( )  - 1 );

	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

template < class T_in, class Allocator_in, class T_out, class Allocator_out, class T_kernel, class Allocator_kernel >
inline void  linear_filter(
							const array3< T_in, Allocator_in >			&in, 
							array3< T_out, Allocator_out >				&out, 
							const array3< T_kernel, Allocator_kernel >	&kernel, 
							unsigned int								kernel_center_i,
							unsigned int								kernel_center_j,
							unsigned int								kernel_center_k )
{
	typedef typename array3< T_out, Allocator_out >::value_type															out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	__linear_filter__::apply< calc_type >( in, out, kernel, kernel_center_i, kernel_center_j, kernel_center_k, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief �K�E�V�A��( array, array1, array2, array3 )
//! 
//! kernel�T�C�Y3�~3�̃K�E�V�A��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  u_func �c �㏈���֐� ( �����Ă��ǂ� )
//! 
// �ʏ�̊֐���n���ꍇ
template < class Array_in, class Array_out, class Unary_arg, class Unary_res >
inline void  gaussian(
						const Array_in								&in,
						Array_out									&out,
						Unary_res									u_func( Unary_arg ) )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;
 
	__linear_filter__::apply_pre_defined_kernel< filter_style::gaus, calc_type >( in, out, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// �֐��I�u�W�F�N�g��n���ꍇ
template < class Array_in, class Array_out, class Unary_func >
inline void  gaussian(
						const Array_in								&in,
						Array_out									&out,
						Unary_func									u_func )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;
 
	__linear_filter__::apply_pre_defined_kernel< filter_style::gaus, calc_type >( in, out, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// �����n���Ȃ��ꍇ
template < class Array_in, class Array_out >
inline void  gaussian(
							const Array_in		&in,
							Array_out			&out )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
 
	__linear_filter__::apply_pre_defined_kernel< filter_style::gaus, calc_type >( in, out, __linear_filter__::default_func< calc_type, out_type >( ) );
}


/// @brief ���v���V�A��( array, array1, array2, array3 )
//! 
//! kernel�T�C�Y3�~3�̃��v���V�A��
//!
//! �o�͔z��̗v�f�^�� unsigned �Ƃ���ꍇ�͒��ӂ��K�v�D
//! �K�؂� u_func ���`���Ȃ��ꍇ�C�z�肷��o�͂������Ȃ��ꍇ������܂��D
//! 
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  u_func �c �㏈���֐� ( �����Ă��ǂ� )
//!
// �ʏ�̊֐���n���ꍇ
template < class Array_in, class Array_out, class Unary_arg, class Unary_res >
inline void  laplacian(
						const Array_in		&in,
						Array_out			&out,
						Unary_res			u_func( Unary_arg ) )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_arg																							unary_arg;
	typedef typename Unary_res																							unary_res;

	__linear_filter__::apply_pre_defined_kernel< filter_style::lapl, calc_type >( in, out, __linear_filter__::post_func2< calc_type, out_type, unary_arg, unary_res >( u_func ) );
}

// �֐��I�u�W�F�N�g��n���ꍇ
template < class Array_in, class Array_out, class Unary_func >
inline void  laplacian(
						const Array_in		&in,
						Array_out			&out,
						Unary_func			u_func )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;
	typedef typename Unary_func																							unary_func;

	__linear_filter__::apply_pre_defined_kernel< filter_style::lapl, calc_type >( in, out, __linear_filter__::post_func1< calc_type, out_type, unary_func >( u_func ) );
}

// �����n���Ȃ��ꍇ
template < class Array_in, class Array_out >
inline void  laplacian(
							const Array_in		&in,
							Array_out			&out )
{
	typedef typename __linear_filter__::filter_style																	filter_style;
	typedef typename Array_out::value_type																				out_type;
	typedef typename __linear_filter__::_is_arithm< is_arithmetic< out_type >::value >::_type< out_type >::calc_type	calc_type;

	__linear_filter__::apply_pre_defined_kernel< filter_style::lapl, calc_type >( in, out, __linear_filter__::default_func< calc_type, out_type >( ) );
}

/// @}
//  ���`�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
