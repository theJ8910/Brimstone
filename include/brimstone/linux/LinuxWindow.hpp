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
    LinuxWindow( Window& cParent );
    ~LinuxWindow();

    static bool     processEvents();
    void            setTitle( const ustring& strTitle );
    void            setPopup( const bool bPopup );
    void            setBounds( const LongRectangle& cBounds );
private:
    LinuxWindow( const LinuxWindow& );
    LinuxWindow&  operator =( const LinuxWindow& );
    void windowProc( XEvent& cXEvent );
private:
    //This is sort of annoying. Both Brimstone and X11 define "Window",
    //albeit in different namespaces. Within the Brimstone namespace
    //we need to qualify the X11 Window by prefixing "::" to it
    //to indicate we want the Window in the global namespace.
    Window*             m_pcParent;
    ::Window            m_cWindow;
    GC                  m_cGraphicsContext;
    XIM                 m_pcInputMethod;
    XIC                 m_pcInputContext;

private:
    static void        mainProc( XEvent& cXEvent );
    static MouseButton xButtonToMouseButton( const int iButton );
    static Key         xKeySymToKey( const KeySym& pcKeySym );
private:
    static Display*         m_pcDisplay;
    static int              m_iScreen;
    static XWinToWindowMap  m_acWindowMap;
};

typedef LinuxWindow WindowImpl;
}
}

#endif //LINUX_LINUXWINDOW_HPP
