/*
linux/x11/XPixmap.hpp
---------------------
Copyright (c) 2024, theJ89

Description:
    Defines a C++ wrapper class for working with X11 Pixmap objects.
*/
#ifndef BS_LINUX_X11_XPIXMAP_HPP
#define BS_LINUX_X11_XPIXMAP_HPP




//Includes
#include <X11/Xlib.h>  //X11; Pixmap, Drawable, XCreateBitmapFromData, XFreePixmap




namespace Brimstone::Private {




class XPixmap {
public:
    XPixmap();
    XPixmap( const ::Pixmap pixmap );
    XPixmap( const ::Drawable d, char* const data, const unsigned int width, const unsigned int height );
    XPixmap( const XPixmap& toCopy ) = delete;
    XPixmap( XPixmap&& toMove );
    ~XPixmap();
    void create( const ::Drawable d, char* const data, const unsigned int width, const unsigned int height );
    void reset( const ::Pixmap pixmap );
    ::Pixmap release();
    void destroy();

    inline ::Pixmap get() const {
        return m_pixmap;
    }
    
    XPixmap& operator =( const XPixmap& toCopy ) = delete;
    XPixmap& operator =( XPixmap&& toMove );
private:
    void createInternal( const ::Drawable d, char* const data, const unsigned int width, const unsigned int height );
private:
    ::Pixmap m_pixmap;
};




} //namespace Brimstone::Private




#endif //BS_LINUX_X11_XPIXMAP_HPP
