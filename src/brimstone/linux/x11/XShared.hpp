/*
linux/x11/XShared.hpp
---------------------
Copyright (c) 2024, theJ89

Description:
    Defines the XShared class.
    This class is not meant to be instantiated; instead, it consists of several static variables which manage X11 data shared by multiple XWindow objects.
*/
#ifndef BS_LINUX_X11_XSHARED_HPP
#define BS_LINUX_X11_XSHARED_HPP




//Includes
#include "XDisplay.hpp"      //Brimstone::Private::XDisplay
#include "XVisualInfo.hpp"   //Brimstone::Private::XVisualInfo
#include "XColormap.hpp"     //Brimstone::Private::XColormap
#include "XInputMethod.hpp"  //Brimstone::Private::XInputMethod
#include "XCursor.hpp"       //Brimstone::Private::XCursor




namespace Brimstone::Private {




class XShared {
public:
    static void init();
    static void destroy();
    static void createBlankCursor();

    static inline ::Display* getDisplay() {
        return m_display.get();
    }

    static inline int getScreen() {
        return m_screen;
    }

    static inline ::Window getRootWindow() {
        return m_rootWindow;
    }

    static inline ::XVisualInfo* getVisualInfo() {
        return m_vi.get();
    }

    static inline ::Colormap getColormap() {
        return m_colormap.get();
    }

    static inline ::XIM getInputMethod() {
        return m_inputMethod.get();
    }

    static inline ::Cursor getBlankCursor() {
        return m_blankCursor.get();
    }

private:
    static bool           m_initialized;
    static XDisplay       m_display;
    static XVisualInfo    m_vi;
    static int            m_screen;
    static ::Window       m_rootWindow;
    static XColormap      m_colormap;
    static XInputMethod   m_inputMethod;
    static XCursor        m_blankCursor;
public:
    static Atom           m_atomWmDeleteWindow;
    static Atom           m_atomNetWmName;
    static Atom           m_atomNetWmState;
    static Atom           m_atomNetWmStateFullscreen;
    static Atom           m_atomNetWmStateMaximizedVert;
    static Atom           m_atomNetWmStateMaximizedHorz;
    static Atom           m_atomNetWmStateHidden;
    static Atom           m_atomNetWmStateShaded;
    static Atom           m_atomNetFrameExtents;
    static Atom           m_atomMotifWmHints;
};




} //namespace Brimstone::Private




#endif //BS_LINUX_X11_XSHARED_HPP
