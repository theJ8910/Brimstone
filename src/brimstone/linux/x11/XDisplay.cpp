/*
linux/x11/XDisplay.cpp
----------------------
Copyright (c) 2024, theJ89

Description:
    See XDisplay.hpp for more information.
*/




//Includes
#include "XDisplay.hpp"             //Class header
#include "XException.hpp"           //Brimstone::Private::xerrBegin, Brimstone::Private::xerrEnd, Brimstone::Private::xerrExists, Brimstone::Private::xerrGet

#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone {
namespace Private {




XDisplay::XDisplay() :
    m_display( nullptr ) {    
}

XDisplay::XDisplay( ::Display* const display ) :
    m_display( display ) {
}

XDisplay::XDisplay( const std::nullptr_t /*display_name*/ ) {
    createInternal( nullptr );
}

XDisplay::XDisplay( char* const display_name ) {
    createInternal( display_name );
};

XDisplay::XDisplay( XDisplay&& toMove ) :
    m_display( toMove.m_display ) {
    toMove.m_display = nullptr;
}

XDisplay::~XDisplay() {
    destroy();
}

::Display* XDisplay::create() {
    destroy();
    return createInternal( nullptr );
}

::Display* XDisplay::create( char* const display_name ) {
    destroy();
    return createInternal( display_name );
}

void XDisplay::reset( ::Display* const display ) {
    destroy();
    m_display = display;
}

::Display* XDisplay::release() {
    ::Display* display = m_display;
    m_display = nullptr;
    return display;
}

void XDisplay::destroy() {
    if( m_display == nullptr )
        return;

    xerrBegin();
    XCloseDisplay( m_display );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    m_display = nullptr;
}

XDisplay& XDisplay::operator =( XDisplay&& toMove ) {
    m_display = toMove.m_display;
    toMove.m_display = nullptr;
    return *this;
}

::Display* XDisplay::createInternal( char* const display_name ) {
    m_display = XOpenDisplay( display_name );
    if( m_display == nullptr )
        throw Exception( "XOpenDisplay() failed." );
    return m_display;
}




}
}
