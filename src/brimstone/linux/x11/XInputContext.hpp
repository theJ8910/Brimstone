/*
linux/x11/XInputContext.hpp
---------------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XInputContext class.
    Objects of this class manage an underlying X11 input context object (XIC).
*/
#ifndef BS_LINUX_X11_XINPUTCONTEXT_HPP
#define BS_LINUX_X11_XINPUTCONTEXT_HPP




//Includes
#include <X11/Xlib.h>               //X11; XIC, XIM, XCreateIC, XDestroyIC
#include "XException.hpp"           //Brimstone::Private::xerrBegin, Brimstone::Private::xerrEnd, Brimstone::Private::xerrExists, Brimstone::Private::xerrGet

#include <brimstone/Exception.hpp>  //Brimstone::Exception




namespace Brimstone::Private {




class XInputContext {
public:
    XInputContext();
    XInputContext( const ::XIC inputContext );
    template< typename... Args >
    XInputContext( ::XIM im, Args... args );
    XInputContext( const XInputContext& toCopy ) = delete;
    XInputContext( XInputContext&& toMove );
    ~XInputContext();
    template< typename... Args >
    void create( ::XIM im, Args... args );
    void reset( const ::XIC inputContext );
    ::XIC release();
    void destroy();

    inline ::XIC get() const {
        return m_inputContext;
    }

    XInputContext& operator =( const XInputContext& toCopy ) = delete;
    XInputContext& operator =( XInputContext&& toMove );
private:
    template< typename... Args >
    void createInternal( ::XIM im, Args... args );
private:
    ::XIC m_inputContext;
};

/*
XInputContext::XInputContext{3}
-------------------------------

Description:
    Constructor. Creates a new X11 input context and sets the XInputContext up to manage it.

Arguments:
    im:    An X11 input method to be passed to XCreateIC.
    args:  Variadic arguments to be passed to XCreateIC.

Returns:
    N/A
*/
template< typename... Args >
XInputContext::XInputContext( ::XIM im, Args... args ) {
    createInternal( im, args... );
}

/*
XInputContext::create
---------------------

Description:
    Creates a new X11 input context and sets the XInputContext up to manage it.
    If there is already an input context being managed, it will be destroyed beforehand.

Arguments:
    im:    An X11 input method to be passed to XCreateIC.
    args:  Variadic arguments to be passed to XCreateIC.

Returns:
    N/A
*/
template< typename... Args >
void XInputContext::create( ::XIM im, Args... args ) {
    destroy();
    createInternal( im, args... );
}

/*
XInputContext::createInternal
-----------------------------

Description:
    Internal function that handles the actual creation of new input contexts and setting them up to be managed.

Arguments:
    im:    An X11 input method to be passed to XCreateIC.
    args:  Variadic arguments to be passed to XCreateIC.

Returns:
    N/A
*/
template< typename... Args >
void XInputContext::createInternal( ::XIM im, Args... args ) {
    xerrBegin();
    m_inputContext = XCreateIC(
        im,
        args...,
        nullptr
    );
    xerrEnd();
    if( xerrExists() )
        throw xerrGet();

    if( m_inputContext == nullptr )
        throw Exception( "XCreateIC() failed." );
}




} //namespace Brimstone::Private




#endif //BS_LINUX_X11_XINPUTCONTEXT_HPP
