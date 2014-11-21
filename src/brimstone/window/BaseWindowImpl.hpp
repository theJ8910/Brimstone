/*
window/BaseWindowImpl.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Base class for Window implementations.
*/
#ifndef BS_WINDOW_BASEWINDOWIMPL_HPP
#define BS_WINDOW_BASEWINDOWIMPL_HPP




//Includes
#include <brimstone/window/WindowEvent.hpp>     //Brimstone::WindowEvent
#include <brimstone/Bounds.hpp>                 //Brimstone::Bounds2i
#include <brimstone/types.hpp>                  //Brimstone::ustring

#include <queue>                                //std::queue




namespace Brimstone {
namespace Private {

class BaseWindowImpl {
public:
    BaseWindowImpl();

    void            pushEvent( const WindowEvent& eventIn );

    void            setTitle( const ustring& title );
    ustring         getTitle() const;

    void            setPopup( const bool popup );
    bool            isPopup() const;

    void            setResizable( const bool resizable );
    bool            isResizable() const;

    void            setVisible( const bool visible );
    bool            isVisible() const;

    void            setBounds( const Bounds2i bounds );
    Bounds2i        getBounds() const;

    void            setKeyRepeat( const bool keyRepeat );
    bool            getKeyRepeat() const;

    void            setMouseCapture( const bool capture );
    bool            getMouseCapture() const;
protected:
    ustring                     m_title;
    Bounds2i                    m_bounds;
    bool                        m_visible;
    bool                        m_popup;
    bool                        m_resizable;
    bool                        m_keyRepeat;
    bool                        m_mouseCapture;

    std::queue< WindowEvent >   m_eventQueue;
};

}
}

#endif //BS_WINDOW_BASEWINDOWIMPL_HPP