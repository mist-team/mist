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

/// @file mist/active_search.h
//!
//! @brief �A�N�e�B�u�T���̂��߂̃��C�u����
//!


#ifndef __INCLUDE_ACTIVE_SEARCH_H__
#define __INCLUDE_ACTIVE_SEARCH_H__



#include <mist/mist.h>
#include <mist/config/color.h>

#include <functional>



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN




//! @addtogroup active_search_group Active Search
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/active_search.h>
//! @endcode
//!
//!  @{

/// @brief �C�ӂ̌^��v�f�Ƃ��Ď��摜��ΏۂƂ����A�N�e�B�u�T���̌`��
//! 
//! @attention ���̌`���ɏ]���āC�����ŌĂяo���֐����ύX�����
//! 
struct active_search_style
{
	/// @brief �C�ӂ̌^��v�f�Ƃ��Ď��摜��ΏۂƂ����A�N�e�B�u�T���̌`��
	enum style
	{
		speed,  ///< speed  �d���i�q�X�g�O�����쐬�̍ۂɎ��O�Ɍv�Z�����C�ӂ̌^����T�C�Y�^�̕ϊ����ʂ��g�p���邱�Ƃɂ�荂���������ꍇ������A�ő�œ��͉摜�ƎQ�Ɖ摜�Ɠ������炢�̃�������]���Ɏg�p�j
		memory,	///< memory �d���i�q�X�g�O�����쐬�̍ۂɔC�ӂ̌^����T�C�Y�^�̕ϊ��𖈉�s�����Ƃɂ�胁�������ȗ͉������A���̕ϊ��ɔ����R�X�g�������ꍇ�͌v�Z������������j
	};
};




namespace _active_search_
{


	/*************************************************************************************************************************************************
	**
	**  �Q�l����:
	**
	**     ���� �m�CV.V.Vinod, �Ǐ��F����p�����������̒T�� -�A�N�e�B�u�T���@- (Fast visual search using focussed color matching - active search -), 
	**     �d�q���ʐM�w��_����, Vol. J81-DII,No,9, pp.2035-2042, 1998. 
	**
	**************************************************************************************************************************************************/


	inline double _minimum(
			const double	&v0,
			const double	&v1 )
	{
		return ( v0 < v1 ) ? v0 : v1;
	}

	inline double _upper(
			const size_t	&overlap,
			const size_t	&size,
			const double	&sim )
	{
		return ( _minimum( sim * size, static_cast< double >( overlap ) ) + size - overlap ) / size;
	}

	template < typename Histogram_type >
	inline double _similarity( 
			const Histogram_type	&histo0, 
			const Histogram_type	&histo1 )
	{
		double similarity = 0.0;
		for( size_t i = 0 ; i < histo0.size( ) ; i ++ )
		{
			similarity += _minimum( histo0[ i ], histo1[ i ] );
		}
		return similarity;
	}

	template < typename Functor >
	inline void _quantize( 
			const array< typename Functor::argument_type >	&in,
			array< typename Functor::result_type >			&out,
			const Functor									&f )
	{
		for( size_t i = 0 ; i < in.size( ) ; i ++ )
		{
			out[ i ] = f( in[ i ] );
		}
	}

	


	template < typename Argument_type, typename Result_type >
	struct _functor : std::unary_function< Argument_type, Result_type >
	{
		typedef typename std::unary_function< Argument_type, Result_type >	base_type;
		typedef typename base_type::argument_type							argument_type;
		typedef typename base_type::result_type								result_type;
		_functor( )
		{
		}
		const result_type &operator ( )( const argument_type &v ) const
		{
			return v;
		}
	};

	template < >
	struct _functor< rgb< unsigned char >, unsigned int > : std::unary_function< rgb< unsigned char >, unsigned int >
	{
		typedef std::unary_function< rgb< unsigned char >, unsigned int >	base_type;
		typedef base_type::argument_type									argument_type;
		typedef base_type::result_type										result_type;
		unsigned char	level_;
		size_t			d1_;
		size_t			d2_;
		_functor( const size_t &num_of_bins ) : level_( static_cast< unsigned char >( ( 256 % num_of_bins == 0 ) ? ( 256 / num_of_bins ) : ( 256 / ( num_of_bins - 1 ) ) ) ), d1_( num_of_bins ), d2_( num_of_bins * num_of_bins )
		{	
		}
		result_type operator ( )( const argument_type &v ) const
		{
			return static_cast< result_type >( ( v.r / level_ ) * d2_ + ( v.g / level_ ) * d1_ + ( v.b / level_ ) );
		}
	};



	template < typename Array_type, typename Histogram_type, typename Functor >
	inline void _make_histogram( 
			const Array_type	&array,
			Histogram_type		&histo,
			const size_t		&bi,
			const size_t		&w,
			const Functor		&f )
	{
		histo.fill( 0.0 );
		const double gain = 1.0 / w;
		for( size_t i = 0 ; i < w ; i ++ )
		{
			histo[ f( array[ bi + i ] ) ] += gain;
		}
	}

	template < typename Array_type, typename Histogram_type, typename Functor >
	inline void _diff_histogram( 
			const Array_type	&array,
			Histogram_type		&histo,
			const size_t		&bi,
			const size_t		&pbi,
			const size_t		&w,
			const Functor		&f )
	{
		if( bi < pbi + w )
		{
			const double gain = 1.0 / w;
			for( size_t i = 0 ; i < bi - pbi ; i ++ )
			{
				histo[ f( array[ pbi + i ] ) ] -= gain;
				histo[ f( array[ pbi + w + i ] ) ] += gain;
			}
		}
		else
		{
			_make_histogram( array, histo, bi, w, f );
		}
	}

	template< typename Table_type >
	inline void _culling(
			Table_type		&c_tab,
			const size_t	&bi,
			const size_t	&w,
			const double	&sim,
			const double	&max_sim )
	{
		const size_t size = w;
		for( size_t i = 0 ; i < w ; i ++ )
		{
			if( _upper( w - i, size, sim ) < max_sim )
			{
				c_tab[ bi + i ] = true;
			}
			else
			{
				break;
			}
		}
	}

	template< typename Array_type, typename Functor >
	inline bool __search(
			const Array_type		&in,
			const Array_type		&ref,
			unsigned int			&x,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		typedef array< double >		histogram_type;
		
		histogram_type i_histo( num_of_bins );
		histogram_type r_histo( num_of_bins );
		_make_histogram( ref, r_histo, 0, ref.size( ), f );
		array< bool > culling_table( in.size( ) );	
		x = 0;
		scale = b_scale;
		similarity = ( similarity < 0.0 || 1.0 < similarity ) ? 0.0 : similarity;
		double s = b_scale;
		double sim = 0.0;
		size_t w, pi;
		bool is_found = false;
		
		while( ( scale_factor > 1.0 ) ? ( s <= e_scale ) : ( s >= e_scale ) )
		{
			w = static_cast< size_t >( ref.size( ) * s );
			_make_histogram( in, i_histo, 0, w, f );
			pi = 0;
			culling_table.fill( false );
			for( size_t i = 0 ; i < in.size( ) - w + 1 ; i ++ )
			{
				if( !culling_table[ i ] )
				{
					_diff_histogram( in, i_histo, i, pi, w, f );
					pi = i;
					sim = _similarity( i_histo, r_histo );
					if( sim > similarity )
					{
						is_found = true;
						x = static_cast< unsigned int >( i );
						scale = s;
						similarity = sim;
					}
					_culling( culling_table, i, w, sim, similarity );
				}
			}
			s *= scale_factor;
		}		
		return is_found;
	}

	template< typename Array_type, typename Functor >
	inline bool _search_memory(
			const Array_type		&in,
			const Array_type		&ref,
			unsigned int			&x,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		//std::cout << "memory" << std::endl;
		return __search( in, ref, x, scale, similarity, num_of_bins, f, b_scale, e_scale, scale_factor );
	}

	template< typename Array_type, typename Functor >
	inline bool _search_speed(
			const Array_type		&in2,
			const Array_type		&ref2,
			unsigned int			&x,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		//std::cout << "speed" << std::endl;
		typedef typename Functor::result_type		value_type;
		array< value_type > in( in2.size( ) );
		_quantize( in2, in, f );
		array< value_type > ref( ref2.size( ) );
		_quantize( ref2, ref, f );
		return __search( in, ref, x, scale, similarity, num_of_bins, _functor< value_type, value_type >( ), b_scale, e_scale, scale_factor );
	}

	template< typename Array_type, typename Functor >
	inline bool _search(
			const Array_type					&input,
			const Array_type					&reference,
			unsigned int						&x,
			double								&scale,
			double								&similarity,
			double								min_scale,
			double								max_scale,
			const double						&scale_factor,
			const size_t						&num_of_bins,
			const Functor						&f,
			const active_search_style::style	&style )
	{
		if( scale_factor <= 0.0 || scale_factor == 1.0 )
		{
			return false;
		}
		const double u_bound = static_cast< double >( input.size( ) ) / reference.size( );
		const double l_bound = 0.01;
		min_scale = ( min_scale < l_bound ) ? l_bound : min_scale;
		min_scale = ( min_scale > u_bound ) ? u_bound : min_scale;
		max_scale = ( max_scale < l_bound ) ? l_bound : max_scale;
		max_scale = ( max_scale > u_bound ) ? u_bound : max_scale;
		if( min_scale > max_scale )
		{
			double tmp = min_scale;
			min_scale = max_scale;
			max_scale = tmp;
		}
		double begin_scale, end_scale;
		if( scale_factor > 1.0 )
		{
			begin_scale = min_scale;
			end_scale = max_scale;
		}
		else
		{
			begin_scale = max_scale;
			end_scale = min_scale;
		}
		if( style == active_search_style::speed )
		{
			return _search_speed( input, reference, x, scale, similarity, num_of_bins, f, begin_scale, end_scale, scale_factor );
		}
		if( style == active_search_style::memory )
		{
			return _search_memory( input, reference, x, scale, similarity, num_of_bins, f, begin_scale, end_scale, scale_factor );
		}
		return false;
	}




	template < typename Array_type, typename Histogram_type, typename Functor >
	inline void _make_histogram( 
			const Array_type	&array,
			Histogram_type		&histo,
			const size_t		&bi,
			const size_t		&bj,
			const size_t		&w,
			const size_t		&h,
			const Functor		&f )
	{
		histo.fill( 0.0 );
		const double gain = 1.0 / ( w * h );
		for( size_t j = 0 ; j < h ; j ++ )
		{
			for( size_t i = 0 ; i < w ; i ++ )
			{
				histo[ f( array( bi + i, bj + j ) ) ] += gain;
			}
		}
	}

	template < typename Array_type, typename Histogram_type, typename Functor >
	inline void _diff_histogram( 
			const Array_type	&array,
			Histogram_type		&histo,
			const size_t		&bi,
			const size_t		&bj,
			const size_t		&pbi,
			const size_t		&pbj,
			const size_t		&w,
			const size_t		&h,
			const Functor		&f )
	{
		if( bi < pbi + w )
		{
			const double gain = 1.0 / ( w * h );
			size_t j = 0;
			for( ; j < bj - pbj ; j ++ )
			{
				for( size_t i = 0 ; i < w ; i ++ )
				{
					histo[ f( array( pbi + i, pbj + j ) ) ] -= gain;
					histo[ f( array( bi + i, pbj + h + j ) ) ] += gain;
				}
			}
			for( ; j < h ; j ++ )
			{
				for( size_t i = 0 ; i < bi - pbi ; i ++ )
				{
					histo[ f( array( pbi + i, pbj + j ) ) ] -= gain;
					histo[ f( array( pbi + w + i, bj + j ) ) ] += gain;
				}
			}
		}
		else
		{
			_make_histogram( array, histo, bi, bj, w, h, f );
		}
	}

	template< typename Table_type >
	inline void _culling(
			Table_type		&c_tab,
			const size_t	&bi,
			const size_t	&bj,
			const size_t	&w,
			const size_t	&h,
			const double	&sim,
			const double	&max_sim )
	{
		const size_t size = w * h;
		for( size_t j = 0 ; j < h ; j ++ )
		{
			if( _upper( w * ( h - j ), size, sim ) >= max_sim )
			{
				break;
			}
			for( size_t i = 0 ; i < w ; i ++ )
			{
				if( _upper( ( w - i ) * ( h - j ), size, sim ) < max_sim )
				{
					c_tab( bi + i, bj + j ) = true;
					if( i <= bi )
					{
						c_tab( bi - i, bj + j ) = true;
					}
				}
				else
				{
					break;
				}
			}
		}
	}

	template< typename Array_type, typename Functor >
	inline bool __search(
			const Array_type		&in,
			const Array_type		&ref,
			unsigned int			&x,
			unsigned int			&y,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		typedef array< double >		histogram_type;
		
		histogram_type i_histo( num_of_bins );
		histogram_type r_histo( num_of_bins );
		_make_histogram( ref, r_histo, 0, 0, ref.width( ), ref.height( ), f );
		array2< bool > culling_table( in.width( ), in.height( ) );	
		x = 0;
		y = 0;
		scale = b_scale;
		similarity = ( similarity < 0.0 || 1.0 < similarity ) ? 0.0 : similarity;
		double s = b_scale;
		double sim = 0.0;
		size_t w, h, pi, pj;
		histogram_type histo( num_of_bins );
		bool is_found = false;
		
		while( ( scale_factor > 1.0 ) ? ( s <= e_scale ) : ( s >= e_scale ) )
		{
			w = static_cast< size_t >( ref.width( ) * s );
			h = static_cast< size_t >( ref.height( ) * s );
			_make_histogram( in, histo, 0, 0, w, h, f );	
			culling_table.fill( false );
			pj = 0;
			for( size_t j = 0 ; j < in.height( ) - h + 1 ; j ++ )
			{
				_diff_histogram( in, histo, 0, j, 0, pj, w, h, f );
				i_histo = histo;
				pj = j;
				pi = 0;
				for( size_t i = 0 ; i < in.width( ) - w + 1 ; i ++ )
				{
					if( !culling_table( i, j ) )
					{
						_diff_histogram( in, i_histo, i, j, pi, pj, w, h, f );
						pi = i;
						sim = _similarity( i_histo, r_histo );
						if( sim > similarity )
						{
							is_found = true;
							x = static_cast< unsigned int >( i );
							y = static_cast< unsigned int >( j );
							scale = s;
							similarity = sim;
						}
						_culling( culling_table, i, j, w, h, sim, similarity );
					}
				}
			}
			s *= scale_factor;
		}		
		return is_found;
	}

	template< typename Array_type, typename Functor >
	inline bool _search_memory(
			const Array_type		&in,
			const Array_type		&ref,
			unsigned int			&x,
			unsigned int			&y,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		//std::cout << "memory" << std::endl;
		return __search( in, ref, x, y, scale, similarity, num_of_bins, f, b_scale, e_scale, scale_factor );
	}

	template< typename Array_type, typename Functor >
	inline bool _search_speed(
			const Array_type		&in2,
			const Array_type		&ref2,
			unsigned int			&x,
			unsigned int			&y,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		//std::cout << "speed" << std::endl;
		typedef typename Functor::result_type		value_type;
		array2< value_type > in( in2.width( ), in2.height( ) );
		_quantize( in2, in, f );
		array2< value_type > ref( ref2.width( ), ref2.height( ) );
		_quantize( ref2, ref, f );
		return __search( in, ref, x, y, scale, similarity, num_of_bins, _functor< value_type, value_type >( ), b_scale, e_scale, scale_factor );
	}

	template< typename Array_type, typename Functor >
	inline bool _search(
			const Array_type					&input,
			const Array_type					&reference,
			unsigned int						&x,
			unsigned int						&y,
			double								&scale,
			double								&similarity,
			double								min_scale,
			double								max_scale,
			const double						&scale_factor,
			const size_t						&num_of_bins,
			const Functor						&f,
			const active_search_style::style	&style )
	{
		if( scale_factor <= 0.0 || scale_factor == 1.0 )
		{
			return false;
		}
		const double u_bound = _minimum( static_cast< double >( input.width( ) ) / reference.width( ), static_cast< double >( input.height( ) ) / reference.height( ) );
		const double l_bound = 0.01;
		min_scale = ( min_scale < l_bound ) ? l_bound : min_scale;
		min_scale = ( min_scale > u_bound ) ? u_bound : min_scale;
		max_scale = ( max_scale < l_bound ) ? l_bound : max_scale;
		max_scale = ( max_scale > u_bound ) ? u_bound : max_scale;
		if( min_scale > max_scale )
		{
			double tmp = min_scale;
			min_scale = max_scale;
			max_scale = tmp;
		}
		double begin_scale, end_scale;
		if( scale_factor > 1.0 )
		{
			begin_scale = min_scale;
			end_scale = max_scale;
		}
		else
		{
			begin_scale = max_scale;
			end_scale = min_scale;
		}
		if( style == active_search_style::speed )
		{
			return _search_speed( input, reference, x, y, scale, similarity, num_of_bins, f, begin_scale, end_scale, scale_factor );
		}
		if( style == active_search_style::memory )
		{
			return _search_memory( input, reference, x, y, scale, similarity, num_of_bins, f, begin_scale, end_scale, scale_factor );
		}
		return false;
	}



	template < typename Array_type, typename Histogram_type, typename Functor >
	inline void _make_histogram( 
			const Array_type	&array,
			Histogram_type		&histo,
			const size_t		&bi,
			const size_t		&bj,
			const size_t		&bk,
			const size_t		&w,
			const size_t		&h,
			const size_t		&d,
			const Functor		&f )
	{
		histo.fill( 0.0 );
		const double gain = 1.0 / ( w * h * d );
		for( size_t k = 0 ; k < d ; k ++ )
		{
			for( size_t j = 0 ; j < h ; j ++ )
			{
				for( size_t i = 0 ; i < w ; i ++ )
				{
					histo[ f( array( bi + i, bj + j, bk + k ) ) ] += gain;
				}
			}
		}
	}

	template < typename Array_type, typename Histogram_type, typename Functor >
	inline void _diff_histogram( 
			const Array_type	&array,
			Histogram_type		&histo,
			const size_t		&bi,
			const size_t		&bj,
			const size_t		&bk,
			const size_t		&pbi,
			const size_t		&pbj,
			const size_t		&pbk,
			const size_t		&w,
			const size_t		&h,
			const size_t		&d,
			const Functor		&f )
	{
		if( bi < pbi + w )
		{
			const double gain = 1.0 / ( w * h * d );
			size_t k = 0;
			for( ; k < bk - pbk ; k ++ )
			{
				for( size_t j = 0 ; j < h ; j ++ )
				{
					for( size_t i = 0 ; i < w ; i ++ )
					{
						histo[ f( array( pbi + i, pbj + j, pbk + k ) ) ] -= gain;
						histo[ f( array( bi + i, bj + j, pbk + d + k ) ) ] += gain;
					}
				}
			}
			for( ; k < d ; k ++ )
			{
				size_t j = 0;
				for( ; j < bj - pbj ; j ++ )
				{
					for( size_t i = 0 ; i < w ; i ++ )
					{
						histo[ f( array( pbi + i, pbj + j, bk + k ) ) ] -= gain;
						histo[ f( array( bi + i, pbj + h + j, bk + k ) ) ] += gain;
					}
				}
				for( ; j < h ; j ++ )
				{
					for( size_t i = 0 ; i < bi - pbi ; i ++ )
					{
						histo[ f( array( pbi + i, bj + j, bk + k ) ) ] -= gain;
						histo[ f( array( pbi + w + i, bj + j, bk + k ) ) ] += gain;
					}
				}
			}
		}
		else
		{
			_make_histogram( array, histo, bi, bj, bk, w, h, d, f );
		}
	}

	template< typename Table_type >
	inline void _culling(
			Table_type		&c_tab,
			const size_t	&bi,
			const size_t	&bj,
			const size_t	&bk,
			const size_t	&w,
			const size_t	&h,
			const size_t	&d,
			const double	&sim,
			const double	&max_sim )
	{
		const size_t size = w * h * d;
		for( size_t k = 0 ; k < d ; k ++ )
		{
			if( _upper( w * h * ( d - k ), size, sim ) >= max_sim )
			{
				break;
			}		
			for( size_t j = 0 ; j < h ; j ++ )
			{
				if( _upper( w * ( h - j ) * ( d - k ), size, sim ) >= max_sim )
				{
					break;
				}
				for( size_t i = 0 ; i < w ; i ++ )
				{
					if( _upper( ( w - i ) * ( h - j ) * ( d - k ), size, sim ) < max_sim )
					{
						c_tab( bi + i, bj + j, bk + k ) = true;
						if( j <= bj )
						{
							c_tab( bi + i, bj - j, bk + k ) = true;
						}
						if( i <= bi )
						{
							c_tab( bi - i, bj + j, bk + k ) = true;
							if( j <= bj )
							{
								c_tab( bi - i, bj - j, bk + k ) = true;
							}
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	template< typename Array_type, typename Functor >
	inline bool __search(
			const Array_type		&in,
			const Array_type		&ref,
			unsigned int			&x,
			unsigned int			&y,
			unsigned int			&z,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		typedef array< double >		histogram_type;
		
		histogram_type i_histo( num_of_bins );
		histogram_type r_histo( num_of_bins );
		_make_histogram( ref, r_histo, 0, 0, 0, ref.width( ), ref.height( ), ref.depth( ), f );
		array3< bool > culling_table( in.width( ), in.height( ), in.depth( ) );	
		x = 0;
		y = 0;
		z = 0;
		scale = b_scale;
		similarity = ( similarity < 0.0 || 1.0 < similarity ) ? 0.0 : similarity;
		double s = b_scale;
		double sim = 0.0;
		size_t w, h, d, pi, pj, pk;
		histogram_type histo1( num_of_bins ), histo2( num_of_bins );
		bool is_found = false;
		
		while( ( scale_factor > 1.0 ) ? ( s <= e_scale ) : ( s >= e_scale ) )
		{
			w = static_cast< size_t >( ref.width( ) * s );
			h = static_cast< size_t >( ref.height( ) * s );
			d = static_cast< size_t >( ref.depth( ) * s );
			_make_histogram( in, histo2, 0, 0, 0, w, h, d, f );	
			culling_table.fill( false );
			pk = 0;
			for( size_t k = 0 ; k < in.depth( ) - d + 1 ; k ++ )
			{
				_diff_histogram( in, histo2, 0, 0, k, 0, 0, pk, w, h, d, f );
				histo1 = histo2;
				pk = k;
				pj = 0;
				for( size_t j = 0 ; j < in.height( ) - h + 1 ; j ++ )
				{
					_diff_histogram( in, histo1, 0, j, k, 0, pj, pk, w, h, d, f );
					i_histo = histo1;
					pj = j;
					pi = 0;
					for( size_t i = 0 ; i < in.width( ) - w + 1 ; i ++ )
					{
						if( !culling_table( i, j, k ) )
						{	
							//_make_histogram( in, i_histo, i, j, k, w, h, d, f );	
							_diff_histogram( in, i_histo, i, j, k, pi, pj, pk, w, h, d, f );
							pi = i;
							sim = _similarity( i_histo, r_histo );
							if( sim > similarity )
							{
								is_found = true;
								x = static_cast< unsigned int >( i );
								y = static_cast< unsigned int >( j );
								z = static_cast< unsigned int >( k );
								scale = s;
								similarity = sim;
							}
							_culling( culling_table, i, j, k, w, h, d, sim, similarity );
						}
					}
				}
			}
			s *= scale_factor;
		}		
		return is_found;
	}

	template< typename Array_type, typename Functor >
	inline bool _search_memory(
			const Array_type		&in,
			const Array_type		&ref,
			unsigned int			&x,
			unsigned int			&y,
			unsigned int			&z,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		//std::cout << "memory" << std::endl;
		return __search( in, ref, x, y, z, scale, similarity, num_of_bins, f, b_scale, e_scale, scale_factor );
	}

	template< typename Array_type, typename Functor >
	inline bool _search_speed(
			const Array_type		&in2,
			const Array_type		&ref2,
			unsigned int			&x,
			unsigned int			&y,
			unsigned int			&z,
			double					&scale,
			double					&similarity,
			const size_t			&num_of_bins,
			const Functor			&f,
			const double			&b_scale,
			const double			&e_scale,
			const double			&scale_factor )
	{
		//std::cout << "speed" << std::endl;
		typedef typename Functor::result_type		value_type;
		array3< value_type > in( in2.width( ), in2.height( ), in2.depth( ) );
		_quantize( in2, in, f );
		array3< value_type > ref( ref2.width( ), ref2.height( ), ref2.depth( ) );
		_quantize( ref2, ref, f );
		return __search( in, ref, x, y, z, scale, similarity, num_of_bins, _functor< value_type, value_type >( ), b_scale, e_scale, scale_factor );
	}

	template< typename Array_type, typename Functor >
	inline bool _search(
			const Array_type					&input,
			const Array_type					&reference,
			unsigned int						&x,
			unsigned int						&y,
			unsigned int						&z,
			double								&scale,
			double								&similarity,
			double								min_scale,
			double								max_scale,
			const double						&scale_factor,
			const size_t						&num_of_bins,
			const Functor						&f,
			const active_search_style::style	&style )
	{
		if( scale_factor <= 0.0 || scale_factor == 1.0 )
		{
			return false;
		}
		const double u_bound = _minimum( static_cast< double >( input.width( ) ) / reference.width( ), _minimum( static_cast< double >( input.height( ) ) / reference.height( ), static_cast< double >( input.depth( ) ) / reference.depth( ) ) );
		const double l_bound = 0.01;
		min_scale = ( min_scale < l_bound ) ? l_bound : min_scale;
		min_scale = ( min_scale > u_bound ) ? u_bound : min_scale;
		max_scale = ( max_scale < l_bound ) ? l_bound : max_scale;
		max_scale = ( max_scale > u_bound ) ? u_bound : max_scale;
		if( min_scale > max_scale )
		{
			double tmp = min_scale;
			min_scale = max_scale;
			max_scale = tmp;
		}
		double begin_scale, end_scale;
		if( scale_factor > 1.0 )
		{
			begin_scale = min_scale;
			end_scale = max_scale;
		}
		else
		{
			begin_scale = max_scale;
			end_scale = min_scale;
		}
		if( style == active_search_style::speed )
		{
			return _search_speed( input, reference, x, y, z, scale, similarity, num_of_bins, f, begin_scale, end_scale, scale_factor );
		}
		if( style == active_search_style::memory )
		{
			return _search_memory( input, reference, x, y, z, scale, similarity, num_of_bins, f, begin_scale, end_scale, scale_factor );
		}
		return false;
	}

}





/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array2< mist::rgb< unsigned char > > �^�����o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array2< mist::rgb< unsigned char > > �^�łȂ���΂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍���[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍���[��x���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍���[��y���W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins2 �c �er, g, b�̃q�X�g�O�����̃r�����i��256�j
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//! 
inline bool active_search(
		const array2< rgb< unsigned char > >	&input,
		const array2< rgb< unsigned char > >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		double									&scale,
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		size_t									num_of_bins2 )
{
	//std::cout << "rgb" << std::endl;
	typedef _active_search_::_functor< rgb< unsigned char >, unsigned int >	functor;
	num_of_bins2 = ( num_of_bins2 > 256 ) ? 256 : num_of_bins2; 
	const size_t num_of_bins = num_of_bins2 * num_of_bins2 * num_of_bins2;
	return _active_search_::_search( input, reference, x, y, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, functor( num_of_bins2 ), active_search_style::speed );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array2< mist::rgb< unsigned char > > �^�����o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array2< mist::rgb< unsigned char > > �^�łȂ���΂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍���[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍���[��x���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍���[��y���W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins2 �c �er, g, b�̃q�X�g�O�����̃r�����i��256�j
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
inline bool active_search(
		const array2< rgb< unsigned char > >	&input,
		const array2< rgb< unsigned char > >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		double									&similarity,
		const double							&scale,
		size_t									num_of_bins2 )
{
	//std::cout << "rgb" << std::endl;
	typedef _active_search_::_functor< rgb< unsigned char >, unsigned int >	functor;
	num_of_bins2 = ( num_of_bins2 > 256 ) ? 256 : num_of_bins2;
	const size_t num_of_bins = num_of_bins2 * num_of_bins2 * num_of_bins2;
	double tmp;
	return _active_search_::_search( input, reference, x, y, tmp, similarity, scale, scale, 2.0, num_of_bins, functor( num_of_bins2 ), active_search_style::speed );
}




/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍��[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array< Value_type, Allocator >	&input,
		const array< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor;
	return _active_search_::_search( input, reference, x, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍��[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array< Value_type, Allocator >	&input,
		const array< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor;
	double tmp;
	return _active_search_::_search( input, reference, x, tmp, similarity, scale, scale, 2.0, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍��[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array< Value_type, Allocator >	&input,
		const array< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	return _active_search_::_search( input, reference, x, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, f, style );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍��[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array< Value_type, Allocator >	&input,
		const array< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	double tmp;
	return _active_search_::_search( input, reference, x, tmp, similarity, scale, scale, 2.0, num_of_bins, f, style );
}




/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array1< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array1< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍��[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//!
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array1< Value_type, Allocator >	&input,
		const array1< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor;
	return _active_search_::_search( input, reference, x, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array1< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array1< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍��[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array1< Value_type, Allocator >	&input,
		const array1< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor;
	double tmp;
	return _active_search_::_search( input, reference, x, tmp, similarity, scale, scale, 2.0, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array1< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array1< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍��[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array1< Value_type, Allocator >	&input,
		const array1< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	return _active_search_::_search( input, reference, x, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, f, style );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array1< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array1< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍��[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍��[�̍��W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array1< Value_type, Allocator >	&input,
		const array1< Value_type, Allocator >	&reference,
		unsigned int							&x,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	double tmp;
	return _active_search_::_search( input, reference, x, tmp, similarity, scale, scale, 2.0, num_of_bins, f, style );
}




/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array2< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array2< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍���[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍���[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍���[�� y ���W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//!
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array2< Value_type, Allocator >	&input,
		const array2< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor;	
	return _active_search_::_search( input, reference, x, y, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array2< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array2< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍���[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍���[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍���[�� y ���W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array2< Value_type, Allocator >	&input,
		const array2< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor;	
	double tmp;
	return _active_search_::_search( input, reference, x, y, tmp, similarity, scale, scale, 2.0, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array2< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array2< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍���[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍���[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍���[�� y ���W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array2< Value_type, Allocator >	&input,
		const array2< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	return _active_search_::_search( input, reference, x, y, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, f, style );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array2< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array2< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍���[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍���[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍���[�� y ���W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array2< Value_type, Allocator >	&input,
		const array2< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	double tmp;
	return _active_search_::_search( input, reference, x, y, tmp, similarity, scale, scale, 2.0, num_of_bins, f, style );
}




/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array3< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array3< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍����O�[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍����O�[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍����O�[�� y ���W
//! @param[out]    z            �c �T�����ꂽ�̈�̍����O�[�� z ���W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//!
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array3< Value_type, Allocator >	&input,
		const array3< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		unsigned int							&z,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor; 
	return _active_search_::_search( input, reference, x, y, z, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array3< �T�C�Y�^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array3< �T�C�Y�^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f�� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍����O�[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍����O�[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍����O�[�� y ���W
//! @param[out]    z            �c �T�����ꂽ�̈�̍����O�[�� z ���W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator >
inline bool active_search(
		const array3< Value_type, Allocator >	&input,
		const array3< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		unsigned int							&z,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins )
{
	//std::cout << "default" << std::endl;
	typedef typename _active_search_::_functor< Value_type, Value_type >	functor; 
	double tmp;
	return _active_search_::_search( input, reference, x, y, z, tmp, similarity, scale, scale, 2.0, num_of_bins, functor( ), active_search_style::memory );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array3< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array3< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note �Q�Ɖ摜�̃X�P�[�����w�肵���͈͂ŕω������Ȃ�����͉摜���ōł��ގ�����̈�̍����O�[�̈ʒu�A�X�P�[���A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍����O�[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍����O�[�� y ���W
//! @param[out]    z            �c �T�����ꂽ�̈�̍����O�[�� z ���W
//! @param[out]    scale        �c �T�����ꂽ�̈�̎Q�Ɖ摜�ɑ΂���X�P�[��
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     min_scale    �c �ŏ��X�P�[��(��0)
//! @param[in]     max_scale    �c �ő�X�P�[��(��0)
//! @param[in]     scale_factor �c min_scale �� max_scale �̊Ԃ� scale_factor �Ŋg��k�����Ȃ���T��(��0)
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale_factor �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array3< Value_type, Allocator >	&input,
		const array3< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		unsigned int							&z,
		double									&scale, 
		double									&similarity,
		const double							&min_scale,
		const double							&max_scale,
		const double							&scale_factor,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	return _active_search_::_search( input, reference, x, y, z, scale, similarity, min_scale, max_scale, scale_factor, num_of_bins, f, style );
}

/// @brief �w�肵���Q�Ɖ摜����͉摜�����獂���ɒT������: mist::array3< �C�ӂ̌^ > �^�o�[�W����
//!
//! @attention ���͉摜�A�Q�Ɖ摜�Ƃ��� mist::array3< �C�ӂ̌^ > �^�łȂ���΂Ȃ�Ȃ�
//! @attention �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^�istd::unary_functon ����h���j���`���Ȃ���΂Ȃ�Ȃ�
//! @attention �t�@���N�^�ɂ� argument_type �i�C�ӂ̌^�j�� result_type �i�T�C�Y�^�j�� typedef ���Ă����K�v������ 
//! @attention �T�C�Y�^�Ƃ� char, unsigned char, short, unsigned short, int �Ȃ�
//! @attention ���͉摜�A�Q�Ɖ摜�̊e�v�f����ϊ����ꂽ�T�C�Y�^�ϐ��� 0 �ȏォ�q�X�g�O�����̃r������菬�����Ȃ��Ă͂Ȃ�Ȃ�
//!
//! @note ���͉摜���Ŏw�肵���X�P�[���̎Q�Ɖ摜�ƍł��ގ�����̈�̍����O�[�̈ʒu�A�ގ��x��Ԃ�
//! @note �ގ��x�̉������w�肷�邱�Ƃɂ�薳�ʂȒT�����ȗ��\
//!
//! @param[in]     input        �c ���͉摜 
//! @param[in]     reference    �c �Q�Ɖ摜�i���̃p�^�[���ƍł��ގ��x�̍����̈����͉摜������T������j
//! @param[out]    x            �c �T�����ꂽ�̈�̍����O�[�� x ���W
//! @param[out]    y            �c �T�����ꂽ�̈�̍����O�[�� y ���W
//! @param[out]    z            �c �T�����ꂽ�̈�̍����O�[�� z ���W
//! @param[in,out] similarity   �c �ގ��x�̉�����n���A�T�����ꂽ�̈�̎Q�Ɖ摜�Ƃ̗ގ��x��Ԃ�(��1)
//! @param[in]     scale        �c �Q�Ɖ摜�̃X�P�[��
//! @param[in]     num_of_bins  �c �q�X�g�O�����̃r����
//! @param[in]     f            �c �C�ӂ̌^����T�C�Y�^�ɕϊ�����t�@���N�^
//! @param[in]     style        �c memory �d���� speed �d����I���i�f�t�H���g�� memory �d���j 
//! 
//! @retval true  �c similarity �ȏ�̗ގ��x�����̈悪�T�����ꂽ
//! @retval false �c similarity �ȏ�̗ގ��x�����̈�͑��݂��Ȃ������Ascale �� 0.0 �ȉ��̒l���n���ꂽ
//! 
template < typename Value_type, typename Allocator, typename Functor >
inline bool active_search(
		const array3< Value_type, Allocator >	&input,
		const array3< Value_type, Allocator >	&reference,
		unsigned int							&x,
		unsigned int							&y,
		unsigned int							&z,
		double									&similarity,
		const double							&scale,
		const size_t							&num_of_bins,
		const Functor							&f,
		const active_search_style::style		&style	= active_search_style::memory )
{
	//std::cout << "functor" << std::endl;
	double tmp;
	return _active_search_::_search( input, reference, x, y, z, tmp, similarity, scale, scale, 2.0, num_of_bins, f, style );
}


/// @}
//  Active Search �O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // #ifdef __INCLUDE_ACTIVE_SEARCH_H__
