#include "config.h"

#include "main_window.h"

#include "filter/input.h"
#include "filter/median.h"


void main_window::ClearFilters( )
{
	mist_builder &app = getAppInstance( );
	app.filters.clear( );

	for( size_type i = 0 ; i < app.filters.size( ) ; i++ )
	{
		delete app.filters[ i ];
	}

	app.filters.clear( );
}

void main_window::EnumFilters( )
{
	mist_builder &app = getAppInstance( );
	ClearFilters( );

	// �O���[�o���X�y�[�X�Ƀt�B���^�̈ꗗ���쐬����
	app.filters.push_back( new input_filter( ) );
	app.filters.push_back( new median_filter( ) );


	// �t�B���^�f�[�^�����X�g�ɓo�^����
	listbox_->clearItems( );
	for( size_type i = 0 ; i < app.filters.size( ) ; i++ )
	{
		listbox_->appendItem( app.filters[ i ]->name( ).c_str( ) );
	}
}


