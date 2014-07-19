/*
linux/LinuxWindow.cpp
-----------------------
Copyright (c) 2014, theJ89
A lot of code in this file was taken from SFML (by Laurent Gomila)

Description:
    See LinuxWindow.hpp for more information.
*/




//Includes
#include <brimstone/linux/LinuxWindow.hpp>      //Class header
#include <brimstone/util/Range.hpp>             //ClampedValue
#include <brimstone/WindowEvents.hpp>           //MouseClickEvent, MouseMoveEvent, KeyPressEvent
#include <brimstone/Exception.hpp>              //NullPointerException

#include <brimstone/Logger.hpp>                 //TEMP
#include <boost/format.hpp>                     //TEMP

#include <brimstone/Window.hpp>                 //Really stupid circular dependency hack




namespace Brimstone {
namespace Private {

LinuxWindow::XWinToWindowMap    LinuxWindow::m_windowMap;
Display*                        LinuxWindow::m_display  = nullptr;
int                             LinuxWindow::m_screen   = 0;

LinuxWindow::LinuxWindow( Window& parent ) : m_parent( &parent ) {
    if( m_windowMap.empty() ) {
        m_display = XOpenDisplay( nullptr );
        if( m_display == nullptr )
            throw NullPointerException();
        m_screen = DefaultScreen( m_display );
    }

    unsigned long black, white;
    black       = BlackPixel( m_display, m_screen );
    white       = WhitePixel( m_display, m_screen );

    //Get title
    ustring title;
    parent.getTitle( title );

    //Get bounds
    LongRectangle bounds;
    parent.getBounds( bounds );
    long width  = bounds.getWidth();
    long height = bounds.getHeight();

    m_window    = XCreateSimpleWindow( m_display, DefaultRootWindow( m_display ), bounds.left, bounds.top, width, height, 5, white, black );
    if( !m_window )
        throw NullPointerException();  

    //Set title
    XSetStandardProperties( m_display, m_window, title.c_str(), title.c_str(), None, nullptr, 0, nullptr );

    //Tell X what kinds of events we're interested in
    XSelectInput(
        m_display, m_window,
        ButtonPressMask     | ButtonReleaseMask   |
        PointerMotionMask   |
        KeyPressMask        | KeyReleaseMask      |
        KeymapStateMask     |
        StructureNotifyMask /*| ExposureMask*/
    );

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
    //send a ClientMessage event whenever the close button is clicked instead of doing it's default behavior.
    //NOTE: XInternAtom can return None if the third parameter (only_if_exists) is true,
    //      therefore we set it to "false" to ensure the atom is always created.
    //NOTE: you can run xlsatoms ("X list atoms") in a terminal to get registered atoms
    m_closeAtom = XInternAtom( m_display, "WM_DELETE_WINDOW", false );
    XSetWMProtocols( m_display, m_window, &m_closeAtom, 1 );

    m_windowMap.emplace( m_window, *this );

    //XClearWindow( m_display, m_window );
    //XMapRaised( m_display, m_window );
    XMapWindow( m_display, m_window );
}

LinuxWindow::~LinuxWindow() {
    if( m_inputContext != nullptr )
        XDestroyIC( m_inputContext );

    if( m_inputMethod != nullptr )
        XCloseIM( m_inputMethod );

    //XFreeGC( m_pcDisplay, m_cGraphicsContext );
    XDestroyWindow( m_display, m_window );
    
    m_windowMap.erase( m_window );

    if( m_windowMap.empty() ) {
        XCloseDisplay( m_display );
        m_display = nullptr;
    }
}

void LinuxWindow::processEvents() {
    XEvent xEvent;
    //int iEventsQueued = XEventsQueued( m_pcDisplay, QueuedAlready );
    while( m_display != nullptr ) {
        XNextEvent( m_display, &xEvent );
        mainProc( xEvent );
    }
}

void LinuxWindow::mainProc( XEvent& xEvent ) {
    //Locate what Window this belongs to
    auto it = m_windowMap.find( xEvent.xany.window );
    if( it == m_windowMap.end() )
        return;

    it->second.windowProc( xEvent );
}

void LinuxWindow::windowProc( XEvent& xEvent ) {
    switch( xEvent.type ) {
        //Window close request
        case ClientMessage: {
            //Client messages are generic, so we need to determine for certain that we were sent a close request.
            //Specifically, the first data member should be the "WM_DELETE_WINDOW" atom.
            //The .data field can be interpreted as an array of 8-bit, 16-bit, or 32-bit values.
            //The .format field tells us how we should interpret the data.
            //Atoms are 32-bit unsigned ints, so we're expecting the message to contain 32-bit values.
            //This is good to check because another message could have a different size, but the same data in l[0].
            if( xEvent.xclient.format == 32 && (Atom)( xEvent.xclient.data.l[0] ) == m_closeAtom ) {
                WindowCloseEvent eventObj( *m_parent );
                m_parent->m_signalWindowClose( eventObj );
            }
        } break;
        //Window moved or resized
        case ConfigureNotify: {
        } break;
        //Mouse move
        case MotionNotify: {
            MouseMoveEvent eventObj( xEvent.xbutton.x, xEvent.xbutton.y );
            m_parent->m_signalMouseMove( eventObj );
        } break;
        //Key down / Character typed
        case KeyPress: {
            //The docs aren't too clear on the string returned by XLookupString.
            //I assumed it returns 1 encoding plus a null character, but this doesn't
            //seem to be the case for all keys.
            CharacterTypedEvent typedEvent;
            uchar* translatedCharacter = const_cast< uchar* >( typedEvent.getCharacter() );
            KeySym keySym;

            //Note: we're only interested in the KeySym at this point.
            //The translated character is ignored
            XLookupString( &xEvent.xkey, translatedCharacter, 4, &keySym, nullptr );

            //Dispatch a keydown event
            KeyDownEvent keydownEvent( xKeySymToKey( keySym ) );
            m_parent->m_signalKeyDown( keydownEvent );

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
            
            m_parent->m_signalCharacterTyped( typedEvent );
        } break;
        //Key up
        case KeyRelease: {
            char translatedCharacter[5];
            KeySym keySym;
            XLookupString( &xEvent.xkey, translatedCharacter, sizeof( translatedCharacter ), &keySym, nullptr );

            //Dispatch a KeyUp event
            KeyUpEvent eventObj( xKeySymToKey( keySym ) );
            m_parent->m_signalKeyUp( eventObj );
        } break;
        //Button down
        case ButtonPress: {
            int button = xEvent.xbutton.button;
            //Vertical scrolling
            if( button == Button4 || button == Button5 ) {
                MouseVScrollEvent eventObj(
                    xEvent.xbutton.button == Button4 ? 1.0f : -1.0f,
                    xEvent.xbutton.x,
                    xEvent.xbutton.y
                );

                m_parent->m_signalMouseVScroll( eventObj );

            //Horizontal scrolling (note: direction is assumed and needs to be tested somehow)
            } else if( button == 6 || button == 7 ) {
                MouseHScrollEvent eventObj(
                    xEvent.xbutton.button == 6 ? -1.0f : 1.0f,
                    xEvent.xbutton.x,
                    xEvent.xbutton.y
                );

                m_parent->m_signalMouseHScroll( eventObj );

            //Actual buttons
            } else {
                MouseDownEvent eventObj(
                    xButtonToMouseButton( button ),
                    xEvent.xbutton.x,
                    xEvent.xbutton.y
                );

                m_parent->m_signalMouseDown( eventObj );
            }
        } break;
        //Button up
        case ButtonRelease: {
            int button = xEvent.xbutton.button;

            //Ignore wheel scrolling. This is handled in the ButtonPress case above
            if( button == Button4 || button == Button5 || button == 6 || button == 7 )
                break;

            MouseUpEvent eventObj(
                xButtonToMouseButton( button ),
                xEvent.xbutton.x,
                xEvent.xbutton.y
            );
            m_parent->m_signalMouseUp( eventObj );
        } break;
        //Unhandled event
        default: {
            logError( ( boost::format( "Unhandled event: %1%" ) % xEvent.type ).str().c_str() );
        } break;
    }
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
        logError( ( boost::format( "Unrecognized button: 0x%|04x|" ) % button ).str().c_str() );
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
        logError( ( boost::format( "Unrecognized keycode: 0x%|04x|" ) % lower ).str().c_str() );
        return Key::INVALID;
    }
}

void LinuxWindow::setTitle( const ustring& title ) {
}

void LinuxWindow::setPopup( const bool popup ) {
}

void LinuxWindow::setBounds( const LongRectangle& bounds ) {
}

LinuxWindow::LinuxWindow( const LinuxWindow& ) : m_parent( nullptr ) {
}

LinuxWindow& LinuxWindow::operator =( const LinuxWindow& ) {
    return *this;
}

}
}
