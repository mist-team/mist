#ifndef __INCLUDE_MEDIAN_FILTER__
#define __INCLUDE_MEDIAN_FILTER__

#include <mist/filter/median.h>

class median_filter : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Median filter" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new median_filter( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			mist::median( in, data_, 3 );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	median_filter( )
	{
		input_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::COLOR ) );
	}
};


#endif	// __INCLUDE_MEDIAN_FILTER__
