#include "config.h"

#include "property.h"


#include <mist/draw.h>


FXDEFMAP( property ) property_map[] =
	{
		//________Message_Type_____________________ID_________________________Message_Handler_______
		FXMAPFUNC ( SEL_PAINT,				0,								property::onPaint ),
		FXMAPFUNC ( SEL_SELECTED,			property::ID_LIST,				property::onSelected ),
		FXMAPFUNC ( SEL_DESELECTED,			property::ID_LIST,				property::onDeSelected ),
		FXMAPFUNC ( SEL_KEYPRESS,			0,								property::onKeyDown ),
		FXMAPFUNC ( SEL_KEYRELEASE,			0,								property::onKeyUp ),
	};


FXIMPLEMENT( property, property::base, property_map, ARRAYNUMBER( property_map ) )



// Construct a property
property::property( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h )
				: base( p, NULL, ID_LIST, opts, x, y, w, h ), current_item_( NULL )
{
	setTarget( this );

	boolean_combo_ = new FXComboBox( this, 1, this, ID_COMBO, COMBOBOX_STATIC | LAYOUT_FIX_X | LAYOUT_FIX_Y | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 0, 5, 6, 2, 0 );
	boolean_combo_->appendItem( "false" );
	boolean_combo_->appendItem( "true" );
	boolean_combo_->setNumVisible( 2 );

	text_input_ = new FXTextField( this, 255, this, ID_TEXT, FRAME_NONE | LAYOUT_FIX_X | LAYOUT_FIX_Y | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 0, 5, 6, 2, 0 );

	data[ 0 ] = property_data( property_data::TEXT, "文字列です" );
	data[ 1 ] = property_data( property_data::BOOLEAN, true );
	data[ 2 ] = property_data( property_data::INTEGER, 3 );
	data[ 3 ] = property_data( property_data::REAL, 2.5 );

	appendItem( "string", NULL, &( data[ 0 ] ) );
	appendItem( "boolean", NULL, &( data[ 1 ] ) );
	appendItem( "integer", NULL, &( data[ 2 ] ) );
	appendItem( "real", NULL, &( data[ 3 ] ) );
}

property::~property( )
{
}


void property::create( )
{
	boolean_combo_->setFont( getFont( ) );
	text_input_->setFont( getFont( ) );

	boolean_combo_->hide( );
	text_input_->hide( );

	base::create( );
}


long property::onKeyDown( FXObject *obj, FXSelector sel, void *ptr )
{
	return( text_input_->shown( ) ? text_input_->handle( obj, FXSEL( SEL_KEYPRESS, 0 ), ptr ) : 1 );
}

long property::onKeyUp( FXObject *obj, FXSelector sel, void *ptr )
{
	return( text_input_->shown( ) ? text_input_->handle( obj, FXSEL( SEL_KEYRELEASE, 0 ), ptr ) : 1 );
}

long property::onSelected( FXObject *obj, FXSelector sel, void *ptr )
{
	// ボックスの表示位置を決定する
	FXint margin = 3;
	FXint curItem = getCurrentItem( );
	FXint posY = 0;
	FXint itemW = 0;
	FXint winW = getWidth( );
	FXint i;

	// 項目の最大サイズを取得する
	for( i = 0 ; i < curItem ; i++ )
	{
		FXint w = getItemWidth( i );
		if( itemW < w )
		{
			itemW = w;
		}
		posY += getItemHeight( i );
	}
	for( ; i < getNumItems( ) ; i++ )
	{
		FXint w = getItemWidth( i );
		if( itemW < w )
		{
			itemW = w;
		}
	}

	if( itemW > winW / 2 )
	{
		itemW = winW / 2;
	}

	itemW += margin * 4;

	FXint itemH = getItemHeight( curItem );
	FXint l = 1 + itemW;
	FXint t = 1 + posY;
	FXint w = winW - itemW - 2;
	FXint h = itemH - 2;


	// 表示するデータを設定する
	property_data *item = current_item_ = static_cast< property_data * >( getItemData( curItem ) );
	if( item != NULL )
	{
		switch( item->type )
		{
		case property_data::BOOLEAN:
			boolean_combo_->setCurrentItem( item->boolean );
			boolean_combo_->position( l, t, w, h );
			boolean_combo_->show( );
			break;

		case property_data::INTEGER:
			text_input_->setTextStyle( TEXTFIELD_INTEGER | TEXTFIELD_LIMITED );
			text_input_->setText( FXStringVal( item->integer ) );
			text_input_->position( l, t, w, h );
			text_input_->show( );
			text_input_->setFocus( );
			break;

		case property_data::REAL:
			text_input_->setTextStyle( TEXTFIELD_REAL | TEXTFIELD_LIMITED );
			text_input_->setText( FXStringVal( item->real ) );
			text_input_->position( l, t, w, h );
			text_input_->show( );
			text_input_->setFocus( );
			break;

		case property_data::TEXT:
			text_input_->setTextStyle( TEXTFIELD_LIMITED );
			text_input_->setText( item->text );
			text_input_->position( l, t, w, h );
			text_input_->show( );
			text_input_->setFocus( );
			break;

		default:
			break;
		}
	}

	return( 1 );
}


long property::onDeSelected( FXObject *obj, FXSelector sel, void *ptr )
{
	// 変更結果を反映する
	if( current_item_ != NULL )
	{
		switch( current_item_->type )
		{
		case property_data::BOOLEAN:
			current_item_->boolean = boolean_combo_->getCurrentItem( ) ? true : false;
			break;

		case property_data::INTEGER:
			current_item_->integer = FXIntVal( text_input_->getText( ) );
			break;

		case property_data::REAL:
			current_item_->real = FXDoubleVal( text_input_->getText( ) );
			break;

		case property_data::TEXT:
			current_item_->text = text_input_->getText( );
			break;

		default:
			break;
		}
	}

	current_item_ = NULL;

	boolean_combo_->hide( );
	text_input_->hide( );

	return( 1 );
}



// Paint the canvas
long property::onPaint( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	FXDCWindow dc( this, &e );

	FXFont *font = getFont( );
	dc.setFont( font );

	// キャンバスの範囲外にある領域を塗りつぶす
	dc.setForeground( FXRGB( 128, 128, 128 ) );
	dc.fillRectangle( 0, 0, getWidth( ), getHeight( ) );

	FXint margin = 3;

	dc.setForeground( FXRGB( 0, 0, 0 ) );

	FXint posY = 0;
	FXint itemW = 0;
	FXint winW = getWidth( );

	// 項目の最大サイズを取得する
	for( FXint i = 0 ; i < getNumItems( ) ; i++ )
	{
		FXint w = getItemWidth( i );
		if( itemW < w )
		{
			itemW = w;
		}
	}

	if( itemW > winW / 2 )
	{
		itemW = winW / 2;
	}

	itemW += margin * 4;

	for( FXint i = 0 ; i < getNumItems( ) ; i++ )
	{
		FXint itemH = getItemHeight( i );
		FXint l = 0;
		FXint t = posY;
		FXint r = l + winW;
		FXint b = t + itemH;

		// 描画領域を設定する
		dc.setClipRectangle( l, t, r - l, b - t );

		// 外枠を描画する
		dc.setForeground( FXRGB( 255, 255, 255 ) );
		dc.fillRectangle( l, t, r - l, b - t );
		if( isItemSelected( i ) )
		{
			// 選択されている場合は，背景を若干青くする
			dc.setForeground( FXRGB( 49, 106, 197 ) );
			dc.fillRectangle( l, t, itemW, b - t );
		}
		dc.setForeground( FXRGB( 212, 208, 200 ) );
		dc.drawRectangle( l, t, r - l, b - t );
		dc.drawLine( l + itemW, t, l + itemW, b );


		// 文字を描画する
		FXString str = getItemText( i );
		dc.setForeground( FXRGB( 0, 0, 0 ) );
		dc.setClipRectangle( l, t, itemW, b - t );
		dc.drawText( l + margin * 2, b - margin, str.text( ), str.length( ) );

		// データ部分を描画する
		property_data *item = static_cast< property_data * >( getItemData( i ) );
		if( item != NULL )
		{
			dc.setForeground( FXRGB( 0, 0, 0 ) );
			dc.setClipRectangle( l + itemW, t, winW - itemW, b - t );
			FXString text;
			switch( item->type )
			{
			case property_data::BOOLEAN:
				text = item->boolean ? "true": "false";
				break;

			case property_data::INTEGER:
				text = FXStringVal( item->integer );
				break;

			case property_data::REAL:
				text = FXStringVal( item->real );
				break;

			case property_data::TEXT:
				text = item->text;
				break;

			default:
				text = "";
				break;
			}
			dc.drawText( l + itemW + margin * 2, b - margin, text.text( ), text.length( ) );
		}

		posY += itemH;
	}

	return( 1 );
}