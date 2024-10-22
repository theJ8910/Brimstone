/*
LuaInstance.cpp
---------------
Copyright (c) 2024, theJ89

Description:
    See LuaInstance.hpp for more information.
*/




//Includes
#include <brimstone/LuaInstance.hpp>    //Header
#include <brimstone/Exception.hpp>      //Brimstone::LuaException

#include <luajit/lua.hpp>               //lua_State, etc.

#include <boost/format.hpp>             //boost::format




namespace Brimstone {




LuaInstance::LuaInstance() {
    m_state = lua_open();
}

LuaInstance::~LuaInstance() {
    lua_close( m_state );
}

void LuaInstance::start() {
    if( !isFunction( "start" ) )
        return;

    pushFunction( "start" );
    callVoid();
}

void LuaInstance::stop() {
    if( !isFunction( "stop" ) )
        return;

    pushFunction( "stop" );
    callVoid();
}

void LuaInstance::preframe() {
    if( !isFunction( "preframe" ) )
        return;

    pushFunction( "preframe" );
    callVoid();
}

void LuaInstance::frame() {
    if( !isFunction( "frame" ) )
        return;

    pushFunction( "frame" );
    callVoid();
}

void LuaInstance::postframe() {
    if( !isFunction( "postframe" ) )
        return;

    pushFunction( "postframe" );
    callVoid();
}

bool LuaInstance::isFunction( const uchar* functionName ) const {
    lua_getglobal( m_state, functionName );
    bool result = lua_isfunction( m_state, -1 );
    lua_pop( m_state, 1 );

    return result;
}

/*
Load and run a script
*/
void LuaInstance::load( const uchar* filepath ) {
    if( luaL_dofile( m_state, filepath ) ) {
        LuaException e( lua_tostring( m_state, -1 ) );
        lua_pop( m_state, 1 );
        throw e;
    }
}

/*
Push a global function with the given name to the stack.
Must be done before doing call().
*/
void LuaInstance::pushFunction( const uchar* functionName ) {
    lua_getglobal( m_state, functionName );
    if( !lua_isfunction( m_state, -1 ) ) {
        LuaException e( ( boost::format( "\"%1%\" is not a global function" ) % functionName ).str() );
        lua_pop( m_state, 1 );
        throw e;
    }
}

void LuaInstance::pushStack( const bool value ) {
    lua_pushboolean( m_state, value );
}

void LuaInstance::pushStack( const intN value ) {
    lua_pushinteger( m_state, value );
}

void LuaInstance::pushStack( const float value ) {
    lua_pushnumber( m_state, value );
}

void LuaInstance::pushStack( const double value ) {
    lua_pushnumber( m_state, value );
}

void LuaInstance::pushStack( const uchar* const value ) {
    lua_pushstring( m_state, value );
}

void LuaInstance::pushStack( const ustring& value ) {
    pushStack( value.c_str() );
}

void LuaInstance::pushStack() {
    //Push nothing to stack (used when doing .call() with no arguments)
}

void LuaInstance::popStack( bool& valueOut ) {
    //lua_isboolean( m_pcState, -1 );
    valueOut = ( lua_toboolean( m_state, -1 ) > 0 );
    lua_pop( m_state, 1 );
}

void LuaInstance::popStack( intN& valueOut ) {
    //lua_isinteger( m_pcState, -1 )
    valueOut = lua_tointeger( m_state, -1 );
    lua_pop( m_state, 1 );
}

void LuaInstance::popStack( float& valueOut ) {
    valueOut = (float)lua_tonumber( m_state, -1 );
    lua_pop( m_state, 1 );
}

void LuaInstance::popStack( double& valueOut ) {
    valueOut = lua_tonumber( m_state, -1 );
    lua_pop( m_state, 1 );
}

void LuaInstance::popStack( ustring& valueOut ) {
    valueOut = lua_tostring( m_state, -1 );
    lua_pop( m_state, 1 );
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
        lua_pushcfunction( m_state, p->func );
        callVoid( p->name );
    }
}

void LuaInstance::callFunction( const int32 args, const int32 returnValues ) {
    //lua_pcall pops the function and arguments that were previously pushed to the stack
    if( lua_pcall( m_state, args, returnValues, 0 ) != 0 ) {
        LuaException e( lua_tostring( m_state, -1 ) );
        lua_pop( m_state, 1 );  //pop error message
        throw e;
    }
    //If lua_pcall completes successfully, then the stack now contains
    //the return values for that function (with the last value at the top of the stack)
}




} //namespace Brimstone
