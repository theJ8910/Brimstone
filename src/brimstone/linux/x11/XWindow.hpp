/*
linux/x11/XWindow.hpp
---------------------
Copyright (c) 2024, theJ89

Description:
    The windowing implementation used on Linux with the X11 display server, XWindow, is defined here.
*/
#ifndef BS_LINUX_X11_XWINDOW_HPP
#define BS_LINUX_X11_XWINDOW_HPP




//Includes
#include <brimstone/types.hpp>                  //Brimstone::ustring
#include <brimstone/Bounds.hpp>                 //Brimstone::Bounds2i
#include <brimstone/window/WindowHandle.hpp>    //Brimstone::WindowHandle
#include <brimstone/window/WindowDisplay.hpp>   //Brimstone::WindowDisplay

#include <unordered_map>                        //std::unordered_map
#include <mutex>                                //std::mutex

#include <X11/Xlib.h>                           //X11

#include "../../window/BaseWindowImpl.hpp"      //Brimstone::Private::BaseWindowImpl
#include "XInputContext.hpp"                    //Brimstone::Private::XInputContext




namespace Brimstone {
namespace Private {

class XWindow : public BaseWindowImpl {
private:
    typedef std::unordered_map< ::Window, XWindow& > XWinToWindowMap;
public:
    XWindow();
    XWindow( XWindow& toCopy ) = delete;
    XWindow& operator =( XWindow& toCopy ) = delete;
    ~XWindow();

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

    void            setMouseCapture( const bool capture );

    void            setCursorVisible( const bool cursorVisible );
    
    void            sendToTop();
    void            sendToBottom();

    Point2i         screenToWindow( Point2i screenCoords ) const;
    Point2i         windowToScreen( Point2i windowCoords ) const;

    WindowDisplay   getDisplay() const;
    WindowHandle    getHandle() const;

private:
    void            windowProc( XEvent& xEvent, const bool repeated );
    void            setFullscreenInternal( const bool fullscreen );
    void            setMaximizedInternal( const bool maximized );
    void            setMinimizedInternal( const bool minimized );
    void            setShadedInternal( const bool shaded );
    void            updateWindowState();
    void            updateFrameExtents();
    void            updateFrameExtentsFromBounds();
    void            updateWindowHierarchy();

private:
    //This is sort of annoying. Both Brimstone and X11 define "Window",
    //albeit in different namespaces. Within the Brimstone namespace
    //we need to qualify the X11 Window by prefixing "::" to it
    //to indicate we want the Window in the global namespace.
    ::Window                m_window;
    ::Window                m_rootWindow;
    ::Window                m_topLevelWindow;
    XInputContext           m_inputContext;
    bool                    m_mapped;
    bool                    m_shaded;
    Bounds2i                m_extents;
    Bounds2i                m_normalBounds;
    bool                    m_pendingResize;
    Bounds2i                m_pendingResizeBounds;
    bool                    m_pendingFocus;
private:
    static void             mainProc( XEvent& xEvent, const bool repeated );

private:
    static std::mutex       m_windowsMutex;
    static XWinToWindowMap  m_windowMap;
};

}
}

#endif //BS_LINUX_X11_XWINDOW_HPP
