/*
ui/Events.cpp
-------------
Copyright (c) 2024, theJ89

Description:
    See Events.hpp for more information.
*/




//Includes
#include <brimstone/ui/Events.hpp>  //Header
#include <brimstone/ui/IPanel.hpp>  //Brimstone::IPanel




namespace Brimstone {




AbstractEvent::AbstractEvent( IPanel& source ) :
    m_source( &source ) {
}

IPanel& AbstractEvent::getSource() const {
    return *m_source;
}

MouseEvent::MouseEvent( IPanel& source, const Point2i& cursorPos ) :
    AbstractEvent( source ),
    m_cursorPos( cursorPos ) {
}
Point2i MouseEvent::getCursorPos() const {
    return m_cursorPos;
}

MouseButtonEvent::MouseButtonEvent( IPanel& source, const MouseButton button, const Point2i& cursorPos ) :
    MouseEvent( source, cursorPos ),
    m_button( button ) {
}
MouseButton MouseButtonEvent::getButton() const {
    return m_button;
}

MouseDownEvent::MouseDownEvent( IPanel& source, const MouseButton button, const Point2i& cursorPos ) :
    MouseButtonEvent( source, button, cursorPos ) {
}
EventType MouseDownEvent::getType() const {
    return EventType::MOUSE_DOWN;
}
void MouseDownEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

MouseUpEvent::MouseUpEvent( IPanel& source, const MouseButton button, const Point2i& cursorPos ) :
    MouseButtonEvent( source, button, cursorPos ) {
}
EventType MouseUpEvent::getType() const {
    return EventType::MOUSE_UP;
}
void MouseUpEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

MouseMoveEvent::MouseMoveEvent( IPanel& source, const Point2i& cursorPos ) :
    MouseEvent( source, cursorPos ) {
}
EventType MouseMoveEvent::getType() const {
    return EventType::MOUSE_MOVE;
}
void MouseMoveEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

MouseOverEvent::MouseOverEvent( IPanel& source, const Point2i& cursorPos ) :
    MouseEvent( source, cursorPos ) {
}
EventType MouseOverEvent::getType() const {
    return EventType::MOUSE_OVER;
}
void MouseOverEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

MouseOutEvent::MouseOutEvent( IPanel& source, const Point2i& cursorPos ) :
    MouseEvent( source, cursorPos ) {
}
EventType MouseOutEvent::getType() const {
    return EventType::MOUSE_OUT;
}
void MouseOutEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

MouseScrollEvent::MouseScrollEvent( IPanel& source, const float scrollAmount, const Point2i& cursorPos ) :
    MouseEvent( source, cursorPos ),
    m_scrollAmount( scrollAmount ) {
}

float MouseScrollEvent::getScrollAmount() const {
    //This value is almost always 1.0f or -1.0f.
    //However, this number can potentially be larger or smaller
    //based on the precision of the mousewheel being used.
    //The magnitude of this value indicates how much the wheel was scrolled.
    //The sign of the value indicates the direction the user scrolled:
    //    If scrolling vertically:
    //        +: forwards (towards the screen)
    //        -: backwards (towards the user)
    //    If scrolling horizontally:
    //        +: to the right
    //        -: to the left
    return m_scrollAmount;
}

MouseVScrollEvent::MouseVScrollEvent( IPanel& source, const float scrollAmount, const Point2i& cursorPos ) :
    MouseScrollEvent( source, scrollAmount, cursorPos ) {
}
EventType MouseVScrollEvent::getType() const {
    return EventType::MOUSE_VSCROLL;
}
void MouseVScrollEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

MouseHScrollEvent::MouseHScrollEvent( IPanel& source, const float scrollAmount, const Point2i& cursorPos ) :
    MouseScrollEvent( source, scrollAmount, cursorPos ) {
}
EventType MouseHScrollEvent::getType() const {
    return EventType::MOUSE_HSCROLL;
}
void MouseHScrollEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

KeyEvent::KeyEvent( IPanel& source, const Key key ) :
    AbstractEvent( source ),
    m_key( key ) {
}
Key KeyEvent::getKey() const {
    return m_key;
}

KeyDownEvent::KeyDownEvent( IPanel& source, const Key key ) :
    KeyEvent( source, key ) {
}
EventType KeyDownEvent::getType() const {
    return EventType::KEY_DOWN;
}
void KeyDownEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

KeyUpEvent::KeyUpEvent( IPanel& source, const Key key ) :
    KeyEvent( source, key ) {
}
EventType KeyUpEvent::getType() const {
    return EventType::KEY_UP;
}
void KeyUpEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

const uchar* TextEvent::getCharacter() const {
    return m_key;
}
EventType TextEvent::getType() const {
    return EventType::TEXT;
}
void TextEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

FocusEvent::FocusEvent( IPanel& source ) :
    AbstractEvent( source ) {
}
EventType FocusEvent::getType() const {
    return EventType::FOCUS;
}
void FocusEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

BlurEvent::BlurEvent( IPanel& source ) :
    AbstractEvent( source ) {
}
EventType BlurEvent::getType() const {
    return EventType::BLUR;
}
void BlurEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

MoveEvent::MoveEvent( IPanel& source, const Point2i& pos ) :
    AbstractEvent( source ),
    m_pos( pos ) {
}
Point2i MoveEvent::getPos() const {
    return m_pos;
}
EventType MoveEvent::getType() const {
    return EventType::MOVE;
}
void MoveEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

ResizeEvent::ResizeEvent( IPanel& source, const Size2i& size ) :
    AbstractEvent( source ),
    m_size( size ) {
}
Size2i ResizeEvent::getSize() const {
    return m_size;
}
EventType ResizeEvent::getType() const {
    return EventType::RESIZE;
}
void ResizeEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

CloseEvent::CloseEvent( IPanel& source ) :
    AbstractEvent( source ) {
}
EventType CloseEvent::getType() const {
    return EventType::CLOSE;
}
void CloseEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

ClickEvent::ClickEvent( IPanel& source ) :
    AbstractEvent( source ) {
}
EventType ClickEvent::getType() const {
    return EventType::CLICK;
}
void ClickEvent::accept( IEventHandler& visitor ) const {
    visitor.handle( *this );
}

void AbstractEventHandler::handle( const MouseDownEvent&    ) {}
void AbstractEventHandler::handle( const MouseUpEvent&      ) {}
void AbstractEventHandler::handle( const MouseMoveEvent&    ) {}
void AbstractEventHandler::handle( const MouseOverEvent&    ) {}
void AbstractEventHandler::handle( const MouseOutEvent&     ) {}
void AbstractEventHandler::handle( const MouseVScrollEvent& ) {}
void AbstractEventHandler::handle( const MouseHScrollEvent& ) {}
void AbstractEventHandler::handle( const KeyDownEvent&      ) {}
void AbstractEventHandler::handle( const KeyUpEvent&        ) {}
void AbstractEventHandler::handle( const TextEvent&         ) {}
void AbstractEventHandler::handle( const FocusEvent&        ) {}
void AbstractEventHandler::handle( const BlurEvent&         ) {}
void AbstractEventHandler::handle( const MoveEvent&         ) {}
void AbstractEventHandler::handle( const ResizeEvent&       ) {}
void AbstractEventHandler::handle( const CloseEvent&        ) {}
void AbstractEventHandler::handle( const ClickEvent&        ) {}




}
