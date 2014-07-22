/*
Lua.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Defines a system that manages instances of Lua.
    You can call newInstance() to create a new Lua instance.
*/
#ifndef BS_LUA_HPP
#define BS_LUA_HPP




//Includes
#include <unordered_map>                //std::unordered_map
#include <memory>                       //std::shared_ptr, std::weak_ptr

#include <brimstone/Systems.hpp>        //AbstractSystem
#include <brimstone/LuaInstance.hpp>    //LuaInstance




namespace Brimstone {

class Lua : public AbstractSystem {
public:
    Lua();
    ~Lua();

    virtual void start();
    virtual void stop();

    std::weak_ptr< LuaInstance > newInstance();
private:
    std::vector< std::shared_ptr< LuaInstance > > m_instances;
};

}




#endif //BS_LUA_HPP