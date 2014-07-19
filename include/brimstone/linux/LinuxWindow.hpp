#ifndef LINUX_LINUXWINDOW_HPP
#define LINUX_LINUXWINDOW_HPP




//Includes
#include <unordered_map>
#include <brimstone/Rectangle.hpp>              //LongRectangle
#include <brimstone/types.hpp>                  //ustring
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>




namespace Brimstone {

class Window;
enum class MouseButton;
enum class Key;

namespace Private {

class LinuxWindow {
private:
    typedef std::unordered_map< ::Window, LinuxWindow& > XWinToWindowMap;
public:
    LinuxWindow( Window& parent );
    ~LinuxWindow();

    void            setTitle( const ustring& title );
    void            setPopup( const bool popup );
    void            setBounds( const LongRectangle& bounds );

private:
    LinuxWindow( const LinuxWindow& );
    LinuxWindow&  operator =( const LinuxWindow& );
    void windowProc( XEvent& xEvent );

private:
    //This is sort of annoying. Both Brimstone and X11 define "Window",
    //albeit in different namespaces. Within the Brimstone namespace
    //we need to qualify the X11 Window by prefixing "::" to it
    //to indicate we want the Window in the global namespace.
    Window*             m_parent;
    ::Window            m_window;
    XIM                 m_inputMethod;
    XIC                 m_inputContext;
    Atom                m_closeAtom;
public:
    static void     processEvents();

private:
    static void        mainProc( XEvent& xEvent );
    static MouseButton xButtonToMouseButton( const int button );
    static Key         xKeySymToKey( const KeySym& keySym );

private:
    static Display*         m_display;
    static int              m_screen;
    static XWinToWindowMap  m_windowMap;
};

typedef LinuxWindow WindowImpl;
}
}

#endif //LINUX_LINUXWINDOW_HPP
