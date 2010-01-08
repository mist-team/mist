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

/// @file mist/bitmap.h
//!
//! @brief Windows�S�ʂŗ��p�����r�b�g�}�b�v�摜�������R���e�i
//!

#ifndef __INCLUDE_BITMAP_H__
#define __INCLUDE_BITMAP_H__


#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include "config/color.h"
#endif



// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

template < size_t BITS >
struct bitmap_type
{
	typedef bgr< unsigned char > value_type;
};

template < >
struct bitmap_type< 8 >
{
	typedef unsigned char value_type;
};


/// @brief �v�f�����ς̃r�b�g�}�b�v�摜�������N���X
//! 
//! 2�����摜���������߂̊�{�N���X
//! �i@ref mist/bitmap.h ���C���N���[�h����j
//! 
//! @attention MIST�Œ񋟂���قƂ�ǂ̃A���S���Y���ɑ΂��Ė{�R���e�i�𒼐ڗ��p���邱�Ƃ͂ł��܂���D
//! @attention MIST�̃A���S���Y���𗘗p����ꍇ�́C @ref mist::convert �̃R���o�[�g�֐��𗘗p���āCarray2 �^�ɕϊ����Ă���s���Ă��������D
//! 
//! @param BITS      �c �r�b�g�}�b�v�ŗ��p����1��f������̃r�b�g���i24�r�b�g��32�r�b�g�݂̂��T�|�[�g�j
//! @param Allocator �c MIST�R���e�i�����p����A���P�[�^�^�D�ȗ������ꍇ�́CSTL�̃f�t�H���g�A���P�[�^���g�p����
//! 
template < size_t BITS, class Allocator = std::allocator< unsigned char > >
class bitmap : public array< unsigned char, Allocator >
{
public:
	typedef typename bitmap_type< BITS >::value_type value_type;	///< @brief �����f�[�^�^
	typedef value_type* pointer;									///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef value_type& reference;									///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef const value_type& const_reference;						///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::array< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef const value_type* const_pointer;						///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::array< data > �̏ꍇ�Cconst data * �ƂȂ�
	typedef typename Allocator::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Allocator::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���


protected:
	typedef array< unsigned char, Allocator > base;		///< @brief ���N���X
	size_type size1_;									///< @brief X�������̗v�f��
	size_type size2_;									///< @brief Y�������̗v�f��
	size_type nbytes_;									///< @brief �P�s������̃o�C�g��

	/// @brief �P��f��\������̂ɗp����o�C�g��
	static size_type pixel_bytes( )
	{
		return( BITS / 8 );
	}

	/// @brief �P�s������Ɏg�p����o�C�g��
	static size_type num_bytes( size_type width )
	{
		width = width * pixel_bytes( );
		if( width % 4 > 0 )
		{
			width += 4 - ( width % 4 );
		}
		return( width );
	}

public:
	/// @brief �R���e�i���̗v�f����ύX����
	//! 
	//! �v�f���� num1 �~ num2 �ɕύX���C�v�f�����ύX���ꂽ�ꍇ�̂ݑS�Ă̗v�f��l�f�t�H���g�l�ŏ���������D
	//! 
	//! @attention �K���v�f�����������邽�߂ɂ́Cfill �֐��𗘗p���Ă��������D
	//! 
	//! @param[in] num1 �c ���T�C�Y���X�������̗v�f��
	//! @param[in] num2 �c ���T�C�Y���Y�������̗v�f��
	//! @param[in] dmy1 �c �_�~�[�ϐ��i�g�p���Ȃ��j
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num1, size_type num2, size_type dmy1 = 0 )
	{
		nbytes_ = num_bytes( num1 );
		if( base::resize( nbytes_ * num2 ) )
		{
			size1_ = num1;
			size2_ = num2;
			return( true );
		}
		else
		{
			size1_ = size2_ = nbytes_ = 0;
			return( false );
		}
	}

	/// @brief �R���e�i���̗v�f���g���~���O����
	//! 
	//! @param[in] x �c �g���~���O��X�������̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] y �c �g���~���O��Y�������̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] w �c �g���~���O���X�������̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! @param[in] h �c �g���~���O���Y�������̗v�f���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! 
	//! @retval true  �c �g���~���O�ɐ��������ꍇ�i���ƃT�C�Y���ς��Ȃ��ꍇ���܂ށj
	//! @retval false �c �s���ȃg���~���O���s�����Ƃ����ꍇ
	//! 
	bool trim( size_type x, size_type y, difference_type w = -1, difference_type h = -1 )
	{
		difference_type w_ = width( );
		difference_type h_ = height( );
		if( w_ <= static_cast< difference_type >( x ) || w_ < static_cast< difference_type >( x + w ) )
		{
			return( false );
		}
		else if( h_ <= static_cast< difference_type >( y ) || h_ < static_cast< difference_type >( y + h ) )
		{
			return( false );
		}
		else if( w_ == w && h_ == h )
		{
			return( true );
		}

		if( w < 0 )
		{
			w = w_ - x;
		}
		if( h < 0 )
		{
			h = h_ - y;
		}


		if( base::is_memory_shared( ) )
		{
			// �O���������𗘗p���Ă���ꍇ
			bitmap o( *this );
			if( resize( w, h ) )
			{
				for( difference_type j = 0 ; j < h ; j++ )
				{
					for( difference_type i = 0 ; i < w ; i++ )
					{
						operator ()( i, j ) = o( i + x, j + y );
					}
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			bitmap o( w, h );
			for( difference_type j = 0 ; j < h ; j++ )
			{
				for( difference_type i = 0 ; i < w ; i++ )
				{
					o( i, j ) = operator ()( i + x, j + y );
				}
			}

			swap( o );
		}

		return( true );
	}


	/// @brief �R���e�i���̑S�Ă̓��e�����ւ���D
	//! 
	//! ����ւ����̃R���e�i a �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] a  �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( bitmap &a )
	{
		if( base::swap( a ) )
		{
			size_type _dmy_size1 = size1_;
			size_type _dmy_size2 = size2_;
			size1_ = a.size1_;
			size2_ = a.size2_;
			a.size1_ = _dmy_size1;
			a.size2_ = _dmy_size2;
			return( true );
		}
		else
		{
			return( false );
		}
	}


	/// @brief �R���e�i�̗v�f����ɂ���
	//! 
	//! �R���e�i�Ɋi�[����Ă���f�[�^��S�č폜���C�R���e�i����i�v�f��0�j�ɂ���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}


	size_type size1( )  const { return( size1_ ); }			///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type size2( )  const { return( size2_ ); }			///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type width( )  const { return( size1_ ); }			///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const { return( size2_ ); }			///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�

	size_type size( ) const { return( size1_ * size2_ ); }			///< @brief �R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�

public:
	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �R�s�[���ƃR�s�[��Ńf�[�^�^�iarray< data > �� data�j���قȂ�ꍇ�̑�����s���D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < size_t BBITS, class AAlocator >
	const bitmap& operator =( const bitmap< BBITS, AAlocator > &o )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2_ ; j++ )
			{
				for( size_type i = 0 ; i < size1_ ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}

		return( *this );
	}


	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �R�s�[���ƃR�s�[��Ńf�[�^�^�iarray< data > �� data�j���قȂ�ꍇ�̑�����s���D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const bitmap& operator =( const array2< TT, AAlocator > &o )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2_ ; j++ )
			{
				for( size_type i = 0 ; i < size1_ ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}

		return( *this );
	}


	/// @brief �v�f�̌^���قȂ�R���e�i��������
	//! 
	//! �R�s�[���ł���R���e�i o �ƑS�������R���e�i���쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���̃R���e�i
	//! 
	//! @return �������g
	//! 
	const bitmap& operator =( const bitmap &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );
		size1_ = o.size1_;
		size2_ = o.size2_;

		return( *this );
	}

// �v�f�ւ̃A�N�Z�X
protected:
	/// @brief index �Ŏ������ʒu�̗v�f�̃|�C���^��Ԃ�
	//! 
	//! @param[in] i �c �R���e�i����X�������̈ʒu
	//! @param[in] j �c �R���e�i����Y�������̈ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	pointer paccess( size_type i, size_type j )
	{
		return( reinterpret_cast< pointer >( base::data_ + i * pixel_bytes( ) + j * nbytes_ ) );
	}

	/// @brief index �Ŏ������ʒu�̗v�f�� const �|�C���^��Ԃ�
	//! 
	//! @param[in] i �c �R���e�i����X�������̈ʒu
	//! @param[in] j �c �R���e�i����Y�������̈ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	const_pointer paccess( size_type i, size_type j ) const
	{
		return( reinterpret_cast< const_pointer >( base::data_ + i * pixel_bytes( ) + j * nbytes_ ) );
	}

public:
	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator []( size_type index )
	{
		_CHECK_ACCESS_VIOLATION1U_( index )
		size_type j = index / size1_;
		size_type i = index - j * size1_;
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


	/// @brief index �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ index ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] index �c �R���e�i���̗v�f�ʒu
	//! 
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator []( size_type index ) const
	{
		_CHECK_ACCESS_VIOLATION1U_( index )
		size_type j = index / size1_;
		size_type i = index - j * size1_;
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference at( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference at( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f�������Q��
	//!
	reference operator ()( size_type i, size_type j, size_type dmy = 0 )
	{
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


	/// @brief ( i, j ) �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ ( i, j ) ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//!
	/// @param[in] i   �c �R���e�i����X�������̈ʒu
	/// @param[in] j   �c �R���e�i����Y�������̈ʒu
	/// @param[in] dmy �c �g�p���Ȃ�
	//!
	/// @return �w�肳�ꂽ�v�f������ const �Q��
	//!
	const_reference operator ()( size_type i, size_type j, size_type dmy = 0 ) const
	{
		_CHECK_ACCESS_VIOLATION2U_( i, j )
		return( reinterpret_cast< reference >( base::data_[ i * pixel_bytes( ) + j * nbytes_ ] ) );
	}


public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D�v�f�� 0�C�𑜓x 1.0 �~ 1.0 �̃R���e�i���쐬����
	bitmap( ) : base( ), size1_( 0 ), size2_( 0 ), nbytes_( 0 ) {}

	/// @brief �A���P�[�^ a �̃R�s�[�𗘗p����
	explicit bitmap( const Allocator &a ) : base( a ), size1_( 0 ), size2_( 0 ), nbytes_( 0 ) {}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�f�t�H���g�l�ŗv�f������������
	bitmap( size_type num1, size_type num2 ) : base( num_bytes( num1 ) * num2 ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�g�p����A���P�[�^�� a �ɐݒ肷��
	bitmap( size_type num1, size_type num2, const Allocator &a ) : base( num_bytes( num1 ) * num2, a ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}


	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�S�v�f�� val �ŏ���������
	bitmap( size_type num1, size_type num2, const value_type &val ) : base( num_bytes( num1 ) * num2, val ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}

	/// @brief �v�f�� num1 �~ num2 �̃R���e�i���쐬���C�S�v�f�� val �ŏ��������C�g�p����A���P�[�^�� a �ɐݒ肷��
	bitmap( size_type num1, size_type num2, const value_type &val, const Allocator &a ) : base( num_bytes( num1 ) * num2, val, a ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}


	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �̃R���e�i���쐬����iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	bitmap( size_type num1, size_type num2, void *ptr, size_type mem_available ) : base( num_bytes( num1 ) * num2, reinterpret_cast< unsigned char * >( ptr ), mem_available ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) ) size1_ = size2_ = nbytes_ = 0;
	}

	/// @brief ptr ���w���������̈�ɁC�v�f�� num1 �~ num2 �̃R���e�i���쐬���C�S�v�f�� val �ŏ���������iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	bitmap( size_type num1, size_type num2, const value_type &val, void *ptr, size_type mem_available ) : base( num_bytes( num1 ) * num2, reinterpret_cast< unsigned char * >( ptr ), mem_available ), size1_( num1 ), size2_( num2 ), nbytes_( num_bytes( num1 ) )
	{
		if( base::empty( ) )
		{
			size1_ = size2_ = nbytes_ = 0;
		}
		else
		{
			for( size_type j = 0 ; j < size2_ ; j++ )
			{
				for( size_type i = 0 ; i < size1_ ; i++ )
				{
					operator ()( i, j ) = val;
				}
			}
		}
	}


	/// @brief ���� bitmap �z��ŗv�f�̌^���قȂ���̂��瓯���v�f���̔z����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < size_t BBITS, class AAlocator >
	bitmap( const bitmap< BBITS, AAlocator > &o ) : base( ), size1_( 0 ), size2_( 0 ), nbytes_( 0 )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2( ) ; j++ )
			{
				for( size_type i = 0 ; i < size1( ) ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}
	}


	/// @brief ���� array2 �z��ŗv�f�̌^���قȂ���̂��瓯���v�f���̔z����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	bitmap( const array2< TT, AAlocator > &o ) : base( ), size1_( 0 ), size2_( 0 ), nbytes_( 0 )
	{
		if( resize( o.size1( ), o.size2( ) ) )
		{
			for( size_type j = 0 ; j < size2( ) ; j++ )
			{
				for( size_type i = 0 ; i < size1( ) ; i++ )
				{
					operator ()( i, j ) = o( i, j );
				}
			}
		}
	}

	/// @brief ���� bitmap �z��œ����v�f�^�̂��̂�p���ď���������
	bitmap( const bitmap< BITS, Allocator > &o ) : base( o ), size1_( o.size1_ ), size2_( o.size2_ ), nbytes_( o.nbytes_ ) {}
};



/// @brief �w�肳�ꂽ�X�g���[���ɁC�R���e�i���̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     a   �c bitmap �z��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 2, 3, 4
//! 5, 6, 7, 8
//! 9, 10, 11, 12
//! @endcode
//! 
template < size_t BITS, class Allocator >
inline std::ostream &operator <<( std::ostream &out, const bitmap< BITS, Allocator > &a )
{
	typename bitmap< BITS, Allocator >::size_type i, j;
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



// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_H__
