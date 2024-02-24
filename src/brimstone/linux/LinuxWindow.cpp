/*
linux/LinuxWindow.cpp
---------------------
Copyright (c) 2024, theJ89
A lot of code in this file was taken from SFML (by Laurent Gomila)

Description:
    See LinuxWindow.hpp for more information.
*/




//Includes
#include "LinuxWindow.hpp"                      //Class header
#include "XException.hpp"                       //Brimstone::xerrBegin, Brimstone::xerrEnd
#include "../opengl/LinuxGLContext.hpp"         //TEMP? LinuxGLContext::getIdealVisualInfo

#include <brimstone/util/Range.hpp>             //Brimstone::clampedValue
#include <brimstone/Exception.hpp>              //Brimstone::NullPointerException
#include <brimstone/Logger.hpp>                 //Brimstone::logError

#include <boost/format.hpp>                     //boost::format




namespace {
    static const long event_mask = (
        ButtonPressMask     | ButtonReleaseMask   |
        PointerMotionMask   |
        KeyPressMask        | KeyReleaseMask      |
        FocusChangeMask     |
        KeymapStateMask     |
        StructureNotifyMask /*| ExposureMask*/
    );

    static const long valuemask = (
        CWBorderPixel | CWColormap | CWEventMask
    );
}
namespace Brimstone {
namespace Private {

std::mutex                      LinuxWindow::m_windowsMutex;
bool                            LinuxWindow::m_xInitialized = false;

Display*                        LinuxWindow::m_display      = nullptr;
LinuxWindow::XWinToWindowMap    LinuxWindow::m_windowMap;




LinuxWindow::LinuxWindow() :
    m_window( 0 ),
    m_screen( 0 ),
    m_colorMap( 0 ),
    m_inputMethod( 0 ),
    m_inputContext( 0 ),
    m_closeAtom( 0 ) {
}

LinuxWindow::~LinuxWindow() {
    try {
        close();
    } catch( const IException& e ) {
        uncaughtException( e );
    }
}

void LinuxWindow::open() {
    //Initialize X11 if we haven't already
    if( !m_xInitialized )
        initX();
    
    //Ask GLX for info about the ideal visual for our window.
    //NOTE: It kind of irks me that we need to interact with OpenGL prior to creating a window it can use.
    //Is there something better we can do here?
    {
        XVisualInfo* vi = LinuxGLContext::getIdealVisualInfo( m_display );
        std::unique_ptr< XVisualInfo, int(*)( void* ) > uptr( vi, &XFree );

        m_screen = vi->screen;
        
        //Find root window
        ::Window rootWindow = RootWindow( m_display, m_screen );

        //Create a colormap
        m_colorMap = XCreateColormap(
            m_display,
            rootWindow,
            vi->visual,
            AllocNone
        );

        //Set window attributes.
        //There's no need to zero out the entire structure - we only set the attributes we need to, then specify which values are set via the valuemask argument
        //specified alongside the attributes argument.
        //NOTE: The background_pixmap attribute is set in the example, but its corresponding mask bit isn't provided in the valuemask argument to XCreateWindow.
        //I commented it out because setting this appears to not make any difference.
        XSetWindowAttributes swa;
        swa.colormap     = m_colorMap;
        //swa.background_pixmap = None;
        swa.border_pixel = 0;
        swa.event_mask   = event_mask;

        //Create the window.
        m_window = XCreateWindow(
            m_display,
            rootWindow,
            m_bounds.mins.x,
            m_bounds.mins.y,
            m_bounds.getWidth(),
            m_bounds.getHeight(),
            5,
            vi->depth,
            InputOutput,
            vi->visual,
            valuemask,
            &swa
        );

        if( !m_window )
            throw NullPointerException();
    }

    //Set the window title
    XStoreName( m_display, m_window, m_title.c_str() );

    //Create an input method and context.
    //We need this to translate key press events to typed characters.
    m_inputMethod = XOpenIM( m_display, nullptr, nullptr, nullptr );
    if( m_inputMethod != nullptr ) {
        m_inputContext = XCreateIC(
            m_inputMethod,
            XNClientWindow, m_window,
            XNFocusWindow,  m_window,
            XNInputStyle,   XIMPreeditNothing | XIMStatusNothing,
            nullptr
        );
    } else {
        m_inputContext = nullptr;
    }

    //X doesn't add decorations to a created Window - the system's Window Manager is in charge of this.
    //One of these decorations added by the Window Manager happens to be the close button.
    //By default, the close button destroys the Window and closes the Display, bypassing the application
    //that created the Window completely. This will cause the program to crash, since it is still using
    //these resources. We need to take advantage of an extension to transfer this responsibility
    //from the Window Manager to our application so we can handle it appropriately.
    //By setting the WM_PROTOCOLS property to use the WM_DELETE_WINDOW atom, we're telling it to
    //send a ClientMessage event whenever the close button is clicked instead of doing its default behavior.
    //NOTE: XInternAtom can return None if the third parameter (only_if_exists) is True,
    //      therefore we set it to False to ensure the atom is always created.
    //NOTE: you can run xlsatoms ("X list atoms") in a terminal to get registered atoms
    m_closeAtom = XInternAtom( m_display, "WM_DELETE_WINDOW", False );
    if( m_closeAtom == None )
        throw Exception( "XInternAtom failed." );

    if( XSetWMProtocols( m_display, m_window, &m_closeAtom, 1 ) == 0 )
        throw Exception( "XSetWMProtocols failed." );

    //Add the window to the map
    {
        std::lock_guard< std::mutex > l( m_windowsMutex );
        m_windowMap.emplace( m_window, *this );
    }

    //Display the window to the user
    XMapWindow( m_display, m_window );
}

void LinuxWindow::close() {
    //Destroy the input context and method
    if( m_inputContext != nullptr ) {
        XDestroyIC( m_inputContext );
        m_inputContext = 0;
    }
    if( m_inputMethod != nullptr ) {
        XCloseIM( m_inputMethod );
        m_inputMethod = 0;
    }

    if( m_colorMap != 0 ) {
        XFreeColormap( m_display, m_colorMap );
        m_colorMap = 0;
    }

    //Destroy the window
    if( m_window != 0 ) {
        XDestroyWindow( m_display, m_window );

        //Remove the window from the map
        {
            std::lock_guard< std::mutex > l( m_windowsMutex );
            m_windowMap.erase( m_window );
        }

        //If this is the last window being closed, clean up shared X11 resources (display, etc)
        if( m_xInitialized && m_windowMap.empty() )
            destroyX();

        m_window = 0;
    }
}

bool LinuxWindow::isOpen() const {
    return m_window != 0;
}

bool LinuxWindow::peekEvent( WindowEvent& eventOut ) {
    //Window not open
    if( m_window == 0 )
        return false;

    //Nothing in event queue
    XEvent xEvent;
    if( m_eventQueue.empty() ) {
        //Process all messages currently in the application's message queue
        for( size_t i = XPending( m_display ); i > 0; --i ) {
            XNextEvent( m_display, &xEvent );
            mainProc( xEvent );
        }

        //Even after processing the messages in the queue, it's possible that:
        //1.) The message queue was empty
        //2.) No messages for this window were processed
        //So we need to check for these situations here.
        if( m_eventQueue.empty() )
            return false;
    }

    //Pop a message from the queue
    eventOut = m_eventQueue.front();
    m_eventQueue.pop();
    return true;
}

bool LinuxWindow::getEvent( WindowEvent& eventOut ) {
    //Window not open
    if( m_window == 0 )
        return false;

    //Wait for an event to arrive in the window's event queue if it's currently empty.
    //Process messages from the application's message queue, blocking until a message arrives if necessary.
    //The WindowImpl::getEvent() call returns true if a message was processed, or false if the application is shutting down.
    XEvent xEvent;
    while( m_eventQueue.empty() ) {
        XNextEvent( m_display, &xEvent );
        mainProc( xEvent );
    }

    //Pop a message from the queue
    eventOut = m_eventQueue.front();
    m_eventQueue.pop();
    return true;
}

void LinuxWindow::mainProc( XEvent& xEvent ) {
    //Locate what LinuxWindow this belongs to
    auto it = m_windowMap.find( xEvent.xany.window );
    if( it == m_windowMap.end() )
        return;

    it->second.windowProc( xEvent );
}

void LinuxWindow::windowProc( XEvent& xEvent ) {
    switch( xEvent.type ) {
        //Mouse move
        case MotionNotify: {
            auto p = getCursorPosFromXEvent( xEvent.xmotion );

            if( p != m_cursorPos ) {
                m_cursorPos = p;

                WindowEvent e;
                e.type        = WindowEventType::MouseMove;
                e.mouseMove.x = p.x;
                e.mouseMove.y = p.y;

                pushEvent( e );
            }
        } break;
        //Button down
        case ButtonPress: {
            int button = xEvent.xbutton.button;
            auto p = getCursorPosFromXEvent( xEvent.xbutton );

            //Vertical scrolling
            if( button == Button4 || button == Button5 ) {
                WindowEvent e;
                e.type               = WindowEventType::MouseVScroll;
                e.mouseScroll.scroll = button == Button4 ? 1.0f : -1.0f;
                e.mouseScroll.x      = p.x;
                e.mouseScroll.y      = p.y;

                pushEvent( e );
            //Horizontal scrolling (note: direction is assumed and needs to be tested somehow)
            } else if( button == 6 || button == 7 ) {
                WindowEvent e;
                e.type               = WindowEventType::MouseHScroll;
                e.mouseScroll.scroll = button == 6 ? -1.0f : 1.0f;
                e.mouseScroll.x      = p.x;
                e.mouseScroll.y      = p.y;

                pushEvent( e );
            //Actual buttons
            } else {
                WindowEvent e;
                e.type         = WindowEventType::MouseDown;
                e.mouse.button = xButtonToMouseButton( button );
                e.mouse.x      = p.x;
                e.mouse.y      = p.y;

                pushEvent( e );
            }
        } break;
        //Button up
        case ButtonRelease: {
            int button = xEvent.xbutton.button;

            //Ignore wheel scrolling. This is handled in the ButtonPress case above
            if( button == Button4 || button == Button5 || button == 6 || button == 7 )
                break;

            auto p = getCursorPosFromXEvent( xEvent.xbutton );

            WindowEvent e;
            e.type         = WindowEventType::MouseUp;
            e.mouse.button = xButtonToMouseButton( button );
            e.mouse.x      = p.x;
            e.mouse.y      = p.y;

            pushEvent( e );
        } break;
        //Key down / Character typed
        case KeyPress: {
            //The docs aren't too clear on the string returned by XLookupString.
            //I assumed it returns 1 encoding plus a null character, but this doesn't
            //seem to be the case for all keys.
            WindowEvent e2;
            e2.type = WindowEventType::Text;
            uchar* translatedCharacter = e2.text.utf8;

            KeySym keySym;

            //Note: we're only interested in the KeySym at this point.
            //The translated character is ignored
            XLookupString( &xEvent.xkey, translatedCharacter, 4, &keySym, nullptr );

            //Dispatch a keydown event
            WindowEvent e;
            e.type               = WindowEventType::KeyDown;
            e.key.key            = xKeySymToKey( keySym );

            pushEvent( e );

            //Why do I need to do this...?
            if( XFilterEvent( &xEvent, None ) ) {
                logInfo( "Filtered a keypress event" );
                break;
            }

            int count;
#ifdef X_HAVE_UTF8_STRING

            if( m_inputContext != nullptr ) {
                count = Xutf8LookupString( m_inputContext, &xEvent.xkey, translatedCharacter, 4, nullptr, nullptr );
            } else
#endif
            {
                count = XLookupString( &xEvent.xkey, translatedCharacter, 4, nullptr, nullptr );
            }

            //NOTE: Xutf8LookupString and XLookupString return a count of 0 when
            //trying to get the character for a non-character keycode (SHIFT, for instance).
            if( count == 0 )
                break;

            //A UTF-8 encoding of a single code point should never exceed 4 bytes in length
            if( count > 4 )
                throw UnexpectedResultException();

            //Null terminate the string (this is a problem with the strings for some keys, such as Numpad /)
            translatedCharacter[count] = 0;

            pushEvent( e2 );
        } break;
        //Key up
        case KeyRelease: {
            char translatedCharacter[5];
            KeySym keySym;
            XLookupString( &xEvent.xkey, translatedCharacter, 4, &keySym, nullptr );

            //Dispatch a KeyUp event
            WindowEvent e;
            e.type               = WindowEventType::KeyUp;
            e.key.key            = xKeySymToKey( keySym );

            pushEvent( e );
        } break;
        //Window moved and/or resized
        case ConfigureNotify: {
            //New position and size of the window
            Point2i pos( xEvent.xconfigure.x, xEvent.xconfigure.y );
            Size2i  size( xEvent.xconfigure.width, xEvent.xconfigure.height );

            //Moved
            //TODO: Apparently the coordinates that X gives are relative to the parent window.
            //I need to figure out how to use XTranslateCoordinates to get the actual position of the window onscreen.
            //Currently, when resizing we're getting a ConfigureNotify event where x and y are set to 0, followed by one using the actual window coordinates,
            //which is causing unnecessary Move events to be dispatched.
            //My best guess is maybe the window manager is reparenting the inner window each time it's resized? Either way, until I find a proper solution for
            //this problem this HACK of checking if the coordinates aren't 0 should suffice.
            if( !pos.isZero() and pos != m_bounds.getPosition() ) {
                //Update position
                m_bounds.setPosition( pos );

                //Push event
                WindowEvent e;
                e.type = WindowEventType::Move;
                e.move.x = pos.x;
                e.move.y = pos.y;

                pushEvent( e );
            }

            //Resized
            if( size != m_bounds.getSize() ) {
                //Update size
                m_bounds.setSize( size );

                //Push event
                WindowEvent e;
                e.type = WindowEventType::Resize;
                e.resize.w = size.w;
                e.resize.h = size.h;

                pushEvent( e );
            }
        } break;
        //Window gains keyboard focus
        case FocusIn: {
            WindowEvent e;
            e.type = WindowEventType::Focus;
            pushEvent( e );
        } break;
        //Window loses keyboard focus
        case FocusOut: {
            WindowEvent e;
            e.type = WindowEventType::Blur;
            pushEvent( e );
        } break;
        //Window close request
        case ClientMessage: {
            //Client messages are generic, so we need to determine for certain that we were sent a close request.
            //Specifically, the first data member should be the "WM_DELETE_WINDOW" atom.
            //The .data field can be interpreted as an array of 8-bit, 16-bit, or 32-bit values.
            //The .format field tells us how we should interpret the data.
            //Atoms are 32-bit unsigned ints, so we're expecting the message to contain 32-bit values.
            //This is good to check because another message could have a different size, but the same data in l[0].
            if( xEvent.xclient.format == 32 && (Atom)( xEvent.xclient.data.l[0] ) == m_closeAtom ) {
                WindowEvent e;
                e.type = WindowEventType::Close;

                pushEvent( e );
            }
        } break;
        //Events enabled by specifying StructureNotifyMask that we're not interested in
        case CirculateNotify:
        case DestroyNotify:
        case GravityNotify:
        case MapNotify:
        case UnmapNotify:
        case ReparentNotify:
            break;
        //Unhandled event
        default: {
            logError( ( boost::format( "Unhandled event: %1%" ) % xEvent.type ).str() );
        } break;
    }
}

Point2i LinuxWindow::getCursorPosFromXEvent( const XButtonEvent& xbutton ) {
    return Point2i( xbutton.x, xbutton.y );
}

Point2i LinuxWindow::getCursorPosFromXEvent( const XMotionEvent& xmotion ) {
    return Point2i( xmotion.x, xmotion.y );
}

MouseButton LinuxWindow::xButtonToMouseButton( const int button ) {
    switch( button ) {
    case Button1:
        return MouseButton::LEFT;
    case Button2:
        return MouseButton::MIDDLE;
    case Button3:
        return MouseButton::RIGHT;

    //No X11 constants are defined for the X1 / X2 buttons
    case 8:
        return MouseButton::X1;
    case 9:
        return MouseButton::X2;

    //The invalid button is returned if an unrecognized button is provided.
    default:
        logError( ( boost::format( "Unrecognized button: 0x%|04x|" ) % button ).str() );
        return MouseButton::INVALID;
    }
}

Key LinuxWindow::xKeySymToKey( const KeySym& keySym ) {
    KeySym lower, upper;
    XConvertCase( keySym, &lower, &upper );

    switch( lower ) {
    case XK_Escape:         return Key::ESCAPE;             //0xff1b
    case XK_F1:             return Key::F1;                 //0xffbe
    case XK_F2:             return Key::F2;                 //0xffbf
    case XK_F3:             return Key::F3;                 //0xffc0
    case XK_F4:             return Key::F4;                 //0xffc1
    case XK_F5:             return Key::F5;                 //0xffc2
    case XK_F6:             return Key::F6;                 //0xffc3
    case XK_F7:             return Key::F7;                 //0xffc4
    case XK_F8:             return Key::F8;                 //0xffc5
    case XK_F9:             return Key::F9;                 //0xffc6
    case XK_F10:            return Key::F10;                //0xffc7
    case XK_F11:            return Key::F11;                //0xffc8
    case XK_F12:            return Key::F12;                //0xffc9
    case XK_grave:          return Key::TILDE;              //0x0060
    case XK_asciitilde:     return Key::TILDE;              //0x007e
    case XK_0:              return Key::DIGIT_0;            //0x0030
    case XK_parenright:     return Key::DIGIT_0;            //0x0029
    case XK_1:              return Key::DIGIT_1;            //0x0031
    case XK_exclam:         return Key::DIGIT_1;            //0x0021
    case XK_2:              return Key::DIGIT_2;            //0x0032
    case XK_at:             return Key::DIGIT_2;            //0x0040
    case XK_3:              return Key::DIGIT_3;            //0x0033
    case XK_numbersign:     return Key::DIGIT_3;            //0x0023
    case XK_4:              return Key::DIGIT_4;            //0x0034
    case XK_dollar:         return Key::DIGIT_4;            //0x0024
    case XK_5:              return Key::DIGIT_5;            //0x0035
    case XK_percent:        return Key::DIGIT_5;            //0x0025
    case XK_6:              return Key::DIGIT_6;            //0x0036
    case XK_asciicircum:    return Key::DIGIT_6;            //0x005e
    case XK_7:              return Key::DIGIT_7;            //0x0037
    case XK_ampersand:      return Key::DIGIT_7;            //0x0026
    case XK_8:              return Key::DIGIT_8;            //0x0038
    case XK_asterisk:       return Key::DIGIT_8;            //0x002a
    case XK_9:              return Key::DIGIT_9;            //0x0039
    case XK_parenleft:      return Key::DIGIT_9;            //0x0028
    case XK_minus:          return Key::DASH;               //0x002d
    case XK_underscore:     return Key::DASH;               //0x005f
    case XK_equal:          return Key::EQUALS;             //0x003d
    case XK_plus:           return Key::EQUALS;             //0x002b
    case XK_a:              return Key::A;                  //0x0041
    case XK_b:              return Key::B;                  //0x0042
    case XK_c:              return Key::C;                  //0x0043
    case XK_d:              return Key::D;                  //0x0044
    case XK_e:              return Key::E;                  //0x0045
    case XK_f:              return Key::F;                  //0x0046
    case XK_g:              return Key::G;                  //0x0047
    case XK_h:              return Key::H;                  //0x0048
    case XK_i:              return Key::I;                  //0x0049
    case XK_j:              return Key::J;                  //0x004a
    case XK_k:              return Key::K;                  //0x004b
    case XK_l:              return Key::L;                  //0x004c
    case XK_m:              return Key::M;                  //0x004d
    case XK_n:              return Key::N;                  //0x004e
    case XK_o:              return Key::O;                  //0x004f
    case XK_p:              return Key::P;                  //0x0050
    case XK_q:              return Key::Q;                  //0x0051
    case XK_r:              return Key::R;                  //0x0052
    case XK_s:              return Key::S;                  //0x0053
    case XK_t:              return Key::T;                  //0x0054
    case XK_u:              return Key::U;                  //0x0055
    case XK_v:              return Key::V;                  //0x0056
    case XK_w:              return Key::W;                  //0x0057
    case XK_x:              return Key::X;                  //0x0058
    case XK_y:              return Key::Y;                  //0x0059
    case XK_z:              return Key::Z;                  //0x005a
    case XK_space:          return Key::SPACE;              //0x0020
    case XK_Return:         return Key::ENTER;              //0xff0d
    case XK_Tab:            return Key::TAB;                //0xff09
    case XK_BackSpace:      return Key::BACKSPACE;          //0xff08
    case XK_Caps_Lock:      return Key::CAPS_LOCK;          //0xffe5
    case XK_comma:          return Key::COMMA;              //0x002c
    case XK_less:           return Key::COMMA;              //0x003c
    case XK_period:         return Key::PERIOD;             //0x002e
    case XK_greater:        return Key::PERIOD;             //0x003e
    case XK_apostrophe:     return Key::QUOTE;              //0x0027
    case XK_quotedbl:       return Key::QUOTE;              //0x0022
    case XK_semicolon:      return Key::SEMICOLON;          //0x003b
    case XK_colon:          return Key::SEMICOLON;          //0x003a
    case XK_slash:          return Key::SLASH;              //0x002f
    case XK_question:       return Key::SLASH;              //0x003f
    case XK_backslash:      return Key::BACKSLASH;          //0x005c
    case XK_bar:            return Key::BACKSLASH;          //0x007c
    case XK_bracketleft:    return Key::LBRACKET;           //0x005b
    case XK_braceleft:      return Key::LBRACKET;           //0x007b
    case XK_bracketright:   return Key::RBRACKET;           //0x005d
    case XK_braceright:     return Key::RBRACKET;           //0x007d
    case XK_Shift_L:        return Key::LSHIFT;             //0xffe1
    case XK_Shift_R:        return Key::RSHIFT;             //0xffe2
    case XK_Control_L:      return Key::LCTRL;              //0xffe3
    case XK_Control_R:      return Key::RCTRL;              //0xffe4
    case XK_Alt_L:          return Key::LALT;               //0xffe9
    case XK_Alt_R:          return Key::RALT;               //0xffea
    case XK_Super_L:        return Key::LSYSTEM;            //0xffeb
    case XK_Super_R:        return Key::RSYSTEM;            //0xffec
    case XK_Menu:           return Key::MENU;               //0xff67
    case XK_Sys_Req:        return Key::PRINT_SCREEN;       //0xff15
    case XK_Scroll_Lock:    return Key::SCROLL_LOCK;        //0xff14
    case XK_Pause:          return Key::PAUSE_BREAK;        //0xff13
    case XK_Insert:         return Key::INSERT;             //0xff63
    case XK_Delete:         return Key::DEL;                //0xffff
    case XK_Home:           return Key::HOME;               //0xff50
    case XK_End:            return Key::END;                //0xff57
    case XK_Page_Up:        return Key::PAGE_UP;            //0xff55
    case XK_Page_Down:      return Key::PAGE_DOWN;          //0xff56
    case XK_Up:             return Key::UP;                 //0xff52
    case XK_Down:           return Key::DOWN;               //0xff54
    case XK_Left:           return Key::LEFT;               //0xff51
    case XK_Right:          return Key::RIGHT;              //0xff53
    case XK_KP_0:           return Key::NUMPAD_0;           //0xffb0
    case XK_KP_1:           return Key::NUMPAD_1;           //0xffb1
    case XK_KP_2:           return Key::NUMPAD_2;           //0xffb2
    case XK_KP_3:           return Key::NUMPAD_3;           //0xffb3
    case XK_KP_4:           return Key::NUMPAD_4;           //0xffb4
    case XK_KP_5:           return Key::NUMPAD_5;           //0xffb5
    case XK_KP_6:           return Key::NUMPAD_6;           //0xffb6
    case XK_KP_7:           return Key::NUMPAD_7;           //0xffb7
    case XK_KP_8:           return Key::NUMPAD_8;           //0xffb8
    case XK_KP_9:           return Key::NUMPAD_9;           //0xffb9
    case XK_KP_Add:         return Key::ADD;                //0xffab
    case XK_KP_Subtract:    return Key::SUBTRACT;           //0xffad
    case XK_KP_Multiply:    return Key::MULTIPLY;           //0xffaa
    case XK_KP_Divide:      return Key::DIVIDE;             //0xffae
    case XK_KP_Decimal:     return Key::DECIMAL;            //0xffae
    case XK_Num_Lock:       return Key::NUMLOCK;            //0xff7f
    case XK_KP_Insert:      return Key::NUMPAD_INSERT;      //0xff9e
    case XK_KP_Delete:      return Key::NUMPAD_DEL;         //0xff9f
    case XK_KP_Home:        return Key::NUMPAD_HOME;        //0xff95
    case XK_KP_End:         return Key::NUMPAD_END;         //0xff9c
    case XK_KP_Page_Up:     return Key::NUMPAD_PAGE_UP;     //0xff9a
    case XK_KP_Page_Down:   return Key::NUMPAD_PAGE_DOWN;   //0xff9b
    case XK_KP_Up:          return Key::NUMPAD_UP;          //0xff97
    case XK_KP_Down:        return Key::NUMPAD_DOWN;        //0xff99
    case XK_KP_Left:        return Key::NUMPAD_LEFT;        //0xff96
    case XK_KP_Right:       return Key::NUMPAD_RIGHT;       //0xff98
    case XK_KP_Enter:       return Key::NUMPAD_ENTER;       //0xff8d
    case XK_KP_Begin:       return Key::CLEAR;              //0xff9d

    //The invalid key is returned if an unrecognized keycode is provided.
    default:
        logError( ( boost::format( "Unrecognized keycode: 0x%|04x|" ) % lower ).str() );
        return Key::INVALID;
    }
}

void LinuxWindow::setTitle( const ustring& title ) {
    BaseWindowImpl::setTitle( title );
    //TODO
}

void LinuxWindow::setPopup( const bool popup ) {
    BaseWindowImpl::setPopup( popup );
    //TODO
}

void LinuxWindow::setResizable( const bool resizeable ) {
    BaseWindowImpl::setResizable( resizeable );
    //TODO
}

void LinuxWindow::setVisible( const bool visible ) {
    BaseWindowImpl::setVisible( visible );
    //TODO
}

void LinuxWindow::setBounds( const Bounds2i bounds ) {
    BaseWindowImpl::setBounds( bounds );
    //TODO
}

void LinuxWindow::setMouseCapture( const bool capture ) {
    BaseWindowImpl::setMouseCapture( capture );
    //TODO
}

void LinuxWindow::sendToBottom() {
    xerrBegin();
    XLowerWindow( m_display, m_window );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
}

Point2i LinuxWindow::screenToWindow( Point2i screenCoords ) const {
    //TODO
    return screenCoords;
}

Point2i LinuxWindow::windowToScreen( Point2i windowCoords ) const {
    //TODO
    return windowCoords;
}

WindowDisplay LinuxWindow::getDisplay() const {
    return m_display;
}

WindowHandle LinuxWindow::getHandle() const {
    return m_window;
}

void LinuxWindow::initX() {
    //Open a display
    m_display = XOpenDisplay( nullptr );
    if( m_display == nullptr )
        throw NullPointerException();

    //Finished initializing
    m_xInitialized = true;
}

void LinuxWindow::destroyX() {
    m_xInitialized = false;

    //Close the display
    XCloseDisplay( m_display );
    m_display = nullptr;
}

}
}
