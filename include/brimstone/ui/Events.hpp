/*
ui/Events.hpp
-------------
Copyright (c) 2024, theJ89

Description:
    Various UI event classes are defined here.
*/
#ifndef BS_UI_EVENTS_HPP
#define BS_UI_EVENTS_HPP




//Includes
#include <brimstone/types.hpp>              //Brimstone::int32, Brimstone::uchar
#include <brimstone/Point.hpp>              //Brimstone::Point2i
#include <brimstone/Size.hpp>               //Brimstone::Size2i
#include <brimstone/input/Key.hpp>          //Brimstone::Key
#include <brimstone/input/MouseButton.hpp>  //Brimstone::MouseButton




namespace Brimstone {




//Forward declarations
class IPanel;
class IEventHandler;




enum class EventType {
    MOUSE_DOWN,
    MOUSE_UP,
    MOUSE_MOVE,
    MOUSE_OVER,
    MOUSE_OUT,
    MOUSE_VSCROLL,
    MOUSE_HSCROLL,
    KEY_DOWN,
    KEY_UP,
    TEXT,
    FOCUS,
    BLUR,
    MOVE,
    RESIZE,
    CLOSE,
    CLICK
};

class IEvent {
public:
    virtual IPanel&   getSource() const                         = 0;
    virtual EventType getType() const                           = 0;
    virtual void      accept( IEventHandler& visitor ) const    = 0;
};

class AbstractEvent : public IEvent {
public:
    AbstractEvent( IPanel& source );
    virtual IPanel& getSource() const;
private:
    IPanel* m_source;
};

class MouseEvent : public AbstractEvent {
public:
    MouseEvent( IPanel& source, const Point2i& cursorPos );
    Point2i getCursorPos() const;
private:
    Point2i m_cursorPos;
};

class MouseButtonEvent : public MouseEvent {
public:
    MouseButtonEvent( IPanel& source, const MouseButton button, const Point2i& cursorPos );
    MouseButton getButton() const;
private:
    MouseButton m_button;
};

class MouseDownEvent : public MouseButtonEvent {
public:
    MouseDownEvent( IPanel& source, const MouseButton button, const Point2i& cursorPos );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class MouseUpEvent : public MouseButtonEvent {
public:
    MouseUpEvent( IPanel& source, const MouseButton button, const Point2i& cursorPos );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class MouseMoveEvent : public MouseEvent {
public:
    MouseMoveEvent( IPanel& source, const Point2i& cursorPos );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class MouseOverEvent : public MouseEvent {
public:
    MouseOverEvent( IPanel& source, const Point2i& cursorPos );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class MouseOutEvent : public MouseEvent {
public:
    MouseOutEvent( IPanel& source, const Point2i& cursorPos );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class MouseScrollEvent : public MouseEvent {
public:
    MouseScrollEvent( IPanel& source, const float scrollAmount, const Point2i& cursorPos );
    float getScrollAmount() const;
private:
    float m_scrollAmount;
};

class MouseVScrollEvent : public MouseScrollEvent {
public:
    MouseVScrollEvent( IPanel& source, const float scrollAmount, const Point2i& cursorPos );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class MouseHScrollEvent : public MouseScrollEvent {
public:
    MouseHScrollEvent( IPanel& source, const float scrollAmount, const Point2i& cursorPos );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class KeyEvent : public AbstractEvent {
public:
    KeyEvent( IPanel& source, const Key key );
    Key getKey() const;
private:
    Key m_key;
};

class KeyDownEvent : public KeyEvent {
public:
    KeyDownEvent( IPanel& source, const Key key );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class KeyUpEvent : public KeyEvent {
public:
    KeyUpEvent( IPanel& source, const Key key );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class TextEvent : public AbstractEvent {
public:
    TextEvent( IPanel& source );
    const uchar* getCharacter() const;
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
private:
    uchar m_key[5];
};

class FocusEvent : public AbstractEvent {
public:
    FocusEvent( IPanel& source );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class BlurEvent : public AbstractEvent {
public:
    BlurEvent( IPanel& source );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class MoveEvent : public AbstractEvent {
public:
    MoveEvent( IPanel& source, const Point2i& pos );
    Point2i getPos() const;
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
private:
    Point2i m_pos;
};

class ResizeEvent : public AbstractEvent {
public:
    ResizeEvent( IPanel& source, const Size2i& size );
    Size2i getSize() const;
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
private:
    Size2i m_size;
};

class CloseEvent : public AbstractEvent {
public:
    CloseEvent( IPanel& source );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class ClickEvent : public AbstractEvent {
public:
    ClickEvent( IPanel& source );
    virtual EventType getType() const;
    virtual void accept( IEventHandler& visitor ) const;
};

class IEventHandler {
public:
    virtual void handle( const MouseDownEvent& eventIn )     = 0;
    virtual void handle( const MouseUpEvent& eventIn )       = 0;
    virtual void handle( const MouseMoveEvent& eventIn )     = 0;
    virtual void handle( const MouseOverEvent& eventIn )     = 0;
    virtual void handle( const MouseOutEvent& eventIn )      = 0;
    virtual void handle( const MouseVScrollEvent& eventIn )  = 0;
    virtual void handle( const MouseHScrollEvent& eventIn )  = 0;
    virtual void handle( const KeyDownEvent& eventIn )       = 0;
    virtual void handle( const KeyUpEvent& eventIn )         = 0;
    virtual void handle( const TextEvent& eventIn )          = 0;
    virtual void handle( const FocusEvent& eventIn )         = 0;
    virtual void handle( const BlurEvent& eventIn )          = 0;
    virtual void handle( const MoveEvent& eventIn )          = 0;
    virtual void handle( const ResizeEvent& eventIn )        = 0;
    virtual void handle( const CloseEvent& eventIn )         = 0;
    virtual void handle( const ClickEvent& eventIn )         = 0;
};

class AbstractEventHandler : public virtual IEventHandler {
public:
    virtual void handle( const MouseDownEvent& eventIn );
    virtual void handle( const MouseUpEvent& eventIn );
    virtual void handle( const MouseMoveEvent& eventIn );
    virtual void handle( const MouseOverEvent& eventIn );
    virtual void handle( const MouseOutEvent& eventIn );
    virtual void handle( const MouseVScrollEvent& eventIn );
    virtual void handle( const MouseHScrollEvent& eventIn );
    virtual void handle( const KeyDownEvent& eventIn );
    virtual void handle( const KeyUpEvent& eventIn );
    virtual void handle( const TextEvent& eventIn );
    virtual void handle( const FocusEvent& eventIn );
    virtual void handle( const BlurEvent& eventIn );
    virtual void handle( const MoveEvent& eventIn );
    virtual void handle( const ResizeEvent& eventIn );
    virtual void handle( const CloseEvent& eventIn );
    virtual void handle( const ClickEvent& eventIn );
};




} //namespace Brimstone




#endif //BS_UI_EVENTS_HPP
