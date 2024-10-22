/*
linux/x11/XColorMap.hpp
-----------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XColorMap class.

    Objects of this class manage an underlying X11 Colormap object.
*/
#ifndef BS_LINUX_X11_XCOLORMAP_HPP
#define BS_LINUX_X11_XCOLORMAP_HPP




//Includes
#include <X11/Xlib.h>  //X11; Colormap, Window, XVisualInfo, XCreateColormap, XFreeColormap




namespace Brimstone::Private {




class XColormap {
public:
    XColormap();
    XColormap( const ::Colormap colormap );
    XColormap( const ::Window w, ::Visual* const visual, const int alloc );
    XColormap( const XColormap& toCopy ) = delete;
    XColormap( XColormap&& toMove );
    ~XColormap();
    void create( const ::Window w, ::Visual* const visual, const int alloc );
    void reset( const ::Colormap colormap );
    ::Cursor release();
    void destroy();

    inline ::Colormap get() const {
        return m_colormap;
    }
    
    XColormap& operator =( const XColormap& toCopy ) = delete;
    XColormap& operator =( XColormap&& toMove );
private:
    void createInternal( const ::Window w, ::Visual* const visual, const int alloc );
private:
    ::Colormap m_colormap;
};




} //namespace Brimstone::Private




#endif //BS_LINUX_X11_XCOLORMAP_HPP
