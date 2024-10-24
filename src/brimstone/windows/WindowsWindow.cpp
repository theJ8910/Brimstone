/*
windows/WindowsWindow.cpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    See WindowsWindow.cpp for more information.

    TODO:
        Add support for MouseEnter, MouseLeave, EnterFullscreen, ExitFullscreen, Maximize, Unmaximize, Minimize, Unminimize and Restore events. Shading isn't a thing on Windows, so the Shade
        and Unshade events can be ignored.
*/




//Includes
#include "WindowsWindow.hpp"                       //Header

#include <brimstone/windows/WindowsUtil.hpp>       //Brimstone::Private::utf8to16
#include <brimstone/windows/WindowsException.hpp>  //Brimstone::Private::throwWindowsException
#include <brimstone/util/Clamp.hpp>                //Brimstone::clampedValue
#include <brimstone/Logger.hpp>                    //Brimstone::logError
#include <brimstone/Exception.hpp>                 //TEMP: Brimstone::NotImplementedException

#include <windowsx.h>                              //GET_X_LPARAM, GET_Y_LPARAM

#include <boost/format.hpp>                        //boost::format




namespace {




//Constants
constexpr wchar_t* WINDOW_CLASS            = L"BrimstoneWindow";
constexpr DWORD    WINDOW_STYLE_STANDARD   = ( WS_CAPTION | WS_MINIMIZEBOX );   //Characteristics only standard windows have
constexpr DWORD    WINDOW_STYLE_COMMON     = ( WS_SYSMENU );                    //Characteristics both standard and popup windows share

constexpr DWORD    WINDOW_STYLE_NORMAL     = WINDOW_STYLE_STANDARD | WINDOW_STYLE_COMMON;
constexpr DWORD    WINDOW_STYLE_BORDERLESS = WS_POPUP              | WINDOW_STYLE_COMMON;




} //namespace




namespace {




using namespace Brimstone;
using namespace Brimstone::Private;




const Key vkToKeyMap[] = {
    Key::INVALID,           //0x00: (undefined)
    Key::INVALID,           //0x01: VK_LBUTTON
    Key::INVALID,           //0x02: VK_RBUTTON
    Key::INVALID,           //0x03: VK_CANCEL
    Key::INVALID,           //0x04: VK_MBUTTON
    Key::INVALID,           //0x05: VK_XBUTTON1
    Key::INVALID,           //0x06: VK_XBUTTON2
    Key::INVALID,           //0x07: (undefined)
    Key::BACKSPACE,         //0x08: VK_BACK
    Key::TAB,               //0x09: VK_TAB
    Key::INVALID,           //0x0A: (reserved)
    Key::INVALID,           //0x0B: (reserved)
    Key::CLEAR,             //0x0C: VK_CLEAR (on my keyboard, this is Numpad 5 with numlock off)
    Key::RETURN,            //0x0D: VK_RETURN
    Key::INVALID,           //0x0E: (undefined)
    Key::INVALID,           //0x0F: (undefined)
    Key::INVALID,           //0x10: VK_SHIFT ('any' shift)
    Key::INVALID,           //0x11: VK_CONTROL ('any' control)
    Key::INVALID,           //0x12: VK_MENU ('any' alt)
    Key::PAUSE_BREAK,       //0x13: VK_PAUSE
    Key::CAPS_LOCK,         //0x14: VK_CAPITAL
    Key::INVALID,           //0x15: VK_HANGUEL / VK_HANGUL
    Key::INVALID,           //0x16: (undefined)
    Key::INVALID,           //0x17: VK_JUNJA
    Key::INVALID,           //0x18: VK_FINAL
    Key::INVALID,           //0x19: VK_HANJA / VK_KANJI
    Key::INVALID,           //0x1A: (undefined)
    Key::ESCAPE,            //0x1B: VK_ESCAPE
    Key::INVALID,           //0x1C: VK_CONVERT
    Key::INVALID,           //0x1D: VK_NONCONVERT
    Key::INVALID,           //0x1E: VK_ACCEPT
    Key::INVALID,           //0x1F: VK_MODECHANGE
    Key::SPACE,             //0x20: VK_SPACE
    Key::PAGE_UP,           //0x21: VK_PRIOR
    Key::PAGE_DOWN,         //0x22: VK_NEXT
    Key::END,               //0x23: VK_END
    Key::HOME,              //0x24: VK_HOME
    Key::LEFT,              //0x25: VK_LEFT
    Key::UP,                //0x26: VK_UP
    Key::RIGHT,             //0x27: VK_RIGHT
    Key::DOWN,              //0x28: VK_DOWN
    Key::INVALID,           //0x29: VK_SELECT
    Key::INVALID,           //0x2A: VK_PRINT
    Key::INVALID,           //0x2B: VK_EXECUTE
    Key::PRINT_SCREEN,      //0x2C: VK_SNAPSHOT
    Key::INSERT,            //0x2D: VK_INSERT
    Key::DEL,               //0x2E: VK_DELETE
    Key::INVALID,           //0x2F: VK_HELP
    Key::DIGIT_0,           //0x30: 0 key
    Key::DIGIT_1,           //0x31: 1 key
    Key::DIGIT_2,           //0x32: 2 key
    Key::DIGIT_3,           //0x33: 3 key
    Key::DIGIT_4,           //0x34: 4 key
    Key::DIGIT_5,           //0x35: 5 key
    Key::DIGIT_6,           //0x36: 6 key
    Key::DIGIT_7,           //0x37: 7 key
    Key::DIGIT_8,           //0x38: 8 key
    Key::DIGIT_9,           //0x39: 9 key
    Key::INVALID,           //0x3A: (undefined)
    Key::INVALID,           //0x3B: (undefined)
    Key::INVALID,           //0x3C: (undefined)
    Key::INVALID,           //0x3D: (undefined)
    Key::INVALID,           //0x3E: (undefined)
    Key::INVALID,           //0x3F: (undefined)
    Key::INVALID,           //0x40: (undefined)
    Key::A,                 //0x41: A key
    Key::B,                 //0x42: B key
    Key::C,                 //0x43: C key
    Key::D,                 //0x44: D key
    Key::E,                 //0x45: E key
    Key::F,                 //0x46: F key
    Key::G,                 //0x47: G key
    Key::H,                 //0x48: H key
    Key::I,                 //0x49: I key
    Key::J,                 //0x4A: J key
    Key::K,                 //0x4B: K key
    Key::L,                 //0x4C: L key
    Key::M,                 //0x4D: M key
    Key::N,                 //0x4E: N key
    Key::O,                 //0x4F: O key
    Key::P,                 //0x50: P key
    Key::Q,                 //0x51: Q key
    Key::R,                 //0x52: R key
    Key::S,                 //0x53: S key
    Key::T,                 //0x54: T key
    Key::U,                 //0x55: U key
    Key::V,                 //0x56: V key
    Key::W,                 //0x57: W key
    Key::X,                 //0x58: X key
    Key::Y,                 //0x59: Y key
    Key::Z,                 //0x5A: Z key
    Key::LSYSTEM,           //0x5B: VK_LWIN
    Key::RSYSTEM,           //0x5C: VK_RWIN
    Key::MENU,              //0x5D: VK_APPS
    Key::INVALID,           //0x5E: Reserved
    Key::INVALID,           //0x5F: VK_SLEEP
    Key::NUMPAD_0,          //0x60: VK_NUMPAD0
    Key::NUMPAD_1,          //0x61: VK_NUMPAD1
    Key::NUMPAD_2,          //0x62: VK_NUMPAD2
    Key::NUMPAD_3,          //0x63: VK_NUMPAD3
    Key::NUMPAD_4,          //0x64: VK_NUMPAD4
    Key::NUMPAD_5,          //0x65: VK_NUMPAD5
    Key::NUMPAD_6,          //0x66: VK_NUMPAD6
    Key::NUMPAD_7,          //0x67: VK_NUMPAD7
    Key::NUMPAD_8,          //0x68: VK_NUMPAD8
    Key::NUMPAD_9,          //0x69: VK_NUMPAD9
    Key::MULTIPLY,          //0x6A: VK_MULTIPLY
    Key::ADD,               //0x6B: VK_ADD
    Key::INVALID,           //0x6C: VK_SEPARATOR
    Key::SUBTRACT,          //0x6D: VK_SUBTRACT
    Key::DECIMAL,           //0x6E: VK_DECIMAL
    Key::DIVIDE,            //0x6F: VK_DIVIDE
    Key::F1,                //0x70: VK_F1
    Key::F2,                //0x71: VK_F2
    Key::F3,                //0x72: VK_F3
    Key::F4,                //0x73: VK_F4
    Key::F5,                //0x74: VK_F5
    Key::F6,                //0x75: VK_F6
    Key::F7,                //0x76: VK_F7
    Key::F8,                //0x77: VK_F8
    Key::F9,                //0x78: VK_F9
    Key::F10,               //0x79: VK_F10
    Key::F11,               //0x7A: VK_F11
    Key::F12,               //0x7B: VK_F12
    Key::INVALID,           //0x7C: VK_F13
    Key::INVALID,           //0x7D: VK_F14
    Key::INVALID,           //0x7E: VK_F15
    Key::INVALID,           //0x7F: VK_F16
    Key::INVALID,           //0x80: VK_F17
    Key::INVALID,           //0x81: VK_F18
    Key::INVALID,           //0x82: VK_F19
    Key::INVALID,           //0x83: VK_F20
    Key::INVALID,           //0x84: VK_F21
    Key::INVALID,           //0x85: VK_F22
    Key::INVALID,           //0x86: VK_F23
    Key::INVALID,           //0x87: VK_F24
    Key::INVALID,           //0x88: (unassigned)
    Key::INVALID,           //0x89: (unassigned)
    Key::INVALID,           //0x8A: (unassigned)
    Key::INVALID,           //0x8B: (unassigned)
    Key::INVALID,           //0x8C: (unassigned)
    Key::INVALID,           //0x8D: (unassigned)
    Key::INVALID,           //0x8E: (unassigned)
    Key::INVALID,           //0x8F: (unassigned)
    Key::NUMLOCK,           //0x90: VK_NUMLOCK
    Key::SCROLL_LOCK,       //0x91: VK_SCROLL
    Key::INVALID,           //0x92: (OEM specific)
    Key::INVALID,           //0x93: (OEM specific)
    Key::INVALID,           //0x94: (OEM specific)
    Key::INVALID,           //0x95: (OEM specific)
    Key::INVALID,           //0x96: (OEM specific)
    Key::INVALID,           //0x97: (unassigned)
    Key::INVALID,           //0x98: (unassigned)
    Key::INVALID,           //0x99: (unassigned)
    Key::INVALID,           //0x9A: (unassigned)
    Key::INVALID,           //0x9B: (unassigned)
    Key::INVALID,           //0x9C: (unassigned)
    Key::INVALID,           //0x9D: (unassigned)
    Key::INVALID,           //0x9E: (unassigned)
    Key::INVALID,           //0x9F: (unassigned)
    Key::LSHIFT,            //0xA0: VK_LSHIFT
    Key::RSHIFT,            //0xA1: VK_RSHIFT
    Key::LCTRL,             //0xA2: VK_LCONTROL
    Key::RCTRL,             //0xA3: VK_RCONTROL
    Key::LALT,              //0xA4: VK_LMENU
    Key::RALT,              //0xA5: VK_RMENU
    Key::INVALID,           //0xA6: VK_BROWSER_BACK
    Key::INVALID,           //0xA7: VK_BROWSER_FORWARD
    Key::INVALID,           //0xA8: VK_BROWSER_REFRESH
    Key::INVALID,           //0xA9: VK_BROWSER_STOP
    Key::INVALID,           //0xAA: VK_BROWSER_SEARCH
    Key::INVALID,           //0xAB: VK_BROWSER_FAVORITES
    Key::INVALID,           //0xAC: VK_BROWSER_HOME
    Key::MUTE,              //0xAD: VK_VOLUME_MUTE
    Key::VOLUME_DOWN,       //0xAE: VK_VOLUME_DOWN
    Key::VOLUME_UP,         //0xAF: VK_VOLUME_UP
    Key::MEDIA_NEXT,        //0xB0: VK_MEDIA_NEXT_TRACK
    Key::MEDIA_PREVIOUS,    //0xB1: VK_MEDIA_PREV_TRACK
    Key::MEDIA_STOP,        //0xB2: VK_MEDIA_STOP
    Key::MEDIA_PLAY_PAUSE,  //0xB3: VK_MEDIA_PLAY_PAUSE
    Key::INVALID,           //0xB4: VK_LAUNCH_MAIL
    Key::INVALID,           //0xB5: VK_LAUNCH_MEDIA_SELECT
    Key::INVALID,           //0xB6: VK_LAUNCH_APP1
    Key::INVALID,           //0xB7: VK_LAUNCH_APP2
    Key::INVALID,           //0xB8: (reserved)
    Key::INVALID,           //0xB9: (reserved)
    Key::SEMICOLON,         //0xBA: VK_OEM_1      (varies; on US keyboard: [;:] key)
    Key::EQUALS,            //0xBB: VK_OEM_PLUS   (for any country/region, [+=] key)
    Key::COMMA,             //0xBC: VK_OEM_COMMA  (for any country/region, [,<] key)
    Key::MINUS,             //0xBD: VK_OEM_MINUS  (for any country/region, [-_] key)
    Key::PERIOD,            //0xBE: VK_OEM_PERIOD (for any country/region, [.>] key)
    Key::SLASH,             //0xBF: VK_OEM_2      (varies; on US keyboard: [/?] key)
    Key::TILDE,             //0xC0: VK_OEM_3      (varies; on US keyboard: [`~] key)
    Key::INVALID,           //0xC1: (reserved)
    Key::INVALID,           //0xC2: (reserved)
    Key::INVALID,           //0xC3: (reserved)
    Key::INVALID,           //0xC4: (reserved)
    Key::INVALID,           //0xC5: (reserved)
    Key::INVALID,           //0xC6: (reserved)
    Key::INVALID,           //0xC7: (reserved)
    Key::INVALID,           //0xC8: (reserved)
    Key::INVALID,           //0xC9: (reserved)
    Key::INVALID,           //0xCA: (reserved)
    Key::INVALID,           //0xCB: (reserved)
    Key::INVALID,           //0xCC: (reserved)
    Key::INVALID,           //0xCD: (reserved)
    Key::INVALID,           //0xCE: (reserved)
    Key::INVALID,           //0xCF: (reserved)
    Key::INVALID,           //0xD0: (reserved)
    Key::INVALID,           //0xD1: (reserved)
    Key::INVALID,           //0xD2: (reserved)
    Key::INVALID,           //0xD3: (reserved)
    Key::INVALID,           //0xD4: (reserved)
    Key::INVALID,           //0xD5: (reserved)
    Key::INVALID,           //0xD6: (reserved)
    Key::INVALID,           //0xD7: (reserved)
    Key::INVALID,           //0xD8: (unassigned)
    Key::INVALID,           //0xD9: (unassigned)
    Key::INVALID,           //0xDA: (unassigned)
    Key::LBRACKET,          //0xDB: VK_OEM_4      (varies; on US keyboard: [[{] key)
    Key::BACKSLASH,         //0xDC: VK_OEM_5      (varies; on US keyboard: [\|] key)
    Key::RBRACKET,          //0xDD: VK_OEM_6      (varies; on US keyboard: []}] key)
    Key::QUOTE,             //0xDE: VK_OEM_7      (varies; on US keyboard: ['"] key)
    Key::INVALID,           //0xDF: VK_OEM_8      (varies; on US keyboard, unknown )
    Key::INVALID,           //0xE0: (reserved)
    Key::INVALID,           //0xE1: (OEM specific)
    Key::INVALID,           //0xE2: VK_OEM_102    (angle bracket / backslash on RT 102 keyboard)
    Key::INVALID,           //0xE3: (OEM specific)
    Key::INVALID,           //0xE4: (OEM specific)
    Key::INVALID,           //0xE5: VK_PROCESSKEY
    Key::INVALID,           //0xE6: (OEM specific)
    Key::INVALID,           //0xE7: VK_PACKET
    Key::INVALID,           //0xE8: (unassigned)
    Key::INVALID,           //0xE9: (OEM specific)
    Key::INVALID,           //0xEA: (OEM specific)
    Key::INVALID,           //0xEB: (OEM specific)
    Key::INVALID,           //0xEC: (OEM specific)
    Key::INVALID,           //0xED: (OEM specific)
    Key::INVALID,           //0xEE: (OEM specific)
    Key::INVALID,           //0xEF: (OEM specific)
    Key::INVALID,           //0xF0: (OEM specific)
    Key::INVALID,           //0xF1: (OEM specific)
    Key::INVALID,           //0xF2: (OEM specific)
    Key::INVALID,           //0xF3: (OEM specific)
    Key::INVALID,           //0xF4: (OEM specific)
    Key::INVALID,           //0xF5: (OEM specific)
    Key::INVALID,           //0xF6: VK_ATTN
    Key::INVALID,           //0xF7: VK_CRSEL
    Key::INVALID,           //0xF8: VK_EXSEL
    Key::INVALID,           //0xF9: VK_EREOF
    Key::INVALID,           //0xFA: VK_PLAY
    Key::INVALID,           //0xFB: VK_ZOOM
    Key::INVALID,           //0xFC: VK_NONAME
    Key::INVALID,           //0xFD: VK_PA1
    Key::INVALID,           //0xFE: VK_OEM_CLEAR
};

Key WindowsWindow::vkToKey( WPARAM wParam, LPARAM lParam ) {
    //Virtual keys are in the range 0-254
    if( wParam > 254 )
        return Key::INVALID;

    bool isExtended = ( ( lParam & 0x01000000 ) != 0 );

    //Translate shift, control, and menu VK codes into left/right VK codes
    switch( wParam ) {
    case VK_SHIFT:
        wParam = MapVirtualKey( ( lParam & 0x00FF0000 ) >> 16, MAPVK_VSC_TO_VK_EX );
        break;
    case VK_CONTROL:
        wParam = isExtended ? VK_RCONTROL   : VK_LCONTROL;
        break;
    case VK_MENU:
        wParam = isExtended ? VK_RMENU      : VK_LMENU;
        break;

    //The following keys are present on both the main part
    //of the keyboard and the numpad. We need to check the "extended key"
    //bit to differentiate them.
    case VK_INSERT:
         return isExtended ? Key::INSERT        : Key::NUMPAD_INSERT;
    case VK_DELETE:
         return isExtended ? Key::DEL           : Key::NUMPAD_DEL;
    case VK_HOME:
         return isExtended ? Key::HOME          : Key::NUMPAD_HOME;
    case VK_END:
         return isExtended ? Key::END           : Key::NUMPAD_END;
    case VK_PRIOR:      //Page up
        return isExtended ? Key::PAGE_UP        : Key::NUMPAD_PAGE_UP;
    case VK_NEXT:       //Page down
        return isExtended ? Key::PAGE_DOWN      : Key::NUMPAD_PAGE_DOWN;
    case VK_RETURN:
        return isExtended ? Key::ENTER          : Key::RETURN;
    case VK_UP:
        return isExtended ? Key::UP             : Key::NUMPAD_UP;
    case VK_DOWN:
        return isExtended ? Key::DOWN           : Key::NUMPAD_DOWN;
    case VK_LEFT:
        return isExtended ? Key::LEFT           : Key::NUMPAD_LEFT;
    case VK_RIGHT:
        return isExtended ? Key::RIGHT          : Key::NUMPAD_RIGHT;
    }

    if( vkToKeyMap[ wParam ] == Key::INVALID )
        logError( ( boost::format( "Invalid key: 0x%|04X|") % wParam ).str() );

    return vkToKeyMap[ wParam ];
}




} //namespace



namespace Brimstone::Private {




std::mutex                     WindowsWindow::m_windowsMutex;
WindowsWindow::HWNDToWindowMap WindowsWindow::m_windowMap;
bool                           WindowsWindow::m_classRegistered = false;
HCURSOR                        WindowsWindow::m_arrowCursor     = NULL;




WindowsWindow::WindowsWindow() :
    m_wnd( nullptr ),
    m_leadSurrogate( 0 ),
    m_hovering( false ) {
}

WindowsWindow::~WindowsWindow() {
    try {
        close();
    } catch( const IException& e ) {
        uncaughtException( e );
    }
}

void WindowsWindow::open() {
    HINSTANCE instance = GetModuleHandle( nullptr );

    //Register the window class
    {
        std::lock_guard< std::mutex > l( m_windowsMutex );
        if( m_classRegistered == false )
            registerWindowClass( instance );
    }

    //Get window style and bounds
    DWORD style = getWindowStyle();
    Bounds2i bounds = m_bounds;
    if( AdjustWindowRect( reinterpret_cast< RECT* >( &bounds ), style, false ) == FALSE )
        throwWindowsException();

    //Create the window
    m_wnd = CreateWindow(
        WINDOW_CLASS,                   //Name of Window Class
        utf8to16( m_title ).c_str(),    //Title of Window
        style,                          //Style of window (flags)
        bounds.minX,                    //x
        bounds.minY,                    //y
        bounds.getWidth(),              //w
        bounds.getHeight(),             //h
        nullptr,                        //parent window
        nullptr,                        //parent menu (which is... for submenues?)
        instance,                       //this window belongs to this instance of our program
        nullptr                         //Window creation data (which is?)
    );

    if( !m_wnd )
        throwWindowsException();

    //Add the window to the map
    {
        std::lock_guard< std::mutex > l( m_windowsMutex );
        m_windowMap.emplace( m_wnd, *this );
    }
}

void WindowsWindow::close() {
    m_hovering      = false;
    m_leadSurrogate = 0;

    if( m_wnd == nullptr )
        return;

    if( DestroyWindow( m_wnd ) == FALSE )
        throwWindowsException();

    //Remove the window from the map
    {
        std::lock_guard< std::mutex > l( m_windowsMutex );
        m_windowMap.erase( m_wnd );
    }

    m_wnd = nullptr;

    if( m_windowMap.empty() )
        PostQuitMessage( 0 );
}

bool WindowsWindow::isOpen() const {
    return m_wnd != nullptr;
}

void WindowsWindow::frame() {
    if( m_keepCursorCentered ) {
        //Find the coordinates for the center of the window in screen coordinates:
        Point2i center = m_bounds.getCenter();

        //Center the cursor on the window:
        if( SetCursorPos( center.x, center.y ) == 0 )
            throwWindowsException();
    }
}

/*
WindowsWindow::peekEvent
------------------------

Description:
    This function processes all messages that are currently in the message queue
    for the thread that called this function, returning when the message queue is empty.

    NOTE:
    Windows assigns each thread its own message queue.
    A window can only receive messages on the same thread it was created on.
Arguments:
    N/A

Returns:
    bool:   N/A
*/
bool WindowsWindow::peekEvent( WindowEvent& eventOut ) {
    //Nothing in event queue
    if( m_eventQueue.empty() ) {
        //Process all messages currently in the application's message queue
        MSG msg;
        while( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
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
WindowsWindow::getEvent
-----------------------

Description:
    This function will process a single message from the message queue for the thread
    that called this function.

    If the message queue is empty, this function will block until a message is available,
    or until a quit message is posted to the application. Therefore, running this function
    from a thread dedicated specifically to handling Window messages is recommended.

    NOTE:
    Windows assigns each thread its own message queue.
    A window can only receive messages on the same thread it was created on.

Arguments:
    N/A

Returns:
    bool:   true if an event was processed.
            false if a quit message was posted.
*/
bool WindowsWindow::getEvent( WindowEvent& eventOut ) {
    //Wait for an event to arrive in the window's event queue if it's currently empty.
    //Process messages from the application's message queue, blocking until a message arrives if necessary.
    //The WindowImpl::getEvent() call returns true if a message was processed, or false if the application is shutting down.
    while( m_eventQueue.empty() ) {
        MSG msg;
        if( GetMessage( &msg, nullptr, 0, 0 ) ) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        } else {
            return false;
        }
    }

    //Pop a message from the queue
    eventOut = m_eventQueue.front();
    m_eventQueue.pop();
    return true;
}

void WindowsWindow::setResizable( const bool resizable ) {
    BaseWindowImpl::setResizable( resizable );

    if( m_wnd == nullptr )
        return;

    if( SetWindowLongPtr( m_wnd, GWL_STYLE, getWindowStyle() ) == FALSE )
        throwWindowsException();
}

void WindowsWindow::setVisible( const bool visible ) {
    BaseWindowImpl::setVisible( visible );

    if( m_wnd == nullptr )
        return;

    ShowWindow( m_wnd, visible ? SW_SHOW : SW_HIDE );
}

void WindowsWindow::setTitle( const ustring& title ) {
    BaseWindowImpl::setTitle( title );

    if( m_wnd == nullptr )
        return;

    if( SetWindowTextW( m_wnd, utf8to16( title ).c_str() ) == false )
        throwWindowsException();
}

void WindowsWindow::setBorderless( const bool borderless ) {
    BaseWindowImpl::setBorderless( borderless );

    if( m_wnd == nullptr )
        return;

    if( SetWindowLongPtr( m_wnd, GWL_STYLE, getWindowStyle() ) == FALSE )
        throwWindowsException();

    readjustWindow();
}

void WindowsWindow::setBounds( const Bounds2i bounds ) {
    BaseWindowImpl::setBounds( bounds );

    if( m_wnd == nullptr )
        return;

    readjustWindow();
}

void WindowsWindow::setFullscreen( const bool fullscreen ) {
    if( m_fullscreen == fullscreen )
        return;

    if( m_wnd == nullptr ) {
        BaseWindowImpl::setFullscreen( fullscreen );
        return;
    }
    //TODO
    throw NotImplementedException();
}

void WindowsWindow::setMaximized( const bool maximized ) {
    //If we're already at the requested maximization state, there's nothing to do:
    if( m_maximized == maximized )
        return;

    //If the window isn't open yet, just set whether or not the window should be maximized:
    if( m_wnd == nullptr ) {
        BaseWindowImpl::setMaximized( maximized );
        return;
    }

    //TODO: Needs testing
    ShowWindow( m_wnd, maximized ? SW_MAXIMIZE : SW_RESTORE );
}

void WindowsWindow::setMinimized( const bool minimized ) {
    //If we're already at the requested minimization state, there's nothing to do:
    if( m_minimized == minimized )
        return;

    //If the window isn't open yet, just set whether or not the window should be minimized:
    if( m_wnd == nullptr ) {
        BaseWindowImpl::setMinimized( minimized );
        return;
    }

    //TODO: Needs testing
    ShowWindow( m_wnd, maximized ? SW_MINIMIZE : SW_RESTORE );
}

void WindowsWindow::setShaded( const bool shaded ) {
    //Shading a window isn't a thing on Windows. This function does nothing.
}

bool WindowsWindow::isShaded() const {
    //Shading a window isn't a thing on Windows. This function always return false.
    return false;
}

void WindowsWindow::restore() {
    //Nothing to do if we're already restored:
    if( isRestored() )
        return;

    //If the window isn't open yet, just set that the window should be restored:
    if( m_wnd == nullptr ) {
        BaseWindowImpl::setFullscreen( false );
        BaseWindowImpl::setMaximized( false );
        BaseWindowImpl::setMinimized( false );
        return;
    }

    //TODO: Needs testing
    ShowWindow( m_wnd, SW_RESTORE );
}

bool WindowsWindow::isRestored() const {
    return !( m_fullscreen || m_maximized || m_minimized );
}

void WindowsWindow::focus() {
    //If we're already focused, there's nothing to do:
    if( m_focused )
        return;

    //TODO: Needs testing
    if( SetActiveWindow( m_wnd ) == NULL )
        throwWindowsException();
}

void WindowsWindow::setMouseCapture( const bool mouseCapture ) {
    //If we're already at the requested mouse capture state, there's nothing to do:
    if( m_mouseCapture == mouseCapture )
        return;

    //If the window isn't open yet, just set whether mouse events should be captured not:
    if( m_wnd == nullptr ) {
        BaseWindowImpl::setMouseCapture( mouseCapture );
        return;
    }

    //Capture mouse events:
    if( mouseCapture ) {
        SetCapture( m_wnd );
    //Stop capturing mouse events:
    } else {
        if( ReleaseCapture() == FALSE )
            throwWindowsException();
    }

    //Update the mouse capture state:
    BaseWindowImpl::setMouseCapture( mouseCapture );
}

void WindowsWindow::setCursorTrapped( const bool cursorTrapped ) {
    //TODO: Needs testing
    
    //If we're already at the requested cursor trap state, there's nothing to do:
    if( m_cursorTrapped == cursorTrapped )
        return;

    //If the window isn't open yet, just set whether the cursor should be trapped or not:
    if( m_wnd == nullptr ) {
        BaseWindowImpl::setCursorTrapped( cursorTrapped );
        return;
    }

    //Trap the cursor:
    //NOTE:
    //    The clipping rect needs to be updated every time the window's bounds change.
    //    This is done in the onBoundsUpdated() function.
    if( cursorTrapped ) {
        if( ClipCursor( reinterpret_cast< LPRECT >( &m_bounds ) ) == FALSE )
            throwWindowsException();
    //Stop trapping the cursor:
    } else {
        if( ClipCursor( NULL ) == FALSE )
            throwWindowsException();
    }

    //Update the cursor trap state:
    BaseWindowImpl::setCursorTrapped( cursorTrapped );
}

void WindowsWindow::sendToTop() {
    //TODO: Needs testing
    if( SetWindowPos( m_wnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE ) == FALSE )
        throwWindowsException();
}

void WindowsWindow::sendToBottom() {
    //TODO: Needs testing
    if( SetWindowPos( m_wnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE ) == FALSE )
        throwWindowsException();
}

Point2i WindowsWindow::screenToWindow( Point2i screenCoords ) const {
    if( ScreenToClient( m_wnd, reinterpret_cast< LPPOINT >( &screenCoords ) ) == FALSE )
        throwWindowsException();

    return screenCoords;
}

Point2i WindowsWindow::windowToScreen( Point2i windowCoords ) const {
    if( ClientToScreen( m_wnd, reinterpret_cast< LPPOINT >( &windowCoords ) ) == FALSE )
        throwWindowsException();

    return windowCoords;
}

WindowDisplay WindowsWindow::getDisplay() const {
    //Displays are an X11 thing
    return nullptr;
}

WindowHandle WindowsWindow::getHandle() const {
    return m_wnd;
}

LRESULT WindowsWindow::windowProc( UINT message, WPARAM wParam, LPARAM lParam ) {
    switch( message ) {
    case WM_ERASEBKGND: {
        //Do nothing here to avoid screen flickering
    } break;
    case WM_SETCURSOR: {
        //Only change the cursor if it's over the client area:
        if( LOWORD( lParam ) != HTCLIENT )
            break;

        //Change the cursor to what it should be:
        SetCursor( m_cursorVisible ? m_arrowCursor : NULL );
    } break;
    case WM_MOUSEMOVE: {
        WindowEvent e;

        auto p = getCursorPos( lParam );
        m_cursorPos = p;

        //If the mouse wasn't previously hovering over this window, set it as hovering and push a MouseEnter event:
        if( !m_hovering ) {
            trackMouseEvent();
            m_hovering = true;

            //TODO: Needs testing and probably more work
            e.type = WindowEvent::MouseEnter;
            e.mouseMove.x = p.x;
            e.mouseMove.y = p.y;
            //TODO
            e.mouseMove.ctrl   = false;
            e.mouseMove.alt    = false;
            e.mouseMove.shift  = false;
            e.mouseMove.system = false;

            pushEvent( e );
        }

        e.type        = WindowEventType::MouseMove;
        e.mouseMove.x = p.x;
        e.mouseMove.y = p.y;
        //TODO
        e.mouseMove.ctrl   = false;
        e.mouseMove.alt    = false;
        e.mouseMove.shift  = false;
        e.mouseMove.system = false;

        pushEvent( e );
    } break;
    case WM_LBUTTONDOWN: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseDown;
        e.mouse.button = MouseButton::LEFT;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_LBUTTONUP: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseUp;
        e.mouse.button = MouseButton::LEFT;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_RBUTTONDOWN: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseDown;
        e.mouse.button = MouseButton::RIGHT;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_RBUTTONUP: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseUp;
        e.mouse.button = MouseButton::RIGHT;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_MBUTTONDOWN: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseDown;
        e.mouse.button = MouseButton::MIDDLE;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_MBUTTONUP: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseUp;
        e.mouse.button = MouseButton::MIDDLE;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_XBUTTONDOWN: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseDown;
        e.mouse.button =
            GET_XBUTTON_WPARAM( wParam ) == XBUTTON1 ?
                MouseButton::X1 :
                MouseButton::X2;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_XBUTTONUP: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type         = WindowEventType::MouseUp;
        e.mouse.button =
            GET_XBUTTON_WPARAM( wParam ) == XBUTTON1 ?
                MouseButton::X1 :
                MouseButton::X2;
        e.mouse.x      = p.x;
        e.mouse.y      = p.y;
        //TODO
        e.mouse.ctrl   = false;
        e.mouse.alt    = false;
        e.mouse.shift  = false;
        e.mouse.system = false;

        pushEvent( e );
    } break;
    case WM_MOUSEWHEEL: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type               = WindowEventType::MouseVScroll;
        e.mouseScroll.scroll = (float)GET_WHEEL_DELTA_WPARAM( wParam ) / (float)WHEEL_DELTA;
        e.mouseScroll.x      = p.x;
        e.mouseScroll.y      = p.y;
        //TODO
        e.mouseScroll.ctrl   = false;
        e.mouseScroll.alt    = false;
        e.mouseScroll.shift  = false;
        e.mouseScroll.system = false;

        pushEvent( e );
    } break;
    case WM_MOUSEHWHEEL: {
        auto p = getCursorPos( lParam );

        WindowEvent e;
        e.type               = WindowEventType::MouseHScroll;
        e.mouseScroll.scroll = -(float)GET_WHEEL_DELTA_WPARAM( wParam ) / (float)WHEEL_DELTA;
        e.mouseScroll.x      = p.x;
        e.mouseScroll.y      = p.y;
        //TODO
        e.mouseScroll.ctrl   = false;
        e.mouseScroll.alt    = false;
        e.mouseScroll.shift  = false;
        e.mouseScroll.system = false;

        pushEvent( e );
    } break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN: {
        bool isRepeating = ( ( lParam & ( 1 << 30 ) ) != 0 );
        if( isRepeating && !m_keyRepeat )
            break;

        WindowEvent e;
        e.type               = WindowEventType::KeyDown;
        e.key.key            = vkToKey( wParam, lParam );
        e.key.ctrl           = false;  //TODO
        e.key.alt            = false;  //TODO
        e.key.shift          = false;  //TODO
        e.key.system         = false;  //TODO
        e.key.repeat         = isRepeating;

        pushEvent( e );
    } break;
    case WM_SYSKEYUP:
    case WM_KEYUP: {
        WindowEvent e;
        e.type               = WindowEventType::KeyUp;
        e.key.key            = vkToKey( wParam, lParam );
        e.key.ctrl           = false;   //TODO
        e.key.alt            = false;   //TODO
        e.key.shift          = false;   //TODO
        e.key.system         = false;   //TODO
        e.key.repeat         = false;

        pushEvent( e );
    } break;
    case WM_SYSCHAR:
    case WM_CHAR: {
        //If the user holds a key down, Windows repeatedly sends the character
        //to the window. By default, repeating characters are ignored.
        //This can be changed in the Window with setKeyRepeat().
        bool isRepeating = ( ( lParam & ( 1 << 30 ) ) != 0 );
        if( isRepeating && !m_keyRepeat )
            break;

        //A leading surrogate of a surrogate pair was provided
        //A WindowEvent will not be dispatched until the trail surrogate arrives.
        if( wParam >= 0xDB00 && wParam <= 0xDBFF ) {
            m_leadSurrogate = (wchar)wParam;

        //Some other type of character was provided.
        } else {
            WindowEvent e;
            e.type = WindowEventType::Text;
            int32 utf8Count;

            //A trail surrogate was provided
            if( wParam >= 0xDC00 && wParam <= 0xDFFF ) {
                //UTF-16 encoded surrogate pair
                wchar utf16[2] = { m_leadSurrogate, (wchar)wParam };

                //Translate the pair to its equivalent UTF-8
                utf8Count = utf16to8( utf16, 2, e.text.utf8, 5 );

            //A non-surrogate was provided
            } else {
                //Translate the UTF-16 character to its equivalent UTF-8
                utf8Count = utf16to8( reinterpret_cast<wchar*>( &wParam ), 1, e.text.utf8, 5 );
            }

            //A UTF-8 encoding of a single code point should never exceed 4 bytes in length
            if( utf8Count > 4 )
                throw UnexpectedResultException();

            //Null terminate the string
            e.text.utf8[utf8Count] = 0;
            e.text.repeat = isRepeating;

            pushEvent( e );
        }
    } break;
    case WM_UNICHAR: {
        if( wParam == UNICODE_NOCHAR )
            return TRUE;
    } break;
    case WM_MOUSEHOVER: {
    } break;
    case WM_MOUSELEAVE: {
        //Update internal tracking:
        m_hovering = false;

        //TODO: Needs testing and probably more work
        auto p = getCursorPos( lParam );

        //Push MouseLeave event:
        WindowEvent e;
        e.type = WindowEvent::MouseLeave;
        e.mouseMove.x = m_cursorPos.x;
        e.mouseMove.y = m_cursorPos.y;
        //TODO
        e.mouseMove.ctrl   = false;
        e.mouseMove.alt    = false;
        e.mouseMove.shift  = false;
        e.mouseMove.system = false;

        pushEvent( e );
    } break;
    case WM_ACTIVATEAPP: {
    } break;
    case WM_ACTIVATE: {
        WORD lw = LOWORD( wParam );

        //Update focus state:
        m_focus = ( lw == WA_ACTIVE || lw == WA_CLICKACTIVE );

        //Push focus / blur event:
        WindowEvent e;
        e.type = m_focus ? WindowEventType::Focus : WindowEventType::Blur;

        pushEvent( e );
    } break;
    case WM_MOVE: {
        //Note: LOWORD and HIWORD return values between 0 and 65535 (unsigned short range),
        //so we need to cast to signed short here so that coordinates will
        //range between -32768 and 32767 (signed short range).
        Point2i pos( (short)LOWORD( lParam ), (short)HIWORD( lParam ) );

        //Update position
        m_bounds.setPosition( pos );

        WindowEvent e;
        e.type = WindowEventType::Move;
        e.move.x = pos.x;
        e.move.y = pos.y;

        pushEvent( e );

        //Update anything internal that relies on the bounds of the window:
        onBoundsUpdated();
    } break;
    case WM_MOVING: {
    } break;
    case WM_SIZE: {
        Size2i size( LOWORD( lParam ), HIWORD( lParam ) );

        //Update size
        m_bounds.setSize( size );

        WindowEvent e;
        e.type = WindowEventType::Resize;
        e.resize.w = size.w;
        e.resize.h = size.h;

        pushEvent( e );

        //Update anything internal that relies on the bounds of the window:
        onBoundsUpdated();
    } break;
    case WM_SIZING: {
    } break;
    case WM_SYSCOMMAND: {
        //NOTE:
        //    The 4 lowest bits of wParam are reserved for system use, so we need to mask those bits out with a bitwise AND before doing our comparison:
        switch( wParam & 0xFFF0 ) {
        case SC_MAXIMIZE: {
            //Update internal tracking:
            m_maximized = true;

            //Generate Maximize event:
            WindowEvent e;
            e.type = WindowEventType::Maximize;
        } break;
        case SC_MINIMIZE: {
            //Update internal tracking:
            m_minimized = true;

            //Generate Minimize event:
            WindowEvent e;
            e.type = WindowEventType::Minimize;
        } break;
        case SC_RESTORE: {
            WindowEvent e;
            //Update internal tracking and generate Unmaximize / Unminimize events (if applicable):
            if( m_maximized ) {
                m_maximized = false;
                e.type = WindowEventType::Unmaximize;
                pushEvent( e );
            }

            if( m_minimized ) {
                m_minimized = false;
                e.type = WindowEventType::Unminimize;
                pushEvent( e );
            }

            //Generate Restore event:
            e.type = WindowEventType::Restore;
            pushEvent( e );
        } break;
        }
        return DefWindowProc( m_wnd, message, wParam, lParam );
    } break;
    case WM_CLOSE: {
        WindowEvent e;
        e.type = WindowEventType::Close;

        pushEvent( e );
    } break;
    default:
        return DefWindowProc( m_wnd, message, wParam, lParam );
    }

    return 0;
}

Point2i WindowsWindow::getCursorPos( LPARAM lParam ) const {
    //The coordinates provided in lParam are relative to the upper-left corner of the window's client area.
    //However, we need to clamp values here because the window will return coordinates outside of the client
    //area as well (borders, etc)! Raw coordinates could even be negative.
    return Point2i(
        clampedValue( (long)GET_X_LPARAM( lParam ), 0L, m_bounds.getWidth()  - 1L ),
        clampedValue( (long)GET_Y_LPARAM( lParam ), 0L, m_bounds.getHeight() - 1L )
    );
}

DWORD WindowsWindow::getWindowStyle() const {
    DWORD style;
    if( m_borderless ) {
        style = WINDOW_STYLE_BORDERLESS;
    } else {
        style = WINDOW_STYLE_NORMAL;
        if( m_resizable )
            style |= WS_THICKFRAME;
    }

    if( m_visible )
        style |= WS_VISIBLE;

    return style;
}

void WindowsWindow::readjustWindow() {
    Bounds2i boundsAdj = m_bounds;
    if( AdjustWindowRect( reinterpret_cast< LPRECT >( &boundsAdj ), getWindowStyle(), false ) == FALSE )
        throwWindowsException();

    if( MoveWindow( m_wnd, boundsAdj.minX, boundsAdj.minY, boundsAdj.getWidth(), boundsAdj.getHeight(), TRUE ) == FALSE )
        throwWindowsException();
}

void WindowsWindow::onBoundsUpdated() {
    if( m_cursorTrapped ) {
        if( ClipCursor( reinterpret_cast< LPRECT >( &m_bounds ) ) == FALSE )
            throwWindowsException();
    }
}

void WindowsWindow::trackMouseEvent() {
    TRACKMOUSEEVENT tme;
    tme.cbSize      = sizeof( tme );
    tme.dwFlags     = TME_HOVER | TME_LEAVE;
    tme.hwndTrack   = m_wnd;
    tme.dwHoverTime = 0;
    if( TrackMouseEvent( &tme ) == FALSE )
        throwWindowsException();
}

/*
WindowsWindow::registerWindowClass
----------------------------------

Description:
    Registers the class of window our application uses with Windows.

Arguments:
    N/A

Returns:
    N/A
*/
void WindowsWindow::registerWindowClass( HINSTANCE instance ) {
    WNDCLASSEXW wcex;
    wcex.cbSize         =   sizeof( wcex );
    wcex.style          =   CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    =   (WNDPROC)mainProc;
    wcex.cbClsExtra     =   0;
    wcex.cbWndExtra     =   0;
    wcex.hInstance      =   instance;
    wcex.hbrBackground  =   (HBRUSH)GetStockObject( BLACK_BRUSH );
    wcex.lpszMenuName   =   nullptr;
    wcex.lpszClassName  =   WINDOW_CLASS;
    wcex.hIcon          =   LoadIcon(   nullptr, IDI_APPLICATION );
    wcex.hIconSm        =   LoadIcon(   nullptr, IDI_APPLICATION );
    //NOTE:
    //    Previously, this was set to was LoadCursor( nullptr, IDC_ARROW ).
    //    But now, in order to show or hide the cursor as needed, I have to wait for a WM_SETCURSOR message to arrive, then handle it by calling SetCursor(), passing the appropriate cursor to
    //    be shown at that time. I have to set the window class's hCursor to NULL here, because if I don't then every time the mouse is moved it will switch the cursor back to whatever cursor
    //    was specified for hCursor instead.
    wcex.hCursor        =   NULL;

    if( RegisterClassExW( &wcex ) == 0 )
        throwWindowsException();

    m_arrowCursor = LoadCursor( nullptr, IDC_ARROW );

    m_classRegistered = true;
}

/*
WindowsWindow::mainProc
-----------------------

Description:
    Windows occasionally sends messages to created windows such as this.
    When this happens, the Window will dispatch events to any listeners.

Arguments:
    hWnd:       The window that the message was sent to.
    uiMessage:  The type of message posted. Corresponds to a WM_* enum (WM_PAINT, WM_DESTROY, etc).
    wParam:     Contains additional data related to the message.
    lParam:     Also contains additional data related to the message.

Returns:
    LRESULT:    Appears to be a pointer or number.
                0 seems to be the appropriate value to return in most cases.
                The only time something other than 0 is returned (I assume) is when DefWindowProc() returns something.
                DefWindowProc() runs when a message we don't specifically handle here is processed.
*/
LRESULT CALLBACK WindowsWindow::mainProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    //Locate what Window this belongs to
    auto it = m_windowMap.find( hWnd );
    if( it == m_windowMap.end() )
        return DefWindowProc( hWnd, message, wParam, lParam );

    return it->second.windowProc( message, wParam, lParam );
}




} //namespace Brimstone::Private
