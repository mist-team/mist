#ifndef __INCLUDE_THRESHOLD_FILTER__
#define __INCLUDE_THRESHOLD_FILTER__

#include <mist/threshold.h>

class ptile_threshold_filter : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "P-Tile" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new ptile_threshold_filter( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			element_type2 th = mist::ptile::threshold( in.gray_image_, properties_[ 0 ].real );

			data_.mono_image_.resize( in.gray_image_.width( ), in.gray_image_.height( ) );
			data_.mono_image_.reso( in.gray_image_.reso1( ), in.gray_image_.reso2( ) );

			for( size_type i = 0 ; i < in.gray_image_.size( ) ; i++ )
			{
				data_.mono_image_[ i ] = in.gray_image_[ i ] < th ? 0 : 1;
			}
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	ptile_threshold_filter( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::MONO ) );

		// プロパティを設定する
		properties_.push_back( property( property::REAL, "Ratio", 90.5 ) );
	}
};

class discriminant_analysis_threshold_filter : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Discriminant Analysis" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new discriminant_analysis_threshold_filter( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			element_type2 th = mist::discriminant_analysis::threshold( in.gray_image_ );

			data_.mono_image_.resize( in.gray_image_.width( ), in.gray_image_.height( ) );
			data_.mono_image_.reso( in.gray_image_.reso1( ), in.gray_image_.reso2( ) );

			for( size_type i = 0 ; i < in.gray_image_.size( ) ; i++ )
			{
				data_.mono_image_[ i ] = in.gray_image_[ i ] < th ? 0 : 1;
			}
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	discriminant_analysis_threshold_filter( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::MONO ) );
	}
};


#endif	// __INCLUDE_THRESHOLD_FILTER__
