// 
// Copyright (c) 2003-2008, MIST Project, Intelligent Media Integration COE, Nagoya University
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

/// @file mist/config/binary.h
//!
//! @brief �o�C�i�����Z���s�����߂̃N���X
//!

#ifndef __INCLUDE_MIST_FACET__
#define __INCLUDE_MIST_FACET__



#ifndef __INCLUDE_MIST_CONF_H__
#include "config/mist_conf.h"
#endif

#ifndef __INCLUDE_MIST_TYPE_TRAIT_H__
#include "config/type_trait.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <vector>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


/// @brief �|���S���i�O�p�`�j�������N���X
//! 
//! @param T �c �����ŗp����f�[�^�^
//! 
template < class T >
class facet
{
public:
	typedef T value_type;										///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef size_t size_type;									///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;							///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef vector3< T > vector_type;							///< @brief ���_�Ȃǂ�\��3�����x�N�g���^
	typedef typename float_type< T >::value_type float_type;	///< @brief �����Ȃǂ��v�Z����Ƃ��ɗp���镂�������_�^

public:
	vector_type normal;	///< @brief �@��
	vector_type p1;		///< @brief ���W1
	vector_type p2;		///< @brief ���W2
	vector_type p3;		///< @brief ���W3

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�v�f���f�t�H���g�l�ŏ���������j
	facet( ) : normal( 0, 0, 1 ){ }

	/// @brief �w�肵���l��p���ď���������
	facet( const vector_type &N, const vector_type &P1, const vector_type &P2, const vector_type &P3 ) : normal( N ), p1( P1 ), p2( P2 ), p3( P3 ){ }

	/// @brief ���̃|���S���I�u�W�F�N�g��p���ď���������
	template < class TT >
	facet( const facet< TT > &f ) : normal( f.normal ), p1( f.p1 ), p2( f.p2 ), p3( f.p3 ){ }


	/// @brief ���̃|���S���I�u�W�F�N�g��������
	template < class TT >
	const facet &operator =( const facet< TT > &f )
	{
		if( &f != this )
		{
			normal = q.normal;
			p1     = f.p1;
			p2     = f.p2;
			p3     = f.p3;
		}
		return ( *this );
	}
};


/// @brief �w�肳�ꂽ�X�g���[���ɁC�|���S���I�u�W�F�N�g�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     f   �c �|���S���I�u�W�F�N�g
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! ( 1, 2, 3, 4 )
//! @endcode
//! 
template < class T > inline std::ostream &operator <<( std::ostream &out, const facet< T > &f )
{
	out << "(" << f.normal << ") - ";
	out << "(" << f.p1 << "), ";
	out << "(" << f.p2 << "), ";
	out << "(" << f.p3 << ")";
	return( out );
}


/// @brief �|���S���i�O�p�`�j�̏W���������N���X
//! 
//! @param T �c �����ŗp����f�[�^�^
//! 
template < class T >
class facet_list : public std::vector< facet< T > >
{
private:
	typedef std::vector< facet< T > > base;

public:
	typedef facet< T > facet_type;								///< @brief �|���S���^�C�v
	typedef size_t size_type;									///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;							///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename facet_type::vector_type vector_type;		///< @brief ���_�Ȃǂ�\��3�����x�N�g���^
	typedef typename float_type< T >::value_type float_type;	///< @brief �����Ȃǂ��v�Z����Ƃ��ɗp���镂�������_�^

public:
	std::string name;	///< @brief �@��

	/// @brief �f�t�H���g�R���X�g���N�^�i�S�v�f���f�t�H���g�l�ŏ���������j
	facet_list( ){ }

	/// @brief �w�肵�����O�Ń|���S���W��������������
	facet_list( const std::string &name_ ) : name( name_ ){ }

	/// @brief ���̃|���S���I�u�W�F�N�g��p���ď���������
	template < class TT >
	facet_list( const facet_list< TT > &f ) : base( f ), name( f.name ){ }


	/// @brief ���̃|���S���I�u�W�F�N�g��������
	template < class TT >
	const facet_list &operator =( const facet_list< TT > &f )
	{
		if( &f != this )
		{
			base::operator =( f );
			name = q.name;
		}
		return ( *this );
	}
};

template < class T, class T1, class T2 >
inline bool convert_to_index_list( const facet_list< T > &facets, std::vector< vector3< T1 > > &vertices, std::vector< vector3< T2 > > &indices )
{
	if( facets.empty( ) )
	{
		return( false );
	}

	typedef typename facet_list< T >::size_type size_type;
	typedef vector3< T1 > vector_type;
	typedef vector3< T2 > ivector_type;

	vertices.clear( );
	indices.clear( );
	vertices.reserve( facets.size( ) * 3 );
	indices.reserve( facets.size( ) );

	for( size_type i = 0 ; i < facets.size( ) ; i++ )
	{
		const typename facet_list< T >::facet_type &f = facets[ i ];

		size_type indx[ 3 ] = { 0, 0, 0 };
		vector_type v[ 3 ] = { f.p1, f.p2, f.p3 };

		for( size_type j = 0 ; j < 3 ; j++ )
		{
			if( vertices.empty( ) )
			{
				indx[ j ] = vertices.size( );
				vertices.push_back( v[ j ] );
			}
			else
			{
				size_type k = 0;
				for( ; k < vertices.size( ) ; k++ )
				{
					if( ( vertices[ k ] - v[ j ] ).length( ) < 1.0e-12 )
					{
						break;
					}
				}

				if( k < vertices.size( ) )
				{
					// �o�^�ς݂̒��_�𔭌�
					indx[ j ] = k;
				}
				else
				{
					// �V�K�ɒ��_��o�^����
					indx[ j ] = vertices.size( );
					vertices.push_back( v[ j ] );
				}
			}
		}

		indices.push_back( ivector_type( indx[ 0 ], indx[ 1 ], indx[ 2 ] ) );
	}

	return( true );
}


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_FACET__
