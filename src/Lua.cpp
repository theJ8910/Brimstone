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
    for( auto cInstance : m_acInstances )
        cInstance->start();
}

void Lua::stop() {
    for( auto cInstance : m_acInstances )
        cInstance->stop();
}

std::weak_ptr< LuaInstance > Lua::newInstance() {
    std::shared_ptr< LuaInstance > pcInst( new LuaInstance() );
    pcInst->openLibraries();

    m_acInstances.push_back( pcInst );

    return pcInst;
}

}