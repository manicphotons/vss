/*─────────────────────────────────────────────────────────────────────────────
 * PENDING TASKS
 *
 * Check for and correctly handle errors when using xlib.
 * Set all relavent window properties (WM_NAME etc…) for window manager usage.
 * Load/save settings from/to a config file.
 * Create vulkan interface.
 * Handle window resizing.
 * Enable both fullscreen and windowed modes.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>

#define IGNORE_INPUT( Arg ) ((void)Arg);

void PrintError( char* Message ){
	fprintf( stderr, "ERROR:\n\t%s\n\tClosing…\n", Message );
}

int
main( int ArgCount, char** ArgValues ){
	IGNORE_INPUT( ArgCount  );
	IGNORE_INPUT( ArgValues );

	Display* DefaultDisplay = XOpenDisplay( NULL );
	if( !DefaultDisplay ){
		PrintError( "Xlib: Could not open the default display." );
		return EXIT_FAILURE;
	}

	int RootWindow = DefaultRootWindow( DefaultDisplay );
	int DefaultScreen = DefaultScreen( DefaultDisplay );

	int32_t ScreenBitDepth = 24;
	XVisualInfo VisualInfo = {};
	if( !XMatchVisualInfo( DefaultDisplay, DefaultScreen, ScreenBitDepth, TrueColor, &VisualInfo) ){
		PrintError( "Xlib: No matching visual info." );
		return EXIT_FAILURE;
	}

	int32_t WindowPosition[2] = {0, 0};
	int32_t WindowDimensions[2] = {800, 600};
	XSetWindowAttributes WindowAttributes;
	WindowAttributes.background_pixel = 0;
	WindowAttributes.colormap = XCreateColormap( DefaultDisplay, RootWindow, VisualInfo.visual, AllocNone );
	WindowAttributes.event_mask = StructureNotifyMask;
	unsigned long AttributeMask = CWBackPixel | CWColormap | CWEventMask;
	Window WindowID = XCreateWindow(
		DefaultDisplay, RootWindow,
		WindowPosition[0], WindowPosition[1],
		WindowDimensions[0], WindowDimensions[1], 0,
		VisualInfo.depth, InputOutput,
		VisualInfo.visual, AttributeMask, &WindowAttributes
	);

	if( !WindowID ){
		PrintError( "Xlib: Window wasn't created properly" );
		return EXIT_FAILURE;
	}

	XStoreName( DefaultDisplay, WindowID, "Hello, World!" );

	Atom DeleteWindowProtocol = XInternAtom( DefaultDisplay, "WM_DELETE_WINDOW", False);
	if( !XSetWMProtocols( DefaultDisplay, WindowID, &DeleteWindowProtocol, 1 ) ){
		PrintError( "Xlib: Delete window protocol wasn't set properly" );
		return EXIT_FAILURE;
	}

	XMapWindow( DefaultDisplay, WindowID );
	XFlush( DefaultDisplay );

	int WindowIsOpen = 1;
	while( WindowIsOpen ){
		XEvent Event = {};
		while( XPending( DefaultDisplay ) > 0 ){
			XNextEvent( DefaultDisplay, &Event );
			switch( Event.type ){
				case ClientMessage: {
					if( Event.xclient.data.l[0] == DeleteWindowProtocol ) {
						XDestroyWindow( DefaultDisplay, WindowID );
					}
				}
				case DestroyNotify: {
					XDestroyWindowEvent* e = (XDestroyWindowEvent*)&Event;
					if( e->window == WindowID ){
						WindowIsOpen = 0;
            				}
				} break;
        		}
		}
	};

	XCloseDisplay( DefaultDisplay );
	DefaultDisplay = NULL;

	return EXIT_SUCCESS;
}
