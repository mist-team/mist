#ifndef __INCLUDE_COPY_FILTER__
#define __INCLUDE_COPY_FILTER__



class copy_filter1 : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Copy Image (Color)" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new copy_filter1( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			data_ = in;
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	copy_filter1( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::COLOR ) );
	}
};

class copy_filter2 : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Copy Image (Gray)" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new copy_filter2( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			data_ = in;
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	copy_filter2( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );
	}
};

class copy_filter3 : public filter
{
public:
	/// @brief フィルタの名前を返す
	virtual std::string name( ) const { return( "Copy Image (Mono)" ); }

	/// @brief 自分のコピーを返す
	virtual filter *clone( ) const { return( new copy_filter3( ) ); }

	/// @brief フィルタ処理を行って，data_ に格納する
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// 全てのデータが接続されている場合のみ，フィルタリングを行う
			data_type &in = get_input_data( 0 );
			data_ = in;
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	copy_filter3( )
	{
		// ピンを設定する
		input_pins_.push_back( pin( pin::MONO ) );
		output_pins_.push_back( pin( pin::MONO ) );
		output_pins_.push_back( pin( pin::MONO ) );
	}
};



#endif	// __INCLUDE_COPY_FILTER__
