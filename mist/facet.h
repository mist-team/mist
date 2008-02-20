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

#ifndef __INCLUDE_MIST_MATRIX__
#include "matrix.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "vector.h"
#endif


#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>

// mist���O��Ԃ̎n�܂�
_MIST_BEGIN


#define __SHOW_FACET_DEBUG_INFORMATION__	0

//! @addtogroup facet_group �|���S���������N���X
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/facet.h>
//! @endcode
//!
//!  @{

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

	/// @brief �w�肵���l��p���ď���������i�_�̕��я��͔����v��������j
	facet( const vector_type &P1, const vector_type &P2, const vector_type &P3 ) : normal( ( ( P2 - P1 ).outer( P3 - P1 ) ).unit( ) ), p1( P1 ), p2( P2 ), p3( P3 ){ }

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

/// @brief 3�p�`�p�b�`�̏W�����璸�_�W���ƎO�p�`�p�b�`���\������C���f�b�N�X�W���ɕϊ�����
//!
//! ���ʂ��Ďg�p���钸�_��S�Ă܂Ƃ߂����_�W�����쐬����D
//!
//! @param[in]  facets   �c 3�p�`�p�b�`�̏W��
//! @param[in]  vertices �c ���_�W��
//! @param[in]  faces    �c �O�p�`�p�b�`���\������C���f�b�N�X�W��
//! @param[in]  eps      �c ���꒸�_�Ɣ��肳��钸�_�̋���
//!
//! @return ���_�W���ƃC���f�b�N�X�W���̍쐬�ɐ����������ǂ���
//!
template < class T, class T1, class T2 >
inline bool convert_to_vertex_face_list( const facet_list< T > &facets, std::vector< vector3< T1 > > &vertices, std::vector< vector3< T2 > > &faces, const double eps = 1.0e-6 )
{
	if( facets.empty( ) )
	{
		return( false );
	}

	typedef typename facet_list< T >::size_type size_type;
	typedef typename facet_list< T >::difference_type difference_type;
	typedef vector3< T1 > vector_type;
	typedef vector3< T2 > ivector_type;

	typedef std::pair< size_type, vector_type > value_type;
	typedef std::multimap< difference_type, value_type > map_type;
	typedef std::pair< difference_type, value_type > key_type;
	map_type table;

	faces.clear( );
	faces.reserve( facets.size( ) );

	// �f�[�^���e�[�u���ɓo�^����
	for( size_type i = 0 ; i < facets.size( ) ; i++ )
	{
		const typename facet_list< T >::facet_type &f = facets[ i ];
		size_type indx[ 3 ] = { 0, 0, 0 };
		vector_type v[ 3 ] = { f.p1, f.p2, f.p3 };

		for( size_type j = 0 ; j < 3 ; j++ )
		{
			const vector_type &vec = v[ j ];
			difference_type k1 = static_cast< difference_type > ( vec.x * 100000.0 );
			difference_type k2 = static_cast< difference_type > ( vec.y * 100000.0 );
			difference_type k3 = static_cast< difference_type > ( vec.z * 100000.0 );
			difference_type key = k1 ^ k2 ^ k3;

			typename map_type::iterator ite = table.find( key );
			if( ite == table.end( ) )
			{
				indx[ j ] = table.size( );
				table.insert( key_type( key, value_type( indx[ j ], vec ) ) );
			}
			else
			{
				typename map_type::iterator upper = table.upper_bound( key );
				for( ; ite != upper ; ++ite )
				{
					if( ( ite->second.second - vec ).length( ) < eps )
					{
						break;
					}
				}

				if( ite == upper )
				{
					indx[ j ] = table.size( );
					table.insert( key_type( key, value_type( indx[ j ], vec ) ) );
				}
				else
				{
					indx[ j ] = ite->second.first;
				}
			}
		}

		// ���_�𔽎��v���ɕ��ׂ�
		if( f.normal.inner( ( v[ 1 ] - v[ 0 ] ).outer( v[ 2 ] - v[ 0 ] ) ) < 0 )
		{
			size_type tmp = indx[ 1 ];
			indx[ 1 ] = indx[ 2 ];
			indx[ 2 ] = tmp;
		}

		if( indx[ 0 ] != indx[ 1 ] && indx[ 1 ] != indx[ 2 ] && indx[ 2 ] != indx[ 0 ] )
		{
			faces.push_back( ivector_type( indx[ 0 ], indx[ 1 ], indx[ 2 ] ) );
		}
	}

	vertices.clear( );
	vertices.resize( table.size( ) );
	for( typename map_type::iterator ite = table.begin( ) ; ite != table.end( ) ; ++ite )
	{
		vertices[ ite->second.first ] = ite->second.second;
	}


	return( true );
}


namespace __mc__
{
	struct __face__
	{
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;

		difference_type eid1;
		difference_type eid2;
		difference_type eid3;
		bool            flag;

		__face__( ) : eid1( 0 ), eid2( 0 ), eid3( 0 ), flag( true ){ }
	};

	template < class VERTEX >
	struct __edge__
	{
		typedef size_t    size_type;
		typedef ptrdiff_t difference_type;
		typedef VERTEX    vector_type;

		difference_type v1;
		difference_type v2;
		difference_type fid1;
		difference_type fid2;
		difference_type key;
		vector_type     v;
		double          err;

		__edge__( ) : fid1( 0 ), fid2( 0 ), key( -1 ), err( 0.0 ){ }
		__edge__( difference_type vv1, difference_type vv2 ) : v1( vv1 ), v2( vv2 ), fid1( 0 ), fid2( 0 ), key( -1 ), err( 0.0 ) { }
		__edge__( difference_type vv1, difference_type vv2, difference_type id1 ) : v1( vv1 ), v2( vv2 ), fid1( id1 ), fid2( 0 ), key( -1 ), err( 0.0 ) { }

		bool operator <( const __edge__ &v ) const
		{
			return( err < v.err );
		}
	};

	template < class EDGE >
	struct __edge_comp__
	{
		typedef EDGE edge_type;
		typedef typename edge_type::difference_type difference_type;

		const std::vector< edge_type > &edges;

		__edge_comp__( const std::vector< edge_type > &edge_list ) : edges( edge_list )
		{
		}

		bool operator ()( difference_type v1, difference_type v2 ) const
		{
			return( edges[ v1 ].err < edges[ v2 ].err );
		}
	};

	template< class T1, class T2 >
	inline T1 create_key( const T1 v1, const T1 v2, const T2 stride )
	{
		if( v1 < v2 )
		{
			return( v1 * stride + v2 );
		}
		else
		{
			return( v2 * stride + v1 );
		}
	}


	template< class T >
	inline T find_edge( const T V1, const T V2, const T V3, const T v1, const T v2 )
	{
		if( V1 == v1 )
		{
			if( V2 == v2 )
			{
				return( 0 );
			}
			else if( V3 == v2 )
			{
				return( 2 );
			}
		}
		else if( V2 == v1 )
		{
			if( V3 == v2 )
			{
				return( 1 );
			}
			else if( V1 == v2 )
			{
				return( 0 );
			}
		}
		else if( V3 == v1 )
		{
			if( V1 == v2 )
			{
				return( 2 );
			}
			else if( V2 == v2 )
			{
				return( 1 );
			}
		}

		return( -1 );
	}

	template < class T1, class T2, class T3 >
	inline bool convert_to_vertex_face_list( const std::vector< vector3< T1 > > &vertices, const std::vector< vector3< T2 > > &faces, std::vector< __face__ > &face_lists, std::vector< __edge__< T3 > > &edge_lists )
	{
		if( vertices.empty( ) || faces.empty( ) )
		{
			return( false );
		}

		typedef typename vector3< T1 >::size_type size_type;
		typedef typename vector3< T1 >::difference_type difference_type;
		typedef vector3< T1 >           vector_type;
		typedef vector3< T2 >           ivector_type;
		typedef __face__                face_type;
		typedef __edge__< T3 >          edge_type;

		typedef std::map< difference_type, difference_type > map_type;
		typedef map_type::iterator iterator;
		map_type table;

		edge_lists.clear( );
		face_lists.clear( );
		face_lists.reserve( faces.size( ) );

		// �_�~�[��1�����}������
		face_lists.push_back( face_type( ) );
		edge_lists.push_back( edge_type( ) );

		// �ӂ��e�[�u���ɓo�^����
		for( size_type i = 1 ; i <= faces.size( ) ; i++ )
		{
			const ivector_type &f = faces[ i - 1 ];

			difference_type key12 = create_key( f.x, f.y, vertices.size( ) );
			difference_type key23 = create_key( f.y, f.z, vertices.size( ) );
			difference_type key31 = create_key( f.z, f.x, vertices.size( ) );

			// ��1-2�𒲂ׂ�
			iterator ite = table.find( key12 );
			if( ite == table.end( ) )
			{
				// �V�K�ɕӂƖʂ̑Ή�����ǉ�����
				table[ key12 ] = edge_lists.size( );

				// �G�b�W��o�^����
				if( f.x < f.y )
				{
					edge_lists.push_back( edge_type( f.x, f.y, i ) );
				}
				else
				{
					edge_lists.push_back( edge_type( f.y, f.x, i ) );
				}
			}
			else if( edge_lists[ ite->second ].fid2 == 0 )
			{
				edge_lists[ ite->second ].fid2 = i;
			}
			else
			{
				std::cerr << "Edge may be shared among more than three faces." << std::endl;
			}

			// ��2-3�𒲂ׂ�
			ite = table.find( key23 );
			if( ite == table.end( ) )
			{
				// �V�K�ɕӏ���ǉ�����
				table[ key23 ] = edge_lists.size( );

				// �G�b�W��o�^����
				if( f.y < f.z )
				{
					edge_lists.push_back( edge_type( f.y, f.z, i ) );
				}
				else
				{
					edge_lists.push_back( edge_type( f.z, f.y, i ) );
				}
			}
			else if( edge_lists[ ite->second ].fid2 == 0 )
			{
				edge_lists[ ite->second ].fid2 = i;
			}
			else
			{
				std::cerr << "Edge may be shared among more than three faces." << std::endl;
			}

			// ��2-3�𒲂ׂ�
			ite = table.find( key31 );
			if( ite == table.end( ) )
			{
				// �V�K�ɕӏ���ǉ�����
				table[ key31 ] = edge_lists.size( );

				// �G�b�W��o�^����
				if( f.z < f.x )
				{
					edge_lists.push_back( edge_type( f.z, f.x, i ) );
				}
				else
				{
					edge_lists.push_back( edge_type( f.x, f.z, i ) );
				}
			}
			else if( edge_lists[ ite->second ].fid2 == 0 )
			{
				edge_lists[ ite->second ].fid2 = i;
			}
			else
			{
				std::cerr << "Edge may be shared among more than three faces." << std::endl;
			}
		}

		// �ʂƕӂ��֘A�t����
		for( size_type i = 1 ; i <= faces.size( ) ; i++ )
		{
			const ivector_type &F = faces[ i - 1 ];
			face_type f;

			difference_type key12 = create_key( F.x, F.y, vertices.size( ) );
			difference_type key23 = create_key( F.y, F.z, vertices.size( ) );
			difference_type key31 = create_key( F.z, F.x, vertices.size( ) );

			// ��1-2�𒲂ׂ�
			iterator ite = table.find( key12 );
			if( ite == table.end( ) )
			{
				std::cerr << "Can't find edge 1 from edge table." << std::endl;
			}
			else
			{
				edge_type &e = edge_lists[ ite->second ];
				f.eid1 = F.x == e.v1 ? ite->second : -ite->second;
			}

			// ��2-3�𒲂ׂ�
			ite = table.find( key23 );
			if( ite == table.end( ) )
			{
				std::cerr << "Can't find edge 2 from edge table." << std::endl;
			}
			else
			{
				edge_type &e = edge_lists[ ite->second ];
				f.eid2 = F.y == e.v1 ? ite->second : -ite->second;
			}

			// ��2-3�𒲂ׂ�
			ite = table.find( key31 );
			if( ite == table.end( ) )
			{
				std::cerr << "Can't find edge 3 from edge table." << std::endl;
			}
			else
			{
				edge_type &e = edge_lists[ ite->second ];
				f.eid3 = F.z == e.v1 ? ite->second : -ite->second;

				if( e.fid1 != i )
				{
					e.fid2 = i;
				}
			}

			face_lists.push_back( f );
		}

		return( true );
	}

	template < class T >
	inline T ABS( T val )
	{
		return( val < 0 ? -val : val );
	}

	template < class T >
	void contract_edges( std::vector< __edge__< T > > &edges, std::vector< __face__ > &faces, __face__::difference_type eid0, __face__::difference_type eid1, __face__::difference_type EID, __face__::difference_type FID )
	{
		typedef __edge__< T > edge_type;
		typedef __face__ face_type;
		typedef __face__::difference_type difference_type;

		difference_type fid2 = 0;
		const edge_type &E  = edges[ EID ];
		edge_type &e0 = edges[ eid0 ];
		edge_type &e1 = edges[ eid1 ];

		if( e0.fid1 == FID )
		{
			if( e1.fid1 == FID )
			{
				fid2 = e1.fid2;
				e0.fid1 = e1.fid2;
			}
			else if( e1.fid2 == FID )
			{
				fid2 = e1.fid1;
				e0.fid1 = e1.fid1;
			}
			else
			{
				std::cerr << "Face-edge correspondence is incorrect!!" << std::endl;
			}

		}
		else if( e0.fid2 == FID )
		{
			if( e1.fid1 == FID )
			{
				fid2 = e1.fid2;
				e0.fid2 = e1.fid2;
			}
			else if( e1.fid2 == FID )
			{
				fid2 = e1.fid1;
				e0.fid2 = e1.fid1;
			}
			else
			{
				std::cerr << "Face-edge correspondence is incorrect!!" << std::endl;
			}
		}
		else
		{
			std::cerr << "Unknown error occured!!" << std::endl;
		}

		if( fid2 != 0 )
		{
			face_type &f2 = faces[ fid2 ];

			if( ABS( f2.eid1 ) == eid1 )
			{
				if( e0.v1 == e1.v1 || e0.v2 == e1.v2 )
				{
					f2.eid1 = f2.eid1 < 0 ? -eid0 : eid0;
				}
				else if( e0.v1 == e1.v2 || e0.v2 == e1.v1 )
				{
					f2.eid1 = f2.eid1 < 0 ? eid0 : -eid0;
				}
				else
				{
					std::cerr << "Vertex-edge map is incorrect!! " << "(" << e1.v1 << ", " << e1.v2 << ") <-> (" << E.v1 << ", " << E.v2 << ")" << std::endl;
				}
			}
			else if( ABS( f2.eid2 ) == eid1 )
			{
				if( e0.v1 == e1.v1 || e0.v2 == e1.v2 )
				{
					f2.eid2 = f2.eid2 < 0 ? -eid0 : eid0;
				}
				else if( e0.v1 == e1.v2 || e0.v2 == e1.v1 )
				{
					f2.eid2 = f2.eid2 < 0 ? eid0 : -eid0;
				}
				else
				{
					std::cerr << "Vertex-edge map is incorrect!! " << "(" << e1.v1 << ", " << e1.v2 << ") <-> (" << E.v1 << ", " << E.v2 << ")" << std::endl;
				}
			}
			else if( ABS( f2.eid3 ) == eid1 )
			{
				if( e0.v1 == e1.v1 || e0.v2 == e1.v2 )
				{
					f2.eid3 = f2.eid3 < 0 ? -eid0 : eid0;
				}
				else if( e0.v1 == e1.v2 || e0.v2 == e1.v1 )
				{
					f2.eid3 = f2.eid3 < 0 ? eid0 : -eid0;
				}
				else
				{
					std::cerr << "Vertex-edge map is incorrect!! " << "(" << e1.v1 << ", " << e1.v2 << ") <-> (" << E.v1 << ", " << E.v2 << ")" << std::endl;
				}
			}
			else
			{
				std::cerr << "Face-edge correspondence is incorrect!!" << std::endl;
			}
		}
	}

	template < class T, class Allocator >
	inline bool inv4x4( matrix< T, Allocator > &a )
	{
		typedef typename matrix< T, Allocator >::value_type value_type;

		double a11 = a( 0, 0 ), a12 = a( 0, 1 ), a13 = a( 0, 2 ), a14 = a( 0, 3 );
		double a21 = a( 1, 0 ), a22 = a( 1, 1 ), a23 = a( 1, 2 ), a24 = a( 1, 3 );
		double a31 = a( 2, 0 ), a32 = a( 2, 1 ), a33 = a( 2, 2 ), a34 = a( 2, 3 );
		double a41 = a( 3, 0 ), a42 = a( 3, 1 ), a43 = a( 3, 2 ), a44 = a( 3, 3 );

		// ���ʂŗ��p����W���̌v�Z
		double _33x44_34x43_ = a33 * a44 - a34 * a43;
		double _34x42_32x44_ = a34 * a42 - a32 * a44;
		double _32x43_33x42_ = a32 * a43 - a33 * a42;
		double _31x44_34x41_ = a31 * a44 - a34 * a41;
		double _33x41_31x43_ = a33 * a41 - a31 * a43;
		double _31x42_32x41_ = a31 * a42 - a32 * a41;

		// �s�񎮂��v�Z
		double detA = a11 * ( a22 * _33x44_34x43_ + a23 * _34x42_32x44_ + a24 * _32x43_33x42_ )
					- a12 * ( a21 * _33x44_34x43_ - a23 * _31x44_34x41_ - a24 * _33x41_31x43_ )
					- a13 * ( a21 * _34x42_32x44_ + a22 * _31x44_34x41_ - a24 * _31x42_32x41_ )
					- a14 * ( a21 * _32x43_33x42_ + a22 * _33x41_31x43_ + a23 * _31x42_32x41_ );

		// �t�s�񂪑��݂���ꍇ�̂��t�s����v�Z����
		if( detA != 0 )
		{
			// ���ʂŗ��p����W���̌v�Z
			double _23x44_24x43_ = a23 * a44 - a24 * a43;
			double _24x42_22x44_ = a24 * a42 - a22 * a44;
			double _22x43_23x42_ = a22 * a43 - a23 * a42;
			double _24x33_23x34_ = a24 * a33 - a23 * a34;
			double _22x34_24x32_ = a22 * a34 - a24 * a32;
			double _23x32_22x33_ = a23 * a32 - a22 * a33;
			double _21x44_24x41_ = a21 * a44 - a24 * a41;
			double _23x41_21x43_ = a23 * a41 - a21 * a43;
			double _24x31_21x34_ = a24 * a31 - a21 * a34;
			double _21x33_23x31_ = a21 * a33 - a23 * a31;
			double _21x42_22x41_ = a21 * a42 - a22 * a41;
			double _22x31_21x32_ = a22 * a31 - a21 * a32;

			// �e�v�f�̒l���v�Z
			double A11 =  a22 * _33x44_34x43_ + a23 * _34x42_32x44_ + a24 * _32x43_33x42_;
			double A12 = -a12 * _33x44_34x43_ - a13 * _34x42_32x44_ - a14 * _32x43_33x42_;
			double A13 =  a12 * _23x44_24x43_ + a13 * _24x42_22x44_ + a14 * _22x43_23x42_;
			double A14 =  a12 * _24x33_23x34_ + a13 * _22x34_24x32_ + a14 * _23x32_22x33_;
			double A21 = -a21 * _33x44_34x43_ + a23 * _31x44_34x41_ + a24 * _33x41_31x43_;
			double A22 =  a11 * _33x44_34x43_ - a13 * _31x44_34x41_ - a14 * _33x41_31x43_;
			double A23 = -a11 * _23x44_24x43_ + a13 * _21x44_24x41_ + a14 * _23x41_21x43_;
			double A24 = -a11 * _24x33_23x34_ + a13 * _24x31_21x34_ + a14 * _21x33_23x31_;
			double A31 = -a21 * _34x42_32x44_ - a22 * _31x44_34x41_ + a24 * _31x42_32x41_;
			double A32 =  a11 * _34x42_32x44_ + a12 * _31x44_34x41_ - a14 * _31x42_32x41_;
			double A33 = -a11 * _24x42_22x44_ - a12 * _21x44_24x41_ + a14 * _21x42_22x41_;
			double A34 = -a11 * _22x34_24x32_ - a12 * _24x31_21x34_ + a14 * _22x31_21x32_;
			double A41 = -a21 * _32x43_33x42_ - a22 * _33x41_31x43_ - a23 * _31x42_32x41_;
			double A42 =  a11 * _32x43_33x42_ + a12 * _33x41_31x43_ + a13 * _31x42_32x41_;
			double A43 = -a11 * _22x43_23x42_ - a12 * _23x41_21x43_ - a13 * _21x42_22x41_;
			double A44 = -a11 * _23x32_22x33_ - a12 * _21x33_23x31_ - a13 * _22x31_21x32_;

			double _1_detA = 1.0 / detA;
			a( 0, 0 ) = static_cast< value_type >( A11 * _1_detA );
			a( 0, 1 ) = static_cast< value_type >( A12 * _1_detA );
			a( 0, 2 ) = static_cast< value_type >( A13 * _1_detA );
			a( 0, 3 ) = static_cast< value_type >( A14 * _1_detA );
			a( 1, 0 ) = static_cast< value_type >( A21 * _1_detA );
			a( 1, 1 ) = static_cast< value_type >( A22 * _1_detA );
			a( 1, 2 ) = static_cast< value_type >( A23 * _1_detA );
			a( 1, 3 ) = static_cast< value_type >( A24 * _1_detA );
			a( 2, 0 ) = static_cast< value_type >( A31 * _1_detA );
			a( 2, 1 ) = static_cast< value_type >( A32 * _1_detA );
			a( 2, 2 ) = static_cast< value_type >( A33 * _1_detA );
			a( 2, 3 ) = static_cast< value_type >( A34 * _1_detA );
			a( 3, 0 ) = static_cast< value_type >( A41 * _1_detA );
			a( 3, 1 ) = static_cast< value_type >( A42 * _1_detA );
			a( 3, 2 ) = static_cast< value_type >( A43 * _1_detA );
			a( 3, 3 ) = static_cast< value_type >( A44 * _1_detA );

			return( true );
		}
		else
		{
			return( false );
		}
	}

	template < class T1, class T2, class Matrix >
	inline void update_edge( const std::vector< vector3< T1 > > &vertices, const std::vector< Matrix > &Q, __edge__< T2 > &edge )
	{
		typedef Matrix matrix_type;
		typedef vector3< T1 > vector_type;

		// �e���_�̌덷�]���s������߂�
		matrix_type QQ = Q[ edge.v1 ] + Q[ edge.v2 ];
		matrix_type QQQ( QQ );
		QQQ( 3, 0 ) = 0;
		QQQ( 3, 1 ) = 0;
		QQQ( 3, 2 ) = 0;
		QQQ( 3, 3 ) = 1;

		//if( inv4x4( QQQ ) )
		//{
		//	matrix_type V = QQQ * matrix_type::_41( 0, 0, 0, 1.0 );
		//	edge.v   = vector_type( V[ 0 ], V[ 1 ], V[ 2 ] );
		//	edge.err = ( V.t( ) * QQ * V )[ 0 ];
		//}
		//else
		{
			vector_type v1 = vertices[ edge.v1 ];
			vector_type v2 = vertices[ edge.v2 ];
			vector_type v3 = ( v1 + v2 ) * 0.5;
			matrix_type V1 = matrix_type::_41( v1.x, v1.y, v1.z, 1.0 );
			matrix_type V2 = matrix_type::_41( v2.x, v2.y, v2.z, 1.0 );
			matrix_type V3 = matrix_type::_41( v3.x, v3.y, v3.z, 1.0 );

			double err1 = ( V1.t( ) * QQ * V1 )[ 0 ];
			double err2 = ( V2.t( ) * QQ * V2 )[ 0 ];
			double err3 = ( V3.t( ) * QQ * V3 )[ 0 ];

			if( err1 < err2 && err1 < err3 )
			{
				edge.v   = v1;
				edge.err = err1;
			}
			else if( err2 < err1 && err2 < err3 )
			{
				edge.v   = v2;
				edge.err = err2;
			}
			else
			{
				edge.v   = v3;
				edge.err = err3;
			}
		}
	}

	template < class T >
	inline bool is_sharp_edge( const __edge__< T > &edge )
	{
		return( edge.fid1 * edge.fid2 == 0 );
	}

	template < class MULTIMAP, class T >
	inline size_t number_of_sharp_edges( const MULTIMAP &vertex_edge_map, const std::vector< __edge__< T > > &edges, typename MULTIMAP::difference_type vid )
	{
		typedef MULTIMAP vertex_edge_map_type;
		typedef typename vertex_edge_map_type::const_iterator const_iterator;
		typedef typename vertex_edge_map_type::difference_type difference_type;

		difference_type count = 0;

		const_iterator ite = vertex_edge_map.find( vid );
		if( ite != vertex_edge_map.end( ) )
		{
			const_iterator upper = vertex_edge_map.upper_bound( vid );
			for( ; ite != upper ; ++ite )
			{
				if( is_sharp_edge( edges[ ite->second ] ) )
				{
					count++;
				}
			}
		}

		return( count );
	}

	template < class T >
	inline void compute_edge_connections( const std::vector< __edge__< T > > &edges, __face__::difference_type EID, __face__::difference_type eid,
											__face__::difference_type vs, __face__::difference_type vt, __face__::difference_type &vv,
											__face__::difference_type &eid1, __face__::difference_type &eid2 )
	{
		typedef __edge__< T > edge_type;

		if( eid != EID )
		{
			const edge_type &e = edges[ eid ];
			if( e.v1 == vs && eid1 == 0 )
			{
				// vs �֐ڑ������
				eid1 = eid;
				vv = e.v2;
			}
			else if( e.v2 == vs && eid1 == 0 )
			{
				// vs �֐ڑ������
				eid1 = eid;
				vv = e.v1;
			}
			else if( e.v1 == vt )
			{
				// vt �֐ڑ������
				eid2 = eid;
				vv = e.v2;
			}
			else if( e.v2 == vt )
			{
				// vt �֐ڑ������
				eid2 = eid;
				vv = e.v1;
			}
			else
			{
				std::cerr << "Incorrect edge pairs." << std::endl;
			}
		}
	}

	template < class T >
	inline bool compute_edge_connections( const std::vector< __face__ > &faces, const std::vector< __edge__< T > > &edges, __face__::difference_type EID,
											__face__::difference_type vs, __face__::difference_type vt, __face__::difference_type &vl, __face__::difference_type &vr, __face__::difference_type eid[ 4 ] )
	{
		typedef __edge__< T >             edge_type;
		typedef __face__                  face_type;
		typedef __face__::difference_type difference_type;

		const edge_type &EDGE = edges[ EID ];

		if( is_sharp_edge( EDGE ) )
		{
			// ���ɐڂ��Ă���ӂ͍폜�ΏۊO
			return( false );
		}

		const face_type &f1 = faces[ EDGE.fid1 ];
		const face_type &f2 = faces[ EDGE.fid2 ];

		eid[ 0 ] = eid[ 1 ] = eid[ 2 ] = eid[ 3 ] = 0;

		const edge_type &e11 = edges[ f1.eid1 < 0 ? -f1.eid1 : f1.eid1 ];
		const edge_type &e12 = edges[ f1.eid2 < 0 ? -f1.eid2 : f1.eid2 ];
		const edge_type &e13 = edges[ f1.eid3 < 0 ? -f1.eid3 : f1.eid3 ];
		const edge_type &e21 = edges[ f2.eid1 < 0 ? -f2.eid1 : f2.eid1 ];
		const edge_type &e22 = edges[ f2.eid2 < 0 ? -f2.eid2 : f2.eid2 ];
		const edge_type &e23 = edges[ f2.eid3 < 0 ? -f2.eid3 : f2.eid3 ];

		// ��1���`�F�b�N
		compute_edge_connections( edges, EID, f1.eid1 < 0 ? -f1.eid1 : f1.eid1, vs, vt, vl, eid[ 0 ], eid[ 1 ] );
		compute_edge_connections( edges, EID, f1.eid2 < 0 ? -f1.eid2 : f1.eid2, vs, vt, vl, eid[ 0 ], eid[ 1 ] );
		compute_edge_connections( edges, EID, f1.eid3 < 0 ? -f1.eid3 : f1.eid3, vs, vt, vl, eid[ 0 ], eid[ 1 ] );

		// ��2���`�F�b�N
		compute_edge_connections( edges, EID, f2.eid1 < 0 ? -f2.eid1 : f2.eid1, vs, vt, vr, eid[ 3 ], eid[ 2 ] );
		compute_edge_connections( edges, EID, f2.eid2 < 0 ? -f2.eid2 : f2.eid2, vs, vt, vr, eid[ 3 ], eid[ 2 ] );
		compute_edge_connections( edges, EID, f2.eid3 < 0 ? -f2.eid3 : f2.eid3, vs, vt, vr, eid[ 3 ], eid[ 2 ] );

		return( eid[ 0 ] * eid[ 1 ] * eid[ 2 ] * eid[ 3 ] != 0 );
	}

	template < class MULTIMAP >
	inline void remove_edge_from_map( MULTIMAP &vertex_edge_map, typename MULTIMAP::key_type key, typename MULTIMAP::mapped_type val )
	{
		typename MULTIMAP::iterator ite = vertex_edge_map.find( key );
		if( ite != vertex_edge_map.end( ) )
		{
			typename MULTIMAP::iterator upper = vertex_edge_map.upper_bound( key );
			for( ; ite != upper ; )
			{
				if( ite->second == val )
				{
					// �폜����
					ite = vertex_edge_map.erase( ite );
				}
				else
				{
					++ite;
				}
			}
		}
	}

	template < class SET >
	inline void remove_edge_from_set( SET &edge_set, typename SET::value_type EID )
	{
		typename SET::iterator ite = edge_set.find( EID );
		if( ite != edge_set.end( ) )
		{
			edge_set.erase( ite );
		}
	}

	template < class MULTIMAP, class T >
	inline bool check_topology_change( const MULTIMAP &vertex_edge_map, const std::vector< __face__ > &faces, const std::vector< __edge__< T > > &edges, __face__::difference_type EID )
	{
		typedef MULTIMAP vertex_edge_map_type;
		typedef typename vertex_edge_map_type::const_iterator const_iterator;

		typedef __edge__< T >             edge_type;
		typedef __face__                  face_type;
		typedef __face__::difference_type difference_type;

		const edge_type &EDGE = edges[ EID ];

		if( is_sharp_edge( EDGE ) )
		{
			// ���ɐڂ��Ă���ӂ͍폜�ΏۊO
			return( true );
		}

		// �����Ώۂ̕ӂ̓o�^�ƁC�e�[�u��������̍폜�����s��
		difference_type vs = EDGE.v1;
		difference_type vt = EDGE.v2;
		difference_type vl, vr;
		difference_type eid[ 4 ];
		if( !__mc__::compute_edge_connections( faces, edges, EID, vs, vt, vl, vr, eid ) )
		{
			return( true );
		}

		// �w�肵���ӂ��폜�����ꍇ�Ƀg�|���W�[���ω����邩�ǂ������`�F�b�N����
		if( is_sharp_edge( edges[ eid[ 0 ] ] ) && is_sharp_edge( edges[ eid[ 1 ] ] ) )
		{
			return( true );
		}
		else if( is_sharp_edge( edges[ eid[ 3 ] ] ) && is_sharp_edge( edges[ eid[ 2 ] ] ) )
		{
			return( true );
		}

		difference_type nvs = number_of_sharp_edges( vertex_edge_map, edges, vs );
		difference_type nvt = number_of_sharp_edges( vertex_edge_map, edges, vt );

		if( nvs >= 1 && nvt >= 1 )
		{
			return( true );
		}

		//std::set< difference_type > vertex_list;

		//{
		//	const_iterator ite = vertex_edge_map.find( EDGE.v1 );
		//	if( ite != vertex_edge_map.end( ) )
		//	{
		//		const_iterator upper = vertex_edge_map.upper_bound( EDGE.v1 );
		//		for( ; ite != upper ; ++ite )
		//		{
		//			if( ite->second != EID )
		//			{
		//				const edge_type &e = edges[ ite->second ];
		//				if( ( e.v1 == EDGE.v1 && e.v2 == EDGE.v2 ) || ( e.v1 == EDGE.v2 && e.v2 == EDGE.v1 ) )
		//				{
		//					return( true );
		//				}
		//			}
		//		}
		//	}
		//}


		std::set< difference_type > vertex_list;

		{
			const_iterator ite = vertex_edge_map.find( EDGE.v1 );
			if( ite != vertex_edge_map.end( ) )
			{
				const_iterator upper = vertex_edge_map.upper_bound( EDGE.v1 );
				for( ; ite != upper ; ++ite )
				{
					if( ite->second != EID )
					{
						const edge_type &e = edges[ ite->second ];
						if( e.fid1 != EDGE.fid1 && e.fid2 != EDGE.fid2 && e.fid1 != EDGE.fid2 && e.fid2 != EDGE.fid1 )
						{
							if( e.v1 != EDGE.v1 )
							{
								vertex_list.insert( e.v1 );
							}
							else
							{
								vertex_list.insert( e.v2 );
							}
						}
					}
				}
			}
		}

		{
			const_iterator ite = vertex_edge_map.find( EDGE.v2 );
			if( ite != vertex_edge_map.end( ) )
			{
				const_iterator upper = vertex_edge_map.upper_bound( EDGE.v2 );
				for( ; ite != upper ; ++ite )
				{
					if( ite->second != EID )
					{
						const edge_type &e = edges[ ite->second ];
						if( e.fid1 != EDGE.fid1 && e.fid2 != EDGE.fid2 && e.fid1 != EDGE.fid2 && e.fid2 != EDGE.fid1 )
						{
							if( e.v1 != EDGE.v1 )
							{
								if( vertex_list.find( e.v1 ) != vertex_list.end( ) )
								{
									// �ʂ̑��݂��Ȃ����[�v�𔭌�
									return( true );
								}
							}
							else
							{
								if( vertex_list.find( e.v2 ) != vertex_list.end( ) )
								{
									// �ʂ̑��݂��Ȃ����[�v�𔭌�
									return( true );
								}
							}
						}
					}
				}
			}
		}

		return( false );
	}
}

/// @brief 3�p�`�p�b�`�̏W�����璸�_�W���ƎO�p�`�p�b�`���\������C���f�b�N�X�W���ɕϊ�����
//!
//! ���ʂ��Ďg�p���钸�_��S�Ă܂Ƃ߂����_�W�����쐬����D
//!
//! @param[in]  facets           �c 3�p�`�p�b�`�̏W��
//! @param[in]  number_of_facets �c �팸���3�p�`�p�b�`��
//!
//! @return ���_�W���ƃC���f�b�N�X�W���̍쐬�ɐ����������ǂ���
//!
template < class T >
inline bool surface_simplification( facet_list< T > &facets, size_t number_of_facets )
{
	typedef typename facet_list< T >::facet_type   facet_type;
	typedef typename facet_type::size_type         size_type;
	typedef typename facet_type::difference_type   difference_type;
	typedef typename facet_type::vector_type       vector_type;
	typedef typename vector3< difference_type >    ivector_type;
	typedef __mc__::__edge__< vector_type >        edge_type;
	typedef __mc__::__face__                       face_type;
	typedef matrix< double >                       matrix_type;
	typedef std::vector< edge_type >               edge_list_type;


	std::vector< vector_type >  vertices;
	std::vector< ivector_type > face_ids;
	std::vector< face_type >    faces;
	edge_list_type              edges;

	// ���_�Ɩʂ̃��X�g�ɕϊ�����
	if( !convert_to_vertex_face_list( facets, vertices, face_ids ) )
	{
		return( false );
	}

	// �ʏ��ɃG�b�W����t�^����
	if( !convert_to_vertex_face_list( vertices, face_ids, faces, edges ) )
	{
		return( false );
	}
	else
	{
		face_ids.clear( );
	}

	// Contraction Error ���v�Z���邽�߂̍s��W����ݒ肷��
	std::vector< matrix_type >  Q( vertices.size( ) );
	for( size_type i = 0 ; i < Q.size( ) ; i++ )
	{
		Q[ i ].resize( 4, 4 );
	}

	// �ڑ����钸�_�̌��W�������߂�
	for( size_type i = 1 ; i < faces.size( ) ; i++ )
	{
		const face_type   &f  = faces[ i ];
		const difference_type v1 = f.eid1 < 0 ? edges[ -f.eid1 ].v2 : edges[ f.eid1 ].v1;
		const difference_type v2 = f.eid2 < 0 ? edges[ -f.eid2 ].v2 : edges[ f.eid2 ].v1;
		const difference_type v3 = f.eid3 < 0 ? edges[ -f.eid3 ].v2 : edges[ f.eid3 ].v1;
		const vector_type  &p1 = vertices[ v1 ];
		const vector_type  &p2 = vertices[ v2 ];
		const vector_type  &p3 = vertices[ v3 ];
		const vector_type  &n  = facets[ i - 1 ].normal;

		double a = n.x;
		double b = n.y;
		double c = n.z;

		{
			double d = -( a * p1.x + b * p1.y + c * p1.z );
			Q[ v1 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d );
		}

		{
			double d = -( a * p2.x + b * p2.y + c * p2.z );
			Q[ v2 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d );
		}

		{
			double d = -( a * p3.x + b * p3.y + c * p3.z );
			Q[ v3 ] += matrix_type::_44( a * a, a * b, a * c, a * d,
										 a * b, b * b, b * c, b * d,
										 a * c, b * c, c * c, c * d,
										 a * d, b * d, c * d, d * d );
		}
	}

	// ���_�ƃG�b�W�̃e�[�u�����쐬����
	typedef __mc__::__edge_comp__< edge_type > edge_compare_type;
	typedef std::set< difference_type, edge_compare_type > edge_map_type;
	typedef std::multimap< size_type, difference_type > vertex_edge_map_type;
	typedef std::pair< size_type, difference_type > vertex_edge_map_pair_type;

	vertex_edge_map_type vertex_edge_map;
	for( size_type i = 1 ; i < edges.size( ) ; i++ )
	{
		edge_type &e = edges[ i ];
		e.key = __mc__::create_key( e.v1, e.v2, vertices.size( ) );

		vertex_edge_map.insert( vertex_edge_map_pair_type( e.v1, i ) );
		vertex_edge_map.insert( vertex_edge_map_pair_type( e.v2, i ) );
	}

	edge_compare_type ecomp( edges );
	edge_map_type edge_map( ecomp );
	for( size_type i = 1 ; i < edges.size( ) ; i++ )
	{
		// �폜�Ώۂɂł��邩�ǂ����𔻒�
		if( !__mc__::check_topology_change( vertex_edge_map, faces, edges, i ) )
		{
			// �e���_�̌덷�]�����s��
			__mc__::update_edge( vertices, Q, edges[ i ] );
			edge_map.insert( i );
		}
	}

	size_t num_facets = faces.size( ) - 1;

	// ���_���R�X�g���ɍ팸����
	for( ; num_facets - 2 >= number_of_facets && !edge_map.empty( ) ; num_facets -= 2 )
	{
		typename edge_map_type::iterator mite = edge_map.end( );

		for( typename edge_map_type::iterator ite = edge_map.begin( ) ; ite != edge_map.end( ) ; )
		{
			const edge_type &e = edges[ *ite ];

			if( __mc__::check_topology_change( vertex_edge_map, faces, edges, *ite ) )
			{
				// �폜�Ώۂɂł��Ȃ�
				ite = edge_map.erase( ite );
			}
			else
			{
				mite = ite;
				break;
			}
		}

		if( mite == edge_map.end( ) )
		{
			// �폜�ł���ӂ�������Ȃ������̂ŏI������
			break;
		}

		difference_type EID = *mite < 0 ? -( *mite ) : *mite;
		edge_type &EDGE = edges[ EID ];

#if defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ >= 1
		std::cout << "Contraction [" << num_facets << "] : " << EDGE.v2 << " -> " << EDGE.v1 << std::endl;
#endif

		// �ӂ��폜����
		edge_map.erase( mite );

		// �����Ώۂ̕ӂ̓o�^�ƁC�e�[�u��������̍폜�����s��
		difference_type vs = EDGE.v1;
		difference_type vt = EDGE.v2;
		difference_type vl, vr;
		difference_type eid[ 4 ];
		if( !__mc__::compute_edge_connections( faces, edges, EID, vs, vt, vl, vr, eid ) )
		{
			std::cerr << "Unknown error occured!!" << std::endl;
			break;
		}
		else if( vl == vr )
		{
			continue;
		}

		// �ʂ��폜����
		face_type &f1 = faces[ EDGE.fid1 ];
		face_type &f2 = faces[ EDGE.fid2 ];
		f1.flag = false;
		f2.flag = false;

		// ���_���ړ�������
		vertices[ EDGE.v1 ] = EDGE.v;
		vertices[ EDGE.v2 ] = EDGE.v;

		// �덷�v�Z�s����X�V����
		matrix_type QQQ = Q[ EDGE.v1 ] + Q[ EDGE.v2 ];
		Q[ EDGE.v1 ] = QQQ;
		Q[ EDGE.v2 ] = QQQ;

		std::set< difference_type > emap;
		{
			vertex_edge_map_type::iterator ite = vertex_edge_map.find( vs );
			if( ite != vertex_edge_map.end( ) )
			{
				vertex_edge_map_type::iterator upper = vertex_edge_map.upper_bound( vs );
				for( ; ite != upper ; )
				{
					if( ite->second == EID )
					{
						// �폜����
						ite = vertex_edge_map.erase( ite );
					}
					else
					{
						emap.insert( ite->second );
						++ite;
					}
				}
			}
		}
		{
			vertex_edge_map_type::iterator ite = vertex_edge_map.find( vt );
			if( ite != vertex_edge_map.end( ) )
			{
				std::vector< difference_type > combine_edge;
				vertex_edge_map_type::iterator upper = vertex_edge_map.upper_bound( vt );
				for( ; ite != upper ; )
				{
					if( ite->second == EID )
					{
						// �폜����
						ite = vertex_edge_map.erase( ite );
					}
					else
					{
						if( ite->second == eid[ 1 ] || ite->second == eid[ 2 ] )
						{
						}
						else
						{
							emap.insert( ite->second );
							combine_edge.push_back( ite->second );

							if( edges[ ite->second ].v1 == EDGE.v2 )
							{
								edges[ ite->second ].v1 = EDGE.v1;
							}
							else if( edges[ ite->second ].v2 == EDGE.v2 )
							{
								edges[ ite->second ].v2 = EDGE.v1;
							}
							else
							{
								std::cerr << "Incorrect edge is found!!" << std::endl;
							}
						}

						ite = vertex_edge_map.erase( ite );
					}
				}

				// �s�v�ȕӂ����ׂč폜����
				__mc__::remove_edge_from_map( vertex_edge_map, vl, eid[ 1 ] );
				__mc__::remove_edge_from_map( vertex_edge_map, vr, eid[ 2 ] );

				// �����Ώۂ̎}��������O����
				__mc__::remove_edge_from_set( edge_map, eid[ 1 ] );
				__mc__::remove_edge_from_set( edge_map, eid[ 2 ] );

				// �ύX���ꂽ�ӂ��ēo�^����
				for( size_type ii = 0 ; ii < combine_edge.size( ) ; ii++ )
				{
					vertex_edge_map.insert( vertex_edge_map_pair_type( vs, combine_edge[ ii ] ) );
				}
			}
		}

		// �ӂ̕t���ւ����s��
		__mc__::contract_edges( edges, faces, eid[ 0 ], eid[ 1 ], EID, EDGE.fid1 );
		__mc__::contract_edges( edges, faces, eid[ 3 ], eid[ 2 ], EID, EDGE.fid2 );

		// �e���_�����L����G�b�W���X�V����
		// �������C�������ė̈�̒[�ɐڂ����ꍇ�͈ȍ~�̑Ώۂ��珜��
		for( std::set< difference_type >::iterator ite = emap.begin( ) ; ite != emap.end( ) ; ++ite )
		{
			__mc__::remove_edge_from_set( edge_map, *ite );
			if( __mc__::check_topology_change( vertex_edge_map, faces, edges, *ite ) )
			{
				// �����Ώۂ̎}��������O����
			}
			else
			{
				__mc__::update_edge( vertices, Q, edges[ *ite ] );
				edge_map.insert( *ite );
			}
		}

#if defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ >= 3
		for( vertex_edge_map_type::iterator ite = vertex_edge_map.begin( ) ; ite != vertex_edge_map.end( ) ; ++ite )
		{
			edge_type &e = edges[ ite->second ];
			if( e.fid1 > 0 && !faces[ e.fid1 ].flag )
			{
				std::cerr << "Edge " << ite->second << " connects to disappeared face." << std::endl;
			}
			if( e.fid2 > 0 && !faces[ e.fid2 ].flag )
			{
				std::cerr << "Edge " << ite->second << " connects to disappeared face." << std::endl;
			}
		}

		// �ʂ̐ڑ��֌W���������ۂ���Ă��邩�ǂ������`�F�b�N����
		for( size_type i = 1 ; i < faces.size( ) ; i++ )
		{
			const face_type &f = faces[ i ];
			if( f.flag )
			{
				const difference_type eid1 = f.eid1 < 0 ? -f.eid1 : f.eid1;
				const difference_type eid2 = f.eid2 < 0 ? -f.eid2 : f.eid2;
				const difference_type eid3 = f.eid3 < 0 ? -f.eid3 : f.eid3;

				if( eid1 != 0 && edges[ eid1 ].fid1 != i && edges[ eid1 ].fid2 != i )
				{
					std::cerr << "Face-edge relationships is broken." << std::endl;
				}
				if( eid2 != 0 && edges[ eid2 ].fid1 != i && edges[ eid2 ].fid2 != i )
				{
					std::cerr << "Face-edge relationships is broken." << std::endl;
				}
				if( eid3 != 0 && edges[ eid3 ].fid1 != i && edges[ eid3 ].fid2 != i )
				{
					std::cerr << "Face-edge relationships is broken." << std::endl;
				}

				if( eid1 * eid2 * eid3 != 0 )
				{
					const edge_type &e1 = edges[ eid1 ];
					const edge_type &e2 = edges[ eid2 ];
					const edge_type &e3 = edges[ eid3 ];
					difference_type v11 = f.eid1 < 0 ? e1.v2 : e1.v1;
					difference_type v12 = f.eid1 < 0 ? e1.v1 : e1.v2;
					difference_type v21 = f.eid2 < 0 ? e2.v2 : e2.v1;
					difference_type v22 = f.eid2 < 0 ? e2.v1 : e2.v2;
					difference_type v31 = f.eid3 < 0 ? e3.v2 : e3.v1;
					difference_type v32 = f.eid3 < 0 ? e3.v1 : e3.v2;

					if( v12 == v21 && v22 == v31 && v32 == v11 )
					{
					}
					else
					{
						std::cerr << "Face-edge connection is broken." << std::endl;
						std::cout << "(" << v11 << ", " << v12 << ") -> (" << v21 << ", " << v22 << ") -> (" << v31 << ", " << v32 << ")" << std::endl;
					}
				}
			}
		}
#endif

#if defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ == 2
		for( size_type i = 1 ; i < faces.size( ) ; i++ )
		{
			const face_type &f = faces[ i ];
			if( f.flag )
			{
				std::cout << i << ": ";
				if( f.eid1 < 0 )
				{
					std::cout << edges[ -f.eid1 ].v2 << ", " << edges[ -f.eid1 ].v1 << "[" << -f.eid1 << "] -> ";
				}
				else
				{
					std::cout << edges[ f.eid1 ].v1 << ", " << edges[ f.eid1 ].v2 << "[" << f.eid1 << "] -> ";
				}

				if( f.eid2 < 0 )
				{
					std::cout << edges[ -f.eid2 ].v2 << ", " << edges[ -f.eid2 ].v1 << "[" << -f.eid2 << "] -> ";
				}
				else
				{
					std::cout << edges[ f.eid2 ].v1 << ", " << edges[ f.eid2 ].v2 << "[" << f.eid2 << "] -> ";
				}

				if( f.eid3 < 0 )
				{
					std::cout << edges[ -f.eid3 ].v2 << ", " << edges[ -f.eid3 ].v1 << "[" << -f.eid3 << "]" << std::endl;
				}
				else
				{
					std::cout << edges[ f.eid3 ].v1 << ", " << edges[ f.eid3 ].v2  << "[" << f.eid3 << "]" << std::endl;
				}
			}
		}
		std::cout << std::endl;
#elif defined( __SHOW_FACET_DEBUG_INFORMATION__ ) && __SHOW_FACET_DEBUG_INFORMATION__ >= 3
		for( size_type i = 1 ; i < faces.size( ) ; i++ )
		{
			const face_type &f = faces[ i ];
			if( f.flag )
			{
				printf( "%2d: ", i );

				difference_type eid1 = f.eid1 < 0 ? -f.eid1 : f.eid1;
				difference_type eid2 = f.eid2 < 0 ? -f.eid2 : f.eid2;
				difference_type eid3 = f.eid3 < 0 ? -f.eid3 : f.eid3;
				edge_type &e1 = edges[ eid1 ];
				edge_type &e2 = edges[ eid2 ];
				edge_type &e3 = edges[ eid3 ];

				if( f.eid1 < 0 )
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e1.v2, e1.v1, eid1, e1.fid1, e1.fid2 );
				}
				else
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e1.v1, e1.v2, eid1, e1.fid1, e1.fid2 );
				}

				if( f.eid2 < 0 )
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e2.v2, e2.v1, eid2, e2.fid1, e2.fid2 );
				}
				else
				{
					printf( "%2d, %2d [%2d] (%2d, %2d) -> ", e2.v1, e2.v2, eid2, e2.fid1, e2.fid2 );
				}

				if( f.eid3 < 0 )
				{
					printf( "%2d, %2d [%2d] (%2d, %2d)\n", e3.v2, e3.v1, eid3, e3.fid1, e3.fid2 );
				}
				else
				{
					printf( "%2d, %2d [%2d] (%2d, %2d)\n", e3.v1, e3.v2, eid3, e3.fid1, e3.fid2 );
				}
			}
		}
		std::cout << std::endl;
#endif
	}

	facets.clear( );

	for( size_type i = 1 ; i < faces.size( ) ; i++ )
	{
		const face_type &f = faces[ i ];
		if( f.flag )
		{
			const difference_type v1 = f.eid1 < 0 ? edges[ -f.eid1 ].v2 : edges[ f.eid1 ].v1;
			const difference_type v2 = f.eid2 < 0 ? edges[ -f.eid2 ].v2 : edges[ f.eid2 ].v1;
			const difference_type v3 = f.eid3 < 0 ? edges[ -f.eid3 ].v2 : edges[ f.eid3 ].v1;
			facets.push_back( facet_type( vertices[ v1 ], vertices[ v2 ], vertices[ v3 ] ) );
		}
	}

	return( true );
}

/// @}
//  Marching Cubes�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_FACET__
