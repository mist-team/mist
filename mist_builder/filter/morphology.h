#ifndef __INCLUDE_MORPHOLOGY_FILTER__
#define __INCLUDE_MORPHOLOGY_FILTER__

#include <mist/filter/morphology.h>

class erosion_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Erosion" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new erosion_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			data_.gray_image_ = in.gray_image_;
			if( properties_[ 0 ].integer == 0 )
			{
				mist::erosion( data_.gray_image_, mist::morphology::circle( properties_[ 1 ].integer, 1, 1 ) );
			}
			else
			{
				mist::erosion( data_.gray_image_, mist::morphology::square( properties_[ 1 ].integer, 1, 1 ) );
			}
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	erosion_filter( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::TEXTCOMBO, "Element", "Circle;Square", 0 ) );
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};

class dilation_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Dilation" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new dilation_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			data_.gray_image_ = in.gray_image_;
			if( properties_[ 0 ].integer == 0 )
			{
				mist::dilation( data_.gray_image_, mist::morphology::circle( properties_[ 1 ].integer, 1, 1 ) );
			}
			else
			{
				mist::dilation( data_.gray_image_, mist::morphology::square( properties_[ 1 ].integer, 1, 1 ) );
			}
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	dilation_filter( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::TEXTCOMBO, "Element", "Circle;Square", 0 ) );
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};


class opening_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Opening" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new opening_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			data_.gray_image_ = in.gray_image_;
			if( properties_[ 0 ].integer == 0 )
			{
				mist::opening( data_.gray_image_, mist::morphology::circle( properties_[ 1 ].integer, 1, 1 ) );
			}
			else
			{
				mist::opening( data_.gray_image_, mist::morphology::square( properties_[ 1 ].integer, 1, 1 ) );
			}
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	opening_filter( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::TEXTCOMBO, "Element", "Circle;Square", 0 ) );
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};


class closing_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Closing" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new closing_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			data_.gray_image_ = in.gray_image_;
			if( properties_[ 0 ].integer == 0 )
			{
				mist::closing( data_.gray_image_, mist::morphology::circle( properties_[ 1 ].integer, 1, 1 ) );
			}
			else
			{
				mist::closing( data_.gray_image_, mist::morphology::square( properties_[ 1 ].integer, 1, 1 ) );
			}
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	closing_filter( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::TEXTCOMBO, "Element", "Circle;Square", 0 ) );
		properties_.push_back( property( property::INTEGER, "Mask Size", 3 ) );
	}
};


#endif	// __INCLUDE_MORPHOLOGY_FILTER__
