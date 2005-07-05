/// @file mist/filter/region_growing.h
//!
//! @brief 各次元の画像に対応し，任意の位置から任意の構造要素と任意の条件判定を用いて領域拡張法を適用するためのライブラリ
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


// mist名前空間の始まり
_MIST_BEGIN



//! @addtogroup region_growing_group 領域拡張法
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/filter/region_growing.h>
//! @endcode
//!
//!  @{


/// @brief 領域拡張法に利用する構造要素関連
//! 
//! 領域拡張法に利用する構造要素の定義
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


	// 条件判定リスト内の全ての画素がある範囲内にある場合に条件を満たしたとするクラス
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


/// @brief 任意の構造要素と条件を用いて領域拡張を行う関数
//!
//! @param[in]  in           … 入力画像
//! @param[out] out          … 出力マークデータ
//! @param[in]  mask         … 処理対象外マスク
//! @param[in]  start_points … 領域拡張の開始点のリスト（複数指定可能）
//! @param[in]  output_value … 出力マークデータに書き込む値
//! @param[in]  components   … 領域拡張に用いる構造要素
//! @param[in]  condition    … 構造要素内の画素が満たすべき条件
//! @param[in]  max_num      … 最大反復回数
//!
//! @return 入力画像が不適切な場合や，最大反復回数を試行しても終了条件を満たさなかった場合に false を返す
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

	// 注目点が範囲外に出ないことを監視するマスク
	work.fill_margin( 255 );
	work.fill( );

	// マスクの内容を注目点のデータに反映させる
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



/// @brief 任意の構造要素と条件を用いて領域拡張を行う関数
//!
//! 処理対象外マスクを設定しない領域拡張法
//!
//! @param[in]  in           … 入力画像
//! @param[out] out          … 出力マークデータ
//! @param[in]  start_points … 領域拡張の開始点のリスト（複数指定可能）
//! @param[in]  output_value … 出力マークデータに書き込む値
//! @param[in]  components   … 領域拡張に用いる構造要素
//! @param[in]  condition    … 構造要素内の画素が満たすべき条件
//! @param[in]  max_num      … 最大反復回数
//!
//! @return 入力画像が不適切な場合や，最大反復回数を試行しても終了条件を満たさなかった場合に false を返す
//!
template < class Array1, class Array2, class PointList, class Component, class Condition >
bool region_growing( const Array1 &in, Array2 &out, const PointList &start_points, typename Array2::value_type output_value,
										const Component &components, const Condition &condition, typename Array1::size_type max_num = type_limits< typename Array1::size_type >::maximum( ) )
{
	return( region_growing( in, out, region_growing_utility::no_mask( ), start_points, output_value, components, condition, max_num ) );
}



/// @brief 任意の構造要素と条件を用いて領域拡張を行う関数
//!
//! 処理対象外マスクを設定しない領域拡張法
//!
//! @param[in]  in           … 入力画像
//! @param[out] out          … 出力マークデータ
//! @param[in]  start_point  … 領域拡張の開始点（１点のみ指定可能）
//! @param[in]  output_value … 出力マークデータに書き込む値
//! @param[in]  components   … 領域拡張に用いる構造要素
//! @param[in]  condition    … 構造要素内の画素が満たすべき条件
//! @param[in]  max_num      … 最大反復回数
//!
//! @return 入力画像が不適切な場合や，最大反復回数を試行しても終了条件を満たさなかった場合に false を返す
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
//  領域拡張法グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_REGIONGROWING__
