/*
Input.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Input.hpp for more information.
*/




//Includes
#include <brimstone/Input.hpp>          //Header file
#include <brimstone/Exception.hpp>      //SingletonException




namespace Brimstone {

BS_REGISTER_SYSTEM( Input, INPUT );

Input* Input::m_singleton = nullptr;
Input::Input() {
    if( m_singleton != nullptr )
        throw SingletonException();
    m_singleton = this;
}

Input::~Input() {
    if( m_singleton == this )
        m_singleton = nullptr;
}

void Input::start() {
}

void Input::stop() {
}

void Input::preframe() {
    //Read input from window
}

void Input::frame() {
}

void Input::postframe() {
}

Input* Input::get() {
    return m_singleton;
}

}