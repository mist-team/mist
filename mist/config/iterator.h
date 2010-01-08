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

/// @file mist/config/iterator.h
//!
//! @brief MIST�R���e�i�𑀍삷�邽�߂̃C�e���[�^
//!

#ifndef __INCLUDE_MIST_ITERATOR_H__
#define __INCLUDE_MIST_ITERATOR_H__


#include <iterator>

#ifndef __INCLUDE_MIST_CONF_H__
#include "mist_conf.h"
#endif

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



/// @brief mist�R���e�i�ŗ��p����1��������p�����_���A�N�Z�X�C�e���[�^
//! 
//! array�Carray1�Carray2�Cmatrix�Cvector �p�̏����������_���A�N�Z�X�C�e���[�^
//! 
//! @param T         �c ���͌^
//! @param Distance  �c �C�e���[�^�Ԃ̍���\���^�i�f�t�H���g�� ptrdiff_t�j
//! @param Pointer   �c �C�e���[�^�̗v�f�ւ̃|�C���^�^�i�f�t�H���g�� T*�j
//! @param Reference �c �C�e���[�^�̗v�f�ւ̎Q�ƌ^�i�f�t�H���g�� T&�j
//! 
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator1 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;					///< @brief �����f�[�^�^
	typedef Pointer pointer;				///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef Reference reference;			///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef Distance difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef Reference const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�

private:
	pointer data_;					///< @brief ���݂̗v�f���w���|�C���^
	size_type diff_pointer_;		///< @brief �v�f�Ԃ̃|�C���^�̍�

public:
	/// @brief �f�t�H���g�R���X�g���N�^
	mist_iterator1( pointer p = NULL, size_type diff = 1 ) : data_( p ), diff_pointer_( diff ){ }

	/// @brief ���̃C�e���[�^��p���ď���������
	template< class TT, class DD, class PP, class RR >
	mist_iterator1( const mist_iterator1< TT, DD, PP, RR > &ite ) : data_( ite.data( ) ), diff_pointer_( ite.diff( ) ){ }


	/// @brief ���̃C�e���[�^��������
	template< class TT, class DD, class PP, class RR >
	const mist_iterator1& operator =( const mist_iterator1< TT, DD, PP, RR > &ite )
	{
		data_ = ite.data( );
		diff_pointer_ = ite.diff( );
		return( *this );
	}


	/// @brief �v�f�̎Q��
	reference operator *(){ return( *data_ ); }

	/// @brief �v�f�� const �Q��
	const_reference operator *() const { return( *data_ ); }

	/// @brief �v�f�̃A�h���X���Z�q
	pointer operator->( ){ return( &**this ); }

	/// @brief �v�f�� const �A�h���X���Z�q
	const pointer operator->( ) const { return( &**this ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�̎Q��
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�� const �Q��
	const_reference operator []( difference_type dist ) const { return( data_[ dist * diff_pointer_ ] ); }

	///< @brief ���݂̗v�f���w���|�C���^
	pointer data( )
	{
		return( data_ );
	}

	///< @brief ���݂̗v�f���w���|�C���^
	const pointer data( ) const
	{
		return( data_ );
	}

	///< @brief �v�f�Ԃ̃|�C���^�̍�
	size_type diff( ) const
	{
		return( diff_pointer_ );
	}

	/// @brief �O�u�^�̃C���N�������g���Z�q
	mist_iterator1& operator ++( )
	{
		*this += 1;
		return( *this );
	}

	/// @brief ��u�^�̃C���N�������g���Z�q
	const mist_iterator1 operator ++( int )
	{
		mist_iterator1 old_val( *this );
		*this += 1;
		return( old_val );
	}

	/// @brief �O�u�^�̃f�N�������g���Z�q
	mist_iterator1& operator --( )
	{
		*this -= 1;
		return( *this );
	}

	/// @brief ��u�^�̃f�N�������g���Z�q
	const mist_iterator1 operator --( int )
	{
		mist_iterator1 old_val( *this );
		*this -= 1;
		return( old_val );
	}


	/// @brief dist ���� + �����ɗ��ꂽ�ʒu�̗v�f�Ɉړ�����
	const mist_iterator1& operator +=( difference_type dist )
	{
		data_ += dist * diff_pointer_;
		return( *this );
	}

	/// @brief dist ���� - �����ɗ��ꂽ�ʒu�̗v�f�Ɉړ�����
	const mist_iterator1& operator -=( difference_type dist )
	{
		data_ -= dist * diff_pointer_;
		return( *this );
	}


	/// @brief �v�f�Ԃ̃|�C���^�̍����v�Z����
	template< class TT, class DD, class PP, class RR >
	const difference_type operator -( const mist_iterator1< TT, DD, PP, RR > &ite ) const
	{
		return( ( data_ - ite.data( ) ) / diff_pointer_ );
	}


	/// @brief ��̃C�e���[�^�������v�f�������Ă��邩�ǂ���
	template< class TT, class DD, class PP, class RR >
	bool operator ==( const mist_iterator1< TT, DD, PP, RR > &ite ) const { return( data_ == ite.data( ) ); }

	/// @brief ��̃C�e���[�^�������v�f�������Ă��Ȃ����ǂ���
	template< class TT, class DD, class PP, class RR >
	bool operator !=( const mist_iterator1< TT, DD, PP, RR > &ite ) const { return( !( *this == ite )  ); }

	/// @brief ��̃C�e���[�^�� < �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator < ( const mist_iterator1< TT, DD, PP, RR > &ite ) const { return( data_ <  ite.data( ) ); }

	/// @brief ��̃C�e���[�^�� <= �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator <=( const mist_iterator1< TT, DD, PP, RR > &ite ) const { return( data_ <= ite.data( ) ); }

	/// @brief ��̃C�e���[�^�� > �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator > ( const mist_iterator1< TT, DD, PP, RR > &ite ) const { return( data_ >  ite.data( ) ); }

	/// @brief ��̃C�e���[�^�� >= �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator >=( const mist_iterator1< TT, DD, PP, RR > &ite ) const { return( data_ >= ite.data( ) ); }
};


/// @brief dist ���� + �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief dist ���� + �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator +( typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator1< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief dist ���� - �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator1< T, Distance, Pointer, Reference > operator -( const mist_iterator1< T, Distance, Pointer, Reference > &ite, typename mist_iterator1< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator1< T, Distance, Pointer, Reference >( ite ) -= dist );
}



/// @brief mist�R���e�i�ŗ��p����2��������p�����_���A�N�Z�X�C�e���[�^
//! 
//! array3 �p�̏����������_���A�N�Z�X�C�e���[�^
//! 
//! @param T         �c ���͌^
//! @param Distance  �c �C�e���[�^�Ԃ̍���\���^�i�f�t�H���g�� ptrdiff_t�j
//! @param Pointer   �c �C�e���[�^�̗v�f�ւ̃|�C���^�^�i�f�t�H���g�� T*�j
//! @param Reference �c �C�e���[�^�̗v�f�ւ̎Q�ƌ^�i�f�t�H���g�� T&�j
//! 
template< class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance >
#else
class mist_iterator2 : public std::iterator< std::random_access_iterator_tag, T, Distance, Pointer, Reference >
#endif
{
public:
	typedef T value_type;					///< @brief �����f�[�^�^�Dbool �Ɠ���
	typedef Pointer pointer;				///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef Reference reference;			///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef size_t size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef Distance difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef Reference const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�

private:
	pointer data_;				///< @brief ���݂̃C�e���[�^�[���w���Ă���̈�̐擪�ʒu
	difference_type index_;		///< @brief ���݂̃C�e���[�^�[���w���Ă���C���f�b�N�X
	difference_type width_;		///< @brief 1�s������̃f�[�^��
	difference_type step_;		///< @brief �C���N�������g���̃X�L�b�v��

public:
	/// @brief �f�t�H���g�R���X�g���N�^
	mist_iterator2( pointer p = NULL, difference_type index = 0, difference_type width = 1, difference_type step = 1 )
												: data_( p ), index_( index ), width_( width ), step_( step )
	{
	}

	/// @brief ���̃C�e���[�^��p���ď���������
	template< class TT, class DD, class PP, class RR >
	mist_iterator2( const mist_iterator2< TT, DD, PP, RR > &ite ) : data_( ite.data( ) ), index_( ite.index( ) ), width_( ite.width( ) ), step_( ite.step( ) )
	{
	}


	/// @brief ���̃C�e���[�^��������
	template< class TT, class DD, class PP, class RR >
	const mist_iterator2& operator =( const mist_iterator2< TT, DD, PP, RR > &ite )
	{
		if( &ite != this )
		{
			data_  = ite.data( );
			index_ = ite.index( );
			width_ = ite.width( );
			step_  = ite.step( );
		}
		return( *this );
	}

	///< @brief ���݂̃C�e���[�^�[���w���Ă���̈�̐擪�ʒu
	pointer data( )
	{
		return( data_ );
	}

	///< @brief ���݂̃C�e���[�^�[���w���Ă���̈�̐擪�ʒu
	const pointer data( ) const
	{
		return( data_ );
	}

	///< @brief ���݂̃C�e���[�^�[���w���Ă���C���f�b�N�X
	difference_type index( ) const
	{
		return( index_ );
	}

	///< @brief 1�s������̃f�[�^��
	difference_type width( ) const
	{
		return( width_ );
	}

	///< @brief �C���N�������g���̃X�L�b�v��
	difference_type step( ) const
	{
		return( step_ );
	}

	/// @brief �v�f�̎Q��
	reference operator *( )
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

	/// @brief �v�f�� const �Q��
	const_reference operator *( ) const
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

	/// @brief �v�f�̃A�h���X���Z�q
	pointer operator->( ){ return( &**this ); }

	/// @brief �v�f�� const �A�h���X���Z�q
	const pointer operator->( ) const { return( &**this ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�̎Q��
	reference operator []( difference_type dist ){ return( *( *this += dist ) ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�� const �Q��
	const_reference operator []( difference_type dist ) const { return( *( *this += dist ) ); }


	/// @brief �O�u�^�̃C���N�������g���Z�q
	mist_iterator2& operator ++( )
	{
		*this += 1;
		return( *this );
	}

	/// @brief ��u�^�̃C���N�������g���Z�q
	const mist_iterator2 operator ++( int )
	{
		mist_iterator2 old_val( *this );
		*this += 1;
		return( old_val );
	}

	/// @brief �O�u�^�̃f�N�������g���Z�q
	mist_iterator2& operator --( )
	{
		*this -= 1;
		return( *this );
	}

	/// @brief ��u�^�̃f�N�������g���Z�q
	const mist_iterator2 operator --( int )
	{
		mist_iterator2 old_val( *this );
		*this -= 1;
		return( old_val );
	}

	/// @brief dist ���� + �����ɗ��ꂽ�ʒu�̗v�f�Ɉړ�����
	const mist_iterator2& operator +=( difference_type dist )
	{
		index_ += dist;
		return( *this );
	}

	/// @brief dist ���� - �����ɗ��ꂽ�ʒu�̗v�f�Ɉړ�����
	const mist_iterator2& operator -=( difference_type dist )
	{
		index_ -= dist;
		return( *this );
	}


	/// @brief �v�f�Ԃ̃|�C���^�̍����v�Z����
	template< class TT, class DD, class PP, class RR >
	const difference_type operator -( const mist_iterator2< TT, DD, PP, RR > &ite ) const
	{
		return( index_ - ite.index( ) );
	}

	/// @brief ��̃C�e���[�^�������v�f�������Ă��邩�ǂ���
	template< class TT, class DD, class PP, class RR >
	bool operator ==( const mist_iterator2< TT, DD, PP, RR > &ite ) const { return( *this - ite == 0 ); }

	/// @brief ��̃C�e���[�^�������v�f�������Ă��Ȃ����ǂ���
	template< class TT, class DD, class PP, class RR >
	bool operator !=( const mist_iterator2< TT, DD, PP, RR > &ite ) const { return( !( *this == ite ) ); }

	/// @brief ��̃C�e���[�^�� < �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator < ( const mist_iterator2< TT, DD, PP, RR > &ite ) const { return( *this - ite < 0 ); }

	/// @brief ��̃C�e���[�^�� <= �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator <=( const mist_iterator2< TT, DD, PP, RR > &ite ) const { return( !( *this > ite ) ); }

	/// @brief ��̃C�e���[�^�� > �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator > ( const mist_iterator2< TT, DD, PP, RR > &ite ) const { return( ite < *this ); }

	/// @brief ��̃C�e���[�^�� >= �̊֌W�𒲂ׂ�
	template< class TT, class DD, class PP, class RR >
	bool operator >=( const mist_iterator2< TT, DD, PP, RR > &ite ) const { return( !( *this < ite ) ); }
};



/// @brief dist ���� + �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief dist ���� + �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator +( typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist, const mist_iterator2< T, Distance, Pointer, Reference > &ite )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) += dist );
}

/// @brief dist ���� - �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T, class Distance, class Pointer, class Reference >
inline const mist_iterator2< T, Distance, Pointer, Reference > operator -( const mist_iterator2< T, Distance, Pointer, Reference > &ite, typename mist_iterator2< T, Distance, Pointer, Reference >::difference_type dist )
{
	return( mist_iterator2< T, Distance, Pointer, Reference >( ite ) -= dist );
}



/// @brief �C�e���[�^����t���������_���A�N�Z�X�C�e���[�^�𐶐�����
//! 
//! �S�Ă�MIST�R���e�i�ŗ��p����t���������_���A�N�Z�X�C�e���[�^
//! 
//! @param _Ite �c ��{�ƂȂ鏇���������_���A�N�Z�X�C�e���[�^
//! 
template< class T >
class mist_reverse_iterator :
#if defined( __MIST_MSVC__ ) && __MIST_MSVC__ <= 6
	public std::iterator<
		typename T::iterator_category,
		typename T::value_type,
		typename T::difference_type
	>
#else
	public std::iterator<
		typename T::iterator_category,
		typename T::value_type,
		typename T::difference_type,
		typename T::pointer,
		typename T::reference
	>
#endif
{
private:
	typedef T iterator_type;

public:
 	typedef typename T::size_type size_type;					///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
 	typedef typename T::difference_type difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename T::value_type value_type;					///< @brief ///< @brief �����f�[�^�^�Dbool �Ɠ���
	typedef typename T::pointer pointer;						///< @brief �f�[�^�^�̃|�C���^�[�^�Ddata �̏ꍇ�Cdata * �ƂȂ�
	typedef typename T::reference reference;					///< @brief �f�[�^�^�̎Q�ƁDdata �̏ꍇ�Cdata & �ƂȂ�
	typedef typename T::const_reference const_reference;		///< @brief �f�[�^�^�� const �Q�ƁDdata �̏ꍇ�Cconst data & �ƂȂ�

protected:
	iterator_type current_iterator_;		///< @brief �����������_���A�N�Z�X�C�e���[�^

public:
	/// @brief �f�t�H���g�R���X�g���N�^
	mist_reverse_iterator( ){ }

	/// @brief �����������_���A�N�Z�X�C�e���[�^��p���ď���������
	mist_reverse_iterator( const iterator_type &ite ) : current_iterator_( ite ){ }

	/// @brief �t���������_���A�N�Z�X�C�e���[�^��p���ď���������
	mist_reverse_iterator( const mist_reverse_iterator &ite ) : current_iterator_( ite.current_iterator_ ){ }


	/// @brief �����������_���A�N�Z�X�C�e���[�^��������
	const mist_reverse_iterator& operator =( const iterator_type &ite )
	{
		current_iterator_ = ite;
		return( *this );
	}

	/// @brief �t���������_���A�N�Z�X�C�e���[�^��������
	const mist_reverse_iterator& operator =( const mist_reverse_iterator &ite )
	{
		current_iterator_ = ite.current_iterator_;
		return( *this );
	}


	/// @brief �v�f�̎Q��
	reference operator *()
	{
		iterator_type _tmp = current_iterator_;
		return( *( --_tmp ) );
	}

	/// @brief �v�f�� const �Q��
	const_reference operator *() const
	{
		iterator_type _tmp = current_iterator_;
		return( *( --_tmp ) );
	}

	/// @brief �v�f�̃A�h���X���Z�q
	pointer operator->( ){ return( &**this ); }

	/// @brief �v�f�� const �A�h���X���Z�q
	const pointer operator->( ) const { return( &**this ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�̎Q��
	reference operator []( difference_type dist ){ return( *( *this + dist ) ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�� const �Q��
	const_reference operator []( difference_type dist ) const { return( *( *this + dist ) ); }


	/// @brief �O�u�^�̃C���N�������g���Z�q
	mist_reverse_iterator& operator ++( )
	{
		--current_iterator_;
		return( *this );
	}

	/// @brief ��u�^�̃C���N�������g���Z�q
	const mist_reverse_iterator operator ++( int )
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_--;
		return( old_val );
	}

	/// @brief �O�u�^�̃f�N�������g���Z�q
	mist_reverse_iterator& operator --( )
	{
		++current_iterator_;
		return( *this );
	}

	/// @brief ��u�^�̃f�N�������g���Z�q
	const mist_reverse_iterator operator --( int )
	{
		mist_reverse_iterator old_val( *this );
		current_iterator_++;
		return( old_val );
	}

	/// @brief dist ���� + �����ɗ��ꂽ�ʒu�̗v�f�Ɉړ�����
	const mist_reverse_iterator& operator +=( difference_type dist )
	{
		current_iterator_ -= dist;
		return( *this );
	}

	/// @brief dist ���� - �����ɗ��ꂽ�ʒu�̗v�f�Ɉړ�����
	const mist_reverse_iterator& operator -=( difference_type dist )
	{
		current_iterator_ += dist;
		return( *this );
	}

	/// @brief �v�f�Ԃ̃|�C���^�̍����v�Z����
	const difference_type operator -( const mist_reverse_iterator &ite ) const
	{
		return( ite.current_iterator_ - current_iterator_ );
	}



	/// @brief ��̃C�e���[�^�������v�f�������Ă��邩�ǂ���
	bool operator ==( const mist_reverse_iterator &ite ) const { return( current_iterator_ == ite.current_iterator_ ); }

	/// @brief ��̃C�e���[�^�������v�f�������Ă��Ȃ����ǂ���
	bool operator !=( const mist_reverse_iterator &ite ) const { return( !( *this == ite ) ); }

	/// @brief ��̃C�e���[�^�� < �̊֌W�𒲂ׂ�
	bool operator < ( const mist_reverse_iterator &ite ) const { return( ite.current_iterator_ < current_iterator_  ); }

	/// @brief ��̃C�e���[�^�� <= �̊֌W�𒲂ׂ�
	bool operator <=( const mist_reverse_iterator &ite ) const { return( !( *this > ite ) ); }

	/// @brief ��̃C�e���[�^�� > �̊֌W�𒲂ׂ�
	bool operator > ( const mist_reverse_iterator &ite ) const { return( ite < *this ); }

	/// @brief ��̃C�e���[�^�� >= �̊֌W�𒲂ׂ�
	bool operator >=( const mist_reverse_iterator &ite ) const { return( !( *this < ite ) ); }
};


/// @brief dist ���� + �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T >
inline const mist_reverse_iterator< T > operator +( const mist_reverse_iterator< T > &ite1, const mist_reverse_iterator< T > ite2 )
{
	return( mist_reverse_iterator< T >( ite1 ) += ite2 );
}

/// @brief dist ���� + �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T >
inline const mist_reverse_iterator< T > operator +( const mist_reverse_iterator< T > &ite, typename T::difference_type dist )
{
	return( mist_reverse_iterator< T >( ite ) += dist );
}

/// @brief dist ���� - �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T >
inline const mist_reverse_iterator< T > operator +( typename T::difference_type dist, const mist_reverse_iterator< T > &ite )
{
	return( mist_reverse_iterator< T >( ite ) += dist );
}


//template< class T >
//inline const typename mist_reverse_iterator< T >::difference_type operator -( const mist_reverse_iterator< T > &ite1, const mist_reverse_iterator< T > ite2 )
//{
//	return( ite1 - ite2 );
//}


/// @brief dist ���� - �����ɃC�e���[�^��i�߂��C�e���[�^��Ԃ�
template< class T >
inline const mist_reverse_iterator< T > operator -( const mist_reverse_iterator< T > &ite, typename T::difference_type dist )
{
	return( mist_reverse_iterator< T >( ite ) -= dist );
}

// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_ITERATOR_H__
