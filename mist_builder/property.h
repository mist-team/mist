#ifndef __INCLUDE_PROPERTY__
#define __INCLUDE_PROPERTY__


// Event Handler Object
class property : public FXPacker
{
	// Macro for class hierarchy declarations
	FXDECLARE( property )

public:
	typedef FXPacker base;

protected:
	FXTable		*table_;

	// アプリケーションオブジェクトを取得する
	mist_builder *getApp( ) const { return( reinterpret_cast< mist_builder * >( base::getApp( ) ) ); }
	mist_builder &getAppInstance( ) const { return( *getApp( ) ); }


public:


protected:
	property( ){}
	property( const property& ){}
	property( FXComposite *p, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
				FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
				FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
				FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING ){}


public:
	long onInterpolateChange( FXObject *obj, FXSelector sel, void *ptr );

	virtual void create( );

public:
	enum
	{
		ID_TABLE = base::ID_LAST,
		ID_LAST
	};

public:
	property( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0,
				FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
				FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
				FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
				FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING );

	virtual ~property( );
};



#endif	// __INCLUDE_PROPERTY__
