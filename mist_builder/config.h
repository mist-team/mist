#ifndef __INCLUDE_DIO_DEF__
#define __INCLUDE_DIO_DEF__


#ifndef __INCLUDE_MIST_H__
#include <mist/mist.h>
#endif

#ifndef __INCLUDE_MIST_COLOR_H__
#include <mist/config/color.h>
#endif

#ifndef __INCLUDE_MIST_VECTOR__
#include <mist/vector.h>
#endif

#ifndef __INCLUDE_MIST_MATRIX__
#include <mist/matrix.h>
#endif


#include <vector>
#include <deque>
#include <set>


typedef mist::rgb< unsigned char > color_type;

typedef mist::rgb< unsigned char > element_type;
typedef mist::array2< element_type > data_type;


typedef mist::vector2< double > point2;
typedef mist::vector3< double > point3;

typedef mist::matrix< double > matrix;

#include <fx.h>
#include <fx3d.h>


// ���[�U�[��`���b�Z�[�W
enum UserMessage
{
	MIST_FILTER_CHANGED = FX::SEL_LAST,
	MIST_FILTER_IMAGE_CHANGED,
};


typedef size_t size_type;
typedef ptrdiff_t difference_type;

class filter;

struct layout_parameter
{
	const static FXint margin = 5;
	const static FXint box_size = 8;

	FXint x;
	FXint y;
	FXint width;
	FXint height;

	bool active;

	bool hittest( FXint xx, FXint yy ) const
	{
		if( xx < x || xx > x + width || yy < y || yy > y + height )
		{
			return( false );
		}
		else
		{
			return( true );
		}
	}

	layout_parameter( ) : x( 0 ), y( 0 ), width( 0 ), height( 0 ), active( false )
	{
	}
};

class pin : public layout_parameter
{
public:
	enum PINTYPE
	{
		NONE,
		BINARY,
		GRAY,
		COLOR,
	};

private:
	PINTYPE pin_type_;
	pin		*connected_pin_;
	filter	*related_filter_;

public:
	PINTYPE pin_type( ) const { return( pin_type_ ); }

	pin *connected_pin( ) const { return( connected_pin_ ); }
	pin *connected_pin( pin *p ){ return( connected_pin_ = p ); }

	filter *related_filter( ) const { return( related_filter_ ); }
	filter *related_filter( filter *f ){ return( related_filter_ = f ); }

	pin( PINTYPE t = NONE ) : pin_type_( t ), connected_pin_( NULL ){ }
};

typedef std::vector< pin > pin_list;




class filter : public layout_parameter
{
protected:
	pin_list input_pins_;
	pin_list output_pins_;

	data_type data_;

public:
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const = 0;

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const = 0;

	/// @brief �t�B���^�������s���āCdata_ �Ɋi�[����
	virtual bool filtering( )
	{
		// �f�t�H���g�͒P�Ȃ���̓f�[�^�̃R�s�[
		if( !is_input_pins_connected( ) )
		{
			data_.clear( );
		}
		else
		{
			pin *p = input_pins_[ 0 ].connected_pin( );
			filter *pf = p->related_filter( );
			data_ = pf->data( );
		}

		return( true );
	}

public:
	/// @brief ���̓s���̏W����Ԃ�
	pin_list &input_pins( ){ return( input_pins_ ); }

	/// @brief ���̓s���̏W����Ԃ�
	const pin_list &input_pins( ) const { return( input_pins_ ); }


	/// @brief �o�̓s���̏W����Ԃ�
	pin_list &output_pins( ){ return( output_pins_ ); }

	/// @brief �o�̓s���̏W����Ԃ�
	const pin_list &output_pins( ) const { return( output_pins_ ); }


	/// @brief �����Ŏ����Ă���f�[�^��Ԃ�
	data_type &data( ){ return( data_ ); }

	/// @brief �����Ŏ����Ă���f�[�^��Ԃ�
	const data_type &data( ) const { return( data_ ); }


	/// @brief �S�Ă̓��̓s�����ڑ�����Ă���ꍇ�ɐ^��Ԃ�
	bool is_input_pins_connected( ) const
	{
		for( size_type i = 0 ; i < input_pins_.size( ) ; i++ )
		{
			if( input_pins_[ i ].connected_pin( ) == NULL )
			{
				return( false );
			}
		}
		return( true );
	}

	/// @brief �s���ɑΉ�����f�[�^���擾����
	//!
	//! @attention index ���L���Ȕ͈͂ɂ���C���s�����ڑ�����Ă���K�v����
	data_type &get_input_data( size_type index )
	{
		pin *p = input_pins_[ index ].connected_pin( );
		filter *pf = p->related_filter( );
		return( pf->data( ) );
	}


	void move( FXint dx, FXint dy )
	{
		x += dx;
		y += dy;
		for( size_type i = 0 ; i < input_pins_.size( ) ; i++ )
		{
			input_pins_[ i ].x += dx;
			input_pins_[ i ].y += dy;
		}
		for( size_type i = 0 ; i < output_pins_.size( ) ; i++ )
		{
			output_pins_[ i ].x += dx;
			output_pins_[ i ].y += dy;
		}
	}

public:
	filter( ){ }
	virtual ~filter( ){ }
};

typedef std::deque< filter * > filter_list;
typedef std::set< filter * > filter_set;


struct sample_filter : public filter
{
	/// @brief �t�B���^�̖��O��Ԃ�
	virtual std::string name( ) const { return( "Median filter" ); }

	/// @brief �����̃R�s�[��Ԃ�
	virtual filter *clone( ) const { return( new sample_filter( ) ); }

	sample_filter( )
	{
		input_pins_.push_back( pin( pin::BINARY ) );
		input_pins_.push_back( pin( pin::BINARY ) );
		input_pins_.push_back( pin( pin::COLOR ) );
		input_pins_.push_back( pin( pin::GRAY ) );

		output_pins_.push_back( pin( pin::BINARY ) );
		output_pins_.push_back( pin( pin::BINARY ) );
		output_pins_.push_back( pin( pin::COLOR ) );
		output_pins_.push_back( pin( pin::GRAY ) );
	}
};


#endif	// __INCLUDE_DIO_DEF__


#include "mist_builder.h"


