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
	mist_iterator1( const mist_iterator1 &ite ) : data_( ite.data_ ), diff_pointer_( ite.diff_pointer_ ){ }


	/// @brief ���̃C�e���[�^��������
	const mist_iterator1& operator =( const mist_iterator1 &ite )
	{
		data_ = ite.data_;
		diff_pointer_ = ite.diff_pointer_;
		return( *this );
	}


	/// @brief �v�f�̎Q��
	reference operator *(){ return( *data_ ); }

	/// @brief �v�f�� const �Q��
	const_reference operator *() const { return( *data_ ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�̎Q��
	reference operator []( difference_type dist ){ return( data_[ dist * diff_pointer_ ] ); }

	/// @brief dist �Ŏw�肵���ʒu�̗v�f�� const �Q��
	const_reference operator []( difference_type dist ) const { return( data_[ dist * diff_pointer_ ] ); }


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
	const difference_type operator -( const mist_iterator1 &ite ) const
	{
		return( ( data_ - ite.data_ ) / diff_pointer_ );
	}


	/// @brief ��̃C�e���[�^�������v�f�������Ă��邩�ǂ���
	bool operator ==( const mist_iterator1 &ite ) const { return( data_ == ite.data_ ); }

	/// @brief ��̃C�e���[�^�������v�f�������Ă��Ȃ����ǂ���
	bool operator !=( const mist_iterator1 &ite ) const { return( !( *this == ite )  ); }

	/// @brief ��̃C�e���[�^�� < �̊֌W�𒲂ׂ�
	bool operator < ( const mist_iterator1 &ite ) const { return( data_ <  ite.data_ ); }

	/// @brief ��̃C�e���[�^�� <= �̊֌W�𒲂ׂ�
	bool operator <=( const mist_iterator1 &ite ) const { return( data_ <= ite.data_ ); }

	/// @brief ��̃C�e���[�^�� > �̊֌W�𒲂ׂ�
	bool operator > ( const mist_iterator1 &ite ) const { return( data_ >  ite.data_ ); }

	/// @brief ��̃C�e���[�^�� >= �̊֌W�𒲂ׂ�
	bool operator >=( const mist_iterator1 &ite ) const { return( data_ >= ite.data_ ); }
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
	pointer data_;				///< @brief �ϐ��̐���������
	difference_type index_;		///< @brief �ϐ��̐���������
	difference_type width_;		///< @brief �ϐ��̐���������
	difference_type step_;		///< @brief �ϐ��̐���������

public:
	/// @brief �f�t�H���g�R���X�g���N�^
	mist_iterator2( pointer p = NULL, difference_type index = 0, difference_type width = 1, difference_type step = 1 )
												: data_( p ), index_( index ), width_( width ), step_( step )
	{
	}

	/// @brief ���̃C�e���[�^��p���ď���������
	mist_iterator2( const mist_iterator2 &ite ) : data_( ite.data_ ), index_( ite.index_ ), width_( ite.width_ ), step_( ite.step_ )
	{
	}


	/// @brief ���̃C�e���[�^��������
	const mist_iterator2& operator =( const mist_iterator2 &ite )
	{
		if( &ite != this )
		{
			data_  = ite.data_;
			index_ = ite.index_;
			width_ = ite.width_;
			step_  = ite.step_;
		}
		return( *this );
	}


	/// @brief �v�f�̎Q��
	reference operator *()
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

	/// @brief �v�f�� const �Q��
	const_reference operator *() const
	{
		difference_type step = index_ / width_;
		difference_type rest = index_ - step * width_;
		return( *( data_ + rest + step * step_ ) );
	}

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
	const difference_type operator -( const mist_iterator2 &ite ) const
	{
		return( index_ - ite.index_ );
	}

	/// @brief ��̃C�e���[�^�������v�f�������Ă��邩�ǂ���
	bool operator ==( const mist_iterator2 &ite ) const { return( *this - ite == 0 ); }

	/// @brief ��̃C�e���[�^�������v�f�������Ă��Ȃ����ǂ���
	bool operator !=( const mist_iterator2 &ite ) const { return( !( *this == ite ) ); }

	/// @brief ��̃C�e���[�^�� < �̊֌W�𒲂ׂ�
	bool operator < ( const mist_iterator2 &ite ) const { return( *this - ite < 0 ); }

	/// @brief ��̃C�e���[�^�� <= �̊֌W�𒲂ׂ�
	bool operator <=( const mist_iterator2 &ite ) const { return( !( *this > ite ) ); }

	/// @brief ��̃C�e���[�^�� > �̊֌W�𒲂ׂ�
	bool operator > ( const mist_iterator2 &ite ) const { return( ite < *this ); }

	/// @brief ��̃C�e���[�^�� >= �̊֌W�𒲂ׂ�
	bool operator >=( const mist_iterator2 &ite ) const { return( !( *this < ite ) ); }
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
