#ifndef __INCLUDE_FILTER_GRAPH__
#define __INCLUDE_FILTER_GRAPH__



// Event Handler Object
class filter_graph : public FXPacker
{
	// Macro for class hierarchy declarations
	FXDECLARE( filter_graph )

public:
	typedef FXPacker base;

protected:
	FXScrollBar		*hscrollbar_;
	FXScrollBar		*vscrollbar_;
	FXCanvas		*canvas_;
	FXFont			*font_;
	FXImage			*mem_image_;

	bool			damage_;
	filter			*current_filter_;
	pin				*current_pin_;
	filter_list		filters_;


	// アプリケーションオブジェクトを取得する
	mist_builder *getApp( ) const { return( reinterpret_cast< mist_builder * >( base::getApp( ) ) ); }
	mist_builder &getAppInstance( ) const { return( *getApp( ) ); }

	void initialize_filter( filter &filter );

	bool draw_filter( FXDC &dc, const filter &f );
	void draw_box( FXDC &dc, const FXint x, const FXint y, const FXint w, const FXint h, bool active );
	void draw_arrow( FXDC &dc, const FXint x1, const FXint y1, const FXint x2, const FXint y2, double r );

	bool apply_filters( filter &f );

	void SendUserMessage( UserMessage msg, void *ptr )
	{
		if( getTarget( ) != NULL )
		{
			getTarget( )->handle( this, FXSEL( msg, getSelector( ) ), ptr );
		}
	}

public:
	void append_filter( const filter &f );
	void recompute_current_filter( );

private:
	filter_graph( ){}
	filter_graph( const filter_graph& ){}
	filter_graph( FXComposite *p, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
				FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
				FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
				FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING ){}


public:
	long onPaint( FXObject *obj, FXSelector sel, void *ptr );
	long onMouseDown( FXObject *obj, FXSelector sel, void *ptr );
	long onMouseUp( FXObject *obj, FXSelector sel, void *ptr );
	long onMouseMove( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyDown( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyUp( FXObject *obj, FXSelector sel, void *ptr );

	long onResize( FXObject *obj, FXSelector sel, void *ptr );
	long onScroll( FXObject *obj, FXSelector sel, void *ptr );

	virtual void create( );

public:
	enum
	{
		ID_CANVAS = base::ID_LAST,
		ID_HSCROLLBAR,
		ID_VSCROLLBAR,
		ID_LAST
	};

public:
	filter_graph( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
				FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
				FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
				FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING );

	virtual ~filter_graph( );
};



#endif	// __INCLUDE_FILTER_GRAPH__
