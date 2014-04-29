/*
LuaInstance.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See LuaInstance.hpp for more information.
*/




//Includes
#include <brimstone/LuaInstance.hpp>    //Class header
#include <brimstone/Exception.hpp>      //LuaException
#include <luajit/lua.hpp>               //lua_State, etc
#include <boost/format.hpp>             //boost::format




namespace Brimstone {

LuaInstance::LuaInstance() {
    m_pcState = lua_open();
}

LuaInstance::~LuaInstance() {
    lua_close( m_pcState );
}

void LuaInstance::start() {
    try {
        pushFunction( "start" );
    } catch( LuaException& ) { return; }

    callVoid();
}

void LuaInstance::stop() {
    try {
        pushFunction( "stop" );
    } catch( LuaException& ) { return; }

    callVoid();
}

/*
Load and run a script
*/
void LuaInstance::load( const uchar* pszFilepath ) {
    if( luaL_dofile( m_pcState, pszFilepath ) ) {
        LuaException e( lua_tostring( m_pcState, -1 ) );
        lua_pop( m_pcState, -1 );
        throw e;
    }
}

/*
Push a global function with the given name to the stack.
Must be done before doing call().
*/
void LuaInstance::pushFunction( const uchar* pszFunctionName ) {
    lua_getglobal( m_pcState, pszFunctionName );
    if( !lua_isfunction( m_pcState, -1 ) ) {
        LuaException e( ( boost::format( "\"%1%\" is not a global function" ) % pszFunctionName ).str() );
        lua_pop( m_pcState, -1 );
        throw e;
    }
}

void LuaInstance::pushStack( const bool b ) {
    lua_pushboolean( m_pcState, b );
}

void LuaInstance::pushStack( const int32 i ) {
    lua_pushinteger( m_pcState, i );
}

void LuaInstance::pushStack( const float f ) {
    lua_pushnumber( m_pcState, f );
}

void LuaInstance::pushStack( const double f ) {
    lua_pushnumber( m_pcState, f );
}

void LuaInstance::pushStack( const uchar* pszString ) {
    lua_pushstring( m_pcState, pszString );
}

void LuaInstance::pushStack() {
    //Push nothing to stack (used when doing .call() with no arguments)
}

void LuaInstance::popStack( bool& bOut ) {
    //lua_isboolean( m_pcState, -1 );
    bOut = ( lua_toboolean( m_pcState, -1 ) > 0 );
    lua_pop( m_pcState, -1 );
}

void LuaInstance::popStack( int32& iOut ) {
    //lua_isinteger( m_pcState, -1 )
    iOut = lua_tointeger( m_pcState, -1 );
    lua_pop( m_pcState, -1 );
}

void LuaInstance::popStack( float& fOut ) {
    fOut = (float)lua_tonumber( m_pcState, -1 );
    lua_pop( m_pcState, -1 );
}

void LuaInstance::popStack( double& fOut ) {
    fOut = lua_tonumber( m_pcState, -1 );
    lua_pop( m_pcState, -1 );
}

void LuaInstance::popStack( const uchar*& pszStringOut ) {
    pszStringOut = lua_tostring( m_pcState, -1 );
    lua_pop( m_pcState, -1 );
}

void LuaInstance::popStack() {
}

void LuaInstance::openLibraries() {
    //Limited list of libraries to load
    static const luaL_Reg acLibraries[] = {
        { "",               luaopen_base    },  //standard functions, e.g. print, load, loadfile, etc.
        { LUA_BITLIBNAME,   luaopen_bit     },  //bit operations
      //{ LUA_DBLIBNAME,    luaopen_debug   },  //debugging utilities; e.g. information about a function, access to local variables, etc
      //{ LUA_FFILIBNAME,   luaopen_ffi     },  //LuaJIT; allows binding to C functions from Lua
      //{ LUA_IOLIBNAME,    luaopen_io      },  //File I/O among other things
      //{ LUA_JITLIBNAME,   luaopen_jit     },  //LuaJIT; control JIT compilation (turn on/off from Lua, among other things)
        { LUA_MATHLIBNAME,  luaopen_math    },  //Mathematics library (sqrt, sin, cos, etc).
      //{ LUA_OSLIBNAME,    luaopen_os      },  //System date & time, delete, rename, execute, exit, etc
      //{ LUA_LOADLIBNAME,  luaopen_package },  //loadlib
        { LUA_STRLIBNAME,   luaopen_string  },  //string operations
        { LUA_TABLIBNAME,   luaopen_table   },  //table operations

        { nullptr, nullptr }                    //delimiter
    };

    //Load libraries
    for( const luaL_Reg* p = acLibraries; p->name != nullptr ; ++p ) {
        lua_pushcfunction( m_pcState, p->func );
        callVoid( p->name );
    }
}

void LuaInstance::callFunction( const int32 iArgs, const int32 iReturnValues ) {
    if( lua_pcall( m_pcState, iArgs, iReturnValues, 0 ) != 0 ) {
        LuaException e( lua_tostring( m_pcState, -1 ) );
        lua_pop( m_pcState, -1 );
        throw e;
    }
}

}