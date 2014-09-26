/*
linux/LinuxWindow.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    The windowing implementation used on Linux, LinuxWindow, is defined here.
*/
#ifndef LINUX_LINUXWINDOW_HPP
#define LINUX_LINUXWINDOW_HPP




//Includes
#include <unordered_map>        //std::unordered_map

#include <brimstone/Bounds.hpp> //Bounds2i
#include <brimstone/types.hpp>  //ustring

#include <X11/Xlib.h>           //X11
#include <X11/Xutil.h>          //X11
#include <X11/Xos.h>            //X11




namespace Brimstone {

class Window;
enum class MouseButton;
enum class Key;

namespace Private {

typedef class LinuxWindow WindowImpl;
typedef       ::Window    WindowHandle;

class LinuxWindow {
private:
    typedef std::unordered_map< ::Window, LinuxWindow& > XWinToWindowMap;
public:
    LinuxWindow( Window& parent );
    ~LinuxWindow();

    void                    setTitle( const ustring& title );
    void                    setPopup( const bool popup );
    void                    setBounds( const Bounds2i& bounds );
    WindowHandle            getHandle() const;

private:
    LinuxWindow( const LinuxWindow& );
    LinuxWindow&  operator =( const LinuxWindow& );
    void                    windowProc( XEvent& xEvent );
    

private:
    //This is sort of annoying. Both Brimstone and X11 define "Window",
    //albeit in different namespaces. Within the Brimstone namespace
    //we need to qualify the X11 Window by prefixing "::" to it
    //to indicate we want the Window in the global namespace.
    Window*                 m_parent;
    ::Window                m_window;
    XIM                     m_inputMethod;
    XIC                     m_inputContext;
    Atom                    m_closeAtom;
public:
    static void             processEvents();
    static Point2i          getCursorPos( const XEvent& xEvent );
private:
    static void             mainProc( XEvent& xEvent );
    static MouseButton      xButtonToMouseButton( const int button );
    static Key              xKeySymToKey( const KeySym& keySym );

private:
    static Display*         m_display;
    static int              m_screen;
    static XWinToWindowMap  m_windowMap;
};

}
}

#endif //LINUX_LINUXWINDOW_HPP
