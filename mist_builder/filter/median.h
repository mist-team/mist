#ifndef __INCLUDE_MEDIAN_FILTER__
#define __INCLUDE_MEDIAN_FILTER__

#include <mist/filter/median.h>

class median_filter1 : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Median filter (Color)" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new median_filter1( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
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
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::COLOR ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};

class median_filter2 : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Median filter (Gray)" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new median_filter2( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
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
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};

class median_filter3 : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Median filter (Mono)" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new median_filter3( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
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
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::MONO ) );
		output_pins_.push_back( pin( pin::MONO ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};

#endif	// __INCLUDE_MEDIAN_FILTER__
