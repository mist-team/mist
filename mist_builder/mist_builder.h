#ifndef __INCLUDE_DIO_APP__
#define __INCLUDE_DIO_APP__


// Event Handler Object
class mist_builder : public FXApp
{
	// Macro for class hierarchy declarations
	FXDECLARE( mist_builder )

public:
	typedef FXApp base;

public:
	// アプリケーションで扱うフィルタ群
	filter_list filters;

	std::vector< FXCursor * > cursors;

	virtual void create( );

public:
	mist_builder( const FXString &name = "Application", const FXString &vendor = "FoxDefault" );
	virtual ~mist_builder( );

};


#endif	// __INCLUDE_DIO_APP__
