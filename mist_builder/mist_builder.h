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

	// 画像等を描画する際に必要となるグローバルな描画パラメータ

public:
	mist_builder( const FXString &name = "Application", const FXString &vendor = "FoxDefault" );

};


#endif	// __INCLUDE_DIO_APP__
