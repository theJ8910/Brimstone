/*
linux/x11/XPixmap.cpp
---------------------
Copyright (c) 2024, theJ89

Description:
    See XPixmap.hpp for more information.
*/




//Includes
#include "XPixmap.hpp"              //Class header
#include "XShared.hpp"              //Brimstone::Private::XShared
#include "XException.hpp"           //Brimstone::Private::xerrBegin, Brimstone::Private::xerrEnd, Brimstone::Private::xerrExists, Brimstone::Private::xerrGet

#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone {
namespace Private {




XPixmap::XPixmap() :
    m_pixmap( None ) {
}

XPixmap::XPixmap( const ::Pixmap pixmap ) :
    m_pixmap( pixmap ) {
}

XPixmap::XPixmap( const ::Drawable d, char* const data, const unsigned int width, const unsigned int height ) {
    createInternal( d, data, width, height );
}

XPixmap::XPixmap( XPixmap&& toMove ) :
    m_pixmap( toMove.m_pixmap ) {
    toMove.m_pixmap = None;
}

XPixmap::~XPixmap() {
    destroy();
}

void XPixmap::create( const ::Drawable d, char* const data, const unsigned int width, const unsigned int height ) {
    destroy();
    createInternal( d, data, width, height );
}

void XPixmap::reset( const ::Pixmap pixmap ) {
    destroy();
    m_pixmap = pixmap;
}

::Pixmap XPixmap::release() {
    ::Pixmap pixmap = m_pixmap;
    m_pixmap = None;
    return pixmap;
}

void XPixmap::destroy() {
    if( m_pixmap == None )
        return;
    
    xerrBegin();
    XFreePixmap( XShared::getDisplay(), m_pixmap );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    m_pixmap = None;
}

XPixmap& XPixmap::operator =( XPixmap&& toMove ) {
    m_pixmap = toMove.m_pixmap;
    toMove.m_pixmap = None;
    return *this;
}

void XPixmap::createInternal( const ::Drawable d, char* const data, const unsigned int width, const unsigned int height ) {
    xerrBegin();
    m_pixmap = XCreateBitmapFromData( XShared::getDisplay(), d, data, width, height );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    if( m_pixmap == None )
        throw Exception( "XCreateBitmapFromData() failed." );
}




}
}
