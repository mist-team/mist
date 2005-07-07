#include <iostream>
#include <mist/mist.h>
#include <mist/io/bmp.h>
#include <mist/filter/region_growing.h>
#include <mist/timer.h>

int main( int argc, char *argv[] )
{
	using namespace std;

	typedef mist::array2< unsigned int > image_type;
	typedef image_type::size_type size_type;
	typedef image_type::difference_type difference_type;

	image_type input( 100, 100 ), output;
	difference_type w = input.width( );
	difference_type h = input.height( );

	input.fill( 255 );

	for( difference_type j = 0 ; j < h ; j++ )
	{
		double ry = j - h / 2.0;

		for( difference_type i = 0 ; i < w ; i++ )
		{
			double rx = i - w / 2.0;

			if( std::abs( rx ) + std::abs( ry ) <= w * 0.4 )
			//if( rx * rx + ry * ry <= w * w  / 5.0 )
			{
				input( i, j ) = 0;
			}
		}
	}

	mist::timer t;

	typedef mist::vector2< int >								point_type;     // 領域拡張の開始点を指定する型（mist::vector2 や mist::vector3 で代用可）
//	typedef mist::region_growing_utility::pixel					component_type; // 領域拡張に用いる構造要素
	typedef mist::region_growing_utility::circle				component_type; // 領域拡張に用いる構造要素
	typedef mist::region_growing_utility::less< unsigned int >	condition_type; // 領域拡張のの拡張条件

	mist::region_growing(
							input,							// 入力画像
							output,							// 領域拡張を行った結果を格納する画像	
							point_type( (int)w / 2, (int)h / 2 ),		// 拡張開始点（リストにすることで複数指定可能）
							128,							// 領域拡張の結果に代入する値
							component_type( 20 ),			// 領域拡張に用いる構造要素（画素，円，球など）
							condition_type( 128 )			// 領域拡張のの拡張条件（未満，以上，範囲など）
						);

	cout << "Calculation Time: " << t << " (sec)" << endl;

	mist::write_bmp( output, "hoge.bmp" );

	return( 0 );
}
