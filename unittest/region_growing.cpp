#include <iostream>
#include "macros.h"
#include <mist/filter/region_growing.h>

class TestRegionGrowing : public CPPUNIT_NS::TestCase
{
private:
	typedef mist::array2< unsigned char >	image_type;
	typedef image_type::size_type			size_type;
	typedef image_type::difference_type		difference_type;
	typedef image_type::value_type			value_type;

	CPPUNIT_TEST_SUITE( TestRegionGrowing );
	CPPUNIT_TEST( PaintedRegion1 );
	CPPUNIT_TEST( PaintedRegion2 );
	CPPUNIT_TEST( PaintedRegion3 );
	CPPUNIT_TEST_SUITE_END( );

	image_type image;

public:
	TestRegionGrowing( ) : image( 10, 10 )
	{
		for( size_type j = 1 ; j < image.height( ) - 1 ; j++ )
		{
			for( size_type i = 1 ; i < image.width( ) - 1 ; i++ )
			{
				image( i, j ) = 1;
			}
		}
	}

protected:
	void PaintedRegion1( )
	{
		image_type tmp;

		typedef mist::vector2< difference_type >					point_type;     // 領域拡張の開始点を指定する型（mist::vector2 や mist::vector3 で代用可）
		typedef mist::region_growing_utility::pixel					component_type; // 領域拡張に用いる構造要素
		typedef mist::region_growing_utility::equal< value_type >	condition_type; // 領域拡張のの拡張条件

		difference_type num = mist::region_growing(
											image,					// 入力画像
											tmp,					// 領域拡張を行った結果を格納する画像	
											point_type( 5, 5 ),		// 拡張開始点（リストにすることで複数指定可能）
											2,						// 領域拡張の結果に代入する値
											component_type( ),		// 領域拡張に用いる構造要素（画素）
											condition_type( 1 )		// 領域拡張のの拡張条件（未満，以上，範囲など）
										);

		difference_type count[ 3 ] = { 0, 0, 0 };
		for( size_type i = 0 ; i < tmp.size( ) ; i++ )
		{
			count[ tmp[ i ] ]++;
		}

		CPPUNIT_ASSERTION_EQUAL( count[ 0 ], ( difference_type )36 );
		CPPUNIT_ASSERTION_EQUAL( count[ 1 ], ( difference_type )0  );
		CPPUNIT_ASSERTION_EQUAL( count[ 2 ], ( difference_type )64 );
		CPPUNIT_ASSERTION_EQUAL( num, count[ 2 ] );
	}

	void PaintedRegion2( )
	{
		image_type tmp;

		typedef mist::vector2< difference_type >					point_type;     // 領域拡張の開始点を指定する型（mist::vector2 や mist::vector3 で代用可）
		typedef mist::region_growing_utility::circle				component_type; // 領域拡張に用いる構造要素
		typedef mist::region_growing_utility::equal< value_type >	condition_type; // 領域拡張のの拡張条件

		difference_type num = mist::region_growing(
											image,					// 入力画像
											tmp,					// 領域拡張を行った結果を格納する画像	
											point_type( 5, 5 ),		// 拡張開始点（リストにすることで複数指定可能）
											2,						// 領域拡張の結果に代入する値
											component_type( 2 ),	// 領域拡張に用いる構造要素（円）
											condition_type( 1 )		// 領域拡張のの拡張条件（未満，以上，範囲など）
										);

		difference_type count[ 3 ] = { 0, 0, 0 };
		for( size_type i = 0 ; i < tmp.size( ) ; i++ )
		{
			count[ tmp[ i ] ]++;
		}

		CPPUNIT_ASSERTION_EQUAL( count[ 0 ], ( difference_type )48 );
		CPPUNIT_ASSERTION_EQUAL( count[ 1 ], ( difference_type )0  );
		CPPUNIT_ASSERTION_EQUAL( count[ 2 ], ( difference_type )52 );
		CPPUNIT_ASSERTION_EQUAL( num, count[ 2 ] );
	}

	void PaintedRegion3( )
	{
		image_type tmp;

		typedef mist::vector2< difference_type >					point_type;     // 領域拡張の開始点を指定する型（mist::vector2 や mist::vector3 で代用可）
		typedef mist::region_growing_utility::pixel					component_type; // 領域拡張に用いる構造要素
		typedef mist::region_growing_utility::equal< value_type >	condition_type; // 領域拡張のの拡張条件

		difference_type num = mist::region_growing(
											image,					// 入力画像
											tmp,					// 領域拡張を行った結果を格納する画像	
											point_type( 5, 5 ),		// 拡張開始点（リストにすることで複数指定可能）
											2,						// 領域拡張の結果に代入する値
											component_type( ),		// 領域拡張に用いる構造要素（画素）
											condition_type( 1 ),	// 領域拡張のの拡張条件（未満，以上，範囲など）
											30
										);

		difference_type count[ 3 ] = { 0, 0, 0 };
		for( size_type i = 0 ; i < tmp.size( ) ; i++ )
		{
			count[ tmp[ i ] ]++;
		}

		CPPUNIT_ASSERTION_EQUAL( count[ 0 ], ( difference_type )70 );
		CPPUNIT_ASSERTION_EQUAL( count[ 1 ], ( difference_type )0  );
		CPPUNIT_ASSERTION_EQUAL( count[ 2 ], ( difference_type )30 );
		CPPUNIT_ASSERTION_EQUAL( num, 30 );
	}
};


CPPUNIT_TEST_SUITE_REGISTRATION( TestRegionGrowing );
