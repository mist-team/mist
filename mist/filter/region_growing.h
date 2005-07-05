/// @file mist/filter/region_growing.h
//!
//! @brief �e�����̉摜�ɑΉ����C�C�ӂ̈ʒu����C�ӂ̍\���v�f�ƔC�ӂ̏��������p���ė̈�g���@��K�p���邽�߂̃��C�u����
//!

#ifndef __INCLUDE_MIST_REGIONGROWING__
#define __INCLUDE_MIST_REGIONGROWING__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include "../vector.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif

#include <deque>
#include <vector>
#include <cmath>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



//! @addtogroup region_growing_group �̈�g���@
//!
//! @code ���̃w�b�_���C���N���[�h����
//! #include <mist/filter/region_growing.h>
//! @endcode
//!
//!  @{


/// @brief �̈�g���@�ɗ��p����\���v�f�֘A
//! 
//! �̈�g���@�ɗ��p����\���v�f�̒�`
//! 
namespace region_growing_utility
{
	typedef vector3< ptrdiff_t > point_type;
	typedef std::vector< point_type > point_list_type;

	struct no_mask
	{
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		bool empty( ) const { return( true ); }
		bool operator ()( difference_type i, difference_type j, difference_type k ) const { return( false ); }
	};

	struct pixel
	{
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		size_type width( ) const { return( 1 ); }
		size_type height( ) const { return( 1 ); }
		size_type depth( ) const { return( 1 ); }

		bool operator ()( difference_type i, difference_type j, difference_type k ) const { return( true ); }

		size_type expand_mode( ) const { return( 0 ); }
	};

	struct voxel
	{
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		size_type width( ) const { return( 1 ); }
		size_type height( ) const { return( 1 ); }
		size_type depth( ) const { return( 1 ); }

		bool operator ()( difference_type i, difference_type j, difference_type k ) const { return( true ); }

		size_type expand_mode( ) const { return( 1 ); }
	};

	class circle
	{
	public:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

	private:
		double radius_;
		double resoX_;
		double resoY_;
		size_type width_;
		size_type height_;

	public:
		size_type width( ) const { return( width_ ); }
		size_type height( ) const { return( height_ ); }
		size_type depth( ) const { return( 1 ); }

		bool operator ()( difference_type i, difference_type j, difference_type k ) const
		{
			if( k != 0 )
			{
				return( false );
			}
			else
			{
				return( i * i * resoX_ * resoX_ + j * j * resoY_ * resoY_ <= radius_ * radius_ );
			}
		}

		size_type expand_mode( ) const { return( 0 ); }

		circle( double radius, double resoX = 1.0, double resoY = 1.0 ) : radius_( radius ), resoX_( resoX ), resoY_( resoY ),
			width_( static_cast< size_type >( std::ceil( radius_ / resoX_ ) ) * 2 + 1 ), height_( static_cast< size_type >( std::ceil( radius_ / resoY_ ) ) * 2 + 1 )
		{
		}
	};


	// �������胊�X�g���̑S�Ẳ�f������͈͓��ɂ���ꍇ�ɏ����𖞂������Ƃ���N���X
	template < class T >
	class Range
	{
	private:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T			value_type;

	protected:
		value_type min_;
		value_type max_;

	public:
		template < class VerifyList >
		bool operator()( const VerifyList &elements, size_type num ) const
		{
			typedef typename VerifyList::value_type verify_value_type;

			for( size_type i = 0 ; i < num ; i++ )
			{
				const verify_value_type &v = elements[ i ];
				if( v < min_ || max_ < v )
				{
					return ( false );
				}
			}

			return ( true );
		}

		Range( value_type min, value_type max ) : min_( min ), max_( max ){ }
	};


template < class Component >
	point_list_type create_component_list( const Component &components )
	{
		typedef typename point_list_type::size_type			size_type;
		typedef typename point_list_type::difference_type	difference_type;
		typedef typename point_list_type::value_type		value_type;

		point_list_type list;

		difference_type w = components.width( );
		difference_type h = components.height( );
		difference_type d = components.depth( );
		difference_type cx = w / 2;
		difference_type cy = h / 2;
		difference_type cz = d / 2;

		for( difference_type k = 0 ; k < d ; k++ )
		{
			for( difference_type j = 0 ; j < h ; j++ )
			{
				for( difference_type i = 0 ; i < w ; i++ )
				{
					if( components( i - cx, j - cy, k - cz ) )
					{
						list.push_back( point_type( i - cx, j - cy, k - cz ) );
					}
				}
			}
		}

		return( list );
	}

	template < class T >
	inline const T &maximum( const T &v0, const T &v1, const T &v2 )
	{
		return( v0 > v1 ? ( v0 > v2 ? v0 : v2 ) : ( v1 > v2 ? v1 : v2 ) );
	}
}


/// @brief �C�ӂ̍\���v�f�Ə�����p���ė̈�g�����s���֐�
//!
//! @param[in]  in           �c ���͉摜
//! @param[out] out          �c �o�̓}�[�N�f�[�^
//! @param[in]  mask         �c �����ΏۊO�}�X�N
//! @param[in]  start_points �c �̈�g���̊J�n�_�̃��X�g�i�����w��\�j
//! @param[in]  output_value �c �o�̓}�[�N�f�[�^�ɏ������ޒl
//! @param[in]  components   �c �̈�g���ɗp����\���v�f
//! @param[in]  condition    �c �\���v�f���̉�f���������ׂ�����
//! @param[in]  max_num      �c �ő唽����
//!
//! @return ���͉摜���s�K�؂ȏꍇ��C�ő唽���񐔂����s���Ă��I�������𖞂����Ȃ������ꍇ�� false ��Ԃ�
//!
template < class Array1, class Array2, class MaskType, class PointList, class Component, class Condition >
bool region_growing( const Array1 &in, Array2 &out, const MaskType &mask, const PointList &start_points, typename Array2::value_type output_value,
														const Component &components, const Condition &condition, typename Array1::size_type max_num )
{
	if( in.empty( ) || is_same_object( in, out ) )
	{
		return( false );
	}

	typedef typename Array1::template rebind< unsigned char >::other mask_type;
	typedef region_growing_utility::point_type point_type;

	typedef typename Array1::size_type       size_type;
	typedef typename Array1::value_type      value_type;
	typedef typename Array1::difference_type difference_type;
	typedef typename Array2::value_type      out_value_type;

	std::deque< point_type > que;
	std::vector< point_type > clist = region_growing_utility::create_component_list( components );
	std::vector< value_type > element( clist.size( ) );

	difference_type w = in.width( );
	difference_type h = in.height( );
	difference_type d = in.depth( );

	size_type rx = components.width( ) / 2;
	size_type ry = components.height( ) / 2;
	size_type rz = components.depth( ) / 2;

	marray< mask_type > work( in, region_growing_utility::maximum( rx, ry, rz ) );
	size_type count = 0;

	// ���ړ_���͈͊O�ɏo�Ȃ����Ƃ��Ď�����}�X�N
	work.fill_margin( 255 );
	work.fill( );

	// �}�X�N�̓��e�𒍖ړ_�̃f�[�^�ɔ��f������
	if( !mask.empty( ) )
	{
		for( size_type k = 0 ; k < in.depth( ) ; k++ )
		{
			for( size_type j = 0 ; j < in.height( ) ; j++ )
			{
				for( size_type i = 0 ; i < in.width( ) ; i++ )
				{
					if( mask( i, j, k ) )
					{
						work( i, j, k ) = 255;
					}
				}
			}
		}
	}

	for( typename PointList::const_iterator ite = start_points.begin( ) ; ite != start_points.end( ) ; ++ite )
	{
		const typename PointList::value_type &cur = *ite;

		if( cur.x < 0 || cur.x >= w ) continue;
		if( cur.y < 0 || cur.y >= h ) continue;
		if( cur.z < 0 || cur.z >= d ) continue;

		if( work( cur.x, cur.y, cur.z ) == 0 )
		{
			work( cur.x, cur.y, cur.z ) = 1;
			que.push_back( point_type( cur.x, cur.y, cur.z ) );
		}
	}

	if( in.size( ) != out.size( ) )
	{
		out.resize( in.width( ), in.height( ), in.depth( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
	}

	while( !que.empty( ) )
	{
		point_type cur = que.front( );
		que.pop_front( );

		size_type num = 0;
		for( size_type i = 0 ; i < clist.size( ) ; i++ )
		{
			point_type pt = clist[ i ] + cur;
			if( work( pt.x, pt.y, pt.z ) != 255 )
			{
				element[ num++ ] = in( pt.x, pt.y, pt.z );
			}
		}

		if( condition( element, num ) )
		{
			if( ++count > max_num )
			{
				return( false );
			}

			for( size_type i = 0 ; i < clist.size( ) ; i++ )
			{
				point_type pt = cur + clist[ i ];
				if( work( pt.x, pt.y, pt.z ) != 255 )
				{
					out( pt.x, pt.y, pt.z ) = output_value;
				}
			}

			switch( components.expand_mode( ) )
			{
			case 0:
				{
					if( work( cur.x - 1, cur.y , cur.z ) == 0 )
					{
						work( cur.x - 1, cur.y, cur.z ) = 1;
						que.push_back( point_type( cur.x - 1, cur.y    , cur.z ) );
					}
					if( work( cur.x + 1, cur.y, cur.z ) == 0 )
					{
						work( cur.x + 1, cur.y, cur.z ) = 1;
						que.push_back( point_type( cur.x + 1, cur.y    , cur.z ) );
					}
					if( work( cur.x, cur.y - 1, cur.z ) == 0 )
					{
						work( cur.x, cur.y - 1, cur.z ) = 1;
						que.push_back( point_type( cur.x    , cur.y - 1, cur.z ) );
					}
					if( work( cur.x, cur.y + 1, cur.z ) == 0 )
					{
						work( cur.x, cur.y + 1, cur.z ) = 1;
						que.push_back( point_type( cur.x    , cur.y + 1, cur.z ) );
					}
				}
				break;

			case 1:
				{
					if( work( cur.x - 1, cur.y , cur.z ) == 0 )
					{
						work( cur.x - 1, cur.y, cur.z ) = 1;
						que.push_back( point_type( cur.x - 1, cur.y    , cur.z     ) );
					}
					if( work( cur.x + 1, cur.y, cur.z ) == 0 )
					{
						work( cur.x + 1, cur.y, cur.z ) = 1;
						que.push_back( point_type( cur.x + 1, cur.y    , cur.z     ) );
					}
					if( work( cur.x, cur.y - 1, cur.z ) == 0 )
					{
						work( cur.x, cur.y - 1, cur.z ) = 1;
						que.push_back( point_type( cur.x    , cur.y - 1, cur.z     ) );
					}
					if( work( cur.x, cur.y + 1, cur.z ) == 0 )
					{
						work( cur.x, cur.y + 1, cur.z ) = 1;
						que.push_back( point_type( cur.x    , cur.y + 1, cur.z     ) );
					}
					if( work( cur.x, cur.y, cur.z - 1 ) == 0 )
					{
						work( cur.x, cur.y, cur.z - 1 ) = 1;
						que.push_back( point_type( cur.x    , cur.y    , cur.z - 1 ) );
					}
					if( work( cur.x, cur.y, cur.z + 1 ) == 0 )
					{
						work( cur.x, cur.y, cur.z + 1 ) = 1;
						que.push_back( point_type( cur.x    , cur.y    , cur.z + 1 ) );
					}
				}
				break;

			case 3:
			default:
				{
					for( size_type i = 0 ; i < clist.size( ) ; i++ )
					{
						point_type pt = clist[ i ] + cur;
						if( work( pt.x, pt.y, pt.z ) == 0 )
						{
							work( pt.x, pt.y, pt.z ) = 1;
							que.push_back( pt );
						}
					}
				}
				break;
			}
		}
	}

	return( true );
}



/// @brief �C�ӂ̍\���v�f�Ə�����p���ė̈�g�����s���֐�
//!
//! �����ΏۊO�}�X�N��ݒ肵�Ȃ��̈�g���@
//!
//! @param[in]  in           �c ���͉摜
//! @param[out] out          �c �o�̓}�[�N�f�[�^
//! @param[in]  start_points �c �̈�g���̊J�n�_�̃��X�g�i�����w��\�j
//! @param[in]  output_value �c �o�̓}�[�N�f�[�^�ɏ������ޒl
//! @param[in]  components   �c �̈�g���ɗp����\���v�f
//! @param[in]  condition    �c �\���v�f���̉�f���������ׂ�����
//! @param[in]  max_num      �c �ő唽����
//!
//! @return ���͉摜���s�K�؂ȏꍇ��C�ő唽���񐔂����s���Ă��I�������𖞂����Ȃ������ꍇ�� false ��Ԃ�
//!
template < class Array1, class Array2, class PointList, class Component, class Condition >
bool region_growing( const Array1 &in, Array2 &out, const PointList &start_points, typename Array2::value_type output_value,
										const Component &components, const Condition &condition, typename Array1::size_type max_num = type_limits< typename Array1::size_type >::maximum( ) )
{
	return( region_growing( in, out, region_growing_utility::no_mask( ), start_points, output_value, components, condition, max_num ) );
}



/// @brief �C�ӂ̍\���v�f�Ə�����p���ė̈�g�����s���֐�
//!
//! �����ΏۊO�}�X�N��ݒ肵�Ȃ��̈�g���@
//!
//! @param[in]  in           �c ���͉摜
//! @param[out] out          �c �o�̓}�[�N�f�[�^
//! @param[in]  start_point  �c �̈�g���̊J�n�_�i�P�_�̂ݎw��\�j
//! @param[in]  output_value �c �o�̓}�[�N�f�[�^�ɏ������ޒl
//! @param[in]  components   �c �̈�g���ɗp����\���v�f
//! @param[in]  condition    �c �\���v�f���̉�f���������ׂ�����
//! @param[in]  max_num      �c �ő唽����
//!
//! @return ���͉摜���s�K�؂ȏꍇ��C�ő唽���񐔂����s���Ă��I�������𖞂����Ȃ������ꍇ�� false ��Ԃ�
//!
template < class Array1, class Array2, class Component, class Condition >
bool region_growing( const Array1 &in, Array2 &out, const region_growing_utility::point_type &start_point, typename Array2::value_type output_value,
										const Component &components, const Condition &condition, typename Array1::size_type max_num = type_limits< typename Array1::size_type >::maximum( ) )
{
	region_growing_utility::point_list_type list( 1 );
	list[ 0 ] = start_point;
	return( region_growing( in, out, region_growing_utility::no_mask( ), list, output_value, components, condition, max_num ) );
}

/// @}
//  �̈�g���@�O���[�v�̏I���


// mist���O��Ԃ̏I���
_MIST_END


#endif // __INCLUDE_MIST_REGIONGROWING__
