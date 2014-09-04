/*
Graphics.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Graphics.hpp for more information.
*/




//Includes
#include <brimstone/Graphics.hpp>       //Header file
#include <brimstone/Exception.hpp>      //SingletonException



namespace Brimstone {

BS_REGISTER_SYSTEM( Graphics, GRAPHICS );

Graphics* Graphics::m_singleton = nullptr;
Graphics::Graphics() {
    if( m_singleton != nullptr )
        throw SingletonException();
    m_singleton = this;
}

Graphics::~Graphics() {
    if( m_singleton == this )
        m_singleton = nullptr;
}

void Graphics::start() {
}

void Graphics::stop() {
}

void Graphics::preframe() {
}

void Graphics::frame() {
}

void Graphics::postframe() {
}

Graphics* Graphics::get() {
    return m_singleton;
}

}