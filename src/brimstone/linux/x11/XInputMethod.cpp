/*
linux/x11/XInputMethod.cpp
--------------------------
Copyright (c) 2024, theJ89

Description:
    See XInputMethod.hpp for more information.
*/




//Includes
#include "XInputMethod.hpp"        //Class header
#include "XShared.hpp"             //Brimstone::Private::XShared

#include <brimstone/Exception.hpp> //Brimstone::Exception




namespace Brimstone {
namespace Private {




XInputMethod::XInputMethod() :
    m_inputMethod( nullptr ) {
}

XInputMethod::XInputMethod( const ::XIM inputMethod ) :
    m_inputMethod( inputMethod ) {
}

XInputMethod::XInputMethod( const ::XrmDatabase db, char* const res_name, char* const res_class ) {
    createInternal( db, res_name, res_class );
}

XInputMethod::XInputMethod( XInputMethod&& toMove ) :
    m_inputMethod( toMove.m_inputMethod ) {
    toMove.m_inputMethod = nullptr;
}

XInputMethod::~XInputMethod() {
    destroy();
}

void XInputMethod::create() {
    destroy();
    createInternal( nullptr, nullptr, nullptr );
}

void XInputMethod::create( const ::XrmDatabase db, char* const res_name, char* const res_class ) {
    destroy();
    createInternal( db, res_name, res_class );
}

void XInputMethod::reset( const ::XIM inputMethod ) {
    destroy();
    m_inputMethod = inputMethod;
}

::XIM XInputMethod::release() {
    ::XIM inputMethod = m_inputMethod;
    m_inputMethod = nullptr;
    return inputMethod;
}

void XInputMethod::destroy() {
    if( m_inputMethod == nullptr )
        return;

    if( XCloseIM( m_inputMethod ) == 0 )
        throw Exception( "XCloseIM() failed." );

    m_inputMethod = nullptr;
}

XInputMethod& XInputMethod::operator =( XInputMethod&& toMove ) {
    m_inputMethod = toMove.m_inputMethod;
    toMove.m_inputMethod = nullptr;
    return *this;
}

void XInputMethod::createInternal( const ::XrmDatabase db, char* const res_name, char* const res_class ) {
    m_inputMethod = XOpenIM( XShared::getDisplay(), db, res_name, res_class );
    if( m_inputMethod == nullptr )
        throw Exception( "XOpenIM() failed." );
}




}
}
