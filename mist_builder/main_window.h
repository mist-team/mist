#ifndef __INCLUDE_DIO__
#define __INCLUDE_DIO__

#include "filter_graph.h"
#include "data_view.h"

// Event Handler Object
class main_window : public FXMainWindow
{
	// Macro for class hierarchy declarations
	FXDECLARE( main_window )

public:
	typedef FXMainWindow base;

private:
	FXMenuBar		*menubar_;
	FXMenuPane		*filemenu_;

	filter_graph	*filter_graph_;
	data_view		*data_view_;

	FXListBox		*listbox_;
	FXStatusBar		*statusbar_;
	FXProgressBar	*progressbar_;

	// DIOのアプリケーションオブジェクトを取得する
	mist_builder *getApp( ) const { return( reinterpret_cast< mist_builder * >( base::getApp( ) ) ); }
	mist_builder &getAppInstance( ) const { return( *getApp( ) ); }

	void EnumFilters( );
	void ClearFilters( );

protected:
	main_window( ){}

public:
	long onFileOpen( FXObject *obj, FXSelector sel, void *ptr );
	long onAppendFilter( FXObject *obj, FXSelector sel, void *ptr );
	long onFilterChanged( FXObject *obj, FXSelector sel, void *ptr );
	long onFilterImageChanged( FXObject *obj, FXSelector sel, void *ptr );
	long onRecomputeFilter( FXObject *obj, FXSelector sel, void *ptr );

	virtual void create( );


public:
	enum
	{
		ID_FILE_OPEN = base::ID_LAST,
		ID_FILTER_GRAPH,
		ID_DATA_VIEW,
		ID_FILTER_LIST,
		ID_APPEND_FILTER,
		ID_RECOMPUTE_FILTER,
		ID_LAST
	};

public:
	main_window( FXApp* a );
	virtual ~main_window( );
};


#endif	// __INCLUDE_DIO__
