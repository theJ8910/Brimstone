/*
linux/x11/XWindow.cpp
---------------------
Copyright (c) 2024, theJ89
A lot of code in this file is based off of code from SDL (Simple DirectMedia Layer) and SFML (by Laurent Gomila), and to a lesser extent examples from the OpenGL wiki.

Description:
    See XWindow.hpp for more information.
*/




//TODO: When the window is opened for the first time or shown after being hidden, check if the window should be fullscreen.
//      If so, make the window fullscreen. Currently, the window can only be made fullscreen after being mapped (shown), and X11 removes this window property when the window is unmapped
//      (hidden).
//      The window can also be manually taken out of fullscreen by the window manager (e.g. with KWin, you can open a context menu for the window and de-select "Fullscreen") from the menu,
//      or even set up a global keyboard shortcut to toggle fullscreen on the active window.
//      We should add some way to detect this and update the internal fullscreen state and/or generate window events to respond to this happening.
//TODO: Hiding the window then showing it is moving the window about a border's width (1px) to the right, and a title bar's height (28px) down. This almost certainly has something to do with
//      us moving our window programmatically, and then it having its position getting adjusted by Window Manager's decorations. Need to get that figured out.




//Includes
#include "XWindow.hpp"                          //Class header
#include "XException.hpp"                       //Brimstone::Private::xerrBegin, Brimstone::Private::xerrEnd
#include "XShared.hpp"                          //Brimstone::Private::XShared

#include <brimstone/util/Range.hpp>             //Brimstone::clampedValue
#include <brimstone/Exception.hpp>              //Brimstone::NullPointerException
#include <brimstone/Logger.hpp>                 //Brimstone::logError

#include <X11/XF86keysym.h>                     //X11; XF86XK_*
#include <X11/Xatom.h>                          //X11; XA_ATOM

#include <boost/format.hpp>                     //boost::format




namespace {
    constexpr long WINDOW_ATTRIBUTES_VALUEMASK = (
        CWBorderPixel | CWColormap | CWEventMask
    );

    constexpr long WINDOW_ATTRIBUTES_EVENT_MASK = (
        ButtonPressMask     | ButtonReleaseMask   |
        PointerMotionMask   |
        EnterWindowMask     | LeaveWindowMask     |
        KeyPressMask        | KeyReleaseMask      |
        FocusChangeMask     |
        KeymapStateMask     |
        StructureNotifyMask | /*ExposureMask |*/
        PropertyChangeMask
    );

    constexpr long XGRABPOINTER_EVENT_MASK = (
        ButtonPressMask   | ButtonReleaseMask |
        PointerMotionMask |
        EnterWindowMask   | LeaveWindowMask
    );

    constexpr long XSENDEVENT_EVENT_MASK = (
        SubstructureNotifyMask | SubstructureRedirectMask
    );

    constexpr long XSIZEHINTS_FLAGS_BOUNDS     = (
        PPosition | PSize
    );
    constexpr long XSIZEHINTS_FLAGS_MINMAXSIZE = (
        PMinSize | PMaxSize
    );

    constexpr long _NET_WM_STATE_REMOVE = 0L;
    constexpr long _NET_WM_STATE_ADD    = 1L;

    //Structure used to set the _MOTIF_WM_HINTS property, copied from "lib/Xm/MwmUtil.h" of the OpenMotif project (just called Motif on SourceForge).
    struct PropMotifWmHints {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
                 long inputMode;
        unsigned long status;
    };

    //Bit definitions for PropMotifWmHints.flags that we use, also taken from "lib/Xm/MwmUtil.h":
    constexpr unsigned long MWM_HINTS_DECORATIONS = 2UL; //1L << 1
    constexpr unsigned long MWM_DECOR_ALL         = 1UL; //1L << 0
}

namespace {
    using namespace Brimstone;
    using namespace Brimstone::Private;

    /*
    getCursorPosFromXEvent{1}
    -------------------------

    Description:
        Returns the position the cursor was at during an X11 mouse up / mouse down / mousewheel scroll event (XButtonEvent).

    Arguments:
        xbutton:  An XButtonEvent.

    Returns:
        Point2i:  The cursor position.
    */
    Point2i getCursorPosFromXEvent( const XButtonEvent& xbutton ) {
        return Point2i( xbutton.x, xbutton.y );
    }

    /*
    getCursorPosFromXEvent{2}
    -------------------------

    Description:
        Returns the position the cursor was at during an X11 mouse movement event (XMotionEvent).

    Arguments:
        xmotion:  An XMotionEvent.

    Returns:
        Point2i:  The cursor position.
    */
    Point2i getCursorPosFromXEvent( const XMotionEvent& xmotion ) {
        return Point2i( xmotion.x, xmotion.y );
    }

    /*
    xButtonToMouseButton
    --------------------

    Description:
        Translates the given X11 mouse button ID to a Brimstone MouseButton enum.

    Arguments:
        button:  The X11 mouse button ID.

    Returns:
        MouseButton:  A Brimstone MouseButton enum.
    */
    MouseButton xButtonToMouseButton( const int button ) {
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
            logError( ( boost::format( "Unrecognized button: 0x%|04X|" ) % button ).str() );
            return MouseButton::INVALID;
        }
    }

    /*
    xKeySymToKey
    ------------

    Description:
        Translates the given X11 KeySym to a Brimstone Key enum.

    Arguments:
        keySym:  The X11 KeySym.

    Returns:
        Key:     A Brimstone Key enum.
    */
    Key xKeySymToKey( const KeySym& keySym ) {
        KeySym lower, upper;
        XConvertCase( keySym, &lower, &upper );

        switch( lower ) {
        case XK_Escape:                return Key::ESCAPE;             //0x0000FF1B
        case XK_F1:                    return Key::F1;                 //0x0000FFBE
        case XK_F2:                    return Key::F2;                 //0x0000FFBF
        case XK_F3:                    return Key::F3;                 //0x0000FFC0
        case XK_F4:                    return Key::F4;                 //0x0000FFC1
        case XK_F5:                    return Key::F5;                 //0x0000FFC2
        case XK_F6:                    return Key::F6;                 //0x0000FFC3
        case XK_F7:                    return Key::F7;                 //0x0000FFC4
        case XK_F8:                    return Key::F8;                 //0x0000FFC5
        case XK_F9:                    return Key::F9;                 //0x0000FFC6
        case XK_F10:                   return Key::F10;                //0x0000FFC7
        case XK_F11:                   return Key::F11;                //0x0000FFC8
        case XK_F12:                   return Key::F12;                //0x0000FFC9
        case XK_grave:                 return Key::TILDE;              //0x00000060
        case XK_asciitilde:            return Key::TILDE;              //0x0000007E
        case XK_0:                     return Key::DIGIT_0;            //0x00000030
        case XK_parenright:            return Key::DIGIT_0;            //0x00000029
        case XK_1:                     return Key::DIGIT_1;            //0x00000031
        case XK_exclam:                return Key::DIGIT_1;            //0x00000021
        case XK_2:                     return Key::DIGIT_2;            //0x00000032
        case XK_at:                    return Key::DIGIT_2;            //0x00000040
        case XK_3:                     return Key::DIGIT_3;            //0x00000033
        case XK_numbersign:            return Key::DIGIT_3;            //0x00000023
        case XK_4:                     return Key::DIGIT_4;            //0x00000034
        case XK_dollar:                return Key::DIGIT_4;            //0x00000024
        case XK_5:                     return Key::DIGIT_5;            //0x00000035
        case XK_percent:               return Key::DIGIT_5;            //0x00000025
        case XK_6:                     return Key::DIGIT_6;            //0x00000036
        case XK_asciicircum:           return Key::DIGIT_6;            //0x0000005E
        case XK_7:                     return Key::DIGIT_7;            //0x00000037
        case XK_ampersand:             return Key::DIGIT_7;            //0x00000026
        case XK_8:                     return Key::DIGIT_8;            //0x00000038
        case XK_asterisk:              return Key::DIGIT_8;            //0x0000002A
        case XK_9:                     return Key::DIGIT_9;            //0x00000039
        case XK_parenleft:             return Key::DIGIT_9;            //0x00000028
        case XK_minus:                 return Key::MINUS;              //0x0000002D
        case XK_underscore:            return Key::MINUS;              //0x0000005F
        case XK_equal:                 return Key::EQUALS;             //0x0000003D
        case XK_plus:                  return Key::EQUALS;             //0x0000002B
        case XK_a:                     return Key::A;                  //0x00000041
        case XK_b:                     return Key::B;                  //0x00000042
        case XK_c:                     return Key::C;                  //0x00000043
        case XK_d:                     return Key::D;                  //0x00000044
        case XK_e:                     return Key::E;                  //0x00000045
        case XK_f:                     return Key::F;                  //0x00000046
        case XK_g:                     return Key::G;                  //0x00000047
        case XK_h:                     return Key::H;                  //0x00000048
        case XK_i:                     return Key::I;                  //0x00000049
        case XK_j:                     return Key::J;                  //0x0000004A
        case XK_k:                     return Key::K;                  //0x0000004B
        case XK_l:                     return Key::L;                  //0x0000004C
        case XK_m:                     return Key::M;                  //0x0000004D
        case XK_n:                     return Key::N;                  //0x0000004E
        case XK_o:                     return Key::O;                  //0x0000004F
        case XK_p:                     return Key::P;                  //0x00000050
        case XK_q:                     return Key::Q;                  //0x00000051
        case XK_r:                     return Key::R;                  //0x00000052
        case XK_s:                     return Key::S;                  //0x00000053
        case XK_t:                     return Key::T;                  //0x00000054
        case XK_u:                     return Key::U;                  //0x00000055
        case XK_v:                     return Key::V;                  //0x00000056
        case XK_w:                     return Key::W;                  //0x00000057
        case XK_x:                     return Key::X;                  //0x00000058
        case XK_y:                     return Key::Y;                  //0x00000059
        case XK_z:                     return Key::Z;                  //0x0000005A
        case XK_space:                 return Key::SPACE;              //0x00000020
        case XK_Return:                return Key::RETURN;             //0x0000FF0D
        case XK_Tab:                   return Key::TAB;                //0x0000FF09
        case XK_BackSpace:             return Key::BACKSPACE;          //0x0000FF08
        case XK_Caps_Lock:             return Key::CAPS_LOCK;          //0x0000FFE5
        case XK_comma:                 return Key::COMMA;              //0x0000002C
        case XK_less:                  return Key::COMMA;              //0x0000003C
        case XK_period:                return Key::PERIOD;             //0x0000002E
        case XK_greater:               return Key::PERIOD;             //0x0000003E
        case XK_apostrophe:            return Key::QUOTE;              //0x00000027
        case XK_quotedbl:              return Key::QUOTE;              //0x00000022
        case XK_semicolon:             return Key::SEMICOLON;          //0x0000003B
        case XK_colon:                 return Key::SEMICOLON;          //0x0000003A
        case XK_slash:                 return Key::SLASH;              //0x0000002F
        case XK_question:              return Key::SLASH;              //0x0000003F
        case XK_backslash:             return Key::BACKSLASH;          //0x0000005C
        case XK_bar:                   return Key::BACKSLASH;          //0x0000007C
        case XK_bracketleft:           return Key::LBRACKET;           //0x0000005B
        case XK_braceleft:             return Key::LBRACKET;           //0x0000007B
        case XK_bracketright:          return Key::RBRACKET;           //0x0000005D
        case XK_braceright:            return Key::RBRACKET;           //0x0000007D
        case XK_Shift_L:               return Key::LSHIFT;             //0x0000FFE1
        case XK_Shift_R:               return Key::RSHIFT;             //0x0000FFE2
        case XK_Control_L:             return Key::LCTRL;              //0x0000FFE3
        case XK_Control_R:             return Key::RCTRL;              //0x0000FFE4
        case XK_Alt_L:                 return Key::LALT;               //0x0000FFE9
        case XK_Meta_L:                return Key::LALT;               //0x0000FFE7
        case XK_Alt_R:                 return Key::RALT;               //0x0000FFEA
        case XK_Meta_R:                return Key::RALT;               //0x0000FFE8
        case XK_Super_L:               return Key::LSYSTEM;            //0x0000FFEB
        case XK_Super_R:               return Key::RSYSTEM;            //0x0000FFEC
        case XK_Menu:                  return Key::MENU;               //0x0000FF67
        case XK_Sys_Req:               return Key::PRINT_SCREEN;       //0x0000FF15
        case XK_Print:                 return Key::PRINT_SCREEN;       //0x0000FF61
        case XK_Scroll_Lock:           return Key::SCROLL_LOCK;        //0x0000FF14
        case XK_Pause:                 return Key::PAUSE_BREAK;        //0x0000FF13
        case XK_Break:                 return Key::PAUSE_BREAK;        //0x0000FF6B
        case XK_Insert:                return Key::INSERT;             //0x0000FF63
        case XK_Delete:                return Key::DEL;                //0x0000FFFF
        case XK_Home:                  return Key::HOME;               //0x0000FF50
        case XK_End:                   return Key::END;                //0x0000FF57
        case XK_Page_Up:               return Key::PAGE_UP;            //0x0000FF55
        case XK_Page_Down:             return Key::PAGE_DOWN;          //0x0000FF56
        case XK_Up:                    return Key::UP;                 //0x0000FF52
        case XK_Down:                  return Key::DOWN;               //0x0000FF54
        case XK_Left:                  return Key::LEFT;               //0x0000FF51
        case XK_Right:                 return Key::RIGHT;              //0x0000FF53
        case XK_KP_0:                  return Key::NUMPAD_0;           //0x0000FFB0
        case XK_KP_1:                  return Key::NUMPAD_1;           //0x0000FFB1
        case XK_KP_2:                  return Key::NUMPAD_2;           //0x0000FFB2
        case XK_KP_3:                  return Key::NUMPAD_3;           //0x0000FFB3
        case XK_KP_4:                  return Key::NUMPAD_4;           //0x0000FFB4
        case XK_KP_5:                  return Key::NUMPAD_5;           //0x0000FFB5
        case XK_KP_6:                  return Key::NUMPAD_6;           //0x0000FFB6
        case XK_KP_7:                  return Key::NUMPAD_7;           //0x0000FFB7
        case XK_KP_8:                  return Key::NUMPAD_8;           //0x0000FFB8
        case XK_KP_9:                  return Key::NUMPAD_9;           //0x0000FFB9
        case XK_KP_Add:                return Key::ADD;                //0x0000FFAB
        case XK_KP_Subtract:           return Key::SUBTRACT;           //0x0000FFAD
        case XK_KP_Multiply:           return Key::MULTIPLY;           //0x0000FFAA
        case XK_KP_Divide:             return Key::DIVIDE;             //0x0000FFAE
        case XK_KP_Decimal:            return Key::DECIMAL;            //0x0000FFAE
        case XK_Num_Lock:              return Key::NUMLOCK;            //0x0000FF7F
        case XK_KP_Insert:             return Key::NUMPAD_INSERT;      //0x0000FF9E
        case XK_KP_Delete:             return Key::NUMPAD_DEL;         //0x0000FF9F
        case XK_KP_Home:               return Key::NUMPAD_HOME;        //0x0000FF95
        case XK_KP_End:                return Key::NUMPAD_END;         //0x0000FF9C
        case XK_KP_Page_Up:            return Key::NUMPAD_PAGE_UP;     //0x0000FF9A
        case XK_KP_Page_Down:          return Key::NUMPAD_PAGE_DOWN;   //0x0000FF9B
        case XK_KP_Up:                 return Key::NUMPAD_UP;          //0x0000FF97
        case XK_KP_Down:               return Key::NUMPAD_DOWN;        //0x0000FF99
        case XK_KP_Left:               return Key::NUMPAD_LEFT;        //0x0000FF96
        case XK_KP_Right:              return Key::NUMPAD_RIGHT;       //0x0000FF98
        case XK_KP_Enter:              return Key::ENTER;              //0x0000FF8D
        case XK_KP_Begin:              return Key::CLEAR;              //0x0000FF9D
        case XF86XK_AudioPlay:         return Key::MEDIA_PLAY_PAUSE;   //0x1008FF14
        case XF86XK_AudioStop:         return Key::MEDIA_STOP;         //0x1008FF15
        case XF86XK_AudioPrev:         return Key::MEDIA_PREVIOUS;     //0x1008FF16
        case XF86XK_AudioNext:         return Key::MEDIA_NEXT;         //0x1008FF17
        case XF86XK_AudioRaiseVolume:  return Key::VOLUME_UP;          //0x1008FF13
        case XF86XK_AudioLowerVolume:  return Key::VOLUME_DOWN;        //0x1008FF11
        case XF86XK_AudioMute:         return Key::MUTE;               //0x1008FF12

        //The invalid key is returned if an unrecognized keycode is provided.
        default:
            logError( ( boost::format( "Unrecognized keycode: 0x%|08X|" ) % lower ).str() );
            return Key::INVALID;
        }
    }

    //TEMP
    /*
    void logKeyEvent( const XKeyEvent& xKeyEvent ) {
        logDetail(
            (
                boost::format(
                    "XKeyEvent {\n"
                    "    int type = %d;\n"
                    "    unsigned long serial = %d;\n"
                    "    Bool send_event = %d;\n"
                    "    Display *display = 0x%016X;\n"
                    "    Window window = %d;\n"
                    "    Window root = %d;\n"
                    "    Window subwindow = %d;\n"
                    "    Time time = %d;\n"
                    "    int x = %d, y = %d;\n"
                    "    int x_root = %d, y_root = %d;\n"
                    "    unsigned int state = %d;\n"
                    "    unsigned int keycode = %d;\n"
                    "    Bool same_screen = %d;\n"
                    "}"
                )
                % xKeyEvent.type
                % xKeyEvent.serial
                % xKeyEvent.send_event
                % reinterpret_cast<uintptr_t>( xKeyEvent.display )
                % xKeyEvent.window
                % xKeyEvent.root
                % xKeyEvent.subwindow
                % xKeyEvent.time
                % xKeyEvent.x
                % xKeyEvent.y
                % xKeyEvent.x_root
                % xKeyEvent.y_root
                % xKeyEvent.state
                % xKeyEvent.keycode
                % xKeyEvent.same_screen
            ).str()
        );
    }
    */

    //TEMP
    /*
    void logConfigureEvent( const XConfigureEvent& xConfigureEvent ) {
        logDetail(
            (
                boost::format(
                    "XConfigureEvent {\n"
                    "    int type = %d;\n"
                    "    unsigned long serial = %d;\n"
                    "    Bool send_event = %d;\n"
                    "    Display *display = 0x%016X;\n"
                    "    Window event = %d;\n"
                    "    Window window = %d;\n"
                    "    int x = %d, y = %d;\n"
                    "    int width = %d, height = %d;\n"
                    "    int border_width = %d;\n"
                    "    Window above = %d;\n"
                    "    Bool override_redirect = %d;\n"
                    "}"
                )
                % xConfigureEvent.type
                % xConfigureEvent.serial
                % xConfigureEvent.send_event
                % reinterpret_cast<uintptr_t>( xConfigureEvent.display )
                % xConfigureEvent.event
                % xConfigureEvent.window
                % xConfigureEvent.x
                % xConfigureEvent.y
                % xConfigureEvent.width
                % xConfigureEvent.height
                % xConfigureEvent.border_width
                % xConfigureEvent.above
                % xConfigureEvent.override_redirect
            ).str()
        );
    }
    */

    /*
    getWindowBounds
    ---------------

    Description:
        Returns the bounds of the given X11 window, relative to its parent window.

    Arguments:
        display:     Pointer to the X11 Display.
        window:      The X11 window to get the bounds of.

    Returns:
        Bounds2i:    The X11 window's bounds.

    Throws:
        Exception:   If a call to an Xlib function fails.
        XException:  If a call to an Xlib function fails.
    */
    Bounds2i getWindowBounds( Display* display, ::Window window ) {
        //Bounds returned by this function.
        Bounds2i bounds;

        //Variables storing return values for XGetGeometry:
        unsigned int width_return, height_return;

        //Same as above, but we don't make use of these:
        ::Window root_return;
        unsigned int border_width_return, depth_return;

        xerrBegin();
        Status status = XGetGeometry(
            display,
            window,
            &root_return,
            &bounds.minX,
            &bounds.minY,
            &width_return,
            &height_return,
            &border_width_return,
            &depth_return
        );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        //XGetGeometry failed.
        if( status == 0 )
            throw Exception( "XGetGeometry() failed." );

        bounds.setSize( static_cast<int>( width_return ), static_cast<int>( height_return ) );
        return bounds;
    }

    /*
    translateWindowCoords
    ---------------------

    Description:
        Translates the given coordinates, srcCoords, which are relative to the top-left corner of one X11 window, src, and returns new coordinates which are relative to the top-left corner of
        another X11 window, dst.

        If src is the root window and dst is the inner window, this function translates from screen coordinates to window coordinates.
        If src is the inner window and dst is the root window, this function translates from window coordinates to screen coordinates.

    Arguments:
        src:          The window that srcCoords are relative to.
        dst:          The window that the returned coordinates should be relative to.
        srcCoords:    The coordinates to translate, relative to the top-left corner of src.

    Returns:
        Point2i:      The translated coordinates, relative to the top-left corner of dst.

    Throws:
        Exception:   If a call to an Xlib function fails.
        XException:  If a call to an Xlib function fails.
    */
    Point2i translateWindowCoords( Display* display, ::Window src, ::Window dst, Point2i srcCoords ) {
        ::Window childReturn;  //Unused.
        xerrBegin();
        Bool rv = XTranslateCoordinates(
            display,
            src,
            dst,
            srcCoords.x,
            srcCoords.y,
            &srcCoords.x,
            &srcCoords.y,
            &childReturn
        );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        if( rv == True ) { return srcCoords;       } //The window is on this screen.
        else             { return Point2i( 0, 0 ); } //The window is not on this screen.
    }
}

namespace Brimstone {
namespace Private {

std::mutex               XWindow::m_windowsMutex;
XWindow::XWinToWindowMap XWindow::m_windowMap;




/*
XWindow::XWindow
----------------

Description:
    Constructor. Initializes members to defaults.

Arguments:
    N/A

Returns:
    N/A
*/
XWindow::XWindow() :
    m_window( None ),
    m_rootWindow( None ),
    m_topLevelWindow( None ),
    m_inputContext(),
    m_mapped( false ),
    m_shaded( false ),
    m_extents( 0, 0, 0, 0 ),
    m_normalBounds( m_bounds ),
    m_pendingResize( false ),
    m_pendingResizeBounds( 0, 0, 0, 0 ),
    m_pendingFocus( false ) {
}

/*
XWindow::~XWindow
-----------------

Description:
    Destructor. Closes the window if it is open.
    Any exceptions that are generated will be handled by the uncaught exception handler.

Arguments:
    N/A

Returns:
    N/A
*/
XWindow::~XWindow() {
    try {
        close();
    } catch( const IException& e ) {
        uncaughtException( e );
    }
}

/*
XWindow::open
-------------

Description:
    Opens the window.

Arguments:
    N/A

Returns:
    N/A

Throws:
    Exception:  If a call to an Xlib function fails.
*/
void XWindow::open() {
    //Window is already open; nothing to do:
    if( m_window != None )
        return;

    //Initialize X11 if we haven't already
    XShared::init();

    int x = m_bounds.mins.x,
        y = m_bounds.mins.y,
        w = m_bounds.getWidth(),
        h = m_bounds.getHeight();

    ::XVisualInfo* const vi         = XShared::getVisualInfo();
    ::Display* const     display    = XShared::getDisplay();
    ::Window             rootWindow = XShared::getRootWindow();

    //Set window attributes.
    //NOTE:
    //    There's no need to zero out the entire structure - we only set the attributes we need to, then specify which values are set via the valuemask argument specified alongside the
    //    attributes argument.
    //NOTE:
    //    The background_pixmap attribute is set in the OpenGL wiki's window creation example, but its corresponding mask bit isn't provided in the valuemask argument to XCreateWindow. I
    //    commented it out because setting this appears to not make any difference.
    XSetWindowAttributes swa;
    swa.colormap     = XShared::getColormap();
    //swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask   = WINDOW_ATTRIBUTES_EVENT_MASK;

    //Create the window.
    xerrBegin();
    m_window = XCreateWindow(
        display,
        rootWindow,
        x,
        y,
        w,
        h,
        5,
        vi->depth,
        InputOutput,
        vi->visual,
        WINDOW_ATTRIBUTES_VALUEMASK,
        &swa
    );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    if( m_window == None )
        throw Exception( "XCreateWindow() failed." );

    //Set initial values for the root window and top level window.
    //NOTE:
    //    We assume our own window is the top level window at first. In practice though, the top level window will almost always be a window created by the system's window manager, which
    //    contains our window as well as various decorations (e.g. borders, minimize, restore, maximize, and close buttons, etc). We'll wait until we get a notification that the window has
    //    been reparented to determine the actual top-level window.
    m_rootWindow     = rootWindow;
    m_topLevelWindow = m_window;

    //Set the window title:
    xerrBegin();
    XStoreName( display, m_window, m_title.c_str() );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    //Create an input context.
    //We need this to translate key press events to typed characters.
    ::XIM inputMethod = XShared::getInputMethod();
    if( inputMethod != nullptr ) {
        m_inputContext.create(
            inputMethod,
            XNClientWindow, m_window,
            XNFocusWindow,  m_window,
            XNInputStyle,   XIMPreeditNothing | XIMStatusNothing
        );
    }

    //X doesn't add decorations to a created Window - the system's Window Manager is in charge of this.
    //One of these decorations added by the Window Manager happens to be the close button.
    //By default, the close button destroys the Window and closes the Display, bypassing the application that created the Window completely. This will cause the program to crash, since it is
    //still using these resources. We need to take advantage of an extension to transfer this responsibility from the Window Manager to our application so we can handle it appropriately. By
    //setting the WM_PROTOCOLS property to use the WM_DELETE_WINDOW atom, we're telling it to send a ClientMessage event whenever the close button is clicked instead of doing its default
    //behavior.
    Status status;
    xerrBegin();
    status = XSetWMProtocols( display, m_window, &XShared::m_atomWmDeleteWindow, 1 );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
    if( status == 0 )
        throw Exception( "XSetWMProtocols() failed." );

    //Window managers often ignore whatever position you tell it to create the window at (in XCreateWindow) and then place it where it feels like it should go.
    //We don't want it to do this, so before displaying the window we set its size hints to tell it as much.
    //NOTE:
    //    There's no need to zero out the entire structure - we only set the attributes we need to, then specify which values are set via the XSizeHint structure's "flags" member.
    //    PPosition  = Program specified position, USPosition = User specified position,
    //    PSize      = Program specified size,     USSize     = User specified size.
    //    The presence of either PPosition or USPosition seems to imply that "x" and "y" are set.
    //    The presence of either PSize or USSize seems to imply that "width" and "height" are set.
    //    It's unknown if both PPosition and USPosition, or PSize and USSize can be set as flags at the same time.
    //    According to Xlib docs, x, y, width, and height are all obsolete, but it looks like other projects like SFML and SDL are still using it, so
    //    whatever.
    XSizeHints xsh;
    xsh.flags  = XSIZEHINTS_FLAGS_BOUNDS;
    xsh.x      = x;
    xsh.y      = y;
    xsh.width  = w;
    xsh.height = h;

    //If the window isn't supposed to be resizable, make sure this is specified in the size hints:
    if( !m_resizable ) {
        xsh.flags      |=  XSIZEHINTS_FLAGS_MINMAXSIZE;
        xsh.min_width   =
        xsh.max_width   = m_bounds.getWidth();
        xsh.min_height  =
        xsh.max_height  = m_bounds.getHeight();
    }

    xerrBegin();
    XSetWMNormalHints( display, m_window, &xsh );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    //We still have to wait for the window manager to set the extents so we know where to correctly position the window.
    m_pendingResize = true;
    m_pendingResizeBounds = m_bounds;

    //Add the window to the map
    {
        std::lock_guard< std::mutex > l( m_windowsMutex );
        m_windowMap.emplace( m_window, *this );
    }

    //Display the window to the user
    xerrBegin();
    XMapWindow( display, m_window );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    //TODO: Apply window states set on the Window prior to the call to open() like fullscreen, maximize, minimize, shaded, borderless, and visible here.
}

/*
XWindow::close
--------------

Description:
    Closes the window.

Arguments:
    N/A

Returns:
    N/A
*/
void XWindow::close() {
    //Clear pending actions:
    m_pendingFocus = false;
    m_pendingResizeBounds.zero();
    m_pendingResize = false;

    //Zero out normal bounds and extents:
    m_normalBounds.zero();
    m_extents.zero();

    //Window no longer mapped:
    m_mapped = false;

    //Destroy the input context:
    m_inputContext.destroy();

    //Clear references to top level window and root window:
    m_topLevelWindow = None;
    m_rootWindow     = None;

    //Destroy the window:
    if( m_window != None ) {
        xerrBegin();
        XDestroyWindow( XShared::getDisplay(), m_window );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        //Remove the window from the map:
        {
            std::lock_guard< std::mutex > l( m_windowsMutex );
            m_windowMap.erase( m_window );
        }

        //If this is the last window being closed, clean up shared X11 resources (display, etc):
        if( m_windowMap.empty() )
            XShared::destroy();

        m_window = None;
    }
}

/*
XWindow::isOpen
---------------

Description:
    Returns true if the window has been opened, false otherwise.

Arguments:
    N/A

Returns:
    bool:      true if the window is open, false otherwise.
*/
bool XWindow::isOpen() const {
    return m_window != None;
}

/*
XWindow::frame
--------------

Description:
    The application should call this function once per frame.
    Any per-frame window operations are performed here.

Arguments:
    N/A

Returns:
    N/A
*/
void XWindow::frame() {
    if( m_keepCursorCentered ) {
        //Calculate the coordinates for the center of the window, relative to the origin of the window:
        Point2i center(
            ( m_bounds.maxX - m_bounds.minX ) >> 1, //x >> 1 == x / 2
            ( m_bounds.maxY - m_bounds.minY ) >> 1  //x >> 1 == x / 2
        );

        //If our cursor is already at the center of the window, there's no need to call XWarpPointer() to move it there:
        if( m_cursorPos == center )
            return;

        //Center the cursor on the window:
        //NOTE:
        //    If dest_w is not None, then dest_x and dest_y are coordinates relative to the origin of dest_w.
        //    If src_w is None, then src_x, src_y, src_width, and src_height are ignored.
        xerrBegin();
        XWarpPointer(
            XShared::getDisplay(),  //display
            None,                   //src_w
            m_window,               //dest_w
            0,                      //src_x
            0,                      //src_y
            0,                      //src_width
            0,                      //src_height
            center.x,               //dest_x 
            center.y                //dest_y 
        );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
    }
}

/*
XWindow::peekEvent
------------------

Description:
    If a Brimstone WindowEvent is available, outputs the event and returns true. Otherwise, returns false.

    Specifically, processes all queued X11 events, which potentially generates Brimstone WindowEvents.
    Aferwards, if at least one Brimstone WindowEvent is present on the queue, removes the front event from the queue, outputs it to eventOut, and returns true.
    Otherwise, eventOut is left unchanged and false is returned.

Arguments:
    eventOut:  The Brimstone WindowEvent is output here.

Returns:
    bool:      true if a Brimstone WindowEvent was returned, false otherwise.
               false is always returned if the window is not open.
*/
bool XWindow::peekEvent( WindowEvent& eventOut ) {
    //Window not open
    if( m_window == None )
        return false;

    //Nothing in event queue
    if( m_eventQueue.empty() ) {
        ::Display* const display = XShared::getDisplay();
        //Process all messages currently in the application's message queue
        XEvent event, nextEvent;
        int i = XPending( display );
        while( i > 0 ) {
            //Grab the next event.
            XNextEvent( display, &event );
            --i;

            //If this event was a KeyRelease, there's a possibility that this event and the event that follow it belong to a KeyRelease, KeyPress event pair generated by auto-repeat rather
            //than from the key actually being pressed/released by the user.
            //We want to be able to identify these auto-repeated KeyPress/KeyRelease events so we can filter them out or identify them as such.
            if( event.type == KeyRelease ) {
                do {
                    //Grab the next event if one exists.
                    if( i > 0 ) {
                        XNextEvent( display, &nextEvent );
                        --i;
                    //If there is no next event, process the event as we normally would, then exit the inner loop.
                    } else {
                        mainProc( event, false );
                        break;
                    }

                    //If the next event is...
                    if(
                        nextEvent.type         == KeyPress           &&  //a KeyPress event,
                        nextEvent.xkey.window  == event.xkey.window  &&  //occurred on the same window as the first event,
                        nextEvent.xkey.serial  == event.xkey.serial  &&  //has the same serial number as the first event,
                        nextEvent.xkey.keycode == event.xkey.keycode     //and was for the same key as the first event,
                    //...then the first and second events are part of an auto-repeated keypress.
                    ) {
                        //If key repeat is enabled, ignore the KeyRelease event, but process the KeyPress event, making sure to mark it as a repeated key event.
                        //Otherwise, ignore both events.
                        if( m_keyRepeat )
                            mainProc( nextEvent, true );
                        //Exit the inner loop:
                        break;
                    //This event isn't part of an auto-repeated keypress.
                    } else {
                        //Process the event as we normally would.
                        mainProc( event, false );
                        //The next event could be a KeyRelease event; if so, it can be part of a key KeyRelease, KeyPress pair.
                        //Swap the next event with the current event.
                        event = nextEvent;
                    }
                } while( event.type == KeyRelease );
            //Not a KeyRelease event, process as usual.
            } else {
                mainProc( event, false );
            }
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

/*
XWindow::getEvent
-----------------

Description:
    Blocks until a Brimstone WindowEvent is available, and then outputs the event and returns true.

    Specifically, processes queued X11 window events, which potentially generates Brimstone WindowEvents.
    Afterwards, if at least one Brimstone WindowEvent is present on the queue, removes the front event from the queue, outputs it to eventOut, and returns true.
    Otherwise, blocks until more X11 window events are available and the process is repeated.

Arguments:
    eventOut:  The Brimstone WindowEvent is output here.

Returns:
    bool:      true if a WindowEvent was returned, false otherwise.
               false is always returned if the window is not open.
*/
bool XWindow::getEvent( WindowEvent& eventOut ) {
    //Wait for an event to arrive in the window's event queue if it's currently empty.
    //Process messages from the application's message queue, blocking until a message arrives if necessary.
    //The WindowImpl::getEvent() call returns true if a message was processed, or false if the application is shutting down.
    ::Display* const display = XShared::getDisplay();
    XEvent event, nextEvent;
    while( m_eventQueue.empty() ) {

        //Wait for the next X11 event to arrive.
        XNextEvent( display, &event );

        //If this event was a KeyRelease, there's a possibility that this event and the event that follow it belong to a KeyRelease, KeyPress event pair generated by auto-repeat rather
        //than from the key actually being pressed/released by the user.
        //We want to be able to identify these auto-repeated KeyPress/KeyRelease events so we can filter them out or identify them as such.
        if( event.type == KeyRelease ) {
            int i = XPending( display );
            do {
                //Grab the next event if one exists.
                if( i > 0 ) {
                    XNextEvent( display, &nextEvent );
                    --i;
                //If there is no next event, process the event as we normally would, then exit the inner loop.
                } else {
                    mainProc( event, false );
                    break;
                }

                //If the next event is...
                if(
                    nextEvent.type         == KeyPress           &&  //a KeyPress event,
                    nextEvent.xkey.window  == event.xkey.window  &&  //occurred on the same window as the first event,
                    nextEvent.xkey.serial  == event.xkey.serial  &&  //has the same serial number as the first event,
                    nextEvent.xkey.keycode == event.xkey.keycode     //and was for the same key as the first event,
                //...then the first and second events are part of an auto-repeated keypress.
                ) {
                    //If key repeat is enabled, ignore the KeyRelease event, but process the KeyPress event, making sure to mark it as a repeated key event.
                    //Otherwise, ignore both events.
                    if( m_keyRepeat )
                        mainProc( nextEvent, true );
                    //Exit the inner loop:
                    break;
                //This event isn't part of an auto-repeated keypress.
                } else {
                    //Process the event as we normally would.
                    mainProc( event, false );
                    //The next event could be a KeyRelease event; if so, it can be part of a key KeyRelease, KeyPress pair.
                    //Swap the next event with the current event.
                    event = nextEvent;
                }
            } while( event.type == KeyRelease );
        //Not a KeyRelease event, process as usual.
        } else {
            mainProc( event, false );
        }
    }

    //Pop a message from the queue
    eventOut = m_eventQueue.front();
    m_eventQueue.pop();
    return true;
}

/*
XWindow::mainProc
-----------------

Description:
    Processes the given X11 window event.
    Calls windowProc on the Window associated with the event, or silently ignores the event if no such window could be found.

Arguments:
    xEvent:    The X11 window event.
    repeated:  true if this event is an auto-repeated keypress event, false otherwise.

Returns:
    N/A
*/
void XWindow::mainProc( XEvent& xEvent, const bool repeated ) {
    //Locate what XWindow this belongs to
    auto it = m_windowMap.find( xEvent.xany.window );
    if( it == m_windowMap.end() )
        return;

    it->second.windowProc( xEvent, repeated );
}

/*
XWindow::windowProc
-------------------

Description:
    Processes an X11 window event for this window.

Arguments:
    xEvent:    The X11 window event.
    repeated:  true if this event is an auto-repeated keypress event, false otherwise.

Returns:
    N/A

Throws:
    UnexpectedResultException:  If the X11 event would have generated a Text event, but the UTF-8 encoding of the typed character was longer than 4 bytes.
*/
void XWindow::windowProc( XEvent& xEvent, const bool repeated ) {
    switch( xEvent.type ) {
        //Mouse move
        case MotionNotify: {
            auto p = getCursorPosFromXEvent( xEvent.xmotion );

            if( p != m_cursorPos ) {
                m_cursorPos = p;

                unsigned int state = xEvent.xmotion.state;

                WindowEvent e;
                e.type             = WindowEventType::MouseMove;
                e.mouseMove.x      = p.x;
                e.mouseMove.y      = p.y;
                e.mouseMove.ctrl   = state & ControlMask;
                e.mouseMove.alt    = state & Mod1Mask;
                e.mouseMove.shift  = state & ShiftMask;
                e.mouseMove.system = state & Mod4Mask;

                pushEvent( e );
            }
        } break;
        //Button down
        case ButtonPress: {
            int button = xEvent.xbutton.button;
            auto p = getCursorPosFromXEvent( xEvent.xbutton );

            unsigned int state = xEvent.xbutton.state;

            //Vertical scrolling
            if( button == Button4 || button == Button5 ) {
                WindowEvent e;
                e.type               = WindowEventType::MouseVScroll;
                e.mouseScroll.scroll = button == Button4 ? 1.0f : -1.0f;
                e.mouseScroll.x      = p.x;
                e.mouseScroll.y      = p.y;
                e.mouseScroll.ctrl   = state & ControlMask;
                e.mouseScroll.alt    = state & Mod1Mask;
                e.mouseScroll.shift  = state & ShiftMask;
                e.mouseScroll.system = state & Mod4Mask;

                pushEvent( e );
            //Horizontal scrolling (note: direction is assumed and needs to be tested somehow)
            } else if( button == 6 || button == 7 ) {
                WindowEvent e;
                e.type               = WindowEventType::MouseHScroll;
                e.mouseScroll.scroll = button == 6 ? -1.0f : 1.0f;
                e.mouseScroll.x      = p.x;
                e.mouseScroll.y      = p.y;
                e.mouseScroll.ctrl   = state & ControlMask;
                e.mouseScroll.alt    = state & Mod1Mask;
                e.mouseScroll.shift  = state & ShiftMask;
                e.mouseScroll.system = state & Mod4Mask;

                pushEvent( e );
            //Actual buttons
            } else {
                WindowEvent e;
                e.type         = WindowEventType::MouseDown;
                e.mouse.button = xButtonToMouseButton( button );
                e.mouse.x      = p.x;
                e.mouse.y      = p.y;
                e.mouse.ctrl   = state & ControlMask;
                e.mouse.alt    = state & Mod1Mask;
                e.mouse.shift  = state & ShiftMask;
                e.mouse.system = state & Mod4Mask;

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

            unsigned int state = xEvent.xbutton.state;

            WindowEvent e;
            e.type         = WindowEventType::MouseUp;
            e.mouse.button = xButtonToMouseButton( button );
            e.mouse.x      = p.x;
            e.mouse.y      = p.y;
            e.mouse.ctrl   = state & ControlMask;
            e.mouse.alt    = state & Mod1Mask;
            e.mouse.shift  = state & ShiftMask;
            e.mouse.system = state & Mod4Mask;

            pushEvent( e );
        } break;
        //Mouse entered window
        case EnterNotify: {
            //Ignore EnterNotify events generated by pointer grabs / ungrabs
            if( xEvent.xcrossing.mode != NotifyNormal )
                break;

            WindowEvent e;
            e.type = WindowEventType::MouseEnter;

            pushEvent( e );
        } break;
        //Mouse left window
        case LeaveNotify: {
            //Ignore LeaveNotify events generated by pointer grabs / ungrabs
            if( xEvent.xcrossing.mode != NotifyNormal )
                break;

            WindowEvent e;
            e.type = WindowEventType::MouseLeave;
            
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
            e2.text.repeat = repeated;

            KeySym keySym;

            //Note: we're only interested in the KeySym at this point.
            //The translated character is ignored
            XLookupString( &xEvent.xkey, translatedCharacter, 4, &keySym, nullptr );

            unsigned int state = xEvent.xkey.state;

            //Dispatch a keydown event
            WindowEvent e;
            e.type       = WindowEventType::KeyDown;
            e.key.key    = xKeySymToKey( keySym );
            e.key.ctrl   = state & ControlMask;
            e.key.alt    = state & Mod1Mask;
            e.key.shift  = state & ShiftMask;
            e.key.system = state & Mod4Mask;
            e.key.repeat = repeated;

            pushEvent( e );

            //If XFilterEvent() returns True, then the window's input method (created by XOpenIM() in XWindow::open()) has filtered out this KeyPress event.
            //Our program should respond to this by not processing it further (i.e. we won't generate a Text event).
            if( XFilterEvent( &xEvent, None ) ) {
                logDetail( "Filtered a keypress event." );
                break;
            }

            int count;
#ifdef X_HAVE_UTF8_STRING
            ::XIC inputContext = m_inputContext.get();
            if( inputContext != nullptr ) {
                count = Xutf8LookupString( inputContext, &xEvent.xkey, translatedCharacter, 4, nullptr, nullptr );
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

            unsigned int state = xEvent.xkey.state;

            //Dispatch a KeyUp event
            WindowEvent e;
            e.type       = WindowEventType::KeyUp;
            e.key.key    = xKeySymToKey( keySym );
            e.key.ctrl   = state & ControlMask;
            e.key.alt    = state & Mod1Mask;
            e.key.shift  = state & ShiftMask;
            e.key.system = state & Mod4Mask;
            e.key.repeat = false;

            pushEvent( e );
        } break;
        //Window moved and/or resized
        case ConfigureNotify: {
            //New position and size of the window
            Point2i pos( xEvent.xconfigure.x, xEvent.xconfigure.y );
            Size2i  size( xEvent.xconfigure.width, xEvent.xconfigure.height );

            //Moved
            //NOTE:
            //    When a new X11 Window is created, if the window wants decorations then window managers will automatically reparent that window to a new window that contains the decorations.
            //    The x and y coordinates of a ConfigureNotify event are relative to the parent window, so if our window has decorations, these x and y coordinates will always be 0, 0.
            //    This also should mean that when a window with decorations is moved, we *shouldn't* be getting ConfigureNotify events for our window, since it hasn't moved relative to its
            //    parent window. Oddly though, it *does* receives ConfigureNotify events when moved, whose x and y coordinates are the upper-left corner of our window (the "client area" if you
            //    will) relative to the *screen*, which goes against our expectations. Logging these events reveals that such events have the "send_event" member set to True, which makes me
            //    suspect that the window manager may be sending these events. When resizing, two events are received - one where send_event is set to False and x and y are 0, and the
            //    aformentioned one where send_event is set to True and x and y are the screen coordinates of the upper-left corner of the window's client area.
            if( xEvent.xconfigure.send_event != False && pos != m_bounds.getPosition() ) {
                //Update position
                m_bounds.setPosition( pos );

                //Update normal bounds' position if not maximized or fullscreen:
                if( isRestored() )
                    m_normalBounds.setPosition( pos );

                //Push event
                WindowEvent e;
                e.type   = WindowEventType::Move;
                e.move.x = pos.x;
                e.move.y = pos.y;

                pushEvent( e );
            }

            //Resized
            if( size != m_bounds.getSize() ) {
                //Update size
                m_bounds.setSize( size );

                //Update normal bounds' size if not maximized:
                if( isRestored() )
                    m_normalBounds.setSize( size );

                //Push event
                WindowEvent e;
                e.type     = WindowEventType::Resize;
                e.resize.w = size.w;
                e.resize.h = size.h;

                pushEvent( e );
            }
        } break;
        //Window gains keyboard focus
        case FocusIn: {
            //Update focus state:
            m_focused = true;

            //Push focus event:
            WindowEvent e;
            e.type = WindowEventType::Focus;
            pushEvent( e );
        } break;
        //Window loses keyboard focus
        case FocusOut: {
            //Update focus state:
            m_focused = false;

            //Push blur event:
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
            if( xEvent.xclient.format == 32 && static_cast<Atom>( xEvent.xclient.data.l[0] ) == XShared::m_atomWmDeleteWindow ) {
                WindowEvent e;
                e.type = WindowEventType::Close;

                pushEvent( e );
            }
        } break;
        //Window property was updated
        case PropertyNotify: {
            //TEMP: Print property name
            //char* name = XGetAtomName( XShared::getDisplay(), xEvent.xproperty.atom );
            //logInfo( ( boost::format( "PropertyNotify: %s" ) % name ).str() );
            //XFree( name );

            Atom a = xEvent.xproperty.atom;

            //The _NET_WM_STATE property was updated (e.g. the window was put into / taken out of fullscreen, maximized / unmaximized, etc.)
            if(      a == XShared::m_atomNetWmState      ) { updateWindowState();  }
            //The _NET_FRAME_EXTENTS property was updated (the size of the window's decorations changed).
            else if( a == XShared::m_atomNetFrameExtents ) { updateFrameExtents(); }
        } break;
        case ReparentNotify: {
            updateWindowHierarchy();
        } break;
        case MapNotify: {
            m_mapped = true;
            if( m_pendingFocus )
                focus();
        } break;
        case UnmapNotify: {
            m_mapped = false;
        } break;
        //Events enabled by specifying StructureNotifyMask that we're not interested in
        case CirculateNotify:
        case DestroyNotify:
        case GravityNotify:
            break;
        //Unhandled event
        default: {
            logError( ( boost::format( "Unhandled event: %1%" ) % xEvent.type ).str() );
        } break;
    }
}

/*
XWindow::setTitle
-----------------

Description:
    Sets the text that appears in the window's titlebar, among other places.

Arguments:
    title:  The title to set on the window.

Returns:
    N/A
*/
void XWindow::setTitle( const ustring& title ) {
    //If the window isn't open yet, just set what its title should be:
    if( m_window == None ) {
        BaseWindowImpl::setTitle( title );
        return;
    }

    //NOTE:
    //    WM_NAME (identified by the standard Atom XA_WM_NAME) is the legacy window title property, encoded with a locale-specific encoding (as requested via XTextStyle).
    //    _NET_WM_NAME (indentified by the Atom we look up XShared::m_atomNetWmName) is a more modern window title property, encoded as UTF-8 (as requested via XUTF8StringStyle).
    //    Modern window managers prefer _NET_WM_NAME over WM_NAME when determining the window title.
    //    We set both properties to cover our bases.
    XTextProperty xtp;
    int status;
    ::Display* const display = XShared::getDisplay();

    //HACK: Using string's internal buffer directly...
    //NOTE: The const_cast from const char* to char* is necessary here because XmbTextListToTextProperty() and Xutf8TextListToTextProperty() expect a char**, not a const char**,
    //despite the fact that neither of the functions modify the strings in the given list.
    char* pszTitle = const_cast<char*>( title.c_str() );

    //Set the WM_NAME property:
    status = XmbTextListToTextProperty( display, &pszTitle, 1, XTextStyle, &xtp );
    if( status == XNoMemory || status == XLocaleNotSupported ) {
        throw Exception( "XmbTextListToTextProperty() failed." );
    } else {
        //XmbTextListToTextProperty allocates memory in xtp.value. Automatically free it after we're done using it:
        std::unique_ptr< unsigned char[], int(*)(void*) > uptr( xtp.value, &XFree );

        if( status != Success )
            logWarning( "XmbTextListToTextProperty() couldn't convert some characters." );

        //Set the property:
        xerrBegin();
        XSetTextProperty( display, m_window, &xtp, XA_WM_NAME );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
    }

    //Set the _NET_WM_NAME property (if Xlib was built with UTF-8 string support):
#ifdef X_HAVE_UTF8_STRING
    status = Xutf8TextListToTextProperty( display, &pszTitle, 1, XUTF8StringStyle, &xtp );
    if( status == XNoMemory || status == XLocaleNotSupported ) {
        throw Exception( "Xutf8TextListToTextProperty() failed." );
    } else {
        //Xutf8TextListToTextProperty allocates memory in xtp.value. Automatically free it after we're done using it:
        std::unique_ptr< unsigned char[], int(*)(void*) > uptr( xtp.value, &XFree );

        if( status != Success )
            logWarning( "Xutf8TextListToTextProperty() couldn't convert some characters." );

        //Set the property:
        xerrBegin();
        XSetTextProperty( display, m_window, &xtp, XShared::m_atomNetWmName );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
    }
#endif //X_HAVE_UTF8_STRING

    //Update tracking of title:
    BaseWindowImpl::setTitle( title );
}

/*
XWindow::setBorderless
----------------------

Description:
    Sets whether or not the window is borderless.
    A borderless window does not have decorations (borders, a titlebar, minimize/maximize/restore/close buttons, etc) and consists only of its client area.

Arguments:
    borderless:  true if the window should be borderless, false otherwise.

Returns:
    N/A
*/
void XWindow::setBorderless( const bool borderless ) {
    //If we're already at the requested borderless state, there's nothing to do:
    if( m_borderless == borderless )
        return;

    //If the window isn't open yet, just set whether or not it should be borderless:
    if( m_window == None ) {
        BaseWindowImpl::setBorderless( borderless );
        return;
    }

    //Create a PropMotifWmHints objects (which defines the value of the _MOTIF_WM_HINTS property) and set its members appropriately depending on whether we want a borderless window or not.
    //NOTE:
    //    This property is super old at this point (originating from the Motif Window Manager) and not that well documented, but apparently a lot of window managers still honor it and setting
    //    it works better than any other alternative methods I could find at achieving our goal here.
    PropMotifWmHints hints = {
        MWM_HINTS_DECORATIONS,            //flags:       Specify that the decorations field of the hints structure is set.
        0UL,                              //functions:   unused
        borderless ? 0UL : MWM_DECOR_ALL, //decorations: Specify no decorations (0) if borderless, or all decorations (MWM_DECOR_ALL) if not borderless.
        0L,                               //inputMode:   unused
        0UL                               //status:      unused
    };

    //Update the property:
    xerrBegin();
    XChangeProperty(
        XShared::getDisplay(),                      //::Display* display
        m_window,                                   //::Window   w
        XShared::m_atomMotifWmHints,                //::Atom     property
        XShared::m_atomMotifWmHints,                //::Atom     type
        32,                                         //int        format
        PropModeReplace,                            //int        mode
        reinterpret_cast<unsigned char*>( &hints ), //unsigned char *data
        sizeof( hints ) / sizeof( long )
    );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    //Adding or removing decorations moves the client area of the window to a different location on-screen, so we need to counteract that by readjusting the window's bounds.
    //In the case that we're transitioning to borderless, we can set the bounds immediately because we know the new extents will be 0, 0, 0, 0:
    if( borderless ) {
        Bounds2i adjusted(
            Point2i(
                m_bounds.minX + m_extents.minX,
                m_bounds.minY + m_extents.minY
            ),
            m_bounds.getSize()
        );

        setBounds( adjusted );
    //When we're transitioning from borderless to bordered, however, we need to wait for the window to remind us what its extents are again, since they get updated to (0, 0, 0, 0) after the
    //window transitions to borderless.
    //TODO:
    //    The current method I'm using has a drawback - there is a brief but noticeable flicker due to the window's position changing twice - once when it transitions to bordered, and a second
    //    time after the window's new extents are received a short time later and the pending resize triggers.
    //    This flickering can can be probably be avoided by storing the old extents before going into borderless, then after going back into bordered, repositioning the window immediately
    //    using the old extents, but also setting up a pending resize just in case the new extents are different from the old ones for some reason.
    } else {
        m_pendingResize = true;
        m_pendingResizeBounds = m_bounds;
    }

    //Update tracking of borderless state:
    BaseWindowImpl::setBorderless( borderless );
}

/*
XWindow::setResizable
---------------------

Description:
    Sets whether or not the window can be resized by the user.

Arguments:
    resizable:  true if the user can resize the window, false otherwise.

Returns:
    N/A
*/
void XWindow::setResizable( const bool resizable ) {
    //If we're already at the requested resizable state, there's nothing to do:
    if( m_resizable == resizable )
        return;

    //If the window isn't open yet, just set whether or not it should be resizeble:
    if( m_window == None ) {
        BaseWindowImpl::setResizable( resizable );
        return;
    }

    ::Display* display = XShared::getDisplay();
    XSizeHints xsh;

    //Unused.
    long supplied_return;
    
    //Get the window's current window manager normal hints:
    xerrBegin();
    Status status = XGetWMNormalHints( display, m_window, &xsh, &supplied_return );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
    if( status == 0 )
        throw Exception( "XGetWMNormalHints() failed." );

    //Update the hints according to whether the window should be resizable or not:
    if( resizable ) {
        xsh.flags      &= ~XSIZEHINTS_FLAGS_MINMAXSIZE;
    } else {
        xsh.flags      |=  XSIZEHINTS_FLAGS_MINMAXSIZE;
        xsh.min_width   =
        xsh.max_width   = m_bounds.getWidth();
        xsh.min_height  =
        xsh.max_height  = m_bounds.getHeight();
    }

    //Set the window's current window manager normal hints:
    xerrBegin();
    XSetWMNormalHints( display, m_window, &xsh );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    //Update tracking of resizability:
    BaseWindowImpl::setResizable( resizable );
}

/*
XWindow::setVisible
-------------------

Description:
    Sets whether or not the window is visible (maps or unmaps the window).

Arguments:
    visible:  true to show the window, false to hide it.

Returns:
    N/A

Throws:
    XException:  If a call to an Xlib function fails.
*/
void XWindow::setVisible( const bool visible ) {
    //If we're already at the requested level of visibility, there's nothing to do:
    if( m_visible == visible )
        return;

    //If the window isn't open yet, just set whether or not it should be visible:
    if( m_window == None ) {
        BaseWindowImpl::setVisible( visible );
        return;
    }

    //Show or hide the window by mapping or unmapping it, respectively:
    xerrBegin();
    if( visible ) { XMapWindow(   XShared::getDisplay(), m_window ); }
    else          { XUnmapWindow( XShared::getDisplay(), m_window ); }
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    //If showing the window, restore some properties of the window that were lost when it was hidden:
    if( visible ) {
        //De-maximize the window if it was maximized or fullscreen.
        //This is necessary to restore the original position/size of the window below.
        //NOTE:
        //    If the window was fullscreen when it was hidden, when it is shown it will instead be maximized, so we have to check whether or not it was fullscreen as well.
        //    This is undesired behavior.
        if( m_maximized || m_fullscreen )
            setMaximizedInternal( false );

        //Restore the position and size of the window.
        setBounds( m_normalBounds );

        //If the window was maximized when it was hidden, we remaximize it here.
        if( m_maximized )
            setMaximizedInternal( true );

        //If the window was fullscreen when it was hidden, we restore it to its fullscreen state here.
        if( m_fullscreen )
            setFullscreenInternal( true );
    }

    //Update tracking of visibility:
    BaseWindowImpl::setVisible( visible );
}

/*
XWindow::isVisible
------------------

Description:
    Returns whether or not the window is visible (whether or not the window is currently mapped).

Arguments:
    N/A

Returns:
    bool:  true if the window is visible, false otherwise.
*/
/*
bool XWindow::isVisible() const {
    XWindowAttributes xwa;
    xerrBegin();
    int status = XGetWindowAttributes( XShared::getDisplay(), m_window, &xwa );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
    if( status == 0 )
        raise Exception( "XGetWindowAttributes() failed." );
    return xwa.map_state == IsViewable;
}
*/

/*
XWindow::setBounds
------------------

Description:
    Sets the window's bounds (moves and/or resizes the window).

Arguments:
    bounds:  The window's new bounds.

Returns:
    N/A

Throws:
    XException:  If a call to an Xlib function fails.
*/
void XWindow::setBounds( const Bounds2i bounds ) {
    //If the window isn't open yet, just set what the bounds should be:
    if( m_window == None ) {
        BaseWindowImpl::setBounds( bounds );
        return;
    }

    //NOTE:
    //    Our window is likely a descendant of a top-level window managed by the system's window manager, in which case its position is relative to its parent window and almost always (0, 0).
    //    Oddly enough though, calling XMoveResizeWindow doesn't change our window's position relative to its parent window; it remains at (0,0). Instead, it changes the position of the
    //    top-level window, moving it so *its* top-left corner (not the top-left corner of the client area!) is at the given position on screen. Resizing the window sets the size of our window
    //    to the size given to XMoveResizeWindow(), and then the top-level window resizes to a larger size accomodate it.
    //
    //    Our setBounds() method is supposed to position the upper-left corner of *this* window at the mins of the given bounds.
    //    In order to do this, we need to offset the position of the given bounds by the amount this window's top-left corner is offset from the top-level window's top-left corner.
    //    We track what these offsets are in m_extents, which is updated every time the window is reparented.
    Bounds2i adjusted(
        Point2i(
            bounds.minX - m_extents.minX,
            bounds.minY - m_extents.minY
        ),
        bounds.getSize()
    );

    //Resize the window.
    xerrBegin();
    XMoveResizeWindow(
        XShared::getDisplay(),
        m_window,
        adjusted.minX,
        adjusted.minY,
        adjusted.getWidth(),
        adjusted.getHeight()
    );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    //If the window is in its restored state (not minimized, maximized, fullscreen, shaded, etc.), save this as its normal bounds.
    if( isRestored() )
        m_normalBounds = bounds;
}

/*
XWindow::setFullscreen
----------------------

Description:
    Puts the window into or takes the window out of fullscreen mode.

Arguments:
    fullscreen:  true if the window should be fullscreen, false otherwise.

Returns:
    N/A
*/
void XWindow::setFullscreen( const bool fullscreen ) {
    //If we're already at the requested full screen state, there's nothing to do:
    if( m_fullscreen == fullscreen )
        return;

    //If the window isn't open yet, just set whether or not the window should be fullscreen:
    if( m_window == None ) {
        BaseWindowImpl::setFullscreen( fullscreen );
        return;
    }

    setFullscreenInternal( fullscreen );
}

/*
XWindow::setFullscreenInternal
------------------------------

Description:
    Internal method that does the actual work of putting the window into or taking the window out of fullscreen mode.

Arguments:
    fullscreen:  true if the window should be fullscreen, false otherwise.

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::setFullscreenInternal( const bool fullscreen ) {
    //To enable or disable the fullscreen mode on our window, we'll need to request an update of the window state (specifically, its "fullscreen" property) by sending an appropriate
    //ClientMessage to the root window. More information here:
    //    https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html#id-1.6.8
    XEvent event {};                                                                     //Zero-initialize the event.
    event.type = ClientMessage;                                                          //We'll be sending a ClientMessage.
    event.xclient.window = m_window;                                                     //Indicate that this event was generated from our window.
    event.xclient.message_type = XShared::m_atomNetWmState;                              //Update the window state (_NET_WM_STATE).
    event.xclient.format = 32;                                                           //View data as a list of 32-bit values (stored as longs, which may be 32-bit or 64-bit each).
    event.xclient.data.l[0] = fullscreen ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;     //Add or remove properties, depending on the value of fullscreen.
    event.xclient.data.l[1] = static_cast<long>( XShared::m_atomNetWmStateFullscreen );  //The first property to modify. Specifically, the property named by the "_NET_WM_STATE_FULLSCREEN" atom.
                                                                                          //NOTE: The static_cast to long is necessary here because Atom is a typedef to unsigned long.
    event.xclient.data.l[2] = 0;                                                          //The second property to modify. Set to 0 to indicate that we don't want to modify a second property.
    event.xclient.data.l[3] = 1;                                                          //Source indication. 1 means the request to update the window state came from a "normal application",
                                                                                          //as opposed to some other source (e.g. direct user interaction).

    //Send our ClientMessage event off to the root window.
    //NOTE:
    //    I don't really understand the propagate (False) or event mask (SubstructureNotifyMask | SubstructureRedirectMask) arguments to this function.
    //    The documentation says that when propagate is False, it will send the event to every client that is selecting for any of the events specified by the mask.
    //    I assume the window manager is an X11 client which selects for these events, and that's why you need to specify them?
    //    At any rate, it seems both of those arguments are necessary in order for the root window to receive the event.
    xerrBegin();
    Status status = XSendEvent( XShared::getDisplay(), m_rootWindow, False, XSENDEVENT_EVENT_MASK, &event );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
    if( status == 0 )
        throw Exception( "XSendEvent() failed." );
}

/*
XWindow::setMaximized
---------------------

Description:
    Maximizes or unmaximizes the window.

Arguments:
    maximized:  true if the window should be maximized, false otherwise.

Returns:
    N/A
*/
void XWindow::setMaximized( const bool maximized ) {
    //If we're already at the requested maximization state, there's nothing to do:
    if( m_maximized == maximized )
        return;

    //If the window isn't open yet, just set whether or not the window should be maximized:
    if( m_window == None ) {
        BaseWindowImpl::setMaximized( maximized );
        return;
    }

    //The window can't be fullscreen and maximized at the same time. If the window is currently fullscreen, disable it first:
    if( m_fullscreen )
        setFullscreen( false );

    setMaximizedInternal( maximized );
}

/*
XWindow::setMaximizedInternal
-----------------------------

Description:
    Internal method that does the actual work of maximizing or unmaximizing the window.

Arguments:
    maximized:  true if the window should be maximized, false otherwise.

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::setMaximizedInternal( const bool maximized ) {
    //Maximize or unmaximize the window in the same way that we put it into or out of fullscreen.
    //See setFullscreenInternal() for explanatory comments.
    XEvent event {};
    event.type = ClientMessage;
    event.xclient.window = m_window;
    event.xclient.message_type = XShared::m_atomNetWmState;
    event.xclient.format = 32;
    event.xclient.data.l[0] = maximized ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
    event.xclient.data.l[1] = static_cast<long>( XShared::m_atomNetWmStateMaximizedVert );
    event.xclient.data.l[2] = static_cast<long>( XShared::m_atomNetWmStateMaximizedHorz );
    event.xclient.data.l[3] = 1;
    xerrBegin();
    Status status = XSendEvent( XShared::getDisplay(), m_rootWindow, False, XSENDEVENT_EVENT_MASK, &event );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
    if( status == 0 )
        throw Exception( "XSendEvent() failed." );
}

/*
XWindow::setMinimized
---------------------

Description:
    Minimizes or unminimizes the window.

Arguments:
    minimized:  true if the window should be minimized, false otherwise.

Returns:
    N/A
*/
void XWindow::setMinimized( const bool minimized ) {
    //If we're already at the requested minimization state, there's nothing to do:
    if( m_minimized == minimized )
        return;

    //If the window isn't open yet, just set whether or not the window should be minimized:
    if( m_window == None ) {
        BaseWindowImpl::setMinimized( minimized );
        return;
    }

    setMinimizedInternal( minimized );
}

/*
XWindow::setMinimizedInternal
-----------------------------

Description:
    Internal method that does the actual work of minimizing or unminimizing the window.

Arguments:
    minimized:  true if the window should be minimized, false otherwise.

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::setMinimizedInternal( const bool minimized ) {
    if( minimized ) {
        //Minimize.
        //NOTE:
        //    Unlike fullscreen, maximize, and shade, Xlib has a dedicated function for doing minimizing windows - XIconifyWindow():
        xerrBegin();
        Status status = XIconifyWindow( XShared::getDisplay(), m_window, XShared::getScreen() );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
        if( status == 0 )
            throw Exception( "XIconifyWindow() failed." );
    } else if( m_visible ) {
        //Unminimize.
        //NOTE:
        //    There's no dedicated unminimize function. XIconifyWindow unmaps the window, so following the advice of the ICCCM, to unminimize the window, we call XMapWindow().
        //    The check for if the window is visible earlier ensures that we don't inadvertently show a window that should be hidden, since both showing a window and unminimizing it are
        //    accomplished with XMapWindow().
        xerrBegin();
        XMapWindow( XShared::getDisplay(), m_window );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        //Bring the window to the top of the stack.
        sendToTop();

        //Focus the window.
        //NOTE: This might not be possible immediately, in which case we'll set it as pending and activate it when possible.
        focus();
    }
}

/*
XWindow::setShaded
------------------

Description:
    Shades or unshades the window.

Arguments:
    shaded:  true if the window should be shaded, false otherwise.

Returns:
    N/A
*/
void XWindow::setShaded( const bool shaded ) {
    //If we're already at the requested shading state, there's nothing to do:
    if( m_shaded == shaded )
        return;

    //If the window isn't open yet, just set whether or not the window should be shaded:
    if( m_window == None ) {
        BaseWindowImpl::setShaded( shaded );
        return;
    }

    //The window can't be fullscreen and shaded at the same time. If the window is currently fullscreen, disable it first:
    if( m_fullscreen )
        setFullscreen( false );

    setShadedInternal( shaded );
}

/*
XWindow::setShadedInternal
--------------------------

Description:
    Internal method that does the actual work of shading or unshading the window.

Arguments:
    shaded:  true if the window should be shaded, false otherwise.

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::setShadedInternal( const bool shaded ) {
    //Shade or unshade the window in the same way that we put it into or out of fullscreen.
    //See setFullscreenInternal() for explanatory comments.
    XEvent event {};
    event.type = ClientMessage;
    event.xclient.window = m_window;
    event.xclient.message_type = XShared::m_atomNetWmState;
    event.xclient.format = 32;
    event.xclient.data.l[0] = shaded ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
    event.xclient.data.l[1] = static_cast<long>( XShared::m_atomNetWmStateShaded );
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 1;
    xerrBegin();
    Status status = XSendEvent( XShared::getDisplay(), m_rootWindow, False, XSENDEVENT_EVENT_MASK, &event );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
    if( status == 0 )
        throw Exception( "XSendEvent() failed." );
}

/*
XWindow::isShaded
-----------------

Description:
    Returns whether or not the window is shaded.

Arguments:
    N/A

Returns:
    bool:  true if the window is shaded, false otherwise.
*/
bool XWindow::isShaded() const {
    return m_shaded;
}

/*
XWindow::restore
----------------

Description:
    Restore the window to its normal state (not maximized, minimized, shaded, or in fullscreen).

Arguments:
    N/A

Returns:
    N/A
*/
void XWindow::restore() {
    //NOTE: We have to exit fullscreen first, otherwise the window won't be unmaximized:
    setFullscreen( false );
    setMaximized( false );
    setShaded( false );
    setMinimized( false );
}

/*
XWindow::isRestored
-------------------

Description:
    Returns whether or not the window is in its restored (normal) state.
    A window is in a restored state if it is not fullscreen, maximized, minimized, or shaded.

Arguments:
    N/A

Returns:
    bool:  true if the window is in its restored (normal) state, false otherwise.
*/
bool XWindow::isRestored() const {
    return !( m_fullscreen || m_maximized || m_minimized || m_shaded );
}

/*
XWindow::focus
--------------

Description:
    Sets the input focus to the window.

Arguments:
    N/A

Returns:
    bool:  true if the window is in its restored (normal) state. false otherwise.

Throws:
    XException:  If a call to an Xlib function fails.
*/
void XWindow::focus() {
    //If we're already focused, there's nothing to do:
    if( m_focused )
        return;

    //If the window is mapped, actually focus the window:
    if( m_mapped ) {
        //If this was a pending focus, it's no longer pending:
        m_pendingFocus = false;

        //Try to set the input focus:
        xerrBegin();
        XSetInputFocus( XShared::getDisplay(), m_window, RevertToParent, CurrentTime );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
    //Otherwise, focus as soon as possible:
    } else {
        //XSetInputFocus can generate BadMatch errors if the window is not currently mapped and "viewable".
        //NOTE:
        //    The terminology here is a little confusing, so to clarify:
        //    * A window is "viewable" if the window and all of its ancestors are mapped.
        //    * Just because a window is "viewable" doesn't necessarily mean the window is visible; e.g. the window can be obscured by other windows.
        //Wait for the window to be mapped first, then try again:
        m_pendingFocus = true;
    }
}

/*
XWindow::setMouseCapture
------------------------

Description:
    Enables or disables mouse capture.

Arguments:
    capture:  true if mouse capture should be enabled, false otherwise.

Returns:
    N/A
*/
void XWindow::setMouseCapture( const bool capture ) {
    //If we're already at the requested mouse capture state, there's nothing to do:
    if( m_mouseCapture == capture )
        return;
    
    //We can't capture the mouse if the window isn't open yet:
    if( m_window == None )
        return;
    
    if( capture ) {
        xerrBegin();
        int rv = XGrabPointer(
            XShared::getDisplay(),
            m_window,
            False,
            XGRABPOINTER_EVENT_MASK,
            GrabModeAsync,
            GrabModeAsync,
            None,
            None,
            CurrentTime
        );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
        if( rv != GrabSuccess )
            throw Exception( "XGrabPointer() failed." );
    } else {
        xerrBegin();
        XUngrabPointer( XShared::getDisplay(), CurrentTime );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
    }

    BaseWindowImpl::setMouseCapture( capture );
}

/*
XWindow::setCursorVisible
-------------------------

Description:
    Sets whether or not the cursor should be visible.

Arguments:
    N/A

Returns:
    N/A

Throws:
    XException:  If a call to an Xlib function fails.
*/
void XWindow::setCursorVisible( const bool cursorVisible ) {
    //If we're already at the requested cursor visibility state, there's nothing to do:
    if( m_cursorVisible == cursorVisible )
        return;

    //If the window isn't open yet, just set whether the cursor should be visible or not:
    if( m_window == None ) {
        BaseWindowImpl::setCursorVisible( cursorVisible );
        return;
    }

    //NOTE:
    //    While somewhat counter-intuitively named, showing the cursor after it has been hidden involves calling XUndefineCursor(), which unsets the custom blank cursor that our window is
    //    using, causing it to use whatever cursor the parent window is using, i.e. the default arrow cursor.
    if( cursorVisible ) {
        xerrBegin();
        XUndefineCursor( XShared::getDisplay(), m_window );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
    } else {
        xerrBegin();
        XDefineCursor( XShared::getDisplay(), m_window, XShared::getBlankCursor() );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();
    }

    //Update cursor visibility state:
    BaseWindowImpl::setCursorVisible( cursorVisible );
}

/*
XWindow::sendToTop
------------------

Description:
    Moves the window to the top of the window stack.

Arguments:
    N/A

Returns:
    N/A

Throws:
    XException:  If a call to an Xlib function fails.
*/
void XWindow::sendToTop() {
    xerrBegin();
    XRaiseWindow( XShared::getDisplay(), m_window );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
}

/*
XWindow::sendToBottom
---------------------

Description:
    Moves the window to the bottom of the window stack.

Arguments:
    N/A

Returns:
    N/A

Throws:
    XException:  If a call to an Xlib function fails.
*/
void XWindow::sendToBottom() {
    xerrBegin();
    XLowerWindow( XShared::getDisplay(), m_window );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();
}

/*
XWindow::screenToWindow
-----------------------

Description:
    Converts the given screen coordinates to window coordinates.
    Screen coordinates are relative to the top-left corner of the screen, and window coordinates are relative to the top-left corner of the window's client area.

Arguments:
    screenCoords:  The screen coordinates.

Returns:
    Point2i:       The window coordinates.
*/
Point2i XWindow::screenToWindow( Point2i screenCoords ) const {
    //If the window isn't open yet, just return (0,0):
    if( m_window == None )
        return Point2i( 0, 0 );

    return translateWindowCoords( XShared::getDisplay(), m_rootWindow, m_window, screenCoords );
}

/*
XWindow::windowToScreen
-----------------------

Description:
    Converts the given window coordinates to screen coordinates.
    Window coordinates are relative to the top-left corner of the window's client area, and screen coordinates are relative to the top-left corner of the screen.

Arguments:
    windowCoords:  The window coordinates.

Returns:
    Point2i:       The screen coordinates.
*/
Point2i XWindow::windowToScreen( Point2i windowCoords ) const {
    //If the window isn't open yet, just return (0,0):
    if( m_window == None )
        return Point2i( 0, 0 );

    return translateWindowCoords( XShared::getDisplay(), m_window, m_rootWindow, windowCoords );
}

/*
XWindow::getDisplay
-------------------

Description:
    Returns a pointer to the X11 Display.

Arguments:
    N/A

Returns:
    WindowDisplay:  A pointer to the X11 Display.
*/
WindowDisplay XWindow::getDisplay() const {
    return XShared::getDisplay();
}

/*
XWindow::getHandle
------------------

Description:
    Returns the X11 Window identifier.

Arguments:
    N/A

Returns:
    WindowHandle:  The X11 Window identifier.
*/
WindowHandle XWindow::getHandle() const {
    return m_window;
}

/*
XWindow::updateWindowState
--------------------------

Description:
    Checks the list of window state hints (which indicate if the window is fullscreen and/or maximized, among other things) and updates XWindow's internal bookkeeping to match.
    Should be called when the window's _NET_WM_STATE property is updated.

Arguments:
    N/A

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::updateWindowState() {
    //Hints we're checking for:
    bool fullscreen = false;
    bool maximized  = false;
    bool minimized  = false;
    bool shaded     = false;

    //Variables provided as arguments to or storing the return values of XGetWindowProperty():
    int            status;
    Atom           actual_type_return;
    int            actual_format_return;
    long           long_offset = 0L;
    //NOTE:
    //    _NET_WM_STATE is a list of Atom that normally can contain up to 12 unique Atoms (though, some Window Managers like KWin may extend this with additional possible Atoms).
    //    Although this is pretty much never the case, we assume that all 12 of the Atoms are in the list to (hopefully) avoid partial reads.
    long           long_length = 12L;
    unsigned long  nitems_return, bytes_after_return = 1UL;
    unsigned char* prop_return;

    //Variables for property iteration:
    Atom*          pProperty;
    Atom           property;

    ::Display* const display = XShared::getDisplay();

    do {
        prop_return = nullptr;

        //NOTE:
        //    These are some links to some resources that were very helpful in writing this function:
        //    https://stackoverflow.com/questions/69249370/how-do-i-capture-minimize-and-maximize-events-in-xwindows
        //    https://tronche.com/gui/x/xlib/window-information/XGetWindowProperty.html
        xerrBegin();
        status = XGetWindowProperty(
            display,                       //Specifies the connection to the X server.
            m_window,                      //Specifies the window whose property you want to obtain, change, rotate or delete.
            XShared::m_atomNetWmState,     //Specifies the property name.
            long_offset,                   //Specifies the offset in the specified property (in 32-bit quantities) where the data is to be retrieved.
            long_length,                   //Specifies the length in 32-bit multiples of the data to be retrieved.
            False,                         //Specifies a Boolean value that determines whether the property is deleted.
            XA_ATOM,                       //Specifies the atom identifier associated with the property type or AnyPropertyType.
            &actual_type_return,           //Returns the atom identifier that defines the actual type of the property.
            &actual_format_return,         //Returns the actual format of the property.
            &nitems_return,                //Returns the actual number of 8-bit, 16-bit, or 32-bit items stored in the prop_return data.
            &bytes_after_return,           //Returns the number of bytes remaining to be read in the property if a partial read was performed.
            &prop_return                   //Returns the data in the specified format. If the returned format is 8, the returned data is represented as a char array.
                                           //If the returned format is 16, the returned data is represented as a array of short int type and should be cast to that type to obtain the
                                           //elements. If the returned format is 32, the property data will be stored as an array of longs (which in a 64-bit application will be 64-bit
                                           //values that are padded in the upper 4 bytes).
        );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        //XGetWindowProperty() returns Success if it executes successfully.
        if( status != Success )
            throw Exception( "XGetWindowProperty() failed." );

        //Automatically free the data that XGetWindowProperty() allocated (pointed to by prop_return) when exiting this loop / at the end of this iteration.
        std::unique_ptr< unsigned char[], int(*)(void*) > uptr( prop_return, &XFree );

        //If the data we received didn't match what we were expecting, exit the loop.
        if(
            actual_type_return   != XA_ATOM || //The _NET_WM_STATE property is an array of Atom, so we're expecting XGetWindowProperty() to indicate this by setting actual_type_return to
                                               //XA_ATOM.
            actual_format_return != 32      || //Atoms are 32-bit unsigned ints, so we expect XGetWindowProperty() to indicate this by setting actual_format_return to be 32.
            nitems_return        == 0UL     || //We expect XGetWindowProperty() to return at least one item.
            prop_return          == nullptr    //Data was allocated to prop_return
        ) {
            break;
        }

        //Cast prop_return from (a pointer to an array of bytes) to (a pointer to an array of Atom).
        pProperty = reinterpret_cast<Atom*>( prop_return );

        //Iterate over returned Atoms.
        for( unsigned long i = 0UL; i < nitems_return; ++i ) {
            //Get the current property.
            property = *pProperty;

            //If we see the _NET_WM_STATE_FULLSCREEN Atom in the list of Atoms provided by the _NET_WM_STATE property, we know that the window is in fullscreen mode:
            if( property == XShared::m_atomNetWmStateFullscreen ) {
                fullscreen = true;
            }

            //If we see the _NET_WM_STATE_MAXIMIZED_VERT or _NET_WM_STATE_MAXIMIZED_HORZ Atom, we know that the window is maximized:
            if(
                property == XShared::m_atomNetWmStateMaximizedVert ||
                property == XShared::m_atomNetWmStateMaximizedHorz
            ) {
                maximized = true;
            }

            //If we see the _NET_WM_STATE_HIDDEN Atom, we know that the window is minimized.
            if( property == XShared::m_atomNetWmStateHidden ) {
                minimized = true;
            }

            //If we see the _NET_WM_STATE_SHADED Atom, we know that the window is shaded (collapsed to just a titlebar).
            if( property == XShared::m_atomNetWmStateShaded ) {
                shaded = true;
            }

            //If we see the _NET_WM_STATE_FOCUSED Atom, we know that the window has focus.
            //...

            //If we see the _NET_WM_STATE_ABOVE Atom, we know that the window is set to keep above other windows.
            //...

            //If we see the _NET_WM_STATE_BELOW Atom, we know that the window is set to keep below other windows.
            //...

            //Move the pointer to the next property in the array.
            ++pProperty;
        }

        //Increment the offset of the data to be fetched (measured in longs) in the next run by nitems_return (which we know is also measured in longs because we checked
        //actual_format_return == 32).
        long_offset += nitems_return;

        //Set the length of data to be fetched (measured in longs) in the next run to the remaining number of bytes divided by 4 (the length of a long).
        //NOTE:
        //    We're casting from unsigned long to signed long here, which normally would have the potential to cause an overflow, but the division by 4 here prior to the conversion
        //    has the added benefit of ensuring an overflow cannot occur. This is because the maximum value for an unsigned long is 2^32-1, and the maximum value for a signed long is
        //    2^31-1; if you were to represent an arbitrary number x as the difference between 2 to the power of some number, p, and some other number i (x = 2^p - i), then dividing
        //    both sides of the equation by 4 would have the effect of subtracting 2 from p:
        //        x / 4 = (2^p    - i) / 4
        //        x / 4 = 2^p/4   - i/4
        //        x / 4 = 2^p/2^2 - i/4
        //        x / 4 = 2^(p-2) - i/4
        //    So in in the worst case, where x = 2^32-1, x/4 = 2^30-1/4 (truncated to 2^30), which is well below the signed long maximum of 2^31-1, ensuring an overflow cannot occur.
        long_length = static_cast<long>( bytes_after_return / 4UL );
    //If bytes_after_return is greater than 0, XGetWindowProperty() performed a partial read and more data is available for reading.
    //However, if bytes_after_return is less than 4 somehow, it should mean whatever data wasn't read isn't large enough to be an Atom, so we just skip it.
    } while( long_length > 0L );

    //Update our windows' state flags and generate events if the window is visible:
    if( m_visible ) {
        WindowEvent e;
        
        bool wasRestored = isRestored();

        //Generate EnterFullscreen/ExitFullscreen events and update internal tracking:
        if( m_fullscreen ) {
            if( !fullscreen ) {
                e.type = WindowEventType::ExitFullscreen;
                pushEvent( e );
            }
        } else if( fullscreen ) {
            e.type = WindowEventType::EnterFullscreen;
            pushEvent( e );
        }
        m_fullscreen = fullscreen;

        //Generate Maximize/Unmaximize events and update internal tracking:
        if( m_maximized ) {
            if( !maximized ) {
                e.type = WindowEventType::Unmaximize;
                pushEvent( e );
            }
        } else if( maximized ) {
            e.type = WindowEventType::Maximize;
            pushEvent( e );
        }
        m_maximized  = maximized;

        //Generate Minimize/Unminimize events and update internal tracking:
        if( m_minimized ) {
            if( !minimized ) {
                e.type = WindowEventType::Unminimize;
                pushEvent( e );
            }
        } else if( minimized ) {
            e.type = WindowEventType::Minimize;
            pushEvent( e );
        }
        m_minimized  = minimized;

        //Generate Shade/Unshade events and update internal tracking:
        if( m_shaded ) {
            if( !shaded ) {
                e.type = WindowEventType::Unshade;
                pushEvent( e );
            }
        } else if( shaded ) {
            e.type = WindowEventType::Shade;
            pushEvent( e );
        }
        m_shaded = shaded;

        //Generate a Restore event if the window wasn't restored before, but is restored now:
        if( !wasRestored && isRestored() ) {
            e.type = WindowEventType::Restore;
            pushEvent( e );
        }
    }
}

/*
XWindow::updateFrameExtents
---------------------------

Description:
    Updates the window's cache of its frame extents, which describe how much space (in pixels) is between the borders of this window (the "client area") and the borders of its top-level window
    (which includes decorations like borders, the title bar, minimize/maximize/restore/close buttons, etc).

    Should be called when the window's _NET_FRAME_EXTENTS property is updated.

Arguments:
    N/A

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::updateFrameExtents() {
    int  extents[4];
    int* pExtents = extents;
    int* pExtentsEnd = extents + 4;

    //Variables provided as arguments to or storing the return values of XGetWindowProperty():
    int            status;
    Atom           actual_type_return;
    int            actual_format_return;
    long           long_offset = 0L;
    //NOTE:
    //    _NET_FRAME_EXTENTS is an array of 4 32-bit "cardinals" (representing the left, right, top, and bottom extents, in that order). We assume that the length is 4 to (hopefully) avoid
    //    partial reads.
    //    I can't find any real documented explanation of what "cardinals" are supposed to represent in X11, but Cardinal is defined as a typedef of unsigned int in <X11/Intrinsic.h>.
    //    However, even though we're receiving 32-bit values, the documentation of XGetWindowProperty states that the data is presented to us as an array of long, which can be 32-bit or 64-bit
    //    depending on the system.
    long           long_length = 4L;
    unsigned long  nitems_return, bytes_after_return = 1UL;
    unsigned char* prop_return;

    //Variables for property iteration.
    long*          pl;

    ::Display* const display = XShared::getDisplay();
    do {
        prop_return = nullptr;

        xerrBegin();
        status = XGetWindowProperty(
            display,
            m_window,
            XShared::m_atomNetFrameExtents,
            long_offset,
            long_length,
            False,
            XA_CARDINAL,
            &actual_type_return,
            &actual_format_return,
            &nitems_return,
            &bytes_after_return,
            &prop_return
        );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        if( status != Success )
            throw Exception( "XGetWindowProperty() failed." );

        std::unique_ptr< unsigned char[], int(*)(void*) > uptr( prop_return, &XFree );
        if(
            actual_type_return   != XA_CARDINAL || //The _NET_FRAME_EXTENTS property is an array of Cardinal, so we're expecting XGetWindowProperty() to indicate this by setting
                                                   //actual_type_return to XA_CARDINAL.
            actual_format_return != 32          ||
            nitems_return        == 0UL         ||
            prop_return          == nullptr
        ) {
            return;
        }
        pl = reinterpret_cast<long*>( prop_return );
        for( unsigned long i = 0UL; i < nitems_return; ++i, ++pl, ++pExtents ) {
            //Don't receive more than 4 values:
            if( pExtents >= pExtentsEnd )
                return;
            *pExtents = static_cast< int >( *pl );
        }
        long_offset += nitems_return;
        long_length = static_cast<long>( bytes_after_return / 4UL );
    } while( long_length > 0L );

    //We were expecting exactly 4 values.
    if( pExtents != pExtentsEnd )
        return;

    //Update the extents with the values we read:
    m_extents.set( extents[0], extents[2], extents[1], extents[3] );

    //Now that extents are known, if there was a pending resize, execute that now:
    if( m_pendingResize && isRestored() ) {
        setBounds( m_pendingResizeBounds );
        m_pendingResize = false;
    }
    
    //TEMP
    /*
    logInfo(
        (
            boost::format( "_NET_FRAME_EXTENTS received: (L: %d, R: %d, T: %d, B: %d)" )
            % m_extents.minX
            % m_extents.maxX
            % m_extents.minY
            % m_extents.maxY
        ).str()
    );
    */
}

/*
XWindow::updateFrameExtentsFromBounds
-------------------------------------

Description:
    This function offers an alternative method for calculating frame extents, deriving them from differences in geometry between the top-level window and inner window, rather than relying on
    the _NET_FRAME_EXTENTS property. It can be used if the _NET_FRAME_EXTENTS property isn't available on the system for whatever reason.

    If used, should be called when the window is reparented, after the window hierarchy has been updated with updateWindowHierarchy().

Arguments:
    N/A

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::updateFrameExtentsFromBounds() {
    ::Display* const display = XShared::getDisplay();

    //Update extents
    Bounds2i tlwBounds = getWindowBounds( display, m_topLevelWindow );
    Bounds2i bounds    = getWindowBounds( display, m_window );
    bounds.mins        = translateWindowCoords( display, m_window, m_rootWindow, bounds.mins );
    bounds.maxs        = translateWindowCoords( display, m_window, m_rootWindow, bounds.maxs );

    m_extents.set(
        bounds.minX - tlwBounds.minX,
        bounds.minY - tlwBounds.minY,
        tlwBounds.maxX - bounds.maxX,
        tlwBounds.maxY - bounds.maxY
    );

    //TEMP
    //logInfo( ( boost::format( "bounds: %s\ntlwBounds: %s\nextents: %s" ) % bounds % tlwBounds % m_extents ).str() );
}

/*
XWindow::updateWindowHierarchy
------------------------------

Description:
    Updates the window's internal references to its root window and top-level window.
    Should be called when the window is reparented.

Arguments:
    N/A

Returns:
    N/A

Throws:
    Exception:   If a call to an Xlib function fails.
    XException:  If a call to an Xlib function fails.
*/
void XWindow::updateWindowHierarchy() {
    ::Window topLevelWindow;

    //Variables storing return values for XQueryTree:
    Status   status;
    ::Window rootWindow;
    ::Window parentWindow = m_window;

    //Same as above, but we don't make use of these:
    ::Window*    children = nullptr;
    unsigned int children_len;

    //Traverse the list of our window's ancestors:
    ::Display* const display = XShared::getDisplay();
    do {
        topLevelWindow = parentWindow;

        //Ascend the window hierarchy until we arrive at the top level window.
        xerrBegin();
        status = XQueryTree( display, topLevelWindow, &rootWindow, &parentWindow, &children, &children_len );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        //XQueryTree failed.
        if( status == 0 )
            throw Exception( "XQueryTree() failed." );

        //We're not using the list of child windows that XQueryTree outputs, immediately free the list if one was allocated:
        if( children != NULL )
            XFree( children );
    //Stop looping if the parentWindow was None, or if parentWindow is the root window.
    //If parentWindow == None, this indicates that m_window was the root window somehow.
    //This shouldn't ever happen, but I still check for it anyway since that would result in an infinite loop if it *did* happen.
    //If parentWindow == rootWindow, this means that topLevelWindow is both an ancestor of our window and a child of the rootWindow, which should indicate that it's our top-level window.
    } while( parentWindow != None && parentWindow != rootWindow );

    //Update the root window and our top level window:
    m_rootWindow     = rootWindow;
    m_topLevelWindow = topLevelWindow;
}




}
}
