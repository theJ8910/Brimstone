#ifndef LINUX_WINDOW_HPP
#define LINUX_WINDOW_HPP




//Includes
#include <brimstone/Rectangle.hpp>              //LongRectangle
#include <brimstone/types.hpp>                  //ustring


namespace Brimstone {

class Window;

namespace Private {

class LinuxWindow {
public:
    LinuxWindow( Window& cParent );
    ~LinuxWindow();

    static bool     processEvents();
    void            setTitle( const ustring& strTitle );
    void            setPopup( const bool bPopup );
    void            setBounds( const LongRectangle& cBounds );
private:
    LinuxWindow( const LinuxWindow& );
    LinuxWindow&  operator =( const LinuxWindow& );
private:
    Window*         m_pcParent;
};

typedef LinuxWindow WindowImpl;

}
}

#endif //LINUX_WINDOW_HPP
