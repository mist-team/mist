#include "config.h"

#include "mist_builder.h"


//FXDEFMAP( dio_app ) dio_app_map[] =
//{
//	//________Message_Type_____________________ID_______________Message_Handler_______
//	//FXMAPFUNC( SEL_PAINT,				dio_app::ID_CANVAS,				dio_app::onPaint ),
//	FXMAPFUNC( SEL_COMMAND,				dio_app::ID_FILE_OPEN,			dio_app::onFileOpen ),
//	FXMAPFUNC( SEL_COMMAND,				dio_app::ID_SHOW_CONTROLLER,		dio_app::onShowController ),
//};

//FXIMPLEMENT( dio_app, dio_app::base, dio_app_map, ARRAYNUMBER( dio_app_map ) )
FXIMPLEMENT( mist_builder, mist_builder::base, 0, 0 )



// Construct a dio_app
mist_builder::mist_builder( const FXString &name, const FXString &vendor ) : base( name, vendor )
{
}


