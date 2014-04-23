/*
WindowsWindow.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Window implementation used on the Windows platform.
    Handles the creation and management of any windows created through the engine.

    Defines Brimstone::Private::WindowImpl as Brimstone::Private::WindowsWindow.
*/

#ifndef BS_WINDOWS_WINDOW_HPP
#define BS_WINDOWS_WINDOW_HPP




//Includes
#include "WindowsHeader.hpp"        //HWND, HINSTANCE, ATOM, LRESULT, CALLBACK, WPARAM, LPARAM, etc
#include <unordered_map>            //std::unordered_map

#include "../Rectangle.hpp"         //LongRectangle
#include "../types.hpp"
#include "../WindowEvents.hpp"      //MouseClickEvent, MouseMoveEvent, KeyPressEvent


namespace Brimstone {
class Window;

namespace Private {

class WindowsWindow {
private:
    typedef std::unordered_map< HWND, WindowsWindow& > HWNDToWindowMap;
private:
    static bool                                       m_bClassRegistered;
    static std::unordered_map< HWND, WindowsWindow& > m_acWindowMap;

private:
    static ATOM WindowsWindow::registerWindowClass( HINSTANCE hInstance );
    static LRESULT CALLBACK mainProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
public:
    WindowsWindow( Window& cParent );
    ~WindowsWindow();

    static bool     processEvents();

    void            setTitle( const ustring& strTitle );
    void            getTitle( ustring& strTitleOut ) const;

    void            setPopup( const bool bPopup );

    void            setBounds( const LongRectangle& cBounds );
    void            getBounds( LongRectangle& cBoundsOut ) const;

    HWND            getHandle();
private:
    void            adjustWindowBounds( long iWidth, long iHeight );
    WindowsWindow( const WindowsWindow& );
    WindowsWindow&  operator =( const WindowsWindow& );
private:
    HWND            m_hWnd;
    Window*         m_pcParent;
};

typedef WindowsWindow WindowImpl;

}
}




#endif //BS_WINDOWS_WINDOW_HPP