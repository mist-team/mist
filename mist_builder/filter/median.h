#ifndef __INCLUDE_MEDIAN_FILTER__
#define __INCLUDE_MEDIAN_FILTER__

#include <mist/filter/median.h>

class median_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Median filter" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new median_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
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
