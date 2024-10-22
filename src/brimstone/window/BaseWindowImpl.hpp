/*
window/BaseWindowImpl.hpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    Base class for Window implementations.
*/
#ifndef BS_WINDOW_BASEWINDOWIMPL_HPP
#define BS_WINDOW_BASEWINDOWIMPL_HPP




//Includes
#include <brimstone/window/WindowEvent.hpp>  //Brimstone::WindowEvent
#include <brimstone/Point.hpp>               //Brimstone::Point2i
#include <brimstone/Bounds.hpp>              //Brimstone::Bounds2i
#include <brimstone/types.hpp>               //Brimstone::ustring

#include <queue>                             //std::queue




namespace Brimstone::Private {




class BaseWindowImpl {
public:
    BaseWindowImpl();

    void            pushEvent( const WindowEvent& eventIn );

    void            setTitle( const ustring& title );
    ustring         getTitle() const;

    void            setBorderless( const bool borderless );
    bool            isBorderless() const;

    void            setResizable( const bool resizable );
    bool            isResizable() const;

    void            setVisible( const bool visible );
    bool            isVisible() const;

    Point2i         getCursorPos() const;

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
protected:
    ustring                     m_title;
    Point2i                     m_cursorPos;
    Bounds2i                    m_bounds;
    bool                        m_visible;
    bool                        m_borderless;
    bool                        m_resizable;
    bool                        m_fullscreen;
    bool                        m_maximized;
    bool                        m_minimized;
    bool                        m_shaded;
    bool                        m_focused;
    bool                        m_keyRepeat;
    bool                        m_mouseCapture;
    bool                        m_cursorTrapped;
    bool                        m_cursorVisible;
    bool                        m_keepCursorCentered;

    std::queue< WindowEvent >   m_eventQueue;
};




}




#endif //BS_WINDOW_BASEWINDOWIMPL_HPP
