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
#include <brimstone/Bounds.hpp>                 //Bounds2i
#include <brimstone/signals/Signal.hpp>         //Signal
#include <brimstone/types.hpp>                  //ustring
#include <brimstone/WindowEvents.hpp>           //MouseDownEvent, MouseUpEvent, etc
#include <brimstone/WindowImpl.hpp>             //Private::WindowImpl



//Macros
#define BS_WINDOW_EVENT( name )                     \
    Signal< void( name##Event& ) > m_signal##name;




namespace Brimstone {

class Window {
    friend class Graphics;
    friend Private::WindowImpl;
public:
    Window();
    ~Window();

    void        open();
    void        close();
    bool        isOpen() const;

    void        setTitle( const ustring& title );
    ustring     getTitle() const;

    void        setPopup( const bool popup );
    bool        isPopup() const;

    void        setResizable( const bool resizable );
    bool        isResizable() const;

    void        setVisible( const bool visible );
    bool        isVisible() const;

    void        setBounds( const Bounds2i& bounds );
    Bounds2i    getBounds() const;

    void        setKeyRepeat( const bool keyRepeat );
    bool        getKeyRepeat() const;

    void        setAutoClose( const bool autoClose );
    bool        getAutoClose() const;

    void        setMouseCapture( const bool capture );
    bool        getMouseCapture() const;

    Point2i     screenToWindow( const Point2i& screenCoords ) const;
    Point2i     windowToScreen( const Point2i& windowCoords ) const;

    BS_WINDOW_EVENT( MouseDown      );
    BS_WINDOW_EVENT( MouseUp        );
    BS_WINDOW_EVENT( MouseMove      );
    BS_WINDOW_EVENT( MouseVScroll   );
    BS_WINDOW_EVENT( MouseHScroll   );
    BS_WINDOW_EVENT( KeyDown        );
    BS_WINDOW_EVENT( KeyUp          );
    BS_WINDOW_EVENT( CharacterTyped );
    BS_WINDOW_EVENT( WindowFocus    );
    BS_WINDOW_EVENT( WindowBlur     );
    BS_WINDOW_EVENT( WindowMove     );
    BS_WINDOW_EVENT( WindowResize   );
    BS_WINDOW_EVENT( WindowClose    );
private:
    ustring                 m_title;
    bool                    m_visible;
    bool                    m_resizable;
    bool                    m_autoClose;
    bool                    m_mouseCapture;
    bool                    m_popup;
    bool                    m_keyRepeat;
    Bounds2i                m_bounds;

    Private::WindowImpl*    m_impl;

public:
    static void processEvents();
};

#undef BS_WINDOW_EVENT

}




#endif //BS_WINDOW_HPP
