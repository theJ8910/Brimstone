/*
linux/x11/XCursor.cpp
---------------------
Copyright (c) 2024, theJ89

Description:
    See XCursor.hpp for more information.
*/




//Includes
#include "XCursor.hpp"              //Header
#include "XShared.hpp"              //Brimstone::Private::XShared
#include "XException.hpp"           //Brimstone::Private::xerrBegin, Brimstone::Private::xerrEnd, Brimstone::Private::xerrExists, Brimstone::Private::xerrGet

#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone::Private {




XCursor::XCursor() :
    m_cursor( None ) {
}

XCursor::XCursor( const ::Cursor cursor ) :
    m_cursor( cursor ) {
};

XCursor::XCursor( ::Pixmap source, ::Pixmap mask, ::XColor* const foreground_color, ::XColor* const background_color, unsigned int x, unsigned int y ) {
    createInternal( source, mask, foreground_color, background_color, x, y );
}

XCursor::XCursor( XCursor&& toMove ) :
    m_cursor( toMove.m_cursor ) {
    toMove.m_cursor = None;
}

XCursor::~XCursor() {
    destroy();
}

void XCursor::create( ::Pixmap source, ::Pixmap mask, ::XColor* const foreground_color, ::XColor* const background_color, unsigned int x, unsigned int y ) {
    destroy();
    createInternal( source, mask, foreground_color, background_color, x, y );
}

void XCursor::reset( const ::Cursor cursor ) {
    destroy();
    m_cursor = cursor;
}

::Cursor XCursor::release() {
    ::Cursor cursor = m_cursor;
    m_cursor = None;
    return cursor;
}

/*
XCursor::destroy
----------------

Description:
    Frees memory for the managed X11 Cursor if any was allocated.

Arguments:
    N/A

Returns:
    N/A
*/
void XCursor::destroy() {
    if( m_cursor == None )
        return;

    xerrBegin();
    XFreeCursor( XShared::getDisplay(), m_cursor );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    m_cursor = None;
}

XCursor& XCursor::operator =( XCursor&& toMove ) {
    m_cursor = toMove.m_cursor;
    toMove.m_cursor = None;
    return *this;
}

void XCursor::createInternal( ::Pixmap source, ::Pixmap mask, ::XColor* const foreground_color, ::XColor* const background_color, unsigned int x, unsigned int y ) {
    xerrBegin();
    m_cursor = XCreatePixmapCursor( XShared::getDisplay(), source, mask, foreground_color, background_color, x, y );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    if( m_cursor == None )
        throw Exception( "XCreatePixmapCursor() failed." );
}




} //namespace Brimstone::Private
