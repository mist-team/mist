#ifndef __INCLUDE_DATA_VIEW__
#define __INCLUDE_DATA_VIEW__


const static double zoom_minimum	= 1.0;
const static double zoom_maximum	= 20.0;
const static double zoom_step		= 0.25;



namespace FX
{
	enum
	{
		SLIDER_INVERT       = 0x00200000,
	};
}


// Event Handler Object
class invert_slider : public FXSlider
{
	// Macro for class hierarchy declarations
	FXDECLARE( invert_slider )

public:
	typedef FXSlider base;

protected:
	bool			invert_;

protected:
	invert_slider( ){}
	invert_slider( const invert_slider& ){}

public:
	/// Construct a slider widget
	invert_slider( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = SLIDER_NORMAL,
					FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
					FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
					FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING )
					: base( p, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb ), invert_( opts & SLIDER_INVERT ? true : false )
	{
	}


	/// Return slider value
	FXint getValue() const
	{
		if( invert_ )
		{
            return( range[ 1 ] - ( base::getValue( ) - range[ 0 ] ) );
		}
		else
		{
            return( base::getValue( ) );
		}
	}

	void setValue( FXint value )
	{
		if( invert_ )
		{
			base::setValue( range[ 1 ] - ( value - range[ 0 ] ) );
		}
		else
		{
            base::setValue( value );
		}
	}
};


// Event Handler Object
class data_view : public FXPacker
{
	// Macro for class hierarchy declarations
	FXDECLARE( data_view )

public:
	typedef FXPacker base;

protected:
	FXGLVisual		*glvisual_;
	FXGLCanvas		*canvas_;
	invert_slider	*zoom_slider_;

	std::vector< FXCursor * >	cursors_;
	std::vector< FXIcon * >		icons_;

	color_image buffer_;

	color_type background_color_;

	bool interpolate_;
	double zoom_;
	bool draw_cross_;

	point2 mark_position_;
	point2 current_position_;


	// アプリケーションオブジェクトを取得する
	mist_builder *getApp( ) const { return( reinterpret_cast< mist_builder * >( base::getApp( ) ) ); }
	mist_builder &getAppInstance( ) const { return( *getApp( ) ); }


public:
	point2 point2screen( const point2 &pt ) const;
	point2 screen2point( const point2 &pt ) const;

public:
	void draw_image( const data_type &buf );
	void draw_image( const filter *pf );
	void draw_image( );

	void set_cursors( bool is_drag );

protected:
	data_view( ){}
	data_view( const data_view& ){}
	data_view( FXComposite *p, FXuint opts = 0,
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

	long onInterpolateChange( FXObject *obj, FXSelector sel, void *ptr );
	long onDrawCrossChange( FXObject *obj, FXSelector sel, void *ptr );
	long onZoomChanged( FXObject *obj, FXSelector sel, void *ptr );

	virtual void create( );

public:
	enum
	{
		ID_CANVAS = base::ID_LAST,
		ID_DRAW_CROSS,
		ID_INTERPOLATE_MODE,
		ID_ZOOM_SLIDER,
		ID_LAST
	};

public:
	data_view( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
				FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
				FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
				FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING );

	virtual ~data_view( );
};



#endif	// __INCLUDE_DATA_VIEW__
