#ifndef __INCLUDE_PROPERTY__
#define __INCLUDE_PROPERTY__



struct property_data
{
	enum PROPERTY_TYPE
	{
		NONE,
		TEXT,
		BOOLEAN,
		INTEGER,
		REAL,
	};

	PROPERTY_TYPE	type;

	FXString	text;
	bool		boolean;
	FXint		integer;
	FXdouble	real;

	const property_data &operator =( const property_data &p )
	{
		if( &p != this )
		{
			type = p.type;
			text = p.text;
			boolean = p.boolean;
			integer = p.integer;
			real = p.real;
		}
		return( *this );
	}

	property_data( ) : type( NONE ), text( "" ), boolean( false ), integer( 0 ), real( 0.0 ){ }

	property_data( PROPERTY_TYPE t, const FXString &v ) : type( t ), text( v ), boolean( false ), integer( 0 ), real( 0.0 ){ }
	property_data( PROPERTY_TYPE t, const char *v ) : type( t ), text( v ), boolean( false ), integer( 0 ), real( 0.0 ){ }
	property_data( PROPERTY_TYPE t, bool v ) : type( t ), text( "" ), boolean( v ), integer( 0 ), real( 0.0 ){ }
	property_data( PROPERTY_TYPE t, FXint v ) : type( t ), text( "" ), boolean( false ), integer( v ), real( 0.0 ){ }
	property_data( PROPERTY_TYPE t, FXdouble v ) : type( t ), text( "" ), boolean( false ), integer( 0 ), real( v ){ }

	property_data( const property_data &p ) : type( p.type ), text( p.text ), boolean( p.boolean ), integer( p.integer ), real( p.real ){ }
};


// Event Handler Object
class property : public FXList
{
	// Macro for class hierarchy declarations
	FXDECLARE( property )

public:
	typedef FXList base;

protected:
	// アプリケーションオブジェクトを取得する
	mist_builder *getApp( ) const { return( reinterpret_cast< mist_builder * >( base::getApp( ) ) ); }
	mist_builder &getAppInstance( ) const { return( *getApp( ) ); }


public:
	property_data data[ 10 ];

	property_data	*current_item_;
	FXComboBox		*boolean_combo_;
	FXTextField		*text_input_;


protected:
	property( ){}
	property( const property& ){}


public:
	long onPaint( FXObject *obj, FXSelector sel, void *ptr );
	long onSelected( FXObject *obj, FXSelector sel, void *ptr );
	long onDeSelected( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyDown( FXObject *obj, FXSelector sel, void *ptr );
	long onKeyUp( FXObject *obj, FXSelector sel, void *ptr );

	virtual void create( );
	virtual FXbool  canFocus( ) const { return( true ); }
public:
	enum
	{
		ID_LIST = base::ID_LAST,
		ID_COMBO,
		ID_TEXT,
		ID_LAST
	};

public:
	property( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0 );

	virtual ~property( );
};



#endif	// __INCLUDE_PROPERTY__
