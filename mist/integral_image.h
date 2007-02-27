// 
// Copyright (c) 2003-2007, MIST Project, Intelligent Media Integration COE, Nagoya University
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

#include <mist/mist.h>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

// ���͂̃f�[�^�^�����f�l�̑��a�̃f�[�^�^�����肷�邽�߂̃N���X
namespace __integral_image__
{
	template< typename Type >	struct is_signed					{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_signed< unsigned char >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< unsigned short >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< unsigned int >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< unsigned long >	{ _MIST_CONST( bool, value, false ); };

	template< typename Type >	struct is_decimal			{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_decimal< float >	{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_decimal< double >	{ _MIST_CONST( bool, value, true  ); };


	template< typename Type >	struct is_signed< mist::rgb< Type > >			{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_signed< mist::rgb< unsigned char > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< mist::rgb< unsigned short > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< mist::rgb< unsigned int > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_signed< mist::rgb< unsigned long > >	{ _MIST_CONST( bool, value, false ); };

	template< typename Type >	struct is_decimal< mist::rgb< Type > >	{ _MIST_CONST( bool, value, false ); };
	template< >					struct is_decimal< mist::rgb< float > >	{ _MIST_CONST( bool, value, true  ); };
	template< >					struct is_decimal< mist::rgb< double > >{ _MIST_CONST( bool, value, true  ); };


	template< bool Is_signed, bool Is_decimal, bool Is_color >	struct integral_type						{ typedef mist::rgb< double >	type; };
	template< >													struct integral_type< true,  false, true >	{ typedef mist::rgb< int >		type; };
	template< >													struct integral_type< false, false, true >	{ typedef mist::rgb< size_t >	type; };
	template< >													struct integral_type< true,  true,  false >	{ typedef double				type; };
	template< >													struct integral_type< true,  false, false >	{ typedef int					type; };
	template< >													struct integral_type< false, false, false >	{ typedef size_t				type; };
}

// �������ꉻ���Ȃ��ꍇ�i�s�g�p�j
template< typename Value_type >
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



/// @brief �C�ӂ̋�`�̈�̉�f�l�̑��a�������Ɍv�Z���邽�߂̃N���X
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
//! @param Value_type  �c ���͂̃f�[�^�^�i double �� mist::rgb< unsigned char > �Ȃ� �j
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
template< typename Value_type >
class integral_image< mist::array< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array< integral_type > integral_;

	integral_type _value( const size_t i ) const
	{
		return integral_[ i ];
	}

public:

	/// @brief �C�ӂ̋�`�̈���̉�f�̑��a��Ԃ��FO(1)
	//! 
	//! @param[in] begin �c �n�_
	//! @param[in] size  �c �T�C�Y
	//!
	//! @return ��f�l�̑��a
	//!
	integral_type operator ( )( const size_t begin, const size_t size ) const
	{
		return _value( begin + size ) - _value( begin );
	}

	/// @brief �S�Ẳ�f�l�̑��a��Ԃ��FO(1)
	//! 
	//! @return ��f�l�̑��a
	//!
	integral_type overall( ) const 
	{
		return integral_[ integral_.size( ) - 1 ];
	}

	/// @brief �摜�z��̃T�C�Y��Ԃ�
	//! 
	//! @return �T�C�Y
	//!
	size_t size( ) const
	{
		return integral_.size( ) - 1;;
	}

	/// @brief Integral Image �̍\�z�FO(n)
	//! 
	//! @param[in] in �c �摜�z��
	//!
	void construct_integral_array( const mist::array< Value_type > &in )
	{
		integral_.resize( in.size( ) + 1 );	
		for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
		{
			integral_[ i ] = in[ i - 1 ] + integral_[ i - 1 ];
		}
	}

	/// @brief �R���X�g���N�^
	//! 
	//! @param[in] in �c �摜�z��
	//!
	integral_image( const mist::array< Value_type > &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	//!
	
	integral_image( ) : integral_( mist::array< integral_type >( ) )
	{
	}
};


template< typename Value_type >
class integral_image< mist::array1< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array1< integral_type > integral_;

	integral_type _value( const size_t i ) const
	{
		return integral_[ i ];
	}

public:

	/// @brief �C�ӂ̋�`�̈���̉�f�̑��a��Ԃ��FO(1)
	//! 
	//! @param[in] begin �c �n�_
	//! @param[in] size  �c �T�C�Y
	//!
	//! @return ��f�l�̑��a
	//!
	integral_type operator ( )( const size_t begin, const size_t size ) const
	{
		return _value( begin + size ) - _value( begin );
	}

	/// @brief �S�Ẳ�f�l�̑��a��Ԃ��FO(1)
	//! 
	//! @return ��f�l�̑��a
	//!
	integral_type overall( ) const 
	{
		return integral_[ integral_.size( ) - 1 ];
	}

	/// @brief �摜�z��̃T�C�Y��Ԃ�
	//! 
	//! @return �T�C�Y
	//!
	size_t size( ) const
	{
		return integral_.size( ) - 1;;
	}

	/// @brief Integral Image �̍\�z�FO(n)
	//! 
	//! @param[in] in �c �摜�z��
	//!
	void construct_integral_array( const mist::array1< Value_type > &in )
	{ 
		integral_.resize( in.size( ) + 1 );	
		for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
		{
			integral_[ i ] = in[ i - 1 ] + integral_[ i - 1 ];
		}
	}

	/// @brief �R���X�g���N�^
	//! 
	//! @param[in] in �c �摜�z��
	//!
	integral_image( const mist::array1< Value_type > &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	//!
	integral_image( ) : integral_( mist::array1< integral_type >( ) )
	{
	}
};


template< typename Value_type >
class integral_image< mist::array2< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array2< integral_type > integral_;

	integral_type _value( const size_t i, const size_t j ) const
	{
		return integral_( i, j );
	}

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
	integral_type operator ( )( const size_t begin_i, const size_t begin_j, const size_t width, const size_t height ) const
	{
		return( _value( begin_i + width, begin_j + height ) + _value( begin_i, begin_j ) - _value( begin_i + width, begin_j ) - _value( begin_i, begin_j + height ) );
	}

	/// @brief �S�Ẳ�f�l�̑��a��Ԃ��FO(1)
	//! 
	//! @return ��f�l�̑��a
	//!
	integral_type overall( ) const 
	{
		return integral_( integral_.width( ) - 1, integral_.height( ) - 1 );
	}

	/// @brief �摜�z���i�����̃T�C�Y��Ԃ�
	//! 
	//! @return i�����̃T�C�Y
	//!
	size_t width( ) const
	{
		return integral_.width( ) - 1;
	}

	/// @brief �摜�z���j�����̃T�C�Y��Ԃ�
	//! 
	//! @return j�����̃T�C�Y
	//!
	size_t height( ) const
	{
		return integral_.height( ) - 1;
	}

	/// @brief �摜�z��̃T�C�Y��Ԃ�
	//! 
	//! @return �T�C�Y
	//!
	size_t size( ) const
	{
		return width( ) * height( );
	}

	/// @brief Integral Image �̍\�z�FO(n)
	//! 
	//! @param[in] in �c �摜�z��
	//!
	void construct_integral_array( const mist::array2< Value_type > &in )
	{ 
		integral_.resize( in.width( ) + 1, in.height( ) + 1 );	
		for( size_t j = 1 ; j < integral_.height( ) ; j ++ )
		{
			for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
			{
				integral_( i, j ) = in( i - 1, j - 1 ) + integral_( i, j - 1 ) + integral_( i - 1, j ) - integral_( i - 1, j - 1 );
			}
		}
	}

	/// @brief �R���X�g���N�^
	//! 
	//! @param[in] in �c �摜�z��
	//!
	integral_image( const mist::array2< Value_type > &in )
	{ 
		construct_integral_array( in );
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	//!
	integral_image( ) : integral_( mist::array2< integral_type >( ) )
	{
	}
};


template< typename Value_type >
class integral_image< mist::array3< Value_type > >
{
public:

	typedef typename __integral_image__::integral_type< 
		__integral_image__::is_signed< Value_type >::value, 
		__integral_image__::is_decimal< Value_type >::value, 
		mist::is_color< Value_type >::value 
	>::type integral_type;

private:

	mist::array3< integral_type > integral_;

	integral_type _value( const size_t i, const size_t j, const size_t k ) const
	{
		return integral_( i, j, k );
	}

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
	integral_type operator ( )( const size_t begin_i, const size_t begin_j, const size_t begin_k, const size_t width, const size_t height, const size_t depth ) const
	{
		return( _value( begin_i + width, begin_j + height, begin_k + depth ) + _value( begin_i, begin_j, begin_k + depth ) + _value( begin_i, begin_j + height, begin_k ) + _value( begin_i + width, begin_j, begin_k ) - _value( begin_i, begin_j + height, begin_k + depth ) - _value( begin_i + width, begin_j, begin_k + depth ) - _value( begin_i + width, begin_j + height, begin_k ) - _value( begin_i, begin_j, begin_k ) );
	}

	/// @brief �S�Ẳ�f�l�̑��a��Ԃ��FO(1)
	//! 
	//! @return ��f�l�̑��a
	//!
	integral_type overall( ) const 
	{
		return integral_( integral_.width( ) - 1, integral_.height( ) - 1, integral_.depth( ) - 1 );
	}

	/// @brief �摜�z���i�����̃T�C�Y��Ԃ�
	//! 
	//! @return i�����̃T�C�Y
	//!
	size_t width( ) const
	{
		return integral_.width( ) - 1;
	}

	/// @brief �摜�z���j�����̃T�C�Y��Ԃ�
	//! 
	//! @return j�����̃T�C�Y
	//!
	size_t height( ) const
	{
		return integral_.height( ) - 1;
	}

	/// @brief �摜�z���k�����̃T�C�Y��Ԃ�
	//! 
	//! @return k�����̃T�C�Y
	//!
	size_t depth( ) const
	{
		return integral_.depth( ) - 1;
	}

	/// @brief �摜�z��̃T�C�Y��Ԃ�
	//! 
	//! @return �T�C�Y
	//!
	size_t size( ) const
	{
		return width( ) * height( ) * depth( );
	}

	/// @brief Integral Image �̍\�z�FO(n)
	//! 
	//! @param[in] in �c �摜�z��
	//!
	void construct_integral_array( const mist::array3< Value_type > &in )
	{
		integral_.resize( in.width( ) + 1, in.height( ) + 1, in.depth( ) + 1 );	
		for( size_t k = 1 ; k < integral_.depth( ) ; k ++  )
		{
			for( size_t j = 1 ; j < integral_.height( ) ; j ++ )
			{
				for( size_t i = 1 ; i < integral_.width( ) ; i ++ )
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
	integral_image( const mist::array3< Value_type > &in )
	{
		construct_integral_array( in );
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	//!
	integral_image( ) : integral_( mist::array3< integral_type >( ) )
	{
	}
};


/// @}
//  Integral Image�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_INTEGRAL_IMAGE_H__
