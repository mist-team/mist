#ifndef __INCLUDE_PROPERTY_LIST__
#define __INCLUDE_PROPERTY_LIST__




// Event Handler Object
class property_list : protected FXList
{
	// Macro for class hierarchy declarations
	FXDECLARE( property_list )

public:
	typedef FXList base;

protected:
	// アプリケーションオブジェクトを取得する
	mist_builder *getApp( ) const { return( reinterpret_cast< mist_builder * >( base::getApp( ) ) ); }
	mist_builder &getAppInstance( ) const { return( *getApp( ) ); }


protected:
	property			*current_item_;
	FXComboBox			*boolean_combo_;
	FXTextField			*text_input_;
	FXHorizontalFrame	*filename_area_;
	FXTextField			*filename_;

public:
	FXint appendItem( property &p )
	{
		return( base::appendItem( p.label, NULL, &p ) );
	}

	void clearItems( FXbool notify = FALSE )
	{
		onDeSelected( NULL, 0, NULL );
		base::clearItems( notify );
	}

protected:
	property_list( ){}
	property_list( const property_list& ){}


public:
	long onPaint( FXObject *obj, FXSelector sel, void *ptr );
	long onSelected( FXObject *obj, FXSelector sel, void *ptr );
	long onDeSelected( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyDown( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyUp( FXObject *obj, FXSelector sel, void *ptr );

	long onFileOpen( FXObject *obj, FXSelector sel, void *ptr );

	long onDataChanged( FXObject *obj, FXSelector sel, void *ptr );

	virtual void create( );
	virtual FXbool  canFocus( ) const { return( true ); }

public:
	enum
	{
		ID_LIST = base::ID_LAST,
		ID_COMBO,
		ID_TEXT,
		ID_FILENAME,
		ID_BUTTON,
		ID_LAST
	};

public:
	property_list( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0 );

	virtual ~property_list( );
};



#endif	// __INCLUDE_PROPERTY_LIST__
