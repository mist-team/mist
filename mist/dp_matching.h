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

/// @file mist/dp_matching.h
//!
//! @brief DP�}�b�`���O�̂��߂̃��C�u����
//!

#ifndef __INCLUDE_DP_MATCHING_H__
#define __INCLUDE_DP_MATCHING_H__

#include <mist/mist.h>
#include <mist/config/color.h>
#include <vector>
#include <functional>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


namespace __dp__
{
	// �v�f�Ԃ�2�拗����Ԃ��t�@���N�^�i�C���X�^���X�̓R���X�g���N�^�̃f�t�H���g�����ƂȂ�j
	template< typename Type >
	struct square_error : public std::binary_function< Type, Type, double >
	{
		double operator( )( const Type &v0, const Type &v1 ) const
		{
			return ( static_cast< double >( v0 ) - v1 ) * ( static_cast< double >( v0 ) - v1 );
		}
	};

	// �v�f�Ԃ�2�拗����Ԃ��t�@���N�^�i�C���X�^���X�̓R���X�g���N�^�̃f�t�H���g�����ƂȂ�j
	template< typename Type >
	struct square_error< mist::rgb< Type > > : public std::binary_function< mist::rgb< Type >, mist::rgb< Type >, double >
	{
		double operator( )( const mist::rgb< Type > &v0, const mist::rgb< Type > &v1 ) const
		{
			return ( static_cast< double >( v0.r ) - v1.r ) * ( static_cast< double >( v0.r ) - v1.r ) + ( static_cast< double >( v0.g ) - v1.g ) * ( static_cast< double >( v0.g ) - v1.g ) + ( static_cast< double >( v0.b ) - v1.b ) * ( static_cast< double >( v0.b ) - v1.b );
		}
	};
}

// �Ή��t�����ʂ��i�[���邽�߂̍\����
class dp_pair
{
	size_t a_; 
	size_t b_;

public:

	dp_pair( )
	{
	}

	dp_pair( const size_t a, const size_t b ) : a_( a ), b_( b )
	{
	}

	size_t a( ) const
	{
		return a_;
	}

	size_t b( ) const
	{
		return b_;
	}
};

inline bool operator !=( const dp_pair &p0, const dp_pair &p1 )
{
	return ( ( p0.a( ) != p1.a( ) ) || ( p0.b( ) != p1.b( ) ) );
}

inline std::ostream &operator <<( std::ostream &out, const dp_pair &in )
{
	return ( out << "( " << in.a( ) << ", " << in.b( ) << " )" );
}


//! @addtogroup dp_matching_group DP�}�b�`���O
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/dp_matching.h>
//! @endcode
//!
//!  @{



/// @brief DP�}�b�`���O���s�����߂̃N���X
//!
//! �C�ӂ̃T�C�Y�C�f�[�^�^��1�����f�[�^���m�̑Ή��t��
//!
//! @attention �v�f�Ԃ̋�����Ԃ��֐���std::binary_function����p�������t�@���N�^�̌`���ŗ^����K�v������D
//! @attention �Z�p�^�Cmist::rgb< �Z�p�^ >�ɑ΂��ẮA�f�t�H���g��2�拗����Ԃ��t�@���N�^��p�ӂ��Ă���D
//! 
//! @param Value_type  �c ���͂̃f�[�^�^�i double �� mist::rgb< unsigned char > �Ȃ� �j
//! @param Functor     �c �v�f�Ԃ̋�����Ԃ��t�@���N�^( �f�t�H���g�� __dp__::square_error< Value_type > )
//!
//! @code �g�p��
//! // �Ή��t������2�̃p�^�[����p��
//! char data0[ ] = { 'a', 'b', 'a', 'a', 'a', 'b', 'b', 'b', 'c', 'c' };
//!	char data1[ ] = { 'a', 'a', 'a', 'b', 'a', 'b', 'b', 'c', 'c', 'c' };
//! mist::array< char > pattern0( 10 ), pattern1( 10 );
//! for( size_t i = 0 ; i < pattern0.size( ) ; i ++ )
//! {
//!		pattern0[ i ] = data0[ i ];
//! 	pattern1[ i ] = data1[ i ];
//!	}
//!
//! // �Ή��t��
//! mist::dp_matching< char > dm0( pattern0, pattern1, 1.0, 2.0, 1.0 );
//!
//! // ���ʂ̕\��
//! std::cout << dm0.path( ) << std::endl;
//! std::cout << dm0.distance( ) << std::endl;
//! 
//! // �d�݂�ύX���čēx�Ή��t��
//! dm0.weights( 1.0, 1.0, 1.0 );
//!
//! // ���ʂ̕\��
//! std::cout << dm0.path( ) << std::endl;
//! std::cout << dm0.distance( ) << std::endl;
//!
//! // �����֐����w�肵�đΉ��t��
//! mist::dp_matching< char, my_distance > dm1( pattern0, pattern1, 1.0, 2.0, 1.0, my_distance( ) );
//!
//! // ���ʂ̕\��
//! std::cout << dm1.path( ) << std::endl;
//! std::cout << dm1.distance( ) << std::endl;
//! @endcode
//!
//! �����ŁAmy_distance�͗Ⴆ�Έȉ��̂悤�ɒ�`�ł���D
//!
//! @code �t�@���N�^�̗�
//1 #include <functional>
//! struct my_distance : public std::binary_function< char, char, double >
//! {
//! 	int operator ( )( const char v0, const char v1 ) const
//! 	{
//! 		return std::abs( static_cast< double >( v0 ) - v1 );
//! 	}
//! };
//! @endcode
//!
template< typename Value_type, typename Functor = __dp__::square_error< Value_type > >
class dp_matching
{
	typedef typename Functor::result_type	distance_type; ///< @brief �p�^�[���Ԃ̋�����\������^�istd::binary_function< Arg1, Arg2, Res >::result_type, ���̏ꍇ�� Res �j

	mist::array< Value_type >	pattern_a_;		///< @brief ���̓p�^�[��A
	mist::array< Value_type >	pattern_b_;		///< @brief ���̓p�^�[��B
	mist::array< dp_pair >		path_;			///< @brief ������p�X
	distance_type				distance_;		///< @brief ����ꂽ�p�X��ʂ�ꍇ�̃p�^�[���Ԃ̋���
	double						weight_0_;		///< @brief �������̏d�݁i�p�^�[��A�݂̂𓮂����ꍇ�j
	double						weight_1_;		///< @brief �΂ߕ����̏d�݁i���p�^�[���Ƃ��ɓ������ꍇ�j
	double						weight_2_;		///< @brief �c�����̏d�݁i�p�^�[��B�݂̂𓮂����ꍇ�j
	Functor						func_;			///< @brief �p�^�[���̗v�f�Ԃ̋�����Ԃ��t�@���N�^
	
	distance_type __distance( const size_t i0, const size_t i1 ) const
	{
		return func_( pattern_a_[ i0 ], pattern_b_[ i1 ] );
	}

	void __find_path( )
	{
		mist::array2< distance_type >	distance_array( pattern_a_.size( ), pattern_b_.size( ) );
		mist::array2< size_t >			path_array( pattern_a_.size( ), pattern_b_.size( ) );
		distance_array( 0, 0 ) = __distance( 0, 0 );
		for( size_t i = 1 ; i < distance_array.width( ) ; i ++ )
		{
			distance_array( i, 0 ) = static_cast< distance_type >( distance_array( i - 1, 0 ) + __distance( i, 0 ) * weight_0_ );
			path_array( i, 0 ) = 0;
		}
		for( size_t j = 1 ; j < distance_array.height( ) ; j ++ )
		{
			distance_array( 0, j ) = static_cast< distance_type >( distance_array( 0, j - 1 ) + __distance( 0, j ) * weight_2_ );
			path_array( 0, j ) = 2;
			for( size_t i = 1 ; i < distance_array.width( ) ; i ++ )
			{
				const distance_type distance	= __distance( i, j );
				const distance_type distance_0	= static_cast< distance_type >( distance_array( i - 1, j )		+ distance * weight_0_ );
				const distance_type distance_1	= static_cast< distance_type >( distance_array( i - 1, j - 1 )	+ distance * weight_1_ );
				const distance_type distance_2	= static_cast< distance_type >( distance_array( i, j - 1 )		+ distance * weight_2_ );
				path_array( i, j ) = ( distance_0 < distance_1 ) ? ( ( distance_0 < distance_2 ) ? 0 : 2 ) : ( ( distance_1 < distance_2 ) ? 1 : 2 );
				switch( path_array( i, j ) )
				{
					case 0:
						distance_array( i, j )	= distance_0;
						break;
					case 1:
						distance_array( i, j )	= distance_1;
						break;
					case 2:
						distance_array( i, j )	= distance_2;
						break;
					default:
						break;
				}
			}
		}
		distance_ = distance_array( distance_array.width( ) - 1, distance_array.height( ) - 1 );
		std::vector< dp_pair > path;
		path.reserve( pattern_a_.size( ) );
		size_t mi = pattern_a_.size( ) - 1;
		size_t mj = pattern_b_.size( ) - 1;
		path.push_back( dp_pair( mi, mj ) );
		while( dp_pair( mi, mj ) != dp_pair( 0, 0 ) )
		{
			switch( path_array( mi, mj ) )
			{
				case 0:
					path.push_back( dp_pair( -- mi, mj ) );
					break;
				case 1:
					path.push_back( dp_pair( -- mi, -- mj ) );
					break;
				case 2:
					path.push_back( dp_pair( mi, -- mj ) );
					break;
				default:
					break;
			}
		}
		path_.resize( path.size( ) );
		for( size_t i = 0 ; i < path_.size( ) ; i ++ )
		{
			path_[ i ] = path[ path_.size( ) - 1 - i ];
		}
	}

public:

	/// @brief �R���X�g���N�^
	//! 
	//! @param[in] pattern_a �c ���̓p�^�[��A
	//! @param[in] pattern_b �c ���̓p�^�[��B
	//! @param[in] weight_0  �c �������d��
	//! @param[in] weight_1  �c �΂ߕ����d��
	//! @param[in] weight_2  �c �c�����d��
	//! @param[in] func      �c �v�f�Ԃ̋�����Ԃ��t�@���N�^�i�f�t�H���g�l�F__dp__::square_error< Value_type >( )�j
	//!
	dp_matching( const mist::array< Value_type > &pattern_a, const mist::array< Value_type > &pattern_b, const double weight_0, const double weight_1, const double weight_2, const Functor func = __dp__::square_error< Value_type >( ) )		
		: pattern_a_( pattern_a ), pattern_b_( pattern_b ), weight_0_( weight_0 ), weight_1_( weight_1 ), weight_2_( weight_2 ), func_( func )
	{
		__find_path( );
	}

	/// @brief �d�݂�ω������đΉ��t������蒼��
	//!
	//! @param[in] weight_0  �c �������d��
	//! @param[in] weight_1  �c �΂ߕ����d��
	//! @param[in] weight_2  �c �c�����d��
	//!
	void weights( const double weight_0, const double weight_1, const double weight_2 )
	{
		weight_0_ = weight_0;
		weight_1_ = weight_1;
		weight_2_ = weight_2;
		__find_path( );
	}

	/// @brief �Ή��t����ꂽ�p�X��Ԃ�
	//!
	//! @return �Ή��t����ꂽ�p�X
	//!
	const mist::array< dp_pair > &path( ) const
	{
		return path_;
	}

	/// @brief �Ή��t����ꂽ�p�X�̒�����Ԃ�
	//!
	//! @return �p�X�̒���
	//!
	size_t length( ) const
	{
		return path_.size( );
	}

	/// @brief �Ή��t����ꂽ�p�X��ʂ�ꍇ�̃p�^�[���Ԃ̋�����Ԃ�
	//!
	//! @return �p�^�[���Ԃ̋���
	//!
	distance_type distance( ) const
	{
		return distance_;
	}
};


/// @}
//  DP�}�b�`���O�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_DP_MATCHING_H__
