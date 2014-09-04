/*
Lua.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Lua.hpp for more information.
*/




//Includes
#include <brimstone/Lua.hpp>            //Header file
#include <brimstone/Exception.hpp>      //SingletonException




namespace Brimstone {

BS_REGISTER_SYSTEM( Lua, LUA );

Lua* Lua::m_singleton = nullptr;
Lua::Lua() :
    m_up( false ) {
    if( m_singleton != nullptr )
        throw SingletonException();
    m_singleton = this;
}

Lua::~Lua() {
    if( m_singleton == this )
        m_singleton = nullptr;
}

void Lua::start() {
    for( auto instance : m_instances )
        instance->start();

    m_up = true;
}

void Lua::stop() {
    m_up = false;

    for( auto instance : m_instances )
        instance->stop();
}

void Lua::preframe() {
    for( auto instance : m_instances )
        instance->preframe();
}

void Lua::frame() {
    for( auto instance : m_instances )
        instance->frame();
}

void Lua::postframe() {
    for( auto instance : m_instances )
        instance->postframe();
}

std::weak_ptr< LuaInstance > Lua::newInstance() {
    std::shared_ptr< LuaInstance > inst( new LuaInstance() );
    inst->openLibraries();

    if( m_up )
        inst->start();

    m_instances.push_back( inst );

    return inst;
}

Lua* Lua::get() {
    return m_singleton;
}

}