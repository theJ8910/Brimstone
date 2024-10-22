/*
linux/x11/XDisplay.hpp
----------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XDisplay class.

    The XDisplay class manages an X11 Display*, ensuring the display is properly cleaned up when the XDisplay object is destroyed.
*/
#ifndef BS_LINUX_X11_XDISPLAY_HPP
#define BS_LINUX_X11_XDISPLAY_HPP




//Includes
#include <cstddef>     //std::nullptr_t

#include <X11/Xlib.h>  //X11; Display, XOpenDisplay, XCloseDisplay




namespace Brimstone::Private {




class XDisplay {
public:
    XDisplay();
    XDisplay( ::Display* const display );
    XDisplay( std::nullptr_t /*display_name*/ );
    XDisplay( char* const display_name );
    XDisplay( const XDisplay& toCopy ) = delete;
    XDisplay( XDisplay&& toMove );
    ~XDisplay();
    ::Display* create();
    ::Display* create( char* const display_name );
    void reset( ::Display* const display );
    ::Display* release();
    void destroy();
    
    inline ::Display* get() const {
        return m_display;
    }

    XDisplay& operator =( const XDisplay& toCopy ) = delete;
    XDisplay& operator =( XDisplay&& toMove );
private:
    ::Display* createInternal( char* const display_name );
private:
    ::Display* m_display;
};




} //namespace Brimstone::Private




#endif //BS_LINUX_X11_XDISPLAY_HPP
