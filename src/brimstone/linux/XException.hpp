/*
linux/XException.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XException class.

    The XException class stores an error code.
    Calling getDescription() generates the appropriate error message corresponding to this code.
*/
#ifndef BS_LINUX_XEXCEPTION_HPP
#define BS_LINUX_XEXCEPTION_HPP




//Includes
#include <brimstone/Exception.hpp>  //Brimstone::IException

#include <X11/Xlib.h>               //X11




namespace Brimstone {
namespace Private {

class XException : public IException {
public:
    XException( Display* const display, const int code );
            int      getCode() const;
            Display* getDisplay() const;
    virtual ustring  getDescription() const;
private:
    Display* const m_display;
    const int      m_code;
};

void       xerrBegin();
void       xerrEnd();
bool       xerrExists();
XException xerrGet();

}
}

#endif //BS_LINUX_XEXCEPTION_HPP
