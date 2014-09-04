/*
Audio.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Audio.hpp for more information.
*/




//Includes
#include <brimstone/Audio.hpp>          //Header file
#include <brimstone/Exception.hpp>      //SingletonException



namespace Brimstone {

BS_REGISTER_SYSTEM( Audio, AUDIO );

Audio* Audio::m_singleton = nullptr;
Audio::Audio() {
    if( m_singleton != nullptr )
        throw SingletonException();
    m_singleton = this;
}

Audio::~Audio() {
    if( m_singleton == this )
        m_singleton = nullptr;
}

void Audio::start() {

}

void Audio::stop() {

}

void Audio::preframe() {

}

void Audio::frame() {

}

void Audio::postframe() {

}

Audio* Audio::get() {
    return m_singleton;
}

}