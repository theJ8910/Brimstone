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
    GAME, EVENTS, LUA
};

}

BS_MAKE_ENUM_HASHER( Brimstone::SystemType );

namespace Brimstone {

class ISystem {
protected:
    typedef std::unordered_set< SystemType > DependencySet_t;
public:
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual const DependencySet_t& getDependencies() = 0;
};

class AbstractSystem : public ISystem {
public:
    virtual const DependencySet_t& getDependencies();
protected:
    void addDependency( SystemType eType );
private:
    DependencySet_t m_aeDependencies;
};

class Systems {
private:
    typedef std::unordered_map< SystemType, ISystem* >  SystemsMap_t;
    typedef std::vector< ISystem* >                     SystemsStack_t;
    typedef std::unordered_set< SystemType >            SystemTypeSet_t;
public:
    static void add( SystemType eType );
    
    template< typename T >
    static T* get( SystemType eType );

    static void startAll();
    static void stopAll();
private:
    static void add( SystemType eType, SystemTypeSet_t& aeDependencyChain );

    static SystemsMap_t     m_acSystemsByType;
    static SystemsStack_t   m_acSystemsByLoadOrder;
public:
    static FactoryManager< SystemType, ISystem* >& getFactoryManager();
};

template< typename T >
T* Systems::get( SystemType eType ) {
    auto it = m_acSystemsByType.find( eType );
    return it != m_acSystemsByType.end() ? static_cast< T* >( it->second ) : nullptr;
}

}




#endif //BS_SYSTEMS_HPP
