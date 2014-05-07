/*
windows/WindowsWindow.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See WindowsWindow.cpp for more information.
*/




//Includes
#include <memory>                                       //std::unique_ptr<>
#include <windowsx.h>                                   //GET_X_LPARAM, GET_Y_LPARAM

#include <brimstone/windows/WindowsWindow.hpp>          //Class header
#include <brimstone/windows/WindowsUtil.hpp>            //utf8to16
#include <brimstone/windows/WindowsException.hpp>       //throwWindowsException
#include <brimstone/WindowEvents.hpp>                   //MouseClickEvent, MouseMoveEvent, KeyPressEvent
#include <brimstone/util/Range.hpp>                     //ClampedValue
#include <brimstone/Logger.hpp>                         //logError
#include <boost/format.hpp>                             //boost::format

#include <brimstone/Window.hpp>                         //Really stupid circular dependency hack




namespace {
    static const wchar_t* pszWindowClass = L"BrimstoneWindow";
    static const int eStandard = ( WS_CAPTION | WS_MINIMIZEBOX );   //Characteristics only standard windows have
    static const int eCommon   = ( WS_SYSMENU | WS_VISIBLE );       //Characteristics both standard and popup windows share

    static const int eNormalWindow = eStandard | eCommon;
    static const int ePopupWindow  = WS_POPUP  | eCommon;
}



namespace Brimstone {
namespace Private {

WindowsWindow::HWNDToWindowMap WindowsWindow::m_acWindowMap;
bool                           WindowsWindow::m_bClassRegistered = false;

WindowsWindow::WindowsWindow( Window& cParent ) : m_pcParent( &cParent ) {
    m_cLeadSurrogate = 0;

    HINSTANCE hInstance = GetModuleHandle( nullptr );

    if( m_bClassRegistered == false )
        registerWindowClass( hInstance );

    //Get title
    ustring strTitle;
    cParent.getTitle( strTitle );

    //Get bounds
    LongRectangle cBounds;
    cParent.getBounds( cBounds );
    long iWidth  = cBounds.getWidth();
    long iHeight = cBounds.getHeight();

    //Get popup
    bool bPopup = cParent.getPopup();

    //Create the window
    m_hWnd = CreateWindow(
        pszWindowClass,                                 //Name of Window Class
        utf8to16( strTitle ).c_str(),                   //Title of Window
        ( bPopup ? ePopupWindow : eNormalWindow ),      //Style of window (flags)
        cBounds.left,                                   //x
        cBounds.top,                                    //y
        iWidth,                                         //w
        iHeight,                                        //h
        nullptr,                                        //parent window
        nullptr,                                        //parent menu (which is... for submenues?)
        hInstance,                                      //this window belongs to this instance of our program
        nullptr                                         //Window creation data (which is?)
    );
    
    if( !m_hWnd )
        throwWindowsException();

    m_acWindowMap.emplace( m_hWnd, *this );

    adjustWindowBounds( iWidth, iHeight );
}

WindowsWindow::~WindowsWindow() {
    DestroyWindow( m_hWnd );
    m_acWindowMap.erase( m_hWnd );
    
    if( m_acWindowMap.empty() )
        PostQuitMessage( 0 );
}

void WindowsWindow::processEvents() {
    MSG msg;

    while( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
}

/*
WindowsWindow::adjustWindowBounds
-----------------------

Description:
    Called after resizing the window or changing it to a popup.

    Unforunately we need the CLIENT RECTANGLE OF THE WINDOW to be w pixels wide and h pixels high, not the window itself!
    If this isn't done, the presented image would be stretched.

Arguments:
    N/A

Returns:
    void:   N/A
*/
void WindowsWindow::adjustWindowBounds( long iWidth, long iHeight ) {
    if( m_pcParent->getPopup() )
        return;

    //The window rectangle is the rectangle that surrounds the window,
    //whereas the client rectangle is the rectangle surrounding the content area inside of the window.
    RECT cWndRect, cClientRect;

    if( GetWindowRect( m_hWnd, &cWndRect ) == FALSE )
        throwWindowsException();

    //NOTE: GetClientRect() outputs a rectangle whose .left and .top are 0,
    //which means the right and bottom are the width and height (respectively) of the client area.
    if( GetClientRect( m_hWnd, &cClientRect ) == FALSE )
        throwWindowsException();

    //MoveWindow is sort of a misnomer. You're actually framing the window in a new rectangle, meaning you can position and/or resize.
    //Here we're effectively adding width/height to the window to give the padding between the window and the client area its own space
    //so it doesn't take up the client area's space. This is done like so:
    //newSize =     oldSize + ( oldSize - oldClientSize ) 
    //        = 2 * oldSize - oldClientSize
    //NOTE: (x << 1) == (x * 2)
    iWidth  = ( iWidth  << 1 ) - cClientRect.right;
    iHeight = ( iHeight << 1 ) - cClientRect.bottom;
    if( MoveWindow( m_hWnd, cWndRect.left, cWndRect.top, iWidth, iHeight, TRUE ) == FALSE )
        throwWindowsException();
}

Key aeVKtoKey[] = {
    Key::INVALID,       //0x00: (undefined)
    Key::INVALID,       //0x01: VK_LBUTTON
    Key::INVALID,       //0x02: VK_RBUTTON
    Key::INVALID,       //0x03: VK_CANCEL
    Key::INVALID,       //0x04: VK_MBUTTON
    Key::INVALID,       //0x05: VK_XBUTTON1
    Key::INVALID,       //0x06: VK_XBUTTON2
    Key::INVALID,       //0x07: (undefined)
    Key::BACKSPACE,     //0x08: VK_BACK
    Key::TAB,           //0x09: VK_TAB
    Key::INVALID,       //0x0A: (reserved)
    Key::INVALID,       //0x0B: (reserved)
    Key::CLEAR,         //0x0C: VK_CLEAR (on my keyboard, this is Numpad 5 with numlock off)
    Key::ENTER,         //0x0D: VK_RETURN
    Key::INVALID,       //0x0E: (undefined)
    Key::INVALID,       //0x0F: (undefined)
    Key::INVALID,       //0x10: VK_SHIFT ('any' shift)
    Key::INVALID,       //0x11: VK_CONTROL ('any' control)
    Key::INVALID,       //0x12: VK_MENU ('any' alt)
    Key::PAUSE_BREAK,   //0x13: VK_PAUSE
    Key::CAPS_LOCK,     //0x14: VK_CAPITAL
    Key::INVALID,       //0x15: VK_HANGUEL / VK_HANGUL
    Key::INVALID,       //0x16: (undefined)
    Key::INVALID,       //0x17: VK_JUNJA
    Key::INVALID,       //0x18: VK_FINAL
    Key::INVALID,       //0x19: VK_HANJA / VK_KANJI
    Key::INVALID,       //0x1A: (undefined)
    Key::ESCAPE,        //0x1B: VK_ESCAPE
    Key::INVALID,       //0x1C: VK_CONVERT
    Key::INVALID,       //0x1D: VK_NONCONVERT
    Key::INVALID,       //0x1E: VK_ACCEPT
    Key::INVALID,       //0x1F: VK_MODECHANGE
    Key::SPACE,         //0x20: VK_SPACE
    Key::PAGE_UP,       //0x21: VK_PRIOR
    Key::PAGE_DOWN,     //0x22: VK_NEXT
    Key::END,           //0x23: VK_END
    Key::HOME,          //0x24: VK_HOME
    Key::LEFT,          //0x25: VK_LEFT
    Key::UP,            //0x26: VK_UP
    Key::RIGHT,         //0x27: VK_RIGHT
    Key::DOWN,          //0x28: VK_DOWN
    Key::INVALID,       //0x29: VK_SELECT
    Key::INVALID,       //0x2A: VK_PRINT
    Key::INVALID,       //0x2B: VK_EXECUTE
    Key::PRINT_SCREEN,  //0x2C: VK_SNAPSHOT
    Key::INSERT,        //0x2D: VK_INSERT
    Key::DEL,           //0x2E: VK_DELETE
    Key::INVALID,       //0x2F: VK_HELP
    Key::DIGIT_0,       //0x30: 0 key
    Key::DIGIT_1,       //0x31: 1 key
    Key::DIGIT_2,       //0x32: 2 key
    Key::DIGIT_3,       //0x33: 3 key
    Key::DIGIT_4,       //0x34: 4 key
    Key::DIGIT_5,       //0x35: 5 key
    Key::DIGIT_6,       //0x36: 6 key
    Key::DIGIT_7,       //0x37: 7 key
    Key::DIGIT_8,       //0x38: 8 key
    Key::DIGIT_9,       //0x39: 9 key
    Key::INVALID,       //0x3A: (undefined)
    Key::INVALID,       //0x3B: (undefined)
    Key::INVALID,       //0x3C: (undefined)
    Key::INVALID,       //0x3D: (undefined)
    Key::INVALID,       //0x3E: (undefined)
    Key::INVALID,       //0x3F: (undefined)
    Key::INVALID,       //0x40: (undefined)
    Key::A,             //0x41: A key
    Key::B,             //0x42: B key
    Key::C,             //0x43: C key
    Key::D,             //0x44: D key
    Key::E,             //0x45: E key
    Key::F,             //0x46: F key
    Key::G,             //0x47: G key
    Key::H,             //0x48: H key
    Key::I,             //0x49: I key
    Key::J,             //0x4A: J key
    Key::K,             //0x4B: K key
    Key::L,             //0x4C: L key
    Key::M,             //0x4D: M key
    Key::N,             //0x4E: N key
    Key::O,             //0x4F: O key
    Key::P,             //0x50: P key
    Key::Q,             //0x51: Q key
    Key::R,             //0x52: R key
    Key::S,             //0x53: S key
    Key::T,             //0x54: T key
    Key::U,             //0x55: U key
    Key::V,             //0x56: V key
    Key::W,             //0x57: W key
    Key::X,             //0x58: X key
    Key::Y,             //0x59: Y key
    Key::Z,             //0x5A: Z key
    Key::LSYSTEM,       //0x5B: VK_LWIN
    Key::RSYSTEM,       //0x5C: VK_RWIN
    Key::MENU,          //0x5D: VK_APPS
    Key::INVALID,       //0x5E: Reserved
    Key::INVALID,       //0x5F: VK_SLEEP
    Key::NUMPAD_0,      //0x60: VK_NUMPAD0
    Key::NUMPAD_1,      //0x61: VK_NUMPAD1
    Key::NUMPAD_2,      //0x62: VK_NUMPAD2
    Key::NUMPAD_3,      //0x63: VK_NUMPAD3
    Key::NUMPAD_4,      //0x64: VK_NUMPAD4
    Key::NUMPAD_5,      //0x65: VK_NUMPAD5
    Key::NUMPAD_6,      //0x66: VK_NUMPAD6
    Key::NUMPAD_7,      //0x67: VK_NUMPAD7
    Key::NUMPAD_8,      //0x68: VK_NUMPAD8
    Key::NUMPAD_9,      //0x69: VK_NUMPAD9
    Key::MULTIPLY,      //0x6A: VK_MULTIPLY
    Key::ADD,           //0x6B: VK_ADD
    Key::INVALID,       //0x6C: VK_SEPARATOR
    Key::SUBTRACT,      //0x6D: VK_SUBTRACT
    Key::DECIMAL,       //0x6E: VK_DECIMAL
    Key::DIVIDE,        //0x6F: VK_DIVIDE
    Key::F1,            //0x70: VK_F1
    Key::F2,            //0x71: VK_F2
    Key::F3,            //0x72: VK_F3
    Key::F4,            //0x73: VK_F4
    Key::F5,            //0x74: VK_F5
    Key::F6,            //0x75: VK_F6
    Key::F7,            //0x76: VK_F7
    Key::F8,            //0x77: VK_F8
    Key::F9,            //0x78: VK_F9
    Key::F10,           //0x79: VK_F10
    Key::F11,           //0x7A: VK_F11
    Key::F12,           //0x7B: VK_F12
    Key::INVALID,       //0x7C: VK_F13
    Key::INVALID,       //0x7D: VK_F14
    Key::INVALID,       //0x7E: VK_F15
    Key::INVALID,       //0x7F: VK_F16
    Key::INVALID,       //0x80: VK_F17
    Key::INVALID,       //0x81: VK_F18
    Key::INVALID,       //0x82: VK_F19
    Key::INVALID,       //0x83: VK_F20
    Key::INVALID,       //0x84: VK_F21
    Key::INVALID,       //0x85: VK_F22
    Key::INVALID,       //0x86: VK_F23
    Key::INVALID,       //0x87: VK_F24
    Key::INVALID,       //0x88: (unassigned)
    Key::INVALID,       //0x89: (unassigned)
    Key::INVALID,       //0x8A: (unassigned)
    Key::INVALID,       //0x8B: (unassigned)
    Key::INVALID,       //0x8C: (unassigned)
    Key::INVALID,       //0x8D: (unassigned)
    Key::INVALID,       //0x8E: (unassigned)
    Key::INVALID,       //0x8F: (unassigned)
    Key::NUMLOCK,       //0x90: VK_NUMLOCK
    Key::SCROLL_LOCK,   //0x91: VK_SCROLL
    Key::INVALID,       //0x92: (OEM specific)
    Key::INVALID,       //0x93: (OEM specific)
    Key::INVALID,       //0x94: (OEM specific)
    Key::INVALID,       //0x95: (OEM specific)
    Key::INVALID,       //0x96: (OEM specific)
    Key::INVALID,       //0x97: (unassigned)
    Key::INVALID,       //0x98: (unassigned)
    Key::INVALID,       //0x99: (unassigned)
    Key::INVALID,       //0x9A: (unassigned)
    Key::INVALID,       //0x9B: (unassigned)
    Key::INVALID,       //0x9C: (unassigned)
    Key::INVALID,       //0x9D: (unassigned)
    Key::INVALID,       //0x9E: (unassigned)
    Key::INVALID,       //0x9F: (unassigned)
    Key::LSHIFT,        //0xA0: VK_LSHIFT
    Key::RSHIFT,        //0xA1: VK_RSHIFT
    Key::LCTRL,         //0xA2: VK_LCONTROL
    Key::RCTRL,         //0xA3: VK_RCONTROL
    Key::LALT,          //0xA4: VK_LMENU
    Key::RALT,          //0xA5: VK_RMENU
    Key::INVALID,       //0xA6: VK_BROWSER_BACK
    Key::INVALID,       //0xA7: VK_BROWSER_FORWARD
    Key::INVALID,       //0xA8: VK_BROWSER_REFRESH
    Key::INVALID,       //0xA9: VK_BROWSER_STOP
    Key::INVALID,       //0xAA: VK_BROWSER_SEARCH
    Key::INVALID,       //0xAB: VK_BROWSER_FAVORITES
    Key::INVALID,       //0xAC: VK_BROWSER_HOME
    Key::INVALID,       //0xAD: VK_VOLUME_MUTE
    Key::INVALID,       //0xAE: VK_VOLUME_DOWN
    Key::INVALID,       //0xAF: VK_VOLUME_UP
    Key::INVALID,       //0xB0: VK_MEDIA_NEXT_TRACK
    Key::INVALID,       //0xB1: VK_MEDIA_PREV_TRACK
    Key::INVALID,       //0xB2: VK_MEDIA_STOP
    Key::INVALID,       //0xB3: VK_MEDIA_PLAY_PAUSE
    Key::INVALID,       //0xB4: VK_LAUNCH_MAIL
    Key::INVALID,       //0xB5: VK_LAUNCH_MEDIA_SELECT
    Key::INVALID,       //0xB6: VK_LAUNCH_APP1
    Key::INVALID,       //0xB7: VK_LAUNCH_APP2
    Key::INVALID,       //0xB8: (reserved)
    Key::INVALID,       //0xB9: (reserved)
    Key::SEMICOLON,     //0xBA: VK_OEM_1      (varies; on US keyboard: [;:] key)
    Key::EQUALS,        //0xBB: VK_OEM_PLUS   (for any country/region, [+=] key)
    Key::COMMA,         //0xBC: VK_OEM_COMMA  (for any country/region, [,<] key)
    Key::DASH,          //0xBD: VK_OEM_MINUS  (for any country/region, [-_] key)
    Key::PERIOD,        //0xBE: VK_OEM_PERIOD (for any country/region, [.>] key)
    Key::SLASH,         //0xBF: VK_OEM_2      (varies; on US keyboard: [/?] key)
    Key::TILDE,         //0xC0: VK_OEM_3      (varies; on US keyboard: [`~] key)
    Key::INVALID,       //0xC1: (reserved)
    Key::INVALID,       //0xC2: (reserved)
    Key::INVALID,       //0xC3: (reserved)
    Key::INVALID,       //0xC4: (reserved)
    Key::INVALID,       //0xC5: (reserved)
    Key::INVALID,       //0xC6: (reserved)
    Key::INVALID,       //0xC7: (reserved)
    Key::INVALID,       //0xC8: (reserved)
    Key::INVALID,       //0xC9: (reserved)
    Key::INVALID,       //0xCA: (reserved)
    Key::INVALID,       //0xCB: (reserved)
    Key::INVALID,       //0xCC: (reserved)
    Key::INVALID,       //0xCD: (reserved)
    Key::INVALID,       //0xCE: (reserved)
    Key::INVALID,       //0xCF: (reserved)
    Key::INVALID,       //0xD0: (reserved)
    Key::INVALID,       //0xD1: (reserved)
    Key::INVALID,       //0xD2: (reserved)
    Key::INVALID,       //0xD3: (reserved)
    Key::INVALID,       //0xD4: (reserved)
    Key::INVALID,       //0xD5: (reserved)
    Key::INVALID,       //0xD6: (reserved)
    Key::INVALID,       //0xD7: (reserved)
    Key::INVALID,       //0xD8: (unassigned)
    Key::INVALID,       //0xD9: (unassigned)
    Key::INVALID,       //0xDA: (unassigned)
    Key::LBRACKET,      //0xDB: VK_OEM_4      (varies; on US keyboard: [[{] key)
    Key::BACKSLASH,     //0xDC: VK_OEM_5      (varies; on US keyboard: [\|] key)
    Key::RBRACKET,      //0xDD: VK_OEM_6      (varies; on US keyboard: []}] key)
    Key::QUOTE,         //0xDE: VK_OEM_7      (varies; on US keyboard: ['"] key)
    Key::INVALID,       //0xDF: VK_OEM_8      (varies; on US keyboard, unknown )
    Key::INVALID,       //0xE0: (reserved)
    Key::INVALID,       //0xE1: (OEM specific)
    Key::INVALID,       //0xE2: VK_OEM_102    (angle bracket / backslash on RT 102 keyboard)
    Key::INVALID,       //0xE3: (OEM specific)
    Key::INVALID,       //0xE4: (OEM specific)
    Key::INVALID,       //0xE5: VK_PROCESSKEY
    Key::INVALID,       //0xE6: (OEM specific)
    Key::INVALID,       //0xE7: VK_PACKET
    Key::INVALID,       //0xE8: (unassigned)
    Key::INVALID,       //0xE9: (OEM specific)
    Key::INVALID,       //0xEA: (OEM specific)
    Key::INVALID,       //0xEB: (OEM specific)
    Key::INVALID,       //0xEC: (OEM specific)
    Key::INVALID,       //0xED: (OEM specific)
    Key::INVALID,       //0xEE: (OEM specific)
    Key::INVALID,       //0xEF: (OEM specific)
    Key::INVALID,       //0xF0: (OEM specific)
    Key::INVALID,       //0xF1: (OEM specific)
    Key::INVALID,       //0xF2: (OEM specific)
    Key::INVALID,       //0xF3: (OEM specific)
    Key::INVALID,       //0xF4: (OEM specific)
    Key::INVALID,       //0xF5: (OEM specific)
    Key::INVALID,       //0xF6: VK_ATTN
    Key::INVALID,       //0xF7: VK_CRSEL
    Key::INVALID,       //0xF8: VK_EXSEL
    Key::INVALID,       //0xF9: VK_EREOF
    Key::INVALID,       //0xFA: VK_PLAY
    Key::INVALID,       //0xFB: VK_ZOOM
    Key::INVALID,       //0xFC: VK_NONAME
    Key::INVALID,       //0xFD: VK_PA1
    Key::INVALID,       //0xFE: VK_OEM_CLEAR
};

LRESULT WindowsWindow::windowProc( UINT uiMessage, WPARAM wParam, LPARAM lParam ) {
    switch( uiMessage ) {
    case WM_MOUSEMOVE: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseMoveEvent cEvent( iX, iY );
        m_pcParent->m_cSignalMouseMove( cEvent );  
    } break;
    case WM_LBUTTONDOWN: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseDownEvent cEvent( MouseButton::LEFT, iX, iY );
        m_pcParent->m_cSignalMouseDown( cEvent );
    } break;
    case WM_LBUTTONUP: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseUpEvent cEvent( MouseButton::LEFT, iX, iY );
        m_pcParent->m_cSignalMouseUp( cEvent );
    } break;
    case WM_RBUTTONDOWN: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseDownEvent cEvent( MouseButton::RIGHT, iX, iY );
        m_pcParent->m_cSignalMouseDown( cEvent );
    } break;
    case WM_RBUTTONUP: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseUpEvent cEvent( MouseButton::RIGHT, iX, iY );
        m_pcParent->m_cSignalMouseUp( cEvent );
    } break;
    case WM_MBUTTONDOWN: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseDownEvent cEvent( MouseButton::MIDDLE, iX, iY );
        m_pcParent->m_cSignalMouseDown( cEvent );
    } break;
    case WM_MBUTTONUP: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseUpEvent cEvent( MouseButton::MIDDLE, iX, iY );
        m_pcParent->m_cSignalMouseUp( cEvent );
    } break;
    case WM_XBUTTONDOWN: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseDownEvent cEvent(
            GET_XBUTTON_WPARAM( wParam ) == XBUTTON1 ?
                MouseButton::X1 :
                MouseButton::X2,
            iX, iY
        );

        m_pcParent->m_cSignalMouseDown( cEvent );
    } break;
    case WM_XBUTTONUP: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseUpEvent cEvent(
            GET_XBUTTON_WPARAM( wParam ) == XBUTTON1 ?
                MouseButton::X1 :
                MouseButton::X2,
            iX, iY
        );

        m_pcParent->m_cSignalMouseUp( cEvent );
    } break;
    case WM_MOUSEWHEEL: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseVScrollEvent cEvent( (float)GET_WHEEL_DELTA_WPARAM( wParam ) / (float)WHEEL_DELTA, iX, iY );
        m_pcParent->m_cSignalMouseVScroll( cEvent );
    } break;
    case WM_MOUSEHWHEEL: {
        int32 iX, iY;
        getMouseCoordinates( lParam, iX, iY );

        MouseHScrollEvent cEvent( (float)GET_WHEEL_DELTA_WPARAM( wParam ) / (float)WHEEL_DELTA, iX, iY );
        m_pcParent->m_cSignalMouseHScroll( cEvent ); 
    } break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN: {
        bool bIsRepeating = ( ( lParam & ( 1 << 30 ) ) != 0 );
        if( bIsRepeating && !m_pcParent->getKeyRepeat() )
            break;

        KeyDownEvent cEvent( vkToKey( wParam, lParam ) );
        m_pcParent->m_cSignalKeyDown( cEvent );
    } break;
    case WM_SYSKEYUP:
    case WM_KEYUP: {
        KeyUpEvent cEvent( vkToKey( wParam, lParam ) );
        m_pcParent->m_cSignalKeyUp( cEvent );
    } break;
    case WM_SYSCHAR:
    case WM_CHAR: {
        //If the user holds a key down, Windows repeatedly sends the character
        //to the window. By default, repeating characters are ignored.
        //This can be changed in the Window with setKeyRepeat().
        bool bIsRepeating = ( ( lParam & ( 1 << 30 ) ) != 0 );
        if( bIsRepeating && !m_pcParent->getKeyRepeat() )
            break;

        //A leading surrogate of a surrogate pair was provided
        //A CharacterTypedEvent will not be dispatched until the trail surrogate arrives.
        if( wParam >= 0xDB00 && wParam <= 0xDBFF ) {
            m_cLeadSurrogate = (wchar)wParam;

        //Some other type of character was provided.
        } else {
            CharacterTypedEvent cEvent;
            uchar* pszUTF8 = const_cast< uchar* >( cEvent.getCharacter() );
            int32 iUTF8Count;

            //A trail surrogate was provided
            if( wParam >= 0xDC00 && wParam <= 0xDFFF ) {
                //UTF-16 encoded surrogate pair
                wchar acUTF16[2] = { m_cLeadSurrogate, (wchar)wParam };

                //Translate the pair to its equivalent UTF-8
                iUTF8Count = utf16to8( acUTF16, 2, pszUTF8, 5 );

            //A non-surrogate was provided
            } else {
                //Translate the UTF-16 character to its equivalent UTF-8
                iUTF8Count = utf16to8( reinterpret_cast<wchar*>( &wParam ), 1, pszUTF8, 5 );
            }

            //A UTF-8 encoding of a single code point should never exceed 4 bytes in length
            if( iUTF8Count > 4 )
                throw UnexpectedResultException();

            //Null terminate the string
            pszUTF8[iUTF8Count] = 0;
                
            m_pcParent->m_cSignalCharacterTyped( cEvent );
        }
    } break;
    case WM_UNICHAR: {
        if( wParam == UNICODE_NOCHAR )
            return TRUE;
    } break;
    case WM_ACTIVATEAPP: {
    } break;
    case WM_MOVING: {
    } break;
    case WM_SIZING: {
    } break;
    case WM_CLOSE: {
        WindowCloseEvent cEvent( *m_pcParent );
        m_pcParent->m_cSignalWindowClose( cEvent );
    } break;
    default:
        return DefWindowProc( getHandle(), uiMessage, wParam, lParam );
    }

    return 0;
}

void WindowsWindow::getMouseCoordinates( LPARAM lParam, int32& iXOut, int32& iYOut ) {
    //The coordinates provided in lParam are relative to the upper-left corner of the window's client area.
    //However, we need to clamp values here because the window will return coordinates outside of the client
    //area as well (borders, etc)! Raw coordinates could even be negative.
    LongRectangle cRect;
    m_pcParent->getBounds( cRect );

    iXOut = ClampedValue( (long)GET_X_LPARAM( lParam ), 0L, cRect.getWidth()  - 1L );
    iYOut = ClampedValue( (long)GET_Y_LPARAM( lParam ), 0L, cRect.getHeight() - 1L );
}

/*
WindowsWindow::registerWindowClass
-----------------------

Description:
    Registers the class of window our application uses with Windows.

Arguments:
    N/A

Returns:
    ATOM:   The value returned from RegisterClassEx - assuming this can be used to determine if registration successful?
*/
ATOM WindowsWindow::registerWindowClass( HINSTANCE hInstance ) {
    m_bClassRegistered = true;

    WNDCLASSEXW wcex;
    wcex.cbSize         =   sizeof( wcex );
    wcex.style          =   CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    =   (WNDPROC)mainProc;
    wcex.cbClsExtra     =   0;
    wcex.cbWndExtra     =   0;
    wcex.hInstance      =   hInstance;
    wcex.hbrBackground  =   (HBRUSH)GetStockObject( WHITE_BRUSH );
    wcex.lpszMenuName   =   nullptr;
    wcex.lpszClassName  =   pszWindowClass;
    wcex.hIcon          =   LoadIcon(   nullptr, IDI_APPLICATION );
    wcex.hIconSm        =   LoadIcon(   nullptr, IDI_APPLICATION );
    wcex.hCursor        =   LoadCursor( nullptr, IDC_ARROW       );

    return RegisterClassExW( &wcex );
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
LRESULT CALLBACK WindowsWindow::mainProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam ) {
    //Locate what Window this belongs to
    auto it = m_acWindowMap.find( hWnd );
    if( it == m_acWindowMap.end() )
        return DefWindowProc( hWnd, uiMessage, wParam, lParam );

    return it->second.windowProc( uiMessage, wParam, lParam );
}

void WindowsWindow::setTitle( const ustring& strTitle ) {
    if( SetWindowTextW( m_hWnd, utf8to16( strTitle ).c_str() ) == false )
        throwWindowsException();
}

void WindowsWindow::getTitle( ustring& strTitleOut ) const {
    int iLen;

    //Retrieve the length of the title, measured in wchar_t, not including the terminating null character.
    //This function can potentially overestimate the amount of wchar_t necessary to store the title.
    if( ( iLen = GetWindowTextLengthW( m_hWnd ) ) == 0 )
        throwWindowsException();

    //Allocate a temporary buffer to hold the string, plus a terminating null character.
    ++iLen;
    std::unique_ptr< wchar[] > pszTitle( new wchar[ iLen ] );

    //Copy the string into the buffer. Measure how many characters were output, not including the null character.
    //Unlike GetWindowTextLength, the returned value is the actual number of wchar_t that were written to the buffer.
    if( ( iLen = GetWindowTextW( m_hWnd, pszTitle.get(), iLen ) ) == 0 )
        throwWindowsException();

    //Convert to UTF-8 and return
    strTitleOut = utf16to8( pszTitle.get(), iLen + 1 );
}

Key WindowsWindow::vkToKey( WPARAM wParam, LPARAM lParam ) {
    //Virtual keys are in the range 0-254
    if( wParam > 254 )
        return Key::INVALID;

    bool bIsExtended = ( ( lParam & 0x01000000 ) != 0 );

    //Translate shift, control, and menu VK codes into left/right VK codes
    switch( wParam ) {
    case VK_SHIFT:
        wParam = MapVirtualKey( ( lParam & 0x00ff0000 ) >> 16, MAPVK_VSC_TO_VK_EX );
        break;
    case VK_CONTROL:
        wParam = bIsExtended ? VK_RCONTROL : VK_LCONTROL;
        break;
    case VK_MENU:
        wParam = bIsExtended ? VK_RMENU    : VK_LMENU;
        break;

    //The following keys are present on both the main part
    //of the keyboard and the numpad. We need to check the "extended key"
    //bit to differentiate them.
    case VK_INSERT:
         return bIsExtended ? Key::INSERT       : Key::NUMPAD_INSERT;
    case VK_DELETE:
         return bIsExtended ? Key::DEL          : Key::NUMPAD_DEL;
    case VK_HOME:
         return bIsExtended ? Key::HOME         : Key::NUMPAD_HOME;
    case VK_END:
         return bIsExtended ? Key::END          : Key::NUMPAD_END;
    case VK_PRIOR:      //Page up
        return bIsExtended ? Key::PAGE_UP       : Key::NUMPAD_PAGE_UP;
    case VK_NEXT:       //Page down
        return bIsExtended ? Key::PAGE_DOWN     : Key::NUMPAD_PAGE_DOWN;
    case VK_RETURN:
        return bIsExtended ? Key::NUMPAD_ENTER  : Key::ENTER;
    case VK_UP:
        return bIsExtended ? Key::UP            : Key::NUMPAD_UP;
    case VK_DOWN:
        return bIsExtended ? Key::DOWN          : Key::NUMPAD_DOWN;
    case VK_LEFT:
        return bIsExtended ? Key::LEFT          : Key::NUMPAD_LEFT;
    case VK_RIGHT:
        return bIsExtended ? Key::RIGHT         : Key::NUMPAD_RIGHT;
    }

    if( aeVKtoKey[ wParam ] == Key::INVALID )
        logError( ( boost::format( "Invalid key: 0x%|04x|") % wParam ).str().c_str() );

    return aeVKtoKey[ wParam ];
}

void WindowsWindow::setPopup( const bool bPopup ) {
    SetWindowLongPtr( m_hWnd, GWL_STYLE, ( bPopup ? ePopupWindow : eNormalWindow ) );
}

void WindowsWindow::setBounds( const LongRectangle& cBounds ) {
    if( MoveWindow( m_hWnd, cBounds.left, cBounds.right, cBounds.getWidth(), cBounds.getHeight(), TRUE ) == FALSE )
        throwWindowsException();
}

void WindowsWindow::getBounds( LongRectangle& cBoundsOut ) const {
    if( GetWindowRect( m_hWnd, (LPRECT)(&cBoundsOut) ) == FALSE )
        throwWindowsException();
}

HWND WindowsWindow::getHandle() {
    return m_hWnd;
}

//No copying allowed
WindowsWindow::WindowsWindow( const WindowsWindow& ) {
}

//No copying allowed
WindowsWindow&  WindowsWindow::operator =( const WindowsWindow& ) {
    throw NotImplementedException();
}

}
}
