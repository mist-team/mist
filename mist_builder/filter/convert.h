#ifndef __INCLUDE_CONVERT_FILTER__
#define __INCLUDE_CONVERT_FILTER__

#include <mist/converter.h>


class color2gray_filter : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Color -> Gray" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new color2gray_filter( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			mist::convert( in.color_image_, data_.gray_image_ );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	color2gray_filter( )
	{
		input_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::GRAY ) );
	}
};


#endif	// __INCLUDE_CONVERT_FILTER__
