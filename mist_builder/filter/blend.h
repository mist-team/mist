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
			data_type3 &in1 = get_input_data( 0 ).color_image_;
			data_type3 &in2 = get_input_data( 1 ).color_image_;
			data_type3 &out = data_.color_image_;

			double a = properties_[ 0 ].real;
			double ca = 1.0 - a;

			size_type w = in1.width( ) < in2.width( ) ? in1.width( ) : in2.width( );
			size_type h = in1.height( ) < in2.height( ) ? in1.height( ) : in2.height( );

			out.resize( w, h );
			out.reso( in1.reso1( ), in1.reso2( ) );

			for( size_type j = 0 ; j < h ; j++ )
			{
				for( size_type i = 0 ; i < w ; i++ )
				{
					out( i, j ) = in1( i, j ) * a + in2( i, j ) * ca;
				}
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
