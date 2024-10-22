/*
linux/x11/XVisualInfo.cpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    See XVisualInfo.hpp for more information.
*/




//Includes
#include "XVisualInfo.hpp"  //Header




namespace Brimstone::Private {




XVisualInfo::XVisualInfo() :
    m_vi( nullptr ) {    
}

XVisualInfo::XVisualInfo( ::XVisualInfo* const vi ) :
    m_vi( vi ) {
}

XVisualInfo::XVisualInfo( XVisualInfo&& toMove ) :
    m_vi( toMove.m_vi ) {
    toMove.m_vi = nullptr;
}

XVisualInfo::~XVisualInfo() {
    destroy();
}

void XVisualInfo::reset( ::XVisualInfo* const vi ) {
    destroy();
    m_vi = vi;
}

::XVisualInfo* XVisualInfo::release() {
    ::XVisualInfo* vi = m_vi;
    m_vi = nullptr;
    return vi;
}

void XVisualInfo::destroy() {
    if( m_vi == nullptr )
        return;

    XFree( m_vi );

    m_vi = nullptr;
}

XVisualInfo& XVisualInfo::operator =( XVisualInfo&& toMove ) {
    m_vi = toMove.m_vi;
    toMove.m_vi = nullptr;
    return *this;
}




} //namespace Brimstone::Private
