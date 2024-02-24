/*
linux/XException.cpp
--------------------
Copyright (c) 2024, theJ89

Description:
    See XException.hpp for more information.
*/




//Includes
#include "XException.hpp"   //Header file




namespace {
    //X Error Handler prototype
    typedef int (*XErrorHandler)( Display*, XErrorEvent* );

    //An X11 error handler that sets a global code to something other than 0 if an error occurs.
    //This handler is used temporarily between calls to xerrBegin() and xerrEnd() X11-related functions we want to detect errors in.
    //HACK: Not threadsafe. This is a functional approach suggested by the tutorial.
    //There is likely a better solution, but this will do for now.
    XErrorHandler xerrOldHandler  = nullptr;
    Display*      xerrDisplay     = nullptr;
    int           xerrCode        = 0;
    int xerrHandler( Display* display, XErrorEvent* event ) {
        xerrDisplay = event->display;
        xerrCode    = event->error_code;
        return 0;
    }
}

namespace Brimstone {
namespace Private {

/*
XException::XException
-----------------------

Description:
    XException constructor.

Arguments:
    display:                The X11 display the error occurred on
    code:                   The error code.

Returns:
    N/A
*/
XException::XException( Display* const display, const int code ) :
    m_display( display ),
    m_code( code ) {
}

/*
XException::getDisplay
-----------------------

Description:
    Returns the display the error occurred on.

Arguments:
    N/A

Returns:
    Display*:   The display.
*/
Display* XException::getDisplay() const {
    return m_display;
}

/*
XException::getCode
-----------------------

Description:
    Returns the error code.

    Note: Taken from http://tronche.com/gui/x/xlib/event-handling/protocol-errors/default-handlers.html
    Can be one of the following X11 enums:
    * Success
    * BadAccess
    * BadAlloc
    * BadAtom
    * BadColor
    * BadCursor
    * BadDrawable
    * BadFont
    * BadGC
    * BadIDChoice
    * BadImplementation
    * BadLength
    * BadMatch
    * BadName
    * BadPixmap
    * BadRequest
    * BadValue
    * BadWindow

Arguments:
    N/A

Returns:
    int:    The code.
*/
int XException::getCode() const {
    return m_code;
}

/*
XException::getDescription
-----------------------

Description:
    Returns a textual description of the error that occurred.

Arguments:
    N/A

Returns:
    ustring:    The message.
*/
ustring XException::getDescription() const {
    static const int BUFFER_SIZE = 1024;
    ustring msg( BUFFER_SIZE, '\0' );

    //Grab a textual description of the error that occurred from X11.
    //HACK: Using string's internal buffer directly...
    XGetErrorText( m_display, m_code, &msg[0], BUFFER_SIZE );

    //NOTE: the string's internal buffer is (at least) BUFFER_SIZE+1 characters long.
    //The final character is a null character, so we don't have to worry about null terminating the string.

    return msg;
}

/*
xerrBegin
-----------------------

Description:
    Temporarily sets the X11 Error Handler to an internal handler that records the codes of any errors that may occur.
    Stores the previous error handler so it can be restored later.

    Every call to xerrBegin() should have a matching call to xerrEnd().
    For example:
        xerrBegin();
        //Call to some function that can generate X11 errors...
        xerrEnd();

    NOTE:
        I don't recommend calling multiple functions that can generate X11 errors between calls to xerrBegin() and xerrEnd().
        Only the code of the most recent error is stored; if multiple errors occur, only the code of the most recent error will be recorded.

Arguments:
    N/A

Returns:
    void:       N/A

Throws:
    Exception:  If xerrBegin() is called between calls to xerrBegin() and xerrEnd().
*/
void xerrBegin() {
    //xerrBegin(); xerrBegin(); is not allowed
    if( xerrOldHandler != nullptr )
        throw Exception( "xerrBegin() has already been called." );

    //NOTE: Success = 0; i.e. no error
    xerrDisplay = nullptr;
    xerrCode = 0;
    xerrOldHandler = XSetErrorHandler( &xerrHandler );
}

/*
xerrEnd
-----------------------

Description:
    Restores the previous X11 Error Handler.

    Every call to xerrBegin() should have a matching call to xerrEnd().
    For example:
        xerrBegin();
        //Call to some function that can generate X11 errors...
        xerrEnd();
Arguments:
    N/A

Returns:
    void:       N/A

Throws:
    Exception:  If a matching call to xerrBegin() was not made.
                If the X error handler was manually set after calling xerrBegin().
*/
void xerrEnd() {
    //Can't call xerrEnd() unless xerrBegin() has been called first
    if( xerrOldHandler == nullptr )
        throw Exception( "xerrBegin() not called before matching xerrEnd()." );

    //Reset the error handler to what it was before the xerrBegin() call
    XErrorHandler previous = XSetErrorHandler( xerrOldHandler );

    //Clear the reference to the old handler.
    xerrOldHandler = nullptr;

    //Check to make sure the X Error Handler wasn't changed between calls to xerrBegin() and xerrEnd()
    //NOTE: If the error handler changed midway, should I restore THAT error handler, or the one from before xerrBegin?
    if( previous != xerrHandler ) {
        XSetErrorHandler( previous );
        throw Exception( "X11 error handler was changed between xerrBegin() and xerrEnd()." );
    }


}

/*
Returns true if an error occurred between calls to xerrBegin() and xerrEnd(), false otherwise
*/
bool xerrExists() {
    return xerrCode != 0;
}

/*
Returns an XException containing the display and error code of the most recent X11 error
captured between calls to xerrBegin() and xerrEnd().

Call xerrExists() first to confirm that an error is available.
*/
XException xerrGet() {
    return XException( xerrDisplay, xerrCode );
}

}
}
