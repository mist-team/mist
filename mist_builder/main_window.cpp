#include "config.h"

#include "main_window.h"

#include <mist/io/image.h>

FXDEFMAP( main_window ) main_window_map[] =
{
	//________Message_Type______________________________ID_____________________________Message_Handler_______
	FXMAPFUNC( SEL_COMMAND,					main_window::ID_FILE_OPEN,			main_window::onFileOpen ),
	FXMAPFUNC( SEL_COMMAND,					main_window::ID_APPEND_FILTER,		main_window::onAppendFilter ),
	FXMAPFUNC( SEL_COMMAND,					main_window::ID_RECOMPUTE_FILTER,	main_window::onRecomputeFilter ),

	FXMAPFUNC( MIST_FILTER_CHANGED,			main_window::ID_FILTER_GRAPH,		main_window::onFilterChanged ),
	FXMAPFUNC( MIST_FILTER_IMAGE_CHANGED,	main_window::ID_FILTER_GRAPH,		main_window::onFilterImageChanged ),
};


FXIMPLEMENT( main_window, main_window::base, main_window_map, ARRAYNUMBER( main_window_map ) )


const static FXuint __frame_style__ = FRAME_NONE | FRAME_SUNKEN | FRAME_RAISED | FRAME_THICK | FRAME_GROOVE | FRAME_RIDGE | FRAME_LINE | FRAME_NORMAL;


struct progress_callback
{
	FXProgressBar *f_;
	bool operator()( double percent ) const
	{
		if( percent > 100.0 )
		{
			f_->hide( );
			f_->getParent( )->layout( );
			return( true );
		}
		else if( !f_->shown( ) )
		{
			f_->show( );
			f_->getParent( )->layout( );
		}
		f_->setProgress( static_cast< FXuint >( percent ) );
		f_->update( );
		return( true );
	}

	progress_callback( FXProgressBar *f ) : f_( f ){ }
};


// Construct a main_window
main_window::main_window( FXApp *a ) : base( a, "main_window", NULL, NULL, DECOR_ALL, 100, 100, 800, 600 )
//main_window::main_window( FXApp *a ) : base( a, "main_window", NULL, NULL, DECOR_ALL, 0, 0, 600, 450 )
{
	FXPacker *client = new FXPacker( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0 );

	// Menubar
	menubar_ = new FXMenuBar( client, LAYOUT_SIDE_TOP | LAYOUT_FILL_X );

	// Separator
	//new FXHorizontalSeparator( client, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | SEPARATOR_GROOVE );

	// File Menu
	filemenu_ = new FXMenuPane( this );
	new FXMenuCommand( filemenu_, "開く(&O)\tCtl+O", NULL, this, main_window::ID_FILE_OPEN, 0 );
	new FXMenuCommand( filemenu_, "&Quit\tCtl+Q", NULL, getApp( ), FXApp::ID_QUIT, 0 );
	new FXMenuTitle( menubar_, "ファイル(&F)", NULL, filemenu_ );

	statusbar_ = new FXStatusBar( client, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | LAYOUT_FIX_HEIGHT | STATUSBAR_WITH_DRAGCORNER, 0, 0, 0, 20 );
	{
		// ステータスバーの文字表示ペインのフレームをなくす
		statusbar_->getStatusLine( )->setFrameStyle( ( statusbar_->getStatusLine( )->getFrameStyle( ) & ~__frame_style__ ) | FRAME_NONE );
		statusbar_->getStatusLine( )->setNormalText( "時よ止まれ" );

		// ステータスバーに区画を設定
		FXHorizontalFrame *o = new FXHorizontalFrame( statusbar_, LAYOUT_SIDE_RIGHT, 0, 0, 0, 0, 0, 0, 0, 0/*, 0, 0*/ );
		{
			// プログレスバー
			progressbar_ = new FXProgressBar( o, 0, 0, LAYOUT_SIDE_LEFT | LAYOUT_FILL_Y | LAYOUT_FIX_WIDTH | PROGRESSBAR_HORIZONTAL | PROGRESSBAR_PERCENTAGE | FRAME_SUNKEN, 0, 0, 200, 0 );
			progressbar_->setTotal( 100 );
		}
	}

	// 一番下のステータスバーの上にセパレータを描画する
	//new FXHorizontalSeparator( client, SEPARATOR_GROOVE | LAYOUT_FILL_X | LAYOUT_SIDE_BOTTOM );

	FXSplitter *o = new FXSplitter( client, SPLITTER_NORMAL | SPLITTER_REVERSED | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	{
		FXPacker *p = new FXPacker( o, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
		filter_graph_ = new filter_graph( p, this, ID_FILTER_GRAPH, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0 );

		FXSplitter *v1 = new FXSplitter( o, SPLITTER_VERTICAL | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 240, 0 );
		{
			data_view_ = new data_view( v1, this, ID_DATA_VIEW, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 240, 0, 0, 0, 0, 0, 0 );

			FXVerticalFrame *v2 = new FXVerticalFrame( v1, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
			{
				FXGroupBox *g1 = new FXGroupBox( v2, "Filter List", GROUPBOX_NORMAL | FRAME_THICK | LAYOUT_FILL_X );
				new FXButton( g1, "Add", 0, this, ID_APPEND_FILTER, BUTTON_NORMAL | LAYOUT_SIDE_RIGHT );
				listbox_ = new FXListBox( g1, this, ID_FILTER_LIST, FRAME_LINE | LAYOUT_FILL_X );
				listbox_->setNumVisible( 5 );

				FXGroupBox *g2 = new FXGroupBox( v2, "Filter Property", GROUPBOX_NORMAL | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_FILL_Y );
				new FXButton( g2, "Recompute", 0, this, ID_RECOMPUTE_FILTER, BUTTON_NORMAL | LAYOUT_FILL_X );
				new FXHorizontalSeparator( g2 );
				new property( g2, 0, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0 );
			}
		}
	}
}

main_window::~main_window( )
{
	ClearFilters( );
}

// Create and initialize
void main_window::create( )
{
	// Create the windows
	base::create( );

	progressbar_->setBarColor( FXRGB( 66, 124, 206 ) );
	progressbar_->setBackColor( getApp( )->getBaseColor( ) );

	progressbar_->hide( );

	// フィルタリストを初期化する
	EnumFilters( );

	// Make the main window appear
	show( );
}



long main_window::onFileOpen( FXObject *obj, FXSelector sel, void *ptr )
{
	//FXString filename = FXFileDialog::getOpenFilename( this, "画像を開く", "" );

	//if( filename != "" )
	//{
	//	data_type image;
	//	mist::read_image( image, filename.text( ) );
	//	data_view_->draw_image( image );
	//}

	return( 1 );
}


long main_window::onAppendFilter( FXObject *obj, FXSelector sel, void *ptr )
{
	size_type indx = listbox_->getCurrentItem( );

	mist_builder &app = getAppInstance( );

	if( indx < 0 || indx >= app.filters.size( ) )
	{
		return( 0 );
	}

	filter_graph_->append_filter( *( app.filters[ indx ] ) );

	return( 1 );
}


long main_window::onRecomputeFilter( FXObject *obj, FXSelector sel, void *ptr )
{
	filter_graph_->recompute_current_filter( );

	return( 1 );
}


long main_window::onFilterChanged( FXObject *obj, FXSelector sel, void *ptr )
{
	filter *f = static_cast< filter * >( ptr );

	if( f != NULL )
	{
		data_view_->draw_image( f );
	}

	return( 1 );
}


long main_window::onFilterImageChanged( FXObject *obj, FXSelector sel, void *ptr )
{
	filter *f = static_cast< filter * >( ptr );

	if( f != NULL )
	{
		data_view_->draw_image( f );
	}

	return( 1 );
}



