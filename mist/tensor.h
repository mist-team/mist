// 
// Copyright (c) 2003-2009, MIST Project, Intelligent Media Integration COE, Nagoya University
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



/// @file mist/tensor.h
//!
//! @brief Tensor classes for higher-order singular value decomposition 
//!
//! 2nd, 3rd, 4th, and 5th order tensors are available.
//!
#ifndef __INCLUDE_MIST_TENSOR__
#define __INCLUDE_MIST_TENSOR__

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif

#ifndef __INCLUDE_NUMERIC__
#include "numeric.h"
#endif

#include <numeric>
#include <functional>


// beginning of mist namespace
_MIST_BEGIN


template< int M, typename V, typename A >
class tensor;

namespace __tensor__
{
	template< int M, typename V, typename A >
	class _base_
	{
	protected:
		typedef V value_type;
		typedef A allocator;
		typedef typename array< value_type, allocator >::size_type size_type;
		typedef matrix< V, A > matrix_type;

		size_type _mode;
		array< size_type > _ranks;
		array< value_type, allocator > _data;

		template< typename MV, typename MA >
		void _fold( const size_type &m, const matrix< MV, MA > &mat )
		{
			array< size_type > i( _mode ), d( _mode );
			for( size_type ii = 0 ; ii < _mode ; d[ ii ] = _diff( ii ), ii ++ );
			for( typename matrix_type::size_type c = 0 ; c < mat.cols( ) ; c ++ )
			{
				for( typename matrix_type::size_type r = i[ m - 1 ] = 0 ; r < mat.rows( ) ; r ++, i[ m - 1 ] ++ )
				{
					operator [ ]( ::std::inner_product( &i[ 0 ], &i[ 0 ] + i.size( ), &d[ 0 ], 0 ) ) = mat( r, c );
				}
				_inc( m % _mode, m - 1, i );
			}
		}

	private:
		size_type _diff( const size_type &i  ) const
		{
			return i == 0 ? 1 : _ranks[ i - 1 ] * _diff( i - 1 );
		}
		void _inc( const size_type &m, const size_type &em, array< size_type > &i ) const
		{
			if( m != em && ++ i[ m ] == _ranks[ m ] )
			{
				i[ m ] = 0;
				_inc( ( m + 1 ) % _mode, em, i );
			}
		}
		void _out( ::std::ostream &o, const array< size_type > &d, array< size_type > &i, const size_type &m  ) const
		{		
			for( i[ m ] = 0 ; i[ m ] < _ranks[ m ] ; i[ m ] ++ )
			{
				if( m != 0 )
				{
					i[ m ] != 0 ? o << std::endl : o;
					for( size_type ii = 0 ; ii < _mode - m - 1 ; o << "  ", ii ++ );
					m > 1 ?  o << i[ m ] + 1 << " ----------" << ::std::endl : o; 
					_out( o, d, i, m - 1 );
				}
				else
				{
					o << operator [ ]( ::std::inner_product( &i[ 0 ], &i[ 0 ] + _mode, &d[ 0 ], 0 ) );
					i[ m ] != _ranks[ m ] - 1 ? o << ", " : o;
				}
			}
		}

	protected:
		const size_type mode( ) const
		{
			return  _mode;
		}
		const size_type rank( const size_type &m ) const
		{
			return _ranks[ m - 1 ];
		}
		const size_type size( ) const
		{
			return _data.size( );
		}
		value_type &operator [ ]( const size_type &i )
		{
			return _data[ i ];
		}
		const value_type &operator [ ]( const size_type &i ) const
		{
			return _data[ i ];
		}
		matrix_type unfold( const size_type &m ) const
		{
			array< size_type > i( _mode ), d( _mode );
			for( size_type ii = 0 ; ii < _mode ; d[ ii ] = _diff( ii ), ii ++ );
			matrix_type mat( _ranks[ m - 1 ], _data.size( ) / _ranks[ m - 1 ] );
			for( typename matrix_type::size_type c = 0 ; c < mat.cols( ) ; c ++ )
			{
				for( typename matrix_type::size_type r = i[ m - 1 ] = 0 ; r < mat.rows( ) ; r ++, i[ m - 1 ] ++ )
				{
					mat( r, c ) = operator [ ]( ::std::inner_product( &i[ 0 ], &i[ 0 ] + i.size( ), &d[ 0 ], 0 ) );
				}
				_inc( m % _mode, m - 1, i );
			}
			return mat;
		}
		void out( ::std::ostream &o ) const
		{
			array< size_type > i( _mode ), d( _mode );
			for( size_type ii = 0 ; ii < _mode ; d[ ii ] = _diff( ii ), ii ++ );
			_out( o, d, i, _mode - 1 );
		}

		_base_( ) : _mode( M ), _ranks( array< size_type >( M ) )
		{
		}
	};

	template< int M, typename V, typename A >
	class _data_ : public _base_< M, V, A >
	{
		_data_( )
		{
		}
	};

	/*******************************************/
	/*                                         */
	/*          for 2nd order tensor           */
	/*                                         */
	/*******************************************/
	template< typename V, typename A >
	class _data_< 2, V, A >
	{
		template< int FM, typename FV, typename FA >
		friend class tensor;

		typedef V value_type;
		typedef A allocator;
		typedef matrix< V, A > matrix_type; 
		typedef typename matrix_type::size_type size_type;	

		matrix_type _mat;

		const size_type mode( ) const
		{
			return  2;
		}
		const size_type rank( const size_type &m ) const
		{
			return m == 1 ? _mat.rows( ) : _mat.cols( );
		}
		const size_type size( ) const
		{
			return _mat.size( );
		}
		void _resize( const size_type &r1, const size_type &r2 )
		{
			_mat.resize( r1, r2 );
		}
		void resize( const size_type &r1, const size_type &r2 )
		{
			_resize( r1, r2 );
			_mat.fill( );
		}		
		value_type &operator [ ]( const size_type &i )
		{
			return _mat[ i ];
		}
		const value_type &operator [ ]( const size_type &i ) const
		{
			return _mat[ i ];
		}
		value_type &operator ( )( const size_type &i1, const size_type &i2 )
		{
			return _mat( i1, i2 );
		}
		const value_type &operator ( )( const size_type &i1, const size_type &i2 ) const
		{
			return _mat( i1, i2 );
		}
		template< typename MV, typename MA >
		_data_< 2, V, A > fold( const size_type &m, const matrix< MV, MA > &mat ) const
		{
			return _data_< 2, V, A >( m == 1 ? mat : mat.t( ) );
		}
		matrix_type unfold( const size_type &m ) const
		{
			return m == 1 ? _mat : _mat.t( );
		}
		void out( ::std::ostream &o ) const
		{
			o << _mat.t( );
		}
		template< typename AV, typename AA >
		_data_< 2, V, A >( const size_t &r1, const size_t &r2, const array< AV, AA > &data ) : _mat( matrix_type( r1, r2 ) )
		{
			::std::copy( &data[ 0 ], &data[ 0 ] + data.size( ), &_mat[ 0 ] );
		}
		template< typename AV, typename AA >
		_data_< 2, V, A >( const array2< AV, AA > &img ) : _mat( matrix_type( img.width( ), img.height( ) ) )
		{
			::std::copy( &img[ 0 ], &img[ 0 ] + img.size( ), &_mat[ 0 ] );
		}
		template< typename MV, typename MA >
		_data_< 2, V, A >( const matrix< MV, MA > &mat ) : _mat( mat )
		{
		}
		_data_< 2, V, A >( const size_type &r1, const size_type &r2 ) : _mat( matrix_type( r1, r2 ) )
		{		
		}
		_data_< 2, V, A >( ) : _mat( matrix_type( ) )
		{
		}
	};

	/*******************************************/
	/*                                         */
	/*          for 3rd order tensor           */
	/*                                         */
	/*******************************************/
	template< typename V, typename A >
	class _data_< 3, V, A > : public _base_< 3, V, A >
	{
		template< int FM, typename FV, typename FA >
		friend class tensor;

		typedef _base_< 3, V, A > base;
		typedef typename base::value_type value_type;
		typedef typename base::size_type size_type;

		void _resize( const size_type &r1, const size_type &r2, const size_type &r3 )
		{
			if( base::_ranks[ 0 ] != r1 || base::_ranks[ 1 ] != r2 || base::_ranks[ 2 ] != r3 )
			{
				base::_ranks[ 0 ] = r1;
				base::_ranks[ 1 ] = r2;
				base::_ranks[ 2 ] = r3;
				base::_data.resize( r1 * r2 * r3 );
			}
		}
		void resize( const size_type &r1, const size_type &r2, const size_type &r3 )
		{
			_resize( r1, r2, r3 );
			base::_data.fill( );
		}
		value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3 )
		{
			return base::_data[ i1 + i2 * base::_ranks[ 0 ] + i3 * base::_ranks[ 0 ] * base::_ranks[ 1 ] ];
		}
		const value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3 ) const
		{
			return base::_data[ i1 + i2 * base::_ranks[ 0 ] + i3 * base::_ranks[ 0 ] * base::_ranks[ 1 ] ];
		}
		template< typename MV, typename MA >
		_data_< 3, V, A > fold( const size_type &m, const matrix< MV, MA > &mat ) const
		{
			_data_< 3, V, A > ret( base::_ranks[ 0 ], base::_ranks[ 1 ], base::_ranks[ 2 ] );
			ret._ranks[ m - 1 ] = mat.rows( );
			ret._resize( base::_ranks[ 0 ], base::_ranks[ 1 ], base::_ranks[ 2 ] );
			ret._fold( m, mat );
			return ret;
		}
		template< typename AV, typename AA >
		_data_< 3, V, A >( const size_type &r1, const size_type &r2, const size_type &r3, const array< AV, AA > &data )
		{
			_resize( r1, r2, r3 );
			::std::copy( &data[ 0 ], &data[ 0 ] + data.size( ), &base::_data[ 0 ] );
		}
		_data_< 3, V, A >( const size_type &r1, const size_type &r2, const size_type &r3 )
		{
			_resize( r1, r2, r3 );
		}
		template< typename TV, typename TA, typename AA >
		_data_< 3, V, A >( const array< tensor< 2, TV, TA >, AA > &ts )
		{
			_resize( ts[ 0 ].rank( 1 ), ts[ 0 ].rank( 2 ), ts.size( ) );
			for( size_type i = 0 ; i < ts.size( ) ; i ++ )
			{
				::std::copy( &ts[ i ][ 0 ], &ts[ i ][ 0 ] + ts[ 0 ].size( ), &base::_data[ i * ts[ 0 ].size( ) ] );
			}
		}
		_data_< 3, V, A >( )
		{
		}
	};

	/*******************************************/
	/*                                         */
	/*          for 4th order tensor           */
	/*                                         */
	/*******************************************/
	template< typename V, typename A >
	class _data_< 4, V, A > : public _base_< 4, V, A >
	{
		template< int FM, typename FV, typename FA >
		friend class tensor;

		typedef _base_< 4, V, A > base;
		typedef typename base::value_type value_type;
		typedef typename base::size_type size_type;

		void _resize( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4 )
		{
			if( base::_ranks[ 0 ] != r1 || base::_ranks[ 1 ] != r2 || base::_ranks[ 2 ] != r3 || base::_ranks[ 3 ] != r4 )
			{
				base::_ranks[ 0 ] = r1;
				base::_ranks[ 1 ] = r2;
				base::_ranks[ 2 ] = r3;
				base::_ranks[ 3 ] = r4;
				base::_data.resize( r1 * r2 * r3 * r4 );
			}
		}
		void resize( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4 )
		{
			_resize( r1, r2, r3, r4 );
			base::_data.fill( );
		}
		value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4 )
		{
			return base::_data[ i1 + i2 * base::_ranks[ 0 ] + i3 * base::_ranks[ 0 ] * base::_ranks[ 1 ] + i4 * base::_ranks[ 0 ] * base::_ranks[ 1 ] * base::_ranks[ 2 ] ];
		}
		const value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4 ) const
		{
			return base::_data[ i1 + i2 * base::_ranks[ 0 ] + i3 * base::_ranks[ 0 ] * base::_ranks[ 1 ] + i4 * base::_ranks[ 0 ] * base::_ranks[ 1 ] * base::_ranks[ 2 ] ];
		}
		template< typename MV, typename MA >
		_data_< 4, V, A > fold( const size_type &m, const matrix< MV, MA > &mat ) const
		{
			_data_< 4, V, A > ret( base::_ranks[ 0 ], base::_ranks[ 1 ], base::_ranks[ 2 ], base::_ranks[ 3 ] );
			ret.base::_ranks[ m - 1 ] = mat.rows( );
			ret._resize( base::_ranks[ 0 ], base::_ranks[ 1 ], base::_ranks[ 2 ], base::_ranks[ 3 ] );
			ret._fold( m, mat );
			return ret;
		}
		template< typename AV, typename AA >
		_data_< 4, V, A >( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4, const array< AV, AA > &data )
		{
			_resize( r1, r2, r3, r4 );
			::std::copy( &data[ 0 ], &data[ 0 ] + data.size( ), &base::_data[ 0 ] );
		}
		_data_< 4, V, A >( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4 )
		{
			_resize( r1, r2, r3, r4 );
		}
		template< typename TV, typename TA, typename AA >
		_data_< 4, V, A >( const array< tensor< 3, TV, TA >, AA > &ts )
		{
			_resize( ts[ 0 ].rank( 1 ), ts[ 0 ].rank( 2 ), ts[ 0 ].rank( 3 ), ts.size( ) );
			for( size_type i = 0 ; i < ts.size( ) ; i ++ )
			{
				::std::copy( &ts[ i ][ 0 ], &ts[ i ][ 0 ] + ts[ 0 ].size( ), &base::_data[ i * ts[ 0 ].size( ) ] );
			}
		}
		_data_< 4, V, A >( )
		{
		}
	};

	/*******************************************/
	/*                                         */
	/*          for 5th order tensor           */
	/*                                         */
	/*******************************************/
	template< typename V, typename A >
	class _data_< 5, V, A > : public _base_< 5, V, A >
	{
		template< int FM, typename FV, typename FA >
		friend class tensor;

		typedef _base_< 5, V, A > base;
		typedef typename base::value_type value_type;
		typedef typename base::size_type size_type;

		void _resize( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4, const size_type &r5 )
		{
			if( base::_ranks[ 0 ] != r1 || base::_ranks[ 1 ] != r2 || base::_ranks[ 2 ] != r3 || base::_ranks[ 3 ] != r4 || base::_ranks[ 4 ] != r5 )
			{
				base::_ranks[ 0 ] = r1;
				base::_ranks[ 1 ] = r2;
				base::_ranks[ 2 ] = r3;
				base::_ranks[ 3 ] = r4;
				base::_ranks[ 4 ] = r5;
				base::_data.resize( r1 * r2 * r3 * r4 * r5 );
			}
		}
		void resize( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4, const size_type &r5 )
		{
			_resize( r1, r2, r3, r4, r5 );
			base::_data.fill( );
		}
		value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4, const size_type &i5 )
		{
			return base::_data[ i1 + i2 * base::_ranks[ 0 ] + i3 * base::_ranks[ 0 ] * base::_ranks[ 1 ] + i4 * base::_ranks[ 0 ] * base::_ranks[ 1 ] * base::_ranks[ 2 ] + i5 * base::_ranks[ 0 ] * base::_ranks[ 1 ] * base::_ranks[ 2 ] * base::_ranks[ 3 ] ];
		}
		const value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4, const size_type &i5 ) const
		{
			return base::_data[ i1 + i2 * base::_ranks[ 0 ] + i3 * base::_ranks[ 0 ] * base::_ranks[ 1 ] + i4 * base::_ranks[ 0 ] * base::_ranks[ 1 ] * base::_ranks[ 2 ] + i5 * base::_ranks[ 0 ] * base::_ranks[ 1 ] * base::_ranks[ 2 ] * base::_ranks[ 3 ] ];
		}
		template< typename MV, typename MA >
		_data_< 5, V, A > fold( const size_type &m, const matrix< MV, MA > &mat ) const
		{
			_data_< 5, V, A > ret( base::_ranks[ 0 ], base::_ranks[ 1 ], base::_ranks[ 2 ], base::_ranks[ 3 ], base::_ranks[ 4 ] );
			ret.base::_ranks[ m - 1 ] = mat.rows( );
			ret._resize( base::_ranks[ 0 ], base::_ranks[ 1 ], base::_ranks[ 2 ], base::_ranks[ 3 ], base::_ranks[ 4 ] );
			ret._fold( m, mat );
			return ret;
		}
		template< typename AV, typename AA >
		_data_< 5, V, A >( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4, const size_type &r5, const array< AV, AA > &data )
		{
			_resize( r1, r2, r3, r4, r5 );
			::std::copy( &data[ 0 ], &data[ 0 ] + data.size( ), &base::_data[ 0 ] );
		}
		_data_< 5, V, A >( const size_type &r1, const size_type &r2, const size_type &r3, const size_type &r4, const size_type &r5 )
		{
			_resize( r1, r2, r3, r4, r5 );
		}
		template< typename TV, typename TA, typename AA >
		_data_< 5, V, A >( const array< tensor< 4, TV, TA >, AA > &ts )
		{
			_resize( ts[ 0 ].rank( 1 ), ts[ 0 ].rank( 2 ), ts[ 0 ].rank( 3 ), ts[ 0 ].rank( 4 ), ts.size( ) );
			for( size_type i = 0 ; i < ts.size( ) ; i ++ )
			{
				::std::copy( &ts[ i ][ 0 ], &ts[ i ][ 0 ] + ts[ 0 ].size( ), &base::_data[ i * ts[ 0 ].size( ) ] );
			}
		}
		_data_< 5, V, A >( )
		{
		}
	};
}

/// @brief Tensor class
//! 
//! An N-th order tensor T has mode-ranks I_1, I_2, ..., I_N 
//! and belongs to the product space R^{I_1 \times I_2 \times ... \times I_N}.
//!
//! @param M  integer parameter to assign tensor's order (number of modes)
//! @param V  data type parameter used as tensor's element type
//! @param A  allocator type paramerter used by mist containers
//! 
template< int M, typename V, typename A = ::std::allocator< V > >
class tensor
{
	typedef matrix< V, A > matrix_type;
	typedef typename __tensor__::_data_< M, V, A > data_type;

public:
	typedef V value_type;   ///< @brief data type of tensor's elements
	typedef A allocator;    ///< @brief allocator type of mist containers
	typedef typename data_type::size_type size_type;   ///< @brief unsigned integer type used for contener size or data index (same as size_t)

private:
	data_type _d;

	bool _is_valid( const size_type &m ) const
	{
		return 0 < m && m <= mode( );
	}
	template< typename TV, typename TA >
	bool _is_same_size( const tensor< M, TV, TA > &t ) const 
	{
		for( size_type i = 1 ; i <= mode( ) ; i ++ )
		{
			if( rank( i ) != t.rank( i ) )
			{
				return false;
			}
		}
		return true;
	}
	template< typename MV, typename MA >
	tensor< M, V, A > _fold( const size_type &m, const matrix< MV, MA > &mat ) const
	{
		tensor< M, V, A > ret;
		ret._d = _d.fold( m, mat );
		return ret;
	}
	matrix_type _unfold( const size_type &m ) const
	{
		return _d.unfold( m );
	}
	template< typename MV, typename MA >
	tensor< M, V, A > _x( const size_type &m, const matrix< MV, MA > &mat ) const
	{
		return _fold( m, mat * _unfold( m ) );
	}

public:
	/// @brief return the number of tensor's modes
	const size_type mode( ) const
	{
		return _d.mode( );
	}
	/// @brief return a mode rank of the tensor for each mode
	//! 
	//! @param[in] mode  mode value to obtain the corresponding tensor's rank 
	//! @return          the tensor's rank corresponding to the mode 
	//! 
	const size_type rank( const size_type &mode ) const 
	{
		if( _is_valid( mode ) )
		{
			return _d.rank( mode );
		}
		else
		{
			::std::cerr << "can't obtain rank" << ::std::endl;
			return 0;
		}
	}
	/// @brief return the number of tensor's elements
	const size_type size( ) const
	{
		return _d.size( );
	}
	/// @brief return the i-th element of the tensor without depending on the number of modes
	//! 
	//! @param[in] i  index to specify tensor's element 
	//! @return       the i-th element of the tensor 
	//! 
	value_type &operator [ ]( const size_type &i )
	{
		return _d[ i ];
	}
	/// @brief return the i-th element of the tensor without depending on the number of modes
	//! 
	//! @param[in] i  index to specify tensor's element 
	//! @return       the i-th element of the tensor 
	//! 
	const value_type &operator [ ]( const size_type &i ) const 
	{
		return _d[ i ];
	}
	/// @brief return a matrix form of the tensor obtained by unfolding it for each mode
	//! 
	//! For a tensor T in R^{I_1 \times I_2 \times ... \times I_N}, 
	//! n-mode-unfolding means constructing a matrix T_n from T
	//! so that T_n consits of I_n dimensional column vectors 
	//! and the number of vectors is I_1 \times I_2 \times ... \times I_{n-1} \times I_{n+1} \times ... \times I_N. 
	//! 
	//! @param[in] mode  mode value to unfold the tensor 
	//! @return          the the matrix obtained by unfolding the tensor
	//! 
	matrix_type unfold( const size_type &mode ) const
	{
		if( _is_valid( mode ) )
		{
			return _unfold( mode );
		}
		else
		{
			::std::cerr << "can't unfold tensor" << ::std::endl;
			return matrix_type( );
		}
	}
	/// @brief return a mode-product between the tensor and a matrix
	//! 
	//! An n-mode-product between a tensor T and a matrix M is denoted as T \times_n M.
	//! Using n-mode-unfolding matrix T_n, elements of the mode-product are calculated by M T_n.
	//!
	//! @param[in] mode  mode value to determine the direction of multiplication
	//! @return          the mode-product
	//! 
	template< typename MV, typename MA >
	tensor< M, V, A > x( const size_type &mode, const matrix< MV, MA > &mat ) const
	{
		if( _is_valid( mode ) )
		{
			return _x( mode, mat );
		}
		else
		{
			::std::cerr << "can't obtain n-mode product" << ::std::endl;
			return tensor< M, V, A >( );
		}
	}
	void out( ::std::ostream &o ) const
	{
		_d.out( o );
	}
	/// @brief equivalence operator
	template< typename TV, typename TA >
	bool operator ==( const tensor< M, TV, TA > &t ) const 
	{
		if( !_is_same_size( t ) )
		{
			return false;
		}
		else
		{
			for( size_type i = 0 ; i < size( ) ; i ++ )
			{
				if( operator [ ]( i ) != t[ i ] )
				{
					return false;
				}
			}
			return true;
		}
	}
	/// @brief non-equivalence operator
	template< typename TV, typename TA >
	bool operator !=( const tensor< M, TV, TA > &t ) const 
	{
		return !operator ==( t );
	}
	/// @brief addision assignment operator to add a tensor to the tensor
	template< typename TV, typename TA >
	const tensor< M, V, A > &operator +=( const tensor< M, TV, TA > &t )
	{
		if( _is_same_size( t ) )
		{
			::std::transform( &operator [ ]( 0 ), &operator [ ]( 0 ) + size( ), &t[ 0 ], &operator [ ]( 0 ), ::std::plus< value_type >( ) );
		}
		else
		{
			::std::cerr << "can't add tensors" << ::std::endl;
		}
		return *this;
	}
	/// @brief subtraction assignment operator to subtract a tensor from the tensor
	template< typename TV, typename TA >
	const tensor< M, V, A > &operator -=( const tensor< M, TV, TA > &t )
	{
		if( _is_same_size( t ) )
		{
			::std::transform( &operator [ ]( 0 ), &operator [ ]( 0 ) + size( ), &t[ 0 ], &operator [ ]( 0 ), ::std::minus< value_type >( ) );
		}
		else
		{
			::std::cerr << "can't subtract tensors" << ::std::endl;
		}
		return *this;
	}
	/// @brief multiplication assignment operator to multiply the tensor by a value
	template< typename VV >
	const tensor< M, V, A > &operator *=( const VV &v )
	{
		::std::transform( &operator [ ]( 0 ), &operator [ ]( 0 ) + size( ), &operator [ ]( 0 ), ::std::bind2nd( ::std::multiplies< value_type >( ), v ) );
		return *this;
	}
	/// @brief division assignment operator to divide the tensor by a value
	template< typename VV >
	const tensor< M, V, A > &operator /=( const VV &v )
	{
		::std::transform( &operator [ ]( 0 ), &operator [ ]( 0 ) + size( ), &operator [ ]( 0 ), ::std::bind2nd( ::std::divides< value_type >( ), v ) );
		return *this;
	}
	/// @brief construct a tensor (default constructer)
	tensor< M, V, A >( ) : _d( )
	{
	}


	/*******************************************/
	/*                                         */
	/*          for 2nd order tensor           */
	/*                                         */
	/*******************************************/

	/// @brief resize the 2nd order tensor
	//! 
	//! @param[in] rank1  1st mode rank after this operation
	//! @param[in] rank2  2nd mode rank after this operation
	//! 
	void resize( const size_type &rank1, const size_type &rank2 )
	{
		_d.resize( rank1, rank2 );
	}
	/// @brief return a element of the 2nd order tensor for each pair of indices
	//! 
	//! @param[in] i1  index for the 1st mode
	//! @param[in] i2  index for the 2nd mode
	//! @return        the element corresponding to the indices
	//! 
	value_type &operator ( )( const size_type &i1, const size_type &i2 )
	{
		return _d( i1, i2 );
	}
	/// @brief return a element of the 2nd order tensor for each pair of indices
	//! 
	//! @param[in] i1  index for the 1st mode
	//! @param[in] i2  index for the 2nd mode
	//! @return        the element corresponding to the indices
	//! 
	const value_type &operator ( )( const size_type &i1, const size_type &i2 ) const 
	{
		return _d( i1, i2 );
	}
	/// @brief apply higher order singular value decomposition (HOSVD) to the 2nd order tensor
	//! 
	//! HOSVD decomposes a 2nd order tensor T as T = Z \times_1 U_1 \times_2 U_2.
	//! Here U_1 and U_2 are unitary matrices and a 2nd order tensor Z is called as a core tensor.
	//! Using 1-mode-unfolding matrix of T, this is described like SVD of a matrix as T_1 = U1 S U2^t
	//! Here S is a singular value matrix.
	//!
	//! @param[in,out] z   core tensor of the tensor
	//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
	//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
	//! 
	template< typename MV, typename MA >
	void hosvd( tensor< M, V, A > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2 ) const
	{
		matrix< MV, MA > s;
		svd( _unfold( 1 ), u1, s, u2 );
		z = _x( 1, u1.dagger( ) )._x( 2, u2 );
		u2 = u2.dagger( );
	}
	/// @brief construct a 2nd order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! @param[in] data   data to assign initial values to elements of the tensor
	//! 
	template< typename AV, typename AA >
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2, const array< AV, AA > &data ) : _d( rank1, rank2, data )
	{
	}
	/// @brief construct a 2nd order tensor from an image
	//! 
	//! Width and height of the image are assigned to the 1st and 2nd mode ranks respectively.
	//!
	//! @param[in] img  image to assign initial values to elements of the tensor
	//! 
	template< typename AV, typename AA >
	tensor< M, V, A >( const array2< AV, AA > &img ) : _d( img.width( ), img.height( ), img )
	{
	}
	/// @brief construct a 2nd order tensor from a matrix
	//! 
	//! Numbers of rows and cols of the matrix are assigned to the 1st and 2nd mode ranks respectively.
	//!
	//! @param[in] mat  matrix to assign initial values to elements of the tensor
	//! 
	template< typename MV, typename MA >
	tensor< M, V, A >( const matrix< MV, MA > &mat ) : _d( mat )
	{
	}
	/// @brief construct a 2nd order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! 
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2 ) : _d( rank1, rank2 )
	{
	}


	/*******************************************/
	/*                                         */
	/*          for 3rd order tensor           */
	/*                                         */
	/*******************************************/

	/// @brief resize the 3rd order tensor
	//! 
	//! @param[in] rank1  1st mode rank after this operation
	//! @param[in] rank2  2nd mode rank after this operation
	//! @param[in] rank3  3rd mode rank after this operation
	//! 
	void resize( const size_type &rank1, const size_type &rank2, const size_type &rank3 )
	{
		_d.resize( rank1, rank2, rank3 );
	}
	/// @brief return a element of the 3rd order tensor for each set of indices
	//! 
	//! @param[in] i1  index of the 1st mode
	//! @param[in] i2  index of the 2nd mode
	//! @param[in] i3  index of the 3rd mode
	//! @return        the element corresponding to the indices
	//! 
	value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3 )
	{
		return _d( i1, i2, i3 );
	}
	/// @brief return a element of the 3rd order tensor for each set of indices
	//! 
	//! @param[in] i1  index of the 1st mode
	//! @param[in] i2  index of the 2nd mode
	//! @param[in] i3  index of the 3rd mode
	//! @return        the element corresponding to the indices
	//! 
	const value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3 ) const 
	{
		return _d( i1, i2, i3 );
	}
	/// @brief apply higher order singular value decomposition (HOSVD) to the 3rd order tensor
	//! 
	//! HOSVD decomposes a 3rd order tensor T as T = Z \times_1 U_1 \times_2 U_2 \times_3 U_3.
	//! Here U_1, U_2, and U_3 are unitary matrices and a 3rd order tensor Z is called as a core tensor.
	//!
	//! @param[in,out] z   core tensor of the tensor
	//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
	//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
	//! @param[in,out] u3  unitary matrix corresponding to the 3rd mode
	//! 
	template< typename MV, typename MA >
	void hosvd( tensor< M, V, A > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2, matrix< MV, MA > &u3 ) const
	{
		matrix< MV, MA > s, tmp;
		svd( _unfold( 1 ), u1, s, tmp );
		svd( _unfold( 2 ), u2, s, tmp );
		svd( _unfold( 3 ), u3, s, tmp );
		z = _x( 1, u1.dagger( ) )._x( 2, u2.dagger( ) )._x( 3, u3.dagger( ) );
	}
	/// @brief construct a 3rd order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! @param[in] rank3  3rd mode rank
	//! @param[in] data   data to assign initial values to elements of the tensor
	//! 
	template< typename AV, typename AA >
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2, const size_type &rank3, const array< AV, AA > &data ) : _d( rank1, rank2, rank3, data )
	{
	}
	/// @brief construct a 3rd order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! @param[in] rank3  3rd mode rank
	//! 
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2, const size_type &rank3 ) : _d( rank1, rank2, rank3 )
	{
	}
	/// @brief construct a 3rd order tensor from an array of 2nd order tensors
	//! 
	//! @param[in] ts  array of 2nd order tensors
	//! 
	template< typename TV, typename TA, typename AA >
	tensor< M, V, A >( const array< tensor< 2, TV, TA >, AA > &ts ) : _d( ts )
	{
	}


	/*******************************************/
	/*                                         */
	/*          for 4th order tensor           */
	/*                                         */
	/*******************************************/

	/// @brief resize the 4th order tensor
	//! 
	//! @param[in] rank1  1st mode rank after this operation
	//! @param[in] rank2  2nd mode rank after this operation
	//! @param[in] rank3  3rd mode rank after this operation
	//! @param[in] rank4  4th mode rank after this operation
	//! 
	void resize( const size_type &rank1, const size_type &rank2, const size_type &rank3, const size_type &rank4 )
	{
		_d.resize( rank1, rank2, rank3, rank4 );
	}
	/// @brief return a element of the 4th order tensor for each set of indices
	//! 
	//! @param[in] i1  index of the 1st mode
	//! @param[in] i2  index of the 2nd mode
	//! @param[in] i3  index of the 3rd mode
	//! @param[in] i4  index of the 4th mode
	//! @return        the element corresponding to the indices
	//! 
	value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4 )
	{
		return _d( i1, i2, i3, i4 );
	}
	/// @brief return a element of the 4th order tensor for each set of indices
	//! 
	//! @param[in] i1  index of the 1st mode
	//! @param[in] i2  index of the 2nd mode
	//! @param[in] i3  index of the 3rd mode
	//! @param[in] i4  index of the 4th mode
	//! @return        the element corresponding to the indices
	//! 
	const value_type &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4 ) const 
	{
		return _d( i1, i2, i3, i4 );
	}
	/// @brief apply higher order singular value decomposition (HOSVD) to the 4th order tensor
	//! 
	//! HOSVD decomposes a 4th order tensor T as T = Z \times_1 U_1 \times_2 U_2 \times_3 U_3 \times_4 U_4.
	//! Here U_1, U_2, U_3, and U_4 are unitary matrices and a 4th order tensor Z is called as a core tensor.
	//!
	//! @param[in,out] z   core tensor of the tensor
	//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
	//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
	//! @param[in,out] u3  unitary matrix corresponding to the 3rd mode
	//! @param[in,out] u4  unitary matrix corresponding to the 4th mode
	//! 
	template< typename MV, typename MA >
	void hosvd( tensor< M, V, A > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2, matrix< MV, MA > &u3, matrix< MV, MA > &u4 ) const
	{
		matrix< MV, MA > s, tmp;
		svd( _unfold( 1 ), u1, s, tmp );
		svd( _unfold( 2 ), u2, s, tmp );
		svd( _unfold( 3 ), u3, s, tmp );
		svd( _unfold( 4 ), u4, s, tmp );
		z = _x( 1, u1.dagger( ) )._x( 2, u2.dagger( ) )._x( 3, u3.dagger( ) )._x( 4, u4.dagger( ) );
	}
	/// @brief construct a 4th order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! @param[in] rank3  3rd mode rank
	//! @param[in] rank4  4th mode rank
	//! @param[in] data   data to assign initial values to elements of the tensor
	//! 
	template< typename AV, typename AA >
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2, const size_type &rank3, const size_type &rank4, const array< AV, AA > &data ) : _d( rank1, rank2, rank3, rank4, data )
	{
	}
	/// @brief construct a 4th order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! @param[in] rank3  3rd mode rank
	//! @param[in] rank4  4th mode rank
	//! 
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2, const size_type &rank3, const size_type &rank4 ) : _d( rank1, rank2, rank3, rank4 )
	{
	}
	/// @brief construct a 4th order tensor from an array of 3rd order tensors
	//! 
	//! @param[in] ts  array of 3rd order tensors
	//! 
	template< typename TV, typename TA, typename AA >
	tensor< M, V, A >( const array< tensor< 3, TV, TA >, AA > &ts ) : _d( ts )
	{
	}


	/*******************************************/
	/*                                         */
	/*          for 5th order tensor           */
	/*                                         */
	/*******************************************/

	/// @brief resize the 5th order tensor
	//! 
	//! @param[in] rank1  1st mode rank after this operation
	//! @param[in] rank2  2nd mode rank after this operation
	//! @param[in] rank3  3rd mode rank after this operation
	//! @param[in] rank4  4th mode rank after this operation
	//! @param[in] rank5  5th mode rank after this operation
	//! 
	void resize( const size_type &rank1, const size_type &rank2, const size_type &rank3, const size_type &rank4, const size_type &rank5 )
	{
		_d.resize( rank1, rank2, rank3, rank4, rank5 );
	}
	/// @brief return a element of the 5th order tensor for each set of indices
	//! 
	//! @param[in] i1  index of the 1st mode
	//! @param[in] i2  index of the 2nd mode
	//! @param[in] i3  index of the 3rd mode
	//! @param[in] i4  index of the 4th mode
	//! @param[in] i5  index of the 5th mode
	//! @return        the element corresponding to the indices
	//! 
	tensor< M, V, A > &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4, const size_type &i5 )
	{
		return _d( i1, i2, i3, i4, i5 );
	}
	/// @brief return a element of the 5th order tensor for each set of indices
	//! 
	//! @param[in] i1  index of the 1st mode
	//! @param[in] i2  index of the 2nd mode
	//! @param[in] i3  index of the 3rd mode
	//! @param[in] i4  index of the 4th mode
	//! @param[in] i5  index of the 5th mode
	//! @return        the element corresponding to the indices
	//! 
	const tensor< M, V, A > &operator ( )( const size_type &i1, const size_type &i2, const size_type &i3, const size_type &i4, const size_type &i5 ) const 
	{
		return _d( i1, i2, i3, i4, i5 );
	}
	/// @brief apply higher order singular value decomposition (HOSVD) to the 5th order tensor
	//! 
	//! HOSVD decomposes a 5th order tensor T as T = Z \times_1 U_1 \times_2 U_2 \times_3 U_3 \times_4 U_4 \times_5 U_5.
	//! Here U_1, U_2, U_3, U_4, and U_5 are unitary matrices and a 5th order tensor Z is called as a core tensor.
	//!
	//! @param[in,out] z   core tensor of the tensor
	//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
	//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
	//! @param[in,out] u3  unitary matrix corresponding to the 3rd mode
	//! @param[in,out] u4  unitary matrix corresponding to the 4th mode
	//! @param[in,out] u5  unitary matrix corresponding to the 5th mode
	//! 
	template< typename MV, typename MA >
	void hosvd( tensor< M, V, A > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2, matrix< MV, MA > &u3, matrix< MV, MA > &u4, matrix< MV, MA > &u5 ) const
	{
		matrix< MV, MA > s, tmp;
		svd( _unfold( 1 ), u1, s, tmp );
		svd( _unfold( 2 ), u2, s, tmp );
		svd( _unfold( 3 ), u3, s, tmp );
		svd( _unfold( 4 ), u4, s, tmp );
		svd( _unfold( 5 ), u5, s, tmp );
		z = _x( 1, u1.dagger( ) )._x( 2, u2.dagger( ) )._x( 3, u3.dagger( ) )._x( 4, u4.dagger( ) )._x( 5, u5.dagger( ) );
	}
	/// @brief construct a 5th order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! @param[in] rank3  3rd mode rank
	//! @param[in] rank4  4th mode rank
	//! @param[in] rank5  5th mode rank
	//! @param[in] data   data to assign initial values to elements of the tensor
	//! 
	template< typename AV, typename AA >
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2, const size_type &rank3, const size_type &rank4, const size_type &rank5, const array< AV, AA > &data ) : _d( rank1, rank2, rank3, rank4, rank5, data )
	{
	}
	/// @brief construct a 5th order tensor
	//! 
	//! @param[in] rank1  1st mode rank
	//! @param[in] rank2  2nd mode rank
	//! @param[in] rank3  3rd mode rank
	//! @param[in] rank4  4th mode rank
	//! @param[in] rank5  5th mode rank
	//! 
	tensor< M, V, A >( const size_type &rank1, const size_type &rank2, const size_type &rank3, const size_type &rank4, const size_type &rank5 ) : _d( rank1, rank2, rank3, rank4, rank5 )
	{
	}
	/// @brief construct a 5th order tensor from an array of 4th order tensors
	//! 
	//! @param[in] ts  array of 4th order tensors
	//! 
	template< typename TV, typename TA, typename AA >
	tensor< M, V, A >( const array< tensor< 4, TV, TA >, AA > &ts ) : _d( ts )
	{
	}
};


/// @brief addition operator to calculate summation of two tensors
//! 
//! @param[in] t1  1st tensor
//! @param[in] t2  2nd tensor
//! @return        summation of the 1st tensor and the 2nd tensor
//! 
template< int M, typename V, typename A >
inline tensor< M, V, A > operator +( const tensor< M, V, A > &t1, const tensor< M, V, A > &t2 )
{
	return tensor< M, V, A >( t1 ) += t2;
}

/// @brief subtraction operator to calculate difference betwwen two tensors
//! 
//! @param[in] t1  1st tensor
//! @param[in] t2  2nd tensor
//! @return        difference between the 1st tensor and the 2nd tensor
//! 
template< int M, typename V, typename A >
inline tensor< M, V, A > operator -( const tensor< M, V, A > &t1, const tensor< M, V, A > &t2 )
{
	return tensor< M, V, A >( t1 ) -= t2;
}

/// @brief multiplication operator to calculate product of a tensor and a value
//! 
//! @param[in] t  tensor
//! @param[in] v  value
//! @return       product of the tensor and the value
//! 
template< int M, typename V, typename A >
inline tensor< M, V, A > operator *( const tensor< M, V, A > &t, const typename type_trait< V >::value_type &v )
{
	return tensor< M, V, A >( t ) *= v;
}

/// @brief multiplication operator to calculate product of a value and a tensor
//! 
//! @param[in] v  value
//! @param[in] t  tensor
//! @return       product of the value and the tensor
//! 
template< int M, typename V, typename A >
inline tensor< M, V, A > operator *( const typename type_trait< V >::value_type &v, const tensor< M, V, A > &t )
{
	return tensor< M, V, A >( t ) *= v;
}

/// @brief division operator to calculate quotient of a tensor and a value
//! 
//! @param[in] t  tensor
//! @param[in] v  value
//! @return       quotient of the tensor and the value
//! 
template< int M, typename V, typename A >
inline tensor< M, V, A > operator /( const tensor< M, V, A > &t, const typename type_trait< V >::value_type &v )
{
	return tensor< M, V, A >( t ) /= v;
}

/// @brief output stream operator
//! 
//! @param[in,out] o  output stream
//! @param[in]     t  tensor
//! @return           output stream
//! 
template < int M, typename V, typename A >
inline ::std::ostream &operator <<( ::std::ostream &o, const tensor< M, V, A > &t )
{
	t.out( o );	
	return o;
}


	/*******************************************/
	/*                                         */
	/*          for 2nd order tensor           */
	/*                                         */
	/*******************************************/

/// @brief apply higher order singular value decomposition (HOSVD) to a 2nd order tensor
//! 
//! HOSVD decomposes a 2nd order tensor T as T = Z \times_1 U_1 \times_2 U_2.
//! Here U_1 and U_2 are unitary matrices and a 2nd order tensor Z is called as a core tensor.
//! Using 1-mode-unfolding matrix of T, this is described like SVD of a matrix as T_1 = U_1 S U_2^t
//! Here S is a singular value matrix.
//!
//! @param[in]     t   tensor to be decomposed
//! @param[in,out] z   core tensor of the tensor
//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
//! 
template< typename TV, typename TA, typename MV, typename MA >
inline void hosvd( const tensor< 2, TV, TA > &t, tensor< 2, TV, TA > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2 )
{
	t.hosvd( z, u1, u2 );
}


	/*******************************************/
	/*                                         */
	/*          for 3rd order tensor           */
	/*                                         */
	/*******************************************/

/// @brief apply higher order singular value decomposition (HOSVD) to a 3rd order tensor
//! 
//! HOSVD decomposes a 3rd order tensor T as T = Z \times_1 U_1 \times_2 U_2 \times_3 U_3.
//! Here U_1, U_2, and U_3 are unitary matrices and a 3rd order tensor Z is called as a core tensor.
//!
//! @param[in]     z   tensorto be decomposed
//! @param[in,out] z   core tensor of the tensor
//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
//! @param[in,out] u3  unitary matrix corresponding to the 3rd mode
//! 
template< typename TV, typename TA, typename MV, typename MA >
inline void hosvd( const tensor< 3, TV, TA > &t, tensor< 3, TV, TA > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2, matrix< MV, MA > &u3 )
{
	t.hosvd( z, u1, u2, u3 );
}


	/*******************************************/
	/*                                         */
	/*          for 4th order tensor           */
	/*                                         */
	/*******************************************/

/// @brief apply higher order singular value decomposition (HOSVD) to a 4th order tensor
//! 
//! HOSVD decomposes a 4th order tensor T as T = Z \times_1 U_1 \times_2 U_2 \times_3 U_3 \times_4 U_4.
//! Here U_1, U_2, U_3 and U_4 are unitary matrices and a 4th order tensor Z is called as a core tensor.
//!
//! @param[in]     z   tensorto be decomposed
//! @param[in,out] z   core tensor of the tensor
//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
//! @param[in,out] u3  unitary matrix corresponding to the 3rd mode
//! @param[in,out] u4  unitary matrix corresponding to the 4th mode
//! 
template< typename TV, typename TA, typename MV, typename MA >
inline void hosvd( const tensor< 4, TV, TA > &t, tensor< 4, TV, TA > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2, matrix< MV, MA > &u3, matrix< MV, MA > &u4 )
{
	t.hosvd( z, u1, u2, u3, u4 );
}


	/*******************************************/
	/*                                         */
	/*          for 5th order tensor           */
	/*                                         */
	/*******************************************/

/// @brief apply higher order singular value decomposition (HOSVD) to a 5th order tensor
//! 
//! HOSVD decomposes a 5th order tensor T as T = Z \times_1 U_1 \times_2 U_2 \times_3 U_3 \times_4 U_4 \times_5 U_5.
//! Here U_1, U_2, U_3 and U_4 are unitary matrices and a 5th order tensor Z is called as a core tensor.
//!
//! @param[in]     z   tensorto be decomposed
//! @param[in,out] z   core tensor of the tensor
//! @param[in,out] u1  unitary matrix corresponding to the 1st mode
//! @param[in,out] u2  unitary matrix corresponding to the 2nd mode
//! @param[in,out] u3  unitary matrix corresponding to the 3rd mode
//! @param[in,out] u4  unitary matrix corresponding to the 4th mode
//! @param[in,out] u5  unitary matrix corresponding to the 5th mode
//! 
template< typename TV, typename TA, typename MV, typename MA >
inline void hosvd( const tensor< 5, TV, TA > &t, tensor< 5, TV, TA > &z, matrix< MV, MA > &u1, matrix< MV, MA > &u2, matrix< MV, MA > &u3, matrix< MV, MA > &u4, matrix< MV, MA > &u5 )
{
	t.hosvd( z, u1, u2, u3, u4, u5 );
}


// closing of mist namespace
_MIST_END

#endif // __INCLUDE_MIST_TENSOR__