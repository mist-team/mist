#ifndef __INCLUDE_BLEND_FILTER__
#define __INCLUDE_BLEND_FILTER__

#include <mist/threshold.h>

class blend_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Blending" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new blend_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		if( is_input_pins_connected( ) )
		{
			// �S�Ẵf�[�^���ڑ�����Ă���ꍇ�̂݁C�t�B���^�����O���s��
			data_type &in1 = get_input_data( 0 );
			data_type &in2 = get_input_data( 1 );

			data_.color_image_.resize( in1.color_image_.width( ), in1.color_image_.height( ) );
			data_.color_image_.reso( in1.color_image_.reso1( ), in1.color_image_.reso2( ) );

			double a = properties_[ 0 ].real;
			double ca = 1.0 - a;
			for( size_type i = 0 ; i < in1.color_image_.size( ) ; i++ )
			{
				data_.color_image_[ i ] = in1.color_image_[ i ] * a + in2.color_image_[ i ] * ca;
			}
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	blend_filter( )
	{
		// �s����ݒ肷��
		input_pins_.push_back( pin( pin::COLOR ) );
		input_pins_.push_back( pin( pin::COLOR ) );

		output_pins_.push_back( pin( pin::COLOR ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::REAL, "Alpha", 0.5 ) );
	}
};



#endif	// __INCLUDE_BLEND_FILTER__
