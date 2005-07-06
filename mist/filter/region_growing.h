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
	struct pointer_diff2
	{
		typedef ptrdiff_t	difference_type;

		ptrdiff_t diff1;
		ptrdiff_t diff2;

		pointer_diff2( ptrdiff_t d1 = 0, ptrdiff_t d2 = 0 ) : diff1( d1 ), diff2( d2 ){ }
	};

	typedef vector3< ptrdiff_t >			point_type;			///< @brief 領域拡張法で利用する座標を表す型
	typedef std::vector< point_type >		point_list_type;	///< @brief 領域拡張法で利用する座標のリスト型
	typedef std::vector< pointer_diff2 >	ptrdiff_list_type;


	/// @brief 次の注目点の定め方
	enum expand_mode
	{
		NC4,	///< @brief ４近傍（２次元）
		NC8,	///< @brief ８近傍（２次元）
		NC6,	///< @brief ６近傍（３次元）
		NC26,	///< @brief ２６近傍（３次元）
		ALL		///< @brief 構造要素内のすべての点
	};

	struct no_mask
	{
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		bool empty( ) const { return( true ); }
		bool operator ()( difference_type i, difference_type j, difference_type k ) const { return( false ); }
	};


	/// @brief 領域拡張法で用いる２次元画素の構造要素
	struct pixel
	{
		typedef size_t		size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t	difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		size_type width( ) const { return( 1 ); }	///< @brief 構造要素の幅を返す
		size_type height( ) const { return( 1 ); }	///< @brief 構造要素の高さを返す
		size_type depth( ) const { return( 1 ); }	///< @brief 構造要素の深さを返す

		/// @brief 任意の位置が構造要素内かどうかを判定する関数
		bool operator ()( difference_type i, difference_type j, difference_type k ) const { return( true ); }

		/// @brief 次の注目点の定め方
		expand_mode expand_mode( ) const { return( NC4 ); }
	};


	/// @brief 領域拡張法で用いる３次元画素の構造要素
	struct voxel
	{
		typedef size_t		size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t	difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

		size_type width( ) const { return( 1 ); }	///< @brief 構造要素の幅を返す
		size_type height( ) const { return( 1 ); }	///< @brief 構造要素の高さを返す
		size_type depth( ) const { return( 1 ); }	///< @brief 構造要素の深さを返す

		/// @brief 任意の位置が構造要素内かどうかを判定する関数
		bool operator ()( difference_type i, difference_type j, difference_type k ) const { return( true ); }

		/// @brief 次の注目点の定め方
		expand_mode expand_mode( ) const { return( NC6 ); }
	};


	/// @brief 領域拡張法で用いる円の構造要素
	class circle
	{
	public:
		typedef size_t		size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	private:
		double radius_;
		double resoX_;
		double resoY_;
		size_type width_;
		size_type height_;

	public:
		size_type width( ) const { return( width_ ); }		///< @brief 構造要素の幅を返す
		size_type height( ) const { return( height_ ); }	///< @brief 構造要素の高さを返す
		size_type depth( ) const { return( 1 ); }			///< @brief 構造要素の深さを返す

		/// @brief 任意の位置が構造要素内かどうかを判定する関数
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

		/// @brief 次の注目点の定め方
		expand_mode expand_mode( ) const { return( NC4 ); }

		/// @brief 半径と解像度を指定して球を初期化（楕円の作成も可能）
		circle( double radius, double resoX = 1.0, double resoY = 1.0 ) : radius_( radius ), resoX_( resoX ), resoY_( resoY ),
			width_( static_cast< size_type >( std::ceil( radius_ / resoX_ ) ) * 2 + 1 ), height_( static_cast< size_type >( std::ceil( radius_ / resoY_ ) ) * 2 + 1 )
		{
		}
	};


	/// @brief 領域拡張法で用いる球の構造要素
	class sphere
	{
	public:
		typedef size_t		size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	private:
		double radius_;		///< @brief 半径
		double resoX_;		///< @brief X軸方向の解像度
		double resoY_;		///< @brief Y軸方向の解像度
		double resoZ_;		///< @brief Z軸方向の解像度
		size_type width_;
		size_type height_;
		size_type depth_;

	public:
		size_type width( ) const { return( width_ ); }		///< @brief 構造要素の幅を返す
		size_type height( ) const { return( height_ ); }	///< @brief 構造要素の高さを返す
		size_type depth( ) const { return( depth_ ); }		///< @brief 構造要素の深さを返す

		/// @brief 任意の位置が構造要素内かどうかを判定する関数
		bool operator ()( difference_type i, difference_type j, difference_type k ) const
		{
			return( i * i * resoX_ * resoX_ + j * j * resoY_ * resoY_+ k * k * resoZ_ * resoZ_ <= radius_ * radius_ );
		}

		/// @brief 次の注目点の定め方
		expand_mode expand_mode( ) const { return( NC6 ); }

		/// @brief 半径と解像度を指定して球を初期化（楕円体の作成も可能）
		sphere( double radius, double resoX = 1.0, double resoY = 1.0, double resoZ = 1.0 ) :
						radius_( radius ), resoX_( resoX ), resoY_( resoY ), resoZ_( resoZ ),
						width_( static_cast< size_type >( std::ceil( radius_ / resoX_ ) ) * 2 + 1 ),
						height_( static_cast< size_type >( std::ceil( radius_ / resoY_ ) ) * 2 + 1 ),
						depth_( static_cast< size_type >( std::ceil( radius_ / resoZ_ ) ) * 2 + 1 )
		{
		}
	};


	/// @brief 領域拡張法で用いる拡張条件
	//!
	//! 条件判定リスト内の全ての画素がある値未満
	//!
	template < class T >
	class less
	{
	private:
		typedef size_t		size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
		typedef T			value_type;			///< @brief 条件判定に用いるデータ型

	protected:
		value_type th_;		///< @brief しきい値

	public:
		/// @brief 条件判定リスト内の画素が条件を満たすかどうかを調べる
		template < class VerifyList >
		bool operator()( const VerifyList &elements, size_type num ) const
		{
			typedef typename VerifyList::value_type verify_value_type;

			for( size_type i = 0 ; i < num ; i++ )
			{
				const verify_value_type &v = elements[ i ];
				if( !( v < th_ ) )
				{
					return ( false );
				}
			}

			return ( true );
		}

		/// @brief 条件判定を行う際に，構造要素内の全画素が必要な場合は true を返す
		bool require_all_elements( ) const { return( false ); }

		/// @brief 任意のしきい値で初期化
		less( value_type threshold ) : th_( threshold ){ }
	};


	/// @brief 領域拡張法で用いる拡張条件
	//!
	//! 条件判定リスト内の全ての画素がある範囲内に存在する
	//!
	template < class T >
	class range
	{
	private:
		typedef size_t		size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
		typedef ptrdiff_t	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
		typedef T			value_type;			///< @brief 条件判定に用いるデータ型

	protected:
		value_type min_;	///< @brief 範囲の下限
		value_type max_;	///< @brief 範囲の上限

	public:
		/// @brief 条件判定リスト内の画素が条件を満たすかどうかを調べる
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

		/// @brief 条件判定を行う際に，構造要素内の全画素が必要な場合は true を返す
		bool require_all_elements( ) const { return( false ); }

		/// @brief 任意の下限と上限で範囲を初期化
		range( value_type min, value_type max ) : min_( min ), max_( max ){ }
	};


	template < class Array1, class Array2, class Component >
	ptrdiff_list_type create_component_list( const Array1 &in1, const Array2 &in2, const Component &components )
	{
		typedef typename Array1::const_pointer				const_pointer1;
		typedef typename Array2::const_pointer				const_pointer2;
		typedef typename ptrdiff_list_type::size_type		size_type;
		typedef typename ptrdiff_list_type::difference_type	difference_type;
		typedef typename ptrdiff_list_type::value_type		value_type;

		ptrdiff_list_type list;

		difference_type w = components.width( );
		difference_type h = components.height( );
		difference_type d = components.depth( );
		difference_type cx = w / 2;
		difference_type cy = h / 2;
		difference_type cz = d / 2;
		difference_type ox1 = in1.width( ) / 2;
		difference_type oy1 = in1.height( ) / 2;
		difference_type oz1 = in1.depth( ) / 2;
		difference_type ox2 = in2.width( ) / 2;
		difference_type oy2 = in2.height( ) / 2;
		difference_type oz2 = in2.depth( ) / 2;

		const_pointer1 p1 = &in1( ox1, oy1, oz1 );
		const_pointer2 p2 = &in2( ox2, oy2, oz2 );

		for( difference_type k = 0 ; k < d ; k++ )
		{
			for( difference_type j = 0 ; j < h ; j++ )
			{
				for( difference_type i = 0 ; i < w ; i++ )
				{
					if( components( i - cx, j - cy, k - cz ) )
					{
						ptrdiff_t d1 = &in1( ox1 + i - cx, oy1 + j - cy, oz1 + k - cz ) - p1;
						ptrdiff_t d2 = &in2( ox2 + i - cx, oy2 + j - cy, oz2 + k - cz ) - p2;
						list.push_back( pointer_diff2( d1, d2 ) );
					}
				}
			}
		}

		return( list );
	}


	template < class Array1, class Array2, class Component >
	ptrdiff_list_type create_update_list( const Array1 &in1, const Array2 &in2, const Component &components )
	{
		typedef typename Array1::const_pointer				const_pointer1;
		typedef typename Array2::const_pointer				const_pointer2;
		typedef typename ptrdiff_list_type::size_type		size_type;
		typedef typename ptrdiff_list_type::difference_type	difference_type;
		typedef typename ptrdiff_list_type::value_type		value_type;

		ptrdiff_list_type list;

		difference_type w = components.width( );
		difference_type h = components.height( );
		difference_type d = components.depth( );
		difference_type cx = w / 2;
		difference_type cy = h / 2;
		difference_type cz = d / 2;
		difference_type ox1 = in1.width( ) / 2;
		difference_type oy1 = in1.height( ) / 2;
		difference_type oz1 = in1.depth( ) / 2;
		difference_type ox2 = in2.width( ) / 2;
		difference_type oy2 = in2.height( ) / 2;
		difference_type oz2 = in2.depth( ) / 2;

		const_pointer1 p1 = &in1( ox1, oy1, oz1 );
		const_pointer2 p2 = &in2( ox2, oy2, oz2 );

		switch( components.expand_mode( ) )
		{
		case NC4:
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1, oz1 ) - p1, &in2( ox2 - 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1, oz1 ) - p1, &in2( ox2 + 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 - 1, oz1 ) - p1, &in2( ox2, oy2 - 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 + 1, oz1 ) - p1, &in2( ox2, oy2 + 1, oz2 ) - p2 ) );
			break;

		case NC6:
			list.push_back( pointer_diff2( &in1( ox1 - 1, oy1, oz1 ) - p1, &in2( ox2 - 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1 + 1, oy1, oz1 ) - p1, &in2( ox2 + 1, oy2, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 - 1, oz1 ) - p1, &in2( ox2, oy2 - 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1 + 1, oz1 ) - p1, &in2( ox2, oy2 + 1, oz2 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1, oz1 - 1 ) - p1, &in2( ox2, oy2, oz2 - 1 ) - p2 ) );
			list.push_back( pointer_diff2( &in1( ox1, oy1, oz1 + 1 ) - p1, &in2( ox2, oy2, oz2 + 1 ) - p2 ) );
			break;

		case ALL:
		default:
			list = create_component_list( in1, in2, components );
			break;
		}

		return( list );
	}


	template < class T, class Allocator, class Array, class PointType >
	bool check_component_list( const array2< T, Allocator > &in1, const Array &in2, const PointType &pt, pointer_diff2 &ptr )
	{
		typedef typename array2< T, Allocator >::const_pointer	const_pointer1;
		typedef typename Array::const_pointer					const_pointer2;
		typedef typename ptrdiff_list_type::difference_type		difference_type;

		if( pt.x < 0 || pt.x >= static_cast< difference_type >( in1.width( ) ) || pt.y < 0 || pt.y >= static_cast< difference_type >( in1.height( ) ) )
		{
			return( false );
		}
		else if( in2( pt.x, pt.y ) != 0 )
		{
			return( false );
		}
		else
		{
			ptr.diff1 = &in1( pt.x, pt.y) - &in1[ 0 ];
			ptr.diff2 = &in2( pt.x, pt.y ) - &in2[ 0 ];
			return( true );
		}
	}


	template < class T, class Allocator, class Array, class PointType >
	bool check_component_list( const array3< T, Allocator > &in1, const Array &in2, const PointType &pt, pointer_diff2 &ptr )
	{
		typedef typename array3< T, Allocator >::const_pointer	const_pointer1;
		typedef typename Array::const_pointer					const_pointer2;
		typedef typename ptrdiff_list_type::difference_type		difference_type;

		if( pt.x < 0 || pt.x >= static_cast< difference_type >( in1.width( ) ) || pt.y < 0 || pt.y >= static_cast< difference_type >( in1.height( ) ) || pt.z < 0 || pt.z >= static_cast< difference_type >( in1.depth( ) ) )
		{
			return( false );
		}
		else if( in2( pt.x, pt.y, pt.z ) != 0 )
		{
			return( false );
		}
		else
		{
			ptr.diff1 = &in1( pt.x, pt.y, pt.z ) - &in1[ 0 ];
			ptr.diff2 = &in2( pt.x, pt.y, pt.z ) - &in2[ 0 ];
			return( true );
		}
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
//! @code 領域拡張法の使用例
//! typedef mist::region_growing_utility::point_type    point_type;     // 領域拡張の開始点を指定する型（mist::vector2 や mist::vector3 で代用可）
//! typedef mist::region_growing_utility::circle        component_type; // 領域拡張に用いる構造要素
//! typedef mist::region_growing_utility::less< short > condition_type; // 領域拡張のの拡張条件
//!
//! mist::region_growing(
//!                       in,                    // 入力画像
//!                       out,                   // 領域拡張を行った結果を格納する画像	
//!                       point_type( x, y ),    // 拡張開始点（リストにすることで複数指定可能）
//!                       128,                   // 領域拡張の結果に代入する値
//!                       component_type( 20 ),  // 領域拡張に用いる構造要素（画素，円，球など）
//!                       condition_type( 128 )  // 領域拡張のの拡張条件（未満，以上，範囲など）
//!                     );
//! @endcode
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
	typedef region_growing_utility::pointer_diff2 pointer_diff_type;

	typedef typename Array1::size_type			size_type;
	typedef typename Array1::value_type			value_type;
	typedef typename Array1::difference_type	difference_type;
	typedef typename Array2::value_type			out_value_type;

	typedef typename Array1::const_pointer		const_pointer;
	typedef typename mask_type::pointer			work_pointer;
	typedef typename Array2::pointer			output_pointer;

	size_type rx = components.width( ) / 2;
	size_type ry = components.height( ) / 2;
	size_type rz = components.depth( ) / 2;

	marray< mask_type > work( in, region_growing_utility::maximum( rx, ry, rz ) );	// 注目点が範囲外に出ないことを監視するマスク

	region_growing_utility::ptrdiff_list_type clist = region_growing_utility::create_component_list( in, work, components );	// 構造要素内の画素のリスト
	region_growing_utility::ptrdiff_list_type ulist = region_growing_utility::create_update_list( in, work, components );		// ある注目点の次に注目点になる画素のリスト

	std::deque< pointer_diff_type > que;								// 注目点画素のリスト
	std::vector< value_type > element( clist.size( ) );					// 拡張条件の判定に用いる画素の配列
	region_growing_utility::ptrdiff_list_type elist( clist.size( ) );	// 拡張条件の判定に用いる画素へのポインタを覚える配列

	difference_type w = in.width( );
	difference_type h = in.height( );
	difference_type d = in.depth( );
	size_type count = 0;

	// 画像の外側に拡張しないようにマスクを設定する
	work.fill( );
	work.fill_margin( 255 );

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

	// 拡張開始点が画像内に存在しているかどうかをチェックする
	for( typename PointList::const_iterator ite = start_points.begin( ) ; ite != start_points.end( ) ; ++ite )
	{
		region_growing_utility::pointer_diff2 ptr;
		if( region_growing_utility::check_component_list( in, work, *ite, ptr ) )
		{
			work[ ptr.diff2 ] = 1;
			que.push_back( ptr );
		}
	}

	// 出力画像の大きさをチェックする
	if( in.size( ) != out.size( ) )
	{
		out.resize( in.width( ), in.height( ), in.depth( ) );
		out.reso1( in.reso1( ) );
		out.reso2( in.reso2( ) );
		out.reso3( in.reso3( ) );
	}

	// 領域拡張の条件によって，構造要素内の全要素を判定に用いるかどうかを決める
	// require_all_elements が false の場合には，すでに判定済みの画素は2度目の判定を行わない
	if( condition.require_all_elements( ) )
	{
		// 構造要素内のすべての点を用いて評価を行う
		while( !que.empty( ) )
		{
			// リストの先頭画素を取り出す
			pointer_diff_type cur = que.front( );
			que.pop_front( );

			const_pointer  pi = &in[ 0 ] + cur.diff1;
			work_pointer   pw = &work[ 0 ] + cur.diff2;
			output_pointer po = &out[ 0 ] + cur.diff1;

			// 拡張条件判定に用いる画素を列挙する
			// ここで，マスクされている領域は範囲に含めない（画像外など）
			size_type num = 0;
			for( size_type i = 0 ; i < clist.size( ) ; i++ )
			{
				const pointer_diff_type &d = clist[ i ];
				if( pw[ d.diff2 ] <= 1 )
				{
					element[ num ] = pi[ d.diff1 ];
					elist[ num ]   = d;
					num++;
				}
			}

			// 拡張条件の判定を行う
			if( condition( element, num ) )
			{
				if( ++count > max_num )
				{
					return( false );
				}

				// 条件を満たした構造要素内の画素すべてを塗りつぶす
				for( size_type i = 0 ; i < num ; i++ )
				{
					const pointer_diff_type &d = elist[ i ];
					po[ d.diff1 ] = output_value;
				}

				// 構造要素によって次の注目点を決定し，リストの最後に追加する
				for( size_type i = 0 ; i < ulist.size( ) ; i++ )
				{
					const pointer_diff_type &d = ulist[ i ];
					if( pw[ d.diff2 ] <= 128 )
					{
						pw[ d.diff2 ] = 200;
						que.push_back( pointer_diff_type( cur.diff1 + d.diff1, cur.diff2 + d.diff2 ) );
					}
				}
			}
		}
	}
	else
	{
		// すでに判定された画素については評価を行わない
		while( !que.empty( ) )
		{
			// リストの先頭画素を取り出す
			pointer_diff_type cur = que.front( );
			que.pop_front( );

			const_pointer  pi = &in[ 0 ] + cur.diff1;
			work_pointer   pw = &work[ 0 ] + cur.diff2;
			output_pointer po = &out[ 0 ] + cur.diff1;

			// 拡張条件判定に用いる画素を列挙する
			// ここで，マスクされている領域は範囲に含めない（画像外など）
			// また，すでに判定済みの画素も除外する
			// 判定画素と更新部分のリストを作成する
			size_type num = 0;
			for( size_type i = 0 ; i < clist.size( ) ; i++ )
			{
				const pointer_diff_type &d = clist[ i ];
				if( pw[ d.diff2 ] <= 1 )
				{
					element[ num ] = pi[ d.diff1 ];
					elist[ num ]   = d;
					num++;
				}
			}

			// 拡張条件の判定を行う
			if( condition( element, num ) )
			{
				if( ++count > max_num )
				{
					return( false );
				}

				// 条件を満たした構造要素内の画素すべてを塗りつぶす
				for( size_type i = 0 ; i < num ; i++ )
				{
					const pointer_diff_type &d = elist[ i ];
					pw[ d.diff2 ] = 2;
					po[ d.diff1 ] = output_value;
				}

				// 構造要素によって次の注目点を決定し，リストの最後に追加する
				for( size_type i = 0 ; i < ulist.size( ) ; i++ )
				{
					const pointer_diff_type &d = ulist[ i ];
					if( pw[ d.diff2 ] <= 128 )
					{
						pw[ d.diff2 ] = 200;
						que.push_back( pointer_diff_type( cur.diff1 + d.diff1, cur.diff2 + d.diff2 ) );
					}
				}
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
