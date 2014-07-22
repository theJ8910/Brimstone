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
#include <brimstone/Bounds.hpp>                 //Bounds2i
#include <brimstone/types.hpp>


namespace Brimstone {
class Window;
enum class Key;

namespace Private {

class WindowsWindow {
private:
    typedef std::unordered_map< HWND, WindowsWindow& > HWNDToWindowMap;

public:
    WindowsWindow( Window& parent );
    ~WindowsWindow();

    void            setTitle( const ustring& title );
    void            getTitle( ustring& titleOut ) const;

    void            setPopup( const bool popup );

    void            setBounds( const Bounds2i& bounds );
    void            getBounds( Bounds2i& boundsOut ) const;

    HWND            getHandle();

private:
    LRESULT         windowProc( UINT message, WPARAM wParam, LPARAM lParam );
    Point2i         getCursorPos( LPARAM lParam );

    WindowsWindow( const WindowsWindow& );
    WindowsWindow&  operator =( const WindowsWindow& );

private:
    HWND            m_wnd;
    Window*         m_parent;
    wchar           m_leadSurrogate;

public:
    static void             processEvents();

private:
    static ATOM             registerWindowClass( HINSTANCE instance );
    static LRESULT CALLBACK mainProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam );
    static Key              vkToKey( WPARAM wParam, LPARAM lParam );
private:
    static bool                                       m_classRegistered;
    static std::unordered_map< HWND, WindowsWindow& > m_windowMap;
};

typedef WindowsWindow WindowImpl;

}
}




#endif //BS_WINDOWS_WINDOWSWINDOW_HPP
