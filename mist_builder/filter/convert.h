#ifndef __INCLUDE_CONVERT_FILTER__
#define __INCLUDE_CONVERT_FILTER__

#include <mist/converter.h>


class color2gray_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Color -> Gray" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new color2gray_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
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


class color2mono_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Color -> Mono" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new color2mono_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::convert( in.color_image_, data_.mono_image_ );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	color2mono_filter( )
	{
		input_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::MONO ) );
	}
};


class gray2mono_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Gray -> Mono" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new gray2mono_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::convert( in.gray_image_, data_.mono_image_ );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	gray2mono_filter( )
	{
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::MONO ) );
	}
};


class gray2color_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Gray -> Color" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new gray2color_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::convert( in.gray_image_, data_.color_image_ );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	gray2color_filter( )
	{
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::COLOR ) );
	}
};


class mono2color_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Mono -> Color" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new mono2color_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::convert( in.mono_image_, data_.color_image_ );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	mono2color_filter( )
	{
		input_pins_.push_back( pin( pin::MONO ) );
		output_pins_.push_back( pin( pin::COLOR ) );
	}
};



class mono2gray_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Mono -> Gray" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new mono2gray_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::convert( in.mono_image_, data_.gray_image_ );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	mono2gray_filter( )
	{
		input_pins_.push_back( pin( pin::MONO ) );
		output_pins_.push_back( pin( pin::GRAY ) );
	}
};


#endif	// __INCLUDE_CONVERT_FILTER__
