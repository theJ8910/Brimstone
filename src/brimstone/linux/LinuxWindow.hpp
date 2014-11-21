/*
linux/LinuxWindow.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    The windowing implementation used on Linux, LinuxWindow, is defined here.
*/
#ifndef BS_LINUX_LINUXWINDOW_HPP
#define BS_LINUX_LINUXWINDOW_HPP




//Includes
#include <brimstone/types.hpp>                  //Brimstone::ustring
#include <brimstone/Bounds.hpp>                 //Brimstone::Bounds2i
#include <brimstone/window/WindowHandle.hpp>    //Brimstone::WindowHandle

#include <unordered_map>                        //std::unordered_map
#include <mutex>                                //std::mutex

#include <X11/Xlib.h>                           //X11
#include <X11/Xutil.h>                          //X11
#include <X11/Xos.h>                            //X11


#include "../window/BaseWindowImpl.hpp"         //Brimstone::Private::BaseWindowImpl




namespace Brimstone {

class Window;
enum class MouseButton;
enum class Key;

namespace Private {

class LinuxWindow : public BaseWindowImpl {
private:
    typedef std::unordered_map< ::Window, LinuxWindow& > XWinToWindowMap;
public:
    LinuxWindow();
    LinuxWindow( LinuxWindow& toCopy ) = delete;
    LinuxWindow& operator =( LinuxWindow& toCopy ) = delete;
    ~LinuxWindow();

    void            open();
    void            close();
    bool            isOpen() const;

    bool            peekEvent( WindowEvent& eventOut );
    bool            getEvent( WindowEvent& eventOut );

    void            setTitle( const ustring& title );
    void            setPopup( const bool popup );
    void            setResizable( const bool sizable );
    void            setVisible( const bool visible );
    void            setBounds( const Bounds2i bounds );
    void            setMouseCapture( const bool capture );

    Point2i         screenToWindow( Point2i screenCoords ) const;
    Point2i         windowToScreen( Point2i windowCoords ) const;

    WindowHandle    getHandle() const;

private:
    void            windowProc( XEvent& xEvent );

private:
    //This is sort of annoying. Both Brimstone and X11 define "Window",
    //albeit in different namespaces. Within the Brimstone namespace
    //we need to qualify the X11 Window by prefixing "::" to it
    //to indicate we want the Window in the global namespace.
    ::Window                m_window;
    XIM                     m_inputMethod;
    XIC                     m_inputContext;
    Atom                    m_closeAtom;
private:
    static void             initX();
    static void             destroyX();

    static void             mainProc( XEvent& xEvent );
    static Point2i          getCursorPos( const XEvent& xEvent );
    static MouseButton      xButtonToMouseButton( const int button );
    static Key              xKeySymToKey( const KeySym& keySym );

private:
    static std::mutex       m_windowsMutex;
    static bool             m_xInitialized;
    static Display*         m_display;
    static int              m_screen;
    static unsigned long    m_black;
    static unsigned long    m_white;
    static XWinToWindowMap  m_windowMap;
};

}
}

#endif //BS_LINUX_LINUXWINDOW_HPP
