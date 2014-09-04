/*
Systems.hpp
-----------------------
Copyright (c) 2014, theJ89
Based on code written by Zachary Blystone.

Description:
    Systems provides a framework for registering, instantiating, and managing the various modules (systems)
    that make up the engine and game.
*/
#ifndef BS_SYSTEMS_HPP
#define BS_SYSTEMS_HPP




//Includes
#include <unordered_map>                        //std::unordered_map
#include <unordered_set>                        //std::unordered_set
#include <vector>                               //std::vector
#include <mutex>                                //std::recursive_mutex, std::lock_guard

#include <brimstone/factory/FactoryManager.hpp> //FactoryManager
#include <brimstone/factory/BasicFactory.hpp>   //BasicFactory
#include <brimstone/util/Misc.hpp>              //BS_MAKE_ENUM_HASHER




//Macros
/*
BS_REGISTER_SYSTEM
-----------------------

Description:
    Registers a system with Systems.

    IMPORTANT:
        This macro should only be invoked in a .cpp file to avoid multiple declarations of the same system.

    Creates a factory for the class with the given name, instantiates it, then registers the factory with System's
    factory manager under the given system type.

Arguments:
    className:  Name of the class to register. The class must implement ISystem.
    type:       The type of system being registered. Must be an enum in SystemType.
*/
#define BS_REGISTER_SYSTEM( className, type ) \
    BS_MAKE_FACTORY( ::Brimstone::ISystem, className, ::Brimstone::Systems::getFactoryManager(), ::Brimstone::SystemType::type );




namespace Brimstone {

enum class SystemType {
    SCHEDULER, GAME, INPUT, GRAPHICS, AUDIO, PHYSICS, LUA
};

}

BS_MAKE_ENUM_HASHER( Brimstone::SystemType );

namespace Brimstone {

class ISystem {
protected:
    typedef std::unordered_set< SystemType > DependencySet;
public:
    virtual void start()                           = 0;
    virtual void stop()                            = 0;

    virtual void preframe()                        = 0;
    virtual void frame()                           = 0;
    virtual void postframe()                       = 0;

    virtual void wait()                            = 0;

    virtual const DependencySet& getDependencies() = 0;
protected:

};

class AbstractSystem : public ISystem {
public:
    virtual const DependencySet& getDependencies();
    void wait();
protected:
    void addDependency( SystemType type );
protected:
    std::mutex m_mutex;
private:
    DependencySet m_dependencies;
};

class Systems {
private:
    typedef std::unordered_map< SystemType, ISystem* >  SystemsMap;
    typedef std::vector< ISystem* >                     SystemsStack;
    typedef std::unordered_set< SystemType >            SystemTypeSet;
    typedef std::lock_guard< std::recursive_mutex >     LockGuard;
public:
    static void add( SystemType type );

    static void start();
    static void stop();

    static void frame();
    static void join();
private:
    static std::recursive_mutex m_systemsMutex;
    static SystemsMap           m_systemsByType;
    static SystemsStack         m_systemsByLoadOrder;
public:
    static FactoryManager< SystemType, ISystem* >& getFactoryManager();
};

}




#endif //BS_SYSTEMS_HPP
