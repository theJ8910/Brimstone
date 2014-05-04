/*
windows/WindowsWindow.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Window implementation used on the Windows platform.
    Handles the creation and management of any windows created through the engine.

    Defines Brimstone::Private::WindowImpl as Brimstone::Private::WindowsWindow.
*/

#ifndef BS_WINDOWS_WINDOWSWINDOW_HPP
#define BS_WINDOWS_WINDOWSWINDOW_HPP




//Includes
#include <unordered_map>                        //std::unordered_map

#include <brimstone/windows/WindowsHeader.hpp>  //HWND, HINSTANCE, ATOM, LRESULT, CALLBACK, WPARAM, LPARAM, etc
#include <brimstone/Rectangle.hpp>              //LongRectangle
#include <brimstone/types.hpp>


namespace Brimstone {
class Window;

namespace Private {

class WindowsWindow {
private:
    typedef std::unordered_map< HWND, WindowsWindow& > HWNDToWindowMap;

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
    LRESULT         windowProc( UINT uiMessage, WPARAM wParam, LPARAM lParam );
    void            getMouseCoordinates( LPARAM lParam, int32& iXOut, int32& iYOut );

    WindowsWindow( const WindowsWindow& );
    WindowsWindow&  operator =( const WindowsWindow& );

private:
    HWND            m_hWnd;
    Window*         m_pcParent;
    wchar           m_cLeadSurrogate;

private:
    static ATOM             registerWindowClass( HINSTANCE hInstance );
    static LRESULT CALLBACK mainProc( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam );
    static Key              vkToKey( WPARAM wParam, LPARAM lParam );
private:
    static bool                                       m_bClassRegistered;
    static std::unordered_map< HWND, WindowsWindow& > m_acWindowMap;
};

typedef WindowsWindow WindowImpl;

}
}




#endif //BS_WINDOWS_WINDOWSWINDOW_HPP
