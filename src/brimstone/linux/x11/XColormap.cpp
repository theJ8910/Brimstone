/*
linux/x11/XColormap.cpp
-----------------------
Copyright (c) 2024, theJ89

Description:
    See XColormap.hpp for more information.
*/




//Includes
#include "XColormap.hpp"            //Header
#include "XShared.hpp"              //Brimstone::Private::XShared
#include "XException.hpp"           //Brimstone::Private::xerrBegin, Brimstone::Private::xerrEnd, Brimstone::Private::xerrExists, Brimstone::Private::xerrGet

#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone::Private {




XColormap::XColormap() :
    m_colormap( None ) {
}

XColormap::XColormap( const ::Colormap colormap ) :
    m_colormap( colormap ) {
}

XColormap::XColormap( const ::Window w, ::Visual* const visual, const int alloc ) {
    createInternal( w, visual, alloc );
}

XColormap::XColormap( XColormap&& toMove ) :
    m_colormap( toMove.m_colormap ) {
    toMove.m_colormap = None;
}

XColormap::~XColormap() {
    destroy();
}

void XColormap::create( const ::Window w, ::Visual* const visual, const int alloc ) {
    destroy();
    createInternal( w, visual, alloc );
}

void XColormap::reset( const ::Colormap colormap ) {
    destroy();
    m_colormap = colormap;
}

::Colormap XColormap::release() {
    ::Colormap colormap = m_colormap;
    m_colormap = None;
    return colormap;
}

void XColormap::destroy() {
    if( m_colormap == None )
        return;

    xerrBegin();
    XFreeColormap( XShared::getDisplay(), m_colormap );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    m_colormap = None;
}

XColormap& XColormap::operator =( XColormap&& toMove ) {
    m_colormap = toMove.m_colormap;
    toMove.m_colormap = None;
    return *this;
}

void XColormap::createInternal( const ::Window w, ::Visual* const visual, const int alloc ) {
    xerrBegin();
    m_colormap = XCreateColormap( XShared::getDisplay(), w, visual, alloc );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    if( m_colormap == None )
        throw Exception( "XCreateColormap() failed." );
}




} //namespace Brimstone::Private
