#ifndef __INCLUDE_INTERLACE_FILTER__
#define __INCLUDE_INTERLACE_FILTER__

#include <mist/filter/interlace.h>

class interlace_filter1 : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Interlace (Color)" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new interlace_filter1( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::interlace( in.color_image_, data_.color_image_, properties_[ 0 ].integer == 0 );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	interlace_filter1( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::COLOR ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::TEXTCOMBO, "Interpolate Line", "Odd;Even", 1 ) );
	}
};


class interlace_filter2 : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Interlace (Gray)" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new interlace_filter2( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::interlace( in.gray_image_, data_.gray_image_, properties_[ 0 ].integer == 0 );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	interlace_filter2( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::GRAY ) );
		output_pins_.push_back( pin( pin::GRAY ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::TEXTCOMBO, "Interpolate Line", "Odd;Even", 1 ) );
	}
};


class interlace_filter3 : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Interlace (Mono)" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new interlace_filter3( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in = get_input_data( 0 );
			mist::interlace( in.mono_image_, data_.mono_image_, properties_[ 0 ].integer == 0 );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	interlace_filter3( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::MONO ) );
		output_pins_.push_back( pin( pin::MONO ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::TEXTCOMBO, "Interpolate Line", "Odd;Even", 1 ) );
	}
};


#endif	// __INCLUDE_INTERLACE_FILTER__