#ifndef __INCLUDE_MEDIAN_FILTER__
#define __INCLUDE_MEDIAN_FILTER__

#include <mist/filter/median.h>

class median_filter1 : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Median filter (Color)" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new median_filter1( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			mist::median( in.color_image_, data_.color_image_, properties_[ 0 ].integer );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	median_filter1( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::COLOR ) );

		// プロパティを設定する
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};

class median_filter2 : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Median filter (Gray)" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new median_filter2( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			mist::median( in.gray_image_, data_.gray_image_, properties_[ 0 ].integer );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	median_filter2( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );

		// プロパティを設定する
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};

class median_filter3 : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Median filter (Mono)" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new median_filter3( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			mist::median( in.mono_image_, data_.mono_image_, properties_[ 0 ].integer );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	median_filter3( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::MONO ) );
		output_pins_.push_back( pin( pin::MONO ) );

		// プロパティを設定する
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};

#endif	// __INCLUDE_MEDIAN_FILTER__
