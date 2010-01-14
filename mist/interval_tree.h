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

/// @file mist/interval_tree.h
//!
//! @brief Interval-tree �̂��߂̃��C�u����
//!

#ifndef __INCLUDE_INTERVAL_TREE_H__
#define __INCLUDE_INTERVAL_TREE_H__

#include <iostream>
#include <vector>
#include <algorithm>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN

//! @addtogroup interval_tree_group Interval-tree
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/interval_tree.h>
//! @endcode
//!
//!  @{

template< typename M, typename T >	class tagged_section;
template< typename M, typename T >	class tagged_section_min_less;
template< typename M, typename T >	class tagged_section_max_greater;

/// @brief ��Ԃ̏W������C�ӂ̒l���܂܂���Ԃ݂̂������ɒT�����邽�߂̃N���X
//!
//! ��Ԃ̏W������2���؂��\�z���A�ʏ�O(n)������T�����Ԃ�O(log n)�ɍ팸����D
//! ������ʕ��̏d�Ȃ蔻��ȂǂɗL���D
//!
//! @attention mist::tagged_section< M, T >�^��std::vetor����͂Ƃ���D
//! @attention mist::tagged_section< M, T >::tag_type�^��std::vetor��T�����ʂ̏o�͂Ƃ���D
//! 
//! @param S  �c ��Ԃ̃f�[�^�^�i mist::tagged_section< M, T >�^ �j
//! @param V  �c ��Ԃ̒T���̍ۂɓ��͂���C�ӂ̒l�̃f�[�^�^�i double �� float �ȂǁCmist::rgb< M, T >::min_max_type�Ɠ������A���邢�͂�萸�x�̍����^ �j
//!
//! @code �g�p��
//! // ��Ԃ̏W���̗p��
//! typedef mist::tagged_section< int, size_t > section_type;
//! std::vector< section_type > secs; // int�^�̍ő�l�C�ŏ��l�ɂ͂��܂ꂽ��Ԃ̏W���C�e��Ԃɂ�size_t�^�̃^�O���t������D
//! secs.push_back( section_type( -1, 5, 0 ) );�@// �^�O0���t�������[ -1, 5 )���W���ɒǉ��D
//! /* 
//!		��Ԃ̏W���֒ǉ�����
//! */
//! 
//! // Interval-tree�̍\�z
//! typedef mist::interval_tree< section_type, double > i_tree_type;
//! mist::i_tree_type i_tree( secs );
//!
//! // �C�ӂ̒l���܂܂���Ԃ̃^�O�̏W���������ɒT��
//! std::vector< section_type::tag_type > tags;
//! i_tree.find( 0.0, tags ); // 0.0 ���܂܂���Ԃ̃^�O�̏W���𓾂�
//!
//! // �T�����ʂ̕\��
//! for( size_t i = 0 ; i < tags.size( ) ; i ++ )
//! {
//!		std::cout << secs[ tags[ i ] ] << std::endl;
//! }
//! @endcode
//!
template< typename S, typename V >
class interval_tree
{
public:
	typedef S							section_type;	///< @brief ��Ԃ�\���^
	typedef typename S::min_max_type	min_max_type;	///< @brief ��Ԃ̗��[�̒l��\���^
	typedef typename S::tag_type		tag_type;		///< @brief ��Ԃ̃^�O������\���^
	typedef V							value_type;		///< @brief ��Ԃ̒T���̍ۂɓ��͂����l��\���^

	class node;

	/// @brief ���͂��ꂽ�l���܂܂���Ԃ̒T��
	//!
	//! @param[in]	val		�c ���́i���̒l���ԂɊ܂ދ�Ԃ�T���j
	//! @param[out] tags	�c �T�����ꂽ��Ԃ̃^�O���̏W��
	//!
	void find( const value_type &val, std::vector< tag_type > &tags )
	{
		tags.resize( 0 );
		_find( val, tags, root_ );
	}

	/// @brief Interval-tree�̍\�z
	//!
	//! @param[in] secs   �c ��Ԃ̏W��
	//! @return �؂��������\�z���ꂽ�ꍇ�ɐ^��Ԃ�
	//!
	bool construct( const std::vector< section_type > &secs )
	{
		if( root_ != NULL ) 
		{ 
			destruct( ); 
		}
		if( secs.size( ) > 0 ) 
		{
			size_t tree_size = 0;
			root_= new node( secs, tree_size );
			return ( tree_size == secs.size( ) );
		}
		destruct( );
		return false;
	}

	/// @brief Interval-tree�̔j��
	void destruct( )
	{
		if( root_ != NULL ) 
		{ 
			delete root_; 
			root_ = NULL; 
		}
	}

	/// @brief �f�t�H���g�R���X�g���N�^
	interval_tree( ) : root_( NULL )
	{ 
	}

	/// @brief �R���X�g���N�^�iInterval-tree�̍\�z�j
	//!
	//! @param[in] secs �c ��ԏW��
	//!
	interval_tree( const std::vector< section_type > &secs ) : root_( NULL ) 
	{ 
		construct( secs ); 
	}

	/// @brief �R�s�[�R���X�g���N�^
	//!
	//! @param[in] it �c interval_tree�I�u�W�F�N�g
	//!
	interval_tree( const interval_tree< section_type, value_type > &it ) : root_( NULL )
	{
		*this = it;
	}

	/// @brief �f�X�g���N�^
	~interval_tree( ) 
	{ 
		destruct( ); 
	}

	/// @brief ������Z�q
	//!
	//! @param[in] it �c interval_tree�I�u�W�F�N�g
	//!
	interval_tree &operator =( const interval_tree< section_type, value_type > &it )
	{
		if( &it == this )
		{
			return *this;
		}
		else
		{
			destruct( );
			if( it.root_ != NULL )
			{
				root_ = new node( *it.root_ );
			}
			return *this;
		}
	}

private:
	void _find( const value_type &val, std::vector< tag_type > &tags, node *p )
	{
		if( p != NULL )
		{
			if( val < p->val( ) )
			{	
				for( size_t i = 0 ; i < p->min_inc_secs_size( ) ; i ++ )
				{
					if( static_cast< value_type >( p->min_inc_secs( i ).minimum( ) ) > val )
					{
						break;
					}
					tags.push_back( p->min_inc_secs( i ).tag( ) );
				}
				_find( val, tags, p->left( ) );
			}
			else if( val > p->val( ) )
			{
				for( size_t i = 0 ; i < p->max_dec_secs_size( ) ; i ++ )
				{
					if( static_cast< value_type >( p->max_dec_secs( i ).maximum( ) ) <= val )
					{
						break;
					}
					tags.push_back( p->max_dec_secs( i ).tag( ) );
				}
				_find( val, tags, p->right( ) );
			}
			else
			{
				for( size_t i = 0 ; i < p->min_inc_secs_size( ) ; i ++ )
				{
					tags.push_back( p->min_inc_secs( i ).tag( ) );
				}
			}
		} 
	}

	node *root_;
};


/// @brief Interval-tree�̂��߂̃^�O���t����Ԃ�\���N���X
//!
//! ��Ԃ̗��[��\��2�̒l�i�ő�l�C�ŏ��l�j�Ƌ�Ԃ����ʂ��邽�߂̃^�O�������D
//!
//! @attention [ min, max )�ŕ\������Ԃ�ΏۂƂ���D
//! 
//! @param M �c ��Ԃ̍ő�l�C�ŏ��l�̌^
//! @param T �c ��Ԃɕt������^�O���̃f�[�^�^
//!
template< typename M, typename T >
class tagged_section
{
public:
	typedef M min_max_type;		///< @brief �ő�l�C�ŏ��l��\���^
	typedef T tag_type;			///< @brief �^�O����\���^

	/// @brief ��Ԃ̍ŏ��l��Ԃ�
	//!
	//! @return ��Ԃ̍ŏ��l
	//!
	const min_max_type	&minimum( ) const { return min_; }
	
	/// @brief ��Ԃ̍ő�l��Ԃ�
	//!
	//! @return ��Ԃ̍ő�l
	//!
	const min_max_type	&maximum( ) const { return max_; }
	
	/// @brief ��Ԃ̃^�O����Ԃ�
	//!
	//! @return ��Ԃ̃^�O���
	//!
	const tag_type		&tag( ) const { return tag_; }
	
	/// @brief ��Ԃ̍ŏ��l��Ԃ��i���o�͉\�j
	//!
	//! @return ��Ԃ̍ŏ��l
	//!
	min_max_type	&minimum( ) { return min_; }
	
	/// @brief ��Ԃ̍ő�l��Ԃ��i���o�͉\�j
	//!
	//! @return ��Ԃ̍ő�l
	//!
	min_max_type	&maximum( ) { return max_; }
	
	/// @brief ��Ԃ̃^�O����Ԃ��i���o�͉\�j
	//!
	//! @return ��Ԃ̃^�O���
	//!
	tag_type		&tag( ) { return tag_; }

	/// @brief �f�t�H���g�R���X�g���N�^
	tagged_section( ) : min_( min_max_type( ) ), max_( min_max_type( ) ), tag_( tag_type( ) ) { }
	
	/// @brief �R���X�g���N�^
	//!
	//! @param[in] min �c �ŏ��l
	//! @param[in] max �c �ő�l
	//! @param[in] tag �c �^�O���
	//!
	tagged_section( const min_max_type &min, const min_max_type &max, const tag_type &tag ) : min_( min ), max_( max ), tag_( tag ) { }
	
private:
	min_max_type	min_;
	min_max_type	max_;
	tag_type		tag_;
};

/// @brief �^�O�t����ԃI�u�W�F�N�g�̏o�͉��Z�q
//!
//! @param[in,out] out   �c �o�̓X�g���[��
//! @param[in]     in    �c ���
//! @return �o�̓X�g���[��
//!
template< typename M, typename T >
inline std::ostream &operator <<( std::ostream &out, const tagged_section< M, T > &in )
{
	return out << in.tag( ) << " [" << in.minimum( ) << ", " << in.maximum( ) << ")";
}

/// @brief ��ԏW���̃\�[�g�ɗp����֐��I�u�W�F�N�g
template< typename M, typename T > 
class tagged_section_min_less	
{ 
public: 
	bool operator ( )( const tagged_section< M, T > &s0, const tagged_section< M, T > &s1 ) const { return s0.minimum( ) < s1.minimum( ); } 
};

/// @brief ��ԏW���̃\�[�g�ɗp����֐��I�u�W�F�N�g
template< typename M, typename T >
class tagged_section_max_greater	
{ 
public: 
	bool operator ( )( const tagged_section< M, T > &s0, const tagged_section< M, T > &s1 ) const { return s0.maximum( ) > s1.maximum( ); } 
};


/// @brief Interval-tree�̃m�[�h��\���N���X�iinterval_tree�N���X�̓����ł̂ݎg�p�j
//!
//! @param S  �c ��Ԃ̃f�[�^�^�i mist::tagged_section< M, T >�^ �j
//! @param V  �c ��ԂɊ܂܂��C�ӂ̒l�̃f�[�^�^�i double �� float �ȂǁCmist::rgb< M, T >::min_max_type�Ɠ������A���邢�͂�萸�x�̍����^ �j
//!
template< typename S, typename V >
class interval_tree< S, V >::node
{
	friend class interval_tree< S, V >;

private:
	const value_type	&val( )							const { return val_; } 
	const section_type	&min_inc_secs( const size_t i )	const { return min_inc_secs_[ i ]; }
	const section_type	&max_dec_secs( const size_t i )	const { return max_dec_secs_[ i ]; }
	size_t		min_inc_secs_size( )					const { return min_inc_secs_.size( ); }
	size_t		max_dec_secs_size( )					const { return max_dec_secs_.size( ); }
	
	node *left( )	{ return left_; }
	node *right( )	{ return right_; }

	node( const std::vector< section_type > &secs, size_t &tree_size ) : left_( NULL ), right_( NULL )
	{ 
		val_ = middle( secs ); 
		std::vector< section_type > l_secs, r_secs;
		for( size_t i = 0 ; i < secs.size( ) ; i ++ )
		{
			if( static_cast< value_type >( secs[ i ].maximum( ) ) <= val_ )
			{
				l_secs.push_back( secs[ i ] );
				continue;
			}
			if( static_cast< value_type >( secs[ i ] .minimum( ) ) > val_ )
			{
				r_secs.push_back( secs[ i ] );
				continue;
			}
			min_inc_secs_.push_back( secs[ i ] );
		}
		tree_size += min_inc_secs_.size( );
		std::sort( min_inc_secs_.begin( ), min_inc_secs_.end( ), tagged_section_min_less< min_max_type, tag_type >( ) );
		max_dec_secs_ = min_inc_secs_; 
		std::sort( max_dec_secs_.begin( ), max_dec_secs_.end( ), tagged_section_max_greater< min_max_type, tag_type >( ) );
		if( l_secs.size( ) > 0 )
		{
			left_ = new node( l_secs, tree_size );
		}
		if( r_secs.size( ) > 0 )
		{
			right_ = new node( r_secs, tree_size );
		}
	}

	node( const node &nd ) : left_( NULL ), right_( NULL )
	{
		*this = nd;	
	}

	~node( )
	{ 
		if( left_ != NULL )
		{
			delete left_;
			left_ = NULL;
		}
		if( right_ != NULL )
		{
			delete right_;
			right_ = NULL;
		}
	}

	node &operator =( const node &nd )
	{
		if( &nd == this )
		{
			return *this;
		}
		else
		{
			delete left_;
			delete right_;
			val_			= nd.val_;
			min_inc_secs_	= nd.min_inc_secs_;
			max_dec_secs_	= nd.max_dec_secs_;
			if( nd.left_ != NULL )
			{
				left_	= new node( *nd.left_ );
			}
			if( nd.right_ != NULL )
			{
				right_	= new node( *nd.right_ );
			}
			return *this;
		}
	}

	value_type middle( const std::vector< section_type > &secs ) const
	{
		typename section_type::min_max_type minimum = secs[ 0 ].minimum( );
		typename section_type::min_max_type maximum = secs[ 0 ].maximum( );
		for( size_t i = 1 ; i < secs.size( ) ; i ++ )
		{
			minimum = ( secs[ i ].minimum( ) < minimum ) ? secs[ i ].minimum( ) : minimum;
			maximum = ( secs[ i ].maximum( ) > maximum ) ? secs[ i ].maximum( ) : maximum;
		}

		return( static_cast< value_type >( ( minimum + maximum ) / 2.0 ) );
	}

	value_type					val_;
	std::vector< section_type >	min_inc_secs_;
	std::vector< section_type >	max_dec_secs_;
	node						*left_;
	node						*right_;
};


/// @}
//  Interval-tree �O���[�v�̏I���

// mist���O��Ԃ̏I���
_MIST_END

#endif // #ifndef __INCLUDE_INTERVAL_TREE_H__
