/*
windows/WindowsWindow.hpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    Window implementation used on the Windows platform.
    Handles the creation and management of any windows created through the engine.
*/
#ifndef BS_WINDOWS_WINDOWSWINDOW_HPP
#define BS_WINDOWS_WINDOWSWINDOW_HPP




//Includes
#include "../window/BaseWindowImpl.hpp"         //Brimstone::Private::BaseWindowImpl

#include <brimstone/windows/WindowsHeader.hpp>  //HWND, HINSTANCE, ATOM, LRESULT, CALLBACK, WPARAM, LPARAM, etc.
#include <brimstone/types.hpp>                  //Brimstone::ustring
#include <brimstone/Bounds.hpp>                 //Brimstone::Bounds2i
#include <brimstone/window/WindowHandle.hpp>    //Brimstone::WindowHandle
#include <brimstone/window/WindowDisplay.hpp>   //Brimstone::WindowDisplay

#include <unordered_map>                        //std::unordered_map
#include <mutex>                                //std::mutex




namespace Brimstone::Private {




class WindowsWindow : public BaseWindowImpl {
private:
    using HWNDToWindowMap = std::unordered_map< HWND, WindowsWindow& >;

public:
    WindowsWindow();
    WindowsWindow( WindowsWindow& toCopy ) = delete;
    WindowsWindow& operator =( WindowsWindow& toCopy ) = delete;
    ~WindowsWindow();

    void            open();
    void            close();
    bool            isOpen() const;

    void            frame();

    bool            peekEvent( WindowEvent& eventOut );
    bool            getEvent( WindowEvent& eventOut );

    void            setTitle( const ustring& title );
    
    void            setBorderless( const bool borderless );
    
    void            setResizable( const bool resizable );
    
    void            setVisible( const bool visible );
    
    void            setBounds( const Bounds2i bounds );
    
    void            setFullscreen( const bool fullscreen );
    
    void            setMaximized( const bool maximized );
    
    void            setMinimized( const bool minimized );
    
    void            setShaded( const bool shaded );
    bool            isShaded() const;
    
    void            restore();
    bool            isRestored() const;

    void            focus();

    void            setMouseCapture( const bool mouseCapture );

    void            setCursorTrapped( const bool cursorTrapped );

    void            setCursorVisible( const bool cursorVisible );

    void            sendToTop();
    void            sendToBottom();

    Point2i         screenToWindow( Point2i screenCoords ) const;
    Point2i         windowToScreen( Point2i windowCoords ) const;

    WindowDisplay   getDisplay() const;
    WindowHandle    getHandle() const;

private:
    LRESULT         windowProc( UINT message, WPARAM wParam, LPARAM lParam );
    Point2i         getCursorPos( LPARAM lParam ) const;
    DWORD           getWindowStyle() const;
    void            readjustWindow();
    void            trackMouseEvent();
    void            onBoundsUpdated();

private:
    HWND            m_wnd;
    wchar           m_leadSurrogate;
    bool            m_hovering;

private:
    static void             registerWindowClass( HINSTANCE instance );
    static LRESULT CALLBACK mainProc( HWND wnd, UINT message, WPARAM wParam, LPARAM lParam );
private:
    static std::mutex      m_windowsMutex;
    static bool            m_classRegistered;
    static HWNDToWindowMap m_windowMap;
    static HCURSOR         m_arrowCursor;
};




} //namespace Brimstone::Private




#endif //BS_WINDOWS_WINDOWSWINDOW_HPP
