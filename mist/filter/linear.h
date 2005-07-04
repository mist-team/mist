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

	//
	// �J�[�l���z����󂯎���Ċ֐��I�u�W�F�N�g�����
	//
	template< typename In, typename Out, typename Kernel_array >
	class function_type
	{
		Kernel_array ka_;
		array< int > pd_;

	public:

		function_type( const Kernel_array &ka, const size_t o ) : ka_( ka )
		{
			// std::cout << "function_type_1" << std::endl;
			pd_.resize( ka_.size( ) );
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{
				pd_[ i ] = i - o;
			}
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t w ) : ka_( ka )
		{
			// std::cout << "function_type_2" << std::endl;
			array2< size_t > pd_2( ka_.width( ), ka_.height( ) );
			for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
			{
				for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
				{
					pd_2( i, j ) = ( j - oj ) * w + i - oi;
				}
			}
			pd_ = pd_2;
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t ok, const size_t w, const size_t h ) : ka_( ka )
		{
			// std::cout << "function_type_3" << std::endl;
			array3< size_t > pd_3( ka_.width( ), ka_.height( ), ka_.depth( ) );
			for( size_t k = 0 ; k < ka_.depth( ) ; k ++ )
			{
				for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
				{
					for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
					{
						pd_3( i, j, k ) = ( k - ok ) * w * h + ( j - oj ) * w + i - oi;
					}
				}
			}
			pd_ = pd_3;
		}

		const Out operator ( )( const In &v ) const
		{
			const In * const pv = &v;
			double res = 0.0;
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{ 
				res += *( pv + pd_[ i ] ) * ka_[ i ];
			}
			return ( static_cast< Out >( res ) );
		}

	};



	//
	// �J�[�l���z����󂯎���Ċ֐��I�u�W�F�N�g�����imist::rgb�p�j
	//
	template < typename In, typename Out, typename Kernel_array >
	class function_type< In, rgb< Out >, Kernel_array >
	{
		Kernel_array ka_;
		array< int > pd_;

	public:

		function_type( const Kernel_array &ka, const size_t o ) : ka_( ka )
		{
			// std::cout << "function_type_1" << std::endl;
			pd_.resize( ka_.size( ) );
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{
				pd_[ i ] = i - o;
			}
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t w ) : ka_( ka )
		{
			// std::cout << "function_type_2" << std::endl;
			array2< size_t > pd_2( ka_.width( ), ka_.height( ) );
			for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
			{
				for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
				{
					pd_2( i, j ) = ( j - oj ) * w + i - oi;
				}
			}
			pd_ = pd_2;
		}
		function_type( const Kernel_array &ka, const size_t oi, const size_t oj, const size_t ok, const size_t w, const size_t h ) : ka_( ka )
		{
			// std::cout << "function_type_3" << std::endl;
			array3< size_t > pd_3( ka_.width( ), ka_.height( ), ka_.depth( ) );
			for( size_t k = 0 ; k < ka_.depth( ) ; k ++ )
			{
				for( size_t j = 0 ; j < ka_.height( ) ; j ++ )
				{
					for( size_t i = 0 ; i < ka_.width( ) ; i ++ )
					{
						pd_3( i, j, k ) = ( k - ok ) * w * h + ( j - oj ) * w + i - oi;
					}
				}
			}
			pd_ = pd_3;
		}

		const rgb< Out > operator ( )( const In &v ) const
		{
			const In * const pv = &v;
			rgb< double > res = rgb< double >( );
			for( size_t i = 0 ; i < ka_.size( ) ; i ++ )
			{ 
				res += *( pv + pd_[ i ] ) * ka_[ i ];
			}
			return ( static_cast< rgb< Out > >( res ) );
		}

	};


	template< typename In, typename Out, typename Func >
	class abs
	{
		Func f_;
	public:
		abs( const Func &f ) : f_( f )
		{
			// std::cout << "abs" << std::endl;
		}
		const Out operator ( )( const In &v ) const
		{
			return static_cast< Out >( std::fabs( f_( v ) ) );
		}
	};

	template< typename In, typename Out, typename Func >
	class abs< In, rgb< Out >, Func >
	{
		Func f_;
	public:
		abs( const Func &f ) : f_( f )
		{
			// std::cout << "abs" << std::endl;
		}
		const rgb< Out > operator ( )( const In &v ) const
		{
			const rgb< Out > res = f_( v );
			return ( rgb< Out >( static_cast< Out >( std::fabs( static_cast< double >( res.r ) ) ) , static_cast< Out >( std::fabs( static_cast< double >( res.g ) ) ) , static_cast< Out >( std::fabs( static_cast< double >( res.b ) ) ) ) );
		}
	};



	inline void uniw( array< double > &a, const size_t size )
	{
		// std::cout << "uniw_0" << std::endl;
		a.resize( size );
		a.fill( 1.0 / a.size( ) );
	}

	inline void uniw_1( array1< double > &a, const size_t size )
	{
		// std::cout << "uniw_1" << std::endl;
		a.resize( size );
		a.fill( 1.0 / a.size( ) );
	}

	inline void uniw_2( array2< double > &a, const size_t size )
	{
		// std::cout << "uniw_2" << std::endl;
		a.resize( size, size );
		a.fill( 1.0 / a.size( ) );
	}

	inline void uniw_3( array3< double > &a, const size_t size )
	{
		// std::cout << "uniw_3" << std::endl;
		a.resize( size, size, size );
		a.fill( 1.0 / a.size( ) );
	}



	template< typename Array >
	inline void normalize( Array &a )
	{
		double nrm  = 0;
		for( size_t i = 0 ; i < a.size( ) ; i ++ )
		{
			nrm += a[ i ];
		}
		for( size_t i = 0 ; i < a.size( ) ; i ++ )
		{
			a[ i ] /= nrm;
		}
	}


	inline void gaus( array< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_0" << std::endl;
		a.resize( size );
		const int o = size / 2;
		for( size_t i = 0 ; i < size ; i ++ )
		{
			const int ii = static_cast< int >( i );
			a[ i ] = std::exp( -( ( ii - o ) * ( ii - o ) ) / ( 2 * sigma * sigma ) );
		}
		normalize( a );
	}

	inline void gaus_1( array1< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_1" << std::endl;
		a.resize( size );
		const int o = size / 2;
		for( size_t i = 0 ; i < size ; i ++ )
		{
			const int ii = static_cast< int >( i );
			a[ i ] = std::exp( -( ( ii - o ) * ( ii - o ) ) / ( 2 * sigma * sigma ) );
		}
		normalize( a );
	}

	inline void gaus_2( array2< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_2" << std::endl;
		a.resize( size, size );
		const int o = size / 2;
		for( size_t j = 0 ; j < size ; j ++ )
		{
			for( size_t i = 0 ; i < size ; i ++ )
			{
				const int ii = static_cast< int >( i );
				const int jj = static_cast< int >( j );
				a( i, j ) = std::exp( -( ( ii - o ) * ( ii - o ) + ( jj - o ) * ( jj - o ) ) / ( 2 * sigma * sigma ) );
			}
		}
		normalize( a );
	}

	inline void gaus_3( array3< double > &a, const size_t size, const double sigma )
	{
		// std::cout << "gaus_3" << std::endl;
		a.resize( size, size, size );
		const int o = size / 2;
		for( size_t k = 0 ; k < size ; k ++ )
		{
			for( size_t j = 0 ; j < size ; j ++ )
			{
				for( size_t i = 0 ; i < size ; i ++ )
				{
					const int ii = static_cast< int >( i );
					const int jj = static_cast< int >( j );
					const int kk = static_cast< int >( k );
					a( i, j, k ) = std::exp( -( ( ii - o ) * ( ii - o ) + ( jj - o ) * ( jj - o ) + ( kk - o ) * ( kk - o ) ) / ( 2 * sigma * sigma ) );
				}
			}
		}
		normalize( a );
	}

	

	inline void lapl( array< double > &a )
	{
		// std::cout << "lapl_0" << std::endl;
		a.resize( 3 );
		a[ 0 ] = 1.0;  a[ 1 ] = -2.0;  a[ 2 ] = 1.0;
	}

	inline void lapl_1( array1< double > &a )
	{
		// std::cout << "lapl_1" << std::endl;
		a.resize( 3 );
		a[ 0 ] = 1.0;  a[ 1 ] = -2.0;  a[ 2 ] = 1.0;
	}

	inline void lapl_2( array2< double > &a )
	{
		// std::cout << "lapl_2" << std::endl;
		a.resize( 3, 3 );
		a( 0, 0 ) = 1.0;  a( 1, 0 ) = 1.0;  a( 2, 0 ) = 1.0;
		a( 0, 1 ) = 1.0;  a( 1, 1 ) = -8.0;  a( 2, 1 ) = 1.0;
		a( 0, 2 ) = 1.0;  a( 1, 2 ) = 1.0;  a( 2, 2 ) = 1.0;
	}

	inline void lapl_3( array3< double > &a )
	{
		// std::cout << "lapl_3" << std::endl;
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

}



// 
// std::transform�̃C���^�t�F�[�X��ς�������
//
template< typename In_array, typename Out_array, typename Func >
inline void filtering(
				   const In_array &in,
				   Out_array &out,
				   const Func func,
				   const size_t begin,
				   const size_t end )
{
	std::transform( &in[ begin ], &in[ end ], &out[ begin ], func );
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
//! �J�[�l���T�C�Y3�i1�����j�C3�~3�i2�����j�C3�~3�~3�i3�����j�ɓ����������`�t�B���^��p�ӂ��Ă���܂��D
//!
//! @code �g�p��(5�~5��l�d�ݕ������t�B���^)
//! mist::array2< unsigned char > in, out;
//!
//! mist::read_bmp( in, "hoge.bmp" );  // �K���ȓ��͔z�� 
//!
//! mist::array2< double > size_3_kernel( 3, 3 ); // �K���ȃJ�[�l���z��
//! size_3_kernel.fill( 1.0 / size_3_kernel.size( ) );  // 3�~3��l�d�ݕ������t�B���^�p�̃J�[�l�����쐬
//!
//! mist::size_3_filter( in, out, size_3_kernel );  // �t�B���^�̓K�p�i�J�[�l���z��ƃJ�[�l�����S��n���܂��j
//! @endcode
//!
//! ��l�d�ݕ������t�B���^�C�K�E�V�A���t�B���^�C���v���V�A���t�B���^�i��Βl�j�� array, array1, array2, array3 �ŗp�ӂ��Ă���܂��D
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
inline bool linear_filter(
				   const array< In_value, In_alloc > &in,
				   array< Out_value, Out_alloc > &out,
				   const array< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset )
{
	// std::cout << "linear_0" << std::endl;
	typedef array< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.size( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset );
	const size_t begin = offset;
	const size_t end = in.size( ) - ( kernel.size( ) - offset - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
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
inline bool linear_filter(
				   const array1< In_value, In_alloc > &in,
				   array1< Out_value, Out_alloc > &out,
				   const array1< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset )
{
	// std::cout << "linear_1" << std::endl;
	typedef array1< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.width( ) );
	out.reso1( in.reso1( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset );
	const size_t begin = offset;
	const size_t end = in.size( ) - ( kernel.width( ) - offset - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
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
inline bool linear_filter(
				   const array2< In_value, In_alloc > &in,
				   array2< Out_value, Out_alloc > &out,
				   const array2< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset_i,
				   const size_t offset_j )
{
	// std::cout << "linear_2" << std::endl;
	typedef array2< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.width( ), in.height( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset_i, offset_j, in.width( ) );
	const size_t begin = offset_j * in.width( ) + offset_i;
	const size_t end = in.size( ) - ( ( kernel.height( ) - offset_j - 1 ) * in.width( ) + kernel.width( ) - offset_i - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
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
//! @param[in]  offset_j �ck�����̃J�[�l�����S�ʒu
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool linear_filter(
				   const array3< In_value, In_alloc > &in,
				   array3< Out_value, Out_alloc > &out,
				   const array3< Kernel_value, Kernel_alloc > &kernel,
				   const size_t offset_i,
				   const size_t offset_j,
				   const size_t offset_k )
{
	// std::cout << "linear_3" << std::endl;
	typedef array3< Kernel_value, Kernel_alloc > Kernel_array;
	out.resize( in.width( ), in.height( ), in.depth( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	const __linear__::function_type< In_value, Out_value, Kernel_array > func( kernel, offset_i, offset_j, offset_k, in.width( ), in.height( ) );
	const size_t begin = offset_k * in.height( ) * in.width( ) + offset_j * in.width( ) + offset_i;
	const size_t end = in.size( ) - ( ( kernel.depth( ) - offset_k - 1 ) * in.height( ) * in.width( ) + ( kernel.height( ) - offset_j - 1 ) * in.width( ) + kernel.width( ) - offset_i - 1 );
	filtering( in, out, func, begin, end );
	return ( 1 );
}



/// @brief �T�C�Y3�̐��`�t�B���^( array, array1, array2, array3 )
//! 
//! �T�C�Y3(1����)�C3�~3(2����)�C3�~3�~3(3����)�ɓ����������`�t�B���^
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  kernel �c�J�[�l���z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array< In_value, In_alloc > &in,
				   array< Out_value, Out_alloc > &out,
				   const array< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_0" << std::endl;
	if( kernel.width( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array1< In_value, In_alloc > &in,
				   array1< Out_value, Out_alloc > &out,
				   const array1< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_1" << std::endl;
	if( kernel.width( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array2< In_value, In_alloc > &in,
				   array2< Out_value, Out_alloc > &out,
				   const array2< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_2" << std::endl;
	if( kernel.width( ) - 3 || kernel.height( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc, typename Kernel_value, typename Kernel_alloc >
inline bool size_3_filter(
				   const array3< In_value, In_alloc > &in,
				   array3< Out_value, Out_alloc > &out,
				   const array3< Kernel_value, Kernel_alloc > &kernel )
{
	// std::cout << "size_3_3" << std::endl;
	if( kernel.width( ) - 3 || kernel.height( ) - 3 || kernel.depth( ) - 3 )
	{
		return ( 0 );
	}
	return ( linear_filter( in, out, kernel, 1, 1, 1 ) );
}


/// @brief ��l�d�ݕ�����( array, array1, array2, array3 )
//! 
//! �T�C�Y�w��\�Ȉ�l�d�ݕ�����
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  size    �c �t�B���^�̃T�C�Y�i�f�t�H���g 3�j
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_0" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array< double > kernel;
	__linear__::uniw( kernel, _size );
	return ( linear_filter( in, out, kernel, offset ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_1" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array1< double > kernel;
	__linear__::uniw_1( kernel, _size );
	return ( linear_filter( in, out, kernel, offset ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_2" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array2< double > kernel;
	__linear__::uniw_2( kernel, _size );
	return ( linear_filter( in, out, kernel, offset, offset ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool uniform_weight( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	// std::cout << "uniform_3" << std::endl;
	const size_t _size = ( size % 2 ) ? size : size - 1;
	const size_t offset = _size / 2;
	array3< double > kernel;
	__linear__::uniw_3( kernel, _size );
	return ( linear_filter( in, out, kernel, offset, offset, offset ) );
}


/// @brief �K�E�V�A��( array, array1, array2, array3 )
//! 
//! �͈͂Ƌ��x���w��\�ȃK�E�V�A��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in]  range  �c �K�E�V�A���͈̔́i�f�t�H���g 1�j
//! @param[out] sigma  �c �K�E�V�A���̋��x�i�f�t�H���g 1.0�j
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_0" << std::endl;
	array< double > kernel;
	__linear__::gaus( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_1" << std::endl;
	array1< double > kernel;
	__linear__::gaus_1( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_2" << std::endl;
	array2< double > kernel;
	__linear__::gaus_2( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range, range ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool gaussian( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out,
				   const size_t range = 1,
				   const double sigma = 1.0 )
{
	// std::cout << "gaussian_3" << std::endl;
	array3< double > kernel;
	__linear__::gaus_3( kernel, range, sigma );
	return ( linear_filter( in, out, kernel, range, range, range ) );
}



/// @brief ���v���V�A��( array, array1, array2, array3 )
//! 
//! �T�C�Y3�̃��v���V�A��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_0" << std::endl;
	array< double > kernel;
	__linear__::lapl( kernel );
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_1" << std::endl;
	array1< double > kernel;
	__linear__::lapl_1( kernel );
	return ( linear_filter( in, out, kernel, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_2" << std::endl;
	array2< double > kernel;
	__linear__::lapl_2( kernel );
	return ( linear_filter( in, out, kernel, 1, 1 ) );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out )
{
	// std::cout << "laplacian_3" << std::endl;
	array3< double > kernel;
	__linear__::lapl_3( kernel );
	return ( linear_filter( in, out, kernel, 1, 1, 1 ) );
}



/// @brief ��Βl��Ԃ����v���V�A��( array, array1, array2, array3 )
//! 
//! �T�C�Y3�̐�Βl��Ԃ����v���V�A��
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array< double > > Function_type;
	// std::cout << "lapl_abs_0" << std::endl;
	out.resize( in.size( ) );
	array< double > kernel;
	__linear__::lapl( kernel );
	const Function_type func( kernel, 1 );
	const size_t begin = 1;
	const size_t end = in.size( ) - 1; 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array1< double > > Function_type;
	// std::cout << "lapl_abs_1" << std::endl;
	out.resize( in.width( ) );
	out.reso1( in.reso1( ) );
	array1< double > kernel;
	__linear__::lapl_1( kernel );
	const __linear__::function_type< In_value, Out_value, array1< double > > func( kernel, 1 );
	const size_t begin = 1;
	const size_t end = in.size( ) - 1; 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array2< double > > Function_type; 
	// std::cout << "lapl_abs_2" << std::endl;
	out.resize( in.width( ), in.height( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	array2< double > kernel;
	__linear__::lapl_2( kernel );
	const Function_type func( kernel, 1, 1, in.width( ) );
	const size_t begin = in.width( ) + 1;
	const size_t end = in.size( ) - ( in.width( ) + 1 ); 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}

template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline bool laplacian_abs( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out )
{
	typedef __linear__::function_type< In_value, Out_value, array3< double > > Function_type;
	// std::cout << "lapl_abs_3" << std::endl;
	out.resize( in.width( ), in.height( ), in.depth( ) );
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	array3< double > kernel;
	__linear__::lapl_3( kernel );
	const Function_type func( kernel, 1, 1, 1, in.width( ), in.height( ) );
	const size_t begin = in.height( ) * in.width( ) + in.width( ) + 1;
	const size_t end = in.size( ) - ( in.height( ) * in.width( ) + in.width( ) + 1 ); 
	filtering( in, out, __linear__::abs< In_value, Out_value, Function_type >( func ), begin, end );
	return ( 1 );
}



/// @}
//  ���`�O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_FILTER_LINEAR_FILTER_H__
