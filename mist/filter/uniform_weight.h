/// @file mist/filter/uniform_weight.h
//!
//! @brief ��l�d�ݕ������t�B���^�̂��߂̃��C�u����
//!



#ifndef __INCLUDE_FILTER_UNIFORM_WEIGHT_FILTER_H__
#define __INCLUDE_FILTER_UNIFORM_WEIGHT_FILTER_H__



#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#include <algorithm>
#include <cmath>
#include <mist/config/color.h>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __uniform_weight__
{

	inline void uniform_weight_kernel( mist::array< double > &uni_w, const size_t size )
	{
		uni_w.resize( size );
		uni_w.fill( 1.0 / uni_w.size( ) );
	}


	inline void pointer_differences( mist::array< int > &p_diff, const int inc )
	{
		const int o = static_cast< int >( p_diff.size( ) ) / 2;
		for( size_t i = 0 ; i < p_diff.size( ) ; i ++ )
		{
			const int di = static_cast< int >( i );
			p_diff[ i ] = ( di - o ) * inc;
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



	template< typename Calc_type >
	struct filter_type
	{
		template< typename In_type, typename Out_type >
			static filtering( const mist::array< In_type > &in, mist::array< Out_type > &out, const size_t begin, const size_t end, const mist::array< double > &uni_w, const mist::array< int > &p_diff )
		{
			Calc_type val;
			for( size_t i = begin ; i < end ; i ++ )
			{
				val = Calc_type( );
				int di = static_cast< int >( i );
				for( size_t m = 0 ; m < uni_w.size( ) ; m ++ )
				{
					val += in[ static_cast< size_t >( di + p_diff[ m ] ) ] * uni_w[ m ];
				}
				out[ i ] = static_cast< Out_type >( val );
			}
		}
	};	
}



//! @addtogroup uniform_weight_group ��l�d�ݕ������t�B���^
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/uniform_weight.h>
//! @endcode
//!
//! �z��̒[�i�J�[�l�����͂ݏo���Ƃ���j�̗v�f�̌v�Z�l�͕ۏႵ�܂���D
//! �g�p�\�ȗv�f�^�́C�Z�p�^�� mist::rgb< �Z�p�^ > �Ɍ���܂��D
//! �����̌v�Z�́C�o�͔z��̗v�f�^���Z�p�^�̏ꍇ�� double �^�C
//! mist::rgb< �Z�p�^ > �̏ꍇ�� mist::rgb< double > �ōs���܂��D
//! 
//!  @{


/// @brief ��l�d�ݕ�����( array, array1, array2, array3 )
//! 
//! �T�C�Y���w��\�Ȉ�l�d�ݕ�����
//!
//! @param[in]  in     �c ���͔z��
//! @param[out] out    �c �o�͔z��
//! @param[in] size  �c �t�B���^�̈�ӂ̃T�C�Y�i�f�t�H���g 3�j
//!
template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
				   const mist::array< In_value, In_alloc > &in, 
				   mist::array< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< Out_alloc::value_type >::value >::type calc_type;
	
	array< double > uni_w;
	__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );
	
	out.resize( in.size( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, out, begin, end, uni_w, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
				   const mist::array1< In_value, In_alloc > &in, 
				   mist::array1< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< Out_alloc::value_type >::value >::type calc_type;
	
	out.reso1( in.reso1( ) );
	
	array< double > uni_w;
	__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );

	out.resize( in.width( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, out, begin, end, uni_w, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
				   const mist::array2< In_value, In_alloc > &in, 
				   mist::array2< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< Out_alloc::value_type >::value >::type calc_type;

	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	
	array< double > uni_w;
	mist::__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );

	mist::array< calc_type > work( in.size( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, work, begin, end, uni_w, p_diff );
	
	out.resize( in.width( ) , in.height( ) );
	begin = ( uni_w.size( ) / 2 ) * ( in.width( ) + 1 );
	end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, static_cast< int >( in.width( ) ) );	
	__uniform_weight__::filter_type< calc_type >::filtering( work, out, begin, end, uni_w, p_diff );
}


template< typename In_value, typename In_alloc, typename Out_value, typename Out_alloc >
inline void uniform_weight( 
				   const mist::array3< In_value, In_alloc > &in, 
				   mist::array3< Out_value, Out_alloc > &out,
				   const size_t size = 3 )
{
	typedef typename __uniform_weight__::calc_type< is_color< Out_alloc::value_type >::value >::type calc_type;
	
	out.reso1( in.reso1( ) );
	out.reso2( in.reso2( ) );
	out.reso3( in.reso3( ) );
	
	array< double > uni_w;
	__uniform_weight__::uniform_weight_kernel( uni_w, size );
	
	mist::array< int > p_diff( uni_w.size( ) );

	mist::array< calc_type > work1( in.size( ) );
	size_t begin = uni_w.size( ) / 2;
	size_t end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, 1 );
	__uniform_weight__::filter_type< calc_type >::filtering( in, work1, begin, end, uni_w, p_diff );
	
	mist::array< calc_type > work2( in.size( ) );
	begin = ( uni_w.size( ) / 2 ) * ( in.width( ) + 1 );
	end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, static_cast< int >( in.width( ) ) );	
	__uniform_weight__::filter_type< calc_type >::filtering( work1, work2, begin, end, uni_w, p_diff );

	out.resize( in.width( ), in.height( ), in.depth( ) );
	begin = ( uni_w.size( ) / 2 ) * ( in.height( ) + in.width( ) + 1 );
	end = in.size( ) - begin;
	__uniform_weight__::pointer_differences( p_diff, static_cast< int >( in.width( ) * in.height( ) ) );	
	__uniform_weight__::filter_type< calc_type >::filtering( work2, out, begin, end, uni_w, p_diff );
}



/// @}
//   ��l�d�ݕ������O���[�v�̏I���



// mist���O��Ԃ̏I���
_MIST_END



#endif // __INCLUDE_FILTER_UNIFORM_WEIGHT_H__
