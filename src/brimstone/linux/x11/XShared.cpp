/*
linux/x11/XShared.cpp
---------------------
Copyright (c) 2024, theJ89

Description:
    See XShared.hpp for more information.
*/




//Includes
#include "XShared.hpp"                   //Class header
#include "XPixmap.hpp"                   //Brimstone::Private::XPixmap
#include "../opengl/LinuxGLContext.hpp"  //LinuxGLContext::getIdealVisualInfo (TEMP?)




namespace Brimstone {
namespace Private {




//Static class variables
//BUG:
//    The order in which static storage duration objects in different compilation units are constructed and destructed isn't easy to control. This is causing a problem where objects being
//    depended upon are destroyed *before* the objects that depend on them. For example, currently in my test application Brimstone's static XInputMethod XShared::m_inputMethod is being
//    destroyed first, followed by the test application's global Window object containing an XInputContext that depends on that XInputMethod, which results in a segfault.
//TODO:
//    Redesign the program architecture to avoid this problem.
bool         XShared::m_initialized                 = false;
XDisplay     XShared::m_display;
XVisualInfo  XShared::m_vi;
int          XShared::m_screen                      = 0;
::Window     XShared::m_rootWindow                  = None;
XColormap    XShared::m_colormap;
XInputMethod XShared::m_inputMethod;
XCursor      XShared::m_blankCursor;
::Atom       XShared::m_atomWmDeleteWindow          = None;
::Atom       XShared::m_atomNetWmName               = None;
::Atom       XShared::m_atomNetWmState              = None;
::Atom       XShared::m_atomNetWmStateFullscreen    = None;
::Atom       XShared::m_atomNetWmStateMaximizedVert = None;
::Atom       XShared::m_atomNetWmStateMaximizedHorz = None;
::Atom       XShared::m_atomNetWmStateHidden        = None;
::Atom       XShared::m_atomNetWmStateShaded        = None;
::Atom       XShared::m_atomNetFrameExtents         = None;
::Atom       XShared::m_atomMotifWmHints            = None;




/*
XShared::init
-------------

Description:
    Performs one-time initialization of shared X11 resources.

Arguments:
    N/A

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XShared::init() {
    //Already initialized; nothing to do:
    if( m_initialized )
        return;

    //Open a display:
    ::Display* display = m_display.create();

    //Ask GLX for info about the ideal visual for our windows:
    //NOTE:
    //    It kind of irks me that we need to interact with OpenGL prior to creating a window it can use.
    //    Is there something better we can do here?
    m_vi = LinuxGLContext::getIdealVisualInfo( display );
    ::XVisualInfo* vi = m_vi.get();

    //Set screen:
    m_screen = vi->screen;

    //Set root window:
    m_rootWindow = RootWindow( display, m_screen );

    //Create the colormap:
    m_colormap.create( m_rootWindow, vi->visual, AllocNone );

    //Create an input method:
    m_inputMethod.create();

    //Create a blank cursor:
    createBlankCursor();

    //Look up various Atoms we use:
    //NOTE:
    //    An Atom is basically just an association of a unique name (e.g. "SOME_ATOM_NAME") with a unique ID number (e.g. 12345).
    //    XInternAtom() takes an Atom's name and gives you its ID number, and XGetAtomName() takes an Atom's ID number and gives you its name.
    //    XInternAtom() can return None if the third parameter (only_if_exists) is True, therefore we set it to False to ensure the atoms are always created if they didn't exist beforehand.
    //NOTE:
    //    You can run xlsatoms ("X list atoms") in a terminal to get registered atoms.
    m_atomWmDeleteWindow          = XInternAtom( display, "WM_DELETE_WINDOW",             False );
    m_atomNetWmName               = XInternAtom( display, "_NET_WM_NAME",                 False );
    m_atomNetWmState              = XInternAtom( display, "_NET_WM_STATE",                False );
    m_atomNetWmStateFullscreen    = XInternAtom( display, "_NET_WM_STATE_FULLSCREEN",     False );
    m_atomNetWmStateMaximizedVert = XInternAtom( display, "_NET_WM_STATE_MAXIMIZED_VERT", False );
    m_atomNetWmStateMaximizedHorz = XInternAtom( display, "_NET_WM_STATE_MAXIMIZED_HORZ", False );
    m_atomNetWmStateHidden        = XInternAtom( display, "_NET_WM_STATE_HIDDEN",         False );
    m_atomNetWmStateShaded        = XInternAtom( display, "_NET_WM_STATE_SHADED",         False );
    m_atomNetFrameExtents         = XInternAtom( display, "_NET_FRAME_EXTENTS",           False );
    m_atomMotifWmHints            = XInternAtom( display, "_MOTIF_WM_HINTS",              False );

    //Finished initializing XShared:
    m_initialized = true;
}

/*
XShared::destroy
----------------

Description:
    Cleans up shared X11 resources.

Arguments:
    N/A

Returns:
    N/A
*/
void XShared::destroy() {
    //Not initialized; nothing to do:
    if( !m_initialized )
        return;

    //Reset atoms:
    m_atomMotifWmHints            = None;
    m_atomNetFrameExtents         = None;
    m_atomNetWmStateShaded        = None;
    m_atomNetWmStateHidden        = None;
    m_atomNetWmStateMaximizedHorz = None;
    m_atomNetWmStateMaximizedVert = None;
    m_atomNetWmStateFullscreen    = None;
    m_atomNetWmState              = None;
    m_atomNetWmName               = None;
    m_atomWmDeleteWindow          = None;

    //Destroy blank cursor:
    m_blankCursor.destroy();

    //Close the input method:
    m_inputMethod.destroy();

    //Destroy the colormap:
    m_colormap.destroy();

    //Reset root window reference:
    m_rootWindow = None;

    //Reset screen number:
    m_screen = 0;

    //Destroy the visual info:
    m_vi.destroy();

    //Close the display:
    m_display.destroy();

    //XShared no longer initialized:
    m_initialized = false;
}

/*
XShared::createBlankCursor
--------------------------

Description:
    Creates a blank cursor, which is used to hide the cursor when XWindow::setCursorVisible( false ) is called.

Arguments:
    N/A

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XShared::createBlankCursor() {
    //Create the pixmap we'll be using both for the source and the mask of the cursor we're creating.
    //This is a 1x1 pixmap, with data initialized to 0.
    char data[1] = { 0 };
    XPixmap xpm( m_rootWindow, data, 1, 1 );
    ::Pixmap pixmap = xpm.get();

    //Define the color used for both the foreground and background color of the cursor.
    //NOTE:
    //    Only the red, green, and blue values of the XColor structure are actually used by XCreatePixmapCursor(), so we can just set those and leave the rest of the members uninitialized.
    //    We set the color to black here, but the color itself isn't really that important since it gets masked out anyway.
    XColor color;
    color.red   = 0;
    color.green = 0;
    color.blue  = 0;

    //Create the cursor:
    //NOTE:
    //    The cursor's source pixmap specifies which pixels in the cursor should use the foreground color / and which pixels should use the background color.
    //    The cursor's mask pixmap specifies which pixels in the cursor should be visible and which ones shouldn't be.
    //    When we use the pixmap we previously defined for both the source and mask of a cursor, the result is a 1x1 pixel cursor whose only pixel would use the cursor's background color if it
    //    wasn't also invisible.
    m_blankCursor.create( pixmap, pixmap, &color, &color, 0, 0 );
}




}
}
