#ifndef __INCLUDE_INPUT_FILTER__
#define __INCLUDE_INPUT_FILTER__

#include <mist/io/image.h>

class input_filter : public filter
{
public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Input filter" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new input_filter( ) ); }

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		// �f�[�^���t�@�C������ǂݍ���
		FXString filename = properties_[ 0 ].text;

		if( filename != "" )
		{
			mist::read_image( data_.color_image_, filename.text( ) );
		}
		else
		{
			data_.clear( );
		}

		return( true );
	}

	input_filter( )
	{
		// �s����ݒ肷��
		output_pins_.push_back( pin( pin::COLOR ) );

		// �v���p�e�B��ݒ肷��
		properties_.push_back( property( property::FILEOPEN, "Open Image", "" ) );
	}
};


#endif	// __INCLUDE_INPUT_FILTER__
