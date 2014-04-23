//Includes
#include <memory>                                       //std::unique_ptr<>
#include <windowsx.h>                                   //GET_X_LPARAM, GET_Y_LPARAM

#include <brimstone/windows/WindowsWindow.hpp>          //Class header
#include <brimstone/windows/WindowsUtil.hpp>            //utf8to16
#include <brimstone/windows/WindowsException.hpp>       //throwWindowsException
#include <brimstone/util/Range.hpp>

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
}

bool WindowsWindow::processEvents() {
    MSG m_msg;

    while( PeekMessage( &m_msg, nullptr, 0, 0, PM_REMOVE ) == TRUE )
    {
        if( m_msg.message == WM_QUIT )
            return false;

        TranslateMessage( &m_msg );
        DispatchMessage( &m_msg );
    }
    return true;
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

    WindowsWindow* pcWindow = &it->second;


    switch( uiMessage ) {
    case WM_MOUSEMOVE: {
        LongRectangle cRect;
        pcWindow->m_pcParent->getBounds( cRect );

        MouseMoveEvent cEvent(
            Brimstone::ClampedValue( (long)GET_X_LPARAM( lParam ), 0L, cRect.getWidth()  - 1L ),
            Brimstone::ClampedValue( (long)GET_Y_LPARAM( lParam ), 0L, cRect.getHeight() - 1L )
        );
        pcWindow->m_pcParent->m_cSignalMouseMove( cEvent );  
    } break;
    case WM_LBUTTONDOWN: {
        MousePressEvent cEvent( MouseButton::LEFT );
        pcWindow->m_pcParent->m_cSignalMousePress( cEvent );
    } break;
    case WM_LBUTTONUP: {
        MouseReleaseEvent cEvent( MouseButton::LEFT );
        pcWindow->m_pcParent->m_cSignalMouseRelease( cEvent );
    } break;
    case WM_RBUTTONDOWN: {
        MousePressEvent cEvent( MouseButton::RIGHT );
        pcWindow->m_pcParent->m_cSignalMousePress( cEvent );
    } break;
    case WM_RBUTTONUP: {
        MouseReleaseEvent cEvent( MouseButton::RIGHT );
        pcWindow->m_pcParent->m_cSignalMouseRelease( cEvent );
    } break;
    case WM_MBUTTONDOWN: {
        MousePressEvent cEvent( MouseButton::MIDDLE );
        pcWindow->m_pcParent->m_cSignalMousePress( cEvent );
    } break;
    case WM_MBUTTONUP: {
        MouseReleaseEvent cEvent( MouseButton::MIDDLE );
        pcWindow->m_pcParent->m_cSignalMouseRelease( cEvent );
    } break;
    case WM_ACTIVATEAPP: {
    } break;
    case WM_MOVING: {
    } break;
    case WM_SIZING: {
    } break;
    case WM_CHAR: {
    } break;
    case WM_UNICHAR: {
        if( wParam == UNICODE_NOCHAR )
            return TRUE;
    } break;
    case WM_DESTROY: {
        PostQuitMessage( 0 );
    } break;
    default:
        return DefWindowProc( hWnd, uiMessage, wParam, lParam );
    }

    return 0;
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
