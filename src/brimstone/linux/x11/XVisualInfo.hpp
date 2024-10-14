/*
linux/x11/XVisualInfo.hpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XVisualInfo class.

    The XVisualInfo class manages an X11 XVisualInfo*, ensuring the X11 XVisualInfo* is properly cleaned up when the XVisualInfo object is destroyed.
*/
#ifndef BS_LINUX_X11_XVISUALINFO_HPP
#define BS_LINUX_X11_XVISUALINFO_HPP




//Includes
#include <X11/Xlib.h>  //X11; XVisualInfo, XFree
#include <X11/Xutil.h> //X11; XVisualInfo




namespace Brimstone {
namespace Private {




class XVisualInfo {
public:
    XVisualInfo();
    XVisualInfo( ::XVisualInfo* const vi );
    XVisualInfo( const XVisualInfo& toCopy ) = delete;
    XVisualInfo( XVisualInfo&& toMove );
    ~XVisualInfo();
    void reset( ::XVisualInfo* const vi );
    ::XVisualInfo* release();
    void destroy();

    inline ::XVisualInfo* get() const {
        return m_vi;
    }
    
    XVisualInfo& operator =( const XVisualInfo& toCopy ) = delete;
    XVisualInfo& operator =( XVisualInfo&& toMove );
private:
    ::XVisualInfo* m_vi;
};




}
}

#endif //BS_LINUX_X11_XVISUALINFO_HPP
