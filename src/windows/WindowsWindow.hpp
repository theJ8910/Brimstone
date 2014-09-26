/*
windows/WindowsWindow.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Window implementation used on the Windows platform.
    Handles the creation and management of any windows created through the engine.
*/

#ifndef BS_WINDOWS_WINDOWSWINDOW_HPP
#define BS_WINDOWS_WINDOWSWINDOW_HPP




//Includes
#include <unordered_map>                        //std::unordered_map
#include <mutex>                                //std::mutex

#include <brimstone/windows/WindowsHeader.hpp>  //HWND, HINSTANCE, ATOM, LRESULT, CALLBACK, WPARAM, LPARAM, etc
#include <brimstone/Bounds.hpp>                 //Bounds2i
#include <brimstone/util/NonCopyable.hpp>       //NonCopyable




namespace Brimstone {

class Window;
enum class Key;

namespace Private {

class WindowsWindow : public NonCopyable {
private:
    typedef std::unordered_map< HWND, WindowsWindow& > HWNDToWindowMap;

public:
    WindowsWindow( Window& parent );
    ~WindowsWindow();

    void            open();
    void            close();
    bool            isOpen() const;

    void            setTitle( const ustring& title );
    void            setPopup( const bool popup );
    void            setResizable( const bool sizable );
    void            setVisible( const bool visible );
    void            setBounds( const Bounds2i& bounds );
    void            setMouseCapture( const bool capture );

    Point2i         screenToWindow( const Point2i& screenCoords ) const;
    Point2i         windowToScreen( const Point2i& windowCoords ) const;

    HWND            getHandle();

private:
    LRESULT         windowProc( UINT message, WPARAM wParam, LPARAM lParam );
    Point2i         getCursorPos( LPARAM lParam );

private:
    HWND            m_wnd;
    Window*         m_parent;
    wchar           m_leadSurrogate;
    std::mutex      m_contextMutex;

public:
    static void             processEvents();

private:
    static ATOM             registerWindowClass( HINSTANCE instance );
    static LRESULT CALLBACK mainProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam );
    static Key              vkToKey( WPARAM wParam, LPARAM lParam );
    static DWORD            getWindowStyle( const bool popup, const bool visible, const bool resizable );
private:
    static bool            m_classRegistered;
    static HWNDToWindowMap m_windowMap;
};

}
}




#endif //BS_WINDOWS_WINDOWSWINDOW_HPP
