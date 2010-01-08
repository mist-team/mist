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

/// @file mist/integral_image.h
//!
//! @brief Integral Image �̂��߂̃��C�u����
//!

#ifndef __INCLUDE_INTEGRAL_IMAGE_H__
#define __INCLUDE_INTEGRAL_IMAGE_H__

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

// ���͂̃f�[�^�^�����f�l�̑��a�̃f�[�^�^�����肷�邽�߂̃N���X
namespace __integral_image__
{
	template< class T, bool Is_decimal, bool Is_color >	struct integral_type					{ typedef typename T::template rebind< double >::other		type; };
	template< class T >									struct integral_type< T, false, true >	{ typedef typename T::template rebind< ptrdiff_t >::other	type; };
	template< class T >									struct integral_type< T, true,  false >	{ typedef double			type; };
	template< class T >									struct integral_type< T, false, false >	{ typedef ptrdiff_t			type; };
}

/// @brief 1�����̐ϕ��摜��ێ����C�C�ӂ̋�`�̈�̉�f�l�̑��a�������Ɍv�Z���邽�߂̃N���X
//!
//! ��f��n�ɑ΂���O(n)�̑O�v�Z���s�����Ƃɂ��C�C�ӂ̋�`�̈�̉�f�l�̑��a��O(1)�ŎZ�o����
//! ����̉摜�z�񒆂̕����̈قȂ��`�̈�ɑ΂��āA���a���v�Z����K�v������ꍇ�ȂǂɗL��
//!
//! mist::array, mist::array1, mist::array2, mist::array3�ɑΉ��D
//! @attention ���͂̃f�[�^�^�́A�Z�p�^�Cmist::rgb< �Z�p�^ >�̂ݑΉ��D
//! @attention �������f�l�̑��a�̃f�[�^�^�́C���͂̃f�[�^�^�Ɉˑ����ĕω��D
//! @attention ���͌^�F������������ �� ��f�l�̑��a�^�Fsize_t
//! @attention ���͌^�F�����t������ �� ��f�l�̑��a�^�Fint
//! @attention ���͌^�F���������_   �� ��f�l�̑��a�^�Fdouble
//! @attention mist::rgb< ���͌^ >  �� mist::rgb< ��f�l�̑��a�^ >
//!
//! ���ꉻ�̂��߂̃N���X�ł���O������͗��p���Ȃ�
//!
template< typename Array >
class integral_image
{
};


//! @addtogroup integral_image_group Integral Image
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/integral_image.h>
//! @endcode
//!
//!  @{



/// @brief 1�����̐ϕ��摜��ێ����C�C�ӂ̋�`�̈�̉�f�l�̑��a�������Ɍv�Z���邽�߂̃N���X
//!
//! ��f��n�ɑ΂���O(n)�̑O�v�Z���s�����Ƃɂ��C�C�ӂ̋�`�̈�̉�f�l�̑��a��O(1)�ŎZ�o����
//! ����̉摜�z�񒆂̕����̈قȂ��`�̈�ɑ΂��āA���a���v�Z����K�v������ꍇ�ȂǂɗL��
//!
//! mist::array, mist::array1, mist::array2, mist::array3�ɑΉ��D
//! @attention ���͂̃f�[�^�^�́A�Z�p�^�Cmist::rgb< �Z�p�^ >�̂ݑΉ��D
//! @attention �������f�l�̑��a�̃f�[�^�^�́C���͂̃f�[�^�^�Ɉˑ����ĕω��D
//! @attention ���͌^�F������������ �� ��f�l�̑��a�^�Fsize_t
//! @attention ���͌^�F�����t������ �� ��f�l�̑��a�^�Fint
//! @attention ���͌^�F���������_   �� ��f�l�̑��a�^�Fdouble
//! @attention mist::rgb< ���͌^ >  �� mist::rgb< ��f�l�̑��a�^ >
//! 
//! @param T         �c ���͂̃f�[�^�^�i double �� mist::rgb< unsigned char > �Ȃ� �j
//! @param Allocator �c MIST�R���e�i�Ŏg�p����A���P�[�^
//!
//! @code �g�p��
//! // �摜�z��i1-3�����j�̗p��
//! mist::array2< unsigned char > img;
//! mist::read_bmp( img, "img.bmp" );
//! 
//! // Integral Image �̍\�z�FO(n) �i�e���v���[�g�̈����Ƃ��ē��͔z��̌^���w��j
//! mist::integral_image< mist::array2< unsigned char > > i_img( img );
//!
//! // �n�_( 100, 200 ), �T�C�Y( 50, 60 )�̗̈�̉�f�l�̑��a���o�́FO(1)
//! std::cout << i_img( 100, 200, 50, 60 ) << std::endl;
//!
//! // �n�_( 200, 100 ), �T�C�Y( 60, 50 )�̗̈�̉�f�l�̑��a���o�́FO(1)
//! std::cout << i_img( 200, 100, 60, 50 ) << std::endl;
//! @endcode
//!
template< typename T, typename Allocator >
class integral_image< array< T, Allocator > >
{
public:
	typedef typename __integral_image__::integral_type< T, is_float< T >::value, is_color< T >::value >::type value_type;
	typedef array< value_type > integral_image_type;
	typedef array< T, Allocator > image_type;
	typedef typename image_type::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename image_type::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	integral_image_type integral_;

public:

	/// @brief �C�ӂ̋�`�̈���̉�f�̑��a��Ԃ��FO(1)
	//! 
	//! @param[in] begin �c �n�_
	//! @param[in] size  �c �T�C�Y
	//!
	//! @return ��f�l�̑��a
	//!
	value_type operator ( )( const size_type begin, const size_type size ) const
	{
		return( integral_( begin + size ) - integral_( begin ) );
	}

	/// @brief �C�ӂ̈ʒu�̐ϕ��l��Ԃ�
	//! 
	//! @param[in] i �c i�����̎n�_
	//!
	//! @return ��f�l�̑��a
	//!
	value_type operator ( )( const size_type i ) const
	{
		return( integral_( i + 1 ) );
	}

	/// @brief �S�Ẳ�f�l�̑��a��Ԃ��FO(1)
	//! 
	//! @return ��f�l�̑��a
	//!
	value_type overall( ) const 
	{
		return( integral_[ integral_.size( ) - 1 ] );
	}

	/// @brief �摜�z��̃T�C�Y��Ԃ�
	//! 
	//! @return �T�C�Y
	//!
	size_type size( ) const
	{
		return( integral_.size( ) - 1 );
	}

	/// @brief Integral Image �̍\�z�FO(n)
	//! 
	//! @param[in] in �c �摜�z��
	//!
	void construct_integral_array( const image_type &in )
	{
		integral_.resize( in.size( ) + 1 );	
		for( size_type i = 1 ; i < integral_.width( ) ; i ++ )
		{
			integral_[ i ] = in[ i - 1 ] + integral_[ i - 1 ];
		}
	}

	/// @brief �R���X�g���N�^
	//! 
	//! @param[in] in �c �摜�z��
	//!
	integral_image( const image_type &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	//!
	integral_image( ) : integral_( )
	{
	}
};


/// @brief 2�����̐ϕ��摜��ێ����C�C�ӂ̋�`�̈�̉�f�l�̑��a�������Ɍv�Z���邽�߂̃N���X
//!
//! ��f��n�ɑ΂���O(n)�̑O�v�Z���s�����Ƃɂ��C�C�ӂ̋�`�̈�̉�f�l�̑��a��O(1)�ŎZ�o����
//! ����̉摜�z�񒆂̕����̈قȂ��`�̈�ɑ΂��āA���a���v�Z����K�v������ꍇ�ȂǂɗL��
//!
//! mist::array, mist::array1, mist::array2, mist::array3�ɑΉ��D
//! @attention ���͂̃f�[�^�^�́A�Z�p�^�Cmist::rgb< �Z�p�^ >�̂ݑΉ��D
//! @attention �������f�l�̑��a�̃f�[�^�^�́C���͂̃f�[�^�^�Ɉˑ����ĕω��D
//! @attention ���͌^�F������������ �� ��f�l�̑��a�^�Fsize_t
//! @attention ���͌^�F�����t������ �� ��f�l�̑��a�^�Fint
//! @attention ���͌^�F���������_   �� ��f�l�̑��a�^�Fdouble
//! @attention mist::rgb< ���͌^ >  �� mist::rgb< ��f�l�̑��a�^ >
//! 
//! @param T         �c ���͂̃f�[�^�^�i double �� mist::rgb< unsigned char > �Ȃ� �j
//! @param Allocator �c MIST�R���e�i�Ŏg�p����A���P�[�^
//!
//! @code �g�p��
//! // �摜�z��i1-3�����j�̗p��
//! mist::array2< unsigned char > img;
//! mist::read_bmp( img, "img.bmp" );
//! 
//! // Integral Image �̍\�z�FO(n) �i�e���v���[�g�̈����Ƃ��ē��͔z��̌^���w��j
//! mist::integral_image< mist::array2< unsigned char > > i_img( img );
//!
//! // �n�_( 100, 200 ), �T�C�Y( 50, 60 )�̗̈�̉�f�l�̑��a���o�́FO(1)
//! std::cout << i_img( 100, 200, 50, 60 ) << std::endl;
//!
//! // �n�_( 200, 100 ), �T�C�Y( 60, 50 )�̗̈�̉�f�l�̑��a���o�́FO(1)
//! std::cout << i_img( 200, 100, 60, 50 ) << std::endl;
//! @endcode
//!
template< typename T, typename Allocator >
class integral_image< array2< T, Allocator > >
{
public:
	typedef typename __integral_image__::integral_type< T, is_float< T >::value, is_color< T >::value >::type value_type;
	typedef array2< value_type > integral_image_type;
	typedef array2< T, Allocator > image_type;
	typedef typename image_type::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename image_type::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	integral_image_type integral_;

public:
	/// @brief �C�ӂ̋�`�̈���̉�f�̑��a��Ԃ��FO(1)
	//! 
	//! @param[in] begin_i �c i�����̎n�_
	//! @param[in] begin_j �c j�����̎n�_
	//! @param[in] width   �c i�����̃T�C�Y
	//! @param[in] height  �c i�����̃T�C�Y
	//!
	//! @return ��f�l�̑��a
	//!
	value_type operator ( )( const size_type begin_i, const size_type begin_j, const size_type width, const size_type height ) const
	{
		return( integral_( begin_i + width, begin_j + height ) + integral_( begin_i, begin_j ) - integral_( begin_i + width, begin_j ) - integral_( begin_i, begin_j + height ) );
	}

	/// @brief �C�ӂ̈ʒu�̐ϕ��l��Ԃ�
	//! 
	//! @param[in] i �c i�����̎n�_
	//! @param[in] j �c j�����̎n�_
	//!
	//! @return ��f�l�̑��a
	//!
	value_type operator ( )( const size_type i, const size_type j ) const
	{
		return( integral_( i + 1, j + 1 ) );
	}

	/// @brief �S�Ẳ�f�l�̑��a��Ԃ��FO(1)
	//! 
	//! @return ��f�l�̑��a
	//!
	value_type overall( ) const 
	{
		return( integral_[ integral_.size( ) - 1 ] );
	}

	/// @brief �摜�z���i�����̃T�C�Y��Ԃ�
	//! 
	//! @return i�����̃T�C�Y
	//!
	size_type size1( ) const
	{
		return( integral_.width( ) - 1 );
	}

	/// @brief �摜�z���j�����̃T�C�Y��Ԃ�
	//! 
	//! @return j�����̃T�C�Y
	//!
	size_type size2( ) const
	{
		return( integral_.height( ) - 1 );
	}

	/// @brief �摜�z���i�����̃T�C�Y��Ԃ�
	//! 
	//! @return i�����̃T�C�Y
	//!
	size_type width( ) const
	{
		return( size1( ) );
	}

	/// @brief �摜�z���j�����̃T�C�Y��Ԃ�
	//! 
	//! @return j�����̃T�C�Y
	//!
	size_type height( ) const
	{
		return( size2( ) );
	}

	/// @brief �摜�z��̃T�C�Y��Ԃ�
	//! 
	//! @return �T�C�Y
	//!
	size_type size( ) const
	{
		return( width( ) * height( ) );
	}

	/// @brief Integral Image �̍\�z�FO(n)
	//! 
	//! @param[in] in �c �摜�z��
	//!
	void construct_integral_array( const image_type &in )
	{ 
		integral_.resize( in.width( ) + 1, in.height( ) + 1 );	

#if 0
		int iw       = static_cast< int >( in.width( ) );
		int ih       = static_cast< int >( in.height( ) );
		size_type ow = integral_.width( );

		#pragma omp parallel for firstprivate( iw, ih ) schedule( guided )
		for( int j = 0 ; j < ih ; j++ )
		{
			typename image_type::const_pointer    ip = &in( 0, j );
			typename integral_image_type::pointer op = &integral_( 1, j + 1 );

			op[ 0 ] = ip[ 0 ];
			for( int i = 1 ; i < iw ; i++ )
			{
				op[ i ] = op[ i - 1 ] + ip[ i ];
			}
		}

		#pragma omp parallel for firstprivate( iw, ih, ow ) schedule( guided )
		for( int i = 0 ; i < iw ; i++ )
		{
			typename integral_image_type::pointer    op = &integral_( i + 1, 1 );
			typename integral_image_type::value_type ov = *op;

			op += ow;

			for( int j = 1 ; j < ih ; j++ )
			{
				ov  += *op;
				*op  = ov;
				op  += ow;
			}
		}
#else
		typename image_type::const_pointer    ip = &in[ 0 ];
		typename integral_image_type::pointer op = &integral_( 1, 1 );

		op[ 0 ] = ip[ 0 ];
		for( size_type i = 1 ; i < in.width( ) ; i++ )
		{
			op[ i ] = op[ i - 1 ] + ip[ i ];
		}

		typename integral_image_type::pointer oop = op;
		ip += in.width( );
		op += integral_.width( );

		for( size_type j = 1 ; j < in.height( ) ; j++ )
		{
			value_type tmp = ip[ 0 ];
			op[ 0 ] = oop[ 0 ] + tmp;

			for( size_type i = 1 ; i < in.width( ) ; i++ )
			{
				tmp += ip[ i ];
				op[ i ] = oop[ i ] + tmp;
			}

			oop = op;
			ip += in.width( );
			op += integral_.width( );
		}
#endif
	}

	/// @brief �R���X�g���N�^
	//! 
	//! @param[in] in �c �摜�z��
	//!
	integral_image( const image_type &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	//!
	integral_image( ) : integral_( )
	{
	}
};


/// @brief 3�����̐ϕ��摜��ێ����C�C�ӂ̋�`�̈�̉�f�l�̑��a�������Ɍv�Z���邽�߂̃N���X
//!
//! ��f��n�ɑ΂���O(n)�̑O�v�Z���s�����Ƃɂ��C�C�ӂ̋�`�̈�̉�f�l�̑��a��O(1)�ŎZ�o����
//! ����̉摜�z�񒆂̕����̈قȂ��`�̈�ɑ΂��āA���a���v�Z����K�v������ꍇ�ȂǂɗL��
//!
//! mist::array, mist::array1, mist::array2, mist::array3�ɑΉ��D
//! @attention ���͂̃f�[�^�^�́A�Z�p�^�Cmist::rgb< �Z�p�^ >�̂ݑΉ��D
//! @attention �������f�l�̑��a�̃f�[�^�^�́C���͂̃f�[�^�^�Ɉˑ����ĕω��D
//! @attention ���͌^�F������������ �� ��f�l�̑��a�^�Fsize_t
//! @attention ���͌^�F�����t������ �� ��f�l�̑��a�^�Fint
//! @attention ���͌^�F���������_   �� ��f�l�̑��a�^�Fdouble
//! @attention mist::rgb< ���͌^ >  �� mist::rgb< ��f�l�̑��a�^ >
//! 
//! @param T         �c ���͂̃f�[�^�^�i double �� mist::rgb< unsigned char > �Ȃ� �j
//! @param Allocator �c MIST�R���e�i�Ŏg�p����A���P�[�^
//!
//! @code �g�p��
//! // �摜�z��i1-3�����j�̗p��
//! mist::array2< unsigned char > img;
//! mist::read_bmp( img, "img.bmp" );
//! 
//! // Integral Image �̍\�z�FO(n) �i�e���v���[�g�̈����Ƃ��ē��͔z��̌^���w��j
//! mist::integral_image< mist::array2< unsigned char > > i_img( img );
//!
//! // �n�_( 100, 200 ), �T�C�Y( 50, 60 )�̗̈�̉�f�l�̑��a���o�́FO(1)
//! std::cout << i_img( 100, 200, 50, 60 ) << std::endl;
//!
//! // �n�_( 200, 100 ), �T�C�Y( 60, 50 )�̗̈�̉�f�l�̑��a���o�́FO(1)
//! std::cout << i_img( 200, 100, 60, 50 ) << std::endl;
//! @endcode
//!
template< typename T, typename Allocator >
class integral_image< array3< T, Allocator > >
{
public:
	typedef typename __integral_image__::integral_type< T, is_float< T >::value, is_color< T >::value >::type value_type;
	typedef array3< value_type > integral_image_type;
	typedef array3< T, Allocator > image_type;
	typedef typename image_type::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename image_type::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	integral_image_type integral_;

public:
	/// @brief �C�ӂ̋�`�̈���̉�f�̑��a��Ԃ��FO(1)
	//! 
	//! @param[in] begin_i �c i�����̎n�_
	//! @param[in] begin_j �c j�����̎n�_
	//! @param[in] begin_k �c k�����̎n�_
	//! @param[in] width   �c i�����̃T�C�Y
	//! @param[in] height  �c j�����̃T�C�Y
	//! @param[in] depth   �c k�����̃T�C�Y
	//!
	//! @return ��f�l�̑��a
	//!
	value_type operator ( )( const size_type begin_i, const size_type begin_j, const size_type begin_k, const size_type width, const size_type height, const size_type depth ) const
	{
		return( integral_( begin_i + width, begin_j + height, begin_k + depth ) + integral_( begin_i, begin_j, begin_k + depth ) + integral_( begin_i, begin_j + height, begin_k ) + integral_( begin_i + width, begin_j, begin_k ) - integral_( begin_i, begin_j + height, begin_k + depth ) - integral_( begin_i + width, begin_j, begin_k + depth ) - integral_( begin_i + width, begin_j + height, begin_k ) - integral_( begin_i, begin_j, begin_k ) );
	}

	/// @brief �C�ӂ̈ʒu�̐ϕ��l��Ԃ�
	//! 
	//! @param[in] i �c i�����̎n�_
	//! @param[in] j �c j�����̎n�_
	//! @param[in] k �c k�����̎n�_
	//!
	//! @return ��f�l�̑��a
	//!
	value_type operator ( )( const size_type i, const size_type j, const size_type k ) const
	{
		return( integral_( i + 1, j + 1, k + 1 ) );
	}

	/// @brief �S�Ẳ�f�l�̑��a��Ԃ��FO(1)
	//! 
	//! @return ��f�l�̑��a
	//!
	value_type overall( ) const 
	{
		return( integral_( integral_.width( ) - 1, integral_.height( ) - 1, integral_.depth( ) - 1 ) );
	}

	/// @brief �摜�z���i�����̃T�C�Y��Ԃ�
	//! 
	//! @return i�����̃T�C�Y
	//!
	size_type size1( ) const
	{
		return( integral_.width( ) - 1 );
	}

	/// @brief �摜�z���j�����̃T�C�Y��Ԃ�
	//! 
	//! @return j�����̃T�C�Y
	//!
	size_type size2( ) const
	{
		return( integral_.height( ) - 1 );
	}

	/// @brief �摜�z���k�����̃T�C�Y��Ԃ�
	//! 
	//! @return k�����̃T�C�Y
	//!
	size_type size3( ) const
	{
		return( integral_.depth( ) - 1 );
	}

	/// @brief �摜�z���i�����̃T�C�Y��Ԃ�
	//! 
	//! @return i�����̃T�C�Y
	//!
	size_type width( ) const
	{
		return( size1( ) );
	}

	/// @brief �摜�z���j�����̃T�C�Y��Ԃ�
	//! 
	//! @return j�����̃T�C�Y
	//!
	size_type height( ) const
	{
		return( size2( ) );
	}

	/// @brief �摜�z���k�����̃T�C�Y��Ԃ�
	//! 
	//! @return k�����̃T�C�Y
	//!
	size_type depth( ) const
	{
		return( size3( ) );
	}

	/// @brief �摜�z��̃T�C�Y��Ԃ�
	//! 
	//! @return �T�C�Y
	//!
	size_type size( ) const
	{
		return( width( ) * height( ) * depth( ) );
	}

	/// @brief Integral Image �̍\�z�FO(n)
	//! 
	//! @param[in] in �c �摜�z��
	//!
	void construct_integral_array( const image_type &in )
	{
		integral_.resize( in.width( ) + 1, in.height( ) + 1, in.depth( ) + 1 );	
		for( size_type k = 1 ; k < integral_.depth( ) ; k ++  )
		{
			for( size_type j = 1 ; j < integral_.height( ) ; j ++ )
			{
				for( size_type i = 1 ; i < integral_.width( ) ; i ++ )
				{
					integral_( i, j, k ) = in( i - 1, j - 1, k - 1 ) + integral_( i - 1, j, k ) + integral_( i, j - 1, k ) + integral_( i, j, k - 1 ) + integral_( i - 1, j - 1, k - 1 ) - integral_( i - 1, j - 1, k ) - integral_( i - 1, j, k - 1 ) - integral_( i, j - 1, k - 1 );
				}
			}
		}
	}

	/// @brief �R���X�g���N�^
	//! 
	//! @param[in] in �c �摜�z��
	//!
	integral_image( const image_type &in )
	{
		construct_integral_array( in );
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	//!
	integral_image( ) : integral_( )
	{
	}
};


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const integral_image< array< T, Allocator > > &a )
{
	typename array< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[ i ];
		if( i != a.size1( ) - 1 ) out << ", ";
	}

	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array1 �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const integral_image< array1< T, Allocator > > &a )
{
	typename array1< T, Allocator >::size_type i;
	for( i = 0 ; i < a.size( ) ; i++ )
	{
		out << a[ i ];
		if( i != a.size1( ) - 1 ) out << ", ";
	}

	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array2 �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const integral_image< array2< T, Allocator > > &a )
{
	typename array2< T, Allocator >::size_type i, j;
	for( j = 0 ; j < a.size2( ) ; j++ )
	{
		if( j != 0 )
		{
			out << std::endl;
		}
		for( i = 0 ; i < a.size1( ) ; i++ )
		{
			out << a( i, j );
			if( i != a.size1( ) - 1 ) out << ", ";
		}
	}

	return( out );
}


/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c array3 �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! ----- separator -----
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < class T, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const integral_image< array3< T, Allocator > > &a )
{
	typename array3< T, Allocator >::size_type i, j, k;
	for( k = 0 ; k < a.size3( ) ; k++ )
	{
		for( j = 0 ; j < a.size2( ) ; j++ )
		{
			for( i = 0 ; i < a.size1( ) ; i++ )
			{
				out << a( i, j, k );
				if( i != a.size1( ) - 1 ) out << ", ";
			}
			out << std::endl;
		}
		if( k != a.size3( ) - 1 )
		{
			out << "----- separator -----";
		}
		out << std::endl;
	}

	return( out );
}


/// @}
//  Integral Image�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_INTEGRAL_IMAGE_H__
