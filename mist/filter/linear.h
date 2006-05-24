// 
// Copyright (c) 2003-2006, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/filter/linear.h
//!
//! @brief ���`�t�B���^�̂��߂̃��C�u����
//!



#ifndef __INCLUDE_FILTER_LINEAR_FILTER_H__
#define __INCLUDE_FILTER_LINEAR_FILTER_H__



#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <algorithm>
#include <cmath>
#include <mist/config/color.h>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __linear__
{
	inline void pointer_differences( mist::array< int > &p_diff, const int o, const int inc )
	{
		for( size_t i = 0 ; i < p_diff.size( ) ; i ++ )
		{
			const int di = static_cast< int >( i );
			p_diff[ i ] = ( di - o ) * inc;
		}
	}

	inline void pointer_differences_2( mist::array2< int > &p_diff, const int o1, const int o2, const int inc1, const int inc2 )
	{
		for( size_t j = 0 ; j < p_diff.height( ) ; j ++ )
		{
			const int dj = static_cast< int >( j );
			for( size_t i = 0 ; i < p_diff.width( ) ; i ++ )
			{
				const int di = static_cast< int >( i );
				p_diff( i, j ) = ( dj - o2 ) * inc2 + ( di - o1 ) * inc1;
			}
		}
	}

	inline void pointer_differences_3( mist::array3< int > &p_diff, const int o1, const int o2, const int o3, const int inc1, const int inc2, const int inc3 )
	{
		for( size_t k = 0 ; k < p_diff.depth( ) ; k ++ )
		{
			const int dk = static_cast< int >( k );
			for( size_t j = 0 ; j < p_diff.height( ) ; j ++ )
			{
				const int dj = static_cast< int >( j );
				for( size_t i = 0 ; i < p_diff.width( ) ; i ++ )
				{
					const int di = static_cast< int >( i );
					p_diff( i, j, k ) = ( dk - o3 ) * inc3 + ( dj - o2 ) * inc2 + ( di - o1 ) * inc1;
				}
			}
		}
	}



	template< bool Is_color >
	struct calc_type
	{
		typedef double type;
	};

	template< >
	struct calc_type< true >
	{
		typedef mist::rgb< double > type;
	};


	
	template< class T > struct is_unsigned         { _MIST_CONST( bool, value, false ); };
	template<> struct is_unsigned< unsigned char > { _MIST_CONST( bool, value, true  ); };
	template<> struct is_unsigned< unsigned short >{ _MIST_CONST( bool, value, true  ); };
	template<> struct is_unsigned< unsigned int >  { _MIST_CONST( bool, value, true  ); };
	template<> struct is_unsigned< unsigned long > { _MIST_CONST( bool, value, true  ); };

	template< bool Is_unsigned >
	struct abs_type
	{
		static double func( double val )
		{
			return val; 
		}
	};

	template< >
	struct abs_type< true >
	{
		static double func( double val )
		{
			return std::fabs( val );
		}
	};


	template< typename Kernel_type >
	inline void kernel_culling( const mist::array< Kernel_type > &k1, const mist::array< int > &p1, mist::array< Kernel_type > &k2, mist::array< int > &p2 )
	{
		size_t size = 0;
		for( size_t i = 0 ; i < k1.size( ) ; i ++ )
		{
			if( k1[ i ] != 0 )
			{
				size ++;
			}
		}
		k2.resize( size );
		p2.resize( size );
		size_t m = 0;
		for( size_t i = 0 ; i < k1.size( ) ; i ++ )
		{
			if( k1[ i ] != 0 )
			{
				k2[ m ] = k1[ i ];
				p2[ m ] = p1[ i ];
				m ++;
			}
		}
	}


	template< typename Calc_type >
	struct filter_type
	{
		template< typename In_type, typename Out_type, typename Kernel_type >
		static void filtering( const mist::array< In_type > &in, mist::array< Out_type > &out, const size_t begin, const size_t end, const mist::array< Kernel_type > &kernel, const mist::array< int > &p_diff )
		{

			mist::array< Kernel_type > kernel2;
			mist::array< int > p_diff2;
			kernel_culling( kernel, p_diff, kernel2, p_diff2 );
			Calc_type val;
			for( size_t i = begin ; i < end ; i ++ )
			{
				val = Calc_type( );
				int di = static_cast< int >( i );
				for( size_t m = 0 ; m < kernel2.size( ) ; m ++ )
				{
					val += in[ static_cast< size_t >( di + p_diff2[ m ] ) ] * kernel2[ m ];
				}
				out[ i ] = static_cast< Out_type >( abs_type< is_unsigned< Out_type >::value >::func( val ) );
			}
		}
	};

	template<  >
	struct filter_type< mist::rgb< double > >
	{
		template< typename In_type, typename Out_type, typename Kernel_type >
		static void filtering( const mist::array< In_type > &in, mist::array< mist::rgb< Out_type > > &out, const size_t begin, const size_t end, const mist::array< Kernel_type > &kernel, const mist::array< int > &p_diff )
		{
			mist::array< Kernel_type > kernel2;
			mist::array< int > p_diff2;
			kernel_culling( kernel, p_diff, kernel2, p_diff2 );
			mist::rgb< double > val;
			for( size_t i = begin ; i < end ; i ++ )
			{
				val = mist::rgb< double >( );
				int di = static_cast< int >( i );
				for( size_t m = 0 ; m < kernel2.size( ) ; m ++ )
				{
					val += in[ static_cast< size_t >( di + p_diff2[ m ] ) ] * kernel2[ m ];
				}
				out[ i ].r = static_cast< Out_type >( abs_type< is_unsigned< Out_type >::value >::func( val.r ) );
				out[ i ].g = static_cast< Out_type >( abs_type< is_unsigned< Out_type >::value >::func( val.g ) );
				out[ i ].b = static_cast< Out_type >( abs_type< is_unsigned< Out_type >::value >::func( val.b ) );

			}
		}
	};	

	

	struct lapl_1
	{
		mist::array< double > a;
		lapl_1( )
		{
			a.resize( 3 );
			a[ 0 ] = 1.0;  a[ 1 ] = -2.0;  a[ 2 ] = 1.0;
		}
	};

	struct lapl_2
	{
		mist::array2< double > a;
		lapl_2( )
		{
			a.resize( 3, 3 );
			a( 0, 0 ) = 1.0;  a( 1, 0 ) = 1.0;  a( 2, 0 ) = 1.0;
			a( 0, 1 ) = 1.0;  a( 1, 1 ) = -8.0;  a( 2, 1 ) = 1.0;
			a( 0, 2 ) = 1.0;  a( 1, 2 ) = 1.0;  a( 2, 2 ) = 1.0;
		}
	};

	struct lapl_3
	{
		mist::array3< double > a;
		lapl_3( )
		{
			a.resize( 3, 3, 3 );
			a( 0, 0, 0 ) = 1.0;  a( 1, 0, 0 ) = 1.0;  a( 2, 0, 0 ) = 1.0;
			a( 0, 1, 0 ) = 1.0;  a( 1, 1, 0 ) = 1.0;  a( 2, 1, 0 ) = 1.0;
			a( 0, 2, 0 ) = 1.0;  a( 1, 2, 0 ) = 1.0;  a( 2, 2, 0 ) = 1.0;
			a( 0, 0, 1 ) = 1.0;  a( 1, 0, 1 ) = 1.0;  a( 2, 0, 1 ) = 1.0;
			a( 0, 1, 1 ) = 1.0;  a( 1, 1, 1 ) = -26.0;  a( 2, 1, 1 ) = 1.0;
			a( 0, 2, 1 ) = 1.0;  a( 1, 2, 1 ) = 1.0;  a( 2, 2, 1 ) = 1.0;
			a( 0, 0, 2 ) = 1.0;  a( 1, 0, 2 ) = 1.0;  a( 2, 0, 2 ) = 1.0;
			a( 0, 1, 2 ) = 1.0;  a( 1, 1, 2 ) = 1.0;  a( 2, 1, 2 ) = 1.0;
			a( 0, 2, 2 ) = 1.0;  a( 1, 2, 2 ) = 1.0;  a( 2, 2, 2 ) = 1.0;
		}
	};
}



//! @addtogroup linear_group ���`�t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/linear.h>
//! @endcode
//!
//! �z��̒[�i�J�[�l�����͂ݏo���Ƃ���j�̗v�f�̌v�Z�͍s���܂���D
//!
//! �g�p�\�ȗv�f�^�́C�Z�p�^�� mist::rgb< �Z�p�^ > �Ɍ���܂��D
//! �����̌v�Z�́C�o�͔z��̗v�f�^���Z�p�^�̏ꍇ�� double �^�C
//! mist::rgb< �Z�p�^ > �̏ꍇ�� mist::rgb< double > �ōs���܂��D
//! �o�͂̌^��unsigned�̏ꍇ�́C�������ʂ̐�Βl��Ԃ��܂��D
//!
//! �����Œ�`�������`�t�B���^�p�J�[�l�����e�����̉摜�ɓK�p�ł��܂��D
//! 
//! @code �g�p��(5�~5��l�d�ݕ������t�B���^)
//! mist::array2< unsigned char > in, out;
//!
//! mist::read_bmp( in, "hoge.bmp" );  // �K���ȓ��͔z�� 
//!
//! mist::array2< double > kernel( 5, 5 ); // �K���ȃJ�[�l���z��
//! kernel.fill( 1.0 / kernel.size( ) );  // 5�~5��l�d�ݕ������t�B���^�p�̃J�[�l�����쐬
//!
//! mist::linear_filter( in, out, kernel, 2, 2 );  // �t�B���^�̓K�p�i�J�[�l���z��ƃJ�[�l�����S��n���܂��j
//! @endcode
//!
//! ���v���V�A���t�B���^�� array, array1, array2, array3 �ŗp�ӂ��Ă���܂��D
//!
//!  @{



/// @brief ��ʂ̐��`�t�B���^( array )
//! 
//! �J�[�l���z��Ƃ��̒��S�ʒu���w�肷��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  kernel �c�J�[�l���z��
//! @param[in]  offset �c�J�[�l�����S�ʒu
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array< In_value, In_alloc > &in,
						  array< Out_value, Out_alloc > &out,
						  const array< Kernel_value, Kernel_alloc > &kernel,
						  const size_t offset )
{
	typedef typename __linear__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	mist::array< int > p_diff( kernel.size( ) );
	__linear__::pointer_differences( p_diff, offset, 1 );
	out.resize( in.size( ) );
	const size_t begin = offset;
	const size_t end = in.size( ) - ( kernel.width( ) - offset - 1 );
	__linear__::filter_type< calc_type >::filtering( in, out, begin, end, kernel, p_diff );
}

/// @brief ��ʂ̐��`�t�B���^( array )
//! 
//! �J�[�l���z����w�肷��(���S�̓J�[�l���̃T�C�Y����v�Z)
//!
//! @param[in]  in       �c ���͔z��
//! @param[out] out      �c �o�͔z��
//! @param[in]  kernel   �c�J�[�l���z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array< In_value, In_alloc > &in,
						  array< Out_value, Out_alloc > &out,
						  const array< Kernel_value, Kernel_alloc > &kernel )
{
	linear_filter( in, out, kernel, kernel.size( ) / 2 );
}

/// @brief ��ʂ̐��`�t�B���^( array1 )
//! 
//! �J�[�l���z��Ƃ��̒��S�ʒu���w�肷��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  kernel �c�J�[�l���z��
//! @param[in]  offset �c�J�[�l�����S�ʒu
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array1< In_value, In_alloc > &in,
						  array1< Out_value, Out_alloc > &out,
						  const array1< Kernel_value, Kernel_alloc > &kernel,
						  const size_t offset )
{
	typedef typename __linear__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	mist::array< int > p_diff( kernel.size( ) );
	__linear__::pointer_differences( p_diff, offset, 1 );
	out.resize( in.size( ) );
	out.reso1( in.reso1( ) );
	const size_t begin = offset;
	const size_t end = in.size( ) - ( kernel.size( ) - offset - 1 );
	__linear__::filter_type< calc_type >::filtering( in, out, begin, end, kernel, p_diff );
}

/// @brief ��ʂ̐��`�t�B���^( array1 )
//! 
//! �J�[�l���z����w�肷��(���S�̓J�[�l���̃T�C�Y����v�Z)
//!
//! @param[in]  in       �c ���͔z��
//! @param[out] out      �c �o�͔z��
//! @param[in]  kernel   �c�J�[�l���z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array1< In_value, In_alloc > &in,
						  array1< Out_value, Out_alloc > &out,
						  const array1< Kernel_value, Kernel_alloc > &kernel )
{
	linear_filter( in, out, kernel, kernel.width( ) / 2 );
}

/// @brief ��ʂ̐��`�t�B���^( array2 )
//! 
//! �J�[�l���z��Ƃ��̒��S�ʒu���w�肷��
//!
//! @param[in]  in       �c ���͔z��
//! @param[out] out      �c �o�͔z��
//! @param[in]  kernel   �c�J�[�l���z��
//! @param[in]  offset_i �ci�����̃J�[�l�����S�ʒu
//! @param[in]  offset_j �cj�����̃J�[�l�����S�ʒu
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array2< In_value, In_alloc > &in,
						  array2< Out_value, Out_alloc > &out,
						  const array2< Kernel_value, Kernel_alloc > &kernel,
						  const size_t offset_i,
						  const size_t offset_j )
{
	typedef typename __linear__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	mist::array2< int > p_diff( kernel.width( ), kernel.height( ) );
	__linear__::pointer_differences_2( p_diff, offset_i, offset_j, 1, in.width( ) );
	out.resize( in.width( ), in.height( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	const size_t begin = offset_j * in.width( ) + offset_i;
	const size_t end = in.size( ) - ( ( kernel.height( ) - offset_j - 1 ) * in.width( ) + kernel.width( ) - offset_i - 1 );
	__linear__::filter_type< calc_type >::filtering( in, out, begin, end, kernel, p_diff );
}

/// @brief ��ʂ̐��`�t�B���^( array2 )
//! 
//! �J�[�l���z����w�肷��(���S�̓J�[�l���̃T�C�Y����v�Z)
//!
//! @param[in]  in       �c ���͔z��
//! @param[out] out      �c �o�͔z��
//! @param[in]  kernel   �c�J�[�l���z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array2< In_value, In_alloc > &in,
						  array2< Out_value, Out_alloc > &out,
						  const array2< Kernel_value, Kernel_alloc > &kernel )
{
	linear_filter( in, out, kernel, kernel.width( ) / 2, kernel.height( ) / 2 );
}

/// @brief ��ʂ̐��`�t�B���^( array3 )
//! 
//! �J�[�l���z��Ƃ��̒��S�ʒu���w�肷��
//!
//! @param[in]  in       �c ���͔z��
//! @param[out] out      �c �o�͔z��
//! @param[in]  kernel   �c�J�[�l���z��
//! @param[in]  offset_i �ci�����̃J�[�l�����S�ʒu
//! @param[in]  offset_j �cj�����̃J�[�l�����S�ʒu
//! @param[in]  offset_k �ck�����̃J�[�l�����S�ʒu
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array3< In_value, In_alloc > &in,
						  array3< Out_value, Out_alloc > &out,
						  const array3< Kernel_value, Kernel_alloc > &kernel,
						  const size_t offset_i,
						  const size_t offset_j,
						  const size_t offset_k )
{
	typedef typename __linear__::calc_type< is_color< typename Out_alloc::value_type >::value >::type calc_type;
	mist::array3< int > p_diff( kernel.width( ), kernel.height( ), kernel.depth( ) );
	__linear__::pointer_differences_3( p_diff, offset_i, offset_j, offset_k, 1, in.width( ), in.width( ) * in.height( ) );
	out.resize( in.width( ), in.height( ), in.depth( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	const size_t begin = offset_k * in.height( ) * in.width( ) + offset_j * in.width( ) + offset_i;
	const size_t end = in.size( ) - ( ( kernel.depth( ) - offset_k - 1 ) * in.height( ) * in.width( ) + ( kernel.height( ) - offset_j - 1 ) * in.width( ) + kernel.width( ) - offset_i - 1 );
	__linear__::filter_type< calc_type >::filtering( in, out, begin, end, kernel, p_diff );
}

/// @brief ��ʂ̐��`�t�B���^( array3 )
//! 
//! �J�[�l���z����w�肷��(���S�̓J�[�l���̃T�C�Y����v�Z)
//!
//! @param[in]  in       �c ���͔z��
//! @param[out] out      �c �o�͔z��
//! @param[in]  kernel   �c�J�[�l���z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline void linear_filter(
						  const array3< In_value, In_alloc > &in,
						  array3< Out_value, Out_alloc > &out,
						  const array3< Kernel_value, Kernel_alloc > &kernel )
{
	linear_filter( in, out, kernel, kernel.width( ) / 2, kernel.height( ) / 2, kernel.depth( ) / 2 );
}


/// @brief ���v���V�A��( array, array1, array2, array3 )
//! 
//! �T�C�Y3�̃��v���V�A��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void laplacian( const mist::array< In_value, In_alloc > &in, mist::array< Out_value, Out_alloc > &out )
{
	linear_filter( in, out, __linear__::lapl_1( ).a );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void laplacian( const mist::array1< In_value, In_alloc > &in, mist::array1< Out_value, Out_alloc > &out )
{
	linear_filter( in, out, __linear__::lapl_1( ).a );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void laplacian( const mist::array2< In_value, In_alloc > &in, mist::array2< Out_value, Out_alloc > &out )
{
	linear_filter( in, out, __linear__::lapl_2( ).a );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void laplacian( const mist::array3< In_value, In_alloc > &in, mist::array3< Out_value, Out_alloc > &out )
{
	linear_filter( in, out, __linear__::lapl_3( ).a );
}


/// @}
//  ���`�O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
