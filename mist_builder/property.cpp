#include "config.h"

#include "property.h"


#include <mist/draw.h>


FXDEFMAP( property ) property_map[] =
	{
		//________Message_Type_____________________ID_______________Message_Handler_______
		FXMAPFUNC ( SEL_COMMAND,			property::ID_TABLE,	property::onInterpolateChange ),
	};


FXIMPLEMENT( property, property::base, property_map, ARRAYNUMBER( property_map ) )



// Construct a property
property::property( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb, FXint hs, FXint vs )
				: base( p, opts, x, y, w, h, pl, pr, pt, pb, hs, vs )
{
	// このウィンドウからメッセージを送る先の設定
	setTarget( tgt );
	setSelector( sel );

	// クライアント領域に部品を配置する
	table_ = new FXTable( this, this, ID_TABLE, LAYOUT_FILL_X | LAYOUT_FILL_Y );

	table_->setRowHeaderWidth( 0 );
	table_->setColumnHeaderHeight( 0 );

	table_->setVisibleRows( 20 );
	table_->setVisibleColumns( 8 );
	//  table_->setRowHeaderMode( 0 );
	table_->setTableSize( 10, 2 );
	table_->setBackColor( FXRGB( 255, 255, 255 ) );
	table_->setCellColor( 0, 0, FXRGB( 255, 255, 255 ) );
	table_->setCellColor( 0, 1, FXRGB( 255, 240, 240 ) );
	table_->setCellColor( 1, 0, FXRGB( 240, 255, 240 ) );
	table_->setCellColor( 1, 1, FXRGB( 240, 240, 255 ) );

	// Initialize scrollable part of table_
	FXint r, c;
	for( r = 0 ; r < 10 ; r++ )
	{
		for( c = 0 ; c < 2 ; c++ )
		{
			table_->setItemText( r, c, "r:" + FXStringVal( r ) + " c:" + FXStringVal( c ) );
		}
	}

	//// Initialize column headers
	//for( c = 0 ; c < 12 ; c++ )
	//{
	//	table_->setColumnText( c, months[ c ] );
	//}

	//// Initialize row headers
	//for( r = 0 ; r < 50 ; r++ )
	//{
	//	table_->setRowText( r, "Row"+FXStringVal( r ) );
	//}
}

property::~property( )
{
}


void property::create( )
{
	base::create( );
}


long property::onInterpolateChange( FXObject *obj, FXSelector sel, void *ptr )
{
	return( 1 );
}

