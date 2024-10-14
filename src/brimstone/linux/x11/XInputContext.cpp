/*
linux/x11/XInputContext.cpp
---------------------------
Copyright (c) 2024, theJ89

Description:
    See XInputContext.hpp for more information.
*/




//Includes
#include "XInputContext.hpp"  //Class header
#include "XShared.hpp"        //Brimstone::Private::XShared




namespace Brimstone {
namespace Private {




/*
XInputContext::XInputContext{1}
-------------------------------

Description:
    Default constructor. Sets the XInputContext up to manage nothing.

Arguments:
    N/A

Returns:
    N/A
*/
XInputContext::XInputContext() :
    m_inputContext( nullptr ) {
}

/*
XInputContext::XInputContext{2}
-------------------------------

Description:
    Constructor. Sets the XInputContext up to manage the given X11 input context.

Arguments:
    inputContext:  The X11 input context object to manage.

Returns:
    N/A
*/
XInputContext::XInputContext( const ::XIC inputContext ) :
    m_inputContext( inputContext ) {
}

/*
XInputContext::XInputContext{4}
-------------------------------

Description:
    Move constructor. Transfers the responsibility of managing the given XInputContext's X11 input context to this XInputContext.

Arguments:
    toMove:  The XInputContext to transfer responsibility from.

Returns:
    N/A
*/
XInputContext::XInputContext( XInputContext&& toMove ) :
    m_inputContext( toMove.m_inputContext ) {
    toMove.m_inputContext = nullptr;
}

/*
XInputContext::~XInputContext
-----------------------------

Description:
    Destructor. Calls destroy().

Arguments:
    N/A

Returns:
    N/A
*/
XInputContext::~XInputContext() {
    destroy();
}

/*
XInputContext::reset
--------------------

Description:
    Sets the managed input context object with the given X11 input context object.
    If there is already an input context being managed, it will be destroyed beforehand.

Arguments:
    inputContext:  The X11 input context object to manage.

Returns:
    XIC
*/
void XInputContext::reset( const ::XIC inputContext ) {
    destroy();
    m_inputContext = inputContext;
}

/*
XInputContext::release
----------------------

Description:
    Releases this XInputContext from the responsibility of managing its X11 input context object.
    The previously managed X11 input context object is returned.

Arguments:
    N/A

Returns:
    XIC:  The X11 input context object that we were previously managing.
*/
::XIC XInputContext::release() {
    ::XIC inputContext = m_inputContext;
    m_inputContext = nullptr;
    return inputContext;
}

/*
XInputContext::destroy
----------------------

Description:
    Destroys the managed input context if one exists.
    NOTE:
        The input context must be destroyed *before* the input method it was created from. Failing to do so will result in a segmentation fault.

Arguments:
    N/A

Returns:
    N/A
*/
void XInputContext::destroy() {
    if( m_inputContext == nullptr )
        return;

    XDestroyIC( m_inputContext );

    m_inputContext = nullptr;
}

/*
XInputContext::operator =
-------------------------

Description:
    Move assignment operator. Transfers the responsibility of managing the given XInputContext's X11 input context to this XInputContext.

Arguments:
    toMove:  The XInputContext to transfer responsibility from.

Returns:
    XInputContext&:  A reference to this object.
*/
XInputContext& XInputContext::operator =( XInputContext&& toMove ) {
    m_inputContext = toMove.m_inputContext;
    toMove.m_inputContext = nullptr;
    return *this;
}




}
}
