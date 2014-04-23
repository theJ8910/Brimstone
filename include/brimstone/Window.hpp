/*
Window.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines a Window class.
    The window class is used to create, interact with, and receive feedback from a window.
    The underlying implementation is platform-specific.
*/

#ifndef BS_WINDOW_HPP
#define BS_WINDOW_HPP




//Includes
#include <brimstone/Rectangle.hpp>              //LongRectangle
#include <brimstone/signals/Signal.hpp>         //Signal
#include <brimstone/types.hpp>                  //ustring
#include <brimstone/WindowEvents.hpp>           //MousePressEvent, MouseReleaseEvent, etc

#if defined( BS_BUILD_WINDOWS )

#include <brimstone/windows/WindowsWindow.hpp>  //WindowImpl (WindowsWindow)

#elif defined( BS_BUILD_LINUX )

#include <brimstone/linux/LinuxWindow.hpp>      //WindowImpl (LinuxWindow)

#endif




namespace Brimstone {

class Window {
public:
    Window();
    ~Window();

    static bool    processEvents();

    void    setTitle( const ustring& strTitle );
    void    getTitle( ustring& strTitleOut ) const;

    void    setPopup( const bool bPopup );
    bool    getPopup() const;

    void    setVisible( const bool bVisible );
    bool    getVisible() const;

    void    setBounds( const LongRectangle& cBounds );
    void    getBounds( LongRectangle& cBoundsOut ) const;

    Signal< void( MousePressEvent& ) >      m_cSignalMousePress;
    Signal< void( MouseReleaseEvent& ) >    m_cSignalMouseRelease;
    Signal< void( MouseMoveEvent& ) >       m_cSignalMouseMove;
    Signal< void( KeyPressEvent& ) >        m_cSignalKeyPress;

private:
    ustring                 m_strTitle;
    bool                    m_bPopup;
    LongRectangle           m_cBounds;

    Private::WindowImpl*    m_pcImpl;
};

}




#endif //BS_WINDOW_HPP