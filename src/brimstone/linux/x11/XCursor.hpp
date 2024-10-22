/*
linux/x11/XCursor.hpp
---------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XCursor class.

    Objects of this class manage an underlying X11 Cursor object.
*/
#ifndef BS_LINUX_X11_XCURSOR_HPP
#define BS_LINUX_X11_XCURSOR_HPP




//Includes
#include <X11/Xlib.h>  //X11; Cursor, Pixmap, XCreatePixmapCursor, XFreeCursor




namespace Brimstone::Private {




class XCursor {
public:
    XCursor();
    XCursor( const ::Cursor cursor );
    XCursor( ::Pixmap source, ::Pixmap mask, ::XColor* const foreground_color, ::XColor* const background_color, unsigned int x, unsigned int y );
    XCursor( const XCursor& toCopy ) = delete;
    XCursor( XCursor&& toMove );
    ~XCursor();
    void create( ::Pixmap source, ::Pixmap mask, ::XColor* const foreground_color, ::XColor* const background_color, unsigned int x, unsigned int y );
    void reset( const ::Cursor cursor );
    ::Cursor release();
    void destroy();

    inline ::Cursor get() const {
        return m_cursor;
    }
    
    XCursor& operator =( const XCursor& toCopy ) = delete;
    XCursor& operator =( XCursor&& toMove );
private:
    void createInternal( ::Pixmap source, ::Pixmap mask, ::XColor* const foreground_color, ::XColor* const background_color, unsigned int x, unsigned int y );
private:
    ::Cursor m_cursor;
};




} //namespace Brimstone::Private




#endif //BS_LINUX_X11_XCURSOR_HPP
