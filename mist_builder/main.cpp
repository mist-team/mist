#include "config.h"

#include "main_window.h"

int main( int argc, char *argv[] )
{
	// Make application
	mist_builder *application = new mist_builder( "Dio", "daibo lab" );

	// Start app
	application->init( argc, argv );

	// Scribble window
	new main_window( application );

	// Create the application's windows
	application->create( );

	// Run the application
	application->run( );

	return( 0 );
}

