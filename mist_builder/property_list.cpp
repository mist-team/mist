#include "config.h"

#include "property_list.h"


#include <mist/draw.h>


FXDEFMAP( property_list ) property_list_map[] =
	{
		//________Message_Type_____________________ID_________________________Message_Handler_______
		FXMAPFUNC ( SEL_PAINT,				0,								property_list::onPaint ),
		FXMAPFUNC ( SEL_SELECTED,			property_list::ID_LIST,			property_list::onSelected ),
		FXMAPFUNC ( SEL_DESELECTED,			property_list::ID_LIST,			property_list::onDeSelected ),

		FXMAPFUNC ( SEL_KEYPRESS,			0,								property_list::onKeyDown ),
		FXMAPFUNC ( SEL_KEYRELEASE,			0,								property_list::onKeyUp ),

		FXMAPFUNC ( SEL_COMMAND,			property_list::ID_BUTTON,		property_list::onFileOpen ),

		FXMAPFUNC ( SEL_CHANGED,			property_list::ID_TEXT,			property_list::onDataChanged ),
		FXMAPFUNC ( SEL_CHANGED,			property_list::ID_COMBO,		property_list::onDataChanged ),
		FXMAPFUNC ( SEL_CHANGED,			property_list::ID_FILENAME,		property_list::onDataChanged ),
	};


FXIMPLEMENT( property_list, property_list::base, property_list_map, ARRAYNUMBER( property_list_map ) )



// Construct a property_list
property_list::property_list( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h )
				: base( p, NULL, ID_LIST, opts, x, y, w, h ), current_item_( NULL )
{
	setTarget( this );

	// true false の選択用
	boolean_combo_ = new FXComboBox( this, 1, this, ID_COMBO, COMBOBOX_STATIC | LAYOUT_FIX_X | LAYOUT_FIX_Y | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 0, 5, 6, 2, 0 );
	boolean_combo_->appendItem( "false" );
	boolean_combo_->appendItem( "true" );
	boolean_combo_->setNumVisible( 2 );

	// 文字列・数値を入力用
	text_input_ = new FXTextField( this, 255, this, ID_TEXT, FRAME_NONE | LAYOUT_FIX_X | LAYOUT_FIX_Y | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 0, 5, 6, 2, 0 );

	// ファイル名を入力用
	filename_area_ = new FXHorizontalFrame( this, FRAME_NONE | LAYOUT_FIX_X | LAYOUT_FIX_Y | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	filename_ = new FXTextField( filename_area_, 255, this, ID_FILENAME, FRAME_NONE | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 5, 6, 2, 0 );
	new FXButton( filename_area_, "...", NULL, this, ID_BUTTON, BUTTON_NORMAL | LAYOUT_SIDE_RIGHT | LAYOUT_FILL_Y );

}

property_list::~property_list( )
{
}


void property_list::create( )
{
	boolean_combo_->setFont( getFont( ) );
	text_input_->setFont( getFont( ) );

	boolean_combo_->hide( );
	text_input_->hide( );

	base::create( );
}


long property_list::onKeyDown( FXObject *obj, FXSelector sel, void *ptr )
{
	if( text_input_->shown( ) )
	{
		return( text_input_->handle( obj, FXSEL( SEL_KEYPRESS, 0 ), ptr ) );
	}
	else if( filename_->shown( ) )
	{
		return( filename_->handle( obj, FXSEL( SEL_KEYPRESS, 0 ), ptr ) );
	}
	else
	{
		return( 1 );
	}
}

long property_list::onKeyUp( FXObject *obj, FXSelector sel, void *ptr )
{
	if( text_input_->shown( ) )
	{
		return( text_input_->handle( obj, FXSEL( SEL_KEYRELEASE, 0 ), ptr ) );
	}
	else if( filename_->shown( ) )
	{
		return( filename_->handle( obj, FXSEL( SEL_KEYRELEASE, 0 ), ptr ) );
	}
	else
	{
		return( 1 );
	}
}

long property_list::onSelected( FXObject *obj, FXSelector sel, void *ptr )
{
	// 現在どこかで，入力欄が表示されている場合は，非表示にする
	if( current_item_ != NULL )
	{
		boolean_combo_->hide( );
		text_input_->hide( );
		filename_area_->hide( );
	}

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
	property *item = current_item_ = static_cast< property * >( getItemData( curItem ) );
	if( item != NULL )
	{
		switch( item->type )
		{
		case property::BOOLEAN:
			boolean_combo_->setCurrentItem( item->boolean );
			boolean_combo_->position( l, t, w, h );
			boolean_combo_->show( );
			break;

		case property::INTEGER:
			text_input_->setTextStyle( TEXTFIELD_INTEGER | TEXTFIELD_LIMITED );
			text_input_->setText( FXStringVal( item->integer ) );
			text_input_->position( l, t, w, h );
			text_input_->setCursorPos( text_input_->getText( ).length( ) );
			text_input_->selectAll( );
			text_input_->show( );
			text_input_->setFocus( );
			break;

		case property::REAL:
			text_input_->setTextStyle( TEXTFIELD_REAL | TEXTFIELD_LIMITED );
			text_input_->setText( FXStringVal( item->real ) );
			text_input_->position( l, t, w, h );
			text_input_->setCursorPos( text_input_->getText( ).length( ) );
			text_input_->selectAll( );
			text_input_->show( );
			text_input_->setFocus( );
			break;

		case property::TEXT:
			text_input_->setTextStyle( TEXTFIELD_LIMITED );
			text_input_->setText( item->text );
			text_input_->position( l, t, w, h );
			text_input_->setCursorPos( text_input_->getText( ).length( ) );
			text_input_->selectAll( );
			text_input_->show( );
			text_input_->setFocus( );
			break;

		case property::FILEOPEN:
		case property::FILESAVE:
			filename_->setTextStyle( TEXTFIELD_LIMITED );
			filename_->setText( item->text );
			filename_->setCursorPos( filename_->getText( ).length( ) );
			filename_->selectAll( );
			filename_area_->position( l, t, w, h );
			filename_area_->show( );
			filename_->setFocus( );
			break;

		default:
			break;
		}
	}

	return( 1 );
}


long property_list::onDeSelected( FXObject *obj, FXSelector sel, void *ptr )
{
	current_item_ = NULL;

	boolean_combo_->hide( );
	text_input_->hide( );
	filename_area_->hide( );

	return( 1 );
}

long property_list::onDataChanged( FXObject *obj, FXSelector sel, void *ptr )
{
	// 変更結果を反映する
	if( current_item_ != NULL )
	{
		switch( current_item_->type )
		{
		case property::BOOLEAN:
			current_item_->boolean = boolean_combo_->getCurrentItem( ) ? true : false;
			break;

		case property::INTEGER:
			current_item_->integer = FXIntVal( text_input_->getText( ) );
			break;

		case property::REAL:
			current_item_->real = FXDoubleVal( text_input_->getText( ) );
			break;

		case property::TEXT:
			current_item_->text = text_input_->getText( );
			break;

		case property::FILEOPEN:
		case property::FILESAVE:
			current_item_->text = filename_->getText( );
			break;

		default:
			break;
		}
	}

	return( 1 );
}


// Paint the canvas
long property_list::onPaint( FXObject *obj, FXSelector sel, void *ptr )
{
	FXEvent &e = *( ( FXEvent * )ptr );

	FXDCWindow dc( this, &e );

	FXFont *font = getFont( );
	dc.setFont( font );

	// キャンバスの範囲外にある領域を塗りつぶす
	dc.setForeground( FXRGB( 128, 128, 128 ) );
	dc.fillRectangle( 0, 0, getWidth( ), getHeight( ) );

	if( getNumItems( ) < 1 )
	{
		return( 1 );
	}

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
		property *item = static_cast< property * >( getItemData( i ) );
		if( item != NULL )
		{
			dc.setForeground( FXRGB( 0, 0, 0 ) );
			dc.setClipRectangle( l + itemW, t, winW - itemW, b - t );
			FXString text;
			switch( item->type )
			{
			case property::BOOLEAN:
				text = item->boolean ? "true": "false";
				break;

			case property::INTEGER:
				text = FXStringVal( item->integer );
				break;

			case property::REAL:
				text = FXStringVal( item->real );
				break;

			case property::TEXT:
			case property::FILEOPEN:
			case property::FILESAVE:
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

// Paint the canvas
long property_list::onFileOpen( FXObject *obj, FXSelector sel, void *ptr )
{
	// 変更結果を反映する
	if( current_item_ != NULL )
	{
		switch( current_item_->type )
		{
		case property::FILEOPEN:
			filename_->setText( FXFileDialog::getOpenFilename( this, "Open", "" ) );
			onDataChanged( obj, sel, ptr );
			break;

		case property::FILESAVE:
			filename_->setText( FXFileDialog::getSaveFilename( this, "Save", "" ) );
			onDataChanged( obj, sel, ptr );
			break;

		default:
			break;
		}
	}

	return( 1 );
}






