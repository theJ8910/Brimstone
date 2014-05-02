/*
linux/LinuxWindow.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Window implementation used on the Linux platform.
    Handles the creation and management of any windows created through the engine.

    Defines Brimstone::Private::WindowImpl as Brimstone::Private::LinuxWindow.
*/




//Includes
#include <brimstone/linux/LinuxWindow.hpp>




namespace Brimstone {
namespace Private {

LinuxWindow::LinuxWindow( Window& cParent ) : m_pcParent( &cParent ) {
}

LinuxWindow::~LinuxWindow() {
}

bool LinuxWindow::processEvents() {
    return false;
}

void LinuxWindow::setTitle( const ustring& strTitle ) {
}

void LinuxWindow::setPopup( const bool bPopup ) {
}

void LinuxWindow::setBounds( const LongRectangle& cBounds ) {
}

LinuxWindow::LinuxWindow( const LinuxWindow& ) : m_pcParent( nullptr ) {
}

LinuxWindow& LinuxWindow::operator =( const LinuxWindow& ) {
    return *this;
}

}
}
