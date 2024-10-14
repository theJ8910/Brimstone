/*
linux/x11/XInputMethod.hpp
--------------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XInputMethod class.
    Objects of this class manage an underlying X11 input method object (XIM).
*/
#ifndef BS_LINUX_X11_XINPUTMETHOD_HPP
#define BS_LINUX_X11_XINPUTMETHOD_HPP




//Includes
#include <X11/Xlib.h>       //X11; XIM, XOpenIM, XCloseIM
#include <X11/Xresource.h>  //X11; XrmDatabase




namespace Brimstone {
namespace Private {




class XInputMethod {
public:
    XInputMethod();
    XInputMethod( const ::XIM inputMethod );
    XInputMethod( const ::XrmDatabase db, char* const res_name, char* const res_class );
    XInputMethod( const XInputMethod& toCopy ) = delete;
    XInputMethod( XInputMethod&& toMove );
    ~XInputMethod();
    void create();
    void create( const ::XrmDatabase db, char* const res_name, char* const res_class );
    void reset( const ::XIM inputMethod );
    ::XIM release();
    void destroy();

    inline ::XIM get() const {
        return m_inputMethod;
    }

    XInputMethod& operator =( const XInputMethod& toCopy ) = delete;
    XInputMethod& operator =( XInputMethod&& toMove );
private:
    void createInternal( const ::XrmDatabase db, char* const res_name, char* const res_class );
private:
    ::XIM m_inputMethod;
};




}
}

#endif //BS_LINUX_X11_XINPUTMETHOD_HPP
