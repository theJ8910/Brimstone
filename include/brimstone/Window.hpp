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
#include <brimstone/WindowEvents.hpp>           //MouseDownEvent, MouseUpEvent, etc

#if defined( BS_BUILD_WINDOWS )

#include <brimstone/windows/WindowsWindow.hpp>  //WindowImpl (WindowsWindow)

#elif defined( BS_BUILD_LINUX )

#include <brimstone/linux/LinuxWindow.hpp>      //WindowImpl (LinuxWindow)

#endif




//Macros
#define BS_WINDOW_EVENT( name ) Signal< void( name##Event& ) > m_signal##name




namespace Brimstone {

class Window {
public:
    Window();
    ~Window();

    void        setTitle( const ustring& title );
    void        getTitle( ustring& titleOut ) const;

    void        setPopup( const bool popup );
    bool        getPopup() const;

    void        setVisible( const bool visible );
    bool        getVisible() const;

    void        setBounds( const LongRectangle& bounds );
    void        getBounds( LongRectangle& boundsOut ) const;

    void        setKeyRepeat( const bool keyRepeat );
    bool        getKeyRepeat() const;

    BS_WINDOW_EVENT( MouseDown      );
    BS_WINDOW_EVENT( MouseUp        );
    BS_WINDOW_EVENT( MouseMove      );
    BS_WINDOW_EVENT( MouseVScroll   );
    BS_WINDOW_EVENT( MouseHScroll   );
    BS_WINDOW_EVENT( KeyDown        );
    BS_WINDOW_EVENT( KeyUp          );
    BS_WINDOW_EVENT( CharacterTyped );
    BS_WINDOW_EVENT( WindowClose    );

private:
    ustring                 m_title;
    bool                    m_popup;
    bool                    m_keyRepeat;
    LongRectangle           m_bounds;

    Private::WindowImpl*    m_impl;

public:
    static void processEvents();
};

#undef BS_WINDOW_EVENT

}




#endif //BS_WINDOW_HPP
