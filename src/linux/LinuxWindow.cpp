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
#include <brimstone/util/Range.hpp>             //ClampedValue
#include <brimstone/Exception.hpp>              //NullPointerException

#include <brimstone/Logger.hpp>                 //TEMP
#include <boost/format.hpp>                     //TEMP

#include <brimstone/Window.hpp>                 //Really stupid circular dependency hack




namespace Brimstone {
namespace Private {

LinuxWindow::XWinToWindowMap    LinuxWindow::m_acWindowMap;
Display*                        LinuxWindow::m_pcDisplay = nullptr;
int                             LinuxWindow::m_iScreen = 0;

LinuxWindow::LinuxWindow( Window& cParent ) : m_pcParent( &cParent ) {
    if( m_acWindowMap.empty() ) {
        m_pcDisplay = XOpenDisplay( nullptr );
        if( m_pcDisplay == nullptr )
            throw NullPointerException();
        m_iScreen = DefaultScreen( m_pcDisplay );
    }

    unsigned long iBlack, iWhite;
    iBlack      = BlackPixel( m_pcDisplay, m_iScreen );
    iWhite      = WhitePixel( m_pcDisplay, m_iScreen );

    //Get title
    ustring strTitle;
    cParent.getTitle( strTitle );

    //Get bounds
    LongRectangle cBounds;
    cParent.getBounds( cBounds );
    long iWidth  = cBounds.getWidth();
    long iHeight = cBounds.getHeight();

    m_cWindow   = XCreateSimpleWindow( m_pcDisplay, DefaultRootWindow( m_pcDisplay ), cBounds.left, cBounds.top, iWidth, iHeight, 5, iWhite, iBlack );
    if( !m_cWindow )
        throw NullPointerException();  

    //Set title
    XSetStandardProperties( m_pcDisplay, m_cWindow, strTitle.c_str(), strTitle.c_str(), None, nullptr, 0, nullptr );

    //Tell X what kinds of events we're interested in
    XSelectInput(
        m_pcDisplay, m_cWindow,
        ButtonPressMask     | ButtonReleaseMask   |
        PointerMotionMask   |
        KeyPressMask        | KeyReleaseMask      |
        KeymapStateMask     |
        StructureNotifyMask /*| ExposureMask*/
    );

    //Create an input method and context.
    //We need this to translate key press events to typed characters.
    m_pcInputMethod = XOpenIM( m_pcDisplay, nullptr, nullptr, nullptr );
    if( m_pcInputMethod != nullptr ) {
        m_pcInputContext = XCreateIC(
            m_pcInputMethod,
            XNClientWindow, m_cWindow,
            XNFocusWindow,  m_cWindow,
            XNInputStyle,   XIMPreeditNothing | XIMStatusNothing,
            nullptr
        );
    } else {
        m_pcInputContext = nullptr;
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
    m_uiCloseAtom = XInternAtom( m_pcDisplay, "WM_DELETE_WINDOW", false );
    XSetWMProtocols( m_pcDisplay, m_cWindow, &m_uiCloseAtom, 1 );

    m_acWindowMap.emplace( m_cWindow, *this );

    //XClearWindow( m_pcDisplay, m_cWindow );
    //XMapRaised( m_pcDisplay, m_cWindow );
    XMapWindow( m_pcDisplay, m_cWindow );
}

LinuxWindow::~LinuxWindow() {
    if( m_pcInputContext != nullptr )
        XDestroyIC( m_pcInputContext );

    if( m_pcInputMethod != nullptr )
        XCloseIM( m_pcInputMethod );

    //XFreeGC( m_pcDisplay, m_cGraphicsContext );
    XDestroyWindow( m_pcDisplay, m_cWindow );
    
    m_acWindowMap.erase( m_cWindow );

    if( m_acWindowMap.empty() ) {
        XCloseDisplay( m_pcDisplay );
        m_pcDisplay = nullptr;
    }
}

void LinuxWindow::processEvents() {
    XEvent cXEvent;
    //int iEventsQueued = XEventsQueued( m_pcDisplay, QueuedAlready );
    while( m_pcDisplay != nullptr ) {
        XNextEvent( m_pcDisplay, &cXEvent );
        mainProc( cXEvent );
    }
}

void LinuxWindow::mainProc( XEvent& cXEvent ) {
    //Locate what Window this belongs to
    auto it = m_acWindowMap.find( cXEvent.xany.window );
    if( it == m_acWindowMap.end() )
        return;

    it->second.windowProc( cXEvent );
}

void LinuxWindow::windowProc( XEvent& cXEvent ) {
    switch( cXEvent.type ) {
        //Window close request
        case ClientMessage: {
            //Client messages are generic, so we need to determine for certain that we were sent a close request.
            //Specifically, the first data member should be the "WM_DELETE_WINDOW" atom.
            //The .data field can be interpreted as an array of 8-bit, 16-bit, or 32-bit values.
            //The .format field tells us how we should interpret the data.
            //Atoms are 32-bit unsigned ints, so we're expecting the message to contain 32-bit values.
            //This is good to check because another message could have a different size, but the same data in l[0].
            if( cXEvent.xclient.format == 32 && (Atom)( cXEvent.xclient.data.l[0] ) == m_uiCloseAtom ) {
                WindowCloseEvent cEvent( *m_pcParent );
                m_pcParent->m_cSignalWindowClose( cEvent );
            }
        } break;
        //Window moved or resized
        case ConfigureNotify: {
        } break;
        //Mouse move
        case MotionNotify: {
            MouseMoveEvent cEvent( cXEvent.xbutton.x, cXEvent.xbutton.y );
            m_pcParent->m_cSignalMouseMove( cEvent );
        } break;
        //Key down / Character typed
        case KeyPress: {
            //The docs aren't too clear on the string returned by XLookupString.
            //I assumed it returns 1 encoding plus a null character, but this doesn't
            //seem to be the case for all keys.
            CharacterTypedEvent cTypedEvent;
            uchar* pszTranslatedCharacter = const_cast< uchar* >( cTypedEvent.getCharacter() );
            KeySym cKeySym;

            //Note: we're only interested in the KeySym at this point.
            //The translated character is ignored
            XLookupString( &cXEvent.xkey, pszTranslatedCharacter, 4, &cKeySym, nullptr );

            //Dispatch a keydown event
            KeyDownEvent cEvent( xKeySymToKey( cKeySym ) );
            m_pcParent->m_cSignalKeyDown( cEvent );

            //Why do I need to do this...?
            if( XFilterEvent( &cXEvent, None ) ) {
                logInfo( "Filtered a keypress event" );
                break;
            }

            int iCount;
#ifdef X_HAVE_UTF8_STRING
            
            if( m_pcInputContext != nullptr ) {
                iCount = Xutf8LookupString( m_pcInputContext, &cXEvent.xkey, pszTranslatedCharacter, 4, nullptr, nullptr );
            } else
#endif
            {
                iCount = XLookupString( &cXEvent.xkey, pszTranslatedCharacter, 4, nullptr, nullptr );
            }
            
            //NOTE: Xutf8LookupString and XLookupString return 0 when
            //trying to get the character for a non-character keycode (SHIFT, for instance).
            if( iCount == 0 )
                break;

            //A UTF-8 encoding of a single code point should never exceed 4 bytes in length
            if( iCount > 4 )
                throw UnexpectedResultException();

            //Null terminate the string (this is a problem with the strings for some keys, such as Numpad /)
            pszTranslatedCharacter[iCount] = 0;
            
            m_pcParent->m_cSignalCharacterTyped( cTypedEvent );
        } break;
        //Key up
        case KeyRelease: {
            char szTranslatedCharacter[5];
            KeySym cKeySym;
            XLookupString( &cXEvent.xkey, szTranslatedCharacter, sizeof( szTranslatedCharacter ), &cKeySym, nullptr );

            //Dispatch a KeyUp event
            KeyUpEvent cEvent( xKeySymToKey( cKeySym ) );
            m_pcParent->m_cSignalKeyUp( cEvent );
        } break;
        //Button down
        case ButtonPress: {
            int iButton = cXEvent.xbutton.button;
            //Vertical scrolling
            if( iButton == Button4 || iButton == Button5 ) {
                MouseVScrollEvent cEvent(
                    cXEvent.xbutton.button == Button4 ? 1.0f : -1.0f,
                    cXEvent.xbutton.x,
                    cXEvent.xbutton.y
                );

                m_pcParent->m_cSignalMouseVScroll( cEvent );

            //Horizontal scrolling (note: direction is assumed and needs to be tested somehow)
            } else if( iButton == 6 || iButton == 7 ) {
                MouseHScrollEvent cEvent(
                    cXEvent.xbutton.button == 6 ? -1.0f : 1.0f,
                    cXEvent.xbutton.x,
                    cXEvent.xbutton.y
                );

                m_pcParent->m_cSignalMouseHScroll( cEvent );

            //Actual buttons
            } else {
                MouseUpEvent cEvent(
                    xButtonToMouseButton( iButton ),
                    cXEvent.xbutton.x,
                    cXEvent.xbutton.y
                );

                m_pcParent->m_cSignalMouseUp( cEvent );
            }
        } break;
        //Button up
        case ButtonRelease: {
            int iButton = cXEvent.xbutton.button;

            //Ignore wheel scrolling. This is handled in the ButtonPress case below
            if( iButton == Button4 || iButton == Button5 || iButton == 6 || iButton == 7 )
                break;

            MouseDownEvent cEvent(
                xButtonToMouseButton( iButton ),
                cXEvent.xbutton.x,
                cXEvent.xbutton.y
            );
            m_pcParent->m_cSignalMouseDown( cEvent );
        } break;
        //Unhandled event
        default: {
            logError( ( boost::format( "Unhandled event: %1%" ) % cXEvent.type ).str().c_str() );
        } break;
    }
}

MouseButton LinuxWindow::xButtonToMouseButton( const int iButton ) {
    switch( iButton ) {
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
        logError( ( boost::format( "Unrecognized button: 0x%|04x|" ) % iButton ).str().c_str() );
        return MouseButton::INVALID;
    }
}

Key LinuxWindow::xKeySymToKey( const KeySym& pcKeySym ) {
    KeySym pcLower, pcUpper;
    XConvertCase( pcKeySym, &pcLower, &pcUpper );
    
    switch( pcLower ) {
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
        logError( ( boost::format( "Unrecognized keycode: 0x%|04x|" ) % pcLower ).str().c_str() );
        return Key::INVALID;
    }
}

void LinuxWindow::setTitle( const ustring& strTitle ) {
}

void LinuxWindow::setPopup( const bool bPopup ) {
}

void LinuxWindow::setBounds( const LongRectangle& cBounds ) {
}

LinuxWindow::LinuxWindow( const LinuxWindow& ) : m_pcParent( nullptr ) {
}

LinuxWindow& LinuxWindow::operator =( const LinuxWindow& ) {
    return *this;
}

}
}
