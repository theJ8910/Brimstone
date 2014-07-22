/*
Lua.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See Lua.hpp for more information.
*/




//Includes
#include <brimstone/Lua.hpp>            //Header file




namespace Brimstone {

BS_REGISTER_SYSTEM( Lua, LUA );

Lua::Lua() {
}

Lua::~Lua() {
}

void Lua::start() {
    for( auto instance : m_instances )
        instance->start();
}

void Lua::stop() {
    for( auto instance : m_instances )
        instance->stop();
}

std::weak_ptr< LuaInstance > Lua::newInstance() {
    std::shared_ptr< LuaInstance > inst( new LuaInstance() );
    inst->openLibraries();

    m_instances.push_back( inst );

    return inst;
}

}