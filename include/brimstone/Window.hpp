/*
Window.hpp
----------
Copyright (c) 2024, theJ89

Description:
    Defines a Window class.
    The window class is used to create, interact with, and receive feedback from a window.
    The underlying implementation is platform-specific.
*/
#ifndef BS_WINDOW_HPP
#define BS_WINDOW_HPP




//Includes
#include <brimstone/window/DWindowImpl.hpp>    //Brimstone::Private::WindowImpl
#include <brimstone/Bounds.hpp>                //Brimstone::Bounds2i
#include <brimstone/types.hpp>                 //Brimstone::ustring
#include <brimstone/window/WindowEvent.hpp>    //Brimstone::WindowEvent
#include <brimstone/window/WindowDisplay.hpp>  //Brimstone::WindowDisplay
#include <brimstone/window/WindowHandle.hpp>   //Brimstone::WindowHandle




namespace Brimstone {




class Window {
private:
    using WindowList = std::vector< Window* >;
public:
    Window();
    Window( Window& toCopy ) = delete;
    Window& operator =( Window& toCopy ) = delete;
    Window( Window&& toMove );
    Window& operator =( Window&& toMove );
    ~Window();

    void            open();
    void            close();
    bool            isOpen() const;

    void            frame();

    bool            peekEvent( WindowEvent& eventOut );
    bool            getEvent( WindowEvent& eventOut );
    void            pushEvent( const WindowEvent& eventIn );

    void            setTitle( const ustring& title );
    ustring         getTitle() const;

    void            setBorderless( const bool borderless );
    bool            isBorderless() const;

    void            setResizable( const bool resizable );
    bool            isResizable() const;

    void            setVisible( const bool visible );
    bool            isVisible() const;

    void            setBounds( const Bounds2i bounds );
    Bounds2i        getBounds() const;

    void            setFullscreen( const bool fullscreen );
    bool            isFullscreen() const;

    void            setMaximized( const bool maximized );
    bool            isMaximized() const;

    void            setMinimized( const bool minimized );
    bool            isMinimized() const;

    void            setShaded( const bool shaded );
    bool            isShaded() const;

    void            restore();
    bool            isRestored() const;

    void            focus();
    bool            isFocused() const;

    void            setKeyRepeat( const bool keyRepeat );
    bool            getKeyRepeat() const;

    void            setMouseCapture( const bool mouseCapture );
    bool            getMouseCapture() const;

    void            setCursorTrapped( const bool cursorTrapped );
    bool            isCursorTrapped() const;

    void            setCursorVisible( const bool cursorVisible );
    bool            isCursorVisible() const;

    void            setKeepCursorCentered( const bool keepCursorCentered );
    bool            getKeepCursorCentered() const;

    void            sendToTop();
    void            sendToBottom();

    Point2i         screenToWindow( Point2i screenCoords ) const;
    Point2i         windowToScreen( Point2i windowCoords ) const;

    WindowDisplay   getDisplay() const;
    WindowHandle    getHandle() const;
private:
    Private::WindowImpl*        m_impl;
};




} //namespace Brimstone




#endif //BS_WINDOW_HPP
